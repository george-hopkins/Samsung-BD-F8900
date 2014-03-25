/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_register_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_register_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_REGISTER_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_REGISTER
void dm_sm_register_req(
    CsrSchedQid phandle,
    CsrUint16 context,
    dm_protocol_id_t protocol_id,
    CsrUint16 channel,
    CsrBool outgoing_ok,
    dm_security_level_t security_level,
    psm_t psm,  /* Zero if don't care about connectionless security */
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_REGISTER_REQ_T *p_prim = pnew(DM_SM_REGISTER_REQ_T);

    p_prim->type = DM_SM_REGISTER_REQ;
    p_prim->phandle = phandle;
    p_prim->context = context;
    p_prim->service.protocol_id = protocol_id;
    p_prim->service.channel = channel;
    p_prim->outgoing_ok = outgoing_ok;
    p_prim->security_level = security_level;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_REGISTER */
