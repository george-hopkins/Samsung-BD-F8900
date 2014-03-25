/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_sched.h"
#include "csr_sched_private.h"
#include "csr_util.h"
#include "csr_time.h"

#include "csr_framework_ext.h"

#include "csr_usr_config.h"

#include "csr_sched_init.h"

#include "csr_log.h"

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_ips.h"
#include "csr_sched_ips.h"
#endif

#ifndef CSR_SCHED_MESSAGE_POOL_LIMIT
#define CSR_SCHED_MESSAGE_POOL_LIMIT    10
#endif

#ifndef CSR_SCHED_TIMER_POOL_LIMIT
#define CSR_SCHED_TIMER_POOL_LIMIT    10
#endif

#define QUEUE_EXTRACT_SEGMENT(queue)    (((queue) & CSR_SCHED_QUEUE_SEGMENT) >> CSR_SCHED_QUEUE_SEGMENT_SHIFT)
#define QUEUE_EXTRACT_INDEX(queue)      ((queue) & 0x07FF)

/* Scheduler instance - we need this to be global in order to gain
 * access to the instance from CsrSchedMessagePut/Get et al */
static SchedulerInstanceType *instance = NULL;
static CsrBool (*externalSend)(CsrSchedQid q, CsrUint16 mi, void *mv) = NULL;

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
static void contextSwitch(ThreadInstanceType *thread, CsrSchedIpsContext context, CsrUint32 **measurements)
{
    CsrMutexLock(&thread->ipsMutex);

    thread->currentContext = context;
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, *measurements);

    CsrMutexUnlock(&thread->ipsMutex);
}

#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrGetThreadIndex
 *
 *  DESCRIPTION
 *      Local support function. Finds the current thread's instance data
 *      index and returns this.
 *
 *  RETURNS
 *      Index of the current thread or 0xFF if not found.
 *
 *----------------------------------------------------------------------------*/
CsrUint8 CsrGetThreadIndex(void)
{
    CsrUint8 index;
    CsrThreadHandle handle;

    if ((instance != NULL) && (CsrThreadGetHandle(&handle) == CSR_RESULT_SUCCESS))
    {
        for (index = 0; index < CSR_SCHED_MAX_SEGMENTS; index++)
        {
            if (instance->thread[index].inUse &&
                (CsrThreadEqual(&instance->thread[index].thread_handle, &handle) == CSR_RESULT_SUCCESS))
            {
                return index;
            }
        }
    }
    return 0xFF;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getMsgId
 *
 *  DESCRIPTION
 *      Actually, this just returns the next value from incrementing the
 *      lower 28 bits of a CsrUint32 value, the top 4 bits are used to identify
 *      the scheduler thread. We don't care about potential wrap-arounds....
 *
 *  RETURNS
 *      CsrSchedIdentifier - An CSR_UNUSED identifier.
 *
 *----------------------------------------------------------------------------*/
#define MAX_SCHED_ID            ((CsrSchedIdentifier) 0x0FFFFFFF)
#define EXTERNAL_SCHED_ID       ((CsrSchedIdentifier) 0x80000000)

static CsrSchedIdentifier getMsgId(ThreadInstanceType *thread)
{
    thread->msgId++;
    thread->msgId = (thread->msgId) & MAX_SCHED_ID;

    return thread->msgId | ((CsrSchedIdentifier) thread->id << 28);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      messageAlloc
 *
 *  DESCRIPTION
 *      A helper function for allocating message containers which are
 *      taken from the thread-specific free list or allocated using
 *      pnew().
 *
 *  RETURNS
 *      MessageQueueEntryType *
 *
 *----------------------------------------------------------------------------*/
static MessageQueueEntryType *messageAlloc(ThreadInstanceType *thread)
{
    MessageQueueEntryType *message;

    /*
     * If there is a message on the free list, pick
     * the one in front (order doesn't matter on the
     * free list and the first is more likely to be
     * in cache), and allocate a new one otherwise.
     */
    if (thread->messageFreeList != NULL)
    {
        message = thread->messageFreeList;
        thread->messageFreeList = message->next;
        thread->messageFreeListLength--;
    }
    else
    {
        message = pnew(MessageQueueEntryType);
    }

    return message;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      messageFree
 *
 *  DESCRIPTION
 *      A helper function for deallocating message containers which
 *      are returned to the thread-specific free list or deallocated
 *      using CsrPfree().
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
static void messageFree(ThreadInstanceType *thread,
    MessageQueueEntryType *message)
{
    /*
     * If the free list is not full, return container
     * to front of list.  Otherwise deallocate using
     * CsrPmemFree().
     */
    if (thread->messageFreeListLength < CSR_SCHED_MESSAGE_POOL_LIMIT)
    {
        message->next = thread->messageFreeList;
        thread->messageFreeList = message;
        thread->messageFreeListLength++;
    }
    else
    {
        CsrPmemFree(message);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      timerAlloc
 *
 *  DESCRIPTION
 *      A helper function for allocating timer containers which are
 *      taken from the thread-specific free list or allocated using
 *      pnew().
 *
 *  RETURNS
 *      TimerType *
 *
 *----------------------------------------------------------------------------*/
static TimerType *timerAlloc(ThreadInstanceType *thread)
{
    TimerType *timer;

    /*
     * If there is a timer on the free list, pick
     * the one in front (order doesn't matter on the
     * free list and the first is more likely to be
     * in cache), and allocate a new one otherwise.
     */
    if (thread->timerFreeList != NULL)
    {
        timer = thread->timerFreeList;
        thread->timerFreeList = timer->next;
        thread->timerFreeListLength--;
    }
    else
    {
        timer = pnew(TimerType);
    }

    return timer;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      timerFree
 *
 *  DESCRIPTION
 *      A helper function for deallocating timer containers which
 *      are returned to the thread-specific free list or deallocated
 *      using CsrPfree().
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
static void timerFree(ThreadInstanceType *thread,
    TimerType *timer)
{
    /*
     * If the free list is not full, return container
     * to front of list.  Otherwise deallocate using
     * CsrPmemFree().
     */
    if (thread->timerFreeListLength < CSR_SCHED_TIMER_POOL_LIMIT)
    {
        timer->next = thread->timerFreeList;
        thread->timerFreeList = timer;
        thread->timerFreeListLength++;
    }
    else
    {
        CsrPmemFree(timer);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      do_put_message
 *
 *  DESCRIPTION
 *      Does the actual work of CsrSchedMessagePut().
 *
 *  RETURNS
 *      -
 *
 *----------------------------------------------------------------------------*/
static void do_put_message(CsrSchedQid q,
    CsrUint16 mi,
    void *mv,
    CsrUint32 line,
    const CsrCharString *file)
{
    CsrSchedQid currentTask;
    CsrUint16 qi;
    CsrUint8 src_index, dest_index;
    CsrSchedMsgId id;

    src_index = CsrGetThreadIndex();
    if (src_index < CSR_SCHED_MAX_SEGMENTS)
    {
        currentTask = instance->thread[src_index].currentTask;
        id = getMsgId(&instance->thread[src_index]);
    }
    else
    {
        currentTask = CSR_SCHED_TASK_ID;
        id = EXTERNAL_SCHED_ID;
    }

    /* Get segment and task index */
    dest_index = QUEUE_EXTRACT_SEGMENT(q);
    qi = QUEUE_EXTRACT_INDEX(q);

    if (dest_index < CSR_SCHED_MAX_SEGMENTS)
    {
        /* this message is destined for another sched task */
        MessageQueueEntryType *message;
        ThreadInstanceType *dest_thread = &instance->thread[dest_index];

        if (!dest_thread->inUse || (qi >= dest_thread->numberOfTasks))
        {
            /* Task index is out-of-bounds */
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNKNOWN_TASK, "Invalid receiver task");
        }

        /* Package the message for queue storage. */
        if (currentTask != CSR_SCHED_TASK_ID)
        {
            message = messageAlloc(&(instance->thread[src_index]));
        }
        else
        {
            message = pnew(MessageQueueEntryType);
        }
        message->next = NULL;
        message->event = mi;
        message->sender = currentTask | (src_index << CSR_SCHED_QUEUE_SEGMENT_SHIFT);
        message->message = mv;
        message->id = id;

#ifdef CSR_LOG_ENABLE
        if (!(src_index < CSR_SCHED_MAX_SEGMENTS) || (instance->thread[src_index].currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_PUT))
        {
            CsrLogMessagePut(line,
                file,
                message->sender,
                q,
                id,
                mi,
                mv);
        }
#endif

        if (src_index == dest_index) /* message to the calling thread */
        {
            /* Store the message on the end of the task's message chain. */
            if (dest_thread->tasksInfo[qi].messageQueueLast == NULL)
            {
                dest_thread->tasksInfo[qi].messageQueueFirst = message;
                dest_thread->tasksInfo[qi].messageQueueLast = message;
            }
            else
            {
                dest_thread->tasksInfo[qi].messageQueueLast->next = message;
                dest_thread->tasksInfo[qi].messageQueueLast = message;
            }
            dest_thread->pendingMessages++;
        }
        else /* message to another thread than the calling */
        {
            message->receiver = qi;
            CsrMutexLock(&dest_thread->qMutex);

            if (dest_thread->extMsgQueueLast == NULL)
            {
                dest_thread->extMsgQueueFirst = message;
                dest_thread->extMsgQueueLast = message;
            }
            else
            {
                dest_thread->extMsgQueueLast->next = message;
                dest_thread->extMsgQueueLast = message;
            }

            CsrMutexUnlock(&dest_thread->qMutex);

            CsrEventSet(&dest_thread->eventHandle, EXT_MSG_EVENT);
        }
    }
    else
    {
        /* this message is destined for an external receiver */
        if (externalSend != NULL)
        {
            /* Message flagged for remote scheduler/thread thingy */
#ifdef CSR_LOG_ENABLE
            if (!(src_index < CSR_SCHED_MAX_SEGMENTS) || (instance->thread[src_index].currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_PUT))
            {
                CsrLogMessagePut(line,
                    file,
                    (CsrSchedQid) (currentTask | (src_index << CSR_SCHED_QUEUE_SEGMENT_SHIFT)),
                    q,
                    id,
                    mi,
                    mv);
            }
#endif

            externalSend(q, mi, mv);
        }
        else
        {
            /* Not handler for this segment! */
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNKNOWN_TASK, "external send fct. not registered");
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedBgintReg
 *
 *  DESCRIPTION
 *      Register a background interrupt handler function with the scheduler.
 *        When CsrSchedBgint() is called from the foreground (e.g. an interrupt
 *        routine) the registered function is called.
 *
 *        cb is a function pointer to a callback function that is invoked to
 *        service the background interrupt.  If cb is NULL or no bgints are
 *        available, CSR_SCHED_BGINT_INVALID is returned, otherwise a CsrSchedBgint
 *        value is returned to be used in subsequent calls to CsrSchedBgint().
 *        context is a pointer that is passed as an argument to the handler
 *        function.  id is a possibly NULL identifier used for logging
 *        purposes only.
 *
 *  RETURNS
 *      CsrSchedBgint -- CSR_SCHED_BGINT_INVALID denotes failure to obtain a CsrSchedBgintSet.
 *
 *----------------------------------------------------------------------------*/
CsrSchedBgint CsrSchedBgintReg(CsrSchedBgintHandler cb,
    void *context,
    const CsrCharString *id)
{
    CsrSchedBgint irq;

    if (cb != NULL)
    {
        CsrUint8 idx = 0;
        CsrUint8 hdlcount;

        hdlcount = sizeof(instance->bgint) / sizeof(instance->bgint[0]);

        CsrMutexLock(&instance->bgMutex);

        while (idx < hdlcount && instance->bgint[idx].handler != NULL)
        {
            idx++;
        }

        if (idx < hdlcount)
        {
            instance->bgint[idx].qid = CsrSchedTaskQueueGet();
            instance->bgint[idx].handler = cb;
            instance->bgint[idx].arg = context;
            instance->bgint[idx].ownerThreadIndex = CsrGetThreadIndex();
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            instance->bgint[idx].activeBgMeasurements = NULL;
#endif
            irq = 1 << idx;
        }
        else
        {
            irq = CSR_SCHED_BGINT_INVALID;
        }

        CsrMutexUnlock(&instance->bgMutex);
    }
    else
    {
        irq = CSR_SCHED_BGINT_INVALID;
    }

#ifdef CSR_LOG_ENABLE
    if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_REG))
    {
        CsrLogBgintRegister(CsrGetThreadIndex(), irq, id, context);
    }
#endif

    return irq;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedBgintUnreg
 *
 *  DESCRIPTION
 *      Unregister a background interrupt handler.
 *
 *      irq is the interrupt number assigned by a previous successful call to
 *      CsrSchedBgintReg().
 *
 *  RETURNS
 *      void.
 *
 *----------------------------------------------------------------------------*/
void CsrSchedBgintUnreg(CsrSchedBgint irq)
{
    if (irq != CSR_SCHED_BGINT_INVALID)
    {
        CsrUint8 idx;

#ifdef CSR_LOG_ENABLE
        if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_UNREG))
        {
            CsrLogBgintUnregister(irq);
        }
#endif

        idx = 0;
        /* Trivial mod_2 of irq -> idx */
        while (irq > 1)
        {
            irq = irq >> 1;
            idx++;
        }

        CsrMutexLock(&instance->bgMutex);
        instance->bgint[idx].handler = NULL;
        instance->bgint[idx].arg = NULL;
        CsrMutexUnlock(&instance->bgMutex);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedBgintSet
 *
 *  DESCRIPTION
 *      Set background interrupt.
 *
 *  RETURNS
 *      void.
 *
 *----------------------------------------------------------------------------*/
void CsrSchedBgintSet(CsrSchedBgint irq)
{
    CsrSchedBgint vector = irq;
    CsrUint8 i = 0;

    if (irq == CSR_SCHED_BGINT_INVALID)
    {
        /* Invalid bgint */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Invalid CsrSchedBgint");
        return;
    }

#ifdef CSR_LOG_ENABLE
    if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_SET))
    {
        CsrLogBgintSet(irq);
    }
#endif

    while (!(vector & 1))
    {
        i++;
        vector >>= 1;
    }

    CsrEventSet(&instance->thread[instance->bgint[i].ownerThreadIndex].eventHandle, BG_INT_EVENT | irq);
}

static void adjust_wrap_count(ThreadInstanceType *thread, CsrTime now)
{
    if (now < thread->lastNow)
    {
        thread->currentWrapCount++;
    }

    thread->lastNow = now;
}

#ifdef ENABLE_SHUTDOWN
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedInstanceTaskDeinit
 *
 *  DESCRIPTION
 *      After the scheduler has been stopped this function
 *      is invoked by each scheduler thread instance. It runs
 *      the deinit functions of tasks in this thread instance.
 *
 *  RETURNS
 *        void
 *
 *----------------------------------------------------------------------------*/
static void CsrSchedInstanceTaskDeinit(void *data)
{
    ThreadInstanceType *thread;
    TimerType *timer;
    CsrUint8 i;

    thread = (ThreadInstanceType *) data;

    /* Tasks' deinitialisation functions. */

    CSR_LOG_TEXT("-------------- CLEAN UP --------------");
    CSR_LOG_TEXT("Call task deinit function");

    for (i = 0; i < thread->numberOfTasks; i++)
    {
        if (thread->tasks[i].deinitFunction)
        {
            thread->currentTask = (CsrSchedQid) (i | (thread->id << CSR_SCHED_QUEUE_SEGMENT_SHIFT));

#ifdef CSR_LOG_ENABLE
            thread->currentTaskLogFilter = CsrLogTaskFilterGet(thread->currentTask);
            CsrLogDeinitTask(thread->currentTask);
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_TASK(thread->currentTask), &thread->tasks[i].activeTaskMeasurements);
#endif

            thread->tasks[i].deinitFunction(&(thread->tasksInfo[i].instanceDataPointer));

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_SYSTEM(thread->id), &thread->activeSystemMeasurements);
#endif

            thread->currentTask = CSR_SCHED_TASK_ID;
        }
    }

    CSR_LOG_TEXT("-------------- CLEAN UP --------------");
    CSR_LOG_TEXT("Free any remaining timed events");

    while (thread->timerList)
    {
        timer = thread->timerList;
        thread->timerList = thread->timerList->next;
        CsrPmemFree(timer);
    }

    CSR_LOG_TEXT("-------- CLEANED UP (Sched exit) --------");
}

#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedLoop
 *
 *  DESCRIPTION
 *      The main function of the background task scheduler. This
 *      invokes tasks as messages become available for them and
 *      delivers timed event calls.
 *
 *
 *  RETURNS
 *        void
 *
 *----------------------------------------------------------------------------*/
void CsrSchedLoop(void *data)
{
    ThreadInstanceType *thread;
    CsrUint32 eventBits, timeout;
    CsrUint32 eventBitsOther;
    CsrTime now;
    TimerType *timer;
    CsrUint16 i;
#ifdef ENABLE_SHUTDOWN
    CsrResult result;
#endif
    thread = (ThreadInstanceType *) data;

#ifdef CSR_LOG_ENABLE
    CsrLogSchedStart(thread->id);
#endif

    CsrEventWait(&thread->eventHandle, CSR_EVENT_WAIT_INFINITE, &eventBits);
    eventBitsOther = (eventBits & ~START_EVENT);
    if (eventBitsOther)
    {
        /* If thread is signalled from another thread, by a message put,
         * then pass this signal to our self. */
        CsrEventSet(&thread->eventHandle, eventBitsOther);
    }

    /* make sure the CsrSched loop continues until interrupted */
    thread->schedRunning = TRUE;

    /* Tasks' initialisation functions. */
    for (i = 0; i < thread->numberOfTasks; i++)
    {
        thread->currentTask = (CsrSchedQid) (i | (thread->id << CSR_SCHED_QUEUE_SEGMENT_SHIFT));

#ifdef CSR_LOG_ENABLE
        thread->currentTaskLogFilter = CsrLogTaskFilterGet(thread->currentTask);
        CsrLogInitTask(thread->id, thread->currentTask, thread->tasks[i].name);
#endif
        if (thread->tasks[i].initFunction)
        {
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_TASK(thread->currentTask), &thread->tasks[i].activeTaskMeasurements);
#endif

            thread->tasks[i].initFunction(&(thread->tasksInfo[i].instanceDataPointer));

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_SYSTEM(thread->id), &thread->activeSystemMeasurements);
#endif
        }
        thread->currentTask = CSR_SCHED_TASK_ID;
    }

    i = 0;
    while (thread->schedRunning)
    {
        if (thread->pendingMessages > 0)
        {
            /* Consider each task in turn. */
            while (1)
            {
                if (i >= thread->numberOfTasks)
                {
                    i = 0;
                }
                if (thread->tasksInfo[i].messageQueueFirst)
                {
                    thread->currentTask = (CsrSchedQid) (i | (thread->id << CSR_SCHED_QUEUE_SEGMENT_SHIFT));

#ifdef CSR_LOG_ENABLE
                    thread->currentTaskLogFilter = CsrLogTaskFilterGet(thread->currentTask);

                    if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TASK_SWITCH)
                    {
                        CsrLogActivate(thread->currentTask);
                    }
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                    contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_TASK(thread->currentTask), &thread->tasks[i].activeTaskMeasurements);
#endif

                    thread->tasks[i].handlerFunction(&(thread->tasksInfo[i].instanceDataPointer));

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                    contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_SYSTEM(thread->id), &thread->activeSystemMeasurements);
#endif

#ifdef CSR_LOG_ENABLE
                    if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TASK_SWITCH)
                    {
                        CsrLogDeactivate(thread->currentTask);
                    }
#endif

                    thread->currentTask = CSR_SCHED_TASK_ID;
                    i++;
                    break;
                }
                else
                {
                    i++;
                }
            }
        }

        /* if no internal messages wait for external event or a timer to to expire */
        if (thread->pendingMessages == 0)
        {
            /* Currently there is no messages on any of the Scheduler
               task queues. Wait for a timer or an external event to occur */
            if (thread->timerList != NULL)
            {
                now = CsrTimeGet(NULL);
                if (thread->timerList->wrapCount > thread->currentWrapCount)
                {
                    timeout = (CSR_SCHED_TIME_MAX - now + thread->timerList->when);
                    /* Avoid rounding to zero in division below. */
                    timeout += 999;
                }
                else
                {
                    if (now < thread->timerList->when)
                    {
                        timeout = CsrTimeSub(thread->timerList->when, now);
                        /* Avoid rounding to zero in division below. */
                        timeout += 999;
                    }
                    else
                    {
                        timeout = 0;
                    }
                }
                timeout /= 1000;
                if (timeout >= CSR_EVENT_WAIT_INFINITE)
                {
                    timeout = CSR_EVENT_WAIT_INFINITE - 1;
                }
            }
            else
            {
                timeout = CSR_EVENT_WAIT_INFINITE;
            }
        }
        else
        {
            timeout = 0;
        }

        if (CsrEventWait(&thread->eventHandle, (CsrUint16) timeout, &eventBits) == CSR_RESULT_SUCCESS)
        {
            /* background interrupt */
            if (BG_INT_EVENT & eventBits)
            {
                CsrUint8 j;
                CsrSchedBgint vector = (CsrSchedBgint) (eventBits & BG_INT_MASK);
                for (j = 0; j < BGINT_COUNT; j++)
                {
                    /* Check if this bgint is set */
                    if (vector & 0x1)
                    {
                        thread->currentTask = instance->bgint[j].qid;
#ifdef CSR_LOG_ENABLE
                        if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_START))
                        {
                            CsrLogBgintServiceStart((CsrSchedBgint) (1 << j));
                        }
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                        contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_BGINT(1 << j), &instance->bgint[j].activeBgMeasurements);
#endif

                        instance->bgint[j].handler(instance->bgint[j].arg);

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                        contextSwitch(thread, CSR_SCHED_IPS_CONTEXT_SYSTEM(thread->id), &thread->activeSystemMeasurements);
#endif

#ifdef CSR_LOG_ENABLE
                        if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_DONE))
                        {
                            CsrLogBgintServiceDone((CsrSchedBgint) (1 << j));
                        }
#endif
                        thread->currentTask = CSR_SCHED_TASK_ID;
                    }

                    vector >>= 1;
                    if (!vector)
                    {
                        break;
                    }
                }
            }

            /* a message arriving from outside */
            if (EXT_MSG_EVENT & eventBits)
            {
                CsrSchedTaskId qi;
                MessageQueueEntryType *message;

                CsrMutexLock(&thread->qMutex);

                while (thread->extMsgQueueFirst != NULL)
                {
                    /* remove from ext queue */
                    message = thread->extMsgQueueFirst;
                    thread->extMsgQueueFirst = message->next;
                    message->next = NULL;

                    qi = message->receiver;

                    /* ...store the on the end of the task's message chain. */
                    if (thread->tasksInfo[qi].messageQueueLast == NULL)
                    {
                        thread->tasksInfo[qi].messageQueueFirst = message;
                        thread->tasksInfo[qi].messageQueueLast = message;
                    }
                    else
                    {
                        thread->tasksInfo[qi].messageQueueLast->next = message;
                        thread->tasksInfo[qi].messageQueueLast = message;
                    }
                    thread->pendingMessages++;
                }

                /* At this point the external queue is empty. */
                thread->extMsgQueueLast = NULL;

                CsrMutexUnlock(&thread->qMutex);
            }
#ifdef ENABLE_SHUTDOWN
            /* a sched stop req */
            if (STOP_REQ_EVENT & eventBits)
            {
                /* return stop cfm and break out of scheduler loop */
                thread->schedRunning = FALSE;
                CsrEventSet(&instance->eventHandle, STOP_CFM_EVENT | (0x0001 << (CsrUint32) thread->id));
                break;
            }
#endif
        }

        while (thread->timerList)
        {
            /* Now, back to timed events. */
            now = CsrTimeGet(NULL);
            /* Handle timer wrap */
            adjust_wrap_count(thread, now);

            if (thread->timerList->wrapCount > thread->currentWrapCount)
            {
                /* The earliest deadline is more than 1 wrapCount in the future. */
                break;
            }
            else
            {
                if ((thread->timerList->when > now) &&
                    (thread->timerList->wrapCount == thread->currentWrapCount))
                {
                    break;
                }
                else
                {
                    /* Run the timed event function. */
                    timer = thread->timerList;
                    thread->timerList = timer->next;
                    thread->currentTask = timer->queue;
#ifdef CSR_LOG_ENABLE
                    thread->currentTaskLogFilter = CsrLogTaskFilterGet(thread->currentTask);

                    if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_FIRE)
                    {
                        CsrLogTimedEventFire(thread->currentTask,
                            timer->id);
                    }
#endif

                    timer->eventFunction(timer->fniarg, timer->fnvarg);

#ifdef CSR_LOG_ENABLE
                    if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_DONE)
                    {
                        CsrLogTimedEventDone(thread->currentTask,
                            timer->id);
                    }
#endif

                    thread->currentTask = CSR_SCHED_TASK_ID;

                    timerFree(thread, timer);
                    --thread->pendingTimers;
                }
            }
        }
    } /*  while (thread->schedRunning) */

#ifdef ENABLE_SHUTDOWN
#ifdef CSR_LOG_ENABLE
    CsrLogSchedStop(thread->id);
#endif

    /* wait for deinit req event */
    while (1)
    {
        eventBits = 0;
        result = CsrEventWait(&thread->eventHandle, CSR_EVENT_WAIT_INFINITE, &eventBits);
        if ((result == CSR_RESULT_SUCCESS) && (DEINIT_REQ_EVENT & eventBits))
        {
            CsrSchedTaskId qi;
            MessageQueueEntryType *message;

            /*
             * Pull any messages off the external queue and
             * put them on the relevant task queue. These may
             * be put here after we stopped because the sender
             * had not received its stop event yet.
             */
            while (thread->extMsgQueueFirst != NULL)
            {
                /* remove from ext queue */
                message = thread->extMsgQueueFirst;
                thread->extMsgQueueFirst = message->next;
                message->next = NULL;

                qi = message->receiver;

                /* ...store the on the end of the task's message chain. */
                if (thread->tasksInfo[qi].messageQueueLast == NULL)
                {
                    thread->tasksInfo[qi].messageQueueFirst = message;
                    thread->tasksInfo[qi].messageQueueLast = message;
                }
                else
                {
                    thread->tasksInfo[qi].messageQueueLast->next = message;
                    thread->tasksInfo[qi].messageQueueLast = message;
                }
                thread->pendingMessages++;
            }

            /* deinit all tasks in this scheduler thread and
             * return deinit cfm */
            CsrSchedInstanceTaskDeinit(thread);
            CsrEventSet(&instance->eventHandle, DEINIT_CFM_EVENT | (0x0001 << (CsrUint32) thread->id));
            return;
        }
        else if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT("Failure in wait for thread deinit req");
            return;
        }
    }
#endif
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSched
 *
 *  DESCRIPTION
 *      Starts the scheduler. The function will not return until
 *      CsrSchedStop() has been called.
 *
 *      The data parameter is the scheduler context returned by the
 *      CsrSchedInit() function.
 *
 *  RETURNS
 *        void
 *
 *----------------------------------------------------------------------------*/
void CsrSched(void *data)
{
    CsrUint8 i;
    CsrUint32 eventBits;
    CsrResult result;
#ifdef ENABLE_SHUTDOWN
    CsrUint16 readThreadIdVector;
#endif
    CsrCharString name[8] = "sched-x";
    SchedulerInstanceType *inst = (SchedulerInstanceType *) data;

    /* Create scheduler threads */
    for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
    {
        if (inst->thread[i].inUse)
        {
            name[6] = '0' + i;

            if (CsrThreadCreate(CsrSchedLoop, &inst->thread[i], inst->thread[i].stackSize, inst->thread[i].priority, name, &(inst->thread[i].thread_handle))
                != CSR_RESULT_SUCCESS)
            {
                CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Thread creation failed");
                return;
            }
        }
    }

    /* Start scheduler threads */
    for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
    {
        if (inst->thread[i].inUse)
        {
            CsrEventSet(&inst->thread[i].eventHandle, START_EVENT);
        }
    }

    /* The following handles scheduler shutdown and task deinit.
     * The shutdown sequence first signals all threads to stop
     * their scheduler loop. Then it signals all threads to run
     * their task deinit functions.
     *
     * Shutdown is successfull if all threads respond to stop and deinit
     * requests. Missing responses will cause the shutdown sequence to
     * be aborted and the incident will be logged. The CsrSched() function
     * will then return immediately. No panics will be generated.
     *
     * If shutdown is disabled the CsrSched() function will block forever.
     */

    /*
     * Block until told to exit from CsrSchedStop(). Or block forever if
     * shutdown is disabled.
     */
    result = CsrEventWait(&inst->eventHandle, CSR_EVENT_WAIT_INFINITE, &eventBits);
#ifndef ENABLE_SHUTDOWN
    /* We should never come here if shutdown is disabled. */
    CSR_UNUSED(result);

    CSR_LOG_TEXT("Unexpected scheduler shutdown");
#else
    if ((result != CSR_RESULT_SUCCESS) || (eventBits != STOP_REQ_EVENT))
    {
        CSR_LOG_TEXT("Failure in wait for task stop request");
        return;
    }

    /* Signal all threads to stop their scheduler loop */
    for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
    {
        if (inst->thread[i].inUse)
        {
            CsrEventSet(&inst->thread[i].eventHandle, STOP_REQ_EVENT);
        }
    }

    /* Block until all threads are stopped. */
    eventBits = 0;
    readThreadIdVector = 0;
    while (readThreadIdVector != instance->threadIdVector)
    {
        result = CsrEventWait(&inst->eventHandle, CSR_EVENT_WAIT_INFINITE, &eventBits);
        if ((result == CSR_RESULT_SUCCESS) && (STOP_CFM_EVENT & eventBits))
        {
            readThreadIdVector |= THREAD_ID_MASK & eventBits;
        }
        else if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT("Failure in wait for thread stop confirm");
            return;
        }
    }

    /* signal all threads to run their task deinit */
    for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
    {
        if (inst->thread[i].inUse)
        {
            CsrEventSet(&inst->thread[i].eventHandle, DEINIT_REQ_EVENT);
        }
    }

    /* block until all threads have completed task deinit */
    eventBits = 0;
    readThreadIdVector = 0;
    while (readThreadIdVector != instance->threadIdVector)
    {
        result = CsrEventWait(&inst->eventHandle, CSR_EVENT_WAIT_INFINITE, &eventBits);
        if ((result == CSR_RESULT_SUCCESS) && (DEINIT_CFM_EVENT & eventBits))
        {
            readThreadIdVector |= THREAD_ID_MASK & eventBits;
        }
        else if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT("Failure in wait for thread deinit confirm");
            return;
        }
    }
#endif
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedInit
 *
 *  DESCRIPTION
 *      Prepare scheduler instance with identifier id. The scheduler instance
 *      will run in its own thread with given priority and stack size.
 *      The function returns the new scheduler instance. Priority and stack
 *      size values are passed unchanged to the CsrThreadCreate() call of the
 *      underlying Framework Extension API.
 *
 *      Valid id range is 0 to CSR_SCHED_MAX_SEGMENTS - 1
 *
 *      Valid priority and stack size values are determined be the Framework
 *      Extensions API.
 *
 *  RETURNS
 *      The scheduler instance
 *
 *----------------------------------------------------------------------------*/
void *CsrSchedInit(CsrUint16 id, CsrUint16 priority, CsrUint32 stackSize)
{
    CsrUint16 i;

    /* Sanity check */
    if (id >= CSR_SCHED_MAX_SEGMENTS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "CSR_SCHED_MAX_SEGMENTS exceeded");
        return NULL;
    }

    /* Alloc instance */
    if (instance == NULL)
    {
        instance = (SchedulerInstanceType *) CsrPmemZalloc(sizeof(SchedulerInstanceType));

        for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
        {
            instance->thread[i].currentTask = CSR_SCHED_TASK_ID;
        }

        CsrMutexCreate(&instance->bgMutex);

        if (CsrEventCreate(&instance->eventHandle))
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Event creation failed");
            return NULL;
        }
    }

    instance->thread[id].inUse = TRUE;
    instance->threadIdVector |= 0x0001 << (CsrUint16) id;

    /* Initial running state */
    instance->thread[id].schedRunning = FALSE;

    /* Set thread priority */
    instance->thread[id].priority = priority;

    /* Set thread stack size */
    instance->thread[id].stackSize = stackSize;

    /* Start initialisation of a single thread */
    instance->thread[id].init = TRUE;

    /* Store thread id */
    instance->thread[id].id = (CsrUint8) id;

    /* Collect the number of tasks for this thread */
    instance->setupId = id;
    CsrSchedTaskInit((void *) instance);

    /* Setup task structures for thread */
    instance->thread[id].tasks = CsrPmemAlloc(instance->thread[id].numberOfTasks * sizeof(TaskDefinitionType));
    instance->thread[id].tasksInfo = CsrPmemAlloc(instance->thread[id].numberOfTasks * sizeof(TaskInformationType));

    /* Run task setup once more to transfer the function pointers */
    instance->thread[id].numberOfTasks = 0;
    instance->thread[id].init = FALSE;

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    instance->thread[id].activeSystemMeasurements = NULL;
    instance->thread[id].currentContext = CSR_SCHED_IPS_CONTEXT_SYSTEM(id);
    if (CsrMutexCreate(&(instance->thread[id].ipsMutex)) != CSR_RESULT_SUCCESS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Mutex creation failed");
        return NULL;
    }
#endif

    /* Prepare tasks */
    CsrSchedTaskInit((void *) instance);

    instance->setupId = CSR_SCHED_TASK_ID;

    if (CsrMutexCreate(&(instance->thread[id].qMutex)) != CSR_RESULT_SUCCESS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Mutex creation failed");
        return NULL;
    }

    if (CsrEventCreate(&(instance->thread[id].eventHandle)))
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Event creation failed");
        return NULL;
    }

#ifdef CSR_LOG_ENABLE
    CsrLogSchedInit((CsrUint8) id);
#endif

    /* Prepare queues */
    for (i = 0; i < instance->thread[id].numberOfTasks; i++)
    {
        instance->thread[id].tasksInfo[i].instanceDataPointer = NULL;
        instance->thread[id].tasksInfo[i].messageQueueFirst = NULL;
        instance->thread[id].tasksInfo[i].messageQueueLast = NULL;
    }

    return instance;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedDeinit
 *
 *  DESCRIPTION
 *      Deinitialise the scheduler.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrSchedDeinit(void *data)
{
    SchedulerInstanceType *inst;
    inst = (SchedulerInstanceType *) data;

    if (instance != inst)
    {
        return;
    }

    if (inst != NULL)
    {
        CsrUint8 i;

        for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
        {
            if (inst->thread[i].inUse)
            {
                MessageQueueEntryType *msg, *msgNext;
                TimerType *timer, *timerNext;

                for (msg = inst->thread[i].messageFreeList;
                     msg;
                     msg = msgNext)
                {
                    msgNext = msg->next;
                    CsrPmemFree(msg);
                }

                for (timer = inst->thread[i].timerFreeList;
                     timer;
                     timer = timerNext)
                {
                    timerNext = timer->next;
                    CsrPmemFree(timer);
                }

                CsrPmemFree(inst->thread[i].tasksInfo);
                CsrPmemFree(inst->thread[i].tasks);
                CsrMutexDestroy(&inst->thread[i].qMutex);
                CsrEventDestroy(&inst->thread[i].eventHandle);
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                CsrMutexDestroy(&inst->thread[i].ipsMutex);
#endif
#ifdef CSR_LOG_ENABLE
                CsrLogSchedDeinit(inst->thread[i].id);
#endif
            }
        }

        CsrMutexDestroy(&inst->bgMutex);
        CsrEventDestroy(&inst->eventHandle);

        instance = NULL;
        CsrPmemFree(inst);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedStop
 *
 *  DESCRIPTION
 *      Stop the scheduler in order to make the CsrSched() function return.
 *      This is usually called from within a scheduler task like a demo-program
 *
 *  RETURNS
 *        void
 *
 *----------------------------------------------------------------------------*/
void CsrSchedStop(void)
{
#ifdef ENABLE_SHUTDOWN
    CSR_LOG_TEXT("CsrSchedStop() called");
    /* Set flag to interrupt the CsrSched loop and wake it if it's
     * sleeping */
    CsrEventSet(&instance->eventHandle, STOP_REQ_EVENT);
#endif
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBroadcastMessage
 *
 *  DESCRIPTION
 *      Sends a message to all tasks.
 *
 *      The user must supply a "factory function" that is called once
 *      for every task that exists. The "factory function", msg_build_func,
 *      must allocate and initialise the message and set the msg_build_ptr
 *      to point to the message when done.
 *
 *  NOTE
 *      N/A
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER)
void CsrSchedMessageBroadcastStringLog(CsrUint16 mi,
    void *(*msg_build_func)(void *),
    void *msg_build_ptr,
    CsrUint32 line,
    const CsrCharString *file)
#else
void CsrSchedMessageBroadcast(CsrUint16 mi,
    void *(*msg_build_func)(void *),
    void *msg_build_ptr)
#endif
{
    void *msg;
    ThreadInstanceType *thread;
    CsrUint8 i;
    CsrSchedQid t, qid;

#if !(defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER))
    CsrUint32 line = 0;
    const CsrCharString *file = NULL;
#endif

    if (msg_build_func)
    {
        for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
        {
            thread = &instance->thread[i];
            if (thread->inUse && thread->schedRunning)
            {
                for (t = 0; t < thread->numberOfTasks; t++)
                {
                    if (thread->tasks[t].handlerFunction != NULL)
                    {
                        msg = msg_build_func(msg_build_ptr);

                        qid = (i << CSR_SCHED_QUEUE_SEGMENT_SHIFT) | t;
                        do_put_message(qid, mi, msg, line, file);
                    }
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedMessagePut
 *
 *  DESCRIPTION
 *      Sends a message consisting of the integer "mi" and the void * pointer
 *      "mv" to the message queue "q".
 *
 *      "mi" and "mv" are neither inspected nor changed by the scheduler - the
 *      task that owns "q" is expected to make sense of the values. "mv" may
 *      be null.
 *
 *  NOTE
 *      If "mv" is not null then it will typically be a chunk of CsrPmemAlloc()ed
 *      memory, though there is no need for it to be so. Tasks should normally
 *      obey the convention that when a message built with CsrPmemAlloc()ed memory
 *      is given to CsrSchedMessagePut() then ownership of the memory is ceded to the
 *      scheduler - and eventually to the recipient task. I.e., the receiver of
 *      the message will be expected to CsrPmemFree() the message storage.
 *
 *  RETURNS
 *      void.
 *
 *----------------------------------------------------------------------------*/
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER)
void CsrSchedMessagePutStringLog(CsrSchedQid q,
    CsrUint16 mi,
    void *mv,
    CsrUint32 line,
    const CsrCharString *file)
#else
void CsrSchedMessagePut(CsrSchedQid q,
    CsrUint16 mi,
    void *mv)
#endif
{
#if !(defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER))
    CsrUint32 line = 0;
    const CsrCharString *file = NULL;
#endif
    /* Obtain a new message identifier and call do_put_message() */
    do_put_message(q, mi, mv, line, file);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedMessageGet
 *
 *  DESCRIPTION
 *      Obtains a message from the message queue "q" if one is available. The
 *      calling task must own "q". The message consists of one or both of a
 *      CsrUint16 and a void *.
 *
 *      If the calling task does not own "q" then the scheduler calls panic().
 *
 *  RETURNS
 *      CsrBool - TRUE if a message has been obtained from the queue, else FALSE.
 *      If a message is taken from the queue, then "*pmi" and "*pmv" are set to
 *      the "mi" and "mv" passed to CsrSchedMessagePut() respectively.
 *
 *      "pmi" and "pmv" can be null, in which case the corresponding value from
 *      them message is discarded.
 *
 *----------------------------------------------------------------------------*/
CsrBool CsrSchedMessageGet(CsrUint16 *pmi, void **pmv)
{
    ThreadInstanceType *thread;
    MessageQueueEntryType *message;
    CsrUint8 index;
    CsrSchedQid qi;

    index = CsrGetThreadIndex();
    if (index < CSR_SCHED_MAX_SEGMENTS)
    {
        thread = &instance->thread[index];
    }
    else
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
            "CsrSchedMessageGet not called from scheduler thread");
        return FALSE;
    }

    /* Try to remove message from the base of the message chain. */
    qi = QUEUE_EXTRACT_INDEX(thread->currentTask);
    message = thread->tasksInfo[qi].messageQueueFirst;
    if (message)
    {
        if (pmi)
        {
            *pmi = message->event;
        }
        if (pmv)
        {
            *pmv = message->message;
        }

        thread->tasksInfo[qi].messageQueueFirst = message->next;
        if (thread->tasksInfo[qi].messageQueueLast == message)
        {
            /* Removed the final element */
            thread->tasksInfo[qi].messageQueueLast = NULL;
        }

        --thread->pendingMessages;

#ifdef CSR_LOG_ENABLE
        if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_GET)
        {
            CsrLogMessageGet(message->sender,
                (CsrSchedQid) (thread->currentTask),
                TRUE,
                message->id,
                message->event,
                message->message);
        }
#endif

        messageFree(thread, message);

        return TRUE;
    }

#ifdef CSR_LOG_ENABLE
    if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_GET)
    {
        CsrLogMessageGet(0,
            (CsrSchedQid) (thread->currentTask),
            FALSE,
            0,
            0,
            NULL);
    }
#endif

    return FALSE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedTimerSet
 *
 *  DESCRIPTION
 *      Causes the void function "fn" to be called with the arguments
 *      "fniarg" and "fnvarg" after "delay" has elapsed.
 *
 *      "delay" must be less than half the range of a CsrTime.
 *
 *      CsrSchedTimerSet() does nothing with "fniarg" and "fnvarg" except
 *      deliver them via a call to "fn()".   (Unless CsrSchedTimerCancel()
 *      is used to prevent delivery.)
 *
 *  NOTE
 *      The function will be called at or after "delay"; the actual delay will
 *      depend on the timing behaviour of the scheduler's tasks.
 *
 *  RETURNS
 *      CsrSchedTid - A timer identifier, can be used in CsrSchedTimerCancel().
 *
 *----------------------------------------------------------------------------*/
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER)
CsrSchedTid CsrSchedTimerSetStringLog(CsrTime delay,
    void (*fn)(CsrUint16 mi, void *mv),
    CsrUint16 fniarg,
    void *fnvarg,
    CsrUint32 line,
    const CsrCharString *file)
#else
CsrSchedTid CsrSchedTimerSet(CsrTime delay,
    void (*fn)(CsrUint16 mi, void *mv),
    CsrUint16 fniarg,
    void *fnvarg)
#endif
{
    TimerType *ptr;
    TimerType *prev;
    TimerType *newTimer;
    CsrTime when;
    CsrTime now;
    CsrUint32 whenWrapCount;
    CsrUint8 index;
    ThreadInstanceType *thread;


#if defined(CSR_LOG_ENABLE) && !defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER)
    CsrUint32 line = 0;
    const CsrCharString *file = NULL;
#endif

    index = CsrGetThreadIndex();
    if (index < CSR_SCHED_MAX_SEGMENTS)
    {
        thread = &instance->thread[index];
    }
    else
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
            "CsrSchedTimerSet not called from scheduler thread");
        return FALSE;
    }

    /* Get absolute time of event */
    now = CsrTimeGet(NULL);
    when = CsrTimeAdd(now, delay);

    if (++thread->pendingTimers >= MAX_TIMED_EVENTS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_TIMED_EVENTS,
            "too many timers");
    }

    if (when < now)
    {
        whenWrapCount = 1;
    }
    else
    {
        whenWrapCount = 0;
    }

    newTimer = timerAlloc(thread);

    /* Build the structure. */
    newTimer->when = when;
    newTimer->wrapCount = whenWrapCount;
    newTimer->eventFunction = fn;
    newTimer->fniarg = fniarg;
    newTimer->fnvarg = fnvarg;
    newTimer->id = (CsrSchedTid) getMsgId(thread);
    newTimer->next = NULL;
    newTimer->queue = CsrSchedTaskQueueGet();

    /* Handle timer wrap */
    adjust_wrap_count(thread, now);

    newTimer->wrapCount += thread->currentWrapCount;

    /* Store struct in time position in the timer list. */
    if (!thread->timerList)
    {
        thread->timerList = newTimer;
    }
    else
    {
        ptr = thread->timerList;
        prev = NULL;

        /* Determine where to insert element */
        while (ptr)
        {
            if (((ptr->when > newTimer->when) &&
                 (ptr->wrapCount == newTimer->wrapCount)) ||
                (ptr->wrapCount > newTimer->wrapCount))

            {
                break;
            }
            else
            {
                prev = ptr;
                ptr = ptr->next;
            }
        }

        if (prev == NULL)
        {
            newTimer->next = thread->timerList;
            thread->timerList = newTimer;
        }
        else
        {
            prev->next = newTimer;
            newTimer->next = ptr;
        }
    }

#ifdef CSR_LOG_ENABLE
    if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_IN)
    {
        CsrLogTimedEventIn(line,
            file,
            (CsrSchedQid) (thread->currentTask),
            newTimer->id,
            delay,
            fniarg,
            fnvarg);
    }
#endif
    return newTimer->id;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedTimerCancel
 *
 *  DESCRIPTION
 *      Attempts to prevent the timer with identifier "eventid" from
 *      occurring.
 *
 *  RETURNS
 *      CsrBool - TRUE if cancelled, FALSE if the event has already occurred.
 *
 *----------------------------------------------------------------------------*/
#if defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER)
CsrBool CsrSchedTimerCancelStringLog(CsrSchedTid eventid,
    CsrUint16 *pmi,
    void **pmv,
    CsrUint32 line,
    const CsrCharString *file)
#else
CsrBool CsrSchedTimerCancel(CsrSchedTid eventid,
    CsrUint16 *pmi,
    void **pmv)
#endif
{
    TimerType **pte, *tmpte;
    CsrUint8 index;
    ThreadInstanceType *thread;

#if defined(CSR_LOG_ENABLE) && !defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER)
    CsrUint32 line = 0;
    const CsrCharString *file = NULL;
#endif

    index = CsrGetThreadIndex();
    if (index < CSR_SCHED_MAX_SEGMENTS)
    {
        thread = &instance->thread[index];
    }
    else
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
            "CsrSchedTimerCancel not called from scheduler thread");
        return FALSE;
    }

    if (eventid != CSR_SCHED_TID_INVALID)
    {
        /* Search the list of timers. */
        for (pte = &thread->timerList; *pte != (TimerType *) NULL; pte = &((*pte)->next))
        {
            /* If we match the id then snip out the event. */
            if (eventid == (*pte)->id)
            {
                tmpte = *pte;
                *pte = tmpte->next;

                if (pmi)
                {
                    *pmi = tmpte->fniarg;
                }
                if (pmv)
                {
                    *pmv = tmpte->fnvarg;
                }

#ifdef CSR_LOG_ENABLE
                if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_CANCEL)
                {
                    CsrLogTimedEventCancel(line, file,
                        (CsrSchedQid) (thread->currentTask),
                        eventid,
                        TRUE);
                }
#endif

                timerFree(thread, tmpte);
                --thread->pendingTimers;

                return TRUE;
            }
        }
    }

#ifdef CSR_LOG_ENABLE
    if (thread->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_CANCEL)
    {
        CsrLogTimedEventCancel(line, file,
            thread->currentTask,
            eventid,
            FALSE);
    }
#endif

    return FALSE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedTaskQueueGet
 *
 *  DESCRIPTION
 *      Return the queue identifier for the currently running queue
 *
 *  RETURNS
 *      CsrSchedQid - The current task queue identifier, or CSR_SCHED_TASK_ID if not available.
 *
 *----------------------------------------------------------------------------*/
CsrSchedQid CsrSchedTaskQueueGet(void)
{
    CsrUint8 index = CsrGetThreadIndex();

    if (index < CSR_SCHED_MAX_SEGMENTS)
    {
        return instance->thread[index].currentTask;
    }
    else
    {
        return CSR_SCHED_TASK_ID;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedRegisterTask
 *
 *  DESCRIPTION
 *      Register a task in the scheduler.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrSchedRegisterTask(CsrSchedQid *queue,
    schedEntryFunction_t init,
    schedEntryFunction_t deinit,
    schedEntryFunction_t handler,
    const CsrCharString *taskName,
    void *data,
    CsrUint16 id)
{
    SchedulerInstanceType *inst;
    ThreadInstanceType *thread;

    inst = (SchedulerInstanceType *) data;
    thread = &(inst->thread[id]);

    /* Only setup tasks for current thread */
    if (inst->setupId == id)
    {
        if (thread->init)
        {
            /* First run - assign queue and count */
            *queue = ((id << CSR_SCHED_QUEUE_SEGMENT_SHIFT) | thread->numberOfTasks);
            thread->numberOfTasks++;
        }
        else
        {
            /* Second run - store functions etc. */
            thread->tasks[thread->numberOfTasks].initFunction = init;
            thread->tasks[thread->numberOfTasks].deinitFunction = deinit;
            thread->tasks[thread->numberOfTasks].handlerFunction = handler;
            thread->tasks[thread->numberOfTasks].name = taskName;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            thread->tasks[thread->numberOfTasks].activeTaskMeasurements = NULL;
#endif
            thread->numberOfTasks++;
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedRegisterExternalSend
 *
 *  DESCRIPTION
 *      Register a function for handling messages that are not targetted
 *      this scheduler
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrSchedRegisterExternalSend(CsrBool (*f)(CsrSchedQid q, CsrUint16 mi, void *mv))
{
    externalSend = f;
}

#ifdef CSR_LOG_ENABLE
CsrUint16 CsrSchedNumberOfTasksGet(void)
{
    CsrUint16 tasks = 0;
    CsrUint8 index;
    CsrThreadHandle handle;

    if ((instance != NULL) && (CsrThreadGetHandle(&handle) == CSR_RESULT_SUCCESS))
    {
        for (index = 0; index < CSR_SCHED_MAX_SEGMENTS; index++)
        {
            if (instance->thread[index].inUse &&
                (CsrThreadEqual(&instance->thread[index].thread_handle, &handle) == CSR_RESULT_SUCCESS))
            {
                ++tasks;
            }
        }
    }

    return tasks;
}

#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
CsrUint32 *CsrSchedIpsInsert(CsrSchedIpsContext context, CsrUint32 *measurements)
{
    CsrUint8 targetThreadIndex;
    CsrUint32 *previousMeasurements;
    CsrUint32 **targetMeasurementsPtr;

    /* Handle the automatic CSR_SCHED_IPS_CONTEXT_CURRENT */
    if (context == CSR_SCHED_IPS_CONTEXT_CURRENT)
    {
        CsrUint8 threadIndex;

        /* Determine the current scheduler thread */
        threadIndex = CsrGetThreadIndex();

        if (threadIndex == 0xFF)
        {
            /* Called from outside of any of the scheduler threads - pass on as is */
            return CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, measurements);
        }
        else
        {
            /* Determine the current scheduler context */
            context = instance->thread[threadIndex].currentContext;
        }
    }

    /* Determine target of insert */
    switch (context >> 16)
    {
        case (CSR_SCHED_IPS_CONTEXT_SYSTEM(0) >> 16):
        {
            /* Determine scheduler thread being targeted */
            targetThreadIndex = (CsrUint8) (context & 0xFF);

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &instance->thread[targetThreadIndex].activeSystemMeasurements;

            break;
        }
        case (CSR_SCHED_IPS_CONTEXT_TASK(0) >> 16):
        {
            CsrUint16 q;
            CsrUint16 qi;

            /* Determine queue and index */
            q = (CsrUint16) (context & 0xFFFF);
            qi = QUEUE_EXTRACT_INDEX(q);

            /* Determine scheduler thread being targeted */
            targetThreadIndex = QUEUE_EXTRACT_SEGMENT(q);

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &instance->thread[targetThreadIndex].tasks[qi].activeTaskMeasurements;

            break;
        }
        case (CSR_SCHED_IPS_CONTEXT_BGINT(0) >> 16):
        {
            CsrSchedBgint bgint;
            CsrUint8 idx = 0;

            bgint = (CsrSchedBgint) (context & 0xFFFF);

            /* Trivial mod_2 of bgint -> idx */
            while (bgint > 1)
            {
                bgint = bgint >> 1;
                idx++;
            }

            if (idx <= BGINT_COUNT)
            {
                targetThreadIndex = instance->bgint[idx].ownerThreadIndex;
            }
            else
            {
                return NULL;
            }

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &instance->bgint[idx].activeBgMeasurements;

            break;
        }
        default:
        {
            return NULL;
        }
    }

    CsrMutexLock(&instance->thread[targetThreadIndex].ipsMutex);

    /* Substitute measurements */
    previousMeasurements = *targetMeasurementsPtr;
    *targetMeasurementsPtr = measurements;

    /* Check if replacing the currently active measurements for the targeted thread */
    if (instance->thread[targetThreadIndex].currentContext == context)
    {
        CsrIpsInsert(&instance->thread[targetThreadIndex].thread_handle, measurements);
    }

    CsrMutexUnlock(&instance->thread[targetThreadIndex].ipsMutex);

    return previousMeasurements;
}

CsrUint32 *CsrSchedIpsMeasurementsGet(CsrSchedIpsContext context)
{
    CsrUint8 targetThreadIndex;
    CsrUint32 *previousMeasurements;
    CsrUint32 **targetMeasurementsPtr;

    /* Handle the automatic CSR_SCHED_IPS_CONTEXT_CURRENT */
    if (context == CSR_SCHED_IPS_CONTEXT_CURRENT)
    {
        CsrUint8 threadIndex;

        /* Determine the current scheduler thread */
        threadIndex = CsrGetThreadIndex();

        if (threadIndex == 0xFF)
        {
            /* Called from outside of any of the scheduler threads */
            return CsrIpsMeasurementsGet(CSR_IPS_CONTEXT_CURRENT);
        }
        else
        {
            /* Determine the current scheduler context */
            context = instance->thread[threadIndex].currentContext;
        }
    }

    /* Determine which measurements to get */
    switch (context >> 16)
    {
        case (CSR_SCHED_IPS_CONTEXT_SYSTEM(0) >> 16):
        {
            /* Determine scheduler thread being targeted */
            targetThreadIndex = (CsrUint8) (context & 0xFF);

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &instance->thread[targetThreadIndex].activeSystemMeasurements;

            break;
        }
        case (CSR_SCHED_IPS_CONTEXT_TASK(0) >> 16):
        {
            CsrUint16 q;
            CsrUint16 qi;

            /* Determine queue and index */
            q = (CsrUint16) (context & 0xFFFF);
            qi = QUEUE_EXTRACT_INDEX(q);

            /* Determine scheduler thread being targeted */
            targetThreadIndex = QUEUE_EXTRACT_SEGMENT(q);

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &instance->thread[targetThreadIndex].tasks[qi].activeTaskMeasurements;

            break;
        }
        case (CSR_SCHED_IPS_CONTEXT_BGINT(0) >> 16):
        {
            CsrSchedBgint bgint;
            CsrUint8 idx = 0;

            bgint = (CsrSchedBgint) (context & 0xFFFF);

            /* Trivial mod_2 of bgint -> idx */
            while (bgint > 1)
            {
                bgint = bgint >> 1;
                idx++;
            }

            if (idx <= BGINT_COUNT)
            {
                targetThreadIndex = instance->bgint[idx].ownerThreadIndex;
            }
            else
            {
                return NULL;
            }

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &instance->bgint[idx].activeBgMeasurements;

            break;
        }
        default:
        {
            return NULL;
        }
    }

    CsrMutexLock(&instance->thread[targetThreadIndex].ipsMutex);

    /* Get measurements */
    previousMeasurements = *targetMeasurementsPtr;

    CsrMutexUnlock(&instance->thread[targetThreadIndex].ipsMutex);

    return previousMeasurements;
}

#endif
