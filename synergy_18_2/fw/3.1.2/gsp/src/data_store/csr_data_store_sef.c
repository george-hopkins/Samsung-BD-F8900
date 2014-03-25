/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"

#include "csr_log_text_2.h"

#include "csr_data_store_handler.h"
#include "csr_data_store_prim.h"
#include "csr_data_store_lib.h"
#include "csr_data_store_utils.h"

#include "csr_fsal_lib.h"
#include "csr_usr_config.h"
#include "csr_panic.h"

void CsrDataStoreCreateReqHandler(CsrDataStoreInstanceData *inst)
{
    CsrDataStoreCreateReq *prim = inst->msg;
    CsrUtf8String *dataStoreDir = CsrUtf8StringConcatenateTexts(inst->storagePrefix, prim->storeName, NULL, NULL);

    inst->state = CSR_DATA_STORE_STATE_BUSY;

    inst->handleInProgess = CsrDataStoreHandleInit(&inst->handles, prim->queueId, CsrUtf8StrDup(prim->storeName));

    inst->handleInProgess->currentOperation = CSR_DATA_STORE_OPERATION_CREATE;
    inst->handleInProgess->keyLength = prim->keyLength;
    inst->handleInProgess->recordLength = prim->recordLength;

    CsrFsalDirMakeReqSend(inst->sessionId, dataStoreDir)
}

void CsrDataStoreOpenReqHandler(CsrDataStoreInstanceData *inst)
{
    CsrDataStoreOpenReq *prim = inst->msg;
    CsrUtf8String *dataStoreDefaultEntry =
        CsrUtf8StringConcatenateTexts(inst->storagePrefix, prim->storeName, (CsrUtf8String *) "/", (CsrUtf8String *) CSR_DATA_STORE_DEFAULT_ENTRY_NAME);

    inst->state = CSR_DATA_STORE_STATE_BUSY;

    inst->handleInProgess = CsrDataStoreHandleInit(&inst->handles, prim->queueId, prim->storeName);
    prim->storeName = NULL; /* Make sure not to free the storeName */

    inst->handleInProgess->currentOperation = CSR_DATA_STORE_OPERATION_OPEN;

    CsrFsalFileOpenReqSend(inst->sessionId, dataStoreDefaultEntry,
        CSR_FSAL_OPEN_FLAGS_READ_ONLY,
        CSR_FSAL_OPEN_PERMS_NOT_APPLICABLE);
}

void CsrDataStoreCloseReqHandler(CsrDataStoreInstanceData *inst)
{
    CsrDataStoreCloseReq *prim = inst->msg;

    if (CsrDataStoreValidateStoreHandle(inst, prim->storeHandle))
    {
        /* Clean up the created handles */
        CsrDataStoreHandleRemove(&inst->handles, prim->storeHandle);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrDataStoreLto, 0,
                              "Handle was not validated in the list!!! This should never happen"));
    }

    CsrDataStoreCloseCfmSend(prim->queueId, prim->storeHandle);
}

void CsrDataStoreDeleteReqHandler(CsrDataStoreInstanceData *inst)
{
    CsrDataStoreDeleteReq *prim = inst->msg;
    CsrUtf8String *dataStoreName =
        CsrUtf8StringConcatenateTexts(inst->storagePrefix, prim->storeName, NULL, NULL);

    inst->state = CSR_DATA_STORE_STATE_BUSY;

    inst->handleInProgess = CsrDataStoreHandleInit(&inst->handles, prim->queueId, prim->storeName);
    prim->storeName = NULL; /* Make sure not to free the storeName */

    CsrFsalRemoveRecursivelyReqSend(inst->sessionId, dataStoreName);
}

void CsrDataStoreRecordReadReqHandler(CsrDataStoreInstanceData *inst)
{
    CsrDataStoreRecordReadReq *prim = inst->msg;
    CsrUtf8String *keyName = CsrDataStoreKeyToFileName(prim->keyLength, prim->key);

    if (CsrDataStoreValidateStoreHandle(inst, prim->storeHandle))
    {
        CsrUtf8String *dataStoreKey = CsrUtf8StringConcatenateTexts(inst->storagePrefix, inst->handleInProgess->storeName, (CsrUtf8String *) "/", keyName);

        inst->state = CSR_DATA_STORE_STATE_BUSY;

        inst->handleInProgess->currentOperation = CSR_DATA_STORE_OPERATION_READ;
        inst->handleInProgess->keyLength = prim->keyLength;
        inst->handleInProgess->key = prim->key;

        CsrFsalFileOpenReqSend(inst->sessionId, dataStoreKey,
            CSR_FSAL_OPEN_FLAGS_READ_ONLY,
            CSR_FSAL_OPEN_PERMS_NOT_APPLICABLE);
    }
    else
    {
        CsrDataStoreRecordReadCfmSend(prim->queueId, prim->storeHandle, CSR_DATA_STORE_RECORD_READ_INVALID_HANDLE, prim->keyLength, prim->key, 0, NULL);
    }

    prim->key = NULL;
    CsrPmemFree(keyName);
}

void CsrDataStoreRecordWriteReqHandler(CsrDataStoreInstanceData *inst)
{
    CsrDataStoreRecordWriteReq *prim = inst->msg;
    CsrUtf8String *keyName = CsrDataStoreKeyToFileName(prim->keyLength, prim->key);

    if (CsrDataStoreValidateStoreHandle(inst, prim->storeHandle))
    {
        CsrUtf8String *dataStoreKey = CsrUtf8StringConcatenateTexts(inst->storagePrefix, inst->handleInProgess->storeName, (CsrUtf8String *) "/", keyName);

        inst->state = CSR_DATA_STORE_STATE_BUSY;

        inst->handleInProgess->currentOperation = CSR_DATA_STORE_OPERATION_WRITE;
        inst->handleInProgess->keyLength = prim->keyLength;
        inst->handleInProgess->key = prim->key;
        inst->handleInProgess->recordLength = prim->recordLength;
        inst->handleInProgess->record = prim->record;
        prim->record = NULL;

        CsrFsalFileOpenReqSend(inst->sessionId, dataStoreKey,
            (CSR_FSAL_OPEN_FLAGS_CREATE | CSR_FSAL_OPEN_FLAGS_WRITE_ONLY | CSR_FSAL_OPEN_FLAGS_TRUNCATE),
            (CSR_FSAL_OPEN_PERMS_READ | CSR_FSAL_OPEN_PERMS_WRITE));
    }
    else
    {
        CsrDataStoreRecordWriteCfmSend(prim->queueId, prim->storeHandle, CSR_DATA_STORE_RECORD_WRITE_INVALID_HANDLE, prim->keyLength, prim->key);
    }

    prim->key = NULL;
    CsrPmemFree(keyName);
}

void CsrDataStoreRecordDeleteReqHandler(CsrDataStoreInstanceData *inst)
{
    CsrDataStoreRecordDeleteReq *prim = inst->msg;
    CsrUtf8String *keyName = CsrDataStoreKeyToFileName(prim->keyLength, prim->key);

    if (CsrDataStoreValidateStoreHandle(inst, prim->storeHandle))
    {
        CsrUtf8String *dataStoreKey = CsrUtf8StringConcatenateTexts(inst->storagePrefix, inst->handleInProgess->storeName, (CsrUtf8String *) "/", keyName);

        inst->state = CSR_DATA_STORE_STATE_BUSY;

        inst->handleInProgess->currentOperation = CSR_DATA_STORE_OPERATION_DELETE;
        inst->handleInProgess->keyLength = prim->keyLength;
        inst->handleInProgess->key = prim->key;

        CsrFsalRemoveReqSend(inst->sessionId, dataStoreKey);
    }
    else
    {
        CsrDataStoreRecordDeleteCfmSend(prim->queueId, prim->storeHandle, CSR_DATA_STORE_RECORD_DELETEE_INVALID_HANDLE, prim->keyLength, prim->key);
    }

    prim->key = NULL;
    CsrPmemFree(keyName);
}

void CsrDataStoreFsalSessionCreateCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalSessionCreateCfm *cfm = inst->msg;

    if (cfm->result == CSR_RESULT_SUCCESS)
    {
        inst->currentWorkingDir = cfm->currentWorkingDir; /* Avoid copying */
        cfm->currentWorkingDir = NULL; /* Make sure not to free */
        inst->sessionId = cfm->sessionId;

        inst->storagePrefix = (CsrUtf8String *) CsrStrDup(CSR_DATA_STORE_ROOT_DIR);

        CsrFsalDirMakeReqSend(inst->sessionId, CsrUtf8StrDup(inst->storagePrefix));
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrDataStoreLto, 0,
                              "Failed to create a new session"));
    }
}

void CsrDataStoreFsalFileOpenCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalFileOpenCfm *cfm = inst->msg;

    if (cfm->result == CSR_RESULT_SUCCESS)
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_CREATE)
        {
            CsrUint8 *defaultEntry = CsrPmemAlloc(6);

            CSR_COPY_UINT16_TO_BIG_ENDIAN(inst->handleInProgess->keyLength, defaultEntry);
            CSR_COPY_UINT32_TO_BIG_ENDIAN(inst->handleInProgess->recordLength, &defaultEntry[2]);

            CsrFsalFileWriteReqSend(cfm->handle, 6, defaultEntry);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_OPEN) /* A data store is being opened read out the keyLength and recordLength from default entry */
        {
            CsrFsalFileReadReqSend(cfm->handle, 6);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_READ) /* A key in a data store has just been opened for reading now read out recordLength of data from the key file */
        {
            CsrFsalFileReadReqSend(cfm->handle, inst->handleInProgess->recordLength);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_WRITE) /* A key in a data store has just been opened for writing now write down the record to the key file */
        {
            CsrFsalFileWriteReqSend(cfm->handle, inst->handleInProgess->recordLength, inst->handleInProgess->record);
            inst->handleInProgess->record = NULL;
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileOpen succes but invalid currentOperation"));
        }
    }
    else /* Report error */
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_CREATE)
        {
            CsrDataStoreCreateCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_DATA_STORE_CREATE_NOT_ALLOWED, CSR_DATA_STORE_INVALID_HANDLE);

            /* Clean up the created handles */
            CsrDataStoreHandleRemove(&inst->handles, inst->handleInProgess);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_OPEN)
        {
            CsrDataStoreOpenCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_DATA_STORE_OPEN_NOT_EXIST, CSR_DATA_STORE_INVALID_HANDLE, 0, 0);

            /* Clean up the created handles */
            CsrDataStoreHandleRemove(&inst->handles, inst->handleInProgess);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_READ)
        {
            CsrDataStoreRecordReadCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_DATA_STORE_RECORD_READ_NOT_EXIST, inst->handleInProgess->keyLength, CsrMemDup(inst->handleInProgess->key, inst->handleInProgess->keyLength), 0, NULL);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_WRITE)
        {
            CsrDataStoreRecordWriteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_DATA_STORE_RECORD_WRITE_FAILURE, inst->handleInProgess->keyLength, CsrMemDup(inst->handleInProgess->key, inst->handleInProgess->keyLength));
            CsrPmemFree(inst->handleInProgess->record);
            inst->handleInProgess->record = NULL;
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileOpen failed and invalid currentOperation"));
        }

        /* Now restore the queue */
        CsrDataStoreRestoreSavedMessages(inst);
    }
}

void CsrDataStoreFsalFileCloseCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalFileCloseCfm *cfm = inst->msg;

    if (cfm->result == CSR_RESULT_SUCCESS)
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_CREATE)
        {
            CsrDataStoreCreateCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_RESULT_SUCCESS, inst->handleInProgess);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_OPEN)
        {
            CsrDataStoreOpenCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_RESULT_SUCCESS, inst->handleInProgess, inst->handleInProgess->keyLength, inst->handleInProgess->recordLength);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_READ)
        {
            CsrDataStoreRecordReadCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_RESULT_SUCCESS, inst->handleInProgess->keyLength, inst->handleInProgess->key, inst->handleInProgess->recordLength, inst->handleInProgess->record);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_WRITE) /* A key in a data store has just been opened for writing now write down the record to the key file */
        {
            CsrDataStoreRecordWriteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_RESULT_SUCCESS, inst->handleInProgess->keyLength, inst->handleInProgess->key);
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileOpen succes but invalid currentOperation"));
        }

        inst->handleInProgess->key = NULL; /* Responsibility of this pointer has now been given to the upper layer */
        inst->handleInProgess->record = NULL; /* Responsibility of this pointer has now been given to the upper layer */
    }
    else /* Report error */
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_CREATE)
        {
            CsrDataStoreCreateCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_DATA_STORE_CREATE_FAILURE, CSR_DATA_STORE_INVALID_HANDLE);

            /* Clean up the created handles */
            CsrDataStoreHandleRemove(&inst->handles, inst->handleInProgess);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_OPEN)
        {
            CsrDataStoreOpenCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_DATA_STORE_OPEN_FAILURE, CSR_DATA_STORE_INVALID_HANDLE, 0, 0);

            /* Clean up the created handles */
            CsrDataStoreHandleRemove(&inst->handles, inst->handleInProgess);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_READ)
        {
            CsrDataStoreRecordReadCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_DATA_STORE_RECORD_READ_FAILURE, inst->handleInProgess->keyLength, CsrMemDup(inst->handleInProgess->key, inst->handleInProgess->keyLength), 0, NULL);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_WRITE)
        {
            CsrDataStoreRecordWriteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_DATA_STORE_RECORD_WRITE_FAILURE, inst->handleInProgess->keyLength, CsrMemDup(inst->handleInProgess->key, inst->handleInProgess->keyLength));
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileOpen failed and invalid currentOperation"));
        }
    }

    /* Always restore the queue after a close regardless of the outcome */
    CsrDataStoreRestoreSavedMessages(inst);
}

void CsrDataStoreFsalFileReadCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalFileReadCfm *cfm = inst->msg;

    if (cfm->result == CSR_RESULT_SUCCESS)
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_OPEN) /* We've read out the data store default info about keyLength and recordLength now store it for later use */
        {
            inst->handleInProgess->keyLength = CSR_GET_UINT16_FROM_BIG_ENDIAN(cfm->data);
            inst->handleInProgess->recordLength = CSR_GET_UINT32_FROM_BIG_ENDIAN(&cfm->data[2]);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_READ) /* We've read out the record from the key file */
        {
            inst->handleInProgess->record = cfm->data;
            inst->handleInProgess->recordLength = cfm->dataLen;
            cfm->data = NULL;
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileRead succes but invalid currentOperation"));
        }

        CsrFsalFileCloseReqSend(cfm->handle);
    }
    else /* Report error */
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_OPEN)
        {
            CsrDataStoreOpenCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_DATA_STORE_OPEN_FAILURE, CSR_DATA_STORE_INVALID_HANDLE, 0, 0);

            /* Clean up the created handles */
            CsrDataStoreHandleRemove(&inst->handles, inst->handleInProgess);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_READ)
        {
            CsrDataStoreRecordReadCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_DATA_STORE_RECORD_READ_FAILURE, inst->handleInProgess->keyLength, CsrMemDup(inst->handleInProgess->key, inst->handleInProgess->keyLength), 0, NULL);
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileRead failed and invalid currentOperation"));
        }

        /* Now restore the queue */
        CsrDataStoreRestoreSavedMessages(inst);
    }
}

void CsrDataStoreFsalFileWriteCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalFileWriteCfm *cfm = inst->msg;

    if (cfm->result == CSR_RESULT_SUCCESS)
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_CREATE) /* We succeeded in writing down the default entry do nothing yet but to close the file */
        {
            if (6 != cfm->bytesWritten)
            {
                CSR_LOG_TEXT_WARNING((CsrDataStoreLto, 0,
                                      "FileWrite failed to write down all the default entry data"));
            }
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_WRITE) /* We succeeded in writing down the record data do nothing yet but to close the file */
        {
            if (inst->handleInProgess->recordLength != cfm->bytesWritten)
            {
                CSR_LOG_TEXT_WARNING((CsrDataStoreLto, 0,
                                      "FileWrite failed to write down all the record data"));
            }
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileOpen succes but invalid currentOperation"));
        }

        CsrFsalFileCloseReqSend(cfm->handle);
    }
    else /* Report error */
    {
        if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_CREATE)
        {
            CsrDataStoreCreateCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_DATA_STORE_CREATE_FAILURE, CSR_DATA_STORE_INVALID_HANDLE);

            /* Clean up the created handles */
            CsrDataStoreHandleRemove(&inst->handles, inst->handleInProgess);
        }
        else if (inst->handleInProgess->currentOperation == CSR_DATA_STORE_OPERATION_WRITE)
        {
            CsrDataStoreRecordWriteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_DATA_STORE_RECORD_WRITE_FAILURE, inst->handleInProgess->keyLength, CsrMemDup(inst->handleInProgess->key, inst->handleInProgess->keyLength));
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDataStoreLto, 0, "FileOpen failed and invalid currentOperation"));
        }

        /* Now restore the queue */
        CsrDataStoreRestoreSavedMessages(inst);
    }
}

void CsrDataStoreFsalDirMakeCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalDirMakeCfm *cfm = inst->msg;

    if ((cfm->result == CSR_RESULT_SUCCESS) || (cfm->result == CSR_FSAL_DIR_MAKE_EXIST))
    {
        if (inst->handleInProgess) /* A new database directory has been created now write down the keyLength and recordLength values */
        {
            CsrUtf8String *dataStoreDefaultEntry =
                CsrUtf8StringConcatenateTexts(inst->storagePrefix, inst->handleInProgess->storeName, (CsrUtf8String *) "/", (CsrUtf8String *) CSR_DATA_STORE_DEFAULT_ENTRY_NAME);

            CsrFsalFileOpenReqSend(inst->sessionId, dataStoreDefaultEntry,
                (CSR_FSAL_OPEN_FLAGS_CREATE | CSR_FSAL_OPEN_FLAGS_WRITE_ONLY | CSR_FSAL_OPEN_FLAGS_TRUNCATE),
                (CSR_FSAL_OPEN_PERMS_READ | CSR_FSAL_OPEN_PERMS_WRITE));
        }
        else /* This only happens during initalization where we create the directory under which all data stores will be created */
        {
            CsrDataStoreRestoreSavedMessages(inst);
        }
    }
    else
    {
        if (inst->handleInProgess) /* A new database directory was not created correctly report error to the relevant task */
        {
            CsrDataStoreCreateCfmSend(inst->handleInProgess->queueId, CsrUtf8StrDup(inst->handleInProgess->storeName), CSR_DATA_STORE_CREATE_NOT_ALLOWED, CSR_DATA_STORE_INVALID_HANDLE);
        }
        else /* Critical error the CSR_DATA_STORE task could not create its default directory used for all data stores */
        {
            CSR_LOG_TEXT_WARNING((CsrDataStoreLto, 0,
                                  "Failed to create a new directory"));
        }
    }
}

void CsrDataStoreFsalRemoveCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalRemoveCfm *cfm = inst->msg;

    if (cfm->result == CSR_RESULT_SUCCESS)
    {
        CsrDataStoreRecordDeleteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_RESULT_SUCCESS, inst->handleInProgess->keyLength, inst->handleInProgess->key);
    }
    else
    {
        CsrDataStoreRecordDeleteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess, CSR_DATA_STORE_RECORD_DELETE_FAILURE, inst->handleInProgess->keyLength, inst->handleInProgess->key);
    }

    /* Always restore the queue after a remove regardless of the outcome */
    inst->handleInProgess->key = NULL; /* Responsibility of this pointer has now been given to the upper layer */
    CsrDataStoreRestoreSavedMessages(inst);
}

void CsrDataStoreFsalRemoveRecursivelyCfmHandler(CsrDataStoreInstanceData *inst)
{
    CsrFsalRemoveRecursivelyCfm *cfm = inst->msg;

    if (cfm->result == CSR_RESULT_SUCCESS)
    {
        CsrDataStoreDeleteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess->storeName, CSR_RESULT_SUCCESS);
    }
    else
    {
        CsrDataStoreDeleteCfmSend(inst->handleInProgess->queueId, inst->handleInProgess->storeName, CSR_DATA_STORE_DELETE_FAILURE);
    }

    /* Clean up the created handles */
    inst->handleInProgess->storeName = NULL;
    CsrDataStoreHandleRemove(&inst->handles, inst->handleInProgess);

    /* Always restore the queue after a remove regardless of the outcome */
    CsrDataStoreRestoreSavedMessages(inst);
}
