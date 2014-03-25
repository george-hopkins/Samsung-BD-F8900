/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_cid.c

\brief  This file implements the L2CAP channel state machine.

        For every L2CAP channel there will exist an instance of the
        CIDCB structure, this structure holds all the state
        information for a L2CAP channel.

        This file includes the channel state machine, upper and lower
        interfaces. The data path is removed from the state machine
        for simplicity and speed reasons.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"
#ifndef BUILD_FOR_HOST
#include "buffers.h"
#endif

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_control.h"
#include "l2cap_interface.h"
#include "l2caplib.h"
#include "l2cap_cid_fsm_definitions.h"

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT

/*! \brief Notify CID instance of local create request.

    This function handles a create request, it just passes the
    appropriate event into the state machine.
 */
void CID_MapFixedCidReq(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, (fsm_event_t)(CH_IS_CONNECTIONLESS(cidcb->chcb) ?
                CID_EV_CID_MAP_BROADCAST : CID_EV_CID_MAP_FIXED_REQ), NULL);
}

/*! \brief Notify CID instance of fixed channel creation from peer.

    This function is called from the connection handler when a data is
    received on a new fixed channel.
*/
void CID_MapFixedCidInd(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, CID_EV_CID_MAP_FIXED_IND, NULL);
}

/*! \brief Fixed channel created from peer

    This function is invoked if the fixed channel was registered and
    peer sent us data before the CIDCB structure was created.
*/
fsm_event_t CID_FsmMapFixedCidReq(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CONFIG_T *config;
    PARAM_UNUSED(arg2);

    /* Store config, use same config for local and remote */
    config = MCB_GetConfigFromFixedCid(cidcb->remote_cid);
    if(config != NULL)
    {
        /* Store and apply settings. Note that configuration is
         * completely symmetric here and we can't do fixups  */
        CID_StoreConfiguration(cidcb, config, config);
    }

    return CID_EVENT_NULL;
}

/*! \brief Fixed channel created from peer

    This function is invoked if the fixed channel was registered and
    peer sent us data before the CIDCB structure was created.
*/
fsm_event_t CID_FsmMapFixedCidInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CONFIG_T *config;
    PARAM_UNUSED(arg2);

    /* Tell the upper layer about this event */
    L2CA_MapFixedCidInd(cidcb);

    /* Store config, use same config for local and remote. Note that
     * configuration is completely symmetric here and we can't do fixups. */
    if ((config = MCB_GetConfigFromFixedCid(cidcb->remote_cid)) != NULL)
        CID_StoreConfiguration(cidcb, config, config);

    return CID_EVENT_NULL;
}

/*! \brief Locally initiated fixed channel operational

    Fixed channel is now ready for use. Inform upper layer.
*/
fsm_event_t CID_FsmFixedAccessOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Tell the upper layer about this event */
    L2CA_MapFixedCidCfm(cidcb, NULL, cidcb->p_handle, L2CA_MISC_SUCCESS);

    return CID_EVENT_NULL;
}

/*! \brief Fixed channel internal disconnect

    If the peer misbehaves very badly on a fixed channel, the spec
    mandates us to close the entire ACL. Let upper layer know about
    the failure condition.
*/
fsm_event_t CID_FsmFixedDisconnectAbnormal(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_disc_result_t *result = (l2ca_disc_result_t*)arg2;

    L2CA_UnmapFixedCidInd(cidcb, *result);

    /* Exterminate the ACL and exit */
    CH_Exterminate(cidcb->chcb);
    return CID_EVENT_NULL;
}

/*! \brief Fixed channel ACL drop

    The 'normal' way to disconnect fixed channels is when the ACL
    drops. Inform the peer about this.
*/
fsm_event_t CID_FsmFixedDisconnectAcl(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    L2CA_UnmapFixedCidInd(cidcb, L2CA_DISCONNECT_LINK_LOSS);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);
    return CID_EVENT_NULL;
}

#endif /* INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT */
