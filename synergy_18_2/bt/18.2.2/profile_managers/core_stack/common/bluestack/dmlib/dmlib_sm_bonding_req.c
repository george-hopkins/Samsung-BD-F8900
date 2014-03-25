/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_bonding_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_bonding_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_BONDING_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_BONDING
void dm_sm_bonding_req(
    TYPED_BD_ADDR_T *addrt,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_BONDING_REQ_T *p_prim = pnew(DM_SM_BONDING_REQ_T);
    p_prim->type                        = DM_SM_BONDING_REQ;
    p_prim->flags                       = 0;
    tbdaddr_copy(&p_prim->addrt, addrt);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_BONDING */
