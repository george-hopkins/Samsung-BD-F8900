/*!
   Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
     All rights reserved.

\file   l2cap_con_handle.c

\brief  This file handles L2CAP connections, there is a one-to-one
        mapping between a L2CAP connection and an ACL.
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
#include "csr_sched.h"

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_interface.h"
#include "l2cap_control.h"
#include "l2cap_flow.h"

#include "dm_acl_core.h"
#include "dm_acl_manager.h"
#include "dm_acl_policy_manager.h"
#include "dm_layer_manager.h"

#ifdef INSTALL_AMP_SUPPORT
#include "l2cap_amp_utils.h"
#endif

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
#include "l2cap_crc_flow.h"
#endif

#ifndef BUILD_FOR_HOST
#include "radioio.h"
#include "patch.h"
#else
#include "csr_bt_hcishim.h"
#endif

#ifdef INSTALL_AMP_SUPPORT
#define QUEUE_IS_BR_EDR(queue) ((queue)->controller_id == L2CA_AMP_CONTROLLER_BT)
#else
#define QUEUE_IS_BR_EDR(queue) TRUE
#endif

#ifdef BUILD_FOR_HOST
static CsrUint8 CH_SendACLDataPacket(
        TXQUEUE_T *queue,
        CsrUint16 handle_plus_flags,
        L2CAP_CH_DATA_SIZES_T *sizes,
        TXQE_T *txqe)
{
    CsrMblk *head = NULL, *tail = NULL, *mblk = NULL;

    if (sizes->body == 0 ||
            (mblk = CsrMblkDuplicateRegion(txqe->mblk,
                                          txqe->sent,
                                          sizes->body)) != NULL)
    {
        if (sizes->header == 0 ||
                (head = CsrMblkDataCreate(pcopy(txqe->header, sizes->header),
                                         sizes->header,
                                         TRUE)) != NULL)
        {
            mblk = CsrMblkAddHead(head, mblk);

            if (sizes->trailer == 0 ||
                (tail = CsrMblkDataCreate(
                    pcopy(txqe->trailer + txqe->trailersent, sizes->trailer),
                          sizes->trailer,
                          TRUE)) != NULL)
            {
                mblk = CsrMblkAddTail(tail, mblk);

#ifdef INSTALL_AMP_SUPPORT
                if (!QUEUE_IS_BR_EDR(queue))
                {
                    CsrBtHcishimAclDataReq(
                            (CsrUint16)(queue->controller_id & 0xFF),
                            queue->physical_handle,
                            queue->logical_handle,
                            mblk,
                            queue->hci_data_queue);
                }
                else
#endif
                {
                    CsrBtHcishimAclDataReq(L2CA_AMP_CONTROLLER_BT,
                                           0,
                                           handle_plus_flags,
                                           mblk,
                                           CSR_SCHED_QID_INVALID);
                }

                return HCI_SUCCESS;
            }
        }
        CsrMblkDestroy(mblk);
    }

    return HCI_ERROR_MEMORY_FULL;
}
#else
#define CH_SendACLDataPacket(queue, handle, sizes, txqe) \
    (radioio_send_acl_data_packet((handle), (sizes), (txqe)))
#endif

/*! \brief Sends head of transmit queue element over ACL

    This function will transmit the tx_queue head over the ACL. If a
    transmission is already pending (due to HCI fragmentation), we
    continue where we left. It is crucial that this function is as
    fast as possible because we are guaranteed to have the necessary
    credits so link is just waiting to be saturated! :-D

    \param chcb Pointer to CHCB instance.
*/
static CsrBool CH_DataTx(L2CAP_CHCB_T *chcb,
                        TXQUEUE_T *queue)
{
    TXQE_T *txqe;
    CsrMblkSize mblk_size;
    CsrUint16 handle_plus_flags;
    CsrUint16 packet_boundary_flag = HCI_PKT_BOUNDARY_FLAG_CONT;
    L2CAP_CH_DATA_SIZES_T data_sizes;

    /* Sanity */
    if(queue->tx_current == NULL)
    {
        return FALSE;
    }

    /* Prepare various bits */
    txqe = queue->tx_current;
    handle_plus_flags = ((txqe->type == L2CAP_FRAMETYPE_CONNECTIONLESS)
                         ? (L2CAP_BCAST_HCI_HANDLE | HCI_BROADCAST_FLAG_PICONET)
                         : CH_GET_HANDLE(chcb));

    mblk_size = ((txqe->mblk != NULL)
                 ? (CsrMblkSize)CsrMblkGetLength(txqe->mblk)
                 : (CsrMblkSize)0);

    /* First packet to go out. Setup flushable/non-flushable boundary
     * flag on-host: Disable flush for non-2.1 controllers and
     * infinite channels. On-chip, radioio will know what to do */
    data_sizes.header = 0;
    if(txqe->sent == 0)
    {
        data_sizes.header  = txqe->headersize;
#ifdef BUILD_FOR_HOST
        if(LYMCB_TEST(LYM_Packet_Boundary_Flag_Supported) &&
           (txqe->flush == FLUSH_INFINITE_TIMEOUT))
        {
            packet_boundary_flag = HCI_PKT_BOUNDARY_FLAG_START_NONFLUSH;
        }
        else
#endif
        {
            packet_boundary_flag = HCI_PKT_BOUNDARY_FLAG_START_FLUSH;
        }
    }

    /* Send HCI packets */
    do
    {
        CsrUint16 credits;

        data_sizes.body = mblk_size - txqe->sent;
        data_sizes.trailer = txqe->trailersize - txqe->trailersent;

        /* Request credits. Exit if none to be had. */
        if ((credits = dm_amp_getdatacredits(queue, &data_sizes,
                               CH_GET_ACL_TYPE(chcb))) == 0)
        {
            return FALSE;
        }

        txqe->credits += credits;

#ifdef INSTALL_L2CAP_RAW_SUPPORT
        if (chcb->raw_hci_sniff)
        {
            CsrUint8 *data;

            if ((data = xpmalloc(data_sizes.body)) != NULL)
            {
                CsrMblk *duplicate;

                CsrMblkCopyToMemory(txqe->mblk,
                                    txqe->sent,
                                    data_sizes.body,
                                    data);

                duplicate = CsrMblkDataCreate(data, data_sizes.body, TRUE);

                /* Send debug copy to application. */
                if (duplicate != NULL)
                {
                    CIDCB_T *cidcb;
                    l2ca_cid_t cid = txqe->cid;

                    /* For fixed channels use remote CID instead as this
                       will be the fixed channel number and will make more
                       sense. */
                    if ((cidcb = CIDME_GetCidcb(cid)) != NULL
                            && cidcb->remote_cid < L2CA_CID_DYNAMIC_FIRST)
                        cid = cidcb->remote_cid;

                    L2CA_RawDataInd(chcb->raw_handle,
                                    CH_GET_TBDADDR(chcb),
                                    L2CA_DATA_RAW_COPY,
                                    (l2ca_raw_packet_t)(packet_boundary_flag
                                            == HCI_PKT_BOUNDARY_FLAG_CONT ?
                                        L2CA_TX_CONT : L2CA_TX_START),
                                    txqe->cid,
                                    duplicate);
                }
                else
                    CsrPmemFree(data);
            }
        }
#endif

        switch (CH_SendACLDataPacket(
                    queue,
                    (CsrUint16)(handle_plus_flags | packet_boundary_flag),
                    &data_sizes,
                    txqe))
        {
#ifdef BUILD_FOR_HOST
            case HCI_ERROR_MEMORY_FULL:
                return FALSE;
#else
            case HCI_ERROR_HARDWARE_FAIL:
                BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        }

#ifndef DISABLE_DM_BREDR
        /* Notify ACL manager that something has happened (BR/EDR only) */
        if (txqe->type != L2CAP_FRAMETYPE_CONNECTIONLESS
                && QUEUE_IS_BR_EDR(queue))
            acl_activity(CH_GET_ACL(chcb));
#endif

        /* Increment offsets */
        txqe->sent += data_sizes.body;
        txqe->trailersent += data_sizes.trailer;

        /* All subsequent packets will be continuations without any
         * L2CAP headers */
        packet_boundary_flag = HCI_PKT_BOUNDARY_FLAG_CONT;
        data_sizes.header = 0;
    }
    while ((txqe->sent != mblk_size) ||
           (txqe->trailersent != txqe->trailersize));

    /* Free data now. In case of F&EC backup data, the refcount will
     * make sure that the data is still around */
    CsrMblkDestroy(txqe->mblk);
    txqe->mblk = NULL;

    /* Move TXQE to the end of the done queue and clear the tx_current
     * element so we can start next transmission. This MUST be the
     * last thing we do as we reuse the txqe variable! */
    if(queue->tx_done == NULL)
    {
        queue->tx_done = queue->tx_current;
    }
    else
    {
        for(txqe = queue->tx_done; txqe->next != NULL; txqe = txqe->next)
            /* nop */;
        txqe->next = queue->tx_current;
    }
    queue->tx_current = NULL;

    return TRUE;
}

/*! \brief Transmission complete

    When HCI sends us NCPs indications, we scan the list of
    transmitted elements and generate data write or do callbacks so
    users can know that data they've sent has actually been shovelled
    out.

    \param chcb Pointer to CHCB instance.
    \param ncp Number of HCI packets sent
*/
static void CH_DataSendComplete(TXQUEUE_T *queue, CsrUint16 ncp)
{
    TXQE_T *txqe;
    CsrUint16 consume;

    txqe = queue->tx_done;

    if (ncp > queue->credits_taken)
    {
        /* This should never happen. */
#ifdef L2CAP_HCI_DATA_CREDIT_CHECKS_PANIC
        BLUESTACK_PANIC(CSR_BT_PANIC_L2CAP_HCI_DATA_CREDITS_INCONSISTENT);
#else
        BLUESTACK_WARNING(CONTROL_ERR);
#endif

        queue->credits_taken = 0;
    }
    else
        queue->credits_taken -= ncp;

    /* Make sure that there is always one credit reserved just for us. */
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
    if(txqe != NULL)
    {
        CIDCB_T *pcidcb = NULL;
        pcidcb = CIDME_GetCidcb(txqe->cid);

        if(pcidcb)
        {
            dm_amp_reserve_credit(queue, CH_GET_ACL_TYPE(pcidcb->chcb));
        }
    }
#else
    dm_amp_reserve_credit(queue, DM_AMP_ACL_TYPE_BR_EDR);
#endif

    while((ncp > 0) && (queue->tx_done != NULL))
    {
        /* Cache element */
        txqe = queue->tx_done;

        /* Consume credits - make sure we don't underflow */
        consume = CSRMIN(txqe->credits, ncp);
        txqe->credits -= consume;
        ncp -= consume;

        /* Has element been completely sent? */
        if(txqe->credits == 0)
        {
            CIDCB_T *cidcb;
            cidcb = CIDME_GetCidcb(txqe->cid);

            /* Invoke special completion functions or send confirm -
             * only makes sense if the CIDCB is assigned */
            if(cidcb != NULL)
            {
                switch(txqe->type)
                {
                    case L2CAP_FRAMETYPE_SIGNAL:
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
                        /* Crank the state machine */
                        CID_SignalNcpInd(cidcb);
                        break;
#endif
                    case L2CAP_FRAMETYPE_NORMAL:
                        /* -fallthrough */
                    case L2CAP_FRAMETYPE_CONNECTIONLESS:
                        /* Confirm (function is cidcb-NULL safe) */
                        L2CA_DataWriteCfm(cidcb,
                                          txqe->context,
                                          txqe->sent,
                                          L2CA_DATA_SUCCESS);
                        break;

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                    case L2CAP_FRAMETYPE_FLOW_CONTROL:
                        if(CID_IsFlowControl(cidcb))
                        {
                            FLOW_DataWriteComplete(cidcb,
                                                   txqe,
                                                   L2CA_DATA_SUCCESS);
                        }
                        break;
#endif

                    default:
                        /* No completion */
                        break;
                }

                /* Send any delayed confirms for flushed packets. */
                CH_EmptyAbortQueue(cidcb,
                                   &txqe->aborted,
                                   L2CA_DATA_LOCAL_ABORTED);
            }

            /* Remove TXQE from list and free it */
            queue->tx_done = txqe->next;
            CsrPmemFree(txqe);

        }
    }

    /* Check current element too */
    txqe = queue->tx_current;
    if((ncp > 0) && (txqe != NULL))
    {
        consume = CSRMIN(txqe->credits, ncp);
        txqe->credits -= consume;
        ncp -= consume;
    }

#ifdef L2CAP_HCI_DATA_CREDIT_CHECKS
    /* ACL Data credit debugging.
       We should never get back more credits than we have consumed. */
    if (ncp != 0)
    {
#ifdef L2CAP_HCI_DATA_CREDIT_CHECKS_PANIC
        BLUESTACK_PANIC(CSR_BT_PANIC_L2CAP_HCI_DATA_CREDITS_INCONSISTENT);
#else
        BLUESTACK_WARNING(CONTROL_ERR);
#endif
    }
#endif
}

#ifdef L2CAP_HCI_DATA_CREDIT_SLOW_CHECKS
/*! \brief Count number of outstanding credits for connectionless data.

    \returns Number of outstanding credits.
*/
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
CsrUint16 CH_CLUsedDataCredits(void)
{
    return CH_UsedDataCredits(mcb.connectionless);
}
#endif

/*! \brief Count number of outstanding credits for given chcb.

    \param chcb Pointer to Connection Handle Control Block.
    \returns Number of outstanding credits.
*/
CsrUint16 CH_UsedDataCredits(L2CAP_CHCB_T *chcb)
{
    TXQE_T *qe;
    CsrUint16 credits = 0;

    if (chcb != NULL)
    {
        if (chcb->queue.tx_current != NULL)
            credits = chcb->queue.tx_current->credits;

        for (qe = chcb->queue.tx_done; qe != NULL; qe = qe->next)
            credits += qe->credits;

#ifdef L2CAP_HCI_DATA_CREDIT_CHECKS_PANIC
        if (credits != chcb->queue.credits_taken)
            BLUESTACK_PANIC(CSR_BT_PANIC_L2CAP_HCI_DATA_CREDITS_INCONSISTENT);
#endif

        credits += chcb->queue.reserved_credit;
    }

    return credits;
}
#endif

/*! \brief Send queued connectionless data */
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
void CH_CLDataSendQueued(void)
{
    if(mcb.connectionless != NULL)
    {
        CH_DataSendQueued(mcb.connectionless, &(mcb.connectionless->queue), 0, FALSE);
    }
}
#endif

/*! \brief Send queued data/signals

    Send any queued signals or data, signals take preference over
    data.  If there is no data to be sent a check is made to see if
    the connection is idle.

    It's safe for everybody to call this function with a valid CHCB.

    \param chcb Pointer to CHCB instance.
*/
void CH_DataSendQueued(L2CAP_CHCB_T *chcb,
                       TXQUEUE_T *queue,
                       CsrUint8 priority,
                       CsrBool from_nhcp)
{
    TXQE_T *txqe;

    /* Make sure that the channel is in a valid state and that we are not
     * already running. As we are dealing with callbacks on a queue it's
     * necessary to avoid flooding the call stack with re-scheduling.
     * The simplest solution is a lock - queue->tx_active, which will
     * work because we're running in the background. */
    if((!CH_IS_ULP(chcb) && LYMCB.fc.max_acl_data_packet_length == 0) ||
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
        (CH_IS_ULP(chcb) && LYMCB.le_fc.max_acl_data_packet_length == 0) ||
#endif
       !CH_IS_CONNECTED(chcb) ||
       queue->tx_active)
    {
        return;
    }
    queue->tx_active = TRUE;

    /* Loop while we still have credits we can use */
    for(;;)
    {
        /* Check if there is any data waiting for credits */
        if(queue->tx_current != NULL)
        {
            CIDCB_T *cidcb = CIDME_GetCidcb(queue->tx_current->cid);

            /* Resume sending data if it is of sufficient priority */
            if ((!from_nhcp &&
                 cidcb != NULL &&
                 cidcb->priority > priority)
             || (!CH_DataTx(chcb, queue)))
            {
                /* Channel is of insufficient priority, or it is not ready
                 * for more, so break out until we receive more credits */
                break;
            }
            continue;
        }

        /* Assign current TXQE element, but don't transfer it completely */
        txqe = queue->tx_queue[priority];

        if(txqe != NULL)
        {
            /* Move queue forward as we now know that element is valid */
            queue->tx_queue[priority] = txqe->next;
            queue->tx_current = txqe;
            txqe->next = NULL;

            /* Does this transmitter require a pre-send callback? */
            if(txqe->headersize == 0)
            {
                switch(txqe->type)
                {
                    case L2CAP_FRAMETYPE_SIGNAL:
                        /* Pre-send callback for signals */
                        SIGH_SignalPresend(chcb, txqe);
                        break;

#if 0
                    case L2CAP_FRAMETYPE_NORMAL:
                        /* -fallthrough */
                    case L2CAP_FRAMETYPE_CONNECTIONLESS:
                        /* Nop */
                        break;
#endif

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                    case L2CAP_FRAMETYPE_FLOW_CONTROL:
                        /* Pre-send callback for F&EC */
                        FLOW_DataPresend(txqe);
                        break;
#endif

                    default:
                        /* No callbacks */
                        break;
                }
            }

            /* Queue element already setup, so just call
             * transmitter */
            if((txqe->headersize != 0) || (txqe->mblk != NULL))
            {
                if(!CH_DataTx(chcb, queue))
                {
                    /* Channel got busy, so bail out */
                    break;
                }
            }
            else
            {
                /* Callback didn't have any data, so just remove the
                 * current element. Queue has already been moved
                 * forward */
                CH_EmptyAbortQueue(NULL, &txqe->aborted, L2CA_DATA_SILENT);
                CsrPmemFree(txqe);
                queue->tx_current = NULL;
            }
            continue;
        }
        else
        {
            /* Queue is empty */
            queue->tx_queue[priority] = NULL;
            queue->tx_current = NULL;
        }

        /* No signals or data queued, so we have finished. Note that
         * both flow control and the standard data sender above
         * restarts the while() loop. */
        break;
    }

    queue->tx_active = FALSE;
}

/*! \brief Fill out standard header in txqe

    Fill out the header and headersize fields for a txqe. We always
    fill out the CID and length fields according to the spec. Any
    further header will be copied in in case paramters are given.

    \param txqe Tx queue element to modify
    \param cid CID to copy into header
    \param mblksize Size of payload (without headers!)
    \param head Pointer to extra header fields
    \param headsize Length of extra headers in octets
*/
void CH_DataAddHeader(TXQE_T *txqe,
                      CsrUint16 cid,
                      CsrUint16 mblksize,
                      CsrUint8 *head,
                      CsrUint8 headsize)
{
    CsrUint8 *dataptr;
    txqe->headersize = L2CAP_SIZEOF_CID_HEADER + headsize;

    /* Fill common header */
    dataptr = txqe->header;
    write_uints(&dataptr,
                URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
                mblksize + headsize,
                cid);

    /* Copy in optional header fields */
    CsrMemCpy(dataptr, head, headsize);
}

/*! \brief Add tx elememt to tail of tx_queue */
static void CH_DataTxAppend(TXQUEUE_T *queue,
                            TXQE_T *txqe,
                            CsrUint8 priority)
{
    /* Insert into tx_queue tail */
    if(queue->tx_queue[priority] == NULL)
    {
        queue->tx_queue[priority] = txqe;
    }
    else
    {
        TXQE_T *tail;
        for(tail = queue->tx_queue[priority]; tail->next != NULL; tail = tail->next)
        {
            /* nop */;
        }
        tail->next = txqe;
    }
}

/*! \brief Add a CID-style data element to the queue

    Add data to the (basic) transmit queue, possibly transmitting it
    in the same go. This function should only be used by the normal
    (ie. basic) transmit path.

    We need to be able to jump the normal transmit queue for
    normal data elements (the duplicate request signal handling
    comes spring to mind). This function allows both normal
    prepend and append

    \param cidcb Pointer to channel control block.
    \param context Opaque value to be returned in _CFM.
    \param mblk Pointer to data MBLK.
    \returns TRUE if successful, otherwise FALSE.
*/
CsrBool CH_DataTxBasic(CIDCB_T *cidcb,
                    CsrUint16 context,
                    CsrMblk *mblk)
{
    TXQE_T *txqe;
    TXQUEUE_T *queue;
    FRAMETYPE_T frame_type = L2CAP_FRAMETYPE_NORMAL;
    CsrUint8 *psm = NULL;
    CsrUint8 size = 0;
    L2CAP_CHCB_T *chcb = cidcb->chcb;

#if defined(INSTALL_L2CAP_CONNLESS_SUPPORT) \
        || defined(INSTALL_L2CAP_UCD_SUPPORT)
    CsrUint8 buf[L2CAP_SIZEOF_PSM_FIELD];

    if (cidcb->remote_cid == L2CA_CID_CONNECTIONLESS)
    {
        CsrUint8 *psm_copy;  /* We don't want the original to be incremented */

        if (cidcb->remote_psm == L2CA_PSM_INVALID)
            return FALSE;

        if (CH_IS_CONNECTIONLESS(mcb.connectionless))
            frame_type = L2CAP_FRAMETYPE_CONNECTIONLESS;

        psm_copy = psm = buf;
        write_uint16(&psm_copy, cidcb->remote_psm);
        size = L2CAP_SIZEOF_PSM_FIELD;
    }
#endif /* INSTALL_L2CAP_CONNLESS_SUPPORT || INSTALL_L2CAP_UCD_SUPPORT */

    txqe = (TXQE_T*)CsrPmemZalloc(sizeof(TXQE_T) + L2CAP_SIZEOF_CID_HEADER + size);
    txqe->type = frame_type;
    txqe->cid = cidcb->local_cid;
    txqe->context = context;
    txqe->mblk = mblk;
    txqe->flush = cidcb->local_flush_to;
    CH_DataAddHeader(txqe, cidcb->remote_cid, CsrMblkGetLength(mblk), psm, size);

    /* Allow AMP queues to be used (if AMP enabled) so we can test the
     * data path with basic mode */
#ifdef INSTALL_AMP_SUPPORT
    queue = CID_GetTxQueue(cidcb);
#else
    queue = &(chcb->queue);
#endif

    CH_DataTxAppend(queue, txqe, cidcb->priority);
    CH_DataSendQueued(chcb, queue, cidcb->priority, FALSE);

    return TRUE;
}

/*! \brief Add a raw data element to the transmit queue

    This allows the upper interface to send raw L2CAP data.
*/
#ifdef INSTALL_L2CAP_RAW_SUPPORT
void CH_DataTxRaw(L2CA_RAW_DATA_REQ_T *req)
{
    L2CAP_CHCB_T *chcb;
    TXQUEUE_T *queue;
    CIDCB_T *cidcb;
    CsrUint16 remcid;
    CsrUint8 priority;

    /* Try to get the CHCB via the CID. If that doesn't work, simply
     * pick the first one available. This is a 'raw' interface after
     * all... */
    cidcb = CIDME_GetCidcb(req->cid);
    if (cidcb == NULL)
    {
        chcb = CH_GET_CHCB(DM_ACL_FIRST());
        remcid = req->cid;
    }
    else
    {
        chcb = cidcb->chcb;
        remcid = cidcb->remote_cid;
    }

    if (chcb != NULL)
    {
        TXQE_T *txqe;

        txqe = (TXQE_T*)CsrPmemZalloc(sizeof(TXQE_T) + L2CAP_SIZEOF_CID_HEADER);
        txqe->type = L2CAP_FRAMETYPE_RAW;
        txqe->flush = req->flush_to;
        txqe->mblk = req->data;

        /* Shall we auto generate the header? */
        if(req->raw_length == 0)
        {
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
            /* Figure out whether FCS is used, and set the trailer
             * size if that's the case.  This will automatically mark
             * the packet for HW CRC generation */
            if((cidcb != NULL) &&
               CID_IsFlowControl(cidcb) &&
               cidcb->flow->config.fcs)
            {
                txqe->trailersize = 2;
            }
#endif
            /* Generate packet and put on queue */
            CH_DataAddHeader(txqe, remcid,
                             (CsrUint16)(CsrMblkGetLength(txqe->mblk) + txqe->trailersize),
                             NULL, 0);

#if defined(INSTALL_L2CAP_ENHANCED_SUPPORT) && !defined(INSTALL_L2CAP_CRC)
            /* Software CRC may be used at this point */
            if(txqe->trailersize == 2)
            {
                CRC_FLOW_AddFcs(txqe);
            }
#endif
        }
        else
        {
            /* Fill in raw length */
            CH_DataAddHeader(txqe, remcid,
                             req->raw_length,
                             NULL, 0);

            /* If this is a zero-MBLK, it may be a McDSP packet. In
             * this case, piggeback the non-existant data via the
             * trailer size */
            if(req->data == NULL)
            {
                txqe->trailersize = req->raw_length;
            }
        }

        /* Stuff data into queue */
        if(cidcb != NULL)
        {
            priority = cidcb->priority;
            queue = CID_GetTxQueue(cidcb);
        }
        else
        {
            priority = 0;
            queue = &(chcb->queue);
        }
        CH_DataTxAppend(queue, txqe, priority);
        CH_DataSendQueued(chcb, queue, priority, FALSE);

        /* Unlink MBLK from primitive */
        req->data = NULL;
    }
}
#endif

/*! \brief Add F&EC-style callback element to transmit queue

    The F&EC code can only determine what to send when we are
    guarenteed that the channel is actually able to send something, so
    we insert a txqe callback element. The callback is performed by
    the CH_DataSendQueued() function.
*/
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
void CH_DataTxFecCallback(CIDCB_T *cidcb)
{
    TXQE_T *txqe;
    TXQUEUE_T *queue;

    queue = CID_GetTxQueue(cidcb);

    txqe = (TXQE_T*)CsrPmemZalloc(sizeof(TXQE_T) + L2CAP_SIZEOF_MAX_HEADER);
    txqe->type = L2CAP_FRAMETYPE_FLOW_CONTROL;
    txqe->cid = cidcb->local_cid;
    txqe->flush = cidcb->local_flush_to;

    CH_DataTxAppend(queue, txqe, cidcb->priority);
    CH_DataSendQueued(cidcb->chcb, queue, cidcb->priority, FALSE);
}
#endif


/*! \brief Add signal-style callback element to head of queue

    Signals need to take priority over all other data packets, so we
    place a callback on the head of the basic queue. Using the
    callback avoids a runtime check on the queue loop.
*/
void CH_DataTxSignalCallback(L2CAP_CHCB_T *chcb)
{
    TXQE_T *txqe;
    TXQUEUE_T *queue = &(chcb->queue);

    /* Don't flood the tx-queue */
    if(!chcb->signal_scheduled)
    {
        txqe = (TXQE_T*)CsrPmemZalloc(sizeof(TXQE_T) + L2CAP_SIZEOF_CID_HEADER);
        txqe->type = L2CAP_FRAMETYPE_SIGNAL;

        /* Insert as new head so signals take priority */
        txqe->next = queue->tx_queue[0];
        queue->tx_queue[0] = txqe;
        chcb->signal_scheduled = TRUE;

        CH_DataSendQueued(chcb, queue, 0, FALSE);
    }
}

/*! \brief Decide whether there is TX data pending to a device.

    Check TX queues for data that is waiting to be sent across
    HCI and also for HCI credits whose return we are awaiting.

    \param chcb Pointer to L2CAP connection handle control block.
    \returns TRUE if there is TX data pending, FALSE otherwise.
*/
CsrBool CH_DataTxPending(L2CAP_CHCB_T *chcb)
{
    TXQE_T **end, **q;

    if (chcb->queue.tx_active                       /* loop active */
            || chcb->queue.tx_current != NULL       /* current txqe */
            || chcb->queue.tx_done != NULL)         /* wait-for-ncp */
        return TRUE;

    for (q = chcb->queue.tx_queue, end = q + L2CAP_MAX_TX_QUEUES; q != end; ++q)
        if (*q != NULL)                             /* pending queues */
            return TRUE;

    return FALSE;
}

/*! \brief Raw reception path

    Investigate if a packet has been received on a CID that's
    currently in rx-raw mode. If so, pass the data up to the
    application unprocessed
*/
#ifdef INSTALL_L2CAP_RAW_SUPPORT
static void CH_DataRxRaw(L2CAP_CHCB_T *chcb, CsrMblk *mblk)
{
    CsrUint8 header[L2CAP_SIZEOF_CID_HEADER];
    CsrUint16 cid;

    if(CsrMblkReadHead(&mblk, header, L2CAP_SIZEOF_CID_HEADER) == L2CAP_SIZEOF_CID_HEADER)
    {
        /* Length can be read as UINT16_R(header, L2CAP_PKT_POS_LENGTH); */
        cid = UINT16_R(header, L2CAP_PKT_POS_CID);
    }
    else
    {
        cid = 0;
    }

    /* Always pass up data */
    L2CA_RawDataInd(chcb->raw_handle,
                    CH_GET_TBDADDR(chcb),
                    L2CA_DATA_RAW,
                    L2CA_RX,
                    cid,
                    mblk);
}
#endif

/*! \brief Receive signals/data packets.

    This function is called to handle receive L2CAP signals/data.

    \param chcb Pointer to CHCB instance.
    \param prim Data primitive with MBLK containing signals or data PDU.
    \param bc TRUE if received data is broadcast.
*/
void CH_DataRx(L2CAP_CHCB_T *chcb, CsrMblk *mblk, CsrBool bc)
{
    CsrMblkSize mblk_size;
    CsrUint16 payload_size;
    l2ca_cid_t cid = L2CA_CID_INVALID;
    CsrUint8 header[L2CAP_SIZEOF_CID_HEADER];
    CIDCB_T *cidcb = NULL;

#ifndef BUILD_FOR_HOST
    patch_fn(l2cap_shared_patchpoint);
#endif

#ifdef INSTALL_L2CAP_RAW_SUPPORT
    if(chcb->raw_rx)
    {
        /* Channel has full ACL raw mode on */
        CH_DataRxRaw(chcb, mblk);
        return;
    }
#endif

    /* Make sure that we can read in the complete header */
    if(CsrMblkReadHead(&mblk, header, L2CAP_SIZEOF_CID_HEADER) == L2CAP_SIZEOF_CID_HEADER)
    {
        /* Get size of payload */
        payload_size = UINT16_R(header, L2CAP_PKT_POS_LENGTH);

        /* Get the CID if we can */
        cid = UINT16_R(header, L2CAP_PKT_POS_CID);

        /* Fetch the CIDCB structure now. If it doesn't exist, we'll
         * handle it further down. Signals don't care about this  */
        cidcb = CIDME_GetCidcbWithHandle(chcb, cid);

#ifdef INSTALL_L2CAP_RAW_SUPPORT
        /* CH/CID raw mode requires us to send up complete packet */
        if((cidcb != NULL) && (cidcb->raw_rx))
        {
            /* Transfers ownership of mblk */
            L2CA_RawDataInd(cidcb->p_handle,
                            CH_GET_TBDADDR(chcb),
                            L2CA_DATA_RAW,
                            L2CA_RX,
                            cid,
                            mblk);
            return;
        }

        /* If we're just sniffing then take a copy of the mblk. */
        if (chcb->raw_hci_sniff)
        {
            CsrUint8 *data;
            CsrUint16 length = CsrMblkGetLength(mblk);

            if ((data = xzpmalloc(length)) != NULL)
            {
                CsrMblk *duplicate;

                CsrMblkCopyToMemory(mblk,
                                    0,
                                    length,
                                    data);

                duplicate = CsrMblkDataCreate(data, length, TRUE);

                if (duplicate != NULL)
                    L2CA_RawDataInd(chcb->raw_handle,
                                    CH_GET_TBDADDR(chcb),
                                    L2CA_DATA_RAW_COPY,
                                    L2CA_RX,
                                    cid,
                                    duplicate);
                else
                    CsrPmemFree(data);
            }
        }
#endif

        /* Get MBLK data size */
        mblk_size = CsrMblkGetLength(mblk);

        /* Check the payload size is correct, receiving a partial
         * L2CAP packet is expected behaviour when the remote device
         * is using flush timeout */

        /* Make sure that size matches and that only connectionless L2CAP
           is sent via broadcast. */
        if(payload_size == mblk_size && (cid == L2CA_CID_CONNECTIONLESS || !bc))
        {
            /* Route by cid. */

            switch (cid)
            {
                case L2CA_CID_INVALID:
                    /* Do nothing. */
                    break;

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
                case L2CA_CID_SIGNAL:
                    if (payload_size >= L2CAP_SIZEOF_SIGNAL_HEADER && !CH_IS_ULP(chcb))
                    {
                        if(payload_size <= L2CAP_SIGNAL_INCOMING_MTU_MAX)
                        {
                            /* Process received signals if MTU is ok */
                            SIGH_SignalReceive(chcb, &mblk, payload_size);
                        }
                        else
                        {
                            /* Report invalid MTU to peer if exceeded */
                            SIGH_SignalMtuExceeded(chcb, mblk, payload_size);
                        }
                    }
                    break;
#endif

#ifdef INSTALL_ULP
                case L2CA_CID_LE_SIGNAL:
                    /* Low energy signalling channel */
                    if (payload_size >= L2CAP_SIZEOF_SIGNAL_HEADER && CH_IS_ULP(chcb))
                        SIGH_LeSignalReceive(chcb, &mblk, payload_size);
                    break;
#endif

                default:
                    /* Data packet. */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
                    if (cid < L2CA_CID_DYNAMIC_FIRST)
                    {
                        /* Fixed CID */
                        psm_t psm = L2CA_PSM_INVALID;

#if defined(INSTALL_L2CAP_UCD_SUPPORT) \
                    || defined (INSTALL_L2CAP_CONNLESS_SUPPORT)
                        if (cid == L2CA_CID_CONNECTIONLESS)
                        {
                            if (mblk_size < L2CAP_SIZEOF_PSM_FIELD
                                    || mblk_size > L2CAP_SIZEOF_PSM_FIELD
                                        + L2CAP_CL_MTU
                                    || !CsrBtMblkReadHead16(&mblk, &psm)
                                    || !MCB_ReceiveConnectionlessAllowed(psm, bc))
                            {
                                /* Failure. Ensure that data is discarded. */
                                cid = L2CA_CID_INVALID;
                            }
                        }

#endif /* INSTALL_L2CAP_UCD_SUPPORT || INSTALL_L2CAP_CONNLESS_SUPPORT */
                    /* Remote CID is the fixed channel number, so just
                     * make sure that the range is lower than the
                     * first dynamic one and we'll get the CIDCB.
                     * If not found, try to create fixed CIDCB. */
                         if (cidcb == NULL && (cidcb = CIDME_GetCLCidcbRemote(
                                         chcb, cid, psm)) == NULL)
                             cidcb = MCB_MapFixedCidInd(chcb, cid, psm);

                         /* Data received on fixed channel. ACL might not
                            be locked. So extend idle timeout. */
                         if (cidcb != NULL && !CH_IS_CONNECTIONLESS(chcb))
                             dm_acl_idle_timeout_extend_short(CH_GET_ACL(chcb));
                    }
#endif /* INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT */

                    if (cidcb != NULL)
                    {
                        /* Pass data to CIDCB return to avoid freeing the MBLK*/
                        if (CID_DataReadInd(cidcb, mblk, payload_size, header))
                            return;
                    }

                    break;
            } /* switch CID */
        } /* payload size ok */
        else
        {
            /* Sizes don't match. */
            CIDCB_T *fcidcb;

            if ((fcidcb = CIDME_GetCidcbWithHandle(chcb, cid)) != NULL)
                L2CA_DataReadInd(fcidcb, NULL, L2CA_DATA_FAIL, 0, NULL);
        }
    }

    /* Destroy MBLK */
    CsrMblkDestroy(mblk);
}

/*! \brief Free a tx queue element, possibly with ack

    Helper function to free, and possible acknowledge, a tx queue
    element.

    \param txqe Pointer to TX queue element.
    \param result failure result code or L2CA_DATA_SILENT to send nothing.
*/
void CH_FlushTxqe(TXQE_T *txqe, l2ca_data_result_t result)
{
    CIDCB_T *cidcb;

    cidcb = CIDME_GetCidcb(txqe->cid);

    /* Any send acknowledgments if there's a CIDCB and allowed to do
     * so */
    if(cidcb != NULL && result != L2CA_DATA_SILENT)
    {
        switch (txqe->type)
        {
            case L2CAP_FRAMETYPE_NORMAL:
            case L2CAP_FRAMETYPE_CONNECTIONLESS:
                /* Cfm function is cidcb-NULL-safe */
                L2CA_DataWriteCfm(cidcb, txqe->context, txqe->sent, result);
                break;

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
            case L2CAP_FRAMETYPE_FLOW_CONTROL:
                if (CID_IsFlowControl(cidcb))
                    FLOW_DataWriteComplete(cidcb, txqe, result);
                break;
#endif
            default:
                break;
        }
    }

    CH_EmptyAbortQueue(cidcb, &txqe->aborted, result);
    CsrMblkDestroy(txqe->mblk);
    CsrPmemFree(txqe);
}

/*! \brief Empty data queue of packets from specified channel.

    This function will empty the data queue of packets from the specified channel,
    the packets will be destroyed.

    \param chcb Pointer to CHCB instance.
    \param cid The channel ID for the packets to destroy. L2CA_CID_INVALID
               can be used to target all TXQEs
    \param result failure result code or L2CA_DATA_SILENT to send nothing.
    \return Number of credits salvaged from queue->tx_current
*/
CsrUint16 CH_FlushPendingQueueWithCid(TXQUEUE_T *queue,
                                     l2ca_cid_t cid,
                                     l2ca_data_result_t result)
{
    TXQE_T *elp, **elpp, **q, **end;
    CsrUint16 credits = 0;

    /* Special case for the current tx element */
    if(queue->tx_current != NULL)
    {
        if((queue->tx_current->cid == cid) || (cid == L2CA_CID_INVALID))
        {
            credits = queue->tx_current->credits;
            CH_FlushTxqe(queue->tx_current, result);
            queue->tx_current = NULL;
        }
    }

    /* Traverse the transmit queues */
    for (q = queue->tx_queue, end = q + L2CAP_MAX_TX_QUEUES; q != end; ++q)
    {
        for (elpp = q; (elp = *elpp) != NULL; /* empty */ )
        {
            if (cid == L2CA_CID_INVALID || elp->cid == cid)
            {
                *elpp = elp->next;
                CH_FlushTxqe(elp, result);
            }
            else
                elpp = &elp->next;
        }
    }

    return credits;
}

/*! \brief Empty the "done" queue

    This function will flush the "tx_done" queue for the specified
    queue object. Function will return the number of credits freed.

    \param chcb Pointer to CHCB instance.
    \param result failure result code or L2CA_DATA_SILENT to send nothing.
*/
CsrUint16 CH_FlushDoneQueue(TXQUEUE_T *queue, l2ca_data_result_t result)
{
    TXQE_T *txqe;
    CsrUint16 credits = 0;

    while(queue->tx_done != NULL)
    {
        /* Get head element and move to next */
        txqe = queue->tx_done;
        credits += txqe->credits;
        queue->tx_done = txqe->next;
        CH_FlushTxqe(txqe, result);
    }
    return credits;
}


/*! \brief Flush CHCB of channel data.

    This function will flush out any queued data in the CHCB instance
    for the specified CIDCB instance. Upper layer is informed of any
    data that was partially sent.

    \param chcb Pointer to CHCB instance.
    \param cidcb Pointer to CIDCB instance.
    \param result failure result code or L2CA_DATA_SILENT to send nothing.
*/
void CH_FlushCidcbData(L2CAP_CHCB_T *chcb,
                       CIDCB_T *cidcb,
                       l2ca_data_result_t result)
{
    /* Empty the data queue of packet belonging to the CID */
    if (!CH_IS_CONNECTIONLESS(chcb))
        CH_GET_ACL(chcb)->pending_credits
                += CH_FlushPendingQueueWithCid(&(chcb->queue),
                                               cidcb->local_cid,
                                               result);

    /* With AMP spport there may be a specific AMP queue. Encase AMP
     * wasn't enabled, the basic queue will just be cleared twice */
#ifdef INSTALL_AMP_SUPPORT
    (void)CH_FlushPendingQueueWithCid(CID_GetTxQueue(cidcb),
                                      cidcb->local_cid,
                                      result);
#endif


#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT

    /* Empty the signal queues of signals belonging to this CID */
    SIGH_SignalQueueEmptyWithCid(&chcb->signal_queue,
                                 cidcb->local_cid);
    SIGH_SignalQueueEmptyWithCid(&chcb->signal_pending,
                                 cidcb->local_cid);
#endif
}

/*! \brief Flush CHCB of queued data and signals.

    This is the main CHCB cleanup function.

    This function frees any queued signals and data on the specified
    CHCB. Upper layer is informed of any data that was partially sent
    unless result == L2CA_DATA_SILENT.

    \param chcb Pointer to CHCB instance.
    \param result failure result code or L2CA_DATA_SILENT to send nothing.
*/
CsrUint16 CH_FlushChcb(L2CAP_CHCB_T *chcb, l2ca_data_result_t result)
{
    TYPED_BD_ADDR_T *addrt = NULL;
    TXQUEUE_T *queue;
    CsrUint16 credits;

    if (result != L2CA_DATA_SILENT && !CH_IS_CONNECTIONLESS(chcb))
        addrt = CH_GET_TBDADDR(chcb);

    SIGH_SignalQueueEmpty(&chcb->signal_pending, &TBDADDR_ADDR(*addrt));
    SIGH_SignalQueueEmpty(&chcb->signal_queue, &TBDADDR_ADDR(*addrt));

    /* Flush the basic radio transmit queue */
    queue = &(chcb->queue);
    credits = CH_FlushPendingQueueWithCid(queue,
                                          L2CA_CID_INVALID,
                                          result)
            + CH_FlushDoneQueue(queue, result);

    /* Flush the AMP queues */
#ifdef INSTALL_AMP_SUPPORT
    for(queue = chcb->amp_queues; queue != NULL; queue = queue->next_ptr)
    {
        (void)CH_FlushPendingQueueWithCid(queue,
                                          L2CA_CID_INVALID,
                                          result);
        (void)CH_FlushDoneQueue(queue, result);
    }
#endif

    /* Free duplicate signal detection block */
    CsrMblkDestroy(chcb->signal_dup.sig_data);
    chcb->signal_dup.sig_data = NULL;
    chcb->signal_dup.type  = 0;
    chcb->signal_dup.identifier = 0;
    chcb->signal_dup.length = 0;

#ifdef BUILD_FOR_HOST
    /* Free ACL reassembly buffer */
    CsrMblkDestroy(chcb->reassem.mblk);
    chcb->reassem.length = 0;
    chcb->reassem.offset = 0;
#endif

    /* Return number of credits freed */
    return credits;
}

/*! \brief Destroy CHCB instance.

    This function destroys a CHCB instance, all CIDS on the CHCB are closed
    down.

    \param chcb Pointer to CHCB instance.
    \param status Close status to report to CIDs.
*/
static CsrUint16 CH_Destroy(L2CAP_CHCB_T *chcb, CsrUint16 status)
{
    CsrUint16 reserved_credit;
    CIDCB_T *cidcb;
    CIDCB_T **pcidcb = &chcb->cidcb_list;

    /* Notify CIDs connection has gone, CID may remove itself from list */
    /* Use double indirection to detect and cope with CIDCB destruction */
    while ((cidcb = *pcidcb) != NULL)
    {
        CID_AclCloseInd(cidcb, status);

        if (*pcidcb == cidcb)
        {
            /* CIDCB not destroyed, so advance. */
            pcidcb = &cidcb->next_ptr;
        }
    }

    /* Flush any data/signals */
    reserved_credit = chcb->queue.reserved_credit;
    chcb->queue.reserved_credit = 0;

    return CH_FlushChcb(chcb, L2CA_DATA_LINK_TERMINATED) + reserved_credit;
}

/*! \brief Initialise non-zero values of CHCB instance.

    This is called from the ACL manager after it has just zpmalloced
    the ACL record structure and before it starts trying to connect.
    It is also called after a connection has closed and the ACL
    manager has reset everything to zero.

    Note. You must not allocate and attach memory here as the
    ACL structure may be about to be deleted.

    \param chcb Pointer to CHCB structure.
*/
void CH_Reset(L2CAP_CHCB_T *chcb)
{
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    chcb->signal_mtu = L2CAP_SIGNAL_EXT_MTU_MAX;
#else
    PARAM_UNUSED(chcb);
#endif
}

/*! \brief Release a lock on the ACL identified by the CHCB

    When a CHCB is no longer needed, tell the DM that so it
    may close the ACL encase no other users are present

    \param chcb Pointer to Connection handle control block.
    \param reason Reason for disconnecting.
    \param timeout Number of multiples of idle timeout required.
*/
void CH_Close(L2CAP_CHCB_T *chcb,
              hci_reason_t reason,
              ACL_IDLE_TIMEOUT_MULTIPLIER_T timeout)
{
    /* We don't free the CL block. We will free it at shutdown */
    if (!CH_IS_CONNECTIONLESS(chcb))
        dm_acl_close(CH_GET_ACL(chcb),
                     dm_acl_client_l2cap, /* decrement lock */
                     reason,
                     timeout);
}

void CH_StandardClose(L2CAP_CHCB_T *chcb)
{
    CH_Close(chcb, HCI_ERROR_OETC_USER, (CH_IS_ULP(chcb)?
        ACL_IDLE_TIMEOUT_AT_LEAST_NONE:ACL_IDLE_TIMEOUT_AT_LEAST_TINY));
}

/*! \brief In abnormal circumstances we may need to kill an ACL

    Exterminate the ACL, Dalek-style.
*/
void CH_Exterminate(L2CAP_CHCB_T *chcb)
{
    /* We don't free the CL block. We will free it at shutdown */
    if (!CH_IS_CONNECTIONLESS(chcb))
        dm_acl_close(CH_GET_ACL(chcb),
                     dm_acl_client_max, /* force disconnect */
                     HCI_ERROR_OETC_USER,
                     ACL_IDLE_TIMEOUT_AT_MOST_NONE); /* don't use timeout */
}

/*! \brief Find or create data structures for a connection
           and initiate connection procedures if necessary.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param psm PSM to be connected or L2CA_PSM_INVALID if not applicable.
    \param pp_chcb Function passes back pointer to CHCB structure.
    \param flags See l2ca_conflags_t in l2cap_prim.h.

    \returns Generic L2CAP error code: INITIATING/NOT_READY/OUT_OF_MEM
*/
l2ca_conn_result_t CH_Connect(const TYPED_BD_ADDR_T *const addrt,
                              psm_t psm,
                              L2CAP_CHCB_T **pp_chcb,
                              l2ca_conflags_t flags)
{
    /* Check HCI is initialised */
    if((!L2CA_FLAGS_ULP(flags & DM_ACL_CONNECTION_MASK) &&
         LYMCB.fc.max_acl_data_packet_length != 0)
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
         || (L2CA_FLAGS_ULP(flags & DM_ACL_CONNECTION_MASK) &&
         LYMCB.le_fc.max_acl_data_packet_length != 0)
#endif
     )
    {
        CsrUint16 acl_flags;
        DM_ACL_T *p_acl;
        DM_SM_SERVICE_T service;

        service.protocol_id = SEC_PROTOCOL_L2CAP;
        service.channel = psm;

        acl_flags = flags &
                (L2CA_CONFLAG_ENUM_MASK |
                 L2CA_CONFLAG_PAGE_TIMEOUT |
                 L2CA_CONFLAG_INCOMING);

        /* Get CHCB from ACL Manager. */
        if (dm_acl_open(addrt,
                        &service,
                        dm_acl_client_l2cap,
                        acl_flags,
                        &p_acl) != DM_ACL_FAILED)
        {
            *pp_chcb = CH_GET_CHCB(p_acl);
            return L2CA_CONNECT_SUCCESS;
        }
        else
        {
            *pp_chcb = NULL;
            return L2CA_CONNECT_OUT_OF_MEM;
        }
    }

    return L2CA_CONNECT_NOT_READY;
}

/*! \brief Connect confirmation.

    Handle connect confirmation from the device manager.

    \param chcb Pointer to CHCB instance.
    \param cfm DM_ACL_CONNECT_CFM from device manager.
*/
void CH_ConnectInd(DM_ACL_T *p_acl, CsrUint8 status)
{
    L2CAP_CHCB_T *chcb = CH_GET_CHCB(p_acl);
    if (status == HCI_SUCCESS)
    {
        CIDCB_T *cidcb;
        CsrUint8 priority;

        /* Tell all CIDs that the ACL connection is ready */
        for (cidcb = chcb->cidcb_list; cidcb != NULL; cidcb = cidcb->next_ptr)
        {
            CID_AclReady(cidcb);
        }

        /* Reserve credit for new ACL. */
        dm_amp_reserve_credit(&chcb->queue, CH_GET_ACL_TYPE(chcb));

        for (priority = 0; priority != L2CAP_MAX_TX_QUEUES; ++priority)
        {
            /* Check if we have any queued data to send */
            CH_DataSendQueued(chcb, &chcb->queue, priority, FALSE);
        }
    }
    else
    {
        /* Destroy this instance */
#ifdef L2CAP_HCI_DATA_CREDIT_CHECKS_PANIC
        if (CH_Destroy(chcb, status) != 0)
            BLUESTACK_PANIC(CSR_BT_PANIC_L2CAP_HCI_DATA_CREDITS_INCONSISTENT);
#else
        (void)CH_Destroy(chcb, status);
#endif
    }
}

/*! \brief Disconnect indication.

    Handle disconnect indication from the device manager.

    \param chcb Pointer to CHCB instance.
    \param ind DM_ACL_CONNECT_IND from device manager.
*/
CsrUint16 CH_DisconnectInd(DM_ACL_T *p_acl, CsrUint16 reason)
{
    /* Destroy this instance */
    return CH_Destroy(CH_GET_CHCB(p_acl), reason);
}

/*! \brief AMP physical connect confirm

    DM may have established a physical AMP channel for us. Relay this
    information into the state machine and crank it.
*/
#ifdef INSTALL_AMP_SUPPORT
void CH_AmpPhysicalConnectCfm(L2CAP_CHCB_T *chcb,
                              hci_return_t status,
                              l2ca_cid_t cid,
                              l2ca_controller_t remote_amp_id)
{
    CIDCB_T *cidcb;

    cidcb = CIDME_GetCidcbWithHandle(chcb, cid);
    if(cidcb != NULL)
    {
        CID_AmpCreatePhysicalCfm(cidcb, status, remote_amp_id);
    }
}
#endif

/*! \brief AMP logical channel connect confirm

    DM may have established a logical AMP channel for us. Relay this
    information into the state machine.
*/
#ifdef INSTALL_AMP_SUPPORT
void CH_AmpLogicalConnectCfm(L2CAP_CHCB_T *chcb,
                             DM_AMPM_CONNECT_CHANNEL_RSP_T *prim)
{
    CIDCB_T *cidcb;

    if ((cidcb = CIDME_GetCidcbWithHandle(chcb, prim->cid)) != NULL)
        CID_AmpCreateLogicalCfm(cidcb, prim);
}
#endif

/*! \brief AMP disconnect indication

    The AMP channel has gone. Divert function into the AMP utility
    manager.
*/
#ifdef INSTALL_AMP_SUPPORT
void CH_AmpDisconnectInd(L2CAP_CHCB_T *chcb,
                         l2ca_controller_t local_amp_id,
                         hci_error_t reason)
{
    CIDCB_T *cidcb;
    /* Loop all CIDCBs in the connection and send them the link loss
     * if they are AMP-enabled and matches the controller ID */
    for(cidcb = chcb->cidcb_list; cidcb != NULL; cidcb = cidcb->next_ptr)
    {
        /* Make sure channel matches AMP and is open */
        if(CID_IsAmp(cidcb) &&
           (cidcb->amp_info->local_amp == local_amp_id))
        {
            CID_AmpDisconnectInd(cidcb, reason);
        }
    }
}
#endif

/*! \brief AMP has verified whether a physical link exists

    When handling incoming move/create requests, we must make sure
    that the peer has actually established the link before continuing.
    This function handles that verify-response from the DM/AMPM
*/
#ifdef INSTALL_AMP_SUPPORT
void CH_AmpVerifyPhysicalLinkCfm(L2CAP_CHCB_T *chcb,
                                 CsrUint16 identifier,
                                 CsrBool exists,
                                 l2ca_fs_flush_t link_supervision_timeout,
                                 l2ca_fs_flush_t best_effort_flush_timeout)
{
    CIDCB_T *cidcb;
    CID_VERIFY_LINK_CFM_T sig_args;

    sig_args.lsto = link_supervision_timeout;
    sig_args.flush_to = best_effort_flush_timeout;

    /* We use the CID as the identifier. Find the CIDCB and crank the FSM */
    cidcb = CIDME_GetCidcbWithHandle(chcb, (l2ca_cid_t)identifier);
    if(cidcb != NULL)
    {
        CID_AmpVerifyPhysicalLinkCfm(cidcb, exists, &sig_args);
    }
}
#endif

/*! \brief Remove CIDCB.

    Remove CIDCB from the connection, flushs any signals/data for the CIDCB.

    \param chcb Pointer to CHCB instance.
    \param cidcb Pointer to CIDCB instance.
*/
void CH_RemoveCidcb(L2CAP_CHCB_T *chcb, struct cidtag *cidcb)
{
    if (CH_IS_CONNECTED(chcb))
    {
        /* Flush data for this CIDCB (if required) */
        CH_FlushCidcbData(chcb, cidcb, L2CA_DATA_LINK_TERMINATED);
    }
}

/*! \brief NCPs received from HCI

    Indication that packets have been sent across the air.

    \param chcb Pointer to l2cap connection record.
    \param completed_packets number of completed packets.
*/
void CH_CompletedPackets(L2CAP_CHCB_T *chcb, CsrUint16 completed_packets)
{
    CH_DataSendComplete(&(chcb->queue), completed_packets);
}

#ifdef INSTALL_AMP_SUPPORT
/*! \brief Number of completed blocks/packets from AMPM

    When credits come back from an AMP, all we have is the logical
    handle. We then use the logical_lookup cache to quickly find the
    correct CHCB and queue that the credits are ment for.

*/
void CH_AmpCompletedPackets(const DM_AMPM_NCB_T *const ncb)
{
    LOGIC_Q_LOOKUP_T *lookup;

    for (lookup = mcb.logical_lookup; lookup != NULL; lookup = lookup->next)
    {
        if (ncb->logical_link_handle == lookup->logical_handle
                || ncb->logical_link_handle == 0)
        {
            CsrUint8 priority;

            /* Generate confirms for matching logical channel. */
            CH_DataSendComplete(lookup->queue,
                                ncb->num_of_completed_blocks);

            /* Poke queues to send more data. */
            for (priority = 0; priority < L2CAP_MAX_TX_QUEUES; ++priority)
                CH_DataSendQueued(lookup->chcb, lookup->queue, priority, TRUE);

            /* Nothing else will match, unless it's a global wakeup call */
            if (ncb->logical_link_handle != 0)
                break;
        }
    }
}
#endif

#ifdef INSTALL_AMP_SUPPORT
/*! \brief Re-calculate MPS

    Different AMPs may have different packet or HCI limitiations.
    This function forces a recalculation of the MPS for all links with
    the givel local AMP controller id.
*/
void CH_AmpRecalculateMPS(l2ca_controller_t local_amp_id)
{
    l2ca_cid_t index;

    /* Scan list */
    for(index = 0; index < L2CAP_MAX_NUM_CIDS; index++)
    {
        CIDCB_T *cidcb = mcb.cid_table[index];

        if((cidcb != NULL) &&
           CID_IsAmp(cidcb) &&
           CID_IsFlowControl(cidcb) &&
           (cidcb->amp_info->local_amp == local_amp_id))
        {
            FLOW_ClipMPS(cidcb);
        }
    }
}
#endif

/*! \brief Abort all unsent data on queue->tx_queue.

    If there is data to be aborted then we will try to send confirmations
    to the application immediately. If this is not possible then we queue
    the confirmations until they can be sent.

    \param cidcb Pointer to channel control structure.
*/
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
void CH_AbortTX(CIDCB_T *cidcb)
{
    L2CAP_ABORTED_PACKET_T **abort_queue = NULL;
    TXQE_T *last_transmitted, *elp, **elpp, **q, **end;
    TXQUEUE_T *queue = CID_GetTxQueue(cidcb);

    /* Find last transmitted packet (full or partial). */
    if ((last_transmitted = queue->tx_current) == NULL)
    {
        for (   last_transmitted = queue->tx_done;
                last_transmitted != NULL && last_transmitted->next != NULL;
                last_transmitted = last_transmitted->next)
            ;
    }

    if (last_transmitted != NULL)
        abort_queue = &last_transmitted->aborted;

    /* Remove all Basic (including connectionless) from tx queue. */
    for (q = queue->tx_queue, end = q + L2CAP_MAX_TX_QUEUES; q != end; ++q)
    {
        for (elpp = q; (elp = *elpp) != NULL; /* empty */)
        {
            if (elp->cid == cidcb->local_cid
                    && (elp->type == L2CAP_FRAMETYPE_NORMAL
                        || elp->type == L2CAP_FRAMETYPE_CONNECTIONLESS))
            {
                CH_AbortElement(cidcb, elp->context, &abort_queue);

                *elpp = elp->next;
                CH_FlushTxqe(elp, L2CA_DATA_SILENT);
            }
            else
                elpp = &elp->next;
        }
    }

    CH_AbortCfm(cidcb, abort_queue);
}
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
/*! \brief Either send an L2CA_DATAWRITE_ABORT_CFM now, or add it to the
           end of the abort queue.
    \param cidcb Pointer to L2CAP Channel Identifier Control Block.
    \param abort_queue Pointer to queue of L2CAP CFM primitives.
*/
void CH_AbortCfm(CIDCB_T *cidcb, L2CAP_ABORTED_PACKET_T **abort_queue)
{
    L2CA_DATAWRITE_ABORT_CFM_T *prim = NULL;

    if (abort_queue != NULL)
    {
        L2CAP_ABORTED_PACKET_T **elpp, *elp;

        /* Add to end of queue. */
        for (elpp = abort_queue; (elp = *elpp) != NULL; elpp = &elp->next)
            ;

        elp = *elpp = zpnew(L2CAP_ABORTED_PACKET_T);
        prim = &elp->u.abort_cfm;
    }

    L2CA_DataWriteAbortCfm(cidcb, prim);
}
#endif

/*! \brief Abort a data element queued for TX. If abort_queue is
    NULL then we send L2CA_DATAWRITE_CFM immediately. Otherwise
    we queue it for later.

    \param cidcb Pointer to channel control block structure.
    \param context Opaque context from L2CA_DATAWRITE_REQ.
    \param abort_queue Pointer to pointer to abort queue, or NULL.
    \returns Pointer to pointer to new last element in list, or NULL.
*/
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
void CH_AbortElement(CIDCB_T *cidcb,
                     CsrUint16 context,
                     L2CAP_ABORTED_PACKET_T ***abort_queue)
{
    /* Build primitive */
    L2CAP_ABORTED_PACKET_T *pkt = pnew(L2CAP_ABORTED_PACKET_T);
    L2CA_BuildDataWriteCfm(cidcb,
                           context,
                           0,
                           L2CA_DATA_LOCAL_ABORTED,
                           &pkt->u.datawrite_cfm);
    pkt->next = NULL;

    /* Either send now, or queue for later. */
    if (*abort_queue == NULL)
    {
        L2CA_PrimSend(&pkt->u.common);
    }
    else
    {
        L2CAP_ABORTED_PACKET_T **elpp, *elp;

        /* Add to end of queue. */
        for (elpp = *abort_queue; (elp = *elpp) != NULL; elpp = &elp->next)
            ;

        *elpp = pkt;

        /* Make finding the end faster next time. */
        *abort_queue = &pkt->next;
    }
}
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

/*! \brief Empty the abort queue, possibly sending L2CA_DATAWRITE_CFMs
           to application.

    \param cidcb Pointer to channel control block.
    \param abort_queue Pointer to pointer to queue.
    \param result Set to L2CA_DATA_SILENT to suppress messages to application.
*/
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
void CH_EmptyAbortQueue(CIDCB_T *cidcb,
                        L2CAP_ABORTED_PACKET_T **abort_queue,
                        l2ca_data_result_t result)
{
    L2CAP_ABORTED_PACKET_T *pkt;

    while ((pkt = *abort_queue) != NULL)
    {
        *abort_queue = pkt->next;

        if (result == L2CA_DATA_SILENT)
            CsrPmemFree(pkt);
        else
            L2CA_PrimSend(&pkt->u.common);
    }
}
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

/*! \brief NCP event received for connectionless data.

    We just call CH_CompletedPackets() with the connectionless chcb.

    \param completed_packets number of completed packets.
*/
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
void CH_CompletedPacketsCL(CsrUint16 completed_packets)
{
    CH_CompletedPackets(mcb.connectionless, completed_packets);
}
#endif
