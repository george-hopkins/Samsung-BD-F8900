/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SPP_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_spp_lib.h"
#include "csr_bt_spp_prim.h"

void CsrBtSppFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SPP_PRIM)
    {
        CsrBtSppPrim *prim = (CsrBtSppPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND
            case CSR_BT_SPP_SERVICE_NAME_IND:
            {
                CsrBtSppServiceNameInd *p = message;
                CsrPmemFree(p->serviceNameList);
                p->serviceNameList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SPP_SERVICE_NAME_IND */
#ifndef EXCLUDE_CSR_BT_SPP_DATA_IND
            case CSR_BT_SPP_DATA_IND:
            {
                CsrBtSppDataInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SPP_DATA_IND */
#ifndef EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM
            case CSR_BT_SPP_GET_INSTANCES_QID_CFM:
            {
                CsrBtSppGetInstancesQidCfm *p = message;
                CsrPmemFree(p->phandlesList);
                p->phandlesList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SPP_GET_INSTANCES_QID_CFM */
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
#endif /* EXCLUDE_CSR_BT_SPP_MODULE */
