/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_MapFixedCidReq.c

\brief  Build and send a L2CA_MAP_FIXED_CID_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_common.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_MAP_FIXED_CID_REQ for a fixed channel other than 2.

    Build and send a L2CA_MAP_FIXED_CID_REQ primitive to L2CAP.
    See \ref L2CA_MAP_FIXED_CID_REQ_T for more details.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
void L2CA_MapFixedCidReq(const TYPED_BD_ADDR_T *addrt,
                         l2ca_cid_t fixed_cid,
                         CsrUint16 con_ctx,
                         l2ca_conflags_t flags)
{
    L2CA_MAP_FIXED_CID_REQ_T *prim = zpnew(L2CA_MAP_FIXED_CID_REQ_T);

    prim->type = L2CA_MAP_FIXED_CID_REQ;
    tbdaddr_copy(&prim->addrt, addrt);
    prim->fixed_cid = fixed_cid;
    prim->con_ctx = con_ctx;
    prim->flags = flags;

    L2CA_PutMsg(prim);
}
#endif
