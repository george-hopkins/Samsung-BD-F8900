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

void CsrBtBppsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BPPS_PRIM)
    {
        CsrBtBppsPrim *prim = (CsrBtBppsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ
            case CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ:
            {
                CsrBtBppsGetReferencedObjReq *p = message;
                CsrPmemFree(p->objName);
                p->objName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ
            case CSR_BT_BPPS_AUTHENTICATE_REQ:
            {
                CsrBtBppsAuthenticateReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES
            case CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES:
            {
                CsrBtBppsGetPrinterAttribsRes *p = message;
                CsrPmemFree(p->printerName);
                p->printerName = NULL;
                CsrPmemFree(p->printerLocation);
                p->printerLocation = NULL;
                CsrPmemFree(p->printerState);
                p->printerState = NULL;
                CsrPmemFree(p->printerStateReasons);
                p->printerStateReasons = NULL;
                CsrPmemFree(p->documentFormatsSupported);
                p->documentFormatsSupported = NULL;
                CsrPmemFree(p->colorSupported);
                p->colorSupported = NULL;
                CsrPmemFree(p->maxCopiesSupported);
                p->maxCopiesSupported = NULL;
                CsrPmemFree(p->sidesSupported);
                p->sidesSupported = NULL;
                CsrPmemFree(p->numberUpSupported);
                p->numberUpSupported = NULL;
                CsrPmemFree(p->orientationsSupported);
                p->orientationsSupported = NULL;
                CsrPmemFree(p->mediaSizesSupported);
                p->mediaSizesSupported = NULL;
                CsrPmemFree(p->mediaTypesSupported);
                p->mediaTypesSupported = NULL;
                CsrPmemFree(p->mediaLoaded);
                p->mediaLoaded = NULL;
                CsrPmemFree(p->printQualitySupported);
                p->printQualitySupported = NULL;
                CsrPmemFree(p->queuedJobCount);
                p->queuedJobCount = NULL;
                CsrPmemFree(p->imageFormatsSupported);
                p->imageFormatsSupported = NULL;
                CsrPmemFree(p->basicTextPageWidth);
                p->basicTextPageWidth = NULL;
                CsrPmemFree(p->basicTextPageHeight);
                p->basicTextPageHeight = NULL;
                CsrPmemFree(p->printerGeneralCurrentOperator);
                p->printerGeneralCurrentOperator = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ
            case CSR_BT_BPPS_ACTIVATE_REQ:
            {
                CsrBtBppsActivateReq *p = message;
                CsrPmemFree(p->documentFormatsSupported);
                p->documentFormatsSupported = NULL;
                CsrPmemFree(p->characterRepertoiresSupported);
                p->characterRepertoiresSupported = NULL;
                CsrPmemFree(p->imageFormatsSupported);
                p->imageFormatsSupported = NULL;
                CsrPmemFree(p->ieee1284Id);
                p->ieee1284Id = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES
            case CSR_BT_BPPS_GET_JOB_ATTRIBS_RES:
            {
                CsrBtBppsGetJobAttribsRes *p = message;
                CsrPmemFree(p->jobState);
                p->jobState = NULL;
                CsrPmemFree(p->jobName);
                p->jobName = NULL;
                CsrPmemFree(p->jobOriginatingUserName);
                p->jobOriginatingUserName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES
            case CSR_BT_BPPS_GET_EVENT_RES:
            {
                CsrBtBppsGetEventRes *p = message;
                CsrPmemFree(p->jobState);
                p->jobState = NULL;
                CsrPmemFree(p->printerState);
                p->printerState = NULL;
                CsrPmemFree(p->printerStateReasons);
                p->printerStateReasons = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES
            case CSR_BT_BPPS_AUTHENTICATE_RES:
            {
                CsrBtBppsAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES */
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
