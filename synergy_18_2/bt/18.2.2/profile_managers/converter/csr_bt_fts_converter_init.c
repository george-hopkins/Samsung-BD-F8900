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
#include "csr_bt_fts_converter_init.h"
#ifndef EXCLUDE_CSR_BT_FTS_MODULE
#include "csr_bt_fts_serialize.h"
#include "csr_bt_fts_prim.h"

static CsrMsgConvMsgEntry csr_bt_fts_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_FTS_ABORT_IND
    { CSR_BT_FTS_ABORT_IND, CsrBtFtsAbortIndSizeof, CsrBtFtsAbortIndSer, CsrBtFtsAbortIndDes, CsrBtFtsAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ
    { CSR_BT_FTS_ACTIVATE_REQ, CsrBtFtsActivateReqSizeof, CsrBtFtsActivateReqSer, CsrBtFtsActivateReqDes, CsrBtFtsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM
    { CSR_BT_FTS_AUTHENTICATE_CFM, CsrBtFtsAuthenticateCfmSizeof, CsrBtFtsAuthenticateCfmSer, CsrBtFtsAuthenticateCfmDes, CsrBtFtsAuthenticateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM */
#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND
    { CSR_BT_FTS_AUTHENTICATE_IND, CsrBtFtsAuthenticateIndSizeof, CsrBtFtsAuthenticateIndSer, CsrBtFtsAuthenticateIndDes, CsrBtFtsAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ
    { CSR_BT_FTS_AUTHENTICATE_REQ, CsrBtFtsAuthenticateReqSizeof, CsrBtFtsAuthenticateReqSer, CsrBtFtsAuthenticateReqDes, CsrBtFtsAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES
    { CSR_BT_FTS_AUTHENTICATE_RES, CsrBtFtsAuthenticateResSizeof, CsrBtFtsAuthenticateResSer, CsrBtFtsAuthenticateResDes, CsrBtFtsAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_IND
    { CSR_BT_FTS_CONNECT_IND, CsrBtFtsConnectIndSizeof, CsrBtFtsConnectIndSer, CsrBtFtsConnectIndDes, CsrBtFtsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_RES
    { CSR_BT_FTS_CONNECT_RES, CsrBtFtsConnectResSizeof, CsrBtFtsConnectResSer, CsrBtFtsConnectResDes, CsrBtFtsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND
    { CSR_BT_FTS_COPY_OBJ_IND, CsrBtFtsCopyObjIndSizeof, CsrBtFtsCopyObjIndSer, CsrBtFtsCopyObjIndDes, CsrBtFtsCopyObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES
    { CSR_BT_FTS_COPY_OBJ_RES, CsrBtFtsCopyObjResSizeof, CsrBtFtsCopyObjResSer, CsrBtFtsCopyObjResDes, CsrBtFtsCopyObjResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM
    { CSR_BT_FTS_DEACTIVATE_CFM, CsrBtFtsDeactivateCfmSizeof, CsrBtFtsDeactivateCfmSer, CsrBtFtsDeactivateCfmDes, CsrBtFtsDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ
    { CSR_BT_FTS_DEACTIVATE_REQ, CsrBtFtsDeactivateReqSizeof, CsrBtFtsDeactivateReqSer, CsrBtFtsDeactivateReqDes, CsrBtFtsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND
    { CSR_BT_FTS_DEL_OBJ_IND, CsrBtFtsDelObjIndSizeof, CsrBtFtsDelObjIndSer, CsrBtFtsDelObjIndDes, CsrBtFtsDelObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES
    { CSR_BT_FTS_DEL_OBJ_RES, CsrBtFtsDelObjResSizeof, CsrBtFtsDelObjResSer, CsrBtFtsDelObjResDes, CsrBtFtsDelObjResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_FTS_DISCONNECT_IND
    { CSR_BT_FTS_DISCONNECT_IND, CsrBtFtsDisconnectIndSizeof, CsrBtFtsDisconnectIndSer, CsrBtFtsDisconnectIndDes, CsrBtFtsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND
    { CSR_BT_FTS_GET_LIST_FOLDER_IND, CsrBtFtsGetListFolderIndSizeof, CsrBtFtsGetListFolderIndSer, CsrBtFtsGetListFolderIndDes, CsrBtFtsGetListFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND
    { CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND, CsrBtFtsGetListFolderNextIndSizeof, CsrBtFtsGetListFolderNextIndSer, CsrBtFtsGetListFolderNextIndDes, CsrBtFtsGetListFolderNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES
    { CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES, CsrBtFtsGetListFolderNextResSizeof, CsrBtFtsGetListFolderNextResSer, CsrBtFtsGetListFolderNextResDes, CsrBtFtsGetListFolderNextResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES
    { CSR_BT_FTS_GET_LIST_FOLDER_RES, CsrBtFtsGetListFolderResSizeof, CsrBtFtsGetListFolderResSer, CsrBtFtsGetListFolderResDes, CsrBtFtsGetListFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_IND
    { CSR_BT_FTS_GET_OBJ_IND, CsrBtFtsGetObjIndSizeof, CsrBtFtsGetObjIndSer, CsrBtFtsGetObjIndDes, CsrBtFtsGetObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND
    { CSR_BT_FTS_GET_OBJ_NEXT_IND, CsrBtFtsGetObjNextIndSizeof, CsrBtFtsGetObjNextIndSer, CsrBtFtsGetObjNextIndDes, CsrBtFtsGetObjNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES
    { CSR_BT_FTS_GET_OBJ_NEXT_RES, CsrBtFtsGetObjNextResSizeof, CsrBtFtsGetObjNextResSer, CsrBtFtsGetObjNextResDes, CsrBtFtsGetObjNextResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_RES
    { CSR_BT_FTS_GET_OBJ_RES, CsrBtFtsGetObjResSizeof, CsrBtFtsGetObjResSer, CsrBtFtsGetObjResDes, CsrBtFtsGetObjResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND
    { CSR_BT_FTS_MOVE_OBJ_IND, CsrBtFtsMoveObjIndSizeof, CsrBtFtsMoveObjIndSer, CsrBtFtsMoveObjIndDes, CsrBtFtsMoveObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES
    { CSR_BT_FTS_MOVE_OBJ_RES, CsrBtFtsMoveObjResSizeof, CsrBtFtsMoveObjResSer, CsrBtFtsMoveObjResDes, CsrBtFtsMoveObjResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND
    { CSR_BT_FTS_PUT_OBJ_IND, CsrBtFtsPutObjIndSizeof, CsrBtFtsPutObjIndSer, CsrBtFtsPutObjIndDes, CsrBtFtsPutObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND
    { CSR_BT_FTS_PUT_OBJ_NEXT_IND, CsrBtFtsPutObjNextIndSizeof, CsrBtFtsPutObjNextIndSer, CsrBtFtsPutObjNextIndDes, CsrBtFtsPutObjNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES
    { CSR_BT_FTS_PUT_OBJ_NEXT_RES, CsrBtFtsPutObjNextResSizeof, CsrBtFtsPutObjNextResSer, CsrBtFtsPutObjNextResDes, CsrBtFtsPutObjNextResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES
    { CSR_BT_FTS_PUT_OBJ_RES, CsrBtFtsPutObjResSizeof, CsrBtFtsPutObjResSer, CsrBtFtsPutObjResDes, CsrBtFtsPutObjResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM
    { CSR_BT_FTS_SECURITY_IN_CFM, CsrBtFtsSecurityInCfmSizeof, CsrBtFtsSecurityInCfmSer, CsrBtFtsSecurityInCfmDes, CsrBtFtsSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ
    { CSR_BT_FTS_SECURITY_IN_REQ, CsrBtFtsSecurityInReqSizeof, CsrBtFtsSecurityInReqSer, CsrBtFtsSecurityInReqDes, CsrBtFtsSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND
    { CSR_BT_FTS_SET_ADD_FOLDER_IND, CsrBtFtsSetAddFolderIndSizeof, CsrBtFtsSetAddFolderIndSer, CsrBtFtsSetAddFolderIndDes, CsrBtFtsSetAddFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES
    { CSR_BT_FTS_SET_ADD_FOLDER_RES, CsrBtFtsSetAddFolderResSizeof, CsrBtFtsSetAddFolderResSer, CsrBtFtsSetAddFolderResDes, CsrBtFtsSetAddFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND
    { CSR_BT_FTS_SET_BACK_FOLDER_IND, CsrBtFtsSetBackFolderIndSizeof, CsrBtFtsSetBackFolderIndSer, CsrBtFtsSetBackFolderIndDes, CsrBtFtsSetBackFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES
    { CSR_BT_FTS_SET_BACK_FOLDER_RES, CsrBtFtsSetBackFolderResSizeof, CsrBtFtsSetBackFolderResSer, CsrBtFtsSetBackFolderResDes, CsrBtFtsSetBackFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND
    { CSR_BT_FTS_SET_FOLDER_IND, CsrBtFtsSetFolderIndSizeof, CsrBtFtsSetFolderIndSer, CsrBtFtsSetFolderIndDes, CsrBtFtsSetFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES
    { CSR_BT_FTS_SET_FOLDER_RES, CsrBtFtsSetFolderResSizeof, CsrBtFtsSetFolderResSer, CsrBtFtsSetFolderResDes, CsrBtFtsSetFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES */
#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND
    { CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND, CsrBtFtsSetObjPermissionsIndSizeof, CsrBtFtsSetObjPermissionsIndSer, CsrBtFtsSetObjPermissionsIndDes, CsrBtFtsSetObjPermissionsIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND */
#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES
    { CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES, CsrBtFtsSetObjPermissionsResSizeof, CsrBtFtsSetObjPermissionsResSer, CsrBtFtsSetObjPermissionsResDes, CsrBtFtsSetObjPermissionsResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES */
#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND
    { CSR_BT_FTS_SET_ROOT_FOLDER_IND, CsrBtFtsSetRootFolderIndSizeof, CsrBtFtsSetRootFolderIndSer, CsrBtFtsSetRootFolderIndDes, CsrBtFtsSetRootFolderIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES
    { CSR_BT_FTS_SET_ROOT_FOLDER_RES, CsrBtFtsSetRootFolderResSizeof, CsrBtFtsSetRootFolderResSer, CsrBtFtsSetRootFolderResDes, CsrBtFtsSetRootFolderResSerFree },
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtFtsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_FTS_PRIM, csr_bt_fts_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_fts_conv_info = {
    CSR_BT_FTS_PRIM,
    "CSR_BT_FTS_PRIM",
    csr_bt_fts_conv_lut
};

CsrLogPrimitiveInformation* CsrBtFtsTechInfoGet(void)
{
    return &csr_bt_fts_conv_info;
}
#endif
#endif

