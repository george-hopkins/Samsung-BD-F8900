/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <string.h>
#include <stdio.h>
#include "csr_sched.h"
#include "csr_time.h"
#include "csr_pmem.h"
#include "csr_sched_test.h"

#define NUMBER_OF_TIMERS  10000000

#define TIMEOUT_TIME ((CsrTime) 5000000)   /* 5 sec. timer */

static void timeout(CsrUint16 unused1, void *unused2)
{
}

void srv_start_tc_timer_perf(void **gash)
{
    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
}

int srv_handle_tc_timer_perf(void **gash)
{
    void *msg;
    CsrUint16 event;

    CsrUint32 counter;
    CsrTime t1, t2, dT;
    CsrSchedTid timer;

    CsrSchedMessageGet(&event, &msg);

    t1 = CsrTimeGet(NULL);

    for (counter = 0;
         counter < NUMBER_OF_TIMERS;
         counter++)
    {
        timer = CsrSchedTimerSet(TIMEOUT_TIME, timeout, 0, NULL);
        CsrSchedTimerCancel(timer, NULL, NULL);
    }

    t2 = CsrTimeGet(NULL);
    dT = CsrTimeSub(t2, t1);

    printf("Started and stopped %u timers in %u usecs (%.0f timers/sec)\n",
        counter, dT, ((float) counter / dT) * 1000000);

    set_tc_verdict(TC_PASSED);

    return TC_FINISHED;
}
