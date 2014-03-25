/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_membuf.h"
#include "csr_pmem.h"
#include "csr_panic.h"

#include <windows.h>

struct csrMemBufPool
{
    /*
     * Freelist is a linked listed with new elements
     * inserted at head (i.e. sorted MRU->LRU).
     */
    CsrMemBuf *freelist;

    /* A tail queue of currently enqueued elements. */
    CsrMemBuf *head;
    CsrMemBuf *tail;

    /* Synchronization locks. */
    CRITICAL_SECTION lkFree;
    CRITICAL_SECTION lkActive;
};


/*
 * The following functions are called from thread context
 * and used to set up or tear down a receive buffer pool.
 */

void *CsrMemBufInit(CsrUint16 elementCount, CsrSize elementSize, CsrUint32 flags)
{
    struct csrMemBufPool *mempool;
    int i;

    CSR_UNUSED(flags);

    if ((elementCount == 0) || (elementSize == 0))
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
            "only DMA-capable memory allocation supported");
    }

    mempool = CsrPmemAlloc(sizeof(*mempool));

    mempool->freelist = NULL;

    InitializeCriticalSection(&mempool->lkFree);
    InitializeCriticalSection(&mempool->lkActive);

    for (i = 0; i < elementCount; i++)
    {
        CsrMemBuf *elm;

        elm = CsrPmemAlloc(sizeof(*elm));
        elm->buf = CsrPmemAlloc(elementSize);

        elm->next = mempool->freelist;
        mempool->freelist = elm;
    }

    mempool->head = NULL;
    mempool->tail = NULL;

    return mempool;
}

void CsrMemBufDeinit(void *ph)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm, *next;

    mempool = (struct csrMemBufPool *) ph;

    for (elm = mempool->freelist; elm; elm = next)
    {
        next = elm->next;

        CsrPmemFree(elm->buf);
        CsrPmemFree(elm);
    }

    for (elm = mempool->head; elm; elm = next)
    {
        next = elm->next;

        CsrPmemFree(elm->buf);
        CsrPmemFree(elm);
    }

    DeleteCriticalSection(&mempool->lkFree);
    DeleteCriticalSection(&mempool->lkActive);

    CsrPmemFree(mempool);
}

/*
 * The following functions are called from thread context
 * and used to return used buffer elements to the freelist
 * or to obtain the currently enqueued elements.
 * These manipulate the freelist and the rx-queue that are
 * also manipulated from interrupt context and disable
 * interrupts to ensure data integrity.
 */
void CsrMemBufFree(void *ph, CsrMemBuf *buf)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm;

    mempool = (struct csrMemBufPool *) ph;
    elm = (CsrMemBuf *) buf;

    EnterCriticalSection(&mempool->lkFree);

    elm->next = mempool->freelist;
    mempool->freelist = elm;

    LeaveCriticalSection(&mempool->lkFree);
}

CsrMemBuf *CsrMemBufGet(void *ph)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm;

    mempool = (struct csrMemBufPool *) ph;

    EnterCriticalSection(&mempool->lkActive);

    elm = mempool->head;
    mempool->head = NULL;
    mempool->tail = NULL;

    LeaveCriticalSection(&mempool->lkActive);

    return elm;
}

void *CsrMemBufAlloc(void *ph, CsrMemBuf **buf, void **context)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm;

    mempool = (struct csrMemBufPool *) ph;

    EnterCriticalSection(&mempool->lkFree);

    elm = mempool->freelist;

    if (elm == NULL)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_HEAP_EXHAUSTION,
            "out of intr memory");
    }
    else
    {
        mempool->freelist = elm->next;
    }

    LeaveCriticalSection(&mempool->lkFree);

    if (buf)
    {
        *buf = elm;
    }

    if (context)
    {
        *context = elm;
    }

    return elm->buf;
}

void CsrMemBufQueue(void *ph, void *context, CsrSize elementDataSize, CsrUint8 chan)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm;

    mempool = (struct csrMemBufPool *) ph;
    elm = (CsrMemBuf *) context;

    if (elementDataSize != 0)
    {
        EnterCriticalSection(&mempool->lkActive);

        elm->buflen = elementDataSize;
        elm->chan = chan;
        elm->next = NULL;

        if (mempool->tail == NULL)
        {
            mempool->head = elm;
            mempool->tail = elm;
        }
        else
        {
            mempool->tail->next = elm;
            mempool->tail = elm;
        }

        LeaveCriticalSection(&mempool->lkActive);
    }
    else
    {
        EnterCriticalSection(&mempool->lkFree);

        /* Return to free list */
        elm->next = mempool->freelist;
        mempool->freelist = elm;

        LeaveCriticalSection(&mempool->lkFree);
    }
}
