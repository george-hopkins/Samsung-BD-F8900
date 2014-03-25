/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_framework_ext.h"
#include "csr_test_common.h"

#define    PRIORITY     2
#define    EVENTBIT_SUCCESS_MASK  ((1 << CSR_TEST_THREAD_NUM) - 1)

static void threadFunc(void *parm);

typedef struct
{
    CsrEventHandle *event;
    CsrUint16       id;
} csrThreadInfo;

int testEventWait(void)
{
    CsrResult rc;
    CsrUint16 i;
    CsrThreadHandle threads[CSR_TEST_THREAD_NUM];
    CsrUint32 eventBits;
    CsrEventHandle event;


    rc = CsrEventCreate(&event);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    printf("Creating %d threads each setting an event bit\n", CSR_TEST_THREAD_NUM);
    for (i = 0; i < CSR_TEST_THREAD_NUM; i++)
    {
        csrThreadInfo *tinfo;

        tinfo = CsrPmemAlloc(sizeof(*tinfo));
        tinfo->id = i;
        tinfo->event = &event;

        rc = CsrThreadCreate(threadFunc, (void *) tinfo, 0, PRIORITY, NULL, &threads[i]);
        if (rc != CSR_RESULT_SUCCESS)
        {
            printf("Error on CsrThreadCreate(), rc=%d\n", rc);
            printf("Test FAILED\n");
            return 0;
        }
        CsrThreadSleep(1);
    }
    printf("Done creating threads - sleep enough time to allow all threads to set their events\n");
    CsrThreadSleep(1);

    rc = CsrEventWait(&event, CSR_EVENT_WAIT_INFINITE, &eventBits);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventWait, rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    CsrEventDestroy(&event);

    /* Check if the final value is as expected */
    if (eventBits != EVENTBIT_SUCCESS_MASK)
    {
        printf("Final value must be 0x%04x instead of 0x%04x\n", EVENTBIT_SUCCESS_MASK, eventBits);
        printf("Test FAILED\n");
        return 0;
    }

    printf("Final value is 0x%04x as expected\n", eventBits);
    printf("Test PASSED\n");
    return 1;
}

void threadFunc(void *parm)
{
    csrThreadInfo *tinfo;

    tinfo = (csrThreadInfo *) parm;

    printf("Thread %d started\n", tinfo->id);

    CsrEventSet(tinfo->event, 0x0001 << tinfo->id);
}
