/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ppp_sef.h"
#include "csr_bt_ppp_hdlc.h"
#include "csr_bt_lsl_prim.h"
#include "csr_bt_lsl_to_prim_converter.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* protocol ids */
#define LCP_PROTOCOL                    0xC021
#define PAP_PROTOCOL                    0xC023


/* -------------------- PPP upstream message factory -------------------- */

static void pppConnectCfmSend(PppInstanceData * instanceData,
                              CsrUint16       maxPayloadLength,
                              CsrBtResultCode resultCode,
                              CsrBtSupplier   resultSupplier)
{
    CsrBtPppConnectCfm * connectCfm;

    connectCfm = (CsrBtPppConnectCfm *) CsrPmemAlloc(sizeof(CsrBtPppConnectCfm));
    connectCfm->type                = CSR_BT_PPP_CONNECT_CFM;
    connectCfm->maxPayloadLength    = maxPayloadLength;
    connectCfm->resultCode          = resultCode;
    connectCfm->resultSupplier      = resultSupplier;
    CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_PPP_PRIM, connectCfm);
}

static void pppDisconnectIndSend(PppInstanceData * instanceData, CsrUint8 reason)
{
    CsrBtPppDisconnectInd * disconnectInd;

    disconnectInd = (CsrBtPppDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtPppDisconnectInd));
    disconnectInd->type = CSR_BT_PPP_DISCONNECT_IND;
    disconnectInd->reason = reason;

    CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_PPP_PRIM, disconnectInd);
}

static void pppDownIndSend(PppInstanceData * instanceData)
{
    CsrBtPppDownInd * downInd;

    downInd = (CsrBtPppDownInd *) CsrPmemAlloc(sizeof(CsrBtPppDownInd));
    downInd->type = CSR_BT_PPP_DOWN_IND;

    CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_PPP_PRIM, downInd);
}

static void pppUpIndSend(PppInstanceData * instanceData)
{
    CsrBtPppUpInd * upInd;

    upInd = (CsrBtPppUpInd *) CsrPmemAlloc(sizeof(CsrBtPppUpInd));
    upInd->type = CSR_BT_PPP_UP_IND;

    CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_PPP_PRIM, upInd);
}

static void pppRegisterNetworkProtocolCfmSend(PppInstanceData * instanceData, CsrSchedQid responseQueue)
{
    CsrBtPppRegisterNetworkProtocolCfm * rnpCfm;

    rnpCfm                  = (CsrBtPppRegisterNetworkProtocolCfm *) CsrPmemAlloc(sizeof(CsrBtPppRegisterNetworkProtocolCfm));
    rnpCfm->type            = CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM;
    rnpCfm->resultCode      = CSR_BT_RESULT_CODE_BSL_SUCCESS;
    rnpCfm->resultSupplier  = CSR_BT_SUPPLIER_BSL;
    CsrSchedMessagePut(responseQueue, CSR_BT_PPP_PRIM, rnpCfm);
}

static void pppAuthenticateIndSend(PppInstanceData * instanceData)
{
    CsrBtPppAuthenticateInd * authenticateInd;

    authenticateInd = (CsrBtPppAuthenticateInd *) CsrPmemAlloc(sizeof(CsrBtPppAuthenticateInd));
    authenticateInd->type = CSR_BT_PPP_AUTHENTICATE_IND;

    CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_PPP_PRIM, authenticateInd);
}


/* PPP utility functions */

static void pppDataReqSendOrQueue(PppInstanceData * instanceData, CsrBtPppDataReq * pppDataReq)
{
    if (instanceData->pppDataReq || instanceData->dataReqQueue)
    {
        CsrMessageQueuePush(&instanceData->dataReqQueue, 0, pppDataReq);
    }
    else
    {
        instanceData->pppDataReq = pppDataReq;
        instanceData->txHdlcState = HDLC_STATE_TRANSMIT_START_DELIMITER;
    }
    if (instanceData->transmissionAllowed)
    {
        PppHdlcTransmit(instanceData);
    }
}

static void pppDataReqSendOrDiscard(PppInstanceData * instanceData, CsrBtPppDataReq * pppDataReq)
{
    if (instanceData->pppDataReq || instanceData->dataReqQueue)
    {
        if (instanceData->numberOnQueue >= CSR_BT_PPP_DATA_REQ_QUEUE_SIZE)
        {
            CsrPmemFree(instanceData->pppDataReqQueue[instanceData->queueTail]->payload);
            CsrPmemFree(instanceData->pppDataReqQueue[instanceData->queueTail++]);
            if (instanceData->queueTail >= CSR_BT_PPP_DATA_REQ_QUEUE_SIZE)
            {
                instanceData->queueTail = 0;
            }
            instanceData->numberOnQueue--;
        }
        instanceData->pppDataReqQueue[instanceData->queueHead++] = pppDataReq;
        if (instanceData->queueHead >= CSR_BT_PPP_DATA_REQ_QUEUE_SIZE)
        {
            instanceData->queueHead = 0;
        }
        instanceData->receivedMessage = NULL;
        instanceData->numberOnQueue++;
    }
    else
    {
        instanceData->pppDataReq = pppDataReq;
        instanceData->txHdlcState = HDLC_STATE_TRANSMIT_START_DELIMITER;
    }
    if (instanceData->transmissionAllowed)
    {
        PppHdlcTransmit(instanceData);
    }
}


/* -------------------- PAP -------------------- */

/* pap codes */
#define PAP_AUTHENTICATE_REQUEST        0x01
#define PAP_AUTHENTICATE_ACK            0x02
#define PAP_AUTHENTICATE_NACK            0x03

#define MAX_NUMBER_OF_PAP_RETRANSMISSIONS        12
#define PAP_RETRANSMISSION_TIMEOUT                5000000

static void papAuthenticateRequestSend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;
    CsrUint8 * papPayload;
    CsrUint8 nameLength;
    CsrUint8 passwordLength;
    CsrUint16 totalLength;
    CsrUint16 index;

    if (instanceData->loginName)
    {
        nameLength = (CsrUint8) CsrStrLen((char *) instanceData->loginName);
    }
    else
    {
        nameLength = 0;
    }
    if (instanceData->password)
    {
        passwordLength = (CsrUint8) CsrStrLen((char *) instanceData->password);
    }
    else
    {
        passwordLength = 0;
    }

    totalLength = 4 + 1 + nameLength + 1 + passwordLength;
    papPayload = (CsrUint8 *) CsrPmemAlloc(totalLength);
    papPayload[0] = PAP_AUTHENTICATE_REQUEST;
    papPayload[1] = ++instanceData->papIdentifier;
    papPayload[2] = ((CsrUint8) ((totalLength >> 8) & 0x00FF));
    papPayload[3] = ((CsrUint8) (totalLength & 0x00FF));
    papPayload[4] = nameLength;
    for (index = 0; index < nameLength; index++)
    {
        papPayload[5 + index] = instanceData->loginName[index];
    }
    papPayload[5 + nameLength] = passwordLength;
    for (index = 0; index < passwordLength; index++)
    {
        papPayload[6 + nameLength + index] = instanceData->password[index];
    }

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = CSR_BT_LAN_WORD(PAP_PROTOCOL);
    pppDataReq->payloadLength = totalLength;
    pppDataReq->payload = papPayload;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}

static void papRetransmission(CsrUint16 retransmissionCount, void * instanceDataPointer)
{
    PppInstanceData * instanceData;

    instanceData = (PppInstanceData *) instanceDataPointer;
    if (retransmissionCount)
    {
        papAuthenticateRequestSend(instanceData);
        instanceData->papTimer = CsrSchedTimerSet(PAP_RETRANSMISSION_TIMEOUT, papRetransmission, ((CsrUint16) (retransmissionCount - 1)), (void *) instanceData);
    }
    else
    {
        instanceData->papTimer = 0;
        /* macro - must be defined for every type of LSL */
        LslDisconnectReqSend();
        instanceData->pppState = PPP_STATE_STOPPED;
    }
}

static void papHandleAuthenticateRes(PppInstanceData * instanceData)
{
    CsrBtPppAuthenticateRes * authenticateRes;

    authenticateRes = (CsrBtPppAuthenticateRes *) instanceData->receivedMessage;
    instanceData->loginName = authenticateRes->loginName;
    instanceData->password = authenticateRes->password;

    papAuthenticateRequestSend(instanceData);

    instanceData->papTimer = CsrSchedTimerSet(PAP_RETRANSMISSION_TIMEOUT, papRetransmission, MAX_NUMBER_OF_PAP_RETRANSMISSIONS - 1, instanceData);
}

static void papHandleCsrBtLslDataInd(PppInstanceData * instanceData)
{
    if (instanceData->pppDataInd->payload[1] == instanceData->papIdentifier)
    {
        switch (instanceData->pppDataInd->payload[0])
        {
            case PAP_AUTHENTICATE_ACK:
                if (instanceData->papTimer)
                {
                    CsrSchedTimerCancel(instanceData->papTimer, NULL, NULL);
                    instanceData->papTimer = 0;
                }
                pppConnectCfmSend(instanceData, instanceData->maxPayloadLength, CSR_BT_RESULT_CODE_BSL_SUCCESS, CSR_BT_SUPPLIER_BSL);
                break;

            case PAP_AUTHENTICATE_NACK:
                if (instanceData->papTimer)
                {
                    CsrSchedTimerCancel(instanceData->papTimer, NULL, NULL);
                    instanceData->papTimer = 0;
                }
                pppConnectCfmSend(instanceData, instanceData->maxPayloadLength, CSR_BT_RESULT_CODE_BSL_TIMEOUT, CSR_BT_SUPPLIER_BSL);
                /* macro - must be defined for every type of LSL */
                LslDisconnectReqSend();
                instanceData->pppState = PPP_STATE_STOPPED;
                break;
        }
    }
}


/* -------------------- LCP -------------------- */

/* lcp codes */
#define LCP_CONFIGURATION_REQUEST        0x01
#define LCP_CONFIGURATION_ACK            0x02
#define LCP_CONFIGURATION_NACK            0x03
#define LCP_CONFIGURATION_REJECT        0x04
#define LCP_TERMINATE_REQUEST            0x05
#define LCP_TERMINATE_ACK                0x06
#define LCP_CODE_REJECT                    0x07
#define LCP_PROTOCOL_REJECT                0x08
#define LCP_ECHO_REQUEST                0x09
#define LCP_ECHO_REPLY                    0x0A
#define LCP_DISCARD_REQUEST                0x0B

/* lcp option types */
#define LCP_OPTION_MRU                    0x01
#define LCP_OPTION_ACCM                    0x02
#define LCP_OPTION_AP                    0x03
#define LCP_OPTION_QP                    0x04
#define LCP_OPTION_MN                    0x05
#define LCP_OPTION_PFC                    0x07
#define LCP_OPTION_ACFC                    0x08

/* lcp option parameters */
#define LCP_CONFIGURE_REQUEST_LENGTH(configureAccm, configureAp, configureMru)    (4 + (configureAccm ? 6 : 0) + (configureAp ? 4 : 0) + (configureMru ? 4 : 0))
#define LCP_OPTION_AP_PAP_HIGH            0xC0
#define LCP_OPTION_AP_PAP_LOW            0x23

static void pppLcpConfigureReqSend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;
    CsrUint8 * configurationRequestPayload;
    CsrUint16 index;
    index = 0;
    configurationRequestPayload = (CsrUint8 *) CsrPmemAlloc(LCP_CONFIGURE_REQUEST_LENGTH(instanceData->configureAccm, instanceData->configureAp, instanceData->configureMru));
    configurationRequestPayload[index++] = LCP_CONFIGURATION_REQUEST;
    configurationRequestPayload[index++] = instanceData->txIdentifier;
    configurationRequestPayload[index++] = 0;    /* high octet of length */
    configurationRequestPayload[index++] = LCP_CONFIGURE_REQUEST_LENGTH(instanceData->configureAccm, instanceData->configureAp, instanceData->configureMru);
    if (instanceData->configureAccm)
    {
        configurationRequestPayload[index++] = LCP_OPTION_ACCM;
        configurationRequestPayload[index++] = 6;
        configurationRequestPayload[index++] = instanceData->rxAccmConfiguration[0];
        configurationRequestPayload[index++] = instanceData->rxAccmConfiguration[1];
        configurationRequestPayload[index++] = instanceData->rxAccmConfiguration[2];
        configurationRequestPayload[index++] = instanceData->rxAccmConfiguration[3];
    }
    if (instanceData->configureAp)
    {
        configurationRequestPayload[index++] = LCP_OPTION_AP;
        configurationRequestPayload[index++] = 4;
        configurationRequestPayload[index++] = LCP_OPTION_AP_PAP_HIGH;
        configurationRequestPayload[index++] = LCP_OPTION_AP_PAP_LOW;
    }
    if (instanceData->configureMru)
    {
        configurationRequestPayload[index++] = LCP_OPTION_MRU;
        configurationRequestPayload[index++] = 4;
        configurationRequestPayload[index++] = ((CsrUint8) ((instanceData->mru >> 8) & 0x00FF));
        configurationRequestPayload[index++] = ((CsrUint8) (instanceData->mru & 0x00FF));
    }

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
    pppDataReq->payloadLength = index;
    pppDataReq->payload = configurationRequestPayload;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}

static void pppLcpTerminateReqSend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;
    CsrUint8 * terminateRequestPayload;

    terminateRequestPayload = (CsrUint8 *) CsrPmemAlloc(4);
    terminateRequestPayload[0] = LCP_TERMINATE_REQUEST;
    terminateRequestPayload[1] = instanceData->txIdentifier;
    terminateRequestPayload[2] = 0;    /* high octet of length */
    terminateRequestPayload[3] = 4;

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
    pppDataReq->payloadLength = 4;
    pppDataReq->payload = terminateRequestPayload;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}

static void pppLcpProtocolRejectSend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;
    CsrUint8 * protocolRejectPayload;
    CsrUint16 maxLength;
    CsrUint16 index;

    maxLength = instanceData->pppDataInd->payloadLength < instanceData->mtu - 6 ? instanceData->pppDataInd->payloadLength : instanceData->mtu - 6;

    protocolRejectPayload = (CsrUint8 *) CsrPmemAlloc(6 + maxLength);
    protocolRejectPayload[0] = LCP_PROTOCOL_REJECT;
    protocolRejectPayload[1] = instanceData->hdlcAndPppHeader[3];
    protocolRejectPayload[2] = ((CsrUint8) (((6 + maxLength) >> 8) & 0x00FF));
    protocolRejectPayload[3] = ((CsrUint8) ((6 + maxLength) & 0x00FF));
    protocolRejectPayload[4] = instanceData->hdlcAndPppHeader[2];
    protocolRejectPayload[5] = instanceData->hdlcAndPppHeader[3];
    for (index = 0; index < maxLength; index++)
    {
        protocolRejectPayload[6 + index] = instanceData->pppDataInd->payload[index];
    }

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
    pppDataReq->payloadLength = 6 + maxLength;
    pppDataReq->payload = protocolRejectPayload;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}

static void pppNcpDataSend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;
    CsrBtPppNcpReq * ncpReq;

    ncpReq = (CsrBtPppNcpReq *) instanceData->receivedMessage;

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = ncpReq->protocol;
    pppDataReq->payloadLength = ncpReq->payloadLength;
    pppDataReq->payload = ncpReq->payload;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}

static void pppLcpTerminateAckSend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
    pppDataReq->payloadLength = instanceData->pppDataInd->payloadLength;
    pppDataReq->payload = instanceData->pppDataInd->payload;
    instanceData->pppDataInd->payload = NULL;
    pppDataReq->payload[0] = LCP_TERMINATE_ACK;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}

static void pppLcpEchoReplySend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
    pppDataReq->payloadLength = instanceData->pppDataInd->payloadLength;
    pppDataReq->payload = instanceData->pppDataInd->payload;
    instanceData->pppDataInd->payload = NULL;
    pppDataReq->payload[0] = LCP_ECHO_REPLY;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}

static void pppLcpCodeRejectSend(PppInstanceData * instanceData)
{
    CsrBtPppDataReq * pppDataReq;
    CsrUint8 * codeRejectPayload;
    CsrUint16 maxLength;
    CsrUint16 index;

    maxLength = instanceData->pppDataInd->payloadLength < instanceData->mtu - 4 ? instanceData->pppDataInd->payloadLength : instanceData->mtu - 4;

    codeRejectPayload = (CsrUint8 *) CsrPmemAlloc(4 + maxLength);
    codeRejectPayload[0] = LCP_CODE_REJECT;
    codeRejectPayload[1] = instanceData->pppDataInd->payload[0]; /* use the rejected code as identifier - should be different every time unless the peer does not listen to the reject! */
    codeRejectPayload[2] = ((CsrUint8) (((4 + maxLength) >> 8) & 0x00FF));
    codeRejectPayload[3] = ((CsrUint8) ((4 + maxLength) & 0x00FF));
    for (index = 0; index < maxLength; index++)
    {
        codeRejectPayload[4 + index] = instanceData->pppDataInd->payload[index];
    }

    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
    pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
    pppDataReq->payloadLength = 4 + maxLength;
    pppDataReq->payload = codeRejectPayload;

    pppDataReqSendOrQueue(instanceData, pppDataReq);
}


#define MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS        10
#define MAX_NUMBER_OF_LCP_TERMINATION_RETRANSMISSIONS        2
#define LCP_RETRANSMISSION_TIMEOUT                            3000000

static void lcpRetransmission(CsrUint16 retransmissionCount, void * instanceDataPointer)
{
    PppInstanceData * instanceData;

    instanceData = (PppInstanceData *) instanceDataPointer;
    if (retransmissionCount)
    {
        switch (instanceData->pppState)
        {
            case PPP_STATE_ACK_RCVD:
                instanceData->pppState = PPP_STATE_REQ_SENT;
            case PPP_STATE_REQ_SENT:
            case PPP_STATE_ACK_SENT:
                pppLcpConfigureReqSend(instanceData);
                break;

            case PPP_STATE_CLOSING:
            case PPP_STATE_STOPPING:
                pppLcpTerminateReqSend(instanceData);
                break;
        }
        instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, --retransmissionCount, instanceDataPointer);
    }
    else
    {
        instanceData->lcpTimer = 0;
        /* macro - must be defined for every type of LSL */
        LslDisconnectReqSend();
        instanceData->pppState = PPP_STATE_STOPPED;
    }
}

static CsrUint16 lcpConfigureRequestRejectSize(PppInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;
    CsrUint16 rejectSize;

    rejectSize = 0;
    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
            case LCP_OPTION_ACCM:
            case LCP_OPTION_AP:
            case LCP_OPTION_MN:
                break;

            case LCP_OPTION_QP:
            case LCP_OPTION_PFC:
            case LCP_OPTION_ACFC:
            default:
                rejectSize += ((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 1]);
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }

    return rejectSize;
}

static void lcpCopyConfigurationRejectOptions(PppInstanceData * instanceData, CsrUint8 * data)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;
    CsrUint16 rejectIndex;

    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
            case LCP_OPTION_ACCM:
            case LCP_OPTION_AP:
            case LCP_OPTION_MN:
                break;

            case LCP_OPTION_QP:
            case LCP_OPTION_PFC:
            case LCP_OPTION_ACFC:
            default:
                for (rejectIndex = 0; rejectIndex < ((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 1]); rejectIndex++)
                {
                    *data++ = instanceData->pppDataInd->payload[currentIndex + rejectIndex];
                }
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }
}

static CsrUint16 lcpConfigureRequestNackSize(PppInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;
    CsrUint16 nackSize;

    nackSize = 0;

    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
            case LCP_OPTION_ACCM:
            case LCP_OPTION_MN:
                break;

            case LCP_OPTION_AP:
                /* currently, we only support PAP! */
                if ((instanceData->pppDataInd->payload[currentIndex + 2] != LCP_OPTION_AP_PAP_HIGH) || (instanceData->pppDataInd->payload[currentIndex + 3] != LCP_OPTION_AP_PAP_LOW))
                {
                    nackSize += 4;
                }
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }

    return nackSize;
}

static void lcpCopyConfigurationNackOptions(PppInstanceData * instanceData, CsrUint8 * data)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;

    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
            case LCP_OPTION_ACCM:
            case LCP_OPTION_MN:
                break;

            case LCP_OPTION_AP:
                *data++ = LCP_OPTION_AP;
                *data++ = 4;
                *data++ = LCP_OPTION_AP_PAP_HIGH;
                *data++ = LCP_OPTION_AP_PAP_LOW;
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }
}

static void lcpRetrieveConfigurationRequestOptions(PppInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;

    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
                instanceData->mtu = (((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 2]) << 8) + ((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 3]);
                break;

            case LCP_OPTION_ACCM:
                {
                    CsrUint32 accm;
                    CsrUint16 index;

                    accm = (((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 2]) << 24) + (((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 3]) << 16) + (((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 4]) << 8) + ((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 5]);
                    for (index = 0; index < 32; index++)
                    {
                        if (!(accm & 0x1L))
                        {
                            instanceData->txAsynchronousControlCharacterMap[index] = FALSE;
                        }
                        accm >>= 1;
                    }
                    break;
                }

            case LCP_OPTION_AP:
                instanceData->usePap = TRUE;
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }
}

static void lcpHandleConfigurationRequest(PppInstanceData * instanceData)
{
    CsrUint16 size;

    /* do we need to reject some? */
    if ((size = lcpConfigureRequestRejectSize(instanceData)))
    {
        switch (instanceData->pppState)
        {
            case PPP_STATE_OPENED:
                pppDownIndSend(instanceData);
                pppLcpConfigureReqSend(instanceData);
                CsrSchedTimerCancel(instanceData->lcpTimer,NULL,NULL);
                instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            case PPP_STATE_ACK_SENT:
                instanceData->pppState = PPP_STATE_REQ_SENT;
                break;
        }

        switch (instanceData->pppState)
        {
            case PPP_STATE_REQ_SENT:
            case PPP_STATE_ACK_RCVD:
                {
                    CsrBtPppDataReq * pppDataReq;
                    CsrUint8 * configurationRejectPayload;
                    CsrUint16 index;

                    index = 0;
                    size += 4;
                    configurationRejectPayload = (CsrUint8 *) CsrPmemAlloc(size);
                    configurationRejectPayload[index++] = LCP_CONFIGURATION_REJECT;
                    configurationRejectPayload[index++] = instanceData->pppDataInd->payload[1];
                    configurationRejectPayload[index++] = ((CsrUint8) ((size >> 8) & 0x00FF));
                    configurationRejectPayload[index++] = ((CsrUint8) (size & 0x00FF));

                    lcpCopyConfigurationRejectOptions(instanceData, &configurationRejectPayload[index]);

                    pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
                    pppDataReq->type = CSR_BT_PPP_DATA_REQ;
                    pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
                    pppDataReq->payloadLength = size;
                    pppDataReq->payload = configurationRejectPayload;

                    pppDataReqSendOrQueue(instanceData, pppDataReq);
                    break;
                }
        }
    }
    else
    {
        /* do we need to nak some */
        if ((size = lcpConfigureRequestNackSize(instanceData)))
        {
            switch (instanceData->pppState)
            {
                case PPP_STATE_OPENED:
                    pppDownIndSend(instanceData);
                    pppLcpConfigureReqSend(instanceData);
                    CsrSchedTimerCancel(instanceData->lcpTimer,NULL,NULL);
                    instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
                case PPP_STATE_ACK_SENT:
                    instanceData->pppState = PPP_STATE_REQ_SENT;
                    break;
            }

            switch (instanceData->pppState)
            {
                case PPP_STATE_REQ_SENT:
                case PPP_STATE_ACK_RCVD:
                    {
                        CsrBtPppDataReq * pppDataReq;
                        CsrUint8 * configurationNackPayload;
                        CsrUint16 index;

                        index = 0;
                        size += 4;
                        configurationNackPayload = (CsrUint8 *) CsrPmemAlloc(size);
                        configurationNackPayload[index++] = LCP_CONFIGURATION_NACK;
                        configurationNackPayload[index++] = instanceData->pppDataInd->payload[1];
                        configurationNackPayload[index++] = ((CsrUint8) ((size >> 8) & 0x00FF));
                        configurationNackPayload[index++] = ((CsrUint8) (size & 0x00FF));

                        lcpCopyConfigurationNackOptions(instanceData, &configurationNackPayload[index]);

                        pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
                        pppDataReq->type = CSR_BT_PPP_DATA_REQ;
                        pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
                        pppDataReq->payloadLength = size;
                        pppDataReq->payload = configurationNackPayload;

                        pppDataReqSendOrQueue(instanceData, pppDataReq);
                        break;
                    }
            }
        }
        else /* otherwise ack */
        {
            switch (instanceData->pppState)
            {
                case PPP_STATE_OPENED:
                    pppDownIndSend(instanceData);
                    pppLcpConfigureReqSend(instanceData);
                    CsrSchedTimerCancel(instanceData->lcpTimer,NULL,NULL);
                    instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
                case PPP_STATE_REQ_SENT:
                    instanceData->pppState = PPP_STATE_ACK_SENT;
                    break;

                case PPP_STATE_ACK_RCVD:
                    if (instanceData->lcpTimer)
                    {
                        CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                        instanceData->lcpTimer = 0;
                    }
                    pppUpIndSend(instanceData);
                    instanceData->pppState = PPP_STATE_OPENED;

                    if (instanceData->usePap)
                    {
                        pppAuthenticateIndSend(instanceData);
                    }
                    else
                    {
                        pppConnectCfmSend(instanceData, instanceData->maxPayloadLength, CSR_BT_RESULT_CODE_BSL_SUCCESS, CSR_BT_SUPPLIER_BSL);
                    }
                    break;
            }

            switch (instanceData->pppState)
            {
                case PPP_STATE_ACK_SENT:
                case PPP_STATE_OPENED:
                    {
                        CsrBtPppDataReq * pppDataReq;

                        lcpRetrieveConfigurationRequestOptions(instanceData);

                        pppDataReq = (CsrBtPppDataReq *) CsrPmemAlloc(sizeof(CsrBtPppDataReq));
                        pppDataReq->type = CSR_BT_PPP_DATA_REQ;
                        pppDataReq->protocol = CSR_BT_LAN_WORD(LCP_PROTOCOL);
                        pppDataReq->payloadLength = instanceData->pppDataInd->payloadLength;
                        pppDataReq->payload = instanceData->pppDataInd->payload;
                        instanceData->pppDataInd->payload = NULL;
                        pppDataReq->payload[0] = LCP_CONFIGURATION_ACK;
                        pppDataReqSendOrQueue(instanceData, pppDataReq);
                        break;
                    }
            }
        }
    }
}

static void pppLcpProcessNack(PppInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;

    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
                instanceData->mru = (((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 2]) << 8) + ((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 3]);
                instanceData->configureMru = TRUE;
                if (instanceData->mru > 1500)
                {
                    instanceData->mru = 1500;
                }
                if (instanceData->mru < 576)    /* Minimum IP size */
                {
                    instanceData->mru = 576;
                }
                break;

            case LCP_OPTION_ACCM:
                {
                    CsrUint16 index;

                    for (index = 0; index < 4; index++)
                    {
                        instanceData->rxAccmConfiguration[index] = instanceData->pppDataInd->payload[currentIndex + 2 + index];
                    }
                    break;
                }

            case LCP_OPTION_AP:
                instanceData->configureAp = TRUE;
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }
}

static void pppLcpProcessReject(PppInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;

    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
                instanceData->configureMru = FALSE;
                break;

            case LCP_OPTION_ACCM:
                instanceData->configureAccm = FALSE;
                break;

            case LCP_OPTION_AP:
                instanceData->configureAp = FALSE;
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }
}

static void pppLcpProcessAcknowledgement(PppInstanceData * instanceData)
{
    CsrUint16 optionsLength;
    CsrUint16 currentIndex;

    optionsLength = (((CsrUint16) instanceData->pppDataInd->payload[2]) << 8) + instanceData->pppDataInd->payload[3];
    currentIndex = 4;
    while (currentIndex < optionsLength)
    {
        switch (instanceData->pppDataInd->payload[currentIndex])
        {
            case LCP_OPTION_MRU:
                instanceData->mru = (((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 2]) << 8) + ((CsrUint16) instanceData->pppDataInd->payload[currentIndex + 3]);
                break;

            case LCP_OPTION_ACCM:
                {
                    CsrUint32 accm;
                    CsrUint16 index;

                    accm = (((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 2]) << 24) + (((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 3]) << 16) + (((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 4]) << 8) + ((CsrUint32) instanceData->pppDataInd->payload[currentIndex + 5]);
                    for (index = 0; index < 32; index++)
                    {
                        if (!(accm & 0x1L))
                        {
                            instanceData->rxAsynchronousControlCharacterMap[index] = FALSE;
                        }
                        accm >>= 1;
                    }
                    break;
                }

            case LCP_OPTION_AP:
                instanceData->usePap = TRUE;
                break;
        }
        currentIndex += instanceData->pppDataInd->payload[currentIndex + 1];
    }
}

static void lcpHandleConfigurationAcknowledgement(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_REQ_SENT:
            instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            instanceData->pppState = PPP_STATE_ACK_RCVD;
            break;

        case PPP_STATE_OPENED:
            pppDownIndSend(instanceData);
        case PPP_STATE_ACK_RCVD:
            pppLcpConfigureReqSend(instanceData);
            instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            instanceData->pppState = PPP_STATE_REQ_SENT;
            break;

        case PPP_STATE_ACK_SENT:
            pppUpIndSend(instanceData);
            instanceData->pppState = PPP_STATE_OPENED;
            if (instanceData->usePap)
            {
                pppAuthenticateIndSend(instanceData);
            }
            else
            {
                pppConnectCfmSend(instanceData, instanceData->maxPayloadLength, CSR_BT_RESULT_CODE_BSL_SUCCESS, CSR_BT_SUPPLIER_BSL);
            }
            break;
    }
    pppLcpProcessAcknowledgement(instanceData);
}

static void lcpHandleConfigurationNack(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_OPENED:
            pppDownIndSend(instanceData);
        case PPP_STATE_REQ_SENT:
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_ACK_SENT:
            pppLcpProcessNack(instanceData);
            pppLcpConfigureReqSend(instanceData);
            instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            break;
    }
    switch (instanceData->pppState)
    {
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_OPENED:
            instanceData->pppState = PPP_STATE_REQ_SENT;
            break;
    }
}

static void lcpHandleConfigurationReject(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_OPENED:
            pppDownIndSend(instanceData);
        case PPP_STATE_REQ_SENT:
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_ACK_SENT:
            pppLcpProcessReject(instanceData);
            pppLcpConfigureReqSend(instanceData);
            instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
            break;
    }
    switch (instanceData->pppState)
    {
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_OPENED:
            instanceData->pppState = PPP_STATE_REQ_SENT;
            break;
    }
}

static void lcpHandleTerminateRequest(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_REQ_SENT:
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_ACK_SENT:
        case PPP_STATE_OPENED:
        case PPP_STATE_STOPPING:
        case PPP_STATE_CLOSING:
            pppLcpTerminateAckSend(instanceData);
            break;
    }
    switch (instanceData->pppState)
    {
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_ACK_SENT:
            instanceData->pppState = PPP_STATE_REQ_SENT;
            break;

        case PPP_STATE_OPENED:
            pppDownIndSend(instanceData);
            CsrSchedTimerCancel(instanceData->lcpTimer,NULL,NULL);
            instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, 0, (void *) instanceData);
            instanceData->pppState = PPP_STATE_STOPPING;
            break;
    }
}

static void lcpHandleTerminateAcknowledgement(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_CLOSING:
        case PPP_STATE_STOPPING:
            CsrSchedTimerCancel(instanceData->lcpTimer,NULL,NULL);
            instanceData->lcpTimer = 0;
            /* macro - must be defined for every type of LSL */
            LslDisconnectReqSend();
            instanceData->pppState = PPP_STATE_STOPPED;
            break;

        case PPP_STATE_OPENED:
            pppDownIndSend(instanceData);
            pppLcpConfigureReqSend(instanceData);
            CsrSchedTimerCancel(instanceData->lcpTimer,NULL,NULL);
            instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
        case PPP_STATE_ACK_RCVD:
            instanceData->pppState = PPP_STATE_REQ_SENT;
            break;
    }
}

static void lcpHandleCodeAndProtocolReject(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_CLOSING:
        case PPP_STATE_STOPPING:
        case PPP_STATE_REQ_SENT:
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_ACK_SENT:
            /* macro - must be defined for every type of LSL */
            LslDisconnectReqSend();
            instanceData->pppState = PPP_STATE_STOPPED;
            break;

        case PPP_STATE_OPENED:
            pppDownIndSend(instanceData);
            pppLcpTerminateReqSend(instanceData);
            instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_TERMINATION_RETRANSMISSIONS - 1, (void *) instanceData);
            instanceData->pppState = PPP_STATE_STOPPING;
            break;
    }
}

static void lcpHandleEchoRequest(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_OPENED:
            pppLcpEchoReplySend(instanceData);
            break;
    }
}


static void lcpHandleUnknownCode(PppInstanceData * instanceData)
{
    switch (instanceData->pppState)
    {
        case PPP_STATE_CLOSING:
        case PPP_STATE_STOPPING:
        case PPP_STATE_REQ_SENT:
        case PPP_STATE_ACK_RCVD:
        case PPP_STATE_ACK_SENT:
        case PPP_STATE_OPENED:
            pppLcpCodeRejectSend(instanceData);
            break;
    }
}

static void lcpHandleCsrBtLslDataInd(PppInstanceData * instanceData)
{
    switch (instanceData->pppDataInd->payload[0])
    {
        case LCP_CONFIGURATION_REQUEST:
            lcpHandleConfigurationRequest(instanceData);
            break;

        case LCP_CONFIGURATION_ACK:
            if (instanceData->lcpTimer)
            {
                CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                instanceData->lcpTimer = 0;
            }
            if (instanceData->pppDataInd->payload[1] == instanceData->txIdentifier)
            {
                instanceData->txIdentifier++;
                lcpHandleConfigurationAcknowledgement(instanceData);
            }
            break;

        case LCP_CONFIGURATION_NACK:
            if (instanceData->lcpTimer)
            {
                CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                instanceData->lcpTimer = 0;
            }
            if (instanceData->pppDataInd->payload[1] == instanceData->txIdentifier)
            {
                instanceData->txIdentifier++;
                lcpHandleConfigurationNack(instanceData);
            }
            break;

        case LCP_CONFIGURATION_REJECT:
            if (instanceData->lcpTimer)
            {
                CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                instanceData->lcpTimer = 0;
            }
            if (instanceData->pppDataInd->payload[1] == instanceData->txIdentifier)
            {
                instanceData->txIdentifier++;
                lcpHandleConfigurationReject(instanceData);
            }
            break;

        case LCP_TERMINATE_REQUEST:
            lcpHandleTerminateRequest(instanceData);
            break;

        case LCP_TERMINATE_ACK:
            if (instanceData->lcpTimer)
            {
                CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                instanceData->lcpTimer = 0;
            }
            if (instanceData->pppDataInd->payload[1] == instanceData->txIdentifier)
            {
                instanceData->txIdentifier++;
                lcpHandleTerminateAcknowledgement(instanceData);
            }
            break;

        case LCP_CODE_REJECT:
            if (instanceData->lcpTimer)
            {
                CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                instanceData->lcpTimer = 0;
            }
            if (instanceData->pppDataInd->payload[1] == instanceData->txIdentifier)
            {
                instanceData->txIdentifier++;
                lcpHandleCodeAndProtocolReject(instanceData);
            }
            break;

        case LCP_PROTOCOL_REJECT:
            if (instanceData->lcpTimer)
            {
                CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                instanceData->lcpTimer = 0;
            }
            if (instanceData->pppDataInd->payload[1] == instanceData->txIdentifier)
            {
                instanceData->txIdentifier++;
                lcpHandleCodeAndProtocolReject(instanceData);
            }
            break;

        case LCP_ECHO_REQUEST:
            lcpHandleEchoRequest(instanceData);
            break;

        case LCP_ECHO_REPLY:
            /* just ignore - the request is not implemented?!?! */
            break;

        case LCP_DISCARD_REQUEST:
            /* ignore! */
            break;

        default:
            lcpHandleUnknownCode(instanceData);
            break;
    }
}

static void handleCsrBtLslDataInd(PppInstanceData * instanceData)
{
    CsrBtLslDataInd * dataInd;

    dataInd = (CsrBtLslDataInd *) instanceData->receivedMessage;

    instanceData->currentPayloadIndex = 0;

    while (PppHdlcReceive(instanceData))
    {
        instanceData->pppDataInd->payloadLength -= 2;
        if (CheckHdlcFrameFcs16(instanceData->hdlcAndPppHeader, instanceData->pppDataInd->payloadLength, instanceData->pppDataInd->payload))
        {
            CsrUint16 protocol;

            protocol = (((CsrUint16) instanceData->hdlcAndPppHeader[2]) << 8) + ((CsrUint16) instanceData->hdlcAndPppHeader[3]);
            instanceData->pppDataInd->protocol = CSR_BT_LAN_WORD(protocol);
            if (protocol > 0xC000) /* link layer control protocols */
            {
                switch (protocol)
                {
                    case LCP_PROTOCOL:
                        lcpHandleCsrBtLslDataInd(instanceData);
                        break;

                    case PAP_PROTOCOL:
                        papHandleCsrBtLslDataInd(instanceData);
                        break;
                }
            }
            else if (protocol > 0x8000) /* network layer control protocols */
            {
                if (instanceData->pppState == PPP_STATE_OPENED)
                {
                    PppRegisteredNetworkProtocol * searchRnpPtr;

                    for (searchRnpPtr = instanceData->registeredNetworkProtocolList; searchRnpPtr; searchRnpPtr = searchRnpPtr->next)
                    {
                        if (searchRnpPtr->ncpProtocol == protocol)
                        {
                            break;
                        }
                    }
                    if (searchRnpPtr)
                    {
                        instanceData->pppDataInd->type = CSR_BT_PPP_NCP_IND;
                        CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_PPP_PRIM, instanceData->pppDataInd);
                        instanceData->pppDataInd = NULL;
                    }
                    else
                    {
                        pppLcpProtocolRejectSend(instanceData);
                    }
                }
                /*
                  else
                  {
                  discard silently;
                  }
 */
            }
            else /* network layer protocols */
            {
                if (instanceData->pppState == PPP_STATE_OPENED)
                {
                    PppRegisteredNetworkProtocol * searchRnpPtr;

                    for (searchRnpPtr = instanceData->registeredNetworkProtocolList; searchRnpPtr; searchRnpPtr = searchRnpPtr->next)
                    {
                        if (searchRnpPtr->dataProtocol == protocol)
                        {
                            break;
                        }
                    }
                    if (searchRnpPtr)
                    {
                        instanceData->pppDataInd->type = CSR_BT_PPP_DATA_IND;
                        CsrSchedMessagePut(searchRnpPtr->dataQueue, CSR_BT_PPP_PRIM, instanceData->pppDataInd);
                        instanceData->pppDataInd = NULL;
                    }
                    else
                    {
                        pppLcpProtocolRejectSend(instanceData);
                    }
                }
                /*
                  else
                  {
                  discard silently;
                  }
 */
            }

            if (instanceData->pppDataInd)
            {
                CsrPmemFree(instanceData->pppDataInd->payload);
                CsrPmemFree(instanceData->pppDataInd);
                instanceData->pppDataInd = NULL;
            }
        }
        if (instanceData->currentPayloadIndex >= dataInd->length)
        {
            break;
        }
    }
    CsrPmemFree(dataInd->payload);
    /* macro - must be defined for every type of LSL */
    LslDataResSend();
}

void HandleLslMessage(PppInstanceData * instanceData)
{
    CsrBtLslPrim type;

    type = *((CsrBtLslPrim *) instanceData->receivedMessage);
    switch (type)
    {
        case CSR_BT_LSL_DATA_IND:
            handleCsrBtLslDataInd(instanceData);
            break;

        case CSR_BT_LSL_DATA_CFM:
            instanceData->transmissionAllowed = TRUE;
            PppHdlcTransmit(instanceData);
            CsrPmemFree(instanceData->receivedMessage);
            instanceData->receivedMessage = NULL;
            break;

        case CSR_BT_LSL_STATUS_IND:
            {
                CsrBtLslStatusInd * status;
                status = (CsrBtLslStatusInd *) instanceData->receivedMessage;

                if(status->connected == TRUE)
                {
                    if (instanceData->pppState == PPP_STATE_STARTING)
                    {
                        int index;
                        CsrUint8 * clientString;

                        instanceData->maxPayloadLength = status->maxPayloadLength;
                        clientString = (CsrUint8 *) CsrPmemAlloc(7);
                        CsrStrCpy((char *) clientString, "CLIENT");
                        /* macro - must be defined for every type of LSL */
                        LslDataReqSend(6, clientString);

                        instanceData->maxPayloadLength = status->maxPayloadLength;
                        instanceData->mru = 1500;
                        instanceData->mtu = 1500;
                        instanceData->rxHdlcState = HDLC_STATE_SEARCHING_FOR_FRAME_DELIMITER;
                        instanceData->rxHdlcEscapeFound = FALSE;
                        instanceData->hdlcAndPppHeader[0] = 0xFF;
                        instanceData->hdlcAndPppHeader[1] = 0x03;
                        instanceData->txHdlcState = HDLC_STATE_TRANSMIT_START_DELIMITER;
                        instanceData->txHdlcEscapeSent = FALSE;
                        instanceData->queueHead = instanceData->queueTail = 0;
                        instanceData->numberOnQueue = 0;

                        instanceData->txIdentifier = 0;
                        instanceData->configureAccm = TRUE;
                        instanceData->configureAp = TRUE;
                        instanceData->configureMru = TRUE;
                        instanceData->usePap = TRUE;
                        for (index = 0; index < 0x20; index++)
                        {
                            instanceData->rxAsynchronousControlCharacterMap[index] = TRUE;
                            instanceData->txAsynchronousControlCharacterMap[index] = TRUE;
                        }
                        instanceData->rxAccmConfiguration[0] = 0;
                        instanceData->rxAccmConfiguration[1] = 0;
                        instanceData->rxAccmConfiguration[2] = 0;
                        instanceData->rxAccmConfiguration[3] = 0;

                        pppLcpConfigureReqSend(instanceData);
                        instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_CONFIGURATION_RETRANSMISSIONS - 1, (void *) instanceData);
                        instanceData->pppState = PPP_STATE_REQ_SENT;
                    }
                    else if (instanceData->pppState == PPP_STATE_STOPPED)
                    {
                        /* connect confirm in wrong state???, should
                         * not be possible, so ignore! */
                    }
                }
                else if (status->connected == FALSE)
                {
                    if (instanceData->pppState != PPP_STATE_INITIAL)
                    {
                        CsrUint16 event;
                        CsrBtPppDataReq * pppDataReq;

                        pppDisconnectIndSend(instanceData, status->reason);
                        instanceData->pppState = PPP_STATE_INITIAL;

                        instanceData->applicationQueue = CSR_BT_PPP_IFACEQUEUE;
                        if (instanceData->lcpTimer)
                        {
                            CsrSchedTimerCancel(instanceData->lcpTimer, NULL, NULL);
                            instanceData->lcpTimer = 0;
                        }
                        while (CsrMessageQueuePop(&instanceData->dataReqQueue, &event, (void **) &pppDataReq))
                        {
                            CsrPmemFree(pppDataReq->payload);
                            CsrPmemFree(pppDataReq);
                        }
                        while (instanceData->numberOnQueue)
                        {
                            instanceData->numberOnQueue--;
                            CsrPmemFree(instanceData->pppDataReqQueue[instanceData->queueTail]->payload);
                            CsrPmemFree(instanceData->pppDataReqQueue[instanceData->queueTail++]);
                            if (instanceData->queueTail >= CSR_BT_PPP_DATA_REQ_QUEUE_SIZE)
                            {
                                instanceData->queueTail = 0;
                            }
                        }
                        if (instanceData->pppDataInd)
                        {
                            CsrPmemFree(instanceData->pppDataInd->payload);
                            CsrPmemFree(instanceData->pppDataInd);
                            instanceData->pppDataInd = NULL;
                        }
                        if (instanceData->pppDataReq)
                        {
                            CsrPmemFree(instanceData->pppDataReq->payload);
                            CsrPmemFree(instanceData->pppDataReq);
                            instanceData->pppDataReq = NULL;
                        }
                        if (instanceData->loginName)
                        {
                            CsrPmemFree(instanceData->loginName);
                            instanceData->loginName = NULL;
                        }
                        if (instanceData->password)
                        {
                            CsrPmemFree(instanceData->password);
                            instanceData->password = NULL;
                        }
                        if (instanceData->papTimer)
                        {
                            CsrSchedTimerCancel(instanceData->papTimer, NULL, NULL);
                            instanceData->papTimer = 0;
                        }
                    }
                }
                else
                {
                    /*should never happen */
                }
                break;
            }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            CsrStateEventException("PPP",
                                    instanceData->event,
                                    type,
                                    instanceData->pppState);
            break;
#endif
    }
}

void HandlePppMessage(PppInstanceData * instanceData)
{
    CsrBtPppPrim type;

    type = *((CsrBtPppPrim *) instanceData->receivedMessage);
    switch (type)
    {
        case CSR_BT_PPP_CONNECT_REQ:
            {
                CsrBtPppConnectReq * connectReq;

                connectReq = (CsrBtPppConnectReq *) instanceData->receivedMessage;
                if (instanceData->pppState == PPP_STATE_INITIAL)
                {
                    instanceData->applicationQueue = connectReq->applicationQueue;
                    /* macro - must be defined for every type of LSL */
                    LslConnectReqSend(CSR_BT_PPP_IFACEQUEUE, &connectReq->deviceAddr);
                    instanceData->pppState = PPP_STATE_STARTING;
                }
                break;
            }

        case CSR_BT_PPP_NCP_REQ:
            {
                pppNcpDataSend(instanceData);
                break;
            }

        case CSR_BT_PPP_DATA_REQ:
            {
                if (instanceData->pppState == PPP_STATE_OPENED)
                {
                    pppDataReqSendOrDiscard(instanceData, (CsrBtPppDataReq *) instanceData->receivedMessage);
                    instanceData->receivedMessage = NULL;
                }
                else
                {
                    CsrPmemFree(((CsrBtPppDataReq *) instanceData->receivedMessage)->payload);
                }
                break;
            }

        case CSR_BT_PPP_DISCONNECT_REQ:
            {
                CsrBtPppDisconnectReq * disconnectReq;

                disconnectReq = (CsrBtPppDisconnectReq *) instanceData->receivedMessage;
                switch (instanceData->pppState)
                {
                    case PPP_STATE_INITIAL:
                        pppDisconnectIndSend(instanceData, HCI_ERROR_OETC_USER);
                        break;

                    case PPP_STATE_STARTING:
                        instanceData->pppState = PPP_STATE_STOPPED;
                        break;

                    case PPP_STATE_OPENED:
                        pppDownIndSend(instanceData);
                    case PPP_STATE_REQ_SENT:
                    case PPP_STATE_ACK_RCVD:
                    case PPP_STATE_ACK_SENT:
                        pppLcpTerminateReqSend(instanceData);
                        CsrSchedTimerCancel(instanceData->lcpTimer,NULL,NULL);
                        instanceData->lcpTimer = CsrSchedTimerSet(LCP_RETRANSMISSION_TIMEOUT, lcpRetransmission, MAX_NUMBER_OF_LCP_TERMINATION_RETRANSMISSIONS - 1, (void *) instanceData);
                    case PPP_STATE_STOPPING:
                        instanceData->pppState = PPP_STATE_CLOSING;
                    case PPP_STATE_CLOSING:
                        break;
                }
                break;
            }

        case CSR_BT_PPP_AUTHENTICATE_RES:
            {
                papHandleAuthenticateRes(instanceData);
                break;
            }

        case CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_REQ:
            {
                PppRegisteredNetworkProtocol * newEntry;
                CsrBtPppRegisterNetworkProtocolReq * rnpReq;

                rnpReq = (CsrBtPppRegisterNetworkProtocolReq *) instanceData->receivedMessage;
                newEntry = (PppRegisteredNetworkProtocol *) CsrPmemAlloc(sizeof(PppRegisteredNetworkProtocol));
                newEntry->ncpProtocol = rnpReq->ncpProtocol;
                newEntry->dataQueue= rnpReq->dataQueue;
                newEntry->dataProtocol = rnpReq->dataProtocol;
                newEntry->next = instanceData->registeredNetworkProtocolList;
                instanceData->registeredNetworkProtocolList = newEntry;

                pppRegisterNetworkProtocolCfmSend(instanceData, rnpReq->responseQueue);
                break;
            }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            CsrStateEventException("PPP",
                                    instanceData->event,
                                    type,
                                    instanceData->pppState);
            break;
#endif
    }
}

