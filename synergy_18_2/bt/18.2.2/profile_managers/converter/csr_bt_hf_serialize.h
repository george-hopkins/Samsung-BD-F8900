#ifndef CSR_BT_HF_SERIALIZE_H__
#define CSR_BT_HF_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtHfPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM
CsrSize CsrBtHfServiceConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHfServiceConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfServiceConnectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfServiceConnectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ
CsrSize CsrBtHfIndicatorActivationReqSizeof(void *msg);
CsrUint8 *CsrBtHfIndicatorActivationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfIndicatorActivationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfIndicatorActivationReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM
CsrSize CsrBtHfDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtHfDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDeactivateCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND
CsrSize CsrBtHfServiceConnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfServiceConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfServiceConnectIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfServiceConnectIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ
CsrSize CsrBtHfGetAllStatusIndicatorsReqSizeof(void *msg);
CsrUint8 *CsrBtHfGetAllStatusIndicatorsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetAllStatusIndicatorsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetAllStatusIndicatorsReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM
CsrSize CsrBtHfSetCallWaitingNotificationCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSetCallWaitingNotificationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetCallWaitingNotificationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetCallWaitingNotificationCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ
CsrSize CsrBtHfGenerateDtmfReqSizeof(void *msg);
CsrUint8 *CsrBtHfGenerateDtmfReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGenerateDtmfReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGenerateDtmfReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ
CsrSize CsrBtHfSetExtendedAgErrorResultCodeReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetExtendedAgErrorResultCodeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetExtendedAgErrorResultCodeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetExtendedAgErrorResultCodeReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND
CsrSize CsrBtHfSetVoiceRecognitionIndSizeof(void *msg);
CsrUint8 *CsrBtHfSetVoiceRecognitionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetVoiceRecognitionIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetVoiceRecognitionIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND
CsrSize CsrBtHfAudioDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfAudioDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfAudioDisconnectIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND
CsrSize CsrBtHfStatusIndicatorUpdateIndSizeof(void *msg);
CsrUint8 *CsrBtHfStatusIndicatorUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfStatusIndicatorUpdateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfStatusIndicatorUpdateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ
CsrSize CsrBtHfMicGainStatusReqSizeof(void *msg);
CsrUint8 *CsrBtHfMicGainStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfMicGainStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfMicGainStatusReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM
CsrSize CsrBtHfSetCallNotificationIndicationCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSetCallNotificationIndicationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetCallNotificationIndicationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetCallNotificationIndicationCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_IND
CsrSize CsrBtHfMicGainIndSizeof(void *msg);
CsrUint8 *CsrBtHfMicGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfMicGainIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfMicGainIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ
CsrSize CsrBtHfAudioDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfAudioDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfAudioDisconnectReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_REQ
CsrSize CsrBtHfAtCmdReqSizeof(void *msg);
CsrUint8 *CsrBtHfAtCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAtCmdReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfAtCmdReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM
CsrSize CsrBtHfCallHandlingCfmSizeof(void *msg);
CsrUint8 *CsrBtHfCallHandlingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallHandlingCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallHandlingCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ
CsrSize CsrBtHfConfigLowPowerReqSizeof(void *msg);
CsrUint8 *CsrBtHfConfigLowPowerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfConfigLowPowerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfConfigLowPowerReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM
CsrSize CsrBtHfMicGainStatusCfmSizeof(void *msg);
CsrUint8 *CsrBtHfMicGainStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfMicGainStatusCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfMicGainStatusCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_REQ
CsrSize CsrBtHfC2cSfReqSizeof(void *msg);
CsrUint8 *CsrBtHfC2cSfReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cSfReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cSfReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ
CsrSize CsrBtHfDeregisterTimeReqSizeof(void *msg);
CsrUint8 *CsrBtHfDeregisterTimeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDeregisterTimeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDeregisterTimeReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND
CsrSize CsrBtHfAudioConnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfAudioConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfAudioConnectIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_RINGING_IND
CsrSize CsrBtHfCallRingingIndSizeof(void *msg);
CsrUint8 *CsrBtHfCallRingingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallRingingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallRingingIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_RINGING_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_IND
CsrSize CsrBtHfC2cBattIndSizeof(void *msg);
CsrUint8 *CsrBtHfC2cBattIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cBattIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cBattIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_IND */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM
CsrSize CsrBtHfConfigAudioCfmSizeof(void *msg);
CsrUint8 *CsrBtHfConfigAudioCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfConfigAudioCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfConfigAudioCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ
CsrSize CsrBtHfSetStatusIndicatorUpdateReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetStatusIndicatorUpdateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetStatusIndicatorUpdateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetStatusIndicatorUpdateReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ
CsrSize CsrBtHfSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtHfSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSecurityOutReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_IND
CsrSize CsrBtHfC2cSfIndSizeof(void *msg);
CsrUint8 *CsrBtHfC2cSfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cSfIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfC2cSfIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM
CsrSize CsrBtHfC2cGetSmsCfmSizeof(void *msg);
CsrUint8 *CsrBtHfC2cGetSmsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cGetSmsCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfC2cGetSmsCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND
CsrSize CsrBtHfSetC2cSampleRateIndSizeof(void *msg);
CsrUint8 *CsrBtHfSetC2cSampleRateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetC2cSampleRateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetC2cSampleRateIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND
CsrSize CsrBtHfStatusLowPowerIndSizeof(void *msg);
CsrUint8 *CsrBtHfStatusLowPowerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfStatusLowPowerIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfStatusLowPowerIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ
CsrSize CsrBtHfUpdateSupportedCodecReqSizeof(void *msg);
CsrUint8 *CsrBtHfUpdateSupportedCodecReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfUpdateSupportedCodecReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfUpdateSupportedCodecReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_IND
CsrSize CsrBtHfDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDisconnectIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND
CsrSize CsrBtHfSetC2cAudioCodecIndSizeof(void *msg);
CsrUint8 *CsrBtHfSetC2cAudioCodecIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetC2cAudioCodecIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetC2cAudioCodecIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND
CsrSize CsrBtHfCallNotificationIndSizeof(void *msg);
CsrUint8 *CsrBtHfCallNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallNotificationIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfCallNotificationIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SMS_IND
CsrSize CsrBtHfC2cSmsIndSizeof(void *msg);
CsrUint8 *CsrBtHfC2cSmsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cSmsIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cSmsIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_SMS_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES
CsrSize CsrBtHfAudioAcceptConnectResSizeof(void *msg);
CsrUint8 *CsrBtHfAudioAcceptConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioAcceptConnectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfAudioAcceptConnectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ
CsrSize CsrBtHfServiceConnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfServiceConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfServiceConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfServiceConnectReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_CFM
CsrSize CsrBtHfAtCmdCfmSizeof(void *msg);
CsrUint8 *CsrBtHfAtCmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAtCmdCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfAtCmdCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM
CsrSize CsrBtHfSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSecurityInCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ
CsrSize CsrBtHfSpeakerGainStatusReqSizeof(void *msg);
CsrUint8 *CsrBtHfSpeakerGainStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSpeakerGainStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSpeakerGainStatusReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_REQ
CsrSize CsrBtHfDialReqSizeof(void *msg);
CsrUint8 *CsrBtHfDialReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDialReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfDialReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_DIAL_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND
CsrSize CsrBtHfGetCurrentCallListIndSizeof(void *msg);
CsrUint8 *CsrBtHfGetCurrentCallListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetCurrentCallListIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfGetCurrentCallListIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ
CsrSize CsrBtHfGetCurrentOperatorSelectionReqSizeof(void *msg);
CsrUint8 *CsrBtHfGetCurrentOperatorSelectionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetCurrentOperatorSelectionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetCurrentOperatorSelectionReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM
CsrSize CsrBtHfAudioConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHfAudioConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfAudioConnectCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM
CsrSize CsrBtHfSetEchoAndNoiseCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSetEchoAndNoiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetEchoAndNoiseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetEchoAndNoiseCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_CFM
CsrSize CsrBtHfBtInputCfmSizeof(void *msg);
CsrUint8 *CsrBtHfBtInputCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfBtInputCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfBtInputCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ
CsrSize CsrBtHfSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtHfSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSecurityInReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ
CsrSize CsrBtHfGetCurrentCallListReqSizeof(void *msg);
CsrUint8 *CsrBtHfGetCurrentCallListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetCurrentCallListReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetCurrentCallListReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
CsrSize CsrBtHfGetC2cAdpcmLocalSupportedReqSizeof(void *msg);
CsrUint8 *CsrBtHfGetC2cAdpcmLocalSupportedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetC2cAdpcmLocalSupportedReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetC2cAdpcmLocalSupportedReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM
CsrSize CsrBtHfConfigLowPowerCfmSizeof(void *msg);
CsrUint8 *CsrBtHfConfigLowPowerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfConfigLowPowerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfConfigLowPowerCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_REQ
CsrSize CsrBtHfDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDisconnectReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM
CsrSize CsrBtHfAudioDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHfAudioDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfAudioDisconnectCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM
CsrSize CsrBtHfSetVoiceRecognitionCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSetVoiceRecognitionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetVoiceRecognitionCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetVoiceRecognitionCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_HOUSE_CLEANING
CsrSize CsrBtHfHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtHfHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfHouseCleaningSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM
CsrSize CsrBtHfSetStatusIndicatorUpdateCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSetStatusIndicatorUpdateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetStatusIndicatorUpdateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetStatusIndicatorUpdateCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
CsrSize CsrBtHfGetC2cAdpcmLocalSupportedIndSizeof(void *msg);
CsrUint8 *CsrBtHfGetC2cAdpcmLocalSupportedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetC2cAdpcmLocalSupportedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetC2cAdpcmLocalSupportedIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM
CsrSize CsrBtHfUpdateSupportedCodecCfmSizeof(void *msg);
CsrUint8 *CsrBtHfUpdateSupportedCodecCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfUpdateSupportedCodecCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfUpdateSupportedCodecCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ
CsrSize CsrBtHfConfigAudioReqSizeof(void *msg);
CsrUint8 *CsrBtHfConfigAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfConfigAudioReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfConfigAudioReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND
CsrSize CsrBtHfSpeakerGainIndSizeof(void *msg);
CsrUint8 *CsrBtHfSpeakerGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSpeakerGainIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSpeakerGainIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM
CsrSize CsrBtHfDeregisterTimeCfmSizeof(void *msg);
CsrUint8 *CsrBtHfDeregisterTimeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDeregisterTimeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDeregisterTimeCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ
CsrSize CsrBtHfCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCancelConnectReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_CFM
CsrSize CsrBtHfActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtHfActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfActivateCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM
CsrSize CsrBtHfGetSubscriberNumberInformationCfmSizeof(void *msg);
CsrUint8 *CsrBtHfGetSubscriberNumberInformationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetSubscriberNumberInformationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetSubscriberNumberInformationCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND
CsrSize CsrBtHfCallWaitingNotificationIndSizeof(void *msg);
CsrUint8 *CsrBtHfCallWaitingNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallWaitingNotificationIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfCallWaitingNotificationIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM
CsrSize CsrBtHfIndicatorActivationCfmSizeof(void *msg);
CsrUint8 *CsrBtHfIndicatorActivationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfIndicatorActivationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfIndicatorActivationCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_CFM
CsrSize CsrBtHfCallEndCfmSizeof(void *msg);
CsrUint8 *CsrBtHfCallEndCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallEndCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallEndCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_CFM */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_REQ
CsrSize CsrBtHfActivateReqSizeof(void *msg);
CsrUint8 *CsrBtHfActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfActivateReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ
CsrSize CsrBtHfGetSubscriberNumberInformationReqSizeof(void *msg);
CsrUint8 *CsrBtHfGetSubscriberNumberInformationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetSubscriberNumberInformationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetSubscriberNumberInformationReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM
CsrSize CsrBtHfGetCurrentOperatorSelectionCfmSizeof(void *msg);
CsrUint8 *CsrBtHfGetCurrentOperatorSelectionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetCurrentOperatorSelectionCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfGetCurrentOperatorSelectionCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ
CsrSize CsrBtHfSetC2cSampleRateReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetC2cSampleRateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetC2cSampleRateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetC2cSampleRateReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM
CsrSize CsrBtHfSpeakerGainStatusCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSpeakerGainStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSpeakerGainStatusCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSpeakerGainStatusCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ
CsrSize CsrBtHfSetCallWaitingNotificationReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetCallWaitingNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetCallWaitingNotificationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetCallWaitingNotificationReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_IND
CsrSize CsrBtHfAtCmdIndSizeof(void *msg);
CsrUint8 *CsrBtHfAtCmdIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAtCmdIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfAtCmdIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM
CsrSize CsrBtHfCallAnswerCfmSizeof(void *msg);
CsrUint8 *CsrBtHfCallAnswerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallAnswerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallAnswerCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_CFM
CsrSize CsrBtHfC2cPwrCfmSizeof(void *msg);
CsrUint8 *CsrBtHfC2cPwrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cPwrCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cPwrCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ
CsrSize CsrBtHfSetCallNotificationIndicationReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetCallNotificationIndicationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetCallNotificationIndicationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetCallNotificationIndicationReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_REQ
CsrSize CsrBtHfC2cPwrReqSizeof(void *msg);
CsrUint8 *CsrBtHfC2cPwrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cPwrReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cPwrReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM
CsrSize CsrBtHfGenerateDtmfCfmSizeof(void *msg);
CsrUint8 *CsrBtHfGenerateDtmfCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGenerateDtmfCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGenerateDtmfCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM
CsrSize CsrBtHfGetCurrentCallListCfmSizeof(void *msg);
CsrUint8 *CsrBtHfGetCurrentCallListCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetCurrentCallListCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfGetCurrentCallListCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ
CsrSize CsrBtHfSetEchoAndNoiseReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetEchoAndNoiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetEchoAndNoiseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetEchoAndNoiseReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_REQ
CsrSize CsrBtHfCallEndReqSizeof(void *msg);
CsrUint8 *CsrBtHfCallEndReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallEndReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallEndReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_REQ */

#ifndef EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND
CsrSize CsrBtHfInbandRingSettingChangedIndSizeof(void *msg);
CsrUint8 *CsrBtHfInbandRingSettingChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfInbandRingSettingChangedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfInbandRingSettingChangedIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ
CsrSize CsrBtHfC2cGetSmsReqSizeof(void *msg);
CsrUint8 *CsrBtHfC2cGetSmsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cGetSmsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cGetSmsReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ
CsrSize CsrBtHfSetVoiceRecognitionReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetVoiceRecognitionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetVoiceRecognitionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetVoiceRecognitionReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM
CsrSize CsrBtHfGetAllStatusIndicatorsCfmSizeof(void *msg);
CsrUint8 *CsrBtHfGetAllStatusIndicatorsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetAllStatusIndicatorsCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfGetAllStatusIndicatorsCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND
CsrSize CsrBtHfCallHandlingIndSizeof(void *msg);
CsrUint8 *CsrBtHfCallHandlingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallHandlingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallHandlingIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_CFM
CsrSize CsrBtHfDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHfDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDisconnectCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM
CsrSize CsrBtHfSetExtendedAgErrorResultCodeCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSetExtendedAgErrorResultCodeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetExtendedAgErrorResultCodeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetExtendedAgErrorResultCodeCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM
CsrSize CsrBtHfSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtHfSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSecurityOutCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND
CsrSize CsrBtHfGetSubscriberNumberInformationIndSizeof(void *msg);
CsrUint8 *CsrBtHfGetSubscriberNumberInformationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfGetSubscriberNumberInformationIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfGetSubscriberNumberInformationIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_CFM
CsrSize CsrBtHfDialCfmSizeof(void *msg);
CsrUint8 *CsrBtHfDialCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDialCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDialCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DIAL_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ
CsrSize CsrBtHfSetC2cAudioCodecReqSizeof(void *msg);
CsrUint8 *CsrBtHfSetC2cAudioCodecReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfSetC2cAudioCodecReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfSetC2cAudioCodecReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_REQ
CsrSize CsrBtHfC2cBattReqSizeof(void *msg);
CsrUint8 *CsrBtHfC2cBattReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cBattReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cBattReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_TXT_IND
CsrSize CsrBtHfC2cTxtIndSizeof(void *msg);
CsrUint8 *CsrBtHfC2cTxtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cTxtIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfC2cTxtIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_C2C_TXT_IND */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_REQ
CsrSize CsrBtHfBtInputReqSizeof(void *msg);
CsrUint8 *CsrBtHfBtInputReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfBtInputReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfBtInputReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND
CsrSize CsrBtHfAudioAcceptConnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfAudioAcceptConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioAcceptConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfAudioAcceptConnectIndSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ
CsrSize CsrBtHfDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtHfDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfDeactivateReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ
CsrSize CsrBtHfCallHandlingReqSizeof(void *msg);
CsrUint8 *CsrBtHfCallHandlingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallHandlingReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallHandlingReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ
CsrSize CsrBtHfCallAnswerReqSizeof(void *msg);
CsrUint8 *CsrBtHfCallAnswerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfCallAnswerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfCallAnswerReqSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_CFM
CsrSize CsrBtHfC2cBattCfmSizeof(void *msg);
CsrUint8 *CsrBtHfC2cBattCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfC2cBattCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfC2cBattCfmSerFree CsrBtHfPfree
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ
CsrSize CsrBtHfAudioConnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfAudioConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfAudioConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfAudioConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ */

#ifdef __cplusplus
}
#endif

#endif
