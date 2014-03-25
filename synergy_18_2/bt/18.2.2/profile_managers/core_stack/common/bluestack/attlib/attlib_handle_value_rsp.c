/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_handle_value_rsp.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#ifdef INSTALL_ATT_MODULE

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_handle_value_rsp
 *
 *  DESCRIPTION
 *      Build and send an ATT_HANDLE_VALUE_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_handle_value_rsp(
    CsrSchedQid phandle,
    CsrUint16 cid,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_HANDLE_VALUE_RSP_T *prim = zpnew(ATT_HANDLE_VALUE_RSP_T);

    prim->type = ATT_HANDLE_VALUE_RSP;
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
