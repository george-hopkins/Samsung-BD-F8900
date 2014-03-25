/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_phdc_ag_lib.h"
#include "csr_bt_phdc_ag_prim.h"

void CsrBtPhdcAgFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_PHDC_AG_PRIM)
    {
        CsrBtPhdcAgPrim *prim = (CsrBtPhdcAgPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND
            case CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND:
            {
                CsrBtPhdcAgGetDeviceCapabInd *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->providerName);
                p->providerName = NULL;
                CsrPmemFree(p->supportedFeatureList);
                p->supportedFeatureList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND */
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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_MODULE */
