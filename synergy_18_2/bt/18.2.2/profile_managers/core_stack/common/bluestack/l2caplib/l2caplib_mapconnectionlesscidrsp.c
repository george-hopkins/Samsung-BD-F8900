/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_MapConnectionlessCidRsp.c

\brief  Build and send a L2CA_MAP_FIXED_CID_RSP primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_MAP_FIXED_CID_RSP for fixed channel 2 (connectionless)

    Build and send a L2CA_MAP_FIXED_CID_RSP primitive to L2CAP.

    Set ucd_remote_psm if you receive incoming connectionless data and
    intend to send outgoing UCD data from the same local PSM to the same
    remote device. If you don't need to do this then it can be set to 0.

    See \ref L2CA_MAP_FIXED_CID_RSP_T for more details.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
#if defined(INSTALL_L2CAP_CONNLESS_SUPPORT) || defined(INSTALL_L2CAP_UCD_SUPPORT)
void L2CA_MapConnectionlessCidRsp(l2ca_cid_t cid,
                                  CsrUint16 con_ctx,
                                  psm_t ucd_remote_psm,
                                  l2ca_conflags_t flags)
{
    L2CA_MAP_FIXED_CID_RSP_T *prim = pnew(L2CA_MAP_FIXED_CID_RSP_T);

    prim->type = L2CA_MAP_FIXED_CID_RSP;
    prim->cid = cid;
    prim->con_ctx = con_ctx;
    prim->ucd_remote_psm = ucd_remote_psm;
    prim->flags = flags;

    L2CA_PutMsg(prim);
}
#endif
#endif
