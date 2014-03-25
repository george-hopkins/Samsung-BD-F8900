#ifndef COAL_SCHED_H__
#define COAL_SCHED_H__
/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           coal_sched.h

REVISION:       $Revision: #2 $

DESCRIPTION:    The system scheduler gives structure to the bulk of the code in the
                system.   It organises the code as a set of tasks, and provides means
                for communication between the tasks.

                This is a non-preemptive system - there is no higher powered entity
                that forces one task to suspend so as to allow another to run.  Any
                time-critical code should be driven by system interrupts, which are
                of no concern to this scheduler.   This scheduler only supports code
                in the system's background, i.e.  it doesn't support the interrupts.

                Each task has two main elements: a set of message queues and a
                message handler function.   The message queues accumulate messages to
                be consumed by the message handler.   The message handler consumes
                messages from its queues and acts upon them.

                Each message has two elements: a uint16_t and a void* pointer.   Much
                of the time the uint16_t carries some form of event message and the
                void* pointer is null.   Sometimes it is convenient to have extra
                data in the void*.   The scheduler does not examine or alter
                either of the elements of each message.

                The core of the scheduler inspects each task's set of message queues
                in turn and calls the task's handler if there is any message in any
                of the task's queues.

                Key point: the message handler function must run to completion and
                return control to the scheduler.   Thus, to keep the system flowing,
                message handlers must hog the cpu for only a limited time.   In this
                respect the scheduling is similar to calling a STREAMS service
                procedure - described in:

                    cm.bell-labs.com/cm/cs/who/dmr/st.html

                The main gain of this approach, rather than a more conventional
                co-processing, non-preemptive scheduler, is that the system only
                requires one stack.  This should help constrain the ram consumption
                of this embedded application.

                This structure also appears appropriate for the current application,
                which is largely the handling of command messages which provoke
                a report or a change of configuration, or cause other elements of
                the local network of devices to do the same, thus the tasks are
                not cpu-intensive.

                A task with a heavy cpu load can timeslice its work by breaking the
                work into consecutive chunks, then performing each chunk in separate
                handler invocations.   The task might send itself a sequence of
                messages to provoke the sequence of invocations.

                A task's handler function isn't required to consume any of the
                messages pending for it.   It is also free to consume as many pending
                messages as it wishes.   Thus, if a message handler consumes no
                messages then the scheduler will call it again at the next time it
                inspects its queues.

                This scheduler uses a static list of tasks and a static set of
                message queues, so (unlike the first version of this scheduler) there
                is no need for scheduler primitives to create and destroy tasks.

                The scheduler stores a void* pointer for the internal use of each
                task.   The scheduler makes no use of the pointer except to pass it
                as an argument to each invocation of the task's message handler.
                This allows the system to maintain data between invocations of the
                handler rather than in static data.  This is expected to be of use
                only where there are multiple tasks which share a single message
                handler.

                Most tasks are expected to be structured as message-driven state
                machines.

                A task's message handler can post to any tasks' queues, but it can
                consume messages only from its own queues.

                Each task can have an initialisation function which is called exactly
                once before any task's main function is called.   The initialisation
                function is passed the same void* pointer that is passed to the main
                task function.

                The scheduling of tasks is naive: round-robin.

                All tasks run in a single address space; this isn't a real processor.

                It is intended that this scheduler's functions will be called only
                from the processor's background - in particular, communication with
                the foreground must not be via the scheduler's message queues.  (The
                scheduler code is not thread-safe - it uses no resource locks.)

                An expeditious bodge: the main body of the scheduler frequently
                examines one or more flags.   If such a flag is set the flag is
                cleared and a hard-wired function is called.   This is to allow
                the foreground to "interrupt" the background to tell it that it
                has data available to the background.   This avoids the need
                for the background to poll the foreground's data buffers.

****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* System time, in microseconds */
typedef uint32_t                TIME;

/* An identifier issued by the scheduler. */
typedef uint32_t                scheduler_identifier;

/* A task identifier */
typedef uint16_t                taskid;

/* A queue identifier */
typedef uint16_t                qid;

/* A message identifier */
typedef scheduler_identifier    msgid;

/* A timer event identifier */
typedef scheduler_identifier    tid;

/* Scheduler entry functions share this structure */
typedef void (*schedEntryFunction_t)(void **inst);

/* Time constants. */
#define COAL_TIME_MAX                ((TIME) 0xFFFFFFFF)
#define COAL_MILLISECOND             ((TIME)(1000))
#define COAL_SECOND                  ((TIME)(1000 * COAL_MILLISECOND))
#define COAL_MINUTE                  ((TIME)(60 * COAL_SECOND))

/* Max namelength of a task, and the maximum number of tasknames to
 * store within the scheduler (used for logging) */
#define MAX_TASK_NAME_LENGTH    32
#define MAX_TASK_NAME_ARRAY     50

/* Queue and primitive that identifies the environment */
#define ENVIRONMENT_TASK_ID        0xFFFF
#define ENV_PRIM                   (ENVIRONMENT_TASK_ID)
#define EXCLUDED_MODULE_QUEUE      0xFFFF

/* Queue number bit that should never leave the BlueCore */
#define SCHED_QUEUE_ON_CHIP        0x8000

/* Part of the queue-id that is used to identify a segment
 * and the number of bits to shift to obtain it */
#define SCHED_QUEUE_SEGMENT        0x7000
#define SCHED_QUEUE_SEGMENT_SHIFT  12

/* Maximum number of supported segments */
#define SCHED_MAX_SEGMENTS         4

/* Define a global task queue id number */
#define DECLARE_TASK(task) \
    uint16_t task = ENVIRONMENT_TASK_ID

#if defined(STRING_LOG__) || defined(ENABLE_MIPS_MEASURE)
#define FUNCTION_TO_STRING(function) #function
#define INIT_TASK(queue, init, deinit, handler, data, id) \
    sched_register_task(&queue, init, deinit, handler, #handler, data, id)
#else
#define INIT_TASK(queue, init, deinit, handler, data, id) \
    sched_register_task(&queue, init, deinit, handler, NULL, data, id)
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      register_bg_int
 *
 *  DESCRIPTION
 *      Register a background interrupt handler function with the scheduler.
 *      When bg_int*() is then called from the foreground (e.g. an interrupt
 *      routine) the registered function is called.
 *
 *      If "fn" is null then the interrupt is effectively disabled. If a
 *      function is already registered this function returns FALSE.
 *
 *      bgint_id identifies the bg_int* function to be registered (1-8).
 *
 *  RETURNS
 *      TRUE if function registered.
 *
 *----------------------------------------------------------------------------*/
extern bool_t register_bg_int(int bgint_id, void (* function)(void));

/*----------------------------------------------------------------------------*
 *  NAME
 *      bg_intn
 *
 *  DESCRIPTION
 *      Set background interrupt n.
 *
 *  RETURNS
 *      void.
 *
 *----------------------------------------------------------------------------*/
extern void bg_int1(void);
extern void bg_int2(void);
extern void bg_int3(void);
extern void bg_int4(void);
extern void bg_int5(void);
extern void bg_int6(void);
extern void bg_int7(void);
extern void bg_int8(void);
extern void bg_int9(void);
extern void bg_int10(void);

/*----------------------------------------------------------------------------*
 *  NAME
 *      get_time
 *
 *  DESCRIPTION
 *      Get the current system time.
 *
 *  RETURNS
 *      TIME - the current system time.
 *
 *----------------------------------------------------------------------------*/
extern TIME get_time(void);

/*----------------------------------------------------------------------------*
 *  NAME
 *      time_add
 *
 *  DESCRIPTION
 *      Add two time values. Adding the numbers can overflow the range of a
 *      TIME, so the user must be cautious.
 *
 *  RETURNS
 *      TIME - the sum of "t1" and "t2".
 *
 *----------------------------------------------------------------------------*/
#define time_add(t1, t2) ((t1) + (t2))

/*----------------------------------------------------------------------------*
 *  NAME
 *      time_sub
 *
 *  DESCRIPTION
 *      Subtract two time values. Subtracting the numbers can provoke an
 *      underflow, so the user must be cautious.
 *
 *  RETURNS
 *      TIME - "t1" - "t2".
 *
 *----------------------------------------------------------------------------*/
#define time_sub(t1, t2)    ((int32_t) (t1) - (int32_t) (t2))

/*----------------------------------------------------------------------------*
 *  NAME
 *      time_eq
 *
 *  DESCRIPTION
 *      Compare two time values.
 *
 *  RETURNS
 *      !0 if "t1" equal "t2", else 0.
 *
 *----------------------------------------------------------------------------*/
#define time_eq(t1, t2) ((t1) == (t2))

/*----------------------------------------------------------------------------*
 *  NAME
 *      time_gt
 *
 *  DESCRIPTION
 *      Compare two time values.
 *
 *  RETURNS
 *      !0 if "t1" is greater than "t2", else 0.
 *
 *----------------------------------------------------------------------------*/
#define time_gt(t1, t2) (time_sub((t1), (t2)) > 0)

/*----------------------------------------------------------------------------*
 *  NAME
 *      time_ge
 *
 *  DESCRIPTION
 *      Compare two time values.
 *
 *  RETURNS
 *      !0 if "t1" is greater than, or equal to "t2", else 0.
 *
 *----------------------------------------------------------------------------*/
#define time_ge(t1, t2) (time_sub((t1), (t2)) >= 0)

/*----------------------------------------------------------------------------*
 *  NAME
 *      time_lt
 *
 *  DESCRIPTION
 *      Compare two time values.
 *
 *  RETURNS
 *      !0 if "t1" is less than "t2", else 0.
 *
 *----------------------------------------------------------------------------*/
#define time_lt(t1, t2) (time_sub((t1), (t2)) < 0)

/*----------------------------------------------------------------------------*
 *  NAME
 *      time_le
 *
 *  DESCRIPTION
 *      Compare two time values.
 *
 *  RETURNS
 *      !0 if "t1" is less than, or equal to "t2", else 0.
 *
 *----------------------------------------------------------------------------*/
#define time_le(t1, t2) (time_sub((t1), (t2)) <= 0)

/*----------------------------------------------------------------------------*
 *  NAME
 *      put_message
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
 *      If "mv" is not null then it will typically be a chunk of pmalloc()ed
 *      memory, though there is no need for it to be so. Tasks should normally
 *      obey the convention that when a message built with pmalloc()ed memory
 *      is given to put_message() then ownership of the memory is ceded to the
 *      scheduler - and eventually to the recipient task. I.e., the receiver of
 *      the message will be expected to pfree() the message storage.
 *
 *  RETURNS
 *      msgid - message identifier, can be used with cancel_message().
 *
 *----------------------------------------------------------------------------*/
extern msgid put_message(qid q,
                         uint16_t mi,
                         void *mv);

/*----------------------------------------------------------------------------*
 *  NAME
 *      broadcast_message
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
extern void broadcast_message(uint16_t mi,
                              void *(*msg_build_func)(void*),
                              void *msg_build_ptr);

/*----------------------------------------------------------------------------*
 *  NAME
 *      get_message
 *
 *  DESCRIPTION
 *      Obtains a message from the message queue "q" if one is available. The
 *      calling task must own "q". The message consists of one or both of a
 *      uint16_t and a void *.
 *
 *      If the calling task does not own "q" then the scheduler calls panic().
 *
 *  RETURNS
 *      bool_t - TRUE if a message has been obtained from the queue, else FALSE.
 *      If a message is taken from the queue, then "*pmi" and "*pmv" are set to
 *      the "mi" and "mv" passed to put_message() respectively.
 *
 *      "pmi" and "pmv" can be null, in which case the corresponding value from
 *      them message is discarded.
 *
 *----------------------------------------------------------------------------*/
extern bool_t get_message(qid q,
                          uint16_t *pmi,
                          void **pmv);

/*----------------------------------------------------------------------------*
 *  NAME
 *      cancel_message
 *
 *  DESCRIPTION
 *      Attempts to prevent delivery of the message with message identifier
 *      "mid" which was previously sent onto message queue "q". Other arguments
 *      are as for get_message().
 *
 *      "mid" will have been returned from a call to put_message().
 *
 *  NOTES
 *      If the function returns TRUE then the caller of cancel_message() is
 *      responsible for discarding any pmalloc()ed memory thoughtfully.
 *
 *      A call to cancel_message() may fail because the target message has
 *      already been delivered.
 *
 *  RETURNS
 *      bool_t - TRUE if the message was caught in time, else FALSE.
 *
 *----------------------------------------------------------------------------*/
extern bool_t cancel_message(qid q,
                             msgid mid,
                             uint16_t *pmi,
                             void **pmv);

/*----------------------------------------------------------------------------*
 *  NAME
 *      timed_event_in
 *
 *  DESCRIPTION
 *      Causes the void function "fn" to be called with the arguments
 *      "fniarg" and "fnvarg" after "delay" has elapsed.
 *
 *      "delay" must be less than half the range of a TIME.
 *
 *      timed_event_in() does nothing with "fniarg" and "fnvarg" except
 *      deliver them via a call to "fn()".   (Unless cancel_timed_event()
 *      is used to prevent delivery.)
 *
 *  NOTE
 *      The function will be called at or after "delay"; the actual delay will
 *      depend on the timing behaviour of the scheduler's tasks.
 *
 *  RETURNS
 *      tid - A timed event identifier, can be used in cancel_timed_event().
 *
 *----------------------------------------------------------------------------*/
extern tid timed_event_in(TIME delay,
                          void (*fn) (uint16_t, void *),
                          uint16_t fniarg,
                          void *fnvarg);

/*----------------------------------------------------------------------------*
 *  NAME
 *      cancel_timed_event
 *
 *  DESCRIPTION
 *      Attempts to prevent the timed event with identifier "eventid" from
 *      occurring.
 *
 *  RETURNS
 *      bool_t - TRUE if cancelled, FALSE if the event has already occurred.
 *
 *----------------------------------------------------------------------------*/
extern bool_t cancel_timed_event(tid eventid,
                                 uint16_t *pmi,
                                 void **pmv);

/*----------------------------------------------------------------------------*
 *  NAME
 *      sched_get_task_queue
 *
 *  DESCRIPTION
 *      Return the queue identifier for the currently running queue
 *
 *  RETURNS
 *      qid - The current task queue identifier, or 0xFFFF if not available.
 *
 *----------------------------------------------------------------------------*/
extern qid sched_get_task_queue(void);

/*----------------------------------------------------------------------------*
 *  NAME
 *      csr_task_init
 *
 *  DESCRIPTION
 *      User-implemented function (usually in a file named tasks.c). The
 *      function must declare and initialise all CSR tasks that the
 *      program uses via means of DECLARE_TASK() and DEFINE_TASK().
 *      The parameter passed to this function must NOT be modified and
 *      shall be passed to the INIT_TASK() function.
 *
 *  RETURNS
 *      <void>
 *
 *----------------------------------------------------------------------------*/
extern void csr_task_init(void *data);

/*----------------------------------------------------------------------------*
 *  NAME
 *      Various platform/implementation specific functions
 *
 *  DESCRIPTION
 *      Special utility-functions for internal CSR use. CSR has implemented
 *      these functions as they fit our use of the scheduler. Customers needs
 *      may vary.
 *
 *  RETURNS
 *      ...
 *
 *----------------------------------------------------------------------------*/

/* Prepare scheduler with identifier. Returns the new scheduler
 * context. */
void *init_sched(uint16_t id);

/* Free scheduler. Parameter is the scheduler context. */
void deinit_sched(void *data);

/* Deinitialise tasks */
void sched_task_deinit(void *data);

/* Run the scheduler */
void sched(void *data);

/* Stop scheduler when it's running. NB: No context is given, as
 * function must be called from within a task. */
void sched_stop(void);

/* Register function for external communication */
void register_external_send(bool_t (*f)(qid q,uint16_t mi, void *mv));

/* Get total number of tasks for all schedulers */
uint16_t sched_number_of_tasks(void);

/* Register a task */
void sched_register_task(qid *queue,
                         schedEntryFunction_t init,
                         schedEntryFunction_t deinit,
                         schedEntryFunction_t handler,
                         char *name,
                         void *data,
                         uint16_t id);

/* Get name of a task */
const char *sched_task_name(uint16_t taskid);

#ifdef __cplusplus
}
#endif

#endif
