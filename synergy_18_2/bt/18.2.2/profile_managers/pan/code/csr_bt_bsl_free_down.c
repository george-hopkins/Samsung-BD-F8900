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

void CsrBtBslFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_BSL_PRIM)
    {
        CsrBtBslPrim *prim = (CsrBtBslPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_BSL_DATA_REQ
            case CSR_BT_BSL_DATA_REQ:
            {
                CsrBtBslDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BSL_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ
            case CSR_BT_BSL_MULTICAST_DATA_REQ:
            {
                CsrBtBslMulticastDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ */
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
