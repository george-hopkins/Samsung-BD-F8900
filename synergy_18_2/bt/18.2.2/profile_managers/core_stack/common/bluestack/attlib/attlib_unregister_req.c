/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_unregister_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_unregister_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_UNREGISTER_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_unregister_req(
    CsrSchedQid phandle,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_UNREGISTER_REQ_T *prim = zpnew(ATT_UNREGISTER_REQ_T);

    prim->type = ATT_UNREGISTER_REQ;
    prim->phandle = phandle;

    if (pp_prim)
    {
        *pp_prim = (ATT_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(ATT_IFACEQUEUE, ATT_PRIM, prim);
    }
}

#endif /* INSTALL_ATT_MODULE */
