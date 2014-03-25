#ifndef CSR_FSAL_HANDLER_H__
#define CSR_FSAL_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_fsal_prim.h"
#include "csr_file.h"
#include "csr_dir.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrFsalLto);

#define CSR_FSAL_STATE_INITIALIZED          (0)

#define CSR_FSAL_NUMBER_OF_STATES           (1)

#define CSR_FSAL_INVALID_SESSION_ID         (0xFFFFFFFF)

typedef struct CsrFsalFileHandles
{
    CsrFileHandle             *fileHandle;
    struct CsrFsalFileHandles *next;
} CsrFsalFileHandles;

typedef struct CsrFsalDirHandles
{
    CsrDirHandle             *dirHandle;
    struct CsrFsalDirHandles *next;
} CsrFsalDirHandles;

typedef struct CsrFsalHandles
{
    CsrSchedQid            appHandle;
    CsrFsalSession         sessionId;
    CsrUtf8String         *currentWorkingDir;
    CsrFsalFileHandles    *fileHandles;
    CsrFsalDirHandles     *dirHandles;
    struct CsrFsalHandles *next;
} CsrFsalHandles;

typedef struct
{
    CsrUint8        state;
    void           *recvMsgP;
    CsrFsalHandles *handleList;
} CsrFsalInstanceData;


#ifdef __cplusplus
}
#endif

#endif /* CSR_FSAL_HANDLER_H__ */
