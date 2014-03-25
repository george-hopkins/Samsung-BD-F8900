/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_DisconnectRsp.c

\brief  Build and send an L2CA_DISCONNECT_RSP primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
/*! \brief Send L2CA_DISCONNECT_RSP

    Build and send an L2CA_DISCONNECT_RSP primitive to L2CAP.
    See \ref L2CA_DISCONNECT_RSP_T for more details.
*/
void L2CA_DisconnectRsp(l2ca_identifier_t identifier,
                        l2ca_cid_t cid)
{
    L2CA_DISCONNECT_RSP_T *prim = pnew(L2CA_DISCONNECT_RSP_T);

    prim->type = L2CA_DISCONNECT_RSP;
    prim->identifier = identifier;
    prim->cid = cid;

    L2CA_PutMsg(prim);
}
#endif
