/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_message_queue.h"
#include "csr_log_text_2.h"

/* DSPM Headers */
#include "csr_dspm_task.h"
#include "csr_dspm_prim.h"
#include "csr_dspm_lib.h"
#include "csr_dspm_handler.h"
#include "csr_dspm_sef.h"
#include "csr_dspm_util.h"

/* Allocate and initialise tuple and store it in the given list */
void CsrDspmInfoTupleInsert(CsrDspmInfoTuple **list, CsrUint16 key, CsrUint16 value)
{
    CsrDspmInfoTuple *tuple = CsrPmemAlloc(sizeof(CsrDspmInfoTuple));
    tuple->key = key;
    tuple->value = value;
    tuple->next = *list;
    *list = tuple;
}

/* Look up the value corresponding to a specified key in the given list */
CsrResult CsrDspmInfoTupleLookup(CsrDspmInfoTuple **list, CsrUint16 key, CsrUint16 *value)
{
    CsrDspmInfoTuple *tuple = *list;
    while (tuple != NULL)
    {
        if (tuple->key == key)
        {
            *value = tuple->value;
            return CSR_RESULT_SUCCESS;
        }
        tuple = tuple->next;
    }
    return CSR_RESULT_FAILURE;
}

/* Remove (and deallocate) the tuple corresponding to a specified key in the given list */
CsrResult CsrDspmInfoTupleRemove(CsrDspmInfoTuple **list, CsrUint16 key)
{
    CsrDspmInfoTuple *tuple = *list;
    if ((tuple != NULL) && (tuple->key == key))
    {
        *list = tuple->next;
        CsrPmemFree(tuple);
        return CSR_RESULT_SUCCESS;
    }
    else
    {
        while (tuple != NULL)
        {
            if ((tuple->next != NULL) && (tuple->next->key == key))
            {
                CsrDspmInfoTuple *tupleNext = tuple->next;
                tuple->next = tupleNext->next;
                CsrPmemFree(tupleNext);
                return CSR_RESULT_SUCCESS;
            }
            tuple = tuple->next;
        }
    }
    return CSR_RESULT_FAILURE;
}

void CsrDspmInfoTupleRemoveAll(CsrDspmInfoTuple **list)
{
    while (*list != NULL)
    {
        CsrDspmInfoTuple *tuple = *list;
        *list = tuple->next;
        CsrPmemFree(tuple);
    }
}

/* Start handling DSPM requests on the queue, if any */
void CsrDspmRequestQueueHandle(CsrDspmInstanceData *instanceData)
{
    while (instanceData->currentRequest != NULL &&
           CsrDspmRequestHandler(instanceData))
    {
        CsrDspmRequestQueuePop(instanceData);
    }
}

/* Push a DSPM request onto the request queue and start handling it, if possible */
void CsrDspmRequestQueuePushHandle(CsrDspmInstanceData *instanceData, void *message)
{
    if (instanceData->currentRequest != NULL)
    {
        CsrMessageQueuePush(&instanceData->saveQueue, 0, message);
    }
    else
    {
        instanceData->currentRequest = message;
        instanceData->currentRequestIndex = 0;
        if (instanceData->state == CSR_DSPM_STATE_ACTIVE)
        {
            CsrDspmRequestQueueHandle(instanceData);
        }
    }
}

/* Pop a DSPM request from the request queue and returns TRUE if additional requests are on the queue */
void CsrDspmRequestQueuePop(CsrDspmInstanceData *instanceData)
{
    if (instanceData->currentRequest != NULL)
    {
        CsrUint16 event;
        CsrDspmFreeDownstreamMessageContents(CSR_DSPM_PRIM, instanceData->currentRequest);
        CsrPmemFree(instanceData->currentRequest);
        instanceData->currentRequest = NULL;
        (void) CsrMessageQueuePop(&instanceData->saveQueue, &event, &instanceData->currentRequest);
        instanceData->currentRequestIndex = 0;
    }
}

/* Pop a DSPM request from the request queue and start handling the next request on the queue, if any */
void CsrDspmRequestQueuePopHandle(CsrDspmInstanceData *instanceData)
{
    CsrDspmRequestQueuePop(instanceData);
    CsrDspmRequestQueueHandle(instanceData);
}
