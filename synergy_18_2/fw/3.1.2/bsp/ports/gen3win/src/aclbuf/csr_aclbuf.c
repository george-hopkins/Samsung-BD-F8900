/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#define DEBUGLEVEL 0
#if DEBUGLEVEL > 0
#include <stdio.h>
#define DPRINTF(x) printf x
#else
#define DPRINTF(x)
#endif /* DEBUGLEVEL > 0 */

#if 0
#define MODULETEST
#endif

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_panic.h"
#include "csr_aclbuf.h"
#include "csr_sdio.h"
#include "csr_aclbuf_lower.h"
#include "platform/csr_aclbuf_init.h"

#include <windows.h>

#define ACLBUF_GPS_PACKET_SIZE  (0x800) /* Excluding headers. */
#define ACLBUF_ELM_IN_BATCH     (25)
#define ACLBUF_BATCHS_IN_POOL   (100)

/* Must match defines from hci_prim.h without ACLBUF prefix */
#define ACLBUF_HCI_PKT_BOUNDARY_FLAG_START_NONFLUSH ((CsrUint16) 0)
#define ACLBUF_HCI_PKT_BOUNDARY_FLAG_CONT           ((CsrUint16) 0x1000)
#define ACLBUF_HCI_PKT_BOUNDARY_FLAG_START_FLUSH    ((CsrUint16) 0x2000)
#define ACLBUF_HCI_PKT_BOUNDARY_MASK                ((CsrUint16) 0x3000)

typedef struct CsrAclSubscriber CsrAclSubscriber;
typedef struct CsrAclBufElmImpl CsrAclBufElmImpl;

struct CsrAclBufElmImpl
{
    struct CsrAclBufElmImpl *next; /* Must be 1st */
    CsrUint16                chanAndFlags; /* Must be 2nd */
    CsrUint16                size; /* Must be 3rd */
    void                    *data; /* Must be 4th */
    CsrBool                  isFragment;
    CsrUint16                fragmentSize;
    CsrAclSubscriber        *s;
};

struct CsrAclSubscriber
{
    struct CsrAclSubscriber *next;
    CsrUint16                chan;
    CsrBool                  doBuffering;
    CsrAclBufReady           bufReady;
    CsrAclBufSizer           aclSizer;
    void                    *privArg;
    CsrAclBufElmImpl        *freePool;
    CsrAclBufElmImpl        *activeBatch;
    CsrAclBufElmImpl        *activeBatchLast;
    CsrUint16                activeBatchSize;
    CRITICAL_SECTION         freePoolMutex;
    CRITICAL_SECTION         activeBatchMutex;
    CsrAclBufElmImpl       **batchQueue;
    CRITICAL_SECTION         batchQueueMutex;
};

static CsrAclSubscriber *subList = NULL;

static CsrBool csrAclBufRegister(CsrUint16 chan,
    CsrBool doBuffering,
    CsrAclBufReady bufReady,
    CsrAclBufSizer aclSizer,
    void *privArg)
{
    CsrUint16 i;
    CsrUint32 poolSize;
    CsrAclSubscriber *s;
    CsrAclBufElmImpl *elm;

    /* Check if subscription on chan already exists. */
    for (s = subList; s != NULL; s = s->next)
    {
        if (s->chan == chan)
        {
            DPRINTF(("ACLBUF: Register failed. Channel 0x%x already subscribed\n", chan));
            return FALSE;
        }
    }

    /* Create new subscriber and insert at head of list. */
    s = CsrPmemAlloc(sizeof(CsrAclSubscriber));
    s->next = subList;
    subList = s;
    s->chan = chan;
    s->doBuffering = doBuffering;
    s->bufReady = bufReady;
    s->aclSizer = aclSizer;
    s->privArg = privArg;
    s->activeBatch = NULL;
    s->activeBatchLast = NULL;
    s->activeBatchSize = 0;
    s->freePool = NULL;
    InitializeCriticalSection(&s->freePoolMutex);
    InitializeCriticalSection(&s->activeBatchMutex);
    InitializeCriticalSection(&s->batchQueueMutex);

    /* Allocate pool elements. */
    if (s->doBuffering)
    {
        poolSize = ACLBUF_ELM_IN_BATCH * ACLBUF_BATCHS_IN_POOL;
    }
    else
    {
        poolSize = ACLBUF_BATCHS_IN_POOL;
    }
    EnterCriticalSection(&s->freePoolMutex);
    for (i = 0; i < poolSize; i++)
    {
        elm = CsrPmemAlloc(sizeof(CsrAclBufElmImpl));
        elm->data = CsrPmemAlloc(ACLBUF_GPS_PACKET_SIZE);
        elm->chanAndFlags = 0;
        elm->size = 0;
        elm->next = s->freePool;
        elm->isFragment = FALSE;
        elm->fragmentSize = 0;
        s->freePool = elm;
        elm = NULL;
    }
    /* Allocate full batch deliver queue */
    s->batchQueue = CsrPmemZalloc(ACLBUF_BATCHS_IN_POOL * sizeof(CsrAclBufElmImpl *));

    LeaveCriticalSection(&s->freePoolMutex);

    DPRINTF(("ACLBUF: Channel 0x%x registered with batching %d\n", chan, doBuffering));
    return TRUE;
}

static void csrAclBufDeregister(CsrUint16 chan)
{
    CsrAclSubscriber *s;
    CsrAclSubscriber *prev;
    CsrAclBufElmImpl *elm, *next;

    /* Find subscriber. */
    for (s = subList, prev = NULL; s != NULL; prev = s, s = s->next)
    {
        if (s->chan == chan)
        {
            break;
        }
    }
    if (s == NULL)
    {
        return;
    }

    /* Remove subscriber from list. */
    if (prev != NULL)
    {
        prev->next = s->next;
    }
    else
    {
        subList = NULL;
    }

    /* Free memory for found subscriber. */
    EnterCriticalSection(&s->freePoolMutex);
    elm = s->freePool;
    s->freePool = NULL;
    LeaveCriticalSection(&s->freePoolMutex);
    while (elm != NULL)
    {
        next = elm->next;
        CsrPmemFree(elm->data);
        elm->data = NULL;
        CsrPmemFree(elm);
        elm = next;
    }
    EnterCriticalSection(&s->activeBatchMutex);
    elm = s->activeBatch;
    s->activeBatch = NULL;
    s->activeBatchLast = NULL;
    s->activeBatchSize = 0;
    LeaveCriticalSection(&s->activeBatchMutex);
    while (elm != NULL)
    {
        next = elm->next;
        CsrPmemFree(elm->data);
        elm->data = NULL;
        CsrPmemFree(elm);
        elm = next;
    }
    EnterCriticalSection(&s->batchQueueMutex);
    CsrPmemFree(s->batchQueue);
    LeaveCriticalSection(&s->batchQueueMutex);

    DeleteCriticalSection(&s->freePoolMutex);
    DeleteCriticalSection(&s->activeBatchMutex);
    DeleteCriticalSection(&s->batchQueueMutex);

    CsrPmemFree(s);
}

static void csrAclBufFree(CsrAclBufElm **bufElm)
{
    CsrAclSubscriber *s;
    CsrAclBufElmImpl *elm, *next;
    CsrUint16 chan;

    elm = (CsrAclBufElmImpl *) *bufElm;
    chan = elm->chanAndFlags & 0x0fff;

    DPRINTF(("ACLBUF: Free buffer for channel 0x%x\n", chan));

    /* Find subscriber. */
    for (s = subList; s != NULL; s = s->next)
    {
        if (s->chan == chan)
        {
            break;
        }
    }

    /* If no subscriber, free elm and don't return to pool;
       else return elm to head of free pool. */
    if (s == NULL)
    {
        while (elm != NULL)
        {
            next = elm->next;
            CsrPmemFree(elm->data);
            elm->data = NULL;
            CsrPmemFree(elm);
            elm = next;
        }
        DPRINTF(("ACLBUF: No subscriber for released buffer - freeing memory\n"));
    }
    else
    {
        while (elm != NULL)
        {
            next = elm->next;
            elm->chanAndFlags = 0;
            elm->size = 0;
            elm->isFragment = FALSE;
            elm->fragmentSize = 0;
            EnterCriticalSection(&s->freePoolMutex);
            elm->next = s->freePool;
            s->freePool = elm;
            LeaveCriticalSection(&s->freePoolMutex);
            elm = next;
        }
        DPRINTF(("ACLBUF: Released buffer inserted in free pool\n"));
    }

    *bufElm = NULL;
}

static CsrAclBufElm *csrAclBufFlush(CsrUint16 chan)
{
    CsrAclSubscriber *s;
    CsrAclBufElmImpl *elm;

    /* Find subscriber. */
    for (s = subList; s != NULL; s = s->next)
    {
        if (s->chan == chan)
        {
            break;
        }
    }

    if (s == NULL)
    {
        return NULL;
    }

    EnterCriticalSection(&s->activeBatchMutex);
    elm = s->activeBatch;
    s->activeBatch = NULL;
    s->activeBatchLast = NULL;
    s->activeBatchSize = 0;
    LeaveCriticalSection(&s->activeBatchMutex);

    return (CsrAclBufElm *) elm;
}

static void csrAclBufBatchFull(void)
{
    CsrAclSubscriber *s;

    /* Find subscriber. */
    for (s = subList; s != NULL; s = s->next)
    {
        int i;

        EnterCriticalSection(&s->batchQueueMutex);

        for (i = 0; i < ACLBUF_BATCHS_IN_POOL; i++)
        {
            if (s->batchQueue[i] != NULL)
            {
                DPRINTF(("ACLBUF: bufReady() on index: %u\n", i));
                s->bufReady((CsrAclBufElm *) s->batchQueue[i], s->privArg);
                s->batchQueue[i] = NULL;
            }
        }

        LeaveCriticalSection(&s->batchQueueMutex);
    }
}

static CsrAclBufCb csrAclBufDeliver(CsrUint16 chanAndFlags,
    void *ctx,
    CsrUint16 size)
{
    CsrAclBufElmImpl *elm;
    CsrAclSubscriber *s;
    CsrUint16 i;

    DPRINTF(("ACLBUF: Deliver buffer: ctx:%p, chanAndFlags=0x%x, size=%d\n", ctx, chanAndFlags, size));

    elm = (CsrAclBufElmImpl *) ctx;
    if (!elm)
    {
        return NULL;
    }

    s = elm->s;

    if (size == 0)
    {
        csrAclBufFree((CsrAclBufElm **) &elm);
        return NULL;
    }

    if ((size == elm->size) || (elm->isFragment && (elm->fragmentSize == elm->size)))
    {
        /* Update active batch. */
        EnterCriticalSection(&s->activeBatchMutex);
        s->activeBatchSize++;
        if (s->activeBatch == NULL)
        {
            s->activeBatch = elm;
            s->activeBatchLast = elm;
        }
        else
        {
            s->activeBatchLast->next = elm;
            s->activeBatchLast = elm;
        }
        LeaveCriticalSection(&s->activeBatchMutex);

        /* If batch is full - deliver it. */
        if ((s->doBuffering && (s->activeBatchSize == ACLBUF_ELM_IN_BATCH)) ||
            (!s->doBuffering && (s->activeBatchSize == 1)))
        {
            EnterCriticalSection(&s->activeBatchMutex);
            elm = s->activeBatch;
            s->activeBatch = NULL;
            s->activeBatchLast = NULL;
            s->activeBatchSize = 0;
            LeaveCriticalSection(&s->activeBatchMutex);

            /* Enqueue batch and return HISR function */
            EnterCriticalSection(&s->batchQueueMutex);
            for (i = 0; i < ACLBUF_BATCHS_IN_POOL; i++)
            {
                if (s->batchQueue[i] == NULL)
                {
                    break;
                }
            }

            if (i < ACLBUF_BATCHS_IN_POOL)
            {
                CsrAclBufCb cb;

                s->batchQueue[i] = elm;
                DPRINTF(("ACLBUF: batch full\n"));

#ifdef MODULETEST
                csrAclBufBatchFull(NULL, 0);
#else
                LeaveCriticalSection(&s->batchQueueMutex);

                cb = csrAclBufBatchFull;
                return cb;
#endif
            }
            else
            {
                CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_EXCEPTION,
                    "No free batch queue elements");
            }
            LeaveCriticalSection(&s->batchQueueMutex);
        }
    }
    else if (elm->isFragment && (elm->fragmentSize < elm->size))
    {
        /* do nothing */
    }
    else
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_EXCEPTION,
            "aclbuf deliver size invalid");
    }

    return NULL;
}

static CsrAclSubscriber *csrAclSubscriberFor(CsrUint16 chanAndFlags)
{
    CsrAclSubscriber *s;
    CsrUint16 chan;

    chan = chanAndFlags & 0x0fff;

    DPRINTF(("ACLBUF: Susbcriber for: chan=0x%x\n", chan));

    /* Find subscriber. */
    for (s = subList; s != NULL; s = s->next)
    {
        if (s->chan == chan)
        {
            return s;
        }
    }

    return NULL;
}

static CsrUint8 csrAclBufAlloc(CsrUint16 chanAndFlags,
    CsrUint16 size,
    void *hdr,
    CsrUint16 hdrSize,
    void **ctx,
    CsrUint8 **data)
{
    CsrAclSubscriber *s;
    CsrAclBufElmImpl *elm;
    CsrBool startFragment;

    startFragment = ((CsrUint16) (chanAndFlags & ACLBUF_HCI_PKT_BOUNDARY_MASK)) != ACLBUF_HCI_PKT_BOUNDARY_FLAG_CONT;

    DPRINTF(("ACLBUF: Allocate buffer: [%s] chan=0x%x, size=%d\n", startFragment ? "S" : "C", chanAndFlags & 0x0fff, size));

    /* Find subscriber. */
    s = csrAclSubscriberFor(chanAndFlags);
    if (s == NULL)
    {
        DPRINTF(("ACLBUF: No subscriber for chan=0x%x found\n", chanAndFlags & 0x0fff));
        return CSR_ACLBUF_NONE;
    }

    EnterCriticalSection(&s->freePoolMutex);
    if ((s->freePool == NULL) || (size == 0) || (size > ACLBUF_GPS_PACKET_SIZE))
    {
        DPRINTF(("ACLBUF: No free buffers for size=%d\n", size));
        LeaveCriticalSection(&s->freePoolMutex);
        return CSR_ACLBUF_NOMEM;
    }

    elm = s->freePool;
    /* If ACL packet is start packet, determine data size from data hdr. */
    if (startFragment)
    {
        CsrInt32 dataSize;
        /* If new start packet before fragment complete,
         * deliver fragments received so far.
         */
        if (elm->isFragment)
        {
            s->freePool = elm->next;
            elm->next = NULL;
            elm->size = elm->fragmentSize;
            csrAclBufDeliver(elm->chanAndFlags, elm, elm->size);
            elm = s->freePool;
            if (elm == NULL)
            {
                DPRINTF(("ACLBUF: No free buffers (empty free pool)\n"));
                LeaveCriticalSection(&s->freePoolMutex);
                return CSR_ACLBUF_NOMEM;
            }
        }

        dataSize = s->aclSizer(chanAndFlags, hdrSize, hdr);
        if ((dataSize > size) && (dataSize <= ACLBUF_GPS_PACKET_SIZE))
        {
            elm->isFragment = TRUE;
            elm->fragmentSize = size;
            elm->size = (CsrUint16) dataSize;
        }
        else
        {
            s->freePool = elm->next;
            elm->next = NULL;
            elm->size = size;
        }
        elm->chanAndFlags = chanAndFlags;
        elm->s = s;
        *ctx = elm;
        *data = elm->data;
    }
    else
    {
        /* Handle continuation packets. */

        if (elm->isFragment)
        {
            /* If accumulated fragment size is bounded by total payload size,
             * continue accumulating fragments. Otherwise, deliver fragments
             * received so far and return next free elm.
             */
            if (elm->fragmentSize + size <= elm->size)
            {
                *ctx = elm;
                *data = (CsrUint8 *) elm->data + elm->fragmentSize;
                elm->fragmentSize += size;
                if (elm->fragmentSize == elm->size)
                {
                    s->freePool = elm->next;
                    elm->next = NULL;
                }
            }
            else
            {
                s->freePool = elm->next;
                elm->next = NULL;
                elm->size = elm->fragmentSize;
                csrAclBufDeliver(elm->chanAndFlags, elm, elm->size);
                elm = s->freePool;
                if (elm == NULL)
                {
                    DPRINTF(("ACLBUF: No free buffers (empty free pool)\n"));
                    LeaveCriticalSection(&s->freePoolMutex);
                    return CSR_ACLBUF_NOMEM;
                }
                s->freePool = elm->next;
                elm->next = NULL;
                elm->size = size;
                elm->chanAndFlags = chanAndFlags;
                elm->s = s;
                *ctx = elm;
                *data = elm->data;
            }
        }
        else
        {
            s->freePool = elm->next;
            elm->next = NULL;
            elm->size = size;
            elm->chanAndFlags = chanAndFlags;
            elm->s = s;
            *ctx = elm;
            *data = elm->data;
        }
    }
    LeaveCriticalSection(&s->freePoolMutex);

    return CSR_ACLBUF_SUCCESS;
}

static void csrAclBufDeinit(CsrAclBuf **p)
{
    CsrPmemFree(*p);
    *p = NULL;
}

CsrAclBuf *CsrAclBufInit(void)
{
    CsrAclBuf *p = CsrPmemAlloc(sizeof(CsrAclBuf));
    p->reg = csrAclBufRegister;
    p->dereg = csrAclBufDeregister;
    p->free = csrAclBufFree;
    p->flush = csrAclBufFlush;
    p->deinit = csrAclBufDeinit;
    CsrAclBufLowerInit(csrAclBufAlloc,
        csrAclBufDeliver);

    return p;
}

#if DEBUGLEVEL > 0
void CsrAclBufDump(CsrUint16 chan)
{
    CsrAclBufElmImpl *elm;
    CsrAclSubscriber *s;

    /* Find subscriber. */
    for (s = subList; s != NULL; s = s->next)
    {
        if (s->chan == chan)
        {
            break;
        }
    }
    if (s == NULL)
    {
        DPRINTF(("No subscriber for chan=0x%x found\n", chan));
        return;
    }

    /* Dump freepool head. */
    elm = s->freePool;
    DPRINTF(("\n***freePool head:\n"));
    if (elm != NULL)
    {
        DPRINTF(("next=0x%04x\n", elm->next));
        DPRINTF(("chanAndFlags=0x%02x\n", elm->chanAndFlags));
        DPRINTF(("size=%d\n", elm->size));
        DPRINTF(("data=0x%04x\n", elm->data));
        DPRINTF(("isFragment=%s\n", (elm->isFragment) ? "TRUE" : "FALSE"));
        DPRINTF(("fragmentSize=0x%02x\n", elm->fragmentSize));
    }
    else
    {
        DPRINTF(("NULL\n"));
    }
    /* Dump activebatch head and tail. */
    elm = s->activeBatch;
    DPRINTF(("\n***activeBatch head:\n"));
    if (elm != NULL)
    {
        DPRINTF(("next=0x%04x\n", elm->next));
        DPRINTF(("chanAndFlags=0x%02x\n", elm->chanAndFlags));
        DPRINTF(("size=%d\n", elm->size));
        DPRINTF(("data=0x%04x\n", elm->data));
        DPRINTF(("isFragment=%s\n", (elm->isFragment) ? "TRUE" : "FALSE"));
        DPRINTF(("fragmentSize=0x%02x\n", elm->fragmentSize));
    }
    else
    {
        DPRINTF(("NULL\n"));
    }
}

#endif
