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

void CsrBtBppcFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BPPC_PRIM)
    {
        CsrBtBppcPrim *prim = (CsrBtBppcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM
            case CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM:
            {
                CsrBtBppcGetJobAttributesCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND
            case CSR_BT_BPPC_GET_EVENT_IND:
            {
                CsrBtBppcGetEventInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM
            case CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM:
            {
                CsrBtBppcGetPrinterAttributesCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND
            case CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND:
            {
                CsrBtBppcGetJobAttributesInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND
            case CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND:
            {
                CsrBtBppcGetReferenceObjectInd *p = message;
                CsrPmemFree(p->fileName);
                p->fileName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND
            case CSR_BT_BPPC_AUTHENTICATE_IND:
            {
                CsrBtBppcAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND
            case CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND:
            {
                CsrBtBppcGetPrinterAttributesInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_CFM
            case CSR_BT_BPPC_CONNECT_CFM:
            {
                CsrBtBppcConnectCfm *p = message;
                CsrPmemFree(p->xhtmlPrintImageFormats);
                p->xhtmlPrintImageFormats = NULL;
                CsrPmemFree(p->documentFormatsSupported);
                p->documentFormatsSupported = NULL;
                CsrPmemFree(p->mediaTypesSupported);
                p->mediaTypesSupported = NULL;
                CsrPmemFree(p->printerModelId);
                p->printerModelId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_CFM */
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
