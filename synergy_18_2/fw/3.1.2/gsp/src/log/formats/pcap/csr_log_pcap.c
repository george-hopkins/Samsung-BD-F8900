/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_logtransport.h"
#include "csr_panic.h"
#include "csr_log_formats.h"
#include "csr_log_gsp.h"
#include "csr_log_version.h"
#include "csr_log_private.h"
#include "csr_log_pcap.h"
#include "csr_log_pcap_common.h"
#include "csr_usr_config.h"
#include "csr_msgconv.h"

#ifdef CSR_LOG_ENABLE

#define MEMCPY_VARARGS_STR_ITER(buf, offset, max_len, str, varargs)      \
    do {                                                    \
        CsrInt32 __length;                                  \
                                                            \
        if (!str)                                            \
        {                                                   \
            __length = 0;                                   \
            MEMCPY_ITER(buf, offset, max_len, __length);    \
        }                                                   \
        else                                                \
        {                                                   \
            __length = CsrVsnprintf((CsrCharString *) ((buf) + (*offset) + sizeof(CsrInt32)), CSR_LOG_TEXT_MAX_STRING_LEN + 1, (CsrCharString *) str, varargs);  \
            if ((__length >= 0) && (__length < CSR_LOG_TEXT_MAX_STRING_LEN + 1)) \
            {                                                   \
            }                                                   \
            else                                                \
            {                                                   \
                __length = CSR_LOG_TEXT_MAX_STRING_LEN;         \
            }                                                   \
            MEMCPY_ITER(buf, offset, max_len, __length);        \
        }                                                       \
        *(offset) += __length;                                  \
        CsrStrCpy((CsrCharString *) ((buf) + *(offset)), "");            \
        *(offset) += 1;                                         \
        CSR_LOG_ASSERT(*(offset) <= max_len);                   \
    } while (0)


/****************************************************************************
 * Synergy format
 ***************************************************************************/

static void csrLogPcapSerializePlatformInformation(CsrUint8 *buf, CsrSize *offset, CsrSize max_len)
{
    CsrUint16 endianProbe = 1;
    CsrBool littleEndian;
    CsrUint8 pointerSize;
    CsrUint8 alignment;
    CsrUint8 type = HCI_H4_TYPE_SYNERGY_V3;
    const CsrCharString *ver = CSR_LOG_VERSION;

    littleEndian = *((CsrUint8 *) &endianProbe) != 0 ? TRUE : FALSE;
    pointerSize = sizeof(void *);
    alignment = 4;

    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_STR_ITER(buf, offset, max_len, ver);
    MEMCPY_ITER(buf, offset, max_len, littleEndian);
    MEMCPY_ITER(buf, offset, max_len, pointerSize);
    MEMCPY_ITER(buf, offset, max_len, alignment);
}

static CsrSize csrLogPcapSizeofPlatformInformation(void)
{
    return sizeof(CsrUint8) +
           CsrLogPcapStrLen(CSR_LOG_VERSION) + 1 +
           sizeof(CsrBool) +
           sizeof(CsrUint8) +
           sizeof(CsrUint8);
}

static void csrLogPcapSerializeTechInformation(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    const CsrLogTechInformation *techInfo)
{
    CsrUint32 primitiveInfoIdx;
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_TECHINFO;

    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_STR_ITER(buf, offset, max_len, techInfo->techVer);
    MEMCPY_ITER(buf, offset, max_len, techInfo->primitiveInfoCount);

    for (primitiveInfoIdx = 0; primitiveInfoIdx < techInfo->primitiveInfoCount; ++primitiveInfoIdx)
    {
        MEMCPY_ITER(buf, offset, max_len, techInfo->primitiveInfo[primitiveInfoIdx].primitiveType);
        MEMCPY_STR_ITER(buf, offset, max_len, techInfo->primitiveInfo[primitiveInfoIdx].primitiveName);
    }
}

static CsrSize csrLogPcapSizeofTechInformation(const CsrLogTechInformation *techInfo)
{
    CsrSize size = 0;
    CsrUint32 primitiveInfoIdx;

    size += sizeof(CsrUint8) +
            CsrLogPcapStrLen(techInfo->techVer) + 1 +
            sizeof(techInfo->primitiveInfoCount);

    for (primitiveInfoIdx = 0; primitiveInfoIdx < techInfo->primitiveInfoCount; ++primitiveInfoIdx)
    {
        size += sizeof(techInfo->primitiveInfo[primitiveInfoIdx].primitiveType) +
                CsrLogPcapStrLen(techInfo->primitiveInfo[primitiveInfoIdx].primitiveName) + 1;
    }

    return size;
}

static CsrSize csrLogPcapSizeofSynergyExceptionStateEvent(const CsrCharString *file)
{
    SynergyExceptionStateEvent *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->prim_type) +
           sizeof(msg->msg_type) +
           sizeof(msg->state) +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1;
}

static void csrLogPcapSerializeSynergyExceptionStateEvent(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint16 prim_type,
    CsrUint16 msg_type,
    CsrUint16 state,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 type = SYNERGY_EXCEPTION_TYPE_STATE_EVENT;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, prim_type);
    MEMCPY_ITER(buf, offset, max_len, msg_type);
    MEMCPY_ITER(buf, offset, max_len, state);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
}

static CsrSize csrLogPcapSizeofSynergyExceptionGeneral(const CsrCharString *text, const CsrCharString *file)
{
    SynergyExceptionGeneral *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->prim_type) +
           sizeof(msg->state) +
           CsrLogPcapStrLen(text) + 1 +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1;
}

static void csrLogPcapSerializeSynergyExceptionGeneral(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint16 prim_type,
    CsrUint16 state,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 type = SYNERGY_EXCEPTION_TYPE_GENERAL;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, prim_type);
    MEMCPY_ITER(buf, offset, max_len, state);
    MEMCPY_STR_ITER(buf, offset, max_len, text);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
}

static CsrSize csrLogPcapSizeofSynergyExceptionWarning(const CsrCharString *text, const CsrCharString *file)
{
    SynergyExceptionWarning *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->prim_type) +
           sizeof(msg->state) +
           CsrLogPcapStrLen(text) + 1 +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1;
}

static void csrLogPcapSerializeSynergyExceptionWarning(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint16 prim_type,
    CsrUint16 state,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 type = SYNERGY_EXCEPTION_TYPE_WARNING;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, prim_type);
    MEMCPY_ITER(buf, offset, max_len, state);
    MEMCPY_STR_ITER(buf, offset, max_len, text);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
}

/****************************************************************************
 * Scheduler format
 ***************************************************************************/
static void csrLogPcapSerializeSynergySchedulerInit(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 thread_id)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_INIT;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, thread_id);
}

static CsrSize csrLogPcapSizeofSynergySchedulerInit(void)
{
    SynergySchedulerInit *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->thread_id);
}

static void csrLogPcapSerializeSynergySchedulerDeinit(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 thread_id)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_DEINIT;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, thread_id);
}

static CsrSize csrLogPcapSizeofSynergySchedulerDeinit(void)
{
    SynergySchedulerDeinit *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->thread_id);
}

static void csrLogPcapSerializeSynergySchedulerStart(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 thread_id)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_START;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, thread_id);
}

static CsrSize csrLogPcapSizeofSynergySchedulerStart(void)
{
    SynergySchedulerStart *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->thread_id);
}

static void csrLogPcapSerializeSynergySchedulerStop(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 thread_id)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_STOP;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, thread_id);
}

static CsrSize csrLogPcapSizeofSynergySchedulerStop(void)
{
    SynergySchedulerStop *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->thread_id);
}

static void csrLogPcapSerializeSynergySchedulerCommonTask(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 type,
    CsrSchedQid task_id)
{
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, task_id);
}

static CsrSize csrLogPcapSizeofSynergySchedulerCommonTask(void)
{
    SynergySchedulerCommonTask *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->task_id);
}

static void csrLogPcapSerializeSynergySchedulerInitTask(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 thread_id,
    CsrSchedQid task_id,
    const CsrCharString *tskName)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_INIT_TASK;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, thread_id);
    MEMCPY_ITER(buf, offset, max_len, task_id);
    MEMCPY_STR_ITER(buf, offset, max_len, tskName);
}

static CsrSize csrLogPcapSizeofSynergySchedulerInitTask(const CsrCharString *tskName)
{
    SynergySchedulerInitTask *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->thread_id) +
           sizeof(msg->task_id) +
           CsrLogPcapStrLen(tskName) + 1;
}

static void csrLogPcapSerializeSynergySchedulerDeinitTask(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedQid task_id)
{
    csrLogPcapSerializeSynergySchedulerCommonTask(buf, offset, max_len, SYNERGY_SCHEDULER_TYPE_DEINIT_TASK, task_id);
}

static CsrSize csrLogPcapSizeofSynergySchedulerDeinitTask(void)
{
    return csrLogPcapSizeofSynergySchedulerCommonTask();
}

static void csrLogPcapSerializeSynergySchedulerActivateTask(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedQid task_id)
{
    csrLogPcapSerializeSynergySchedulerCommonTask(buf, offset, max_len, SYNERGY_SCHEDULER_TYPE_ACTIVATE_TASK, task_id);
}

static void csrLogPcapSerializeSynergySchedulerDeactivateTask(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedQid task_id)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_DEACTIVATE_TASK;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, task_id);
}

static CsrSize csrLogPcapSizeofSynergySchedulerDeactivateTask(void)
{
    SynergySchedulerDeactivateTask *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->task_id);
}

static void csrLogPcapSerializeSynergySchedulerPutMessage(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid src_task_id,
    CsrSchedQid dst_task_id,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_PUT_MESSAGE;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
    MEMCPY_ITER(buf, offset, max_len, src_task_id);
    MEMCPY_ITER(buf, offset, max_len, dst_task_id);
    MEMCPY_ITER(buf, offset, max_len, msg_id);
    MEMCPY_ITER(buf, offset, max_len, prim_type);
}

static CsrSize csrLogPcapSizeofSynergySchedulerPutMessage(const CsrCharString *file, CsrSize msg_length)
{
    SynergySchedulerPutMessage *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1 +
           sizeof(msg->src_task_id) +
           sizeof(msg->dst_task_id) +
           sizeof(msg->msg_id) +
           sizeof(msg->prim_type) +
           sizeof(msg->serializer_type) +
           /* sizeof(msg->msg_length) + */
           msg_length;
}

static void csrLogPcapSerializeSynergySchedulerGetMessage(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedQid src_task_id,
    CsrSchedQid dst_task_id,
    CsrBool res,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_GET_MESSAGE;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, src_task_id);
    MEMCPY_ITER(buf, offset, max_len, dst_task_id);
    MEMCPY_ITER(buf, offset, max_len, res);
    MEMCPY_ITER(buf, offset, max_len, msg_id);
    MEMCPY_ITER(buf, offset, max_len, prim_type);
}

static CsrSize csrLogPcapSizeofSynergySchedulerGetMessage(CsrSize msg_length)
{
    SynergySchedulerGetMessage *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->src_task_id) +
           sizeof(msg->dst_task_id) +
           sizeof(msg->get_res) +
           sizeof(msg->msg_id) +
           sizeof(msg->prim_type) +
           sizeof(msg->serializer_type) +
           /* sizeof(msg->msg_length) + */
           msg_length;
}

static void csrLogPcapSerializeSynergySchedulerTimedEventIn(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid task_id,
    CsrSchedTid csrTid,
    CsrTime requested_delay,
    CsrUint16 fniarg,
    const CsrUint8 *fnvarg)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_IN;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
    MEMCPY_ITER(buf, offset, max_len, task_id);
    MEMCPY_ITER(buf, offset, max_len, csrTid);
    MEMCPY_ITER(buf, offset, max_len, requested_delay);
    MEMCPY_ITER(buf, offset, max_len, fniarg);
    MEMCPY_ITER(buf, offset, max_len, fnvarg);
}

static CsrSize csrLogPcapSizeofSynergySchedulerTimedEventIn(const CsrCharString *file)
{
    SynergySchedulerTimedEventIn *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1 +
           sizeof(msg->task_id) +
           sizeof(msg->csrTid) +
           sizeof(msg->requested_delay) +
           sizeof(msg->fniarg) +
           sizeof(msg->fnvarg);
}

static void csrLogPcapSerializeSynergySchedulerTimedEventFire(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedQid task_id,
    CsrSchedTid csrTid)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_FIRE;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, task_id);
    MEMCPY_ITER(buf, offset, max_len, csrTid);
}

static CsrSize csrLogPcapSizeofSynergySchedulerTimedEventFire(void)
{
    SynergySchedulerTimedEventFire *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->task_id) +
           sizeof(msg->csrTid);
}

static void csrLogPcapSerializeSynergySchedulerTimedEventDone(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedQid task_id,
    CsrSchedTid csrTid)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_DONE;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, task_id);
    MEMCPY_ITER(buf, offset, max_len, csrTid);
}

static CsrSize csrLogPcapSizeofSynergySchedulerTimedEventDone(void)
{
    SynergySchedulerTimedEventDone *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->task_id) +
           sizeof(msg->csrTid);
}

static void csrLogPcapSerializeSynergySchedulerTimedEventCancel(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid task_id,
    CsrSchedTid csrTid,
    CsrBool cancel_res)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_TIMED_EVENT_CANCEL;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
    MEMCPY_ITER(buf, offset, max_len, task_id);
    MEMCPY_ITER(buf, offset, max_len, csrTid);
    MEMCPY_ITER(buf, offset, max_len, cancel_res);
}

static CsrSize csrLogPcapSizeofSynergySchedulerTimedEventCancel(const CsrCharString *file)
{
    SynergySchedulerTimedEventCancel *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1 +
           sizeof(msg->task_id) +
           sizeof(msg->csrTid) +
           sizeof(msg->cancel_res);
}

static CsrSize csrLogPcapSizeofSynergySchedulerBgintRegister(const CsrCharString *callback)
{
    SynergySchedulerBgintRegister *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->thread_id) +
           sizeof(msg->irq) +
           CsrLogPcapStrLen(callback) + 1 +
           sizeof(msg->ptr);
}

static void csrLogPcapSerializeSynergySchedulerBgintRegister(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 thread_id,
    CsrSchedBgint irq,
    const CsrCharString *callback,
    const void *ptr)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_BGINT_REGISTER;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, thread_id);
    MEMCPY_ITER(buf, offset, max_len, irq);
    MEMCPY_STR_ITER(buf, offset, max_len, callback);
    MEMCPY_ITER(buf, offset, max_len, ptr);
}

static CsrSize csrLogPcapSizeofSynergySchedulerBgintSetUnregisterCommon(void)
{
    SynergySchedulerBgintSetUnregisterCommon *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->irq);
}

static void csrLogPcapSerializeSynergySchedulerBgintSetUnregisterCommon(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 type,
    CsrSchedBgint irq)
{
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, irq);
}

static CsrSize csrLogPcapSizeofSynergySchedulerBgintSet(void)
{
    return csrLogPcapSizeofSynergySchedulerBgintSetUnregisterCommon();
}

static void csrLogPcapSerializeSynergySchedulerBgintSet(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedBgint irq)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_BGINT_SET;
    csrLogPcapSerializeSynergySchedulerBgintSetUnregisterCommon(buf, offset, max_len, type, irq);
}

static CsrSize csrLogPcapSizeofSynergySchedulerBgintUnregister(void)
{
    return csrLogPcapSizeofSynergySchedulerBgintSetUnregisterCommon();
}

static void csrLogPcapSerializeSynergySchedulerBgintUnregister(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedBgint irq)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_BGINT_UNREGISTER;
    csrLogPcapSerializeSynergySchedulerBgintSetUnregisterCommon(buf, offset, max_len, type, irq);
}

static CsrSize csrLogPcapSizeofSynergySchedulerBgintServiceCommon(void)
{
    SynergySchedulerBgintServiceCommon *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->irq);
}

static void csrLogPcapSerializeSynergySchedulerBgintServiceCommon(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 type,
    CsrSchedBgint irq)
{
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, irq);
}

static void csrLogPcapSerializeSynergySchedulerBgintServiceStart(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedBgint irq)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_BGINT_SERVICE_START;
    csrLogPcapSerializeSynergySchedulerBgintServiceCommon(buf, offset, max_len, type, irq);
}

static void csrLogPcapSerializeSynergySchedulerBgintServiceDone(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrSchedBgint irq)
{
    CsrUint8 type = SYNERGY_SCHEDULER_TYPE_BGINT_SERVICE_DONE;
    csrLogPcapSerializeSynergySchedulerBgintServiceCommon(buf, offset, max_len, type, irq);
}

static void csrLogPcapSerializeSynergyCommonSavePopMsg(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 type,
    const void *orig_ptr,
    CsrUint16 prim_type)
{
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, orig_ptr);
    MEMCPY_ITER(buf, offset, max_len, prim_type);
}

static CsrSize csrLogPcapSizeofSynergyCommonSavePopMsg(CsrSize msg_length)
{
    SynergyCommonSavePopMsg *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->orig_ptr) +
           sizeof(msg->prim_type) +
           sizeof(msg->serializer_type) +
           msg_length;
}

static void csrLogPcapSerializeSynergySaveMsg(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    const void *orig_ptr,
    CsrUint16 prim_type)
{
    CsrUint8 type = SYNERGY_SAVE_POP_MSG_TYPE_SAVE;
    csrLogPcapSerializeSynergyCommonSavePopMsg(buf, offset, max_len, type, orig_ptr, prim_type);
}

static CsrSize csrLogPcapSizeofSynergySaveMsg(CsrSize msg_length)
{
    return csrLogPcapSizeofSynergyCommonSavePopMsg(msg_length);
}

static void csrLogPcapSerializeSynergyPopMsg(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    const void *orig_ptr,
    CsrUint16 prim_type)
{
    CsrUint8 type = SYNERGY_SAVE_POP_MSG_TYPE_POP;
    csrLogPcapSerializeSynergyCommonSavePopMsg(buf, offset, max_len, type, orig_ptr, prim_type);
}

static CsrSize csrLogPcapSizeofSynergyPopMsg(CsrSize msg_length)
{
    return csrLogPcapSizeofSynergyCommonSavePopMsg(msg_length);
}

static void csrLogPcapSerializeSynergyStateTransition(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    bitmask16_t mask,
    CsrUint32 identifier,
    const CsrCharString *fsm_name,
    CsrUint32 prev_state,
    const CsrCharString *prev_state_str,
    CsrUint32 in_event,
    const CsrCharString *in_event_str,
    CsrUint32 next_state,
    const CsrCharString *next_state_str,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 type = SYNERGY_TASK_BGINT_EXT_TYPE_STATE;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_ITER(buf, offset, max_len, mask);
    MEMCPY_ITER(buf, offset, max_len, identifier);
    MEMCPY_STR_ITER(buf, offset, max_len, fsm_name);
    MEMCPY_ITER(buf, offset, max_len, prev_state);
    MEMCPY_STR_ITER(buf, offset, max_len, prev_state_str);
    MEMCPY_ITER(buf, offset, max_len, in_event);
    MEMCPY_STR_ITER(buf, offset, max_len, in_event_str);
    MEMCPY_ITER(buf, offset, max_len, next_state);
    MEMCPY_STR_ITER(buf, offset, max_len, next_state_str);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
}

static CsrSize csrLogPcapSizeofSynergyStateTransition(const CsrCharString *fsm_name, const CsrCharString *prev_state_str, const CsrCharString *in_event_str, const CsrCharString *next_state_str, const CsrCharString *file)
{
    SynergyStateTransition *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           sizeof(msg->mask) +
           sizeof(msg->identifier) +
           CsrLogPcapStrLen(fsm_name) + 1 +
           sizeof(msg->prev_state) +
           CsrLogPcapStrLen(prev_state_str) + 1 +
           sizeof(msg->in_event) +
           CsrLogPcapStrLen(in_event_str) + 1 +
           sizeof(msg->next_state) +
           CsrLogPcapStrLen(next_state_str) + 1 +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1;
}

static void csrLogPcapSerializeSynergyTaskText(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 type = SYNERGY_TASK_BGINT_EXT_TYPE_TASK_TEXT;
    MEMCPY_ITER(buf, offset, max_len, type);
    MEMCPY_STR_ITER(buf, offset, max_len, text);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
}

static CsrSize csrLogPcapSizeofSynergyTaskText(const CsrCharString *text, const CsrCharString *file)
{
    SynergyTaskText *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->type) +
           CsrLogPcapStrLen(text) + 1 +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1;
}

static void csrLogPcapSerializeSynergyTextRegister(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrLogTextTaskId taskId,
    const CsrCharString *taskName,
    CsrUint16 subOriginsLen,
    const CsrLogSubOrigin *subOrigins)
{
    CsrUint16 i;
    CsrUint8 event = SYNERGY_TEXT_LEVEL_EVENT_REGISTER;
    MEMCPY_ITER(buf, offset, max_len, event);
    MEMCPY_ITER(buf, offset, max_len, taskId);
    MEMCPY_STR_ITER(buf, offset, max_len, taskName);
    MEMCPY_ITER(buf, offset, max_len, subOriginsLen);

    for (i = 0; i < subOriginsLen; i++)
    {
        MEMCPY_ITER(buf, offset, max_len, subOrigins[i].subOriginNumber);
        MEMCPY_STR_ITER(buf, offset, max_len, subOrigins[i].subOriginName);
    }
}

static CsrSize csrLogPcapSizeofSynergyTextRegister(const CsrCharString *taskName, CsrUint16 subOriginsLen, const CsrLogSubOrigin *subOrigins)
{
    SynergyTextRegister *msg = NULL;
    CsrUint16 i;
    CsrSize length = 0;
    CSR_UNUSED(msg);
    for (i = 0; i < subOriginsLen; i++)
    {
        length += sizeof(subOrigins[i].subOriginNumber);
        length += CsrLogPcapStrLen(subOrigins[i].subOriginName) + 1;
    }
    return sizeof(msg->event) +
           sizeof(msg->taskId) +
           CsrLogPcapStrLen(taskName) + 1 +
           sizeof(msg->subOriginsLen) +
           length;
}

static void csrLogPcapSerializeSynergyTextPrint(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrLogTextTaskId taskId,
    CsrUint16 subOrigin,
    CsrLogLevelText level,
    CsrSize bufferLength,
    const CsrUint8 *buffer,
    const CsrCharString *formatString,
    va_list varargs)
{
    CsrUint8 event = SYNERGY_TEXT_LEVEL_EVENT_PRINT;
    MEMCPY_ITER(buf, offset, max_len, event);
    MEMCPY_ITER(buf, offset, max_len, taskId);
    MEMCPY_ITER(buf, offset, max_len, subOrigin);
    MEMCPY_ITER(buf, offset, max_len, level);
    MEMCPY_VARARGS_STR_ITER(buf, offset, max_len, formatString, varargs);
    MEMCPY_PTR_ITER(buf, offset, max_len, bufferLength, buffer);
}

static CsrSize csrLogPcapSizeofSynergyTextPrint(CsrSize length)
{
    SynergyTextPrint *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->event) +
           sizeof(msg->taskId) +
           sizeof(msg->subOrigin) +
           sizeof(msg->level) +
           sizeof(msg->textLength) +
           CSR_LOG_TEXT_MAX_STRING_LEN + 1 +
           sizeof(msg->bufferLength) +
           length;
}

static void csrLogPcapSerializeSynergyBtiTrans(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 channel,
    CsrBool received,
    CsrSize payload_length,
    const CsrUint8 *payload)
{
    MEMCPY_ITER(buf, offset, max_len, channel);
    MEMCPY_ITER(buf, offset, max_len, received);
    MEMCPY_PTR_NO_LEN_ITER(buf, offset, max_len, payload_length, payload);
}

static CsrSize csrLogPcapSizeofSynergyBtiTrans(CsrSize payload_length)
{
    SynergyBtiTrans *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->channel) +
           sizeof(msg->received) +
           payload_length;
}

static void csrLogPcapSerializeHci(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 channel,
    CsrSize payload_length,
    const CsrUint8 *payload)
{
    MEMCPY_ITER(buf, offset, max_len, channel);
    MEMCPY_PTR_NO_LEN_ITER(buf, offset, max_len, payload_length, payload);
}

static CsrSize csrLogPcapSizeofHci(CsrSize payload_length)
{
    SynergyHci *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->channel) +
           payload_length;
}

static void csrLogPcapSerializeSynergyTransport(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 transport_type,
    CsrUint8 direction,
    CsrUint32 consumed_length,
    CsrUint32 payload_length,
    const CsrUint8 *payload)
{
    MEMCPY_ITER(buf, offset, max_len, transport_type);
    MEMCPY_ITER(buf, offset, max_len, direction);
    MEMCPY_ITER(buf, offset, max_len, consumed_length);
    MEMCPY_ITER(buf, offset, max_len, payload_length);
    MEMCPY_PTR_NO_LEN_ITER(buf, offset, max_len, payload_length, payload);
}

static CsrSize csrLogPcapSizeofSynergyTrans(CsrUint32 payload_length)
{
    SynergyTrans *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->transport) +
           sizeof(msg->direction) +
           sizeof(msg->consumed_length) +
           sizeof(msg->payload_length) +
           payload_length;
}

static void csrLogPcapSerializeSynergyProtocol(CsrUint8 *buf, CsrSize *offset, CsrSize max_len,
    CsrUint8 protocol,
    CsrUint8 event,
    CsrUint32 line,
    const CsrCharString *file)
{
    MEMCPY_ITER(buf, offset, max_len, protocol);
    MEMCPY_ITER(buf, offset, max_len, event);
    MEMCPY_ITER(buf, offset, max_len, line);
    MEMCPY_STR_ITER(buf, offset, max_len, file);
}

static CsrSize csrLogPcapSizeofSynergyProtocol(const CsrCharString *file)
{
    SynergyProtocol *msg = NULL;
    CSR_UNUSED(msg);
    return sizeof(msg->protocol) +
           sizeof(msg->event) +
           sizeof(msg->line) +
           CsrLogPcapStrLen(file) + 1;
}

/****************************************************************************
 * Log scheduler startup
 ***************************************************************************/
static void csrLogSchedStart(void *ltHdl, CsrUint8 thread_id)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerStart();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerStart(buf, &offset, length, thread_id);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log scheduler shutdown
 ***************************************************************************/
static void csrLogSchedStop(void *ltHdl, CsrUint8 thread_id)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerStop();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerStop(buf, &offset, length, thread_id);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Initialize log core
 ***************************************************************************/
static void csrLogSchedInit(void *ltHdl,
    CsrUint8 thread_id)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerInit();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerInit(buf, &offset, length, thread_id);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Deinitialize log core
 ***************************************************************************/
static void csrLogSchedDeinit(void *ltHdl,
    CsrUint8 thread_id)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerDeinit();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerDeinit(buf, &offset, length, thread_id);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log task initialisation
 ***************************************************************************/
static void csrLogInitTask(void *ltHdl,
    CsrUint8 thread_id,
    CsrSchedQid task_id,
    const CsrCharString *tskName)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerInitTask(tskName);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerInitTask(buf, &offset, length, thread_id, task_id, tskName);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * Log task deinitialisation
 ***************************************************************************/
static void csrLogDeinitTask(void *ltHdl,
    CsrSchedQid task_id)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerDeinitTask();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerDeinitTask(buf, &offset, length, task_id);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * Log activation of log core
 ***************************************************************************/
static void csrLogActivate(void *ltHdl,
    CsrSchedQid task_id)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerCommonTask();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerActivateTask(buf, &offset, length, task_id);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log deactivation of log core
 ***************************************************************************/
static void csrLogDeactivate(void *ltHdl,
    CsrSchedQid task_id)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerDeactivateTask();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerDeactivateTask(buf, &offset, length, task_id);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler put_message
 ***************************************************************************/
static void csrLogPutMessage(void *ltHdl,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid src_task_id,
    CsrSchedQid dst_task_id,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type,
    const void *msg,
    CsrBool onlyPrimType,
    CsrBool limitPrim)
{
    CsrUint8 *buf;
    CsrSize actualOffset;
    CsrSize offset = 20;
    CsrSize recOffset = 0;
    CsrMsgConvMsgEntry *msgEntry;
    CsrSize msg_length;
    CsrSize length;

    msgEntry = CsrMsgConvFindEntryByMsg(prim_type, msg);

    msg_length = CsrLogPcapSizeofPrim(msgEntry, onlyPrimType, msg);

    length = CsrLogPcapSizeofRecHdr() +
             CsrLogPcapSizeofSynergyVerHeader() +
             csrLogPcapSizeofSynergySchedulerPutMessage(file, msg_length);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerPutMessage(buf, &offset, length, line, file, src_task_id, dst_task_id, msg_id, prim_type);
    CsrLogPcapSerializePrim(buf, &offset, length, msgEntry, onlyPrimType, limitPrim, msg, &actualOffset);

    /* Write out the length of the actual packet. This is a needed
     * workaround because the prim serialiser doesn't respect max len */
    offset = CSRMIN(offset, PCAP_MAX_REC_SIZE);
    actualOffset = CSRMIN(actualOffset, PCAP_MAX_REC_SIZE);
    CsrLogPcapSerializeRecHdrNoDirection(buf, &recOffset, length, offset, actualOffset);

    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler get_message
 ***************************************************************************/
static void csrLogGetMessage(void *ltHdl,
    CsrSchedQid src_task_id,
    CsrSchedQid dst_task_id,
    CsrBool get_res,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type,
    const void *msg,
    CsrBool onlyPrimType,
    CsrBool limitPrim)
{
    CsrUint8 *buf;
    CsrSize actualOffset;
    CsrSize offset = 20;
    CsrSize recOffset = 0;
    CsrMsgConvMsgEntry *msgEntry;
    CsrSize msg_length;
    CsrSize length;

    msgEntry = CsrMsgConvFindEntryByMsg(prim_type, msg);

    msg_length = CsrLogPcapSizeofPrim(msgEntry, onlyPrimType, msg);

    length = CsrLogPcapSizeofRecHdr() +
             CsrLogPcapSizeofSynergyVerHeader() +
             csrLogPcapSizeofSynergySchedulerGetMessage(msg_length);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerGetMessage(buf, &offset, length, src_task_id, dst_task_id, get_res, msg_id, prim_type);
    CsrLogPcapSerializePrim(buf, &offset, length, msgEntry, onlyPrimType, limitPrim, msg, &actualOffset);

    /* Write out the length of the actual packet. This is a needed
     * workaround because the prim serialiser doesn't respect max len */
    offset = CSRMIN(offset, PCAP_MAX_REC_SIZE);
    actualOffset = CSRMIN(actualOffset, PCAP_MAX_REC_SIZE);
    CsrLogPcapSerializeRecHdrNoDirection(buf, &recOffset, length, offset, actualOffset);

    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler timed_event_in
 ***************************************************************************/
static void csrLogTimedEventIn(void *ltHdl,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid task_id,
    CsrSchedTid csrTid,
    CsrTime requested_delay,
    CsrUint16 fniarg,
    const CsrUint8 *fnvarg)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerTimedEventIn(file);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerTimedEventIn(buf, &offset, length, line, file, task_id, csrTid, requested_delay, fniarg, fnvarg);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler timed event fire/run
 ***************************************************************************/
static void csrLogTimedEventFire(void *ltHdl,
    CsrSchedQid task_id,
    CsrSchedTid csrTid)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerTimedEventFire();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerTimedEventFire(buf, &offset, length, task_id, csrTid);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler timed event done
 ***************************************************************************/
static void csrLogTimedEventDone(void *ltHdl,
    CsrSchedQid task_id,
    CsrSchedTid csrTid)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerTimedEventDone();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerTimedEventDone(buf, &offset, length, task_id, csrTid);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler cancel_timed_event
 ***************************************************************************/
static void csrLogTimedEventCancel(void *ltHdl,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid task_id,
    CsrSchedTid csrTid,
    CsrBool cancel_res)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerTimedEventCancel(file);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerTimedEventCancel(buf, &offset, length, line, file, task_id, csrTid, cancel_res);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler bgint register
 ***************************************************************************/
static void csrLogBgIntRegister(void *ltHdl,
    CsrUint8 thread_id,
    CsrSchedBgint irq,
    const CsrCharString *callback,
    const void *ptr)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerBgintRegister(callback);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerBgintRegister(buf, &offset, length, thread_id, irq, callback, ptr);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * Log a scheduler bgint unregister
 ***************************************************************************/
static void csrLogBgIntUnregister(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerBgintUnregister();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerBgintUnregister(buf, &offset, length, irq);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * Log a scheduler bgint set
 ***************************************************************************/
static void csrLogBgIntSet(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerBgintSet();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerBgintSet(buf, &offset, length, irq);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler bgint service start
 ***************************************************************************/
static void csrLogBgIntServiceStart(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerBgintServiceCommon();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerBgintServiceStart(buf, &offset, length, irq);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a scheduler bgint service done
 ***************************************************************************/
static void csrLogBgIntServiceDone(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergySchedulerBgintServiceCommon();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySchedulerBgintServiceDone(buf, &offset, length, irq);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a save message
 ***************************************************************************/
static void csrLogSaveMsg(void *ltHdl,
    CsrUint16 prim_type,
    const void *msg,
    CsrBool onlyPrimType,
    CsrBool limitPrim)
{
    CsrUint8 *buf;
    CsrSize actualOffset;
    CsrSize offset = 20;
    CsrSize recOffset = 0;
    CsrMsgConvMsgEntry *msgEntry;
    CsrSize msg_length;
    CsrSize length;

    msgEntry = CsrMsgConvFindEntryByMsg(prim_type, msg);

    msg_length = CsrLogPcapSizeofPrim(msgEntry, onlyPrimType, msg);

    length = CsrLogPcapSizeofRecHdr() +
             CsrLogPcapSizeofSynergyVerHeader() +
             csrLogPcapSizeofSynergySaveMsg(msg_length);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergySaveMsg(buf, &offset, length, msg, prim_type);
    CsrLogPcapSerializePrim(buf, &offset, length, msgEntry, onlyPrimType, limitPrim, msg, &actualOffset);

    /* Write out the length of the actual packet. This is a needed
     * workaround because the prim serialiser doesn't respect max len */
    offset = CSRMIN(offset, PCAP_MAX_REC_SIZE);
    actualOffset = CSRMIN(actualOffset, PCAP_MAX_REC_SIZE);
    CsrLogPcapSerializeRecHdrNoDirection(buf, &recOffset, length, offset, actualOffset);

    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a pop message
 ***************************************************************************/
static void csrLogPopMsg(void *ltHdl,
    CsrUint16 prim_type,
    const void *msg,
    CsrBool onlyPrimType,
    CsrBool limitPrim)
{
    CsrUint8 *buf;
    CsrSize actualOffset;
    CsrSize offset = 20;
    CsrSize recOffset = 0;
    CsrMsgConvMsgEntry *msgEntry;
    CsrSize msg_length;
    CsrSize length;

    msgEntry = CsrMsgConvFindEntryByMsg(prim_type, msg);

    msg_length = CsrLogPcapSizeofPrim(msgEntry, onlyPrimType, msg);

    length = CsrLogPcapSizeofRecHdr() +
             CsrLogPcapSizeofSynergyVerHeader() +
             csrLogPcapSizeofSynergyPopMsg(msg_length);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyPopMsg(buf, &offset, length, msg, prim_type);
    CsrLogPcapSerializePrim(buf, &offset, length, msgEntry, onlyPrimType, limitPrim, msg, &actualOffset);

    /* Write out the length of the actual packet. This is a needed
     * workaround because the prim serialiser doesn't respect max len */
    offset = CSRMIN(offset, PCAP_MAX_REC_SIZE);
    actualOffset = CSRMIN(actualOffset, PCAP_MAX_REC_SIZE);
    CsrLogPcapSerializeRecHdrNoDirection(buf, &recOffset, length, offset, actualOffset);

    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log state event exception
 ***************************************************************************/
static void csrLogExceptionStateEvent(void *ltHdl,
    CsrUint16 prim_type,
    CsrPrim msg_type,
    CsrUint16 state,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyExceptionStateEvent(file);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyExceptionStateEvent(buf, &offset, length, prim_type, msg_type, state, line, file);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log general exception
 ***************************************************************************/
static void csrLogExceptionGeneral(void *ltHdl,
    CsrUint16 prim_type,
    CsrUint16 state,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyExceptionGeneral(text, file);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyExceptionGeneral(buf, &offset, length, prim_type, state, text, line, file);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log warning exception
 ***************************************************************************/
static void csrLogExceptionWarning(void *ltHdl,
    CsrUint16 prim_type,
    CsrUint16 state,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyExceptionWarning(text, file);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyExceptionWarning(buf, &offset, length, prim_type, state, text, line, file);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

static CsrUint8 csrLogPcapConvertChannel(CsrUint8 channel, CsrBool received)
{
    switch (channel)
    {
        case SYNERGY_BTI_TRANS_TYPE_HCI:
            if (received)
            {
                channel = 0x04;
            }
            else
            {
                channel = 0x01;
            }
            break;

        case SYNERGY_BTI_TRANS_TYPE_ACL:
            channel = 0x02;
            break;

        case SYNERGY_BTI_TRANS_TYPE_SCO:
            channel = 0x03;
            break;

        default:
            break;
    }

    return channel;
}

/****************************************************************************
 * Log a BlueCore transport channel message
 ***************************************************************************/
static void csrLogBlueCoreTransport(void *ltHdl,
    CsrUint8 channel,
    CsrBool received,
    CsrSize payload_length,
    const void *payload)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length;
    CsrBool pureHci = channel == SYNERGY_BTI_TRANS_TYPE_ACL ||
                      channel == SYNERGY_BTI_TRANS_TYPE_HCI ||
                      channel == SYNERGY_BTI_TRANS_TYPE_SCO;

    if (pureHci)
    {
        length = CsrLogPcapSizeofRecHdr() +
                 csrLogPcapSizeofHci(payload_length);

        CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

        channel = csrLogPcapConvertChannel(channel, received);
        CsrLogPcapSerializeRecHdr(buf, &offset, (CsrUint32) length, (CsrUint32) length, (CsrUint32) length, received);
        csrLogPcapSerializeHci(buf, &offset, length, channel, payload_length, payload);
    }
    else
    {
        length = CsrLogPcapSizeofRecHdr() +
                 CsrLogPcapSizeofSynergyVerHeader() +
                 csrLogPcapSizeofSynergyBtiTrans(payload_length);

        CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

        CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
        CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
        csrLogPcapSerializeSynergyBtiTrans(buf, &offset, length, channel, received, payload_length, payload);
    }

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a BCSP transport message
 ***************************************************************************/
static void csrLogTransport(void *ltHdl,
    CsrUint8 transport_type,
    CsrUint8 direction,
    CsrUint32 consumed_length,
    CsrUint32 payload_length,
    const void *payload)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyTrans(payload_length);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyTransport(buf, &offset, length, transport_type, direction, consumed_length, payload_length, payload);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a text message
 ***************************************************************************/
static void csrLogTaskText(void *ltHdl,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyTaskText(text, file);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyTaskText(buf, &offset, length, text, line, file);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a Log Text subOrigin registration
 ***************************************************************************/
static void csrLogTextRegister(void *ltHdl,
    CsrLogTextTaskId taskId,
    const CsrCharString *taskName,
    CsrUint16 subOriginsLen,
    const CsrLogSubOrigin *subOrigins)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyTextRegister(taskName, subOriginsLen, subOrigins);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyTextRegister(buf, &offset, length, taskId, taskName, subOriginsLen, subOrigins);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * Log a buffer message
 ***************************************************************************/
static void csrLogBufferTextPrint(void *ltHdl,
    CsrLogTextTaskId taskId,
    CsrUint16 subOrigin,
    CsrLogLevelText level,
    CsrSize bufferLength,
    const void *buffer,
    const CsrCharString *formatString,
    va_list varargs)
{
    CsrUint8 *buf;
    CsrSize offset = 20;
    CsrSize realLength = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyTextPrint(bufferLength);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyTextPrint(buf, &offset, length, taskId, subOrigin, level, bufferLength, buffer, formatString, varargs);
    realLength = offset;
    offset = 0;
    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, realLength, realLength);

    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, realLength);
}

/****************************************************************************
 * Log a text message
 ***************************************************************************/
static void csrLogTextPrint(void *ltHdl,
    CsrLogTextTaskId taskId,
    CsrUint16 subOrigin,
    CsrLogLevelText level,
    const CsrCharString *formatString,
    va_list varargs)
{
    csrLogBufferTextPrint(ltHdl, taskId, subOrigin, level, 0, NULL, formatString, varargs);
}

/****************************************************************************
 * Log a transport protocol state event messages (BCSP/H4/H4DS)
 ***************************************************************************/
static void csrLogProtocol(void *ltHdl,
    CsrUint8 protocol,
    CsrUint8 event,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofSynergyProtocol(file);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeSynergyProtocol(buf, &offset, length, protocol, event, line, file);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log state transitions
 ***************************************************************************/
static void csrLogStateTransition(void *ltHdl,
    bitmask16_t mask,
    CsrUint32 identifier,
    const CsrCharString *fsm_name,
    CsrUint32 prev_state,
    const CsrCharString *prev_state_str,
    CsrUint32 in_event,
    const CsrCharString *in_event_str,
    CsrUint32 next_state,
    const CsrCharString *next_state_str,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length1 = CsrLogPcapSizeofRecHdr();
    CsrSize length2 = CsrLogPcapSizeofSynergyVerHeader();
    CsrSize length3 = csrLogPcapSizeofSynergyStateTransition(fsm_name, prev_state_str, in_event_str, next_state_str, file);
    CsrSize length = length1 + length2 + length3;

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CSR_LOG_ASSERT(offset == length1);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    CSR_LOG_ASSERT(offset == length1 + length2);
    CSR_LOG_ASSERT(offset == length1 + length2);
    csrLogPcapSerializeSynergyStateTransition(buf, &offset, length, mask, identifier, fsm_name, prev_state, prev_state_str, in_event, in_event_str, next_state, next_state_str, line, file);
    CSR_LOG_ASSERT(offset == length1 + length2 + length3);

    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

/****************************************************************************
 * Log a technology register message
 ***************************************************************************/
static void csrLogTechInfo(void *ltHdl,
    const CsrLogTechInformation *techInfo)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     CsrLogPcapSizeofSynergyVerHeader() +
                     csrLogPcapSizeofTechInformation(techInfo);

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    CsrLogPcapSerializeSynergyVerHeader(buf, &offset, length);
    csrLogPcapSerializeTechInformation(buf, &offset, length, techInfo);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * Log a platform register message
 ***************************************************************************/
static void csrLogPlatformInfo(void *ltHdl)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofRecHdr() +
                     csrLogPcapSizeofPlatformInformation();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeRecHdrNoDirection(buf, &offset, length, length, length);
    csrLogPcapSerializePlatformInformation(buf, &offset, length);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

/****************************************************************************
 * Log PCAP header
 ***************************************************************************/
static void csrLogPcapHdr(void *ltHdl)
{
    CsrUint8 *buf;
    CsrSize offset = 0;
    CsrSize length = CsrLogPcapSizeofHdr();

    CSR_LOG_PCAP_TRANSPORT_ALLOC(buf, ltHdl, length);

    CsrLogPcapSerializeHdr(buf, &offset, length);

    CSR_LOG_ASSERT(offset == length);
    CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
}

CsrLog *CsrLogPcapCreate(void *logHandle)
{
    CsrLog *l;

    l = CsrPmemZalloc(sizeof(*l));

    l->lregtech = csrLogTechInfo;
    l->lbci = csrLogBlueCoreTransport;
    l->ltrans = csrLogTransport;
    l->ltasktext = csrLogTaskText;
    l->ltextregister = csrLogTextRegister;
    l->ltextprint = csrLogTextPrint;
    l->ltextbufprint = csrLogBufferTextPrint;
    l->lproto = csrLogProtocol;
    l->lexcpstate = csrLogExceptionStateEvent;
    l->lexcpgen = csrLogExceptionGeneral;
    l->lexcpwarn = csrLogExceptionWarning;
    l->lstatetrans = csrLogStateTransition;
    l->lschedinit = csrLogSchedInit;
    l->lscheddeinit = csrLogSchedDeinit;
    l->lschedstart = csrLogSchedStart;
    l->lschedstop = csrLogSchedStop;
    l->linittask = csrLogInitTask;
    l->ldeinittask = csrLogDeinitTask;
    l->lactivate = csrLogActivate;
    l->ldeactivate = csrLogDeactivate;
    l->lputmsg = csrLogPutMessage;
    l->lgetmsg = csrLogGetMessage;
    l->ltimedeventin = csrLogTimedEventIn;
    l->ltimedeventfire = csrLogTimedEventFire;
    l->ltimedeventdone = csrLogTimedEventDone;
    l->ltimedeventcancel = csrLogTimedEventCancel;
    l->bgintreg = csrLogBgIntRegister;
    l->bgintunreg = csrLogBgIntUnregister;
    l->bgintset = csrLogBgIntSet;
    l->bgintservicestart = csrLogBgIntServiceStart;
    l->bgintservicedone = csrLogBgIntServiceDone;
    l->lsave = csrLogSaveMsg;
    l->lpop = csrLogPopMsg;
    l->ltHdl = logHandle;

    csrLogPcapHdr(logHandle);
    csrLogPlatformInfo(logHandle);

    return l;
}

void CsrLogPcapDestroy(CsrLog *l)
{
    CsrPmemFree(l);
}

#endif /* CSR_LOG_ENABLE */
