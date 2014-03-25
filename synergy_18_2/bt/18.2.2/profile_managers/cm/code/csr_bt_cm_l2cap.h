#ifndef CSR_BT_CM_L2CA_H__
#define CSR_BT_CM_L2CA_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_bt_cm_main.h"
#include "l2caplib.h"
#include "csr_bt_cm_util.h"

#define BTCONN_ID_RESERVED                          0
#define BTCONN_ID_EMPTY                             1
#define NO_REMOTE_PSM                               0xFFFF
#define NO_LOCAL_PSM                                0xFFFF
#define MAX_L2CAP_DATA_QUEUE_LENGTH                 20 /* default rx queue size for basic mode */

#ifndef MAX_BUFFERED_L2CAP_REQUESTS
#define MAX_BUFFERED_L2CAP_REQUESTS                 (0x02) /* default number of pending datawrites for basic mode */
#endif

#ifndef MAX_BUFFERED_L2CAP_HIGH_PRIORITY_REQUESTS
#define MAX_BUFFERED_L2CAP_HIGH_PRIORITY_REQUESTS   (0x04) /* default number of pending datawrites for basic mode high priority channels */
#endif

/* Values defined in the specification. Do not touch! */
#define MAX_L2CAP_TX_WINDOW                         32
#define MIN_L2CAP_TX_WINDOW                         1
#define MAX_L2CAP_RETRANS_COUNT                     0xFF
#define MIN_L2CAP_RETRANS_COUNT                     0

#ifdef __cplusplus
extern "C" {
#endif

#define CM_L2CA_GET_FIRST(list) \
    ((cmL2caConnElement *)CsrCmnListGetFirst(&(list)))
#define CM_FIND_L2CA_ELEMENT(func, searchDataPtr) \
    ((cmL2caConnElement *)CsrCmnListSearch(&(cmData->l2caVar.connList), func, searchDataPtr))

/*************************************************************************************
 These function are found under csr_bt_cm_l2cap_register_handler.c
************************************************************************************/
void CsrBtCmL2caRegisterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caRegisterCfmHandler(cmInstanceData_t *cmData);
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_UNREGISTER
void CsrBtCmL2caUnRegisterReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caUnRegisterCfmHandler(cmInstanceData_t *cmData);
#endif
/*************************************************************************************
 These function are found under csr_bt_l2cap_connect_handler.c
************************************************************************************/
void CsrBtCmL2caConnectAcceptCfmHandler(cmInstanceData_t *cmData, cmL2caConnElement * theElement,
                                        CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmL2caConnectCfmMsgHandler(cmInstanceData_t *cmData, cmL2caConnElement * theElement,
                                     CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmL2caCancelConnectAcceptCfmMsgSend(cmInstanceData_t *cmData, CsrSchedQid appHandle, psm_t psm,
                                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier,
                                              CsrUint16 context);
void CsrBtCml2caAutoConnectSetup(cmInstanceData_t *cmData, cmL2caConnInstType *l2CaConnection);
void CsrBtCml2caCancelAcceptConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCml2caConnectAcceptReqHandler(cmInstanceData_t *cmData);
void CsrBtCml2caConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caAutoConnectIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caAutoConnectCfmHandler(cmInstanceData_t *cmData);
void CsrBtCml2caCancelConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2CaConnectCfmErrorHandler(cmInstanceData_t     *cmData,
                                       cmL2caConnElement    *theElement,
                                       CsrBtResultCode      resultCode,
                                       CsrBtSupplier  resultSupplier);

void CsrBtCmL2caConnectCancelCleanup(cmInstanceData_t *cmData, cmL2caConnElement * theElement, CsrBool restoreDm);
CsrBool CsrBtCmL2caCheckLegacyDetach(cmInstanceData_t *cmData, cmL2caConnInstType *l2CaConn);
/*************************************************************************************
 These function are found under csr_bt_l2cap_arrival_handler.c
************************************************************************************/
void CsrBtCmL2CaArrivalHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_l2cap_config_handler.c
************************************************************************************/
void CsrBtCmL2capStoreConfigOptions(L2CA_CONFIG_T *config, cmL2caConnInstType * thelink);
void CsrBtCmL2capConfigHandler(cmInstanceData_t *cmData, cmL2caConnInstType * l2CaConnection);

/*************************************************************************************
 These function are found under csr_bt_cm_l2cap_data_handler.c
************************************************************************************/
void CsrBtCmL2caDataWriteReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caDataWriteCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caDataReadIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caRestoreRxQueue(cmInstanceData_t *cmData, cmL2caConnInstType *l2capConn);
void CsrBtCmL2caDataResHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caDataAbortReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caBusyIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caDatawriteAbortCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caDataStart(cmInstanceData_t *cmData, cmL2caConnInstType *l2capConn);
void CsrBtCmL2caGetChannelInfoReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caGetChannelInfoCfmHandler(cmInstanceData_t *cmData);

/*************************************************************************************
 These function are found under csr_bt_cm_l2cap_disconnect_handler.c
************************************************************************************/
void CsrBtCmL2capDisconnectIndMsgSend(cmInstanceData_t *cmData, cmL2caConnElement *theElement,
                        CsrBtReasonCode reasonCode, CsrBtSupplier reasonSupplier, CsrBool localTerminated);
void CsrBtCmL2caDisconnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caDisconnectIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caDisconnectCfmHandler(cmInstanceData_t *cmData);

void CsrBtCmL2caFinalReleaseHandler(cmInstanceData_t    *cmData,
                                    cmL2caConnElement   *theElement,
                                    CsrBool              flush,
                                    CsrBtReasonCode     reasonCode,
                                    CsrBtSupplier reasonSupplier,
                                    CsrBool              localTerminated);

/*************************************************************************************
 These function are found under csr_bt_l2cap_sco_handler.c
************************************************************************************/
void CsrBtCmDmSyncConnectCompletel2capHandler(cmL2caConnInstType *l2CaConnection, DM_SYNC_CONNECT_COMPLETE_IND_T * dmPrim);
void CsrBtCmDmSyncConnectL2capCfmHandler(cmInstanceData_t *cmData, cmL2caConnInstType *l2CaConnection);
void CsrBtCmDmSyncDisconnectL2caHandler(cmInstanceData_t *cmData, cmL2caConnInstType *l2CaConnection, CsrUint8 status, CsrBtReasonCode reason);
void CsrBtCmDmScoL2capCancelAcceptConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoL2capAcceptConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoL2capDisconnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoL2capConnectReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoL2capRenegotiateReqHandler(cmInstanceData_t *cmData);
void CsrBtCmDmScoL2capRenegotiateIndMsgSend(cmL2caConnInstType * l2CaConnection,
                            CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtCmDmScoL2capRenegotiateCfmMsgSend(cmL2caConnInstType * l2CaConnection,
                            CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

/*************************************************************************************
 These function are found under csr_bt_cm_l2cap_maintenance_handler.c
************************************************************************************/
CsrBool CsrBtCmL2caFindL2caConnElementFromScoHandle(CsrCmnListElm_t *elem, void *value);
void CsrBtCmL2capClearL2capTableIndex(cmInstanceData_t *cmData, cmL2caConnInstType **theLink);
void CsrBtCleanUpCmL2caConnInst(cmL2caConnInstType **theLink);
void CsrBtCmL2capAcceptFailClearing(cmInstanceData_t *cmData, cmL2caConnInstType *theLink);
CsrBool CsrBtCmL2caFindL2caConnElementFromReserveBtConnIdPsm(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmL2caFindL2caConnElementFromCancelledBtConnIdPsm(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmL2caFindL2caConnElementFromReserveScoDeviceAddr(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmL2caFindL2caConnElementFromBtConnId(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmL2caFindL2caConnElementFromConnectedSDeviceAddr(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmL2caFindL2caConnElementFromIndex(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtCmL2caFindL2caConnElementFromPsm(CsrCmnListElm_t *elem, void *value);
void CsrBtCmL2caTimeoutIndHandler(cmInstanceData_t *cmData);
void numberOfSecurityRegister(cmInstanceData_t *cmData, psm_t localPsm, CsrBtDeviceAddr deviceAddr, CsrUint8 *numOfOutgoing, CsrUint8 *numOfIncoming);
aclTable* returnAclTable(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr);
CsrUint16 returnL2capSmallestFlushTo(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr);
CsrBool CsrBtCmL2caSetElementCounter(cmInstanceData_t *cmData);
cmL2caConnElement * CsrBtCmRfcFindRfcConnElementFromDeviceAddrConnectedSRequestedMode(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr, CsrUint8 requestedMode);
CsrBool CsrBtCmL2caFindL2caPriorityDataChannel(CsrCmnListElm_t *elem, void *value);

/*************************************************************************************
 These functions are found in csr_bt_cm_l2cap_amp_handler.c
************************************************************************************/
void CsrBtCmL2caMoveChannelReqHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caMoveChannelResHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caMoveChannelIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caMoveChannelCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caMoveChannelCmpIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caAmpLinkLossIndHandler(cmInstanceData_t *cmData);
CsrUint16 * CsrBtCmL2caBuildFcsConftab(CsrBtAmpController controller, CsrUint16 *conftabLength);
CsrBool CsrBtCmL2caFcsReConfigEnable(BKV_ITERATOR_T conftabIter);

/*************************************************************************************
 These functions are found in csr_bt_cm_l2cap_connless_handler.c
************************************************************************************/
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
CsrBool CsrBtCmL2caConnlessDatawriteCfmHandler(cmInstanceData_t *cmData);
CsrBool CsrBtCmL2caConnlessDatareadIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caConnlessInit(cmInstanceData_t *cmData);
void CsrBtCmL2caMapFixedCidIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caMapFixedCidCfmHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caUnmapFixedCidIndHandler(cmInstanceData_t *cmData);
void CsrBtCmL2caConnlessDataReqHandler(cmInstanceData_t *cmData);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ndef _CM_L2CA_H */


