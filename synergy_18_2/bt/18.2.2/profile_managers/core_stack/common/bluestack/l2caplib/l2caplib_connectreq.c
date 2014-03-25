/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_ConnectReq.c

\brief  Build and send an L2CA_CONNECT_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_common.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT

/*! \brief Send L2CA_CONNECT_REQ

    Build and send an L2CA_CONNECT_REQ primitive to L2CAP.
    See \ref L2CA_CONNECT_REQ_T for more details.
*/
void L2CA_ConnectReq(const BD_ADDR_T *p_bd_addr,
                     psm_t psm_local,
                     psm_t psm_remote,
                     CsrUint16 con_ctx,
                     DM_SM_SERVICE_T *substitute_security_service)
{
    L2CA_CONNECT_REQ_T *prim = zpnew(L2CA_CONNECT_REQ_T);

    prim->type = L2CA_CONNECT_REQ;
    bd_addr_copy(&prim->bd_addr, p_bd_addr);
    prim->psm_remote = psm_remote;
    prim->psm_local = psm_local;
    prim->con_ctx = con_ctx;

    if (substitute_security_service != NULL)
        prim->service = *substitute_security_service;

    L2CA_PutMsg(prim);
}

#endif
