/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include "csr_types.h"
#include "csr_framework_ext.h"

#define    PRIORITY     2

static void threadFunc(void *parm);

typedef struct
{
    CsrUint16       id;
    CsrThreadHandle thread;
    CsrMutexHandle  mutex;
    CsrEventHandle  event;
    CsrUint32       eventMask;
    CsrUint16       res;
} csrTestData;

int testThreadGetHandle(void)
{
    CsrResult rc;
    csrTestData td;

    td.id = 0;
    CsrMutexCreate(&td.mutex);
    CsrEventCreate(&td.event);

    CsrMutexLock(&td.mutex);
    rc = CsrThreadCreate(threadFunc, (void *) &td, 0, PRIORITY, NULL, &td.thread);
    CsrMutexUnlock(&td.mutex);


    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrThreadCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    CsrEventWait(&td.event, CSR_EVENT_WAIT_INFINITE, &td.eventMask);
    CsrEventDestroy(&td.event);
    CsrMutexDestroy(&td.mutex);

    return td.res;
}

void threadFunc(void *parm)
{
    CsrResult rc;
    CsrThreadHandle gethdl;
    csrTestData *td;

    td = (csrTestData *) parm;

    CsrMutexLock(&td->mutex);

    printf("Thread %d started\n", td->id);

    rc = CsrThreadGetHandle(&gethdl);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrThreadGetHandle(), rc=%d", rc);
        printf("Test FAILED\n");
        td->res = 0;

        CsrEventSet(&td->event, 1);

        return;
    }

    if (CsrThreadEqual(&td->thread, &gethdl) != CSR_RESULT_SUCCESS)
    {
        printf("Handle mismatch: hdl=%p, gethdl=%p\n", td->thread, gethdl);
        printf("Test FAILED\n");
        td->res = 0;

        CsrEventSet(&td->event, 1);

        return;
    }

    printf("Handle match: hdl=%p, gethdl=%p\n", td->thread, gethdl);
    printf("Test PASSED\n");

    td->res = 1;

    CsrEventSet(&td->event, 1);
}
