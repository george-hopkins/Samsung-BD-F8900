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

#define NUMBER_OF_MESSAGE  100000

static CsrUint32 count1;
static CsrTime start_time;
static CsrTime end_time;

void srv_start_tc_put_message_perf(void **gash)
{
    count1 = 0;
    start_time = CsrTimeGet(NULL);
    sendTest2Req();
}

int srv_handle_tc_put_message_perf(void **gash)
{
    CsrUint16 eventClass;
    void *msg;
    int ret;
    CsrTime diff;

    ret = TC_NOT_FINISHED;

    CsrSchedMessageGet(&eventClass, &msg);

    switch (eventClass)
    {
        case TEST1_PRIM:
        {
            TestPrimType *primType;

            primType = (TestPrimType *) msg;

            switch (*primType)
            {
                case TEST_1_REQ:
                {
                    count1++;

                    if (count1 >= NUMBER_OF_MESSAGE)
                    {
                        end_time = CsrTimeGet(NULL);
                        diff = CsrTimeSub(end_time, start_time);
                        printf("Sending %u messages in %u usecs (%.0f msg/sec)\n", count1, diff, ((float) count1 / diff) * 1000000);

                        set_tc_verdict(TC_PASSED);
                        ret = TC_FINISHED;
                    }
                    else
                    {
                        sendTest2Req();
                    }
                    break;
                }
                default:
                    set_tc_verdict(TC_FAILED);
                    tc_log(UNKNOWN_PRIM_TYPE);
                    ret = TC_FINISHED;
                    break;
            }
            break;
        }

        default:
            set_tc_verdict(TC_FAILED);
            tc_log(UNKNOWN_EVENT_CLASS);
            ret = TC_FINISHED;
            break;
    }
    CsrPmemFree(msg);

    return ret;
}

int cli_handle_tc_put_message_perf(void **gash)
{
    CsrUint16 eventClass;
    void *msg;
    int ret;

    ret = TC_NOT_FINISHED;
    CsrSchedMessageGet(&eventClass, &msg);

    switch (eventClass)
    {
        case TEST2_PRIM:
        {
            TestPrimType *primType;

            primType = (TestPrimType *) msg;

            switch (*primType)
            {
                case TEST_2_REQ:
                {
                    if (count1 >= NUMBER_OF_MESSAGE - 1)
                    {
                        ret = TC_FINISHED;
                    }
                    sendTest1Req();
                    break;
                }
                default:
                    set_tc_verdict(TC_FAILED);
                    tc_log(UNKNOWN_PRIM_TYPE);
                    ret = TC_FINISHED;
                    break;
            }
            break;
        }

        default:
            set_tc_verdict(TC_FAILED);
            tc_log(UNKNOWN_EVENT_CLASS);
            ret = TC_FINISHED;
            break;
    }
    CsrPmemFree(msg);

    return ret;
}
