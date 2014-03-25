/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_GATT_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_gatt_lib.h"
#include "csr_bt_gatt_prim.h"
#include "csr_bt_gatt_free_handcoded.h"

void CsrBtGattFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_GATT_PRIM)
    {
        CsrBtGattPrim *prim = (CsrBtGattPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES
            case CSR_BT_GATT_DB_ACCESS_RES:
            {
                CsrBtGattDbAccessRes *p = message;
                CsrPmemFree(p->value);
                p->value = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES */
#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ
            case CSR_BT_GATT_EVENT_SEND_REQ:
            {
                CsrBtGattEventSendReq *p = message;
                CsrPmemFree(p->value);
                p->value = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ
            case CSR_BT_GATT_WHITELIST_ADD_REQ:
            {
                CsrBtGattWhitelistAddReq *p = message;
                CsrPmemFree(p->address);
                p->address = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ
            case CSR_BT_GATT_ADVERTISE_REQ:
            {
                CsrBtGattAdvertiseReq *p = message;
                CsrPmemFree(p->advertisingData);
                p->advertisingData = NULL;
                CsrPmemFree(p->scanResponseData);
                p->scanResponseData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ
            case CSR_BT_GATT_READ_MULTI_REQ:
            {
                CsrBtGattReadMultiReq *p = message;
                CsrPmemFree(p->handles);
                p->handles = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ */
#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ
            case CSR_BT_GATT_PERIPHERAL_REQ:
            {
                CsrBtGattPeripheralReq *p = message;
                CsrPmemFree(p->advertisingData);
                p->advertisingData = NULL;
                CsrPmemFree(p->scanResponseData);
                p->scanResponseData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ */
            default:
            {
                CsrBtGattFreeHandcoded(prim);
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_MODULE */
