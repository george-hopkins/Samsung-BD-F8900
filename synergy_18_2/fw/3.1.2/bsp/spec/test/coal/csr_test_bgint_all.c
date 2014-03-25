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

static void testStop(CsrUint16 i, void *p)
{
    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
        DONE_PRIM,
        NULL);
}

void timedMessage(CsrUint16 i, void *p)
{
    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
        TEST1_PRIM,
        NULL);
}

int srv_handle_tc_bgint_all(void **gash)
{
    BgintTestInstanceData *inst;
    CsrUint16 event;
    void *msg;
    int ret;

    inst = (BgintTestInstanceData *) *gash;

    if (!CsrSchedMessageGet(&event, &msg))
    {
        test_bgint_unregister(inst);
        set_tc_verdict(TC_FAILED);
        ret = TC_FINISHED;
    }
    else if (event == DONE_PRIM)
    {
        /* All 12 messages must have been received, and
         * interrupt mask must match all 8 interrupts (8 LSB set) */
        if ((inst->count == 12) && (inst->bgint_mask == 0x00FF))
        {
            test_bgint_unregister(inst);
            set_tc_verdict(TC_PASSED);
            ret = TC_FINISHED;
        }
        else
        {
            test_bgint_unregister(inst);
            set_tc_verdict(TC_PASSED);
            ret = TC_FINISHED;
        }
    }
    else if (event == TEST1_PRIM)
    {
        inst->count++;
        ret = TC_NOT_FINISHED;
    }
    else
    {
        test_bgint_unregister(inst);
        /* Wrong message received */
        set_tc_verdict(TC_FAILED);
        ret = TC_FINISHED;
    }

    CsrPmemFree(msg);
    return ret;
}

void srv_start_tc_bgint_all(void **gash)
{
    BgintTestInstanceData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    inst->count = 0;
    inst->bgint_mask = 0;

    inst->send_task = TEST_TASK1_IFACEQUEUE;
    inst->send_prim = TEST1_PRIM;
    inst->send_data = NULL;

    if (test_bgint_register(inst) == TRUE)
    {
        /* Will send 2 messages */
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
            TEST1_PRIM,
            NULL);
        CsrSchedTimerSet(CSR_SCHED_SECOND * 2,
            timedMessage,
            0,
            NULL);

        /* Will send 8 messages */
        CsrSchedBgintSet(inst->testBgint1);
        CsrSchedBgintSet(inst->testBgint2);
        CsrSchedBgintSet(inst->testBgint3);
        CsrSchedBgintSet(inst->testBgint4);
        CsrSchedBgintSet(inst->testBgint5);
        CsrSchedBgintSet(inst->testBgint6);
        CsrSchedBgintSet(inst->testBgint7);
        CsrSchedBgintSet(inst->testBgint8);

        /* Will send 2 messages */
        CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE,
            TEST1_PRIM,
            NULL);
        CsrSchedTimerSet(CSR_SCHED_SECOND,
            timedMessage,
            0,
            NULL);

        /* Timeout */
        CsrSchedTimerSet(CSR_SCHED_SECOND * 4,
            testStop,
            DONE_PRIM,
            NULL);
    }
    else
    {
        set_tc_verdict(TC_FAILED);
    }
}
