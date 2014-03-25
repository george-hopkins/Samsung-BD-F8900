/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"

#include "csr_ip_task.h"

#include "csr_ip_ether_prim.h"
#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_socket_prim.h"

#include "csr_ip_ether_sef.h"
#include "csr_ip_ifconfig_sef.h"
#include "csr_ip_socket_sef.h"

#include "csr_ip_ether_lib.h"
#include "csr_ip_ifconfig_lib.h"
#include "csr_ip_socket_lib.h"

#include "csr_ip_handler.h"

/* Enable all three interfaces */
#define IMPLEMENT_ETHER_INTERFACE
#define IMPLEMENT_IFCONFIG_INTERFACE
#define IMPLEMENT_SOCKET_INTERFACE

#ifdef IMPLEMENT_ETHER_INTERFACE
/* Jump table for Ether messages */
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
#endif

#ifdef IMPLEMENT_IFCONFIG_INTERFACE
/* Jump table for Ifconfig messages */
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
#endif

#ifdef IMPLEMENT_SOCKET_INTERFACE
/* Jump table for Socket messages */
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
    CsrIpSocketOptionsReqHandler,
};
#endif

void CsrIpInit(void **gash)
{
    *gash = CsrPmemZalloc(sizeof(CsrIpInstanceData));
    CsrIpEtherInit();
}

void CsrIpHandler(void **gash)
{
    CsrIpInstanceData *instanceData = *gash;
    CsrUint16 event;
    void *message;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->msg = message;

        switch (event)
        {
#ifdef IMPLEMENT_ETHER_INTERFACE
            case CSR_IP_ETHER_PRIM:
            {
                CsrIpEtherPrim *type = message;

                if ((*type < CSR_IP_ETHER_PRIM_DOWNSTREAM_COUNT) && ((etherStateHandlers[*type]) != NULL))
                {
                    etherStateHandlers[*type](instanceData);
                }
                break;
            }
#endif
#ifdef IMPLEMENT_IFCONFIG_INTERFACE
            case CSR_IP_IFCONFIG_PRIM:
            {
                CsrIpIfconfigPrim *type = message;

                if ((*type < CSR_IP_IFCONFIG_PRIM_DOWNSTREAM_COUNT) && ((ifconfigStateHandlers[*type]) != NULL))
                {
                    ifconfigStateHandlers[*type](instanceData);
                }
                break;
            }
#endif
#ifdef IMPLEMENT_SOCKET_INTERFACE
            case CSR_IP_SOCKET_PRIM:
            {
                CsrIpSocketPrim *type = message;

                if ((*type < CSR_IP_SOCKET_PRIM_DOWNSTREAM_COUNT) && ((socketStateHandlers[*type]) != NULL))
                {
                    socketStateHandlers[*type](instanceData);
                }
                break;
            }
#endif
            default:
                break;
        }

        /* Free the message itself */
        CsrPmemFree(instanceData->msg);
        instanceData->msg = NULL;
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrIpDeinit(void **gash)
{
    CsrIpInstanceData *instanceData = *gash;
    CsrUint16 event;
    void *message;

    /* Clean own queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
#ifdef IMPLEMENT_ETHER_INTERFACE
            case CSR_IP_ETHER_PRIM:
            {
                CsrIpEtherFreeDownstreamMessageContents(event, message);
                break;
            }
#endif
#ifdef IMPLEMENT_IPCONFIG_INTERFACE
            case CSR_IP_IFCONFIG_PRIM:
            {
                CsrIpIfconfigFreeDownstreamMessageContents(event, message);
                break;
            }
#endif
#ifdef IMPLEMENT_SOCKET_INTERFACE
            case CSR_IP_SOCKET_PRIM:
            {
                CsrIpSocketFreeDownstreamMessageContents(event, message);
                break;
            }
#endif
            default:
                break;
        }
        CsrPmemFree(message);
    }

    CsrIpEtherDeinit();

    /* Deallocate instance data */
    CsrPmemFree(instanceData);
}

#endif
