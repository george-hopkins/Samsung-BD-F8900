/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_abcsp.h"
#include "csr_abcsp_events.h"
#include "csr_types.h"
#include "csr_serial_com.h"

#include "csr_abcsp_log_text.h"

#include "csr_log_text_2.h"
#include "csr_log_gsp.h"

/****************************************************************************
FILE
        config_txmsg.h  -  wire to the environment's tx message support

CONTAINS
        ABCSP_UART_GETTXBUF  -  obtain buffer for uart output
        ABCSP_UART_SENDBYTES  -  pass a block of bytes to the output uart

DESCRIPTION

*/

/****************************************************************************
NAME
        ABCSP_UART_GETTXBUF  -  obtain buffer for uart output

SYNOPSIS
        CsrUint8 *ABCSP_UART_GETTXBUF(CsrUint32 *bufsiz);

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

CsrSize CsrAbcspUartTxBufGet(void)
{
    return sizeof(CsrAbcspInstanceData.txslip.ubuf);
}

/****************************************************************************
NAME
        ABCSP_UART_SENDBYTES  -  pass a block of bytes to the output uart

SYNOPSIS
        void ABCSP_UART_SENDBYTES(CsrUint8 *buf, CsrUint32 n);

FUNCTION
        Tells external code that it that must pass to the output UART the
        "n" bytes in the buffer "buf", obtained by the preceding call
        to ABCSP_UART_GETTXBUF().

        The call returns ownership of the buffer to the external code.
*/

void CsrAbcspUartBytesSend(CsrUint8 *buf, CsrUint32 n)
{
    CsrUint32 numSent;   /* The number of bytes put onto the TX queue */
    abcsp *const _this = &CsrAbcspInstanceData;

    /* Put bytes onto the TX buffer */
    if (!CsrUartDrvTx(_this->uartHandle, buf, n, &numSent))
    {
        CSR_LOG_TEXT_WARNING((CsrBcspLto, 0, "EVT_OVERRUN"));
    }

    CSR_LOG_TRANSPORT(TRANSPORT_TYPE_BCSP, CSR_LOG_TRANSPORT_OUTGOING,
        numSent, n, buf);
}
