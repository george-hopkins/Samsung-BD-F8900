#ifndef CSR_QUEUE_LIB_H__
#define CSR_QUEUE_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_transport.h"

#ifdef __cplusplus
extern "C" {
#endif


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
 ***************************************************************************/
CsrUint8 CsrQueueMake(TXMSG **theQueue,
    void *msg,
    CsrUint8 chan,
    CsrUint8 rel);


/***************************************************************************
 *
 *    FUNCTION:
 *        Take a message from the queue,
 *           and remove the message from the queue
 *
 *    RETURN.
 *        If message is available return pointer to message
 *         ELSE
 *        return NULL pointer.
 *****************************************************************************/
TXMSG *CsrQueueMsgTake(TXMSG **QStart);


/****************************************************************************
 *
 *FUNCTION
 *        Free the message holding structure and returns the orginale
 *          message, chan, rel values.
 *****************************************************************************/
void CsrQueueDestroy(TXMSG **q,
    void **msg,
    CsrUint8 *chan,
    CsrUint8 *rel);


/****************************************************************************
 *
 *    FUNCTION:
 *        Put the received structure onto the ABCSP queue
 *
 *    RETURN.
 *        NON
 *****************************************************************************/
void CsrQueueMsgStore(TXMSG **theQueue,
    TXMSG *m);


/****************************************************************************
 *
 * RETURNS
 *        The number of messages stored in the queue "q".
 *
 *****************************************************************************/
CsrUint16 CsrQueueLength(TXMSG
    *q);

#ifdef __cplusplus
}
#endif

#endif
