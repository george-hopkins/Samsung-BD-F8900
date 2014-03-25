/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp.h"
#include "csr_abcsp_events.h"
#include "csr_abcsp_panics.h"
#include "csr_abcsp_config_timer.h"
#include "csr_abcsp_config_event.h"
#include "csr_abcsp_config_panic.h"
#include "csr_abcsp_config_txmsg.h"
#include "csr_abcsp_txmsg.h"
#include "csr_abcsp_crc.h"
#include "csr_abcsp_prototype.h"

#include "csr_abcsp_log_text.h"
#include "csr_util.h"
#include "csr_log_text_2.h"

abcsp CsrAbcspInstanceData;


/* Forward references. */
static CsrUint32 xabcsp_pumptxmsgs(void);
static CsrBool pkt_avail(void);
static void pkt_cull(void);
static CsrUint8 *get_txmsgbuf(CsrUint16 *len);
static void restart_timeout(void);
static void txmsgbuf_taken(CsrUint16 ntaken);
static CsrBool slip_one_byte(CsrUint8 b);


/****************************************************************************
NAME
        CsrAbcspTxMsgInit  -  initialise the transmit message assembler
*/

void CsrAbcspTxMsgInit(void)
{
    CsrUint8 seq;
    abcsp *const _this = &CsrAbcspInstanceData;

    ABCSP_CANCEL_BCSP_TIMER();

    _this->txmsg.state = txstate_idle;
    _this->txmsg.msgq_txseq = 0;
    _this->txmsg.msgq_unackd_txseq = 0;
    _this->txmsg.txseq = 0;
    _this->txmsg.unrel_index = 0;
    _this->txrx.rxseq_txack = 0;

    for (seq = 0; seq < 2; seq++)
    {
        _this->txmsg.unrelUsed[seq] = FALSE;
    }
}

/****************************************************************************
NAME
        CsrAbcspTxMsgDeinit  -  deinitialise the transmit message assembler
*/

void CsrAbcspTxMsgDeinit(void)
{
    CsrUint32 seq;
    abcsp *const _this = &CsrAbcspInstanceData;

    ABCSP_CANCEL_BCSP_TIMER();

    /* Clear the two message queues. */
    for (seq = _this->txmsg.msgq_unackd_txseq;
         seq != _this->txmsg.msgq_txseq;
         seq = (seq + 1) % 8)
    {
        ABCSP_TXMSG_DONE(&_this->txmsg.rel[seq].m);
    }

    for (seq = 0; seq < 2; seq++)
    {
        if (_this->txmsg.unrelUsed[seq])
        {
            ABCSP_TXMSG_DONE(&_this->txmsg.unrel[seq].m);
            _this->txmsg.unrelUsed[seq] = FALSE;
        }
    }
}

/****************************************************************************
NAME
        CsrAbcspMsgSend  -  set up message for sending to the uart
*/

CsrUint32 CsrAbcspMsgSend(ABCSP_TXMSG *msg,
    CsrUint8 chan, CsrUint32 rel)
{
    /* Reject all traffic if the choke is applied.

    BCSP-LE messages are transmitted from code below this entry point.

    The choke should be applied at the "mux" layer.  Applying it here
    means that if the choke is turned on while messages are queued for
    transmission then those messages will drain out.  This is strictly
    incorrect, but this won't harm any real system as the choke is only
    set TRUE by abcsp library init, so any peer is going to see
    disrupted traffic for a while anyway.  (Ideally, bcsp-le messages
    from here will tell the peer that we've restarted, so it should
    reinit and rechoke.) */
    abcsp *const _this = &CsrAbcspInstanceData;

    if (_this->txrx.choke)
    {
        ABCSP_EVENT(ABCSP_EVT_TX_CHOKE_DISCARD);
        return 0;
    }

    /* Parameter sanity checks. */
    if ((rel > 1) || (chan < 2) || (chan > 15) || (msg == NULL))
    {
        return 0;
    }

    if (rel)
    {
        /* We queue enough reliable messages to fill the
         * WINSIZE window. */
        if (((_this->txmsg.msgq_txseq
              - _this->txmsg.msgq_unackd_txseq) % 8)
            >= CSR_ABCSP_TXWINSIZE)
        {
            ABCSP_EVENT(ABCSP_EVT_TX_WINDOW_FULL_DISCARD);
            return 0;
        }

        /* We've checked the reliable queue has room. */
        CsrMemCpy(&_this->txmsg.rel[_this->txmsg.msgq_txseq].m, msg,
            sizeof(_this->txmsg.rel[_this->txmsg.msgq_txseq].m));
        _this->txmsg.rel[_this->txmsg.msgq_txseq].chan = chan;
        _this->txmsg.msgq_txseq = (_this->txmsg.msgq_txseq + 1) % 8;
    }
    else
    {
        /* The unreliable channel is biased towards supporting
           sco, for which the data has to be fresh.  The queue
           holds only one message, so we displace any message that's
           already in the queue. */
        if (_this->txmsg.unrelUsed[_this->txmsg.unrel_index] != FALSE)
        {
            ABCSP_TXMSG_DONE(&_this->txmsg.unrel[_this->txmsg.unrel_index].m);
        }
        CsrMemCpy(&_this->txmsg.unrel[_this->txmsg.unrel_index].m, msg,
            sizeof(_this->txmsg.unrel[_this->txmsg.unrel_index].m));
        _this->txmsg.unrel[_this->txmsg.unrel_index].chan = chan;
    }

    /* Tell external code that it needs to call CsrAbcspPumpTxMsgs(). */
    ABCSP_REQ_PUMPTXMSGS(_this);

    /* Report message accepted. */
    return 1;
}

/****************************************************************************
NAME
        restart_timeout  -  stop and restart the bcsp timer

FUNCTION
        Cancels the current BCSP timed event timer, if it is running, then
        starts the BCSP timed timer.
*/

static
void restart_timeout(void)
{
    ABCSP_CANCEL_BCSP_TIMER();
    ABCSP_START_BCSP_TIMER();
}

/****************************************************************************
NAME
        slip_one_byte  -  slip one byte and send it to the uart

RETURNS
        TRUE if the byte "b" was slipped and sent to the UART, else FALSE.
*/

static
CsrBool slip_one_byte(CsrUint8 b)
{
    return CsrAbcspTxSlipSendBuf(&b, 1) == 1;
}

/****************************************************************************
NAME
        relq_contains  -  Does the retransmit Q conatin SEQ n

FUNCTION
        Returns TRUE if the retransmit Q contains a packet with the
        given sequence number.
*/

static
CsrBool relq_contains(CsrUint8 num)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    if (_this->txmsg.msgq_txseq >= _this->txmsg.msgq_unackd_txseq)
    {
        return num >= _this->txmsg.msgq_unackd_txseq
               && num < _this->txmsg.msgq_txseq;
    }
    else
    {
        return num >= _this->txmsg.msgq_unackd_txseq
               || num < _this->txmsg.msgq_txseq;
    }
}

/****************************************************************************
NAME
        pkt_avail  -  is there a new transmit message available

FUNCTION
        This determines which of the available transmit messages should
        be sent next to the UART, i.e., it implements the message
        prioritisation.

        The function also configures this file's state ready for the
        chosen message's transmission.

RETURNS
        TRUE if a message is available to be sent, else FALSE.

        If the function returns TRUE then details of the packet to be sent
        have been written directly to this file's static variable "curr".
*/

static
CsrBool pkt_avail(void)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    /* Default settings - an ack packet. */
    _this->txmsg.curr.rel = FALSE;
    _this->txmsg.curr.len = 0;
    _this->txmsg.curr.type = CURRMSG_TYPE_INTMSG;
    _this->txmsg.curr.m.inter.outdex = 0;
    _this->txmsg.curr.m.inter.chan = 0;
    _this->txmsg.curr.m.inter.buf = NULL;

    /* BCSP-LE messages have the highest priority.  (Always unrel.) */
    if (_this->txrx.txsync_req || _this->txrx.txsyncresp_req
        || _this->txrx.txconf_req || _this->txrx.txconfresp_req)
    {
        _this->txmsg.curr.len = 4;
        _this->txmsg.curr.m.inter.chan = 1;
        if (_this->txrx.txsync_req)
        {
            /*            BT_DBG("sync");*/
            _this->txrx.txsync_req = FALSE;
            _this->txmsg.curr.m.inter.buf =
                (CsrUint8 *) (CsrAbcspLeSyncMsg);
        }
        else if (_this->txrx.txsyncresp_req)
        {
            /*            BT_DBG("syncresp");*/
            _this->txrx.txsyncresp_req = FALSE;
            _this->txmsg.curr.m.inter.buf =
                (CsrUint8 *) (CsrAbcspLeSyncrespMsg);
        }
        else if (_this->txrx.txconf_req)
        {
            /*            BT_DBG("conf");*/
            _this->txrx.txconf_req = FALSE;
            _this->txmsg.curr.m.inter.buf =
                (CsrUint8 *) (CsrAbcspLeConfMsg);
        }
        else if (_this->txrx.txconfresp_req)
        {
            /*            BT_DBG("confresp");*/
            _this->txrx.txconfresp_req = FALSE;
            _this->txmsg.curr.m.inter.buf =
                (CsrUint8 *) (CsrAbcspLeConfrespMsg);
        }
        return TRUE;
    }

    /* Any unreliable TXMSG has the */
    if (_this->txmsg.unrelUsed[_this->txmsg.unrel_index])
    {
        /*BT_DBG("unrel");*/

        /* We *remove* the message from the queue.  We will
           destroy it after sending it once.  (This is
           different from the way in which relq is used, where
           messages remain in the queue until their reception
           is acknowledged by the peer.) */
        _this->txmsg.curr.type = CURRMSG_TYPE_TXMSG;
        _this->txmsg.curr.m.txmsg = &_this->txmsg.unrel[_this->txmsg.unrel_index];
        ABCSP_TXMSG_INIT_READ(&_this->txmsg.curr.m.txmsg->m);
        _this->txmsg.curr.len = (CsrUint16) ABCSP_TXMSG_LENGTH(&_this->txmsg.curr.m.txmsg->m);

        _this->txmsg.unrel_index = (_this->txmsg.unrel_index + 1) % 2;

        return TRUE;
    }
    /*
      BT_DBG("msgQ-unack %d  txseq %d  msgQ %d",
      _this->txmsg.msgq_unackd_txseq,
      _this->txmsg.txseq,
      _this->txmsg.msgq_txseq);
 */
    /* Any reliable data is next in the pecking order. */
    if (relq_contains((CsrUint8) _this->txmsg.txseq))
    {
        _this->txmsg.curr.type = CURRMSG_TYPE_TXMSG;
        _this->txmsg.curr.seq = _this->txmsg.txseq;
        _this->txmsg.curr.m.txmsg = &_this->txmsg.rel[_this->txmsg.txseq];

        ABCSP_TXMSG_INIT_READ(&_this->txmsg.curr.m.txmsg->m);
        _this->txmsg.curr.len = (CsrUint16) ABCSP_TXMSG_LENGTH(&_this->txmsg.curr.m.txmsg->m);
        _this->txmsg.curr.rel = TRUE;

        _this->txmsg.txseq = (_this->txmsg.txseq + 1) % 8;
        restart_timeout();

        return TRUE;
    }

    /* Finally, send an ack packet if needed. */
    if (_this->txrx.txack_req)
    {
        /* All messages send ack val, so clear the flag later.
           This also fights race hazard if tx and rx threads
           are separate. */
        return TRUE;
    }

    /* No message available. */
    return FALSE;
}

/****************************************************************************
NAME
        pkt_cull  -  discard transmit messages no longer needed

FUNCTION
        Determines which reliable packets have been received (acked)
        by the peer and discards the local copy of these if they are in
        the reliable transmit queue.
*/

static
void pkt_cull(void)
{
    CsrUint32 rxack, seq;
    abcsp *const _this = &CsrAbcspInstanceData;

    /* Give up immediately if there's nothing to cull. */
    if (_this->txmsg.msgq_txseq == _this->txmsg.msgq_unackd_txseq)
    {
        return;
    }

    /* The received ack value is always one more than the seq of
       the packet being acknowledged. */

    rxack = (_this->txrx.rxack - 1 + 8) & 7;

    /* abcsp_txrx.rxack carries the ack value from the last packet
     * received from the peer.  It marks the high water mark of
     * packets accepted by the peer.  We only use it if it refers
     * to a message in the transmit window, i.e., it should refer
     * to a message in relq.  (It's not always an error if it
     * refers to a message outside the window - it is initialised
     * that way - but it would be possible to extend this code to
     * detect absurd rxack values.)
     *
     * In the buffer relq packets in positions unackd_txseq to
     * txseq-1 have been sent but not ackd.  Ie. there is
     * something in this retransmit slot.  We walk through the
     * unsent packets (from txseq to unackd_txseq-1), and if the
     * received ack is in this set then ack should be ignored.
 */

    if (!relq_contains((CsrUint8) rxack))
    {
        return;
    }

    /* Finally, we walk relq, discarding its contents, until after
     * we discard the packet with sequence number rxack.
     *
     * Seq counts through all of the packets that we have sent but
     * not yet got an ack for.  If at some point seq == rxack the
     * ack is in the range of packets that we have sent, and we
     * need to clear some packets out of our retransmit buffer.
 */
    while ((seq = _this->txmsg.msgq_unackd_txseq)
           != _this->txmsg.msgq_txseq)
    {
        CsrUint32 tmpIdx = _this->txmsg.msgq_unackd_txseq;

        ABCSP_TXMSG_DONE(&_this->txmsg.rel[seq].m);

        _this->txmsg.msgq_unackd_txseq = (seq + 1) & 7;

        if (tmpIdx == _this->txmsg.txseq) /* acknowledged message was to be retransmitted... update retreansmit index accordingly
            this is done to avoid strange cross situations between acknowledge and acknowledge timeout */
        {
            _this->txmsg.txseq = _this->txmsg.msgq_unackd_txseq;
        }

        if (seq == rxack)
        {
            break;
        }
    }
}

/****************************************************************************
NAME
        get_txmsgbuf  -  get a block of transmit message bytes

FUNCTION
        Obtains a block of bytes from the currently selected transmit
        message.

RETURNS
        The base of the byte buffer to be transmitted, or NULL if no
        (more) bytes remain to be sent.

        The length of any returned buffer is written at "*len".
*/

static
CsrUint8 *get_txmsgbuf(CsrUint16 *len)
{
    CsrUint32 ulen;
    CsrUint8 *ret;
    abcsp *const _this = &CsrAbcspInstanceData;

    /* Take from a plain byte buffer if an internally-generated msg. */
    if (_this->txmsg.curr.type == CURRMSG_TYPE_INTMSG)
    {
        if (_this->txmsg.curr.m.inter.outdex < _this->txmsg.curr.len)
        {
            *len = _this->txmsg.curr.len
                   - _this->txmsg.curr.m.inter.outdex;
            return &(_this->txmsg.curr.m.inter.buf
                     [_this->txmsg.curr.m.inter.outdex]);
        }
        return NULL;
    }

    /* Otherwise ask the environment for a buffer holding part
       of the whole message. */

    /* This should be one line, but lint moans. */
    ret = (CsrUint8 *) ABCSP_TXMSG_GETBUF(&_this->txmsg.curr.m.txmsg->m,
        &ulen);
    *len = (CsrUint16) ulen;

    return ret;
}

/****************************************************************************
NAME
        txmsgbuf_taken  -  how many source bytes have been taken

FUNCTION
        Tells the current message source how many bytes have just been
        consumed.
*/

static
void txmsgbuf_taken(CsrUint16 ntaken)
{
    abcsp *const _this = &CsrAbcspInstanceData;

    if (_this->txmsg.curr.type == CURRMSG_TYPE_INTMSG)
    {
        _this->txmsg.curr.m.inter.outdex += ntaken;
    }
    else
    {
        ABCSP_TXMSG_TAKEN(&_this->txmsg.curr.m.txmsg->m, ntaken);
    }
}

/****************************************************************************
NAME
        xabcsp_pumptxmsgs  -  send message to the uart

FUNCTION
        As CsrAbcspPumpTxMsgs(), except that this does not send messages out
        to the UART via ABCSP_UART_SENDBYTES().  This function obtains a
        buffer via ABCSP_UART_GETBUF() and may put slipped bytes into this,
        but it does not actually push the buffer through to the UART.
*/

static
CsrUint32 xabcsp_pumptxmsgs(void)
{
    CsrUint8 *buf, b;
    CsrUint16 buflen;
    abcsp *const _this = &CsrAbcspInstanceData;

    for ( ; ; )
    {
        switch (_this->txmsg.state)
        {
            default:
            case txstate_idle:
                /* Discard any acknowledged reliable messages. */
                pkt_cull();

                /* Choose pkt to send.  Writes directly to "curr". */
                if (!pkt_avail())
                {
                    return 0;
                }

                /* Sanity check. */
                if (_this->txmsg.curr.len > ABCSP_MAX_MSG_LEN)
                {
                    ABCSP_PANIC(ABCSP_PANIC_BCSP_MSG_OVERSIZE,
                        "bcsp msg too big");
                }
            /* FALLTHROUGH */

            case txstate_msgstart:
                if (abcsp_txslip_msgstart() == FALSE)
                {
                    _this->txmsg.state = txstate_msgstart;
                    return 1;
                }
#ifdef  CSR_ABCSP_TXCRC
                CsrAbcspCrcInit(&_this->txmsg.crc);
                _this->txrx.txcrc = TRUE;
#endif  /* CSR_ABCSP_TXCRC */

            /* FALLTHROUGH */

            case txstate_hdr1:
                /* The txack value is 1 more than the seq of
                   the received packet being acknowledged.
                   However, the rxseq_txack value was
                   incremented by the receive path, so we use
                   the value of rxseq_txack directly. */

                b = _this->txrx.rxseq_txack << 3;
                _this->txrx.txack_req = FALSE;
                if (_this->txmsg.curr.rel)
                {
                    b |= 0x80 + _this->txmsg.curr.seq;
                }
#ifdef  CSR_ABCSP_TXCRC
                b |= 0x40;
#endif  /* CSR_ABCSP_TXCRC */
                _this->txmsg.cs = b;
                if (!slip_one_byte(b))
                {
                    _this->txmsg.state = txstate_hdr1;
                    return 1;
                }

            /* FALLTHROUGH */

            case txstate_hdr2:
                b = _this->txmsg.curr.len << 4;
                if (_this->txmsg.curr.type == CURRMSG_TYPE_INTMSG)
                {
                    b |= _this->txmsg.curr.m.inter.chan;
                }
                else
                {
                    b |= _this->txmsg.curr.m.txmsg->chan;
                }
                if (!slip_one_byte(b))
                {
                    _this->txmsg.state = txstate_hdr2;
                    return 1;
                }
                _this->txmsg.cs += b;

            /* FALLTHROUGH */
            case txstate_hdr3:
                b = _this->txmsg.curr.len >> 4;
                if (!slip_one_byte(b))
                {
                    _this->txmsg.state = txstate_hdr3;
                    return 1;
                }
                _this->txmsg.cs += b;

            /* FALLTHROUGH */

            case txstate_hdr4:
                if (!slip_one_byte((CsrUint8) ~_this->txmsg.cs))
                {
                    _this->txmsg.state = txstate_hdr4;
                    return 1;
                }

            /* FALLTHROUGH */

            case txstate_payload:

                while ((buf = get_txmsgbuf(&buflen)) != NULL)
                {
                    CsrUint16 taken = CsrAbcspTxSlipSendBuf(buf, buflen);
                    txmsgbuf_taken(taken);
                    if (taken != buflen)
                    {
                        _this->txmsg.state = txstate_payload;
                        return 1;
                    }
                }

                /* Destroy the message if it's unreliable and
                   externally generated (main example: sco).
                   We send this sort of message only once, and
                   it's already been removed from the unrelq
                   in pkt_avail(). */

                if ((_this->txmsg.curr.type == CURRMSG_TYPE_TXMSG)
                    && (_this->txmsg.curr.rel == FALSE))
                {
                    ABCSP_TXMSG_DONE(&_this->txmsg.curr.m.txmsg->m);
                }
#ifdef CSR_ABCSP_TXCRC
                _this->txrx.txcrc = FALSE;
                _this->txmsg.crc
                    = CsrAbcspCrcReverse(_this->txmsg.crc);

            /* FALLTHROUGH */

            case txstate_crc1:
                if (!slip_one_byte((CsrUint8) ((_this->txmsg.crc >> 8) & 0xff)))
                {
                    _this->txmsg.state = txstate_crc1;
                    return 1;
                }

            /* FALLTHROUGH */

            case txstate_crc2:
                if (!slip_one_byte((CsrUint8) (_this->txmsg.crc & 0xff)))
                {
                    _this->txmsg.state = txstate_crc2;
                    return 1;
                }
#endif  /* CSR_ABCSP_TXCRC */

            /* FALLTHROUGH */

            case txstate_msgend:
                if (abcsp_txslip_msgend() == FALSE)
                {
                    _this->txmsg.state = txstate_msgend;
                    return 1;
                }
                _this->txmsg.state = txstate_idle;
                break;
        }
    }
    /* we will not get here */
}

/****************************************************************************
NAME
        CsrAbcspPumpTxMsgs  -  send message to the uart
*/

CsrUint32 CsrAbcspPumpTxMsgs(void)
{
    CsrUint32 ret;

    /* Prepare bytes for passing to the UART. */
    ret = xabcsp_pumptxmsgs();

    /* Push (once!) the prepared bytes to the UART. */
    CsrAbcspTxSlipFlush();

    /* Report whether any work remains to be done immediately. */
    return ret;
}

/****************************************************************************
NAME
        CsrAbcspBcspTimedEvent  -  transmit path's timed event
*/

void CsrAbcspBcspTimedEvent(void)
{
    CsrUint8 new_txseq;
    abcsp *const _this = &CsrAbcspInstanceData;

    if (_this->txmsg.msgq_txseq == _this->txmsg.msgq_unackd_txseq)
    {
        CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_TX_RETRANS_NOOP"));
        return;
    }
    else
    {
        new_txseq = (CsrUint8) _this->txmsg.msgq_unackd_txseq;
        CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_TX_RETRANS_OP (%u)", new_txseq));
    }

    /* Arrange to retransmit all messages in the relq. */
    if (new_txseq != _this->txmsg.txseq)
    {
        _this->txmsg.txseq = new_txseq;
        ABCSP_REQ_PUMPTXMSGS(_this);
        restart_timeout();
    }
}
