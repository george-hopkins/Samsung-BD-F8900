#ifndef CSR_BCCMD_HANDLER_H__
#define CSR_BCCMD_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_pmem.h"
#include "csr_message_queue.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_bccmd_prim.h"
#include "csr_bccmd_private_prim.h"
#include "csr_bccmd_task.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrBccmdLto);

#define CSR_BCCMD_MIN_SIZE                      0x12
#define CSR_BCCMD_STD_HEADER_SIZE               0x0A
#define CSR_BCCMD_PS_VARID                    0x7003
#define CSR_BCCMD_COLD_RESET_VARID            0x4001
#define CSR_BCCMD_WARM_RESET_VARID            0x4002
#define CSR_BCCMD_BUILD_ID_VARID              0x2819
#define CSR_BCCMD_PANIC_ARG_VARID             0x6805
#define CSR_BCCMD_FAULT_ARG_VARID             0x6806
#define CSR_BCCMD_INVALID_VARID               0x0000

#define CSR_BCCMD_STATE_ACTIVATING_TRANSPORT    0
#define CSR_BCCMD_STATE_TRANSPORT_ACTIVATED     1
#define CSR_BCCMD_STATE_DEACTIVATING_TRANSPORT  2

#ifdef CSR_CHIP_MANAGER_ENABLE
struct CsrBccmdCmInstanceData;
#endif

typedef struct
{
    CsrBool   resetInitiated;
    CsrUint16 currentIndex;
    CsrUint16 payloadLength;
    CsrUint8 *payload;
} CsrBccmdBoot;

typedef struct
{
    CsrSchedQid          phandle;
    CsrBool              inProgress;
    CsrUint8             state;
    CsrBccmdBoot        *bootVar;
    CsrMessageQueueType *messageQueue;
    void                *recvMsgP;
#ifdef CSR_BLUECORE_ONOFF
    CsrSchedTid timerId;
#endif
#ifdef CSR_CHIP_MANAGER_ENABLE
    struct CsrBccmdCmInstanceData *cmInst;
    CsrResult                      panicStatus;
    CsrUint16                      panicArg;
#endif
} CsrBccmdInstanceData;

void CsrBccmdFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BCCMD_HANDLER_H__ */
