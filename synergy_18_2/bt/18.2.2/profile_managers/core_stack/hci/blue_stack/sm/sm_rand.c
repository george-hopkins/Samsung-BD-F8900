/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_rand.c

\brief  Random number handling for Synergy.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE
#ifndef SM_HAS_FUNCTION_FOR_RAND

/*! \brief Request random numbers from controller.
    \param num Number of requests to send.
*/
void sm_rand_generate(CsrUint8 num)
{
    CsrUint8 i;
    HCI_ULP_RAND_T prim;

    prim.common.op_code = HCI_ULP_RAND;
    prim.common.length = DM_HCI_SENDER_SECURITY_MANAGER;

    for (i = num; i != 0; --i)
        send_to_hci((DM_UPRIM_T*)pcopy(&prim, sizeof(HCI_ULP_RAND_T)));
}

/*! \brief Generate random numbers required for pairing process.
    \param pairing Pointer to pairing contol structure.
*/
void sm_pairing_rand_generate(SM_PAIRING_T *pairing)
{
    SM_RAND_GENERATION_QUEUE_T **elpp, *elp;

    SM_SANITY_CHECK(pairing);

    /* Search for existing record - we should not find one. */
    for (elpp = &smcb.rand_generation; (elp=*elpp) != NULL; elpp = &elp->next)
        if (elp->pairing == pairing)
            BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

    /* Add new record */
    *elpp = zpnew(SM_RAND_GENERATION_QUEUE_T);
    (*elpp)->pairing = pairing;

    pairing->sm_random_number_index = 0;

    /* Request random numbers. */
    sm_rand_generate(4);
}

/*! \brief Notification that the pairing structure has gone and we should free
           any random number structure associated with it.
    \param pairing Pointer to pairing contol structure.
*/
void sm_rand_pairing_freed(SM_PAIRING_T *pairing)
{
    SM_RAND_GENERATION_QUEUE_T **elpp, *elp;

    for (elpp = &smcb.rand_generation; (elp=*elpp) != NULL; elpp = &elp->next)
    {
        if (elp->pairing == pairing)
        {
            *elpp = elp->next;
            CsrPmemFree(elp->data);
            CsrPmemFree(elp);
            break;
        }
    }
}

/*! \brief Random numbers have arrived for pairing.
    \returns Pointer to where they should be written, or NULL
*/
static CsrUint16 *sm_pairing_rand_complete(void)
{
    CsrUint16 *ret;
    SM_PAIRING_T *pairing;
    SM_RAND_GENERATION_QUEUE_T *elp;

    /* See if we need any random numbers. */
    if ((elp = smcb.rand_generation) == NULL)
        return NULL;

    pairing = elp->pairing;

    SM_SANITY_CHECK(pairing);

    /* Point to where they're needed and update the index. */
    ret = pairing->sm_random_number_store + pairing->sm_random_number_index;
    pairing->sm_random_number_index += SM_SIZE16_64;

    /* See if we still need any more random numbers for this pairing. */
    if (pairing->sm_random_number_index == RANDS_MAX)
    {
        /* This one's done */
        smcb.rand_generation = elp->next;
        /* Send any postponed L2CAP PDU */
        if (elp->data != NULL)
            sm_l2cap_datawrite_req(pairing, elp->data);
        CsrPmemFree(elp->data);
        CsrPmemFree(elp);
    }

    return ret;
}

/*! \brief Read a random number stored in a pairing structure.
    \param pairing Pointer to pairing structure.
    \param rand Pointer to where they should be written.
    \param length How many to write.
*/
void sm_pairing_random_number(SM_PAIRING_T *pairing,
                              CsrUint16 *rand,
                              CsrUint8 length)
{
    SM_SANITY_CHECK(pairing);

    /* Sanity checks */
    if (pairing == NULL || pairing->sm_random_number_index < length)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

    /* Update record of how many random numbers we'll be left with. */
    pairing->sm_random_number_index -= length;

    /* Copy them across */
    CsrMemCpy(rand,
           pairing->sm_random_number_store + pairing->sm_random_number_index,
           length * sizeof(CsrUint16));
}

/*! \brief Random numbers have been sent to us by the controller.
    \param Pouinter to 8 octets of randomness.
*/
void sm_rand_complete(CsrUint8 *random)
{
    CsrUint16 *dst;

    switch (smcb.ir_er_rand_index)
    {
        case -4:
        case -3:
        case -2:
        case -1:
            /* We don't need the random number anymore, so throw away. */
            ++smcb.ir_er_rand_index;
            return;

        case 1:
            dst = smcb.sm_key_state.ir;
            break;

        case 2:
            dst = smcb.sm_key_state.ir + SM_SIZE16_64;
            break;

        case 3:
            dst = smcb.sm_key_state.er;
            break;

        case 4:
            dst = smcb.sm_key_state.er + SM_SIZE16_64;
            break;

        default:
            /* This is actually case 0. It's done as the default case
               to prevent some compilers from warning that dst might
               be used before being set. */
            if ((dst = sm_pairing_rand_complete()) == NULL)
            {
                /* It's not for pairing, so try for privacy */
                dst = sm_privacy_rand_complete(random);
            }
            break;
    }

    if (smcb.ir_er_rand_index != 0 && --smcb.ir_er_rand_index == 0)
        dm_sm_ble_init_complete();

    if (dst != NULL)
        CsrBtMemCpyPack(dst, random, SM_SIZE16_64);
}

/*! \brief Check to see if we have sufficient random numbers for a given
    pairing.
    \param pairing Pointer to pairing control block.
    \param l2cap_data Opaque data to store until we have sufficient random numbers.
    \returns TRUE if we have sufficient, FALSE if we're waiting for more.
*/
CsrBool sm_rand_is_generated(SM_PAIRING_T *pairing, CsrUint8 *l2cap_data)
{
    SM_RAND_GENERATION_QUEUE_T **elpp, *elp;

    SM_SANITY_CHECK(pairing);

    for (elpp = &smcb.rand_generation; (elp=*elpp) != NULL; elpp = &elp->next)
    {
        if (elp->pairing == pairing)
        {
            /* Still waiting for enough randomness */
            if (elp->data != NULL)
                BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

            elp->data = pcopy(l2cap_data, SM_EVENT_PAIRING_REQUEST_LENGTH);
            return FALSE;
        }
    }

    return TRUE;
}

#endif /* SM_HAS_FUNCTION_FOR_RAND */
#endif /* INSTALL_SM_MODULE */
