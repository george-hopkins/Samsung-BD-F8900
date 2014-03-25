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

void CsrBtHcrpFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HCRP_PRIM)
    {
        CsrBtHcrpPrim *prim = (CsrBtHcrpPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND
            case CSR_BT_HCRP_L2CA_DATA_IND:
            {
                CsrBtHcrpL2caDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND */
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
