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
#include "platform/csr_interrupt.h"

static CsrSize logTransportStdoutWrite(void *arg, CsrLogContext context, void *data, CsrSize length)
{
    CsrSize numberOfBytesWritten;

    CSR_UNUSED(arg);
    CSR_UNUSED(context);

    INT_ATOMIC_EXECUTE
    (
        numberOfBytesWritten = fwrite(data, 1, length, stdout);
    );

    return numberOfBytesWritten;
}

static void *logTransportStdoutAlloc(void *arg, CsrSize size)
{
    CSR_UNUSED(arg);
    return CsrPmemAlloc(size);
}

static void logTransportStdoutFree(void *arg, void *data)
{
    CSR_UNUSED(arg);
    CsrPmemFree(data);
}

void *CsrLogTransportStdoutOpen(void)
{
    csrLogTransportHandle *ltHdl;

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
    ltHdl->write = logTransportStdoutWrite;
    ltHdl->alloc = logTransportStdoutAlloc;
    ltHdl->free = logTransportStdoutFree;

    return ltHdl;
}

void CsrLogTransportStdoutClose(void *ltHdl)
{
    CsrPmemFree(ltHdl);
}

#endif
