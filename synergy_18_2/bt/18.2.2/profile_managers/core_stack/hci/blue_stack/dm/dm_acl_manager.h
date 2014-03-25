#ifndef _DM_ACL_MANAGER_H_
#define _DM_ACL_MANAGER_H_

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_acl_manager.h

\brief  This file defines the interface to the ACL Manager, which is part
        of the Bluestack Device Manager.
*/

#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_usr_config.h"
#include "dm_acl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef INSTALL_ULP
extern const DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T dm_ble_default_conn_params;
extern DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *p_dm_ble_conn_params;
#endif

/* Return type of dm_acl_open() */
typedef enum
{
    DM_ACL_SUCCESS,
    DM_ACL_PENDING,
    DM_ACL_FAILED
} DM_ACL_RESULT_T;

typedef enum
{
    DM_ACL_AUTH_RECONNECT,
    DM_ACL_AUTH_CONNECT,
    DM_ACL_CONNECT
} DM_ACL_CONNECT_T;

/* Application connection requests. */
void dm_acl_close(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client, hci_reason_t reason, const ACL_IDLE_TIMEOUT_MULTIPLIER_T idle_multiplier);
DM_ACL_RESULT_T dm_acl_open(const TYPED_BD_ADDR_T *addrt, const DM_SM_SERVICE_T *service, const DM_ACL_CLIENT_T client, CsrUint16 flags, DM_ACL_T **pp_acl);
void dm_acl_idle_timeout_change(DM_ACL_T *const p_acl, const ACL_IDLE_TIMEOUT_MULTIPLIER_T idle_multiplier, CsrUint16 reason);
void dm_acl_idle_timeout_extend_short(DM_ACL_T *const p_acl);


/* LE specific setting API for Host */
#ifdef INSTALL_ULP
void dm_ble_set_connection_parameters_req(const DM_UPRIM_T * const uprim);
void dm_ble_update_connection_parameters_req(const DM_UPRIM_T * const uprim);
void dm_set_ble_central_connection_paramerters_update_req(const DM_UPRIM_T * const uprim);
void dm_handle_accept_connection_par_update_signal(L2CA_UPRIM_T* message);
void dm_ble_accept_connection_par_update_rsp(const DM_UPRIM_T * const uprim);
#else /* INSTALL_ULP */
#define dm_ble_set_connection_parameters_req NULL
#define dm_ble_update_connection_parameters_req NULL
#define dm_set_ble_central_connection_paramerters_update_req NULL
#define dm_ble_accept_connection_par_update_rsp NULL
#endif /* INSTALL_ULP */

/* HCI connection events. */
void dm_acl_hci_connect_failed(const BD_ADDR_T *const p_bd_addr, hci_return_t status);
void dm_acl_hci_connect_success(const BD_ADDR_T *const p_bd_addr, const hci_connection_handle_t handle, const CsrUint8 enc_mode);
void dm_acl_hci_disconnect_failure(const hci_connection_handle_t handle);
void dm_acl_hci_disconnect_success(const hci_connection_handle_t handle, hci_error_t reason);

/* HCI ULP connection events. */
#ifdef INSTALL_ULP
void dm_acl_ulp_opened(const HCI_EV_ULP_CONNECTION_COMPLETE_T *prim);
#endif

/* Other HCI events. */
#ifndef DISABLE_DM_BREDR
void dm_acl_hci_connect_request(const HCI_EV_CONN_REQUEST_T *const p_prim);
void dm_acl_hci_auth_enable_cc(CsrUint8 status, const CsrUint8 auth_enable);
void dm_acl_write_scan_enable_req(const DM_UPRIM_T *const p_uprim);
void dm_acl_hci_remote_supported_features(const HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE_T *const p_prim);
#else
#define dm_acl_write_scan_enable_req   NULL
#endif
void dm_acl_hci_number_completed_packets(HCI_EV_NUMBER_COMPLETED_PKTS_T *const p_prim);

CsrBool dm_acl_is_connected(const DM_ACL_T *const p_acl);

#ifdef __cplusplus
}
#endif

#endif

