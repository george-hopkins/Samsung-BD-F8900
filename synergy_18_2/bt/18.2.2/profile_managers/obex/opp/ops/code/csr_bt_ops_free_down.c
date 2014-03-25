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

void CsrBtOpsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_OPS_PRIM)
    {
        CsrBtOpsPrim *prim = (CsrBtOpsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_OPS_GET_NEXT_RES
            case CSR_BT_OPS_GET_NEXT_RES:
            {
                CsrBtOpsGetNextRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPS_GET_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES
            case CSR_BT_OPS_AUTHENTICATE_RES:
            {
                CsrBtOpsAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_OPS_GET_RES
            case CSR_BT_OPS_GET_RES:
            {
                CsrBtOpsGetRes *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_OPS_GET_RES */
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
