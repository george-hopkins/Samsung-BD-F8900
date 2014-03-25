/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_data_store_prim.h"

#ifndef EXCLUDE_CSR_DATA_STORE_MODULE

void CsrData_storePfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_REQ
CsrSize CsrDataStoreCloseReqSizeof(void *msg)
{
    CsrDataStoreCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_REQ
CsrUint8 *CsrDataStoreCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreCloseReq *primitive;

    primitive = (CsrDataStoreCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_REQ
void *CsrDataStoreCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreCloseReq *) CsrPmemZalloc(sizeof(CsrDataStoreCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_REQ
void CsrDataStoreCloseReqSerFree(void *msg)
{
    CsrDataStoreCloseReq *primitive;

    primitive = (CsrDataStoreCloseReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_CFM
CsrSize CsrDataStoreCloseCfmSizeof(void *msg)
{
    CsrDataStoreCloseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_CFM
CsrUint8 *CsrDataStoreCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreCloseCfm *primitive;

    primitive = (CsrDataStoreCloseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_CFM
void *CsrDataStoreCloseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreCloseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreCloseCfm *) CsrPmemZalloc(sizeof(CsrDataStoreCloseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_CFM
void CsrDataStoreCloseCfmSerFree(void *msg)
{
    CsrDataStoreCloseCfm *primitive;

    primitive = (CsrDataStoreCloseCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM
CsrSize CsrDataStoreRecordReadCfmSizeof(void *msg)
{
    CsrDataStoreRecordReadCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keyLength * sizeof(CsrUint8)) + (primitive->recordLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM
CsrUint8 *CsrDataStoreRecordReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreRecordReadCfm *primitive;

    primitive = (CsrDataStoreRecordReadCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->recordLength);
    CsrMemCpySer(buffer, length, &primitive->record, sizeof(primitive->record));
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    if(primitive->record)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->record), ((CsrSize) (primitive->recordLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM
void *CsrDataStoreRecordReadCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreRecordReadCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreRecordReadCfm *) CsrPmemZalloc(sizeof(CsrDataStoreRecordReadCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    CsrUint32Des((CsrUint32 *) &primitive->recordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->record), buffer, &offset, ((CsrSize) sizeof(primitive->record)));
    if (primitive->keyLength)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }
    if (primitive->recordLength)
    {
        primitive->record = CsrPmemZalloc(((CsrUint32) (primitive->recordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->record), buffer, &offset, ((CsrSize) (primitive->recordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->record = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM
void CsrDataStoreRecordReadCfmSerFree(void *msg)
{
    CsrDataStoreRecordReadCfm *primitive;

    primitive = (CsrDataStoreRecordReadCfm *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    if(primitive->record != NULL)
    {
        CsrPmemFree(primitive->record);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_REQ
CsrSize CsrDataStoreDeleteReqSizeof(void *msg)
{
    CsrDataStoreDeleteReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->storeName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_REQ
CsrUint8 *CsrDataStoreDeleteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreDeleteReq *primitive;

    primitive = (CsrDataStoreDeleteReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->storeName, sizeof(primitive->storeName));
    CsrUtf8StringSer(buffer, length, primitive->storeName);

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_REQ
void *CsrDataStoreDeleteReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreDeleteReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreDeleteReq *) CsrPmemZalloc(sizeof(CsrDataStoreDeleteReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeName), buffer, &offset, ((CsrSize) sizeof(primitive->storeName)));
    CsrUtf8StringDes(&primitive->storeName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_REQ
void CsrDataStoreDeleteReqSerFree(void *msg)
{
    CsrDataStoreDeleteReq *primitive;

    primitive = (CsrDataStoreDeleteReq *) msg;

    if(primitive->storeName != NULL)
    {
        CsrPmemFree(primitive->storeName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ
CsrSize CsrDataStoreRecordDeleteReqSizeof(void *msg)
{
    CsrDataStoreRecordDeleteReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ
CsrUint8 *CsrDataStoreRecordDeleteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreRecordDeleteReq *primitive;

    primitive = (CsrDataStoreRecordDeleteReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ
void *CsrDataStoreRecordDeleteReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreRecordDeleteReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreRecordDeleteReq *) CsrPmemZalloc(sizeof(CsrDataStoreRecordDeleteReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    if (primitive->keyLength)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ
void CsrDataStoreRecordDeleteReqSerFree(void *msg)
{
    CsrDataStoreRecordDeleteReq *primitive;

    primitive = (CsrDataStoreRecordDeleteReq *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_REQ
CsrSize CsrDataStoreOpenReqSizeof(void *msg)
{
    CsrDataStoreOpenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->storeName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_REQ
CsrUint8 *CsrDataStoreOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreOpenReq *primitive;

    primitive = (CsrDataStoreOpenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->storeName, sizeof(primitive->storeName));
    CsrUtf8StringSer(buffer, length, primitive->storeName);

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_REQ
void *CsrDataStoreOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreOpenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreOpenReq *) CsrPmemZalloc(sizeof(CsrDataStoreOpenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeName), buffer, &offset, ((CsrSize) sizeof(primitive->storeName)));
    CsrUtf8StringDes(&primitive->storeName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_REQ
void CsrDataStoreOpenReqSerFree(void *msg)
{
    CsrDataStoreOpenReq *primitive;

    primitive = (CsrDataStoreOpenReq *) msg;

    if(primitive->storeName != NULL)
    {
        CsrPmemFree(primitive->storeName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_CFM
CsrSize CsrDataStoreDeleteCfmSizeof(void *msg)
{
    CsrDataStoreDeleteCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->storeName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_CFM
CsrUint8 *CsrDataStoreDeleteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreDeleteCfm *primitive;

    primitive = (CsrDataStoreDeleteCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->storeName, sizeof(primitive->storeName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUtf8StringSer(buffer, length, primitive->storeName);

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_CFM
void *CsrDataStoreDeleteCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreDeleteCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreDeleteCfm *) CsrPmemZalloc(sizeof(CsrDataStoreDeleteCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeName), buffer, &offset, ((CsrSize) sizeof(primitive->storeName)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUtf8StringDes(&primitive->storeName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_CFM
void CsrDataStoreDeleteCfmSerFree(void *msg)
{
    CsrDataStoreDeleteCfm *primitive;

    primitive = (CsrDataStoreDeleteCfm *) msg;

    if(primitive->storeName != NULL)
    {
        CsrPmemFree(primitive->storeName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_REQ
CsrSize CsrDataStoreCreateReqSizeof(void *msg)
{
    CsrDataStoreCreateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->storeName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_REQ
CsrUint8 *CsrDataStoreCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreCreateReq *primitive;

    primitive = (CsrDataStoreCreateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->storeName, sizeof(primitive->storeName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->recordLength);
    CsrUtf8StringSer(buffer, length, primitive->storeName);

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_REQ
void *CsrDataStoreCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreCreateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreCreateReq *) CsrPmemZalloc(sizeof(CsrDataStoreCreateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeName), buffer, &offset, ((CsrSize) sizeof(primitive->storeName)));
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->recordLength, buffer, &offset);
    CsrUtf8StringDes(&primitive->storeName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_REQ
void CsrDataStoreCreateReqSerFree(void *msg)
{
    CsrDataStoreCreateReq *primitive;

    primitive = (CsrDataStoreCreateReq *) msg;

    if(primitive->storeName != NULL)
    {
        CsrPmemFree(primitive->storeName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_CFM
CsrSize CsrDataStoreCreateCfmSizeof(void *msg)
{
    CsrDataStoreCreateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->storeName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_CFM
CsrUint8 *CsrDataStoreCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreCreateCfm *primitive;

    primitive = (CsrDataStoreCreateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->storeName, sizeof(primitive->storeName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUtf8StringSer(buffer, length, primitive->storeName);

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_CFM
void *CsrDataStoreCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreCreateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreCreateCfm *) CsrPmemZalloc(sizeof(CsrDataStoreCreateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeName), buffer, &offset, ((CsrSize) sizeof(primitive->storeName)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUtf8StringDes(&primitive->storeName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_CFM
void CsrDataStoreCreateCfmSerFree(void *msg)
{
    CsrDataStoreCreateCfm *primitive;

    primitive = (CsrDataStoreCreateCfm *) msg;

    if(primitive->storeName != NULL)
    {
        CsrPmemFree(primitive->storeName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ
CsrSize CsrDataStoreRecordWriteReqSizeof(void *msg)
{
    CsrDataStoreRecordWriteReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keyLength * sizeof(CsrUint8)) + (primitive->recordLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ
CsrUint8 *CsrDataStoreRecordWriteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreRecordWriteReq *primitive;

    primitive = (CsrDataStoreRecordWriteReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->recordLength);
    CsrMemCpySer(buffer, length, &primitive->record, sizeof(primitive->record));
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    if(primitive->record)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->record), ((CsrSize) (primitive->recordLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ
void *CsrDataStoreRecordWriteReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreRecordWriteReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreRecordWriteReq *) CsrPmemZalloc(sizeof(CsrDataStoreRecordWriteReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    CsrUint32Des((CsrUint32 *) &primitive->recordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->record), buffer, &offset, ((CsrSize) sizeof(primitive->record)));
    if (primitive->keyLength)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }
    if (primitive->recordLength)
    {
        primitive->record = CsrPmemZalloc(((CsrUint32) (primitive->recordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->record), buffer, &offset, ((CsrSize) (primitive->recordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->record = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ
void CsrDataStoreRecordWriteReqSerFree(void *msg)
{
    CsrDataStoreRecordWriteReq *primitive;

    primitive = (CsrDataStoreRecordWriteReq *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    if(primitive->record != NULL)
    {
        CsrPmemFree(primitive->record);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM
CsrSize CsrDataStoreRecordWriteCfmSizeof(void *msg)
{
    CsrDataStoreRecordWriteCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM
CsrUint8 *CsrDataStoreRecordWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreRecordWriteCfm *primitive;

    primitive = (CsrDataStoreRecordWriteCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM
void *CsrDataStoreRecordWriteCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreRecordWriteCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreRecordWriteCfm *) CsrPmemZalloc(sizeof(CsrDataStoreRecordWriteCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    if (primitive->keyLength)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM
void CsrDataStoreRecordWriteCfmSerFree(void *msg)
{
    CsrDataStoreRecordWriteCfm *primitive;

    primitive = (CsrDataStoreRecordWriteCfm *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ
CsrSize CsrDataStoreRecordReadReqSizeof(void *msg)
{
    CsrDataStoreRecordReadReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ
CsrUint8 *CsrDataStoreRecordReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreRecordReadReq *primitive;

    primitive = (CsrDataStoreRecordReadReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ
void *CsrDataStoreRecordReadReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreRecordReadReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreRecordReadReq *) CsrPmemZalloc(sizeof(CsrDataStoreRecordReadReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    if (primitive->keyLength)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ
void CsrDataStoreRecordReadReqSerFree(void *msg)
{
    CsrDataStoreRecordReadReq *primitive;

    primitive = (CsrDataStoreRecordReadReq *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_CFM
CsrSize CsrDataStoreOpenCfmSizeof(void *msg)
{
    CsrDataStoreOpenCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->storeName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_CFM
CsrUint8 *CsrDataStoreOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreOpenCfm *primitive;

    primitive = (CsrDataStoreOpenCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->storeName, sizeof(primitive->storeName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->recordLength);
    CsrUtf8StringSer(buffer, length, primitive->storeName);

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_CFM
void *CsrDataStoreOpenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreOpenCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreOpenCfm *) CsrPmemZalloc(sizeof(CsrDataStoreOpenCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeName), buffer, &offset, ((CsrSize) sizeof(primitive->storeName)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->recordLength, buffer, &offset);
    CsrUtf8StringDes(&primitive->storeName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_CFM
void CsrDataStoreOpenCfmSerFree(void *msg)
{
    CsrDataStoreOpenCfm *primitive;

    primitive = (CsrDataStoreOpenCfm *) msg;

    if(primitive->storeName != NULL)
    {
        CsrPmemFree(primitive->storeName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM
CsrSize CsrDataStoreRecordDeleteCfmSizeof(void *msg)
{
    CsrDataStoreRecordDeleteCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM
CsrUint8 *CsrDataStoreRecordDeleteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDataStoreRecordDeleteCfm *primitive;

    primitive = (CsrDataStoreRecordDeleteCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->storeHandle, sizeof(primitive->storeHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyLength);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM
void *CsrDataStoreRecordDeleteCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDataStoreRecordDeleteCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDataStoreRecordDeleteCfm *) CsrPmemZalloc(sizeof(CsrDataStoreRecordDeleteCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->storeHandle), buffer, &offset, ((CsrSize) sizeof(primitive->storeHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    if (primitive->keyLength)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM
void CsrDataStoreRecordDeleteCfmSerFree(void *msg)
{
    CsrDataStoreRecordDeleteCfm *primitive;

    primitive = (CsrDataStoreRecordDeleteCfm *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM */

#endif /* EXCLUDE_CSR_DATA_STORE_MODULE */
