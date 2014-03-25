/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_init_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_init_req_le_enc
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_INIT_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_INIT
void dm_sm_init_req_le_enc(
    CsrUint16 options,
    dm_security_mode_t security_mode,
    dm_security_level_t security_level_default,
    CsrUint16 config,
    CsrUint16 write_auth_enable,
    CsrUint8 mode3_enc,
    DM_SM_KEY_STATE_T *sm_key_state,
    CsrUint16 sm_div_state,
    CsrUint32 sm_sign_counter,
    CsrUint8 max_enc_key_size_thres,
    CsrUint8 min_enc_key_size_thres,
    DM_UPRIM_T **pp_prim)
{
    DM_SM_INIT_REQ_T *p_prim = zpnew(DM_SM_INIT_REQ_T);

    p_prim->type = DM_SM_INIT_REQ;
    p_prim->options = options;
    p_prim->security_mode = security_mode;
    p_prim->security_level_default = security_level_default;
    p_prim->config = config;
    p_prim->write_auth_enable = write_auth_enable;
    p_prim->mode3_enc = mode3_enc;
    if (sm_key_state != NULL)
    {
        p_prim->sm_key_state = *sm_key_state;
    }
    p_prim->sm_div_state = sm_div_state;
    p_prim->sm_sign_counter = sm_sign_counter;

    p_prim->max_enc_key_size_thres = max_enc_key_size_thres;
    p_prim->min_enc_key_size_thres = min_enc_key_size_thres;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_INIT */
