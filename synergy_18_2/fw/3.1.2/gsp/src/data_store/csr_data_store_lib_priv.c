/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_data_store_lib.h"
#include "csr_pmem.h"

CsrDataStoreCreateCfm *CsrDataStoreCreateCfm_struct(
    CsrUtf8String *storeName,
    CsrResult result,
    CsrDataStoreHandle storeHandle)
{
    CsrDataStoreCreateCfm *prim = pnew(CsrDataStoreCreateCfm);

    prim->type = CSR_DATA_STORE_CREATE_CFM;
    prim->storeName = storeName;
    prim->result = result;
    prim->storeHandle = storeHandle;

    return prim;
}

CsrDataStoreOpenCfm *CsrDataStoreOpenCfm_struct(
    CsrUtf8String *storeName,
    CsrResult result,
    CsrDataStoreHandle storeHandle,
    CsrUint16 keyLength,
    CsrUint32 recordLength)
{
    CsrDataStoreOpenCfm *prim = pnew(CsrDataStoreOpenCfm);

    prim->type = CSR_DATA_STORE_OPEN_CFM;
    prim->storeName = storeName;
    prim->result = result;
    prim->storeHandle = storeHandle;
    prim->keyLength = keyLength;
    prim->recordLength = recordLength;

    return prim;
}

CsrDataStoreCloseCfm *CsrDataStoreCloseCfm_struct(
    CsrDataStoreHandle storeHandle)
{
    CsrDataStoreCloseCfm *prim = pnew(CsrDataStoreCloseCfm);

    prim->type = CSR_DATA_STORE_CLOSE_CFM;
    prim->storeHandle = storeHandle;

    return prim;
}

CsrDataStoreDeleteCfm *CsrDataStoreDeleteCfm_struct(
    CsrUtf8String *storeName,
    CsrResult result)
{
    CsrDataStoreDeleteCfm *prim = pnew(CsrDataStoreDeleteCfm);

    prim->type = CSR_DATA_STORE_DELETE_CFM;
    prim->storeName = storeName;
    prim->result = result;

    return prim;
}

CsrDataStoreRecordReadCfm *CsrDataStoreRecordReadCfm_struct(
    CsrDataStoreHandle storeHandle,
    CsrResult result,
    CsrUint16 keyLength,
    CsrUint8 *key,
    CsrUint32 recordLength,
    CsrUint8 *record)
{
    CsrDataStoreRecordReadCfm *prim = pnew(CsrDataStoreRecordReadCfm);

    prim->type = CSR_DATA_STORE_RECORD_READ_CFM;
    prim->storeHandle = storeHandle;
    prim->result = result;
    prim->keyLength = keyLength;
    prim->key = key;
    prim->recordLength = recordLength;
    prim->record = record;

    return prim;
}

CsrDataStoreRecordWriteCfm *CsrDataStoreRecordWriteCfm_struct(
    CsrDataStoreHandle storeHandle,
    CsrResult result,
    CsrUint16 keyLength,
    CsrUint8 *key)
{
    CsrDataStoreRecordWriteCfm *prim = pnew(CsrDataStoreRecordWriteCfm);

    prim->type = CSR_DATA_STORE_RECORD_WRITE_CFM;
    prim->storeHandle = storeHandle;
    prim->result = result;
    prim->keyLength = keyLength;
    prim->key = key;

    return prim;
}

CsrDataStoreRecordDeleteCfm *CsrDataStoreRecordDeleteCfm_struct(
    CsrDataStoreHandle storeHandle,
    CsrResult result,
    CsrUint16 keyLength,
    CsrUint8 *key)
{
    CsrDataStoreRecordDeleteCfm *prim = pnew(CsrDataStoreRecordDeleteCfm);

    prim->type = CSR_DATA_STORE_RECORD_DELETE_CFM;
    prim->storeHandle = storeHandle;
    prim->result = result;
    prim->keyLength = keyLength;
    prim->key = key;

    return prim;
}
