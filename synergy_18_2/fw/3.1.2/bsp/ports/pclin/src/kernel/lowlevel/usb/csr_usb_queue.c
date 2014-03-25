/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <linux/kernel.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif


#include <linux/module.h>
#include <linux/time.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <asm/bitops.h>

#include "csr_sched.h"
#include "csr_pmem.h"

#include "csr_tm_bluecore_usb.h"
#include "csr_transport.h"

#include "lowlevel/csr_linux_usb_com.h"
#include "lowlevel/csr_linux_usb_extra.h"

/*************************************************************
 * NAME:
 *      QueuePrepare
 *
 * DESCRIPTION:
*       Setup the queue function
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void QueuePrepare(csr_dev_t *dv)
{
    usb_queue_t *q;

    q = &(dv->queue);
    q->head = NULL;
    q->tail = NULL;
    atomic_set(&(q->count), 0);
    spin_lock_init(&(q->lock));
    init_waitqueue_head(&(q->wait));

    DBG_VERBOSE("Queue prepared\n");
}

/*************************************************************
 * NAME:
 *      QueueFree
 *
 * DESCRIPTION:
 *       Empty (free) all elements of the queue
 *
 * RETURNS:
 *      -
 *
 *************************************************************/
void QueueFree(csr_dev_t *dv)
{
    struct usb_qe *e;
    usb_queue_t *q;
    int i;
    int j;

    /* Get queue length */
    q = &(dv->queue);
    j = atomic_read(&(q->count));

    /* Remove all elements off the queue */
    i = 0;
    do
    {
        e = QueuePop(dv);

        /* If element existed, free buffer, message and element itself */
        if (e != NULL)
        {
            CsrPmemFree(e->msg->buf);
            CsrPmemFree(e->msg);
            CsrPmemFree(e);
            i++;
        }
    } while (e != NULL);

    if (j != i)
    {
        printk(PRNPREFIX "Queue elements freed does not match queue count\n");
    }
    DBG_VERBOSE("Free queue: Contained %i, freed %i\n", j, i);

    atomic_set(&(q->count), 0);
}

/*************************************************************
 * NAME:
 *      QueueAppend
 *
 * DESCRIPTION:
 *      Append a new element to the tail of the queue.
 *
 * PARAMETERS:
 *      chan: BCSP channel id (see hc_com.h)
 *      data: Bluetooth data
 *      len: Length ofdata
 *      copy: Should we copy (true) data or consume it (false)
 *
 * RETURNS:
 *      void
 *
 *************************************************************/
void QueueAppend(csr_dev_t *dv, CsrUint8 chan, void *data,
    CsrUint16 len, CsrBool copy)
{
    struct usb_qe *new;
    usb_queue_t *q;
    unsigned long irq;

    /* Alloc new element and BCSP structure */
    new = CsrPmemAlloc(sizeof(struct usb_qe));
    new->next = NULL;

    new->msg = CsrPmemAlloc(sizeof(MessageStructure));
    new->msg->buflen = len;
    new->msg->chan = chan;

    /* Transfer / consume data */
    if (copy)
    {
        new->msg->buf = CsrPmemAlloc(len);
        memcpy(new->msg->buf, data, len);
    }
    else
    {
        new->msg->buf = data;
    }

    q = &(dv->queue);
    spin_lock_irqsave(&(q->lock), irq);

    /* Installing head */
    if (q->head == NULL)
    {
        q->head = new;
    }

    /* Snap into the list */
    if (q->tail != NULL)
    {
        q->tail->next = new;
    }

    /* Always replace the tail */
    q->tail = new;
    atomic_inc(&(q->count));
    spin_unlock_irqrestore(&(q->lock), irq);

    DBG_VERBOSE("Appended element to queue, count is now %i\n",
        atomic_read(&(q->count)));
}

/*************************************************************
 * NAME:
 *      QueuePop
 *
 * DESCRIPTION:
 *       Pop queue head, ie. take off the first element
 *       The caller must free the returned element!
 *
 * RETURNS:
 *      -
 *
 *************************************************************/
struct usb_qe *QueuePop(csr_dev_t *dv)
{
    struct usb_qe *res;
    usb_queue_t *q;
    unsigned long irq;
    int i;

    res = NULL;
    q = &(dv->queue);
    i = atomic_read(&(q->count));

    DBG_VERBOSE("Popping element from queue (%i elements)\n", i);

    /* Bail out now if queue is empty */
    if (i == 0)
    {
        return NULL;
    }

    spin_lock_irqsave(&(q->lock), irq);

    /* Get head */
    if (q->head != NULL)
    {
        /* Chop off the head, and set to next in list */
        res = q->head;
        q->head = res->next;

        /* If head is also tail, update */
        if (res == q->tail)
        {
            q->tail = NULL;
        }
    }

    atomic_dec(&(q->count));
    spin_unlock_irqrestore(&(q->lock), irq);

    if (res != NULL)
    {
        DBG_VERBOSE("Popped element (chan=%d size=%d) from queue\n",
            res->chan, res->msg->buflen);
    }

    return res;
}
