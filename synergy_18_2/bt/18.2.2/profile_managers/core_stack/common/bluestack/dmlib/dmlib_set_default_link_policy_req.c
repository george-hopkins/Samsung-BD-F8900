/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_set_default_link_policy_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_set_default_link_policy
 *
 *  DESCRIPTION
 *      Build and send a DM_SET_DEFAULT_LINK_POLICY_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SET_DEFAULT_LINK_POLICY
void dm_set_default_link_policy_req(
    link_policy_settings_t default_lp_in,
    link_policy_settings_t default_lp_out,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SET_DEFAULT_LINK_POLICY_REQ_T *p_prim = pnew(DM_SET_DEFAULT_LINK_POLICY_REQ_T);

    p_prim->type = DM_SET_DEFAULT_LINK_POLICY_REQ;
    p_prim->link_policy_settings_in = default_lp_in;
    p_prim->link_policy_settings_out = default_lp_out;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SET_DEFAULT_LINK_POLICY */
