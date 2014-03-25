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

#define DONE_PRIM (TEST1_PRIM + 1)
#define BGINT_PRIM (TEST1_PRIM + 2)

static void testStop(CsrUint16 i, void *p)
{
    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
        DONE_PRIM,
        NULL);
}

int srv_handle_tc_bgint_msg_notimer(void **gash)
{
    BgintTestInstanceData *inst;
    CsrUint16 event;
    void *msg;
    int ret = TC_NOT_FINISHED;

    inst = (BgintTestInstanceData *) *gash;

    if (!CsrSchedMessageGet(&event, &msg))
    {
        test_bgint_unregister(inst);
        set_tc_verdict(TC_FAILED);
        ret = TC_FINISHED;
    }
    else if (event == DONE_PRIM)
    {
        test_bgint_unregister(inst);
        set_tc_verdict(TC_PASSED);
        ret = TC_FINISHED;
    }
    else if (event == BGINT_PRIM)
    {
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
            TEST1_PRIM,
            NULL);
    }
    else if (event == TEST1_PRIM)
    {
        inst->count++;
        if ((inst->count == 3) && !inst->stop)
        {
            inst->stop = TRUE;
            CsrSchedTimerSet(CSR_SCHED_SECOND * 4,
                testStop,
                0,
                NULL);
        }
        else if (inst->stop)
        {
            test_bgint_unregister(inst);
            set_tc_verdict(TC_FAILED);
            ret = TC_FINISHED;
        }
    }
    else
    {
        test_bgint_register(inst);
        /* Wrong message received */
        set_tc_verdict(TC_FAILED);
        ret = TC_FINISHED;
    }

    CsrPmemFree(msg);
    return ret;
}

void srv_start_tc_bgint_msg_notimer(void **gash)
{
    BgintTestInstanceData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    inst->count = 0;
    inst->stop = FALSE;
    inst->bgint_mask = 0;

    inst->send_task = TEST_TASK1_IFACEQUEUE;
    inst->send_prim = BGINT_PRIM;
    inst->send_data = NULL;

    if (test_bgint_register(inst) == TRUE)
    {
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
            TEST1_PRIM,
            NULL);
        CsrSchedBgintSet(inst->testBgint1);
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
            TEST1_PRIM,
            NULL);
    }
    else
    {
        set_tc_verdict(TC_FAILED);
    }
}
