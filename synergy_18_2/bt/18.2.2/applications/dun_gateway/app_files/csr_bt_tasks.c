/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <string.h>
#include "csr_bt_declare_tasks.h"

/* CSR scheduler task initialisation */
void CsrSchedTaskInit(void *data)
{
    /* Initialise standard tasks (see csr_bt_declare_tasks.c) */
    CsrBtAppTaskInitFw(data);
    CsrBtAppTaskInitBt(data);
    CsrBtAppTaskInitWifi(data);

    /* Synergy Bluetooth application/profile specific tasks */
    CsrSchedRegisterTask(&CSR_BT_DG_IFACEQUEUE, CSR_BT_DG_INIT, CSR_BT_DG_DEINIT, CSR_BT_DG_HANDLER, "CSR_BT_DG", data, ID_STACK);

    /* Application task and helpers */
    CsrSchedRegisterTask(&CSR_BT_AT_IFACEQUEUE, CSR_BT_AT_INIT, CSR_BT_AT_DEINIT, CSR_BT_AT_HANDLER, "CSR_BT_AT", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_IWU_IFACEQUEUE, CSR_BT_IWU_INIT, CSR_BT_IWU_DEINIT, CSR_BT_IWU_HANDLER, "CSR_BT_IWU", data, ID_APP);
#if !defined(EXCLUDE_TEST_MODULE) && (CSR_BT_RUN_TASK_TEST_TASK == 1)
    CsrSchedRegisterTask(&TESTQUEUE, CSR_BT_TEST_INIT, CSR_BT_TEST_DEINIT, CSR_BT_TEST_HANDLER, "CSR_BT_APP", data, ID_APP);
#endif
}
