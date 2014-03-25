#ifndef CSR_BT_CM_DM_SC_SSP_HANDLER_H__
#define CSR_BT_CM_DM_SC_SSP_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_sc_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtCmSmBondingReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSmBondingScStateHandler(cmInstanceData_t * cmData);
void CsrBtCmDmBondingCfm(cmInstanceData_t *cmData, 
                         BD_ADDR_T *p_bd_addr, CsrUint8 status);
void CsrBtCmSmBondingCancelReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmSecModeConfigReqHandler(cmInstanceData_t *cmData);
void cmSmReadDeviceReqHandler(cmInstanceData_t *cmData);

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
void CsrBtCmSmIoCapabilityRequestResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmIoCapabilityRequestNegResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmReadLocalOobDataReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmSendKeypressNotificationReqHandler(cmInstanceData_t *cmData);
void CsrBtCmSmRepairResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmUserConfirmationRequestNegResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmUserConfirmationRequestResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmUserPasskeyRequestNegResHandler(cmInstanceData_t *cmData);
void CsrBtCmSmUserPasskeyRequestResHandler(cmInstanceData_t *cmData);
#else
#define CsrBtCmSmIoCapabilityRequestResHandler NULL
#define CsrBtCmSmIoCapabilityRequestNegResHandler NULL
#define CsrBtCmSmReadLocalOobDataReqHandler NULL
#define CsrBtCmSmSendKeypressNotificationReqHandler NULL
#define CsrBtCmSmRepairResHandler NULL
#define CsrBtCmSmUserConfirmationRequestNegResHandler NULL
#define CsrBtCmSmUserConfirmationRequestResHandler NULL
#define CsrBtCmSmUserPasskeyRequestNegResHandler NULL
#define CsrBtCmSmUserPasskeyRequestResHandler NULL
#endif

void CsrBtCmSmLeSecurityReqHandler(cmInstanceData_t *cmData);

#ifdef __cplusplus
}
#endif

#endif
