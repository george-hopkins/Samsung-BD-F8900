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

#include "csr_pmem.h"
#include "csr_sched_test.h"
#include "csr_formatted_io.h"

static int *ts_result = NULL;

static void dump_ts_result(int i)
{
    char txt[200];

    CsrSnprintf(txt, 200, "Test case %2d: %30s : ",
        i, ts[i].tc_name);

    output(txt);

    switch (ts_result[i])
    {
        case TC_NOT_RUNNED:
            output("NOT TESTED\n");
            break;

        case TC_PASSED:
            output("PASSED\n");
            break;

        case TC_INCONCLUSIVE:
            output("INCONCLUSIVE\n");
            break;

        case TC_FAILED:
            output("FAILED\n");
            break;

        default:
            CsrSnprintf(txt, 200, "Unknown verdict result: 0x%x\n", ts_result[i]);
            output(txt);
            break;
    }
}

void dump_ts_results(void)
{
    int i;

    for (i = 0; i < number_testcases; i++)
    {
        dump_ts_result(i);
        output("\n");
    }
}

void set_tc_verdict(int result)
{
    int tc_no;

    tc_no = get_tc_no();

    if (ts_result[tc_no] < result)
    {
        ts_result[tc_no] = result;
        dump_ts_result(tc_no);
    }
}

typedef struct
{
    void (*fn)(CsrUint16, void *);
    CsrUint16 fniarg;
    void     *fnvarg;
} tc_callback_data_t;

static void tc_wait_callback(CsrUint16 fniarg, void *fnvarg)
{
    tc_callback_data_t *pdata = fnvarg;

    void (*fn)(CsrUint16, void *) = pdata->fn;
    fniarg = pdata->fniarg;
    fnvarg = pdata->fnvarg;

    fn(fniarg, fnvarg);
}

void tc_timer_must_be_before(CsrTime time, void (*fn)(CsrUint16, void *), CsrUint16 fniarg, void *fnvarg)
{
    CsrUint32 now = CsrTimeGet(NULL);

    if (now < time)
    {
        fn(fniarg, fnvarg);
    }
    else
    {
        tc_callback_data_t *pdata = pnew(tc_callback_data_t);
        pdata->fn = fn;
        pdata->fniarg = fniarg;
        pdata->fnvarg = fnvarg;

        CsrSchedTimerSet(CSR_SCHED_TIME_MAX - now + 1, tc_wait_callback, 0, pdata);
    }
}

void tc_log(int result)
{
    switch (result)
    {
        case SCHED_SUCCESS:
            output("Verdict SCHED_SUCCESS\n");
            break;
        case UNKNOWN_EVENT_CLASS:
            output("Verdict UNKNOWN_EVENT_CLASS\n");
            break;
        case UNKNOWN_PRIM_TYPE:
            output("Verdict UNKNOWN_PRIM_TYPE\n");
            break;
        case GET_MESSAGE_NOT_ALLOWED:
            output("Verdict GET_MESSAGE_NOT_ALLOWED\n");
            break;
        case CANCEL_TIMED_EVENT_FAIL:
            output("Verdict CANCEL_TIMED_EVENT_FAIL\n");
            break;
        case CANCEL_PUT_MSG_FAIL:
            output("Verdict CANCEL_PUT_MSG_FAIL\n");
            break;
        case CANCEL_PUT_MSG_IN_FAIL:
            output("Verdict CANCEL_PUT_MSG_IN_FAIL\n");
            break;
        case SCHED_FAILED:
            output("Verdict SCHED_FAILED\n");
            break;
        default:
            output("Verdict UNKNOWN ERROR\n");
            break;
    }
}

void init_ts_verdict(void)
{
    int i;

    ts_result = CsrPmemAlloc(number_testcases * sizeof(int));

    for (i = 0; i < number_testcases; i++)
    {
        ts_result[i] = TC_NOT_RUNNED;
    }
}

void deinit_ts_verdict(void)
{
    CsrPmemFree(ts_result);
}
