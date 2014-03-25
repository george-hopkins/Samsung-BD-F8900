#ifndef CSR_BT_AV_SERIALIZE_H__
#define CSR_BT_AV_SERIALIZE_H__
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

void CsrBtAvPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ
CsrSize CsrBtAvDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtAvDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDeactivateReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND
CsrSize CsrBtAvSecurityControlIndSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityControlIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSecurityControlIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM
CsrSize CsrBtAvSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSecurityInCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES
CsrSize CsrBtAvSecurityControlResSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityControlResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityControlResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSecurityControlResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ
CsrSize CsrBtAvSecurityControlReqSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityControlReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSecurityControlReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM
CsrSize CsrBtAvSecurityControlCfmSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityControlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityControlCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSecurityControlCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM */

#ifndef EXCLUDE_CSR_BT_AV_STATUS_IND
CsrSize CsrBtAvStatusIndSizeof(void *msg);
CsrUint8 *CsrBtAvStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvStatusIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_REQ
CsrSize CsrBtAvAbortReqSizeof(void *msg);
CsrUint8 *CsrBtAvAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvAbortReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ
CsrSize CsrBtAvSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSecurityInReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_IND
CsrSize CsrBtAvAbortIndSizeof(void *msg);
CsrUint8 *CsrBtAvAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvAbortIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvAbortIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_CFM
CsrSize CsrBtAvCloseCfmSizeof(void *msg);
CsrUint8 *CsrBtAvCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvCloseCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvCloseCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_IND
CsrSize CsrBtAvStreamDataIndSizeof(void *msg);
CsrUint8 *CsrBtAvStreamDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStreamDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvStreamDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM
CsrSize CsrBtAvGetConfigurationCfmSizeof(void *msg);
CsrUint8 *CsrBtAvGetConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetConfigurationCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvGetConfigurationCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_CFM
CsrSize CsrBtAvActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtAvActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvActivateCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_HOUSE_CLEANING
CsrSize CsrBtAvHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtAvHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvHouseCleaningSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM
CsrSize CsrBtAvSetConfigurationCfmSizeof(void *msg);
CsrUint8 *CsrBtAvSetConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSetConfigurationCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSetConfigurationCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER
CsrSize CsrBtAvLpSupervisionTimerSizeof(void *msg);
CsrUint8 *CsrBtAvLpSupervisionTimerSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvLpSupervisionTimerDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvLpSupervisionTimerSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM
CsrSize CsrBtAvGetChannelInfoCfmSizeof(void *msg);
CsrUint8 *CsrBtAvGetChannelInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetChannelInfoCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetChannelInfoCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_AV_START_REQ
CsrSize CsrBtAvStartReqSizeof(void *msg);
CsrUint8 *CsrBtAvStartReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStartReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvStartReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_START_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM
CsrSize CsrBtAvDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtAvDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDeactivateCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_IND
CsrSize CsrBtAvReconfigureIndSizeof(void *msg);
CsrUint8 *CsrBtAvReconfigureIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvReconfigureIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvReconfigureIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_IND */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ
CsrSize CsrBtAvDelayReportReqSizeof(void *msg);
CsrUint8 *CsrBtAvDelayReportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDelayReportReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDelayReportReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ
CsrSize CsrBtAvLpNegConfigReqSizeof(void *msg);
CsrUint8 *CsrBtAvLpNegConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvLpNegConfigReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvLpNegConfigReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AV_START_IND
CsrSize CsrBtAvStartIndSizeof(void *msg);
CsrUint8 *CsrBtAvStartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStartIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvStartIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_START_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ
CsrSize CsrBtAvGetCapabilitiesReqSizeof(void *msg);
CsrUint8 *CsrBtAvGetCapabilitiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetCapabilitiesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetCapabilitiesReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM
CsrSize CsrBtAvSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSecurityOutCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ
CsrSize CsrBtAvSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtAvSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSecurityOutReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_REQ
CsrSize CsrBtAvConnectReqSizeof(void *msg);
CsrUint8 *CsrBtAvConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvConnectReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_IND
CsrSize CsrBtAvOpenIndSizeof(void *msg);
CsrUint8 *CsrBtAvOpenIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvOpenIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvOpenIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_OPEN_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ
CsrSize CsrBtAvGetChannelInfoReqSizeof(void *msg);
CsrUint8 *CsrBtAvGetChannelInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetChannelInfoReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetChannelInfoReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_RES
CsrSize CsrBtAvSuspendResSizeof(void *msg);
CsrUint8 *CsrBtAvSuspendResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSuspendResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSuspendResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_RES */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_RES
CsrSize CsrBtAvDiscoverResSizeof(void *msg);
CsrUint8 *CsrBtAvDiscoverResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDiscoverResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvDiscoverResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_RES */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_IND
CsrSize CsrBtAvDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtAvDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDisconnectIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_CFM
CsrSize CsrBtAvConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtAvConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvConnectCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES
CsrSize CsrBtAvGetCapabilitiesResSizeof(void *msg);
CsrUint8 *CsrBtAvGetCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetCapabilitiesResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvGetCapabilitiesResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_IND
CsrSize CsrBtAvConnectIndSizeof(void *msg);
CsrUint8 *CsrBtAvConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvConnectIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_RES
CsrSize CsrBtAvCloseResSizeof(void *msg);
CsrUint8 *CsrBtAvCloseResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvCloseResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvCloseResSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_RES */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_REQ
CsrSize CsrBtAvActivateReqSizeof(void *msg);
CsrUint8 *CsrBtAvActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvActivateReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_CFM
CsrSize CsrBtAvDiscoverCfmSizeof(void *msg);
CsrUint8 *CsrBtAvDiscoverCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDiscoverCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvDiscoverCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_CFM */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND
CsrSize CsrBtAvDelayReportIndSizeof(void *msg);
CsrUint8 *CsrBtAvDelayReportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDelayReportIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDelayReportIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ
CsrSize CsrBtAvGetConfigurationReqSizeof(void *msg);
CsrUint8 *CsrBtAvGetConfigurationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetConfigurationReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetConfigurationReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_CFM
CsrSize CsrBtAvOpenCfmSizeof(void *msg);
CsrUint8 *CsrBtAvOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvOpenCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvOpenCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM
CsrSize CsrBtAvReconfigureCfmSizeof(void *msg);
CsrUint8 *CsrBtAvReconfigureCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvReconfigureCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvReconfigureCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_CFM
CsrSize CsrBtAvAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtAvAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvAbortCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ
CsrSize CsrBtAvSetConfigurationReqSizeof(void *msg);
CsrUint8 *CsrBtAvSetConfigurationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSetConfigurationReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSetConfigurationReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM
CsrSize CsrBtAvRegisterStreamHandleCfmSizeof(void *msg);
CsrUint8 *CsrBtAvRegisterStreamHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvRegisterStreamHandleCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvRegisterStreamHandleCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND
CsrSize CsrBtAvGetConfigurationIndSizeof(void *msg);
CsrUint8 *CsrBtAvGetConfigurationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetConfigurationIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetConfigurationIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_START_RES
CsrSize CsrBtAvStartResSizeof(void *msg);
CsrUint8 *CsrBtAvStartResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStartResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvStartResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_START_RES */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_REQ
CsrSize CsrBtAvCloseReqSizeof(void *msg);
CsrUint8 *CsrBtAvCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvCloseReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_REQ
CsrSize CsrBtAvDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtAvDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDisconnectReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ
CsrSize CsrBtAvRegisterStreamHandleReqSizeof(void *msg);
CsrUint8 *CsrBtAvRegisterStreamHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvRegisterStreamHandleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvRegisterStreamHandleReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_IND
CsrSize CsrBtAvCloseIndSizeof(void *msg);
CsrUint8 *CsrBtAvCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvCloseIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvCloseIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_REQ
CsrSize CsrBtAvDiscoverReqSizeof(void *msg);
CsrUint8 *CsrBtAvDiscoverReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDiscoverReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDiscoverReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_REQ */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND
CsrSize CsrBtAvStreamMtuSizeIndSizeof(void *msg);
CsrUint8 *CsrBtAvStreamMtuSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStreamMtuSizeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvStreamMtuSizeIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ
CsrSize CsrBtAvReconfigureReqSizeof(void *msg);
CsrUint8 *CsrBtAvReconfigureReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvReconfigureReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvReconfigureReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_QOS_IND
CsrSize CsrBtAvQosIndSizeof(void *msg);
CsrUint8 *CsrBtAvQosIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvQosIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvQosIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_QOS_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES
CsrSize CsrBtAvGetAllCapabilitiesResSizeof(void *msg);
CsrUint8 *CsrBtAvGetAllCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetAllCapabilitiesResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvGetAllCapabilitiesResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM
CsrSize CsrBtAvGetCapabilitiesCfmSizeof(void *msg);
CsrUint8 *CsrBtAvGetCapabilitiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetCapabilitiesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvGetCapabilitiesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ
CsrSize CsrBtAvStreamDataReqSizeof(void *msg);
CsrUint8 *CsrBtAvStreamDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStreamDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvStreamDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_IND
CsrSize CsrBtAvSuspendIndSizeof(void *msg);
CsrUint8 *CsrBtAvSuspendIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSuspendIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSuspendIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES
CsrSize CsrBtAvSetConfigurationResSizeof(void *msg);
CsrUint8 *CsrBtAvSetConfigurationResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSetConfigurationResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSetConfigurationResSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND
CsrSize CsrBtAvGetCapabilitiesIndSizeof(void *msg);
CsrUint8 *CsrBtAvGetCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetCapabilitiesIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_START_CFM
CsrSize CsrBtAvStartCfmSizeof(void *msg);
CsrUint8 *CsrBtAvStartCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvStartCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvStartCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_START_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND
CsrSize CsrBtAvSetConfigurationIndSizeof(void *msg);
CsrUint8 *CsrBtAvSetConfigurationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSetConfigurationIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSetConfigurationIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_REQ
CsrSize CsrBtAvSuspendReqSizeof(void *msg);
CsrUint8 *CsrBtAvSuspendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSuspendReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvSuspendReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_RES
CsrSize CsrBtAvOpenResSizeof(void *msg);
CsrUint8 *CsrBtAvOpenResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvOpenResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvOpenResSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_OPEN_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND
CsrSize CsrBtAvGetAllCapabilitiesIndSizeof(void *msg);
CsrUint8 *CsrBtAvGetAllCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetAllCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetAllCapabilitiesIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_IND
CsrSize CsrBtAvDiscoverIndSizeof(void *msg);
CsrUint8 *CsrBtAvDiscoverIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvDiscoverIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvDiscoverIndSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_IND */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_REQ
CsrSize CsrBtAvOpenReqSizeof(void *msg);
CsrUint8 *CsrBtAvOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvOpenReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvOpenReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ
CsrSize CsrBtAvSetQosIntervalReqSizeof(void *msg);
CsrUint8 *CsrBtAvSetQosIntervalReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSetQosIntervalReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSetQosIntervalReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_RES
CsrSize CsrBtAvAbortResSizeof(void *msg);
CsrUint8 *CsrBtAvAbortResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvAbortResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvAbortResSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_ABORT_RES */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_CFM
CsrSize CsrBtAvSuspendCfmSizeof(void *msg);
CsrUint8 *CsrBtAvSuspendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvSuspendCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvSuspendCfmSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES
CsrSize CsrBtAvGetConfigurationResSizeof(void *msg);
CsrUint8 *CsrBtAvGetConfigurationResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetConfigurationResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAvGetConfigurationResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_RES
CsrSize CsrBtAvReconfigureResSizeof(void *msg);
CsrUint8 *CsrBtAvReconfigureResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvReconfigureResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvReconfigureResSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ
CsrSize CsrBtAvGetStreamChannelInfoReqSizeof(void *msg);
CsrUint8 *CsrBtAvGetStreamChannelInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvGetStreamChannelInfoReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvGetStreamChannelInfoReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ
CsrSize CsrBtAvCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtAvCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAvCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAvCancelConnectReqSerFree CsrBtAvPfree
#endif /* EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ */

#ifdef __cplusplus
}
#endif

#endif
