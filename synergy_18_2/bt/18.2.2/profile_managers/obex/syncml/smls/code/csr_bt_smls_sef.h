#ifndef CSR_BT_SMLS_SEF_H__
#define CSR_BT_SMLS_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"

#define CsrBtSmlsStateEventException(event, type) CsrStateEventException("SMLS", (event), (type), 0)
#else
#define CsrBtSmlsStateEventException(event, type)
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */

CsrUint8 CsrBtSmlsActivateReqHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsConnectResHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsAuthenticateReqHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsDeactivateReqHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsAuthenticateResHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsPutSmlMsgObjResHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsPutSmlMsgObjNextResHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsGetSmlMsgObjResHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsGetSmlMsgObjNextResHandler(SmlsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtSmlsSecurityInReqHandler(SmlsInstanceDataType *pInst, void *msg);

/* Prototypes from smls_free_down.c */
void CsrBtSmlsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SMLS_SEF_H__ */
