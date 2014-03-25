/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_AT_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_at_lib.h"
#include "csr_bt_at_prim.h"

void CsrBtAtFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_AT_PRIM)
    {
        CsrBtAtPrim *prim = (CsrBtAtPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_D_IND
            case CSR_BT_AT_DG_ATC_D_IND:
            {
                CsrBtAtDgAtcDInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_D_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_IND
            case CSR_BT_AT_DG_DATA_IND:
            {
                CsrBtAtDgDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_IND */
#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND
            case CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND:
            {
                CsrBtAtDgAtcUnknownExtendedCmdInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND */
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
#endif /* EXCLUDE_CSR_BT_AT_MODULE */
