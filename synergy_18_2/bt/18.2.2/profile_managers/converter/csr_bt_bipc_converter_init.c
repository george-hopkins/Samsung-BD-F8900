/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_bipc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_BIPC_MODULE
#include "csr_bt_bipc_serialize.h"
#include "csr_bt_bipc_prim.h"

static CsrMsgConvMsgEntry csr_bt_bipc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND
    { CSR_BT_BIPC_AA_ABORT_IND, CsrBtBipcAaAbortIndSizeof, CsrBtBipcAaAbortIndSer, CsrBtBipcAaAbortIndDes, CsrBtBipcAaAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND
    { CSR_BT_BIPC_AA_DELETE_IMAGE_IND, CsrBtBipcAaDeleteImageIndSizeof, CsrBtBipcAaDeleteImageIndSer, CsrBtBipcAaDeleteImageIndDes, CsrBtBipcAaDeleteImageIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES
    { CSR_BT_BIPC_AA_DELETE_IMAGE_RES, CsrBtBipcAaDeleteImageResSizeof, CsrBtBipcAaDeleteImageResSer, CsrBtBipcAaDeleteImageResDes, CsrBtBipcAaDeleteImageResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND
    { CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND, CsrBtBipcAaGetCapabilitiesHeaderIndSizeof, CsrBtBipcAaGetCapabilitiesHeaderIndSer, CsrBtBipcAaGetCapabilitiesHeaderIndDes, CsrBtBipcAaGetCapabilitiesHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES
    { CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES, CsrBtBipcAaGetCapabilitiesHeaderResSizeof, CsrBtBipcAaGetCapabilitiesHeaderResSer, CsrBtBipcAaGetCapabilitiesHeaderResDes, CsrBtBipcAaGetCapabilitiesHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND
    { CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND, CsrBtBipcAaGetCapabilitiesObjectIndSizeof, CsrBtBipcAaGetCapabilitiesObjectIndSer, CsrBtBipcAaGetCapabilitiesObjectIndDes, CsrBtBipcAaGetCapabilitiesObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES
    { CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES, CsrBtBipcAaGetCapabilitiesObjectResSizeof, CsrBtBipcAaGetCapabilitiesObjectResSer, CsrBtBipcAaGetCapabilitiesObjectResDes, CsrBtBipcAaGetCapabilitiesObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND
    { CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND, CsrBtBipcAaGetImageHeaderIndSizeof, CsrBtBipcAaGetImageHeaderIndSer, CsrBtBipcAaGetImageHeaderIndDes, CsrBtBipcAaGetImageHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES
    { CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES, CsrBtBipcAaGetImageHeaderResSizeof, CsrBtBipcAaGetImageHeaderResSer, CsrBtBipcAaGetImageHeaderResDes, CsrBtBipcAaGetImageHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND
    { CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND, CsrBtBipcAaGetImageListHeaderIndSizeof, CsrBtBipcAaGetImageListHeaderIndSer, CsrBtBipcAaGetImageListHeaderIndDes, CsrBtBipcAaGetImageListHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES
    { CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES, CsrBtBipcAaGetImageListHeaderResSizeof, CsrBtBipcAaGetImageListHeaderResSer, CsrBtBipcAaGetImageListHeaderResDes, CsrBtBipcAaGetImageListHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND
    { CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND, CsrBtBipcAaGetImageListObjectIndSizeof, CsrBtBipcAaGetImageListObjectIndSer, CsrBtBipcAaGetImageListObjectIndDes, CsrBtBipcAaGetImageListObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES
    { CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES, CsrBtBipcAaGetImageListObjectResSizeof, CsrBtBipcAaGetImageListObjectResSer, CsrBtBipcAaGetImageListObjectResDes, CsrBtBipcAaGetImageListObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND
    { CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND, CsrBtBipcAaGetImageObjectIndSizeof, CsrBtBipcAaGetImageObjectIndSer, CsrBtBipcAaGetImageObjectIndDes, CsrBtBipcAaGetImageObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES
    { CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES, CsrBtBipcAaGetImageObjectResSizeof, CsrBtBipcAaGetImageObjectResSer, CsrBtBipcAaGetImageObjectResDes, CsrBtBipcAaGetImageObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND
    { CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND, CsrBtBipcAaGetImagePropertiesHeaderIndSizeof, CsrBtBipcAaGetImagePropertiesHeaderIndSer, CsrBtBipcAaGetImagePropertiesHeaderIndDes, CsrBtBipcAaGetImagePropertiesHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES
    { CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES, CsrBtBipcAaGetImagePropertiesHeaderResSizeof, CsrBtBipcAaGetImagePropertiesHeaderResSer, CsrBtBipcAaGetImagePropertiesHeaderResDes, CsrBtBipcAaGetImagePropertiesHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND
    { CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND, CsrBtBipcAaGetImagePropertiesObjectIndSizeof, CsrBtBipcAaGetImagePropertiesObjectIndSer, CsrBtBipcAaGetImagePropertiesObjectIndDes, CsrBtBipcAaGetImagePropertiesObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES
    { CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES, CsrBtBipcAaGetImagePropertiesObjectResSizeof, CsrBtBipcAaGetImagePropertiesObjectResSer, CsrBtBipcAaGetImagePropertiesObjectResDes, CsrBtBipcAaGetImagePropertiesObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND
    { CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND, CsrBtBipcAaGetLinkedAttachmentHeaderIndSizeof, CsrBtBipcAaGetLinkedAttachmentHeaderIndSer, CsrBtBipcAaGetLinkedAttachmentHeaderIndDes, CsrBtBipcAaGetLinkedAttachmentHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES
    { CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES, CsrBtBipcAaGetLinkedAttachmentHeaderResSizeof, CsrBtBipcAaGetLinkedAttachmentHeaderResSer, CsrBtBipcAaGetLinkedAttachmentHeaderResDes, CsrBtBipcAaGetLinkedAttachmentHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND
    { CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND, CsrBtBipcAaGetLinkedAttachmentObjectIndSizeof, CsrBtBipcAaGetLinkedAttachmentObjectIndSer, CsrBtBipcAaGetLinkedAttachmentObjectIndDes, CsrBtBipcAaGetLinkedAttachmentObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES
    { CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES, CsrBtBipcAaGetLinkedAttachmentObjectResSizeof, CsrBtBipcAaGetLinkedAttachmentObjectResSer, CsrBtBipcAaGetLinkedAttachmentObjectResDes, CsrBtBipcAaGetLinkedAttachmentObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND
    { CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND, CsrBtBipcAaGetLinkedThumbnailHeaderIndSizeof, CsrBtBipcAaGetLinkedThumbnailHeaderIndSer, CsrBtBipcAaGetLinkedThumbnailHeaderIndDes, CsrBtBipcAaGetLinkedThumbnailHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES
    { CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES, CsrBtBipcAaGetLinkedThumbnailHeaderResSizeof, CsrBtBipcAaGetLinkedThumbnailHeaderResSer, CsrBtBipcAaGetLinkedThumbnailHeaderResDes, CsrBtBipcAaGetLinkedThumbnailHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND
    { CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND, CsrBtBipcAaGetLinkedThumbnailObjectIndSizeof, CsrBtBipcAaGetLinkedThumbnailObjectIndSer, CsrBtBipcAaGetLinkedThumbnailObjectIndDes, CsrBtBipcAaGetLinkedThumbnailObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES
    { CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES, CsrBtBipcAaGetLinkedThumbnailObjectResSizeof, CsrBtBipcAaGetLinkedThumbnailObjectResSer, CsrBtBipcAaGetLinkedThumbnailObjectResDes, CsrBtBipcAaGetLinkedThumbnailObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_CFM
    { CSR_BT_BIPC_ABORT_CFM, CsrBtBipcAbortCfmSizeof, CsrBtBipcAbortCfmSer, CsrBtBipcAbortCfmDes, CsrBtBipcAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_REQ
    { CSR_BT_BIPC_ABORT_REQ, CsrBtBipcAbortReqSizeof, CsrBtBipcAbortReqSer, CsrBtBipcAbortReqDes, CsrBtBipcAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND
    { CSR_BT_BIPC_AUTHENTICATE_IND, CsrBtBipcAuthenticateIndSizeof, CsrBtBipcAuthenticateIndSer, CsrBtBipcAuthenticateIndDes, CsrBtBipcAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES
    { CSR_BT_BIPC_AUTHENTICATE_RES, CsrBtBipcAuthenticateResSizeof, CsrBtBipcAuthenticateResSer, CsrBtBipcAuthenticateResDes, CsrBtBipcAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_CFM
    { CSR_BT_BIPC_CONNECT_CFM, CsrBtBipcConnectCfmSizeof, CsrBtBipcConnectCfmSer, CsrBtBipcConnectCfmDes, CsrBtBipcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_REQ
    { CSR_BT_BIPC_CONNECT_REQ, CsrBtBipcConnectReqSizeof, CsrBtBipcConnectReqSer, CsrBtBipcConnectReqDes, CsrBtBipcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND
    { CSR_BT_BIPC_DISCONNECT_IND, CsrBtBipcDisconnectIndSizeof, CsrBtBipcDisconnectIndSer, CsrBtBipcDisconnectIndDes, CsrBtBipcDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ
    { CSR_BT_BIPC_DISCONNECT_REQ, CsrBtBipcDisconnectReqSizeof, CsrBtBipcDisconnectReqSer, CsrBtBipcDisconnectReqDes, CsrBtBipcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM
    { CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM, CsrBtBipcGetRemoteFeaturesCfmSizeof, CsrBtBipcGetRemoteFeaturesCfmSer, CsrBtBipcGetRemoteFeaturesCfmDes, CsrBtBipcGetRemoteFeaturesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ
    { CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ, CsrBtBipcGetRemoteFeaturesReqSizeof, CsrBtBipcGetRemoteFeaturesReqSer, CsrBtBipcGetRemoteFeaturesReqDes, CsrBtBipcGetRemoteFeaturesReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM
    { CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM, CsrBtBipcPushGetCapabilitiesCfmSizeof, CsrBtBipcPushGetCapabilitiesCfmSer, CsrBtBipcPushGetCapabilitiesCfmDes, CsrBtBipcPushGetCapabilitiesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND
    { CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND, CsrBtBipcPushGetCapabilitiesIndSizeof, CsrBtBipcPushGetCapabilitiesIndSer, CsrBtBipcPushGetCapabilitiesIndDes, CsrBtBipcPushGetCapabilitiesIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ
    { CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ, CsrBtBipcPushGetCapabilitiesReqSizeof, CsrBtBipcPushGetCapabilitiesReqSer, CsrBtBipcPushGetCapabilitiesReqDes, CsrBtBipcPushGetCapabilitiesReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES
    { CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES, CsrBtBipcPushGetCapabilitiesResSizeof, CsrBtBipcPushGetCapabilitiesResSer, CsrBtBipcPushGetCapabilitiesResDes, CsrBtBipcPushGetCapabilitiesResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM
    { CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM, CsrBtBipcPushPutAttachmentCfmSizeof, CsrBtBipcPushPutAttachmentCfmSer, CsrBtBipcPushPutAttachmentCfmDes, CsrBtBipcPushPutAttachmentCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND
    { CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND, CsrBtBipcPushPutAttachmentFileIndSizeof, CsrBtBipcPushPutAttachmentFileIndSer, CsrBtBipcPushPutAttachmentFileIndDes, CsrBtBipcPushPutAttachmentFileIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES
    { CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES, CsrBtBipcPushPutAttachmentFileResSizeof, CsrBtBipcPushPutAttachmentFileResSer, CsrBtBipcPushPutAttachmentFileResDes, CsrBtBipcPushPutAttachmentFileResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ
    { CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ, CsrBtBipcPushPutAttachmentReqSizeof, CsrBtBipcPushPutAttachmentReqSer, CsrBtBipcPushPutAttachmentReqDes, CsrBtBipcPushPutAttachmentReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM
    { CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM, CsrBtBipcPushPutImageCfmSizeof, CsrBtBipcPushPutImageCfmSer, CsrBtBipcPushPutImageCfmDes, CsrBtBipcPushPutImageCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND
    { CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND, CsrBtBipcPushPutImageFileIndSizeof, CsrBtBipcPushPutImageFileIndSer, CsrBtBipcPushPutImageFileIndDes, CsrBtBipcPushPutImageFileIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES
    { CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES, CsrBtBipcPushPutImageFileResSizeof, CsrBtBipcPushPutImageFileResSer, CsrBtBipcPushPutImageFileResDes, CsrBtBipcPushPutImageFileResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ
    { CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ, CsrBtBipcPushPutImageReqSizeof, CsrBtBipcPushPutImageReqSer, CsrBtBipcPushPutImageReqDes, CsrBtBipcPushPutImageReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND
    { CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND, CsrBtBipcPushPutThumbnailFileIndSizeof, CsrBtBipcPushPutThumbnailFileIndSer, CsrBtBipcPushPutThumbnailFileIndDes, CsrBtBipcPushPutThumbnailFileIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES
    { CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES, CsrBtBipcPushPutThumbnailFileResSizeof, CsrBtBipcPushPutThumbnailFileResSer, CsrBtBipcPushPutThumbnailFileResDes, CsrBtBipcPushPutThumbnailFileResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM
    { CSR_BT_BIPC_RC_GET_IMAGE_CFM, CsrBtBipcRcGetImageCfmSizeof, CsrBtBipcRcGetImageCfmSer, CsrBtBipcRcGetImageCfmDes, CsrBtBipcRcGetImageCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND
    { CSR_BT_BIPC_RC_GET_IMAGE_IND, CsrBtBipcRcGetImageIndSizeof, CsrBtBipcRcGetImageIndSer, CsrBtBipcRcGetImageIndDes, CsrBtBipcRcGetImageIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM
    { CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM, CsrBtBipcRcGetImagePropertiesCfmSizeof, CsrBtBipcRcGetImagePropertiesCfmSer, CsrBtBipcRcGetImagePropertiesCfmDes, CsrBtBipcRcGetImagePropertiesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND
    { CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND, CsrBtBipcRcGetImagePropertiesIndSizeof, CsrBtBipcRcGetImagePropertiesIndSer, CsrBtBipcRcGetImagePropertiesIndDes, CsrBtBipcRcGetImagePropertiesIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ
    { CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ, CsrBtBipcRcGetImagePropertiesReqSizeof, CsrBtBipcRcGetImagePropertiesReqSer, CsrBtBipcRcGetImagePropertiesReqDes, CsrBtBipcRcGetImagePropertiesReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES
    { CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES, CsrBtBipcRcGetImagePropertiesResSizeof, CsrBtBipcRcGetImagePropertiesResSer, CsrBtBipcRcGetImagePropertiesResDes, CsrBtBipcRcGetImagePropertiesResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ
    { CSR_BT_BIPC_RC_GET_IMAGE_REQ, CsrBtBipcRcGetImageReqSizeof, CsrBtBipcRcGetImageReqSer, CsrBtBipcRcGetImageReqDes, CsrBtBipcRcGetImageReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES
    { CSR_BT_BIPC_RC_GET_IMAGE_RES, CsrBtBipcRcGetImageResSizeof, CsrBtBipcRcGetImageResSer, CsrBtBipcRcGetImageResDes, CsrBtBipcRcGetImageResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM
    { CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM, CsrBtBipcRcGetLinkedThumbnailCfmSizeof, CsrBtBipcRcGetLinkedThumbnailCfmSer, CsrBtBipcRcGetLinkedThumbnailCfmDes, CsrBtBipcRcGetLinkedThumbnailCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND
    { CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND, CsrBtBipcRcGetLinkedThumbnailIndSizeof, CsrBtBipcRcGetLinkedThumbnailIndSer, CsrBtBipcRcGetLinkedThumbnailIndDes, CsrBtBipcRcGetLinkedThumbnailIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ
    { CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ, CsrBtBipcRcGetLinkedThumbnailReqSizeof, CsrBtBipcRcGetLinkedThumbnailReqSer, CsrBtBipcRcGetLinkedThumbnailReqDes, CsrBtBipcRcGetLinkedThumbnailReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES
    { CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES, CsrBtBipcRcGetLinkedThumbnailResSizeof, CsrBtBipcRcGetLinkedThumbnailResSer, CsrBtBipcRcGetLinkedThumbnailResDes, CsrBtBipcRcGetLinkedThumbnailResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM
    { CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM, CsrBtBipcRcGetMonitoringImageCfmSizeof, CsrBtBipcRcGetMonitoringImageCfmSer, CsrBtBipcRcGetMonitoringImageCfmDes, CsrBtBipcRcGetMonitoringImageCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND
    { CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND, CsrBtBipcRcGetMonitoringImageFileIndSizeof, CsrBtBipcRcGetMonitoringImageFileIndSer, CsrBtBipcRcGetMonitoringImageFileIndDes, CsrBtBipcRcGetMonitoringImageFileIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES
    { CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES, CsrBtBipcRcGetMonitoringImageFileResSizeof, CsrBtBipcRcGetMonitoringImageFileResSer, CsrBtBipcRcGetMonitoringImageFileResDes, CsrBtBipcRcGetMonitoringImageFileResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND
    { CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND, CsrBtBipcRcGetMonitoringImageHeaderIndSizeof, CsrBtBipcRcGetMonitoringImageHeaderIndSer, CsrBtBipcRcGetMonitoringImageHeaderIndDes, CsrBtBipcRcGetMonitoringImageHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES
    { CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES, CsrBtBipcRcGetMonitoringImageHeaderResSizeof, CsrBtBipcRcGetMonitoringImageHeaderResSer, CsrBtBipcRcGetMonitoringImageHeaderResDes, CsrBtBipcRcGetMonitoringImageHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ
    { CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ, CsrBtBipcRcGetMonitoringImageReqSizeof, CsrBtBipcRcGetMonitoringImageReqSer, CsrBtBipcRcGetMonitoringImageReqDes, CsrBtBipcRcGetMonitoringImageReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM
    { CSR_BT_BIPC_SECURITY_OUT_CFM, CsrBtBipcSecurityOutCfmSizeof, CsrBtBipcSecurityOutCfmSer, CsrBtBipcSecurityOutCfmDes, CsrBtBipcSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ
    { CSR_BT_BIPC_SECURITY_OUT_REQ, CsrBtBipcSecurityOutReqSizeof, CsrBtBipcSecurityOutReqSer, CsrBtBipcSecurityOutReqDes, CsrBtBipcSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtBipcConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_BIPC_PRIM, csr_bt_bipc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_bipc_conv_info = {
    CSR_BT_BIPC_PRIM,
    "CSR_BT_BIPC_PRIM",
    csr_bt_bipc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtBipcTechInfoGet(void)
{
    return &csr_bt_bipc_conv_info;
}
#endif
#endif

