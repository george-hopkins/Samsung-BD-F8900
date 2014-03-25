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

#define    LOOPS        4
#define    PRIORITY     2
#define    EVENTBIT_SUCCESS_MASK  ((1 << CSR_TEST_THREAD_NUM) - 1)

static void threadFunc(void *parm);

typedef struct
{
    CsrUint16       id;
    CsrEventHandle *event;
    CsrMutexHandle *mutex;
    CsrUint16      *value;     /* value protected by mutex */
} csrTestData;

int testThreadMutexLock(void)
{
    CsrResult rc;
    CsrUint16 i;
    CsrThreadHandle threads[CSR_TEST_THREAD_NUM];
    CsrUint32 eventBits = 0;
    CsrEventHandle event;
    CsrMutexHandle mutex;
    CsrUint16 value;

    rc = CsrMutexCreate(&mutex);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrMutexCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }
    rc = CsrEventCreate(&event);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    value = 0;
    printf("Creating %d threads each looping %d times\n", CSR_TEST_THREAD_NUM, LOOPS);
    for (i = 0; i < CSR_TEST_THREAD_NUM; i++)
    {
        csrTestData *td;

        td = CsrPmemAlloc(sizeof(*td));

        td->id = i;
        td->event = &event;
        td->mutex = &mutex;
        td->value = &value;

        rc = CsrThreadCreate(threadFunc, (void *) td, 0, PRIORITY, NULL, &threads[i]);
        if (rc != CSR_RESULT_SUCCESS)
        {
            printf("Error on CsrThreadCreate(), rc=%d\n", rc);
            printf("Test FAILED\n");
            return 0;
        }
        CsrThreadSleep(1);
    }
    printf("Done creating threads\n");

    rc = 0;
    while (eventBits != (CsrUint16) (EVENTBIT_SUCCESS_MASK) && rc == CSR_RESULT_SUCCESS)
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
    CsrMutexDestroy(&mutex);

    /* Check if the final value is as expected */
    if (value != (CSR_TEST_THREAD_NUM) *LOOPS)
    {
        printf("Using %d threads and each loops %d times\n", CSR_TEST_THREAD_NUM, LOOPS);
        printf("Final value must be %d instead of %d\n", (CSR_TEST_THREAD_NUM) *LOOPS, value);
        printf("Test FAILED\n");
        return 0;
    }

    printf("Final value is %d (CSR_TEST_THREAD_NUM=%d * LOOPS=%d)\n", value, CSR_TEST_THREAD_NUM, LOOPS);
    printf("Test PASSED\n");
    return 1;
}

void threadFunc(void *parm)
{
    CsrUint16 i, tmp;
    CsrResult rc;
    csrTestData *td;

    td = (csrTestData *) parm;

    /* Loopd M times to acquire the mutex, increase the value,
       and then release the mutex. */

    printf("Thread %d started\n", td->id);

    for (i = 0; i < LOOPS; i++)
    {
        rc = CsrMutexLock(td->mutex);
        if (rc != CSR_RESULT_SUCCESS)
        {
            printf("Error on CsrMutexLock(), rc=%d\n", rc);
        }

        tmp = *td->value;
        tmp = tmp + 1;
        CsrThreadSleep(1); /* delay the increasement operation */
        *td->value = tmp;
        printf("Thread %d: value: %d -> %d\n", td->id, tmp - 1, tmp);

        rc = CsrMutexUnlock(td->mutex);
        if (rc != CSR_RESULT_SUCCESS)
        {
            printf("Error on CsrMutexUnLock(), rc=%d\n", rc);
        }
        CsrThreadSleep(1);
    }
    CsrEventSet(td->event, 0x0001 << td->id);
}
