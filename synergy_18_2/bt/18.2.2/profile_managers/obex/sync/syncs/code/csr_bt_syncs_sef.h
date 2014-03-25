#ifndef CSR_BT_SYNCS_SEF_H__
#define CSR_BT_SYNCS_SEF_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_result.h"

extern CsrUint8 CsrBtSyncsActivateReqHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsActiveDeactivateReqHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsDeactivateActivationPendingStateHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsDeactivateReqHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsConnectResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsConnectResActiveStateHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsAuthenticateReqHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsAuthenticateReqActiveStateHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsAuthenticateResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetNextResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsPutResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsPutNextResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetDeviceInfoResHandler(SyncsInstanceDataType *pInst, void *msg);

extern CsrUint8 CsrBtSyncsGetPbChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetPbCurChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetPbInfoLogResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetPbEntryResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetPbAllResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsPutPbEntryResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsPutPbAddEntryResHandler(SyncsInstanceDataType *pInst, void *msg);

extern CsrUint8 CsrBtSyncsSecurityInReqHandler(SyncsInstanceDataType *pInst, void *msg);

extern CsrUint8 CsrBtSyncsGetCalChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetCalCurChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetCalInfoLogResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetCalEntryResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsGetCalAllResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsPutCalEntryResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsPutCalAddEntryResHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsSdsRegisterCfmHandler(SyncsInstanceDataType *pInst, void *msg);
extern CsrUint8 CsrBtSyncsSdsUnregisterCfmHandler(SyncsInstanceDataType *pInst, void *msg);


/* Prototypes from syncs_free_down.c */
void CsrBtSyncsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif
#endif
#endif
