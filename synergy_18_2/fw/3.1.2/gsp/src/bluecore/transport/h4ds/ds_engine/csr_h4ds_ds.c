/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/****************************************************************************
CONTAINS
        CsrH4DSFsmInit  -  initialise the h4ds deep sleep state machine
        CsrH4DSHlTxMsgAvail  -  higher layer transmit message available
        h4ds_uart_rx_activity  -  uart rx activity detected
        CsrH4DSWuMsgRxed  -  wake-up message received
        CsrH4DSIaMsgRxed  -  i'm-awake message received
        CsrH4DSYmsMsgRxed  -  you-may-sleep message received
        CsrH4DSTwuTimedEvent  -  wake-up timed event fired
        CsrH4DSTflushTimedEvent  -  tflush timed event fired
        CsrH4DSUartIdleTimedEvent  -  uart idle timed event fired
        CsrH4DSAlertRxUartActivity  -  uart receive activity detected
        CsrH4DSAlertTxBufferEmpty  -  uart transmit buffers empty
        h4ds_reset_uart_idle_timer  -  ask to be told of uart traffic
        CsrH4DSUartActive  -  inform ds sm of uart traffic
        CsrH4DSCanTransmit  -  does the h4ds deep sleep engine allows tx
        CsrH4DSCanReceive  -  does the h4ds deep sleep engine allows rx
        CsrH4DSTxPumpCalled  -  signal call has been made to h4dx_tx_pump

        h4ds_ds_sm  -  the h4ds deep sleep state machine

DESCRIPTION
        This file holds the h4ds deep sleep state machine.

        The core of the file is the state machine in h4ds_ds_sm().  Other
        h4ds code passes event messages into the state machine via a set of
        access functions: CsrH4DSWuMsgRxed(), etc.

        The state machine uses a pair of timers that must be provided by
        external code.  These are described in config_timers.h.

        External code must implement a function for macro
        H4DS_UART_BUFFERS_EMPTY() to report if the UART's receive and
        transmit buffers are empty.  This is described in config_h4ds.h.  The
        design will survive if this function doesn't provide a completely
        honest report, but dubious information will produce an inefficient
        system.

        The state machine uses an activity monitor that must be provided by
        external code.  The monitor calls into the state machine if any UART
        receive activity is detected.  This is described in config_alerts.h.
        The monitor is only used when the UART is believe to be idle, so the
        implementation can be quite inefficient.

        The state machine signals that it requires the h4ds code to transmit
        Wake-Up, I'm-Awake and You-May-Sleep messages to the peer with calls
        to CsrH4DSReqTxDsWu(), etc.

        The state machine uses one of its timers to derive knowledge of when
        the UART goes idle.  This requires h4ds code to call
        h4ds_reset_uart_idle_timer() whenever they send or receive h4 or h4ds
        link establishment messages to/from the UART.
*/

#include "csr_h4ds_private.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#include "csr_formatted_io.h"
#endif

extern CsrBool CsrSendingH4Msg;

/* h4ds deep sleep state machine states. */
enum ds_state_enum
{
    state_init = 0,                    /* Initialise state machine. */
    state_flush_tx = 1,                /* Flush transmit uart buffers. */
    state_torpid = 2,                  /* Local device may sleep. */
    state_drowsy = 3,                  /* Waking up. */
    state_peer_idle = 4,               /* Peer is not responding. */
    state_awake = 5,                   /* Send and receive HL traffic. */
    state_awake_pass_rx = 6,           /* Only receive HL traffic. */
    state_awake_pass_tx = 7            /* Only transmit HL traffic. */
};
typedef enum ds_state_enum ds_state;

/* The deep sleep state machine's state. */
static ds_state state;


/* Messages to the h4ds deep sleep state machine. */
enum dsmsgid_enum
{
    dsmsgid_none = 0,                  /* No message. */
    dsmsgid_hl_tx_msg_avail = 1,       /* Higher layer msg available. */
    dsmsgid_ia_rxed = 2,               /* I'm-Awake msg received. */
    dsmsgid_wu_rxed = 3,               /* Wake-Up msg received. */
    dsmsgid_yms_rxed = 4,              /* You-May-Sleep msg received. */
    dsmsgid_twu_timeout = 5,           /* Wake-Up timer fired. */
    dsmsgid_tflush_timeout = 6,        /* UART tx timeout timer fired. */
    dsmsgid_uart_idle_timeout = 7,     /* UART idle timer fired. */
    dsmsgid_rx_uart_activity = 8,      /* UART rx activity detected. */
    dsmsgid_tx_buffer_empty = 9        /* Tx UART buffers empty. */
};
typedef enum dsmsgid_enum dsmsgid;


/* The number of Wake-Up messages sent in a train. */
static CsrUint16 nwu;

/* Flag that is set whenever UART activity is reported. */
static CsrBool uart_active;

/* Flag to check for an empty UART transmit buffer. */
static CsrBool tx_buffer_empty_check_pending;

/* Forward references. */
static void h4ds_ds_sm(dsmsgid msg);
static void state_drowsy_prelude(void);
static void state_awake_prelude(void);
static void state_torpid_prelude(void);
static void state_flush_tx_prelude(void);
static void h4ds_reset_uart_idle_timer(void);


/****************************************************************************
NAME
        CsrH4DSFsmInit  -  initialise the h4ds deep sleep state machine
*/

void CsrH4DSFsmInit(void)
{
    /* Initialise the transmit state machine. */

    state = state_init;
    h4ds_ds_sm(dsmsgid_none);
}

/****************************************************************************
NAME
        CsrH4DSHlTxMsgAvail  -  higher layer transmit message available
*/

void CsrH4DSHlTxMsgAvail(void)
{
    h4ds_ds_sm(dsmsgid_hl_tx_msg_avail);
}

/****************************************************************************
NAME
        CsrH4DSWuMsgRxed  -  wake-up message received
*/

void CsrH4DSWuMsgRxed(void)
{
    h4ds_ds_sm(dsmsgid_wu_rxed);
}

/****************************************************************************
NAME
        CsrH4DSIaMsgRxed  -  i'm-awake message received
*/

void CsrH4DSIaMsgRxed(void)
{
    h4ds_ds_sm(dsmsgid_ia_rxed);
}

/****************************************************************************
NAME
        CsrH4DSYmsMsgRxed  -  you-may-sleep message received
*/

void CsrH4DSYmsMsgRxed(void)
{
    h4ds_ds_sm(dsmsgid_yms_rxed);
}

/****************************************************************************
NAME
        CsrH4DSTwuTimedEvent  -  wake-up timed event fired
*/

void CsrH4DSTwuTimedEvent(void)
{
    h4ds_ds_sm(dsmsgid_twu_timeout);
}

/****************************************************************************
NAME
        CsrH4DSTflushTimedEvent  -  tflush timed event fired
*/

void CsrH4DSTflushTimedEvent(void)
{
    h4ds_ds_sm(dsmsgid_tflush_timeout);
}

/****************************************************************************
NAME
        CsrH4DSUartIdleTimedEvent  -  uart idle timed event fired
*/

void CsrH4DSUartIdleTimedEvent(void)
{
    h4ds_ds_sm(dsmsgid_uart_idle_timeout);
}

/****************************************************************************
NAME
        CsrH4DSAlertRxUartActivity  -  uart receive activity detected
*/

void CsrH4DSAlertRxUartActivity(void)
{
    h4ds_ds_sm(dsmsgid_rx_uart_activity);
}

/****************************************************************************
NAME
        CsrH4DSAlertTxBufferEmpty  -  uart transmit buffers empty

IMPLEMENTATION NOTE
        This function should be called when external code determines that the
        UART's transmit buffer is empty.  The call is requested by the deep
        sleep code calling H4DS_ALERT_ARM_TX_BUFFER_EMPTY().

        However, the h4ds library may be waiting to send one of the seven
        h4ds messages (you-may-sleep, etc.).  As far as the deep sleep state
        machine is concerned, these have been sent, but in reality the
        requests to send them may still be lurking in the transmit path.
        These pending requests should be counted as part of the "uart"
        transmit queue.

        Consequently, this function only tells the deep sleep state machine
        that the "uart" transmit buffers are empty if there are no pending
        messages in the transmit path.  Otherwise it sets a flag that causes
        H4DS_ALERT_ARM_TX_BUFFER_EMPTY() to be called again when the external
        code next calls CsrH4DSPump() (which would allow the transmit path's
        pending messages to be sent).
*/

void CsrH4DSAlertTxBufferEmpty(void)
{
    if (CsrH4DsTxPathClear())
    {
        h4ds_ds_sm(dsmsgid_tx_buffer_empty);
    }
    else
    {
        tx_buffer_empty_check_pending = TRUE;
    }
}

/****************************************************************************
NAME
        CsrH4DSTxPumpCalled  -  signal call has been made to h4dx_tx_pump

IMPLEMENTATION NOTE
        See CsrH4DSAlertTxBufferEmpty.
*/

void CsrH4DSTxPumpCalled()
{
    if (tx_buffer_empty_check_pending)
    {
        tx_buffer_empty_check_pending = FALSE;
        H4DS_ALERT_ARM_TX_BUFFER_EMPTY();
    }
}

/****************************************************************************
NAME
        h4ds_ds_sm  -  the h4ds deep sleep state machine

FUNCTION
        Feeds the message "msg" into the h4ds transmit state machine.
*/

static void h4ds_ds_sm(dsmsgid msg)
{
#ifdef CSR_LOG_ENABLE
    CsrCharString logmsg[100];
#endif

    switch (state)
    {
        case state_init:              /* Initialise local state machine. */
            /* Kill the state machine's timers, in case they're running. */

            H4DS_CANCEL_TWU_TIMER();
            H4DS_CANCEL_TFLUSH_TIMER();
            H4DS_CANCEL_UART_IDLE_TIMER();

            /* Lose interest in being told when any UART receive activity
            is detected, and in when the UART's transmit buffers are
            empty. */

            H4DS_ALERT_CANCEL_RX_UART_ACTIVITY();
            H4DS_ALERT_CANCEL_TX_BUFFER_EMPTY();

            /* Initially assume UART is idle. */
            uart_active = FALSE;

            /* Remove any pending req to check for empty UART tx buf. */
            tx_buffer_empty_check_pending = FALSE;

            /* Allow the local device to sleep until either higher level
            traffic is ready to be transmitted (e.g., a link
            establishment message) or until we receive traffic from the
            peer. */

            state_torpid_prelude();
            break;
        case state_flush_tx:          /* Flush transmit uart. */
            switch (msg)
            {
                case dsmsgid_tx_buffer_empty:
                    H4DS_CANCEL_TFLUSH_TIMER();
                    state_torpid_prelude();
                    break;
                case dsmsgid_tflush_timeout:
                    H4DS_ALERT_CANCEL_TX_BUFFER_EMPTY();
                    state_torpid_prelude();
                    break;
                case dsmsgid_hl_tx_msg_avail:
                    H4DS_ALERT_CANCEL_TX_BUFFER_EMPTY();
                    H4DS_CANCEL_TFLUSH_TIMER();
                    state_drowsy_prelude();
                    break;
                case dsmsgid_wu_rxed:
                    H4DS_ALERT_CANCEL_TX_BUFFER_EMPTY();
                    H4DS_CANCEL_TFLUSH_TIMER();
                    state_drowsy_prelude();
                    CsrH4DSReqTxDsIa();
                    break;
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_ds.c: function h4ds_ds_sm() received message: %#x in state: %#x", msg, state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        case state_torpid:            /* Local device may sleep. */
            switch (msg)
            {
                case dsmsgid_hl_tx_msg_avail:
                    H4DS_EVENT(H4DS_EVT_LOCAL_DEVICE_MAY_NOT_SLEEP);
                    state_drowsy_prelude();
                    break;
                case dsmsgid_wu_rxed:
                    H4DS_EVENT(H4DS_EVT_LOCAL_DEVICE_MAY_NOT_SLEEP);
                    state_drowsy_prelude();
                    CsrH4DSReqTxDsIa();
                    break;
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_ds.c: function h4ds_ds_sm() received message: %#x in state: %#x", msg, state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        case state_drowsy:              /* Waking up. */
            switch (msg)
            {
                case dsmsgid_wu_rxed:
                    /* Send an I'm-Awake message to the peer. */
                    CsrH4DSReqTxDsIa();
                    break;
                case dsmsgid_twu_timeout:
                    if (nwu >= H4DS_MAX_WU_MSGS)
                    {
                        /* Give up waiting for the peer to respond to
                        our Wake-Up messages, and go idle.  In the
                        idle state we'll only start to do things
                        again if the UART receives traffic. */

                        H4DS_ALERT_ARM_RX_UART_ACTIVITY();

                        /* The link appears to be dead, so there's no
                        point in allowing the local link
                        establishment state machine to continue to
                        emit its messages every 250ms.  Pause le, or
                        we'll never get any sleep. */

                        CsrH4DSLePause();

                        /* We let the local machine sleep if the
                        peer is unresponsive - no point in wasting
                        power. */

                        H4DS_EVENT(H4DS_EVT_LOCAL_DEVICE_MAY_SLEEP);

                        state = state_peer_idle;
                    }
                    else
                    {
                        /* Send a Wake-Up message to the peer. */
                        CsrH4DSReqTxDsWu();

                        /* Continue train of Wake-Up msgs. */
                        H4DS_START_TWU_TIMER();

                        /* Bump the counter of wake-up messages. */
                        ++nwu;
                    }
                    break;
                case dsmsgid_ia_rxed:
                    /* We've received an I'm-Awake message.  Take this
                    to mean that the peer is awake. */
                    H4DS_EVENT(H4DS_EVT_PEER_AWAKE);

                    /* Cancel the train of Wake-Up messages. */
                    H4DS_CANCEL_TWU_TIMER();

                    state_awake_prelude();
                    break;
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_ds.c: function h4ds_ds_sm() received message: %#x in state: %#x", msg, state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        case state_peer_idle:           /* Peer is not responding. */
            switch (msg)
            {
                case dsmsgid_rx_uart_activity:
                    CsrH4DSLeUnpause();
                    H4DS_EVENT(H4DS_EVT_LOCAL_DEVICE_MAY_NOT_SLEEP);
                    state_drowsy_prelude();
                    break;
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_ds.c: function h4ds_ds_sm() received message: %#x in state: %#x", msg, state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        case state_awake:               /* Send and receive traffic. */
            switch (msg)
            {
                case dsmsgid_wu_rxed:
                    /* Send an I'm-Awake message to the peer. */
                    CsrH4DSReqTxDsIa();
                    h4ds_reset_uart_idle_timer();
                    break;
                case dsmsgid_yms_rxed:
                    /* Don't call h4ds_reset_uart_idle_timer() for yms. */
                    state = state_awake_pass_tx;
                    break;
                case dsmsgid_uart_idle_timeout:
                    if (CsrSendingH4Msg == FALSE)
                    {
                        if (!uart_active && H4DS_UART_BUFFERS_EMPTY())
                        {
                            /* Send a You-May-Sleep message to the peer. */
                            CsrH4DSReqTxDsYms();
                            state = state_awake_pass_rx;
                        }
                        else
                        {
                            uart_active = FALSE;
                            h4ds_reset_uart_idle_timer();
                        }
                    }
                    else
                    {
                        h4ds_reset_uart_idle_timer();
                    }
                    break;
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_ds.c: function h4ds_ds_sm() received message: %#x in state: %#x", msg, state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        case state_awake_pass_rx:               /* Only receive traffic. */
            switch (msg)
            {
                case dsmsgid_hl_tx_msg_avail:
                    state_drowsy_prelude();
                    break;
                case dsmsgid_wu_rxed:
                    state_drowsy_prelude();
                    CsrH4DSReqTxDsIa();
                    break;
                case dsmsgid_yms_rxed:
                    state_flush_tx_prelude();
                    break;
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_ds.c: function h4ds_ds_sm() received message: %#x in state: %#x", msg, state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        case state_awake_pass_tx:               /* Only transmit traffic. */
            switch (msg)
            {
                case dsmsgid_wu_rxed:
                    /* Send an I'm-Awake message to the peer. */
                    CsrH4DSReqTxDsIa();
                    h4ds_reset_uart_idle_timer();
                    state = state_awake;
                    break;
                case dsmsgid_uart_idle_timeout:
                    if (CsrSendingH4Msg == FALSE)
                    {
                        if (!uart_active && H4DS_UART_BUFFERS_EMPTY())
                        {
                            /* Send a You-May-Sleep message to the peer. */
                            state_flush_tx_prelude();
                            CsrH4DSReqTxDsYms();
                        }
                        else
                        {
                            uart_active = FALSE;
                            h4ds_reset_uart_idle_timer();
                        }
                    }
                    else
                    {
                        h4ds_reset_uart_idle_timer();
                    }
                    break;
                default:
                {
#ifdef CSR_LOG_ENABLE
                    CsrSnprintf(logmsg, sizeof(logmsg), "h4ds_ds.c: function h4ds_ds_sm() received message: %#x in state: %#x", msg, state);
                    CSR_LOG_TEXT(logmsg);
#endif
                    break;
                }
            }
            break;
        default:
            H4DS_PANIC(H4DS_PANIC_INT_CORRUPTION);
            break;
    }
}

/* State preludes - snipped out to save code. */

static void state_flush_tx_prelude(void)
{
    tx_buffer_empty_check_pending = FALSE;
    H4DS_ALERT_ARM_TX_BUFFER_EMPTY();
    H4DS_START_TFLUSH_TIMER();
    state = state_flush_tx;
}

static void state_torpid_prelude(void)
{
    H4DS_EVENT(H4DS_EVT_LOCAL_DEVICE_MAY_SLEEP);
    state = state_torpid;
}

static void state_awake_prelude(void)
{
    /* In the awake state we'll be interested in the UART
    becoming idle. */

    uart_active = FALSE;
    h4ds_reset_uart_idle_timer();
    state = state_awake;
}

static void state_drowsy_prelude(void)
{
    /* Send a Wake-Up message to the peer. */
    CsrH4DSReqTxDsWu();

    /* Start timer to send train of Wake-Up msgs. */
    H4DS_START_TWU_TIMER();

    /* Init counter of number of txed Wake-Up msgs. */
    nwu = 1;

    state = state_drowsy;
}

/****************************************************************************
NAME
        h4ds_reset_uart_idle_timer  -  ask to be told of uart traffic
*/

static void h4ds_reset_uart_idle_timer(void)
{
    H4DS_CANCEL_UART_IDLE_TIMER();
    H4DS_START_UART_IDLE_TIMER();
}

/****************************************************************************
NAME
        CsrH4DSUartActive  -  inform ds sm of uart traffic
*/

void CsrH4DSUartActive(void)
{
    uart_active = TRUE;
}

/****************************************************************************
NAME
        CsrH4DSCanTransmit  -  does the h4ds deep sleep engine allows tx
*/

CsrBool CsrH4DSCanTransmit(void)
{
    return state == state_awake || state == state_awake_pass_tx;
}

/****************************************************************************
NAME
        CsrH4DSCanReceive  -  does the h4ds deep sleep engine allows rx
*/

CsrBool CsrH4DSCanReceive(void)
{
    return state == state_awake || state == state_awake_pass_rx
           || state == state_drowsy;
}
