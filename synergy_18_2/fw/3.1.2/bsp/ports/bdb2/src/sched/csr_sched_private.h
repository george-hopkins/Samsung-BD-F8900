#ifndef CSR_SCHED_PRIVATE_H__
#define CSR_SCHED_PRIVATE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_sched_ips.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Various scheduler constants */
#define TIMEOUT_EVENT                    (0x1000)
#define BACKGROUND_IRQ                   (0x1001)
#define BROADCAST                        (0x1002)
#define STOP_REQ_EVENT                   (0x1003)
#define STOP_CFM_EVENT                   (0x1004)
#define DEINIT_REQ_EVENT                 (0x1005)
#define DEINIT_CFM_EVENT                 (0x1006)
#define NUMBER_OF_BG_INTS                (10)
#define MAX_NUMBER_OF_EXTERNAL_QUEUES    10
#define SCHED_ID_BASE                    1
#define SCHED_ID_BITRANGE                29
#define SCHED_ID_WRAP                    (1 << SCHED_ID_BITRANGE)

/* Queue number bit that should never leave the BlueCore */
#define CSR_SCHED_QUEUE_ON_CHIP        0x8000

/* Part of the queue-id that is used to identify a segment
 * and the number of bits to shift to obtain it */
#define CSR_SCHED_QUEUE_SEGMENT        0x7000
#define CSR_SCHED_QUEUE_SEGMENT_SHIFT  12

void NucleusTimerExpirationRoutine(UNSIGNED id);

/* Task placeholder */
typedef struct
{
    void (*initFunction)(void **);
    void (*deinitFunction)(void **);
    void (*handlerFunction)(void **);
    const CsrCharString *name;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *activeTaskMeasurements;
#endif
} TaskDefinitionType;

/* Placeholder for message queue */
typedef struct MessageQueueEntryTag
{
    struct MessageQueueEntryTag *next;
    CsrUint16                    event;
    CsrSchedTaskId               sender;
    void                        *message;
    CsrSchedMsgId                id;
} MessageQueueEntryType;

/* Placeholder for task information */
typedef struct
{
    void                  *instanceDataPointer;
    MessageQueueEntryType *messageQueueFirst;
    MessageQueueEntryType *messageQueueLast;
} TaskInformationType;

/* Placeholder fpr timed event queue */
struct NucleusTimerWrapperTag;
typedef struct TimedEventTypeEntryTag
{
    struct TimedEventTypeEntryTag *next;
    NU_TIMER                       timer;
    void (*eventFunction)(CsrUint16 mi, void *mv);
    CsrUint16                      fniarg;
    void                          *fnvarg;
    CsrSchedTid                    id;
    struct NucleusTimerWrapperTag *nuTimer;
    CsrSchedQid                    queue;
} NucleusTimedEventType;

/* Placeholder for timer information */
typedef struct
{
    NucleusTimedEventType *timerListFirst;
    NucleusTimedEventType *timerListLast;
} TimerInformationType;

typedef struct
{
    CsrBool                init;                    /* Scheduler initialisation running */
    CsrUint16              numberOfTasks;           /* Number of tasks */
    CsrUint8               inUse;                   /* Is thread really used? */
    TaskDefinitionType    *tasks;                   /* Container for tasks */
    TaskInformationType   *tasksInfo;               /* Container for task queues */
    NucleusTimedEventType *events;                  /* Container for timed events */
    TimerInformationType   timerInfo;
    CsrSchedIdentifier     schedIdentifier;
    CsrSchedQid            currentTask;
#ifdef CSR_LOG_ENABLE
    CsrLogLevelTask currentTaskLogFilter;
#endif

    CsrSchedMsgId pendingMessages;
    CsrUint16     sched_running;
    CsrUint16     pendingEvents;                    /* Number of pending timed events */
    NU_TASK       task;
    NU_QUEUE      taskQueue;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32         *activeSystemMeasurements;
    CsrSchedIpsContext currentContext;
#endif
} ThreadInstanceType;

typedef struct NucleusTimerWrapperTag
{
    CsrSchedTid         timerId;
    ThreadInstanceType *thrist;
} NucleusTimerWrapper;

typedef struct
{
    CsrUint16 mi;
    void * (*msg_build_func)(void *);
    void *msg_build_ptr;
} NucleusBroadcastWrapper;

/* Scheduler instance data placeholder */
typedef struct
{
    CsrUint16            totalTasks;                     /* Total, global no of tasks */
    CsrUint16            runCount;                       /* Number of tasks/timers currently active */
    CsrUint16            setupId;                        /* Current thread undergoing setup */
    CsrBool              init;
    CsrSchedBgintHandler bgIntFunctions[NUMBER_OF_BG_INTS];
    CsrUint8             bgIntThread[NUMBER_OF_BG_INTS];
    CsrSchedQid          bgIntTask[NUMBER_OF_BG_INTS];
    void                *bgIntContext[NUMBER_OF_BG_INTS];
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *activeBgMeasurements[NUMBER_OF_BG_INTS];
#endif
    CsrSchedIdentifier schedIdentifier;
    ThreadInstanceType thread[CSR_SCHED_MAX_SEGMENTS];              /* Thread instances */
    NU_QUEUE          *externalTaskQueues[MAX_NUMBER_OF_EXTERNAL_QUEUES];
    UNSIGNED           tickInterval;                    /* Interval in microseconds between Nucleus PLUS timer ticks */
    CsrUint16          threadIdVector;                  /* Vector of thread ids. Bit n set means thread id n is in use */
    NU_QUEUE           eventQueue;                      /* Event queue - to signal stop of scheduler loop, and task deinit */
} SchedulerInstanceDataType;


#ifdef __cplusplus
}
#endif

#endif
