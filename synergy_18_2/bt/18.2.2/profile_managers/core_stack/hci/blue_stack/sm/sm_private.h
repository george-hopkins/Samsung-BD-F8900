/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_private.h

\brief  Security Manager private header
*/

#ifndef __SM_PRIVATE_H__
#define __SM_PRIVATE_H__

#include "csr_synergy.h"

#include "sm.h"
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"
#include "l2cap_prim.h"

#include "tbdaddr.h"
#include "error.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_panic.h"
#include "csr_sched.h"
#include "csr_sched.h"
#include "csr_mblk.h"
#include "l2caplib.h"
#include "dm_mgt_interface.h"
#include "dm_hci_interface.h"
#include "dm_acl_manager.h"
#include "dm_security_manager.h"

#include "dmlib.h"
#include "dm_ble_advertising.h"
#ifdef SM_HAS_FUNCTION_FOR_AES
#include "aes.h"
#endif
#ifdef SM_HAS_FUNCTION_FOR_RAND
#include "sm_rand.h"
#endif
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! Xmacro table of PDU types, associated lengths  and dependencies.

    SM_EVENT_TABLE_X(name, length, dependency1, ... , dependency11)

    The dependencies are references to other events. For an event to be
    considered valid, we must be expecting it and not expecting any of
    its dependencies.

    The dependencies are derived from the follow expected ordering:

        Master

    1 SM_EVENT_SECURITY_REQUEST         =
    1 SM_EVENT_APP_IO_CAPABILITIES      =
    2 SM_EVENT_PAIRING_RESPONSE
    3 SM_EVENT_PASSKEY_OR_CONFIRMATION
    4 SM_EVENT_PAIRING_CONFIRM
    5 SM_EVENT_PAIRING_RANDOM
    6 SM_EVENT_HCI_ENCRYPT              =
    6 <keys in order>                   =

    Slave

    1 SM_EVENT_PAIRING_REQUEST
    2 SM_EVENT_APP_IO_CAPABILITIES
    3 SM_EVENT_PASSKEY_OR_CONFIRMATION  =
    3 SM_EVENT_PAIRING_CONFIRM          =
    4 SM_EVENT_PAIRING_RANDOM
    5 SM_EVENT_HCI_ENCRYPT              =
    5 <keys in order>                   =

    Events marked with an =, and having the same number, indicate that there
    is a natural race and we should expect to see these events in any order.
*/
#define SM_EVENT_TABLE_PDUS \
    SM_EVENT_TABLE_X(RESERVED,                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(PAIRING_REQUEST,                7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(PAIRING_RESPONSE,               7, APP_IO_CAPABILITIES, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(PAIRING_CONFIRM,               17, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, 0, 0, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(PAIRING_RANDOM,                17, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        PAIRING_CONFIRM, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, \
                                                        PASSKEY_OR_CONFIRMATION, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(PAIRING_FAILED,                 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(ENCRYPTION_INFORMATION,        17, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        PAIRING_CONFIRM, \
                                                        PAIRING_RANDOM, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, \
                                                        PASSKEY_OR_CONFIRMATION, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(MASTER_IDENTIFICATION,         11, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        PAIRING_CONFIRM, \
                                                        PAIRING_RANDOM, \
                                                        ENCRYPTION_INFORMATION, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, \
                                                        PASSKEY_OR_CONFIRMATION, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(IDENTITY_INFORMATION,          17, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        PAIRING_CONFIRM, \
                                                        PAIRING_RANDOM, \
                                                        ENCRYPTION_INFORMATION, \
                                                        MASTER_IDENTIFICATION, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, \
                                                        PASSKEY_OR_CONFIRMATION, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(IDENTITY_ADDRESS_INFORMATION,   8, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        PAIRING_CONFIRM, \
                                                        PAIRING_RANDOM, \
                                                        ENCRYPTION_INFORMATION, \
                                                        MASTER_IDENTIFICATION, \
                                                        IDENTITY_INFORMATION, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, \
                                                        PASSKEY_OR_CONFIRMATION, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(SIGNING_INFORMATION,           17, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        PAIRING_CONFIRM, \
                                                        PAIRING_RANDOM, \
                                                        ENCRYPTION_INFORMATION, \
                                                        MASTER_IDENTIFICATION, \
                                                        IDENTITY_INFORMATION, \
                                                        IDENTITY_ADDRESS_INFORMATION, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, \
                                                        PASSKEY_OR_CONFIRMATION) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(SECURITY_REQUEST,               2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

/*! Xmacro table of non-PDU type events */
#define SM_EVENT_TABLE_OTHER \
    SM_EVENT_TABLE_X(APP_IO_CAPABILITIES,            0, PAIRING_REQUEST, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(PASSKEY_OR_CONFIRMATION,        0, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, 0, 0, 0, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(HCI_ENCRYPT,                    0, PAIRING_REQUEST, \
                                                        PAIRING_RESPONSE, \
                                                        PAIRING_CONFIRM, \
                                                        PAIRING_RANDOM, \
                                                        SECURITY_REQUEST, \
                                                        APP_IO_CAPABILITIES, \
                                                        PASSKEY_OR_CONFIRMATION, 0, 0, 0, 0) SM_EVENT_TABLE_SEP \
    SM_EVENT_TABLE_X(DATAWRITE_CFM,                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

/*! SM Protocol PDU types. */
typedef enum
{
#define SM_EVENT_TABLE_SEP ,
#define SM_EVENT_TABLE_X(name, length, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11) SM_EVENT_ ## name
    SM_EVENT_TABLE_PDUS,
    SM_EVENT_TABLE_OTHER,
#undef SM_EVENT_TABLE_SEP
#undef SM_EVENT_TABLE_X
    SM_ABORT_WAIT_PDU,
    SM_EVENT_DIV,
    SM_EVENT_0 = 0,
    SM_EVENT_PDU_MAXIMUM = SM_EVENT_APP_IO_CAPABILITIES
} SM_EVENT_TYPE_T;

#define SM_EXPECT(bit) (1 << ((bit) - 1))
#define SM_EXPECT_WITH_ZERO(bit) \
    ((CsrUint16)((bit) == 0 ? 0 : (1U << ((bit) == 0 ? 0 : (bit) - 1))))

/* Defines useful for setting up what we're expecting */
#define SM_EXPECT_COMMON    SM_EXPECT(SM_EVENT_PAIRING_CONFIRM) | \
                            SM_EXPECT(SM_EVENT_PAIRING_RANDOM) | \
                            SM_EXPECT(SM_EVENT_APP_IO_CAPABILITIES) | \
                            SM_EXPECT(SM_EVENT_HCI_ENCRYPT) | \
                            SM_EXPECT(SM_EVENT_PAIRING_FAILED)
#define SM_EXPECT_PASSCONF  SM_EXPECT(SM_EVENT_PASSKEY_OR_CONFIRMATION)
#define SM_EXPECT_MASTER    SM_EXPECT(SM_EVENT_PAIRING_RESPONSE) | \
                            SM_EXPECT(SM_EVENT_SECURITY_REQUEST)
#define SM_EXPECT_SLAVE     SM_EXPECT(SM_EVENT_PAIRING_REQUEST)
#define SM_EXPECT_KEY_ENC   SM_EXPECT(SM_EVENT_ENCRYPTION_INFORMATION) | \
                            SM_EXPECT(SM_EVENT_MASTER_IDENTIFICATION)
#define SM_EXPECT_KEY_ID    SM_EXPECT(SM_EVENT_IDENTITY_INFORMATION) | \
                            SM_EXPECT(SM_EVENT_IDENTITY_ADDRESS_INFORMATION)
#define SM_EXPECT_KEY_SIGN  SM_EXPECT(SM_EVENT_SIGNING_INFORMATION)
#define SM_EXPECT_KEY_SEND  SM_EXPECT(SM_EVENT_DATAWRITE_CFM)

/*! SM Protocol PDU lengths. */
typedef enum
{
#define SM_EVENT_TABLE_SEP ,
#define SM_EVENT_TABLE_X(name, length, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11) SM_EVENT_ ## name ## _LENGTH = length
    SM_EVENT_TABLE_PDUS
#undef SM_EVENT_TABLE_SEP
#undef SM_EVENT_TABLE_X
} SM_EVENT_LENGTH_T;

/*! SM Event dependencies. */
typedef enum
{
#define SM_EVENT_TABLE_SEP ,
#define SM_EVENT_TABLE_X(name, length, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11) \
    SM_DEPEND_ ## name = \
        (SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d1) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d2) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d3) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d4) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d5) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d6) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d7) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d8) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d9) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d10) | \
         SM_EXPECT_WITH_ZERO(SM_EVENT_ ## d11))
SM_EVENT_TABLE_PDUS,
SM_EVENT_TABLE_OTHER
#undef SM_EVENT_TABLE_SEP
#undef SM_EVENT_TABLE_X
} SM_DEPEND_T;

typedef enum
{
    SM_KEY_ENC,
    SM_KEY_ID,
    SM_KEY_SIGN,
    SM_KEY_ENC_SENT,
    SM_KEY_ID_SENT,
    SM_KEY_SIGN_SENT,
    SM_KEYS_SENT,
    SM_KEY_SEND_TO_HOST,
    SM_KEY_GENERATING
} SM_KEY_T;

typedef enum
{
    SM_IR_TO_IRK = 1,
    SM_IR_TO_DHK = 3
} SM_IR_DIVERSIFIER_T;

typedef enum
{
    SM_ER_TO_LTK = 0,
    SM_ER_TO_CSRK = 1
} SM_ER_DIVERSIFIER_T;

typedef enum
{
    SM_SIZE8_SIGNATURE = 8,
    SM_SIZE8_SIGN_COUNTER = 4,
    SM_SIZE8_128 = 16,
    SM_SIZE8_64 = 8
} SM_SIZE8_T;

typedef enum
{
    SM_SIZE16_SIGNATURE = 4,
    SM_SIZE16_BD_ADDR = 3,
    SM_SIZE16_PASSKEY = 3,
    SM_SIZE16_64 = 4,
    SM_SIZE16_128 = 8
} SM_SIZE16_T;

typedef enum
{
    SM_ASSOC_MODEL_NONE,            /* Pairing has not yet started. */
    SM_ASSOC_MODEL_UNKNOWN,         /* Pairing has started, but model has not been determined. */
    SM_ASSOC_MODEL_JUST_WORKS,      /* We're using the Just Works model. */
    SM_ASSOC_MODEL_OOB,             /* We're using the Out Of Band data model. */
    SM_ASSOC_MODEL_DISPLAY_PASSKEY, /* We're the display side of the passkey model. */
    SM_ASSOC_MODEL_ENTER_PASSKEY,   /* We're the keyboard side of the passkey model. */

    /* Dummy assocition mode for (JW or OOB) with user confirmation.
       This is only ever returned by sm_process_pairing_information().
       It is never stored in SM_PAIRING_ASSOC_MODEL(pairing). */
    SM_ASSOC_MODEL_USER_CONFIRMATION
} SM_ASSOC_MODEL_T;

/* TODO consider compressing this, or splitting it into multiple blocks */
/* TODO do we need all elements simultaneously? */
struct SM_PAIRING
{
#ifdef SM_DEBUG_PAIRING_SANITY
    CsrUint16 sanity;
#endif
#ifdef SM_FOR_ROBINSON
    TYPED_BD_ADDR_T addrt_remote;
#endif /* SM_FOR_ROBINSON */

    SM_ASSOC_MODEL_T association_model;

    l2ca_cid_t cid;

    /* Bitfield of expected events */
    CsrUint16 expecting;
#ifndef DISABLE_SM_KEY_GENERATION
    CsrUint16 key_distribution;
#endif /* DISABLE_SM_KEY_GENERATION */

    CsrUint8 encryption_key_size;
    CsrUint16 random[SM_SIZE16_128];
    CsrUint16 confirm[SM_SIZE16_128];
    CsrUint16 tk[SM_SIZE16_128];
    CsrUint16 p1[SM_SIZE16_128];
    DM_SM_KEYS_IND_T *keys_ind;
    DM_SM_SECURITY_COMMON_T *security_cfm;
    CsrSchedTid sm_timeout;
#ifndef SM_HAS_FUNCTION_FOR_RAND
#define RANDS_MAX 16
    CsrUint16 sm_random_number_store[RANDS_MAX];
    CsrUint8 sm_random_number_index;
#endif
};

#ifndef SM_HAS_FUNCTION_FOR_RAND
typedef struct SM_RAND_GENERATION_QUEUE_T_tag
{
    struct SM_RAND_GENERATION_QUEUE_T_tag *next;
    SM_PAIRING_T *pairing;
    CsrUint8 *data;
} SM_RAND_GENERATION_QUEUE_T;
#endif

#ifndef SM_HAS_FUNCTION_FOR_AES
typedef struct SM_AES_CALCULATION_QUEUE_T_tag SM_AES_CALCULATION_QUEUE_T;
#endif

typedef struct SM_SIGNATURE_QUEUE_T_tag
{
    struct SM_SIGNATURE_QUEUE_T_tag *next;
    DM_SM_DATA_SIGN_COMMON_T *prim;
} SM_SIGNATURE_QUEUE_T;


typedef struct
{
    TYPED_BD_ADDR_T addrt_local;

    SM_SIGNATURE_QUEUE_T *signature_queue;

#ifndef DISABLE_SM_KEY_GENERATION
    CsrUint32 sign_counter;
    CsrUint16 div;
    DM_SM_KEY_STATE_T sm_key_state;
#endif
#ifndef SM_HAS_FUNCTION_FOR_RAND
    CsrInt16 ir_er_rand_index;
    SM_RAND_GENERATION_QUEUE_T *rand_generation;
#endif
#ifndef SM_HAS_FUNCTION_FOR_AES
    SM_AES_CALCULATION_QUEUE_T *aes_calculation;
#endif

    /* Encryption key sizes are now configurable */
    CsrUint8 enc_key_size_max_threshold;
    CsrUint8 enc_key_size_min_threshold;

    /* Because Scatternets are prohibited and LTK regeneration is only
       done by the peripheral, we will only need to recreate the LTK
       for one link at any one time. So we can keep the state in global */
    struct
    {
        CsrUint16 div;
        CsrUint8 enc_key_size;
#ifndef SM_HAS_FUNCTION_FOR_AES
        CsrBool mitm;
#endif
    } ltk_regeneration;
} SMCB_T;

extern SMCB_T smcb;

#define SM_TBDADDR_LOCAL() (&smcb.addrt_local)
#define SM_TBDADDR_ADDR_LOCAL() TBDADDR_ADDR(*SM_TBDADDR_LOCAL())
#define SM_TBDADDR_TYPE_LOCAL() TBDADDR_TYPE(*SM_TBDADDR_LOCAL())
#define SM_TBDADDR_ADDR_REMOTE(smp) TBDADDR_ADDR(*SM_TBDADDR_REMOTE(smp))
#define SM_TBDADDR_TYPE_REMOTE(smp) TBDADDR_TYPE(*SM_TBDADDR_REMOTE(smp))

#ifdef SM_FOR_ROBINSON
extern SM_PAIRING_T sm_pairing;
extern CsrBool sm_is_master(TYPED_BD_ADDR_T *addrt);
#define SM_PAIRING_ASSOC_MODEL(smp) (sm_pairing.association_model)
#define SM_TBDADDR_REMOTE(smp) (&sm_pairing.addrt_remote)
#ifdef DISABLE_SM_SLAVE
#define SM_IS_MASTER(smp) TRUE
#else /* DISABLE_SM_SLAVE */
#define SM_IS_MASTER(smp) (sm_is_master(SM_TBDADDR_REMOTE(smp)))
#endif /* DISABLE_SM_SLAVE */
#define SM_EXPECTING(smp) (sm_pairing.expecting)
#define SM_P1(smp) (sm_pairing.p1)
#define SM_RAND(smp) (sm_pairing.random)
#define SM_CONF(smp) (sm_pairing.confirm)
#define SM_TK(smp) (sm_pairing.tk)
#define SM_STK(smp) (sm_pairing.random)
#define SM_HANDLE(smp) (dm_acl_get_handle_by_bd_addr(SM_TBDADDR_REMOTE(smp)))
#define SM_CID(smp) (sm_pairing.cid)
#ifdef DISABLE_SM_KEY_GENERATION
#define SM_KEY_DISTRIBUTION(smp) 0
#else /* DISABLE_SM_KEY_GENERATION */
#define SM_KEY_DISTRIBUTION(smp) (sm_pairing.key_distribution)
#endif /* DISABLE_SM_KEY_GENERATION */
#define SM_KEYS_IND(smp) (sm_pairing.keys_ind)
#define SM_SEC_CFM(smp) (sm_pairing.security_cfm)
#define SM_TIMEOUT(smp) (sm_pairing.sm_timeout)
#define SM_ENCRYPTION_KEY_SIZE(smp) (sm_pairing.encryption_key_size)
#else /* SM_FOR_ROBINSON */
extern DM_ACL_T *sm_acl_from_pairing(SM_PAIRING_T *pairing);
extern CsrBool sm_lock_acl(TYPED_BD_ADDR_T *addrt, l2ca_conflags_t flags);
extern void sm_unlock_acl(TYPED_BD_ADDR_T *addrt);
#define SM_PAIRING_ASSOC_MODEL(smp) ((smp)->association_model)
#define SM_TBDADDR_REMOTE(smp) (&sm_acl_from_pairing(smp)->addrt)
#ifdef DISABLE_SM_SLAVE
#define SM_IS_MASTER(smp) TRUE
#else /* DISABLE_SM_SLAVE */
#define SM_IS_MASTER(smp) \
    ((sm_acl_from_pairing(smp)->flags & DM_ACL_ROLE) == DM_ACL_ROLE_MASTER)
#endif /* DISABLE_SM_SLAVE */
#define SM_EXPECTING(smp) ((smp)->expecting)
#define SM_P1(smp) ((smp)->p1)
#define SM_RAND(smp) ((smp)->random)
#define SM_CONF(smp) ((smp)->confirm)
#define SM_TK(smp) ((smp)->tk)
#define SM_STK(smp) ((smp)->random)
#define SM_HANDLE(smp) (sm_acl_from_pairing(smp)->handle)
#define SM_CID(smp) ((smp)->cid)
#ifdef DISABLE_SM_KEY_GENERATION
#define SM_KEY_DISTRIBUTION(smp) 0
#else /* DISABLE_SM_KEY_GENERATION */
#define SM_KEY_DISTRIBUTION(smp) ((smp)->key_distribution)
#endif /* DISABLE_SM_KEY_GENERATION */
#define SM_KEYS_IND(smp) ((smp)->keys_ind)
#define SM_SEC_CFM(smp) ((smp)->security_cfm)
#define SM_TIMEOUT(smp) ((smp)->sm_timeout)
#define SM_ENCRYPTION_KEY_SIZE(smp) ((smp)->encryption_key_size)
#endif /* SM_FOR_ROBINSON */

/* Offset in octets of SM_EVENT_PAIRING_REQUEST in P1 */
#define P1_OFFSET8_PAIRING_REQ 2

/* Offset in octets of SM_EVENT_PAIRING_RESPONSE in P1 */
#define P1_OFFSET8_PAIRING_RSP \
        (P1_OFFSET8_PAIRING_REQ + SM_EVENT_PAIRING_REQUEST_LENGTH)

#define SM_PAIRING_OFFSET_IO_CAPABILITY 1
#define SM_PAIRING_OFFSET_OOB_DATA 2
#define SM_PAIRING_OFFSET_AUTHREQ 3
#define SM_PAIRING_OFFSET_MAX_ENC_KEY_SIZE 4
#define SM_PAIRING_OFFSET_INITIATOR_KEY_DIST 5
#define SM_PAIRING_OFFSET_RESPONDER_KEY_DIST 6

/* Offset in 16bit words of initiator BD_ADDR in P2 */
#define P2_OFFSET16_BD_ADDR_INITIATOR 3

/* Offset in 16bit words of responder BD_ADDR in P2 */
#define P2_OFFSET16_BD_ADDR_RESPONDER 0


#ifdef SM_DEBUG_PAIRING_SANITY
extern void sm_pairing_sanity(SM_PAIRING_T *pairing);
#define SM_SANITY_CHECK(arg) sm_pairing_sanity(arg)
#else
#define SM_SANITY_CHECK(arg) ((void)0)
#endif

#ifdef SM_HAS_FUNCTION_FOR_AES
extern void sm_privacy_generate_hash_local(TYPED_BD_ADDR_T *addrt, CsrUint8 *addr);
#define SM_STATIC static
#define sm_aes_deinit() ((void)0)
#define sm_aes_pairing_freed(arg1) ((void)0)
#else
#define SM_STATIC

#ifndef DISABLE_SM_KEY_GENERATION
extern void sm_send_keys_to_peer(SM_PAIRING_T *pairing);
#endif /* DISABLE_SM_KEY_GENERATION */

extern void sm_aes_recreate_ltk_from_div(hci_connection_handle_t handle);
extern void sm_encode_key_size_and_mitm(CsrUint16 *rand, CsrBool mitm, CsrUint8 enc_key_size);
extern void sm_aes_deinit(void);
extern void sm_confirm_good(SM_PAIRING_T *pairing, CsrUint16 *rand);
extern void sm_drop_link(SM_PAIRING_T *pairing);
extern void sm_aes_generate_confirm_rand_xor_p1(SM_PAIRING_T *pairing,
                                                CsrUint16 *rand_xor_p1,
                                                const CsrUint16 *rand);
extern void sm_aes_generate_confirm_xor_p2(SM_PAIRING_T *pairing,
                                           CsrUint16 *confirm);
extern CsrBool sm_confirm_value_matches_stored(SM_PAIRING_T *pairing,
                                              CsrUint16 *confirm);
extern void sm_encrypt_using_stk(SM_PAIRING_T *pairing);
extern void sm_aes_stk_generate(SM_PAIRING_T *pairing, CsrUint16 *rand);
extern void sm_aes_irk_distribute(SM_PAIRING_T *pairing);
extern void sm_aes_csrk_distribute(SM_PAIRING_T *pairing);
extern void sm_aes_enc_distribute(SM_PAIRING_T *pairing);
extern CsrBool sm_data_sign_stage1(DM_SM_DATA_SIGN_COMMON_T *prim, CsrUint16 *key);
extern void sm_sign_block(DM_SM_DATA_SIGN_COMMON_T *prim, CsrUint16 i, CsrUint16 *K, CsrUint16 *C, const CsrUint16 *key);
extern void sm_data_sign_check(DM_SM_DATA_SIGN_COMMON_T *prim, CsrUint16 *K, CsrUint16 *C);
extern void sm_aes_recreate_ltk(CsrUint16 ediv, CsrUint16 *rand, hci_connection_handle_t handle);
extern void sm_send_hci_long_term_key_requested_reply( hci_connection_handle_t handle, CsrUint16 *ltk);
extern void sm_aes_pairing_freed(SM_PAIRING_T *pairing);
extern void sm_aes_check_hash(const CsrUint16 *irk,
                              const TYPED_BD_ADDR_T *current_addrt,
                              const TYPED_BD_ADDR_T *permanent_addrt,
                              CsrUint8 identifier);
extern void sm_privacy_aes_hci(const TYPED_BD_ADDR_T *current_addrt,
                               const TYPED_BD_ADDR_T *permanent_addrt,
                               CsrUint8 identifier,
                               CsrBool success);
extern void sm_aes_generate_hash(CsrUint8 *rand);
#endif

extern void sm_fixed_cid_mapped(SM_PAIRING_T *pairing, CsrUint16 authentication_requirements);
extern void sm_fixed_cid_unmapped(l2ca_cid_t cid);

extern void sm_pdu_from_air(SM_PAIRING_T *pairing, SM_EVENT_TYPE_T type, void *data);
extern SM_ASSOC_MODEL_T sm_process_pairing_information(SM_PAIRING_T *pairing);
extern void sm_datawrite_cfm(l2ca_cid_t cid, CsrUint16 context, l2ca_data_result_t result);
extern void sm_pairing_timeout(CsrUint16 pdu_type, void *pairing_arg);
extern void sm_start_encryption(SM_PAIRING_T *pairing, CsrUint16 *ltk, CsrUint16 *random, CsrUint16 ediv);
extern void sm_reduce_enc_key_size(CsrUint16 *key, CsrUint8 size);
extern void sm_pairing_failed(SM_PAIRING_T *pairing, SM_ERROR_T error);

/* L2CAP to-air */
extern void sm_l2cap_datawrite_req(SM_PAIRING_T *pairing, CsrUint8 *pdu);
extern void sm_l2cap_pairing_failed(l2ca_cid_t cid, SM_ERROR_T error);
extern void sm_l2cap_pairing_confirm(SM_PAIRING_T *pairing, const CsrUint16 *packed_confirm);
extern void sm_l2cap_pairing_rand(SM_PAIRING_T *pairing);
extern void sm_l2cap_pairing_reqrsp(SM_PAIRING_T *pairing,
                                    CsrUint8 *data);
extern void sm_l2cap_security_request(SM_PAIRING_T *pairing,
                                      CsrUint8 authentication_requirements);
extern void sm_l2cap_send_key(SM_PAIRING_T *pairing,
                              SM_EVENT_TYPE_T type,
                              CsrUint16 *data);
extern void sm_l2cap_send_identity_address(SM_PAIRING_T *pairing);

/* To-host */
extern void send_dm_sm_io_capability_request_ind(SM_PAIRING_T *pairing);
extern void send_dm_common_ind(dm_prim_t type, TYPED_BD_ADDR_T *addrt, CsrUint16 flags);
extern void send_dm_sm_io_capability_response_ind(SM_PAIRING_T *pairing);
extern void sm_send_passkey_request(SM_PAIRING_T *pairing);
extern void sm_send_passkey_indication(SM_PAIRING_T *pairing, CsrUint16 *passkey);
extern void send_user_confirmation_request(SM_PAIRING_T *pairing);

extern void sm_send_keys_to_host(SM_PAIRING_T *pairing,
                                 SM_EVENT_TYPE_T type,
                                 void *ptr);
extern void sm_send_pairing_complete(SM_PAIRING_T *pairing, CsrUint8 status, CsrBool sm_err_base);
extern void sm_send_key_request(TYPED_BD_ADDR_T *addrt,
                                CsrUint16 security_requirements,
                                DM_SM_KEY_TYPE_T key_type,
                                CsrUint16 encryption_diversifier);
extern void sm_send_csrk_div_request(TYPED_BD_ADDR_T *addrt);

extern void sm_send_csrk_counter_change_ind(TYPED_BD_ADDR_T *addrt, CsrUint32 counter);

/* AES */
extern void memcpy_unpack128(CsrUint8 *dst, const CsrUint16 *src);
extern void memcpy_pack128(CsrUint16 *dst, const CsrUint8 *src);

#ifdef SM_DEBUG_CRYPTO_CALCULATIONS
#define SM_AES_DEBUG_KEY(key, type) sm_aes_debug_key(key, type)
extern void sm_aes_debug_key(const CsrUint16 *const key, const CsrUint8 type);
#else
#define SM_AES_DEBUG_KEY(key, type)
#endif
extern void sm_aes_encrypt_128(CsrUint16 *packed_out, const CsrUint16 *packed_in, const CsrUint16 *packed_key);
extern void sm_aes_pack_pairing(CsrUint16 *out, const CsrUint8 *in);
extern void sm_aes_generate_rand_confirm(SM_PAIRING_T *pairing);
extern CsrBool sm_aes_check_rand_confirm(SM_PAIRING_T *pairing, const CsrUint16 *rand);
#define SM_DHK(key_out) sm_ir_diversifier((key_out), SM_IR_TO_DHK)
#define SM_IRK(key_out) sm_ir_diversifier((key_out), SM_IR_TO_IRK)
#define SM_LTK(key_out, div) sm_er_diversifier((key_out), div, SM_ER_TO_LTK)
#define SM_CSRK(key_out, div) sm_er_diversifier((key_out), div, SM_ER_TO_CSRK)
extern void sm_ir_diversifier(CsrUint16 *key_out, SM_IR_DIVERSIFIER_T div);
extern void sm_er_diversifier(CsrUint16 *key_out,
                              CsrUint16 div,
                              SM_ER_DIVERSIFIER_T r);
extern CsrUint16 sm_div_toggle_mask(const CsrUint16 *rand, CsrUint16 value);
extern void sm_xor_128(CsrUint16 *out, const CsrUint16 *a, const CsrUint16 *b);

extern SM_PAIRING_T *sm_find_by_tbdaddr(TYPED_BD_ADDR_T *addrt);
extern SM_PAIRING_T *sm_find_by_handle(hci_connection_handle_t handle);
extern SM_PAIRING_T *sm_find_by_cid(l2ca_cid_t cid);
extern SM_PAIRING_T *sm_add(TYPED_BD_ADDR_T *addrt, l2ca_cid_t cid);

extern void sm_send_hci_long_term_key_requested_neg_reply(hci_connection_handle_t handle);
extern void sm_privacy_resolvable_random_address(CsrUint8 *addr);
extern void sm_privacy_init(void);
extern void sm_data_sigining_deinit(void);
extern void sm_data_unsign_key_missing(TYPED_BD_ADDR_T *addrt, CsrBool verify);
extern void sm_data_unsign_key_returned(TYPED_BD_ADDR_T *addrt, CsrBool verify);

/* Synergy-only random number handling. */
#ifdef SM_HAS_FUNCTION_FOR_RAND
#define sm_pairing_rand_generate(smp) ((void)0)
#define sm_rand_is_generated(pairing, l2cap_data) TRUE
#define RANDOM_NUMBER(instance, dst, length) random_number((dst), (length))
#define sm_rand_pairing_freed(arg) ((void)0)
#define sm_privacy_rand_read(dst, count) random_number((dst), (count))
#else /* SM_HAS_FUNCTION_FOR_RAND */
#define RANDOM_NUMBER(instance, dst, length) \
    sm_pairing_random_number((instance), (dst), (length))
extern void sm_pairing_rand_generate(SM_PAIRING_T *pairing);
extern void sm_rand_generate(CsrUint8 num);
extern void sm_pairing_random_number(SM_PAIRING_T *pairing,
                                     CsrUint16 *rand,
                                     CsrUint8 length);
extern CsrBool sm_rand_is_generated(SM_PAIRING_T *pairing, CsrUint8 *l2cap_data);
extern void sm_rand_pairing_freed(SM_PAIRING_T *pairing);
extern void sm_privacy_rand_read(CsrUint16 *dst, CsrUint8 count);
extern CsrUint16 *sm_privacy_rand_complete(const CsrUint8 *random);


#endif /* SM_HAS_FUNCTION_FOR_RAND */

#ifdef __cplusplus
}
#endif

#endif  /* __SM_PRIVATE_H__ */
