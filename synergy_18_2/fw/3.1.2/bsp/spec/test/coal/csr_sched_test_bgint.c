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

static void test_bgint1(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0001;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

static void test_bgint2(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0002;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

static void test_bgint3(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0004;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

static void test_bgint4(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0008;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

static void test_bgint5(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0010;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

static void test_bgint6(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0020;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

static void test_bgint7(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0040;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

static void test_bgint8(void *arg)
{
    BgintTestInstanceData *inst;

    inst = (BgintTestInstanceData *) arg;

    inst->bgint_mask |= 0x0080;
    CsrSchedMessagePut(inst->send_task, inst->send_prim, inst->send_data);
}

/* Register all interrupts */
CsrBool test_bgint_register(BgintTestInstanceData *inst)
{
    CsrBool res = TRUE;

    inst->testBgint1 = CsrSchedBgintReg(test_bgint1, inst, "testBgint1");
    inst->testBgint2 = CsrSchedBgintReg(test_bgint2, inst, "testBgint2");
    inst->testBgint3 = CsrSchedBgintReg(test_bgint3, inst, "testBgint3");
    inst->testBgint4 = CsrSchedBgintReg(test_bgint4, inst, "testBgint4");
    inst->testBgint5 = CsrSchedBgintReg(test_bgint5, inst, "testBgint5");
    inst->testBgint6 = CsrSchedBgintReg(test_bgint6, inst, "testBgint6");
    inst->testBgint7 = CsrSchedBgintReg(test_bgint7, inst, "testBgint7");
    inst->testBgint8 = CsrSchedBgintReg(test_bgint8, inst, "testBgint8");

    if ((inst->testBgint1 == CSR_SCHED_BGINT_INVALID) ||
        (inst->testBgint2 == CSR_SCHED_BGINT_INVALID) ||
        (inst->testBgint3 == CSR_SCHED_BGINT_INVALID) ||
        (inst->testBgint4 == CSR_SCHED_BGINT_INVALID) ||
        (inst->testBgint5 == CSR_SCHED_BGINT_INVALID) ||
        (inst->testBgint6 == CSR_SCHED_BGINT_INVALID) ||
        (inst->testBgint7 == CSR_SCHED_BGINT_INVALID) ||
        (inst->testBgint8 == CSR_SCHED_BGINT_INVALID))
    {
        res = FALSE;
    }

    return res;
}

void test_bgint_unregister(BgintTestInstanceData *inst)
{
    CsrSchedBgintUnreg(inst->testBgint1);
    CsrSchedBgintUnreg(inst->testBgint2);
    CsrSchedBgintUnreg(inst->testBgint3);
    CsrSchedBgintUnreg(inst->testBgint4);
    CsrSchedBgintUnreg(inst->testBgint5);
    CsrSchedBgintUnreg(inst->testBgint6);
    CsrSchedBgintUnreg(inst->testBgint7);
    CsrSchedBgintUnreg(inst->testBgint8);
}
