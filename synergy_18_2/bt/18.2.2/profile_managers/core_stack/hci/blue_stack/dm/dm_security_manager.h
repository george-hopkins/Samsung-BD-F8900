#ifndef _DM_SECURITY_MANAGER_H_
#define _DM_SECURITY_MANAGER_H_

#include "csr_synergy.h"
/*!

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              dm_security_manager.h

DESCRIPTION:       The security management entity of the BlueStack Device Manager.



*/

#include "csr_bt_usr_config.h"
#include "dm_acl_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Service record structure. */
typedef struct DM_SM_SERVICE_RECORD_T_tag
{
    struct DM_SM_SERVICE_RECORD_T_tag *p_next;

    DM_SM_SERVICE_T service;

    dm_security_level_t security_level;

    /*! TRUE if this is a standard service record, FALSE if it is a device-
        specific outgoing service record. */
    CsrBool inout;

    union
    {
        CsrBool outgoing_ok;     /*! Standard service record */
        BD_ADDR_T bd_addr;      /*! device-specific outgoing service record */
    } u;
} DM_SM_SERVICE_RECORD_T;

typedef enum
{
    DM_SM_ACCEPTABLE,
    DM_SM_UNACCEPTABLE,
    DM_SM_NOT_FOUND
} DM_SM_RESULT_T;

#define DM_SM_MITM_SERVICES_SOME_BIT 1
#define DM_SM_MITM_SERVICES_ALL_BIT 2
typedef enum
{
    DM_SM_MITM_SERVICES_NONE = 0,
    DM_SM_MITM_SERVICES_SOME = DM_SM_MITM_SERVICES_SOME_BIT,
    DM_SM_MITM_SERVICES_ALL = DM_SM_MITM_SERVICES_SOME_BIT |
                              DM_SM_MITM_SERVICES_ALL_BIT
} DM_SM_MITM_SERVICES_T;

void dm_sm_hci_enc_complete(const hci_connection_handle_t handle, CsrUint8 status, const CsrBool encrypted);
void send_dm_sm_common(DM_UPRIM_T *p_uprim, const TYPED_BD_ADDR_T *const addrt);
CsrUint16 dm_sm_get_diversifier(const TYPED_BD_ADDR_T *const addrt);
CsrBool dm_sm_auto_accept_just_works(void);
void dm_sm_hci_refresh_encryption_key_complete(hci_connection_handle_t handle, hci_return_t status);

#ifndef DISABLE_DM_BREDR

/*! Application requests */
void dm_sm_handle_register_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_register_outgoing_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_unregister_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_unregister_outgoing_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_remove_device_req(DM_UPRIM_T *const p_uprim);


void dm_sm_handle_key_request_rsp(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_key_request_neg_rsp(DM_UPRIM_T *const p_uprim);

void dm_sm_handle_pin_request_rsp(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_authorise_rsp(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_authenticate_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_encrypt_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_io_capability_request_rsp(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_io_capability_request_neg_rsp(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_add_device_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_bonding_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_bonding_cancel_req(DM_UPRIM_T *const p_uprim);
void dm_sm_handle_read_device_req(DM_UPRIM_T *const p_uprim);
void dm_sm_convert_to_hci(DM_UPRIM_T *const p_uprim);

/*! Protocol layer requests */
void dm_sm_handle_access_req(DM_UPRIM_T *const p_uprim);

/*! ACL Manager */
CsrUint8 dm_sm_get_device_link_key(const TYPED_BD_ADDR_T *const addrt, CsrUint16 **pp_key);
void dm_sm_acl_reset(DM_SM_T *p_sm);
fsm_event_t dm_sm_retry_authentication_event(DM_ACL_T *p_acl, CsrUint8 *status);
DM_ACL_CONNECT_T dm_sm_pair_at_link_setup(const DM_ACL_T *p_acl, const DM_SM_SERVICE_T *service);
CsrUint16 dm_sm_acl_closed(DM_ACL_T *p_acl, CsrUint16 reason);
void dm_sm_acl_opened(DM_ACL_T *p_acl, CsrUint8 status);
void dm_sm_remote_ssp_controller_support(DM_ACL_T *p_acl, CsrUint16 ssp_support);

/*! Idle timeout based on security mode of two connected devices */
ACL_IDLE_TIMEOUT_MULTIPLIER_T dm_sm_acl_timeout_security(DM_ACL_T *p_acl);

/*! Internal to Security Manager */
CsrUint8 auth_key_required(const TYPED_BD_ADDR_T *const addrt);
CsrBool legacy_security_ok(const TYPED_BD_ADDR_T *const addrt);
void dm_sm_update_device_link_key(const TYPED_BD_ADDR_T *const addrt,
                                  const CsrUint8 *const p_key,
                                  CsrUint8 key_type);
DM_SM_RESULT_T dm_sm_current_link_key_acceptable(const TYPED_BD_ADDR_T *const addrt, CsrUint16 **pp_key);
void dm_sm_store_authorisation(DM_CONNECTION_T *connection, CsrUint16 authorisation);
void dm_sm_device_db_init(void);
void dm_sm_service_db_init(void);
CsrBool dm_sm_remove_device(const TYPED_BD_ADDR_T *const addrt,
                           const CsrBool temporary_only);
DM_SM_RESULT_T dm_sm_trust(const TYPED_BD_ADDR_T *const addrt,
                           const DM_SM_SERVICE_T *const p_service);
DM_SM_SERVICE_RECORD_T **find_service(const DM_SM_SERVICE_T *service, const BD_ADDR_T *bd_addr);
CsrBool connection_setup_match(const DM_CONNECTION_SETUP_T *const p_cs1, const DM_CONNECTION_SETUP_T *const p_cs2);
CsrUint8 dm_sm_max_authentication_requirement(void);
DM_SM_MITM_SERVICES_T dm_sm_mitm_services(CsrUint16 mitm_mask);
CsrBool service_match(const DM_SM_SERVICE_T *const p_service1, const DM_SM_SERVICE_T *const p_service2);
void dm_sm_update_keys(const TYPED_BD_ADDR_T *const addrt, const DM_SM_KEYS_T *keys);
DM_SM_RESULT_T dm_sm_current_security_acceptable(
        const TYPED_BD_ADDR_T *const addrt,
        CsrUint16 security_requirements);


#define DM_SM_IS_LINK_ENCRYPTED(p_acl) ((p_acl)->dm_acl_client_sm.encrypted)
CsrBool dm_sm_is_ulp_link_authenticated(const TYPED_BD_ADDR_T *const addrt);

CsrBool dm_sm_get_csrk(CsrUint16 *csrk, const TYPED_BD_ADDR_T *const addrt);
TYPED_BD_ADDR_T *dm_sm_resolve_address(const TYPED_BD_ADDR_T *const addrt);
DM_SM_KEY_ENC_CENTRAL_T *dm_sm_get_enc_key(const TYPED_BD_ADDR_T *const addrt);

void dm_sm_update_sec_requirements(const TYPED_BD_ADDR_T *const addrt,
                                   CsrUint16 security_requirements);


CsrBool dm_sm_is_device_present(const TYPED_BD_ADDR_T *const addrt);

#ifndef SM_HAS_FUNCTION_FOR_RAND
void dm_sm_ble_init_complete(void);
#endif

/*! Upstream HCI events */
void dm_sm_hci_remote_host_supported_features(const hci_connection_handle_t handle, CsrUint8 status, const CsrUint16 host_features);
void dm_sm_hci_pin_code_request(const BD_ADDR_T *const p_bd_addr);
void dm_sm_hci_link_key_request(const BD_ADDR_T *const p_bd_addr);
void dm_sm_hci_link_key_notification(const BD_ADDR_T *const p_bd_addr, CsrUint8 key_type, const CsrUint8 key[SIZE_LINK_KEY]);
void dm_sm_hci_auth_success( const hci_connection_handle_t handle);
void dm_sm_hci_auth_failure( const hci_connection_handle_t handle, hci_return_t status);
#ifndef DISABLE_DM_BREDR
void dm_sm_handle_init_req(DM_UPRIM_T *const p_uprim);
CsrBool dm_sm_hci_auth_enable(const hci_return_t status);
void dm_sm_hci_write_enc_mode_complete(const hci_return_t status);
void dm_sm_hci_write_simple_pairing_mode_cc(const CsrUint8 status);
void dm_sm_hci_write_simple_pairing_debug_mode_cc(const CsrUint8 status);
void dm_sm_hci_read_local_oob_data_cc(const CsrUint8 *const oob_hash_c, const CsrUint8 *const oob_rand_r, const CsrUint8 status);
void dm_sm_hci_link_key_request_reply_cc(const BD_ADDR_T *const p_bd_addr, const CsrUint8 status);
void dm_sm_hci_io_capability_request(const BD_ADDR_T *const p_bd_addr);
void dm_sm_hci_io_capability_response(const BD_ADDR_T *const p_bd_addr, const CsrUint8 io_cap_remote,
                const CsrUint8 oob_data_present, const CsrUint8 authentication_requirements);
void dm_sm_hci_simple_pairing_complete(HCI_UPRIM_T *hci_uprim);
void dm_sm_hci_user_confirmation_request(const BD_ADDR_T *const p_bd_addr, const CsrUint32 *const numeric_value);
void dm_sm_hci_user_passkey_request(const BD_ADDR_T *const p_bd_addr);
void dm_sm_hci_remote_oob_data_request(const BD_ADDR_T *const p_bd_addr);
void dm_sm_hci_user_passkey_notification(const BD_ADDR_T *const p_bd_addr, const CsrUint32 *const passkey);
void dm_sm_hci_keypress_notification(const BD_ADDR_T *const p_bd_addr, const CsrUint8 notification_type);
#else
#define dm_sm_handle_init_req  NULL
#define dm_sm_hci_simple_pairing_complete  dm_ev_not_used
#endif

void dm_sm_init(void);

#ifdef ENABLE_SHUTDOWN
void dm_sm_unregister_device_record(void);
void dm_sm_unregister_service_record(void);
void dm_acl_client_deinit_sm(DM_SM_T *p_sm, CsrBool is_ble);
void dm_sm_deinit(void);
#else
#define dm_sm_deinit() /* Nothing to do */
#endif

#else /* DISABLE_DM_BREDR */
#define dm_sm_acl_opened NULL
#define dm_sm_acl_closed NULL
#define dm_sm_acl_reset(arg1) ((void)0)
#define dm_sm_pair_at_link_setup(arg1, arg2) DM_ACL_CONNECT
#define dm_sm_retry_authentication_event(arg1, arg2) FSM_EVENT_NULL
#define dm_sm_init() ((void)0)
#define dm_sm_hci_simple_pairing_complete dm_ev_not_used
#define dm_sm_handle_init_req NULL
#define dm_sm_handle_register_req NULL
#define dm_sm_handle_unregister_req NULL
#define dm_sm_handle_register_outgoing_req NULL
#define dm_sm_handle_unregister_outgoing_req NULL
#define dm_sm_handle_access_req NULL
#define dm_sm_handle_add_device_req NULL
#define dm_sm_handle_remove_device_req NULL
#define dm_sm_handle_link_key_request_rsp NULL
#define dm_sm_handle_pin_request_rsp NULL
#define dm_sm_handle_authorise_rsp NULL
#define dm_sm_handle_authenticate_req NULL
#define dm_sm_handle_encrypt_req NULL
#define dm_sm_handle_io_capability_request_rsp sm_handle_io_capability_request_rsp
#define dm_sm_handle_bonding_req NULL
#define dm_sm_handle_bonding_cancel_req NULL
#define dm_sm_handle_read_device_req NULL
#define dm_sm_handle_io_capability_request_neg_rsp sm_handle_io_capability_request_neg_rsp
#define dm_sm_handle_key_request_rsp sm_handle_key_request_rsp
#define dm_sm_handle_key_request_neg_rsp sm_handle_key_request_neg_rsp
#define dm_sm_convert_to_hci NULL
#define dm_sm_current_security_acceptable(arg1, arg2) DM_SM_UNACCEPTABLE
#define DM_SM_IS_LINK_ENCRYPTED(p_acl) FALSE
#define dm_sm_is_ulp_link_authenticated(arg) FALSE
#define dm_sm_update_keys(arg1, arg2) ((void)0)
#define dm_sm_get_csrk(arg1, arg2) (FALSE)
#define dm_sm_resolve_address(arg1) NULL
#define dm_sm_get_irk(arg1) NULL
#define dm_sm_get_enc_key(arg1) NULL
#endif /* DISABLE_DM_BREDR */

#ifdef __cplusplus
}
#endif

#endif

