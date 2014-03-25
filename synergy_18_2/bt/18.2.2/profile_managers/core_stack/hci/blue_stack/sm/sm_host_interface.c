/*!
 Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
     All rights reserved.

\file   sm_host_interface.c

\brief  Security Manager Host interface
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE

/*! \brief Start pairing process. By this stage we've already
           decided that current security level is not enough.
    \param prim Pointer to request primitive.
*/
static SM_PAIRING_T *sm_start_security_req(DM_SM_SECURITY_REQ_T *prim)
{
    SM_PAIRING_T *pairing = NULL;
    l2ca_conflags_t flags =
            (l2ca_conflags_t)(prim->connection_flags | L2CA_CONFLAG_LOCK_ACL);

    /* Open ACL and create pairing structure. */
    if (sm_lock_acl(&prim->addrt, flags)
            && (pairing = sm_add(&prim->addrt, L2CA_CID_INVALID)) != NULL)
    {
        SM_SANITY_CHECK(pairing);

        if (SM_CID(pairing) == L2CA_CID_INVALID)
        {
            /* Must map fixed CID */
            L2CA_MapFixedCidReq(
                    SM_TBDADDR_REMOTE(pairing),
                    L2CA_CID_SECURITY_MANAGER,
                    prim->security_requirements,
                    flags);
        }
        else
        {
            /* Connection already exists. */
            /* Make sure there isn't something in progress already. */
            if ((SM_EXPECTING(pairing) & (SM_EXPECT(SM_EVENT_PAIRING_REQUEST)
                            | SM_EXPECT(SM_EVENT_SECURITY_REQUEST))) != 0
                    && SM_SEC_CFM(pairing) == NULL)
            {
                sm_fixed_cid_mapped(pairing, prim->security_requirements);
            }
            else
                pairing = NULL;
        }
    }

    return pairing;
}

/*! \brief send DM_SM_SECURITY_CFM.
    \param cfm Pointer to pointer to primitive to send.
    \param status indication of success or failure.
*/
static void sm_send_security_cfm(DM_SM_SECURITY_COMMON_T **cfm, CsrUint8 status, CsrUint8 error_base)
{
    if ((*cfm)->phandle != CSR_SCHED_QID_INVALID)
    {
        if(error_base & SM_ERROR_BASE)
        {
            (*cfm)->status = DM_SM_CONVERT_SM_ERROR(status);
        }
        else
        {
            (*cfm)->status = status;
        }
        DM_SendMessage(*cfm);
    }
    else
        CsrPmemFree(*cfm);

    *cfm = NULL;
}

/*! \brief Handle application request for security.
           Decide whether current security level is enough.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_security_req(DM_UPRIM_T *const uprim)
{
    CsrUint8 status = HCI_SUCCESS;
    CsrUint8 error_base = SM_ERROR_BASE;

    DM_SM_SECURITY_COMMON_T *cfm =
        pcopy(&uprim->dm_sm_security_common, sizeof(DM_SM_SECURITY_COMMON_T));
    cfm->type = DM_SM_SECURITY_CFM;

    if (dm_sm_current_security_acceptable(
                &uprim->dm_sm_security_req.addrt,
                uprim->dm_sm_security_req.security_requirements)
            != DM_SM_ACCEPTABLE)
    {
        /* Need more security, so start pairing process. */
        SM_PAIRING_T *pairing;

        if ((pairing = sm_start_security_req(&uprim->dm_sm_security_req))
                != NULL)
        {
            /* Pairing underway, so store CFM primitive and exit */
            SM_SANITY_CHECK(pairing);

            CsrPmemFree(SM_SEC_CFM(pairing));
            SM_SEC_CFM(pairing) = cfm;
            return;
        }
        else
        {
            /* Something's gone wrong. TODO actually work out what. */
            status = HCI_ERROR_HOST_BUSY_PAIRING;
            /* Sending HCI error, not SM error */
            error_base &= ~SM_ERROR_BASE;

            /* Unlock the ACL as it is locked earlier in sm_start_security_req() */
            sm_unlock_acl(&uprim->dm_sm_security_req.addrt);
        }
    }

    sm_send_security_cfm(&cfm, status, error_base);
}

/*! \brief Key request response from application.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_key_request_rsp(DM_UPRIM_T *const uprim)
{
    DM_SM_KEYS_T keys;

    /* Merge key into device database. */
    CsrMemSet(&keys, 0, sizeof(keys));
    keys.security_requirements =
            uprim->dm_sm_key_request_rsp.security_requirements;
    keys.present = uprim->dm_sm_key_request_rsp.key_type;
    keys.u[0] = uprim->dm_sm_key_request_rsp.key;
    dm_sm_update_keys(&uprim->dm_sm_key_request_rsp.addrt, &keys);

    switch (uprim->dm_sm_key_request_rsp.key_type)
    {
        case DM_SM_KEY_ENC_CENTRAL:
        {
            SM_PAIRING_T *pairing;
            DM_SM_KEY_ENC_CENTRAL_T *key =
                uprim->dm_sm_key_request_rsp.key.enc_central;

            pairing = sm_find_by_tbdaddr(&uprim->dm_sm_key_request_rsp.addrt);
            if (pairing != NULL && SM_CID(pairing) != L2CA_CID_INVALID)
                sm_start_encryption(pairing, key->ltk, key->rand, key->ediv);
            break;
        }

        case DM_SM_KEY_SIGN:
            sm_data_unsign_key_returned(&uprim->dm_sm_key_request_rsp.addrt, TRUE);
            break;

#ifndef DISABLE_SM_SLAVE
        case DM_SM_KEY_DIV:
            if (uprim->dm_sm_key_request_rsp.key.div
                    == smcb.ltk_regeneration.div)
            {
                sm_key_request_rsp_div(&uprim->dm_sm_key_request_rsp.addrt,
                                       TRUE);
            }
            break;

        case DM_SM_KEY_DIV_CSRK:
            sm_data_unsign_key_returned(&uprim->dm_sm_key_request_rsp.addrt, FALSE);
            break;
#endif /* DISABLE_SM_SLAVE */

        default:
            /* Currently unhandled key */
            break;
    }

#ifdef DISABLE_DM_BREDR
    /* Free the key memory */
    if(uprim->dm_sm_key_request_rsp.key_type!=DM_SM_KEY_DIV&&
       uprim->dm_sm_key_request_rsp.key_type!=DM_SM_KEY_DIV_CSRK)
        CsrPmemFree(uprim->dm_sm_key_request_rsp.key.none);
#endif /* DISABLE_DM_BREDR */

}

/*! \brief Key request negative response from application.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_key_request_neg_rsp(DM_UPRIM_T *const uprim)
{
    switch (uprim->dm_sm_key_request_neg_rsp.key_type)
    {
        case DM_SM_KEY_ENC_CENTRAL:
        {
            SM_PAIRING_T *pairing;

            /* Find suitable pairing control structure. */
            pairing = sm_find_by_tbdaddr(&uprim->dm_sm_key_request_neg_rsp.addrt);
            if (pairing != NULL && SM_CID(pairing) != L2CA_CID_INVALID)
                send_dm_sm_io_capability_request_ind(pairing);
            break;
        }

        case DM_SM_KEY_SIGN:
            sm_data_unsign_key_missing(&uprim->dm_sm_key_request_neg_rsp.addrt, TRUE);
            break;

#ifndef DISABLE_SM_SLAVE
        case DM_SM_KEY_DIV:
            sm_key_request_rsp_div(&uprim->dm_sm_key_request_neg_rsp.addrt, FALSE);
            break;
        case DM_SM_KEY_DIV_CSRK:
            sm_data_unsign_key_missing(&uprim->dm_sm_key_request_neg_rsp.addrt, FALSE);
            break;
#endif /* DISABLE_SM_SLAVE */

        default:
            /* Currently unhandled key */
            break;
    }
}

/*! \brief IO capability request response from application.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_io_capability_request_rsp(DM_UPRIM_T *const uprim)
{
    SM_PAIRING_T *pairing;
    DM_SM_IO_CAPABILITY_REQUEST_RSP_T *prim
            = (DM_SM_IO_CAPABILITY_REQUEST_RSP_T*)uprim;
    CsrUint8 data[SM_EVENT_PAIRING_REQUEST_LENGTH];

    /* Find suitable pairing control structure. */
    if ((pairing = sm_find_by_tbdaddr(&prim->addrt)) == NULL
            || (SM_CID(pairing) == L2CA_CID_INVALID))
        return;

    SM_SANITY_CHECK(pairing);

    /* Store pairing information. */
    data[SM_PAIRING_OFFSET_IO_CAPABILITY] = prim->io_capability;
    if ((data[SM_PAIRING_OFFSET_OOB_DATA] = prim->oob_data_present) != DM_SM_OOB_DATA_NONE)
    {
        data[SM_PAIRING_OFFSET_OOB_DATA] = 1;
        memcpy_pack128(SM_TK(pairing), prim->oob_hash_c);
    }
    data[SM_PAIRING_OFFSET_AUTHREQ] = prim->authentication_requirements;
    data[SM_PAIRING_OFFSET_MAX_ENC_KEY_SIZE] = smcb.enc_key_size_max_threshold;
    data[SM_PAIRING_OFFSET_INITIATOR_KEY_DIST] =
            prim->key_distribution & 0xFF;
    data[SM_PAIRING_OFFSET_RESPONDER_KEY_DIST] =
            (prim->key_distribution >> 8) & 0xFF;

    /* Poke state machine. */
    sm_pdu_from_air(pairing, SM_EVENT_APP_IO_CAPABILITIES, data);

    /* Free hash c memory as it has been copied in pairing instance */
    CsrPmemFree(prim->oob_hash_c);
    /*
     * Free rand r, if it has been provided by application just to avoid a memory leak.
     * Though it is of no use for a LE case and shall be NULL.
     */
    CsrPmemFree(prim->oob_rand_r);

}

/*! \brief Common function to fail pairing process when application aborts.
    \param addt Pointer to address.
    \param reason Error code to use.
    \param signal type
*/
static void sm_abort(TYPED_BD_ADDR_T *addrt, SM_ERROR_T reason, dm_prim_t type)
{
    SM_PAIRING_T *pairing;
    CsrUint8 data[2];
    CsrBool pdu_with_abort = FALSE;

    /* Find suitable pairing control structure. */
    if ((pairing = sm_find_by_tbdaddr(addrt)) == NULL
            || (SM_CID(pairing) == L2CA_CID_INVALID))
        return;

    SM_SANITY_CHECK(pairing);

    switch(type)
    {
        case DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP:
        {
            if(!SM_IS_MASTER(pairing))
            {
                pdu_with_abort = TRUE;
            }
            /* for Master just a normal sm pairing fail in the end of function*/
        }
        break;

        case DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP:
        {
            if(SM_IS_MASTER(pairing) ||
                (SM_EXPECTING(pairing) & SM_EXPECT(SM_EVENT_PAIRING_CONFIRM)) == 0)
            {
                pdu_with_abort = TRUE;
            }
            else
            {
                /* For Slave we will wait till next PDU from Master , to follow transaction rules*/
                SM_EXPECTING(pairing) = (SM_EXPECT(SM_ABORT_WAIT_PDU) | (reason & 0xff));
                return;
            }
        }
        break;

        case DM_SM_USER_PASSKEY_REQUEST_NEG_RSP:
        {
            if(SM_IS_MASTER(pairing) ||
                (SM_EXPECTING(pairing) & SM_EXPECT(SM_EVENT_PAIRING_CONFIRM)) == 0)
            {
                pdu_with_abort = TRUE;
            }
            else
            {
                /* For Slave we will wait till next PDU from Master , to follow transaction rules*/
                SM_EXPECTING(pairing) = (SM_EXPECT(SM_ABORT_WAIT_PDU) | (reason & 0xff)) ;
                return;
            }
        }
        break;

        default:
            /* Someone has called this function without proper type */
            break;
    }

    if(pdu_with_abort)
    {
        sm_pairing_failed(pairing, reason);
    }
    else
    {
        /* Send pairing failed without sending a PDU*/
        data[1] = (CsrUint8)reason;
        data[0] = SM_EVENT_PAIRING_FAILED;
        sm_pdu_from_air(pairing, SM_EVENT_PAIRING_FAILED, data);
    }
}

/*! \brief IO capability request negative response from application.
           We just pass it on to the abort function.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_io_capability_request_neg_rsp(DM_UPRIM_T *const uprim)
{
    sm_abort(&uprim->dm_sm_io_capability_request_neg_rsp.addrt,
             SM_ERROR_UNSPECIFIED_REASON, uprim->type);
}

/*! \brief Common security IND generation.
    \param type Primitive type.
    \param addrt Pointer to address.
    \param flags flags bitfield.
*/
void send_dm_common_ind(dm_prim_t type, TYPED_BD_ADDR_T *addrt, CsrUint16 flags)
{
    /* TODO sort out duplication between this and BR/EDR Security Manager */
    DM_COMMON_IND_T *prim = zpnew(DM_COMMON_IND_T);

    prim->type = type;
    tbdaddr_copy(&prim->addrt, addrt);
    prim->flags = flags | DM_SM_FLAGS_SECURITY_MANAGER;

    DM_SendMessageAMphandle(prim);
}

/*! \brief Send DM_SM_SIMPLE_PAIRING_COMPLETE and DM_SM_SECURITY_CFM.
    \param pairing Pointer to pairing structure.
    \param status Indication of success or failure.
*/
void sm_send_pairing_complete(SM_PAIRING_T *pairing, CsrUint8 status, CsrBool sm_err_base)
{
    SM_SANITY_CHECK(pairing);

    /* DM_SM_SIMPLE_PAIRING_COMPLETE */
    if (SM_PAIRING_ASSOC_MODEL(pairing) != SM_ASSOC_MODEL_NONE)
    {
        DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T *prim;

        SM_PAIRING_ASSOC_MODEL(pairing) = SM_ASSOC_MODEL_NONE;

        prim = pnew(DM_SM_SIMPLE_PAIRING_COMPLETE_IND_T);
        prim->type = DM_SM_SIMPLE_PAIRING_COMPLETE_IND;
        tbdaddr_copy(&prim->addrt, SM_TBDADDR_REMOTE(pairing));
        prim->status = DM_SM_CONVERT_SM_ERROR(status);
        prim->flags = DM_SM_FLAGS_SECURITY_MANAGER;

        DM_SendMessageAMphandle(prim);
    }
    /* The case handles give indication at master
       when slave initiated encryption (without pairing process) fails.
     */
    else if (SM_SEC_CFM(pairing) &&
                SM_IS_MASTER(pairing) &&
                SM_SEC_CFM(pairing)->phandle == CSR_SCHED_QID_INVALID)
    {
        DM_SM_SECURITY_IND_T *prim = pnew(DM_SM_SECURITY_IND_T);
        prim->type = DM_SM_SECURITY_IND;
        tbdaddr_copy(&prim->addrt, SM_TBDADDR_REMOTE(pairing));
        prim->connection_flags = DM_SM_FLAGS_NONE; /* we want to send HCI status here */
        prim->security_requirements = SM_SEC_CFM(pairing)->security_requirements;
        prim->status = status;
        DM_SendMessageAMphandle(prim);
    }

    /* DM_SM_SECURITY_CFM */
    if (SM_SEC_CFM(pairing) != NULL)
    {
        sm_send_security_cfm(&SM_SEC_CFM(pairing), status, (CsrUint8)((sm_err_base==TRUE)?SM_ERROR_BASE:0));
        sm_unlock_acl(SM_TBDADDR_REMOTE(pairing));
    }
}

/*! \brief Send DM_SM_IO_CAPABILITY_REQUEST_IND to application.
    \param pairing Pointer to pairing structure.
*/
void send_dm_sm_io_capability_request_ind(SM_PAIRING_T *pairing)
{
    SM_SANITY_CHECK(pairing);

    /* TODO Initiator flag */
    SM_PAIRING_ASSOC_MODEL(pairing) = SM_ASSOC_MODEL_UNKNOWN;
    send_dm_common_ind(DM_SM_IO_CAPABILITY_REQUEST_IND,
                       SM_TBDADDR_REMOTE(pairing), \
                       DM_SM_FLAGS_SECURITY_MANAGER);
}

/*! \brief Send DM_SM_IO_CAPABILITY_RESPONSE_IND to application.
    \param pairing Pointer to pairing structure.
*/
void send_dm_sm_io_capability_response_ind(SM_PAIRING_T *pairing)
{
    /* SM_EVENT_PAIRING_REQUEST_LENGTH is odd. We pack two CsrUint8s into a CsrUint16.
       So we need to unpack length+1 bytes to be sure we get it all. */
    CsrUint8 data[SM_EVENT_PAIRING_REQUEST_LENGTH + 1];
    CsrUint8 *ptr;
    CsrUint8 offset;
    CsrUint16 flags;
    DM_SM_IO_CAPABILITY_RESPONSE_IND_T *prim;

    SM_SANITY_CHECK(pairing);

    /* If the offset is odd then we'll need to jump over the first byte
       unpacked. We set ptr accordingly. */
    if (SM_IS_MASTER(pairing))
    {
        offset = P1_OFFSET8_PAIRING_RSP/2;
        ptr = data + (P1_OFFSET8_PAIRING_RSP % 2);
        flags = DM_SM_FLAGS_SECURITY_MANAGER | DM_SM_FLAGS_INITIATOR;
    }
    else
    {
        offset = P1_OFFSET8_PAIRING_REQ/2;
        ptr = data + (P1_OFFSET8_PAIRING_REQ % 2);
        flags = DM_SM_FLAGS_SECURITY_MANAGER;
    }

    CsrBtMemCpyUnpack(data,
                  SM_P1(pairing) + offset,
                  (SM_EVENT_PAIRING_REQUEST_LENGTH + 1)/2);

    prim = pnew(DM_SM_IO_CAPABILITY_RESPONSE_IND_T);
    prim->type = DM_SM_IO_CAPABILITY_RESPONSE_IND;
    tbdaddr_copy(&prim->addrt, SM_TBDADDR_REMOTE(pairing));
    prim->io_capability = ptr[SM_PAIRING_OFFSET_IO_CAPABILITY];
    prim->oob_data_present = ptr[SM_PAIRING_OFFSET_OOB_DATA];
    prim->authentication_requirements = ptr[SM_PAIRING_OFFSET_AUTHREQ];
    prim->flags = flags;
    prim->key_distribution = ptr[SM_PAIRING_OFFSET_INITIATOR_KEY_DIST]
            | ((CsrUint16)ptr[SM_PAIRING_OFFSET_RESPONDER_KEY_DIST] << 8);

    DM_SendMessageAMphandle(prim);
}

/*! \brief Handle DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*)
*/
void sm_handle_user_confirmation_request_neg_rsp(DM_UPRIM_T *const uprim)
{
    sm_abort(&uprim->dm_sm_user_confirmation_request_neg_rsp.addrt,
             SM_ERROR_UNSPECIFIED_REASON, uprim->type);
}

/*! \brief Handle DM_SM_USER_CONFIRMATION_REQUEST_RSP.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*)
*/
void sm_handle_user_confirmation_request_rsp(DM_UPRIM_T *const uprim)
{
    SM_PAIRING_T *pairing =
            sm_find_by_tbdaddr(&uprim->dm_sm_user_confirmation_request_rsp.addrt);

    if (pairing == NULL)
        return;

    SM_SANITY_CHECK(pairing);

    sm_pdu_from_air(pairing, SM_EVENT_PASSKEY_OR_CONFIRMATION, NULL);
}

#ifndef DISABLE_SM_PASSKEY
/*! \brief Handle DM_SM_USER_PASSKEY_REQUEST_NEG_RSP.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*)
*/
void sm_handle_passkey_neg_response(DM_UPRIM_T *const uprim)
{
    sm_abort(&uprim->dm_sm_user_passkey_request_neg_rsp.addrt,
            SM_ERROR_PASSKEY_ENTRY_FAILED, uprim->type);
}

/*! \brief Send passkey request to application.
    \param pairing Pointer to pairing structure.
*/
void sm_send_passkey_request(SM_PAIRING_T *pairing)
{
    DM_SM_USER_PASSKEY_REQUEST_IND_T *ind
        = zpnew(DM_SM_USER_PASSKEY_REQUEST_IND_T);

    SM_SANITY_CHECK(pairing);

    ind->type = DM_SM_USER_PASSKEY_REQUEST_IND;
    tbdaddr_copy(&ind->addrt, SM_TBDADDR_REMOTE(pairing));
    /* TODO initiator flag */
    ind->flags = DM_SM_FLAGS_SECURITY_MANAGER;

    DM_SendMessageAMphandle(ind);

    SM_EXPECTING(pairing) |= SM_EXPECT_PASSCONF;
}

/*! \brief Handle DM_SM_USER_PASSKEY_REQUEST_RSP.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*)
*/
void sm_handle_passkey_response(DM_UPRIM_T *const uprim)
{
    SM_PAIRING_T *pairing =
            sm_find_by_tbdaddr(&uprim->dm_sm_user_passkey_request_rsp.addrt);

    if (pairing == NULL)
        return;

    SM_SANITY_CHECK(pairing);

    sm_pdu_from_air(pairing,
                    SM_EVENT_PASSKEY_OR_CONFIRMATION,
                    &uprim->dm_sm_user_passkey_request_rsp.numeric_value);
}

/*! \brief Generate passkey and send indication to application.
    \param pairing Pointer to pairing structure.
    \param passkey Pointer to return generated passkey to caller.
*/
void sm_send_passkey_indication(SM_PAIRING_T *pairing, CsrUint16 *passkey)
{
    DM_SM_USER_PASSKEY_NOTIFICATION_IND_T *ind
        = zpnew(DM_SM_USER_PASSKEY_NOTIFICATION_IND_T);

    SM_SANITY_CHECK(pairing);

    ind->type = DM_SM_USER_PASSKEY_NOTIFICATION_IND;
    tbdaddr_copy(&ind->addrt, SM_TBDADDR_REMOTE(pairing));

    /* Need a random number from 0 to 999999 */
    RANDOM_NUMBER(pairing, passkey, 2);
    ind->passkey = ((CsrUint32)passkey[0] | ((CsrUint32)passkey[1]) << 16)
            % 1000000ul;
    passkey[0] = (CsrUint16)(ind->passkey & 0xFFFF);
    passkey[1] = (CsrUint16)((ind->passkey >> 16) & 0xFFFF);

    /* TODO initiator flag */
    ind->flags = DM_SM_FLAGS_SECURITY_MANAGER;

    DM_SendMessageAMphandle(ind);
}
#endif /* DISABLE_SM_PASSKEY */

/*! \brief Send key request to application.
    \brief addrt Pointer to typed Bluetooth address of peer.
    \brief security_requirements security requirements.
    \brief key_type Key type.
*/
void sm_send_key_request(TYPED_BD_ADDR_T *addrt,
                         CsrUint16 security_requirements,
                         DM_SM_KEY_TYPE_T key_type,
                         CsrUint16 encryption_diversifier)
{
    DM_SM_KEY_REQUEST_IND_T *ind = zpnew(DM_SM_KEY_REQUEST_IND_T);


    ind->type = DM_SM_KEY_REQUEST_IND;
    tbdaddr_copy(&ind->addrt, addrt);
    ind->security_requirements = security_requirements;
    ind->key_type = key_type;
    ind->diversifier = encryption_diversifier;

    DM_SendMessageAMphandle(ind);
}

/*! \brief Send div request for CSRK generation to application.
    \brief addrt Pointer to typed Bluetooth address of peer.
*/
void sm_send_csrk_div_request(TYPED_BD_ADDR_T *addrt)
{
    DM_SM_KEY_REQUEST_IND_T *ind = zpnew(DM_SM_KEY_REQUEST_IND_T);

    ind->type = DM_SM_KEY_REQUEST_IND;
    tbdaddr_copy(&ind->addrt, addrt);
    ind->key_type= DM_SM_KEY_DIV_CSRK;
    DM_SendMessageAMphandle(ind);
}

/*! \brief Send user confirmation request to application.
    \param pairing Pointer to pairing structure.
*/
void send_user_confirmation_request(SM_PAIRING_T *pairing)
{
    CsrUint8 p1[SM_SIZE8_128];
    DM_SM_USER_CONFIRMATION_REQUEST_IND_T *ind =
        zpnew(DM_SM_USER_CONFIRMATION_REQUEST_IND_T);

    SM_SANITY_CHECK(pairing);

    ind->type = DM_SM_USER_CONFIRMATION_REQUEST_IND;
    tbdaddr_copy(&ind->addrt, SM_TBDADDR_REMOTE(pairing));
    ind->numeric_value = 0xFFFFFFFF;

    memcpy_unpack128(p1, SM_P1(pairing));
    if (SM_IS_MASTER(pairing))
    {
        ind->io_cap_local = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_IO_CAPABILITY];
        ind->io_cap_remote = p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_IO_CAPABILITY];
    }
    else
    {
        ind->io_cap_local = p1[P1_OFFSET8_PAIRING_RSP + SM_PAIRING_OFFSET_IO_CAPABILITY];
        ind->io_cap_remote = p1[P1_OFFSET8_PAIRING_REQ + SM_PAIRING_OFFSET_IO_CAPABILITY];
    }

    /* TODO initiator flag */
    ind->flags = DM_SM_FLAGS_SECURITY_MANAGER;

    SM_EXPECTING(pairing) |= SM_EXPECT_PASSCONF;
    DM_SendMessageAMphandle(ind);
}

/*! \brief Send signing counter change indication to application.
    \param addrt Pointer to address, or NULL for local signing counter.
    \param counter Signing counter.
*/
void sm_send_csrk_counter_change_ind(TYPED_BD_ADDR_T *addrt, CsrUint32 counter)
{
    DM_SM_CSRK_COUNTER_CHANGE_IND_T *prim;

    prim = zpnew(DM_SM_CSRK_COUNTER_CHANGE_IND_T);
    prim->type = DM_SM_CSRK_COUNTER_CHANGE_IND;
    if (addrt == NULL)
        prim->local_csrk = TRUE;
    else
        tbdaddr_copy(&prim->addrt, addrt);
    prim->counter = counter;

    DM_SendMessageAMphandle(prim);
}

/*! \brief Collect received keys and send to application when we've got them all.
    We expect to receive the keys in the order specified by the SM spec.
    So we assume that we will get master ID after encryption info.
    \param pairing Pointer to pairing structure.
    \param type Event for this particular key.
    \param ptr Pointer to key cast to (void*).
*/
void sm_send_keys_to_host(SM_PAIRING_T *pairing,
                          SM_EVENT_TYPE_T type,
                          void *ptr)
{
    DM_SM_KEY_TYPE_T key;       /* Key type */
    CsrUint16 i;
    CsrUint16 k;
    DM_SM_KEYS_IND_T *ind;  /* Pointer to primitive */

    SM_SANITY_CHECK(pairing);

    /* Create or retrieve IND primitive. */
    if ((ind = SM_KEYS_IND(pairing)) == NULL)
    {
        if (type == SM_EVENT_HCI_ENCRYPT || type == SM_EVENT_DATAWRITE_CFM)
            return;

        ind = SM_KEYS_IND(pairing) = zpnew(DM_SM_KEYS_IND_T);

        ind->type = DM_SM_KEYS_IND;
        tbdaddr_copy(&ind->addrt, SM_TBDADDR_REMOTE(pairing));
    }

    ind->keys.encryption_key_size = SM_ENCRYPTION_KEY_SIZE(pairing);
    ind->keys.security_requirements =
        SM_PAIRING_ASSOC_MODEL(pairing) > SM_ASSOC_MODEL_JUST_WORKS
        ? (DM_SM_SECURITY_MITM_PROTECTION | DM_SM_SECURITY_ENCRYPTION)
        : DM_SM_SECURITY_ENCRYPTION;

    /* Mostly we're searching for a free space. But if we've received a
       SM_EVENT_MASTER_IDENTIFICATION then we're searching for an existing
       DM_SM_KEY_ENC_CENTRAL. */
    key = type == SM_EVENT_MASTER_IDENTIFICATION
            ? DM_SM_KEY_ENC_CENTRAL : DM_SM_KEY_NONE;

    for (i = 0, k = ind->keys.present;
        (i < DM_SM_MAX_NUM_KEYS) && ((k & ((1 << DM_SM_NUM_KEY_BITS) - 1)) != key);
         ++i, k >>= DM_SM_NUM_KEY_BITS)/* nop */;
    if (i == DM_SM_MAX_NUM_KEYS)
        type = SM_EVENT_RESERVED;

    /* Deal with key depending on type. */
    switch (type)
    {
#ifndef DISABLE_SM_KEY_GENERATION
        case SM_EVENT_DIV:
            ind->keys.u[i].div = *(CsrUint16*)ptr;
            key = DM_SM_KEY_DIV;
            break;
#endif

        case SM_EVENT_ENCRYPTION_INFORMATION:
            ind->keys.u[i].enc_central = pnew(DM_SM_KEY_ENC_CENTRAL_T);
            CsrMemCpy(ind->keys.u[i].enc_central->ltk,
                   ptr,
                   SM_SIZE16_128 * sizeof(CsrUint16));
            key = DM_SM_KEY_ENC_CENTRAL;
            break;

        case SM_EVENT_MASTER_IDENTIFICATION:
            ind->keys.u[i].enc_central->ediv = ((CsrUint16*)ptr)[0];
            CsrMemCpy(ind->keys.u[i].enc_central->rand,
                   &((CsrUint16*)ptr)[1],
                   SM_SIZE16_64 * sizeof(CsrUint16));
            /* key = DM_SM_KEY_ENC_CENTRAL; */
            break;

#ifndef DISABLE_SM_PRIVACY
        case SM_EVENT_IDENTITY_INFORMATION:
            ind->keys.u[i].id = pnew(DM_SM_KEY_ID_T);
            CsrMemCpy(ind->keys.u[i].id->irk,
                   ptr,
                   SM_SIZE16_128 * sizeof(CsrUint16));
            key = DM_SM_KEY_ID;
            break;

        case SM_EVENT_IDENTITY_ADDRESS_INFORMATION:
            {
                CsrUint8 *a = (CsrUint8*)ptr;
                TBDADDR_TYPE(ind->id_addrt) = a[1];
                TBDADDR_LAP(ind->id_addrt) = a[2] | (((CsrUint24)a[3]) << 8)
                        | (((CsrUint24)a[4]) << 16);
                TBDADDR_UAP(ind->id_addrt) = a[5];
                TBDADDR_NAP(ind->id_addrt) = a[6] | (((CsrUint16)a[7]) << 8);
                /* key = DM_SM_KEY_NONE; */
                break;
            }
#endif
#ifndef DISABLE_SM_SIGNING
        case SM_EVENT_SIGNING_INFORMATION:
            ind->keys.u[i].sign = zpnew(DM_SM_KEY_SIGN_T);
            CsrMemCpy(ind->keys.u[i].sign->csrk,
                   ptr,
                   SM_SIZE16_128 * sizeof(CsrUint16));
            key = DM_SM_KEY_SIGN;
            break;
#endif
        default:
            break;
    }

    /* Record which key we've added. */
    ind->keys.present |= key << (i * DM_SM_NUM_KEY_BITS);

    /* When we've got them all, we can send the primitive. */
    if ((SM_EXPECTING(pairing) &
            (SM_EXPECT_KEY_ENC |
             SM_EXPECT_KEY_ID |
             SM_EXPECT_KEY_SIGN |
             SM_EXPECT(SM_EVENT_HCI_ENCRYPT) |
             SM_EXPECT(SM_EVENT_DATAWRITE_CFM))) == 0)
    {
        dm_sm_update_keys(SM_TBDADDR_REMOTE(pairing), &ind->keys);
        DM_SendMessageAMphandle(ind);
        SM_KEYS_IND(pairing) = NULL;
    }
}

#endif /* INSTALL_SM_MODULE */
