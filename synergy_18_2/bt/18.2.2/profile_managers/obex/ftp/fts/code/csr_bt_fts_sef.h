#ifndef CSR_BT_FTS_SEF_H__
#define CSR_BT_FTS_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

CsrUint8 CsrBtFtsActivateReqHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsDeactivateReqHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsConnectResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsAuthenticateReqHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsAuthenticateResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsPutObjResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsPutObjNextResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsDelObjResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsGetObjResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsGetObjNextResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsGetListFolderResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsGetListFolderNextResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsSetFolderResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsSetBackFolderResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsSetRootFolderResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsSetAddFolderResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsSecurityInReqHandler(FtsInstanceDataType *pInst, void *msg);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
CsrUint8 CsrBtFtsCopyObjResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsMoveObjResHandler(FtsInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtFtsSetObjPermissionsResHandler(FtsInstanceDataType *pInst, void *msg);
#endif
/* Prototypes from fts_free_down.c */
void CsrBtFtsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif

