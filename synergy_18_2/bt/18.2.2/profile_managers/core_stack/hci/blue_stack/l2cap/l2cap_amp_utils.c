/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_amp_utils.c

\brief  Different AMP queue and utility functions. Note that the queue
        updating functions are host-only as they would prevent us from
        doing smoke-tests on-chip.
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

#ifdef INSTALL_AMP_SUPPORT

#include "l2cap_common.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_control.h"
#include "l2cap_interface.h"
#include "dm_amp_interface.h"
#include "l2cap_amp_utils.h"
#include "l2caplib.h"

/*! \brief Allocate AMP information structure for CIDCB

    Possibly allocate a new AMPINFO_T for a CIDCB
*/
void AMP_AllocInfo(CIDCB_T *cidcb)
{
    if(cidcb->amp_info == NULL)
    {
        cidcb->amp_info = zpnew(AMPINFO_T);
    }
}

/*! \brief Free AMP information structure for CIDCB

    If any AMP information was attached to a CIDCB, free it.
*/
void AMP_FreeInfo(CIDCB_T *cidcb)
{
    if(cidcb->amp_info != NULL)
    {
        CsrPmemFree(cidcb->amp_info);
        cidcb->amp_info = NULL;
    }
}

/*! \brief Swap the current controller IDs

    When moving a channel we need to store backups of the old AMP
    controller IDs encase something goes wrong and we need to revert
    to the old AMP.
*/
void AMP_SwapControllerIds(CIDCB_T *cidcb)
{
    l2ca_controller_t contr;
    AMPINFO_T *info;

    AMP_InfoSanity(cidcb);
    info = cidcb->amp_info;

    contr = info->old_local_amp;
    info->old_local_amp = info->local_amp;
    info->local_amp = contr;

    contr = info->old_remote_amp;
    info->old_remote_amp = info->remote_amp;
    info->remote_amp = contr;
}

/*! \brief Disconnect a new or old AMP logical channel

    Disconnect either the "new" or the "old" AMP logical channel. The
    new/old bits are primarily used for move channel procedures.
*/
void AMP_DisconnectReq(CIDCB_T *cidcb, CsrBool isnew)
{
    AMPINFO_T *info;
    l2ca_controller_t ctrl;

    AMP_InfoSanity(cidcb);
    info = cidcb->amp_info;
    ctrl = (l2ca_controller_t)(isnew
                               ? info->local_amp
                               : info->old_local_amp);

    if((ctrl != L2CA_AMP_CONTROLLER_BT))
    {
        dm_amp_disconnect_req(CH_GET_ACL(cidcb->chcb),
                              cidcb->local_cid,
                              ctrl);
    }
}

/*! \brief Return the transmit queue for a CIDCB

    If a CIDCB is AMP enabled, return the AMP queue, otherwise default
    to the CHCB basic queue
*/
TXQUEUE_T *AMP_GetTxQueue(CIDCB_T *cidcb)
{
    AMPINFO_T *amp_info = cidcb->amp_info;

    if((amp_info != NULL) &&
       (amp_info->queue != NULL))
    {
        return amp_info->queue;
    }
    else
    {
        return &(cidcb->chcb->queue);
    }
}

/*! \brief Prune unused AMP queues

    Scan the CHCB list of AMP queues and free unused ones, based on
    the local AMP controller ID.

    The logical handle lookup cache is also cleaned up, if possible.
*/
void AMP_PruneQueues(L2CAP_CHCB_T *chcb)
{
    TXQUEUE_T **chcb_pn;
    TXQUEUE_T *chcb_q;
    CIDCB_T *cidcb;

    /* Sanity check */
    if(chcb == NULL)
    {
        return;
    }

    /* We can only prune queues if we are NOT already creating or
     * moving any around -- otherwise we may throw stuff out that's
     * currently being set up */
    for(cidcb = chcb->cidcb_list; cidcb != NULL; cidcb = cidcb->next_ptr)
    {
        if(CID_IsAmp(cidcb))
        {
            if((cidcb->amp_info->amp_state != AMPST_RUNNING) &&
               (cidcb->amp_info->amp_state != AMPST_NULL))
            {
                /* Setup in progress. Bail out */
                return;
            }
        }
    }

    /* Double indirect looping of CHCB list. Note that the 'update
     * statement' is a bit tricky: We manually update the 'chcb_pn'
     * pointer in the code below - this allows us to stay in the loop
     * even after elements have been deleted */
    for(chcb_pn = &chcb->amp_queues; (chcb_q = *chcb_pn) != NULL; /* empty */)
    {
        /* Simple scan of the CIDCBs */
        for (cidcb = chcb->cidcb_list;cidcb != NULL; cidcb = cidcb->next_ptr)
        {
            /* Check CIDCB amp-info block against CHCB queue */
            if (CID_IsAmp(cidcb) &&
                cidcb->amp_info->local_amp == chcb_q->controller_id)
            {
                break;
            }
        }

        /* If there are no users left, proceed with prune */
        if (cidcb == NULL)
        {
            LOGIC_Q_LOOKUP_T *pl, **ppl;

            /* Find and remove the MCB lookup cache element */
            ppl = AMP_LogicalLookup(chcb_q->logical_handle);
            if ((pl = *ppl) != NULL)
            {
                *ppl = pl->next;
                CsrPmemFree(pl);
            }

            /* Free the queue and members  */
            (void)CH_FlushPendingQueueWithCid(chcb_q,
                                              L2CA_CID_INVALID,
                                              L2CA_DATA_SILENT);

            /* Really remove queue - unless it's BR/EDR. Also note
             * that we can only flush the done queue if it's an AMP -
             * otherwise the credits won't match up! */
            if(chcb_q->controller_id != L2CA_AMP_CONTROLLER_BT)
            {
                (void)CH_FlushDoneQueue(chcb_q, L2CA_DATA_SILENT);
                *chcb_pn = chcb_q->next_ptr;

                CsrPmemFree(chcb_q);
                chcb_q = NULL;
            }
            else
            {
                /* Don't evict the BR/EDR queue */
                chcb_pn = &chcb_q->next_ptr;
            }
        }
        else
        {
            /* Queue has users - skip */
            chcb_pn = &chcb_q->next_ptr;
        }
    }
}

/*! \brief Scan for, and possibly create, a new AMP queue

    When we received a new logical handle we need to link the AMP
    information with the logical handle (and the transmit queue).
    This function will scan the CHCB queues for a match and then
    return the reference. If the queue didn't already exist, it will
    be created.

    The function also automatically updates the logical lookup cache
    if needed.
*/
TXQUEUE_T *AMP_CreateQueue(CIDCB_T *cidcb, DM_AMPM_CONNECT_CHANNEL_RSP_T *prim)
{
    TXQUEUE_T *queue;
    amp_link_id_t logical = prim->logical_link_id;
    L2CAP_CHCB_T *chcb = cidcb->chcb;
    l2ca_controller_t controller = cidcb->amp_info->local_amp;

    if(controller == L2CA_AMP_CONTROLLER_BT)
    {
        /* Don't create queue for BR/EDR - we already have the CHCB
         * for that */
        return &(chcb->queue);
    }

    /* Scan CHCB list of queues */
    for(queue = chcb->amp_queues; queue != NULL; queue = queue->next_ptr)
    {
        if((queue->controller_id == controller) &&
           (queue->logical_handle == logical))
        {
            /* Match found */
            break;
        }
    }

    /* If no match found, create one and add to list */
    if(queue == NULL)
    {
        queue = zpnew(TXQUEUE_T);
        queue->logical_handle = logical;
        queue->controller_id = controller;
        queue->physical_handle = prim->physical_link_id;
        queue->hci_data_queue = prim->hci_data_queue;

        /* Insert */
        queue->next_ptr = chcb->amp_queues;
        chcb->amp_queues = queue;
    }

    /* Make sure a lookup structure exists, otherwise create it */
    if (*AMP_LogicalLookup(logical) == NULL)
    {
        LOGIC_Q_LOOKUP_T *pl = pnew(LOGIC_Q_LOOKUP_T);
        pl->logical_handle = logical;
        pl->queue = queue;
        pl->chcb = chcb;

        /* Add to head of list */
        pl->next = mcb.logical_lookup;
        mcb.logical_lookup = pl;
    }

    return queue;
}

/*! \brief Re-link a CIDCB to a new AMP queue

     If a channel has been moved we may need to update the transmit
     queue for a CIDCB to point to a new one.
*/
CsrBool AMP_LinkQueue(CIDCB_T *cidcb,
                     amp_link_id_t logical)
{
    L2CAP_CHCB_T *chcb = cidcb->chcb;

#ifdef BUILD_FOR_HOST
    TXQUEUE_T *queue;

    /* Scan CHCBs to look for the logical handle */
    for(queue = chcb->amp_queues; queue != NULL; queue = queue->next_ptr)
    {
        if(queue->logical_handle == logical)
        {
            /* Found. Update link and exit */
            cidcb->amp_info->queue = queue;
            return TRUE;
        }
    }
    return FALSE;
#else
    /* Always use BR/EDR queue on-chip */
    PARAM_UNUSED(logical);
    cidcb->amp_info->queue = &(chcb->queue);
    return TRUE;
#endif
}

/*! \brief Revert to old controller

    If the entire setup was successful except for the final move
    channel confirm then we are effectively using an invalid
    controller queue. We then need to revert to the old
    phys/logical/hci_task values
*/
void AMP_RevertQueue(CIDCB_T *cidcb)
{
    AMPINFO_T *info = cidcb->amp_info;
    if(info && info->logical_ok)
    {
        DM_AMPM_CONNECT_CHANNEL_RSP_T p;
        p.logical_link_id = info->old_logical;
        p.physical_link_id = info->old_physical;
        p.hci_data_queue = info->old_hci_data_queue;
        info->queue = AMP_CreateQueue(cidcb, &p);
    }
}

/*! \brief Finalise queue setup

    After a move or create channel has been _completed_, the state of
    the AMP information must be updated.
*/
void AMP_FinaliseSetup(CIDCB_T *cidcb)
{
    AMPINFO_T *info;
    AMP_InfoSanity(cidcb);
    info = cidcb->amp_info;

    /* Disconnect old controller */
    if(info->old_local_amp != L2CA_AMP_CONTROLLER_BT)
    {
        dm_amp_disconnect_req(CH_GET_ACL(cidcb->chcb),
                              cidcb->local_cid,
                              info->old_local_amp);
        info->old_local_amp = L2CA_AMP_CONTROLLER_BT;
    }

    /* AMP enabled and now running -- except if we're creating the
     * link. In that case we also need to do the configuration */
    if(info->amp_state != AMPST_CREATING)
    {
        info->amp_state = AMPST_RUNNING;
    }
}

/*! \brief Trigger creation of logical channel

    During a move we must create the logical channel. This may involve
    generating a set of default (best effort) flow specifications if
    such doesn't exist already
*/
void AMP_KickLogicalChannelReq(CIDCB_T *cidcb)
{
    /* Make sure we have the flowspecs */
    if(cidcb->tx_flowspec == NULL)
    {
        cidcb->tx_flowspec = L2CA_AllocFlowspec();
    }
    if(cidcb->rx_flowspec == NULL)
    {
        cidcb->rx_flowspec = L2CA_AllocFlowspec();
    }

    /* Ask the DM to set up the channel, if not already requested or
     * completed */
    if(!cidcb->amp_info->logical_pending &&
       !cidcb->amp_info->logical_ok)
    {
        cidcb->amp_info->logical_pending = TRUE;
        dm_amp_logical_connect_req(CH_GET_ACL(cidcb->chcb),
                                   cidcb->local_cid,
                                   cidcb->amp_info->local_amp,
                                   cidcb->tx_flowspec,
                                   cidcb->rx_flowspec);
    }
}

/*! \brief Trigger confirmation-request

    When both a move-channel-response and the logical channel setup
    have been completed, we must start the confirmation phase of the
    move channel. This function analyses the current state and starts
    the next if possible
*/
void AMP_KickConfirmationReq(CIDCB_T *cidcb)
{
    SIG_SIGNAL_T *sig_ptr;

    /* If logical channel setup has completed, proceed with the move
     * confirmation */
    if(!cidcb->amp_info->logical_pending &&
       (cidcb->amp_info->move_result != L2CA_MOVE_INTERNAL_PENDING))
    {
        /* The wait-for-confirm-response state is internal to AMP */
        AMP_SetState(cidcb, AMPST_CONFIRM_LOCAL);

        /* Send move-channel-confirm-request */
        sig_ptr = SIG_SignalMoveChannelConfirmReq(cidcb->local_cid,
                                                  (CsrUint16)(cidcb->amp_info->logical_ok
                                                             ? L2CA_MOVE_CONFIRM_SUCCESS
                                                             : L2CA_MOVE_CONFIRM_REFUSED));
        SIGH_SignalSend(cidcb->chcb, sig_ptr);
    }
}

LOGIC_Q_LOOKUP_T **AMP_LogicalLookup(amp_link_id_t logical_handle)
{
    LOGIC_Q_LOOKUP_T *pl, **ppl;

    for (ppl = &mcb.logical_lookup;
            (pl = *ppl) != NULL && pl->logical_handle != logical_handle;
            ppl = &pl->next)
        ;

   return ppl;
}

#endif /* INSTALL_AMP_SUPPORT */
