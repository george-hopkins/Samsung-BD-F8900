/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           sdp_sda_deinit.c

DESCRIPTION:    Shutdown code for SDA.



****************************************************************************/

#include "csr_synergy.h"

#ifdef ENABLE_SHUTDOWN

#include "csr_bt_bluestack_types.h"
#include "csr_sched.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_env_prim.h"
#include "bluetooth.h"
#include "csr_bt_tasks.h"

#include "sds_prim.h"
#include "bluestacklib.h"

extern void sds_deinit(void);

void deinit_sdp_sda(void **gash)
{
    CsrUint16 msg_type;
    void *msg_data;
    CsrBool  lastMsg;

    lastMsg = FALSE;

    while(!lastMsg)
    {
        lastMsg = !CsrSchedMessageGet(&msg_type, &msg_data);

        if (!lastMsg)
        {
            switch (msg_type)
            {
                case SDP_PRIM:
                    {
                        sdp_free_downstream_primitive( (SDS_UPRIM_T *) msg_data);
                        msg_data = NULL;
                    }

                case CSR_SCHED_PRIM:
                    break;
            }
            CsrPmemFree(msg_data);
        }
    }

    sds_deinit();
}

#endif

