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

#include <nucleus.h>

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

    CsrPmemFree(mempool);
}

void CsrMemBufFree(void *ph, CsrMemBuf *buf)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm;
    INT intrLevel;

    mempool = (struct csrMemBufPool *) ph;
    elm = (CsrMemBuf *) buf;

    intrLevel = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS);

    elm->next = mempool->freelist;
    mempool->freelist = elm;

    NU_Local_Control_Interrupts(intrLevel);
}

CsrMemBuf *CsrMemBufGet(void *ph)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm;
    INT intrLevel;

    mempool = (struct csrMemBufPool *) ph;

    intrLevel = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS);

    elm = mempool->head;
    mempool->head = NULL;
    mempool->tail = NULL;

    /* Restore interrupt setting. */
    NU_Local_Control_Interrupts(intrLevel);

    return elm;
}

/*
 * The following functions are called from interrupt context
 * and thus the locking is implicit.
 */

void *CsrMemBufAlloc(void *ph, CsrMemBuf **buf, void **context)
{
    struct csrMemBufPool *mempool;
    CsrMemBuf *elm;
    INT intrLevel;

    mempool = (struct csrMemBufPool *) ph;

    intrLevel = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS);

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

    NU_Local_Control_Interrupts(intrLevel);

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
    }
    else
    {
        /* Return to free list */
        elm->next = mempool->freelist;
        mempool->freelist = elm;
    }
}
