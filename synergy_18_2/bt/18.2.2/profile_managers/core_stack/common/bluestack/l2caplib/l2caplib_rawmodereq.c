/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_RawModeReq.c

\brief  Build and send a L2CA_RAW_MODE_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_RAW_MODE_REQ

    Compatibility form of function.
*/
void L2CA_RawModeReq(l2ca_cid_t cid,
                     CsrBool cid_rx,
                     CsrBool acl_rx,
                     CsrSchedQid phandle)
{
    l2ca_raw_t raw_mode = L2CA_RAW_MODE_OFF;

    if (acl_rx)
        raw_mode = L2CA_RAW_MODE_BYPASS_CONNECTION;
    else if (cid_rx)
        raw_mode = L2CA_RAW_MODE_BYPASS_CHANNEL;

    L2CA_ExRawModeReq(cid, raw_mode, phandle);
}

/*! \brief Send L2CA_RAW_MODE_REQ

    Build and send a L2CA_RAW_MODE_REQ primitive to L2CAP.
    See \ref L2CA_RAW_MODE_REQ_T for more details.
*/
void L2CA_ExRawModeReq(l2ca_cid_t cid,
                       l2ca_raw_t raw_mode,
                       CsrSchedQid phandle)
{
    L2CA_RAW_MODE_REQ_T *prim = pnew(L2CA_RAW_MODE_REQ_T);

    prim->type = L2CA_RAW_MODE_REQ;
    prim->cid = cid;
    prim->raw_mode = raw_mode;
    prim->phandle = phandle;

    L2CA_PutMsg(prim);
}
