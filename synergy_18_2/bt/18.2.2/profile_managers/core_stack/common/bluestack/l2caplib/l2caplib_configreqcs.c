/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_ConfigReqCs.c

\brief  Build and send an L2CA_CONFIG_REQ primitive to L2CAP using
        config settings from a L2CA_CONFIG_T structure.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_CONFIG_REQ using config structure

    Build and send an L2CA_CONFIG_REQ primitive to L2CAP using
    config settings from a L2CA_CONFIG_T structure.

    Note: This function will resume ownership of the pointers in the
    L2CA_CONFIG_T, but will NOT do it for the structure itself!
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_ConfigReqCs(l2ca_cid_t cid,
                      L2CA_CONFIG_T *config)
{
    L2CA_CONFIG_REQ_T *prim = pnew(L2CA_CONFIG_REQ_T);

    prim->type = L2CA_CONFIG_REQ;
    prim->cid = cid;

    /* Copy config structure */
    CsrMemCpy(&prim->config, config, sizeof(L2CA_CONFIG_T));

    /* Clear pointers and lengths in old structure */
    config->unknown = NULL;
    config->unknown_length = 0;
    config->qos = NULL;
    config->flow = NULL;
    config->flowspec = NULL;

    L2CA_PutMsg(prim);
}
#endif
