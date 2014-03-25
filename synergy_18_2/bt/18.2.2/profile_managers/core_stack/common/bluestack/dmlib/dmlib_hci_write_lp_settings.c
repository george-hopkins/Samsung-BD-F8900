/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_write_lp_settings.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_write_lp_settings
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_WRITE_LP_SETTINGS primitive, changing only the LM
 *      link policy settings.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_WRITE_LINK_POLICY_SETTINGS
void dm_hci_write_lp_settings(
    BD_ADDR_T *p_bd_addr,
    link_policy_settings_t link_policy_settings,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T *p_prim = zpnew(DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ_T);

    p_prim->common.op_code = DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->link_policy_settings = link_policy_settings;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_WRITE_LINK_POLICY_SETTINGS */
