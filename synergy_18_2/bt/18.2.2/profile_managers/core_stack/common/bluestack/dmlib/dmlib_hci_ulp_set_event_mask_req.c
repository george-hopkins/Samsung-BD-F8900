/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_set_event_mask_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_set_event_mask_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_SET_EVENT_MASK_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_SET_EVENT_MASK
void dm_hci_ulp_set_event_mask_req(
    hci_event_mask_t *ulp_event_mask,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_SET_EVENT_MASK_REQ_T *prim = zpnew(DM_HCI_ULP_SET_EVENT_MASK_REQ_T);

    prim->common.op_code = DM_HCI_ULP_SET_EVENT_MASK_REQ;
    CsrMemCpy(prim->ulp_event_mask, ulp_event_mask, 2 * sizeof(hci_event_mask_t));

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_SET_EVENT_MASK */
