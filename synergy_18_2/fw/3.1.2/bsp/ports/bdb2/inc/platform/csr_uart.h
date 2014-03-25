#ifndef CSR_UART_H__
#define CSR_UART_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Baud rates for use by UARTInitialise */  /*     Actual Rate     */
#define UART_BAUD_1200           5207       /*    1200.08 (+0.01%) */
#define UART_BAUD_2400           2603       /*    2400.15 (+0.01%) */
#define UART_BAUD_4800           1301       /*    4800.31 (+0.01%) */
#define UART_BAUD_9600            650       /*    9600.61 (+0.01%) */
#define UART_BAUD_14400           433       /*   14400.92 (+0.01%) */
#define UART_BAUD_19200           325       /*   19171.78 (-0.15%) */
#define UART_BAUD_28800           216       /*   28801.84 (+0.01%) */
#define UART_BAUD_33600           185       /*   33602.15 (+0.01%) */
#define UART_BAUD_38400           162       /*   38343.56 (-0.15%) */
#define UART_BAUD_57600           108       /*   57339.45 (-0.45%) */
#define UART_BAUD_115200           53       /*  115740.74 (+0.47%) */
#define UART_BAUD_230400           26       /*  231481.48 (+0.47%) */
#define UART_BAUD_460800           13       /*  446428.57 (-3.12%) */
#define UART_BAUD_921600            6       /*  892857.14 (-3.12%) */
#define UART_BAUD_1075200           5       /* 1041666.67 (-3.12%) */
#define UART_BAUD_1228800           4       /* 1250000.00 (+1.73%) */
#define UART_BAUD_1536000           3       /* 1562500.00 (+1.73%) */
#define UART_BAUD_2048000           2       /* 2083333.33 (+1.73%) */
#define UART_BAUD_3072000           1       /* 3125000.00 (+1.73%) */

/* Configuration parameters for use by UARTInitialise */
#define UART_CONF_PARITY_NONE     (0 << 3)   /* No parity */
#define UART_CONF_PARITY_ODD      (4 << 3)   /* Odd parity */
#define UART_CONF_PARITY_EVEN     (5 << 3)   /* Even parity */
#define UART_CONF_PARITY_MARK     (6 << 3)   /* Mark (1) parity */
#define UART_CONF_PARITY_SPACE    (7 << 3)   /* Space (0) parity */
#define UART_CONF_STOPBIT_1       (0 << 2)   /* 1 Stopbit */
#define UART_CONF_STOPBIT_2       (1 << 2)   /* 2 Stopbit */
#define UART_CONF_WORDLENGTH_5    (0 << 0)   /* 5bit word */
#define UART_CONF_WORDLENGTH_6    (1 << 0)   /* 6bit word */
#define UART_CONF_WORDLENGTH_7    (2 << 0)   /* 7bit word */
#define UART_CONF_WORDLENGTH_8    (3 << 0)   /* 8bit word */
#define UART_CONF_INFRARED        (1 << 6)   /* Infrared Mode */
#define UART_CONF_8N1             (UART_CONF_WORDLENGTH_8 | UART_CONF_PARITY_NONE | UART_CONF_STOPBIT_1)

/* Tokens for use by UARTTokenDetectorRegister and UARTReceiveDataCountBeforeTokenGet */
#define UART_TOKEN_DATA(value)  ((value) & 0xFF)    /* Matches the specified value */
#define UART_TOKEN_EOL          (0x100)             /* Matches any of "\r\n" and '\n' */

/* Return value from UARTReceiveLine */
#define UART_NO_FULL_LINE       -1

/* Number of UART channels */
#define UART_CHANNEL_COUNT       2

/* Notification function type for Token Detector */
typedef void (*UARTNotifyFunction)(void);

/* Initialise UART hardware and start reception/transmission */
void UARTInitialise(CsrUint8 channel, CsrUint16 baudRate, CsrUint8 configuration, CsrUint8 hardwareFlowControl, CsrUint32 receiveBufferSize, CsrUint8 *receiveBuffer, CsrUint32 transmitBufferSize, CsrUint8 *transmitBuffer);

/* Stop reception/transmission and deinitialise UART hardware. This will also
   unregister any token and data detectors. */
void UARTDeinitialise(CsrUint8 channel);

/* Token detector. If a token is registered, every received byte will be
   examined, and every time the registered token is detected, the supplied
   UARTNotifyFunction will be called. Only one token can be registered.
   Repeated calls will replace any previous registrations. */
void UARTTokenDetectorRegister(CsrUint8 channel, CsrUint16 token, UARTNotifyFunction detect);
void UARTTokenDetectorUnregister(CsrUint8 channel);

/* Data detector. Register a UARTNotifyFunction that will be called whenever
   received data are available. The supplied function will be called once when
   data become available, and the callback will then be inhibited until at least
   a single byte is retrieved by a call to UARTReceive or UARTReceiveLine. */
void UARTDataDetectorRegister(CsrUint8 channel, UARTNotifyFunction detect);
void UARTDataDetectorUnregister(CsrUint8 channel);

/* Retrieve the total amount of data available in the receive/transmit buffers */
CsrUint32 UARTReceiveDataCountGet(CsrUint8 channel);
CsrUint32 UARTTransmitDataCountGet(CsrUint8 channel);

/* Receive a specified number of byte and store them in the array pointed at by
   data. Specify NULL as data parameter to only consume/discard data from the
   buffer. Returns the number of byte consumed. */
CsrUint32 UARTReceive(CsrUint8 channel, CsrUint32 length, CsrUint8 *data);

/* Receive a full line of text delimited by "\r\n" or '\n'. Always returns the
   length of the line not including the delimiter or zero termination, or the
   return value UART_NO_FULL_LINE if no delimiter was found. If and only if the
   argument length is equal to or greater than the line length + 1, and a
   delimiter was found, the line of text and the delimiter will be consumed from
   the buffer, and if the argument data is not NULL the line of text, not
   including the delimiter, will be written to the array and zero terminated. */
CsrInt32 UARTReceiveLine(CsrUint8 channel, CsrUint32 length, CsrUint8 *data);

/* Retrieve a pointer to the receive buffer. Returns the number of
   contiguous bytes available starting from the pointer. */
CsrUint32 UARTReceiveDataPointerGet(CsrUint8 channel, CsrUint8 **data);

/* Transmit a specified number of bytes. Returns the number of bytes consumed */
CsrUint32 UARTTransmit(CsrUint8 channel, CsrUint32 length, const CsrUint8 *data);

#ifdef __cplusplus
}
#endif

#endif
