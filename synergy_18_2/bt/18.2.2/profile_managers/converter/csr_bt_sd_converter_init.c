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
#include "csr_bt_sd_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SD_MODULE
#include "csr_bt_sd_serialize.h"
#include "csr_bt_sd_private_prim.h"

static CsrMsgConvMsgEntry csr_bt_sd_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM
    { CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM, CsrBtSdCancelReadAvailableServicesCfmSizeof, CsrBtSdCancelReadAvailableServicesCfmSer, CsrBtSdCancelReadAvailableServicesCfmDes, CsrBtSdCancelReadAvailableServicesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */
#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ
    { CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ, CsrBtSdCancelReadAvailableServicesReqSizeof, CsrBtSdCancelReadAvailableServicesReqSer, CsrBtSdCancelReadAvailableServicesReqDes, CsrBtSdCancelReadAvailableServicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ
    { CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ, CsrBtSdCancelReadServiceRecordReqSizeof, CsrBtSdCancelReadServiceRecordReqSer, CsrBtSdCancelReadServiceRecordReqDes, CsrBtSdCancelReadServiceRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ
    { CSR_BT_SD_CANCEL_SEARCH_REQ, CsrBtSdCancelSearchReqSizeof, CsrBtSdCancelSearchReqSer, CsrBtSdCancelSearchReqDes, CsrBtSdCancelSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_CFM
    { CSR_BT_SD_CLEANUP_CFM, CsrBtSdCleanupCfmSizeof, CsrBtSdCleanupCfmSer, CsrBtSdCleanupCfmDes, CsrBtSdCleanupCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_CFM */
#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_REQ
    { CSR_BT_SD_CLEANUP_REQ, CsrBtSdCleanupReqSizeof, CsrBtSdCleanupReqSer, CsrBtSdCleanupReqDes, CsrBtSdCleanupReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_REQ */
#ifndef EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND
    { CSR_BT_SD_CLOSE_SEARCH_IND, CsrBtSdCloseSearchIndSizeof, CsrBtSdCloseSearchIndSer, CsrBtSdCloseSearchIndDes, CsrBtSdCloseSearchIndSerFree },
#endif /* EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND */
#ifndef EXCLUDE_CSR_BT_SD_HOUSE_CLEANING
    { CSR_BT_SD_HOUSE_CLEANING, CsrBtSdHouseCleaningSizeof, CsrBtSdHouseCleaningSer, CsrBtSdHouseCleaningDes, CsrBtSdHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_SD_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND
    { CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND, CsrBtSdImmediateSearchResultIndSizeof, CsrBtSdImmediateSearchResultIndSer, CsrBtSdImmediateSearchResultIndDes, CsrBtSdImmediateSearchResultIndSerFree },
#endif /* EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM
    { CSR_BT_SD_MEMORY_CONFIG_CFM, CsrBtSdMemoryConfigCfmSizeof, CsrBtSdMemoryConfigCfmSer, CsrBtSdMemoryConfigCfmDes, CsrBtSdMemoryConfigCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM */
#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ
    { CSR_BT_SD_MEMORY_CONFIG_REQ, CsrBtSdMemoryConfigReqSizeof, CsrBtSdMemoryConfigReqSer, CsrBtSdMemoryConfigReqDes, CsrBtSdMemoryConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM
    { CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM, CsrBtSdReadAvailableServicesCfmSizeof, CsrBtSdReadAvailableServicesCfmSer, CsrBtSdReadAvailableServicesCfmDes, CsrBtSdReadAvailableServicesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ
    { CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ, CsrBtSdReadAvailableServicesReqSizeof, CsrBtSdReadAvailableServicesReqSer, CsrBtSdReadAvailableServicesReqDes, CsrBtSdReadAvailableServicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM
    { CSR_BT_SD_READ_DEVICE_INFO_CFM, CsrBtSdReadDeviceInfoCfmSizeof, CsrBtSdReadDeviceInfoCfmSer, CsrBtSdReadDeviceInfoCfmDes, CsrBtSdReadDeviceInfoCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ
    { CSR_BT_SD_READ_DEVICE_INFO_REQ, CsrBtSdReadDeviceInfoReqSizeof, CsrBtSdReadDeviceInfoReqSer, CsrBtSdReadDeviceInfoReqDes, CsrBtSdReadDeviceInfoReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_CFM
    { CSR_BT_SD_READ_DEVICE_LIST_CFM, CsrBtSdReadDeviceListCfmSizeof, CsrBtSdReadDeviceListCfmSer, CsrBtSdReadDeviceListCfmDes, CsrBtSdReadDeviceListCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_IND
    { CSR_BT_SD_READ_DEVICE_LIST_IND, CsrBtSdReadDeviceListIndSizeof, CsrBtSdReadDeviceListIndSer, CsrBtSdReadDeviceListIndDes, CsrBtSdReadDeviceListIndSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_IND */
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ
    { CSR_BT_SD_READ_DEVICE_LIST_REQ, CsrBtSdReadDeviceListReqSizeof, CsrBtSdReadDeviceListReqSer, CsrBtSdReadDeviceListReqDes, CsrBtSdReadDeviceListReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM
    { CSR_BT_SD_READ_SERVICE_RECORD_CFM, CsrBtSdReadServiceRecordCfmSizeof, CsrBtSdReadServiceRecordCfmSer, CsrBtSdReadServiceRecordCfmDes, CsrBtSdReadServiceRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND
    { CSR_BT_SD_READ_SERVICE_RECORD_IND, CsrBtSdReadServiceRecordIndSizeof, CsrBtSdReadServiceRecordIndSer, CsrBtSdReadServiceRecordIndDes, CsrBtSdReadServiceRecordIndSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ
    { CSR_BT_SD_READ_SERVICE_RECORD_REQ, CsrBtSdReadServiceRecordReqSizeof, CsrBtSdReadServiceRecordReqSer, CsrBtSdReadServiceRecordReqDes, CsrBtSdReadServiceRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM
    { CSR_BT_SD_READ_SERVICES_CANCEL_CFM, CsrBtSdReadServicesCancelCfmSizeof, CsrBtSdReadServicesCancelCfmSer, CsrBtSdReadServicesCancelCfmDes, CsrBtSdReadServicesCancelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ
    { CSR_BT_SD_READ_SERVICES_CANCEL_REQ, CsrBtSdReadServicesCancelReqSizeof, CsrBtSdReadServicesCancelReqSer, CsrBtSdReadServicesCancelReqDes, CsrBtSdReadServicesCancelReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM
    { CSR_BT_SD_READ_SERVICES_CFM, CsrBtSdReadServicesCfmSizeof, CsrBtSdReadServicesCfmSer, CsrBtSdReadServicesCfmDes, CsrBtSdReadServicesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ
    { CSR_BT_SD_READ_SERVICES_REQ, CsrBtSdReadServicesReqSizeof, CsrBtSdReadServicesReqSer, CsrBtSdReadServicesReqDes, CsrBtSdReadServicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ
    { CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ, CsrBtSdRegisterAvailableServiceReqSizeof, CsrBtSdRegisterAvailableServiceReqSer, CsrBtSdRegisterAvailableServiceReqDes, CsrBtSdRegisterAvailableServiceReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ */
#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM
    { CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM, CsrBtSdRegisterServiceRecordCfmSizeof, CsrBtSdRegisterServiceRecordCfmSer, CsrBtSdRegisterServiceRecordCfmDes, CsrBtSdRegisterServiceRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ
    { CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ, CsrBtSdRegisterServiceRecordReqSizeof, CsrBtSdRegisterServiceRecordReqSer, CsrBtSdRegisterServiceRecordReqDes, CsrBtSdRegisterServiceRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM
    { CSR_BT_SD_SEARCH_CONFIG_CFM, CsrBtSdSearchConfigCfmSizeof, CsrBtSdSearchConfigCfmSer, CsrBtSdSearchConfigCfmDes, CsrBtSdSearchConfigCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM */
#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ
    { CSR_BT_SD_SEARCH_CONFIG_REQ, CsrBtSdSearchConfigReqSizeof, CsrBtSdSearchConfigReqSer, CsrBtSdSearchConfigReqDes, CsrBtSdSearchConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_SD_SEARCH_REQ
    { CSR_BT_SD_SEARCH_REQ, CsrBtSdSearchReqSizeof, CsrBtSdSearchReqSer, CsrBtSdSearchReqDes, CsrBtSdSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND
    { CSR_BT_SD_SEARCH_RESULT_IND, CsrBtSdSearchResultIndSizeof, CsrBtSdSearchResultIndSer, CsrBtSdSearchResultIndDes, CsrBtSdSearchResultIndSerFree },
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM
    { CSR_BT_SD_SYNCHRONIZE_CFM, CsrBtSdSynchronizeCfmSizeof, CsrBtSdSynchronizeCfmSer, CsrBtSdSynchronizeCfmDes, CsrBtSdSynchronizeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM */
#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ
    { CSR_BT_SD_SYNCHRONIZE_REQ, CsrBtSdSynchronizeReqSizeof, CsrBtSdSynchronizeReqSer, CsrBtSdSynchronizeReqDes, CsrBtSdSynchronizeReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ */
#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM
    { CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM, CsrBtSdUnregisterServiceRecordCfmSizeof, CsrBtSdUnregisterServiceRecordCfmSer, CsrBtSdUnregisterServiceRecordCfmDes, CsrBtSdUnregisterServiceRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ
    { CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ, CsrBtSdUnregisterServiceRecordReqSizeof, CsrBtSdUnregisterServiceRecordReqSer, CsrBtSdUnregisterServiceRecordReqDes, CsrBtSdUnregisterServiceRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ
    { CSR_BT_SD_UPDATE_DEVICE_REQ, CsrBtSdUpdateDeviceReqSizeof, CsrBtSdUpdateDeviceReqSer, CsrBtSdUpdateDeviceReqDes, CsrBtSdUpdateDeviceReqSerFree },
#endif /* EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSdConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SD_PRIM, csr_bt_sd_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_sd_conv_info = {
    CSR_BT_SD_PRIM,
    "CSR_BT_SD_PRIM",
    csr_bt_sd_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSdTechInfoGet(void)
{
    return &csr_bt_sd_conv_info;
}
#endif
#endif

