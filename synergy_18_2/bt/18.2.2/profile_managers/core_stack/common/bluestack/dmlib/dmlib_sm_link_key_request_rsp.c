/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_link_key_request_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_link_key_request_res
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_LINK_KEY_REQUEST_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_LINK_KEY_REQUEST
void dm_sm_link_key_request_rsp(
    BD_ADDR_T *p_bd_addr,
    CsrUint8   key_type,
    CsrUint8   *key,
    DM_UPRIM_T **pp_prim
    )
{
    TYPED_BD_ADDR_T addrt;
    CsrUint16 security_requirements;
    DM_SM_UKEY_T u;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    if (key_type == DM_SM_LINK_KEY_AUTHENTICATED
            || key_type == DM_SM_LINK_KEY_LEGACY)
    {
        security_requirements =
            DM_SM_SECURITY_AUTHENTICATED_NO_BONDING;
    }
    else
    {
        security_requirements =
            DM_SM_SECURITY_UNAUTHENTICATED_NO_BONDING;
    }

    u.enc_bredr = pnew(DM_SM_KEY_ENC_BREDR_T);
    u.enc_bredr->link_key_type = key_type;
    if(DM_SM_LINK_KEY_NONE!=key_type)
    /* Application must take care to pass key value when key_type is none */
        CsrMemCpy(u.enc_bredr->link_key, key, SIZE_LINK_KEY);

    dm_sm_key_request_rsp(&addrt, security_requirements, DM_SM_KEY_ENC_BREDR, u, pp_prim);
}


#endif /* CSR_BT_EXCLUDE_LINK_KEY_REQUEST */
