/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #7 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_util.h"
#include "csr_bt_hf_main.h"
#include "csr_bt_hf_util.h"
#include "csr_bt_hf_at_inter.h"
#include "csr_bt_hf_main_sef.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_hfhs_data_sef.h"
#ifdef CSR_USE_DSPM
#include "csr_bt_hf_dspm.h"
#endif

#define TO_UPPER(x)        ((((x) >= 'a') && ((x) <= 'z')) ? ((x) & 0xDF) : (x))
#define IS_DIGIT_CHAR(x)   ((((x) >= '0') && ((x) <= '9')) ? TRUE : FALSE)

/* AT command table structure */
typedef struct
{
    CsrUint8        token;       /* Numeric token */
    char          *at;          /* AT command string */
    HfAtHandler_t  func;        /* AT handler function */
} HfAtCheck_t;

/**********************************************************************
*  Forward declarations: function prototypes
***********************************************************************/
void CsrBtHfSendHfCurrentOperatorSelectionCfm(HfMainInstanceData_t *instData, CsrUint8 *atTextString);

CsrBool CsrBtHfVgmSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfVgsSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCindHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfStoreChldInfo(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfFindCievValue(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfOkHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfErrorHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfBrsfHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfBtrhHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfSendHfClccInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfSendHfCnumInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfRingHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfBsirHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCopsHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCmeErrorHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfSendClipInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfSendCcwaInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfSendBtInputCfm(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfBvraHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfBusyHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfNoCarrierHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfNoAnswerHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfDelayedHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfBlacklistedHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCsrBatSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCsrGetsmsSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCsrNewSmsHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCsrSfSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCsrFnSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCsrTxtHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString);
CsrBool CsrBtHfCodecNegSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString);

static const HfAtCheck_t hfAtCheck[] =
{
    { CSR_BT_VGM_TOKEN,             "+VGM",        CsrBtHfVgmSet          },
    { CSR_BT_VGS_TOKEN,             "+VGS",        CsrBtHfVgsSet          },
    { CSR_BT_CIND_STATUS_TOKEN,     "+CIND:",      CsrBtHfCindHandler     },
    { CSR_BT_CHLD_TOKEN,            "+CHLD:",      CsrBtHfStoreChldInfo   },
    { CSR_BT_CIEV_TOKEN,            "+CIEV:",      CsrBtHfFindCievValue   },
    { CSR_BT_OK_TOKEN,              "OK",          CsrBtHfOkHandler       },
    { CSR_BT_ERROR_TOKEN,           "ERROR",       CsrBtHfErrorHandler    },
    { CSR_BT_BRSF_TOKEN,            "+BRSF:",      CsrBtHfBrsfHandler     },
    { CSR_BT_BTRH_STATUS_TOKEN,     "+BTRH",       CsrBtHfBtrhHandler     },
    { CSR_BT_CMEE_STATUS_TOKEN,     "+CMEERROR",   CsrBtHfCmeErrorHandler },
    { CSR_BT_CCWA_TOKEN,            "+CCWA:",      CsrBtHfSendCcwaInd     },
    { CSR_BT_CLIP_TOKEN,            "+CLIP:",      CsrBtHfSendClipInd     },
    { CSR_BT_CLCC_STATUS_TOKEN,     "+CLCC:",      CsrBtHfSendHfClccInd   },
    { CSR_BT_CNUM_STATUS_TOKEN,     "+CNUM:",      CsrBtHfSendHfCnumInd   },
    { CSR_BT_RING_TOKEN,            "RING",        CsrBtHfRingHandler     },
    { CSR_BT_BSIR_TOKEN,            "+BSIR:",      CsrBtHfBsirHandler     },
    { CSR_BT_COPS_TOKEN,            "+COPS:",      CsrBtHfCopsHandler     },
    { CSR_BT_BVRA_TOKEN,            "+BVRA:",      CsrBtHfBvraHandler     },
    { CSR_BT_BUSY_TOKEN,            "BUSY",        CsrBtHfBusyHandler     },
    { CSR_BT_NO_CARRIER_TOKEN,      "NO CARRIER",  CsrBtHfNoCarrierHandler},
    { CSR_BT_NO_ANSWER_TOKEN,       "NO ANSWER",   CsrBtHfNoAnswerHandler },
    { CSR_BT_DELAYED_TOKEN,         "DELAYED",     CsrBtHfDelayedHandler  },
    { CSR_BT_BLACKLISTED_TOKEN,     "BLACKLISTED", CsrBtHfBlacklistedHandler },
    { CSR_BT_CSR_TOKEN,             "+CSR:",       CsrBtHfCsrSfSet        },
    { CSR_BT_CSR_BAT_TOKEN,         "+CSRBATT?",   CsrBtHfCsrBatSet       },
    { CSR_BT_CSR_GETSMS_TOKEN,      "+CSRGETSMS:", CsrBtHfCsrGetsmsSet    },
    { CSR_BT_CSR_SMS_TOKEN,         "+CSRSMS:",    CsrBtHfCsrNewSmsHandler },
    { CSR_BT_CSR_SF_TOKEN,          "+CSRSF:",     CsrBtHfCsrSfSet        },
    { CSR_BT_CSR_FN_TOKEN,          "+CSRFN:",     CsrBtHfCsrFnSet        },
    { CSR_BT_CSR_TXT_TOKEN,         "+CSRTXT:",    CsrBtHfCsrTxtHandler   },
    { CSR_BT_BINP_TOKEN,            "+BINP:",      CsrBtHfSendBtInputCfm  },
    { CSR_BT_BCS_TOKEN,             "+BCS:",       CsrBtHfCodecNegSet     },
    { CSR_BT_OTHER_TOKEN,           NULL,          NULL                   }
};

/*************************************************************************************
 SendConfirmMessage : Find out what message to give the application and do it
************************************************************************************/
CsrBool SendConfirmMessage(HfMainInstanceData_t  *instData, CsrUint16 resultToSend)
{
    CsrBool retValue = TRUE;
    CsrBtHfPrim type = 0xFFFF;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    switch(linkPtr->lastAtCmdSent)
    {
        case vgm:
        {
            type = CSR_BT_HF_MIC_GAIN_STATUS_CFM;
            break;
        }
        case vgs:
        {
            type = CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM;
            break;
        }
        case bcc:
            /* No need for confirmation here: just break.
               The bcc cmd means the HFG should start the eSCO and codec negotiation;
               the bcs means the HF has acepted the HFG device's codec suggestion and
               waits for the eSCO establishment from the HFG */
            
#ifdef CSR_USE_DSPM
            if (linkPtr->dspmInst.codecAlreadyNegotiated)
#endif
            {
                CsrBtHfAcceptIncomingSco(instData);
            }
            break;
        case bcs:
            {
                linkPtr->lastAtCmdSent = idleCmd;
#ifdef CSR_USE_DSPM
                linkPtr->dspmInst.codecAlreadyNegotiated = TRUE;
                linkPtr->dspmInst.codecNegOngoing = FALSE;
#endif          
                CsrBtHfAcceptIncomingSco(instData);
            }
            /* else, ignore and wait for HFG move. */
            break;

        case bac:
        {
            linkPtr->lastAtCmdSent = idleCmd;
            if ((linkPtr->atSequenceState == codecSupport) && (linkPtr->state == ServiceSearch_s))
            {
                linkPtr->atSequenceState = cindSupport;
                sendCindSupport(instData);
            }
            return retValue;
        }
        case bsrf:
        {
            if ((linkPtr->atSequenceState == supportFeatures) && (linkPtr->state == ServiceSearch_s))
            {
                linkPtr->serviceState = serviceConnect_s;
                if (linkPtr->remoteVersion == 0)
                {/* remote version yet unknown: BRSF not supported means version 0.96....*/
                    linkPtr->remoteVersion = CSR_BT_FIRST_HFP_NO_ESCO;
                }
                linkPtr->lastAtCmdSent = idleCmd;
                if (/* (linkPtr->remoteVersion == CSR_BT_FIRST_HFP_CODEC_NEG_ESCO) &&*/
                    (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) &&
                    (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION))
                {/* Codec negotiation supported locally and by the remote */
                    linkPtr->atSequenceState = codecSupport;
                    sendCodecSupport(instData);
                }
                else
                {
                    linkPtr->atSequenceState = cindSupport;
                    sendCindSupport(instData);
                }
            }
            return retValue;
        }
        case cindStatusCmd:
        {
            linkPtr->lastAtCmdSent = idleCmd;
            type = CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM;

            if ((linkPtr->state == Connected_s) || (linkPtr->state == LpEnabled_s))
            {
                CsrBtHfSendIndicatorsUpdateCfm(instData,resultToSend);
                return retValue;
            }
            break;
        }
        case cnum:
        {
            type = CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM;
            break;
        }
        case cmer:
        {
            type = CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM;
            break;
        }
        case btrh:
        case chld:
        {
            type = CSR_BT_HF_CALL_HANDLING_CFM;
            break;
        }
        case ccwa:
        {
            type = CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM;
            if (linkPtr->pendingSlcXtraCmd)
            {
                linkPtr->lastAtCmdSent = idleCmd;
                if (((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CMEE_ACTIVATION) != 0) ||
                    ((linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES) == 0))
                {/* Finished sending automatic AT commands; send low power mode request
                     if not already in LP mode, and LP request enabled */
                    linkPtr->pendingSlcXtraCmd = FALSE;
                    CsrBtHfLpStart(instData,TRUE);
                }
                else
                {
                    CsrBtHfSendHfHouseCleaning(instData);
                }
                return retValue;
            }
            break;
        }
        case clip:
        {
            type = CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM;
            if (linkPtr->pendingSlcXtraCmd)
            {
                linkPtr->lastAtCmdSent = idleCmd;
                if  (((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CCWA_ACTIVATION) == 0) &&
                        (((linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING) ||
                         (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL)) &&
                        ((linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING) ||
                         (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL))) )
                {
                    CsrBtHfSendHfHouseCleaning(instData);
                }
                else if (((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CMEE_ACTIVATION) == 0) &&
                    (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES))
                {
                    CsrBtHfSendHfHouseCleaning(instData);
                }
                else
                {/* Finished sending automatic AT commands; send low power mode request
                     if not already in LP mode, and LP request enabled */
                    linkPtr->pendingSlcXtraCmd = FALSE;
                    CsrBtHfLpStart(instData,TRUE);
                }
                return retValue;
            }
            break;
        }
        case clcc:
        {
            type = CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM;
            break;
        }
        case cmee:
        {
            type = CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM;
            if (linkPtr->pendingSlcXtraCmd)
            {   /* Finished sending automatic AT commands; send low power mode request
                   if not already in LP mode, and LP request enabled */
                linkPtr->pendingSlcXtraCmd = FALSE;
                linkPtr->lastAtCmdSent = idleCmd;
                CsrBtHfLpStart(instData,TRUE);
                return retValue;
            }
            break;
        }
        case bvra:
        {
            type = CSR_BT_HF_SET_VOICE_RECOGNITION_CFM;
            break;
        }
        case binp:
        {
            CsrBtHfSendBtInputCfm(instData,NULL);
            /*No break intended!*/
        }
        case copsSet:
        { /* Confirmation already sent */
            linkPtr->lastAtCmdSent = idleCmd;
            return retValue;
        }
        case copsQueryCmd:
        {
            CsrBtHfSendHfCurrentOperatorSelectionCfm(instData, NULL);
            linkPtr->lastAtCmdSent = idleCmd;
            return retValue;
        }
        case answer:
        {
            type = CSR_BT_HF_CALL_ANSWER_CFM;
            break;
        }
        case callEnd:
        {
            type = CSR_BT_HF_CALL_END_CFM;
            break;
        }
        case ckpd:
        {
            linkPtr->lastAtCmdSent = idleCmd;
            return retValue;
        }
        case dialNumber:
        case dialMem:
        case redial:
        {
            type = CSR_BT_HF_DIAL_CFM;
            break;
        }
        case nrec:
        {
            type = CSR_BT_HF_SET_ECHO_AND_NOISE_CFM;
            break;
        }
        case vts:
        {
            type = CSR_BT_HF_GENERATE_DTMF_CFM;
            break;
        }
        case c2cSf:
        {
            linkPtr->lastAtCmdSent = idleCmd;
            if (linkPtr->pendingSlcXtraCmd)
            {/* SLC establishment automatic signals being sent... Go on */
                if (((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CLIP_ACTIVATION) == 0) &&
                    (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY))
                {
                    CsrBtHfSendHfHouseCleaning(instData);
                }
                else if  (((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CCWA_ACTIVATION) == 0) &&
                        (((linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING) ||
                         (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL)) &&
                        ((linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING) ||
                         (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL))) )
                {
                    CsrBtHfSendHfHouseCleaning(instData);
                }
                else if (((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CMEE_ACTIVATION) == 0) &&
                    (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES))
                {
                    CsrBtHfSendHfHouseCleaning(instData);
                }
                else
                {/* Finished sending automatic AT commands; send low power mode request
                     if not already in LP mode, and LP request enabled */
                    linkPtr->pendingSlcXtraCmd = FALSE;
                    CsrBtHfLpStart(instData,TRUE);
                }
            }
            return retValue;
            /*no break; needed*/
        }
        case c2cpwr:
        {
            type = CSR_BT_HF_C2C_PWR_CFM;
            break;
        }
        case c2cbatt:
        {
            type = CSR_BT_HF_C2C_BATT_CFM;
            break;
        }
        case c2cSms:
        case c2c:
        {/* No need for confirmation here: just break */
            break;
        }
        case c2cFn:
        {
            linkPtr->lastAtCmdSent = idleCmd;
            if (resultToSend != CSR_BT_CME_SUCCESS)
            {/* If due to 'ERROR'...*/
                linkPtr->scoConnectAcceptPending = TRUE;
                CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                          linkPtr->hfConnId,
                                          linkPtr->audioSetupParams.theAudioQuality,
                                          linkPtr->audioSetupParams.theTxBandwidth,
                                          linkPtr->audioSetupParams.theRxBandwidth,
                                          linkPtr->audioSetupParams.theMaxLatency,
                                          linkPtr->audioSetupParams.theVoiceSettings,
                                          linkPtr->audioSetupParams.theReTxEffort);
                if (linkPtr->outgoingAudio)
                {/*.. and no more trials left: let the application know */
                    CsrBtHfSendHfAudioInd(instData, 0,0,0,0,0,0,0, CSR_BT_RESULT_CODE_HF_C2C_REJECTED, 0xDEAD, CSR_BT_SUPPLIER_HF);
                }
            }
            /* else, ignore and wait for HFG move. */
            return retValue;
        }
        case bia:
        {
            type = CSR_BT_HF_INDICATOR_ACTIVATION_CFM;
            break;
        }
        case other:
        {   type = CSR_BT_HF_AT_CMD_CFM;
            break;
        }
        default: /* Just send the message back to the app as an AT cmd ind */
        {
            retValue = FALSE;
            break;
        }
    }
    linkPtr->lastAtCmdSent = idleCmd;

    if ((type != 0xFFFF) && (retValue))
    {
        CsrBtHfSendHfGeneralCfmMsg(instData,resultToSend,type);
    }

    return retValue;
}

/************************************************************************************
*      Data response timer handling: timeout handler and cancellation
*************************************************************************************/
void CsrBtHfAtResponseTimeout(CsrUint16 mi, void *mv)
{
    HfInstanceData_t *linkPtr;
    HfMainInstanceData_t *inst;
    HfHsData_t  *data;

    inst = (HfMainInstanceData_t*) mv;
    inst->index = (CsrUint8)mi;
    linkPtr = (HfInstanceData_t *)&(inst->linkData[inst->index]);
    data = linkPtr->data;

    /* Timer was fired */
    data->atResponseTimerId = 0;
    /* Now send error response to the application */
    SendConfirmMessage(inst,CSR_BT_CME_AG_FAILURE);

    /* And check whether more messages are waiting to be delivered */
    if ((linkPtr->lastAtCmdSent == idleCmd) || (linkPtr->atSequenceState < copsQuery))
    {/* Now check if there are new messages to send in the queue */
        CsrUint16        eventClass;
        void *            msg;

        if(CsrMessageQueuePop(&data->cmDataReqQueue, &eventClass, &msg))
        {
            if (eventClass == CSR_BT_HF_PRIM)
            {/* This is a request to send an AT command */
                data->allowed2SendCmData = TRUE;
                inst->eventClass = eventClass;
                /*CsrPmemFree(inst->recvMsgP);*/
                inst->recvMsgP = msg;
                CsrBtHfCommonAtCmdPrimReqHandler(inst,(CsrBtHfPrim *)msg);
            }
            else
            {/* CSR_BT_CM_PRIM */
                linkPtr->lastAtCmdSent = FindCurrentCmdFromPayload(((CsrBtCmDataReq *)msg)->payload);
                CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE ,eventClass ,msg);
                data->atResponseTimerId = CsrSchedTimerSet((CsrTime)(inst->atRespWaitTime*1000000),CsrBtHfAtResponseTimeout,
                                                        (CsrUint16)inst->index,(void*)inst);
            }
        }
        else
        {
            data->allowed2SendCmData = TRUE;
        }
    }

}

void cancelAtResponseTimer(HfMainInstanceData_t *instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->data->atResponseTimerId != 0)
    {
        void *mv;
        CsrUint16 mi;
        CsrSchedTimerCancel(linkPtr->data->atResponseTimerId, &mi, &mv);
        linkPtr->data->atResponseTimerId = 0;
    }
}

/*************************************************************************************
 StrATLen : Find Length of AT string without \r\n
************************************************************************************/
CsrUint32 StrATLen(const char *string)
{
    CsrUint32    length = 0;

    if (string != NULL)
    {
        while (*string++ != '\r')
        {
            length++;
        }
    }

    return length;
}

/******************************************************************************************
*           Accept incoming SCO after SLC has been completely established
*******************************************************************************************/
void CsrBtHfAcceptIncomingSco(HfMainInstanceData_t *instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if ((!linkPtr->scoConnectAcceptPending) && (linkPtr->scoHandle == HF_SCO_UNUSED))
    {
        CsrUint16 voiceSettings = linkPtr->audioSetupParams.theVoiceSettings;
        CsrUint32 txBandwidth   = linkPtr->audioSetupParams.theTxBandwidth;
        CsrUint32 rxBandwidth   = linkPtr->audioSetupParams.theRxBandwidth;
        CsrUint16 audioQuality  = linkPtr->audioSetupParams.theAudioQuality;

        if (/* (linkPtr->remoteVersion == CSR_BT_FIRST_HFP_CODEC_NEG_ESCO) &&*/
            (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) &&
            (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION))
        {/* wbs: check the "codecToUse" variable */
            audioQuality   = CSR_BT_HF_WBS_AUDIO_QUALITY; /* 2-EV3 may be used */

            if (linkPtr->codecToUse == CSR_BT_WBS_MSBC_CODEC)
            {/* mSBC: transparent data; 2-EV3 and EV3 may be used; tx-rx-bdwidth = 8000
                max Latency 8 ms or 13 msec (T1 and T2 settings) retransmission effort = 0x02 */
                linkPtr->audioSetupParams.theVoiceSettings |= CSR_BT_AIRCODING_TRANSPARENT_DATA;
                voiceSettings |= CSR_BT_AIRCODING_TRANSPARENT_DATA;
                audioQuality   = linkPtr->audioSetupParams.theAudioQuality  = CSR_BT_HF_WBS_AUDIO_QUALITY;
                linkPtr->audioSetupParams.theMaxLatency                     = CSR_BT_HF_WBS_T2_LATENCY;
            }
            else if (linkPtr->codecToUse == CSR_BT_WBS_CVSD_CODEC)
            {
                voiceSettings  = linkPtr->audioSetupParams.theVoiceSettings = CSR_BT_ESCO_DEFAULT_CONNECT_VOICE_SETTINGS; 
                audioQuality   = linkPtr->audioSetupParams.theAudioQuality  = CSR_BT_ESCO_DEFAULT_CONNECT_AUDIO_QUALITY;
                linkPtr->audioSetupParams.theMaxLatency                     = CSR_BT_ESCO_DEFAULT_CONNECT_MAX_LATENCY;
            }

        }
        linkPtr->scoConnectAcceptPending = TRUE;
        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HF_IFACEQUEUE,
                                  linkPtr->hfConnId,
                                  audioQuality,
                                  txBandwidth,
                                  rxBandwidth,
                                  linkPtr->audioSetupParams.theMaxLatency,
                                  voiceSettings,
                                  linkPtr->audioSetupParams.theReTxEffort);
    }
}
/*************************************************************************************
*   Send CSR_BT_HF_AT_CMD_IND
**************************************************************************************/
void CsrBtHfSendHfAtCmdInd(HfMainInstanceData_t *instData)
{
    CsrBtHfAtCmdInd *prim;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    prim                 = (CsrBtHfAtCmdInd *)CsrPmemAlloc(sizeof(CsrBtHfAtCmdInd));
    prim->type           = CSR_BT_HF_AT_CMD_IND;
    prim->atCmdString    = (CsrCharString *)linkPtr->data->recvAtCmd;
    prim->connectionId   = linkPtr->hfConnId;
    CsrBtHfMessagePut(instData->appHandle, prim);
}
/*************************************************************************************
 whiteSpace : Remove whitespaces from the received message
************************************************************************************/
static CsrBool whiteSpace(CsrUint8 theChar)
{
    CsrBool retVal = FALSE;

    switch (theChar)
    {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            {
                retVal = TRUE;
                break;
            }
    }

    return retVal;
}

/*************************************************************************************
 getByteValue : Return the received value
************************************************************************************/
static CsrBool getByteValue(CsrUint8 **line, CsrUint32 *tmpValue)
{
    CsrBool valueFound = FALSE;
    *tmpValue = 0;

    while ((**line != '\0') && (IS_DIGIT_CHAR(**line)))
    {
        *tmpValue = (CsrUint32)(*tmpValue * 10 + **line - '0');
        (*line)++;
        valueFound = TRUE;
    }

    return valueFound;
}

/*************************************************************************************
*   IsSlcEstablished: ready to send and receive data?
**************************************************************************************/
CsrBool IsSlcEstablished(HfMainInstanceData_t *instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
    {
        if (linkPtr->atSequenceState == rest)
        {
            return TRUE;
        }
    }
    else
    { /* HS_CONNECTION: if we have received data we are connected basically... */
        return TRUE;
    }
    return FALSE;
}

/*************************************************************************************
    Send a CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM to app
************************************************************************************/
void CsrBtHfSendHfCurrentOperatorSelectionCfm(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrBtHfGetCurrentOperatorSelectionCfm    *prim;
    CsrUint8 len = 0;

    if (atTextString != NULL)
    {
        len = (CsrUint8)CsrStrLen((char *)atTextString)+1;
    }

    prim = (CsrBtHfGetCurrentOperatorSelectionCfm *)CsrPmemAlloc(sizeof(CsrBtHfGetCurrentOperatorSelectionCfm));
    prim->type = CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM;
    prim->connectionId = instData->linkData[instData->index].hfConnId;

    prim->copsString = NULL;

    if (atTextString != NULL)
    {
        prim->copsString = CsrPmemAlloc(len);
        CsrStrNCpyZero((char *)prim->copsString,(char *)atTextString,len);
    }
    prim->cmeeResultCode = CSR_BT_CME_SUCCESS;

    CsrBtHfMessagePut(instData->appHandle, prim);
}

/*************************************************************************************
*  Handle OK response: OK_TOKEN
**************************************************************************************/
CsrBool CsrBtHfOkHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
   CsrBool returnValue = FALSE;
   HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

   CSR_UNUSED(atTextString);

    cancelAtResponseTimer(instData);

    if ((linkPtr->state == Connect_s) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS))
    {
        CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
        linkPtr->lastAtCmdSent = idleCmd;
        if (linkPtr->scoHandle != HF_SCO_UNUSED)
        {/* this means that the HS have received a CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM in connect state */
            CsrBtHfSendHfAudioInd(instData, linkPtr->pcmSlot, 0, 0, 0, 0, 0, 0,
                                        CSR_BT_RESULT_CODE_HF_SUCCESS, 0xDEAD, CSR_BT_SUPPLIER_HF);
        }
        else
        {
            linkPtr->state = Connected_s;
        }

        returnValue = TRUE;
    }
    else
    {
        if ((linkPtr->state == ServiceSearch_s) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF))
        {
            linkPtr->lastAtCmdSent = idleCmd;
            if (linkPtr->pendingCancel)
            {/* Just make sure the data pointer is freed and wait for disconnection.... */
                return TRUE;
            }
            if (linkPtr->atSequenceState == supportFeatures)
            {
                linkPtr->serviceState = serviceConnect_s;
                if (/* (linkPtr->remoteVersion == CSR_BT_FIRST_HFP_CODEC_NEG_ESCO) &&*/
                    (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION) &&
                    (instData->localSupportedFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION))
                {/* Codec negotiation supported locally and by the remote */
                    linkPtr->atSequenceState = codecSupport;
                    sendCodecSupport(instData);
                }
                else
                {
                    linkPtr->atSequenceState = cindSupport;
                    sendCindSupport(instData);
                }
                returnValue = TRUE;
            }
            else if (linkPtr->atSequenceState == codecSupport)
            {
                linkPtr->atSequenceState = cindSupport;
                sendCindSupport(instData);
                returnValue = TRUE;
            }
            else if (linkPtr->atSequenceState == cindSupport)
            {
                linkPtr->atSequenceState = cindStatus;
                sendCindStatus(instData);
                returnValue = TRUE;
            }
            else if ((linkPtr->atSequenceState == cindStatus) &&
                    ((instData->mainConfig & CSR_BT_HF_CNF_DISABLE_CMER_UNDER_SLC_ESTABLISHMENT) == 0))
            {
                linkPtr->atSequenceState = eventReport;
                sendSetCmer(instData,TRUE);
                returnValue = TRUE;
            }
            else if ((linkPtr->atSequenceState == eventReport) ||
                ((linkPtr->atSequenceState == cindStatus) && (instData->mainConfig & CSR_BT_HF_CNF_DISABLE_CMER_UNDER_SLC_ESTABLISHMENT)))
            {   /* Finished setting up ServiceConnection */
                linkPtr->atSequenceState = serviceLevel;
                CsrBtHfSendHfHouseCleaning(instData);
                linkPtr->data->dataReceivedInConnected = FALSE;
                /* the link state could have been changed by the HF side while doing the
                   service search. If LP is enabled change to lpenabled state */
                if ( (linkPtr->linkState == CSR_BT_SNIFF_MODE)
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    || (linkPtr->linkState == CSR_BT_PARK_MODE) 
#endif
                    )
                {
                    linkPtr->state = LpEnabled_s;
                }
                else
                {
                    linkPtr->state = Connected_s;
                    linkPtr->atSequenceState = rest;
                }
                /* check if we want to send CHLD command */
                if ( (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING) &&
                     (linkPtr->supportedFeatures & CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING) &&
                     (!(instData->mainConfig & CSR_BT_HF_CNF_DISABLE_CHLD_UNDER_SLC_ESTABLISHMENT)))
                {
                    /* Both HF and HFG supports 3-way calling */
                    linkPtr->atSequenceState = callHold;
                    sendCallHoldStatus(instData);
                }
                else
                {/* It is first now that the SLC is established! */
                    CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    CsrBtHfAcceptIncomingSco(instData);
                }
                returnValue = TRUE;
            }
            else if (linkPtr->atSequenceState == callHold)
            {
                /* Finished setting up ServiceConnection */
                if (linkPtr->disconnectReqReceived)
                {
                    CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
                }
                else
                {
                    linkPtr->atSequenceState = serviceLevel;
                    CsrBtHfSendHfHouseCleaning(instData);
                    linkPtr->data->dataReceivedInConnected = FALSE;
                    /* the link state could have been changed by the HF side while doing the
                       service search. If LP is enabled change to lpenabled state */
                    if ( (linkPtr->linkState == CSR_BT_SNIFF_MODE)
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                        || (linkPtr->linkState == CSR_BT_PARK_MODE) 
#endif
                        )
                    {
                        linkPtr->state = LpEnabled_s;
                    }
                    else
                    {
                        linkPtr->state = Connected_s;
                        linkPtr->atSequenceState = rest;
                    }
                    CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    CsrBtHfAcceptIncomingSco(instData);
                }
                returnValue = TRUE;
            }

        }
        else
        {
            if (linkPtr->atSequenceState == callHold)
            {
                linkPtr->lastAtCmdSent = idleCmd;
                linkPtr->atSequenceState = rest;
                if (linkPtr->disconnectReqReceived)
                {
                    CsrBtCmDisconnectReqSend(linkPtr->hfConnId);
                }
                else
                {
                    CsrBtHfSendHfServiceConnectInd(instData, CSR_BT_RESULT_CODE_HF_SUCCESS, CSR_BT_SUPPLIER_HF);
                    CsrBtHfAcceptIncomingSco(instData);
                }
                returnValue = TRUE;
            }
            else if (linkPtr->atSequenceState == copsQuery)
            {
                linkPtr->lastAtCmdSent = idleCmd;
                CsrBtHfAtCopsQuerySend(instData);
                linkPtr->atSequenceState = rest;
                returnValue = TRUE;
            }
            else
            {
                returnValue = SendConfirmMessage(instData, CSR_BT_CME_SUCCESS);
            }
      }
   }
   return returnValue;
}

/*************************************************************************************
*  Handle ERROR response: ERROR_TOKEN
**************************************************************************************/
CsrBool CsrBtHfCmeErrorHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        cancelAtResponseTimer(instData);
        CsrBtHfSendHfCmeeInd(instData, atTextString);
    }
    return TRUE;
}

/*******************************************************************************************
*  Handle alternative error responses: busy, no carrier, no answer, delayed and blacklisted
*  as specified in version 1.05 of the HFP
********************************************************************************************/
CsrBool CsrBtHfBusyHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CSR_UNUSED(atTextString);
    if (IsSlcEstablished(instData))
    {
        cancelAtResponseTimer(instData);
        SendConfirmMessage(instData,CSR_BT_CME_BUSY);
    }
    return TRUE;
}

CsrBool CsrBtHfNoCarrierHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CSR_UNUSED(atTextString);
    if (IsSlcEstablished(instData))
    {
        cancelAtResponseTimer(instData);
        SendConfirmMessage(instData, CSR_BT_CME_NO_CARRIER);
    }
    return TRUE;
}

CsrBool CsrBtHfNoAnswerHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CSR_UNUSED(atTextString);
    if (IsSlcEstablished(instData))
    {
        cancelAtResponseTimer(instData);
        SendConfirmMessage(instData, CSR_BT_CME_NO_ANSWER);
    }
    return TRUE;
}

CsrBool CsrBtHfDelayedHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CSR_UNUSED(atTextString);
    if (IsSlcEstablished(instData))
    {
        cancelAtResponseTimer(instData);
        SendConfirmMessage(instData, CSR_BT_CME_DELAYED);
    }
    return TRUE;
}

CsrBool CsrBtHfBlacklistedHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CSR_UNUSED(atTextString);
    if (IsSlcEstablished(instData))
    {
        cancelAtResponseTimer(instData);
        SendConfirmMessage(instData, CSR_BT_CME_BLACKLISTED);
    }
    return TRUE;
}

/*************************************************************************************
*  Handle ERROR response: ERROR_TOKEN
**************************************************************************************/
CsrBool CsrBtHfErrorHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrBool returnValue = FALSE;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    CSR_UNUSED(atTextString);

    cancelAtResponseTimer(instData);

    if ((linkPtr->state == ServiceSearch_s) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF))
    {/* Hfg does not support version 1.0, continue AT sequence. Use supported features
        found during service search or default value (saved in instance data) */
        if (linkPtr->atSequenceState == supportFeatures)
        {
            linkPtr->atSequenceState = cindSupport;
            linkPtr->serviceState = serviceConnect_s;
            if (linkPtr->remoteVersion == 0)
            {
                linkPtr->remoteVersion = CSR_BT_FIRST_HFP_NO_ESCO; /* 0.96 */
            }
            sendCindSupport(instData);
            returnValue = TRUE;
        }
        else
        {
            returnValue = SendConfirmMessage(instData,CSR_BT_CME_AG_FAILURE);
        }
    }
    else
    {
        if(linkPtr->atSequenceState == copsQuery)
        {
            CsrBtHfSendHfCurrentOperatorSelectionCfm(instData, atTextString);
            linkPtr->atSequenceState = rest;
            returnValue = TRUE;
        }
        else
        {
            returnValue = SendConfirmMessage(instData,CSR_BT_CME_AG_FAILURE);
        }
    }
    return returnValue;
}

/*************************************************************************************
*  Handle VGS message
**************************************************************************************/
CsrBool CsrBtHfVgsSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrUint32 value = 0;
        CsrUint8 *index_i;
        HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

        atTextString++;/* skip the first charachter; it will be ':' or '=' */

        index_i = atTextString;
        index_i = (CsrUint8 *)CsrStrChr((char *)index_i,' '); /* There might be spaces after the separator charachter */
        if (index_i == NULL)
        {
            index_i = atTextString;
        }
        else
        {/* jump to the next charachter */
            index_i++;
        }

        if (getByteValue(&index_i,&value))
        {
            if (value <= CSR_BT_MAX_VGS_VALUE)
            {
                if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
                {
                    if (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL)
                    {
                        /* only send Indication if the device supports RemoteVolumeControl */
                        CsrBtHfSendHfSpeakerGainInd(instData, (CsrUint8) value);
                    }
                }
                else if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS)
                {
                    if (!(instData->mainConfig & CSR_BT_HF_CNF_DISABLE_REMOTE_VOLUME_CONTROL))
                    {
                        /* only send Indication if the device supports RemoteVolumeControl */
                        CsrBtHfSendHfSpeakerGainInd(instData, (CsrUint8) value);
                    }
                }
            }
        }
    }
    return TRUE;
}

/*************************************************************************************
*  Handle VGM message
**************************************************************************************/
CsrBool CsrBtHfVgmSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrUint32 value = 0;
        CsrUint8 *index_i;
        HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

        atTextString++;/* skip the first charachter; it will be ':' or '=' */

        index_i = atTextString;
        index_i = (CsrUint8 *)CsrStrChr((char *)index_i,' '); /* There might be spaces after the separator charachter */
        if (index_i == NULL)
        {
            index_i = atTextString;
        }
        else
        {/* jump to the next charachter */
            index_i++;
        }

        if (getByteValue(&index_i,&value))
        {
            if (value <= CSR_BT_MAX_VGM_VALUE)
            {
                if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
                {
                    if (linkPtr->localSupportedFeatures & CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL)
                    {
                        /* only send Indication if the device supports RemoteVolumeControl */
                        CsrBtHfSendHfMicGainInd(instData, (CsrUint8) value);
                    }
                }
                else if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS)
                {
                    if (!(instData->mainConfig & CSR_BT_HF_CNF_DISABLE_REMOTE_VOLUME_CONTROL))
                    {
                        /* only send Indication if the device supports RemoteVolumeControl */
                        CsrBtHfSendHfMicGainInd(instData, (CsrUint8) value);
                    }
                }
            }
        }
    }
    return TRUE;
}

/*************************************************************************************
*  Handle BRSF message
**************************************************************************************/
CsrBool CsrBtHfBrsfHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrUint32 value = 0;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (getByteValue(&atTextString,&value))
    {
        linkPtr->supportedFeatures = value;
        if (linkPtr->remoteVersion == 0)
        {/* remote version yet unknown; guess out of the value received. 
            Bit 10 is Codec Negotiation - in V1.6 only
            Bit 6 gives a clue: if it is 1 then at least version 1.05; else version 1.0 */
            if (value & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION)
            {
                linkPtr->remoteVersion = CSR_BT_FIRST_HFP_CODEC_NEG_ESCO; /* 1.06 */
            }
            else if (value & CSR_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS)
            {
                linkPtr->remoteVersion = CSR_BT_FIRST_HFP_ESCO; /* 1.05 */
#ifdef CSR_USE_DSPM
                linkPtr->codecToUse = CSR_BT_WBS_CVSD_CODEC;
#endif
            }
            else
            {
                linkPtr->remoteVersion = CSR_BT_LAST_HFP_NO_ESCO; /* 1.00 */
#ifdef CSR_USE_DSPM
                linkPtr->codecToUse = CSR_BT_WBS_CVSD_CODEC;
#endif

            }
        }
#ifdef CSR_USE_DSPM
        if (!(value & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION))
            {
                linkPtr->codecToUse = CSR_BT_WBS_CVSD_CODEC;
            }
#endif

        /* Make sure that the network attribute has the proper value; must be the same as bit 5 of the BRSF value received.
           Per default it is set to 1 before service search in order to follow the spec....*/
        if (value & CSR_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL)
        {
            linkPtr->network = 1;
        }
        else
        {
            linkPtr->network = 0;
        }
    }

    return TRUE;
}

/*************************************************************************************
*  Handle BTRH message
**************************************************************************************/
CsrBool CsrBtHfBtrhHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfSendHfCallHandlingInd(instData, atTextString);
    }
    return TRUE;
}

/*************************************************************************************
*  Handle RING message
**************************************************************************************/
CsrBool CsrBtHfRingHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CSR_UNUSED(atTextString);
    if (IsSlcEstablished(instData))
    {
        CsrBtHfSendHfRingInd(instData);
    }
    return TRUE;
}

/*************************************************************************************
*  Handle BSIR message
**************************************************************************************/
CsrBool CsrBtHfBsirHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrUint32 value = 0;

        if (getByteValue(&atTextString,&value))
        {
            CsrBtHfSendHfInBandRingToneInd(instData, (CsrBool) value);
        }
    }
    return TRUE;
}

/*************************************************************************************
*  Handle COPS message
**************************************************************************************/
CsrBool CsrBtHfCopsHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CsrBtHfSendHfCurrentOperatorSelectionCfm(instData, atTextString);
    instData->linkData[instData->index].atSequenceState = rest;
    return TRUE;
}

/*************************************************************************************
    Send a HF_CLCC_IND to app
************************************************************************************/
CsrBool CsrBtHfSendHfClccInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfGetCurrentCallListInd *prim;
        CsrUint8 len = (CsrUint8)CsrStrLen((char *)atTextString)+1;

        prim = (CsrBtHfGetCurrentCallListInd *)CsrPmemAlloc(sizeof(CsrBtHfGetCurrentCallListInd));
        prim->type = CSR_BT_HF_GET_CURRENT_CALL_LIST_IND;
        prim->connectionId = instData->linkData[instData->index].hfConnId;
        prim->clccString = CsrPmemAlloc(len);
        CsrStrNCpyZero((char *)prim->clccString,(char *)atTextString,len);

        CsrBtHfMessagePut(instData->appHandle, prim);
    }

    return TRUE;
}

/*************************************************************************************
    Send a CSR_BT_HF_CALL_NOTIFICATION_IND to app
************************************************************************************/
CsrBool CsrBtHfSendClipInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfCallNotificationInd *prim;
        CsrUint8 len = (CsrUint8)CsrStrLen((char *)atTextString)+1;

        prim = (CsrBtHfCallNotificationInd*)CsrPmemAlloc(sizeof(CsrBtHfCallNotificationInd));
        prim->type = CSR_BT_HF_CALL_NOTIFICATION_IND;
        prim->connectionId = instData->linkData[instData->index].hfConnId;
        prim->clipString = CsrPmemAlloc(len);
        CsrStrNCpyZero((char *)prim->clipString,(char *)atTextString,len);

        CsrBtHfMessagePut(instData->appHandle, prim);
    }
    return TRUE;
}


/*************************************************************************************
    Send a CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND to app
************************************************************************************/
CsrBool CsrBtHfSendCcwaInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfCallWaitingNotificationInd *prim;
        CsrUint8 len = (CsrUint8)CsrStrLen((char *)atTextString)+1;

        prim = (CsrBtHfCallWaitingNotificationInd*)CsrPmemAlloc(sizeof(CsrBtHfCallWaitingNotificationInd));
        prim->type = CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND;
        prim->connectionId = instData->linkData[instData->index].hfConnId;
        prim->ccwaString = CsrPmemAlloc(len);
        CsrStrNCpyZero((char *)prim->ccwaString,(char *)atTextString,len);

        CsrBtHfMessagePut(instData->appHandle, prim);
    }
    return TRUE;
}

/*************************************************************************************
    Send a CSR_BT_HF_BT_INPUT_CFM to app
************************************************************************************/
CsrBool CsrBtHfSendBtInputCfm(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfBtInputCfm *prim;
        CsrUint8 len = 1;

        if (atTextString != NULL)
        {
          len += (CsrUint8)CsrStrLen((char *)atTextString);
        }

        prim = (CsrBtHfBtInputCfm*)CsrPmemAlloc(sizeof(CsrBtHfBtInputCfm));
        prim->type = CSR_BT_HF_BT_INPUT_CFM;
        prim->connectionId = instData->linkData[instData->index].hfConnId;
        prim->cmeeResultCode = CSR_BT_CME_SUCCESS;
        if (len > 1)
        {
            prim->dataRespString = CsrPmemAlloc(len);
            CsrStrNCpyZero((char *)prim->dataRespString,(char *)atTextString,len);
        }
        else
        {
            prim->dataRespString = NULL;
        }

        CsrBtHfMessagePut(instData->appHandle, prim);
    }
    return TRUE;
}

/*************************************************************************************
    Send a CSR_BT_HF_SET_VOICE_RECOGNITION_IND to app
************************************************************************************/
CsrBool CsrBtHfBvraHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrUint32 value = 0;

        if (getByteValue(&atTextString,&value))
        {
            CsrBtHfSetVoiceRecognitionInd *prim;

            prim = (CsrBtHfSetVoiceRecognitionInd*)CsrPmemAlloc(sizeof(CsrBtHfSetVoiceRecognitionInd));
            prim->type = CSR_BT_HF_SET_VOICE_RECOGNITION_IND;
            prim->connectionId = instData->linkData[instData->index].hfConnId;
            if (value!=0)
            {
                prim->started = TRUE;
            }
            else
            {
                prim->started = FALSE;
            }

            CsrBtHfMessagePut(instData->appHandle, prim);
        }
    }
    return TRUE;
}

/*************************************************************************************
    Send a HF_CNUM_IND to app
************************************************************************************/
CsrBool CsrBtHfSendHfCnumInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfGetSubscriberNumberInformationInd    *prim;
        CsrUint8 len = (CsrUint8)CsrStrLen((char *)atTextString)+1;

        prim = (CsrBtHfGetSubscriberNumberInformationInd *)CsrPmemAlloc(sizeof(CsrBtHfGetSubscriberNumberInformationInd));
        prim->type = CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND;
        prim->connectionId = instData->linkData[instData->index].hfConnId;
        prim->cnumString = CsrPmemAlloc(len);
        CsrStrNCpyZero((char *)prim->cnumString,(char *)atTextString,len);

        CsrBtHfMessagePut(instData->appHandle, prim);
    }
    return TRUE;
}

/*************************************************************************************
    Send a HF_CMEE_IND to app
************************************************************************************/
void CsrBtHfSendHfCmeeInd(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    char *index_i;
    char *index_j;

    index_i = (char *)atTextString;
    index_i = CsrStrChr(index_i,':');

    if(index_i != NULL)
    {
        if(index_i[1] == ' ')
        {
            index_i++;
        }
    }
    if(index_i!=NULL)
    {
        CsrUint8 length;
        CsrUint8 *temp;
        CsrUint16 error = 0;

        index_j = CsrStrChr(index_i,'\r');
        length = (CsrUint8)(StrATLen(index_i+1) - StrATLen(index_j));
        temp = CsrPmemAlloc(length);
        CsrMemCpy(temp,index_i+1,length);

        if(length == 1)
        {
            error = *temp - '0';
        }
        else if(length == 2)
        {
            error = (temp[0] - '0')*10 + (temp[1] - '0');
        }

        CsrPmemFree(temp);

        if(error != 0xFFFF) /*only send message if error is decoded*/
        {
            SendConfirmMessage(instData,error);
        }
    }
    instData->linkData[instData->index].lastAtCmdSent = idleCmd;
}

/*************************************************************************************
    Store HF_CHLD_IND information in the link instance for later use
************************************************************************************/
CsrBool CsrBtHfStoreChldInfo(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
    {
        CsrUint16 length;
        char    * chldStart;

        chldStart = (char *)atTextString;
        while (*chldStart != '(')
        {
            chldStart++;
        }
        length = 0;
        while ((*(chldStart + length)) != ')')
        {
            length++;
        }
        length++;

        linkPtr->chldStringStored = CsrPmemAlloc(length+1);

        CsrStrNCpyZero((char *)linkPtr->chldStringStored,chldStart,length+1);
    }
    return TRUE;
}

/*************************************************************************************
    Send a CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND to app
*************************************************************************************/
static void csrBtHfSendStatusIndicatorUpdateInd(HfMainInstanceData_t *instData,
                          CsrUint32               theIndex,
                          CsrUint32               theValue)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (linkPtr->cindSupportString != NULL)
    {
        CsrBtHfStatusIndicatorUpdateInd *prim;
        CsrUint32       x=0, cindSupportStringLen;
        CsrUint16       searchIndex;
        CsrUint8       *startName;
        CsrUint8        lengthName;
        CsrBool         startFound, stop;


        prim = (CsrBtHfStatusIndicatorUpdateInd *)CsrPmemAlloc(sizeof(CsrBtHfStatusIndicatorUpdateInd));
        prim->type  = CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND;
        prim->value = (CsrUint8)theValue;
        prim->connectionId = linkPtr->hfConnId;
        prim->index = (CsrUint8)theIndex;

        startName   = linkPtr->cindSupportString - 1;
        lengthName  = 0;
        searchIndex = 0;

        cindSupportStringLen = CsrStrLen((char *)linkPtr->cindSupportString);

        while ((x<cindSupportStringLen) && (searchIndex != theIndex))
        {
            searchIndex++;
            stop       = FALSE;
            startFound = FALSE;
            startName  = (startName + lengthName +1);
            lengthName = 0;
            while ((!stop) && (x<cindSupportStringLen))
            {
                x++;
                if (startFound)
                {
                    lengthName++;
                    if (*(startName + lengthName) == '"')
                    {
                        stop = TRUE;
                    }
                }
                else
                {
                    if (*startName == '"')
                    {
                        startFound = TRUE;
                        startName++;
                    }
                    else
                    {
                        startName++;
                    }
                }
            }
        }

        if (x<cindSupportStringLen)
        {
            prim->name = CsrPmemAlloc(lengthName+1);
            CsrMemCpy(prim->name, startName, lengthName);
            prim->name[lengthName] = '\0';
            CsrBtHfMessagePut(instData->appHandle, prim);
        }
        else
        {/* If not sent, remember to free the heap area! */
            CsrPmemFree(prim);
        }
    }
}

void saveSupportString(HfMainInstanceData_t * instData, CsrUint8 *line)
{
    CsrUint8 * cindStart;
    CsrUint32 cindLength;
    CsrBool stop = FALSE, temp = TRUE;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    cindLength = 0;
    cindStart = line;

    do
    {
        if (*line == '"')
        {
            temp = temp ? FALSE : TRUE;
        }
        line++;
        cindLength++;
        if (CsrIsSpace(*line))
        {
            if (temp)
            {
                stop = TRUE;
            }
        }
    } while (!stop);
    /* allocate memory for support string + \0 */
    linkPtr->cindSupportString = CsrPmemAlloc(cindLength + 1);
    CsrMemCpy(linkPtr->cindSupportString, cindStart, cindLength);
    *(linkPtr->cindSupportString + cindLength) = '\0';
}

CsrBool CsrBtHfFindCievValue(HfMainInstanceData_t *instData, CsrUint8  *atTextString)
{
    CsrUint8 * cindString;
    CsrUint8 valueLength;
    CsrUint32 valueIndex, value;

    cindString = atTextString;

    /* skip initial spaces */
    while (CsrIsSpace(*cindString))
    {
        cindString++;
        (*atTextString)++;
    }
    valueLength = 0;
    while (*(cindString + valueLength) != ',')
    {
        valueLength++;
    }

    *(cindString + valueLength) = '\0';
    if (getByteValue(&cindString, &valueIndex))
    {
        cindString++;
        (*atTextString)++;
    }
    /* Reset valueLength before reading next value */
    valueLength = 0;
    while (*(cindString + valueLength) != '\r')
    {
        valueLength++;
    }
    *(cindString + valueLength) = '\0';
    if (getByteValue(&cindString, &value))
    {
        cindString++;
        (*atTextString)++;
    }
    csrBtHfSendStatusIndicatorUpdateInd(instData, valueIndex, value);

    return TRUE;
}

void findCievStatus(HfMainInstanceData_t * instData,CsrUint8 *line)

{
    CsrUint8 *cindString;
    CsrBool   stop = FALSE, temp = TRUE;
    CsrUint16 cindValLength = 0;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    cindString = line;

    do
    {
        if (*line == '"')
        {
            temp = temp ? FALSE : TRUE;
        }
        line++;
        cindValLength++;
        if (CsrIsSpace(*line))
        {
            if (temp)
            {
                stop = TRUE;
            }
        }
    } while (!stop);
    /* allocate memory for support string + \0 */
    CsrPmemFree(linkPtr->cindStartValueString);
    linkPtr->cindStartValueString = CsrPmemAlloc(cindValLength + 1);
    CsrMemCpy(linkPtr->cindStartValueString, cindString, cindValLength);
    *(linkPtr->cindStartValueString + cindValLength) = '\0';
}

CsrBool CsrBtHfCindHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    if (*atTextString == '(')
    { /* CIND_SUPPORT_TOKEN */
        if (linkPtr->disconnectReqReceived == FALSE && instData->deactivated == FALSE)
        {
            saveSupportString(instData, atTextString);
        }
    }
    else
    {/*CIND_STATUS_TOKEN*/
        if (linkPtr->disconnectReqReceived == FALSE && instData->deactivated == FALSE)
        {
            findCievStatus(instData, atTextString);
        }
    }
    return TRUE;
}

/*****************************************************************************************
  handle +CSRBATT response code
*****************************************************************************************/
CsrBool CsrBtHfCsrBatSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfSendHfC2CBattInd(instData);
    }
    return TRUE;
}

/*****************************************************************************************
  handle +CSRGETSMS response code
*****************************************************************************************/
CsrBool CsrBtHfCsrGetsmsSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfSendHfC2CGetSMSCfm(instData, atTextString);
    }
    return TRUE;
}

/*****************************************************************************************
  handle +CSRSMS response code
*****************************************************************************************/
CsrBool CsrBtHfCsrNewSmsHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrUint32 idx = 0;
        CsrUint8 *index_i;

        index_i = atTextString;
        index_i = (CsrUint8 *)CsrStrChr((char *)index_i,' '); /* There might be spaces after the separator charachter */
        if (index_i == NULL)
        {
            index_i = atTextString;
        }
        else
        {/* jump to the next charachter */
            index_i++;
        }
        if (getByteValue(&index_i,&idx))
        {
            CsrBtHfSendHfC2CSmsInd(instData,(CsrUint8)idx);
        }
    }
    return TRUE;
}

/*****************************************************************************************
  handle +BCS response code
*****************************************************************************************/
CsrBool CsrBtHfCodecAcceptable(HfMainInstanceData_t *instData)
{
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
    CsrBool retValue = FALSE;
    CsrUint32 mask =  (1 << (linkPtr->codecToUse - 1));

    if (mask & instData->supportedCodecsMask)
    {
        retValue = TRUE;
    }
    return retValue;
}


CsrBool CsrBtHfCodecNegSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    /* This means the codec needs negotiating: can cel accept of SCO connection until the code to use is agreed upon */

    if (IsSlcEstablished(instData))
    {
        CsrUint8 *index_i;
        HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);
        CsrBool newAcceptSettings = FALSE;

        index_i = atTextString;
        index_i = (CsrUint8 *)CsrStrChr((char *)index_i,' '); /* There might be spaces after the separator charachter */
        if (index_i == NULL)
        {
            index_i = atTextString;
        }
        else
        {/* jump to the next charachter */
            index_i++;
        }
        
        if (linkPtr->codecToUse != (CsrUint8)CsrStrToInt((char*)index_i))
        {
            newAcceptSettings = TRUE;
        }
        linkPtr->codecToUse = (CsrUint8)CsrStrToInt((char*)index_i);
        /* if the codec suggestion from the HFG is acceptable, answer with AT+BCS= "<codecId>";
           else, answer with AT+BAC="<codec1>","<codec2>",... */
        if (CsrBtHfCodecAcceptable(instData))
        {/* AT+BCS; accept */
            
#ifdef CSR_USE_DSPM
            if (newAcceptSettings)
            {/*if we are using the DSP Manager however, we need to ensure that the DSP can run the codec - create operators*/
                linkPtr->dspmInst.codecNegOngoing = TRUE;
                if ((linkPtr->dspmInst.operatorEncoder == HF_DSPM_DEFAULT_HANDLE) || (linkPtr->scoHandle != HF_SCO_UNUSED))
                {/* Either there is an eSCO connection ongoing or no operators exist: just allocate the new operators and store them in the proper instance field */
                    CsrBtHfDspmSendCreateOperators(instData);
                }
                else 
                {/* No eSCO exists, but there are operators allocated which are not the ones that shall be used for the next SCO:
                    1. Allocate the new operators and store them as "next" operators
                    2. Destroy the allocated operators, which will automatically set the "next" operators to be the "current" operators
                    */
                    CsrBtHfDspmSendCreateOperators(instData);
                    linkPtr->dspmInst.opSwap =TRUE;
                }
                /* In both cases, the At+BCS will be sent once the operators are created */
            }
            else
            {
                CsrBtHfSendAtBcs(instData);
            }
#else
            CsrBtHfSendAtBcs(instData);
#endif

            if (newAcceptSettings)
            {/* Make sure to cancel accept of old settings and start accepting the new ones */
                CsrBtCmScoCancelReqSend(CSR_BT_HF_IFACEQUEUE, linkPtr->hfConnId);
                linkPtr->scoConnectAcceptPending = FALSE;
            }
            /* Codec agreed upon: accept incoming SCO connection */
            CsrBtHfAcceptIncomingSco(instData);
        }
        else
        {/* AT+BAC; codec not acceptable */
            sendCodecSupport(instData);
        }
    }
    return TRUE;
}


/*****************************************************************************************
  handle +CSRTXT response code
*****************************************************************************************/
CsrBool CsrBtHfCsrTxtHandler(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrBtHfSendHfC2CTxtInd(instData,atTextString);
    }
    return TRUE;
}

/*****************************************************************************************
  handle +CSRSF response code
*****************************************************************************************/
CsrBool CsrBtHfCsrSfSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    if (IsSlcEstablished(instData))
    {
        CsrUint32 value = 0;
        CsrUint8 *index_i;
        CsrUintFast8 i;
        HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

        index_i = atTextString;
        index_i = (CsrUint8 *)CsrStrChr((char *)index_i,' '); /* There might be spaces after the separator charachter */
        if (index_i == NULL)
        {
            index_i = atTextString;
        }
        else
        {/* jump to the next charachter */
            index_i++;
        }

        for (i=1;((i < CSR_BT_C2C_NUMBER_OF_INDICATORS) && (index_i != NULL));i++)
        {
            if (getByteValue(&index_i,&value))
            {
                linkPtr->linkInd.remC2C[i] = (CsrUint8)value;
            }
            index_i = (CsrUint8 *)CsrStrChr((char *)index_i,',');
            if (index_i != NULL)
            {/* move to next charachter */
                index_i++;
            }
        }
        linkPtr->linkInd.remoteSupport = TRUE;
        CsrBtHfSendHfC2CSfInd(instData,atTextString);
    }
    return TRUE;
}

/*****************************************************************************************
  handle +CSRFN response code
*****************************************************************************************/
CsrBool CsrBtHfCsrFnSet(HfMainInstanceData_t *instData, CsrUint8 *atTextString)
{
    CSR_UNUSED(instData);
    CSR_UNUSED(atTextString);
    return TRUE;
}

/*****************************************************************************************
  handle the AT-command string and take appropriate action.
*****************************************************************************************/

/* Local: AT compare function. Perform a very safe comparison of the
 * atStr against maStr, with full range checking etc. Set 'index' to
 * the first occurrence of any trailing non-whitespace
 * (ie. values). */
static CsrBool csrBtHfCompare(char *atStr, CsrUint16 atLen, char *maStr, CsrUint16 *index)
{
    CsrUint16 atIdx;
    CsrUint16 maIdx;
    CsrUint16 maLen;

    /* Dummy checks*/
    if((atStr == NULL) ||
       (maStr == NULL) ||
       (index == NULL) ||
       (atLen < CsrStrLen(maStr)))
    {
        return FALSE;
    }

    atIdx = 0;
    maIdx = 0;
    *index = 0;
    maLen = (CsrUint16)CsrStrLen(maStr);

    /* Scan characters */
    while((maIdx < maLen) &&
          (atIdx < atLen))
    {
        /* Skip whitespace */
        while((atStr[atIdx] == ' ') ||
           (atStr[atIdx] == '\n') ||
           (atStr[atIdx] == '\r') ||
           (atStr[atIdx] == '\t'))
        {
            atIdx++;
        }

        /* Does AT command match? */
        if(TO_UPPER(atStr[atIdx]) != maStr[maIdx])
        {
            /* Mismatch, break out now */
            return FALSE;
        }

        /* Next set of character */
        atIdx++;
        maIdx++;
    }

    /* Skip any last whitespaces */
    while((atIdx < atLen) &&
          ( (atStr[atIdx] == ' ') ||
            (atStr[atIdx] == '\n') ||
            (atStr[atIdx] == '\r') ||
            (atStr[atIdx] == '\t')) )
    {
        atIdx++;
    }

    /* If index has reached the end, it's an actual match */
    if(maIdx == maLen)
    {
        *index = atIdx;
        return TRUE;
    }
    else
    {
        *index = 0;
        return FALSE;
    }
}

/*******************************************************************
 * Local: Fault-tolerant parsing of raw ATs to get token - 'at' and
 * 'atLen' is the AT input string and length. At exit 'index' is the
 * index of the first parameter (unless OTHER_TOKEN is returned)
 *******************************************************************/
static CsrUint8 csrBtHfGetIndex(char *at,CsrUint16 atLen, CsrUint16 *index)
{
    CsrUint8 i;

    /* Scan the AT command table for a token */
    i = 0;
    while(hfAtCheck[i].at != NULL)
    {
        if(csrBtHfCompare(at,atLen,hfAtCheck[i].at,index))
        {
            break;
        }
        i++;
    }

    /* The 'i' will always be valid at this point */
    return i;
}

/*******************************************************************
 * This is the function that receives the actual raw AT command.  Find
 * out if token index and call the appropiate handle if necessary
 *******************************************************************/
CsrBool CsrBtHfHandleAtCommand(HfMainInstanceData_t *instData, CsrUint32  theConnectionId,
                         CsrUint8  *handledAtCmd, CsrUint16  handledAtCmdSize)
{
    CsrUint16 voff = 0;
    CsrUint8 index;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    /*If in Transparent mode, send all commands up to application */
    if((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE)==CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE)
        {
            /* Transparent AT mode - send directly upstream to app now */
                CsrBtHfAtCmdInd *prim;
                prim                 = (CsrBtHfAtCmdInd *)CsrPmemAlloc(sizeof(CsrBtHfAtCmdInd));
                prim->type           = CSR_BT_HF_AT_CMD_IND;
                prim->atCmdString    = CsrPmemAlloc(handledAtCmdSize+1);
                CsrMemCpy((CsrUint8 *)prim->atCmdString,handledAtCmd,handledAtCmdSize);
                prim->atCmdString[handledAtCmdSize] = '\0';
                prim->connectionId   = linkPtr->hfConnId;
                CsrBtHfMessagePut(instData->appHandle, prim);
                /*Application needs to */
                linkPtr->lastAtCmdSent = idleCmd;

        }
    else
    {
        index = csrBtHfGetIndex((char*)handledAtCmd,handledAtCmdSize,&voff);
        /* Execute command if known */
        if ((hfAtCheck[index].token == CSR_BT_OTHER_TOKEN) ||
            ((hfAtCheck[index].func(instData, (CsrUint8 *)&handledAtCmd[voff])) == FALSE) )
          { /* Note that "voff" is the char index of the first AT
             * argument. Pass the address in order to save a lot of
             * variables in the AT handlers... */
             if (IsSlcEstablished(instData))
             {/* Forward the unrecognized AT command to the application only if the SLC is established */
                CsrBtHfAtCmdInd *prim;

                prim                 = (CsrBtHfAtCmdInd *)CsrPmemAlloc(sizeof(CsrBtHfAtCmdInd));
                prim->type           = CSR_BT_HF_AT_CMD_IND;
                prim->atCmdString    = CsrPmemAlloc(handledAtCmdSize+1);
                CsrMemCpy((CsrUint8 *)prim->atCmdString,handledAtCmd,handledAtCmdSize);
                prim->atCmdString[handledAtCmdSize] = '\0';
                prim->connectionId   = linkPtr->hfConnId;
                CsrBtHfMessagePut(instData->appHandle, prim);
             }
           }
    }
    return TRUE;
}

void CsrBtHfHandleMultipleAtCommand(HfMainInstanceData_t *instData, CsrUint32 theConnectionId)
{
    CsrUintFast16 i        = 0;
    CsrUint16     beginPos = 0;
    CsrUint16     endPos   = 0;
    CsrBool       handledSignal         = FALSE;
    CsrUint16     handledAtCmdSize      = 0;
    CsrUint8     *handledAtCmd          = NULL;
    HfHsData_t  *data                  = NULL;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);


    data = linkPtr->data;

    if (data)
    {
        CsrBool deleteAtString = TRUE;

        for ( i = 0; i < data->recvAtCmdSize; i++)
        {
            handledSignal = FALSE;

            /* The AT-parsing below is done in this way, to be most compliant with other devices out on the market.
               We are aware that the specification says that an AT command SHALL start and end with a \r\n,
               but fact is that not all devices out there follows this rule, and hence we have made the AT-parser
               generic enough to also cater for those devices. */

            /* scan the received command sequence to see if a command
               is received (cr or lf must be found) */
            if ((data->recvAtCmd[i] == '\r') || (data->recvAtCmd[i] == '\n'))
            {
                /* Found a delimiter */
                beginPos = endPos;
                endPos = (CsrUint16)i;
                if (endPos-beginPos > 1)
                {
                    /* Found something between the delimiters */
                    CsrUintFast16 x;
                    CsrBool foundNonWhiteSpace = FALSE;
                    for (x=beginPos+1; x<endPos; x++)
                    {
                        /* Check if the found stuff is different from only whitespaces */
                        if (whiteSpace(data->recvAtCmd[x]) == FALSE)
                        {
                            /* Found a non-white-space character */
                            foundNonWhiteSpace = TRUE ;
                        }
                    }
                    if (foundNonWhiteSpace == TRUE)
                    {
                        /* Create an AT-specification correct AT command to pass on in the system */
                        CsrUint16 startCopyPos = beginPos;

                        while ( ( *(data->recvAtCmd + startCopyPos) == '\r' ) ||
                             ( *(data->recvAtCmd + startCopyPos) == '\n' ) )
                        {
                            startCopyPos++;
                        };
                        handledSignal    = TRUE;
                        if (startCopyPos > beginPos)
                        {
                            handledAtCmdSize = endPos - beginPos + 3;
                        }
                        else
                        {/* We need one more byte of space as we shall copy one more byte into the heap area allocated */
                            handledAtCmdSize = endPos - beginPos + 4;
                        }
                        handledAtCmd     = (CsrUint8 *) CsrPmemAlloc(handledAtCmdSize+1);

                        handledAtCmd[0]  = '\r';
                        handledAtCmd[1]  = '\n';
                        CsrMemCpy((handledAtCmd + 2), (data->recvAtCmd + startCopyPos), handledAtCmdSize - 4);
                        handledAtCmd[handledAtCmdSize - 2] = '\r';
                        handledAtCmd[handledAtCmdSize - 1] = '\n';
                        handledAtCmd[handledAtCmdSize] = '\0';
                    }
                }
            }

            if (handledSignal)
            {
                deleteAtString = CsrBtHfHandleAtCommand(instData, theConnectionId, handledAtCmd, handledAtCmdSize);
            }
            if ((handledAtCmd != NULL) && (deleteAtString == TRUE))
            {
                CsrPmemFree(handledAtCmd);
                handledAtCmd = NULL;
            }
        }
        CsrPmemFree(data->recvAtCmd);
        data->recvAtCmd = NULL;

        if ((linkPtr->lastAtCmdSent == idleCmd) || (linkPtr->atSequenceState < copsQuery))
        {/* Now check if there are new messages to send in the queue */
            CsrUint16        eventClass;
            void *            msg;

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
                    if ((instData->mainConfig & CSR_BT_HF_AT_MODE_TRANSPARENT_ENABLE) == 0x00000000)
                        {
                            linkPtr->data->atResponseTimerId = CsrSchedTimerSet((CsrTime)(instData->atRespWaitTime*1000000),CsrBtHfAtResponseTimeout,
                                                        (CsrUint16)instData->index,(void*)instData);
                        }

                }
            }
            else
            {
                data->allowed2SendCmData = TRUE;
            }
        }
    }
}

