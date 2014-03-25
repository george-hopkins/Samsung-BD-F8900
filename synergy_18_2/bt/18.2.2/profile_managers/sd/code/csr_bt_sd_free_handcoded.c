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
#include "csr_bt_sd_free_handcoded.h"
#ifndef EXCLUDE_CSR_BT_SD_MODULE
#include "csr_bt_sd_prim.h"

void CsrBtSdFreeHandcoded(void *message)
{
    CsrBtSdPrim *prim = (CsrBtSdPrim *) message;
    switch(*prim)
    {
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_CFM
        case CSR_BT_SD_READ_DEVICE_LIST_CFM:
        {
            CsrBtSdReadDeviceListCfmPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_CFM */
#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_IND
        case CSR_BT_SD_READ_DEVICE_LIST_IND:
        {
            CsrBtSdReadDeviceListIndPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_IND */
        default:
        {
            break;
        }
    }
}

#endif

