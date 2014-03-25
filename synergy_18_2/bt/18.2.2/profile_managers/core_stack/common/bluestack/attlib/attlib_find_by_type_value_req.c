/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_find_by_type_value_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_find_by_type_value_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_FIND_BY_TYPE_VALUE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *      This will take ownership of the block pointed to by value.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_find_by_type_value_req(
    CsrSchedQid phandle,
    CsrUint16 cid,
    CsrUint16 start,
    CsrUint16 end,
    CsrUint16 uuid,
    CsrUint16 size_value,
    CsrUint8 *value,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_FIND_BY_TYPE_VALUE_REQ_T *prim = zpnew(ATT_FIND_BY_TYPE_VALUE_REQ_T);

    prim->type = ATT_FIND_BY_TYPE_VALUE_REQ;
    prim->phandle = phandle;
    prim->cid = cid;
    prim->start = start;
    prim->end = end;
    prim->uuid = uuid;
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
