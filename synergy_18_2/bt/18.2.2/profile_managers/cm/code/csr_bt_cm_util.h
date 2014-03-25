#ifndef CSR_BT_CM_UTIL_H__
#define CSR_BT_CM_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_cm_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 These function are found under csr_bt_cm_profile_provider.c
************************************************************************************/
void CsrBtCmProfileProvider(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_maintenance_handler.c
************************************************************************************/
void CsrBtCmPutMessage(CsrSchedQid phandle, void *msg);

void CsrBtCmModeChangeReqMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr);
CsrUint8 returnServerRegistrationIndex(cmInstanceData_t *cmData, CsrUint8 theServerChannel);
CsrUint8 CsrBtCmReturnNumOfConnectionsToPeerDevice(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr);
CsrUint8 CsrBtCmReturnNumOfConnectionToPeerDeviceFlags(cmInstanceData_t *cmData, CsrBtDeviceAddr *theAddr, CsrUint32 flags);
void CsrBtCmInitInstData(cmInstanceData_t *cmData);
CsrUint24 CsrBtCmReturnClassOfdevice(cmInstanceData_t *cmData);
link_policy_settings_t CsrBtCmEnableMSSwitchParameter(cmInstanceData_t *cmData);
CsrBool updateLogicalChannelTypeMaskAndNumber(cmInstanceData_t *cmData, CsrBtDeviceAddr *theAddr);
void CsrBtCmRegisterHandlerReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDataBufferEmptyReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDataBufferEmptyCfmSend(CsrSchedQid appHandle, CsrUint16 context);

#ifdef CSR_AMP_ENABLE
CsrUint8 CsrBtCmBtControllerActive(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr);
#endif

/*************************************************************************************
 These function are found under csr_bt_cm_service_manager_provider.c
************************************************************************************/
void CsrBtCmRestoreServiceManagerQueue(cmInstanceData_t *cmData);
void CsrBtCmServiceManagerLocalQueueHandler(cmInstanceData_t *cmData);
void CsrBtCmServiceManagerProvider(cmInstanceData_t *cmData);
void CsrBtCmServiceManagerCancelAcceptConnectProvider(cmInstanceData_t *cmData);
void CsrBtCmServiceManagerL2caConnectAcceptProvider(cmInstanceData_t *cmData);
void CsrBtCmServiceManagerL2caCancelConnectAcceptProvider(cmInstanceData_t *cmData);
CsrBool cancelServiceManagerMsg(cmInstanceData_t *cmData,
                               CsrBtCmPrim type,
                               CsrSchedQid phandle,
                               CsrBtDeviceAddr bd_addr,
                               CsrUint8 serverCh,
                               psm_t localPsm,
                               CsrUint8 *sdcServer,
                               CsrUint16     *context);
CsrBool CsrBtCmRemoveSavedOutgoingConnectMessage(cmInstanceData_t *cmData, CsrPrim type, CsrUint32 matchValue);
CsrBool CsrBtCmCheckSavedIncomingConnectMessages(cmInstanceData_t *cmData, CsrBtDeviceAddr bd_addr);
void CsrBtCmRemoveSavedIncomingConnectMessages(cmInstanceData_t *cmData, CsrBtDeviceAddr bd_addr);

/*************************************************************************************
 These function are found under csr_bt_cm_l2cap_connect_handler.c
************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif

