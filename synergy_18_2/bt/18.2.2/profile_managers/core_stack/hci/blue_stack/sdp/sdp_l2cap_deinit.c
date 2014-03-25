/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           sdp_l2cap_deinit.c

DESCRIPTION:    Shutdown code for L2CAP interface in SDP.



****************************************************************************/

#include "csr_synergy.h"

#ifdef ENABLE_SHUTDOWN

#include "csr_bt_bluestack_types.h"
#include "csr_sched.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_env_prim.h"
#include "bluetooth.h"
#include "csr_bt_tasks.h"

#include "l2cap_prim.h"
#include "bluestacklib.h"

void sdp_l2cap_deinit(void **gash)
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
                case L2CAP_PRIM:
                    {
                        L2CA_FreePrimitive( (L2CA_UPRIM_T *) msg_data);
                        msg_data = NULL;
                        break;
                    }

                case CSR_SCHED_PRIM:
                    break;
            }
            CsrPmemFree(msg_data);
        }
    }
}

#endif

