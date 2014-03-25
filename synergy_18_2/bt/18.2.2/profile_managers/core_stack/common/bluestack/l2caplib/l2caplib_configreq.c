/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_ConfigReq.c

\brief  Build and send an L2CA_CONFIG_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_CONFIG_REQ

    Build and send an L2CA_CONFIG_REQ primitive to L2CAP.
    NB: Ownership of any pointers will be transferred to the primitive!
    See \ref L2CA_CONFIG_REQ_T for more details.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_ConfigReq(l2ca_cid_t cid,
                    l2ca_options_t options,
                    l2ca_options_t hints,
                    CsrUint8 unknown_length,
                    CsrUint8 *unknown,
                    l2ca_mtu_t mtu,
                    l2ca_timeout_t flush_to,
                    L2CA_QOS_T *qos,
                    L2CA_FLOW_T *flow,
                    l2ca_fcs_t fcs,
                    L2CA_FLOWSPEC_T *flowspec,
                    l2ca_window_size_t ext_window)
{
    L2CA_CONFIG_REQ_T *prim = pnew(L2CA_CONFIG_REQ_T);

    prim->type = L2CA_CONFIG_REQ;
    prim->cid = cid;

    prim->config.options = options;
    prim->config.hints = hints;
    prim->config.unknown_length = unknown_length;
    prim->config.unknown = unknown;
    prim->config.mtu = mtu;
    prim->config.flush_to = flush_to;
    prim->config.qos = qos;
    prim->config.flow = flow;
    prim->config.fcs = fcs;
    prim->config.flowspec = flowspec;
    prim->config.ext_window = ext_window;

    L2CA_PutMsg(prim);
}
#endif
