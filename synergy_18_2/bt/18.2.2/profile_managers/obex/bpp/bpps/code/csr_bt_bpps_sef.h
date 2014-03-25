#ifndef CSR_BT_BPPS_SEF_H__
#define CSR_BT_BPPS_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_sc_private_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"

#define CsrBtBppsStateEventException(event, type)   CsrStateEventException("BPPS", (event), (type), 0)
#define CsrBtBppsGeneralException(theText)          CsrGeneralException("BPPS", 0, 0, theText)
#else
#define CsrBtBppsStateEventException(event, type)
#define CsrBtBppsGeneralException(theText)
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */

CsrUint8 CsrBtBppsActivateReqHandler(BppsInstanceData *pInst);
CsrUint8 CsrBtBppsDeactivateReqHandler(BppsInstanceData *pInst);
CsrUint8 CsrBtBppsConnectResHandler(BppsInstanceData *pInst);
CsrUint8 CsrBtBppsGetReferencedObjReqHandler(BppsInstanceData *pInst);
CsrUint8 CsrBtBppsSecurityInReqHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsAuthenticateResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsNextResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsGetReferencedObjResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsCreateJobResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsGetPrinterAttribsResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsGetJobAttribsResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsCancelJobResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsAuthenticateReqHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsGetEventResHandler(BppsInstanceData * pInst);
CsrUint8 CsrBtBppsJobCompleteReqHandler(BppsInstanceData * pInst);

/* Prototypes from csr_bt_bpps_free_down.c and csr_bt_bpps_free_up.c */
void CsrBtBppsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);
void CsrBtBppsFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BPPS_SEF_H__ */
