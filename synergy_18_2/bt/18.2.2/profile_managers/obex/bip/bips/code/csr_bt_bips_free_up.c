/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_BIPS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_bips_lib.h"
#include "csr_bt_bips_prim.h"

void CsrBtBipsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BIPS_PRIM)
    {
        CsrBtBipsPrim *prim = (CsrBtBipsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM
            case CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM:
            {
                CsrBtBipsAaGetLinkedThumbnailCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM
            case CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM:
            {
                CsrBtBipsAaGetImagePropertiesCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND
            case CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND:
            {
                CsrBtBipsPushPutImageHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND
            case CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND:
            {
                CsrBtBipsAaGetLinkedAttachmentInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM
            case CSR_BT_BIPS_GET_INSTANCES_QID_CFM:
            {
                CsrBtBipsGetInstancesQidCfm *p = message;
                CsrPmemFree(p->phandlesList);
                p->phandlesList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND
            case CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND:
            {
                CsrBtBipsRcGetImagePropertiesHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND
            case CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND:
            {
                CsrBtBipsPushPutLinkedThumbnailHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND
            case CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND:
            {
                CsrBtBipsAaGetImageListHeaderInd *p = message;
                CsrPmemFree(p->imageHandlesDescriptor);
                p->imageHandlesDescriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND
            case CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND:
            {
                CsrBtBipsRcGetImageHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM
            case CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM:
            {
                CsrBtBipsAaGetLinkedAttachmentCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM
            case CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM:
            {
                CsrBtBipsAaGetCapabilitiesCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND
            case CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND:
            {
                CsrBtBipsAaGetImageListInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND
            case CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND:
            {
                CsrBtBipsPushPutLinkedThumbnailFileInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM
            case CSR_BT_BIPS_AA_GET_IMAGE_CFM:
            {
                CsrBtBipsAaGetImageCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND
            case CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND:
            {
                CsrBtBipsPushPutLinkedAttachmentHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM
            case CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM:
            {
                CsrBtBipsAaGetImageListCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND
            case CSR_BT_BIPS_AA_GET_IMAGE_IND:
            {
                CsrBtBipsAaGetImageInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND
            case CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND:
            {
                CsrBtBipsRcGetLinkedThumbnailHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND
            case CSR_BT_BIPS_AA_GET_CAPABILITIES_IND:
            {
                CsrBtBipsAaGetCapabilitiesInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND
            case CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND:
            {
                CsrBtBipsPushPutLinkedAttachmentFileInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND
            case CSR_BT_BIPS_AUTHENTICATE_IND:
            {
                CsrBtBipsAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND
            case CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND:
            {
                CsrBtBipsPushPutImageFileInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND
            case CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND:
            {
                CsrBtBipsAaGetLinkedThumbnailInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND
            case CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND:
            {
                CsrBtBipsAaGetImagePropertiesInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */
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
#endif /* EXCLUDE_CSR_BT_BIPS_MODULE */
