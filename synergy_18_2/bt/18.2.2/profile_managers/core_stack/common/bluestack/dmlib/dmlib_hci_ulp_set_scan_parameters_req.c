/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_set_scan_parameters_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_set_scan_parameters_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_SET_SCAN_PARAMETERS
void dm_hci_ulp_set_scan_parameters_req(
    CsrUint8 scan_type,
    CsrUint16 scan_interval,
    CsrUint16 scan_window,
    CsrUint8 own_address_type,
    CsrUint8 scanning_filter_policy,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T *prim = zpnew(DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T);

    prim->common.op_code = DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ;
    prim->scan_type = scan_type;
    prim->scan_interval = scan_interval;
    prim->scan_window = scan_window;
    prim->own_address_type = own_address_type;
    prim->scanning_filter_policy = scanning_filter_policy;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_SET_SCAN_PARAMETERS */
