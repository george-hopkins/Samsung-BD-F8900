/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_at_handler.h"

/*************************************************************************************
atDgControlIndHandler
************************************************************************************/
void atDgControlIndHandler(atInstanceData_t *atData)
{
    CsrBtDgControlInd        *dgPrim;
    dgPrim    = (CsrBtDgControlInd *) atData->atMsgP;

    atDgControlIndMsgSend(atData->appHandle, dgPrim->btConnId, dgPrim->modemstatus);
    atStateChangeHandler(atData, dgPrim->modemstatus);
}

/*************************************************************************************
atDgPortnegIndHandler
************************************************************************************/
void atDgPortnegIndHandler(atInstanceData_t *atData)
{
    CsrBtDgPortnegInd     *dgPrim;

    dgPrim = (CsrBtDgPortnegInd *) atData->atMsgP;

    atDgPortnegIndMsgSend(atData->appHandle, dgPrim->btConnId, dgPrim->portPar, dgPrim->request);
}


/*************************************************************************************
atDgDisconnectIndHandler
************************************************************************************/
void atDgDisconnectIndHandler(atInstanceData_t *atData)
{
    CsrBtDgDisconnectInd        *dgPrim;
    dgPrim    = (CsrBtDgDisconnectInd *) atData->atMsgP;

    atDgDisconnectIndMsgSend(atData->appHandle, dgPrim->btConnId,
                    dgPrim->localTerminated, dgPrim->reasonCode, dgPrim->reasonSupplier);

    atDgDisconnectCleanUp(atData);
}


/*************************************************************************************
atDgConnectIndHandler
************************************************************************************/
void atDgConnectIndHandler(atInstanceData_t *atData)
{
    CsrBtDgConnectInd    *dgPrim;
    CsrBtAtDgConnectInd    *atPrim;

    dgPrim    = (CsrBtDgConnectInd *) atData->atMsgP;
    atPrim    = (CsrBtAtDgConnectInd *)CsrPmemAlloc(sizeof(CsrBtAtDgConnectInd));
    atData->btConnId            =  dgPrim->btConnId;
    atData->serverChannel       =  dgPrim->serverChannel;
    atPrim->type                =  CSR_BT_AT_DG_CONNECT_IND;

    if (dgPrim->resultSupplier == CSR_BT_SUPPLIER_DG &&
        dgPrim->resultCode == CSR_BT_RESULT_CODE_DG_SUCCESS)
    {
        atPrim->resultCode       = CSR_BT_RESULT_CODE_AT_SUCCESS;
        atPrim->resultSupplier   = CSR_BT_SUPPLIER_AT;
    }
    else
    {
        atPrim->resultCode       =  dgPrim->resultCode;
        atPrim->resultSupplier   = dgPrim->resultSupplier;
    }
    atPrim->btConnId             =  dgPrim->btConnId;
    atPrim->serverChannel        =  dgPrim->serverChannel;
    atPrim->profileMaxFrameSize  =  dgPrim->profileMaxFrameSize;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
atDgDataPathStatusIndHandler
************************************************************************************/
void atDgDataPathStatusIndHandle(atInstanceData_t *atData)
{
    CsrBtDgDataPathStatusInd    *dgPrim;
    CsrBtAtDgDataPathStatusInd *atPrim;

    dgPrim      = (CsrBtDgDataPathStatusInd*)atData->atMsgP;
    atPrim      = (CsrBtAtDgDataPathStatusInd*)CsrPmemAlloc(sizeof(CsrBtAtDgDataPathStatusInd));

    atPrim->type            = CSR_BT_AT_DG_DATA_PATH_STATUS_IND;
    atPrim->dgInstanceQueue = dgPrim->dgInstanceQueue;
    atPrim->status          = dgPrim->status;

    CsrBtAtMessagePut(atData->appHandle, atPrim);
}

/*************************************************************************************
atDgDeactivateCfmHandler
************************************************************************************/
void atDgDeactivateCfmHandler(atInstanceData_t *atData)
{
    CsrBtDgDeactivateCfm    *dgPrim;
    CsrBtAtDgDeactivateCfm    *atPrim;

    dgPrim    = (CsrBtDgDeactivateCfm *) atData->atMsgP;
    atPrim    = (CsrBtAtDgDeactivateCfm *)CsrPmemAlloc(sizeof(CsrBtAtDgDeactivateCfm));

    atData->state = atCmd_s;
    atPrim->type            = CSR_BT_AT_DG_DEACTIVATE_CFM;

    if (dgPrim->resultSupplier == CSR_BT_SUPPLIER_DG &&
        dgPrim->resultCode == CSR_BT_RESULT_CODE_DG_SUCCESS)
    {
        atPrim->resultCode     = CSR_BT_RESULT_CODE_AT_SUCCESS;
        atPrim->resultSupplier = CSR_BT_SUPPLIER_AT;
    }
    else
    {
        atPrim->resultCode     = dgPrim->resultCode;
        atPrim->resultSupplier = dgPrim->resultSupplier;
    }
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}



/*************************************************************************************
atDgJustDataIndHandler
************************************************************************************/
void atDgJustDataIndHandler(atInstanceData_t *atData)
{
    CsrBtDgDataInd    *dgPrim;
    CsrBtAtDgDataInd    *atPrim;


    dgPrim    = (CsrBtDgDataInd *) atData->atMsgP;
    atPrim    = (CsrBtAtDgDataInd *)CsrPmemAlloc(sizeof(CsrBtAtDgDataInd));

    atData->appResFlag        = NO_APP_RES;

    atPrim->type             = CSR_BT_AT_DG_DATA_IND;
    atPrim->payload          = dgPrim->payload;
    atPrim->payloadLength    = dgPrim->payloadLength;
    atPrim->btConnId         = dgPrim->btConnId;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
atDgDataCfmHandler
************************************************************************************/
void atDgDataCfmHandler(atInstanceData_t *atData)
{
    CsrUint16          eventClass;
    void *              msg;

    if(CsrMessageQueuePop(&atData->cmDataReqQueue, &eventClass, &msg))
    {
        CsrSchedMessagePut(CSR_BT_DG_IFACEQUEUE, eventClass , msg);
    }
    else
    {
        atData->atCfmFlag = CFM_RECEIVE;

        if(atData->state == atOnlineData_s)
        {
            CsrBtDgDataCfm    *dgPrim;
            CsrBtAtDgDataCfm    *atPrim;
            dgPrim    = (CsrBtDgDataCfm *) atData->atMsgP;
            atPrim    = (CsrBtAtDgDataCfm *)CsrPmemAlloc(sizeof(CsrBtAtDgDataCfm));

            atPrim->type     = CSR_BT_AT_DG_DATA_CFM;
            atPrim->btConnId = dgPrim->btConnId;
            CsrBtAtMessagePut(atData->appHandle, atPrim);
        }
    }
}

/*************************************************************************************
 setAbortFlag :
************************************************************************************/
void setAbortFlag (CsrUint16 m, atInstanceData_t * atData)
{
    CSR_UNUSED(m);
    atData->abortFlag            = ABORT_ON;
    atData->superVisionTimerId    = FALSE;
}

/*************************************************************************************
 atDgdataIndInUnknownStateHandler :
************************************************************************************/
void atDgdataIndInUnknownStateHandler(atInstanceData_t *atData)
{
    CsrBtDgDataInd    *dgPrim;

    dgPrim    = (CsrBtDgDataInd *) atData->atMsgP;
    CsrPmemFree(dgPrim->payload);
}

/*************************************************************************************
 atDgdataIndInUnknownStateHandler :
************************************************************************************/
void atDgdataIndAtCmdAtOnlineCmdStateHandler(atInstanceData_t *atData)
{
    CsrBtDgDataInd    *dgPrim;

    dgPrim    = (CsrBtDgDataInd *) atData->atMsgP;

    CsrBtDgDataResSend(atData->btConnId);

    if (atData->paramSettings.eValue == ECHO_ON)
    {
        dgDataEchoSend(atData);
    }

    if (atData->responseFlag == NOWAITINGFORRESPONSE)
    {
        atDgdataIndHandler(atData);
    }
    else
    {
        abortHandler(atData);
    }
    CsrPmemFree(dgPrim->payload);
}


/*************************************************************************************
 atDgdatas3Present : TRUE if the s3 terminator has been received
************************************************************************************/
static CsrBool atDgdataS3Present(atInstanceData_t *atData)
{
    CsrUint16 i;

    for (i=0; i < atData->packetLength; i++)
    {
        if (atData->cmd[i] == atData->paramSettings.s3Value)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*************************************************************************************
 atDgdataIndHandler :
************************************************************************************/
void atDgdataIndHandler(atInstanceData_t *atData)
{

    CsrBtDgDataInd    *dgPrim;
    
    dgPrim    = (CsrBtDgDataInd *) atData->atMsgP;
    if (atData->packetLength != 0)
    {
        CsrUint8 *tmp           = atData->cmd;
        atData->cmd             = CsrPmemZalloc(atData->packetLength + dgPrim->payloadLength);
        CsrMemCpy(atData->cmd, tmp, atData->packetLength);
        CsrMemCpy(&atData->cmd[atData->packetLength], dgPrim->payload, dgPrim->payloadLength);
        CsrPmemFree(tmp);
    }
    else
    {
        atData->cmd             = CsrPmemZalloc(dgPrim->payloadLength);
        CsrMemCpy(atData->cmd, dgPrim->payload, dgPrim->payloadLength);
    }
    atData->packetLength    += dgPrim->payloadLength;
    atData->counter         = EMPTY;
    
    if (atDgdataS3Present(atData))
    {
        mainAtInterpret(atData->cmd, atData);

        if ((atData->reachPoint == INTERPRET_ERROR) || (atData->terminatorFlag == TERMINATORNOTFOUND))
        {
            atData->reachPoint                            = CONTINUATION_AT_INTERPRET_FUNC;
            atData->terminatorFlag                        = TERMINATORFOUND;
            atData->counter                                = EMPTY;
            atData->tempValue                            = EMPTY;
            atData->tokenFlipArray[EMPTY].receiveToken    = INTERPRET_ERROR;
            if(atData->state == atCmd_s)
            {
                atResultCodeErrorHandler(atData);
            }
        }
        else
        {
            atTokenHandler(atData);
        }
    }
}

#endif

