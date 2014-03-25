/****************************************************************************

                (c) CSR plc 2010
                All rights reserved

FILE:           csr_bt_hci_convert_ulp.c

DESCRIPTION:    Converter for BLE/ULP HCI commands/events

REVISION:       $Revision: #1 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hci_convert_ulp.h"
#include "csr_bt_hci_convert_ulpdownstream.h"
#include "csr_bt_hci_convert_ulpupstream.h"
#include "csr_bt_hci_cm.h"

#ifdef CSR_BT_LE_ENABLE

/* Main specific ULP event decoder */
CsrBool ConvertHciEvUlpMainUpstream(CsrUint8 *hciData, HCI_UPRIM_T *prim)
{
    HCI_ULP_EVENT_COMMON_T *msg;
    msg = (HCI_ULP_EVENT_COMMON_T*)prim;

    /* Caller already progressed the hciData pointer to after the
     * generic HCI part so first octet is the sub_op_code */
    msg->ulp_sub_opcode = CSR_BT_CONVERT_8_FROM_STREAM(&hciData[0]);
    hciData = &hciData[1];

    switch(msg->ulp_sub_opcode)
    {
        case HCI_EV_ULP_CONNECTION_COMPLETE:
        {
            return (ConvertHciEvUlpConnectionComplete(hciData, prim));
        }
        case HCI_EV_ULP_ADVERTISING_REPORT:
        {
            return (ConvertHciEvUlpAdvertisingReport(hciData, prim));
        }
        case HCI_EV_ULP_CONNECTION_UPDATE_COMPLETE:
        {
            return (ConvertHciEvUlpConnectionUpdateComplete(hciData, prim));
        }
        case HCI_EV_ULP_READ_REMOTE_USED_FEATURES_COMPLETE:
        {
            return (ConvertHciEvUlpReadRemoteUsedFeaturesComplete(hciData, prim));
        }
        case HCI_EV_ULP_LONG_TERM_KEY_REQUESTED:
        {
            return (ConvertHciEvUlpLongTermKeyRequested(hciData, prim));
        }
        default:
        {
            return FALSE;
        }
    }
}

#endif
