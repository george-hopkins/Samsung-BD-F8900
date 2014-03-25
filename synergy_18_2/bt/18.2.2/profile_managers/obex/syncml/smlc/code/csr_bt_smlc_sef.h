#ifndef CSR_BT_SMLC_SEF_H__
#define CSR_BT_SMLC_SEF_H__

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

#define CsrBtSmlcStateEventException(event, type) CsrStateEventException("SMLC", (event), (type), 0)
#else
#define CsrBtSmlcStateEventException(event, type)
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */

CsrUint8 CsrBtSmlcActivateReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcDeactivateReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcConnectReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcAbortReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcPutSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcPutNextChunkSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcGetSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcGetNextChunkSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcAuthenticateResHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcCancelConnectReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcDisconnectReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcSecurityInReqHandler(SmlcInstanceData *pInst, void *msg);
CsrUint8 CsrBtSmlcSecurityOutReqHandler(SmlcInstanceData *pInst, void *msg);

/* Prototypes from smlc_free_down.c */
void CsrBtSmlcFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SMLC_SEF_H__ */
