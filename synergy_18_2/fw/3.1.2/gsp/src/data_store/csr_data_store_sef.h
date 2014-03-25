#ifndef CSR_DATA_STORE_SEF_H__
#define CSR_DATA_STORE_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_data_store_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrDataStoreCreateReqHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreOpenReqHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreCloseReqHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreDeleteReqHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreRecordReadReqHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreRecordWriteReqHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreRecordDeleteReqHandler(CsrDataStoreInstanceData *inst);


void CsrDataStoreFsalSessionCreateCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalSessionDestroyCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalFileOpenCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalFileCloseCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalFileSeekCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalFileReadCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalFileWriteCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalFileTellCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalStatCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalRemoveCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalRenameCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalPermissionsSetCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalDirMakeCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalDirChangeCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalDirFindFirstCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalDirFindNextCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalDirFindCloseCfmHandler(CsrDataStoreInstanceData *inst);
void CsrDataStoreFsalRemoveRecursivelyCfmHandler(CsrDataStoreInstanceData *inst);

#ifdef __cplusplus
}
#endif

#endif
