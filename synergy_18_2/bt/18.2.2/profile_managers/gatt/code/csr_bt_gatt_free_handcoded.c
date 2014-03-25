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
#include "csr_bt_gatt_free_handcoded.h"
#ifndef EXCLUDE_CSR_BT_GATT_MODULE
#include "csr_bt_gatt_prim.h"

void CsrBtGattFreeHandcoded(void *message)
{
    CsrBtGattPrim *prim = (CsrBtGattPrim *) message;
    switch(*prim)
    {
#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_WRITE_IND
        case CSR_BT_GATT_DB_ACCESS_WRITE_IND:
        {
            CsrBtGattDbAccessWriteIndPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_WRITE_IND */
#ifndef EXCLUDE_CSR_BT_GATT_DB_ADD_REQ
        case CSR_BT_GATT_DB_ADD_REQ:
        {
            CsrBtGattDbAddReqPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_GATT_DB_ADD_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_SCAN_REQ
        case CSR_BT_GATT_SCAN_REQ:
        {
            CsrBtGattScanReqPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_GATT_SCAN_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_WRITE_REQ
        case CSR_BT_GATT_WRITE_REQ:
        {
            CsrBtGattWriteReqPrimFree(prim);
            break;
        }
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_REQ */
        default:
        {
            break;
        }
    }
}

#endif

