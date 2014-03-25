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
#include "csr_formatted_io.h"

#define TIMEOUT_TIME ((CsrTime) 5000000)   /* 5 sec. timer */

static CsrTime start_time;
static CsrTime timeout_time;

static void timeout(CsrUint16 temp, void *instanceData)
{
    CsrTime diff;
    char txt[100];

    timeout_time = CsrTimeGet(NULL);

    diff = CsrTimeSub(timeout_time, start_time);

    CsrSnprintf(txt, 100, "Timeout: observed=%u, expected=%u\n", diff, TIMEOUT_TIME);
    output(txt);

    if (diff < TIMEOUT_TIME)
    {
        set_tc_verdict(TC_FAILED);
        tc_log(SCHED_FAILED);
    }
    else
    {
        set_tc_verdict(TC_PASSED);
    }

    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
}

void srv_start_tc_timed_event1(void **gash)
{
    start_time = CsrTimeGet(NULL);

    CsrSchedTimerSet(TIMEOUT_TIME, timeout, (CsrUint16) 0, NULL);
}

int srv_handle_tc_timed_event1(void **gash)
{
    CsrUint16 event;
    void *msg;

    CsrSchedMessageGet(&event, &msg);

    return TC_FINISHED;
}
