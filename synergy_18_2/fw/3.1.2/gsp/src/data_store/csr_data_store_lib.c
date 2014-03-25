/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_data_store_lib.h"
#include "csr_pmem.h"

CsrDataStoreCreateReq *CsrDataStoreCreateReq_struct(
    CsrSchedQid queueId,
    CsrUtf8String *storeName,
    CsrUint16 keyLength,
    CsrUint32 recordLength)
{
    CsrDataStoreCreateReq *prim = pnew(CsrDataStoreCreateReq);

    prim->type = CSR_DATA_STORE_CREATE_REQ;
    prim->queueId = queueId;
    prim->storeName = storeName;
    prim->keyLength = keyLength;
    prim->recordLength = recordLength;

    return prim;
}

CsrDataStoreOpenReq *CsrDataStoreOpenReq_struct(
    CsrSchedQid queueId,
    CsrUtf8String *storeName)
{
    CsrDataStoreOpenReq *prim = pnew(CsrDataStoreOpenReq);

    prim->type = CSR_DATA_STORE_OPEN_REQ;
    prim->queueId = queueId;
    prim->storeName = storeName;

    return prim;
}

CsrDataStoreCloseReq *CsrDataStoreCloseReq_struct(
    CsrSchedQid queueId,
    CsrDataStoreHandle storeHandle)
{
    CsrDataStoreCloseReq *prim = pnew(CsrDataStoreCloseReq);

    prim->type = CSR_DATA_STORE_CLOSE_REQ;
    prim->queueId = queueId;
    prim->storeHandle = storeHandle;

    return prim;
}

CsrDataStoreDeleteReq *CsrDataStoreDeleteReq_struct(
    CsrSchedQid queueId,
    CsrUtf8String *storeName)
{
    CsrDataStoreDeleteReq *prim = pnew(CsrDataStoreDeleteReq);

    prim->type = CSR_DATA_STORE_DELETE_REQ;
    prim->queueId = queueId;
    prim->storeName = storeName;

    return prim;
}

CsrDataStoreRecordReadReq *CsrDataStoreRecordReadReq_struct(
    CsrSchedQid queueId,
    CsrDataStoreHandle storeHandle,
    CsrUint16 keyLength,
    CsrUint8 *key)
{
    CsrDataStoreRecordReadReq *prim = pnew(CsrDataStoreRecordReadReq);

    prim->type = CSR_DATA_STORE_RECORD_READ_REQ;
    prim->queueId = queueId;
    prim->storeHandle = storeHandle;
    prim->keyLength = keyLength;
    prim->key = key;

    return prim;
}

CsrDataStoreRecordWriteReq *CsrDataStoreRecordWriteReq_struct(
    CsrSchedQid queueId,
    CsrDataStoreHandle storeHandle,
    CsrUint16 keyLength,
    CsrUint8 *key,
    CsrUint32 recordLength,
    CsrUint8 *record)
{
    CsrDataStoreRecordWriteReq *prim = pnew(CsrDataStoreRecordWriteReq);

    prim->type = CSR_DATA_STORE_RECORD_WRITE_REQ;
    prim->queueId = queueId;
    prim->storeHandle = storeHandle;
    prim->keyLength = keyLength;
    prim->key = key;
    prim->recordLength = recordLength;
    prim->record = record;

    return prim;
}

CsrDataStoreRecordDeleteReq *CsrDataStoreRecordDeleteReq_struct(
    CsrSchedQid queueId,
    CsrDataStoreHandle storeHandle,
    CsrUint16 keyLength,
    CsrUint8 *key)
{
    CsrDataStoreRecordDeleteReq *prim = pnew(CsrDataStoreRecordDeleteReq);

    prim->type = CSR_DATA_STORE_RECORD_DELETE_REQ;
    prim->queueId = queueId;
    prim->storeHandle = storeHandle;
    prim->keyLength = keyLength;
    prim->key = key;

    return prim;
}
