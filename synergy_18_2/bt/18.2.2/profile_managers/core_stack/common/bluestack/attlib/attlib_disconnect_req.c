/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_disconnect_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_disconnect_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_DISCONNECT_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_disconnect_req(
    CsrSchedQid phandle,
    CsrUint16 cid,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_DISCONNECT_REQ_T *prim = zpnew(ATT_DISCONNECT_REQ_T);

    prim->type = ATT_DISCONNECT_REQ;
    prim->phandle = phandle;
    prim->cid = cid;

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
