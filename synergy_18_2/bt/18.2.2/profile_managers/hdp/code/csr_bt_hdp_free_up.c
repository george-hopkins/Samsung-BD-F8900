/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_HDP_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_hdp_prim.h"

void CsrBtHdpFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HDP_PRIM)
    {
        CsrBtHdpPrim *prim = (CsrBtHdpPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HDP_DATA_IND
            case CSR_BT_HDP_DATA_IND:
            {
                CsrBtHdpDataInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HDP_DATA_IND */
#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND
            case CSR_BT_HDP_CTRL_GET_CAPAB_IND:
            {
                CsrBtHdpCtrlGetCapabInd *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->serviceDescrip);
                p->serviceDescrip = NULL;
                CsrPmemFree(p->providerName);
                p->providerName = NULL;
                CsrPmemFree(p->supportedFeatureList);
                p->supportedFeatureList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND */
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
#endif /* EXCLUDE_CSR_BT_HDP_MODULE */
