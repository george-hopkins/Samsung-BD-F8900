#ifndef CSR_BT_SYNCS_SERIALIZE_H__
#define CSR_BT_SYNCS_SERIALIZE_H__
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

void CsrBtSyncsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND
CsrSize CsrBtSyncsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsDisconnectIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND
CsrSize CsrBtSyncsGetPbEntryIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetPbEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND
CsrSize CsrBtSyncsGetPbAllIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbAllIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbAllIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetPbAllIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetCalChangeLogResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalChangeLogResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetCalChangeLogResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES
CsrSize CsrBtSyncsPutPbEntryResSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutPbEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutPbEntryResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutPbEntryResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_IND
CsrSize CsrBtSyncsPutIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_IND
CsrSize CsrBtSyncsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsConnectIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES
CsrSize CsrBtSyncsAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtSyncsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES
CsrSize CsrBtSyncsGetPbAllResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbAllResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbAllResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetPbAllResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND
CsrSize CsrBtSyncsGetNextIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetNextIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetNextIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND
CsrSize CsrBtSyncsGetDeviceInfoIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetDeviceInfoIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetDeviceInfoIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetDeviceInfoIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND
CsrSize CsrBtSyncsPutPbEntryIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutPbEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutPbEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutPbEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND
CsrSize CsrBtSyncsGetPbInfoLogIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbInfoLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbInfoLogIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetPbInfoLogIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetCalCurChangeLogIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalCurChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalCurChangeLogIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetCalCurChangeLogIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES
CsrSize CsrBtSyncsGetCalInfoLogResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalInfoLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalInfoLogResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetCalInfoLogResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM
CsrSize CsrBtSyncsSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtSyncsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsSecurityInCfmSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ
CsrSize CsrBtSyncsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtSyncsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsDeactivateReqSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_IND
CsrSize CsrBtSyncsGetIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND
CsrSize CsrBtSyncsPutCalEntryIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutCalEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutCalEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutCalEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND
CsrSize CsrBtSyncsPutCalAddEntryIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutCalAddEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutCalAddEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutCalAddEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES
CsrSize CsrBtSyncsGetCalAllResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalAllResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalAllResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetCalAllResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES
CsrSize CsrBtSyncsGetPbEntryResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbEntryResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetPbEntryResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND
CsrSize CsrBtSyncsGetCalAllIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalAllIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalAllIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetCalAllIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES
CsrSize CsrBtSyncsPutCalEntryResSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutCalEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutCalEntryResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutCalEntryResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetPbChangeLogIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbChangeLogIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetPbChangeLogIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND
CsrSize CsrBtSyncsPutPbAddEntryIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutPbAddEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutPbAddEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutPbAddEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES
CsrSize CsrBtSyncsPutNextResSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutNextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutNextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM
CsrSize CsrBtSyncsDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSyncsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsDeactivateCfmSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ
CsrSize CsrBtSyncsSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtSyncsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsSecurityInReqSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_RES
CsrSize CsrBtSyncsPutResSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES
CsrSize CsrBtSyncsGetCalEntryResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalEntryResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetCalEntryResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetCalChangeLogIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalChangeLogIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetCalChangeLogIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ
CsrSize CsrBtSyncsAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtSyncsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsAuthenticateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES
CsrSize CsrBtSyncsPutPbAddEntryResSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutPbAddEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutPbAddEntryResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutPbAddEntryResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_RES
CsrSize CsrBtSyncsConnectResSizeof(void *msg);
CsrUint8 *CsrBtSyncsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsConnectResSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND
CsrSize CsrBtSyncsAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND
CsrSize CsrBtSyncsGetCalEntryIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalEntryIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetCalEntryIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES
CsrSize CsrBtSyncsGetPbInfoLogResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbInfoLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbInfoLogResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetPbInfoLogResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ
CsrSize CsrBtSyncsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtSyncsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsActivateReqSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_RES
CsrSize CsrBtSyncsGetResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND
CsrSize CsrBtSyncsGetCalInfoLogIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalInfoLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalInfoLogIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetCalInfoLogIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetPbCurChangeLogIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbCurChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbCurChangeLogIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsGetPbCurChangeLogIndSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM
CsrSize CsrBtSyncsAuthenticateCfmSizeof(void *msg);
CsrUint8 *CsrBtSyncsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSyncsAuthenticateCfmSerFree CsrBtSyncsPfree
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES
CsrSize CsrBtSyncsPutCalAddEntryResSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutCalAddEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutCalAddEntryResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutCalAddEntryResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetPbCurChangeLogResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbCurChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbCurChangeLogResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetPbCurChangeLogResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES
CsrSize CsrBtSyncsGetDeviceInfoResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetDeviceInfoResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetDeviceInfoResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetDeviceInfoResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES
CsrSize CsrBtSyncsGetNextResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetNextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetNextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetCalCurChangeLogResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetCalCurChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetCalCurChangeLogResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetCalCurChangeLogResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetPbChangeLogResSizeof(void *msg);
CsrUint8 *CsrBtSyncsGetPbChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsGetPbChangeLogResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsGetPbChangeLogResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_ABORT_IND
CsrSize CsrBtSyncsAbortIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND
CsrSize CsrBtSyncsPutNextIndSizeof(void *msg);
CsrUint8 *CsrBtSyncsPutNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSyncsPutNextIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSyncsPutNextIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND */

#ifdef __cplusplus
}
#endif

#endif
