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
#include "csr_bt_gatt_converter_init.h"
#ifndef EXCLUDE_CSR_BT_GATT_MODULE
#include "csr_bt_gatt_serialize.h"
#include "csr_bt_gatt_private_prim.h"

static CsrMsgConvMsgEntry csr_bt_gatt_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM
    { CSR_BT_GATT_ADVERTISE_CFM, CsrBtGattAdvertiseCfmSizeof, CsrBtGattAdvertiseCfmSer, CsrBtGattAdvertiseCfmDes, CsrBtGattAdvertiseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ
    { CSR_BT_GATT_ADVERTISE_REQ, CsrBtGattAdvertiseReqSizeof, CsrBtGattAdvertiseReqSer, CsrBtGattAdvertiseReqDes, CsrBtGattAdvertiseReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM
    { CSR_BT_GATT_BREDR_ACCEPT_CFM, CsrBtGattBredrAcceptCfmSizeof, CsrBtGattBredrAcceptCfmSer, CsrBtGattBredrAcceptCfmDes, CsrBtGattBredrAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ
    { CSR_BT_GATT_BREDR_ACCEPT_REQ, CsrBtGattBredrAcceptReqSizeof, CsrBtGattBredrAcceptReqSer, CsrBtGattBredrAcceptReqDes, CsrBtGattBredrAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM
    { CSR_BT_GATT_BREDR_CONNECT_CFM, CsrBtGattBredrConnectCfmSizeof, CsrBtGattBredrConnectCfmSer, CsrBtGattBredrConnectCfmDes, CsrBtGattBredrConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ
    { CSR_BT_GATT_BREDR_CONNECT_REQ, CsrBtGattBredrConnectReqSizeof, CsrBtGattBredrConnectReqSer, CsrBtGattBredrConnectReqDes, CsrBtGattBredrConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_CANCEL_REQ
    { CSR_BT_GATT_CANCEL_REQ, CsrBtGattCancelReqSizeof, CsrBtGattCancelReqSer, CsrBtGattCancelReqDes, CsrBtGattCancelReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_CANCEL_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_CFM
    { CSR_BT_GATT_CENTRAL_CFM, CsrBtGattCentralCfmSizeof, CsrBtGattCentralCfmSer, CsrBtGattCentralCfmDes, CsrBtGattCentralCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_REQ
    { CSR_BT_GATT_CENTRAL_REQ, CsrBtGattCentralReqSizeof, CsrBtGattCentralReqSer, CsrBtGattCentralReqDes, CsrBtGattCentralReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_CONNECT_IND
    { CSR_BT_GATT_CONNECT_IND, CsrBtGattConnectIndSizeof, CsrBtGattConnectIndSer, CsrBtGattConnectIndDes, CsrBtGattConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND
    { CSR_BT_GATT_DB_ACCESS_COMPLETE_IND, CsrBtGattDbAccessCompleteIndSizeof, CsrBtGattDbAccessCompleteIndSer, CsrBtGattDbAccessCompleteIndDes, CsrBtGattDbAccessCompleteIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND
    { CSR_BT_GATT_DB_ACCESS_READ_IND, CsrBtGattDbAccessReadIndSizeof, CsrBtGattDbAccessReadIndSer, CsrBtGattDbAccessReadIndDes, CsrBtGattDbAccessReadIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES
    { CSR_BT_GATT_DB_ACCESS_RES, CsrBtGattDbAccessResSizeof, CsrBtGattDbAccessResSer, CsrBtGattDbAccessResDes, CsrBtGattDbAccessResSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_WRITE_IND
    { CSR_BT_GATT_DB_ACCESS_WRITE_IND, CsrBtGattDbAccessWriteIndSizeof, CsrBtGattDbAccessWriteIndSer, CsrBtGattDbAccessWriteIndDes, CsrBtGattDbAccessWriteIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_WRITE_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ADD_CFM
    { CSR_BT_GATT_DB_ADD_CFM, CsrBtGattDbAddCfmSizeof, CsrBtGattDbAddCfmSer, CsrBtGattDbAddCfmDes, CsrBtGattDbAddCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ADD_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ADD_REQ
    { CSR_BT_GATT_DB_ADD_REQ, CsrBtGattDbAddReqSizeof, CsrBtGattDbAddReqSer, CsrBtGattDbAddReqDes, CsrBtGattDbAddReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ADD_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM
    { CSR_BT_GATT_DB_ALLOC_CFM, CsrBtGattDbAllocCfmSizeof, CsrBtGattDbAllocCfmSer, CsrBtGattDbAllocCfmDes, CsrBtGattDbAllocCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ
    { CSR_BT_GATT_DB_ALLOC_REQ, CsrBtGattDbAllocReqSizeof, CsrBtGattDbAllocReqSer, CsrBtGattDbAllocReqDes, CsrBtGattDbAllocReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM
    { CSR_BT_GATT_DB_DEALLOC_CFM, CsrBtGattDbDeallocCfmSizeof, CsrBtGattDbDeallocCfmSer, CsrBtGattDbDeallocCfmDes, CsrBtGattDbDeallocCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ
    { CSR_BT_GATT_DB_DEALLOC_REQ, CsrBtGattDbDeallocReqSizeof, CsrBtGattDbDeallocReqSer, CsrBtGattDbDeallocReqDes, CsrBtGattDbDeallocReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM
    { CSR_BT_GATT_DB_REMOVE_CFM, CsrBtGattDbRemoveCfmSizeof, CsrBtGattDbRemoveCfmSer, CsrBtGattDbRemoveCfmDes, CsrBtGattDbRemoveCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ
    { CSR_BT_GATT_DB_REMOVE_REQ, CsrBtGattDbRemoveReqSizeof, CsrBtGattDbRemoveReqSer, CsrBtGattDbRemoveReqDes, CsrBtGattDbRemoveReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_IND
    { CSR_BT_GATT_DISCONNECT_IND, CsrBtGattDisconnectIndSizeof, CsrBtGattDisconnectIndSer, CsrBtGattDisconnectIndDes, CsrBtGattDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ
    { CSR_BT_GATT_DISCONNECT_REQ, CsrBtGattDisconnectReqSizeof, CsrBtGattDisconnectReqSer, CsrBtGattDisconnectReqDes, CsrBtGattDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM
    { CSR_BT_GATT_DISCOVER_CHARAC_CFM, CsrBtGattDiscoverCharacCfmSizeof, CsrBtGattDiscoverCharacCfmSer, CsrBtGattDiscoverCharacCfmDes, CsrBtGattDiscoverCharacCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM
    { CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM, CsrBtGattDiscoverCharacDescriptorsCfmSizeof, CsrBtGattDiscoverCharacDescriptorsCfmSer, CsrBtGattDiscoverCharacDescriptorsCfmDes, CsrBtGattDiscoverCharacDescriptorsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND
    { CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND, CsrBtGattDiscoverCharacDescriptorsIndSizeof, CsrBtGattDiscoverCharacDescriptorsIndSer, CsrBtGattDiscoverCharacDescriptorsIndDes, CsrBtGattDiscoverCharacDescriptorsIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ
    { CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ, CsrBtGattDiscoverCharacDescriptorsReqSizeof, CsrBtGattDiscoverCharacDescriptorsReqSer, CsrBtGattDiscoverCharacDescriptorsReqDes, CsrBtGattDiscoverCharacDescriptorsReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND
    { CSR_BT_GATT_DISCOVER_CHARAC_IND, CsrBtGattDiscoverCharacIndSizeof, CsrBtGattDiscoverCharacIndSer, CsrBtGattDiscoverCharacIndDes, CsrBtGattDiscoverCharacIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ
    { CSR_BT_GATT_DISCOVER_CHARAC_REQ, CsrBtGattDiscoverCharacReqSizeof, CsrBtGattDiscoverCharacReqSer, CsrBtGattDiscoverCharacReqDes, CsrBtGattDiscoverCharacReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM
    { CSR_BT_GATT_DISCOVER_SERVICES_CFM, CsrBtGattDiscoverServicesCfmSizeof, CsrBtGattDiscoverServicesCfmSer, CsrBtGattDiscoverServicesCfmDes, CsrBtGattDiscoverServicesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND
    { CSR_BT_GATT_DISCOVER_SERVICES_IND, CsrBtGattDiscoverServicesIndSizeof, CsrBtGattDiscoverServicesIndSer, CsrBtGattDiscoverServicesIndDes, CsrBtGattDiscoverServicesIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ
    { CSR_BT_GATT_DISCOVER_SERVICES_REQ, CsrBtGattDiscoverServicesReqSizeof, CsrBtGattDiscoverServicesReqSer, CsrBtGattDiscoverServicesReqDes, CsrBtGattDiscoverServicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM
    { CSR_BT_GATT_EVENT_SEND_CFM, CsrBtGattEventSendCfmSizeof, CsrBtGattEventSendCfmSer, CsrBtGattEventSendCfmDes, CsrBtGattEventSendCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ
    { CSR_BT_GATT_EVENT_SEND_REQ, CsrBtGattEventSendReqSizeof, CsrBtGattEventSendReqSer, CsrBtGattEventSendReqDes, CsrBtGattEventSendReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ
    { CSR_BT_GATT_EXCHANGE_MTU_REQ, CsrBtGattExchangeMtuReqSizeof, CsrBtGattExchangeMtuReqSer, CsrBtGattExchangeMtuReqDes, CsrBtGattExchangeMtuReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM
    { CSR_BT_GATT_FIND_INCL_SERVICES_CFM, CsrBtGattFindInclServicesCfmSizeof, CsrBtGattFindInclServicesCfmSer, CsrBtGattFindInclServicesCfmDes, CsrBtGattFindInclServicesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND
    { CSR_BT_GATT_FIND_INCL_SERVICES_IND, CsrBtGattFindInclServicesIndSizeof, CsrBtGattFindInclServicesIndSer, CsrBtGattFindInclServicesIndDes, CsrBtGattFindInclServicesIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND */
#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ
    { CSR_BT_GATT_FIND_INCL_SERVICES_REQ, CsrBtGattFindInclServicesReqSizeof, CsrBtGattFindInclServicesReqSer, CsrBtGattFindInclServicesReqDes, CsrBtGattFindInclServicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND
    { CSR_BT_GATT_MTU_CHANGED_IND, CsrBtGattMtuChangedIndSizeof, CsrBtGattMtuChangedIndSer, CsrBtGattMtuChangedIndDes, CsrBtGattMtuChangedIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND */
#ifndef EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND
    { CSR_BT_GATT_NOTIFICATION_IND, CsrBtGattNotificationIndSizeof, CsrBtGattNotificationIndSer, CsrBtGattNotificationIndDes, CsrBtGattNotificationIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM
    { CSR_BT_GATT_PARAM_ADVERTISE_CFM, CsrBtGattParamAdvertiseCfmSizeof, CsrBtGattParamAdvertiseCfmSer, CsrBtGattParamAdvertiseCfmDes, CsrBtGattParamAdvertiseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ
    { CSR_BT_GATT_PARAM_ADVERTISE_REQ, CsrBtGattParamAdvertiseReqSizeof, CsrBtGattParamAdvertiseReqSer, CsrBtGattParamAdvertiseReqDes, CsrBtGattParamAdvertiseReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND
    { CSR_BT_GATT_PARAM_CONN_CHANGED_IND, CsrBtGattParamConnChangedIndSizeof, CsrBtGattParamConnChangedIndSer, CsrBtGattParamConnChangedIndDes, CsrBtGattParamConnChangedIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM
    { CSR_BT_GATT_PARAM_CONN_UPDATE_CFM, CsrBtGattParamConnUpdateCfmSizeof, CsrBtGattParamConnUpdateCfmSer, CsrBtGattParamConnUpdateCfmDes, CsrBtGattParamConnUpdateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND
    { CSR_BT_GATT_PARAM_CONN_UPDATE_IND, CsrBtGattParamConnUpdateIndSizeof, CsrBtGattParamConnUpdateIndSer, CsrBtGattParamConnUpdateIndDes, CsrBtGattParamConnUpdateIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ
    { CSR_BT_GATT_PARAM_CONN_UPDATE_REQ, CsrBtGattParamConnUpdateReqSizeof, CsrBtGattParamConnUpdateReqSer, CsrBtGattParamConnUpdateReqDes, CsrBtGattParamConnUpdateReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES
    { CSR_BT_GATT_PARAM_CONN_UPDATE_RES, CsrBtGattParamConnUpdateResSizeof, CsrBtGattParamConnUpdateResSer, CsrBtGattParamConnUpdateResDes, CsrBtGattParamConnUpdateResSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM
    { CSR_BT_GATT_PARAM_CONNECTION_CFM, CsrBtGattParamConnectionCfmSizeof, CsrBtGattParamConnectionCfmSer, CsrBtGattParamConnectionCfmDes, CsrBtGattParamConnectionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ
    { CSR_BT_GATT_PARAM_CONNECTION_REQ, CsrBtGattParamConnectionReqSizeof, CsrBtGattParamConnectionReqSer, CsrBtGattParamConnectionReqDes, CsrBtGattParamConnectionReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM
    { CSR_BT_GATT_PARAM_SCAN_CFM, CsrBtGattParamScanCfmSizeof, CsrBtGattParamScanCfmSer, CsrBtGattParamScanCfmDes, CsrBtGattParamScanCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ
    { CSR_BT_GATT_PARAM_SCAN_REQ, CsrBtGattParamScanReqSizeof, CsrBtGattParamScanReqSer, CsrBtGattParamScanReqDes, CsrBtGattParamScanReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM
    { CSR_BT_GATT_PERIPHERAL_CFM, CsrBtGattPeripheralCfmSizeof, CsrBtGattPeripheralCfmSer, CsrBtGattPeripheralCfmDes, CsrBtGattPeripheralCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ
    { CSR_BT_GATT_PERIPHERAL_REQ, CsrBtGattPeripheralReqSizeof, CsrBtGattPeripheralReqSer, CsrBtGattPeripheralReqDes, CsrBtGattPeripheralReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND
    { CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND, CsrBtGattPhysicalLinkStatusIndSizeof, CsrBtGattPhysicalLinkStatusIndSer, CsrBtGattPhysicalLinkStatusIndDes, CsrBtGattPhysicalLinkStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ
    { CSR_BT_GATT_PRIVATE_UNREGISTER_REQ, CsrBtGattPrivateUnregisterReqSizeof, CsrBtGattPrivateUnregisterReqSer, CsrBtGattPrivateUnregisterReqDes, CsrBtGattPrivateUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM
    { CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM, CsrBtGattReadAggregateFormatCfmSizeof, CsrBtGattReadAggregateFormatCfmSer, CsrBtGattReadAggregateFormatCfmDes, CsrBtGattReadAggregateFormatCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM
    { CSR_BT_GATT_READ_BY_UUID_CFM, CsrBtGattReadByUuidCfmSizeof, CsrBtGattReadByUuidCfmSer, CsrBtGattReadByUuidCfmDes, CsrBtGattReadByUuidCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND
    { CSR_BT_GATT_READ_BY_UUID_IND, CsrBtGattReadByUuidIndSizeof, CsrBtGattReadByUuidIndSer, CsrBtGattReadByUuidIndDes, CsrBtGattReadByUuidIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND */
#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ
    { CSR_BT_GATT_READ_BY_UUID_REQ, CsrBtGattReadByUuidReqSizeof, CsrBtGattReadByUuidReqSer, CsrBtGattReadByUuidReqDes, CsrBtGattReadByUuidReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_READ_CFM
    { CSR_BT_GATT_READ_CFM, CsrBtGattReadCfmSizeof, CsrBtGattReadCfmSer, CsrBtGattReadCfmDes, CsrBtGattReadCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM
    { CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM, CsrBtGattReadClientConfigurationCfmSizeof, CsrBtGattReadClientConfigurationCfmSer, CsrBtGattReadClientConfigurationCfmDes, CsrBtGattReadClientConfigurationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM
    { CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM, CsrBtGattReadExtendedPropertiesCfmSizeof, CsrBtGattReadExtendedPropertiesCfmSer, CsrBtGattReadExtendedPropertiesCfmDes, CsrBtGattReadExtendedPropertiesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM
    { CSR_BT_GATT_READ_MULTI_CFM, CsrBtGattReadMultiCfmSizeof, CsrBtGattReadMultiCfmSer, CsrBtGattReadMultiCfmDes, CsrBtGattReadMultiCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ
    { CSR_BT_GATT_READ_MULTI_REQ, CsrBtGattReadMultiReqSizeof, CsrBtGattReadMultiReqSer, CsrBtGattReadMultiReqDes, CsrBtGattReadMultiReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM
    { CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM, CsrBtGattReadPresentationFormatCfmSizeof, CsrBtGattReadPresentationFormatCfmSer, CsrBtGattReadPresentationFormatCfmDes, CsrBtGattReadPresentationFormatCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM
    { CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM, CsrBtGattReadProfileDefinedDescriptorCfmSizeof, CsrBtGattReadProfileDefinedDescriptorCfmSer, CsrBtGattReadProfileDefinedDescriptorCfmDes, CsrBtGattReadProfileDefinedDescriptorCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_REQ
    { CSR_BT_GATT_READ_REQ, CsrBtGattReadReqSizeof, CsrBtGattReadReqSer, CsrBtGattReadReqDes, CsrBtGattReadReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM
    { CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM, CsrBtGattReadServerConfigurationCfmSizeof, CsrBtGattReadServerConfigurationCfmSer, CsrBtGattReadServerConfigurationCfmDes, CsrBtGattReadServerConfigurationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM
    { CSR_BT_GATT_READ_USER_DESCRIPTION_CFM, CsrBtGattReadUserDescriptionCfmSizeof, CsrBtGattReadUserDescriptionCfmSer, CsrBtGattReadUserDescriptionCfmDes, CsrBtGattReadUserDescriptionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_CFM
    { CSR_BT_GATT_REGISTER_CFM, CsrBtGattRegisterCfmSizeof, CsrBtGattRegisterCfmSer, CsrBtGattRegisterCfmDes, CsrBtGattRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_REQ
    { CSR_BT_GATT_REGISTER_REQ, CsrBtGattRegisterReqSizeof, CsrBtGattRegisterReqSer, CsrBtGattRegisterReqDes, CsrBtGattRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_REPORT_IND
    { CSR_BT_GATT_REPORT_IND, CsrBtGattReportIndSizeof, CsrBtGattReportIndSer, CsrBtGattReportIndDes, CsrBtGattReportIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_REPORT_IND */
#ifndef EXCLUDE_CSR_BT_GATT_SCAN_CFM
    { CSR_BT_GATT_SCAN_CFM, CsrBtGattScanCfmSizeof, CsrBtGattScanCfmSer, CsrBtGattScanCfmDes, CsrBtGattScanCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SCAN_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_SCAN_REQ
    { CSR_BT_GATT_SCAN_REQ, CsrBtGattScanReqSizeof, CsrBtGattScanReqSer, CsrBtGattScanReqDes, CsrBtGattScanReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SCAN_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_SDS_REQ
    { CSR_BT_GATT_SDS_REQ, CsrBtGattSdsReqSizeof, CsrBtGattSdsReqSer, CsrBtGattSdsReqDes, CsrBtGattSdsReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SDS_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_CFM
    { CSR_BT_GATT_SECURITY_CFM, CsrBtGattSecurityCfmSizeof, CsrBtGattSecurityCfmSer, CsrBtGattSecurityCfmDes, CsrBtGattSecurityCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_REQ
    { CSR_BT_GATT_SECURITY_REQ, CsrBtGattSecurityReqSizeof, CsrBtGattSecurityReqSer, CsrBtGattSecurityReqDes, CsrBtGattSecurityReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND
    { CSR_BT_GATT_SERVICE_CHANGED_IND, CsrBtGattServiceChangedIndSizeof, CsrBtGattServiceChangedIndSer, CsrBtGattServiceChangedIndDes, CsrBtGattServiceChangedIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND */
#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM
    { CSR_BT_GATT_SET_EVENT_MASK_CFM, CsrBtGattSetEventMaskCfmSizeof, CsrBtGattSetEventMaskCfmSer, CsrBtGattSetEventMaskCfmDes, CsrBtGattSetEventMaskCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ
    { CSR_BT_GATT_SET_EVENT_MASK_REQ, CsrBtGattSetEventMaskReqSizeof, CsrBtGattSetEventMaskReqSer, CsrBtGattSetEventMaskReqDes, CsrBtGattSetEventMaskReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM
    { CSR_BT_GATT_SUBSCRIPTION_CFM, CsrBtGattSubscriptionCfmSizeof, CsrBtGattSubscriptionCfmSer, CsrBtGattSubscriptionCfmDes, CsrBtGattSubscriptionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ
    { CSR_BT_GATT_SUBSCRIPTION_REQ, CsrBtGattSubscriptionReqSizeof, CsrBtGattSubscriptionReqSer, CsrBtGattSubscriptionReqDes, CsrBtGattSubscriptionReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM
    { CSR_BT_GATT_UNREGISTER_CFM, CsrBtGattUnregisterCfmSizeof, CsrBtGattUnregisterCfmSer, CsrBtGattUnregisterCfmDes, CsrBtGattUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ
    { CSR_BT_GATT_UNREGISTER_REQ, CsrBtGattUnregisterReqSizeof, CsrBtGattUnregisterReqSer, CsrBtGattUnregisterReqDes, CsrBtGattUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM
    { CSR_BT_GATT_WHITELIST_ADD_CFM, CsrBtGattWhitelistAddCfmSizeof, CsrBtGattWhitelistAddCfmSer, CsrBtGattWhitelistAddCfmDes, CsrBtGattWhitelistAddCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ
    { CSR_BT_GATT_WHITELIST_ADD_REQ, CsrBtGattWhitelistAddReqSizeof, CsrBtGattWhitelistAddReqSer, CsrBtGattWhitelistAddReqDes, CsrBtGattWhitelistAddReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND
    { CSR_BT_GATT_WHITELIST_CHANGE_IND, CsrBtGattWhitelistChangeIndSizeof, CsrBtGattWhitelistChangeIndSer, CsrBtGattWhitelistChangeIndDes, CsrBtGattWhitelistChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM
    { CSR_BT_GATT_WHITELIST_CLEAR_CFM, CsrBtGattWhitelistClearCfmSizeof, CsrBtGattWhitelistClearCfmSer, CsrBtGattWhitelistClearCfmDes, CsrBtGattWhitelistClearCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ
    { CSR_BT_GATT_WHITELIST_CLEAR_REQ, CsrBtGattWhitelistClearReqSizeof, CsrBtGattWhitelistClearReqSer, CsrBtGattWhitelistClearReqDes, CsrBtGattWhitelistClearReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM
    { CSR_BT_GATT_WHITELIST_READ_CFM, CsrBtGattWhitelistReadCfmSizeof, CsrBtGattWhitelistReadCfmSer, CsrBtGattWhitelistReadCfmDes, CsrBtGattWhitelistReadCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ
    { CSR_BT_GATT_WHITELIST_READ_REQ, CsrBtGattWhitelistReadReqSizeof, CsrBtGattWhitelistReadReqSer, CsrBtGattWhitelistReadReqDes, CsrBtGattWhitelistReadReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CFM
    { CSR_BT_GATT_WRITE_CFM, CsrBtGattWriteCfmSizeof, CsrBtGattWriteCfmSer, CsrBtGattWriteCfmDes, CsrBtGattWriteCfmSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ
    { CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ, CsrBtGattWriteClientConfigurationReqSizeof, CsrBtGattWriteClientConfigurationReqSer, CsrBtGattWriteClientConfigurationReqDes, CsrBtGattWriteClientConfigurationReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_WRITE_REQ
    { CSR_BT_GATT_WRITE_REQ, CsrBtGattWriteReqSizeof, CsrBtGattWriteReqSer, CsrBtGattWriteReqDes, CsrBtGattWriteReqSerFree },
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtGattConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_GATT_PRIM, csr_bt_gatt_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_gatt_conv_info = {
    CSR_BT_GATT_PRIM,
    "CSR_BT_GATT_PRIM",
    csr_bt_gatt_conv_lut
};

CsrLogPrimitiveInformation* CsrBtGattTechInfoGet(void)
{
    return &csr_bt_gatt_conv_info;
}
#endif
#endif

