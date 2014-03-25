/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <string.h>
#include "csr_bt_declare_tasks.h"
//
/* CSR scheduler task initialisation */
void CsrSchedTaskInit(void *data)
{
    /* Initialise standard tasks (see csr_bt_declare_tasks.c) */
    CsrBtAppTaskInitFw(data);
    CsrBtAppTaskInitBt(data);
    /* Synergy BT high-level protocols */
//	CsrSchedRegisterTask(&CSR_BT_HF_IFACEQUEUE, CSR_BT_HF_INIT, CSR_BT_HF_DEINIT, CSR_BT_HF_HANDLER, "CSR_BT_HF", data, ID_STACK);
	CsrSchedRegisterTask(&CSR_BT_AV_IFACEQUEUE, CSR_BT_AV_INIT, CSR_BT_AV_DEINIT, CSR_BT_AV_HANDLER, "CSR_BT_AV", data, ID_STACK);
	CsrSchedRegisterTask(&CSR_BT_AVRCP_IFACEQUEUE, CSR_BT_AVRCP_INIT, CSR_BT_AVRCP_DEINIT, CSR_BT_AVRCP_HANDLER, "CSR_BT_AVRCP", data, ID_STACK);
//    CsrSchedRegisterTask(&CSR_BT_PAC_IFACEQUEUE, CSR_BT_PAC_INIT, CSR_BT_PAC_DEINIT, CSR_BT_PAC_HANDLER, "CSR_BT_PAC", data, ID_STACK);
//	CsrSchedRegisterTask(&CSR_BT_SPP_IFACEQUEUE, CSR_BT_SPP_INIT, CSR_BT_SPP_DEINIT, CSR_BT_SPP_HANDLER, "CSR_BT_SPP", data, ID_STACK);
	CsrSchedRegisterTask(&TESTQUEUE, CSR_BT_TEST_INIT, CSR_BT_TEST_DEINIT, CSR_BT_TEST_HANDLER, "MOBIS_BT_MAIN", data, ID_STACK);
}
