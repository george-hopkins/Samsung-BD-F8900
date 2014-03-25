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
#include "l2cap_flow.h"
#include "l2caplib.h"
#include "dmlib.h"
#include "dm_security_manager.h"
#include "l2cap_cid_fsm_definitions.h"

#ifdef INSTALL_AMP_SUPPORT
#include "dm_amp_interface.h"
#include "l2cap_amp_utils.h"
#endif

/*! \brief Run CID state machine.

    This function runs the appropriate CID state machine. For AMP-FSM
    enabled builds we use a 32 bit FSM, whereas for normal builds we
    use the more compact 16 bit variant.
*/
void CID_FsmRun(CIDCB_T *cidcb, fsm_event_t event, void *param2)
{
#ifndef DISABLE_AMP_FSM_SUPPORT
    (void)fsm_32bit_run(&l2cap_cid_fsm, &cidcb->state, event, cidcb, param2, FALSE);
#else
    (void)fsm_16bit_run(&l2cap_cid_fsm, &cidcb->state, event, cidcb, param2);
#endif
}

/*! \brief Store configuration paramters

    When we know that both local and remote configuration parameters
    are present, we can store them in the CIDCB and possibly also the
    flow control structure
*/
void CID_StoreConfiguration(CIDCB_T *cidcb,
                            L2CA_CONFIG_T *local,
                            L2CA_CONFIG_T *remote)
{
    /* Local MTU and flush timeout are stored in the config-req
     * function as we depend on sensible values before we get to run
     * this function (ie. before peer accepts the values) */

    /* Remote MTU */
    if(remote->options & L2CA_SPECIFY_MTU)
    {
        /* MTU can not be lowered for F&EC channels during
         * reconfigurations */
        if(!(CID_IsFlowControl(cidcb)
                    && cidcb->opened
                    && (remote->mtu < cidcb->remote_mtu)))
        {
            cidcb->remote_mtu = remote->mtu;
        }
    }

#ifdef INSTALL_L2CAP_QOS_SUPPORT
    if(local->options & L2CA_SPECIFY_QOS)
    {
        /* Not implemented */
    }
    if(remote->options & L2CA_SPECIFY_QOS)
    {
        /* Not implemented */
    }
#endif

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    if((local->options & L2CA_SPECIFY_FLOW)
            || (remote->options & L2CA_SPECIFY_FLOW)
            || CID_IsFlowControl(cidcb))
    {
        /* The F&EC code is able to detect critical errors which
         * requires us to disconnect */
        l2ca_disc_result_t result;
        result = FLOW_Setup(cidcb, local, remote);
        if(result != L2CA_STAY_CONNECTED)
        {
            CID_DisconnectReqInternal(cidcb, result);
            return;
        }
    }
#endif

#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    if((local->options & L2CA_SPECIFY_FLOWSPEC) && local->flowspec)
    {
        CsrPmemFree(cidcb->tx_flowspec);
        cidcb->tx_flowspec = pnew(L2CA_FLOWSPEC_T);
        CsrMemCpy(cidcb->tx_flowspec, local->flowspec, sizeof(L2CA_FLOWSPEC_T));
    }
    if((remote->options & L2CA_SPECIFY_FLOWSPEC) && remote->flowspec)
    {
        CsrPmemFree(cidcb->rx_flowspec);
        cidcb->rx_flowspec = pnew(L2CA_FLOWSPEC_T);
        CsrMemCpy(cidcb->rx_flowspec, remote->flowspec, sizeof(L2CA_FLOWSPEC_T));
    }
#endif
}

/*! \brief Destroy CID instance.

    This function destroys specified CID instance, removes CID from
    connection, optionally drops ACL connection, frees all resources
    associated with CID.

    \param cidcb Pointer to Channel Identifier control block.
    \param reason Reason for disconnecting.
    \param timeout Number of multiples of ACL idle timeout required.
*/
void CID_Destroy(CIDCB_T *cidcb,
                 hci_reason_t reason,
                 ACL_IDLE_TIMEOUT_MULTIPLIER_T timeout)
{
    L2CAP_CHCB_T *chcb = cidcb->chcb;

    /* Clean up everything -- this function will stop timers and flush
     * buffers, free the F&EC instance etc */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    CID_DisconnectCleanup(cidcb);
#endif

    /* Disconnect AMP links. Instance is freed in the
     * CIDME_FreeCidcb. */
#ifdef INSTALL_AMP_SUPPORT
    if(CID_IsAmp(cidcb))
    {
        /* Kill both new and old controller here in case we were
         * interrupted during a move */
        AMP_DisconnectReq(cidcb, TRUE); /* TRUE - "new" controller */
        AMP_DisconnectReq(cidcb, FALSE); /* TRUE - "new" controller */
    }
#endif

    /* Remove lock from locked ACLs */
    if (!cidcb->acl_unlocked)
        CH_Close(chcb, reason, timeout);

    /* Remove CIDCB from connection handle and free it */
    CH_RemoveCidcb(chcb, cidcb);
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
    CIDME_Unroute(cidcb);
#endif
    CIDME_FreeCidcb(cidcb);
    cidcb = NULL;
}

void CID_StandardDestroy(CIDCB_T *cidcb)
{
    CID_Destroy(cidcb, HCI_ERROR_OETC_USER, ACL_IDLE_TIMEOUT_AT_LEAST_TINY);
}

/*! \brief ACL channel is ready.

    For fixed channels, we enter the open fixed state as no security
    is required. For other channels, poke the SM to verify that
    security is OK */
fsm_event_t CID_FsmConnectAclReady(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
    if(CID_IsFixed(cidcb))
    {
        return CID_EV_CID_FIXED_ACCESS_CFM_OK;
    }
    else
#endif
    {
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
        /* Send access request to Security Manager */
        CID_SendDMSMAccessReq(cidcb, FALSE);
#endif
        /* No more events */
        return CID_EVENT_NULL;
    }
}

/*! \brief Notify CID instance of local close indication.

    This function handles a close indication, it converts the HCI
    status into the appropriate L2CAP error code and passes the
    appropriate event into the state machine.  This function is
    called from the connect handler when the ACL is disconnected.
*/
void CID_AclCloseInd(CIDCB_T *cidcb, CsrUint16 status)
{
    /* Send the L2CAP error as the second parameter */
    CID_FsmRun(cidcb, CID_EV_CID_ACL_CLOSE_IND, &status);
}

/*! \brief Notify CID instance that ACL is ready.

    This function is called from the connection handler when the newly creatd
    ACL connection is ready.
*/
void CID_AclReady(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, CID_EV_CID_ACL_READY, NULL);
}

/*!\brief Notify CID instance of received L2CAP packet.

    Data from channel has been read and is targeted this particular
    CID. Send data either to upper layer or into special handler
    function like the F&EC code or the fixed channel buffer.

    This function is NOT used for connectionless data - that's handled
    directly in CH_DataRxConnectionless.
*/
CsrBool CID_DataReadInd(CIDCB_T *cidcb,
                       CsrMblk *mblk_ptr,
                       CsrMblkSize mblk_size,
                       CsrUint8 *header)
{
    /* We will only accept data on a channel if the state is either
     * completely open or it's a basic mode channel being reconfigured.
     * During reconfiguration, we can still receive stuff, but it's OK
     * to dump data in streaming mode and enhanced retransmission mode.
     */
    if(CID_IsOpen(cidcb) ||
       CID_IsFixed(cidcb) ||
       CID_IsMoving(cidcb) ||
       CID_IsReconfiguring(cidcb))
    {
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
        /* Check if CID is using flow control, and if yes, pass data to flow
           module.
        */
        if(CID_IsFlowControl(cidcb))
        {
            FLOW_DataRead(cidcb, mblk_ptr, mblk_size, header);
            return TRUE;
        }
        else
#endif
        {
            /* Make sure length is less than MTU in basic mode */
            if (mblk_size <= cidcb->local_mtu)
            {
                /* Pass L2CAP packet to higher layer */
                L2CA_DataReadInd(cidcb, mblk_ptr, L2CA_DATA_SUCCESS, 0, NULL);
                return TRUE;
            }
        }
    }

    /* Data was not consumed */
    return FALSE;
}

/*! \brief Send MBLK over logical channel.

    This function attempts to send a data block in a MBLK over the specified
    logical channel.  If the channel is in the open state then we send the
    data immediately otherwise we pass it into the state machine.

    Note: Pointer to MBLK is set to NULL if this function takes ownership of the
    MBLK, otherwise the caller has responsibility for free-ing the MBLK.
*/
void CID_DataWriteReq(CIDCB_T *cidcb, CsrMblk **mblk_ptr, CsrUint16 context)
{
    CsrBool fixed = FALSE;

#ifdef INSTALL_L2CAP_DEBUG
    if (CsrMblkGetLength(*mblk_ptr) > cidcb->remote_mtu)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif

    /* Extend idle timeout for data transfer on fixed channels. */
    if (CID_IsFixedWithTX(cidcb))
    {
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
        if (cidcb->chcb != mcb.connectionless)
#endif
        {
            dm_acl_idle_timeout_extend_short(CH_GET_ACL(cidcb->chcb));
        }

        fixed = TRUE;
    }

    /* Check if logical channel is open for business. Because of
     * config/move crossovers we need to be fairly relaxed about
     * this */
    if (fixed || CID_IsOpen(cidcb) || CID_IsMoving(cidcb))
    {
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
        if (CID_IsFlowControl(cidcb))
        {
            /* Flow and error control modes */
            FLOW_DataWrite(cidcb, *mblk_ptr, context);
            *mblk_ptr = NULL;
            return;
        }
#endif
        /* Basic mode */
        if(CH_DataTxBasic(cidcb, context, *mblk_ptr))
        {
            *mblk_ptr = NULL;
            return;
        }
    }

    /* Reject the request */
    L2CA_DataWriteCfm(cidcb, 0, context, L2CA_DATA_NO_CONNECTION);
}

/*! \brief Kill defunct CID

     If we for some reason receive an ACL disconnect for a NULL state
     CID it must be destroyed.
*/
fsm_event_t CID_FsmDestroyCid(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;

    PARAM_UNUSED(arg2);

    /* Clean up everything -- this function will stop timers and flush
     * buffers, free the F&EC instance etc */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    CID_DisconnectCleanup(cidcb);
#endif

    /* Unlock the ACL by closing it - use standard idle timeout  */
    if (!cidcb->acl_unlocked)
        CH_StandardClose(cidcb->chcb);

    /* Remove CIDCB from connection handle and free it */
    CH_RemoveCidcb(cidcb->chcb, cidcb);
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
    CIDME_Unroute(cidcb);
#endif
    CIDME_FreeCidcb(cidcb);

    return CID_EVENT_NULL;
}

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void CID_SendConnectCfm(CIDCB_T *cidcb, l2ca_conn_result_t result)
{
    L2CA_ConnectCfm(CH_GET_BD_ADDR(cidcb->chcb),
                    cidcb->local_cid,
                    cidcb->p_handle,
                    CIDME_LocalPsm(cidcb),
                    result,
                    cidcb->context,
                    CIDME_RegistrationContext(cidcb));
}
#endif

/*! \brief Error occurred during ACL setup */
fsm_event_t CID_FsmConnectCfmAclError(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    CsrUint16 status = *(CsrUint16*)arg2;
#endif

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
    if(CID_IsFixed(cidcb))
    {
        /* Fixed channels simply die when ACL couldn't be
         * established */
        L2CA_MapFixedCidCfm(cidcb, NULL, cidcb->p_handle, L2CA_MISC_FAILED);
        CID_StandardDestroy(cidcb);
        return CID_EVENT_NULL;
    }
#endif

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    /* If retry allowed, construct and post a L2CA_CONNECT_REQ to
     * ourselves with an _even_ PSM. The even PSM indicates that this
     * is a retry and thus prevents a retry-deadlock.
     * We only retry if our connection attempt had the misfortune to
     * coincide with a standard disconnect. Otherwise we give up. */
    if((status == HCI_ERROR_OETC_USER || status == HCI_ERROR_CONN_TERM_LOCAL_HOST)
       && cidcb->allow_retry
       && cidcb->remote_cid == L2CA_CID_INVALID)
    {
        /* Preserve and reserve local CID */
        MCB_RetryCidFifoPush(cidcb->local_cid);

        /* Inform app that we're about to retry */
        CID_SendConnectCfm(cidcb, L2CA_CONNECT_RETRYING);

        /* Issue retry */
        L2CA_ConnectReq(CH_GET_BD_ADDR(cidcb->chcb),
                        (psm_t)(cidcb->local_psm->psm - 1), /* even number means retry */
                        cidcb->remote_psm,
                        cidcb->context,
                        cidcb->dm_sm_service);
    }
    else
    {
        l2ca_conn_result_t result = L2CA_CONNECT_FAILED;

        status &= 0xFF;
        if (status == HCI_ERROR_AUTH_FAIL)
            result = L2CA_CONNECT_REJ_SECURITY;

        if (status == HCI_ERROR_KEY_MISSING)
            result = L2CA_CONNECT_KEY_MISSING;

        /* Send connect confirmation to higher layer */
        CID_SendConnectCfm(cidcb, result);
    }
#endif

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Handle the disconnect to abort ACL setup
*/

extern FSM_ACTION_FN(CID_FsmConnectAclAbort)
{
    CIDCB_T *cidcb = (CIDCB_T *)pv_arg1;
    L2CAP_CHCB_T *chcb = cidcb->chcb;

    PARAM_UNUSED(pv_arg2);

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    /* Send a confirm failure immediately and not a disconnect as the channel
       is not established. This will be indicated to application
       via the auto-connect system.

       If a subsequent message is received against this cid, a bad message
       indication will be sent to the application */
    CID_SendConnectCfm(cidcb, L2CA_CONNECT_FAILED);
#endif

    if (!cidcb->acl_unlocked)
        CH_Close(chcb, HCI_ERROR_OETC_USER, ACL_IDLE_TIMEOUT_AT_LEAST_TINY);


    return CID_EVENT_NULL;
}


/*! \brief ACL went down just before we could send connect request.
           If it's coming back up again then just wait, otherwise
           we treat it as a fatal error.
*/
fsm_event_t CID_FsmConnectCfmAclDown(void *arg1, void *arg2)
{
    if ((*(CsrUint16*)arg2 & DM_ACL_REASON_RECONNECTING_AND_LOCKED) == 0)
        return CID_EV_CID_ACL_CLOSE_DESTROY;

    return CID_EVENT_NULL;
}

