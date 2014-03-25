/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <string.h>
#include "csr_bt_declare_tasks.h"
#include "csr_bt_tpt_tasks.h"

/* CSR scheduler task initialisation */
void CsrSchedTaskInit(void *data)
{
    /* Initialise standard tasks (see csr_bt_declare_tasks.c) */
    CsrBtAppTaskInitFw(data);
    CsrBtAppTaskInitBt(data);
    CsrBtAppTaskInitWifi(data);

    /* Synergy BT high-level protocols */
    CsrSchedRegisterTask(&CSR_BT_AV_IFACEQUEUE, CSR_BT_AV_INIT, CSR_BT_AV_DEINIT, CSR_BT_AV_HANDLER, "CSR_BT_AV", data, ID_STACK);
    CsrSchedRegisterTask(&CSR_BT_SPP_IFACEQUEUE, CSR_BT_SPP_INIT, CSR_BT_SPP_DEINIT, CSR_BT_SPP_HANDLER, "CSR_BT_SPP", data, ID_STACK);

    /* Application task and helpers */
#if !defined(EXCLUDE_TEST_MODULE) && (CSR_BT_RUN_TASK_TEST_TASK == 1)
    /* Application task */
    CsrSchedRegisterTask(&TESTQUEUE, TPT_INIT, TPT_DEINIT, TPT_HANDLER, "CSR_BT_TPT", data, ID_APP);
#endif

}
