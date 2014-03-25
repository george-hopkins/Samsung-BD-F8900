/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef CSR_LOG_ENABLE

#include <stdio.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_logtransport.h"

#include "platform/csr_logtransport_init.h"
#include "platform/csr_uart.h"
#include "platform/csr_interrupt.h"

#define TX_BUFFER_SIZE  (100 * 64 * 1024)
static unsigned char txBuffer[TX_BUFFER_SIZE];

static CsrSize CsrLogTransportUartWrite(void *arg, CsrLogContext context,
    void *data, CsrSize length)
{
    int *uartno;
    CsrSize numberOfBytesWritten = 0;

    INT_ATOMIC_EXECUTE_HEAD();

    uartno = (int *) arg;

    CSR_UNUSED(context);

    numberOfBytesWritten = UARTTransmit(*uartno, length, data); /* just push data out */

    if (numberOfBytesWritten != length)
    {
        /* HW buffer full */
    }
    INT_ATOMIC_EXECUTE_TAIL();
    return numberOfBytesWritten;
}

static void *csrLogTransportUartAlloc(void *arg, CsrSize size)
{
    return CsrPmemAlloc(size);
}

static void csrLogTransportUartFree(void *arg, void *data)
{
    CsrPmemFree(data);
}

void *CsrLogTransportUartOpen(int uartno)
{
    csrLogTransportHandle *ltHdl;
    int *intPtr;

    INT_ATOMIC_EXECUTE_HEAD();

    intPtr = CsrPmemAlloc(sizeof(*intPtr));
    *intPtr = uartno;

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
    ltHdl->write = CsrLogTransportUartWrite;
    ltHdl->alloc = csrLogTransportUartAlloc;
    ltHdl->free = csrLogTransportUartFree;
    ltHdl->arg = intPtr;

    UARTInitialise(uartno,
        UART_BAUD_115200,
        UART_CONF_WORDLENGTH_8 | UART_CONF_PARITY_EVEN | UART_CONF_STOPBIT_1,
        FALSE,
        0,                  /* We only do TX */
        NULL,               /* We only do TX */
        TX_BUFFER_SIZE,
        txBuffer);

    INT_ATOMIC_EXECUTE_TAIL();

    return ltHdl;
}

void CsrLogTransportUartClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    int *uartno;

    INT_ATOMIC_EXECUTE_HEAD();

    ltHdl = (csrLogTransportHandle *) arg;
    uartno = (int *) ltHdl->arg;

    UARTDeinitialise(*uartno);

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);

    INT_ATOMIC_EXECUTE_TAIL();
}

#endif
