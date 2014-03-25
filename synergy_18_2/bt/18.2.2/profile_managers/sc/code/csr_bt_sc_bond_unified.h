#ifndef CSR_BT_SC_BOND_UNIFIED_H__
#define CSR_BT_SC_BOND_UNIFIED_H__

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2008

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScBondReqIdleS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyIndIdleS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyRequestIndIdleS(ScInstanceData_t *scData);
void CsrBtScDmSmUserConfirmationRequestIndBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmLocalUserConfirmationRequestIndBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmUserPasskeyRequestIndBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmUserPasskeyNotificationIndBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmPinRequestIndBondingS(ScInstanceData_t *scData);

void CsrBtScSspPasskeyResBondingS(ScInstanceData_t *scData);
void CsrBtScSspNotificationResBondingS(ScInstanceData_t *scData);
void CsrBtScSspCompareResBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyIndRemoteBondingS(ScInstanceData_t *scData);

void CsrBtScSspKeypressNotificationReqBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmKeypressNotificationIndBondingS(ScInstanceData_t *scData);

void CsrBtScSmIoCapabilityRequestIndDmPendingS(ScInstanceData_t *scData);
void CsrBtScSmIoCapabilityResponseIndBondingS(ScInstanceData_t *scData);
void CsrBtScSspPairingResBondingS(ScInstanceData_t *scData);

void CsrBtScCancelBondReqAnyS(ScInstanceData_t *scData);

void CsrBtScPairingCancelled(ScInstanceData_t *scData);
void CsrBtScPairingSuccessUpdateDeviceReset(ScInstanceData_t *scData);
void CsrBtScPairingSuccessUpdateDevice(ScInstanceData_t *scData);
void CsrBtScPairingFailedCleanupDeviceReset(ScInstanceData_t *scData);

void CsrBtScSmBondingCfmOobBondingS(ScInstanceData_t *scData);
void CsrBtScSmBondingCfmSspBondingS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_BOND_UNIFIED_H__ */

