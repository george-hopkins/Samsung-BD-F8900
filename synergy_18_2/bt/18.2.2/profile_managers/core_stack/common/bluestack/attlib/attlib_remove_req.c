/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_remove_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

#ifdef BUILD_FOR_HOST
/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_remove_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_REMOVE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_remove_req(
    CsrSchedQid phandle,
    CsrUint16 start,
    CsrUint16 end,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_REMOVE_REQ_T *prim = zpnew(ATT_REMOVE_REQ_T);

    prim->type = ATT_REMOVE_REQ;
    prim->phandle = phandle;
    prim->start = start;
    prim->end = end;

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
