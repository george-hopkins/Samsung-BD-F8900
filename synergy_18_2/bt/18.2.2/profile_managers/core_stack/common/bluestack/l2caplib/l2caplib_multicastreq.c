/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_MulticastReq.c

\brief  Build and send an L2CA_MULTICAST_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
/*! \brief Send L2CA_MULTICAST_REQ

    Build and send an L2CA_MULTICAST_REQ primitive to L2CAP.
    See \ref L2CA_MULTICAST_REQ_T for more details.
*/
void L2CA_MulticastReq(l2ca_cid_t *cids,
                       CsrUint16 length,
                       void *p_data)
{
    L2CA_MULTICAST_REQ_T *prim = pnew(L2CA_MULTICAST_REQ_T);

    prim->type = L2CA_MULTICAST_REQ;
    CsrMemCpy(prim->cids, cids, sizeof(l2ca_cid_t) * L2CA_MAX_MULTICAST_CIDS);
    prim->length = length;
    prim->data = L2CA_MblkWrap(p_data, length);
    L2CA_PutMsg(prim);
}
#endif
