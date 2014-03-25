/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_read_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_read_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_READ_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_read_req(
    CsrSchedQid phandle,
    CsrUint16 cid,
    CsrUint16 handle,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_READ_REQ_T *prim = zpnew(ATT_READ_REQ_T);

    prim->type = ATT_READ_REQ;
    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;

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
