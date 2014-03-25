/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_access_rsp.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_access_rsp
 *
 *  DESCRIPTION
 *      Build and send an ATT_ACCESS_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *      This will take ownership of the block pointed to by value.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_access_rsp(
    CsrSchedQid phandle,
    CsrUint16 cid,
    CsrUint16 handle,
    att_result_t result,
    CsrUint16 size_value,
    CsrUint8 *value,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_ACCESS_RSP_T *prim = zpnew(ATT_ACCESS_RSP_T);

    prim->type = ATT_ACCESS_RSP;
    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->result = result;
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
