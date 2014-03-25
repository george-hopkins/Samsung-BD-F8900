#ifndef CSR_BT_BIPS_SERIALIZE_H__
#define CSR_BT_BIPS_SERIALIZE_H__
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

void CsrBtBipsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES
CsrSize CsrBtBipsPushGetCapabilitiesHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushGetCapabilitiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushGetCapabilitiesHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushGetCapabilitiesHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES
CsrSize CsrBtBipsRcGetMonitoringImageObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetMonitoringImageObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetMonitoringImageObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsRcGetMonitoringImageObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ
CsrSize CsrBtBipsAaGetCapabilitiesReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetCapabilitiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetCapabilitiesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetCapabilitiesReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ
CsrSize CsrBtBipsAaGetLinkedAttachmentReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedAttachmentReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetLinkedAttachmentReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM
CsrSize CsrBtBipsAaGetLinkedThumbnailCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedThumbnailCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetLinkedThumbnailCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM
CsrSize CsrBtBipsAaAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaAbortCfmSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES
CsrSize CsrBtBipsAaGetCapabilitiesResSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetCapabilitiesResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetCapabilitiesResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES
CsrSize CsrBtBipsRcGetLinkedThumbnailHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetLinkedThumbnailHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetLinkedThumbnailHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM
CsrSize CsrBtBipsAaGetImagePropertiesCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImagePropertiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImagePropertiesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImagePropertiesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND
CsrSize CsrBtBipsPushPutImageHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutImageHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsPushPutImageHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND
CsrSize CsrBtBipsAaGetLinkedAttachmentIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedAttachmentIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetLinkedAttachmentIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_IND
CsrSize CsrBtBipsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtBipsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsConnectIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM
CsrSize CsrBtBipsGetInstancesQidCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsGetInstancesQidCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsGetInstancesQidCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsGetInstancesQidCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ
CsrSize CsrBtBipsGetInstancesQidReqSizeof(void *msg);
CsrUint8 *CsrBtBipsGetInstancesQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsGetInstancesQidReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsGetInstancesQidReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ
CsrSize CsrBtBipsAaAbortReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaAbortReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND
CsrSize CsrBtBipsRcGetImagePropertiesHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImagePropertiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImagePropertiesHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsRcGetImagePropertiesHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES
CsrSize CsrBtBipsRcGetImagePropertiesObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImagePropertiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImagePropertiesObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsRcGetImagePropertiesObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND
CsrSize CsrBtBipsRcGetImageObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImageObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImageObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetImageObjectIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND
CsrSize CsrBtBipsRcGetMonitoringImageHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetMonitoringImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetMonitoringImageHeaderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetMonitoringImageHeaderIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND
CsrSize CsrBtBipsChallengeIndSizeof(void *msg);
CsrUint8 *CsrBtBipsChallengeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsChallengeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsChallengeIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND
CsrSize CsrBtBipsPushPutLinkedThumbnailHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedThumbnailHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsPushPutLinkedThumbnailHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES
CsrSize CsrBtBipsRcGetMonitoringImageHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetMonitoringImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetMonitoringImageHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetMonitoringImageHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES
CsrSize CsrBtBipsAaGetImageListHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageListHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageListHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetImageListHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ
CsrSize CsrBtBipsDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtBipsDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsDisconnectReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_RES
CsrSize CsrBtBipsConnectResSizeof(void *msg);
CsrUint8 *CsrBtBipsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsConnectResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND
CsrSize CsrBtBipsAaGetImageListHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageListHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageListHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImageListHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND
CsrSize CsrBtBipsRcGetMonitoringImageObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetMonitoringImageObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetMonitoringImageObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetMonitoringImageObjectIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ
CsrSize CsrBtBipsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtBipsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsActivateReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES
CsrSize CsrBtBipsAaGetImagePropertiesResSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImagePropertiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImagePropertiesResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetImagePropertiesResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND
CsrSize CsrBtBipsRcGetLinkedThumbnailObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetLinkedThumbnailObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetLinkedThumbnailObjectIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND
CsrSize CsrBtBipsRcGetImageHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImageHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsRcGetImageHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND
CsrSize CsrBtBipsPushGetCapabilitiesHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushGetCapabilitiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushGetCapabilitiesHeaderIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushGetCapabilitiesHeaderIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND
CsrSize CsrBtBipsRcGetImagePropertiesObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImagePropertiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImagePropertiesObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetImagePropertiesObjectIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES
CsrSize CsrBtBipsChallengeResSizeof(void *msg);
CsrUint8 *CsrBtBipsChallengeResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsChallengeResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsChallengeResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM
CsrSize CsrBtBipsAaGetLinkedAttachmentCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedAttachmentCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetLinkedAttachmentCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM
CsrSize CsrBtBipsAaGetCapabilitiesCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetCapabilitiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetCapabilitiesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetCapabilitiesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND
CsrSize CsrBtBipsAaGetImageListIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageListIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImageListIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES
CsrSize CsrBtBipsPushPutImageHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutImageHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushPutImageHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES
CsrSize CsrBtBipsPushPutLinkedThumbnailHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedThumbnailHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushPutLinkedThumbnailHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES
CsrSize CsrBtBipsPushPutLinkedAttachmentFileResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedAttachmentFileResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushPutLinkedAttachmentFileResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ
CsrSize CsrBtBipsAaDeleteImageReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaDeleteImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaDeleteImageReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaDeleteImageReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES
CsrSize CsrBtBipsPushPutImageFileResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutImageFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutImageFileResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushPutImageFileResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND
CsrSize CsrBtBipsPushPutLinkedThumbnailFileIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedThumbnailFileIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsPushPutLinkedThumbnailFileIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM
CsrSize CsrBtBipsAaGetImageCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImageCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND
CsrSize CsrBtBipsPushPutLinkedAttachmentHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedAttachmentHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsPushPutLinkedAttachmentHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES
CsrSize CsrBtBipsAaGetLinkedThumbnailResSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedThumbnailResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetLinkedThumbnailResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ
CsrSize CsrBtBipsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtBipsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsDeactivateReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM
CsrSize CsrBtBipsAaGetImageListCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageListCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageListCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImageListCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES
CsrSize CsrBtBipsAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtBipsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES
CsrSize CsrBtBipsAaGetLinkedAttachmentResSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedAttachmentResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetLinkedAttachmentResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES
CsrSize CsrBtBipsRcGetImageHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImageHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetImageHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND
CsrSize CsrBtBipsAaGetImageIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImageIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES
CsrSize CsrBtBipsRcGetImagePropertiesHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImagePropertiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImagePropertiesHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRcGetImagePropertiesHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ
CsrSize CsrBtBipsAaGetImageReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImageReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES
CsrSize CsrBtBipsAaGetImageResSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetImageResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND
CsrSize CsrBtBipsRcGetLinkedThumbnailHeaderIndSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetLinkedThumbnailHeaderIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsRcGetLinkedThumbnailHeaderIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES
CsrSize CsrBtBipsRcGetImageObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetImageObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetImageObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsRcGetImageObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES
CsrSize CsrBtBipsPushPutLinkedAttachmentHeaderResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedAttachmentHeaderResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushPutLinkedAttachmentHeaderResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND
CsrSize CsrBtBipsAaGetCapabilitiesIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetCapabilitiesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_ABORT_IND
CsrSize CsrBtBipsAbortIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAbortIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAbortIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND
CsrSize CsrBtBipsPushPutLinkedAttachmentFileIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedAttachmentFileIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsPushPutLinkedAttachmentFileIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND
CsrSize CsrBtBipsPushGetCapabilitiesObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushGetCapabilitiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushGetCapabilitiesObjectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushGetCapabilitiesObjectIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND
CsrSize CsrBtBipsAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM
CsrSize CsrBtBipsSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsSecurityInCfmSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND
CsrSize CsrBtBipsPushPutImageFileIndSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutImageFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutImageFileIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsPushPutImageFileIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND
CsrSize CsrBtBipsAaGetLinkedThumbnailIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedThumbnailIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetLinkedThumbnailIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ
CsrSize CsrBtBipsSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtBipsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsSecurityInReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ
CsrSize CsrBtBipsAaGetImagePropertiesReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImagePropertiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImagePropertiesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetImagePropertiesReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM
CsrSize CsrBtBipsAaDeleteImageCfmSizeof(void *msg);
CsrUint8 *CsrBtBipsAaDeleteImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaDeleteImageCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaDeleteImageCfmSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND
CsrSize CsrBtBipsAaGetImagePropertiesIndSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImagePropertiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImagePropertiesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImagePropertiesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ
CsrSize CsrBtBipsAaGetImageListReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageListReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsAaGetImageListReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND
CsrSize CsrBtBipsDeactivateIndSizeof(void *msg);
CsrUint8 *CsrBtBipsDeactivateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsDeactivateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsDeactivateIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES
CsrSize CsrBtBipsAaGetImageListResSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetImageListResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetImageListResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetImageListResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ
CsrSize CsrBtBipsRegisterQidReqSizeof(void *msg);
CsrUint8 *CsrBtBipsRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRegisterQidReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsRegisterQidReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES
CsrSize CsrBtBipsRcGetLinkedThumbnailObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsRcGetLinkedThumbnailObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsRcGetLinkedThumbnailObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND
CsrSize CsrBtBipsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtBipsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsDisconnectIndSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES
CsrSize CsrBtBipsPushGetCapabilitiesObjectResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushGetCapabilitiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushGetCapabilitiesObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBipsPushGetCapabilitiesObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ
CsrSize CsrBtBipsAaGetLinkedThumbnailReqSizeof(void *msg);
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsAaGetLinkedThumbnailReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsAaGetLinkedThumbnailReqSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES
CsrSize CsrBtBipsPushPutLinkedThumbnailFileResSizeof(void *msg);
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBipsPushPutLinkedThumbnailFileResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBipsPushPutLinkedThumbnailFileResSerFree CsrBtBipsPfree
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES */

#ifdef __cplusplus
}
#endif

#endif
