#ifndef CSR_BT_BIPC_SEF_H__
#define CSR_BT_BIPC_SEF_H__

#include "csr_synergy.h"
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

extern CsrUint8 CsrBtBipcConnectReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAbortReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcDisconnectReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAuthenticateResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcSecurityOutReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRemoteFeaturesReqHandler(BipcInstanceData *pInst, void *msg);

extern CsrUint8 CsrBtBipcImagePushGetCapabilitiesReqHandler(BipcInstanceData * pInst, void *msg);
extern CsrUint8 CsrBtBipcImagePushGetCapabilitiesResHandler(BipcInstanceData * pInst, void *msg);
extern CsrUint8 CsrBtBipcImagePushPutImageReqHandler(BipcInstanceData * pInst, void *msg);
extern CsrUint8 CsrBtBipcImagePushPutImageFileResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcImagePushPutThumbnailFileResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcImagePushPutLinkedAttachmentReqHandler(BipcInstanceData * pInst, void *msg);
extern CsrUint8 CsrBtBipcImagePushPutLinkedAttachmentResHandler(BipcInstanceData *pInst, void *msg);

extern CsrUint8 CsrBtBipcRcGetMonitoringImageReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetRcMonitoringImageHeaderResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetMonitoringImageFileResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetImagePropertiesReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetImagePropertiesResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetImageReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetImageResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetLinkedThumbnailReqHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcRcGetLinkedThumbnailResHandler(BipcInstanceData *pInst, void *msg);

extern CsrUint8 CsrBtBipcAaGetImageListHeaderResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetImageListObjectResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetCapabilitiesHeaderResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetCapabilitiesObjectResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetImagePropertiesHeaderResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetImagePropertiesObjectResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetImageHeaderResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetImageObjectResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetLinkedThumbnailHeaderResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetLinkedThumbnailObjectResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetLinkedAttachmentHeaderResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaGetLinkedAttachmentObjectResHandler(BipcInstanceData *pInst, void *msg);
extern CsrUint8 CsrBtBipcAaDeleteImageResHandler(BipcInstanceData *pInst, void *msg);


extern void CsrBtBipcExtractRemoteFeatureSdpAttributes(void              *inst,
                                                CmnCsrBtLinkedListStruct *sdpTagList,
                                                CsrBtDeviceAddr          deviceAddr,
                                                CsrBtResultCode          resultCode,
                                                CsrBtSupplier            resultSupplier);




/* Prototypes from bipc_free_down.c */
void CsrBtBipcFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_BIPC_SEF_H__ */
