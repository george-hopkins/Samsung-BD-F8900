/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_result.h"
#include "csr_ip_task.h"
#include "csr_ip_util.h"
#include "csr_ip_handler.h"
#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_sef.h"
#include "csr_ip_socket_lib.h"
#include "csr_log_text_2.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/raw.h"
#include "lwip/igmp.h"
#include "netif/etharp.h"

static void tcpTrySend(CsrIpSocketInstanceData *socketInstance);

static CsrResult socketHandleAllocate(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *socketInstance, CsrUint16 *socketHandle)
{
    for (*socketHandle = 0; *socketHandle < CSR_IP_MAX_SOCKETS; ++(*socketHandle))
    {
        if (instanceData->socketInstance[*socketHandle] == NULL)
        {
            instanceData->socketInstance[*socketHandle] = socketInstance;
            instanceData->socketInstance[*socketHandle]->connectReq = FALSE;
            instanceData->socketInstance[*socketHandle]->currentMsg = NULL;
            instanceData->socketInstance[*socketHandle]->currentMsgIndex = 0;
            instanceData->socketInstance[*socketHandle]->state = CSR_IP_SOCKET_STATE_OPEN;
            instanceData->socketInstance[*socketHandle]->timer = CSR_SCHED_TID_INVALID;
            instanceData->socketInstance[*socketHandle]->rxQueue = NULL;
            return CSR_RESULT_SUCCESS;
        }
    }
    return CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS;
}

static CsrIpSocketInstanceData *socketInstanceRetrieve(CsrIpInstanceData *instanceData, CsrUint16 socketHandle)
{
    CsrIpSocketInstanceData *returner = NULL;
    if (socketHandle < CSR_IP_MAX_SOCKETS)
    {
        returner = instanceData->socketInstance[socketHandle];
    }
    return returner;
}

static void tcpSendRetry(CsrUint16 unused, void *arg)
{
    CsrIpSocketInstanceData *socketInstance = arg;

    socketInstance->timer = CSR_SCHED_TID_INVALID;

    tcpTrySend(socketInstance);
}

static void tcpTrySend(CsrIpSocketInstanceData *socketInstance)
{
    CsrUint16 buffSizeAvail;

    buffSizeAvail = tcp_sndbuf(socketInstance->pcb.tcp);

    if (buffSizeAvail > 0)
    {
        CsrUint8 *data;
        CsrUint16 datalen;
        err_t err;

        data = socketInstance->currentMsg->data + socketInstance->currentMsgIndex;
        datalen = socketInstance->currentMsg->dataLength - socketInstance->currentMsgIndex;

        datalen = CSRMIN(datalen, buffSizeAvail);

        /* We can send everything in one go. */
        err = tcp_write(socketInstance->pcb.tcp,
            data,
            datalen, TCP_WRITE_FLAG_COPY);

        if (err)
        {
            if (ERR_IS_FATAL(err))
            {
                CsrPmemFree(socketInstance->currentMsg->data);
                CsrPmemFree(socketInstance->currentMsg);
                socketInstance->currentMsg = NULL;

                CsrIpSocketTcpDataCfmSend2(socketInstance->queue, socketInstance->socketHandle, CSR_RESULT_FAILURE);
            }
            else
            {
                /* Reschedule */
                CsrSchedTimerCancel(socketInstance->timer, NULL, NULL);
                socketInstance->timer = CsrSchedTimerSet(100000, tcpSendRetry, 0, socketInstance);
            }
        }
        else
        {
            socketInstance->currentMsgIndex += datalen;

            tcp_output(socketInstance->pcb.tcp);

            if (socketInstance->currentMsgIndex == socketInstance->currentMsg->dataLength)
            {
                CsrPmemFree(socketInstance->currentMsg->data);
                CsrPmemFree(socketInstance->currentMsg);

                socketInstance->currentMsg = NULL;

                CsrSchedTimerCancel(socketInstance->timer, NULL, NULL);

                CsrIpSocketTcpDataCfmSend2(socketInstance->queue, socketInstance->socketHandle, CSR_RESULT_SUCCESS);
            }
        }
    }
    else
    {
        /*
         * No buffer space -- we can't do anything
         * until we get a window update from our peer.
         */
    }
}

static void tcpErrf(void *arg, err_t err)
{
    CsrIpSocketInstanceData *socketInstance = arg;
    if (socketInstance != NULL)
    {
        if (socketInstance->pcb.tcp != NULL)
        {
            if (err == ERR_RST)
            {
                if (socketInstance->connectReq == TRUE)
                {
                    CsrIpSocketTcpConnectCfmSend2(socketInstance->queue,
                        socketInstance->socketHandle,
                        CSR_RESULT_FAILURE);
                    socketInstance->instanceData->socketInstance[socketInstance->socketHandle] = NULL;
                    CsrPmemFree(socketInstance);
                }
                else if (socketInstance->pendingDataResSize > 0)
                {
                    socketInstance->state = CSR_IP_SOCKET_STATE_CLOSING;
                    socketInstance->pcb.tcp = NULL;
                }
                else
                {
                    CsrIpSocketTcpCloseIndSend2(socketInstance->queue, socketInstance->socketHandle);
                    CsrIpSocketInstanceDestroy(socketInstance->instanceData, socketInstance->socketHandle);
                }
            }
            else if (err == ERR_ABRT)
            {
                CsrIpSocketInstanceDestroy(socketInstance->instanceData, socketInstance->socketHandle);
            }
        }
    }
}

static err_t tcpSent(void *arg, struct tcp_pcb *pcb, u16_t unused)
{
    CsrIpSocketInstanceData *socketInstance = arg;

    if (socketInstance->currentMsg != NULL)
    {
        CsrSchedTimerCancel(socketInstance->timer, NULL, NULL);
        socketInstance->timer = CSR_SCHED_TID_INVALID;

        tcpTrySend(socketInstance);
    }

    return ERR_OK;
}

static void tcpDataIndSend(CsrIpSocketInstanceData *socketInstance, struct pbuf *p)
{
    CsrUint8 *buf;
    CsrUint16 offset;
    CsrUint16 bufLen, remainder;

    bufLen = 0xffff;
    bufLen = CSRMIN(bufLen, p->tot_len);

    remainder = bufLen;

    buf = CsrPmemAlloc(bufLen);

    offset = 0;

    while (p != NULL)
    {
        CsrUint16 copybytes;

        copybytes = CSRMIN(p->len, remainder);

        CsrMemCpy(buf + offset, p->payload, copybytes);

        remainder -= copybytes;
        offset += copybytes;

        if (copybytes == p->len)
        {
            /*
             * If we empty the pbuf element, dechain
             * and free it.
             */
            struct pbuf *q;

            q = p->next;
            p->next = NULL;

            pbuf_free(p);

            p = q;
        }
        else
        {
            /*
             * Adjust the pbuf data pointer to account
             * for the data that has already been read.
             * Having done so, bail out of the loop and
             * save the remaining pbuf for later.
             */
            pbuf_header(p, (CsrUint16) (-copybytes));

            socketInstance->rxQueue = p;

            break;
        }
    }

    socketInstance->pendingDataResSize = bufLen;

    CsrIpSocketTcpDataIndSend2(socketInstance->queue,
        socketInstance->socketHandle,
        bufLen,
        buf);
}

static err_t tcpDataInd(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    CsrIpSocketInstanceData *socketInstance = arg;

    if (p != NULL)
    {
        if (socketInstance->pendingDataResSize > 0)
        {
            /* Still waiting for response, queue the pbuf */
            if (socketInstance->rxQueue != NULL)
            {
                pbuf_cat(socketInstance->rxQueue, p);
            }
            else
            {
                socketInstance->rxQueue = p;
            }
        }
        else /* Application is ready for data */
        {
            tcpDataIndSend(socketInstance, p);
        }
    }
    else if (socketInstance->pendingDataResSize > 0)
    {
        socketInstance->state = CSR_IP_SOCKET_STATE_CLOSING;
    }
    else
    {
        tcp_arg(pcb, NULL);
        tcp_recv(pcb, NULL);
        tcp_sent(pcb, NULL);
        tcp_close(pcb);
        CsrIpSocketTcpCloseIndSend2(socketInstance->queue, socketInstance->socketHandle);
        CsrIpSocketInstanceDestroy(socketInstance->instanceData, socketInstance->socketHandle);
    }

    return ERR_OK;
}

static err_t socketAcceptConnect(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    CsrIpSocketInstanceData *socketInstance = arg;
    CsrIpSocketInstanceData *newSocketInstance = CsrPmemAlloc(sizeof(CsrIpSocketInstanceData));
    CsrUint16 socketHandle = 0;

    if (socketHandleAllocate(socketInstance->instanceData, newSocketInstance, &socketHandle) != CSR_RESULT_SUCCESS)
    {
        CsrPmemFree(newSocketInstance);
        return ERR_CLSD;
    }
    else
    {
        CsrUint8 ipAddress[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        CsrIpUtilLwipToCsrIp(&(newpcb->remote_ip), ipAddress);
        newSocketInstance->pcb.tcp = newpcb;
        newSocketInstance->instanceData = socketInstance->instanceData;
        newSocketInstance->queue = socketInstance->queue;
        newSocketInstance->socketType = CSR_IP_SOCKET_TYPE_TCP;
        newSocketInstance->pendingDataResSize = 0;
        newSocketInstance->socketHandle = socketHandle;

        /* Disable Nagle by default */
        tcp_nagle_disable(newpcb);

        tcp_arg(newpcb, newSocketInstance);
        tcp_err(newpcb, tcpErrf);
        tcp_sent(newpcb, tcpSent);
        tcp_recv(newpcb, tcpDataInd);

        tcp_accepted(socketInstance->pcb.tcp);
        CsrIpSocketTcpAcceptIndSend2(newSocketInstance->queue, socketHandle,
            CSR_IP_SOCKET_FAMILY_IP4, ipAddress, newpcb->remote_port,
            socketInstance->socketHandle);

        return ERR_OK;
    }
}

static err_t socketConnect(void *arg, struct tcp_pcb *pcb, err_t err)
{
    CsrIpSocketInstanceData *socketInstance = arg;

    /* Register callback for incoming data. */
    tcp_recv(pcb, tcpDataInd);

    socketInstance->connectReq = FALSE;

    CsrIpSocketTcpConnectCfmSend2(socketInstance->queue, socketInstance->socketHandle, CSR_RESULT_SUCCESS);

    return ERR_OK;
}

static void udpDataInd(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, CsrUint16 port)
{
    CsrIpSocketInstanceData *socketInstance = arg;
    CsrUint8 ipAddress[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CsrUint8 *payload = CsrPmemAlloc(p->tot_len);
    CsrUint16 offset = 0;
    struct pbuf *q = p;

    while (q)
    {
        CsrMemCpy(payload + offset, q->payload, q->len);
        offset += q->len;
        q = q->next;
    }

    socketInstance->pcb.udp = upcb;
    CsrIpUtilLwipToCsrIp(addr, ipAddress);
    CsrIpSocketUdpDataIndSend2(socketInstance->queue,
        socketInstance->socketHandle, CSR_IP_SOCKET_FAMILY_IP4,
        ipAddress, port, (CsrUint16) p->tot_len, payload);
    pbuf_free(p);
}

static CsrUint8 rawDataInd(void *arg, struct raw_pcb *rpcb, struct pbuf *p, struct ip_addr *addr)
{
    CsrIpSocketInstanceData *socketInstance = (CsrIpSocketInstanceData *) arg;
    CsrUint8 ipAddress[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CsrUint8 *payload = CsrPmemAlloc(p->tot_len);
    CsrUint16 offset = 0;
    struct pbuf *q = p;

    while (q)
    {
        CsrMemCpy(payload + offset, q->payload, q->len);
        offset += q->len;
        q = q->next;
    }

    socketInstance->pcb.raw = rpcb;
    CsrIpUtilLwipToCsrIp(addr, ipAddress);
    CsrIpSocketRawDataIndSend2(socketInstance->queue,
        socketInstance->socketHandle, CSR_IP_SOCKET_FAMILY_IP4,
        ipAddress, (CsrUint16) p->tot_len, payload);
    return 0;
}

void CsrIpSocketInstanceDestroy(CsrIpInstanceData *instanceData, CsrUint16 socketHandle)
{
    if (socketHandle < CSR_IP_MAX_SOCKETS)
    {
        CsrIpSocketInstanceData *socketInstance = instanceData->socketInstance[socketHandle];
        if (socketInstance != NULL)
        {
            if (socketInstance->currentMsg != NULL)
            {
                CsrIpSocketTcpDataReq *request = socketInstance->currentMsg;
                CsrPmemFree(request->data);
                CsrPmemFree(request);
                CsrSchedTimerCancel(socketInstance->timer, 0, NULL);
            }
            if (socketInstance->rxQueue != NULL)
            {
                pbuf_free(socketInstance->rxQueue);
            }
            instanceData->socketInstance[socketHandle] = NULL;
            CsrPmemFree(socketInstance);
        }
    }
}

static void dnsFound(const CsrCharString *name, struct ip_addr *ipaddr, void *callback_arg)
{
    CsrSchedQid queue = *((CsrSchedQid *) callback_arg);
    CsrUint8 ipAddress[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (ipaddr)
    {
        CsrIpUtilLwipToCsrIp(ipaddr, ipAddress);
        CsrIpSocketDnsResolveNameCfmSend2(queue, CSR_IP_SOCKET_FAMILY_IP4,
            ipAddress, CsrStrDup(name), CSR_RESULT_SUCCESS);
    }
    else
    {
        CsrIpSocketDnsResolveNameCfmSend2(queue, CSR_IP_SOCKET_FAMILY_IP4,
            ipAddress, CsrStrDup(name), CSR_RESULT_FAILURE);
    }
    CsrPmemFree(callback_arg);
}

void CsrIpSocketDnsResolveNameReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketDnsResolveNameReq *request = instanceData->msg;
    struct ip_addr ipaddr = {INADDR_NONE};
    CsrSchedQid *queue = CsrPmemAlloc(sizeof(*queue));
    err_t err;
    CsrUint8 ipAddress[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    *queue = request->appHandle;

    err = dns_gethostbyname(request->name, &ipaddr, dnsFound, (void *) queue);

    if (err == ERR_OK)
    {
        CsrIpUtilLwipToCsrIp(&ipaddr, ipAddress);
        CsrIpSocketDnsResolveNameCfmSend2(*queue, CSR_IP_SOCKET_FAMILY_IP4,
            ipAddress, request->name, CSR_RESULT_SUCCESS);
        CsrPmemFree(queue);
    }
    else if (err != ERR_INPROGRESS)
    {
        CsrIpSocketDnsResolveNameCfmSend2(*queue, CSR_IP_SOCKET_FAMILY_IP4,
            ipAddress, request->name, CSR_RESULT_FAILURE);
        CsrPmemFree(queue);
    }
    else
    {
        CsrPmemFree(request->name);
    }
}

void CsrIpSocketUdpNewReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpNewReq *request = instanceData->msg;

    if (request->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
    {
        CsrIpSocketInstanceData *socketInstance = CsrPmemAlloc(sizeof(CsrIpSocketInstanceData));

        if (socketHandleAllocate(instanceData, socketInstance,
                &socketInstance->socketHandle) == CSR_RESULT_SUCCESS)
        {
            socketInstance->queue = request->appHandle;
            socketInstance->pcb.udp = udp_new();
            socketInstance->socketType = CSR_IP_SOCKET_TYPE_UDP;
            socketInstance->instanceData = instanceData;
            udp_recv(socketInstance->pcb.udp, udpDataInd, socketInstance);

            CsrIpSocketUdpNewCfmSend2(socketInstance->queue,
                socketInstance->socketHandle, CSR_RESULT_SUCCESS);
        }
        else
        {
            CsrIpSocketUdpNewCfmSend2(socketInstance->queue, 0,
                CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS);
            CsrPmemFree(socketInstance);
        }
    }
    else
    {
        CsrIpSocketUdpNewCfmSend2(request->appHandle, 0,
            CSR_IP_SOCKET_RESULT_IP6_NOT_SUPPORTED);
    }
}

void CsrIpSocketUdpBindReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpBindReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct ip_addr ipAddress = {0};

        CsrIpUtilCsrIpToLwIp(request->ipAddress, &ipAddress);
        if (udp_bind(socketInstance->pcb.udp, &ipAddress, request->port) != ERR_OK)
        {
            CsrIpSocketUdpBindCfmSend2(socketInstance->queue, request->socketHandle, CSR_IP_SOCKET_RESULT_PORT_IN_USE, request->port);
        }
        else
        {
            CsrIpSocketUdpBindCfmSend2(socketInstance->queue, request->socketHandle, CSR_RESULT_SUCCESS, socketInstance->pcb.udp->local_port);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] UdpBind failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketUdpDataReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpDataReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct pbuf *data = pbuf_alloc(PBUF_TRANSPORT, request->dataLength, PBUF_RAM);

        CsrMemCpy(data->payload, request->data, request->dataLength);
        data->next = NULL;
        data->tot_len = data->len;
        CsrPmemFree(request->data);
        request->data = NULL;

        if (socketInstance->pcb.udp->flags & UDP_FLAGS_CONNECTED)
        {
            udp_send(socketInstance->pcb.udp, data);
        }
        else
        {
            struct ip_addr ipAddress = {0};
            CsrIpUtilCsrIpToLwIp(request->ipAddress, &ipAddress);
            udp_sendto(socketInstance->pcb.udp, data, &ipAddress, request->port);
        }
        pbuf_free(data);

        CsrIpSocketUdpDataCfmSend2(socketInstance->queue, request->socketHandle);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] UdpData failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketUdpCloseReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpCloseReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        udp_remove(socketInstance->pcb.udp);
        instanceData->socketInstance[request->socketHandle] = NULL;
        CsrPmemFree(socketInstance);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] UdpClose failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketUdpMulticastSubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpMulticastSubscribeReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        CsrResult result = CSR_RESULT_SUCCESS;
        struct ip_addr interfaceIp = {0};
        struct ip_addr group = {0};

        CsrIpUtilCsrIpToLwIp(request->group, &group);
        CsrIpUtilCsrIpToLwIp(request->interfaceIp, &interfaceIp);

        if ((request->group[0] & 0xF0) != 0xE0)
        {
            CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] Cannot join invalid multicast group", request->socketHandle));
            result = CSR_RESULT_FAILURE;
        }
        igmp_joingroup(&interfaceIp, &group);

        CsrIpSocketUdpMulticastSubscribeCfmSend2(socketInstance->queue,
            request->socketHandle, CSR_IP_SOCKET_FAMILY_IP4,
            request->interfaceIp, request->group, result);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] MulticastSubscribe failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketUdpMulticastUnsubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpMulticastUnsubscribeReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        CsrResult result = CSR_RESULT_SUCCESS;
        struct ip_addr interfaceIp = {0};
        struct ip_addr group = {0};

        CsrIpUtilCsrIpToLwIp(request->group, &group);
        CsrIpUtilCsrIpToLwIp(request->interfaceIp, &interfaceIp);

        if ((request->group[0] & 0xF0) != 0xE0)
        {
            CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] Cannot leave invalid multicast group", request->socketHandle));
            result = CSR_RESULT_FAILURE;
        }
        igmp_leavegroup(&interfaceIp, &group);

        CsrIpSocketUdpMulticastUnsubscribeCfmSend2(socketInstance->queue,
            request->socketHandle, CSR_IP_SOCKET_FAMILY_IP4,
            request->interfaceIp, request->group, result);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] MulticastUnsubscribe failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketUdpMulticastInterfaceReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpMulticastInterfaceReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct ip_addr interfaceIp = {0};

        CsrIpUtilCsrIpToLwIp(request->interfaceIp, &interfaceIp);

        ip_addr_set(&socketInstance->pcb.udp->multicast_ip, &interfaceIp);

        CsrIpSocketUdpMulticastInterfaceCfmSend2(socketInstance->queue,
            request->socketHandle, CSR_RESULT_SUCCESS);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] MulticastInterface failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketTcpNewReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpNewReq *request = instanceData->msg;

    if (request->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
    {
        CsrIpSocketInstanceData *socketInstance = CsrPmemAlloc(sizeof(CsrIpSocketInstanceData));

        if ((socketHandleAllocate(instanceData, socketInstance,
                 &socketInstance->socketHandle) == CSR_RESULT_SUCCESS) &&
            ((socketInstance->pcb.tcp = tcp_new()) != NULL))
        {
            socketInstance->queue = request->appHandle;
            socketInstance->socketType = CSR_IP_SOCKET_TYPE_TCP;
            socketInstance->instanceData = instanceData;
            socketInstance->pendingDataResSize = 0;
            socketInstance->timer = CSR_SCHED_TID_INVALID;
            tcp_arg(socketInstance->pcb.tcp, socketInstance);
            tcp_err(socketInstance->pcb.tcp, tcpErrf);
            tcp_sent(socketInstance->pcb.tcp, tcpSent);
            tcp_nagle_disable(socketInstance->pcb.tcp); /* Disable Nagle by default */

            CsrIpSocketTcpNewCfmSend2(socketInstance->queue,
                socketInstance->socketHandle, CSR_RESULT_SUCCESS);
        }
        else
        {
            CsrIpSocketTcpNewCfmSend2(socketInstance->queue, 0,
                CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS);
            CsrPmemFree(socketInstance);
        }
    }
    else
    {
        CsrIpSocketTcpNewCfmSend2(request->appHandle, 0,
            CSR_IP_SOCKET_RESULT_IP6_NOT_SUPPORTED);
    }
}

void CsrIpSocketTcpBindReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpBindReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct ip_addr ipAddress = {0};

        CsrIpUtilCsrIpToLwIp(request->ipAddress, &ipAddress);
        if (tcp_bind(socketInstance->pcb.tcp, &ipAddress, request->port) != ERR_OK)
        {
            CsrIpSocketTcpBindCfmSend2(socketInstance->queue, request->socketHandle, CSR_IP_SOCKET_RESULT_PORT_IN_USE, request->port);
        }
        else
        {
            CsrIpSocketTcpBindCfmSend2(socketInstance->queue, request->socketHandle, CSR_RESULT_SUCCESS, socketInstance->pcb.tcp->local_port);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] TcpBind failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketTcpListenReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpListenReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        socketInstance->pcb.tcp = tcp_listen(socketInstance->pcb.tcp);
        socketInstance->socketType = CSR_IP_SOCKET_TYPE_TCPLISTEN;
        tcp_accept(socketInstance->pcb.tcp, socketAcceptConnect);
        CsrIpSocketTcpListenCfmSend2(socketInstance->queue, socketInstance->socketHandle, CSR_RESULT_SUCCESS);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] TcpListen failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketTcpConnectReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpConnectReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct ip_addr ipAddress = {0};
        CsrIpUtilCsrIpToLwIp(request->ipAddress, &ipAddress);
        socketInstance->connectReq = TRUE;
        tcp_connect(socketInstance->pcb.tcp, &ipAddress, request->port, socketConnect);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] TcpConnect failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketTcpDataReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpDataReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance == NULL)
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] TcpDataReq failed (invalid handle)", request->socketHandle));
        CsrPmemFree(request->data);
        request->data = NULL;
        return;
    }

    if (socketInstance->state == CSR_IP_SOCKET_STATE_CLOSING)
    {
        CsrPmemFree(request->data);
        request->data = NULL;
    }
    else
    {
        socketInstance->currentMsg = request;
        socketInstance->currentMsgIndex = 0;
        instanceData->msg = NULL;

        tcpTrySend(socketInstance);
    }
}

void CsrIpSocketTcpDataResHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpDataRes *response = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, response->socketHandle);

    if (socketInstance != NULL)
    {
        if (socketInstance->pcb.tcp != NULL)
        {
            tcp_recved(socketInstance->pcb.tcp, socketInstance->pendingDataResSize);
        }
        socketInstance->pendingDataResSize = 0;
        if (socketInstance->rxQueue != NULL)
        {
            tcpDataIndSend(socketInstance, socketInstance->rxQueue);
            socketInstance->rxQueue = NULL;
        }
        else if (socketInstance->state == CSR_IP_SOCKET_STATE_CLOSING)
        {
            if (socketInstance->pcb.tcp != NULL)
            {
                tcp_arg(socketInstance->pcb.tcp, NULL);
                tcp_recv(socketInstance->pcb.tcp, NULL);
                tcp_sent(socketInstance->pcb.tcp, NULL);
                tcp_close(socketInstance->pcb.tcp);
            }
            CsrIpSocketTcpCloseIndSend2(socketInstance->queue, socketInstance->socketHandle);
            CsrIpSocketInstanceDestroy(socketInstance->instanceData, socketInstance->socketHandle);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] TcpDataRes failed (invalid handle)", response->socketHandle));
    }
}

void CsrIpSocketTcpCloseReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpCloseReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        tcp_arg(socketInstance->pcb.tcp, NULL);
        if (socketInstance->socketType != CSR_IP_SOCKET_TYPE_TCPLISTEN)
        {
            tcp_recv(socketInstance->pcb.tcp, NULL);
            tcp_sent(socketInstance->pcb.tcp, NULL);
        }
        tcp_close(socketInstance->pcb.tcp);

        CsrIpSocketInstanceDestroy(instanceData, request->socketHandle);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] TcpClose failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketTcpAbortReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpAbortReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct tcp_pcb *tcp = socketInstance->pcb.tcp;
        CsrIpSocketInstanceDestroy(instanceData, request->socketHandle);
        tcp_abort(tcp);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] TcpAbort failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketRawNewReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketRawNewReq *request = instanceData->msg;

    if (request->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
    {
        CsrIpSocketInstanceData *socketInstance = CsrPmemAlloc(sizeof(CsrIpSocketInstanceData));

        if (socketHandleAllocate(instanceData, socketInstance,
                &socketInstance->socketHandle) == CSR_RESULT_SUCCESS)
        {
            socketInstance->queue = request->appHandle;
            socketInstance->pcb.raw = raw_new(request->protocolNumber);
            socketInstance->socketType = CSR_IP_SOCKET_TYPE_RAW;
            socketInstance->instanceData = instanceData;
            raw_recv(socketInstance->pcb.raw, rawDataInd, socketInstance);

            CsrIpSocketRawNewCfmSend2(socketInstance->queue,
                socketInstance->socketHandle, CSR_RESULT_SUCCESS);
        }
        else
        {
            CsrIpSocketRawNewCfmSend2(socketInstance->queue, 0,
                CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS);
            CsrPmemFree(socketInstance);
        }
    }
    else
    {
        CsrIpSocketRawNewCfmSend2(request->appHandle, 0,
            CSR_IP_SOCKET_RESULT_IP6_NOT_SUPPORTED);
    }
}

void CsrIpSocketRawBindReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketRawBindReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct ip_addr ipAddress = {0};
        CsrIpUtilCsrIpToLwIp(request->ipAddress, &ipAddress);
        raw_bind(socketInstance->pcb.raw, &ipAddress);
        CsrIpSocketRawBindCfmSend2(socketInstance->queue, request->socketHandle, CSR_RESULT_SUCCESS);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] RawBind failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketRawDataReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketRawDataReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        struct pbuf *data = pbuf_alloc(PBUF_TRANSPORT, request->dataLength, PBUF_RAM);
        struct ip_addr ipAddress = {0};

        CsrMemCpy(data->payload, request->data, request->dataLength);
        data->next = NULL;
        data->tot_len = data->len;
        CsrPmemFree(request->data);
        request->data = NULL;
        CsrIpUtilCsrIpToLwIp(request->ipAddress, &ipAddress);
        raw_sendto(socketInstance->pcb.raw, data, &ipAddress);
        pbuf_free(data);
        CsrIpSocketRawDataCfmSend2(socketInstance->queue, request->socketHandle);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] RawData failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketRawCloseReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketRawCloseReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        raw_remove(socketInstance->pcb.raw);
        instanceData->socketInstance[request->socketHandle] = NULL;
        CsrPmemFree(socketInstance);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] RawClose failed (invalid handle)", request->socketHandle));
    }
}

void CsrIpSocketOptionsReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketOptionsReq *request = instanceData->msg;
    CsrIpSocketInstanceData *socketInstance = socketInstanceRetrieve(instanceData, request->socketHandle);

    if (socketInstance != NULL)
    {
        CsrUint32 txWindow = 0; /* Not supported */
        CsrUint32 rxWindow = 0; /* Not supported */
        CsrBool nagle = FALSE; /* TCP only */
        CsrBool keepAlive = FALSE;
        CsrBool broadcast = FALSE;
        CsrIpSocketPriority dscp = CSR_IP_SOCKET_PRIORITY_DEFAULT;
        CsrIpSocketOptionMask validOptions = 0;

        if (socketInstance->socketType == CSR_IP_SOCKET_TYPE_TCP)
        {
            if (request->validOptions & CSR_IP_SOCKET_OPTION_NAGLE)
            {
                validOptions |= CSR_IP_SOCKET_OPTION_NAGLE;
                nagle = request->nagle;
                if (nagle)
                {
                    tcp_nagle_enable(socketInstance->pcb.tcp);
                }
                else
                {
                    tcp_nagle_disable(socketInstance->pcb.tcp);
                }
            }
        }

        if (request->validOptions & CSR_IP_SOCKET_OPTION_KEEPALIVE)
        {
            validOptions |= CSR_IP_SOCKET_OPTION_KEEPALIVE;
            keepAlive = request->keepAlive;
            if (keepAlive)
            {
                socketInstance->pcb.ip->so_options |= SOF_KEEPALIVE;
            }
            else
            {
                socketInstance->pcb.ip->so_options &= ~SOF_KEEPALIVE;
            }
        }

        if (request->validOptions & CSR_IP_SOCKET_OPTION_BROADCAST)
        {
            validOptions |= CSR_IP_SOCKET_OPTION_BROADCAST;
            broadcast = request->broadcast;
            if (broadcast)
            {
                socketInstance->pcb.ip->so_options |= SOF_BROADCAST;
            }
            else
            {
                socketInstance->pcb.ip->so_options &= ~SOF_BROADCAST;
            }
        }

        if (request->validOptions & CSR_IP_SOCKET_OPTION_DSCP)
        {
            validOptions |= CSR_IP_SOCKET_OPTION_DSCP;
            dscp = request->dscp;
            socketInstance->pcb.ip->tos = (socketInstance->pcb.ip->tos & 0x3) | (dscp << 2);
        }

        CsrIpSocketOptionsCfmSend2(socketInstance->queue, request->socketHandle,
            txWindow, rxWindow, nagle, keepAlive, broadcast, dscp, validOptions);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_SOCKET, "[%u] Options failed (invalid handle)", request->socketHandle));
    }
}
