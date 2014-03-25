/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_log_text.h"

#include "csr_data_store_handler.h"
#include "csr_data_store_prim.h"
#include "csr_data_store_lib.h"
#include "csr_data_store_utils.h"

#define HEX_CHAR_OFFSET(x)  ((x) < 10 ? 0x30 : 0x37)

CsrUtf8String *CsrDataStoreKeyToFileName(CsrUint16 keyLength, CsrUint8 *key)
{
    CsrUtf8String *fileName;
    CsrUint16 count;
    CsrUint16 index;

    fileName = CsrPmemAlloc((keyLength << 1) + 5);
    for (index = count = 0; count < keyLength; count++)
    {
        CsrUint8 hexValue;

        hexValue = key[count] >> 4;
        fileName[index++] = ((CsrUtf8String) (hexValue + HEX_CHAR_OFFSET(hexValue)));
        hexValue = key[count] & 0x0F;
        fileName[index++] = ((CsrUtf8String) (hexValue + HEX_CHAR_OFFSET(hexValue)));
    }
    fileName[index++] = '.';
    fileName[index++] = 'c';
    fileName[index++] = 'd';
    fileName[index++] = 's';
    fileName[index] = '\0';

    return fileName;
}

void CsrDataStoreSaveMessage(CsrDataStoreInstanceData *inst)
{
    CsrMessageQueuePush(&inst->saveQueue, CSR_DATA_STORE_PRIM, inst->msg);
    inst->msg = NULL;
}

void CsrDataStoreRestoreSavedMessages(CsrDataStoreInstanceData *inst)
{
    inst->state = CSR_DATA_STORE_STATE_READY;
    inst->handleInProgess = NULL; /* No need to store the handleInProgress anymore */
    if (inst->saveQueue) /* A downstream messages is received during the just finished operation
         of the DATA_STORE task. Make sure that the messages
         is restored                                                        */
    {
        inst->restoreFlag = TRUE;
        CsrSchedMessagePut(CSR_DATA_STORE_IFACEQUEUE, CSR_DATA_STORE_PRIM, NULL);
    }
    else /* No downstream messages is received during the just finished operation
         of the task. Ignore the message                       */
    {
    }
}

CsrDataStoreHandles *CsrDataStoreHandleInit(CsrDataStoreHandles **handles, CsrSchedQid queueId, CsrUtf8String *storeName)
{
    CsrDataStoreHandles *prev, *next = *handles;
    prev = *handles;
    while (next)
    {
        prev = next;
        if (prev->queueId == queueId)
        {
            if (CsrUtf8StrCmp(prev->storeName, storeName) == 0)
            {
                CSR_LOG_TEXT_WARNING((CsrDataStoreLto, 0,
                                      "StoreName is already in the list!!! This should never happen the app has violated the API"));
                return prev;
            }
        }
        next = next->next;
    }

    /* storeName was not found in the list (and nor should it be!!!) initialize a new entry */
    next = CsrPmemZalloc(sizeof(CsrDataStoreHandles));
    next->queueId = queueId;
    next->storeName = storeName;

    if (prev == NULL) /* *handles was NULL so this is the first entry */
    {
        *handles = next;
    }
    else /* append it to the list */
    {
        prev->next = next;
    }
    return next;
}

void CsrDataStoreHandleRemove(CsrDataStoreHandles **handles, CsrDataStoreHandles *handle)
{
    CsrDataStoreHandles *prev, *next = *handles;

    prev = *handles;

    while (next)
    {
        if (next == handle)
        {
            if (next == *handles)
            {
                *handles = next->next;
            }
            else
            {
                prev->next = next->next;
            }

            CsrPmemFree(next->key);
            CsrPmemFree(next->record);
            CsrPmemFree(next->storeName);
            CsrPmemFree(next);
            return;
        }
        prev = next;
        next = prev->next;
    }

    CSR_LOG_TEXT_WARNING((CsrDataStoreLto, 0,
                          "Handle was not found in the list!!! This should never happen"));
}

CsrBool CsrDataStoreValidateStoreHandle(CsrDataStoreInstanceData *inst, CsrDataStoreHandle handle)
{
    CsrDataStoreHandles *next = inst->handles;
    CsrBool found = FALSE;

    while (next)
    {
        if (next == handle)
        {
            inst->handleInProgess = handle;
            found = TRUE;
            break;
        }
        next = next->next;
    }
    return found;
}
