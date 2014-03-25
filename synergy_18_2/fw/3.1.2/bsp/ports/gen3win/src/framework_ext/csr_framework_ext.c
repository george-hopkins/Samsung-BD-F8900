/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include "csr_types.h"
#include "csr_framework_ext.h"
#include "csr_mem_hook.h"
#include "csr_log.h"

static LONG globalLock;

static int mapPri(CsrUint16 pri)
{
    return 2 - pri;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventCreate
 *
 *  DESCRIPTION
 *      Creates an event and returns a handle to the created event.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS          in case of success
 *          CSR_FE_RESULT_NO_MORE_EVENTS   in case of out of event resources
 *          CSR_FE_RESULT_INVALID_POINTER  in case the eventHandle pointer is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrEventCreate(CsrEventHandle *eventHandle)
{
    if (eventHandle == NULL)
    {
        return CSR_FE_RESULT_NO_MORE_EVENTS;
    }

    eventHandle->event = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (eventHandle->event == NULL)
    {
        return CSR_FE_RESULT_NO_MORE_EVENTS;
    }
    InitializeCriticalSection(&(eventHandle->mutex));
    eventHandle->waitingBits = 0;
    eventHandle->eventBits = 0;

    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventWait
 *
 *  DESCRIPTION
 *      Wait for the event to be set.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS              in case of success
 *          CSR_FE_RESULT_TIMEOUT              in case of timeout
 *          CSR_FE_RESULT_INVALID_HANDLE       in case the eventHandle is invalid
 *          CSR_FE_RESULT_INVALID_POINTER      in case the eventBits pointer is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrEventWait(CsrEventHandle *eventHandle, CsrUint16 timeoutInMs, CsrUint32 *eventBits)
{
    CsrResult result = CSR_RESULT_SUCCESS;

    if (eventHandle == NULL)
    {
        return CSR_FE_RESULT_INVALID_HANDLE;
    }

    if (eventBits == NULL)
    {
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    EnterCriticalSection(&(eventHandle->mutex));

    if ((eventHandle->eventBits == 0) && (timeoutInMs != 0))
    {
        LeaveCriticalSection(&(eventHandle->mutex));
        WaitForSingleObject(eventHandle->event, timeoutInMs == CSR_EVENT_WAIT_INFINITE ? INFINITE : timeoutInMs);
        EnterCriticalSection(&(eventHandle->mutex));
    }

    result = (eventHandle->eventBits == 0) ? CSR_FE_RESULT_TIMEOUT : CSR_RESULT_SUCCESS;
    /* Indicate to caller which events were triggered and cleared */
    *eventBits = eventHandle->eventBits;
    /* Clear triggered events */
    eventHandle->eventBits = 0;
    ResetEvent(eventHandle->event);
    LeaveCriticalSection(&(eventHandle->mutex));
    return result;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventSet
 *
 *  DESCRIPTION
 *      Set an event.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS              in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE       in case the eventHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrEventSet(CsrEventHandle *eventHandle, CsrUint32 eventBits)
{
    EnterCriticalSection(&(eventHandle->mutex));

    /* Set the requested bits */
    eventHandle->eventBits |= eventBits;

    if (eventHandle->eventBits)
    {
        SetEvent(eventHandle->event);
    }

    LeaveCriticalSection(&(eventHandle->mutex));

    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrEventDestroy
 *
 *  DESCRIPTION
 *      Destroy the event associated.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrEventDestroy(CsrEventHandle *eventHandle)
{
    DeleteCriticalSection(&(eventHandle->mutex));
    CloseHandle(eventHandle->event);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexCreate
 *
 *  DESCRIPTION
 *      Create a mutex and return a handle to the created mutex.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_NO_MORE_MUTEXES   in case of out of mutex resources
 *          CSR_FE_RESULT_INVALID_POINTER   in case the mutexHandle pointer is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrMutexCreate(CsrMutexHandle *mutexHandle)
{
    *mutexHandle = CreateMutex(NULL, FALSE, NULL);
    if (*mutexHandle == NULL)
    {
        return CSR_FE_RESULT_NO_MORE_MUTEXES;
    }

    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexLock
 *
 *  DESCRIPTION
 *      Lock the mutex refered to by the provided handle.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE    in case the mutexHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrMutexLock(CsrMutexHandle *mutexHandle)
{
    DWORD result;

    result = WaitForSingleObject(*mutexHandle, INFINITE);
    if (result != WAIT_OBJECT_0)
    {
        return CSR_FE_RESULT_INVALID_HANDLE;
    }

    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexUnlock
 *
 *  DESCRIPTION
 *      Unlock the mutex refered to by the provided handle.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_INVALID_HANDLE    in case the mutexHandle is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrMutexUnlock(CsrMutexHandle *mutexHandle)
{
    BOOL result;

    result = ReleaseMutex(*mutexHandle);
    if (result == 0)
    {
        return CSR_FE_RESULT_INVALID_HANDLE;
    }

    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMutexDestroy
 *
 *  DESCRIPTION
 *      Destroy the previously created mutex.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrMutexDestroy(CsrMutexHandle *mutexHandle)
{
    CloseHandle(*mutexHandle);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrGlobalMutexLock
 *
 *  DESCRIPTION
 *      Lock the global mutex.
 *
 *----------------------------------------------------------------------------*/
void CsrGlobalMutexLock(void)
{
    LONG globalLockMirror;
    do
    {
        globalLockMirror = InterlockedExchange(&globalLock, 1);
    } while (globalLockMirror == 1);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrGlobalMutexUnlock
 *
 *  DESCRIPTION
 *      Unlock the global mutex.
 *
 *----------------------------------------------------------------------------*/
void CsrGlobalMutexUnlock(void)
{
    InterlockedExchange(&globalLock, 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadCreate
 *
 *  DESCRIPTION
 *      Create thread function and return a handle to the created thread.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS           in case of success
 *          CSR_FE_RESULT_NO_MORE_THREADS   in case of out of thread resources
 *          CSR_FE_RESULT_INVALID_POINTER   in case one of the supplied pointers is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrThreadCreate(void (*threadFunction)(void *pointer), void *pointer,
    CsrUint32 stackSize, CsrUint16 priority,
    const CsrCharString *threadName, CsrThreadHandle *threadHandle)
{
    DWORD tid;
    HANDLE th;

    if ((threadFunction == NULL) || (threadHandle == NULL))
    {
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    th = CreateThread(NULL, stackSize, (LPTHREAD_START_ROUTINE) threadFunction, pointer, 0, &tid);
    if (th == NULL)
    {
        *threadHandle = 0;
        return CSR_FE_RESULT_NO_MORE_THREADS;
    }

    SetThreadPriority(th, mapPri(priority));
    *threadHandle = tid;
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadGetHandle
 *
 *  DESCRIPTION
 *      Return thread handle of calling thread.
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS             in case of success
 *          CSR_FE_RESULT_INVALID_POINTER  in case the threadHandle pointer is invalid
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrThreadGetHandle(CsrThreadHandle *threadHandle)
{
    if (threadHandle == NULL)
    {
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    *threadHandle = GetCurrentThreadId();
    return CSR_RESULT_SUCCESS;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadEqual
 *
 *  DESCRIPTION
 *      Compare thread handles
 *
 *  RETURNS
 *      Possible values:
 *          CSR_RESULT_SUCCESS             in case thread handles are identical
 *          CSR_FE_RESULT_INVALID_POINTER  in case either threadHandle pointer is invalid
 *          CSR_RESULT_FAILURE             otherwise
 *
 *----------------------------------------------------------------------------*/
CsrResult CsrThreadEqual(CsrThreadHandle *threadHandle1, CsrThreadHandle *threadHandle2)
{
    if ((threadHandle1 == NULL) || (threadHandle2 == NULL))
    {
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    if (*threadHandle1 == *threadHandle2)
    {
        return CSR_RESULT_SUCCESS;
    }
    else
    {
        return CSR_RESULT_FAILURE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadSleep
 *
 *  DESCRIPTION
 *      Sleep for a given period.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrThreadSleep(CsrUint16 sleepTimeInMs)
{
    ULONG ticksToSleep/*, unused1*/;
/*    BOOL unused2;*/
    ULONG tickInterval=1;

 //   GetSystemTimeAdjustment(&unused1, &tickInterval, &unused2); /* 100 nsec units */
    ticksToSleep = (((ULONG) sleepTimeInMs * 10000) / tickInterval);

    if (ticksToSleep == 0)
    {
        /* sleepTimeInMs is smaller than one Windows timer tick, set delay
         * to one tick */
        ticksToSleep++;
    }
    else if ((ticksToSleep * tickInterval) < ((ULONG) sleepTimeInMs * 10000))
    {
        /* Add an extra timer tick because of the rounding off error */
        ticksToSleep++;
    }

    Sleep((ticksToSleep * tickInterval) / 10000);
}

#define bufHdr(ptr, hlen)       ((((CsrUint8 *) (ptr)) - hlen))
#define hdrBuf(hdr, hlen)       (((CsrUint8 *) (hdr)) + hlen)

#ifdef CSR_MEMALLOC_PROFILING
/* Align data buffer to an 8 byte boundary. */
#define bufAlignBytes           8
#define bufAlign(ptr, align)    ((ptr + (align - 1)) & ~(align - 1))

static CsrMemHookAlloc cbAlloc;
static CsrMemHookAlloc cbCalloc;
static CsrMemHookFree cbFree;
static CsrMemHookAlloc cbAllocDma;
static CsrMemHookFree cbFreeDma;
static CsrSize headerSize;
static CsrSize tailSize;

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemHookSet
 *
 *  DESCRIPTION
 *      Installs hooks to be called during memory allocation
 *      and deallocation.
 *
 *  RETURNS
 *
 *----------------------------------------------------------------------------*/
void CsrMemHookSet(CsrMemHookAlloc allocCb, CsrMemHookFree freeCb,
    CsrMemHookAlloc allocDmaCb, CsrMemHookFree freeDmaCb,
    CsrSize hdrSz, CsrSize tailSz)
{
    headerSize = bufAlign(hdrSz, bufAlignBytes); /* Align once */
    tailSize = tailSz; /* Unaligned!  Immediately follows buffer. */
    cbAlloc = allocCb;
    cbFree = freeCb;
    cbAllocDma = allocDmaCb;
    cbFreeDma = freeDmaCb;
}

#else
#define headerSize 0
#define tailSize 0
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemAlloc
 *
 *  DESCRIPTION
 *      Allocate dynamic memory of a given size.
 *
 *  RETURNS
 *      Pointer to allocated memory, or NULL in case of failure.
 *      Allocated memory is not initialised.
 *
 *----------------------------------------------------------------------------*/
#ifdef CSR_MEM_DEBUG
#undef CsrMemAlloc
void *CsrMemAlloc(CsrSize size)
{
    return CsrMemAllocDebug(size, __FILE__, __LINE__);
}

void *CsrMemAllocDebug(CsrSize size,
    const CsrCharString *file, CsrUint32 line)
#else
void *CsrMemAlloc(CsrSize size)
#endif
{
    void *hdr;

    hdr = malloc(size + headerSize + tailSize);

#ifdef CSR_MEMALLOC_PROFILING
    if ((hdr != NULL) && (cbAlloc != NULL))
    {
#ifdef CSR_MEM_DEBUG
        cbAlloc(hdr, hdrBuf(hdr, headerSize), 1, size, file, line);
#else
        cbAlloc(hdr, hdrBuf(hdr, headerSize), 1, size, "n/a", 0);
#endif
    }
#endif

    /*lint -save -e429 */
    return hdrBuf(hdr, headerSize);
    /*lint -restore */
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemCalloc
 *
 *  DESCRIPTION
 *      Allocate dynamic memory of a given size calculated as the
 *      numberOfElements times the elementSize.
 *
 *  RETURNS
 *      Pointer to allocated memory, or NULL in case of failure.
 *      Allocated memory is zero initialised.
 *
 *----------------------------------------------------------------------------*/
#ifdef CSR_MEM_DEBUG
#undef CsrMemCalloc
void *CsrMemCalloc(CsrSize numberOfElements, CsrSize elementSize)
{
    return CsrMemCallocDebug(numberOfElements, elementSize,
        __FILE__, __LINE__);
}

void *CsrMemCallocDebug(CsrSize numberOfElements, CsrSize elementSize,
    const CsrCharString *file, CsrUint32 line)
#else
void *CsrMemCalloc(CsrSize numberOfElements, CsrSize elementSize)
#endif
{
    void *hdr;

    if (headerSize + tailSize)
    {
        /* Adjust element count to fit header and tail space. */

        /* +1 is to avoid expensive modulo operation */
        numberOfElements += (headerSize + tailSize) / elementSize + 1;
    }

    hdr = calloc(numberOfElements, elementSize);

#ifdef CSR_MEMALLOC_PROFILING
    if ((hdr != NULL) && (cbAlloc != NULL))
    {
#ifdef CSR_MEM_DEBUG
        cbAlloc(hdr, hdrBuf(hdr, headerSize), numberOfElements, elementSize,
            file, line);
#else
        cbAlloc(hdr, hdrBuf(hdr, headerSize), numberOfElements, elementSize,
            "n/a", 0);
#endif
    }
#endif

    /*lint -save -e429 */
    return hdrBuf(hdr, headerSize);
    /*lint -restore */
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemFree
 *
 *  DESCRIPTION
 *      Free dynamic allocated memory.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrMemFree(void *pointer)
{
    if (pointer == NULL)
    {
        return;
    }
#ifdef CSR_MEMALLOC_PROFILING
    else if (cbFree != NULL)
    {
        cbFree(bufHdr(pointer, headerSize), pointer);
    }
#endif

    free(bufHdr(pointer, headerSize));
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemAllocDma
 *
 *  DESCRIPTION
 *      Allocate dynamic memory suitable for DMA transfers.
 *
 *  RETURNS
 *      Pointer to allocated memory, or NULL in case of failure.
 *      Allocated memory is not initialised.
 *
 *----------------------------------------------------------------------------*/
#ifdef CSR_MEM_DEBUG
#undef CsrMemAllocDma
void *CsrMemAllocDma(CsrSize size)
{
    return CsrMemAllocDmaDebug(size, __FILE__, __LINE__);
}

void *CsrMemAllocDmaDebug(CsrSize size,
    const CsrCharString *file, CsrUint32 line)
#else
void *CsrMemAllocDma(CsrSize size)
#endif
{
    void *hdr;

    hdr = malloc(size + headerSize + tailSize);

#ifdef CSR_MEMALLOC_PROFILING
    if ((hdr != NULL) && (cbAllocDma != NULL))
    {
#ifdef CSR_MEM_DEBUG
        cbAllocDma(hdr, hdrBuf(hdr, headerSize), 1, size, file, line);
#else
        cbAllocDma(hdr, hdrBuf(hdr, headerSize), 1, size, "n/a", 0);
#endif
    }
#endif

    /*lint -save -e429 */
    return hdrBuf(hdr, headerSize);
    /*lint -restore */
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemFreeDma
 *
 *  DESCRIPTION
 *      Free dynamic memory allocated by CsrMemAllocDma.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrMemFreeDma(void *pointer)
{
    if (pointer == NULL)
    {
        return;
    }
#ifdef CSR_MEMALLOC_PROFILING
    else if (cbFreeDma != NULL)
    {
        cbFreeDma(bufHdr(pointer, headerSize), pointer);
    }
#endif

    free(bufHdr(pointer, headerSize));
}
