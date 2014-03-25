/*!
   Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
     All rights reserved.

\file   sm.c

\brief  Security Manager
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE

SMCB_T smcb;

/* Array of event dependencies.

   To find the dependencies of an event, use the event as in index into this
   array. The value returned will be a bitmask of all the events that are
   required to have occured before the event in question. So if we receive
   an event while we're still expecting one of its dependencies then that is
   probably an error of some sort. */
static const CsrUint16 sm_event_dependencies[] =
{
#define SM_EVENT_TABLE_SEP ,
#define SM_EVENT_TABLE_X(name, length, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11) SM_DEPEND_ ## name
    SM_EVENT_TABLE_PDUS,
    SM_EVENT_TABLE_OTHER
#undef SM_EVENT_TABLE_SEP
#undef SM_EVENT_TABLE_X
};

/*! \brief Main Security Manager initialisation function. This gets called
           from the DM Layer Manager when the application selects BT4.0.
*/
void sm_init(void)
{
    L2CA_CONFIG_T config;
    HCI_READ_BD_ADDR_T *prim;

    /* Register with L2CAP */
    CsrMemSet(&config, 0, sizeof(L2CA_CONFIG_T));
    config.options = L2CA_SPECIFY_MTU;
    config.mtu = 23;

    L2CA_RegisterFixedCidReq(DM_IFACEQUEUE,
                             L2CA_CID_SECURITY_MANAGER,
                             &config,
                             0);

    /* We need to know our own Bluetooth Address for pairing. */
    prim = pnew(HCI_READ_BD_ADDR_T);
    prim->common.op_code = HCI_READ_BD_ADDR;
    prim->common.length = DM_HCI_SENDER_SECURITY_MANAGER;
    send_to_hci((DM_UPRIM_T*)prim);

    /* Initialise Key-generation. */
#ifndef DISABLE_SM_KEY_GENERATION
    smcb.div = 1;
    smcb.sign_counter = 1;
#ifdef SM_HAS_FUNCTION_FOR_RAND
    random_number(smcb.sm_key_state.ir, SM_SIZE16_128);
    random_number(smcb.sm_key_state.er, SM_SIZE16_128);
#else
    smcb.ir_er_rand_index = 4;
    sm_rand_generate(4);
#endif
#endif
    smcb.enc_key_size_min_threshold = SM_MIN_ENC_KEY_SIZE;
    smcb.enc_key_size_max_threshold = SM_MAX_ENC_KEY_SIZE;

    sm_privacy_init();
}

/*! \brief Either update the Security Manager's record of ER, IR and Div,
           or report them back. This is called from the DM Security Manager
           during initialisation and facilitates persistent keys.

    \param options DM Security Manager options. This determines whether we
           use the other paramters to take the values provided, or to report
           our values.
    \param sm_key_state Used to read or write values of IR and ER.
    \param sm_div_state Used to read or write values of div.
    \param sm_sign_counter Used to read or write the sign counter.
*/
CsrBool sm_update_ir_er_div(CsrUint16 options,
                           DM_SM_KEY_STATE_T *sm_key_state,
                           CsrUint16 *sm_div_state,
                           CsrUint32 *sm_sign_counter)
{
#ifndef DISABLE_SM_KEY_GENERATION
    if ((options & DM_SM_INIT_SM_STATE) != 0)
    {
        /* Overwrite key-generation parameters with values provided. */
        smcb.div = *sm_div_state;
        smcb.sign_counter = *sm_sign_counter;
        smcb.sm_key_state = *sm_key_state;

#ifndef SM_HAS_FUNCTION_FOR_RAND
        /* Throw away any random numbers in progress. This is to deal with
           a race - if sm_update_ir_er_div() is called to write IR and ER
           before we've managed to generate enough random numbers to
           initialise them then we must be careful not to overwrite
           them when we finally get enough random numbers. */
        smcb.ir_er_rand_index = -smcb.ir_er_rand_index;
#endif
    }
#ifndef SM_HAS_FUNCTION_FOR_RAND
    else if (smcb.ir_er_rand_index != 0)
    {
        /* Still waiting for IR/ER generation. */
        return FALSE;
    }
#endif

    /* Report back our values for the key-generation parameters. */
    *sm_div_state = smcb.div;
    *sm_key_state = smcb.sm_key_state;
    *sm_key_state = smcb.sm_key_state;
#endif /* !DISABLE_SM_KEY_GENERATION */
    return TRUE;
}

/*! \brief Cause the link to fall, no matter whether or not it is being
           used by L2CAP, ATT, or anyone else. This is only called when
           the peer violates the SM specification and it's not possible
           to continue.

    \param pairing Pointer to pairing instance.
*/
SM_STATIC void sm_drop_link(SM_PAIRING_T *pairing)
{
    DM_ACL_T *acl;

    SM_SANITY_CHECK(pairing);

    /* Look up ACL structure and close it violently. */
    if ((acl = dm_acl_find_by_tbdaddr(SM_TBDADDR_REMOTE(pairing))) != NULL)
        dm_acl_close(acl,
                     dm_acl_client_max,
                     HCI_ERROR_OETC_USER,
                     ACL_IDLE_TIMEOUT_AT_MOST_NONE);
}

/*! \brief Reduce the encryption key size from 128bit to the size that
           we've negotiated. This gets done to the STK and the LTK and
           is acheived by overwriting part of the key with zeros. size
           is in octets. If it is set to 0x10 then this function will
           do nothing, so it is always safe to call even if you don't
           actually need to do any padding.
    \param key Pointer to key to be reduced.
    \param number of significant octets needed in key.
*/
void sm_reduce_enc_key_size(CsrUint16 *key, CsrUint8 size)
{
    if (size <= smcb.enc_key_size_max_threshold
            && size >= smcb.enc_key_size_min_threshold)
    {
        /* Work out how many CsrUint16s cannot be overwritten by zeros. */
        CsrUint8 size16 = (size + 1)/2;

        /* Overwrite the rest. */
        if (size16 < SM_SIZE16_128)
            CsrMemSet(key + size16, 0, SM_SIZE16_128 - size16);

        /* Deal with the case of zeroing half a 16bit word. */
        if ((size & 1) != 0)
            key[size16-1] &= 0xFF;
    }
}

/*! \brief Pairing has finished, so clean up. This will report to
           the application, unmap the L2CAP channel, and free memory.
    \param pairing Pointer to pairing structure.
    \param status indication of whether the pairing succeeded or failed.
*/
static void sm_pairing_complete(SM_PAIRING_T *pairing, CsrUint8 status)
{
    if (pairing != NULL)
    {
        SM_SANITY_CHECK(pairing);

        sm_send_pairing_complete(pairing, status,TRUE);
        if (SM_CID(pairing) != L2CA_CID_INVALID)
            L2CA_UnmapFixedCidReq(SM_CID(pairing));
        sm_free(pairing);
    }
}

/*! \brief Called by the scheduler when the SM timeout fires. This
           means that the peer has not responded to one of our SM
           signals within the allowed 30 seconds. We fail the
           pairing.

           Note that we should really prevent the BLE-U from being
           used anymore.

    \param pdu_type TODO Doesn't appear to be used for anything.
    \param pairing_arg Pointer to pairing structure cast to (void*).
*/
void sm_pairing_timeout(CsrUint16 pdu_type, void *pairing_arg)
{
    SM_PAIRING_T *pairing = (SM_PAIRING_T*)pairing_arg;

    SM_SANITY_CHECK(pairing);

    TIMER_EXPIRED(SM_TIMEOUT(pairing));

    /* TODO make sure we don't use the BLE-U any more and choose a better
       reason code for the application. */
    sm_pairing_complete(pairing, SM_ERROR_UNSPECIFIED_REASON);
}

#ifndef DISABLE_SM_KEY_GENERATION
/*! \brief Deal with L2CA_DATAWRITE_CFM. We're interested in these confirms
           from L2CAP because they enable us to detect when pairing has
           finished if we've sent the final message. If we try to close the
           channel before wairing for this then there is a danger that the
           last message might not get sent at all.
    \param cid L2CAP channel on which the confirm was received.
    \param context event type stored in L2CAP's opaque context.
    \param result Whether our message made it across OK.
*/
void sm_datawrite_cfm(l2ca_cid_t cid, CsrUint16 context, l2ca_data_result_t result)
{
    SM_PAIRING_T *pairing = sm_find_by_cid(cid);

    if (pairing != NULL)
    {
        SM_SANITY_CHECK(pairing);

        if (result != L2CA_DATA_SUCCESS)
        {
            /* Our message did not get to the peer, so pairing has failed. */
            sm_pairing_complete(pairing, SM_ERROR_UNSPECIFIED_REASON);
        }
        else
        {
            CsrUint16 mask;

            /* Build up a mask of what could possibly have been sent after
               this signal. */
            switch ((SM_EVENT_TYPE_T)context)
            {
                case SM_EVENT_MASTER_IDENTIFICATION:
                    mask = (1 << SM_KEY_ID) | (1 << SM_KEY_SIGN);
                    break;

                case SM_EVENT_IDENTITY_ADDRESS_INFORMATION:
                    mask = (1 << SM_KEY_SIGN);
                    break;

                case SM_EVENT_SIGNING_INFORMATION:
                    mask = 0;
                    break;

                default:
                    return;
            }

            /* Check mask against what we actually sent. If there's nothing
               after this one then we can kick the state machine. */
            if ((SM_KEY_DISTRIBUTION(pairing) & mask) == 0)
                sm_pdu_from_air(pairing, SM_EVENT_DATAWRITE_CFM, NULL);
        }
    }
}
#endif

#ifndef DISABLE_SM_SLAVE
/*! \brief Create and send HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY
    \param handle HCI connection handle.
*/
void sm_send_hci_long_term_key_requested_neg_reply(
        hci_connection_handle_t handle)
{
    HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_T *prim =
            pnew(HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY_T);
    prim->common.op_code = HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY;
    prim->common.length = DM_HCI_SENDER_SECURITY_MANAGER;
    prim->connection_handle = handle;
    send_to_hci((DM_UPRIM_T*)prim);
}

/*! \brief Create and send HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY
    \param handle HCI connection handle.
    \param ltk Pointer to long term key.
*/
SM_STATIC void sm_send_hci_long_term_key_requested_reply(
        hci_connection_handle_t handle,
        CsrUint16 *ltk)
{
    HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY_T *prim =
            pnew(HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY_T);
    prim->common.op_code = HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY;
    prim->common.length = DM_HCI_SENDER_SECURITY_MANAGER;
    prim->connection_handle = handle;
    memcpy_unpack128(prim->long_term_key, ltk);
    send_to_hci((DM_UPRIM_T*)prim);
}

/*! \brief Hide the encryption key size and authentication requirements in
           the 'rand' value given to the peer. The peer must return this
           value to us to authenticate an LTK when it is central and we
           are peripheral.
    \param rand Pointer to rand array in which we hide values.
    \param mitm MITM value to hide.
    \param enc_key_size Encryption key size value to hide.
*/
SM_STATIC void sm_encode_key_size_and_mitm(
        CsrUint16 *rand,
        CsrBool mitm,
        CsrUint8 enc_key_size)
{
    CsrUint16 store;

    store = (enc_key_size - 1) | (mitm ? 0x0010 : 0x0000);
    rand[0] = (rand[0] & 0xFFE0) | ((rand[1] & 0x001F) ^ store);
}

/*! \brief Retrieve the encryption key size and authentication requirements
           hidden in the 'rand' value given to the peer. The peer has
           returned this value to us to authenticate an LTK. It is central
           and we are peripheral.
    \param rand Pointer to rand array in which we hid values.
    \param mitm MITM value to return.
    \param enc_key_size Encryption key size value to return.
*/
static void sm_decode_key_size_and_mitm(
        const CsrUint16 *rand,
        CsrBool *mitm,
        CsrUint8 *enc_key_size)
{
    CsrUint16 store;

    store = (rand[0] ^ rand[1]) & 0x001F;

    *mitm = !!(store & 0x10);
    *enc_key_size = (store & 0x000F) + 1;
}

/*! \brief Request from HCI for our long term key. This will only be called
           on the slave side. We have to decide whether to provide the LTK
           or the STK. If the former then we have to reconstruct it from
           the rand and ediv provided.
    \param handle HCI connection handle.
    \param random_number Random number used to reconstruct LTK.
    \param encryption_diversifier Ediv used to reconstruct LTK.
*/
void sm_pairing_ltk_req(hci_connection_handle_t handle,
                        CsrUint8 *random_number,
                        CsrUint16 encryption_diversifier)
{
    SM_PAIRING_T *pairing;
    DM_ACL_T *acl = dm_acl_find_by_handle(handle);

    /* If the ACL has gone then we can ignore this request. */
    if (NULL == acl)
        return;

    /* Decide whether to reply with LTK or STK. */
    if ((pairing = sm_find_by_handle(handle)) != NULL &&
        SM_EXPECT(SM_EVENT_HCI_ENCRYPT) == (SM_EXPECTING(pairing) &
            (SM_EXPECT(SM_EVENT_HCI_ENCRYPT) | SM_DEPEND_HCI_ENCRYPT)))
    {
        /* STK */
        SM_SANITY_CHECK(pairing);

        /* No need to send a DM_SM_KEY_REQUEST_IND in this case */
        sm_send_hci_long_term_key_requested_reply(handle, SM_STK(pairing));
    }
    else
    {
        /* Recreate LTK */
        CsrBool mitm;
        CsrUint16 rand[SM_SIZE16_64];
        CsrBtMemCpyPack(rand, random_number, SM_SIZE16_64);

        /* Recover key size and MITM information */
        sm_decode_key_size_and_mitm(
                rand,
                &mitm,
                &smcb.ltk_regeneration.enc_key_size);

#ifdef SM_HAS_FUNCTION_FOR_AES
        smcb.ltk_regeneration.div =
                sm_div_toggle_mask(rand, encryption_diversifier);

        /* Send a key req IND to application. */
        sm_send_key_request(&acl->addrt,
                            mitm ? DM_SM_SECURITY_ENCRYPTION |
                                   DM_SM_SECURITY_BONDING |
                                   DM_SM_SECURITY_MITM_PROTECTION
                                 : DM_SM_SECURITY_ENCRYPTION |
                                   DM_SM_SECURITY_BONDING,
                            DM_SM_KEY_DIV,
                            smcb.ltk_regeneration.div);
#else
        smcb.ltk_regeneration.mitm = mitm;
        smcb.ltk_regeneration.div = 0;
        sm_aes_recreate_ltk(encryption_diversifier, rand, handle);
#endif
    }
}
#endif

/*! \brief Command complete event giving local Bluetooth Address.
    \param status Indication of success or failure.
    \param bd_addr Pointer to Bluetooth address.
*/
void sm_read_bd_addr_cc(CsrUint8 status, BD_ADDR_T *bd_addr)
{
    tbdaddr_copy_from_public_bd_addr(&smcb.addrt_local, bd_addr);
}

/*! \brief Indication from L2CAP that the SM fixed channel has been mapped.
    \param pairing Pointer to pairing structure.
    \param authentication_requirements authentication requirements, if we
           happen to know them already.
*/
void sm_fixed_cid_mapped(SM_PAIRING_T *pairing,
                         CsrUint16 authentication_requirements)
{
    SM_SANITY_CHECK(pairing);

    if (SM_IS_MASTER(pairing))
    {
        /* Set message expectations appropriate for master role. */
        SM_EXPECTING(pairing) = SM_EXPECT_COMMON | SM_EXPECT_MASTER;

        if (authentication_requirements != 0)
        {
#ifndef DISABLE_DM_BREDR
            if(!dm_sm_is_device_present(SM_TBDADDR_REMOTE(pairing)))
#endif /* DISABLE_DM_BREDR */
            {
                /* Device is not found with SM. So, we are not sure if this is a new device
                   or a device which is already paired and keys are stored with the application.
                   Therefore, send key request to application
                 */
                sm_send_key_request(SM_TBDADDR_REMOTE(pairing),
                                    authentication_requirements,
                                    DM_SM_KEY_ENC_CENTRAL, 0);
            }
#ifndef DISABLE_DM_BREDR
            else
            {
                /* If application asks security level higher than existing
                   i.e. a link with MITM protection while it is existing without MITM protection,
                   then start the pairing process. Else encrypt the link using the existing keys
                   in database or from application.
                 */
                if ((authentication_requirements & DM_SM_SECURITY_MITM_PROTECTION) &&
                    !dm_sm_is_ulp_link_authenticated(SM_TBDADDR_REMOTE(pairing)))
                {
                    /* Start pairing process */
                    /* There is no need to send key request to application, as
                       the key with the application (even if it present) is not sufficient
                       becuase that is unauthenticated key.
                       There directly send IO capability request indication.
                     */
                    send_dm_sm_io_capability_request_ind(pairing);
                }
                else
                {
                    /* Start encryption process */
                    DM_SM_KEY_ENC_CENTRAL_T *key;

                    sm_pairing_rand_generate(pairing);
                    if ((key = dm_sm_get_enc_key(SM_TBDADDR_REMOTE(pairing))) == NULL)
                    {
                        sm_send_key_request(SM_TBDADDR_REMOTE(pairing),
                                            authentication_requirements,
                                            DM_SM_KEY_ENC_CENTRAL, 0);
                    }
                    else
                    {
                        sm_start_encryption(pairing, key->ltk, key->rand, key->ediv);
                    }
                }
            }
#endif /* DISABLE_DM_BREDR */
        }
    }
    else
    {
        /* Set message expectations appropriate for slave role. */
        SM_EXPECTING(pairing) = SM_EXPECT_COMMON | SM_EXPECT_SLAVE;

        /* TODO better conversion of authentication_requirements */
        if (authentication_requirements != 0)
        {
            /* Start pairing process from the slave side. */
            sm_l2cap_security_request(pairing,
                                      (CsrUint8)authentication_requirements);
        }

    }
}

/*! \brief Indication from L2CAP that the SM fixed channel has gone away.
    \param cid Channel ID. If there is a pairing in progress then it gets
    failed.
*/
void sm_fixed_cid_unmapped(l2ca_cid_t cid)
{
    SM_PAIRING_T *pairing;

    if ((pairing = sm_find_by_cid(cid)) != NULL)
    {
        SM_SANITY_CHECK(pairing);

        SM_CID(pairing) = L2CA_CID_INVALID;
        sm_pairing_complete(pairing, SM_ERROR_UNSPECIFIED_REASON);
    }
}

/*! \brief wrapper around sent_to_hci()
    \param prim Pointer to primitive, cast to (void*)
    \param op_code Command OP Code
*/
static void sm_hci_send(void *prim, hci_op_code_t op_code)
{
    ((HCI_UPRIM_T*)prim)->op_code = op_code;
    send_to_hci(prim);
}

/*! \brief Issue HCI_ULP_START_ENCRYPTION
           random can be NULL but the other pointers can't.
    \param pairing Pointer to pairing structure.
    \param ltk Pointer to long term key.
    \param random Pointer to rand.
    \param ediv Pointer to encrypted diversifier (ediv).
*/
void sm_start_encryption(SM_PAIRING_T *pairing,
                         CsrUint16 *ltk,
                         CsrUint16 *random,
                         CsrUint16 ediv)
{
    HCI_ULP_START_ENCRYPTION_T *prim = zpnew(HCI_ULP_START_ENCRYPTION_T);

#ifdef BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE
    DM_ACL_T *p_acl = dm_acl_find_by_tbdaddr(SM_TBDADDR_REMOTE(pairing));
    if(NULL!=p_acl)
        p_acl->dm_acl_client_sm.encrypting= TRUE;
#endif /* BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE */

    SM_SANITY_CHECK(pairing);

    prim->connection_handle = SM_HANDLE(pairing);
    memcpy_unpack128(prim->long_term_key, ltk);
    if (random != NULL)
        CsrBtMemCpyUnpack(prim->random_number, random, SM_SIZE16_64);
    prim->encrypted_diversifier = ediv;

    sm_hci_send(prim, HCI_ULP_START_ENCRYPTION);
}

/*! \brief Indication from HCI that the encryption state has changed.
    \param handle HCI connection handle.
    \param encrypted TRUE if the link is now encrypted.
*/
void sm_encryption_changed(hci_connection_handle_t handle,
                           CsrBool encrypted, CsrUint8 status)
{
    SM_PAIRING_T *pairing = sm_find_by_handle(handle);

#ifndef DISABLE_DM_BREDR
    CsrUint16 security_requirements;
#endif /* DISABLE_DM_BREDR */


    if (encrypted && pairing != NULL)
    {
        SM_SANITY_CHECK(pairing);

#ifndef DISABLE_DM_BREDR
        security_requirements = DM_SM_SECURITY_ENCRYPTION;
        dm_sm_update_sec_requirements(SM_TBDADDR_REMOTE(pairing),
                                        security_requirements);
#endif /* DISABLE_DM_BREDR */

        if ((SM_EXPECTING(pairing) &
                    sm_event_dependencies[SM_EVENT_HCI_ENCRYPT]) == 0)
        {
            /* We're expecting this event as part of the pairing procedure. */
            sm_pdu_from_air(pairing, SM_EVENT_HCI_ENCRYPT, NULL);
        }
        else
        {
            /* This event is not part of a pairing procedure. So we must be
               authenticating an existing key. If pairing isn't also going
               on in parallel then we can clean-up. */
            if (SM_PAIRING_ASSOC_MODEL(pairing) == SM_ASSOC_MODEL_NONE)
            {
                sm_pairing_complete(pairing, HCI_SUCCESS);
            }
        }
    }
    else if((!encrypted) && pairing != NULL)
    {
        /* We will reach here only in cases when we are encrypting a link on
           reconnection. When LL_Reject_Ind is received in response of LL_Enc_Req. */
        /* send DM_SM_SECURITY_CFM to application*/
        sm_send_pairing_complete(pairing, status,FALSE);
    }
}

/*! \brief Update our expectations for the keys that we will receive.
    \param pairing Pointer to pairing structure.
    \param keys bitmask of keys that we're expecting.
*/
static void sm_expect_keys(SM_PAIRING_T *pairing, CsrUint16 keys)
{
    SM_SANITY_CHECK(pairing);

    if ((keys & (1 << SM_KEY_ENC)) != 0)
        SM_EXPECTING(pairing) |= SM_EXPECT_KEY_ENC;

    if ((keys & (1 << SM_KEY_ID)) != 0)
        SM_EXPECTING(pairing) |= SM_EXPECT_KEY_ID;

    if ((keys & (1 << SM_KEY_SIGN)) != 0)
        SM_EXPECTING(pairing) |= SM_EXPECT_KEY_SIGN;
}

/*! \brief Try to send keys to peer.
    \param pairing Pointer to pairing structure.
*/
#ifndef DISABLE_SM_KEY_GENERATION
#ifdef SM_HAS_FUNCTION_FOR_AES
SM_STATIC void sm_send_keys_to_peer(SM_PAIRING_T *pairing)
{
    CsrUint16 scratch[SM_SIZE16_128];
    CsrBool send_keys_to_host = FALSE;

    SM_SANITY_CHECK(pairing);

    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEYS_SENT)) != 0 ||
            (SM_IS_MASTER(pairing) && (SM_EXPECTING(pairing) &
            (SM_EXPECT_KEY_ENC | SM_EXPECT_KEY_ID | SM_EXPECT_KEY_SIGN)) != 0))
    {
        /* Either we've already sent the keys, or we're master and we're
           still waiting for some of the slave's keys. */
        return;
    }


    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ENC)) != 0 ||
        (SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_SIGN)) != 0)
        ++smcb.div;

    /* Send encryption keys */
    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ENC)) != 0)
    {
        /* First send LTK */
        send_keys_to_host = TRUE;
        SM_LTK(scratch, smcb.div);
        sm_reduce_enc_key_size(scratch, SM_ENCRYPTION_KEY_SIZE(pairing));
        sm_l2cap_send_key(pairing, SM_EVENT_ENCRYPTION_INFORMATION, scratch);

        /* Now send EDIV and RAND */
        RANDOM_NUMBER(pairing, scratch + 1, SM_SIZE16_64);
        sm_encode_key_size_and_mitm(
                scratch + 1,
                SM_PAIRING_ASSOC_MODEL(pairing) > SM_ASSOC_MODEL_JUST_WORKS,
                SM_ENCRYPTION_KEY_SIZE(pairing));
        scratch[0] = sm_div_toggle_mask(scratch + 1, smcb.div);
        sm_l2cap_send_key(pairing, SM_EVENT_MASTER_IDENTIFICATION, scratch);
    }

    /* Send identity keys */
    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ID)) != 0)
    {
        SM_IRK(scratch);
        sm_l2cap_send_key(pairing, SM_EVENT_IDENTITY_INFORMATION, scratch);
        sm_l2cap_send_identity_address(pairing);
    }

    /* Send signature keys */
    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_SIGN)) != 0)
    {
        send_keys_to_host = TRUE;
        SM_CSRK(scratch, smcb.div);
        sm_l2cap_send_key(pairing, SM_EVENT_SIGNING_INFORMATION, scratch);
    }

    /* If we're not sending any keys then we won't have to wait for
       L2CA_DATAWRITE_CFM before CsrSchedTidying up. */
    if (SM_KEY_DISTRIBUTION(pairing) == 0)
        SM_EXPECTING(pairing) &= ~SM_EXPECT(SM_EVENT_DATAWRITE_CFM);

    SM_KEY_DISTRIBUTION(pairing) |= (1 << SM_KEYS_SENT);

    /* If we've used the diversifier then we'll need to tell the application */
    if (send_keys_to_host)
    {
        sm_send_keys_to_host(pairing, SM_EVENT_DIV, &smcb.div);
    }
}
#else /* SM_HAS_FUNCTION_FOR_AES */
SM_STATIC void sm_send_keys_to_peer(SM_PAIRING_T *pairing)
{
    SM_SANITY_CHECK(pairing);

    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_GENERATING)) != 0)
    {
        /* We are in the process of generating a key, so come back later. */
        return;
    }


    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEYS_SENT)) != 0 ||
            (SM_IS_MASTER(pairing) && (SM_EXPECTING(pairing) &
            (SM_EXPECT_KEY_ENC | SM_EXPECT_KEY_ID | SM_EXPECT_KEY_SIGN)) != 0))
    {
        /* Either we've already sent the keys, or we're master and we're
           still waiting for some of the slave's keys. */
        return;
    }

    if (((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ENC)) != 0 &&
        (SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ENC_SENT)) == 0) ||
        ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ENC)) == 0 &&
        (SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_SIGN)) != 0 &&
        (SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_SIGN_SENT)) == 0))
            ++smcb.div;


    /* Send encryption keys */
    if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ENC)) != 0 &&
        (SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ENC_SENT)) == 0)
    {
        /* First send LTK */
        SM_KEY_DISTRIBUTION(pairing) |= ((1<<SM_KEY_ENC_SENT)|(1 << SM_KEY_GENERATING));
        sm_aes_enc_distribute(pairing);
    }
    /* Send identity keys */
    else if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ID)) != 0 &&
             (SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_ID_SENT)) == 0)
    {
        SM_KEY_DISTRIBUTION(pairing) |= ((1<<SM_KEY_ID_SENT)|(1 << SM_KEY_GENERATING));
        sm_aes_irk_distribute(pairing);
    }
    /* Send signature keys */
    else if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_SIGN)) != 0 &&
             (SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_SIGN_SENT)) == 0)
    {
        SM_KEY_DISTRIBUTION(pairing) |= ((1<<SM_KEY_SIGN_SENT)|(1 << SM_KEY_GENERATING));
        sm_aes_csrk_distribute(pairing);
    }
    else
    {
        /* If we're not sending any keys then we won't have to wait for
           L2CA_DATAWRITE_CFM before CsrSchedTidying up. */
        if (SM_KEY_DISTRIBUTION(pairing) == 0)
            SM_EXPECTING(pairing) &= ~SM_EXPECT(SM_EVENT_DATAWRITE_CFM);

        SM_KEY_DISTRIBUTION(pairing) |= (1 << SM_KEYS_SENT);

        /* If we've used the diversifier then we'll need to tell the application */
        if ((SM_KEY_DISTRIBUTION(pairing) & (1 << SM_KEY_SEND_TO_HOST)) != 0)
        {
            sm_send_keys_to_host(pairing, SM_EVENT_DIV, &smcb.div);
            SM_KEY_DISTRIBUTION(pairing) &= ~(1 << SM_KEY_SEND_TO_HOST);
        }
    }
    return;
}
#endif /* SM_HAS_FUNCTION_FOR_AES */
#endif /* DISABLE_SM_KEY_GENERATION */





/*! \brief Deal with pairing information after the exchange or pairing
           request and response. This function decides whether to use
           Just Works, Passkey entry or Out of Band data for the association
           model. It also works out key distribution and encryption keys size.
    \param pairing Pointer to pairing structure.
    \returns Indication of how pairing should proceed. This can be summarised
             as Abort, Wait or Proceed.
*/
static SM_ASSOC_MODEL_T sm_process_pairing_information_real(SM_PAIRING_T *pairing)
{
    CsrUint8 keys;
    CsrUint8 keys2;
    CsrUint8 io_local;
    CsrUint8 io_peer;
    CsrUint8 mitm_local;
    CsrUint8 mitm_peer;
    CsrUint8 p1[SM_SIZE8_128];

    SM_SANITY_CHECK(pairing);

    memcpy_unpack128(p1, SM_P1(pairing));

    /* Key distribution.

       Master sends the initiator keys and expects the responder keys.
       Slave sends the responder keys and expects the initiator keys.
    */

    keys  = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_INITIATOR_KEY_DIST] &
            p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_INITIATOR_KEY_DIST];
    keys2 = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_RESPONDER_KEY_DIST] &
            p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_RESPONDER_KEY_DIST];

    /* Read IO capability and authentication requirements. */
    if (SM_IS_MASTER(pairing))
    {
        io_local = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_IO_CAPABILITY];
        io_peer = p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_IO_CAPABILITY];
        mitm_local = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_AUTHREQ];
        mitm_peer = p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_AUTHREQ];

#ifndef DISABLE_SM_KEY_GENERATION
        SM_KEY_DISTRIBUTION(pairing) = keys;
#endif
        keys = keys2;
    }
    else
    {
        io_local = p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_IO_CAPABILITY];
        io_peer = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_IO_CAPABILITY];
        mitm_local = p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_AUTHREQ];
        mitm_peer = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_AUTHREQ];

#ifndef DISABLE_SM_KEY_GENERATION
        SM_KEY_DISTRIBUTION(pairing) = keys2;
#endif
    }

    /* Only interested in the MITM bit */
    mitm_local &= 4;
    mitm_peer &= 4;

#ifndef DISABLE_SM_KEY_GENERATION
    if (SM_KEY_DISTRIBUTION(pairing) != 0)
        SM_EXPECTING(pairing) |= SM_EXPECT_KEY_SEND;
#endif

    sm_expect_keys(pairing, keys);

    /* Encryption key size */
    SM_ENCRYPTION_KEY_SIZE(pairing) =
            p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_MAX_ENC_KEY_SIZE] <
            p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_MAX_ENC_KEY_SIZE] ?
            p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_MAX_ENC_KEY_SIZE] :
            p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_MAX_ENC_KEY_SIZE] ;

    if (SM_ENCRYPTION_KEY_SIZE(pairing) < smcb.enc_key_size_min_threshold)
    {
        sm_pairing_failed(pairing, SM_ERROR_ENCRYPTION_KEY_SIZE);
        return SM_ASSOC_MODEL_NONE;
    }

    /* Determine Pairing Method

       First we check for OOB.
       Then we check for Just Works.
       Then we check to see if passkey pairing is possible.
       Finally we decide whether we should be displaying a passkey, or
       asking the user to type one in. */

    if (p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_OOB_DATA] != 0
            && p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_OOB_DATA] != 0)
        return SM_ASSOC_MODEL_OOB;

    /* TODO Should we fail pairing if we have OOB and they don't? */

    /* Not using OOB, so Zero the TK */
    CsrMemSet(SM_TK(pairing), 0, SM_SIZE16_128 * sizeof(CsrUint16));

    if ((mitm_local | mitm_peer) == 0)
        return SM_ASSOC_MODEL_JUST_WORKS;

    if (io_local == HCI_IO_CAP_NO_INPUT_NO_OUTPUT
            || io_peer == HCI_IO_CAP_NO_INPUT_NO_OUTPUT
            || ((io_local == HCI_IO_CAP_DISPLAY_ONLY
                    || io_local == HCI_IO_CAP_DISPLAY_YES_NO)
                && (io_peer == HCI_IO_CAP_DISPLAY_ONLY
                    || io_peer == HCI_IO_CAP_DISPLAY_YES_NO)))
    {
        /* Authenticated key requested, but not possible. If we are the
           ones demanding MITM protection then we must fail pairing now.
           Otherwise we will drop down to Just Works and leave it up to
           the peer device to enforce its own security. */
        if (mitm_local != 0)
        {
            sm_pairing_failed(pairing, SM_ERROR_AUTHENTICATION_REQUIREMENTS);
            return SM_ASSOC_MODEL_NONE;
        }

        return SM_ASSOC_MODEL_JUST_WORKS;
    }

#ifdef DISABLE_SM_PASSKEY
    /* Passkey requested but not supported. */
    sm_pairing_failed(pairing, SM_ERROR_AUTHENTICATION_REQUIREMENTS);
    return SM_ASSOC_MODEL_NONE;
#else
    if (io_local != HCI_IO_CAP_KEYBOARD_ONLY
            && (io_local != HCI_IO_CAP_KEYBOARD_DISPLAY
                || (io_peer == HCI_IO_CAP_KEYBOARD_DISPLAY
                    && SM_IS_MASTER(pairing))))
    {
        sm_send_passkey_indication(pairing, SM_TK(pairing));
        return SM_ASSOC_MODEL_DISPLAY_PASSKEY;
    }

    sm_send_passkey_request(pairing);

    return SM_ASSOC_MODEL_ENTER_PASSKEY;
#endif
}

/* Modify results of sm_process_pairing_information_real so that we can wait
   for user confirmation even for Just Works. */
/*! \brief Wrapper around sm_process_pairing_information_real() that allows
           us to wait for user confirmation even for Just Works.
    \param pairing Pointer to pairing structure.
    \returns Indication of how pairing should proceed. This can be summarised
             as Abort, Wait or Proceed.
*/
SM_ASSOC_MODEL_T sm_process_pairing_information(SM_PAIRING_T *pairing)
{
    SM_ASSOC_MODEL_T assoc_model;
    SM_SANITY_CHECK(pairing);

    if((assoc_model = sm_process_pairing_information_real(pairing)) == SM_ASSOC_MODEL_NONE)
        return assoc_model;

    SM_PAIRING_ASSOC_MODEL(pairing) = assoc_model;

    if ((SM_PAIRING_ASSOC_MODEL(pairing) == SM_ASSOC_MODEL_JUST_WORKS
                || SM_PAIRING_ASSOC_MODEL(pairing) == SM_ASSOC_MODEL_OOB)
            && !dm_sm_auto_accept_just_works())
    {
        /* Just works but we're not configured to auto-accept, so request
           confirmation and wait. */
        send_user_confirmation_request(pairing);
        return SM_ASSOC_MODEL_USER_CONFIRMATION;
    }

    return SM_PAIRING_ASSOC_MODEL(pairing);
}

/*! \brief Start, or prepare for, encryption using STK.
    \param pairing Pointer to pairing procedure.
*/
SM_STATIC void sm_encrypt_using_stk(SM_PAIRING_T *pairing)
{
    SM_SANITY_CHECK(pairing);

    sm_reduce_enc_key_size(SM_STK(pairing),
                           SM_ENCRYPTION_KEY_SIZE(pairing));
    SM_AES_DEBUG_KEY(SM_STK(pairing), 7);

    if (SM_IS_MASTER(pairing))
    {
        sm_start_encryption(pairing, SM_STK(pairing), NULL, 0);
    }
}

/*! \brief We've just reconstructed the peer's Confirm value from its
           Rand and it matches. So it's time to generate the STK.

           When this function is called, rand points to the 128bit
           random number provided by the peer during the pairing
           process. We re-use it here as a place to store pre_stk.
           If we're Slave then the peer's contribution to the STK
           (which is LS8(Mrand)) is already in the right place -
           the least significant 8 octets of rand) - but if we're
           master then the peer's contribution (which is LS8(Srand))
           is in the wrong place and we have to move it.

           pre_stk = LS8(Srand) || LS8(Mrand) where LS8(data) is the
           least significant 8 octets of data and || is a concatenation
           so 0x1234 || 0x5678 = 0x12345678

           STK = e(TK, pre_stk)
           So we just take pre_stk and AES encrypt it using TK as the key.

    \param pairing Pointer to pairing procedure.
    \param rand Pointer to RAND from peer.
*/
SM_STATIC void sm_confirm_good(SM_PAIRING_T *pairing, CsrUint16 *rand)
{
    /* Default to pointing to where LS8(Srand) should start in pre_stk. */
    CsrUint16 *randp = rand + SM_SIZE16_64;

    SM_SANITY_CHECK(pairing);

    SM_AES_DEBUG_KEY(SM_RAND(pairing), 20);

    if (SM_IS_MASTER(pairing))
    {
        /* rand will be used to generate the STK. Currently it contains
           the Srand. We need to move LS8(Srand) to where we need it. */
        CsrMemCpy(randp,
               rand,
               SM_SIZE16_64 * sizeof(CsrUint16));

        /* Change to pointing to where LS8(Mrand) should be copied. */
        randp = rand;
    }
#ifdef SM_HAS_FUNCTION_FOR_AES
    else
        sm_l2cap_pairing_rand(pairing);
#endif

    /* Copy the least significant octets of the local RAND into the
       right place in rand. If we're master then we copy it into the
       least significant 8 octets. If we're slave then we copy it into
       the most significant octets. */
    CsrMemCpy(randp,
           SM_RAND(pairing),
           SM_SIZE16_64 * sizeof(CsrUint16));

    SM_AES_DEBUG_KEY(rand, 6);

    /* rand now contains pre-stk, so we just need to run AES encryption. */
#ifdef SM_HAS_FUNCTION_FOR_AES
    sm_aes_encrypt_128(SM_STK(pairing),
                       rand,
                       SM_TK(pairing));
    sm_encrypt_using_stk(pairing);
#else
    sm_aes_stk_generate(pairing, rand);
#endif

    SM_AES_DEBUG_KEY(SM_RAND(pairing), 21);
}

/*! \brief We've detected an error in the pairing process and need to
           tell both the peer and the application.
    \param pairing Pointer to pairing structure.
    \param error Error code of failure.
*/
void sm_pairing_failed(SM_PAIRING_T *pairing, SM_ERROR_T error)
{
    SM_SANITY_CHECK(pairing);

    sm_l2cap_pairing_failed(SM_CID(pairing), error);
    sm_pairing_complete(pairing, (CsrUint8)error);
}

/*! \brief Very basic state machine. Most messages received from air and
    from the application will be routed through here. The pairing process
    is very linear, so there is usually only one valid message to receive
    at any given time. So we keep track of what PDU we're expecting and
    compare it with the one we actually receive.
    \param pairing Pointer to pairing structure.
    \param type Event to be processed.
    \param data Pointer to data associated with event.
*/
void sm_pdu_from_air(SM_PAIRING_T *pairing, SM_EVENT_TYPE_T type, void *data)
{
    CsrUint16 expect = SM_EXPECT(type);
    SM_ERROR_T error = SM_ERROR_UNSPECIFIED_REASON;

    SM_SANITY_CHECK(pairing);


    if(SM_EVENT_PAIRING_FAILED==type)
    {
        sm_pairing_complete(pairing, ((CsrUint8*)data)[1]);
        return;
    }

    /* A speical case where we are Slave but he are holding a NEG RSP,
        we just wait for a our turn of transaction to occur.
     */
    if ((SM_EXPECTING(pairing) & SM_EXPECT(SM_ABORT_WAIT_PDU)) != 0)
    {
        error = (SM_ERROR_T)(SM_EXPECTING(pairing) & 0xFF);

        /* Certainly we would not like to send a Pairing failed PDU in response
             to a recieved Pairing failed PDU.
        */
        sm_pairing_failed(pairing, error);
        return;
    }

    /* Ensure that we are actually expecting to receive this event and that
       we are not expecting any events that come before this one */
    if ((SM_EXPECTING(pairing) & expect) == 0
            || (SM_EXPECTING(pairing) & sm_event_dependencies[type]) != 0)
    {
        sm_drop_link(pairing);
        return;
    }

    /* We're no longer expecting this event. */
    SM_EXPECTING(pairing) &= ~expect;

    /* Calling function will guarantee that data is of correct length */
    switch (type)
    {
#ifndef DISABLE_SM_SLAVE
        case SM_EVENT_PAIRING_REQUEST:
            /* We'll need this primitive as part of the pairing algorithm. */
            sm_aes_pack_pairing(SM_P1(pairing), data);

            /* Inform the application and charge the random number cache. */
            send_dm_sm_io_capability_response_ind(pairing);
            sm_pairing_rand_generate(pairing);
            send_dm_sm_io_capability_request_ind(pairing);
            break;
#endif

        case SM_EVENT_PAIRING_RESPONSE:
            /* We'll need this primitive as part of the pairing algorithm. */
            sm_aes_pack_pairing(SM_P1(pairing), data);

            /* Inform the application */
            send_dm_sm_io_capability_response_ind(pairing);

            /* Decide what to do next. Either we'll plough ahead with
               generating the confirm, or we'll have to wait for user
               invovement. Or it could even fail completely. */
            switch (sm_process_pairing_information(pairing))
            {
                case SM_ASSOC_MODEL_JUST_WORKS:
                case SM_ASSOC_MODEL_OOB:
                case SM_ASSOC_MODEL_DISPLAY_PASSKEY:
                    /* Plough ahead */
                    sm_aes_generate_rand_confirm(pairing);
                    break;

                case SM_ASSOC_MODEL_NONE:
                case SM_ASSOC_MODEL_UNKNOWN:
                    /* Failure. pairing structure has been destroyed,
                       so return immediately to avoid accessing freed
                       memory. */
                    return;

                case SM_ASSOC_MODEL_ENTER_PASSKEY:
                case SM_ASSOC_MODEL_USER_CONFIRMATION:
                    /* Wait for user involvement */
                    break;
            }

            /* We should not expect to receive security requests after this */
            SM_EXPECTING(pairing) &= ~SM_EXPECT(SM_EVENT_SECURITY_REQUEST);
            break;

        case SM_EVENT_PAIRING_CONFIRM:
            /* Store the value for later. We'll need it to compare with after
               we've received RAND. */
            CsrMemCpy(SM_CONF(pairing), data, SM_SIZE16_128 * sizeof(CsrUint16));

            /* Proceed with the next step of the pairing process */
            if (SM_IS_MASTER(pairing))
                sm_l2cap_pairing_rand(pairing);
            else if ((SM_EXPECTING(pairing)
                        & SM_EXPECT(SM_EVENT_PASSKEY_OR_CONFIRMATION)) == 0)
                sm_aes_generate_rand_confirm(pairing);
            break;

        case SM_EVENT_PAIRING_RANDOM:
            SM_AES_DEBUG_KEY(SM_RAND(pairing), 13);

            /* We've received RAND, so we must regenerate the confirm value
               and compare with the one we received earlier. */
#ifdef SM_HAS_FUNCTION_FOR_AES
            if (!sm_aes_check_rand_confirm(pairing, data))
            {
                sm_pairing_failed(pairing, SM_ERROR_CONFIRM_VALUE_FAILED);
                return;
            }

            sm_confirm_good(pairing, data);
            break;
#else
            (void)sm_aes_check_rand_confirm(pairing, data);
            return;
#endif

        case SM_EVENT_HCI_ENCRYPT:
        case SM_EVENT_ENCRYPTION_INFORMATION:
        case SM_EVENT_MASTER_IDENTIFICATION:
        case SM_EVENT_IDENTITY_INFORMATION:
        case SM_EVENT_IDENTITY_ADDRESS_INFORMATION:
        case SM_EVENT_SIGNING_INFORMATION:
#ifndef DISABLE_SM_KEY_GENERATION
            sm_send_keys_to_peer(pairing);
#endif

            /* Fallthrough */
        case SM_EVENT_DATAWRITE_CFM:
            sm_send_keys_to_host(pairing, type, data);
            break;

        case SM_EVENT_SECURITY_REQUEST:
            /* The peer is requesting slave-initiated security. If we're
               not already initiating our own then we fake up an equivalent
               request from the application and process that. */
            if (SM_SEC_CFM(pairing) == NULL)
            {
                /* Fake a security request. */
                DM_UPRIM_T *uprim;
                /* BLE ACL Connection should be up by now.
                   Get the acl instance and fill in flags value */
                DM_ACL_T *p_acl;
                CsrUint16 connection_flags = 0;

                p_acl = dm_acl_find_by_tbdaddr(SM_TBDADDR_REMOTE(pairing));

                /* p_acl should not be NULL */
                if(NULL != p_acl)
                    connection_flags = p_acl->flags;

                dm_sm_security_req(
                    CSR_SCHED_QID_INVALID,
                    SM_TBDADDR_REMOTE(pairing),
                    connection_flags,
                    0,
                    (CsrUint16)(((CsrUint8*)data)[1] | DM_SM_SECURITY_ENCRYPTION),
                    &uprim);

                /* Convince ourselves that nothing is yet in progress. */
                SM_EXPECTING(pairing) |= SM_EXPECT(SM_EVENT_SECURITY_REQUEST);

                sm_handle_security_req(uprim);
                CsrPmemFree(uprim);
            }
            break;

        case SM_EVENT_APP_IO_CAPABILITIES:
            /* Application has given us its IO capabilities. We transmit
               them to the peer in either a Pairing Request, or a Pairing
               Response message, depending on role. */
            sm_l2cap_pairing_reqrsp(pairing, data);
            /* The above function can destroy the pairing structure,
               so we must return now to avoid illegal memory accesses. */
            return;

        case SM_EVENT_PASSKEY_OR_CONFIRMATION:
            /* Passkey/Confirmation stage is over, so we can set TK and
               continue with the pairing process. */
            if (data != NULL)
            {
                SM_TK(pairing)[0] = (CsrUint16)(*(CsrUint32*)data & 0xFFFF);
                SM_TK(pairing)[1] = (CsrUint16)((*(CsrUint32*)data >> 16) & 0xFFFF);
            }

            if (SM_IS_MASTER(pairing) || (SM_EXPECTING(pairing)
                        & SM_EXPECT(SM_EVENT_PAIRING_CONFIRM)) == 0)
                sm_aes_generate_rand_confirm(pairing);
            break;

        default:
            sm_drop_link(pairing);
    }

    /* When we're expecting nothing more (except failure) that means we're
       done. */
    if ((SM_EXPECTING(pairing) & ~SM_EXPECT(SM_EVENT_PAIRING_FAILED)) == 0)
        sm_pairing_complete(pairing, 0);
}

/*! \brief Sets the Max and Min encryption key size threshold
    value. It sets the Max value on the condition that the size
    is less than SM_MAX_ENC_KEY_SIZE

    \param Max key size supplied from sm_init func.
    \param Min key size supplied from sm_init func.
    \return None.
*/
void sm_set_enc_key_sizes(CsrUint8 max_size, CsrUint8 min_size)
{
    if(max_size<= SM_MAX_ENC_KEY_SIZE)
      smcb.enc_key_size_max_threshold = max_size;
    smcb.enc_key_size_min_threshold = min_size;
}

/*! \brief Process and sends LTK request reply to the
    controller depending upon the response from the
    application.

    \param Addrt (BD_ADDR and Type).
    \param response (TRUE/FALSE).
    \return None.
*/
#ifndef DISABLE_SM_SLAVE
void sm_key_request_rsp_div(TYPED_BD_ADDR_T * addrt, CsrBool response)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) != NULL)
    {
        if(response)
        {
#ifdef SM_HAS_FUNCTION_FOR_AES
            CsrUint16 ltk[SM_SIZE16_128];

            SM_LTK(ltk, smcb.ltk_regeneration.div);
            sm_reduce_enc_key_size(ltk, smcb.ltk_regeneration.enc_key_size);
            sm_send_hci_long_term_key_requested_reply(p_acl->handle, ltk);
#else /* SM_HAS_FUNCTION_FOR_AES */
            sm_aes_recreate_ltk_from_div(p_acl->handle);
#endif /* SM_HAS_FUNCTION_FOR_AES */
        }
        else
        {
            sm_send_hci_long_term_key_requested_neg_reply(p_acl->handle);
        }
    }
}
#endif /* DISABLE_SM_SLAVE */
#endif
