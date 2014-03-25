/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_create_connection_cancel.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_create_connection_cancel
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_CREATE_CONNECTION_CANCEL_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_CREATE_CONNECTION_CANCEL
void dm_hci_create_connection_cancel(
    BD_ADDR_T *p_bd_addr,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T *p_prim = zpnew(DM_HCI_CREATE_CONNECTION_CANCEL_REQ_T);

    p_prim->common.op_code = DM_HCI_CREATE_CONNECTION_CANCEL_REQ;
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


#endif /* CSR_BT_EXCLUDE_HCI_CREATE_CONNECTION_CANCEL */
