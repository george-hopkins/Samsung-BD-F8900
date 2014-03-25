/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_register_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_register_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_REGISTER_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_register_req(
    CsrSchedQid phandle,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_REGISTER_REQ_T *prim = zpnew(ATT_REGISTER_REQ_T);

    prim->type = ATT_REGISTER_REQ;
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
