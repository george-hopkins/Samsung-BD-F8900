#ifndef CSR_BT_PHDC_AG_FSM_H__
#define CSR_BT_PHDC_AG_FSM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_phdc_ag_main.h"
#include "csr_bt_core_stack_fsm.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtPhdcAgentFsmRun(CsrBtPhdcAgentInstance *pInstance, fsm_event_t event);
void CsrBtPhdcAgentDeviceFsmRun(CsrBtPhdcAgentInstance *pInstance, fsm_event_t event);
void CsrBtPhdcAgentTimedEvent(CsrUint16 event, void *pv_arg1);

#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_AG_FSM_H*/
