/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_hf_converter_init.h"
#ifndef EXCLUDE_CSR_BT_HF_MODULE
#include "csr_bt_hf_serialize.h"
#include "csr_bt_hf_prim.h"

static CsrMsgConvMsgEntry csr_bt_hf_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_CFM
    { CSR_BT_HF_ACTIVATE_CFM, CsrBtHfActivateCfmSizeof, CsrBtHfActivateCfmSer, CsrBtHfActivateCfmDes, CsrBtHfActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_REQ
    { CSR_BT_HF_ACTIVATE_REQ, CsrBtHfActivateReqSizeof, CsrBtHfActivateReqSer, CsrBtHfActivateReqDes, CsrBtHfActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_CFM
    { CSR_BT_HF_AT_CMD_CFM, CsrBtHfAtCmdCfmSizeof, CsrBtHfAtCmdCfmSer, CsrBtHfAtCmdCfmDes, CsrBtHfAtCmdCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_CFM */
#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_IND
    { CSR_BT_HF_AT_CMD_IND, CsrBtHfAtCmdIndSizeof, CsrBtHfAtCmdIndSer, CsrBtHfAtCmdIndDes, CsrBtHfAtCmdIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_IND */
#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_REQ
    { CSR_BT_HF_AT_CMD_REQ, CsrBtHfAtCmdReqSizeof, CsrBtHfAtCmdReqSer, CsrBtHfAtCmdReqDes, CsrBtHfAtCmdReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_REQ */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND
    { CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND, CsrBtHfAudioAcceptConnectIndSizeof, CsrBtHfAudioAcceptConnectIndSer, CsrBtHfAudioAcceptConnectIndDes, CsrBtHfAudioAcceptConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES
    { CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES, CsrBtHfAudioAcceptConnectResSizeof, CsrBtHfAudioAcceptConnectResSer, CsrBtHfAudioAcceptConnectResDes, CsrBtHfAudioAcceptConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM
    { CSR_BT_HF_AUDIO_CONNECT_CFM, CsrBtHfAudioConnectCfmSizeof, CsrBtHfAudioConnectCfmSer, CsrBtHfAudioConnectCfmDes, CsrBtHfAudioConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND
    { CSR_BT_HF_AUDIO_CONNECT_IND, CsrBtHfAudioConnectIndSizeof, CsrBtHfAudioConnectIndSer, CsrBtHfAudioConnectIndDes, CsrBtHfAudioConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ
    { CSR_BT_HF_AUDIO_CONNECT_REQ, CsrBtHfAudioConnectReqSizeof, CsrBtHfAudioConnectReqSer, CsrBtHfAudioConnectReqDes, CsrBtHfAudioConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM
    { CSR_BT_HF_AUDIO_DISCONNECT_CFM, CsrBtHfAudioDisconnectCfmSizeof, CsrBtHfAudioDisconnectCfmSer, CsrBtHfAudioDisconnectCfmDes, CsrBtHfAudioDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND
    { CSR_BT_HF_AUDIO_DISCONNECT_IND, CsrBtHfAudioDisconnectIndSizeof, CsrBtHfAudioDisconnectIndSer, CsrBtHfAudioDisconnectIndDes, CsrBtHfAudioDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ
    { CSR_BT_HF_AUDIO_DISCONNECT_REQ, CsrBtHfAudioDisconnectReqSizeof, CsrBtHfAudioDisconnectReqSer, CsrBtHfAudioDisconnectReqDes, CsrBtHfAudioDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_CFM
    { CSR_BT_HF_BT_INPUT_CFM, CsrBtHfBtInputCfmSizeof, CsrBtHfBtInputCfmSer, CsrBtHfBtInputCfmDes, CsrBtHfBtInputCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_REQ
    { CSR_BT_HF_BT_INPUT_REQ, CsrBtHfBtInputReqSizeof, CsrBtHfBtInputReqSer, CsrBtHfBtInputReqDes, CsrBtHfBtInputReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_CFM
    { CSR_BT_HF_C2C_BATT_CFM, CsrBtHfC2cBattCfmSizeof, CsrBtHfC2cBattCfmSer, CsrBtHfC2cBattCfmDes, CsrBtHfC2cBattCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_IND
    { CSR_BT_HF_C2C_BATT_IND, CsrBtHfC2cBattIndSizeof, CsrBtHfC2cBattIndSer, CsrBtHfC2cBattIndDes, CsrBtHfC2cBattIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_IND */
#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_REQ
    { CSR_BT_HF_C2C_BATT_REQ, CsrBtHfC2cBattReqSizeof, CsrBtHfC2cBattReqSer, CsrBtHfC2cBattReqDes, CsrBtHfC2cBattReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM
    { CSR_BT_HF_C2C_GET_SMS_CFM, CsrBtHfC2cGetSmsCfmSizeof, CsrBtHfC2cGetSmsCfmSer, CsrBtHfC2cGetSmsCfmDes, CsrBtHfC2cGetSmsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM */
#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ
    { CSR_BT_HF_C2C_GET_SMS_REQ, CsrBtHfC2cGetSmsReqSizeof, CsrBtHfC2cGetSmsReqSer, CsrBtHfC2cGetSmsReqDes, CsrBtHfC2cGetSmsReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ */
#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_CFM
    { CSR_BT_HF_C2C_PWR_CFM, CsrBtHfC2cPwrCfmSizeof, CsrBtHfC2cPwrCfmSer, CsrBtHfC2cPwrCfmDes, CsrBtHfC2cPwrCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_CFM */
#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_REQ
    { CSR_BT_HF_C2C_PWR_REQ, CsrBtHfC2cPwrReqSizeof, CsrBtHfC2cPwrReqSer, CsrBtHfC2cPwrReqDes, CsrBtHfC2cPwrReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_REQ */
#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_IND
    { CSR_BT_HF_C2C_SF_IND, CsrBtHfC2cSfIndSizeof, CsrBtHfC2cSfIndSer, CsrBtHfC2cSfIndDes, CsrBtHfC2cSfIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_IND */
#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_REQ
    { CSR_BT_HF_C2C_SF_REQ, CsrBtHfC2cSfReqSizeof, CsrBtHfC2cSfReqSer, CsrBtHfC2cSfReqDes, CsrBtHfC2cSfReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_REQ */
#ifndef EXCLUDE_CSR_BT_HF_C2C_SMS_IND
    { CSR_BT_HF_C2C_SMS_IND, CsrBtHfC2cSmsIndSizeof, CsrBtHfC2cSmsIndSer, CsrBtHfC2cSmsIndDes, CsrBtHfC2cSmsIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_SMS_IND */
#ifndef EXCLUDE_CSR_BT_HF_C2C_TXT_IND
    { CSR_BT_HF_C2C_TXT_IND, CsrBtHfC2cTxtIndSizeof, CsrBtHfC2cTxtIndSer, CsrBtHfC2cTxtIndDes, CsrBtHfC2cTxtIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_C2C_TXT_IND */
#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM
    { CSR_BT_HF_CALL_ANSWER_CFM, CsrBtHfCallAnswerCfmSizeof, CsrBtHfCallAnswerCfmSer, CsrBtHfCallAnswerCfmDes, CsrBtHfCallAnswerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM */
#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ
    { CSR_BT_HF_CALL_ANSWER_REQ, CsrBtHfCallAnswerReqSizeof, CsrBtHfCallAnswerReqSer, CsrBtHfCallAnswerReqDes, CsrBtHfCallAnswerReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ */
#ifndef EXCLUDE_CSR_BT_HF_CALL_END_CFM
    { CSR_BT_HF_CALL_END_CFM, CsrBtHfCallEndCfmSizeof, CsrBtHfCallEndCfmSer, CsrBtHfCallEndCfmDes, CsrBtHfCallEndCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_CFM */
#ifndef EXCLUDE_CSR_BT_HF_CALL_END_REQ
    { CSR_BT_HF_CALL_END_REQ, CsrBtHfCallEndReqSizeof, CsrBtHfCallEndReqSer, CsrBtHfCallEndReqDes, CsrBtHfCallEndReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_REQ */
#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM
    { CSR_BT_HF_CALL_HANDLING_CFM, CsrBtHfCallHandlingCfmSizeof, CsrBtHfCallHandlingCfmSer, CsrBtHfCallHandlingCfmDes, CsrBtHfCallHandlingCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM */
#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND
    { CSR_BT_HF_CALL_HANDLING_IND, CsrBtHfCallHandlingIndSizeof, CsrBtHfCallHandlingIndSer, CsrBtHfCallHandlingIndDes, CsrBtHfCallHandlingIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND */
#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ
    { CSR_BT_HF_CALL_HANDLING_REQ, CsrBtHfCallHandlingReqSizeof, CsrBtHfCallHandlingReqSer, CsrBtHfCallHandlingReqDes, CsrBtHfCallHandlingReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ */
#ifndef EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND
    { CSR_BT_HF_CALL_NOTIFICATION_IND, CsrBtHfCallNotificationIndSizeof, CsrBtHfCallNotificationIndSer, CsrBtHfCallNotificationIndDes, CsrBtHfCallNotificationIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_HF_CALL_RINGING_IND
    { CSR_BT_HF_CALL_RINGING_IND, CsrBtHfCallRingingIndSizeof, CsrBtHfCallRingingIndSer, CsrBtHfCallRingingIndDes, CsrBtHfCallRingingIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_RINGING_IND */
#ifndef EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND
    { CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND, CsrBtHfCallWaitingNotificationIndSizeof, CsrBtHfCallWaitingNotificationIndSer, CsrBtHfCallWaitingNotificationIndDes, CsrBtHfCallWaitingNotificationIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ
    { CSR_BT_HF_CANCEL_CONNECT_REQ, CsrBtHfCancelConnectReqSizeof, CsrBtHfCancelConnectReqSer, CsrBtHfCancelConnectReqDes, CsrBtHfCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM
    { CSR_BT_HF_CONFIG_AUDIO_CFM, CsrBtHfConfigAudioCfmSizeof, CsrBtHfConfigAudioCfmSer, CsrBtHfConfigAudioCfmDes, CsrBtHfConfigAudioCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM */
#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ
    { CSR_BT_HF_CONFIG_AUDIO_REQ, CsrBtHfConfigAudioReqSizeof, CsrBtHfConfigAudioReqSer, CsrBtHfConfigAudioReqDes, CsrBtHfConfigAudioReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ */
#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM
    { CSR_BT_HF_CONFIG_LOW_POWER_CFM, CsrBtHfConfigLowPowerCfmSizeof, CsrBtHfConfigLowPowerCfmSer, CsrBtHfConfigLowPowerCfmDes, CsrBtHfConfigLowPowerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM */
#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ
    { CSR_BT_HF_CONFIG_LOW_POWER_REQ, CsrBtHfConfigLowPowerReqSizeof, CsrBtHfConfigLowPowerReqSer, CsrBtHfConfigLowPowerReqDes, CsrBtHfConfigLowPowerReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ */
#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM
    { CSR_BT_HF_DEACTIVATE_CFM, CsrBtHfDeactivateCfmSizeof, CsrBtHfDeactivateCfmSer, CsrBtHfDeactivateCfmDes, CsrBtHfDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ
    { CSR_BT_HF_DEACTIVATE_REQ, CsrBtHfDeactivateReqSizeof, CsrBtHfDeactivateReqSer, CsrBtHfDeactivateReqDes, CsrBtHfDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM
    { CSR_BT_HF_DEREGISTER_TIME_CFM, CsrBtHfDeregisterTimeCfmSizeof, CsrBtHfDeregisterTimeCfmSer, CsrBtHfDeregisterTimeCfmDes, CsrBtHfDeregisterTimeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM */
#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ
    { CSR_BT_HF_DEREGISTER_TIME_REQ, CsrBtHfDeregisterTimeReqSizeof, CsrBtHfDeregisterTimeReqSer, CsrBtHfDeregisterTimeReqDes, CsrBtHfDeregisterTimeReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ */
#ifndef EXCLUDE_CSR_BT_HF_DIAL_CFM
    { CSR_BT_HF_DIAL_CFM, CsrBtHfDialCfmSizeof, CsrBtHfDialCfmSer, CsrBtHfDialCfmDes, CsrBtHfDialCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DIAL_CFM */
#ifndef EXCLUDE_CSR_BT_HF_DIAL_REQ
    { CSR_BT_HF_DIAL_REQ, CsrBtHfDialReqSizeof, CsrBtHfDialReqSer, CsrBtHfDialReqDes, CsrBtHfDialReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DIAL_REQ */
#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_CFM
    { CSR_BT_HF_DISCONNECT_CFM, CsrBtHfDisconnectCfmSizeof, CsrBtHfDisconnectCfmSer, CsrBtHfDisconnectCfmDes, CsrBtHfDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_IND
    { CSR_BT_HF_DISCONNECT_IND, CsrBtHfDisconnectIndSizeof, CsrBtHfDisconnectIndSer, CsrBtHfDisconnectIndDes, CsrBtHfDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_REQ
    { CSR_BT_HF_DISCONNECT_REQ, CsrBtHfDisconnectReqSizeof, CsrBtHfDisconnectReqSer, CsrBtHfDisconnectReqDes, CsrBtHfDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM
    { CSR_BT_HF_GENERATE_DTMF_CFM, CsrBtHfGenerateDtmfCfmSizeof, CsrBtHfGenerateDtmfCfmSer, CsrBtHfGenerateDtmfCfmDes, CsrBtHfGenerateDtmfCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM */
#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ
    { CSR_BT_HF_GENERATE_DTMF_REQ, CsrBtHfGenerateDtmfReqSizeof, CsrBtHfGenerateDtmfReqSer, CsrBtHfGenerateDtmfReqDes, CsrBtHfGenerateDtmfReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ */
#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM
    { CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM, CsrBtHfGetAllStatusIndicatorsCfmSizeof, CsrBtHfGetAllStatusIndicatorsCfmSer, CsrBtHfGetAllStatusIndicatorsCfmDes, CsrBtHfGetAllStatusIndicatorsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM */
#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ
    { CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ, CsrBtHfGetAllStatusIndicatorsReqSizeof, CsrBtHfGetAllStatusIndicatorsReqSer, CsrBtHfGetAllStatusIndicatorsReqDes, CsrBtHfGetAllStatusIndicatorsReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */
#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
    { CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND, CsrBtHfGetC2cAdpcmLocalSupportedIndSizeof, CsrBtHfGetC2cAdpcmLocalSupportedIndSer, CsrBtHfGetC2cAdpcmLocalSupportedIndDes, CsrBtHfGetC2cAdpcmLocalSupportedIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */
#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
    { CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ, CsrBtHfGetC2cAdpcmLocalSupportedReqSizeof, CsrBtHfGetC2cAdpcmLocalSupportedReqSer, CsrBtHfGetC2cAdpcmLocalSupportedReqDes, CsrBtHfGetC2cAdpcmLocalSupportedReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */
#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM
    { CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM, CsrBtHfGetCurrentCallListCfmSizeof, CsrBtHfGetCurrentCallListCfmSer, CsrBtHfGetCurrentCallListCfmDes, CsrBtHfGetCurrentCallListCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM */
#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND
    { CSR_BT_HF_GET_CURRENT_CALL_LIST_IND, CsrBtHfGetCurrentCallListIndSizeof, CsrBtHfGetCurrentCallListIndSer, CsrBtHfGetCurrentCallListIndDes, CsrBtHfGetCurrentCallListIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND */
#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ
    { CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ, CsrBtHfGetCurrentCallListReqSizeof, CsrBtHfGetCurrentCallListReqSer, CsrBtHfGetCurrentCallListReqDes, CsrBtHfGetCurrentCallListReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */
#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM
    { CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM, CsrBtHfGetCurrentOperatorSelectionCfmSizeof, CsrBtHfGetCurrentOperatorSelectionCfmSer, CsrBtHfGetCurrentOperatorSelectionCfmDes, CsrBtHfGetCurrentOperatorSelectionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM */
#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ
    { CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ, CsrBtHfGetCurrentOperatorSelectionReqSizeof, CsrBtHfGetCurrentOperatorSelectionReqSer, CsrBtHfGetCurrentOperatorSelectionReqDes, CsrBtHfGetCurrentOperatorSelectionReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */
#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM
    { CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM, CsrBtHfGetSubscriberNumberInformationCfmSizeof, CsrBtHfGetSubscriberNumberInformationCfmSer, CsrBtHfGetSubscriberNumberInformationCfmDes, CsrBtHfGetSubscriberNumberInformationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM */
#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND
    { CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND, CsrBtHfGetSubscriberNumberInformationIndSizeof, CsrBtHfGetSubscriberNumberInformationIndSer, CsrBtHfGetSubscriberNumberInformationIndDes, CsrBtHfGetSubscriberNumberInformationIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND */
#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ
    { CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ, CsrBtHfGetSubscriberNumberInformationReqSizeof, CsrBtHfGetSubscriberNumberInformationReqSer, CsrBtHfGetSubscriberNumberInformationReqDes, CsrBtHfGetSubscriberNumberInformationReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */
#ifndef EXCLUDE_CSR_BT_HF_HOUSE_CLEANING
    { CSR_BT_HF_HOUSE_CLEANING, CsrBtHfHouseCleaningSizeof, CsrBtHfHouseCleaningSer, CsrBtHfHouseCleaningDes, CsrBtHfHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_HF_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND
    { CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND, CsrBtHfInbandRingSettingChangedIndSizeof, CsrBtHfInbandRingSettingChangedIndSer, CsrBtHfInbandRingSettingChangedIndDes, CsrBtHfInbandRingSettingChangedIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND */
#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM
    { CSR_BT_HF_INDICATOR_ACTIVATION_CFM, CsrBtHfIndicatorActivationCfmSizeof, CsrBtHfIndicatorActivationCfmSer, CsrBtHfIndicatorActivationCfmDes, CsrBtHfIndicatorActivationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM */
#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ
    { CSR_BT_HF_INDICATOR_ACTIVATION_REQ, CsrBtHfIndicatorActivationReqSizeof, CsrBtHfIndicatorActivationReqSer, CsrBtHfIndicatorActivationReqDes, CsrBtHfIndicatorActivationReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ */
#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_IND
    { CSR_BT_HF_MIC_GAIN_IND, CsrBtHfMicGainIndSizeof, CsrBtHfMicGainIndSer, CsrBtHfMicGainIndDes, CsrBtHfMicGainIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_IND */
#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM
    { CSR_BT_HF_MIC_GAIN_STATUS_CFM, CsrBtHfMicGainStatusCfmSizeof, CsrBtHfMicGainStatusCfmSer, CsrBtHfMicGainStatusCfmDes, CsrBtHfMicGainStatusCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM */
#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ
    { CSR_BT_HF_MIC_GAIN_STATUS_REQ, CsrBtHfMicGainStatusReqSizeof, CsrBtHfMicGainStatusReqSer, CsrBtHfMicGainStatusReqDes, CsrBtHfMicGainStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM
    { CSR_BT_HF_SECURITY_IN_CFM, CsrBtHfSecurityInCfmSizeof, CsrBtHfSecurityInCfmSer, CsrBtHfSecurityInCfmDes, CsrBtHfSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ
    { CSR_BT_HF_SECURITY_IN_REQ, CsrBtHfSecurityInReqSizeof, CsrBtHfSecurityInReqSer, CsrBtHfSecurityInReqDes, CsrBtHfSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM
    { CSR_BT_HF_SECURITY_OUT_CFM, CsrBtHfSecurityOutCfmSizeof, CsrBtHfSecurityOutCfmSer, CsrBtHfSecurityOutCfmDes, CsrBtHfSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ
    { CSR_BT_HF_SECURITY_OUT_REQ, CsrBtHfSecurityOutReqSizeof, CsrBtHfSecurityOutReqSer, CsrBtHfSecurityOutReqDes, CsrBtHfSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM
    { CSR_BT_HF_SERVICE_CONNECT_CFM, CsrBtHfServiceConnectCfmSizeof, CsrBtHfServiceConnectCfmSer, CsrBtHfServiceConnectCfmDes, CsrBtHfServiceConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND
    { CSR_BT_HF_SERVICE_CONNECT_IND, CsrBtHfServiceConnectIndSizeof, CsrBtHfServiceConnectIndSer, CsrBtHfServiceConnectIndDes, CsrBtHfServiceConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ
    { CSR_BT_HF_SERVICE_CONNECT_REQ, CsrBtHfServiceConnectReqSizeof, CsrBtHfServiceConnectReqSer, CsrBtHfServiceConnectReqDes, CsrBtHfServiceConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND
    { CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND, CsrBtHfSetC2cAudioCodecIndSizeof, CsrBtHfSetC2cAudioCodecIndSer, CsrBtHfSetC2cAudioCodecIndDes, CsrBtHfSetC2cAudioCodecIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND */
#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ
    { CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ, CsrBtHfSetC2cAudioCodecReqSizeof, CsrBtHfSetC2cAudioCodecReqSer, CsrBtHfSetC2cAudioCodecReqDes, CsrBtHfSetC2cAudioCodecReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND
    { CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND, CsrBtHfSetC2cSampleRateIndSizeof, CsrBtHfSetC2cSampleRateIndSer, CsrBtHfSetC2cSampleRateIndDes, CsrBtHfSetC2cSampleRateIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND */
#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ
    { CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ, CsrBtHfSetC2cSampleRateReqSizeof, CsrBtHfSetC2cSampleRateReqSer, CsrBtHfSetC2cSampleRateReqDes, CsrBtHfSetC2cSampleRateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM
    { CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM, CsrBtHfSetCallNotificationIndicationCfmSizeof, CsrBtHfSetCallNotificationIndicationCfmSer, CsrBtHfSetCallNotificationIndicationCfmDes, CsrBtHfSetCallNotificationIndicationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ
    { CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ, CsrBtHfSetCallNotificationIndicationReqSizeof, CsrBtHfSetCallNotificationIndicationReqSer, CsrBtHfSetCallNotificationIndicationReqDes, CsrBtHfSetCallNotificationIndicationReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM
    { CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM, CsrBtHfSetCallWaitingNotificationCfmSizeof, CsrBtHfSetCallWaitingNotificationCfmSer, CsrBtHfSetCallWaitingNotificationCfmDes, CsrBtHfSetCallWaitingNotificationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ
    { CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ, CsrBtHfSetCallWaitingNotificationReqSizeof, CsrBtHfSetCallWaitingNotificationReqSer, CsrBtHfSetCallWaitingNotificationReqDes, CsrBtHfSetCallWaitingNotificationReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM
    { CSR_BT_HF_SET_ECHO_AND_NOISE_CFM, CsrBtHfSetEchoAndNoiseCfmSizeof, CsrBtHfSetEchoAndNoiseCfmSer, CsrBtHfSetEchoAndNoiseCfmDes, CsrBtHfSetEchoAndNoiseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ
    { CSR_BT_HF_SET_ECHO_AND_NOISE_REQ, CsrBtHfSetEchoAndNoiseReqSizeof, CsrBtHfSetEchoAndNoiseReqSer, CsrBtHfSetEchoAndNoiseReqDes, CsrBtHfSetEchoAndNoiseReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM
    { CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM, CsrBtHfSetExtendedAgErrorResultCodeCfmSizeof, CsrBtHfSetExtendedAgErrorResultCodeCfmSer, CsrBtHfSetExtendedAgErrorResultCodeCfmDes, CsrBtHfSetExtendedAgErrorResultCodeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ
    { CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ, CsrBtHfSetExtendedAgErrorResultCodeReqSizeof, CsrBtHfSetExtendedAgErrorResultCodeReqSer, CsrBtHfSetExtendedAgErrorResultCodeReqDes, CsrBtHfSetExtendedAgErrorResultCodeReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM
    { CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM, CsrBtHfSetStatusIndicatorUpdateCfmSizeof, CsrBtHfSetStatusIndicatorUpdateCfmSer, CsrBtHfSetStatusIndicatorUpdateCfmDes, CsrBtHfSetStatusIndicatorUpdateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ
    { CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ, CsrBtHfSetStatusIndicatorUpdateReqSizeof, CsrBtHfSetStatusIndicatorUpdateReqSer, CsrBtHfSetStatusIndicatorUpdateReqDes, CsrBtHfSetStatusIndicatorUpdateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM
    { CSR_BT_HF_SET_VOICE_RECOGNITION_CFM, CsrBtHfSetVoiceRecognitionCfmSizeof, CsrBtHfSetVoiceRecognitionCfmSer, CsrBtHfSetVoiceRecognitionCfmDes, CsrBtHfSetVoiceRecognitionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND
    { CSR_BT_HF_SET_VOICE_RECOGNITION_IND, CsrBtHfSetVoiceRecognitionIndSizeof, CsrBtHfSetVoiceRecognitionIndSer, CsrBtHfSetVoiceRecognitionIndDes, CsrBtHfSetVoiceRecognitionIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND */
#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ
    { CSR_BT_HF_SET_VOICE_RECOGNITION_REQ, CsrBtHfSetVoiceRecognitionReqSizeof, CsrBtHfSetVoiceRecognitionReqSer, CsrBtHfSetVoiceRecognitionReqDes, CsrBtHfSetVoiceRecognitionReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */
#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND
    { CSR_BT_HF_SPEAKER_GAIN_IND, CsrBtHfSpeakerGainIndSizeof, CsrBtHfSpeakerGainIndSer, CsrBtHfSpeakerGainIndDes, CsrBtHfSpeakerGainIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND */
#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM
    { CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM, CsrBtHfSpeakerGainStatusCfmSizeof, CsrBtHfSpeakerGainStatusCfmSer, CsrBtHfSpeakerGainStatusCfmDes, CsrBtHfSpeakerGainStatusCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ
    { CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ, CsrBtHfSpeakerGainStatusReqSizeof, CsrBtHfSpeakerGainStatusReqSer, CsrBtHfSpeakerGainStatusReqDes, CsrBtHfSpeakerGainStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND
    { CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND, CsrBtHfStatusIndicatorUpdateIndSizeof, CsrBtHfStatusIndicatorUpdateIndSer, CsrBtHfStatusIndicatorUpdateIndDes, CsrBtHfStatusIndicatorUpdateIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND */
#ifndef EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND
    { CSR_BT_HF_STATUS_LOW_POWER_IND, CsrBtHfStatusLowPowerIndSizeof, CsrBtHfStatusLowPowerIndSer, CsrBtHfStatusLowPowerIndDes, CsrBtHfStatusLowPowerIndSerFree },
#endif /* EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND */
#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM
    { CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM, CsrBtHfUpdateSupportedCodecCfmSizeof, CsrBtHfUpdateSupportedCodecCfmSer, CsrBtHfUpdateSupportedCodecCfmDes, CsrBtHfUpdateSupportedCodecCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM */
#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ
    { CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ, CsrBtHfUpdateSupportedCodecReqSizeof, CsrBtHfUpdateSupportedCodecReqSer, CsrBtHfUpdateSupportedCodecReqDes, CsrBtHfUpdateSupportedCodecReqSerFree },
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtHfConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_HF_PRIM, csr_bt_hf_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_hf_conv_info = {
    CSR_BT_HF_PRIM,
    "CSR_BT_HF_PRIM",
    csr_bt_hf_conv_lut
};

CsrLogPrimitiveInformation* CsrBtHfTechInfoGet(void)
{
    return &csr_bt_hf_conv_info;
}
#endif
#endif

