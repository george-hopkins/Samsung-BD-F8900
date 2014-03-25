/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_cm_free_handcoded.h"
#ifndef EXCLUDE_CSR_BT_CM_MODULE
#include "csr_bt_cm_prim.h"

void CsrBtCmFreeHandcoded(void *message)
{
    CsrBtCmPrim *prim = (CsrBtCmPrim *) message;
    switch(*prim)
    {
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ
        case CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ:
        {
            CsrBtCmBnepConnectAcceptReqPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_REQ
        case CSR_BT_CM_BNEP_CONNECT_REQ:
        {
            CsrBtCmBnepConnectReqPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_REQ */
        default:
        {
            break;
        }
    }
}

#endif

