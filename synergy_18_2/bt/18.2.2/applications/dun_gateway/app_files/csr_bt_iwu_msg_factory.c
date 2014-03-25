/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "bluetooth.h"

#ifndef __arm
#include <stdio.h>
#include "csr_bt_iwu.h"
static CsrUint32 oldModemStat;
#endif

#include "csr_bt_dg_prim.h"
#include "csr_bt_dg_lib.h"
#include "csr_bt_iwu_prim.h"
#include "csr_bt_iwu_handler.h"
#include "csr_bt_at_lib.h"
#include "csr_bt_usr_config.h"

extern void ClearDunState(void);
#ifndef __arm
/******************************************************************************
  checkModemStat :
******************************************************************************/
void checkModemStat(void)
{
    CsrUint32    modemStat;
    /*    read the modem status lines */
    if ( CsrIwuModemStatGet(&modemStat) == TRUE )
    {
        if ( modemStat != oldModemStat )
        {
            IWU_MODEM_STATUS_REQ_T *prim;
            CsrUint8 newModemStat = 0x00;

            prim = (IWU_MODEM_STATUS_REQ_T *)CsrPmemAlloc(sizeof(IWU_MODEM_STATUS_REQ_T));
            prim->type = IWU_MODEM_STATUS_REQ;
            oldModemStat = modemStat;

            if ( (modemStat & MS_CTS_ON)  ==  MS_CTS_ON)
            {
                newModemStat |= CSR_BT_MODEM_CTS_MASK;
            }

            if ( (modemStat & MS_DSR_ON) == MS_DSR_ON)
            {
                newModemStat |= CSR_BT_MODEM_DSR_MASK;
            }

            if ( (modemStat & MS_RING_ON) == MS_RING_ON)
            {
                newModemStat |= CSR_BT_MODEM_RI_MASK;
            }

            if ( (modemStat & MS_RLSD_ON) == MS_RLSD_ON)
            {
                newModemStat |= CSR_BT_MODEM_DCD_MASK;
            }

            prim->modemStatusFlag = newModemStat;
            CsrSchedMessagePut(CSR_BT_IWU_IFACEQUEUE, CSR_BT_IWU_PRIM, prim);
        }
    }
    else
    {
        printf("modem stat not supported\n");
    }
}


/******************************************************************************
  sendReadySignaltoDemoApp:
******************************************************************************/
void sendReadySignaltoDemoApp(void)
{
    IWU_MODEM_READY_REQ_T    *prim;

    prim                        = (IWU_MODEM_READY_REQ_T *) CsrPmemAlloc(sizeof(IWU_MODEM_READY_REQ_T));
    prim->type                    = IWU_MODEM_READY_REQ;
    prim->result                = 0;
    CsrSchedMessagePut(TESTQUEUE, CSR_BT_IWU_PRIM, prim);
}

/*****************************************************************************
  deactivateReq :
******************************************************************************/
void deactivateReq(iwuInstanceData_t *iwuData)
{
    CsrBtAtDgDeactivateReqSend();
    if (iwuData->connectFlag == CONNECT)
    {
        modemReset(iwuData);
    }

    printf("Page scan deactivated\n\n");
}

/*****************************************************************************
  releaseReq :
******************************************************************************/
void releaseReq(iwuInstanceData_t *iwuData)
{
    CsrBtAtDgDisconnectReqSend(iwuData->btConnId);
    modemReset(iwuData);
    iwuData->lastSend = AT;
    iwuData->connectFlag = NOT_CONNECT;
    iwuData->btConnId = 0x00ff;
    iwuData->serverChannel = 0xff;
    printf("Connection has been released\n\n");
}

#endif


/*****************************************************************************
  iwuSendCommandAtA: send a AT A command to the modem
******************************************************************************/
void iwuSendCommandAtA(iwuInstanceData_t *iwuData)
{
    CsrUint8 dataPtr[4];
    CsrUint16 valueLen;
    CsrUint16 numSend;

    iwuData->lastSend    = A;
    valueLen            = 4;

    dataPtr[0]            = 'A';
    dataPtr[1]            = 'T';
    dataPtr[2]            = 'A';
    dataPtr[3]            = 13;
    UartDrvTx((char*)dataPtr, valueLen, &numSend);
}

/*****************************************************************************
  iwuSendCommandAtH: send a AT H command to the modem
******************************************************************************/
void iwuSendCommandAtH(iwuInstanceData_t *iwuData)
{
    CsrUint8 dataPtr[4];
    CsrUint16 valueLen;
    CsrUint16 numSend;

    iwuData->lastSend    = AT;
    valueLen             = 4;

    dataPtr[0]            = 'A';
    dataPtr[1]            = 'T';
    dataPtr[2]            = 'H';
    dataPtr[3]            = 13;
    UartDrvTx((char*)dataPtr, valueLen, &numSend);
}

/*****************************************************************************
  iwuSendCommandAthIn:
******************************************************************************/
void iwuSendCommandAthIn(CsrUint16 dummy, void *iwuData)
{
    iwuSendCommandAtH(iwuData);
}

/*****************************************************************************
  iwuSendCommandAtD: send a AT A command to the modem
******************************************************************************/
void iwuSendCommandAtD(iwuInstanceData_t *iwuData)
{
    CsrUint8 dataPtr[CSR_BT_MAX_DIAL_STRING_LENGTH + 5];
    CsrUint16 valueLen;
    CsrUint16 numSend;
    int i;
    CsrBtAtDgAtcDInd *iwuPrim;

    iwuPrim    = (CsrBtAtDgAtcDInd *) iwuData->iwuMsgP;

    dataPtr[0]            = 'A';
    dataPtr[1]            = 'T';
    dataPtr[2]            = 'D';
    for(i = 0; i < iwuPrim->payloadLength; i++)
    {
        dataPtr[i + 3] = iwuPrim->payload[i];
    }
    if(iwuPrim->value == SEMICOLON)
    {
        dataPtr[iwuPrim->payloadLength + 3] = ';';
        dataPtr[iwuPrim->payloadLength + 4] = 13;
        valueLen = (CsrUint16)(iwuPrim->payloadLength + 5);
    }
    else
    {
        dataPtr[iwuPrim->payloadLength + 3]    = 13;
        valueLen = (CsrUint16)(iwuPrim->payloadLength + 4);
    }
    UartDrvTx((char*)dataPtr, valueLen, &numSend);
    CsrPmemFree(iwuPrim->payload);
    iwuData->lastSend = D;
}


/*****************************************************************************
  iwuSendModemResetReq:
******************************************************************************/
void iwuSendModemResetReq(iwuInstanceData_t * iwuData)
{
    CsrUint8                dataPtr[1];
    CsrUint16            valueLen;
    CsrUint16            numSend;

    valueLen                =  1;
    dataPtr[0]                = '+';
    iwuData->lastSend        = AT;
    iwuData->connectFlag    = NOT_CONNECT;
    UartDrvTx((char*)dataPtr, valueLen, &numSend);
}

/*****************************************************************************
  iwuSendModemResetReqIn:
******************************************************************************/
void iwuSendModemResetReqIn(CsrUint16 dummy, void *iwuData)
{
    iwuSendModemResetReq(iwuData);
}

/*****************************************************************************
  iwuSendAbortReq:
******************************************************************************/
void iwuSendAbortReq(iwuInstanceData_t * iwuData)
{
    CsrUint8 dataPtr[1];
    CsrUint16 valueLen;
    CsrUint16 numSend;

    valueLen     =  1;
    dataPtr[0]   = 'F';
    UartDrvTx((char*)dataPtr, valueLen, &numSend);
    iwuData->lastSend = D;
}


/*****************************************************************************
  iwuSendAtDgDataReq:
******************************************************************************/
void iwuSendAtDgDataReq(iwuInstanceData_t * iwuData)
{
    CsrBtAtDgDataReq *atPrim;
    CsrUint8 * dataPtr;
    int i;

    atPrim    = (CsrBtAtDgDataReq *)CsrPmemAlloc(sizeof(CsrBtAtDgDataReq));
    dataPtr    = CsrPmemAlloc(iwuData->bufCount);

    for(i = 0; i < iwuData->bufCount; i++)
    {
        dataPtr[i] = iwuData->modemBuf[i];
    }

    atPrim->type            = CSR_BT_AT_DG_DATA_REQ;
    atPrim->btConnId        = iwuData->btConnId;
    atPrim->payloadLength   = iwuData->bufCount;
    atPrim->payload         = dataPtr;
    if(iwuData->iwuCfmFlag == FALSE)
    {
        CsrMessageQueuePush(&iwuData->iwuDataReqQueue, CSR_BT_AT_PRIM, atPrim);
        iwuData->queueCounter++;
    }
    else
    {
        CsrSchedMessagePut(CSR_BT_AT_IFACEQUEUE, CSR_BT_AT_PRIM, atPrim);
        iwuData->iwuCfmFlag = FALSE;
    }
    iwuData->bufCount = 0;
}

/*****************************************************************************
  iwuSendAtDgDataReq:
******************************************************************************/
void iwuSendModemDataReq(int theLength, char *theData, CsrUint8 theResult)
{
    IWU_MODEM_DATA_REQ_T *iwuPrim;
    iwuPrim = (IWU_MODEM_DATA_REQ_T *) CsrPmemAlloc(sizeof(IWU_MODEM_DATA_REQ_T));
    iwuPrim->type = IWU_MODEM_DATA_REQ;
    iwuPrim->modemPayload    = (CsrUint8 *)theData;
    iwuPrim->payloadLength    = theLength;
    iwuPrim->uart1ErrCodes    = theResult;
    CsrSchedMessagePut(CSR_BT_IWU_IFACEQUEUE, CSR_BT_IWU_PRIM, iwuPrim);
}


/*****************************************************************************
  modemDefaultSetupReq:     default setting for modem
******************************************************************************/
void modemDefaultSetupReq(iwuInstanceData_t * iwuData)
{
    CsrUint8 dataPtr[20];
    CsrUint16 valueLen;
    CsrUint16 numSend;
    iwuData->lastSend    = INIT;
    iwuData->paramSettings.xValue        =  0;
    iwuData->paramSettings.s0Value        =  0;
    iwuData->paramSettings.s3Value        = 13;
    iwuData->paramSettings.s4Value        = 10;
    iwuData->paramSettings.s5Value        =  8;
    iwuData->paramSettings.s6Value        =  2;
    iwuData->paramSettings.s7Value        = 45;
    iwuData->paramSettings.s8Value        =  2;
    iwuData->paramSettings.s10Value        = 14;
    iwuData->connectFlag                = NOT_CONNECT;
#ifndef __arm
    oldModemStat                    = MS_CTS_ON | MS_DSR_ON;
#endif
    valueLen    = 11;
    dataPtr[0]  = 'A';
    dataPtr[1]  = 'T';
    dataPtr[2]  = 'E';
    dataPtr[3]  = '0';
    dataPtr[4]  = 'L';
    dataPtr[5]  = '0';
    dataPtr[6]  = 'V';
    dataPtr[7]  = '0';
    dataPtr[8]  = 'M';
    dataPtr[9]  = '1';
    dataPtr[10]  = 13;
    UartDrvTx((char*)dataPtr, valueLen, &numSend);
    if (!iwuData->startUpTimerActive)
    {
        iwuData->startUpTimerId = CsrSchedTimerSet(MODEM_CHECK_STARTUP_TIME, (void (*) (CsrUint16, void *)) startUpTimeHandler, 0, (void *) iwuData);
        iwuData->startUpTimerActive = TRUE;
    }
}

static void sendIwuModemResetReq(CsrUint16 val, void *ptr)
{
    IWU_MODEM_RESET_REQ_T    *prim;

    prim = (IWU_MODEM_RESET_REQ_T *)CsrPmemAlloc(sizeof(IWU_MODEM_RESET_REQ_T));
    prim->type = IWU_MODEM_RESET_REQ;

    CsrSchedMessagePut(CSR_BT_IWU_IFACEQUEUE, CSR_BT_IWU_PRIM, prim);
}

/*****************************************************************************
  modemReset :
******************************************************************************/
void modemReset(iwuInstanceData_t *iwuData)
{
    CsrUint16                eventClass;
    CsrBtAtDgDataReq        *msg;
    CsrUint8                    dataPtr[1];
    CsrUint16                numSend;

    while((CsrMessageQueuePop(&iwuData->iwuDataReqQueue, &eventClass, (void **) &msg)))
    {
        CsrPmemFree(msg->payload);
        CsrPmemFree(msg);
    }
    iwuData->queueCounter = 0;
    /*    send a H command to modem to disconnect line */
    dataPtr[0]   = 'H';
    iwuData->lastSend = AT;
    iwuData->connectFlag = NOT_CONNECT;
    UartDrvTx((char*)dataPtr, 1, &numSend);
#ifndef __arm
    oldModemStat = MS_CTS_ON | MS_DSR_ON;
#endif

    /* Schedule to send 3 signals to myself for reset of the modem side */
    CsrSchedTimerSet(MODEM_RESET_DELAY,sendIwuModemResetReq,0,NULL);
    CsrSchedTimerSet(MODEM_RESET_DELAY*2,sendIwuModemResetReq,0,NULL);
    CsrSchedTimerSet(MODEM_RESET_DELAY*3,sendIwuModemResetReq,0,NULL);
}



/*****************************************************************************
  modemAnswerOrDialHandler :
******************************************************************************/
void modemAnswerOrDialHandler(iwuInstanceData_t * iwuData)
{
    CsrBtAtResultCode modemResult;
    IWU_MODEM_DATA_REQ_T *iwuPrim;
    iwuPrim    = (IWU_MODEM_DATA_REQ_T *) iwuData->iwuMsgP;

    if(iwuData->bufCount > 0)
    {
        modemResult = (iwuData->modemBuf[0] - '0');

        if(iwuData->modemBuf[0] == '1')
        {
            iwuData->lastSend = DATA;
#ifndef __arm
            printf("Connect accept from network received\n");
#endif
        }
#ifndef __arm
        else
        {
            printf("Code other than connect accept from network received: %i", modemResult);
        }
#endif
    }
    else
    {
        modemResult            = iwuPrim->modemPayload[0] - '0';
        if(iwuPrim->modemPayload[0] == '1')
        {
            iwuData->lastSend = DATA;
#ifndef __arm
            printf("Connect accept from network received\n");
#endif
        }
#ifndef __arm
        else
        {
            printf("Code other than connect accept from network received: %i", modemResult);
        }
#endif
    }
    CsrBtAtDgAtcResultCodeResSend(modemResult);
}


/*****************************************************************************
  modemNoSignalHandler :
******************************************************************************/
void modemNoSignalHandler(iwuInstanceData_t * iwuData)
{
    CsrUint8 modemResult;
    IWU_MODEM_DATA_REQ_T *iwuPrim;
    iwuPrim    = (IWU_MODEM_DATA_REQ_T *) iwuData->iwuMsgP;

    modemResult            = iwuPrim->modemPayload[0] - '0';
#ifndef __arm
    if(modemResult == 2)
    {
        printf("Ring indication from NT received\n");
    }
    else if (modemResult == 1)
    {
        printf("Connect indication from NT received \n");
    }
#endif
    CsrBtAtDgAtcResultCodeResSend(modemResult);
}


/*****************************************************************************
  iwuModemStatusReqHandler :
******************************************************************************/
void iwuModemStatusReqHandler(iwuInstanceData_t * iwuData)
{
    IWU_MODEM_STATUS_REQ_T *iwuPrim;
    iwuPrim    = (IWU_MODEM_STATUS_REQ_T *) iwuData->iwuMsgP;
    iwuData->bitSetting = iwuPrim->modemStatusFlag;
    if(iwuData->connectFlag == CONNECT)
    {
        CsrBtAtDgControlReqSend(iwuData->btConnId, iwuPrim->modemStatusFlag,0x00);
    }
}

/*****************************************************************************
    Receive a port neg and respond if required.
******************************************************************************/
void iwuAtDgPortnegIndHandler(iwuInstanceData_t * iwuData)
{
    CsrBtAtDgPortnegInd *prim;

    prim = (CsrBtAtDgPortnegInd *)iwuData->iwuMsgP;
    if (prim->request == TRUE)
    {
        RFC_PORTNEG_VALUES_T portPar;

        portPar.baud_rate = CSR_BT_DEFAULT_PORT_SPEED;
        portPar.data_bits = CSR_BT_DEFAULT_DATA_BITS;
        portPar.stop_bits = CSR_BT_DEFAULT_STOP_BITS;
        portPar.parity = CSR_BT_DEFAULT_PARITY;
        portPar.parity_type = CSR_BT_DEFAULT_PARITY_TYPE;
        portPar.flow_ctrl_mask = CSR_BT_DEFAULT_FLOW_CTRL_MASK;
        portPar.xon = CSR_BT_DEFAULT_XON;
        portPar.xoff = CSR_BT_DEFAULT_XOFF;
        portPar.parameter_mask = CSR_BT_DEFAULT_PARAMETER_MASK;
        CsrBtAtDgPortnegResSend(prim->btConnId, &portPar);
    }
    else
    {
        CsrBtAtDgPortnegResSend(prim->btConnId, &(prim->portPar) );
    }
}

/*****************************************************************************
  iwuAtDgConnectIndHandler :
******************************************************************************/
void iwuAtDgConnectIndHandler(iwuInstanceData_t *iwuData)
{
    CsrBtAtDgConnectInd        *iwuPrim;

    iwuPrim    = (CsrBtAtDgConnectInd *) iwuData->iwuMsgP;
    if((iwuPrim->resultCode == CSR_BT_RESULT_CODE_AT_SUCCESS) &&
       (iwuPrim->resultSupplier == CSR_BT_SUPPLIER_AT))
    {
        iwuData->btConnId         = iwuPrim->btConnId;
        iwuData->serverChannel    = iwuPrim->serverChannel;
        iwuData->connectFlag = CONNECT;

        CsrBtAtDgControlReqSend(iwuData->btConnId, iwuData->bitSetting,0x00);

#ifndef __arm
        printf("Connect ind received from client side with success\n");
#endif
    }
    else
    {
        iwuData->state = modemReady;
        ClearDunState();
#ifndef __arm
        printf("Connect ind received from client side with error code %02X\n", iwuPrim->resultCode);
#endif
    }
}

/*****************************************************************************
  iwuAtDgDataCfmHandler :
******************************************************************************/
void iwuAtDgDataCfmHandler(iwuInstanceData_t *iwuData)
{
    CsrUint16          eventClass;
    void *              msg;

    if(CsrMessageQueuePop(&iwuData->iwuDataReqQueue, &eventClass, &msg))
    {
        iwuData->queueCounter--;
        CsrSchedMessagePut(CSR_BT_AT_IFACEQUEUE, eventClass , msg);
    }
    else
    {
        iwuData->iwuCfmFlag = TRUE;
    }
}

/*****************************************************************************
  iwuAtDgDataIndHandler :
******************************************************************************/
void iwuAtDgDataIndHandler(iwuInstanceData_t *iwuData)
{
    CsrUint16 numSend;
    CsrBtAtDgDataInd *iwuPrim;
    iwuPrim    = (CsrBtAtDgDataInd *) iwuData->iwuMsgP;

    CsrBtAtDgDataResSend(iwuData->btConnId);
    UartDrvTx((char*)iwuPrim->payload, iwuPrim->payloadLength, &numSend);
    CsrPmemFree(iwuPrim->payload);
    iwuData->lastSend = DATA;
}

/*****************************************************************************
  iwuAtDgDisconnectIndHandler :
******************************************************************************/
void iwuAtDgDisconnectIndHandler(iwuInstanceData_t *iwuData)
{
    modemReset(iwuData);
    ClearDunState();
    iwuData->state = modemReady;
    iwuData->lastSend = AT;
    iwuData->connectFlag = NOT_CONNECT;
    iwuData->btConnId = 0x00ff;
    iwuData->serverChannel = 0xff;
#ifndef __arm
    printf("Disconnect from client side received\n");
#endif
}

/*****************************************************************************
  iwuAtDgAtcXIndHandler :
******************************************************************************/
void iwuAtDgAtcXIndHandler(iwuInstanceData_t *iwuData)
{
    CsrBtAtDgAtcXInd    *iwuPrim;
    iwuPrim    = (CsrBtAtDgAtcXInd *) iwuData->iwuMsgP;

    iwuData->paramSettings.xValue = iwuPrim->value;
    CsrBtAtDgAtcResultCodeResSend(0);
}


/*****************************************************************************
  iwuAtDgAtcAndFIndHandler :
******************************************************************************/
void iwuAtDgAtcAndFIndHandler(iwuInstanceData_t *iwuData)
{
    iwuData->paramSettings.xValue = 0;
    CsrBtAtDgAtcFactorySettingResSend(13, 0, 1, 10, 1, 8, 2);
}


/*****************************************************************************
  iwuAtDgAtcZIndHandler :
******************************************************************************/
void iwuAtDgAtcZIndHandler(iwuInstanceData_t *iwuData)
{
    CsrBtAtDgAtcDefaultSettingResSend(13, 0, 1, 10, 1, 8, 2);
    iwuData->paramSettings.xValue        = 0;
}

/*****************************************************************************
  iwuAtDgControlIndHandler :
******************************************************************************/
void iwuAtDgControlIndHandler(iwuInstanceData_t *iwuData)
{
#ifndef __arm
    printf("A CSR_BT_AT_DG_CONTROL_IND signal is received. It is not supported in this demo version\n");
#endif
}


/*****************************************************************************
  iwuAtDgAtcOIndHandler :
******************************************************************************/
void iwuAtDgAtcOIndHandler(void)
{
#ifndef __arm
    printf("A CSR_BT_AT_DG_ATC_O_IND received, just sending OK to peer device\n");
#endif
    CsrBtAtDgAtcResultCodeResSend(0);
}


#ifdef __arm
static eventToSendAtDgActivateRequest(CsrUint16 val, void *ptr)
{
    CsrBtAtDgActivateReq    *prim;

    prim    = (CsrBtAtDgActivateReq *) CsrPmemAlloc(sizeof(CsrBtAtDgActivateReq));
    prim->type                = CSR_BT_AT_DG_ACTIVATE_REQ;
    prim->phandle            = CSR_BT_IWU_IFACEQUEUE;
    prim->timeout            = 0;

    CsrSchedMessagePut(CSR_BT_AT_IFACEQUEUE, CSR_BT_AT_PRIM, prim);
}
#endif

/*****************************************************************************
  iwuAtDgDeactivateCfmHandler :
******************************************************************************/
void iwuAtDgDeactivateCfmHandler(iwuInstanceData_t *iwuData)
{
    CsrBtAtDgDeactivateCfm    *iwuPrim;
    iwuPrim    = (CsrBtAtDgDeactivateCfm *) iwuData->iwuMsgP;

    if((iwuPrim->resultCode == CSR_BT_RESULT_CODE_AT_SUCCESS) &&
       (iwuPrim->resultSupplier == CSR_BT_SUPPLIER_AT))
    {
#ifdef __arm
        CsrSchedTimerSet(10000000,eventToSendAtDgActivateRequest,0,NULL);
#endif

#ifndef __arm
        printf("The service has been deactivated\n");
#endif
    }
#ifndef __arm
    else
    {
        printf("Unable to deactivate the service\n");
    }
#endif
}

/*****************************************************************************
  iwuAtDgDataPathStatusIndHandler
******************************************************************************/
void iwuAtDgDataPathStatusIndHandler(iwuInstanceData_t *iwuData)
{
    CsrBtAtDgDataPathStatusInd *atPrim;
    IWU_DATA_PATH_STATUS_IND_T   *iwPrim;

    atPrim = (CsrBtAtDgDataPathStatusInd*)iwuData->iwuMsgP;
    iwPrim = (IWU_DATA_PATH_STATUS_IND_T*)CsrPmemAlloc(sizeof(IWU_DATA_PATH_STATUS_IND_T));

    iwPrim->type            = IWU_DATA_PATH_STATUS_IND;
    iwPrim->dgInstanceQueue = atPrim->dgInstanceQueue;
    iwPrim->status          = atPrim->status;

    CsrSchedMessagePut(TESTQUEUE, CSR_BT_IWU_PRIM, iwPrim);
}

/*****************************************************************************
  iwuAtResponseCodeHandler :
******************************************************************************/
void iwuAtResponseCodeHandler(iwuInstanceData_t *iwuData)
{
    int i;
    IWU_MODEM_DATA_REQ_T *iwuPrim;

    iwuPrim    = (IWU_MODEM_DATA_REQ_T *) iwuData->iwuMsgP;

    if((iwuData->lastSend == D) || (iwuData->lastSend == A))
    {
        CsrBool tempFlag = FALSE;
        if(iwuData->bufCount == 0)
        {
            for(i = 0; i < iwuPrim->payloadLength; i++)
            {
                if(!tempFlag && (iwuPrim->modemPayload[i] == '\r'))
                {
                    signalHandler(iwuData);
                    if( (i + 1) < iwuPrim->payloadLength)
                    {
                        tempFlag = TRUE;
                        i++;
                    }
                }

                if(tempFlag == TRUE)
                {
                    iwuData->modemBuf[iwuData->bufCount] = iwuPrim->modemPayload[i];
                    iwuData->bufCount++;
                }
            }
        }
        else
        {
            for(i = 0; i < iwuPrim->payloadLength; i++)
            {
                if(!tempFlag && (iwuPrim->modemPayload[i] == '\r'))
                {
                    signalHandler(iwuData);
                    iwuData->bufCount = 0;
                    tempFlag = TRUE;
                }
                else
                {
                    iwuData->modemBuf[iwuData->bufCount] = iwuPrim->modemPayload[i];
                    iwuData->bufCount++;
                    if(iwuData->bufCount >= 120)
                    {
                        iwuData->bufCount = 0;
                    }
                }
            }

        }
    }
    else
    {
        iwuData->bufCount = 0;
        signalHandler(iwuData);
    }
}


/*****************************************************************************
  iwuModemDataReq :
******************************************************************************/
void iwuModemDataReq(iwuInstanceData_t *iwuData)
{
    int i;
    IWU_MODEM_DATA_REQ_T *iwuPrim;

    iwuPrim    = (IWU_MODEM_DATA_REQ_T *) iwuData->iwuMsgP;

    if(iwuData->queueCounter < MAX_QUEUE_SIZE && iwuData->waterMarkFlag == FALSE)
    {
        if(iwuData->lastSend == DATA)
        {
            for(i = 0; i < iwuPrim->payloadLength; i++)
            {
                iwuData->modemBuf[iwuData->bufCount] = iwuPrim->modemPayload[i];
                iwuData->bufCount++;
                if(iwuData->bufCount > 120)
                {
                    bufHandler(iwuData);
                    iwuData->bufCount = 0;
                }
            }

            if(iwuData->bufCount > 0)
            {
                bufHandler(iwuData);
            }
        }
        else
        {
            iwuAtResponseCodeHandler(iwuData);
        }
    }
    else
    {
        if(iwuData->waterMarkFlag == FALSE)
        {
            iwuData->waterMarkFlag = TRUE;
        }
        else
        {
            if(iwuData->queueCounter == 0)
            {
                iwuData->waterMarkFlag = FALSE;
            }
        }

    }
    CsrPmemFree(iwuPrim->modemPayload);
}


/*****************************************************************************
  bufHandler :
******************************************************************************/
void bufHandler(iwuInstanceData_t * iwuData)
{
    if(iwuData->timerFlag == FALSE && iwuData->bufCount < 96)
    {
        iwuData->timerFlag = TRUE;
        iwuData->timerId = CsrSchedTimerSet(12000, (void (*) (CsrUint16, void *)) timeHandler, 0, (void *) iwuData);
    }
    else if(iwuData->bufCount >= 96)
    {
        iwuSendAtDgDataReq(iwuData);
        if(iwuData->timerFlag == TRUE)
        {
            CsrSchedTimerCancel(iwuData->timerId,NULL,NULL);
            iwuData->timerFlag = FALSE;
        }
    }
}

/*****************************************************************************
  timeHandler :
******************************************************************************/
void timeHandler(CsrUint16 m, iwuInstanceData_t * iwuData)
{
    iwuSendAtDgDataReq(iwuData);
    iwuData->timerFlag = FALSE;
    m = 0;
}

/*****************************************************************************
  startupTimeHandler :
******************************************************************************/
void startUpTimeHandler(CsrUint16 m, iwuInstanceData_t * iwuData)
{
    iwuData->startUpTimerActive = FALSE;
    printf("\nModem is not responding on the chosen Com port\n\n");
    printf("     Hit <Esc> to exit\n\n");
}

/*****************************************************************************
  interPretHandler :
******************************************************************************/
CsrBool interPretHandler(iwuInstanceData_t * iwuData)
{
    CsrUint8                    i;
    IWU_MODEM_DATA_REQ_T    *iwuPrim;

    iwuPrim    = (IWU_MODEM_DATA_REQ_T *) iwuData->iwuMsgP;

    for(i = 0; i < iwuPrim->payloadLength; i++)
    {
        if(iwuData->countN == 0)
        {
            if(iwuPrim->modemPayload[i] == '\r')
            { /* Found <cr> */
                if(iwuData->count == 1)
                { /* The modem has send 0<cr>, the numeric result code for OK.
                     The modem is now initialize return TRUE */
                    iwuData->count    = 0;
                    return TRUE;
                }
                else if(iwuData->count == 0)
                { /* Assume that the modem now sending <cr><lf>OK<cr><lf> */
                    iwuData->countN++;
                }
                else
                { /* The modem has echo the send message */
                    iwuData->count = 0;
                }
            }
            else
            {
                iwuData->count++;
            }
        }
        else
        {
            if(iwuPrim->modemPayload[i] == '\n')
            { /* Found a <lf> */
                if(iwuData->countN == 2)
                { /* Found the second <lf>. The modem is now initialize return TRUE */
                    iwuData->count    = 0;
                    iwuData->countN = 0;
                    return TRUE;
                }
                else
                { /* Found the first <lf> */
                    iwuData->countN++;
                }
            }
        }
    }
    return FALSE;
}

/*****************************************************************************
  signalHandler :
******************************************************************************/
void signalHandler(iwuInstanceData_t * iwuData)
{
    switch(iwuData->lastSend)
    {
        case A     :
        case D     :
            {
                modemAnswerOrDialHandler(iwuData);
                break;
            }
        case AT :
            {
                modemDefaultSetupReq(iwuData);
                break;
            }
            /*    the iwu is being initialised */
        case INIT :
            {
                if(interPretHandler(iwuData))
                {
                    iwuData->state = modemReady;
#ifndef __arm
                    sendReadySignaltoDemoApp();
#else
                    CsrBtAtDgActivateReqSend(CSR_BT_IWU_IFACEQUEUE, INFINIT_TIME);
#endif
                    if (iwuData->startUpTimerActive)
                    {
                        if (CsrSchedTimerCancel(iwuData->startUpTimerId, NULL, NULL) == TRUE)
                        {
                            iwuData->startUpTimerActive = FALSE;
                        }
                    }
                    iwuData->lastSend = NONE;
                }
                break;
            }
            /*    iwu is initialised but no signals have been sent yet */
        case NONE:
            {
                modemNoSignalHandler(iwuData);
                break;
            }
        default :
            {
                /* Modem data received in iwu signalHandler is ignore*/
                break;
            }
    }
}

