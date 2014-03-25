#ifndef CSR_BT_FTC_SEF_H__
#define CSR_BT_FTC_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

CsrUint8 CsrBtFtcConnectReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcAuthenticateResHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcGetListFolderReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcGetListFolderBodyReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcGetObjReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcGetObjBodyReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcPutObjHeaderReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcPutObjBodyReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcDelObjReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcSetFolderReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcSetBackFolderReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcSetRootFolderReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcSetAddFolderReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcAbortReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcDisconnectReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcCancelConnectReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcSecurityOutReqHandler(FtcInstanceData *pInst, void *msg);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
CsrUint8 CsrBtFtcCopyingObjReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcMovingObjReqHandler(FtcInstanceData *pInst, void *msg);
CsrUint8 CsrBtFtcSetObjPermissionsReqHandler(FtcInstanceData *pInst, void *msg);
#endif
/* Prototypes from ftc_free_down.c */
void CsrBtFtcFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_FTC_SEF_H__ */

