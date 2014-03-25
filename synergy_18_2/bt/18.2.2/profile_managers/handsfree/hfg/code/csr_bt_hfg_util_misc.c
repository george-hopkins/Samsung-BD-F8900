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
#include "csr_bt_cm_lib.h"
#include "csr_bt_util.h"
#include "csr_bccmd_prim.h"
#include "csr_bccmd_lib.h"
#ifdef CSR_USE_DSPM
#include "csr_dspm_lib.h"
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"


/* Update service records and connections */
CsrBool CsrBtHfgUpdateService(HfgMainInstance_t *inst)
{
    if(CsrBtHfgRecordUpdate(inst))
    {
        return TRUE;
    }
    if(CsrBtHfgConnectionUpdate(inst))
    {
        return TRUE;
    }
    return FALSE;
}

void CsrBtHfgLpTimeout(CsrUint16 mi, void *mv)
{
    HfgInstance_t *inst;
    inst = (HfgInstance_t*) mv;

    /* Timer was fired */
    inst->lpTimer = 0;
    CsrBtHfgLpTimeoutHandler(inst);
    CSR_UNUSED(mi);
}

/* Timer event function for low-power (sniff) timer */
void CsrBtHfgLpTimeoutHandler(HfgInstance_t *inst)
{
    CsrBool rerun;

    /* Paranoid checks - may happen at a time-cancel combined
     * with a shutdown */
    if((inst == NULL) || (CsrBtHfgGetMainInstance(inst) == NULL))
    {
        return;
    }

    rerun = FALSE;

    /* If data pending either incoming or outgoing, re-start timer and bail out */
    if ((inst->atCmd != NULL) || (inst->cmTxQueue != NULL))
    {
        inst->lpTimer = CsrSchedTimerSet(CSR_BT_HFG_LP_TIME,
                                       CsrBtHfgLpTimeout,
                                       0,
                                       (void *)inst);
        return;

    }

    /* Override wanted mode if sniff not allowed? */
    if(CsrBtHfgGetMainInstance(inst)->lpMask & CSR_BT_HFG_PWR_DISABLE_SNIFF_REQ)
    {
        inst->lpWantedMode = CSR_BT_ACTIVE_MODE;
    }

    /* Request LP-mode again? */
    if(inst->lpWantedMode != inst->lpMode)
    {
        rerun = TRUE;
        inst->pendingModeChange = TRUE;
        CsrBtCmRfcModeChangeReqSend(inst->hfgConnId,
                            inst->lpWantedMode);
    }

    /* Reschedule timer if needed */
    if(inst->lpCount != 0)
    {
        inst->lpCount--;
    }
    if(rerun && (inst->lpCount > 0) && (inst->lpTimer == 0))
    {
        inst->lpTimer = CsrSchedTimerSet(CSR_BT_HFG_LP_TIME,
                                       CsrBtHfgLpTimeout,
                                       0,
                                       (void *)inst);
    }
}

/* Start low-power (sniff) timer */
void CsrBtHfgLpStart(HfgInstance_t *inst, CsrBool now)
{
    /* Restart timer? */
    if(inst->lpTimer == 0)
    {
        inst->lpTimer = CsrSchedTimerSet(CSR_BT_HFG_LP_TIME,
                                       CsrBtHfgLpTimeout,
                                       0,
                                       (void*)inst);
    }

    /* Always restart retry-counter */
    inst->lpWantedMode = CSR_BT_SNIFF_MODE;
    inst->lpCount = CSR_BT_HFG_LP_RETRY;

    /* Schedule an immediate LP request? */
    if(now)
    {
        inst->lpCount++;
        CsrBtHfgLpTimeoutHandler(inst);
    }
}

/* Stop low-power (sniff) timer */
void CsrBtHfgLpStop(HfgInstance_t *inst)
{
    CsrUint16 mi;
    void *mv;

    if(inst->lpTimer != 0)
    {
        CsrSchedTimerCancel(inst->lpTimer,
                           &mi,
                           &mv);
        inst->lpTimer = 0;
    }
    inst->lpCount = CSR_BT_HFG_LP_RETRY;
}

/* Link connection established, perform standard actions */
void CsrBtHfgLinkConnectSuccess(HfgInstance_t *inst)
{
    HfgMainInstance_t *mainInst;
    mainInst = CsrBtHfgGetMainInstance(inst);

    /* Setup a few "minor" settings */
    mainInst->activeConnection = CSR_BT_HFG_NO_CONID;

    /* HFG must accept incoming SCO */
    if(inst->linkType == CSR_BT_HFG_CONNECTION_HFG)
    {
        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                  inst->hfgConnId,
                                  CSR_BT_COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY,
                                  CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH,
                                  CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH,
                                  CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY,
                                  CSR_BT_ESCO_DEFAULT_CONNECT_VOICE_SETTINGS,
                                  CSR_BT_SCO_DEFAULT_ACCEPT_RE_TX_EFFORT);
    }
    /* Start low-power timer and force an immediate timeout */
    CsrBtHfgLpStart(inst, TRUE);

    /* Start record and connection update */
    CsrBtHfgUpdateService(mainInst);
}

/* Link connection failed, perform standard actions */
void CsrBtHfgLinkConnectFailed(HfgInstance_t *inst)
{
    HfgMainInstance_t *mainInst;
    mainInst = CsrBtHfgGetMainInstance(inst);

    /* Mark this channel as not-accepting */
    CsrBtHfgConnectionConfirm(CsrBtHfgGetMainInstance(inst),
                         inst->serverChannel);

    /* Active setup connection is current */
    mainInst->activeConnection = inst->index;

    /* Clear connection instance */
    CsrBtHfgInitializeConnInstance(mainInst, inst->index);
    HFG_CHANGE_STATE(inst->state, Activate_s);

    /* Start record and connection update */
    if (mainInst->state != MainDeactivate_s)
    {
        CsrBtHfgUpdateService(mainInst);
    }
}

/* Link disconnected, perform standard actions */
void CsrBtHfgLinkDisconnect(HfgInstance_t *inst, CsrBool localTerminated, CsrBtResultCode reasonCode, CsrBtSupplier reasonSupplier)
{
    HfgMainInstance_t *mainInst;
    mainInst = CsrBtHfgGetMainInstance(inst);

    /* Notify application and then clean instance */
    CsrBtHfgSendHfgDisconnectInd(inst, localTerminated, reasonCode, reasonSupplier);
    CsrBtHfgInitializeConnInstance(mainInst, inst->index);
    HFG_CHANGE_STATE(inst->state, Activate_s);

    /* Start record and connection update */
    CsrBtHfgUpdateService(mainInst);
}

/* De Register timeout function*/
void CsrBtHfgDeRegisterTimeout(CsrUint16 mi, void *mv)
{
    HfgMainInstance_t *inst = (HfgMainInstance_t *)mv;
    CsrUintFast8 i;
    CsrUint8 sc = (CsrUint8)mi;
    HfgInstance_t *link;
    CsrBool deRegister = FALSE;

    /* First find out whether the link is still connected or not */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        link = &(inst->linkData[i]);
        if((link->serverChannel == sc) && (link->state == Connected_s))
        {
            link->deRegisterTimer = 0;
            deRegister = TRUE;
            break;
        }
    }
    /* If still connected, and the service record is still registered, 
       unregister it */
    if (deRegister)
    {
        for(i=0; (i<CSR_BT_HFG_NUM_CHANNELS && deRegister); i++)
        {
            if (inst->service[i].chan == sc)
            {
                CsrBtCmSdsUnRegisterReqSend(CSR_BT_HFG_IFACEQUEUE,inst->service[i].recHandle);
                deRegister = FALSE;
            }
        }
    }

}

/* Connection guard function */
void CsrBtHfgConnectionTimeout(CsrUint16 mi, void *mv)
{/* Disconnect and let the app know */
    HfgInstance_t *inst = (HfgInstance_t *)mv;
    CsrBtCmDisconnectReqSend(inst->hfgConnId);
}

/* Ring timeout function */
void CsrBtHfgRingTimeout(CsrUint16 mi, void *mv)
{
    HfgInstance_t *inst;
    inst = (HfgInstance_t*)mv;

    /* Timer was fired */
    inst->ringTimer = 0;

    CsrBtHfgHandleRingEvent(inst);
    CSR_UNUSED(mi);
}

/* Handle ring event */
void CsrBtHfgHandleRingEvent(HfgInstance_t *inst)
{
    /* Don't decrease if ringing forever and don't underflow */
    if((inst->ringMsg->numOfRings > 0) &&
       (inst->ringMsg->numOfRings != 255))
    {
        inst->ringMsg->numOfRings--;
    }

    /* Make sure ring-count has not expired */
    if(inst->ringMsg->numOfRings != 0)
    {
        if ((inst->linkType != CSR_BT_HFG_CONNECTION_AG) ||
            (inst->ind.other[CSR_BT_HFG_INBAND_RINGING] == 0))
        {/* Update for HSP BT 2.1: do not send ring if in-band ringing is enabled and it is a HSP connection*/
         /* Send RING before sending CLIP!!*/
            CsrBtHfgSendAtRing(inst);
        }

        /* Send CLIP? */
        if((inst->ind.other[CSR_BT_HFG_SET_CLIP]) && (inst->ringMsg->number != NULL))
        {
            CsrCharString *name = NULL;

#ifdef CSR_BT_INSTALL_HFG_C2C
            if(CsrBtHfgCsrFeature(inst, CSR_BT_C2C_NAME_IND))
            {
                name = inst->ringMsg->name;
            }
#endif
            CsrBtHfgSendAtClip(inst,
                          inst->ringMsg->number,
                          name,
                          inst->ringMsg->numType);
        }

    }

    if(inst->ringMsg->numOfRings == 0)
    {
        /* Stop ringing? */
        CsrBtHfgSendHfgRingCfm(inst);
        CsrBtHfgRingStop(inst);
    }
    else
    {
        /* Reschedule ring */
        inst->ringTimer = CsrSchedTimerSet((CsrTime)(inst->ringMsg->repetitionRate * 1000000),
                                         CsrBtHfgRingTimeout,
                                         0,
                                         (void*)inst);
    }
}

/* Stop ring timer */
void CsrBtHfgRingStop(HfgInstance_t *inst)
{
    /* Kill timer */
    if(inst->ringTimer != 0)
    {
        CsrUint16 mi;
        void *mv;
        CsrSchedTimerCancel(inst->ringTimer,
                           &mi,
                           &mv);
        inst->ringTimer = 0;
    }

    /* Kill message */
    if(inst->ringMsg)
    {
        CsrPmemFree(inst->ringMsg->number);
        CsrPmemFree(inst->ringMsg->name);
        CsrPmemFree(inst->ringMsg);
        inst->ringMsg = NULL;
    }
}

/* Save message on global save queue */
void CsrBtHfgMainSaveMessage(HfgMainInstance_t *inst)
{
    CsrMessageQueuePush(&inst->saveQueue, inst->msgClass, inst->msg);
    inst->msg = NULL;
    inst->msgClass = 0;
    inst->restoreFlag = FALSE;
}

#if defined(CSR_BT_INSTALL_HFG_CONFIG_AUDIO) || defined(CSR_BT_INSTALL_HFG_C2C)
void CsrBtHfgMainStoreMessageData(HfgMainInstance_t *inst)
{
    if (inst->msgClass == CSR_BT_HFG_PRIM)
    {
        CsrBtHfgPrim         *primType;
        primType = (CsrBtHfgPrim *)inst->msg;

        switch(*primType)
        {
#ifdef CSR_BT_INSTALL_HFG_C2C
            case CSR_BT_HFG_C2C_SF_REQ:
            {/* No connection but message received from app: use data and keep message */
                CsrBtHfgC2cSfReq *prim;
                prim = (CsrBtHfgC2cSfReq*)inst->msg;
                if((prim->number > CSR_BT__UNUSED_C2C_IND) &&
                   (prim->number < CSR_BT_C2C_NUMBER_OF_INDICATORS) &&
                   (prim->connectionId == CSR_BT_HFG_CONNECTION_ALL))
                {
                    CsrUintFast8 i;

                    inst->ind.loc2c[prim->number] = prim->value;
                    for (i=0;i<CSR_BT_HFG_NUM_SERVERS;i++)
                    {
                        inst->linkData[i].ind.loc2c[prim->number] = prim->value;
                    }
                }
                return; /* Do not save message*/
            }
#endif

            case CSR_BT_HFG_CONFIG_AUDIO_REQ:
            {
                CsrBtHfgConfigAudioReq *prim;
                prim = (CsrBtHfgConfigAudioReq*)inst->msg;

                if((prim->audioSetting != NULL) &&
                   (prim->audioSettingLen > 0) &&
                   (prim->connectionId == CSR_BT_HFG_CONNECTION_ALL))
                {
                    switch (prim->audioType)
                    {
                        case CSR_BT_HFG_AUDIO_CODEC_SETUP: /* ADPCM */
                        {
                            /* This indicates the "preferred" settings from the application. 
                              No communication is needed with the remote device yet... */
                            CsrBtHfgAudioCodecType *data;
                            data = (CsrBtHfgAudioCodecType*)prim->audioSetting;
                            inst->preferredCodecType = *data;
                            break;
                        }
                        case CSR_BT_HFG_AUDIO_SAMPLE_RATE_SETUP:
                        {/* This indicates the "preferred" settings from the application. 
                              No communication is needed with the remote device yet... */
                            CsrBtHfgAudioSampleRateType *data;
                            data = (CsrBtHfgAudioSampleRateType*)prim->audioSetting;
                            inst->preferredSampleRate = *data;
                            break;
                        }
#ifdef CSR_USE_DSPM
                        case CSR_BT_HFG_AUDIO_DSPM_STREAMS_CONFIG:
                        {
                            CsrBtHfgAudioDspmStreamConfig *dspmAudConfig = (CsrBtHfgAudioDspmStreamConfig *)prim->audioSetting;
                
                            inst->dspmInst.sourceType               = dspmAudConfig->sourceType;
                            inst->dspmInst.sourceInstance           = dspmAudConfig->sourceInstance;
                            inst->dspmInst.sourceChannel            = dspmAudConfig->sourceChannel;
                            inst->dspmInst.sinkType                 = dspmAudConfig->sinkType;
                            inst->dspmInst.sinkInstance             = dspmAudConfig->sinkInstance;
                            inst->dspmInst.sinkChannel              = dspmAudConfig->sinkChannel;
                            /* Make sure that there are no memory leaks: free the config pointers to begin with */
                            CsrPmemFree(inst->dspmInst.sourceWbsConfigParams);
                            CsrPmemFree(inst->dspmInst.sourceNbConfigParams);
                            CsrPmemFree(inst->dspmInst.sinkWbsConfigParams);
                            CsrPmemFree(inst->dspmInst.sinkNbConfigParams);
                            inst->dspmInst.numSourceWbsConfigParams = dspmAudConfig->numSourceWbsConfigParams;
                            inst->dspmInst.sourceWbsConfigParams    = dspmAudConfig->sourceWbsConfigParams;
                            inst->dspmInst.numSourceNbConfigParams  = dspmAudConfig->numSourceNbConfigParams;
                            inst->dspmInst.sourceNbConfigParams     = dspmAudConfig->sourceNbConfigParams;
                            inst->dspmInst.numSinkWbsConfigParams   = dspmAudConfig->numSinkWbsConfigParams;
                            inst->dspmInst.sinkWbsConfigParams      = dspmAudConfig->sinkWbsConfigParams;
                            inst->dspmInst.numSinkNbConfigParams    = dspmAudConfig->numSinkNbConfigParams;
                            inst->dspmInst.sinkNbConfigParams       = dspmAudConfig->sinkNbConfigParams;
                            break;
                        }
#endif
                        default:
                        {
                            CsrBtHfgMainSaveMessage(inst);
                            break;
                        }

                    }
                }
                else
                {
                    CsrBtHfgMainSaveMessage(inst);
                }
                break;
            }
            default:
                break;
        }
    }

}
#endif /* defined(CSR_BT_INSTALL_HFG_CONFIG_AUDIO) || defined(CSR_BT_INSTALL_HFG_C2C) */

/* Save message on connection save queue */
void CsrBtHfgSaveMessage(HfgInstance_t *inst)
{
    CsrMessageQueuePush(&inst->saveQueue, inst->msgClass, inst->msg);
    inst->msg = NULL;
    inst->msgClass = 0;
    inst->restoreFlag = FALSE;
    ((HfgMainInstance_t *)inst->main)->msg = NULL;
    ((HfgMainInstance_t *)inst->main)->msgClass = 0;
    ((HfgMainInstance_t *)inst->main)->restoreFlag = FALSE;
}

/* Empty local save queue */
void CsrBtHfgEmptySaveQueue(HfgInstance_t *inst)
{
    CsrUint16 mi;
    void *mv;

    if(inst->restoreFlag)
    {
        while(CsrMessageQueuePop(&(inst->saveQueue),
                         &mi,
                         &mv))
        {
            CsrBtHfgFreeMessage(mi, mv);
        }
        inst->restoreFlag = FALSE;
    }
}

#ifdef CSR_BT_INSTALL_HFG_C2C
/* Return whether a CSR-2-CSR feature (indicator) is enabled both
 * locally, remote and in general config bitmask */
CsrBool CsrBtHfgCsrFeature(HfgInstance_t *inst, CsrUint8 feat)
{
    if((inst->ind.c2c[feat] >= 1) &&
       (inst->ind.loc2c[feat] >= 1) &&
       !(CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_C2C))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif /* CSR_BT_INSTALL_HFG_C2C */

/* Initialize status indicators */
void CsrBtHfgInitializeIndicators(HfgInstance_t *inst)
{
    HfgMainInstance_t *mainInst;
    CsrUintFast8 i;

    /* Copy all indicators from global */
    mainInst = CsrBtHfgGetMainInstance(inst);

    for(i=0; i<CSR_BT_CIEV_NUMBER_OF_INDICATORS; i++)
    {
        inst->ind.ciev[i] = mainInst->ind.ciev[i];
        inst->ind.bia[i] = 1;
    }
#ifdef CSR_BT_INSTALL_HFG_C2C
    for(i=0; i<CSR_BT_C2C_NUMBER_OF_INDICATORS; i++)
    {
        inst->ind.loc2c[i] = mainInst->ind.loc2c[i];
    }
#endif
    for(i=0; i<CSR_BT_HFG_NUM_OF_SETTINGS; i++)
    {
        inst->ind.other[i] = mainInst->ind.other[i];
    }
}

/* Initialize connection instance */
void CsrBtHfgInitializeConnInstance(HfgMainInstance_t *inst, CsrUint8 index)
{
    HfgInstance_t *link;
    link = &(inst->linkData[index]);

    /* Empty the save-queue. Pending messages must *NOT* be freed here
     * as they are the same pointer as in the main instance! */
    CsrBtHfgEmptySaveQueue(link);

    /* Stop timers */
    CsrBtHfgRingStop(link);
    CsrBtHfgLpStop(link);
    if (link->deRegisterTimer != 0)
    {
        void *mv;
        CsrUint16 mi;
        CsrSchedTimerCancel(link->deRegisterTimer,&mi,&mv);
        link->deRegisterTimer = 0;
    }
    /* Stop AT-sequence CHLD and CKPD timers */
    if(link->atChldTimer != 0)
    {
        void *mv;
        CsrUint16 mi;
        CsrSchedTimerCancel(link->atChldTimer,
                           &mi,
                           &mv);
        link->atChldTimer = 0;
    }
    CsrPmemFree(link->atCmd);
    /* Empty the TX list */ 
    if (link->cmTxQueue)
    {
        CsrUint16 class;
        void *msg;

        while(CsrMessageQueuePop(&link->cmTxQueue, &class, &msg))
        {
            CsrPmemFree(((CsrBtCmDataReq*)msg)->payload);
            CsrPmemFree(msg);
        }
    }
    if (link->conGuardTimer != 0)
    {
        void *mv;
        CsrUint16 mi;
        CsrSchedTimerCancel(link->conGuardTimer ,&mi, &mv);
        link->conGuardTimer = 0;
    }
    /* Zero-initialize everything */
    CsrMemSet(link, 0, sizeof(HfgInstance_t));

    /* Setup the few special members */
    link->main                     = inst;
    link->index                    = index;
    HFG_CHANGE_STATE(link->state, Unused_s);
    HFG_CHANGE_STATE(link->oldState, Unused_s);
    link->serverChannel            = CSR_BT_NO_SERVER;
    link->scoHandle                = CSR_SCHED_QID_INVALID;
    link->cmTxReady                = TRUE;
    link->lpMode                   = CSR_BT_LINK_STATUS_DISCONNECTED;
    link->scoWantedState           = TRUE;

    /* Setup default eSCO settings */
    link->scoParams.txBandwidth    = CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH;
    link->scoParams.rxBandwidth    = CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH;
    link->scoParams.maxLatency     = CSR_BT_ESCO_DEFAULT_CONNECT_MAX_LATENCY;
    link->scoParams.voiceSettings  = CSR_BT_ESCO_DEFAULT_CONNECT_VOICE_SETTINGS;
    link->scoParams.audioQuality   = CSR_BT_ESCO_DEFAULT_CONNECT_AUDIO_QUALITY;
    link->scoParams.reTxEffort     = CSR_BT_ESCO_DEFAULT_CONNECT_RE_TX_EFFORT;
    link->audioCodecType = inst->preferredCodecType; /* inherit the main codec as default*/
    link->sampleRateUsed = inst->preferredSampleRate; /* inherit the main sample rate as default */

    /* Reset indicators */
    CsrBtHfgInitializeIndicators(link);
#ifdef CSR_USE_DSPM
    CsrHfgDspmInit(link);
#endif
}

/* Reset profile for idle state */
void CsrBtHfgResetProfile(HfgMainInstance_t *inst)
{
    CsrUint16 mi;
    CsrUintFast8 i;
    void *mv;
    HfgService_t service[CSR_BT_HFG_NUM_CHANNELS];
    void *links[CSR_BT_HFG_NUM_SERVERS];
    CsrUint16 secIncoming;
    CsrUint16 secOutgoing;
    CsrUint8 tmpAtCmdSettings[CSR_BT_HFG_NUMBER_AT_CMD];
    CsrBtHfgAudioCodecType         tmpCodec;
    CsrBtHfgAudioSampleRateType    tmpSampleRate;
    void *sdcTemp1 = inst->sdpHfgSearchConData;
    void *sdcTemp2 = inst->sdpHfgSearchData;
    CsrBool tmpInitFlag = inst->initSequenceDone;
#ifdef CSR_USE_DSPM
    HfgDspm_t *dspmTmp  = (HfgDspm_t *)CsrPmemZalloc(sizeof(HfgDspm_t));

    CsrMemCpy(dspmTmp, &inst->dspmInst, sizeof(HfgDspm_t));
#endif

    /* Pop everything off global queue */
    if(inst->restoreFlag)
    {
        while(CsrMessageQueuePop(&(inst->saveQueue),
                         &mi,
                         &mv))
        {
            CsrBtHfgFreeMessage(mi, mv);
        }
    }

    /* Free pending message */
    if(inst->msg != NULL)
    {
        CsrBtHfgFreeMessage(inst->msgClass, inst->msg);
    }

#ifdef CSR_BT_INSTALL_HFG_C2C
    inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] = CSR_BT_AUDIO_CODEC_SUPPORT_MASK;
#endif
    /* Reset connection instances */
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        CsrBtHfgInitializeConnInstance(inst,(CsrUint8) i);
    }

    /* Backup server channels and links */
    CsrMemCpy(service, inst->service, sizeof(HfgService_t)*CSR_BT_HFG_NUM_CHANNELS);
    CsrMemCpy(links, inst->linkData, sizeof(void*)*CSR_BT_HFG_NUM_SERVERS);
    tmpCodec = inst->preferredCodecType;
    tmpSampleRate = inst->preferredSampleRate;
    secIncoming = inst->secIncoming;
    secOutgoing = inst->secOutgoing;
    CsrMemCpy(tmpAtCmdSettings,inst->HfgSendAtToApp,CSR_BT_HFG_NUMBER_AT_CMD);

    /* Zero-initialize everything */
    CsrMemSet(inst, 0, sizeof(HfgMainInstance_t));

    /* Make sure not to remove some useful pointers and create heap leaks....*/
    inst->sdpHfgSearchConData = sdcTemp1;
    inst->sdpHfgSearchData = sdcTemp2;

    inst->secIncoming = secIncoming;
    inst->secOutgoing = secOutgoing;
    inst->preferredCodecType = tmpCodec;
    inst->preferredSampleRate = tmpSampleRate;
    inst->initSequenceDone = tmpInitFlag;
    CsrMemCpy(inst->HfgSendAtToApp,tmpAtCmdSettings,CSR_BT_HFG_NUMBER_AT_CMD);

    /* Setup special members */
    inst->appHandle                = CSR_SCHED_QID_INVALID;
    inst->maxConnections           = CSR_BT_HFG_NUM_SERVERS;
    inst->activeService            = CSR_BT_HFG_NO_CONID;
    inst->activeConnection         = CSR_BT_HFG_NO_CONID;

    /* Clear special arrays */
    CsrMemSet(&(inst->ind), 0, sizeof(HfgIndicators_t));
#ifdef CSR_BT_INSTALL_HFG_C2C
    inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] = CSR_BT_AUDIO_CODEC_SUPPORT_MASK;
#endif

    /* Restore backup server channels */
    CsrMemCpy(inst->service, service, sizeof(HfgService_t)*CSR_BT_HFG_NUM_CHANNELS);
    CsrMemCpy(inst->linkData, links, sizeof(void*)*CSR_BT_HFG_NUM_SERVERS);

#ifdef CSR_BT_INSTALL_HFG_C2C
    /* set default codec support*/
    for(i=0; i<CSR_BT_HFG_NUM_SERVERS; i++)
    {
        inst->linkData[i].ind.loc2c[CSR_BT_C2C_ADPCM_IND] = CSR_BT_AUDIO_CODEC_SUPPORT_MASK;
    }
#endif

    /* Setup service record/server channel placeholder */
    for(i=0; i<CSR_BT_HFG_NUM_CHANNELS; i++)
    {
        inst->service[i].registered = 0;
        inst->service[i].accepting = 0;
        inst->service[i].recHandle = 0;
    }

    /* Set default service name */
    CsrStrNCpy((char*)(inst->localName),
            CSR_BT_HFG_SERVICE_NAME,
            CSR_BT_MAX_FRIENDLY_NAME_LEN+1);
    CsrUtf8StrTruncate(inst->localName, CSR_BT_MAX_FRIENDLY_NAME_LEN);
#ifdef CSR_USE_DSPM
    CsrMemCpy(&inst->dspmInst, dspmTmp, sizeof(HfgDspm_t));
    CsrPmemFree(dspmTmp);
#endif
}

/* Free message and member-data if any */
void CsrBtHfgFreeMessage(CsrUint16 class, void *msg)
{
    switch(class)
    {
        case CSR_BT_CM_PRIM:
        {
            CsrBtCmFreeUpstreamMessageContents(class, msg);
            break;
        }

        case CSR_BT_HFG_PRIM:
        {
            CsrBtHfgFreeDownstreamMessageContents(class, msg);
            break;
        }
#ifdef CSR_BT_INSTALL_HFG_C2C
        case CSR_BCCMD_PRIM:
        {
            CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg);
            break;
        }
#endif
        case CSR_SCHED_PRIM:
            {
                CsrEnvPrim *primType;

                primType = (CsrEnvPrim*)msg;
                switch (*primType)
                {
                    case CSR_CLEANUP_IND:
                        {
                            /* Yet nothing to be done for cleanup when deinit */
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
                break;
            }
#ifdef CSR_USE_DSPM
        case CSR_DSPM_PRIM:
        {
            CsrDspmFreeUpstreamMessageContents(CSR_DSPM_PRIM, msg);
            break;
        }
#endif
        default:
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("HFG",
                                    class,
                                    0,
                                    "Unhandled PRIM in CsrBtHfgFreeMessage");
#endif
                break;
            }
    }
    CsrPmemFree(msg);
}

