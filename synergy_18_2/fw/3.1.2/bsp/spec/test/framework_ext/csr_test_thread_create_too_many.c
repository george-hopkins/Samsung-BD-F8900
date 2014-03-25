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
    CsrUint16       id;
    CsrEventHandle *event;
} csrTestData;


int testThreadCreateTooMany(void)
{
    CsrResult rc;
    CsrUint16 i;
    CsrThreadHandle threads[CSR_TEST_THREAD_NUM];
    CsrUint32 eventBits = 0;
    CsrEventHandle event;
    CsrUint8 res = 0;

    rc = CsrEventCreate(&event);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    printf("Creating %d threads\n", CSR_TEST_THREAD_NUM);
    for (i = 0; i < CSR_TEST_THREAD_NUM; i++)
    {
        csrTestData *td;

        td = CsrPmemAlloc(sizeof(*td));

        td->id = i;
        td->event = &event;

        rc = CsrThreadCreate(threadFunc, (void *) td, 0, PRIORITY, NULL, &threads[i]);
        if ((i == CSR_TEST_THREAD_NUM - 1) && (rc == CSR_FE_RESULT_NO_MORE_THREADS))
        {
            printf("Creating thread %d returned CSR_FE_RESULT_NO_MORE_THREADS\n", i);
            res = 1;
        }
        else if (rc != CSR_RESULT_SUCCESS)
        {
            printf("Creating thread %d returned %d\n", i, rc);
            printf("Test FAILED\n");
            return 0;
        }
        CsrThreadSleep(1);
    }

    printf("Done creating threads\n");
    rc = 0;
    /* eventBits = 0;*/
    while (eventBits != (CsrUint16) (EVENTBIT_SUCCESS_MASK >> res) && rc == CSR_RESULT_SUCCESS)
    {
        CsrUint32 readEventBits = 0;
        rc = CsrEventWait(&event, CSR_EVENT_WAIT_INFINITE, &readEventBits);
        eventBits |= readEventBits;
        printf("CsrEventWait return: eventBits=0x%04ulx, rc=%d\n", eventBits, rc);
    }

    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventWait, rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    CsrEventDestroy(&event);
    printf("Test PASSED\n");
    return 1;
}

void threadFunc(void *parm)
{
    csrTestData *td;

    td = (csrTestData *) parm;

    printf("Thread %d started\n", td->id);

    CsrEventSet(td->event, 0x0001 << td->id);
}
