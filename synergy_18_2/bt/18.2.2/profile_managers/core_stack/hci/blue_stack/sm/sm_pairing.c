/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_pairing.c

\brief  Pairing and key-generation functions.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE

/* Offset in octets of SM_EVENT_PAIRING_REQUEST in P1 */
#define P1_OFFSET8_PAIRING_REQ 2

/* Offset in octets of SM_EVENT_PAIRING_RESPONSE in P1 */
#define P1_OFFSET8_PAIRING_RSP \
        (P1_OFFSET8_PAIRING_REQ + SM_EVENT_PAIRING_REQUEST_LENGTH)

#ifdef SM_DEBUG_CRYPTO_CALCULATIONS
/*! \brief Abuse DM_SM_LINK_KEY_IND to return debugging information.
    \param key 128bit key.
    \param type Key type.
*/
void sm_aes_debug_key(const CsrUint16 *const key, const CsrUint8 type)
{
    DM_SM_LINK_KEY_IND_T *prim = zpnew(DM_SM_LINK_KEY_IND_T);
    prim->type = DM_SM_LINK_KEY_IND;
    prim->key_type = type;
    memcpy_unpack128(prim->key, key);
    DM_SendMessageAMphandle(prim);
}
#endif

/*! \brief Pack Pairing Request/Response signal.
    \param out Array where packed signal gets stored.
    \param in Array from which unpacked signal is read.
*/
void sm_aes_pack_pairing(CsrUint16 *out, const CsrUint8 *in)
{
    CsrUint8 temp[SM_SIZE8_128];
    CsrUint8 *offset = temp + (in[0] == SM_EVENT_PAIRING_REQUEST ?
            P1_OFFSET8_PAIRING_REQ : P1_OFFSET8_PAIRING_RSP);

    memcpy_unpack128(temp, out);
    /* Pairing request and response are same length. */
    CsrMemCpy(offset, in, SM_EVENT_PAIRING_REQUEST_LENGTH);
    memcpy_pack128(out, temp);
}

/*! \brief Calculate XOR of two 128bit values.
    \param out Output.
    \param a Input 1.
    \param b Input 2.
*/
void sm_xor_128(CsrUint16 *out, const CsrUint16 *a, const CsrUint16 *b)
{
    CsrUint8 i;

    for (i = 0; i != SM_SIZE16_128; ++i)
        out[i] = a[i] ^ b[i];
}

/*! \brief XOR packed array in-place using BD_ADDR.
    \param inout Packed array.
    \param addrt Pointer to address.
*/
static void sm_xor_bd_addr(CsrUint16 *inout, const TYPED_BD_ADDR_T *addrt)
{
    inout[2] ^= TBDADDR_NAP(*addrt);
    inout[1] ^= (TBDADDR_UAP(*addrt) << 8)
            | (CsrUint16)(TBDADDR_LAP(*addrt) >> 16);
    inout[0] ^= (CsrUint16)(TBDADDR_LAP(*addrt) & 0xFFFF);
}

/*! \brief. As part of confirm generation, XOR RAND with P1.
            We also fill in the address type part of RAND.
    \param pairing Pointer to pairing structure.
    \param rand_xor_p1 Result of XOR is written here.
    \param rand RAND as input.
*/
SM_STATIC void sm_aes_generate_confirm_rand_xor_p1(SM_PAIRING_T *pairing,
                                                   CsrUint16 *rand_xor_p1,
                                                   const CsrUint16 *rand)
{
    SM_SANITY_CHECK(pairing);

    SM_AES_DEBUG_KEY(rand, 1);

    /* Set address type in P1. */
    SM_P1(pairing)[0] = SM_IS_MASTER(pairing)
        ? (SM_TBDADDR_TYPE_REMOTE(pairing) << 8) | SM_TBDADDR_TYPE_LOCAL()
        : (SM_TBDADDR_TYPE_LOCAL() << 8) | SM_TBDADDR_TYPE_REMOTE(pairing);

    sm_xor_128(rand_xor_p1, rand, SM_P1(pairing));

    SM_AES_DEBUG_KEY(rand_xor_p1, 2);
}

/*! \brief As part of confirm generation, XOR what we've got so far with P2.
    \param pairing Pointer to pairing structure.
    \param confirm Result of XOR is written here.
*/
SM_STATIC void sm_aes_generate_confirm_xor_p2(SM_PAIRING_T *pairing,
                                              CsrUint16 *confirm)
{
    SM_SANITY_CHECK(pairing);

    SM_AES_DEBUG_KEY(confirm, 3);

    /* TODO random local address */
    if (SM_IS_MASTER(pairing))
    {
        sm_xor_bd_addr(confirm + P2_OFFSET16_BD_ADDR_INITIATOR,
                       SM_TBDADDR_LOCAL());
        sm_xor_bd_addr(confirm + P2_OFFSET16_BD_ADDR_RESPONDER,
                       SM_TBDADDR_REMOTE(pairing));
    }
    else
    {
        sm_xor_bd_addr(confirm + P2_OFFSET16_BD_ADDR_INITIATOR,
                       SM_TBDADDR_REMOTE(pairing));
        sm_xor_bd_addr(confirm + P2_OFFSET16_BD_ADDR_RESPONDER,
                       SM_TBDADDR_LOCAL());
    }

    SM_AES_DEBUG_KEY(confirm, 4);
}

/*! \brief Check regenerated confirm value with the one stored.
    \param pairing Pointer to pairing structure.
    \param confirm Regenerated confirm value.
    \returns TRUE if the confirms match, otherwise FALSE.
*/
SM_STATIC CsrBool sm_confirm_value_matches_stored(SM_PAIRING_T *pairing,
                                                 CsrUint16 *confirm)
{
    CsrUint16 *c1;
    CsrUint16 *c2;

    SM_SANITY_CHECK(pairing);

    /* Check against stored value. */
    c1 = SM_CONF(pairing) + SM_SIZE16_128;
    c2 = confirm + SM_SIZE16_128;
    while (c2 != confirm)
        if (*--c1 != *--c2)
            return FALSE;

    return TRUE;
}

#ifdef SM_HAS_FUNCTION_FOR_AES
/*! \brief Wrapper around aes_encrypt() that will take packed input
           and output.
    \param packed_out Where the output gets written.
    \param packed_in Plain text input.
    \param packed_key AES key.
*/
void sm_aes_encrypt_128(CsrUint16 *packed_out,
                        const CsrUint16 *packed_in,
                        const CsrUint16 *packed_key)
{
    CsrUint8 unpacked_data[SM_SIZE8_128];
    CsrUint8 unpacked_key[SM_SIZE8_128];

    memcpy_unpack128(unpacked_key, packed_key);
    memcpy_unpack128(unpacked_data, packed_in);
    aes_encrypt(unpacked_data, unpacked_key, unpacked_data);
    memcpy_pack128(packed_out, unpacked_data);
}

/*! \brief Generate confirm value.

    C1 = e(TK, e(TK, RAND ^ P1) ^ P2)

    \param pairing Pointer to pairing structure.
    \param confirm Where we write the confirm value.
    \param rand rand value used to generate the confirm.
*/
static void sm_aes_generate_confirm(SM_PAIRING_T *pairing,
                                    CsrUint16 *confirm,
                                    const CsrUint16 *rand)
{
    SM_SANITY_CHECK(pairing);

    sm_aes_generate_confirm_rand_xor_p1(pairing, confirm, rand);
    sm_aes_encrypt_128(confirm, confirm, SM_TK(pairing));
    sm_aes_generate_confirm_xor_p2(pairing, confirm);
    sm_aes_encrypt_128(confirm, confirm, SM_TK(pairing));
    SM_AES_DEBUG_KEY(confirm, 5);
}

/*! \brief Generate rand and matching confirm.
    Stores rand and sends confirm over the air.

    \param pairing Pointer to pairing structure.
*/
void sm_aes_generate_rand_confirm(SM_PAIRING_T *pairing)
{
    CsrUint16 packed_confirm[SM_SIZE16_128];

    SM_SANITY_CHECK(pairing);

    RANDOM_NUMBER(pairing, SM_RAND(pairing), SM_SIZE16_128);
    sm_aes_generate_confirm(pairing, packed_confirm, SM_RAND(pairing));
    sm_l2cap_pairing_confirm(pairing, packed_confirm);
}

/*! \brief Re-generate confirm and check match with stored confirm.

    \param pairing Pointer to pairing structure.
    \param rand rand value used to generate the confirm.
    \returns TRUE if re-generated confirm matches, otherwise FALSE.
*/
CsrBool sm_aes_check_rand_confirm(SM_PAIRING_T *pairing, const CsrUint16 *rand)
{
    CsrUint16 confirm[SM_SIZE16_128];

    SM_SANITY_CHECK(pairing);

    SM_AES_DEBUG_KEY(SM_RAND(pairing),14);

    /* Reconstruct peer's confirm from rand. */
    sm_aes_generate_confirm(pairing, confirm, rand);

    SM_AES_DEBUG_KEY(SM_RAND(pairing),15);

    /* Check match */
    return sm_confirm_value_matches_stored(pairing, confirm);
}
#endif /* SM_HAS_FUNCTION_FOR_AES */

/*! \brief Wrapper around memcpy_unpack for 128bit values.
    \param dst Where we write the output.
    \param src Where we read the input.
*/
void memcpy_unpack128(CsrUint8 *dst, const CsrUint16 *src)
{
    CsrBtMemCpyUnpack(dst, src, SM_SIZE16_128);
}

/*! \brief Wrapper around memcpy_pack for 128bit values.
    \param dst Where we write the output.
    \param src Where we read the input.
*/
void memcpy_pack128(CsrUint16 *dst, const CsrUint8 *src)
{
    CsrBtMemCpyPack(dst, src, SM_SIZE16_128);
}

#ifndef DISABLE_SM_KEY_GENERATION
#ifdef SM_HAS_FUNCTION_FOR_AES
/*! \brief Function to generate IRK and DHK from IR.
    \param key_out Where we write the generated key.
    \param div key type to generate.
*/
void sm_ir_diversifier(CsrUint16 *key_out, SM_IR_DIVERSIFIER_T div)
{
    CsrMemSet(key_out, 0, SM_SIZE16_128 * sizeof(CsrUint16));
    key_out[0] = (CsrUint16)div;
    sm_aes_encrypt_128(key_out, key_out, smcb.sm_key_state.ir);
}

/*! \brief Function to generate LTK and CSRK from ER.
    \param key_out Where we write the generated key.
    \param div Diversifier used to generate key.
    \param r key type to generate.
*/
void sm_er_diversifier(CsrUint16 *key_out,
                       CsrUint16 div,
                       SM_ER_DIVERSIFIER_T r)
{
    CsrMemSet(key_out, 0, SM_SIZE16_128 * sizeof(CsrUint16));
    key_out[0] = div;
    key_out[1] = (CsrUint16)r;
    sm_aes_encrypt_128(key_out, key_out, smcb.sm_key_state.er);
}

/*! \brief Hide/unhide diversifier using rand and DHK.
    \param rand rand value used for (un)hiding.
    \param value div/ediv to (un)hide.
    \return result of (un)hiding value.
*/
CsrUint16 sm_div_toggle_mask(const CsrUint16 *rand, CsrUint16 value)
{
    CsrUint16 dhk[SM_SIZE16_128];
    CsrUint16 r[SM_SIZE16_128];

    SM_DHK(dhk);

    /* Copy and pad rand. */
    CsrMemCpy(r, rand, SM_SIZE16_64 * sizeof(CsrUint16));
    CsrMemSet(r + SM_SIZE16_64, 0, SM_SIZE16_64 * sizeof(CsrUint16));

    sm_aes_encrypt_128(r, r, dhk);

    /* (un)hide */
    return value ^ r[0];
}
#endif
#endif /* DISABLE_SM_KEY_GENERATION */

#endif /* INSTALL_SM_MODULE */
