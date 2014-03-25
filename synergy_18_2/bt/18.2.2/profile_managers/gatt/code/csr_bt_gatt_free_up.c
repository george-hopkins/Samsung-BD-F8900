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

void CsrBtGattFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_GATT_PRIM)
    {
        CsrBtGattPrim *prim = (CsrBtGattPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND
            case CSR_BT_GATT_READ_BY_UUID_IND:
            {
                CsrBtGattReadByUuidInd *p = message;
                CsrPmemFree(p->value);
                p->value = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND */
#ifndef EXCLUDE_CSR_BT_GATT_READ_CFM
            case CSR_BT_GATT_READ_CFM:
            {
                CsrBtGattReadCfm *p = message;
                CsrPmemFree(p->value);
                p->value = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_READ_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM
            case CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM:
            {
                CsrBtGattReadProfileDefinedDescriptorCfm *p = message;
                CsrPmemFree(p->value);
                p->value = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM
            case CSR_BT_GATT_WHITELIST_READ_CFM:
            {
                CsrBtGattWhitelistReadCfm *p = message;
                CsrPmemFree(p->address);
                p->address = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM
            case CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM:
            {
                CsrBtGattReadAggregateFormatCfm *p = message;
                CsrPmemFree(p->handles);
                p->handles = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM
            case CSR_BT_GATT_READ_USER_DESCRIPTION_CFM:
            {
                CsrBtGattReadUserDescriptionCfm *p = message;
                CsrPmemFree(p->usrDescription);
                p->usrDescription = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM
            case CSR_BT_GATT_READ_MULTI_CFM:
            {
                CsrBtGattReadMultiCfm *p = message;
                CsrPmemFree(p->value);
                p->value = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM */
#ifndef EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND
            case CSR_BT_GATT_NOTIFICATION_IND:
            {
                CsrBtGattNotificationInd *p = message;
                CsrPmemFree(p->value);
                p->value = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND */
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
