/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BCCMD_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_bccmd_lib.h"
#include "csr_bccmd_prim.h"

void CsrBccmdFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BCCMD_PRIM)
    {
        CsrBccmdPrim *prim = (CsrBccmdPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BCCMD_REQ
            case CSR_BCCMD_REQ:
            {
                CsrBccmdReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BCCMD_REQ */
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
#endif /* EXCLUDE_CSR_BCCMD_MODULE */
