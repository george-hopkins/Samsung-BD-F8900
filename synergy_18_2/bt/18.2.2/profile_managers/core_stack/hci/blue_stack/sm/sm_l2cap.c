/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_l2cap.c

\brief  Security Manager L2CAP interface
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE

/*! SM Protocol PDU lengths. */
static const CsrUint8 sm_pdu_length[] =
{
#define SM_EVENT_TABLE_SEP ,
#define SM_EVENT_TABLE_X(name, length, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11) length
    SM_EVENT_TABLE_PDUS
#undef SM_EVENT_TABLE_SEP
#undef SM_EVENT_TABLE_X
};

/* \brief The L2CAP fixed channel for SM has been (or has failed to be) mapped.
   \param addrt Pointer to address.
   \param cid L2CAP CID.
   \param con_ctx Opaque connection context.
   \param result Indication of success or failure.
   \param flags Connection flags.
*/
static void sm_l2cap_mapped_fixed_cid(TYPED_BD_ADDR_T * addrt,
                                      l2ca_cid_t cid,
                                      CsrUint16 con_ctx,
                                      l2ca_misc_result_t result,
                                      l2ca_conflags_t flags)
{
    SM_PAIRING_T *pairing = sm_add(addrt, cid);

    if (result == L2CA_MISC_SUCCESS)
    {
        if (pairing == NULL)
            L2CA_UnmapFixedCidReq(cid);
        else
            sm_fixed_cid_mapped(pairing, con_ctx);
    }
    else if (result != L2CA_MISC_INITIATING && pairing != NULL)
        sm_fixed_cid_unmapped(SM_CID(pairing));
}

/*! \brief Deal with L2CA_DATAREAD_IND.

     This function assumes that we will get an MBLK of chain-length 1.
     This is guaranteed because upstream L2CAP data only comes in multi-block
     chains as a result of segmentation, and SM is Basic mode only, which
     does not permit segmentation.

    \param result Indication of success or failure.
    \param cid L2CAP CID.
    \param mblk. Pointer to MBLK.
*/
static void sm_l2cap_dataread_ind(l2ca_data_result_t result,
                                    l2ca_cid_t cid,
                                    CsrMblk *mblk)
{
    SM_PAIRING_T *pairing;
    CsrMblkSize length;
    CsrUint8 *mblk_data = NULL;
    void *data;

    if (result == L2CA_DATA_SUCCESS
            && (pairing = sm_find_by_cid(cid)) != NULL
            && (length = CsrMblkGetLength(mblk)) != 0
            && (mblk_data = CsrMblkMap(mblk, 0, length)) != NULL)
    {
        SM_EVENT_TYPE_T op_code = (SM_EVENT_TYPE_T)mblk_data[0];

        SM_SANITY_CHECK(pairing);

        /* Do sanity checks on the message we've received. */
        if (op_code != SM_EVENT_RESERVED
                && op_code < SM_EVENT_PDU_MAXIMUM
                && sm_pdu_length[op_code] == length)
        {
            /* If it's a key then may as well squash it now. */
            if (length == 17 || length == 11)
            {
                /* It's a key, so squash it. */
                CsrUint16 key_size16 = (length - 1)/2;
                data = CsrPmemAlloc(key_size16 * sizeof(CsrUint16));
                CsrBtMemCpyPack(data, mblk_data + 1, key_size16);
            }
            else
            {
                /* It's not a key, so take a copy. */
                data = pcopy(mblk_data, length);
            }

            /* Poke state machine. */
            sm_pdu_from_air(pairing, op_code, data);
            CsrPmemFree(data);

            CsrMblkUnmap(mblk, mblk_data);
            return;
        }
    }

    if(mblk_data)
    {
        CsrMblkUnmap(mblk, mblk_data);
    }

    sm_l2cap_pairing_failed(cid, SM_ERROR_INVALID_PARAMETERS);
}

/*! \brief Main L2CAP primitive handling function.
    \param uprim Pointer to L2CAP primitive cast to (L2CA_UPRIM_T*).
*/
void sm_l2cap_handler(L2CA_UPRIM_T * const uprim)
{
    /* We will panic if either of these values change. */
    l2ca_cid_t fixed_cid = L2CA_CID_SECURITY_MANAGER;
    l2ca_misc_result_t panic_result = L2CA_MISC_SUCCESS;

    switch (uprim->type)
    {
        case L2CA_REGISTER_FIXED_CID_CFM:
            /* L2CA_REGISTER_FIXED_CID_REQ should never fail and the response
               should always refer to the Security Manager fixed channel. */
            fixed_cid = uprim->l2ca_register_fixed_cid_cfm.fixed_cid;
            panic_result = uprim->l2ca_register_fixed_cid_cfm.result;
            break;

        case L2CA_MAP_FIXED_CID_IND:
            fixed_cid = uprim->l2ca_map_fixed_cid_ind.fixed_cid;
            sm_l2cap_mapped_fixed_cid(
                    &uprim->l2ca_map_fixed_cid_ind.addrt,
                    uprim->l2ca_map_fixed_cid_ind.cid,
                    0,          /* Dummy context TODO #define */
                    L2CA_MISC_SUCCESS,
                    uprim->l2ca_map_fixed_cid_ind.flags);
            break;

        case L2CA_MAP_FIXED_CID_CFM:
            fixed_cid = uprim->l2ca_map_fixed_cid_cfm.fixed_cid;
            sm_l2cap_mapped_fixed_cid(
                    &uprim->l2ca_map_fixed_cid_cfm.addrt,
                    uprim->l2ca_map_fixed_cid_cfm.cid,
                    uprim->l2ca_map_fixed_cid_cfm.con_ctx,
                    uprim->l2ca_map_fixed_cid_cfm.result,
                    uprim->l2ca_map_fixed_cid_cfm.flags);
            break;

        case L2CA_UNMAP_FIXED_CID_IND:
            sm_fixed_cid_unmapped(uprim->l2ca_unmap_fixed_cid_ind.cid);
            break;

        case L2CA_DATAWRITE_CFM:
#ifndef DISABLE_SM_KEY_GENERATION
            sm_datawrite_cfm(uprim->l2ca_datawrite_cfm.cid,
                             uprim->l2ca_datawrite_cfm.req_ctx,
                             uprim->l2ca_datawrite_cfm.result);
#endif
            break;

        case L2CA_DATAREAD_IND:
            sm_l2cap_dataread_ind(uprim->l2ca_dataread_ind.result,
                                  uprim->l2ca_dataread_ind.cid,
                                  uprim->l2ca_dataread_ind.data);
            break;

        default:
            /* Unexpected L2CAP primitive. Ignore. */
            break;
    }

    if (fixed_cid != L2CA_CID_SECURITY_MANAGER
            || (panic_result != L2CA_MISC_SUCCESS))
        BLUESTACK_PANIC(CSR_BT_PANIC_SM_L2CAP_HANDLER);
}

/*! \brief Wrapper around L2CA_DataWriteReqEx() to start/reset
    timer for each packet sent.
    \param pairing Pointer to pairing structure.
    \param pdu Pointer to array containing PDU to be sent.
*/
void sm_l2cap_datawrite_req(SM_PAIRING_T *pairing, CsrUint8 *pdu)
{
    SM_SANITY_CHECK(pairing);

    timer_start(&SM_TIMEOUT(pairing),
                CSR_SCHED_SECOND*30,
                sm_pairing_timeout,
                pdu[0],
                pairing);

    L2CA_DataWriteReqEx(SM_CID(pairing),
                        sm_pdu_length[pdu[0]],
                        pcopy(pdu, sm_pdu_length[pdu[0]]),
                        pdu[0]);
}

/*! \brief Send Pairing request or response, depending on role.
    \param pairing Pointer to pairing structure.
    \param data Pointer to PDU data to send.
*/
void sm_l2cap_pairing_reqrsp(SM_PAIRING_T *pairing,
                             CsrUint8 *data)
{
    SM_SANITY_CHECK(pairing);

    /* Write PDU type. */
    data[0] = (CsrUint8)(SM_IS_MASTER(pairing)
            ? SM_EVENT_PAIRING_REQUEST : SM_EVENT_PAIRING_RESPONSE);

    /* Store message locally for use later. */
    sm_aes_pack_pairing(SM_P1(pairing), data);

    /* Final checks before sending. */
    if (SM_IS_MASTER(pairing)
            || sm_process_pairing_information(pairing) != SM_ASSOC_MODEL_NONE)
    {
        /* Delay sending if we're still waiting for random numbers. */
        if (sm_rand_is_generated(pairing, data))
            sm_l2cap_datawrite_req(pairing, data);
    }
}

/*! \brief Send pairing failed message.
    \param cid L2CAP CID.
    \param error Error code.
*/
void sm_l2cap_pairing_failed(l2ca_cid_t cid, SM_ERROR_T error)
{
    CsrUint8 *pdu = (CsrUint8*)CsrPmemAlloc(SM_EVENT_PAIRING_FAILED_LENGTH);

    pdu[0] = (CsrUint8)SM_EVENT_PAIRING_FAILED;
    pdu[1] = (CsrUint8)error;

    L2CA_DataWriteReqEx(cid,
                        SM_EVENT_PAIRING_FAILED_LENGTH,
                        pdu,
                        SM_EVENT_PAIRING_FAILED);
}

/*! \brief Send pairing confirm message.
    \param pairing Pointer to pairing structure.
    \param packed_confirm Packed confirm data.
*/
void sm_l2cap_pairing_confirm(SM_PAIRING_T *pairing,
                              const CsrUint16 *packed_confirm)
{
    CsrUint8 pdu[SM_EVENT_PAIRING_CONFIRM_LENGTH];

    SM_SANITY_CHECK(pairing);

    pdu[0] = (CsrUint8)SM_EVENT_PAIRING_CONFIRM;

    memcpy_unpack128(&pdu[1], packed_confirm);
    sm_l2cap_datawrite_req(pairing, pdu);
}

/*! \brief Send pairing rand message.
    \param pairing Pointer to pairing structure.
*/
void sm_l2cap_pairing_rand(SM_PAIRING_T *pairing)
{
    CsrUint8 pdu[SM_EVENT_PAIRING_RANDOM_LENGTH];

    SM_SANITY_CHECK(pairing);

    pdu[0] = (CsrUint8)SM_EVENT_PAIRING_RANDOM;

    memcpy_unpack128(&pdu[1], SM_RAND(pairing));
    sm_l2cap_datawrite_req(pairing, pdu);
}

#ifndef DISABLE_SM_SLAVE
/*! \brief Send slave-initiated security request.
    \param pairing Pointer to pairing structure.
    \param authentication_requirements authentication requirements.
*/
void sm_l2cap_security_request(SM_PAIRING_T *pairing,
                               CsrUint8 authentication_requirements)
{
    CsrUint8 pdu[SM_EVENT_SECURITY_REQUEST_LENGTH];

    SM_SANITY_CHECK(pairing);

    pdu[0] = (CsrUint8)SM_EVENT_SECURITY_REQUEST;
    pdu[1] = authentication_requirements;

    sm_l2cap_datawrite_req(pairing, pdu);
}
#endif

#ifndef DISABLE_SM_KEY_GENERATION
/*! \brief Send cryptographic keys.
    \param pairing Pointer to pairing structure.
    \param type Key type.
    \param data Pointer to packed key.
*/
void sm_l2cap_send_key(SM_PAIRING_T *pairing,
                       SM_EVENT_TYPE_T type,
                       CsrUint16 *data)
{
    CsrUint8 *pdu = CsrPmemAlloc(sm_pdu_length[type]);

    SM_SANITY_CHECK(pairing);

    pdu[0] = (CsrUint8)type;
    /* We don't need to subtract 1 from length because
       we know that it will be odd and so the /2 does
       it for us. */
    CsrBtMemCpyUnpack(&pdu[1], data, (CsrUint16)(sm_pdu_length[type]/2));

    L2CA_DataWriteReqEx(SM_CID(pairing),
                        sm_pdu_length[type],
                        pdu,
                        (CsrUint16)type);
}
#endif

#ifndef DISABLE_SM_KEY_GENERATION
#ifndef DISABLE_SM_PRIVACY
/*! \brief Send Identity address.
    \param pairing Pointer to pairing structure.
*/
void sm_l2cap_send_identity_address(SM_PAIRING_T *pairing)
{
    CsrUint8 pdu[SM_EVENT_IDENTITY_ADDRESS_INFORMATION_LENGTH];

    SM_SANITY_CHECK(pairing);

    pdu[0] = (CsrUint8)SM_EVENT_IDENTITY_ADDRESS_INFORMATION;
    pdu[1] = 0;
    /* TODO support addresses other than public */
    pdu[2] = (CsrUint8)(TBDADDR_LAP(*SM_TBDADDR_LOCAL()) & 0xFF);
    pdu[3] = (CsrUint8)((TBDADDR_LAP(*SM_TBDADDR_LOCAL()) >> 8) & 0xFF);
    pdu[4] = (CsrUint8)((TBDADDR_LAP(*SM_TBDADDR_LOCAL()) >> 16) & 0xFF);
    pdu[5] = (CsrUint8)TBDADDR_UAP(*SM_TBDADDR_LOCAL());
    pdu[6] = (CsrUint8)(TBDADDR_NAP(*SM_TBDADDR_LOCAL()) & 0xFF);
    pdu[7] = (CsrUint8)((TBDADDR_NAP(*SM_TBDADDR_LOCAL()) >> 8) & 0xFF);

    sm_l2cap_datawrite_req(pairing, pdu);
}
#endif
#endif

#endif /* INSTALL_SM_MODULE */
