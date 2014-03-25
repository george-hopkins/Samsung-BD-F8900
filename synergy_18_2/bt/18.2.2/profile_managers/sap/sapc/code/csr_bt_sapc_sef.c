/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_sapc_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sap_common_private.h"

static void csrBtSapcMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SAPC_PRIM, msg);
}

/*************************************************************************************
  CsrBtSapcSavemessage
************************************************************************************/
void CsrBtSapcSavemessage(SapcInstanceData_t * instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_SAPC_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

/*************************************************************************************
  CsrBtSapcRestoreSavedMessages
************************************************************************************/
void CsrBtSapcRestoreSavedMessages(SapcInstanceData_t * instData)
{
    CsrBtSapcPrim * sapcPrim;
    if (instData->saveQueue != NULL)
    {
        sapcPrim = CsrPmemAlloc(sizeof(CsrBtSapcPrim));
        *sapcPrim = CSR_BT_SAPC_HOUSE_CLEANING;
        csrBtSapcMessagePut(CSR_BT_SAPC_IFACEQUEUE, sapcPrim);
        instData->restoreSapcFlag = TRUE;
    }
}

/******************************************************************************
 *
 * Local functions for sending upstream (to Application)
 *
 ******************************************************************************/
static void csrBtSapcSendConnectCfm(CsrSchedQid phandle,
                                    CsrUint32 btConnId,
                                    CsrUint16 maxMsgSize,
                                    CsrBtResultCode resultCode,
                                    CsrBtSupplier resultSupplier)
{
    CsrBtSapcConnectCfm * prim;
    prim = (CsrBtSapcConnectCfm *) CsrPmemAlloc(sizeof (CsrBtSapcConnectCfm));

    prim->type = CSR_BT_SAPC_CONNECT_CFM;
    prim->maxMsgSize     = maxMsgSize;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->btConnId       = btConnId;

    csrBtSapcMessagePut(phandle, prim);
}

static void csrBtSapcSendStatusInd(CsrSchedQid phandle, CsrUint8 theStatus)
{
    CsrBtSapcStatusInd * prim;
    prim = (CsrBtSapcStatusInd *) CsrPmemAlloc(sizeof(CsrBtSapcStatusInd));

    prim->type = CSR_BT_SAPC_STATUS_IND;
    prim->statusChange = theStatus;

    csrBtSapcMessagePut(phandle, prim);
}

static void csrBtSapcSendDisconnectInd(CsrSchedQid phandle,
                                              CsrBtReasonCode reasonCode,
                                              CsrBtSupplier reasonSupplier)
{
    CsrBtSapcDisconnectInd * prim;
    prim = (CsrBtSapcDisconnectInd *)CsrPmemAlloc(sizeof (CsrBtSapcDisconnectInd));

    prim->type = CSR_BT_SAPC_DISCONNECT_IND;
    prim->reasonCode     = reasonCode;
    prim->reasonSupplier = reasonSupplier;

    csrBtSapcMessagePut(phandle, prim);
}

static void csrBtSapcSendTransferApduCfm(CsrSchedQid appHandle,
                                                  CsrBtResultCode resultCode,
                                                  CsrBtSupplier resultSupplier,
                                                  CsrUint16 theResponseLength,
                                                  CsrUint8 * theResponse)
{
    CsrBtSapcTransferApduCfm * prim;
    prim = (CsrBtSapcTransferApduCfm *) CsrPmemAlloc(sizeof (CsrBtSapcTransferApduCfm));

    prim->type = CSR_BT_SAPC_TRANSFER_APDU_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->responseApduLength = theResponseLength;
    prim->responseApdu = theResponse;

    csrBtSapcMessagePut(appHandle, prim);
}

static void csrBtSapcSendTransferAtrCfm(CsrSchedQid appHandle,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier resultSupplier,
                                               CsrUint16 theAtrLength,
                                               CsrUint8 * theAtr)
{
    CsrBtSapcTransferAtrCfm * prim;
    prim = (CsrBtSapcTransferAtrCfm *) CsrPmemAlloc(sizeof(CsrBtSapcTransferAtrCfm));

    prim->type = CSR_BT_SAPC_TRANSFER_ATR_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->atrLength = theAtrLength;
    prim->atr = theAtr;

    csrBtSapcMessagePut(appHandle, prim);
}

static void csrBtSapcSendPowerSimOffCfm(CsrSchedQid appHandle,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier resultSupplier)
{
    CsrBtSapcPowerSimOffCfm * prim;
    prim = (CsrBtSapcPowerSimOffCfm *) CsrPmemAlloc(sizeof(CsrBtSapcPowerSimOffCfm));

    prim->type = CSR_BT_SAPC_POWER_SIM_OFF_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    csrBtSapcMessagePut(appHandle, prim);
}

static void csrBtSapcSendPowerSimOnCfm(CsrSchedQid appHandle,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier resultSupplier)
{
    CsrBtSapcPowerSimOnCfm * prim;
    prim = (CsrBtSapcPowerSimOnCfm *) CsrPmemAlloc(sizeof(CsrBtSapcPowerSimOnCfm));

    prim->type = CSR_BT_SAPC_POWER_SIM_ON_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    csrBtSapcMessagePut(appHandle, prim);
}

static void csrBtSapcSendResetSimCfm(CsrSchedQid appHandle,
                                             CsrBtResultCode resultCode,
                                             CsrBtSupplier resultSupplier)
{
    CsrBtSapcResetSimCfm * prim;
    prim = (CsrBtSapcResetSimCfm *) CsrPmemAlloc(sizeof(CsrBtSapcResetSimCfm));

    prim->type = CSR_BT_SAPC_RESET_SIM_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    csrBtSapcMessagePut(appHandle, prim);
}

static void csrBtSapcSendCardReaderStatusCfm(CsrSchedQid appHandle,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier resultSupplier,
                                               CsrUint8 theCardReaderStatus)
{
    CsrBtSapcTransferCardReaderStatusCfm * prim;
    prim = (CsrBtSapcTransferCardReaderStatusCfm *) CsrPmemAlloc(sizeof(CsrBtSapcTransferCardReaderStatusCfm));

    prim->type = CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->cardReaderStatus = theCardReaderStatus;

    csrBtSapcMessagePut(appHandle, prim);
}

static void csrBtSapcSendSetTransferProtocolCfm(CsrSchedQid appHandle,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier resultSupplier)
{
    CsrBtSapcSetTransferProtocolCfm * prim;
    prim = (CsrBtSapcSetTransferProtocolCfm * )CsrPmemAlloc(sizeof(CsrBtSapcSetTransferProtocolCfm));

    prim->type = CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    csrBtSapcMessagePut(appHandle, prim);
}

static void resetSendRecvBuffer(SapcInstanceData_t * instData)
{
    if (instData->sendBuffer != NULL)
    {
        /* free sendBuffer */
        CsrPmemFree (instData->sendBuffer);
        instData->sendBuffer = NULL;
    }
    instData->transmitReady = TRUE;
    if (instData->recvBuffer != NULL)
    {
        /* free recvBuffer */
        CsrPmemFree (instData->recvBuffer);
        instData->recvBuffer = NULL;
    }
}

static void SendDataReq(SapcInstanceData_t * instData, CsrUint8 * thePayload, CsrUint16 thePayloadLength)
{
    if (instData->sendBuffer == NULL)
    {
        if (instData->transmitReady)
        {
            if (thePayloadLength > instData->rfcMaxPacketSize)
            {/* The message can not be send in one RFC packet. Save a reference
                to the payload and send the first fragment */
                CsrUint8 *payloadPiece;

                instData->transmitReady        = FALSE;
                instData->sendBuffer    = thePayload;
                instData->payloadSize    = thePayloadLength;
                instData->payloadIndex    = instData->rfcMaxPacketSize;
                payloadPiece = (CsrUint8 *) CsrPmemAlloc(instData->rfcMaxPacketSize);
                CsrMemCpy(payloadPiece, thePayload, instData->rfcMaxPacketSize);
                CsrBtCmDataReqSend(instData->sapcConnId, instData->rfcMaxPacketSize, payloadPiece);
            }
            else
            {
                instData->transmitReady    = FALSE;
                CsrBtCmDataReqSend(instData->sapcConnId, thePayloadLength, thePayload);
            }
        }
        else
        {
            instData->sendBuffer = thePayload;
            instData->payloadSize = thePayloadLength;
            instData->payloadIndex = 0;
        }
    }
    else
    {
        CsrPmemFree (thePayload);
    }
}

static CsrUint16 getPacketLength(CsrUint8 * receivedData, CsrUint16 receivedDataLength)
{
    CsrUint16 packetLength, i;
    CsrUint8 numberOfPar;
    CsrUint8 padding;
    CsrUint16 parLength;

    packetLength = CSR_BT_MSG_HEADER_LENGTH;
    if (receivedDataLength > 1)
    {
        numberOfPar = receivedData[1];
    }
    else
    {
        return 0;
    }
    padding = 0;
    parLength = 0;
    for (i=0;i < numberOfPar; i++)
    {
        CsrUint8 parLengthIndex = CSR_BT_MSG_HEADER_LENGTH + i * CSR_BT_MIN_PAR_LENGTH + 2 + padding + parLength;
        if ((parLengthIndex + 1) >= receivedDataLength)
        {
            return 0;
        }
        parLength = receivedData[parLengthIndex] << 8;
        parLength += receivedData[parLengthIndex + 1];
        padding = CSR_BT_FIND_PADDING(parLength);
        packetLength += CSR_BT_MIN_PAR_LENGTH + parLength + padding;
    }
    return packetLength;
}

static void csrBtSapcDefragmentationHandler(SapcInstanceData_t * instData, CsrUint8 **data, CsrUint16 payloadLength)
{
    CsrUint8        *sapcData;

    sapcData    = *(data);

    if (instData->recvBuffer == NULL)
    {/* This data packet contains the first part of an SAPC packet */
        CsrUint16    sapcPacketLength;

        sapcPacketLength = getPacketLength(sapcData, payloadLength);
        if (sapcPacketLength == 0)
        {
            /* header not complete */
            instData->recvBuffer = sapcData;
            *data = NULL;
            instData->recvBufferLength = payloadLength;
            instData->headerComplete = FALSE;
        }
        else if (payloadLength < sapcPacketLength)
        { /* The data packet contains only part of the SAPC packet */
            if (sapcPacketLength > instData->maxMsgSize)
            { /* The Packet sent from server is larger than agreed upon. */
                CsrBtCmDisconnectReqSend(instData->sapcConnId);
            }
            else
            { /* The SAPC packet is within the agreed SAPC packet size.
                 Save the received part of the SAPC packet */
                instData->recvBuffer = (CsrUint8 *) CsrPmemAlloc(sapcPacketLength);
                CsrMemCpy(instData->recvBuffer, sapcData, payloadLength);
                instData->recvBufferLength = payloadLength;
            }
            instData->headerComplete = TRUE;
            CsrPmemFree(sapcData);
            *data = NULL;
        }
    }
    else
    {/* This data packet is a defragment of the SAPC packet being send */
        if (instData->headerComplete == FALSE)
        {
            CsrUint8 * tmp;
            tmp = instData->recvBuffer;
            instData->recvBuffer = CsrPmemAlloc(instData->recvBufferLength + payloadLength);
            CsrMemCpy(instData->recvBuffer, tmp, instData->recvBufferLength);
            CsrPmemFree (tmp);
            CsrMemCpy(instData->recvBuffer + instData->recvBufferLength, sapcData, payloadLength);
            instData->recvBufferLength += payloadLength;
            if (instData->recvBufferLength >= getPacketLength(instData->recvBuffer, instData->recvBufferLength))
            {
                /* this means that this was the rest of the packet */
                CsrPmemFree(sapcData);
                *data = instData->recvBuffer;
            }
        }
        else if (instData->recvBufferLength + payloadLength >= getPacketLength(instData->recvBuffer, instData->recvBufferLength))
        { /* This data packet make up the last fragment of the SAPC packet */
            CsrMemCpy(&(instData->recvBuffer[instData->recvBufferLength]), sapcData, getPacketLength(instData->recvBuffer, instData->recvBufferLength) - instData->recvBufferLength);
            instData->recvBufferLength = getPacketLength(instData->recvBuffer, instData->recvBufferLength);
            CsrPmemFree(sapcData);
            *data = instData->recvBuffer;
        }
        else
        { /* This data packet is not the last defragment of the SAPC packet */
            CsrMemCpy(&(instData->recvBuffer[instData->recvBufferLength]), sapcData, payloadLength);
            instData->recvBufferLength += payloadLength;
            CsrPmemFree(sapcData);
            *data = NULL;
        }
    }
}

/******************************************************************************
 *
 * Local functions for sending downstream (data messages with SAP commands)
 *
 ******************************************************************************/
static void sendConnectReq(SapcInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;
    sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_CONNECT_REQ);

    sendBuffer[0] = CSR_BT_MSGID_CONNECT_REQ;
    sendBuffer[1] = 1;                                        /* Number of parameters */
    sendBuffer[2] = 0x00;                                    /* Reserved */
    sendBuffer[3] = 0x00;                                    /* Reserved */
    sendBuffer[4] = CSR_BT_PARAM_ID_MAX_MSG_SIZE;
    sendBuffer[5] = 0x00;                                    /* Reserved */
    sendBuffer[6] = 0x00;                                    /* Parameter length MSB */
    sendBuffer[7] = 0x02;                                    /* Parameter length LSB */
    sendBuffer[8] = (instData->maxMsgSize >> 8) & 0xFF;        /* Parameter value MSB */
    sendBuffer[9] = instData->maxMsgSize & 0xFF;            /* Parameter value LSB */
    sendBuffer[10] = 0x00;                                    /* Padding */
    sendBuffer[11] = 0x00;                                    /* padding */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_CONNECT_REQ);
}

static void sendDisconnectReq(SapcInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;
    sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_DISCONNECT_REQ);

    sendBuffer[0] = CSR_BT_MSGID_DISCONNECT_REQ;
    sendBuffer[1] = 0;                                        /* Number of parameters */
    sendBuffer[2] = 0x00;                                    /* Reserved */
    sendBuffer[3] = 0x00;                                    /* Reserved */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_DISCONNECT_REQ);
}

static void startSdcSearch(SapcInstanceData_t * instData)
{
    if (instData->searchOngoing == FALSE)
    {
        CmnCsrBtLinkedListStruct *sdpTagList = NULL;
        CsrUint16 shIndex;

        instData->searchOngoing = TRUE;
        sdpTagList = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTagList, CSR_BT_SIM_ACCESS_PROFILE_UUID, &shIndex);
        if (NULL != sdpTagList)
        {
            CsrUint8 modemStatus = CSR_BT_MODEM_SEND_CTRL_DTE_DEFAULT;
            CsrBtUtilSdrInsertLocalServerChannel(sdpTagList, shIndex, instData->serverChannel);
            CsrBtUtilRfcConStart((void *)instData, instData->sdpSapcSearchConData, sdpTagList,instData->bdAddr,
                                instData->secOutgoing, FALSE, NULL,
                                CSR_BT_SAPC_PROFILE_DEFAULT_MTU_SIZE,
                                modemStatus,
                                CSR_BT_DEFAULT_MSC_TIMEOUT);
            instData->state = Search_s;
        }
    }

}

/******************************************************************************
 *
 * Handlers for Upstream messages
 *
 ******************************************************************************/

/******************************************************************************
 * Handler for CSR_BT_CM_REGISTER_CFM in Init_s
 ******************************************************************************/
void CsrBtSapcInitStateCmRegisterHandler(SapcInstanceData_t * instData)
{
    CsrBtCmRegisterCfm * prim;
    prim = (CsrBtCmRegisterCfm *) instData->recvMsgP;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->serverChannel = prim->serverChannel;
        CsrBtSapcRestoreSavedMessages(instData);
        instData->state = Idle_s;
    }
    else
    {/* try again */
        CsrBtCmRegisterReqSend(CSR_BT_SAPC_IFACEQUEUE);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_CONTROL_IND in all States
 ******************************************************************************/
void CsrBtSapcXStateCmControlIndHandler(SapcInstanceData_t * instData)
{
    /* ignore CSR_BT_CM_CONTROL_IND in all states */
}

/******************************************************************************
 * Handler for CSR_BT_CM_RFC_MODE_CHANGE_IND in all States
 ******************************************************************************/
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtSapcXStateCmModeChangeIndHandler(SapcInstanceData_t * instData)
{
    CsrBtCmRfcModeChangeInd * cmPrim;
    cmPrim = (CsrBtCmRfcModeChangeInd *) instData->recvMsgP;

    if(cmPrim->mode == CSR_BT_PARK_MODE)
    {
        CsrBtCmRfcModeChangeReqSend(cmPrim->btConnId, CSR_BT_ACTIVE_MODE);
    }
}
#endif
/******************************************************************************
 * Handler for CSR_BT_CM_CONNECT_CFM in Connected_s
 ******************************************************************************/
void CsrBtSapcConnectedStateCmConnectCfmHandler(SapcInstanceData_t * instData)
{
    CsrBtCmConnectCfm * prim;
    prim = (CsrBtCmConnectCfm *) instData->recvMsgP;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->sapcConnId         = prim->btConnId;
        instData->rfcMaxPacketSize   = prim->profileMaxFrameSize;
        /* set default connectCfm Result , used if CSR_BT_CM_DISCONNECT_IND is received
           before SAP connection is established */
        instData->connectCfmResult  = CSR_BT_RESULT_CODE_SAP_CONNECTION_ATTEMPT_FAILED;
        sendConnectReq(instData);
    }
    else
    {
        instData->connectCfmResult = CSR_BT_RESULT_CODE_SAP_CONNECTION_ATTEMPT_FAILED;
        /* send CSR_BT_SAPC_CONNECT_CFM in Idle_s when CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM have been received */
        instData->state = Idle_s;
    }

}

/******************************************************************************
 * Handler for CSR_BT_CM_DISCONNECT_IND in Connected_s
 ******************************************************************************/
void CsrBtSapcConnectedStateCmDisconnectIndHandler(SapcInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;
    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                instData->bdAddr,prim->btConnId);

        switch(instData->state)
        {
            case Connected_s:
                csrBtSapcSendConnectCfm(instData->appHandle, 
                                        instData->sapcConnId, 0,
                                        (CsrBtResultCode) prim->reasonCode,
                                        (CsrBtSupplier) prim->reasonSupplier);
                break;

            case ConnectedFail_s:
                csrBtSapcSendConnectCfm(instData->appHandle, 
                                        instData->sapcConnId, 0,
                                        instData->connectCfmResult,
                                        CSR_BT_SUPPLIER_SAPC);
                break;
        }
        resetSendRecvBuffer(instData);
        instData->state = Idle_s;
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->sapcConnId);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_DISCONNECT_IND in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStateCmDisconnectIndHandler(SapcInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;
    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                instData->bdAddr,prim->btConnId);
        csrBtSapcSendDisconnectInd(instData->appHandle, prim->reasonCode, prim->reasonSupplier);
        resetSendRecvBuffer(instData);
        instData->state = Idle_s;
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->sapcConnId);
    }
}

static CsrBtResultCode csrBtSapcMsgInterpretError(CsrUint8 errorCode)
{
    static const CsrBtResultCode errorLut[] =
    {
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS,
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_UNSPECIFICED_ERROR,
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_CARD_NOT_ACC,
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_CARD_POW_OFF,
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_CARD_REMOVED,
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_CARD_POW_ON,
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_DATA_NOT_AVAIL,
        CSR_BT_RESULT_CODE_SAP_PROTOCOL_NOT_SUPPORTED
    };

    if (errorCode == CSR_BT_RSLT_ERR_INVALID_MSG)
    {
        return CSR_BT_RESULT_CODE_SAP_PROTOCOL_INVALID_MSG;
    }
    else
    {
        return (errorCode < CSR_ARRAY_SIZE(errorLut))
            ? errorLut[errorCode]
            : CSR_BT_RESULT_CODE_SAP_UNSPECIFICED_ERROR;
    }
}

static CsrBtResultCode csrBtSapcMsgInterpretConnectResult(CsrUint8 errorCode)
{
    static const CsrBtResultCode errorLut[] =
    {
        CSR_BT_RESULT_CODE_SAP_CONNECTION_SUCCESS,
        CSR_BT_RESULT_CODE_SAP_CONNECTION_ATTEMPT_FAILED,
        CSR_BT_RESULT_CODE_SAP_CONNECTION_MAX_MSG_SIZE_NOT_SUPPORTED,
        CSR_BT_RESULT_CODE_SAP_CONNECTION_MSG_SIZE_TO_SMALL,
        CSR_BT_RESULT_CODE_SAP_CONNECTION_OK_ONGOING_CALL,
    };

    return (errorCode < CSR_ARRAY_SIZE(errorLut))
        ? errorLut[errorCode]
        : CSR_BT_RESULT_CODE_SAP_UNSPECIFICED_ERROR;
}

static void csrBtSapcMsgInterpretHandler(SapcInstanceData_t * instData, CsrUint8 * msg)
{
    switch (msg[CSR_BT_MSGID_INDEX])
    {
        case CSR_BT_MSGID_CONNECT_RES:
            {
                if (instData->state == Connected_s)
                {
                    CsrUint8 connectionStatus;
                    connectionStatus = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    if (msg[CSR_BT_NUMBER_OF_PARAMETER_INDEX] == 2)
                    {/* maxMsgSize included in response message */
                        instData->maxMsgSize = (msg[16] << 8);
                        instData->maxMsgSize += msg[17];
                    }
                    if ((connectionStatus == CSR_BT_OK_CONNECT) || (connectionStatus == CSR_BT_OK_ONGOING_CALL))
                    {
                        csrBtSapcSendConnectCfm(instData->appHandle, instData->sapcConnId, instData->maxMsgSize, csrBtSapcMsgInterpretConnectResult(connectionStatus), CSR_BT_SUPPLIER_SAPC);
                        instData->state = SapConnected_s;
                        instData->SapState = SapIdle_s;
                    }
                    else if ((connectionStatus == CSR_BT_UNABLE_TO_ESTABLISH_CONNECT) || (connectionStatus == CSR_BT_MSG_SIZE_TO_SMALL))
                    {
                        instData->state = ConnectedFail_s;
                        instData->connectCfmResult = csrBtSapcMsgInterpretConnectResult(connectionStatus);
                        CsrBtCmDisconnectReqSend(instData->sapcConnId);
                        /* wait for CSR_BT_CM_DISCONNECT_IND */
                    }
                    else
                    {
                        /* server did not accept maxMsgSize, but suggested a new maxMsgSize */
                        sendConnectReq(instData);
                    }
                }
                /* else ignore */
                break;
            }
        case CSR_BT_MSGID_DISCONNECT_RES:
            {
                if (instData->state == Disconnecting_s)
                {
                    CsrBtCmDisconnectReqSend(instData->sapcConnId);
                }
                break;
            }
        case CSR_BT_MSGID_DISCONNECT_IND:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_DISCONNECTION_TYPE)
                {
                    CsrUint8 disconnectType = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];

                    if (disconnectType == CSR_BT_IMMEDIATE_DISCONNECT)
                    {
                        instData->state = Disconnecting_s;
                        instData->serverDisconnect = TRUE;
                        CsrBtCmDisconnectReqSend(instData->sapcConnId);
                    }
                    else
                    {
                        /* else notify the client that server want's to disconnect */
                        csrBtSapcSendDisconnectInd(instData->appHandle, CSR_BT_RESULT_CODE_SAP_GRACEFUL_CONNECTION_TERM_BY_PEER, CSR_BT_SUPPLIER_SAPC);
                    }
                }
                /* else ignore */
                break;
            }
        case CSR_BT_MSGID_TRANSFER_APDU_RES:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_RESULT_CODE)
                {
                    CsrUint8 resultCode;
                    resultCode = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    if (resultCode == CSR_BT_RSLT_OK_REQUEST)
                    {
                        CsrUint16 responseLength;
                        CsrUint8 * response;
                        responseLength = msg[14] << 8;
                        responseLength += msg[15];
                        response = CsrPmemAlloc(responseLength * sizeof(CsrUint8));
                        CsrMemCpy (response, msg + 16, responseLength);
                        csrBtSapcSendTransferApduCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC, responseLength, response);
                    }
                    else
                    {
                        csrBtSapcSendTransferApduCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC, 0, NULL);
                    }
                    instData->SapState = SapIdle_s;
                }
                break;
            }
        case CSR_BT_MSGID_TRANSFER_ATR_RES:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_RESULT_CODE)
                {
                    CsrUint8 resultCode;
                    resultCode = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    if (resultCode == CSR_BT_RSLT_OK_REQUEST)
                    {
                        CsrUint16 atrLength;
                        CsrUint8 * atr;
                        atrLength = msg[14] << 8;
                        atrLength += msg[15];
                        atr = CsrPmemAlloc(atrLength * sizeof(CsrUint8));
                        CsrMemCpy (atr, msg + 16, atrLength);
                        csrBtSapcSendTransferAtrCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC, atrLength, atr);
                    }
                    else
                    {
                        csrBtSapcSendTransferAtrCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC, 0, NULL);
                    }
                    instData->SapState = SapIdle_s;
                }
                break;
            }
        case CSR_BT_MSGID_POWER_SIM_OFF_RES:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_RESULT_CODE)
                {
                    CsrUint8 resultCode;
                    resultCode = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    csrBtSapcSendPowerSimOffCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC);
                    instData->SapState = SapIdle_s;
                }
                break;
            }
        case CSR_BT_MSGID_POWER_SIM_ON_RES:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_RESULT_CODE)
                {
                    CsrUint8 resultCode;
                    resultCode = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    csrBtSapcSendPowerSimOnCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC);
                    instData->SapState = SapIdle_s;
                }
                break;
            }
        case CSR_BT_MSGID_RESET_SIM_RES:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_RESULT_CODE)
                {
                    CsrUint8 resultCode;
                    resultCode = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    csrBtSapcSendResetSimCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC);
                    instData->SapState = SapIdle_s;
                }
                break;
            }
        case CSR_BT_MSGID_TRANSFER_CARD_READER_STATUS_RES:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_RESULT_CODE)
                {
                    CsrUint8 resultCode;
                    resultCode = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    if (resultCode == CSR_BT_RSLT_OK_REQUEST)
                    {
                        CsrUint8 cardReaderStatus;
                        cardReaderStatus = msg[16];
                        csrBtSapcSendCardReaderStatusCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC, cardReaderStatus);
                    }
                    else
                    {
                        csrBtSapcSendCardReaderStatusCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC, 0);
                    }

                }
                break;
            }
        case CSR_BT_MSGID_SET_TRANSFER_PROTOCOL_RES:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_RESULT_CODE)
                {
                    CsrUint8 resultCode;
                    resultCode = msg[CSR_BT_FIRST_PARAMETER_VALUE_INDEX];
                    csrBtSapcSendSetTransferProtocolCfm(instData->appHandle, csrBtSapcMsgInterpretError(resultCode), CSR_BT_SUPPLIER_SAPC);
                    instData->SapState = SapIdle_s;
                }
                break;
            }

        case CSR_BT_MSGID_STATUS_IND:
            {
                if (msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_STATUS_CHANGE)
                {
                    CsrUint8 statusChange = msg[8];
                    csrBtSapcSendStatusInd(instData->appHandle, statusChange);
                }
                /* else ignore */
                break;
            }
        case CSR_BT_MSGID_ERROR_RES:
            {
                /* ERROR_RESP received from server */
                CsrBtResultCode resultCode = csrBtSapcMsgInterpretError(CSR_BT_RSLT_ERR_INVALID_MSG);
                switch (instData->SapState)
                {
                    case SapApdu_s:
                        {
                            csrBtSapcSendTransferApduCfm(instData->appHandle, resultCode, CSR_BT_SUPPLIER_SAPC, 0, NULL);
                            break;
                        }
                    case SapAtr_s:
                        {
                            csrBtSapcSendTransferAtrCfm(instData->appHandle, resultCode, CSR_BT_SUPPLIER_SAPC, 0, NULL);
                            break;
                        }
                    case SapOff_s:
                        {
                            csrBtSapcSendPowerSimOffCfm(instData->appHandle, resultCode, CSR_BT_SUPPLIER_SAPC);
                            break;
                        }
                    case SapOn_s:
                        {
                            csrBtSapcSendPowerSimOnCfm(instData->appHandle, resultCode, CSR_BT_SUPPLIER_SAPC);
                            break;
                        }
                    case SapReset_s:
                        {
                            csrBtSapcSendResetSimCfm(instData->appHandle, resultCode, CSR_BT_SUPPLIER_SAPC);
                            break;
                        }
                    case SapCardRead_s:
                        {
                            csrBtSapcSendCardReaderStatusCfm(instData->appHandle, resultCode, CSR_BT_SUPPLIER_SAPC, 0);
                            break;
                        }
                    case SapSetProtocol_s:
                        {
                            csrBtSapcSendSetTransferProtocolCfm(instData->appHandle, resultCode, CSR_BT_SUPPLIER_SAPC);
                            break;
                        }
                    default :
                        { /* SapIdle_s */
                            break;
                        }
                }
                instData->SapState = SapIdle_s;
                break;

            }
        default:
            {
                /* */
                break;
            }
    }
    CsrPmemFree (msg);
}
/******************************************************************************
 * Handler for CSR_BT_CM_DATA_IND in all states
 ******************************************************************************/
void CsrBtSapcDataIndHandler(SapcInstanceData_t * instData)
{
    CsrBtCmDataInd * prim;
    CsrUint8 * msg;
    prim = (CsrBtCmDataInd *) instData->recvMsgP;

    CsrBtCmDataResSend(prim->btConnId);
    msg = prim->payload;
    csrBtSapcDefragmentationHandler(instData, &msg, prim->payloadLength);
    if(msg != NULL)
    {
        csrBtSapcMsgInterpretHandler(instData, msg);
        instData->recvBuffer = NULL;
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_DATA_CFM in all states
 ******************************************************************************/
void CsrBtSapcDataCfmHandler(SapcInstanceData_t * instData)
{
    if (instData->sendBuffer != NULL)
    { /* There is still data available in the sundBuffer that need
         to be sent */
        CsrUint8        *payloadPiece;
        CsrUint32        missingPieceLength;

        missingPieceLength = instData->payloadSize - instData->payloadIndex;

        if (missingPieceLength > instData->rfcMaxPacketSize)
        { /* The data that is available, cannot be sent in one RFC packet.
             Send the next piece of information */
            payloadPiece        = (CsrUint8 *) CsrPmemAlloc(instData->rfcMaxPacketSize);
            CsrMemCpy(payloadPiece, &(instData->sendBuffer[instData->payloadIndex]), instData->rfcMaxPacketSize);
            instData->payloadIndex += instData->rfcMaxPacketSize;
            CsrBtCmDataReqSend(instData->sapcConnId, instData->rfcMaxPacketSize, payloadPiece);
        }
        else
        { /* The data that is available fits into one RFC packet */
            payloadPiece = (CsrUint8 *) CsrPmemAlloc(missingPieceLength);
            CsrMemCpy(payloadPiece, &(instData->sendBuffer[instData->payloadIndex]), missingPieceLength);
            CsrBtCmDataReqSend(instData->sapcConnId, (CsrUint16) missingPieceLength, payloadPiece);
            CsrPmemFree(instData->sendBuffer);
            instData->sendBuffer    = NULL;
        }
    }
    else
    { /* There is no data in the sendBuffer that need to be sent */
        instData->transmitReady    = TRUE;
        CsrBtSapcRestoreSavedMessages(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_DISCONNECT_IND in Disconnecting_s
 ******************************************************************************/
void CsrBtSapcDisconnectingStateCmDisconnectIndHandler(SapcInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;
    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                instData->bdAddr,prim->btConnId);
        csrBtSapcSendDisconnectInd(instData->appHandle, CSR_BT_RESULT_CODE_SAP_IMMEDIATE_CONNECTION_TERM_BY_PEER, CSR_BT_SUPPLIER_SAPC);
        resetSendRecvBuffer(instData);
        instData->state = Idle_s;
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->sapcConnId);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_CONNECT_REQ in Idle_s
 ******************************************************************************/
void CsrBtSapcIdleStateConnectReqHandler(SapcInstanceData_t * instData)
{
    CsrBtSapcConnectReq * prim;
    prim = (CsrBtSapcConnectReq *) instData->recvMsgP;

    instData->appHandle = prim->appHandle;
    bd_addr_copy(&instData->bdAddr, &prim->bdAddr);
    instData->maxMsgSize = prim->maxMsgSize;
    startSdcSearch(instData);
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_DISCONNECT_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStateDisconnectReqHandler(SapcInstanceData_t * instData)
{
    if (instData->SapState == SapIdle_s || instData->sendBuffer == NULL)
    {
        /* th profile is in SapIdle and ready to send DISCONNECT_REQ OR
           is currently processing another request    and has finished sending
           the request, this means it is OK to send disconnect */

        sendDisconnectReq(instData);
        instData->serverDisconnect = FALSE;
        instData->state = Disconnecting_s;
    }
    else
    {
        /* the profile is sending another command, save the disconnect request */
        CsrBtSapcSavemessage(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_TRANSFER_APDU_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStateTransferApduReqHandler(SapcInstanceData_t * instData)
{
    CsrBtSapcTransferApduReq * prim;
    CsrUint8 * sendBuffer;
    CsrUint8 index;
    CsrUint8 paddingLength;
    CsrUint16 packetLength;

    prim = (CsrBtSapcTransferApduReq *) instData->recvMsgP;
    /* calculate number of padding bytes for last parameter */
    paddingLength = CSR_BT_FIND_PADDING(prim->commandApduLength);
    packetLength = CSR_BT_LENGTH_TRANSFER_APDU_REQ + prim->commandApduLength + paddingLength;
    sendBuffer = CsrPmemAlloc(packetLength);

    sendBuffer[0] = CSR_BT_MSGID_TRANSFER_APDU_REQ;
    sendBuffer[1] = 0x01;        /* Number of Parameters */
    sendBuffer[2] = 0x00;        /* Reserved */
    sendBuffer[3] = 0x00;        /* Reserved */
    if (prim->apdu7816Type == TRUE)
    {
        sendBuffer[4] = CSR_BT_PARAM_ID_COMMAND_APDU7816;
    }
    else
    {
        sendBuffer[4] = CSR_BT_PARAM_ID_COMMAND_APDU;
    }
    sendBuffer[5] = 0x00;        /* Reserved */
    sendBuffer[6] = (prim->commandApduLength & 0xFF00) >> 8;
    sendBuffer[7] = prim->commandApduLength & 0xFF;

    CsrMemCpy(&sendBuffer[8], prim->commandApdu, prim->commandApduLength);
    /* Add padding to end of the message. The length of each Parameter must be a multiple of four */
    for (index = 0; index < paddingLength; index++)
    {
        sendBuffer[8 + prim->commandApduLength + index] = 0x00;
    }
    SendDataReq(instData, sendBuffer, packetLength);
    instData->SapState = SapApdu_s;
    CsrPmemFree (prim->commandApdu);
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_TRANSFER_ATR_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStateTransferAtrReq(SapcInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;
    sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_TRANSFER_ATR_REQ);
    sendBuffer[0] = CSR_BT_MSGID_TRANSFER_ATR_REQ;
    sendBuffer[1] = 0;                                        /* Number of parameters */
    sendBuffer[2] = 0x00;                                    /* Reserved */
    sendBuffer[3] = 0x00;                                    /* Reserved */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_TRANSFER_ATR_REQ);
    instData->SapState = SapAtr_s;
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_POWER_SIM_OFF_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStatePowerSimOffReq(SapcInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;

    if (instData->SapState == SapIdle_s || instData->sendBuffer == NULL)
    {
        sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_POWER_SIM_OFF_REQ);

        sendBuffer[0] = CSR_BT_MSGID_POWER_SIM_OFF_REQ;
        sendBuffer[1] = 0;                                        /* Number of parameters */
        sendBuffer[2] = 0x00;                                    /* Reserved */
        sendBuffer[3] = 0x00;                                    /* Reserved */

        SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_POWER_SIM_OFF_REQ);
        instData->SapState = SapOff_s;
    }
    else
    {
        CsrBtSapcSavemessage(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_POWER_SIM_ON_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStatePowerSimOnReq(SapcInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;
    sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_POWER_SIM_ON_REQ);
    sendBuffer[0] = CSR_BT_MSGID_POWER_SIM_ON_REQ;
    sendBuffer[1] = 0;                                        /* Number of parameters */
    sendBuffer[2] = 0x00;                                    /* Reserved */
    sendBuffer[3] = 0x00;                                    /* Reserved */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_POWER_SIM_ON_REQ);
    instData->SapState = SapOn_s;
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_RESET_SIM_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStateResetSimReq(SapcInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;
    if (instData->SapState == SapIdle_s || instData->sendBuffer == NULL)
    {
        sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_RESET_SIM_REQ);

        sendBuffer[0] = CSR_BT_MSGID_RESET_SIM_REQ;
        sendBuffer[1] = 0;                                        /* Number of parameters */
        sendBuffer[2] = 0x00;                                    /* Reserved */
        sendBuffer[3] = 0x00;                                    /* Reserved */

        SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_RESET_SIM_REQ);
        instData->SapState = SapReset_s;
    }
    else
    {
        CsrBtSapcSavemessage(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStateTransferCardStatusReq(SapcInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;
    sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_TRANSFER_CARD_READER_STATUS_REQ);

    sendBuffer[0] = CSR_BT_MSGID_TRANSFER_CARD_READER_STATUS_REQ;
    sendBuffer[1] = 0;                                        /* Number of parameters */
    sendBuffer[2] = 0x00;                                    /* Reserved */
    sendBuffer[3] = 0x00;                                    /* Reserved */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_TRANSFER_CARD_READER_STATUS_REQ);
    instData->SapState = SapCardRead_s;
}

/******************************************************************************
 * Handler for CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapcSapConnectedStateSetTransferProtocolReq(SapcInstanceData_t * instData)
{
    CsrBtSapcSetTransferProtocolReq * prim;
    CsrUint8 * sendBuffer;

    prim = (CsrBtSapcSetTransferProtocolReq *) instData->recvMsgP;
    sendBuffer = (CsrUint8 *) CsrPmemAlloc(CSR_BT_LENGTH_SET_TRANSFER_PROTOCOL_REQ);

    sendBuffer[0] = CSR_BT_MSGID_SET_TRANSFER_PROTOCOL_REQ;
    sendBuffer[1] = 0x01;
    sendBuffer[2] = 0x00;
    sendBuffer[3] = 0x00;
    sendBuffer[4] = CSR_BT_PARAM_ID_TRANSFER_PROTOCOL;
    sendBuffer[5] = 0x00;
    sendBuffer[6] = 0x00;
    sendBuffer[7] = 0x01;
    sendBuffer[8] = prim->transportProtocol;
    sendBuffer[9] = 0x00;
    sendBuffer[10] = 0x00;
    sendBuffer[11] = 0x00;

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_SET_TRANSFER_PROTOCOL_REQ);
    instData->SapState = SapSetProtocol_s;
}

/******************************************************************************
* Handler and confirmation sender for security level changing
******************************************************************************/
void CsrBtSapcSecurityOutReqHandler(SapcInstanceData_t * instData)
{
    CsrBtResultCode rval;
    CsrBtSapcSecurityOutReq *prim;

    prim = (CsrBtSapcSecurityOutReq*)instData->recvMsgP;

    rval = CsrBtScSetSecOutLevel(&instData->secOutgoing, prim->secLevel,
        CSR_BT_SIM_ACCESS_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_SIM_ACCESS_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_SAP_SUCCESS,
        CSR_BT_RESULT_CODE_SAP_UNACCEPTABLE_PARAMETER);

    CsrBtSapcSecurityOutCfmSend(prim->appHandle,
        rval, CSR_BT_SUPPLIER_SAPC);
}

void CsrBtSapcSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSapcSecurityOutCfm *prim;
    prim = (CsrBtSapcSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtSapcSecurityOutCfm));
    prim->type = CSR_BT_SAPC_SECURITY_OUT_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    csrBtSapcMessagePut(appHandle, prim);
}

/******************************************************************************
* Connection result handler
******************************************************************************/
void CsrBtSapcRfcSdcConResultHandler(void                   *inst,
                              CsrUint8                      localServerCh,
                              CsrUint32                     sapcConnId,
                              CsrBtDeviceAddr               deviceAddr,
                              CsrUint16                     maxFrameSize,
                              CsrBool                       validPortPar,
                              RFC_PORTNEG_VALUES_T          portPar,
                              CsrBtResultCode               resultCode,
                              CsrBtSupplier                 resultSupplier,
                              CmnCsrBtLinkedListStruct      *sdpTagList)
{
    SapcInstanceData_t *instData = (SapcInstanceData_t *) inst;

    instData->searchOngoing = FALSE;

    if (resultSupplier == CSR_BT_SUPPLIER_CM &&
        resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        CsrUintFast16    numOfSdpRecords = CsrBtUtilBllGetNofEntriesEx(sdpTagList);
        CsrUintFast16    sdpRecordIndex;
        CsrUint16    tmpResult;
        CsrBtUuid32    tmpUuid = 0;
        CsrUint16    dummy1, dummy2; /* Currently CSR_UNUSED */

        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,
                deviceAddr,sapcConnId);

        for (sdpRecordIndex = 0; sdpRecordIndex < numOfSdpRecords; sdpRecordIndex++)
        {
            if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
                                            sdpRecordIndex,
                                            &tmpUuid,
                                            &tmpResult,
                                            &dummy1,
                                            &dummy2))
            {
                if (tmpResult == SDR_SDC_SEARCH_SUCCESS)
                {
                   CsrBtUuid32 serviceHandleTemp = 0;
                   if (TRUE == CsrBtUtilSdrGetServiceHandle(sdpTagList, sdpRecordIndex, &serviceHandleTemp))
                   {
                       instData->serviceHandle = serviceHandleTemp;
                   }
                }
            }
        }
        instData->bdAddr = deviceAddr;
        instData->sapcConnId  = sapcConnId;
        instData->rfcMaxPacketSize = maxFrameSize;
        instData->state  = Connected_s;
        sendConnectReq(instData);
    }
    else
    {
        csrBtSapcSendConnectCfm(instData->appHandle, instData->sapcConnId, 0, resultCode, resultSupplier);
        instData->state = Idle_s;
    }

    CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
}

