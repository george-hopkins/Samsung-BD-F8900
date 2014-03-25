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

static void thirdCancelTimeout(CsrUint16 temp, void *instanceData);

static void firstCancelTimeout(CsrUint16 temp, void *instanceData)
{
    Test1InstanceDataType *test1InstanceData = instanceData;

    test1InstanceData->firstTimerCounter++;

    switch (test1InstanceData->firstTimerCounter)
    {
        case 1:
        {
            if (!CsrSchedTimerCancel(test1InstanceData->timerID3, NULL,
                    NULL))
            {
                set_tc_verdict(TC_FAILED);
                tc_log(CANCEL_TIMED_EVENT_FAIL);
                CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
            }
            else
            {
#if 0
                char verdictMsgPtr[sizeof(CANCEL_TIMER3)];
                strcpy((char *) verdictMsgPtr, CANCEL_TIMER3);
                testCaseVerboseInformation(CANCEL_TIMED_EVENT, verdictMsgPtr, sizeof(CANCEL_TIMER3));
#endif
            }
            break;
        }

        case 2:
        {
            set_tc_verdict(TC_PASSED);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
            break;
        }

        default:
            set_tc_verdict(TC_FAILED);
            tc_log(CANCEL_TIMED_EVENT_FAIL);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
            break;
    }
}

static void secondCancelTimeout(CsrUint16 temp, void *instanceData)
{
    Test1InstanceDataType *test1InstanceData = instanceData;
    test1InstanceData->secondTimerCounter++;

    switch (test1InstanceData->secondTimerCounter)
    {
        case 1:
        {
            test1InstanceData->timerID1 = CsrSchedTimerSet(4000000,
                (void (*)(CsrUint16, void *))firstCancelTimeout,
                (CsrUint16) 0,
                (void *) test1InstanceData);
            test1InstanceData->timerID2 = CsrSchedTimerSet(1000000,
                (void (*)(CsrUint16, void *))secondCancelTimeout,
                (CsrUint16) 0,
                (void *) test1InstanceData);
            test1InstanceData->timerID3 = CsrSchedTimerSet(2000000,
                (void (*)(CsrUint16, void *))thirdCancelTimeout,
                (CsrUint16) 0,
                (void *) test1InstanceData);
            break;
        }
        case 2:
        {
            if (!CsrSchedTimerCancel(test1InstanceData->timerID1, NULL, NULL))
            {
                set_tc_verdict(TC_FAILED);
                tc_log(CANCEL_TIMED_EVENT_FAIL);
                CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
            }
            else
            {
#if 0
                char verdictMsgPtr[sizeof(CANCEL_TIMER1)];
                strcpy((char *) verdictMsgPtr, CANCEL_TIMER1);
                testCaseVerboseInformation(CANCEL_TIMED_EVENT, verdictMsgPtr, sizeof(CANCEL_TIMER1));
#endif
            }
            break;
        }
        default:
            set_tc_verdict(TC_FAILED);
            tc_log(CANCEL_TIMED_EVENT_FAIL);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
            break;
    }
}

static void thirdCancelTimeout(CsrUint16 temp, void *instanceData)
{
    Test1InstanceDataType *test1InstanceData = instanceData;
    test1InstanceData->thirdTimerCounter++;

    switch (test1InstanceData->thirdTimerCounter)
    {
        case 1:
        {
            test1InstanceData->timerID1 = CsrSchedTimerSet(2000000,
                (void (*)(CsrUint16, void *))firstCancelTimeout,
                (CsrUint16) 0,
                (void *) test1InstanceData);
            test1InstanceData->timerID2 = CsrSchedTimerSet(4000000, (void (*)(CsrUint16, void *))secondCancelTimeout,
                (CsrUint16) 0,
                (void *) test1InstanceData);
            test1InstanceData->timerID3 = CsrSchedTimerSet(1000000,
                (void (*)(CsrUint16, void *))thirdCancelTimeout,
                (CsrUint16) 0,
                (void *) test1InstanceData);
            break;
        }
        case 2:
        {
            if (!CsrSchedTimerCancel(test1InstanceData->timerID2, NULL, NULL))
            {
                set_tc_verdict(TC_FAILED);
                tc_log(CANCEL_TIMED_EVENT_FAIL);
                CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
            }
            else
            {
#if 0
                char verdictMsgPtr[sizeof(CANCEL_TIMER2)];
                strcpy((char *) verdictMsgPtr, CANCEL_TIMER2);
                testCaseVerboseInformation(CANCEL_TIMED_EVENT, verdictMsgPtr, sizeof(CANCEL_TIMER2));
#endif
            }
            break;
        }
        default:
            set_tc_verdict(TC_FAILED);
            tc_log(CANCEL_TIMED_EVENT_FAIL);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
            break;
    }
}

void srv_start_tc_cancel_timed_event(void **gash)
{
    Test1InstanceDataType *test1InstanceData;

    test1InstanceData = CsrPmemZalloc(sizeof(*test1InstanceData));
    *gash = test1InstanceData;

    test1InstanceData->firstTimerCounter = 0;
    test1InstanceData->secondTimerCounter = 0;
    test1InstanceData->thirdTimerCounter = 0;

    test1InstanceData->timerID1 = CsrSchedTimerSet(1000000,
        (void (*)(CsrUint16, void *))firstCancelTimeout,
        (CsrUint16) 0,
        (void *) test1InstanceData);
    test1InstanceData->timerID2 = CsrSchedTimerSet(2000000,
        (void (*)(CsrUint16, void *))secondCancelTimeout,
        (CsrUint16) 0,
        (void *) test1InstanceData);
    test1InstanceData->timerID3 = CsrSchedTimerSet(4000000,
        (void (*)(CsrUint16, void *))thirdCancelTimeout,
        (CsrUint16) 0,
        (void *) test1InstanceData);
}

int srv_handle_tc_cancel_timed_event(void **gash)
{
    CsrUint16 event;
    void *msg;
    Test1InstanceDataType *test1InstanceData;

    CsrSchedMessageGet(&event, &msg);

    test1InstanceData = *gash;

    CsrPmemFree(test1InstanceData);

    return TC_FINISHED;
}
