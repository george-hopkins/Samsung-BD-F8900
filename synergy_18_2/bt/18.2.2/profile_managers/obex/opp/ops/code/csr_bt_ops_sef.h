#ifndef CSR_BT_OPS_SEF_H__
#define CSR_BT_OPS_SEF_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_OPS_MODULE

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


extern CsrUint8 CsrBtOpsActivateReqHandler(OpsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtOpsDeactivateReqHandler(OpsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtOpsPutResHandler(OpsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtOpsGetResHandler(OpsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtOpsGetNextResHandler(OpsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtOpsConnectResHandler(OpsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtOpsSecurityInReqHandler(OpsInstanceDataType *pInst, void *msg);
#ifdef CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
extern CsrUint8 CsrBtOpsAuthenticateResHandler(OpsInstanceDataType *pInst, void *msg);
#endif /* CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */

/* Prototypes from ops_free_down.c */
void CsrBtOpsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_OPS_SEF_H__ */
#endif
