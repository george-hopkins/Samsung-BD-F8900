/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_saps_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sap_common_private.h"

#define SAP_SERVER_CHANNEL_INDEX 27

static const CsrUint8 sdsSapsServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x2D,        /* 2 byte UUID, Service class = SIM_Access = 0x112D */
    0x19,0x12,0x04,        /* 2 byte UUID, Service class = GenericTelephony = 0x1204 */

    /* protocol descriptor list */
    0x09,0x00,0x04,        /* AttrId ProtocolDescriptorList */
    0x35,0x0C,             /* 12 bytes in total DataElSeq */
    /* L2CAP */
    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,        /* 2 byte UUID, Protocol = L2CAP */
    /* RFCOMM */
    0x35,0x05,             /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,        /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,             /* 1 byte UINT - server channel template value 0 - to be filled in by app (index:24) */

    /* Bluetooth Profile descriptor List */
    0x09,0x00,0x09,        /* AttrId Bluetooth Profile DescriptorList */
    0x35,0x08,             /* 8 bytes in total DataElSeq */
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x2D,        /* 2 byte UUID, Service class = SIM Access = 0x112D */

    /* Profile version */
    0x09,0x01,0x02,        /* 2 byte UINT - Profile version = 0x0102 = version 1.2 */

    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name. Use language base attribute 0x0100 (primary language) */
    0x25, 0x0A,            /* length of service string: 10 */
    'S','I','M',' ','A','c','c','e','s','s', /* string = "SIM Access" */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static void csrBtSapsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SAPS_PRIM, msg);
}

/*************************************************************************************
  CsrBtSapsSavemessage
************************************************************************************/
void CsrBtSapsSavemessage(SapsInstanceData_t * instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_SAPS_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

/*************************************************************************************
  CsrBtSapsRestoreSavedMessages
************************************************************************************/
void CsrBtSapsRestoreSavedMessages(SapsInstanceData_t * instData)
{
    CsrBtSapsPrim * sapsPrim;
    if (instData->saveQueue != NULL)
    {
        sapsPrim = CsrPmemAlloc(sizeof(CsrBtSapsPrim));
        *sapsPrim = CSR_BT_SAPS_HOUSE_CLEANING;
        csrBtSapsMessagePut(CSR_BT_SAPS_IFACEQUEUE, sapsPrim);
        instData->restoreSapsFlag = TRUE;
    }
}

#define CSR_BT_SAPS_STATE_CHANGE(var, val) csrBtSapsChangeState((CsrCharString*)__FILE__, __LINE__, (CsrCharString*) #var, &(var), val, (CsrCharString*) #val)

static void csrBtSapsChangeState(const CsrCharString *file, CsrUint32 line,
                                      const CsrCharString *fsmName,
                                      CsrUint32 *state, CsrUint32 newState, const CsrCharString *newStateName)
{
#ifdef CSR_LOG_ENABLE
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                           CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
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

/*****************************************************************************
 *
 * Local functions. Only used in csr_bt_saps_sef.c
 *
 *****************************************************************************/

static void sendSapsActivateCfm(SapsInstanceData_t * instData)
{
    CsrBtSapsActivateCfm * prim;

    prim = (CsrBtSapsActivateCfm *) CsrPmemAlloc(sizeof(CsrBtSapsActivateCfm));
    prim->type = CSR_BT_SAPS_ACTIVATE_CFM;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsDeactivateCfm(SapsInstanceData_t * instData)
{
    CsrBtSapsDeactivateCfm * prim;

    prim = (CsrBtSapsDeactivateCfm *) CsrPmemAlloc(sizeof(CsrBtSapsDeactivateCfm));
    prim->type = CSR_BT_SAPS_DEACTIVATE_CFM;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsConnectReq(SapsInstanceData_t * instData, CsrUint16 theMaxMsgSize)
{
    CsrBtSapsConnectInd * prim;

    prim = (CsrBtSapsConnectInd *) CsrPmemAlloc(sizeof(CsrBtSapsConnectInd));
    prim->type = CSR_BT_SAPS_CONNECT_IND;
    prim->maxMsgSize = theMaxMsgSize;
    bd_addr_copy(&(prim->deviceAddr), &(instData->deviceAddr));
    prim->btConnId = instData->sapsConnId;

    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsDisconnectInd(SapsInstanceData_t * instData,
                                        CsrBtReasonCode reasonCode,
                                        CsrBtSupplier reasonSupplier)
{
    CsrBtSapsDisconnectInd * prim;

    prim = (CsrBtSapsDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtSapsDisconnectInd));
    prim->type = CSR_BT_SAPS_DISCONNECT_IND;
    prim->reasonCode = reasonCode;
    prim->reasonSupplier = reasonSupplier;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsTransferAPDUInd(SapsInstanceData_t * instData, CsrUint8 * msg, CsrBool isApdu7816Type)
{
    CsrBtSapsTransferApduInd * prim;

    prim = (CsrBtSapsTransferApduInd *) CsrPmemAlloc(sizeof(CsrBtSapsTransferApduInd));
    prim->type = CSR_BT_SAPS_TRANSFER_APDU_IND;
    prim->apduCommandLength = msg[7] + (msg[6] << 8);
    prim->apduCommand = CsrPmemAlloc(prim->apduCommandLength);
    prim->isApdu7816Type = isApdu7816Type;
    CsrMemCpy(prim->apduCommand, msg + 8, prim->apduCommandLength);
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsTransferAtrInd(SapsInstanceData_t * instData)
{
    CsrBtSapsTransferAtrInd * prim;

    prim = (CsrBtSapsTransferAtrInd *) CsrPmemAlloc(sizeof(CsrBtSapsTransferAtrInd));
    prim->type = CSR_BT_SAPS_TRANSFER_ATR_IND;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsPowerSimOffReq(SapsInstanceData_t * instData)
{
    CsrBtSapsPowerSimOffInd * prim;

    prim = (CsrBtSapsPowerSimOffInd *) CsrPmemAlloc(sizeof(CsrBtSapsPowerSimOffInd));
    prim->type = CSR_BT_SAPS_POWER_SIM_OFF_IND;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsPowerSimOnReq(SapsInstanceData_t * instData)
{
    CsrBtSapsPowerSimOnInd * prim;

    prim = (CsrBtSapsPowerSimOnInd *) CsrPmemAlloc(sizeof(CsrBtSapsPowerSimOnInd));
    prim->type = CSR_BT_SAPS_POWER_SIM_ON_IND;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsCardReaderStatus(SapsInstanceData_t * instData)
{
    CsrBtSapsTransferCardReaderStatusInd * prim;

    prim = (CsrBtSapsTransferCardReaderStatusInd *) CsrPmemAlloc(sizeof(CsrBtSapsTransferCardReaderStatusInd));
    prim->type = CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsSetTransferProtocol(SapsInstanceData_t * instData, CsrUint8 * msg)
{
    CsrBtSapsSetTransferProtocolInd * prim;
    prim = (CsrBtSapsSetTransferProtocolInd *) CsrPmemAlloc(sizeof(CsrBtSapsSetTransferProtocolInd));
    prim->type = CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND;
    prim->protocol = msg[8];  /* value of the transfer protocol parameter */
    csrBtSapsMessagePut(instData->appHandle, prim);
}

static void sendSapsResetSimReq(SapsInstanceData_t * instData)
{
    CsrBtSapsResetSimInd * prim;

    prim = (CsrBtSapsResetSimInd *) CsrPmemAlloc(sizeof(CsrBtSapsResetSimInd));
    prim->type = CSR_BT_SAPS_RESET_SIM_IND;
    csrBtSapsMessagePut(instData->appHandle, prim);
}

void sendTimedModeChangeReq(CsrUint16 notUsed, void * data)
{
    SapsInstanceData_t * instData;
    instData = (SapsInstanceData_t *) data;
    instData->timerPending = FALSE;
    if (instData->state == Connected_s || instData->state == SapConnected_s)
    {
        CsrBtCmRfcModeChangeReqSend(instData->sapsConnId, CSR_BT_SNIFF_MODE);
    }
}

static void registerSapsService(SapsInstanceData_t * instData)
{
    CsrUint8 * sapsServiceRecord;
    sapsServiceRecord = (CsrUint8 *) CsrPmemAlloc(sizeof(sdsSapsServiceRecord));
    CsrMemCpy(sapsServiceRecord, sdsSapsServiceRecord, sizeof(sdsSapsServiceRecord));
    sapsServiceRecord[SAP_SERVER_CHANNEL_INDEX] = instData->serverChannel;

    CsrBtCmSdsRegisterReqSend(CSR_BT_SAPS_IFACEQUEUE, sapsServiceRecord, sizeof(sdsSapsServiceRecord));
}

static void unRegisterSapsService(SapsInstanceData_t * instData)
{
    CsrBtCmSdsUnRegisterReqSend(CSR_BT_SAPS_IFACEQUEUE, instData->serviceRecHandle);
}

static void sendConnectAccept(SapsInstanceData_t * instData)
{
    CsrUint8 modemStatus = CsrBtMapSendingControlSignal((CSR_BT_MODEM_CTS_MASK | CSR_BT_MODEM_DCD_MASK | CSR_BT_MODEM_DSR_MASK), CSR_BT_DCE);
    CsrBtCmConnectAcceptReqSend(CSR_BT_SAPS_IFACEQUEUE,
                           CSR_BT_TELEPHONY_MAJOR_SERVICE_MASK,
                           0,
                           CSR_BT_SAPS_PROFILE_DEFAULT_MTU_SIZE,
                           instData->serverChannel,
                           instData->secIncoming,
                           CSR_BT_SIM_ACCESS_PROFILE_UUID,
                           modemStatus,
                           CSR_BT_DEFAULT_BREAK_SIGNAL,
                           CSR_BT_DEFAULT_MSC_TIMEOUT);
}

static void resetSendRecvBuffer(SapsInstanceData_t * instData)
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

static void SendDataReq(SapsInstanceData_t * instData,
                        CsrUint8 * thePayload,
                        CsrUint16 thePayloadLength)
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
                CsrBtCmDataReqSend(instData->sapsConnId,
                              instData->rfcMaxPacketSize,
                              payloadPiece);
            }
            else
            {
                instData->transmitReady    = FALSE;
                CsrBtCmDataReqSend(instData->sapsConnId,
                              thePayloadLength,
                              thePayload);
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

static void sendDisconnectInd(SapsInstanceData_t * instData, CsrUint8 theDisconnectType)
{
    CsrUint8 * sendBuffer;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_DISCONNECT_IND);

    sendBuffer[0]  = CSR_BT_MSGID_DISCONNECT_IND;
    sendBuffer[1]  = 0x01;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;        /* Reserved */
    sendBuffer[3]  = 0x00;        /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_DISCONNECTION_TYPE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = theDisconnectType;
    sendBuffer[9]  = 0x00;        /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */
    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_DISCONNECT_IND);
}

static void sendDisconnectRes(SapsInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_DISCONNECT_RES);

    sendBuffer[0] = CSR_BT_MSGID_DISCONNECT_RES;
    sendBuffer[1] = 0x00;        /* Number of Parameters */
    sendBuffer[2] = 0x00;        /* Reserved */
    sendBuffer[3] = 0x00;        /* Reserved */
    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_DISCONNECT_RES);
}

static void sendStatusInd(SapsInstanceData_t * instData, CsrUint8 theStatus)
{
    CsrUint8 * sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_STATUS_IND);

    sendBuffer[0]  = CSR_BT_MSGID_STATUS_IND;
    sendBuffer[1]  = 0x01;            /* Number of Parameters */
    sendBuffer[2]  = 0x00;            /* Reserved */
    sendBuffer[3]  = 0x00;            /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_STATUS_CHANGE;
    sendBuffer[5]  = 0x00;            /* Reserved */
    sendBuffer[6]  = 0x00;            /* Parameter length */
    sendBuffer[7]  = 0x01;            /* Parameter length */
    sendBuffer[8]  = theStatus;       /* Parameter value */
    sendBuffer[9]  = 0x00;            /* Padding */
    sendBuffer[10] = 0x00;            /* Padding */
    sendBuffer[11] = 0x00;            /* Padding */
    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_STATUS_IND);
}

static void sendErrorRes(SapsInstanceData_t * instData)
{
    CsrUint8 * sendBuffer;

    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_ERROR_RES);

    sendBuffer[0] = CSR_BT_MSGID_ERROR_RES;
    sendBuffer[1] = 0x00;            /* Number of Parameters */
    sendBuffer[2] = 0x00;            /* Reserved */
    sendBuffer[3] = 0x00;            /* Reserved */
    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_ERROR_RES);
}

static CsrUint16 getPacketLength(CsrUint8 * receivedData, CsrUint16 receivedDataLength)
{
    CsrUint16 packetLength, i;
    CsrUint8 numberOfPar;
    CsrUint8 padding;

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
    for (i=0;i < numberOfPar; i++)
    {
        CsrUint16 parLength;
        CsrUint16 receivedDataIndex;
        receivedDataIndex = CSR_BT_MSG_HEADER_LENGTH + i * CSR_BT_MIN_PAR_LENGTH + 3 + padding;
        if (receivedDataIndex >= receivedDataLength)
        {
            return 0;
        }
        parLength = receivedData[CSR_BT_MSG_HEADER_LENGTH + i * CSR_BT_MIN_PAR_LENGTH + 2 + padding] << 8;
        parLength += receivedData[CSR_BT_MSG_HEADER_LENGTH + i * CSR_BT_MIN_PAR_LENGTH + 3 + padding];
        padding = CSR_BT_FIND_PADDING(parLength);
        packetLength += CSR_BT_MIN_PAR_LENGTH + parLength + padding;
    }
    return packetLength;
}

static void csrBtSapsDefragmentationHandler(SapsInstanceData_t * instData, CsrUint8 **data, CsrUint16 payloadLength)
{
    CsrUint8        *sapsData;

    sapsData    = *(data);

    if (instData->recvBuffer == NULL)
    {/* This data packet contains the first part of an SAPS packet */
        CsrUint16    sapsPacketLength;
        if ( (instData->state == Disconnecting_s) && (instData->serverDisconnect == TRUE))
        {
            CsrPmemFree(sapsData);
            *data = NULL;
        }
        else
        {
            sapsPacketLength = getPacketLength(sapsData, payloadLength);
            if (sapsPacketLength == 0)
            {
                /* header not complete */
                instData->recvBuffer = sapsData;
                *data = NULL;
                instData->recvBufferLength = payloadLength;
                instData->headerComplete = FALSE;
            }
            else if ((instData->maxMsgSize != 0) && (sapsPacketLength > instData->maxMsgSize))
            { /* The SAPS Packet is larger than agreed upon. */
                sendErrorRes(instData);
                CSR_BT_SAPS_STATE_CHANGE(instData->state, Disconnecting_s);
                instData->serverDisconnect = TRUE;
                instData->serverDeactivated = FALSE;
                CsrPmemFree(sapsData);
                *data = NULL;
                /* send CSR_BT_CM_DISCONNECT_REQ when CSR_BT_CM_DATA_CFM is received in Disconnecting_s */
            }
            else if (payloadLength < sapsPacketLength)
            { /* The data packet contains only part of the SAPS packet */

                /* The SAPS packet is within the agreed SAPS packet size.
                   Save the received part of the SAPS packet */
                instData->recvBuffer = (CsrUint8 *) CsrPmemAlloc(sapsPacketLength);
                CsrMemCpy(instData->recvBuffer, sapsData, payloadLength);
                instData->recvBufferLength = payloadLength;
                instData->headerComplete = TRUE;
                CsrPmemFree(sapsData);
                *data = NULL;
            }
            else
            {
                /* else entire message was contained in this packet */
            }
        }
    }
    else
    {
        /* This data packet is a defragment of the SAPS packet being send */
        if (instData->headerComplete == FALSE)
        {
            CsrUint8 * tmp;
            tmp = instData->recvBuffer;
            instData->recvBuffer = CsrPmemAlloc(instData->recvBufferLength + payloadLength);
            CsrMemCpy(instData->recvBuffer, tmp, instData->recvBufferLength);
            CsrPmemFree (tmp);
            CsrMemCpy(instData->recvBuffer + instData->recvBufferLength, sapsData, payloadLength);
            instData->recvBufferLength += payloadLength;
            if (instData->recvBufferLength >= getPacketLength(instData->recvBuffer, instData->recvBufferLength))
            {
                /* this means that this was the rest of the packet */
                CsrPmemFree(sapsData);
                *data = instData->recvBuffer;
            }
        }
        else if (instData->recvBufferLength + payloadLength >= getPacketLength(instData->recvBuffer, instData->recvBufferLength))
        {
            /* This data packet make up the last fragment of the SAPS packet */
            CsrMemCpy(&(instData->recvBuffer[instData->recvBufferLength]),
                   sapsData,
                   getPacketLength(instData->recvBuffer, instData->recvBufferLength) - instData->recvBufferLength);

            instData->recvBufferLength = getPacketLength(instData->recvBuffer, instData->recvBufferLength);
            CsrPmemFree(sapsData);
            *data = instData->recvBuffer;
        }
        else
        {
            /* This data packet is not the last defragment of the SAPS packet */
            CsrMemCpy(&(instData->recvBuffer[instData->recvBufferLength]), sapsData, payloadLength);
            instData->recvBufferLength += payloadLength;
            CsrPmemFree(sapsData);
            *data = NULL;
        }
    }
}
/*****************************************************************************
 *
 * Handler functions for Upstream messages
 *
 *****************************************************************************/

/******************************************************************************
 * Handler for CSR_BT_CM_REGISTER_CFM in Init_s
 ******************************************************************************/
void CsrBtSapsInitStateCmRegisterHandler(SapsInstanceData_t * instData)
{
    CsrBtCmRegisterCfm * prim;

    prim = (CsrBtCmRegisterCfm *) instData->recvMsgP;
    instData->serverChannel = prim->serverChannel;

    if (instData->activateSaps && instData->sapRegistered == FALSE)
    {
        registerSapsService(instData);
    }
    else if (instData->activateSaps)
    {
        sendConnectAccept(instData);
    }
    CSR_BT_SAPS_STATE_CHANGE(instData->state, Idle_s);
}



/******************************************************************************
 * Handler for CSR_BT_CM_SDS_REGISTER_CFM in Idle_s
 ******************************************************************************/
void CsrBtSapsIdleStateCmSdsRegisterCfm(SapsInstanceData_t * instData)
{
    CsrBtCmSdsRegisterCfm * prim;

    prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->serviceRecHandle = prim->serviceRecHandle;
        instData->sapRegistered    = TRUE;
        CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
        sendSapsActivateCfm(instData);
        sendConnectAccept(instData);
    }
    else
    {
        registerSapsService(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_CONNECT_ACCEPT_CFM in Activated_s
 ******************************************************************************/
void CsrBtSapsActivatedStateConnectAcceptHandler(SapsInstanceData_t * instData)
{
    CsrBtCmConnectAcceptCfm * prim;

    prim = (CsrBtCmConnectAcceptCfm *) instData->recvMsgP;

    if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
        prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        CSR_BT_SAPS_STATE_CHANGE(instData->state, Connected_s);
        CSR_BT_SAPS_STATE_CHANGE(instData->connectState, BtConnect_s);
        instData->sapsConnId       = prim->btConnId;
        instData->rfcMaxPacketSize = prim->profileMaxFrameSize;
        instData->deviceAddr       = prim->deviceAddr;
        
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,
                        prim->deviceAddr,prim->btConnId);

        if (instData->timerPending == FALSE)
        {
            /* Send Request for Sniff in SNIFF_DELAY secs if state is Connected or SapConnected */
            instData->timerId = CsrSchedTimerSet(SNIFF_DELAY, sendTimedModeChangeReq, 0, instData);
            instData->timerPending = TRUE;
        }
    }
    else
    {
        sendConnectAccept(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_DISCONNECT_IND in Connected_s
 ******************************************************************************/
void CsrBtSapsConnectStateDisconnectInd(SapsInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;

    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                instData->deviceAddr,prim->btConnId);

        resetSendRecvBuffer(instData);
        if (instData->timerPending)
        {
            if (CsrSchedTimerCancel(instData->timerId, NULL, NULL))
            {
                instData->timerPending = FALSE;
            }
        }

        switch (instData->connectState)
        {
            case BtConnect_s:
                {
                    /* Disconnect received before information have been send to server app
                       disconnect silently */
                    sendConnectAccept(instData);
                    CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
                    break;
                }
            case ConnectIndSend_s:
                {
                    /* Disconnect received after connect indication have been send to server app
                       wait for Connect response from server app and send Disconnect Ind. Handled in
                       Disconnecting_s*/
                   instData->discReason = prim->reasonCode;
                   instData->serverDeactivated = FALSE;
                   CSR_BT_SAPS_STATE_CHANGE(instData->state, Disconnecting_s);
                   break;
                }
            case ConnectResSend_s:
                {
                    sendConnectAccept(instData);
                    CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
                    sendSapsDisconnectInd(instData, prim->reasonCode, prim->reasonSupplier);
                    break;
                }
            case ConnectDisconnect_s:
                {
                    /* Server has responded with error and disconnected */
                    sendConnectAccept(instData);
                    CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
                    break;
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            default:
                {
                    CsrGeneralException("SAPS",
                                        0,
                                        (CsrUint16)  instData->connectState,
                                         "Unknown state");
                    break;
                }
#endif

        }
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->sapsConnId);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_CONTROL_IND in in all states
 ******************************************************************************/
void CsrBtSapsControlInd(SapsInstanceData_t * instData)
{
    /* empty handler */
}

/******************************************************************************
 * Handler for CSR_BT_CM_RFC_MODE_CHANGE_IND in all states
 ******************************************************************************/
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtSapsModeChangeInd(SapsInstanceData_t * instData)
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
 * Handler for CSR_BT_CM_DISCONNECT_IND in SapConnected_s
 ******************************************************************************/
void CsrBtSapsSapConnectedStateDisconnectInd(SapsInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;

    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                instData->deviceAddr,prim->btConnId);
        resetSendRecvBuffer(instData);
        sendConnectAccept(instData);
        CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
        sendSapsDisconnectInd(instData, prim->reasonCode, prim->reasonSupplier);
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->sapsConnId);
    }
}

void CsrBtSapsMsgInterpretHandler(SapsInstanceData_t * instData, CsrUint8 * msg)
{
    switch (msg[CSR_BT_MSGID_INDEX])
    {
        case CSR_BT_MSGID_CONNECT_REQ:
            {
                if (msg[CSR_BT_NUMBER_OF_PARAMETER_INDEX] == 1 && msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_MAX_MSG_SIZE)
                {
                    CsrUint16 maxMsgSize;
                    maxMsgSize = msg[CSR_BT_MAX_MSG_SIZE_INDEX_LSB] + (msg[CSR_BT_MAX_MSG_SIZE_INDEX_MSB] << 8);
                    sendSapsConnectReq(instData, maxMsgSize);
                    CSR_BT_SAPS_STATE_CHANGE(instData->connectState, ConnectIndSend_s);
                }
                else
                {
                    sendErrorRes(instData);
                }
                break;
            }
        case CSR_BT_MSGID_DISCONNECT_REQ:
            {
                sendSapsDisconnectInd(instData, CSR_BT_RESULT_CODE_SAP_GRACEFUL_CONNECTION_TERM_BY_PEER, CSR_BT_SUPPLIER_SAPS);
                sendDisconnectRes(instData);
                CSR_BT_SAPS_STATE_CHANGE(instData->state, Disconnecting_s);
                instData->serverDisconnect = FALSE;
                instData->serverDeactivated = FALSE;
                break;
            }
        case CSR_BT_MSGID_TRANSFER_APDU_REQ:
            {
                /* verify that message is properly formatted */
                if (msg[CSR_BT_NUMBER_OF_PARAMETER_INDEX] == 1 && msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_COMMAND_APDU)
                {
                    sendSapsTransferAPDUInd(instData, msg, FALSE);
                }
                else if (msg[CSR_BT_NUMBER_OF_PARAMETER_INDEX] == 1 && msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_COMMAND_APDU7816)
                {
                    sendSapsTransferAPDUInd(instData, msg, TRUE);
                }
                else
                {
                    sendErrorRes(instData);
                }
                break;
            }
        case CSR_BT_MSGID_TRANSFER_ATR_REQ:
            {
                sendSapsTransferAtrInd(instData);
                break;
            }
        case CSR_BT_MSGID_POWER_SIM_OFF_REQ:
            {
                sendSapsPowerSimOffReq(instData);
                break;
            }
        case CSR_BT_MSGID_POWER_SIM_ON_REQ:
            {
                sendSapsPowerSimOnReq(instData);
                break;
            }
        case CSR_BT_MSGID_RESET_SIM_REQ:
            {
                sendSapsResetSimReq(instData);
                break;
            }
        case CSR_BT_MSGID_TRANSFER_CARD_READER_STATUS_REQ:
            {
                sendSapsCardReaderStatus(instData);
                break;
            }
        case CSR_BT_MSGID_SET_TRANSFER_PROTOCOL_REQ:
            {
                if (msg[CSR_BT_NUMBER_OF_PARAMETER_INDEX] == 1 && msg[CSR_BT_FIRST_PARAMETER_INDEX] == CSR_BT_PARAM_ID_TRANSFER_PROTOCOL)
                {
                    sendSapsSetTransferProtocol(instData, msg);
                }
                else
                {
                    sendErrorRes(instData);
                }
                break;
            }
        default:
            {
                sendErrorRes(instData);
                break;
            }
    }
    CsrPmemFree (msg);
}

/******************************************************************************
 * Handler for CSR_BT_CM_DATA_IND in all states
 ******************************************************************************/
void CsrBtSapsDataIndHandler(SapsInstanceData_t * instData)
{
    CsrBtCmDataInd * prim;
    CsrUint8 * msg;

    prim = (CsrBtCmDataInd *) instData->recvMsgP;

    msg = prim->payload;

    CsrBtCmDataResSend(instData->sapsConnId);
    csrBtSapsDefragmentationHandler(instData, &msg, prim->payloadLength);

    if(msg != NULL)
    {
        CsrBtSapsMsgInterpretHandler(instData, msg);
        instData->recvBuffer = NULL;
    }
}

/******************************************************************************
 * General handler for CSR_BT_CM_DATA_CFM
 ******************************************************************************/
void CsrBtSapsDataCfmHandler(SapsInstanceData_t * instData)
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
            CsrBtCmDataReqSend(instData->sapsConnId, instData->rfcMaxPacketSize, payloadPiece);
        }
        else
        { /* The data that is available fits into one RFC packet */
            payloadPiece = (CsrUint8 *) CsrPmemAlloc(missingPieceLength);
            CsrMemCpy(payloadPiece, &(instData->sendBuffer[instData->payloadIndex]), missingPieceLength);
            CsrBtCmDataReqSend(instData->sapsConnId, (CsrUint16) missingPieceLength, payloadPiece);
            CsrPmemFree(instData->sendBuffer);
            instData->sendBuffer    = NULL;
        }
    }
    else
    { /* There is no data in the sendBuffer that need to be sent */
        instData->transmitReady    = TRUE;
        CsrBtSapsRestoreSavedMessages(instData);
        switch (instData->state)
        {
            case Connected_s:
                {
                    if (instData->connectState == ConnectResSend_s)
                    {
                        sendStatusInd(instData, instData->statusToSend);
                        CSR_BT_SAPS_STATE_CHANGE(instData->state, SapConnected_s);
                    }
                    else if (instData->connectState == ConnectDisconnect_s)
                    {
                        CsrBtCmDisconnectReqSend(instData->sapsConnId);
                    }
                    break;
                }
            case SapConnected_s:
                {
                    /* ignore */
                    break;
                }
            case Disconnecting_s:
                {
                    if (instData->serverDisconnect == TRUE)
                    {
                        CsrBtCmDisconnectReqSend(instData->sapsConnId);
                    }
                    else
                    {
                        if (instData->serverDeactivated)
                        {
                            /* go to Deactivate_s and wait for CSR_BT_CM_DISCONNECT_IND */
                            CSR_BT_SAPS_STATE_CHANGE(instData->state, Deactivate_s);
                        }
                        else
                        {
                            CSR_BT_SAPS_STATE_CHANGE(instData->state, Connected_s);
                            CSR_BT_SAPS_STATE_CHANGE(instData->connectState, BtConnect_s);
                        }
                    }
                    break;
                }
            case Deactivate_s:
                {
                    /* This means that we just send a DATA_REQ with a disconnect Ind */
                    CsrBtCmDisconnectReqSend(instData->sapsConnId);
                    break;
                }
            default:
                {
                    /* ignore */
                    break;
                }
        }
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_DISCONNECT_IND in Disconnecting_s
 ******************************************************************************/
void CsrBtSapsDisconnectingStateDisconnectInd(SapsInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;

    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                instData->deviceAddr,prim->btConnId);
        resetSendRecvBuffer(instData);
        if (instData->serverDisconnect)
        {
            /* server disconnected. Disconnect ind send after the Bluetooth link is down */
            sendSapsDisconnectInd(instData, CSR_BT_RESULT_CODE_SAP_CONNECTION_TERM_BY_SELF, CSR_BT_SUPPLIER_SAPS);
            if (instData->serverDeactivated)
            {
                /* a deactivate request has been send while disconnecting */
                unRegisterSapsService(instData);
                CSR_BT_SAPS_STATE_CHANGE(instData->state, Deactivate_s);
            }
            else
            {
                CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
                sendConnectAccept(instData);
            }
        }
        else
        {
            CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
            sendConnectAccept(instData);
        }
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->sapsConnId);
    }
}
/******************************************************************************
 * Handler for CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM in Deactivate_s
 ******************************************************************************/
void CsrBtSapsDeactivateCancelAcceptConnectCfm(SapsInstanceData_t * instData)
{
    CsrBtCmCancelAcceptConnectCfm * prim;
    prim = (CsrBtCmCancelAcceptConnectCfm *) instData->recvMsgP;

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        unRegisterSapsService(instData);
    }
    /* else wait for CM_ACCEPT_CONNECT_CFM and disconnect */
}

/******************************************************************************
 * Handler for CSR_BT_CM_CONNECT_ACCEPT_CFM in Deactivate_s
 ******************************************************************************/
void CsrBtSapsDeactivateAcceptConnectCfm(SapsInstanceData_t * instData)
{
    CsrBtCmConnectAcceptCfm * prim;
    prim = (CsrBtCmConnectAcceptCfm *) instData->recvMsgP;

    if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
        prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        /* Connection established while trying to Deactivate, Disconnect at once */
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
    else
    {
        unRegisterSapsService(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_DISCONNECT_IND in Deactivate_s
 ******************************************************************************/
void CsrBtSapsDeactivateDisconnectInd(SapsInstanceData_t * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;
    if (prim->status)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                instData->deviceAddr,prim->btConnId);
        resetSendRecvBuffer(instData);
        unRegisterSapsService(instData);
    }
    else
    {
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_SDS_REGISTER_CFM in Deactivate_s
 ******************************************************************************/
void CsrBtSapsDeactivateSdsRegisterCfm(SapsInstanceData_t * instData)
{
    CsrBtCmSdsRegisterCfm * prim;
    prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->serviceRecHandle = prim->serviceRecHandle;
        unRegisterSapsService(instData);
    }
    else
    {
        sendSapsDeactivateCfm(instData);
        CSR_BT_SAPS_STATE_CHANGE(instData->state, Idle_s);
    }
}

/******************************************************************************
 * Handler for CSR_BT_CM_SDS_UNREGISTER_CFM in Deactivate_s
 ******************************************************************************/
void CsrBtSapsDeactivateSdsUnregisterCfm(SapsInstanceData_t * instData)
{
    CsrBtCmSdsUnregisterCfm * prim = (CsrBtCmSdsUnregisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CSR_BT_SAPS_STATE_CHANGE(instData->state, Idle_s);
        instData->sapRegistered    = FALSE;
        sendSapsDeactivateCfm(instData);
    }
    else if (instData->numUnregRetries < NAX_NUM_RETRIES)
    {
        instData->numUnregRetries++;
        unRegisterSapsService(instData);
    }
    else
    {
        /* if the servicerecord have not been unregistered after NAX_NUM_RETRIES
           give up and remember status */
        instData->numUnregRetries = 0;
        CSR_BT_SAPS_STATE_CHANGE(instData->state, Idle_s);
        instData->sapRegistered    = TRUE;
        sendSapsDeactivateCfm(instData);
    }
}

/*****************************************************************************
 *
 * Handler functions for Downstream messages
 *
 *****************************************************************************/

/******************************************************************************
 * Handler for CSR_BT_SAPS_ACTIVATE_REQ in Init_s
 ******************************************************************************/
void CsrBtSapsActivateInitState(SapsInstanceData_t * instData)
{
    CsrBtSapsActivateReq * prim;
    prim = (CsrBtSapsActivateReq *) instData->recvMsgP;
    instData->appHandle = prim->phandle;
    instData->activateSaps = TRUE;
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Init_s
 ******************************************************************************/
void CsrBtSapsDeactivateInitState(SapsInstanceData_t * instData)
{
    instData->activateSaps = FALSE;
    sendSapsDeactivateCfm(instData);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_ACTIVATE_REQ in Idle_s
 ******************************************************************************/
void CsrBtSapsActivateIdleState(SapsInstanceData_t * instData)
{
    CsrBtSapsActivateReq * prim;
    prim = (CsrBtSapsActivateReq *) instData->recvMsgP;
    instData->appHandle = prim->phandle;
    if (instData->sapRegistered == FALSE)
    {
        registerSapsService(instData);
    }
    else
    {
        sendConnectAccept(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Idle_s
 ******************************************************************************/
void CsrBtSapsDeactivateIdleState(SapsInstanceData_t * instData)
{
    CSR_BT_SAPS_STATE_CHANGE(instData->state, Deactivate_s);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Activated_s
 ******************************************************************************/
void CsrBtSapsDeactivateActivatedState(SapsInstanceData_t * instData)
{
    CsrBtCmCancelAcceptConnectReqSend(CSR_BT_SAPS_IFACEQUEUE, instData->serverChannel);
    CSR_BT_SAPS_STATE_CHANGE(instData->state, Deactivate_s);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Connected_s
 ******************************************************************************/
void CsrBtSapsDeactivateReqConnectedState(SapsInstanceData_t * instData)
{
    if (instData->connectState == ConnectResSend_s)
    {
        /* This means that a SIM connection has been established */
        sendDisconnectInd(instData, CSR_BT_IMMEDIATE_DISCONNECT);
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->sapsConnId);
    }
    CSR_BT_SAPS_STATE_CHANGE(instData->state, Deactivate_s);
}

static CsrUint8 csrBtSapsMsgInterpretError(CsrBtResultCode errorCode)
{
    static const CsrBtResultCode errorLut[] =
    {
        CSR_BT_RSLT_OK_REQUEST,
        CSR_BT_RSLT_ERR_NO_REASON,
        CSR_BT_RSLT_ERR_CARD_NOT_ACC,
        CSR_BT_RSLT_ERR_CARD_POW_OFF,
        CSR_BT_RSLT_ERR_CARD_REMOVED,
        CSR_BT_RSLT_ERR_CARD_POW_ON,
        CSR_BT_RSLT_ERR_DATA_NOT_AVAIL,
        CSR_BT_RSLT_ERR_NOT_SUPPORTED,
        CSR_BT_RSLT_ERR_INVALID_MSG
    };

    return (errorCode <= CSR_BT_RESULT_CODE_SAP_PROTOCOL_INVALID_MSG) && (errorCode >= CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS)
        ? errorLut[errorCode-CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS]
        : CSR_BT_RSLT_ERR_NO_REASON;
}

static CsrUint8 csrBtSapsMsgInterpretConnectResult(CsrBtResultCode errorCode)
{
    static const CsrUint8 errorLut[] =
    {
        CSR_BT_OK_CONNECT,
        CSR_BT_OK_ONGOING_CALL,
        CSR_BT_UNABLE_TO_ESTABLISH_CONNECT,
        CSR_BT_MAX_MSG_SIZE_NOT_SUPPORTED,
        CSR_BT_MSG_SIZE_TO_SMALL
    };

    return (errorCode <= CSR_BT_RESULT_CODE_SAP_CONNECTION_MSG_SIZE_TO_SMALL) && (errorCode >= CSR_BT_RESULT_CODE_SAP_CONNECTION_SUCCESS)
        ? errorLut[errorCode-CSR_BT_RESULT_CODE_SAP_CONNECTION_SUCCESS]
        : CSR_BT_UNABLE_TO_ESTABLISH_CONNECT;
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_CONNECT_RES in Connected_s
 ******************************************************************************/
void CsrBtSapsConnectResConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsConnectRes * prim;
    CsrUint8 * sendBuffer;
    CsrUint8 errorCode;

    prim = (CsrBtSapsConnectRes *) instData->recvMsgP;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_CONNECT_RES);

    errorCode = csrBtSapsMsgInterpretConnectResult(prim->resultCode);

    sendBuffer[0]  = CSR_BT_MSGID_CONNECT_RES;
    sendBuffer[1]  = 2;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;     /* Reserved */
    sendBuffer[3]  = 0x00;     /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_CONNECTION_STATUS;
    sendBuffer[5]  = 0x00;     /* Reserved */
    sendBuffer[6]  = 0x00;     /* Parameter length */
    sendBuffer[7]  = 0x01;     /* Parameter length */
    sendBuffer[8]  = errorCode;
    sendBuffer[9]  = 0x00;     /* Padding */
    sendBuffer[10] = 0x00;     /* Padding */
    sendBuffer[11] = 0x00;     /* Padding */
    sendBuffer[12] = CSR_BT_PARAM_ID_MAX_MSG_SIZE;
    sendBuffer[13] = 0x00;     /* Reserved */
    sendBuffer[14] = 0x00;     /* Parameter length */
    sendBuffer[15] = 0x02;     /* Parameter length */
    sendBuffer[16] = (prim->maxMsgSize & 0xFF00) >> 8;
    sendBuffer[17] = prim->maxMsgSize & 0xFF;
    sendBuffer[18] = 0x00;     /* Padding */
    sendBuffer[19] = 0x00;     /* Padding */

    if ((errorCode == CSR_BT_OK_CONNECT)|| (errorCode == CSR_BT_OK_ONGOING_CALL))
    {
        /* This means that the App has accepted the connection */
        CSR_BT_SAPS_STATE_CHANGE(instData->connectState, ConnectResSend_s);
        instData->maxMsgSize   = prim->maxMsgSize;
        instData->statusToSend = prim->cardStatus;
    }
    else if ((errorCode == CSR_BT_UNABLE_TO_ESTABLISH_CONNECT)|| (errorCode == CSR_BT_MSG_SIZE_TO_SMALL))
    {
        CSR_BT_SAPS_STATE_CHANGE(instData->connectState, ConnectDisconnect_s);
    }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
    else
    {
        CsrGeneralException("SAPS",
                            0,
                             errorCode,
                             "Unknown error code");
    }
#endif

    /* else the client will send another CONNECT_REQ */
    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_CONNECT_RES);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_DEACTIVATE_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapsDeactivateReqSapConnectedState(SapsInstanceData_t * instData)
{
    if (instData->sendBuffer == NULL)
    {
        sendDisconnectInd(instData, CSR_BT_IMMEDIATE_DISCONNECT);
        CSR_BT_SAPS_STATE_CHANGE(instData->state, Deactivate_s);
    }
    else
    {
        CsrBtSapsSavemessage(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_DISCONNECT_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapsDisconnectReqSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsDisconnectReq * prim;
    prim = (CsrBtSapsDisconnectReq *) instData->recvMsgP;

    if (instData->sendBuffer == NULL)
    {
        sendDisconnectInd(instData, prim->disconnectType);
        if (prim->disconnectType == CSR_BT_GRACEFUL_DISCONNECT)
        {
            instData->serverDisconnect = FALSE;
            /* remain in SapConnected_s and wait for a DISCONNECT_IND from client */
        }
        else
        {
            instData->serverDisconnect = TRUE;
            instData->serverDeactivated = FALSE;
            CSR_BT_SAPS_STATE_CHANGE(instData->state, Disconnecting_s);
        }
    }
    else
    {
        CsrBtSapsSavemessage(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_TRANSFER_APDU_RES in SapConnected_s
 ******************************************************************************/
void CsrBtSapsTransferApduResConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsTransferApduRes * prim;
    CsrUint8 * sendBuffer;
    CsrUint8 index;
    CsrUint8 paddingLength;
    CsrUint16 packetLength;

    prim = (CsrBtSapsTransferApduRes *) instData->recvMsgP;

    /* calculate number of padding bytes for last parameter */
    paddingLength = CSR_BT_FIND_PADDING(prim->apduResponseLength);
    packetLength = CSR_BT_LENGTH_TRANSFER_APDU_RES + prim->apduResponseLength + paddingLength;
    sendBuffer = CsrPmemAlloc(packetLength);

    sendBuffer[0]  = CSR_BT_MSGID_TRANSFER_APDU_RES;
    sendBuffer[1]  = 0x02;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;        /* Reserved */
    sendBuffer[3]  = 0x00;        /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_RESULT_CODE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = csrBtSapsMsgInterpretError(prim->resultCode);
    sendBuffer[9]  = 0x00;        /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */
    sendBuffer[12] = CSR_BT_PARAM_ID_RESPONSE_APDU;
    sendBuffer[13] = 0x00;        /* Reserved */
    sendBuffer[14] = (prim->apduResponseLength & 0xFF00) >> 8;
    sendBuffer[15] = prim->apduResponseLength & 0xFF;

    CsrMemCpy(&sendBuffer[16], prim->apduResponse, prim->apduResponseLength);
    /* Add padding to end of the message. The length of each Parameter must be a multiple of four */
    for (index = 0; index < paddingLength; index++)
    {
        sendBuffer[16 + prim->apduResponseLength + index] = 0x00;
    }
    SendDataReq(instData, sendBuffer, packetLength);
    CsrPmemFree (prim->apduResponse);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_TRANSFER_ATR_RES in SapConnected_s
 ******************************************************************************/
void CsrBtSapsTransferAtrResSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsTransferAtrRes * prim;
    CsrUint8 * sendBuffer;
    CsrUint8 index;
    CsrUint8 paddingLength;
    CsrUint16 packetLength;

    prim = (CsrBtSapsTransferAtrRes *) instData->recvMsgP;

    /* calculate number of padding bytes for last parameter */
    paddingLength = CSR_BT_FIND_PADDING(prim->atrResponseLength);
    packetLength = CSR_BT_LENGTH_TRANSFER_ATR_RES + prim->atrResponseLength + paddingLength;
    sendBuffer = CsrPmemAlloc(packetLength);

    sendBuffer[0]  = CSR_BT_MSGID_TRANSFER_ATR_RES;
    sendBuffer[1]  = 0x02;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;        /* Reserved */
    sendBuffer[3]  = 0x00;        /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_RESULT_CODE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = csrBtSapsMsgInterpretError(prim->resultCode);
    sendBuffer[9] = 0x00;         /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */
    sendBuffer[12] = CSR_BT_PARAM_ID_ATR;
    sendBuffer[13] = 0x00;        /* Reserved */
    sendBuffer[14] = (prim->atrResponseLength & 0xFF00) >> 8;
    sendBuffer[15] = prim->atrResponseLength & 0xFF;

    CsrMemCpy(&sendBuffer[16], prim->atrResponse, prim->atrResponseLength);
    /* Add padding to end of the message. The length of each Parameter must be a multiple of four */
    for (index = 0; index < paddingLength; index++)
    {
        sendBuffer[16 + prim->atrResponseLength + index] = 0x00;
    }
    SendDataReq(instData, sendBuffer, packetLength);
    CsrPmemFree (prim->atrResponse);
}

/******************************************************************************
 * Handler for CsrBtSapsPowerSimOffRes in SapConnected_s
 ******************************************************************************/
void CsrBtSapsTransferPowerSimOffResSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsPowerSimOffRes * prim;
    CsrUint8 * sendBuffer;
    prim = (CsrBtSapsPowerSimOffRes *) instData->recvMsgP;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_POWER_OFF_RES);

    sendBuffer[0]  = CSR_BT_MSGID_POWER_SIM_OFF_RES;
    sendBuffer[1]  = 0x01;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;        /* Reserved */
    sendBuffer[3]  = 0x00;        /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_RESULT_CODE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = csrBtSapsMsgInterpretError(prim->resultCode);
    sendBuffer[9]  = 0x00;        /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_POWER_OFF_RES);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_POWER_SIM_ON_RES in SapConnected_s
 ******************************************************************************/
void CsrBtSapsTransferPowerSimOnResSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsPowerSimOnRes * prim;
    CsrUint8 * sendBuffer;
    prim = (CsrBtSapsPowerSimOnRes *) instData->recvMsgP;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_POWER_ON_RES);

    sendBuffer[0]  = CSR_BT_MSGID_POWER_SIM_ON_RES;
    sendBuffer[1]  = 0x01;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;        /* Reserved */
    sendBuffer[3]  = 0x00;        /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_RESULT_CODE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = csrBtSapsMsgInterpretError(prim->resultCode);
    sendBuffer[9]  = 0x00;        /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_POWER_ON_RES);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_RESET_SIM_RES in SapConnected_s
 ******************************************************************************/
void CsrBtSapsTransferResetSimResSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsResetSimRes * prim;
    CsrUint8 * sendBuffer;
    prim = (CsrBtSapsResetSimRes *) instData->recvMsgP;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_RESET_SIM_RES);

    sendBuffer[0]  = CSR_BT_MSGID_RESET_SIM_RES;
    sendBuffer[1]  = 0x01;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;        /* Reserved */
    sendBuffer[3]  = 0x00;        /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_RESULT_CODE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = csrBtSapsMsgInterpretError(prim->resultCode);
    sendBuffer[9]  = 0x00;        /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_RESET_SIM_RES);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES in SapConnected_s
 ******************************************************************************/
void CsrBtSapsTransferCardStatusSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsTransferCardReaderStatusRes * prim;
    CsrUint8 * sendBuffer;
    prim = (CsrBtSapsTransferCardReaderStatusRes *) instData->recvMsgP;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_TRANSFER_CARD_READER_STATUS_RES);

    sendBuffer[0]  = CSR_BT_MSGID_TRANSFER_CARD_READER_STATUS_RES;
    sendBuffer[1]  = 0x02;        /* Number of Parameters */
    sendBuffer[2]  = 0x00;        /* Reserved */
    sendBuffer[3]  = 0x00;        /* Reserved */
    sendBuffer[4]  = CSR_BT_PARAM_ID_RESULT_CODE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = csrBtSapsMsgInterpretError(prim->resultCode);
    sendBuffer[9]  = 0x00;        /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */
    sendBuffer[12] = CSR_BT_PARAM_ID_CARD_READER_STATUS;
    sendBuffer[13] = 0x00;        /* Reserved */
    sendBuffer[14] = 0x00;        /* Parameter length */
    sendBuffer[15] = 0x01;        /* Parameter length */
    sendBuffer[16] = prim->cardReaderStatus;
    sendBuffer[17] = 0x00;        /* Padding */
    sendBuffer[18] = 0x00;        /* Padding */
    sendBuffer[19] = 0x00;        /* Padding */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_TRANSFER_CARD_READER_STATUS_RES);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES in SapConnected_s
 ******************************************************************************/
void CsrBtSapsSetTransferProtocolSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsSetTransferProtocolRes * prim;
    CsrUint8 * sendBuffer;
    prim = (CsrBtSapsSetTransferProtocolRes *) instData->recvMsgP;
    sendBuffer = CsrPmemAlloc(CSR_BT_LENGTH_SET_TRANSFER_PROTOCOL_RES);

    sendBuffer[0]  = CSR_BT_MSGID_SET_TRANSFER_PROTOCOL_RES;
    sendBuffer[1]  = 0x01;
    sendBuffer[2]  = 0x00;
    sendBuffer[3]  = 0x00;
    sendBuffer[4]  = CSR_BT_PARAM_ID_RESULT_CODE;
    sendBuffer[5]  = 0x00;        /* Reserved */
    sendBuffer[6]  = 0x00;        /* Parameter length */
    sendBuffer[7]  = 0x01;        /* Parameter length */
    sendBuffer[8]  = csrBtSapsMsgInterpretError(prim->resultCode);
    sendBuffer[9]  = 0x00;        /* Padding */
    sendBuffer[10] = 0x00;        /* Padding */
    sendBuffer[11] = 0x00;        /* Padding */

    SendDataReq(instData, sendBuffer, CSR_BT_LENGTH_SET_TRANSFER_PROTOCOL_RES);
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_SEND_STATUS_REQ in SapConnected_s
 ******************************************************************************/
void CsrBtSapsSendStatusReqSapConnectedState(SapsInstanceData_t * instData)
{
    CsrBtSapsSendStatusReq * prim;
    prim = (CsrBtSapsSendStatusReq *) instData->recvMsgP;

    if (instData->sendBuffer == NULL)
    {
        sendStatusInd(instData, prim->statusChange);
    }
    else
    {
        CsrBtSapsSavemessage(instData);
    }
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Disconnecting_s
 ******************************************************************************/
void CsrBtSapsDeactivateReqDisconnectingState(SapsInstanceData_t * instData)
{
    instData->serverDeactivated = TRUE;
}

/******************************************************************************
 * Handler for CSR_BT_SAPS_CONNECT_RES in Disconnecting_s
 ******************************************************************************/
void CsrBtSapsConnectResDisconnectingState(SapsInstanceData_t * instData)
{
    /* The CSR_BT_SAPS_CONNECT_RES is received in Disconnecting_s when a
     * CSR_BT_CM_DISCONNECT_IND is received after the CSR_BT_SAPS_CONNECT_IND is
     * send to the application during the connection setup. */
    sendConnectAccept(instData);
    CSR_BT_SAPS_STATE_CHANGE(instData->state, Activated_s);
    sendSapsDisconnectInd(instData, instData->discReason, CSR_BT_SUPPLIER_CM);
}

/******************************************************************************
 * Handler and confirm-sender for security level changes
 ******************************************************************************/
void CsrBtSapsSecurityInReqHandler(SapsInstanceData_t * instData)
{
    CsrBtResultCode rval;
    CsrBtSapsSecurityInReq *prim;

    prim = (CsrBtSapsSecurityInReq*)instData->recvMsgP;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_SIM_ACCESS_MANDATORY_SECURITY_INCOMING,
        CSR_BT_SIM_ACCESS_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_SAP_SUCCESS,
        CSR_BT_RESULT_CODE_SAP_UNACCEPTABLE_PARAMETER);

    CsrBtSapsSecurityInCfmSend(prim->appHandle,
        rval, CSR_BT_SUPPLIER_SAPS);

}

void CsrBtSapsSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSapsSecurityInCfm *prim;
    prim = (CsrBtSapsSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtSapsSecurityInCfm));
    prim->type = CSR_BT_SAPS_SECURITY_IN_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    csrBtSapsMessagePut(appHandle, prim);
}

