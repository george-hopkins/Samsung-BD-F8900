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

void CsrBtAmpmFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_AMPM_PRIM)
    {
        CsrBtAmpmPrim *prim = (CsrBtAmpmPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM
            case CSR_BT_AMPM_CONTROLLER_CFM:
            {
                CsrBtAmpmControllerCfm *p = message;
                CsrPmemFree(p->amps);
                p->amps = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND
            case CSR_BT_AMPM_TEST_HCI_IND:
            {
                CsrBtAmpmTestHciInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND
            case CSR_BT_AMPM_PAL_HCI_COMMAND_IND:
            {
                CsrBtAmpmPalHciCommandInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND
            case CSR_BT_AMPM_TEST_RECEIVED_SDU_IND:
            {
                CsrBtAmpmTestReceivedSduInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND
            case CSR_BT_AMPM_CONTROLLER_IND:
            {
                CsrBtAmpmControllerInd *p = message;
                CsrPmemFree(p->amps);
                p->amps = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND */
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
