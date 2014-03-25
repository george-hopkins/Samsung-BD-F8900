#ifndef CSR_BCCMD_CM_H__
#define CSR_BCCMD_CM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_types.h"
#include "csr_sched.h"
#include "csr_list.h"
#include "csr_bccmd_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BCCMD_CM_STATE_NORMAL                    0x00
#define CSR_BCCMD_CM_STATE_RESETTING                 0x01
#define CSR_BCCMD_CM_REPLAYING                       0x02

#define CSR_BCCMD_CM_WARM_RESET                0x4002
#define CSR_BCCMD_CM_COLD_RESET                0x4001
#define CSR_BCCMD_CM_PSKEY_VARID               0x7003  /* PSKEY */
#define CSR_BCCMD_CM_PSKEY_BDADDR              0x0001  /* PKEY_BDADDR */
#define CSR_BCCMD_CM_PSKEY_SEQNO               0x1234  /* Unique */
#define CSR_BCCMD_CM_TYPE_SET                  0x0002

#define CSR_BCCMD_CM_PSKEY_BDADDR_LEN            0x18  /* PKEY_BDADDR Length in bytes*/

/* If a non-reset BCCMD request haven't had a response before this timeout
   the RM will interpret this as a reset. Timeout is in us. */
#define CSR_BCCMD_CM_RES_TIMEOUT (2000000)

/* If a reset (warm or cold) BCCMD request haven't had a response before this timeout
   the BC CM will notify CM. Timeout is in us. */
#define CSR_BCCMD_CM_RESET_TIMEOUT (5000000)

/* Generate key from sequence number and var id */
#define CSR_BCCMD_CM_KEY(ptr) \
    (CsrUint32) ((((CsrUint32) (ptr)[4]) << 24) | \
                 (((CsrUint32) (ptr)[5]) << 16) | \
                 (((CsrUint32) (ptr)[6]) << 8) | \
                 (((CsrUint32) (ptr)[7])))

#define CSR_BCCMD_CM_KEY_INVALID ((CsrUint32) 0x00000000)

#define CSR_BCCMD_CM_VARID(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[7]) << 8) |   \
                 ((CsrUint16) (ptr)[6]))

#define CSR_BCCMD_CM_SEQNO(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[5]) << 8) |   \
                 ((CsrUint16) (ptr)[4]))

#define CSR_BCCMD_CM_PSKEY(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[11]) << 8) |   \
                 ((CsrUint16) (ptr)[10]))

#define CSR_BCCMD_CM_LENGTH(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[3]) << 8) |   \
                 ((CsrUint16) (ptr)[2]))

#define CSR_BCCMD_CM_TYPE(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[1]) << 8) |   \
                 ((CsrUint16) (ptr)[0]))

#define CSR_BCCMD_CM_SET_ERROR_RES(ptr) \
    ((CsrUint8 *) (ptr))[0] = 0x01;                 \
    ((CsrUint8 *) (ptr))[1] = 0x00;                 \
    ((CsrUint8 *) (ptr))[8] = 0x08;                 \
    ((CsrUint8 *) (ptr))[9] = 0x00;

#define BCCMD_PSKEY_HDR_LEN (16)

#define CSR_BCCMD_CM_BDADDR_LAP_LOW(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[BCCMD_PSKEY_HDR_LEN + 1]) << 8) |   \
                 ((CsrUint16) (ptr)[BCCMD_PSKEY_HDR_LEN]))

#define CSR_BCCMD_CM_BDADDR_LAP_HIGH(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[BCCMD_PSKEY_HDR_LEN + 3]) << 8) |   \
                 ((CsrUint16) (ptr)[BCCMD_PSKEY_HDR_LEN + 2]))

#define CSR_BCCMD_CM_BDADDR_LAP(ptr) \
    (CsrUint24) ((((CsrUint24) CSR_BCCMD_CM_BDADDR_LAP_HIGH(ptr)) << 16) | \
                 ((CsrUint24) CSR_BCCMD_CM_BDADDR_LAP_LOW(ptr)))

#define CSR_BCCMD_CM_BDADDR_UAP(ptr) \
    (CsrUint8) (((CsrUint8) (ptr)[BCCMD_PSKEY_HDR_LEN + 4]))

#define CSR_BCCMD_CM_BDADDR_NAP(ptr) \
    (CsrUint16) ((((CsrUint16) (ptr)[BCCMD_PSKEY_HDR_LEN + 7]) << 8) |   \
                 ((CsrUint16) (ptr)[BCCMD_PSKEY_HDR_LEN + 6]))

#define CSR_BCCMD_CM_IS_BDADDR_QUERY(msg) \
    ((CSR_BCCMD_CM_VARID(msg)) == (CSR_BCCMD_CM_PSKEY_VARID) && \
     (CSR_BCCMD_CM_SEQNO(msg)) == (CSR_BCCMD_CM_PSKEY_SEQNO) && \
     (CSR_BCCMD_CM_PSKEY(msg)) == (CSR_BCCMD_CM_PSKEY_BDADDR))

#define CSR_BCCMD_CM_IS_BDADDR_SET(msg) \
    ((CSR_BCCMD_CM_VARID(msg)) == (CSR_BCCMD_CM_PSKEY_VARID) && \
     (CSR_BCCMD_CM_TYPE(msg)) == (CSR_BCCMD_CM_TYPE_SET) && \
     (CSR_BCCMD_CM_PSKEY(msg)) == (CSR_BCCMD_CM_PSKEY_BDADDR))

typedef struct CsrBccmdCmListTypeTag
{
    struct CsrBccmdCmListTypeTag *next; /* must be 1st */
    struct CsrBccmdCmListTypeTag *prev; /* must be 2nd */
    CsrUint32                     key;
    void                         *msg;
    CsrSchedQid                   pHandle;
} CsrBccmdCmListType;

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
typedef struct
{
    CsrUint24 lap;        /* Lower Address Part 00..23 */
    CsrUint8  uap;              /* Upper Address Part 24..31 */
    CsrUint16 nap;        /* Non-significant    32..47 */
} CsrBccmdCmDeviceAddrType;

typedef struct
{
    void       *msg;
    CsrUint16   len;
    CsrBool     highPriority;
    CsrSchedQid pHandle;
} CsrBccmdCmSavedBccmdMsgType;
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */

typedef struct CsrBccmdCmInstanceData
{
    CsrCmnList_t bccmdList;
    CsrUint8     state;
    CsrSchedTid  bccmdResTimerId;
    CsrSchedTid  bccmdResetTimerId;
    CsrBool      bccmdResetTimerFired;
    CsrUint32    activeHighPriorityKey;
#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    CsrBool                     hciNopDetected;
    CsrBccmdCmSavedBccmdMsgType savedBccmd;
    CsrBccmdCmDeviceAddrType    expectedBdaddr;
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */
} CsrBccmdCmInstanceData;

typedef struct
{
    CsrUint32   cmdId;
    const void *eventArg;
    CsrBool     cmdComplete;
} CsrBccmdCmCompareType;

/* bccmdList specific macros                */
#define CSR_BCCMD_CM_ADD_TO_LIST(listPtr) (CsrBccmdCmListType *) CsrCmnListElementAddLast((CsrCmnList_t *) listPtr, sizeof(CsrBccmdCmListType))
#define CSR_BCCMD_CM_FIND_AND_MATCH_ON_LIST(listPtr, keyPtr) ((CsrBccmdCmListType *) CsrCmnListSearch((CsrCmnList_t *) listPtr, CsrBccmdCmCompareBccmd, (void *) (keyPtr)))
#define CSR_BCCMD_CM_REMOVE_FROM_LIST(listPtr, elementPtr) (CsrCmnListElementRemove((CsrCmnList_t *) listPtr, (CsrCmnListElm_t *) (elementPtr)))
#define CSR_BCCMD_CM_FIND_ON_LIST(listPtr, instPtr) (CsrCmnListIterate((CsrCmnList_t *) listPtr, CsrBccmdCmErrorResSend, (void *) instPtr))
#define CSR_BCCMD_CM_FREE_ALL_ON_LIST(listPtr, instPtr) (CsrCmnListIterate((CsrCmnList_t *) listPtr, CsrBccmdCmFreeBccmdElement, (void *) instPtr))
void CsrBccmdCmErrorResSend(CsrCmnListElm_t *elem, void *data);
void CsrBccmdCmFreeBccmdElement(CsrCmnListElm_t *elem, void *data);

void CsrBccmdCmInit(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdCmDeinit(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdCmResendStart(CsrBccmdInstanceData *bccmdInst);
CsrBool CsrBccmdCmReqHandler(CsrBccmdInstanceData *bccmdInst, void *request,
    CsrUint16 len, CsrBool highPriority);
CsrBool CsrBccmdCmResHandler(CsrBccmdInstanceData *bccmdInst, void *response);
void CsrBccmdCmResetDetectedRequest(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdCmRestartDetectedRequest(CsrBccmdInstanceData *bccmdInst);
#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
void CsrBccmdQueryReqHandler(CsrBccmdInstanceData *bccmdInst);
#endif
#ifdef __cplusplus
}
#endif

#endif /* CSR_BCCMD_CM_H__ */
