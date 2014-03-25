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
#include "csr_framework_ext.h"

#define NUMBER_OF_MESSAGE  1000
#define    PRIORITY     2

typedef struct
{
    CsrEventHandle *event;
} csrThreadInfo;

void threadFunc(void *parm)
{
    csrThreadInfo *tinfo;
    int i = 0;

    tinfo = (csrThreadInfo *) parm;

    CsrThreadSleep(1);

    for (i = 0; i < NUMBER_OF_MESSAGE; i++)
    {
        sendTest2Req();
        CsrThreadSleep(1);
    }

    CsrEventSet(tinfo->event, 0x0001);
}

void srv_start_tc_put_message_external(void **gash)
{
    sendTest1Req();
}

int srv_handle_tc_put_message_external(void **gash)
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
                    CsrThreadHandle thread;
                    CsrResult rc;
                    csrThreadInfo *tinfo;
                    CsrEventHandle event;
                    CsrUint32 eventBits;

                    rc = CsrEventCreate(&event);

                    if (rc != CSR_RESULT_SUCCESS)
                    {
                        set_tc_verdict(TC_FAILED);
                        return TC_FINISHED;
                    }

                    tinfo = CsrPmemAlloc(sizeof(*tinfo));
                    tinfo->event = &event;

                    rc = CsrThreadCreate(threadFunc, (void *) tinfo, 0, PRIORITY, NULL, &thread);
                    if (rc != CSR_RESULT_SUCCESS)
                    {
                        set_tc_verdict(TC_FAILED);
                        return TC_FINISHED;
                    }

                    rc = CsrEventWait(&event, CSR_EVENT_WAIT_INFINITE, &eventBits);
                    if (rc != CSR_RESULT_SUCCESS)
                    {
                        set_tc_verdict(TC_FAILED);
                        return TC_FINISHED;
                    }

                    CsrEventDestroy(&event);
                    CsrPmemFree(tinfo);

                    set_tc_verdict(TC_PASSED);
                    ret = TC_FINISHED;
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

int cli_handle_tc_put_message_external(void **gash)
{
    CsrUint16 eventClass;
    void *msg;
    int ret;
    static int count = 0;

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
                    if (count == NUMBER_OF_MESSAGE - 1)
                    {
                        ret = TC_FINISHED;
                    }
                    count++;
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
