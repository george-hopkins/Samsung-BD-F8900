/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_PAS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_pas_lib.h"
#include "csr_bt_pas_prim.h"

void CsrBtPasFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_PAS_PRIM)
    {
        CsrBtPasPrim *prim = (CsrBtPasPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ
            case CSR_BT_PAS_AUTHENTICATE_REQ:
            {
                CsrBtPasAuthenticateReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES
            case CSR_BT_PAS_AUTHENTICATE_RES:
            {
                CsrBtPasAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_PAS_NEXT_RES
            case CSR_BT_PAS_NEXT_RES:
            {
                CsrBtPasNextRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAS_NEXT_RES */
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
#endif /* EXCLUDE_CSR_BT_PAS_MODULE */
