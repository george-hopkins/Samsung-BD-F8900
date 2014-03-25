/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_lp_write_roleswitch_policy_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_lp_write_roleswitch_policy_req
 *
 *  DESCRIPTION
 *      Build and send a DM_LP_WRITE_ROLESWITCH_POLICY_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

#ifndef CSR_BT_EXCLUDE_LP_WRITE_ROLESWITCH_POLICY
void dm_lp_write_roleswitch_policy_req(
    CsrUint16    version,
    CsrUint16    length,
    CsrUint16    *rs_table,
    DM_UPRIM_T  **pp_prim
    )
{
    DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T *prim =
        pnew(DM_LP_WRITE_ROLESWITCH_POLICY_REQ_T);

    prim->type = DM_LP_WRITE_ROLESWITCH_POLICY_REQ;
    prim->version = version;
    prim->length = length;
    prim->rs_table = rs_table;

    if (pp_prim == NULL)
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    else
        *pp_prim = (DM_UPRIM_T*)prim;
}

#endif /* CSR_BT_EXCLUDE_LP_WRITE_ROLESWITCH_POLICY */
