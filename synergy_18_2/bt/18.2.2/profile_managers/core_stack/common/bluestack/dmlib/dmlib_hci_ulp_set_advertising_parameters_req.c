/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_set_advertising_parameters_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_set_advertising_parameters_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_SET_ADVERTISING_PARAMETERS
void dm_hci_ulp_set_advertising_parameters_req(
    CsrUint16 adv_interval_min,
    CsrUint16 adv_interval_max,
    CsrUint8 advertising_type,
    CsrUint8 own_address_type,
    TYPED_BD_ADDR_T *direct_address,
    CsrUint8 advertising_channel_map,
    CsrUint8 advertising_filter_policy,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T *prim = zpnew(DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ_T);

    prim->common.op_code = DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ;
    prim->adv_interval_min = adv_interval_min;
    prim->adv_interval_max = adv_interval_max;
    prim->advertising_type = advertising_type;
    prim->own_address_type = own_address_type;
    if (direct_address != NULL)
        prim->direct_address_type = tbdaddr_copy_to_bd_addr(
                &prim->direct_address,
                direct_address);

    prim->advertising_channel_map = advertising_channel_map;
    prim->advertising_filter_policy = advertising_filter_policy;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_SET_ADVERTISING_PARAMETERS */
