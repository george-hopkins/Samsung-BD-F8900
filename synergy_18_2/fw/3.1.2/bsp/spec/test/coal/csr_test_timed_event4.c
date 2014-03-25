#ifdef _WIN32
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "csr_types.h"
#include "csr_time.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_sched_test.h"
#include "csr_formatted_io.h"

#define TIMEOUT_TIME    5000000   /* 5 sec. timer */
#define TIMEOUT_DELTA     20000   /* +/- 20 ms */

#define TIMER_WRAP_LIMIT            (0xFFFFFFFF)

static CsrTime start_time;
static CsrTime timeout_time;

static void sched_test_timer1(CsrUint16 temp, void *instanceData)
{
    char txt[100];
    CsrTime diff;

    timeout_time = CsrTimeGet(NULL);

    diff = CsrTimeSub(timeout_time, start_time);

    CsrSnprintf(txt, 100, "now: %u, diff: %u\n", timeout_time, diff);
    output(txt);

    if ((diff < TIMEOUT_TIME - TIMEOUT_DELTA) ||
        (diff > TIMEOUT_TIME + TIMEOUT_DELTA))
    {
        set_tc_verdict(TC_FAILED);
    }
    else
    {
        set_tc_verdict(TC_PASSED);
    }

    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0x0, NULL);
}

static void sched_register_timer(CsrUint16 fniarg, void *fnvarg)
{
    char txt[100];

    CsrTime now = CsrTimeGet(NULL);
    CsrTime t1 = (CsrTime) (TIMER_WRAP_LIMIT - now + 1);

    CsrSnprintf(txt, 100, "Wrap: %u, now: %u, sleep: %u\n", TIMER_WRAP_LIMIT, now, t1);
    output(txt);

    Sleep(t1 / 1000);

    start_time = CsrTimeGet(NULL);

    CsrSchedTimerSet(TIMEOUT_TIME, sched_test_timer1, 0, NULL);
}

void srv_start_tc_timed_event4(void **gash)
{
    start_time = 0;
    timeout_time = 0;

    /* Fire immediately */
    CsrSchedTimerSet(0, sched_register_timer, 0, NULL);
}

int srv_handle_tc_timed_event4(void **gash)
{
    CsrUint16 event;
    void *msg;

    CsrSchedMessageGet(&event, &msg);

    return TC_FINISHED;
}

#endif
