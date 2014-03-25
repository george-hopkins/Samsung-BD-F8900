/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_read_multi_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_read_multi_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_READ_MULTI_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *      Ownership of block pointed to by handles passes to this function.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_read_multi_req(
    CsrSchedQid phandle,
    CsrUint16 cid,
    CsrUint16 size_handles,
    CsrUint16 *handles,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_READ_MULTI_REQ_T *prim = zpnew(ATT_READ_MULTI_REQ_T);

    prim->type = ATT_READ_MULTI_REQ;
    prim->phandle = phandle;
    prim->cid = cid;
    prim->size_handles = size_handles;
    prim->handles = handles;

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
