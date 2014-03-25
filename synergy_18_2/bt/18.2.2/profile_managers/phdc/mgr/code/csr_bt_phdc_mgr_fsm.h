#ifndef CSR_BT_PHDC_MGR_FSM_H__
#define CSR_BT_PHDC_MGR_FSM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_phdc_mgr_main.h"

#ifdef __cplusplus
extern "C" {
#endif


void csrBtPhdcMgrFsmRun(CsrBtPhdcMgrInstance *pInstance,
                        fsm_event_t event,
                        void *param2);

void csrBtPhdcMgrDeviceFsmRun(CsrBtPhdcMgrInstance *pInstance,
                              PhdcMgrDeviceListEntry *pDevice,
                              fsm_event_t event,
                              void *param2);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_MGR_FSM_H*/
