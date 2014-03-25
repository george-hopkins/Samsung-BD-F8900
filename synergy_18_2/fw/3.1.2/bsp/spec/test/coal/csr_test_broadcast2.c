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

typedef struct
{
    CsrUint32 count;
    CsrBool   broadcast_received;
} CsrTestBcast2InstanceData;

static void *msg_build_function(void *arg)
{
    CsrTestBcast2InstanceData *inst;
    Test1Req *msg;

    inst = (CsrTestBcast2InstanceData *) arg;
    inst->count++;

    msg = CsrPmemAlloc(sizeof(Test1Req));
    msg->type = TEST_1_REQ;

    return (void *) msg;
}

void srv_start_broadcast2(void **gash)
{
    CsrTestBcast2InstanceData *inst;
    char *data;
    int i;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    data = CsrPmemAlloc(10);

    for (i = 0; i < 10; i++)
    {
        data[i] = i;
    }

    inst->count = 0;
    inst->broadcast_received = FALSE;

    CsrSchedMessageBroadcast(CSR_SCHED_TASK_ID, msg_build_function,
        inst);

    CsrPmemFree(data);
}

int srv_handle_broadcast2(void **gash)
{
    CsrTestBcast2InstanceData *inst;
    CsrUint16 eventClass;
    void *msg;
    int ret;

    inst = (CsrTestBcast2InstanceData *) *gash;

    ret = TC_NOT_FINISHED;

    CsrSchedMessageGet(&eventClass, &msg);

    switch (eventClass)
    {
        case CSR_SCHED_TASK_ID:
            inst->broadcast_received = TRUE;
            break;

        case TEST1_PRIM:
        {
            TestPrimType *primType;

            primType = (TestPrimType *) msg;

            switch (*primType)
            {
                case TEST_1_REQ:
                {
                    if ((inst->count == 2) &&
                        (inst->broadcast_received == TRUE))
                    {
                        set_tc_verdict(TC_PASSED);
                        ret = TC_FINISHED;
                    }
                    else
                    {
                        set_tc_verdict(TC_FAILED);
                        ret = TC_FINISHED;
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

int cli_handle_broadcast2(void **gash)
{
    CsrUint16 eventClass;
    void *msg;
    int ret;

    ret = TC_NOT_FINISHED;
    CsrSchedMessageGet(&eventClass, &msg);

    switch (eventClass)
    {
        case CSR_SCHED_TASK_ID:
            sendTest1Req();
            set_tc_verdict(TC_PASSED);
            ret = TC_FINISHED;
            break;

        default:
            set_tc_verdict(TC_FAILED);
            tc_log(UNKNOWN_EVENT_CLASS);
            ret = TC_FINISHED;
            break;
    }
    CsrPmemFree(msg);

    return ret;
}
