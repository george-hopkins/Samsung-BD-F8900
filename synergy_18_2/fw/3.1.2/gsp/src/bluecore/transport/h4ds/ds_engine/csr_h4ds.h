#ifndef CSR_H4DS_H__
#define CSR_H4DS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

/****************************************************************************
CONTAINS
        CsrH4DSInit  -  initialise h4ds library
        CsrH4DSTxMsgFrag  -  transmit an h4 message fragment to the peer
        CsrH4DSPump  -  drive the transmit state machine
        CsrH4DSAlertRxUartActivity  -  receive uart activity detected
        CsrH4DSAlertTxBufferEmpty  -  uart transmit buffers empty
        CsrH4DSTsyncTimedEvent  -  tsync timer timed event fired
        CsrH4DSTconfTimedEvent  -  tconf timer timed event fired
        CsrH4DSTwuTimedEvent  -  wake-up timed event fired
        CsrH4DSTflushTimedEvent  -  uart flush timed event fired
        CsrH4DSUartIdleTimedEvent  -  uart idle timed event fired
        CsrH4DSUartDeliverbytes  -  push uart bytes up into h4ds stack

DESCRIPTION
        The h4ds library provides an implementation of the h4ds protocol,
        defined in CSR document bcore-sp-013d and described in
        bcore-me-010d.  Logically, the h4ds library lies between a UART and a
        normal h4 engine (implementing the UART Transport Protocol, defined
        in section H:4 of version 1.1 of the Bluetooth Core Specification).


                            |                    ^
                     .......|.........hci........|.......
                            v                    |
                     ------------------------------------
                     |      .     h4 engine      .      |
                     ------------------------------------
                            |                    ^
                            v                    |
                     ------------------------------------
                     |      .    h4ds library    .      |
                     ------------------------------------
                            |                    ^
                            v                    |
                     ------------------------------------
                     |      .        UART        .      |
                     ------------------------------------
                            |   RTS        CTS   ^
                            v    |          ^    |
                           TxD   v          |   RxD


        When the system is carrying h4 traffic, it should be almost as
        efficient as a plain h4 implementation.  For example, it should be
        possible to transfer h4 messages to/from the UART using DMA.
        However, in practice, there must be extra interaction between the h4
        and h4ds code than is suggested by the diagram above, otherwise the
        system would be absurdly bloated and slow.

        This file describes the h4ds library's main external interfaces:


                CsrH4DSTxMsgFrag()
                CsrH4DSPump()             H4DS_DELIVERBYTES()
                        |                       ^
                        v                       |
                ------------------------------------------
                |                                        |
                |               h4ds library             | <- CsrH4DSInit()
                |                                        |
                ------------------------------------------
                        |                       ^
                        v                       |
             H4DS_UART_PUTBYTES()    CsrH4DSUartDeliverbytes()


        The library must first be initialised by a call to CsrH4DSInit().

        Transmit path:
            To send an h4 message, higher layer code calls the Boolean
            function CsrH4DSTxMsgFrag().  This either directly results in a
            corresponding call to H4DS_UART_PUTBYTES() with the same
            arguments, or the call fails.  The normal reason for the call
            failing is that the h4ds engine thinks the peer is asleep, in
            which case the call to CsrH4DSTxMsgFrag() gets the h4ds library to
            wake the peer.  This is driven by a sequence of calls to
            CsrH4DSPump().  When the peer has been woken, a subsequent call to
            CsrH4DSTxMsgFrag() will normally succeed.

        Receive path:
            For inbound messages, the UART driver code passes bytes from the
            wire into the library via calls to CsrH4DSUartDeliverbytes().
            Normally the bytes will form part of an h4 message, and they are
            passed directly to H4DS_DELIVERBYTES().  However, the bytes may
            hold messages from the peer's h4ds engine, in which case they are
            absorbed by the local h4ds engine.

        In the above diagram, function names in lower case are part of the
        library code.  Function names in upper case are macros within the
        code - the external environment must implement these according to the
        descriptions given in the various h4ds configuration header files.
        This convention is used throughout the h4ds code.

        The library contains no internal scheduler; it depends entirely on
        the above function calls (and a few other calls) to drive the code.
        The transmit path is driven ("down") by calls to CsrH4DSTxMsgFrag() and
        CsrH4DSPump(); these result in calls to H4DS_UART_PUTBYTES().
        Similarly, the receive path is driven ("up") entirely by calls to
        CsrH4DSUartDeliverbytes(), resulting in calls to H4DS_DELIVERBYTES().

        Although H4DS_UART_PUTBYTES() and H4DS_DELIVERBYTES() can refuse to
        accept bytes, it will normally be most efficient if bytes are not
        pushed into the library if it is known that the corresponding output
        function cannot accept them.  Thus, it is normal to call
        CsrH4DSUartDeliverbytes() only if it is known that H4DS_DELIVERBYTES()
        can accept bytes.  Similarly, it is normal to call CsrH4DSTxMsgFrag()
        or CsrH4DSPump() only when it is known that H4DS_UART_PUTBYTES() can
        accept bytes.

        An associated point: on the receive path, a call to
        CsrH4DSUartDeliverbytes() makes, at most, one call to
        H4DS_DELIVERBYTES().  Similarly, on the transmit path, a call to
        CsrH4DSTxMsgFrag() or CsrH4DSPump() makes, at most, only one call to
        H4DS_UART_PUTBYTES().  This should help external code to decide
        whether it is worth calling the library.

        A consequence of this arrangement that caught many programmers
        using abcsp: a call to CsrH4DSUartDeliverbytes() returns the *number
        of bytes consumed* - it isn't Boolean.  This means that if 100
        bytes are pushed into CsrH4DSUartDeliverbytes() and the function
        returns 20 then it is normal to push the remaining 80 bytes into
        a second call to CsrH4DSUartDeliverbytes(), etc.

        A key point about scheduling: there is no thread/mutex protection in
        the code.  Consequently, external code must make, at most, one call
        into the code at a time.  In particular, a call out from the library
        (e.g., H4DS_DELIVERBYTES()) must not directly result in a call back
        into the library (e.g., CsrH4DSTxMsgFrag()) - the external call must
        allow the original call to return and then make a second call into
        the library.

        The CsrH4DSPump() function is used only to send h4ds messages to the
        peer.  The library signals to external code that it should call
        CsrH4DSPump() at its earliest convenience via H4DS_REQ_PUMP(),
        described in config_h4ds.h.  I.e., the CsrH4DSPump() function does not
        attempt to pass to the peer any h4 message fragments recorded from
        CsrH4DSTxMsgFrag() calls - no such recording occurs.

        The library needs to learn when the UART has become idle.  When it
        enters its active state, able to pass traffic to and from the peer,
        it calls H4DS_START_UART_IDLE_TIMER().  When the timer fires external
        code is required to call CsrH4DSUartIdleTimedEvent().  This normally
        provokes a second call to H4DS_START_UART_IDLE_TIMER(), etc.  This
        means the h4ds code receives a regular alert while it is supposedly
        sending and receiving traffic.  In addition, whenever traffic passes
        through the library it sets a "uart_active" flag.  This flag is
        cleared on every uart_idle timer event.  If the flag is found to be
        FALSE when the timer fires, this is taken as an indication that the
        flow of traffic has become idle.  A call is then made to
        H4DS_UART_BUFFERS_EMPTY(), which asks the external code whether the
        UART's receive and transmit buffers are empty.  If this reports TRUE
        then the h4ds library assumes the link has fallen idle, and sends a
        You-May-Sleep message to the peer, allowing it to sleep.

        The library requires external code to provide other timers, used to
        send Wake-Up and other messages to the peer at regular intervals.
        These are requested with (macro) functions of the form
        H4DS_START_TWU_TIMER(); when the external code's timer fires it
        should call CsrH4DSTwuTimedEvent(), as described in config_timers.h.

        If the library determines that something is catastrophically wrong
        it calls H4DS_PANIC(), described in config_panic.h.  This should
        normally terminate the h4ds library's operation.

        The h4ds library code is peppered with calls to H4DS_ALERT() to tell
        external code of certain events.  Some events can be discarded, but
        of importance are:

          - H4DS_EVT_PEER_RESTART - indicates that the local h4ds library
            believes the peer h4ds engine has restarted.  The consequences
            are discussed in CSR document bcore-me-010.

          - H4DS_EVT_SYNC_LOSS - indicating that a corrupt h4ds message
            has been detected.  This should be treated as seriously as
            when the h4 engine detects that it has received a corrupt h4
            message.

          - H4DS_EVT_TXSM_END - indicates that the peer has not responded
            for an extended period (e.g., because the link is broken or
            the peer device has been switched off).

          - H4DS_EVT_PEER_AWAKE and H4DS_EVT_PEER_MAY_SLEEP indicate whether
            the h4ds library believes the peer to be awake.  While the peer
            is believed to be asleep, all calls to CsrH4DSTxMsgFrag() will
            fail.

          - Finally, what h4ds is all about, the event
            H4DS_EVT_LOCAL_DEVICE_MAY_SLEEP indicates whether the local
            device may enter its deep sleep state, in which it can disable
            its UART.

        Looking within the h4ds library, the code is logically split into
        two layers: link establishment (LE) and deep sleep (ds):


                CsrH4DSTxMsgFrag()
                CsrH4DSPump()             H4DS_DELIVERBYTES()
                        |                       ^
                        v                       |
                ------------------------------------------
                |       |                       |        |
                | -------------------------------------- |
                | |        link establishment          |-|--> rebooted
                | -------------------------------------- |
                |       |                       |        |
                | -------------------------------------- |
                | |             deep sleep             | |
                | -------------------------------------- |
                |       |                       |        |
                ------------------------------------------
                        |                       ^
                        v                       |
             H4DS_UART_PUTBYTES()    CsrH4DSUartDeliverbytes()


        The primary task of the link establishment engine is to emit a
        message signalling that the peer h4ds engine has restarted.

        The picture above bears only a passing resemblance to the code, which
        is structured more like:


              CsrH4DSTxMsgFrag()
              CsrH4DSPump()                       H4DS_DELIVERBYTES()
                     |                                   ^
                     v                                   |
            -------------------------------------------------------
            |        |                                   |        |
            | ---------------  -----------------  --------------- |
            | |             |  |     link      |  |             | |
            | |   transmit  |--| establishment |--|   receive   | |
            | |     path    |  |  (h4ds_le.c)  |  |    path     | |
            | |             |  -----------------  |             | |
            | | (h4ds_tx.c) |                     | (h4ds_rx.c) | |
            | |             |  -----------------  |             | |
            | |             |--|  deep sleep   |--|             | |
            | |             |  |  (h4ds_ds.c)  |  |             | |
            | ---------------  -----------------  --------------- |
            |        |                                   |        |
            -------------------------------------------------------
                     |                                   ^
                     v                                   |
            H4DS_UART_PUTBYTES()            CsrH4DSUartDeliverbytes()


        The two core state machines, link establishment and deep sleep, are
        driven by events, the vast majority of which come from matching
        messages on the receive path.  The bulk data flows through the
        transmit and receive paths.  These two elements also synthesise and
        analyse the seven h4ds messages.
*/

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
NAME
        CsrH4DSInit  -  initialise h4ds library

FUNCTION
        Initialises the state of the h4ds library.  This resets all of its
        internal state, including cancelling any pending timed event timers
        and alert requests.

        This must be called before all other functions in the h4ds library,
        including all of the h4ds functions described in this file.

        This may be called at any time to reinitialise the state of the
        library.
*/

void CsrH4DSInit(void);


/****************************************************************************
NAME
        CsrH4DSTxMsgFrag  -  transmit an h4 message fragment to the peer

FUNCTION
        Sends the "len" byte message fragment at "buf" to the UART for
        transmission to the peer.  If the fragment is the last of a complete
        message then "last_frag" must be non-zero, else it must be zero.

        This function is designed to send h4 messages to the peer, i.e., each
        complete message consists of an hci message prepended by an h4
        channel identification byte.  However, h4 commands can be up to 259
        bytes long (h4 header byte, 3 byte hci command header and up to 255
        parameter bytes), and finding a block of RAM of this size is a
        problem for some grim processors.  This function has been shaped to
        allow a long hci command to be held as a sequence of fragments and
        for these to be sent to the peer in turn.  (Coding on BlueCore has
        taught us that using lots of small lumps of memory gives the best
        flexibility when RAM is scarce.)

        To send an h4 message to the peer, the caller is required to make the
        sequence of calls to this function, each carrying a fragment of the
        complete message.  The fragments must be provided in sequence.  The
        last fragment must have its "last_frag" flag set to non-zero.

        The function returns zero if it failed to send the fragment, e.g.,
        because the peer is currently judged to be asleep.  Once one message
        fragment has been accepted by this function the caller is obliged to
        send the rest of the message's fragments.  For example, if an hci h4
        message is to be sent as two fragments, and the first is accepted by
        this function then the caller is obliged to keep submitting the
        second fragment until this function accepts it.

RETURNS
        Non-zero if the fragment was passed to the UART for transmission, else
        zero.

        If the function returns zero, the h4ds library will normally have
        started actions to wake the peer; these will need calls to
        CsrH4DSPump() to (re-)establish the path.  It is presumed that the
        caller will attempt to send the h4 message again when the path is
        open.

*/

CsrUint32 CsrH4DSTxMsgFrag(const CsrUint8 *buf,
    CsrUint32 len, CsrUint32 last_frag);


/****************************************************************************
NAME
        CsrH4DSPump  -  drive the transmit state machine

FUNCTION
        If the transmit path of the h4ds library wishes to send (part of) a
        message to the peer, then this is sent.  Alternatively, this drives
        the logic of the library's transmit path.

        This function is intended to be called by external code in response
        to a call to H4DS_PUMP_REQ().
*/

void CsrH4DSPump(void);


/****************************************************************************
NAME
        CsrH4DSAlertRxUartActivity  -  receive uart activity detected

FUNCTION
        Function to be called when the alert armed by a call to
        H4DS_ALERT_ARM_RX_UART_ACTIVITY() fires.

        This function tells the h4ds library's deep sleep state machine that
        receive activity has been detected on the UART.
*/

void CsrH4DSAlertRxUartActivity(void);


/****************************************************************************
NAME
        CsrH4DSAlertTxBufferEmpty  -  uart transmit buffers empty

FUNCTION
        Function to be called when the alert armed by a call to
        H4DS_ALERT_ARM_TX_BUFFER_EMPTY() determines that the buffers between
        H4DS_UART_PUTBYTES() and the UART link wire are perceived to be
        empty.  This includes buffering between the h4ds code and the UART,
        plus buffering within the UART.

        Before the h4ds code attempts to enter deep sleep it uses this alert
        to determine if the transmit buffers are empty.  It does this because
        the buffers may contain a You-May-Sleep message just sent to the
        peer, and if this doesn't reach the peer then the peer will be unable
        to sleep.  It is acceptable for this function to lie - i.e., to
        report that the buffers are empty when it's not certain that this is
        true; this allows the local device to sleep, even if the peer has
        to stay awake.

        On systems where it can be difficult to ensure all transmit buffers
        are empty, e.g., Windows, it may be acceptable (or even necessary) to
        call this function when it can be *guessed* that the You-May-Sleep
        message has been sent.  E.g., working from the length of the
        You-May-Sleep message, the Baud rate and checks to ensure that the
        peer has not been asserting flow control for the period.
*/

void CsrH4DSAlertTxBufferEmpty(void);


/****************************************************************************
NAME
        CsrH4DSTsyncTimedEvent  -  tsync timer timed event fired

FUNCTION
        Function to be called when the timer started by a call to
        H4DS_START_TSYNC_TIMER() fires.

        This function tells the h4ds library's link establishment state
        machine that the Tsync timer has fired.
*/

void CsrH4DSTsyncTimedEvent(void);


/****************************************************************************
NAME
        CsrH4DSTconfTimedEvent  -  tconf timer timed event fired

FUNCTION
        Function to be called when the timer started by a call to
        H4DS_START_TCONF_TIMER() fires.

        This function tells the h4ds library's link establishment state
        machine that the Tconf timer has fired.
*/

void CsrH4DSTconfTimedEvent(void);


/****************************************************************************
NAME
        CsrH4DSTwuTimedEvent  -  wake-up timed event fired

FUNCTION
        Function to be called when the timer started by a call to
        H4DS_START_TWU_TIMER() fires.

        This function tells the h4ds library's deep sleep state machine that
        the Twu timer has fired.
*/

void CsrH4DSTwuTimedEvent(void);


/****************************************************************************
NAME
        CsrH4DSTflushTimedEvent  -  uart flush timed event fired

FUNCTION
        Function to be called when the timer started by a call to
        H4DS_START_TFLUSH_TIMER() fires.

        This function tells the h4ds library's deep sleep state machine that
        the Tflush timer has fired.
*/

void CsrH4DSTflushTimedEvent(void);


/****************************************************************************
NAME
        CsrH4DSUartIdleTimedEvent  -  uart idle timed event fired

FUNCTION
        Function to be called when the timer started by a call to
        H4DS_START_UART_IDLE_TIMER() fires.

        This function tells the h4ds library's deep sleep state machine that
        the timer has fired.
*/

void CsrH4DSUartIdleTimedEvent(void);


/****************************************************************************
NAME
        CsrH4DSUartDeliverbytes  -  push uart bytes up into h4ds stack

FUNCTION
        Pushes the "len" bytes in the buffer at "buf" up into the h4ds
        stack.  The bytes are presumed to have been received directly
        from the UART.

        If the bytes complete an hci message, this is passed up out of the
        h4ds library via H4DS_DELIVERBYTES().

        Bytes in "buf" may also be consumed by the h4ds library: h4ds
        messages from the peer.

        A single call to CsrH4DSUartDeliverbytes() makes, at most, one call to
        H4DS_DELIVERBYTES().

RETURNS
        The number of bytes consumed from "buf".
*/

CsrUint32 CsrH4DSUartDeliverbytes(const CsrUint8 *buf,
    CsrUint32 len, CsrUint8 chan);

#ifdef __cplusplus
}
#endif

#endif
