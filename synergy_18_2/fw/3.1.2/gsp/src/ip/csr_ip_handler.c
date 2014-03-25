/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_types.h"
#include "csr_log_text_2.h"

#include "csr_ip_ether_prim.h"
#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_socket_prim.h"
#include "csr_ip_internal_prim.h"

#include "csr_ip_ether_lib.h"
#include "csr_ip_ifconfig_lib.h"
#include "csr_ip_socket_lib.h"
#include "csr_ip_internal_lib.h"

#include "csr_ip_ether_sef.h"
#include "csr_ip_ifconfig_sef.h"
#include "csr_ip_socket_sef.h"
#include "csr_ip_internal_sef.h"

#include "csr_ip_handler.h"
#include "csr_ip_task.h"

#include "lwip/nat.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/raw.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"
#include "lwip/memp.h"
#include "lwip/igmp.h"
#include "netif/etharp.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrIpLto);

#ifdef CSR_LOG_ENABLE
static const CsrCharString *subOrigins[] =
{
    "ETHER",
    "IFCONFIG",
    "SOCKET",
};
#endif

static void ethArpTimeout(CsrUint16 fniarg, void *fnvarg)
{
    CsrIpInstanceData *instanceData = (CsrIpInstanceData *) fnvarg;
    instanceData->ethArpTimerId = CsrSchedTimerSet(ARP_TMR_INTERVAL * 1000, ethArpTimeout, 0, fnvarg);
    etharp_tmr();
}

static void csrIpCoarseTimeout(CsrUint16 fniarg, void *fnvarg)
{
    CsrIpInstanceData *instanceData = (CsrIpInstanceData *) fnvarg;
    instanceData->coarseTimerId = CsrSchedTimerSet(DHCP_COARSE_TIMER_MSECS * 1000, csrIpCoarseTimeout, 0, fnvarg);
    dhcp_coarse_tmr();
    nat_tmr();
}

/* The ipTickTimeout function relies on these relations to be true. If the
   intervals are adjusted, make sure the code is updated to reflect the
   correct relationship between the timer intervals. */
#if (DNS_TMR_INTERVAL != (2 * DHCP_FINE_TIMER_MSECS)) || \
    (DHCP_FINE_TIMER_MSECS != TCP_SLOW_INTERVAL) || \
    (TCP_SLOW_INTERVAL != (2 * TCP_FAST_INTERVAL)) || \
    (TCP_FAST_INTERVAL != IGMP_TMR_INTERVAL)
#error Assertion failed: DNS_TMR_INTERVAL == 2 * DHCP_FINE_TIMER_MSECS == 2 * TCP_SLOW_INTERVAL == 4 * TCP_FAST_INTERVAL == 4 * IGMP_TMR_INTERVAL
#endif
#define CSR_IP_TICK_TIMER_INTERVAL IGMP_TMR_INTERVAL
static void ipTickTimeout(CsrUint16 fniarg, void *fnvarg)
{
    CsrIpInstanceData *instanceData = (CsrIpInstanceData *) fnvarg;
    instanceData->ipTickTimerId = CsrSchedTimerSet(CSR_IP_TICK_TIMER_INTERVAL * 1000, ipTickTimeout, (CsrUint16) (fniarg + 1), fnvarg);

    igmp_tmr(); /* IGMP_TMR_INTERVAL */
    tcp_fasttmr(); /* TCP_FAST_INTERVAL */
    if ((fniarg & 1) == 0)
    {
        /* Every 2nd TCP_FAST_INTERVAL */
        tcp_slowtmr(); /* TCP_SLOW_INTERVAL */
        dhcp_fine_tmr(); /* DHCP_FINE_TIMER_MSECS */
    }
    if ((fniarg & 3) == 0)
    {
        /* Every 4th TCP_FAST_INTERVAL */
        dns_tmr(); /* DNS_TMR_INTERVAL */
    }
}

typedef void (*CsrIpEtherStateHandlerType)(CsrIpInstanceData *instanceData);
static const CsrIpEtherStateHandlerType etherStateHandlers[CSR_IP_ETHER_PRIM_DOWNSTREAM_COUNT] =
{
    CsrIpEtherIfAddReqHandler,
    CsrIpEtherIfRemoveReqHandler,
    CsrIpEtherIfLinkUpReqHandler,
    CsrIpEtherIfLinkDownReqHandler,
    CsrIpEtherIfMulticastAddrAddResHandler,
    CsrIpEtherIfMulticastAddrRemoveResHandler,
    CsrIpEtherIfMulticastAddrFlushResHandler,
    CsrIpEtherIfFlowControlPauseReqHandler,
    CsrIpEtherIfFlowControlResumeReqHandler,
};

typedef void (*CsrIpIfconfigStateHandlerType)(CsrIpInstanceData *instanceData);
static const CsrIpIfconfigStateHandlerType ifconfigStateHandlers[CSR_IP_IFCONFIG_PRIM_DOWNSTREAM_COUNT] =
{
    CsrIpIfconfigSubscribeReqHandler,
    CsrIpIfconfigUnsubscribeReqHandler,
    CsrIpIfconfigUpReqHandler,
    CsrIpIfconfigDownReqHandler,
    CsrIpIfconfigNatReqHandler,
    CsrIpIfconfigForwardReqHandler,
    CsrIpIfconfigArpEntryAddReqHandler,
};

typedef void (*CsrIpSocketStateHandlerType)(CsrIpInstanceData *instanceData);
static const CsrIpSocketStateHandlerType socketStateHandlers[CSR_IP_SOCKET_PRIM_DOWNSTREAM_COUNT] =
{
    CsrIpSocketDnsResolveNameReqHandler,
    CsrIpSocketUdpNewReqHandler,
    CsrIpSocketUdpBindReqHandler,
    CsrIpSocketUdpDataReqHandler,
    CsrIpSocketUdpCloseReqHandler,
    CsrIpSocketUdpMulticastSubscribeReqHandler,
    CsrIpSocketUdpMulticastUnsubscribeReqHandler,
    CsrIpSocketUdpMulticastInterfaceReqHandler,
    CsrIpSocketTcpNewReqHandler,
    CsrIpSocketTcpBindReqHandler,
    CsrIpSocketTcpListenReqHandler,
    CsrIpSocketTcpConnectReqHandler,
    CsrIpSocketTcpDataReqHandler,
    CsrIpSocketTcpDataResHandler,
    CsrIpSocketTcpCloseReqHandler,
    CsrIpSocketTcpAbortReqHandler,
    CsrIpSocketRawNewReqHandler,
    CsrIpSocketRawBindReqHandler,
    CsrIpSocketRawDataReqHandler,
    CsrIpSocketRawCloseReqHandler,
    CsrIpSocketOptionsReqHandler
};

typedef void (*CsrIpInternalStateHandlerType)(CsrIpInstanceData *instanceData);
static const CsrIpInternalStateHandlerType internalStateHandlers[NUMBER_OF_CSR_IP_INTERNAL_MESSAGES] =
{
    CsrIpInternalEtherPacketReqHandler
};

void CsrIpInit(void **gash)
{
    CsrIpInstanceData *instanceData;
    struct ip_addr dnsserver;

    *gash = instanceData = CsrPmemZalloc(sizeof(CsrIpInstanceData));

    lwip_init();

    /* Initialize default DNS server addresses */
    dnsserver.addr = inet_addr("0.0.0.0");
    dns_setserver(0, &dnsserver);
    dns_setserver(1, &dnsserver);

    /* Start timers required by lwIP */
    instanceData->ethArpTimerId = CsrSchedTimerSet(ARP_TMR_INTERVAL * 1000, ethArpTimeout, 0, instanceData);
    instanceData->coarseTimerId = CsrSchedTimerSet(DHCP_COARSE_TIMER_MSECS * 1000, csrIpCoarseTimeout, 0, instanceData);
    instanceData->ipTickTimerId = CsrSchedTimerSet(CSR_IP_TICK_TIMER_INTERVAL * 1000, ipTickTimeout, 0, instanceData);

    CSR_LOG_TEXT_REGISTER(&CsrIpLto, "IP", CSR_ARRAY_SIZE(subOrigins), subOrigins);
}

void CsrIpHandler(void **gash)
{
    CsrIpInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrIpInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->msg = message;

        switch (event)
        {
            case CSR_IP_ETHER_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if ((*type < CSR_IP_ETHER_PRIM_DOWNSTREAM_COUNT) && ((etherStateHandlers[*type]) != NULL))
                {
                    etherStateHandlers[*type](instanceData);
                }
                else
                {
                    /* Unknown message type */
                }
                break;
            }
            case CSR_IP_IFCONFIG_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if ((*type < CSR_IP_IFCONFIG_PRIM_DOWNSTREAM_COUNT) && ((ifconfigStateHandlers[*type]) != NULL))
                {
                    ifconfigStateHandlers[*type](instanceData);
                }
                else
                {
                    /* Unknown message type */
                }
                break;
            }
            case CSR_IP_SOCKET_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if ((*type < CSR_IP_SOCKET_PRIM_DOWNSTREAM_COUNT) && ((socketStateHandlers[*type]) != NULL))
                {
                    socketStateHandlers[*type](instanceData);
                }
                else
                {
                    /* Unknown message type */
                }
                break;
            }
            case CSR_IP_INTERNAL_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;

                if ((*type < NUMBER_OF_CSR_IP_INTERNAL_MESSAGES) && ((internalStateHandlers[*type]) != NULL))
                {
                    internalStateHandlers[*type](instanceData);
                }
                else
                {
                    /* Unknown message type */
                }
                break;
            }
            default:
            {
                /* Message with unknown/unsupported event class received */
                break;
            }
        }

        /* Free the message itself */
        CsrPmemFree(instanceData->msg);
        instanceData->msg = NULL;
    }
}

#ifdef ENABLE_SHUTDOWN

void CsrIpDeinit(void **gash)
{
    CsrUint16 event;
    void *message;
    CsrIpInstanceData *instanceData;
    CsrUint16 socketHandle;
    CsrIpIfconfigListener *ifconfigListeners;
    instanceData = (CsrIpInstanceData *) *gash;

    /* Cancel timers */
    CsrSchedTimerCancel(instanceData->ethArpTimerId, NULL, NULL);
    CsrSchedTimerCancel(instanceData->coarseTimerId, NULL, NULL);
    CsrSchedTimerCancel(instanceData->ipTickTimerId, NULL, NULL);

    /* Clean queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_IP_ETHER_PRIM:
            {
                CsrIpEtherFreeDownstreamMessageContents(event, message);
                break;
            }
            case CSR_IP_IFCONFIG_PRIM:
            {
                CsrIpIfconfigFreeDownstreamMessageContents(event, message);
                break;
            }
            case CSR_IP_SOCKET_PRIM:
            {
                CsrIpSocketFreeDownstreamMessageContents(event, message);
                break;
            }
            default:
            {
                break;
            }
        }
        CsrPmemFree(message);
    }

    /* Deallocate socket instances */
    for (socketHandle = 0; socketHandle < CSR_IP_MAX_SOCKETS; ++socketHandle)
    {
        CsrIpSocketInstanceData *socketInstance = instanceData->socketInstance[socketHandle];
        if (socketInstance != NULL)
        {
            if (socketInstance->pcb.ip != NULL)
            {
                switch (socketInstance->socketType)
                {
                    case CSR_IP_SOCKET_TYPE_RAW:
                    {
                        raw_remove(socketInstance->pcb.raw);
                        break;
                    }
                    case CSR_IP_SOCKET_TYPE_UDP:
                    {
                        udp_remove(socketInstance->pcb.udp);
                        break;
                    }
                    case CSR_IP_SOCKET_TYPE_TCP:
                    {
                        tcp_pcb_purge(socketInstance->pcb.tcp);
                        memp_free(MEMP_TCP_PCB, socketInstance->pcb.tcp);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            CsrIpSocketInstanceDestroy(instanceData, socketHandle);
        }
    }

    /* Deallocate ifconfig listener information */
    ifconfigListeners = instanceData->ifconfigListeners;
    while (ifconfigListeners)
    {
        CsrIpIfconfigListener *ifconfigListenersNext = ifconfigListeners->next;
        CsrPmemFree(ifconfigListeners);
        ifconfigListeners = ifconfigListenersNext;
    }

    /* Deallocate instance data */
    CsrPmemFree(instanceData);
}

#endif /* ENABLE_SHUTDOWN */
