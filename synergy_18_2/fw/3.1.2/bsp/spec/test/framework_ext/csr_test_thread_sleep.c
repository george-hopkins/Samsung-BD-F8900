/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include "csr_types.h"
#include "csr_framework_ext.h"
#include "csr_util.h"

#if defined(USE_CSRTIME)
#include "csr_time.h"
#elif defined(TARGET_OS_NUCLEUS)
#include <nucleus.h>
#elif defined(TARGET_OS_NT)
#include <windows.h>
#elif defined(TARGET_OS_LINUX)
#include <time.h>
#else
#error OS not handled -- set USE_CSRTIME=1 to use CsrTimeGet()
#endif

#define    EVENT_THREADS_DONE  0x0001
#define    PRIORITY     2

static void threadFunc(void *parm);

typedef struct
{
    CsrEventHandle event;
    CsrUint16      sleepms[5];
    CsrUint32      sleptms[5];
} csrTestData;


int testThreadSleep(void)
{
    CsrResult rc;
    CsrThreadHandle thread;
    CsrUint16 i;
    CsrUint8 res = 1;
    CsrUint32 eventBits;
    csrTestData td;

    CsrMemSet(&td, 0, sizeof(td));

    td.sleepms[0] = 1;
    td.sleepms[1] = 5;
    td.sleepms[2] = 10;
    td.sleepms[3] = 12;
    td.sleepms[4] = 1000;


    rc = CsrEventCreate(&td.event);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    rc = CsrThreadCreate(threadFunc, &td, 0, PRIORITY, NULL, &thread);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrThreadCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    rc = CsrEventWait(&td.event, CSR_EVENT_WAIT_INFINITE, &eventBits);
    CsrEventDestroy(&td.event);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventWait, rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    for (i = 0; i < (sizeof(td.sleepms) / sizeof(td.sleepms[0])); i++)
    {
        printf("Sleep time[%d] = %d ms\n", i, td.sleepms[i]);
        printf("Actual slept time[%d] = %u ms\n", i, td.sleptms[i]);

        if (td.sleptms[i] < td.sleepms[i])
        {
            printf("CsrThreadSleep() did not sleep long enough\n");
            res = 0;
        }
    }

    if (res == 1)
    {
        printf("Test PASSED\n");
        return 1;
    }
    else
    {
        printf("Test FAILED\n");
        return 0;
    }
}

#ifdef TARGET_OS_LINUX
static struct timespec tsdiff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

#endif

void threadFunc(void *parm)
{
    csrTestData *td;
    CsrUint16 i;
#if defined(USE_CSRTIME)
    CsrTime before, after;
#elif defined(TARGET_OS_NUCLEUS)
    UNSIGNED before, after;
    UNSIGNED tickInterval;
#elif defined(TARGET_OS_NT)
    ULONG before, after;
#elif defined(TARGET_OS_LINUX)
    struct timespec before, after, temp;
#else
#error OS not handled -- define USE_CSRTIME to use CsrTimeGet()
#endif

    td = (csrTestData *) parm;

    for (i = 0; i < (sizeof(td->sleepms) / sizeof(td->sleepms[0])); i++)
    {
#if defined(USE_CSRTIME)
        before = CsrTimeGet(NULL);

        CsrThreadSleep(td->sleepms[i]);

        after = CsrTimeGet(NULL);

        td->sleptms[i] = CsrTimeSub(after, before) / 1000;
#elif defined(TARGET_OS_NUCLEUS)
        before = NU_Retrieve_Clock();

        CsrThreadSleep(td->sleepms[i]);

        after = NU_Retrieve_Clock();

        tickInterval = NU_Retrieve_Tick_Interval();
        td->sleptms[i] = ((after - before) * tickInterval) / 1000;
#elif defined(TARGET_OS_NT)
        before = GetTickCount();

        CsrThreadSleep(td->sleepms[i]);

        after = GetTickCount();

        td->sleptms[i] = after - before;
#elif defined(TARGET_OS_LINUX)
        clock_gettime(CLOCK_REALTIME, &before);

        CsrThreadSleep(td->sleepms[i]);

        clock_gettime(CLOCK_REALTIME, &after);

        temp = tsdiff(before, after);
        td->sleptms[i] = (temp.tv_sec) * 1000 + (temp.tv_nsec / 1000000L);
#else
#error OS not handled -- define USE_CSRTIME to use CsrTimeGet()
#endif
    }
    CsrEventSet(&td->event, EVENT_THREADS_DONE);
}
