#ifndef CSR_BT_HFG_SERIALIZE_H__
#define CSR_BT_HFG_SERIALIZE_H__
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

void CsrBtHfgPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ
CsrSize CsrBtHfgCallWaitingReqSizeof(void *msg);
CsrUint8 *CsrBtHfgCallWaitingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgCallWaitingReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgCallWaitingReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_REQ
CsrSize CsrBtHfgC2cSfReqSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cSfReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cSfReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgC2cSfReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_LP_IND
CsrSize CsrBtHfgStatusLpIndSizeof(void *msg);
CsrUint8 *CsrBtHfgStatusLpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgStatusLpIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgStatusLpIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_LP_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND
CsrSize CsrBtHfgC2cBatteryIndSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cBatteryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cBatteryIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgC2cBatteryIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_RES
CsrSize CsrBtHfgOperatorResSizeof(void *msg);
CsrUint8 *CsrBtHfgOperatorResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgOperatorResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgOperatorResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM
CsrSize CsrBtHfgConfigAtcmdHandlingCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgConfigAtcmdHandlingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgConfigAtcmdHandlingCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgConfigAtcmdHandlingCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ
CsrSize CsrBtHfgDeregisterTimeReqSizeof(void *msg);
CsrUint8 *CsrBtHfgDeregisterTimeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDeregisterTimeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgDeregisterTimeReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ
CsrSize CsrBtHfgC2cSmsArriveReqSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cSmsArriveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cSmsArriveReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgC2cSmsArriveReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_IND
CsrSize CsrBtHfgDialIndSizeof(void *msg);
CsrUint8 *CsrBtHfgDialIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDialIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgDialIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_IND */

#ifndef EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ
CsrSize CsrBtHfgInbandRingingReqSizeof(void *msg);
CsrUint8 *CsrBtHfgInbandRingingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgInbandRingingReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgInbandRingingReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES
CsrSize CsrBtHfgCallHandlingResSizeof(void *msg);
CsrUint8 *CsrBtHfgCallHandlingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgCallHandlingResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgCallHandlingResSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ
CsrSize CsrBtHfgSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtHfgSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgSecurityInReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND
CsrSize CsrBtHfgAudioDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAudioDisconnectIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND
CsrSize CsrBtHfgStatusAudioIndSizeof(void *msg);
CsrUint8 *CsrBtHfgStatusAudioIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgStatusAudioIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgStatusAudioIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_RING_REQ
CsrSize CsrBtHfgRingReqSizeof(void *msg);
CsrUint8 *CsrBtHfgRingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgRingReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgRingReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_RING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND
CsrSize CsrBtHfgSpeakerGainIndSizeof(void *msg);
CsrUint8 *CsrBtHfgSpeakerGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSpeakerGainIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgSpeakerGainIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING
CsrSize CsrBtHfgHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtHfgHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgHouseCleaningSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ
CsrSize CsrBtHfgStatusIndicatorSetReqSizeof(void *msg);
CsrUint8 *CsrBtHfgStatusIndicatorSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgStatusIndicatorSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgStatusIndicatorSetReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_RES
CsrSize CsrBtHfgDialResSizeof(void *msg);
CsrUint8 *CsrBtHfgDialResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDialResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgDialResSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM
CsrSize CsrBtHfgSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgSecurityOutCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND
CsrSize CsrBtHfgAudioAcceptConnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioAcceptConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioAcceptConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAudioAcceptConnectIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_REJECT_IND
CsrSize CsrBtHfgRejectIndSizeof(void *msg);
CsrUint8 *CsrBtHfgRejectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgRejectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgRejectIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_REJECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_IND
CsrSize CsrBtHfgC2cSfIndSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cSfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cSfIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgC2cSfIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ
CsrSize CsrBtHfgCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfgCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgCancelConnectReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ
CsrSize CsrBtHfgC2cSmsTxtReqSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cSmsTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cSmsTxtReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgC2cSmsTxtReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM
CsrSize CsrBtHfgAudioDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAudioDisconnectCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES
CsrSize CsrBtHfgVoiceRecogResSizeof(void *msg);
CsrUint8 *CsrBtHfgVoiceRecogResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgVoiceRecogResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgVoiceRecogResSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ
CsrSize CsrBtHfgDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfgDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgDisconnectReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ
CsrSize CsrBtHfgConfigAtcmdHandlingReqSizeof(void *msg);
CsrUint8 *CsrBtHfgConfigAtcmdHandlingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgConfigAtcmdHandlingReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgConfigAtcmdHandlingReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND
CsrSize CsrBtHfgManualIndicatorIndSizeof(void *msg);
CsrUint8 *CsrBtHfgManualIndicatorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgManualIndicatorIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgManualIndicatorIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_RES
CsrSize CsrBtHfgCallListResSizeof(void *msg);
CsrUint8 *CsrBtHfgCallListResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgCallListResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgCallListResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ
CsrSize CsrBtHfgSpeakerGainReqSizeof(void *msg);
CsrUint8 *CsrBtHfgSpeakerGainReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSpeakerGainReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgSpeakerGainReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND
CsrSize CsrBtHfgAudioConnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAudioConnectIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND
CsrSize CsrBtHfgNoiseEchoIndSizeof(void *msg);
CsrUint8 *CsrBtHfgNoiseEchoIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgNoiseEchoIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgNoiseEchoIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ
CsrSize CsrBtHfgServiceConnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfgServiceConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgServiceConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgServiceConnectReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ
CsrSize CsrBtHfgC2cTxtReqSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cTxtReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgC2cTxtReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ
CsrSize CsrBtHfgCallHandlingReqSizeof(void *msg);
CsrUint8 *CsrBtHfgCallHandlingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgCallHandlingReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgCallHandlingReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ
CsrSize CsrBtHfgMicGainReqSizeof(void *msg);
CsrUint8 *CsrBtHfgMicGainReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgMicGainReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgMicGainReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_IND
CsrSize CsrBtHfgDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfgDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgDisconnectIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ
CsrSize CsrBtHfgAudioDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAudioDisconnectReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND
CsrSize CsrBtHfgMicGainIndSizeof(void *msg);
CsrUint8 *CsrBtHfgMicGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgMicGainIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgMicGainIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ
CsrSize CsrBtHfgAudioConnectReqSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAudioConnectReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM
CsrSize CsrBtHfgDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgDeactivateCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_RING_CFM
CsrSize CsrBtHfgRingCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgRingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgRingCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgRingCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_RING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_IND
CsrSize CsrBtHfgOperatorIndSizeof(void *msg);
CsrUint8 *CsrBtHfgOperatorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgOperatorIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgOperatorIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_RES
CsrSize CsrBtHfgBtInputResSizeof(void *msg);
CsrUint8 *CsrBtHfgBtInputResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgBtInputResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgBtInputResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND
CsrSize CsrBtHfgCallHandlingIndSizeof(void *msg);
CsrUint8 *CsrBtHfgCallHandlingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgCallHandlingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgCallHandlingIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HFG_ANSWER_IND
CsrSize CsrBtHfgAnswerIndSizeof(void *msg);
CsrUint8 *CsrBtHfgAnswerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAnswerIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAnswerIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_ANSWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM
CsrSize CsrBtHfgSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgSecurityInCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ
CsrSize CsrBtHfgDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtHfgDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgDeactivateReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND
CsrSize CsrBtHfgC2cSmsGetIndSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cSmsGetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cSmsGetIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgC2cSmsGetIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_IND
CsrSize CsrBtHfgCallListIndSizeof(void *msg);
CsrUint8 *CsrBtHfgCallListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgCallListIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgCallListIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND
CsrSize CsrBtHfgGenerateDtmfIndSizeof(void *msg);
CsrUint8 *CsrBtHfgGenerateDtmfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgGenerateDtmfIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgGenerateDtmfIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM
CsrSize CsrBtHfgAudioConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgAudioConnectCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_REQ
CsrSize CsrBtHfgAtCmdReqSizeof(void *msg);
CsrUint8 *CsrBtHfgAtCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAtCmdReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgAtCmdReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ
CsrSize CsrBtHfgSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtHfgSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgSecurityOutReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND
CsrSize CsrBtHfgVoiceRecogIndSizeof(void *msg);
CsrUint8 *CsrBtHfgVoiceRecogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgVoiceRecogIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgVoiceRecogIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND
CsrSize CsrBtHfgServiceConnectIndSizeof(void *msg);
CsrUint8 *CsrBtHfgServiceConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgServiceConnectIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgServiceConnectIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
CsrSize CsrBtHfgGetC2cAdpcmLocalSupportedReqSizeof(void *msg);
CsrUint8 *CsrBtHfgGetC2cAdpcmLocalSupportedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgGetC2cAdpcmLocalSupportedReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgGetC2cAdpcmLocalSupportedReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_IND
CsrSize CsrBtHfgAtCmdIndSizeof(void *msg);
CsrUint8 *CsrBtHfgAtCmdIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAtCmdIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgAtCmdIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ
CsrSize CsrBtHfgVoiceRecogReqSizeof(void *msg);
CsrUint8 *CsrBtHfgVoiceRecogReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgVoiceRecogReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgVoiceRecogReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_IND
CsrSize CsrBtHfgBtInputIndSizeof(void *msg);
CsrUint8 *CsrBtHfgBtInputIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgBtInputIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgBtInputIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM
CsrSize CsrBtHfgDeregisterTimeCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgDeregisterTimeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgDeregisterTimeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgDeregisterTimeCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
CsrSize CsrBtHfgGetC2cAdpcmLocalSupportedIndSizeof(void *msg);
CsrUint8 *CsrBtHfgGetC2cAdpcmLocalSupportedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgGetC2cAdpcmLocalSupportedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgGetC2cAdpcmLocalSupportedIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ
CsrSize CsrBtHfgConfigSniffReqSizeof(void *msg);
CsrUint8 *CsrBtHfgConfigSniffReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgConfigSniffReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgConfigSniffReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_POWER_IND
CsrSize CsrBtHfgC2cPowerIndSizeof(void *msg);
CsrUint8 *CsrBtHfgC2cPowerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgC2cPowerIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgC2cPowerIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_C2C_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND
CsrSize CsrBtHfgSubscriberNumberIndSizeof(void *msg);
CsrUint8 *CsrBtHfgSubscriberNumberIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSubscriberNumberIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgSubscriberNumberIndSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ
CsrSize CsrBtHfgConfigAudioReqSizeof(void *msg);
CsrUint8 *CsrBtHfgConfigAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgConfigAudioReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgConfigAudioReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES
CsrSize CsrBtHfgManualIndicatorResSizeof(void *msg);
CsrUint8 *CsrBtHfgManualIndicatorResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgManualIndicatorResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgManualIndicatorResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES
CsrSize CsrBtHfgSubscriberNumberResSizeof(void *msg);
CsrUint8 *CsrBtHfgSubscriberNumberResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgSubscriberNumberResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgSubscriberNumberResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES */

#ifndef EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ
CsrSize CsrBtHfgActivateReqSizeof(void *msg);
CsrUint8 *CsrBtHfgActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ
CsrSize CsrBtHfgConfigSingleAtcmdReqSizeof(void *msg);
CsrUint8 *CsrBtHfgConfigSingleAtcmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgConfigSingleAtcmdReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgConfigSingleAtcmdReqSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM
CsrSize CsrBtHfgConfigSingleAtcmdCfmSizeof(void *msg);
CsrUint8 *CsrBtHfgConfigSingleAtcmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgConfigSingleAtcmdCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHfgConfigSingleAtcmdCfmSerFree CsrBtHfgPfree
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES
CsrSize CsrBtHfgAudioAcceptConnectResSizeof(void *msg);
CsrUint8 *CsrBtHfgAudioAcceptConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHfgAudioAcceptConnectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHfgAudioAcceptConnectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES */

#ifdef __cplusplus
}
#endif

#endif
