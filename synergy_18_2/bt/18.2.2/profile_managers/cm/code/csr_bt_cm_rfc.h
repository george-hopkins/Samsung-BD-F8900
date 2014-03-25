#ifndef CSR_BT_CM_RFC_H__
#define CSR_BT_CM_RFC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_cm_main.h"
#include "rfcommlib.h"
#include "csr_bt_rfc_proxy_lib.h"
#include "csr_bt_cm_util.h"

#define RFC_PSM                        (0x0003)

#define RFC_LOCAL_RELEASE               (0)
#define RFC_REMOTE_RELEASE              (1)
#define RFC_RELEASE_FAIL                (2)

#define RFC_FORCE_CLOSE_TIME            (1000000)
#define CSR_BT_CM_RFC_TX_WINDOW_SIZE    (2)


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrBtConnId   bt_conn_id;
    CsrUint8      server;
} cmConnIdServerType;

typedef struct
{
    CsrBtConnId    bt_conn_id;
    CsrUint32      state;
} cmConnIdStateType;

#define CM_RFC_GET_FIRST(list) \
    ((cmRfcConnElement *)CsrCmnListGetFirst(&(list)))
#define CM_FIND_RFC_ELEMENT(func, searchDataPtr) \
    ((cmRfcConnElement *)CsrCmnListSearch(&(cmData->rfcVar.connList), func, searchDataPtr))

cmConnIdServerType CsrBtCmReturnConnIdServerStruct(CsrBtConnId bt_conn_id, CsrUint8 server);

/*************************************************************************************
 These function are found in CsrBtCmRfcArrivalHandler.c
************************************************************************************/
void CsrBtCmRfcArrivalHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcRegisterHandler.c
************************************************************************************/
void CsrBtCmRfcRegisterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcRegisterCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcUnRegisterReqMsgSend(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcConnectHandler.c
************************************************************************************/
void CsrBtCmRfcStartInitiateConnection(cmInstanceData_t *cmData,
                                       cmRfcConnInstType * theLogicalLink);
void CsrBtCmConnectCfmMsgSend(cmInstanceData_t *cmData,
                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtCmRfcConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcConnectReqExtHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSmAccessReqMsgSend(void);
void CsrBtCmSmAccessReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmSmAccessCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmCancelConnectReqHandler(cmInstanceData_t *cmData);

void CsrBtCmRfcCommonErrorHandler(cmInstanceData_t *cmData, 
                                  cmRfcConnInstType * theLogicalLink);
void CsrBtCmRfcServerConnectCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcServerConnectIndHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcClientConnectCfmHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcAcceptConnectHandler.c
************************************************************************************/
void CsrBtCmConnectAcceptCfmMsgSend(cmInstanceData_t *cmData, 
                                    cmRfcConnElement *theElement, 
                                    CsrBtResultCode resultCode, 
                                    CsrBtSupplier resultSupplier);
void CsrBtCmRfcConnectAcceptTimeoutHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcAcceptConnectReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcReleaseHandler.c
************************************************************************************/
CsrBool CsrBtCmRfcReleaseStatus(CsrBtReasonCode     reasonCode,
                               CsrBtSupplier reasonSupplier);

void CsrBtCmDisconnectIndMsgCleanupSend(cmRfcConnElement    *theElement,
                                        CsrBtReasonCode     reasonCode,
                                        CsrBtSupplier reasonSupplier,
                                        CsrBool              status,
                                        CsrBool              localTerminated);

void CsrBtCmRfcReleaseIndHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcReleaseReqHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcControlHandler.c
************************************************************************************/
void CsrBtCmControlIndMsgSend(cmRfcConnInstType * theLogicalLink,
                              CsrUint8 modem_signal,
                              CsrUint8 break_signal);
void CsrBtCmRfcControlReqMsgSend(cmInstanceData_t *cmData);
void CsrBtCmRfcModemStatusIndHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcModemStatusCfmHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcDataHandler.c
************************************************************************************/
void CsrBtCmRfcRestoreDataInReceiveBuffer(cmRfcConnInstType *theLogicalLink);

void CsrBtCmRfcDataReqHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcDataResHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcDataIndHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcDataNoCredit.c
************************************************************************************/
void CsrBtCmRfcDataWriteCfmHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in cmRfcScoHandler.c
************************************************************************************/
void CsrBtCmDmSyncDisconnectRfcHandler(cmInstanceData_t *cmData, cmRfcConnInstType *theLogicalLink,
                                       CsrUint8 status, CsrBtReasonCode reason);
void CsrBtCmDmSyncConnectRfcCfmHandler(cmRfcConnInstType *theLogicalLink, cmInstanceData_t *cmData);
void CsrBtCmDmSyncConnectCompleteRfcHandler(cmRfcConnInstType *theLogicalLink,
                                            DM_SYNC_CONNECT_COMPLETE_IND_T * dmPrim);
void CsrBtCmDmScoRenegotiateCfmMsgSend(cmRfcConnInstType * theLogicalLink,
                    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtCmDmScoRenegotiateIndMsgSend(cmRfcConnInstType * theLogicalLink,
                    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

/*************************************************************************************
 These function are found in cmRfcMaintenanceHandler.c
************************************************************************************/
CsrUint16 CsrBtCmRfcDetermineMtu(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, 
                                 CsrUint16 proposedMtu);
cmRfcConnElement *CsrBtCmRfcFindRfcConnElementFromDeviceAddrState(cmInstanceData_t *cmData,
                                                                  CsrBtDeviceAddr *deviceAddr, 
                                                                  CsrUint32 state);
void cleanUpConnectionTable(cmRfcConnInstType ** theLogicalLink);
CsrBool CsrBtCmRfcFindRfcConnElementFromIndex(CsrCmnListElm_t *elem, void *value);
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
CsrBool CsrBtCmRfcFindRfcConnElementFromScoHandle(CsrCmnListElm_t *elem, void *value);
#endif
CsrBool CsrBtCmRfcFindRfcConnElementFromBtConnId(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmRfcFindRfcConnElementFromConnIdServer(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmRfcFindRfcConnElementFromConnIdRemoteServer(CsrCmnListElm_t *elem, void *value);
cmRfcConnElement *CsrBtCmRfcFindRfcConnElementFromServerState(cmInstanceData_t *cmData, 
                                                              CsrUint8 server, 
                                                              CsrUint32 state);
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
cmRfcConnElement *CsrBtCmRfcFindRfcConnElementFromDeviceAddrScoHandle(cmInstanceData_t *cmData, 
                                                                      CsrBtDeviceAddr *deviceAddr, 
                                                                      hci_connection_handle_t scoHandle);
#endif
cmRfcConnElement *CsrBtCmRfcFindRfcConnElementFromDeviceAddrState1OrState2(cmInstanceData_t *cmData, 
                                                                           CsrBtDeviceAddr *deviceAddr, 
                                                                           CsrUint32 state1, 
                                                                           CsrUint32 state2);
cmRfcConnElement *CsrBtCmRfcFindRfcConnElementFromDeviceAddrStateRequestedMode(cmInstanceData_t *cmData, 
                                                                               CsrBtDeviceAddr *deviceAddr, 
                                                                               CsrUint32 state,
                                                                               CsrUint8 requestedMode);
CsrBool CsrBtCmRfcSetElementCounter(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found in csr_bt_cm_rfc_amp_handler.c
************************************************************************************/
#ifdef CSR_AMP_ENABLE
void CsrBtCmRfcAmpMoveChannelCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcAmpMoveChannelCmpIndHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcAmpMoveChannelIndHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcAmpMoveChannelReqHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcAmpMoveChannelResHandler(cmInstanceData_t *cmData);
void CsrBtCmRfcAmpLinkLossIndHandler(cmInstanceData_t *cmData);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_CM_RFC_H__ */
