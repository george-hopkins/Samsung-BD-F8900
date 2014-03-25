/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_hf_main.h"
#include "csr_bt_hf_main_sef.h"
#include "csr_bt_hf_at_inter.h"
#include "csr_bt_hfhs_data_sef.h"

/* Outgoing AT command table structure */
typedef struct
{
    char               *at;          /* AT command string */
    HfLastAtCmdSent_t  lastCmd;      /* AT cmd that is delivered to CM */
} HfOutAtCheck_t;


static const HfOutAtCheck_t hfOutAtCheck[] =
{
    {"AT+VGM",        vgm                },
    {"AT+VGS",        vgs                },
    {"AT+CIND=",      cindSupportCmd     },
    {"AT+CIND?",      cindStatusCmd      },
    {"AT+CHLD=?",     chldSupport        },
    {"AT+CHLD=",      chld               },
    {"AT+BRSF:",      bsrf               },
    {"AT+BTRH",       btrh               },
    {"AT+CMEE",       cmee               },
    {"AT+CMER",       cmer               },
    {"AT+CCWA",       ccwa               },
    {"AT+CLIP",       clip               },
    {"AT+CLCC",       clcc               },
    {"AT+CNUM",       cnum               },
    {"AT+COPS?",      copsQueryCmd       },
    {"AT+COPS=",      copsSet            },
    {"AT+BVRA",       bvra               },
    {"AT+BINP",       binp               },
    {"ATA",           answer             },
    {"ATD>",          dialMem            },
    {"ATD",           dialNumber         },
    {"AT+BDLN",       redial             },
    {"AT+NREC",       nrec               },
    {"AT+VTS",        vts                },
    {"AT+CHUP",       callEnd            },
    {"AT+CSRBATT=",   c2cbatt            },
    {"AT+CSRPWR",     c2cpwr             },
    {"AT+CSRSF",      c2cSf              },
    {"AT+CSRGETSMS=", c2cSms             },
    {"AT+CSR=",       c2c                },
    {"AT+CSRFN=",     c2cFn              },
    {"AT+BCS=",       bcs                },
    {"AT+BAC=",       bac                },
    {"AT+BCC=",       bcc                },
    { NULL,           other              }
};

/*************************************************************************************

************************************************************************************/
HfLastAtCmdSent_t FindCurrentCmdFromPayload(CsrUint8 *string)
{
    HfLastAtCmdSent_t lastSentCmd = other;
    CsrUint8 i = 0;

    /* Scan the AT command table for a token */
    while(hfOutAtCheck[i].at != NULL)
    {
        if(!CsrMemCmp(string,(unsigned char*)hfOutAtCheck[i].at,CsrStrLen(hfOutAtCheck[i].at)))
        {/* match found; return */
            lastSentCmd = hfOutAtCheck[i].lastCmd;
            return lastSentCmd;
        }
        i++;
    }
    return lastSentCmd;
}

static CsrBool csrBtHfCompletePacketReceived(CsrUint8 *line, CsrUint16 len)
{
    CsrUintFast16       i;
    CsrBool retValue = FALSE;

    /* scan backwards for at least 1 control charachter; ignore ctrl chars at position 0 and 1 */
    for (i=len-1; i > 1; i--)
    {/* skip blank charachters, tabs and NULL terminator! */
        if ((line[i] == '\r') || (line[i] == '\n'))
        {
            /* found! Force return */
            return TRUE;
        }
        else if ((line[i] != ' ') && (line[i] != '\t') && (line[i] != '\0'))
        {/* if a "valid" character is found before a control charachter then the command is not fully received yet...
            return and wait for the rest of the message  */
            return FALSE;
        }

    }

    return retValue;
}


HfHsData_t * getDataInstance(HfMainInstanceData_t * instData, CsrUint32 theHfConnId)
{
    CsrIntFast8 i = 0;
    HfInstanceData_t *linkPtr;


    for (i=0; i < (instData->maxHFConnections + instData->maxHSConnections); i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if (linkPtr->hfConnId == theHfConnId)
        {
            instData->index = i;
            return linkPtr->data;
        }
    }
    return NULL;
}

/*************************************************************************************
    handle data from cm. Collect the data until end of line is received and interpret
    the data. The actions on each command is specified in HfgInterpretAtData function.
    No known commands are forwarded to higher layer without any interpretation.
************************************************************************************/
void CsrBtHfHsXStateCmDataIndHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->data->atResponseTimerId != 0)
    {/* re-start 2 seconds timer if it was running */
        void *mv;
        CsrUint16 mi;
        CsrSchedTimerCancel(linkPtr->data->atResponseTimerId, &mi, &mv);
        linkPtr->data->atResponseTimerId = 0;
    }
    if ( ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HS) && (linkPtr->state > Activate_s)) ||
         ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) && (linkPtr->state != Connect_s)))
    {
        CsrBtCmDataInd    *cmPrim;
        HfHsData_t *    data;

        cmPrim    = (CsrBtCmDataInd *) instData->recvMsgP;
        /* confirm reception of the data */
        CsrBtCmDataResSend(cmPrim->btConnId);
        data = getDataInstance(instData, cmPrim->btConnId);
        /* mark that data has been received. The flag is checked when the LP timer expires */
        data->dataReceivedInConnected = TRUE;
        /* as at commands may be divided into several packets this may be continuation of a
           command where the first part is already stored */
        if (data->recvAtCmd == NULL)
        {
            /* no data is currently stored from a previous command */
            data->recvAtCmd = cmPrim->payload;
            data->recvAtCmdSize = cmPrim->payloadLength;
        }
        else
        {
            /* data is already received. Allocate space for the new data and append */
            CsrUint16    currentAtCmdSize;
            CsrUint8        *p;

            currentAtCmdSize = data->recvAtCmdSize;
            data->recvAtCmdSize = currentAtCmdSize + cmPrim->payloadLength;
            p = (CsrUint8*) CsrPmemAlloc(data->recvAtCmdSize);
            /* copy existing data */
            CsrMemCpy(p, data->recvAtCmd, currentAtCmdSize);
            /* then append new received data */
            CsrMemCpy( (p + currentAtCmdSize), cmPrim->payload, cmPrim->payloadLength);
            /* data from instData and signal copied so free it */
            CsrPmemFree(cmPrim->payload);
            CsrPmemFree(data->recvAtCmd);
            data->recvAtCmd = p;
        }

        /* if end of line received call the interpreter for the received data. If end of line
           is NOT received, jump out and wait for more data to come */
        if ( csrBtHfCompletePacketReceived(data->recvAtCmd, data->recvAtCmdSize) )
        {
            CsrBtHfHandleMultipleAtCommand(instData, cmPrim->btConnId);
        }
    }
}

/*************************************************************************************
    Data has been accepted by CM. Check if more data to send to CM is stored on
    the save queue and send it. If no more data flag that is is ok to send new data
************************************************************************************/
void CsrBtHfHsXStateCmDataCfmHandler(HfMainInstanceData_t *instData)
{
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if ( ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HS) && (linkPtr->state > Activate_s)) ||
         ((linkPtr->linkType == CSR_BT_HF_CONNECTION_HF) && (linkPtr->state != Connect_s)))
    {
        CsrBtCmDataCfm    *cmPrim;
        CsrUint16        eventClass;
        void *            msg;
        HfHsData_t *    data;

        cmPrim = (CsrBtCmDataCfm *) instData->recvMsgP;

        if (linkPtr->data->atResponseTimerId != 0)
        {/* re-start 2 seconds timer if it was running; this is where the command has been
            delivered to the lower layers */
            void *mv;
            CsrUint16 mi;
            CsrSchedTimerCancel(linkPtr->data->atResponseTimerId, &mi, &mv);
        }
        if (linkPtr->lastAtCmdSent == idleCmd)
        {
            linkPtr->data->atResponseTimerId = 0;
        }
        else
        {
            if ((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE) == 0x00000000)
                {
                    linkPtr->data->atResponseTimerId = CsrSchedTimerSet((CsrTime)(instData->atRespWaitTime*1000000),CsrBtHfAtResponseTimeout,
                                                                (CsrUint16)instData->index,(void*)instData);
                }
        }
        data = getDataInstance(instData, cmPrim->btConnId);
        if ((linkPtr->lastAtCmdSent == idleCmd) || (instData->linkData[instData->index].atSequenceState < copsQuery))/*rest))*/
        {/* Allow to send more commands only if ready for it....*/
            if(CsrMessageQueuePop(&data->cmDataReqQueue, &eventClass, &msg))
            {
                if (eventClass == CSR_BT_HF_PRIM)
                {/* This is a request to send an AT command */
                    data->allowed2SendCmData = TRUE;
                    instData->eventClass = eventClass;
                    CsrPmemFree(instData->recvMsgP);
                    instData->recvMsgP = msg;
                    CsrBtHfCommonAtCmdPrimReqHandler(instData,(CsrBtHfPrim *)msg);
                }
                else
                {/* CSR_BT_CM_PRIM */
                    linkPtr->lastAtCmdSent = FindCurrentCmdFromPayload(((CsrBtCmDataReq *)msg)->payload);
                    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE ,eventClass ,msg);
                }
            }
            else
            {
                data->allowed2SendCmData = TRUE;
            }
        }
    }
}

/*************************************************************************************
    Send data to CM
************************************************************************************/
void CsrBtHfHsSendCmDataReq(HfMainInstanceData_t *instData, CsrUint16 payloadLen, CsrUint8 *payload)
{
    HfHsData_t * data;
    CsrUint16 dataSent;
    HfInstanceData_t *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    data = linkPtr->data;
    dataSent = 0;
    while (dataSent < payloadLen)
    {
        CsrBtCmDataReq *cmPrim;

        cmPrim    = CsrPmemAlloc(sizeof(CsrBtCmDataReq));
        cmPrim->type            = CSR_BT_CM_DATA_REQ;
        cmPrim->btConnId        = linkPtr->hfConnId;
        if ((payloadLen - dataSent) <= data->maxRfcFrameSize)
        {
            cmPrim->payloadLength    = (payloadLen - dataSent);
        }
        else
        {
            cmPrim->payloadLength    = data->maxRfcFrameSize;
        }
        cmPrim->payload = CsrPmemAlloc(cmPrim->payloadLength);
        CsrMemCpy(cmPrim->payload, &(payload[dataSent]), cmPrim->payloadLength);
        dataSent = dataSent + cmPrim->payloadLength;;

        if ((data->allowed2SendCmData) && (data->atResponseTimerId == 0))
        {
            CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, cmPrim);
            data->allowed2SendCmData = FALSE;
            /* Now start 2-seconds timer. If no answer is received before the 2 seconds run out,
               create an error answer for the application and allow to send data afterwards */
            if ((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE) == 0x00000000)
                {
                    data->atResponseTimerId = CsrSchedTimerSet((CsrTime)(instData->atRespWaitTime*1000000),CsrBtHfAtResponseTimeout,
                                                        (CsrUint16)instData->index,(void*)instData);
                }
        }
        else
        {
            CsrMessageQueuePush(&data->cmDataReqQueue, CSR_BT_CM_PRIM, cmPrim);
            /* If this is due to race condition between CSR_BT_CM_DATA_IND and CSR_BT_CM_DATA_CFM, make sure to allow
              new messages to be sent */
            if (data->atResponseTimerId == 0)
            {
                linkPtr->lastAtCmdSent = idleCmd;
            }
        }
    }
    CsrPmemFree(payload);
}

void HsHfSecurityInReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtResultCode rval;
    CsrBtHfSecurityInReq *prim;

    prim = (CsrBtHfSecurityInReq*)instData->recvMsgP;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_HANDSFREE_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HANDSFREE_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HF_SUCCESS,
        CSR_BT_RESULT_CODE_HF_UNACCEPTABLE_PARAMETER);

    CsrBtHfSecurityInCfmSend(prim->appHandle,
        rval, CSR_BT_SUPPLIER_HF);
}

void HsHfSecurityOutReqHandler(HfMainInstanceData_t * instData)
{
    CsrBtResultCode rval;
    CsrBtHfSecurityOutReq *prim;

    prim = (CsrBtHfSecurityOutReq*)instData->recvMsgP;

    rval = CsrBtScSetSecOutLevel(&instData->secOutgoing, prim->secLevel,
        CSR_BT_HANDSFREE_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HANDSFREE_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HF_SUCCESS,
        CSR_BT_RESULT_CODE_HF_UNACCEPTABLE_PARAMETER);

    CsrBtHfSecurityOutCfmSend(prim->appHandle,
        rval, CSR_BT_SUPPLIER_HF);
}

void CsrBtHfSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtHfSecurityInCfm *prim;
    prim = (CsrBtHfSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtHfSecurityInCfm));
    prim->type = CSR_BT_HF_SECURITY_IN_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtHfMessagePut(appHandle, prim);
}

void CsrBtHfSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtHfSecurityOutCfm *prim;
    prim = (CsrBtHfSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtHfSecurityOutCfm));
    prim->type = CSR_BT_HF_SECURITY_OUT_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtHfMessagePut(appHandle, prim);
}


