/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_sched_test.h"
#include "csr_formatted_io.h"
#include <stdio.h>

#define TIMEOUT_TIME1 100000   /* 0.1 sec. timer */
#define TIMEOUT_TIME2 200000   /* 0.2 sec. timer */
#define TIMEOUT_TIME3 300000   /* 0.3 sec. timer */
#define TIMEOUT_TIME4 400000   /* 0.4 sec. timer */
#define TIMEOUT_TIME5 500000   /* 0.5 sec. timer */
#define MAX_TIMERS    5
#define RUN_TRIGGERS  43000    /* Require this timeouts before passing */

#define TIMEOUT_DELTA    20000 /* +/- 20 ms */

#define TRIGGER_PR_PRINT 10

static CsrUint16 triggers = 0;

static int next_to_expect = 1;
static CsrTime last_t = 0xffffffff;

static char txt[1000];

static void timeout(CsrUint16 temp, void *instanceData)
{
    CsrTime t;

    t = CsrTimeGet(NULL);

    /* This will print out a lot of verbose info which we really don't need */
    if (triggers % TRIGGER_PR_PRINT == 0)
    {
        CsrSnprintf(txt, 1000, "CsrSchedTid=%u @ %10u, trigger count %u/%u\n", temp, t, triggers, RUN_TRIGGERS);
        output(txt);
    }

    if (last_t == 0xffffffff)
    {
        last_t = CsrTimeGet(NULL);
    }
    else
    {
        if (t < last_t)
        {
            if (last_t - t < 50000)
            {
                CsrSnprintf(txt, 1000, "[%s:%d] start=%u, end: %u\n", __FILE__, __LINE__, last_t, t);
                output(txt);
                set_tc_verdict(TC_FAILED);
                CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0x0, NULL);
                return;
            }
        }
        else
        {
            if (t - last_t < 50000)
            {
                CsrSnprintf(txt, 1000, "[%s:%d] start=%u, end: %u\n", __FILE__, __LINE__, last_t, t);
                output(txt);
                set_tc_verdict(TC_FAILED);
                CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0x0, NULL);
                return;
            }
        }
        last_t = CsrTimeGet(NULL);
    }

    if (temp != next_to_expect)
    {
        CsrSnprintf(txt, 1000, "timers out of order, temp = %u next=%d\n",
            temp, next_to_expect);
        output(txt);
        set_tc_verdict(TC_FAILED);
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0x0, NULL);
        return;
    }

    next_to_expect++;

    if (next_to_expect == MAX_TIMERS + 1)
    {
        if (triggers >= RUN_TRIGGERS)
        {
            set_tc_verdict(TC_PASSED);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0x0, NULL);
        }
        else
        {
            next_to_expect = 1;
            triggers++;

            CsrSchedTimerSet(TIMEOUT_TIME2, timeout, (CsrUint16) 2, NULL);
            CsrSchedTimerSet(TIMEOUT_TIME1, timeout, (CsrUint16) 1, NULL);
            CsrSchedTimerSet(TIMEOUT_TIME5, timeout, (CsrUint16) 5, NULL);
            CsrSchedTimerSet(TIMEOUT_TIME3, timeout, (CsrUint16) 3, NULL);
            CsrSchedTimerSet(TIMEOUT_TIME4, timeout, (CsrUint16) 4, NULL);
        }
    }
}

void srv_start_tc_timer_impl1(void **gash)
{
    CsrSchedTimerSet(TIMEOUT_TIME2, timeout, (CsrUint16) 2, NULL);
    CsrSchedTimerSet(TIMEOUT_TIME1, timeout, (CsrUint16) 1, NULL);
    CsrSchedTimerSet(TIMEOUT_TIME4, timeout, (CsrUint16) 4, NULL);
    CsrSchedTimerSet(TIMEOUT_TIME3, timeout, (CsrUint16) 3, NULL);
    CsrSchedTimerSet(TIMEOUT_TIME5, timeout, (CsrUint16) 5, NULL);
}

int srv_handle_tc_timer_impl1(void **gash)
{
    CsrUint16 event;
    void *msg;

    CsrSchedMessageGet(&event, &msg);

    return TC_FINISHED;
}
