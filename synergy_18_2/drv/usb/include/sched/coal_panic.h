#ifndef COAL_PANIC_H__
#define COAL_PANIC_H__
/****************************************************************************

                (c) CSR plc 2008

                All rights reserved

FILE:           coal_panic.h

DESCRIPTION:    Die noisily... Called if things have gone REALLY BAD.
                Tries to emit a deathbed confession, and then restart the (embedded)
                system, or take an exit().

REVISION:       $Revision: #2 $

****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* Reasons to be cheerless. */
enum panicid_enum
{
    PANIC_MYSTERY, /* The useless catch-all */
    PANIC_HEAP_EXHAUSTION,
    PANIC_INVALID_PFREE_POINTER,
    PANIC_NULL_TASK_HANDLER,
    PANIC_NO_SCHED_MESSAGES,
    PANIC_TOO_MANY_TASKS,
    PANIC_TOO_MANY_MESSAGES,
    PANIC_UNEXPECTED_VALUE,
    PANIC_NO_TASKS,
    PANIC_NO_TIMED_EVENTS,
    PANIC_UNKNOWN_TASK,
    PANIC_UNKNOWN_QUEUE,
    PANIC_PS_OVERREAD,
    PANIC_PS_OVERWRITE,
    PANIC_QUEUE_ACCESS_VIOLATION,
    PANIC_TOO_MANY_TIMED_EVENTS,
    PANIC_WATCHDOG_TIMEOUT,
    PANIC_INVALID_PRIGHTSIZE_POINTER,
    PANIC_UNKNOWN_TXPRIM,
    PANIC_OVERSIZE_ABCSP_PRIM,
    PANIC_INVALID_ESCO_TX_INTERVAL,
    /* L2CAP has lost track of HCI data credits. */
    PANIC_L2CAP_HCI_DATA_CREDITS_INCONSISTENT,
    _LAST_PANIC_ENTRY_
};
typedef enum panicid_enum panicid;

/*----------------------------------------------------------------------------*
 *  NAME
 *      sched_panic_setup
 *
 *  DESCRIPTION
 *       When BCHS is implemented in operating system kernels, such as Linux,
 *       we need to be able to customize what the panic-function should
 *       actually do (for example, reboot the system).
 *       This function takes a single function-pointer as argument, which
 *       will be used as the sched_panic() function instead of the default
 *       BCHS panic function.
 *
 *       Note that this function is currently only available in the Linux Kernel
 *       BCHS port.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
#ifdef __linux__
#ifdef __KERNEL__
typedef void (*bchs_panic_func_t)(panicid deathbed_confession);
extern void sched_panic_setup(bchs_panic_func_t fct);
#endif
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      sched_panic
 *
 *  DESCRIPTION
 *      Terminate the system.  Under unix this causes an error exit().  On
 *      the target this restarts the system.
 *
 *      The "deathbed_confession" gives an indication of why the system was
 *      terminated.  The unix version of sched_panic() emits a string version
 *      of this.  There may be a method of reporting this value on a target
 *      system, at least to a debugger.
 *
 *      This function doesn't return.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
extern void sched_panic(panicid deathbed_confession);


#ifdef __cplusplus
}
#endif

#endif
