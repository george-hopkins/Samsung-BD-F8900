#ifndef CSR_BT_SC_REBOND_H__
#define CSR_BT_SC_REBOND_H__

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2007

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScCmSmRepairIndHandler(ScInstanceData_t *scData);
void CsrBtScCmSmRepairIndHandlerIdleS(ScInstanceData_t *scData);
void CsrBtScRebondResHandlerRebondS(ScInstanceData_t *scData);
void CsrBtScDmHciDeleteStoredLinkKeyCompleteRebondS(ScInstanceData_t *scData);
void CsrBtScDmRemoveDeviceCfmRebondS(ScInstanceData_t *scData);
#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_REBOND_H__ */
