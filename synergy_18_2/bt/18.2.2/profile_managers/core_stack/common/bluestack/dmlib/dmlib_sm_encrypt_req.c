/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_encrypt_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_encrypt_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_ENCRYPT_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_ENCRYPT
void dm_sm_encrypt_req(
    BD_ADDR_T *p_bd_addr,
    CsrBool encrypt,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_ENCRYPT_REQ_T *p_prim = pnew(DM_SM_ENCRYPT_REQ_T);

    p_prim->type = DM_SM_ENCRYPT_REQ;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->encrypt = encrypt;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_ENCRYPT */
