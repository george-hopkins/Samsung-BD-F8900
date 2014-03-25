/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_ipcp.h"

/* ncp codes */
#define NCP_CONFIGURATION_REQUEST        0x01
#define NCP_CONFIGURATION_ACK            0x02
#define NCP_CONFIGURATION_NACK            0x03
#define NCP_CONFIGURATION_REJECT        0x04
#define NCP_TERMINATE_REQUEST            0x05
#define NCP_TERMINATE_ACK                0x06
#define NCP_CODE_REJECT                    0x07

/* ncp ipcp option types */
#define IPCP_OPTION_IP_ADDRESSES                0x01
#define IPCP_OPTION_IP_COMPRESSION_PROTOCOL        0x02
#define IPCP_OPTION_IP_ADDRESS                    0x03


#define MAX_NUMBER_OF_NCP_CONFIGURATION_RETRANSMISSIONS        10
#define MAX_NUMBER_OF_NCP_TERMINATION_RETRANSMISSIONS        2
#define NCP_RETRANSMISSION_TIMEOUT                            3000000


extern void StartIpStack(CsrBtNcpInstanceData * instanceData);
extern void CloseDownIpStack(CsrBtNcpInstanceData * instanceData);


static void ncpTerminateRequestSend(CsrBtNcpInstanceData * instanceData)
{
    CsrUint8 * payload;

    payload = (CsrUint8 *) CsrPmemAlloc(4);
    payload[0] = NCP_TERMINATE_REQUEST;
    payload[1] = instanceData->currentIdentifier;
    payload[2] = 0;
    payload[3] = 4;
    PppNcpReqSend(CSR_BT_LAN_WORD(CSR_BT_IPCP_NCP_PROTOCOL), 4, payload);
}

static void ncpTerminateAcknowledgementSend(CsrBtNcpInstanceData * instanceData)
{
    CsrBtPppNcpInd * ncpInd;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;
    ncpInd->payload[0] = NCP_TERMINATE_ACK;
    PppNcpReqSend(CSR_BT_LAN_WORD(CSR_BT_IPCP_NCP_PROTOCOL), ncpInd->payloadLength, ncpInd->payload);
    ncpInd->payload = NULL;
}

static void ncpCodeRejectSend(CsrBtNcpInstanceData * instanceData)
{
    CsrBtPppNcpInd * ncpInd;
    CsrUint8 * codeRejectPayload;
    CsrUint16 maxLength;
    CsrUint16 index;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;
    maxLength = ncpInd->payloadLength < instanceData->maxPayloadLength - 4 ? ncpInd->payloadLength : instanceData->maxPayloadLength - 4;

    codeRejectPayload = (CsrUint8 *) CsrPmemAlloc(4 + maxLength);
    codeRejectPayload[0] = NCP_CODE_REJECT;
    codeRejectPayload[1] = ncpInd->payload[0];    /* use the rejected code as identifier - should be different every time unless the peer does not listen to the reject! */
    codeRejectPayload[2] = ((CsrUint8) (((4 + maxLength) >> 8) & 0x00FF));
    codeRejectPayload[3] = ((CsrUint8) ((4 + maxLength) & 0x00FF));
    for (index = 0; index < maxLength; index++)
    {
        codeRejectPayload[4 + index] = ncpInd->payload[index];
    }

    PppNcpReqSend(CSR_BT_LAN_WORD(CSR_BT_IPCP_NCP_PROTOCOL), ((CsrUint16) (4 + maxLength)), codeRejectPayload);
}

static void ncpIpConfigurationRequestSend(CsrBtNcpInstanceData * instanceData)
{
    CsrUint8 * payload;

    if (instanceData->configureIpAddress)
    {
        payload = (CsrUint8 *) CsrPmemAlloc(10);
        payload[0] = NCP_CONFIGURATION_REQUEST;
        payload[1] = instanceData->currentIdentifier;
        payload[2] = 0;
        payload[3] = 10;
        payload[4] = IPCP_OPTION_IP_ADDRESS;
        payload[5] = 6;
        payload[6] = instanceData->assignedIpAddress[0];
        payload[7] = instanceData->assignedIpAddress[1];
        payload[8] = instanceData->assignedIpAddress[2];
        payload[9] = instanceData->assignedIpAddress[3];
        PppNcpReqSend(CSR_BT_LAN_WORD(CSR_BT_IPCP_NCP_PROTOCOL), 10, payload);
    }
    else
    {
        payload = (CsrUint8 *) CsrPmemAlloc(4);
        payload[0] = NCP_CONFIGURATION_REQUEST;
        payload[1] = instanceData->currentIdentifier;
        payload[2] = 0;
        payload[3] = 4;
        PppNcpReqSend(CSR_BT_LAN_WORD(CSR_BT_IPCP_NCP_PROTOCOL), 4, payload);
    }
}

static void ncpRetransmission(CsrUint16 retransmissionCount, void * instanceDataPointer)
{
    CsrBtNcpInstanceData * instanceData;

    instanceData = (CsrBtNcpInstanceData *) instanceDataPointer;
    if (retransmissionCount)
    {
        switch (instanceData->ncpState)
        {
            case CSR_BT_NCP_STATE_STOPPING:
                ncpTerminateRequestSend(instanceData);
                break;

            case CSR_BT_NCP_STATE_REQ_SENT:
            case CSR_BT_NCP_STATE_ACK_RCVD:
            case CSR_BT_NCP_STATE_ACK_SENT:
                ncpIpConfigurationRequestSend(instanceData);
                break;
        }
        instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, --retransmissionCount, instanceDataPointer);
    }
    else
    {
        PppDisconnectReqSend();
        instanceData->ncpTimer = 0;
        instanceData->ncpState = CSR_BT_NCP_STATE_STOPPING;
    }
}

static CsrUint16 ncpConfigureRequestRejectSize(CsrBtNcpInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;
    CsrUint16 rejectSize;
    CsrBtPppNcpInd * ncpInd;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;

    rejectSize = 0;
    optionsLength = (((CsrUint16) ncpInd->payload[2]) << 8) + ncpInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (ncpInd->payload[currentIndex])
        {
            case IPCP_OPTION_IP_ADDRESS:
                break;

            default:
                rejectSize += ((CsrUint16) ncpInd->payload[currentIndex + 1]);
                break;
        }
        currentIndex += ncpInd->payload[currentIndex + 1];
    }

    return rejectSize;
}

static void ncpCopyConfigurationRejectOptions(CsrBtNcpInstanceData * instanceData, CsrUint8 * data)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;
    CsrUint16 rejectIndex;
    CsrBtPppNcpInd * ncpInd;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;

    optionsLength = (((CsrUint16) ncpInd->payload[2]) << 8) + ncpInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (ncpInd->payload[currentIndex])
        {
            case IPCP_OPTION_IP_ADDRESS:
                break;

            default:
                for (rejectIndex = 0; rejectIndex < ((CsrUint16) ncpInd->payload[currentIndex + 1]); rejectIndex++)
                {
                    *data++ = ncpInd->payload[currentIndex + rejectIndex];
                }
                break;
        }
        currentIndex += ncpInd->payload[currentIndex + 1];
    }
}

static CsrUint16 ncpConfigureRequestNackSize(CsrBtNcpInstanceData * instanceData)
{
    return 0;
}

static void ncpProcessConfigurationRequest(CsrBtNcpInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;
    CsrBtPppNcpInd * ncpInd;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;

    optionsLength = (((CsrUint16) ncpInd->payload[2]) << 8) + ncpInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (ncpInd->payload[currentIndex])
        {
            case IPCP_OPTION_IP_ADDRESS:
                instanceData->peerIpAddress[0] = ncpInd->payload[currentIndex + 2];
                instanceData->peerIpAddress[1] = ncpInd->payload[currentIndex + 3];
                instanceData->peerIpAddress[2] = ncpInd->payload[currentIndex + 4];
                instanceData->peerIpAddress[3] = ncpInd->payload[currentIndex + 5];
                break;
        }
        currentIndex += ncpInd->payload[currentIndex + 1];
    }
}

static void ncpHandleConfigurationRequest(CsrBtNcpInstanceData * instanceData)
{
    CsrUint16 size;
    CsrBtPppNcpInd * ncpInd;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;

    if ((size = ncpConfigureRequestRejectSize(instanceData)) > 0)
    {
        switch (instanceData->ncpState)
        {
            case CSR_BT_NCP_STATE_STOPPING:
                break;

            case CSR_BT_NCP_STATE_OPENED:
                CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
                ncpIpConfigurationRequestSend(instanceData);
                instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            case CSR_BT_NCP_STATE_ACK_SENT:
                instanceData->ncpState = CSR_BT_NCP_STATE_REQ_SENT;
                break;
        }
        switch (instanceData->ncpState)
        {
            case CSR_BT_NCP_STATE_STOPPING:
                break;

            case CSR_BT_NCP_STATE_REQ_SENT:
            case CSR_BT_NCP_STATE_ACK_RCVD:
            {
                CsrUint8 * configurationRejectPayload;
                CsrUint16 index;

                index = 0;
                size += 4;
                configurationRejectPayload = (CsrUint8 *) CsrPmemAlloc(size);
                configurationRejectPayload[index++] = NCP_CONFIGURATION_REJECT;
                configurationRejectPayload[index++] = ncpInd->payload[1];
                configurationRejectPayload[index++] = ((CsrUint8) ((size >> 8) & 0x00FF));
                configurationRejectPayload[index++] = ((CsrUint8) (size & 0x00FF));

                ncpCopyConfigurationRejectOptions(instanceData, &configurationRejectPayload[index]);

                PppNcpReqSend(CSR_BT_LAN_WORD(CSR_BT_IPCP_NCP_PROTOCOL), size, configurationRejectPayload);
                break;
            }
        }
    }
    else
    {
        if ((size = ncpConfigureRequestNackSize(instanceData)) > 0)
        {
            ; /* we don't nack at the moment, because we will accept any IP address and reject all other codes! */
        }
        else
        {
            ncpProcessConfigurationRequest(instanceData);
            switch (instanceData->ncpState)
            {
                case CSR_BT_NCP_STATE_STOPPING:
                    break;

                case CSR_BT_NCP_STATE_OPENED:
                    CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
                    ncpIpConfigurationRequestSend(instanceData);
                    instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
                case CSR_BT_NCP_STATE_REQ_SENT:
                    instanceData->ncpState = CSR_BT_NCP_STATE_ACK_SENT;
                case CSR_BT_NCP_STATE_ACK_RCVD:
                case CSR_BT_NCP_STATE_ACK_SENT:
                {
                    ncpInd->payload[0] = NCP_CONFIGURATION_ACK;
                    PppNcpReqSend(CSR_BT_LAN_WORD(CSR_BT_IPCP_NCP_PROTOCOL), ncpInd->payloadLength, ncpInd->payload);
                    ncpInd->payload = NULL;
                    break;
                }
            }
            switch (instanceData->ncpState)
            {
                case CSR_BT_NCP_STATE_ACK_RCVD:
                    CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
                    instanceData->ncpState = CSR_BT_NCP_STATE_OPENED;
                    StartIpStack(instanceData);
                    break;
            }
        }
    }
}

static void ncpProcessAcknowledgement(CsrBtNcpInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;
    CsrBtPppNcpInd * ncpInd;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;

    optionsLength = (((CsrUint16) ncpInd->payload[2]) << 8) + ncpInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (ncpInd->payload[currentIndex])
        {
            case IPCP_OPTION_IP_ADDRESS:
                instanceData->assignedIpAddress[0] = ncpInd->payload[currentIndex + 2];
                instanceData->assignedIpAddress[1] = ncpInd->payload[currentIndex + 3];
                instanceData->assignedIpAddress[2] = ncpInd->payload[currentIndex + 4];
                instanceData->assignedIpAddress[3] = ncpInd->payload[currentIndex + 5];
                break;
        }
        currentIndex += ncpInd->payload[currentIndex + 1];
    }
}

static void ncpHandleConfigurationAcknowledgement(CsrBtNcpInstanceData * instanceData)
{
    switch (instanceData->ncpState)
    {
        case CSR_BT_NCP_STATE_STOPPING:
            break;

        case CSR_BT_NCP_STATE_REQ_SENT:
            instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            instanceData->ncpState = CSR_BT_NCP_STATE_ACK_RCVD;
            break;

        case CSR_BT_NCP_STATE_ACK_RCVD:
        case CSR_BT_NCP_STATE_OPENED:
            ncpIpConfigurationRequestSend(instanceData);
            instanceData->ncpState = CSR_BT_NCP_STATE_REQ_SENT;
            break;

        case CSR_BT_NCP_STATE_ACK_SENT:
            instanceData->ncpState = CSR_BT_NCP_STATE_OPENED;
            StartIpStack(instanceData);
            break;
    }
    ncpProcessAcknowledgement(instanceData);
}

static void ncpHandleConfigurationNack(CsrBtNcpInstanceData * instanceData)
{
    ncpProcessAcknowledgement(instanceData);    /* use the proposed value for the next request */
    switch (instanceData->ncpState)
    {
        case CSR_BT_NCP_STATE_STOPPING:
            break;

        case CSR_BT_NCP_STATE_REQ_SENT:
        case CSR_BT_NCP_STATE_ACK_RCVD:
        case CSR_BT_NCP_STATE_ACK_SENT:
        case CSR_BT_NCP_STATE_OPENED:
            ncpIpConfigurationRequestSend(instanceData);
            instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            break;
    }
    switch (instanceData->ncpState)
    {
        case CSR_BT_NCP_STATE_ACK_RCVD:
        case CSR_BT_NCP_STATE_OPENED:
            instanceData->ncpState = CSR_BT_NCP_STATE_REQ_SENT;
            break;
    }
}

static void ncpHandleConfigurationReject(CsrBtNcpInstanceData * instanceData)
{
    switch (instanceData->ncpState)
    {
        case CSR_BT_NCP_STATE_STOPPING:
            break;

        case CSR_BT_NCP_STATE_REQ_SENT:
        case CSR_BT_NCP_STATE_ACK_RCVD:
        case CSR_BT_NCP_STATE_ACK_SENT:
        case CSR_BT_NCP_STATE_OPENED:
            instanceData->configureIpAddress = FALSE;    /* since we only try to configure ip address we don't need to look at the rejected code! */
            ncpIpConfigurationRequestSend(instanceData);
            instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            break;
    }
    switch (instanceData->ncpState)
    {
        case CSR_BT_NCP_STATE_ACK_RCVD:
        case CSR_BT_NCP_STATE_OPENED:
            instanceData->ncpState = CSR_BT_NCP_STATE_REQ_SENT;
            break;
    }
}

static void ncpHandleTerminateRequest(CsrBtNcpInstanceData * instanceData)
{
    if (instanceData->ncpState == CSR_BT_NCP_STATE_OPENED)
    {
        CloseDownIpStack(instanceData);
    }
    ncpTerminateAcknowledgementSend(instanceData);
    instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_TERMINATION_RETRANSMISSIONS - 1, (void *) instanceData);
    instanceData->ncpState = CSR_BT_NCP_STATE_STOPPING;
}

static void ncpHandleTerminateAcknowledgement(CsrBtNcpInstanceData * instanceData)
{
    PppDisconnectReqSend();
}

static void ncpHandleCodeReject(CsrBtNcpInstanceData * instanceData)
{
    switch (instanceData->ncpState)
    {
        case CSR_BT_NCP_STATE_STOPPING:
            break;

        case CSR_BT_NCP_STATE_REQ_SENT:
        case CSR_BT_NCP_STATE_ACK_RCVD:
        case CSR_BT_NCP_STATE_ACK_SENT:
            PppDisconnectReqSend();
            instanceData->ncpState = CSR_BT_NCP_STATE_STOPPING;
            break;

        case CSR_BT_NCP_STATE_OPENED:
            ncpTerminateRequestSend(instanceData);
            instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_TERMINATION_RETRANSMISSIONS - 1, (void *) instanceData);
            instanceData->ncpState = CSR_BT_NCP_STATE_STOPPING;
            CloseDownIpStack(instanceData);
            break;
    }
}

void CsrBtHandleNcpInd(CsrBtNcpInstanceData * instanceData)
{
    CsrBtPppNcpInd * ncpInd;

    ncpInd = (CsrBtPppNcpInd *) instanceData->receivedMessage;

    switch (ncpInd->payload[0])
    {
        case NCP_CONFIGURATION_REQUEST:
            ncpHandleConfigurationRequest(instanceData);
            break;

        case NCP_CONFIGURATION_ACK:
            CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
            instanceData->ncpTimer = 0;
            if (ncpInd->payload[1] == instanceData->currentIdentifier)
            {
                instanceData->currentIdentifier++;
                ncpHandleConfigurationAcknowledgement(instanceData);
            }
            break;

        case NCP_CONFIGURATION_NACK:
            CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
            instanceData->ncpTimer = 0;
            if (ncpInd->payload[1] == instanceData->currentIdentifier)
            {
                instanceData->currentIdentifier++;
                ncpHandleConfigurationNack(instanceData);
            }
            break;

        case NCP_CONFIGURATION_REJECT:
            CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
            instanceData->ncpTimer = 0;
            if (ncpInd->payload[1] == instanceData->currentIdentifier)
            {
                instanceData->currentIdentifier++;
                ncpHandleConfigurationReject(instanceData);
            }
            break;

        case NCP_TERMINATE_REQUEST:
            CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
            instanceData->ncpTimer = 0;
            ncpHandleTerminateRequest(instanceData);
            break;

        case NCP_TERMINATE_ACK:
            CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
            instanceData->ncpTimer = 0;
            if (ncpInd->payload[1] == instanceData->currentIdentifier)
            {
                instanceData->currentIdentifier++;
                ncpHandleTerminateAcknowledgement(instanceData);
            }
            break;

        case NCP_CODE_REJECT:
            CsrSchedTimerCancel(instanceData->ncpTimer,NULL,NULL);
            instanceData->ncpTimer = 0;
            if (ncpInd->payload[1] == instanceData->currentIdentifier)
            {
                instanceData->currentIdentifier++;
                ncpHandleCodeReject(instanceData);
            }
            break;

            default:
                if (instanceData->ncpState != CSR_BT_NCP_STATE_STOPPING)
                {
                    ncpCodeRejectSend(instanceData);
                }
                break;
    }

    CsrPmemFree(ncpInd->payload);
}

void CsrBtStartIpCp(CsrBtNcpInstanceData * instanceData)
{
    instanceData->assignedIpAddress[0] = instanceData->assignedIpAddress[1] = instanceData->assignedIpAddress[2] = instanceData->assignedIpAddress[3] = 0;
    instanceData->peerIpAddress[0] = instanceData->peerIpAddress[1] = instanceData->peerIpAddress[2] = instanceData->peerIpAddress[3] = 0;
    instanceData->configureIpAddress = TRUE;
    instanceData->currentIdentifier = 0;
    ncpIpConfigurationRequestSend(instanceData);
    instanceData->ncpTimer = CsrSchedTimerSet(NCP_RETRANSMISSION_TIMEOUT, ncpRetransmission, MAX_NUMBER_OF_NCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
    instanceData->ncpState = CSR_BT_NCP_STATE_REQ_SENT;
}

void CsrBtNcpHandleDisconnectInd(CsrBtNcpInstanceData * instanceData)
{
    switch (instanceData->ncpState)
    {
        case CSR_BT_NCP_STATE_OPENED:
            CloseDownIpStack(instanceData);
        case CSR_BT_NCP_STATE_STOPPING:
        case CSR_BT_NCP_STATE_REQ_SENT:
        case CSR_BT_NCP_STATE_ACK_RCVD:
        case CSR_BT_NCP_STATE_ACK_SENT:
            if (instanceData->ncpTimer)
            {
                CsrSchedTimerCancel(instanceData->ncpTimer, NULL, NULL);
                instanceData->ncpTimer = 0;
            }
        case CSR_BT_NCP_STATE_STARTING:
            instanceData->ncpState = CSR_BT_NCP_STATE_INITIAL;
            break;
    }
}
