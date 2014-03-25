/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if_packet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"

#include "csr_ip_thread.h"
#include "csr_ip_handler.h"
#include "csr_ip_util.h"

#include "csr_ip_ether_sef.h"
#include "csr_ip_ifconfig_sef.h"
#include "csr_ip_socket_sef.h"
#ifdef CSR_IP_SUPPORT_TLS
#include "csr_ip_tls_sef.h"
#endif

#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"

#include "csr_log.h"
#include "csr_log_text_2.h"

void *ipTaskThreadFunction(void *arg)
{
    CsrIpInstanceData *instanceData = (CsrIpInstanceData *) arg;

    CSR_LOG_TEXT_INFO((CsrIpLto, 0, "socket thread started"));

    while (1)
    {
        int ready, maxfd;
        fd_set rset;
        fd_set wset;

        CsrIpLock(instanceData);
        rset = instanceData->rsetAll;
        wset = instanceData->wsetAll;
        maxfd = instanceData->maxfd;
        CsrIpUnlock(instanceData);

        /*
         * Note:  We use the return value to prevent iterating over all
         * sockets and ether interfaces by decrementing it for every fd
         * that is FD_ISSET().
         */
        ready = select(maxfd + 1, &rset, &wset, NULL, NULL);

        CSR_IP_SOCKET_ASSERT(ready >= 0, "select failed");

        if (FD_ISSET(instanceData->controlPipe[0], &rset))
        {
            CsrIpThreadCmd cmd;

            ready--;

            if (read(instanceData->controlPipe[0], (char *) &cmd, 1) != 1)
            {
                CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "Failed to read from controlPipe"));
            }
            else if (cmd == THREAD_CMD_RESCHEDULE)
            {
                /*
                 * Someone wanted us to update the fd sets we're
                 * waiting on.
                 * We still have to check if any other fds are
                 * live, so this is a noop instead of a continue.
                 */
            }
            else if (cmd == THREAD_CMD_REAP)
            {
                int i;

                i = 0;

                CsrIpLock(instanceData);

                for (i = 0;
                     i < MAX_SOCKETS;
                     i++)
                {
                    if (instanceData->socketInstance[i])
                    {
                        if (instanceData->socketInstance[i]->reap)
                        {
                            csrIpSocketInstFree(instanceData,
                                instanceData->socketInstance[i]);
                        }
                    }
                }

                for (i = 0;
                     i < MAX_ETHERS;
                     i++)
                {
                    if (instanceData->etherInstance[i])
                    {
                        if (instanceData->etherInstance[i]->reap)
                        {
                            csrIpEtherRemove(instanceData, i);
                        }
                    }
                }

#ifdef CSR_IP_SUPPORT_TLS
                csrIpTlsSocketsReap(&instanceData->tlsInstance);
#endif

                CsrIpUnlock(instanceData);
            }
            else if (cmd == THREAD_CMD_IFCONFIG_DEFERRED)
            {
                csrIpIfconfigDeferredProcess(instanceData);
            }
            else
            {
                /*
                 * (cmd == THREAD_CMD_TERMINATE) or something
                 * is broken.  Either way: quit
                 */
                CSR_LOG_TEXT_INFO((CsrIpLto, 0, "socket thread stopped"));
                return 0;
            }
        }

        CsrIpLock(instanceData);

        if (ready)
        {
            csrIpSocketHandleSockets(instanceData, &ready, &rset, &wset);
        }

#ifdef CSR_IP_SUPPORT_TLS
        if (ready)
        {
            csrIpTlsSocketsHandle(&instanceData->tlsInstance,
                &ready, &rset, &wset);
        }
#endif

        if (ready)
        {
            csrIpEtherHandleEthers(instanceData, &ready, &rset);
        }

        CsrIpUnlock(instanceData);

        if (FD_ISSET(instanceData->rtSocketListen, &rset))
        {
            csrIpIfconfigReadRoute(instanceData);

            CSR_IP_SOCKET_ASSERT(ready == 1,
                "bogus ready fd count: expected 1");
        }
        else
        {
            CSR_IP_SOCKET_ASSERT(ready == 0,
                "bogus ready fd count: expected 0");
        }
    }
}

void CsrIpThreadInit(CsrIpInstanceData *instanceData)
{
    struct sockaddr_nl sa;
    int rc;

    struct
    {
        struct nlmsghdr nlh;
        struct rtgenmsg g;
    } req;

    rc = pipe(instanceData->controlPipe);
    CSR_IP_SOCKET_ASSERT(rc == 0, "failed creating control pipe");

    /*
     * Sockets
     *
     *  Listen: used for listening to address and link changes
     *  Query:  used for querying interfaces etc.
     *
     */
    instanceData->ipSocketQuery = socket(AF_INET, SOCK_DGRAM, 0);
    CSR_IP_SOCKET_ASSERT(instanceData->ipSocketQuery != -1,
        "failed creating ip query socket");

    instanceData->rtSocketListen = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    CSR_IP_SOCKET_ASSERT(instanceData->rtSocketListen != -1, "failed creating routing listen socket");

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;

    rc = bind(instanceData->rtSocketListen, (struct sockaddr *) &sa, sizeof(sa));
    CSR_IP_SOCKET_ASSERT(rc != -1, "failed binding routing socket");

    /* Request dump of network interfaces. */
    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = sizeof(req);
    req.nlh.nlmsg_type = RTM_GETLINK;
    req.nlh.nlmsg_flags = NLM_F_ROOT | NLM_F_MATCH | NLM_F_REQUEST;
    req.nlh.nlmsg_pid = 0;
    req.nlh.nlmsg_seq = 0;
    req.g.rtgen_family = AF_UNSPEC;

    /*
     * Write a request on the Listen socket so the results
     * are parsed in the regular path.
     */
    rc = write(instanceData->rtSocketListen, (void *) &req, sizeof(req));
    CSR_IP_SOCKET_ASSERT(rc == sizeof(req), "network interface list request failed");

    FD_ZERO(&instanceData->rsetAll);
    FD_ZERO(&instanceData->wsetAll);

    FD_SET(instanceData->controlPipe[0], &instanceData->rsetAll);
    FD_SET(instanceData->rtSocketListen, &instanceData->rsetAll);

    instanceData->maxfd = CSRMAX(instanceData->controlPipe[0], instanceData->rtSocketListen);

    rc = pthread_create(&instanceData->tid, NULL, ipTaskThreadFunction,
        (void *) instanceData);
    CSR_IP_SOCKET_ASSERT(rc == 0, "couldn't create IP thread");
}
