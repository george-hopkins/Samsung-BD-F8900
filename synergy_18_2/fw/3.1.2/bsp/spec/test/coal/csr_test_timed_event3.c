/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include <string.h>
#include <stdio.h>

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_sched_test.h"
#include "csr_formatted_io.h"

#define TEST_CASE_HYSTERISIS        (2 * CSR_SCHED_SECOND)
#define TEST_CASE_1ST_TIMER_START   (1 * CSR_SCHED_SECOND)
#define TEST_CASE_2ND_TIMER_START   (1 * CSR_SCHED_SECOND)

#define TIMER_WRAP_LIMIT            ((CsrTime) 4289999000U)

static CsrUint32 timer_fired;

static void sched_test_timer2(CsrUint16 temp, void *instanceData)
{
    char txt[100];

    CsrTime now = CsrTimeGet(NULL);

    CsrSnprintf(txt, 100, "t2: %u\n", now);
    output(txt);

    if (++timer_fired != 2)
    {
        set_tc_verdict(TC_FAILED);
    }
    else
    {
        set_tc_verdict(TC_PASSED);
    }

    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0x0, NULL);
}

static void sched_test_timer1(CsrUint16 temp, void *instanceData)
{
    char txt[100];

    CsrTime now = CsrTimeGet(NULL);

    CsrSnprintf(txt, 100, "t1: %u\n", now);
    output(txt);

    if (++timer_fired != 1)
    {
        set_tc_verdict(TC_FAILED);
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0x0, NULL);
    }
}

static void sched_register_timers(CsrUint16 fniarg, void *fnvarg)
{
    char txt[100];

    CsrTime now = CsrTimeGet(NULL);
    CsrTime t1 = (TIMER_WRAP_LIMIT - now - TEST_CASE_1ST_TIMER_START);
    CsrTime t2 = (TIMER_WRAP_LIMIT - now + TEST_CASE_2ND_TIMER_START);

    CsrSnprintf(txt, 100, "Wrap: %u, now: %u, t1: %u, t2: %u\n", TIMER_WRAP_LIMIT, now, t1, t2);
    output(txt);

    CsrSchedTimerSet(t1, sched_test_timer1, 0, NULL);
    CsrSchedTimerSet(t2, sched_test_timer2, 0, NULL);
}

void srv_start_tc_timed_event3(void **gash)
{
    timer_fired = 0;

    /* Make sure we're scheduled 2 sec. before WRAP */
    tc_timer_must_be_before((CsrTime) TIMER_WRAP_LIMIT - TEST_CASE_HYSTERISIS,
        sched_register_timers,
        0,
        NULL);
}

int srv_handle_tc_timed_event3(void **gash)
{
    CsrUint16 event;
    void *msg;

    CsrSchedMessageGet(&event, &msg);

    return TC_FINISHED;
}
