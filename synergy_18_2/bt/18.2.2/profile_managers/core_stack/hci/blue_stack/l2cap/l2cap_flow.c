/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_flow.c

\brief  This file implements the L2CAP flow & error control extensions.
*/

#include "csr_synergy.h"

#include "l2cap_flow.h"

#include "csr_util.h"

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"

#if defined(INSTALL_L2CAP_CRC) && !defined(BUILD_FOR_HOST)
#include "lc.h"
#endif

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_interface.h"
#include "l2caplib.h"
#ifndef INSTALL_L2CAP_CRC
#include "l2cap_crc_flow.h"
#endif
#include "l2cap_cid_fsm_definitions.h"
#ifdef INSTALL_AMP_SUPPORT
#include "dm_amp_interface.h"
#endif

#ifdef INSTALL_L2CAP_DEBUG
#ifdef BUILD_FOR_HOST
#include "csr_exceptionhandler.h"
#define DEBOUT(x)  CsrGeneralWarning(NULL, (CsrUint16)(x), 0, "DEBOUT")
#define DEBDRP(x)  CsrGeneralWarning(NULL, (CsrUint16)(x), 0, "DEBDRP")
#else

#define DEBOUT(x)  { fault((faulCsrSchedTid)(x)); }
#define DEBDRP(x)  { fault((faulCsrSchedTid)(x)); }
#endif
#else
#define DEBOUT(x)
#define DEBDRP(x)
#endif

/* Helper macros for different modes */
#define FLOW_IsEnhancedRet(flow) ((flow)->config.mode == L2CA_FLOW_MODE_ENHANCED_RETRANS)
#define FLOW_IsStreaming(flow) ((flow)->config.mode == L2CA_FLOW_MODE_STREAMING)

/* Helper macros for AMP and extended window sizes */
#ifdef INSTALL_AMP_SUPPORT
#define FLOW_IsExtWindow(flow)  ((flow)->config.ext_window)
#define FLOW_MaxSequence(flow)  ((flow)->config.ext_window ? L2CAP_EXT_MAX_SEQUENCE : L2CAP_STD_MAX_SEQUENCE)
#define FLOW_ControlSize(flow)  ((flow)->config.ext_window ? L2CAP_SIZEOF_EXT_CONTROL_FIELD : L2CAP_SIZEOF_STD_CONTROL_FIELD)
#define FLOW_HeaderSize(flow)   ((flow)->config.ext_window ? L2CAP_SIZEOF_EXT_HEADER : L2CAP_SIZEOF_STD_HEADER)
#else
#define FLOW_IsExtWindow(flow)  (FALSE)
#define FLOW_MaxSequence(flow)  (L2CAP_STD_MAX_SEQUENCE)
#define FLOW_ControlSize(flow)  (L2CAP_SIZEOF_STD_CONTROL_FIELD)
#define FLOW_HeaderSize(flow)   (L2CAP_SIZEOF_STD_HEADER)
#endif
#define FLOW_FooterSize(flow)   ((flow)->config.fcs ? L2CAP_SIZEOF_FCS_FIELD : 0)

/* Helper macros for timers */
#define FLOW_RetransRunning(flow) ((flow)->defer_retrans != 0 \
        || (flow)->retrans_timer != 0)
#define FLOW_MonitorRunning(flow) ((flow)->defer_monitor \
        || (flow)->monitor_timer != 0)
#define FLOW_DelayedRunning(flow) ((flow)->ack_timer != 0)
#define FLOW_DelayedAckStop(flow) timer_cancel(&((flow)->ack_timer))

/* Reset poll retransmission counter */
#define FLOW_ResetPollRetrans(flow) ((flow)->local_poll_retrans = 0)

/* Sanity checks */
#ifdef INSTALL_L2CAP_DEBUG
static void FLOW_Debug_Check_Guard_Flow(const FLOWINFO_T *const flow)
{
    if (flow == NULL || flow->guard != 0xBEEF)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
}

static void FLOW_Debug_Check_Guard_All(const CIDCB_T *const cidcb,
                                       const FLOWINFO_T *const flow)
{
    if (cidcb == NULL || cidcb->flow != flow)
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);

    FLOW_Debug_Check_Guard_Flow(flow);
}

#define DEBUG_CHECK_GUARD_FLOW(flow) FLOW_Debug_Check_Guard_Flow((flow))
#define DEBUG_CHECK_GUARD_ALL(cidcb, flow) \
    FLOW_Debug_Check_Guard_All((cidcb), (flow))
#else
#define DEBUG_CHECK_GUARD_FLOW(flow)
#define DEBUG_CHECK_GUARD_ALL(cidcb, flow)
#endif

/* Prototypes */
static void FLOW_RemoteAckFlush(CIDCB_T *cidcb, FLOWINFO_T *flow, FLOWSEQ_T req_seq, l2ca_data_result_t result);
static void FLOW_MonitorTimerStart(CIDCB_T *cidcb, FLOWINFO_T *flow, FLOWDEFER_T defer);
static void FLOW_RetransTimerStart(CIDCB_T *cidcb, FLOWINFO_T *flow, FLOWDEFER_T defer);
static void FLOW_RaiseException(FLOWINFO_T *flow, FLOWSTATE_T new_state);
static FLOWSEQ_T FLOW_RemoteAvailable(const FLOWINFO_T *flow);

/*! \brief Debug interface to set interval/number of packets to drop

    Allow application to specify the interval and number of packets
    to drop in the rx and tx-path.
*/
#ifdef INSTALL_L2CAP_DEBUG
void FLOW_DebugDrop(CIDCB_T *cidcb,
                    CsrUint16 rx_i, CsrUint16 rx_n,
                    CsrUint16 tx_i, CsrUint16 tx_n)
{
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    if(cidcb->flow == NULL)
    {
        return;
    }
    flow = cidcb->flow;

    flow->drop_rx_interval = rx_i;
    flow->drop_rx_number   = rx_n;
    flow->drop_tx_interval = tx_i;
    flow->drop_tx_number   = tx_n;
}
#endif


/*! \brief Receiver drop checker

    Count a receive-packet, and figure out whether it should be dropped.
    If yes, we return TRUE, otherwise FALSE.
*/
#ifdef INSTALL_L2CAP_DEBUG
static CsrBool FLOW_DebugDropRx(FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Dropping not enabled */
    if(flow->drop_rx_interval == 0)
    {
        return FALSE;
    }

    /* Track numbers for interval and number */
    (flow->drop_cnt_rx)++;
    if(flow->drop_active_rx)
    {
        if(flow->drop_cnt_rx >= flow->drop_rx_number)
        {
            flow->drop_active_rx = FALSE;
            flow->drop_cnt_rx = 0;
        }
    }
    else
    {
        if(flow->drop_cnt_rx >= flow->drop_rx_interval)
        {
            flow->drop_active_rx = TRUE;
            flow->drop_cnt_rx = 0;
        }
    }
    return flow->drop_active_rx;
}
#endif


/*! \brief Transmitter drop checker

    Count a receive-packet, and figure out whether it should be dropped.
    If yes, we return TRUE, otherwise FALSE.
*/
#ifdef INSTALL_L2CAP_DEBUG
static CsrBool FLOW_DebugDropTx(FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Dropping not enabled */
    if(flow->drop_tx_interval == 0)
    {
        return FALSE;
    }

    /* Track numbers for interval and number */
    (flow->drop_cnt_tx)++;
    if(flow->drop_active_tx)
    {
        if(flow->drop_cnt_tx >= flow->drop_tx_number)
        {
            flow->drop_active_tx = FALSE;
            flow->drop_cnt_tx = 0;
        }
    }
    else
    {
        if(flow->drop_cnt_tx >= flow->drop_tx_interval)
        {
            flow->drop_active_tx = TRUE;
            flow->drop_cnt_tx = 0;
        }
    }
    return flow->drop_active_tx;
}
#endif

/*! \brief Possibly reschedule ourself into the tx-queue

    If there's a remote chance that we have something to send,
    reschedule a presend-event. The con_handle code will then call us
    when the ACL is ready to receive data.

    Please make sure that you don't enter a busy-loop due to the
    remote window size being full!

    Also note that we can not guarantee that neither the CIDCB or flow
    structure will not have been freed before the Presend function is
    called - we will need to check for that once we're actually in the
    send loop.
*/
static void FLOW_Reschedule(CIDCB_T *cidcb, FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    if(!flow->scheduled)
    {
        if((flow->tx_head != NULL) ||
           (flow->rt_head != NULL) ||
           (flow->state > FST_ACK) ||
           (flow->remote_poll) ||
           (flow->req_seq_last != flow->req_seq_next))
        {
            flow->scheduled = TRUE;
            CH_DataTxFecCallback(cidcb);
        }
    }
}

/*! \brief Raise exception level due to acking

    This is the timer-function for the delayed ack. All we do
    is (possibly) raise the exception-level to send out an ack.
*/
static void FLOW_DelayedAck(CsrUint16 mi, void *mv)
{
    CIDCB_T *cidcb = (CIDCB_T*)mv;
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    PARAM_UNUSED(mi);
    flow = cidcb->flow;
    flow->ack_timer = 0;

    /* Raise exception-level to ACK such that an ack will be sent if
     * nothing more important is pending */
    FLOW_RaiseException(flow, FST_ACK);
    FLOW_Reschedule(cidcb, flow);
}

/*! \brief Start delayed acknowledgment timer */
static void FLOW_DelayedAckStart(FLOWINFO_T *flow, CIDCB_T *cidcb)
{
    if(!FLOW_DelayedRunning(flow))
    {
        timer_start(&flow->ack_timer,
                    flow->config.ack_delay,
                    FLOW_DelayedAck,
                    0,
                    cidcb);
    }
}

/*! \brief Stop monitor timer

    Stop monitor timer and clear the deferred timer flag.
*/
static void FLOW_MonitorTimerStop(FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);
    timer_cancel(&flow->monitor_timer);
    flow->defer_monitor = FALSE;
}

/*! \brief Stop retransmission timer

    Stop retransmission tiemr, but do NOT touch the deferred counter
    as we rely on the HCI send/ncp events to do that for us.
*/
/* #define FLOW_RetransTimerStop(flow) timer_cancel(&((flow)->retrans_timer))*/
static void FLOW_RetransTimerStop(FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);
    timer_cancel(&flow->retrans_timer);
}

/*! \brief Change exception level

    Escalate our exception level, if permitted. This includes setting
    stuff up for REJ/SREJ and possible also just sending out an ACK.
    Note that we do NOT automatically start the callback insertion, as
    it may be required raise the exception-level several times...
*/
static void FLOW_RaiseException(FLOWINFO_T *flow,
                                FLOWSTATE_T new_state)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Streaming mode doesn't use exceptions */
    if(!FLOW_IsStreaming(flow))
    {
        /* Override old exception level if state is "don't care" */
        if((flow->state == FST_CLEAR) ||
           (flow->state == FST_ACK))
        {
            FLOW_ResetPollRetrans(flow);
            flow->state = new_state;

            /* Stop monitor timer such that we can reschedule
             * ourselves normally */
            FLOW_MonitorTimerStop(flow);
        }
        else
        {
            /* Tried to enter exception while already there. Ignore
             * this. */
        }
    }
}

/*! \brief Store a backup packet on the "re_head" queue

    We need to backup packets for all modes so we can generate correct
    acks and most importantly we need to be able to retransmit
    them. Both the acking and retransmit code requires the backup
    queue (re_head) to be ordered. This function will put a element
    into the correct position in the backup queue.
*/
static void FLOW_FrameBackup(FLOWINFO_T *flow, FLOWPKT_T *pkt)
{
    FLOWPKT_T *curr;
    FLOWSEQ_T me;

    DEBUG_CHECK_GUARD_FLOW(flow);

    /* The simple case */
    pkt->next = NULL;
    curr = flow->re_head;
    me = pkt->seq;
    if(curr == NULL)
    {
        flow->re_head = pkt;
        return;
    }

    /* If we're between the minimum and queue start, we must insert
     * ourselves as the new head */
    if((me >= flow->expected_ack_seq) &&
       (me < curr->seq))
    {
        pkt->next = curr;
        flow->re_head = pkt;
        return;
    }

    /* Otherwise we just find the spot where we're larger than the
     * current value and smaller than next. We must also catch the
     * point where the sequence numbers wrap (curr > next) */
    for(curr = flow->re_head; curr->next != NULL; curr = curr->next)
    {
        FLOWSEQ_T current = curr->seq;
        FLOWSEQ_T next = curr->next->seq;

        /* We must always be larger than the current */
        if(me > current)
        {
            /* And smaller than the next -- or catch the wrap */
            if((me < next) || (current > next))
            {
                break;
            }
        }
    }

    /* Either position was found, or loop ended. We simply insert
     * ourselves as the next-in-list */
    pkt->next = curr->next;
    curr->next = pkt;
}

/*! \brief Retransmit all unacknowledged packets

    In standard retransmission mode we can only retransmit the whole
    lot. This basically means putting all unacked packets back on the
    retransmission queue.
*/
static void FLOW_RetransmitAll(CIDCB_T *cidcb,
                               FLOWINFO_T *flow)
{
    PARAM_UNUSED(cidcb);

    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Put all element on the rt back on the re queue. This ensures
     * that we cope with multiple REJs before the entire rt-queue was
     * cleared. Note that re-queue must always be sorted whereas the
     * rt-queue does not need to be */
    while(flow->rt_head)
    {
        FLOWPKT_T *pkt;
        pkt = flow->rt_head;
        flow->rt_head = pkt->next;
        FLOW_FrameBackup(flow, pkt);
    }

    /* And swap the queues */
    flow->rt_head = flow->re_head;
    flow->re_head = NULL;
}

/*! \brief Search given queue for packet with matching sequence number

    \param head Pointer to head of queue.
    \param seq Sequence number.
    \returns Pointer to matching packet, or NULL if no match.
*/
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
static FLOWPKT_T *FLOW_FindPktBySeq(FLOWPKT_T *head, FLOWSEQ_T seq)
{
    while (head != NULL && head->seq != seq)
        head = head->next;

    return head;
}
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

/*! \brief Find last packet to be transmitted in full or in part.

    \param flow Pointer to flow information structure.
    \returns Pointer to last packet to be transmitted, or NULL if there
             is no longer a record of the last transmitted packet.
*/
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
FLOWPKT_T *FLOW_FindLastTransmitted(FLOWINFO_T *flow)
{
    FLOWPKT_T *pkt;
    FLOWSEQ_T max = FLOW_MaxSequence(flow);
    FLOWSEQ_T last = (max - 1 + flow->next_tx_seq) % max;

    /* If there is one still being transmitted then it must be the last.
       Otherwise search on the retransmit and wait-for-ACK queues. */
    if (((pkt = flow->tx_head) == NULL || flow->sar_offset == 0)
            && (pkt = FLOW_FindPktBySeq(flow->rt_head, last)) == NULL)
        pkt = FLOW_FindPktBySeq(flow->re_head, last);

    return pkt;
}
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

/*! \brief Retransmit single PDU

    In ERTM mode we can retransmit single PDUs. This function puts the
    designated PDU on the tail of the rt-queue so it will be picked up
    and the next tx-instant. We do not touch the exception level.
*/
static void FLOW_RetransmitSingle(CIDCB_T *cidcb,
                                  FLOWINFO_T *flow,
                                  FLOWSEQ_T req_seq)
{
    FLOWPKT_T *pkt;
    FLOWPKT_T **pnext;
    PARAM_UNUSED(cidcb);

    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Double indirect search the re-queue for matching sequence */
    for(pnext = &flow->re_head;
        (pkt = *pnext) != NULL;
        pnext = &pkt->next)
    {
        if(pkt->seq == req_seq)
        {
            /* Slice off the re-queue */
            *pnext = pkt->next;

            /* Append element to rt-queue */
            pkt->next = NULL;
            if(flow->rt_head == NULL)
            {
                flow->rt_head = pkt;
            }
            else
            {
                FLOWPKT_T *last;
                for(last = flow->rt_head; last->next != NULL; last = last->next)
                    /* Nop */;

                last->next = pkt;
            }
            return;
        }
    }
}

/*! \brief Retransmission timeout

    In flow-control mode, mark packet as lost and continue.
    In retransmission mode, start the retransmission loop.
*/
static void FLOW_RetransTimeout(CsrUint16 mi, void *mv)
{
    CIDCB_T *cidcb = (CIDCB_T*)mv;
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    PARAM_UNUSED(mi);
    flow = cidcb->flow;
    flow->retrans_timer = 0;

    if(FLOW_IsEnhancedRet(flow))
    {
        /* Enhanced mode. Query peer for last good I-frame it
         * received.  This sends out an R/RNR frame with poll set. We
         * receive the answer and then start txhe retransmission */
        FLOW_RaiseException(flow, FST_WAIT_ACK);
        FLOW_Reschedule(cidcb, flow);
    }
}

/*! \brief Monitor timeout

    In flow-control mode, transmit a new S-RR frame.
    In retranmission mode, retransmit the last S frame.
*/
static void FLOW_MonitorTimeout(CsrUint16 mi, void *mv)
{
    CIDCB_T *cidcb = (CIDCB_T*)mv;
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    PARAM_UNUSED(mi);
    flow = cidcb->flow;
    flow->monitor_timer = 0;

    /* Special case for the WAIT_FINAL state: We must Poll again, so
     * raise exception level to BUSY_CLEARED */
    if(flow->state == FST_WAIT_FINAL)
    {
        /* Leave the poll retransmit counter alone as this is
         * essentially a retransmission */
        flow->state = FST_BUSY_CLEARED;
    }
    else
    {
        /* Make sure we send an ack as a minimum and then reschedule
         * ourselves. Presend function will know what to do */
        FLOW_RaiseException(flow, FST_ACK);
    }

    FLOW_Reschedule(cidcb, flow);
}

/*! \brief Start monitor timer */
static void FLOW_MonitorTimerStart(CIDCB_T *cidcb,
                                   FLOWINFO_T *flow,
                                   FLOWDEFER_T defer)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Only allow timer or deferred timer to be started when not
     * running already, and if timer is valid... */
    if((flow->monitor_timer == 0) &&
       (flow->config.monitor_timeout != 0))
    {
        if(defer == DEFER_WAIT)
        {
            /* Raise mark so timer can be started when NCPs show up */
            flow->defer_monitor = TRUE;
        }
        else if((defer == DEFER_NOW) ||
                (defer == DEFER_PENDING))
        {
            /* Immediately start the timer */
            flow->defer_monitor = FALSE;
            timer_start(&flow->monitor_timer,
                        flow->config.monitor_timeout,
                        FLOW_MonitorTimeout,
                        0,
                        cidcb);
        }
    }
}

/*! \brief Start retransmission timer */
static void FLOW_RetransTimerStart(CIDCB_T *cidcb,
                                   FLOWINFO_T *flow,
                                   FLOWDEFER_T defer)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Only allow timer or deferred timer to be started when not
     * running already, and if timer is valid... */
    if((flow->retrans_timer == 0) &&
       (flow->config.retrans_timeout != 0))
    {
        CsrBool start;
        switch(defer)
        {
            case DEFER_NOW:
                start = TRUE;
                break;

            case DEFER_PENDING:
                start = flow->defer_retrans > 0;
                break;

            default:
                start = FALSE;
                break;
        }

        if(start)
        {
            timer_start(&flow->retrans_timer,
                        flow->config.retrans_timeout,
                        FLOW_RetransTimeout,
                        0,
                        cidcb);
        }
    }
}

/*! \brief Send datawrite confirms based on credits and SAR
*/
static void FLOW_FlushConfirm(CIDCB_T *cidcb,
                              FLOWINFO_T *flow,
                              FLOWSEQ_T *credits,
                              FLOWPKT_T *pkt)
{
    /* Report flushed packet to upper layer */
    if(((credits == NULL) || (*credits > 0)) &&
       (flow->credits_seq > 0) &&
       ((pkt->sdu_mode == L2CAP_SAR_UNSEGMENTED) ||
        (pkt->sdu_mode == L2CAP_SAR_END)))
    {
        if(credits != NULL)
        {
            (*credits)--;
        }
        flow->credits_seq--;
        L2CA_DataWriteCfm(cidcb,
                          pkt->context,
                          pkt->sdu_length,
                          L2CA_DATA_SUCCESS);
    }

    CH_EmptyAbortQueue(cidcb, &pkt->aborted, L2CA_DATA_LOCAL_ABORTED);
}

/*! \brief Flush single element from the rt-queue

    When the peer acks a set of packets, we want to flush them off the
    re-queue - that's easy. Now, if gaps are detected in the re-queue
    this can only mean that the packet is present on the
    rt-queue. Thus we'll need to rip a single packet of the the
    (unordered) rt-queue.
*/
static void FLOW_FlushAck(CIDCB_T *cidcb,
                          FLOWINFO_T *flow,
                          FLOWSEQ_T req_seq,
                          FLOWSEQ_T credits)
{
    CsrBool found;
    FLOWPKT_T *pkt;
    FLOWSEQ_T seq;

    for(seq = flow->expected_ack_seq;
        seq != req_seq;
        seq = (seq + 1) % FLOW_MaxSequence(flow))
    {
        found = FALSE;

        /* Quick re-queue check for head element match. This will not
         * catch all situations (but only the most likely one) because
         * we may be looking for a low seq near the end of the
         * list */
        pkt = flow->re_head;
        if((pkt != NULL) && (pkt->seq == seq))
        {
            FLOW_FlushConfirm(cidcb, flow, &credits, pkt);
            flow->re_head = pkt->next;
            CsrMblkDestroy(pkt->mblk);
            CsrPmemFree(pkt);
            found = TRUE;
        }
        else
        {
            /* Handle slower but more uncommon 'low sequence number
             * near end of list' case */
            FLOWPKT_T **ppnext;
            for(ppnext = &flow->re_head;
                (pkt = *ppnext) != NULL;
                ppnext = &pkt->next)
            {
                if(pkt->seq == seq)
                {
                    *ppnext = pkt->next;
                    FLOW_FlushConfirm(cidcb, flow, &credits, pkt);
                    CsrMblkDestroy(pkt->mblk);
                    CsrPmemFree(pkt);
                    found = TRUE;
                    break;
                }
            }
        }

        /* Item not found in re-queue, so check the rt-queue. This
         * should be very, very unlikely and only occurs if the peer
         * hasn't implemented the spec correctly, eg. if peer allows
         * REJs to overtake SREJs */
        if(!found)
        {
            /* Packet not on re-queue so scan and remove from rt */
            FLOWPKT_T **ppnext;
            for(ppnext = &flow->rt_head;
                (pkt = *ppnext) != NULL;
                ppnext = &pkt->next)
            {
                if(pkt->seq == seq)
                {
                    FLOW_FlushConfirm(cidcb, flow, &credits, pkt);
                    *ppnext = pkt->next;
                    CsrMblkDestroy(pkt->mblk);
                    CsrPmemFree(pkt);
                    found = TRUE;
                    break;
                }
            }
        }
    }
}

/*! \brief Flush elements off queue, with possible break-off and ack

    This is a generic flush queue function. It supports breaking off
    if the sequence number matches, and can also send up
    L2CA_DATAWRITE_CFMs as long as credits exist (both in the credits
    parameter and the instance flow->credits_seq).

    Note: We can only tear packets off the head, so we expect the
    queue to be sorted!

    Also note: We don't support updating of any queue tail pointer!
    The head pointer will, however, be updated.
*/
void FLOW_FlushQueue(CIDCB_T *cidcb,
                     FLOWINFO_T *flow,
                     FLOWPKT_T **head,
                     FLOWSEQ_T credits,
                     l2ca_data_result_t result)
{
    FLOWPKT_T *pkt;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    while((pkt = *head) != NULL)
    {
        /* Can we confirm this packet? */
        /* Confirm this packet in case it was a end/unsegmented SAR
         * while we still have credits to hand out */
        if((credits > 0) && (flow->credits_seq > 0) &&
           ((pkt->sdu_mode == L2CAP_SAR_UNSEGMENTED) ||
            (pkt->sdu_mode == L2CAP_SAR_END)))
        {
            credits--;
            flow->credits_seq--;

            /* Inform upper layer if allowed to do so */
            if(result != L2CA_DATA_SILENT)
            {
                L2CA_DataWriteCfm(cidcb,
                                  pkt->context,
                                  pkt->sdu_length,
                                  result);
            }
        }

        CH_EmptyAbortQueue(cidcb, &pkt->aborted, result);

        /* Tear off packet and get next */
        *head = pkt->next;
        CsrMblkDestroy(pkt->mblk);
        CsrPmemFree(pkt);
    }
}

/*! \brief Flush the F&EC data/retransmit queues

    In case the channel has been killed we need to flush the queues
    and report all datawrite-reqs from the upper layer as terminated.
    That is, we completely ignore whatever is on the actual tx-queue
    (everything is callbacks there anyway) and just free our own
    stuff.
*/
static void FLOW_FlushAllQueues(CIDCB_T *cidcb,
                                l2ca_data_result_t result)
{
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow = cidcb->flow;

    /* Stop timers and reset defered counters */
    FLOW_MonitorTimerStop(flow);
    FLOW_RetransTimerStop(flow);
    FLOW_DelayedAckStop(flow);
    flow->defer_monitor = FALSE;
    flow->defer_retrans = 0;

    /* Clear retransmission queue */
    FLOW_FlushQueue(cidcb,
                    flow,
                    &(flow->rt_head),
                    (FLOWSEQ_T)0xFFFF,
                    result);

    /* Clear retransmission buffer */
    FLOW_FlushQueue(cidcb,
                    flow,
                    &(flow->re_head),
                    (FLOWSEQ_T)0xFFFF,
                    result);

    /* Clear normal transmission queue */
    FLOW_FlushQueue(cidcb,
                    flow,
                    &(flow->tx_head),
                    (FLOWSEQ_T)0xFFFF,
                    result);
    flow->tx_tail = NULL;

    /* Clear the out-of-sequence buffer. This is a receive-queue, so
     * don't send any confirmations */
    FLOW_FlushQueue(cidcb,
                    flow,
                    &(flow->oos_head),
                    0,
                    L2CA_DATA_SILENT);
}
/*! \brief Fatal error handling

    In FC/RTM/ERTM and STM mode, fatal errors may happen, and we may
    need to kill the link. This function will help cleanup the mess
    and then inject the INTERNAL_DISCONNECT into the CID state
    machine, which will then trigger a disconnect or ACL drop incase
    of fixed channels
*/
static void FLOW_FatalError(CIDCB_T *cidcb,
                            CsrMblk *mblk,
                            l2ca_disc_result_t error_code)
{
    if(mblk != NULL)
    {
        CsrMblkDestroy(mblk);
    }
    FLOW_FlushAllQueues(cidcb,
                        L2CA_DATA_LINK_TERMINATED);
    CID_DisconnectReqInternal(cidcb, error_code);
}

/* \brief Peer may be changing the busy status

   Detect if the remote peer's busy status has changed and inform
   the upper layer.

   This function is NOT allowed to raise a fatal error.
*/
static void FLOW_BusyPeer(CIDCB_T *cidcb, FLOWINFO_T *flow, CsrBool busy)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* In AMP mode, an RR can be used to automatically release choke
     * mode */
#ifdef INSTALL_AMP_SUPPORT
    if(flow->choke == FCOK_AUTO_RR)
    {
        FLOW_Choke(cidcb, FCOK_OFF);
    }
#endif

    if(busy != flow->remote_busy)
    {
        flow->remote_busy = busy;
        L2CA_BusyInd(cidcb, busy);

        if(busy)
        {
            /* Always stop retransmission timer when remote is busy */
            FLOW_RetransTimerStop(flow);
        }
        else
        {
            /* ...and always retransmit everything when condition
             * clears. In ERTM we must send an ack too (usually
             * piggy-backed) */
            FLOW_RetransmitAll(cidcb, flow);
            if(FLOW_IsEnhancedRet(flow))
            {
                FLOW_RaiseException(flow, FST_ACK);
            }
        }
    }
 }

/*! \brief Upper layer has asked us to toggle the RNR bit

    Upper layer is able to toggle the status of the R-bit or transmit
    the RNR frame, depending on which mode we're in.
*/
void FLOW_BusyReq(CIDCB_T *cidcb, CsrBool busy)
{
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow = cidcb->flow;

    if(flow->local_busy != busy)
    {
        /* Store new value of busy flag */
        flow->local_busy = busy;

        /* Raise exception to force-ack for busy state and poll for
         * busy-cleared. */
        if(FLOW_IsEnhancedRet(flow))
        {
            /* We don't want to wait for a state-change-final */
            if(flow->state == FST_WAIT_FINAL)
            {
                flow->state = FST_CLEAR;
                FLOW_ResetPollRetrans(flow);
            }

            /* When entering busy, always send the notification */
            if(busy)
            {
                FLOW_RaiseException(flow, FST_FORCE_ACK);
            }
            /* When clearing the condition, only send notification
             * if the RNR was sent in the first place */
            else if(flow->rnr_sent)
            {
                FLOW_RaiseException(flow, FST_BUSY_CLEARED);
                flow->rnr_sent = FALSE;
            }

        }

        FLOW_Reschedule(cidcb, flow);
    }
}

/*! \brief Calculate number of frames we can send

    This function is used in the channel send routine to see if we are
    allowed to send new I frames to the peer.
*/
static FLOWSEQ_T FLOW_RemoteAvailable(const FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Basically, the remote side can take up to
     *
     *   (window - (next_tx_seq - expected_ack_seq))
     *
     * packets, but we need to take the sequence number wrapping into
     * account. The rationale here is that the number of outstanding
     * packets is "what's next" minus "what's done". The number of
     * available packets is the simply window size minus outstanding
     * packets */
    return (flow->config.remote_window -
            (FLOWSEQ_T)((flow->next_tx_seq + FLOW_MaxSequence(flow) - flow->expected_ack_seq)
                        % FLOW_MaxSequence(flow)));
}

/*! \brief Calculate the number of locally unacknowledged frames

    Figure out how many received frames we have not yet sent any
    acknowledgment for.
*/
static FLOWSEQ_T FLOW_LocalAckDiscrepancy(const FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Calculate difference between req_seq_next and req_seq_last while
     * taking the wrap into account */
    return (FLOWSEQ_T)((flow->req_seq_next + FLOW_MaxSequence(flow) - flow->req_seq_last)
                       % FLOW_MaxSequence(flow));
}

/*! \brief Flush retransmit buffer

    In retransmission mode when the peer acknowledges packets we
    should flush them from the buffer. This function will _never_
    raise the exception level, so it's quite safe to invoke this
    function _before_ processing any P/F bits or frame types.
*/
static void FLOW_RemoteAckFlush(CIDCB_T *cidcb,
                                FLOWINFO_T *flow,
                                FLOWSEQ_T req_seq,
                                l2ca_data_result_t result)
{
    FLOWSEQ_T credits;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Make sure that we have actually progressed */
    if(req_seq == flow->expected_ack_seq)
    {
        return;
    }

    /* Calculate how many "virtual credits" we can give back to the
     * upper layer */
    credits = ((req_seq + FLOW_MaxSequence(flow) - flow->expected_ack_seq)
               % FLOW_MaxSequence(flow));

    /* Clear the retransmission unack buffer */
    FLOW_FlushAck(cidcb,
                  flow,
                  req_seq,
                  credits);


    /* Stop retransmitting this packet and store new tx_seq number */
    flow->expected_ack_seq = req_seq;
    FLOW_RetransTimerStop(flow);

    /* Can we restart a timer? (retrans has been stopped in code above) */
    if(!FLOW_MonitorRunning(flow))
    {
        if((flow->re_head != NULL) && !flow->remote_busy)
        {
            /* If there's still elements on the backup queue, restart
             * the transmission timer immediately. We can't defer this
             * one as the data may already have left the Tx pipe! */
            FLOW_RetransTimerStart(cidcb, flow, DEFER_NOW);
        }
    }
}

/*! \brief Segment a payload accoding to the MPS

    The payload in PDUs may not exceed the configured MPS. If we do
    exceed it, make a mblk_duplicate off the original and send that
    off instead.

    In order to save memory, the MBLK being fragmented is always
    pointed to by the tx_head. We split off a fragment using
    mblk_duplicate() and then "replace" the "**pkt" function parameter
    in order to leave the tx_head alone until it is completely sent.

    \return TRUE if the transmit queue element can be removed, or
            FALSE if we need to fragment the element further.
*/
static CsrBool FLOW_SarSegment(FLOWINFO_T *flow,
                              FLOWPKT_T **pkt,
                              CsrUint16 *offset)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Check if we really care about this packet */
    if((*pkt)->sdu_length > flow->config.effective_mps)
    {
        CsrUint16   this_length;
        CsrMblk    *this_mblk;
        FLOWPKT_T *this_pkt;
        CsrUint8    this_sar;

        /* Calculate length of this fragment */
        this_length = CSRMIN((*pkt)->sdu_length - *offset,
                          flow->config.effective_mps);
        this_pkt = NULL;

        /* Determine continuation bits */
        if(*offset == 0)
        {
            /* This is the first one */
            this_sar = L2CAP_SAR_START;
        }
        else if(this_length == ((*pkt)->sdu_length - *offset))
        {
            /* This will be the last fragment */
            this_sar = L2CAP_SAR_END;
        }
        else
        {
            /* This is a continuation */
            this_sar = L2CAP_SAR_CONTINUE;
        }

        /* Create the fragment */
        this_mblk = CsrMblkDuplicateRegion((*pkt)->mblk, *offset, this_length);
        if(this_mblk == NULL)
        {
            /* Out of memory when creating the duplicate MBLK. We
             * should be able to recover if we just leave the queue
             * alone */
#ifdef INSTALL_L2CAP_DEBUG
            BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
            return FALSE;
        }

        /* Re-use the old queue element if this is the last
         * fragment */
        if(this_sar == L2CAP_SAR_END)
        {
            /* This is the end fragment so resuse the packet
             * structure. Also destroy original mblk as it has now
             * been completely replaced by duplicates */
            CsrMblkDestroy((*pkt)->mblk);
            this_pkt = *pkt;
        }

        /* Allocate new queue element */
        if(this_pkt == NULL)
        {
            this_pkt = xpnew(FLOWPKT_T);
            if(this_pkt == NULL)
            {
                /* Out of memory when creating the fragment queue
                 * element. We should be able to recover from this if
                 * we leave the queue alone. */
#ifdef INSTALL_L2CAP_DEBUG
                BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
                CsrMblkDestroy(this_mblk);
                return FALSE;
            }
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
            this_pkt->aborted = NULL;
#endif
        }

        /* Fill in the new queue element details, using the old
         * element when necessary */
        this_pkt->next = (*pkt)->next;
        this_pkt->mblk = this_mblk;
        this_pkt->context = (*pkt)->context;
        this_pkt->sdu_length = (*pkt)->sdu_length;
        this_pkt->sdu_mode = this_sar;
        this_pkt->seq = 0;
        this_pkt->retrans = 0;

        /* Update offset counter and "replace" queue element */
        *offset += this_length;
        (*pkt) = this_pkt;

        /* If this was the last fragment, caller can destroy original
         * queue element */
        return this_sar == L2CAP_SAR_END;
    }
    else
    {
        /* No need to fragment anything */
        return TRUE;
    }
}

/*! \brief Reassemble segmented PDUs into a single SDUs

    The strategy is first to handle SAR_START fields, as they'll tell
    us if the data already assembled should be discarded and otherwise
    prepare for the soon-to-follow PDUs. If PDU is not a SAR_START,
    just add the MBLK into the assembly chain.

    Second step is to see if we received an SAR_END, and in that case
    perform the SDU-length sanity check and pass the MBLK chain to the
    upper layers.

    Third step is a bit of cleanup and automatic ack generation for
    non-SAR_END packets, which L2CAP needs to do internally as the
    upper layer does not know about start or continuations.

    Preconditions for this function: PDUs arrive in correct tx_seq
    order and the MBLK must contain only the actual payload (no
    headers), except for the SDU-length in case of a SAR_START packet.

    \returns TRUE if function was succesful, FALSE if a fatal error
             was discovered and raised.
*/
static CsrBool FLOW_SarAssemble(CIDCB_T *cidcb,
                               FLOWINFO_T *flow,
                               FLOWSEQ_T tx_seq,
                               CsrUint8 sar,
                               CsrMblk *mblk)
{
    L2CA_DATAREAD_IND_T *ind;
    CsrBool ack = FALSE;
    CsrBool ok  = FALSE;
    CsrMblkSize mblk_length = CsrMblkGetLength(mblk);
    PARAM_UNUSED(tx_seq);

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    ind = flow->l2ca_dataread_ind;

    /* Start packets */
    if (sar == L2CAP_SAR_START)
    {
        CsrUint16 totlen;

        /* Drop existing buffer (can happen in flow control mode) */
        L2CA_FreePrimitive((L2CA_UPRIM_T*)ind);
        ind = NULL;
#ifdef BUILD_FOR_HOST
        flow->sar_count = 0;
#endif

        /* Extract total SAR SDU length */
        if(CsrBtMblkReadHead16(&mblk, &totlen))
        {
            /* Only accept this if the SDU-length <= MTU. If this isn't
             * the case, we can only ignore the entire frame (and very
             * likely end up disconnecting due to re-tx overflow) */
            if (totlen > cidcb->local_mtu)
            {
                /* If the peer sends a SDU-length > MTU, we should
                 * disconnect as this is a fatal, non-recoverable
                 * error */
                DEBDRP(0x4001);
                FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_MTU_VIOLATION);
                flow->l2ca_dataread_ind = ind;
                return FALSE;
            }

            L2CA_DataReadInd(cidcb, mblk, L2CA_DATA_PARTIAL, 1, &ind);
            ind->length = totlen  + L2CAP_SIZEOF_SDU_LEN_FIELD - mblk_length;

#ifdef BUILD_FOR_HOST
            flow->sar_count = 1;
#endif

            /* Data has been consumed and must be acked */
            ok = TRUE;
            ack = TRUE;
        }
    }
    /* Continuations and ends */
    else if (sar == L2CAP_SAR_CONTINUE || sar == L2CAP_SAR_END)
    {
        /* We always append data for both continue and end */
        ind->data   = (ind->data == NULL ? mblk : CsrMblkAddTail(mblk, ind->data));
        ind->length -= mblk_length;
        mblk = NULL;
        ind->packets++;
#ifdef BUILD_FOR_HOST
        flow->sar_count++;
#endif

        if (ind->length != 0)
        {
            /* Data has been consumed and must be acked internally */
            ok = TRUE;
            ack = TRUE;

#ifdef BUILD_FOR_HOST
            /* Because we're using chained MBLKs for the reassembly
             * there's a risk that an attacker would flood us with
             * small packets. We can work around this by coalescing
             * the MBLK once a certain threshold is reached */
            if(flow->sar_count >= L2CAP_SAR_COALESCE_LIMIT)
            {
                if(CsrMblkCoalesceToPmalloc(&ind->data))
                {
                    /* Only set count of coalesce was succesful */
                    flow->sar_count = 1;
                }
            }
#endif
        }
        else if (sar == L2CAP_SAR_END)
        {
            /* Handover MBLK to upper layers */
            ind->result = L2CA_DATA_PARTIAL_END;
            L2CA_PrimSend(ind);

            /* Remove local reference to data as upper layer must
             * consume it and acknowledge it */
            ind  = NULL;
#ifdef BUILD_FOR_HOST
            flow->sar_count = 0;
#endif
            ack = FALSE;
            ok = TRUE;
        }
    }

    if(!ok)
    {
        /* Error in assembly.
         *
         * In RTM/ERTM this is a fatal error, while in FC/STM mode
         * this may happen and we should just discard the whole lot
         * received so far. The tx_seq/SAR check is thus made before
         * this function and we assume that error handling has already
         * happend */
        L2CA_FreePrimitive((L2CA_UPRIM_T*)ind);
        ind = NULL;
        CsrMblkDestroy(mblk);
#ifdef BUILD_FOR_HOST
        flow->sar_count = 0;
#endif
    }

    flow->l2ca_dataread_ind = ind;

    /* Acknowledge that we received this PDU segment. Peer only acks
     * the last bit. */
    if(ack)
    {
        FLOW_DataReadAck(cidcb, flow, 1);
    }

    /* Success */
    return TRUE;
}



/*! \brief Store an out-of-sequence I-frame in ERTM mode

    When an tx_seq does not match the expected_tx_seq in enhanced
    retransmission mode, we must generate the list of missing tx_seq's
    such that they can be requested via SREJs.

    This function will generate the linked list from expected_tx_seq
    to the tx_seq -- if it does not already exist. All "new" elements
    will have the "missing" flag set, and the current element will be
    inserted with the MBLK and marked as "ok". In other words, what we
    do here is "expand" the oos_head linked list to keep track of what
    frames to SREJ.

    If the tx_seq wasn't the added to the tail, we scan the list again
    from head to tx_seq and re-flag any "sent" elements as "missing".

    \return TRUE if SREJ-loss or Packet-loss was detected, FALSE if all OK
*/
static CsrBool FLOW_OosStore(FLOWINFO_T *flow,
                            FLOWSEQ_T tx_seq,
                            CsrUint8 sar,
                            CsrMblk *mblk)
{
    FLOWPKT_T *target;
    FLOWPKT_T *scan;
    FLOWPKT_T **prev;
    FLOWSEQ_T curtx;
    CsrBool missing = FALSE;

    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Expend the OoS buffer from expected_tx_seq to the current
     * tx_seq, creating elements if they do not already exist */
    prev = &(flow->oos_head);
    target = flow->oos_head;
    curtx = flow->expected_tx_seq;
    while(TRUE)
    {
        /* Empty element, so create new */
        if(target == NULL)
        {
            target = zpnew(FLOWPKT_T);
            target->context = CONTEXT_MISSING;
            target->seq = curtx;
            *prev = target;
            /* flag it if we see packet loss */
            if(tx_seq != curtx)
                missing = TRUE;
        }

        /* Element found */
        if(curtx == tx_seq)
        {
            break;
        }

        prev = &(target->next);
        target = target->next;
        curtx = (curtx + 1) % FLOW_MaxSequence(flow);
    }

    /* The 'target' now points to the element in which we can store
     * the out-of-sequence I-frame */
    if(target->seq == tx_seq)
    {
        CsrMblkDestroy(target->mblk);
        target->mblk = mblk;
        target->sdu_mode = sar;
        target->context = CONTEXT_OK;

        /* If this is the tail element, we've found the highest tx_seq
         * number so far.  This number is used to detect duplicates in
         * the tx_seq validator */
        if(target->next == NULL)
        {
            /* Out-of-sequence tail. This is guaranteed to be the highest
             * tx_seq number seen so far */
            flow->highest_tx_seq = tx_seq;
        }
        else
        {
            /* Not tail - scan for lost SREJs */
            for(scan = flow->oos_head; scan != target; scan = scan->next)
            {
                if(scan->context == CONTEXT_SENT)
                {
                    scan->context = CONTEXT_MISSING;
                    missing = TRUE;
                }
            }
        }
    }

    /* Did we reflag anything? */
    return missing;
}

/*! \brief Extract, if the expected_tx_seq matches, the OoS buffer head

    If we have buffered any out-of-sequence I-frames this function
    will return the oos_head if the tx_seq matches the expected_tx_seq
    and move/progress the buffer head to the next element. If no
    elements are found, NULL is returned.
*/
static FLOWPKT_T *FLOW_OosPop(FLOWINFO_T *flow)
{
    FLOWPKT_T *pkt;

    DEBUG_CHECK_GUARD_FLOW(flow);

    if(flow->oos_head == NULL)
    {
        return NULL;
    }

    pkt = flow->oos_head;
    if((pkt->seq == flow->expected_tx_seq) &&
       (pkt->context == CONTEXT_OK))
    {
        flow->oos_head = pkt->next;
        pkt->next = NULL;
        return pkt;
    }
    else
    {
        return NULL;
    }
}

/*! \brief Extract tx_seq number of next, missing I-frame

    Scan the out-of-sequence buffer and find the first "missing"
    tx_seq.  If such an element is found, mark it as "sent", set the
    miss_seq parameter to the "missing" element just found and return
    true.  If an element could not be found, return false.
*/
static CsrBool FLOW_OosFirstMissing(FLOWINFO_T *flow, FLOWSEQ_T *miss_seq, FLOWPF_T *pf)
{
    FLOWPKT_T *pkt;
    CsrUint16 context;

    DEBUG_CHECK_GUARD_FLOW(flow);

    if((*pf == PF_FINAL) &&
       (flow->re_head == NULL) &&
       (flow->oos_head != NULL))
    {
        /* If peer sent us a poll and our retransmit buffer is empty,
         * we must retransmit the head SREJ element with F=1,
         * i.e. look for first "sent" entry */
        context = CONTEXT_SENT;
    }
    else
    {
        /* Continue to empty the list, i.e. look for first "unsent"
         * element and do not allow the final bit to be used */
        context = CONTEXT_MISSING;
        *pf = PF_NONE;
    }

    for(pkt = flow->oos_head; pkt != NULL; pkt = pkt->next)
    {
        if(pkt->context == context)
        {
            pkt->context = CONTEXT_SENT;
            *miss_seq = pkt->seq;
            return TRUE;
        }
    }

    return FALSE;
}

/*! \brief Count number of missing frames in the OoS buffer

    Count number of missing/sent frames in the OoS buffer that has the
    given "mask" flag set.
*/
static CsrUint16 FLOW_OosCountMissing(FLOWINFO_T *flow, CsrUint16 mask)
{
    CsrUint16 missing;
    FLOWPKT_T *pkt;

    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Count missing frames */
    missing = 0;
    for(pkt = flow->oos_head; pkt != NULL; pkt = pkt->next)
    {
        if(pkt->context & mask)
        {
            missing++;
        }
    }
    return missing;
}

/*! \brief Empty the out-of-sequence buffer

    If we're low on memory or we want to move from a SREJ condition to
    a full REJ exception, we must clear the OoS-buffer. This function
    does exactlty that: If flushes the buffer.
*/
static void FLOW_OosFlush(FLOWINFO_T *flow)
{
    FLOWPKT_T *pkt;

    DEBUG_CHECK_GUARD_FLOW(flow);

    while(flow->oos_head != NULL)
    {
        pkt = flow->oos_head;
        flow->oos_head = pkt->next;

        CsrMblkDestroy(pkt->mblk);
        CsrPmemFree(pkt);
    }
    flow->oos_head = NULL;
}

/*! \brief See if we can go from SREJ to REJ

    In case there's more than L2CAP_FLOW_REJ_THRESHOLD number of
    packets missing and we don't have any pending SREJ frames, use REJ
    instead of SREJ. This function will check for that condition and
    move us into the full REJ exception state if possible.
*/
static void FLOW_OosRejectThreshold(FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    if((FLOW_OosCountMissing(flow, CONTEXT_MISSING) >
        (CsrUint16)(flow->config.local_window >> L2CAP_FLOW_REJ_THRESHOLD)) &&
       (FLOW_OosCountMissing(flow, CONTEXT_SENT) == 0) &&
       (flow->state == FST_SREJ_LIST))
    {
        flow->state = FST_REJ;
        FLOW_OosFlush(flow);
        FLOW_ResetPollRetrans(flow);
    }
}

/*! \brief Validate the SAR bits in an I frame

    The BT2.1 spec section 3.3.6.5 requires us to validate segmented
    frames before we start the decode process (sigh), so we need to
    make sure that the SAR bits are correct according to the current
    SAR state. Note that this function does NOT update the state!
*/
static CsrBool FLOW_ValidateSar(FLOWINFO_T *flow,
                               CsrUint16 sar,
                               CsrUint16 payload_size)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    if (flow->l2ca_dataread_ind == NULL)
    {
        if (sar == L2CAP_SAR_UNSEGMENTED
                || (sar == L2CAP_SAR_START && payload_size >= 2))
            return TRUE;
    }
    else if (sar == L2CAP_SAR_CONTINUE || sar == L2CAP_SAR_END)
        return TRUE;

    return FALSE;
}

/*! \brief Validate the FCS field in S/I frames

    Check that the Frame Check Sequence is OK. Either this is done in
    hardware (when on-chip), if not we need to fire up the software
    CRC function.
*/
#ifdef INSTALL_L2CAP_CRC
static CsrBool FLOW_ValidateFcs(CsrMblk *mblk,
                               CsrUint16 *mblk_size,
                               CsrUint8 *header)
{
    /* Hardware FCS */
    CsrUint8 fcs[L2CAP_SIZEOF_FCS_FIELD];
    PARAM_UNUSED(header);

    if(CsrMblkReadTail(&mblk, fcs, L2CAP_SIZEOF_FCS_FIELD) != L2CAP_SIZEOF_FCS_FIELD)
    {
        return FALSE;
    }
    *mblk_size -= L2CAP_SIZEOF_FCS_FIELD;

    /* Hardware sets the FCS with to 0x0000 in case the actual FCS and
     * calculated FCS match, otherwise it's left alone */
    return fcs[0] == 0 && fcs[1] == 0;
}
#else
#define FLOW_ValidateFcs(m, s, h) \
    CRC_FLOW_CheckFcs((m), (s), (h))
#endif



/*! \brief Validate tx_seq in packet

    The tx_seq number can be either:
    - as expected
    - a duplicate (but valid)
    - valid       (but out-of-sequence)
    - invalid     (outside range)

    This function figures out what range we're in and returns the
    appropiate error code. Note that the detection order *must* be as
    stated above (which is based on the state tables in CSA1).

*/
static FLOWTXVALID_T FLOW_ValidateTxSeq(FLOWINFO_T *flow,
                                        FLOWSEQ_T tx_seq)
{
    FLOWSEQ_T hi, lo;
    DEBUG_CHECK_GUARD_FLOW(flow);

    /* Fast path for the most common case */
    if((tx_seq == flow->expected_tx_seq) || FLOW_IsStreaming(flow))
    {
        /* If there's no out-of-sequence list, this tx_seq is the
         * highest seen so far */
        if(flow->oos_head == NULL)
        {
            flow->highest_tx_seq = tx_seq;
        }

        return TXV_VALID;
    }

    /* Catch duplicate packets in the region
     * (highest_tx_seq - window) <= tx_seq < expected_tx_seq
     */
    lo = ((flow->highest_tx_seq + FLOW_MaxSequence(flow) - flow->config.local_window)
          % FLOW_MaxSequence(flow));
    hi = flow->expected_tx_seq;
    if( ((lo <= tx_seq) && (tx_seq < hi) && (hi > lo)) ||
        ((tx_seq < hi) && (hi < lo) && (lo >= tx_seq)) ||
        ((hi < lo) && (lo <= tx_seq) && (tx_seq > hi)) )
    {
        return TXV_DUPLICATE;
    }

    /* Catch good but out-of-sequence packets in the region
     * (expected_tx_seq < tx_seq <= (req_seq_last + window)
     */
    lo = flow->expected_tx_seq;
    hi = ((flow->req_seq_last + flow->config.local_window)
          % FLOW_MaxSequence(flow));
    if( ((lo < tx_seq) && (tx_seq <= hi) && (hi > lo)) ||
        ((tx_seq <= hi) && (hi < lo) && (lo > tx_seq)) ||
        ((hi < lo) && (lo < tx_seq) && (tx_seq >= hi)) )
    {
        return TXV_VALID;
    }

    /* Anything not caught already will be invalid frames */
    return TXV_INVALID;
}


/*! \brief Process a received I frame

    Investigate the tx_seq sequence number and determine whether we
    can accept the packet or if a retransmission shall be requested.
    If the data can be accepted, pass it into the reassembly function
    or directly to the app if un-segmented.

    If the tx_seq is not what we expected, figure out what the next
    step should be, ie. REJ or SREJ.

*/
static CsrBool FLOW_DataReadInformation(CIDCB_T *cidcb,
                                       FLOWINFO_T *flow,
                                       FLOWSEQ_T tx_seq,
                                       FLOWSEQ_T req_seq,
                                       CsrUint8 sar,
                                       CsrMblk *mblk,
                                       CsrUint16 payload_size)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Detect missing frames in streaming mode */
    if(FLOW_IsStreaming(flow) &&
       (tx_seq != flow->expected_tx_seq))
    {
        FLOWSEQ_T total;

        /* Calculate number of missing frames */
        total = (FLOWSEQ_T)((flow->expected_tx_seq < tx_seq)
                            ? tx_seq - flow->expected_tx_seq
                            : FLOW_MaxSequence(flow) - flow->expected_tx_seq + tx_seq);

        /* Report all frames from expected_tx_seq to tx_seq as missing
         * - there's two different ways to do this */
        if(total >= (FLOWSEQ_T)L2CAP_PKT_MASS_LOSS_THRES)
        {
            L2CA_DataReadInd(cidcb, NULL, L2CA_DATA_PKT_MASS_LOSS, total, NULL);
        }
        else
        {
            for( ; total > 0 ; total--)
            {
                L2CA_DataReadInd(cidcb, NULL, L2CA_DATA_PKT_MISSING, 1, NULL);
            }
        }
        flow->expected_tx_seq = tx_seq;
    }

    /* We check for the good case first, which should be the most
     * likely one! */
    if(tx_seq == flow->expected_tx_seq)
    {
        if(FLOW_IsEnhancedRet(flow))
        {
            if(!FLOW_ValidateSar(flow, sar, payload_size))
            {
                /* If the peer violates the SAR in enhanced/normal
                 * retransmission mode, we must disconnect */
                DEBDRP(0x4002);
                FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_SAR_VIOLATION);
                return FALSE;
            }
        }

        /* Process sequence numbers */
        flow->expected_tx_seq = (tx_seq + 1) % FLOW_MaxSequence(flow);

        /* In the REJ/SREJ exception level, this particular I frame
         * may have filled the gap, so clear the exception level */
        if((flow->state == FST_REJ) || (flow->state == FST_REJ_DATA) ||
           (((flow->state == FST_SREJ_LIST) || (flow->state == FST_SREJ_DATA)) &&
            (FLOW_OosCountMissing(flow, (CONTEXT_MISSING | CONTEXT_SENT)) == 0)))
        {
            flow->state = FST_CLEAR;
            FLOW_MonitorTimerStop(flow);
            FLOW_ResetPollRetrans(flow);
        }

        if(sar == L2CAP_SAR_UNSEGMENTED && flow->l2ca_dataread_ind == NULL)
        {
            /* Fast-path for reception of unsegmented SDUs */
            L2CA_DataReadInd(cidcb, mblk, L2CA_DATA_SUCCESS, 1, NULL);
        }
        else
        {
            /* Hand over mblk to reassembly function */
            if(!FLOW_SarAssemble(cidcb, flow, tx_seq, sar, mblk))
            {
                /* If fatal error occurred, bail out now */
                return FALSE;
            }
        }
    }

    /* If an AMP move channel procedure is running, we're not
     * allowed to raise exceptions */
#ifdef INSTALL_AMP_SUPPORT
    else if(flow->move_channel)
    {
        CsrMblkDestroy(mblk);
    }
#endif

    /* If we're in full reject mode already, we don't want anything but
    * what we asked for */
    else if(FLOW_IsEnhancedRet(flow) &&
            ((flow->state == FST_REJ) ||
             (flow->state == FST_REJ_DATA)))
    {
        DEBDRP(0x4100);
        CsrMblkDestroy(mblk);
    }

    /* Out-of-sequence handling */
    else if(FLOW_IsEnhancedRet(flow))
    {
        /* Quickly determine if we should send out a full reject */
        if(((tx_seq > flow->expected_tx_seq) &&
            (tx_seq - flow->expected_tx_seq >
             (CsrUint16)(flow->config.local_window >> L2CAP_FLOW_REJ_THRESHOLD))) ||
           ((tx_seq < flow->expected_tx_seq) &&
            ((CsrUint16)(tx_seq + FLOW_MaxSequence(flow) - flow->expected_tx_seq) >
             (CsrUint16)(flow->config.local_window >> L2CAP_FLOW_REJ_THRESHOLD))))
        {
            flow->state = FST_REJ;
            FLOW_ResetPollRetrans(flow);
            CsrMblkDestroy(mblk);
        }
        else
        {
            /* Store packet in out-of-sequence buffer and start SREJ
             * exception */
            (void)FLOW_OosStore(flow, tx_seq, sar, mblk);
            FLOW_RaiseException(flow, FST_SREJ_LIST);
            FLOW_OosRejectThreshold(flow);
        }
    }
    else
    {
        /* This shouldn't happen */
        DEBDRP(0x4004);
        CsrMblkDestroy(mblk);
    }

    /* Success in the sense that a fatal error didn't occur */
    return TRUE;
}

/*! \brief Handle poll/final bits for a frame

    Final bit can be set for both I and S frames, and S frames
    can also have the poll bit set. Handle these in this function
*/
static void FLOW_DataReadPollFinal(CIDCB_T *cidcb,
                                   FLOWINFO_T *flow,
                                   FLOWSEQ_T req_seq,
                                   CsrBool poll,
                                   CsrBool final,
                                   CsrUint8 frame)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    if(poll)
    {
        /* In poll just raise the flag for now. The Presend
         * function will then automatically send a final as soon
         * as possible */
        flow->remote_poll = TRUE;
        FLOW_RaiseException(flow, FST_ACK);
    }
    else if(final &&
            ((flow->state == FST_WAIT_ACK) || (flow->state == FST_WAIT_FINAL)))
    {
        /* Only process final when we're expecting one, and don't
           forget to flush before we handle it */
        FLOW_MonitorTimerStop(flow);
        FLOW_RemoteAckFlush(cidcb, flow, req_seq, L2CA_DATA_SUCCESS);

        /* Detect duplicate SREJs */
        if((flow->srej_actioned) &&
           (flow->srej_save_seq == req_seq) &&
           (frame == L2CAP_SBIT_SREJ))
        {
            /* Duplicate detected and already handled, so turn
             * this frame into an RR to avoid the extra and
             * illegal retransmission */
            flow->srej_actioned = FALSE;
        }
        else if(flow->state == FST_WAIT_ACK)
        {
            /* Only retransmit if peer hasn't sent us a REJ */
            if(!flow->rej_actioned)
            {
                FLOW_RetransmitAll(cidcb, flow);
            }
        }
        else if(flow->state == FST_WAIT_FINAL)
        {
            /* Peer has acknowledged that we've cleared the
             * local-busy condition: Do nothing */
        }

        /* Exception level cleared */
        FLOW_ResetPollRetrans(flow);
        flow->state = FST_CLEAR;
        flow->rej_actioned = FALSE;
    }
}

/*! \brief Move from FST_SREJ_DATA back to FST_SREJ_LIST so that any
           SREJ frames will be resent.

    \param flow Pointer to flow structure.
*/
static void FLOW_SrejRetry(FLOWINFO_T *flow)
{
    if (flow->state == FST_SREJ_DATA)
    {
        flow->state = FST_SREJ_LIST;
        FLOW_ResetPollRetrans(flow);
    }
}

/*! \brief Process a received S frame

    Process the header-bits of the S frame and act accordingly.  We
    can deal with all modes and all S-frames: RR, RNR, REJ and SREJ.
    We also handle poll and final bits and set the appropiate state
    flags accordingly.
*/
static void FLOW_DataReadSupervisor(CIDCB_T *cidcb,
                                    FLOWINFO_T *flow,
                                    FLOWCTRL_T control,
                                    FLOWSEQ_T req_seq)
{
    CsrUint8 frame;
    CsrBool poll;
    CsrBool final;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

#ifdef INSTALL_AMP_SUPPORT
    /* In AMP move procedures, we're only allowed to process the
     * req_seq field. Anything else must be ignored */
    if(flow->move_channel)
    {
        FLOW_RemoteAckFlush(cidcb, flow, req_seq, L2CA_DATA_SUCCESS);
        return;
    }

    /* Decode the frame header according to the mode */
    if(FLOW_IsExtWindow(flow))
    {
        frame = (CsrUint8)((control & L2CAP_EXT_SBIT_MASK)  >> L2CAP_EXT_SBIT_LHS);
        poll  = (CsrBool)((control & L2CAP_EXT_POLL_MASK)  >> L2CAP_EXT_POLL_LHS);
        final = (CsrBool)((control & L2CAP_EXT_FINAL_MASK) >> L2CAP_EXT_FINAL_LHS);
    }
    else
#endif
    {
        frame = (CsrUint8)((control & L2CAP_STD_SBIT_MASK)  >> L2CAP_STD_SBIT_LHS);
        poll  = (CsrBool)((control & L2CAP_STD_POLL_MASK)  >> L2CAP_STD_POLL_LHS);
        final = (CsrBool)((control & L2CAP_STD_FINAL_MASK) >> L2CAP_STD_FINAL_LHS);
    }

    /* Handle poll/final */
    FLOW_DataReadPollFinal(cidcb, flow, req_seq,
                           poll, final, frame);

    /* Decode S-frame specific parts of control field */
    switch(frame)
    {
        case L2CAP_SBIT_RR:
            {
                FLOW_RemoteAckFlush(cidcb, flow, req_seq, L2CA_DATA_SUCCESS);

                if(FLOW_IsEnhancedRet(flow))
                {
                    /* RR always disables busy */
                    FLOW_BusyPeer(cidcb, flow, FALSE);

                    if (poll)
                        FLOW_SrejRetry(flow);
                }
            }
            break;

        case L2CAP_SBIT_REJ:
            {
                FLOW_RemoteAckFlush(cidcb, flow, req_seq, L2CA_DATA_SUCCESS);

                if(FLOW_IsEnhancedRet(flow))
                {
                    /* REJ always disables busy */
                    FLOW_BusyPeer(cidcb, flow, FALSE);

                    /* If we've already sent out a poll, make sure
                     * that we don't do too many retransmissions */
                    if((flow->state == FST_WAIT_FINAL) ||
                       (flow->state == FST_WAIT_ACK))
                    {
                        flow->rej_actioned = TRUE;
                    }
                }

                if(FLOW_IsEnhancedRet(flow))
                {
                    /* Ack and flush, and then transfer all frames to the
                     * retransmission priority queue. These will
                     * automatically be picked up the the Presend
                     * callback and have the appropiate F-bit set */
                    FLOW_RetransmitAll(cidcb, flow);
                }
            }
            break;

        case L2CAP_SBIT_RNR:
            {
                if(FLOW_IsEnhancedRet(flow))
                {
                    /* RNR always enables busy, so ack and flush. We only
                     * need to ack the RNR in case anything changed, which
                     * the BusyPeer function will do */
                    FLOW_RemoteAckFlush(cidcb, flow, req_seq, L2CA_DATA_SUCCESS);
                    FLOW_BusyPeer(cidcb, flow, TRUE);

                    if (poll)
                        FLOW_SrejRetry(flow);
                }
            }
            break;

        case L2CAP_SBIT_SREJ:
            {
                if(FLOW_IsEnhancedRet(flow))
                {
                    /* Only ack and flush if poll-bit was set */
                    if(poll)
                    {
                        FLOW_RemoteAckFlush(cidcb, flow, req_seq, L2CA_DATA_SUCCESS);
                    }

                    /* SREJ always disables busy and retransmit packet */
                    flow->srej_actioned = TRUE;
                    flow->srej_save_seq = req_seq;
                    FLOW_BusyPeer(cidcb, flow, FALSE);
                    FLOW_RetransmitSingle(cidcb, flow, req_seq);
                }
            }
            break;

        default:
            /* Unknown S-frame. Ignore it. */
            break;
    }
}

/*! \brief Receive I-frames in enhanced retransmission mode

    In ERTM we can handle missing gaps in the received packets by
    means of the out-of-sequence buffer. Doing so requires that we
    "pump" the OoS if it's non-empty to get as many packets as
    possible up to the upper layer. We also need to detect lost SREJ
    frames so we can resend them.

    \returns TRUE if successful, FALSE if a fatal error was found
            and has been raised.
*/
static CsrBool FLOW_DataReadErtm(CIDCB_T *cidcb,
                                FLOWINFO_T *flow,
                                FLOWSEQ_T tx_seq,
                                FLOWSEQ_T req_seq,
                                CsrUint8 sar,
                                CsrMblk *mblk,
                                CsrUint16 payload_size)
{
    FLOWPKT_T *pkt;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Fast-path: If the OoS-buffer is empty, just pass on the data to
     * the normal receive-function. If packet is out-of-sequence the
     * normal receive function will detect this and put us into the
     * required exception-level while storing the packet in the
     * out-of-sequence-buffer */
    if(flow->oos_head == NULL)
    {
        if(!FLOW_DataReadInformation(cidcb, flow, tx_seq, req_seq,
                                     sar, mblk, payload_size))
        {
            /* Fatal error */
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    /* If an AMP channel move is progress, we're not allowed to use
     * the OoS buffers */
#ifdef INSTALL_AMP_SUPPORT
    if(flow->move_channel)
    {
        /* Discard this packet silently */
        CsrMblkDestroy(mblk);
        return TRUE;
    }
#endif

    /* Store packet in buffer and then detect if any SREJ frames have
     * been lost. This allows us to re-request retransmission of those
     * frames */
    if(FLOW_OosStore(flow, tx_seq, sar, mblk))
    {
        /* Force exception level to send out new SREJs */
        flow->state = FST_SREJ_LIST;
        FLOW_ResetPollRetrans(flow);
    }

    /* See if it's possible to enter full reject mode instead */
    FLOW_OosRejectThreshold(flow);

    /* Try to pop off as many elements from the OoS back into the real
     * receive function. Note that the pop will only return packets if
     * the OoS head element sequence number is the expected_tx_seq */
    while(TRUE)
    {
        pkt = FLOW_OosPop(flow);
        if(pkt != NULL)
        {
            /* Pop successfull. Pass data into receive function and
             * dispose off the queue element */
            if(!FLOW_DataReadInformation(cidcb, flow, pkt->seq, req_seq,
                                         pkt->sdu_mode, pkt->mblk,
                                         CsrMblkGetLength(pkt->mblk)))
            {
                /* Fatal error encountered */
                CsrPmemFree(pkt);
                return FALSE;
            }
            CsrPmemFree(pkt);
        }
        else
        {
            /* We can't pop any more bits off, break out */
            break;
        }
    }

    /* Success */
    return TRUE;
}


/*! \brief Receive a L2CAP frame

    This is where new F&EC packets are passed into the flow control
    module.  We perform the required analysis of the packet like
    CRC-16 checking and control header extraction. We then check and
    possibly use the req_seq, tx_seq and other fields from the common
    control header parts, and pass the remaining packet into the I-
    and S-frame specific handlers.

    \param cidcb Pointer to the CIDCB_T structure
    \param mblk The MBLK data - also contains the control header, SDU length and FCS
    \param payload_size Length of the MBLK
    \param header Basic CID and length header
*/
void FLOW_DataRead(CIDCB_T *cidcb,
                   CsrMblk *mblk,
                   CsrUint16 payload_size,
                   CsrUint8 *header)
{
    FLOWINFO_T *flow;
    FLOWCTRL_T control;
    FLOWSEQ_T req_seq;
    FLOWSEQ_T tx_seq;
    CsrBool sframe;
    CsrUint8 sar;
    CsrUint16 tempsize;
    CsrUint16 mps;
    CsrUint8 head[L2CAP_SIZEOF_EXT_CONTROL_FIELD];

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow = cidcb->flow;

    /* If debugging is enabled, see if we should drop this packet */
#ifdef INSTALL_L2CAP_DEBUG
    if(FLOW_DebugDropRx(flow))
    {
        CsrMblkDestroy(mblk);
        return;
    }
#endif

    /* Cache the control header sizes */
    tempsize = FLOW_ControlSize(flow);
    mps = flow->config.local_mps;

    /* Packet *must* contain enough bytes to contain the control field
     * and be less than the (MPS + headers). We assume that the MPS <=
     * MTU.  If MTU is exceeded, we should catch that later on. */
    if((payload_size < tempsize) ||
       ((payload_size > mps) &&
        (payload_size - mps >
         L2CAP_SIZEOF_MAX_HEADER - L2CAP_SIZEOF_CID_HEADER + L2CAP_SIZEOF_FCS_FIELD)))
    {
        DEBDRP(0x400f);
        CsrMblkDestroy(mblk);
        return;
    }

    /* We must check that the FCS is good before we start extracting
     * anything from the PDU */
    if(flow->config.fcs)
    {
        if((payload_size < (tempsize + L2CAP_SIZEOF_FCS_FIELD)) ||
           !FLOW_ValidateFcs(mblk, &payload_size, header))
        {
            /* FCS error */
            DEBDRP(0x4005);

#ifdef INSTALL_L2CAP_RAW_SUPPORT
            if (cidcb->chcb->raw_crc_rejects)
                L2CA_RawDataInd(cidcb->chcb->raw_handle,
                                CH_GET_TBDADDR(cidcb->chcb),
                                L2CA_DATA_CRC_FAILURE,
                                L2CA_RX,
                                cidcb->local_cid,
                                mblk);
            else
#endif
                CsrMblkDestroy(mblk);

            return;
        }
    }

    /* Extract the control field */
    if(CsrMblkReadHead(&mblk, head, tempsize) != tempsize)
    {
        /* No control field */
        DEBDRP(0x4006);
        FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_FEC_HEADER);
        return;
    }
    payload_size -= tempsize;

    /* Decode common parts of control field. Note that 'sar' and
     * 'tx_seq' are only valid for I-frames! */
#ifdef INSTALL_AMP_SUPPORT
    if(FLOW_IsExtWindow(flow))
    {
        control = (FLOWCTRL_T)UINT32_R(head, 0);
        sframe  = (CsrBool)((control & L2CAP_EXT_TYPE_MASK) >> L2CAP_EXT_TYPE_LHS);
        req_seq = (FLOWSEQ_T)((control & L2CAP_EXT_REQSEQ_MASK) >> L2CAP_EXT_REQSEQ_LHS);
        tx_seq  = (FLOWSEQ_T)((control & L2CAP_EXT_TXSEQ_MASK)  >> L2CAP_EXT_TXSEQ_LHS);
        sar     = (CsrUint8)((control & L2CAP_EXT_SAR_MASK)    >> L2CAP_EXT_SAR_LHS);
    }
    else
#endif
    {
        control = (FLOWCTRL_T)UINT16_R(head, 0);
        sframe  = (CsrBool)((control & L2CAP_STD_TYPE_MASK) >> L2CAP_STD_TYPE_LHS);
        req_seq = (FLOWSEQ_T)((control & L2CAP_STD_REQSEQ_MASK) >> L2CAP_STD_REQSEQ_LHS);
        tx_seq  = (FLOWSEQ_T)((control & L2CAP_STD_TXSEQ_MASK)  >> L2CAP_STD_TXSEQ_LHS);
        sar     = (CsrUint8)((control & L2CAP_STD_SAR_MASK)    >> L2CAP_STD_SAR_LHS);
    }

    /* Only go in to req_seq checking details if it's not the nominal
     * case, i.e.  we're not in streaming mode, and it's not the
     * expected value */
    if((req_seq != flow->expected_ack_seq) &&
       !FLOW_IsStreaming(flow))
    {
        FLOWSEQ_T r = req_seq;
        FLOWSEQ_T hi = flow->next_tx_seq;
        FLOWSEQ_T lo = flow->expected_ack_seq;
        /* Check if the req_seq is valid, which is true for
         *
         * (expected_ack_seq <= req_seq <= next_tx_seq)
         *
         * First we deal with wrapping then we exclude invalids.
         */

        if (hi < lo)
        {
            FLOWSEQ_T window_size = FLOW_MaxSequence(flow);

            hi += window_size;

            if (r < lo)
                r += window_size;
        }

        if(r < lo || r > hi)
        {
            /* Invalid frame. Discard it and dump into fatal error
             * mode on non-streaming channel */
            DEBDRP(0x4007);
            FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_FEC_SEQUENCE);
            return;
        }
    }

    /* Specific I and S-frame validation and handlers */
    if(sframe)
    {
        /* Ignore S-frames in streaming mode. This is not a fatal error! */
        if(FLOW_IsStreaming(flow))
        {
            DEBDRP(0x4008);
            CsrMblkDestroy(mblk);
            return;
        }

        /* S-frames must not have any extra data */
        if(payload_size > 0)
        {
            /* Invalid frame size is a fatal error */
            DEBDRP(0x4009);
            FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_FEC_HEADER);
            return;
        }

        /* Enhanced retransmission mode; P and F bits must not both be
         * set in S-frames */
        if(FLOW_IsEnhancedRet(flow))
        {
            FLOWCTRL_T mask = (FLOWCTRL_T)(FLOW_IsExtWindow(flow)
                    ? (L2CAP_EXT_POLL_MASK | L2CAP_EXT_FINAL_MASK)
                    : (L2CAP_STD_POLL_MASK | L2CAP_STD_FINAL_MASK));

            if ((control & mask) == mask)
            {
                /* The P-F bit is not a fatal error, but it does let
                 * us discard the packet */
                DEBDRP(0x400a);
                CsrMblkDestroy(mblk);
                return;
            }
        }

        /* Process the S-frame */
        FLOW_DataReadSupervisor(cidcb, flow, control, req_seq);
        CsrMblkDestroy(mblk);
    }
    else
    {
        /* Check tx_seq of the I-frame */
        FLOWTXVALID_T txres;
        txres = FLOW_ValidateTxSeq(flow, tx_seq);

        /* Invalid frames requires immediate fatal action. Note that
         * streaming mode is a special case in the validate
         * function */
        if(txres == TXV_INVALID)
        {
            DEBDRP(0x400b);
            FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_FEC_SEQUENCE);
            return;
        }

        /* Check frame length against MPS and SAR header for "start"
         * frames and against MTU for "continuation" and "end"
         * frames. Both are fatal errors if detected */
        if(sar == L2CAP_SAR_START)
        {
            if((payload_size < L2CAP_SIZEOF_SDU_LEN_FIELD) ||
               ((payload_size - L2CAP_SIZEOF_SDU_LEN_FIELD) > mps))
            {
                DEBDRP(0x400c);
                FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_MPS_VIOLATION);
                return;
            }
        }
        else
        {
            if(payload_size > mps)
            {
                DEBDRP(0x400d);
                FLOW_FatalError(cidcb, mblk, L2CA_DISCONNECT_MPS_VIOLATION);
                return;
            }
        }

        /* Handle final bit in ERTM now */
        if(FLOW_IsEnhancedRet(flow))
        {
            CsrBool final;

#ifdef INSTALL_AMP_SUPPORT
            if(FLOW_IsExtWindow(flow))
            {
                final = (CsrBool)((control & L2CAP_EXT_FINAL_MASK) >> L2CAP_EXT_FINAL_LHS);
            }
            else
#endif
            {
                final = (CsrBool)((control & L2CAP_STD_FINAL_MASK) >> L2CAP_STD_FINAL_LHS);
            }

            FLOW_DataReadPollFinal(cidcb, flow, req_seq,
                                   FALSE, /* not poll */
                                   final, /* final */
                                   L2CAP_SBIT_ILLEGAL);
        }

        /* Allow expected and valid frames to be read */
        if(txres == TXV_VALID)
        {
            /* ERTM knows how to handle out of sequence packets, so
             * mode has a special reader function */
            if(FLOW_IsEnhancedRet(flow))
            {
                if(!FLOW_DataReadErtm(cidcb, flow, tx_seq, req_seq,
                                      sar, mblk, payload_size))
                {
                    /* Fatal error encountered and raised. Bail out. */
                    return;
                }
            }
            else
            {
                if(!FLOW_DataReadInformation(cidcb, flow, tx_seq, req_seq,
                                             sar, mblk, payload_size))
                {
                    /* Fatal error encountered and raised. Bail out. */
                    return;
                }
            }
        }
        else
        {
            /* Silently drop duplicates */
            DEBDRP(0x400e);
            CsrMblkDestroy(mblk);
        }

        /* Use req_seq number to ack packets immediately and for all
         * valid and duplicates. We need to do this after we've
         * catched all possible fatal errors. */
        if(!FLOW_IsStreaming(flow))
        {
            FLOW_RemoteAckFlush(cidcb, flow, req_seq, L2CA_DATA_SUCCESS);
        }
    }

    /* Possible reschedule ourselves */
    FLOW_Reschedule(cidcb, flow);
}

/*! \brief Upper layer acknowledge of data read

    This function is called when we receive a L2CA_DATAREAD_RSP signal
    and when we receive start and continuation SAR frames.

    We update the next req_seq sequence number we should
    transmit. Instead of triggering a RR/RNR directly, we *may* delay
    the acknowledgment depending on the window utilisation.
*/
void FLOW_DataReadAck(CIDCB_T *cidcb,
                      FLOWINFO_T *flow,
                      FLOWSEQ_T inc)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Sanity check: Don't overrun what peer sent us. */
    if(FLOW_IsStreaming(flow) ||
       (flow->req_seq_next == flow->expected_tx_seq))
    {
        return;
    }

    /* Increase the index of the next req_seq number to transmit and
     * then see if the ack shall be sent now or later */
    if(inc)
    {
        FLOWSEQ_T unacks;
        FLOWSEQ_T window;
        flow->req_seq_next = (flow->req_seq_next + inc) % FLOW_MaxSequence(flow);

        /* If we have pending data and remote side has take it, we know we
         * will piggy-back an acknowledgment shortly */
        if((FLOW_RemoteAvailable(flow) > 0) &&
           !flow->remote_busy &&
           ((flow->tx_head != NULL) || (flow->rt_head != NULL)))
        {
            return;
        }

        /* Get number of frames that are acked, but where the ack has
         * not yet been sent to the peer */
        unacks = FLOW_LocalAckDiscrepancy(flow);
        window = flow->config.local_window;

        /* Force ack immedieately if window is "small" or that the
         * out-sent-ack discrepancy is "big" */
        if((window <= L2CAP_FLOW_IMM_ACK_MINWIN) ||
           (unacks > ((window >> L2CAP_FLOW_IMM_ACK_SHIFT) + 1)))
        {
            FLOW_RaiseException(flow, FST_ACK);
            FLOW_Reschedule(cidcb, flow);
        }
        else
        {
            FLOW_DelayedAckStart(flow, cidcb);
        }
    }
}

/*! \brief Upper layer wants to send data

    Construct the flow control queue element, fill in the details and
    append it to the tail of the queue. Any fragmentation will be done
    just-in-time in the Presend function to save as much memory as
    possible.
*/
void FLOW_DataWrite(CIDCB_T *cidcb,
                    CsrMblk *mblk,
                    CsrUint16 context)
{
    FLOWINFO_T *flow;
    FLOWPKT_T *pkt;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow = cidcb->flow;

    /* Construct queue element */
    pkt = xpnew(FLOWPKT_T);
    if(pkt == NULL)
    {
        /* Out of memory! We can't do anything but report it to upper
         * layer. If this was fatal, upper layer can always just
         * disconnect */
#ifdef INSTALL_L2CAP_DEBUG
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        L2CA_DataWriteCfm(cidcb, context, 0, L2CA_DATA_OUT_OF_MEM);
        CsrMblkDestroy(mblk);
        return;
    }

    /* Fill in data */
    pkt->mblk = mblk;
    pkt->context = context;
    pkt->sdu_length = CsrMblkGetLength(mblk);
    pkt->sdu_mode = L2CAP_SAR_UNSEGMENTED;
    pkt->next = NULL;
    pkt->seq = 0;
    pkt->retrans = 0;
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
    pkt->aborted = NULL;
#endif

    /* Use up one "virtual credit" and bump sequence counter */
    (flow->credits_seq)++;

    /* Insert PDU into flow queue */
    if(flow->tx_tail == NULL)
    {
        flow->tx_tail = pkt;
        flow->tx_head = pkt;

        /* Request ourselves to be scheduled in */
        FLOW_Reschedule(cidcb, flow);
    }
    else
    {
        flow->tx_tail->next = pkt;
        flow->tx_tail = pkt;
    }
}


/*! \brief Obtain the control header for I frames

    As the RTM/FEC, ERTM/STM and EXT modes use different control
    headers, this function should be used in order to generate the
    control header needed in I-frames.
*/
static FLOWCTRL_T FLOW_FrameGetI(FLOWINFO_T *flow,
                                 FLOWPF_T pf,
                                 FLOWPKT_T *pkt)
{
    FLOWCTRL_T control = 0;

    DEBUG_CHECK_GUARD_FLOW(flow);

#ifdef INSTALL_AMP_SUPPORT
    if(FLOW_IsExtWindow(flow))
    {
        /* Extended header. We need to apply 16-lsh'es first, and then
         * shift them upwards. After that, apply the simple stuff */
        control = (FLOWCTRL_T)(((FLOWCTRL_T)flow->req_seq_next << L2CAP_EXT_REQSEQ_LHS) |
                               ((FLOWCTRL_T)pkt->seq << L2CAP_EXT_TXSEQ_LHS) |
                               ((FLOWCTRL_T)pkt->sdu_mode << L2CAP_EXT_SAR_LHS) |
                               (pf == PF_FINAL ? L2CAP_EXT_FINAL_MASK : 0));
    }
    else
#endif
    {
        /* Enhanced header */
        control = (FLOWCTRL_T)(((FLOWCTRL_T)pkt->seq << L2CAP_STD_TXSEQ_LHS) |
                               ((FLOWCTRL_T)pkt->sdu_mode << L2CAP_STD_SAR_LHS) |
                               ((FLOWCTRL_T)flow->req_seq_next << L2CAP_STD_REQSEQ_LHS) |
                               ((FLOWCTRL_T)(pf == PF_FINAL ? L2CAP_STD_FINAL_MASK : 0)));
    }

    return control;
}

/*! \brief Obtain the control header for S frames

    As the RTM/FEC, ERTM/STM and EXT modes use different control
    headers, this function should be used in order to generate the
    control header needed in S-frames.
*/
static FLOWCTRL_T FLOW_FrameGetS(FLOWINFO_T *flow,
                                 FLOWPF_T pf,
                                 CsrUint8 frame,
                                 FLOWSEQ_T req_seq)
{
    FLOWCTRL_T control = 0;

    DEBUG_CHECK_GUARD_FLOW(flow);

#ifdef INSTALL_AMP_SUPPORT
    if(FLOW_IsExtWindow(flow))
    {
        /* Extended header. We need to apply 16-lsh'es first, and then
         * shift them upwards. After that, apply the simple stuff */
        control = (((FLOWCTRL_T)req_seq << L2CAP_EXT_REQSEQ_LHS) |
                   ((FLOWCTRL_T)frame << L2CAP_EXT_SBIT_LHS) |
                   L2CAP_EXT_TYPE_MASK |
                   (pf == PF_FINAL ? L2CAP_EXT_FINAL_MASK : 0) |
                   (pf == PF_POLL ? L2CAP_EXT_POLL_MASK : 0));
    }
    else
#endif
    {
        /* Enhanced header */
        control = (FLOWCTRL_T)(L2CAP_STD_TYPE_MASK |
                               ((FLOWCTRL_T)frame << L2CAP_STD_SBIT_LHS) |
                               ((FLOWCTRL_T)req_seq << L2CAP_STD_REQSEQ_LHS) |
                               ((FLOWCTRL_T)((pf == PF_FINAL ? L2CAP_STD_FINAL_MASK : 0))) |
                               ((FLOWCTRL_T)(pf == PF_POLL ? L2CAP_STD_POLL_MASK : 0)));
    }

    return control;
}

/*! \brief Get R/RNR supervisor bits for an acknowledge

    Depending on what mode we're in, an acknowledgement may be either
    an RR or an RNR frame. Find out and return the bits.
*/
static CsrUint8 FLOW_FrameGetAck(FLOWINFO_T *flow)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    if(FLOW_IsEnhancedRet(flow) && flow->local_busy)
    {
        /* We only use RNR in enhanced retransmission mode. Also, mark
         * the RNR as sent */
        flow->rnr_sent = TRUE;
        return L2CAP_SBIT_RNR;
    }
    else
    {
        return L2CAP_SBIT_RR;
    }
}


/*! \brief Prepare an I-frame for transmission

    This is a helper-function that will try to get the next pending
    (retransmission or first-time) I-frame from the queue and do all
    the necessary preparations (including retransmission backup)
    before it can be sent.

    This function may only called by the presender!
*/
static CsrBool FLOW_FrameGetData(CIDCB_T *cidcb,
                                FLOWINFO_T *flow,
                                TXQE_T *txqe,
                                FLOWPF_T pf,
                                FLOWCTRL_T *control,
                                CsrUint8 **retrans,
                                CsrUint16 **sdu_length)
{
    FLOWPKT_T *pkt = NULL;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* First try the retransmission priority queue */
    pkt = flow->rt_head;
    if(pkt != NULL)
    {
#ifdef INSTALL_AMP_SUPPORT
        /* For AMP move channels, we may need to resegment and
         * renumber retransmitted elements such that we stay below the
         * effective MPS */
        CsrMblkSize mblksize = CsrMblkGetLength(pkt->mblk);
        if (mblksize > flow->config.effective_mps)
        {
            FLOWSEQ_T nrtxseq;
            nrtxseq = (flow->rt_head->seq + 1) % FLOW_MaxSequence(flow);

            if(FLOW_SarSegment(flow, &pkt, &flow->move_sar_offset))
            {
                /* Progress to next element */
                flow->move_sar_offset = 0;
                flow->rt_head = pkt->next;
                if(flow->rt_head == NULL)
                {
                    /* Retransmission queue is now empty, so update
                     * MPS settings and store next_tx_seq due to the
                     * renumbering */
                    flow->next_tx_seq = nrtxseq;
                }
            }

            /* Renumber next retranmission element */
            if(flow->rt_head != NULL)
            {
                flow->rt_head->seq = nrtxseq;
            }
        }
        else
#endif
        {
            flow->rt_head = pkt->next;
        }
    }

    /* Then try to pop the normal transmission queue */
    else if((flow->tx_head != NULL) &&
            (FLOW_IsStreaming(flow) || (FLOW_RemoteAvailable(flow) > 0)))
    {
        pkt = flow->tx_head;

        /* Fragment payload according to MPS. We only need to do
         * it on the first transmissions so check it here. In case
         * the payload was fragmented, the function will return
         * true/false whether we should keep the original on the
         * queue. This is the case if there's more fragments
         * left */
        if(FLOW_SarSegment(flow, &pkt, &flow->sar_offset))
        {
            flow->tx_head = pkt->next;
            if(flow->tx_head == NULL)
            {
                flow->tx_tail = NULL;
            }

            /* Reset SAR offset when progressing the tx queue */
            flow->sar_offset = 0;
        }

        /* Set tx_seq number for packet */
        pkt->seq = flow->next_tx_seq;
        flow->next_tx_seq = (flow->next_tx_seq + 1) % FLOW_MaxSequence(flow);
    }

    /* Packet was found so perform generic I-frame preparation */
    if(pkt != NULL)
    {
        pkt->next   = NULL;
        *control    = FLOW_FrameGetI(flow, pf, pkt);
        *retrans    = &(pkt->retrans);
        *sdu_length = ((pkt->sdu_mode == L2CAP_SAR_START)
                       ? &(pkt->sdu_length)
                       : NULL);

        if(FLOW_IsEnhancedRet(flow))
        {
            /* Increase refcount on MBLK as we'll now have two
             * references to it: One from the re-queue and one from
             * the txqe. Note that pkt->mblk can be NULL if the app
             * is sending down zero-length data. */
            if (pkt->mblk != NULL)
            {
#ifdef BUILD_FOR_HOST
                CsrMblkIncRefcount(pkt->mblk, 1);
#else
                if (!CsrMblkIncRefcount(pkt->mblk, 1))
                {
                    /* We can't send this packet now, probably because we're
                       already trying to send it too many times. This should
                       never happen if the peer is behaving itself. All we
                       can do is put the packet onto the retransmission queue
                       and wait for the peer to sort itself out. */
                    FLOW_FrameBackup(flow, pkt);
                    return FALSE;
                }
#endif
            }

            txqe->mblk = pkt->mblk;
        }
        else
        {
            /* Move mblk from tx queue to txqe. No other references
             * exist */
            txqe->mblk = pkt->mblk;
            pkt->mblk = NULL;
        }

        /* Put the pkt back on the backup queue. We use this in all
         * modes to be able to send confirms with the correct length
         * and context  */
        FLOW_FrameBackup(flow, pkt);

        /* Try to send more data immediately */
        FLOW_Reschedule(cidcb, flow);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*! \brief Prepare a frame for transmission

    This function starts timers for the frame depending on the type
    and poll/final bits.
*/
static void FLOW_FramePrep(CIDCB_T *cidcb,
                           FLOWINFO_T *flow,
                           FLOWPF_T pf,
                           CsrBool i_frame)
{
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    if(pf == PF_FINAL)
    {
        /* Reset remote poll as we are sending the final. Note that
        * this may also be valid for I-frames */
        flow->remote_poll = FALSE;
    }
    else if(pf == PF_POLL)
    {
        /* Poll frames always have the monitor timer running */
        FLOW_RetransTimerStop(flow);
        if(!FLOW_MonitorRunning(flow))
        {
            FLOW_MonitorTimerStart(cidcb, flow, DEFER_WAIT);
        }
    }

    /* Figure out which timers to run for I-frames */
    if(i_frame &&
       !FLOW_RetransRunning(flow) &&
       !flow->remote_busy &&
       (flow->state != FST_WAIT_FINAL))
    {
        /* Data is being transmitted and we are not in the
         * local-busy-cleared-wait state (WAIT_FINAL), so start
         * the retranmission timer. Before that, increment the
         * deferred retransmission timer count so we can actually
         * start the deferred timer */
        flow->defer_retrans++;
        FLOW_MonitorTimerStop(flow);
        FLOW_RetransTimerStart(cidcb, flow, DEFER_WAIT);
    }
}

/*! \brief Transmit a frame

    This function will check and setup the few common bits for I and S
    frames before they can be sent. This includes starting/stopping
    any timers, checking the transmit counter adding the FCS if needed
    etc.

    The last thing we do is fill out the TXQE members such that the
    con_handle transmit loop can send the frame.
*/
static void FLOW_FrameSend(CIDCB_T *cidcb,
                           FLOWINFO_T *flow,
                           TXQE_T *txqe,
                           FLOWCTRL_T control,
                           CsrUint16 *sdu_length,
                           CsrUint8 *retrans)
{
    CsrUint16 mblk_size;
    CsrUint8 ctrl[L2CAP_SIZEOF_MAX_HEADER - L2CAP_SIZEOF_CID_HEADER];
    CsrUint8 *pctrl;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Handle max transmission counter */
    if((flow->config.max_retransmit != 0) &&
       (retrans != NULL))
    {
        /* Promote integral to avoid uncontrolled wraps with bitfield/on-host */
        if((CsrUint16)(*retrans + 1) > (CsrUint16)flow->config.max_retransmit)
        {
            /* Channel is assumed lost as retransmission counter
             * overflow has been detected. Kill txqe and close channel
             * gracefully */
            CsrMblkDestroy(txqe->mblk);
            txqe->mblk = NULL;
            txqe->headersize = 0;
            FLOW_FlushAllQueues(cidcb,
                                L2CA_DATA_LINK_TERMINATED);
            CID_DisconnectReqInternal(cidcb, L2CA_DISCONNECT_RTM_OVERFLOW);
            return;
        }
        (*retrans)++;
    }

    /* Lower exception level from ack to clear, but do rescheule
     * ourselves unconditionally to make sure we will continue to send
     * data after an ack/force ack */
    if((flow->state == FST_ACK) ||
       (flow->state == FST_FORCE_ACK))
    {
        flow->state = FST_CLEAR;
        FLOW_Reschedule(cidcb, flow);
        FLOW_ResetPollRetrans(flow);
    }
    else if(flow->state >= FST_SREJ_LIST)
    {
        FLOW_Reschedule(cidcb, flow);
    }

    /* Write control header */
    pctrl = ctrl;
#ifdef INSTALL_AMP_SUPPORT
    if(FLOW_IsExtWindow(flow))
    {
        write_uint32(&pctrl, &control);
    }
    else
#endif
    {
        mblk_size = (CsrUint16)(control & 0xFFFF);
        write_uint16(&pctrl, mblk_size);
    }

    /* Possibly write SDU length too */
    if(sdu_length != NULL)
    {
        write_uint16(&pctrl, *sdu_length);
    }

    /* Figure out whether we need to append CRC, but don't perform the
     * caculation until the packet is complete. The trailersize is
     * needed in the packet header */
    txqe->trailersize = (flow->config.fcs
                         ? L2CAP_SIZEOF_FCS_FIELD
                         : 0);

    /* Add the header */
    mblk_size = ((txqe->mblk != NULL)
                 ? (CsrMblkSize)CsrMblkGetLength(txqe->mblk)
                 : (CsrMblkSize)0);
    mblk_size += txqe->trailersize;
    CH_DataAddHeader(txqe, cidcb->remote_cid, mblk_size,
                     ctrl, (CsrUint8)(pctrl - ctrl));

    /* In software CRC mode we must calculate the FCS manually. This
     * must be the last step we do perform the packet is sent! */
#ifndef INSTALL_L2CAP_CRC
    if(flow->config.fcs)
    {
        CRC_FLOW_AddFcs(txqe);
    }
#endif
}

/*! \brief Presend: The transmit loop callback function

    Whenever the con_handle transmit loop encounters a F&EC packet it
    will issue a callback to this function. Here we figure out what we
    should send, based on the "state", data-queues, "remote_poll" and
    other state variables.

    Basically, this function consists of the following steps:
    1. Handle special exception states, ie. prepare a special
       S-frame for transmission
    2. If possible, prepare an I-frame for transmission
       This includes the rt/re queue handling
    3. If no I-frame available and no exception level, prepare
       an I-frame ack for transmission
    4. Perform the common frame transmission stuff

*/
void FLOW_DataPresend(TXQE_T *txqe)
{
    CIDCB_T *cidcb;
    FLOWINFO_T *flow;
    FLOWCTRL_T control;   /* native control header integer */
    CsrBool found;         /* target found */
    CsrBool i_frame;       /* is target an I-frame */
    FLOWPF_T pf;          /* default poll/final bit to use */
    CsrUint16 *sdu_length; /* ptr to pkt->sdu_length or null */
    CsrUint8 *retrans;     /* ptr to pkt->retrans or s-frame counter */

    cidcb      = CIDME_GetCidcb(txqe->cid);

    /* If we have disconnected the CIDCB or are in the process of
     * killing it due to internal disconnect (e.g. max transmit
     * exceeded), bail out */
    if((cidcb == NULL) || (cidcb->flow == NULL))
    {
        /* txqe already clean */
        return;
    }

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow       = cidcb->flow;
    control    = 0;
    found      = FALSE;
    i_frame    = FALSE;
    sdu_length = NULL;
    retrans    = NULL;
    pf         = (flow->remote_poll ? PF_FINAL : PF_NONE);

    /* Allow reschedulation. Break out if channel isn't ready any
     * more -- or choke is enabled */
    flow->scheduled = FALSE;
    if((!CID_IsOpen(cidcb) && !CID_IsFixed(cidcb) && !CID_IsMoving(cidcb)))
    {
        /* txqe already clean */
        return;
    }

    /* Check choke mode. Allow polls to go through. */
    if((flow->choke == FCOK_ON) && (flow->state != FST_FORCE_ACK))
    {
        /* txqe already clean */
        return;
    }

    /* What to send depends first of all on the exception level. These
     * are the non-overridable exceptions (ACK and SREJ).  Note that
     * we can't send out REJ/SREJ/RR if the local-busy mode is set
     * while in ERTM as that would flag us as RR */
    switch(flow->state)
    {
        case FST_SREJ_LIST:
            {
                /* Send SREJ, effectively popping of the first missing
                 * frame in the OoS buffer and marking it as
                 * requested. We don't care about the monitor timer
                 * for SREJs */
                if(FLOW_IsEnhancedRet(flow) && !flow->local_busy)
                {
                    FLOWSEQ_T miss_seq = 0;
                    if(FLOW_OosFirstMissing(flow, &miss_seq, &pf))
                    {
                        found = TRUE;
                        control = FLOW_FrameGetS(flow, pf,
                                                 L2CAP_SBIT_SREJ, miss_seq);
                    }
                    else
                    {
                        /* No more SREJs to be sent, so allow normal data
                         * to go through */
                        flow->state = FST_SREJ_DATA;
                    }
                }
            }
            break;

        case FST_WAIT_ACK:
            {
                /* Send R/RNR with poll to query what the last good
                 * packet was that the remote side received */
                if(!FLOW_MonitorRunning(flow) &&
                   !flow->remote_poll)
                {
                    found = TRUE;
                    pf = PF_POLL;
                    retrans = &(flow->local_poll_retrans);
                    control = FLOW_FrameGetS(flow, pf,
                                             FLOW_FrameGetAck(flow),
                                             flow->req_seq_next);
                    flow->req_seq_last = flow->req_seq_next;
                }
            }
            break;

        case FST_BUSY_CLEARED:
            {
                /* Send R/RNR with poll as local busy condition has
                 * cleared. We lower exception level to FST_WAIT_FINAL
                 * as it's OK to send data in this state */
                if(!FLOW_MonitorRunning(flow) &&
                   !flow->remote_poll)
                {
                    found = TRUE;
                    pf = PF_POLL;
                    retrans = &(flow->local_poll_retrans);
                    flow->state = FST_WAIT_FINAL;
                    control = FLOW_FrameGetS(flow, pf,
                                             FLOW_FrameGetAck(flow),
                                             flow->req_seq_next);
                    flow->req_seq_last = flow->req_seq_next;
                }
            }
            break;

        case FST_REJ:
            {
                /* Send REJ (without poll) */
                if(!FLOW_MonitorRunning(flow) &&
                   !(FLOW_IsEnhancedRet(flow) && flow->local_busy))
                {
                    found = TRUE;
                    pf = PF_NONE;
                    flow->state = FST_REJ_DATA;

                    control = FLOW_FrameGetS(flow, pf,
                                             L2CAP_SBIT_REJ,
                                             flow->req_seq_next);
                    flow->req_seq_last = flow->req_seq_next;
                }
            }
            break;

        default:
            /* FST_CLEAR, FST_SREJ_DATA, FST_REJ_DATA, FST_WAIT_FINAL,
             * and FST_ACK are handled outside this switch */
            break;
    }

    /* If state is either CLEAR, REJ_DATA, SREJ_DATA, WAIT_FINAL or
     * ACK, try to send data */
    if(!found && (flow->state <= FST_ACK))
    {
        if(FLOW_IsStreaming(flow) ||
           (FLOW_IsEnhancedRet(flow) && !flow->remote_busy))
        {
            found = FLOW_FrameGetData(cidcb, flow,
                                      txqe, pf,
                                      &control,
                                      &retrans,
                                      &sdu_length);
            /* If found, this is certainly an I-frame */
            i_frame = found;
            if(found)
            {
                flow->req_seq_last = flow->req_seq_next;
            }
        }
    }

    /* See if can can/shall send an RR/RNR ack */
    if(!found)
    {
        if((pf == PF_FINAL) ||
           (!FLOW_MonitorRunning(flow) &&
            ((flow->state == FST_ACK) || (flow->state == FST_FORCE_ACK))))
        {
            /* Don't send redundant acks in ERTM */
            if(!(FLOW_IsEnhancedRet(flow) &&
                 (flow->req_seq_next == flow->req_seq_last) &&
                 (flow->state == FST_ACK) &&
                 (pf == PF_NONE)))
            {
                found = TRUE;
                control = FLOW_FrameGetS(flow, pf,
                                         FLOW_FrameGetAck(flow),
                                         flow->req_seq_next);
                flow->req_seq_last = flow->req_seq_next;
            }
        }
    }

    /* If a PDU is to be sent we apply the common I and S-frame header
     * and tail bits and bobs here */
    if(found)
    {
        FLOW_FramePrep(cidcb, flow, pf, i_frame);
        FLOW_FrameSend(cidcb, flow, txqe, control, sdu_length, retrans);

        /* Debug mode for dropping packets - must be the last thing we
         * do to emulate real over-the-air frame loss. Beware that
         * FrameSend may disconnect the link because of tx overflows,
         * which means that the flow and CIDCB structures are not
         * guaranteed to be valid at this point. We know if we're dead
         * txqe->headersize==0, so only drop stuff if it's still
         * around */
#ifdef INSTALL_L2CAP_DEBUG
        if((txqe->headersize > 0) &&
           FLOW_DebugDropTx(flow))
        {
            CsrMblkDestroy(txqe->mblk);
            txqe->mblk = NULL;
            txqe->headersize = 0;
            return;
        }
#endif
    }
}

/*! \brief Connection handle Tx complete

    This function is a callback from the connection handle transmit
    loop.  It's invoked whenever HCI has returned the NCPs to tell us
    that an element has been transmitted (or flushed). We use this
    event to start any timers and/or pass up datawrite confirms.
*/
void FLOW_DataWriteComplete(CIDCB_T *cidcb, TXQE_T *txqe, l2ca_data_result_t result)
{
    FLOWINFO_T *flow;
    PARAM_UNUSED(txqe);

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow = cidcb->flow;
    if(FLOW_IsStreaming(flow))
    {
        /* Streaming mode uses NCPs to ack data writes */
        FLOWSEQ_T req;
        req = (flow->expected_ack_seq + 1) % FLOW_MaxSequence(flow);
        FLOW_RemoteAckFlush(cidcb, flow, req, result);
    }
    else
    {
        /* Other modes use deferred timers, so figure out which
         * one to start */
        if((flow->defer_monitor) &&
           !FLOW_RetransRunning(flow))
        {
            FLOW_MonitorTimerStart(cidcb, flow, DEFER_NOW);
        }
        else if(flow->defer_retrans > 0)
        {
            /* For data frames we keep track of the number of
             * pending elements such that we only start the timer
             * encase there's actually elements "in the air" */

            if((flow->defer_retrans > 0) &&
               !FLOW_MonitorRunning(flow))
            {
                FLOW_RetransTimerStart(cidcb, flow, DEFER_NOW);
            }
            flow->defer_retrans--;
        }
    }
}

/*! \brief Free the flow control structure for a CIDCB

    Function must be called to free the F&EC code and to flush the transmit,
    acknowledge and re-transmit buffer
*/
void FLOW_Free(CIDCB_T *cidcb, l2ca_data_result_t result)
{
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow = cidcb->flow;

    /* Clear queues (also stops timers) */
    FLOW_FlushAllQueues(cidcb, result);

    /* Remove CIDs from L2CAP CRC checking list. Can always be
     * called */
    (void)FLOW_SetLcFcs(cidcb, FALSE);

    /* Free instance */
    L2CA_FreePrimitive((L2CA_UPRIM_T*)flow->l2ca_dataread_ind);
#ifdef INSTALL_L2CAP_DEBUG
    flow->guard = 0xDEAD;
#endif
    CsrPmemFree(flow);
    cidcb->flow = NULL;
}

/*! \brief Clip effective remote MPS to baseband

    The MPS is set to the highest possible value, so to optimise
    throughput for the specific baseband, we clip the effective value
    to the maximum packet size - AMP or BR/EDR.

    The actual detection and re-numbering/re-segmentation is done at
    transmit time (because we use just-in-time segmentation).
*/
#ifndef INSTALL_AMP_SUPPORT
static
#endif
void FLOW_ClipMPS(CIDCB_T *cidcb)
{
    l2ca_mtu_t clip;
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);
    flow = cidcb->flow;

    /* Default is not to clip. */
    clip = (CsrUint16)~0;

#ifdef INSTALL_AMP_SUPPORT
    if(CID_IsAmp(cidcb))
    {
        /* Using AMP - look up maximum packet size from AMP flow
         * control structure */
        DM_AMP_CHANNEL_T *ampch;
        ampch = dm_amp_find(cidcb->amp_info->local_amp);
        if(ampch)
        {
            CsrUint16 acl_data_packet_length_clip =
                ampch->fc.max_acl_data_packet_length;
            CsrUint16 overhead = FLOW_HeaderSize(flow) +
                                FLOW_FooterSize(flow) +
                                L2CAP_SIZEOF_SDU_LEN_FIELD;

            if (overhead > acl_data_packet_length_clip)
                acl_data_packet_length_clip = 0;
            else
                acl_data_packet_length_clip -= overhead;

            clip = CSRMIN(acl_data_packet_length_clip,
                        ampch->l2cap_max_pdu_length);
        }
    }
#endif

    /* Clip value to either configured OR packet */
    flow->config.effective_mps = CSRMIN(flow->config.remote_mps, clip);
}

/*! \brief Setup for standard retransmission mode

    Perform sanity checks and then setup for RTM
*/
static l2ca_disc_result_t FLOW_SetupEnhancedRet(FLOWINFO_T *flow,
                                                L2CA_CONFIG_T *local,
                                                L2CA_CONFIG_T *remote)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    flow->config.mode = local->flow->mode;
    flow->config.remote_mps = remote->flow->maximum_pdu;
    flow->config.local_mps = local->flow->maximum_pdu;

    if(remote->flow->monitor_timeout != 0)
    {
        flow->config.monitor_timeout = remote->flow->monitor_timeout;
    }
    if(remote->flow->retrans_timeout != 0)
    {
        flow->config.retrans_timeout = remote->flow->retrans_timeout;
        /* The >> operator cannot possibly lead to wrapping. */
        /* xapncc ignore lower precision in wider context: '>>' */
        flow->config.ack_delay = (local->flow->retrans_timeout >> 2); /* div 4 */
    }

    flow->config.max_retransmit = remote->flow->max_retransmit;
    flow->config.local_window = (FLOWSEQ_T)local->ext_window;   /* rx */
    flow->config.remote_window = (FLOWSEQ_T)remote->ext_window; /* tx */

    /* Sanity checks */
    if((flow->config.monitor_timeout == 0) ||
       (flow->config.retrans_timeout == 0) ||
       /* max retransmit can be zero (infinite) */
#ifndef INSTALL_AMP_SUPPORT
       (flow->config.local_window > 63) ||
       (flow->config.remote_window > 63) ||
#endif
       (flow->config.local_window == 0) ||
       (flow->config.remote_window == 0))
    {
        return L2CA_DISCONNECT_CONFIG_ILLEGAL;
    }
    else
    {
        return L2CA_STAY_CONNECTED;
    }

}

/*! \brief Setup for standard retransmission mode

    Perform sanity checks and then setup for RTM
*/
static l2ca_disc_result_t FLOW_SetupStreaming(FLOWINFO_T *flow,
                                              L2CA_CONFIG_T *local,
                                              L2CA_CONFIG_T *remote)
{
    DEBUG_CHECK_GUARD_FLOW(flow);

    flow->config.mode = local->flow->mode;
    flow->config.remote_mps = remote->flow->maximum_pdu;
    flow->config.local_mps = local->flow->maximum_pdu;
    flow->config.monitor_timeout = 0;
    flow->config.retrans_timeout = 0;
    flow->config.ack_delay = 0;
    flow->config.max_retransmit = 0;
    flow->config.local_window = 0;   /* rx */
    flow->config.remote_window = 0;  /* tx */

    /* We're never slightly curved in this case */
    return L2CA_STAY_CONNECTED;
}

/*! \brief Configuration done

    After configuration is done we need to perform a rather large set
    of sanity checks.
*/
static l2ca_disc_result_t FLOW_SetupApply(CIDCB_T *cidcb,
                                          FLOWINFO_T *flow,
                                          L2CA_CONFIG_T *local,
                                          L2CA_CONFIG_T *remote)
{
    l2ca_disc_result_t result = L2CA_STAY_CONNECTED;
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* Optional FCS shall only be disabled if done by both
     * parties */
    flow->config.fcs = (local->options & L2CA_SPECIFY_FCS) == 0
            || (remote->options & L2CA_SPECIFY_FCS) == 0
            || local->fcs
            || remote->fcs;

    /* Extended windows are used if just one of the sides wants it */
#ifdef INSTALL_AMP_SUPPORT
    flow->config.ext_window = (local->options & L2CA_SPECIFY_EXT_WINDOW) != 0
            || (remote->options & L2CA_SPECIFY_EXT_WINDOW) != 0;
#endif

    /* Setup the mode-specific configurations */
    switch(local->flow->mode)
    {
        case L2CA_FLOW_MODE_ENHANCED_RETRANS:
            result = FLOW_SetupEnhancedRet(flow, local, remote);
            break;

        case L2CA_FLOW_MODE_STREAMING:
            result = FLOW_SetupStreaming(flow, local, remote);
            break;

        default:
            /* Mode not supported */
            result = L2CA_DISCONNECT_UNSUPPORTED_MODE;
            break;
    }

    /* Add/remove CIDs from LC's checking list. Note that this is
     * only possible when running on-chip. */
    if (!FLOW_SetLcFcs(cidcb,
                flow->config.fcs && result == L2CA_STAY_CONNECTED))
        result = L2CA_DISCONNECT_HARDWARE;

    /* Setup the common stuff for F&EC */
    if(result == L2CA_STAY_CONNECTED)
    {
        /* Convert timeouts from ms to us once and for all*/
        flow->config.monitor_timeout = flow->config.monitor_timeout * CSR_SCHED_MILLISECOND;
        flow->config.retrans_timeout = flow->config.retrans_timeout * CSR_SCHED_MILLISECOND;
        flow->config.ack_delay = CSRMIN(CSRMAX(flow->config.ack_delay * CSR_SCHED_MILLISECOND,
                                         L2CAP_FLOW_MIN_ACK_DELAY),
                                     L2CAP_FLOW_MAX_ACK_DELAY);

        /* For new channels, we need to set the effective MPS now */
        if(!cidcb->opened)
        {
            FLOW_ClipMPS(cidcb);
        }

        /* Restart retransmission timer if we have pending data */
        if(flow->re_head != NULL)
        {
            FLOW_RetransTimerStart(cidcb, flow, DEFER_PENDING);
        }

        /* Kickstart the F&EC by sending out an ack */
        FLOW_RaiseException(flow, FST_ACK);
        FLOW_Reschedule(cidcb, flow);
    }
    else
    {
        /* Free instance if not used or an error occurred */
        FLOW_Free(cidcb, L2CA_DATA_LINK_TERMINATED);
    }

    return result;
}

/*! \brief Initialize/setup flow control information for CID

    This function is called whenever a lockstep state has been entered
    either because local/remote "config" is OK, or because
    local/remote "pending" is OK. This allows us to store the
    configuration agreed upon so we're ready to begin data processing.

    Note that this function will be called when storing the
    configuration and at least one of the entities have the flow
    control options specified - or the channel has previously been
    configured for flow control.
*/
l2ca_disc_result_t FLOW_Setup(CIDCB_T *cidcb,
                              L2CA_CONFIG_T *local,
                              L2CA_CONFIG_T *remote)
{
    FLOWINFO_T *flow;

    /* Member allocation and initialisation happens only if needed */
    if(cidcb->flow == NULL)
    {
        cidcb->flow = xzpnew(FLOWINFO_T);
        if(cidcb->flow == NULL)
        {
            return L2CA_DISCONNECT_OUT_OF_MEM;
        }
#ifdef INSTALL_L2CAP_DEBUG
        cidcb->flow->guard = 0xBEEF;
#endif
    }
    flow = cidcb->flow;
    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    /* This may be a reconfig, so stop timers */
    FLOW_RetransTimerStop(flow);
    FLOW_MonitorTimerStop(flow);
    FLOW_DelayedAckStop(flow);

    /* Detect errors in flow availablity or mode */
    {
        l2ca_flow_mode_t loc;
        l2ca_flow_mode_t rem;

        loc = (((local->options & L2CA_SPECIFY_FLOW) &&
                (local->flow != NULL))
               ? local->flow->mode : L2CA_FLOW_MODE_BASIC);
        rem = (((remote->options & L2CA_SPECIFY_FLOW) &&
                (remote->flow != NULL))
               ? remote->flow->mode : L2CA_FLOW_MODE_BASIC);

        /* Mode mismatch */
        if(loc != rem)
        {
            FLOW_Free(cidcb, L2CA_DATA_LINK_TERMINATED);
            return L2CA_DISCONNECT_CONFIG_MISMATCH;
        }

        /* If neither side specified the flow options but we've previously
         * configured for a F&EC mode, this changes nothing and we succeed */
        if((local->flow == NULL) && (remote->flow == NULL) &&
           (flow->config.mode != L2CA_FLOW_MODE_BASIC))
        {
            return L2CA_STAY_CONNECTED;
        }

        /* Mode reconfiguration */
        if(cidcb->opened && (flow->config.mode != loc))
        {
            FLOW_Free(cidcb, L2CA_DATA_LINK_TERMINATED);
            return L2CA_DISCONNECT_CONFIG_MISMATCH;
        }

        /* If we still want basic mode at this point it's because we
         * explicitly configured it. Kill the instance and succeed */
        if(loc == L2CA_FLOW_MODE_BASIC)
        {
            FLOW_Free(cidcb, L2CA_DATA_LINK_TERMINATED);
            return L2CA_STAY_CONNECTED;
        }
    }

    /* Transfer window size to extended variable so it's easier to
     * compare stuff. Make sure not to override actual extended value
     * if it's set */
    if(!(local->options & L2CA_SPECIFY_EXT_WINDOW))
    {
        local->ext_window = local->flow->tx_window;
    }
    if(!(remote->options & L2CA_SPECIFY_EXT_WINDOW))
    {
        remote->ext_window = remote->flow->tx_window;
    }

    /* Reconfigurations are only allowed to change certain
     * parameters */
    if(cidcb->opened)
    {
        /* These parameters can never change */
        if((local->flow->maximum_pdu != flow->config.local_mps) ||
           (remote->flow->maximum_pdu != flow->config.remote_mps))
        {
            FLOW_Free(cidcb, L2CA_DATA_LINK_TERMINATED);
            return L2CA_DISCONNECT_CONFIG_MISMATCH;
        }

        /* Local window is rx, remote is tx */
        if((remote->flow->max_retransmit != flow->config.max_retransmit) ||
           (local->ext_window != flow->config.local_window) ||
           (remote->ext_window != flow->config.remote_window))
        {
            FLOW_Free(cidcb, L2CA_DATA_LINK_TERMINATED);
            return L2CA_DISCONNECT_CONFIG_MISMATCH;
        }
    }

    /* Apply */
    return FLOW_SetupApply(cidcb, flow, local, remote);
}

/*! \brief Calculate monitor/retransmission timer for ERTM

    For enhanced retransmission mode, timer values are now autocalculated
    based on the link's use of flush-timeout. In order not to confuse the
    upper layer we intercept and fix these timer values before anything
    is sent either over the air or up to the application.

    All timer values shall be in ms.
*/
static void FLOW_FixTimers(CsrBool from_air,
                           CsrBool amp,
                           l2ca_flow_mode_t mode,
                           l2ca_fs_flush_t flush,
                           l2ca_fs_flush_t lsto,
                           l2ca_timeout_t *monitor,
                           l2ca_timeout_t *retrans)
{
    if(mode == L2CA_FLOW_MODE_ENHANCED_RETRANS)
    {
        /* Check for AMP classification */
#ifdef INSTALL_AMP_SUPPORT
        if(amp)
        {
            /* Check for AMP class 1: Infinite flush timeout */
            if(flush == L2CA_FLOWSPEC_FLUSH_TO_INFINITE)
            {
                if(from_air)
                {
                    /* Both timers must be at least LSTO */
                    *monitor = (l2ca_timeout_t)(lsto & 0xFFFF);
                    *retrans = (l2ca_timeout_t)(lsto & 0xFFFF);
                }
                else
                {
                    /* To-air. Timers are always zero in the config request */
                    *monitor = 0;
                    *retrans = 0;
                }
            }
            /* AMP class 2: Non-infinite flush timeout */
            else
            {
                /* Always set outgoing timers, but only override
                 * incoming ones if they're not set already */
                if(!from_air || (*monitor == 0))
                {
                    *monitor = L2CAP_AC2F_TIMEOUT_MONITOR;
                }
                if(!from_air || (*retrans == 0))
                {
                    *retrans = L2CAP_AC2F_TIMEOUT_RETRANS;
                }
            }
        }
        else
#endif
        if(from_air)
        {
            /* From-air. Values depend on whether we're using
             * automatic flush timeout */
            if(flush == L2CA_FLOWSPEC_FLUSH_TO_INFINITE)
            {
                /* Infinite flush timeout - use hard coded values */
                *retrans = L2CAP_NFTO_TIMEOUT_RETRANS;
                *monitor = L2CAP_NFTO_TIMEOUT_MONITOR;
            }
            else
            {
                /* Finite flush - do calculation in ms */
                CsrUint16 fto = (CsrUint16)(flush / 1000);
                *retrans = CSRMAX((3 * fto), L2CAP_FFTO_TIMEOUT_RETRANS);
                *monitor = CSRMAX((6 * fto), L2CAP_FFTO_TIMEOUT_MONITOR);
            }
        }
        else
        {
            /* To-air. Timers are always zero in the config request */
            *monitor = 0;
            *retrans = 0;
        }
    }
    /* Always zero timers for streaming. And set some extra
     * zeroes. */
    else if(mode == L2CA_FLOW_MODE_STREAMING)
    {
        *retrans = 0;
        *monitor = 0;
    }
}

/*! \brief Fixup configuration values for ERTM/SM

    Fix the configuration paramters with the auto-detected ERTM/SM
    values -- this is mostly the timer values.

    Note: The config->flush_to has already been converted from ms to
    slots while the config->flow->foo_timeout's are still is ms!
*/
void FLOW_FixConfig(CIDCB_T *cidcb,
                    L2CA_CONFIG_T *config,
                    CsrBool from_air)
{
    PARAM_UNUSED(cidcb);

    /* Fixup stuff only for ERTM */
    if((config->options & L2CA_SPECIFY_FLOW) &&
       (config->flow != NULL))
    {
        l2ca_fs_flush_t flush;
        l2ca_fs_flush_t lsto;
        CsrBool amp;

        /* Fetch LSTO/flush timeout and AMP status */
#ifdef INSTALL_AMP_SUPPORT
        if(CID_IsAmp(cidcb) &&
           (cidcb->amp_info->local_amp != L2CA_AMP_CONTROLLER_BT))
        {
            amp   = TRUE;
            flush = cidcb->amp_info->be_flush_to;
            lsto  = cidcb->amp_info->lsto;
        }
        else
#endif
        {
            amp   = FALSE;
            lsto  = 0;
            flush = L2CA_FLOWSPEC_FLUSH_TO_INFINITE;
            if ((config->options & L2CA_SPECIFY_FLUSH) != 0
                    && config->flush_to != FLUSH_INFINITE_TIMEOUT)
            {
                flush = (l2ca_fs_flush_t)SIG_FlushToH2L(config->flush_to)
                    * 1000; /* ms->us */
            }
        }

        /* Call generic timer fixup function */
        FLOW_FixTimers(from_air, amp, config->flow->mode,
                       flush, lsto,
                       &(config->flow->monitor_timeout),
                       &(config->flow->retrans_timeout));

        /* Set a few other bits for streaming mode */
        if(config->flow->mode == L2CA_FLOW_MODE_STREAMING)
        {
            config->flow->max_retransmit = 0;
            config->flow->tx_window = 0;
        }

        /* Clip MPS when on-chip like we clip the MTU */
        CID_ClipMTU(cidcb, &config->flow->maximum_pdu, from_air);
    }
}

/*! \brief Memory resources are tight

    This function may be called if memory resources are running
    low. In this case we will try to free the receive buffers and pass
    up unassembled data. We'll do anything to avoid a deadlock!
*/
#ifndef BUILD_FOR_HOST
void FLOW_LowMemory(CIDCB_T *cidcb, FLOWINFO_T *flow)
{
    /* We're about to drop the out-of-sequence buffer, so check if we
     * need to ask for a complete retransmission */
    L2CA_DATAREAD_IND_T *new_ind, *old_ind;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    if(FLOW_OosCountMissing(flow, CONTEXT_MISSING | CONTEXT_SENT) > 0)
    {
        flow->state = FST_REJ;
        FLOW_ResetPollRetrans(flow);
    }
    FLOW_OosFlush(flow);

    /* Empty the reassembly buffer */
    if((old_ind = flow->l2ca_dataread_ind) != NULL && old_ind->data != NULL)
    {
        /* Create a new L2CA_DATAREAD_IND to handle data still to come. */
        L2CA_DataReadInd(cidcb, NULL, L2CA_DATA_PARTIAL, 1, &new_ind);
        flow->l2ca_dataread_ind = new_ind;

        /* Pass on length of data still to come. */
        new_ind->length = old_ind->length;

        /* Because this is a partial SDU and currently needs no ack. */
        old_ind->packets = 0;

        /* Pass up data */
        L2CA_PrimSend(old_ind);
    }

    /* Reschedule to keep everything going */
    FLOW_Reschedule(cidcb, flow);
}
#endif

/*! \brief Choke transmission

    In order to support the AMP MoveChannel business we must have
    choke support so we can halt and resume transmissions on a channel
    completely. Note that this is very different from the "busy"
    indications in the sense that the choking facility actually works.

    Without AMP support, we still support unchoking as this is needed
    after reconfigurations have completed.
*/
#ifdef INSTALL_AMP_SUPPORT
void FLOW_Choke(CIDCB_T *cidcb, FLOWCHOKE_T choke)
{
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);
    flow = cidcb->flow;

    /* AUTO_RR won't work with streaming mode, so override it and jump
     * directly to OFF */
    if(FLOW_IsStreaming(flow) && (choke == FCOK_AUTO_RR))
    {
        choke = FCOK_OFF;
    }

    /* Update state now as we'll need to call functions that depends
     * on current state */
    flow->choke = choke;

    if(choke == FCOK_CONF_START)
    {
        /* Reconfiguration started - full choke */
        flow->choke = FCOK_ON;
        FLOW_MonitorTimerStop(flow);
        FLOW_RetransTimerStop(flow);
        FLOW_DelayedAckStop(flow);
    }
    else if(choke == FCOK_OFF)
    {
        /* Config done - release */
        flow->choke = FCOK_OFF;
        if((flow->re_head != NULL))
        {
            FLOW_RetransTimerStart(cidcb, flow, DEFER_NOW);
        }
        flow->scheduled = FALSE;
        FLOW_RaiseException(flow, FST_WAIT_ACK);
        FLOW_DelayedAckStart(flow, cidcb);
    }
}
#else
void FLOW_Choke(CIDCB_T *cidcb, FLOWCHOKE_T choke)
{
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);

    flow = cidcb->flow;

    if(choke == FCOK_CONF_START)
    {
        /* Reconfiguration started, so stop timers */
        FLOW_MonitorTimerStop(flow);
        FLOW_RetransTimerStop(flow);
        FLOW_DelayedAckStop(flow);
    }
    else if(choke == FCOK_OFF)
    {
        /* Reconfiguration done, reschedule */
        FLOW_Reschedule(cidcb, flow);
    }
}
#endif

/*! \brief Prepare for AMP move procedure

    The AMP channel move procedure is tightly coupled with ERTM. This
    function prepares an ERTM channel for the move
*/
#ifdef INSTALL_AMP_SUPPORT
void FLOW_AmpMovePrepare(CIDCB_T *cidcb)
{
    FLOWINFO_T *flow;
    FLOWPKT_T *pkt;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);
    flow = cidcb->flow;

    /* Choke data */
    flow->move_channel = TRUE;
    FLOW_Choke(cidcb, FCOK_ON);

    if(cidcb->amp_info &&
       cidcb->amp_info->queue)
    {
        CH_FlushPendingQueueWithCid(cidcb->amp_info->queue,
                                    cidcb->local_cid,
                                    L2CA_DATA_SILENT); /* always silent kill */
    }

    /* This isn't needed in streaming mode */
    if(flow->config.mode == L2CA_FLOW_MODE_STREAMING)
    {
        return;
    }

    /* Stop timers and defer-counters  */
    FLOW_MonitorTimerStop(flow);
    FLOW_RetransTimerStop(flow);
    FLOW_DelayedAckStop(flow);
    flow->defer_monitor = FALSE;
    flow->defer_retrans = 0;

    /* Clear exception and REJ/SREJ states */
    flow->state = FST_CLEAR;
    flow->rej_actioned = FALSE;
    flow->srej_actioned = FALSE;
    flow->remote_poll = FALSE;
    flow->rnr_sent = FALSE;
    flow->highest_tx_seq = flow->expected_tx_seq;

    /* Stuff all retransmit-queue elements back on the backup queue */
    while(flow->rt_head)
    {
        /* Remove head rt-queue, progress to next and stuff pkt onto
         * re-queue */
        pkt = flow->rt_head;
        (void)CsrMblkCoalesceToPmalloc(&(pkt->mblk));
        flow->rt_head = pkt->next;
        FLOW_FrameBackup(flow, pkt);
    }

    /* Zero retry counters */
    flow->local_poll_retrans = 0;
    for(pkt = flow->re_head; pkt != NULL; pkt = pkt->next)
    {
        pkt->retrans = 0;
    }

    /* Flush the receiver OoS buffer */
    FLOW_FlushQueue(cidcb,
                    flow,
                    &(flow->oos_head),
                    0,
                    L2CA_DATA_SILENT);
    flow->oos_head = NULL;
}
#endif

/*! \brief Resume after AMP move procedure

    The AMP channel move procedure is tightly coupled with ERTM. This
    function re-initialises an ERTM channel after a move procedure is
    complete. The primary bit here is to re-clip the effective MPS
    size. We can then detect whether any new retransmission shall be
    re-segmented and renumbered.
*/
#ifdef INSTALL_AMP_SUPPORT
void FLOW_AmpMoveResume(CIDCB_T *cidcb, FLOWCHOKE_T choke)
{
    FLOWINFO_T *flow;

    DEBUG_CHECK_GUARD_ALL(cidcb, cidcb->flow);
    flow = cidcb->flow;

    /* Re-calculate MPS and open the data pipe */
    FLOW_ClipMPS(cidcb);
    flow->move_channel = FALSE;
    FLOW_Choke(cidcb, choke);
}
#endif

#if defined(INSTALL_L2CAP_CRC) && !defined(BUILD_FOR_HOST)
/*! \brief Toggle FCS in the LC */
CsrBool FLOW_LcSetL2capCrc(CIDCB_T *cidcb, CsrBool use_fcs)
{
    hci_connection_handle_t handle = CH_GET_HANDLE(cidcb->chcb);
    CsrBool rx, tx;

    rx = lc_set_l2cap_crc(handle,
                          FALSE, /* from-air */
                          (CID_IsFixed(cidcb) ? cidcb->remote_cid : cidcb->local_cid),
                          use_fcs);
    tx = lc_set_l2cap_crc(handle,
                          TRUE, /* to-air */
                          cidcb->remote_cid,
                          use_fcs);
    return (rx && tx);
}
#endif

/*! \brief Abort all unsent data on flow->tx_head.

    If there is data to be aborted then we will try to send confirmations
    to the application immediately. If this is not possible then we queue
    the confirmations until they can be sent.

    \param cidcb Pointer to channel control structure.
    \param flow Pointer to flow structure.
*/
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
void FLOW_AbortTX(CIDCB_T *cidcb, FLOWINFO_T *flow)
{
    L2CAP_ABORTED_PACKET_T **abort_queue = NULL;
    FLOWPKT_T *elp, **elpp = &flow->tx_head;
    FLOWPKT_T *last_transmitted;

    DEBUG_CHECK_GUARD_ALL(cidcb, flow);

    if ((last_transmitted = FLOW_FindLastTransmitted(flow)) != NULL)
    {
        abort_queue = &last_transmitted->aborted;

        /* Skip head of queue if it is partially transmitted. */
        if (*elpp == last_transmitted)
            elpp = &last_transmitted->next;
    }

    /* Flush queue */
    while ((elp = *elpp) != NULL)
    {
        --flow->credits_seq;
        CH_AbortElement(cidcb, elp->context, &abort_queue);

        /* Remove and destroy element */
        *elpp = elp->next;
        CsrMblkDestroy(elp->mblk);
        CsrPmemFree(elp);
    }

    /* Maximum of one packet left on queue, so update tail. */
    flow->tx_tail = flow->tx_head;

    CH_AbortCfm(cidcb, abort_queue);
}
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

#endif /* INSTALL_L2CAP_ENHANCED_SUPPORT */
