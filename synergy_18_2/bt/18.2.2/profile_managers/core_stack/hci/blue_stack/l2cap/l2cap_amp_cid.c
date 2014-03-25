/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_amp_cid.c

\brief  This file implements the L2CAP AMP specific parts of the
        channel state machine

        This file is simply to keep the l2cap_cid.c file clean and put
        AMP specific bits and bobs into their own separate file.

        The headerfile for prototypes etc. is still l2cap_cid.h.
*/

#include "csr_synergy.h"

#include "l2cap_cid.h"

#ifdef INSTALL_AMP_SUPPORT

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "hci_prim.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"

#include "l2cap_common.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cidme.h"
#include "l2cap_control.h"
#include "l2cap_interface.h"
#include "l2cap_flow.h"
#include "l2cap_amp_utils.h"
#include "l2caplib.h"
#include "dmlib.h"
#include "dm_amp_interface.h"
#include "l2cap_cid_fsm_definitions.h"

static L2CA_RESULT_T CID_AmpConvertPhysError(hci_error_t hci_error)
{
    switch (hci_error)
    {
        case HCI_ERROR_NO_CONNECTION:
            return L2CA_RESULT_PHYS_NO_CONNECTION;

        case HCI_ERROR_CONN_TERM_LOCAL_HOST:
            return L2CA_RESULT_PHYS_CONN_TERM_LOCAL_HOST;

        case HCI_ERROR_INSUFFICIENT_SECURITY:
            return L2CA_RESULT_PHYS_INSUFFICIENT_SECURITY;

        default:
            return L2CA_RESULT_PHYS_FAILED;
    }
}

/*! \brief Notify CID that application wants to connect an AMP channel

    This function is called from the connection handler after a CIDCB
    has been created and we have a controller id -- we can then send
    the peer a create channel signal.
*/
void CID_AmpCreateReq(CIDCB_T *cidcb)
{
    AMP_InfoSanity(cidcb);
    CID_FsmRun(cidcb, CID_EV_CID_AMP_CREATE_REQ, NULL);
}

/*! \brief Notify CID instance of an incoming AMP channel connect request

    Peer has sent us a create channel request, so notify CID state
    machine.
*/
void CID_AmpCreateInd(CIDCB_T *cidcb, CID_CREATE_CHAN_REQ_T *req)
{
    AMP_InfoSanity(cidcb);
    CID_FsmRun(cidcb, CID_EV_CID_AMP_CREATE_IND, req);
}

/*! \brief Notify CID instance of a channel request retry

    Peer has sent us a channel create request for a CID that has
    already been created - ie. this is a retry.
*/
void CID_AmpCreateIndRetry(CIDCB_T *cidcb, CID_CREATE_CHAN_REQ_T *req)
{
    AMP_InfoSanity(cidcb);
    CID_FsmRun(cidcb, CID_EV_CID_AMP_CREATE_IND_RET, req);
}

/*! \brief Notify CID instance of a create channel response

    Peer has responded to our channel create request. Using the result
    code, we pass in different events to the FSM.
*/
void CID_AmpCreateCfm(CIDCB_T *cidcb, l2ca_cid_t cid, l2ca_conn_result_t result)
{
    AMP_InfoSanity(cidcb);
    switch(result)
    {
        case L2CA_CONNECT_SUCCESS:
            CID_FsmRun(cidcb, CID_EV_CID_AMP_CREATE_CFM_OK, &cid);
            break;

        case L2CA_CONNECT_PENDING:
            CID_FsmRun(cidcb, CID_EV_CID_AMP_CREATE_CFM_PND, &cid);
            break;

        default:
            /* All errors at this point */
            CID_FsmRun(cidcb, CID_EV_CID_AMP_CREATE_CFM_NOK, &result);
            break;
    }
}

/*! \brief Notify CID instance of a channel create response

    Upper layer has responded (with an result code) to a remote
    channel create request. Pass in different FSM event based on the
    result code.
*/
void CID_AmpCreateRsp(CIDCB_T *cidcb, L2CA_CREATE_CHANNEL_RSP_T *res)
{
    fsm_event_t event;

    AMP_InfoSanity(cidcb);
    switch(res->response)
    {
        case L2CA_CONNECT_SUCCESS:
            event = CID_EV_CID_AMP_CREATE_RSP_OK;
            break;

        case L2CA_CONNECT_PENDING:
            event = CID_EV_CID_AMP_CREATE_RSP_PND;
            break;

        default:
            /* All errors at this point */
            event = CID_EV_CID_AMP_CREATE_RSP_NOK;
            break;
    }
    CID_FsmRun(cidcb, event, res);
}

/*! \brief Notify CID instance of a channel move request

    Upper layer has requested to move a L2CAP channel. Pass event into
    state machine.
*/
void CID_AmpMoveReq(CIDCB_T *cidcb, L2CA_MOVE_CHANNEL_REQ_T *req)
{
    /* No sanity check allowed here */
    CID_FsmRun(cidcb, CID_EV_CID_AMP_MOVE_REQ, req);
}

/*! \brief Notify CID instance of channel move cfm

    Peer has responded to our channel move request.
*/
void CID_AmpMoveCfm(CIDCB_T *cidcb, l2ca_move_result_t result)
{
    AMP_InfoSanity(cidcb);
    switch(result)
    {
        case L2CA_MOVE_SUCCESS:
            CID_FsmRun(cidcb, CID_EV_CID_AMP_MOVE_CFM_OK, &result);
            break;

        case L2CA_MOVE_PENDING:
            CID_FsmRun(cidcb, CID_EV_CID_AMP_MOVE_CFM_PND, NULL);
            break;

        default:
            /* All errors at this point */
            CID_FsmRun(cidcb, CID_EV_CID_AMP_MOVE_CFM_NOK, &result);
            break;
    }
}

/*! \brief Notify CID instance of a channel move indication

    Peer wants to move a L2CAP channel to another controller. Pass
    signal into state machine.
*/
void CID_AmpMoveInd(CIDCB_T *cidcb, CID_MOVE_CHAN_REQ_T *req)
{
    /* No sanity check allowed here */
    CID_FsmRun(cidcb, CID_EV_CID_AMP_MOVE_IND, req);
}

/*! \brief Upper layer response to move channel indication

    Upper layer responds to a move channel indication
*/
void CID_AmpMoveRsp(CIDCB_T *cidcb, L2CA_MOVE_CHANNEL_RSP_T *res)
{
    fsm_event_t event;

    AMP_InfoSanity(cidcb);
    switch(res->response)
    {
        case L2CA_MOVE_SUCCESS:
            event = CID_EV_CID_AMP_MOVE_RSP_OK;
            break;

        default:
            /* All errors at this point */
            event = CID_EV_CID_AMP_MOVE_RSP_NOK;
            break;
    }
    CID_FsmRun(cidcb, event, res);
}

/*! \brief Notify CID instance of channel move confirm request

    Peer has sent us the channel move confirm request signal.
*/
void CID_AmpConfirmInd(CIDCB_T *cidcb, CID_MOVE_CHAN_CONFIRM_REQ_T *req)
{
    /* Note that we do not do AMP_InfoSanity(cidcb) because it is
       acceptable for cidcb->amp_info to be NULL. */
    CID_FsmRun(cidcb, CID_EV_CID_AMP_CONFIRM_IND, req);
}

/*! \brief Notify CID instance of channel move confirm

    Peer has sent us the channel move confirm response signal.
*/
void CID_AmpConfirmCfm(CIDCB_T *cidcb)
{
    AMP_InfoSanity(cidcb);
    CID_FsmRun(cidcb, CID_EV_CID_AMP_CONFIRM_CFM, NULL);
}

/*! \brief Notify CID of physical channel connect cfm

    Physical channel is now up (or operation failed). This happens
    during outgoing create channel or move channel requests before we
    actually start communicating with the peer.
*/
void CID_AmpCreatePhysicalCfm(CIDCB_T *cidcb, hci_return_t result, l2ca_controller_t remote_amp_id)
{
    fsm_event_t event = CID_EV_CID_AMP_PHYS_CFM_OK;
    void *arg2 = &remote_amp_id;

    AMP_InfoSanity(cidcb);

    if (result != HCI_SUCCESS)
    {
        event = CID_EV_CID_AMP_PHYS_CFM_NOK;
        arg2 = &result;
    }

    CID_FsmRun(cidcb, event, arg2);
}

/*! \brief Notify CID of physical link verification

    On incoming create/move requests we need to verify that the local
    controller ID exists. This has happened, so pass OK/NOK message
    into state machine
*/
void CID_AmpVerifyPhysicalLinkCfm(CIDCB_T *cidcb, CsrBool exists, CID_VERIFY_LINK_CFM_T *cfm)
{
    AMP_InfoSanity(cidcb);
    CID_FsmRun(cidcb,
               (fsm_event_t)(exists ? CID_EV_CID_AMP_VERIFY_PHYS_OK : CID_EV_CID_AMP_VERIFY_PHYS_NOK),
               cfm);
}

/*! \brief Notify CID of physical channel connect cfm

    Physical channel is now up (or operation failed)
*/
void CID_AmpCreateLogicalCfm(CIDCB_T *cidcb,
                             DM_AMPM_CONNECT_CHANNEL_RSP_T *prim)
{
    fsm_event_t event = CID_EV_CID_AMP_LOGICAL_CFM_NOK;

    if (prim->status == HCI_SUCCESS)
        event = CID_EV_CID_AMP_LOGICAL_CFM_OK;

    AMP_InfoSanity(cidcb);
    CID_FsmRun(cidcb, event, prim);
}

/*! \brief Notify CID of an AMP link loss

    The DM has told us that the AMP link has disappeared (link loss).
*/
void CID_AmpDisconnectInd(CIDCB_T *cidcb, hci_error_t reason)
{
    AMP_InfoSanity(cidcb);
    CID_FsmRun(cidcb, CID_EV_CID_AMP_DISCONNECT_IND, &reason);
}

/*! \brief Incoming create channel request

    Peer wants to setup an AMP channel. Before this function,
    sig_handle will already have allocated a CIDCB structure and
    allocated the amp_info including setting the state to
    AMPST_CREATING. We now check that the local controller ID (sent by
    the peer) is valid.
*/
fsm_event_t CID_FsmAmpCreateInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CREATE_CHAN_REQ_T *sig_args = (CID_CREATE_CHAN_REQ_T*)arg2;

    /* Store bits in the CIDCB structure */
    cidcb->remote_cid = sig_args->source_cid;
    cidcb->rcnreq_signal_id = sig_args->signal_id;

    /* Ask DM/AMPM for validity of the controller ID */
    dm_amp_verify_physical_link_req(CH_GET_ACL(cidcb->chcb),
                                    cidcb->local_cid,
                                    cidcb->amp_info->local_amp);
    return CID_EVENT_NULL;
}


/*! \brief Incoming physical channel verified and OK

    Incoming create channel request has a valid local controller ID so
    we can proceed with the channel setup: Send a pending response to
    the peer and start the security stuff.
*/
fsm_event_t CID_FsmAmpCreateIndPhysOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_VERIFY_LINK_CFM_T *cfm = (CID_VERIFY_LINK_CFM_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Store AMP controller classification */
    cidcb->amp_info->lsto = cfm->lsto;
    cidcb->amp_info->be_flush_to = cfm->flush_to;

    /* Send pending result as we do the security clearing */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         L2CA_CONNECT_PENDING,
                                         SIGNAL_STATUS_AUTHORISATION,
                                         cidcb->rcnreq_signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Query SM about this channel */
    CID_SendDMSMAccessReq(cidcb, TRUE);

    return CID_EVENT_NULL;
}

/*! \brief Incoming physical channel not valid

    Incoming create channel request has an invalid local controller ID
    so simply reject the request.
*/
fsm_event_t CID_FsmAmpCreateIndPhysNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Send reject result as controller ID was invalid */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         L2CA_CONNECT_REJ_CONTROL_ID,
                                         SIGNAL_STATUS_NONE,
                                         cidcb->rcnreq_signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* This CID must be killed again */
    CID_StandardDestroy(cidcb);
    return CID_EVENT_NULL;
}

/*! \brief Request physical channel for create-local success

    This function is invoked when the SM has allowed the outgoing
    channel to be created. We then ask for the physical AMP channel to
    be created.

    The connection handler will already have allocated the CIDCB and
    filled in the amp-info structure and set the AMP state to
    CREATING.
*/
fsm_event_t CID_FsmAmpCreateReq(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    PARAM_UNUSED(arg2);

    /* Send physical connect request */
    dm_amp_physical_connect_req(CH_GET_ACL(cidcb->chcb),
                                cidcb->local_cid,
                                cidcb->amp_info->local_amp,
                                cidcb->amp_info->remote_amp);

    return CID_EVENT_NULL;
}

/*! \brief Physical channel established for create procedure

    DM has informed us that the physical channel is now present. We
    then ask for the AMP classification information.
*/
fsm_event_t CID_FsmAmpCreateReqPhysOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_controller_t *remote_amp_id = (l2ca_controller_t*)arg2;

    /* Store results from physical open */
    cidcb->amp_info->remote_amp = *remote_amp_id;

    /* Ask for classification data */
    dm_amp_verify_physical_link_req(CH_GET_ACL(cidcb->chcb),
                                    cidcb->local_cid,
                                    cidcb->amp_info->local_amp);

    return CID_EVENT_NULL;
}

/*! \brief Physical channel establishment failed for create

    The physical channel failed. Inform application and kill the
    CIDCB.
*/
fsm_event_t CID_FsmAmpCreateReqPhysNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    hci_error_t *error = (hci_error_t*)arg2;

    /* Notify upper layer about failure */
    L2CA_CreateChannelCfm(cidcb,
                          NULL,
                          cidcb->p_handle,
                          (l2ca_conn_result_t)CID_AmpConvertPhysError(*error));

    /* Kill the CIDCB */
    CID_StandardDestroy(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Physical link verified for create channel initiator

    Even for locally initiated link creations we still "verify"
    that the link is present as that's how we read out the LSTO
    and AMP best effort flush timeout classification.
*/
fsm_event_t CID_FsmAmpCreateReqVerifyOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    CID_VERIFY_LINK_CFM_T *cfm = (CID_VERIFY_LINK_CFM_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Store AMP controller classification */
    cidcb->amp_info->lsto = cfm->lsto;
    cidcb->amp_info->be_flush_to = cfm->flush_to;

    /* Create and send signal. Store copy for duplicate detection */
    sig_ptr = SIG_SignalCreateChannelReq(cidcb->local_cid,
                                         cidcb->remote_psm,
                                         cidcb->amp_info->remote_amp);

    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Physical channel established for move procedure

    DM has informed us that the physical channel is now present. We
    can then inform upper layer to stop sending data and then ask for
    the AMP classification. We'll inform the peer about the move
    request after the classification data has arrived.
*/
fsm_event_t CID_FsmAmpMoveReqPhysOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_controller_t *remote_amp_id = (l2ca_controller_t*)arg2;

    /* Store results from physical open */
    cidcb->amp_info->remote_amp = *remote_amp_id;

    /* Inform application that physical is up. At this point the app
     * should stop sending us data */
    L2CA_MoveChannelCfm(cidcb, L2CA_MOVE_INITIATING, FALSE);

    /* Choke the F&EC data path */
    if(CID_IsFlowControl(cidcb))
    {
        FLOW_AmpMovePrepare(cidcb);
    }

    /* Ask for classification data */
    dm_amp_verify_physical_link_req(CH_GET_ACL(cidcb->chcb),
                                    cidcb->local_cid,
                                    cidcb->amp_info->local_amp);

    return CID_EVENT_NULL;
}

/*! \brief Physical channel establishment failed for move procedure

    The physical channel failed. Revert to old controller IDs and
    allow traffic to restart. The peer has never been notified about
    this move, so backing out is quite easy.
*/
fsm_event_t CID_FsmAmpMoveReqPhysNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    hci_error_t *error = (hci_error_t*)arg2;

    /* Swap controller IDs back to original values */
    AMP_SwapControllerIds(cidcb);

    /* Zero old controller ID. Logical channel never set up at this
     * point, so no reason to kill it */
    cidcb->amp_info->old_local_amp = L2CA_AMP_CONTROLLER_BT;
    cidcb->amp_info->old_remote_amp = L2CA_AMP_CONTROLLER_BT;

    /* Notify upper layer about failure */
    L2CA_MoveChannelCfm(cidcb,
                        (l2ca_move_result_t)CID_AmpConvertPhysError(*error),
                        FALSE);

    /* Allow F&EC traffic to restart */
    if(CID_IsFlowControl(cidcb))
    {
        FLOW_AmpMoveResume(cidcb, FCOK_OFF);
    }

    /* Trigger a cleanup */
    return CID_EV_CID_AMP_CLEANUP_REQ;
}

/*! \brief Physical link verified for move channel initiator

    Even for locally initiated link moves we still "verify"
    that the link is present as that's how we read out the LSTO
    and AMP best effort flush timeout classification.
*/
fsm_event_t CID_FsmAmpMoveReqVerifyOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    CID_VERIFY_LINK_CFM_T *cfm = (CID_VERIFY_LINK_CFM_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Store AMP controller classification */
    cidcb->amp_info->lsto = cfm->lsto;
    cidcb->amp_info->be_flush_to = cfm->flush_to;

    /* Create move channel request and send sent it. Peer is then
     * expected to reply with 'pending' to progress us into the config
     * state  */
    sig_ptr = SIG_SignalMoveChannelReq(cidcb->local_cid,
                                       cidcb->amp_info->remote_amp);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Create channel response from peer

    Peer has replied succesfully to our create channel request. Pass
    up notification and assume we've entered the configuration phase.
*/
fsm_event_t CID_FsmAmpCreateCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_cid_t *dest_cid = (l2ca_cid_t*)arg2;

    /* Store bits in CIDCB */
    cidcb->remote_cid = *dest_cid;

    /* Notify upper layer about pending */
    L2CA_CreateChannelCfm(cidcb, NULL, cidcb->p_handle, L2CA_CONNECT_SUCCESS);

    /* Reset/start configuration negotiation */
    cidcb->local_config_ok = FALSE;
    cidcb->remote_config_ok = FALSE;
    CID_ConfigTimerStart(cidcb);

    /* No more events - expect us to enter CONFIG */
    return CID_EVENT_NULL;
}

/*! \brief Create channel response pending from peer

    Peer has replied to our create channel request with
    the pending result code
*/
fsm_event_t CID_FsmAmpCreateCfmPnd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_cid_t *dest_cid = (l2ca_cid_t*)arg2;

    /* Store bits in CIDCB */
    cidcb->remote_cid = *dest_cid;

    /* Notify upper layer about pending */
    L2CA_CreateChannelCfm(cidcb, NULL, cidcb->p_handle, L2CA_CONNECT_PENDING);

    return CID_EVENT_NULL;
}

/*! \brief Create channel response from peer

    Pass up create channel cfm to peer with a given error response
    code and then destroy the CIDCB block.
*/
fsm_event_t CID_FsmAmpCreateCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_conn_result_t *response = (l2ca_conn_result_t*)arg2;

    L2CA_CreateChannelCfm(cidcb, NULL, cidcb->p_handle, *response);
    CID_StandardDestroy(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Create channel error

    An error happened during the channel creation
*/
fsm_event_t CID_FsmAmpCreateCfmError(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_conn_result_t response = L2CA_CONNECT_FAILED;
    CsrUint16 status;

    /* See if we can retry. */
    if (arg2 != NULL &&
            ((status = *(CsrUint16*)arg2) == HCI_ERROR_OETC_USER
                || status == HCI_ERROR_CONN_TERM_LOCAL_HOST)
            && cidcb->allow_retry
            && cidcb->remote_cid == L2CA_CID_INVALID)
    {
        /* Preserve and reserve local CID */
        MCB_RetryCidFifoPush(cidcb->local_cid);

        /* Inform app that we're about to retry */
        L2CA_CreateChannelCfm(cidcb,
                              NULL,
                              cidcb->p_handle,
                              L2CA_CONNECT_RETRYING);

        /* Issue retry */
        L2CA_CreateChannelReq((psm_t)(CIDME_LocalPsm(cidcb) - 1), /* even number means retry */
                              CH_GET_BD_ADDR(cidcb->chcb),
                              cidcb->remote_psm,
                              cidcb->context,
                              cidcb->amp_info->remote_amp,
                              cidcb->amp_info->local_amp,
                              cidcb->dm_sm_service);

        CID_StandardDestroy(cidcb);

        return CID_EVENT_NULL;
    }

    /* Do the same as a normal fail, but with a special error code so
     * invoke other FSM event function */
    return CID_FsmAmpCreateCfmNok(arg1, &response);
}

/*! \brief Close CID due to security rejection during connection */
fsm_event_t CID_FsmAmpCreateCfmErrorSec(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    hci_return_t status = *((hci_return_t*)arg2);
    l2ca_conn_result_t response = L2CA_CONNECT_REJ_SECURITY;

    if(status == HCI_ERROR_KEY_MISSING)
    {
        response = L2CA_CONNECT_KEY_MISSING;
    }

    /* Connection has failed due to security limitations */
    L2CA_CreateChannelCfm(cidcb, NULL, cidcb->p_handle, response);

    /* Destroy this instance */
    CID_Destroy(cidcb, HCI_ERROR_AUTH_FAIL, ACL_IDLE_TIMEOUT_AT_LEAST_TINY);

    return CID_EVENT_NULL;
}

/*! \brief Create channel timeout

    The peer didn't respond in time to our create channel request
*/
fsm_event_t CID_FsmAmpCreateCfmTimeout(void *arg1, void *arg2)
{
    l2ca_conn_result_t response = L2CA_CONNECT_TIMEOUT;
    PARAM_UNUSED(arg2);

    /* Do the same as a normal fail, but with a special error code so
     * invoke other FSM event function */
    return CID_FsmAmpCreateCfmNok(arg1, &response);
}

/*! \brief Create channel abort

    The application aborted the channel creation.
*/
fsm_event_t CID_FsmAmpCreateCfmAbort(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    SIG_SIGNAL_T *sig_ptr;
    l2ca_conn_result_t response = L2CA_CONNECT_FAILED;
    PARAM_UNUSED(arg2);

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Create and send disconnect signal */
    sig_ptr = SIG_SignalCreateDisconnectReq(cidcb->remote_cid, cidcb->local_cid);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Do the same as a normal fail, but with a special error code so
     * invoke other FSM event function */
    return CID_FsmAmpCreateCfmNok(arg1, &response);
}

/*! \brief Security Manager says channel is OK

    The SM has allowed the channel to be setup. Inform application
    about the indication.
*/
fsm_event_t CID_FsmAmpCreateSmCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    AMP_InfoSanity(cidcb);

    L2CA_CreateChannelInd(cidcb);
    return CID_EVENT_NULL;
}

/*! \brief Security Manager rejects a channel

    The SM has rejected a channel setup. Reject connection towards
    peer with a security block and then drop the CIDCB.
*/
fsm_event_t CID_FsmAmpCreateSmCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    AMP_InfoSanity(cidcb);

    /* Create, duplicate-detect-enable and send signal */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         L2CA_CONNECT_REJ_SECURITY,
                                         SIGNAL_STATUS_NONE,
                                         cidcb->rcnreq_signal_id);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Destroy this instance */
    CID_Destroy(cidcb, HCI_ERROR_AUTH_FAIL, ACL_IDLE_TIMEOUT_AT_LEAST_TINY);

    return CID_EVENT_NULL;
}

/*! \brief Upper layer responds pending to channel creation

    The upper layer has responded to a channel create indication
    with the pending result code. Tell the peer about this.
*/
fsm_event_t CID_FsmAmpCreateRspPnd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CREATE_CHANNEL_RSP_T *prim = (L2CA_CREATE_CHANNEL_RSP_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Create, duplicate-detect-enable and send signal */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         L2CA_CONNECT_PENDING,
                                         SIGNAL_STATUS_NONE,
                                         prim->identifier);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Upper layer rejects a channel creation

    The upper layer has rejected a channel create indication.  Send
    the reject response. The FSM will put us into the "wait for NCP"
    state to ensure that our reject signal is sent before we close the
    ACL and destroy the CIDCB.
*/
fsm_event_t CID_FsmAmpCreateRspNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CREATE_CHANNEL_RSP_T *prim = (L2CA_CREATE_CHANNEL_RSP_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Create, duplicate-detect-enable and send signal */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         (CsrUint16)(prim->response),
                                         SIGNAL_STATUS_NONE,
                                         prim->identifier);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Detach ourself from PSM registration to allow it to be deleted. */
    cidcb->local_psm = NULL;

    /* Trigger a cleanup */
    return CID_EV_CID_AMP_CLEANUP_REQ;
}

/*! \brief Upper layer accepts channel creation

    The upper layer has accepted to create a new channel. Tell peer
    and proceed into the configuration phase (which is in with the
    normal L2CAP CID FSM file).
*/
fsm_event_t CID_FsmAmpCreateRspOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CREATE_CHANNEL_RSP_T *prim = (L2CA_CREATE_CHANNEL_RSP_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Reset/start configuration negotiation */
    cidcb->local_config_ok = FALSE;
    cidcb->remote_config_ok = FALSE;
    CID_ConfigTimerStart(cidcb);

    /* Notify peer that we've accepted the channel creation */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         L2CA_CONNECT_SUCCESS,
                                         SIGNAL_STATUS_NONE,
                                         prim->identifier);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events - expect us to enter CONFIG state */
    return CID_EVENT_NULL;
}

/*! \brief Peer send a create channel request again

    The peer has re-sent a channel create request for a CID that's
    already in use during setup. Send a pending response code.
*/
fsm_event_t CID_FsmAmpCreateIndRetry(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CREATE_CHAN_REQ_T *signal_args = (CID_CREATE_CHAN_REQ_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Create, duplicate-detect-enable and send signal */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         L2CA_CONNECT_PENDING,
                                         SIGNAL_STATUS_NONE,
                                         signal_args->signal_id);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Peer send a create channel request again

    The peer has re-sent a channel create request for a CID that's
    already in active use. Reject the attempt due to resources.
*/
fsm_event_t CID_FsmAmpCreateIndRetryActive(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CREATE_CHAN_REQ_T *signal_args = (CID_CREATE_CHAN_REQ_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Create, duplicate-detect-enable and send signal */
    sig_ptr = SIG_SignalCreateChannelRsp(cidcb->local_cid,
                                         cidcb->remote_cid,
                                         L2CA_CONNECT_REJ_RESOURCES,
                                         SIGNAL_STATUS_NONE,
                                         signal_args->signal_id);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Application wants to move this channel.

    Upper layer has requested a move. Construct the signal and send
    it. Also, make sure the F&EC code is choked!
*/
fsm_event_t CID_FsmAmpMoveReq(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    L2CA_MOVE_CHANNEL_REQ_T *prim = (L2CA_MOVE_CHANNEL_REQ_T*)arg2;

    /* We must use ERTM or SM to be able to move */
    if(!CID_IsFlowControl(cidcb))
    {
        /* Move not allowed. Inform upper layer and go back to open
         * state. */
        L2CA_MoveChannelCfm(cidcb, L2CA_MOVE_NOT_ALLOWED, FALSE);
        return CID_EV_CID_AMP_MOVE_CANCEL;
    }

    /* No flowspecs? Invent the best effort ones */
    if(cidcb->tx_flowspec == NULL)
    {
        cidcb->tx_flowspec = L2CA_AllocFlowspec();
    }
    if(cidcb->rx_flowspec == NULL)
    {
        cidcb->rx_flowspec = L2CA_AllocFlowspec();
    }

    /* Allocate the AMP info and flag us for a locally initiated
     * move. This also includes backing up the old IDs */
    AMP_AllocInfo(cidcb);
    AMP_SwapControllerIds(cidcb);
    AMP_SetState(cidcb, AMPST_MOVING_LOCAL);
    cidcb->amp_info->local_amp   = prim->local_control;
    cidcb->amp_info->remote_amp  = prim->remote_control;
    cidcb->amp_info->move_result = L2CA_MOVE_INTERNAL_PENDING;
    cidcb->amp_info->logical_pending = FALSE;
    cidcb->amp_info->logical_ok  = FALSE;

    /* Send physical connect request */
    dm_amp_physical_connect_req(CH_GET_ACL(cidcb->chcb),
                                cidcb->local_cid,
                                cidcb->amp_info->local_amp,
                                cidcb->amp_info->remote_amp);

    return CID_EVENT_NULL;
}

/*! \brief Peer wants to move this channel from OPEN state

    Do a bit of sanity checking to catch errors early in the process
    (to which we reply and enter the confirm stage). We validate the
    physical link before sending a positive reply to the peer.
*/
fsm_event_t CID_FsmAmpMoveInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    CID_MOVE_CHAN_REQ_T *sig_args = (CID_MOVE_CHAN_REQ_T*)arg2;
    l2ca_move_result_t result;
    l2ca_controller_t curr_control;

    /* Get current controller ID */
    curr_control = (l2ca_controller_t)(CID_IsAmp(cidcb)
                                       ? cidcb->amp_info->local_amp
                                       : L2CA_AMP_CONTROLLER_BT);

    if(!CID_IsFlowControl(cidcb))
    {
        /* ERTM or SM must be used */
        result = L2CA_MOVE_REFUSED;
    }
    else if(curr_control == sig_args->control_id)
    {
        /* Can't move to same controller */
        result = L2CA_MOVE_REJ_SAME_ID;
    }
    else
    {
        result = L2CA_MOVE_SUCCESS;
    }

    /* Move can not be done. Inform peer that we can't do this and
     * wait for the confirmation */
    if(result != L2CA_MOVE_SUCCESS)
    {
        SIG_SIGNAL_T *sig_ptr;

        if(cidcb->amp_info != NULL)
        {
            AMP_SetState(cidcb, AMPST_DOWN);
        }
        sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                           cidcb->remote_cid,
                                           (CsrUint16)result,
                                           sig_args->signal_id);
        SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
        SIGH_SignalSend(cidcb->chcb, sig_ptr);
        return CID_EVENT_NULL;
    }

    /* Allocate the AMP info and flag us for a locally initiated
     * move. This also includes backing up the old IDs */
    AMP_AllocInfo(cidcb);
    AMP_SwapControllerIds(cidcb);

    /* No flowspecs? Invent the best effort ones */
    if(cidcb->tx_flowspec == NULL)
    {
        cidcb->tx_flowspec = L2CA_AllocFlowspec();
    }
    if(cidcb->rx_flowspec == NULL)
    {
        cidcb->rx_flowspec = L2CA_AllocFlowspec();
    }

    /* Store bits from the signal */
    AMP_SetState(cidcb, AMPST_MOVING_REMOTE);
    cidcb->rcnreq_signal_id       = sig_args->signal_id;
    cidcb->amp_info->move_result = L2CA_MOVE_SUCCESS;
    cidcb->amp_info->local_amp   = sig_args->control_id;
    cidcb->amp_info->logical_pending = FALSE;
    cidcb->amp_info->logical_ok  = FALSE;

    /* Choke the F&EC data path */
    if(CID_IsFlowControl(cidcb))
    {
        FLOW_AmpMovePrepare(cidcb);
    }

    /* Ask DM/AMPM for validity of the controller ID */
    dm_amp_verify_physical_link_req(CH_GET_ACL(cidcb->chcb),
                                    cidcb->local_cid,
                                    cidcb->amp_info->local_amp);

    return CID_EVENT_NULL;
}

/*! \brief Incoming physical channel for move verified and OK

    Incoming move channel request has a valid local controller ID so
    we can now inform the upper layer. We still have not replied to
    the peer.
*/
fsm_event_t CID_FsmAmpMoveIndPhysOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_VERIFY_LINK_CFM_T *cfm = (CID_VERIFY_LINK_CFM_T*)arg2;

    /* Store AMP controller classification */
    cidcb->amp_info->lsto = cfm->lsto;
    cidcb->amp_info->be_flush_to = cfm->flush_to;

    /* Inform upper layer */
    L2CA_MoveChannelInd(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Incoming move does not have a valid physical channel

    Incoming move channel request has an invalid local controller ID
    so reject the request and revert to old channel.
*/
fsm_event_t CID_FsmAmpMoveIndPhysNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Revert to old controller and expect peer to send us the
     * move confirm */
    AMP_SwapControllerIds(cidcb);
    AMP_SetState(cidcb, AMPST_CONFIRM_REMOTE);

    /* Zero old controller ID. Logical channel never set up at this
     * point, so no reason to kill it */
    cidcb->amp_info->move_result = L2CA_MOVE_REJ_CONTROL_ID;
    cidcb->amp_info->old_local_amp = L2CA_AMP_CONTROLLER_BT;
    cidcb->amp_info->old_remote_amp = L2CA_AMP_CONTROLLER_BT;

    /* Send response */
    sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                       cidcb->remote_cid,
                                       L2CA_MOVE_REJ_CONTROL_ID,
                                       cidcb->rcnreq_signal_id);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Local/remote move channel request collision

    Peer has sent us a move channel request while we ourselves have an
    outstanding move channel request. Use the DM's BD_ADDR compare/reject
    function to figure out whether we or the peer won.
*/
fsm_event_t CID_FsmAmpMoveIndCollide(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    CID_MOVE_CHAN_REQ_T *sig_args = (CID_MOVE_CHAN_REQ_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;
    fsm_event_t result;

    if(dm_amp_reject_remote_request(CH_GET_ACL(cidcb->chcb)))
    {
        /* We won. Reject peer's request and continue as initiator */
        sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                           cidcb->remote_cid,
                                           L2CA_MOVE_COLLISION,
                                           sig_args->signal_id);
        SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
        SIGH_SignalSend(cidcb->chcb, sig_ptr);
        result = CID_EVENT_NULL;
    }
    else
    {
        /* We swapped the controller IDs when we received the local
           * move request, so in order not to detect this as a 'same
           * id', swap them back */
        AMP_SwapControllerIds(cidcb);

        /* We loose. First we use the standard move channel indication
         * handler, as we are now effectively the responder */
        (void)CID_FsmAmpMoveInd(arg1, arg2);

        /* Then make sure we transition to the responder state */
        result = CID_EV_CID_AMP_MOVE_RSP_PND;
    }

    return result;
}

/*! \brief Application accepts a move indication

    Application does want to move a channel. Inform peer that we can
    proceed with the logical channel setup by sending a
    move-response-pending, and then start the logical channel creation
    ourselves.
*/
fsm_event_t CID_FsmAmpMoveRspOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    L2CA_MOVE_CHANNEL_RSP_T *prim = (L2CA_MOVE_CHANNEL_RSP_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Send move-pending to peer: Construct, dup-detect and send
     * response. But do NOT do this if we've moving to the BR/EDR and
     * flowspec is best effort */
    if((cidcb->tx_flowspec->fs_service_type != L2CA_QOS_TYPE_BEST_EFFORT) ||
       (cidcb->rx_flowspec->fs_service_type != L2CA_QOS_TYPE_BEST_EFFORT) ||
       (cidcb->amp_info->local_amp != L2CA_AMP_CONTROLLER_BT))
    {
        sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                           cidcb->remote_cid,
                                           L2CA_MOVE_PENDING,
                                           prim->identifier);
        SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
        SIGH_SignalSend(cidcb->chcb, sig_ptr);
    }

    /* Request a logical */
    AMP_KickLogicalChannelReq(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Application replies with failure to a move indication

    Application does not want to move a channel. Inform remote side.
    Peer should still send us a confirmation to complete the (now
    failed) move procedure.
*/
fsm_event_t CID_FsmAmpMoveRspNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    L2CA_MOVE_CHANNEL_RSP_T *prim = (L2CA_MOVE_CHANNEL_RSP_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Don't revert controller IDs or zero the old controller IDs yet.
       Wait until CID_FsmAmpConfirmInd(). */

    /* The wait-for-confirm-response state is internal to AMP */
    AMP_SetState(cidcb, AMPST_CONFIRM_REMOTE);

    cidcb->amp_info->move_result = prim->response;

    /* Send response */
    sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                       cidcb->remote_cid,
                                       (CsrUint16)(prim->response),
                                       prim->identifier);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Logical channel create success

    The logical channel has been created successfully during a
    remotely initiated move request. Setup the AMP queue and inform
    the peer that we can proceed with the confirmation phase.
*/
fsm_event_t CID_FsmAmpMoveRemLogicalCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    DM_AMPM_CONNECT_CHANNEL_RSP_T *prim = (DM_AMPM_CONNECT_CHANNEL_RSP_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;
    AMPINFO_T *info = cidcb->amp_info;

    /* Update states */
    info->logical_pending = FALSE;
    info->logical_ok = TRUE;
    if(info->queue != NULL)
    {
        TXQUEUE_T *q = info->queue;
        info->old_physical = q->physical_handle;
        info->old_logical = q->logical_handle;
        info->old_hci_data_queue = q->hci_data_queue;
    }

    /* Setup new AMP queue and prune old ones */
    info->queue = AMP_CreateQueue(cidcb, prim);
    AMP_PruneQueues(cidcb->chcb);

    /* The wait-for-confirm-response state is internal to AMP */
    AMP_SetState(cidcb, AMPST_CONFIRM_REMOTE);

    /* Send response to peer */
    sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                       cidcb->remote_cid,
                                       L2CA_MOVE_SUCCESS,
                                       cidcb->rcnreq_signal_id);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Logical channel create failed

    The logical channel could not be created during a remotely
    initited move request. Inform the peer, revert to the old values
    and expect peer to start the confirmation process.
*/
fsm_event_t CID_FsmAmpMoveRemLogicalCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Update states */
    cidcb->amp_info->logical_pending = FALSE;
    cidcb->amp_info->logical_ok = FALSE;

    /* Revert back to old defaults */
    AMP_SwapControllerIds(cidcb);

    /* Zero old controller ID. Logical channel never set up at this
     * point, so no reason to kill it */
    cidcb->amp_info->move_result = L2CA_MOVE_REJ_CONFIG;
    cidcb->amp_info->old_local_amp = L2CA_AMP_CONTROLLER_BT;
    cidcb->amp_info->old_remote_amp = L2CA_AMP_CONTROLLER_BT;

    /* The wait-for-confirm-response state is internal to AMP */
    AMP_SetState(cidcb, AMPST_CONFIRM_REMOTE);

    /* Send response to peer */
    sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                       cidcb->remote_cid,
                                       L2CA_MOVE_REJ_CONFIG,
                                       cidcb->rcnreq_signal_id);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Peer rejects our move request.

    Peer has rejected the move request. We must still send a
    move-channel-confirm-request to the peer in order to complete the
    move even though it's failed.

    This puts us into the internal AMP state where we wait for the
    peer's "move-channel-confirm-response" signal.
*/
fsm_event_t CID_FsmAmpMoveCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    l2ca_move_result_t *result = (l2ca_move_result_t*)arg2;

    /* Move collisions are special, as one side simply aborts and
     * become the responder. We must catch this, and inform the upper
     * layer and then bail out */
    if(*result == L2CA_MOVE_COLLISION)
    {
        L2CA_MoveChannelCfm(cidcb, L2CA_MOVE_COLLISION, FALSE);
        return CID_EVENT_NULL;
    }

    /* Revert to old controller */
    AMP_SwapControllerIds(cidcb);

    /* Store the move result until we can inform the upper layer */
    cidcb->amp_info->move_result = *result;

    /* The wait-for-confirm-response state is internal to AMP */
    AMP_SetState(cidcb, AMPST_CONFIRM_LOCAL);

    /* Clear old controller. We musn't disconnect as we never
     * connected the logical link */
    cidcb->amp_info->old_local_amp = L2CA_AMP_CONTROLLER_BT;
    cidcb->amp_info->old_remote_amp = L2CA_AMP_CONTROLLER_BT;

    /* Send move-channel-confirm-request */
    AMP_KickConfirmationReq(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Peer has replied with move pending

    Peer acknowledges our move request by replying with the
    pending result code. Inform upper layer.
*/
fsm_event_t CID_FsmAmpMoveCfmPnd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    PARAM_UNUSED(arg2);

    /* Inform upper layer */
    L2CA_MoveChannelCfm(cidcb, L2CA_MOVE_PENDING, FALSE);

    /* Trigger the logical channel creation */
    AMP_KickLogicalChannelReq(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Peer accepts our move request.

    Peer has accepted the move-request. This means that the flowspecs
    are already acceptable. Make sure logical link is setup up,
    otherwise proceed to the confirmation stage
*/
fsm_event_t CID_FsmAmpMoveCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    l2ca_move_result_t *result = (l2ca_move_result_t*)arg2;

    /* Store the move result until we can inform the upper layer */
    cidcb->amp_info->move_result = *result;

    /* If peer misbehaved and skipped the pending, we need to
     * setup the logical channel here */
    AMP_KickLogicalChannelReq(cidcb);

    /* Start the confirmation process if possible */
    AMP_KickConfirmationReq(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Logical channel create success

    The logical channel has been created successfully during a locally
    initiated move request.
*/
fsm_event_t CID_FsmAmpMoveLocLogicalCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    DM_AMPM_CONNECT_CHANNEL_RSP_T *prim = (DM_AMPM_CONNECT_CHANNEL_RSP_T*)arg2;
    AMPINFO_T *info = cidcb->amp_info;

    /* Update states */
    info->logical_pending = FALSE;
    info->logical_ok = TRUE;
    if(info->queue != NULL)
    {
        TXQUEUE_T *q = info->queue;
        info->old_physical = q->physical_handle;
        info->old_logical = q->logical_handle;
        info->old_hci_data_queue = q->hci_data_queue;
    }

    /* Setup new AMP queue and prune old ones */
    info->queue = AMP_CreateQueue(cidcb, prim);
    AMP_PruneQueues(cidcb->chcb);

    /* Start the confirmation process if possible */
    AMP_KickConfirmationReq(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Logical channel create failed

    The logical channel could not be created during a locally initiated
    move request.
*/
fsm_event_t CID_FsmAmpMoveLocLogicalCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    PARAM_UNUSED(arg2);

    /* Update states */
    cidcb->amp_info->move_result = L2CA_MOVE_INITIATOR_REFUSED;
    cidcb->amp_info->logical_pending = FALSE;
    cidcb->amp_info->logical_ok = FALSE;

    /* Start the confirmation process if possible */
    AMP_KickConfirmationReq(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Move channel timeout

    The peer didn't respond in time to our move channel request. This
    destroys the CIDCB.
*/
fsm_event_t CID_FsmAmpMoveCfmTimeout(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    PARAM_UNUSED(arg2);

    /* Inform upper layer */
    L2CA_MoveChannelCfm(cidcb, L2CA_MOVE_TIMEOUT, FALSE);

    /* Kill the CIDCB */
    CID_StandardDestroy(cidcb);

    return CID_EVENT_NULL;
}

/*! \brief Application move request while busy

    Application sent us a move request in a state where this can't be
    handled.  This is probably due to a crossing signal in the
    scheduler. Simply reject the applications' request.
*/
fsm_event_t CID_FsmAmpMoveReqBusy(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    L2CA_MOVE_CHANNEL_REQ_T *prim = (L2CA_MOVE_CHANNEL_REQ_T*)arg2;
    PARAM_UNUSED(prim);

    /* Simply reject this */
    L2CA_MoveChannelCfm(cidcb, L2CA_MOVE_LOCAL_BUSY, TRUE);

    return CID_EVENT_NULL;
}

/*! \brief Peer sent us a move request while busy

    The peer has sent us a move request in a state where this can't be
    handled. This is unlikely to ever happen, but we play nice and
    tell the peer that we're unavailable.
*/
fsm_event_t CID_FsmAmpMoveIndBusy(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    CID_MOVE_CHAN_REQ_T *sig_args = (CID_MOVE_CHAN_REQ_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Reject the request due to unsupported configuration. This
     * result code isn't perfect, but it's probably the closest
     * match */
    sig_ptr = SIG_SignalMoveChannelRsp(cidcb->local_cid,
                                       cidcb->remote_cid,
                                       L2CA_MOVE_REJ_CONFIG,
                                       sig_args->signal_id);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Logical channel create success

    The logical channel has been created successfully during a create
    channel process -- ie. we're in the CONFIG state.
*/
fsm_event_t CID_FsmAmpConfigLogicalCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    DM_AMPM_CONNECT_CHANNEL_RSP_T *prim = (DM_AMPM_CONNECT_CHANNEL_RSP_T*)arg2;
    AMPINFO_T *info = cidcb->amp_info;

    /* Update AMP states */
    info->logical_pending = FALSE;
    info->logical_ok = TRUE;
    if(info->queue != NULL)
    {
        TXQUEUE_T *q = info->queue;
        info->old_physical = q->physical_handle;
        info->old_logical = q->logical_handle;
        info->old_hci_data_queue = q->hci_data_queue;
    }

    /* Setup new AMP queue and prune old ones */
    info->queue = AMP_CreateQueue(cidcb, prim);
    AMP_PruneQueues(cidcb->chcb);

    /* Call generic logical/admission ok function */
    return CID_LogicalComplete(cidcb);
}

/*! \brief Logical channel create failed

    The logical channel could not be created during a create channel
    process -- ie. we're in the CONFIG state. This should only happen
    if the flowspec was rejected - thusly we reject the flowspec.
*/
fsm_event_t CID_FsmAmpConfigLogicalCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    DM_AMPM_CONNECT_CHANNEL_RSP_T *prim = (DM_AMPM_CONNECT_CHANNEL_RSP_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Update states */
    cidcb->amp_info->logical_pending = FALSE;
    cidcb->amp_info->logical_ok = FALSE;

    /* Store failed flowspec in the "res" buffer */
    CID_SetFlowspecResponse(cidcb, &prim->rx_flowspec);

    /* Send a configuration failure as the logical link creation
     * failed. We point directly to the response collection buffer in
     * order send the suggested flowspec */
    sig_ptr = SIG_SignalCreateConfigSignal(
            cidcb,
            cidcb->confbuf->buffer[CONFBUF_RES].len, /* length */
            cidcb->confbuf->buffer[CONFBUF_RES].buf, /* options */
            FALSE,                                   /* continuation */
            cidcb->rcnreq_signal_id,
            L2CA_CONFIG_FLOWSPEC_REJECTED);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return CID_EVENT_NULL;
}

/*! \brief Channel move confirm request received

    The peer has sent a move channel confirm request signal.  This is
    the final step in the remotely initiated move procedure, and we
    now apply the new (or reverted) data path. We also inform the
    upper layer using a move-channel-complete indication.
*/
fsm_event_t CID_FsmAmpConfirmInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    CID_MOVE_CHAN_CONFIRM_REQ_T *sig_args = (CID_MOVE_CHAN_CONFIRM_REQ_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;
    AMPINFO_T *info = cidcb->amp_info;

    /* Reply if we're in the right state, i.e. aren't using AMP at
     * all, or actually expects this to arrive */
    if((info == NULL) ||
       (info->amp_state == AMPST_CONFIRM_REMOTE) ||
       (info->amp_state == AMPST_DOWN))
    {
        sig_ptr = SIG_SignalMoveChannelConfirmRsp(cidcb->local_cid,
                                                  sig_args->source_cid,
                                                  sig_args->signal_id);
        SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
        SIGH_SignalSend(cidcb->chcb, sig_ptr);
    }

    if (info != NULL)
    {
        /* If move confirm failed, and we haven't already detected an
         * error, set the appropriate error code */
        if((sig_args->result != L2CA_MOVE_SUCCESS) &&
                (info->move_result == L2CA_MOVE_SUCCESS))
        {
            info->move_result = L2CA_MOVE_INITIATOR_REFUSED;
        }

        /* If upper layer expects this, notify it */
        if(info->amp_state == AMPST_CONFIRM_REMOTE)
        {
            if(info->move_result == L2CA_MOVE_SUCCESS)
            {
                /* Disconnect old logical handle. AMPM does it's own
                 * reference counting, so this is safe even if we're still
                 * using it for another channel. Reset handle to avoid
                 * multiple kills */
                AMP_DisconnectReq(cidcb, FALSE);
            }
            else
            {
                /* Move failed. Use the old controller again */
                AMP_SwapControllerIds(cidcb);
                AMP_RevertQueue(cidcb);
            }

            /* Old controller is now invalid */
            info->old_local_amp = L2CA_AMP_CONTROLLER_BT;
            info->old_remote_amp = L2CA_AMP_CONTROLLER_BT;

            /* Responder doesn't need to know about invalid/same controller ID
               errors because these are handled internally. */
            if (info->move_result != L2CA_MOVE_REJ_CONTROL_ID
                    && info->move_result != L2CA_MOVE_REJ_SAME_ID)
            {
                /* Inform upper layer that we're done */
                L2CA_MoveChannelCmpInd(cidcb);
            }
        }
    }

    /* Cleanup queues */
    AMP_FinaliseSetup(cidcb);
    AMP_PruneQueues(cidcb->chcb);

    /* Unchoke F&EC */
    if(CID_IsFlowControl(cidcb))
    {
        /* Move responder is also the data path resume responder */
        FLOW_AmpMoveResume(cidcb, FCOK_AUTO_RR);
    }

    /* We are now up and running */
    if(info != NULL)
    {
        AMP_SetState(cidcb, AMPST_RUNNING);
    }

    /* Trigger a cleanup */
    return CID_EV_CID_AMP_CLEANUP_REQ;
}

/*! \brief Channel move confirm response received

    The peer has responded to our channel-move-confirm-request. This
    is the final step in the locally initiated move
    procedure. Disconnect the old (or reverted) AMP channel and inform
    upper layer */
fsm_event_t CID_FsmAmpConfirmCfm(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    AMPINFO_T *info = cidcb->amp_info;
    PARAM_UNUSED(arg2);

    if((info != NULL) &&
       (info->amp_state == AMPST_CONFIRM_LOCAL))
    {
        /* Inform upper layer that we're done */
        L2CA_MoveChannelCfm(cidcb, info->move_result, FALSE);

        /* Disconnect old link if successfull */
        if(info->move_result == L2CA_MOVE_SUCCESS)
        {
            /* Disconnect old logical handle. AMPM does it's own
             * reference counting, so this is safe even if we're still
             * using it for another channel. Reset handle to avoid
             * multiple kills */
            AMP_DisconnectReq(cidcb, FALSE);
            info->old_local_amp = L2CA_AMP_CONTROLLER_BT;
            info->old_remote_amp = L2CA_AMP_CONTROLLER_BT;
        }
        else
        {
            /* Revert to old queue */
            AMP_RevertQueue(cidcb);
        }

            /* Finalise channel setup */
            AMP_FinaliseSetup(cidcb);
            AMP_PruneQueues(cidcb->chcb);
        }

    /* Unchoke F&EC */
    if(CID_IsFlowControl(cidcb))
    {
        /* The move initiator is also the data path resume
         * initiator */
        FLOW_AmpMoveResume(cidcb, FCOK_OFF);
    }

    /* Trigger a cleanup */
    return CID_EV_CID_AMP_CLEANUP_REQ;
}

/*! \brief AMP link loss indication

    The DM has informed us that the AMP channel for this CID has
    dropped. Send a link loss indication to the upper layer.
*/
fsm_event_t CID_FsmAmpDisconnectInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    hci_error_t *reason = (hci_error_t*)arg2;

    /* Mark channel as down */
    AMP_SetState(cidcb, AMPST_DOWN);

    /* Choke F&EC traffic on this CID as channel is down. Note that
     * this is NOT a move */
    if(CID_IsFlowControl(cidcb))
    {
        FLOW_Choke(cidcb, FCOK_ON);
    }

    /* Inform upper layer */
    L2CA_AmpLinkLossInd(cidcb, *reason);
    return CID_EVENT_NULL;
}

/*! \brief Cleanup AMP instance

    See if we can cleanup (get rid of) the AMP instance.
*/
fsm_event_t CID_FsmAmpCleanupReq(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T*)arg1;
    AMPINFO_T *info = cidcb->amp_info;
    CsrBool clear;
    PARAM_UNUSED(arg2);

    /* Cleanup queues */
    AMP_PruneQueues(cidcb->chcb);

    /* May happen during failed moves */
    if(info == NULL)
    {
        return CID_EVENT_NULL;
    }

    /* Default to freeable */
    clear = TRUE;

    /* Only purge when either completely dead or alive */
    if((info->amp_state == AMPST_NULL) ||
       (info->amp_state == AMPST_RUNNING))
    {
        /* If using AMP, keep structure */
        if(info->local_amp != L2CA_AMP_CONTROLLER_BT)
        {
            clear = FALSE;
        }
    }
    else
    {
        /* Setting up or moving AMP - keep things */
        clear = FALSE;
    }

    if(clear)
    {
        /* Free the entire amp_info structure */
        AMP_FreeInfo(cidcb);
    }

    return CID_EVENT_NULL;
}

#endif /* INSTALL_AMP_SUPPORT */
