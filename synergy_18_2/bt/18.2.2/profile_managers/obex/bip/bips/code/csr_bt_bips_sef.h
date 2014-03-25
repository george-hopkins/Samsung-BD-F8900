#ifndef CSR_BT_BIPS_SEF_H__
#define CSR_BT_BIPS_SEF_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"

#define CsrBtBipsStateEventException(event, type) CsrStateEventException("BIPS", (event), (type), 0)
#else
#define CsrBtBipsStateEventException(event, type)
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */


void CsrBtBipsPfreeCollectedInfo(BipsInstanceData *pInst);
void CsrBtBipsPfreeAaObexPacket(BipsInstanceData *pInst);

/* Common downStream message handlers                                           */
CsrUint8 CsrBtBipsActivateReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsDeactivateReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsConnectResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAuthenticateResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsDisconnectReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsGetInstanceQidReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRegisterQidReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsSecurityInReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsChallengeResHandler(BipsInstanceData *pInst, void *msg);

/* Image Push downStream message handlers                                       */
CsrUint8 CsrBtBipsPushGetCapabilitiesHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsPushGetCapabilitiesObjectResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsPushPutImageHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsPushPutImageFileResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsPushPutLinkedThumbnailHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsPushPutLinkedThumbnailFileResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsPushPutLinkedAttachmentHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsPushPutLinkedAttachmentFileResHandler(BipsInstanceData *pInst, void *msg);

/* Remote Camera (RC) downStream message handlers                               */
CsrUint8 CsrBtBipsRcGetMonitoringHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRcGetMonitoringObjectResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRcGetImagePropertiesHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRcGetImagePropertiesObjectResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRcGetImageHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRcGetImageObjectResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRcGetLinkedThumbnailHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsRcGetLinkedThumbnailObjectResHandler(BipsInstanceData *pInst, void *msg);

/* Auto Archive (AA) downStream message handlers                                */
CsrUint8 CsrBtBipsAaGetImageListResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetImageListReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetImageListHeaderResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetCapabilitiesReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetCapabilitiesResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetImagePropertiesReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetImagePropertiesResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetImageReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetImageResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetLinkedAttachmentReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetLinkedAttachmentResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetLinkedThumbnailReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaGetLinkedThumbnailResHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaDeleteImageReqHandler(BipsInstanceData *pInst, void *msg);
CsrUint8 CsrBtBipsAaAbortReqHandler(BipsInstanceData *pInst, void *msg);


/* Prototypes from bips_free_down.c                                             */
void CsrBtBipsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

void CsrBtBipsMessagePut(CsrSchedQid phandle, void *msg);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BIPS_SEF_H__ */

