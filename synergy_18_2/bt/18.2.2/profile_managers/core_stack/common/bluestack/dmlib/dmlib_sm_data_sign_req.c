/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_data_sign_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dmlib_sm_data_sign_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_DATA_SIGN_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

#ifndef CSR_BT_EXCLUDE_SM_DATA_SIGN
void dmlib_sm_data_sign_req(
    CsrSchedQid phandle,
    TYPED_BD_ADDR_T *addrt,
    CsrUint16 context,
    CsrBool verify,
    CsrUint16 length,
    CsrUint8 *data,
    DM_UPRIM_T **pp_prim)
{
    DM_SM_DATA_SIGN_REQ_T *prim = zpnew(DM_SM_DATA_SIGN_REQ_T);

    prim->type = DM_SM_DATA_SIGN_REQ;
    prim->phandle = phandle;
    tbdaddr_copy(&prim->addrt, addrt);
    prim->context = context;
    prim->verify = verify;
    prim->length = length;
    prim->data = data;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}

#endif /* CSR_BT_EXCLUDE_SM_DATA_SIGN */
