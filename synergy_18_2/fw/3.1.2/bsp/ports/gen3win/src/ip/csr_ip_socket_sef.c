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

#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"

#include "csr_ip_handler.h"
#include "csr_ip_util.h"

#include "csr_log.h"
#include "csr_log_text.h"

#include <Winsock2.h>
#include <ws2tcpip.h>

/*
 * If _MSC_VER is VS7 or greater, we enable IPv6
 * support.  VS6 does not provide getaddrinfo()
 * structures and some of the multicast stuff, so
 * we disable IPv6 support entirely before VS7.
 */

#if _MSC_VER >= 1300
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#endif

#define CSR_IP_SOCKET_SHUTDOWN_APPSOCKET_HANDLE 0

#define CSR_IP_SOCKET_EVENT_HANDLE_FROM_HANDLE(events, socketHandle, idx) ((events)[(((socketHandle) + 1) * CSR_IP_SOCKET_NUMBER_OF_EVENT_TYPES) + (idx)])
#define CSR_IP_SOCKET_HANDLE_FROM_EVENT(event) ((CsrUint16) ((event) / CSR_IP_SOCKET_NUMBER_OF_EVENT_TYPES) - 1)
#define CSR_IP_SOCKET_EVENT_TYPE_FROM_EVENT(event) ((CsrUint8) ((event) % CSR_IP_SOCKET_NUMBER_OF_EVENT_TYPES))

#define CSR_IP_SOCKET_STATE_CHANGE(var, val) csrIpSocketChangeState((CsrCharString *) __FILE__, __LINE__, (CsrCharString *) #var, &(var), val, (CsrCharString *) #val)

static void csrIpSocketChangeState(const CsrCharString *file, CsrUint32 line,
    const CsrCharString *fsmName,
    CsrUint32 *state,
    CsrUint32 newState, const CsrCharString *newStateName)
{
#ifdef CSR_LOG_ENABLE
    bitmask16_t mask = CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                       CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                       CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                       CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE;

    CsrLogStateTransition(mask,
        0,
        fsmName,
        *state,
        NULL,
        0,
        NULL,
        newState,
        newStateName,
        line,
        file);
#endif

    *state = newState;
}

static void csrIpInstLock(CsrIpInstanceData *instanceData)
{
    DWORD result;

    result = WaitForSingleObject(instanceData->instMutex, INFINITE);
    CSR_IP_SOCKET_ASSERT(result == WAIT_OBJECT_0, "Could not lock inst mutex");
}

static void csrIpInstUnlock(CsrIpInstanceData *instanceData)
{
    BOOL released;

    released = ReleaseMutex(instanceData->instMutex);
    CSR_IP_SOCKET_ASSERT(released, "Could not unlock inst mutex");
}

static CsrIpSocketInstanceData *csrIpSocketInstNew(
    CsrIpInstanceData *instanceData, CsrSchedQid appHandle,
    CsrIpSocketFamily family, CsrUint8 socketType, SOCKET platformSocketHandle)
{
    CsrUint16 idx;

    for (idx = 0; idx < CSR_IP_SOCKET_MAX_SOCKETS; ++idx)
    {
        CsrIpSocketInstanceData *sockInst = &instanceData->socketInsts[idx];
        if (sockInst->state == CSR_IP_SOCKET_INST_STATE_FREE)
        {
            CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, CSR_IP_SOCKET_INST_STATE_NEW);
            sockInst->socketType = socketType;
            sockInst->appHandle = appHandle;
            sockInst->socketFamily = family;
            sockInst->rxQueue = NULL;
            sockInst->platformSocketHandle = platformSocketHandle;
            return sockInst;
        }
    }

    return NULL;
}

static void csrIpSocketInstFree(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    csrIpRxElement *rxelem, *rxnext;

    CSR_IP_SOCKET_ASSERT(sockInst->state != CSR_IP_SOCKET_INST_STATE_FREE, "Already freed");

    CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, CSR_IP_SOCKET_INST_STATE_FREE);

    if (sockInst->rxQueue != NULL)
    {
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

    sockInst->appHandle = CSR_SCHED_QID_INVALID;
    sockInst->platformSocketHandle = INVALID_SOCKET;
}

static CsrIpSocketInstanceData *csrIpSocketInstFind(CsrIpInstanceData *instanceData, CsrUint16 appSocketHandle)
{
    if (appSocketHandle < CSR_IP_SOCKET_MAX_SOCKETS)
    {
        CsrIpSocketInstanceData *sockInst = &instanceData->socketInsts[appSocketHandle];
        if (sockInst->state != CSR_IP_SOCKET_INST_STATE_FREE)
        {
            CSR_IP_SOCKET_ASSERT(sockInst->appSocketHandle == appSocketHandle, "Mismatch in indices");
            return sockInst;
        }
    }

    return NULL;
}

static CsrBool csrIpSocketThreadReschedule(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst, CsrUint32 state)
{
    WSAEVENT event = CSR_IP_SOCKET_EVENT_HANDLE_FROM_HANDLE(instanceData->events,
        sockInst->appSocketHandle,
        CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE);
    CsrUint32 oldState = sockInst->state;
    CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, state);
    if (WSASetEvent(event))
    {
        return TRUE;
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "Could not reschedule socket thread"));
        CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, oldState);
        return FALSE;
    }
}

static CsrBool csrIpSocketRegisterEvents(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst, CsrUint32 events)
{
    WSAEVENT eventSocket = CSR_IP_SOCKET_EVENT_HANDLE_FROM_HANDLE(instanceData->events,
        sockInst->appSocketHandle,
        CSR_IP_SOCKET_EVENT_TYPE_SOCKET);
    CSR_IP_SOCKET_ASSERT(eventSocket, "No event found");

    if (WSAEventSelect(sockInst->platformSocketHandle, eventSocket, events))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static void csrIpSocketCloseAndFreeSocket(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    WSAEVENT eventSched = CSR_IP_SOCKET_EVENT_HANDLE_FROM_HANDLE(instanceData->events,
        sockInst->appSocketHandle,
        CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE);

    WSAEVENT eventSocket = CSR_IP_SOCKET_EVENT_HANDLE_FROM_HANDLE(instanceData->events,
        sockInst->appSocketHandle,
        CSR_IP_SOCKET_EVENT_TYPE_SOCKET);

    ResetEvent(eventSched);
    WSAEventSelect(sockInst->platformSocketHandle, eventSocket, 0);
    (void) closesocket(sockInst->platformSocketHandle);

    csrIpSocketInstFree(instanceData, sockInst);
}

static void csrIpSocketAccept(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    SOCKADDR *sa;
    SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
    SOCKADDR_IN6 sa6;
#endif
    SOCKET newSocket;
    int saLen;

#if _MSC_VER >= 1300
    if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
    {
        sa = (SOCKADDR *) &sa4;
        saLen = sizeof(sa4);
    }
#if _MSC_VER >= 1300
    else
    {
        sa = (SOCKADDR *) &sa6;
        saLen = sizeof(sa6);
    }
#endif

    newSocket = accept(sockInst->platformSocketHandle, sa, &saLen);
    if (newSocket != INVALID_SOCKET)
    {
        CsrIpSocketInstanceData *newSockInst;
        newSockInst = csrIpSocketInstNew(instanceData,
            sockInst->appHandle,
            sockInst->socketFamily,
            sockInst->socketType,
            newSocket);

        if (csrIpSocketThreadReschedule(instanceData, newSockInst, CSR_IP_SOCKET_INST_STATE_ACCEPTED))
        {
            CsrUint8 ipAddress[16];
            CsrUint16 port;

#if _MSC_VER >= 1300
            if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
            {
                port = ntohs(sa4.sin_port);
                CsrMemCpy(ipAddress, &sa4.sin_addr, 4);
                CsrMemSet(&ipAddress[4], 0, 12);
            }
#if _MSC_VER >= 1300
            else
            {
                port = ntohs(sa6.sin6_port);
                CsrMemCpy(ipAddress, &sa6.sin6_addr, sizeof(ipAddress));
            }
#endif

            CsrIpMessageForward(newSockInst->appHandle, CSR_IP_SOCKET_PRIM,
                CsrIpSocketTcpAcceptInd_struct(
                    newSockInst->appSocketHandle,
                    sockInst->socketFamily,
                    ipAddress,
                    port,
                    sockInst->appSocketHandle));
        }
        else
        {
            csrIpSocketCloseAndFreeSocket(instanceData, newSockInst);
        }
    }
    else
    {
    }
}

static void csrIpSocketTcpDataTx(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    CsrInt32 bytesSent;

    if (sockInst->dataLength > 0)
    {
        CSR_IP_SOCKET_ASSERT(sockInst->data != NULL, "TCP TX: No data to TX");
    }

    bytesSent = send(sockInst->platformSocketHandle,
        (char *) &sockInst->data[sockInst->dataOffset],
        sockInst->dataLength - sockInst->dataOffset,
        0);

    if (bytesSent != SOCKET_ERROR)
    {
        sockInst->dataOffset += (CsrUint16) bytesSent;
    }
    else if (WSAGetLastError() == WSAEWOULDBLOCK)
    {
        /*
         * If the call failed but only because the
         * operation would block, it's not an error.
         */
        bytesSent = 0;
    }

    if (bytesSent == SOCKET_ERROR)
    {
        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketTcpDataCfm_struct(sockInst->appSocketHandle,
                CSR_RESULT_FAILURE));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
    else if (sockInst->dataLength == sockInst->dataOffset)
    {
        /* Done sending. */

        if ((bytesSent != sockInst->dataLength) &&
            (!csrIpSocketRegisterEvents(instanceData,
                 sockInst, FD_READ | FD_CLOSE) == FALSE))
        {
            CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
                CsrIpSocketTcpDataCfm_struct(sockInst->appSocketHandle,
                    CSR_RESULT_FAILURE));
        }
        else
        {
            CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
                CsrIpSocketTcpDataCfm_struct(sockInst->appSocketHandle,
                    CSR_RESULT_SUCCESS));
        }

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
    else if (!csrIpSocketRegisterEvents(instanceData,
                 sockInst, FD_READ | FD_WRITE | FD_CLOSE))
    {
        /* More data to send but couldn't update event mask. */

        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketTcpDataCfm_struct(sockInst->appSocketHandle,
                CSR_RESULT_FAILURE));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
}

static void csrIpSocketUdpDataTx(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    SOCKADDR *sa;
    SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
    SOCKADDR_IN6 sa6;
#endif
    int saLen;
    CsrInt32 bytesSent;
    CsrUint16 bytesToSend;

#if _MSC_VER >= 1300
    if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
    {
        sa4.sin_family = AF_INET;
        sa4.sin_port = htons(sockInst->port);
        CsrMemCpy(&sa4.sin_addr.s_addr, sockInst->ipAddress,
            sizeof(sa4.sin_addr.s_addr));
        CsrMemSet(sa4.sin_zero, 0, sizeof(sa4.sin_zero));

        sa = (SOCKADDR *) &sa4;
        saLen = sizeof(sa4);
    }
#if _MSC_VER >= 1300
    else
    {
        CsrMemSet(&sa6, 0, sizeof(sa6));
        sa6.sin6_family = AF_INET6;
        sa6.sin6_port = htons(sockInst->port);
        CsrMemCpy(&sa6.sin6_addr, sockInst->ipAddress,
            sizeof(sa6.sin6_addr));

        sa = (SOCKADDR *) &sa6;
        saLen = sizeof(sa6);
    }
#endif

    bytesToSend = CSRMIN(sockInst->dataLength - sockInst->dataOffset,
        sockInst->maxUdpMsgSize);

    bytesSent = sendto(sockInst->platformSocketHandle,
        (char *) &sockInst->data[sockInst->dataOffset],
        sockInst->dataLength - sockInst->dataOffset,
        0,
        sa,
        saLen);

    if (bytesSent != SOCKET_ERROR)
    {
        sockInst->dataOffset += (CsrUint16) bytesSent;
    }
    else if (WSAGetLastError() == WSAEWOULDBLOCK)
    {
        /*
         * If the call failed but only because the
         * operation would block, it's not an error.
         */
        bytesSent = 0;
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "UDP TX: Could not send data"));
        /* Failure - bail out */
    }

    if (bytesSent == SOCKET_ERROR)
    {
        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketUdpDataCfm_struct(sockInst->appSocketHandle));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
    else if (sockInst->dataLength == sockInst->dataOffset)
    {
        /* Done sending. */

        if (bytesSent != sockInst->dataLength)
        {
            csrIpSocketRegisterEvents(instanceData,
                sockInst, FD_READ | FD_CLOSE);
        }

        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketUdpDataCfm_struct(sockInst->appSocketHandle));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
    else if (!csrIpSocketRegisterEvents(instanceData,
                 sockInst, FD_READ | FD_WRITE | FD_CLOSE))
    {
        /* More data to send but couldn't update event mask. */

        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketUdpDataCfm_struct(sockInst->appSocketHandle));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
}

static void csrIpSocketRawDataTx(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    SOCKADDR *sa;
    SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
    SOCKADDR_IN6 sa6;
#endif
    int saLen;
    CsrInt32 bytesSent;
    CsrUint16 bytesToSend;

    CSR_IP_SOCKET_ASSERT(sockInst->dataLength > 0, "RAW TX: No data to send");

    bytesToSend = CSRMIN(sockInst->dataLength - sockInst->dataOffset,
        sockInst->maxUdpMsgSize);

#if _MSC_VER >= 1300
    if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
    {
        sa4.sin_family = AF_INET;
        sa4.sin_port = htons(0); /* Not used. */
        CsrMemCpy(&sa4.sin_addr.s_addr, sockInst->ipAddress,
            sizeof(sa4.sin_addr.s_addr));
        CsrMemSet(sa4.sin_zero, 0, sizeof(sa4.sin_zero));

        sa = (SOCKADDR *) &sa4;
        saLen = sizeof(sa4);
    }
#if _MSC_VER >= 1300
    else
    {
        CsrMemSet(&sa6, 0, sizeof(sa6));
        sa6.sin6_family = AF_INET6;
        sa6.sin6_port = htons(0); /* Not used. */
        CsrMemCpy(&sa6.sin6_addr, sockInst->ipAddress,
            sizeof(sa6.sin6_addr));

        sa = (SOCKADDR *) &sa6;
        saLen = sizeof(sa6);
    }
#endif

    bytesSent = sendto(sockInst->platformSocketHandle,
        (char *) &sockInst->data[sockInst->dataOffset],
        bytesToSend, 0, sa, saLen);

    if (bytesSent != SOCKET_ERROR)
    {
        sockInst->dataOffset += (CsrUint16) bytesSent;
    }
    else if (WSAGetLastError() == WSAEWOULDBLOCK)
    {
        /*
         * If the call failed but only because the
         * operation would block, it's not an error.
         */
        bytesSent = 0;
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "RAW TX: Could not send data"));
        /* Failure - bail out */
    }

    if (bytesSent == SOCKET_ERROR)
    {
        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketRawDataCfm_struct(sockInst->appSocketHandle));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
    else if (sockInst->dataLength == sockInst->dataOffset)
    {
        /* Done sending. */

        if (bytesSent != sockInst->dataLength)
        {
            csrIpSocketRegisterEvents(instanceData,
                sockInst, FD_READ | FD_CLOSE);
        }

        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketRawDataCfm_struct(sockInst->appSocketHandle));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
    else if (!csrIpSocketRegisterEvents(instanceData,
                 sockInst, FD_READ | FD_WRITE | FD_CLOSE))
    {
        /* More data to send but couldn't update event mask. */

        CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
            CsrIpSocketRawDataCfm_struct(sockInst->appSocketHandle));

        CsrPmemFree(sockInst->data);
        sockInst->data = NULL;
    }
}

static void csrIpSocketTcpDataRx(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    int bytesRecv;

    bytesRecv = recv(sockInst->platformSocketHandle, (char *) instanceData->buffer, instanceData->bufferLength, 0);
    if (bytesRecv != SOCKET_ERROR)
    {
        csrIpRxElement *rxelem;

        rxelem = CsrPmemAlloc(sizeof(*rxelem));

        rxelem->buf = CsrMemDup(instanceData->buffer, bytesRecv);
        rxelem->bufLen = bytesRecv;
        rxelem->next = NULL;

        if (sockInst->rxQueue == NULL)
        {
            sockInst->rxQueue = rxelem;
            CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
                CsrIpSocketTcpDataInd_struct(sockInst->appSocketHandle,
                    rxelem->bufLen,
                    rxelem->buf));
        }
        else
        {
            /* Queue indication for later. */

            csrIpRxElement *rxprev;

            for (rxprev = sockInst->rxQueue;
                 rxprev->next != NULL;
                 rxprev = rxprev->next)
            {
                /* nothing -- simply iterate to find last element */
            }

            rxprev->next = rxelem;
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "TCP RX: Could not recv"));
    }
}

static void csrIpSocketUdpDataRx(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    SOCKADDR *sa;
    SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
    SOCKADDR_IN6 sa6;
#endif
    int saLen;
    int bytesRecv;

#if _MSC_VER >= 1300
    if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
    {
        sa = (SOCKADDR *) &sa4;
        saLen = sizeof(sa4);
    }
#if _MSC_VER >= 1300
    else
    {
        sa = (SOCKADDR *) &sa6;
        saLen = sizeof(sa6);
    }
#endif

    bytesRecv = recvfrom(sockInst->platformSocketHandle,
        (char *) instanceData->buffer,
        instanceData->bufferLength,
        0,
        sa, &saLen);
    if (bytesRecv != SOCKET_ERROR)
    {
        if (bytesRecv > 0)
        {
            CsrUint8 *data;
            CsrUint8 ipAddress[16];
            CsrUint16 port;

#if _MSC_VER >= 1300
            if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
            {
                CsrMemCpy(ipAddress, &sa4.sin_addr, 4);
                CsrMemSet(&ipAddress[4], 0, 12);
                port = ntohs(sa4.sin_port);
            }
#if _MSC_VER >= 1300
            else
            {
                CsrMemCpy(ipAddress, &sa6.sin6_addr, sizeof(ipAddress));
                port = ntohs(sa6.sin6_port);
            }
#endif
            data = CsrMemDup(instanceData->buffer, bytesRecv);
            CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
                CsrIpSocketUdpDataInd_struct(sockInst->appSocketHandle,
                    sockInst->socketFamily,
                    ipAddress,
                    port,
                    (CsrUint16) bytesRecv,
                    data));
        }
        else
        {
            CSR_IP_SOCKET_ASSERT_NOT_REACHED();
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "UDP RX: Could not recv"));
    }
}

static void csrIpSocketRawDataRx(CsrIpInstanceData *instanceData, CsrIpSocketInstanceData *sockInst)
{
    SOCKADDR *sa;
    SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
    SOCKADDR_IN6 sa6;
#endif
    int saLen;
    int bytesRecv;

#if _MSC_VER >= 1300
    if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
    {
        sa = (SOCKADDR *) &sa4;
        saLen = sizeof(sa4);
    }
#if _MSC_VER >= 1300
    else
    {
        sa = (SOCKADDR *) &sa6;
        saLen = sizeof(sa6);
    }
#endif

    bytesRecv = recvfrom(sockInst->platformSocketHandle,
        (char *) instanceData->buffer,
        instanceData->bufferLength,
        0, sa, &saLen);
    if (bytesRecv != SOCKET_ERROR)
    {
        if (bytesRecv > 0)
        {
            CsrUint8 *data;
            CsrUint8 ipAddress[16];

#if _MSC_VER >= 1300
            if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
            {
                CsrMemCpy(ipAddress, &sa4.sin_addr, 4);
                CsrMemSet(&ipAddress[4], 0, 12);
            }
#if _MSC_VER >= 1300
            else
            {
                CsrMemCpy(ipAddress, &sa6.sin6_addr, sizeof(ipAddress));
            }
#endif

            data = CsrMemDup(instanceData->buffer, bytesRecv);
            CsrIpMessageForward(sockInst->appHandle, CSR_IP_SOCKET_PRIM,
                CsrIpSocketRawDataInd_struct(sockInst->appSocketHandle,
                    sockInst->socketFamily,
                    ipAddress,
                    (CsrUint16) bytesRecv,
                    data));
        }
        else
        {
            CSR_IP_SOCKET_ASSERT_NOT_REACHED();
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "RAW RX: Could not recv"));
    }
}

static CsrBool CsrIpSocketThreadHalt(CsrIpInstanceData *instanceData,
    CsrUint16 eventId)
{
    return eventId == CSR_IP_SOCKET_SHUTDOWN_APPSOCKET_HANDLE;
}

void CsrIpSocketThreadShutdown(CsrIpInstanceData *instanceData)
{
    WSASetEvent(instanceData->events[CSR_IP_SOCKET_SHUTDOWN_APPSOCKET_HANDLE]);
}

void CsrIpSocketThread(CsrIpInstanceData *instanceData)
{
    CSR_LOG_TEXT_INFO((CsrIpLto, 0, "Socket thread started"));

    while (TRUE)
    {
        CsrIpSocketInstanceData *sockInst;
        CsrUint16 appSocketHandle;
        CsrUint16 eventType;
        WSANETWORKEVENTS networkEvents;
        DWORD resultCode;

        resultCode = WSAWaitForMultipleEvents(CSR_ARRAY_SIZE(instanceData->events),
            instanceData->events,
            FALSE, WSA_INFINITE, FALSE);

        CSR_LOG_TEXT_INFO((CsrIpLto, 0, "Socket thread wakeup"));

        CSR_IP_SOCKET_ASSERT(resultCode != WSA_WAIT_FAILED, "Could not wait for events");

        resultCode -= WSA_WAIT_EVENT_0;

        if (CsrIpSocketThreadHalt(instanceData, (CsrUint16) resultCode))
        {
            break;
        }

        appSocketHandle = CSR_IP_SOCKET_HANDLE_FROM_EVENT(resultCode);
        eventType = CSR_IP_SOCKET_EVENT_TYPE_FROM_EVENT(resultCode);
        sockInst = csrIpSocketInstFind(instanceData, appSocketHandle);

        CSR_IP_SOCKET_ASSERT(sockInst, "No sockInst found");

        switch (eventType)
        {
            case CSR_IP_SOCKET_EVENT_TYPE_SOCKET:
            {
                csrIpInstLock(instanceData);
                if (!WSAEnumNetworkEvents(sockInst->platformSocketHandle,
                        CSR_IP_SOCKET_EVENT_HANDLE_FROM_HANDLE(instanceData->events,
                            appSocketHandle,
                            CSR_IP_SOCKET_EVENT_TYPE_SOCKET),
                        &networkEvents))
                {
                    if (networkEvents.lNetworkEvents & FD_ACCEPT)
                    {
                        csrIpSocketAccept(instanceData, sockInst);
                    }

                    if (networkEvents.lNetworkEvents & FD_READ)
                    {
                        if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_TCP)
                        {
                            csrIpSocketTcpDataRx(instanceData, sockInst);
                        }
                        else if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_UDP)
                        {
                            csrIpSocketUdpDataRx(instanceData, sockInst);
                        }
                        else if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_RAW)
                        {
                            csrIpSocketRawDataRx(instanceData, sockInst);
                        }
                        else
                        {
                            CSR_IP_SOCKET_ASSERT_NOT_REACHED();
                        }
                    }

                    if (networkEvents.lNetworkEvents & FD_WRITE)
                    {
                        if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_TCP)
                        {
                            csrIpSocketTcpDataTx(instanceData, sockInst);
                        }
                        else if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_UDP)
                        {
                            csrIpSocketUdpDataTx(instanceData, sockInst);
                        }
                        else if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_RAW)
                        {
                            csrIpSocketRawDataTx(instanceData, sockInst);
                        }
                        else
                        {
                            CSR_IP_SOCKET_ASSERT_NOT_REACHED();
                        }
                    }

                    if (networkEvents.lNetworkEvents & FD_CLOSE)
                    {
                        if (sockInst->rxQueue == NULL)
                        {
                            /* We can close it right away. */
                            CsrIpMessageForward(sockInst->appHandle,
                                CSR_IP_SOCKET_PRIM,
                                CsrIpSocketTcpCloseInd_struct(
                                    sockInst->appSocketHandle));
                            csrIpSocketCloseAndFreeSocket(instanceData, sockInst);
                        }
                        else
                        {
                            WSAEVENT event = CSR_IP_SOCKET_EVENT_HANDLE_FROM_HANDLE(
                                instanceData->events,
                                sockInst->appSocketHandle,
                                CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE);

                            /*
                             * Make sure we don't end up trying to process
                             * an event in CSR_IP_SOCKET_INST_STATE_DEAD.
                             */
                            if (!WSAResetEvent(event))
                            {
                                CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "Could not clear reschedule event"));
                            }

                            CSR_IP_SOCKET_STATE_CHANGE(sockInst->state,
                                CSR_IP_SOCKET_INST_STATE_DEAD);
                        }
                    }
                }
                else
                {
                    CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "Could not enumerate network events"));
                }
                csrIpInstUnlock(instanceData);
                break;
            }

            case CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE:
            {
                csrIpInstLock(instanceData);

                switch (sockInst->state)
                {
                    case CSR_IP_SOCKET_INST_STATE_LISTEN:
                    {
                        if (!csrIpSocketRegisterEvents(instanceData, sockInst, FD_ACCEPT))
                        {
                            CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "Could not subscribe for events on listen socket"));
                            CsrIpMessageForward(sockInst->appHandle,
                                CSR_IP_SOCKET_PRIM,
                                CsrIpSocketTcpListenCfm_struct(
                                    sockInst->appSocketHandle,
                                    CSR_RESULT_FAILURE));
                        }
                        else
                        {
                            CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, CSR_IP_SOCKET_INST_STATE_ACCEPT);
                            CsrIpMessageForward(sockInst->appHandle,
                                CSR_IP_SOCKET_PRIM,
                                CsrIpSocketTcpListenCfm_struct(
                                    sockInst->appSocketHandle,
                                    CSR_RESULT_SUCCESS));
                        }
                        break;
                    }

                    case CSR_IP_SOCKET_INST_STATE_CONNECT:
                    {
                        SOCKADDR *sa;
                        SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
                        SOCKADDR_IN6 sa6;
#endif
                        int saLen;

#if _MSC_VER >= 1300
                        if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
                        {
                            sa4.sin_family = AF_INET;
                            sa4.sin_port = htons(sockInst->port);
                            CsrMemCpy(&sa4.sin_addr.s_addr,
                                sockInst->ipAddress,
                                sizeof(sa4.sin_addr.s_addr));
                            CsrMemSet(sa4.sin_zero, 0, sizeof(sa4.sin_zero));

                            sa = (SOCKADDR *) &sa4;
                            saLen = sizeof(sa4);
                        }
#if _MSC_VER >= 1300
                        else
                        {
                            CsrMemSet(&sa6, 0, sizeof(sa6));
                            sa6.sin6_family = AF_INET6;
                            sa6.sin6_port = htons(sockInst->port);
                            CsrMemCpy(&sa6.sin6_addr,
                                sockInst->ipAddress,
                                sizeof(sa6.sin6_addr));

                            sa = (SOCKADDR *) &sa6;
                            saLen = sizeof(sa6);
                        }
#endif

                        if (connect(sockInst->platformSocketHandle, sa,
                                saLen))
                        {
                            CsrIpMessageForward(sockInst->appHandle,
                                CSR_IP_SOCKET_PRIM,
                                CsrIpSocketTcpConnectCfm_struct(
                                    sockInst->appSocketHandle,
                                    CSR_RESULT_FAILURE));
                            csrIpSocketCloseAndFreeSocket(instanceData,
                                sockInst);
                        }
                        else
                        {
                            if (!csrIpSocketRegisterEvents(instanceData,
                                    sockInst, FD_READ | FD_CLOSE))
                            {
                                CsrIpMessageForward(sockInst->appHandle,
                                    CSR_IP_SOCKET_PRIM,
                                    CsrIpSocketTcpConnectCfm_struct(
                                        sockInst->appSocketHandle,
                                        CSR_RESULT_FAILURE));

                                CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "Could not subscribe for events on new data socket"));
                                csrIpSocketCloseAndFreeSocket(instanceData, sockInst);
                            }
                            else
                            {
                                CsrIpMessageForward(sockInst->appHandle,
                                    CSR_IP_SOCKET_PRIM,
                                    CsrIpSocketTcpConnectCfm_struct(
                                        sockInst->appSocketHandle,
                                        CSR_RESULT_SUCCESS));
                                CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, CSR_IP_SOCKET_INST_STATE_ACTIVE);
                            }
                        }
                        break;
                    }

                    case CSR_IP_SOCKET_INST_STATE_BIND:
                    case CSR_IP_SOCKET_INST_STATE_ACCEPTED:
                    {
                        if (!csrIpSocketRegisterEvents(instanceData, sockInst, FD_READ | FD_CLOSE))
                        {
                            CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "Could not subscribe for events on new data socket"));
                            csrIpSocketCloseAndFreeSocket(instanceData, sockInst);
                        }
                        else
                        {
                            CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, CSR_IP_SOCKET_INST_STATE_ACTIVE);
                        }
                        break;
                    }

                    case CSR_IP_SOCKET_INST_STATE_ACTIVE:
                    {
                        if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_TCP)
                        {
                            csrIpSocketTcpDataTx(instanceData, sockInst);
                        }
                        else if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_UDP)
                        {
                            csrIpSocketUdpDataTx(instanceData, sockInst);
                        }
                        else if (sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_RAW)
                        {
                            csrIpSocketRawDataTx(instanceData, sockInst);
                        }
                        else
                        {
                            CSR_IP_SOCKET_ASSERT_NOT_REACHED();
                        }
                        break;
                    }

                    case CSR_IP_SOCKET_INST_STATE_ABORT:
                    case CSR_IP_SOCKET_INST_STATE_CLOSE:
                    {
                        /* Per default, winsock performs a graceful shutdown
                         * with immediate return, so we treat abort/close
                         * identically */
                        csrIpSocketCloseAndFreeSocket(instanceData, sockInst);
                        break;
                    }

                    default:
                        CSR_IP_SOCKET_ASSERT_NOT_REACHED();
                        break;
                }

                csrIpInstUnlock(instanceData);
                break;
            }

            default:
                CSR_IP_SOCKET_ASSERT_NOT_REACHED();
                break;
        }
    }

    CSR_LOG_TEXT_INFO((CsrIpLto, 0, "Socket thread shutdown"));

    ExitThread(0);
}

void CsrIpSocketDnsResolveNameReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketDnsResolveNameReq *req = instanceData->msg;
    CsrUint8 ipAddr[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    /* struct addrinfo only provided in the SDK from VS2003. */
#if _MSC_VER >= 1300
    struct addrinfo *res;
    int rval;
#else
    struct hostent *host;
#endif

#if _MSC_VER >= 1300
    rval = !0;

    /*
     * Opportunistic IPv6 lookup
     */
    if (req->socketFamilyMax == CSR_IP_SOCKET_FAMILY_IP6)
    {
        struct addrinfo hint;

        CsrMemSet(&hint, 0, sizeof(hint));
        hint.ai_family = AF_INET6;

        rval = getaddrinfo(req->name, NULL, &hint, &res);
    }

    /*
     * Try IPv4 lookup if IPv6 lookup didn't happen, failed,
     * or returned no results.
     */
    if ((rval != 0) || (res == NULL))
    {
        struct addrinfo hint;

        CsrMemSet(&hint, 0, sizeof(hint));
        hint.ai_family = AF_INET;

        rval = getaddrinfo(req->name, NULL, &hint, &res);
    }

    if (rval == 0)
    {
        if (res != NULL)
        {
            /*
             * Just copy the first element.  This can only be
             * of the correct address type because of how
             * lookups are done.
             */

            if (res->ai_family == AF_INET)
            {
                struct sockaddr_in *sa;

                sa = (struct sockaddr_in *) res->ai_addr;

                CsrMemCpy(ipAddr, &sa->sin_addr,
                    CSRMIN(sizeof(ipAddr), res->ai_addrlen));

                CsrIpSocketDnsResolveNameCfmSend2(req->appHandle,
                    CSR_IP_SOCKET_FAMILY_IP4, ipAddr, req->name,
                    CSR_RESULT_SUCCESS);
            }
            else
            {
                struct sockaddr_in6 *sa;

                sa = (struct sockaddr_in6 *) res->ai_addr;

                CsrMemCpy(ipAddr, &sa->sin6_addr,
                    CSRMIN(sizeof(ipAddr), res->ai_addrlen));

                CsrIpSocketDnsResolveNameCfmSend2(req->appHandle,
                    CSR_IP_SOCKET_FAMILY_IP6, ipAddr, req->name,
                    CSR_RESULT_SUCCESS);
            }

            freeaddrinfo(res);
        }
        else
        {
            CsrIpSocketDnsResolveNameCfmSend2(req->appHandle,
                req->socketFamilyMax, ipAddr, req->name,
                CSR_RESULT_FAILURE);
        }
    }
#else
    host = gethostbyname((char *) req->name);
    if ((host != NULL) && (host->h_addrtype == AF_INET) && (*host->h_addr_list != NULL))
    {
        CsrMemCpy(ipAddr, (CsrUint8 *) host->h_addr_list[0], 4);
        CsrIpSocketDnsResolveNameCfmSend2(req->appHandle,
            CSR_IP_SOCKET_FAMILY_IP4, ipAddr, req->name,
            CSR_RESULT_SUCCESS);
    }
#endif
    else
    {
        CsrIpSocketDnsResolveNameCfmSend2(req->appHandle,
            req->socketFamilyMax,
            ipAddr, req->name, CSR_RESULT_FAILURE);
    }
}

void CsrIpSocketUdpNewReqHandler(CsrIpInstanceData *instanceData)
{
    SOCKET platformSocketHandle;
    CsrIpSocketUdpNewReq *req = instanceData->msg;
    int reuseAddr = TRUE;
    unsigned int maxUdpMsgSize;
    int maxUdpMsgSizeLen = sizeof(maxUdpMsgSize);

    if (req->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
    {
        platformSocketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    else
#if _MSC_VER >= 1300
    {
        platformSocketHandle = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    }
#else
    {
        CsrIpSocketUdpNewCfmSend2(req->appHandle, 0,
            CSR_IP_SOCKET_RESULT_IP6_NOT_SUPPORTED);
        return;
    }
#endif

    if ((platformSocketHandle == INVALID_SOCKET) ||
        setsockopt(platformSocketHandle, SOL_SOCKET, SO_REUSEADDR, (char *) &reuseAddr, sizeof(reuseAddr)) ||
        getsockopt(platformSocketHandle, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *) &maxUdpMsgSize, &maxUdpMsgSizeLen))
    {
        (void) closesocket(platformSocketHandle);
        CsrIpSocketUdpNewCfmSend2(req->appHandle,
            0 /* Invalid platformSocketHandle handle */,
            CSR_RESULT_FAILURE);
    }
    else
    {
        CsrIpSocketInstanceData *sockInst;

        csrIpInstLock(instanceData);
        sockInst = csrIpSocketInstNew(instanceData,
            req->appHandle,
            req->socketFamily,
            CSR_IP_SOCKET_INST_TYPE_UDP,
            platformSocketHandle);
        csrIpInstUnlock(instanceData);


        if (sockInst)
        {
            sockInst->maxUdpMsgSize = CSRMAX(maxUdpMsgSize, 0xFFFF);
            CsrIpSocketUdpNewCfmSend2(req->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_SUCCESS);
        }
        else
        {
            (void) closesocket(platformSocketHandle);
            CsrIpSocketUdpNewCfmSend2(req->appHandle,
                0 /* Invalid platformSocketHandle handle */,
                CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS);
        }
    }
}

void CsrIpSocketUdpBindReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpBindReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        SOCKADDR *sa;
        SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
        SOCKADDR_IN6 sa6;
#endif
        int saLen;
#ifdef SO_EXCLUSIVEADDRUSE
        int optval = 1;
#endif

#if _MSC_VER >= 1300
        if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
        {
            sa4.sin_family = AF_INET;
            sa4.sin_port = htons(req->port);
            CsrMemCpy(&sa4.sin_addr.s_addr, req->ipAddress,
                sizeof(sa4.sin_addr.s_addr));
            CsrMemSet(sa4.sin_zero, 0, sizeof(sa4.sin_zero));

            sa = (SOCKADDR *) &sa4;
            saLen = sizeof(sa4);
        }
#if _MSC_VER >= 1300
        else
        {
            CsrMemSet(&sa6, 0, sizeof(sa6));
            sa6.sin6_family = AF_INET6;
            sa6.sin6_port = htons(req->port);
            CsrMemCpy(&sa6.sin6_addr, req->ipAddress, sizeof(sa6.sin6_addr));

            sa = (SOCKADDR *) &sa6;
            saLen = sizeof(sa6);
        }
#endif

        /*Configure WinSock to allow only one bound per port
          First awailable from WinNT 4 SP4 and VS later than VS6*/
#ifdef SO_EXCLUSIVEADDRUSE
        if (setsockopt(sockInst->platformSocketHandle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *) &optval, sizeof(optval)))
        {
            CSR_LOG_TEXT_INFO((CsrIpLto, 0, "Couldn't ensure exclusive bind of ports"));
        }
#endif

        if (bind(sockInst->platformSocketHandle, sa, saLen))
        {
            CsrIpSocketUdpBindCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_IP_SOCKET_RESULT_PORT_IN_USE,
                req->port);
        }
        else
        {
            if (csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_BIND))
            {
                if (req->port == 0)
                {
                    (void) getsockname(sockInst->platformSocketHandle,
                        sa, &saLen);

#if _MSC_VER >= 1300
                    if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
                    {
                        req->port = ntohs(sa4.sin_port);
                    }
#if _MSC_VER >= 1300
                    else
                    {
                        req->port = ntohs(sa6.sin6_port);
                    }
#endif
                }

                CsrIpSocketUdpBindCfmSend2(sockInst->appHandle,
                    sockInst->appSocketHandle,
                    CSR_RESULT_SUCCESS,
                    req->port);
            }
            else
            {
                CsrIpSocketUdpBindCfmSend2(sockInst->appHandle,
                    sockInst->appSocketHandle,
                    CSR_RESULT_FAILURE,
                    req->port);
            }
        }
    }
}

void CsrIpSocketUdpDataReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpDataReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst != NULL)
    {
        sockInst->data = req->data;
        sockInst->dataLength = req->dataLength;
        sockInst->dataOffset = 0;
        CsrMemCpy(sockInst->ipAddress, req->ipAddress,
            sizeof(sockInst->ipAddress));
        sockInst->port = req->port;
        req->data = NULL;

        if (!sockInst || !csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_ACTIVE))
        {
            CsrPmemFree(sockInst->data);
            sockInst->data = NULL;
            CsrIpSocketUdpDataCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle);
        }
    }
    else
    {
        CsrPmemFree(req->data);
        req->data = NULL;
    }

    csrIpInstUnlock(instanceData);
}

void CsrIpSocketUdpCloseReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpCloseReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        if (!csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_CLOSE))
        {
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "No sockinst found for %u", req->socketHandle));
    }
    csrIpInstUnlock(instanceData);
}

#if _MSC_VER >= 1300
CsrBool interfaceIndexGet(CsrUint8 ipAddress[16], DWORD *interfaceIndex)
{
    PIP_ADAPTER_ADDRESSES ifList, curIf;
    ULONG res;
    ULONG bufLen;
    CsrBool rval;

    rval = FALSE;

    /*
     * This abomination of an API makes my eyes burn.
     *
     * GetAdaptersAddresses() updates bufLen with the
     * required buffer size.
     */
    bufLen = 0;
    ifList = NULL;

    do
    {
        CsrPmemFree(ifList);
        ifList = CsrPmemAlloc(bufLen);

        res = GetAdaptersAddresses(AF_INET6, GAA_FLAG_INCLUDE_PREFIX,
            NULL, ifList, &bufLen);
    } while (res == ERROR_BUFFER_OVERFLOW);

    curIf = ifList;

    while (curIf != NULL)
    {
        PIP_ADAPTER_UNICAST_ADDRESS addr;

        addr = curIf->FirstUnicastAddress;

        while (addr != NULL)
        {
            SOCKADDR_IN6 *sa6;

            sa6 = (SOCKADDR_IN6 *) addr->Address.lpSockaddr;

            if (CsrMemCmp(&sa6->sin6_addr, ipAddress, 16) == 0)
            {
                *interfaceIndex = curIf->Ipv6IfIndex;
                rval = TRUE;
                break;
            }

            addr = addr->Next;
        }

        if (addr != NULL)
        {
            break;
        }
        else
        {
            curIf = curIf->Next;
        }
    }

    CsrPmemFree(ifList);

    return rval;
}

#endif

void CsrIpSocketUdpMulticastSubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpMulticastSubscribeReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        int rval;

#if _MSC_VER >= 1300
        if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
        {
            CsrUint8 *sAddr;
            struct ip_mreq imr;

            sAddr = (CsrUint8 *) &imr.imr_multiaddr.s_addr;
            sAddr[0] = req->group[0];
            sAddr[1] = req->group[1];
            sAddr[2] = req->group[2];
            sAddr[3] = req->group[3];

            sAddr = (CsrUint8 *) &imr.imr_interface.s_addr;
            sAddr[0] = req->interfaceIp[0];
            sAddr[1] = req->interfaceIp[1];
            sAddr[2] = req->interfaceIp[2];
            sAddr[3] = req->interfaceIp[3];

            rval = setsockopt(sockInst->platformSocketHandle,
                IPPROTO_IP,
                IP_ADD_MEMBERSHIP,
                (const char *) &imr,
                sizeof(imr));
        }
#if _MSC_VER >= 1300
        else
        {
            struct ipv6_mreq imr6;
            DWORD interfaceIndex;
            CsrUint8 nullIp[16] =
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            };

            if ((interfaceIndexGet(req->interfaceIp, &interfaceIndex)
                 != FALSE) || (CsrMemCmp(req->interfaceIp, nullIp, 16) == 0))
            {
                CsrUint8 *sAddr;

                sAddr = (CsrUint8 *) &imr6.ipv6mr_multiaddr;
                CsrMemCpy(sAddr, req->group, 16);

                imr6.ipv6mr_interface = interfaceIndex;

                rval = setsockopt(sockInst->platformSocketHandle,
                    IPPROTO_IPV6,
                    IPV6_ADD_MEMBERSHIP,
                    (const char *) &imr6,
                    sizeof(imr6));
            }
            else
            {
                /*
                 * Interface not found, so fake an error.
                 */
                rval = SOCKET_ERROR;
                WSASetLastError(!WSAEADDRNOTAVAIL);
            }
        }
#endif

        if ((rval == SOCKET_ERROR) && (WSAGetLastError() == WSAEADDRNOTAVAIL))
        {
            /* Already subscribed, fake success. */
            rval = 0;
        }

        if (rval == 0)
        {
            CsrIpSocketUdpMulticastSubscribeCfmSend2(sockInst->appHandle,
                req->socketHandle,
                sockInst->socketFamily,
                req->interfaceIp,
                req->group,
                CSR_RESULT_SUCCESS);
        }
        else
        {
            CsrIpSocketUdpMulticastSubscribeCfmSend2(sockInst->appHandle,
                req->socketHandle,
                sockInst->socketFamily,
                req->interfaceIp,
                req->group,
                CSR_RESULT_FAILURE);
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto,
                              0,
                              "No sockinst found for %u",
                              req->socketHandle));
    }

    csrIpInstUnlock(instanceData);
}

void CsrIpSocketUdpMulticastUnsubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpMulticastUnsubscribeReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        int rval;

#if _MSC_VER >= 1300
        if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
        {
            CsrUint8 *sAddr;
            struct ip_mreq imr;

            sAddr = (CsrUint8 *) &imr.imr_multiaddr.s_addr;
            sAddr[0] = req->group[0];
            sAddr[1] = req->group[1];
            sAddr[2] = req->group[2];
            sAddr[3] = req->group[3];

            sAddr = (CsrUint8 *) &imr.imr_interface.s_addr;
            sAddr[0] = req->interfaceIp[0];
            sAddr[1] = req->interfaceIp[1];
            sAddr[2] = req->interfaceIp[2];
            sAddr[3] = req->interfaceIp[3];

            rval = setsockopt(sockInst->platformSocketHandle,
                IPPROTO_IP,
                IP_DROP_MEMBERSHIP,
                (const char *) &imr,
                sizeof(imr));
        }
#if _MSC_VER >= 1300
        else
        {
            struct ipv6_mreq imr6;
            DWORD interfaceIndex;
            CsrUint8 nullIp[16] =
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            };

            if ((interfaceIndexGet(req->interfaceIp, &interfaceIndex)
                 != FALSE) || (CsrMemCmp(req->interfaceIp, nullIp, 16) == 0))
            {
                CsrUint8 *sAddr;

                sAddr = (CsrUint8 *) &imr6.ipv6mr_multiaddr;
                CsrMemCpy(sAddr, req->group, 16);

                imr6.ipv6mr_interface = interfaceIndex;

                rval = setsockopt(sockInst->platformSocketHandle,
                    IPPROTO_IPV6,
                    IPV6_DROP_MEMBERSHIP,
                    (const char *) &imr6,
                    sizeof(imr6));
            }
            else
            {
                /*
                 * Interface not found, so fake an error.
                 */
                rval = SOCKET_ERROR;
                WSASetLastError(!WSAEADDRNOTAVAIL);
            }
        }
#endif

        if ((rval == SOCKET_ERROR) && (WSAGetLastError() == WSAEADDRNOTAVAIL))
        {
            /* Already subscribed, fake success. */
            rval = 0;
        }

        if (rval == 0)
        {
            CsrIpSocketUdpMulticastUnsubscribeCfmSend2(sockInst->appHandle,
                req->socketHandle,
                sockInst->socketFamily,
                req->interfaceIp,
                req->group,
                CSR_RESULT_SUCCESS);
        }
        else
        {
            CsrIpSocketUdpMulticastUnsubscribeCfmSend2(sockInst->appHandle,
                req->socketHandle,
                sockInst->socketFamily,
                req->interfaceIp,
                req->group,
                CSR_RESULT_FAILURE);
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto,
                              0,
                              "No sockinst found for %u",
                              req->socketHandle));
    }

    csrIpInstUnlock(instanceData);
}

void CsrIpSocketUdpMulticastInterfaceReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketUdpMulticastInterfaceReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        int rval;

#if _MSC_VER >= 1300
        if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
        {
            CsrUint8 *sAddr;
            DWORD interfaceIp;

            sAddr = (CsrUint8 *) &interfaceIp;
            sAddr[0] = req->interfaceIp[0];
            sAddr[1] = req->interfaceIp[1];
            sAddr[2] = req->interfaceIp[2];
            sAddr[3] = req->interfaceIp[3];

            rval = setsockopt(sockInst->platformSocketHandle,
                IPPROTO_IP,
                IP_MULTICAST_IF,
                (const char *) &interfaceIp,
                sizeof(interfaceIp));
        }
#if _MSC_VER >= 1300
        else
        {
            DWORD interfaceIndex;
            CsrUint8 nullIp[16] =
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0
            };

            if ((interfaceIndexGet(req->interfaceIp, &interfaceIndex)
                 != FALSE) || (CsrMemCmp(req->interfaceIp, nullIp, 16) == 0))
            {
                rval = setsockopt(sockInst->platformSocketHandle,
                    IPPROTO_IPV6,
                    IPV6_MULTICAST_IF,
                    (const char *) &interfaceIndex,
                    sizeof(interfaceIndex));
            }
            else
            {
                /*
                 * Interface not found, so fake an error.
                 */
                rval = 1;
            }
        }
#endif
        if (rval == 0)
        {
            CsrIpSocketUdpMulticastInterfaceCfmSend2(sockInst->appHandle,
                req->socketHandle,
                CSR_RESULT_SUCCESS);
        }
        else
        {
            CsrIpSocketUdpMulticastInterfaceCfmSend2(sockInst->appHandle,
                req->socketHandle,
                CSR_RESULT_FAILURE);
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto,
                              0,
                              "No sockinst found for %u",
                              req->socketHandle));
    }

    csrIpInstUnlock(instanceData);
}

void CsrIpSocketRawNewReqHandler(CsrIpInstanceData *instanceData)
{
    SOCKET platformSocketHandle;
    CsrIpSocketRawNewReq *req = instanceData->msg;
    int reuseAddr = TRUE;
    unsigned int maxRawMsgSize;
    int maxRawMsgSizeLen = sizeof(maxRawMsgSize);

    if (req->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
    {
        platformSocketHandle = socket(AF_INET, SOCK_RAW, req->protocolNumber);
    }
    else
#if _MSC_VER >= 1300
    {
        platformSocketHandle = socket(AF_INET6, SOCK_RAW, req->protocolNumber);
    }
#else
    {
        CsrIpSocketRawNewCfmSend2(req->appHandle, 0,
            CSR_IP_SOCKET_RESULT_IP6_NOT_SUPPORTED);
        return;
    }
#endif

    if ((platformSocketHandle == INVALID_SOCKET) ||
        setsockopt(platformSocketHandle, SOL_SOCKET, SO_REUSEADDR, (char *) &reuseAddr, sizeof(reuseAddr)) ||
        getsockopt(platformSocketHandle, SOL_SOCKET, SO_MAX_MSG_SIZE, (char *) &maxRawMsgSize, &maxRawMsgSizeLen))
    {
        (void) closesocket(platformSocketHandle);
        CsrIpSocketRawNewCfmSend2(req->appHandle,
            0 /* Invalid platformSocketHandle handle */,
            CSR_RESULT_FAILURE);
    }
    else
    {
        CsrIpSocketInstanceData *sockInst;

        csrIpInstLock(instanceData);
        sockInst = csrIpSocketInstNew(instanceData,
            req->appHandle,
            req->socketFamily,
            CSR_IP_SOCKET_INST_TYPE_RAW,
            platformSocketHandle);
        csrIpInstUnlock(instanceData);

        if (sockInst && csrIpSocketRegisterEvents(instanceData, sockInst, FD_READ))
        {
            sockInst->maxUdpMsgSize = CSRMAX(maxRawMsgSize, 0xFFFF);
            CsrIpSocketRawNewCfmSend2(req->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_SUCCESS);
        }
        else
        {
            (void) closesocket(platformSocketHandle);
            CsrIpSocketRawNewCfmSend2(req->appHandle,
                0 /* Invalid platformSocketHandle handle */,
                CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS);
        }
    }
}

void CsrIpSocketRawBindReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketRawBindReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
#if 0
        SOCKADDR *sa;
        SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
        SOCKADDR_IN6 sa6;
#endif
        int saLen;

#if _MSC_VER >= 1300
        if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
        {
            sa4.sin_family = AF_INET;
            sa4.sin_port = htons(0); /* Not used */
            CsrMemCpy(&sa4.sin_addr.s_addr, req->ipAddress,
                sizeof(sa4.sin_addr.s_addr));
            CsrMemSet(sa4.sin_zero, 0, sizeof(sa4.sin_zero));

            sa = (SOCKADDR *) &sa4;
            saLen = sizeof(sa4);
        }
#if _MSC_VER >= 1300
        else
        {
            CsrMemSet(&sa6, 0, sizeof(sa6));
            sa6.sin6_family = AF_INET6;
            /* sa6.sin6_port not used but alread zeroed. */
            CsrMemCpy(&sa6.sin6_addr, req->ipAddress, sizeof(sa6.sin6_addr));

            sa = (SOCKADDR *) &sa6;
            saLen = sizeof(sa6);
        }
#endif

        csrIpInstLock(instanceData);

        /* Ref: http://msdn.microsoft.com/en-us/library/ms740548%28VS.85%29.aspx
         *
         * "On Windows 7, Windows Server 2008 R2, Windows Vista, and Windows XP
         * with Service Pack 2 (SP2), the ability to send traffic over raw
         * sockets has been restricted in several ways:
         *
         * ......
         * A call to the bind function with a raw socket is not allowed."
         * ......
         *
         */
        if (bind(sockInst->platformSocketHandle, sa, saLen))
        {
            CsrIpSocketRawBindCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_FAILURE);
        }
        else
#endif
        {
            CSR_IP_SOCKET_STATE_CHANGE(sockInst->state,
                CSR_IP_SOCKET_INST_STATE_BIND);
            CsrIpSocketRawBindCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_SUCCESS);
        }
#if 0
        csrIpInstUnlock(instanceData);
#endif
    }
}

void CsrIpSocketRawDataReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketRawDataReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst != NULL)
    {
        sockInst->data = req->data;
        sockInst->dataLength = req->dataLength;
        sockInst->dataOffset = 0;
        CsrMemCpy(sockInst->ipAddress, req->ipAddress,
            sizeof(sockInst->ipAddress));
        req->data = NULL;

        if (!sockInst || !csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_ACTIVE))
        {
            CsrPmemFree(sockInst->data);
            sockInst->data = NULL;
            CsrIpSocketRawDataCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle);
        }
    }
    else
    {
        CsrPmemFree(req->data);
        req->data = NULL;
    }

    csrIpInstUnlock(instanceData);
}

void CsrIpSocketRawCloseReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketRawCloseReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        if (!csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_CLOSE))
        {
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "No sockinst found for %u", req->socketHandle));
    }
    csrIpInstUnlock(instanceData);
}

void CsrIpSocketOptionsReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketOptionsReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        int argSize;
        BOOL nodelaySetting, broadcastSetting;
        DWORD keepAliveSetting;
        int winSetting, dscpSetting;

        if ((sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_TCP) &&
            req->validOptions & CSR_IP_SOCKET_OPTION_NAGLE)
        {
            BOOL nodelay;

            nodelay = !req->nagle;

            setsockopt(sockInst->platformSocketHandle,
                IPPROTO_TCP,
                TCP_NODELAY,
                (char *) &nodelay,
                sizeof(nodelay));
        }

        if (req->validOptions & CSR_IP_SOCKET_OPTION_KEEPALIVE)
        {
            DWORD keepAlive;

            keepAlive = req->keepAlive;

            setsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_KEEPALIVE,
                (char *) &keepAlive,
                sizeof(keepAlive));
        }

        if (req->validOptions & CSR_IP_SOCKET_OPTION_BROADCAST)
        {
            BOOL broadcast;

            broadcast = req->broadcast;

            setsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_BROADCAST,
                (char *) &broadcast,
                sizeof(broadcast));
        }

        if (req->validOptions & CSR_IP_SOCKET_OPTION_DSCP)
        {
            int dscp;

            dscp = req->dscp;
            dscp <<= 2;

            setsockopt(sockInst->platformSocketHandle,
                IPPROTO_IP,
                IP_TOS,
                (char *) &dscp,
                sizeof(dscp));
        }

        if (req->validOptions & CSR_IP_SOCKET_OPTION_RXWIN)
        {
            int rxwin;

            rxwin = req->rxWindow;

            setsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_RCVBUF,
                (char *) &rxwin,
                sizeof(rxwin));
        }

        if (req->validOptions & CSR_IP_SOCKET_OPTION_TXWIN)
        {
            int txwin;

            txwin = req->txWindow;

            setsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_SNDBUF,
                (char *) &txwin,
                sizeof(txwin));
        }


        /*
         * Read out settings
         */
        req->validOptions = 0;

        argSize = sizeof(winSetting);
        if (getsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_SNDBUF,
                (char *) &winSetting,
                &argSize) == 0)
        {
            req->txWindow = winSetting;
            req->validOptions |= CSR_IP_SOCKET_OPTION_TXWIN;
        }

        argSize = sizeof(winSetting);
        if (getsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_RCVBUF,
                (char *) &winSetting,
                &argSize) == 0)
        {
            req->rxWindow = winSetting;
            req->validOptions |= CSR_IP_SOCKET_OPTION_RXWIN;
        }

        argSize = sizeof(keepAliveSetting);
        if (getsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_KEEPALIVE,
                (char *) &keepAliveSetting,
                &argSize) == 0)
        {
            req->keepAlive = (CsrBool) keepAliveSetting;
            req->validOptions |= CSR_IP_SOCKET_OPTION_KEEPALIVE;
        }

        argSize = sizeof(nodelaySetting);
        if ((sockInst->socketType == CSR_IP_SOCKET_INST_TYPE_TCP) &&
            (getsockopt(sockInst->platformSocketHandle,
                 IPPROTO_TCP,
                 TCP_NODELAY,
                 (char *) &nodelaySetting,
                 &argSize) == 0))
        {
            req->nagle = !nodelaySetting;
            req->validOptions |= CSR_IP_SOCKET_OPTION_NAGLE;
        }

        argSize = sizeof(broadcastSetting);
        if (getsockopt(sockInst->platformSocketHandle,
                SOL_SOCKET,
                SO_BROADCAST,
                (char *) &broadcastSetting,
                &argSize) == 0)
        {
            req->broadcast = broadcastSetting;
            req->validOptions |= CSR_IP_SOCKET_OPTION_BROADCAST;
        }

        argSize = sizeof(dscpSetting);
        if (getsockopt(sockInst->platformSocketHandle,
                IPPROTO_IP,
                IP_TOS,
                (char *) &dscpSetting,
                &argSize) == 0)
        {
            req->dscp = dscpSetting;
            req->validOptions |= CSR_IP_SOCKET_OPTION_DSCP;
        }


        CsrIpSocketOptionsCfmSend2(sockInst->appHandle,
            req->socketHandle,
            req->txWindow,
            req->rxWindow,
            req->nagle,
            req->keepAlive,
            req->broadcast,
            req->dscp,
            req->validOptions);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrIpLto, 0, "No sockinst found for %u", req->socketHandle));
    }
    csrIpInstUnlock(instanceData);
}

void CsrIpSocketTcpNewReqHandler(CsrIpInstanceData *instanceData)
{
    SOCKET platformSocketHandle;
    CsrIpSocketTcpNewReq *req = instanceData->msg;
    int reuseAddr = TRUE;

    if (req->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
    {
        platformSocketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else
#if _MSC_VER >= 1300
    {
        platformSocketHandle = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    }
#else
    {
        CsrIpSocketTcpNewCfmSend2(req->appHandle, 0,
            CSR_IP_SOCKET_RESULT_IP6_NOT_SUPPORTED);
        return;
    }
#endif
    if ((platformSocketHandle == INVALID_SOCKET) ||
        setsockopt(platformSocketHandle, SOL_SOCKET, SO_REUSEADDR, (char *) &reuseAddr, sizeof(reuseAddr)))
    {
        (void) closesocket(platformSocketHandle);
        CsrIpSocketTcpNewCfmSend2(req->appHandle,
            0 /* Invalid platformSocketHandle handle */,
            CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS);
    }
    else
    {
        CsrIpSocketInstanceData *sockInst;

        csrIpInstLock(instanceData);
        sockInst = csrIpSocketInstNew(instanceData,
            req->appHandle,
            req->socketFamily,
            CSR_IP_SOCKET_INST_TYPE_TCP,
            platformSocketHandle);
        csrIpInstUnlock(instanceData);

        if (sockInst)
        {
            CsrIpSocketTcpNewCfmSend2(req->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_SUCCESS);
        }
        else
        {
            (void) closesocket(platformSocketHandle);
            CsrIpSocketTcpNewCfmSend2(req->appHandle,
                0 /* Invalid platformSocketHandle handle */,
                CSR_IP_SOCKET_RESULT_NO_MORE_SOCKETS);
        }
    }
}

void CsrIpSocketTcpBindReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpBindReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        SOCKADDR *sa;
        SOCKADDR_IN sa4;
#if _MSC_VER >= 1300
        SOCKADDR_IN6 sa6;
#endif
        int saLen;

#if _MSC_VER >= 1300
        if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
        {
            sa4.sin_family = AF_INET;
            sa4.sin_port = htons(req->port);
            CsrMemCpy(&sa4.sin_addr.s_addr, req->ipAddress,
                sizeof(sa4.sin_addr.s_addr));
            CsrMemSet(sa4.sin_zero, 0, sizeof(sa4.sin_zero));

            sa = (SOCKADDR *) &sa4;
            saLen = sizeof(sa4);
        }
#if _MSC_VER >= 1300
        else
        {
            CsrMemSet(&sa6, 0, sizeof(sa6));
            sa6.sin6_family = AF_INET6;
            sa6.sin6_port = htons(req->port);
            CsrMemCpy(&sa6.sin6_addr, req->ipAddress, sizeof(sa6.sin6_addr));

            sa = (SOCKADDR *) &sa6;
            saLen = sizeof(sa6);
        }
#endif

        if (bind(sockInst->platformSocketHandle, sa, saLen))
        {
            CsrIpSocketTcpBindCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_IP_SOCKET_RESULT_PORT_IN_USE,
                req->port);
        }
        else
        {
            if (req->port == 0)
            {
                (void) getsockname(sockInst->platformSocketHandle,
                    sa, &saLen);

#if _MSC_VER >= 1300
                if (sockInst->socketFamily == CSR_IP_SOCKET_FAMILY_IP4)
#endif
                {
                    req->port = ntohs(sa4.sin_port);
                }
#if _MSC_VER >= 1300
                else
                {
                    req->port = ntohs(sa6.sin6_port);
                }
#endif
            }

            CSR_IP_SOCKET_STATE_CHANGE(sockInst->state, CSR_IP_SOCKET_INST_STATE_BIND);
            CsrIpSocketTcpBindCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_SUCCESS,
                req->port);
        }
    }
}

void CsrIpSocketTcpListenReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpListenReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    csrIpInstLock(instanceData);

    if (sockInst)
    {
        if (listen(sockInst->platformSocketHandle, SOMAXCONN))
        {
            CsrIpSocketTcpListenCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_FAILURE)
        }
        else
        {
            if (!csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_LISTEN))
            {
                CsrIpSocketTcpListenCfmSend2(sockInst->appHandle,
                    sockInst->appSocketHandle,
                    CSR_RESULT_FAILURE);
            }
        }
    }
    csrIpInstUnlock(instanceData);
}

void CsrIpSocketTcpConnectReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpConnectReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        CsrMemCpy(sockInst->ipAddress, req->ipAddress,
            sizeof(sockInst->ipAddress));
        sockInst->port = req->port;

        if (!csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_CONNECT))
        {
            CsrIpSocketTcpConnectCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_FAILURE)
        }
    }
}

void CsrIpSocketTcpDataReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpDataReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if ((sockInst != NULL) &&
        (sockInst->state != CSR_IP_SOCKET_INST_STATE_DEAD))
    {
        sockInst->data = req->data;
        sockInst->dataLength = req->dataLength;
        sockInst->dataOffset = 0;
        req->data = NULL;

        if (!sockInst || !csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_ACTIVE))
        {
            CsrPmemFree(sockInst->data);
            sockInst->data = NULL;
            sockInst->dataLength = 0;
            CsrIpSocketTcpDataCfmSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                CSR_RESULT_FAILURE)
        }
    }
    else
    {
        CsrPmemFree(req->data);
        req->data = NULL;
    }

    csrIpInstUnlock(instanceData);
}

void CsrIpSocketTcpDataResHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpDataRes *res = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;
    csrIpRxElement *rxelem;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, res->socketHandle);

    if (sockInst != NULL)
    {
        rxelem = sockInst->rxQueue;
        sockInst->rxQueue = rxelem->next;

        if (sockInst->rxQueue != NULL)
        {
            /* More data to push up */
            CsrIpSocketTcpDataIndSend2(sockInst->appHandle,
                sockInst->appSocketHandle,
                sockInst->rxQueue->bufLen,
                sockInst->rxQueue->buf);
        }
        else
        {
            if (sockInst->state == CSR_IP_SOCKET_INST_STATE_DEAD)
            {
                /* The socket is dead, now is the time to inform the app. */
                CsrIpSocketTcpCloseIndSend2(sockInst->appHandle, sockInst->appSocketHandle);
                csrIpSocketCloseAndFreeSocket(instanceData, sockInst);
            }
        }

        CsrPmemFree(rxelem);
    }

    csrIpInstUnlock(instanceData);
}

void CsrIpSocketTcpCloseReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpCloseReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        if (!csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_CLOSE))
        {
        }
    }
    csrIpInstUnlock(instanceData);
}

void CsrIpSocketTcpAbortReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpSocketTcpAbortReq *req = instanceData->msg;
    CsrIpSocketInstanceData *sockInst;

    csrIpInstLock(instanceData);

    sockInst = csrIpSocketInstFind(instanceData, req->socketHandle);

    if (sockInst)
    {
        if (!csrIpSocketThreadReschedule(instanceData, sockInst, CSR_IP_SOCKET_INST_STATE_ABORT))
        {
        }
    }
    csrIpInstUnlock(instanceData);
}
