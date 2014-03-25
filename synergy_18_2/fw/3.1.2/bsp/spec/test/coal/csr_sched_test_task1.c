/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#if defined(__linux__) && defined(__KERNEL__)
#include <kernel.h>
#else
#include <stdio.h>
#endif

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_sched_test.h"
#include "csr_sched_init.h"
#include "csr_formatted_io.h"

typedef struct
{
    void *tc_data;
} inst_data_t;

static int testcase_number = 0;

static void start_tc(inst_data_t *inst)
{
    SCHED_START_REQ_t *msg;
    char txt[100];

    msg = CsrPmemAlloc(sizeof(SCHED_START_REQ_t));

    msg->type = 0x0000;
    msg->tc_no = testcase_number;
    CsrSchedMessagePut(TEST_TASK2_IFACEQUEUE, SCHED_START_REQ, msg);

    CsrSnprintf(txt, 100, "Running test case: %s\n", ts[testcase_number].tc_name);
    output(txt);

    ts[testcase_number].srv_tc_start(&inst->tc_data);
}

void task1_init(void **p)
{
    inst_data_t *inst;

    inst = CsrPmemAlloc(sizeof(inst_data_t));

    inst->tc_data = NULL;
    testcase_number = 0;

    start_tc(inst);

    *p = inst;
}

void task1_handler(void **p)
{
    inst_data_t *inst;
    int res;

    inst = (inst_data_t *) *p;

    res = ts[testcase_number].srv_tc_handle(&inst->tc_data);

    if (res == TC_FINISHED)
    {
        testcase_number++;

        if (ts[testcase_number].srv_tc_start == NULL)
        {
            output("Finished running test cases\n");
            dump_ts_results();
#ifndef __arm
            CsrSchedStop();
#endif
        }
        else
        {
            start_tc(inst);
        }
    }
}

void task1_deinit(void **p)
{
    CsrPmemFree(*p);
}

int get_tc_no(void)
{
    return testcase_number;
}
