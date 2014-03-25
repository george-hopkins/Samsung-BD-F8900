/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef CSR_LOG_ENABLE

#include <stdio.h>
#include <nucleus.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_logtransport.h"
#include "platform/csr_logtransport_init.h"
#include "platform/csr_uart.h"

static INT intrLevel;
#define TAKE_MUTEX() intrLevel = NU_Control_Interrupts(NU_DISABLE_INTERRUPTS)
#define RELEASE_MUTEX() NU_Control_Interrupts(intrLevel)

#define ALLOC_BUFFER_SIZE  (512)
static unsigned char allocBuffer[ALLOC_BUFFER_SIZE];
#define TX_BUFFER_SIZE  (100 * 64 * 1024)
static unsigned char txBuffer[TX_BUFFER_SIZE];

static CsrSize csrLogTransportUartWrite(void *arg, CsrLogContext context,
    void *data, CsrSize length)
{
    int *uartno;
    CsrSize numberOfBytesWritten = 0;

    CSR_UNUSED(context);

    uartno = (int *) arg;

    numberOfBytesWritten = UARTTransmit(*uartno, length, data); /* just push data out */

    if (numberOfBytesWritten != length)
    {
        /* HW buffer full */
    }
    RELEASE_MUTEX();

    return numberOfBytesWritten;
}

static void *csrLogTransportUartAlloc(void *arg, CsrSize size)
{
    TAKE_MUTEX();
    return allocBuffer;
}

void *CsrLogTransportUartOpen(int uartno)
{
    csrLogTransportHandle *ltHdl;
    int *intPtr;

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
    ltHdl->write = csrLogTransportUartWrite;
    ltHdl->alloc = csrLogTransportUartAlloc;
    ltHdl->free = NULL;

    intPtr = CsrPmemAlloc(sizeof(*intPtr));
    *intPtr = uartno;
    ltHdl->arg = intPtr;

    UARTInitialise(uartno,
        UART_BAUD_115200,
        UART_CONF_8N1,
        FALSE,
        0,                  /* We only do TX */
        NULL,               /* We only do TX */
        TX_BUFFER_SIZE,
        txBuffer);

    return ltHdl;
}

void CsrLogTransportUartClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    int *uartno;

    ltHdl = (csrLogTransportHandle *) arg;
    uartno = (int *) ltHdl->arg;

    UARTDeinitialise(*uartno);

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);
}

#endif
