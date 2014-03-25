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
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "nucleus.h"
#include "csr_types.h"
#include "csr_framework_ext.h"

#include "csr_mem_hook.h"

#include "platform/csr_mmu.h"
#include "platform/csr_interrupt.h"

#define CSR_THREAD_DEFAULT_MAX_TICKS     (2)
#define CSR_THREAD_DEFAULT_STACK_SIZE    (4 * 1024)
#define CSR_THREAD_DEFAULT_PRIORITY      (4)
#define CSR_THREAD_CREATE_EVENT          (0x0001)

#define CSR_NU_NAME_SIZE                 (8)

extern NU_MEMORY_POOL System_Memory;

struct CsrThread
{
    NU_TASK        task;
    NU_EVENT_GROUP taskEvent;
    OPTION         taskPriority;
    void          *taskStack;
    void           (*threadFunction)(void *pointer);
    void          *pointer;
    CsrBool        inUse;
};

static struct CsrThread *threadPool = NULL;
static CsrUint16 threadPoolSize = 0;
static CsrBool globalLock = FALSE;

static CsrUint16 mapPr(CsrUint16 pri)
{
    return (pri != 0) ? (pri << 6) - 1 : pri;
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
    STATUS status;
    char name[CSR_NU_NAME_SIZE] = "CSREVNT";

    memset(eventHandle, 0, sizeof(*eventHandle));
    status = NU_Create_Event_Group(eventHandle, name);
    if (status != NU_SUCCESS)
    {
        return CSR_FE_RESULT_INVALID_POINTER;
    }

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
    STATUS status;
    UNSIGNED suspend;

    if (timeoutInMs == 0)
    {
        suspend = NU_NO_SUSPEND;
    }
    else if (timeoutInMs == CSR_EVENT_WAIT_INFINITE)
    {
        suspend = NU_SUSPEND;
    }
    else
    {
        UNSIGNED tickCount;

        tickCount = NU_Retrieve_Tick_Interval();

        suspend = (((CsrUint32) timeoutInMs) * 1000);
        suspend += tickCount - 1; /* Round up to avoid it becoming NU_NO_SUSPEND. */
        suspend /= ((CsrUint32) tickCount);
    }

    status = NU_Retrieve_Events(eventHandle, 0xFFFFFFFF, NU_OR_CONSUME, (UNSIGNED *) eventBits, suspend);
    if (status == NU_SUCCESS)
    {
        return CSR_RESULT_SUCCESS;
    }
    else if ((status == NU_TIMEOUT) || (status == NU_NOT_PRESENT))
    {
        *eventBits = 0;

        return CSR_FE_RESULT_TIMEOUT;
    }
    else if ((status != NU_INVALID_GROUP) || (status == NU_GROUP_DELETED))
    {
        return CSR_FE_RESULT_INVALID_HANDLE;
    }
    return CSR_RESULT_FAILURE;
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
    STATUS status;

    status = NU_Set_Events(eventHandle, eventBits, NU_OR);
    if (status != NU_SUCCESS)
    {
        return CSR_FE_RESULT_INVALID_HANDLE;
    }

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
    NU_Delete_Event_Group(eventHandle);
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
    STATUS status;
    char name[CSR_NU_NAME_SIZE] = "CSRMUTX";

    memset(mutexHandle, 0, sizeof(*mutexHandle));
    status = NU_Create_Semaphore(mutexHandle, name, 1, NU_FIFO);
    if (status != NU_SUCCESS)
    {
        return CSR_FE_RESULT_INVALID_POINTER;
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
    STATUS status;

    status = NU_Obtain_Semaphore(mutexHandle, NU_SUSPEND);
    if (status != NU_SUCCESS)
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
    STATUS status;

    status = NU_Release_Semaphore(mutexHandle);
    if (status != NU_SUCCESS)
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
    NU_Delete_Semaphore(mutexHandle);
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
    CsrBool globalLockMirror;
    do
    {
        INT_ATOMIC_EXECUTE(globalLockMirror = globalLock;
            globalLock = TRUE);
    } while (globalLockMirror);
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
    INT_ATOMIC_EXECUTE(globalLock = FALSE);
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
    OPTION oldPreempt;
    struct CsrThread *t;
    CsrUint16 i;

    if (threadFunction == NULL)
    {
        return CSR_FE_RESULT_INVALID_POINTER;
    }

    /* Retrieve next free thread */
    oldPreempt = NU_Change_Preemption(NU_NO_PREEMPT);

    for (i = 0; i < threadPoolSize; i++)
    {
        if (threadPool[i].inUse == FALSE)
        {
            break;
        }
    }

    if (i == threadPoolSize)
    {
        NU_Change_Preemption(oldPreempt);
        return CSR_FE_RESULT_NO_MORE_THREADS;
    }

    t = &threadPool[i];
    t->threadFunction = threadFunction;
    t->pointer = pointer;
    t->taskPriority = (OPTION) mapPr(priority);
    t->inUse = TRUE;

    NU_Change_Preemption(oldPreempt);

    /* Signal to Nucleus task that thread has been created and must be called */
    NU_Set_Events(&t->taskEvent, CSR_THREAD_CREATE_EVENT, NU_OR);
    *threadHandle = &t->task;
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

    *threadHandle = NU_Current_Task_Pointer();

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
    UNSIGNED sleepTimeInUs;
    UNSIGNED tickInterval, ticksToSleep;

    sleepTimeInUs = sleepTimeInMs * 1000L;
    tickInterval = NU_Retrieve_Tick_Interval(); /* tick interval in us */
    ticksToSleep = (sleepTimeInUs / tickInterval);

    if ((ticksToSleep * tickInterval) < sleepTimeInUs)
    {
        /* Add tick because of rounding off error */
        ticksToSleep++;
    }

    /* Add tick to ensure sleeping for at least time specified.
     * NU_Sleep(n) sleeps between n-1 and n ticks */
    ticksToSleep++;

    NU_Sleep(ticksToSleep);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      csrThreadFunc
 *
 *  DESCRIPTION
 *      Local Nucleus task entry function - common for all static Nucleus tasks
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
static void csrTaskFunc(UNSIGNED unused, void *thread)
{
    STATUS status;
    UNSIGNED actualFlags;
    OPTION oldPreempt;
    struct CsrThread *t = (struct CsrThread *) thread;

    while (1)
    {
        status = NU_Retrieve_Events(&t->taskEvent, CSR_THREAD_CREATE_EVENT, NU_OR_CONSUME, &actualFlags, NU_SUSPEND);
        if (status == NU_SUCCESS)
        {
            /* Change task priority as set by CsrThreadCreate */
            NU_Change_Priority(&t->task, t->taskPriority);

            /* Call real thread function set by CsrThreadCreate */
            t->threadFunction(t->pointer);

            /* On termination of thread function, return to free list */
            oldPreempt = NU_Change_Preemption(NU_NO_PREEMPT);
            t->inUse = FALSE;
            NU_Change_Preemption(oldPreempt);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadInit
 *
 *  DESCRIPTION
 *      Initializes static thread pool with size number of threads
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrThreadInit(CsrUint16 size)
{
    struct CsrThread *t;
    CsrUint16 i;
    char name1[CSR_NU_NAME_SIZE] = "CSREVNT";
    char name2[CSR_NU_NAME_SIZE] = "CSRTASK";

    NU_Allocate_Memory(&System_Memory, (void **) &threadPool, size * sizeof(struct CsrThread), NU_NO_SUSPEND);
    memset(threadPool, 0, size * sizeof(*threadPool));

    for (i = 0; i < size; i++)
    {
        t = &threadPool[i];
        NU_Create_Event_Group(&t->taskEvent, name1);
        NU_Allocate_Memory(&System_Memory, (void **) &t->taskStack, CSR_THREAD_DEFAULT_STACK_SIZE, NU_NO_SUSPEND);
        memset(t->taskStack, 0, CSR_THREAD_DEFAULT_STACK_SIZE);
        t->taskPriority = mapPr(CSR_THREAD_DEFAULT_PRIORITY);
        NU_Create_Task(&t->task, name2, csrTaskFunc, 0, t, t->taskStack, CSR_THREAD_DEFAULT_STACK_SIZE, t->taskPriority, CSR_THREAD_DEFAULT_MAX_TICKS, NU_PREEMPT, NU_START);
        t->threadFunction = NULL;
        t->pointer = NULL;
        t->inUse = FALSE;
    }

    threadPoolSize = size;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadDeinit
 *
 *  DESCRIPTION
 *      Deinit static thread pool
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrThreadDeinit(void)
{
    CsrUint16 i;
    struct CsrThread *t;

    for (i = 0; i < threadPoolSize; i++)
    {
        t = &threadPool[i];
        NU_Delete_Event_Group(&t->taskEvent);
        NU_Terminate_Task(&t->task);
        NU_Delete_Task(&t->task);
        NU_Deallocate_Memory((void *) t->taskStack);
    }

    NU_Deallocate_Memory((void *) threadPool);
}

#define bufHdr(ptr, hlen)       ((((CsrUint8 *) (ptr)) - hlen))
#define hdrBuf(hdr, hlen)       (((CsrUint8 *) (hdr)) + hlen)

static void *memAlloc(CsrSize size)
{
    void *hdr;
    STATUS status;

    if (size == 0)
    {
        size = 1;
    }

    status = NU_Allocate_Memory(&System_Memory, &hdr,
        size, NU_NO_SUSPEND);

    if (status != NU_SUCCESS)
    {
        hdr = NULL;
    }

    return hdr;
}

static void memFree(void *pointer)
{
    if (pointer != NULL)
    {
        NU_Deallocate_Memory(pointer);
    }
}

#ifdef CSR_MEMALLOC_PROFILING
/* Align data buffer to an 8 byte boundary. */
#define bufAlignBytes           8
#define bufAlign(ptr, align)    ((ptr + (align - 1)) & ~(align - 1))

static CsrMemHookAlloc cbAlloc;
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

    hdr = memAlloc(size + headerSize + tailSize);
    if (hdr != NULL)
    {
#ifdef CSR_MEMALLOC_PROFILING
        if (cbAlloc != NULL)
        {
#ifdef CSR_MEM_DEBUG
            cbAlloc(hdr, hdrBuf(hdr, headerSize), 1, size, file, line);
#else
            cbAlloc(hdr, hdrBuf(hdr, headerSize), 1, size, "n/a", 0);
#endif
        }
#endif
        return hdrBuf(hdr, headerSize);
    }
    else
    {
        return NULL;
    }
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

#ifdef CSR_MEMALLOC_PROFILING
    if (headerSize + tailSize)
    {
        /* Adjust element count to fit header and tail space. */

        /* +1 is to avoid expensive modulo operation */
        numberOfElements += (headerSize + tailSize) / elementSize + 1;
    }
#endif

    hdr = memAlloc(numberOfElements * elementSize);
    if (hdr != NULL)
    {
        memset(hdr, 0, numberOfElements * elementSize);

#ifdef CSR_MEMALLOC_PROFILING
        if (cbAlloc != NULL)
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
        return hdrBuf(hdr, headerSize);
    }
    else
    {
        return NULL;
    }
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

    NU_Deallocate_Memory(bufHdr(pointer, headerSize));
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
    return CsrMemAllocDmaDebug(size, "n/a", 0);
}

void *CsrMemAllocDmaDebug(CsrSize size,
    const CsrCharString *file, CsrUint32 line)
#else
void *CsrMemAllocDma(CsrSize size)
#endif
{
    void *ptr;
#ifdef CSR_MEMALLOC_PROFILING
    void *hdr;

    ptr = MMUCacheLineAlignedMemoryAllocateDebug(size + tailSize, headerSize,
        &hdr, memAlloc);

    if ((ptr != NULL) && (cbAllocDma != NULL))
    {
#ifdef CSR_MEM_DEBUG
        cbAllocDma(ptr, hdr, 1, size, file, line);
#else
        cbAllocDma(ptr, hdr, 1, size, "n/a", 0);
#endif
    }
#else
    ptr = MMUCacheLineAlignedMemoryAllocate(size, memAlloc);
#endif

    return ptr;
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
#ifdef CSR_MEMALLOC_PROFILING
    if (cbFreeDma != NULL)
    {
        CsrUintptr *hdr;

        hdr = pointer;
        hdr -= sizeof(void *);
        hdr -= headerSize;

        cbFreeDma(hdr, pointer);
    }
#endif

    MMUCacheLineAlignedMemoryFree(pointer, memFree);
}

#if DEBUGLEVEL > 0
void CsrThreadDump(void)
{
    CsrUint16 i;
    struct CsrThread *t;

    for (i = 0; i < threadPoolSize; i++)
    {
        t = &threadPool[i];
        DPRINTF(("&task=0x%04x\n", (CsrUint32) & t->task));
        DPRINTF(("inUse=%d\n", t->inUse));
    }
}

#endif /* DEBUGLEVEL > 0 */
