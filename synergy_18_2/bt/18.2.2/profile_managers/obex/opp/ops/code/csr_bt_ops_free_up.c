/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_OPS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_ops_lib.h"
#include "csr_bt_ops_prim.h"

void CsrBtOpsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_OPS_PRIM)
    {
        CsrBtOpsPrim *prim = (CsrBtOpsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_OPS_ABORT_IND
            case CSR_BT_OPS_ABORT_IND:
            {
                CsrBtOpsAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_PUT_IND
            case CSR_BT_OPS_PUT_IND:
            {
                CsrBtOpsPutInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPS_PUT_IND */
#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND
            case CSR_BT_OPS_AUTHENTICATE_IND:
            {
                CsrBtOpsAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND
            case CSR_BT_OPS_PUT_NEXT_IND:
            {
                CsrBtOpsPutNextInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPS_PUT_NEXT_IND */
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
#endif /* EXCLUDE_CSR_BT_OPS_MODULE */
