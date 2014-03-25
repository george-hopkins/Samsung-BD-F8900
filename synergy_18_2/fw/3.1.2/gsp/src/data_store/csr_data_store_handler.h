#ifndef CSR_DATA_STORE_HANDLER_H__
#define CSR_DATA_STORE_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_message_queue.h"
#include "csr_data_store_prim.h"
#include "csr_fsal_prim.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrDataStoreLto);

typedef CsrUint8 CsrDataStoreState;
#define CSR_DATA_STORE_STATE_BUSY      ((CsrDataStoreState) 0)
#define CSR_DATA_STORE_STATE_READY     ((CsrDataStoreState) 1)
#define CSR_DATA_STORE_NUM_STATES       2

typedef CsrUint8 CsrDataStoreOperation;
#define CSR_DATA_STORE_OPERATION_INVALID    ((CsrDataStoreOperation) 0)
#define CSR_DATA_STORE_OPERATION_CREATE     ((CsrDataStoreOperation) 1)
#define CSR_DATA_STORE_OPERATION_OPEN       ((CsrDataStoreOperation) 2)
#define CSR_DATA_STORE_OPERATION_READ       ((CsrDataStoreOperation) 3)
#define CSR_DATA_STORE_OPERATION_WRITE      ((CsrDataStoreOperation) 4)
#define CSR_DATA_STORE_OPERATION_DELETE     ((CsrDataStoreOperation) 5)
#define CSR_DATA_STORE_OPERATION_DELETE_ALL ((CsrDataStoreOperation) 6)


typedef struct CsrDataStoreHandles
{
    CsrSchedQid                 queueId;
    CsrDataStoreOperation       currentOperation;
    CsrUtf8String              *storeName;
    CsrUint16                   keyLength;
    CsrUint32                   recordLength;
    CsrFsalHandle              *fileHandle;
    CsrUint8                   *key;
    CsrUint8                   *record;
    struct CsrDataStoreHandles *next;
} CsrDataStoreHandles;

typedef struct
{
    CsrBool           initialized;
    CsrDataStoreState state;

    CsrMessageQueueType *saveQueue;
    CsrBool              restoreFlag;

    CsrUtf8String *currentWorkingDir;
    CsrFsalSession sessionId;
    CsrUtf8String *storagePrefix;

    CsrDataStoreHandles *handles;

    CsrDataStoreHandles *handleInProgess;

    void *msg;
} CsrDataStoreInstanceData;

typedef void (*CsrDataStoreEventType)(CsrDataStoreInstanceData *instData);

#ifdef __cplusplus
}
#endif

#endif
