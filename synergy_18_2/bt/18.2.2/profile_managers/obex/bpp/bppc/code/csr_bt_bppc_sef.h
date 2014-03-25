#ifndef CSR_BT_BPPC_SEF_H__
#define CSR_BT_BPPC_SEF_H__

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

#define CsrBtBppcStateEventException(event, type)   CsrStateEventException("BPPC", (event), (type), 0)
#define CsrBtBppcGeneralException(theText)          CsrGeneralException("BPPC", 0, 0, theText)
#else
#define CsrBtBppcStateEventException(event, type)
#define CsrBtBppcGeneralException(theText)
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */

CsrUint8 CsrBtBppcConnectReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcDisconnectReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcCancelConnectReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcSecurityOutReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcAbortReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcAuthenticateResHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcGetPrintAttrReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcGetPrintAttrResHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcCreateJobReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcDocumentReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcDocumentResHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcGetJobAttrReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcGetJobAttrResHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcCancelJobReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcGetEventReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcGetEventResHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcActivateReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcDeactivateReqHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcConnectResHandler(BppcInstanceData *pInst, void *msg);
CsrUint8 CsrBtBppcGetReferenceObjectResHandler(BppcInstanceData *pInst, void *msg);

/* Prototypes from bppc_free_down.c */
void CsrBtBppcFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BPPC_SEF_H__ */
