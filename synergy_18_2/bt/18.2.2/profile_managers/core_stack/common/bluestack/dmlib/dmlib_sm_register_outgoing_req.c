/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_register_outgoing_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_register_outgoing_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_REGISTER_OUTGOING_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_REGISTER_OUTGOING
void dm_sm_register_outgoing_req(
    CsrSchedQid phandle,
    CsrUint16 context,
    BD_ADDR_T *p_bd_addr,
    dm_protocol_id_t protocol_id,
    CsrUint16 remote_channel,
    dm_security_level_t outgoing_security_level,
    psm_t psm,  /* Zero if don't care about connectionless security */
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_REGISTER_OUTGOING_REQ_T *p_prim = pnew(DM_SM_REGISTER_OUTGOING_REQ_T);

    p_prim->type = DM_SM_REGISTER_OUTGOING_REQ;
    p_prim->phandle = phandle;
    p_prim->context = context;
    tbdaddr_copy_from_public_bd_addr(&p_prim->connection.addrt, p_bd_addr);
    p_prim->connection.service.protocol_id = protocol_id;
    p_prim->connection.service.channel = remote_channel;
    p_prim->security_level = outgoing_security_level;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_REGISTER_OUTGOING */
