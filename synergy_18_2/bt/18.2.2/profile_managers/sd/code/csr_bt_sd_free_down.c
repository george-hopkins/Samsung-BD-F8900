/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SD_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_free_handcoded.h"

void CsrBtSdFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SD_PRIM)
    {
        CsrBtSdPrim *prim = (CsrBtSdPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ
            case CSR_BT_SD_READ_SERVICES_REQ:
            {
                CsrBtSdReadServicesReq *p = message;
                CsrPmemFree(p->filter);
                p->filter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_SD_SEARCH_REQ
            case CSR_BT_SD_SEARCH_REQ:
            {
                CsrBtSdSearchReq *p = message;
                CsrPmemFree(p->filter);
                p->filter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ
            case CSR_BT_SD_READ_SERVICE_RECORD_REQ:
            {
                CsrBtSdReadServiceRecordReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ */
#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ
            case CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ:
            {
                CsrBtSdReadAvailableServicesReq *p = message;
                CsrPmemFree(p->filter);
                p->filter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */
#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ
            case CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ:
            {
                CsrBtSdRegisterServiceRecordReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */
            default:
            {
                CsrBtSdFreeHandcoded(prim);
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_BT_SD_MODULE */
