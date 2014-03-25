#ifndef CSR_BT_SD_SERIALIZE_H__
#define CSR_BT_SD_SERIALIZE_H__
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

void CsrBtSdPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ
CsrSize CsrBtSdReadServicesCancelReqSizeof(void *msg);
CsrUint8 *CsrBtSdReadServicesCancelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadServicesCancelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdReadServicesCancelReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM
CsrSize CsrBtSdCancelReadAvailableServicesCfmSizeof(void *msg);
CsrUint8 *CsrBtSdCancelReadAvailableServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdCancelReadAvailableServicesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdCancelReadAvailableServicesCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM
CsrSize CsrBtSdRegisterServiceRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtSdRegisterServiceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdRegisterServiceRecordCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdRegisterServiceRecordCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND
CsrSize CsrBtSdReadServiceRecordIndSizeof(void *msg);
CsrUint8 *CsrBtSdReadServiceRecordIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadServiceRecordIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdReadServiceRecordIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND
CsrSize CsrBtSdSearchResultIndSizeof(void *msg);
CsrUint8 *CsrBtSdSearchResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdSearchResultIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdSearchResultIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_REQ
CsrSize CsrBtSdCleanupReqSizeof(void *msg);
CsrUint8 *CsrBtSdCleanupReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdCleanupReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdCleanupReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ
CsrSize CsrBtSdReadServicesReqSizeof(void *msg);
CsrUint8 *CsrBtSdReadServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadServicesReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdReadServicesReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ
CsrSize CsrBtSdReadDeviceInfoReqSizeof(void *msg);
CsrUint8 *CsrBtSdReadDeviceInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadDeviceInfoReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdReadDeviceInfoReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ
CsrSize CsrBtSdCancelSearchReqSizeof(void *msg);
CsrUint8 *CsrBtSdCancelSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdCancelSearchReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdCancelSearchReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND
CsrSize CsrBtSdImmediateSearchResultIndSizeof(void *msg);
CsrUint8 *CsrBtSdImmediateSearchResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdImmediateSearchResultIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdImmediateSearchResultIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_CFM
CsrSize CsrBtSdCleanupCfmSizeof(void *msg);
CsrUint8 *CsrBtSdCleanupCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdCleanupCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdCleanupCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_REQ
CsrSize CsrBtSdSearchReqSizeof(void *msg);
CsrUint8 *CsrBtSdSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdSearchReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdSearchReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_HOUSE_CLEANING
CsrSize CsrBtSdHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtSdHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdHouseCleaningSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ
CsrSize CsrBtSdMemoryConfigReqSizeof(void *msg);
CsrUint8 *CsrBtSdMemoryConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdMemoryConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdMemoryConfigReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM
CsrSize CsrBtSdReadServicesCfmSizeof(void *msg);
CsrUint8 *CsrBtSdReadServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadServicesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdReadServicesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ
CsrSize CsrBtSdSearchConfigReqSizeof(void *msg);
CsrUint8 *CsrBtSdSearchConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdSearchConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdSearchConfigReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM
CsrSize CsrBtSdUnregisterServiceRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtSdUnregisterServiceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdUnregisterServiceRecordCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdUnregisterServiceRecordCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ
CsrSize CsrBtSdCancelReadAvailableServicesReqSizeof(void *msg);
CsrUint8 *CsrBtSdCancelReadAvailableServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdCancelReadAvailableServicesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdCancelReadAvailableServicesReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM
CsrSize CsrBtSdReadServiceRecordCfmSizeof(void *msg);
CsrUint8 *CsrBtSdReadServiceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadServiceRecordCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdReadServiceRecordCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ
CsrSize CsrBtSdReadDeviceListReqSizeof(void *msg);
CsrUint8 *CsrBtSdReadDeviceListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadDeviceListReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdReadDeviceListReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM
CsrSize CsrBtSdReadServicesCancelCfmSizeof(void *msg);
CsrUint8 *CsrBtSdReadServicesCancelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadServicesCancelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdReadServicesCancelCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ
CsrSize CsrBtSdReadServiceRecordReqSizeof(void *msg);
CsrUint8 *CsrBtSdReadServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadServiceRecordReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdReadServiceRecordReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM
CsrSize CsrBtSdReadAvailableServicesCfmSizeof(void *msg);
CsrUint8 *CsrBtSdReadAvailableServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadAvailableServicesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdReadAvailableServicesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ
CsrSize CsrBtSdUnregisterServiceRecordReqSizeof(void *msg);
CsrUint8 *CsrBtSdUnregisterServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdUnregisterServiceRecordReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdUnregisterServiceRecordReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ
CsrSize CsrBtSdSynchronizeReqSizeof(void *msg);
CsrUint8 *CsrBtSdSynchronizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdSynchronizeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdSynchronizeReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM
CsrSize CsrBtSdSynchronizeCfmSizeof(void *msg);
CsrUint8 *CsrBtSdSynchronizeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdSynchronizeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdSynchronizeCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM
CsrSize CsrBtSdMemoryConfigCfmSizeof(void *msg);
CsrUint8 *CsrBtSdMemoryConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdMemoryConfigCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdMemoryConfigCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM
CsrSize CsrBtSdReadDeviceInfoCfmSizeof(void *msg);
CsrUint8 *CsrBtSdReadDeviceInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadDeviceInfoCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdReadDeviceInfoCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ
CsrSize CsrBtSdCancelReadServiceRecordReqSizeof(void *msg);
CsrUint8 *CsrBtSdCancelReadServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdCancelReadServiceRecordReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdCancelReadServiceRecordReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ
CsrSize CsrBtSdReadAvailableServicesReqSizeof(void *msg);
CsrUint8 *CsrBtSdReadAvailableServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdReadAvailableServicesReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdReadAvailableServicesReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM
CsrSize CsrBtSdSearchConfigCfmSizeof(void *msg);
CsrUint8 *CsrBtSdSearchConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdSearchConfigCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdSearchConfigCfmSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ
CsrSize CsrBtSdUpdateDeviceReqSizeof(void *msg);
CsrUint8 *CsrBtSdUpdateDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdUpdateDeviceReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdUpdateDeviceReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ
CsrSize CsrBtSdRegisterAvailableServiceReqSizeof(void *msg);
CsrUint8 *CsrBtSdRegisterAvailableServiceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdRegisterAvailableServiceReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdRegisterAvailableServiceReqSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ
CsrSize CsrBtSdRegisterServiceRecordReqSizeof(void *msg);
CsrUint8 *CsrBtSdRegisterServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdRegisterServiceRecordReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSdRegisterServiceRecordReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND
CsrSize CsrBtSdCloseSearchIndSizeof(void *msg);
CsrUint8 *CsrBtSdCloseSearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSdCloseSearchIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSdCloseSearchIndSerFree CsrBtSdPfree
#endif /* EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND */

#ifdef __cplusplus
}
#endif

#endif
