#ifndef CSR_BT_SYNCC_SEF_H__
#define CSR_BT_SYNCC_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

CsrUint8 CsrBtSynccConnectReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccCancelConnectReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccDisconnectReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccAuthenticateResHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccGetObjectReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccGetObjectResHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccAddObjectReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccAddObjectResHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccModifyObjectReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccModifyObjectResHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccDeleteObjectReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccAbortReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccActivateSyncCommandReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccSyncCommandResHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccDeactivateSyncCommandReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccSecurityInReqHandler(SynccInstanceData *pInst, void *msg);
CsrUint8 CsrBtSynccSecurityOutReqHandler(SynccInstanceData *pInst, void *msg);
CsrBool CsrBtSynccObexServerCmHandler(SynccInstanceData *pInst, void *msg);

/* Prototypes from syncc_free_down.c */
void CsrBtSynccFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SYNCC_SEF_H__ */

