/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_HCRP_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_hcrp_lib.h"
#include "csr_bt_hcrp_prim.h"

void CsrBtHcrpFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HCRP_PRIM)
    {
        CsrBtHcrpPrim *prim = (CsrBtHcrpPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE_1284_RES
            case CSR_BT_HCRP_GET_IEEE_1284_RES:
            {
                CsrBtHcrpGetIeee1284Res *p = message;
                CsrPmemFree(p->ieee1284);
                p->ieee1284 = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE_1284_RES */
#ifndef EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ
            case CSR_BT_HCRP_ACTIVATE_REQ:
            {
                CsrBtHcrpActivateReq *p = message;
                CsrPmemFree(p->ieee1284Id);
                p->ieee1284Id = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ */
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
#endif /* EXCLUDE_CSR_BT_HCRP_MODULE */
