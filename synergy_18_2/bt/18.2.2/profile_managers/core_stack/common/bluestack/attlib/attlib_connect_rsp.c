/*!
        (c) CSR plc 2010
        All rights reserved

\file   attlib_connect_rsp.c
*/

#include "csr_synergy.h"

#include "attlib_private.h"

#if defined (INSTALL_ATT_MODULE) && defined (INSTALL_ATT_BREDR)

/*----------------------------------------------------------------------------*
 *  NAME
 *      attlib_connect_rsp
 *
 *  DESCRIPTION
 *      Build and send an ATT_CONNECT_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void attlib_connect_rsp(
    CsrSchedQid phandle,
    CsrUint16 cid,
    l2ca_conn_result_t  response,
    ATT_UPRIM_T **pp_prim
    )
{
    ATT_CONNECT_RSP_T *prim = zpnew(ATT_CONNECT_RSP_T);

    prim->type = ATT_CONNECT_RSP;
    prim->phandle = phandle;
    prim->cid = cid;
    prim->response = response;

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
