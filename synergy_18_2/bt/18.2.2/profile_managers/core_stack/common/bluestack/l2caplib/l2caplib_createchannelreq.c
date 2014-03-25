/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_CreateChannelReq.c

\brief  Build and send a L2CA_CREATE_CHANNEL_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_common.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_CREATE_CHANNEL_REQ

    Build and send a L2CA_CREATE_CHANNEL_REQ primitive to L2CAP.
    See \ref L2CA_CREATE_CHANNEL_REQ_T for more details.
*/
#ifdef INSTALL_AMP_SUPPORT
void L2CA_CreateChannelReq(psm_t               psm_local,
                           const BD_ADDR_T    *p_bd_addr,
                           psm_t               psm,
                           CsrUint16            con_ctx,
                           l2ca_controller_t   remote_control,
                           l2ca_controller_t   local_control,
                           DM_SM_SERVICE_T *substitute_security_service)
{
    L2CA_CREATE_CHANNEL_REQ_T *prim = zpnew(L2CA_CREATE_CHANNEL_REQ_T);

    prim->type = L2CA_CREATE_CHANNEL_REQ;
    prim->psm_local = psm_local;
    bd_addr_copy(&prim->bd_addr, p_bd_addr);
    prim->psm_remote = psm;
    prim->con_ctx = con_ctx;
    prim->remote_control = remote_control;
    prim->local_control = local_control;

    if (substitute_security_service != NULL)
        prim->service = *substitute_security_service;

    L2CA_PutMsg(prim);
}
#endif
