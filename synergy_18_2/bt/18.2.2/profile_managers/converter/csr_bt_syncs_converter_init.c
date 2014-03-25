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
#include "csr_bt_syncs_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
#include "csr_bt_syncs_serialize.h"
#include "csr_bt_syncs_prim.h"

static CsrMsgConvMsgEntry csr_bt_syncs_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SYNCS_ABORT_IND
    { CSR_BT_SYNCS_ABORT_IND, CsrBtSyncsAbortIndSizeof, CsrBtSyncsAbortIndSer, CsrBtSyncsAbortIndDes, CsrBtSyncsAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ
    { CSR_BT_SYNCS_ACTIVATE_REQ, CsrBtSyncsActivateReqSizeof, CsrBtSyncsActivateReqSer, CsrBtSyncsActivateReqDes, CsrBtSyncsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM
    { CSR_BT_SYNCS_AUTHENTICATE_CFM, CsrBtSyncsAuthenticateCfmSizeof, CsrBtSyncsAuthenticateCfmSer, CsrBtSyncsAuthenticateCfmDes, CsrBtSyncsAuthenticateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND
    { CSR_BT_SYNCS_AUTHENTICATE_IND, CsrBtSyncsAuthenticateIndSizeof, CsrBtSyncsAuthenticateIndSer, CsrBtSyncsAuthenticateIndDes, CsrBtSyncsAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ
    { CSR_BT_SYNCS_AUTHENTICATE_REQ, CsrBtSyncsAuthenticateReqSizeof, CsrBtSyncsAuthenticateReqSer, CsrBtSyncsAuthenticateReqDes, CsrBtSyncsAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES
    { CSR_BT_SYNCS_AUTHENTICATE_RES, CsrBtSyncsAuthenticateResSizeof, CsrBtSyncsAuthenticateResSer, CsrBtSyncsAuthenticateResDes, CsrBtSyncsAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_IND
    { CSR_BT_SYNCS_CONNECT_IND, CsrBtSyncsConnectIndSizeof, CsrBtSyncsConnectIndSer, CsrBtSyncsConnectIndDes, CsrBtSyncsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_RES
    { CSR_BT_SYNCS_CONNECT_RES, CsrBtSyncsConnectResSizeof, CsrBtSyncsConnectResSer, CsrBtSyncsConnectResDes, CsrBtSyncsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM
    { CSR_BT_SYNCS_DEACTIVATE_CFM, CsrBtSyncsDeactivateCfmSizeof, CsrBtSyncsDeactivateCfmSer, CsrBtSyncsDeactivateCfmDes, CsrBtSyncsDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ
    { CSR_BT_SYNCS_DEACTIVATE_REQ, CsrBtSyncsDeactivateReqSizeof, CsrBtSyncsDeactivateReqSer, CsrBtSyncsDeactivateReqDes, CsrBtSyncsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND
    { CSR_BT_SYNCS_DISCONNECT_IND, CsrBtSyncsDisconnectIndSizeof, CsrBtSyncsDisconnectIndSer, CsrBtSyncsDisconnectIndDes, CsrBtSyncsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND
    { CSR_BT_SYNCS_GET_CAL_ALL_IND, CsrBtSyncsGetCalAllIndSizeof, CsrBtSyncsGetCalAllIndSer, CsrBtSyncsGetCalAllIndDes, CsrBtSyncsGetCalAllIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES
    { CSR_BT_SYNCS_GET_CAL_ALL_RES, CsrBtSyncsGetCalAllResSizeof, CsrBtSyncsGetCalAllResSer, CsrBtSyncsGetCalAllResDes, CsrBtSyncsGetCalAllResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND
    { CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND, CsrBtSyncsGetCalChangeLogIndSizeof, CsrBtSyncsGetCalChangeLogIndSer, CsrBtSyncsGetCalChangeLogIndDes, CsrBtSyncsGetCalChangeLogIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES
    { CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES, CsrBtSyncsGetCalChangeLogResSizeof, CsrBtSyncsGetCalChangeLogResSer, CsrBtSyncsGetCalChangeLogResDes, CsrBtSyncsGetCalChangeLogResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND
    { CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND, CsrBtSyncsGetCalCurChangeLogIndSizeof, CsrBtSyncsGetCalCurChangeLogIndSer, CsrBtSyncsGetCalCurChangeLogIndDes, CsrBtSyncsGetCalCurChangeLogIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES
    { CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES, CsrBtSyncsGetCalCurChangeLogResSizeof, CsrBtSyncsGetCalCurChangeLogResSer, CsrBtSyncsGetCalCurChangeLogResDes, CsrBtSyncsGetCalCurChangeLogResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND
    { CSR_BT_SYNCS_GET_CAL_ENTRY_IND, CsrBtSyncsGetCalEntryIndSizeof, CsrBtSyncsGetCalEntryIndSer, CsrBtSyncsGetCalEntryIndDes, CsrBtSyncsGetCalEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES
    { CSR_BT_SYNCS_GET_CAL_ENTRY_RES, CsrBtSyncsGetCalEntryResSizeof, CsrBtSyncsGetCalEntryResSer, CsrBtSyncsGetCalEntryResDes, CsrBtSyncsGetCalEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND
    { CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND, CsrBtSyncsGetCalInfoLogIndSizeof, CsrBtSyncsGetCalInfoLogIndSer, CsrBtSyncsGetCalInfoLogIndDes, CsrBtSyncsGetCalInfoLogIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES
    { CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES, CsrBtSyncsGetCalInfoLogResSizeof, CsrBtSyncsGetCalInfoLogResSer, CsrBtSyncsGetCalInfoLogResDes, CsrBtSyncsGetCalInfoLogResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND
    { CSR_BT_SYNCS_GET_DEVICE_INFO_IND, CsrBtSyncsGetDeviceInfoIndSizeof, CsrBtSyncsGetDeviceInfoIndSer, CsrBtSyncsGetDeviceInfoIndDes, CsrBtSyncsGetDeviceInfoIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES
    { CSR_BT_SYNCS_GET_DEVICE_INFO_RES, CsrBtSyncsGetDeviceInfoResSizeof, CsrBtSyncsGetDeviceInfoResSer, CsrBtSyncsGetDeviceInfoResDes, CsrBtSyncsGetDeviceInfoResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_IND
    { CSR_BT_SYNCS_GET_IND, CsrBtSyncsGetIndSizeof, CsrBtSyncsGetIndSer, CsrBtSyncsGetIndDes, CsrBtSyncsGetIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND
    { CSR_BT_SYNCS_GET_NEXT_IND, CsrBtSyncsGetNextIndSizeof, CsrBtSyncsGetNextIndSer, CsrBtSyncsGetNextIndDes, CsrBtSyncsGetNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES
    { CSR_BT_SYNCS_GET_NEXT_RES, CsrBtSyncsGetNextResSizeof, CsrBtSyncsGetNextResSer, CsrBtSyncsGetNextResDes, CsrBtSyncsGetNextResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND
    { CSR_BT_SYNCS_GET_PB_ALL_IND, CsrBtSyncsGetPbAllIndSizeof, CsrBtSyncsGetPbAllIndSer, CsrBtSyncsGetPbAllIndDes, CsrBtSyncsGetPbAllIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES
    { CSR_BT_SYNCS_GET_PB_ALL_RES, CsrBtSyncsGetPbAllResSizeof, CsrBtSyncsGetPbAllResSer, CsrBtSyncsGetPbAllResDes, CsrBtSyncsGetPbAllResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND
    { CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND, CsrBtSyncsGetPbChangeLogIndSizeof, CsrBtSyncsGetPbChangeLogIndSer, CsrBtSyncsGetPbChangeLogIndDes, CsrBtSyncsGetPbChangeLogIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES
    { CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES, CsrBtSyncsGetPbChangeLogResSizeof, CsrBtSyncsGetPbChangeLogResSer, CsrBtSyncsGetPbChangeLogResDes, CsrBtSyncsGetPbChangeLogResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND
    { CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND, CsrBtSyncsGetPbCurChangeLogIndSizeof, CsrBtSyncsGetPbCurChangeLogIndSer, CsrBtSyncsGetPbCurChangeLogIndDes, CsrBtSyncsGetPbCurChangeLogIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES
    { CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES, CsrBtSyncsGetPbCurChangeLogResSizeof, CsrBtSyncsGetPbCurChangeLogResSer, CsrBtSyncsGetPbCurChangeLogResDes, CsrBtSyncsGetPbCurChangeLogResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND
    { CSR_BT_SYNCS_GET_PB_ENTRY_IND, CsrBtSyncsGetPbEntryIndSizeof, CsrBtSyncsGetPbEntryIndSer, CsrBtSyncsGetPbEntryIndDes, CsrBtSyncsGetPbEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES
    { CSR_BT_SYNCS_GET_PB_ENTRY_RES, CsrBtSyncsGetPbEntryResSizeof, CsrBtSyncsGetPbEntryResSer, CsrBtSyncsGetPbEntryResDes, CsrBtSyncsGetPbEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND
    { CSR_BT_SYNCS_GET_PB_INFO_LOG_IND, CsrBtSyncsGetPbInfoLogIndSizeof, CsrBtSyncsGetPbInfoLogIndSer, CsrBtSyncsGetPbInfoLogIndDes, CsrBtSyncsGetPbInfoLogIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES
    { CSR_BT_SYNCS_GET_PB_INFO_LOG_RES, CsrBtSyncsGetPbInfoLogResSizeof, CsrBtSyncsGetPbInfoLogResSer, CsrBtSyncsGetPbInfoLogResDes, CsrBtSyncsGetPbInfoLogResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_RES
    { CSR_BT_SYNCS_GET_RES, CsrBtSyncsGetResSizeof, CsrBtSyncsGetResSer, CsrBtSyncsGetResDes, CsrBtSyncsGetResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND
    { CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND, CsrBtSyncsPutCalAddEntryIndSizeof, CsrBtSyncsPutCalAddEntryIndSer, CsrBtSyncsPutCalAddEntryIndDes, CsrBtSyncsPutCalAddEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES
    { CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES, CsrBtSyncsPutCalAddEntryResSizeof, CsrBtSyncsPutCalAddEntryResSer, CsrBtSyncsPutCalAddEntryResDes, CsrBtSyncsPutCalAddEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND
    { CSR_BT_SYNCS_PUT_CAL_ENTRY_IND, CsrBtSyncsPutCalEntryIndSizeof, CsrBtSyncsPutCalEntryIndSer, CsrBtSyncsPutCalEntryIndDes, CsrBtSyncsPutCalEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES
    { CSR_BT_SYNCS_PUT_CAL_ENTRY_RES, CsrBtSyncsPutCalEntryResSizeof, CsrBtSyncsPutCalEntryResSer, CsrBtSyncsPutCalEntryResDes, CsrBtSyncsPutCalEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_IND
    { CSR_BT_SYNCS_PUT_IND, CsrBtSyncsPutIndSizeof, CsrBtSyncsPutIndSer, CsrBtSyncsPutIndDes, CsrBtSyncsPutIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND
    { CSR_BT_SYNCS_PUT_NEXT_IND, CsrBtSyncsPutNextIndSizeof, CsrBtSyncsPutNextIndSer, CsrBtSyncsPutNextIndDes, CsrBtSyncsPutNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES
    { CSR_BT_SYNCS_PUT_NEXT_RES, CsrBtSyncsPutNextResSizeof, CsrBtSyncsPutNextResSer, CsrBtSyncsPutNextResDes, CsrBtSyncsPutNextResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND
    { CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND, CsrBtSyncsPutPbAddEntryIndSizeof, CsrBtSyncsPutPbAddEntryIndSer, CsrBtSyncsPutPbAddEntryIndDes, CsrBtSyncsPutPbAddEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES
    { CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES, CsrBtSyncsPutPbAddEntryResSizeof, CsrBtSyncsPutPbAddEntryResSer, CsrBtSyncsPutPbAddEntryResDes, CsrBtSyncsPutPbAddEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND
    { CSR_BT_SYNCS_PUT_PB_ENTRY_IND, CsrBtSyncsPutPbEntryIndSizeof, CsrBtSyncsPutPbEntryIndSer, CsrBtSyncsPutPbEntryIndDes, CsrBtSyncsPutPbEntryIndSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES
    { CSR_BT_SYNCS_PUT_PB_ENTRY_RES, CsrBtSyncsPutPbEntryResSizeof, CsrBtSyncsPutPbEntryResSer, CsrBtSyncsPutPbEntryResDes, CsrBtSyncsPutPbEntryResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_RES
    { CSR_BT_SYNCS_PUT_RES, CsrBtSyncsPutResSizeof, CsrBtSyncsPutResSer, CsrBtSyncsPutResDes, CsrBtSyncsPutResSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM
    { CSR_BT_SYNCS_SECURITY_IN_CFM, CsrBtSyncsSecurityInCfmSizeof, CsrBtSyncsSecurityInCfmSer, CsrBtSyncsSecurityInCfmDes, CsrBtSyncsSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ
    { CSR_BT_SYNCS_SECURITY_IN_REQ, CsrBtSyncsSecurityInReqSizeof, CsrBtSyncsSecurityInReqSer, CsrBtSyncsSecurityInReqDes, CsrBtSyncsSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSyncsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SYNCS_PRIM, csr_bt_syncs_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_syncs_conv_info = {
    CSR_BT_SYNCS_PRIM,
    "CSR_BT_SYNCS_PRIM",
    csr_bt_syncs_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSyncsTechInfoGet(void)
{
    return &csr_bt_syncs_conv_info;
}
#endif
#endif

