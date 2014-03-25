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

#define NUMBER_OF_TIMES_OUT    10

static int firstTimerCounter = 0;
static int secondTimerCounter = 0;
static int thirdTimerCounter = 0;
static int supervisorCounter = 0;

static void firstTimeout(CsrUint16 temp, void *instanceData)
{
    firstTimerCounter++;

    if (firstTimerCounter == NUMBER_OF_TIMES_OUT)
    {
        supervisorCounter++;

        if (supervisorCounter == 3)
        {
            set_tc_verdict(TC_PASSED);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
        }
    }
    else
    {
        char *verdictMsgPtr;
        CsrUint16 verdictMsgLength;

        if (firstTimerCounter == (NUMBER_OF_TIMES_OUT - 1))
        {
            verdictMsgLength = (CsrUint16) (strlen(TIMER1_FINISH));
            verdictMsgPtr = (char *) CsrPmemAlloc(verdictMsgLength + 1);
            strcpy((char *) verdictMsgPtr, TIMER1_FINISH);
        }
        else
        {
            verdictMsgLength = (CsrUint16) (strlen(TIMER1_TIMEOUT));
            verdictMsgPtr = (char *) CsrPmemAlloc(verdictMsgLength + 1);
            strcpy((char *) verdictMsgPtr, TIMER1_TIMEOUT);
        }
        CsrSchedTimerSet(1000000, (void (*)(CsrUint16, void *))firstTimeout,
            (CsrUint16) verdictMsgLength, (void *) verdictMsgPtr);
    }
    CsrPmemFree(instanceData);
}

static void secondTimeout(CsrUint16 temp, void *instanceData)
{
    secondTimerCounter++;

    if (secondTimerCounter == NUMBER_OF_TIMES_OUT)
    {
        supervisorCounter++;

        if (supervisorCounter == 3)
        {
            set_tc_verdict(TC_PASSED);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
        }
    }
    else
    {
        char *verdictMsgPtr;
        CsrUint16 verdictMsgLength;

        if (secondTimerCounter == (NUMBER_OF_TIMES_OUT - 1))
        {
            verdictMsgLength = (CsrUint16) (strlen(TIMER2_FINISH));
            verdictMsgPtr = (char *) CsrPmemAlloc(verdictMsgLength + 1);
            strcpy((char *) verdictMsgPtr, TIMER2_FINISH);
        }
        else
        {
            verdictMsgLength = (CsrUint16) (strlen(TIMER2_TIMEOUT));
            verdictMsgPtr = (char *) CsrPmemAlloc(verdictMsgLength + 1);
            strcpy((char *) verdictMsgPtr, TIMER2_TIMEOUT);
        }
        CsrSchedTimerSet(1000000,
            (void (*)(CsrUint16, void *))secondTimeout,
            (CsrUint16) verdictMsgLength,
            (void *) verdictMsgPtr);
    }
    CsrPmemFree(instanceData);
}

static void thirdTimeout(CsrUint16 temp, void *instanceData)
{
    thirdTimerCounter++;

    if (thirdTimerCounter == NUMBER_OF_TIMES_OUT)
    {
        supervisorCounter++;

        if (supervisorCounter == 3)
        {
            set_tc_verdict(TC_PASSED);
            CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, 0, NULL);
        }
    }
    else
    {
        char *verdictMsgPtr;
        CsrUint16 verdictMsgLength;

        if (thirdTimerCounter == (NUMBER_OF_TIMES_OUT - 1))
        {
            verdictMsgLength = (CsrUint16) (strlen(TIMER3_FINISH));
            verdictMsgPtr = (char *) CsrPmemAlloc(verdictMsgLength + 1);
            strcpy((char *) verdictMsgPtr, TIMER3_FINISH);
        }
        else
        {
            verdictMsgLength = (CsrUint16) (strlen(TIMER3_TIMEOUT));
            verdictMsgPtr = (char *) CsrPmemAlloc(verdictMsgLength + 1);
            strcpy((char *) verdictMsgPtr, TIMER3_TIMEOUT);
        }
        CsrSchedTimerSet(1000000,
            (void (*)(CsrUint16, void *))thirdTimeout,
            (CsrUint16) verdictMsgLength,
            (void *) verdictMsgPtr);
    }
    CsrPmemFree(instanceData);
}

void srv_start_tc_timed_event2(void **gash)
{
    char verdictMsgPtr[sizeof(PLEASE_WAIT)];
    char *verdictMsgPtr1;
    char *verdictMsgPtr2;
    char *verdictMsgPtr3;
    CsrUint16 verdictMsgLength1 = (CsrUint16) (strlen(TIMER1_TIMEOUT));
    CsrUint16 verdictMsgLength2 = (CsrUint16) (strlen(TIMER2_TIMEOUT));
    CsrUint16 verdictMsgLength3 = (CsrUint16) (strlen(TIMER3_TIMEOUT));

    firstTimerCounter = 0;
    secondTimerCounter = 0;
    thirdTimerCounter = 0;
    supervisorCounter = 0;
    verdictMsgPtr1 = (char *) CsrPmemAlloc(verdictMsgLength1 + 1);
    verdictMsgPtr2 = (char *) CsrPmemAlloc(verdictMsgLength2 + 1);
    verdictMsgPtr3 = (char *) CsrPmemAlloc(verdictMsgLength3 + 1);

    strcpy((char *) verdictMsgPtr1, TIMER1_TIMEOUT);
    strcpy((char *) verdictMsgPtr2, TIMER2_TIMEOUT);
    strcpy((char *) verdictMsgPtr3, TIMER3_TIMEOUT);

    CsrSchedTimerSet(1000000,
        (void (*)(CsrUint16, void *))firstTimeout,
        (CsrUint16) verdictMsgLength1,
        (void *) verdictMsgPtr1);
    CsrSchedTimerSet(1000000,
        (void (*)(CsrUint16, void *))secondTimeout,
        (CsrUint16) verdictMsgLength2,
        (void *) verdictMsgPtr2);
    CsrSchedTimerSet(1000000,
        (void (*)(CsrUint16, void *))thirdTimeout,
        (CsrUint16) verdictMsgLength3,
        (void *) verdictMsgPtr3);

    strcpy((char *) verdictMsgPtr, PLEASE_WAIT);
}

int srv_handle_tc_timed_event2(void **gash)
{
    CsrUint16 event;
    void *msg;

    CsrSchedMessageGet(&event, &msg);

    return TC_FINISHED;
}
