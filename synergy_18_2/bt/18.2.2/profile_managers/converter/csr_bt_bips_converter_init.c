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
#include "csr_bt_bips_converter_init.h"
#ifndef EXCLUDE_CSR_BT_BIPS_MODULE
#include "csr_bt_bips_serialize.h"
#include "csr_bt_bips_prim.h"

static CsrMsgConvMsgEntry csr_bt_bips_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM
    { CSR_BT_BIPS_AA_ABORT_CFM, CsrBtBipsAaAbortCfmSizeof, CsrBtBipsAaAbortCfmSer, CsrBtBipsAaAbortCfmDes, CsrBtBipsAaAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ
    { CSR_BT_BIPS_AA_ABORT_REQ, CsrBtBipsAaAbortReqSizeof, CsrBtBipsAaAbortReqSer, CsrBtBipsAaAbortReqDes, CsrBtBipsAaAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM
    { CSR_BT_BIPS_AA_DELETE_IMAGE_CFM, CsrBtBipsAaDeleteImageCfmSizeof, CsrBtBipsAaDeleteImageCfmSer, CsrBtBipsAaDeleteImageCfmDes, CsrBtBipsAaDeleteImageCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ
    { CSR_BT_BIPS_AA_DELETE_IMAGE_REQ, CsrBtBipsAaDeleteImageReqSizeof, CsrBtBipsAaDeleteImageReqSer, CsrBtBipsAaDeleteImageReqDes, CsrBtBipsAaDeleteImageReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM
    { CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM, CsrBtBipsAaGetCapabilitiesCfmSizeof, CsrBtBipsAaGetCapabilitiesCfmSer, CsrBtBipsAaGetCapabilitiesCfmDes, CsrBtBipsAaGetCapabilitiesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND
    { CSR_BT_BIPS_AA_GET_CAPABILITIES_IND, CsrBtBipsAaGetCapabilitiesIndSizeof, CsrBtBipsAaGetCapabilitiesIndSer, CsrBtBipsAaGetCapabilitiesIndDes, CsrBtBipsAaGetCapabilitiesIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ
    { CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ, CsrBtBipsAaGetCapabilitiesReqSizeof, CsrBtBipsAaGetCapabilitiesReqSer, CsrBtBipsAaGetCapabilitiesReqDes, CsrBtBipsAaGetCapabilitiesReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES
    { CSR_BT_BIPS_AA_GET_CAPABILITIES_RES, CsrBtBipsAaGetCapabilitiesResSizeof, CsrBtBipsAaGetCapabilitiesResSer, CsrBtBipsAaGetCapabilitiesResDes, CsrBtBipsAaGetCapabilitiesResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM
    { CSR_BT_BIPS_AA_GET_IMAGE_CFM, CsrBtBipsAaGetImageCfmSizeof, CsrBtBipsAaGetImageCfmSer, CsrBtBipsAaGetImageCfmDes, CsrBtBipsAaGetImageCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND
    { CSR_BT_BIPS_AA_GET_IMAGE_IND, CsrBtBipsAaGetImageIndSizeof, CsrBtBipsAaGetImageIndSer, CsrBtBipsAaGetImageIndDes, CsrBtBipsAaGetImageIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM
    { CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM, CsrBtBipsAaGetImageListCfmSizeof, CsrBtBipsAaGetImageListCfmSer, CsrBtBipsAaGetImageListCfmDes, CsrBtBipsAaGetImageListCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND
    { CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND, CsrBtBipsAaGetImageListHeaderIndSizeof, CsrBtBipsAaGetImageListHeaderIndSer, CsrBtBipsAaGetImageListHeaderIndDes, CsrBtBipsAaGetImageListHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES
    { CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES, CsrBtBipsAaGetImageListHeaderResSizeof, CsrBtBipsAaGetImageListHeaderResSer, CsrBtBipsAaGetImageListHeaderResDes, CsrBtBipsAaGetImageListHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND
    { CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND, CsrBtBipsAaGetImageListIndSizeof, CsrBtBipsAaGetImageListIndSer, CsrBtBipsAaGetImageListIndDes, CsrBtBipsAaGetImageListIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ
    { CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ, CsrBtBipsAaGetImageListReqSizeof, CsrBtBipsAaGetImageListReqSer, CsrBtBipsAaGetImageListReqDes, CsrBtBipsAaGetImageListReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES
    { CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES, CsrBtBipsAaGetImageListResSizeof, CsrBtBipsAaGetImageListResSer, CsrBtBipsAaGetImageListResDes, CsrBtBipsAaGetImageListResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM
    { CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM, CsrBtBipsAaGetImagePropertiesCfmSizeof, CsrBtBipsAaGetImagePropertiesCfmSer, CsrBtBipsAaGetImagePropertiesCfmDes, CsrBtBipsAaGetImagePropertiesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND
    { CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND, CsrBtBipsAaGetImagePropertiesIndSizeof, CsrBtBipsAaGetImagePropertiesIndSer, CsrBtBipsAaGetImagePropertiesIndDes, CsrBtBipsAaGetImagePropertiesIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ
    { CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ, CsrBtBipsAaGetImagePropertiesReqSizeof, CsrBtBipsAaGetImagePropertiesReqSer, CsrBtBipsAaGetImagePropertiesReqDes, CsrBtBipsAaGetImagePropertiesReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES
    { CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES, CsrBtBipsAaGetImagePropertiesResSizeof, CsrBtBipsAaGetImagePropertiesResSer, CsrBtBipsAaGetImagePropertiesResDes, CsrBtBipsAaGetImagePropertiesResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ
    { CSR_BT_BIPS_AA_GET_IMAGE_REQ, CsrBtBipsAaGetImageReqSizeof, CsrBtBipsAaGetImageReqSer, CsrBtBipsAaGetImageReqDes, CsrBtBipsAaGetImageReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES
    { CSR_BT_BIPS_AA_GET_IMAGE_RES, CsrBtBipsAaGetImageResSizeof, CsrBtBipsAaGetImageResSer, CsrBtBipsAaGetImageResDes, CsrBtBipsAaGetImageResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM
    { CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM, CsrBtBipsAaGetLinkedAttachmentCfmSizeof, CsrBtBipsAaGetLinkedAttachmentCfmSer, CsrBtBipsAaGetLinkedAttachmentCfmDes, CsrBtBipsAaGetLinkedAttachmentCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND
    { CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND, CsrBtBipsAaGetLinkedAttachmentIndSizeof, CsrBtBipsAaGetLinkedAttachmentIndSer, CsrBtBipsAaGetLinkedAttachmentIndDes, CsrBtBipsAaGetLinkedAttachmentIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ
    { CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ, CsrBtBipsAaGetLinkedAttachmentReqSizeof, CsrBtBipsAaGetLinkedAttachmentReqSer, CsrBtBipsAaGetLinkedAttachmentReqDes, CsrBtBipsAaGetLinkedAttachmentReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES
    { CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES, CsrBtBipsAaGetLinkedAttachmentResSizeof, CsrBtBipsAaGetLinkedAttachmentResSer, CsrBtBipsAaGetLinkedAttachmentResDes, CsrBtBipsAaGetLinkedAttachmentResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM
    { CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM, CsrBtBipsAaGetLinkedThumbnailCfmSizeof, CsrBtBipsAaGetLinkedThumbnailCfmSer, CsrBtBipsAaGetLinkedThumbnailCfmDes, CsrBtBipsAaGetLinkedThumbnailCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND
    { CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND, CsrBtBipsAaGetLinkedThumbnailIndSizeof, CsrBtBipsAaGetLinkedThumbnailIndSer, CsrBtBipsAaGetLinkedThumbnailIndDes, CsrBtBipsAaGetLinkedThumbnailIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ
    { CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ, CsrBtBipsAaGetLinkedThumbnailReqSizeof, CsrBtBipsAaGetLinkedThumbnailReqSer, CsrBtBipsAaGetLinkedThumbnailReqDes, CsrBtBipsAaGetLinkedThumbnailReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES
    { CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES, CsrBtBipsAaGetLinkedThumbnailResSizeof, CsrBtBipsAaGetLinkedThumbnailResSer, CsrBtBipsAaGetLinkedThumbnailResDes, CsrBtBipsAaGetLinkedThumbnailResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_ABORT_IND
    { CSR_BT_BIPS_ABORT_IND, CsrBtBipsAbortIndSizeof, CsrBtBipsAbortIndSer, CsrBtBipsAbortIndDes, CsrBtBipsAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ
    { CSR_BT_BIPS_ACTIVATE_REQ, CsrBtBipsActivateReqSizeof, CsrBtBipsActivateReqSer, CsrBtBipsActivateReqDes, CsrBtBipsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND
    { CSR_BT_BIPS_AUTHENTICATE_IND, CsrBtBipsAuthenticateIndSizeof, CsrBtBipsAuthenticateIndSer, CsrBtBipsAuthenticateIndDes, CsrBtBipsAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES
    { CSR_BT_BIPS_AUTHENTICATE_RES, CsrBtBipsAuthenticateResSizeof, CsrBtBipsAuthenticateResSer, CsrBtBipsAuthenticateResDes, CsrBtBipsAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND
    { CSR_BT_BIPS_CHALLENGE_IND, CsrBtBipsChallengeIndSizeof, CsrBtBipsChallengeIndSer, CsrBtBipsChallengeIndDes, CsrBtBipsChallengeIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES
    { CSR_BT_BIPS_CHALLENGE_RES, CsrBtBipsChallengeResSizeof, CsrBtBipsChallengeResSer, CsrBtBipsChallengeResDes, CsrBtBipsChallengeResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_IND
    { CSR_BT_BIPS_CONNECT_IND, CsrBtBipsConnectIndSizeof, CsrBtBipsConnectIndSer, CsrBtBipsConnectIndDes, CsrBtBipsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_RES
    { CSR_BT_BIPS_CONNECT_RES, CsrBtBipsConnectResSizeof, CsrBtBipsConnectResSer, CsrBtBipsConnectResDes, CsrBtBipsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND
    { CSR_BT_BIPS_DEACTIVATE_IND, CsrBtBipsDeactivateIndSizeof, CsrBtBipsDeactivateIndSer, CsrBtBipsDeactivateIndDes, CsrBtBipsDeactivateIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ
    { CSR_BT_BIPS_DEACTIVATE_REQ, CsrBtBipsDeactivateReqSizeof, CsrBtBipsDeactivateReqSer, CsrBtBipsDeactivateReqDes, CsrBtBipsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND
    { CSR_BT_BIPS_DISCONNECT_IND, CsrBtBipsDisconnectIndSizeof, CsrBtBipsDisconnectIndSer, CsrBtBipsDisconnectIndDes, CsrBtBipsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ
    { CSR_BT_BIPS_DISCONNECT_REQ, CsrBtBipsDisconnectReqSizeof, CsrBtBipsDisconnectReqSer, CsrBtBipsDisconnectReqDes, CsrBtBipsDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM
    { CSR_BT_BIPS_GET_INSTANCES_QID_CFM, CsrBtBipsGetInstancesQidCfmSizeof, CsrBtBipsGetInstancesQidCfmSer, CsrBtBipsGetInstancesQidCfmDes, CsrBtBipsGetInstancesQidCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ
    { CSR_BT_BIPS_GET_INSTANCES_QID_REQ, CsrBtBipsGetInstancesQidReqSizeof, CsrBtBipsGetInstancesQidReqSer, CsrBtBipsGetInstancesQidReqDes, CsrBtBipsGetInstancesQidReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND
    { CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND, CsrBtBipsPushGetCapabilitiesHeaderIndSizeof, CsrBtBipsPushGetCapabilitiesHeaderIndSer, CsrBtBipsPushGetCapabilitiesHeaderIndDes, CsrBtBipsPushGetCapabilitiesHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES
    { CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES, CsrBtBipsPushGetCapabilitiesHeaderResSizeof, CsrBtBipsPushGetCapabilitiesHeaderResSer, CsrBtBipsPushGetCapabilitiesHeaderResDes, CsrBtBipsPushGetCapabilitiesHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND
    { CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND, CsrBtBipsPushGetCapabilitiesObjectIndSizeof, CsrBtBipsPushGetCapabilitiesObjectIndSer, CsrBtBipsPushGetCapabilitiesObjectIndDes, CsrBtBipsPushGetCapabilitiesObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES
    { CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES, CsrBtBipsPushGetCapabilitiesObjectResSizeof, CsrBtBipsPushGetCapabilitiesObjectResSer, CsrBtBipsPushGetCapabilitiesObjectResDes, CsrBtBipsPushGetCapabilitiesObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND
    { CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND, CsrBtBipsPushPutImageFileIndSizeof, CsrBtBipsPushPutImageFileIndSer, CsrBtBipsPushPutImageFileIndDes, CsrBtBipsPushPutImageFileIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES
    { CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES, CsrBtBipsPushPutImageFileResSizeof, CsrBtBipsPushPutImageFileResSer, CsrBtBipsPushPutImageFileResDes, CsrBtBipsPushPutImageFileResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND
    { CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND, CsrBtBipsPushPutImageHeaderIndSizeof, CsrBtBipsPushPutImageHeaderIndSer, CsrBtBipsPushPutImageHeaderIndDes, CsrBtBipsPushPutImageHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES
    { CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES, CsrBtBipsPushPutImageHeaderResSizeof, CsrBtBipsPushPutImageHeaderResSer, CsrBtBipsPushPutImageHeaderResDes, CsrBtBipsPushPutImageHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND
    { CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND, CsrBtBipsPushPutLinkedAttachmentFileIndSizeof, CsrBtBipsPushPutLinkedAttachmentFileIndSer, CsrBtBipsPushPutLinkedAttachmentFileIndDes, CsrBtBipsPushPutLinkedAttachmentFileIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES
    { CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES, CsrBtBipsPushPutLinkedAttachmentFileResSizeof, CsrBtBipsPushPutLinkedAttachmentFileResSer, CsrBtBipsPushPutLinkedAttachmentFileResDes, CsrBtBipsPushPutLinkedAttachmentFileResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND
    { CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND, CsrBtBipsPushPutLinkedAttachmentHeaderIndSizeof, CsrBtBipsPushPutLinkedAttachmentHeaderIndSer, CsrBtBipsPushPutLinkedAttachmentHeaderIndDes, CsrBtBipsPushPutLinkedAttachmentHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES
    { CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES, CsrBtBipsPushPutLinkedAttachmentHeaderResSizeof, CsrBtBipsPushPutLinkedAttachmentHeaderResSer, CsrBtBipsPushPutLinkedAttachmentHeaderResDes, CsrBtBipsPushPutLinkedAttachmentHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND
    { CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND, CsrBtBipsPushPutLinkedThumbnailFileIndSizeof, CsrBtBipsPushPutLinkedThumbnailFileIndSer, CsrBtBipsPushPutLinkedThumbnailFileIndDes, CsrBtBipsPushPutLinkedThumbnailFileIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES
    { CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES, CsrBtBipsPushPutLinkedThumbnailFileResSizeof, CsrBtBipsPushPutLinkedThumbnailFileResSer, CsrBtBipsPushPutLinkedThumbnailFileResDes, CsrBtBipsPushPutLinkedThumbnailFileResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND
    { CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND, CsrBtBipsPushPutLinkedThumbnailHeaderIndSizeof, CsrBtBipsPushPutLinkedThumbnailHeaderIndSer, CsrBtBipsPushPutLinkedThumbnailHeaderIndDes, CsrBtBipsPushPutLinkedThumbnailHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES
    { CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES, CsrBtBipsPushPutLinkedThumbnailHeaderResSizeof, CsrBtBipsPushPutLinkedThumbnailHeaderResSer, CsrBtBipsPushPutLinkedThumbnailHeaderResDes, CsrBtBipsPushPutLinkedThumbnailHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND
    { CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND, CsrBtBipsRcGetImageHeaderIndSizeof, CsrBtBipsRcGetImageHeaderIndSer, CsrBtBipsRcGetImageHeaderIndDes, CsrBtBipsRcGetImageHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES
    { CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES, CsrBtBipsRcGetImageHeaderResSizeof, CsrBtBipsRcGetImageHeaderResSer, CsrBtBipsRcGetImageHeaderResDes, CsrBtBipsRcGetImageHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND
    { CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND, CsrBtBipsRcGetImageObjectIndSizeof, CsrBtBipsRcGetImageObjectIndSer, CsrBtBipsRcGetImageObjectIndDes, CsrBtBipsRcGetImageObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES
    { CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES, CsrBtBipsRcGetImageObjectResSizeof, CsrBtBipsRcGetImageObjectResSer, CsrBtBipsRcGetImageObjectResDes, CsrBtBipsRcGetImageObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND
    { CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND, CsrBtBipsRcGetImagePropertiesHeaderIndSizeof, CsrBtBipsRcGetImagePropertiesHeaderIndSer, CsrBtBipsRcGetImagePropertiesHeaderIndDes, CsrBtBipsRcGetImagePropertiesHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES
    { CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES, CsrBtBipsRcGetImagePropertiesHeaderResSizeof, CsrBtBipsRcGetImagePropertiesHeaderResSer, CsrBtBipsRcGetImagePropertiesHeaderResDes, CsrBtBipsRcGetImagePropertiesHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND
    { CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND, CsrBtBipsRcGetImagePropertiesObjectIndSizeof, CsrBtBipsRcGetImagePropertiesObjectIndSer, CsrBtBipsRcGetImagePropertiesObjectIndDes, CsrBtBipsRcGetImagePropertiesObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES
    { CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES, CsrBtBipsRcGetImagePropertiesObjectResSizeof, CsrBtBipsRcGetImagePropertiesObjectResSer, CsrBtBipsRcGetImagePropertiesObjectResDes, CsrBtBipsRcGetImagePropertiesObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND
    { CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND, CsrBtBipsRcGetLinkedThumbnailHeaderIndSizeof, CsrBtBipsRcGetLinkedThumbnailHeaderIndSer, CsrBtBipsRcGetLinkedThumbnailHeaderIndDes, CsrBtBipsRcGetLinkedThumbnailHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES
    { CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES, CsrBtBipsRcGetLinkedThumbnailHeaderResSizeof, CsrBtBipsRcGetLinkedThumbnailHeaderResSer, CsrBtBipsRcGetLinkedThumbnailHeaderResDes, CsrBtBipsRcGetLinkedThumbnailHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND
    { CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND, CsrBtBipsRcGetLinkedThumbnailObjectIndSizeof, CsrBtBipsRcGetLinkedThumbnailObjectIndSer, CsrBtBipsRcGetLinkedThumbnailObjectIndDes, CsrBtBipsRcGetLinkedThumbnailObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES
    { CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES, CsrBtBipsRcGetLinkedThumbnailObjectResSizeof, CsrBtBipsRcGetLinkedThumbnailObjectResSer, CsrBtBipsRcGetLinkedThumbnailObjectResDes, CsrBtBipsRcGetLinkedThumbnailObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND
    { CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND, CsrBtBipsRcGetMonitoringImageHeaderIndSizeof, CsrBtBipsRcGetMonitoringImageHeaderIndSer, CsrBtBipsRcGetMonitoringImageHeaderIndDes, CsrBtBipsRcGetMonitoringImageHeaderIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES
    { CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES, CsrBtBipsRcGetMonitoringImageHeaderResSizeof, CsrBtBipsRcGetMonitoringImageHeaderResSer, CsrBtBipsRcGetMonitoringImageHeaderResDes, CsrBtBipsRcGetMonitoringImageHeaderResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND
    { CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND, CsrBtBipsRcGetMonitoringImageObjectIndSizeof, CsrBtBipsRcGetMonitoringImageObjectIndSer, CsrBtBipsRcGetMonitoringImageObjectIndDes, CsrBtBipsRcGetMonitoringImageObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES
    { CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES, CsrBtBipsRcGetMonitoringImageObjectResSizeof, CsrBtBipsRcGetMonitoringImageObjectResSer, CsrBtBipsRcGetMonitoringImageObjectResDes, CsrBtBipsRcGetMonitoringImageObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ
    { CSR_BT_BIPS_REGISTER_QID_REQ, CsrBtBipsRegisterQidReqSizeof, CsrBtBipsRegisterQidReqSer, CsrBtBipsRegisterQidReqDes, CsrBtBipsRegisterQidReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM
    { CSR_BT_BIPS_SECURITY_IN_CFM, CsrBtBipsSecurityInCfmSizeof, CsrBtBipsSecurityInCfmSer, CsrBtBipsSecurityInCfmDes, CsrBtBipsSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ
    { CSR_BT_BIPS_SECURITY_IN_REQ, CsrBtBipsSecurityInReqSizeof, CsrBtBipsSecurityInReqSer, CsrBtBipsSecurityInReqDes, CsrBtBipsSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtBipsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_BIPS_PRIM, csr_bt_bips_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_bips_conv_info = {
    CSR_BT_BIPS_PRIM,
    "CSR_BT_BIPS_PRIM",
    csr_bt_bips_conv_lut
};

CsrLogPrimitiveInformation* CsrBtBipsTechInfoGet(void)
{
    return &csr_bt_bips_conv_info;
}
#endif
#endif

