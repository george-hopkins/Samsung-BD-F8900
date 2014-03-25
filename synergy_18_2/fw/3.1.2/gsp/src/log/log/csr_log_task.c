/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_sched.h"
#include "csr_log_private.h"
#include "csr_log_formats.h"
#include "csr_log_gsp.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_msgconv.h"

#ifdef CSR_LOG_ENABLE

CsrLogLevelTask CsrLogTaskFilterGet(CsrSchedQid taskId)
{
    CsrLogLevelFilterTask *task;

    task = CsrLogCtx.logLevelFilterTask.tasks;
    while (task)
    {
        if (task->taskId == taskId)
        {
            return task->taskLogLevel;
        }
        task = task->next;
    }

    return CsrLogCtx.logLevelFilterTask.allTaskLogLevel;
}

CsrBool CsrLogTaskIsFiltered(CsrSchedQid taskId, CsrLogLevelTask level)
{
    CsrBool ret = FALSE;

    if (CsrLogCtx.logLevelFilterTask.tasks)
    {
        CsrBool taskFound = FALSE;
        CsrLogLevelFilterTask *task = CsrLogCtx.logLevelFilterTask.tasks;
        while (task && !taskFound)
        {
            if (task->taskId == taskId)
            {
                taskFound = TRUE;

                if (!(task->taskLogLevel & level))
                {
                    ret = TRUE;
                }
            }
            else
            {
                task = task->next;
            }
        }

        if (!taskFound) /* No special filter has been applied to this taskId so just match it agains the overall warningLevel */
        {
            if (!(CsrLogCtx.logLevelFilterTask.allTaskLogLevel & level))
            {
                ret = TRUE;
            }
        }
    }
    else /* No special filter has been applied to this taskId so just match it agains the overall warningLevel */
    {
        if (!(CsrLogCtx.logLevelFilterTask.allTaskLogLevel & level))
        {
            ret = TRUE;
        }
    }

    return ret;
}

void CsrLogTaskText(const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrLog *p;

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ltasktext != NULL)
        {
            if (!CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(),
                    CSR_LOG_LEVEL_TASK_TEXT_LOC))
            {
                p->ltasktext(p->ltHdl, text, line, file);
            }
            else if (!CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(),
                         CSR_LOG_LEVEL_TASK_TEXT))
            {
                p->ltasktext(p->ltHdl, text, 0, NULL);
            }
        }
    }
}

void CsrLogExceptionStateEvent(CsrUint16 prim_type,
    CsrPrim msg_type,
    CsrUint16 state,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrLog *p;

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lexcpstate != NULL)
        {
            p->lexcpstate(p->ltHdl, prim_type, msg_type, state, line, file);
        }
    }
}

void CsrLogExceptionGeneral(CsrUint16 prim_type,
    CsrUint16 state,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrLog *p;

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lexcpgen != NULL)
        {
            p->lexcpgen(p->ltHdl, prim_type, state, text, line, file);
        }
    }
}

void CsrLogExceptionWarning(CsrUint16 prim_type,
    CsrUint16 state,
    const CsrCharString *text,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrLog *p;

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lexcpwarn != NULL)
        {
            p->lexcpwarn(p->ltHdl, prim_type, state, text, line, file);
        }
    }
}

void CsrLogStateTransition(bitmask16_t mask,
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
    CsrLog *p;

    if (CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_STATE))
    {
        return;
    }

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lstatetrans != NULL)
        {
            if (!CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(),
                    CSR_LOG_LEVEL_TASK_STATE_NAME))
            {
                if (!CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(),
                        CSR_LOG_LEVEL_TASK_STATE_LOC))
                {
                    p->lstatetrans(p->ltHdl, mask,
                        identifier, fsm_name,
                        prev_state, prev_state_str,
                        in_event, in_event_str,
                        next_state, next_state_str,
                        line, file);
                }
                else if (!CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(),
                             CSR_LOG_LEVEL_TASK_STATE))
                {
                    p->lstatetrans(p->ltHdl, mask,
                        identifier, fsm_name,
                        prev_state, prev_state_str,
                        in_event, in_event_str,
                        next_state, next_state_str,
                        0, NULL);
                }
            }
            else
            {
                if (!CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(),
                        CSR_LOG_LEVEL_TASK_STATE_LOC))
                {
                    p->lstatetrans(p->ltHdl, mask,
                        identifier, NULL,
                        prev_state, NULL,
                        in_event, NULL,
                        next_state, NULL,
                        line, file);
                }
                else if (!CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(),
                             CSR_LOG_LEVEL_TASK_STATE))
                {
                    p->lstatetrans(p->ltHdl, mask,
                        identifier, NULL,
                        prev_state, NULL,
                        in_event, NULL,
                        next_state, NULL,
                        0, NULL);
                }
            }
        }
    }
}

void CsrLogActivate(CsrSchedQid tskid)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lactivate != NULL)
        {
            p->lactivate(p->ltHdl, tskid);
        }
    }
}

void CsrLogDeactivate(CsrSchedQid tskid)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ldeactivate != NULL)
        {
            p->ldeactivate(p->ltHdl, tskid);
        }
    }
}

void CsrLogMessagePut(CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid src_task_id,
    CsrSchedQid dst_task_id,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type,
    const void *msg)
{
    CsrLog *p;
    CsrBool onlyPrimType = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_ONLY_TYPE);
    CsrBool limitPrim = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_APPLY_LIMIT);

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lputmsg != NULL)
        {
            if (!CsrLogTaskIsFiltered(src_task_id,
                    CSR_LOG_LEVEL_TASK_MESSAGE_PUT_LOC))
            {
                p->lputmsg(p->ltHdl,
                    line, file,
                    src_task_id,
                    dst_task_id,
                    msg_id,
                    prim_type,
                    msg, onlyPrimType, limitPrim);
            }
            else if (!CsrLogTaskIsFiltered(src_task_id,
                         CSR_LOG_LEVEL_TASK_MESSAGE_PUT))
            {
                p->lputmsg(p->ltHdl,
                    0, NULL,
                    src_task_id,
                    dst_task_id,
                    msg_id,
                    prim_type,
                    msg, onlyPrimType, limitPrim);
            }
        }
    }
}

void CsrLogMessageGet(CsrSchedQid src_task_id,
    CsrSchedQid dst_task_id,
    CsrBool get_res,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type,
    const void *msg)
{
    CsrLog *p;
    CsrBool onlyPrimType = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_ONLY_TYPE);
    CsrBool limitPrim = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_APPLY_LIMIT);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lgetmsg != NULL)
        {
            p->lgetmsg(p->ltHdl,
                src_task_id,
                dst_task_id,
                get_res, msg_id,
                prim_type,
                msg, onlyPrimType, limitPrim);
        }
    }
}

void CsrLogMessageQueuePush(CsrUint16 prim_type,
    const void *ptr)
{
    CsrLog *p;
    CsrBool onlyPrimType = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_ONLY_TYPE);
    CsrBool limitPrim = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_APPLY_LIMIT);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lsave != NULL)
        {
            p->lsave(p->ltHdl,
                prim_type,
                ptr, onlyPrimType, limitPrim);
        }
    }
}

void CsrLogMessageQueuePop(CsrUint16 prim_type,
    const void *ptr)
{
    CsrLog *p;
    CsrBool onlyPrimType = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_ONLY_TYPE);
    CsrBool limitPrim = !CsrLogTaskIsFiltered(CsrSchedTaskQueueGet(), CSR_LOG_LEVEL_TASK_PRIM_APPLY_LIMIT);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lpop != NULL)
        {
            p->lpop(p->ltHdl,
                prim_type,
                ptr, onlyPrimType, limitPrim);
        }
    }
}

void CsrLogTimedEventIn(CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid task_id,
    CsrSchedTid tid,
    CsrTime requested_delay,
    CsrUint16 fniarg,
    const void *fnvarg)
{
    CsrLog *p;

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ltimedeventin != NULL)
        {
            if (!CsrLogTaskIsFiltered(task_id,
                    CSR_LOG_LEVEL_TASK_TIMER_IN_LOC))
            {
                p->ltimedeventin(p->ltHdl,
                    line, file,
                    task_id,
                    tid, requested_delay, fniarg, fnvarg);
            }
            else if (!CsrLogTaskIsFiltered(task_id,
                         CSR_LOG_LEVEL_TASK_TIMER_IN))
            {
                p->ltimedeventin(p->ltHdl,
                    0, NULL,
                    task_id,
                    tid, requested_delay, fniarg, fnvarg);
            }
        }
    }
}

void CsrLogTimedEventFire(CsrSchedQid task_id,
    CsrSchedTid tid)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ltimedeventfire != NULL)
        {
            p->ltimedeventfire(p->ltHdl,
                task_id,
                tid);
        }
    }
}

void CsrLogTimedEventDone(CsrSchedQid task_id,
    CsrSchedTid tid)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ltimedeventdone != NULL)
        {
            p->ltimedeventdone(p->ltHdl,
                task_id,
                tid);
        }
    }
}

void CsrLogTimedEventCancel(CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid task_id,
    CsrSchedTid csrTid,
    CsrBool cancel_res)
{
    CsrLog *p;

    file = CsrGetBaseName(file);

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ltimedeventcancel != NULL)
        {
            if (!CsrLogTaskIsFiltered(task_id,
                    CSR_LOG_LEVEL_TASK_TIMER_CANCEL_LOC))
            {
                p->ltimedeventcancel(p->ltHdl,
                    line, file,
                    task_id,
                    csrTid,
                    cancel_res);
            }
            else if (!CsrLogTaskIsFiltered(task_id,
                         CSR_LOG_LEVEL_TASK_TIMER_CANCEL))
            {
                p->ltimedeventcancel(p->ltHdl,
                    0, NULL,
                    task_id,
                    csrTid,
                    cancel_res);
            }
        }
    }
}

#endif
