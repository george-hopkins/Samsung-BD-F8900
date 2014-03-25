/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_dynamic.c

\brief  Dynamic memory handling in SM.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE

/*! \brief Find Pointer to pairing structure by Bluetooth address.
    \param addrt Pointer to address.
    \returns Pointer to Pointer to pairing structure, or NULL if BLE-U doesn't exist.
             Note that if this return a pointer that isn't NULL, but dereferences to
             a NULL pointer then the BLE-U does exist but it currently doesn't have
             a pairing structure hanging from it.
*/
static SM_PAIRING_T **smpp_find_by_tbdaddr(TYPED_BD_ADDR_T *addrt)
{
    DM_ACL_T *acl;

    if ((acl = dm_acl_find_by_tbdaddr(addrt)) != NULL
            && dm_acl_is_ble(acl))
        return &acl->dm_acl_client_sm.u.p_sm;

    return NULL;
}

/*! \brief Find Pairing structure by Bluetooth address.
    \param addrt Pointer to address.
    \returns Pointer to pairing structure, or NULL if not found.
*/
SM_PAIRING_T *sm_find_by_tbdaddr(TYPED_BD_ADDR_T *addrt)
{
    SM_PAIRING_T **pp = smpp_find_by_tbdaddr(addrt);

    return pp == NULL ? NULL : *pp;
}

/*! \brief Find Pairing structure by handle.
    \param handle HCI connection handle.
    \returns Pointer to pairing structure, or NULL if not found.
*/
SM_PAIRING_T *sm_find_by_handle(hci_connection_handle_t handle)
{
    DM_ACL_T *acl;

    if ((acl = dm_acl_find_by_handle(handle)) != NULL
            && dm_acl_is_ble(acl))
        return acl->dm_acl_client_sm.u.p_sm;

    return NULL;
}

/*! \brief Find Pairing structure by L2CAP CID.
    \param cid L2CAP cid.
    \returns Pointer to pairing structure, or NULL if not found.
*/
SM_PAIRING_T *sm_find_by_cid(l2ca_cid_t cid)
{
    DM_ACL_T *acl;

    /* Loop over ACLs. */
    for (acl = DM_ACL_FIRST(); acl != NULL; acl = acl->p_next)
    {
        /* Only interested in BLE ones. */
        if (dm_acl_is_ble(acl))
        {
            /* Look for pairing structure associated with CID. */
            SM_PAIRING_T *p = acl->dm_acl_client_sm.u.p_sm;

            if (p != NULL)
            {
                SM_SANITY_CHECK(p);

                if (p->cid == cid)
                    return p;
            }
        }
    }

    return NULL;
}

/*! \brief Lock BLE-U open.
    \param addrt Pointer to Bluetooth address.
    \param flags L2CAP configuration flags.
    \returns TRUE unless BLE-U creation fails immediately.
*/
CsrBool sm_lock_acl(TYPED_BD_ADDR_T *addrt,
                   l2ca_conflags_t flags)
{
    return dm_acl_open(addrt,
                       NULL,
                       dm_acl_client_sm,
                       flags,
                       NULL) != DM_ACL_FAILED;
}

/*! \brief Release lock on BLE-U.
    \param addrt Pointer to Bluetooth address.
*/
void sm_unlock_acl(TYPED_BD_ADDR_T *addrt)
{
    DM_ACL_T *acl;

    if ((acl = dm_acl_find_by_tbdaddr(addrt)) != NULL)
        dm_acl_close(acl,
                     dm_acl_client_sm,
                     HCI_ERROR_OETC_USER,
                     ACL_IDLE_TIMEOUT_AT_LEAST_TINY);
}

/*! \brief Find/create pairing structure associated with given CID and address.
    \param addrt Pointer to Bluetooth address.
    \param cid L2CAP CID.
    \returns Pointer to pairing structure, or NULL if it could not be created.
*/
SM_PAIRING_T *sm_add(TYPED_BD_ADDR_T *addrt,
                     l2ca_cid_t cid)
{
    SM_PAIRING_T **pp;
    SM_PAIRING_T *p = NULL;

    if ((pp = smpp_find_by_tbdaddr(addrt)) != NULL)
    {
        if ((p = *pp) != NULL)
        {
            /* Found existing structure. */
            SM_SANITY_CHECK(p);

            /* Existing structure has invalid CID, so update. */
            if (SM_CID(p) == L2CA_CID_INVALID)
                SM_CID(p) = cid;

            /* L2CA_CID_INVALID should match all existing CIDs */
            if (cid == L2CA_CID_INVALID)
                cid = SM_CID(p);

            /* Check CID match. */
            if (SM_CID(p) != cid)
                p = NULL;

        }
        else if ((p = *pp = zpnew(SM_PAIRING_T)) != NULL)
        {
            /* No existing structure, so we've created a new one. */
#ifdef SM_DEBUG_PAIRING_SANITY
            p->sanity = 0xBEEF;
#endif
            SM_CID(p) = cid;
        }
    }

    return p;
}

/*! \brief Try to find the ACL structure associated with a pairing structure
           and fail softly.
    \param pairing Pointer to pairing structure.
*/
static DM_ACL_T *sm_acl_from_pairing_no_panic(SM_PAIRING_T *pairing)
{
    if (pairing != NULL)
    {
        DM_ACL_T *acl;

        SM_SANITY_CHECK(pairing);

        /* Loop over ACL list */
        for (acl = DM_ACL_FIRST(); acl != NULL; acl = acl->p_next)
        {
            /* Check pairing structure */
            if (dm_acl_is_ble(acl) && pairing == acl->dm_acl_client_sm.u.p_sm)
                return acl;
        }
    }

    return NULL;
}

/*! \brief Free a pairing structure, cleaning up the pointer from the
           ACL structure if we can find it.
    \param pairing Pointer to pairing structure.
*/
void sm_free(SM_PAIRING_T *pairing)
{
    if (pairing != NULL)
    {
        DM_ACL_T *acl;

        SM_SANITY_CHECK(pairing);

        /* Tidy up tracking of HCI_ULP_ENCRYPT or HCI_ULP_RAND
           associated with this pairing instance. */
        sm_aes_pairing_freed(pairing);
        sm_rand_pairing_freed(pairing);

        /* Clean up pointer from ACL structure if we can find it. */
        if ((acl = sm_acl_from_pairing_no_panic(pairing)) != NULL)
            acl->dm_acl_client_sm.u.p_sm = NULL;

        /* Clean up and free pairing structure. */
        dm_free_upstream_primitive((DM_UPRIM_T*)(pairing->keys_ind));
        dm_free_upstream_primitive((DM_UPRIM_T*)(pairing->security_cfm));
        timer_cancel(&pairing->sm_timeout);
        CsrPmemFree(pairing);
    }
}

/*! \brief ACL has died, so clear away pairing structure.
    \param pairing Pointer to pairing structure.
*/
void sm_acl_closed(SM_PAIRING_T *pairing)
{
    if (pairing != NULL)
    {
        SM_SANITY_CHECK(pairing);

        if (SM_CID(pairing) != L2CA_CID_INVALID)
            sm_fixed_cid_unmapped(SM_CID(pairing));
    }
}

/*! \brief Find ACL from pairing structure, failing hard.
    \param pairing Pointer to pairing structure.
*/
DM_ACL_T *sm_acl_from_pairing(SM_PAIRING_T *pairing)
{
    DM_ACL_T *acl;

    if ((acl = sm_acl_from_pairing_no_panic(pairing)) == NULL)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

    return acl;
}

#ifdef ENABLE_SHUTDOWN
/*! \brief Synergy deinit. */
void sm_deinit(void)
{
#ifndef SM_HAS_FUNCTION_FOR_RAND
    while (smcb.rand_generation != NULL)
    {
        SM_RAND_GENERATION_QUEUE_T *temp = smcb.rand_generation;
        smcb.rand_generation = smcb.rand_generation->next;
        CsrPmemFree(temp->data);
        CsrPmemFree(temp);
    }
#endif

    sm_data_sigining_deinit();
    sm_aes_deinit();
}
#endif /* ENABLE_SHUTDOWN */

#endif /* INSTALL_SM_MODULE */
