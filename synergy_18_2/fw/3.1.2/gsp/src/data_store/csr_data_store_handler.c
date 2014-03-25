/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_message_queue.h"
#include "csr_log_text.h"

#include "csr_data_store_prim.h"
#include "csr_data_store_task.h"
#include "csr_data_store_sef.h"
#include "csr_data_store_lib.h"
#include "csr_data_store_handler.h"

#include "csr_fsal_lib.h"
#include "csr_data_store_utils.h"
#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrDataStoreLto);

static const CsrDataStoreEventType csrDataStoreMsgHandlers[CSR_DATA_STORE_NUM_STATES][CSR_DATA_STORE_PRIM_DOWNSTREAM_COUNT] =
{
    { /* Jump table to handle downstream CSR_DATA_STORE messages sent from the upper layers */
        CsrDataStoreSaveMessage,                    /* CSR_DATA_STORE_CREATE_REQ            */
        CsrDataStoreSaveMessage,                    /* CSR_DATA_STORE_OPEN_REQ              */
        CsrDataStoreSaveMessage,                    /* CSR_DATA_STORE_CLOSE_REQ             */
        CsrDataStoreSaveMessage,                    /* CSR_DATA_STORE_DELETE_REQ            */
        CsrDataStoreSaveMessage,                    /* CSR_DATA_STORE_RECORD_READ_REQ       */
        CsrDataStoreSaveMessage,                    /* CSR_DATA_STORE_RECORD_WRITE_REQ      */
        CsrDataStoreSaveMessage,                    /* CSR_DATA_STORE_RECORD_DELETE_REQ     */
    },
    { /* Jump table to handle downstream CSR_DATA_STORE messages sent from the upper layers */
        CsrDataStoreCreateReqHandler,               /* CSR_DATA_STORE_CREATE_REQ            */
        CsrDataStoreOpenReqHandler,                 /* CSR_DATA_STORE_OPEN_REQ              */
        CsrDataStoreCloseReqHandler,                /* CSR_DATA_STORE_CLOSE_REQ             */
        CsrDataStoreDeleteReqHandler,               /* CSR_DATA_STORE_DELETE_REQ            */
        CsrDataStoreRecordReadReqHandler,           /* CSR_DATA_STORE_RECORD_READ_REQ       */
        CsrDataStoreRecordWriteReqHandler,          /* CSR_DATA_STORE_RECORD_WRITE_REQ      */
        CsrDataStoreRecordDeleteReqHandler,         /* CSR_DATA_STORE_RECORD_DELETE_REQ     */
    }
};

static const CsrDataStoreEventType csrDataStoreFsalMsgHandlers[CSR_FSAL_PRIM_UPSTREAM_COUNT] =
{
    CsrDataStoreFsalSessionCreateCfmHandler,        /* CSR_FSAL_SESSION_CREATE_CFM          */
    NULL,                                           /* CSR_FSAL_SESSION_DESTROY_CFM         */
    CsrDataStoreFsalFileOpenCfmHandler,             /* CSR_FSAL_FILE_OPEN_CFM               */
    CsrDataStoreFsalFileCloseCfmHandler,            /* CSR_FSAL_FILE_CLOSE_CFM              */
    NULL,                                           /* CSR_FSAL_FILE_SEEK_CFM               */
    CsrDataStoreFsalFileReadCfmHandler,             /* CSR_FSAL_FILE_READ_CFM               */
    CsrDataStoreFsalFileWriteCfmHandler,            /* CSR_FSAL_FILE_WRITE_CFM              */
    NULL,                                           /* CSR_FSAL_FILE_TELL_CFM               */
    NULL,                                           /* CSR_FSAL_STAT_CFM                    */
    CsrDataStoreFsalRemoveCfmHandler,               /* CSR_FSAL_REMOVE_CFM                  */
    NULL,                                           /* CSR_FSAL_RENAME_CFM                  */
    NULL,                                           /* CSR_FSAL_PERMISSIONS_SET_CFM         */
    CsrDataStoreFsalDirMakeCfmHandler,              /* CSR_FSAL_DIR_MAKE_CFM                */
    NULL,                                           /* CSR_FSAL_DIR_CHANGE_CFM              */
    NULL,                                           /* CSR_FSAL_DIR_FIND_FIRST_CFM          */
    NULL,                                           /* CSR_FSAL_DIR_FIND_NEXT_CFM           */
    NULL,                                           /* CSR_FSAL_DIR_FIND_CLOSE_CFM          */
    CsrDataStoreFsalRemoveRecursivelyCfmHandler     /* CSR_FSAL_REMOVE_RECURSIVELY_CFM      */
};

void CsrDataStoreInit(void **gash)
{
    CsrDataStoreInstanceData *instanceData;
    instanceData = (CsrDataStoreInstanceData *) CsrPmemZalloc(sizeof(CsrDataStoreInstanceData));
    *gash = (void *) instanceData;

    instanceData->state = CSR_DATA_STORE_STATE_BUSY;
    instanceData->initialized = FALSE;

    CSR_LOG_TEXT_REGISTER(&CsrDataStoreLto, "DATA_STORE", 0, NULL);

    CsrFsalSessionCreateReqSend(CSR_DATA_STORE_IFACEQUEUE);
}

void CsrDataStoreHandler(void **gash)
{
    CsrDataStoreInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrDataStoreInstanceData *) *gash;

    /* Obtains a message from the CSR_HCI_IFACEQUEUE                                         */
    if (!instanceData->restoreFlag) /* No messages on the local save queue                                                */
    {
        CsrSchedMessageGet(&event, &message);
    }
    else
    {
        if (!CsrMessageQueuePop(&instanceData->saveQueue, &event, &message)) /* No more messages on the local save queue. Call CsrSchedMessageGet to receive
             a message from the scheduler                                                   */
        {
            instanceData->restoreFlag = FALSE;
            CsrSchedMessageGet(&event, &message);
        }
        else /* A message has been restored from the local queed                               */
        {
        }
    }

    instanceData->msg = message;

    switch (event)
    {
        case CSR_DATA_STORE_PRIM:
        {
            if (message != NULL)
            {
                CsrUint16 *type = (CsrUint16 *) message;
                if ((*type < CSR_DATA_STORE_PRIM_DOWNSTREAM_COUNT) && ((csrDataStoreMsgHandlers[instanceData->state][*type]) != NULL))
                {
                    csrDataStoreMsgHandlers[instanceData->state][*type](instanceData);

                    if (instanceData->msg)
                    {
                        CsrDataStoreFreeDownstreamMessageContents(event, message);
                    }
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDataStoreLto, 0, event, *type);
                }
            }
            break;
        }
        case CSR_FSAL_PRIM:
        {
            CsrUint16 *type = (CsrUint16 *) message;

            if (((*type - CSR_FSAL_PRIM_UPSTREAM_LOWEST) < CSR_FSAL_PRIM_UPSTREAM_COUNT) && ((csrDataStoreFsalMsgHandlers[*type - CSR_FSAL_PRIM_UPSTREAM_LOWEST]) != NULL))
            {
                csrDataStoreFsalMsgHandlers[*type - CSR_FSAL_PRIM_UPSTREAM_LOWEST](instanceData);

                CsrFsalFreeUpstreamMessageContents(event, message);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDataStoreLto, 0, event, *type);
            }
            break;
        }
        default:
            break;
    }

    CsrPmemFree(instanceData->msg);
    instanceData->msg = NULL;
}

#ifdef ENABLE_SHUTDOWN
void CsrDataStoreDeinit(void **gash)
{
    CsrUint16 event;
    void *message;
    CsrDataStoreInstanceData *instanceData;
    CsrBool lastMsg = FALSE;

    instanceData = (CsrDataStoreInstanceData *) *gash;

    /* Clean queue */
    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&instanceData->saveQueue, &event, &message))
        {
            lastMsg = (CsrBool) (!CsrSchedMessageGet(&event, &message));
        }
        if (!lastMsg && (message != NULL))
        {
            switch (event)
            {
                case CSR_DATA_STORE_PRIM:
                {
                    CsrDataStoreFreeDownstreamMessageContents(event, message);
                    break;
                }
                case CSR_FSAL_PRIM:
                {
                    CsrFsalFreeUpstreamMessageContents(event, message);
                    break;
                }
            }
            CsrPmemFree(message);
        }
    }

    while (instanceData->handles)
    {
        CsrDataStoreHandleRemove(&instanceData->handles, instanceData->handles);
    }

    CsrPmemFree(instanceData->currentWorkingDir);
    CsrPmemFree(instanceData->storagePrefix);

    /* Deallocate instance data */
    CsrPmemFree(instanceData);
}

#endif
