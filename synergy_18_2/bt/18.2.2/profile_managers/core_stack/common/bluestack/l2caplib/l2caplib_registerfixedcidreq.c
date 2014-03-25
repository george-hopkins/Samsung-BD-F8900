/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_RegisterFixedCidReq.c

\brief  Build and send a L2CA_REGISTER_FIXED_CID_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_REGISTER_FIXED_CID_REQ

    Build and send a L2CA_REGISTER_FIXED_CID_REQ primitive to L2CAP.
    See \ref L2CA_REGISTER_FIXED_CID_REQ_T for more details.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
void L2CA_RegisterFixedCidReq(CsrSchedQid phandle,
                              l2ca_cid_t fixed_cid,
                              L2CA_CONFIG_T *config,
                              CsrUint16 reg_ctx)
{
    L2CA_REGISTER_FIXED_CID_REQ_T *prim = zpnew(L2CA_REGISTER_FIXED_CID_REQ_T);

    prim->type = L2CA_REGISTER_FIXED_CID_REQ;
    prim->phandle = phandle;
    prim->fixed_cid = fixed_cid;
    prim->reg_ctx = reg_ctx;
    CsrMemCpy(&(prim->config),
           config,
           sizeof(L2CA_CONFIG_T));

    L2CA_PutMsg(prim);

}
#endif
