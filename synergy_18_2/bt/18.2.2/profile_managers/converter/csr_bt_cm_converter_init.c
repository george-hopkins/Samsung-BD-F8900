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
#include "csr_bt_cm_converter_init.h"
#ifndef EXCLUDE_CSR_BT_CM_MODULE
#include "csr_bt_cm_serialize.h"
#include "csr_bt_cm_prim.h"

static CsrMsgConvMsgEntry csr_bt_cm_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND
    { CSR_BT_CM_A2DP_BIT_RATE_IND, CsrBtCmA2dpBitRateIndSizeof, CsrBtCmA2dpBitRateIndSer, CsrBtCmA2dpBitRateIndDes, CsrBtCmA2dpBitRateIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND */
#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ
    { CSR_BT_CM_A2DP_BIT_RATE_REQ, CsrBtCmA2dpBitRateReqSizeof, CsrBtCmA2dpBitRateReqSer, CsrBtCmA2dpBitRateReqDes, CsrBtCmA2dpBitRateReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT
    { CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT, CsrBtCmAcceptConnectTimeoutSizeof, CsrBtCmAcceptConnectTimeoutSer, CsrBtCmAcceptConnectTimeoutDes, CsrBtCmAcceptConnectTimeoutSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT */
#ifndef EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND
    { CSR_BT_CM_ACL_CONNECT_IND, CsrBtCmAclConnectIndSizeof, CsrBtCmAclConnectIndSer, CsrBtCmAclConnectIndDes, CsrBtCmAclConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM
    { CSR_BT_CM_ACL_DETACH_CFM, CsrBtCmAclDetachCfmSizeof, CsrBtCmAclDetachCfmSer, CsrBtCmAclDetachCfmDes, CsrBtCmAclDetachCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM */
#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ
    { CSR_BT_CM_ACL_DETACH_REQ, CsrBtCmAclDetachReqSizeof, CsrBtCmAclDetachReqSer, CsrBtCmAclDetachReqDes, CsrBtCmAclDetachReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND
    { CSR_BT_CM_ACL_DISCONNECT_IND, CsrBtCmAclDisconnectIndSizeof, CsrBtCmAclDisconnectIndSer, CsrBtCmAclDisconnectIndDes, CsrBtCmAclDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM
    { CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM, CsrBtCmAlwaysMasterDevicesCfmSizeof, CsrBtCmAlwaysMasterDevicesCfmSer, CsrBtCmAlwaysMasterDevicesCfmDes, CsrBtCmAlwaysMasterDevicesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM */
#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ
    { CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ, CsrBtCmAlwaysMasterDevicesReqSizeof, CsrBtCmAlwaysMasterDevicesReqSer, CsrBtCmAlwaysMasterDevicesReqDes, CsrBtCmAlwaysMasterDevicesReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ */
#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ
    { CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ, CsrBtCmAlwaysSupportMasterRoleReqSizeof, CsrBtCmAlwaysSupportMasterRoleReqSer, CsrBtCmAlwaysSupportMasterRoleReqDes, CsrBtCmAlwaysSupportMasterRoleReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND
    { CSR_BT_CM_BLE_CONNECTION_IND, CsrBtCmBleConnectionIndSizeof, CsrBtCmBleConnectionIndSer, CsrBtCmBleConnectionIndDes, CsrBtCmBleConnectionIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND */
#ifndef EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND
    { CSR_BT_CM_BLUECORE_INITIALIZED_IND, CsrBtCmBluecoreInitializedIndSizeof, CsrBtCmBluecoreInitializedIndSer, CsrBtCmBluecoreInitializedIndDes, CsrBtCmBluecoreInitializedIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM
    { CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM, CsrBtCmBnepCancelConnectAcceptCfmSizeof, CsrBtCmBnepCancelConnectAcceptCfmSer, CsrBtCmBnepCancelConnectAcceptCfmDes, CsrBtCmBnepCancelConnectAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ
    { CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ, CsrBtCmBnepCancelConnectAcceptReqSizeof, CsrBtCmBnepCancelConnectAcceptReqSer, CsrBtCmBnepCancelConnectAcceptReqDes, CsrBtCmBnepCancelConnectAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM
    { CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM, CsrBtCmBnepConnectAcceptCfmSizeof, CsrBtCmBnepConnectAcceptCfmSer, CsrBtCmBnepConnectAcceptCfmDes, CsrBtCmBnepConnectAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ
    { CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ, CsrBtCmBnepConnectAcceptReqSizeof, CsrBtCmBnepConnectAcceptReqSer, CsrBtCmBnepConnectAcceptReqDes, CsrBtCmBnepConnectAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND
    { CSR_BT_CM_BNEP_CONNECT_IND, CsrBtCmBnepConnectIndSizeof, CsrBtCmBnepConnectIndSer, CsrBtCmBnepConnectIndDes, CsrBtCmBnepConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_REQ
    { CSR_BT_CM_BNEP_CONNECT_REQ, CsrBtCmBnepConnectReqSizeof, CsrBtCmBnepConnectReqSer, CsrBtCmBnepConnectReqDes, CsrBtCmBnepConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND
    { CSR_BT_CM_BNEP_DISCONNECT_IND, CsrBtCmBnepDisconnectIndSizeof, CsrBtCmBnepDisconnectIndSer, CsrBtCmBnepDisconnectIndDes, CsrBtCmBnepDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ
    { CSR_BT_CM_BNEP_DISCONNECT_REQ, CsrBtCmBnepDisconnectReqSizeof, CsrBtCmBnepDisconnectReqSer, CsrBtCmBnepDisconnectReqDes, CsrBtCmBnepDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES
    { CSR_BT_CM_BNEP_DISCONNECT_RES, CsrBtCmBnepDisconnectResSizeof, CsrBtCmBnepDisconnectResSer, CsrBtCmBnepDisconnectResDes, CsrBtCmBnepDisconnectResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND
    { CSR_BT_CM_BNEP_EXTENDED_DATA_IND, CsrBtCmBnepExtendedDataIndSizeof, CsrBtCmBnepExtendedDataIndSer, CsrBtCmBnepExtendedDataIndDes, CsrBtCmBnepExtendedDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ
    { CSR_BT_CM_BNEP_EXTENDED_DATA_REQ, CsrBtCmBnepExtendedDataReqSizeof, CsrBtCmBnepExtendedDataReqSer, CsrBtCmBnepExtendedDataReqDes, CsrBtCmBnepExtendedDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ
    { CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ, CsrBtCmBnepExtendedMulticastDataReqSizeof, CsrBtCmBnepExtendedMulticastDataReqSer, CsrBtCmBnepExtendedMulticastDataReqDes, CsrBtCmBnepExtendedMulticastDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND
    { CSR_BT_CM_BNEP_MODE_CHANGE_IND, CsrBtCmBnepModeChangeIndSizeof, CsrBtCmBnepModeChangeIndSer, CsrBtCmBnepModeChangeIndDes, CsrBtCmBnepModeChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ
    { CSR_BT_CM_BNEP_MODE_CHANGE_REQ, CsrBtCmBnepModeChangeReqSizeof, CsrBtCmBnepModeChangeReqSer, CsrBtCmBnepModeChangeReqDes, CsrBtCmBnepModeChangeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ
    { CSR_BT_CM_BNEP_REGISTER_REQ, CsrBtCmBnepRegisterReqSizeof, CsrBtCmBnepRegisterReqSer, CsrBtCmBnepRegisterReqDes, CsrBtCmBnepRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND
    { CSR_BT_CM_BNEP_SWITCH_ROLE_IND, CsrBtCmBnepSwitchRoleIndSizeof, CsrBtCmBnepSwitchRoleIndSer, CsrBtCmBnepSwitchRoleIndDes, CsrBtCmBnepSwitchRoleIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ
    { CSR_BT_CM_BNEP_SWITCH_ROLE_REQ, CsrBtCmBnepSwitchRoleReqSizeof, CsrBtCmBnepSwitchRoleReqSer, CsrBtCmBnepSwitchRoleReqDes, CsrBtCmBnepSwitchRoleReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM
    { CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM, CsrBtCmCancelAcceptConnectCfmSizeof, CsrBtCmCancelAcceptConnectCfmSer, CsrBtCmCancelAcceptConnectCfmDes, CsrBtCmCancelAcceptConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ
    { CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ, CsrBtCmCancelAcceptConnectReqSizeof, CsrBtCmCancelAcceptConnectReqSer, CsrBtCmCancelAcceptConnectReqDes, CsrBtCmCancelAcceptConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ
    { CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ, CsrBtCmCancelBnepConnectReqSizeof, CsrBtCmCancelBnepConnectReqSer, CsrBtCmCancelBnepConnectReqDes, CsrBtCmCancelBnepConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ
    { CSR_BT_CM_CANCEL_CONNECT_REQ, CsrBtCmCancelConnectReqSizeof, CsrBtCmCancelConnectReqSer, CsrBtCmCancelConnectReqDes, CsrBtCmCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ
    { CSR_BT_CM_CANCEL_INQUIRY_REQ, CsrBtCmCancelInquiryReqSizeof, CsrBtCmCancelInquiryReqSer, CsrBtCmCancelInquiryReqDes, CsrBtCmCancelInquiryReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ
    { CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ, CsrBtCmCancelL2caConnectReqSizeof, CsrBtCmCancelL2caConnectReqSer, CsrBtCmCancelL2caConnectReqDes, CsrBtCmCancelL2caConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ
    { CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ, CsrBtCmCancelReadRemoteNameReqSizeof, CsrBtCmCancelReadRemoteNameReqSer, CsrBtCmCancelReadRemoteNameReqDes, CsrBtCmCancelReadRemoteNameReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM
    { CSR_BT_CM_CLEAR_EVENT_FILTER_CFM, CsrBtCmClearEventFilterCfmSizeof, CsrBtCmClearEventFilterCfmSer, CsrBtCmClearEventFilterCfmDes, CsrBtCmClearEventFilterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ
    { CSR_BT_CM_CLEAR_EVENT_FILTER_REQ, CsrBtCmClearEventFilterReqSizeof, CsrBtCmClearEventFilterReqSer, CsrBtCmClearEventFilterReqDes, CsrBtCmClearEventFilterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM
    { CSR_BT_CM_CONNECT_ACCEPT_CFM, CsrBtCmConnectAcceptCfmSizeof, CsrBtCmConnectAcceptCfmSer, CsrBtCmConnectAcceptCfmDes, CsrBtCmConnectAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ
    { CSR_BT_CM_CONNECT_ACCEPT_REQ, CsrBtCmConnectAcceptReqSizeof, CsrBtCmConnectAcceptReqSer, CsrBtCmConnectAcceptReqDes, CsrBtCmConnectAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CONNECT_CFM
    { CSR_BT_CM_CONNECT_CFM, CsrBtCmConnectCfmSizeof, CsrBtCmConnectCfmSer, CsrBtCmConnectCfmDes, CsrBtCmConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ
    { CSR_BT_CM_CONNECT_EXT_REQ, CsrBtCmConnectExtReqSizeof, CsrBtCmConnectExtReqSer, CsrBtCmConnectExtReqDes, CsrBtCmConnectExtReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CONNECT_REQ
    { CSR_BT_CM_CONNECT_REQ, CsrBtCmConnectReqSizeof, CsrBtCmConnectReqSer, CsrBtCmConnectReqDes, CsrBtCmConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ
    { CSR_BT_CM_CONNECTABLE_REQ, CsrBtCmConnectableReqSizeof, CsrBtCmConnectableReqSer, CsrBtCmConnectableReqDes, CsrBtCmConnectableReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND
    { CSR_BT_CM_CONNECTION_REJ_SECURITY_IND, CsrBtCmConnectionRejSecurityIndSizeof, CsrBtCmConnectionRejSecurityIndSer, CsrBtCmConnectionRejSecurityIndDes, CsrBtCmConnectionRejSecurityIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND */
#ifndef EXCLUDE_CSR_BT_CM_CONTROL_IND
    { CSR_BT_CM_CONTROL_IND, CsrBtCmControlIndSizeof, CsrBtCmControlIndSer, CsrBtCmControlIndDes, CsrBtCmControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_CM_CONTROL_REQ
    { CSR_BT_CM_CONTROL_REQ, CsrBtCmControlReqSizeof, CsrBtCmControlReqSer, CsrBtCmControlReqDes, CsrBtCmControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM
    { CSR_BT_CM_DATA_BUFFER_EMPTY_CFM, CsrBtCmDataBufferEmptyCfmSizeof, CsrBtCmDataBufferEmptyCfmSer, CsrBtCmDataBufferEmptyCfmDes, CsrBtCmDataBufferEmptyCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM */
#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ
    { CSR_BT_CM_DATA_BUFFER_EMPTY_REQ, CsrBtCmDataBufferEmptyReqSizeof, CsrBtCmDataBufferEmptyReqSer, CsrBtCmDataBufferEmptyReqDes, CsrBtCmDataBufferEmptyReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DATA_CFM
    { CSR_BT_CM_DATA_CFM, CsrBtCmDataCfmSizeof, CsrBtCmDataCfmSer, CsrBtCmDataCfmDes, CsrBtCmDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_CM_DATA_IND
    { CSR_BT_CM_DATA_IND, CsrBtCmDataIndSizeof, CsrBtCmDataIndSer, CsrBtCmDataIndDes, CsrBtCmDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_DATA_REQ
    { CSR_BT_CM_DATA_REQ, CsrBtCmDataReqSizeof, CsrBtCmDataReqSer, CsrBtCmDataReqDes, CsrBtCmDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DATA_RES
    { CSR_BT_CM_DATA_RES, CsrBtCmDataResSizeof, CsrBtCmDataResSer, CsrBtCmDataResDes, CsrBtCmDataResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DATA_RES */
#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM
    { CSR_BT_CM_DISABLE_DUT_MODE_CFM, CsrBtCmDisableDutModeCfmSizeof, CsrBtCmDisableDutModeCfmSer, CsrBtCmDisableDutModeCfmDes, CsrBtCmDisableDutModeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ
    { CSR_BT_CM_DISABLE_DUT_MODE_REQ, CsrBtCmDisableDutModeReqSizeof, CsrBtCmDisableDutModeReqSer, CsrBtCmDisableDutModeReqDes, CsrBtCmDisableDutModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_IND
    { CSR_BT_CM_DISCONNECT_IND, CsrBtCmDisconnectIndSizeof, CsrBtCmDisconnectIndSer, CsrBtCmDisconnectIndDes, CsrBtCmDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_REQ
    { CSR_BT_CM_DISCONNECT_REQ, CsrBtCmDisconnectReqSizeof, CsrBtCmDisconnectReqSer, CsrBtCmDisconnectReqDes, CsrBtCmDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ
    { CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ, CsrBtCmDmBnepModeSettingsReqSizeof, CsrBtCmDmBnepModeSettingsReqSer, CsrBtCmDmBnepModeSettingsReqDes, CsrBtCmDmBnepModeSettingsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ
    { CSR_BT_CM_DM_CHECK_SSR_REQ, CsrBtCmDmCheckSsrReqSizeof, CsrBtCmDmCheckSsrReqSer, CsrBtCmDmCheckSsrReqDes, CsrBtCmDmCheckSsrReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING
    { CSR_BT_CM_DM_HOUSE_CLEANING, CsrBtCmDmHouseCleaningSizeof, CsrBtCmDmHouseCleaningSer, CsrBtCmDmHouseCleaningDes, CsrBtCmDmHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ
    { CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ, CsrBtCmDmL2caModeSettingsReqSizeof, CsrBtCmDmL2caModeSettingsReqSer, CsrBtCmDmL2caModeSettingsReqDes, CsrBtCmDmL2caModeSettingsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ
    { CSR_BT_CM_DM_MODE_CHANGE_REQ, CsrBtCmDmModeChangeReqSizeof, CsrBtCmDmModeChangeReqSer, CsrBtCmDmModeChangeReqDes, CsrBtCmDmModeChangeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ
    { CSR_BT_CM_DM_MODE_SETTINGS_REQ, CsrBtCmDmModeSettingsReqSizeof, CsrBtCmDmModeSettingsReqSer, CsrBtCmDmModeSettingsReqDes, CsrBtCmDmModeSettingsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ
    { CSR_BT_CM_DM_SWITCH_ROLE_REQ, CsrBtCmDmSwitchRoleReqSizeof, CsrBtCmDmSwitchRoleReqSer, CsrBtCmDmSwitchRoleReqDes, CsrBtCmDmSwitchRoleReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ
    { CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ, CsrBtCmDmUpdateAndClearCachedParamReqSizeof, CsrBtCmDmUpdateAndClearCachedParamReqSer, CsrBtCmDmUpdateAndClearCachedParamReqDes, CsrBtCmDmUpdateAndClearCachedParamReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ
    { CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ, CsrBtCmDmWriteAutoFlushTimeoutReqSizeof, CsrBtCmDmWriteAutoFlushTimeoutReqSer, CsrBtCmDmWriteAutoFlushTimeoutReqDes, CsrBtCmDmWriteAutoFlushTimeoutReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ
    { CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ, CsrBtCmDmWriteCacheParamsReqSizeof, CsrBtCmDmWriteCacheParamsReqSer, CsrBtCmDmWriteCacheParamsReqDes, CsrBtCmDmWriteCacheParamsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ
    { CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ, CsrBtCmDmWriteLpSettingsReqSizeof, CsrBtCmDmWriteLpSettingsReqSer, CsrBtCmDmWriteLpSettingsReqDes, CsrBtCmDmWriteLpSettingsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM
    { CSR_BT_CM_EIR_FLAGS_CFM, CsrBtCmEirFlagsCfmSizeof, CsrBtCmEirFlagsCfmSer, CsrBtCmEirFlagsCfmDes, CsrBtCmEirFlagsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM */
#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ
    { CSR_BT_CM_EIR_FLAGS_REQ, CsrBtCmEirFlagsReqSizeof, CsrBtCmEirFlagsReqSer, CsrBtCmEirFlagsReqDes, CsrBtCmEirFlagsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM
    { CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM, CsrBtCmEirUpdateManufacturerDataCfmSizeof, CsrBtCmEirUpdateManufacturerDataCfmSer, CsrBtCmEirUpdateManufacturerDataCfmDes, CsrBtCmEirUpdateManufacturerDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ
    { CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ, CsrBtCmEirUpdateManufacturerDataReqSizeof, CsrBtCmEirUpdateManufacturerDataReqSer, CsrBtCmEirUpdateManufacturerDataReqDes, CsrBtCmEirUpdateManufacturerDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ
    { CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ, CsrBtCmEnEnableEnhancementsReqSizeof, CsrBtCmEnEnableEnhancementsReqSer, CsrBtCmEnEnableEnhancementsReqDes, CsrBtCmEnEnableEnhancementsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM
    { CSR_BT_CM_ENABLE_DUT_MODE_CFM, CsrBtCmEnableDutModeCfmSizeof, CsrBtCmEnableDutModeCfmSer, CsrBtCmEnableDutModeCfmDes, CsrBtCmEnableDutModeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ
    { CSR_BT_CM_ENABLE_DUT_MODE_REQ, CsrBtCmEnableDutModeReqSizeof, CsrBtCmEnableDutModeReqSer, CsrBtCmEnableDutModeReqDes, CsrBtCmEnableDutModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND
    { CSR_BT_CM_ENCRYPT_CHANGE_IND, CsrBtCmEncryptChangeIndSizeof, CsrBtCmEncryptChangeIndSer, CsrBtCmEncryptChangeIndDes, CsrBtCmEncryptChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND
    { CSR_BT_CM_EXT_SYNC_CONNECT_IND, CsrBtCmExtSyncConnectIndSizeof, CsrBtCmExtSyncConnectIndSer, CsrBtCmExtSyncConnectIndDes, CsrBtCmExtSyncConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM
    { CSR_BT_CM_GET_LINK_QUALITY_CFM, CsrBtCmGetLinkQualityCfmSizeof, CsrBtCmGetLinkQualityCfmSer, CsrBtCmGetLinkQualityCfmDes, CsrBtCmGetLinkQualityCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM */
#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ
    { CSR_BT_CM_GET_LINK_QUALITY_REQ, CsrBtCmGetLinkQualityReqSizeof, CsrBtCmGetLinkQualityReqSer, CsrBtCmGetLinkQualityReqDes, CsrBtCmGetLinkQualityReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND
    { CSR_BT_CM_GET_SECURITY_CONF_IND, CsrBtCmGetSecurityConfIndSizeof, CsrBtCmGetSecurityConfIndSer, CsrBtCmGetSecurityConfIndDes, CsrBtCmGetSecurityConfIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND */
#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES
    { CSR_BT_CM_GET_SECURITY_CONF_RES, CsrBtCmGetSecurityConfResSizeof, CsrBtCmGetSecurityConfResSer, CsrBtCmGetSecurityConfResDes, CsrBtCmGetSecurityConfResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES */
#ifndef EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND
    { CSR_BT_CM_HIGH_PRIORITY_DATA_IND, CsrBtCmHighPriorityDataIndSizeof, CsrBtCmHighPriorityDataIndSer, CsrBtCmHighPriorityDataIndDes, CsrBtCmHighPriorityDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ
    { CSR_BT_CM_INCOMING_SCO_REQ, CsrBtCmIncomingScoReqSizeof, CsrBtCmIncomingScoReqSer, CsrBtCmIncomingScoReqDes, CsrBtCmIncomingScoReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ */
#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_CFM
    { CSR_BT_CM_INQUIRY_CFM, CsrBtCmInquiryCfmSizeof, CsrBtCmInquiryCfmSer, CsrBtCmInquiryCfmDes, CsrBtCmInquiryCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_CFM */
#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND
    { CSR_BT_CM_INQUIRY_PAGE_EVENT_IND, CsrBtCmInquiryPageEventIndSizeof, CsrBtCmInquiryPageEventIndSer, CsrBtCmInquiryPageEventIndDes, CsrBtCmInquiryPageEventIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND */
#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_REQ
    { CSR_BT_CM_INQUIRY_REQ, CsrBtCmInquiryReqSizeof, CsrBtCmInquiryReqSer, CsrBtCmInquiryReqDes, CsrBtCmInquiryReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND
    { CSR_BT_CM_INQUIRY_RESULT_IND, CsrBtCmInquiryResultIndSizeof, CsrBtCmInquiryResultIndSer, CsrBtCmInquiryResultIndDes, CsrBtCmInquiryResultIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM
    { CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM, CsrBtCmL2caCancelConnectAcceptCfmSizeof, CsrBtCmL2caCancelConnectAcceptCfmSer, CsrBtCmL2caCancelConnectAcceptCfmDes, CsrBtCmL2caCancelConnectAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ
    { CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ, CsrBtCmL2caCancelConnectAcceptReqSizeof, CsrBtCmL2caCancelConnectAcceptReqSer, CsrBtCmL2caCancelConnectAcceptReqDes, CsrBtCmL2caCancelConnectAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM
    { CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM, CsrBtCmL2caConnectAcceptCfmSizeof, CsrBtCmL2caConnectAcceptCfmSer, CsrBtCmL2caConnectAcceptCfmDes, CsrBtCmL2caConnectAcceptCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ
    { CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ, CsrBtCmL2caConnectAcceptReqSizeof, CsrBtCmL2caConnectAcceptReqSer, CsrBtCmL2caConnectAcceptReqDes, CsrBtCmL2caConnectAcceptReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM
    { CSR_BT_CM_L2CA_CONNECT_CFM, CsrBtCmL2caConnectCfmSizeof, CsrBtCmL2caConnectCfmSer, CsrBtCmL2caConnectCfmDes, CsrBtCmL2caConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ
    { CSR_BT_CM_L2CA_CONNECT_REQ, CsrBtCmL2caConnectReqSizeof, CsrBtCmL2caConnectReqSer, CsrBtCmL2caConnectReqDes, CsrBtCmL2caConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM
    { CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM, CsrBtCmL2caConnectionlessDataCfmSizeof, CsrBtCmL2caConnectionlessDataCfmSer, CsrBtCmL2caConnectionlessDataCfmDes, CsrBtCmL2caConnectionlessDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND
    { CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND, CsrBtCmL2caConnectionlessDataIndSizeof, CsrBtCmL2caConnectionlessDataIndSer, CsrBtCmL2caConnectionlessDataIndDes, CsrBtCmL2caConnectionlessDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ
    { CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ, CsrBtCmL2caConnectionlessDataReqSizeof, CsrBtCmL2caConnectionlessDataReqSer, CsrBtCmL2caConnectionlessDataReqDes, CsrBtCmL2caConnectionlessDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM
    { CSR_BT_CM_L2CA_DATA_ABORT_CFM, CsrBtCmL2caDataAbortCfmSizeof, CsrBtCmL2caDataAbortCfmSer, CsrBtCmL2caDataAbortCfmDes, CsrBtCmL2caDataAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ
    { CSR_BT_CM_L2CA_DATA_ABORT_REQ, CsrBtCmL2caDataAbortReqSizeof, CsrBtCmL2caDataAbortReqSer, CsrBtCmL2caDataAbortReqDes, CsrBtCmL2caDataAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM
    { CSR_BT_CM_L2CA_DATA_CFM, CsrBtCmL2caDataCfmSizeof, CsrBtCmL2caDataCfmSer, CsrBtCmL2caDataCfmDes, CsrBtCmL2caDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_IND
    { CSR_BT_CM_L2CA_DATA_IND, CsrBtCmL2caDataIndSizeof, CsrBtCmL2caDataIndSer, CsrBtCmL2caDataIndDes, CsrBtCmL2caDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_IND */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ
    { CSR_BT_CM_L2CA_DATA_REQ, CsrBtCmL2caDataReqSizeof, CsrBtCmL2caDataReqSer, CsrBtCmL2caDataReqDes, CsrBtCmL2caDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_RES
    { CSR_BT_CM_L2CA_DATA_RES, CsrBtCmL2caDataResSizeof, CsrBtCmL2caDataResSer, CsrBtCmL2caDataResDes, CsrBtCmL2caDataResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_RES */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND
    { CSR_BT_CM_L2CA_DISCONNECT_IND, CsrBtCmL2caDisconnectIndSizeof, CsrBtCmL2caDisconnectIndSer, CsrBtCmL2caDisconnectIndDes, CsrBtCmL2caDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ
    { CSR_BT_CM_L2CA_DISCONNECT_REQ, CsrBtCmL2caDisconnectReqSizeof, CsrBtCmL2caDisconnectReqSer, CsrBtCmL2caDisconnectReqDes, CsrBtCmL2caDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM
    { CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM, CsrBtCmL2caGetChannelInfoCfmSizeof, CsrBtCmL2caGetChannelInfoCfmSer, CsrBtCmL2caGetChannelInfoCfmDes, CsrBtCmL2caGetChannelInfoCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ
    { CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ, CsrBtCmL2caGetChannelInfoReqSizeof, CsrBtCmL2caGetChannelInfoReqSer, CsrBtCmL2caGetChannelInfoReqDes, CsrBtCmL2caGetChannelInfoReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND
    { CSR_BT_CM_L2CA_MODE_CHANGE_IND, CsrBtCmL2caModeChangeIndSizeof, CsrBtCmL2caModeChangeIndSer, CsrBtCmL2caModeChangeIndDes, CsrBtCmL2caModeChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ
    { CSR_BT_CM_L2CA_MODE_CHANGE_REQ, CsrBtCmL2caModeChangeReqSizeof, CsrBtCmL2caModeChangeReqSer, CsrBtCmL2caModeChangeReqDes, CsrBtCmL2caModeChangeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM
    { CSR_BT_CM_L2CA_REGISTER_CFM, CsrBtCmL2caRegisterCfmSizeof, CsrBtCmL2caRegisterCfmSer, CsrBtCmL2caRegisterCfmDes, CsrBtCmL2caRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ
    { CSR_BT_CM_L2CA_REGISTER_REQ, CsrBtCmL2caRegisterReqSizeof, CsrBtCmL2caRegisterReqSer, CsrBtCmL2caRegisterReqDes, CsrBtCmL2caRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ
    { CSR_BT_CM_L2CA_UNREGISTER_REQ, CsrBtCmL2caUnregisterReqSizeof, CsrBtCmL2caUnregisterReqSer, CsrBtCmL2caUnregisterReqDes, CsrBtCmL2caUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND
    { CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND, CsrBtCmLeAcceptConnparamUpdateIndSizeof, CsrBtCmLeAcceptConnparamUpdateIndSer, CsrBtCmLeAcceptConnparamUpdateIndDes, CsrBtCmLeAcceptConnparamUpdateIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND */
#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES
    { CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES, CsrBtCmLeAcceptConnparamUpdateResSizeof, CsrBtCmLeAcceptConnparamUpdateResSer, CsrBtCmLeAcceptConnparamUpdateResDes, CsrBtCmLeAcceptConnparamUpdateResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES */
#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM
    { CSR_BT_CM_LE_ADVERTISE_CFM, CsrBtCmLeAdvertiseCfmSizeof, CsrBtCmLeAdvertiseCfmSer, CsrBtCmLeAdvertiseCfmDes, CsrBtCmLeAdvertiseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ
    { CSR_BT_CM_LE_ADVERTISE_REQ, CsrBtCmLeAdvertiseReqSizeof, CsrBtCmLeAdvertiseReqSer, CsrBtCmLeAdvertiseReqDes, CsrBtCmLeAdvertiseReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM
    { CSR_BT_CM_LE_CONNPARAM_CFM, CsrBtCmLeConnparamCfmSizeof, CsrBtCmLeConnparamCfmSer, CsrBtCmLeConnparamCfmDes, CsrBtCmLeConnparamCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ
    { CSR_BT_CM_LE_CONNPARAM_REQ, CsrBtCmLeConnparamReqSizeof, CsrBtCmLeConnparamReqSer, CsrBtCmLeConnparamReqDes, CsrBtCmLeConnparamReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND
    { CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND, CsrBtCmLeConnparamUpdateCmpIndSizeof, CsrBtCmLeConnparamUpdateCmpIndSer, CsrBtCmLeConnparamUpdateCmpIndDes, CsrBtCmLeConnparamUpdateCmpIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND */
#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ
    { CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ, CsrBtCmLeConnparamUpdateReqSizeof, CsrBtCmLeConnparamUpdateReqSer, CsrBtCmLeConnparamUpdateReqDes, CsrBtCmLeConnparamUpdateReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND
    { CSR_BT_CM_LE_EVENT_ADVERTISING_IND, CsrBtCmLeEventAdvertisingIndSizeof, CsrBtCmLeEventAdvertisingIndSer, CsrBtCmLeEventAdvertisingIndDes, CsrBtCmLeEventAdvertisingIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND */
#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND
    { CSR_BT_CM_LE_EVENT_CONNECTION_IND, CsrBtCmLeEventConnectionIndSizeof, CsrBtCmLeEventConnectionIndSer, CsrBtCmLeEventConnectionIndDes, CsrBtCmLeEventConnectionIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND */
#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND
    { CSR_BT_CM_LE_EVENT_SCAN_IND, CsrBtCmLeEventScanIndSizeof, CsrBtCmLeEventScanIndSer, CsrBtCmLeEventScanIndDes, CsrBtCmLeEventScanIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND */
#ifndef EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ
    { CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ, CsrBtCmLePhysicalLinkStatusReqSizeof, CsrBtCmLePhysicalLinkStatusReqSer, CsrBtCmLePhysicalLinkStatusReqDes, CsrBtCmLePhysicalLinkStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM
    { CSR_BT_CM_LE_RECEIVER_TEST_CFM, CsrBtCmLeReceiverTestCfmSizeof, CsrBtCmLeReceiverTestCfmSer, CsrBtCmLeReceiverTestCfmDes, CsrBtCmLeReceiverTestCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ
    { CSR_BT_CM_LE_RECEIVER_TEST_REQ, CsrBtCmLeReceiverTestReqSizeof, CsrBtCmLeReceiverTestReqSer, CsrBtCmLeReceiverTestReqDes, CsrBtCmLeReceiverTestReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_REPORT_IND
    { CSR_BT_CM_LE_REPORT_IND, CsrBtCmLeReportIndSizeof, CsrBtCmLeReportIndSer, CsrBtCmLeReportIndDes, CsrBtCmLeReportIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_REPORT_IND */
#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_CFM
    { CSR_BT_CM_LE_SCAN_CFM, CsrBtCmLeScanCfmSizeof, CsrBtCmLeScanCfmSer, CsrBtCmLeScanCfmDes, CsrBtCmLeScanCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_REQ
    { CSR_BT_CM_LE_SCAN_REQ, CsrBtCmLeScanReqSizeof, CsrBtCmLeScanReqSer, CsrBtCmLeScanReqDes, CsrBtCmLeScanReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM
    { CSR_BT_CM_LE_TEST_END_CFM, CsrBtCmLeTestEndCfmSizeof, CsrBtCmLeTestEndCfmSer, CsrBtCmLeTestEndCfmDes, CsrBtCmLeTestEndCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ
    { CSR_BT_CM_LE_TEST_END_REQ, CsrBtCmLeTestEndReqSizeof, CsrBtCmLeTestEndReqSer, CsrBtCmLeTestEndReqDes, CsrBtCmLeTestEndReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM
    { CSR_BT_CM_LE_TRANSMITTER_TEST_CFM, CsrBtCmLeTransmitterTestCfmSizeof, CsrBtCmLeTransmitterTestCfmSer, CsrBtCmLeTransmitterTestCfmDes, CsrBtCmLeTransmitterTestCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ
    { CSR_BT_CM_LE_TRANSMITTER_TEST_REQ, CsrBtCmLeTransmitterTestReqSizeof, CsrBtCmLeTransmitterTestReqSer, CsrBtCmLeTransmitterTestReqDes, CsrBtCmLeTransmitterTestReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM
    { CSR_BT_CM_LE_WHITELIST_SET_CFM, CsrBtCmLeWhitelistSetCfmSizeof, CsrBtCmLeWhitelistSetCfmSer, CsrBtCmLeWhitelistSetCfmDes, CsrBtCmLeWhitelistSetCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM */
#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ
    { CSR_BT_CM_LE_WHITELIST_SET_REQ, CsrBtCmLeWhitelistSetReqSizeof, CsrBtCmLeWhitelistSetReqSer, CsrBtCmLeWhitelistSetReqDes, CsrBtCmLeWhitelistSetReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND
    { CSR_BT_CM_LOCAL_NAME_CHANGE_IND, CsrBtCmLocalNameChangeIndSizeof, CsrBtCmLocalNameChangeIndSer, CsrBtCmLocalNameChangeIndDes, CsrBtCmLocalNameChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ
    { CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ, CsrBtCmLogicalChannelTypeReqSizeof, CsrBtCmLogicalChannelTypeReqSer, CsrBtCmLogicalChannelTypeReqDes, CsrBtCmLogicalChannelTypeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND
    { CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND, CsrBtCmLogicalChannelTypesIndSizeof, CsrBtCmLogicalChannelTypesIndSer, CsrBtCmLogicalChannelTypesIndDes, CsrBtCmLogicalChannelTypesIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND */
#ifndef EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND
    { CSR_BT_CM_LSTO_CHANGE_IND, CsrBtCmLstoChangeIndSizeof, CsrBtCmLstoChangeIndSer, CsrBtCmLstoChangeIndDes, CsrBtCmLstoChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND
    { CSR_BT_CM_MAP_SCO_PCM_IND, CsrBtCmMapScoPcmIndSizeof, CsrBtCmMapScoPcmIndSer, CsrBtCmMapScoPcmIndDes, CsrBtCmMapScoPcmIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND */
#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES
    { CSR_BT_CM_MAP_SCO_PCM_RES, CsrBtCmMapScoPcmResSizeof, CsrBtCmMapScoPcmResSer, CsrBtCmMapScoPcmResDes, CsrBtCmMapScoPcmResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES */
#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM
    { CSR_BT_CM_MODE_CHANGE_CFM, CsrBtCmModeChangeCfmSizeof, CsrBtCmModeChangeCfmSer, CsrBtCmModeChangeCfmDes, CsrBtCmModeChangeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM
    { CSR_BT_CM_MODE_CHANGE_CONFIG_CFM, CsrBtCmModeChangeConfigCfmSizeof, CsrBtCmModeChangeConfigCfmSer, CsrBtCmModeChangeConfigCfmDes, CsrBtCmModeChangeConfigCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM */
#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ
    { CSR_BT_CM_MODE_CHANGE_CONFIG_REQ, CsrBtCmModeChangeConfigReqSizeof, CsrBtCmModeChangeConfigReqSer, CsrBtCmModeChangeConfigReqDes, CsrBtCmModeChangeConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND
    { CSR_BT_CM_MODE_CHANGE_IND, CsrBtCmModeChangeIndSizeof, CsrBtCmModeChangeIndSer, CsrBtCmModeChangeIndDes, CsrBtCmModeChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ
    { CSR_BT_CM_MODE_CHANGE_REQ, CsrBtCmModeChangeReqSizeof, CsrBtCmModeChangeReqSer, CsrBtCmModeChangeReqDes, CsrBtCmModeChangeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM
    { CSR_BT_CM_MOVE_CHANNEL_CFM, CsrBtCmMoveChannelCfmSizeof, CsrBtCmMoveChannelCfmSer, CsrBtCmMoveChannelCfmDes, CsrBtCmMoveChannelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM */
#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND
    { CSR_BT_CM_MOVE_CHANNEL_CMP_IND, CsrBtCmMoveChannelCmpIndSizeof, CsrBtCmMoveChannelCmpIndSer, CsrBtCmMoveChannelCmpIndDes, CsrBtCmMoveChannelCmpIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND */
#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND
    { CSR_BT_CM_MOVE_CHANNEL_IND, CsrBtCmMoveChannelIndSizeof, CsrBtCmMoveChannelIndSer, CsrBtCmMoveChannelIndDes, CsrBtCmMoveChannelIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND */
#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ
    { CSR_BT_CM_MOVE_CHANNEL_REQ, CsrBtCmMoveChannelReqSizeof, CsrBtCmMoveChannelReqSer, CsrBtCmMoveChannelReqDes, CsrBtCmMoveChannelReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ */
#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES
    { CSR_BT_CM_MOVE_CHANNEL_RES, CsrBtCmMoveChannelResSizeof, CsrBtCmMoveChannelResSer, CsrBtCmMoveChannelResDes, CsrBtCmMoveChannelResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES */
#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_CFM
    { CSR_BT_CM_PORTNEG_CFM, CsrBtCmPortnegCfmSizeof, CsrBtCmPortnegCfmSer, CsrBtCmPortnegCfmDes, CsrBtCmPortnegCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_CFM */
#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_IND
    { CSR_BT_CM_PORTNEG_IND, CsrBtCmPortnegIndSizeof, CsrBtCmPortnegIndSer, CsrBtCmPortnegIndDes, CsrBtCmPortnegIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_IND */
#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_REQ
    { CSR_BT_CM_PORTNEG_REQ, CsrBtCmPortnegReqSizeof, CsrBtCmPortnegReqSer, CsrBtCmPortnegReqDes, CsrBtCmPortnegReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_REQ */
#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_RES
    { CSR_BT_CM_PORTNEG_RES, CsrBtCmPortnegResSizeof, CsrBtCmPortnegResSer, CsrBtCmPortnegResDes, CsrBtCmPortnegResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_RES */
#ifndef EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ
    { CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ, CsrBtCmPrivateSwitchRoleReqSizeof, CsrBtCmPrivateSwitchRoleReqSer, CsrBtCmPrivateSwitchRoleReqDes, CsrBtCmPrivateSwitchRoleReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM
    { CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM, CsrBtCmReadAdvertisingChTxPowerCfmSizeof, CsrBtCmReadAdvertisingChTxPowerCfmSer, CsrBtCmReadAdvertisingChTxPowerCfmDes, CsrBtCmReadAdvertisingChTxPowerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ
    { CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ, CsrBtCmReadAdvertisingChTxPowerReqSizeof, CsrBtCmReadAdvertisingChTxPowerReqSer, CsrBtCmReadAdvertisingChTxPowerReqDes, CsrBtCmReadAdvertisingChTxPowerReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM
    { CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM, CsrBtCmReadAfhChannelAssessmentModeCfmSizeof, CsrBtCmReadAfhChannelAssessmentModeCfmSer, CsrBtCmReadAfhChannelAssessmentModeCfmDes, CsrBtCmReadAfhChannelAssessmentModeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ
    { CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ, CsrBtCmReadAfhChannelAssessmentModeReqSizeof, CsrBtCmReadAfhChannelAssessmentModeReqSer, CsrBtCmReadAfhChannelAssessmentModeReqDes, CsrBtCmReadAfhChannelAssessmentModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM
    { CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM, CsrBtCmReadAfhChannelMapCfmSizeof, CsrBtCmReadAfhChannelMapCfmSer, CsrBtCmReadAfhChannelMapCfmDes, CsrBtCmReadAfhChannelMapCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ
    { CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ, CsrBtCmReadAfhChannelMapReqSizeof, CsrBtCmReadAfhChannelMapReqSer, CsrBtCmReadAfhChannelMapReqDes, CsrBtCmReadAfhChannelMapReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM
    { CSR_BT_CM_READ_CLOCK_CFM, CsrBtCmReadClockCfmSizeof, CsrBtCmReadClockCfmSer, CsrBtCmReadClockCfmDes, CsrBtCmReadClockCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ
    { CSR_BT_CM_READ_CLOCK_REQ, CsrBtCmReadClockReqSizeof, CsrBtCmReadClockReqSer, CsrBtCmReadClockReqDes, CsrBtCmReadClockReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_COD_CFM
    { CSR_BT_CM_READ_COD_CFM, CsrBtCmReadCodCfmSizeof, CsrBtCmReadCodCfmSer, CsrBtCmReadCodCfmDes, CsrBtCmReadCodCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_COD_REQ
    { CSR_BT_CM_READ_COD_REQ, CsrBtCmReadCodReqSizeof, CsrBtCmReadCodReqSer, CsrBtCmReadCodReqDes, CsrBtCmReadCodReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM
    { CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM, CsrBtCmReadEncryptionKeySizeCfmSizeof, CsrBtCmReadEncryptionKeySizeCfmSer, CsrBtCmReadEncryptionKeySizeCfmDes, CsrBtCmReadEncryptionKeySizeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ
    { CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ, CsrBtCmReadEncryptionKeySizeReqSizeof, CsrBtCmReadEncryptionKeySizeReqSer, CsrBtCmReadEncryptionKeySizeReqDes, CsrBtCmReadEncryptionKeySizeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM
    { CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM, CsrBtCmReadEncryptionStatusCfmSizeof, CsrBtCmReadEncryptionStatusCfmSer, CsrBtCmReadEncryptionStatusCfmDes, CsrBtCmReadEncryptionStatusCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ
    { CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ, CsrBtCmReadEncryptionStatusReqSizeof, CsrBtCmReadEncryptionStatusReqSer, CsrBtCmReadEncryptionStatusReqDes, CsrBtCmReadEncryptionStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM
    { CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM, CsrBtCmReadFailedContactCounterCfmSizeof, CsrBtCmReadFailedContactCounterCfmSer, CsrBtCmReadFailedContactCounterCfmDes, CsrBtCmReadFailedContactCounterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ
    { CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ, CsrBtCmReadFailedContactCounterReqSizeof, CsrBtCmReadFailedContactCounterReqSer, CsrBtCmReadFailedContactCounterReqDes, CsrBtCmReadFailedContactCounterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_CFM
    { CSR_BT_CM_READ_IAC_CFM, CsrBtCmReadIacCfmSizeof, CsrBtCmReadIacCfmSer, CsrBtCmReadIacCfmDes, CsrBtCmReadIacCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_REQ
    { CSR_BT_CM_READ_IAC_REQ, CsrBtCmReadIacReqSizeof, CsrBtCmReadIacReqSer, CsrBtCmReadIacReqDes, CsrBtCmReadIacReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM
    { CSR_BT_CM_READ_LINK_POLICY_CFM, CsrBtCmReadLinkPolicyCfmSizeof, CsrBtCmReadLinkPolicyCfmSer, CsrBtCmReadLinkPolicyCfmDes, CsrBtCmReadLinkPolicyCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ
    { CSR_BT_CM_READ_LINK_POLICY_REQ, CsrBtCmReadLinkPolicyReqSizeof, CsrBtCmReadLinkPolicyReqSer, CsrBtCmReadLinkPolicyReqDes, CsrBtCmReadLinkPolicyReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM
    { CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM, CsrBtCmReadLocalBdAddrCfmSizeof, CsrBtCmReadLocalBdAddrCfmSer, CsrBtCmReadLocalBdAddrCfmDes, CsrBtCmReadLocalBdAddrCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ
    { CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ, CsrBtCmReadLocalBdAddrReqSizeof, CsrBtCmReadLocalBdAddrReqSer, CsrBtCmReadLocalBdAddrReqDes, CsrBtCmReadLocalBdAddrReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM
    { CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM, CsrBtCmReadLocalExtFeaturesCfmSizeof, CsrBtCmReadLocalExtFeaturesCfmSer, CsrBtCmReadLocalExtFeaturesCfmDes, CsrBtCmReadLocalExtFeaturesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ
    { CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ, CsrBtCmReadLocalExtFeaturesReqSizeof, CsrBtCmReadLocalExtFeaturesReqSer, CsrBtCmReadLocalExtFeaturesReqDes, CsrBtCmReadLocalExtFeaturesReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM
    { CSR_BT_CM_READ_LOCAL_NAME_CFM, CsrBtCmReadLocalNameCfmSizeof, CsrBtCmReadLocalNameCfmSer, CsrBtCmReadLocalNameCfmDes, CsrBtCmReadLocalNameCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ
    { CSR_BT_CM_READ_LOCAL_NAME_REQ, CsrBtCmReadLocalNameReqSizeof, CsrBtCmReadLocalNameReqSer, CsrBtCmReadLocalNameReqDes, CsrBtCmReadLocalNameReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM
    { CSR_BT_CM_READ_LOCAL_VERSION_CFM, CsrBtCmReadLocalVersionCfmSizeof, CsrBtCmReadLocalVersionCfmSer, CsrBtCmReadLocalVersionCfmDes, CsrBtCmReadLocalVersionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ
    { CSR_BT_CM_READ_LOCAL_VERSION_REQ, CsrBtCmReadLocalVersionReqSizeof, CsrBtCmReadLocalVersionReqSer, CsrBtCmReadLocalVersionReqDes, CsrBtCmReadLocalVersionReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM
    { CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM, CsrBtCmReadRemoteExtFeaturesCfmSizeof, CsrBtCmReadRemoteExtFeaturesCfmSer, CsrBtCmReadRemoteExtFeaturesCfmDes, CsrBtCmReadRemoteExtFeaturesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ
    { CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ, CsrBtCmReadRemoteExtFeaturesReqSizeof, CsrBtCmReadRemoteExtFeaturesReqSer, CsrBtCmReadRemoteExtFeaturesReqDes, CsrBtCmReadRemoteExtFeaturesReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ
    { CSR_BT_CM_READ_REMOTE_FEATURES_REQ, CsrBtCmReadRemoteFeaturesReqSizeof, CsrBtCmReadRemoteFeaturesReqSer, CsrBtCmReadRemoteFeaturesReqDes, CsrBtCmReadRemoteFeaturesReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM
    { CSR_BT_CM_READ_REMOTE_NAME_CFM, CsrBtCmReadRemoteNameCfmSizeof, CsrBtCmReadRemoteNameCfmSer, CsrBtCmReadRemoteNameCfmDes, CsrBtCmReadRemoteNameCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND
    { CSR_BT_CM_READ_REMOTE_NAME_IND, CsrBtCmReadRemoteNameIndSizeof, CsrBtCmReadRemoteNameIndSer, CsrBtCmReadRemoteNameIndDes, CsrBtCmReadRemoteNameIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ
    { CSR_BT_CM_READ_REMOTE_NAME_REQ, CsrBtCmReadRemoteNameReqSizeof, CsrBtCmReadRemoteNameReqSer, CsrBtCmReadRemoteNameReqDes, CsrBtCmReadRemoteNameReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM
    { CSR_BT_CM_READ_REMOTE_VERSION_CFM, CsrBtCmReadRemoteVersionCfmSizeof, CsrBtCmReadRemoteVersionCfmSer, CsrBtCmReadRemoteVersionCfmDes, CsrBtCmReadRemoteVersionCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ
    { CSR_BT_CM_READ_REMOTE_VERSION_REQ, CsrBtCmReadRemoteVersionReqSizeof, CsrBtCmReadRemoteVersionReqSer, CsrBtCmReadRemoteVersionReqDes, CsrBtCmReadRemoteVersionReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_CFM
    { CSR_BT_CM_READ_RSSI_CFM, CsrBtCmReadRssiCfmSizeof, CsrBtCmReadRssiCfmSer, CsrBtCmReadRssiCfmDes, CsrBtCmReadRssiCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_REQ
    { CSR_BT_CM_READ_RSSI_REQ, CsrBtCmReadRssiReqSizeof, CsrBtCmReadRssiReqSer, CsrBtCmReadRssiReqDes, CsrBtCmReadRssiReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM
    { CSR_BT_CM_READ_SCAN_ENABLE_CFM, CsrBtCmReadScanEnableCfmSizeof, CsrBtCmReadScanEnableCfmSer, CsrBtCmReadScanEnableCfmDes, CsrBtCmReadScanEnableCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ
    { CSR_BT_CM_READ_SCAN_ENABLE_REQ, CsrBtCmReadScanEnableReqSizeof, CsrBtCmReadScanEnableReqSer, CsrBtCmReadScanEnableReqDes, CsrBtCmReadScanEnableReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM
    { CSR_BT_CM_READ_TX_POWER_LEVEL_CFM, CsrBtCmReadTxPowerLevelCfmSizeof, CsrBtCmReadTxPowerLevelCfmSer, CsrBtCmReadTxPowerLevelCfmDes, CsrBtCmReadTxPowerLevelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */
#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ
    { CSR_BT_CM_READ_TX_POWER_LEVEL_REQ, CsrBtCmReadTxPowerLevelReqSizeof, CsrBtCmReadTxPowerLevelReqSer, CsrBtCmReadTxPowerLevelReqDes, CsrBtCmReadTxPowerLevelReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ */
#ifndef EXCLUDE_CSR_BT_CM_REGISTER_CFM
    { CSR_BT_CM_REGISTER_CFM, CsrBtCmRegisterCfmSizeof, CsrBtCmRegisterCfmSer, CsrBtCmRegisterCfmDes, CsrBtCmRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ
    { CSR_BT_CM_REGISTER_HANDLER_REQ, CsrBtCmRegisterHandlerReqSizeof, CsrBtCmRegisterHandlerReqSer, CsrBtCmRegisterHandlerReqDes, CsrBtCmRegisterHandlerReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_REGISTER_REQ
    { CSR_BT_CM_REGISTER_REQ, CsrBtCmRegisterReqSizeof, CsrBtCmRegisterReqSer, CsrBtCmRegisterReqDes, CsrBtCmRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND
    { CSR_BT_CM_REJECT_RFC_CONNECTION_IND, CsrBtCmRejectRfcConnectionIndSizeof, CsrBtCmRejectRfcConnectionIndSer, CsrBtCmRejectRfcConnectionIndDes, CsrBtCmRejectRfcConnectionIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND */
#ifndef EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND
    { CSR_BT_CM_REMOTE_FEATURES_IND, CsrBtCmRemoteFeaturesIndSizeof, CsrBtCmRemoteFeaturesIndSer, CsrBtCmRemoteFeaturesIndDes, CsrBtCmRemoteFeaturesIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND */
#ifndef EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND
    { CSR_BT_CM_REMOTE_VERSION_IND, CsrBtCmRemoteVersionIndSizeof, CsrBtCmRemoteVersionIndSer, CsrBtCmRemoteVersionIndDes, CsrBtCmRemoteVersionIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND */
#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND
    { CSR_BT_CM_RFC_MODE_CHANGE_IND, CsrBtCmRfcModeChangeIndSizeof, CsrBtCmRfcModeChangeIndSer, CsrBtCmRfcModeChangeIndDes, CsrBtCmRfcModeChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ
    { CSR_BT_CM_RFC_MODE_CHANGE_REQ, CsrBtCmRfcModeChangeReqSizeof, CsrBtCmRfcModeChangeReqSer, CsrBtCmRfcModeChangeReqDes, CsrBtCmRfcModeChangeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND
    { CSR_BT_CM_ROLE_CHANGE_IND, CsrBtCmRoleChangeIndSizeof, CsrBtCmRoleChangeIndSer, CsrBtCmRoleChangeIndDes, CsrBtCmRoleChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM
    { CSR_BT_CM_ROLE_DISCOVERY_CFM, CsrBtCmRoleDiscoveryCfmSizeof, CsrBtCmRoleDiscoveryCfmSer, CsrBtCmRoleDiscoveryCfmDes, CsrBtCmRoleDiscoveryCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM */
#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ
    { CSR_BT_CM_ROLE_DISCOVERY_REQ, CsrBtCmRoleDiscoveryReqSizeof, CsrBtCmRoleDiscoveryReqSer, CsrBtCmRoleDiscoveryReqDes, CsrBtCmRoleDiscoveryReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ
    { CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ, CsrBtCmRoleSwitchConfigReqSizeof, CsrBtCmRoleSwitchConfigReqSer, CsrBtCmRoleSwitchConfigReqDes, CsrBtCmRoleSwitchConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM
    { CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM, CsrBtCmScoAcceptConnectCfmSizeof, CsrBtCmScoAcceptConnectCfmSer, CsrBtCmScoAcceptConnectCfmDes, CsrBtCmScoAcceptConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ
    { CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ, CsrBtCmScoAcceptConnectReqSizeof, CsrBtCmScoAcceptConnectReqSer, CsrBtCmScoAcceptConnectReqDes, CsrBtCmScoAcceptConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ
    { CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ, CsrBtCmScoCancelAcceptConnectReqSizeof, CsrBtCmScoCancelAcceptConnectReqSer, CsrBtCmScoCancelAcceptConnectReqDes, CsrBtCmScoCancelAcceptConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM
    { CSR_BT_CM_SCO_CONNECT_CFM, CsrBtCmScoConnectCfmSizeof, CsrBtCmScoConnectCfmSer, CsrBtCmScoConnectCfmDes, CsrBtCmScoConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ
    { CSR_BT_CM_SCO_CONNECT_REQ, CsrBtCmScoConnectReqSizeof, CsrBtCmScoConnectReqSer, CsrBtCmScoConnectReqDes, CsrBtCmScoConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND
    { CSR_BT_CM_SCO_DISCONNECT_IND, CsrBtCmScoDisconnectIndSizeof, CsrBtCmScoDisconnectIndSer, CsrBtCmScoDisconnectIndDes, CsrBtCmScoDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ
    { CSR_BT_CM_SCO_DISCONNECT_REQ, CsrBtCmScoDisconnectReqSizeof, CsrBtCmScoDisconnectReqSer, CsrBtCmScoDisconnectReqDes, CsrBtCmScoDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM
    { CSR_BT_CM_SCO_RENEGOTIATE_CFM, CsrBtCmScoRenegotiateCfmSizeof, CsrBtCmScoRenegotiateCfmSer, CsrBtCmScoRenegotiateCfmDes, CsrBtCmScoRenegotiateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND
    { CSR_BT_CM_SCO_RENEGOTIATE_IND, CsrBtCmScoRenegotiateIndSizeof, CsrBtCmScoRenegotiateIndSer, CsrBtCmScoRenegotiateIndDes, CsrBtCmScoRenegotiateIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND */
#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ
    { CSR_BT_CM_SCO_RENEGOTIATE_REQ, CsrBtCmScoRenegotiateReqSizeof, CsrBtCmScoRenegotiateReqSer, CsrBtCmScoRenegotiateReqDes, CsrBtCmScoRenegotiateReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM
    { CSR_BT_CM_SDC_ATTRIBUTE_CFM, CsrBtCmSdcAttributeCfmSizeof, CsrBtCmSdcAttributeCfmSer, CsrBtCmSdcAttributeCfmDes, CsrBtCmSdcAttributeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ
    { CSR_BT_CM_SDC_ATTRIBUTE_REQ, CsrBtCmSdcAttributeReqSizeof, CsrBtCmSdcAttributeReqSer, CsrBtCmSdcAttributeReqDes, CsrBtCmSdcAttributeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ
    { CSR_BT_CM_SDC_CANCEL_SEARCH_REQ, CsrBtCmSdcCancelSearchReqSizeof, CsrBtCmSdcCancelSearchReqSer, CsrBtCmSdcCancelSearchReqDes, CsrBtCmSdcCancelSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND
    { CSR_BT_CM_SDC_CLOSE_IND, CsrBtCmSdcCloseIndSizeof, CsrBtCmSdcCloseIndSer, CsrBtCmSdcCloseIndDes, CsrBtCmSdcCloseIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND */
#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ
    { CSR_BT_CM_SDC_CLOSE_REQ, CsrBtCmSdcCloseReqSizeof, CsrBtCmSdcCloseReqSer, CsrBtCmSdcCloseReqDes, CsrBtCmSdcCloseReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM
    { CSR_BT_CM_SDC_OPEN_CFM, CsrBtCmSdcOpenCfmSizeof, CsrBtCmSdcOpenCfmSer, CsrBtCmSdcOpenCfmDes, CsrBtCmSdcOpenCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ
    { CSR_BT_CM_SDC_OPEN_REQ, CsrBtCmSdcOpenReqSizeof, CsrBtCmSdcOpenReqSer, CsrBtCmSdcOpenReqDes, CsrBtCmSdcOpenReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM
    { CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM, CsrBtCmSdcReleaseResourcesCfmSizeof, CsrBtCmSdcReleaseResourcesCfmSer, CsrBtCmSdcReleaseResourcesCfmDes, CsrBtCmSdcReleaseResourcesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ
    { CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ, CsrBtCmSdcReleaseResourcesReqSizeof, CsrBtCmSdcReleaseResourcesReqSer, CsrBtCmSdcReleaseResourcesReqDes, CsrBtCmSdcReleaseResourcesReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
    { CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ, CsrBtCmSdcRfcExtendedSearchReqSizeof, CsrBtCmSdcRfcExtendedSearchReqSer, CsrBtCmSdcRfcExtendedSearchReqDes, CsrBtCmSdcRfcExtendedSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ
    { CSR_BT_CM_SDC_RFC_SEARCH_REQ, CsrBtCmSdcRfcSearchReqSizeof, CsrBtCmSdcRfcSearchReqSer, CsrBtCmSdcRfcSearchReqDes, CsrBtCmSdcRfcSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM
    { CSR_BT_CM_SDC_SEARCH_CFM, CsrBtCmSdcSearchCfmSizeof, CsrBtCmSdcSearchCfmSer, CsrBtCmSdcSearchCfmDes, CsrBtCmSdcSearchCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND
    { CSR_BT_CM_SDC_SEARCH_IND, CsrBtCmSdcSearchIndSizeof, CsrBtCmSdcSearchIndSer, CsrBtCmSdcSearchIndDes, CsrBtCmSdcSearchIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ
    { CSR_BT_CM_SDC_SEARCH_REQ, CsrBtCmSdcSearchReqSizeof, CsrBtCmSdcSearchReqSer, CsrBtCmSdcSearchReqDes, CsrBtCmSdcSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM
    { CSR_BT_CM_SDC_SERVICE_SEARCH_CFM, CsrBtCmSdcServiceSearchCfmSizeof, CsrBtCmSdcServiceSearchCfmSer, CsrBtCmSdcServiceSearchCfmDes, CsrBtCmSdcServiceSearchCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
    { CSR_BT_CM_SDC_SERVICE_SEARCH_REQ, CsrBtCmSdcServiceSearchReqSizeof, CsrBtCmSdcServiceSearchReqSer, CsrBtCmSdcServiceSearchReqDes, CsrBtCmSdcServiceSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
    { CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ, CsrBtCmSdcUuid128RfcSearchReqSizeof, CsrBtCmSdcUuid128RfcSearchReqSer, CsrBtCmSdcUuid128RfcSearchReqDes, CsrBtCmSdcUuid128RfcSearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND
    { CSR_BT_CM_SDC_UUID128_SEARCH_IND, CsrBtCmSdcUuid128SearchIndSizeof, CsrBtCmSdcUuid128SearchIndSer, CsrBtCmSdcUuid128SearchIndDes, CsrBtCmSdcUuid128SearchIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND */
#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ
    { CSR_BT_CM_SDC_UUID128_SEARCH_REQ, CsrBtCmSdcUuid128SearchReqSizeof, CsrBtCmSdcUuid128SearchReqSer, CsrBtCmSdcUuid128SearchReqDes, CsrBtCmSdcUuid128SearchReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM
    { CSR_BT_CM_SDS_EXT_REGISTER_CFM, CsrBtCmSdsExtRegisterCfmSizeof, CsrBtCmSdsExtRegisterCfmSer, CsrBtCmSdsExtRegisterCfmDes, CsrBtCmSdsExtRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM
    { CSR_BT_CM_SDS_EXT_UNREGISTER_CFM, CsrBtCmSdsExtUnregisterCfmSizeof, CsrBtCmSdsExtUnregisterCfmSer, CsrBtCmSdsExtUnregisterCfmDes, CsrBtCmSdsExtUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM
    { CSR_BT_CM_SDS_REGISTER_CFM, CsrBtCmSdsRegisterCfmSizeof, CsrBtCmSdsRegisterCfmSer, CsrBtCmSdsRegisterCfmDes, CsrBtCmSdsRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ
    { CSR_BT_CM_SDS_REGISTER_REQ, CsrBtCmSdsRegisterReqSizeof, CsrBtCmSdsRegisterReqSer, CsrBtCmSdsRegisterReqDes, CsrBtCmSdsRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM
    { CSR_BT_CM_SDS_UNREGISTER_CFM, CsrBtCmSdsUnregisterCfmSizeof, CsrBtCmSdsUnregisterCfmSer, CsrBtCmSdsUnregisterCfmDes, CsrBtCmSdsUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ
    { CSR_BT_CM_SDS_UNREGISTER_REQ, CsrBtCmSdsUnregisterReqSizeof, CsrBtCmSdsUnregisterReqSer, CsrBtCmSdsUnregisterReqDes, CsrBtCmSdsUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM
    { CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM, CsrBtCmSetAfhChannelClassCfmSizeof, CsrBtCmSetAfhChannelClassCfmSer, CsrBtCmSetAfhChannelClassCfmDes, CsrBtCmSetAfhChannelClassCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ
    { CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ, CsrBtCmSetAfhChannelClassReqSizeof, CsrBtCmSetAfhChannelClassReqSer, CsrBtCmSetAfhChannelClassReqDes, CsrBtCmSetAfhChannelClassReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM
    { CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM, CsrBtCmSetEventFilterBdaddrCfmSizeof, CsrBtCmSetEventFilterBdaddrCfmSer, CsrBtCmSetEventFilterBdaddrCfmDes, CsrBtCmSetEventFilterBdaddrCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ
    { CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ, CsrBtCmSetEventFilterBdaddrReqSizeof, CsrBtCmSetEventFilterBdaddrReqSer, CsrBtCmSetEventFilterBdaddrReqDes, CsrBtCmSetEventFilterBdaddrReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM
    { CSR_BT_CM_SET_EVENT_FILTER_COD_CFM, CsrBtCmSetEventFilterCodCfmSizeof, CsrBtCmSetEventFilterCodCfmSer, CsrBtCmSetEventFilterCodCfmDes, CsrBtCmSetEventFilterCodCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ
    { CSR_BT_CM_SET_EVENT_FILTER_COD_REQ, CsrBtCmSetEventFilterCodReqSizeof, CsrBtCmSetEventFilterCodReqSer, CsrBtCmSetEventFilterCodReqDes, CsrBtCmSetEventFilterCodReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM
    { CSR_BT_CM_SET_EVENT_MASK_CFM, CsrBtCmSetEventMaskCfmSizeof, CsrBtCmSetEventMaskCfmSer, CsrBtCmSetEventMaskCfmDes, CsrBtCmSetEventMaskCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ
    { CSR_BT_CM_SET_EVENT_MASK_REQ, CsrBtCmSetEventMaskReqSizeof, CsrBtCmSetEventMaskReqSer, CsrBtCmSetEventMaskReqDes, CsrBtCmSetEventMaskReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM
    { CSR_BT_CM_SET_LOCAL_NAME_CFM, CsrBtCmSetLocalNameCfmSizeof, CsrBtCmSetLocalNameCfmSer, CsrBtCmSetLocalNameCfmDes, CsrBtCmSetLocalNameCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ
    { CSR_BT_CM_SET_LOCAL_NAME_REQ, CsrBtCmSetLocalNameReqSizeof, CsrBtCmSetLocalNameReqSer, CsrBtCmSetLocalNameReqDes, CsrBtCmSetLocalNameReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ
    { CSR_BT_CM_SM_ACCESS_REQ, CsrBtCmSmAccessReqSizeof, CsrBtCmSmAccessReqSer, CsrBtCmSmAccessReqDes, CsrBtCmSmAccessReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ
    { CSR_BT_CM_SM_ACL_CLOSE_REQ, CsrBtCmSmAclCloseReqSizeof, CsrBtCmSmAclCloseReqSer, CsrBtCmSmAclCloseReqDes, CsrBtCmSmAclCloseReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ
    { CSR_BT_CM_SM_ACL_OPEN_REQ, CsrBtCmSmAclOpenReqSizeof, CsrBtCmSmAclOpenReqSer, CsrBtCmSmAclOpenReqDes, CsrBtCmSmAclOpenReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ
    { CSR_BT_CM_SM_AUTHENTICATE_REQ, CsrBtCmSmAuthenticateReqSizeof, CsrBtCmSmAuthenticateReqSer, CsrBtCmSmAuthenticateReqDes, CsrBtCmSmAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES
    { CSR_BT_CM_SM_AUTHORISE_RES, CsrBtCmSmAuthoriseResSizeof, CsrBtCmSmAuthoriseResSer, CsrBtCmSmAuthoriseResDes, CsrBtCmSmAuthoriseResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ
    { CSR_BT_CM_SM_BONDING_CANCEL_REQ, CsrBtCmSmBondingCancelReqSizeof, CsrBtCmSmBondingCancelReqSer, CsrBtCmSmBondingCancelReqDes, CsrBtCmSmBondingCancelReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_REQ
    { CSR_BT_CM_SM_BONDING_REQ, CsrBtCmSmBondingReqSizeof, CsrBtCmSmBondingReqSer, CsrBtCmSmBondingReqDes, CsrBtCmSmBondingReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ
    { CSR_BT_CM_SM_CANCEL_CONNECT_REQ, CsrBtCmSmCancelConnectReqSizeof, CsrBtCmSmCancelConnectReqSer, CsrBtCmSmCancelConnectReqDes, CsrBtCmSmCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ
    { CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ, CsrBtCmSmDeleteStoreLinkKeyReqSizeof, CsrBtCmSmDeleteStoreLinkKeyReqSer, CsrBtCmSmDeleteStoreLinkKeyReqDes, CsrBtCmSmDeleteStoreLinkKeyReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ
    { CSR_BT_CM_SM_ENCRYPTION_REQ, CsrBtCmSmEncryptionReqSizeof, CsrBtCmSmEncryptionReqSer, CsrBtCmSmEncryptionReqDes, CsrBtCmSmEncryptionReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING
    { CSR_BT_CM_SM_HOUSE_CLEANING, CsrBtCmSmHouseCleaningSizeof, CsrBtCmSmHouseCleaningSer, CsrBtCmSmHouseCleaningDes, CsrBtCmSmHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES
    { CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES, CsrBtCmSmIoCapabilityRequestNegResSizeof, CsrBtCmSmIoCapabilityRequestNegResSer, CsrBtCmSmIoCapabilityRequestNegResDes, CsrBtCmSmIoCapabilityRequestNegResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES
    { CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES, CsrBtCmSmIoCapabilityRequestResSizeof, CsrBtCmSmIoCapabilityRequestResSer, CsrBtCmSmIoCapabilityRequestResDes, CsrBtCmSmIoCapabilityRequestResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES
    { CSR_BT_CM_SM_KEY_REQUEST_RES, CsrBtCmSmKeyRequestResSizeof, CsrBtCmSmKeyRequestResSer, CsrBtCmSmKeyRequestResDes, CsrBtCmSmKeyRequestResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ
    { CSR_BT_CM_SM_LE_SECURITY_REQ, CsrBtCmSmLeSecurityReqSizeof, CsrBtCmSmLeSecurityReqSer, CsrBtCmSmLeSecurityReqDes, CsrBtCmSmLeSecurityReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES
    { CSR_BT_CM_SM_LINK_KEY_REQUEST_RES, CsrBtCmSmLinkKeyRequestResSizeof, CsrBtCmSmLinkKeyRequestResSer, CsrBtCmSmLinkKeyRequestResDes, CsrBtCmSmLinkKeyRequestResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES
    { CSR_BT_CM_SM_PIN_REQUEST_RES, CsrBtCmSmPinRequestResSizeof, CsrBtCmSmPinRequestResSer, CsrBtCmSmPinRequestResDes, CsrBtCmSmPinRequestResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ
    { CSR_BT_CM_SM_READ_DEVICE_REQ, CsrBtCmSmReadDeviceReqSizeof, CsrBtCmSmReadDeviceReqSer, CsrBtCmSmReadDeviceReqDes, CsrBtCmSmReadDeviceReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ
    { CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ, CsrBtCmSmReadLocalOobDataReqSizeof, CsrBtCmSmReadLocalOobDataReqSer, CsrBtCmSmReadLocalOobDataReqDes, CsrBtCmSmReadLocalOobDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ
    { CSR_BT_CM_SM_REGISTER_REQ, CsrBtCmSmRegisterReqSizeof, CsrBtCmSmRegisterReqSer, CsrBtCmSmRegisterReqDes, CsrBtCmSmRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ
    { CSR_BT_CM_SM_REMOVE_DEVICE_REQ, CsrBtCmSmRemoveDeviceReqSizeof, CsrBtCmSmRemoveDeviceReqSer, CsrBtCmSmRemoveDeviceReqDes, CsrBtCmSmRemoveDeviceReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_IND
    { CSR_BT_CM_SM_REPAIR_IND, CsrBtCmSmRepairIndSizeof, CsrBtCmSmRepairIndSer, CsrBtCmSmRepairIndDes, CsrBtCmSmRepairIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_IND */
#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_RES
    { CSR_BT_CM_SM_REPAIR_RES, CsrBtCmSmRepairResSizeof, CsrBtCmSmRepairResSer, CsrBtCmSmRepairResDes, CsrBtCmSmRepairResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ
    { CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ, CsrBtCmSmSecModeConfigReqSizeof, CsrBtCmSmSecModeConfigReqSer, CsrBtCmSmSecModeConfigReqDes, CsrBtCmSmSecModeConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
    { CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ, CsrBtCmSmSendKeypressNotificationReqSizeof, CsrBtCmSmSendKeypressNotificationReqSer, CsrBtCmSmSendKeypressNotificationReqDes, CsrBtCmSmSendKeypressNotificationReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ
    { CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ, CsrBtCmSmSetDefaultSecLevelReqSizeof, CsrBtCmSmSetDefaultSecLevelReqSer, CsrBtCmSmSetDefaultSecLevelReqDes, CsrBtCmSmSetDefaultSecLevelReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ
    { CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ, CsrBtCmSmSetEncryptionKeySizeReqSizeof, CsrBtCmSmSetEncryptionKeySizeReqSer, CsrBtCmSmSetEncryptionKeySizeReqDes, CsrBtCmSmSetEncryptionKeySizeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ
    { CSR_BT_CM_SM_SET_SEC_MODE_REQ, CsrBtCmSmSetSecModeReqSizeof, CsrBtCmSmSetSecModeReqSer, CsrBtCmSmSetSecModeReqDes, CsrBtCmSmSetSecModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ
    { CSR_BT_CM_SM_UNREGISTER_REQ, CsrBtCmSmUnregisterReqSizeof, CsrBtCmSmUnregisterReqSer, CsrBtCmSmUnregisterReqDes, CsrBtCmSmUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES
    { CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES, CsrBtCmSmUserConfirmationRequestNegResSizeof, CsrBtCmSmUserConfirmationRequestNegResSer, CsrBtCmSmUserConfirmationRequestNegResDes, CsrBtCmSmUserConfirmationRequestNegResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES
    { CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES, CsrBtCmSmUserConfirmationRequestResSizeof, CsrBtCmSmUserConfirmationRequestResSer, CsrBtCmSmUserConfirmationRequestResDes, CsrBtCmSmUserConfirmationRequestResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES
    { CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES, CsrBtCmSmUserPasskeyRequestNegResSizeof, CsrBtCmSmUserPasskeyRequestNegResSer, CsrBtCmSmUserPasskeyRequestNegResDes, CsrBtCmSmUserPasskeyRequestNegResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES */
#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES
    { CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES, CsrBtCmSmUserPasskeyRequestResSizeof, CsrBtCmSmUserPasskeyRequestResSer, CsrBtCmSmUserPasskeyRequestResDes, CsrBtCmSmUserPasskeyRequestResSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND
    { CSR_BT_CM_SNIFF_SUB_RATING_IND, CsrBtCmSniffSubRatingIndSizeof, CsrBtCmSniffSubRatingIndSer, CsrBtCmSniffSubRatingIndDes, CsrBtCmSniffSubRatingIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND */
#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM
    { CSR_BT_CM_SWITCH_ROLE_CFM, CsrBtCmSwitchRoleCfmSizeof, CsrBtCmSwitchRoleCfmSer, CsrBtCmSwitchRoleCfmDes, CsrBtCmSwitchRoleCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ
    { CSR_BT_CM_SWITCH_ROLE_REQ, CsrBtCmSwitchRoleReqSizeof, CsrBtCmSwitchRoleReqSer, CsrBtCmSwitchRoleReqDes, CsrBtCmSwitchRoleReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND
    { CSR_BT_CM_SYNC_CONNECT_IND, CsrBtCmSyncConnectIndSizeof, CsrBtCmSyncConnectIndSer, CsrBtCmSyncConnectIndDes, CsrBtCmSyncConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND
    { CSR_BT_CM_SYNC_DISCONNECT_IND, CsrBtCmSyncDisconnectIndSizeof, CsrBtCmSyncDisconnectIndSer, CsrBtCmSyncDisconnectIndDes, CsrBtCmSyncDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND
    { CSR_BT_CM_SYNC_RENEGOTIATE_IND, CsrBtCmSyncRenegotiateIndSizeof, CsrBtCmSyncRenegotiateIndSer, CsrBtCmSyncRenegotiateIndDes, CsrBtCmSyncRenegotiateIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND */
#ifndef EXCLUDE_CSR_BT_CM_UNREGISTER_REQ
    { CSR_BT_CM_UNREGISTER_REQ, CsrBtCmUnregisterReqSizeof, CsrBtCmUnregisterReqSer, CsrBtCmUnregisterReqDes, CsrBtCmUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_UNREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM
    { CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM, CsrBtCmWriteAfhChannelAssessmentModeCfmSizeof, CsrBtCmWriteAfhChannelAssessmentModeCfmSer, CsrBtCmWriteAfhChannelAssessmentModeCfmDes, CsrBtCmWriteAfhChannelAssessmentModeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ
    { CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ, CsrBtCmWriteAfhChannelAssessmentModeReqSizeof, CsrBtCmWriteAfhChannelAssessmentModeReqSer, CsrBtCmWriteAfhChannelAssessmentModeReqDes, CsrBtCmWriteAfhChannelAssessmentModeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ
    { CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ, CsrBtCmWriteClassOfDeviceReqSizeof, CsrBtCmWriteClassOfDeviceReqSer, CsrBtCmWriteClassOfDeviceReqDes, CsrBtCmWriteClassOfDeviceReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_CFM
    { CSR_BT_CM_WRITE_COD_CFM, CsrBtCmWriteCodCfmSizeof, CsrBtCmWriteCodCfmSer, CsrBtCmWriteCodCfmDes, CsrBtCmWriteCodCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_REQ
    { CSR_BT_CM_WRITE_COD_REQ, CsrBtCmWriteCodReqSizeof, CsrBtCmWriteCodReqSer, CsrBtCmWriteCodReqDes, CsrBtCmWriteCodReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_IND
    { CSR_BT_CM_WRITE_IAC_IND, CsrBtCmWriteIacIndSizeof, CsrBtCmWriteIacIndSer, CsrBtCmWriteIacIndDes, CsrBtCmWriteIacIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_IND */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ
    { CSR_BT_CM_WRITE_IAC_REQ, CsrBtCmWriteIacReqSizeof, CsrBtCmWriteIacReqSer, CsrBtCmWriteIacReqDes, CsrBtCmWriteIacReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM
    { CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM, CsrBtCmWriteInquiryscanSettingsCfmSizeof, CsrBtCmWriteInquiryscanSettingsCfmSer, CsrBtCmWriteInquiryscanSettingsCfmDes, CsrBtCmWriteInquiryscanSettingsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ
    { CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ, CsrBtCmWriteInquiryscanSettingsReqSizeof, CsrBtCmWriteInquiryscanSettingsReqSer, CsrBtCmWriteInquiryscanSettingsReqDes, CsrBtCmWriteInquiryscanSettingsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM
    { CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM, CsrBtCmWriteInquiryscanTypeCfmSizeof, CsrBtCmWriteInquiryscanTypeCfmSer, CsrBtCmWriteInquiryscanTypeCfmDes, CsrBtCmWriteInquiryscanTypeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ
    { CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ, CsrBtCmWriteInquiryscanTypeReqSizeof, CsrBtCmWriteInquiryscanTypeReqSer, CsrBtCmWriteInquiryscanTypeReqDes, CsrBtCmWriteInquiryscanTypeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND
    { CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND, CsrBtCmWriteLinkPolicyErrorIndSizeof, CsrBtCmWriteLinkPolicyErrorIndSer, CsrBtCmWriteLinkPolicyErrorIndDes, CsrBtCmWriteLinkPolicyErrorIndSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ
    { CSR_BT_CM_WRITE_LINK_POLICY_REQ, CsrBtCmWriteLinkPolicyReqSizeof, CsrBtCmWriteLinkPolicyReqSer, CsrBtCmWriteLinkPolicyReqDes, CsrBtCmWriteLinkPolicyReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM
    { CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM, CsrBtCmWriteLinkSupervTimeoutCfmSizeof, CsrBtCmWriteLinkSupervTimeoutCfmSer, CsrBtCmWriteLinkSupervTimeoutCfmDes, CsrBtCmWriteLinkSupervTimeoutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ
    { CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ, CsrBtCmWriteLinkSupervTimeoutReqSizeof, CsrBtCmWriteLinkSupervTimeoutReqSer, CsrBtCmWriteLinkSupervTimeoutReqDes, CsrBtCmWriteLinkSupervTimeoutReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM
    { CSR_BT_CM_WRITE_PAGE_TO_CFM, CsrBtCmWritePageToCfmSizeof, CsrBtCmWritePageToCfmSer, CsrBtCmWritePageToCfmDes, CsrBtCmWritePageToCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ
    { CSR_BT_CM_WRITE_PAGE_TO_REQ, CsrBtCmWritePageToReqSizeof, CsrBtCmWritePageToReqSer, CsrBtCmWritePageToReqDes, CsrBtCmWritePageToReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM
    { CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM, CsrBtCmWritePagescanSettingsCfmSizeof, CsrBtCmWritePagescanSettingsCfmSer, CsrBtCmWritePagescanSettingsCfmDes, CsrBtCmWritePagescanSettingsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ
    { CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ, CsrBtCmWritePagescanSettingsReqSizeof, CsrBtCmWritePagescanSettingsReqSer, CsrBtCmWritePagescanSettingsReqDes, CsrBtCmWritePagescanSettingsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM
    { CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM, CsrBtCmWritePagescanTypeCfmSizeof, CsrBtCmWritePagescanTypeCfmSer, CsrBtCmWritePagescanTypeCfmDes, CsrBtCmWritePagescanTypeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ
    { CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ, CsrBtCmWritePagescanTypeReqSizeof, CsrBtCmWritePagescanTypeReqSer, CsrBtCmWritePagescanTypeReqDes, CsrBtCmWritePagescanTypeReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM
    { CSR_BT_CM_WRITE_SCAN_ENABLE_CFM, CsrBtCmWriteScanEnableCfmSizeof, CsrBtCmWriteScanEnableCfmSer, CsrBtCmWriteScanEnableCfmDes, CsrBtCmWriteScanEnableCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ
    { CSR_BT_CM_WRITE_SCAN_ENABLE_REQ, CsrBtCmWriteScanEnableReqSizeof, CsrBtCmWriteScanEnableReqSer, CsrBtCmWriteScanEnableReqDes, CsrBtCmWriteScanEnableReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM
    { CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM, CsrBtCmWriteVoiceSettingsCfmSizeof, CsrBtCmWriteVoiceSettingsCfmSer, CsrBtCmWriteVoiceSettingsCfmDes, CsrBtCmWriteVoiceSettingsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ
    { CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ, CsrBtCmWriteVoiceSettingsReqSizeof, CsrBtCmWriteVoiceSettingsReqSer, CsrBtCmWriteVoiceSettingsReqDes, CsrBtCmWriteVoiceSettingsReqSerFree },
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtCmConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_CM_PRIM, csr_bt_cm_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_cm_conv_info = {
    CSR_BT_CM_PRIM,
    "CSR_BT_CM_PRIM",
    csr_bt_cm_conv_lut
};

CsrLogPrimitiveInformation* CsrBtCmTechInfoGet(void)
{
    return &csr_bt_cm_conv_info;
}
#endif
#endif

