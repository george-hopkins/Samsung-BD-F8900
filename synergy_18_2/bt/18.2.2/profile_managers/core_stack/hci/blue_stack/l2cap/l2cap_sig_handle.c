/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_sig_handle.c

\brief  This file handles L2CAP signals for an ACL.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"
#include "csr_sched.h"
#include "dm_mgt_interface.h"
#include "dm_hci_interface.h"
#include "error.h"

#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_interface.h"
#include "l2cap_control.h"

#ifdef INSTALL_AMP_SUPPORT
#include "l2cap_amp_utils.h"
#endif

#ifndef BUILD_FOR_HOST
#include "patch.h"
#endif

/*! Signal minimum length array */
#define SIGNAL_TABLE_X(a, b) b
#define SIGNAL_TABLE_SEP ,
static const CsrUint16 signal_min_length[] =
{
        SIGNAL_TABLE
};
#undef SIGNAL_TABLE_X
#undef SIGNAL_TABLE_SEP

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
#ifdef INSTALL_AMP_SUPPORT
#define CHECK_SIGNAL_INVALID(x) \
                       (x >= SIGNAL_UNKNOWN)
#else
#define CHECK_SIGNAL_INVALID(x) \
                       (x >= SIGNAL_UNKNOWN) || \
                       (x > SIGNAL_INFO_RES)
#endif
#endif

typedef struct
{
    CsrUint8 id;                     /* Signal identifier */
    CsrUint8  type;                  /* Signal opcode */
    CsrUint16 length;                /* in-packet length (don't trust this!) */
    CsrUint16 length_with_header;    /* Signal size, including L2CAP header */
    const CsrUint8 *data;            /* Signal data. */
} L2CAP_SIGNAL_INFO_T;

/* Local prototypes */
static void SIGH_RtxTimeout(CsrUint16 signal_id_in, void *chcb_in);
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static void SIGH_ErtxTimeout(CsrUint16 signal_id_in, void *chcb_in);
#endif

/* Signal handler functions */
#define SIGNAL_TABLE_SEP ;
#define SIGNAL_TABLE_X(a, b) static CsrUint16 SIGH_Handle_ ## a \
    (L2CAP_CHCB_T *chcb, L2CAP_SIGNAL_INFO_T *sig)
SIGNAL_TABLE;
#undef SIGNAL_TABLE_SEP
#undef SIGNAL_TABLE_X

/* Signal function array */
#define SIGNAL_TABLE_X(a, b) SIGH_Handle_ ## a
#define SIGNAL_TABLE_SEP ,
static CsrUint16 (*const signal_handle[])(L2CAP_CHCB_T *chcb,
                                         L2CAP_SIGNAL_INFO_T *sig) =
{
    SIGNAL_TABLE
};
#undef SIGNAL_TABLE_X
#undef SIGNAL_TABLE_SEP
/*! \brief Record information from request signal to detect duplicates.

    \param dup Pointer to duplicate request signal detection block.
    \param sig Pointer to information block for request signal.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static void SIGH_DuplicateSetRequest(DUPSIG_T *dup, L2CAP_SIGNAL_INFO_T *sig)
{
    /* Free any existing response */
    CsrMblkDestroy(dup->sig_data);
    dup->sig_data = NULL;
    dup->identifier = sig->id;
    dup->type = sig->type;
    dup->length = sig->length_with_header;
}
#endif

/*! \brief Detect a duplicate request signal

    If the peer sends us a duplicate request signal we "should"
    respond with a duplicate response. This effectively means that if
    we have cached a response signal, we can send that immediately.
    If no such response is available, we throw away the duplicate
    request.
*/
#if !defined(DISABLE_L2CAP_CONNECTION_FSM_SUPPORT) || defined(INSTALL_AMP_SUPPORT)

static CsrBool SIGH_DuplicateDetect(L2CAP_CHCB_T *chcb, L2CAP_SIGNAL_INFO_T *sig)
{
    /* Not a duplicate if any of: type; length; or identifier differ.
       Previously we only checked the identifier, but we need to work
       around the bad behaviour of some headsets. See B-66245. */
    if (chcb->signal_dup.type != sig->type ||
        chcb->signal_dup.length != sig->length_with_header ||
        chcb->signal_dup.identifier != sig->id)
    {
        return FALSE;
    }

    /* If we have a response then resend it. Otherwise ignore signal. */
    if (chcb->signal_dup.sig_data != NULL)
    {
        /* Create a new TXQE with the raw response signal */
        TXQE_T *txqe;

        txqe = xzpmalloc(sizeof(TXQE_T) + L2CAP_SIZEOF_CID_HEADER);
        if (txqe != NULL)
        {
            CsrMblkSize mblk_size;

            /* Use an MBLK duplicate. This both saves a bit of memory
             * and avoids a horrible race hazard if we should happen
             * to assign a new sig_data before the old one was sent.
             * If we can't create a duplicate then don't bother sending
             * anything at all. */
            mblk_size = CsrMblkGetLength(chcb->signal_dup.sig_data);
            txqe->mblk = CsrMblkDuplicateCreate(chcb->signal_dup.sig_data,
                                               0,
                                               mblk_size);
            if (txqe->mblk != NULL)
            {
                /* Everything has been allocated. Fill out the final
                 * details and stuff it onto the head of the
                 * basic-radio transmit queue */
                CH_DataAddHeader(txqe,
                                 (CsrUint16)CH_GET_SIGNAL_CID(chcb),
                                 mblk_size,
                                 NULL,
                                 0);
                txqe->type = L2CAP_FRAMETYPE_NORMAL;
                txqe->flush = FLUSH_INFINITE_TIMEOUT;
                txqe->next = chcb->queue.tx_queue[0];
                chcb->queue.tx_queue[0] = txqe;
                CH_DataSendQueued(chcb, &chcb->queue, 0, FALSE);
            }
            else
            {
                CsrPmemFree(txqe);
            }
        }
    }

    return TRUE;
}

#endif
/*! \brief Basic configuration option sanity checking

    When the peer sends us configuration options we'll need to check that
    the length headers actually makes sense before we start parsing it
    into our internal structures. This function scans such a byte-array and
    returns TRUE if buffer is OK, otherwise FALSE.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static CsrBool SIGH_ValidateConfigBuffer(CsrUint16 total_size,
                                        const CsrUint8 *buffer)
{
    CsrUint16 offset;

    /* Scan entire buffer */
    for(offset = 0; offset < total_size; /*nop*/ )
    {
        /* Buffer *must* include the header, and as there's always a
         * option payload, it must be greater than header size */
        if((total_size - offset) > L2CAP_SIZEOF_OPTION_HEADER)
        {
            /* Progress offset using length header field at index 1
             * and header size */
            offset += buffer[offset + 1] + L2CAP_SIZEOF_OPTION_HEADER;
        }
        else
        {
            /* Failure, break out */
            return FALSE;
        }
    }

    /* Buffer length is only valid when offset has reached end */
    return offset == total_size;
}
#endif

/*! \brief A new response is being sent, so cache it

    Whenever we send out a new reponse we must cache it such that the
    duplicate request detector can send out a duplicate response.
*/
#if !defined(DISABLE_L2CAP_CONNECTION_FSM_SUPPORT) || defined(INSTALL_AMP_SUPPORT)

void SIGH_DuplicateSetResponse(L2CAP_CHCB_T *chcb, SIG_SIGNAL_T *signal)
{
    /* Free old */
    CsrMblkDestroy(chcb->signal_dup.sig_data);
    chcb->signal_dup.sig_data = NULL;

    /* Our signal duplicate detection can only cope with one signal at a
       time. So before storing our response, we try to make sure that
       we're still dealing with the same signal by comparing the signal
       type and identifier. */
    if (signal != NULL
            && SIGNAL_REQUEST_FROM_RESPONSE(signal->signal_type)
                == chcb->signal_dup.type
            && signal->signal_id == chcb->signal_dup.identifier)
    {
        /* Create duplicate as the signal's contents will be freed as soon
           as the signal has been sent. If we can't create a duplicate then
           we won't be able to send a duplicate response. It's very unlikely
           that we will both not be able to create a duplicate and be called
           upon to send that duplicate. */
        chcb->signal_dup.sig_data = CsrMblkDuplicateCreate(
                signal->signal,
                0,
                CsrMblkGetLength(signal->signal));
    }
}
#endif

/*! \brief Obtain Signal ID

    This function returns a unique signal ID.

    The spec says:
    A different Identifier must be used for each original command.
    Identifiers should not be recycled until a period of 360
    seconds has elapsed from the initial transmission of the
    command using the identifier.

    The latter part of this statement is ambiguous.  There is no indication of
    what should be done in the eventuality of needing to re-use a signal_id
    less than 360 secs after sending it the first time!

    So, for a particular channel, the first signal will be ID=1, the next ID=2
    and so on, wrapping around from 255 to 1. If a signal is pending with the
    ID that was chosen, then increment on to the next one.

    \param chcb Pointer to CHCB instance.
    \return A new signal ID or zero if none available.
*/
static CsrUint8 SIGH_SignalIdObtain(L2CAP_CHCB_T *chcb)
{
    SIG_SIGNAL_T *sig_ptr;

    /* Increment signal ID, handle wrap around */
    chcb->signal_id = (chcb->signal_id + 1) & 0xFF;
    if (chcb->signal_id == 0)
        chcb->signal_id = 1;

    /* Check this signal ID isn't used on the pending signal queue */
    for (sig_ptr = chcb->signal_pending; sig_ptr != NULL; sig_ptr = sig_ptr->next_ptr)
    {
        /* Check if this signal has the same signal ID */
        if (sig_ptr->signal_id == chcb->signal_id)
        {
            /* Increment signal ID, handle wrap around */
            chcb->signal_id = (chcb->signal_id + 1) & 0xFF;
            if (chcb->signal_id == 0)
                chcb->signal_id = 1;
        }
    }

    /* Return unused signal ID */
    return chcb->signal_id;
}

/*! \brief Destroy signal

    This function frees the memory for the specified signal, the
    signal must not be on any queue when it is freed.  Any pending
    timer will be cancelled.

    \param sig_ptr Pointer to signal to destroy.
*/
static void SIGH_SignalDestroy(SIG_SIGNAL_T *sig_ptr)
{
    timer_cancel(&sig_ptr->timer_id);
    CsrMblkDestroy(sig_ptr->signal);
    CsrPmemFree(sig_ptr);
}

/*! \brief Empty specified signal queue.

    This function will empty the specified signal queue of all signals, the
    signals will be destroyed.

    \param sig_list Pointer to signal list to empty, the list pointers will be
           updated automatically.
    \param p_bd_addr BD_ADDR for the CHCB to which these queues exist. If this
           is NULL then we are not allowed to send messages to the upper layers
*/
void SIGH_SignalQueueEmpty(SIG_SIGNAL_T **sig_list,
                           const BD_ADDR_T *const p_bd_addr)
{
    SIG_SIGNAL_T *sig_ptr;
    SIG_SIGNAL_T *sig_free_ptr;

    /* Flush the pending queue and stop any timers */
    for (sig_ptr = *sig_list; sig_ptr != NULL;)
    {
        sig_free_ptr = sig_ptr;

        if (p_bd_addr != NULL)
        {
            /* Echo. GetInfo and ConnParUpdate need special attention */
            switch (sig_ptr->signal_type)
            {
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
                case SIGNAL_ECHO_REQ:
                    L2CA_PingCfm(sig_ptr->p_handle,
                                 p_bd_addr,
                                 L2CA_MISC_NO_CONNECTION,
                                 NULL,
                                 0,
                                 sig_ptr->local_cid); /* req_ctx */
                    break;

                case SIGNAL_INFO_REQ:
                    L2CA_GetInfoCfm(sig_ptr->p_handle,
                                    p_bd_addr,
                                    L2CA_GETINFO_TIMEOUT,
                                    SIG_ReadGetInfoType(sig_ptr),
                                    NULL,
                                    0,
                                    sig_ptr->local_cid); /* req_ctx */
                    break;
#endif
#ifdef INSTALL_ULP
                case SIGNAL_CONN_PARAM_UPDATE_REQ:
                {
                    TYPED_BD_ADDR_T bdaddr;

                    tbdaddr_copy_from_public_bd_addr(&bdaddr,p_bd_addr);

                    L2CA_ConnUpdateCfm(sig_ptr->p_handle,
                                       L2CA_MISC_NO_CONNECTION,
                                       &bdaddr);
                    break;
                }
#endif
            }
        }

        /* Move to next signal in list */
        sig_ptr = sig_ptr->next_ptr;

        /* Free this signal */
        SIGH_SignalDestroy(sig_free_ptr);
    }

    /* Reset list */
    *sig_list = NULL;
}

/*! \brief Empty specified signal queue of signals for specified channel.

    This function will empty the specified signal queue of signals
    from the specified channel, the signals will be destroyed. Note
    that we don't need to check for ECHO/GETINFO as they are never
    tied to CIDCBs.

    \param sig_list Pointer to signal list to empty, the list pointers will be updated
                    automatically.
    \param cid The channel ID for the signals to destroy.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void SIGH_SignalQueueEmptyWithCid(SIG_SIGNAL_T **sig_list,
                                  l2ca_cid_t cid)
{
    SIG_SIGNAL_T *sig_ptr;
    SIG_SIGNAL_T **sig_ptr_ptr;

    /* Flush the queue and stop any timers */
    for (sig_ptr_ptr = sig_list; (sig_ptr = *sig_ptr_ptr) != NULL;)
    {
        /* Check if this signal is for the specified CID - and that
         * this isn't PING or GETINFO. The latter are never tied to
         * CIDCB but still use the local_cid number (for the
         * req_ctx) */
        if((sig_ptr->local_cid == cid) &&
           (sig_ptr->signal_type != SIGNAL_ECHO_REQ) &&
           (sig_ptr->signal_type != SIGNAL_INFO_REQ))
        {
            /* Update pointer to next pointer */
            *sig_ptr_ptr = sig_ptr->next_ptr;

            /* Free this signal */
            SIGH_SignalDestroy(sig_ptr);
            sig_ptr = NULL;
        }
        else
        {
            sig_ptr_ptr = &sig_ptr->next_ptr;
        }
    }
}

#endif

/*! \brief Add signal to queue.

    This function will add a signal to the specified queue, the signal will be
    added at the end of the queue.

    \param sig_ptr_ptr Pointer to signal queue to add signal to.
    \param sig_add_ptr Pointer to signal to add to queue.
*/
static void SIGH_SignalQueueAdd(SIG_SIGNAL_T **sig_ptr_ptr, SIG_SIGNAL_T *sig_add_ptr)
{
    SIG_SIGNAL_T *sig_ptr;

    /* Find last signal in pending list */
    for(;
        (sig_ptr = *sig_ptr_ptr) != NULL;
        sig_ptr_ptr = &sig_ptr->next_ptr)
    {
        /* NOP */
    }

    /* Set next pointer in last entry to new signal */
    *sig_ptr_ptr = sig_add_ptr;

    /* Clear next pointer in next signal */
    sig_add_ptr->next_ptr = NULL;
}

/*! \brief Remove signal from queue with specified signal ID.

    This function will remove a signal with the specified ID from the specified
    signal queue, the signal will NOT be destroyed, instead a pointer will be
    returned to the caller.

    \param sig_list Pointer to signal list to remove signal from, the list
                    pointers will be updated automatically.
    \param signal_id The signal ID for the signal to remove.
    \return Pointer to signal, or NULL if no match was found.
*/
static SIG_SIGNAL_T *SIGH_SignalQueueRemoveWithSignalId(SIG_SIGNAL_T **sig_list, l2ca_identifier_t signal_id)
{
    SIG_SIGNAL_T *sig_ptr;
    SIG_SIGNAL_T **sig_ptr_ptr;

    for (sig_ptr_ptr = sig_list; (sig_ptr = *sig_ptr_ptr) != NULL; sig_ptr_ptr = &sig_ptr->next_ptr)
    {
        /* Check if we've found the signal we're looking for */
        if (sig_ptr->signal_id == signal_id)
        {
            /* Update pointer to next pointer */
            *sig_ptr_ptr = sig_ptr->next_ptr;

            /* Return with pointer to signal */
            return sig_ptr;
        }
    }

    return NULL;
}

/*! \brief Sends command reject signal.

    This function creates a command reject signal from the specified parameters
    and sends it.
*/
static void SIGH_SignalSendCommandReject(L2CAP_CHCB_T *chcb,
                                         l2ca_identifier_t signal_id,
                                         CsrUint16 reason,
                                         l2ca_cid_t local_cid,
                                         l2ca_cid_t remote_cid,
                                         l2ca_mtu_t mtu)
{
    CsrUint16 extra_par1 = 0;
    CsrUint16 extra_par2 = 0;
    CsrUint16 count = 1;

    switch (reason)
    {
        case SIGNAL_REJECT_MTU:
            extra_par1 = mtu;
            count = 2;
            break;

        case SIGNAL_REJECT_CID:
            extra_par1 = local_cid;
            extra_par2 = remote_cid;
            count = 3;
            break;
    }

    /* Create and send signal */
    SIGH_SignalSend(chcb,
            SIG_SignalCreate(
                local_cid,
                (CsrUint16)(SIGNAL_COMMAND_REJECT_MIN_LENGTH + 2*(count - 1)),
                (CsrUint8)SIGNAL_COMMAND_REJECT,
                signal_id,
                count,
                reason,
                extra_par1,
                extra_par2));
}

/*! \brief Low energy connection parameter update response

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_ULP
static CsrUint16 SIGH_Handle_SIGNAL_CONN_PARAM_UPDATE_REQ(L2CAP_CHCB_T *chcb,
        L2CAP_SIGNAL_INFO_T *sig)
{
    CsrUint16 interval_min, interval_max, slave_latency, timeout;
    TYPED_BD_ADDR_T remote_bd_addrt;

    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 4, UNDEFINED, 0, UNDEFINED, 0),
               &interval_min,
               &interval_max,
               &slave_latency,
               &timeout);

    if ((CH_GET_ACL(chcb)->flags & DM_ACL_ROLE_SLAVE) != 0)
    {
        SIGH_SignalSendCommandReject(chcb,
                                     sig->id,
                                     SIGNAL_REJECT_UNKNOWN, /* reason */
                                     L2CA_CID_INVALID, /* local cid */
                                     L2CA_CID_INVALID, /* remote cid */
                                     0); /* mtu, not used */
        return sig->length_with_header;
    }

    tbdaddr_copy_from_bd_addr(&remote_bd_addrt,CH_GET_TBDADDR(chcb)->type,
                              &(CH_GET_TBDADDR(chcb)->addr));

    L2CA_ConnUpdateInd( sig->id,
                        remote_bd_addrt,
                        interval_min,
                        interval_max,
                        slave_latency,
                        timeout);

    return sig->length_with_header;
}
#endif

/*! \brief Low energy connection parameter update request

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_ULP
static CsrUint16 SIGH_Handle_SIGNAL_CONN_PARAM_UPDATE_RES(L2CAP_CHCB_T *chcb,
        L2CAP_SIGNAL_INFO_T *sig)
{
    SIG_SIGNAL_T *sig_ptr;

    CsrUint16 result = read_uint16(&sig->data);

    /* Remove matching request signal from pending queue */
    sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                 sig->id);
    if (sig_ptr != NULL)
    {
        L2CA_ConnUpdateCfm(sig_ptr->p_handle, result == 0 ?
                           L2CA_MISC_SUCCESS : L2CA_MISC_FAILED,
                           CH_GET_TBDADDR(chcb));

        /* Stop the timer */
        SIGH_SignalDestroy(sig_ptr);
    }

    return sig->length_with_header;
}
#endif

/*! \brief Setup a txqe for signal transmission

    This function is a pre-send callback. It's called from channel
    transmit loop when a "signal callback frametype" is found. This
    function prepares the txqe pointer for transmission by setting up
    the header, mblk and all the other stuff that goes into the txqe.

    This function will attempt to also send/prepare any other pending
    signals upto the maximum signal MTU.

    If signal queue is not empty when we are about to return from this
    function, we re-schedule a new callback in the transmitter.

    The txqe must have enough header-space to contain a standard CID
    header.

    \param chcb Pointer to CHCB instance.
    \param txqe Pointer to the tx element to fill out
*/
void SIGH_SignalPresend(L2CAP_CHCB_T *chcb,
                        TXQE_T *txqe)
{
    SIG_SIGNAL_T *signal_queue = chcb->signal_queue;
    CsrMblkSize mblk_chain_length = 0;
    CsrMblk *mblk_chain_ptr = NULL;

    /* This has effectively removed the scheduled callback */
    chcb->signal_scheduled = FALSE;

    /* Handle sending multiple signals in one L2CAP packet */
    while(signal_queue)
    {
        SIG_SIGNAL_T *sig_ptr;
        CsrUint8 *signal_data;
        CsrMblk *mblk_ptr;

        /* Remove signal from queue and prepare for next */
        sig_ptr = signal_queue;
        signal_queue = sig_ptr->next_ptr;
        mblk_ptr = sig_ptr->signal;
        signal_data = CsrMblkMap(mblk_ptr, 0, CsrMblkGetLength(mblk_ptr));

        txqe->cid = sig_ptr->local_cid;

        switch(sig_ptr->signal_type)
        {
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
            /* Signal is a request */
            case SIGNAL_CONNECT_REQ:
                /* -fallthrough */
            case SIGNAL_CONFIG_REQ:
                /* -fallthrough */
            case SIGNAL_DISCONNECT_REQ:
                /* -fallthrough */
            case SIGNAL_ECHO_REQ:
                /* -fallthrough */
            case SIGNAL_INFO_REQ:
                /* -fallthrough */
#endif
#ifdef INSTALL_AMP_SUPPORT
            case SIGNAL_CREATE_CHANNEL_REQ:
                /* -fallthrough */
            case SIGNAL_MOVE_CHANNEL_REQ:
                /* -fallthrough */
            case SIGNAL_MOVE_CONFIRM_REQ:
                /* -fallthrough */
#endif
#ifdef INSTALL_ULP
            case SIGNAL_CONN_PARAM_UPDATE_REQ:
                /* -fallthrough */
#endif
            {
                CsrTime  timeout;

                /* Check if this is the first transmission of this signal */
                if (signal_data[1] == 0x00)
                {
                    /* Store the signal ID within the block & modify outgoing signal */
                    sig_ptr->signal_id = SIGH_SignalIdObtain(chcb);
                    signal_data[1] = sig_ptr->signal_id;
                }

                /* Add signal to pending queue, in case of retransmission, and
                   increase ref count to allow it to be destroyed in both the
                   pending queue and at HCI. */
                SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
#ifdef BUILD_FOR_HOST
                CsrMblkIncRefcount(mblk_ptr, 1);
#else
                /* This only fails if this is the 8th time we're sending this
                   signal and the previous 7 are stuck in L2CAP's TX queue.
                   Since we never send more than 2 copies of a signal, this
                   should never happen. So we will panic if it does. */
                if (!CsrMblkIncRefcount(mblk_ptr, 1))
                    BLUESTACK_PANIC(
                                    PANIC_L2CAP_MBLK_REFCOUNT_EXHAUSTED);
#endif

                /* Start the RTX timer - double the timer for every retry */
                timeout = CSR_BT_L2CAP_RTX_TIMEOUT << (L2CAP_RTX_RETRIES - sig_ptr->rtx_timer_count);
                timer_start(&sig_ptr->timer_id, timeout, SIGH_RtxTimeout, sig_ptr->signal_id, chcb);
            }
            break;

            default:
            {
                /* Free the signal block, data block now owned by MBLK */
                CsrPmemFree(sig_ptr);
            }
            break;
        }

        /* Add MBLK to end of list */
        mblk_chain_ptr = CsrMblkAddTail(mblk_ptr, mblk_chain_ptr);
        mblk_chain_length = CsrMblkGetLength(mblk_chain_ptr);

        CsrMblkUnmap(mblk_ptr, signal_data);
        /* Do not send more than one signal per C frame.

           When multiple signals in a single C frame is supported, we will
           need the following code here:



            * Break out of loop if next signal will make us exceed the
            * MTU. Note that we will actually be able to send a single
            * signal that _does_ exceed the MTU (think echo requests)
            if(signal_queue && mblk_chain_length +
                CsrMblkGetLength(signal_queue->signal) > CH_GET_SIGNAL_MTU(chcb))
            {
                break;
            }


            For now we just break unconditionally.

            See B-81986 and B-56485.
        */
        break;
    }

    chcb->signal_queue = signal_queue;

    /* Augment the txqe element. Transmitter (who called us) will the
     * continue to send it */
    if(mblk_chain_ptr != NULL)
    {
        txqe->mblk = mblk_chain_ptr;
        txqe->flush = FLUSH_INFINITE_TIMEOUT;
        CH_DataAddHeader(txqe,
                         (CsrUint16)CH_GET_SIGNAL_CID(chcb),
                         mblk_chain_length, NULL, 0);
    }

    /* Poke the transmitter loop to shovel data through if possible */
    if(signal_queue != NULL)
    {
        CH_DataTxSignalCallback(chcb);
    }
}

/*! \brief Send L2CAP signal

    Request from a CID to send signal. Stuff it onto the queue and
    send it if possible.

    \param chcb Pointer to the CHCB instance
    \param sig_ptr Pointer to the signal to send
*/
void SIGH_SignalSend(L2CAP_CHCB_T *chcb, SIG_SIGNAL_T *sig_ptr)
{
    /* Add signal to queue, we'll send it later */
    SIGH_SignalQueueAdd(&chcb->signal_queue, sig_ptr);

    /* Poke the transmitter loop to shovel data through if possible */
    CH_DataTxSignalCallback(chcb);
}

/*! \brief Receive signal(s).

    Process any received signals
    This function only gets called when we have received a complete signal

    \param chcb Pointer to CHCB instance.
    \param signal_ptr Pointer to received signal payload.
    \param signal_size Size of signal payload.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void SIGH_SignalReceive(L2CAP_CHCB_T *chcb,
                        CsrMblk **mblk,
                        CsrUint16 frame_size)
{
    CsrUint16 offset;      /* total number of bytes consumed so far */
    CsrUint8 *map = NULL;  /* mapped, linear signal data */
    CsrUint16 consumed;    /* bytes consumed in any particular pass */
    L2CAP_SIGNAL_INFO_T sig;

#ifndef BUILD_FOR_HOST
    patch_fn(l2cap_shared_patchpoint);
#endif

    /* Keep processing the signals until:
     * 1) The last handle_SIGNAL_ function returned 0. These
     *    functions now return 0 if they encounter a malformed signal
     *    and we should not try to process any more signals after
     *    that happens.
     * 2) There's not enough C Frame left to contain a signal header
     * 3) There's not enough C Frame left to contain the quoted signal length
     *
     * Note that the "this_len" variable will be set to 0 in case the
     * actual signal handler wasn't able to decode the signal.
     *
     * Also note that we must be able to handle the cornercase where
     * there is no actual signal payload like echo-requests without
     * data.
     *
     * If nothing bad happens then we should exit the while loop with
     * offset == frame_size
     */

#ifndef BUILD_FOR_HOST
    /* Because signals may hang around for some time, and due to the
     * fact that we can only have one write-buffer at a time, coalesce
     * the signal data */
    (void)CsrMblkCoalesceToPmalloc(mblk);
#endif

    sig.id = 0;

    for (offset = 0;
         offset + L2CAP_SIZEOF_SIGNAL_HEADER <= frame_size;
         offset += consumed)
    {
        /* Map in buffer */
        if ((map = CsrMblkMap(*mblk, 0, frame_size)) == NULL)
        {
            break;
        }

        /* Point to the signal we're considering. */
        sig.data = map + offset;

        /* Read header. We know there's enough there for that. */
        read_uints(&sig.data,
                   URW_FORMAT(CsrUint8, 2, CsrUint16, 1, UNDEFINED, 0),
                   &sig.type,
                   &sig.id,
                   &sig.length);

        if (CHECK_SIGNAL_INVALID(sig.type))
        {
            sig.type = SIGNAL_INVALID;
        }

        /* Check signal fits within block and signal length is at
         * least big enough for this signal. If signal length is too
         * big or too small then frame is invalid, so silently ignore
         * all of it. */
        if((sig.length > (frame_size - offset - L2CAP_SIZEOF_SIGNAL_HEADER)) ||
           (sig.length < signal_min_length[sig.type]))
        {
            break;
        }

        /* Pass signal into handler; returns number of bytes consumed */
        sig.length_with_header = sig.length + L2CAP_SIZEOF_SIGNAL_HEADER;
        consumed = signal_handle[sig.type](chcb, &sig);

        /* Break out if we didn't read anything (error) */
        if (consumed == 0)
        {
            break;
        }

        /* Unmap signal now so next iteration can re-map it. This
         * works around some buffer consistency issues */
        CsrMblkUnmap(*mblk, map);
        map = NULL;

        /* Reset signal identifier so if we break out, we won't
         * complain about a correctly formatted one */
        sig.id = 0;
    }

    /* We need to check if things are unmapped due to the breaks */
    if(map != NULL)
    {
        CsrMblkUnmap(*mblk, map);
    }

    /* If the offset doesn't match the end and the sig_id is non-zero
     * something fishy is going on, but we can still complain */
    if(frame_size != offset)
    {
        if(sig.id != 0)
        {
            SIGH_SignalSendCommandReject(chcb,
                                         sig.id,
                                         SIGNAL_REJECT_UNKNOWN, /* reason */
                                         L2CA_CID_INVALID, /* local cid */
                                         L2CA_CID_INVALID, /* remote cid */
                                         0); /* mtu, not used */
        }
    }
}
#endif

/*! \brief Bluetooth Low Energy signal reception

    The BLE only uses a subset of the signals and use CID 5 instead of
    the normal L2CAP signalling channel
*/
#ifdef INSTALL_ULP
void SIGH_LeSignalReceive(L2CAP_CHCB_T *chcb, CsrMblk **mblk, CsrUint16 signal_size)
{
    CsrUint16 consumed;
    CsrUint8 *map;
    L2CAP_SIGNAL_INFO_T sig;

#ifndef BUILD_FOR_HOST
    /* Because signals may hang around for some time, and due to the
     * fact that we can only have one write-buffer at a time, coalesce
     * the signal data */
    (void)CsrMblkCoalesceToPmalloc(mblk);
#endif

    /* MTU sanity */
    if(signal_size > L2CAP_SIGNAL_BLE_MTU_MAX)
    {
        SIGH_SignalMtuExceeded(chcb, *mblk, signal_size);
        return;
    }

    map = CsrMblkMap(*mblk, 0, signal_size);
    if(map == NULL)
    {
        return;
    }

    /* Decode signal - we know it has enough space for the header */
    sig.data = map;
    read_uints(&sig.data,
               URW_FORMAT(CsrUint8, 2, CsrUint16, 1, UNDEFINED, 0),
               &sig.type,
               &sig.id,
               &sig.length);
    sig.length_with_header = sig.length + L2CAP_SIZEOF_SIGNAL_HEADER;

    /* Make sure signal length matches value in header */
    if(sig.length_with_header != signal_size)
    {
        CsrMblkUnmap(*mblk, map);
        return;
    }

    consumed = 0;
    if(sig.type == SIGNAL_COMMAND_REJECT &&
        sig.length >= SIGNAL_COMMAND_REJECT_MIN_LENGTH)
    {
        /* Normal L2CAP signal supported on BLE signalling channel */
        consumed = SIGH_Handle_SIGNAL_COMMAND_REJECT(chcb, &sig);
    }
    else if(sig.type == SIGNAL_CONN_PARAM_UPDATE_REQ &&
            sig.length == SIGNAL_CONN_PARAM_UPDATE_REQ_MIN_LENGTH)
    {
        /* BLE specific signal */
        consumed = SIGH_Handle_SIGNAL_CONN_PARAM_UPDATE_REQ(chcb, &sig);
    }
    else if(sig.type == SIGNAL_CONN_PARAM_UPDATE_RES &&
            sig.length == SIGNAL_CONN_PARAM_UPDATE_RES_MIN_LENGTH)
    {
        /* BLE specific signals */
        consumed = SIGH_Handle_SIGNAL_CONN_PARAM_UPDATE_RES(chcb, &sig);
    }

    if (consumed != signal_size)
    {
        SIGH_SignalSendCommandReject(chcb,
                                     sig.id,
                                     SIGNAL_REJECT_UNKNOWN, /* reason */
                                     L2CA_CID_INVALID, /* local cid */
                                     L2CA_CID_INVALID, /* remote cid */
                                     0); /* mtu, not used */
    }

    CsrMblkUnmap(*mblk, map);
}
#endif

/*! \brief Handle signal that exceeded the MTU_sig

    We need to send back the signal identifier even for signals that
    exceed the MTU.  This function extracts that number and sends back
    the command-reject response with the MTU-exceed-reason.
*/
void SIGH_SignalMtuExceeded(L2CAP_CHCB_T *chcb,
                            CsrMblk *mblk,
                            CsrUint16 signal_size)
{
    CsrUint8 *map;
    CsrUint8 id;
    PARAM_UNUSED(signal_size);

    /* Map in signal to extract the identifier */
    map = CsrMblkMap(mblk, 0, L2CAP_SIZEOF_SIGNAL_HEADER);
    if(map != NULL)
    {
        id = map[1];
        CsrMblkUnmap(mblk, map);
    }
    else
    {
        id = 0;
    }

    SIGH_SignalSendCommandReject(chcb,
                                 id,
                                 SIGNAL_REJECT_MTU,
                                 L2CA_CID_INVALID,
                                 L2CA_CID_INVALID,
                                 L2CAP_SIGNAL_INCOMING_MTU_MAX);
}

/*! \brief Command reject signal.

    This function handles receiving a command reject signal, it cancels
    any matching pending signal.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_COMMAND_REJECT(L2CAP_CHCB_T *chcb,
                                                  L2CAP_SIGNAL_INFO_T *sig)
{
    CIDCB_T *cidcb = NULL;
    CsrUint16 sig_size = sig->length_with_header;
    SIG_SIGNAL_T *sig_ptr;

    /* Remove signal from pending queue and try to obtain the CIDCB
     * for the signal */
    if((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                     sig->id)) != NULL)
    {
        timer_cancel(&sig_ptr->timer_id);
        cidcb = CIDME_GetCidcbWithHandle(chcb, sig_ptr->local_cid);
    }

    /* Switch by reject reason. */
    switch (read_uint16(&sig->data))
    {
        case SIGNAL_REJECT_UNKNOWN:
            {
                if (sig_size != L2CAP_SIGNAL_SIZEOF_REJECT)
                {
                    /* Error in command */
                    sig_size = 0;
                    break;
                }

                /* We can't really do anything about this except
                 * report it to the upper layer */
                if (cidcb != NULL)
                {
                    L2CA_UnknownSignalInd(cidcb, sig->id);
                }
            }
            break;

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
        case SIGNAL_REJECT_MTU:
            {
                l2ca_mtu_t mtu;

                if (sig_size != L2CAP_SIGNAL_SIZEOF_INV_MTU)
                {
                    /* Error in command */
                    sig_size = 0;
                    break;
                }

                if (cidcb != NULL)
                {
                    read_uints(&sig->data,
                               URW_FORMAT(CsrUint16, 1, UNDEFINED,0,UNDEFINED,0),
                               &mtu);

                    L2CA_MtuViolationInd(cidcb, sig->id, mtu);
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
                    /* In legacy L2CAP mode, we use a signal MTU of 48, in
                     * which case this violation will never
                     * occur. However, with AMP support enabled, this may
                     * happen and we try to do a MTU backoff */
                    chcb->signal_mtu = mtu;
#endif
                }
            }
            break;

        case SIGNAL_REJECT_CID:
            {
                l2ca_cid_t cid_source;

                if (sig_size != L2CAP_SIGNAL_SIZEOF_INV_CID)
                {
                    /* Error in command */
                    sig_size = 0;
                    break;
                }

                /* Read local and remote CIDs. We ignore the remote CID. */
                read_uints(&sig->data,
                           URW_FORMAT(CsrUint16, 2, UNDEFINED,0,UNDEFINED,0),
                           NULL,
                           &cid_source);

                /* If the source CID matches, crank the state machine
                   * in order to tear down the invalid CIDCB */
                if (cidcb != NULL &&
                    sig_ptr != NULL &&
                    sig_ptr->local_cid == cid_source)
                {
                    CID_InvalidCid(cidcb);
                }
            }
            break;
#endif
        default:
            break;
    }

    if (sig_ptr != NULL)
    {
        if (sig_size == 0)
        {
            /* Error in the reject command: Resend the signal */
            SIGH_SignalSend(chcb, sig_ptr);
        }
        else
        {
            /* Signal was handled and rejected, so free it */
            SIGH_SignalDestroy(sig_ptr);
        }
    }

    /* Return indicating signal handled */
    return sig_size;
}

/*! \brief Connect request signal.

    This function handles receiving a connect request signal, first of all a check
    is made to see if the CHCB instance is in the correct state, then the PSM is
    checked to make sure something has registered the required PSM.  If these checks
    pass then the CID state machine is called, otherwise a rejection is sent.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_CONNECT_REQ(L2CAP_CHCB_T *chcb,
                                               L2CAP_SIGNAL_INFO_T *sig)
{
    /* We've already made sure that we're connected way before here. */
    CID_CONNECT_REQ_T sig_args;
    CsrUint16 result;

    /* Check if this is a duplicate */
    if(SIGH_DuplicateDetect(chcb, sig))
    {
        return sig->length_with_header;
    }
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    sig_args.signal_id  = sig->id;
    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
               &sig_args.psm,
               &sig_args.source_cid);

    if ((result = MCB_ConnectInd(chcb, &sig_args)) != L2CA_CONNECT_SUCCESS)
    {
        SIG_SIGNAL_T *sig_ptr;

        /* Inform other end of rejection with signal */
        sig_ptr = SIG_SignalCreateConnectRsp(L2CA_CID_INVALID,
                                             sig_args.source_cid,
                                             result,
                                             SIGNAL_STATUS_NONE,
                                             sig_args.signal_id);
        SIGH_SignalSend(chcb, sig_ptr);
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Connect response signal

    This function handles a connect response signal, it removes the
    pending request signal, stops the signal timer and runs the CID state machine.
    If the connect status is 'pending' then the ERTX timer is started.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_CONNECT_RES(L2CAP_CHCB_T *chcb,
                                               L2CAP_SIGNAL_INFO_T *sig)
{
    l2ca_cid_t cid_dest;
    l2ca_cid_t cid_source;
    CsrUint16 result; /* Used in read_uints() so can't be an enum type */
    CIDCB_T *cidcb;
    SIG_SIGNAL_T *sig_ptr;

    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 4, UNDEFINED, 0, UNDEFINED, 0),
               &cid_dest,
               &cid_source,
               &result,
               NULL);       /* 'status' is currently ignored. */

    /* Remove matching request signal from pending queue */
    if ((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                      sig->id)) != NULL)
    {
        /* Find CIDCB using the locally stored CID from the signal block */
        cidcb = CIDME_GetCidcbWithHandle(chcb, sig_ptr->local_cid);

        /* The spec specifically says that we can not assume that the
         * CIDs are set at this point if the result code is a
         * reject. However, if pending or success the CIDs must be
         * valid. */
        if((cidcb != NULL) &&
           ( ((result <= L2CA_CONNECT_PENDING) && (sig_ptr->local_cid == cid_source)) ||
             (result > L2CA_CONNECT_PENDING)) )
        {
            timer_cancel(&sig_ptr->timer_id);

            /* Crank the FSM */
            CID_ConnectCfm(cidcb,
                           (l2ca_cid_t)(result <= L2CA_CONNECT_PENDING ? cid_dest : L2CA_CID_INVALID),
                           (l2ca_conn_result_t)result);

            /* Check if connection is pending */
            if (result == L2CA_CONNECT_PENDING)
            {
                /* Add signal back on pending queue */
                SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);

                /* Start the ERTX timer */
                timer_start(&sig_ptr->timer_id,
                            CSR_BT_L2CAP_ERTX_TIMEOUT,
                            SIGH_ErtxTimeout,
                            sig_ptr->signal_id,
                            chcb);
            }
            else
            {
                /* Free the signal */
                SIGH_SignalDestroy(sig_ptr);
                sig_ptr = NULL;
            }
        }
        else
        {
            if((result <= L2CA_CONNECT_PENDING) && (sig_ptr->local_cid != cid_source))
            {
                /* To be ignored. Add signal back on pending queue */
                SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
            }
            else
            {
                /* Free the signal */
                SIGH_SignalDestroy(sig_ptr);
            }
        }
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

#endif
/*! \brief Configure request signal.

    This function handles a configure request signal, the appropriate CID state
    machine is called, or if no CID match then a command reject signal is sent.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_CONFIG_REQ(L2CAP_CHCB_T *chcb,
                                              L2CAP_SIGNAL_INFO_T *sig)
{
    CID_CONFIG_REQ_T sig_args;
    CIDCB_T *cidcb;

    /* Check if this is a duplicate */
    if(SIGH_DuplicateDetect(chcb, sig))
    {
        return sig->length_with_header;
    }
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    sig_args.signal_id  = sig->id;
    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
               &sig_args.dest_cid,
               &sig_args.flags);

    sig_args.options = sig->data;
    sig_args.options_size = sig->length - SIGNAL_CONFIG_REQ_MIN_LENGTH;

    /* Verify that the option length fields are valid */
    if(!SIGH_ValidateConfigBuffer(sig_args.options_size, sig_args.options))
    {
        /* Options error, reject rest of signal */
        return 0;
    }

    /* Get CIDCB instance */
    cidcb = CIDME_GetCidcbWithHandle(chcb, sig_args.dest_cid);

    /* If the CIDCB exists, decode the on-air signal structure and
     * pass it into the FSM */
    if(cidcb != NULL)
    {
        /* Shake the FSM */
        CID_ConfigInd(cidcb, &sig_args);
    }
    else
    {
        /* Received Config Request Signal for invalid CID */
        SIGH_SignalSendCommandReject(chcb,
                                     sig_args.signal_id,
                                     SIGNAL_REJECT_CID,
                                     L2CA_CID_INVALID,
                                     sig_args.dest_cid,
                                     0);
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Configure response signal.

    This function handles a configure response signal, it removes the
    pending request signal, stops the signal timer and runs the CID state machine.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_CONFIG_RES(L2CAP_CHCB_T *chcb,
                                              L2CAP_SIGNAL_INFO_T *sig)
{
    CID_CONFIG_RES_T sig_args;
    CIDCB_T *cidcb;
    SIG_SIGNAL_T *sig_ptr;

    sig_args.signal_id = sig->id;
    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 3, UNDEFINED, 0, UNDEFINED, 0),
               &sig_args.source_cid,
               &sig_args.flags,
               &sig_args.result);

    sig_args.options = sig->data;
    sig_args.options_size = sig->length - SIGNAL_CONFIG_RES_MIN_LENGTH;

    /* Verify that the option length fields are valid */
    if (!SIGH_ValidateConfigBuffer(sig_args.options_size, sig_args.options))
    {
        /* Options error, reject rest of signal */
        return 0;
    }

    /* Remove matching request signal from pending queue */
    if ((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                    sig_args.signal_id)) != NULL)
    {
        /* Check CID in signal matches CID in signal block */
        if (sig_ptr->local_cid == sig_args.source_cid)
        {
            /* Stop the timer */
            timer_cancel(&sig_ptr->timer_id);

            /* Get the CIDCB instance */
            cidcb = CIDME_GetCidcbWithHandle(chcb, sig_args.source_cid);
            if (cidcb != NULL)
            {
                /* Shake the FSM */
                CID_ConfigCfm(cidcb, &sig_args);
            }

#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
            /* AMP lockstep uses the PENDING result code. That means
             * we should use an ERTX timer */
            if(sig_args.result == L2CA_CONFIG_PENDING)
            {
                /* Add signal back on pending queue */
                SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);

                /* Start the ERTX timer */
                timer_start(&sig_ptr->timer_id,
                            CSR_BT_L2CAP_ERTX_TIMEOUT,
                            SIGH_ErtxTimeout,
                            sig_ptr->signal_id,
                            chcb);
            }
            else
#endif
            {
                /* Free the signal */
                SIGH_SignalDestroy(sig_ptr);
            }
        }
        else
        {
            /* CID mismatch, ignore signal block. Add signal back on pending queue */
            SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
        }
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Disconnect request signal.

    This function handles a disconnect request signal, the appropriate CID state machine
    is called or if no CID match is found a command reject signal is sent.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_DISCONNECT_REQ(L2CAP_CHCB_T *chcb,
                                                  L2CAP_SIGNAL_INFO_T *sig)
{
    CID_DISCONNECT_REQ_T sig_args;
    CIDCB_T *cidcb;

    /* Check if this is a duplicate */
    if(SIGH_DuplicateDetect(chcb, sig))
    {
        return sig->length_with_header;
    }
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    sig_args.signal_id  = sig->id;
    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
               &sig_args.dest_cid,
               &sig_args.source_cid);

    /* Attempt to get the CIDCB instance, also check remote CID matches CID in signal block */
    if ((cidcb = CIDME_GetCidcbWithHandle(chcb, sig_args.dest_cid)) != NULL
            && cidcb->remote_cid == sig_args.source_cid)
    {
        CID_DisconnectInd(cidcb, &sig_args);
    }
    else
    {
        /* Received Config Request Signal for invalid CID */
        SIGH_SignalSendCommandReject(chcb,
                                     sig_args.signal_id,
                                     SIGNAL_REJECT_CID,
                                     sig_args.dest_cid,
                                     sig_args.source_cid,
                                     0);
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Disconnect response signal.

    This function handles a disconnect response signal, it removes the
    pending request signal, stops the signal timer and runs the CID state machine.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_DISCONNECT_RES(L2CAP_CHCB_T *chcb,
                                                  L2CAP_SIGNAL_INFO_T *sig)
{
    CIDCB_T *cidcb;
    l2ca_cid_t cid_dest;
    l2ca_cid_t cid_source;
    SIG_SIGNAL_T *sig_ptr;

    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
               &cid_dest,
               &cid_source);

    /* Remove matching request signal from pending queue */
    if ((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                      sig->id)) != NULL)
    {
        /* Check CID in signal matches CID in signal block */
        if (sig_ptr->local_cid == cid_source)
        {
            /* Get the CIDCB instance also check remote CID matches CID in signal block */
            if ((cidcb = CIDME_GetCidcbWithHandle(chcb, cid_source)) != NULL
                    && cidcb->remote_cid == cid_dest)
            {
                CID_DisconnectCfm(cidcb);
            }

            /* Free the signal (also stops timer) */
            SIGH_SignalDestroy(sig_ptr);
        }
        else
        {
            /* CID mismatch, ignore signal block. Add signal back on pending queue */
            SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
        }
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Echo response signal.

    This function handles an echo response signal, it removes the pending
    request signal, stops the signal timer and posts in the echo response
    data to the higher layer.

    The connection is also checked if it is idle and can be released.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_ECHO_RES(L2CAP_CHCB_T *chcb,
                                            L2CAP_SIGNAL_INFO_T *sig)
{
    SIG_SIGNAL_T *sig_ptr;

    /* Remove matching request signal from pending queue */
    if ((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                      sig->id)) != NULL)
    {
        CsrUint8 *prim_data;
        CsrUint16 prim_data_size;

        /* Copy echo response data */
        prim_data_size = sig->length - SIGNAL_ECHO_RES_MIN_LENGTH;
        if ((prim_data = xpcopy(sig->data, prim_data_size)) == NULL)
            prim_data_size = 0;

        /* Send ping confirm to higher layer */
        L2CA_PingCfm(sig_ptr->p_handle, CH_GET_BD_ADDR(chcb),
                     L2CA_MISC_SUCCESS, prim_data, prim_data_size,
                     sig_ptr->local_cid); /* local_cid stores the req_ctx */

        /* Free the signal (also stops timer) */
        SIGH_SignalDestroy(sig_ptr);

        /* Remove lock from ACL. */
        CH_StandardClose(chcb);
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Echo request signal.

    This function handles an echo request (ping) signal, it creates and sends
    an echo response.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_ECHO_REQ(L2CAP_CHCB_T *chcb,
                                            L2CAP_SIGNAL_INFO_T *sig)
{
    SIG_SIGNAL_T *sig_ptr;
    CsrUint16 data_size;

    /* Make sure we only reject duplicate echos */
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    /* Mirror the data received in the request into the response
     * signal. Note that the signal create function will memcpy the
     * data */
    data_size = sig->length - SIGNAL_ECHO_REQ_MIN_LENGTH;

    /* Clip data - no point panicking over an echo response */
    if (data_size > L2CAP_ECHO_RESPONSE_MAX_DATA_SIZE)
        data_size = L2CAP_ECHO_RESPONSE_MAX_DATA_SIZE;

    sig_ptr = SIG_SignalCreateEchoRsp(sig->id, sig->data, data_size);
    SIGH_SignalSend(chcb, sig_ptr);

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Info response signal.

    This function handles a info response signal, it removes the pending
    request signal, stops the signal timer and posts in the info response
    data to the higher layer.

    The connection is also checked if it is idle and can be released.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_INFO_RES(L2CAP_CHCB_T *chcb,
                                            L2CAP_SIGNAL_INFO_T *sig)
{
    SIG_SIGNAL_T *sig_ptr;

    /* Remove matching request signal from pending queue */
    if ((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                      sig->id)) != NULL)
    {
        CsrUint8 *prim_data;
        CsrUint16 prim_data_size;
        CsrUint16 info_type;
        CsrUint16 result; /* Used in read_uints() so can't be an enum type */

        /* Extract info type and the result code. */
        read_uints(&sig->data,
                   URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
                   &info_type,
                   &result);

        /* Copy info response data */
        prim_data_size = sig->length - SIGNAL_INFO_RES_MIN_LENGTH;
        if ((prim_data = xpcopy(sig->data, prim_data_size)) == NULL)
        {
            /* Cannot pass on data, so result may as well be Not Supported */
            prim_data_size = 0;
            result = L2CA_GETINFO_NOT_SUPPORTED;
        }

        /* Send get info confirm to higher layer */
        L2CA_GetInfoCfm(sig_ptr->p_handle,
                        CH_GET_BD_ADDR(chcb),
                        (l2ca_info_result_t)result,
                        info_type,
                        prim_data,
                        prim_data_size,
                        sig_ptr->local_cid); /* local_cid stores the req_ctx */

        /* Free the signal (also stops timer) */
        SIGH_SignalDestroy(sig_ptr);

        /* Remove lock from ACL. Longer timeout as probability of more. */
        CH_Close(chcb, HCI_ERROR_OETC_USER, ACL_IDLE_TIMEOUT_AT_LEAST_SHORT);
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief Info request signal.

    This function handle a info request signal, it creates and sends a info
    response signal.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_INFO_REQ(L2CAP_CHCB_T *chcb,
                                            L2CAP_SIGNAL_INFO_T *sig)
{
    SIG_SIGNAL_T *sig_ptr;
    CsrUint16 info_type;

    /* Make sure we only reject duplicate get-info */
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    /* Extract the info_type requested */
    info_type = read_uint16(&sig->data);

    /* Create info response signal */
    sig_ptr = SIG_SignalCreateGetInfoRsp(sig->id, info_type);
    SIGH_SignalSend(chcb, sig_ptr);

    /* Return indicating signal handled */
    return sig->length_with_header;
}
#endif

/*! \brief Invalid signal.

    This function handles invalid or unknown signals.  Sends a command reject
    signal in response.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
static CsrUint16 SIGH_Handle_SIGNAL_INVALID(L2CAP_CHCB_T *chcb,
                                           L2CAP_SIGNAL_INFO_T *sig)
{
    /* Send command reject signal */
    SIGH_SignalSendCommandReject(chcb,
                                 sig->id,
                                 SIGNAL_REJECT_UNKNOWN,
                                 L2CA_CID_INVALID,
                                 L2CA_CID_INVALID,
                                 0);

    /* Return indicating signal handled */
    return sig->length_with_header;
}

/*! \brief AMP create channel request

    Peer has sent an AMP create channel request.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_AMP_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_CREATE_CHANNEL_REQ(L2CAP_CHCB_T *chcb,
                                                      L2CAP_SIGNAL_INFO_T *sig)
{
    CID_CREATE_CHAN_REQ_T sig_args;
    CsrUint16 result;

    /* Check if this is a duplicate */
    if(SIGH_DuplicateDetect(chcb, sig))
    {
        return sig->length_with_header;
    }
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    sig_args.signal_id = sig->id;
    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 2, CsrUint8, 1, UNDEFINED, 0),
               &sig_args.psm,
               &sig_args.source_cid,
               &sig_args.control_id);

    if ((result = MCB_AmpCreateInd(chcb, &sig_args)) != L2CA_CONNECT_SUCCESS)
    {
        /* PSM not registered - reject */
        SIG_SIGNAL_T *sig_ptr;
        sig_ptr = SIG_SignalCreateChannelRsp(L2CA_CID_INVALID,
                                             sig_args.source_cid,
                                             result,
                                             SIGNAL_STATUS_NONE,
                                             sig_args.signal_id);
        SIGH_SignalSend(chcb, sig_ptr);
    }

    /* Return indicating signal was handled */
    return sig->length_with_header;
}
#endif

/*! \brief AMP create channel response

    Peer has sent an AMP create channel response

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_AMP_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_CREATE_CHANNEL_RES(L2CAP_CHCB_T *chcb,
                                                      L2CAP_SIGNAL_INFO_T *sig)
{
    l2ca_cid_t cid_dest;
    l2ca_cid_t cid_source;
    CsrUint16 result; /* Used in read_uints() so can't be an enum type */
    CIDCB_T *cidcb;
    SIG_SIGNAL_T *sig_ptr;

    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 4, UNDEFINED, 0, UNDEFINED, 0),
               &cid_dest,
               &cid_source,
               &result,
               NULL); /* 'status' is currently ignored */

    /* Find and remove matching request signal */
    if((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                     sig->id)) != NULL)
    {
        /* The spec specifically says that we can not assume that the
         * CIDs are set at this point if the result code is a
         * reject. However, if pending or success the CIDs must be
         * valid. */
        if((cidcb = CIDME_GetCidcbWithHandle(chcb, sig_ptr->local_cid)) != NULL
                && (result > L2CA_CONNECT_PENDING
                    || sig_ptr->local_cid == cid_source))
        {
            timer_cancel(&sig_ptr->timer_id);

            /* Crank FSM */
            CID_AmpCreateCfm(cidcb,
                             (l2ca_cid_t)(result <= L2CA_CONNECT_PENDING
                                 ? cid_dest : L2CA_CID_INVALID),
                             (l2ca_conn_result_t)result);

            /* If result is pending, put request back on pending
             * queue and start ERTX timer */
            if(result == L2CA_CONNECT_PENDING)
            {
                SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
                timer_start(&sig_ptr->timer_id,
                            CSR_BT_L2CAP_ERTX_TIMEOUT,
                            SIGH_ErtxTimeout,
                            sig_ptr->signal_id,
                            chcb);
            }
            else
            {
                /* Free request signal */
                SIGH_SignalDestroy(sig_ptr);
            }
        }
        else
        {
            if((result <= L2CA_CONNECT_PENDING) && (sig_ptr->local_cid != cid_source))
            {
                /* CID mismatch, ignore signal block. Add signal back on pending queue */
                SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
            }
            else
            {
                /* Free request signal */
                SIGH_SignalDestroy(sig_ptr);
            }
        }
    }

    /* Return indicating signal was handled */
    return sig->length_with_header;
}
#endif

/*! \brief AMP move channel request

    Peer has sent an AMP move channel request.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_AMP_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_MOVE_CHANNEL_REQ(L2CAP_CHCB_T *chcb,
                                                    L2CAP_SIGNAL_INFO_T *sig)
{
    CID_MOVE_CHAN_REQ_T sig_args;
    CIDCB_T *cidcb;

    /* Check if this is a duplicate */
    if(SIGH_DuplicateDetect(chcb, sig))
    {
        return sig->length_with_header;
    }
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    sig_args.signal_id = sig->id;
    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 1, CsrUint8, 1, UNDEFINED, 0),
               &sig_args.source_cid,
               &sig_args.control_id);

    if((cidcb = CIDME_GetCidcbRemoteWithHandle(chcb, sig_args.source_cid))
            != NULL)
    {
        /* Crank the FSM */
        CID_AmpMoveInd(cidcb, &sig_args);
    }
    else
    {
        /* Reject due to invalid CID */
        SIGH_SignalSendCommandReject(chcb,
                                     sig_args.signal_id,
                                     SIGNAL_REJECT_CID,
                                     L2CA_CID_INVALID,
                                     sig_args.source_cid,
                                     0);
    }

    /* Return indicating signal was handled */
    return sig->length_with_header;
}
#endif

/*! \brief AMP move channel response

    Peer has sent an AMP move channel response.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_AMP_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_MOVE_CHANNEL_RES(L2CAP_CHCB_T *chcb,
                                                    L2CAP_SIGNAL_INFO_T *sig)
{
    SIG_SIGNAL_T *sig_ptr;
    CIDCB_T *cidcb;
    l2ca_cid_t cid_dest;
    CsrUint16 result; /* Used in read_uints() so can't be enum type */

    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
               &cid_dest,
               &result);

    /* Remove matching request signal from pending queue */
    if ((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                      sig->id)) != NULL)
    {
        /* Check CID in signal matches CID in signal block */
        if (sig_ptr->local_cid == cid_dest)
        {
            timer_cancel(&sig_ptr->timer_id);

            /* Get the CIDCB instance and crank the FSM */
            if ((cidcb = CIDME_GetCidcbWithHandle(chcb, cid_dest)) != NULL)
            {
                CID_AmpMoveCfm(cidcb, (l2ca_move_result_t)result);
            }

            /* Stuff signal back in queue if pending */
            if (result == L2CA_MOVE_PENDING)
            {
                /* Add signal back on pending queue */
                SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);

                /* Start the ERTX timer */
                timer_start(&sig_ptr->timer_id,
                            CSR_BT_L2CAP_ERTX_TIMEOUT,
                            SIGH_ErtxTimeout,
                            sig_ptr->signal_id,
                            chcb);
            }
            else
            {
                /* Free the signal */
                SIGH_SignalDestroy(sig_ptr);
            }
        }
        else
        {
            /* CID mismatch, ignore signal block. Add signal back on pending queue */
            SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
        }
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}
#endif

/*! \brief AMP move channel confirm request

    Peer has sent an AMP move channel confirm request.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_AMP_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_MOVE_CONFIRM_REQ(L2CAP_CHCB_T *chcb,
                                                    L2CAP_SIGNAL_INFO_T *sig)
{
    CID_MOVE_CHAN_CONFIRM_REQ_T sig_args;
    CIDCB_T *cidcb;

    /* Check if this is a duplicate */
    if(SIGH_DuplicateDetect(chcb, sig))
    {
        return sig->length_with_header;
    }
    SIGH_DuplicateSetRequest(&chcb->signal_dup, sig);

    sig_args.signal_id = sig->id;
    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 2, UNDEFINED, 0, UNDEFINED, 0),
               &sig_args.source_cid,
               &sig_args.result);

    /* Signal contains the remote/source CID */
    if((cidcb = CIDME_GetCidcbRemoteWithHandle(chcb, sig_args.source_cid))
            != NULL)
    {
        /* Crank the FSM (move-confirm-indication) */
        CID_AmpConfirmInd(cidcb, &sig_args);
    }
    else
    {
        /* Reject due to invalid (remote) CID */
        SIGH_SignalSendCommandReject(chcb,
                                     sig_args.signal_id,
                                     SIGNAL_REJECT_CID,
                                     L2CA_CID_INVALID,
                                     sig_args.source_cid,
                                     0);
    }

    /* Return indicating signal was handled */
    return sig->length_with_header;
}
#endif

/*! \brief AMP move channel confirm response

    Peer has sent an AMP move channel confirm response.

    \param chcb Pointer to CHCB instance.
    \param sig_data_ptr Pointer to signal payload.
    \param sig_id Signal ID.
    \param sig_size Size of signal payload.
    \return The size of the signal or 0 if the signal is malformed.
*/
#ifdef INSTALL_AMP_SUPPORT
static CsrUint16 SIGH_Handle_SIGNAL_MOVE_CONFIRM_RES(L2CAP_CHCB_T *chcb,
                                                    L2CAP_SIGNAL_INFO_T *sig)
{
    SIG_SIGNAL_T *sig_ptr;
    CIDCB_T *cidcb;
    l2ca_cid_t cid_source;

    read_uints(&sig->data,
               URW_FORMAT(CsrUint16, 1, UNDEFINED, 0, UNDEFINED, 0),
               &cid_source);

    /* Remove matching request signal from pending queue */
    if ((sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending,
                                                      sig->id)) != NULL)
    {
        /* Check CID in signal matches CID in signal block */
        if (sig_ptr->local_cid == cid_source)
        {
            /* Get the CIDCB instance */
            if ((cidcb = CIDME_GetCidcbWithHandle(chcb, cid_source)) != NULL)
            {
                /* Shake the FSM */
                CID_AmpConfirmCfm(cidcb);
            }

            /* Free the signal (also stops timer) */
            SIGH_SignalDestroy(sig_ptr);
        }
        else
        {
            /* CID mismatch, ignore signal block. Add signal back on pending queue */
            SIGH_SignalQueueAdd(&chcb->signal_pending, sig_ptr);
        }
    }

    /* Return indicating signal handled */
    return sig->length_with_header;
}
#endif

/*! \brief RTX timeout has expired.

    This function is called when the RTX timer has expired for a signal.
    If there are retries left, the signal is re-transmitted, otherwise the
    signal is destroyed and the CID state machine called.

    \param signal_id_in Signal ID of the signal.
    \param chcb_in Pointer to CHCB instance for the signal.
*/
static void SIGH_RtxTimeout(CsrUint16 signal_id_in, void *chcb_in)
{
    L2CAP_CHCB_T *chcb = (L2CAP_CHCB_T *)chcb_in;
    SIG_SIGNAL_T *sig_ptr;
    l2ca_identifier_t signal_id = (CsrUint8)signal_id_in;

    /* Remove the signal from the pending queue */
    sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending, signal_id);
    if (sig_ptr != NULL)
    {
        CIDCB_T *cidcb = CIDME_GetCidcbWithHandle(chcb, sig_ptr->local_cid);

        /* Reset timer ID */
        TIMER_EXPIRED(sig_ptr->timer_id);

        /* Check if we have retransmissions left */
        if (sig_ptr->rtx_timer_count)
        {
            /* Decrement RTX count */
            sig_ptr->rtx_timer_count--;

            /* Resend original signal block */
            SIGH_SignalSend(chcb, sig_ptr);

            /* Send timeout indication to the higher layer */
            L2CA_TimeoutInd(sig_ptr, cidcb, signal_id);
        }
        else
        {
            /* Send timeout indication to the CID/higher layer */
            switch (sig_ptr->signal_type)
            {
#ifdef INSTALL_ULP
                case SIGNAL_CONN_PARAM_UPDATE_REQ:
                    L2CA_ConnUpdateCfm(sig_ptr->p_handle,
                                       L2CA_MISC_TIMEOUT,
                                       CH_GET_TBDADDR(chcb));
                    break;
#endif
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
                case SIGNAL_ECHO_REQ:
                    L2CA_PingCfm(sig_ptr->p_handle, CH_GET_BD_ADDR(chcb),
                                 L2CA_MISC_TIMEOUT, NULL, 0,
                                 sig_ptr->local_cid); /* local_cid stores the req_ctx */
                    CH_StandardClose(chcb);
                    break;

                case SIGNAL_INFO_REQ:
                    L2CA_GetInfoCfm(sig_ptr->p_handle,
                                    CH_GET_BD_ADDR(chcb), L2CA_GETINFO_TIMEOUT,
                                    SIG_ReadGetInfoType(sig_ptr),
                                    NULL, 0,
                                    sig_ptr->local_cid); /* local_cid stores the req_ctx */
                    CH_StandardClose(chcb);
                    break;

                default:
                    /* Notify the CID that RTX timer has expired */
                    CID_Timeout(cidcb);
#endif
            }

            /* Free the signal */
            SIGH_SignalDestroy(sig_ptr);
        }
    }
}


/*! \brief ERTX timeout has expired.

    This function is called when the ERTX timer has expired for a signal.
    If there are retries left, the signal is re-transmitted, otherwise the
    signal is destroyed and the CID state machine called.

    \param signal_id_in Signal ID of the signal.
    \param chcb_in Pointer to CHCB instance for the signal.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
static void SIGH_ErtxTimeout(CsrUint16 signal_id_in, void *chcb_in)
{
    L2CAP_CHCB_T *chcb = (L2CAP_CHCB_T *)chcb_in;
    SIG_SIGNAL_T *sig_ptr;
    l2ca_identifier_t signal_id = (CsrUint8)signal_id_in;

    /* Remove the signal from the pending queue */
    sig_ptr = SIGH_SignalQueueRemoveWithSignalId(&chcb->signal_pending, signal_id);
    if (sig_ptr != NULL)
    {
        CIDCB_T *cidcb = CIDME_GetCidcbWithHandle(chcb, sig_ptr->local_cid);

        /* Reset timer ID */
        TIMER_EXPIRED(sig_ptr->timer_id);

        /* Check if we have retransmissions left */
        if (sig_ptr->ertx_timer_count)
        {
            /* Decrement ERTX count */
            sig_ptr->ertx_timer_count--;

            /* Resend original signal block */
            SIGH_SignalSend(chcb, sig_ptr);

            /* Send timeout indication to the higher layer */
            L2CA_TimeoutInd(sig_ptr, cidcb, signal_id);
        }
        else
        {
            /* Notify the CID that RTX timer has expired */
            CID_Timeout(cidcb);

            /* Free the signal */
            SIGH_SignalDestroy(sig_ptr);
        }
    }
}

#endif
