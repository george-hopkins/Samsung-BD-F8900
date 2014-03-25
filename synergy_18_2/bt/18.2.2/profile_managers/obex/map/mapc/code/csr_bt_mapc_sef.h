#ifndef CSR_BT_MAPC_SEF_H__
#define CSR_BT_MAPC_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtMapcMessagePut(CsrSchedQid phandle, void *msg);

CsrUint8 CsrBtMapcGetInstanceIdsReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcConnectReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcCancelConnectReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcDisconnectReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcSelectMasInstanceResHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcSetFolderReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcSetBackFolderReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcSetRootFolderReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcGetFolderListingReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcGetFolderListingResHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcGetMessageListingReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcGetMessageListingResHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcGetMessageReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcGetMessageResHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcSetMessageStatusReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcPushMessageReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcPushMessageResHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcUpdateInboxReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcAbortReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcNotificationRegistrationReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcEventNotificationResHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcSecurityInReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcSecurityOutReqHandler(MapcInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapcRegisterQIDReqHandler(MapcInstanceData *pInst, void *msg);
CsrBool CsrBtMapcObexServerCmHandler(MapcInstanceData *pInst, void *msg);

/* Prototypes from mapc_free_down.c */
void CsrBtMapcFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_MAPC_SEF_H__ */

