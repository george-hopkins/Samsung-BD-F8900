/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_add_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

#ifdef BUILD_FOR_HOST
/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_add_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_ADD_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_add_req(
    CsrSchedQid phandle,
    att_attr_t *attrs,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_ADD_REQ_T *prim = zpnew(ATT_ADD_REQ_T);

    prim->type = ATT_ADD_REQ;
    prim->phandle = phandle;
    prim->attrs = attrs;

    if (pp_prim)
    {
        *pp_prim = (ATT_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(ATT_IFACEQUEUE, ATT_PRIM, prim);
    }
}
#endif /* BUILD_FOR_HOST */

#endif /* INSTALL_ATT_MODULE */
