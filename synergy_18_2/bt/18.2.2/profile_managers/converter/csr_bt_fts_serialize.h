#ifndef CSR_BT_FTS_SERIALIZE_H__
#define CSR_BT_FTS_SERIALIZE_H__
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

void CsrBtFtsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM
CsrSize CsrBtFtsAuthenticateCfmSizeof(void *msg);
CsrUint8 *CsrBtFtsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsAuthenticateCfmSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND
CsrSize CsrBtFtsSetFolderIndSizeof(void *msg);
CsrUint8 *CsrBtFtsSetFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetFolderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsSetFolderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES
CsrSize CsrBtFtsMoveObjResSizeof(void *msg);
CsrUint8 *CsrBtFtsMoveObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsMoveObjResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsMoveObjResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND
CsrSize CsrBtFtsPutObjNextIndSizeof(void *msg);
CsrUint8 *CsrBtFtsPutObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsPutObjNextIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsPutObjNextIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES
CsrSize CsrBtFtsGetListFolderResSizeof(void *msg);
CsrUint8 *CsrBtFtsGetListFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetListFolderResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsGetListFolderResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ
CsrSize CsrBtFtsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtFtsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsDeactivateReqSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES
CsrSize CsrBtFtsSetBackFolderResSizeof(void *msg);
CsrUint8 *CsrBtFtsSetBackFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetBackFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSetBackFolderResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_RES
CsrSize CsrBtFtsConnectResSizeof(void *msg);
CsrUint8 *CsrBtFtsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsConnectResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM
CsrSize CsrBtFtsDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtFtsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsDeactivateCfmSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES
CsrSize CsrBtFtsSetFolderResSizeof(void *msg);
CsrUint8 *CsrBtFtsSetFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSetFolderResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SET_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND
CsrSize CsrBtFtsSetRootFolderIndSizeof(void *msg);
CsrUint8 *CsrBtFtsSetRootFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetRootFolderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSetRootFolderIndSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES
CsrSize CsrBtFtsGetObjNextResSizeof(void *msg);
CsrUint8 *CsrBtFtsGetObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetObjNextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsGetObjNextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ
CsrSize CsrBtFtsAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtFtsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsAuthenticateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND
CsrSize CsrBtFtsPutObjIndSizeof(void *msg);
CsrUint8 *CsrBtFtsPutObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsPutObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsPutObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_IND
CsrSize CsrBtFtsGetObjIndSizeof(void *msg);
CsrUint8 *CsrBtFtsGetObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsGetObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ
CsrSize CsrBtFtsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtFtsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsActivateReqSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_CONNECT_IND
CsrSize CsrBtFtsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtFtsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsConnectIndSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_RES
CsrSize CsrBtFtsGetObjResSizeof(void *msg);
CsrUint8 *CsrBtFtsGetObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetObjResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsGetObjResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES
CsrSize CsrBtFtsGetListFolderNextResSizeof(void *msg);
CsrUint8 *CsrBtFtsGetListFolderNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetListFolderNextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsGetListFolderNextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND
CsrSize CsrBtFtsMoveObjIndSizeof(void *msg);
CsrUint8 *CsrBtFtsMoveObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsMoveObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsMoveObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_MOVE_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND
CsrSize CsrBtFtsGetListFolderIndSizeof(void *msg);
CsrUint8 *CsrBtFtsGetListFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetListFolderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsGetListFolderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_ABORT_IND
CsrSize CsrBtFtsAbortIndSizeof(void *msg);
CsrUint8 *CsrBtFtsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND
CsrSize CsrBtFtsSetBackFolderIndSizeof(void *msg);
CsrUint8 *CsrBtFtsSetBackFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetBackFolderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSetBackFolderIndSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SET_BACK_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES
CsrSize CsrBtFtsCopyObjResSizeof(void *msg);
CsrUint8 *CsrBtFtsCopyObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsCopyObjResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsCopyObjResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES
CsrSize CsrBtFtsSetRootFolderResSizeof(void *msg);
CsrUint8 *CsrBtFtsSetRootFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetRootFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSetRootFolderResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SET_ROOT_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND
CsrSize CsrBtFtsGetListFolderNextIndSizeof(void *msg);
CsrUint8 *CsrBtFtsGetListFolderNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetListFolderNextIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsGetListFolderNextIndSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES
CsrSize CsrBtFtsDelObjResSizeof(void *msg);
CsrUint8 *CsrBtFtsDelObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsDelObjResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsDelObjResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DISCONNECT_IND
CsrSize CsrBtFtsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtFtsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsDisconnectIndSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND
CsrSize CsrBtFtsSetAddFolderIndSizeof(void *msg);
CsrUint8 *CsrBtFtsSetAddFolderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetAddFolderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsSetAddFolderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_IND */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES
CsrSize CsrBtFtsPutObjResSizeof(void *msg);
CsrUint8 *CsrBtFtsPutObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsPutObjResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsPutObjResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND
CsrSize CsrBtFtsAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtFtsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES
CsrSize CsrBtFtsSetObjPermissionsResSizeof(void *msg);
CsrUint8 *CsrBtFtsSetObjPermissionsResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetObjPermissionsResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSetObjPermissionsResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES */

#ifndef EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND
CsrSize CsrBtFtsGetObjNextIndSizeof(void *msg);
CsrUint8 *CsrBtFtsGetObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsGetObjNextIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsGetObjNextIndSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_GET_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES
CsrSize CsrBtFtsSetAddFolderResSizeof(void *msg);
CsrUint8 *CsrBtFtsSetAddFolderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetAddFolderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSetAddFolderResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SET_ADD_FOLDER_RES */

#ifndef EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES
CsrSize CsrBtFtsAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtFtsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND
CsrSize CsrBtFtsCopyObjIndSizeof(void *msg);
CsrUint8 *CsrBtFtsCopyObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsCopyObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsCopyObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_COPY_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND
CsrSize CsrBtFtsSetObjPermissionsIndSizeof(void *msg);
CsrUint8 *CsrBtFtsSetObjPermissionsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSetObjPermissionsIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsSetObjPermissionsIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM
CsrSize CsrBtFtsSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtFtsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSecurityInCfmSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ
CsrSize CsrBtFtsSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtFtsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsSecurityInReqSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES
CsrSize CsrBtFtsPutObjNextResSizeof(void *msg);
CsrUint8 *CsrBtFtsPutObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsPutObjNextResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtFtsPutObjNextResSerFree CsrBtFtsPfree
#endif /* EXCLUDE_CSR_BT_FTS_PUT_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND
CsrSize CsrBtFtsDelObjIndSizeof(void *msg);
CsrUint8 *CsrBtFtsDelObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtFtsDelObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtFtsDelObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_FTS_DEL_OBJ_IND */

#ifdef __cplusplus
}
#endif

#endif
