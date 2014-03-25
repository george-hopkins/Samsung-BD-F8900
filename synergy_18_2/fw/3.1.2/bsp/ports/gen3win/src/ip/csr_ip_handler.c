/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_panic.h"
#include "csr_result.h"

#include "csr_ip_task.h"

#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_socket_prim.h"

#include "csr_ip_ifconfig_sef.h"
#include "csr_ip_socket_sef.h"

#include "csr_ip_ifconfig_lib.h"
#include "csr_ip_socket_lib.h"

#include "csr_ip_handler.h"
#include "csr_ip_util.h"

#include "csr_log.h"
#include "csr_log_text_2.h"

/* Log Text */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrIpLto);
#ifdef CSR_LOG_ENABLE
static const CsrCharString *subOrigins[] =
{
    "TCP",
    "UDP",
    "RAW",
    "DNS",
};
#endif

/* The Ifconfig and Socket interfaces are optional, and they should only be
   implemented if there is an explicit need for them. */
#define IMPLEMENT_IFCONFIG_INTERFACE
#define IMPLEMENT_SOCKET_INTERFACE

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
    CsrIpSocketOptionsReqHandler
};
#endif

void CsrIpInit(void **gash)
{
    CsrIpInstanceData *instanceData;
    CsrUint16 idx;
    WSADATA wsaData;

    instanceData = (CsrIpInstanceData *) CsrPmemAlloc(sizeof(CsrIpInstanceData));
    *gash = (void *) instanceData;

    if (WSAStartup(WINSOCK_VERSION, &wsaData))
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_EXCEPTION, "Could not initialise Winsock");
    }

    instanceData->bufferLength = 64 * 1024 * 1024;
    instanceData->buffer = CsrPmemAlloc(instanceData->bufferLength);
    instanceData->msg = NULL;
    instanceData->threadHandle = NULL;
    instanceData->instMutex = CreateMutex(NULL, FALSE, NULL);

    for (idx = 0; idx < CSR_ARRAY_SIZE(instanceData->socketInsts); ++idx)
    {
        CsrIpSocketInstanceData *sockInst = &instanceData->socketInsts[idx];
        sockInst->state = CSR_IP_SOCKET_INST_STATE_FREE;
        sockInst->appSocketHandle = idx;
        sockInst->data = NULL;
        sockInst->dataLength = 0;
    }
    for (idx = 0; idx < CSR_ARRAY_SIZE(instanceData->events); idx += 2)
    {
        instanceData->events[idx + CSR_IP_SOCKET_EVENT_TYPE_SOCKET] = WSACreateEvent();
        CSR_IP_SOCKET_ASSERT(instanceData->events[idx + CSR_IP_SOCKET_EVENT_TYPE_SOCKET] != NULL, "Could not create events");
        instanceData->events[idx + CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE] = CreateEvent(NULL, FALSE, FALSE, NULL);
        CSR_IP_SOCKET_ASSERT(instanceData->events[idx + CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE] != NULL, "Could not create events");
    }

    CSR_LOG_TEXT_REGISTER(&CsrIpLto, "CSR_IP", CSR_ARRAY_SIZE(subOrigins), subOrigins);

    instanceData->threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) CsrIpSocketThread, instanceData, 0, NULL);
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
#ifdef IMPLEMENT_SOCKET_INTERFACE
            case CSR_IP_SOCKET_PRIM:
            {
                CsrPrim *type = (CsrUint16 *) message;

                if ((*type < CSR_IP_SOCKET_PRIM_DOWNSTREAM_COUNT) && ((socketStateHandlers[*type]) != NULL))
                {
                    socketStateHandlers[*type](instanceData);
                }
                break;
            }
#endif
            case CSR_IP_INTERNAL_PRIM:
            {
                csrIpMessageContainer *container = message;

                CsrSchedMessagePut(container->queue, container->prim,
                    container->message);

                instanceData->msg = NULL;
                CsrPmemFree(container);

                break;
            }

#ifdef IMPLEMENT_IFCONFIG_INTERFACE
            case CSR_IP_IFCONFIG_PRIM:
            {
                CsrPrim *type = (CsrUint16 *) message;

                if ((*type < CSR_IP_IFCONFIG_PRIM_DOWNSTREAM_COUNT) && ((ifconfigStateHandlers[*type]) != NULL))
                {
                    ifconfigStateHandlers[*type](instanceData);
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
    else
    {
        /* Failed to retrieve message from queue */
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrIpDeinit(void **gash)
{
    CsrUint16 event;
    void *message;
    CsrIpInstanceData *instanceData;
    CsrUint16 idx;

    instanceData = (CsrIpInstanceData *) *gash;

    CsrIpSocketThreadShutdown(instanceData);

    WaitForSingleObject(instanceData->threadHandle, INFINITE);

    /* Clean queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
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

            case CSR_IP_INTERNAL_PRIM:
            {
                /* This is an upstream signal */
                csrIpMessageContainer *container = message;

                event = container->prim;
                message = container->message;
                CsrPmemFree(container);

                switch (event)
                {
#ifdef IMPLEMENT_IPCONFIG_INTERFACE
                    case CSR_IP_IFCONFIG_PRIM:
                    {
                        CsrIpIfconfigFreeUpstreamMessageContents(event, message);
                        break;
                    }
#endif
#ifdef IMPLEMENT_SOCKET_INTERFACE
                    case CSR_IP_SOCKET_PRIM:
                    {
                        CsrIpSocketFreeUpstreamMessageContents(event, message);
                        break;
                    }
#endif
                }
                break;
            }

            default:
                break;
        }
        CsrPmemFree(message);
    }

    for (idx = 0; idx < CSR_ARRAY_SIZE(instanceData->socketInsts); ++idx)
    {
        CsrIpSocketInstanceData *sockInst = &instanceData->socketInsts[idx];

        CsrPmemFree(sockInst->data);

        if (sockInst->state != CSR_IP_SOCKET_INST_STATE_FREE)
        {
            (void) closesocket(sockInst->platformSocketHandle);

            if (sockInst->rxQueue != NULL)
            {
                csrIpRxElement *rxelem, *rxnext;
                /*
                 * Free receive queue.  The first element has already been
                 * sent to the application so it owns the data buffer.
                 * Free element and data buffer of subsequent elements.
                 */
                rxelem = sockInst->rxQueue;
                rxnext = rxelem->next;

                CsrPmemFree(rxelem);

                for (rxelem = rxnext;
                     rxelem;
                     rxelem = rxnext)
                {
                    rxnext = rxelem->next;
                    CsrPmemFree(rxelem->buf);
                    CsrPmemFree(rxelem);
                }
            }
        }
    }

    for (idx = 0; idx < CSR_ARRAY_SIZE(instanceData->events); idx += 2)
    {
        WSACloseEvent(instanceData->events[idx + CSR_IP_SOCKET_EVENT_TYPE_SOCKET]);
        WSACloseEvent(instanceData->events[idx + CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE]);
    }

    CsrPmemFree(instanceData->buffer);

    /* Deallocate instance data */
    CsrPmemFree(instanceData);

    WSACleanup();
}

#endif
