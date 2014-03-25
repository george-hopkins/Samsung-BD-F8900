/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_set_advertising_data_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_set_advertising_data_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_SET_ADVERTISING_DATA_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *      It is the responsibility of the caller to ensure that
 *      advertising_data_len does not exceed the maximum (31).
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_SET_ADVERTISING_DATA
void dm_hci_ulp_set_advertising_data_req(
    CsrUint8 advertising_data_len,
    CsrUint8 *advertising_data,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T *prim = zpnew(DM_HCI_ULP_SET_ADVERTISING_DATA_REQ_T);

    prim->common.op_code = DM_HCI_ULP_SET_ADVERTISING_DATA_REQ;
    prim->advertising_data_len = advertising_data_len;
    CsrMemCpy(prim->advertising_data,
           advertising_data,
           advertising_data_len * sizeof(CsrUint8));

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_SET_ADVERTISING_DATA */
