/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_sched.h"
#include "csr_macro.h"
#include "csr_h4ds.h"
#include "csr_h4ds_time.h"
#include "csr_h4ds_inst.h"

extern H4DSInstanceData CsrH4DSInst;

static void tsync_timed_event_local(CsrUint16 tmp, void *tmp2)
{
    /* Reset timer id to avoid timers being cancelled later */
    CsrH4DSInst.tsync_tid = CSR_SCHED_TID_INVALID;
    CsrH4DSTsyncTimedEvent();
}

static void tconf_timed_event_local(CsrUint16 tmp, void *tmp2)
{
    /* Reset timer id to avoid timers being cancelled later */
    CsrH4DSInst.tconf_tid = CSR_SCHED_TID_INVALID;
    CsrH4DSTconfTimedEvent();
}

static void twu_timed_event_local(CsrUint16 tmp, void *tmp2)
{
    /* Reset timer id to avoid timers being cancelled later */
    CsrH4DSInst.twu_tid = CSR_SCHED_TID_INVALID;
    CsrH4DSTwuTimedEvent();
}

static void tflush_timed_event_local(CsrUint16 tmp, void *tmp2)
{
    /* Reset timer id to avoid timers being cancelled later */
    CsrH4DSInst.tflush_tid = CSR_SCHED_TID_INVALID;
    CsrH4DSTflushTimedEvent();
}

static void uart_idle_timed_event_local(CsrUint16 tmp, void *tmp2)
{
    /* Reset timer id to avoid timers being cancelled later */
    CsrH4DSInst.uart_idle_tid = CSR_SCHED_TID_INVALID;
    CsrH4DSUartIdleTimedEvent();
}

void CsrH4DSCancelTsyncTimer(void)
{
    CsrSchedTimerCancel(CsrH4DSInst.tsync_tid, 0, NULL);
    CsrH4DSInst.tsync_tid = CSR_SCHED_TID_INVALID;
}

void CsrH4DSStartTsyncTimer(void)
{
    CsrH4DSCancelTsyncTimer();
    CsrH4DSInst.tsync_tid = CsrSchedTimerSet((CsrTime) TSYNC_TIME, tsync_timed_event_local, 0, (void *) (NULL));
}

void CsrH4DSCancelTconfTimer(void)
{
    CsrSchedTimerCancel(CsrH4DSInst.tconf_tid, 0, NULL);
    CsrH4DSInst.tconf_tid = CSR_SCHED_TID_INVALID;
}

void CsrH4DSStartTconfTimer(void)
{
    CsrH4DSCancelTconfTimer();
    CsrH4DSInst.tconf_tid = CsrSchedTimerSet((CsrTime) TCONF_TIME, tconf_timed_event_local, 0, (void *) (NULL));
}

void CsrH4DSCancelTwuTimer(void)
{
    CsrSchedTimerCancel(CsrH4DSInst.twu_tid, 0, NULL);
    CsrH4DSInst.twu_tid = CSR_SCHED_TID_INVALID;
}

void CsrH4DSStartTwuTimer(void)
{
    CsrH4DSCancelTwuTimer();
    CsrH4DSInst.twu_tid = CsrSchedTimerSet((CsrTime) TWU_TIME, twu_timed_event_local, 0, (void *) (NULL));
}

void CsrH4DSCancelTflushTimer(void)
{
    CsrSchedTimerCancel(CsrH4DSInst.tflush_tid, 0, NULL);
    CsrH4DSInst.tflush_tid = CSR_SCHED_TID_INVALID;
}

void CsrH4DSStartTflushTimer(void)
{
    CsrH4DSCancelTflushTimer();
    CsrH4DSInst.tflush_tid = CsrSchedTimerSet((CsrTime) TFLUSH_TIME, tflush_timed_event_local, 0, (void *) (NULL));
}

void CsrH4DSCancelUartIdleTimer(void)
{
    CsrSchedTimerCancel(CsrH4DSInst.uart_idle_tid, 0, NULL);
    CsrH4DSInst.uart_idle_tid = CSR_SCHED_TID_INVALID;
}

void CsrH4DSStartUartIdleTimer(void)
{
    CsrH4DSCancelUartIdleTimer();
    CsrH4DSInst.uart_idle_tid = CsrSchedTimerSet((CsrTime) TUART_IDLE_TIME, uart_idle_timed_event_local, 0, (void *) (NULL));
}
