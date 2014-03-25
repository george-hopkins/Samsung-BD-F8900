/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_MoveChannelRsp.c

\brief  Build and send a L2CA_MOVE_CHANNEL_RSP primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_MOVE_CHANNEL_RSP

    Build and send a L2CA_MOVE_CHANNEL_RSP primitive to L2CAP.
    See \ref L2CA_MOVE_CHANNEL_RSP_T for more details.
*/
#ifdef INSTALL_AMP_SUPPORT
void L2CA_MoveChannelRsp(l2ca_identifier_t   identifier,
                         l2ca_cid_t          cid,
                         l2ca_move_result_t  response)
{
    L2CA_MOVE_CHANNEL_RSP_T *prim = pnew(L2CA_MOVE_CHANNEL_RSP_T);

    prim->type = L2CA_MOVE_CHANNEL_RSP;
    prim->identifier = identifier;
    prim->cid = cid;
    prim->response = response;

    L2CA_PutMsg(prim);
}
#endif
