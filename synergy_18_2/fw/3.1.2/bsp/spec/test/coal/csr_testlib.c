/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_result.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#include "csr_sched_init.h"

#include "csr_sched_test.h"
#include "csr_sched_test_cmn.h"
#include "csr_usr_config.h"

void output(char *txt)
{
#ifdef CSR_LOG_ENABLE
    CSR_LOG_TEXT(txt);
#endif
    printf("%s", txt);
}

#define CSR_INIT_SCHED(x) (CsrSchedInit(x, CSR_THREAD_PRIORITY_NORMAL, DEFAULT_STACK_SIZE))

CsrUint16 id1 = 0;
#if CSR_SCHEDULER_INSTANCES > 1
CsrUint16 id2 = 1;
#else
CsrUint16 id2 = 0;
#endif

int number_testcases = (sizeof(ts) / sizeof(CsrCoalTcDefs)) - 1;

CsrSchedQid TEST_TASK1_IFACEQUEUE;
CsrSchedQid TEST_TASK2_IFACEQUEUE;

void CsrSchedTaskInit(void *data)
{
    CsrSchedRegisterTask(&TEST_TASK1_IFACEQUEUE, task1_init, task1_deinit, task1_handler, "TEST_TASK1", data, id1);
    CsrSchedRegisterTask(&TEST_TASK2_IFACEQUEUE, task2_init, task2_deinit, task2_handler, "TEST_TASK2", data, id2);
}

void sendTest1Req(void)
{
    Test1Req *prim;
    prim = (Test1Req *) CsrPmemAlloc(sizeof(Test1Req));
    prim->type = TEST_1_REQ;
    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, TEST1_PRIM, prim);
}

void sendTest2Req(void)
{
    Test2Req *prim;

    prim = (Test2Req *) CsrPmemAlloc(sizeof(Test2Req));
    prim->type = TEST_2_REQ;
    CsrSchedMessagePut(TEST_TASK2_IFACEQUEUE, TEST2_PRIM, prim);
}
