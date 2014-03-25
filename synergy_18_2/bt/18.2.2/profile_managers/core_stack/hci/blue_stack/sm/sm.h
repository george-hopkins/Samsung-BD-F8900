/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm.h

\brief  Security Manager public header
*/

#ifndef __SM_H__
#define __SM_H__

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "l2cap_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SM_PAIRING SM_PAIRING_T;

#ifdef INSTALL_SM_MODULE

/* TODO Make these configurable */
#ifndef SM_MAX_ENC_KEY_SIZE
#define SM_MAX_ENC_KEY_SIZE 0x10
#endif
#ifndef SM_MIN_ENC_KEY_SIZE
#define SM_MIN_ENC_KEY_SIZE 0x07
#endif

extern void sm_init(void);
extern void sm_read_bd_addr_cc(CsrUint8 status, BD_ADDR_T *bd_addr);
extern void sm_l2cap_handler(L2CA_UPRIM_T *const uprim);
extern void sm_handle_security_req(DM_UPRIM_T *const uprim);
extern void sm_handle_data_sign_req(DM_UPRIM_T *const uprim);
extern void sm_handle_key_request_rsp(DM_UPRIM_T *const uprim);
extern void sm_handle_key_request_neg_rsp(DM_UPRIM_T *const uprim);
extern void sm_handle_io_capability_request_rsp(DM_UPRIM_T *const uprim);
extern void sm_handle_io_capability_request_neg_rsp(DM_UPRIM_T *const uprim);
extern void sm_handle_passkey_request_rsp(DM_UPRIM_T *const uprim);
extern void sm_pairing_ltk_req(hci_connection_handle_t handle,
                               CsrUint8 *random_number,
                               CsrUint16 encryption_diversifier);
extern void sm_encryption_changed(hci_connection_handle_t handle,
                                  CsrBool encrypted, CsrUint8 status);

extern void sm_rand_complete(CsrUint8 *random);
extern void sm_handle_passkey_response(DM_UPRIM_T *const uprim);
extern void sm_handle_passkey_neg_response(DM_UPRIM_T *const uprim);
extern void sm_handle_configure_local_address_req(DM_UPRIM_T *const uprim);
extern void sm_handle_generate_nonresolvable_private_address_req(DM_UPRIM_T *const uprim);
extern void sm_handle_user_confirmation_request_neg_rsp(DM_UPRIM_T *const uprim);
extern void sm_handle_user_confirmation_request_rsp(DM_UPRIM_T *const uprim);
extern void sm_free(SM_PAIRING_T *pairing);
extern void sm_acl_closed(SM_PAIRING_T *pairing);
CsrBool sm_update_ir_er_div(CsrUint16 options,
                           DM_SM_KEY_STATE_T *sm_key_state,
                           CsrUint16 *sm_div_state,
                           CsrUint32 *sm_sign_counter);
void sm_set_enc_key_sizes(CsrUint8 max_size, CsrUint8 min_size);
void sm_key_request_rsp_div(TYPED_BD_ADDR_T * addrt, CsrBool response);
extern void sm_aes_handle_encrypt_result(CsrUint8 status,
                                         const CsrUint8 *encrypted_data);
extern void sm_privacy_generate_nonresolvable_private_address(TYPED_BD_ADDR_T *addrt);
#ifdef DISABLE_SM_PRIVACY
#define sm_privacy_is_enabled() FALSE
#define sm_handle_advertising_report(prim) \
    do { TBDADDR_INVALIDATE((prim)->permanent_addrt); DM_SendMessageAMphandle(prim); } while FALSE
#define sm_privacy_address_type() HCI_ULP_ADDRESS_PUBLIC
#else
extern void sm_handle_advertising_report(DM_HCI_ULP_ADVERTISING_REPORT_IND_T *prim);
extern CsrBool sm_privacy_is_enabled(void);
extern CsrUint8 sm_privacy_address_type(void);
#endif

#ifdef SM_HAS_FUNCTION_FOR_AES
#define SM_PRIVACY_CHECK_HASH(irk, current_addrt, permanent_addrt) \
        sm_privacy_check_hash((irk), (current_addrt))
extern CsrBool sm_privacy_check_hash(const CsrUint16 *irk,
                                    const TYPED_BD_ADDR_T *current_addrt);
#else /* SM_HAS_FUNCTION_FOR_AES */
#define SM_PRIVACY_CHECK_HASH(irk, current_addrt, permanent_addrt) \
        sm_privacy_check_hash((irk), (current_addrt), (permanent_addrt))
extern CsrBool sm_privacy_check_hash(const CsrUint16 *irk,
                                    const TYPED_BD_ADDR_T *current_addrt,
                                    const TYPED_BD_ADDR_T *permanent_addrt);
#endif /* SM_HAS_FUNCTION_FOR_AES */

#else /* INSTALL_SM_MODULE */

#define sm_init() ((void)0)
#define sm_read_bd_addr_cc(status, bd_addr) ((void)0)
#define sm_l2cap_handler(dummy) ((void)0)
#define sm_handle_security_req NULL
/* TODO the next ones are not just SM */
#define sm_handle_data_sign_req NULL
#define sm_handle_key_request_rsp NULL
#define sm_handle_key_request_neg_rsp NULL
#define sm_handle_set_privacy_req NULL
#define sm_handle_generate_nonresolvable_private_address_req NULL
#define sm_handle_configure_local_address_req NULL
#define sm_handle_io_capability_request_rsp(dummy) ((void)0)
#define sm_handle_passkey_request_rsp(dummy) ((void)0)
#define sm_handle_link_key_request_rsp(duimmy) ((void)0)
#define sm_pairing_ltk_req(handle, rand, enc_div) ((void)0)
#define sm_encryption_changed(handle, encrypted, status) ((void)0)
#define sm_long_term_key_cc(dummy) ((void)0)
#define sm_handle_passkey_response(dummy) ((void)0)
#define sm_acl_closed(dummy) ((void)0)
#define sm_free(dummy) ((void)0)
#define sm_set_enc_key_sizes(arg1, arg2) ((void)0)
#define sm_key_request_rsp_div(arg1, arg2) ((void)0)
#define sm_update_ir_er_div(arg1, arg2, arg3, arg4) (TRUE)
#define sm_handle_advertising_report(prim) \
    do { TBDADDR_INVALIDATE((prim)->permanent_addrt); DM_SendMessageAMphandle(prim); } while FALSE
#define sm_resolve_address(permanent_addrt, current_addrt) TBDADDR_INVALIDATE(*(permanent_addrt))
#define sm_privacy_check_hash(arg1, arg2) FALSE
#define sm_privacy_is_enabled() FALSE
#define sm_privacy_address_type() HCI_ULP_ADDRESS_PUBLIC
#endif /* INSTALL_SM_MODULE */

#ifdef __cplusplus
}
#endif

#endif  /* __SM_H__ */
