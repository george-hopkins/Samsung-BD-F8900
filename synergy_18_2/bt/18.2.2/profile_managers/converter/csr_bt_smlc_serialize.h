#ifndef CSR_BT_SMLC_SERIALIZE_H__
#define CSR_BT_SMLC_SERIALIZE_H__
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

void CsrBtSmlcPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ
CsrSize CsrBtSmlcCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcCancelConnectReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcGetSmlMsgObjReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcGetSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcGetSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcGetSmlMsgObjReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM
CsrSize CsrBtSmlcDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcDeactivateCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcGetNextChunkSmlMsgObjCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcGetNextChunkSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcGetNextChunkSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlcGetNextChunkSmlMsgObjCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ
CsrSize CsrBtSmlcActivateReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcActivateReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcPutNextChunkSmlMsgObjCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcPutNextChunkSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcPutNextChunkSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcPutNextChunkSmlMsgObjCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM
CsrSize CsrBtSmlcSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcSecurityOutCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcGetNextChunkSmlMsgObjReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcGetNextChunkSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcGetNextChunkSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcGetNextChunkSmlMsgObjReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES
CsrSize CsrBtSmlcAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtSmlcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlcAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcGetSmlMsgObjCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcGetSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcGetSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlcGetSmlMsgObjCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcPutSmlMsgObjReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcPutSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcPutSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlcPutSmlMsgObjReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_REQ
CsrSize CsrBtSmlcAbortReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcAbortReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ABORT_CFM
CsrSize CsrBtSmlcAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcAbortCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND
CsrSize CsrBtSmlcDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSmlcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcDisconnectIndSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ
CsrSize CsrBtSmlcSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcSecurityOutReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ
CsrSize CsrBtSmlcDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcDeactivateReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_CFM
CsrSize CsrBtSmlcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcConnectCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM
CsrSize CsrBtSmlcPutSmlMsgObjCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcPutSmlMsgObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcPutSmlMsgObjCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcPutSmlMsgObjCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ
CsrSize CsrBtSmlcSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcSecurityInReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM
CsrSize CsrBtSmlcActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcActivateCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ
CsrSize CsrBtSmlcPutNextChunkSmlMsgObjReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcPutNextChunkSmlMsgObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcPutNextChunkSmlMsgObjReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlcPutNextChunkSmlMsgObjReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_CONNECT_REQ
CsrSize CsrBtSmlcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlcConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND
CsrSize CsrBtSmlcAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtSmlcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlcAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM
CsrSize CsrBtSmlcSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlcSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcSecurityInCfmSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ
CsrSize CsrBtSmlcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtSmlcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlcDisconnectReqSerFree CsrBtSmlcPfree
#endif /* EXCLUDE_CSR_BT_SMLC_DISCONNECT_REQ */

#ifdef __cplusplus
}
#endif

#endif
