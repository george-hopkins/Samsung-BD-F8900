/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_AMPM_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_ampm_lib.h"
#include "csr_bt_ampm_prim.h"

void CsrBtAmpmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_AMPM_PRIM)
    {
        CsrBtAmpmPrim *prim = (CsrBtAmpmPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ
            case CSR_BT_AMPM_PRF_MULTI_CONN_REQ:
            {
                CsrBtAmpmPrfMultiConnReq *p = message;
                CsrPmemFree(p->slaves);
                p->slaves = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ
            case CSR_BT_AMPM_PAL_HCI_EVENT_REQ:
            {
                CsrBtAmpmPalHciEventReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ
            case CSR_BT_AMPM_TEST_SDU_REQ:
            {
                CsrBtAmpmTestSduReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ
            case CSR_BT_AMPM_TEST_HCI_REQ:
            {
                CsrBtAmpmTestHciReq *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ */
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
#endif /* EXCLUDE_CSR_BT_AMPM_MODULE */
