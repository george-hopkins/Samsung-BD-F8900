/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_hf_handler.h"
#include "csr_bt_hf_connect_sef.h"
#include "csr_bt_hf_call_sef.h"
#include "csr_bt_hfhs_data_sef.h"
#include "csr_bt_cm_prim.h"
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* ---------- HF jump table ---------- */
static const HfStateHandlerType hfStateHandlers[HfNumberOfStates][CSR_BT_HF_PRIM_DOWNSTREAM_COUNT] =
{
    /* Activate_s */
    {
        NULL,                                                    /* CSR_BT_HF_ACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_DEACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_AUDIO_REQ */
        NULL,                                                    /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                    /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        NULL,                                                    /* CSR_BT_HF_BT_INPUT_REQ */
        NULL,                                                    /* CSR_BT_HF_GENERATE_DTMF_REQ */
        NULL,                                                    /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        NULL,                                                    /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        NULL,                                                    /* CSR_BT_HF_DIAL_REQ */
        NULL,                                                    /* HF_ANSWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CALL_END_REQ */
        NULL,                                                    /* CSR_BT_HF_CALL_HANDLING_REQ */
        NULL,                                                    /* CSR_BT_HF_AT_CMD_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_IN_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_SF_REQ */
        NULL,                                                    /* CsrBtHfSetC2cAudioCodecReq */
        NULL,                                                    /* CsrBtHfGetC2cAdpcmLocalSupportedReq */
        NULL,                                                    /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_PWR_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_BATT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_GET_SMS_REQ */
        NULL,                                                    /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        NULL,                                                    /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
    },
    /* Connect_s */
    {
        NULL,                                                    /* CSR_BT_HF_ACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_DEACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_AUDIO_REQ */
        NULL,                                                    /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        CsrBtHfXStateHfCancelReqHandler,                              /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        CsrBtHfXStateHfCancelReqHandler,                              /* CSR_BT_HF_DISCONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                    /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        NULL,                                                    /* CSR_BT_HF_BT_INPUT_REQ */
        NULL,                                                    /* CSR_BT_HF_GENERATE_DTMF_REQ */
        NULL,                                                    /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        NULL,                                                    /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        NULL,                                                    /* CSR_BT_HF_DIAL_REQ */
        NULL,                                                    /* HF_ANSWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CALL_END_REQ */
        NULL,                                                    /* CSR_BT_HF_CALL_HANDLING_REQ */
        NULL,                                                    /* CSR_BT_HF_AT_CMD_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_IN_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_SF_REQ */
        NULL,                                                    /* CsrBtHfSetC2cAudioCodecReq */
        NULL,                                                    /* CsrBtHfGetC2cAdpcmLocalSupportedReq */
        NULL,                                                    /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_PWR_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_BATT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_GET_SMS_REQ */
        NULL,                                                    /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        NULL,                                                    /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
    },
    /* Connected_s */
    {
        NULL,                                                    /* CSR_BT_HF_ACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_DEACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_AUDIO_REQ */
        NULL,                                                    /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        CsrBtHfXStateHfDisconnectReqHandler,                     /* CSR_BT_HF_DISCONNECT_REQ */
        CsrBtHfConnectedStateHfAudioReqHandler,                  /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        CsrBtHfConnectedStateHfAudioDisconnectReqHandler,        /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        NULL,                                                    /* CSR_BT_HF_BT_INPUT_REQ */
        NULL,                                                    /* CSR_BT_HF_GENERATE_DTMF_REQ */
        CsrBtHfConnectedStateHfSpeakerGainStatusReqHandler,      /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        CsrBtHfConnectedStateHfMicGainStatusReqHandler,          /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        NULL,                                                    /* CSR_BT_HF_DIAL_REQ */
        CsrBtHfConnectedStateHfAnswerReqHandler,                 /* HF_ANSWER_REQ */
        CsrBtHfConnectedStateHfCallEndReqHandler,                /* CSR_BT_HF_CALL_END_REQ */
        CsrBtHfConnectedStateHfChldReqHandler,                   /* CSR_BT_HF_CALL_HANDLING_REQ */
        CsrBtHfConnectedStateHfAtCmdReqHandler,                  /* CSR_BT_HF_AT_CMD_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_IN_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_SF_REQ */
        NULL,                                                    /* CsrBtHfSetC2cAudioCodecReq */
        NULL,                                                    /* CsrBtHfGetC2cAdpcmLocalSupportedReq */
        NULL,                                                    /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_PWR_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_BATT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_GET_SMS_REQ */
        NULL,                                                    /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        NULL,                                                    /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
    },
    /* LpEnabled_s */
    {
        NULL,                                                    /* CSR_BT_HF_ACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_DEACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_AUDIO_REQ */
        NULL,                                                    /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        CsrBtHfXStateHfDisconnectReqHandler,                     /* CSR_BT_HF_DISCONNECT_REQ */
        CsrBtHfLpEnabledHfXReqHandler,                           /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                    /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        CsrBtHfLpEnabledHfXReqHandler,                           /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        NULL,                                                    /* CSR_BT_HF_BT_INPUT_REQ */
        NULL,                                                    /* CSR_BT_HF_GENERATE_DTMF_REQ */
        CsrBtHfConnectedStateHfSpeakerGainStatusReqHandler,      /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        CsrBtHfConnectedStateHfMicGainStatusReqHandler,          /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        NULL,                                                    /* CSR_BT_HF_DIAL_REQ */
        CsrBtHfConnectedStateHfAnswerReqHandler,                 /* HF_ANSWER_REQ */
        CsrBtHfConnectedStateHfCallEndReqHandler,                /* CSR_BT_HF_CALL_END_REQ */
        CsrBtHfConnectedStateHfChldReqHandler,                   /* CSR_BT_HF_CALL_HANDLING_REQ */
        CsrBtHfConnectedStateHfAtCmdReqHandler,                  /* CSR_BT_HF_AT_CMD_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_IN_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_SF_REQ */
        NULL,                                                    /* CsrBtHfSetC2cAudioCodecReq */
        NULL,                                                    /* CsrBtHfGetC2cAdpcmLocalSupportedReq */
        NULL,                                                    /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_PWR_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_BATT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_GET_SMS_REQ */
        NULL,                                                    /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        NULL,                                                    /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
    },
    /* ServiceSearch_s */
    {
        NULL,                                                    /* CSR_BT_HF_ACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_DEACTIVATE_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_LOW_POWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CONFIG_AUDIO_REQ */
        NULL,                                                    /* CSR_BT_HF_SERVICE_CONNECT_REQ */
        CsrBtHfXStateHfCancelReqHandler,                         /* CSR_BT_HF_CANCEL_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_DISCONNECT_REQ */
        CsrBtHfSaveMessage,                                      /* CSR_BT_HF_AUDIO_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
        NULL,                                                    /* CSR_BT_HF_AUDIO_DISCONNECT_CONNECT_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
        NULL,                                                    /* CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
        NULL,                                                    /* CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
        NULL,                                                    /* CSR_BT_HF_BT_INPUT_REQ */
        NULL,                                                    /* CSR_BT_HF_GENERATE_DTMF_REQ */
        CsrBtHfSaveMessage,                                      /* CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
        CsrBtHfSaveMessage,                                      /* CSR_BT_HF_MIC_GAIN_STATUS_REQ */
        NULL,                                                    /* CSR_BT_HF_DIAL_REQ */
        CsrBtHfSaveMessage,                                      /* HF_ANSWER_REQ */
        NULL,                                                    /* CSR_BT_HF_CALL_END_REQ */
        NULL,                                                    /* CSR_BT_HF_CALL_HANDLING_REQ */
        CsrBtHfSaveMessage,                                      /* CSR_BT_HF_AT_CMD_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_IN_REQ */
        NULL,                                                    /* CSR_BT_HF_SECURITY_OUT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_SF_REQ */
        NULL,                                                    /* CsrBtHfSetC2cAudioCodecReq */
        NULL,                                                    /* CsrBtHfGetC2cAdpcmLocalSupportedReq */
        NULL,                                                    /* CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_PWR_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_BATT_REQ */
        NULL,                                                    /* CSR_BT_HF_C2C_GET_SMS_REQ */
        NULL,                                                    /* CSR_BT_HF_DEREGISTER_TIME_REQ */
        NULL,                                                    /* CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
    }
};


static const HfStateHandlerType hfCmStateHandlers[HfNumberOfStates][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    /* Activate_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        CsrBtHfActivateStateCmConnectAcceptCfmHandler,           /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                                    /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtHfHsXStateCmDataIndHandler,                         /* CSR_BT_CM_DATA_IND */
        CsrBtHfHsXStateCmDataCfmHandler,                         /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                    /* CSR_BT_CM_MODE_CHANGE_IND */
        NULL,                                                     /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Connect_s */
    {
        CsrBtHfConnectStateCmCancelAcceptConnectCfmHandler,      /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        CsrBtHfXStateCmConnectAcceptCfmHandler,                  /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtHfXStateCmDisconnectIndHandler,                     /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_DATA_IND */
        NULL,                                                    /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        NULL,                                                    /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                     /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Connected_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtHfXStateCmDisconnectIndHandler,                     /* CSR_BT_CM_DISCONNECT_IND */
        CsrBtHfConnectedStateCmScoConnectCfmHandler,             /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtHfConnectedStateCmScoDisconnectIndHandler,          /* CSR_BT_CM_SCO_DISCONNECT_IND */
        CsrBtHfConnectedStateCmScoAcceptConnectCfmHandler,       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtHfHsXStateCmDataIndHandler,                         /* CSR_BT_CM_DATA_IND */
        CsrBtHfHsXStateCmDataCfmHandler,                         /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        CsrBtHfConnectedStateCmModeChangeIndHandler,             /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                     /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* LpEnabled_s */
    {
        NULL,                                                    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtHfXStateCmDisconnectIndHandler,                     /* CSR_BT_CM_DISCONNECT_IND */
        CsrBtHfConnectedStateCmScoConnectCfmHandler,             /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtHfConnectedStateCmScoDisconnectIndHandler,          /* CSR_BT_CM_SCO_DISCONNECT_IND */
        CsrBtHfConnectedStateCmScoAcceptConnectCfmHandler,       /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtHfHsXStateCmDataIndHandler,                         /* CSR_BT_CM_DATA_IND */
        CsrBtHfHsXStateCmDataCfmHandler,                         /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        CsrBtHfLpEnabledStateCmModeChangeIndHandler,             /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                     /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* ServiceSearch_s */
    {
        CsrBtHfServiceSearchStateCmCancelAcceptCfmHandler,       /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_CONNECT_CFM */
        CsrBtHfXStateCmConnectAcceptCfmHandler,                  /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                    /* CSR_BT_CM_REGISTER_CFM */
        CsrBtHfXStateCmDisconnectIndHandler,                     /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                    /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                    /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtHfHsXStateCmDataIndHandler,                         /* CSR_BT_CM_DATA_IND */
        CsrBtHfHsXStateCmDataCfmHandler,                         /* CSR_BT_CM_DATA_CFM */
        NULL,                                                    /* CSR_BT_CM_CONTROL_IND */
        CsrBtHfServiceSearchStateCmModeChangeIndHandler,         /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                    /* CSR_BT_CM_PORTNEG_CFM */
    },
};



void CsrBtHfpHandler(HfMainInstanceData_t * instData)
{
    void * msg;
    HfInstanceData_t    *linkPtr = (HfInstanceData_t *)&(instData->linkData[instData->index]);

    msg = instData->recvMsgP;

    switch(instData->eventClass)
    {
        case CSR_BT_HF_PRIM:
            {
                CsrBtHfPrim         *primType;

                /* find the message type */
                primType = (CsrBtHfPrim *)msg;
                if ((*primType < CSR_BT_HF_PRIM_DOWNSTREAM_COUNT) &&
                    (linkPtr->state < HfNumberOfStates) &&
                    (hfStateHandlers[linkPtr->state][*primType] != NULL))
                {
                    hfStateHandlers[linkPtr->state][*primType](instData);
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                else if (*primType != CSR_BT_HF_HOUSE_CLEANING)
                {
                    /* State/Event ERROR! */
                    CsrGeneralException("HF",
                                       linkPtr->state,
                                       (CsrUint16)(*primType),
                                       "Unknown HF prim or undefined state IN HF statehandler");
                }
#endif
                break;
            }

        case CSR_BT_CM_PRIM:
        {
            CsrPrim         *primType;

            /* find the message type */
            primType = (CsrPrim *)msg;
            if (((CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST) < CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT) &&
                (linkPtr->state < HfNumberOfStates) &&
                (hfCmStateHandlers[linkPtr->state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                hfCmStateHandlers[linkPtr->state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else if (CSR_BT_CM_MAP_SCO_PCM_IND == *primType)
            {
                CsrBtHfXStateMapScoPcmIndHandler(instData);
            }
            else if ((CSR_BT_CM_SDS_UNREGISTER_CFM == *primType) && (linkPtr->state == Connect_s) && (linkPtr->linkType == CSR_BT_HF_CONNECTION_HS))
            {
                HsConnectStateCmSdsUnRegisterCfmHandler(instData);
            }
            else if ((CSR_BT_CM_SDS_REGISTER_CFM == *primType) && (linkPtr->state == Activate_s))
            {
                CsrBtHfActivateStateCmSdsRegisterCfmHandler(instData);
            }
            else if ((CSR_BT_CM_SDS_UNREGISTER_CFM == *primType) &&
                     ((linkPtr->state == Connected_s) || (linkPtr->state == ServiceSearch_s) || (linkPtr->state == LpEnabled_s)))
            {/* Unregistered due to connection establishment */
                CsrBtHfXStateCmSdsUnRegisterCfmHandler(instData);
            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("HF", linkPtr->state, *primType,
                                   "Unknown CM prim or undefined state IN HF statehandler");
#endif
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsgP);
            }
            break;
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                /* State/Event ERROR! */
                CsrGeneralException("HF",
                                   linkPtr->state,
                                   0xFF,
                                   "Unknown primitive type received IN HF statehandler");
            }
#endif
    }
}





