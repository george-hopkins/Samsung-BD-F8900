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
#include "csr_bt_ftc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_FTC_MODULE
#include "csr_bt_ftc_serialize.h"
#include "csr_bt_ftc_prim.h"

static CsrMsgConvMsgEntry csr_bt_ftc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_FTC_ABORT_CFM
    { CSR_BT_FTC_ABORT_CFM, CsrBtFtcAbortCfmSizeof, CsrBtFtcAbortCfmSer, CsrBtFtcAbortCfmDes, CsrBtFtcAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_ABORT_REQ
    { CSR_BT_FTC_ABORT_REQ, CsrBtFtcAbortReqSizeof, CsrBtFtcAbortReqSer, CsrBtFtcAbortReqDes, CsrBtFtcAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND
    { CSR_BT_FTC_AUTHENTICATE_IND, CsrBtFtcAuthenticateIndSizeof, CsrBtFtcAuthenticateIndSer, CsrBtFtcAuthenticateIndDes, CsrBtFtcAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES
    { CSR_BT_FTC_AUTHENTICATE_RES, CsrBtFtcAuthenticateResSizeof, CsrBtFtcAuthenticateResSer, CsrBtFtcAuthenticateResDes, CsrBtFtcAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ
    { CSR_BT_FTC_CANCEL_CONNECT_REQ, CsrBtFtcCancelConnectReqSizeof, CsrBtFtcCancelConnectReqSer, CsrBtFtcCancelConnectReqDes, CsrBtFtcCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_CFM
    { CSR_BT_FTC_CONNECT_CFM, CsrBtFtcConnectCfmSizeof, CsrBtFtcConnectCfmSer, CsrBtFtcConnectCfmDes, CsrBtFtcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_REQ
    { CSR_BT_FTC_CONNECT_REQ, CsrBtFtcConnectReqSizeof, CsrBtFtcConnectReqSer, CsrBtFtcConnectReqDes, CsrBtFtcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM
    { CSR_BT_FTC_COPYING_OBJ_CFM, CsrBtFtcCopyingObjCfmSizeof, CsrBtFtcCopyingObjCfmSer, CsrBtFtcCopyingObjCfmDes, CsrBtFtcCopyingObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ
    { CSR_BT_FTC_COPYING_OBJ_REQ, CsrBtFtcCopyingObjReqSizeof, CsrBtFtcCopyingObjReqSer, CsrBtFtcCopyingObjReqDes, CsrBtFtcCopyingObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM
    { CSR_BT_FTC_DEL_OBJ_CFM, CsrBtFtcDelObjCfmSizeof, CsrBtFtcDelObjCfmSer, CsrBtFtcDelObjCfmDes, CsrBtFtcDelObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ
    { CSR_BT_FTC_DEL_OBJ_REQ, CsrBtFtcDelObjReqSizeof, CsrBtFtcDelObjReqSer, CsrBtFtcDelObjReqDes, CsrBtFtcDelObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_IND
    { CSR_BT_FTC_DISCONNECT_IND, CsrBtFtcDisconnectIndSizeof, CsrBtFtcDisconnectIndSer, CsrBtFtcDisconnectIndDes, CsrBtFtcDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ
    { CSR_BT_FTC_DISCONNECT_REQ, CsrBtFtcDisconnectReqSizeof, CsrBtFtcDisconnectReqSer, CsrBtFtcDisconnectReqDes, CsrBtFtcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM
    { CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM, CsrBtFtcGetListFolderBodyCfmSizeof, CsrBtFtcGetListFolderBodyCfmSer, CsrBtFtcGetListFolderBodyCfmDes, CsrBtFtcGetListFolderBodyCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ
    { CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ, CsrBtFtcGetListFolderBodyReqSizeof, CsrBtFtcGetListFolderBodyReqSer, CsrBtFtcGetListFolderBodyReqDes, CsrBtFtcGetListFolderBodyReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM
    { CSR_BT_FTC_GET_LIST_FOLDER_CFM, CsrBtFtcGetListFolderCfmSizeof, CsrBtFtcGetListFolderCfmSer, CsrBtFtcGetListFolderCfmDes, CsrBtFtcGetListFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ
    { CSR_BT_FTC_GET_LIST_FOLDER_REQ, CsrBtFtcGetListFolderReqSizeof, CsrBtFtcGetListFolderReqSer, CsrBtFtcGetListFolderReqDes, CsrBtFtcGetListFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM
    { CSR_BT_FTC_GET_OBJ_BODY_CFM, CsrBtFtcGetObjBodyCfmSizeof, CsrBtFtcGetObjBodyCfmSer, CsrBtFtcGetObjBodyCfmDes, CsrBtFtcGetObjBodyCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ
    { CSR_BT_FTC_GET_OBJ_BODY_REQ, CsrBtFtcGetObjBodyReqSizeof, CsrBtFtcGetObjBodyReqSer, CsrBtFtcGetObjBodyReqDes, CsrBtFtcGetObjBodyReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM
    { CSR_BT_FTC_GET_OBJ_CFM, CsrBtFtcGetObjCfmSizeof, CsrBtFtcGetObjCfmSer, CsrBtFtcGetObjCfmDes, CsrBtFtcGetObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ
    { CSR_BT_FTC_GET_OBJ_REQ, CsrBtFtcGetObjReqSizeof, CsrBtFtcGetObjReqSer, CsrBtFtcGetObjReqDes, CsrBtFtcGetObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM
    { CSR_BT_FTC_MOVING_OBJ_CFM, CsrBtFtcMovingObjCfmSizeof, CsrBtFtcMovingObjCfmSer, CsrBtFtcMovingObjCfmDes, CsrBtFtcMovingObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ
    { CSR_BT_FTC_MOVING_OBJ_REQ, CsrBtFtcMovingObjReqSizeof, CsrBtFtcMovingObjReqSer, CsrBtFtcMovingObjReqDes, CsrBtFtcMovingObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM
    { CSR_BT_FTC_PUT_OBJ_BODY_CFM, CsrBtFtcPutObjBodyCfmSizeof, CsrBtFtcPutObjBodyCfmSer, CsrBtFtcPutObjBodyCfmDes, CsrBtFtcPutObjBodyCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ
    { CSR_BT_FTC_PUT_OBJ_BODY_REQ, CsrBtFtcPutObjBodyReqSizeof, CsrBtFtcPutObjBodyReqSer, CsrBtFtcPutObjBodyReqDes, CsrBtFtcPutObjBodyReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM
    { CSR_BT_FTC_PUT_OBJ_HEADER_CFM, CsrBtFtcPutObjHeaderCfmSizeof, CsrBtFtcPutObjHeaderCfmSer, CsrBtFtcPutObjHeaderCfmDes, CsrBtFtcPutObjHeaderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ
    { CSR_BT_FTC_PUT_OBJ_HEADER_REQ, CsrBtFtcPutObjHeaderReqSizeof, CsrBtFtcPutObjHeaderReqSer, CsrBtFtcPutObjHeaderReqDes, CsrBtFtcPutObjHeaderReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM
    { CSR_BT_FTC_SECURITY_OUT_CFM, CsrBtFtcSecurityOutCfmSizeof, CsrBtFtcSecurityOutCfmSer, CsrBtFtcSecurityOutCfmDes, CsrBtFtcSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ
    { CSR_BT_FTC_SECURITY_OUT_REQ, CsrBtFtcSecurityOutReqSizeof, CsrBtFtcSecurityOutReqSer, CsrBtFtcSecurityOutReqDes, CsrBtFtcSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM
    { CSR_BT_FTC_SET_ADD_FOLDER_CFM, CsrBtFtcSetAddFolderCfmSizeof, CsrBtFtcSetAddFolderCfmSer, CsrBtFtcSetAddFolderCfmDes, CsrBtFtcSetAddFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ
    { CSR_BT_FTC_SET_ADD_FOLDER_REQ, CsrBtFtcSetAddFolderReqSizeof, CsrBtFtcSetAddFolderReqSer, CsrBtFtcSetAddFolderReqDes, CsrBtFtcSetAddFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM
    { CSR_BT_FTC_SET_BACK_FOLDER_CFM, CsrBtFtcSetBackFolderCfmSizeof, CsrBtFtcSetBackFolderCfmSer, CsrBtFtcSetBackFolderCfmDes, CsrBtFtcSetBackFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ
    { CSR_BT_FTC_SET_BACK_FOLDER_REQ, CsrBtFtcSetBackFolderReqSizeof, CsrBtFtcSetBackFolderReqSer, CsrBtFtcSetBackFolderReqDes, CsrBtFtcSetBackFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM
    { CSR_BT_FTC_SET_FOLDER_CFM, CsrBtFtcSetFolderCfmSizeof, CsrBtFtcSetFolderCfmSer, CsrBtFtcSetFolderCfmDes, CsrBtFtcSetFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ
    { CSR_BT_FTC_SET_FOLDER_REQ, CsrBtFtcSetFolderReqSizeof, CsrBtFtcSetFolderReqSer, CsrBtFtcSetFolderReqDes, CsrBtFtcSetFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM
    { CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM, CsrBtFtcSetObjPermissionsCfmSizeof, CsrBtFtcSetObjPermissionsCfmSer, CsrBtFtcSetObjPermissionsCfmDes, CsrBtFtcSetObjPermissionsCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ
    { CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ, CsrBtFtcSetObjPermissionsReqSizeof, CsrBtFtcSetObjPermissionsReqSer, CsrBtFtcSetObjPermissionsReqDes, CsrBtFtcSetObjPermissionsReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM
    { CSR_BT_FTC_SET_ROOT_FOLDER_CFM, CsrBtFtcSetRootFolderCfmSizeof, CsrBtFtcSetRootFolderCfmSer, CsrBtFtcSetRootFolderCfmDes, CsrBtFtcSetRootFolderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ
    { CSR_BT_FTC_SET_ROOT_FOLDER_REQ, CsrBtFtcSetRootFolderReqSizeof, CsrBtFtcSetRootFolderReqSer, CsrBtFtcSetRootFolderReqDes, CsrBtFtcSetRootFolderReqSerFree },
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtFtcConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_FTC_PRIM, csr_bt_ftc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_ftc_conv_info = {
    CSR_BT_FTC_PRIM,
    "CSR_BT_FTC_PRIM",
    csr_bt_ftc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtFtcTechInfoGet(void)
{
    return &csr_bt_ftc_conv_info;
}
#endif
#endif

