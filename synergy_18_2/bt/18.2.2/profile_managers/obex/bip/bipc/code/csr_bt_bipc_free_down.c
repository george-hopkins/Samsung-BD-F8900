/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_BIPC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_bipc_lib.h"
#include "csr_bt_bipc_prim.h"

void CsrBtBipcFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BIPC_PRIM)
    {
        CsrBtBipcPrim *prim = (CsrBtBipcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES
            case CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES:
            {
                CsrBtBipcAaGetImageObjectRes *p = message;
                CsrPmemFree(p->imageObject);
                p->imageObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ
            case CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ:
            {
                CsrBtBipcPushPutImageReq *p = message;
                CsrPmemFree(p->ucs2imageName);
                p->ucs2imageName = NULL;
                CsrPmemFree(p->imageDescriptor);
                p->imageDescriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES
            case CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES:
            {
                CsrBtBipcAaGetLinkedAttachmentObjectRes *p = message;
                CsrPmemFree(p->attachmentObject);
                p->attachmentObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_REQ
            case CSR_BT_BIPC_CONNECT_REQ:
            {
                CsrBtBipcConnectReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ
            case CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ:
            {
                CsrBtBipcPushPutAttachmentReq *p = message;
                CsrPmemFree(p->attachmentDescriptor);
                p->attachmentDescriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES
            case CSR_BT_BIPC_AUTHENTICATE_RES:
            {
                CsrBtBipcAuthenticateRes *p = message;
                CsrPmemFree(p->authPassword);
                p->authPassword = NULL;
                CsrPmemFree(p->authUserId);
                p->authUserId = NULL;
                CsrPmemFree(p->chalRealm);
                p->chalRealm = NULL;
                CsrPmemFree(p->chalPassword);
                p->chalPassword = NULL;
                CsrPmemFree(p->chalUserId);
                p->chalUserId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES
            case CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES:
            {
                CsrBtBipcPushPutAttachmentFileRes *p = message;
                CsrPmemFree(p->attachmentFile);
                p->attachmentFile = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ
            case CSR_BT_BIPC_RC_GET_IMAGE_REQ:
            {
                CsrBtBipcRcGetImageReq *p = message;
                CsrPmemFree(p->imageDescriptor);
                p->imageDescriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES
            case CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES:
            {
                CsrBtBipcAaGetLinkedThumbnailObjectRes *p = message;
                CsrPmemFree(p->thumbnailObject);
                p->thumbnailObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES
            case CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES:
            {
                CsrBtBipcAaGetImagePropertiesObjectRes *p = message;
                CsrPmemFree(p->propertiesObject);
                p->propertiesObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES
            case CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES:
            {
                CsrBtBipcAaGetCapabilitiesObjectRes *p = message;
                CsrPmemFree(p->capabilitiesObject);
                p->capabilitiesObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES
            case CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES:
            {
                CsrBtBipcAaGetImageListHeaderRes *p = message;
                CsrPmemFree(p->imageDescriptor);
                p->imageDescriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES
            case CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES:
            {
                CsrBtBipcAaGetImageListObjectRes *p = message;
                CsrPmemFree(p->imagesListObject);
                p->imagesListObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES
            case CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES:
            {
                CsrBtBipcPushPutImageFileRes *p = message;
                CsrPmemFree(p->imageFile);
                p->imageFile = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES
            case CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES:
            {
                CsrBtBipcPushPutThumbnailFileRes *p = message;
                CsrPmemFree(p->thumbnailFile);
                p->thumbnailFile = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */
            default:
            {
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_MODULE */
