#ifndef CSR_DSPM_HANDLER_H__
#define CSR_DSPM_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_message_queue.h"
#include "csr_log_text_2.h"

#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
#include "csr_fp_prim.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrDspmLto);

#define CSR_DSPM_STATE_IDLE         0
#define CSR_DSPM_STATE_ACTIVE       1
#define CSR_DSPM_STATE_DEACTIVATING 2

typedef struct CsrDspmInfoTuple
{
    CsrUint16                key;
    CsrUint16                value;
    struct CsrDspmInfoTuple *next;
} CsrDspmInfoTuple;

typedef struct
{
    /* User list */
    CsrDspmInfoTuple *users;

    /* Opid to Qid look-up-list */
    CsrDspmInfoTuple *opids;

    /* Request queue */
    void                *currentRequest;
    CsrMessageQueueType *saveQueue;

    /* Chip build ID */
    CsrUint16 buildId;

    /* Holds the current item index during the operator patching sequence. Also
       used as a message sequence counter variable during capability download */
    CsrUint16 itemIndex;

#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
    /* Holds the last received status/reason/result code during capability download */
    CsrResult result;

    /* Holds the FastPipe handle for use during capability download */
    CsrFpHandleType fpHandle;
#endif

    /* The index within the current request (for messages with multiple operations) */
    CsrUint8 currentRequestIndex;

    /* Task state */
    CsrUint8 state;

    /* Sequence number for use in BCCMD */
    CsrUint16 bccmdSequenceNumber;
} CsrDspmInstanceData;

/* Forward declaration for csr_dspm_free_down.c */
void CsrDspmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif
