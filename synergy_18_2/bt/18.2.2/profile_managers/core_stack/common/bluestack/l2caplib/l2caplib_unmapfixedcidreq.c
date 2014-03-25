/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_UnmapFixedCidReq.c

\brief  Build and send a L2CA_UNMAP_FIXED_CID_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_UNMAP_FIXED_CID_REQ for any type of fixed channel.

    Build and send a L2CA_UNMAP_FIXED_CID_REQ primitive to L2CAP.
    See \ref L2CA_UNMAP_FIXED_CID_REQ_T for more details.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
void L2CA_UnmapFixedCidReq(l2ca_cid_t cid)
{
    L2CA_UNMAP_FIXED_CID_REQ_T *prim = zpnew(L2CA_UNMAP_FIXED_CID_REQ_T);

    prim->type = L2CA_UNMAP_FIXED_CID_REQ;
    prim->cid = cid;

    L2CA_PutMsg(prim);
}
#endif
