/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_key_request_neg_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_key_request_neg_rsp
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_KEY_REQUEST_NEG_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

#ifndef CSR_BT_EXCLUDE_SM_KEY_REQUEST_NEG
void dm_sm_key_request_neg_rsp(
    TYPED_BD_ADDR_T *addrt,
    DM_SM_KEY_TYPE_T key_type,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_KEY_REQUEST_NEG_RSP_T *prim = pnew(DM_SM_KEY_REQUEST_NEG_RSP_T);

    prim->type = DM_SM_KEY_REQUEST_NEG_RSP;
    tbdaddr_copy(&prim->addrt, addrt);
    prim->key_type = key_type;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}

#endif /* CSR_BT_EXCLUDE_SM_KEY_REQUEST_NEG */
