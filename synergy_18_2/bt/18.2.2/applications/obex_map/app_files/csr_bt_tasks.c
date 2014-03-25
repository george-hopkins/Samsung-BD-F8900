/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <string.h>
#include "csr_bt_declare_tasks.h"

#define EXTRA_MAPC_INSTANCES 9

/* CSR scheduler task initialisation */
void CsrSchedTaskInit(void *data)
{
    CsrUint32 i;

    /* Initialise standard tasks (see csr_bt_declare_tasks.c) */
    CsrBtAppTaskInitFw(data);
    CsrBtAppTaskInitBt(data);
    CsrBtAppTaskInitWifi(data);

    /* Synergy BT high-level protocols */
    CsrSchedRegisterTask(&CSR_BT_MAPS_IFACEQUEUE, CSR_BT_MAPS_INIT, CSR_BT_MAPS_DEINIT, CSR_BT_MAPS_HANDLER, "CSR_BT_MAPS", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_MAPC_IFACEQUEUE, CSR_BT_MAPC_INIT, CSR_BT_MAPC_DEINIT, CSR_BT_MAPC_HANDLER, "CSR_BT_MAPC", data, ID_STACK);
    for(i=0; i<EXTRA_MAPC_INSTANCES; i++)
    {
        CsrSchedRegisterTask(&CSR_BT_MAPC_EXTRA_IFACEQUEUE, CSR_BT_MAPC_INIT, CSR_BT_MAPC_DEINIT, CSR_BT_MAPC_HANDLER, "CSR_BT_MAPC", data, ID_STACK);
    }

#if !defined(EXCLUDE_TEST_MODULE) && (CSR_BT_RUN_TASK_TEST_TASK == 1)
    /* Application task and helpers */
    CsrSchedRegisterTask(&TESTQUEUE, CSR_BT_TEST_INIT, CSR_BT_TEST_DEINIT, CSR_BT_TEST_HANDLER, "CSR_BT_APP", data, ID_STACK /*ID_APP*/);
#endif
}
