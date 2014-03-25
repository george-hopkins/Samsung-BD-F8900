/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_addresses.c

\brief  Random address handling.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE

#ifdef INSTALL_SM_PRIVACY
/*! \brief Send DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM to app.
    \param status Indication of success or failure.
    \param bd_addr Pointer to Bluetooth address.
*/
static void sm_generate_nonresolvable_private_address_cfm(
        CsrUint8 status,
        BD_ADDR_T *bd_addr)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T *prim
        = pnew(DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM_T);

    prim->type = DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_CFM;
    prim->status = status;
    bd_addr_copy(&prim->bd_addr, bd_addr);

    DM_SendMessageAMphandle(prim);
}

/*! \brief Turn CsrUint16 array into random address.
    \param addrt Pointer to typed Bluetooth address.
    \param a CsrUint16 array containing address data.
*/
static void sm_copy_random_address_from_array(TYPED_BD_ADDR_T *addrt, const CsrUint16 *a)
{
    TBDADDR_LAP(*addrt) = a[0] | ((CsrUint24)(a[1] & 0xFF) << 16);
    TBDADDR_UAP(*addrt) = (CsrUint8)((a[1] & 0xFFFF) >> 8);
    TBDADDR_NAP(*addrt) = a[2] & 0x3FFF;
    TBDADDR_TYPE(*addrt) = TBDADDR_RANDOM;
}

/*! \brief Handle application request to generate nonresolvable private address
    \param uprim Pointer to request primitive cast to (DM_UPRIM_T*).
*/
void sm_handle_dm_sm_generate_nonresolvable_private_address_req(
        DM_UPRIM_T *const uprim)
{
#ifdef SM_HAS_FUNCTION_FOR_RAND
    TYPED_BD_ADDR_T addrt;

    /* Generate random numbers until they produce a valid address. */
    do
    {
        CsrUint16 a[SM_SIZE16_BD_ADDR];
        random_number(a, SM_SIZE16_BD_ADDR);
        sm_copy_random_address_from_array(addrt, a);
    } while (!tbdaddr_is_valid(addrt))

    /* Report back to application. */
    dm_sm_generate_nonresolvable_private_address_cfm(
            HCI_SUCCESS,
            TBDADDR_ADDR(tbdaddr));
#endif /* SM_HAS_FUNCTION_FOR_RAND */
}
#endif /* INSTALL_SM_PRIVACY */

#endif /* INSTALL_SM_MODULE */
