/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

/******************************************************************************
dgDataEchoSend
******************************************************************************/
void dgDataEchoSend(atInstanceData_t *atData)
{
    CsrBtDgDataInd    *dgRevPrim;
    CsrBtDgDataReq    *dgPrim;
    CsrUint16 valueLen;
    CsrUint8 * dataPtr;
    int i;

    dgRevPrim    = (CsrBtDgDataInd *) atData->atMsgP;
    valueLen    = dgRevPrim->payloadLength;
    dataPtr        = CsrPmemAlloc(valueLen);

    for(i = 0; i < valueLen; i++)
    {
        dataPtr[i] = dgRevPrim->payload[i];
    }

    dgPrim    = (CsrBtDgDataReq *)CsrPmemAlloc(sizeof(CsrBtDgDataReq));
    dgPrim->type          = CSR_BT_DG_DATA_REQ;
    dgPrim->payload       = dataPtr;
    dgPrim->payloadLength = valueLen;
    dgPrim->btConnId      = dgRevPrim->btConnId;

    if(atData->atCfmFlag == NO_CFM)
    {
        CsrMessageQueuePush(&atData->cmDataReqQueue, CSR_BT_DG_PRIM, dgPrim);
    }
    else
    {
        CsrSchedMessagePut(CSR_BT_DG_IFACEQUEUE, CSR_BT_DG_PRIM, dgPrim);
        atData->atCfmFlag = NO_CFM;
    }
}

/******************************************************************************
dgActivateReqHandler
******************************************************************************/
void dgActivateReqHandler(atInstanceData_t *atData)
{
    CsrBtAtDgActivateReq *atPrim;
    atPrim                = (CsrBtAtDgActivateReq *) atData->atMsgP;

    atData->appHandle               = atPrim->phandle;
    atData->state                   = atCmd_s;
    atData->reachPoint              = CONTINUATION_AT_INTERPRET_FUNC;
    atData->terminatorFlag          = TERMINATORFOUND;
    atData->responseFlag            = NOWAITINGFORRESPONSE;
    atData->counter                 = EMPTY;
    atData->tempValue               = EMPTY;
    atData->numberOfTokens          = EMPTY;
    atData->numberOfRespon          = EMPTY;
    atData->dialStringCounter       = EMPTY;
    atData->atCfmFlag               = CFM_RECEIVE;
    atData->appResFlag              = APP_RES;
    atData->changeSettings          = EMPTY;
    atData->paramSettings.eValue    = 1;
    atData->paramSettings.qValue    = 0;
    atData->paramSettings.s3Value   = 13;
    atData->paramSettings.s4Value   = 10;
    atData->paramSettings.s5Value   = 8;
    atData->paramSettings.vValue    = 1;
    atData->paramSettings.andDValue = 2;

    CsrBtDgActivateReqSend(atData->CsrSchedQid, atPrim->timeout);
}



/******************************************************************************
dgDeactivateReqHandler
******************************************************************************/
void dgDeactivateReqHandler(atInstanceData_t *atData)
{
    CSR_UNUSED(atData);

    CsrBtDgDeactivateReqSend();
}



/*****************************************************************************
dgDisconnectReqHandler
******************************************************************************/
void dgDisconnectReqHandler(atInstanceData_t *atData)
{
    CsrBtAtDgDisconnectReq *atPrim;
    CsrUint16               eventClass;
    CsrBtDgDataReq *           msg;
    void *popMsg;

    atPrim                = (CsrBtAtDgDisconnectReq *) atData->atMsgP;
    atData->state        = atCmd_s;
    CsrBtDgDisconnectReqSend(atPrim->btConnId);
    while((CsrMessageQueuePop(&atData->cmDataReqQueue, &eventClass, &popMsg)))
    {
        msg = (CsrBtDgDataReq *)popMsg;
        CsrPmemFree(msg->payload);
        CsrPmemFree(msg);
    }
}


/*****************************************************************************
dgDataReqHandler
******************************************************************************/
void dgDataReqHandler(atInstanceData_t *atData)
{
    CsrBtAtDgDataReq *atPrim;
    atPrim                = (CsrBtAtDgDataReq *) atData->atMsgP;
    CsrBtDgDataReqSend(atPrim->btConnId, atPrim->payloadLength, atPrim->payload);
}


/*****************************************************************************
atDgDataReqHandler
******************************************************************************/
void atDgDataReqHandler(CsrBtConnId btConnId,
                        CsrUint16 payloadLength, CsrUint8 *payload, atInstanceData_t *atData)
{
    CsrBtDgDataReq *dgPrim;
    dgPrim                = (CsrBtDgDataReq *)CsrPmemAlloc(sizeof(CsrBtDgDataReq));
    dgPrim->type          = CSR_BT_DG_DATA_REQ;
    dgPrim->payload       = payload;
    dgPrim->payloadLength = payloadLength;
    dgPrim->btConnId      = btConnId;

    if(atData->atCfmFlag == NO_CFM)
    {
        CsrMessageQueuePush(&atData->cmDataReqQueue, CSR_BT_DG_PRIM, dgPrim);
    }
    else
    {
        CsrSchedMessagePut(CSR_BT_DG_IFACEQUEUE, CSR_BT_DG_PRIM, dgPrim);
        atData->atCfmFlag = NO_CFM;
    }
}



/*****************************************************************************
atDgControlReqHandler
******************************************************************************/
void atDgControlReqHandler(atInstanceData_t *atData)
{
    CsrBtAtDgControlReq *atPrim;
    atPrim    = (CsrBtAtDgControlReq *) atData->atMsgP;

    CsrBtDgControlReqSend(atPrim->btConnId, atPrim->modemstatus, atPrim->break_signal);
    if(((atPrim->modemstatus & CSR_BT_MODEM_DCD_MASK) == 0) && (atData->state == atOnlineData_s))
    {
        atResultCodeNoCarrierHandler(atData);
    }
}

/*****************************************************************************
atDgPortnegResHandler
******************************************************************************/
void atDgPortnegResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgPortnegRes *atPrim;

    atPrim    = (CsrBtAtDgPortnegRes *) atData->atMsgP;

    CsrBtDgPortnegResSend(atPrim->btConnId, &(atPrim->portPar) );
}


/*****************************************************************************
atDgControlIndMsgSend
******************************************************************************/
void atDgControlIndMsgSend(CsrSchedQid theAppHandle, CsrBtConnId btConnId, CsrUint8 theModemStatus)
{
    CsrBtAtDgControlInd        *atPrim;
    atPrim    = (CsrBtAtDgControlInd *)CsrPmemAlloc(sizeof(CsrBtAtDgControlInd));

    atPrim->type            = CSR_BT_AT_DG_CONTROL_IND;
    atPrim->btConnId        = btConnId;
    atPrim->modemstatus     = theModemStatus;
    CsrBtAtMessagePut(theAppHandle, atPrim);
}

/*****************************************************************************
atDgPortnegIndMsgSend
******************************************************************************/
void atDgPortnegIndMsgSend(CsrSchedQid theAppHandle, CsrBtConnId btConnId, RFC_PORTNEG_VALUES_T portPar, CsrBool request)
{
    CsrBtAtDgPortnegInd *atPrim;
    atPrim = (CsrBtAtDgPortnegInd *)CsrPmemAlloc(sizeof(CsrBtAtDgPortnegInd));

    atPrim->type              = CSR_BT_AT_DG_PORTNEG_IND;
    atPrim->btConnId          = btConnId;
    atPrim->portPar           = portPar;
    atPrim->request           = request;
    CsrBtAtMessagePut(theAppHandle, atPrim);
}

/*****************************************************************************
atDgControlIndMsgSend
******************************************************************************/
void atDgDisconnectIndMsgSend(CsrSchedQid theAppHandle,
                              CsrBtConnId btConnId,
                              CsrBool localTerminated,
                              CsrBtReasonCode reasonCode,
                              CsrBtSupplier reasonSupplier)
{
    CsrBtAtDgDisconnectInd    *atPrim;
    atPrim    = (CsrBtAtDgDisconnectInd *)CsrPmemAlloc(sizeof(CsrBtAtDgDisconnectInd));

    atPrim->type             = CSR_BT_AT_DG_DISCONNECT_IND;
    atPrim->btConnId         = btConnId;
    atPrim->localTerminated  = localTerminated;
    atPrim->reasonCode       = reasonCode;
    atPrim->reasonSupplier   = reasonSupplier;

    CsrBtAtMessagePut(theAppHandle, atPrim);
}

#endif

