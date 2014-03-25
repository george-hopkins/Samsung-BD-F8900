/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_find_info_req.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_find_info_req
 *
 *  DESCRIPTION
 *      Build and send an ATT_FIND_INFO_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_find_info_req(
    CsrSchedQid phandle,
    CsrUint16 cid,
    CsrUint16 start,
    CsrUint16 end,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_FIND_INFO_REQ_T *prim = zpnew(ATT_FIND_INFO_REQ_T);

    prim->type = ATT_FIND_INFO_REQ;
    prim->phandle = phandle;
    prim->cid = cid;
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

#endif /* INSTALL_ATT_MODULE */
