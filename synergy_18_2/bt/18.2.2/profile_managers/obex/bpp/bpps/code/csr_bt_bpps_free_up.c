/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_BPPS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_bpps_lib.h"
#include "csr_bt_bpps_prim.h"

void CsrBtBppsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BPPS_PRIM)
    {
        CsrBtBppsPrim *prim = (CsrBtBppsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND
            case CSR_BT_BPPS_GET_JOB_ATTRIBS_IND:
            {
                CsrBtBppsGetJobAttribsInd *p = message;
                CsrPmemFree(p->jobAttribs);
                p->jobAttribs = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND
            case CSR_BT_BPPS_SEND_DOCUMENT_IND:
            {
                CsrBtBppsSendDocumentInd *p = message;
                CsrPmemFree(p->documentType);
                p->documentType = NULL;
                CsrPmemFree(p->fileName);
                p->fileName = NULL;
                CsrPmemFree(p->documentBody);
                p->documentBody = NULL;
                CsrPmemFree(p->docTypeDependentInfo);
                p->docTypeDependentInfo = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_IND
            case CSR_BT_BPPS_NEXT_IND:
            {
                CsrBtBppsNextInd *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM
            case CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM:
            {
                CsrBtBppsGetReferencedObjCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND
            case CSR_BT_BPPS_CREATE_JOB_IND:
            {
                CsrBtBppsCreateJobInd *p = message;
                CsrPmemFree(p->jobName);
                p->jobName = NULL;
                CsrPmemFree(p->jobOriginatingUserName);
                p->jobOriginatingUserName = NULL;
                CsrPmemFree(p->documentFormat);
                p->documentFormat = NULL;
                CsrPmemFree(p->copies);
                p->copies = NULL;
                CsrPmemFree(p->sides);
                p->sides = NULL;
                CsrPmemFree(p->numberUp);
                p->numberUp = NULL;
                CsrPmemFree(p->orientationRequested);
                p->orientationRequested = NULL;
                CsrPmemFree(p->mediaSize);
                p->mediaSize = NULL;
                CsrPmemFree(p->mediaType);
                p->mediaType = NULL;
                CsrPmemFree(p->printQuality);
                p->printQuality = NULL;
                CsrPmemFree(p->cancelOnLostLink);
                p->cancelOnLostLink = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND
            case CSR_BT_BPPS_GET_REFERENCED_OBJ_IND:
            {
                CsrBtBppsGetReferencedObjInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND
            case CSR_BT_BPPS_AUTHENTICATE_IND:
            {
                CsrBtBppsAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND */
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
#endif /* EXCLUDE_CSR_BT_BPPS_MODULE */
