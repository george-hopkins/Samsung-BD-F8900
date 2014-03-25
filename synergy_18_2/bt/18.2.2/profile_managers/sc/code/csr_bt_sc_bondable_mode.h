#ifndef CSR_BT_SC_BONDABLE_MODE_H__
#define CSR_BT_SC_BONDABLE_MODE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CSR_BT_INSTALL_SC_MODE
void CsrBtScStopBondableTimer(ScInstanceData_t *scData);
#endif
#ifdef CSR_BT_INSTALL_SC_MODE
void CsrBtScModeReqIdleS(ScInstanceData_t *scData);
#endif
void CsrBtScRejectDmSmPinRequestInd(ScInstanceData_t *scData);
void CsrBtScRejectDmSmAuthoriseInd(ScInstanceData_t *scData);
void CsrBtScRejectDmSmIoCapabilityRequestInd(ScInstanceData_t *scData);
void CsrBtScRejectDmSmLinkKeyRequestInd(ScInstanceData_t *scData);
void CsrBtScRejectDmSmSspLinkKeyRequestInd(ScInstanceData_t *scData);
void CsrBtScIgnoreAnyS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif
