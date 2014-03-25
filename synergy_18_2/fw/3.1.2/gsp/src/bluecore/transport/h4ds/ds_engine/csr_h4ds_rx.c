/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4ds_private.h"

/* Receive state machine states. */
#define st_scan         0x0         /* Seeking a message start. */
#define st_h4_msg       0x1         /* Matching an h4 message. */
#define st_h4ds_msg     0x2         /* Matching an h4ds message. */


/* State information for the message scanner. */
static CsrUint8 ds_rx_state = 0;


/* Forward references. */
static CsrUint16 xh4ds_uart_deliverbytes(const CsrUint8 *b, CsrUint16 n, CsrUint8 chan,
    CsrBool *h4ds_deliverbytes_called);
static msglayer msg_start(CsrUint8 b);


/****************************************************************************
NAME
        CsrH4DSRxInit  -  initialise the h4ds library's receive path
*/

void CsrH4DSRxInit(void)
{
    /* Optimisation:
    ds_rx_state = st_groggy_scanning; */

    ds_rx_state = 0;
}

/****************************************************************************
NAME
        CsrH4DSUartDeliverbytes  -  push uart bytes up into h4ds stack

IMPLEMENTATION NOTES
        This function receives all bytes received from the UART.  The bulk of
        traffic is passed up through the h4ds stack layers to form received
        h4/HCI messages.  However, h4ds layer messages are consumed from the
        received byte stream.

        CsrH4DSUartDeliverbytes() is a wrapper for the core function
        xh4ds_uart_deliverbytes(), which does the real work.  The wrapper
        keeps pushing bytes into the core function until either it reports
        that it has has passed bytes up to the h4 layer, or that it has run
        out of bytes.  The former constraint is necessary to meet the h4ds
        library's design requirement that each call into the library will
        provoke at most one transfer of bytes up out of the library.

        The wrapper function cannot call the core function if "len" is zero.
        This simplifies use of xh4ds_uart_deliverbytes()'s calls to
        msg_start().

        While we are receiving h4 traffic, the main state machine remains in
        st_awake_h4_msg, where the h4ds code does not inspect each byte
        flowing past.  This is important for performance.
*/

CsrUint32 CsrH4DSUartDeliverbytes(const CsrUint8 *buf,
    CsrUint32 len, CsrUint8 chan)
{
    CsrBool h4ds_deliverbytes_called;
    CsrUint16 ntaken;

    for (ntaken = 0, h4ds_deliverbytes_called = FALSE; ntaken < len; )
    {
        ntaken += xh4ds_uart_deliverbytes((const CsrUint8 *) (buf + ntaken),
            (CsrUint16) (len - ntaken), chan,
            &h4ds_deliverbytes_called);
        if (h4ds_deliverbytes_called)
        {
            break;
        }
    }

    return (CsrUint32) (ntaken);
}

static CsrUint16 xh4ds_uart_deliverbytes(const CsrUint8 *b, CsrUint16 n, CsrUint8 chan,
    CsrBool *h4ds_deliverbytes_called)
{
    CsrUint16 ntaken = 0;
    CsrBool message_start;
    ds_msgid mid;

    switch (ds_rx_state)
    {
        case st_scan:
            /* Seek the first byte of an h4 or h4ds msg. */

            switch (msg_start(chan))
            {
                case msglayer_h4:
                    /* Pass h4 traffic up out of the h4ds stack if the
                    deep sleep and link establishment state machines
                    allow it.  Otherwise, consume the byte, and the rest
                    of the (apparent) h4 message should then be discarded
                    as junk. */


                    if (CsrH4DSCanReceive() && !CsrH4DSLeRxChoked())
                    {
                        ds_rx_state = st_h4_msg;
                    }
                    else
                    {
                        ntaken = 1;
                    }
                    break;
                case msglayer_h4ds:
                    /* We are always open to receiving h4ds messages.
                    The h4ds msg scanner has its own mechanism for
                    discarding junk. */

                    CsrH4DSRxInitMatchMsg();
                    ds_rx_state = st_h4ds_msg;
                    break;
                case msglayer_none:
                    /* We appear to have received junk.  If this occurs
                    when the link is supposed to be working perfectly
                    then we complain - external code should treat this as
                    seriously as H4 sync loss.  We don't moan to the
                    external code if we haven't completed H4DS Link
                    Establishment as it has no way of using H4's
                    "recovery" mechanism. */

                    if (CsrH4DSCanReceive() && h4ds_le_complete())
                    {
                        H4DS_EVENT(H4DS_EVT_SYNC_LOSS);
                    }
                    ntaken = 1;
                    break;
                default:
                    H4DS_PANIC(H4DS_PANIC_INT_CORRUPTION);
                    ntaken = 1;
                    break;
            }
            break;
        case st_h4_msg:
            /* The byte stream is ducted up into the h4 layer to form an
            h4 message.  The h4ds library is required to make a maximum
            of one call to H4DS_DELIVERBYTES() per call to
            CsrH4DSUartDeliverbytes(); this is signalled by setting
            h4ds_deliverbytes_called to TRUE, which will shortly cause
            CsrH4DSUartDeliverbytes() to return.

            The h4 layer signals that it is will be expecting the start
            of a fresh h4 msg on the next call to
            h4ds_h4rx_deliverbytes() via the message_start flag.  We use
            this to return to the st_scan state, where we search for the
            start of the next h4 or h4ds msg. */

            ntaken = H4DS_DELIVERBYTES(b, n, chan,
            (CsrUint32 *) (&message_start));
            *h4ds_deliverbytes_called = TRUE;
            if (message_start)
            {
                ds_rx_state = st_scan;
            }
            break;
        case st_h4ds_msg:
            switch (mid = CsrH4DSRxMatchMsg(b, n, &ntaken))
            {
                case CsrSchedMsgId_none:
                    /* Message incomplete. */
                    break;
                case CsrSchedMsgId_pre_wu:
                case CsrSchedMsgId_prez_wu:
                    /* These Wake-Up message (fragments) exist only to
                    get the UART going when we wake up.  Ingore them. */
                    break;
                case CsrSchedMsgId_wu:
                    /* Peer requires us to stay awake. */
                    CsrH4DSWuMsgRxed();
                    break;
                case CsrSchedMsgId_ia:
                    /* Peer confirms it's awake. */
                    CsrH4DSIaMsgRxed();
                    break;
                case CsrSchedMsgId_yms:
                    /* Peer allows us to sleep. */
                    CsrH4DSYmsMsgRxed();
                    break;
                case CsrSchedMsgId_sync:
                    CsrH4DSLeFsm(lemsgid_sync);
                    break;
                case CsrSchedMsgId_sack:
                    CsrH4DSLeFsm(lemsgid_sack);
                    break;
                case CsrSchedMsgId_conf:
                    CsrH4DSLeFsm(lemsgid_conf);
                    break;
                case CsrSchedMsgId_cack:
                    CsrH4DSLeFsm(lemsgid_cack);
                    break;
                case CsrSchedMsgId_duff:
                    /* If the link is supposed to be working perfectly
                    then we should receive only crisp, perfectly-formed
                    (h4ds) messages, so we moan if we receive rubbish.
                    If the link is possibly still wobbly then we silently
                    discard the junk.  Only bother to moan to external
                    code if we've completed Link Establishment, as this
                    is needed for H4 to perform its link "recovery"
                    mechanism. */
                    ntaken = n;
                    if (CsrH4DSCanReceive() && h4ds_le_complete())
                    {
                        H4DS_EVENT(H4DS_EVT_SYNC_LOSS);
                    }
                    break;
                default:
                    H4DS_PANIC(H4DS_PANIC_INT_CORRUPTION);
                    break;
            }
            if (mid != CsrSchedMsgId_none)
            {
                ds_rx_state = st_scan;
            }
            if ((mid != CsrSchedMsgId_none) && (mid != CsrSchedMsgId_yms))
            {
                CsrH4DSUartActive();
            }
            break;
        default:
            H4DS_PANIC(H4DS_PANIC_INT_CORRUPTION);
            break;
    }

    return ntaken;
}

/****************************************************************************
NAME
        msg_start  -  match message layer from initial message byte

FUNCTION
        "b" is presumed to be the first byte of a received message.
        This function reports whether the byte is the first of an h4
        message, is the first of an h4ds message, or doesn't match.

RETURNS
        An identifier of the message layer for the received message that
        starts with "b".
*/

static msglayer msg_start(CsrUint8 b)
{
    switch (b)
    {
        /* Start of an h4 message? */
        case 0x02:              /* HCI ACL data. */
#ifdef H4DS_ONHOSTCONTROLLER
        case 0x01:              /* HCI command. */
#endif /* H4DS_ONHOSTCONTROLLER */
#ifdef H4DS_ONHOST
        case 0x04:              /* HCI event. */
#endif /* H4DS_ONHOST */
        case 0x03:              /* HCI SCO data. */
            return msglayer_h4;
        /* Start of the "sync" part of a h4ds message? */
        case 0x40:
            return msglayer_h4ds;
        default:
            break;
    }
    return msglayer_none;
}
