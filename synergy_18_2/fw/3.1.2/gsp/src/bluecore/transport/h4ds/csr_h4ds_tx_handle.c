/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_h4_events.h"
#include "csr_types.h"
#include "csr_serial_com.h"
#include "csr_h4ds_inst.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log_gsp.h"
#endif

/****************************************************************************
NAME
        H4_UART_SENDBYTES  -  pass a block of bytes to the output uart

SYNOPSIS
        CsrUint16 H4_UART_SENDBYTES(CsrUint8 *buf, CsrUint32 n);

FUNCTION
        Tells external code that it that must pass to the output UART the
        "n" bytes in the buffer "buf", obtained by the preceding call
        to H4_UART_GETTXBUF().

        The call returns ownership of the buffer to the external code.
*/

CsrUint32 CsrH4DSUartBytesSend(const CsrUint8 *buf, CsrUint32 n)
{
    CsrUint32 numSent;   /* The number of bytes put onto the TX queue */

    /* Put bytes onto the TX buffer */
    if (!CsrUartDrvTx(CsrH4DSInst.uartHandle, buf, n, &numSent))
    {
#ifdef CSR_LOG_ENABLE
        H4_EVENT(H4_EVT_OVERRUN);
#endif
    }

    return numSent;
}
