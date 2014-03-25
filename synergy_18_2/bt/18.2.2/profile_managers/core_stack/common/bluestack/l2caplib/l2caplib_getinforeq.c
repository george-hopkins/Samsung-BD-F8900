/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_GetInfoReq.c

\brief  Build and send an L2CA_GETINFO_REQ primitive to L2CAP.
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
/*! \brief Send L2CA_GETINFO_REQ

    Build and send an L2CA_GETINFO_REQ primitive to L2CAP.
    See \ref L2CA_GETINFO_REQ_T for more details.
*/
void L2CA_GetInfoReq(const BD_ADDR_T *p_bd_addr,
                     CsrSchedQid phandle,
                     CsrUint16 info_type,
                     CsrUint16 req_ctx,
                     l2ca_conflags_t flags)
{
    L2CA_GETINFO_REQ_T *prim = pnew(L2CA_GETINFO_REQ_T);

    prim->type = L2CA_GETINFO_REQ;
    bd_addr_copy(&prim->bd_addr, p_bd_addr);
    prim->info_type = info_type;
    prim->phandle = phandle;
    prim->req_ctx = req_ctx;
    prim->flags = flags;

    L2CA_PutMsg(prim);
}
#endif
