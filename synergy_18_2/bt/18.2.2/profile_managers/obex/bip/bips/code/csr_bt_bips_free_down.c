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

void CsrBtBipsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BIPS_PRIM)
    {
        CsrBtBipsPrim *prim = (CsrBtBipsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES
            case CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES:
            {
                CsrBtBipsRcGetMonitoringImageObjectRes *p = message;
                CsrPmemFree(p->monitoringObject);
                p->monitoringObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ
            case CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ:
            {
                CsrBtBipsAaGetLinkedAttachmentReq *p = message;
                CsrPmemFree(p->fileName);
                p->fileName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES
            case CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES:
            {
                CsrBtBipsRcGetImagePropertiesObjectRes *p = message;
                CsrPmemFree(p->propertiesObject);
                p->propertiesObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES
            case CSR_BT_BIPS_CHALLENGE_RES:
            {
                CsrBtBipsChallengeRes *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES
            case CSR_BT_BIPS_AUTHENTICATE_RES:
            {
                CsrBtBipsAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ
            case CSR_BT_BIPS_AA_GET_IMAGE_REQ:
            {
                CsrBtBipsAaGetImageReq *p = message;
                CsrPmemFree(p->imageDescriptor);
                p->imageDescriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES
            case CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES:
            {
                CsrBtBipsRcGetImageObjectRes *p = message;
                CsrPmemFree(p->imageObject);
                p->imageObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ
            case CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ:
            {
                CsrBtBipsAaGetImageListReq *p = message;
                CsrPmemFree(p->imageHandlesDescriptor);
                p->imageHandlesDescriptor = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ */
#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES
            case CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES:
            {
                CsrBtBipsRcGetLinkedThumbnailObjectRes *p = message;
                CsrPmemFree(p->thumbnailObject);
                p->thumbnailObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES
            case CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES:
            {
                CsrBtBipsPushGetCapabilitiesObjectRes *p = message;
                CsrPmemFree(p->capabilitiesObject);
                p->capabilitiesObject = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES */
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
