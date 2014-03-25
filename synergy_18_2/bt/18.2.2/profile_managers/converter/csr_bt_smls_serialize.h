#ifndef CSR_BT_SMLS_SERIALIZE_H__
#define CSR_BT_SMLS_SERIALIZE_H__
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

void CsrBtSmlsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_IND
CsrSize CsrBtSmlsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsConnectIndSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM
CsrSize CsrBtSmlsAuthenticateCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsAuthenticateCfmSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ
CsrSize CsrBtSmlsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtSmlsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsActivateReqSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM
CsrSize CsrBtSmlsDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsDeactivateCfmSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES
CsrSize CsrBtSmlsPutSmlMsgObjResSizeof(void *msg);
CsrUint8 *CsrBtSmlsPutSmlMsgObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsPutSmlMsgObjResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsPutSmlMsgObjResSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND
CsrSize CsrBtSmlsGetSmlMsgObjIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsGetSmlMsgObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsGetSmlMsgObjIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsGetSmlMsgObjIndSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES
CsrSize CsrBtSmlsGetSmlMsgObjResSizeof(void *msg);
CsrUint8 *CsrBtSmlsGetSmlMsgObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsGetSmlMsgObjResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsGetSmlMsgObjResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_ABORT_IND
CsrSize CsrBtSmlsAbortIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND
CsrSize CsrBtSmlsGetSmlMsgObjNextIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsGetSmlMsgObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsGetSmlMsgObjNextIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsGetSmlMsgObjNextIndSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ
CsrSize CsrBtSmlsAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtSmlsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsAuthenticateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES
CsrSize CsrBtSmlsPutSmlMsgObjNextResSizeof(void *msg);
CsrUint8 *CsrBtSmlsPutSmlMsgObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsPutSmlMsgObjNextResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsPutSmlMsgObjNextResSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND
CsrSize CsrBtSmlsPutSmlMsgObjIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsPutSmlMsgObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsPutSmlMsgObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsPutSmlMsgObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM
CsrSize CsrBtSmlsSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsSecurityInCfmSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM
CsrSize CsrBtSmlsActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSmlsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsActivateCfmSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND
CsrSize CsrBtSmlsAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES
CsrSize CsrBtSmlsGetSmlMsgObjNextResSizeof(void *msg);
CsrUint8 *CsrBtSmlsGetSmlMsgObjNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsGetSmlMsgObjNextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsGetSmlMsgObjNextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND
CsrSize CsrBtSmlsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsDisconnectIndSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_CONNECT_RES
CsrSize CsrBtSmlsConnectResSizeof(void *msg);
CsrUint8 *CsrBtSmlsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsConnectResSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ
CsrSize CsrBtSmlsSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtSmlsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsSecurityInReqSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND
CsrSize CsrBtSmlsPutSmlMsgObjNextIndSizeof(void *msg);
CsrUint8 *CsrBtSmlsPutSmlMsgObjNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsPutSmlMsgObjNextIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsPutSmlMsgObjNextIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES
CsrSize CsrBtSmlsAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtSmlsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSmlsAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SMLS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ
CsrSize CsrBtSmlsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtSmlsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSmlsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSmlsDeactivateReqSerFree CsrBtSmlsPfree
#endif /* EXCLUDE_CSR_BT_SMLS_DEACTIVATE_REQ */

#ifdef __cplusplus
}
#endif

#endif
