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

#if DEBUGLEVEL > 1
#define DPRINTF_BUF(x) printf x
#else
#define DPRINTF_BUF(x)
#endif /* DEBUGLEVEL > 0 */

#include "nucleus.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_file.h"
#include "csr_panic.h"
#include "csr_logtransport.h"

#include "platform/csr_logtransport_init.h"


#define CSR_LOG_FILE_ASYNC_STACK_SIZE  1024 * 4
#define CSR_LOG_FILE_ASYNC_PRIORITY    20
#define CSR_LOG_FILE_ASYNC_MAX_TICKS   2

#define CSR_LOG_FILE_ASYNC_WRITE_EVENT 0x00000001
#define CSR_LOG_FILE_ASYNC_CLOSE_EVENT 0x00000002
#define CSR_LOG_FILE_ASYNC_WAIT_EVENTS 0x00000003

extern NU_MEMORY_POOL System_Memory;
VOID *pointer;

typedef struct
{
    CsrUint8      *buf;
    CsrUint32      bufMaxSize;
    CsrUint32      bufWrap;
    CsrUint32      bufIn;
    CsrUint32      bufOut;
    INT            bufMutex;
    NU_EVENT_GROUP bufWriteOrCloseEvent;
    NU_EVENT_GROUP bufClosedEvent;
    NU_TASK        bufFileThread;
    CsrFileHandle *fileHdl;
} csrLogTransportFileAsync;

#if DEBUGLEVEL > 1
static CsrUint32 bufSize = 0;
#endif

#define TAKE_MUTEX() fHdl->bufMutex = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS)
#define RELEASE_MUTEX() fHdl->bufMutex = NU_Local_Control_Interrupts(fHdl->bufMutex)

static void *csrLogTransportFileAsyncAlloc(void *arg, CsrSize size)
{
    csrLogTransportFileAsync *fHdl;
    CsrUint8 *ptr;
    CsrUint32 bufOut, bufWrap, bufIn, freeSize;

    fHdl = (csrLogTransportFileAsync *) arg;

    TAKE_MUTEX();
    bufOut = fHdl->bufOut;
    bufIn = fHdl->bufIn;
    bufWrap = fHdl->bufWrap;

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

        if (fHdl->bufOut != bufIn)
        {
            fHdl->bufWrap = bufIn;
        }
        else
        {
            fHdl->bufOut = 0;
        }
        fHdl->bufIn = 0;
    }
    else
    {
        ptr = NULL;
        RELEASE_MUTEX();
        DPRINTF(("Out of buffers\n"));
    }

    return ptr;
}

static CsrSize csrLogTransportFileAsyncWrite(void *arg, CsrLogContext context, void *data, CsrSize len)
{
    csrLogTransportFileAsync *fHdl;

    CSR_UNUSED(context);

    fHdl = (csrLogTransportFileAsync *) arg;

    DPRINTF_BUF(("Write: bufSize =%d\n", bufSize += len));

    fHdl->bufIn += len;
    RELEASE_MUTEX();

    NU_Set_Events(&fHdl->bufWriteOrCloseEvent, CSR_LOG_FILE_ASYNC_WRITE_EVENT, NU_OR);

#if DEBUGLEVEL > 1
    if (bufSize > 50000)
    {
        Sleep(10);
    }
#endif
    return len;
}

static void csrLogTransportFileAsyncThreadFunc(UNSIGNED argc, VOID *arg)
{
    csrLogTransportFileAsync *fHdl;
    CsrUint32 bufOut, bufWrap, bufIn, size;
    CsrSize written;
    UNSIGNED retrieved = 0;

    fHdl = (csrLogTransportFileAsync *) arg;

    while (1)
    {
        DPRINTF_BUF(("Begin wait bufSize=%d\n", bufSize));

        if (NU_Retrieve_Events(&fHdl->bufWriteOrCloseEvent, CSR_LOG_FILE_ASYNC_WAIT_EVENTS, NU_OR_CONSUME, &retrieved, NU_SUSPEND) != NU_SUCCESS)
        {
            DPRINTF(("Log2File_Async received unknown error \n"));
            continue;
        }

        if (retrieved & CSR_LOG_FILE_ASYNC_WRITE_EVENT)
        {
            DPRINTF_BUF(("End wait, retrieved=CSR_LOG_FILE_ASYNC_WRITE_EVENT bufSize=%d\n", bufSize));

            TAKE_MUTEX();

            bufOut = fHdl->bufOut;
            bufIn = fHdl->bufIn;
            bufWrap = fHdl->bufWrap;

            RELEASE_MUTEX();

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

                DPRINTF_BUF(("Begin write bufSize=%d size=%d\n", bufSize, size));
                CsrFileWrite(&fHdl->buf[bufOut], size, fHdl->fileHdl, &written);
                DPRINTF_BUF(("End   write bufSize=%d, written=%d\n", bufSize, written));

                if (written > 0)
                {
                    TAKE_MUTEX();

                    DPRINTF_BUF(("Read: bufSize =%d\n", bufSize -= written));

                    fHdl->bufOut += written;
                    if (fHdl->bufOut >= fHdl->bufWrap)
                    {
                        fHdl->bufOut = 0;
                        fHdl->bufWrap = fHdl->bufMaxSize;
                    }

                    RELEASE_MUTEX();
                }
            }
        }

        if (retrieved & CSR_LOG_FILE_ASYNC_CLOSE_EVENT)
        {
            DPRINTF(("Log2File_Async THREAD STOPPED\n"));
            break;
        }

        if (!((retrieved & CSR_LOG_FILE_ASYNC_CLOSE_EVENT) || (retrieved & CSR_LOG_FILE_ASYNC_WRITE_EVENT)))
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "log2file_async received unknown event");
        }
    }

    NU_Set_Events(&fHdl->bufClosedEvent, CSR_LOG_FILE_ASYNC_CLOSE_EVENT, NU_AND);
}

void *CsrLogTransportFileAsyncOpen(const char *fileName, CsrUint32 bufSize)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileAsync *fHdl;

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));

    fHdl = CsrPmemAlloc(sizeof(*fHdl));
    fHdl->buf = CsrPmemAlloc(bufSize * sizeof(*fHdl->buf));
    fHdl->bufMaxSize = bufSize;
    fHdl->bufWrap = bufSize;
    fHdl->bufIn = 0;
    fHdl->bufOut = 0;
    fHdl->bufMutex = 0;

    NU_Create_Event_Group(&fHdl->bufWriteOrCloseEvent, "ASYNC_FILE_WAIT");
    NU_Create_Event_Group(&fHdl->bufClosedEvent, "ASYNC_CLOSE");

    (void) CsrFileOpen(&fHdl->fileHdl, (CsrUtf8String *) fileName,
        CSR_FILE_OPEN_FLAGS_CREATE | CSR_FILE_OPEN_FLAGS_READ_WRITE | CSR_FILE_OPEN_FLAGS_TRUNCATE,
        CSR_FILE_PERMS_USER_READ | CSR_FILE_PERMS_USER_WRITE);

    NU_Allocate_Memory(&System_Memory, &pointer, CSR_LOG_FILE_ASYNC_STACK_SIZE, NU_NO_SUSPEND);
    NU_Create_Task(&fHdl->bufFileThread, "ASYNC_FILE_TASK", csrLogTransportFileAsyncThreadFunc, 0, fHdl, pointer, CSR_LOG_FILE_ASYNC_STACK_SIZE, CSR_LOG_FILE_ASYNC_PRIORITY, CSR_LOG_FILE_ASYNC_MAX_TICKS, NU_PREEMPT, NU_START);

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
    UNSIGNED retrieved;

    ltHdl = (csrLogTransportHandle *) arg;
    fHdl = (csrLogTransportFileAsync *) ltHdl->arg;

    NU_Set_Events(&fHdl->bufWriteOrCloseEvent, CSR_LOG_FILE_ASYNC_CLOSE_EVENT, NU_OR);
    do
    {
        if (NU_Retrieve_Events(&fHdl->bufClosedEvent, CSR_LOG_FILE_ASYNC_CLOSE_EVENT, NU_AND_CONSUME, &retrieved, NU_SUSPEND) == NU_SUCCESS)
        {
            DPRINTF(("Write Thread closed successfully\n"));
            break;
        }
    } while (1);

    if (fHdl->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileFlush(fHdl->fileHdl);
        CsrFileClose(fHdl->fileHdl);
    }

    NU_Terminate_Task(&fHdl->bufFileThread);
    NU_Delete_Task(&fHdl->bufFileThread);
    NU_Deallocate_Memory(pointer);

    CsrPmemFree(fHdl->buf);
    CsrPmemFree(fHdl);

    CsrPmemFree(ltHdl);
}
