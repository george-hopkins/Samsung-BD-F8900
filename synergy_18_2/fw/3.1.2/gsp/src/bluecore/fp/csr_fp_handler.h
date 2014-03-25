#ifndef CSR_FP_HANDLER_H__
#define CSR_FP_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_fp_prim.h"
#include "csr_message_queue.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrFpLto);

/* Task States */
#define CSR_FP_INIT_ST          0
#define CSR_FP_ACTIVE_ST        1
#define CSR_FP_DEACTIVATING_ST  2

#define CSR_FP_NUM_MAX        15

typedef struct CsrFpClist
{
    struct CsrFpClist *next;
    struct CsrFpClist *previous;
    void              *data;
} CsrFpClistType;

typedef struct
{
    CsrSchedQid    appHandle;
    CsrUint16      aclHandle;
    CsrUint32      creditRx;
    CsrUint32      creditTx;
    CsrUint32      creditTxMax;
    CsrUint32      offset;
    CsrFpClistType cSendListBottom;
    CsrUint32      packetsOnQueue;
    CsrBool        outStandingReadRes;
    CsrBool        clearNotch;
    CsrBool        destroyNotch;
} CsrFpContainerType;

/* Instance data */
typedef struct
{
    void                *msg;
    CsrUint16            state;
    CsrMessageQueueType *queue;
    CsrFpHandleType      last;
    CsrFpContainerType  *list[CSR_FP_NUM_MAX + 1];
} CsrFpInstType;

#ifdef __cplusplus
}
#endif

#endif
