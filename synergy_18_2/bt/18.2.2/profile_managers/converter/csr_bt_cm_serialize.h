#ifndef CSR_BT_CM_SERIALIZE_H__
#define CSR_BT_CM_SERIALIZE_H__
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

void CsrBtCmPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM
CsrSize CsrBtCmWriteAfhChannelAssessmentModeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWriteAfhChannelAssessmentModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteAfhChannelAssessmentModeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteAfhChannelAssessmentModeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ
CsrSize CsrBtCmWriteAfhChannelAssessmentModeReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteAfhChannelAssessmentModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteAfhChannelAssessmentModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteAfhChannelAssessmentModeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND
CsrSize CsrBtCmSdcUuid128SearchIndSizeof(void *msg);
CsrUint8 *CsrBtCmSdcUuid128SearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcUuid128SearchIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcUuid128SearchIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ
CsrSize CsrBtCmWriteVoiceSettingsReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteVoiceSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteVoiceSettingsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteVoiceSettingsReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES
CsrSize CsrBtCmSmUserPasskeyRequestResSizeof(void *msg);
CsrUint8 *CsrBtCmSmUserPasskeyRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmUserPasskeyRequestResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmUserPasskeyRequestResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ
CsrSize CsrBtCmSmBondingCancelReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmBondingCancelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmBondingCancelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmBondingCancelReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND
CsrSize CsrBtCmA2dpBitRateIndSizeof(void *msg);
CsrUint8 *CsrBtCmA2dpBitRateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmA2dpBitRateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmA2dpBitRateIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM
CsrSize CsrBtCmReadRemoteVersionCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteVersionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteVersionCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRemoteVersionCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmL2caConnectAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caConnectAcceptCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ
CsrSize CsrBtCmEirUpdateManufacturerDataReqSizeof(void *msg);
CsrUint8 *CsrBtCmEirUpdateManufacturerDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEirUpdateManufacturerDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmEirUpdateManufacturerDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM
CsrSize CsrBtCmL2caConnectionlessDataCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caConnectionlessDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caConnectionlessDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caConnectionlessDataCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ
CsrSize CsrBtCmRoleSwitchConfigReqSizeof(void *msg);
CsrUint8 *CsrBtCmRoleSwitchConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRoleSwitchConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRoleSwitchConfigReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM
CsrSize CsrBtCmWritePageToCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWritePageToCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWritePageToCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWritePageToCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES
CsrSize CsrBtCmMapScoPcmResSizeof(void *msg);
CsrUint8 *CsrBtCmMapScoPcmResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmMapScoPcmResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmMapScoPcmResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ
CsrSize CsrBtCmLogicalChannelTypeReqSizeof(void *msg);
CsrUint8 *CsrBtCmLogicalChannelTypeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLogicalChannelTypeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLogicalChannelTypeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM
CsrSize CsrBtCmModeChangeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmModeChangeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmModeChangeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmModeChangeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ
CsrSize CsrBtCmL2caGetChannelInfoReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caGetChannelInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caGetChannelInfoReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caGetChannelInfoReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM
CsrSize CsrBtCmReadLinkPolicyCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadLinkPolicyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLinkPolicyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLinkPolicyCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ
CsrSize CsrBtCmSetAfhChannelClassReqSizeof(void *msg);
CsrUint8 *CsrBtCmSetAfhChannelClassReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetAfhChannelClassReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetAfhChannelClassReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND
CsrSize CsrBtCmScoDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmScoDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoDisconnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM
CsrSize CsrBtCmDataBufferEmptyCfmSizeof(void *msg);
CsrUint8 *CsrBtCmDataBufferEmptyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDataBufferEmptyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDataBufferEmptyCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND
CsrSize CsrBtCmMoveChannelCmpIndSizeof(void *msg);
CsrUint8 *CsrBtCmMoveChannelCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmMoveChannelCmpIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmMoveChannelCmpIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ
CsrSize CsrBtCmWriteClassOfDeviceReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteClassOfDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteClassOfDeviceReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteClassOfDeviceReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmConnectAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtCmConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmConnectAcceptReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ
CsrSize CsrBtCmReadClockReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadClockReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadClockReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadClockReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ
CsrSize CsrBtCmSmDeleteStoreLinkKeyReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmDeleteStoreLinkKeyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmDeleteStoreLinkKeyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmDeleteStoreLinkKeyReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_REQ
CsrSize CsrBtCmRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRegisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ
CsrSize CsrBtCmDmCheckSsrReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmCheckSsrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmCheckSsrReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmCheckSsrReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_CFM
CsrSize CsrBtCmInquiryCfmSizeof(void *msg);
CsrUint8 *CsrBtCmInquiryCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmInquiryCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmInquiryCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ
CsrSize CsrBtCmWriteInquiryscanSettingsReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteInquiryscanSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteInquiryscanSettingsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteInquiryscanSettingsReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ
CsrSize CsrBtCmReadLocalVersionReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalVersionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalVersionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLocalVersionReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM
CsrSize CsrBtCmSdsExtRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdsExtRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdsExtRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdsExtRegisterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM
CsrSize CsrBtCmReadAdvertisingChTxPowerCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadAdvertisingChTxPowerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadAdvertisingChTxPowerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadAdvertisingChTxPowerCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES
CsrSize CsrBtCmMoveChannelResSizeof(void *msg);
CsrUint8 *CsrBtCmMoveChannelResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmMoveChannelResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmMoveChannelResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM
CsrSize CsrBtCmSetEventFilterCodCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSetEventFilterCodCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetEventFilterCodCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetEventFilterCodCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND
CsrSize CsrBtCmModeChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmModeChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmModeChangeIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ
CsrSize CsrBtCmSmUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmUnregisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM
CsrSize CsrBtCmReadAfhChannelAssessmentModeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadAfhChannelAssessmentModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadAfhChannelAssessmentModeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadAfhChannelAssessmentModeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM
CsrSize CsrBtCmReadFailedContactCounterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadFailedContactCounterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadFailedContactCounterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadFailedContactCounterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_CFM
CsrSize CsrBtCmDataCfmSizeof(void *msg);
CsrUint8 *CsrBtCmDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDataCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ
CsrSize CsrBtCmBnepDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmBnepDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepDisconnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND
CsrSize CsrBtCmSyncConnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmSyncConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSyncConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSyncConnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM
CsrSize CsrBtCmReadLocalNameCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalNameCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmReadLocalNameCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND
CsrSize CsrBtCmRemoteVersionIndSizeof(void *msg);
CsrUint8 *CsrBtCmRemoteVersionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRemoteVersionIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRemoteVersionIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ
CsrSize CsrBtCmDmWriteCacheParamsReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmWriteCacheParamsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmWriteCacheParamsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmWriteCacheParamsReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND
CsrSize CsrBtCmLeEventConnectionIndSizeof(void *msg);
CsrUint8 *CsrBtCmLeEventConnectionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeEventConnectionIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeEventConnectionIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_CFM
CsrSize CsrBtCmRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRegisterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ
CsrSize CsrBtCmSmEncryptionReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmEncryptionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmEncryptionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmEncryptionReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_CFM
CsrSize CsrBtCmPortnegCfmSizeof(void *msg);
CsrUint8 *CsrBtCmPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmPortnegCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmPortnegCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ
CsrSize CsrBtCmLeTestEndReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeTestEndReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeTestEndReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeTestEndReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ
CsrSize CsrBtCmSmReadDeviceReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmReadDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmReadDeviceReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmReadDeviceReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ
CsrSize CsrBtCmBnepExtendedMulticastDataReqSizeof(void *msg);
CsrUint8 *CsrBtCmBnepExtendedMulticastDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepExtendedMulticastDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmBnepExtendedMulticastDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM
CsrSize CsrBtCmReadLocalBdAddrCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalBdAddrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalBdAddrCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLocalBdAddrCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ
CsrSize CsrBtCmScoDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmScoDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoDisconnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_REQ
CsrSize CsrBtCmConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ
CsrSize CsrBtCmGetLinkQualityReqSizeof(void *msg);
CsrUint8 *CsrBtCmGetLinkQualityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmGetLinkQualityReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmGetLinkQualityReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ
CsrSize CsrBtCmSetLocalNameReqSizeof(void *msg);
CsrUint8 *CsrBtCmSetLocalNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetLocalNameReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSetLocalNameReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ
CsrSize CsrBtCmSdcRfcSearchReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcRfcSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcRfcSearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcRfcSearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ
CsrSize CsrBtCmRoleDiscoveryReqSizeof(void *msg);
CsrUint8 *CsrBtCmRoleDiscoveryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRoleDiscoveryReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRoleDiscoveryReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES
CsrSize CsrBtCmSmPinRequestResSizeof(void *msg);
CsrUint8 *CsrBtCmSmPinRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmPinRequestResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmPinRequestResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ
CsrSize CsrBtCmSdcOpenReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcOpenReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcOpenReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ
CsrSize CsrBtCmWriteInquiryscanTypeReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteInquiryscanTypeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteInquiryscanTypeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteInquiryscanTypeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM
CsrSize CsrBtCmReadScanEnableCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadScanEnableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadScanEnableCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadScanEnableCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ
CsrSize CsrBtCmDisableDutModeReqSizeof(void *msg);
CsrUint8 *CsrBtCmDisableDutModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDisableDutModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDisableDutModeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES
CsrSize CsrBtCmSmUserPasskeyRequestNegResSizeof(void *msg);
CsrUint8 *CsrBtCmSmUserPasskeyRequestNegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmUserPasskeyRequestNegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmUserPasskeyRequestNegResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING
CsrSize CsrBtCmSmHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtCmSmHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmHouseCleaningSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ
CsrSize CsrBtCmConnectExtReqSizeof(void *msg);
CsrUint8 *CsrBtCmConnectExtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmConnectExtReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmConnectExtReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM
CsrSize CsrBtCmSetEventMaskCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSetEventMaskCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetEventMaskCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetEventMaskCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ
CsrSize CsrBtCmDataBufferEmptyReqSizeof(void *msg);
CsrUint8 *CsrBtCmDataBufferEmptyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDataBufferEmptyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDataBufferEmptyReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM
CsrSize CsrBtCmSdcSearchCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdcSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcSearchCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcSearchCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ
CsrSize CsrBtCmSdcSearchReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcSearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcSearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES
CsrSize CsrBtCmLeAcceptConnparamUpdateResSizeof(void *msg);
CsrUint8 *CsrBtCmLeAcceptConnparamUpdateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeAcceptConnparamUpdateResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeAcceptConnparamUpdateResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ
CsrSize CsrBtCmWriteLinkPolicyReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteLinkPolicyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteLinkPolicyReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmWriteLinkPolicyReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ
CsrSize CsrBtCmRegisterHandlerReqSizeof(void *msg);
CsrUint8 *CsrBtCmRegisterHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRegisterHandlerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRegisterHandlerReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM
CsrSize CsrBtCmSdcOpenCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdcOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcOpenCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcOpenCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND
CsrSize CsrBtCmBnepDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmBnepDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepDisconnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ
CsrSize CsrBtCmBnepModeChangeReqSizeof(void *msg);
CsrUint8 *CsrBtCmBnepModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepModeChangeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepModeChangeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_REQ
CsrSize CsrBtCmWriteCodReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteCodReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteCodReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteCodReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND
CsrSize CsrBtCmAclConnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmAclConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAclConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAclConnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM
CsrSize CsrBtCmScoRenegotiateCfmSizeof(void *msg);
CsrUint8 *CsrBtCmScoRenegotiateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoRenegotiateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoRenegotiateCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_IND
CsrSize CsrBtCmDataIndSizeof(void *msg);
CsrUint8 *CsrBtCmDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES
CsrSize CsrBtCmSmUserConfirmationRequestNegResSizeof(void *msg);
CsrUint8 *CsrBtCmSmUserConfirmationRequestNegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmUserConfirmationRequestNegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmUserConfirmationRequestNegResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES
CsrSize CsrBtCmBnepDisconnectResSizeof(void *msg);
CsrUint8 *CsrBtCmBnepDisconnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepDisconnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepDisconnectResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ
CsrSize CsrBtCmL2caDataAbortReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDataAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDataAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caDataAbortReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ
CsrSize CsrBtCmSdsRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdsRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdsRegisterReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdsRegisterReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_REQ
CsrSize CsrBtCmInquiryReqSizeof(void *msg);
CsrUint8 *CsrBtCmInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmInquiryReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmInquiryReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ
CsrSize CsrBtCmSmAclOpenReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmAclOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmAclOpenReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmAclOpenReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtCmSmSetEncryptionKeySizeReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmSetEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmSetEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmSetEncryptionKeySizeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ
CsrSize CsrBtCmRfcModeChangeReqSizeof(void *msg);
CsrUint8 *CsrBtCmRfcModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRfcModeChangeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRfcModeChangeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ
CsrSize CsrBtCmL2caDataReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmL2caDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM
CsrSize CsrBtCmSwitchRoleCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSwitchRoleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSwitchRoleCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSwitchRoleCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmL2caCancelConnectAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caCancelConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caCancelConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caCancelConnectAcceptCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ
CsrSize CsrBtCmWritePagescanSettingsReqSizeof(void *msg);
CsrUint8 *CsrBtCmWritePagescanSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWritePagescanSettingsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWritePagescanSettingsReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ
CsrSize CsrBtCmScoRenegotiateReqSizeof(void *msg);
CsrUint8 *CsrBtCmScoRenegotiateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoRenegotiateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoRenegotiateReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ
CsrSize CsrBtCmCancelReadRemoteNameReqSizeof(void *msg);
CsrUint8 *CsrBtCmCancelReadRemoteNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmCancelReadRemoteNameReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmCancelReadRemoteNameReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ
CsrSize CsrBtCmReadTxPowerLevelReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadTxPowerLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadTxPowerLevelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadTxPowerLevelReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM
CsrSize CsrBtCmWriteInquiryscanSettingsCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWriteInquiryscanSettingsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteInquiryscanSettingsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteInquiryscanSettingsCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
CsrSize CsrBtCmSdcRfcExtendedSearchReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcRfcExtendedSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcRfcExtendedSearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcRfcExtendedSearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ
CsrSize CsrBtCmSdsUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdsUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdsUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdsUnregisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ
CsrSize CsrBtCmPrivateSwitchRoleReqSizeof(void *msg);
CsrUint8 *CsrBtCmPrivateSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmPrivateSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmPrivateSwitchRoleReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ
CsrSize CsrBtCmSdcCancelSearchReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcCancelSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcCancelSearchReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcCancelSearchReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ
CsrSize CsrBtCmEnEnableEnhancementsReqSizeof(void *msg);
CsrUint8 *CsrBtCmEnEnableEnhancementsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEnEnableEnhancementsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmEnEnableEnhancementsReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT
CsrSize CsrBtCmAcceptConnectTimeoutSizeof(void *msg);
CsrUint8 *CsrBtCmAcceptConnectTimeoutSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAcceptConnectTimeoutDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAcceptConnectTimeoutSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ
CsrSize CsrBtCmModeChangeConfigReqSizeof(void *msg);
CsrUint8 *CsrBtCmModeChangeConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmModeChangeConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmModeChangeConfigReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM
CsrSize CsrBtCmL2caConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caConnectCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ
CsrSize CsrBtCmReadLocalBdAddrReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalBdAddrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalBdAddrReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLocalBdAddrReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
CsrSize CsrBtCmSdcUuid128RfcSearchReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcUuid128RfcSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcUuid128RfcSearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcUuid128RfcSearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_REQ
CsrSize CsrBtCmControlReqSizeof(void *msg);
CsrUint8 *CsrBtCmControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmControlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmControlReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmL2caConnectAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmL2caConnectAcceptReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ
CsrSize CsrBtCmL2caConnectionlessDataReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caConnectionlessDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caConnectionlessDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmL2caConnectionlessDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ
CsrSize CsrBtCmLePhysicalLinkStatusReqSizeof(void *msg);
CsrUint8 *CsrBtCmLePhysicalLinkStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLePhysicalLinkStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLePhysicalLinkStatusReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ
CsrSize CsrBtCmSmAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmAuthenticateReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ
CsrSize CsrBtCmSetEventFilterBdaddrReqSizeof(void *msg);
CsrUint8 *CsrBtCmSetEventFilterBdaddrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetEventFilterBdaddrReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetEventFilterBdaddrReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ
CsrSize CsrBtCmScoConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmScoConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmScoConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM
CsrSize CsrBtCmModeChangeConfigCfmSizeof(void *msg);
CsrUint8 *CsrBtCmModeChangeConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmModeChangeConfigCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmModeChangeConfigCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND
CsrSize CsrBtCmScoRenegotiateIndSizeof(void *msg);
CsrUint8 *CsrBtCmScoRenegotiateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoRenegotiateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoRenegotiateIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM
CsrSize CsrBtCmEirFlagsCfmSizeof(void *msg);
CsrUint8 *CsrBtCmEirFlagsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEirFlagsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmEirFlagsCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ
CsrSize CsrBtCmScoCancelAcceptConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmScoCancelAcceptConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoCancelAcceptConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoCancelAcceptConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM
CsrSize CsrBtCmWriteVoiceSettingsCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWriteVoiceSettingsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteVoiceSettingsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteVoiceSettingsCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM
CsrSize CsrBtCmLeAdvertiseCfmSizeof(void *msg);
CsrUint8 *CsrBtCmLeAdvertiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeAdvertiseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeAdvertiseCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ
CsrSize CsrBtCmDmModeChangeReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmModeChangeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmModeChangeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM
CsrSize CsrBtCmL2caDataAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDataAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDataAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caDataAbortCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND
CsrSize CsrBtCmLogicalChannelTypesIndSizeof(void *msg);
CsrUint8 *CsrBtCmLogicalChannelTypesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLogicalChannelTypesIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLogicalChannelTypesIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM
CsrSize CsrBtCmSdcAttributeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdcAttributeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcAttributeCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcAttributeCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM
CsrSize CsrBtCmMoveChannelCfmSizeof(void *msg);
CsrUint8 *CsrBtCmMoveChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmMoveChannelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmMoveChannelCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ
CsrSize CsrBtCmCancelL2caConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmCancelL2caConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmCancelL2caConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmCancelL2caConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES
CsrSize CsrBtCmSmIoCapabilityRequestResSizeof(void *msg);
CsrUint8 *CsrBtCmSmIoCapabilityRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmIoCapabilityRequestResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmIoCapabilityRequestResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND
CsrSize CsrBtCmAclDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmAclDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAclDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAclDisconnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ
CsrSize CsrBtCmLeWhitelistSetReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeWhitelistSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeWhitelistSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeWhitelistSetReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ
CsrSize CsrBtCmL2caDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caDisconnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM
CsrSize CsrBtCmAlwaysMasterDevicesCfmSizeof(void *msg);
CsrUint8 *CsrBtCmAlwaysMasterDevicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAlwaysMasterDevicesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAlwaysMasterDevicesCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ
CsrSize CsrBtCmSdcUuid128SearchReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcUuid128SearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcUuid128SearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcUuid128SearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM
CsrSize CsrBtCmReadClockCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadClockCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadClockCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadClockCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM
CsrSize CsrBtCmL2caDataCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caDataCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ
CsrSize CsrBtCmReadRemoteNameReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteNameReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRemoteNameReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ
CsrSize CsrBtCmReadRemoteFeaturesReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteFeaturesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRemoteFeaturesReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_CFM
CsrSize CsrBtCmWriteCodCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWriteCodCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteCodCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteCodCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND
CsrSize CsrBtCmLocalNameChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmLocalNameChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLocalNameChangeIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmLocalNameChangeIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ
CsrSize CsrBtCmReadLocalNameReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalNameReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLocalNameReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND
CsrSize CsrBtCmReadRemoteNameIndSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteNameIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteNameIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmReadRemoteNameIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES
CsrSize CsrBtCmSmLinkKeyRequestResSizeof(void *msg);
CsrUint8 *CsrBtCmSmLinkKeyRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmLinkKeyRequestResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSmLinkKeyRequestResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ
CsrSize CsrBtCmAclDetachReqSizeof(void *msg);
CsrUint8 *CsrBtCmAclDetachReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAclDetachReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAclDetachReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ
CsrSize CsrBtCmReadScanEnableReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadScanEnableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadScanEnableReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadScanEnableReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM
CsrSize CsrBtCmWriteScanEnableCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWriteScanEnableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteScanEnableCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteScanEnableCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM
CsrSize CsrBtCmWriteInquiryscanTypeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWriteInquiryscanTypeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteInquiryscanTypeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteInquiryscanTypeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND
CsrSize CsrBtCmInquiryPageEventIndSizeof(void *msg);
CsrUint8 *CsrBtCmInquiryPageEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmInquiryPageEventIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmInquiryPageEventIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ
CsrSize CsrBtCmL2caModeChangeReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caModeChangeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caModeChangeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ
CsrSize CsrBtCmSwitchRoleReqSizeof(void *msg);
CsrUint8 *CsrBtCmSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSwitchRoleReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ
CsrSize CsrBtCmL2caConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmL2caConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM
CsrSize CsrBtCmSetAfhChannelClassCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSetAfhChannelClassCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetAfhChannelClassCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetAfhChannelClassCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM
CsrSize CsrBtCmWritePagescanSettingsCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWritePagescanSettingsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWritePagescanSettingsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWritePagescanSettingsCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND
CsrSize CsrBtCmSdcSearchIndSizeof(void *msg);
CsrUint8 *CsrBtCmSdcSearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcSearchIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcSearchIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_UNREGISTER_REQ
CsrSize CsrBtCmUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmUnregisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_REQ
CsrSize CsrBtCmSmBondingReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmBondingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmBondingReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmBondingReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ
CsrSize CsrBtCmSmLeSecurityReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmLeSecurityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmLeSecurityReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmLeSecurityReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND
CsrSize CsrBtCmLeEventAdvertisingIndSizeof(void *msg);
CsrUint8 *CsrBtCmLeEventAdvertisingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeEventAdvertisingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeEventAdvertisingIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM
CsrSize CsrBtCmWritePagescanTypeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWritePagescanTypeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWritePagescanTypeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWritePagescanTypeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND
CsrSize CsrBtCmBluecoreInitializedIndSizeof(void *msg);
CsrUint8 *CsrBtCmBluecoreInitializedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBluecoreInitializedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBluecoreInitializedIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM
CsrSize CsrBtCmWriteLinkSupervTimeoutCfmSizeof(void *msg);
CsrUint8 *CsrBtCmWriteLinkSupervTimeoutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteLinkSupervTimeoutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteLinkSupervTimeoutCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmL2caCancelConnectAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caCancelConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caCancelConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caCancelConnectAcceptReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ
CsrSize CsrBtCmLeConnparamReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeConnparamReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeConnparamReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeConnparamReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ
CsrSize CsrBtCmDmSwitchRoleReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmSwitchRoleReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ
CsrSize CsrBtCmDmWriteAutoFlushTimeoutReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmWriteAutoFlushTimeoutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmWriteAutoFlushTimeoutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmWriteAutoFlushTimeoutReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ
CsrSize CsrBtCmBnepExtendedDataReqSizeof(void *msg);
CsrUint8 *CsrBtCmBnepExtendedDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepExtendedDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmBnepExtendedDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_RES
CsrSize CsrBtCmDataResSizeof(void *msg);
CsrUint8 *CsrBtCmDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDataResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM
CsrSize CsrBtCmReadEncryptionKeySizeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadEncryptionKeySizeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadEncryptionKeySizeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadEncryptionKeySizeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_REQ
CsrSize CsrBtCmReadIacReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadIacReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadIacReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadIacReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
CsrSize CsrBtCmSdcServiceSearchReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcServiceSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcServiceSearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcServiceSearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ
CsrSize CsrBtCmLeTransmitterTestReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeTransmitterTestReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeTransmitterTestReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeTransmitterTestReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND
CsrSize CsrBtCmL2caDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caDisconnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_RES
CsrSize CsrBtCmPortnegResSizeof(void *msg);
CsrUint8 *CsrBtCmPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmPortnegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmPortnegResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM
CsrSize CsrBtCmReadLocalVersionCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalVersionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalVersionCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLocalVersionCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ
CsrSize CsrBtCmEnableDutModeReqSizeof(void *msg);
CsrUint8 *CsrBtCmEnableDutModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEnableDutModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmEnableDutModeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM
CsrSize CsrBtCmLeReceiverTestCfmSizeof(void *msg);
CsrUint8 *CsrBtCmLeReceiverTestCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeReceiverTestCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeReceiverTestCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ
CsrSize CsrBtCmCancelInquiryReqSizeof(void *msg);
CsrUint8 *CsrBtCmCancelInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmCancelInquiryReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmCancelInquiryReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ
CsrSize CsrBtCmSdcAttributeReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcAttributeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcAttributeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcAttributeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ
CsrSize CsrBtCmSdcCloseReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcCloseReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND
CsrSize CsrBtCmEncryptChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmEncryptChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEncryptChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmEncryptChangeIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ
CsrSize CsrBtCmIncomingScoReqSizeof(void *msg);
CsrUint8 *CsrBtCmIncomingScoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmIncomingScoReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmIncomingScoReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_IND
CsrSize CsrBtCmControlIndSizeof(void *msg);
CsrUint8 *CsrBtCmControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmControlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmControlIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ
CsrSize CsrBtCmReadEncryptionStatusReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadEncryptionStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadEncryptionStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadEncryptionStatusReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtCmReadEncryptionKeySizeReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadEncryptionKeySizeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND
CsrSize CsrBtCmRoleChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmRoleChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRoleChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRoleChangeIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_IND
CsrSize CsrBtCmL2caDataIndSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmL2caDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ
CsrSize CsrBtCmCancelAcceptConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmCancelAcceptConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmCancelAcceptConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmCancelAcceptConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ
CsrSize CsrBtCmCancelBnepConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmCancelBnepConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmCancelBnepConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmCancelBnepConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM
CsrSize CsrBtCmL2caGetChannelInfoCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caGetChannelInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caGetChannelInfoCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caGetChannelInfoCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM
CsrSize CsrBtCmSdsExtUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdsExtUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdsExtUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdsExtUnregisterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND
CsrSize CsrBtCmLeEventScanIndSizeof(void *msg);
CsrUint8 *CsrBtCmLeEventScanIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeEventScanIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeEventScanIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND
CsrSize CsrBtCmRfcModeChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmRfcModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRfcModeChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRfcModeChangeIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_REQ
CsrSize CsrBtCmDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDisconnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM
CsrSize CsrBtCmScoAcceptConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtCmScoAcceptConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoAcceptConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoAcceptConnectCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ
CsrSize CsrBtCmDmWriteLpSettingsReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmWriteLpSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmWriteLpSettingsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmWriteLpSettingsReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND
CsrSize CsrBtCmBnepSwitchRoleIndSizeof(void *msg);
CsrUint8 *CsrBtCmBnepSwitchRoleIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepSwitchRoleIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepSwitchRoleIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ
CsrSize CsrBtCmLeAdvertiseReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeAdvertiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeAdvertiseReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmLeAdvertiseReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_CFM
CsrSize CsrBtCmConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtCmConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmConnectCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ
CsrSize CsrBtCmReadLocalExtFeaturesReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalExtFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalExtFeaturesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLocalExtFeaturesReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ
CsrSize CsrBtCmSetEventMaskReqSizeof(void *msg);
CsrUint8 *CsrBtCmSetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetEventMaskReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetEventMaskReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ
CsrSize CsrBtCmReadFailedContactCounterReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadFailedContactCounterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadFailedContactCounterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadFailedContactCounterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND
CsrSize CsrBtCmBnepConnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmBnepConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepConnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND
CsrSize CsrBtCmMapScoPcmIndSizeof(void *msg);
CsrUint8 *CsrBtCmMapScoPcmIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmMapScoPcmIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmMapScoPcmIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM
CsrSize CsrBtCmGetLinkQualityCfmSizeof(void *msg);
CsrUint8 *CsrBtCmGetLinkQualityCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmGetLinkQualityCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmGetLinkQualityCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ
CsrSize CsrBtCmReadRemoteVersionReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteVersionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteVersionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRemoteVersionReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM
CsrSize CsrBtCmReadAfhChannelMapCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadAfhChannelMapCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadAfhChannelMapCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadAfhChannelMapCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_IND
CsrSize CsrBtCmPortnegIndSizeof(void *msg);
CsrUint8 *CsrBtCmPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmPortnegIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmPortnegIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM
CsrSize CsrBtCmRoleDiscoveryCfmSizeof(void *msg);
CsrUint8 *CsrBtCmRoleDiscoveryCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRoleDiscoveryCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRoleDiscoveryCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND
CsrSize CsrBtCmL2caConnectionlessDataIndSizeof(void *msg);
CsrUint8 *CsrBtCmL2caConnectionlessDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caConnectionlessDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmL2caConnectionlessDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ
CsrSize CsrBtCmLeReceiverTestReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeReceiverTestReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeReceiverTestReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeReceiverTestReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ
CsrSize CsrBtCmReadAdvertisingChTxPowerReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadAdvertisingChTxPowerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadAdvertisingChTxPowerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadAdvertisingChTxPowerReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ
CsrSize CsrBtCmModeChangeReqSizeof(void *msg);
CsrUint8 *CsrBtCmModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmModeChangeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmModeChangeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_RES
CsrSize CsrBtCmL2caDataResSizeof(void *msg);
CsrUint8 *CsrBtCmL2caDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caDataResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND
CsrSize CsrBtCmSyncDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmSyncDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSyncDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSyncDisconnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_CFM
CsrSize CsrBtCmReadCodCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadCodCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadCodCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadCodCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND
CsrSize CsrBtCmSniffSubRatingIndSizeof(void *msg);
CsrUint8 *CsrBtCmSniffSubRatingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSniffSubRatingIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSniffSubRatingIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM
CsrSize CsrBtCmLeConnparamCfmSizeof(void *msg);
CsrUint8 *CsrBtCmLeConnparamCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeConnparamCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeConnparamCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_REQ
CsrSize CsrBtCmLeScanReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeScanReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeScanReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeScanReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ
CsrSize CsrBtCmL2caUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caUnregisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND
CsrSize CsrBtCmGetSecurityConfIndSizeof(void *msg);
CsrUint8 *CsrBtCmGetSecurityConfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmGetSecurityConfIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmGetSecurityConfIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM
CsrSize CsrBtCmReadLocalExtFeaturesCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadLocalExtFeaturesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLocalExtFeaturesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLocalExtFeaturesCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ
CsrSize CsrBtCmLeConnparamUpdateReqSizeof(void *msg);
CsrUint8 *CsrBtCmLeConnparamUpdateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeConnparamUpdateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeConnparamUpdateReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ
CsrSize CsrBtCmAlwaysSupportMasterRoleReqSizeof(void *msg);
CsrUint8 *CsrBtCmAlwaysSupportMasterRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAlwaysSupportMasterRoleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAlwaysSupportMasterRoleReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ
CsrSize CsrBtCmReadAfhChannelAssessmentModeReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadAfhChannelAssessmentModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadAfhChannelAssessmentModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadAfhChannelAssessmentModeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ
CsrSize CsrBtCmSmReadLocalOobDataReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmReadLocalOobDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmReadLocalOobDataReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmReadLocalOobDataReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
CsrSize CsrBtCmSmSendKeypressNotificationReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmSendKeypressNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmSendKeypressNotificationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmSendKeypressNotificationReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND
CsrSize CsrBtCmConnectionRejSecurityIndSizeof(void *msg);
CsrUint8 *CsrBtCmConnectionRejSecurityIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmConnectionRejSecurityIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmConnectionRejSecurityIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ
CsrSize CsrBtCmDmUpdateAndClearCachedParamReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmUpdateAndClearCachedParamReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmUpdateAndClearCachedParamReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmUpdateAndClearCachedParamReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ
CsrSize CsrBtCmWriteScanEnableReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteScanEnableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteScanEnableReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteScanEnableReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ
CsrSize CsrBtCmScoAcceptConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmScoAcceptConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoAcceptConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoAcceptConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ
CsrSize CsrBtCmSmCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmCancelConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ
CsrSize CsrBtCmSdcReleaseResourcesReqSizeof(void *msg);
CsrUint8 *CsrBtCmSdcReleaseResourcesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcReleaseResourcesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcReleaseResourcesReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM
CsrSize CsrBtCmLeWhitelistSetCfmSizeof(void *msg);
CsrUint8 *CsrBtCmLeWhitelistSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeWhitelistSetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeWhitelistSetCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM
CsrSize CsrBtCmEirUpdateManufacturerDataCfmSizeof(void *msg);
CsrUint8 *CsrBtCmEirUpdateManufacturerDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEirUpdateManufacturerDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmEirUpdateManufacturerDataCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ
CsrSize CsrBtCmWritePageToReqSizeof(void *msg);
CsrUint8 *CsrBtCmWritePageToReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWritePageToReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWritePageToReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmBnepCancelConnectAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtCmBnepCancelConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepCancelConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepCancelConnectAcceptReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ
CsrSize CsrBtCmSmSetDefaultSecLevelReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmSetDefaultSecLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmSetDefaultSecLevelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmSetDefaultSecLevelReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ
CsrSize CsrBtCmDmL2caModeSettingsReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmL2caModeSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmL2caModeSettingsReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmDmL2caModeSettingsReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND
CsrSize CsrBtCmLeConnparamUpdateCmpIndSizeof(void *msg);
CsrUint8 *CsrBtCmLeConnparamUpdateCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeConnparamUpdateCmpIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeConnparamUpdateCmpIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM
CsrSize CsrBtCmReadTxPowerLevelCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadTxPowerLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadTxPowerLevelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadTxPowerLevelCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM
CsrSize CsrBtCmSdsUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdsUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdsUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdsUnregisterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmConnectAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtCmConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmConnectAcceptCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ
CsrSize CsrBtCmClearEventFilterReqSizeof(void *msg);
CsrUint8 *CsrBtCmClearEventFilterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmClearEventFilterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmClearEventFilterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND
CsrSize CsrBtCmInquiryResultIndSizeof(void *msg);
CsrUint8 *CsrBtCmInquiryResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmInquiryResultIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmInquiryResultIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ
CsrSize CsrBtCmDmBnepModeSettingsReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmBnepModeSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmBnepModeSettingsReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmDmBnepModeSettingsReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ
CsrSize CsrBtCmSmSecModeConfigReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmSecModeConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmSecModeConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmSecModeConfigReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ
CsrSize CsrBtCmL2caRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmL2caRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caRegisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM
CsrSize CsrBtCmSdsRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdsRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdsRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdsRegisterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_REQ
CsrSize CsrBtCmReadRssiReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadRssiReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRssiReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRssiReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ
CsrSize CsrBtCmReadLinkPolicyReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadLinkPolicyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadLinkPolicyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadLinkPolicyReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ
CsrSize CsrBtCmSmSetSecModeReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmSetSecModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmSetSecModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmSetSecModeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_CFM
CsrSize CsrBtCmReadRssiCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadRssiCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRssiCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRssiCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ
CsrSize CsrBtCmSetEventFilterCodReqSizeof(void *msg);
CsrUint8 *CsrBtCmSetEventFilterCodReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetEventFilterCodReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetEventFilterCodReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_RES
CsrSize CsrBtCmSmRepairResSizeof(void *msg);
CsrUint8 *CsrBtCmSmRepairResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmRepairResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmRepairResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_IND
CsrSize CsrBtCmSmRepairIndSizeof(void *msg);
CsrUint8 *CsrBtCmSmRepairIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmRepairIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmRepairIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_IND */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ
CsrSize CsrBtCmCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtCmCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmCancelConnectReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM
CsrSize CsrBtCmCancelAcceptConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtCmCancelAcceptConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmCancelAcceptConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmCancelAcceptConnectCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_REQ
CsrSize CsrBtCmReadCodReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadCodReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadCodReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadCodReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES
CsrSize CsrBtCmGetSecurityConfResSizeof(void *msg);
CsrUint8 *CsrBtCmGetSecurityConfResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmGetSecurityConfResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmGetSecurityConfResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ
CsrSize CsrBtCmBnepSwitchRoleReqSizeof(void *msg);
CsrUint8 *CsrBtCmBnepSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepSwitchRoleReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_REPORT_IND
CsrSize CsrBtCmLeReportIndSizeof(void *msg);
CsrUint8 *CsrBtCmLeReportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeReportIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeReportIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ
CsrSize CsrBtCmConnectableReqSizeof(void *msg);
CsrUint8 *CsrBtCmConnectableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmConnectableReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmConnectableReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES
CsrSize CsrBtCmSmIoCapabilityRequestNegResSizeof(void *msg);
CsrUint8 *CsrBtCmSmIoCapabilityRequestNegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmIoCapabilityRequestNegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmIoCapabilityRequestNegResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING
CsrSize CsrBtCmDmHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtCmDmHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDmHouseCleaningSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ
CsrSize CsrBtCmAlwaysMasterDevicesReqSizeof(void *msg);
CsrUint8 *CsrBtCmAlwaysMasterDevicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAlwaysMasterDevicesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAlwaysMasterDevicesReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ
CsrSize CsrBtCmMoveChannelReqSizeof(void *msg);
CsrUint8 *CsrBtCmMoveChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmMoveChannelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmMoveChannelReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM
CsrSize CsrBtCmClearEventFilterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmClearEventFilterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmClearEventFilterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmClearEventFilterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_REQ
CsrSize CsrBtCmPortnegReqSizeof(void *msg);
CsrUint8 *CsrBtCmPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmPortnegReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmPortnegReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND
CsrSize CsrBtCmLeAcceptConnparamUpdateIndSizeof(void *msg);
CsrUint8 *CsrBtCmLeAcceptConnparamUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeAcceptConnparamUpdateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeAcceptConnparamUpdateIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ
CsrSize CsrBtCmWritePagescanTypeReqSizeof(void *msg);
CsrUint8 *CsrBtCmWritePagescanTypeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWritePagescanTypeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWritePagescanTypeReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM
CsrSize CsrBtCmSdcReleaseResourcesCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdcReleaseResourcesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcReleaseResourcesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcReleaseResourcesCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM
CsrSize CsrBtCmEnableDutModeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmEnableDutModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEnableDutModeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmEnableDutModeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM
CsrSize CsrBtCmAclDetachCfmSizeof(void *msg);
CsrUint8 *CsrBtCmAclDetachCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmAclDetachCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmAclDetachCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ
CsrSize CsrBtCmWriteLinkSupervTimeoutReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteLinkSupervTimeoutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteLinkSupervTimeoutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteLinkSupervTimeoutReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_REQ
CsrSize CsrBtCmDataReqSizeof(void *msg);
CsrUint8 *CsrBtCmDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM
CsrSize CsrBtCmDisableDutModeCfmSizeof(void *msg);
CsrUint8 *CsrBtCmDisableDutModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDisableDutModeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDisableDutModeCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ
CsrSize CsrBtCmReadAfhChannelMapReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadAfhChannelMapReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadAfhChannelMapReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadAfhChannelMapReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_CFM
CsrSize CsrBtCmReadIacCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadIacCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadIacCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadIacCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_IND
CsrSize CsrBtCmDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmDisconnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND
CsrSize CsrBtCmSyncRenegotiateIndSizeof(void *msg);
CsrUint8 *CsrBtCmSyncRenegotiateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSyncRenegotiateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSyncRenegotiateIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM
CsrSize CsrBtCmSdcServiceSearchCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSdcServiceSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcServiceSearchCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSdcServiceSearchCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ
CsrSize CsrBtCmDmModeSettingsReqSizeof(void *msg);
CsrUint8 *CsrBtCmDmModeSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmDmModeSettingsReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmDmModeSettingsReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_CFM
CsrSize CsrBtCmLeScanCfmSizeof(void *msg);
CsrUint8 *CsrBtCmLeScanCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeScanCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeScanCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND
CsrSize CsrBtCmBnepExtendedDataIndSizeof(void *msg);
CsrUint8 *CsrBtCmBnepExtendedDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepExtendedDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmBnepExtendedDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM
CsrSize CsrBtCmL2caRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtCmL2caRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caRegisterCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND
CsrSize CsrBtCmL2caModeChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmL2caModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmL2caModeChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmL2caModeChangeIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ
CsrSize CsrBtCmSmRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmRegisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM
CsrSize CsrBtCmLeTestEndCfmSizeof(void *msg);
CsrUint8 *CsrBtCmLeTestEndCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeTestEndCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeTestEndCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ
CsrSize CsrBtCmReadRemoteExtFeaturesReqSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteExtFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteExtFeaturesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRemoteExtFeaturesReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM
CsrSize CsrBtCmReadRemoteExtFeaturesCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteExtFeaturesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteExtFeaturesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadRemoteExtFeaturesCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND
CsrSize CsrBtCmBnepModeChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmBnepModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepModeChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepModeChangeIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES
CsrSize CsrBtCmSmAuthoriseResSizeof(void *msg);
CsrUint8 *CsrBtCmSmAuthoriseResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmAuthoriseResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmAuthoriseResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_IND
CsrSize CsrBtCmWriteIacIndSizeof(void *msg);
CsrUint8 *CsrBtCmWriteIacIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteIacIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteIacIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmBnepCancelConnectAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtCmBnepCancelConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepCancelConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepCancelConnectAcceptCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM
CsrSize CsrBtCmLeTransmitterTestCfmSizeof(void *msg);
CsrUint8 *CsrBtCmLeTransmitterTestCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLeTransmitterTestCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLeTransmitterTestCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM
CsrSize CsrBtCmReadRemoteNameCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadRemoteNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadRemoteNameCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmReadRemoteNameCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND
CsrSize CsrBtCmWriteLinkPolicyErrorIndSizeof(void *msg);
CsrUint8 *CsrBtCmWriteLinkPolicyErrorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteLinkPolicyErrorIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteLinkPolicyErrorIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES
CsrSize CsrBtCmSmKeyRequestResSizeof(void *msg);
CsrUint8 *CsrBtCmSmKeyRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmKeyRequestResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtCmSmKeyRequestResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM
CsrSize CsrBtCmReadEncryptionStatusCfmSizeof(void *msg);
CsrUint8 *CsrBtCmReadEncryptionStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmReadEncryptionStatusCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmReadEncryptionStatusCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ
CsrSize CsrBtCmSmAclCloseReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmAclCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmAclCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmAclCloseReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ
CsrSize CsrBtCmSmAccessReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmAccessReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmAccessReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmAccessReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ
CsrSize CsrBtCmA2dpBitRateReqSizeof(void *msg);
CsrUint8 *CsrBtCmA2dpBitRateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmA2dpBitRateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmA2dpBitRateReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmBnepConnectAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtCmBnepConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepConnectAcceptCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM
CsrSize CsrBtCmSetLocalNameCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSetLocalNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetLocalNameCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetLocalNameCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND
CsrSize CsrBtCmSdcCloseIndSizeof(void *msg);
CsrUint8 *CsrBtCmSdcCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSdcCloseIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSdcCloseIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES
CsrSize CsrBtCmSmUserConfirmationRequestResSizeof(void *msg);
CsrUint8 *CsrBtCmSmUserConfirmationRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmUserConfirmationRequestResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmUserConfirmationRequestResSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND
CsrSize CsrBtCmRejectRfcConnectionIndSizeof(void *msg);
CsrUint8 *CsrBtCmRejectRfcConnectionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRejectRfcConnectionIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRejectRfcConnectionIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ
CsrSize CsrBtCmWriteIacReqSizeof(void *msg);
CsrUint8 *CsrBtCmWriteIacReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmWriteIacReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmWriteIacReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND
CsrSize CsrBtCmBleConnectionIndSizeof(void *msg);
CsrUint8 *CsrBtCmBleConnectionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBleConnectionIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBleConnectionIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND
CsrSize CsrBtCmExtSyncConnectIndSizeof(void *msg);
CsrUint8 *CsrBtCmExtSyncConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmExtSyncConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmExtSyncConnectIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ
CsrSize CsrBtCmBnepRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtCmBnepRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmBnepRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmBnepRegisterReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM
CsrSize CsrBtCmSetEventFilterBdaddrCfmSizeof(void *msg);
CsrUint8 *CsrBtCmSetEventFilterBdaddrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSetEventFilterBdaddrCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSetEventFilterBdaddrCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ
CsrSize CsrBtCmEirFlagsReqSizeof(void *msg);
CsrUint8 *CsrBtCmEirFlagsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmEirFlagsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmEirFlagsReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ
CsrSize CsrBtCmSmRemoveDeviceReqSizeof(void *msg);
CsrUint8 *CsrBtCmSmRemoveDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmSmRemoveDeviceReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmSmRemoveDeviceReqSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND
CsrSize CsrBtCmLstoChangeIndSizeof(void *msg);
CsrUint8 *CsrBtCmLstoChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmLstoChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmLstoChangeIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND
CsrSize CsrBtCmMoveChannelIndSizeof(void *msg);
CsrUint8 *CsrBtCmMoveChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmMoveChannelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmMoveChannelIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM
CsrSize CsrBtCmScoConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtCmScoConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmScoConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmScoConnectCfmSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND
CsrSize CsrBtCmRemoteFeaturesIndSizeof(void *msg);
CsrUint8 *CsrBtCmRemoteFeaturesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmRemoteFeaturesIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmRemoteFeaturesIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND
CsrSize CsrBtCmHighPriorityDataIndSizeof(void *msg);
CsrUint8 *CsrBtCmHighPriorityDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtCmHighPriorityDataIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtCmHighPriorityDataIndSerFree CsrBtCmPfree
#endif /* EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND */

#ifdef __cplusplus
}
#endif

#endif
