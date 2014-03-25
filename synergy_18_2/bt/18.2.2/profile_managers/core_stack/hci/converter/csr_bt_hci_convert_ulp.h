#ifndef HCI_CONVERT_ULP_H_
#define HCI_CONVERT_ULP_H_

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2010
                All rights reserved

FILE:           csr_bt_hci_convert_ulp.h

DESCRIPTION:    Converter for BLE/ULP HCI commands/events

REVISION:       $Revision: #1 $

****************************************************************************/

#ifdef CSR_BT_LE_ENABLE

#include "csr_types.h"
#include "bluetooth.h"
#include "hci_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBool ConvertHciEvUlpMainUpstream(CsrUint8 *hciData, HCI_UPRIM_T *prim);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_LE_ENABLE */

#endif
