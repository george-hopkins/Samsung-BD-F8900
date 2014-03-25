/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SAPC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_sapc_lib.h"
#include "csr_bt_sapc_prim.h"

void CsrBtSapcFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SAPC_PRIM)
    {
        CsrBtSapcPrim *prim = (CsrBtSapcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM
            case CSR_BT_SAPC_TRANSFER_APDU_CFM:
            {
                CsrBtSapcTransferApduCfm *p = message;
                CsrPmemFree(p->responseApdu);
                p->responseApdu = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM
            case CSR_BT_SAPC_TRANSFER_ATR_CFM:
            {
                CsrBtSapcTransferAtrCfm *p = message;
                CsrPmemFree(p->atr);
                p->atr = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM */
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
#endif /* EXCLUDE_CSR_BT_SAPC_MODULE */
