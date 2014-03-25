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
#include "csr_bt_hfg_converter_init.h"
#ifndef EXCLUDE_CSR_BT_HFG_MODULE
#include "csr_bt_hfg_serialize.h"
#include "csr_bt_hfg_prim.h"

static CsrMsgConvMsgEntry csr_bt_hfg_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ
    { CSR_BT_HFG_ACTIVATE_REQ, CsrBtHfgActivateReqSizeof, CsrBtHfgActivateReqSer, CsrBtHfgActivateReqDes, CsrBtHfgActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_ANSWER_IND
    { CSR_BT_HFG_ANSWER_IND, CsrBtHfgAnswerIndSizeof, CsrBtHfgAnswerIndSer, CsrBtHfgAnswerIndDes, CsrBtHfgAnswerIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_ANSWER_IND */
#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_IND
    { CSR_BT_HFG_AT_CMD_IND, CsrBtHfgAtCmdIndSizeof, CsrBtHfgAtCmdIndSer, CsrBtHfgAtCmdIndDes, CsrBtHfgAtCmdIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_IND */
#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_REQ
    { CSR_BT_HFG_AT_CMD_REQ, CsrBtHfgAtCmdReqSizeof, CsrBtHfgAtCmdReqSer, CsrBtHfgAtCmdReqDes, CsrBtHfgAtCmdReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND
    { CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND, CsrBtHfgAudioAcceptConnectIndSizeof, CsrBtHfgAudioAcceptConnectIndSer, CsrBtHfgAudioAcceptConnectIndDes, CsrBtHfgAudioAcceptConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES
    { CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES, CsrBtHfgAudioAcceptConnectResSizeof, CsrBtHfgAudioAcceptConnectResSer, CsrBtHfgAudioAcceptConnectResDes, CsrBtHfgAudioAcceptConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM
    { CSR_BT_HFG_AUDIO_CONNECT_CFM, CsrBtHfgAudioConnectCfmSizeof, CsrBtHfgAudioConnectCfmSer, CsrBtHfgAudioConnectCfmDes, CsrBtHfgAudioConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND
    { CSR_BT_HFG_AUDIO_CONNECT_IND, CsrBtHfgAudioConnectIndSizeof, CsrBtHfgAudioConnectIndSer, CsrBtHfgAudioConnectIndDes, CsrBtHfgAudioConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ
    { CSR_BT_HFG_AUDIO_CONNECT_REQ, CsrBtHfgAudioConnectReqSizeof, CsrBtHfgAudioConnectReqSer, CsrBtHfgAudioConnectReqDes, CsrBtHfgAudioConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM
    { CSR_BT_HFG_AUDIO_DISCONNECT_CFM, CsrBtHfgAudioDisconnectCfmSizeof, CsrBtHfgAudioDisconnectCfmSer, CsrBtHfgAudioDisconnectCfmDes, CsrBtHfgAudioDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND
    { CSR_BT_HFG_AUDIO_DISCONNECT_IND, CsrBtHfgAudioDisconnectIndSizeof, CsrBtHfgAudioDisconnectIndSer, CsrBtHfgAudioDisconnectIndDes, CsrBtHfgAudioDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ
    { CSR_BT_HFG_AUDIO_DISCONNECT_REQ, CsrBtHfgAudioDisconnectReqSizeof, CsrBtHfgAudioDisconnectReqSer, CsrBtHfgAudioDisconnectReqDes, CsrBtHfgAudioDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_IND
    { CSR_BT_HFG_BT_INPUT_IND, CsrBtHfgBtInputIndSizeof, CsrBtHfgBtInputIndSer, CsrBtHfgBtInputIndDes, CsrBtHfgBtInputIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_RES
    { CSR_BT_HFG_BT_INPUT_RES, CsrBtHfgBtInputResSizeof, CsrBtHfgBtInputResSer, CsrBtHfgBtInputResDes, CsrBtHfgBtInputResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_RES */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND
    { CSR_BT_HFG_C2C_BATTERY_IND, CsrBtHfgC2cBatteryIndSizeof, CsrBtHfgC2cBatteryIndSer, CsrBtHfgC2cBatteryIndDes, CsrBtHfgC2cBatteryIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_POWER_IND
    { CSR_BT_HFG_C2C_POWER_IND, CsrBtHfgC2cPowerIndSizeof, CsrBtHfgC2cPowerIndSer, CsrBtHfgC2cPowerIndDes, CsrBtHfgC2cPowerIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_POWER_IND */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_IND
    { CSR_BT_HFG_C2C_SF_IND, CsrBtHfgC2cSfIndSizeof, CsrBtHfgC2cSfIndSer, CsrBtHfgC2cSfIndDes, CsrBtHfgC2cSfIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_IND */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_REQ
    { CSR_BT_HFG_C2C_SF_REQ, CsrBtHfgC2cSfReqSizeof, CsrBtHfgC2cSfReqSer, CsrBtHfgC2cSfReqDes, CsrBtHfgC2cSfReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ
    { CSR_BT_HFG_C2C_SMS_ARRIVE_REQ, CsrBtHfgC2cSmsArriveReqSizeof, CsrBtHfgC2cSmsArriveReqSer, CsrBtHfgC2cSmsArriveReqDes, CsrBtHfgC2cSmsArriveReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND
    { CSR_BT_HFG_C2C_SMS_GET_IND, CsrBtHfgC2cSmsGetIndSizeof, CsrBtHfgC2cSmsGetIndSer, CsrBtHfgC2cSmsGetIndDes, CsrBtHfgC2cSmsGetIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ
    { CSR_BT_HFG_C2C_SMS_TXT_REQ, CsrBtHfgC2cSmsTxtReqSizeof, CsrBtHfgC2cSmsTxtReqSer, CsrBtHfgC2cSmsTxtReqDes, CsrBtHfgC2cSmsTxtReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ
    { CSR_BT_HFG_C2C_TXT_REQ, CsrBtHfgC2cTxtReqSizeof, CsrBtHfgC2cTxtReqSer, CsrBtHfgC2cTxtReqDes, CsrBtHfgC2cTxtReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND
    { CSR_BT_HFG_CALL_HANDLING_IND, CsrBtHfgCallHandlingIndSizeof, CsrBtHfgCallHandlingIndSer, CsrBtHfgCallHandlingIndDes, CsrBtHfgCallHandlingIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND */
#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ
    { CSR_BT_HFG_CALL_HANDLING_REQ, CsrBtHfgCallHandlingReqSizeof, CsrBtHfgCallHandlingReqSer, CsrBtHfgCallHandlingReqDes, CsrBtHfgCallHandlingReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES
    { CSR_BT_HFG_CALL_HANDLING_RES, CsrBtHfgCallHandlingResSizeof, CsrBtHfgCallHandlingResSer, CsrBtHfgCallHandlingResDes, CsrBtHfgCallHandlingResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES */
#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_IND
    { CSR_BT_HFG_CALL_LIST_IND, CsrBtHfgCallListIndSizeof, CsrBtHfgCallListIndSer, CsrBtHfgCallListIndDes, CsrBtHfgCallListIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_IND */
#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_RES
    { CSR_BT_HFG_CALL_LIST_RES, CsrBtHfgCallListResSizeof, CsrBtHfgCallListResSer, CsrBtHfgCallListResDes, CsrBtHfgCallListResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_RES */
#ifndef EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ
    { CSR_BT_HFG_CALL_WAITING_REQ, CsrBtHfgCallWaitingReqSizeof, CsrBtHfgCallWaitingReqSer, CsrBtHfgCallWaitingReqDes, CsrBtHfgCallWaitingReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ
    { CSR_BT_HFG_CANCEL_CONNECT_REQ, CsrBtHfgCancelConnectReqSizeof, CsrBtHfgCancelConnectReqSer, CsrBtHfgCancelConnectReqDes, CsrBtHfgCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM
    { CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM, CsrBtHfgConfigAtcmdHandlingCfmSizeof, CsrBtHfgConfigAtcmdHandlingCfmSer, CsrBtHfgConfigAtcmdHandlingCfmDes, CsrBtHfgConfigAtcmdHandlingCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ
    { CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ, CsrBtHfgConfigAtcmdHandlingReqSizeof, CsrBtHfgConfigAtcmdHandlingReqSer, CsrBtHfgConfigAtcmdHandlingReqDes, CsrBtHfgConfigAtcmdHandlingReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ
    { CSR_BT_HFG_CONFIG_AUDIO_REQ, CsrBtHfgConfigAudioReqSizeof, CsrBtHfgConfigAudioReqSer, CsrBtHfgConfigAudioReqDes, CsrBtHfgConfigAudioReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM
    { CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM, CsrBtHfgConfigSingleAtcmdCfmSizeof, CsrBtHfgConfigSingleAtcmdCfmSer, CsrBtHfgConfigSingleAtcmdCfmDes, CsrBtHfgConfigSingleAtcmdCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ
    { CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ, CsrBtHfgConfigSingleAtcmdReqSizeof, CsrBtHfgConfigSingleAtcmdReqSer, CsrBtHfgConfigSingleAtcmdReqDes, CsrBtHfgConfigSingleAtcmdReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ
    { CSR_BT_HFG_CONFIG_SNIFF_REQ, CsrBtHfgConfigSniffReqSizeof, CsrBtHfgConfigSniffReqSer, CsrBtHfgConfigSniffReqDes, CsrBtHfgConfigSniffReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM
    { CSR_BT_HFG_DEACTIVATE_CFM, CsrBtHfgDeactivateCfmSizeof, CsrBtHfgDeactivateCfmSer, CsrBtHfgDeactivateCfmDes, CsrBtHfgDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ
    { CSR_BT_HFG_DEACTIVATE_REQ, CsrBtHfgDeactivateReqSizeof, CsrBtHfgDeactivateReqSer, CsrBtHfgDeactivateReqDes, CsrBtHfgDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM
    { CSR_BT_HFG_DEREGISTER_TIME_CFM, CsrBtHfgDeregisterTimeCfmSizeof, CsrBtHfgDeregisterTimeCfmSer, CsrBtHfgDeregisterTimeCfmDes, CsrBtHfgDeregisterTimeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ
    { CSR_BT_HFG_DEREGISTER_TIME_REQ, CsrBtHfgDeregisterTimeReqSizeof, CsrBtHfgDeregisterTimeReqSer, CsrBtHfgDeregisterTimeReqDes, CsrBtHfgDeregisterTimeReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_DIAL_IND
    { CSR_BT_HFG_DIAL_IND, CsrBtHfgDialIndSizeof, CsrBtHfgDialIndSer, CsrBtHfgDialIndDes, CsrBtHfgDialIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_IND */
#ifndef EXCLUDE_CSR_BT_HFG_DIAL_RES
    { CSR_BT_HFG_DIAL_RES, CsrBtHfgDialResSizeof, CsrBtHfgDialResSer, CsrBtHfgDialResDes, CsrBtHfgDialResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_RES */
#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_IND
    { CSR_BT_HFG_DISCONNECT_IND, CsrBtHfgDisconnectIndSizeof, CsrBtHfgDisconnectIndSer, CsrBtHfgDisconnectIndDes, CsrBtHfgDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ
    { CSR_BT_HFG_DISCONNECT_REQ, CsrBtHfgDisconnectReqSizeof, CsrBtHfgDisconnectReqSer, CsrBtHfgDisconnectReqDes, CsrBtHfgDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND
    { CSR_BT_HFG_GENERATE_DTMF_IND, CsrBtHfgGenerateDtmfIndSizeof, CsrBtHfgGenerateDtmfIndSer, CsrBtHfgGenerateDtmfIndDes, CsrBtHfgGenerateDtmfIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND */
#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
    { CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND, CsrBtHfgGetC2cAdpcmLocalSupportedIndSizeof, CsrBtHfgGetC2cAdpcmLocalSupportedIndSer, CsrBtHfgGetC2cAdpcmLocalSupportedIndDes, CsrBtHfgGetC2cAdpcmLocalSupportedIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */
#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
    { CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ, CsrBtHfgGetC2cAdpcmLocalSupportedReqSizeof, CsrBtHfgGetC2cAdpcmLocalSupportedReqSer, CsrBtHfgGetC2cAdpcmLocalSupportedReqDes, CsrBtHfgGetC2cAdpcmLocalSupportedReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING
    { CSR_BT_HFG_HOUSE_CLEANING, CsrBtHfgHouseCleaningSizeof, CsrBtHfgHouseCleaningSer, CsrBtHfgHouseCleaningDes, CsrBtHfgHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ
    { CSR_BT_HFG_INBAND_RINGING_REQ, CsrBtHfgInbandRingingReqSizeof, CsrBtHfgInbandRingingReqSer, CsrBtHfgInbandRingingReqDes, CsrBtHfgInbandRingingReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND
    { CSR_BT_HFG_MANUAL_INDICATOR_IND, CsrBtHfgManualIndicatorIndSizeof, CsrBtHfgManualIndicatorIndSer, CsrBtHfgManualIndicatorIndDes, CsrBtHfgManualIndicatorIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND */
#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES
    { CSR_BT_HFG_MANUAL_INDICATOR_RES, CsrBtHfgManualIndicatorResSizeof, CsrBtHfgManualIndicatorResSer, CsrBtHfgManualIndicatorResDes, CsrBtHfgManualIndicatorResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES */
#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND
    { CSR_BT_HFG_MIC_GAIN_IND, CsrBtHfgMicGainIndSizeof, CsrBtHfgMicGainIndSer, CsrBtHfgMicGainIndDes, CsrBtHfgMicGainIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND */
#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ
    { CSR_BT_HFG_MIC_GAIN_REQ, CsrBtHfgMicGainReqSizeof, CsrBtHfgMicGainReqSer, CsrBtHfgMicGainReqDes, CsrBtHfgMicGainReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND
    { CSR_BT_HFG_NOISE_ECHO_IND, CsrBtHfgNoiseEchoIndSizeof, CsrBtHfgNoiseEchoIndSer, CsrBtHfgNoiseEchoIndDes, CsrBtHfgNoiseEchoIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND */
#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_IND
    { CSR_BT_HFG_OPERATOR_IND, CsrBtHfgOperatorIndSizeof, CsrBtHfgOperatorIndSer, CsrBtHfgOperatorIndDes, CsrBtHfgOperatorIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_IND */
#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_RES
    { CSR_BT_HFG_OPERATOR_RES, CsrBtHfgOperatorResSizeof, CsrBtHfgOperatorResSer, CsrBtHfgOperatorResDes, CsrBtHfgOperatorResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_RES */
#ifndef EXCLUDE_CSR_BT_HFG_REJECT_IND
    { CSR_BT_HFG_REJECT_IND, CsrBtHfgRejectIndSizeof, CsrBtHfgRejectIndSer, CsrBtHfgRejectIndDes, CsrBtHfgRejectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_REJECT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_RING_CFM
    { CSR_BT_HFG_RING_CFM, CsrBtHfgRingCfmSizeof, CsrBtHfgRingCfmSer, CsrBtHfgRingCfmDes, CsrBtHfgRingCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_RING_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_RING_REQ
    { CSR_BT_HFG_RING_REQ, CsrBtHfgRingReqSizeof, CsrBtHfgRingReqSer, CsrBtHfgRingReqDes, CsrBtHfgRingReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_RING_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM
    { CSR_BT_HFG_SECURITY_IN_CFM, CsrBtHfgSecurityInCfmSizeof, CsrBtHfgSecurityInCfmSer, CsrBtHfgSecurityInCfmDes, CsrBtHfgSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ
    { CSR_BT_HFG_SECURITY_IN_REQ, CsrBtHfgSecurityInReqSizeof, CsrBtHfgSecurityInReqSer, CsrBtHfgSecurityInReqDes, CsrBtHfgSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM
    { CSR_BT_HFG_SECURITY_OUT_CFM, CsrBtHfgSecurityOutCfmSizeof, CsrBtHfgSecurityOutCfmSer, CsrBtHfgSecurityOutCfmDes, CsrBtHfgSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ
    { CSR_BT_HFG_SECURITY_OUT_REQ, CsrBtHfgSecurityOutReqSizeof, CsrBtHfgSecurityOutReqSer, CsrBtHfgSecurityOutReqDes, CsrBtHfgSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND
    { CSR_BT_HFG_SERVICE_CONNECT_IND, CsrBtHfgServiceConnectIndSizeof, CsrBtHfgServiceConnectIndSer, CsrBtHfgServiceConnectIndDes, CsrBtHfgServiceConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ
    { CSR_BT_HFG_SERVICE_CONNECT_REQ, CsrBtHfgServiceConnectReqSizeof, CsrBtHfgServiceConnectReqSer, CsrBtHfgServiceConnectReqDes, CsrBtHfgServiceConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND
    { CSR_BT_HFG_SPEAKER_GAIN_IND, CsrBtHfgSpeakerGainIndSizeof, CsrBtHfgSpeakerGainIndSer, CsrBtHfgSpeakerGainIndDes, CsrBtHfgSpeakerGainIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND */
#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ
    { CSR_BT_HFG_SPEAKER_GAIN_REQ, CsrBtHfgSpeakerGainReqSizeof, CsrBtHfgSpeakerGainReqSer, CsrBtHfgSpeakerGainReqDes, CsrBtHfgSpeakerGainReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND
    { CSR_BT_HFG_STATUS_AUDIO_IND, CsrBtHfgStatusAudioIndSizeof, CsrBtHfgStatusAudioIndSer, CsrBtHfgStatusAudioIndDes, CsrBtHfgStatusAudioIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND */
#ifndef EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ
    { CSR_BT_HFG_STATUS_INDICATOR_SET_REQ, CsrBtHfgStatusIndicatorSetReqSizeof, CsrBtHfgStatusIndicatorSetReqSer, CsrBtHfgStatusIndicatorSetReqDes, CsrBtHfgStatusIndicatorSetReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_STATUS_LP_IND
    { CSR_BT_HFG_STATUS_LP_IND, CsrBtHfgStatusLpIndSizeof, CsrBtHfgStatusLpIndSer, CsrBtHfgStatusLpIndDes, CsrBtHfgStatusLpIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_LP_IND */
#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND
    { CSR_BT_HFG_SUBSCRIBER_NUMBER_IND, CsrBtHfgSubscriberNumberIndSizeof, CsrBtHfgSubscriberNumberIndSer, CsrBtHfgSubscriberNumberIndDes, CsrBtHfgSubscriberNumberIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND */
#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES
    { CSR_BT_HFG_SUBSCRIBER_NUMBER_RES, CsrBtHfgSubscriberNumberResSizeof, CsrBtHfgSubscriberNumberResSer, CsrBtHfgSubscriberNumberResDes, CsrBtHfgSubscriberNumberResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES */
#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND
    { CSR_BT_HFG_VOICE_RECOG_IND, CsrBtHfgVoiceRecogIndSizeof, CsrBtHfgVoiceRecogIndSer, CsrBtHfgVoiceRecogIndDes, CsrBtHfgVoiceRecogIndSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND */
#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ
    { CSR_BT_HFG_VOICE_RECOG_REQ, CsrBtHfgVoiceRecogReqSizeof, CsrBtHfgVoiceRecogReqSer, CsrBtHfgVoiceRecogReqDes, CsrBtHfgVoiceRecogReqSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ */
#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES
    { CSR_BT_HFG_VOICE_RECOG_RES, CsrBtHfgVoiceRecogResSizeof, CsrBtHfgVoiceRecogResSer, CsrBtHfgVoiceRecogResDes, CsrBtHfgVoiceRecogResSerFree },
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtHfgConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_HFG_PRIM, csr_bt_hfg_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_hfg_conv_info = {
    CSR_BT_HFG_PRIM,
    "CSR_BT_HFG_PRIM",
    csr_bt_hfg_conv_lut
};

CsrLogPrimitiveInformation* CsrBtHfgTechInfoGet(void)
{
    return &csr_bt_hfg_conv_info;
}
#endif
#endif

