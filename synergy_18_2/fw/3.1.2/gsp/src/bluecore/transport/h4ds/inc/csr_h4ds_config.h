#ifndef CSR_H4DS_CONFIG_H__
#define CSR_H4DS_CONFIG_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_h4ds.h"
#include "csr_h4ds_timers.h"
#include "csr_h4ds_event.h"
#include "csr_h4ds_tx_handle.h"
#include "csr_h4ds_pump_msg.h"
#include "csr_h4_int_handle.h"
#include "csr_h4_rx.h"
#include "csr_h4ds_low_level_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A Wake-Up message starts with a fixed 12-byte sequence, designed to allow
BlueCore-ROM's "auto-baud" logic to work out the host's baud rate.  This
12-byte sequence is repeated H4DS_N_WU_PREAMBLE_CHUNKS times.

(A Wake-Up message continues with a fixed 12-byte sequence that contains four
0x00 bytes, to ensure the receiving UART is locked onto the byte stream
correctly.  The message then finishes with a fixed 12-byte sequence that is
unique to the Wake-Up message - this contains the ASCII sequence {'w', 'u',
'\0', '\0'}.)

The value of H4DS_N_WU_PREAMBLE_CHUNKS must be chosen so that the resulting
complete Wake-Up message is long enough to wake the peer from its deep
sleep state, for it to enable its UART and for it to recognise the final
12-byte sequence ("wu").  The value of H4DS_N_WU_PREAMBLE_CHUNKS therefore
depends on the baud rate, and must be adjusted to suit local conditions.

For example, for 115k2 baud, if it takes 10ms to wake the peer, a reasonable
value for H4DS_N_WU_PREAMBLE_CHUNKS is 10.  ((115200 * 0.01)/(12 * 10)). */

#define H4DS_N_WU_PREAMBLE_CHUNKS           (10)


/* When the local device wants to wake the peer it emits a stream of Wake-Up
messages.  If the peer is not responding, e.g., because the UART cable has
been unplugged, it makes sense to give up sending Wake-Up messages until the
peer shows some sign of life.  H4DS_MAX_WU_MSGS sets the number of Wake-Up
messages sent to the peer before giving up.  This value should reflect (i)
the time taken for the peer to wake up relative to the local device, (ii) the
Twu period and (iii) the period of a Wake-Up message.  The value is compared
against CsrUint16 nwu in h4ds_ds.c, so the code should be changed if the #define
value is made huge. */

#define H4DS_MAX_WU_MSGS                    (50)

/* These are functions mapped to either H4DS wrapper functions or directly to
external code. */

#define H4DS_START_TSYNC_TIMER()            CsrH4DSStartTsyncTimer()
#define H4DS_CANCEL_TSYNC_TIMER()           CsrH4DSCancelTsyncTimer()
#define H4DS_START_TCONF_TIMER()            CsrH4DSStartTconfTimer()
#define H4DS_CANCEL_TCONF_TIMER()           CsrH4DSCancelTconfTimer()
#define H4DS_START_TWU_TIMER()              CsrH4DSStartTwuTimer()
#define H4DS_CANCEL_TWU_TIMER()             CsrH4DSCancelTwuTimer()
#define H4DS_START_TFLUSH_TIMER()           CsrH4DSStartTflushTimer()
#define H4DS_CANCEL_TFLUSH_TIMER()          CsrH4DSCancelTflushTimer()
#define H4DS_START_UART_IDLE_TIMER()        CsrH4DSStartUartIdleTimer()
#define H4DS_CANCEL_UART_IDLE_TIMER()       CsrH4DSCancelUartIdleTimer()

#define H4_UART_SENDBYTES(b, l, f)          CsrH4DSTxMsgFrag((b), (l), (f))
#define H4DS_UART_PUTBYTES(b, l)            CsrH4DSUartBytesSend((b), (CsrUint32) (l))
#define H4_REQ_PUMPTXMSGS(t)                CsrH4PumpTxReq()
#define H4DS_PUMP_REQ()                     CsrH4DSPumpTxMsgsReq()

#define H4DS_RX_PUTMESSAGE(b, c, l)         CsrH4DSUartDeliverbytes((CsrUint8 *) (b), (CsrUint32) (l), (CsrUint8) (c))
#define H4DS_DELIVERBYTES(m, l, c, f)       CsrH4RxMsgPut((CsrUint8 *) (m), (c), (l), (CsrBool *) (f))

#define H4DS_EVENT(n)                       CsrH4DSEvent(n)
#define H4DS_PANIC(p)                       CsrH4DSPanic(p)

#define H4DS_UART_BUFFERS_EMPTY()           CsrH4DSUartBuffersEmpty()
#define H4DS_ALERT_ARM_RX_UART_ACTIVITY()   CsrH4DSAlertArmRxUartActivity()
#define H4DS_ALERT_CANCEL_RX_UART_ACTIVITY() CsrH4DSAlertCancelRxUartActivity()
#define H4DS_ALERT_ARM_TX_BUFFER_EMPTY()    CsrH4DSAlertArmTxBufferEmpty()
#define H4DS_ALERT_CANCEL_TX_BUFFER_EMPTY() CsrH4DSAlertCancelTxBufferEmpty()

#ifdef __cplusplus
}
#endif

#endif
