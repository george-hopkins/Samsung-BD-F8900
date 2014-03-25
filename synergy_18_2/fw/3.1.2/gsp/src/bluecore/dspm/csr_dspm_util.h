#ifndef CSR_DSPM_UTIL_H__
#define CSR_DSPM_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_dspm_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

/* CsrDspmInfoTuple list management */
void CsrDspmInfoTupleInsert(CsrDspmInfoTuple **list, CsrUint16 key, CsrUint16 value);
CsrResult CsrDspmInfoTupleLookup(CsrDspmInfoTuple **list, CsrUint16 key, CsrUint16 *value);
CsrResult CsrDspmInfoTupleRemove(CsrDspmInfoTuple **list, CsrUint16 key);
void CsrDspmInfoTupleRemoveAll(CsrDspmInfoTuple **list);

/* Request Queue */
void CsrDspmRequestQueueHandle(CsrDspmInstanceData *instanceData);
void CsrDspmRequestQueuePushHandle(CsrDspmInstanceData *instanceData, void *message);
void CsrDspmRequestQueuePop(CsrDspmInstanceData *instanceData);
void CsrDspmRequestQueuePopHandle(CsrDspmInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
