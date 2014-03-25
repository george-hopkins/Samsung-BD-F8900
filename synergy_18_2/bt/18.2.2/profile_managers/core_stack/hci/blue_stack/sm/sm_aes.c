/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_aes.c

\brief  This module handles AES via HCI_ULP_ENCRYPT. It's for when we
        do not have access to an aes_encrypt() function. It's only necessary
        off-chip, although it can be used on-chip for testing purposes.

        The strategy is to add state to a linked list when sending
        HCI_ULP_ENCRYPT and retrieve it when the command complete comes
        back again. We then use that state to work out what to do next.
*/

#include "csr_synergy.h"
#include "sm_private.h"


#ifdef INSTALL_SM_MODULE
#ifndef SM_HAS_FUNCTION_FOR_AES

/*! Pointers to functions that deal with AES results */
typedef void (*DM_AES_RESULT_FUNCTION_T)(SM_AES_CALCULATION_QUEUE_T *state,
                                         CsrUint16 *result);


/* X macros used to generate function prototypes, a function pointer array and
   an enum of indices into that array. */
#define SM_AES_KEY_GENERATION_FUNCTION_CALLBACKS \
    SM_AES_FUNCTION_CALLBACKS_X(KEY_DIST_LTK_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(KEY_DIST_DHK_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(KEY_DIST_EDIV_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(KEY_DIST_IRK_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(KEY_DIST_CSRK_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(LTK_DHK_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(LTK_RECREATE_DIV_UNHIDE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(LTK_RECREATE_LTK_GENERATE)

#define SM_AES_OTHER_FUNCTION_CALLBACKS \
    SM_AES_FUNCTION_CALLBACKS_X(CONFIRM_GENERATE_PART1) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(CONFIRM_GENERATE_PART2) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(CONFIRM_CHECK_PART1) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(CONFIRM_CHECK_PART2) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(STK_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(PRIVATE_ADDRESS_GENERATE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(PRIVATE_ADDRESS_RESOLVE) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(SIGN_GENERATE_CSRK) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(SIGN_GENERATE_K_PART1) SM_AES_FUNCTION_CALLBACKS_SEP \
    SM_AES_FUNCTION_CALLBACKS_X(SIGN_ITERATE)

#ifdef DISABLE_SM_KEY_GENERATION
#define SM_AES_FUNCTION_CALLBACKS SM_AES_OTHER_FUNCTION_CALLBACKS
#else /* DISABLE_SM_KEY_GENERATION */
#define SM_AES_FUNCTION_CALLBACKS SM_AES_KEY_GENERATION_FUNCTION_CALLBACKS SM_AES_FUNCTION_CALLBACKS_SEP \
                                  SM_AES_OTHER_FUNCTION_CALLBACKS
#endif /* DISABLE_SM_KEY_GENERATION */

/*! Function prototypes compatible with DM_AES_RESULT_FUNCTION_T */
#define SM_AES_FUNCTION_CALLBACKS_X(name) static void sm_aes_ ## name(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
#define SM_AES_FUNCTION_CALLBACKS_SEP ;
SM_AES_FUNCTION_CALLBACKS;
#undef SM_AES_FUNCTION_CALLBACKS_X
#undef SM_AES_FUNCTION_CALLBACKS_SEP

/*! Indices into callback function-pointer array */
typedef enum
{
#define SM_AES_FUNCTION_CALLBACKS_X(name) SM_AES_ ## name
#define SM_AES_FUNCTION_CALLBACKS_SEP ,
    SM_AES_FUNCTION_CALLBACKS
#undef SM_AES_FUNCTION_CALLBACKS_X
#undef SM_AES_FUNCTION_CALLBACKS_SEP
} SM_AES_FUNCTION_INDEX_T;

/* Callback function-pointer array */
static const DM_AES_RESULT_FUNCTION_T sm_aes_fn[] =
{
#define SM_AES_FUNCTION_CALLBACKS_X(name) sm_aes_ ## name
#define SM_AES_FUNCTION_CALLBACKS_SEP ,
    SM_AES_FUNCTION_CALLBACKS
#undef SM_AES_FUNCTION_CALLBACKS_X
#undef SM_AES_FUNCTION_CALLBACKS_SEP
};

/*! Saved state during AES calculation */
struct SM_AES_CALCULATION_QUEUE_T_tag
{
    struct SM_AES_CALCULATION_QUEUE_T_tag *next;
    CsrBool valid;
    SM_AES_FUNCTION_INDEX_T fn_index;
    SM_PAIRING_T *pairing;
    void *data;
};

/*! \brief A pairing structure has been destroyed, so mark
    all AES elements assocaited with that structure as invalid.

    When we get the command completes back again, we will know
    to ignore them and avoid dereferencing flailing pointers.
    \param pairing Pointer to pairing structure that is about to die.
*/
void sm_aes_pairing_freed(SM_PAIRING_T *pairing)
{
    SM_AES_CALCULATION_QUEUE_T *el;

    SM_SANITY_CHECK(pairing);

    for (el = smcb.aes_calculation; el != NULL; el = el->next)
    {
        if (el->pairing == pairing)
            el->valid = FALSE;
    }
}

#ifdef SM_DEBUG_SLOW_HCI
/*! \brief Timer-compatible wrapper around send_to_hci()

    \param arg1 Not used.
    \param arg2 Pointer to HCI command primitive cast to (void*).
*/
static void sm_timed_send_to_hci(CsrUint16 arg1, void *arg2)
{
    send_to_hci(arg2);
}

/*! \brief Debug replacement for send_to_hci() to delay before sending
    the HCI command. Might be useful to try to provoke possible races.

    \param hci_prim Pointer to HCI command primitive cast to (DM_UPRIM_T*)
*/
static void sm_delayed_send_to_hci(DM_UPRIM_T *hci_prim)
{
    CsrSchedTid dummy;

    timer_start(&dummy,
                CSR_SCHED_MILLISECOND*100,
                sm_timed_send_to_hci,
                0,
                hci_prim);
}

#define SM_SEND_TO_HCI(arg) sm_delayed_send_to_hci(arg)
#else /* SM_DEBUG_SLOW_HCI */
#define SM_SEND_TO_HCI(arg) send_to_hci(arg)
#endif /* SM_DEBUG_SLOW_HCI */

/*! \brief Generate HCI_ULP_ENCRYPT command and send to HCI.
    \param packed_in plain text packet into CsrUint16 array.
    \param packed_key AES key packed into CsrUint16 array.
*/
static void sm_aes_hci_ulp_encrypt(const CsrUint16 *packed_in,
                                   const CsrUint16 *packed_key)
{
    HCI_ULP_ENCRYPT_T *hci_prim = pnew(HCI_ULP_ENCRYPT_T);

    hci_prim->common.op_code = HCI_ULP_ENCRYPT;
    hci_prim->common.length = DM_HCI_SENDER_SECURITY_MANAGER;
    memcpy_unpack128(hci_prim->aes_key, packed_key);
    memcpy_unpack128(hci_prim->plaintext_data, packed_in);

    SM_SEND_TO_HCI((DM_UPRIM_T*)hci_prim);
}

/*! \brief Function to act as replacement for aes_encrypt_128().
           Adds state to linked list and calls sm_aes_hci_ulp_encrypt().
    \param packed_in plain text packet into CsrUint16 array.
    \param packed_key AES key packed into CsrUint16 array.
    \param pairing Pointer to pairing structrure. Can be NULL if not assocaited with pairing.
    \param data Pointer to other context data. The default behaviour is to
           free this after the command complete returns.
    \param fn_index Index into action function pointer array. Specifies which
           function gets called when the command complete returns.
*/
static void sm_aes_encrypt_128_offchip(const CsrUint16 *packed_in,
                                       const CsrUint16 *packed_key,
                                       SM_PAIRING_T *pairing,
                                       void *data,
                                       SM_AES_FUNCTION_INDEX_T fn_index)
{
    SM_AES_CALCULATION_QUEUE_T *elp, **elpp;

    /* Send HCI command. */
    sm_aes_hci_ulp_encrypt(packed_in, packed_key);

    /* Find end of queue. */
    for (elpp = &smcb.aes_calculation; (elp = *elpp) != NULL; elpp = &elp->next)
        /* empty */;

    /* Add new element. */
    elp = pnew(SM_AES_CALCULATION_QUEUE_T);
    elp->next = NULL;
    elp->valid = TRUE;
    elp->pairing = pairing;
    elp->fn_index = fn_index;
    elp->data = data;
    *elpp = elp;
}

/*! \brief Deal with HCI command complete events for HCI_ULP_ENCRYPT.
    \param status Indication of success or failure.
    \param encrypted_data Result of AES encryption.
*/
void sm_aes_handle_encrypt_result(CsrUint8 status,
                                  const CsrUint8 *encrypted_data)
{
    if (smcb.aes_calculation != NULL)
    {
        /* Remove element from head of queue. */
        SM_AES_CALCULATION_QUEUE_T *elp = smcb.aes_calculation;
        smcb.aes_calculation = elp->next;

        /* Callback with packed data */
        if (elp->valid)
        {
            if (status == HCI_SUCCESS)
            {
                CsrUint16 packed[SM_SIZE16_128];
                memcpy_pack128(packed, encrypted_data);
                sm_aes_fn[elp->fn_index](elp, packed);
            }
            else
                sm_aes_fn[elp->fn_index](elp, NULL);
        }

        /* Free data context as well as element itself. */
        CsrPmemFree(elp->data);
        CsrPmemFree(elp);
    }
}

/*! \brief Part 1 of Confirm value generation. This is called for
           Both Confirm creation and recreation - just with
           different indices.

           What we're doing here is e(TK, P1 ^ rand).

    \param pairing Pointer to pairing structure.
    \param rand Pointer to packed rand.
    \param fn_index Function pointer array index.
*/
static void sm_aes_confirm_start_part1(SM_PAIRING_T *pairing,
                                       CsrUint16 *rand,
                                       SM_AES_FUNCTION_INDEX_T fn_index)
{
    CsrUint16 rand_xor_p1[SM_SIZE16_128];

    SM_SANITY_CHECK(pairing);

    sm_aes_generate_confirm_rand_xor_p1(pairing, rand_xor_p1, rand);
    sm_aes_encrypt_128_offchip(rand_xor_p1,
                               SM_TK(pairing),
                               pairing,
                               rand,
                               fn_index);
}

/*! \brief Part 2 of Confirm value generation. This is called for
           both Confirm creation and recreation - just with
           different indices.

           What we're doing here is e(TK, P2 ^ part1)
           Where part1 are the results from part 1 (above).

    \param state The state stored from part 1.
    \param result The AES result from part 1.
    \param fn_index Function pointer array index.
*/
static void sm_aes_confirm_start_part2(SM_AES_CALCULATION_QUEUE_T *state,
                                       CsrUint16 *result,
                                       SM_AES_FUNCTION_INDEX_T fn_index)
{
    SM_SANITY_CHECK(state->pairing);

    sm_aes_generate_confirm_xor_p2(state->pairing, result);
    sm_aes_encrypt_128_offchip(result,
                               SM_TK(state->pairing),
                               state->pairing,
                               state->data,
                               fn_index);
    state->data = NULL;
}

/*! \brief Check consistency of given rand against stored confirm
    This is really just a wrapper around sm_aes_confirm_start_part1().

    \param pairing Pointer to pairing structure.
    \param rand Pointer to rand.
*/
CsrBool sm_aes_check_rand_confirm(SM_PAIRING_T *pairing, const CsrUint16 *rand)
{
    SM_SANITY_CHECK(pairing);

    sm_aes_confirm_start_part1(pairing,
                               pcopy(rand, SM_SIZE16_128 * sizeof(CsrUint16)),
                               SM_AES_CONFIRM_CHECK_PART1);
    return FALSE;
}

/* \brief Generate rand and confirm for transmission to peer.

   \param pairing Pointer to pairing structure.
*/
void sm_aes_generate_rand_confirm(SM_PAIRING_T *pairing)
{
    SM_SANITY_CHECK(pairing);

    /* Generate rand and store in SM_RAND(pairing). */
    RANDOM_NUMBER(pairing, SM_RAND(pairing), SM_SIZE16_128);

    /* Start process of calculating confirm. */
    sm_aes_confirm_start_part1(pairing,
                               pcopy(SM_RAND(pairing), SM_SIZE16_128 * sizeof(CsrUint16)),
                               SM_AES_CONFIRM_GENERATE_PART1);
}

/* \brief Generate STK from combination of Mrand and Srand.

   \param pairing Pointer to pairing structure.
   \param rand Pointer to Mrand/Srand combination.
*/
void sm_aes_stk_generate(SM_PAIRING_T *pairing, CsrUint16 *rand)
{
    SM_SANITY_CHECK(pairing);

    sm_aes_encrypt_128_offchip(rand,
                               SM_TK(pairing),
                               pairing,
                               NULL,
                               SM_AES_STK_GENERATE);
}

#ifndef DISABLE_SM_KEY_GENERATION
/* \brief Generate IRK by diversifying IR.
   \param pairing Pointer to pairing structure.
   \param context Opaque context returned the other end.
*/
static void sm_aes_irk_generate(SM_PAIRING_T *pairing, void *context)
{
    CsrUint16 plain_text[SM_SIZE16_128];

    CsrMemSet(plain_text, 0, sizeof(plain_text));
    plain_text[0] = (CsrUint16)SM_IR_TO_IRK;
    sm_aes_encrypt_128_offchip(plain_text,
                               smcb.sm_key_state.ir,
                               pairing,
                               context,
                               SM_AES_KEY_DIST_IRK_GENERATE);
}

/* \brief Generate IRK for distribution.
   \param pairing Pointer to pairing structure.
*/
void sm_aes_irk_distribute(SM_PAIRING_T *pairing)
{
    SM_SANITY_CHECK(pairing);

    sm_aes_irk_generate(pairing, NULL);
}

/* \brief Generate IRK to make a resolvable private address.
   \param rand Random part of address.
*/
void sm_aes_generate_hash(CsrUint8 *rand)
{
    sm_aes_irk_generate(NULL, rand);
}

/* \brief Generate CSRK for distribution.
   \param pairing Pointer to pairing structure.
*/
void sm_aes_csrk_distribute(SM_PAIRING_T *pairing)
{
    CsrUint16 plain_text[SM_SIZE16_128];

    SM_SANITY_CHECK(pairing);

    /* Diversify ER. */
    CsrMemSet(plain_text, 0, SM_SIZE16_128 * sizeof(CsrUint16));
    plain_text[0] = smcb.div;
    plain_text[1] = (CsrUint16)SM_ER_TO_CSRK;
    sm_aes_encrypt_128_offchip(plain_text,
                               smcb.sm_key_state.er,
                               pairing,
                               NULL,
                               SM_AES_KEY_DIST_CSRK_GENERATE);
}

/* \brief Diversify IR to recover the DHK.
   \param pairing Pointer to pairing structure.
   \param data Opaque context.
   \param fn_index Function pointer array index.
*/
static void sm_aes_recover_dhk(SM_PAIRING_T *pairing,
                               void *data,
                               SM_AES_FUNCTION_INDEX_T fn_index)
{
    CsrUint16 plain_text[SM_SIZE16_128];

    if (pairing != NULL)
    {
        SM_SANITY_CHECK(pairing);
    }

    /* Diversify IR to get DHK. */
    CsrMemSet(plain_text, 0, SM_SIZE16_128 * sizeof(CsrUint16));
    plain_text[0] = SM_IR_TO_DHK;
    sm_aes_encrypt_128_offchip(plain_text,
                               smcb.sm_key_state.ir,
                               pairing,
                               data,
                               fn_index);
}

/* \brief Generate EDIV, RAND and LTK.
   \param pairing Pointer to pairing structure.
*/
void sm_aes_enc_distribute(SM_PAIRING_T *pairing)
{
    CsrUint16 plain_text[SM_SIZE16_128];
    CsrUint16 *div = CsrPmemAlloc(sizeof(CsrUint16));

    SM_SANITY_CHECK(pairing);

    *div = smcb.div;

    /* Start process of generating DHK from IR. */
    sm_aes_recover_dhk(pairing, div, SM_AES_KEY_DIST_DHK_GENERATE);

    /* Diversify ER to get LTK */
    CsrMemSet(plain_text, 0, SM_SIZE16_128 * sizeof(CsrUint16));
    plain_text[0] = smcb.div;
    plain_text[1] = (CsrUint16)SM_ER_TO_LTK;
    sm_aes_encrypt_128_offchip(plain_text,
                               smcb.sm_key_state.er,
                               pairing,
                               NULL,
                               SM_AES_KEY_DIST_LTK_GENERATE);
}

/*! \brief Structure to track state while making an LTK */
typedef struct
{
    CsrUint16 rand[SM_SIZE16_128];   /* 64bit random number in 128bit array */
    CsrUint16 ediv;                  /* Encrypted diversifier. */
    hci_connection_handle_t handle; /* HCI connection handle. */
} SM_LTK_RECREATE_STATE_T;

/*! \brief Recover the LTK from ediv and rand provided by peer device.
    \param ediv Encrypted diversifier.
    \param rand Random number.
    \param handle HCI connection handle.
*/
void sm_aes_recreate_ltk(CsrUint16 ediv, CsrUint16 *rand, hci_connection_handle_t handle)
{
    SM_LTK_RECREATE_STATE_T *ltk_recreate = zpnew(SM_LTK_RECREATE_STATE_T);

    CsrMemCpy(ltk_recreate->rand, rand, SM_SIZE16_64 * sizeof(CsrUint16));
    ltk_recreate->ediv = ediv;
    ltk_recreate->handle = handle;

    /* Recreate DHK and pass everything else in as context to reclaim later. */
    sm_aes_recover_dhk(NULL, ltk_recreate, SM_AES_LTK_DHK_GENERATE);
}
#endif /* !DISABLE_SM_KEY_GENERATION */

/*! \brief Structure to track state while signing data */
typedef struct
{
    DM_SM_DATA_SIGN_COMMON_T prim;  /* Copy of primitive. */
    CsrUint16 csrk[SM_SIZE16_128];   /* Signing key. */
    CsrUint16 K[SM_SIZE16_128];      /* K1 or K2 - part of signing process */
    CsrUint16 C[SM_SIZE16_128];      /* Signature - maybe partly complete */
    CsrUint16 loop_index;            /* Track progress through the signing loop */
} SM_AES_SIGNING_STATE_T;

/*! \brief Part of the data signing process.
           sign->K should point to a zeroed array.
    \param sign Pointer to signing state structure.
*/
static void sm_aes_data_sign_make_k_part1(SM_AES_SIGNING_STATE_T *sign)
{
    sm_aes_encrypt_128_offchip(sign->K,
                               sign->csrk,
                               NULL,
                               sign,
                               SM_AES_SIGN_GENERATE_K_PART1);
}

/*! \brief Send copy of signing confirmation and free signing state.
    \param sign Pointer to signing state structure.
*/
static void sm_aes_send_data_sign_cfm(SM_AES_SIGNING_STATE_T *sign)
{
    DM_SendMessage(pcopy(&sign->prim, sizeof(DM_SM_DATA_SIGN_COMMON_T)));
    CsrPmemFree(sign);
}

/*! \brief Sign some data.
    \param uprim Pointer to SM_AES_SIGNING_STATE primitive, cast to (DM_UPRIM_T*)
*/
void sm_handle_data_sign_req(DM_UPRIM_T *const uprim)
{
    SM_AES_SIGNING_STATE_T *sign = zpnew(SM_AES_SIGNING_STATE_T);

    /* Store copy of primitive. */
    sign->prim = uprim->dm_sm_data_sign_common;

    /* Update primitive and retrieve the unsigning key. */
    if (!sm_data_sign_stage1(&sign->prim, sign->csrk))
    {
        CsrPmemFree(sign);
        return;
    }

    if (sign->prim.status != HCI_SUCCESS)
    {
        sm_aes_send_data_sign_cfm(sign);
        return;
    }

#ifndef DISABLE_SM_KEY_GENERATION
    if (!sign->prim.verify)
    {
        /* If we're signing, rather than unsigning, then
           we need to recover the signing key from ER. */
        sign->csrk[1] = SM_ER_TO_CSRK;
        sm_aes_encrypt_128_offchip(sign->csrk,
                                   smcb.sm_key_state.er,
                                   NULL,
                                   sign,
                                   SM_AES_SIGN_GENERATE_CSRK);
    }
    else
#endif /* DISABLE_SM_KEY_GENERATION */
        sm_aes_data_sign_make_k_part1(sign);
}

/*! \brief State structure for resolvable private address resolution. */
typedef struct
{
    TYPED_BD_ADDR_T permanent_addrt; /* Permanent address associated with IRK. */
    TYPED_BD_ADDR_T current_addrt;   /* Address to resolve. */
    CsrUint8 identifier;              /* Opaque identifier. */
} SM_AES_PRIVATE_ADDRESS_RESOLVE_CONTEXT_T;

/*! \brief Try to resolve a private address with given IRK.
    \param irk Pointer to IRK to try.
    \param current_addrt Address to resolve.
    \param permanent_addrt Address associated with IRK - effectively opaque context.
    \param identifier More opaque context.
*/
void sm_aes_check_hash(const CsrUint16 *irk,
                       const TYPED_BD_ADDR_T *current_addrt,
                       const TYPED_BD_ADDR_T *permanent_addrt,
                       CsrUint8 identifier)
{
    CsrUint16 data[SM_SIZE16_128];
    SM_AES_PRIVATE_ADDRESS_RESOLVE_CONTEXT_T *context
            = pnew(SM_AES_PRIVATE_ADDRESS_RESOLVE_CONTEXT_T);

    /* Store context */
    tbdaddr_copy(&context->permanent_addrt, permanent_addrt);
    tbdaddr_copy(&context->current_addrt, current_addrt);
    context->identifier = identifier;

    /* Prepare 'plain text' for AES routine - UAP and NAP. */
    data[0] = TBDADDR_UAP(*current_addrt) | ((TBDADDR_NAP(*current_addrt) & 0xFF) << 8);
    data[1] = (TBDADDR_NAP(*current_addrt) >> 8) & 0xFF;
    CsrMemSet(&data[2], 0, sizeof(data) - 2);

    sm_aes_encrypt_128_offchip(data,
                               irk,
                               NULL,
                               context,
                               SM_AES_PRIVATE_ADDRESS_RESOLVE);
}

/*! \brief Callback for Confirm generation part 1.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_CONFIRM_GENERATE_PART1(SM_AES_CALCULATION_QUEUE_T *state,
                                          CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL)
    {
        sm_aes_confirm_start_part2(state,
                                   result,
                                   SM_AES_CONFIRM_GENERATE_PART2);
    }
    else
        sm_drop_link(state->pairing);
}


/*! \brief Callback for Confirm generation part 2.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_CONFIRM_GENERATE_PART2(SM_AES_CALCULATION_QUEUE_T *state,
                                          CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL)
        sm_l2cap_pairing_confirm(state->pairing, result);
    else
        sm_drop_link(state->pairing);
}


/*! \brief Callback for Confirm checking part 1.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_CONFIRM_CHECK_PART1(SM_AES_CALCULATION_QUEUE_T *state,
                                       CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL)
        sm_aes_confirm_start_part2(state, result, SM_AES_CONFIRM_CHECK_PART2);
    else
        sm_pairing_failed(state->pairing, SM_ERROR_CONFIRM_VALUE_FAILED);
}


/*! \brief Callback for Confirm checking part 2.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_CONFIRM_CHECK_PART2(SM_AES_CALCULATION_QUEUE_T *state,
                                       CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL && sm_confirm_value_matches_stored(state->pairing,
                                                          result))
    {
        sm_confirm_good(state->pairing, state->data);
    }
    else
    {
        sm_pairing_failed(state->pairing, SM_ERROR_CONFIRM_VALUE_FAILED);
    }
}

/*! \brief Callback for signing key generation.
           Now that we've generated the signing key, we proceed
           to start signing the data.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_SIGN_GENERATE_CSRK(SM_AES_CALCULATION_QUEUE_T *state,
                                      CsrUint16 *result)
{
    SM_AES_SIGNING_STATE_T *sign = (SM_AES_SIGNING_STATE_T*)state->data;

    if (result != NULL && sign != NULL)
    {
        state->data = NULL;

        CsrMemCpy(sign->csrk, result, sizeof(sign->csrk));
        sm_aes_data_sign_make_k_part1(sign);
    }
    /* TODO deal with failure */
}

/*! \brief Signing iteration function. This gets called for each
           block of 128bits of data to be signed.
    \param sign Pointer to signing state.
*/
static void sm_aes_data_sign_iterate(SM_AES_SIGNING_STATE_T *sign)
{
    if (sign->loop_index < sign->prim.length)
    {
        /* Still signing */
        sm_sign_block(&sign->prim,
                      sign->loop_index,
                      sign->K,
                      sign->C,
                      sign->csrk);

        sign->loop_index += 0x10;

        sm_aes_encrypt_128_offchip(sign->C,
                                   sign->csrk,
                                   NULL,
                                   sign,
                                   SM_AES_SIGN_ITERATE);
    }
    else
    {
        /* Finished */
        sm_data_sign_check(&sign->prim, sign->K, sign->C);
        sm_aes_send_data_sign_cfm(sign);
    }
}

/*! \brief Callback for K generation. Part of data signing process.
           Once we've generated K, we start building up the signature
           iteratively by looping across the data to be signed.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_SIGN_GENERATE_K_PART1(SM_AES_CALCULATION_QUEUE_T *state,
                                         CsrUint16 *result)
{
    SM_AES_SIGNING_STATE_T *sign = (SM_AES_SIGNING_STATE_T*)state->data;

    if (result != NULL && sign != NULL)
    {
        /* Don't want to free K. */
        state->data = NULL;

        CsrMemCpy(sign->K, result, sizeof(sign->K));

        sm_aes_data_sign_iterate(sign);
    }
}

/*! \brief Callback each iteration of signature generation.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_SIGN_ITERATE(SM_AES_CALCULATION_QUEUE_T *state,
                                CsrUint16 *result)
{
    SM_AES_SIGNING_STATE_T *sign = (SM_AES_SIGNING_STATE_T*)state->data;

    if (result != NULL && sign != NULL)
    {
        /* Don't want to free C. */
        state->data = NULL;

        CsrMemCpy(sign->C, result, sizeof(sign->C));

        sm_aes_data_sign_iterate(sign);
    }
}

/*! \brief Callback for STK generation.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_STK_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL)
    {
        if (!SM_IS_MASTER(state->pairing))
            sm_l2cap_pairing_rand(state->pairing);

        CsrMemCpy(SM_STK(state->pairing), result, SM_SIZE16_128 * sizeof(CsrUint16));
        sm_encrypt_using_stk(state->pairing);
    }
    else
    {
        sm_pairing_failed(state->pairing, SM_ERROR_UNSPECIFIED_REASON);
    }
}

/*! \brief Callback for resolvable private address generation.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_PRIVATE_ADDRESS_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    CsrUint8 *rand = NULL;

    if (result != NULL)
    {
        rand = (CsrUint8*)state->data;
        CsrMemCpy(rand, result, 3);
    }

    sm_privacy_resolvable_random_address(rand);
}

/*! \brief Callback for resolvable private address resolution.
           We check the address to work out if it correctly resolved.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_PRIVATE_ADDRESS_RESOLVE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    CsrBool success = FALSE;
    SM_AES_PRIVATE_ADDRESS_RESOLVE_CONTEXT_T *context =
            (SM_AES_PRIVATE_ADDRESS_RESOLVE_CONTEXT_T*)state->data;

    if (result != NULL)
    {
        /* Check address resolution. */
        TYPED_BD_ADDR_T reconstructed_addrt;
        tbdaddr_copy(&reconstructed_addrt, &context->current_addrt);

        TBDADDR_LAP(reconstructed_addrt) =
                ((result[1] >> 8) & 0xFF) | (((CsrUint24)result[2]) << 8);

        success = tbdaddr_eq(&reconstructed_addrt, &context->current_addrt);
    }

    /* Report back results. */
    sm_privacy_aes_hci(&context->current_addrt,
                       &context->permanent_addrt,
                       context->identifier,
                       success);
}

#ifndef DISABLE_SM_KEY_GENERATION
/*! \brief Callback for LTK generation for distribution to peer.
           We apply encryption key size limit and then send to peer.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_KEY_DIST_LTK_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL)
    {
        sm_reduce_enc_key_size(result,
                               SM_ENCRYPTION_KEY_SIZE(state->pairing));
        sm_l2cap_send_key(state->pairing,
                          SM_EVENT_ENCRYPTION_INFORMATION,
                          result);
    }
    else
    {
        sm_pairing_failed(state->pairing, SM_ERROR_UNSPECIFIED_REASON);
    }

}

/*! \brief Callback for DHK generation, for enc-key distribution to peer.
           We use the DHK to encrypt a random number, the result of which
           will be used to turn div into ediv.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_KEY_DIST_DHK_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_PAIRING_T *pairing = state->pairing;
    SM_SANITY_CHECK(pairing);

    if (result != NULL && state->data != NULL)
    {
        CsrUint16 *rand = (CsrUint16*)CsrPmemZalloc((1 + SM_SIZE16_128)*sizeof(CsrUint16));

        /* Generate and store random number */
        RANDOM_NUMBER(pairing, rand + 1, SM_SIZE16_64);
        sm_encode_key_size_and_mitm(
                rand + 1,
                (CsrBool)(SM_PAIRING_ASSOC_MODEL(pairing) > SM_ASSOC_MODEL_JUST_WORKS),
                SM_ENCRYPTION_KEY_SIZE(pairing));

        /* Store div */
        rand[0] = *(CsrUint16*)state->data;
        CsrPmemFree(state->data);
        state->data = NULL;

        /* Encrypt random number, and pass through it and div as context. */
        sm_aes_encrypt_128_offchip(rand + 1,
                                   result,
                                   pairing,
                                   rand,
                                   SM_AES_KEY_DIST_EDIV_GENERATE);
    }
    else
    {
        sm_pairing_failed(state->pairing, SM_ERROR_UNSPECIFIED_REASON);
    }
}

/*! \brief Callback for EDIV generation, for enc-key distribution to peer.
           We use the result to hide the diversifier and send that and the
           original RAND to the peer.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_KEY_DIST_EDIV_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL && state->data != NULL)
    {
        /* Use result to encrypt diversifier */
        *(CsrUint16*)state->data ^= result[0];

        /* Send ediv and rand to peer. */
        sm_l2cap_send_key(state->pairing,
                          SM_EVENT_MASTER_IDENTIFICATION,
                          (CsrUint16*)state->data);

        SM_KEY_DISTRIBUTION(state->pairing) |= (1<<SM_KEY_SEND_TO_HOST);
        SM_KEY_DISTRIBUTION(state->pairing) &= ~(1 << SM_KEY_GENERATING);
        sm_send_keys_to_peer(state->pairing);
    }
    else
    {
        sm_pairing_failed(state->pairing, SM_ERROR_UNSPECIFIED_REASON);
    }
}

/*! \brief Callback for IRK generation.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_KEY_DIST_IRK_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    if (state->pairing != NULL)
    {
        /* Key distribution */
        SM_SANITY_CHECK(state->pairing);

        if (result != NULL)
        {
            sm_l2cap_send_key(state->pairing, SM_EVENT_IDENTITY_INFORMATION, result);
            sm_l2cap_send_identity_address(state->pairing);
#ifndef SM_HAS_FUNCTION_FOR_AES
        SM_KEY_DISTRIBUTION(state->pairing) &= ~(1 << SM_KEY_GENERATING);
        sm_send_keys_to_peer(state->pairing);
#endif /* SM_HAS_FUNCTION_FOR_AES */
        }
        else
        {
            sm_pairing_failed(state->pairing, SM_ERROR_UNSPECIFIED_REASON);
        }
    }
    else
    {
        /* Resolvable private address generation */
        if (result != NULL)
        {
            CsrUint16 data[SM_SIZE16_128];
            CsrUint8 *rand = state->data;

            state->data = NULL;

            data[0] = rand[3] | ((CsrUint16)rand[4] << 8);
            data[1] = rand[5];
            CsrMemSet(&data[2], 0, sizeof(data) - 2);

            sm_aes_encrypt_128_offchip(data,
                                       result,
                                       NULL,
                                       rand,
                                       SM_AES_PRIVATE_ADDRESS_GENERATE);
        }
    }
}

/*! \brief Callback for CSRK generation.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_KEY_DIST_CSRK_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_SANITY_CHECK(state->pairing);

    if (result != NULL)
    {
        sm_l2cap_send_key(state->pairing, SM_EVENT_SIGNING_INFORMATION, result);

        SM_KEY_DISTRIBUTION(state->pairing) |= (1<<SM_KEY_SEND_TO_HOST);
        SM_KEY_DISTRIBUTION(state->pairing) &= ~(1 << SM_KEY_GENERATING);
        sm_send_keys_to_peer(state->pairing);
    }
    else
    {
        sm_pairing_failed(state->pairing, SM_ERROR_UNSPECIFIED_REASON);
    }

}

/*! \brief Callback for DHK generation to be used to retrieve the
           diversifier from ediv and eventually recreate the LTK.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_LTK_DHK_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_LTK_RECREATE_STATE_T *ltk_recreate = (SM_LTK_RECREATE_STATE_T*)state->data;

    if (ltk_recreate != NULL)
    {
        if (result != NULL)
        {
            /* Use DHK to encrypt RAND. */
            state->data = NULL;
            sm_aes_encrypt_128_offchip(ltk_recreate->rand,
                                       result,
                                       NULL,
                                       ltk_recreate,
                                       SM_AES_LTK_RECREATE_DIV_UNHIDE);
        }
        else
        {
            sm_send_hci_long_term_key_requested_neg_reply(ltk_recreate->handle);
        }
    }
}

/*! \brief Callback from RAND encryption by DHK. Used to retrieve div
           from ediv and recreate the LTK.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_LTK_RECREATE_DIV_UNHIDE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_LTK_RECREATE_STATE_T *ltk_recreate = (SM_LTK_RECREATE_STATE_T*)state->data;

    if (ltk_recreate != NULL)
    {
        DM_ACL_T *acl;
        if (result != NULL
                && (acl = dm_acl_find_by_handle(ltk_recreate->handle)) != NULL)
        {
            /* Unhide diversifier and store for later. */
            smcb.ltk_regeneration.div = ltk_recreate->ediv ^ result[0];

            /* Send a key req IND to application. */
            sm_send_key_request(
                    &acl->addrt,
                    (CsrUint16) (smcb.ltk_regeneration.mitm ? DM_SM_SECURITY_ENCRYPTION |
                                                 DM_SM_SECURITY_BONDING |
                                                 DM_SM_SECURITY_MITM_PROTECTION
                                               : DM_SM_SECURITY_ENCRYPTION |
                                                 DM_SM_SECURITY_BONDING),
                    DM_SM_KEY_DIV,
                    smcb.ltk_regeneration.div);
        }
        else
            sm_send_hci_long_term_key_requested_neg_reply(ltk_recreate->handle);
    }
}

/* State structure for second part of LTK recreation. */
typedef struct
{
    hci_connection_handle_t handle;
    CsrUint8 enc_key_size;
} SM_LTK_RECREATE_STATE2_T;

/*! \brief Second part of LTK recreation.
           Application has responded to DIV key request.
           Now we use the DIV key to recreate LTK.
    \param handle HCI connection handle for this link.
*/
void sm_aes_recreate_ltk_from_div(hci_connection_handle_t handle)
{
    SM_LTK_RECREATE_STATE2_T *ltk_state = pnew(SM_LTK_RECREATE_STATE2_T);
    CsrUint16 plain_text[SM_SIZE16_128];

    CsrMemSet(plain_text, 0, SM_SIZE16_128 * sizeof(CsrUint16));

    plain_text[0] = smcb.ltk_regeneration.div;
    plain_text[1] = SM_ER_TO_LTK;

    ltk_state->handle = handle;
    ltk_state->enc_key_size = smcb.ltk_regeneration.enc_key_size;

    /* Diversify ER to recreate LTK. */
    sm_aes_encrypt_128_offchip(plain_text,
                               smcb.sm_key_state.er,
                               NULL,
                               ltk_state,
                               SM_AES_LTK_RECREATE_LTK_GENERATE);
}

/*! \brief Callback from LTK recreation.
    \param state AES calculation state.
    \param result Pointer to result of AES encryption.
*/
static void sm_aes_LTK_RECREATE_LTK_GENERATE(SM_AES_CALCULATION_QUEUE_T *state, CsrUint16 *result)
{
    SM_LTK_RECREATE_STATE2_T *ltk_state = (SM_LTK_RECREATE_STATE2_T*)state->data;

    if (ltk_state != NULL)
    {
        if (result != NULL)
        {
            sm_reduce_enc_key_size(result, ltk_state->enc_key_size);
            sm_send_hci_long_term_key_requested_reply(ltk_state->handle, result);
        }
        else
        {
            sm_send_hci_long_term_key_requested_neg_reply(ltk_state->handle);
        }
    }
}
#endif /* DISABLE_SM_KEY_GENERATION */

/*! \brief Synergy deinit routine. */
void sm_aes_deinit(void)
{
    while (smcb.aes_calculation != NULL)
    {
        /* Remove element from head of queue. */
        SM_AES_CALCULATION_QUEUE_T *elp = smcb.aes_calculation;
        smcb.aes_calculation = elp->next;

        CsrPmemFree(elp->data);
        CsrPmemFree(elp);
    }
}


#endif /* SM_HAS_FUNCTION_FOR_AES */
#endif /* INSTALL_SM_MODULE */
