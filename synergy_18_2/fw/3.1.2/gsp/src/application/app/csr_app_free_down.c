/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_APP_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_app_lib.h"
#include "csr_app_prim.h"

void CsrAppFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_APP_PRIM)
    {
        CsrAppPrim *prim = (CsrAppPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_APP_BACKLOG_REQ
            case CSR_APP_BACKLOG_REQ:
            {
                CsrAppBacklogReq *p = message;
                CsrPmemFree(p->menuName);
                p->menuName = NULL;
                CsrPmemFree(p->subMenuName);
                p->subMenuName = NULL;
                CsrPmemFree(p->logText);
                p->logText = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_APP_BACKLOG_REQ */
#ifndef EXCLUDE_CSR_APP_REGISTER_REQ
            case CSR_APP_REGISTER_REQ:
            {
                CsrAppRegisterReq *p = message;
                CsrPmemFree(p->menuName);
                p->menuName = NULL;
                CsrPmemFree(p->subMenuName);
                p->subMenuName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_APP_REGISTER_REQ */
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
#endif /* EXCLUDE_CSR_APP_MODULE */
