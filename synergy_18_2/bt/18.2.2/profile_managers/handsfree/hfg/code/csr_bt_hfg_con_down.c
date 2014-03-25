/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
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

/* Disconnect in non-connected state */
void CsrBtHfgXHfgDisconnectReqHandler(HfgInstance_t *inst)
{
    /* Set flag so disconnect can be handled once we leave state */
    inst->pendingDisconnect = TRUE;
}

/* Cancel connect in non-connecting state */
void CsrBtHfgConnectHfgCancelConnectReqHandler(HfgInstance_t *inst)
{
    /* Set flag so cancel can be handled once we leave state */
    inst->pendingCancel = TRUE;
}

/* Disconnect in connected state */
void CsrBtHfgConnectedHfgDisconnectReqHandler(HfgInstance_t *inst)
{
    inst->pendingDisconnect = TRUE;
    if (inst->scoHandle != CSR_SCHED_QID_INVALID)
    {/* Disconnect any pending audio connections and free DSPM resources */
#ifdef CSR_USE_DSPM
        if (CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
        {
            CsrBtHfgDspmSendDisconnectStreams(inst);
        }
#endif
        CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
    }
    CsrBtCmDisconnectReqSend(inst->hfgConnId);

    CsrBtHfgRingStop(inst);
    CsrBtHfgLpStop(inst);
}

/* Cancel connect in connected state */
void CsrBtHfgConnectedHfgCancelConnectReqHandler(HfgInstance_t *inst)
{
    inst->pendingCancel = TRUE;
    CsrBtCmDisconnectReqSend(inst->hfgConnId);

    CsrBtHfgRingStop(inst);
    CsrBtHfgLpStop(inst);
}

/* Start ringing */
void CsrBtHfgConnectedHfgRingReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgRingReq *prim;
    CsrUint32 len;

    prim = (CsrBtHfgRingReq*)inst->msg;

    /* Ring with numOfRings is zero means stop ringing */
    if((prim->numOfRings == 0) &&
       (inst->ringTimer != 0) &&
       (inst->ringMsg != NULL))
    {
        /* Set repetition rate and trigger timeout to reuse
         * ring-confirm code */
        inst->ringMsg->numOfRings = 0;
        CsrBtHfgHandleRingEvent(inst);
        return;
    }

    /* Stop ringing to make sure old signal is freed */
    CsrBtHfgRingStop(inst);

    /* Copy signal to own instance. Note that numOfRings is
     * incremented with one as we decrement the counter before we send
     * the ring */
    inst->ringMsg = (CsrBtHfgRingReq*)CsrPmemAlloc(sizeof(CsrBtHfgRingReq));
    inst->ringMsg->type = CSR_BT_HFG_RING_REQ;
    inst->ringMsg->connectionId = inst->hfgConnId;
    inst->ringMsg->repetitionRate = prim->repetitionRate;

    if(prim->numOfRings != 255)
    {
        inst->ringMsg->numOfRings = prim->numOfRings + 1;
    }
    else
    {
        inst->ringMsg->numOfRings = 255;
    }
    inst->ringMsg->numType = prim->numType;

    /* Number might be null */
    if(prim->number != NULL)
    {
        len = CsrStrLen((char*)prim->number) + 1;
        inst->ringMsg->number = CsrPmemAlloc(len);
        CsrMemCpy(inst->ringMsg->number, prim->number, len);
    }
    else
    {
        inst->ringMsg->number = NULL;
    }

    /* Name might very well also be null */
    if(prim->name != NULL)
    {
        len = CsrStrLen((char*)prim->name) + 1;
        inst->ringMsg->name = CsrPmemAlloc(len);
        CsrMemCpy(inst->ringMsg->name, prim->name, len);
    }
    else
    {
        inst->ringMsg->name = NULL;
    }

    /* Trigger a timeout right away. This function will schedule any
     * additional timeouts */
    CsrBtHfgHandleRingEvent(inst);
}

/* Send call waiting */
void CsrBtHfgConnectedHfgCallWaitingReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgCallWaitingReq *prim;
    CsrCharString *name = NULL;
    prim = (CsrBtHfgCallWaitingReq*)inst->msg;

#ifdef CSR_BT_INSTALL_HFG_C2C
    if(CsrBtHfgCsrFeature(inst, CSR_BT_C2C_NAME_IND))
    {
        name = prim->name;
    }
#endif
    /* Send with/without name */
    if(inst->ind.other[CSR_BT_HFG_SET_CCWA] == 1)
    {
        CsrBtHfgSendAtCcwa(inst,
                      prim->number,
                      name,
                      prim->numType);
    }
}

/* Send call-handling request (unsolicited BTRH) */
void CsrBtHfgConnectedHfgCallHandlingReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgCallHandlingReq *prim;
    prim = (CsrBtHfgCallHandlingReq*)inst->msg;

    /* Send BTRH request if allowed */
    if((prim->btrh != CSR_BT_HFG_BTRH_IGNORE) &&
       !(CsrBtHfgGetMainInstance(inst)->callConfig & CSR_BT_HFG_CRH_DISABLE_BTRH))
    {
        /* Send BTRH response and OK */
        CsrBtHfgSendAtBtrh(inst, prim->btrh);
    }
}

/* Send call-handling response */
void CsrBtHfgConnectedHfgCallHandlingResHandler(HfgInstance_t *inst)
{
    CsrBtHfgCallHandlingRes *prim;
    prim = (CsrBtHfgCallHandlingRes*)inst->msg;

    /* Send BTRH response if required and allowed */
    if(prim->btrh != CSR_BT_HFG_BTRH_IGNORE)
    {
        if(!(CsrBtHfgGetMainInstance(inst)->callConfig & CSR_BT_HFG_CRH_DISABLE_BTRH) &&
           (prim->cmeeCode == CSR_BT_CME_SUCCESS))
        {
            /* Send BTRH response and OK */
            CsrBtHfgSendAtBtrh(inst, prim->btrh);
            CsrBtHfgSendAtOk(inst);
        }
        else
        {
            /* BTRH not enabled or error */
            CsrUint16 error;
            error = (prim->cmeeCode != CSR_BT_CME_SUCCESS)
                ? prim->cmeeCode : CSR_BT_CME_OPERATION_NOT_SUPPORTED;
            CsrBtHfgSendAtResponse(inst, error);
        }
    }
    else
    {
        /* Always send ok/error on CHLD */
        CsrBtHfgSendAtResponse(inst, prim->cmeeCode);
    }
}

/* Send dial-response */
void CsrBtHfgConnectedHfgDialResHandler(HfgInstance_t *inst)
{
    CsrBtHfgDialRes *prim;
    prim = (CsrBtHfgDialRes*)inst->msg;

    /* Note: Because some applications assume that the CKPD handler
     * treats BLDN and BVRA the same, we should also do the same here
     * as in the bvra-res case */
    CsrBtHfgSendAtResponse(inst, prim->cmeeCode);
}

/* Speaker gain indicator has changed */
void CsrBtHfgConnectedHfgSpeakerGainReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgSpeakerGainReq *prim;
    prim = (CsrBtHfgSpeakerGainReq*)inst->msg;

    CsrBtHfgGetMainInstance(inst)->ind.other[CSR_BT_HFG_SET_SPEAKER_VOL] = prim->gain;
    if((inst->remSupFeatures & CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL) &&
       (inst->ind.other[CSR_BT_HFG_SET_SPEAKER_VOL] != prim->gain))
    {
        inst->ind.other[CSR_BT_HFG_SET_SPEAKER_VOL] = prim->gain;
        CsrBtHfgSendAtSpeakerGain(inst, prim->gain);
    }
}

/* Microphone gain indicator has changed */
void CsrBtHfgConnectedHfgMicGainReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgMicGainReq *prim;
    prim = (CsrBtHfgMicGainReq*)inst->msg;

    CsrBtHfgGetMainInstance(inst)->ind.other[CSR_BT_HFG_SET_MIC_VOL] = prim->gain;
    if((inst->remSupFeatures & CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL) &&
       (inst->ind.other[CSR_BT_HFG_SET_MIC_VOL] != prim->gain))
    {
        inst->ind.other[CSR_BT_HFG_SET_MIC_VOL] = prim->gain;
        CsrBtHfgSendAtMicGain(inst, prim->gain);
    }
}

/* Send raw AT command */
void CsrBtHfgConnectedHfgAtCmdReqHandler(HfgInstance_t *inst)
{
    CsrUint8 *dataPtr;
    CsrUint16 dataLen;

    CsrBtHfgAtCmdReq *prim;
    prim = (CsrBtHfgAtCmdReq*)inst->msg;

    /* Copy AT into buffer and send it */
    dataLen = (CsrUint16)CsrStrLen((char*)prim->command);
    dataPtr = CsrPmemAlloc(dataLen);
    CsrMemCpy(dataPtr, prim->command, dataLen);

    CsrBtHfgSendCmDataReq(inst, dataLen, dataPtr);
}

/* Send operator response */
void CsrBtHfgConnectedHfgOperatorResHandler(HfgInstance_t *inst)
{
    CsrBtHfgOperatorRes *prim;
    prim = (CsrBtHfgOperatorRes*)inst->msg;

    if (prim->cmeeCode == CSR_BT_CME_SUCCESS)
    {
        CsrBtHfgSendAtCops(inst, prim->mode, prim->operatorName);
        CsrBtHfgSendAtOk(inst);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,prim->cmeeCode);
    }
}

/* Send call list response */
void CsrBtHfgConnectedHfgCallListResHandler(HfgInstance_t *inst)
{
    CsrBtHfgCallListRes *prim;
    prim = (CsrBtHfgCallListRes*)inst->msg;

    if (prim->cmeeCode == CSR_BT_CME_SUCCESS)
    {
        if (prim->idx > 0)
        {
            CsrBtHfgSendAtClcc(inst,
                          prim->idx,
                          prim->dir,
                          prim->stat,
                          prim->mode,
                          prim->mpy,
                          prim->number,
                          prim->numType);
        }
        if(prim->final)
        {
            CsrBtHfgSendAtOk(inst);
        }
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,prim->cmeeCode);
    }
}

/* Send subscriber number list */
void CsrBtHfgConnectedHfgSubscriberNumberResHandler(HfgInstance_t *inst)
{
    CsrBtHfgSubscriberNumberRes *prim;
    prim = (CsrBtHfgSubscriberNumberRes*)inst->msg;

    if (prim->cmeeCode == CSR_BT_CME_SUCCESS)
    {
        if (prim->number != NULL)
        {
            CsrBtHfgSendAtCnum(inst,
                          prim->number,
                          prim->numType,
                          prim->service);
        }
        if(prim->final)
        {
            CsrBtHfgSendAtOk(inst);
        }
    }
    else
    {
        CsrBtHfgSendAtResponse(inst,prim->cmeeCode);
    }
}

/* Indicator have changed */
void CsrBtHfgXHfgStatusIndicatorSetReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgStatusIndicatorSetReq *prim;
    CsrUint8 old[CSR_BT_CIEV_NUMBER_OF_INDICATORS];

    CsrMemCpy(old, inst->ind.ciev, CSR_BT_CIEV_NUMBER_OF_INDICATORS);
    prim = (CsrBtHfgStatusIndicatorSetReq*)inst->msg;

    /* Update */
    if((prim->indicator >= CSR_BT_SERVICE_INDICATOR) &&
       (prim->indicator < CSR_BT_CIEV_NUMBER_OF_INDICATORS))
    {
        /* If the indicator changed is call_setup_status and it
           changes from 1 (incoming call setup) to something different,
           make sure to stop ringing in all HF/HS devices, if a ring
           signal exists */
        if ( (prim->indicator == CSR_BT_CALL_SETUP_STATUS_INDICATOR) &&
             (inst->ringMsg != NULL) )
        {
            if ( (old[prim->indicator] == CSR_BT_INCOMING_CALL_SETUP_VALUE) &&
                 (prim->value != old[prim->indicator]) )
            {/* Change from incoming call setup ongoing to
                incoming call setup not ongoing: stop ringing */
               inst->ringMsg->numOfRings = 0;
               CsrBtHfgHandleRingEvent(inst);
            }
        }

        inst->ind.ciev[prim->indicator] = prim->value;
        CsrBtHfgGetMainInstance(inst)->ind.ciev[prim->indicator] = prim->value;

        /* Send the status indicator if:
         * It's not disabled by the AT+BIA and it is neither the call, the call setup nor the call held indicator
         * and it's enabled via the AT+CMER
         * and it's different from the previous value
         * or it's a call held indicator */
        if((inst->state == Connected_s) &&
           ((inst->ind.bia[prim->indicator] != 0) ||
            ((prim->indicator >= CSR_BT_CALL_STATUS_INDICATOR) && (prim->indicator <= CSR_BT_CALL_HELD_INDICATOR)))&&
           (inst->ind.other[CSR_BT_HFG_SET_CMER_CIEV] == 1) &&
           ( (old[prim->indicator] != inst->ind.ciev[prim->indicator]) ||
             (prim->indicator == CSR_BT_CALL_HELD_INDICATOR)))
        {
            CsrBtHfgSendAtCiev(inst, prim->indicator, prim->value);
        }
    }
    else
    {
        if (prim->indicator == CSR_BT_GATHERED_CALL_INDICATORS)
        {
            CsrUint8 valueChangeMask = 0;

            switch(prim->value)
            {
                case CSR_BT_NO_CALL_ACTIVE_NO_CALL_SETUP:
                    {
                        if (old[CSR_BT_CALL_STATUS_INDICATOR] == CSR_BT_CALL_ACTIVE_VALUE)
                        {
                            inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_NO_CALL_ACTIVE_VALUE;
                            CsrBtHfgGetMainInstance(inst)->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_NO_CALL_ACTIVE_VALUE;
                            valueChangeMask |= CSR_BT_CALL_INDICATOR_CHANGED;
                        }
                        if (old[CSR_BT_CALL_SETUP_STATUS_INDICATOR] > CSR_BT_NO_CALL_SETUP_VALUE)
                        {
                            inst->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_NO_CALL_SETUP_VALUE;
                            CsrBtHfgGetMainInstance(inst)->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_NO_CALL_SETUP_VALUE;
                            valueChangeMask |= CSR_BT_CALL_SETUP_INDICATOR_CHANGED;
                            if ((old[CSR_BT_CALL_SETUP_STATUS_INDICATOR] == CSR_BT_INCOMING_CALL_SETUP_VALUE) &&
                                (inst->ringMsg != NULL))
                            {
                                inst->ringMsg->numOfRings = 0;
                                CsrBtHfgHandleRingEvent(inst);
                            }
                        }
                        break;
                    }

                case CSR_BT_CALL_ACTIVE_NO_CALL_SETUP:
                    {
                        if (old[CSR_BT_CALL_STATUS_INDICATOR] == CSR_BT_NO_CALL_ACTIVE_VALUE)
                        {
                            inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_CALL_ACTIVE_VALUE;
                            CsrBtHfgGetMainInstance(inst)->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_CALL_ACTIVE_VALUE;
                            valueChangeMask |= CSR_BT_CALL_INDICATOR_CHANGED;
                        }
                        if (old[CSR_BT_CALL_SETUP_STATUS_INDICATOR] > CSR_BT_NO_CALL_SETUP_VALUE)
                        {
                            inst->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_NO_CALL_SETUP_VALUE;
                            CsrBtHfgGetMainInstance(inst)->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_NO_CALL_SETUP_VALUE;
                            valueChangeMask |= CSR_BT_CALL_SETUP_INDICATOR_CHANGED;
                            if  ((old[CSR_BT_CALL_SETUP_STATUS_INDICATOR] == CSR_BT_INCOMING_CALL_SETUP_VALUE) &&
                                (inst->ringMsg != NULL))
                            {
                                inst->ringMsg->numOfRings = 0;
                                CsrBtHfgHandleRingEvent(inst);
                            }
                        }
                        break;
                    }

                case CSR_BT_CALL_ACTIVE_CALL_SETUP:
                    {
                        if (old[CSR_BT_CALL_STATUS_INDICATOR] == CSR_BT_NO_CALL_ACTIVE_VALUE)
                        {
                            inst->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_CALL_ACTIVE_VALUE;
                            CsrBtHfgGetMainInstance(inst)->ind.ciev[CSR_BT_CALL_STATUS_INDICATOR] = CSR_BT_CALL_ACTIVE_VALUE;
                            valueChangeMask |= CSR_BT_CALL_INDICATOR_CHANGED;
                        }
                        if (old[CSR_BT_CALL_SETUP_STATUS_INDICATOR] != CSR_BT_INCOMING_CALL_SETUP_VALUE)
                        {
                            inst->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_INCOMING_CALL_SETUP_VALUE;
                            CsrBtHfgGetMainInstance(inst)->ind.ciev[CSR_BT_CALL_SETUP_STATUS_INDICATOR] = CSR_BT_INCOMING_CALL_SETUP_VALUE;
                            valueChangeMask |= CSR_BT_CALL_SETUP_INDICATOR_CHANGED;
                        }
                        break;
                    }
            }

            if ((inst->state == Connected_s) && (valueChangeMask > 0) && (inst->ind.other[CSR_BT_HFG_SET_CMER_CIEV] == 1))
            {
                CsrBtHfgSendCombinedAtCiev(inst,valueChangeMask);
            }
        }
    }
}

/* Enable/disable inband ringing */
void CsrBtHfgConnectedHfgInbandRingingReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgInbandRingingReq *prim;
    prim = (CsrBtHfgInbandRingingReq*)inst->msg;

    if(CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_INBAND_RINGING)
    {
        inst->ind.other[CSR_BT_HFG_INBAND_RINGING] = prim->inband;
        CsrBtHfgSendAtBsir(inst, prim->inband);
    }
}

/* Send BT input response */
void CsrBtHfgConnectedHfgBtInputResHandler(HfgInstance_t *inst)
{
    CsrBtHfgBtInputRes *prim;
    prim = (CsrBtHfgBtInputRes*)inst->msg;

    /* Send response and OK */
    if((prim->cmeeCode == CSR_BT_CME_SUCCESS) &&
       (CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG))
    {
        CsrBtHfgSendAtBinp(inst, prim->response);
        CsrBtHfgSendAtOk(inst);
    }
    else
    {
        CsrBtHfgSendAtResponse(inst, prim->cmeeCode);
    }
}

/* Send voice recognition activation */
void CsrBtHfgConnectedHfgVoiceRecogReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgVoiceRecogReq *prim;
    prim = (CsrBtHfgVoiceRecogReq*)inst->msg;

    if(CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION)
    {
        CsrBtHfgSendAtBvra(inst, (CsrUint8)(prim->bvra == TRUE ? 1 : 0));
    }
}


/* Handle voice recognition response */
void CsrBtHfgConnectedHfgVoiceRecogResHandler(HfgInstance_t *inst)
{
    CsrBtHfgVoiceRecogRes *prim;
    prim = (CsrBtHfgVoiceRecogRes*)inst->msg;

    /* Note: Because some applications assume that the CKPD handler
     * treats BLDN and BVRA the same, we should also do the same here
     * as in the dial-res case */
    CsrBtHfgSendAtResponse(inst, prim->cmeeCode);
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* CSR-2-CSR indicators have changed */
void CsrBtHfgXHfgC2cSfReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgC2cSfReq *prim;
    CsrUint8 old[CSR_BT_C2C_NUMBER_OF_INDICATORS];

    CsrMemCpy(old, inst->ind.loc2c, CSR_BT_C2C_NUMBER_OF_INDICATORS);
    prim = (CsrBtHfgC2cSfReq*)inst->msg;

    /* Update */
    if((prim->number > CSR_BT__UNUSED_C2C_IND) &&
       (prim->number < CSR_BT_C2C_NUMBER_OF_INDICATORS))
    {
        CsrBtHfgGetMainInstance(inst)->ind.loc2c[prim->number] = prim->value;
        if (prim->connectionId == CSR_BT_HFG_CONNECTION_ALL)
        {
            CsrUint8 i;
            for (i=0;i<CSR_BT_HFG_NUM_SERVERS;i++)
            {
                CsrBtHfgGetMainInstance(inst)->linkData[i].ind.loc2c[prim->number] = prim->value;
            }
        }
        else
        {/* Only for this connection? */
            inst->ind.loc2c[prim->number] = prim->value;
        }


        /* Send update? */
        if((inst->state == Connected_s) &&
           (inst->ind.other[CSR_BT_HFG_SET_C2C] == 1) &&
           (old[prim->number] != inst->ind.loc2c[prim->number]))
        {
            CsrBtHfgSendAtCsr(inst, prim->number, prim->value);
        }
    }
}

/* Send unsolicited text-to-search text */
void CsrBtHfgConnectedHfgC2cTxtReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgC2cTxtReq *prim;
    prim = (CsrBtHfgC2cTxtReq*)inst->msg;

    if(CsrBtHfgCsrFeature(inst, CSR_BT_C2C_TXT_IND))
    {
        CsrBtHfgSendAtCsrTxt(inst, prim->text);
    }
}

/* Send SMS arrival notification */
void CsrBtHfgConnectedHfgC2cSmsArriveReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgC2cSmsArriveReq *prim;
    prim = (CsrBtHfgC2cSmsArriveReq*)inst->msg;

    if(CsrBtHfgCsrFeature(inst, CSR_BT_C2C_SMS_IND))
    {
        CsrBtHfgSendAtCsrSms(inst,
                        prim->smsIndex,
                        prim->number,
                        prim->name);
    }
}

/* Send SMS text */
void CsrBtHfgConnectedHfgC2cSmsTxtReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgC2cSmsTxtReq *prim;
    prim = (CsrBtHfgC2cSmsTxtReq*)inst->msg;

    if(CsrBtHfgCsrFeature(inst, CSR_BT_C2C_SMS_IND))
    {
        CsrBtHfgSendAtCsrGetSms(inst,
                           prim->smsText);
    }
}
#endif /* CSR_BT_INSTALL_HFG_C2C */

