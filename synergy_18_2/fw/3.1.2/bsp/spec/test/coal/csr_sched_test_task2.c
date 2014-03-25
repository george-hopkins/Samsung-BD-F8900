/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#if defined(__linux__) && defined(__KERNEL__)
#include <kernel.h>
#else
#include <stdio.h>
#endif

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_sched_test.h"
#include "csr_formatted_io.h"

#define STATE_NULL 0
#define STATE_IDLE 1
#define STATE_TC_RUNNING 2

typedef struct
{
    int   state;
    int   tc_no;
    void *tc_data;
} inst_data_t;

static void handle_msg(inst_data_t *inst, void *msg, CsrUint16 event)
{
    switch (event)
    {
        case SCHED_START_REQ:
        {
            SCHED_START_REQ_t *req;

            req = (SCHED_START_REQ_t *) msg;

            inst->tc_no = req->tc_no;

            if (ts[inst->tc_no].cli_tc_start)
            {
                ts[inst->tc_no].cli_tc_start(&inst->tc_data);
            }

            inst->state = STATE_TC_RUNNING;
            break;
        }

        case CSR_SCHED_TASK_ID:
        {
            char txt[100];
            CsrSnprintf(txt, 100, "Broadcast message received in %s:%d\n", __FILE__, __LINE__);
            output(txt);
            break;
        }

        default:
        {
            char txt[200];
            CsrSnprintf(txt, 200, "Error(%s:%d): Unhandled event: %0x\n",
                __FILE__, __LINE__, event);
            output(txt);
            break;
        }
    }
}

void task2_init(void **p)
{
    inst_data_t *inst;

    inst = (inst_data_t *) CsrPmemAlloc(sizeof(inst_data_t));

    inst->state = STATE_IDLE;
    inst->tc_no = 0;
    inst->tc_data = NULL;

    *p = inst;
}

void task2_handler(void **p)
{
    inst_data_t *inst;
    CsrUint16 event;
    void *msg;
    int ret;

    inst = (inst_data_t *) *p;

    if (inst->state == STATE_IDLE)
    {
        CsrSchedMessageGet(&event, &msg);

        handle_msg(inst, msg, event);

        CsrPmemFree(msg);
    }
    else
    {
        if (ts[inst->tc_no].cli_tc_handle)
        {
            ret = ts[inst->tc_no].cli_tc_handle(&inst->tc_data);

            if (ret == TC_FINISHED)
            {
                inst->state = STATE_IDLE;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            inst->state = STATE_IDLE;
        }
    }
}

void task2_deinit(void **p)
{
    CsrPmemFree(*p);
}
