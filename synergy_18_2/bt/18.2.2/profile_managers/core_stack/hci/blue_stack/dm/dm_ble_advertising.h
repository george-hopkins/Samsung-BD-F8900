#ifndef _DM_BLE_ADVERTISING_H_
#define _DM_BLE_ADVERTISING_H_

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_ble_advertising.c

\brief  Look after BLE advertising

*/
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"

#ifdef INSTALL_ULP

#ifdef __cplusplus
extern "C" {
#endif

extern void dm_ble_advertising_local_address_changed(CsrUint8 address_type);
extern void dm_ble_advertising_set_parameters_req(const DM_UPRIM_T *const prim);
extern void dm_ble_advertising_set_enable_req(const DM_UPRIM_T *const prim);
extern void dm_ble_advertising_slave_connect(const TYPED_BD_ADDR_T *peer_addrt, CsrUint8 advertising_filter_policy);
extern void dm_ble_advertising_apply_parameters(CsrUint8 sender);
#define dm_ble_advertising_restore() \
    (dm_ble_advertising_apply_parameters(DM_HCI_SENDER_BLUESTACK))

#else
#define dm_ble_advertising_set_parameters_req NULL
#define dm_ble_advertising_set_enable_req NULL

#endif /* INSTALL_ULP */

#ifdef __cplusplus
}
#endif

#endif /* _DM_BLE_ADVERTISING_H_ */
