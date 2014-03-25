/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_dunc_main.h"
#include "csr_bt_dunc_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_dunc_sef.h"

void CsrBtDuncMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_DUNC_PRIM, msg);
}

static void csrBtDuncResetClearInstData(DuncInstanceDataType * instData)
{
    instData->state         = DUNC_IDLE_STATE;
    instData->ctrlHandle    = CSR_SCHED_QID_INVALID;
    instData->doingCleanup  = FALSE;
}

static CsrUint8 csrBtDuncMapSendingControlSignal(CsrUint8 theModemStatus)
{
    CsrUint8 mapModemSignal = 0;

    if(theModemStatus & CSR_BT_MODEM_RTS_MASK)
    {
        mapModemSignal |= RFC_MSC_RTR_BIT;
    }
    else
    {
        mapModemSignal &= ~RFC_MSC_RTR_BIT;
    }

    if(theModemStatus & CSR_BT_MODEM_DTR_MASK)
    {
        mapModemSignal |= RFC_MSC_RTC_BIT;
    }
    else
    {
        mapModemSignal &= ~RFC_MSC_RTC_BIT;
    }

    mapModemSignal &= ~RFC_MSC_IC_BIT;
    mapModemSignal |= RFC_MSC_DV_BIT;

    return mapModemSignal;
}

static CsrUint8 csrBtDuncMapReceivedControlSignal(CsrUint8 theModemStatus)
{
    CsrUint8 mapModemSignal = 0;

    if(theModemStatus & RFC_MSC_RTC_BIT)
    {
        mapModemSignal |= CSR_BT_MODEM_DSR_MASK;
    }
    else
    {
        mapModemSignal &= ~CSR_BT_MODEM_DSR_MASK;
    }

    if(theModemStatus & RFC_MSC_RTR_BIT)
    {
        mapModemSignal |= CSR_BT_MODEM_CTS_MASK;
    }
    else
    {
        mapModemSignal &= ~CSR_BT_MODEM_CTS_MASK;
    }

    if(theModemStatus & RFC_MSC_IC_BIT)
    {
        mapModemSignal |= CSR_BT_MODEM_RI_MASK;
    }
    else
    {
        mapModemSignal &= ~CSR_BT_MODEM_RI_MASK;
    }

    if(theModemStatus & RFC_MSC_DV_BIT)
    {
        mapModemSignal |= CSR_BT_MODEM_DCD_MASK;
    }
    else
    {
        mapModemSignal &= ~CSR_BT_MODEM_DCD_MASK;
    }

    return mapModemSignal;
}

static void csrBtDuncSendRegisterDataPathCfm(DuncInstanceDataType * instData)
{
    CsrBtDuncRegisterDataPathHandleCfm * cfm;

    cfm                 = (CsrBtDuncRegisterDataPathHandleCfm *) CsrPmemAlloc(sizeof(CsrBtDuncRegisterDataPathHandleCfm));
    cfm->type           = CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM;
    cfm->duncInstanceId = CSR_BT_DUNC_IFACEQUEUE;
    cfm->resultCode     = CSR_BT_RESULT_CODE_DUNC_SUCCESS;
    cfm->resultSupplier = CSR_BT_SUPPLIER_DUNC;
    CsrBtDuncMessagePut(FIND_DATA_PATH_QUEUE(instData), cfm);
}

static void csrBtDuncSendConnectCfm(DuncInstanceDataType * instData,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    if (instData->ctrlHandle != CSR_SCHED_QID_INVALID)
    {
        CsrBtDuncConnectCfm * prim = (CsrBtDuncConnectCfm *) CsrPmemAlloc(sizeof (CsrBtDuncConnectCfm));

        prim->type = CSR_BT_DUNC_CONNECT_CFM;
        prim->resultCode = resultCode;
        prim->resultSupplier = resultSupplier;
        prim->maxMsgSize = instData->rfcMaxPacketSize;
        prim->duncInstanceId = CSR_BT_DUNC_IFACEQUEUE;
        prim->btConnId = instData->btConnId;

        CsrBtDuncMessagePut(instData->ctrlHandle, prim);
    }
}

static void sendCsrBtDuncStatusInd(DuncInstanceDataType * instData, CsrBool connected)
{ /* Only send to data handle which is valid and not equal to control handle */
    if((instData->dataHandle != CSR_SCHED_QID_INVALID) && (instData->dataHandle != instData->ctrlHandle))
    {
        CsrBtDuncStatusInd * prim = (CsrBtDuncStatusInd *) CsrPmemAlloc(sizeof (CsrBtDuncStatusInd));

        prim->type           = CSR_BT_DUNC_STATUS_IND;
        prim->duncInstanceId = CSR_BT_DUNC_IFACEQUEUE;
        prim->connected      = connected;

        if (connected)
        {
            prim->deviceAddr = instData->bdAddr;
            prim->maxMsgSize = instData->rfcMaxPacketSize;
        }
        else
        {
            bd_addr_zero(&prim->deviceAddr);
            prim->maxMsgSize = 0;
        }
        CsrBtDuncMessagePut(instData->dataHandle, prim);
    }
}

static void cleanupDone(DuncInstanceDataType * instData)
{
    CsrBtDuncCleanupQueue();
    CsrBtDuncInitInstanceData(instData);
}

static void csrBtDuncSendDisconnectInd(DuncInstanceDataType * instData)
{
    if (instData->ctrlHandle != CSR_SCHED_QID_INVALID)
    {
        CsrBtCmDisconnectInd * cmPrim;
        CsrBtDuncDisconnectInd * prim;
        cmPrim = (CsrBtCmDisconnectInd *) instData->recvMsgP;
        prim = (CsrBtDuncDisconnectInd *)CsrPmemAlloc(sizeof (CsrBtDuncDisconnectInd));

        prim->type = CSR_BT_DUNC_DISCONNECT_IND;
        prim->duncInstanceId = CSR_BT_DUNC_IFACEQUEUE;
        prim->reasonCode = cmPrim->reasonCode;
        prim->reasonSupplier = cmPrim->reasonSupplier;
        prim->localTerminated = cmPrim->localTerminated;

        CsrBtDuncMessagePut(instData->ctrlHandle, prim);
    }
}

static void csrBtDuncSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtDuncSecurityOutCfm *prim;
    prim = (CsrBtDuncSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtDuncSecurityOutCfm));
    prim->type = CSR_BT_DUNC_SECURITY_OUT_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtDuncMessagePut(appHandle, prim);
}

static void csrBtDuncPortnegIndSend(DuncInstanceDataType * instData,
                               RFC_PORTNEG_VALUES_T portPar, CsrBool request)
{
    CsrBtDuncPortnegInd * duncPrim;

    duncPrim                    = (CsrBtDuncPortnegInd *) CsrPmemAlloc(sizeof(CsrBtDuncPortnegInd));
    duncPrim->type              = CSR_BT_DUNC_PORTNEG_IND;
    duncPrim->duncInstanceId    = CSR_BT_DUNC_IFACEQUEUE;
    duncPrim->portPar           = portPar;
    duncPrim->request           = request;
    CsrBtDuncMessagePut(FIND_DATA_PATH_QUEUE(instData), duncPrim);
}

void CsrBtDuncSendDataPathStatusInd(DuncInstanceDataType * instData, CsrUint8 status, CsrSchedQid handle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    if (instData->ctrlHandle != CSR_SCHED_QID_INVALID)
    {
        CsrBtDuncDataPathStatusInd * prim = (CsrBtDuncDataPathStatusInd *) CsrPmemAlloc(sizeof(CsrBtDuncDataPathStatusInd));
        prim->type            = CSR_BT_DUNC_DATA_PATH_STATUS_IND;
        prim->duncInstanceId  = handle;
        prim->status          = status;
        prim->resultCode      = resultCode;
        prim->resultSupplier  = resultSupplier;

        CsrBtDuncMessagePut(instData->ctrlHandle, prim);
    }
    else
    {
        ;
    }
}

void CsrBtDuncConnectResultHandler(void *instData,
    CsrUint8           localServerCh,
    CsrUint32               btConnId,
    CsrBtDeviceAddr         deviceAddr,
    CsrUint16                maxFrameSize,
    CsrBool                  validPortPar,
    RFC_PORTNEG_VALUES_T    portPar,
    CsrBtResultCode         resultCode,
    CsrBtSupplier     resultSupplier,
    CmnCsrBtLinkedListStruct * sdpTag)
{
    DuncInstanceDataType * inst = instData;

    inst->btConnId          = btConnId;
    inst->rfcMaxPacketSize  = maxFrameSize;
    inst->serverChannel     = localServerCh;
    inst->bdAddr            = deviceAddr;

    if (inst->doingCleanup == TRUE)
    {
        sendCsrBtDuncStatusInd(inst, FALSE);

        if (resultSupplier == CSR_BT_SUPPLIER_CM &&
            resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,deviceAddr,btConnId);
            inst->state     = DUNC_DISCONNECTING_STATE;
            CsrBtCmDisconnectReqSend(inst->btConnId);

        }
        else
        {
            cleanupDone(inst);
        }
    }
    else
    {
        if (resultSupplier == CSR_BT_SUPPLIER_CM &&
            resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,deviceAddr,btConnId);
            csrBtDuncSendConnectCfm(inst, CSR_BT_RESULT_CODE_DUNC_SUCCESS,
                CSR_BT_SUPPLIER_DUNC);
        }
        else
        {
            csrBtDuncSendConnectCfm(inst, resultCode, resultSupplier);
        }

        if (resultSupplier == CSR_BT_SUPPLIER_CM &&
            resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            inst->state             = DUNC_CONNECTED_STATE;

            if( inst->lowPower == TRUE)
            {
                CsrBtCmRfcModeChangeReqSend(btConnId, CSR_BT_LINK_STATUS_SNIFF);
            }
            /* Status indication for connect must be sent after the connect indication */
            sendCsrBtDuncStatusInd(inst, TRUE);
        }
        else
        {
            csrBtDuncResetClearInstData(inst);
        }
    }
    CsrBtUtilBllFreeLinkedList(&sdpTag, CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(validPortPar);
    CSR_UNUSED(portPar);
}


void CsrBtDuncPortNegIndHandler(void                     * instData,
                           CsrBtDeviceAddr             deviceAddr,
                           CsrUint8            serverChannel,
                           RFC_PORTNEG_VALUES_T     portPar,
                           CsrBool                   request)
{
    csrBtDuncPortnegIndSend(instData, portPar, request);
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(serverChannel);
}

void CsrBtDuncIdleStateConnectReqHandler(DuncInstanceDataType * instData)
{
    CsrUint16                index;
    CmnCsrBtLinkedListStruct * sdpTag = NULL;
    CsrBtDuncConnectReq          * prim;
    CsrUint8        myModemStatus = 0;
    
    prim = (CsrBtDuncConnectReq *) instData->recvMsgP;

    instData->ctrlHandle = prim->ctrlHandle;
    instData->lowPower   = prim->lowPowerSupport;
    instData->state      = DUNC_CONNECT_STATE;

    sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_DUN_PROFILE_UUID, &index);

    CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);
    
    myModemStatus = CSR_BT_MODEM_SEND_CTRL_DTE_DEFAULT;

    CsrBtUtilRfcConStart(instData, instData->connectInst, sdpTag, prim->bdAddr,
                                        instData->secOutgoing, FALSE, NULL,
                                        CSR_BT_DUNC_PROFILE_DEFAULT_MTU_SIZE,
                                        myModemStatus,
                                        0);
}

void CsrBtDuncConnectStateCancelConnectReqHandler(DuncInstanceDataType * instData)
{
    CsrBtUtilRfcConCancel(instData, instData->connectInst);
}

void CsrBtDuncConnectStatePortnegResHandler(DuncInstanceDataType * instData)
{
    CsrBtDuncPortnegRes * prim;

    prim = (CsrBtDuncPortnegRes *) instData->recvMsgP;

    CsrBtUtilRfcConSetPortPar(instData->connectInst, prim->portPar);
}

void CsrBtDuncConnectedStateDisconnectReqHandler(DuncInstanceDataType * instData)
{/* Send status indication before the disconnect */
    sendCsrBtDuncStatusInd(instData, FALSE);

    CsrBtCmDisconnectReqSend(instData->btConnId);
    instData->state = DUNC_DISCONNECTING_STATE;
}

void CsrBtDuncConnectedStateDataReqHandler(DuncInstanceDataType * instData)
{
    CsrBtDuncDataReq * prim = (CsrBtDuncDataReq *) instData->recvMsgP;

    CsrBtCmDataReqSend(instData->btConnId, prim->dataLength, prim->data);
}

void CsrBtDuncConnectedStateDataResHandler(DuncInstanceDataType * instData)
{
    CsrBtCmDataResSend(instData->btConnId);
}

void CsrBtDuncConnectedStateControlReqHandler(DuncInstanceDataType * instData)
{
    CsrUint8        myModemStatus = 0;
    CsrBtDuncControlReq * prim        = (CsrBtDuncControlReq *) instData->recvMsgP;

    myModemStatus = csrBtDuncMapSendingControlSignal(prim->modemStatus);
    CsrBtCmControlReqSend(instData->btConnId, myModemStatus, prim->breakSignal);
}

void CsrBtDuncXstateRegisterDataPathHandleReqHandler(DuncInstanceDataType *instData)
{
    CsrBtDuncRegisterDataPathHandleReq * prim = (CsrBtDuncRegisterDataPathHandleReq *) instData->recvMsgP;
    instData->dataHandle = prim->dataAppHandle;
    csrBtDuncSendRegisterDataPathCfm(instData);
}

void CsrBtDuncXStateChangeDataPathStatusReqHandler(DuncInstanceDataType *instData)
{
    CsrBtDuncChangeDataPathStatusReq * prim = (CsrBtDuncChangeDataPathStatusReq *) instData->recvMsgP;

    CsrBtDuncSendDataPathStatusInd(instData, prim->status, CSR_BT_DUNC_IFACEQUEUE,
        CSR_BT_RESULT_CODE_DUNC_SUCCESS, CSR_BT_SUPPLIER_DUNC);
}

void CsrBtDuncConnectedStatePortnegResHandler(DuncInstanceDataType * instData)
{
    CsrBtDuncPortnegRes * prim;

    prim = (CsrBtDuncPortnegRes *) instData->recvMsgP;
    CsrBtCmPortnegResSend(instData->btConnId, &prim->portPar);
}


void CsrBtDuncConnectedStatePortnegReqHandler(DuncInstanceDataType *instData)
{
    CsrBtDuncPortnegReq *prim = (CsrBtDuncPortnegReq *)instData->recvMsgP;

    CsrBtCmPortnegReqSend(instData->btConnId, &prim->portPar, 0);
}

void CsrBtDuncConnectedStateCmDataIndHandler(DuncInstanceDataType * instData)
{
    CsrBtCmDataInd * prim   = (CsrBtCmDataInd *) instData->recvMsgP;
    CsrBtDuncDataInd * duncPrim = (CsrBtDuncDataInd *) CsrPmemAlloc(sizeof(CsrBtDuncDataInd));

    duncPrim->type           = CSR_BT_DUNC_DATA_IND;
    duncPrim->dataLength     = prim->payloadLength;
    duncPrim->data           = prim->payload;
    duncPrim->duncInstanceId = CSR_BT_DUNC_IFACEQUEUE;
    CsrBtDuncMessagePut(FIND_DATA_PATH_QUEUE(instData), duncPrim);
}

void CsrBtDuncConnectedStateCmDataCfmHandler(DuncInstanceDataType * instData)
{
    CsrBtDuncDataCfm * prim = (CsrBtDuncDataCfm *)CsrPmemAlloc(sizeof (CsrBtDuncDataCfm));

    prim->type           = CSR_BT_DUNC_DATA_CFM;
    prim->duncInstanceId = CSR_BT_DUNC_IFACEQUEUE;
    CsrBtDuncMessagePut(FIND_DATA_PATH_QUEUE(instData), prim);
}

void CsrBtDuncConnectedStateCmControlIndHandler(DuncInstanceDataType * instData)
{
    CsrBtCmControlInd * cmPrim;
    CsrBtDuncControlInd * duncPrim;

    cmPrim = (CsrBtCmControlInd *) instData->recvMsgP;
    duncPrim = (CsrBtDuncControlInd *)CsrPmemAlloc(sizeof(CsrBtDuncControlInd));
    duncPrim->type = CSR_BT_DUNC_CONTROL_IND;
    duncPrim->duncInstanceId = CSR_BT_DUNC_IFACEQUEUE;
    duncPrim->modemStatus = csrBtDuncMapReceivedControlSignal(cmPrim->modemstatus);
    duncPrim->breakSignal = cmPrim->break_signal;
    CsrBtDuncMessagePut(FIND_DATA_PATH_QUEUE(instData), duncPrim);
}

void CsrBtDuncConnectedStateCmDisconnectIndHandler(DuncInstanceDataType * instData)
{
    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,instData->bdAddr,instData->btConnId);

    CsrBtCmUnRegisterReqSend(instData->serverChannel);
    csrBtDuncSendDisconnectInd(instData);
    sendCsrBtDuncStatusInd(instData, FALSE);
    csrBtDuncResetClearInstData(instData);

}

void CsrBtDuncDisconnectingStateCmDisconnectIndHandler(DuncInstanceDataType * instData)
{
    CsrBtCmDisconnectInd * prim;
    prim = (CsrBtCmDisconnectInd *) instData->recvMsgP;


    if (prim->status == TRUE)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,instData->bdAddr,instData->btConnId);
        CsrBtCmUnRegisterReqSend(instData->serverChannel);

        if(instData->doingCleanup == TRUE)
        {
            cleanupDone(instData);
        }
        else
        {
            csrBtDuncSendDisconnectInd(instData);
            csrBtDuncResetClearInstData(instData);
        }
    }
    else
    {
        CsrBtCmDisconnectReqSend(instData->btConnId);
    }
}

void CsrBtDuncConnectedStateCmPortNegIndHandler(DuncInstanceDataType * instData)
{
    CsrBtCmPortnegInd * prim;

    prim = (CsrBtCmPortnegInd *) instData->recvMsgP;
    csrBtDuncPortnegIndSend(instData, prim->portPar, prim->request);
}


void CsrBtDuncConnectedStateCmPortNegCfmHandler(DuncInstanceDataType *instData)
{
    CsrBtDuncPortnegCfm *cfmPrim = (CsrBtDuncPortnegCfm *)CsrPmemAlloc(sizeof(CsrBtDuncPortnegCfm));
    CsrBtCmPortnegCfm *cmPrim = (CsrBtCmPortnegCfm *)instData->recvMsgP;

    cfmPrim->type            = CSR_BT_DUNC_PORTNEG_CFM;
    cfmPrim->portPar         = cmPrim->portPar;
    cfmPrim->duncInstanceId  = CSR_BT_DUNC_IFACEQUEUE;


    CsrBtDuncMessagePut(FIND_DATA_PATH_QUEUE(instData),cfmPrim);

}

#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtDuncConnectedStateCmModeChangeIndHandler(DuncInstanceDataType * instData)
{
    CsrBtCmRfcModeChangeInd * prim;
    prim = (CsrBtCmRfcModeChangeInd *) instData->recvMsgP;

    if (prim->resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if( prim->mode == CSR_BT_PARK_MODE )
        {
            if (instData->lowPower == TRUE)
            {
                CsrBtCmRfcModeChangeReqSend(instData->btConnId, CSR_BT_SNIFF_MODE);
            }
            else
            {
                CsrBtCmRfcModeChangeReqSend(instData->btConnId, CSR_BT_ACTIVE_MODE);
            }
        }
    }
}
#endif

void CsrBtDuncXStateIgnoreCmMsgHandler(DuncInstanceDataType * instData)
{
    CsrBtDuncReleaseMessage(CSR_BT_CM_PRIM, instData->recvMsgP);
}

void CsrBtDuncIgnoreMsg(DuncInstanceDataType * instData)
{
    CsrBtDuncReleaseMessage(CSR_BT_DUNC_PRIM, instData->recvMsgP);
}

void CsrBtDuncXSecurityOutReqHandler(DuncInstanceDataType *instData)
{
    CsrBtDuncSecurityOutReq  *prim;
    CsrBtResultCode rval;

    prim = (CsrBtDuncSecurityOutReq*)instData->recvMsgP;

    rval = CsrBtScSetSecOutLevel(&instData->secOutgoing, prim->secLevel,
        CSR_BT_DUN_CLIENT_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_DUN_CLIENT_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_DUNC_SUCCESS,
        CSR_BT_RESULT_CODE_DUNC_UNACCEPTABLE_PARAMETER);

    csrBtDuncSecurityOutCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_DUNC);
}

void CsrBtDuncReleaseMessage(CsrUint16 msgType, void * msgData)
{
    switch (msgType)
    {
        case CSR_BT_CM_PRIM:
        {
            CsrBtCmFreeUpstreamMessageContents(msgType, msgData);
            break;
        }
        case CSR_BT_DUNC_PRIM:
        {
            CsrBtDuncFreeDownstreamMessageContents(msgType, msgData);
            break;
        }
        default:
        {
            break;
        }
    }
}

void CsrBtDuncInitInstanceData(DuncInstanceDataType * instData)
{
    csrBtDuncResetClearInstData(instData);
    instData->recvMsgP              = NULL;
    instData->dataHandle            = CSR_SCHED_QID_INVALID;

    CsrBtScSetSecOutLevel(&instData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_DUN_CLIENT_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_DUN_CLIENT_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_DUNC_SUCCESS,
        CSR_BT_RESULT_CODE_DUNC_UNACCEPTABLE_PARAMETER);
}

void CsrBtDuncCleanupQueue()
{
    CsrUint16 msgType=0;
    void * msgData=NULL;
    CsrBool    msgFound;

    do
    {
        msgFound = CsrSchedMessageGet(&msgType, &msgData);
        if (msgFound)
        {
            CsrBtDuncReleaseMessage(msgType, msgData);
            CsrPmemFree(msgData);
        }

    } while(msgFound);
}
