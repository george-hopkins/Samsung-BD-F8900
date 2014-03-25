/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SD_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_free_handcoded.h"

void CsrBtSdFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SD_PRIM)
    {
        CsrBtSdPrim *prim = (CsrBtSdPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND
            case CSR_BT_SD_READ_SERVICE_RECORD_IND:
            {
                CsrBtSdReadServiceRecordInd *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND */
#ifndef EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND
            case CSR_BT_SD_SEARCH_RESULT_IND:
            {
                CsrBtSdSearchResultInd *p = message;
                CsrPmemFree(p->info);
                p->info = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND
            case CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND:
            {
                CsrBtSdImmediateSearchResultInd *p = message;
                CsrPmemFree(p->info);
                p->info = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND */
#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM
            case CSR_BT_SD_READ_SERVICES_CFM:
            {
                CsrBtSdReadServicesCfm *p = message;
                CsrPmemFree(p->list);
                p->list = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM
            case CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM:
            {
                CsrBtSdReadAvailableServicesCfm *p = message;
                CsrPmemFree(p->info);
                p->info = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM
            case CSR_BT_SD_READ_DEVICE_INFO_CFM:
            {
                CsrBtSdReadDeviceInfoCfm *p = message;
                CsrPmemFree(p->info);
                p->info = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM */
            default:
            {
                CsrBtSdFreeHandcoded(prim);
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_BT_SD_MODULE */
