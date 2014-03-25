#ifndef CSR_BT_SC_BOND_REMOTE_H__
#define CSR_BT_SC_BOND_REMOTE_H__

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

void CsrBtScDmSmPinRequestIndIdleS(ScInstanceData_t *scData);
void CsrBtScSmIoCapabilityResponseIndIdleS(ScInstanceData_t *scData);
void CsrBtScSmIoCapabilityRequestIndIdleS(ScInstanceData_t *scData);
#define CsrBtScSspPairingResRemoteBondingS CsrBtScSspPairingResBondingS
void CsrBtScSmIoCapabilityResponseIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScSmIoCapabilityRequestIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScSspNotificationResRemoteBondingS(ScInstanceData_t *scData);
#define CsrBtScDmSmUserPasskeyRequestIndRemoteBondingS CsrBtScDmSmUserPasskeyRequestIndLocalBondingS
#define CsrBtScDmSmUserPasskeyNotificationIndRemoteBondingS CsrBtScDmSmUserPasskeyNotificationIndLocalBondingS
#define CsrBtScDmSmUserConfirmationRequestIndRemoteBondingS CsrBtScDmSmUserConfirmationRequestIndLocalBondingS
#define CsrBtScDmSmLocalUserConfirmationRequestIndRemoteBondingS CsrBtScDmSmLocalUserConfirmationRequestIndLocalBondingS
#define CsrBtScSspCompareResRemoteBondingS CsrBtScSspCompareResBondingS
#define CsrBtScSspPasskeyResRemoteBondingS CsrBtScSspPasskeyResBondingS
#define CsrBtScSspJustWorksResRemoteBondingS CsrBtScSspCompareResBondingS
void CsrBtScDmSmPinRequestIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyRequestIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScPasskeyResRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScDmSimplePairingCompleteIndRemoteBondingS(ScInstanceData_t *scData);
void CsrBtScDmSimplePairingCompleteIndRemoteDmPendingS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyIndRemoteAuthRespBondingS(ScInstanceData_t *scData);
void CsrBtScDmSmLinkKeyIndRemoteBondingPostLisbonS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_SC_BOND_REMOTE_H__ */

