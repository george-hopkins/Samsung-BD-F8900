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
#define BGINT_PRIM (TEST1_PRIM + 2)

int srv_handle_tc_bgint_nomsg_timer(void **gash)
{
    BgintTestInstanceData *inst;
    CsrUint16 event;
    void *msg;
    int ret;

    inst = (BgintTestInstanceData *) *gash;

    if (!CsrSchedMessageGet(&event, &msg))
    {
        set_tc_verdict(TC_FAILED);
        ret = TC_FINISHED;
    }

    if (event == BGINT_PRIM)
    {
        inst->gotBgint = TRUE;
        ret = TC_NOT_FINISHED;
    }
    else if (event == TEST1_PRIM)
    {
        test_bgint_unregister(inst);
        if ((inst->gotBgint) && (inst->bgint_mask > 0))
        {
            set_tc_verdict(TC_PASSED);
        }
        else
        {
            set_tc_verdict(TC_FAILED);
        }
        ret = TC_FINISHED;
    }
    else
    {
        /* Wrong message received */
        set_tc_verdict(TC_FAILED);
        ret = TC_FINISHED;
    }

    CsrPmemFree(msg);
    return ret;
}

static void timeout(CsrUint16 i, void *p)
{
    CsrSchedMessagePut(TEST_TASK1_IFACEQUEUE, TEST1_PRIM, NULL);
}

void srv_start_tc_bgint_nomsg_timer(void **gash)
{
    BgintTestInstanceData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    inst->send_task = TEST_TASK1_IFACEQUEUE;
    inst->send_prim = BGINT_PRIM;
    inst->send_data = NULL;

    inst->bgint_mask = 0;

    if (test_bgint_register(inst) == TRUE)
    {
        CsrSchedTimerSet(CSR_SCHED_SECOND * 4,
            timeout,
            0,
            NULL);

        CsrSchedBgintSet(inst->testBgint1);
    }
    else
    {
        set_tc_verdict(TC_FAILED);
    }
}
