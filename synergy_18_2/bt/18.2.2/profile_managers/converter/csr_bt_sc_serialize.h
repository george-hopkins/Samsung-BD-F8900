#ifndef CSR_BT_SC_SERIALIZE_H__
#define CSR_BT_SC_SERIALIZE_H__
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

void CsrBtScPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES
CsrSize CsrBtScSspPairingResSizeof(void *msg);
CsrUint8 *CsrBtScSspPairingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspPairingResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSspPairingResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND
CsrSize CsrBtScCancelAcceptBondIndSizeof(void *msg);
CsrUint8 *CsrBtScCancelAcceptBondIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScCancelAcceptBondIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScCancelAcceptBondIndSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM
CsrSize CsrBtScLeSecurityCfmSizeof(void *msg);
CsrUint8 *CsrBtScLeSecurityCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScLeSecurityCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScLeSecurityCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ
CsrSize CsrBtScReadLocalOobDataReqSizeof(void *msg);
CsrUint8 *CsrBtScReadLocalOobDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScReadLocalOobDataReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScReadLocalOobDataReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_RES
CsrSize CsrBtScPasskeyResSizeof(void *msg);
CsrUint8 *CsrBtScPasskeyResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScPasskeyResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScPasskeyResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM
CsrSize CsrBtScSetSecurityModeCfmSizeof(void *msg);
CsrUint8 *CsrBtScSetSecurityModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSetSecurityModeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSetSecurityModeCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_BOND_CFM
CsrSize CsrBtScBondCfmSizeof(void *msg);
CsrUint8 *CsrBtScBondCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScBondCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScBondCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_BOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND
CsrSize CsrBtScSspKeypressNotificationIndSizeof(void *msg);
CsrUint8 *CsrBtScSspKeypressNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspKeypressNotificationIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSspKeypressNotificationIndSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES
CsrSize CsrBtScSspCompareResSizeof(void *msg);
CsrUint8 *CsrBtScSspCompareResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspCompareResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSspCompareResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND
CsrSize CsrBtScSspCompareIndSizeof(void *msg);
CsrUint8 *CsrBtScSspCompareIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspCompareIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScSspCompareIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ
CsrSize CsrBtScReadDeviceRecordReqSizeof(void *msg);
CsrUint8 *CsrBtScReadDeviceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScReadDeviceRecordReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScReadDeviceRecordReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM
CsrSize CsrBtScReadLocalOobDataCfmSizeof(void *msg);
CsrUint8 *CsrBtScReadLocalOobDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScReadLocalOobDataCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScReadLocalOobDataCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM
CsrSize CsrBtScReadDeviceRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtScReadDeviceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScReadDeviceRecordCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScReadDeviceRecordCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ
CsrSize CsrBtScSetAuthRequirementsReqSizeof(void *msg);
CsrUint8 *CsrBtScSetAuthRequirementsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSetAuthRequirementsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSetAuthRequirementsReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ
CsrSize CsrBtScAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtScAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScAuthenticateReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND
CsrSize CsrBtScSspNotificationIndSizeof(void *msg);
CsrUint8 *CsrBtScSspNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspNotificationIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScSspNotificationIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_MODE_REQ
CsrSize CsrBtScModeReqSizeof(void *msg);
CsrUint8 *CsrBtScModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScModeReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ
CsrSize CsrBtScCancelAcceptBondReqSizeof(void *msg);
CsrUint8 *CsrBtScCancelAcceptBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScCancelAcceptBondReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScCancelAcceptBondReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES
CsrSize CsrBtScSspPasskeyResSizeof(void *msg);
CsrUint8 *CsrBtScSspPasskeyResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspPasskeyResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSspPasskeyResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ
CsrSize CsrBtScSetSecurityModeReqSizeof(void *msg);
CsrUint8 *CsrBtScSetSecurityModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSetSecurityModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSetSecurityModeReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_BOND_IND
CsrSize CsrBtScBondIndSizeof(void *msg);
CsrUint8 *CsrBtScBondIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScBondIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScBondIndSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtScEncryptionKeySizeReqSizeof(void *msg);
CsrUint8 *CsrBtScEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScEncryptionKeySizeReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ
CsrSize CsrBtScSetIoCapabilityReqSizeof(void *msg);
CsrUint8 *CsrBtScSetIoCapabilityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSetIoCapabilityReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSetIoCapabilityReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ
CsrSize CsrBtScLeSecurityReqSizeof(void *msg);
CsrUint8 *CsrBtScLeSecurityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScLeSecurityReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScLeSecurityReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ
CsrSize CsrBtScDebugModeReqSizeof(void *msg);
CsrUint8 *CsrBtScDebugModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScDebugModeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScDebugModeReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM
CsrSize CsrBtScEncryptionCfmSizeof(void *msg);
CsrUint8 *CsrBtScEncryptionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScEncryptionCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScEncryptionCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ
CsrSize CsrBtScSspKeypressNotificationReqSizeof(void *msg);
CsrUint8 *CsrBtScSspKeypressNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspKeypressNotificationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSspKeypressNotificationReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ
CsrSize CsrBtScAcceptBondReqSizeof(void *msg);
CsrUint8 *CsrBtScAcceptBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScAcceptBondReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScAcceptBondReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND
CsrSize CsrBtScSspPasskeyIndSizeof(void *msg);
CsrUint8 *CsrBtScSspPasskeyIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspPasskeyIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScSspPasskeyIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_IND
CsrSize CsrBtScLeSecurityIndSizeof(void *msg);
CsrUint8 *CsrBtScLeSecurityIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScLeSecurityIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScLeSecurityIndSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ
CsrSize CsrBtScLeAddServiceChangedReqSizeof(void *msg);
CsrUint8 *CsrBtScLeAddServiceChangedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScLeAddServiceChangedReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScLeAddServiceChangedReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ */

#ifndef EXCLUDE_CSR_BT_SC_REGISTER_REQ
CsrSize CsrBtScRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtScRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScRegisterReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACTIVATE_REQ
CsrSize CsrBtScActivateReqSizeof(void *msg);
CsrUint8 *CsrBtScActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScActivateReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ
CsrSize CsrBtScEncryptionReqSizeof(void *msg);
CsrUint8 *CsrBtScEncryptionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScEncryptionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScEncryptionReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES
CsrSize CsrBtScSspJustworksResSizeof(void *msg);
CsrUint8 *CsrBtScSspJustworksResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspJustworksResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSspJustworksResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_RES
CsrSize CsrBtScRebondResSizeof(void *msg);
CsrUint8 *CsrBtScRebondResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScRebondResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScRebondResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_REBOND_RES */

#ifndef EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ
CsrSize CsrBtScLeKeyDistributionReqSizeof(void *msg);
CsrUint8 *CsrBtScLeKeyDistributionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScLeKeyDistributionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScLeKeyDistributionReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM
CsrSize CsrBtScAuthenticateCfmSizeof(void *msg);
CsrUint8 *CsrBtScAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScAuthenticateCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND
CsrSize CsrBtScSspJustworksIndSizeof(void *msg);
CsrUint8 *CsrBtScSspJustworksIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspJustworksIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScSspJustworksIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND */

#ifndef EXCLUDE_CSR_BT_SC_BOND_REQ
CsrSize CsrBtScBondReqSizeof(void *msg);
CsrUint8 *CsrBtScBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScBondReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScBondReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_IND
CsrSize CsrBtScPasskeyIndSizeof(void *msg);
CsrUint8 *CsrBtScPasskeyIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScPasskeyIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScPasskeyIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_CFM
CsrSize CsrBtScDatabaseCfmSizeof(void *msg);
CsrUint8 *CsrBtScDatabaseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScDatabaseCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScDatabaseCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ
CsrSize CsrBtScSetTrustLevelReqSizeof(void *msg);
CsrUint8 *CsrBtScSetTrustLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSetTrustLevelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSetTrustLevelReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ
CsrSize CsrBtScSetEventMaskReqSizeof(void *msg);
CsrUint8 *CsrBtScSetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSetEventMaskReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSetEventMaskReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_REQ
CsrSize CsrBtScDatabaseReqSizeof(void *msg);
CsrUint8 *CsrBtScDatabaseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScDatabaseReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScDatabaseReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND
CsrSize CsrBtScSspPairingIndSizeof(void *msg);
CsrUint8 *CsrBtScSspPairingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspPairingIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScSspPairingIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM
CsrSize CsrBtScDebugModeCfmSizeof(void *msg);
CsrUint8 *CsrBtScDebugModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScDebugModeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScDebugModeCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_REQ
CsrSize CsrBtScDebondReqSizeof(void *msg);
CsrUint8 *CsrBtScDebondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScDebondReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScDebondReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM
CsrSize CsrBtScEncryptionKeySizeCfmSizeof(void *msg);
CsrUint8 *CsrBtScEncryptionKeySizeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScEncryptionKeySizeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScEncryptionKeySizeCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_IND
CsrSize CsrBtScRebondIndSizeof(void *msg);
CsrUint8 *CsrBtScRebondIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScRebondIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScRebondIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_REBOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_CFM
CsrSize CsrBtScDebondCfmSizeof(void *msg);
CsrUint8 *CsrBtScDebondCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScDebondCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScDebondCfmSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ
CsrSize CsrBtScUpdateDeviceReqSizeof(void *msg);
CsrUint8 *CsrBtScUpdateDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScUpdateDeviceReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScUpdateDeviceReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND
CsrSize CsrBtScUpdateTrustLevelIndSizeof(void *msg);
CsrUint8 *CsrBtScUpdateTrustLevelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScUpdateTrustLevelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScUpdateTrustLevelIndSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND */

#ifndef EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtScSetEncryptionKeySizeReqSizeof(void *msg);
CsrUint8 *CsrBtScSetEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSetEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSetEncryptionKeySizeReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND
CsrSize CsrBtScReadDeviceRecordIndSizeof(void *msg);
CsrUint8 *CsrBtScReadDeviceRecordIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScReadDeviceRecordIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScReadDeviceRecordIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SC_CONFIG_REQ
CsrSize CsrBtScConfigReqSizeof(void *msg);
CsrUint8 *CsrBtScConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScConfigReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_RES
CsrSize CsrBtScAuthoriseResSizeof(void *msg);
CsrUint8 *CsrBtScAuthoriseResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScAuthoriseResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScAuthoriseResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ
CsrSize CsrBtScCancelBondReqSizeof(void *msg);
CsrUint8 *CsrBtScCancelBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScCancelBondReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScCancelBondReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES
CsrSize CsrBtScSspNotificationResSizeof(void *msg);
CsrUint8 *CsrBtScSspNotificationResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScSspNotificationResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScSspNotificationResSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_SC_HOUSE_CLEANING
CsrSize CsrBtScHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtScHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScHouseCleaningSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SC_DEREGISTER_REQ
CsrSize CsrBtScDeregisterReqSizeof(void *msg);
CsrUint8 *CsrBtScDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScDeregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScDeregisterReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ
CsrSize CsrBtScAddRemoteOobDataReqSizeof(void *msg);
CsrUint8 *CsrBtScAddRemoteOobDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScAddRemoteOobDataReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScAddRemoteOobDataReqSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_MODE_IND
CsrSize CsrBtScModeIndSizeof(void *msg);
CsrUint8 *CsrBtScModeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScModeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtScModeIndSerFree CsrBtScPfree
#endif /* EXCLUDE_CSR_BT_SC_MODE_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND
CsrSize CsrBtScAuthoriseExtIndSizeof(void *msg);
CsrUint8 *CsrBtScAuthoriseExtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScAuthoriseExtIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScAuthoriseExtIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_IND
CsrSize CsrBtScAuthoriseIndSizeof(void *msg);
CsrUint8 *CsrBtScAuthoriseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtScAuthoriseIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtScAuthoriseIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_IND */

#ifdef __cplusplus
}
#endif

#endif
