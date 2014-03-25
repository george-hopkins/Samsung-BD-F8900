/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"


/* Local: SLC setup completed for HFG */
void csrBtHfgSlcHfgDone(HfgInstance_t *inst)
{
    inst->linkType = CSR_BT_HFG_CONNECTION_HFG;
    inst->lpMode = CSR_BT_LINK_STATUS_CONNECTED;
    HFG_CHANGE_STATE(inst->state,Connected_s);
    HFG_CHANGE_STATE(inst->oldState,ServiceSearch_s);

    /* The HFG is at this late point actually "connected", so call the
     * common connection-completed handler to sync the connection/sds
     * records etc. */
    CsrBtHfgLinkConnectSuccess(inst);

    CsrBtHfgSendHfgServiceConnectInd(inst, CSR_BT_RESULT_CODE_HFG_SUCCESS, CSR_BT_SUPPLIER_HFG);
    CsrBtHfgMainSendHfgHouseCleaning(CsrBtHfgGetMainInstance(inst));
}

static void csrBtBuildCodecMask(HfgInstance_t *inst, CsrUint8 *codecStr)
{
    switch ( (CsrUint8)CsrStrToInt((char*)codecStr) )
    {
        case CSR_BT_WBS_CVSD_CODEC:
            {
                inst->remSupportedCodecs |= CSR_BT_WBS_CVSD_CODEC_MASK;
                break;
            }
        case CSR_BT_WBS_MSBC_CODEC:
            {
                inst->remSupportedCodecs |= CSR_BT_WBS_MSBC_CODEC_MASK;
                break;
            }
        default:
            break;
    }
}

/* Local: Scan AT string and return next parameter as a separate,
 * newly allocated entity. Updates index to point to next parameter if
 * possible, otherwise set to end so next call will yield NULL */
static CsrUint8 *csrBtHfgGetArgument(CsrUint8 *atStr, CsrUint16 atLen, CsrUint16 *index)
{
    CsrUint16 begin;
    CsrUint16 length;
    CsrBool instr;
    CsrUint8 *res;

    if((atStr == NULL) ||
       (atLen < 1) ||
       (index == NULL))
    {
        return NULL;
    }

    begin = 0xFFFF;
    length = 0;
    instr = FALSE;
    res = NULL;

    /* Scan until end of string reached */
    while(*index < atLen)
    {
        if(begin == 0xFFFF)
        {
            /* Find beginning */
            if((atStr[*index] == ' ') ||
               (atStr[*index] == '\n') ||
               (atStr[*index] == '\r') ||
               (atStr[*index] == '\t') ||
               (atStr[*index] == '(') )
            {
                /* Skip whitespace, skip */
                (*index)++;
            }
            else
            {
                if (atStr[*index] == '\0')
                {/* Null terminator is unconditional stop */
                    break;
                }
                else
                {
                    begin = *index;
                    length++;
                    (*index)++;
                    if(atStr[*index] == '\"')
                    {
                        instr = TRUE;
                    }
                }
            }
            continue;
        }

        if(atStr[*index] == '\0')
        {
            /* Null terminator is unconditional stop */
            break;
        }
        if(instr)
        {
            /* We're in a string */
            if((atStr[*index] == '\"') &&
               (atStr[*index - 1] != '\\'))
            {
                break;
            }
        }
        else
        {
            /* Look for standard terminators */
            if((atStr[*index] == '\n') ||
               (atStr[*index] == '\r') ||
               (atStr[*index] == ',') ||
               (atStr[*index] == ')'))
            {
                break;
            }
        }


        /* One more character in argument */
        length++;
        (*index)++;
    }

    /* If an argument was found, cut it out and prepare for next
     * run */
    if((begin != 0xFFFF) && (length > 0))
    {
        /* Generate argument container */
        res = CsrPmemAlloc(length + 1);
        CsrMemCpy(res,
               atStr + begin,
               length);
        res[length] = '\0';

        /* Skip to next argument start */
        while(*index < atLen)
        {
            if((atStr[*index] == ' ') ||
               (atStr[*index] == '\n') ||
               (atStr[*index] == '\r') ||
               (atStr[*index] == '\t') ||
               (atStr[*index] == ')') ||
               (atStr[*index] == '(') ||
               (atStr[*index] == ','))
            {
                /* We found delimiter in current index, so increase
                 * count and bail out. Next run will skip any heading
                 * whitespace */
                (*index)++;
                break;
            }
            (*index)++;
        }
    }

    return res;
}

/* Local: Single-char helper function */
static CsrUint8 csrBtHfgGetSingleChar(CsrUint8 *at, CsrUint16 len, CsrUint16 *idx)
{
    CsrUint8 res;
    CsrUint8 *arg = NULL;

    res = 0xFF;
    arg = csrBtHfgGetArgument(at, len, idx);
    if(arg && (CsrStrLen((char*)arg) > 0))
    {
        if ((CsrStrLen((char*)arg) == 3) && (arg[0]=='"' && arg[2]=='"'))
        {
            res = arg[1];
        }
        else
        {
            res = arg[0];
        }
    }

    CsrPmemFree(arg);

    return res;
}

/* Local: Store CSR_BT_CM_DATA_IND on save queue */
static void csrBtHfgSaveCmDataInd(HfgInstance_t *inst, CsrUint8 *data, CsrUint16 length)
{
    CsrBtCmDataInd *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtCmDataInd));

    prim->type = CSR_BT_CM_DATA_IND;
    prim->btConnId = inst->hfgConnId;
    prim->payloadLength = inst->atLen;
    prim->payload = CsrPmemAlloc(inst->atLen);
    CsrMemCpy(prim->payload, inst->atCmd, inst->atLen);
    prim->context = ((CsrBtCmDataInd *)inst->msg)->context;

    CsrMessageQueuePush(&(inst->saveQueue), CSR_BT_CM_PRIM, prim);
}

/* Local: CHLD missing in AT-sequence */
static void csrBtHfgChldMissingTimeout(CsrUint16 mi, void *mv)
{
    HfgInstance_t *inst;
    inst = (HfgInstance_t*)mv;

    if((inst->state == ServiceSearch_s) &&
       (inst->atState == At4Cmer_s))
    {
        /* We're still in waiting for the AT+CHLD, but it didn't show
         * up. Assume connection is completed */
        HFG_CHANGE_STATE(inst->atState, At5End_s);
        csrBtHfgSlcHfgDone(inst);
    }
    inst->atChldTimer = 0;
}

/* AT exec: ATA */
void CsrBtHfgAtAtaExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrBtHfgRingStop(inst);

    /* Always ok */
    CsrBtHfgSendHfgAnswerInd(inst);
    CsrBtHfgSendAtOk(inst);
}

/* AT set: AT+VGM= */
void CsrBtHfgAtVgmSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 *arg;
    CsrUint16 cme;

    /* Set indicator, send to app and respond */
    arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if(arg)
    {
        CsrUint8 gain;
        gain = (CsrUint8)CsrStrToInt((char*)arg);

        if(gain <= CSR_BT_HFG_MAX_VGM)
        {
            inst->ind.other[CSR_BT_HFG_SET_MIC_VOL] = gain;
            CsrBtHfgSendHfgMicGainInd(inst, gain);

            cme = CSR_BT_CME_SUCCESS;
        }
        CsrPmemFree(arg);
    }

    CsrBtHfgSendAtResponse(inst, cme);
}

/* AT set: AT+VGS= */
void CsrBtHfgAtVgsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 *arg;
    CsrUint16 cme;

    /* Set indicator, send to app and respond */
    arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if(arg)
    {
        CsrUint8 gain;
        gain = (CsrUint8)CsrStrToInt((char*)arg);

        if(gain <= CSR_BT_HFG_MAX_VGS)
        {
            inst->ind.other[CSR_BT_HFG_SET_SPEAKER_VOL] = gain;
            CsrBtHfgSendHfgSpeakerGainInd(inst, gain);

            cme = CSR_BT_CME_SUCCESS;
        }
        CsrPmemFree(arg);
    }

    CsrBtHfgSendAtResponse(inst, cme);
}

/* AT set: AT+CKPD */
void CsrBtHfgAtCkpdSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrBool skipOk;
    skipOk = FALSE;

    /* Do various things depending on what's running */
    if(inst->ringTimer != 0)
    {
        /* Ringing, so pick it up */
        CsrBtHfgRingStop(inst);
        CsrBtHfgSendHfgAnswerInd(inst);
    }
    else if(inst->scoHandle != CSR_SCHED_QID_INVALID)
    {
        /* Audio is on */
        if(inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] == CSR_BT_CALL_ACTIVE_VALUE)
        {
            /* Hang up if call is active */
            CsrBtHfgSendHfgRejectInd(inst);
        }
        else
        {
            /* No call, transfer SCO back to gateway */
            inst->scoWantedState = FALSE;
            inst->pendingSco = TRUE;
            inst->pendingScoDisconnect = TRUE;
            CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
        }
    }
    else if(inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] == CSR_BT_CALL_ACTIVE_VALUE)
    {
        /* Audio is off, but call is active - transfer audio to HS */
        csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
    }
    else if(CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION)
    {
        /* Voice dialing is supported, so send that */
        CsrBtHfgSendHfgVoiceRecogInd(inst, TRUE);
        skipOk = TRUE;
    }
    else
    {
        /* Not much else to do than the redial */
        CsrBtHfgSendHfgDialInd(inst, CSR_BT_HFG_DIAL_REDIAL, NULL);
        skipOk = TRUE;
    }

    /* Send OK to command, except on dial indication which is handled
     * by the application */
    if(!skipOk)
    {
        CsrBtHfgSendAtOk(inst);
    }
}


/* AT exec: AT+CHUP */
void CsrBtHfgAtChupExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrBtHfgRingStop(inst);

    /* Always ok */
    CsrBtHfgSendHfgRejectInd(inst);
    CsrBtHfgSendAtOk(inst);
}

/* AT test: AT+CHLD=? */
void CsrBtHfgAtChldTest(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* Always respond with supported CHLDs and OK. The CHLD sender
     * will check the "callConfig" and deliver the needed
     * parameters */
    CsrBtHfgSendAtChldSupport(inst);
    CsrBtHfgSendAtOk(inst);

    /* This is always the last command in the sequence */
    if(seq)
    {/* Wait for two CSR_BT_CM_DATA_CFM messages to make sure that the CHLD string and
        the "OK" have been delivered. Needed to avoid HFG app trying to do some HFP specific
        operation before the HF app even knows that the SLC is fully established */
        inst->waitForDataCfm = 2;
        /* Some car-kits don't follow the spec and "forgets" to
         * send the AT+CHLD.  Because of this, we must start a
         * timer to cope with the missing signal.
         * Here we have received the AT+CHLD, hence we need to cancle the timer */
        CsrSchedTimerCancel(inst->atChldTimer,NULL,NULL);
        inst->atChldTimer = 0;
    }
}

/* AT set: AT+CHLD= */
void CsrBtHfgAtChldSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    HfgMainInstance_t *mainInst;
    CsrUint8 *arg;
    CsrUint16 cme;
    CsrUint8 val[2];
    CsrUint8 chld;

    /* Decode. If command is ok inform user about state. He will then
     * have to reply. If command is malformed, send error */
    arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;
    mainInst = CsrBtHfgGetMainInstance(inst);

    if(arg)
    {
        val[0] = arg[0];
        val[1] = 0xFF;
        if(CsrStrLen((char*)arg) > 1)
        {
            val[1] = (CsrUint8)(arg[1]);
        }

        /* Map the (x,y) pair into the linear command opcode */
        if((val[0] == '0') &&
           (val[1] == 0xFF) &&
           ((mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_0)==0))
        {
            /* 0 */
            chld = CSR_BT_RELEASE_ALL_HELD_CALL;
        }
        else if((val[0] == '1') &&
                (val[1] == 0xFF) &&
                ((mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_1)==0))
        {
            /* 1 */
            chld = CSR_BT_RELEASE_ACTIVE_ACCEPT;
        }
        else if((val[0] == '1') &&
                (val[1] != 0xFF) &&
                ((mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_1X)==0))
        {
            /* 1x */
            chld = CSR_BT_RELEASE_SPECIFIED_CALL;
        }
        else if((val[0] == '2') &&
                (val[1] == 0xFF) &&
                ((mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_2)==0))
        {
            /* 2 */
            chld = CSR_BT_HOLD_ACTIVE_ACCEPT;
        }
        else if((val[0] == '2') &&
                (val[1] != 0xFF) &&
                ((mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_2X)==0))
        {
            /* 2x */
            chld = CSR_BT_REQUEST_PRIVATE_WITH_SPECIFIED;
        }
        else if((val[0] == '3') &&
                (val[1] == 0xFF) &&
                ((mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_3)==0))
        {
            /* 3 */
            chld = CSR_BT_ADD_CALL;
        }
        else if((val[0] == '4') &&
                (val[1] == 0xFF) &&
                ((mainInst->callConfig & CSR_BT_HFG_CRH_DISABLE_CHLD_4)==0))
        {
            /* 4 */
            chld = CSR_BT_CONNECT_TWO_CALLS;
        }
        else
        {
            chld = 0xFF;
        }

        /* We must check that we support the specific CHLD command */
        if(chld != 0xFF)
        {
            cme = CSR_BT_CME_SUCCESS;
            CsrBtHfgSendHfgCallHandlingInd(inst,
                                      chld,
                                      (CsrUint8 )(val[1]-'0'));
        }

        CsrPmemFree(arg);
    }

    /* Send error if string is invalid */
    if(cme != CSR_BT_CME_SUCCESS)
    {
        CsrBtHfgSendAtResponse(inst, cme);
    }
}

/* AT exec: ATD### (number) */
void CsrBtHfgAtAtdNumberExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrCharString *arg;
    CsrUint16 numLength = 0;

    /* App must reply, or error on invalid */
    arg = (CsrCharString *)csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
    if(arg)
    {
        while(arg[numLength] != '\0')
        {/*removes all ; at the end and truncate the string*/
            if(arg[numLength] == ';')
            {
                arg[numLength] = (CsrUint8)'\0';
            }
            numLength++;
        }
        CsrBtHfgSendHfgDialInd(inst,
                          CSR_BT_HFG_DIAL_NUMBER,
                          arg);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_INVALID_CHARACTERS_IN_DIAL_STRING);
    }
}

/* AT exec: ATD> (memory) */
void CsrBtHfgAtAtdMemoryExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrCharString *arg;
    CsrUint16 numLength = 0;

    /* App must reply, or error on invalid */
    arg = (CsrCharString *)csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
    if(arg)
    {
        while(arg[numLength] != '\0')
        {/*removes all ; at the end and truncate the string*/
            if(arg[numLength] == ';')
            {
                arg[numLength] = (CsrUint8)'\0';
            }
            numLength++;
        }
        CsrBtHfgSendHfgDialInd(inst,
                          CSR_BT_HFG_DIAL_MEMORY,
                          arg);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_INVALID_CHARACTERS_IN_DIAL_STRING);
    }
}

/* AT codec: AT+BAC= */
void CsrBtHfgAtBacSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{

    if(inst->pendingSearch)
    {
        /* If we're performing the SDC search, save message and bail
         * out now */
        csrBtHfgSaveCmDataInd(inst, inst->atCmd, inst->atLen);
        return;
    }
    else
    {
        CsrUint8 *arg;
        CsrUint16 i;
        /* Get list of codecs supported by the remote HF-device */
        /* No matter what codecs the HF said that it supported before, the new settings are the proper ones: forget the old remote supported codecs */
        inst->remSupportedCodecs = 0; 
        for (i= *index; *index < inst->atLen; i++)
        {
            arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
            if (arg)
            {
                csrBtBuildCodecMask(inst,arg);
            }
            CsrPmemFree(arg);
        }
         /* Send reply */
        if ((inst->remSupFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) &&
            (CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION))
        {
            CsrBtHfgSendAtOk(inst);
            /* Reset the last used codec information if any */
            inst->lastCodecUsed = CSR_BT_WBS_INVALID_CODEC;
            if (inst->pendingCodecNegotiation)
            {/* This is due to a codec negotiation that failed: try again! */
                CsrBtHfgSendCodecNegMsg(inst);
            }
        }
        else
        {
            CsrBtHfgSendAtResponse(inst, CSR_BT_CME_OPERATION_NOT_SUPPORTED);
        }
    }
}

/* AT exec: AT+BCC */
void CsrBtHfgAtBccExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    HfgMainInstance_t *mainInst = CsrBtHfgGetMainInstance(inst);
    /* Start codec negotiation */
    if ((inst->remSupFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) &&
        (mainInst->locSupFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION))
    {
        CsrBtHfgSendAtOk(inst);
        if (CSR_BT_WBS_INVALID_CODEC == inst->lastCodecUsed)
        {/* First indicate the codec to use */
            CsrBtHfgSendCodecNegMsg(inst);
        }
        else
        {
            /* Open sync. connection */
            CsrBtCmScoCancelReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
            
            inst->pendingCodecNegotiation = FALSE;
            /* Establish the eSCO connection too */
            csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
        }
    }
    else
    {
        CsrBtHfgSendAtResponse(inst, CSR_BT_CME_OPERATION_NOT_SUPPORTED);
    }
}

/* AT set: AT+BCS */
void CsrBtHfgAtBcsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    if (inst->pendingCodecNegotiation)
    {
        CsrUint8 *arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
        if (arg == NULL)
        {
            CsrBtHfgSendAtResponse(inst, CSR_BT_CME_INCORRECT_PARAMETERS);
            inst->lastCodecUsed = CSR_BT_WBS_INVALID_CODEC;
        }
        else
        {
            if ((CsrUint8)CsrStrToInt((char*)arg) == inst->lastCodecUsed)
            {
                CsrBtHfgSendAtOk(inst);
                /* Open sync. connection */
                CsrBtCmScoCancelReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
            
                inst->pendingCodecNegotiation = FALSE;
                /* Establish the eSCO connection too */
                csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
            }
            else
            {/* Should not happen! The HF should send At+BCS with the same codec ID as
                the GW issued in the unsolicited response code */
                CsrBtHfgSendAtResponse(inst, CSR_BT_CME_OPERATION_NOT_SUPPORTED);
                inst->lastCodecUsed = CSR_BT_WBS_INVALID_CODEC;
            }
            CsrPmemFree(arg);
        }
    }
    else
    {
        CsrBtHfgSendAtResponse(inst, CSR_BT_CME_OPERATION_NOT_SUPPORTED);
    }
}

/* AT exec: AT+BLDN */
void CsrBtHfgAtBldnExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* App must reply */
    CsrBtHfgSendHfgDialInd(inst,
                      CSR_BT_HFG_DIAL_REDIAL,
                      NULL);
}

/* AT set: AT+BRSF= */
void CsrBtHfgAtBrsfSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* Only sent in AT-sequence, and only by HFG devices */
    if(inst->pendingSearch)
    {
        /* If we're performing the SDC search, save message and bail
         * out now */
        csrBtHfgSaveCmDataInd(inst, inst->atCmd, inst->atLen);
        return;
    }
    else
    {
        CsrUint8 *arg;

        /* App must reply, or error on invalid */
        arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);

        /* This message means that the headset as an old-school
         * HS, and that the SLC has been established. Setup common
         * things and fall through to normal CKPD handling */
        inst->linkType = CSR_BT_HFG_CONNECTION_HFG;
        HFG_CHANGE_STATE(inst->atState, At1Brsf_s);

        if (arg)
        {
            inst->remSupFeatures = CsrStrToInt((char*)arg);
        }

        CsrBtHfgSendAtBrsf(inst);
        CsrBtHfgSendAtOk(inst);

        CsrPmemFree(arg);
    }
}

/* AT read: AT+BTRH? */
void CsrBtHfgAtBtrhRead(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrBool slcEstablished = FALSE;

    if ((!inst->pendingSearch) && (inst->atState != At0Idle_s) &&
        (inst->state != ServiceSearch_s) && (inst->state != Connect_s))
    {
        slcEstablished = TRUE;
    }

    /* App must reply */
    if( (!(CsrBtHfgGetMainInstance(inst)->callConfig & CSR_BT_HFG_CRH_DISABLE_BTRH)) &&  slcEstablished )
    {
        CsrBtHfgSendHfgCallHandlingInd(inst,
                                  CSR_BT_BTRH_READ_STATUS,
                                  0);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst, CSR_BT_CME_NO_NETWORK_SERVICE);
    }
}

/* AT set: AT+BTRH= */
void CsrBtHfgAtBtrhSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint16 cme;
    CsrUint8 val;
    CsrBool slcEstablished = FALSE;

    if ((!inst->pendingSearch) && (inst->atState != At0Idle_s) &&
        (inst->state != ServiceSearch_s) && (inst->state != Connect_s))
    {
        slcEstablished = TRUE;
    }
    /* Decode and notify app of BTRH if command ok and supported,
     * otherwise send error */
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if ( (!(CsrBtHfgGetMainInstance(inst)->callConfig & CSR_BT_HFG_CRH_DISABLE_BTRH)) && slcEstablished )
    {
        val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
        if(val != 0xFF)
        {
            val = val - '0' + CSR_BT_BTRH_PUT_ON_HOLD;

            if((val >= CSR_BT_BTRH_PUT_ON_HOLD) && (val <= CSR_BT_BTRH_REJECT_INCOMING))
            {
                cme = CSR_BT_CME_SUCCESS;
                CsrBtHfgSendHfgCallHandlingInd(inst,
                                          val,
                                          0);
            }
        }
    }

    /* Send error if command malformed */
    if(cme != CSR_BT_CME_SUCCESS)
    {
        CsrBtHfgSendAtResponse(inst, cme);
    }
}

/* AT read: AT+CIND? */
void CsrBtHfgAtCindRead(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* Progress AT sequence forward */
    if(seq)
    {
        /* Is the manual CIND indication update enabled? */
        if(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_MANUAL_INDICATORS)
        {
            /* Send indication to app and bail out. The AT sequence will
             * continue when application has replied */
            CsrBtHfgSendHfgManualIndicatorInd(inst);
            return;
        }

        HFG_CHANGE_STATE(inst->atState, At3CindStatus_s);
    }

    /* Always reply */
    CsrBtHfgSendAtCindStatus(inst);
    CsrBtHfgSendAtOk(inst);
}

/* AT test: AT+CIND=? */
void CsrBtHfgAtCindTest(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* Only sent in AT-sequence */
    if(seq)
    {
        if(inst->pendingSearch)
        {
            /* If we're performing the SDC search, save message and bail
             * out now */
            csrBtHfgSaveCmDataInd(inst, inst->atCmd, inst->atLen);
            return;
        }
        else
        {
            /* If we havn't received a BRSF, this means that the device is
             * a HF-0.96 device, ie. a new HFG style protocol */
            if(inst->atState == At0Idle_s)
            {
                csrBtHfgSlcHfgDone(inst);
            }
            HFG_CHANGE_STATE(inst->atState, At2CindQuestion_s);
        }
    }

    /* Always send reply */
    CsrBtHfgSendAtCindSupport(inst);
    CsrBtHfgSendAtOk(inst);
}

/* AT set: AT+CMER= */
void CsrBtHfgAtCmerSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* We received a full AT+CMER=a,b,c,d request. Extract the
     * "d" value as that's what enable/disable indications */
    CsrUint8 val = 0xFF;
    CsrUintFast8 cnt;

    cnt = 0;
    while(cnt < 4)
    {
        val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
        if(val != 0xFF)
        {
            cnt++;
        }
        else
        {
            break;
        }
    }

    /* Is value OK, set indicator and respond, otherwise send
     * error */
    if((cnt == 4) && ((val == '0') || (val == '1')))
    {
        inst->ind.other[CSR_BT_HFG_SET_CMER_CIEV] = val - '0';
        CsrBtHfgSendAtOk(inst);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_NO_NETWORK_SERVICE);
    }

    /* Special extra meaning in AT sequence */
    if(seq)
    {
        /* If 3-way-calling is supported by both, continue AT sequence,
         * otherwise we're done setting up the SLC */
        if((CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING) &&
           (inst->remSupFeatures & CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING))
        {
            HFG_CHANGE_STATE(inst->atState, At4Cmer_s);

            /* Some car-kits don't follow the spec and "forgets" to
             * send the AT+CHLD.  Because of this, we must start a
             * timer to cope with the missing signal */
            if(inst->atChldTimer == 0)
            {
                inst->atChldTimer = CsrSchedTimerSet(CSR_BT_HFG_MISSING_CHLD_TIMER,
                                                   csrBtHfgChldMissingTimeout,
                                                   0,
                                                   (void*)inst);
            }
        }
        else
        {
            /* 3-way-calling not supported in features, so cmer is the
             * last AT sequence */
            HFG_CHANGE_STATE(inst->atState, At5End_s);
            csrBtHfgSlcHfgDone(inst);
        }
    }
}

/* AT set: AT+CMEE= */
void CsrBtHfgAtCmeeSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    char val;
    CsrUint16 cme;

    /* Set indicator and respond */
    val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if(CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES)
    {
        if((val == '0') || (val == '1'))
        {
            cme = CSR_BT_CME_SUCCESS;
            inst->ind.other[CSR_BT_HFG_SET_CMEE] = val - '0';
        }
    }
    CsrBtHfgSendAtResponse(inst, cme);
}

/* AT set: AT+CCWA= */
void CsrBtHfgAtCcwaSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    char val;
    CsrUint16 cme;

    /* App must reply, or error on invalid */
    val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if((val == '0') || (val == '1'))
    {
        cme = CSR_BT_CME_SUCCESS;
        inst->ind.other[CSR_BT_HFG_SET_CCWA] = val - '0';
    }
    CsrBtHfgSendAtResponse(inst, cme);
}

/* AT set: AT+CLIP= */
void CsrBtHfgAtClipSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    char val;
    CsrUint16 cme;

    /* App must reply, or error on invalid */
    val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if((val == '0') || (val == '1'))
    {
        cme = CSR_BT_CME_SUCCESS;
        inst->ind.other[CSR_BT_HFG_SET_CLIP] = val - '0';
    }
    CsrBtHfgSendAtResponse(inst, cme);
}

/* AT exec: AT+CLCC */
void CsrBtHfgAtClccExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* App must reply */
    CsrBtHfgSendHfgCallListInd(inst);
}

/* AT exec: AT+CNUM */
void CsrBtHfgAtCnumExec(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    /* App must reply */
    CsrBtHfgSendHfgSubscriberNumberInd(inst);
}

/* AT read: AT+COPS? */
void CsrBtHfgAtCopsRead(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    if ((CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_COPS) == 0)
    {/* App must reply */
        CsrBtHfgSendHfgOperatorInd(inst);
    }
    else
    { /* COPS not supported by app */
        CsrBtHfgSendAtCops(inst, 3, NULL); /* always mode 3 (see HFP spec.)*/

        CsrBtHfgSendAtOk(inst);
    }
}

/* AT set: AT+COPS= */
void CsrBtHfgAtCopsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    if (CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_COPS)
    {
        CsrBtHfgSendAtResponse(inst, CSR_BT_CME_NO_NETWORK_SERVICE);
    }
    else
    {
        char val[2];

        val[0] = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
        val[1] = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);

        /* Check that argument really was "3,0" */
        if((val[0] == '3') && (val[1] == '0'))
        {
            CsrBtHfgSendAtOk(inst);
        }
        else
        {
            CsrBtHfgSendAtResponse(inst, CSR_BT_CME_NO_NETWORK_SERVICE);
        }
    }
}


/* AT set: AT+BVRA= */
void CsrBtHfgAtBvraSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    char val;

    /* App must reply, or error on invalid */
    if(CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION)
    {
        val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
        if((val == '0') || (val == '1'))
        {
            CsrBool enable;
            enable = (val == '1' ? TRUE : FALSE);

            /* Application must always respond, so don't send any error-code
             * here */
            CsrBtHfgSendHfgVoiceRecogInd(inst, enable);
        }
        else
        {
            /* Malformed data */
            CsrBtHfgSendAtResponse(inst, CSR_BT_CME_INVALID_CHARACTERS_IN_TEXT_STRING);
        }
    }
    else
    {
        /* No support for NR/EC, so send error */
        CsrBtHfgSendAtResponse(inst, CSR_BT_CME_NO_NETWORK_SERVICE);
    }
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* AT set: AT+CSRFN= */
void CsrBtHfgAtCsrFnSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8  val[2];
    CsrUintFast8  cnt;
    CsrUint8 *arg;
    CsrUint16 cme;

    /* Check on "inst->pendingSco" */

    /* Set C2C indicator to flag that remote peer knows about it */
    inst->ind.other[CSR_BT_HFG_SET_C2C] = 1;
    /* initialize to avoid warnings */
    val[0] = 0;
    val[1] = 0;

    /* Continue until entire line is parsed */
    if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        cnt = 0;
        cme = CSR_BT_CME_SUCCESS;
        while(cme == CSR_BT_CME_SUCCESS)
        {
            arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
            if(arg != NULL)
            {
                val[0] = (CsrUint8)CsrStrToInt((char *)arg);
                CsrPmemFree(arg);
            }
            else
            {
                break; /* we have no more data to read */
            }

            arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
            if(arg != NULL)
            {
                val[1] = (CsrUint8)CsrStrToInt((char *)arg);
                CsrPmemFree(arg);
            }
            else
            {
                if((val[0] == '0'))
                {
                    /* First and only parameter: 0. Disable all C2C indicators */
                    for(cnt=0; cnt < CSR_BT_C2C_NUMBER_OF_INDICATORS; cnt++)
                    {
                        inst->ind.c2c[cnt] = 0;
                    }
                    break;
                }
            }

            if((val[0]) <= CSR_BT_C2C_NUMBER_OF_INDICATORS)
            {
                /* check if it is ADPCM settings*/
                if(val[0] == CSR_BT_C2C_ADPCM_IND)
                {
                    if ((val[1] >= CSR_BT_C2C_ADPCM_OFF_VALUE) && (val[1] <= CSR_BT_C2C_ADPCM_4BIT_VALUE))
                    {
                        if ((inst->audioCodecType == 0) || (!inst->pendingCodecNegotiation))
                        {
                            if ((val[1] & inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND]) != 0)
                            {
                                inst->audioCodecType = val[1];
                            }
                        }

                        if ((inst->pendingSco) && ((val[1] & inst->audioCodecType) == 0))
                        {/* received different value than was sent: cross situation. Send ERROR and wait for HF
                            Maybe there should be a timer to ensure an answer to the application.... */
                            if (!inst->pendingCodecNegotiation)
                            {
                                CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,0);
                            }
                            cme = CSR_BT_CME_NO_NETWORK_SERVICE;
                            inst->pendingCodecNegotiation = FALSE;
                            break;
                        }
                        else
                        {
                            if (inst->pendingCodecNegotiation)
                            {/* Auristream negotiation started by the HFG device */
                                CsrBool sampleRateChecked = FALSE;
                                if (inst->remoteSampleRateSupport)
                                {/* Codec OK; what about sampling rate? */
                                    arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index); /* then extract the received one */
                                    if(arg != NULL)
                                    {
                                        val[0] = (CsrUint8)CsrStrToInt((char *)arg);
                                        sampleRateChecked = TRUE;
                                        CsrPmemFree(arg);

                                        arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
                                        if(arg != NULL)
                                        {
                                            val[1] = (CsrUint8)CsrStrToInt((char *)arg);
                                            CsrPmemFree(arg);
                                            if ((((inst->sampleRateUsed & val[1]) == 0) || (val[1] == 0))&&
                                                ((inst->ind.c2c[CSR_BT_C2C_SAMPLE_RATE_IND] & inst->ind.loc2c[CSR_BT_C2C_SAMPLE_RATE_IND]) != 0) )
                                            {/* No match: find a proper value */
                                                if ((inst->sampleRateUsed == 0) &&
                                                    (inst->ind.loc2c[CSR_BT_C2C_SAMPLE_RATE_IND] & CSR_BT_C2C_SAMPLE_16KHZ_VALUE))
                                                {
                                                    inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_16KHZ_VALUE;
                                                }
                                                else if (inst->sampleRateUsed > CSR_BT_C2C_SAMPLE_8KHZ_VALUE)
                                                {/* 16K tried and failed.... Try 8 K*/
                                                    inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
                                                }
                                                if (val[1] == 0)
                                                {/* Try again */
                                                    CsrBtHfgSendAtOk(inst);
                                                    CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,inst->audioCodecType);
                                                }
                                                else
                                                {/* Send ERROR and wait: cross situation....*/
                                                    CsrBtHfgSendAtResponse(inst, CSR_BT_CME_NO_NETWORK_SERVICE);
                                                    inst->pendingCodecNegotiation = FALSE;
                                                }
                                                return;
                                             /* else it will just fail! 8K SHALL be accepted according to BT spec....so this should not be an option */
                                            }
                                        }
                                     }
                                }
                                /* Now, if we get here, everything is OK: Codec settings accepted and either sample rate not supported or
                                   sample rate negotiation is accepted too: just start the audio connection */
                                {
                                    CsrBtHfgAudioCodecStatus *status;
                                    status = CsrPmemAlloc(sizeof(CsrBtHfgAudioCodecStatus));
                                    status->supportedMask = inst->ind.c2c[CSR_BT_C2C_ADPCM_IND];
                                    if (sampleRateChecked)
                                    {
                                        status->currentCodec  = inst->audioCodecType;
                                        inst->ind.c2c[val[0] - 1] |= inst->audioCodecType;
                                    }
                                    else
                                    {
                                        status->currentCodec = val[1];
                                        inst->ind.c2c[val[0] - 1] |= val[1];
                                    }

                                    inst->bccmdSeqNo = ++(CsrBtHfgGetMainInstance(inst)->bccmdSeqNo);
                                    /* tell application that we have received a "useful" audio codec reply */
                                    CsrBtHfgSendAudioCodecHfgStatusAudioInd(inst, status);
                                    if (!sampleRateChecked)
                                    {
                                        inst->audioCodecType = val[1];
                                    }
                                    CsrBtHfgSendAtOk(inst);
                                    cnt=0;
                                    /* and try to establish audio now...*/
                                    if(inst->scoHandle == CSR_SCHED_QID_INVALID)
                                    {
                                        if(inst->linkType == CSR_BT_HFG_CONNECTION_HFG)
                                        {
                                            /* In HFG connections, we always have an incoming
                                             * SCO connection, so take that one down */
                                            CsrBtCmScoCancelReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
                                        }
                                        /* Now, start connecting SCO by trying highest quality
                                         * first */
                                        csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
                                    }
                                    else
                                    {
                                        /* SCO connection already up */
                                        CsrBtHfgSendHfgExtendedAudioInd(inst,
                                                                   inst->scoHandle,
                                                                   inst->scoPcmSlot,
                                                                   0,0,0,0,0,0,
                                                                   inst->audioCodecType,
                                                                   inst->sampleRateUsed,
                                                                   CSR_BT_RESULT_CODE_HFG_SUCCESS,
                                                                   CSR_BT_SUPPLIER_HFG);
                                        inst->pendingSco = FALSE;
                                    }
                                }
                                return;
                            }
                            else
                            {/* Negotiation started by the remote HF device; answer apropriately */
                                if ((val[1] & inst->audioCodecType) != 0)
                                {/* codec type is compatible; get the sample rate value */
                                    if (inst->remoteSampleRateSupport)
                                    {/* the remote device supports sample rate, so the value chosen MUST be in the string received and
                                        SHALL be one of the values contained in ind.c2c[C2C_SAMPLE_RATE_IND]...*/
                                        inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_8KHZ_VALUE; /* first make sure to keep a sensible value to fallback to */
                                        arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index); /* then extract the received one */
                                        if(arg != NULL)
                                        {
                                            val[0] = (CsrUint8)CsrStrToInt((char *)arg);
                                            CsrPmemFree(arg);

                                            arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
                                            if(arg != NULL)
                                            {
                                                val[1] = (CsrUint8)CsrStrToInt((char *)arg);
                                                CsrPmemFree(arg);
                                                inst->sampleRateUsed = val[1];
                                            }
                                         }
                                    }
                                    CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,inst->audioCodecType);
                                    inst->pendingSco = TRUE;
                                    CsrBtHfgSendAtOk(inst);
                                    /* Start audio: compatible settings agreed on */
                                    if(inst->scoHandle == CSR_SCHED_QID_INVALID)
                                    {
                                        if(inst->linkType == CSR_BT_HFG_CONNECTION_HFG)
                                        {
                                            /* In HFG connections, we always have an incoming
                                             * SCO connection, so take that one down */
                                            CsrBtCmScoCancelReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
                                        }
                                        /* Now, start connecting SCO by trying highest quality
                                         * first */
                                        csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
                                    }
                                    return;
                                }
                                else
                                {
                                    CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,0);
                                    cme = CSR_BT_CME_NO_NETWORK_SERVICE;
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {/* if outgoing audio call pending, try new ADPCM settings if needed */
                        if ((inst->pendingSco) && (val[1] == 0) && (inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] >= CSR_BT_C2C_ADPCM_OFF_VALUE))
                        {/* Settings not accepted by the remote; the remote device supports some ADPCM settings: try new codec */
                            if ((inst->audioCodecType == CSR_BT_C2C_ADPCM_4BIT_VALUE) &&
                                ((inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] & CSR_BT_C2C_ADPCM_2BIT_VALUE) != 0) &&
                                ((inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] & CSR_BT_C2C_ADPCM_2BIT_VALUE) != 0))
                            {
                                inst->audioCodecType = CSR_BT_C2C_ADPCM_2BIT_VALUE;
                                CsrBtHfgSendAtOk(inst);
                                CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,inst->audioCodecType);
                                return;
                            }
                            else if ((inst->audioCodecType == CSR_BT_C2C_ADPCM_2BIT_VALUE) &&
                                    ((inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] & CSR_BT_C2C_ADPCM_OFF_VALUE) != 0) &&
                                    ((inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] & CSR_BT_C2C_ADPCM_OFF_VALUE) != 0))
                            {
                                inst->audioCodecType = CSR_BT_C2C_ADPCM_OFF_VALUE;
                                CsrBtHfgSendAtOk(inst);
                                CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,inst->audioCodecType);
                                return;
                            }
                            else
                            {/* No more tries: send OK and start eSCO/SCO using CVSD*/
                                CsrBtHfgSendAtOk(inst);
                                inst->audioCodecType = 0;
                                inst->sampleRateUsed = 0;
                                if(inst->scoHandle == CSR_SCHED_QID_INVALID)
                                {
                                    if(inst->linkType == CSR_BT_HFG_CONNECTION_HFG)
                                    {
                                        /* In HFG connections, we always have an incoming
                                         * SCO connection, so take that one down */
                                        CsrBtCmScoCancelReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
                                    }
                                    /* Now, start connecting SCO by trying highest quality
                                     * first */
                                    csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
                                }
                                else
                                {
                                    /* SCO connection already up */
                                    CsrBtHfgSendHfgExtendedAudioInd(inst,
                                                               inst->scoHandle,
                                                               inst->scoPcmSlot,
                                                               0,0,0,0,0,0,
                                                               inst->audioCodecType,
                                                               inst->sampleRateUsed,
                                                               CSR_BT_RESULT_CODE_HFG_SUCCESS,
                                                               CSR_BT_SUPPLIER_HFG);
                                    inst->pendingSco = FALSE;
                                }
                                return;
                            }
                        }
                        else
                        {
                            if (inst->pendingSco)
                            { /* This time, let the HF: this is a cross situation! */
                                cme = CSR_BT_CME_NO_NETWORK_SERVICE;
                                /* Make sure to let the HF try and answer correctly when it does */
                                inst->pendingCodecNegotiation = FALSE;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    if (((val[0]) == CSR_BT_C2C_SAMPLE_RATE_IND) && ((val[1] >= CSR_BT_C2C_SAMPLE_8KHZ_VALUE) && (val[1] <= CSR_BT_C2C_SAMPLE_16KHZ_VALUE)))
                    {/* Sample rate settings */
                        inst->ind.c2c[val[0] - 1] |= val[1];
                        inst->remoteSampleRateSupport = TRUE;
                    }
                    else
                    {
                        if((val[1] == 0) || (val[1] == 1))
                        {
                            /* Indicator is within range, and value is either on or
                             * off. Update our indicator table */
                            inst->ind.c2c[val[0] - 1] |= val[1];
                        }
                        else
                        {
                            /* Invalid indicator/value pair. Raise error */
                            cme = CSR_BT_CME_NO_NETWORK_SERVICE;
                            inst->pendingCodecNegotiation = FALSE;
                            break;
                        }
                    }
                }

                /* Notify application with full indicator set */

                CsrBtHfgSendAtOk(inst);
            }
            else
            {
                /* Invalid indicator/value pair. Raise error */
                cme = CSR_BT_CME_NO_NETWORK_SERVICE;
                break;
            }
        }/* while loop */
    }
    else
    {
        cme = CSR_BT_CME_NO_NETWORK_SERVICE;
    }

    CsrBtHfgSendAtResponse(inst, cme);
}


/* AT set: AT+CSR= */
void CsrBtHfgAtCsrSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8  val[2];
    CsrUintFast8  cnt;
    CsrUint8 *arg;
    CsrUint16 cme;

    /* Set C2C indicator to flag that remote peer knows about it */
    inst->ind.other[CSR_BT_HFG_SET_C2C] = 1;
    /* initialize to avoid warnings */
    val[0] = 0;
    val[1] = 0;
    /* Continue until entire line is parsed */
    if(!(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        cnt = 0;
        cme = CSR_BT_CME_SUCCESS;
        while(cme == CSR_BT_CME_SUCCESS)
        {

            arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
            if(arg != NULL)
            {
                val[0] = (CsrUint8)CsrStrToInt((char *)arg);
                CsrPmemFree(arg);
            }
            else
            {
                break; /* we have no more data to read */
            }


            arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
            if(arg != NULL)
            {
                val[1] = (CsrUint8)CsrStrToInt((char *)arg);
                CsrPmemFree(arg);
            }
            else
            {
                if((val[0] == '0'))
                {
                    /* First and only parameter: 0. Disable all C2C indicators */
                    for(cnt=0; cnt < CSR_BT_C2C_NUMBER_OF_INDICATORS; cnt++)
                    {
                        inst->ind.c2c[cnt] = 0;
                    }
                    break;
                }
            }



            if((val[0]) <= CSR_BT_C2C_NUMBER_OF_INDICATORS)
            {
                /* check if it is ADPCM settings*/
                if((val[0]) == CSR_BT_C2C_ADPCM_IND && ((val[1] >= CSR_BT_C2C_ADPCM_OFF_VALUE) && (val[1] <= CSR_BT_C2C_ADPCM_4BIT_VALUE)))
                {
                    inst->ind.c2c[val[0] - 1] = val[1];
                    cnt=0;
                }
                else
                {
                    if ((val[0]) == CSR_BT_C2C_SAMPLE_RATE_IND && ((val[1] >= CSR_BT_C2C_SAMPLE_8KHZ_VALUE) && (val[1] <= CSR_BT_C2C_SAMPLE_16KHZ_VALUE)))
                    {
                        inst->ind.c2c[val[0]-1] = val [1];
                        inst->remoteSampleRateSupport = TRUE;
                    }
                    else
                    {
                        if((val[1] == 0) || (val[1] == 1))
                        {
                            /* Indicator is within range, and value is either on or
                             * off. Update our indicator table */
                            inst->ind.c2c[val[0] - 1] = val[1];
                        }
                        else
                        {
                            /* Invalid indicator/value pair. Raise error */
                            cme = CSR_BT_CME_NO_NETWORK_SERVICE;
                            break;
                        }
                    }
                }

                /* Notify application with full indicator set */
                if ( (val[0] < CSR_BT_C2C_ADPCM_IND) ||
                    (inst->pendingSco && ((val[0] == CSR_BT_C2C_ADPCM_IND)||(val[0] == CSR_BT_C2C_SAMPLE_RATE_IND))))
                {
                    CsrBtHfgSendHfgC2cSfInd(inst);
                }
                else
                {
                    if (!inst->pendingSco && ((val[0] == CSR_BT_C2C_ADPCM_IND)||(val[0] == CSR_BT_C2C_SAMPLE_RATE_IND)))
                    {/* This is the HF trying to trigger an audio connection... Negotiate parameters and if
                        successful, start an audio connection with the settings agreed upon */
                        if ((val[0] == CSR_BT_C2C_SAMPLE_RATE_IND)  || (val[0] == CSR_BT_C2C_ADPCM_IND))
                        {
                            if ((inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] & inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND]) &&
                                ((!inst->remoteSampleRateSupport) || (inst->ind.c2c[CSR_BT_C2C_SAMPLE_RATE_IND] & inst->ind.loc2c[CSR_BT_C2C_SAMPLE_RATE_IND])))
                            {/* Match: answer accordingly and start audio connection */
                                inst->audioCodecType = inst->ind.c2c[CSR_BT_C2C_ADPCM_IND];
                                if (inst->remoteSampleRateSupport)
                                {/* make sure to choose the value just received */
                                    if (val[0] == CSR_BT_C2C_SAMPLE_RATE_IND)
                                    {
                                        inst->sampleRateUsed = inst->ind.c2c[CSR_BT_C2C_SAMPLE_RATE_IND];
                                    }
                                    else
                                    {/* the remote device supports sample rate, so the value chosen MUST be in the string received and
                                        SHALL be one of the values contained in ind.c2c[C2C_SAMPLE_RATE_IND]...*/
                                        inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_8KHZ_VALUE; /* first make sure to keep a sensible value to fallback to */
                                        arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index); /* then extract the received one */
                                        if(arg != NULL)
                                        {
                                            val[0] = (CsrUint8)CsrStrToInt((char *)arg);
                                            CsrPmemFree(arg);

                                            arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
                                            if(arg != NULL)
                                            {
                                                val[1] = (CsrUint8)CsrStrToInt((char *)arg);
                                                CsrPmemFree(arg);
                                                inst->sampleRateUsed = val[1];
                                            }
                                        }
                                    }
                                }
                                CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,inst->audioCodecType);
                                CsrBtHfgSendAtResponse(inst, cme); /* OK */
                                inst->pendingSco = TRUE;
                                if(inst->scoHandle == CSR_SCHED_QID_INVALID)
                                {
                                    if(inst->linkType == CSR_BT_HFG_CONNECTION_HFG)
                                    {
                                        /* In HFG connections, we always have an incoming
                                         * SCO connection, so take that one down */
                                        CsrBtCmScoCancelReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
                                    }
                                    /* Now, start connecting SCO by trying highest quality
                                     * first */
                                    csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
                                }
                                return;
                            }
                            else
                            {
                                if (inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] & inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND])
                                {/* The problem must be the sample rate; make sure to use the proper ADPCM */
                                    inst->audioCodecType = inst->ind.c2c[CSR_BT_C2C_ADPCM_IND];
                                    CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,inst->audioCodecType);
                                }
                                else
                                {
                                    CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,0);
                                }
                                cme = CSR_BT_CME_NO_NETWORK_SERVICE; /* Answer ERROR and wait for the HF to try again...*/
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                /* Invalid indicator/value pair. Raise error */
                cme = CSR_BT_CME_NO_NETWORK_SERVICE;
                break;
            }
        }
    }
    else
    {
        cme = CSR_BT_CME_NO_NETWORK_SERVICE;
    }

    CsrBtHfgSendAtResponse(inst, cme);
}

/* AT set: AT+CSRBATT= */
void CsrBtHfgAtCsrBatSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 val;

    /* Set C2C indicator to flag that remote peer knows about it */
    inst->ind.other[CSR_BT_HFG_SET_C2C] = 1;

    /* Notify app of single parameter and respond to headset */
    val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
    if((val != 0xFF) &&
       (inst->ind.loc2c[CSR_BT_C2C_BAT_IND] == 1))
    {
        val = val - '0';
        CsrBtHfgSendAtOk(inst);

        CsrBtHfgSendHfgC2cBatteryInd(inst, val);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_NO_NETWORK_SERVICE);
    }
}

/* AT set: AT+CSRGETSMS= */
void CsrBtHfgAtCsrGetsmsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 *arg;

    /* Set C2C indicator to flag that remote peer knows about it */
    inst->ind.other[CSR_BT_HFG_SET_C2C] = 1;

    /* Check that SMS is enabled on local part */
    if(inst->ind.loc2c[CSR_BT_C2C_SMS_IND] == 1)
    {
        arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
        if(arg)
        {
            CsrBtHfgSendHfgC2cSmsGetInd(inst,
                                   (CsrUint16)CsrStrToInt((char*)arg));
            CsrPmemFree(arg);
        }
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_NO_NETWORK_SERVICE);
    }
}

/* AT set: AT+CSRPWR= */
void CsrBtHfgAtCsrPwrSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 val;

    /* Set C2C indicator to flag that remote peer knows about it */
    inst->ind.other[CSR_BT_HFG_SET_C2C] = 1;

    /* Notify app of single parameter and respond to headset */
    val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
    if((val != 0xFF) &&
       (inst->ind.loc2c[CSR_BT_C2C_PWR_IND] == 1))
    {
        val = val - '0';
        CsrBtHfgSendAtOk(inst);
        CsrBtHfgSendHfgC2cPowerInd(inst, val);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_NO_NETWORK_SERVICE);
    }
}

/* AT set: AT+CSRSF= */
void CsrBtHfgAtCsrSfSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 val[CSR_BT_C2C_NUMBER_OF_INDICATORS];
    CsrUint8 cnt;
    CsrBool hasADPCM = FALSE;

    /* Set C2C indicator to flag that remote peer knows about it */
    inst->ind.other[CSR_BT_HFG_SET_C2C] = 1;

    CsrMemSet(val,0xFF,CSR_BT_C2C_NUMBER_OF_INDICATORS);

    cnt = 1;
    while(cnt <= CSR_BT_C2C_NUMBER_OF_INDICATORS)
    {
        /* Convert one argument at a time. Abort if it's
         * out-of-range or otherwise invalid */
        val[cnt-1] = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
        /* if no more indicators in list, 0xFF is returned */
        if(val[cnt-1] == 0xFF)
        {
            break;
        }
        cnt++;
    }
    /* check for ADPCM and sample rate support */
    if ( (val[CSR_BT_C2C_SAMPLE_RATE_IND-1] >= '0') && (val[CSR_BT_C2C_SAMPLE_RATE_IND-1] <= '3') )
    {
        inst->remoteSampleRateSupport = TRUE;
    }

    if((val[CSR_BT_C2C_ADPCM_IND-1] >= '0') && (val[CSR_BT_C2C_ADPCM_IND-1] <= '7'))
    {
        hasADPCM = TRUE; /* only show other site that we have ADPCM if app wants to */
    }

    /* Store indicators if all we received */
    if((cnt >= CSR_BT_C2C_NUMBER_OF_INDICATORS-3) && /* "-3" because the remote device might not have support for ADPCM or SAMPLE_RATE */
       !(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        inst->ind.c2c[CSR_BT_C2C_NAME_IND] = val[0] - '0';
        inst->ind.c2c[CSR_BT_C2C_TXT_IND] = val[1] - '0';
        inst->ind.c2c[CSR_BT_C2C_SMS_IND] = val[2] - '0';
        inst->ind.c2c[CSR_BT_C2C_BAT_IND] = val[3] - '0';
        inst->ind.c2c[CSR_BT_C2C_PWR_IND] = val[4] - '0';
        if(hasADPCM)
        {
            inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] = val[5] - '0';
        }
        if (inst->remoteSampleRateSupport)
        {
            inst->ind.c2c[CSR_BT_C2C_SAMPLE_RATE_IND] = val[6] - '0';
        }
        CsrBtHfgSendAtCsrSf(inst);
        CsrBtHfgSendAtOk(inst);
        CsrBtHfgSendHfgC2cSfInd(inst);

    }
    else
    {
        /* Error in received string */
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_NO_NETWORK_SERVICE);
    }
}
#endif /* CSR_BT_INSTALL_HFG_C2C */

/* AT set: AT+BIA= */
void CsrBtHfgAtBiaSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 cnt;
    CsrUint16 i = *index;
    CsrUint8 val[CSR_BT_CIEV_NUMBER_OF_INDICATORS];
    CsrBool  failFormat = FALSE;
    CsrBool  commaReceived = FALSE;

    CsrMemCpy(val,inst->ind.bia,CSR_BT_CIEV_NUMBER_OF_INDICATORS);

    cnt = 1;
    for (i = *index; i < inst->atLen; i++)
    {
        char value = inst->atCmd[i];

        if (((i == *index) || (commaReceived)) && (value == ','))
        {/* The first character is a ',' or a ',' has been received after a previous one */
            cnt++;
        }
        else
        {
            if((value != '0') && (value != '1') && (value != ','))
            {
                if ((value != '\r') && (value != '\n'))
                {/* Strange, unexpected characters detected in AT command: discard! */
                    failFormat = TRUE;
                }
                i = inst->atLen;
            }
            else if (value != ',')
            {
                if (((cnt < CSR_BT_CALL_STATUS_INDICATOR) || (cnt > CSR_BT_CALL_HELD_INDICATOR) ) &&
                    (cnt < CSR_BT_CIEV_NUMBER_OF_INDICATORS))
                {/* Only accept values 1 and 0 for the entries in the array and only for the entries that are allowed to be modified */
                    val[cnt] = value - '0';
                }
                cnt++;
            }
        }
        if (value == ',')
        {
            commaReceived = TRUE;
        }
        else
        {
            commaReceived = FALSE;
        }
    }

    if (failFormat)
    {/* fail in command format: rejct and do not update local values */
        CsrBtHfgSendAtResponse(inst,
                          CSR_BT_CME_NO_NETWORK_SERVICE);
    }
    else
    {/* AT command received and understood: update local values */
        CsrMemCpy(inst->ind.bia,val,CSR_BT_CIEV_NUMBER_OF_INDICATORS);
        CsrBtHfgSendAtOk(inst);
    }
}

/* AT set: AT+NREC= */
void CsrBtHfgAtNrecSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    char val;
    CsrUint16 cme;

    /* Set indicator and respond */
    val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if(CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION)
    {
        if((val == '0') || (val == '1'))
        {
            CsrBool enable;
            cme = CSR_BT_CME_SUCCESS;
            enable = (val == '1' ? TRUE : FALSE);
            CsrBtHfgSendHfgNoiseEchoInd(inst, enable);
        }
    }
    CsrBtHfgSendAtResponse(inst, cme);
}


/* AT set: AT+VTS= */
void CsrBtHfgAtVtsSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 val;
    CsrUint16 cme;

    /* Set indicator and respond */
    val = csrBtHfgGetSingleChar(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if(val != 0xFF)
    {
        CsrBtHfgSendHfgGenerateDtmfInd(inst, val);
        cme = CSR_BT_CME_SUCCESS;
    }

    CsrBtHfgSendAtResponse(inst, cme);
}

/* AT set: AT+BINP= */
void CsrBtHfgAtBinpSet(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq)
{
    CsrUint8 *arg;
    CsrUint16 cme;

    /* Fetch request code and send to app, who must reply */
    arg = csrBtHfgGetArgument(inst->atCmd, inst->atLen, index);
    cme = CSR_BT_CME_NO_NETWORK_SERVICE;

    if(arg &&
       (CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG))
    {
        CsrUint8 req;
        req = (CsrUint8)CsrStrToInt((char*)arg);

        if(req > 0)
        {
            CsrBtHfgSendHfgBtInputInd(inst, req);
            cme = CSR_BT_CME_SUCCESS;
        }
        CsrPmemFree(arg);
    }
    else CsrPmemFree(arg);

    /* If BINP was not supported or message malformed, reply */
    if(cme != CSR_BT_CME_SUCCESS)
    {
        CsrBtHfgSendAtResponse(inst, cme);
    }
}


