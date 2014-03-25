#ifndef CSR_BT_SAPS_SEF_H__
#define CSR_BT_SAPS_SEF_H__

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
#include "csr_sched.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_saps_main.h"
#include "csr_bt_saps_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
* Handler functions for Upstream messages
*
*****************************************************************************/

/******************************************************************************
* Handler for CSR_BT_CM_REGISTER_CFM in Init_s
******************************************************************************/
void CsrBtSapsInitStateCmRegisterHandler(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_SDS_REGISTER_CFM in Idle_s
******************************************************************************/
void CsrBtSapsIdleStateCmSdsRegisterCfm(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CM_ACCEPT_CONNECT_CFM in Activated_s
******************************************************************************/
void CsrBtSapsActivatedStateConnectAcceptHandler(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DATA_IND in all states
******************************************************************************/
void CsrBtSapsDataIndHandler(SapsInstanceData_t * instData);

/******************************************************************************
* General handler for CSR_BT_CM_DATA_CFM
******************************************************************************/
void CsrBtSapsDataCfmHandler(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_CONTROL_IND in all states
******************************************************************************/
void CsrBtSapsControlInd(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_RFC_MODE_CHANGE_IND in all states
******************************************************************************/
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtSapsModeChangeInd(SapsInstanceData_t * instData);
#else
#define CsrBtSapsModeChangeInd NULL
#endif
/******************************************************************************
* Handler for CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM in Connected_s
******************************************************************************/
void CsrBtSapsDeactivateCancelAcceptConnectCfm(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DISCONNECT_IND in Connected_s
******************************************************************************/
void CsrBtSapsConnectStateDisconnectInd(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DISCONNECT_IND in SapConnected_s
******************************************************************************/
void CsrBtSapsSapConnectedStateDisconnectInd(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DATA_IND in all states
******************************************************************************/
void CsrBtSapsDataIndHandler(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DISCONNECT_IND in Disconnecting_s
******************************************************************************/
void CsrBtSapsDisconnectingStateDisconnectInd(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_CONNECT_ACCEPT_CFM in Deactivate_s
******************************************************************************/
void CsrBtSapsDeactivateAcceptConnectCfm(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_DISCONNECT_IND in Deactivate_s
******************************************************************************/
void CsrBtSapsDeactivateDisconnectInd(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_SDS_REGISTER_CFM in Deactivate_s
******************************************************************************/
void CsrBtSapsDeactivateSdsRegisterCfm(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_CM_SDS_UNREGISTER_CFM in Deactivate_s
******************************************************************************/
void CsrBtSapsDeactivateSdsUnregisterCfm(SapsInstanceData_t * instData);

/*****************************************************************************
*
* Handler functions for Downstream messages
*
*****************************************************************************/

/******************************************************************************
* Handler for CSR_BT_SAPS_ACTIVATE_REQ in Init_s
******************************************************************************/
void CsrBtSapsActivateInitState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Init_s
******************************************************************************/
void CsrBtSapsDeactivateInitState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_ACTIVATE_REQ in Idle_s
******************************************************************************/
void CsrBtSapsActivateIdleState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Idle_s
******************************************************************************/
void CsrBtSapsDeactivateIdleState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Activated_s
******************************************************************************/
void CsrBtSapsDeactivateActivatedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Connected_s
******************************************************************************/
void CsrBtSapsDeactivateReqConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_CONNECT_RES in Connected_s
******************************************************************************/
void CsrBtSapsConnectResConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_DEACTIVATE_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapsDeactivateReqSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_DISCONNECT_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapsDisconnectReqSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_TRANSFER_APDU_RES in SapConnected_s
******************************************************************************/
void CsrBtSapsTransferApduResConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_TRANSFER_ATR_RES in SapConnected_s
******************************************************************************/
void CsrBtSapsTransferAtrResSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_POWER_SIM_OFF_RES in SapConnected_s
******************************************************************************/
void CsrBtSapsTransferPowerSimOffResSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_POWER_SIM_ON_RES in SapConnected_s
******************************************************************************/
void CsrBtSapsTransferPowerSimOnResSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_RESET_SIM_RES in SapConnected_s
******************************************************************************/
void CsrBtSapsTransferResetSimResSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES in SapConnected_s
******************************************************************************/
void CsrBtSapsTransferCardStatusSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES in SapConnected_s
******************************************************************************/
void CsrBtSapsSetTransferProtocolSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_SEND_STATUS_REQ in SapConnected_s
******************************************************************************/
void CsrBtSapsSendStatusReqSapConnectedState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_DEACTIVATE_REQ in Disconnecting_s
******************************************************************************/
void CsrBtSapsDeactivateReqDisconnectingState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler for CSR_BT_SAPS_CONNECT_RES in Disconnecting_s
******************************************************************************/
void CsrBtSapsConnectResDisconnectingState(SapsInstanceData_t * instData);

/******************************************************************************
* Handler and confirm-sender for security level changes
******************************************************************************/
void CsrBtSapsSecurityInReqHandler(SapsInstanceData_t * instData);
void CsrBtSapsSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

/* Prototypes from saps_free_down.c */
void CsrBtSapsFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif
#endif    /* EXCLUDE_CSR_BT_SAPS_MODULE */
