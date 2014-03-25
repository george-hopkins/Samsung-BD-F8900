/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_authenticate_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_authenticate_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_AUTHENTICATE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_AUTHENTICATE
void dm_sm_authenticate_req(
    BD_ADDR_T *p_bd_addr,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_AUTHENTICATE_REQ_T *p_prim = pnew(DM_SM_AUTHENTICATE_REQ_T);

    p_prim->type = DM_SM_AUTHENTICATE_REQ;
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


#endif /* CSR_BT_EXCLUDE_SM_AUTHENTICATE */
