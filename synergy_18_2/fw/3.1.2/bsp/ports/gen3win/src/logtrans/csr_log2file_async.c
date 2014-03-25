/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#define DEBUGLEVEL 0
#if DEBUGLEVEL > 0
#define DPRINTF(x) printf x
#else
#define DPRINTF(x)
#endif /* DEBUGLEVEL > 0 */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_file.h"
#include "csr_logtransport.h"

#include "platform/csr_logtransport_init.h"

typedef struct
{
    CsrUint8        *buf;
    CsrSize          bufMaxSize;
    CsrSize          bufWrap;
    CsrSize          bufIn;
    CsrSize          bufOut;
    CRITICAL_SECTION bufReadWriteMutex;
    CRITICAL_SECTION bufWriteMutex;
    HANDLE           bufWriteOrCloseEvent;
    HANDLE           bufClosedEvent;
    HANDLE           bufFileThread;
    CsrFileHandle   *fileHdl;
    CsrBool          isOpen;
} csrLogTransportFileAsync;

#if DEBUGLEVEL > 0
static CsrUint32 bufSize = 0;
#endif

static void *csrLogTransportFileAsyncAlloc(void *arg, CsrSize size)
{
    csrLogTransportFileAsync *fHdl;
    CsrUint8 *ptr;
    CsrSize bufOut, bufWrap, bufIn, freeSize;

    fHdl = (csrLogTransportFileAsync *) arg;

    EnterCriticalSection(&fHdl->bufWriteMutex);

    EnterCriticalSection(&fHdl->bufReadWriteMutex);
    bufOut = fHdl->bufOut;
    bufIn = fHdl->bufIn;
    bufWrap = fHdl->bufWrap;
    LeaveCriticalSection(&fHdl->bufReadWriteMutex);

    if (bufIn >= bufOut)
    {
        freeSize = bufWrap - bufIn;
    }
    else
    {
        freeSize = bufOut - bufIn - 1;
    }

    if (size <= freeSize)
    {
        ptr = &fHdl->buf[bufIn];
    }
    else if ((bufIn >= bufOut) && (size < bufOut))
    {
        ptr = &fHdl->buf[0];
        EnterCriticalSection(&fHdl->bufReadWriteMutex);

        if (fHdl->bufOut != bufIn)
        {
            fHdl->bufWrap = bufIn;
        }
        else
        {
            fHdl->bufOut = 0;
        }
        fHdl->bufIn = 0;
        LeaveCriticalSection(&fHdl->bufReadWriteMutex);
    }
    else
    {
        DPRINTF(("Out of buffers\n"));
        LeaveCriticalSection(&fHdl->bufWriteMutex);
        ptr = NULL;
    }

    return ptr;
}

static CsrSize csrLogTransportFileAsyncWrite(void *arg, CsrLogContext context, void *data, CsrSize len)
{
    csrLogTransportFileAsync *fHdl;

    CSR_UNUSED(context);

    fHdl = (csrLogTransportFileAsync *) arg;

    EnterCriticalSection(&fHdl->bufReadWriteMutex);
#if DEBUGLEVEL > 0
    bufSize += len;
    DPRINTF(("Write: bufSize =%d\n", bufSize));
#endif
    fHdl->bufIn += len;

    LeaveCriticalSection(&fHdl->bufReadWriteMutex);

    LeaveCriticalSection(&fHdl->bufWriteMutex);

    SetEvent(fHdl->bufWriteOrCloseEvent);

#if DEBUGLEVEL > 0
    if (bufSize > 50000)
    {
        Sleep(10);
    }
#endif
    return len;
}

static void csrLogTransportFileAsyncThreadFunc(void *arg)
{
    csrLogTransportFileAsync *fHdl;
    CsrSize bufOut, bufWrap, bufIn, size;
    CsrSize written;

    fHdl = (csrLogTransportFileAsync *) arg;

    while (1)
    {
        DPRINTF(("Begin wait bufSize=%d\n", bufSize));
        WaitForSingleObject(fHdl->bufWriteOrCloseEvent, INFINITE);
        ResetEvent(fHdl->bufWriteOrCloseEvent);
        DPRINTF(("End wait   bufSize=%d\n", bufSize));
        EnterCriticalSection(&fHdl->bufReadWriteMutex);
        bufOut = fHdl->bufOut;
        bufIn = fHdl->bufIn;
        bufWrap = fHdl->bufWrap;
        LeaveCriticalSection(&fHdl->bufReadWriteMutex);

        if (bufIn != bufOut)
        {
            if (bufIn >= bufOut)
            {
                size = bufIn - bufOut;
            }
            else
            {
                size = bufWrap - bufOut;
            }

            DPRINTF(("Begin write bufSize=%d size=%d\n", bufSize, size));
            CsrFileWrite(&fHdl->buf[bufOut], size, fHdl->fileHdl, &written);
            DPRINTF(("End   write bufSize=%d, written=%d\n", bufSize, written));

            if (written > 0)
            {
                EnterCriticalSection(&fHdl->bufReadWriteMutex);
#if DEBUGLEVEL > 0
                bufSize -= written;
                DPRINTF(("Read: bufSize =%d\n", bufSize));
#endif
                fHdl->bufOut += written;
                if (fHdl->bufOut >= fHdl->bufWrap)
                {
                    fHdl->bufOut = 0;
                    fHdl->bufWrap = fHdl->bufMaxSize;
                }
                LeaveCriticalSection(&fHdl->bufReadWriteMutex);
            }
        }

        if (!fHdl->isOpen)
        {
            DPRINTF(("THREAD STOPPED\n"));
            break;
        }
    }

    SetEvent(fHdl->bufClosedEvent);
    ExitThread(0);
}

void *CsrLogTransportFileAsyncOpen(const char *fileName, CsrUint32 bufSize)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileAsync *fHdl;
    DWORD threadId;

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));

    fHdl = CsrPmemAlloc(sizeof(*fHdl));
    fHdl->buf = CsrPmemAlloc(bufSize * sizeof(*fHdl->buf));
    fHdl->bufMaxSize = bufSize;
    fHdl->bufWrap = bufSize;
    fHdl->bufIn = 0;
    fHdl->bufOut = 0;

    InitializeCriticalSection(&fHdl->bufReadWriteMutex);
    InitializeCriticalSection(&fHdl->bufWriteMutex);

    fHdl->bufWriteOrCloseEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    fHdl->bufClosedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    (void) CsrFileOpen(&fHdl->fileHdl, (CsrUtf8String *) fileName,
        CSR_FILE_OPEN_FLAGS_CREATE | CSR_FILE_OPEN_FLAGS_READ_WRITE | CSR_FILE_OPEN_FLAGS_TRUNCATE,
        CSR_FILE_PERMS_USER_READ | CSR_FILE_PERMS_USER_WRITE);
    fHdl->isOpen = TRUE;
    fHdl->bufFileThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) csrLogTransportFileAsyncThreadFunc, fHdl, 0, &threadId);
    ltHdl->arg = fHdl;
    ltHdl->write = csrLogTransportFileAsyncWrite;
    ltHdl->alloc = csrLogTransportFileAsyncAlloc;
    ltHdl->free = NULL;
    return ltHdl;
}

void CsrLogTransportFileAsyncClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileAsync *fHdl;
    DWORD threadExitCode;

    ltHdl = (csrLogTransportHandle *) arg;
    fHdl = (csrLogTransportFileAsync *) ltHdl->arg;

    fHdl->isOpen = FALSE;

    SetEvent(fHdl->bufWriteOrCloseEvent);

    WaitForSingleObject(fHdl->bufClosedEvent, INFINITE);
    do
    {
        if (!GetExitCodeThread(fHdl->bufFileThread, &threadExitCode))
        {
            DPRINTF(("GetExitCodeThread failed!\n"));
            break;
        }
        else
        {
            if (threadExitCode == STILL_ACTIVE)
            {
                Sleep(0);
            }
        }
    } while (threadExitCode == STILL_ACTIVE);

    DeleteCriticalSection(&fHdl->bufReadWriteMutex);
    DeleteCriticalSection(&fHdl->bufWriteMutex);

    if (fHdl->fileHdl != INVALID_HANDLE_VALUE)
    {
        CsrFileFlush(fHdl->fileHdl);
        CsrFileClose(fHdl->fileHdl);
    }

    CloseHandle(fHdl->bufWriteOrCloseEvent);
    CloseHandle(fHdl->bufClosedEvent);
    CloseHandle(fHdl->bufFileThread);
    CsrPmemFree(fHdl->buf);
    CsrPmemFree(fHdl);

    CsrPmemFree(ltHdl);
}
