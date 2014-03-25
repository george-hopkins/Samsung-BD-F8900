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

void CsrBtBipcFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BIPC_PRIM)
    {
        CsrBtBipcPrim *prim = (CsrBtBipcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND
            case CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND:
            {
                CsrBtBipcRcGetImagePropertiesInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND
            case CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND:
            {
                CsrBtBipcAaGetImageHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM
            case CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM:
            {
                CsrBtBipcRcGetImagePropertiesCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM
            case CSR_BT_BIPC_RC_GET_IMAGE_CFM:
            {
                CsrBtBipcRcGetImageCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND
            case CSR_BT_BIPC_AUTHENTICATE_IND:
            {
                CsrBtBipcAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND
            case CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND:
            {
                CsrBtBipcAaGetLinkedAttachmentHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND
            case CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND:
            {
                CsrBtBipcRcGetMonitoringImageFileInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND
            case CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND:
            {
                CsrBtBipcRcGetLinkedThumbnailInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND
            case CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND:
            {
                CsrBtBipcAaGetLinkedThumbnailHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND
            case CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND:
            {
                CsrBtBipcAaGetImagePropertiesHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND
            case CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND:
            {
                CsrBtBipcPushGetCapabilitiesInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND
            case CSR_BT_BIPC_AA_DELETE_IMAGE_IND:
            {
                CsrBtBipcAaDeleteImageInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND
            case CSR_BT_BIPC_RC_GET_IMAGE_IND:
            {
                CsrBtBipcRcGetImageInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND
            case CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND:
            {
                CsrBtBipcAaGetImageListHeaderInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM
            case CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM:
            {
                CsrBtBipcPushGetCapabilitiesCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */
#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM
            case CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM:
            {
                CsrBtBipcRcGetLinkedThumbnailCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */
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
