/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_phdc_mgr_lib.h"
#include "csr_bt_phdc_mgr_prim.h"

void CsrBtPhdcMgrFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_PHDC_MGR_PRIM)
    {
        CsrBtPhdcMgrPrim *prim = (CsrBtPhdcMgrPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DS_IND
            case CSR_BT_PHDC_MGR_DS_IND:
            {
                CsrBtPhdcMgrDsInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DS_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND
            case CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND:
            {
                CsrBtPhdcMgrGetDeviceCapabInd *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->providerName);
                p->providerName = NULL;
                CsrPmemFree(p->supportedFeatureList);
                p->supportedFeatureList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND */
#ifndef EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND
            case CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND:
            {
                CsrBtPhdcMgrExtConfigObjInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND */
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
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_MODULE */
