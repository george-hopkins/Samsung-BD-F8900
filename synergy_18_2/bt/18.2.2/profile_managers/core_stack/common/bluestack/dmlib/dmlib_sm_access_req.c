/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_access_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*============================================================================*
    Private Data Types
 *============================================================================*/
/* None */

/*============================================================================*
    Private Data
 *============================================================================*/
/* None */

/*============================================================================*
    Private Function Prototypes
 *============================================================================*/
/* None */

/*============================================================================*
    Public Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_access_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_ACCESS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

#ifndef CSR_BT_EXCLUDE_SM_ACCESS
void dm_sm_access_req(
    CsrSchedQid phandle,
    BD_ADDR_T *p_bd_addr,
    dm_protocol_id_t protocol_id,
    CsrUint16 channel,
    CsrBool incoming,
    CsrUint32 context,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_ACCESS_REQ_T *p_prim = zpnew(DM_SM_ACCESS_REQ_T);

    p_prim->type = DM_SM_ACCESS_REQ;
    p_prim->phandle = phandle;
    bd_addr_copy(&p_prim->conn_setup.connection.addrt.addr, p_bd_addr);
    p_prim->conn_setup.connection.service.protocol_id = protocol_id;
    p_prim->conn_setup.connection.service.channel = channel;
    p_prim->conn_setup.incoming = incoming;
    p_prim->context = context;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}

#endif /* CSR_BT_EXCLUDE_SM_ACCESS */
