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
#include "csr_bt_av_converter_init.h"
#ifndef EXCLUDE_CSR_BT_AV_MODULE
#include "csr_bt_av_serialize.h"
#include "csr_bt_av_prim.h"

static CsrMsgConvMsgEntry csr_bt_av_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_AV_ABORT_CFM
    { CSR_BT_AV_ABORT_CFM, CsrBtAvAbortCfmSizeof, CsrBtAvAbortCfmSer, CsrBtAvAbortCfmDes, CsrBtAvAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_AV_ABORT_IND
    { CSR_BT_AV_ABORT_IND, CsrBtAvAbortIndSizeof, CsrBtAvAbortIndSer, CsrBtAvAbortIndDes, CsrBtAvAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_AV_ABORT_REQ
    { CSR_BT_AV_ABORT_REQ, CsrBtAvAbortReqSizeof, CsrBtAvAbortReqSer, CsrBtAvAbortReqDes, CsrBtAvAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_AV_ABORT_RES
    { CSR_BT_AV_ABORT_RES, CsrBtAvAbortResSizeof, CsrBtAvAbortResSer, CsrBtAvAbortResDes, CsrBtAvAbortResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_ABORT_RES */
#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_CFM
    { CSR_BT_AV_ACTIVATE_CFM, CsrBtAvActivateCfmSizeof, CsrBtAvActivateCfmSer, CsrBtAvActivateCfmDes, CsrBtAvActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_REQ
    { CSR_BT_AV_ACTIVATE_REQ, CsrBtAvActivateReqSizeof, CsrBtAvActivateReqSer, CsrBtAvActivateReqDes, CsrBtAvActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ
    { CSR_BT_AV_CANCEL_CONNECT_REQ, CsrBtAvCancelConnectReqSizeof, CsrBtAvCancelConnectReqSer, CsrBtAvCancelConnectReqDes, CsrBtAvCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_AV_CLOSE_CFM
    { CSR_BT_AV_CLOSE_CFM, CsrBtAvCloseCfmSizeof, CsrBtAvCloseCfmSer, CsrBtAvCloseCfmDes, CsrBtAvCloseCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_CFM */
#ifndef EXCLUDE_CSR_BT_AV_CLOSE_IND
    { CSR_BT_AV_CLOSE_IND, CsrBtAvCloseIndSizeof, CsrBtAvCloseIndSer, CsrBtAvCloseIndDes, CsrBtAvCloseIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_IND */
#ifndef EXCLUDE_CSR_BT_AV_CLOSE_REQ
    { CSR_BT_AV_CLOSE_REQ, CsrBtAvCloseReqSizeof, CsrBtAvCloseReqSer, CsrBtAvCloseReqDes, CsrBtAvCloseReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_REQ */
#ifndef EXCLUDE_CSR_BT_AV_CLOSE_RES
    { CSR_BT_AV_CLOSE_RES, CsrBtAvCloseResSizeof, CsrBtAvCloseResSer, CsrBtAvCloseResDes, CsrBtAvCloseResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_RES */
#ifndef EXCLUDE_CSR_BT_AV_CONNECT_CFM
    { CSR_BT_AV_CONNECT_CFM, CsrBtAvConnectCfmSizeof, CsrBtAvConnectCfmSer, CsrBtAvConnectCfmDes, CsrBtAvConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_AV_CONNECT_IND
    { CSR_BT_AV_CONNECT_IND, CsrBtAvConnectIndSizeof, CsrBtAvConnectIndSer, CsrBtAvConnectIndDes, CsrBtAvConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_AV_CONNECT_REQ
    { CSR_BT_AV_CONNECT_REQ, CsrBtAvConnectReqSizeof, CsrBtAvConnectReqSer, CsrBtAvConnectReqDes, CsrBtAvConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM
    { CSR_BT_AV_DEACTIVATE_CFM, CsrBtAvDeactivateCfmSizeof, CsrBtAvDeactivateCfmSer, CsrBtAvDeactivateCfmDes, CsrBtAvDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ
    { CSR_BT_AV_DEACTIVATE_REQ, CsrBtAvDeactivateReqSizeof, CsrBtAvDeactivateReqSer, CsrBtAvDeactivateReqDes, CsrBtAvDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND
    { CSR_BT_AV_DELAY_REPORT_IND, CsrBtAvDelayReportIndSizeof, CsrBtAvDelayReportIndSer, CsrBtAvDelayReportIndDes, CsrBtAvDelayReportIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND */
#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ
    { CSR_BT_AV_DELAY_REPORT_REQ, CsrBtAvDelayReportReqSizeof, CsrBtAvDelayReportReqSer, CsrBtAvDelayReportReqDes, CsrBtAvDelayReportReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ */
#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_IND
    { CSR_BT_AV_DISCONNECT_IND, CsrBtAvDisconnectIndSizeof, CsrBtAvDisconnectIndSer, CsrBtAvDisconnectIndDes, CsrBtAvDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_REQ
    { CSR_BT_AV_DISCONNECT_REQ, CsrBtAvDisconnectReqSizeof, CsrBtAvDisconnectReqSer, CsrBtAvDisconnectReqDes, CsrBtAvDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_CFM
    { CSR_BT_AV_DISCOVER_CFM, CsrBtAvDiscoverCfmSizeof, CsrBtAvDiscoverCfmSer, CsrBtAvDiscoverCfmDes, CsrBtAvDiscoverCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_CFM */
#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_IND
    { CSR_BT_AV_DISCOVER_IND, CsrBtAvDiscoverIndSizeof, CsrBtAvDiscoverIndSer, CsrBtAvDiscoverIndDes, CsrBtAvDiscoverIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_IND */
#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_REQ
    { CSR_BT_AV_DISCOVER_REQ, CsrBtAvDiscoverReqSizeof, CsrBtAvDiscoverReqSer, CsrBtAvDiscoverReqDes, CsrBtAvDiscoverReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_REQ */
#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_RES
    { CSR_BT_AV_DISCOVER_RES, CsrBtAvDiscoverResSizeof, CsrBtAvDiscoverResSer, CsrBtAvDiscoverResDes, CsrBtAvDiscoverResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_RES */
#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND
    { CSR_BT_AV_GET_ALL_CAPABILITIES_IND, CsrBtAvGetAllCapabilitiesIndSizeof, CsrBtAvGetAllCapabilitiesIndSer, CsrBtAvGetAllCapabilitiesIndDes, CsrBtAvGetAllCapabilitiesIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND */
#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES
    { CSR_BT_AV_GET_ALL_CAPABILITIES_RES, CsrBtAvGetAllCapabilitiesResSizeof, CsrBtAvGetAllCapabilitiesResSer, CsrBtAvGetAllCapabilitiesResDes, CsrBtAvGetAllCapabilitiesResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES */
#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM
    { CSR_BT_AV_GET_CAPABILITIES_CFM, CsrBtAvGetCapabilitiesCfmSizeof, CsrBtAvGetCapabilitiesCfmSer, CsrBtAvGetCapabilitiesCfmDes, CsrBtAvGetCapabilitiesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM */
#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND
    { CSR_BT_AV_GET_CAPABILITIES_IND, CsrBtAvGetCapabilitiesIndSizeof, CsrBtAvGetCapabilitiesIndSer, CsrBtAvGetCapabilitiesIndDes, CsrBtAvGetCapabilitiesIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND */
#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ
    { CSR_BT_AV_GET_CAPABILITIES_REQ, CsrBtAvGetCapabilitiesReqSizeof, CsrBtAvGetCapabilitiesReqSer, CsrBtAvGetCapabilitiesReqDes, CsrBtAvGetCapabilitiesReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ */
#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES
    { CSR_BT_AV_GET_CAPABILITIES_RES, CsrBtAvGetCapabilitiesResSizeof, CsrBtAvGetCapabilitiesResSer, CsrBtAvGetCapabilitiesResDes, CsrBtAvGetCapabilitiesResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES */
#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM
    { CSR_BT_AV_GET_CHANNEL_INFO_CFM, CsrBtAvGetChannelInfoCfmSizeof, CsrBtAvGetChannelInfoCfmSer, CsrBtAvGetChannelInfoCfmDes, CsrBtAvGetChannelInfoCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM */
#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ
    { CSR_BT_AV_GET_CHANNEL_INFO_REQ, CsrBtAvGetChannelInfoReqSizeof, CsrBtAvGetChannelInfoReqSer, CsrBtAvGetChannelInfoReqDes, CsrBtAvGetChannelInfoReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ */
#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM
    { CSR_BT_AV_GET_CONFIGURATION_CFM, CsrBtAvGetConfigurationCfmSizeof, CsrBtAvGetConfigurationCfmSer, CsrBtAvGetConfigurationCfmDes, CsrBtAvGetConfigurationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM */
#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND
    { CSR_BT_AV_GET_CONFIGURATION_IND, CsrBtAvGetConfigurationIndSizeof, CsrBtAvGetConfigurationIndSer, CsrBtAvGetConfigurationIndDes, CsrBtAvGetConfigurationIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND */
#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ
    { CSR_BT_AV_GET_CONFIGURATION_REQ, CsrBtAvGetConfigurationReqSizeof, CsrBtAvGetConfigurationReqSer, CsrBtAvGetConfigurationReqDes, CsrBtAvGetConfigurationReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ */
#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES
    { CSR_BT_AV_GET_CONFIGURATION_RES, CsrBtAvGetConfigurationResSizeof, CsrBtAvGetConfigurationResSer, CsrBtAvGetConfigurationResDes, CsrBtAvGetConfigurationResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES */
#ifndef EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ
    { CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ, CsrBtAvGetStreamChannelInfoReqSizeof, CsrBtAvGetStreamChannelInfoReqSer, CsrBtAvGetStreamChannelInfoReqDes, CsrBtAvGetStreamChannelInfoReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */
#ifndef EXCLUDE_CSR_BT_AV_HOUSE_CLEANING
    { CSR_BT_AV_HOUSE_CLEANING, CsrBtAvHouseCleaningSizeof, CsrBtAvHouseCleaningSer, CsrBtAvHouseCleaningDes, CsrBtAvHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_AV_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ
    { CSR_BT_AV_LP_NEG_CONFIG_REQ, CsrBtAvLpNegConfigReqSizeof, CsrBtAvLpNegConfigReqSer, CsrBtAvLpNegConfigReqDes, CsrBtAvLpNegConfigReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER
    { CSR_BT_AV_LP_SUPERVISION_TIMER, CsrBtAvLpSupervisionTimerSizeof, CsrBtAvLpSupervisionTimerSer, CsrBtAvLpSupervisionTimerDes, CsrBtAvLpSupervisionTimerSerFree },
#endif /* EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER */
#ifndef EXCLUDE_CSR_BT_AV_OPEN_CFM
    { CSR_BT_AV_OPEN_CFM, CsrBtAvOpenCfmSizeof, CsrBtAvOpenCfmSer, CsrBtAvOpenCfmDes, CsrBtAvOpenCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_OPEN_CFM */
#ifndef EXCLUDE_CSR_BT_AV_OPEN_IND
    { CSR_BT_AV_OPEN_IND, CsrBtAvOpenIndSizeof, CsrBtAvOpenIndSer, CsrBtAvOpenIndDes, CsrBtAvOpenIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_OPEN_IND */
#ifndef EXCLUDE_CSR_BT_AV_OPEN_REQ
    { CSR_BT_AV_OPEN_REQ, CsrBtAvOpenReqSizeof, CsrBtAvOpenReqSer, CsrBtAvOpenReqDes, CsrBtAvOpenReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_OPEN_REQ */
#ifndef EXCLUDE_CSR_BT_AV_OPEN_RES
    { CSR_BT_AV_OPEN_RES, CsrBtAvOpenResSizeof, CsrBtAvOpenResSer, CsrBtAvOpenResDes, CsrBtAvOpenResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_OPEN_RES */
#ifndef EXCLUDE_CSR_BT_AV_QOS_IND
    { CSR_BT_AV_QOS_IND, CsrBtAvQosIndSizeof, CsrBtAvQosIndSer, CsrBtAvQosIndDes, CsrBtAvQosIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_QOS_IND */
#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM
    { CSR_BT_AV_RECONFIGURE_CFM, CsrBtAvReconfigureCfmSizeof, CsrBtAvReconfigureCfmSer, CsrBtAvReconfigureCfmDes, CsrBtAvReconfigureCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM */
#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_IND
    { CSR_BT_AV_RECONFIGURE_IND, CsrBtAvReconfigureIndSizeof, CsrBtAvReconfigureIndSer, CsrBtAvReconfigureIndDes, CsrBtAvReconfigureIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_IND */
#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ
    { CSR_BT_AV_RECONFIGURE_REQ, CsrBtAvReconfigureReqSizeof, CsrBtAvReconfigureReqSer, CsrBtAvReconfigureReqDes, CsrBtAvReconfigureReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ */
#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_RES
    { CSR_BT_AV_RECONFIGURE_RES, CsrBtAvReconfigureResSizeof, CsrBtAvReconfigureResSer, CsrBtAvReconfigureResDes, CsrBtAvReconfigureResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_RES */
#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM
    { CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM, CsrBtAvRegisterStreamHandleCfmSizeof, CsrBtAvRegisterStreamHandleCfmSer, CsrBtAvRegisterStreamHandleCfmDes, CsrBtAvRegisterStreamHandleCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM */
#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ
    { CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ, CsrBtAvRegisterStreamHandleReqSizeof, CsrBtAvRegisterStreamHandleReqSer, CsrBtAvRegisterStreamHandleReqDes, CsrBtAvRegisterStreamHandleReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM
    { CSR_BT_AV_SECURITY_CONTROL_CFM, CsrBtAvSecurityControlCfmSizeof, CsrBtAvSecurityControlCfmSer, CsrBtAvSecurityControlCfmDes, CsrBtAvSecurityControlCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND
    { CSR_BT_AV_SECURITY_CONTROL_IND, CsrBtAvSecurityControlIndSizeof, CsrBtAvSecurityControlIndSer, CsrBtAvSecurityControlIndDes, CsrBtAvSecurityControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ
    { CSR_BT_AV_SECURITY_CONTROL_REQ, CsrBtAvSecurityControlReqSizeof, CsrBtAvSecurityControlReqSer, CsrBtAvSecurityControlReqDes, CsrBtAvSecurityControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES
    { CSR_BT_AV_SECURITY_CONTROL_RES, CsrBtAvSecurityControlResSizeof, CsrBtAvSecurityControlResSer, CsrBtAvSecurityControlResDes, CsrBtAvSecurityControlResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM
    { CSR_BT_AV_SECURITY_IN_CFM, CsrBtAvSecurityInCfmSizeof, CsrBtAvSecurityInCfmSer, CsrBtAvSecurityInCfmDes, CsrBtAvSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ
    { CSR_BT_AV_SECURITY_IN_REQ, CsrBtAvSecurityInReqSizeof, CsrBtAvSecurityInReqSer, CsrBtAvSecurityInReqDes, CsrBtAvSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM
    { CSR_BT_AV_SECURITY_OUT_CFM, CsrBtAvSecurityOutCfmSizeof, CsrBtAvSecurityOutCfmSer, CsrBtAvSecurityOutCfmDes, CsrBtAvSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ
    { CSR_BT_AV_SECURITY_OUT_REQ, CsrBtAvSecurityOutReqSizeof, CsrBtAvSecurityOutReqSer, CsrBtAvSecurityOutReqDes, CsrBtAvSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM
    { CSR_BT_AV_SET_CONFIGURATION_CFM, CsrBtAvSetConfigurationCfmSizeof, CsrBtAvSetConfigurationCfmSer, CsrBtAvSetConfigurationCfmDes, CsrBtAvSetConfigurationCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM */
#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND
    { CSR_BT_AV_SET_CONFIGURATION_IND, CsrBtAvSetConfigurationIndSizeof, CsrBtAvSetConfigurationIndSer, CsrBtAvSetConfigurationIndDes, CsrBtAvSetConfigurationIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND */
#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ
    { CSR_BT_AV_SET_CONFIGURATION_REQ, CsrBtAvSetConfigurationReqSizeof, CsrBtAvSetConfigurationReqSer, CsrBtAvSetConfigurationReqDes, CsrBtAvSetConfigurationReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES
    { CSR_BT_AV_SET_CONFIGURATION_RES, CsrBtAvSetConfigurationResSizeof, CsrBtAvSetConfigurationResSer, CsrBtAvSetConfigurationResDes, CsrBtAvSetConfigurationResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES */
#ifndef EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ
    { CSR_BT_AV_SET_QOS_INTERVAL_REQ, CsrBtAvSetQosIntervalReqSizeof, CsrBtAvSetQosIntervalReqSer, CsrBtAvSetQosIntervalReqDes, CsrBtAvSetQosIntervalReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ */
#ifndef EXCLUDE_CSR_BT_AV_START_CFM
    { CSR_BT_AV_START_CFM, CsrBtAvStartCfmSizeof, CsrBtAvStartCfmSer, CsrBtAvStartCfmDes, CsrBtAvStartCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_START_CFM */
#ifndef EXCLUDE_CSR_BT_AV_START_IND
    { CSR_BT_AV_START_IND, CsrBtAvStartIndSizeof, CsrBtAvStartIndSer, CsrBtAvStartIndDes, CsrBtAvStartIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_START_IND */
#ifndef EXCLUDE_CSR_BT_AV_START_REQ
    { CSR_BT_AV_START_REQ, CsrBtAvStartReqSizeof, CsrBtAvStartReqSer, CsrBtAvStartReqDes, CsrBtAvStartReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_START_REQ */
#ifndef EXCLUDE_CSR_BT_AV_START_RES
    { CSR_BT_AV_START_RES, CsrBtAvStartResSizeof, CsrBtAvStartResSer, CsrBtAvStartResDes, CsrBtAvStartResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_START_RES */
#ifndef EXCLUDE_CSR_BT_AV_STATUS_IND
    { CSR_BT_AV_STATUS_IND, CsrBtAvStatusIndSizeof, CsrBtAvStatusIndSer, CsrBtAvStatusIndDes, CsrBtAvStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_IND
    { CSR_BT_AV_STREAM_DATA_IND, CsrBtAvStreamDataIndSizeof, CsrBtAvStreamDataIndSer, CsrBtAvStreamDataIndDes, CsrBtAvStreamDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_IND */
#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ
    { CSR_BT_AV_STREAM_DATA_REQ, CsrBtAvStreamDataReqSizeof, CsrBtAvStreamDataReqSer, CsrBtAvStreamDataReqDes, CsrBtAvStreamDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND
    { CSR_BT_AV_STREAM_MTU_SIZE_IND, CsrBtAvStreamMtuSizeIndSizeof, CsrBtAvStreamMtuSizeIndSer, CsrBtAvStreamMtuSizeIndDes, CsrBtAvStreamMtuSizeIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND */
#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_CFM
    { CSR_BT_AV_SUSPEND_CFM, CsrBtAvSuspendCfmSizeof, CsrBtAvSuspendCfmSer, CsrBtAvSuspendCfmDes, CsrBtAvSuspendCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_CFM */
#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_IND
    { CSR_BT_AV_SUSPEND_IND, CsrBtAvSuspendIndSizeof, CsrBtAvSuspendIndSer, CsrBtAvSuspendIndDes, CsrBtAvSuspendIndSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_IND */
#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_REQ
    { CSR_BT_AV_SUSPEND_REQ, CsrBtAvSuspendReqSizeof, CsrBtAvSuspendReqSer, CsrBtAvSuspendReqDes, CsrBtAvSuspendReqSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_REQ */
#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_RES
    { CSR_BT_AV_SUSPEND_RES, CsrBtAvSuspendResSizeof, CsrBtAvSuspendResSer, CsrBtAvSuspendResDes, CsrBtAvSuspendResSerFree },
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtAvConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_AV_PRIM, csr_bt_av_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_av_conv_info = {
    CSR_BT_AV_PRIM,
    "CSR_BT_AV_PRIM",
    csr_bt_av_conv_lut
};

CsrLogPrimitiveInformation* CsrBtAvTechInfoGet(void)
{
    return &csr_bt_av_conv_info;
}
#endif
#endif

