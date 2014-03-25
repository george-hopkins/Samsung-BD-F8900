/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_msg_transport.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_main.h"
#include "csr_bt_hfg_proto.h"
#ifdef CSR_USE_DSPM
#include "csr_dspm_lib.h"
#endif

static const HfgAudioParams_t scoNegotiationOrder[] =
{
    {
        CSR_BT_ESCO_DEFAULT_2P0_S3_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S3_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S3_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_2P0_S3_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_2P0_S3_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_2P0_S3_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_2P0_S3,
    },

    {
        CSR_BT_ESCO_DEFAULT_2P0_S2_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S2_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_2P0_S2_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_2P0_S2_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_2P0_S2_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_2P0_S2_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_2P0_S2,
    },

    {
        CSR_BT_ESCO_DEFAULT_1P2_S1_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_1P2_S1_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_1P2_S1_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_1P2_S1_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_1P2_S1_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_1P2_S1_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_1P2_S1,
    },

    {
        CSR_BT_SCO_DEFAULT_1P1_TX_BANDWIDTH,
        CSR_BT_SCO_DEFAULT_1P1_RX_BANDWIDTH,
        CSR_BT_SCO_DEFAULT_1P1_MAX_LATENCY,
        CSR_BT_SCO_DEFAULT_1P1_VOICE_SETTINGS,
        CSR_BT_SCO_DEFAULT_1P1_AUDIO_QUALITY,
        CSR_BT_SCO_DEFAULT_1P1_RE_TX_EFFORT,
        CSR_BT_SCO_DEFAULT_1P1,
    },
};


static const HfgAudioParams_t wbsScoNegotiationOrder[] =
{
    {
        CSR_BT_ESCO_DEFAULT_T2_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_T2_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_T2_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_T2_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_T2_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_T2_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_T2,
    },

    {
        CSR_BT_ESCO_DEFAULT_T1_TX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_T1_RX_BANDWIDTH,
        CSR_BT_ESCO_DEFAULT_T1_MAX_LATENCY,
        CSR_BT_ESCO_DEFAULT_T1_VOICE_SETTINGS,
        CSR_BT_ESCO_DEFAULT_T1_AUDIO_QUALITY,
        CSR_BT_ESCO_DEFAULT_T1_RE_TX_EFFORT,
        CSR_BT_ESCO_DEFAULT_T1,
    },
};

#ifdef CSR_BT_INSTALL_HFG_C2C
static void csrBtHfgDoAdpcmEscoAdjustments(HfgInstance_t *inst, HfgAudioParams_t *aud)
{
    /* we need to change settings if we want ADPCM support (eSCO only) */
    if(inst->audioCodecType > CSR_BT_C2C_ADPCM_OFF_VALUE)
    {
        /* go into transparent mode */
        aud->voiceSettings |= CSR_BT_AIRCODING_TRANSPARENT_DATA;
        aud->maxLatency = CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY;

        switch(inst->audioCodecType)
        {
            case CSR_BT_C2C_ADPCM_2BIT_VALUE:
                {
                    if ( (inst->ind.loc2c[CSR_BT_C2C_SAMPLE_RATE_IND] == CSR_BT_C2C_SAMPLE_8KHZ_VALUE) ||
                          (inst->sampleRateUsed == 0) || (inst->sampleRateUsed == CSR_BT_C2C_SAMPLE_8KHZ_VALUE) ||
                          (!inst->remoteSampleRateSupport))
                    {
                        aud->txBandwidth  = CSR_BT_HFG_C2C_ADPCM_2BIT_8K_BANDWIDTH;
                        aud->rxBandwidth  = CSR_BT_HFG_C2C_ADPCM_2BIT_8K_BANDWIDTH;
                        aud->maxLatency   = CSR_BT_HFG_C2C_ADPCM_2BIT_8K_MAX_LATENCY;
                        inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
                    }
                    else
                    { /* C2C_SAMPLE_16KHZ_VALUE */
                        aud->txBandwidth  = CSR_BT_HFG_C2C_ADPCM_2BIT_16K_BANDWIDTH;
                        aud->rxBandwidth  = CSR_BT_HFG_C2C_ADPCM_2BIT_16K_BANDWIDTH;
                    }
                    aud->audioQuality = CSR_BT_HFG_C2C_ADPCM_2BIT_AUDIO_QUALITY; /* 2-EV3| EV3  */
                    break;

                }
            case CSR_BT_C2C_ADPCM_4BIT_VALUE:
                {
                    if  ( (inst->ind.loc2c[CSR_BT_C2C_SAMPLE_RATE_IND] == CSR_BT_C2C_SAMPLE_8KHZ_VALUE) ||
                          (inst->sampleRateUsed == 0) || (inst->sampleRateUsed == CSR_BT_C2C_SAMPLE_8KHZ_VALUE))
                    {
                        aud->txBandwidth  = CSR_BT_HFG_C2C_ADPCM_4BIT_8K_BANDWIDTH;
                        aud->rxBandwidth  = CSR_BT_HFG_C2C_ADPCM_4BIT_8K_BANDWIDTH;
                    }
                    else
                    { /* C2C_SAMPLE_16KHZ_VALUE */
                        aud->txBandwidth  = CSR_BT_HFG_C2C_ADPCM_4BIT_16K_BANDWIDTH;
                        aud->rxBandwidth  = CSR_BT_HFG_C2C_ADPCM_4BIT_16K_BANDWIDTH;
                    }
                    aud->audioQuality = CSR_BT_HFG_C2C_ADPCM_4BIT_AUDIO_QUALITY; /* 3-EV3 | 2-EV3| EV3 */
                    break;
                }
            default:
                break;
        }
    }
}
#endif

static void hgfCopyInstScoParms(HfgAudioParams_t *aud, HfgInstance_t *inst)
{
    aud->audioQuality     = inst->scoParams.audioQuality;
    aud->txBandwidth     = inst->scoParams.txBandwidth;
    aud->rxBandwidth     = inst->scoParams.rxBandwidth;
    aud->maxLatency     = inst->scoParams.maxLatency;
    aud->voiceSettings     = inst->scoParams.voiceSettings;
    aud->reTxEffort     = inst->scoParams.reTxEffort;
}

static void hgfCopyScoParms(HfgAudioParams_t *dst, const HfgAudioParams_t *src)
{
    dst->audioQuality     = src->audioQuality;
    dst->txBandwidth     = src->txBandwidth;
    dst->rxBandwidth     = src->rxBandwidth;
    dst->maxLatency     = src->maxLatency;
    dst->voiceSettings     = src->voiceSettings;
    dst->reTxEffort     = src->reTxEffort;
}

/* Local: Downstream message for connect SCO */
void csrBtHfgSendCmScoConnectReq(HfgInstance_t *inst, CsrUint8 setting)
{
    HfgAudioParams_t aud;

    CsrBtCmScoCommonParms *parms;
    CsrUint16 parmsOffset;
    CsrUint16 parmsLen;
    CsrUint32 idx;

    if ((CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_DISABLE_ESCO_TO_OLD_DEVICES) &&
        (inst->remoteVersion < CSR_BT_FIRST_HFP_ESCO))
    {/* Try only SCO */
        idx = CSR_ARRAY_SIZE(scoNegotiationOrder) - 1; /* SCO is the last entry in the table */
        hgfCopyScoParms(&aud, &scoNegotiationOrder[idx]);
        CsrBtCmCommonScoConnectPrepare(&parms, &parmsOffset, &parmsLen, 1);
        CsrBtCmCommonScoConnectBuild(parms,
                        &parmsOffset,
                        aud.audioQuality,
                        aud.txBandwidth,
                        aud.rxBandwidth,
                        aud.maxLatency,
                        aud.voiceSettings,
                        aud.reTxEffort);
    }
    else
    {
        if (CSR_BT_WBS_MSBC_CODEC == inst->lastCodecUsed)
        {/* WBS audio settings */
            CsrBtCmCommonScoConnectPrepare(&parms, &parmsOffset, &parmsLen, 2);
            for (idx = 0; idx < CSR_ARRAY_SIZE(wbsScoNegotiationOrder); ++idx)
            {
                hgfCopyScoParms(&aud, &wbsScoNegotiationOrder[idx]);
                CsrBtCmCommonScoConnectBuild(parms,
                                                &parmsOffset,
                                                aud.audioQuality,
                                                aud.txBandwidth,
                                                aud.rxBandwidth,
                                                aud.maxLatency,
                                                aud.voiceSettings,
                                                aud.reTxEffort);
            }
        }
        else
        {

            hgfCopyInstScoParms(&aud, inst);
#ifdef CSR_BT_INSTALL_HFG_C2C
            csrBtHfgDoAdpcmEscoAdjustments(inst, &aud);

            if(inst->audioCodecType > CSR_BT_C2C_ADPCM_OFF_VALUE &&
               (aud.audioQuality & 0x007) == 0)
            {  /* Only 2 trials: ADPCM as first priority and SCO if that fails */
                if (inst->sampleRateUsed < CSR_BT_C2C_SAMPLE_16KHZ_VALUE)
                { /* 8 KHz chosen (might be due to old Auristream implementation; make sure to try 16 Khz if it fails!
                     if 16 KHz chosen but 2-bit ADPCM is agreed and the remote is an "old" HF, try 8 KHZ first anyway... */
                    CsrBtCmCommonScoConnectPrepare(&parms, &parmsOffset, &parmsLen, 3);
                    CsrBtCmCommonScoConnectBuild(parms,
                                    &parmsOffset,
                                    aud.audioQuality,
                                    aud.txBandwidth,
                                    aud.rxBandwidth,
                                    aud.maxLatency,
                                    aud.voiceSettings,
                                    aud.reTxEffort);
                    /* Just multiply by 2 to change from 8 KHz to 16 KHz settings */
                    CsrBtCmCommonScoConnectBuild(parms,
                                    &parmsOffset,
                                    aud.audioQuality,
                                    aud.txBandwidth * 2,
                                    aud.rxBandwidth * 2,
                                    CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY,
                                    aud.voiceSettings,
                                    aud.reTxEffort);
                }
                else
                { /*C2C_SAMPLE_16KHZ_VALUE; make sure to use 8 KHz if it fails! */
                    CsrBtCmCommonScoConnectPrepare(&parms, &parmsOffset, &parmsLen, 3);
                    CsrBtCmCommonScoConnectBuild(parms,
                                    &parmsOffset,
                                    aud.audioQuality,
                                    aud.txBandwidth,
                                    aud.rxBandwidth,
                                    aud.maxLatency,
                                    aud.voiceSettings,
                                    aud.reTxEffort);
                    /* Just divide by 2 to change from 16 KHz to 8 KHz settings */
                    CsrBtCmCommonScoConnectBuild(parms,
                                    &parmsOffset,
                                    aud.audioQuality,
                                    aud.txBandwidth / 2,
                                    aud.rxBandwidth / 2,
                                    aud.maxLatency,
                                    aud.voiceSettings,
                                    aud.reTxEffort);
                }
            }
            else
#endif
            {
                CsrBtCmCommonScoConnectPrepare(&parms, &parmsOffset, &parmsLen, CSR_ARRAY_SIZE(scoNegotiationOrder)+1);
                CsrBtCmCommonScoConnectBuild(parms,
                                    &parmsOffset,
                                    aud.audioQuality,
                                    aud.txBandwidth,
                                    aud.rxBandwidth,
                                    aud.maxLatency,
                                    aud.voiceSettings,
                                    aud.reTxEffort);
            }

#ifdef CSR_BT_INSTALL_HFG_C2C
            if(inst->audioCodecType > CSR_BT_C2C_ADPCM_OFF_VALUE &&
               (aud.audioQuality & 0x007) == 0)
            {/* ADPCM audio negociated; if establishment fails try SCO only */
                idx = CSR_ARRAY_SIZE(scoNegotiationOrder) - 1; /* SCO is the last entry in the table */
                hgfCopyScoParms(&aud, &scoNegotiationOrder[idx]);
                /* No ADPCM adjustments, in order to try SCO if ADPCM fails! */
                CsrBtCmCommonScoConnectBuild(parms,
                                &parmsOffset,
                                aud.audioQuality,
                                aud.txBandwidth,
                                aud.rxBandwidth,
                                aud.maxLatency,
                                aud.voiceSettings,
                                aud.reTxEffort);
            }
            else
#endif
            {
                for (idx = 0; idx < CSR_ARRAY_SIZE(scoNegotiationOrder); ++idx)
                {
                    hgfCopyScoParms(&aud, &scoNegotiationOrder[idx]);
#ifdef CSR_BT_INSTALL_HFG_C2C
                    csrBtHfgDoAdpcmEscoAdjustments(inst, &aud);
#endif
                    CsrBtCmCommonScoConnectBuild(parms,
                                            &parmsOffset,
                                            aud.audioQuality,
                                            aud.txBandwidth,
                                            aud.rxBandwidth,
                                            aud.maxLatency,
                                            aud.voiceSettings,
                                            aud.reTxEffort);
                }
            }
        }
    }

    CsrBtCmScoConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                        inst->scoPcmSlot,
                        inst->scoPcmRealloc,
                        parms,
                        parmsLen,
                        inst->hfgConnId);
}

/* Downstream: Audio request */
void CsrBtHfgConnectedHfgAudioDisconnectReqHandler(HfgInstance_t *inst)
{
    inst->pendingScoDisconnect = TRUE;
    if(!inst->pendingSco)
    {
        inst->pendingSco = TRUE;
        /* We're requested to disconnect SCO */
        if(inst->scoHandle != CSR_SCHED_QID_INVALID)
        {
#ifdef CSR_USE_DSPM
            if (CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
            {
                CsrBtHfgDspmSendDisconnectStreams(inst);
            }
#endif
            /* Send disconnect */
            CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
        }
        else
        {
            /* SCO already disconnected */
            CsrBtHfgSendHfgAudioDisconnectCfm(inst, inst->scoHandle, CSR_BT_RESULT_CODE_HFG_SUCCESS, CSR_BT_SUPPLIER_HFG);
            inst->pendingSco = FALSE;
            inst->pendingScoDisconnect = FALSE;
        }
    }/* pendingSco */
}

/* Downstream: Audio request */
void CsrBtHfgConnectedHfgAudioReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgAudioConnectReq *prim;
    prim = (CsrBtHfgAudioConnectReq*)inst->msg;

    inst->pendingScoDisconnect = FALSE;
    if ((!inst->pendingSco) && (!inst->pendingCodecNegotiation))
    {
        /* We're requested to connect SCO */
        CsrBool negotiateWbsCodec = FALSE;

        inst->pendingSco = TRUE;
        inst->pendingScoConfirm = TRUE;

        if ((inst->remSupFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) &&
            (CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION))
        {/* WBS */
            if (CSR_BT_WBS_INVALID_CODEC == inst->lastCodecUsed)
            {/* Negotiation needed! */
                negotiateWbsCodec = TRUE;
            }
        }
#ifdef CSR_USE_DSPM
        else
        {/* Make sure to use some sensible values when the audio connection shall be routed locally. */
            inst->lastCodecUsed = CSR_BT_WBS_CVSD_CODEC;
        }
#endif
        /* start codec negotiation only if there is something to negotiate */
        if (negotiateWbsCodec)
        {
            CsrBtHfgSendCodecNegMsg(inst);
            if (inst->scoHandle == CSR_SCHED_QID_INVALID)
            { /* Store the neccessary data */
                inst->scoPcmSlot = prim->pcmSlot;
                inst->scoPcmRealloc = prim->pcmRealloc;
            }
        }
        else
        { /* if no audio negotiation needed, just do as always: try to connect audio. No ADPCM audio... */
            inst->audioCodecType = 0;
            inst->sampleRateUsed = 0;
            if(inst->scoHandle == CSR_SCHED_QID_INVALID)
            {
                inst->scoPcmSlot = prim->pcmSlot;
                inst->scoPcmRealloc = prim->pcmRealloc;

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
    }/* pendingSco or codec negotiation */

}

#ifdef CSR_BT_INSTALL_HFG_CONFIG_AUDIO
/* Downstream: Setup user-defined (e)SCO settings */
void CsrBtHfgXHfgConfigAudioReqHandler(HfgInstance_t *inst)
{
    CsrBtHfgConfigAudioReq *prim;
    prim = (CsrBtHfgConfigAudioReq*)inst->msg;

    if((prim->audioSetting != NULL) &&
       (prim->audioSettingLen > 0))
    {
        /* Decode data and store it in our user-defined settings */
        switch(prim->audioType)
        {
            case CSR_BT_HFG_AUDIO_RETRANSMISSION:
                {
                    CsrBtHfgAudioRetransmission *data;
                    data = (CsrBtHfgAudioRetransmission*)prim->audioSetting;
                    inst->scoParams.reTxEffort = *data;
                    break;
                }

            case CSR_BT_HFG_AUDIO_MAX_LATENCY:
                {
                    CsrBtHfgAudioRetransmission *data;
                    data = (CsrBtHfgAudioRetransmission*)prim->audioSetting;
                    inst->scoParams.maxLatency = *data;
                    break;
                }

            case CSR_BT_HFG_AUDIO_SUP_PACKETS:
                {
                    CsrBtHfgAudioSupPackets *data;
                    data = (CsrBtHfgAudioSupPackets *)prim->audioSetting;
                    inst->scoParams.audioQuality = *data;
                    break;
                }

            case CSR_BT_HFG_AUDIO_TX_BANDWIDTH:
                {
                    CsrBtHfgAudioTxBandwidth *data;
                    data = (CsrBtHfgAudioTxBandwidth*)prim->audioSetting;
                    inst->scoParams.txBandwidth = *data;
                    break;
                }

            case CSR_BT_HFG_AUDIO_RX_BANDWIDTH:
                {
                    CsrBtHfgAudioRxBandwidth *data;
                    data = (CsrBtHfgAudioRxBandwidth *)prim->audioSetting;
                    inst->scoParams.rxBandwidth = *data;
                    break;
                }

            case CSR_BT_HFG_AUDIO_VOICE_SETTINGS:
                {
                    CsrBtHfgAudioVoiceSettings *data;
                    data = (CsrBtHfgAudioVoiceSettings *)prim->audioSetting;
                    inst->scoParams.voiceSettings = *data;
                    break;
                }
#ifdef CSR_BT_INSTALL_HFG_C2C
            case CSR_BT_HFG_AUDIO_CODEC_SETUP: /* ADPCM */
                {  /* This indicates the "preferred" settings from the application.
                      No communication is needed with the remote device yet... */
                    if (CsrBtHfgGetMainInstance(inst)->adpcmLocalSupported)
                    {
                        CsrBtHfgAudioCodecType *data;
                        data = (CsrBtHfgAudioCodecType *)prim->audioSetting;
                        inst->audioCodecType = *data;
                        inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] |= *data;
                        if (prim->connectionId == CSR_BT_HFG_CONNECTION_ALL)
                        {
                            CsrBtHfgGetMainInstance(inst)->preferredCodecType = *data;
                        }
                    }

                    break;
                }
            case CSR_BT_HFG_AUDIO_SAMPLE_RATE_SETUP:
                {/* This indicates the "preferred" settings from the application.
                      No communication is needed with the remote device yet... */

                    if (CsrBtHfgGetMainInstance(inst)->adpcmLocalSupported)
                    {
                        CsrBtHfgAudioSampleRateType *data;
                        data = (CsrBtHfgAudioSampleRateType *)prim->audioSetting;
                        inst->sampleRateUsed = *data;
                        inst->ind.loc2c[CSR_BT_C2C_SAMPLE_RATE_IND] |= *data;
                        if (prim->connectionId == CSR_BT_HFG_CONNECTION_ALL)
                        {
                            CsrBtHfgGetMainInstance(inst)->preferredSampleRate = *data;
                        }
                    }
                    break;
                }
#endif
#ifdef CSR_USE_DSPM
            case CSR_BT_HFG_AUDIO_DSPM_STREAMS_CONFIG:
                {
                    if (CsrBtHfgGetMainInstance(inst)->dspmHfgCallBack != CsrBtHfgDspmConfigStreamRspHandler)
                    {
                        CsrBtHfgAudioDspmStreamConfig *dspmAudConfig = (CsrBtHfgAudioDspmStreamConfig *)prim->audioSetting;
                    
                        inst->dspmInst.sourceType     = dspmAudConfig->sourceType;
                        inst->dspmInst.sourceInstance = dspmAudConfig->sourceInstance;
                        inst->dspmInst.sourceChannel  = dspmAudConfig->sourceChannel;
                        inst->dspmInst.sinkType       = dspmAudConfig->sinkType;
                        inst->dspmInst.sinkInstance   = dspmAudConfig->sinkInstance;
                        inst->dspmInst.sinkChannel    = dspmAudConfig->sinkChannel;
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
                    }
                    break;
                }
            case CSR_BT_HFG_AUDIO_DSPM_STREAM_SINGLE_CONFIG:
                {
                    CsrBtHfgAudioDspStreamSingleConfig *configData = (CsrBtHfgAudioDspStreamSingleConfig *)prim->audioSetting;
                    if (inst->scoHandle == configData->scoHandle)
                    {/* the correct sco connection is found */
                        if (CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
                        {
                            CsrBtHfgDspmSendSingleConfigStream(inst, configData);
                        }
                        else
                        {/* not possible to handle this now; store for later use.... */
                            CsrBtHfgSaveMessage(inst);
                        }
                    }
                    break;
                }
#endif
        }
    }
}
#endif /* CSR_BT_INSTALL_HFG_CONFIG_AUDIO */

/* SCO connect confirm */
void CsrBtHfgConnectedCmScoConnectCfmHandler(HfgInstance_t *inst)
{
    CsrBtCmScoConnectCfm *prim;
    HfgMainInstance_t *mainInst;

    prim = (CsrBtCmScoConnectCfm *) inst->msg;
    mainInst = CsrBtHfgGetMainInstance(inst);

    /* If sco already in use which means a race condition may have
     * occurred, so ignore the confirm */
    inst->pendingCodecNegotiation = FALSE;
    inst->incScoCnt = 0;
    if(inst->scoHandle == CSR_SCHED_QID_INVALID)
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            inst->scoHandle = prim->eScoHandle;
            inst->scoPcmSlot = prim->pcmSlot;
#ifdef CSR_USE_DSPM
            if ( CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
            {
                if (inst->dspmInst.operatorEncoder == HFG_DSPM_DEFAULT_HANDLE)
                {
                    CsrBtHfgDspmSendCreateOperators(inst);
                }
                else
                {/* Connect streams: the operator is already created */
                    CsrHfgDspmAudioConnect(inst);
                }
            }
#endif
            if (prim->airMode == CSR_BT_AIRCODING_TRANSPARENT_DATA)
            {/* The HFG does not know whether the Auristream connection is running at 8 or 16 KHz sampling rate;
                so far this is a guess based on rx- and tx- packetLength... */
#ifdef CSR_BT_INSTALL_HFG_C2C
                if (inst->audioCodecType == CSR_BT_C2C_ADPCM_2BIT_VALUE)
                {
                    if (prim->rxPacketLength >= 0x32)
                    {
                        inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_16KHZ_VALUE;
                    }
                    else
                    {
                        inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
                    }
                }
                else if (inst->audioCodecType == CSR_BT_C2C_ADPCM_4BIT_VALUE)
                {
                    if (prim->rxPacketLength > 0x32)
                    {
                        inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_16KHZ_VALUE;
                    }
                    else
                    {
                        inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
                    }
                }
                else if (inst->audioCodecType == CSR_BT_C2C_ADPCM_OFF_VALUE)
#else
                if (inst->audioCodecType == CSR_BT_C2C_ADPCM_OFF_VALUE)
#endif
                {
                    inst->sampleRateUsed = CSR_BT_C2C_SAMPLE_8KHZ_VALUE;
                }

            }
            /* Audio is up and running */
            if (prim->airMode == CSR_BT_AIRCODING_TRANSPARENT_DATA)
            {
                CsrBtHfgSendHfgExtendedAudioInd(inst,
                                           inst->scoHandle,
                                           inst->scoPcmSlot,
                                           prim->linkType,
                                           prim->weSco,
                                           prim->rxPacketLength,
                                           prim->txPacketLength,
                                           prim->airMode,
                                           prim->txInterval,
                                           inst->audioCodecType,
                                           inst->sampleRateUsed,
                                           CSR_BT_RESULT_CODE_HFG_SUCCESS,
                                           CSR_BT_SUPPLIER_HFG);
            }
            else
            {
                CsrBtHfgSendHfgExtendedAudioInd(inst,
                                           inst->scoHandle,
                                           inst->scoPcmSlot,
                                           prim->linkType,
                                           prim->weSco,
                                           prim->rxPacketLength,
                                           prim->txPacketLength,
                                           prim->airMode,
                                           prim->txInterval,
                                           CSR_BT_C2C_ADPCM_OFF_VALUE,
                                           CSR_BT_C2C_SAMPLE_8KHZ_VALUE,
                                           CSR_BT_RESULT_CODE_HFG_SUCCESS,
                                           CSR_BT_SUPPLIER_HFG);
            }

            /* Should we send audio status? */
            if(mainInst->hfgConfig & CSR_BT_HFG_CNF_AUDIO_STATUS)
            {
#ifdef CSR_BT_INSTALL_HFG_C2C
                CsrBtHfgAudioCodecStatus *codecSet;
#endif
                CsrBtHfgAudioScoStatus *set;
                set = (CsrBtHfgAudioScoStatus *)CsrPmemAlloc(sizeof(CsrBtHfgAudioScoStatus));
                set->linkType = prim->linkType;
                set->txInterval = prim->txInterval;
                set->weSco = prim->weSco;
                set->rxPacketLength = prim->rxPacketLength;
                set->txPacketLength = prim->txPacketLength;
                set->airMode = prim->airMode;
                set->resultCode = prim->resultCode;
                set->resultSupplier = prim->resultSupplier;
                CsrBtHfgSendScoHfgStatusAudioInd(inst, set);
#ifdef CSR_BT_INSTALL_HFG_C2C
                /* if ADPCM is supported we need to tell */
                if(mainInst->adpcmLocalSupported)
                {
                    codecSet = (CsrBtHfgAudioCodecStatus*)CsrPmemAlloc(sizeof(CsrBtHfgAudioCodecStatus));
                    codecSet->supportedMask = CSR_BT_AUDIO_CODEC_SUPPORT_MASK;
                    codecSet->currentCodec = inst->audioCodecType;
                    CsrBtHfgSendAudioCodecHfgStatusAudioInd(inst,codecSet);
                    /* Change codec if needed */
                    if(inst->audioCodecType > CSR_BT_C2C_ADPCM_OFF_VALUE)
                    {
                        CsrBtHfgSetC2CAudioCodec(inst->scoHandle, inst->audioCodecType, inst->bccmdSeqNo);
                    }
                    if (inst->lpMode != CSR_BT_LINK_STATUS_CONNECTED)
                    {
                        inst->lpWantedMode = CSR_BT_LINK_STATUS_CONNECTED;
                    }
                }
            }
            else
            { /* Need to setup the proper codec even if audio debug is not enabled....*/
                if((mainInst->adpcmLocalSupported) && (inst->audioCodecType > CSR_BT_C2C_ADPCM_OFF_VALUE))
                {
                    CsrBtHfgSetC2CAudioCodec(inst->scoHandle, inst->audioCodecType, inst->bccmdSeqNo);
                }
#endif
            }

            /* We knew the SCO was coming... */
            if(inst->pendingSco)
            {
                /* But we also requested it to be disconnected */
                if(inst->pendingScoDisconnect)
                {
#ifdef CSR_USE_DSPM
                    if (CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
                    {
                        CsrBtHfgDspmSendDisconnectStreams(inst);
                    }
#endif
                    CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE, inst->hfgConnId);
                }
                else
                {
                    inst->pendingSco = FALSE;
                }
            }
            if (mainInst->hfgConfig & CSR_BT_HFG_CNF_EXIT_LP_ON_AUDIO)
            {/* Exit LP mode and do not enter it until the SCO is disconnected again*/
                CsrBtHfgLpStop(inst); /* Do not keep on trying to enter sniff mode */
                if (inst->lpMode != CSR_BT_LINK_STATUS_CONNECTED)
                {/* Now try to enter active mode if in LP mode */
                    inst->lpWantedMode = CSR_BT_LINK_STATUS_CONNECTED;
                    CsrBtHfgLpTimeoutHandler(inst);
                }
            }
        }
        else
        { /* SCO setup failed, so begin possible retry */
            if ((inst->remSupFeatures & CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION) &&
                (CsrBtHfgGetMainInstance(inst)->locSupFeatures & CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION))
            {/* WBS */
                if (CSR_BT_WBS_MSBC_CODEC == inst->lastCodecUsed)
                {/* Fall back to CVSD and try again */
                    CsrBtHfgSendCodecNegMsg(inst);
                    return;
                }
            }
#ifdef CSR_USE_DSPM
            else
            {/* Make sure to use some sensible values when the audio connection shall be routed locally. */
                inst->lastCodecUsed = CSR_BT_WBS_CVSD_CODEC;
            }
#endif

            if((mainInst->hfgConfig & CSR_BT_HFG_CNF_AUDIO_STATUS))
            {
                /* Notify user if all SCO attempts failed */
                CsrBtHfgAudioScoStatus *set;

                set = (CsrBtHfgAudioScoStatus*)CsrPmemAlloc(sizeof(CsrBtHfgAudioScoStatus));
                set->linkType = prim->linkType;
                set->txInterval = prim->txInterval;
                set->weSco = prim->weSco;
                set->rxPacketLength = prim->rxPacketLength;
                set->txPacketLength = prim->txPacketLength;
                set->airMode = prim->airMode;
                set->resultCode = prim->resultCode;
                set->resultSupplier = prim->resultSupplier;
                CsrBtHfgSendScoHfgStatusAudioInd(inst, set);


            }

            /* SCO connection failed */
            CsrBtHfgSendHfgExtendedAudioInd(inst,
                                       inst->scoHandle,
                                       inst->scoPcmSlot,
                                       prim->linkType,
                                       prim->weSco,
                                       prim->rxPacketLength,
                                       prim->txPacketLength,
                                       prim->airMode,
                                       prim->txInterval,
                                       inst->audioCodecType,
                                       inst->sampleRateUsed,
                                       prim->resultCode,
                                       prim->resultSupplier);
            inst->pendingSco = FALSE;
        }
    }
}

/* SCO disconnect indication */
void CsrBtHfgConnectedCmScoDisconnectIndHandler(HfgInstance_t *inst)
{
    CsrBtCmScoDisconnectInd *prim;

    prim = (CsrBtCmScoDisconnectInd*)inst->msg;

    /* If sco already disconnected a race condition may have occurred
     * so ignore the message */
    if(inst->scoHandle != CSR_SCHED_QID_INVALID)
    {
        inst->incScoCnt = 0;
        if(prim->status)
        {
#ifdef CSR_USE_DSPM
            if (CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
            {
                CsrBtHfgDspmSendDisconnectStreams(inst);
            }      
#endif
            if ((CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_EXIT_LP_ON_AUDIO) ||
                (inst->lpMode != inst->lpWantedMode))
            {
                CsrBtHfgLpStart(inst, TRUE);
            }
            inst->scoHandle = CSR_SCHED_QID_INVALID;

            if ((inst->pendingScoDisconnect) && (inst->scoWantedState))
            {
                CsrBtHfgSendHfgAudioDisconnectCfm(inst, prim->eScoHandle, CSR_BT_RESULT_CODE_HFG_SUCCESS, CSR_BT_SUPPLIER_HFG);
            }
            else
            {
                CsrBtHfgSendHfgAudioDisconnectInd(inst, prim->eScoHandle, prim->reasonCode, prim->reasonSupplier);
            }
            inst->scoWantedState = TRUE;
            if(inst->pendingSco)
            {
                if(!(inst->pendingScoDisconnect))
                {
                    /* Restart full SCO negotiation */
                    csrBtHfgSendCmScoConnectReq(inst, CSR_BT_ESCO_DEFAULT_CONNECT);
                }
                else
                {
                    inst->pendingSco = FALSE;
                    inst->pendingScoDisconnect = FALSE;
                    if(inst->linkType == CSR_BT_HFG_CONNECTION_HFG)
                    {
                        CsrUint16 voiceSettings = CSR_BT_SCO_DEFAULT_ACCEPT_VOICE_SETTINGS;

#ifdef CSR_BT_INSTALL_HFG_C2C
                        if ((CsrBtHfgGetMainInstance(inst)->adpcmLocalSupported && inst->audioCodecType > CSR_BT_C2C_ADPCM_OFF_VALUE &&
                            !(inst->remoteSampleRateSupport) ) ||
                            (inst->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC) )
                        {
                            voiceSettings |= CSR_BT_AIRCODING_TRANSPARENT_DATA;
                            CsrBtCmWriteVoiceSettingsReqSend(CSR_BT_HFG_IFACEQUEUE, voiceSettings);
                        }
#endif

                        CsrBtCmScoAcceptConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                                  inst->hfgConnId,
                                                  CSR_BT_COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY,
                                                  CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH,
                                                  CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH,
                                                  CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY,
                                                  voiceSettings,
                                                  CSR_BT_SCO_DEFAULT_ACCEPT_RE_TX_EFFORT);

                    }
                }
            }
            else
            {
                inst->pendingScoDisconnect = FALSE;
                if(inst->linkType == CSR_BT_HFG_CONNECTION_HFG)
                {
                    /* HFG must accept incoming SCO */
                    CsrUint16 voiceSettings = CSR_BT_SCO_DEFAULT_ACCEPT_VOICE_SETTINGS;

#ifdef CSR_BT_INSTALL_HFG_C2C
                    if ((CsrBtHfgGetMainInstance(inst)->adpcmLocalSupported && inst->audioCodecType > CSR_BT_C2C_ADPCM_OFF_VALUE &&
                        !(inst->remoteSampleRateSupport)) ||
                        (inst->lastCodecUsed == CSR_BT_WBS_MSBC_CODEC) )
                    {
                        voiceSettings |= CSR_BT_AIRCODING_TRANSPARENT_DATA;
                        CsrBtCmWriteVoiceSettingsReqSend(CSR_BT_HFG_IFACEQUEUE, voiceSettings);
                    }
#endif

                    CsrBtCmScoAcceptConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                              inst->hfgConnId,
                                              CSR_BT_COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY,
                                              CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH,
                                              CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH,
                                              CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY,
                                              voiceSettings,
                                              CSR_BT_SCO_DEFAULT_ACCEPT_RE_TX_EFFORT);

                }
            }
        }
        else
        {
            /* SCO disconnect failed, so we have to retry... */
            if((inst->pendingSco) &&
               !(inst->pendingScoDisconnect))
            {
                /* SCO still on, disconnect not requested */
                CsrBtHfgSendHfgExtendedAudioInd(inst,
                                           inst->scoHandle,
                                           inst->scoPcmSlot,
                                           0,0,0,0,0,0,
                                           inst->audioCodecType,
                                           inst->sampleRateUsed,
                                           CSR_BT_RESULT_CODE_HFG_SUCCESS,
                                           CSR_BT_SUPPLIER_HFG);
            }
            else
            {

                CsrBtCmScoDisconnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                       inst->hfgConnId);
            }
        }
    }
}

/* SCO accept connection confirm */
void CsrBtHfgConnectedCmScoAcceptConnectCfmHandler(HfgInstance_t *inst)
{
    CsrBtCmScoAcceptConnectCfm *prim;
    HfgMainInstance_t *mainInst;

    prim = (CsrBtCmScoAcceptConnectCfm*)inst->msg;
    mainInst = CsrBtHfgGetMainInstance(inst);

    /* If SCO already connected ignore the message */
    if(inst->scoHandle == CSR_SCHED_QID_INVALID)
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrBool pendingScoTemp;
            if (CsrBtHfgGetMainInstance(inst)->hfgConfig & CSR_BT_HFG_CNF_EXIT_LP_ON_AUDIO)
            {/* Exit LP mode and do not enter it until the SCO is disconnected again*/
                CsrBtHfgLpStop(inst); /* Do not keep on trying to enter sniff mode */
                if (inst->lpMode != CSR_BT_LINK_STATUS_CONNECTED)
                {/* Now try to enter active mode if in LP mode */
                    inst->lpWantedMode = CSR_BT_LINK_STATUS_CONNECTED;
                    CsrBtHfgLpTimeoutHandler(inst);
                }
            }
            inst->scoHandle = prim->eScoHandle;
            inst->scoPcmSlot = prim->pcmSlot;
#ifdef CSR_USE_DSPM
            if ( CsrBtHfgGetMainInstance(inst)->hfgDspmOpPending == DSPM_NOP)
            {/* map the proper stream */
                if (inst->dspmInst.operatorEncoder == HFG_DSPM_DEFAULT_HANDLE)
                {
                    CsrBtHfgDspmSendCreateOperators(inst);
                }
                else
                {/* Connect streams: the operator is already created */
                    CsrHfgDspmAudioConnect(inst);
                }
            }
#endif
            /* Make sure to handle crossing SCO requests this is surely an incoming connection so it should give a CSR_BT_HFG_AUDIO_CONNECT_IND */
            pendingScoTemp = inst->pendingSco;
            inst->pendingSco = FALSE;

            if ((prim->airMode & CSR_BT_AIRCODING_TRANSPARENT_DATA) != 0)
            {
                CsrBtHfgSendHfgExtendedAudioInd(inst,
                                           inst->scoHandle,
                                           inst->scoPcmSlot,
                                           prim->linkType,
                                           prim->weSco,
                                           prim->rxPacketLength,
                                           prim->txPacketLength,
                                           prim->airMode,
                                           prim->txInterval,
                                           inst->audioCodecType,
                                           inst->sampleRateUsed,
                                           CSR_BT_RESULT_CODE_HFG_SUCCESS,
                                           CSR_BT_SUPPLIER_HFG);
            }
            else
            { /* CVSD audio */
                CsrBtHfgSendHfgExtendedAudioInd(inst,
                                           inst->scoHandle,
                                           inst->scoPcmSlot,
                                           prim->linkType,
                                           prim->weSco,
                                           prim->rxPacketLength,
                                           prim->txPacketLength,
                                           prim->airMode,
                                           prim->txInterval,
                                           CSR_BT_C2C_ADPCM_OFF_VALUE,
                                           CSR_BT_C2C_SAMPLE_8KHZ_VALUE,
                                           CSR_BT_RESULT_CODE_HFG_SUCCESS,
                                           CSR_BT_SUPPLIER_HFG);
            }
            inst->pendingSco = pendingScoTemp;
        }
        else
        {
            CsrUint16 voiceSettings = CSR_BT_SCO_DEFAULT_ACCEPT_VOICE_SETTINGS;

#ifdef CSR_BT_INSTALL_HFG_C2C
            if (CsrBtHfgGetMainInstance(inst)->adpcmLocalSupported && inst->audioCodecType >= CSR_BT_C2C_ADPCM_OFF_VALUE)
            { /* This means that the incoming connection failed.... and that it comes from an "Auristream" device!
                 So this HAS TO BE an "old" headset with Auristream support. Just try to negotiate ADPCM audio and
                 establish it from the AG instead.... */
                CsrBool negotiateAdpcm = FALSE;

                if ( ( (inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] >= CSR_BT_C2C_ADPCM_OFF_VALUE) || (inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] > CSR_BT_C2C_ADPCM_OFF_VALUE) ) &&
                        ((inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] & inst->ind.c2c[CSR_BT_C2C_ADPCM_IND]) >= CSR_BT_C2C_ADPCM_OFF_VALUE) )
                {
                    negotiateAdpcm = TRUE;
                }

                inst->incScoCnt++;

                if ((negotiateAdpcm) && (!inst->pendingCodecNegotiation) && (inst->incScoCnt == CSR_BT_HFG_NUMBER_SCO_TRIALS))
                {/* start audio establishment only after negociation */
                    inst->pendingCodecNegotiation = TRUE;
                    if (inst->audioCodecType == 0)
                    {/* Find the proper value to try...*/
                         inst->audioCodecType = inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] & inst->ind.c2c[CSR_BT_C2C_ADPCM_IND];
                        /* make sure only one value is sent, and not a combination of acceptable values... */
                        if (inst->audioCodecType & CSR_BT_C2C_ADPCM_4BIT_VALUE)
                        {/* try the highest value for starters */
                            inst->audioCodecType = CSR_BT_C2C_ADPCM_4BIT_VALUE;
                        }
                        else
                        {
                            if (inst->audioCodecType & CSR_BT_C2C_ADPCM_2BIT_VALUE)
                            {
                                inst->audioCodecType = CSR_BT_C2C_ADPCM_2BIT_VALUE;
                            }
                            else
                            {
                                inst->audioCodecType = CSR_BT_C2C_ADPCM_OFF_VALUE;
                            }
                        }
                    }

                    CsrBtHfgSendAtCsrFn(inst,CSR_BT_C2C_ADPCM_IND,inst->audioCodecType);
                    inst->scoPcmSlot = prim->pcmSlot;
                    inst->scoPcmRealloc = TRUE;
                }
                else
                {
                    if (negotiateAdpcm)
                    {
                        voiceSettings |= CSR_BT_AIRCODING_TRANSPARENT_DATA;
                    }
                    CsrBtCmScoAcceptConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                              inst->hfgConnId,
                                              CSR_BT_COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY,
                                              CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH,
                                              CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH,
                                              CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY,
                                              voiceSettings,
                                              CSR_BT_SCO_DEFAULT_ACCEPT_RE_TX_EFFORT);
                }
            }
            else
#endif
            {
                CsrBtCmScoAcceptConnectReqSend(CSR_BT_HFG_IFACEQUEUE,
                                          inst->hfgConnId,
                                          CSR_BT_COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY,
                                          CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH,
                                          CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH,
                                          CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY,
                                          voiceSettings,
                                          CSR_BT_SCO_DEFAULT_ACCEPT_RE_TX_EFFORT);
            }
        }
    }
#ifdef CSR_BT_INSTALL_HFG_C2C
    /* if we use ADPCM we need to set the codec in the chip right away to prevent */
    /* loud noise from the eSCO transparent_mode                                  */
    if((inst->audioCodecType != CSR_BT_C2C_ADPCM_OFF_VALUE) &&
       ((prim->airMode & CSR_BT_AIRCODING_TRANSPARENT_DATA) != 0)  &&
        (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) &&
        (prim->resultSupplier == CSR_BT_SUPPLIER_CM))
    {
        if ( ( (inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] >= CSR_BT_C2C_ADPCM_OFF_VALUE) || (inst->ind.c2c[CSR_BT_C2C_ADPCM_IND] > CSR_BT_C2C_ADPCM_OFF_VALUE) ) &&
             ((inst->ind.loc2c[CSR_BT_C2C_ADPCM_IND] & inst->ind.c2c[CSR_BT_C2C_ADPCM_IND]) >= CSR_BT_C2C_ADPCM_OFF_VALUE) )
        {
            CsrBtHfgSetC2CAudioCodec(inst->scoHandle, inst->audioCodecType, inst->bccmdSeqNo);
        }
    }
#endif
    /* Notify user of SCO */
    if((mainInst->hfgConfig & CSR_BT_HFG_CNF_AUDIO_STATUS)  &&
        (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) &&
        (prim->resultSupplier == CSR_BT_SUPPLIER_CM))
    {
        CsrBtHfgAudioScoStatus *set;
#ifdef CSR_BT_INSTALL_HFG_C2C
        CsrBtHfgAudioCodecStatus *codecSet;
#endif
        set = (CsrBtHfgAudioScoStatus*)CsrPmemAlloc(sizeof(CsrBtHfgAudioScoStatus));
        set->linkType = prim->linkType;
        set->txInterval = prim->txInterval;
        set->weSco = prim->weSco;
        set->rxPacketLength = prim->rxPacketLength;
        set->txPacketLength = prim->txPacketLength;
        set->airMode = prim->airMode;
        set->resultCode = prim->resultCode;
        set->resultSupplier = prim->resultSupplier;

        CsrBtHfgSendScoHfgStatusAudioInd(inst, set);
#ifdef CSR_BT_INSTALL_HFG_C2C
        if(mainInst->adpcmLocalSupported)
        {
            codecSet = (CsrBtHfgAudioCodecStatus*)CsrPmemAlloc(sizeof(CsrBtHfgAudioCodecStatus));
            codecSet->supportedMask = CSR_BT_AUDIO_CODEC_SUPPORT_MASK;
            if ((prim->airMode & CSR_BT_AIRCODING_TRANSPARENT_DATA) != 0)
            {
                codecSet->currentCodec = inst->audioCodecType;
            }
            else
            {
                codecSet->currentCodec = CSR_BT_C2C_ADPCM_OFF_VALUE;
            }
            CsrBtHfgSendAudioCodecHfgStatusAudioInd(inst,codecSet);
        }
#endif
    }
}

/* Downstream: SCO PCM mapping */
void CsrBtHfgConnectedHfgMapScoPcmResHandler(HfgInstance_t *inst)
{
    CsrBtHfgAudioAcceptConnectRes* prim;
    CsrBtCmScoCommonParms *scoParms = NULL;
    CsrUint8 pcmSlot;

    prim = (CsrBtHfgAudioAcceptConnectRes*)inst->msg;

    if(prim->acceptParameters)
    {
        scoParms = CsrPmemAlloc(sizeof(CsrBtCmScoCommonParms));
        scoParms->audioQuality = prim->acceptParameters->packetTypes;
        scoParms->txBandwidth = prim->acceptParameters->txBandwidth;
        scoParms->rxBandwidth = prim->acceptParameters->rxBandwidth;
        scoParms->maxLatency = prim->acceptParameters->maxLatency;
        scoParms->voiceSettings = prim->acceptParameters->contentFormat;
        scoParms->reTxEffort = prim->acceptParameters->reTxEffort;
        CsrPmemFree(prim->acceptParameters);
    }
#ifdef CSR_USE_DSPM
    pcmSlot = CSR_BT_PCM_DONT_MAP;
#else
    pcmSlot = prim->pcmSlot;
#endif

    CsrBtCmMapScoPcmExtResSend(inst->hfgConnId,
                       prim->acceptResponse,
                       scoParms,
                       pcmSlot,
                       prim->pcmReassign);
}

/* Upstream: SCO PCM mapping */
void CsrBtHfgXCmMapScoPcmIndHandler(HfgInstance_t *inst)
{
    /* Simply forward CM indication as HFG */
    CsrBtHfgSendHfgMapScoPcmInd(inst);
}

