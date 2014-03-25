/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdint.h>
#include "nucleus.h"
#include "csr_util.h"
#include "csr_panic.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_sched_init.h"
#include "csr_time.h"

#include "csr_sched_private.h"

#include "platform/csr_timer.h"
#include "platform/csr_interrupt.h"
#include "platform/csr_pmem_init.h"

#include "csr_log.h"

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_ips.h"
#include "csr_sched_ips.h"
#endif

#ifndef CSR_SCHED_DEFAULT_PRIORITY
#define CSR_SCHED_DEFAULT_PRIORITY                       127
#endif
#ifndef CSR_SCHED_DEFAULT_MAX_TICKS
#define CSR_SCHED_DEFAULT_MAX_TICKS                      2
#endif
#ifndef SCHEDULER_THREAD_STACK_SIZE
#define SCHEDULER_THREAD_STACK_SIZE                 (4 * 1024)
#endif
#ifndef SCHEDULER_THREAD_MAX_QUEUE_SIZE
#define SCHEDULER_THREAD_MAX_QUEUE_SIZE             (200)
#endif

#define NUCLEUS_MSG_SIZE                            3

#define QUEUE_EXTRACT_SEGMENT(queue)                (((queue) & CSR_SCHED_QUEUE_SEGMENT) >> CSR_SCHED_QUEUE_SEGMENT_SHIFT)
#define QUEUE_EXTRACT_INDEX(queue)                  ((queue) & ((1 << CSR_SCHED_QUEUE_SEGMENT_SHIFT) - 1))

/* 5 sec timeout for scheduler scheduler shutdown sequence */
#define SCHED_STOP_TIMEOUT_MS (5000)
//yjkim...
//#define SCHED_STOP_SUSPEND    ((((CsrUint32) SCHED_STOP_TIMEOUT_MS) * 1000) / ((CsrUint32) NU_Retrieve_Tick_Interval()))
#define SCHED_STOP_SUSPEND    NU_NO_SUSPEND //((((CsrUint32) SCHED_STOP_TIMEOUT_MS) * 1000) / ((CsrUint32) NU_Retrieve_Tick_Interval()))

extern NU_MEMORY_POOL System_Memory;
extern volatile UNSIGNED TMD_System_Clock;

/*  Scheduler instance - we need this to be global in order to gain access to the instance from get/CsrSchedMessagePut et al */
static SchedulerInstanceDataType *instance = NULL;

static const char hexChar[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

#ifdef ENABLE_SHUTDOWN
static void clear_nucleus_scheduler_queue(ThreadInstanceType *thrist);
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
static void contextSwitch(ThreadInstanceType *thrist, CsrSchedIpsContext context, CsrUint32 *measurements)
{
    INT previousLevel;

    /* Mask interrupts in case they are not */
    previousLevel = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS);

    thrist->currentContext = context;
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, measurements);

    /* Restore interrupts */
    NU_Local_Control_Interrupts(previousLevel);
}

#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *      getThreadInstance
 *
 *  DESCRIPTION
 *      Local support function. Finds the current thread's instance data
 *      and returns the pointer.
 *
 *  RETURNS
 *      Pointer to the current thread's instance data or NULL if not found.
 *
 *----------------------------------------------------------------------------*/
static ThreadInstanceType *getThreadInstance(void)
{
    UNSIGNED threadIndex;
    ThreadInstanceType *thrist;
    NU_TASK *runningTask;

    thrist = NULL;
    runningTask = NU_Current_Task_Pointer();
    for (threadIndex = 0; threadIndex < CSR_SCHED_MAX_SEGMENTS; threadIndex++)
    {
        if (instance->thread[threadIndex].inUse && (&instance->thread[threadIndex].task == runningTask))
        {
            thrist = &(instance->thread[threadIndex]);
            break;
        }
    }
    return thrist;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getThreadIndex
 *
 *  DESCRIPTION
 *      Local support function. Finds the current thread's instance data
 *      index and returns this.
 *
 *  RETURNS
 *      Index of the current thread or 0xFF if not found.
 *
 *----------------------------------------------------------------------------*/
static CsrUint8 getThreadIndex(void)
{
    CsrUint8 threadIndex;
    NU_TASK *runningTask;

    runningTask = NU_Current_Task_Pointer();
    for (threadIndex = 0; threadIndex < CSR_SCHED_MAX_SEGMENTS; threadIndex++)
    {
        if (instance->thread[threadIndex].inUse && (&instance->thread[threadIndex].task == runningTask))
        {
            return threadIndex;
        }
    }
    return 0xFF;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      csrSchedGetIdentifier
 *
 *  DESCRIPTION
 *      Actually, this just returns the next value from an incrementing
 *      CsrUint32, but it's pretty unlikely that this will cause problems.
 *
 *      It's a bit more subtle than that: if this number wraps (pretty
 *      unlikely!) then we wrap to a quarter of the range of a CsrUint32.   We
 *      can vaguely guess that there may be a few elements of the system that
 *      get created when the system starts and which exist until the system
 *      reboots.  These might have identifiers.  The approach of only giving
 *      away the lower identifiers once should circumvent such nasties,
 *      but it clearly isn't perfect.
 *
 *      Setting next_scheduler_identifier initially to one should prevent
 *      this function returning the value zero.   An identifier with value
 *      zero may have special meanings in some circumstances.
 *
 *  RETURNS
 *      CsrSchedIdentifier - An CSR_UNUSED identifier.
 *
 *  NOTICE:
 *      We use "disable pre-emption" instead of a mutex for handling mutual
 *      access. This avoids the problem of priority inversion!
 *
 *----------------------------------------------------------------------------*/

static CsrSchedIdentifier csrSchedGetIdentifier(void)
{
    CsrSchedIdentifier i;
    CsrUint8 threadIndex;

    threadIndex = getThreadIndex();
    if (threadIndex == 0xFF)
    {
        threadIndex = CSR_SCHED_MAX_SEGMENTS;
    }

    if (threadIndex < CSR_SCHED_MAX_SEGMENTS)
    {
        i = instance->thread[threadIndex].schedIdentifier;
        if (++instance->thread[threadIndex].schedIdentifier == SCHED_ID_WRAP)
        {
            instance->thread[threadIndex].schedIdentifier = SCHED_ID_BASE;
        }
    }
    else
    {
        OPTION oldPreempt;

        oldPreempt = NU_Change_Preemption(NU_NO_PREEMPT);
        i = instance->schedIdentifier;
        if (++instance->schedIdentifier == SCHED_ID_WRAP)
        {
            instance->schedIdentifier = SCHED_ID_BASE;
        }
        NU_Change_Preemption(oldPreempt);
    }
    return (threadIndex << SCHED_ID_BITRANGE) | i;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      deleteNucleusTimer
 *
 *  DESCRIPTION
 *      Disable and deletes a previously created Nucleus application timer.
 *      The parameter "timer" identifies the timer to delete.
 *----------------------------------------------------------------------------*/
static void deleteNucleusTimer(NU_TIMER *timer)
{
    NU_Control_Timer(timer, NU_DISABLE_TIMER);
    NU_Delete_Timer(timer);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      handle_timeout_message
 *
 *  DESCRIPTION
 *      This function finds the message associated with a timeout event and
 *      calls the event handler function.
 *
 *    RETURN
 *        None
 *----------------------------------------------------------------------------*/
static void handle_timeout_message(ThreadInstanceType *thrist, CsrSchedTid id)
{
    NucleusTimedEventType *searchPointer;
    NucleusTimedEventType *previousPointer = NULL;

    for (searchPointer = thrist->timerInfo.timerListFirst; searchPointer != NULL; searchPointer = searchPointer->next) /* Search for the timed event with identifier "id" in the linked
         list of timers                                                            */
    {
        if (id == searchPointer->id) /* The timed event is found. Delete the Nucleus application timer,
             remove it from the linked list of timers, place the message into
             its final delivery queue, and free the previous allocated memory
             for the NucleusTimedEventType structure                            */
        {
            if (previousPointer == NULL)
            {
                thrist->timerInfo.timerListFirst = searchPointer->next;
            }
            else
            {
                previousPointer->next = searchPointer->next;
                if (searchPointer == thrist->timerInfo.timerListLast)
                {
                    thrist->timerInfo.timerListLast = previousPointer;
                }
            }

            thrist->currentTask = searchPointer->queue;

#ifdef CSR_LOG_ENABLE
            thrist->currentTaskLogFilter = CsrLogTaskFilterGet(thrist->currentTask);

            if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_FIRE)
            {
                CsrLogTimedEventFire(thrist->currentTask,
                    id);
            }
#endif

            searchPointer->eventFunction(searchPointer->fniarg, searchPointer->fnvarg);

#ifdef CSR_LOG_ENABLE
            if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_DONE)
            {
                CsrLogTimedEventDone(thrist->currentTask,
                    id);
            }
#endif

            thrist->currentTask = CSR_SCHED_TASK_ID;
            deleteNucleusTimer(&(searchPointer->timer));
            CsrPmemFree(searchPointer->nuTimer);
            CsrPmemFree(searchPointer);
            return;
        }
        previousPointer = searchPointer;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      create_timer
 *
 *  DESCRIPTION
 *      This function obtain a unique timer identifier, allocates a
 *      new timer structure, and creates a Nucleus timer. If the Nucleus is
 *      created the timer structure is placed on the linked timer list.
 *
 *  RETURNS
 *      CsrSchedTid - A timed event identifier, can be used in CsrSchedTimerCancel().
 *
 *  NOTICE
 *      Timers can only be created from within the scheduler!
 *----------------------------------------------------------------------------*/
static CsrSchedTid create_timer(UNSIGNED initialtime, void (*fn)(CsrUint16 mi, void *mv), CsrUint16 fniarg, void *fnvarg, CsrUint32 line, const CsrCharString *file)
{
    STATUS status;
    CsrSchedTid timerId;
    NucleusTimedEventType *timedEvent;
    NucleusTimerWrapper *nuTimer;
    CsrUint8 threadIndex;

    threadIndex = getThreadIndex();
    if (threadIndex == 0xFF)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "threadIndex == 0xFF");
    }
    timedEvent = (NucleusTimedEventType *) CsrPmemZalloc(sizeof(NucleusTimedEventType));

    timerId = (CsrSchedTid) csrSchedGetIdentifier();

    nuTimer = (NucleusTimerWrapper *) CsrPmemAlloc(sizeof(NucleusTimerWrapper));
    nuTimer->timerId = timerId;
    nuTimer->thrist = &(instance->thread[threadIndex]);

    timedEvent->id = timerId;
    timedEvent->eventFunction = fn;
    timedEvent->fniarg = fniarg;
    timedEvent->fnvarg = fnvarg;
    timedEvent->next = NULL;
    timedEvent->nuTimer = nuTimer;
    timedEvent->queue = CsrSchedTaskQueueGet();

    status = NU_Create_Timer(&(timedEvent->timer), "CsrSched", NucleusTimerExpirationRoutine, (UNSIGNED) nuTimer, initialtime, 0, NU_ENABLE_TIMER);

    if (status == NU_SUCCESS)
    {
        /* A Nucleus application timer is created with success. The
         * the new timer structure is placed on the Scheduler linked
         * list of timers. */
        if (nuTimer->thrist->timerInfo.timerListFirst == NULL)
        {
            nuTimer->thrist->timerInfo.timerListFirst = timedEvent;
            nuTimer->thrist->timerInfo.timerListLast = timedEvent;
        }
        else
        {
            nuTimer->thrist->timerInfo.timerListLast->next = timedEvent;
            nuTimer->thrist->timerInfo.timerListLast = nuTimer->thrist->timerInfo.timerListLast->next;
        }

#ifdef CSR_LOG_ENABLE
        if (nuTimer->thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_IN)
        {
            CsrLogTimedEventIn(line,
                file,
                nuTimer->thrist->currentTask,
                timerId,
                initialtime,
                fniarg,
                fnvarg);
        }
#endif
    }
    else
    {
        /* An exception has occurred, because it fails to create a new Nucleus
         * timer. Free the memory previously allocated for the new timer
         * structure and call sched_panic function */
        CsrPmemFree(timedEvent);
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_TIMED_EVENTS, "Too many timed events");
    }
    return timerId;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      do_put_message
 *
 *  DESCRIPTION
 *      Does the actual work of CsrSchedMessagePut(), and accepts the message
 *      identifier to be used in its "id" argument. CsrSchedMessagePut() allocates a
 *      new message identifier and then calls do_put_message().
 *----------------------------------------------------------------------------*/
static void do_put_message(CsrSchedQid q, CsrUint16 mi, void *mv, CsrSchedMsgId id, CsrUint32 line, const CsrCharString *file)
{
    CsrUint32 threadIndex;
    CsrUint16 qi;
    ThreadInstanceType *thrist;

    /* Get the instance data of the calling thread */
    thrist = getThreadInstance();

    /* Get segment and task index */
    threadIndex = QUEUE_EXTRACT_SEGMENT(q);
    qi = QUEUE_EXTRACT_INDEX(q);

    if (threadIndex < CSR_SCHED_MAX_SEGMENTS)
    {
        if (!instance->thread[threadIndex].inUse || (qi >= instance->thread[threadIndex].numberOfTasks))
        {
            /* Task index is out-of-bounds */
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNKNOWN_TASK, "Unknown Task");
        }

        if (&instance->thread[threadIndex].task == NU_Current_Task_Pointer())
        {
            MessageQueueEntryType *messageSearch;
            MessageQueueEntryType *message;

            /* Package the message for queue storage. */
            message = pnew(MessageQueueEntryType);
            message->next = (MessageQueueEntryType *) NULL;
            message->event = mi;
            message->sender = thrist->currentTask;
            message->message = mv;
            message->id = id;

            /* Store the message on the end of the task's message chain. */
            if (!(instance->thread[threadIndex].tasksInfo[qi].messageQueueFirst))
            {
                instance->thread[threadIndex].tasksInfo[qi].messageQueueFirst = message;
            }
            else
            {
                for (messageSearch = instance->thread[threadIndex].tasksInfo[qi].messageQueueFirst;
                     messageSearch->next;
                     messageSearch = messageSearch->next)
                {
                    /* Do nothing */
                }
                messageSearch->next = message;
            }
            instance->thread[threadIndex].pendingMessages++;

#ifdef CSR_LOG_ENABLE
            if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_PUT)
            {
                CsrLogMessagePut(line,
                    file,
                    thrist->currentTask,
                    q,
                    id,
                    mi,
                    mv);
            }
#endif
        }
        else /* destination thread is not this thread - should be another scheduler instance! */
        {
            STATUS status;
            UNSIGNED message[3];

            message[0] = q;
            message[1] = mi;
            message[2] = (UNSIGNED) mv;

#ifdef CSR_LOG_ENABLE
            if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_PUT)
            {
                CsrLogMessagePut(line,
                    file,
                    thrist->currentTask,
                    q,
                    id,
                    mi,
                    mv);
            }
#endif

            status = NU_Send_To_Queue(&instance->thread[threadIndex].taskQueue, &message[0], 3, NU_NO_SUSPEND);
            if (status != NU_SUCCESS)
            {
                CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_MESSAGES, "Too many messages");
            }
        }
    }
    else /* external task */
    {
        STATUS status;
        UNSIGNED message[3];

        if ((threadIndex != CSR_SCHED_MAX_SEGMENTS) || (qi >= MAX_NUMBER_OF_EXTERNAL_QUEUES) || (instance->externalTaskQueues[qi] == NULL))
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNKNOWN_TASK, "Unknown Task");
        }

        message[0] = q;
        message[1] = mi;
        message[2] = (UNSIGNED) mv;

        status = NU_Send_To_Queue(instance->externalTaskQueues[qi], &message[0], 3, NU_NO_SUSPEND);
        if (status != NU_SUCCESS)
        {
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_MESSAGES, "Too many messages");
        }

#ifdef CSR_LOG_ENABLE
        if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_PUT)
        {
            CsrLogMessagePut(line,
                file,
                thrist->currentTask,
                q,
                id,
                mi,
                mv);
        }
#endif
    }
}

CsrSchedBgint CsrSchedBgintReg(CsrSchedBgintHandler cb,
    void *context,
    const CsrCharString *id)
{
    OPTION oldPreempt;
    CsrUint16 vector;
    CsrUint8 threadIndex;
    CsrUint16 idx = 0;
    CsrUint16 hdlcount;

    if (cb == NULL)
    {
        /* Must provide callback function! */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "handler == NULL");
        return CSR_SCHED_BGINT_INVALID;
    }

    threadIndex = getThreadIndex();
    if (threadIndex >= CSR_SCHED_MAX_SEGMENTS)
    {
        /* Must be called from task context! */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNKNOWN_TASK, "Unknown Task");
        return CSR_SCHED_BGINT_INVALID;
    }

    hdlcount = sizeof(instance->bgIntFunctions) /
               sizeof(instance->bgIntFunctions[0]);

    oldPreempt = NU_Change_Preemption(NU_NO_PREEMPT);
    while (idx < hdlcount && instance->bgIntFunctions[idx] != NULL)
    {
        idx++;
    }

    if (idx < hdlcount)
    {
        instance->bgIntFunctions[idx] = cb;
        instance->bgIntThread[idx] = threadIndex;
        instance->bgIntTask[idx] = CsrSchedTaskQueueGet();
        instance->bgIntContext[idx] = context;

        vector = idx;
    }
    else
    {
        /* Too many bgints */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_MESSAGES, "Too many bgints");
        return CSR_SCHED_BGINT_INVALID;
    }
    NU_Change_Preemption(oldPreempt);

#ifdef CSR_LOG_ENABLE
    if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_REG))
    {
        CsrLogBgintRegister(0,
            (CsrSchedBgint) vector,
            id,
            context);
    }
#endif

    return (CsrSchedBgint) vector;
}

void CsrSchedBgintUnreg(CsrSchedBgint bgint)
{
    CsrUint16 vector = (CsrUint16) bgint;

    if ((vector <= NUMBER_OF_BG_INTS) && (vector != CSR_SCHED_BGINT_INVALID) && (instance->bgIntFunctions[vector] != NULL))
    {
#ifdef CSR_LOG_ENABLE
        if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_UNREG))
        {
            CsrLogBgintUnregister((CsrSchedBgint) vector);
        }
#endif

        instance->bgIntFunctions[vector] = NULL;
        instance->bgIntThread[vector] = 0;
        instance->bgIntContext[vector] = NULL;
    }
    else
    {
        /* Invalid bgint */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Invalid CsrSchedBgintUnreg");
    }
}

void CsrSchedBgintSet(CsrSchedBgint bgint)
{
    STATUS status;
    UNSIGNED message[3];
    CsrUint16 vector = (CsrUint16) bgint;

    if ((vector > NUMBER_OF_BG_INTS) || (vector == CSR_SCHED_BGINT_INVALID) || (instance->bgIntFunctions[vector] == NULL))
    {
        /* Invalid bgint */
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "Invalid CsrSchedBgint");
        return;
    }

    message[0] = vector;
    message[1] = BACKGROUND_IRQ;
    message[2] = 0;

    status = NU_Send_To_Queue(&instance->thread[instance->bgIntThread[vector]].taskQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);

#ifdef CSR_LOG_ENABLE
    if (!CsrLogEnvironmentIsFiltered(CSR_LOG_LEVEL_ENVIRONMENT_BGINT_SET))
    {
        CsrLogBgintSet((CsrSchedBgint) vector);
    }
#endif

    if (status != NU_SUCCESS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_MESSAGES, "Could not queue Bgint");
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
void CsrSchedRegisterTask(CsrSchedQid *queue, schedEntryFunction_t init,
    schedEntryFunction_t deinit,
    schedEntryFunction_t handler,
    const CsrCharString *taskName,
    void *data,
    CsrUint16 id)
{
    SchedulerInstanceDataType *inst;
    ThreadInstanceType *thrist;

    inst = (SchedulerInstanceDataType *) data;
    thrist = &(inst->thread[id]);

    /* Only setup tasks for current thread */
    if (inst->setupId == id)
    {
        if (thrist->init)
        {
            /* First run - assign queue and count */
            *queue = ((id << CSR_SCHED_QUEUE_SEGMENT_SHIFT) | thrist->numberOfTasks);

            thrist->numberOfTasks++;
            inst->totalTasks++;
        }
        else
        {
            /* Second run - store functions etc. */
            thrist->tasks[thrist->numberOfTasks].initFunction = init;
            thrist->tasks[thrist->numberOfTasks].deinitFunction = deinit;
            thrist->tasks[thrist->numberOfTasks].handlerFunction = handler;
            thrist->tasks[thrist->numberOfTasks].name = taskName;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            thrist->tasks[thrist->numberOfTasks].activeTaskMeasurements = NULL;
#endif
            (thrist->numberOfTasks)++;
        }
    }
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
    CsrSchedQid taskIndex;
    NucleusTimedEventType *searchPointer;
    ThreadInstanceType *thrist;
    CsrUint8 threadIndex;

    thrist = (ThreadInstanceType *) data;

    threadIndex = getThreadIndex();

    /* Empty queue */
    clear_nucleus_scheduler_queue(thrist);

    /* Let all tasks, cleanup what they know to clean up */
    /* Tasks' deinitialisation functions. */

    CSR_LOG_TEXT("-------------- CLEAN UP --------------");
    CSR_LOG_TEXT("Call task deinit function");

    for (taskIndex = 0; taskIndex < thrist->numberOfTasks; taskIndex++)
    {
        if (thrist->tasks[taskIndex].deinitFunction != NULL)
        {
            thrist->currentTask = (((CsrUint16) (threadIndex << CSR_SCHED_QUEUE_SEGMENT_SHIFT)) | taskIndex);

#ifdef CSR_LOG_ENABLE
            thrist->currentTaskLogFilter = CsrLogTaskFilterGet(thrist->currentTask);

            CsrLogDeinitTask(thrist->currentTask);
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_TASK(thrist->currentTask), thrist->tasks[taskIndex].activeTaskMeasurements);
#endif

            thrist->tasks[taskIndex].deinitFunction(&thrist->tasksInfo[taskIndex].instanceDataPointer);

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_SYSTEM(threadIndex), thrist->activeSystemMeasurements);
#endif

            thrist->currentTask = CSR_SCHED_TASK_ID;
        }
        else
        {
            /* Do nothing */
        }
    }

    /* Free all remaining timers */

    CSR_LOG_TEXT("-------------- CLEAN UP --------------");
    CSR_LOG_TEXT("Free any remaining timed events");

    searchPointer = thrist->timerInfo.timerListFirst;
    while (searchPointer)
    {
        thrist->timerInfo.timerListFirst = searchPointer->next;
        deleteNucleusTimer(&(searchPointer->timer));
        CsrPmemFree(searchPointer);
        searchPointer = thrist->timerInfo.timerListFirst;
    }

    CSR_LOG_TEXT("-------- CLEANED UP (Sched exit) --------");
}

#endif


static void nucleusSched(UNSIGNED threadIndex, void *argv)
{
    UNSIGNED nucleusMsg[3];
    UNSIGNED actualSize;
    UNSIGNED suspend;
    ThreadInstanceType *thrist;
    CsrSchedQid taskIndex;
#ifdef ENABLE_SHUTDOWN
    STATUS status;
#endif

    suspend = NU_SUSPEND;

#ifdef CSR_LOG_ENABLE
    CsrLogSchedStart(threadIndex);
#endif

    /* Run task init functions */
    thrist = &(instance->thread[threadIndex]);
    thrist->sched_running = TRUE;

    /* Tasks' initialisation functions. */
    if (thrist->numberOfTasks > 0)
    {
        for (taskIndex = 0; taskIndex < thrist->numberOfTasks; taskIndex++)
        {
            thrist->currentTask = (((CsrUint16) (threadIndex << CSR_SCHED_QUEUE_SEGMENT_SHIFT)) | taskIndex);

#ifdef CSR_LOG_ENABLE
            thrist->currentTaskLogFilter = CsrLogTaskFilterGet(thrist->currentTask);

            CsrLogInitTask(threadIndex, thrist->currentTask, thrist->tasks[taskIndex].name);
#endif
            if (thrist->tasks[taskIndex].initFunction)
            {
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_TASK(thrist->currentTask), thrist->tasks[taskIndex].activeTaskMeasurements);
#endif
                thrist->tasks[taskIndex].initFunction(&(thrist->tasksInfo[taskIndex].instanceDataPointer));
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_SYSTEM(threadIndex), thrist->activeSystemMeasurements);
#endif
            }
            thrist->currentTask = CSR_SCHED_TASK_ID;
        }
    }
    else if (threadIndex != 0) /* we'll keep instance zero around - it is the default handler for bg-ints */
    {
        thrist->inUse = FALSE;
        return;
    }

    /* Run the scheduler loop */
    taskIndex = thrist->numberOfTasks;
    while (thrist->sched_running)
    {
        if (thrist->pendingMessages > 0)
        {
            suspend = NU_NO_SUSPEND;
            do
            {
                if (++taskIndex >= thrist->numberOfTasks)
                {
                    taskIndex = 0;
                }

                if (thrist->tasksInfo[taskIndex].messageQueueFirst)
                {
                    thrist->currentTask = (((CsrUint16) (threadIndex << CSR_SCHED_QUEUE_SEGMENT_SHIFT)) | taskIndex);

#ifdef CSR_LOG_ENABLE
                    thrist->currentTaskLogFilter = CsrLogTaskFilterGet(thrist->currentTask);

                    if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TASK_SWITCH)
                    {
                        CsrLogActivate(thrist->currentTask);
                    }
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                    contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_TASK(thrist->currentTask), thrist->tasks[taskIndex].activeTaskMeasurements);
#endif

                    thrist->tasks[taskIndex].handlerFunction(&(thrist->tasksInfo[taskIndex].instanceDataPointer));

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                    contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_SYSTEM(threadIndex), thrist->activeSystemMeasurements);
#endif

#ifdef CSR_LOG_ENABLE
                    if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TASK_SWITCH)
                    {
                        CsrLogDeactivate(thrist->currentTask);
                    }
#endif
                    thrist->currentTask = CSR_SCHED_TASK_ID;
                    break;
                }
            } while (TRUE);
        }

        if (thrist->sched_running && (thrist->pendingMessages == 0))
        {
            /* Currently there is no messages on any of the Scheduler
               task queues. The Nucleus kernel is allowed to suspend
               if the Nucleus, queue dedicated for the Scheduler, is
               empty */
            suspend = NU_SUSPEND;
        }

        if (NU_Receive_From_Queue(&thrist->taskQueue, &nucleusMsg[0], NUCLEUS_MSG_SIZE, &actualSize, suspend) == NU_SUCCESS)
        {
            /* A message has been retrieved from the Scheduler's Nucleus queue */
            if (actualSize == NUCLEUS_MSG_SIZE)
            {
                /* Check which kind of message has been received */
                switch (nucleusMsg[1])
                {
                    case TIMEOUT_EVENT:
                        /* A Nucleus timer has expired */
                        handle_timeout_message(thrist, (CsrSchedTid) nucleusMsg[0]);
                        break;

                    case BACKGROUND_IRQ:
                    {
                        CsrUint16 vector = (CsrUint16) nucleusMsg[0];

                        /* A background interrupt has occurred */
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                        contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_BGINT(vector), instance->activeBgMeasurements[vector]);
#endif
                        thrist->currentTask = instance->bgIntTask[vector];

                        instance->bgIntFunctions[vector](instance->bgIntContext[vector]);

                        thrist->currentTask = CSR_SCHED_TASK_ID;

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
                        contextSwitch(thrist, CSR_SCHED_IPS_CONTEXT_SYSTEM(threadIndex), thrist->activeSystemMeasurements);
#endif
                        break;
                    }

                    case BROADCAST:
                    {
                        void *msg;
                        CsrSchedQid t, qid;
                        NucleusBroadcastWrapper *broadcastWrapper;

                        broadcastWrapper = (NucleusBroadcastWrapper *) nucleusMsg[2];

                        if (broadcastWrapper->msg_build_func)
                        {
                            for (t = 0; t < thrist->numberOfTasks; t++)
                            {
                                if (thrist->tasks[t].handlerFunction)
                                {
                                    msg = broadcastWrapper->msg_build_func(broadcastWrapper->msg_build_ptr);
                                    qid = (((CsrUint16) (threadIndex << CSR_SCHED_QUEUE_SEGMENT_SHIFT)) | t);
                                    do_put_message(qid, broadcastWrapper->mi, msg, (CsrSchedMsgId) csrSchedGetIdentifier(), __LINE__, (CsrCharString *) __FILE__);
                                }
                                else
                                {
                                    /* Do nothing */
                                }
                            }
                        }
                        else
                        {
                            /* Do nothing */
                        }
                        CsrPmemFree(broadcastWrapper);
                        break;
                    }
#ifdef ENABLE_SHUTDOWN
                    case STOP_REQ_EVENT:
                    {
                        UNSIGNED message[3] = {0};

                        /* return stop cfm and break out of scheduler loop */
                        thrist->sched_running = FALSE;

                        message[0] = threadIndex;
                        message[1] = STOP_CFM_EVENT;
                        NU_Send_To_Queue(&instance->eventQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);
                        break;
                    }
#endif
                    default:
                        /* A message from the Application task */
                        CsrSchedMessagePut(nucleusMsg[0], nucleusMsg[1], (void *) nucleusMsg[2]);
                        break;
                }
            }
            else
            {
                /* An exception has occurred because "actualSize" must
                   correspond to the the message size
                   (NUCLEUS_MSG_SIZE) defined when the queue was
                   created in the Application_Initialize function */
                CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "message size invalid");
            }
        }
    }
#ifdef ENABLE_SHUTDOWN
    /* wait for deinit req event */
    status = NU_Receive_From_Queue(&thrist->taskQueue, &nucleusMsg[0], NUCLEUS_MSG_SIZE, &actualSize, NU_SUSPEND);
    if ((status == NU_SUCCESS) && (actualSize == NUCLEUS_MSG_SIZE) && (nucleusMsg[1] == DEINIT_REQ_EVENT))
    {
        UNSIGNED message[3] = {0};

        /* deinit all tasks in this scheduler thread and
         * return deinit cfm */
        CsrSchedInstanceTaskDeinit(thrist);

        message[0] = threadIndex;
        message[1] = DEINIT_CFM_EVENT;
        NU_Send_To_Queue(&instance->eventQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);
    }
#endif
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSched
 *
 *  DESCRIPTION
 *      In the Nucleus scheduler, this function just resumes the tasks created
 *      by the CsrSchedInit() function. Then blocks until told to stop by task
 *      calling CsrSchedStop().
 *
 *----------------------------------------------------------------------------*/
void CsrSched(void *data)
{
    CsrUint16 i;
    STATUS status;
    UNSIGNED actualSize;
    UNSIGNED nucleusMsg[3];
#ifdef ENABLE_SHUTDOWN
    CsrUint16 readThreadIdVector;
#endif

    if (instance == NULL)
    {
        return;
    }

    /* Start scheduler threads */
    for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
    {
        if (instance->thread[i].inUse)
        {
            NU_Resume_Task(&instance->thread[i].task);
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
    status = NU_Receive_From_Queue(&instance->eventQueue, &nucleusMsg[0], NUCLEUS_MSG_SIZE, &actualSize, NU_SUSPEND);
#ifndef ENABLE_SHUTDOWN
    /* We should never come here if shutdown is disabled. */
    CSR_UNUSED(status);

    CSR_LOG_TEXT("Unexpected scheduler shutdown");
#else
    if ((status != NU_SUCCESS) || (actualSize != NUCLEUS_MSG_SIZE) || (nucleusMsg[1] != STOP_REQ_EVENT))
    {
        CSR_LOG_TEXT("Failure in wait for task stop request");
        return;
    }

    /* Signal all threads to stop their scheduler loop */
    for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
    {
        if (instance->thread[i].inUse)
        {
            UNSIGNED message[3] = {0};
            message[1] = STOP_REQ_EVENT;
            NU_Send_To_Queue(&instance->thread[i].taskQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);
        }
    }

    /* Block until all threads are stopping or timeout occurs */
    readThreadIdVector = 0;
    while (readThreadIdVector != instance->threadIdVector)
    {
        status = NU_Receive_From_Queue(&instance->eventQueue, &nucleusMsg[0], NUCLEUS_MSG_SIZE, &actualSize, SCHED_STOP_SUSPEND);
        if ((status == NU_SUCCESS) && (actualSize == NUCLEUS_MSG_SIZE) && (nucleusMsg[1] == STOP_CFM_EVENT))
        {
            readThreadIdVector |= (0x0001) << (CsrUint32) nucleusMsg[0];
        }
        else if (status != NU_SUCCESS)
        {
            CSR_LOG_TEXT("Failure in wait for thread stop confirm");
            return;
        }
    }

    /* Signal all threads to run their task deinit */
    for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
    {
        if (instance->thread[i].inUse)
        {
            UNSIGNED message[3] = {0};
            message[1] = DEINIT_REQ_EVENT;
            NU_Send_To_Queue(&instance->thread[i].taskQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);
        }
    }

    /* Block until all threads have completed task deinit or timeout occurs */
    readThreadIdVector = 0;
    while (readThreadIdVector != instance->threadIdVector)
    {
        status = NU_Receive_From_Queue(&instance->eventQueue, &nucleusMsg[0], NUCLEUS_MSG_SIZE, &actualSize, SCHED_STOP_SUSPEND);
        if ((status == NU_SUCCESS) && (actualSize == NUCLEUS_MSG_SIZE) && (nucleusMsg[1] == DEINIT_CFM_EVENT))
        {
            readThreadIdVector |= (0x0001) << (CsrUint32) nucleusMsg[0];
        }
        else if (status != NU_SUCCESS)
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
 *        This function initialize the Scheduler.
 *----------------------------------------------------------------------------*/
void *CsrSchedInit(CsrUint16 id, CsrUint16 priority, CsrUint32 stackSize)
{
    CsrUint16 i;
    VOID *pointer;
    char name[9];

    name[0] = 'C';
    name[1] = 'O';
    name[2] = 'A';
    name[3] = 'L';
    name[4] = hexChar[(id >> 4) & 0x000F];
    name[5] = hexChar[id & 0x000F];
    name[6] = name[7] = name[8] = '\0';

    /* Sanity check */
    if (id >= CSR_SCHED_MAX_SEGMENTS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE, "CSR_SCHED_MAX_SEGMENTS exceeded");
        return NULL;
    }

    /* Allocate global (shared) instance */
    if (instance == NULL)
    {
        char queueName[8] = "CSRQUEU";

        instance = (SchedulerInstanceDataType *) CsrPmemZalloc(sizeof(SchedulerInstanceDataType));

        /* Initialize main instance */
        instance->schedIdentifier = SCHED_ID_BASE;

        /* Initialise bg-ints */
        for (i = 0; i < NUMBER_OF_BG_INTS; i++)
        {
            instance->bgIntFunctions[i] = NULL;
            instance->bgIntThread[i] = 0;
            instance->bgIntTask[i] = CSR_SCHED_QID_INVALID;
            instance->bgIntContext[i] = NULL;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            instance->activeBgMeasurements[i] = NULL;
#endif
        }

        /* Setup thread instances */
        for (i = 0; i < CSR_SCHED_MAX_SEGMENTS; i++)
        {
            CsrMemSet(&(instance->thread[i]), 0, sizeof(ThreadInstanceType));
            instance->thread[i].init = FALSE;
            instance->thread[i].inUse = FALSE;
            instance->thread[i].currentTask = CSR_SCHED_TASK_ID;
            instance->thread[i].schedIdentifier = SCHED_ID_BASE;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            instance->thread[i].activeSystemMeasurements = NULL;
            instance->thread[i].currentContext = CSR_SCHED_IPS_CONTEXT_SYSTEM(i);
#endif
        }

        for (i = 0; i < MAX_NUMBER_OF_EXTERNAL_QUEUES; i++)
        {
            instance->externalTaskQueues[i] = NULL;
        }

		//jckim_0112
        //instance->tickInterval = NU_Retrieve_Tick_Interval();
        instance->tickInterval = 5000;

        NU_Allocate_Memory(&System_Memory, &pointer, NUCLEUS_MSG_SIZE * SCHEDULER_THREAD_MAX_QUEUE_SIZE * sizeof(UNSIGNED), NU_NO_SUSPEND);
        NU_Create_Queue(&instance->eventQueue, queueName, pointer, NUCLEUS_MSG_SIZE * SCHEDULER_THREAD_MAX_QUEUE_SIZE, NU_FIXED_SIZE, NUCLEUS_MSG_SIZE, NU_FIFO);
    }

    NU_Allocate_Memory(&System_Memory, &pointer, SCHEDULER_THREAD_STACK_SIZE, NU_NO_SUSPEND);
    NU_Create_Task(&instance->thread[id].task, name, nucleusSched, id, NU_NULL, pointer, SCHEDULER_THREAD_STACK_SIZE, CSR_SCHED_DEFAULT_PRIORITY, CSR_SCHED_DEFAULT_MAX_TICKS, NU_PREEMPT, NU_NO_START);
    name[6] = 'Q';
    NU_Allocate_Memory(&System_Memory, &pointer, NUCLEUS_MSG_SIZE * SCHEDULER_THREAD_MAX_QUEUE_SIZE * sizeof(UNSIGNED), NU_NO_SUSPEND);
    NU_Create_Queue(&instance->thread[id].taskQueue, name, pointer, NUCLEUS_MSG_SIZE * SCHEDULER_THREAD_MAX_QUEUE_SIZE, NU_FIXED_SIZE, NUCLEUS_MSG_SIZE, NU_FIFO);
    instance->thread[id].inUse = TRUE;
    instance->threadIdVector |= 0x0001 << (CsrUint16) id;

    /* Start initialisation of a single thread */
    instance->thread[id].init = TRUE;

    /* Collect the number of tasks for this thread */
    instance->setupId = id;
    CsrSchedTaskInit((void *) instance);

    /* Setup task structures for thread */
    instance->thread[id].tasks = (TaskDefinitionType *) CsrPmemAlloc(instance->thread[id].numberOfTasks * sizeof(TaskDefinitionType));
    instance->thread[id].tasksInfo = (TaskInformationType *) CsrPmemAlloc(instance->thread[id].numberOfTasks * sizeof(TaskInformationType));

    /* Run task setup once more to transfer the function pointers */
    instance->thread[id].numberOfTasks = 0;
    instance->thread[id].init = FALSE;
    CsrSchedTaskInit((void *) instance);

    instance->setupId = CSR_SCHED_TASK_ID;

#ifdef CSR_LOG_ENABLE
    CsrLogSchedInit((CsrUint8) id);
#endif

    /* Prepare queues */
    for (i = 0; i < instance->thread[id].numberOfTasks; i++)
    {
        instance->thread[id].tasksInfo[i].instanceDataPointer = NULL;
        instance->thread[id].tasksInfo[i].messageQueueFirst = NULL;
    }

    return instance;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedMessagePut
 *
 *  DESCRIPTION
 *      Sends a message consisting of the integer "mi" and the void pointer
 *      "mv" to the message queue "q".
 *
 *      "mi" and "mv" are neither inspected nor changed by the scheduler - the
 *      task that owns "q" is expected to make sense of the values. "mv" may
 *      be null.
 *
 *  NOTE
 *      If "mv" is not null then it will typically be a chunk of CsrPmemAlloc()ed
 *      memory, though there is no need for it to be so. instance->taskDef should normally
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
    CsrSchedMsgId id;
    CsrUint8 threadIndex;

#if !(defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER))
    CsrUint32 line = 0;
    const CsrCharString *file = NULL;
#endif
    threadIndex = getThreadIndex();
    if (threadIndex == 0xFF)
    {
        threadIndex = CSR_SCHED_MAX_SEGMENTS;
    }

    /* Logging is done in do_put_message */
    id = (CsrSchedMsgId) csrSchedGetIdentifier();
    do_put_message(q, mi, mv, id, line, file);
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
 *      If the calling task does not own "q" then the scheduler calls sched_panic().
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
    MessageQueueEntryType *message;
    ThreadInstanceType *thrist;
    CsrSchedQid qi;

    thrist = getThreadInstance();
    if (thrist == NULL)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNKNOWN_TASK, "Unknown Task");
    }

    qi = QUEUE_EXTRACT_INDEX(thrist->currentTask);
    message = thrist->tasksInfo[qi].messageQueueFirst;
    if (message)
    {
        /* A message is taken from the queue */
        thrist->tasksInfo[qi].messageQueueFirst = message->next;

        if (pmi)
        {
            *pmi = message->event;
        }
        if (pmv)
        {
            *pmv = message->message;
        }
        thrist->pendingMessages--;

#ifdef CSR_LOG_ENABLE
        if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_GET)
        {
            CsrLogMessageGet(message->sender,
                thrist->currentTask,
                TRUE,
                message->id,
                *pmi,
                *pmv);
        }
#endif

        CsrPmemFree(message);
        return TRUE;
    }

#ifdef CSR_LOG_ENABLE
    if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_MESSAGE_GET)
    {
        CsrLogMessageGet(0, thrist->currentTask, FALSE, 0, 0, NULL);
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
 *      CsrSchedTid - A timed event identifier, can be used in CsrSchedTimerCancel().
 *
 *  NOTICE
 *      Timers can only be created from within the scheduler!
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
    UNSIGNED numOfTicks;

#if !(defined(CSR_LOG_ENABLE) && defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER))
    CsrUint32 line = 0;
    const CsrCharString *file = NULL;
#endif

    numOfTicks = (delay / instance->tickInterval);
    if (numOfTicks == 0)
    {
        /* "delay" is smaller than one Nucleus timer tick, set delay
         * to one tick */
        numOfTicks++;
    }
    else if ((numOfTicks * instance->tickInterval) < delay)
    {
        /* Add an extra timer tick because of the rounding off error */
        numOfTicks++;
    }

    return create_timer(numOfTicks, fn, fniarg, fnvarg, line, file);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedTimerCancel
 *
 *  DESCRIPTION
 *      Attempts to prevent the timed event with identifier "eventid" from
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
    NucleusTimedEventType *searchPointer;
    NucleusTimedEventType *previousPointer = NULL;
    ThreadInstanceType *thrist;

#if defined(CSR_LOG_ENABLE) && !defined(CSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER)
    CsrUint32 line = 0;
    const CsrCharString *file = NULL;
#endif

    thrist = getThreadInstance();
    if (thrist == NULL)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNKNOWN_TASK, "Unknown Task");
    }

    if (eventid != CSR_SCHED_TID_INVALID)
    {
        /* Walk the timed events looking for the doomed event */
        for (searchPointer = thrist->timerInfo.timerListFirst; searchPointer != NULL; searchPointer = searchPointer->next)
        {
            /* Search for the timed event "eventid" on the linked list of timers */
            if (eventid == searchPointer->id)
            {
                /* The timed event is found, delete its Nucleus timer and
                 * remove the timed event from the linked list */
                deleteNucleusTimer(&(searchPointer->timer));
                if (previousPointer == NULL)
                {
                    thrist->timerInfo.timerListFirst = searchPointer->next;
                }
                else
                {
                    previousPointer->next = searchPointer->next;
                    if (searchPointer == thrist->timerInfo.timerListLast)
                    {
                        thrist->timerInfo.timerListLast = previousPointer;
                    }
                }
                if (pmi)
                {
                    *pmi = searchPointer->fniarg;
                }
                if (pmv)
                {
                    *pmv = searchPointer->fnvarg;
                }

#ifdef CSR_LOG_ENABLE
                if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_CANCEL)
                {
                    CsrLogTimedEventCancel(line, file,
                        thrist->currentTask,
                        eventid,
                        TRUE);
                }
#endif

                CsrPmemFree(searchPointer->nuTimer);
                CsrPmemFree(searchPointer);
                return TRUE;
            }
            previousPointer = searchPointer;
        }
    }

#ifdef CSR_LOG_ENABLE
    if (thrist->currentTaskLogFilter & CSR_LOG_LEVEL_TASK_TIMER_CANCEL)
    {
        CsrLogTimedEventCancel(line, file, thrist->currentTask, eventid, FALSE);
    }
#endif

    return FALSE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      NucleusTimerExpirationRoutine
 *
 *  DESCRIPTION
 *      This is the user-supply High-Level interrupt expiration routine
 *      executed each time a Nucleus timer expires. It sends a message to
 *      the Nucleus queue dedicated for the Scheduler to provide a means of
 *      allowing the foreground to provoke function calls
 *      in the background
 *
 *----------------------------------------------------------------------------*/
void NucleusTimerExpirationRoutine(UNSIGNED timerInfo)
{
    STATUS status;
    UNSIGNED message[3];
    NucleusTimerWrapper *nuTimer;

    nuTimer = (NucleusTimerWrapper *) timerInfo;

    message[0] = nuTimer->timerId;
    message[1] = TIMEOUT_EVENT;
    message[2] = 0;

    status = NU_Send_To_Queue(&nuTimer->thrist->taskQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);
    if (status != NU_SUCCESS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_MESSAGES, "Too many messages");
    }
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
    UNSIGNED threadIndex;

    for (threadIndex = 0; threadIndex < CSR_SCHED_MAX_SEGMENTS; threadIndex++)
    {
        if (instance->thread[threadIndex].inUse)
        {
            STATUS status;
            UNSIGNED message[3];
            NucleusBroadcastWrapper *broadcastWrapper;

            broadcastWrapper = (NucleusBroadcastWrapper *) CsrPmemAlloc(sizeof(NucleusBroadcastWrapper));
            broadcastWrapper->mi = mi;
            broadcastWrapper->msg_build_func = msg_build_func;
            broadcastWrapper->msg_build_ptr = msg_build_ptr;

            message[0] = 0;
            message[1] = BROADCAST;
            message[2] = (UNSIGNED) broadcastWrapper;

            status = NU_Send_To_Queue(&instance->thread[threadIndex].taskQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);

            if (status != NU_SUCCESS)
            {
                CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_MESSAGES, "Too many messages");
                break;
            }
        }
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
    STATUS status;
    UNSIGNED message[3] = {0};

    /* Set flag to interrupt the CsrSched loop and wake it if it's
     * sleeping */
    message[1] = STOP_REQ_EVENT;
    status = NU_Send_To_Queue(&instance->eventQueue, &message[0], NUCLEUS_MSG_SIZE, NU_NO_SUSPEND);
    if (status != NU_SUCCESS)
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_TOO_MANY_MESSAGES, "Too many messages");
    }
#endif
}

#ifdef ENABLE_SHUTDOWN
/*----------------------------------------------------------------------------*
 *  NAME
 *      clear_nucleus_scheduler_queue
 *
 *  DESCRIPTION
 *      Clean the Nucleus scheduler queue. This function is called automatically
 *      by the CSR scheduler deinit function
 *
 *  RETURNS
 *        void
 *
 *----------------------------------------------------------------------------*/
static void clear_nucleus_scheduler_queue(ThreadInstanceType *thrist)
{
    UNSIGNED actualSize;
    UNSIGNED nucleusMsg[3];

    while (NU_Receive_From_Queue(&thrist->taskQueue, &nucleusMsg[0], NUCLEUS_MSG_SIZE, &actualSize, NU_NO_SUSPEND) == NU_SUCCESS)
    {
        if (actualSize == NUCLEUS_MSG_SIZE)
        {
            switch (nucleusMsg[1])
            {
                /* Check which kind of message has been received */
                case TIMEOUT_EVENT:
                {
                    NucleusTimedEventType *searchPointer;
                    NucleusTimedEventType *previousPointer = NULL;

                    for (searchPointer = thrist->timerInfo.timerListFirst; searchPointer != NULL; searchPointer = searchPointer->next) /* Search for the timed event with identifier "id" in the linked
                         list of timers                                                            */
                    {
                        if (nucleusMsg[0] == searchPointer->id) /* The timed event is found. Delete the Nucleus application timer,
                             remove it from the linked list of timers, place the message into
                             its final delivery queue, and free the previous allocated memory
                             for the NucleusTimedEventType structure                            */
                        {
                            if (previousPointer == NULL)
                            {
                                thrist->timerInfo.timerListFirst = searchPointer->next;
                            }
                            else
                            {
                                previousPointer->next = searchPointer->next;
                                if (searchPointer == thrist->timerInfo.timerListLast)
                                {
                                    thrist->timerInfo.timerListLast = previousPointer;
                                }
                            }
                            deleteNucleusTimer(&(searchPointer->timer));
                            CsrPmemFree(searchPointer->nuTimer);
                            CsrPmemFree(searchPointer);
                            break;
                        }
                        previousPointer = searchPointer;
                    }
                    break;
                }

                case BACKGROUND_IRQ:
                    /* A background interrupt has occurred */
                    break;

                case BROADCAST:
                    CsrPmemFree((void *) nucleusMsg[2]);
                    break;

                case STOP_REQ_EVENT:
                    break;

                case DEINIT_REQ_EVENT:
                    break;

                default:
                    /* A message from the Application task */
                    CsrSchedMessagePut(nucleusMsg[0], nucleusMsg[1], &nucleusMsg[2]);
                    break;
            }
        }
    }
}

#endif

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
    SchedulerInstanceDataType *inst;
    inst = (SchedulerInstanceDataType *) data;

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
                CsrPmemFree(inst->thread[i].tasksInfo);
                CsrPmemFree(inst->thread[i].tasks);
#ifdef CSR_LOG_ENABLE
                CsrLogSchedDeinit(i);
#endif
            }
        }

        instance = NULL;
        CsrPmemFree(inst);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrSchedTaskQueueGet
 *
 *  DESCRIPTION
 *      Return the queue identifier for the currently running queue
 *
 *  RETURNS
 *      CsrSchedQid - The current task queue identifier, or 0xFFFF if not available.
 *
 *----------------------------------------------------------------------------*/
CsrSchedQid CsrSchedTaskQueueGet(void)
{
    ThreadInstanceType *thrist;

    thrist = getThreadInstance();

    if (thrist)
    {
        return thrist->currentTask;
    }
    else
    {
        return 0xFFFF;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      sched_register_external_queue
 *
 *  DESCRIPTION
 *      Assigns an external scheduler queue to an external application queue.
 *
 *  RETURNS
 *      CsrSchedQid - the assigned task queue identifier, or zero if no more handles.
 *
 *----------------------------------------------------------------------------*/
CsrSchedQid sched_register_external_queue(NU_QUEUE *externalTaskQueue)
{
    CsrSchedQid q;

    if (instance == NULL) /* called before first CsrSchedInit() NOT allowed! */
    {
        return (CsrSchedQid) 0;
    }
    for (q = 0; q < MAX_NUMBER_OF_EXTERNAL_QUEUES; q++)
    {
        if (instance->externalTaskQueues[q] == NULL)
        {
            instance->externalTaskQueues[q] = externalTaskQueue;
            return (CsrSchedQid) ((CSR_SCHED_MAX_SEGMENTS << CSR_SCHED_QUEUE_SEGMENT_SHIFT) | q);
        }
    }
    return (CsrSchedQid) 0;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      nucleus_sched_change_priority
 *
 *  DESCRIPTION
 *      Changes priority of a given instance of the scheduler thread.
 *
 *  RETURNS
 *      Returns TRUE if successful otherwise FALSE.
 *
 *----------------------------------------------------------------------------*/
CsrBool nucleus_sched_change_priority(CsrUint16 id, OPTION newPriority)
{
    if (instance->thread[id].inUse)
    {
        NU_Change_Priority(&instance->thread[id].task, newPriority);
        return TRUE;
    }
    return FALSE;
}

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
CsrUint32 *CsrSchedIpsInsert(CsrSchedIpsContext context, CsrUint32 *measurements)
{
    INT oldLevel;
    CsrUint8 targetThreadIndex;
    CsrUint32 *previousMeasurements;
    CsrUint32 **targetMeasurementsPtr;

    /* Handle the automatic CSR_SCHED_IPS_CONTEXT_CURRENT */
    if (context == CSR_SCHED_IPS_CONTEXT_CURRENT)
    {
        CsrUint8 threadIndex;

        /* Determine the current scheduler thread */
        threadIndex = getThreadIndex();

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

    /* Disable interrupts in case they are enabled */
    oldLevel = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS);

    /* Determine target of insert */
    switch (context >> 16)
    {
        case (CSR_SCHED_IPS_CONTEXT_SYSTEM(0) >> 16):
        {
            /* Determine scheduler thread being targeted */
            targetThreadIndex = context & 0xFFFF;

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                NU_Local_Control_Interrupts(oldLevel);
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &(instance->thread[targetThreadIndex].activeSystemMeasurements);

            break;
        }
        case (CSR_SCHED_IPS_CONTEXT_TASK(0) >> 16):
        {
            CsrUint16 q;
            CsrUint16 qi;

            /* Determine queue and index */
            q = context & 0xFFFF;
            qi = QUEUE_EXTRACT_INDEX(q);

            /* Determine scheduler thread being targeted */
            targetThreadIndex = QUEUE_EXTRACT_SEGMENT(q);

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                NU_Local_Control_Interrupts(oldLevel);
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &(instance->thread[targetThreadIndex].tasks[qi].activeTaskMeasurements);

            break;
        }
        case (CSR_SCHED_IPS_CONTEXT_BGINT(0) >> 16):
        {
            CsrSchedBgint bgint;

            bgint = context & 0xFFFF;

            if (bgint && (bgint <= NUMBER_OF_BG_INTS))
            {
                targetThreadIndex = instance->bgIntThread[bgint - 1];
            }
            else
            {
                NU_Local_Control_Interrupts(oldLevel);
                return NULL;
            }

            /* Sanity check targetThreadIndex */
            if (targetThreadIndex >= CSR_SCHED_MAX_SEGMENTS)
            {
                NU_Local_Control_Interrupts(oldLevel);
                return NULL;
            }

            /* Setup measurement pointer */
            targetMeasurementsPtr = &(instance->activeBgMeasurements[bgint - 1]);

            break;
        }
        default:
        {
            NU_Local_Control_Interrupts(oldLevel);
            return NULL;
        }
    }

    /* Substitute measurements */
    previousMeasurements = *targetMeasurementsPtr;
    *targetMeasurementsPtr = measurements;

    /* Check if replacing the currently active measurements for the targeted thread */
    if (instance->thread[targetThreadIndex].currentContext == context)
    {
        CsrIpsInsert(&(instance->thread[targetThreadIndex].task), measurements);
    }

    /* Restore interrupts to previous level */
    NU_Local_Control_Interrupts(oldLevel);

    return previousMeasurements;
}

#endif
