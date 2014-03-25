/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_unregister_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_unregister_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_UNREGISTER_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_UNREGISTER
void dm_sm_unregister_req(
    CsrSchedQid phandle,
    CsrUint16 context,
    dm_protocol_id_t protocol_id,
    CsrUint16 channel,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_UNREGISTER_REQ_T *p_prim = pnew(DM_SM_UNREGISTER_REQ_T);

    p_prim->type = DM_SM_UNREGISTER_REQ;
    p_prim->phandle = phandle;
    p_prim->context = context;
    p_prim->service.protocol_id = protocol_id;
    p_prim->service.channel = channel;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_UNREGISTER */
