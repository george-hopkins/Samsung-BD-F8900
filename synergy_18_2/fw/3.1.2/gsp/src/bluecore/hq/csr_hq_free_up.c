/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_HQ_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_hq_lib.h"
#include "csr_hq_prim.h"

void CsrHqFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_HQ_PRIM)
    {
        CsrHqPrimType *prim = (CsrHqPrimType *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_HQ_MSG_IND
            case CSR_HQ_MSG_IND:
            {
                CsrHqMsgInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_HQ_MSG_IND */
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
#endif /* EXCLUDE_CSR_HQ_MODULE */
