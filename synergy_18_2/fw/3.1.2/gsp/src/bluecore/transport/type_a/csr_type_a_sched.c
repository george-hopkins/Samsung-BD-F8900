/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* drive the protocol in type_a_protocol.c - defined in type_a_protocol.h */

#include "csr_types.h"
#include "csr_panic.h"
#include "csr_pmem.h"
#include "csr_membuf.h"
#include "csr_sched.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_transport_descriptor.h"
#include "csr_tm_bluecore_type_a.h"
#include "csr_log_gsp.h"
#include "csr_type_a_com.h"
#include "csr_macro.h"

#include "csr_util.h"
#include "csr_type_a_private.h"
#include "csr_type_a_protocol.h"

/* local structures */
typedef struct TypeATxQueueElm
{
    CsrUint8                channel;
    CsrUint32               length;
    void                   *data;
    struct TypeATxQueueElm *next;
} TypeATxQueueElm;

typedef struct
{
    TypeATxQueueElm *first;
    TypeATxQueueElm *last;
} TypeATxQueue;

/* local vars */
static TypeATxQueue *tx_queue = NULL; /* msg from host stack to chip */

static void *pSdioDriverInstData = NULL;


/*************************************************************
 * NAME:
 *      SdioBtADrvRx
 *
 * DESCRIPTION:
 *      This function is called from the Type-A sdioABgintRxDone()
 *      bgint handler when a packet (ACL, HCI or SCO) is received.
 *      The function passes the data to the higher layers.
 *
 * RETURNS:
 *      -
 *
 *************************************************************/
static void SdioBtADrvRx(void *poolHandle)
{
    CsrMemBuf *elm;

    elm = CsrMemBufGet(poolHandle);

    while (elm)
    {
        CsrMemBuf *next;
        MessageStructure msg;

        next = elm->next;

        msg.buf = elm->buf;
        msg.buflen = (CsrUint32) elm->buflen;
        msg.chan = elm->chan;
        msg.dex = 0;

        /* pass pointer to data to upper layers */
        CsrTmBlueCoreTypeADeliverMsg(&msg, 0);

        CsrMemBufFree(poolHandle, elm);
        elm = next;
    }
}

/*---   Static functions   ---*/

static void createAndInit_ReadQueue(TypeATxQueue **ppQueue)
{
    TypeATxQueue *pQueue;

    pQueue = *ppQueue = (TypeATxQueue *) CsrPmemAlloc(sizeof(TypeATxQueue));
    pQueue->first = NULL;
    pQueue->last = NULL;
}

static void remove_ReadQueue(TypeATxQueue **ppQueue)
{
    TypeATxQueue *pQueue = *ppQueue;
    TypeATxQueueElm *q;

    /* Free the queue elements */
    while ((q = pQueue->first) != NULL)
    {
        pQueue->first = q->next;
        /* Free data and element */
        if (q->data != NULL)
        {
            CsrPmemFree(q->data);
            q->data = NULL;
            q->next = NULL;
        }
        CsrPmemFree(q);
    }

    /* Free the queue itself */
    CsrPmemFree(pQueue);
    *ppQueue = NULL;
} /* remove_ReadQueue */

/* normal CsrSched context */
CsrBool CsrSdioBtADrvStart(CsrUint8 reset)
{
    CSR_UNUSED(reset);

    createAndInit_ReadQueue(&tx_queue);

    /* initialize */
    CsrSdioAProtInit(&pSdioDriverInstData);

    return TRUE;
}

CsrBool CsrSdioBtADrvStop(void)
{
    /* deinitialize protocol handler */
    CsrSdioAProtDeinit(&pSdioDriverInstData);

    remove_ReadQueue(&tx_queue); /* rx and tx got same queue structure */

    return TRUE;
}

CsrBool CsrSdioBtADrvRestart(void)
{
    remove_ReadQueue(&tx_queue); /* rx and tx got same queue structure */

    createAndInit_ReadQueue(&tx_queue);

    CsrSdioAProtReinit(&pSdioDriverInstData);

    return TRUE;
}

/*************************************************************
 * NAME:
 *      CsrSdioBtADrvTx
 *
 * DESCRIPTION:
 *      This function is called by the core to transmit a packet.
 *      Data is put on queue and tx_thread will do the
 *      communication.
 *
 * RETURNS:
 *      -
 *************************************************************/
CsrBool CsrSdioBtADrvTx(CsrUint8 *data, CsrUint32 size)
{
    TypeATxQueueElm *p;
    CsrUint8 channel = data[3];

    p = (TypeATxQueueElm *) CsrPmemAlloc(sizeof(TypeATxQueueElm));
    p->channel = channel;
    p->length = size;
    p->data = data;
    p->next = NULL;

    if (tx_queue->first == NULL)
    {
        tx_queue->first = p;
        tx_queue->last = p;
    }
    else
    {
        tx_queue->last->next = p;
        tx_queue->last = p;
    }

    /* Notify driver of pending message */
    CsrSdioATxQueueSignal(pSdioDriverInstData);

    return TRUE;
}

void CsrTypeATxQueueMessageRemove(SdioATxData **ppTxData)
{
    SdioATxData *pTxData;

    pTxData = *ppTxData;
    CsrPmemFree(pTxData->buf);
    CsrPmemFree(pTxData);

    *ppTxData = NULL;
}

CsrInt32 CsrTypeATxQueueMessageGet(SdioATxData **ppTxData)
{
    CsrInt32 retVal;
    TypeATxQueueElm *ptr;
    SdioATxData *pTxData;

    if (tx_queue->first)
    {
        pTxData = *ppTxData = CsrPmemZalloc(sizeof(SdioATxData));

        ptr = tx_queue->first;
        pTxData->buf = ptr->data;
        pTxData->bufLen = ptr->length;

        tx_queue->first = tx_queue->first->next;
        if (tx_queue->last == ptr)
        {
            tx_queue->last = NULL;
        }
        CsrPmemFree(ptr);

        retVal = 0;
    }
    else
    {
        retVal = -1;
    }


    return retVal;
}

/* data received from chip */
void CsrTypeARxQueueMsgPut(void *poolHandle)
{
    SdioBtADrvRx(poolHandle); /* signal the scheduler that data is ready */
}

void CsrSdioBtACardEvent(CsrSdioFunction *f, CsrBool inserted)
{
    CsrSdioAInstanceData *instData;

    if (TRUE == inserted)
    {
        instData = pSdioDriverInstData;

        if (f->features & CSR_SDIO_FEATURE_DMA_CAPABLE_MEM_REQUIRED)
        {
            instData->rxPool = CsrMemBufInit(CSR_TYPE_A_RXBUF_POOLSIZE,
                CSR_TYPE_A_RXBUF_ELMSIZE, CSR_MEMBUF_FLAG_DMA);
            instData->txPool = CsrMemBufInit(1, CSR_TYPE_A_TXBUF_SIZE,
                CSR_MEMBUF_FLAG_DMA);
        }
        else
        {
            instData->rxPool = CsrMemBufInit(CSR_TYPE_A_RXBUF_POOLSIZE,
                CSR_TYPE_A_RXBUF_ELMSIZE, 0);
            instData->txPool = CsrMemBufInit(1, CSR_TYPE_A_TXBUF_SIZE,
                0);
        }

        /* Allocate room for header. */
        instData->rxMsg.hdr = CsrMemBufAlloc(instData->rxPool,
            &instData->rxMsg.hdrBuf, NULL);

        /* Allocate the tx buffer. */
        instData->txBuffer = CsrMemBufAlloc(instData->txPool,
            &instData->txBuf, NULL);

        /* Tell the driver that something happened. */
        CsrSdioACardEvent(instData, f, inserted);
    }
    else
    {
        /* We don't handle these paths yet. */
    }
}
