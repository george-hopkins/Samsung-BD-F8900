/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_HIDD_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_hidd_lib.h"
#include "csr_bt_hidd_prim.h"

void CsrBtHiddFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HIDD_PRIM)
    {
        CsrBtHiddPrim *prim = (CsrBtHiddPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HIDD_DATA_IND
            case CSR_BT_HIDD_DATA_IND:
            {
                CsrBtHiddDataInd *p = message;
                CsrPmemFree(p->report);
                p->report = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_IND
            case CSR_BT_HIDD_CONTROL_IND:
            {
                CsrBtHiddControlInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_IND */
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
#endif /* EXCLUDE_CSR_BT_HIDD_MODULE */
