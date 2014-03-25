/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_data_store_converter_init.h"
#ifndef EXCLUDE_CSR_DATA_STORE_MODULE
#include "csr_data_store_serialize.h"
#include "csr_data_store_prim.h"

static CsrMsgConvMsgEntry csr_data_store_conv_lut[] = {
#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_CFM
    { CSR_DATA_STORE_CLOSE_CFM, CsrDataStoreCloseCfmSizeof, CsrDataStoreCloseCfmSer, CsrDataStoreCloseCfmDes, CsrDataStoreCloseCfmSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_CLOSE_REQ
    { CSR_DATA_STORE_CLOSE_REQ, CsrDataStoreCloseReqSizeof, CsrDataStoreCloseReqSer, CsrDataStoreCloseReqDes, CsrDataStoreCloseReqSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_CLOSE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_CFM
    { CSR_DATA_STORE_CREATE_CFM, CsrDataStoreCreateCfmSizeof, CsrDataStoreCreateCfmSer, CsrDataStoreCreateCfmDes, CsrDataStoreCreateCfmSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_CREATE_REQ
    { CSR_DATA_STORE_CREATE_REQ, CsrDataStoreCreateReqSizeof, CsrDataStoreCreateReqSer, CsrDataStoreCreateReqDes, CsrDataStoreCreateReqSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_CREATE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_CFM
    { CSR_DATA_STORE_DELETE_CFM, CsrDataStoreDeleteCfmSizeof, CsrDataStoreDeleteCfmSer, CsrDataStoreDeleteCfmDes, CsrDataStoreDeleteCfmSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_DELETE_REQ
    { CSR_DATA_STORE_DELETE_REQ, CsrDataStoreDeleteReqSizeof, CsrDataStoreDeleteReqSer, CsrDataStoreDeleteReqDes, CsrDataStoreDeleteReqSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_DELETE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_CFM
    { CSR_DATA_STORE_OPEN_CFM, CsrDataStoreOpenCfmSizeof, CsrDataStoreOpenCfmSer, CsrDataStoreOpenCfmDes, CsrDataStoreOpenCfmSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_OPEN_REQ
    { CSR_DATA_STORE_OPEN_REQ, CsrDataStoreOpenReqSizeof, CsrDataStoreOpenReqSer, CsrDataStoreOpenReqDes, CsrDataStoreOpenReqSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_OPEN_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM
    { CSR_DATA_STORE_RECORD_DELETE_CFM, CsrDataStoreRecordDeleteCfmSizeof, CsrDataStoreRecordDeleteCfmSer, CsrDataStoreRecordDeleteCfmDes, CsrDataStoreRecordDeleteCfmSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ
    { CSR_DATA_STORE_RECORD_DELETE_REQ, CsrDataStoreRecordDeleteReqSizeof, CsrDataStoreRecordDeleteReqSer, CsrDataStoreRecordDeleteReqDes, CsrDataStoreRecordDeleteReqSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_DELETE_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM
    { CSR_DATA_STORE_RECORD_READ_CFM, CsrDataStoreRecordReadCfmSizeof, CsrDataStoreRecordReadCfmSer, CsrDataStoreRecordReadCfmDes, CsrDataStoreRecordReadCfmSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ
    { CSR_DATA_STORE_RECORD_READ_REQ, CsrDataStoreRecordReadReqSizeof, CsrDataStoreRecordReadReqSer, CsrDataStoreRecordReadReqDes, CsrDataStoreRecordReadReqSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_READ_REQ */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM
    { CSR_DATA_STORE_RECORD_WRITE_CFM, CsrDataStoreRecordWriteCfmSizeof, CsrDataStoreRecordWriteCfmSer, CsrDataStoreRecordWriteCfmDes, CsrDataStoreRecordWriteCfmSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_CFM */
#ifndef EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ
    { CSR_DATA_STORE_RECORD_WRITE_REQ, CsrDataStoreRecordWriteReqSizeof, CsrDataStoreRecordWriteReqSer, CsrDataStoreRecordWriteReqDes, CsrDataStoreRecordWriteReqSerFree },
#endif /* EXCLUDE_CSR_DATA_STORE_RECORD_WRITE_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrDataStoreConverterInit(void)
{
    CsrMsgConvInsert(CSR_DATA_STORE_PRIM, csr_data_store_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_data_store_conv_info = {
    CSR_DATA_STORE_PRIM,
    "CSR_DATA_STORE_PRIM",
    csr_data_store_conv_lut
};

CsrLogPrimitiveInformation* CsrDataStoreTechInfoGet(void)
{
    return &csr_data_store_conv_info;
}
#endif
#endif

