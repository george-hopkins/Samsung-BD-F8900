/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_hold_mode.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_hold_mode
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_HOLD_MODE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_HOLD_MODE
void dm_hci_hold_mode(
    BD_ADDR_T *p_bd_addr,
    CsrUint16 max_interval,
    CsrUint16 min_interval,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_HOLD_MODE_REQ_T *p_prim = zpnew(DM_HCI_HOLD_MODE_REQ_T);

    p_prim->common.op_code = DM_HCI_HOLD_MODE_REQ;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->max_interval = max_interval;
    p_prim->min_interval = min_interval;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_HOLD_MODE */
