#ifndef CSR_TM_BLUECORE_HANDLER_H__
#define CSR_TM_BLUECORE_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_message_queue.h"
#include "csr_tm_bluecore_task.h"
#include "csr_bccmd_private_prim.h"
#include "csr_list.h"
#include "csr_transport.h"
#include "csr_time.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_TM_BLUECORE_STATE_IDLE                  (0x00)
#define CSR_TM_BLUECORE_STATE_ACTIVATING            (0x01)
#define CSR_TM_BLUECORE_STATE_RESTARTING            (0x02)
#define CSR_TM_BLUECORE_STATE_RESETTING             (0x03)
#define CSR_TM_BLUECORE_STATE_ACTIVATED             (0x04)
#define CSR_TM_BLUECORE_STATE_DEACTIVATING          (0x05)

typedef struct CsrTmBlueCoreHandleListTag
{
    struct CsrTmBlueCoreHandleListTag *next; /* must be 1st */
    struct CsrTmBlueCoreHandleListTag *prev; /* must be 2nd */
    CsrSchedQid                        phandle;
} CsrTmBlueCoreHandleList;

typedef struct
{
#ifdef CSR_BLUECORE_ONOFF
    CsrCmnList_t         transportDelegates;
    CsrSchedQid          transportActivator;
    CsrUint16            pendingResponses;
    CsrMessageQueueType *saveQueue;
#ifdef CSR_BLUECORE_PING_INTERVAL
    CsrSchedTid pingTimerId;
#endif
#else
    CsrCmnList_t transportActivators;
#endif
    CsrUint8    numberOfForcedReset;
    CsrUint8    state;
    CsrSchedTid timerId;
    void       *blueCoreTransportHandle;
    void       *recvMsgP;
    CsrSchedQid nopHandler;
    CsrUint8   *savedNop;
    CsrUint16   savedNopLength;
#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrBool      activateTransportCfmIsSend;
    CsrCmnList_t cmStatusSubscribers;
    CsrCmnList_t cmReplayersRegistered;
    CsrCmnList_t cmReplayersStarted;
    CsrSchedTid  pingTimerId;
    CsrTime      pingInterval;
    CsrBool      pingStarted;
#endif
} CsrTmBlueCoreInstanceData;

#define CSR_TM_BLUECORE_LIST_ADD(listPtr) (CsrTmBlueCoreHandleList *) CsrCmnListElementAddLast((CsrCmnList_t *) listPtr, sizeof(CsrTmBlueCoreHandleList))
#define CSR_TM_BLUECORE_LIST_ITERATE(listPtr, func, data) CsrCmnListIterate((CsrCmnList_t *) listPtr, func, (void *) data)
#define CSR_TM_BLUECORE_LIST_ITERATE_AND_REMOVE(listPtr, func, data) CsrCmnListIterateAllowRemove((CsrCmnList_t *) listPtr, func, (void *) data)
#define CSR_TM_BLUECORE_LIST_IS_EMPTY(listPtr) (CsrCmnListGetCount(listPtr) == 0)

#ifdef __cplusplus
}
#endif

#endif /* CSR_TM_BLUECORE_HANDLER_H__ */
