/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_data_signing.c

\brief  Security Manager Protocol routines to sign/resolve unencrypted data.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE

/* Magic number defined in signing specification. */
#define Rb 0x87

/*! \brief Shift a 128bit CsrUint16 array by 1
    \param data Pointer to 128bit CsrUint16 array
    \returns TRUE if a set bit fell off the top, otherwise FALSE.
*/
static CsrBool bit_shift_left(CsrUint16 *data)
{
    CsrUint16 overflow = 0;
    CsrUint16 *end;

    for (end = data + SM_SIZE16_128; data != end; ++data)
    {
        CsrUint16 temp;

        /* Left shift with previous overflow */
        temp = ((*data << 1) + overflow) & 0xFFFF;

        /* Overflow is the bit that falls off the end */
        overflow = (*data & 0xFFFF) >> 15;

        *data = temp;
    }

    return (CsrBool)overflow;
}

/*! \brief make K1 or K2 - magic values used on last block in signing process
           and prepare last block and EOR K1 or K2 with that block, as appropriate.
    \param out This function will write the padded last block here.
    \param K This function will write K1 or K2 here.
    \param length_last_block Length of the last block of data to be signed.
    \param last_block Pointer to last block of data.
    \param key Pointer to signing key.
*/
static void sm_make_and_apply_K(
        CsrUint16 *out,
        CsrUint16 *K,
        CsrUint16 length_last_block,
        const CsrUint8 *last_block,
        const CsrUint16 *key)
{
    /* Create K1 and store in K */
    if (bit_shift_left(K))
        K[0] ^= Rb;

    if (length_last_block == 0x10)
    {
        /* Full block so don't need K2 */

        /* Pack last block */
        memcpy_pack128(out, last_block);
    }
    else
    {
        CsrUint16 length_last_block16;

        /* Turn K1 into K2 and store in K */
        if (bit_shift_left(K))
            K[0] ^= Rb;

        /* Pack and pad last block */
        CsrMemSet(out, 0, SM_SIZE16_128 * sizeof(CsrUint16));
        length_last_block16 = length_last_block/2;
        CsrBtMemCpyPack(&out[0x8 - length_last_block16],
                    &last_block[length_last_block & 1],
                    length_last_block16);

        /* Set the top bit of the first octet of padding */
        out[0x7 - length_last_block16] = (length_last_block & 1) == 0
            ? 0x8000 : (((CsrUint16)last_block[0]) << 8) | 0x80;
    }

    /* EOR final block with K1 or K2, as appropraite. */
    sm_xor_128(out, K, out);
}

/*! \brief Data signing iteration function - update signature for next block.
    This function is called for each 128bit block of data to be signed. It
    usually just EORs the block with the state of the signature so far. But
    when we reach the final block, we generate and EOR with the magic K value
    instead.
    \param prim Pointer to signature request primitive.
    \param i Index to position in data that we're signing.
    \param K Pointer to memory where we can store K.
    \param C Pointer to signature that we're iterating.
    \param key Pointer to signing key.
*/
SM_STATIC void sm_sign_block(DM_SM_DATA_SIGN_COMMON_T *prim,
                             CsrUint16 i,
                             CsrUint16 *K,
                             CsrUint16 *C,
                             const CsrUint16 *key)
{
    CsrUint16 data[SM_SIZE16_128];
    const CsrUint8 *read_ptr = &prim->data[i];
    CsrUint16 length_remaining = prim->length - i;

    if (length_remaining > 0x10)
    {
        /* Not the final block */
        memcpy_pack128(data, read_ptr);
    }
    else
    {
        /* The final block */
        sm_make_and_apply_K(data,
                            K,
                            length_remaining,
                            read_ptr,
                            key);
    }

    sm_xor_128(C, data, C);
}

#ifndef DISABLE_SM_KEY_GENERATION
/*! \brief Write the sign counter to the message and
           notify the application that it has been changed.
    \param buf Pointer to place where we should write the counter.
*/
static void sm_write_sign_counter(CsrUint8 *buf)
{
    ++smcb.sign_counter;

    write_uint32(&buf, &smcb.sign_counter);

    /* TODO wait for reply before signing data? */
    sm_send_csrk_counter_change_ind(NULL, smcb.sign_counter);
}
#endif /* DISABLE_SM_KEY_GENERATION */


static void sm_add_to_signature_queue(DM_SM_DATA_SIGN_COMMON_T *prim)
{
    SM_SIGNATURE_QUEUE_T *elp, **elpp;

    /* Find end of queue */
    for (elpp = &smcb.signature_queue;
            (elp = *elpp) != NULL;
            elpp = &elp->next)
        /* empty */;

    /* Add new element. */
    *elpp = elp = zpnew(SM_SIGNATURE_QUEUE_T);
    elp->prim = pcopy(prim, sizeof(DM_SM_DATA_SIGN_COMMON_T));
    return;
}



/*! \brief Start of the signing process.
    \param prim Pointer to request primitive.
    \param key Pointer to memory to be used to store the signing key.
    \returns TRUE if the signing key is found and we can proceed,
             or FALSE if we must wait for the application to respond
             and give us the signing key.
*/
SM_STATIC CsrBool sm_data_sign_stage1(DM_SM_DATA_SIGN_COMMON_T *prim,
                                     CsrUint16 *key)
{
    prim->type = DM_SM_DATA_SIGN_CFM;
    prim->status = HCI_SUCCESS;

    if (prim->verify)
    {
        /* Verifying a signature, so need peer's CSRK */
        if (!dm_sm_get_csrk(key, &prim->addrt))
        {
            /* Must ask application for signature. */
            sm_add_to_signature_queue(prim);

            /* Request key from application. */
            sm_send_key_request(&prim->addrt, 0, DM_SM_KEY_SIGN, 0);
            return FALSE;
        }
    }
#ifndef DISABLE_SM_KEY_GENERATION
    else
    {
        CsrUint16 div;

        /* Signing, so need our CSRK */
        if ((div = dm_sm_get_diversifier(&prim->addrt)) == 0)
        {
            /* Get DIV from application */
            sm_add_to_signature_queue(prim);

            /* Request div from application. */
            sm_send_csrk_div_request(&prim->addrt);
            return FALSE;
        }
        else
        {
#ifdef SM_HAS_FUNCTION_FOR_AES
            SM_CSRK(key, div);
#else
            key[0] = div;
#endif
            sm_write_sign_counter(prim->data + prim->length);
        }
    }
#endif /* DISABLE_SM_KEY_GENERATION */
    prim->length += SM_SIZE8_SIGN_COUNTER;

    return TRUE;
}

/*! \brief End of the signing process. We either check or apply the signature.
    \param prim Pointer to request primitive.
    \param K Pointer to 128bit of scratch space.
    \param C Pointer to 128bit signature.
*/
SM_STATIC void sm_data_sign_check(DM_SM_DATA_SIGN_COMMON_T *prim,
                                  CsrUint16 *K,
                                  CsrUint16 *C)
{
    CsrUint8 *signature = prim->data + prim->length;

    if (prim->verify)
    {
        /* Copy packet signature into K and compare with ours in C. */
        CsrBtMemCpyPack(K, signature, SM_SIZE16_SIGNATURE);
        if (CsrMemCmp(K, C+SM_SIZE16_SIGNATURE, SM_SIZE16_SIGNATURE * sizeof(CsrUint16)) != 0)
            prim->status = HCI_ERROR_AUTH_FAIL;

            /* TODO check sign counter. */
    }
    else
    {
        /* Write our signature to the packet. */
        CsrBtMemCpyUnpack(signature,
                      C + SM_SIZE16_SIGNATURE,
                      SM_SIZE16_SIGNATURE);
    }
}

#ifdef SM_HAS_FUNCTION_FOR_AES
/*! \brief Handle signing request from application.
    \param uprim Pointer to signing primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_data_sign_req(DM_UPRIM_T *const uprim)
{
    CsrUint16 key[SM_SIZE16_128];

    if (!sm_data_sign_stage1(&uprim->dm_sm_data_sign_common, key))
        return;

    if (uprim->dm_sm_data_sign_common.status == HCI_SUCCESS)
    {
        CsrUint16 K[SM_SIZE16_128];
        CsrUint16 C[SM_SIZE16_128];
        CsrUint16 i;

        CsrMemSet(K, 0, sizeof(K));
        CsrMemSet(C, 0, sizeof(C));

        sm_aes_encrypt_128(K, K, key);

        /* Loop over number of blocks */
        for (i = 0; i < uprim->dm_sm_data_sign_common.length; i += 0x10)
        {
            sm_sign_block(&uprim->dm_sm_data_sign_common, i, K, C, key);
            sm_aes_encrypt_128(C, C, key);
        }

        sm_data_sign_check(&uprim->dm_sm_data_sign_common, K, C);
    }

    DM_SendMessage(pcopy(uprim, sizeof(DM_SM_DATA_SIGN_COMMON_T)));
}
#endif

/*! \brief Retrieve data signing primitive from queue.
    \param addrt Typed Bluetooth address of peer.
    \param verify - key for verifying the signature or adding the signature
    \returns Pointer to primitive, or NULL if not found.
*/
static DM_SM_DATA_SIGN_COMMON_T *sm_data_sign_get_from_queue(TYPED_BD_ADDR_T *addrt,
                                                             CsrBool verify)
{
    SM_SIGNATURE_QUEUE_T *elp, **elpp;

    for (elpp = &smcb.signature_queue; (elp = *elpp) != NULL; elpp = &elp->next)
    {
        if ((tbdaddr_eq(addrt, &elp->prim->addrt)) &&
            (verify==elp->prim->verify))
        {
            DM_SM_DATA_SIGN_COMMON_T *prim = elp->prim;
            *elpp = elp->next;
            CsrPmemFree(elp);

            return prim;
        }
    }

    return NULL;
}

/*! \brief Application has responded to CSRK request and doesn't have one.
           Fail the unsigning request from ATT.
    \param addrt Typed Bluetooth address of peer.
    \param verify - key missing for verifying the signature or adding the signature
*/
void sm_data_unsign_key_missing(TYPED_BD_ADDR_T *addrt, CsrBool verify)
{
    /* Send failure message to caller. */
    DM_SM_DATA_SIGN_COMMON_T *prim = sm_data_sign_get_from_queue(addrt,
                                                                 verify);
    if(NULL != prim)
    {
        prim->type = DM_SM_DATA_SIGN_CFM;
        prim->status = HCI_ERROR_KEY_MISSING;
        DM_SendMessage(prim);
    }
}

/*! \brief Application has responded to CSRK request and has one.
    \param addrt Typed Bluetooth address of peer.
    \param verify - key returned for verifying the signature or adding the signature
*/
void sm_data_unsign_key_returned(TYPED_BD_ADDR_T *addrt, CsrBool verify)

{
    DM_SM_DATA_SIGN_COMMON_T *prim = sm_data_sign_get_from_queue(addrt,
                                                                  verify);

    if (prim != NULL)
    {
        /* Key will have been added to database, so try to sign again. */
        sm_handle_data_sign_req((DM_UPRIM_T*)prim);
        CsrPmemFree(prim);
    }
}

#ifdef ENABLE_SHUTDOWN
void sm_data_sigining_deinit(void)
{
    while (smcb.signature_queue != NULL)
    {
        SM_SIGNATURE_QUEUE_T *temp = smcb.signature_queue;
        smcb.signature_queue = temp->next;
        CsrPmemFree(temp->prim);
        CsrPmemFree(temp);
    }
}
#endif /* ENABLE_SHUTDOWN */
#endif /* INSTALL_SM_MODULE */
