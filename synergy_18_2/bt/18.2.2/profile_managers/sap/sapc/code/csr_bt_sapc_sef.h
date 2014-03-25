#ifndef CSR_BT_SAPC_SEF_H__
#define CSR_BT_SAPC_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sapc_prim.h"
#include "csr_bt_sapc_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
  CsrBtSapcSavemessage
************************************************************************************/
void CsrBtSapcSavemessage(SapcInstanceData_t * instData);

/*************************************************************************************
  CsrBtSapcRestoreSavedMessages
************************************************************************************/
void CsrBtSapcRestoreSavedMessages(SapcInstanceData_t * instData);

/*****************************************************************************
*
* Handler functions for Upstream messages
*
*****************************************************************************/

/******************************************************************************
* Handler for CSR_BT_CM_REGISTER_CFM in Init_s
******************************************************************************/
void CsrBtSapcInitStateCmRegisterHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_CONTROL_IND in all States
******************************************************************************/
void CsrBtSapcXStateCmControlIndHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_RFC_MODE_CHANGE_IND in all States
******************************************************************************/
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtSapcXStateCmModeChangeIndHandler(SapcInstanceData_t * instData);
#else
#define CsrBtSapcXStateCmModeChangeIndHandler NULL
#endif
/******************************************************************************
* Handler for CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM in Idle_s
******************************************************************************/
void SapcIdleStateCmSdcResourcesCfmHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_CONNECT_CFM in Connected_s
******************************************************************************/
void CsrBtSapcConnectedStateCmConnectCfmHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DATA_CFM in all states
******************************************************************************/
void CsrBtSapcDataCfmHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DATA_IND in all states
******************************************************************************/
void CsrBtSapcDataIndHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DISCONNECT_IND in Connected_s
******************************************************************************/
void CsrBtSapcConnectedStateCmDisconnectIndHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DISCONNECT_IND in Connected_s
******************************************************************************/
void CsrBtSapcSapConnectedStateCmDisconnectIndHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DISCONNECT_IND in Disconnecting_s
******************************************************************************/
void CsrBtSapcDisconnectingStateCmDisconnectIndHandler(SapcInstanceData_t * instData);

/*****************************************************************************
*
* Handler functions for Downstream messages
*
*****************************************************************************/

/******************************************************************************
* Handler for CSR_BT_SAPC_CONNECT_REQ in Idle_s
******************************************************************************/
void CsrBtSapcIdleStateConnectReqHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_DISCONNECT_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStateDisconnectReqHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_TRANSFER_APDU_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStateTransferApduReqHandler(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_TRANSFER_ATR_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStateTransferAtrReq(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_POWER_SIM_OFF_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStatePowerSimOffReq(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_POWER_SIM_ON_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStatePowerSimOnReq(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_RESET_SIM_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStateResetSimReq(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStateTransferCardStatusReq(SapcInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapcSapConnectedStateSetTransferProtocolReq(SapcInstanceData_t * instData);

/******************************************************************************
* Handler and confirmation sender for security level changing
******************************************************************************/
void CsrBtSapcSecurityOutReqHandler(SapcInstanceData_t * instData);
void CsrBtSapcSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

/* Prototypes from sapc_free_down.c */
void CsrBtSapcFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif
