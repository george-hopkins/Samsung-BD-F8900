/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_prepare_write_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_prepare_write_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_PREPARE_WRITE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *      Takes ownership of block pointed to by value.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_prepare_write_req(
    CsrSchedQid phandle,
    CsrUint16 cid,
    CsrUint16 handle,
    CsrUint16 offset,
    CsrUint16 size_value,
    CsrUint8 *value,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_PREPARE_WRITE_REQ_T *prim = zpnew(ATT_PREPARE_WRITE_REQ_T);

    prim->type = ATT_PREPARE_WRITE_REQ;
    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->offset = offset;
    prim->size_value = size_value;
    prim->value = value;

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
