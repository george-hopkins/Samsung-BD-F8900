/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_write_inquiryscan_activity.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_write_inquiryscan_activity
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_WRITE_INQUIRYSCAN_ACTIVITY
void dm_hci_write_inquiryscan_activity(
    CsrUint16 inqscan_interval,
    CsrUint16 inqscan_window,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T *p_prim = zpnew(DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ_T);

    p_prim->common.op_code = DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_REQ;
    p_prim->inqscan_interval = inqscan_interval;
    p_prim->inqscan_window = inqscan_window;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_WRITE_INQUIRYSCAN_ACTIVITY */
