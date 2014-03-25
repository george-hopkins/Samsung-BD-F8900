/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_read_clock.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_read_clock
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_READ_CLOCK_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_READ_CLOCK
void dm_hci_read_clock(
    CsrUint8 whichClock,
    BD_ADDR_T *p_bd_addr,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_READ_CLOCK_REQ_T *p_prim = zpnew(DM_HCI_READ_CLOCK_REQ_T);
    p_prim->common.op_code = DM_HCI_READ_CLOCK_REQ;
    p_prim->which_clock = whichClock;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_READ_CLOCK */
