/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include "csr_types.h"
#include "csr_framework_ext.h"
#ifdef TARGET_OS_NUCLEUS
#include <nucleus.h>
#endif
#ifdef TARGET_OS_NT
#include <windows.h>
#endif
#ifdef TARGET_OS_LINUX
#include <time.h>
#endif

#define TEST_EVENT_WAIT_TIMEOUT_MS (1000)

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

int testEventWaitTimeout(void)
{
    CsrResult rc;
    CsrUint32 eventBits;
    CsrUint32 sleptms;
    CsrEventHandle event;

#ifdef TARGET_OS_NUCLEUS
    UNSIGNED before, after;
    UNSIGNED tickInterval;
#endif
#ifdef TARGET_OS_NT
    ULONG before, after;
#endif
#ifdef TARGET_OS_LINUX
    struct timespec before, after, temp;
#endif

    rc = CsrEventCreate(&event);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrEventCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

#ifdef TARGET_OS_NUCLEUS
    before = NU_Retrieve_Clock();
#endif
#ifdef TARGET_OS_NT
    before = GetTickCount();
#endif
#ifdef TARGET_OS_LINUX
    clock_gettime(CLOCK_REALTIME, &before);
#endif
    rc = CsrEventWait(&event, TEST_EVENT_WAIT_TIMEOUT_MS, &eventBits);
#ifdef TARGET_OS_NUCLEUS
    after = NU_Retrieve_Clock();
    tickInterval = NU_Retrieve_Tick_Interval();
    sleptms = ((after - before) * tickInterval) / 1000;
#endif
#ifdef TARGET_OS_NT
    after = GetTickCount();
    sleptms = after - before;
#endif
#ifdef TARGET_OS_LINUX
    clock_gettime(CLOCK_REALTIME, &after);
    temp = tsdiff(before, after);
    sleptms = (temp.tv_sec) * 1000 + (temp.tv_nsec / 1000000L);
#endif

    CsrEventDestroy(&event);

    if (rc != CSR_FE_RESULT_TIMEOUT)
    {
        printf("Error on CsrEventWait, rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    if (eventBits != 0)
    {
        printf("Event bits not zero, 0x%04x\n", eventBits);
        printf("Test FAILED\n");
        return 0;
    }

    if (sleptms < TEST_EVENT_WAIT_TIMEOUT_MS)
    {
        printf("Event timeout too short %u(%d)\n", sleptms, TEST_EVENT_WAIT_TIMEOUT_MS);
        printf("Test FAILED\n");
        return 0;
    }

    printf("Test PASSED\n");
    return 1;
}
