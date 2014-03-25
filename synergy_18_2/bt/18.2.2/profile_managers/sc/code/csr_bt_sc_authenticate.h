#ifndef CSR_BT_SC_AUTHENTICATE_H__
#define CSR_BT_SC_AUTHENTICATE_H__

#include "csr_synergy.h"
/****************************************************************************

                (c) CSR plc 2007

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_sc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtScAuthenticateReqIdleS(ScInstanceData_t *scData);
#define CsrBtScDmSmLinkKeyRequestIndAuthenticateS       CsrBtScDmSmLinkKeyRequestIndIdleS
#define CsrBtScSmIoCapabilityRequestIndAuthenticateS    CsrBtScSmIoCapabilityRequestIndLocalBondingS
#define CsrBtScSspPairingResAuthenticateS               CsrBtScSspPairingResBondingS
#define CsrBtScSmIoCapabilityResponseIndAuthenticateS   CsrBtScSmIoCapabilityResponseIndBondingS
#define CsrBtScDmSmPinRequestIndAuthenticateS           CsrBtScDmSmPinRequestIndBondingS
#define CsrBtScDmSmUserConfirmationRequestIndAuthenticateS CsrBtScDmSmUserConfirmationRequestIndBondingS
#define CsrBtScDmSmLocalUserConfirmationRequestIndAuthenticateS CsrBtScDmSmLocalUserConfirmationRequestIndBondingS
#define CsrBtScDmSmUserPasskeyRequestIndAuthenticateS   CsrBtScDmSmUserPasskeyRequestIndBondingS
#define CsrBtScDmSmUserPasskeyNotificationIndAuthenticateS CsrBtScDmSmUserPasskeyNotificationIndBondingS
#define CsrBtScPasskeyResAuthenticateS                  CsrBtScPasskeyResLocalBondingS
#define CsrBtScSspCompareResAuthenticateS               CsrBtScSspCompareResBondingS
#define CsrBtScSspNotificationResAuthenticateS          CsrBtScSspNotificationResBondingS
#define CsrBtScSspPasskeyResAuthenticateS               CsrBtScSspPasskeyResBondingS
#define CsrBtScSspJustWorksAuthenticateS                CsrBtScSspCompareResAuthenticateS
void CsrBtScDmSmLinkKeyIndAuthenticateS(ScInstanceData_t *scData);
void CsrBtScDmSmAuthenticateCfmAuthenticateS(ScInstanceData_t *scData);

#ifdef __cplusplus
}
#endif

#endif /* Defined CSR_BT_SC_AUTHENTICATE_H__ */
