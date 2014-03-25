/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_authorise_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_authorise_res
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_AUTHORISE_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_AUTHORISE
void dm_sm_authorise_rsp(
    BD_ADDR_T *p_bd_addr,
    dm_protocol_id_t protocol_id,
    CsrUint16 channel,
    CsrBool incoming,
    CsrUint16 authorisation,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_AUTHORISE_RSP_T *p_prim = zpnew(DM_SM_AUTHORISE_RSP_T);

    p_prim->type = DM_SM_AUTHORISE_RSP;
    bd_addr_copy(&p_prim->cs.connection.addrt.addr, p_bd_addr);
    p_prim->cs.connection.service.protocol_id = protocol_id;
    p_prim->cs.connection.service.channel = channel;
    p_prim->cs.incoming = incoming;
    p_prim->authorisation = authorisation;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_AUTHORISE */
