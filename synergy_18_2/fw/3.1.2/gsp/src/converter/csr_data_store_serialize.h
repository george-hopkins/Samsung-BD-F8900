#ifndef CSR_DATA_STORE_SERIALIZE_H__
#define CSR_DATA_STORE_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrData_storePfree(void *ptr);

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_REQ
CsrSize CsrDataStoreCloseReqSizeof(void *msg);
CsrUint8 *CsrDataStoreCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreCloseReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreCloseReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_CFM
CsrSize CsrDataStoreCloseCfmSizeof(void *msg);
CsrUint8 *CsrDataStoreCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreCloseCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreCloseCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM
CsrSize CsrDataStoreRecordReadCfmSizeof(void *msg);
CsrUint8 *CsrDataStoreRecordReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreRecordReadCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreRecordReadCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_REQ
CsrSize CsrDataStoreDeleteReqSizeof(void *msg);
CsrUint8 *CsrDataStoreDeleteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreDeleteReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreDeleteReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ
CsrSize CsrDataStoreRecordDeleteReqSizeof(void *msg);
CsrUint8 *CsrDataStoreRecordDeleteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreRecordDeleteReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreRecordDeleteReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_REQ
CsrSize CsrDataStoreOpenReqSizeof(void *msg);
CsrUint8 *CsrDataStoreOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreOpenReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreOpenReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_CFM
CsrSize CsrDataStoreDeleteCfmSizeof(void *msg);
CsrUint8 *CsrDataStoreDeleteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreDeleteCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreDeleteCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_REQ
CsrSize CsrDataStoreCreateReqSizeof(void *msg);
CsrUint8 *CsrDataStoreCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreCreateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreCreateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_CFM
CsrSize CsrDataStoreCreateCfmSizeof(void *msg);
CsrUint8 *CsrDataStoreCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreCreateCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreCreateCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ
CsrSize CsrDataStoreRecordWriteReqSizeof(void *msg);
CsrUint8 *CsrDataStoreRecordWriteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreRecordWriteReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreRecordWriteReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM
CsrSize CsrDataStoreRecordWriteCfmSizeof(void *msg);
CsrUint8 *CsrDataStoreRecordWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreRecordWriteCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreRecordWriteCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ
CsrSize CsrDataStoreRecordReadReqSizeof(void *msg);
CsrUint8 *CsrDataStoreRecordReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreRecordReadReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreRecordReadReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ */

#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_CFM
CsrSize CsrDataStoreOpenCfmSizeof(void *msg);
CsrUint8 *CsrDataStoreOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreOpenCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreOpenCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_CFM */

#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM
CsrSize CsrDataStoreRecordDeleteCfmSizeof(void *msg);
CsrUint8 *CsrDataStoreRecordDeleteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDataStoreRecordDeleteCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDataStoreRecordDeleteCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM */

#ifdef __cplusplus
}
#endif

#endif
