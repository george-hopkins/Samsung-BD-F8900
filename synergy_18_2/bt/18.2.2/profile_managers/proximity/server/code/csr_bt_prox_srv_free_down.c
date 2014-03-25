/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_prox_srv_lib.h"
#include "csr_bt_prox_srv_prim.h"

void CsrBtProxSrvFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_PROX_SRV_PRIM)
    {
        CsrBtProxSrvPrim *prim = (CsrBtProxSrvPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ
            case CSR_BT_PROX_SRV_ACTIVATE_REQ:
            {
                CsrBtProxSrvActivateReq *p = message;
                CsrPmemFree(p->clientConfig);
                p->clientConfig = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ */
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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_MODULE */
