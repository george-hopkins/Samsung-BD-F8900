#ifndef CSR_ABCSP_H__
#define CSR_ABCSP_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_abcsp_txmsg.h"
#include "csr_abcsp_txslip.h"
#include "csr_abcsp_rx.h"
#include "csr_abcsp_txrx.h"
#include "csr_abcsp_le.h"
#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrBcspLto);

/* #define BT_DBG printf */
#define BT_DBG

extern MessageStructure CsrAbcspRxMsgStruct;

#define ABCSP_TXMSG             void
#define ABCSP_RXMSG             void

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    struct txmsg_state txmsg;
    /* Slip encoders state */
    struct txslip_state txslip;
    /* Slip decoder's state. */
    struct rxslip_state rxslip;
    /* Database of info common to tx and rx paths. */
    struct txrx_info txrx;
    struct le_state  le;
    CsrSchedBgint    bgint_pumptxmsg;
    CsrSchedBgint    bgint_rx;
    void            *uartHandle;
#ifndef CSR_BCSP_RETRANSMISSION_TIMER
    CsrTime retransmitTimerPeriod;
#endif
#if CSR_BCSP_RETRANSMISSION_JITTER > 0
    void *randomState;
#endif
} abcsp;


extern abcsp CsrAbcspInstanceData;

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
/* Assign a pointer to a measurement array to this variable to capture
   time spent for this module (the time spent will be lost otherwise) before
   the call to CsrAbcspInit. */
extern CsrUint32 *CsrAbcspMeasurements;
#endif

/****************************************************************************
NAME
        CsrAbcspInit  -  initialise abcsp library

FUNCTION
    Initialises the state of the abcsp library.  This resets all of its
    internal state, including FREE()ing held memory, cancelling any
    pending timed event timers and aborting the transfer of any messages
    in progress.

    This must be called before all other functions in the abcsp library,
    including all of the abcsp functions described in this file.

        This may be called at any time to reinitialise the state of the
        library.
*/
void CsrAbcspInit(void);

/****************************************************************************
NAME
        CsrAbcspDeinit  -  de-initialise abcsp library

FUNCTION
*/

void CsrAbcspDeinit(void);


/****************************************************************************
NAME
        CsrAbcspMsgSend  -  set up message for sending to the uart

FUNCTION
    Passes the message "msg" into the library ready for transmission.
    The actual message translation and transmission is performed by
    CsrAbcspPumpTxMsgs(), so this call really just parks "msg" within the
    library.

    The message is sent on BCSP channel "chan".  If "rel" is zero then it
    is sent as an unreliable datagram, else it is sent as a reliable
    datagram.

    Ownership of the memory referenced via "msg" is retained by the
    caller.  The caller must not alter the contents of "msg" until the
    abcsp library signals that it has completed its transmission (by
    calling ABCSP_TXMSG_DONE()) or until the library has been
    reinitialised.

RETURNS
        1 if the message was accepted for transmission, else 0.

    The function returns 0 if the initialisation function has not been
    called, if message transmission is currently being prevented (choke),
    if any argument is unacceptable or if the queue that would accept the
    message is full.
*/

CsrUint32 CsrAbcspMsgSend(ABCSP_TXMSG *msg,
    CsrUint8 chan, CsrUint32 rel);


/****************************************************************************
NAME
        CsrAbcspPumpTxMsgs  -  send message to the uart

FUNCTION
    Processes messages posted into the abcsp library by CsrAbcspMsgSend(),
    translating them to their wire SLIP form and sending this to the
    UART.  One or more calls to this function will be required to send
    each ABCSP_TXMSG.

    This function must not be called unless it is known that
    ABCSP_UART_SENDBYTES() can accept a block of bytes.

    One call to CsrAbcspPumpTxMsgs() makes, at most, one call to
    ABCSP_UART_SENDBYTES().

    The function's return value can be used to decide on when next to
    call the function - it is an aid to the library's scheduling.  If it
    returns 0 then all immediate work on the library's transmit path is
    finished.  It returns 1 to indicate that there is work to be done as
    soon as possible.

RETURNS
    1 to indicate that the transmit path of the abcsp library still has
    work to do immediately, else 0.
*/

CsrUint32 CsrAbcspPumpTxMsgs(void);


/****************************************************************************
NAME
        CsrAbcspBcspTimedEvent  -  transmit path's timed event

FUNCTION
    The abcsp library's transmit path uses a single timed event
    controlled with ABCSP_START_BCSP_TIMER() and
    ABCSP_CANCEL_BCSP_TIMER().  If the timer fires it must call this
    function.

    The timer event provokes a retransmission of all unacknowledged
    transmitted BCSP messages.
*/

void CsrAbcspBcspTimedEvent(void);


/****************************************************************************
NAME
        CsrAbcspTshyTimedEvent  -  report a tshy timeout event to the fsm

FUNCTION
        The bcsp link establishment engine uses two timers.  The first is
        controlled by ABCSP_START_TSHY_TIMER() and ABCSP_CANCEL_TSHY_TIMER().
        If this timer fires it must call this function.

        This function sends sends a Tshy timeout event into the bcsp-le
        state machine.
*/

void CsrAbcspTshyTimedEvent(void);


/****************************************************************************
NAME
        CsrAbcspTconfTimedEvent  -  report a tconf timeout event to the fsm

FUNCTION
    The bcsp link establishment engine uses two timers.  The second is
    controlled by ABCSP_START_TCONF_TIMER() and
    ABCSP_CANCEL_TCONF_TIMER().  If this timer fires it must call this
    function.

        This function sends sends a Tconf timeout event into the bcsp-le
        state machine.
*/

void CsrAbcspTconfTimedEvent(void);

void CsrAbcspRegisterUartHandle(void *handle);

#ifdef __cplusplus
}
#endif

#endif
