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

#define ERROR_PRIM (TEST1_PRIM + 1)
#define DONE_PRIM (TEST1_PRIM + 1)

static void startTest(CsrUint16 i, void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    /* Prepare */
    if (!test_bgint_register(inst))
    {
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, ERROR_PRIM, NULL);
    }
    else
    {
        inst->send_task = TEST_TASK1_IFACEQUEUE;
        inst->send_prim = TEST1_PRIM;
        inst->send_data = NULL;

        inst->bgint_mask = 0;

        CsrSchedBgintSet(inst->testBgint1);
    }
}

int srv_handle_tc_bgint_nomsg_notimer(void **gash)
{
    BgintTestInstanceData *inst;
    CsrUint16 event;
    void *msg;
    int ret;

    inst = (BgintTestInstanceData *) *gash;

    ret = TC_FINISHED;

    if (!CsrSchedMessageGet(&event, &msg))
    {
        set_tc_verdict(TC_FAILED);
    }
    else if (event == TEST1_PRIM)
    {
        test_bgint_unregister(inst);

        /* Check mask - should not contain anything */
        if (inst->bgint_mask == 0x0001)
        {
            set_tc_verdict(TC_PASSED);
        }
        else
        {
            set_tc_verdict(TC_FAILED);
        }
    }
    else
    {
        /* Wrong message received */
        set_tc_verdict(TC_FAILED);
    }

    CsrPmemFree(msg);
    return ret;
}

void srv_start_tc_bgint_nomsg_notimer(void **gash)
{
    BgintTestInstanceData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    /* Delay testcase start to make sure that no
     * interrupts are pending */
    CsrSchedTimerSet(CSR_SCHED_SECOND * 4,
        startTest,
        0,
        inst);
}
