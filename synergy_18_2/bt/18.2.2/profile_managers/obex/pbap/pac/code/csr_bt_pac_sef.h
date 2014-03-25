#ifndef CSR_BT_PAC_SEF_H__
#define CSR_BT_PAC_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

CsrUint8 CsrBtPacConnectReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacAuthenticateResHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacPullPbReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacPullPbResHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacPullvCardEntryReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacPullvCardEntryResHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacPullvCardListReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacPullvCardListResHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacSetFolderReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacSetBackFolderReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacSetRootFolderReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacAbortReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacDisconnectReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacCancelConnectReqHandler(PacInst *pInst, void *msg);
CsrUint8 CsrBtPacSecurityOutReqHandler(PacInst *pInst, void *msg);

void CsrBtPacCmSdsRegisterReqHandler(PacInst *pInst);
void CsrBtPacCmSdsRegistertCfmHandler(PacInst *pInst, void *msg);

/* Prototypes from pac_free_down.c */
void CsrBtPacFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_PAC_SEF_H__ */
