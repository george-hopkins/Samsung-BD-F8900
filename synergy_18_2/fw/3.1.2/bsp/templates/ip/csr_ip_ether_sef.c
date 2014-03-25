/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_framework_ext.h"

#include "csr_ip_handler.h"
#include "csr_ip_ether_prim.h"
#include "csr_ip_ether_lib.h"

/* PORTING: Set to the desired encapsulation format for both tx and rx. Can be
   any of the CSR_IP_ETHER_ENCAPSULATION_* defines in csr_ip_ether_prim.h. This
   will override the encapsulation suggested by the network interface. */
#define ENCAPSULATION CSR_IP_ETHER_ENCAPSULATION_LLC_SNAP

/* Mutex for synchronising between the threads */
static CsrMutexHandle mutex;

/* Static local storage of the information received in the CsrIpEtherIfAddReq
   message. */
/* static CsrUint16 ifType; */
/* static CsrUint8 mac[6]; */
/* static CsrUint8 encapsulation; */
/* static CsrUint16 maxTxUnit; */
static CsrIpEtherIfContext ifContext; /* Context that must be passed to frameTxFunction */
static CsrIpEtherFrameTxFunction frameTxFunction; /* Function for transmitting frames */

/* State of the link between the IP stack and the network interface */
#define STATE_UNAVAILABLE      0 /* Network interface is unavailable */
#define STATE_LINK_DOWN        1 /* Network interface is available, but link is down */
#define STATE_LINK_UP_OPEN     2 /* Link is up and frames can be transmitted */
#define STATE_LINK_UP_CLOSED   3 /* Link is up, but no frames can be transmitted */
static CsrUint8 linkState = STATE_UNAVAILABLE;

/* Individial priority queue flow control state */
static CsrIpEtherPriority flowControlState = 0;


/*******************************************************************************

    NAME
        CsrIpEtherDeinit

    DESCRIPTION
        This function is called exactly once before the first message is
        received.

*******************************************************************************/
void CsrIpEtherInit(void)
{
    CsrMutexCreate(&mutex);
}

/*******************************************************************************

    NAME
        CsrIpEtherDeinit

    DESCRIPTION
        This function is called exactly once after the final message has been
        received prior to the scheduler shutting down.

*******************************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrIpEtherDeinit(void)
{
    CsrMutexDestroy(&mutex);
}

#endif


/*******************************************************************************

    NAME
        frameRxFunction

    DESCRIPTION
        This function is called by the network interface to deliver a received
        ethernet frame to the IP stack.

    PARAMETERS
        destinationMac - Pointer to first byte of destination MAC address.
        sourceMac - Pointer to first byte of source MAC address.
        rssi - Radio Signal Strength Indication.
        frameLength - length in byte of frame pointer.
        frame - Pointer to first byte of frame (not including MAC addresses).
        ipContext - Pointer to context passed in the CSR_IP_ETHER_IF_ADD_CFM.
                    This can be set to anything that is useful for this module.

*******************************************************************************/
static void frameRxFunction(CsrUint8 *destinationMac, CsrUint8 *sourceMac, CsrInt16 rssi, CsrUint16 frameLength, CsrUint8 *frame, CsrIpEtherIpContext ipContext)
{
    /* PORTING: Add code to pass the received ethernet frame to the IP stack
       here. The frame pointer points at the first byte of the frame following
       the destination and source MAC addresses. This function must copy the
       contents before returning. The memory ownership remains with the caller,
       so no attempts must be made to free the memory of any of the arguments. */
}

/*******************************************************************************

    NAME
        CsrIpEtherFrameTxFunctionWrapper

    DESCRIPTION
        PORTING: This function can be called by the IP stack to deliver an
        ethernet frame to the interface for transmission. The parameter list and
        return value can be adjusted, along with the implementation, to anything
        that is suitable for the particular IP stack.

    PARAMETERS
        destinationMac - Pointer to first byte of destination MAC address.
        sourceMac - Pointer to first byte of source MAC address.
        frameLength - length in byte of frame pointer.
        frame - Pointer to first byte of frame (not including MAC addresses).

*******************************************************************************/
void CsrIpEtherFrameTxFunctionWrapper(CsrUint8 *destinationMac, CsrUint8 *sourceMac, CsrUint16 frameLength, CsrUint8 *frame)
{
    CsrMutexLock(&mutex);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            /* The interface is not yet available. */
            break;
        case STATE_LINK_DOWN:
            /* The link is not up. */
            break;
        case STATE_LINK_UP_CLOSED:
            /* PORTING: The link is closed due to flow control on at least one
               priority queue. Depending on the priority of the frame, it must
               either be enqueued or dropped here or the return value from
               this function must cause the IP stack to enqueue (or drop) it. */
            break;
        case STATE_LINK_UP_OPEN:
        {
            CsrResult result;

            /* Transmit the frame. The callee will copy the contents, and memory
               ownership remains with the caller (here), so it is the
               responsibility of this module to free the memory, if and only if
               necessary. */
            result = frameTxFunction(destinationMac, sourceMac, frameLength, frame, ifContext);
            if (result == CSR_IP_ETHER_RESULT_NOT_SENT)
            {
                /* PORTING: The frame was not transmitted due to flow control.
                   The frame must either be enqueued or dropped here or the
                   return value from this function must cause the IP stack to
                   enqueue (or drop) it. */
            }
            break;
        }
        default:
            break;
    }

    CsrMutexUnlock(&mutex);
}

/*******************************************************************************

    NAME
        CsrIpEtherIfAddReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_ADD_REQ message is
        received. This message is received when the interface becomes available.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfAddReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfAddReq *request = instanceData->msg;

    CsrMutexLock(&mutex);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
        {
            /* ifType = request->ifType; */ /* Type of network interface */
            /* CsrMemCpy(mac, request->mac, 6); */ /* MAC address of the network interface */
            /* encapsulation = request->encapsulation; */ /* Encapsulation suggested by network interface */
            /* maxTxUnit = request->maxTxUnit; */ /* The maximum transmission unit */
            frameTxFunction = request->frameTxFunction;
            ifContext = request->ifContext;
            linkState = STATE_LINK_DOWN;
            /* PORTING: The IP stack should be notified that the interface is
               available. The above information can be passed into the IP
               stack if required. Note that the suggested encapsulation is
               ignored and the define ENCAPSULATION is used for forcing the
               network interface to use the enapsulation format that suits
               the IP stack. The final argument in the confirm message is a
               void pointer that can be set to anything. The void pointer
               will then be passed to the frameRxFunction above, when a
               frame is received. */
            CsrIpEtherIfAddCfmSend(request->appHandle,
                0 /* ifHandle */,
                CSR_RESULT_SUCCESS,
                ENCAPSULATION,
                frameRxFunction,
                NULL /* ipContext */);
            break;
        }
        case STATE_LINK_DOWN:
        case STATE_LINK_UP_CLOSED:
        case STATE_LINK_UP_OPEN:
        default:
        {
            CsrIpEtherIfAddCfmSend(request->appHandle,
                0,
                CSR_IP_ETHER_RESULT_NO_MORE_INTERFACES,
                request->encapsulation,
                NULL,
                NULL);
            break;
        }
    }

    CsrMutexUnlock(&mutex);
}

/*******************************************************************************

    NAME
        CsrIpEtherIfRemoveReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_REMOVE_REQ message is
        received. This message is received if the interface becomes unavailable.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfRemoveReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfRemoveReq *request = instanceData->msg;

    CsrMutexLock(&mutex);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
        {
            CsrIpEtherIfRemoveCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
            break;
        }
        case STATE_LINK_DOWN:
        case STATE_LINK_UP_CLOSED:
        case STATE_LINK_UP_OPEN:
        default:
        {
            frameTxFunction = NULL;
            ifContext = NULL;
            linkState = STATE_UNAVAILABLE;
            /* PORTING: The IP stack should be notified that the interface is no
               longer available. */
            CsrIpEtherIfRemoveCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
            break;
        }
    }

    CsrMutexUnlock(&mutex);
}

/*******************************************************************************

    NAME
        CsrIpEtherIfLinkUpReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_LINK_UP_REQ message is
        received. This message is received when the media is connected on the
        interface, indicating that it is ready to transmit/receive ethernet
        frames.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfLinkUpReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfLinkUpReq *request = instanceData->msg;

    CsrMutexLock(&mutex);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
        {
            CsrIpEtherIfLinkUpCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
            break;
        }
        case STATE_LINK_DOWN:
        case STATE_LINK_UP_CLOSED:
        case STATE_LINK_UP_OPEN:
        default:
        {
            linkState = STATE_LINK_UP_OPEN;
            flowControlState = 0;
            /* PORTING: The IP stack should be notified that the link is now
               up and open, and frame transmission can begin. */
            CsrIpEtherIfLinkUpCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
            break;
        }
    }

    CsrMutexUnlock(&mutex);
}

/*******************************************************************************

    NAME
        CsrIpEtherIfLinkDownReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_LINK_DOWN_REQ message is
        received. This message is received when the media is disconnected on the
        interface, indicating that it is not possible to transmit/receive
        ethernet frames.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfLinkDownReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfLinkDownReq *request = instanceData->msg;

    CsrMutexLock(&mutex);

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
        {
            CsrIpEtherIfLinkDownCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
            break;
        }
        case STATE_LINK_DOWN:
        case STATE_LINK_UP_CLOSED:
        case STATE_LINK_UP_OPEN:
        default:
        {
            linkState = STATE_LINK_DOWN;
            /* PORTING: The IP stack should be notified that the link is now
               down, and frame transmission should stop. */
            CsrIpEtherIfLinkDownCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
            break;
        }
    }

    CsrMutexUnlock(&mutex);
}

/*******************************************************************************

    NAME
        CsrIpEtherIfMulticastAddrAddResHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES
        message is received. This message will be received in response to a
        CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfMulticastAddrAddResHandler(CsrIpInstanceData *instanceData)
{
    /* CsrIpEtherIfMulticastAddrAddRes *response = instanceData->msg; */
}

/*******************************************************************************

    NAME
        CsrIpEtherIfMulticastAddrRemoveResHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES
        message is received. This message will be received in response to a
        CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfMulticastAddrRemoveResHandler(CsrIpInstanceData *instanceData)
{
    /* CsrIpEtherIfMulticastAddrRemoveRes *response = instanceData->msg; */
}

/*******************************************************************************

    NAME
        CsrIpEtherIfMulticastAddrFlushResHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES
        message is received. This message will be received in response to a
        CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfMulticastAddrFlushResHandler(CsrIpInstanceData *instanceData)
{
    /* CsrIpEtherIfMulticastAddrFlushRes *response = instanceData->msg; */
}

/*******************************************************************************

    NAME
        CsrIpEtherIfFlowControlPauseReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ
        message is received. This message will be received when one or several
        priority queues are full, causing the corresponding queues to go into
        a flow controlled state, where ethernet frames cannot be sent.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfFlowControlPauseReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfFlowControlPauseReq *request = instanceData->msg;

    CsrMutexLock(&mutex);

    flowControlState |= request->priority;

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            break;
        case STATE_LINK_DOWN:
            break;
        case STATE_LINK_UP_CLOSED:
            break;
        case STATE_LINK_UP_OPEN:
        {
            linkState = STATE_LINK_UP_CLOSED;
            /* PORTING: The link is now closed due to flow control on at least
               one priority queue. It may be necessary to notify the IP stack
               of this, to stop it from transmitting frames on flow controlled
               priority queues. */
            break;
        }
        default:
            break;
    }

    CsrMutexUnlock(&mutex);
}

/*******************************************************************************

    NAME
        CsrIpEtherIfFlowControlResumeReqHandler

    DESCRIPTION
        This function is called when a CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ
        message is received. This message will be received when one or several
        priority queues are again open for transmission of ethernet frames.

    PARAMETERS
        instanceData - Task instance data.

*******************************************************************************/
void CsrIpEtherIfFlowControlResumeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfFlowControlResumeReq *request = instanceData->msg;

    CsrMutexLock(&mutex);

    flowControlState &= ~request->priority;

    switch (linkState)
    {
        case STATE_UNAVAILABLE:
            break;
        case STATE_LINK_DOWN:
            break;
        case STATE_LINK_UP_CLOSED:
        {
            if (flowControlState == 0)
            {
                linkState = STATE_LINK_UP_OPEN;
                /* PORTING: Dequeue and transmit any frames that were queued, and/or
                   notify the IP stack that it is possible to transmit frames again,
                   causing it to retransmit any frames it may have enqueued. */
            }
            break;
        }
        case STATE_LINK_UP_OPEN:
            break;
        default:
            break;
    }

    CsrMutexUnlock(&mutex);
}
