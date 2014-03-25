#ifndef CSR_BT_FTC_SERIALIZE_H__
#define CSR_BT_FTC_SERIALIZE_H__
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

void CsrBtFtcPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ
CsrSize CsrBtFtcPutObjHeaderReqSizeof(void *msg);
CsrUint8 *CsrBtFtcPutObjHeaderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcPutObjHeaderReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcPutObjHeaderReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ
CsrSize CsrBtFtcSetFolderReqSizeof(void *msg);
CsrUint8 *CsrBtFtcSetFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetFolderReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcSetFolderReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_REQ
CsrSize CsrBtFtcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtFtcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM
CsrSize CsrBtFtcSetFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcSetFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSetFolderCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ
CsrSize CsrBtFtcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtFtcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcDisconnectReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_CFM
CsrSize CsrBtFtcAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcAbortCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_CFM
CsrSize CsrBtFtcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcConnectCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ
CsrSize CsrBtFtcPutObjBodyReqSizeof(void *msg);
CsrUint8 *CsrBtFtcPutObjBodyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcPutObjBodyReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcPutObjBodyReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM
CsrSize CsrBtFtcCopyingObjCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcCopyingObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcCopyingObjCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcCopyingObjCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM
CsrSize CsrBtFtcSetRootFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcSetRootFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetRootFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSetRootFolderCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM
CsrSize CsrBtFtcGetListFolderBodyCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcGetListFolderBodyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetListFolderBodyCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcGetListFolderBodyCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ
CsrSize CsrBtFtcSetObjPermissionsReqSizeof(void *msg);
CsrUint8 *CsrBtFtcSetObjPermissionsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetObjPermissionsReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcSetObjPermissionsReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ
CsrSize CsrBtFtcCopyingObjReqSizeof(void *msg);
CsrUint8 *CsrBtFtcCopyingObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcCopyingObjReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcCopyingObjReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ
CsrSize CsrBtFtcMovingObjReqSizeof(void *msg);
CsrUint8 *CsrBtFtcMovingObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcMovingObjReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcMovingObjReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ
CsrSize CsrBtFtcSetBackFolderReqSizeof(void *msg);
CsrUint8 *CsrBtFtcSetBackFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetBackFolderReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSetBackFolderReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ
CsrSize CsrBtFtcSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtFtcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSecurityOutReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ
CsrSize CsrBtFtcGetListFolderReqSizeof(void *msg);
CsrUint8 *CsrBtFtcGetListFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetListFolderReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcGetListFolderReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ
CsrSize CsrBtFtcCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtFtcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcCancelConnectReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM
CsrSize CsrBtFtcDelObjCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcDelObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcDelObjCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcDelObjCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM
CsrSize CsrBtFtcSetBackFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcSetBackFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetBackFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSetBackFolderCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SET_BACK_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES
CsrSize CsrBtFtcAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtFtcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ
CsrSize CsrBtFtcGetListFolderBodyReqSizeof(void *msg);
CsrUint8 *CsrBtFtcGetListFolderBodyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetListFolderBodyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcGetListFolderBodyReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM
CsrSize CsrBtFtcPutObjHeaderCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcPutObjHeaderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcPutObjHeaderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcPutObjHeaderCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ
CsrSize CsrBtFtcGetObjReqSizeof(void *msg);
CsrUint8 *CsrBtFtcGetObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetObjReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcGetObjReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ
CsrSize CsrBtFtcSetRootFolderReqSizeof(void *msg);
CsrUint8 *CsrBtFtcSetRootFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetRootFolderReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSetRootFolderReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SET_ROOT_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM
CsrSize CsrBtFtcSetAddFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcSetAddFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetAddFolderCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSetAddFolderCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM
CsrSize CsrBtFtcGetObjCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcGetObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetObjCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcGetObjCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM
CsrSize CsrBtFtcGetListFolderCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcGetListFolderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetListFolderCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcGetListFolderCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_ABORT_REQ
CsrSize CsrBtFtcAbortReqSizeof(void *msg);
CsrUint8 *CsrBtFtcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcAbortReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM
CsrSize CsrBtFtcMovingObjCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcMovingObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcMovingObjCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcMovingObjCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ
CsrSize CsrBtFtcGetObjBodyReqSizeof(void *msg);
CsrUint8 *CsrBtFtcGetObjBodyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetObjBodyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcGetObjBodyReqSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_DISCONNECT_IND
CsrSize CsrBtFtcDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtFtcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcDisconnectIndSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM
CsrSize CsrBtFtcPutObjBodyCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcPutObjBodyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcPutObjBodyCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcPutObjBodyCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM
CsrSize CsrBtFtcSetObjPermissionsCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcSetObjPermissionsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetObjPermissionsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSetObjPermissionsCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ
CsrSize CsrBtFtcSetAddFolderReqSizeof(void *msg);
CsrUint8 *CsrBtFtcSetAddFolderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSetAddFolderReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcSetAddFolderReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND
CsrSize CsrBtFtcAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtFtcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM
CsrSize CsrBtFtcSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtcSecurityOutCfmSerFree CsrBtFtcPfree
#endif /* EXCLUDE_CSR_BT_FTC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ
CsrSize CsrBtFtcDelObjReqSizeof(void *msg);
CsrUint8 *CsrBtFtcDelObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcDelObjReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcDelObjReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM
CsrSize CsrBtFtcGetObjBodyCfmSizeof(void *msg);
CsrUint8 *CsrBtFtcGetObjBodyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtcGetObjBodyCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtcGetObjBodyCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM */

#ifdef __cplusplus
}
#endif

#endif
