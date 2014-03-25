#ifndef CSR_BT_PAS_SEF_H__
#define CSR_BT_PAS_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

CsrUint8 CsrBtPasActivateReqHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasDeactivateReqHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasConnectResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasAuthenticateReqHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasAuthenticateResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasSetFolderResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullPbResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullPbSizeResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullMchResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullMchSizeResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullvCardListResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullvCardMchListResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullvCardMchListSizeResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullvCardEntryResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasSecurityInReqHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasPullvCardListSizeResHandler(PasInst *pInst, void *msg);
CsrUint8 CsrBtPasNextResHandler(PasInst *pInst, void *msg);

/* Prototypes from pas_free_down.c */
void CsrBtPasFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif

