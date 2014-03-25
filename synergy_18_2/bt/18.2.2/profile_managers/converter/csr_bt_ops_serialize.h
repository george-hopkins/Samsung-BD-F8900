#ifndef CSR_BT_OPS_SERIALIZE_H__
#define CSR_BT_OPS_SERIALIZE_H__
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

void CsrBtOpsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ
CsrSize CsrBtOpsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtOpsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsDeactivateReqSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ
CsrSize CsrBtOpsSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtOpsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsSecurityInReqSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM
CsrSize CsrBtOpsSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtOpsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsSecurityInCfmSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ
CsrSize CsrBtOpsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtOpsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsActivateReqSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_RES
CsrSize CsrBtOpsGetNextResSizeof(void *msg);
CsrUint8 *CsrBtOpsGetNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsGetNextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpsGetNextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_IND
CsrSize CsrBtOpsGetIndSizeof(void *msg);
CsrUint8 *CsrBtOpsGetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsGetIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsGetIndSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_GET_IND */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES
CsrSize CsrBtOpsAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtOpsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpsAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_IND
CsrSize CsrBtOpsGetNextIndSizeof(void *msg);
CsrUint8 *CsrBtOpsGetNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsGetNextIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsGetNextIndSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM
CsrSize CsrBtOpsDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtOpsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsDeactivateCfmSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_OPS_ABORT_IND
CsrSize CsrBtOpsAbortIndSizeof(void *msg);
CsrUint8 *CsrBtOpsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsAbortIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpsAbortIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_GET_RES
CsrSize CsrBtOpsGetResSizeof(void *msg);
CsrUint8 *CsrBtOpsGetResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsGetResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpsGetResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPS_GET_RES */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_IND
CsrSize CsrBtOpsPutIndSizeof(void *msg);
CsrUint8 *CsrBtOpsPutIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsPutIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpsPutIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_RES
CsrSize CsrBtOpsConnectResSizeof(void *msg);
CsrUint8 *CsrBtOpsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsConnectResSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_DISCONNECT_IND
CsrSize CsrBtOpsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtOpsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsDisconnectIndSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES
CsrSize CsrBtOpsPutNextResSizeof(void *msg);
CsrUint8 *CsrBtOpsPutNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsPutNextResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsPutNextResSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND
CsrSize CsrBtOpsAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtOpsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpsAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_RES
CsrSize CsrBtOpsPutResSizeof(void *msg);
CsrUint8 *CsrBtOpsPutResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsPutResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsPutResSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_OPS_CONNECT_IND
CsrSize CsrBtOpsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtOpsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtOpsConnectIndSerFree CsrBtOpsPfree
#endif /* EXCLUDE_CSR_BT_OPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND
CsrSize CsrBtOpsPutNextIndSizeof(void *msg);
CsrUint8 *CsrBtOpsPutNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtOpsPutNextIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtOpsPutNextIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND */

#ifdef __cplusplus
}
#endif

#endif
