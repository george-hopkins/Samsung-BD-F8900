/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef CSR_LOG_ENABLE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_logtransport.h"

#include <windows.h>
#include "platform/csr_logtransport_init.h"

static CsrSize logTransportStdoutWrite(void *arg, CsrLogContext context, void *data, CsrSize length)
{
    CRITICAL_SECTION *criticalSection;
    CsrSize numberOfBytesWritten;

    CSR_UNUSED(context);

    criticalSection = (CRITICAL_SECTION *) arg;

    EnterCriticalSection(criticalSection);
    numberOfBytesWritten = fwrite(data, 1, length, stdout);
    LeaveCriticalSection(criticalSection);

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
    CRITICAL_SECTION *criticalSection;

    ltHdl = CsrPmemAlloc(sizeof(csrLogTransportHandle));
    criticalSection = CsrPmemAlloc(sizeof(CRITICAL_SECTION));

    InitializeCriticalSection(criticalSection);

    ltHdl->write = logTransportStdoutWrite;
    ltHdl->alloc = logTransportStdoutAlloc;
    ltHdl->free = logTransportStdoutFree;
    ltHdl->arg = criticalSection;

    return ltHdl;
}

void CsrLogTransportStdoutClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    CRITICAL_SECTION *criticalSection;

    ltHdl = (csrLogTransportHandle *) arg;
    criticalSection = (CRITICAL_SECTION *) ltHdl->arg;

    DeleteCriticalSection(criticalSection);

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);
}

#endif
