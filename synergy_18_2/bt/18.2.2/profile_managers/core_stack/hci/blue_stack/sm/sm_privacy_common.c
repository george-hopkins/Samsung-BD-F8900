/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_privacy_common.c

\brief  Privacy.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE
static TYPED_BD_ADDR_T static_address;
static void sm_privacy_set_random_address(TYPED_BD_ADDR_T *addrt);

#ifndef DISABLE_SM_PRIVACY

static void resolvable_random_address_change(CsrUint16 arg1, void *arg2);
static CsrBool sm_privacy_check_resolvable_random(CsrUint8 *random);

static DM_SM_PRIVACY_T sm_privacy = DM_SM_PRIVACY_DISABLED;
static TYPED_BD_ADDR_T resolvable_random_address;
static TYPED_BD_ADDR_T static_address;
static CsrSchedTid address_renew_countdown;

#ifndef SM_HAS_FUNCTION_FOR_RAND
#define SM_PRIVACY_RAND_BLOCK_SIZE 8
#define SM_PRIVACY_RAND_NUM_BLOCKS 8
#define SM_PRIVACY_RAND_SIZE \
    (SM_PRIVACY_RAND_BLOCK_SIZE * SM_PRIVACY_RAND_NUM_BLOCKS)
CsrUint16 sm_privacy_rands[SM_PRIVACY_RAND_SIZE];
CsrUint8 sm_privacy_rands_quantity;

/*! \brief Initialise random number cache. */
static void sm_privacy_rands_init(void)
{
    sm_privacy_rands_quantity = 0;
    sm_rand_generate(SM_PRIVACY_RAND_NUM_BLOCKS + 1);
}

/*! \brief New random number just been generated.
           We store it in the cache.
    \param random Pointer to random number.
    \returns NULL or a pointer to let the caller copy the random number for us.
*/
CsrUint16 *sm_privacy_rand_complete(const CsrUint8 *random)
{
    CsrUint16 *ret = NULL;

    if (sm_privacy_rands_quantity <= SM_PRIVACY_RAND_SIZE - SM_PRIVACY_RAND_BLOCK_SIZE)
    {
        /* Random number gets added to cache. */
        ret = &sm_privacy_rands[sm_privacy_rands_quantity];

        sm_privacy_rands_quantity += SM_PRIVACY_RAND_BLOCK_SIZE;
    }
    else if (address_renew_countdown == 0)
    {
        /* Random number used to generate new resolvable random address. */
        CsrUint8 *addr = pcopy(random, 6);

        if (sm_privacy_check_resolvable_random(addr))
        {
            /* Valid so generate hash. */
#ifdef SM_HAS_FUNCTION_FOR_AES
            sm_privacy_generate_hash_local(&resolvable_random_address, addr);

            if (sm_privacy_is_enabled())
            {
                sm_privacy_set_random_address(&resolvable_random_address);
                dm_ble_advertising_local_address_changed(HCI_ULP_ADDRESS_RANDOM);
                timer_start(&address_renew_countdown,
                            20 * CSR_SCHED_MINUTE,
                            resolvable_random_address_change,
                            0,
                            NULL);
            }
#else
            sm_aes_generate_hash(addr);
#endif
        }
        else
        {
            /* Invalid, so try again. */
            CsrPmemFree(addr);
            sm_rand_generate(1);
        }
    }

    return ret;
}

/*! \brief Read a random number from the cache.
    \param dst Where to write the number we read.
    \param count How many CsrUint16s to read.
*/
void sm_privacy_rand_read(CsrUint16 *dst, CsrUint8 count)
{
    CsrUint8 num_blocks = sm_privacy_rands_quantity / SM_PRIVACY_RAND_BLOCK_SIZE;

    /* Read random number. */
    while (count != 0)
    {
        if (sm_privacy_rands_quantity == 0)
        {
            /* We cannot keep up with random number demand. TODO */
            break;
        }
        else
            dst[--count] = sm_privacy_rands[--sm_privacy_rands_quantity];
    }

    /* Replenish our stocks. */
    for(num_blocks -= sm_privacy_rands_quantity / SM_PRIVACY_RAND_BLOCK_SIZE;
            num_blocks != 0;
            num_blocks--)
    {
        sm_rand_generate(SM_PRIVACY_RAND_NUM_BLOCKS);
    }
}

/*! \brief Timer function that fires when we need a new resolvable random address.
    \param arg1 Unused.
    \param arge Unused.
*/
static void resolvable_random_address_change(CsrUint16 arg1, void *arg2)
{
    TIMER_EXPIRED(address_renew_countdown);
    sm_rand_generate(1);
}
#else
#define sm_privacy_rands_init() ((void)0)

/*! \brief Timer function that fires when we need a new resolvable random address.
    \param arg1 Unused.
    \param arge Unused.
*/
static void resolvable_random_address_change(CsrUint16 arg1, void *arg2)
{
    CsrUint8 rand8[6];

    TIMER_EXPIRED(address_renew_countdown);

    /* Generate random numbers until we get a valid address. */
    do
    {
        CsrUint16 rand16[3];
        random_number(rand16, 3);
        CsrBtMemCpyUnpack(rand8, rand16, 3);
    } while (!sm_privacy_check_resolvable_random(rand8));

    /* Generate hash part. */
#ifdef SM_HAS_FUNCTION_FOR_AES
    sm_privacy_generate_hash_local(&resolvable_random_address, rand8);

    if (sm_privacy_is_enabled())
    {
        sm_privacy_set_random_address(&resolvable_random_address);
        dm_ble_advertising_local_address_changed(HCI_ULP_ADDRESS_RANDOM);
    }

    timer_start(&address_renew_countdown,
                20 * CSR_SCHED_MINUTE,
                resolvable_random_address_change,
                0,
                NULL);
#else
    sm_aes_generate_hash(pcopy(rand8,sizeof(rand8)));
#endif
}
#endif /* !SM_HAS_FUNCTION_FOR_RAND */

/*! \brief Check validity of random part of resolvable random address
           and set type bits appropriately.
    \param random Random part of resolvable random address.
    \returns TRUE if valid, otherwise FALSE.
*/
static CsrBool sm_privacy_check_resolvable_random(CsrUint8 *random)
{
    if (random[3] != random[4]
            || (random[3] & 0x3F) != (random[5] & 0x3F)
            || (random[3] != 0 && random[3] == 0xFF))
    {
        random[5] = (CsrUint8)((random[5] & 0x3F) |
                              (TBDADDR_RANDOM_NAP_TYPE_PRIVATE_RESOLVABLE >> 8));
        return TRUE;
    }

    return FALSE;
}

/*! \brief Work out whether we are currently using a public or random address.
    \returns address type.
*/
CsrUint8 sm_privacy_address_type(void)
{
    if (sm_privacy_is_enabled() || TBDADDR_IS_STATIC(static_address))
        return HCI_ULP_ADDRESS_RANDOM;

    return HCI_ULP_ADDRESS_PUBLIC;
}

/*! \brief Determine whether privacy is enabled.
    \returns TRUE if it is, otherwise FALSE.
*/
CsrBool sm_privacy_is_enabled(void)
{
    return sm_privacy == DM_SM_PRIVACY_ENABLED;
}

#ifndef SM_HAS_FUNCTION_FOR_AES
/*! \brief Store freshly-generated resolvable random address.
    \param addr Pointer to address stored in array.
*/
void sm_privacy_resolvable_random_address(CsrUint8 *addr)
{
    /* Sanity check. If the timer is running then it's not yet
       time to change the address. So stick with the one we've got. */
    if (address_renew_countdown != 0)
        return;

    /* If we failed to generate an address then try again. */
    if (addr == NULL)
    {
        resolvable_random_address_change(0, NULL);
        return;
    }

    /* Store the address. */
    TBDADDR_TYPE(resolvable_random_address) = TBDADDR_RANDOM;
    TBDADDR_LAP(resolvable_random_address) = addr[0] \
            | ((CsrUint24)addr[1] << 8) | ((CsrUint24)addr[2] << 16);
    TBDADDR_UAP(resolvable_random_address) = addr[3];
    TBDADDR_NAP(resolvable_random_address) = addr[4] | ((CsrUint16)addr[5] << 8);

    /* Restart the address refresh timer. */
    timer_start(&address_renew_countdown,
                20 * CSR_SCHED_MINUTE,
                resolvable_random_address_change,
                0,
                NULL);

    /* Update advertising. */
    if (sm_privacy_is_enabled())
    {
        sm_privacy_set_random_address(&resolvable_random_address);
        dm_ble_advertising_local_address_changed(HCI_ULP_ADDRESS_RANDOM);
    }
}
#endif /* SM_HAS_FUNCTION_FOR_AES */
#endif /* !DISABLE_SM_PRIVACY */

/*! \brief Write random address to controller.
    \param addrt Pointer to address to be written.
*/
static void sm_privacy_set_random_address(TYPED_BD_ADDR_T *addrt)
{
    HCI_ULP_SET_RANDOM_ADDRESS_T *prim = zpnew(HCI_ULP_SET_RANDOM_ADDRESS_T);

    prim->common.op_code = HCI_ULP_SET_RANDOM_ADDRESS;
    tbdaddr_copy_to_bd_addr(&prim->random_address, addrt);

    send_to_hci((DM_UPRIM_T*)prim);
}

/*! \brief Generate a non-resolvable private address.
    \param addrt Place to write the address. If it's NULL then write it to the controller.
*/
void sm_privacy_generate_nonresolvable_private_address(TYPED_BD_ADDR_T *addrt)
{
    TYPED_BD_ADDR_T temp;

    if (addrt == NULL)
        addrt = &temp;

    /* Generate addresses until we get one that's valid */
    do
    {
        CsrUint16 rand[3];

        sm_privacy_rand_read(rand, 3);
        TBDADDR_LAP(*addrt) = rand[0] | (((CsrUint24)(rand[1] & 0xFF)) << 16);
        TBDADDR_UAP(*addrt) = (rand[1] >> 8) & 0xFF;
        TBDADDR_NAP(*addrt) = rand[2] & 0x3FFF;
    } while (!TBDADDR_IS_VALID(*addrt));

    /* Write the address. */
    if (addrt == &temp)
        sm_privacy_set_random_address(&temp);
}

/*! \brief Handle primitive to configure local address type and enable/disable
           privacy.
    \param uprim Pointer to DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ cast to (DM_UPRIM_T*)
*/
void sm_handle_configure_local_address_req(DM_UPRIM_T *const uprim)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T *prim;

    /* Turn privacy on or off */
#ifndef DISABLE_SM_PRIVACY
    if (uprim->dm_sm_configure_local_address_req.privacy == DM_SM_PRIVACY_DISABLED
            || uprim->dm_sm_configure_local_address_req.privacy == DM_SM_PRIVACY_ENABLED)
    {
        sm_privacy = uprim->dm_sm_configure_local_address_req.privacy;
    }
#endif

    /* Work out the type of our permanent address. */
    switch (uprim->dm_sm_configure_local_address_req.set_permanent_address_type)
    {
        case DM_SM_PERMANENT_ADDRESS_UNCHANGED:
            break;

        case DM_SM_PERMANENT_ADDRESS_PUBLIC:
            TBDADDR_INVALIDATE(static_address);
            break;

        case DM_SM_PERMANENT_ADDRESS_WRITE_STATIC:
            if (TBDADDR_IS_STATIC(uprim->dm_sm_configure_local_address_req.static_addrt))
            {
                tbdaddr_copy(&static_address,
                             &uprim->dm_sm_configure_local_address_req.static_addrt);
                break;
            }

            /* Fallthrough */
        case DM_SM_PERMANENT_ADDRESS_GENERATE_STATIC:
            sm_privacy_generate_nonresolvable_private_address(&static_address);
            TBDADDR_NAP(static_address) =
                    (TBDADDR_NAP(static_address) & ~TBDADDR_RANDOM_NAP_TYPE_MASK)
                    | TBDADDR_RANDOM_NAP_TYPE_STATIC;
            break;
    }

    /* Pepare response to application. */
    prim = zpnew(DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM_T);
    prim->type = DM_SM_CONFIGURE_LOCAL_ADDRESS_CFM;
    prim->status = HCI_SUCCESS;
    prim->permanent_address_type = HCI_ULP_ADDRESS_RANDOM;
    TBDADDR_INVALIDATE(prim->static_addrt);

#ifdef DISABLE_SM_PRIVACY
    prim->privacy = DM_SM_PRIVACY_DISABLED;
#else
    prim->privacy = sm_privacy;
    if (sm_privacy_is_enabled())
    {
        /* Privacy is enabled so make sure timer is running. */
        if (address_renew_countdown == 0)
            resolvable_random_address_change(0, NULL);
    }
    else
#endif
    {
        /* Privacy is disabled. */
        if (TBDADDR_IS_STATIC(static_address))
        {
            /* Return static address in primitive. */
            sm_privacy_set_random_address(&static_address);
            tbdaddr_copy(&prim->static_addrt, &static_address);
        }
        else
        {
            /* Using public address, so just return the type. */
            prim->permanent_address_type = HCI_ULP_ADDRESS_PUBLIC;
        }
    }

    /* Update advertising */
    dm_ble_advertising_local_address_changed(prim->permanent_address_type);

    DM_SendMessageAMphandle(prim);
}

/*! \brief Handle request from application to generate nonresolvable random address.
    \param uprim Pointer to primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_generate_nonresolvable_private_address_req(
                DM_UPRIM_T *const uprim)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T *prim
        = pnew(DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T);

    prim->type = DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM;
#ifdef DISABLE_SM_PRIVACY
    prim->status = HCI_ERROR_UNSUPPORTED_FEATURE;
#else
    prim->status = HCI_SUCCESS;
    sm_privacy_generate_nonresolvable_private_address(&prim->addrt);
#endif

    DM_SendMessageAMphandle(prim);
}

/*! \brief Initialise privacy globals */
void sm_privacy_init(void)
{
    TBDADDR_INVALIDATE(static_address);
#ifndef DISABLE_SM_PRIVACY
    TBDADDR_INVALIDATE(resolvable_random_address);
    TIMER_EXPIRED(address_renew_countdown);
    sm_privacy = DM_SM_PRIVACY_DISABLED;
    sm_privacy_rands_init();
#endif
}

#endif /* INSTALL_SM_MODULE */
