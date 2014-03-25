/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <string.h>

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_sched_test.h"

#define NUMBER_OF_MESSAGE        100

static int count1;

void srv_start_tc_put_message1(void **gash)
{
    count1 = 0;
    sendTest1Req();
}

int srv_handle_tc_put_message1(void **gash)
{
    CsrUint16 eventClass;
    void *msg;
    int ret;

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
                        set_tc_verdict(TC_PASSED);
                        ret = TC_FINISHED;
                    }
                    else
                    {
                        sendTest1Req();
                    }
                    break;
                }
                default:
                    ret = TC_FINISHED;
                    set_tc_verdict(TC_FAILED);
                    tc_log(UNKNOWN_PRIM_TYPE);
                    break;
            }
            break;
        }
        default:
            ret = TC_FINISHED;
            set_tc_verdict(TC_FAILED);
            tc_log(UNKNOWN_EVENT_CLASS);
            break;
    }
    CsrPmemFree(msg);

    return ret;
}
