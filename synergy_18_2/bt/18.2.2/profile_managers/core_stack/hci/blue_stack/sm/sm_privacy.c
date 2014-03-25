/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_privacy.c

\brief  Privacy.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE
#ifndef DISABLE_SM_PRIVACY
#ifdef SM_HAS_FUNCTION_FOR_AES
/*! \brief Generate hash part of resolvable private address.
    \param addrt Pointer to address.
    \param irk Pointer to irk.
*/
static void sm_privacy_hash(TYPED_BD_ADDR_T *addrt, const CsrUint16 *irk)
{
    CsrUint16 data[SM_SIZE16_128];

    data[0] = TBDADDR_UAP(*addrt) | ((TBDADDR_NAP(*addrt) & 0xFF) << 8);
    data[1] = (TBDADDR_NAP(*addrt) >> 8) & 0xFF;
    CsrMemSet(&data[2], 0, sizeof(data) - 2);

    sm_aes_encrypt_128(data, data, irk);

    TBDADDR_LAP(*addrt) = ((data[1] >> 8) & 0xFF) | (((CsrUint24)data[2]) << 8);
}

/*! \brief Generate hash part of local resolvable private address.
    First we get the IRK, then we call sm_privacy_hash() to generate
    the hash part of the address.
    \param addrt Pointer to address.
    \param addr random part of address.
*/
void sm_privacy_generate_hash_local(TYPED_BD_ADDR_T *addrt, CsrUint8 *addr)
{
    CsrUint16 irk[SM_SIZE16_128];

    /* Recreate IRK */
    SM_IRK(irk);

    /* Copy random part into address. */
    TBDADDR_UAP(*addrt) = addr[0];
    TBDADDR_NAP(*addrt) = addr[1] | ((CsrUint16)addr[2] << 8);

    /* Use IRK to calculate hash from random part. */
    sm_privacy_hash(addrt, irk);
}

/*! \brief Check a resolvable random address against an IRK
    \param irk Pointer to IRK.
    \param current_addrt Pointer to current address.
*/
CsrBool sm_privacy_check_hash(const CsrUint16 *irk,
                             const TYPED_BD_ADDR_T *current_addrt)
{
    TYPED_BD_ADDR_T reconstructed_addrt;

    tbdaddr_copy(&reconstructed_addrt, current_addrt);

    sm_privacy_hash(&reconstructed_addrt, irk);

    return tbdaddr_eq(&reconstructed_addrt, current_addrt);
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
        TYPED_BD_ADDR_T *addrt;

        if ((addrt  = dm_sm_resolve_address(&prim->permanent_addrt)) != NULL)
            tbdaddr_copy(&prim->permanent_addrt, addrt);
    }
    else if (!TBDADDR_IS_PRIVATE_NONRESOLVABLE(prim->current_addrt))
    {
        /* We assume that everything that is not private resolvable
           and notprivate nonresolvable is a permanent address that
           can just be copied. */
        tbdaddr_copy(&prim->permanent_addrt, &prim->current_addrt);
    }
}
#endif /* SM_HAS_FUNCTION_FOR_AES */
#endif /* !DISABLE_SM_PRIVACY */
#endif /* INSTALL_SM_MODULE */
