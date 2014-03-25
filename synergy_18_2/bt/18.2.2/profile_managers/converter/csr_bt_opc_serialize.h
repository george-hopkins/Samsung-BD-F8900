#ifndef CSR_BT_OPC_SERIALIZE_H__
#define CSR_BT_OPC_SERIALIZE_H__
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

void CsrBtOpcPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_IND
CsrSize CsrBtOpcDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtOpcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcDisconnectIndSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ
CsrSize CsrBtOpcGetObjectReqSizeof(void *msg);
CsrUint8 *CsrBtOpcGetObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcGetObjectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcGetObjectReqSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ
CsrSize CsrBtOpcCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtOpcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcCancelConnectReqSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM
CsrSize CsrBtOpcGetHeaderCfmSizeof(void *msg);
CsrUint8 *CsrBtOpcGetHeaderCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcGetHeaderCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpcGetHeaderCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ
CsrSize CsrBtOpcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtOpcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcDisconnectReqSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_REQ
CsrSize CsrBtOpcAbortReqSizeof(void *msg);
CsrUint8 *CsrBtOpcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcAbortReqSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_ABORT_CFM
CsrSize CsrBtOpcAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtOpcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcAbortCfmSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND
CsrSize CsrBtOpcAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtOpcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpcAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM
CsrSize CsrBtOpcSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtOpcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcSecurityOutCfmSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_CFM
CsrSize CsrBtOpcPutCfmSizeof(void *msg);
CsrUint8 *CsrBtOpcPutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcPutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcPutCfmSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_PUT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES
CsrSize CsrBtOpcPutObjectResSizeof(void *msg);
CsrUint8 *CsrBtOpcPutObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcPutObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpcPutObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES
CsrSize CsrBtOpcAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtOpcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpcAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND
CsrSize CsrBtOpcPutObjectIndSizeof(void *msg);
CsrUint8 *CsrBtOpcPutObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcPutObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcPutObjectIndSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM
CsrSize CsrBtOpcGetObjectCfmSizeof(void *msg);
CsrUint8 *CsrBtOpcGetObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcGetObjectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpcGetObjectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_REQ
CsrSize CsrBtOpcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtOpcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpcConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_CFM
CsrSize CsrBtOpcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtOpcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcConnectCfmSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ
CsrSize CsrBtOpcSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtOpcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcSecurityOutReqSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_PUT_REQ
CsrSize CsrBtOpcPutReqSizeof(void *msg);
CsrUint8 *CsrBtOpcPutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcPutReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpcPutReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPC_PUT_REQ */

#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ
CsrSize CsrBtOpcGetHeaderReqSizeof(void *msg);
CsrUint8 *CsrBtOpcGetHeaderReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpcGetHeaderReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpcGetHeaderReqSerFree CsrBtOpcPfree
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ */

#ifdef __cplusplus
}
#endif

#endif
