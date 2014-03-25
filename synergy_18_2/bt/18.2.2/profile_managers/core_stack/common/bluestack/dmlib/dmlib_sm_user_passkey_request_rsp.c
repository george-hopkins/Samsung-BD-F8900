/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_user_passkey_request_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_user_passkey_request_res
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_USER_PASSKEY_REQUEST_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_USER_PASSKEY_REQUEST
void dm_sm_user_passkey_request_rsp(
    TYPED_BD_ADDR_T *addrt,
    CsrUint32   numeric_value,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_USER_PASSKEY_REQUEST_RSP_T *p_prim = pnew(DM_SM_USER_PASSKEY_REQUEST_RSP_T);
    p_prim->type            = DM_SM_USER_PASSKEY_REQUEST_RSP;
    p_prim->numeric_value   = numeric_value;
    tbdaddr_copy(&p_prim->addrt, addrt);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_USER_PASSKEY_REQUEST */
