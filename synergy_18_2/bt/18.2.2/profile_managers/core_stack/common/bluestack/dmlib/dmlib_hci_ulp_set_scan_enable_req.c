/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_set_scan_enable_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_set_scan_enable_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_SET_SCAN_ENABLE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_SET_SCAN_ENABLE
void dm_hci_ulp_set_scan_enable_req(
    CsrUint8 scan_enable,
    CsrUint8 filter_duplicates,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T *prim = zpnew(DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T);

    prim->common.op_code = DM_HCI_ULP_SET_SCAN_ENABLE_REQ;
    prim->scan_enable = scan_enable;
    prim->filter_duplicates = filter_duplicates;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_SET_SCAN_ENABLE */
