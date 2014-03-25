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
#include "csr_bt_jsr82_converter_init.h"
#ifndef EXCLUDE_CSR_BT_JSR82_MODULE
#include "csr_bt_jsr82_serialize.h"
#include "csr_bt_jsr82_prim.h"

static CsrMsgConvMsgEntry csr_bt_jsr82_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ
    { CSR_BT_JSR82_CLEANUP_REQ, CsrBtJsr82CleanupReqSizeof, CsrBtJsr82CleanupReqSer, CsrBtJsr82CleanupReqDes, CsrBtJsr82CleanupReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA
    { CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA, CsrBtJsr82ContinueRfcSendDataSizeof, CsrBtJsr82ContinueRfcSendDataSer, CsrBtJsr82ContinueRfcSendDataDes, CsrBtJsr82ContinueRfcSendDataSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ
    { CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ, CsrBtJsr82DaCancelInquiryReqSizeof, CsrBtJsr82DaCancelInquiryReqSer, CsrBtJsr82DaCancelInquiryReqDes, CsrBtJsr82DaCancelInquiryReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ
    { CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ, CsrBtJsr82DaCancelServiceSearchReqSizeof, CsrBtJsr82DaCancelServiceSearchReqSer, CsrBtJsr82DaCancelServiceSearchReqDes, CsrBtJsr82DaCancelServiceSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM
    { CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM, CsrBtJsr82DaRetrieveDevicesCfmSizeof, CsrBtJsr82DaRetrieveDevicesCfmSer, CsrBtJsr82DaRetrieveDevicesCfmDes, CsrBtJsr82DaRetrieveDevicesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ
    { CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ, CsrBtJsr82DaRetrieveDevicesReqSizeof, CsrBtJsr82DaRetrieveDevicesReqSer, CsrBtJsr82DaRetrieveDevicesReqDes, CsrBtJsr82DaRetrieveDevicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ
    { CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ, CsrBtJsr82DaSearchServicesReqSizeof, CsrBtJsr82DaSearchServicesReqSer, CsrBtJsr82DaSearchServicesReqDes, CsrBtJsr82DaSearchServicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM
    { CSR_BT_JSR82_DA_SELECT_SERVICE_CFM, CsrBtJsr82DaSelectServiceCfmSizeof, CsrBtJsr82DaSelectServiceCfmSer, CsrBtJsr82DaSelectServiceCfmDes, CsrBtJsr82DaSelectServiceCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ
    { CSR_BT_JSR82_DA_SELECT_SERVICE_REQ, CsrBtJsr82DaSelectServiceReqSizeof, CsrBtJsr82DaSelectServiceReqSer, CsrBtJsr82DaSelectServiceReqDes, CsrBtJsr82DaSelectServiceReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ
    { CSR_BT_JSR82_DA_START_INQUIRY_REQ, CsrBtJsr82DaStartInquiryReqSizeof, CsrBtJsr82DaStartInquiryReqSer, CsrBtJsr82DaStartInquiryReqDes, CsrBtJsr82DaStartInquiryReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND
    { CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND, CsrBtJsr82DlDeviceDiscoveredIndSizeof, CsrBtJsr82DlDeviceDiscoveredIndSer, CsrBtJsr82DlDeviceDiscoveredIndDes, CsrBtJsr82DlDeviceDiscoveredIndSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND */
#ifndef EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND
    { CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND, CsrBtJsr82DlInquiryCompletedIndSizeof, CsrBtJsr82DlInquiryCompletedIndSer, CsrBtJsr82DlInquiryCompletedIndDes, CsrBtJsr82DlInquiryCompletedIndSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND */
#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND
    { CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND, CsrBtJsr82DlServiceSearchCompletedIndSizeof, CsrBtJsr82DlServiceSearchCompletedIndSer, CsrBtJsr82DlServiceSearchCompletedIndDes, CsrBtJsr82DlServiceSearchCompletedIndSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND */
#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND
    { CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND, CsrBtJsr82DlServicesDiscoveredIndSizeof, CsrBtJsr82DlServicesDiscoveredIndSer, CsrBtJsr82DlServicesDiscoveredIndDes, CsrBtJsr82DlServicesDiscoveredIndSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND */
#ifndef EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING
    { CSR_BT_JSR82_HOUSE_CLEANING, CsrBtJsr82HouseCleaningSizeof, CsrBtJsr82HouseCleaningSer, CsrBtJsr82HouseCleaningDes, CsrBtJsr82HouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM
    { CSR_BT_JSR82_L2CA_ACCEPT_CFM, CsrBtJsr82L2caAcceptCfmSizeof, CsrBtJsr82L2caAcceptCfmSer, CsrBtJsr82L2caAcceptCfmDes, CsrBtJsr82L2caAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ
    { CSR_BT_JSR82_L2CA_ACCEPT_REQ, CsrBtJsr82L2caAcceptReqSizeof, CsrBtJsr82L2caAcceptReqSer, CsrBtJsr82L2caAcceptReqDes, CsrBtJsr82L2caAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM
    { CSR_BT_JSR82_L2CA_CLOSE_CFM, CsrBtJsr82L2caCloseCfmSizeof, CsrBtJsr82L2caCloseCfmSer, CsrBtJsr82L2caCloseCfmDes, CsrBtJsr82L2caCloseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND
    { CSR_BT_JSR82_L2CA_CLOSE_IND, CsrBtJsr82L2caCloseIndSizeof, CsrBtJsr82L2caCloseIndSer, CsrBtJsr82L2caCloseIndDes, CsrBtJsr82L2caCloseIndSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ
    { CSR_BT_JSR82_L2CA_CLOSE_REQ, CsrBtJsr82L2caCloseReqSizeof, CsrBtJsr82L2caCloseReqSer, CsrBtJsr82L2caCloseReqDes, CsrBtJsr82L2caCloseReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM
    { CSR_BT_JSR82_L2CA_DISCONNECT_CFM, CsrBtJsr82L2caDisconnectCfmSizeof, CsrBtJsr82L2caDisconnectCfmSer, CsrBtJsr82L2caDisconnectCfmDes, CsrBtJsr82L2caDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ
    { CSR_BT_JSR82_L2CA_DISCONNECT_REQ, CsrBtJsr82L2caDisconnectReqSizeof, CsrBtJsr82L2caDisconnectReqSer, CsrBtJsr82L2caDisconnectReqDes, CsrBtJsr82L2caDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM
    { CSR_BT_JSR82_L2CA_GET_CONFIG_CFM, CsrBtJsr82L2caGetConfigCfmSizeof, CsrBtJsr82L2caGetConfigCfmSer, CsrBtJsr82L2caGetConfigCfmDes, CsrBtJsr82L2caGetConfigCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ
    { CSR_BT_JSR82_L2CA_GET_CONFIG_REQ, CsrBtJsr82L2caGetConfigReqSizeof, CsrBtJsr82L2caGetConfigReqSer, CsrBtJsr82L2caGetConfigReqDes, CsrBtJsr82L2caGetConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM
    { CSR_BT_JSR82_L2CA_GET_PSM_CFM, CsrBtJsr82L2caGetPsmCfmSizeof, CsrBtJsr82L2caGetPsmCfmSer, CsrBtJsr82L2caGetPsmCfmDes, CsrBtJsr82L2caGetPsmCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ
    { CSR_BT_JSR82_L2CA_GET_PSM_REQ, CsrBtJsr82L2caGetPsmReqSizeof, CsrBtJsr82L2caGetPsmReqSer, CsrBtJsr82L2caGetPsmReqDes, CsrBtJsr82L2caGetPsmReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM
    { CSR_BT_JSR82_L2CA_OPEN_CFM, CsrBtJsr82L2caOpenCfmSizeof, CsrBtJsr82L2caOpenCfmSer, CsrBtJsr82L2caOpenCfmDes, CsrBtJsr82L2caOpenCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ
    { CSR_BT_JSR82_L2CA_OPEN_REQ, CsrBtJsr82L2caOpenReqSizeof, CsrBtJsr82L2caOpenReqSer, CsrBtJsr82L2caOpenReqDes, CsrBtJsr82L2caOpenReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM
    { CSR_BT_JSR82_L2CA_RX_DATA_CFM, CsrBtJsr82L2caRxDataCfmSizeof, CsrBtJsr82L2caRxDataCfmSer, CsrBtJsr82L2caRxDataCfmDes, CsrBtJsr82L2caRxDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ
    { CSR_BT_JSR82_L2CA_RX_DATA_REQ, CsrBtJsr82L2caRxDataReqSizeof, CsrBtJsr82L2caRxDataReqSer, CsrBtJsr82L2caRxDataReqDes, CsrBtJsr82L2caRxDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM
    { CSR_BT_JSR82_L2CA_RX_READY_CFM, CsrBtJsr82L2caRxReadyCfmSizeof, CsrBtJsr82L2caRxReadyCfmSer, CsrBtJsr82L2caRxReadyCfmDes, CsrBtJsr82L2caRxReadyCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ
    { CSR_BT_JSR82_L2CA_RX_READY_REQ, CsrBtJsr82L2caRxReadyReqSizeof, CsrBtJsr82L2caRxReadyReqSer, CsrBtJsr82L2caRxReadyReqDes, CsrBtJsr82L2caRxReadyReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM
    { CSR_BT_JSR82_L2CA_TX_DATA_CFM, CsrBtJsr82L2caTxDataCfmSizeof, CsrBtJsr82L2caTxDataCfmSer, CsrBtJsr82L2caTxDataCfmDes, CsrBtJsr82L2caTxDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ
    { CSR_BT_JSR82_L2CA_TX_DATA_REQ, CsrBtJsr82L2caTxDataReqSizeof, CsrBtJsr82L2caTxDataReqSer, CsrBtJsr82L2caTxDataReqDes, CsrBtJsr82L2caTxDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM
    { CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM, CsrBtJsr82LdGetBluetoothAddressCfmSizeof, CsrBtJsr82LdGetBluetoothAddressCfmSer, CsrBtJsr82LdGetBluetoothAddressCfmDes, CsrBtJsr82LdGetBluetoothAddressCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ
    { CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ, CsrBtJsr82LdGetBluetoothAddressReqSizeof, CsrBtJsr82LdGetBluetoothAddressReqSer, CsrBtJsr82LdGetBluetoothAddressReqDes, CsrBtJsr82LdGetBluetoothAddressReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM
    { CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM, CsrBtJsr82LdGetDeviceClassCfmSizeof, CsrBtJsr82LdGetDeviceClassCfmSer, CsrBtJsr82LdGetDeviceClassCfmDes, CsrBtJsr82LdGetDeviceClassCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ
    { CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ, CsrBtJsr82LdGetDeviceClassReqSizeof, CsrBtJsr82LdGetDeviceClassReqSer, CsrBtJsr82LdGetDeviceClassReqDes, CsrBtJsr82LdGetDeviceClassReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM
    { CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM, CsrBtJsr82LdGetDiscoverableCfmSizeof, CsrBtJsr82LdGetDiscoverableCfmSer, CsrBtJsr82LdGetDiscoverableCfmDes, CsrBtJsr82LdGetDiscoverableCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ
    { CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ, CsrBtJsr82LdGetDiscoverableReqSizeof, CsrBtJsr82LdGetDiscoverableReqSer, CsrBtJsr82LdGetDiscoverableReqDes, CsrBtJsr82LdGetDiscoverableReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM
    { CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM, CsrBtJsr82LdGetFriendlyNameCfmSizeof, CsrBtJsr82LdGetFriendlyNameCfmSer, CsrBtJsr82LdGetFriendlyNameCfmDes, CsrBtJsr82LdGetFriendlyNameCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ
    { CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ, CsrBtJsr82LdGetFriendlyNameReqSizeof, CsrBtJsr82LdGetFriendlyNameReqSer, CsrBtJsr82LdGetFriendlyNameReqDes, CsrBtJsr82LdGetFriendlyNameReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM
    { CSR_BT_JSR82_LD_GET_PROPERTY_CFM, CsrBtJsr82LdGetPropertyCfmSizeof, CsrBtJsr82LdGetPropertyCfmSer, CsrBtJsr82LdGetPropertyCfmDes, CsrBtJsr82LdGetPropertyCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ
    { CSR_BT_JSR82_LD_GET_PROPERTY_REQ, CsrBtJsr82LdGetPropertyReqSizeof, CsrBtJsr82LdGetPropertyReqSer, CsrBtJsr82LdGetPropertyReqDes, CsrBtJsr82LdGetPropertyReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM
    { CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM, CsrBtJsr82LdGetSecurityLevelCfmSizeof, CsrBtJsr82LdGetSecurityLevelCfmSer, CsrBtJsr82LdGetSecurityLevelCfmDes, CsrBtJsr82LdGetSecurityLevelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ
    { CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ, CsrBtJsr82LdGetSecurityLevelReqSizeof, CsrBtJsr82LdGetSecurityLevelReqSer, CsrBtJsr82LdGetSecurityLevelReqDes, CsrBtJsr82LdGetSecurityLevelReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM
    { CSR_BT_JSR82_LD_IS_MASTER_CFM, CsrBtJsr82LdIsMasterCfmSizeof, CsrBtJsr82LdIsMasterCfmSer, CsrBtJsr82LdIsMasterCfmDes, CsrBtJsr82LdIsMasterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ
    { CSR_BT_JSR82_LD_IS_MASTER_REQ, CsrBtJsr82LdIsMasterReqSizeof, CsrBtJsr82LdIsMasterReqSer, CsrBtJsr82LdIsMasterReqDes, CsrBtJsr82LdIsMasterReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM
    { CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM, CsrBtJsr82LdSetDeviceClassCfmSizeof, CsrBtJsr82LdSetDeviceClassCfmSer, CsrBtJsr82LdSetDeviceClassCfmDes, CsrBtJsr82LdSetDeviceClassCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ
    { CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ, CsrBtJsr82LdSetDeviceClassReqSizeof, CsrBtJsr82LdSetDeviceClassReqSer, CsrBtJsr82LdSetDeviceClassReqDes, CsrBtJsr82LdSetDeviceClassReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM
    { CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM, CsrBtJsr82LdSetDiscoverableCfmSizeof, CsrBtJsr82LdSetDiscoverableCfmSer, CsrBtJsr82LdSetDiscoverableCfmDes, CsrBtJsr82LdSetDiscoverableCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ
    { CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ, CsrBtJsr82LdSetDiscoverableReqSizeof, CsrBtJsr82LdSetDiscoverableReqSer, CsrBtJsr82LdSetDiscoverableReqDes, CsrBtJsr82LdSetDiscoverableReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM
    { CSR_BT_JSR82_LD_UPDATE_RECORD_CFM, CsrBtJsr82LdUpdateRecordCfmSizeof, CsrBtJsr82LdUpdateRecordCfmSer, CsrBtJsr82LdUpdateRecordCfmDes, CsrBtJsr82LdUpdateRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ
    { CSR_BT_JSR82_LD_UPDATE_RECORD_REQ, CsrBtJsr82LdUpdateRecordReqSizeof, CsrBtJsr82LdUpdateRecordReqSer, CsrBtJsr82LdUpdateRecordReqDes, CsrBtJsr82LdUpdateRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM
    { CSR_BT_JSR82_RD_AUTHENTICATE_CFM, CsrBtJsr82RdAuthenticateCfmSizeof, CsrBtJsr82RdAuthenticateCfmSer, CsrBtJsr82RdAuthenticateCfmDes, CsrBtJsr82RdAuthenticateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ
    { CSR_BT_JSR82_RD_AUTHENTICATE_REQ, CsrBtJsr82RdAuthenticateReqSizeof, CsrBtJsr82RdAuthenticateReqSer, CsrBtJsr82RdAuthenticateReqDes, CsrBtJsr82RdAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM
    { CSR_BT_JSR82_RD_ENCRYPT_CFM, CsrBtJsr82RdEncryptCfmSizeof, CsrBtJsr82RdEncryptCfmSer, CsrBtJsr82RdEncryptCfmDes, CsrBtJsr82RdEncryptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ
    { CSR_BT_JSR82_RD_ENCRYPT_REQ, CsrBtJsr82RdEncryptReqSizeof, CsrBtJsr82RdEncryptReqSer, CsrBtJsr82RdEncryptReqDes, CsrBtJsr82RdEncryptReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM
    { CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM, CsrBtJsr82RdGetFriendlyNameCfmSizeof, CsrBtJsr82RdGetFriendlyNameCfmSer, CsrBtJsr82RdGetFriendlyNameCfmDes, CsrBtJsr82RdGetFriendlyNameCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ
    { CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ, CsrBtJsr82RdGetFriendlyNameReqSizeof, CsrBtJsr82RdGetFriendlyNameReqSer, CsrBtJsr82RdGetFriendlyNameReqDes, CsrBtJsr82RdGetFriendlyNameReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM
    { CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM, CsrBtJsr82RdIsAuthenticatedCfmSizeof, CsrBtJsr82RdIsAuthenticatedCfmSer, CsrBtJsr82RdIsAuthenticatedCfmDes, CsrBtJsr82RdIsAuthenticatedCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ
    { CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ, CsrBtJsr82RdIsAuthenticatedReqSizeof, CsrBtJsr82RdIsAuthenticatedReqSer, CsrBtJsr82RdIsAuthenticatedReqDes, CsrBtJsr82RdIsAuthenticatedReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM
    { CSR_BT_JSR82_RD_IS_CONNECTED_CFM, CsrBtJsr82RdIsConnectedCfmSizeof, CsrBtJsr82RdIsConnectedCfmSer, CsrBtJsr82RdIsConnectedCfmDes, CsrBtJsr82RdIsConnectedCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ
    { CSR_BT_JSR82_RD_IS_CONNECTED_REQ, CsrBtJsr82RdIsConnectedReqSizeof, CsrBtJsr82RdIsConnectedReqSer, CsrBtJsr82RdIsConnectedReqDes, CsrBtJsr82RdIsConnectedReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM
    { CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM, CsrBtJsr82RdIsEncryptedCfmSizeof, CsrBtJsr82RdIsEncryptedCfmSer, CsrBtJsr82RdIsEncryptedCfmDes, CsrBtJsr82RdIsEncryptedCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ
    { CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ, CsrBtJsr82RdIsEncryptedReqSizeof, CsrBtJsr82RdIsEncryptedReqSer, CsrBtJsr82RdIsEncryptedReqDes, CsrBtJsr82RdIsEncryptedReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM
    { CSR_BT_JSR82_RD_IS_TRUSTED_CFM, CsrBtJsr82RdIsTrustedCfmSizeof, CsrBtJsr82RdIsTrustedCfmSer, CsrBtJsr82RdIsTrustedCfmDes, CsrBtJsr82RdIsTrustedCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ
    { CSR_BT_JSR82_RD_IS_TRUSTED_REQ, CsrBtJsr82RdIsTrustedReqSizeof, CsrBtJsr82RdIsTrustedReqSer, CsrBtJsr82RdIsTrustedReqDes, CsrBtJsr82RdIsTrustedReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM
    { CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM, CsrBtJsr82RfcAcceptAndOpenCfmSizeof, CsrBtJsr82RfcAcceptAndOpenCfmSer, CsrBtJsr82RfcAcceptAndOpenCfmDes, CsrBtJsr82RfcAcceptAndOpenCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ
    { CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ, CsrBtJsr82RfcAcceptAndOpenReqSizeof, CsrBtJsr82RfcAcceptAndOpenReqSer, CsrBtJsr82RfcAcceptAndOpenReqDes, CsrBtJsr82RfcAcceptAndOpenReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM
    { CSR_BT_JSR82_RFC_CLOSE_CFM, CsrBtJsr82RfcCloseCfmSizeof, CsrBtJsr82RfcCloseCfmSer, CsrBtJsr82RfcCloseCfmDes, CsrBtJsr82RfcCloseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND
    { CSR_BT_JSR82_RFC_CLOSE_IND, CsrBtJsr82RfcCloseIndSizeof, CsrBtJsr82RfcCloseIndSer, CsrBtJsr82RfcCloseIndDes, CsrBtJsr82RfcCloseIndSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ
    { CSR_BT_JSR82_RFC_CLOSE_REQ, CsrBtJsr82RfcCloseReqSizeof, CsrBtJsr82RfcCloseReqSer, CsrBtJsr82RfcCloseReqDes, CsrBtJsr82RfcCloseReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM
    { CSR_BT_JSR82_RFC_CONNECT_CFM, CsrBtJsr82RfcConnectCfmSizeof, CsrBtJsr82RfcConnectCfmSer, CsrBtJsr82RfcConnectCfmDes, CsrBtJsr82RfcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ
    { CSR_BT_JSR82_RFC_CONNECT_REQ, CsrBtJsr82RfcConnectReqSizeof, CsrBtJsr82RfcConnectReqSer, CsrBtJsr82RfcConnectReqDes, CsrBtJsr82RfcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM
    { CSR_BT_JSR82_RFC_DISCONNECT_CFM, CsrBtJsr82RfcDisconnectCfmSizeof, CsrBtJsr82RfcDisconnectCfmSer, CsrBtJsr82RfcDisconnectCfmDes, CsrBtJsr82RfcDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ
    { CSR_BT_JSR82_RFC_DISCONNECT_REQ, CsrBtJsr82RfcDisconnectReqSizeof, CsrBtJsr82RfcDisconnectReqSer, CsrBtJsr82RfcDisconnectReqDes, CsrBtJsr82RfcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM
    { CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM, CsrBtJsr82RfcGetAvailableCfmSizeof, CsrBtJsr82RfcGetAvailableCfmSer, CsrBtJsr82RfcGetAvailableCfmDes, CsrBtJsr82RfcGetAvailableCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ
    { CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ, CsrBtJsr82RfcGetAvailableReqSizeof, CsrBtJsr82RfcGetAvailableReqSer, CsrBtJsr82RfcGetAvailableReqDes, CsrBtJsr82RfcGetAvailableReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM
    { CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM, CsrBtJsr82RfcGetServerChannelCfmSizeof, CsrBtJsr82RfcGetServerChannelCfmSer, CsrBtJsr82RfcGetServerChannelCfmDes, CsrBtJsr82RfcGetServerChannelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ
    { CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ, CsrBtJsr82RfcGetServerChannelReqSizeof, CsrBtJsr82RfcGetServerChannelReqSer, CsrBtJsr82RfcGetServerChannelReqDes, CsrBtJsr82RfcGetServerChannelReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM
    { CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM, CsrBtJsr82RfcReceiveDataCfmSizeof, CsrBtJsr82RfcReceiveDataCfmSer, CsrBtJsr82RfcReceiveDataCfmDes, CsrBtJsr82RfcReceiveDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ
    { CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ, CsrBtJsr82RfcReceiveDataReqSizeof, CsrBtJsr82RfcReceiveDataReqSer, CsrBtJsr82RfcReceiveDataReqDes, CsrBtJsr82RfcReceiveDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM
    { CSR_BT_JSR82_RFC_SEND_DATA_CFM, CsrBtJsr82RfcSendDataCfmSizeof, CsrBtJsr82RfcSendDataCfmSer, CsrBtJsr82RfcSendDataCfmDes, CsrBtJsr82RfcSendDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ
    { CSR_BT_JSR82_RFC_SEND_DATA_REQ, CsrBtJsr82RfcSendDataReqSizeof, CsrBtJsr82RfcSendDataReqSer, CsrBtJsr82RfcSendDataReqDes, CsrBtJsr82RfcSendDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM
    { CSR_BT_JSR82_SET_EVENT_MASK_CFM, CsrBtJsr82SetEventMaskCfmSizeof, CsrBtJsr82SetEventMaskCfmSer, CsrBtJsr82SetEventMaskCfmDes, CsrBtJsr82SetEventMaskCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ
    { CSR_BT_JSR82_SET_EVENT_MASK_REQ, CsrBtJsr82SetEventMaskReqSizeof, CsrBtJsr82SetEventMaskReqSer, CsrBtJsr82SetEventMaskReqDes, CsrBtJsr82SetEventMaskReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM
    { CSR_BT_JSR82_SR_CREATE_RECORD_CFM, CsrBtJsr82SrCreateRecordCfmSizeof, CsrBtJsr82SrCreateRecordCfmSer, CsrBtJsr82SrCreateRecordCfmDes, CsrBtJsr82SrCreateRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ
    { CSR_BT_JSR82_SR_CREATE_RECORD_REQ, CsrBtJsr82SrCreateRecordReqSizeof, CsrBtJsr82SrCreateRecordReqSer, CsrBtJsr82SrCreateRecordReqDes, CsrBtJsr82SrCreateRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM
    { CSR_BT_JSR82_SR_POPULATE_RECORD_CFM, CsrBtJsr82SrPopulateRecordCfmSizeof, CsrBtJsr82SrPopulateRecordCfmSer, CsrBtJsr82SrPopulateRecordCfmDes, CsrBtJsr82SrPopulateRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ
    { CSR_BT_JSR82_SR_POPULATE_RECORD_REQ, CsrBtJsr82SrPopulateRecordReqSizeof, CsrBtJsr82SrPopulateRecordReqSer, CsrBtJsr82SrPopulateRecordReqDes, CsrBtJsr82SrPopulateRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM
    { CSR_BT_JSR82_SR_REGISTER_RECORD_CFM, CsrBtJsr82SrRegisterRecordCfmSizeof, CsrBtJsr82SrRegisterRecordCfmSer, CsrBtJsr82SrRegisterRecordCfmDes, CsrBtJsr82SrRegisterRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ
    { CSR_BT_JSR82_SR_REGISTER_RECORD_REQ, CsrBtJsr82SrRegisterRecordReqSizeof, CsrBtJsr82SrRegisterRecordReqSer, CsrBtJsr82SrRegisterRecordReqDes, CsrBtJsr82SrRegisterRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM
    { CSR_BT_JSR82_SR_REMOVE_RECORD_CFM, CsrBtJsr82SrRemoveRecordCfmSizeof, CsrBtJsr82SrRemoveRecordCfmSer, CsrBtJsr82SrRemoveRecordCfmDes, CsrBtJsr82SrRemoveRecordCfmSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ
    { CSR_BT_JSR82_SR_REMOVE_RECORD_REQ, CsrBtJsr82SrRemoveRecordReqSizeof, CsrBtJsr82SrRemoveRecordReqSer, CsrBtJsr82SrRemoveRecordReqDes, CsrBtJsr82SrRemoveRecordReqSerFree },
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtJsr82ConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_JSR82_PRIM, csr_bt_jsr82_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_jsr82_conv_info = {
    CSR_BT_JSR82_PRIM,
    "CSR_BT_JSR82_PRIM",
    csr_bt_jsr82_conv_lut
};

CsrLogPrimitiveInformation* CsrBtJsr82TechInfoGet(void)
{
    return &csr_bt_jsr82_conv_info;
}
#endif
#endif

