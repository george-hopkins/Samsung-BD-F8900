/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_prim.h"

void CsrBtScFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SC_PRIM)
    {
        CsrBtScPrim *prim = (CsrBtScPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND
            case CSR_BT_SC_SSP_JUSTWORKS_IND:
            {
                CsrBtScSspJustworksInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND */
#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_IND
            case CSR_BT_SC_PASSKEY_IND:
            {
                CsrBtScPasskeyInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_IND */
#ifndef EXCLUDE_CSR_BT_SC_DATABASE_CFM
            case CSR_BT_SC_DATABASE_CFM:
            {
                CsrBtScDatabaseCfm *p = message;
                CsrPmemFree(p->data);
                p->data = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_CFM */
#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND
            case CSR_BT_SC_SSP_COMPARE_IND:
            {
                CsrBtScSspCompareInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND */
#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM
            case CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM:
            {
                CsrBtScReadLocalOobDataCfm *p = message;
                CsrPmemFree(p->oobHashC);
                p->oobHashC = NULL;
                CsrPmemFree(p->oobRandR);
                p->oobRandR = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM
            case CSR_BT_SC_READ_DEVICE_RECORD_CFM:
            {
                CsrBtScReadDeviceRecordCfm *p = message;
                CsrPmemFree(p->deviceProperties);
                p->deviceProperties = NULL;
                CsrPmemFree(p->serviceChangedList);
                p->serviceChangedList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM */
#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND
            case CSR_BT_SC_SSP_PAIRING_IND:
            {
                CsrBtScSspPairingInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND
            case CSR_BT_SC_SSP_NOTIFICATION_IND:
            {
                CsrBtScSspNotificationInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_SC_REBOND_IND
            case CSR_BT_SC_REBOND_IND:
            {
                CsrBtScRebondInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_REBOND_IND */
#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND
            case CSR_BT_SC_READ_DEVICE_RECORD_IND:
            {
                CsrBtScReadDeviceRecordInd *p = message;
                CsrPmemFree(p->deviceProperties);
                p->deviceProperties = NULL;
                CsrPmemFree(p->serviceChangedList);
                p->serviceChangedList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND */
#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND
            case CSR_BT_SC_SSP_PASSKEY_IND:
            {
                CsrBtScSspPasskeyInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND */
#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND
            case CSR_BT_SC_AUTHORISE_EXT_IND:
            {
                CsrBtScAuthoriseExtInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND */
#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_IND
            case CSR_BT_SC_AUTHORISE_IND:
            {
                CsrBtScAuthoriseInd *p = message;
                CsrPmemFree(p->deviceName);
                p->deviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_IND */
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
#endif /* EXCLUDE_CSR_BT_SC_MODULE */
