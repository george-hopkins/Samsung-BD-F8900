#ifndef CSR_BT_SC_NULL_H__
#define CSR_BT_SC_NULL_H__

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

void CsrBtScActivateReqAnyS(ScInstanceData_t *scData);
void CsrBtScCmGetSecurityConfIndNullS(ScInstanceData_t *scData);
void CsrBtScCmReadLocalBdAddrCfmNullS(ScInstanceData_t *scData);
#ifdef CSR_BT_INSTALL_SC_SET_IO_CAPABILITY
void CsrBtScSetIoCapabilityReqHandler(ScInstanceData_t *scData);
#endif
#ifdef CSR_BT_INSTALL_SC_SET_AUTH_REQUIREMENTS
void CsrBtScSetAuthRequirementsReqHandler(ScInstanceData_t *scData);
#endif
#ifdef CSR_BT_INSTALL_SC_SET_EVENT_MASK
void CsrBtScSetEventMaskAnyS(ScInstanceData_t *scData);
#endif
void CsrBtScDmSmInitCfmNullS(ScInstanceData_t *scData);
void CsrBtScCmSetEventMaskCfmNullS(ScInstanceData_t *scData);
void CsrBtScDebugModeReqIdleS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif
