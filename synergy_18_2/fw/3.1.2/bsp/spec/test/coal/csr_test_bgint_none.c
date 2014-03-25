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

void testStop(CsrUint16 i, void *p)
{
    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, TEST1_PRIM, NULL);
}

int srv_handle_tc_bgint_none(void **gash)
{
    BgintTestInstanceData *inst;
    CsrUint16 event;
    void *msg;
    CsrSchedMessageGet(&event, &msg);

    inst = (BgintTestInstanceData *) *gash;

    if (event == TEST1_PRIM)
    {
        /* Check mask - should not contain anything */
        if (inst->bgint_mask == 0)
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
        set_tc_verdict(TC_FAILED);
    }

    CsrPmemFree(msg);
    return TC_FINISHED;
}

void srv_start_tc_bgint_none(void **gash)
{
    BgintTestInstanceData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    /* Prepare */
    inst->send_task = TEST_TASK1_IFACEQUEUE;
    inst->send_prim = ERROR_PRIM;
    inst->send_data = NULL;

    inst->bgint_mask = 0;

    /* Raise all background interrupts */
    CsrSchedBgintSet(inst->testBgint1);
    CsrSchedBgintSet(inst->testBgint2);
    CsrSchedBgintSet(inst->testBgint3);
    CsrSchedBgintSet(inst->testBgint4);
    CsrSchedBgintSet(inst->testBgint5);
    CsrSchedBgintSet(inst->testBgint6);
    CsrSchedBgintSet(inst->testBgint7);
    CsrSchedBgintSet(inst->testBgint8);

    /* Test stop timeout */
    CsrSchedTimerSet(CSR_SCHED_SECOND * 4,
        testStop,
        0,
        NULL);
}
