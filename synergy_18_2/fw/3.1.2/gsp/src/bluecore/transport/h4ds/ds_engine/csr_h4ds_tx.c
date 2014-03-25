/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/****************************************************************************
CONTAINS
        CsrH4DSTxInit  -  initialise the h4ds transmit path
        CsrH4DSTxMsgFrag  -  transmit a message fragment to the peer
        CsrH4DSTxPump  -  send h4ds messages to the uart
        CsrH4DSReqTxLeSync  -  req to send an le-sync msg to the peer
        CsrH4DSReqTxLeSack  -  req to send an le-sack msg to the peer
        CsrH4DSReqTxLeConf  -  req to send an le-conf msg to the peer
        CsrH4DSReqTxLeCack  -  req to send an le-cack msg to the peer
        CsrH4DSReqTxDsWu  -  request a wake-up msg to be sent to the peer
        CsrH4DSReqTxDsIa  -  request an i'm-awake msg to be sent to the peer
        CsrH4DSReqTxDsYms  -  req a you-may-sleep msg to be sent to the peer
        CsrH4DsTxPathClear  -  are any h4ds tx messages pending?

        h4ds_tx_pump_le  -  transmit pending h4ds link establishment messages
        h4ds_tx_pump_ds  -  transmit pending h4ds deep sleep messages
        init_tx_ds_msg_queue  -  initialise the deep sleep msg queue
        tx_ds_msg_req  -  ask to send a deep sleep msg to the peer
        get_tx_ds_msg  -  get the next deep sleep message to be transmitted
        ds_msg_pending  -  is there at least one queued deep sleep message
        shuffle_dsmsgq_from  -  shuffle ds msg queue elements
        tx_h4ds_msg  -  send an h4ds message to the uart

DESCRIPTION
        This file holds the transmit path of the h4ds library.

        There are four main inputs:

          - The primary input is CsrH4DSTxMsgFrag().  This is used to send h4
            messages to the UART.  The interface allows each h4 message to be
            transmitted as a set of fragments.  This may be useful where the
            local device doesn't have much RAM, and it may be difficult to
            assemble the whole of a large message in a single buffer (e.g.,
            the 256+ bytes of an HCI local name message).  All calls to
            CsrH4DSTxMsgFrag() are refused until the h4ds library is content
            that the transmit path to the peer is open (i.e., that the peer
            is has confirmed it is awake and is in a fit state to receive
            traffic.)

          - The functions CsrH4DSReqTxLeSync(), CsrH4DSReqTxLeSack(),
            CsrH4DSReqTxLeConf() and CsrH4DSReqTxLeCack() are used by the
            link establishment state machine, in h4ds_le.c, to request
            transmission of link establishment messages (Sync, Sack, Conf and
            Cack) to the peer.

          - The functions CsrH4DSReqTxDsWu(), CsrH4DSReqTxDsIa() and
            CsrH4DSReqTxDsYms() are used by the deep sleep state machine, in
            h4ds_ds.c, to request transmission of deep sleep messages
            (Wake-Up, I'm-Awake and You-May-Sleep) to the peer.  These
            requests, as well as the four h4ds_req_tx_le_*() functions,
            provoke calls to CsrH4DSPumpTxReq().  Calls to CsrH4DSPumpTxReq()
            should cause external code to call CsrH4DSPump().

          - The CsrH4DSTxPump() sends h4ds messages to the peer, as
            requested by the seven request functions.  CsrH4DSTxPump()
            is called from CsrH4DSPump().

        H4 Messages
        -----------

        When the UART is carrying h4 messages freely to the peer, a call to
        CsrH4DSTxMsgFrag() results almost directly in a call to
        H4DS_UART_PUTBYTES().  The request does not pass through the deep
        sleep state machine, as the design documentation suggests.  This
        optimisation is important for efficiency.  (E.g., the code in
        CsrH4DSTxMsgFrag() could be restructured to use a DMA engine.)

        The CsrH4DSTxMsgFrag() function can be used to send a complete h4
        message to the peer or, as noted above, it can be used to send a
        fragment of an h4 message.  If a first fragment is sent then all
        calls to CsrH4DSTxPump() are prevented from sending h4ds messages to
        the peer; the calls simply invoke CsrH4DSPumpTxReq() to request the
        pump function to be called again.  The external code must complete
        sending the fragmented h4 message before CsrH4DSPump() can do anything
        useful again.  This interlock is to prevent an h4ds message appearing
        in the middle of an h4 message on the UART link.

        Link Establishment
        ------------------

        The four requests to send link establishment messages simply set
        flags and call CsrH4DSPumpTxReq().  When external code calls
        CsrH4DSPump(), which calls CsrH4DSTxPump(), it spots a flag is set and
        attempts to send the corresponding link establishment message.  If it
        succeeds, the flag is cleared.  If it fails, it calls
        CsrH4DSPumpTxReq() again so that another attempt can be made later.

        It's possible that an attempt to send a link establishment message is
        blocked because the deep sleep engine doesn't think the peer is
        awake.  If so, the failed attempt to send the message will have
        caused the deep sleep engine to try to rouse the peer.

        Note that there is only a single flag per h4ds deep sleep message.
        If the link establishment engine makes two requests to send a Sync
        message in rapid succession then the code in this file only attempts
        to send one Sync message.  This is acceptable because the link
        establishment messages are "unreliable" - the link establishment
        protocol tolerates lost messages.

        The code to send link establishment messages, h4ds_tx_pump_le(),
        services the request flags in a set order that the protocol expects.
        A set of 4 flags is much cheaper than a message queue (as is needed
        for holding deep sleep message requests).

        Deep Sleep
        ----------

        The deep sleep engine, held in h4ds_ds.c, calls the three
        h4ds_tx_req_ds_*() functions to request deep sleep messages be sent
        to the peer.  As with the link establishment message requests, only
        one of each type of message can be pending at a time.  The deep sleep
        protocol also allows messages to be lost.  However, unlike the link
        establishment messages, the deep sleep message order must be
        maintained.  This is achieved with the three element message queue
        dsmsgq[], which is surrounded by a forest of support code.

        When the code considers sending a deep sleep message (via
        CsrH4DSTxPump() and h4ds_pump_ds()) it takes a message from the head
        of the message queue dsmsgq[], and places it in curr_dsmsg.  This
        effectively forms a second queue of length 1.  The code then attempts
        to send this message to the peer.  This awkward arrangement is
        necessary because Wake-Up messages are relatively long, and are sent
        as a sequence of fragments.

        UART
        ----

        A call to CsrH4DSPump() or CsrH4DSTxMsgFrag() makes, at most, one call
        to H4DS_UART_PUTBYTES().  This design constraint should make it
        easier for external code to decide whether to call either of the h4ds
        functions: if the UART is refusing bytes then there's no point in
        calling h4ds functions that try to send to the UART.
*/

#include "csr_h4ds_private.h"
#include "csr_types.h"
#include "csr_h4ds_inst.h"
#include "csr_util.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#include "csr_formatted_io.h"
#endif

/* The block of requests to send h4ds link establishment messages. */
#define req_tx_sync             (0x1)     /* Request to send LE Sync message. */
#define req_tx_sack             (0x2)     /* Request to send LE Sack message. */
#define req_tx_conf             (0x4)     /* Request to send LE Conf message. */
#define req_tx_cack             (0x8)     /* Request to send LE Cack message. */

static CsrUint8 tx_req_flags = 0;

#define BUF_NOT_READY_TIME      40000

extern H4DSInstanceData CsrH4DSInst;

/* Identifiers of h4ds deep sleep messages. */
enum dsmsgid_enum
{
    dsmsgid_none = 0,           /* No message. */
    dsmsgid_wu = 1,             /* Wake-Up. */
    dsmsgid_ia = 2,             /* I'm-Awake. */
    dsmsgid_yms = 3             /* You-May-Sleep. */
};
typedef enum dsmsgid_enum dsmsgid;


/* How many blocks of pre-wu characters (the vanguard of a Wake-Up message)
have been sent to wake the peer? */

static CsrUint16 wake_chunk_count;

/* The queue of pending requests to send deep sleep messages.  Element
dsmsgq[0] is the head of the queue, i.e., the next msg to be transmitted.
Queue length, DSMSGQLEN, must match the number of deep sleep msg types. */

#define DSMSGQLEN       (3)
static dsmsgid dsmsgq[DSMSGQLEN];

/* The h4ds deep sleep message being sent "now" is held in curr_dsmsg. */

static dsmsgid curr_dsmsg;

/* Is the transmit path currently sending an h4 message.  If this is TRUE
then a call has been made to CsrH4DSTxMsgFrag() with its last_frag flag set to
FALSE.  This requires that the external code should complete sending the h4
message (until the last fragment is signalled with its last_frag set to TRUE)
before any call is made to CsrH4DSPump().  The system cannot send an h4ds
message in the middle of an h4 message. */

CsrBool CsrSendingH4Msg;

/* Forward references. */
static void h4ds_tx_pump_le(void);
static CsrBool h4ds_tx_pump_ds(void);
static void init_tx_ds_msg_queue(void);
static void tx_ds_msg_req(dsmsgid id);
static dsmsgid get_tx_ds_msg(void);
static CsrBool ds_msg_pending(void);
static void shuffle_dsmsgq_from(CsrUint32 i);
static CsrBool tx_h4ds_msg(const CsrUint8 *msgbody);


/****************************************************************************
NAME
        CsrH4DSTxInit  -  initialise the h4ds transmit path
*/

void CsrH4DSTxInit(void)
{
    tx_req_flags = 0;
    init_tx_ds_msg_queue();
    curr_dsmsg = dsmsgid_none;
    wake_chunk_count = 0;
    CsrSendingH4Msg = FALSE;
}

/****************************************************************************
NAME
        CsrH4DSTxMsgFrag  -  transmit a message fragment to the peer
*/

CsrUint32 CsrH4DSTxMsgFrag(const CsrUint8 *buf, CsrUint32 len,
    CsrUint32 last_frag)
{
    CsrUint32 ret;

    /* If we're already in the midst of sending an h4 message to the peer
    then keep on sending.  (While the CsrSendingH4Msg flag is set, all
    calls to the pump function will be refused to ensure that h4ds
    messages do not appear in the middle of an h4 message.) */

    if (!CsrSendingH4Msg)
    {
        /* To get here the caller must be requesting to send a
        complete h4 message or the first fragment of an h4 message.

        Are we allowed to send the h4 message (fragment)?

        - Is the link establishment engine refusing to transmit
                 traffic?
        - Are there any pending requests to send link establishment
                messages (which have higher priority than h4)?
        - Is the deep sleep engine in a state that allows messages to
                be transmitted?
        - Has the deep sleep engine any pending requests to send
                deep sleep msgs (which have higher priority than h4)?
        - Is the deep sleep engine is in the middle of sending a
                deep sleep message.  (Caught by ds_msg_pending().) */

        if (CsrH4DSLeTxChoked() || tx_req_flags || !CsrH4DSCanTransmit()
            || ds_msg_pending())
        {
            /* Permission refused.  Signal that there's an
            attempt has been made to send an h4 message.  This
            may try to rouse the peer. */

            if (!CsrH4DSLeTxChoked())
            {
                CsrH4DSHlTxMsgAvail();
            }

            /* Efficiency: this call is not being allowed to send
            H4 traffic, but it seems a shame to waste the
            opportunity to send to the UART, so act as if the
            pump function had been called instead. */

            CsrH4DSTxPump();

            return 0;
        }
    }

    /* Defences passed.  Try to send the bytes. */
    ret = H4DS_UART_PUTBYTES(buf, len);
    if (ret)
    {
        CsrH4DSUartActive();
        if (ret == len)
        {
            CsrSendingH4Msg = !last_frag;
        }
        else
        {
            CsrSendingH4Msg = TRUE;
        }
    }

    return ret;
}

/****************************************************************************
NAME
        CsrH4DSTxPump  -  send h4ds messages to the uart
*/

void CsrH4DSTxPump(void)
{
    /* We may be in the middle of sending an h4 message.  The caller
    must complete this before attempting to send any h4ds traffic. */

    if (CsrSendingH4Msg)
    {
        CsrH4DSPumpTxReq();
    }
    else
    {
        /* We're only allowed to send once to the UART for each call
        to the  message per call to the pump function, so only call
        the link establishment engine if the deep sleep engine has
        not tried to send.  Also, ensure that a pump call request is
        requested if a deep sleep message has been sent and a link
        establishment message is pending */

        if (h4ds_tx_pump_ds())
        {
            if (tx_req_flags)
            {
                CsrH4DSPumpTxReq();
            }
        }
        else
        {
            h4ds_tx_pump_le();
        }

        /* The deep sleep state code sometimes needs to know when
        CsrH4DSTxPump() has been called. */

        CsrH4DSTxPumpCalled();
    }
}

/****************************************************************************
NAME
        h4ds_tx_pump_le  -  transmit pending h4ds link establishment messages

FUNCTION
        Attempt to send a single pending h4ds link establishment message to
        the peer.
*/

static void h4ds_tx_pump_le(void)
{
    CsrUint8 tmp = tx_req_flags;

    if (tx_req_flags)
    {
        if (CsrH4DSCanTransmit() && !ds_msg_pending())
        {
            /* We have (at least) one pending request to send a
            link establishment message; try to honour it. */

            if (tx_req_flags & req_tx_sync)
            {
                if (tx_h4ds_msg(CsrH4DSSyncMsgfrag))
                {
                    tx_req_flags = (tx_req_flags & (~req_tx_sync));                     /* set to FALSE */
                }
            }
            else if (tx_req_flags & req_tx_sack)
            {
                if (tx_h4ds_msg(CsrH4DSSackMsgfrag))
                {
                    tx_req_flags = (tx_req_flags & (~req_tx_sack));                     /* set to FALSE */
                }
            }
            else if (tx_req_flags & req_tx_conf)
            {
                if (tx_h4ds_msg(CsrH4DSConfMsgfrag))
                {
                    tx_req_flags = (tx_req_flags & (~req_tx_conf));                     /* set to FALSE */
                }
            }
            else if (tx_req_flags & req_tx_cack)
            {
                if (tx_h4ds_msg(CsrH4DSCackMsgfrag))
                {
                    tx_req_flags = (tx_req_flags & (~req_tx_cack));                     /* set to FALSE */
                }
            }
        }

        /* Kick the deep sleep engine if no link establishment
        message has been sent.  (It could have failed because the
        deep sleep engine thinks the peer is asleep, so this call
        should attempt to rouse the peer.  It could also have failed
        because the UART refused the bytes.  In this case the signal
        to the deep sleep engine should be harmlessly ignored. */

        if (tx_req_flags == tmp)
        {
            CsrH4DSHlTxMsgAvail();
        }

        /* Arrange to be called again if there's still work to do. */

        if (tx_req_flags)
        {
            CsrH4DSPumpTxReq();
        }
    }
}

/****************************************************************************
NAME
        h4ds_tx_pump_ds  -  transmit pending h4ds deep sleep messages

FUNCTION
        Attempt to send a single pending h4ds deep sleep message to the
        peer.

RETURNS
        TRUE if an attempt has been made to send a h4ds deep sleep message
        to the peer, else FALSE.

        (A return value of TRUE does not imply that a message *has* been
        sent to the peer.)
*/

static CsrBool h4ds_tx_pump_ds(void)
{
    CsrBool ret = FALSE;

    /* If we don't already have a deep sleep message to send (in
    curr_dsmsg), try to obtain one from the queue (posted to by the deep
    sleep state machine. */

    if (curr_dsmsg == dsmsgid_none)
    {
        curr_dsmsg = get_tx_ds_msg();
    }

    /* Have we a message to send to the UART? */

    if (curr_dsmsg != dsmsgid_none)
    {
        /* If curr_dsmsg is not dsmsgid_none then we will be
        attempting to send bytes to the UART, which is what we need
        to report to our caller. */

        ret = TRUE;

        /* Try to send (part of?) the deep sleep msg to the UART. */

        switch (curr_dsmsg)
        {
            case dsmsgid_wu:
                /* Each Wake-Up message starts with a train of
                H4DS_N_WU_PREAMBLE_CHUNKS of a 12-byte sequence
                designed to allow BlueCore-ROM's "auto-baud" logic to
                guess the peer's baud rate, then one instance of a
                12-byte sequence that contains four 0x00 bytes to
                ensure the peer's UART is locked to bytes' start
                bits, then one instance of a 12-byte sequence that
                contains "wu\0\0" (Wake-Up). */

                if (wake_chunk_count < H4DS_N_WU_PREAMBLE_CHUNKS)
                {
                    if (tx_h4ds_msg(CsrH4DSPreWuMsgfrag))
                    {
                        wake_chunk_count++;
                    }
                }
                else if (wake_chunk_count
                         == H4DS_N_WU_PREAMBLE_CHUNKS)
                {
                    if (tx_h4ds_msg(CsrH4DSPrezWuMsgfrag))
                    {
                        wake_chunk_count++;
                    }
                }
                else
                {
                    if (tx_h4ds_msg(CsrH4DSWuMsgfrag))
                    {
                        wake_chunk_count = 0;
                        curr_dsmsg = dsmsgid_none;
                    }
                }
                break;
            case dsmsgid_ia:
                if (tx_h4ds_msg(CsrH4DSIaMsgfrag))
                {
                    curr_dsmsg = dsmsgid_none;
                }
                break;
            case dsmsgid_yms:
                if (tx_h4ds_msg(CsrH4DSYmsMsgfrag))
                {
                    curr_dsmsg = dsmsgid_none;
                }
                break;
            default:
            {
#ifdef CSR_LOG_ENABLE
                CsrCharString logmsg[100];
                CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_tx.c: function h4ds_tx_pump_ds received unhandled message: %#x", curr_dsmsg);
                CSR_LOG_TEXT(logmsg);
#endif
                break;
            }
        }

        /* If we sent (or completed sending) a message, curr_dsmsg
        will be back at dsmsgid_none.  Let's see if there's another
        message waiting to be sent. */

        if (curr_dsmsg == dsmsgid_none)
        {
            curr_dsmsg = get_tx_ds_msg();
        }

        if (curr_dsmsg != dsmsgid_none)
        {
            CsrH4DSPumpTxReq();
        }
    }

    return ret;
}

/****************************************************************************
NAME
        CsrH4DSReqTxLeSync  -  req to send an le-sync msg to the peer
*/

void CsrH4DSReqTxLeSync(void)
{
    tx_req_flags = (tx_req_flags | req_tx_sync);
    CsrH4DSPumpTxReq();
}

/****************************************************************************
NAME
        CsrH4DSReqTxLeSack  -  req to send an le-sack msg to the peer
*/

void CsrH4DSReqTxLeSack(void)
{
    tx_req_flags = (tx_req_flags | req_tx_sack);
    CsrH4DSPumpTxReq();
}

/****************************************************************************
NAME
        CsrH4DSReqTxLeConf  -  req to send an le-conf msg to the peer
*/

void CsrH4DSReqTxLeConf(void)
{
    tx_req_flags = (tx_req_flags | req_tx_conf);
    CsrH4DSPumpTxReq();
}

/****************************************************************************
NAME
        CsrH4DSReqTxLeCack  -  req to send an le-cack msg to the peer
*/

void CsrH4DSReqTxLeCack(void)
{
    tx_req_flags = (tx_req_flags | req_tx_cack);
    CsrH4DSPumpTxReq();
}

/****************************************************************************
NAME
        CsrH4DSReqTxDsWu  -  request a wake-up msg to be sent to the peer
*/

void CsrH4DSReqTxDsWu(void)
{
    tx_ds_msg_req(dsmsgid_wu);
}

/****************************************************************************
NAME
        CsrH4DSReqTxDsIa  -  request an i'm-awake msg to be sent to the peer
*/

void CsrH4DSReqTxDsIa(void)
{
    tx_ds_msg_req(dsmsgid_ia);
}

/****************************************************************************
NAME
        CsrH4DSReqTxDsYms  -  req a you-may-sleep msg to be sent to the peer
*/

void CsrH4DSReqTxDsYms(void)
{
    tx_ds_msg_req(dsmsgid_yms);
}

/****************************************************************************
NAME
        CsrH4DsTxPathClear  -  are any h4ds tx messages pending?
*/

CsrBool CsrH4DsTxPathClear(void)
{
    return tx_req_flags == 0 && !ds_msg_pending();
}

/****************************************************************************
NAME
        init_tx_ds_msg_queue  -  initialise the deep sleep msg queue

FUNCTION
        Initialise the deep sleep message queue dsmsgq[].
*/

static void init_tx_ds_msg_queue(void)
{
    CsrUint32 i;

    /* Suck all elements from the message queue, discarding the results.
    This has the side effect of setting all elements of the queue to
    dsmsgid_none, i.e. emptying the queue. */

    for (i = 0; i < DSMSGQLEN; i++)
    {
        (void) get_tx_ds_msg();
    }
}

/****************************************************************************
NAME
        tx_ds_msg_req  -  ask to send a deep sleep msg to the peer

FUNCTION
        Request the deep sleep message identified by "id" to be sent to the
        peer.

IMPLEMENTATION NOTES
        This is evil.

        The code must record requests for deep sleep messages to be sent to
        the peer.  The requests must be kept in order, which naturally
        suggests a message queue.  Unfortunately, the deep sleep state
        machine can emit an arbitrary number of transmit message requests, so
        if the path to the peer is blocked this can imply a message queue of
        infinite length.  This is challenging on embedded systems, even for
        small values of infinity.

        The workaround applied here uses a queue of length 3, the length
        matching the number of types of deep sleep message.  When a request
        is made to send a message to the peer, the queue is searched.  If the
        queue already contains a message of that type it is removed from the
        queue, and the new request is added to the tail of the queue.  This
        approach depends on the fact that deep sleep messages are unreliable,
        so the system must operate if messages are lost, but it also avoids
        the pathological case: allowing the peer to sleep (by sending a
        You-May-Sleep message) then sending it reliable (H4) messages.

        If we were really desperate for RAM, it should be possible to
        implement the infinite length message queue in 8 bits.  This code is
        relatively clumsy, but should be more legible than such a "really
        clever" scheme

        The function also contains a feature to improve efficiency on some
        systems: the request to send "id" is ignored if the message is
        already being sent and if the dsmsgq[] queue is empty.  This is a bit
        dubious because this function is supposed to concern itself only with
        dsmsgq[], so it has no right to look at curr_dsmsg.  The code has
        been added because some systems with slow UARTs tend to be sluggish
        at sending a Wake-Up message, so an unnecessary second Wake-Up
        message was queued behind it, and this pointlessly blocked H4
        traffic.
*/

static void tx_ds_msg_req(dsmsgid id)
{
    CsrUint32 i;
    dsmsgid *m;

    /* Feature to improve efficiency on some systems with slow UARTs. */
    if ((curr_dsmsg == id) && (dsmsgq[0] == dsmsgid_none))
    {
        return;
    }

    /* Airbrush any instance of id from the message queue and shuffle the
    rest of the queue to remove the embarrassing gap. */

    for (i = 0, m = &(dsmsgq[0]); i < DSMSGQLEN; i++, m++)
    {
        if (*m == id)
        {
            shuffle_dsmsgq_from(i);
            break;
        }
    }

    /* Append id to the tail of the queue. */

    for (i = 0, m = &(dsmsgq[0]); i < DSMSGQLEN; i++, m++)
    {
        if (*m == dsmsgid_none)
        {
            *m = id;
            break;
        }
    }

    /* Request the transmit path to send the message. */
    CsrH4DSPumpTxReq();
}

/****************************************************************************
NAME
        get_tx_ds_msg  -  get the next deep sleep message to be transmitted

RETURNS
        The next (queued) request to send a deep sleep message to the peer,
        or dsmsgid_none if no request is available.
*/

static dsmsgid get_tx_ds_msg(void)
{
    dsmsgid ret = dsmsgq[0];
    shuffle_dsmsgq_from(0);
    return ret;
}

/****************************************************************************
NAME
        ds_msg_pending  -  is there at least one queued deep sleep message
*/

static CsrBool ds_msg_pending(void)
{
    return !(dsmsgq[0] == dsmsgid_none && curr_dsmsg == dsmsgid_none);
}

/****************************************************************************
NAME
        shuffle_dsmsgq_from  -  shuffle ds msg queue elements

FUNCTION
        Advances all elements of dsmsgq[] from element "i" (range zero to
        (DSMSGQLEN-1)) to the end, feeding a dsmsgid_none into the tail of
        the queue.
*/

static void shuffle_dsmsgq_from(CsrUint32 i)
{
    for ( ; i < DSMSGQLEN; i++)
    {
        if (i == (DSMSGQLEN - 1))
        {
            dsmsgq[DSMSGQLEN - 1] = dsmsgid_none;
        }
        else
        {
            dsmsgq[i] = dsmsgq[i + 1];
        }
    }
}

static void h4ds_buf_not_ready_wait(CsrUint16 tmp, void *tmp2)
{
    CsrH4DSInst.txBufNotReadyTid = 0;
    H4DS_PUMP_REQ();
}

static void h4ds_buf_not_ready_start_wait(void)
{
    /* Tx buffer is full. Give it a little piece and quite to send something */
    if (CsrH4DSInst.txBufNotReadyTid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.txBufNotReadyTid, 0, NULL);
    }
    CsrH4DSInst.txBufNotReadyTid = CsrSchedTimerSet((CsrTime) BUF_NOT_READY_TIME, h4ds_buf_not_ready_wait, 0, (void *) (NULL));
}

/****************************************************************************
NAME
        tx_h4ds_msg  -  send an h4ds message to the uart

FUNCTION
        Builds an h4ds message and sends it to the UART.  "msgbody" must
        point to a byte buffer of size H4DS_MSGFRAG_BODY_LEN, holding the
        h4ds message's body.

RETURNS
        TRUE if the h4ds message has been sent to the UART, else FALSE.
*/

static CsrBool tx_h4ds_msg(const CsrUint8 *msgbody)
{
    CsrUint8 msg[H4DS_MSG_LEN];

    /* Assemble the h4ds message. */
    (void) CsrMemCpy((void *) (msg),
        (const void *) (CsrH4DSLockMsgfrag),
        (CsrUint32) (H4DS_MSGFRAG_LOCK_LEN));
    (void) CsrMemCpy((void *) (&(msg[H4DS_MSGFRAG_LOCK_LEN])),
        (const void *) (msgbody),
        (CsrUint32) (H4DS_MSGFRAG_BODY_LEN));

    if (H4DS_UART_PUTBYTES(msg, H4DS_MSG_LEN) == 0)
    {
        h4ds_buf_not_ready_start_wait();
        return FALSE;
    }

    CsrH4DSUartActive();
    return TRUE;
}
