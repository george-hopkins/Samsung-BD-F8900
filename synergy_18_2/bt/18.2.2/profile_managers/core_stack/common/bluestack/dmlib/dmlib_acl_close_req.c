/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_acl_close_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_acl_close_req
 *
 *  DESCRIPTION
 *      Build and send a DM_ACL_CLOSE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_ACL_CLOSE
void dm_acl_close_req(
    TYPED_BD_ADDR_T *addrt,
    CsrUint16 flags,
    CsrUint8 reason,
    DM_UPRIM_T **pp_prim
    )
{
    DM_ACL_CLOSE_REQ_T *p_prim = zpnew(DM_ACL_CLOSE_REQ_T);

    p_prim->type = DM_ACL_CLOSE_REQ;
    p_prim->flags = flags;
    p_prim->reason = reason;
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


#endif /* CSR_BT_EXCLUDE_ACL_CLOSE */
