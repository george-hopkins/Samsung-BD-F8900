/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_privacy_aes_hci.c

\brief  Privacy.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE
#ifndef DISABLE_SM_PRIVACY
#ifndef SM_HAS_FUNCTION_FOR_AES

/*! \brief Global to store state while resolving addresses in adverts. */
struct
{
    DM_HCI_ULP_ADVERTISING_REPORT_IND_T *ind;
    CsrUint8 identifier;
    CsrUint16 remaining;
} smcb_privacy_aes_hci;

/*! \brief Called from loop through all stored IRKs. Will start
           reconstructing the address hash using the IRK.
    \param irk IRK used to recreate hash.
    \param current_addrt Pointer to address we're trying to check.
    \param permanent_addrt If current_addrt resolves correctly then
                           this is the address we'll write to the advert.
    \returns TRUE.
*/
CsrBool sm_privacy_check_hash(const CsrUint16 *irk,
                             const TYPED_BD_ADDR_T *current_addrt,
                             const TYPED_BD_ADDR_T *permanent_addrt)
{
    ++smcb_privacy_aes_hci.remaining;
    sm_aes_check_hash(irk,
                      current_addrt,
                      permanent_addrt,
                      smcb_privacy_aes_hci.identifier);
    return TRUE;
}

/*! \brief Called after the AES function returns.
    \param current_addrt The address we've been trying to resolve.
    \param permanent_addrt The address we need to use if it has resolved.
    \param identifier Opaque context to be matched.
    \param success TRUE is the address did resolve.
*/
void sm_privacy_aes_hci(const TYPED_BD_ADDR_T *current_addrt,
                        const TYPED_BD_ADDR_T *permanent_addrt,
                        CsrUint8 identifier,
                        CsrBool success)
{
    if (identifier == smcb_privacy_aes_hci.identifier
            && smcb_privacy_aes_hci.ind != NULL
            && tbdaddr_eq(current_addrt, &smcb_privacy_aes_hci.ind->current_addrt))
    {
        if (success)
        {
            tbdaddr_copy(&smcb_privacy_aes_hci.ind->permanent_addrt, permanent_addrt);
            smcb_privacy_aes_hci.remaining = 0;
        }

        if (smcb_privacy_aes_hci.remaining == 0
                || --smcb_privacy_aes_hci.remaining == 0)
        {
            DM_SendMessageAMphandle(smcb_privacy_aes_hci.ind);
            smcb_privacy_aes_hci.ind = NULL;
        }
    }
}

/*! \brief Called from dm_hci_interface to handle adverts received.
           We try to work out what to write to the permanent_address
           field of the advert. Once we're done, we send the advert
           to the host.
    \param prim Advert primitive.
*/
void sm_handle_advertising_report(DM_HCI_ULP_ADVERTISING_REPORT_IND_T *prim)
{
    TBDADDR_INVALIDATE(prim->permanent_addrt);

    if (TBDADDR_IS_PRIVATE_RESOLVABLE(prim->current_addrt))
    {
        if (smcb_privacy_aes_hci.ind != NULL)
        {
            /* We're already busy trying to resolve an advert, so throw this
               new one away. */
            CsrPmemFree(prim->data);
            CsrPmemFree(prim);
            return;
        }
        smcb_privacy_aes_hci.remaining = 0;
        smcb_privacy_aes_hci.identifier = (smcb_privacy_aes_hci.identifier + 1) & 0xFF;
        if (dm_sm_resolve_address(&prim->permanent_addrt) != NULL)
        {
            /* Must wait for HCI AES commands to return. */
            smcb_privacy_aes_hci.ind = prim;
            return;
        }
    }
    else if (!TBDADDR_IS_PRIVATE_NONRESOLVABLE(prim->current_addrt))
    {
        /* We assume that everything that is not private resolvable
           and notprivate nonresolvable is a permanent address that
           can just be copied. */
        tbdaddr_copy(&prim->permanent_addrt, &prim->current_addrt);
    }

    DM_SendMessageAMphandle(prim);
}

#endif /* !SM_HAS_FUNCTION_FOR_AES */
#endif /* !DISABLE_SM_PRIVACY */
#endif /* INSTALL_SM_MODULE  */
