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
#include "csr_bt_sc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SC_MODULE
#include "csr_bt_sc_serialize.h"
#include "csr_bt_sc_private_prim.h"

static CsrMsgConvMsgEntry csr_bt_sc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ
    { CSR_BT_SC_ACCEPT_BOND_REQ, CsrBtScAcceptBondReqSizeof, CsrBtScAcceptBondReqSer, CsrBtScAcceptBondReqDes, CsrBtScAcceptBondReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ */
#ifndef EXCLUDE_CSR_BT_SC_ACTIVATE_REQ
    { CSR_BT_SC_ACTIVATE_REQ, CsrBtScActivateReqSizeof, CsrBtScActivateReqSer, CsrBtScActivateReqDes, CsrBtScActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ
    { CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ, CsrBtScAddRemoteOobDataReqSizeof, CsrBtScAddRemoteOobDataReqSer, CsrBtScAddRemoteOobDataReqDes, CsrBtScAddRemoteOobDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM
    { CSR_BT_SC_AUTHENTICATE_CFM, CsrBtScAuthenticateCfmSizeof, CsrBtScAuthenticateCfmSer, CsrBtScAuthenticateCfmDes, CsrBtScAuthenticateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM */
#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ
    { CSR_BT_SC_AUTHENTICATE_REQ, CsrBtScAuthenticateReqSizeof, CsrBtScAuthenticateReqSer, CsrBtScAuthenticateReqDes, CsrBtScAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND
    { CSR_BT_SC_AUTHORISE_EXT_IND, CsrBtScAuthoriseExtIndSizeof, CsrBtScAuthoriseExtIndSer, CsrBtScAuthoriseExtIndDes, CsrBtScAuthoriseExtIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND */
#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_IND
    { CSR_BT_SC_AUTHORISE_IND, CsrBtScAuthoriseIndSizeof, CsrBtScAuthoriseIndSer, CsrBtScAuthoriseIndDes, CsrBtScAuthoriseIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_IND */
#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_RES
    { CSR_BT_SC_AUTHORISE_RES, CsrBtScAuthoriseResSizeof, CsrBtScAuthoriseResSer, CsrBtScAuthoriseResDes, CsrBtScAuthoriseResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_RES */
#ifndef EXCLUDE_CSR_BT_SC_BOND_CFM
    { CSR_BT_SC_BOND_CFM, CsrBtScBondCfmSizeof, CsrBtScBondCfmSer, CsrBtScBondCfmDes, CsrBtScBondCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_BOND_CFM */
#ifndef EXCLUDE_CSR_BT_SC_BOND_IND
    { CSR_BT_SC_BOND_IND, CsrBtScBondIndSizeof, CsrBtScBondIndSer, CsrBtScBondIndDes, CsrBtScBondIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_BOND_IND */
#ifndef EXCLUDE_CSR_BT_SC_BOND_REQ
    { CSR_BT_SC_BOND_REQ, CsrBtScBondReqSizeof, CsrBtScBondReqSer, CsrBtScBondReqDes, CsrBtScBondReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_BOND_REQ */
#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND
    { CSR_BT_SC_CANCEL_ACCEPT_BOND_IND, CsrBtScCancelAcceptBondIndSizeof, CsrBtScCancelAcceptBondIndSer, CsrBtScCancelAcceptBondIndDes, CsrBtScCancelAcceptBondIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND */
#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ
    { CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ, CsrBtScCancelAcceptBondReqSizeof, CsrBtScCancelAcceptBondReqSer, CsrBtScCancelAcceptBondReqDes, CsrBtScCancelAcceptBondReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ */
#ifndef EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ
    { CSR_BT_SC_CANCEL_BOND_REQ, CsrBtScCancelBondReqSizeof, CsrBtScCancelBondReqSer, CsrBtScCancelBondReqDes, CsrBtScCancelBondReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ */
#ifndef EXCLUDE_CSR_BT_SC_CONFIG_REQ
    { CSR_BT_SC_CONFIG_REQ, CsrBtScConfigReqSizeof, CsrBtScConfigReqSer, CsrBtScConfigReqDes, CsrBtScConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_SC_DATABASE_CFM
    { CSR_BT_SC_DATABASE_CFM, CsrBtScDatabaseCfmSizeof, CsrBtScDatabaseCfmSer, CsrBtScDatabaseCfmDes, CsrBtScDatabaseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_CFM */
#ifndef EXCLUDE_CSR_BT_SC_DATABASE_REQ
    { CSR_BT_SC_DATABASE_REQ, CsrBtScDatabaseReqSizeof, CsrBtScDatabaseReqSer, CsrBtScDatabaseReqDes, CsrBtScDatabaseReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_DEBOND_CFM
    { CSR_BT_SC_DEBOND_CFM, CsrBtScDebondCfmSizeof, CsrBtScDebondCfmSer, CsrBtScDebondCfmDes, CsrBtScDebondCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_CFM */
#ifndef EXCLUDE_CSR_BT_SC_DEBOND_REQ
    { CSR_BT_SC_DEBOND_REQ, CsrBtScDebondReqSizeof, CsrBtScDebondReqSer, CsrBtScDebondReqDes, CsrBtScDebondReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_REQ */
#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM
    { CSR_BT_SC_DEBUG_MODE_CFM, CsrBtScDebugModeCfmSizeof, CsrBtScDebugModeCfmSer, CsrBtScDebugModeCfmDes, CsrBtScDebugModeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM */
#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ
    { CSR_BT_SC_DEBUG_MODE_REQ, CsrBtScDebugModeReqSizeof, CsrBtScDebugModeReqSer, CsrBtScDebugModeReqDes, CsrBtScDebugModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_DEREGISTER_REQ
    { CSR_BT_SC_DEREGISTER_REQ, CsrBtScDeregisterReqSizeof, CsrBtScDeregisterReqSer, CsrBtScDeregisterReqDes, CsrBtScDeregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_DEREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM
    { CSR_BT_SC_ENCRYPTION_CFM, CsrBtScEncryptionCfmSizeof, CsrBtScEncryptionCfmSer, CsrBtScEncryptionCfmDes, CsrBtScEncryptionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM */
#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM
    { CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM, CsrBtScEncryptionKeySizeCfmSizeof, CsrBtScEncryptionKeySizeCfmSer, CsrBtScEncryptionKeySizeCfmDes, CsrBtScEncryptionKeySizeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM */
#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ
    { CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ, CsrBtScEncryptionKeySizeReqSizeof, CsrBtScEncryptionKeySizeReqSer, CsrBtScEncryptionKeySizeReqDes, CsrBtScEncryptionKeySizeReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ
    { CSR_BT_SC_ENCRYPTION_REQ, CsrBtScEncryptionReqSizeof, CsrBtScEncryptionReqSer, CsrBtScEncryptionReqDes, CsrBtScEncryptionReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ */
#ifndef EXCLUDE_CSR_BT_SC_HOUSE_CLEANING
    { CSR_BT_SC_HOUSE_CLEANING, CsrBtScHouseCleaningSizeof, CsrBtScHouseCleaningSer, CsrBtScHouseCleaningDes, CsrBtScHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_SC_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ
    { CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ, CsrBtScLeAddServiceChangedReqSizeof, CsrBtScLeAddServiceChangedReqSer, CsrBtScLeAddServiceChangedReqDes, CsrBtScLeAddServiceChangedReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ */
#ifndef EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ
    { CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ, CsrBtScLeKeyDistributionReqSizeof, CsrBtScLeKeyDistributionReqSer, CsrBtScLeKeyDistributionReqDes, CsrBtScLeKeyDistributionReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ */
#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM
    { CSR_BT_SC_LE_SECURITY_CFM, CsrBtScLeSecurityCfmSizeof, CsrBtScLeSecurityCfmSer, CsrBtScLeSecurityCfmDes, CsrBtScLeSecurityCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM */
#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_IND
    { CSR_BT_SC_LE_SECURITY_IND, CsrBtScLeSecurityIndSizeof, CsrBtScLeSecurityIndSer, CsrBtScLeSecurityIndDes, CsrBtScLeSecurityIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_IND */
#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ
    { CSR_BT_SC_LE_SECURITY_REQ, CsrBtScLeSecurityReqSizeof, CsrBtScLeSecurityReqSer, CsrBtScLeSecurityReqDes, CsrBtScLeSecurityReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ */
#ifndef EXCLUDE_CSR_BT_SC_MODE_IND
    { CSR_BT_SC_MODE_IND, CsrBtScModeIndSizeof, CsrBtScModeIndSer, CsrBtScModeIndDes, CsrBtScModeIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_MODE_IND */
#ifndef EXCLUDE_CSR_BT_SC_MODE_REQ
    { CSR_BT_SC_MODE_REQ, CsrBtScModeReqSizeof, CsrBtScModeReqSer, CsrBtScModeReqDes, CsrBtScModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_IND
    { CSR_BT_SC_PASSKEY_IND, CsrBtScPasskeyIndSizeof, CsrBtScPasskeyIndSer, CsrBtScPasskeyIndDes, CsrBtScPasskeyIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_IND */
#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_RES
    { CSR_BT_SC_PASSKEY_RES, CsrBtScPasskeyResSizeof, CsrBtScPasskeyResSer, CsrBtScPasskeyResDes, CsrBtScPasskeyResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_RES */
#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM
    { CSR_BT_SC_READ_DEVICE_RECORD_CFM, CsrBtScReadDeviceRecordCfmSizeof, CsrBtScReadDeviceRecordCfmSer, CsrBtScReadDeviceRecordCfmDes, CsrBtScReadDeviceRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND
    { CSR_BT_SC_READ_DEVICE_RECORD_IND, CsrBtScReadDeviceRecordIndSizeof, CsrBtScReadDeviceRecordIndSer, CsrBtScReadDeviceRecordIndDes, CsrBtScReadDeviceRecordIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND */
#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ
    { CSR_BT_SC_READ_DEVICE_RECORD_REQ, CsrBtScReadDeviceRecordReqSizeof, CsrBtScReadDeviceRecordReqSer, CsrBtScReadDeviceRecordReqDes, CsrBtScReadDeviceRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM
    { CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM, CsrBtScReadLocalOobDataCfmSizeof, CsrBtScReadLocalOobDataCfmSer, CsrBtScReadLocalOobDataCfmDes, CsrBtScReadLocalOobDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ
    { CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ, CsrBtScReadLocalOobDataReqSizeof, CsrBtScReadLocalOobDataReqSer, CsrBtScReadLocalOobDataReqDes, CsrBtScReadLocalOobDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_SC_REBOND_IND
    { CSR_BT_SC_REBOND_IND, CsrBtScRebondIndSizeof, CsrBtScRebondIndSer, CsrBtScRebondIndDes, CsrBtScRebondIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_REBOND_IND */
#ifndef EXCLUDE_CSR_BT_SC_REBOND_RES
    { CSR_BT_SC_REBOND_RES, CsrBtScRebondResSizeof, CsrBtScRebondResSer, CsrBtScRebondResDes, CsrBtScRebondResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_REBOND_RES */
#ifndef EXCLUDE_CSR_BT_SC_REGISTER_REQ
    { CSR_BT_SC_REGISTER_REQ, CsrBtScRegisterReqSizeof, CsrBtScRegisterReqSer, CsrBtScRegisterReqDes, CsrBtScRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ
    { CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ, CsrBtScSetAuthRequirementsReqSizeof, CsrBtScSetAuthRequirementsReqSer, CsrBtScSetAuthRequirementsReqDes, CsrBtScSetAuthRequirementsReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ
    { CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ, CsrBtScSetEncryptionKeySizeReqSizeof, CsrBtScSetEncryptionKeySizeReqSer, CsrBtScSetEncryptionKeySizeReqDes, CsrBtScSetEncryptionKeySizeReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ
    { CSR_BT_SC_SET_EVENT_MASK_REQ, CsrBtScSetEventMaskReqSizeof, CsrBtScSetEventMaskReqSer, CsrBtScSetEventMaskReqDes, CsrBtScSetEventMaskReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ
    { CSR_BT_SC_SET_IO_CAPABILITY_REQ, CsrBtScSetIoCapabilityReqSizeof, CsrBtScSetIoCapabilityReqSer, CsrBtScSetIoCapabilityReqDes, CsrBtScSetIoCapabilityReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM
    { CSR_BT_SC_SET_SECURITY_MODE_CFM, CsrBtScSetSecurityModeCfmSizeof, CsrBtScSetSecurityModeCfmSer, CsrBtScSetSecurityModeCfmDes, CsrBtScSetSecurityModeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM */
#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ
    { CSR_BT_SC_SET_SECURITY_MODE_REQ, CsrBtScSetSecurityModeReqSizeof, CsrBtScSetSecurityModeReqSer, CsrBtScSetSecurityModeReqDes, CsrBtScSetSecurityModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ
    { CSR_BT_SC_SET_TRUST_LEVEL_REQ, CsrBtScSetTrustLevelReqSizeof, CsrBtScSetTrustLevelReqSer, CsrBtScSetTrustLevelReqDes, CsrBtScSetTrustLevelReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND
    { CSR_BT_SC_SSP_COMPARE_IND, CsrBtScSspCompareIndSizeof, CsrBtScSspCompareIndSer, CsrBtScSspCompareIndDes, CsrBtScSspCompareIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES
    { CSR_BT_SC_SSP_COMPARE_RES, CsrBtScSspCompareResSizeof, CsrBtScSspCompareResSer, CsrBtScSspCompareResDes, CsrBtScSspCompareResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES */
#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND
    { CSR_BT_SC_SSP_JUSTWORKS_IND, CsrBtScSspJustworksIndSizeof, CsrBtScSspJustworksIndSer, CsrBtScSspJustworksIndDes, CsrBtScSspJustworksIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES
    { CSR_BT_SC_SSP_JUSTWORKS_RES, CsrBtScSspJustworksResSizeof, CsrBtScSspJustworksResSer, CsrBtScSspJustworksResDes, CsrBtScSspJustworksResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES */
#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND
    { CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND, CsrBtScSspKeypressNotificationIndSizeof, CsrBtScSspKeypressNotificationIndSer, CsrBtScSspKeypressNotificationIndDes, CsrBtScSspKeypressNotificationIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ
    { CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ, CsrBtScSspKeypressNotificationReqSizeof, CsrBtScSspKeypressNotificationReqSer, CsrBtScSspKeypressNotificationReqDes, CsrBtScSspKeypressNotificationReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ */
#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND
    { CSR_BT_SC_SSP_NOTIFICATION_IND, CsrBtScSspNotificationIndSizeof, CsrBtScSspNotificationIndSer, CsrBtScSspNotificationIndDes, CsrBtScSspNotificationIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES
    { CSR_BT_SC_SSP_NOTIFICATION_RES, CsrBtScSspNotificationResSizeof, CsrBtScSspNotificationResSer, CsrBtScSspNotificationResDes, CsrBtScSspNotificationResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES */
#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND
    { CSR_BT_SC_SSP_PAIRING_IND, CsrBtScSspPairingIndSizeof, CsrBtScSspPairingIndSer, CsrBtScSspPairingIndDes, CsrBtScSspPairingIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES
    { CSR_BT_SC_SSP_PAIRING_RES, CsrBtScSspPairingResSizeof, CsrBtScSspPairingResSer, CsrBtScSspPairingResDes, CsrBtScSspPairingResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES */
#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND
    { CSR_BT_SC_SSP_PASSKEY_IND, CsrBtScSspPasskeyIndSizeof, CsrBtScSspPasskeyIndSer, CsrBtScSspPasskeyIndDes, CsrBtScSspPasskeyIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES
    { CSR_BT_SC_SSP_PASSKEY_RES, CsrBtScSspPasskeyResSizeof, CsrBtScSspPasskeyResSer, CsrBtScSspPasskeyResDes, CsrBtScSspPasskeyResSerFree },
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES */
#ifndef EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ
    { CSR_BT_SC_UPDATE_DEVICE_REQ, CsrBtScUpdateDeviceReqSizeof, CsrBtScUpdateDeviceReqSer, CsrBtScUpdateDeviceReqDes, CsrBtScUpdateDeviceReqSerFree },
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ */
#ifndef EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND
    { CSR_BT_SC_UPDATE_TRUST_LEVEL_IND, CsrBtScUpdateTrustLevelIndSizeof, CsrBtScUpdateTrustLevelIndSer, CsrBtScUpdateTrustLevelIndDes, CsrBtScUpdateTrustLevelIndSerFree },
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtScConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SC_PRIM, csr_bt_sc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_sc_conv_info = {
    CSR_BT_SC_PRIM,
    "CSR_BT_SC_PRIM",
    csr_bt_sc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtScTechInfoGet(void)
{
    return &csr_bt_sc_conv_info;
}
#endif
#endif

