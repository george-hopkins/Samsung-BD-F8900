#ifndef CSR_BT_CM_BNEP_H__
#define CSR_BT_CM_BNEP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_cm_main.h"
#include "csr_bt_bnep_prim.h"
#include "csr_bt_bnep_lib.h"
#include "csr_bt_tasks.h"

#define ID_RESERVED                             0
#define ID_EMPTY                                1

#define NO_CONNECT                              0
#define CONNECT                                 1
#define CONNECT_ACCEPT                          2

#define CM_BNEP_MTU                             1504

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 These function are found under cmBnepRegisterHandler.c
************************************************************************************/
void CsrBtCmBnepRegisterReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under cmBnepConnectHandler.c
************************************************************************************/
void CsrBtCmBnepConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepConnectAcceptReqInitHandler(cmInstanceData_t *cmData, CsrUint8 theIndex, CsrUint24 classOfDevice);
void CsrBtCmBnepCancelConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepConnectAcceptReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepCancelConnectAcceptReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepConnectCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepConnectIndHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepConnectAcceptCfmMsgSend(cmInstanceData_t *cmData, CsrSchedQid appHandle,
                                        CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmBnepCancelConnectAcceptCfmMsgSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmBnepConnectIndMsgSend(cmInstanceData_t *cmData, CsrSchedQid appHandle, CsrUint16 theId,
                             ETHER_ADDR theRem_addr, CsrUint16 theRem_uuid, CsrUint16 theLoc_uuid, CsrUint16 theFrameSize,
                             CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmBnepConnectErrorIndHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr,
                                  bnepTable *bnepConnection, CsrUint16 theId, CsrUint16 rem_uuid, CsrUint16 loc_uuid,
                                  ETHER_ADDR rem_addr, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

/*************************************************************************************
 These function are found under CsrBtCmBnepArrivalHandler.c
************************************************************************************/
void CsrBtCmBnepArrivalHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under cmBnepDataHandler.c
************************************************************************************/
void CsrBtCmBnepExtendedDataReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepExtendedMulticastDataReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepExtendedDataIndHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under cmBnepDisconnectHandler.c
************************************************************************************/
void CsrBtCmBnepDisconnectIndMsgSend(CsrUint16 theId, CsrSchedQid appHandle,
                                     CsrBtReasonCode reasonCode,
                                     CsrBtSupplier reasonSupplier);

void CsrBtCmBnepDisconnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepDisconnectIndHandler(cmInstanceData_t *cmData);
void CsrBtCmBnepDisconnectResHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under cmBnepMaintenanceHandler.c
************************************************************************************/
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
CsrBool CsrBtCmBnepRoleSwitchAllowed(cmInstanceData_t *cmData);
#else
#define CsrBtCmBnepRoleSwitchAllowed(cmData) TRUE
#endif
CsrUint8 returnEmptyBnepIdIndex(cmInstanceData_t *cmData);
CsrUint8 returnReserveBnepIdIndex(cmInstanceData_t *cmData);
CsrUint8 returnBnepIdIndex(cmInstanceData_t *cmData, CsrUint16 id);
CsrUint8 returnIndexToThisBdAddr(cmInstanceData_t *cmData, CsrBtDeviceAddr    deviceAddr);
void CsrBtCmBnepClearBnepTableIndex(bnepTable *theLink);

#ifdef __cplusplus
}
#endif

#endif /* ndef _CM_BNEP_H */


