#ifndef CSR_BT_BIPC_SERIALIZE_H__
#define CSR_BT_BIPC_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtBipcPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND
CsrSize CsrBtBipcRcGetImagePropertiesIndSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImagePropertiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImagePropertiesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetImagePropertiesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM
CsrSize CsrBtBipcSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcSecurityOutCfmSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES
CsrSize CsrBtBipcRcGetMonitoringImageHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetMonitoringImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetMonitoringImageHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetMonitoringImageHeaderResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES
CsrSize CsrBtBipcAaGetImageObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetImageObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ
CsrSize CsrBtBipcPushPutImageReqSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutImageReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcPushPutImageReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES
CsrSize CsrBtBipcAaGetLinkedThumbnailHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedThumbnailHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetLinkedThumbnailHeaderResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND
CsrSize CsrBtBipcAaGetImageHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetImageHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES
CsrSize CsrBtBipcAaGetLinkedAttachmentObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedAttachmentObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetLinkedAttachmentObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ
CsrSize CsrBtBipcRcGetMonitoringImageReqSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetMonitoringImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetMonitoringImageReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetMonitoringImageReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_REQ
CsrSize CsrBtBipcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtBipcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ
CsrSize CsrBtBipcPushGetCapabilitiesReqSizeof(void *msg);
CsrUint8 *CsrBtBipcPushGetCapabilitiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushGetCapabilitiesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcPushGetCapabilitiesReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND
CsrSize CsrBtBipcPushPutThumbnailFileIndSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutThumbnailFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutThumbnailFileIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcPushPutThumbnailFileIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES
CsrSize CsrBtBipcRcGetLinkedThumbnailResSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetLinkedThumbnailResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetLinkedThumbnailResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES
CsrSize CsrBtBipcAaGetCapabilitiesHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetCapabilitiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetCapabilitiesHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetCapabilitiesHeaderResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_CFM
CsrSize CsrBtBipcAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAbortCfmSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND
CsrSize CsrBtBipcRcGetMonitoringImageHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetMonitoringImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetMonitoringImageHeaderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetMonitoringImageHeaderIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES
CsrSize CsrBtBipcRcGetImageResSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImageResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImageResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetImageResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ
CsrSize CsrBtBipcPushPutAttachmentReqSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutAttachmentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutAttachmentReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcPushPutAttachmentReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM
CsrSize CsrBtBipcRcGetImagePropertiesCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImagePropertiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImagePropertiesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetImagePropertiesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES
CsrSize CsrBtBipcAaGetLinkedAttachmentHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedAttachmentHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetLinkedAttachmentHeaderResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND
CsrSize CsrBtBipcPushPutAttachmentFileIndSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutAttachmentFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutAttachmentFileIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcPushPutAttachmentFileIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES
CsrSize CsrBtBipcPushGetCapabilitiesResSizeof(void *msg);
CsrUint8 *CsrBtBipcPushGetCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushGetCapabilitiesResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcPushGetCapabilitiesResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM
CsrSize CsrBtBipcRcGetMonitoringImageCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetMonitoringImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetMonitoringImageCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetMonitoringImageCfmSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES
CsrSize CsrBtBipcAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtBipcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES
CsrSize CsrBtBipcPushPutAttachmentFileResSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutAttachmentFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutAttachmentFileResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcPushPutAttachmentFileResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ
CsrSize CsrBtBipcRcGetImageReqSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImageReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetImageReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM
CsrSize CsrBtBipcRcGetImageCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImageCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetImageCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES
CsrSize CsrBtBipcAaGetLinkedThumbnailObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedThumbnailObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetLinkedThumbnailObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND
CsrSize CsrBtBipcAaGetLinkedAttachmentObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedAttachmentObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetLinkedAttachmentObjectIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ
CsrSize CsrBtBipcRcGetLinkedThumbnailReqSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetLinkedThumbnailReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetLinkedThumbnailReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND
CsrSize CsrBtBipcAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND
CsrSize CsrBtBipcPushPutImageFileIndSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutImageFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutImageFileIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcPushPutImageFileIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM
CsrSize CsrBtBipcGetRemoteFeaturesCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcGetRemoteFeaturesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcGetRemoteFeaturesCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcGetRemoteFeaturesCfmSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND
CsrSize CsrBtBipcAaGetImageListObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageListObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageListObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetImageListObjectIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND
CsrSize CsrBtBipcAaGetLinkedAttachmentHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedAttachmentHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetLinkedAttachmentHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES
CsrSize CsrBtBipcAaGetImagePropertiesObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImagePropertiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImagePropertiesObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetImagePropertiesObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND
CsrSize CsrBtBipcAaGetImageObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetImageObjectIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND
CsrSize CsrBtBipcRcGetMonitoringImageFileIndSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetMonitoringImageFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetMonitoringImageFileIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetMonitoringImageFileIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND
CsrSize CsrBtBipcAaGetCapabilitiesObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetCapabilitiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetCapabilitiesObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetCapabilitiesObjectIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND
CsrSize CsrBtBipcDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtBipcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcDisconnectIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM
CsrSize CsrBtBipcPushPutAttachmentCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutAttachmentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutAttachmentCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcPushPutAttachmentCfmSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND
CsrSize CsrBtBipcRcGetLinkedThumbnailIndSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetLinkedThumbnailIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetLinkedThumbnailIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ
CsrSize CsrBtBipcSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtBipcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcSecurityOutReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND
CsrSize CsrBtBipcAaGetImagePropertiesObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImagePropertiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImagePropertiesObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetImagePropertiesObjectIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND
CsrSize CsrBtBipcAaGetLinkedThumbnailHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedThumbnailHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetLinkedThumbnailHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ
CsrSize CsrBtBipcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtBipcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcDisconnectReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES
CsrSize CsrBtBipcAaGetImagePropertiesHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImagePropertiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImagePropertiesHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetImagePropertiesHeaderResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES
CsrSize CsrBtBipcAaGetCapabilitiesObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetCapabilitiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetCapabilitiesObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetCapabilitiesObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ
CsrSize CsrBtBipcRcGetImagePropertiesReqSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImagePropertiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImagePropertiesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetImagePropertiesReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ
CsrSize CsrBtBipcGetRemoteFeaturesReqSizeof(void *msg);
CsrUint8 *CsrBtBipcGetRemoteFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcGetRemoteFeaturesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcGetRemoteFeaturesReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND
CsrSize CsrBtBipcAaGetImagePropertiesHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImagePropertiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImagePropertiesHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetImagePropertiesHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES
CsrSize CsrBtBipcRcGetImagePropertiesResSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImagePropertiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImagePropertiesResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetImagePropertiesResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES
CsrSize CsrBtBipcAaGetImageHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetImageHeaderResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND
CsrSize CsrBtBipcPushGetCapabilitiesIndSizeof(void *msg);
CsrUint8 *CsrBtBipcPushGetCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushGetCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcPushGetCapabilitiesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES
CsrSize CsrBtBipcRcGetMonitoringImageFileResSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetMonitoringImageFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetMonitoringImageFileResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcRcGetMonitoringImageFileResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES
CsrSize CsrBtBipcAaGetImageListHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageListHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageListHeaderResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetImageListHeaderResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND
CsrSize CsrBtBipcAaDeleteImageIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaDeleteImageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaDeleteImageIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaDeleteImageIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND
CsrSize CsrBtBipcRcGetImageIndSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetImageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetImageIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetImageIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES
CsrSize CsrBtBipcAaGetImageListObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageListObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageListObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetImageListObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES
CsrSize CsrBtBipcPushPutImageFileResSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutImageFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutImageFileResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcPushPutImageFileResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_CFM
CsrSize CsrBtBipcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcConnectCfmSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM
CsrSize CsrBtBipcPushPutImageCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutImageCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcPushPutImageCfmSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND
CsrSize CsrBtBipcAaGetCapabilitiesHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetCapabilitiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetCapabilitiesHeaderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetCapabilitiesHeaderIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND
CsrSize CsrBtBipcAaGetImageListHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetImageListHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetImageListHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcAaGetImageListHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES
CsrSize CsrBtBipcPushPutThumbnailFileResSizeof(void *msg);
CsrUint8 *CsrBtBipcPushPutThumbnailFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushPutThumbnailFileResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcPushPutThumbnailFileResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM
CsrSize CsrBtBipcPushGetCapabilitiesCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcPushGetCapabilitiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcPushGetCapabilitiesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcPushGetCapabilitiesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES
CsrSize CsrBtBipcAaDeleteImageResSizeof(void *msg);
CsrUint8 *CsrBtBipcAaDeleteImageResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaDeleteImageResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaDeleteImageResSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_REQ
CsrSize CsrBtBipcAbortReqSizeof(void *msg);
CsrUint8 *CsrBtBipcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAbortReqSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM
CsrSize CsrBtBipcRcGetLinkedThumbnailCfmSizeof(void *msg);
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcRcGetLinkedThumbnailCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipcRcGetLinkedThumbnailCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND
CsrSize CsrBtBipcAaAbortIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaAbortIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaAbortIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND
CsrSize CsrBtBipcAaGetLinkedThumbnailObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipcAaGetLinkedThumbnailObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipcAaGetLinkedThumbnailObjectIndSerFree CsrBtBipcPfree
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND */

#ifdef __cplusplus
}
#endif

#endif
