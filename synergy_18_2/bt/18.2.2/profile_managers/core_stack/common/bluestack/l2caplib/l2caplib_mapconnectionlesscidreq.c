/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_MapConnectionlessCidReq.c

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

/*! \brief Send L2CA_MAP_FIXED_CID_REQ for fixed channel 2 (connectionless)

    Build and send a L2CA_MAP_FIXED_CID_REQ primitive to L2CAP.
    See \ref L2CA_MAP_FIXED_CID_REQ_T for more details.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
#if defined(INSTALL_L2CAP_CONNLESS_SUPPORT) || defined(INSTALL_L2CAP_UCD_SUPPORT)
void L2CA_MapConnectionlessCidReq(const BD_ADDR_T *p_bd_addr,
                                  psm_t cl_local_psm,
                                  psm_t cl_remote_psm,
                                  CsrUint16 con_ctx,
                                  l2ca_conflags_t flags)
{
    L2CA_MAP_FIXED_CID_REQ_T *prim = zpnew(L2CA_MAP_FIXED_CID_REQ_T);

    prim->type = L2CA_MAP_FIXED_CID_REQ;

    /* Copy BD_ADDR if it is there (they probably want UCD).
       Otherwise the only thing we can do is broadcast. */
    if (p_bd_addr == NULL)
        flags |= L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR_BROADCAST);
    else
        tbdaddr_copy_from_public_bd_addr(&prim->addrt, p_bd_addr);

    prim->fixed_cid = L2CA_CID_CONNECTIONLESS;
    prim->cl_local_psm = cl_local_psm;
    prim->cl_remote_psm = cl_remote_psm;
    prim->con_ctx = con_ctx;
    prim->flags = flags;

    L2CA_PutMsg(prim);
}
#endif
#endif
