#ifndef CSR_ABCSP_PROTOTYPE_H__
#define CSR_ABCSP_PROTOTYPE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_abcsp.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
NAME
        CsrAbcspBcspLeInit  -  initialise the link establishment engine

FUNCTION
        Initialises the BCSP Link Establishment engine, abandoning any
        work in progress.

        This must be called before all other functions declared in this
        file.

        This may be called at any time to reinitialise the engine.
*/

void CsrAbcspBcspLeInit(void);

/****************************************************************************
NAME
        CsrAbcspUartInit -  initialise the slip reception code block

FUNCTION
        Initialises the state of the rxslip code block.

        This must be called before all other functions described in this
        file.

        This may be called at any time to reinitialise the state of the
        code block.
*/

void CsrAbcspUartInit(void);

/****************************************************************************
NAME
        CsrAbcspTxMsgInit  -  initialise the transmit message assembler

FUNCTION
        Initialises the transmit message assembler, abandoning any work
        in progress.

        This must be called before all other functions declared in this
        file.

        This may be called at any time to reinitialise the assembler.
*/

void CsrAbcspTxMsgInit(void);

/****************************************************************************
NAME
        CsrAbcspTxSlipInit  -  initialise the slip encoder

FUNCTION
        Initialises the state of the txslip code block.

        This must be called before all other functions described in this
        file.

        This may be called at any time to reinitialise the state of the
        code block.
*/

void CsrAbcspTxSlipInit(void);

/****************************************************************************
NAME
        CsrAbcspBcspLeDeinit  -  deinitialise the link establishment engine

FUNCTION
*/

void CsrAbcspBcspLeDeinit(void);

/****************************************************************************
NAME
        CsrAbcspTxMsgDeinit  -  deinitialise the transmit message assembler

FUNCTION
*/

void CsrAbcspTxMsgDeinit(void);
/****************************************************************************
NAME
        ABCSP_UART_GETTXBUF  -  obtain buffer for uart output

SYNOPSIS
        CsrUint8 *ABCSP_UART_GETTXBUF(abcsp *_this, CsrUint32 *bufsiz);

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

CsrSize CsrAbcspUartTxBufGet(void);

/****************************************************************************
NAME
        CsrAbcspTxSlipSendBuf -  slip encode buffer and send to uart

FUNCTION
        Slip-encodes and sends up to bufsiz bytes from "buf" to the UART.

RETURNS
        The number of bytes consumed from "buf".

NOTE
        Code under this function makes at most one call to
        ABCSP_UART_SENDBYTES().
*/

CsrUint16 CsrAbcspTxSlipSendBuf(CsrUint8 *buf, CsrUint16 bufsiz);


/****************************************************************************
NAME
        CsrAbcspTxSlipFlush  -  send buffer to uart

FUNCTION
        If the slip-encoder is holding a buffer obtained via a call to
        ABCSP_UART_GETBUF() then this is released by making a call to
        ABCSP_UART_SENDBYTES().
*/

void CsrAbcspTxSlipFlush(void);

/****************************************************************************
NAME
        CsrAbcspTxSlipMsgDelim  -  send slip fame message delimiter to uart

RETURNS
    TRUE if the function has passed a BCSP frame delimiter byte (0xc0) to
    the UART buffer, else FALSE.
*/

CsrBool CsrAbcspTxSlipMsgDelim(void);

/****************************************************************************
NAME
        abcsp_bcsple_putbyte  -  Process an entire bcsp-le message

RETURNS
    TRUE if all went well, FALSE if something went wrong.
*/

CsrBool CsrAbcspBcspLeMsgPut(const CsrUint8 *buf, CsrUint32 len);

/****************************************************************************
NAME
        ABCSP_UART_GETTXBUF  -  obtain buffer for uart output

SYNOPSIS
        CsrUint8 *ABCSP_UART_GETTXBUF(abcsp *_this, CsrUint32 *bufsiz);

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
        void ABCSP_UART_SENDBYTES(abcsp *_this, CsrUint8 *buf, CsrUint32 n);

FUNCTION
        Tells external code that it that must pass to the output UART the
        "n" bytes in the buffer "buf", obtained by the preceding call
        to ABCSP_UART_GETTXBUF().

        The call returns ownership of the buffer to the external code.
*/
void CsrAbcspUartBytesSend(CsrUint8 *buf, CsrUint32 n);

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
void CsrAbcspTxMsgInitRead(void *msg);

/****************************************************************************
NAME
        ABCSP_TXMSG_LENGTH  -  how long is a transmit message

SYNOPSIS
        CsrUint32 ABCSP_TXMSG_LENGTH(ABCSP_TXMSG *msg);

RETURNS
        The number of bytes in the message "msg".
*/
CsrUint32 CsrAbcspTxMsgLength(void *msg);

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
CsrUint8 *CsrAbcspTxMsgBufGet(void *msg, CsrUint32 *buflen);

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
void CsrAbcspTxMsgTaken(void *msg, CsrUint32 ntaken);

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
void CsrAbcspTxMsgDone(void *msg);

/****************************************************************************
NAME
        ABCSP_REQ_PUMPTXMSGS  -  request external code call CsrAbcspPumpTxMsgs

SYNOPSIS
        void ABCSP_REQ_PUMPTXMSGS(void);

FUNCTION
    Tells external code that there is work to be done on the abcsp
    library's transmit path, and requests that it make a call to
    CsrAbcspPumpTxMsgs() at its earliest convenience.

NOTE
        This macro must not be wired directly to CsrAbcspPumpTxMsgs() or the
        code will go re-entrant and bomb.  This function should normally set
        a flag that should provoke a call to CsrAbcspPumpTxMsgs() after the
        current call into the abcsp library has returned.
*/

void CsrAbcspReqPumpTxMsgs(void);
/****************************************************************************
NAME
        ABCSP_RXMSG_DESTROY  -  destroy a abcsp receive message

SYNOPSIS
        void ABCSP_RXMSG_DESTROY(ABCSP_RXMSG *m);

FUNCTION
        Destroys the message identified by the message reference "m".

    All resources identified by "m" are no longer of interest to the
    abcsp library.
*/

void CsrAbcspRxMsgDestroy(ABCSP_RXMSG *m);

#ifdef __cplusplus
}
#endif

#endif
