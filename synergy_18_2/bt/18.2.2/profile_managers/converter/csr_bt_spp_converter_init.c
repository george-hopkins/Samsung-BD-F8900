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
#include "csr_bt_spp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SPP_MODULE
#include "csr_bt_spp_serialize.h"
#include "csr_bt_spp_prim.h"

static CsrMsgConvMsgEntry csr_bt_spp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ
    { CSR_BT_SPP_ACCEPT_AUDIO_REQ, CsrBtSppAcceptAudioReqSizeof, CsrBtSppAcceptAudioReqSer, CsrBtSppAcceptAudioReqDes, CsrBtSppAcceptAudioReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM
    { CSR_BT_SPP_ACTIVATE_CFM, CsrBtSppActivateCfmSizeof, CsrBtSppActivateCfmSer, CsrBtSppActivateCfmDes, CsrBtSppActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ
    { CSR_BT_SPP_ACTIVATE_REQ, CsrBtSppActivateReqSizeof, CsrBtSppActivateReqSer, CsrBtSppActivateReqDes, CsrBtSppActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND
    { CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND, CsrBtSppAudioAcceptConnectIndSizeof, CsrBtSppAudioAcceptConnectIndSer, CsrBtSppAudioAcceptConnectIndDes, CsrBtSppAudioAcceptConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES
    { CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES, CsrBtSppAudioAcceptConnectResSizeof, CsrBtSppAudioAcceptConnectResSer, CsrBtSppAudioAcceptConnectResDes, CsrBtSppAudioAcceptConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM
    { CSR_BT_SPP_AUDIO_CONNECT_CFM, CsrBtSppAudioConnectCfmSizeof, CsrBtSppAudioConnectCfmSer, CsrBtSppAudioConnectCfmDes, CsrBtSppAudioConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND
    { CSR_BT_SPP_AUDIO_CONNECT_IND, CsrBtSppAudioConnectIndSizeof, CsrBtSppAudioConnectIndSer, CsrBtSppAudioConnectIndDes, CsrBtSppAudioConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ
    { CSR_BT_SPP_AUDIO_CONNECT_REQ, CsrBtSppAudioConnectReqSizeof, CsrBtSppAudioConnectReqSer, CsrBtSppAudioConnectReqDes, CsrBtSppAudioConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM
    { CSR_BT_SPP_AUDIO_DISCONNECT_CFM, CsrBtSppAudioDisconnectCfmSizeof, CsrBtSppAudioDisconnectCfmSer, CsrBtSppAudioDisconnectCfmDes, CsrBtSppAudioDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND
    { CSR_BT_SPP_AUDIO_DISCONNECT_IND, CsrBtSppAudioDisconnectIndSizeof, CsrBtSppAudioDisconnectIndSer, CsrBtSppAudioDisconnectIndDes, CsrBtSppAudioDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ
    { CSR_BT_SPP_AUDIO_DISCONNECT_REQ, CsrBtSppAudioDisconnectReqSizeof, CsrBtSppAudioDisconnectReqSer, CsrBtSppAudioDisconnectReqDes, CsrBtSppAudioDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM
    { CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM, CsrBtSppAudioRenegotiateCfmSizeof, CsrBtSppAudioRenegotiateCfmSer, CsrBtSppAudioRenegotiateCfmDes, CsrBtSppAudioRenegotiateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND
    { CSR_BT_SPP_AUDIO_RENEGOTIATE_IND, CsrBtSppAudioRenegotiateIndSizeof, CsrBtSppAudioRenegotiateIndSer, CsrBtSppAudioRenegotiateIndDes, CsrBtSppAudioRenegotiateIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND */
#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ
    { CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ, CsrBtSppAudioRenegotiateReqSizeof, CsrBtSppAudioRenegotiateReqSer, CsrBtSppAudioRenegotiateReqDes, CsrBtSppAudioRenegotiateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ
    { CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ, CsrBtSppCancelAcceptAudioReqSizeof, CsrBtSppCancelAcceptAudioReqSer, CsrBtSppCancelAcceptAudioReqDes, CsrBtSppCancelAcceptAudioReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ
    { CSR_BT_SPP_CANCEL_CONNECT_REQ, CsrBtSppCancelConnectReqSizeof, CsrBtSppCancelConnectReqSer, CsrBtSppCancelConnectReqDes, CsrBtSppCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_IND
    { CSR_BT_SPP_CONNECT_IND, CsrBtSppConnectIndSizeof, CsrBtSppConnectIndSer, CsrBtSppConnectIndDes, CsrBtSppConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_REQ
    { CSR_BT_SPP_CONNECT_REQ, CsrBtSppConnectReqSizeof, CsrBtSppConnectReqSer, CsrBtSppConnectReqDes, CsrBtSppConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_IND
    { CSR_BT_SPP_CONTROL_IND, CsrBtSppControlIndSizeof, CsrBtSppControlIndSer, CsrBtSppControlIndDes, CsrBtSppControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_REQ
    { CSR_BT_SPP_CONTROL_REQ, CsrBtSppControlReqSizeof, CsrBtSppControlReqSer, CsrBtSppControlReqDes, CsrBtSppControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_DATA_CFM
    { CSR_BT_SPP_DATA_CFM, CsrBtSppDataCfmSizeof, CsrBtSppDataCfmSer, CsrBtSppDataCfmDes, CsrBtSppDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_DATA_IND
    { CSR_BT_SPP_DATA_IND, CsrBtSppDataIndSizeof, CsrBtSppDataIndSer, CsrBtSppDataIndDes, CsrBtSppDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DATA_IND */
#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND
    { CSR_BT_SPP_DATA_PATH_STATUS_IND, CsrBtSppDataPathStatusIndSizeof, CsrBtSppDataPathStatusIndSer, CsrBtSppDataPathStatusIndDes, CsrBtSppDataPathStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ
    { CSR_BT_SPP_DATA_PATH_STATUS_REQ, CsrBtSppDataPathStatusReqSizeof, CsrBtSppDataPathStatusReqSer, CsrBtSppDataPathStatusReqDes, CsrBtSppDataPathStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_DATA_REQ
    { CSR_BT_SPP_DATA_REQ, CsrBtSppDataReqSizeof, CsrBtSppDataReqSer, CsrBtSppDataReqDes, CsrBtSppDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_DATA_RES
    { CSR_BT_SPP_DATA_RES, CsrBtSppDataResSizeof, CsrBtSppDataResSer, CsrBtSppDataResDes, CsrBtSppDataResSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DATA_RES */
#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM
    { CSR_BT_SPP_DEACTIVATE_CFM, CsrBtSppDeactivateCfmSizeof, CsrBtSppDeactivateCfmSer, CsrBtSppDeactivateCfmDes, CsrBtSppDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ
    { CSR_BT_SPP_DEACTIVATE_REQ, CsrBtSppDeactivateReqSizeof, CsrBtSppDeactivateReqSer, CsrBtSppDeactivateReqDes, CsrBtSppDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_IND
    { CSR_BT_SPP_DISCONNECT_IND, CsrBtSppDisconnectIndSizeof, CsrBtSppDisconnectIndSer, CsrBtSppDisconnectIndDes, CsrBtSppDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ
    { CSR_BT_SPP_DISCONNECT_REQ, CsrBtSppDisconnectReqSizeof, CsrBtSppDisconnectReqSer, CsrBtSppDisconnectReqDes, CsrBtSppDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ
    { CSR_BT_SPP_EXTENDED_ACTIVATE_REQ, CsrBtSppExtendedActivateReqSizeof, CsrBtSppExtendedActivateReqSer, CsrBtSppExtendedActivateReqDes, CsrBtSppExtendedActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ
    { CSR_BT_SPP_EXTENDED_CONNECT_REQ, CsrBtSppExtendedConnectReqSizeof, CsrBtSppExtendedConnectReqSer, CsrBtSppExtendedConnectReqDes, CsrBtSppExtendedConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ
    { CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ, CsrBtSppExtendedUuidConnectReqSizeof, CsrBtSppExtendedUuidConnectReqSer, CsrBtSppExtendedUuidConnectReqDes, CsrBtSppExtendedUuidConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM
    { CSR_BT_SPP_GET_INSTANCES_QID_CFM, CsrBtSppGetInstancesQidCfmSizeof, CsrBtSppGetInstancesQidCfmSer, CsrBtSppGetInstancesQidCfmDes, CsrBtSppGetInstancesQidCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ
    { CSR_BT_SPP_GET_INSTANCES_QID_REQ, CsrBtSppGetInstancesQidReqSizeof, CsrBtSppGetInstancesQidReqSer, CsrBtSppGetInstancesQidReqDes, CsrBtSppGetInstancesQidReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING
    { CSR_BT_SPP_HOUSE_CLEANING, CsrBtSppHouseCleaningSizeof, CsrBtSppHouseCleaningSer, CsrBtSppHouseCleaningDes, CsrBtSppHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND
    { CSR_BT_SPP_MODE_CHANGE_IND, CsrBtSppModeChangeIndSizeof, CsrBtSppModeChangeIndSer, CsrBtSppModeChangeIndDes, CsrBtSppModeChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ
    { CSR_BT_SPP_MODE_CHANGE_REQ, CsrBtSppModeChangeReqSizeof, CsrBtSppModeChangeReqSer, CsrBtSppModeChangeReqDes, CsrBtSppModeChangeReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_CFM
    { CSR_BT_SPP_PORTNEG_CFM, CsrBtSppPortnegCfmSizeof, CsrBtSppPortnegCfmSer, CsrBtSppPortnegCfmDes, CsrBtSppPortnegCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_IND
    { CSR_BT_SPP_PORTNEG_IND, CsrBtSppPortnegIndSizeof, CsrBtSppPortnegIndSer, CsrBtSppPortnegIndDes, CsrBtSppPortnegIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_IND */
#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_REQ
    { CSR_BT_SPP_PORTNEG_REQ, CsrBtSppPortnegReqSizeof, CsrBtSppPortnegReqSer, CsrBtSppPortnegReqDes, CsrBtSppPortnegReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_RES
    { CSR_BT_SPP_PORTNEG_RES, CsrBtSppPortnegResSizeof, CsrBtSppPortnegResSer, CsrBtSppPortnegResDes, CsrBtSppPortnegResSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_RES */
#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM
    { CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM, CsrBtSppRegisterDataPathHandleCfmSizeof, CsrBtSppRegisterDataPathHandleCfmSer, CsrBtSppRegisterDataPathHandleCfmDes, CsrBtSppRegisterDataPathHandleCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ
    { CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ, CsrBtSppRegisterDataPathHandleReqSizeof, CsrBtSppRegisterDataPathHandleReqSer, CsrBtSppRegisterDataPathHandleReqDes, CsrBtSppRegisterDataPathHandleReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ
    { CSR_BT_SPP_REGISTER_QID_REQ, CsrBtSppRegisterQidReqSizeof, CsrBtSppRegisterQidReqSer, CsrBtSppRegisterQidReqDes, CsrBtSppRegisterQidReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM
    { CSR_BT_SPP_SECURITY_IN_CFM, CsrBtSppSecurityInCfmSizeof, CsrBtSppSecurityInCfmSer, CsrBtSppSecurityInCfmDes, CsrBtSppSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ
    { CSR_BT_SPP_SECURITY_IN_REQ, CsrBtSppSecurityInReqSizeof, CsrBtSppSecurityInReqSer, CsrBtSppSecurityInReqDes, CsrBtSppSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM
    { CSR_BT_SPP_SECURITY_OUT_CFM, CsrBtSppSecurityOutCfmSizeof, CsrBtSppSecurityOutCfmSer, CsrBtSppSecurityOutCfmDes, CsrBtSppSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ
    { CSR_BT_SPP_SECURITY_OUT_REQ, CsrBtSppSecurityOutReqSizeof, CsrBtSppSecurityOutReqSer, CsrBtSppSecurityOutReqDes, CsrBtSppSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND
    { CSR_BT_SPP_SERVICE_NAME_IND, CsrBtSppServiceNameIndSizeof, CsrBtSppServiceNameIndSer, CsrBtSppServiceNameIndDes, CsrBtSppServiceNameIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND */
#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES
    { CSR_BT_SPP_SERVICE_NAME_RES, CsrBtSppServiceNameResSizeof, CsrBtSppServiceNameResSer, CsrBtSppServiceNameResDes, CsrBtSppServiceNameResSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES */
#ifndef EXCLUDE_CSR_BT_SPP_STATUS_IND
    { CSR_BT_SPP_STATUS_IND, CsrBtSppStatusIndSizeof, CsrBtSppStatusIndSer, CsrBtSppStatusIndDes, CsrBtSppStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_SPP_STATUS_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSppConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SPP_PRIM, csr_bt_spp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_spp_conv_info = {
    CSR_BT_SPP_PRIM,
    "CSR_BT_SPP_PRIM",
    csr_bt_spp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSppTechInfoGet(void)
{
    return &csr_bt_spp_conv_info;
}
#endif
#endif

