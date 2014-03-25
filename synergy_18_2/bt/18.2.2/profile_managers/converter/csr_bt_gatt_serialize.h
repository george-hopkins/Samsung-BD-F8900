#ifndef CSR_BT_GATT_SERIALIZE_H__
#define CSR_BT_GATT_SERIALIZE_H__
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

void CsrBtGattPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM
CsrSize CsrBtGattReadServerConfigurationCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadServerConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadServerConfigurationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReadServerConfigurationCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ
CsrSize CsrBtGattDiscoverCharacReqSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverCharacReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverCharacReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverCharacReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ
CsrSize CsrBtGattParamConnectionReqSizeof(void *msg);
CsrUint8 *CsrBtGattParamConnectionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamConnectionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamConnectionReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM
CsrSize CsrBtGattWhitelistClearCfmSizeof(void *msg);
CsrUint8 *CsrBtGattWhitelistClearCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWhitelistClearCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattWhitelistClearCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND
CsrSize CsrBtGattServiceChangedIndSizeof(void *msg);
CsrUint8 *CsrBtGattServiceChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattServiceChangedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattServiceChangedIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_CFM
CsrSize CsrBtGattSecurityCfmSizeof(void *msg);
CsrUint8 *CsrBtGattSecurityCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattSecurityCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattSecurityCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND
CsrSize CsrBtGattDbAccessReadIndSizeof(void *msg);
CsrUint8 *CsrBtGattDbAccessReadIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbAccessReadIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbAccessReadIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND
CsrSize CsrBtGattReadByUuidIndSizeof(void *msg);
CsrUint8 *CsrBtGattReadByUuidIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadByUuidIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattReadByUuidIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM
CsrSize CsrBtGattBredrAcceptCfmSizeof(void *msg);
CsrUint8 *CsrBtGattBredrAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattBredrAcceptCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattBredrAcceptCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CFM
CsrSize CsrBtGattWriteCfmSizeof(void *msg);
CsrUint8 *CsrBtGattWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWriteCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattWriteCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND
CsrSize CsrBtGattParamConnChangedIndSizeof(void *msg);
CsrUint8 *CsrBtGattParamConnChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamConnChangedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamConnChangedIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ
CsrSize CsrBtGattBredrAcceptReqSizeof(void *msg);
CsrUint8 *CsrBtGattBredrAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattBredrAcceptReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattBredrAcceptReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM
CsrSize CsrBtGattDbRemoveCfmSizeof(void *msg);
CsrUint8 *CsrBtGattDbRemoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbRemoveCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbRemoveCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM
CsrSize CsrBtGattPeripheralCfmSizeof(void *msg);
CsrUint8 *CsrBtGattPeripheralCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattPeripheralCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattPeripheralCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ
CsrSize CsrBtGattDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtGattDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDisconnectReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES
CsrSize CsrBtGattDbAccessResSizeof(void *msg);
CsrUint8 *CsrBtGattDbAccessResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbAccessResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattDbAccessResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM
CsrSize CsrBtGattReadPresentationFormatCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadPresentationFormatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadPresentationFormatCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReadPresentationFormatCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ
CsrSize CsrBtGattEventSendReqSizeof(void *msg);
CsrUint8 *CsrBtGattEventSendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattEventSendReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattEventSendReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM
CsrSize CsrBtGattReadExtendedPropertiesCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadExtendedPropertiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadExtendedPropertiesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReadExtendedPropertiesCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM
CsrSize CsrBtGattAdvertiseCfmSizeof(void *msg);
CsrUint8 *CsrBtGattAdvertiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattAdvertiseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattAdvertiseCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ
CsrSize CsrBtGattParamScanReqSizeof(void *msg);
CsrUint8 *CsrBtGattParamScanReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamScanReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamScanReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND
CsrSize CsrBtGattDiscoverCharacIndSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverCharacIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverCharacIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverCharacIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND
CsrSize CsrBtGattWhitelistChangeIndSizeof(void *msg);
CsrUint8 *CsrBtGattWhitelistChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWhitelistChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattWhitelistChangeIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ
CsrSize CsrBtGattReadByUuidReqSizeof(void *msg);
CsrUint8 *CsrBtGattReadByUuidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadByUuidReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReadByUuidReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ
CsrSize CsrBtGattWhitelistAddReqSizeof(void *msg);
CsrUint8 *CsrBtGattWhitelistAddReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWhitelistAddReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattWhitelistAddReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES
CsrSize CsrBtGattParamConnUpdateResSizeof(void *msg);
CsrUint8 *CsrBtGattParamConnUpdateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamConnUpdateResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamConnUpdateResSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM
CsrSize CsrBtGattEventSendCfmSizeof(void *msg);
CsrUint8 *CsrBtGattEventSendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattEventSendCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattEventSendCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ
CsrSize CsrBtGattExchangeMtuReqSizeof(void *msg);
CsrUint8 *CsrBtGattExchangeMtuReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattExchangeMtuReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattExchangeMtuReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM
CsrSize CsrBtGattDiscoverCharacCfmSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverCharacCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverCharacCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverCharacCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND
CsrSize CsrBtGattFindInclServicesIndSizeof(void *msg);
CsrUint8 *CsrBtGattFindInclServicesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattFindInclServicesIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattFindInclServicesIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM
CsrSize CsrBtGattDiscoverCharacDescriptorsCfmSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverCharacDescriptorsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverCharacDescriptorsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverCharacDescriptorsCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ
CsrSize CsrBtGattAdvertiseReqSizeof(void *msg);
CsrUint8 *CsrBtGattAdvertiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattAdvertiseReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattAdvertiseReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_REQ
CsrSize CsrBtGattSecurityReqSizeof(void *msg);
CsrUint8 *CsrBtGattSecurityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattSecurityReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattSecurityReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ
CsrSize CsrBtGattDiscoverServicesReqSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverServicesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverServicesReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM
CsrSize CsrBtGattDbDeallocCfmSizeof(void *msg);
CsrUint8 *CsrBtGattDbDeallocCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbDeallocCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbDeallocCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ
CsrSize CsrBtGattBredrConnectReqSizeof(void *msg);
CsrUint8 *CsrBtGattBredrConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattBredrConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattBredrConnectReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CFM
CsrSize CsrBtGattReadCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattReadCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_REQ
CsrSize CsrBtGattRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtGattRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattRegisterReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM
CsrSize CsrBtGattDiscoverServicesCfmSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverServicesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverServicesCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM
CsrSize CsrBtGattReadProfileDefinedDescriptorCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadProfileDefinedDescriptorCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadProfileDefinedDescriptorCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattReadProfileDefinedDescriptorCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM
CsrSize CsrBtGattReadClientConfigurationCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadClientConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadClientConfigurationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReadClientConfigurationCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ADD_CFM
CsrSize CsrBtGattDbAddCfmSizeof(void *msg);
CsrUint8 *CsrBtGattDbAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbAddCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbAddCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM
CsrSize CsrBtGattWhitelistAddCfmSizeof(void *msg);
CsrUint8 *CsrBtGattWhitelistAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWhitelistAddCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattWhitelistAddCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ
CsrSize CsrBtGattDbAllocReqSizeof(void *msg);
CsrUint8 *CsrBtGattDbAllocReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbAllocReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbAllocReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM
CsrSize CsrBtGattWhitelistReadCfmSizeof(void *msg);
CsrUint8 *CsrBtGattWhitelistReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWhitelistReadCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattWhitelistReadCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM
CsrSize CsrBtGattDbAllocCfmSizeof(void *msg);
CsrUint8 *CsrBtGattDbAllocCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbAllocCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbAllocCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_REQ
CsrSize CsrBtGattReadReqSizeof(void *msg);
CsrUint8 *CsrBtGattReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReadReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_REQ
CsrSize CsrBtGattCentralReqSizeof(void *msg);
CsrUint8 *CsrBtGattCentralReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattCentralReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattCentralReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ
CsrSize CsrBtGattParamAdvertiseReqSizeof(void *msg);
CsrUint8 *CsrBtGattParamAdvertiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamAdvertiseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamAdvertiseReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM
CsrSize CsrBtGattReadAggregateFormatCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadAggregateFormatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadAggregateFormatCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattReadAggregateFormatCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SCAN_CFM
CsrSize CsrBtGattScanCfmSizeof(void *msg);
CsrUint8 *CsrBtGattScanCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattScanCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattScanCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REPORT_IND
CsrSize CsrBtGattReportIndSizeof(void *msg);
CsrUint8 *CsrBtGattReportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReportIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReportIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ
CsrSize CsrBtGattSubscriptionReqSizeof(void *msg);
CsrUint8 *CsrBtGattSubscriptionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattSubscriptionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattSubscriptionReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ
CsrSize CsrBtGattSetEventMaskReqSizeof(void *msg);
CsrUint8 *CsrBtGattSetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattSetEventMaskReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattSetEventMaskReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM
CsrSize CsrBtGattReadUserDescriptionCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadUserDescriptionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadUserDescriptionCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattReadUserDescriptionCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_CANCEL_REQ
CsrSize CsrBtGattCancelReqSizeof(void *msg);
CsrUint8 *CsrBtGattCancelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattCancelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattCancelReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_CFM
CsrSize CsrBtGattCentralCfmSizeof(void *msg);
CsrUint8 *CsrBtGattCentralCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattCentralCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattCentralCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ
CsrSize CsrBtGattWriteClientConfigurationReqSizeof(void *msg);
CsrUint8 *CsrBtGattWriteClientConfigurationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWriteClientConfigurationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattWriteClientConfigurationReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SDS_REQ
CsrSize CsrBtGattSdsReqSizeof(void *msg);
CsrUint8 *CsrBtGattSdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattSdsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattSdsReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SDS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ
CsrSize CsrBtGattPrivateUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtGattPrivateUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattPrivateUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattPrivateUnregisterReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM
CsrSize CsrBtGattSetEventMaskCfmSizeof(void *msg);
CsrUint8 *CsrBtGattSetEventMaskCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattSetEventMaskCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattSetEventMaskCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ
CsrSize CsrBtGattFindInclServicesReqSizeof(void *msg);
CsrUint8 *CsrBtGattFindInclServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattFindInclServicesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattFindInclServicesReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ
CsrSize CsrBtGattWhitelistClearReqSizeof(void *msg);
CsrUint8 *CsrBtGattWhitelistClearReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWhitelistClearReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattWhitelistClearReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND
CsrSize CsrBtGattMtuChangedIndSizeof(void *msg);
CsrUint8 *CsrBtGattMtuChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattMtuChangedIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattMtuChangedIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM
CsrSize CsrBtGattSubscriptionCfmSizeof(void *msg);
CsrUint8 *CsrBtGattSubscriptionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattSubscriptionCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattSubscriptionCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM
CsrSize CsrBtGattReadByUuidCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadByUuidCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadByUuidCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattReadByUuidCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND
CsrSize CsrBtGattDbAccessCompleteIndSizeof(void *msg);
CsrUint8 *CsrBtGattDbAccessCompleteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbAccessCompleteIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbAccessCompleteIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_IND
CsrSize CsrBtGattDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtGattDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDisconnectIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ
CsrSize CsrBtGattReadMultiReqSizeof(void *msg);
CsrUint8 *CsrBtGattReadMultiReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadMultiReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattReadMultiReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM
CsrSize CsrBtGattParamAdvertiseCfmSizeof(void *msg);
CsrUint8 *CsrBtGattParamAdvertiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamAdvertiseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamAdvertiseCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND
CsrSize CsrBtGattDiscoverCharacDescriptorsIndSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverCharacDescriptorsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverCharacDescriptorsIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverCharacDescriptorsIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM
CsrSize CsrBtGattReadMultiCfmSizeof(void *msg);
CsrUint8 *CsrBtGattReadMultiCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattReadMultiCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattReadMultiCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND
CsrSize CsrBtGattNotificationIndSizeof(void *msg);
CsrUint8 *CsrBtGattNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattNotificationIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattNotificationIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ
CsrSize CsrBtGattUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtGattUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattUnregisterReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM
CsrSize CsrBtGattUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtGattUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattUnregisterCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND
CsrSize CsrBtGattDiscoverServicesIndSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverServicesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverServicesIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverServicesIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ
CsrSize CsrBtGattWhitelistReadReqSizeof(void *msg);
CsrUint8 *CsrBtGattWhitelistReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattWhitelistReadReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattWhitelistReadReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CONNECT_IND
CsrSize CsrBtGattConnectIndSizeof(void *msg);
CsrUint8 *CsrBtGattConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattConnectIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ
CsrSize CsrBtGattDiscoverCharacDescriptorsReqSizeof(void *msg);
CsrUint8 *CsrBtGattDiscoverCharacDescriptorsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDiscoverCharacDescriptorsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDiscoverCharacDescriptorsReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ
CsrSize CsrBtGattDbDeallocReqSizeof(void *msg);
CsrUint8 *CsrBtGattDbDeallocReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbDeallocReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbDeallocReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND
CsrSize CsrBtGattParamConnUpdateIndSizeof(void *msg);
CsrUint8 *CsrBtGattParamConnUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamConnUpdateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamConnUpdateIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ
CsrSize CsrBtGattDbRemoveReqSizeof(void *msg);
CsrUint8 *CsrBtGattDbRemoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattDbRemoveReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattDbRemoveReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM
CsrSize CsrBtGattFindInclServicesCfmSizeof(void *msg);
CsrUint8 *CsrBtGattFindInclServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattFindInclServicesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattFindInclServicesCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ
CsrSize CsrBtGattPeripheralReqSizeof(void *msg);
CsrUint8 *CsrBtGattPeripheralReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattPeripheralReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtGattPeripheralReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM
CsrSize CsrBtGattBredrConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtGattBredrConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattBredrConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattBredrConnectCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND
CsrSize CsrBtGattPhysicalLinkStatusIndSizeof(void *msg);
CsrUint8 *CsrBtGattPhysicalLinkStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattPhysicalLinkStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattPhysicalLinkStatusIndSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM
CsrSize CsrBtGattParamConnUpdateCfmSizeof(void *msg);
CsrUint8 *CsrBtGattParamConnUpdateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamConnUpdateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamConnUpdateCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM
CsrSize CsrBtGattParamConnectionCfmSizeof(void *msg);
CsrUint8 *CsrBtGattParamConnectionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamConnectionCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamConnectionCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM
CsrSize CsrBtGattParamScanCfmSizeof(void *msg);
CsrUint8 *CsrBtGattParamScanCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamScanCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamScanCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_CFM
CsrSize CsrBtGattRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtGattRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattRegisterCfmSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ
CsrSize CsrBtGattParamConnUpdateReqSizeof(void *msg);
CsrUint8 *CsrBtGattParamConnUpdateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtGattParamConnUpdateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtGattParamConnUpdateReqSerFree CsrBtGattPfree
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ */

#ifdef __cplusplus
}
#endif

#endif
