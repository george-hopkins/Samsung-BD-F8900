/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_set_random_address_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_set_random_address_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_SET_RANDOM_ADDRESS
void dm_hci_ulp_set_random_address_req(
    BD_ADDR_T *random_address,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T *prim = zpnew(DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ_T);

    prim->common.op_code = DM_HCI_ULP_SET_RANDOM_ADDRESS_REQ;
    bd_addr_copy(&prim->random_address, random_address);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_SET_RANDOM_ADDRESS */
