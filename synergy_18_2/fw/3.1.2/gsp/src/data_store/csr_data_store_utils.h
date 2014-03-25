#ifndef CSR_DATA_STORE_UTILS_H__
#define CSR_DATA_STORE_UTILS_H__
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

CsrUtf8String *CsrDataStoreKeyToFileName(CsrUint16 keyLength, CsrUint8 *key);
CsrDataStoreHandles *CsrDataStoreHandleInit(CsrDataStoreHandles **handles, CsrSchedQid queueId, CsrUtf8String *storeName);
void CsrDataStoreHandleRemove(CsrDataStoreHandles **handles, CsrDataStoreHandles *handle);
CsrBool CsrDataStoreValidateStoreHandle(CsrDataStoreInstanceData *inst, CsrDataStoreHandle handle);


void CsrDataStoreSaveMessage(CsrDataStoreInstanceData *inst);
void CsrDataStoreRestoreSavedMessages(CsrDataStoreInstanceData *inst);

#ifdef __cplusplus
}
#endif

#endif
