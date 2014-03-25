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

void CsrBtMapsMessagePut(CsrSchedQid phandle, void *msg);

CsrUint8 CsrBtMapsGetInstanceIdsReqHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsSelectMasInstanceResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsActivateReqHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsDeactivateReqHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsConnectResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsSetBackFolderResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsSetRootFolderResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsSetFolderResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsGetFolderListingHeaderResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsGetFolderListingResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsGetMessageListingHeaderResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsGetMessageListingResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsGetMessageHeaderResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsGetMessageResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsSetMessageStatusResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsPushMessageHeaderResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsPushMessageResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsUpdateInboxResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsNotificationRegistrationResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsEventNotificationReqHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsEventNotificationResHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsSecurityLevelReqHandler(MapsInstanceData *pInst, void *msg);
CsrUint8 CsrBtMapsRegisterQIDReqHandler(MapsInstanceData *pInst, void *msg);


void csrBtMapsResetAppHeaderTags(MapsInstanceData *pInst);

/* Prototypes from maps_free_down.c */
void CsrBtMapsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_MAPC_SEF_H__ */

