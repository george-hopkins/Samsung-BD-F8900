/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_pmem_hook.h"
#include "csr_panic.h"
#include "csr_framework_ext.h"
#include "csr_memstat.h"

typedef struct appMemInfoTag
{
    CsrMemStatApp appStat;
    CsrBool       free;
} appMemInfoList;

/*
 * memAllocHdr: prev/next pointers split in
 * odd/even bits parts to make purify
 * able to detect memleaks by not having a
 * real pointer to any allocation.
 *
 * Get odd/even bits of pointer bytes.
 */
#define ptrInt(addr)    ((CsrIntptr) ((void *) (addr)))
#define intPtr(intval)  ((struct memAllocHdrTag *) (intval))
#define highAddr(addr)  (intPtr(ptrInt(addr) & 0x55555555))
#define lowAddr(addr)   (intPtr(ptrInt(addr) & 0xaaaaaaaa))
/*
 * Combine odd/even address parts.
 */
#define nextAddr(hdr)   (intPtr(ptrInt((hdr)->next_low) | ptrInt((hdr)->next_high)))
#define prevAddr(hdr)   (intPtr(ptrInt((hdr)->prev_low) | ptrInt((hdr)->prev_high)))

typedef struct memAllocHdrTag
{
    CsrSize              size;
    CsrUint32            magic;
    const CsrCharString *file;
    CsrUint32            line;
    CsrMemStatApp       *appStat;

    struct memAllocHdrTag *prev_low;
    struct memAllocHdrTag *prev_high;
    struct memAllocHdrTag *next_low;
    struct memAllocHdrTag *next_high;
} memAllocHdr;

typedef struct
{
    CsrMemStat global;
    CsrMemStat interval[CSR_MEMALLOC_PROFILING_RANGES];

    appMemInfoList appUsageList[CSR_MEMALLOC_PROFILING_TASKS];
} memStats;

static CsrMutexHandle statsLock;
static memAllocHdr *allocList;
static memStats stats;

#ifdef CSR_MEMALLOC_PROFILING
static CsrUint8 sizeToStatsIndex(CsrSize sz)
{
    sz /= CSR_MEMALLOC_PROFILING_INTERVAL;

    if (sz > (CSR_MEMALLOC_PROFILING_RANGES - 1))
    {
        /* Clamp to statistics array. */
        sz = CSR_MEMALLOC_PROFILING_RANGES - 1;
    }

    return sz;
}
#endif

#define MEMORY_MAGIC 0x28111981

#ifdef CSR_MEMALLOC_PROFILING
static CsrMemStatApp *appStatGet(void)
{
    CsrThreadHandle hdl;

    if (CsrThreadGetHandle(&hdl) == CSR_RESULT_SUCCESS)
    {
        CsrUint32 idx;
        CsrSchedQid qid;

        qid = CsrSchedTaskQueueGet();

        for (idx = 0; idx < CSR_MEMALLOC_PROFILING_TASKS; idx++)
        {
            appMemInfoList *appInfo;
            CsrMemStatApp *appStat;

            appInfo = &stats.appUsageList[idx];
            appStat = &appInfo->appStat;

            if ((appInfo->free == FALSE) &&
                (appStat->threadId == hdl) &&
                (appStat->taskId == qid))
            {
                /* If we find a matching entry, use it. */
                return appStat;
            }
            else if (appInfo->free)
            {
                /* If we find a free entry, this is our first allocation. */
                CsrMemSet(appInfo, 0, sizeof(*appInfo));
                appStat->threadId = hdl;
                appStat->taskId = qid;
                appInfo->free = FALSE;

                return appStat;
            }
        }
    }

    return NULL;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      memStatsAlloc
 *
 *  DESCRIPTION
 *      Called when a chunk of memory is allocated.
 *
 *  RETURNS
 *      void * - pointer to allocated block
 *
 *----------------------------------------------------------------------------*/
#ifdef CSR_MEMALLOC_PROFILING
static void memStatsAlloc(void *arg_hdr, void *buf,
    CsrSize unused /* count */, CsrSize size,
    const CsrCharString *file, CsrUint32 line)
{
    memAllocHdr *hdr;
    CsrMemStatApp *appStat;
    CsrUint8 *tail;
    CsrUint8 statIndex;

    CsrMutexLock(&statsLock);

    tail = ((CsrUint8 *) buf) + size;
    tail[0] = (MEMORY_MAGIC >> 24) & 0xff;
    tail[1] = (MEMORY_MAGIC >> 16) & 0xff;
    tail[2] = (MEMORY_MAGIC >> 8) & 0xff;
    tail[3] = (MEMORY_MAGIC >> 0) & 0xff;

    hdr = arg_hdr;
    hdr->size = size;
    hdr->magic = MEMORY_MAGIC ^ size;
    hdr->file = file;
    hdr->line = line;

    /* Element will be stored at front of the list. */
    hdr->prev_low = lowAddr(NULL);
    hdr->prev_high = highAddr(NULL);

    /* Insert allocation at list head */
    hdr->next_high = highAddr(allocList);
    hdr->next_low = lowAddr(allocList);

    if (allocList) /* allocation list not empty */
    {
        allocList->prev_high = highAddr(hdr);
        allocList->prev_low = lowAddr(hdr);
    }
    allocList = hdr;

    appStat = appStatGet();
    if (appStat == NULL)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_HEAP_EXHAUSTION,
            "out of memory (metadata)");
    }

    /* Update global allocation statistics */
    stats.global.currentAllocationCount++;
    if (stats.global.currentAllocationCount > stats.global.peakAllocationCount)
    {
        stats.global.peakAllocationCount = stats.global.currentAllocationCount;
    }

    stats.global.currentAllocationSize += size;
    if (stats.global.currentAllocationSize > stats.global.peakAllocationSize)
    {
        stats.global.peakAllocationSize = stats.global.currentAllocationSize;
    }

    statIndex = sizeToStatsIndex(size);

    stats.interval[statIndex].currentAllocationCount++;
    if (stats.interval[statIndex].currentAllocationCount >
        stats.interval[statIndex].peakAllocationCount)
    {
        stats.interval[statIndex].peakAllocationCount =
            stats.interval[statIndex].currentAllocationCount;
    }

    stats.interval[statIndex].currentAllocationSize += size;
    if (stats.interval[statIndex].currentAllocationSize >
        stats.interval[statIndex].peakAllocationSize)
    {
        stats.interval[statIndex].peakAllocationSize =
            stats.interval[statIndex].currentAllocationSize;
    }

    /* Update task allocation statistics */
    appStat->stats.currentAllocationSize += size;
    if (appStat->stats.currentAllocationSize > appStat->stats.peakAllocationSize)
    {
        appStat->stats.peakAllocationSize = appStat->stats.currentAllocationSize;
    }

    appStat->stats.currentAllocationCount++;
    if (appStat->stats.currentAllocationCount >
        appStat->stats.peakAllocationCount)
    {
        appStat->stats.peakAllocationCount =
            appStat->stats.currentAllocationCount;
    }

    hdr->appStat = appStat;

    CsrMutexUnlock(&statsLock);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      memStatsFree
 *
 *  DESCRIPTION
 *      Called when a memory block is freed.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
static void memStatsFree(void *arg_hdr, void *arg_buf)
{
    memAllocHdr *hdr;
    CsrUint8 statIndex;

    hdr = arg_hdr;

    if (hdr->magic != (hdr->size ^ MEMORY_MAGIC))
    {
        if ((~hdr->magic) == (hdr->size ^ MEMORY_MAGIC))
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER,
                "double free");
        }
        else
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_INVALID_PFREE_POINTER,
                "bad pointer");
        }
    }
    else
    {
        CsrUint8 *tail;
        CsrUint32 sig;

        tail = arg_buf;
        tail += hdr->size;

        sig = tail[0] << 24;
        sig |= tail[1] << 16;
        sig |= tail[2] << 8;
        sig |= tail[3] << 0;

        if (sig != MEMORY_MAGIC)
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
                "heap corruption detected");
        }
    }

    /*
     * Make sure magic is invalid to more
     * easily detect double frees.
     */
    hdr->magic = ~hdr->magic;

    /*
     * It is safe to delay the lock to here, because
     * it's an error if two threads are trying to
     * free the same pointer at the same time.
     */
    CsrMutexLock(&statsLock);

    if ((prevAddr(hdr) == NULL) && (nextAddr(hdr) != NULL))
    {
        /* Removing head. */
        allocList = nextAddr(hdr);
        allocList->prev_high = highAddr(NULL);
        allocList->prev_low = lowAddr(NULL);
    }
    else if ((prevAddr(hdr) != NULL) && (nextAddr(hdr) != NULL))
    {
        /* Removing element in the middle. */
        prevAddr(hdr)->next_high = highAddr(nextAddr(hdr));
        prevAddr(hdr)->next_low = lowAddr(nextAddr(hdr));

        nextAddr(hdr)->prev_high = highAddr(prevAddr(hdr));
        nextAddr(hdr)->prev_low = lowAddr(prevAddr(hdr));
    }
    else if ((prevAddr(hdr) != NULL) && (nextAddr(hdr) == NULL))
    {
        /* Removing tail. */
        prevAddr(hdr)->next_high = highAddr(NULL);
        prevAddr(hdr)->next_low = lowAddr(NULL);
    }
    else
    {
        /* Removing final element. */

        allocList = NULL;
    }

    /* Update statistics */

    hdr->appStat->stats.currentAllocationCount--;
    hdr->appStat->stats.currentAllocationSize -= hdr->size;

    statIndex = sizeToStatsIndex(hdr->size);
    stats.interval[statIndex].currentAllocationCount--;
    stats.interval[statIndex].currentAllocationSize -= hdr->size;

    stats.global.currentAllocationCount--;
    stats.global.currentAllocationSize -= hdr->size;

    CsrMutexUnlock(&statsLock);
}

void CsrMemStatTaskGet(CsrSchedQid qid, CsrMemStat *statsPtr)
{
    appMemInfoList *appInfo;
    CsrUint32 idx;

    CsrMutexLock(&statsLock);

    for (idx = 0; idx < CSR_MEMALLOC_PROFILING_TASKS; idx++)
    {
        CsrMemStatApp *appStat;

        appInfo = &stats.appUsageList[idx];
        appStat = &appInfo->appStat;

        if ((appInfo->free == FALSE) && (appStat->taskId == qid))
        {
            statsPtr->currentAllocationSize = appStat->stats.currentAllocationSize;
            statsPtr->peakAllocationSize = appStat->stats.peakAllocationSize;
            statsPtr->currentAllocationCount = appStat->stats.currentAllocationCount;
            statsPtr->peakAllocationCount = appStat->stats.peakAllocationCount;
            break;
        }
    }

    CsrMutexUnlock(&statsLock);

    if (appInfo == NULL)
    {
        statsPtr->currentAllocationSize = 0;
        statsPtr->peakAllocationSize = 0;
        statsPtr->currentAllocationCount = 0;
        statsPtr->peakAllocationCount = 0;
    }
}
#endif /* CSR_MEMALLOC_PROFILING */

CsrUint32 CsrMemStatAppGet(
    CsrMemStatApp measurements[CSR_MEMALLOC_PROFILING_TASKS])
{
    CsrUint32 idx;

    CsrMutexLock(&statsLock);

    for (idx = 0; idx < CSR_MEMALLOC_PROFILING_TASKS; idx++)
    {
        appMemInfoList *appInfo;

        appInfo = &stats.appUsageList[idx];

        CsrMemCpy(&measurements[idx], appInfo, sizeof(measurements[idx]));
    }

    CsrMutexUnlock(&statsLock);

    return idx;
}

void CsrMemStatGlobalGet(CsrMemStat *statsPtr)
{
    CsrMutexLock(&statsLock);

    statsPtr->currentAllocationSize = stats.global.currentAllocationSize;
    statsPtr->peakAllocationSize = stats.global.peakAllocationSize;
    statsPtr->currentAllocationCount = stats.global.currentAllocationCount;
    statsPtr->peakAllocationCount = stats.global.peakAllocationCount;

    CsrMutexUnlock(&statsLock);
}

void CsrMemStatProfileGet(CsrUint32 *pools, CsrUint32 *poolWidth,
    CsrMemStat **statsPtr)
{
    *pools = CSR_MEMALLOC_PROFILING_RANGES;
    *poolWidth = CSR_MEMALLOC_PROFILING_INTERVAL;
    *statsPtr = CsrPmemAlloc(sizeof(stats.interval));

    CsrMutexLock(&statsLock);
    CsrMemCpy(*statsPtr, stats.interval, sizeof(stats.interval));
    CsrMutexUnlock(&statsLock);
}

void CsrMemStatTaskPeakReset(CsrSchedQid qid, CsrBool size, CsrBool count)
{
    CsrUint32 idx;

    CsrMutexLock(&statsLock);

    for (idx = 0; idx < CSR_MEMALLOC_PROFILING_TASKS; idx++)
    {
        appMemInfoList *appInfo;
        CsrMemStatApp *appStat;

        appInfo = &stats.appUsageList[idx];
        appStat = &appInfo->appStat;

        if (appStat->taskId == qid)
        {
            if (size)
            {
                appStat->stats.peakAllocationSize =
                    appStat->stats.currentAllocationSize;
            }

            if (count)
            {
                appStat->stats.peakAllocationCount =
                    appStat->stats.currentAllocationCount;
            }
            break;
        }
    }

    CsrMutexUnlock(&statsLock);
}

void CsrMemStatGlobalPeakReset(CsrBool size, CsrBool count)
{
    CsrMutexLock(&statsLock);

    if (size)
    {
        stats.global.peakAllocationSize = stats.global.currentAllocationSize;
    }

    if (count)
    {
        stats.global.peakAllocationCount = stats.global.currentAllocationCount;
    }

    CsrMutexUnlock(&statsLock);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrMemStatInit
 *
 *  DESCRIPTION
 *      Sets up the pool control blocks and establishes the pools' free
 *      lists.   Use only at the system's initialisation.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrMemStatInit(void)
{
    CsrUint32 idx;

    allocList = NULL;

    stats.global.currentAllocationCount = 0;
    stats.global.peakAllocationCount = 0;
    stats.global.currentAllocationSize = 0;
    stats.global.peakAllocationSize = 0;

    for (idx = 0; idx < CSR_MEMALLOC_PROFILING_TASKS; idx++)
    {
        appMemInfoList *appInfo;

        appInfo = &stats.appUsageList[idx];

        appInfo->free = TRUE;
    }

    CsrMutexCreate(&statsLock);

#ifdef CSR_MEMALLOC_PROFILING
    CsrPmemHookSet(memStatsAlloc, memStatsFree,
        sizeof(memAllocHdr), sizeof(CsrUint32));
#endif
}

void CsrMemStatDeinit(void (*leakCb)(void *ptr, CsrSize sz,
        const CsrCharString *file, CsrUint32 line))
{
    if (leakCb != NULL)
    {
        memAllocHdr *hdr, *h_next;

        for (hdr = allocList;
             hdr;
             hdr = h_next)
        {
            h_next = nextAddr(hdr);

            leakCb(hdr, hdr->size, hdr->file, hdr->line);
        }
    }

    CsrMutexDestroy(&statsLock);
}
