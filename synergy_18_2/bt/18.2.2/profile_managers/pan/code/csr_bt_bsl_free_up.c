/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_BSL_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_bsl_lib.h"
#include "csr_bt_bsl_prim.h"

void CsrBtBslFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BSL_PRIM)
    {
        CsrBtBslPrim *prim = (CsrBtBslPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND
            case CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND:
            {
                CsrBtBslServiceSearchResultInd *p = message;
                CsrPmemFree(p->searchResult.serviceName);
                p->searchResult.serviceName = NULL;
                CsrPmemFree(p->searchResult.serviceDescription);
                p->searchResult.serviceDescription = NULL;
                CsrPmemFree(p->searchResult.languageElements);
                p->searchResult.languageElements = NULL;
                CsrPmemFree(p->searchResult.ipv4Subnet);
                p->searchResult.ipv4Subnet = NULL;
                CsrPmemFree(p->searchResult.ipv6Subnet);
                p->searchResult.ipv6Subnet = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_BSL_DATA_IND
            case CSR_BT_BSL_DATA_IND:
            {
                CsrBtBslDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BSL_DATA_IND */
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
#endif /* EXCLUDE_CSR_BT_BSL_MODULE */
