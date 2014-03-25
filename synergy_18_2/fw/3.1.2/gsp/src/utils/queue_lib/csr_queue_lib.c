/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_pmem.h"
#include "csr_types.h"
#include "csr_queue_lib.h"

/****************************************************************************
NAME
        CsrQueueMsgAdd  -  add a message to the end of a queue

FUNCTION
        Adds the message "m" to the end of the message queue "q".
*/

void CsrQueueMsgAdd(TXMSG **q, TXMSG *m)
{
    TXMSG *pm;

    if (*q == (TXMSG *) (NULL))
    {
        *q = m;
    }
    else
    {
        for (pm = *q; pm->next != (TXMSG *) (NULL); pm = pm->next)
        {
        }
        pm->next = m;
    }
}

/****************************************************************************
NAME
        CsrQueueMsgPop  -  take a message from the base of a queue

RETURNS
        The message from the base of the message queue "q", or NULL if
        the queue is empty.
*/


TXMSG *CsrQueueMsgPop(TXMSG **q)
{
    TXMSG *ret;

    if (*q == (TXMSG *) (NULL))
    {
        return (TXMSG *) (NULL);
    }

    ret = *q;
    *q = ret->next;

    return ret;
}

/************************************************************************
 *
 *    FUNCTION:
 *        Generate Message queue structure
 *         This should be used for storing the queues in sequences
 *
 *
 *    RETURN:
 *        0 = fail to place message onto queue
 *        1 = success
 */

CsrUint8 CsrQueueMake(TXMSG **theQueue, void *msg, CsrUint8 chan, CsrUint8 rel)
{
    TXMSG *m;

    if ((m = (TXMSG *) CsrPmemZalloc(sizeof(TXMSG))) == (TXMSG *) (NULL))
    {
        return 0;
    }
    CsrMemCpy(&m->m, msg, sizeof(m->m));
    m->chan = chan;
    m->seq = rel;       /* Not used*/
    CsrQueueMsgAdd(theQueue, m); /* put message onto end of the queue */
    return 1;
}

/************************************************************************
 *
 *    FUNCTION:
 *        Take a message from the queue,
 *           and remove the message from the queue
 *
 *    RETURN.
 *        If message is available return pointer to message
 *         ELSE
 *        return NULL pointer.
 */

TXMSG *CsrQueueMsgTake(TXMSG **QStart)
{
    return CsrQueueMsgPop(QStart);
}

/****************************************************************************
 *
 *FUNCTION
 *        Free the message holding structure and returns the orginale
 *          message, chan, rel values.
 */
void CsrQueueDestroy(TXMSG **q, void **msg, CsrUint8 *chan, CsrUint8 *rel)
{
    TXMSG *m;
    m = (TXMSG *) q;

    *msg = NULL;              /*restore the orginal message*/
    *chan = m->chan;          /*restore the channel*/

    CsrPmemFree(m);                /*CsrPmemFree the message holding structure */
}

/************************************************************************
 *
 *    FUNCTION:
 *        Put the recieved structure onto the queue
 *
 *    RETURN.
 *        NON
 */

void CsrQueueMsgStore(TXMSG **theQueue, TXMSG *m)
{
    m->next = NULL;
    CsrQueueMsgAdd(theQueue, m); /* put message onto end of the queue */
}

/****************************************************************************
 *
 * RETURNS
 *        The number of messages stored in the queue "q".
 *
 */

CsrUint16 CsrQueueLength(TXMSG *q)
{
    CsrUint16 i;

    for (i = 0; q != (TXMSG *) (NULL); q = q->next, i++)
    {
    }
    return i;
}
