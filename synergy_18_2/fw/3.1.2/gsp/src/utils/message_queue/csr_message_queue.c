/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_message_queue.h"
#include "csr_pmem.h"

#include "csr_log_gsp.h"

void CsrMessageQueuePush(CsrMessageQueueType **messageQueue, CsrUint16 event, void *message)
{
    CsrMessageQueueType *currentMessageQueue;
    CSR_LOG_MESSAGE_QUEUE_PUSH(event, message);

    if (*messageQueue == NULL)
    {
        *messageQueue = (CsrMessageQueueType *) CsrPmemAlloc(sizeof(CsrMessageQueueType));
        currentMessageQueue = *messageQueue;
        currentMessageQueue->numberOfMessagesInThisPool = NUMBER_OF_MESSAGES_IN_ONE_POOL;
        currentMessageQueue->nextEntryNumber = 0;
        currentMessageQueue->nextQueuePool = NULL;
    }
    else
    {
        if (((*messageQueue)->nextQueuePool == NULL) && ((*messageQueue)->nextEntryNumber < (*messageQueue)->numberOfMessagesInThisPool))
        {
            currentMessageQueue = *messageQueue;
        }
        else
        {
            CsrMessageQueueType *previousQueue;

            for (previousQueue = *messageQueue, currentMessageQueue = (*messageQueue)->nextQueuePool
                 ; (currentMessageQueue != NULL) && (currentMessageQueue->nextEntryNumber == currentMessageQueue->numberOfMessagesInThisPool)
                 ; currentMessageQueue = currentMessageQueue->nextQueuePool)
            {
                previousQueue = currentMessageQueue;
            }
            if (currentMessageQueue == NULL)
            {
                previousQueue->nextQueuePool = (CsrMessageQueueType *) CsrPmemAlloc(sizeof(CsrMessageQueueType));
                currentMessageQueue = previousQueue->nextQueuePool;
                currentMessageQueue->numberOfMessagesInThisPool = NUMBER_OF_MESSAGES_IN_ONE_POOL;
                currentMessageQueue->nextEntryNumber = 0;
                currentMessageQueue->nextQueuePool = NULL;
            }
        }
    }

    currentMessageQueue->messageQueueEntryArray[currentMessageQueue->nextEntryNumber].event = event;
    currentMessageQueue->messageQueueEntryArray[currentMessageQueue->nextEntryNumber++].message = message;
}

CsrUint16 CsrMessageQueueRestore(CsrMessageQueueType **messageQueue, CsrSchedQid queue)
{
    CsrUint16 event;
    void *message;
    CsrUint16 numberOfRestoredMessages = 0;

    if (*messageQueue != NULL)
    {
        while (CsrMessageQueuePop(messageQueue, &event, &message))
        {
            CsrSchedMessagePut(queue, event, message);
            numberOfRestoredMessages++;
        }
    }

    return numberOfRestoredMessages;
}

CsrBool CsrMessageQueuePop(CsrMessageQueueType **messageQueue, CsrUint16 *event, void **message)
{
    CsrUint16 index;

    if (*messageQueue != NULL)
    {
        CsrMessageQueueType *currentMessageQueue;

        currentMessageQueue = *messageQueue;

        CSR_LOG_MESSAGE_QUEUE_POP(currentMessageQueue->messageQueueEntryArray[0].event,
            currentMessageQueue->messageQueueEntryArray[0].message);

        *event = currentMessageQueue->messageQueueEntryArray[0].event;
        *message = currentMessageQueue->messageQueueEntryArray[0].message;
        currentMessageQueue->nextEntryNumber--;
        if (currentMessageQueue->nextEntryNumber == 0)
        {
            *messageQueue = (*messageQueue)->nextQueuePool;
            CsrPmemFree(currentMessageQueue);
        }
        else
        {
            for (index = 0; index < currentMessageQueue->nextEntryNumber; index++)
            {
                currentMessageQueue->messageQueueEntryArray[index] = currentMessageQueue->messageQueueEntryArray[index + 1];
            }
        }

        return TRUE;
    }

    return FALSE;
}
