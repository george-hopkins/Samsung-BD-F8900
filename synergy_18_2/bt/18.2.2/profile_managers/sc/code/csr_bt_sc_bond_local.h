#ifndef CSR_BT_SC_BOND_LOCAL_H__
#define CSR_BT_SC_BOND_LOCAL_H__

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2007

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScBondReqIdlePostLisbonS(ScInstanceData_t *scData);
void CsrBtScSmBondingCfmBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScDmSimplePairingCompleteIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyRequestIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScCancelBondReqAnyS(ScInstanceData_t *scData);
void CsrBtScCancelBondReqBondingPostLisbonS(ScInstanceData_t *scData);
void CsrBtScSmIoCapabilityRequestIndLocalBondingS(ScInstanceData_t *scData);
#define CsrBtScSspPairingResLocalBondingS CsrBtScSspPairingResBondingS
void CsrBtScSmIoCapabilityResponseIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmUserConfirmationRequestIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmLocalUserConfirmationRequestIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmUserPasskeyRequestIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmUserPasskeyNotificationIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmPinRequestIndLocalBondingS(ScInstanceData_t *scData);
void CsrBtScPasskeyResLocalBondingS(ScInstanceData_t *scData);
void CsrBtScPasskeyResLocalActiveBondingS(ScInstanceData_t *scData);
void CsrBtScSspPasskeyResLocalBondingS(ScInstanceData_t *scData);
void CsrBtScSspNotificationResLocalBondingS(ScInstanceData_t *scData);
#define CsrBtScSspJustWorksResLocalBondingS CsrBtScSspCompareResLocalBondingS
void CsrBtScSspCompareResLocalBondingS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_BOND_LOCAL_H__ */

