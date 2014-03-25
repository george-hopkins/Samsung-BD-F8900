/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_BPPC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_bppc_lib.h"
#include "csr_bt_bppc_prim.h"

void CsrBtBppcFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BPPC_PRIM)
    {
        CsrBtBppcPrim *prim = (CsrBtBppcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES
            case CSR_BT_BPPC_SEND_DOCUMENT_RES:
            {
                CsrBtBppcSendDocumentRes *p = message;
                CsrPmemFree(p->printContent);
                p->printContent = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ
            case CSR_BT_BPPC_SEND_DOCUMENT_REQ:
            {
                CsrBtBppcSendDocumentReq *p = message;
                CsrPmemFree(p->mimeMediaType);
                p->mimeMediaType = NULL;
                CsrPmemFree(p->ucs2documentName);
                p->ucs2documentName = NULL;
                CsrPmemFree(p->docTypeDependentInfo);
                p->docTypeDependentInfo = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ
            case CSR_BT_BPPC_CREATE_JOB_REQ:
            {
                CsrBtBppcCreateJobReq *p = message;
                CsrPmemFree(p->sides);
                p->sides = NULL;
                CsrPmemFree(p->orientation);
                p->orientation = NULL;
                CsrPmemFree(p->printQuality);
                p->printQuality = NULL;
                CsrPmemFree(p->jobName);
                p->jobName = NULL;
                CsrPmemFree(p->jobUserName);
                p->jobUserName = NULL;
                CsrPmemFree(p->documentFormat);
                p->documentFormat = NULL;
                CsrPmemFree(p->mediaSize);
                p->mediaSize = NULL;
                CsrPmemFree(p->mediaType);
                p->mediaType = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES
            case CSR_BT_BPPC_AUTHENTICATE_RES:
            {
                CsrBtBppcAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES
            case CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES:
            {
                CsrBtBppcGetReferenceObjectRes *p = message;
                CsrPmemFree(p->file);
                p->file = NULL;
                CsrPmemFree(p->fileName);
                p->fileName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES */
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
#endif /* EXCLUDE_CSR_BT_BPPC_MODULE */
