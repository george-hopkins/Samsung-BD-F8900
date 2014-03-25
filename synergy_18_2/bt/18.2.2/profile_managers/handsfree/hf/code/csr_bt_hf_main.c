/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_bt_result.h"
#include "csr_env_prim.h"
#include "csr_bt_util.h"
#include "csr_bt_hf_main.h"
#include "csr_bt_hf_main_sef.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_hf_connect_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#ifdef CSR_USE_DSPM
#include "csr_dspm_lib.h"
#include "csr_bt_hf_dspm.h"
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

static const HfStateHandlerType hfMainStateHandlers[HfMainNumberOfStates][CSR_BT_HF_PRIM_DOWNSTREAM_COUNT] =
{
    /* Null_s */
    {
        CsrBtHfXStateActivateReqHandler,                              /* CSR_BT_HF_ACTIVATE_REQ */
        CsrBtHfNullStateDeactivateReqHandler,                         /* CSR_BT_HF_DEACTIVATE_REQ */
        CsrBtHfXStateConfigLowPowerReqHandler,                        /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
#ifdef CSR_BT_INSTALL_HF_CONFIG_AUDIO
        CsrBtHfXStateConfigAudioReqHandler,                           /* CSR_BT_HF_CONFIG_AUDIO_REQ */
#else
        NULL,                                                         /* CSR_BT_HF_CONFIG_AUDIO_REQ */
#endif
        CsrBtHfSaveMessage,                                           /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_DISCONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                         /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_BT_INPUT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GENERATE_DTMF_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_DIAL_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_ANSWER_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_END_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_HANDLING_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_AT_CMD_REQ */
        HsHfSecurityInReqHandler,                                     /* CSR_BT_HF_SECURITY_IN_REQ */
        HsHfSecurityOutReqHandler,                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        CsrBtHfXStateHfC2cSfReqHandler,                               /* CSR_BT_HF_C2C_SF_REQ */
        CsrBtHfSaveMessage,                                           /* CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */
        CsrBtHfGetC2CAdpcmLocalSupportedReqHandler,                   /* CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */
        CsrBtHfMainIgnoreMessage,                                     /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_PWR_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_BATT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_GET_SMS_REQ */
        CsrBtHfXStateSetDeregisterTimeReqHandler,                     /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
        CsrBtHfXStateHfUpdateCodecSupportReqHandler,                  /* CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */
    },
    /* Activated_s */
    {
        CsrBtHfXStateActivateReqHandler,                              /* CSR_BT_HF_ACTIVATE_REQ */
        CsrBtHfActivatedStateHfDeactivateReqHandler,                  /* CSR_BT_HF_DEACTIVATE_REQ */
        CsrBtHfXStateConfigLowPowerReqHandler,                        /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
#ifdef CSR_BT_INSTALL_HF_CONFIG_AUDIO
        CsrBtHfXStateConfigAudioReqHandler,                           /* CSR_BT_HF_CONFIG_AUDIO_REQ */
#else
        NULL,                                                         /* CSR_BT_HF_CONFIG_AUDIO_REQ */
#endif
        CsrBtHfActivatedStateHfServiceConnectReq,                     /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        CsrBtHfActivatedStateHfCancelReqHandler,                      /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        CsrBtHfActivatedStateHfDisconnectReqHandler,                  /* CSR_BT_HF_DISCONNECT_REQ */
        CsrBtHfActivatedStateAudioReqHandler,                         /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        CsrBtHfActivatedStateMapScoPcmResHandler,                     /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        CsrBtHfActivatedStateAudioDisconnectReqHandler,               /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_BT_INPUT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GENERATE_DTMF_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_DIAL_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_ANSWER_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_END_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_HANDLING_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_AT_CMD_REQ */
        HsHfSecurityInReqHandler,                                     /* CSR_BT_HF_SECURITY_IN_REQ */
        HsHfSecurityOutReqHandler,                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        CsrBtHfXStateHfC2cSfReqHandler,                               /* CSR_BT_HF_C2C_SF_REQ */
        CsrBtHfMainIgnoreMessage,                                     /* CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */
        CsrBtHfGetC2CAdpcmLocalSupportedReqHandler,                   /* CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */
        CsrBtHfMainIgnoreMessage,                                     /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_PWR_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_BATT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_GET_SMS_REQ */
        CsrBtHfXStateSetDeregisterTimeReqHandler,                     /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
        CsrBtHfXStateHfUpdateCodecSupportReqHandler,                  /* CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */
    },
    /* Deactivate_s */
    {
        NULL,                                                         /* CSR_BT_HF_ACTIVATE_REQ */
        CsrBtHfMainIgnoreMessage,                                     /* CSR_BT_HF_DEACTIVATE_REQ */
        CsrBtHfXStateConfigLowPowerReqHandler,                        /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
#ifdef CSR_BT_INSTALL_HF_CONFIG_AUDIO
        CsrBtHfXStateConfigAudioReqHandler,                           /* CSR_BT_HF_CONFIG_AUDIO_REQ */
#else
        NULL,                                                         /* CSR_BT_HF_CONFIG_AUDIO_REQ */
#endif
        NULL,                                                         /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_DISCONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        NULL,                                                         /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                         /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_BT_INPUT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_GENERATE_DTMF_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_DIAL_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_ANSWER_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_END_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_CALL_HANDLING_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_AT_CMD_REQ */
        HsHfSecurityInReqHandler,                                     /* CSR_BT_HF_SECURITY_IN_REQ */
        HsHfSecurityOutReqHandler,                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        NULL,                                                         /* CSR_BT_HF_C2C_SF_REQ */
        NULL,                                                         /* CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */
        CsrBtHfGetC2CAdpcmLocalSupportedReqHandler,                   /* CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */
        CsrBtHfMainIgnoreMessage,                                     /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_PWR_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_BATT_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_C2C_GET_SMS_REQ */
        CsrBtHfXStateSetDeregisterTimeReqHandler,                     /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        CsrBtHfXStateHfCommonAtCmdReqHandler,                         /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
        CsrBtHfXStateHfUpdateCodecSupportReqHandler,                  /* CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */
    }
};

/* ---------- CM jump table ---------- */
/* PLEASE NOTE: only the top most part of the CM primitives are included */
static const HfStateHandlerType mainCmStateHandlers[HfMainNumberOfUpstreamStates][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{

    /* Null_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtHfNullStateCmRegisterCfmHandler,                    /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                                    /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_DATA_IND */
        NULL,                                                    /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                    /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Activated_s */
    {
        CsrBtHfActivatedStateCmCancelAcceptConnectCfmHandler,    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        CsrBtHfActivatedStateCmConnectAcceptCfmHandler,          /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtHfActivatedStateCmDisconnectIndHandler,             /* CSR_BT_CM_DISCONNECT_IND */
        CsrBtHfActivatedStateCmScoConnectCfmHandler,             /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtHfActivatedStateCmScoDisconnectIndHandler,          /* CSR_BT_CM_SCO_DISCONNECT_IND */
        CsrBtHfActivatedStateCmScoAcceptConnectCfmHandler,       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtHfActivatedStateCmDataIndHandler,                   /* CSR_BT_CM_DATA_IND */
        CsrBtHfActivatedStateCmDataCfmHandler,                   /* CSR_BT_CM_DATA_CFM */
        CsrBtHfXStateIgnoreCmControlIndHandler,                  /* CSR_BT_CM_CONTROL_IND */
        CsrBtHfActivatedStateCmModeChangeIndHandler,             /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtHfActivatedStateCmPortnegIndHandler,                /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Deactivate_s */
    {
        CsrBtHfDeactivateStateCmCancelAcceptConnectCfmHandler,   /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        CsrBtHfDeactivateStateCmConnectAcceptCfmHandler,         /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtHfDeactivateStateCmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        CsrBtHfDeactivateStateCmScoConnectCfmHandler,            /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtHfDeactivateStateCmScoDisconnectIndHandler,         /* CSR_BT_CM_SCO_DISCONNECT_IND */
        CsrBtHfDeactivateStateCmScoAcceptConnectCfm,             /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtHfDeactivateStateCmDataIndHandler,                  /* CSR_BT_CM_DATA_IND */
        CsrBtHfDeactivateStateCmDataCfmHandler,                  /* CSR_BT_CM_DATA_CFM */
        CsrBtHfXStateIgnoreCmControlIndHandler,                  /* CSR_BT_CM_CONTROL_IND */
        CsrBtHfDeactivateStateCmModeChangeIndHandler,            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    }
};

void CsrBtHfInit(void **gash)
{
    HfMainInstanceData_t  *instData;

    /* allocate and initialise instance data space */
    *gash    = CsrPmemAlloc(sizeof(HfMainInstanceData_t));
    instData = (HfMainInstanceData_t *) *gash;
    CsrMemSet((CsrUint8 *)instData,0,sizeof(HfMainInstanceData_t));
    /* Init common instance data */
    instData->linkRegistering           = CSR_BT_HF_CONNECTION_UNKNOWN;
    instData->appHandle                 = 0xAA;
    CsrBtScSetSecInLevel(&instData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_HANDSFREE_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HANDSFREE_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HF_SUCCESS,
        CSR_BT_RESULT_CODE_HF_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecOutLevel(&instData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_HANDSFREE_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HANDSFREE_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HF_SUCCESS,
        CSR_BT_RESULT_CODE_HF_UNACCEPTABLE_PARAMETER);

    instData->searchIdxHf = 0xFF;
    instData->searchIdxHs = 0xFF;
    instData->generalAudioParams.theAudioQuality = CSR_BT_COMPLETE_SCO_DEFAULT_ACCEPT_AUDIO_QUALITY; /* 0x38F */
    instData->generalAudioParams.theTxBandwidth  = CSR_BT_ESCO_DEFAULT_CONNECT_TX_BANDWIDTH;   /*8000*/
    instData->generalAudioParams.theRxBandwidth  = CSR_BT_ESCO_DEFAULT_CONNECT_RX_BANDWIDTH;   /*8000*/
    instData->generalAudioParams.theMaxLatency  = CSR_BT_SCO_DEFAULT_ACCEPT_MAX_LATENCY;       /*0xFF*/
    instData->generalAudioParams.theVoiceSettings = CSR_BT_ESCO_DEFAULT_CONNECT_VOICE_SETTINGS; /*CVSD*/
    instData->generalAudioParams.theReTxEffort = CSR_BT_SCO_DEFAULT_ACCEPT_RE_TX_EFFORT;       /*0xFF*/

    instData->sdpHfSearchData          = CsrBtUtilSdcInit(CsrBtHfSdcResultHandler, CSR_BT_HF_IFACEQUEUE);
    instData->sdpHfSearchConData       = CsrBtUtilSdpRfcConInit(CsrBtHfSdcSelectServiceHandler,NULL,CsrBtHfRfcSdcConResultHandler, CSR_BT_HF_IFACEQUEUE);
    instData->atRespWaitTime           = CSR_BT_AT_DEFAULT_RESPONSE_TIME;
    instData->modemStatus              = CSR_BT_MODEM_SEND_CTRL_DTE_DEFAULT; 
#ifdef CSR_USE_DSPM
    /* Init main dspm data */

    instData->dspmInst.scoSourceHandle  = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.scoSinkHandle    = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.sourceHandle     = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.sinkHandle       = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.operatorEncoder  = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.operatorDecoder  = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.nextOperatorEncoder = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.nextOperatorDecoder = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.connIdAudioDsp   = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.connIdDspSco     = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.connIdScoDsp     = HF_DSPM_DEFAULT_HANDLE;
    instData->dspmInst.connIdDspAudio   = HF_DSPM_DEFAULT_HANDLE;

    instData->dspmInst.sourceType = CSR_BT_HFP_DEFAULT_AUDIO_CONNECTION_POINT;
    instData->dspmInst.sinkType   = CSR_BT_HFP_DEFAULT_AUDIO_CONNECTION_POINT;

    /* Now register to the DSPM */
    CsrDspmActivateReqSend(CSR_BT_HF_IFACEQUEUE);
    instData->dspmHfCallBack = CsrHfDspmActivateRspHandler;
#endif
    instData->supportedCodecsMask      = CSR_BT_WBS_CVSD_CODEC_MASK | CSR_BT_WBS_MSBC_CODEC_MASK;
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
 *  This function is called by the scheduler to perform a graceful shutdown
 *  of a scheduler task.
 *  This function must:
 *  1)  empty the input message queue and free any allocated memory in the
 *      messages.
 *  2)  free any instance data that may be allocated.
 ****************************************************************************/
void CsrBtHfDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrUintFast8  i;
    CsrUint8 total;
    CsrBool    lastMsg;
    HfMainInstanceData_t  *instData = (HfMainInstanceData_t *) (*gash);
    HfInstanceData_t *linkPtr = instData->linkData;

    /* continue to poll any message of the input queue */
    lastMsg = FALSE;
    total = instData->maxHFConnections + instData->maxHSConnections + instData->allocInactiveHsCons + instData->allocInactiveHfCons;
    while (!lastMsg)
    {
        msg_data = NULL;
        msg_type = 0;
        lastMsg = CsrMessageQueuePop(&instData->saveQueue, &msg_type, &msg_data);


        i = 0;
        while(!lastMsg && (i<total) && linkPtr != NULL)
        {
            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
            if (linkPtr != NULL)
            {
                if (linkPtr->data != NULL)
                {
                    lastMsg = CsrMessageQueuePop(&(linkPtr->data->cmDataReqQueue),&msg_type,&msg_data);
                }
                i++;
            }
        }


        if(!lastMsg)
                {
                    lastMsg = !CsrSchedMessageGet(&msg_type, &msg_data);
                }

        if((!lastMsg) && (msg_data != NULL))
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_BT_HF_PRIM:
                {
                    CsrBtHfFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }
                case CSR_AM_PRIM:
                case CSR_SCHED_PRIM:
                    break;
#ifdef CSR_USE_DSPM
                case CSR_DSPM_PRIM:
                {
                    CsrDspmFreeUpstreamMessageContents(CSR_DSPM_PRIM, instData->recvMsgP);
                    break;
                }
#endif
                default:
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrGeneralException("HF",
                                           msg_type,
                                           0,
                                           "Unhandled PRIM in HF Deinit");
#endif
                        break;
                    }
            }
            CsrPmemFree (msg_data);
        }
    }

    linkPtr = instData->linkData;
#ifdef CSR_USE_DSPM
    instData->dspmHfCallBack = NULL;
#endif
    for (i=0; i<total;i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        if (linkPtr != NULL)
        {
            if (linkPtr->cindSupportString != NULL)
            {
                CsrPmemFree (linkPtr->cindSupportString);
                linkPtr->cindSupportString = NULL;
            }
            if (linkPtr->cindStartValueString != NULL)
            {
                CsrPmemFree (linkPtr->cindStartValueString);
                linkPtr->cindStartValueString = NULL;
            }
            if (linkPtr->chldStringStored != NULL)
            {
                CsrPmemFree (linkPtr->chldStringStored);
                linkPtr->chldStringStored = NULL;
            }
            if (linkPtr->data != NULL)
            {
                if (linkPtr->data->recvAtCmd != NULL)
                {
                    CsrPmemFree(linkPtr->data->recvAtCmd);
                    linkPtr->data->recvAtCmd = NULL;
                }
                CsrPmemFree(linkPtr->data);
                linkPtr->data = NULL;
            }
#ifdef CSR_USE_DSPM
            /* If operators allocated, make sure to free them! */
            CsrBtHfDspmCleanUpOperators(linkPtr);
            CsrPmemFree(linkPtr->dspmInst.sourceWbsConfigParams);
            CsrPmemFree(linkPtr->dspmInst.sourceNbConfigParams);
            CsrPmemFree(linkPtr->dspmInst.sinkWbsConfigParams);
            CsrPmemFree(linkPtr->dspmInst.sinkNbConfigParams);
#endif
        }
        else
        {
            i = (instData->maxHFConnections + instData->maxHSConnections);
        }
    }

    linkPtr = instData->linkData;
    if (linkPtr != NULL)
    {
        CsrPmemFree(linkPtr);
        linkPtr = NULL;
    }
    if (instData->sdpHfSearchData != NULL)
    {
        CsrBtUtilSdcRfcDeinit(&(instData->sdpHfSearchData));
        instData->sdpHfSearchData = NULL;
    }
    if (instData->sdpHfSearchConData)
    {
        CsrBtUtilSdcRfcDeinit(&(instData->sdpHfSearchConData));
        instData->sdpHfSearchConData = NULL;
    }
#ifdef CSR_USE_DSPM
    CsrPmemFree(instData->dspmInst.sourceWbsConfigParams);
    CsrPmemFree(instData->dspmInst.sourceNbConfigParams);
    CsrPmemFree(instData->dspmInst.sinkWbsConfigParams);
    CsrPmemFree(instData->dspmInst.sinkNbConfigParams);
#endif
    CsrPmemFree(instData);
}
#endif

void CsrBtHfCleanup_queue(HfMainInstanceData_t* instData)
{
    CsrUint16 msg_type;
    void *msg_data;
    CsrBool    lastMsg;
    CsrUint8 i = 0;

    /* continue to poll any message of the input queue */
    lastMsg = FALSE;

    while (!lastMsg)
    {
        msg_data = NULL;
        msg_type = 0;
        lastMsg = CsrMessageQueuePop(&instData->saveQueue, &msg_type, &msg_data);


        i = 0;
        while(!lastMsg && (i<(instData->maxHFConnections + instData->maxHSConnections)))
        {
            lastMsg = CsrMessageQueuePop(&(instData->linkData[i].data->cmDataReqQueue),&msg_type,&msg_data);
            i++;
        }


        if(!lastMsg)
                {
                    lastMsg = !CsrSchedMessageGet(&msg_type, &msg_data);
                }

        if((!lastMsg) && (msg_data != NULL))
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg_data);
                    break;
                }

                case CSR_BT_HF_PRIM:
                    {
                    CsrBtHfFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }
                
                case CSR_AM_PRIM:
                case CSR_SCHED_PRIM:
                    break;

                default:
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrGeneralException("HF",
                                           msg_type,
                                           0,
                                           "Unhandled PRIM in HF Deinit");
#endif
                        break;
                    }
            }
            CsrPmemFree (msg_data);
        }
    }
}

void CsrBtHfCleanup_reinit_instance(HfMainInstanceData_t *instData)
{
    /* Reinit common instance data Differs from initial init in that
     * it does not reserves an internal server channel, since one has
     * already been allocated */
    CsrUintFast8 i=0;
    HfInstanceData_t *linkPtr = instData->linkData;

    CsrPmemFree(instData->recvMsgP);
    instData->recvMsgP = NULL;
    CsrBtHfCleanup_queue(instData);
    instData->saveQueue = NULL;
    instData->restoreFlag = FALSE;
    instData->linkRegistering = CSR_BT_HF_CONNECTION_UNKNOWN;
    instData->registerPending = FALSE;
    instData->connectSdcActive = FALSE;
    instData->deactivated = FALSE;
    instData->doingCleanup = FALSE;
    instData->lpMask                   = 0;
    instData->reActivating             = 0;
    instData->modemStatus              = CSR_BT_MODEM_SEND_CTRL_DTE_DEFAULT; 

    for (i=0;i<(CsrUintFast8)(instData->maxHFConnections + instData->maxHSConnections);i++)
    {
        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
        linkPtr[i].serviceRecHandle = 0;
        linkPtr[i].sdsRegistered = FALSE;
        linkPtr[i].deactivated = TRUE;
        if (linkPtr[i].cindSupportString != NULL)
        {
            CsrPmemFree(linkPtr[i].cindSupportString);
            linkPtr[i].cindSupportString = NULL;
        }
        if (linkPtr[i].cindStartValueString != NULL)
        {
            CsrPmemFree(linkPtr[i].cindStartValueString);
            linkPtr[i].cindStartValueString = NULL;
        }
        if (linkPtr[i].chldStringStored != NULL)
        {
            CsrPmemFree(linkPtr[i].chldStringStored);
            linkPtr[i].chldStringStored = NULL;
        }
        instData->index = (CsrUint8)i;
        CsrBtHfInitInstanceData(instData);
    }
}

void CsrBtHfCleanup(HfMainInstanceData_t *instData)
{
    /* Purge queue */
    CsrBtHfCleanup_queue(instData);

    /*Purge app_queue*/

    /* Reset instance data */
    CsrBtHfCleanup_reinit_instance(instData);
}

void CsrBtHfHandler(void **gash)
{
    HfMainInstanceData_t    *instData = (HfMainInstanceData_t *) (*gash);
    void                *msg=NULL;
    HfInstanceData_t *linkPtr = instData->linkData;


    if(!instData->restoreFlag)
    {
        CsrSchedMessageGet(&instData->eventClass  , &msg);
    }
    else
    {
        if(!CsrMessageQueuePop(&instData->saveQueue, &instData->eventClass , &msg))
        {
            CsrSchedMessageGet(&instData->eventClass  , &msg);
        }
        instData->restoreFlag = FALSE;
    }
    instData->recvMsgP = msg;

    switch(instData->eventClass)
    {
        case CSR_BT_HF_PRIM:
            {
                CsrBtHfPrim         *primType;

                /* find the message type */
                primType = (CsrBtHfPrim *)msg;
                if ((*primType < CSR_BT_HF_PRIM_DOWNSTREAM_COUNT) &&
                    (instData->state < HfMainNumberOfStates) &&
                    (hfMainStateHandlers[instData->state][*primType] != NULL))
                {
                    hfMainStateHandlers[instData->state][*primType](instData);
                }
                else if (*primType != CSR_BT_HF_HOUSE_CLEANING)
                {
                    CsrBtHfFreeDownstreamMessageContents(instData->eventClass, msg);
                    /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrGeneralException("HF",
                                       instData->state,
                                       *primType,
                                       "Unknown HF prim or undefined state");
#endif
                }

                break;
            }

        case CSR_BT_CM_PRIM:
        {
            CsrPrim *primType;
            /* find the message type */
            primType = (CsrPrim *)msg;
            if (((CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)< CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT) &&
                (instData->state < HfMainNumberOfUpstreamStates) &&
                (mainCmStateHandlers[instData->state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                mainCmStateHandlers[instData->state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if (CSR_BT_CM_MAP_SCO_PCM_IND == *primType)
            {
                CsrBtHfMainXStateMapScoPcmIndHandler(instData);
            }
            else if ((CSR_BT_CM_REGISTER_CFM == *primType) && (instData->reActivating > 0))
            {
                CsrBtHfNullStateCmRegisterCfmHandler(instData);
            }
            else if (CsrBtUtilRfcConVerifyCmMsg(instData->recvMsgP))
            {
                CsrUint8 serverChanRcvd = CSR_BT_NO_SERVER;
                CsrBool found = FALSE, searchAndConFound = FALSE;
                CsrUintFast8 i=0;

                switch (*primType)
                {
                    case CSR_BT_CM_SDC_SEARCH_IND:
                        {
                            serverChanRcvd = ((CsrBtCmSdcSearchInd *)primType)->localServerChannel;
                            break;
                        }
                    case CSR_BT_CM_SDC_SEARCH_CFM:
                        {
                            serverChanRcvd = ((CsrBtCmSdcSearchCfm *)primType)->localServerChannel;
                            break;
                        }
                    case CSR_BT_CM_SDC_CLOSE_IND:
                        {
                            serverChanRcvd = ((CsrBtCmSdcCloseInd *)primType)->localServerChannel;
                            break;
                        }
                    case CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM:
                        {
                            serverChanRcvd = ((CsrBtCmSdcReleaseResourcesCfm *)primType)->localServerChannel;
                            break;
                        }
                    case CSR_BT_CM_SDC_ATTRIBUTE_CFM:
                        {
                            serverChanRcvd = ((CsrBtCmSdcAttributeCfm *)primType)->localServerChannel;
                            break;
                        }
                    default:
                        break;
                }

                for (i=0;((i<(CsrUintFast8)(instData->maxHFConnections + instData->maxHSConnections)) && (!found));i++)
                {
                    if (linkPtr[i].serverChannel == serverChanRcvd)
                    {
                        found = TRUE;
                        if (linkPtr[i].searchAndCon)
                        {
                            searchAndConFound = TRUE;
                        }
                        linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                    }
                }
                if (!found)
                {
                    for (i=0;((i<(CsrUintFast8)(instData->maxHFConnections + instData->maxHSConnections)) && (!searchAndConFound));i++)
                    {
                        if (linkPtr[i].searchAndCon == TRUE)
                        {
                            searchAndConFound = TRUE;
                            linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                        }
                    }
                }
                if ((*primType == CSR_BT_CM_SDC_ATTRIBUTE_CFM) || found || (serverChanRcvd == CSR_BT_NO_SERVER))
                {

                    if ( searchAndConFound )
                    {
                        CsrBtUtilRfcConCmMsgHandler(instData, instData->sdpHfSearchConData, instData->recvMsgP);
                    }
                    else
                    {
                        CsrBtUtilSdcCmMsgHandler(instData, instData->sdpHfSearchData, instData->recvMsgP);
                    }
                }
                else
                {
                    if ( searchAndConFound )
                    {
                        CsrBtUtilRfcConCmMsgHandler(instData, instData->sdpHfSearchConData, instData->recvMsgP);
                    }
                    else
                    {
                        CsrBtUtilSdcCmMsgHandler(instData, instData->sdpHfSearchData, instData->recvMsgP);
                    }
                }
            }
            else if (*primType  == CSR_BT_CM_SDS_REGISTER_CFM)
            {
                if ((instData->state == Null_s) || (instData->reActivating > 0))
                {
                    CsrBtHfNullStateCmSdsRegisterCfmHandler(instData);
                }
                else if (instData->state == Activated_s)
                {
                    CsrBtHfActivatedStateCmSdsRegisterCfmHandler(instData);
                }
                else /* Deactivate. Only possible in case of crossing signals. */
                {
                    CsrBtHfDeactivateStateCmSdsRegisterCfmHandler(instData);
                }
            }
            else if ((*primType  == CSR_BT_CM_SDS_UNREGISTER_CFM) &&
                     ( (instData->state == Deactivate_s) || (instData->state == Activated_s) ))
            {
                if (instData->state == Activated_s)
                {
                    CsrBtHfActivatedStateCmSdsUnregisterCfmHandler(instData);
                }
                else if (instData->state == Deactivate_s)
                {
                    CsrBtHfDeactivateStateCmSdsUnregisterCfmHandler(instData);
                }
            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
               if ((*primType  != CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM) && (instData->state != Null_s))
               {
                CsrGeneralException("HF", instData->state,
                                   (CsrUint16)(*primType - CSR_PRIM_UPSTREAM),
                                   "Unknown CM prim or undefined state");
               }
#endif
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsgP);
            }
            break;
        }
#ifdef CSR_USE_DSPM
       case CSR_DSPM_PRIM:
        {
            if (instData->dspmHfCallBack != NULL)
            {/* Just call the call back function */
                instData->dspmHfCallBack(instData);
            }
            else
            {
                CsrDspmFreeUpstreamMessageContents(CSR_DSPM_PRIM, instData->recvMsgP);
            }
            break;
        }
#endif
        case CSR_SCHED_PRIM:
            {
                CsrEnvPrim *primType;
                primType = (CsrEnvPrim *) msg;
                switch(*primType)
                {
                    case CSR_CLEANUP_IND:
                        {
                            CsrCleanupInd* prim;
                            prim = (CsrCleanupInd*) msg;
                            if(prim->phandle == instData->appHandle)
                            {
                                CsrUintFast8 i = 0;
                                CsrBool  cleanUp = TRUE;

                                instData->doingCleanup = TRUE;
                                for (i=0;((i<(CsrUintFast8)(instData->maxHFConnections + instData->maxHSConnections)) && (cleanUp));i++)
                                {
                                    linkPtr = (HfInstanceData_t *)&(instData->linkData[i]);
                                    if(linkPtr->state != Activate_s)
                                    {
                                        instData->index = (CsrUint8)i;
                                        if (linkPtr->linkType == CSR_BT_HF_CONNECTION_HF)
                                        {
                                            DeactivateHfHandler(instData);
                                        }
                                        else
                                        {
                                            DeactivateHsHandler(instData);
                                        }
                                        cleanUp = FALSE;
                                    }
                                }
                                if (cleanUp)
                                {
                                    CsrBtHfCleanup(instData);
                                }
                            }
                            break;
                        }
                    default:
                        {
                            /* Other primitives are ignored if exception handler module is excluded! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrGeneralException("HF",
                                               instData->state,
                                               *primType,
                                               "Unknown ENV prim or undefined state");
#endif
                        }
                }
                break;
            }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                /* State/Event ERROR! */
                CsrGeneralException("HF",
                                   instData->state,
                                   0xFF,
                                   "Unknown primitive type received in main");
            }
#endif
    }
    CsrPmemFree(instData->recvMsgP);    /* free the received message. if the ptr is NULL just CsrSched just ignores */
}


