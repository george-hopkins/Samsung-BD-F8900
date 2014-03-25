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

    /* Synergy BT high-level protocols */
    CsrSchedRegisterTask(&CSR_BT_BSL_IFACEQUEUE, CSR_BT_BSL_INIT, CSR_BT_BSL_DEINIT, CSR_BT_BSL_HANDLER, "CSR_BT_BSL", data, ID_STACK);

    /* Application task and helpers */
    CsrSchedRegisterTask(&CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_INIT, CSR_BT_IP_DEINIT, CSR_BT_IP_TASK, "CSR_BT_IP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_ICMP_IFACEQUEUE, CSR_BT_ICMP_INIT, CSR_BT_ICMP_DEINIT, CSR_BT_ICMP_TASK, "CSR_BT_ICMP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_INIT, CSR_BT_UDP_DEINIT, CSR_BT_UDP_HANDLER, "CSR_BT_UDP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_DHCP_IFACEQUEUE, CSR_BT_DHCP_INIT, CSR_BT_DHCP_DEINIT, CSR_BT_DHCP_TASK, "CSR_BT_DHCP", data, ID_APP);
    CsrSchedRegisterTask(&CSR_BT_TFTP_IFACEQUEUE, CSR_BT_TFTP_INIT, CSR_BT_TFTP_DEINIT, CSR_BT_TFTP_HANDLER, "CSR_BT_TFTP", data, ID_APP);

#if !defined(EXCLUDE_TEST_MODULE) && (CSR_BT_RUN_TASK_TEST_TASK == 1)
    CsrSchedRegisterTask(&TESTQUEUE, CSR_BT_TEST_INIT, CSR_BT_TEST_DEINIT, CSR_BT_TEST_HANDLER, "CSR_BT_APP", data, ID_APP);
#endif
}
