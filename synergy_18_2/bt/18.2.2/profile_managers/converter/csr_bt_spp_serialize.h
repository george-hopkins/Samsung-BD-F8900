#ifndef CSR_BT_SPP_SERIALIZE_H__
#define CSR_BT_SPP_SERIALIZE_H__
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

void CsrBtSppPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_IND
CsrSize CsrBtSppConnectIndSizeof(void *msg);
CsrUint8 *CsrBtSppConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppConnectIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_IND
CsrSize CsrBtSppPortnegIndSizeof(void *msg);
CsrUint8 *CsrBtSppPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppPortnegIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppPortnegIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND
CsrSize CsrBtSppDataPathStatusIndSizeof(void *msg);
CsrUint8 *CsrBtSppDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDataPathStatusIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_CFM
CsrSize CsrBtSppPortnegCfmSizeof(void *msg);
CsrUint8 *CsrBtSppPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppPortnegCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppPortnegCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ
CsrSize CsrBtSppAudioRenegotiateReqSizeof(void *msg);
CsrUint8 *CsrBtSppAudioRenegotiateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioRenegotiateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioRenegotiateReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ
CsrSize CsrBtSppGetInstancesQidReqSizeof(void *msg);
CsrUint8 *CsrBtSppGetInstancesQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppGetInstancesQidReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppGetInstancesQidReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ
CsrSize CsrBtSppDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtSppDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDisconnectReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ
CsrSize CsrBtSppAudioConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSppAudioConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppAudioConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM
CsrSize CsrBtSppAudioConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtSppAudioConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioConnectCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ
CsrSize CsrBtSppAudioDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtSppAudioDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioDisconnectReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM
CsrSize CsrBtSppSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtSppSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppSecurityOutCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_IND
CsrSize CsrBtSppControlIndSizeof(void *msg);
CsrUint8 *CsrBtSppControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppControlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppControlIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM
CsrSize CsrBtSppAudioDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrBtSppAudioDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioDisconnectCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND
CsrSize CsrBtSppAudioAcceptConnectIndSizeof(void *msg);
CsrUint8 *CsrBtSppAudioAcceptConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioAcceptConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioAcceptConnectIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ
CsrSize CsrBtSppSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtSppSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppSecurityOutReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ
CsrSize CsrBtSppActivateReqSizeof(void *msg);
CsrUint8 *CsrBtSppActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ
CsrSize CsrBtSppModeChangeReqSizeof(void *msg);
CsrUint8 *CsrBtSppModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppModeChangeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppModeChangeReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ
CsrSize CsrBtSppExtendedUuidConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSppExtendedUuidConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppExtendedUuidConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppExtendedUuidConnectReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_UUID_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM
CsrSize CsrBtSppSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtSppSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppSecurityInCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND
CsrSize CsrBtSppModeChangeIndSizeof(void *msg);
CsrUint8 *CsrBtSppModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppModeChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppModeChangeIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ
CsrSize CsrBtSppExtendedConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSppExtendedConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppExtendedConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppExtendedConnectReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES
CsrSize CsrBtSppAudioAcceptConnectResSizeof(void *msg);
CsrUint8 *CsrBtSppAudioAcceptConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioAcceptConnectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppAudioAcceptConnectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_RES
CsrSize CsrBtSppPortnegResSizeof(void *msg);
CsrUint8 *CsrBtSppPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppPortnegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppPortnegResSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM
CsrSize CsrBtSppRegisterDataPathHandleCfmSizeof(void *msg);
CsrUint8 *CsrBtSppRegisterDataPathHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppRegisterDataPathHandleCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppRegisterDataPathHandleCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ
CsrSize CsrBtSppDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtSppDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDeactivateReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ
CsrSize CsrBtSppRegisterDataPathHandleReqSizeof(void *msg);
CsrUint8 *CsrBtSppRegisterDataPathHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppRegisterDataPathHandleReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppRegisterDataPathHandleReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ
CsrSize CsrBtSppSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtSppSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppSecurityInReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_CFM
CsrSize CsrBtSppDataCfmSizeof(void *msg);
CsrUint8 *CsrBtSppDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDataCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_CONTROL_REQ
CsrSize CsrBtSppControlReqSizeof(void *msg);
CsrUint8 *CsrBtSppControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppControlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppControlReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ
CsrSize CsrBtSppExtendedActivateReqSizeof(void *msg);
CsrUint8 *CsrBtSppExtendedActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppExtendedActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppExtendedActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_EXTENDED_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND
CsrSize CsrBtSppServiceNameIndSizeof(void *msg);
CsrUint8 *CsrBtSppServiceNameIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppServiceNameIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppServiceNameIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM
CsrSize CsrBtSppAudioRenegotiateCfmSizeof(void *msg);
CsrUint8 *CsrBtSppAudioRenegotiateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioRenegotiateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioRenegotiateCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ
CsrSize CsrBtSppAcceptAudioReqSizeof(void *msg);
CsrUint8 *CsrBtSppAcceptAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAcceptAudioReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAcceptAudioReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_IND
CsrSize CsrBtSppDataIndSizeof(void *msg);
CsrUint8 *CsrBtSppDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_SPP_DISCONNECT_IND
CsrSize CsrBtSppDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSppDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDisconnectIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM
CsrSize CsrBtSppGetInstancesQidCfmSizeof(void *msg);
CsrUint8 *CsrBtSppGetInstancesQidCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppGetInstancesQidCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppGetInstancesQidCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND
CsrSize CsrBtSppAudioRenegotiateIndSizeof(void *msg);
CsrUint8 *CsrBtSppAudioRenegotiateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioRenegotiateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioRenegotiateIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CONNECT_REQ
CsrSize CsrBtSppConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSppConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppConnectReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND
CsrSize CsrBtSppAudioDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSppAudioDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioDisconnectIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ
CsrSize CsrBtSppCancelAcceptAudioReqSizeof(void *msg);
CsrUint8 *CsrBtSppCancelAcceptAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppCancelAcceptAudioReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppCancelAcceptAudioReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_ACCEPT_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND
CsrSize CsrBtSppAudioConnectIndSizeof(void *msg);
CsrUint8 *CsrBtSppAudioConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppAudioConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppAudioConnectIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SPP_STATUS_IND
CsrSize CsrBtSppStatusIndSizeof(void *msg);
CsrUint8 *CsrBtSppStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppStatusIndSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING
CsrSize CsrBtSppHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtSppHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppHouseCleaningSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ
CsrSize CsrBtSppDataPathStatusReqSizeof(void *msg);
CsrUint8 *CsrBtSppDataPathStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDataPathStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDataPathStatusReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DATA_PATH_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ
CsrSize CsrBtSppCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSppCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppCancelConnectReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_PORTNEG_REQ
CsrSize CsrBtSppPortnegReqSizeof(void *msg);
CsrUint8 *CsrBtSppPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppPortnegReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppPortnegReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM
CsrSize CsrBtSppActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSppActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppActivateCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_RES
CsrSize CsrBtSppDataResSizeof(void *msg);
CsrUint8 *CsrBtSppDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDataResSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DATA_RES */

#ifndef EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ
CsrSize CsrBtSppRegisterQidReqSizeof(void *msg);
CsrUint8 *CsrBtSppRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppRegisterQidReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppRegisterQidReqSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM
CsrSize CsrBtSppDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSppDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppDeactivateCfmSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SPP_DATA_REQ
CsrSize CsrBtSppDataReqSizeof(void *msg);
CsrUint8 *CsrBtSppDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSppDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SPP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES
CsrSize CsrBtSppServiceNameResSizeof(void *msg);
CsrUint8 *CsrBtSppServiceNameResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSppServiceNameResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSppServiceNameResSerFree CsrBtSppPfree
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_RES */

#ifdef __cplusplus
}
#endif

#endif
