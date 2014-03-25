/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_delete_stored_link_key.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_delete_stored_link_key
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_DELETE_STORED_LINK_KEY_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_DELETE_STORED_LINK_KEY
void dm_hci_delete_stored_link_key(
    BD_ADDR_T *p_bd_addr,       /* Optional, can be NULL */
    delete_all_flag_t flag,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_DELETE_STORED_LINK_KEY_REQ_T *p_prim = zpnew(DM_HCI_DELETE_STORED_LINK_KEY_REQ_T);

    p_prim->common.op_code = DM_HCI_DELETE_STORED_LINK_KEY_REQ;
    if (p_bd_addr)
    {
        bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    }
    p_prim->flag = flag;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_DELETE_STORED_LINK_KEY */
