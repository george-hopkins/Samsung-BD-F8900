/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_AutoConnectRsp.c

\brief  Build and send a L2CA_AUTO_CONNECT_RSP primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_AUTO_CONNECT_RSP

    Build and send a L2CA_AUTO_CONNECT_RSP primitive to L2CAP.
    See \ref L2CA_MOVE_AUTO_CONNECT_RSP_T for more details.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_AutoConnectRsp(l2ca_identifier_t    identifier,
                         l2ca_cid_t           cid,
                         l2ca_conn_result_t   response,
                         CsrUint16             con_ctx,
                         CsrUint16             conftab_length,
                         CsrUint16            *conftab)
{
    L2CA_AUTO_CONNECT_RSP_T *prim = pnew(L2CA_AUTO_CONNECT_RSP_T);

    prim->type = L2CA_AUTO_CONNECT_RSP;
    prim->identifier = identifier;
    prim->cid = cid;
    prim->response = response;
    prim->con_ctx = con_ctx;
    prim->conftab_length = conftab_length;
    prim->conftab = conftab;

    L2CA_PutMsg(prim);
}
#endif
