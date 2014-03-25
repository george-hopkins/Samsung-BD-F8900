#ifndef CSR_ABCSP_CONFIG_TXMSG_H__
#define CSR_ABCSP_CONFIG_TXMSG_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Use #defines rather than typedefs to aid portability.  The abcsp code
treats these types as opaque references, so void* is appropriate. */


/****************************************************************************
NAME
        ABCSP_UART_GETTXBUF  -  obtain buffer for uart output

SYNOPSIS
        CsrUint8 *ABCSP_UART_GETTXBUF(abcsp *_this);

FUNCTION
        Obtains a buffer into which to write UART output bytes.

        When the abcsp library is ready to write (slipped) bytes to
        the UART it calls this function to obtain a buffer.  If this
        function obtains a buffer the library writes to the buffer then
        calls ABCSP_UART_SENDBYTES().  This requires the UART to transmit
        the bytes written to the buffer.  It also returns ownership of
        the buffer to external code.

        The abcsp library guarantees not to hold more than one of these
        buffers at any one time.

RETURNS
        A buffer into which to write UART output bytes, or address zero
        (NULL) if no buffer is available.  The length of any buffer
        returned is written to the location pointed to by "bufsiz".
*/

#define ABCSP_UART_GETTXBUF(t)            CsrAbcspUartTxBufGet()


/****************************************************************************
NAME
        ABCSP_UART_SENDBYTES  -  pass a block of bytes to the output uart

SYNOPSIS
        void ABCSP_UART_SENDBYTES(abcsp *_this, CsrUint32 n);

FUNCTION
        Tells external code that it that must pass to the output UART the
        "n" bytes in the buffer "buf", obtained by the preceding call
        to ABCSP_UART_GETTXBUF().

        The call returns ownership of the buffer to the external code.
*/
#define ABCSP_UART_SENDBYTES(n)    CsrAbcspUartBytesSend(CsrAbcspInstanceData.txslip.ubuf, n)

/****************************************************************************
NAME
        ABCSP_TXMSG_INIT_READ  -  initialise reading a bcsp transmit message

SYNOPSIS
        void ABCSP_TXMSG_INIT_READ(ABCSP_TXMSG *msg);

FUNCTION
    Tells the surrounding code that it wishes to start reading the
    message identified by "msg" from its start.

    The next call to ABCSP_TXMSG_GETBUF() is expected to obtain the first
    raw message bytes from "msg".
*/
#define ABCSP_TXMSG_INIT_READ(m)    CsrAbcspTxMsgInitRead(m)

/****************************************************************************
NAME
        ABCSP_TXMSG_LENGTH  -  how long is a transmit message

SYNOPSIS
        CsrUint32 ABCSP_TXMSG_LENGTH(ABCSP_TXMSG *msg);

RETURNS
        The number of bytes in the message "msg".
*/
#define ABCSP_TXMSG_LENGTH(m)        CsrAbcspTxMsgLength(m)

/****************************************************************************
NAME
        ABCSP_TXMSG_GETBUF  -  access raw message bytes in a message

SYNOPSIS
        CsrUint8 *ABCSP_TXMSG_GETBUF(ABCSP_TXMSG *msg, CsrUint32 *buflen);

RETURNS
    The address of a buffer containing the next raw message bytes to be
    read from "msg", or address zero (NULL) if all of the bytes have been
    read.

        If a buffer is returned its size is written at "buflen".
*/
#define ABCSP_TXMSG_GETBUF(m, l)        CsrAbcspTxMsgBufGet(m, l)

/****************************************************************************
NAME
        ABCSP_TXMSG_TAKEN  -  tell message how many bytes have been read

SYNOPSIS
        void ABCSP_TXMSG_TAKEN(ABCSP_TXMSG *msg, CsrUint32 ntaken);

FUNCTION
        Tells surrounding code that the abcsp library has read "ntaken"
        bytes from the buffer obtained from the preceding call to
        ABCSP_TXMSG_GETBUF().
*/
#define ABCSP_TXMSG_TAKEN(m, n)    CsrAbcspTxMsgTaken(m, n)

/****************************************************************************
NAME
        ABCSP_TXMSG_DONE  -  signal that message has been delivered

SYNOPSIS
        void ABCSP_TXMSG_DONE(ABCSP_TXMSG *msg);

FUNCTION
    Tells the surrounding code that the abcsp library has finished with
    "msg".  For unreliable messages this means it has been sent to the
    UART.  For reliable messages this means the peer BCSP stack has
    acknowledged reception of the message.
*/
#define ABCSP_TXMSG_DONE(m)    CsrAbcspTxMsgDone(m)

#ifdef __cplusplus
}
#endif

#endif
