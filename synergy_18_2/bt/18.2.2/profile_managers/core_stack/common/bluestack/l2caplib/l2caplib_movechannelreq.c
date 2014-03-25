/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_MoveChannelReq.c

\brief  Build and send a L2CA_MOVE_CHANNEL_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_MOVE_CHANNEL_REQ

    Build and send a L2CA_MOVE_CHANNEL_REQ primitive to L2CAP.
    See \ref L2CA_MOVE_CHANNEL_REQ_T for more details.
*/
#ifdef INSTALL_AMP_SUPPORT
void L2CA_MoveChannelReq(l2ca_cid_t          cid,
                         l2ca_controller_t   remote_control,
                         l2ca_controller_t   local_control)
{
    L2CA_MOVE_CHANNEL_REQ_T *prim = pnew(L2CA_MOVE_CHANNEL_REQ_T);

    prim->type = L2CA_MOVE_CHANNEL_REQ;
    prim->cid = cid;
    prim->remote_control = remote_control;
    prim->local_control = local_control;

    L2CA_PutMsg(prim);
}
#endif
