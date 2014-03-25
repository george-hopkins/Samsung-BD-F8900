/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_acl_open_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_acl_open_req
 *
 *  DESCRIPTION
 *      Build and send a DM_ACL_OPEN_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_ACL_OPEN
void dm_acl_open_req(
    TYPED_BD_ADDR_T *addrt,
    DM_UPRIM_T **pp_prim
    )
{
    DM_ACL_OPEN_REQ_T *p_prim = zpnew(DM_ACL_OPEN_REQ_T);

    p_prim->type = DM_ACL_OPEN_REQ;
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


#endif /* CSR_BT_EXCLUDE_ACL_OPEN */
