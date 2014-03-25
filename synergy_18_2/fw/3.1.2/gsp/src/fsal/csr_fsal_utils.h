#ifndef CSR_FSAL_UTILS_H__
#define CSR_FSAL_UTILS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_file.h"
#include "csr_fsal_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrFsalFileHandles *CsrFsalFileHandleInit(CsrFsalFileHandles **handles, CsrFileHandle *fileHandle);
CsrFsalDirHandles *CsrFsalDirHandleInit(CsrFsalDirHandles **handles, CsrDirHandle *dirHandle);

CsrSchedQid CsrFsalGetAppHandleFromSessionId(CsrFsalHandles **handles, CsrFsalSession sessionId);
CsrSchedQid CsrFsalGetAppHandleFromFileHandle(CsrFsalInstanceData *pInst, CsrFileHandle *fileHandle);
CsrSchedQid CsrFsalGetAppHandleFromDirHandle(CsrFsalInstanceData *pInst, CsrDirHandle *dirHandle);
CsrSchedQid CsrFsalGetAppHandleFromSessionIdAndStoreFileHandle(CsrFsalInstanceData *pInst, CsrFsalSession sessionId, CsrFileHandle *fileHandle);
CsrSchedQid CsrFsalGetAppHandleFromSessionIdAndStoreDirHandle(CsrFsalInstanceData *pInst, CsrFsalSession sessionId, CsrDirHandle *dirHandle);
CsrSchedQid CsrFsalClearHandleIndexFromSessionId(CsrFsalInstanceData *pInst, CsrFsalSession sessionId);
CsrSchedQid CsrFsalGetAppHandleFromFileHandleAndClearFileHandleIndex(CsrFsalHandles **handles, CsrFileHandle *fileHandle);
CsrSchedQid CsrFsalGetAppHandleFromDirHandleAndClearDirHandleIndex(CsrFsalHandles **handles, CsrDirHandle *dirHandle);
CsrFsalHandles *CsrFsalReserveNewSessionId(CsrFsalInstanceData *pInst, CsrUint16 appHandle);
CsrFsalHandles *CsrFsalGetSession(CsrFsalInstanceData *pInst, CsrFsalSession sessionId);

#ifdef __cplusplus
}
#endif

#endif /* CSR_FSAL_UTILS_H__ */
