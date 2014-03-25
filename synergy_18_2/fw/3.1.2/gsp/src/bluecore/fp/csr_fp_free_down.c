/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_FP_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_fp_lib.h"
#include "csr_fp_prim.h"

void CsrFpFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_FP_PRIM)
    {
        CsrFpPrimType *prim = (CsrFpPrimType *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_FP_WRITE_REQ
            case CSR_FP_WRITE_REQ:
            {
                CsrFpWriteReq *p = message;
                CsrMblkDestroy(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_FP_WRITE_REQ */
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
#endif /* EXCLUDE_CSR_FP_MODULE */
