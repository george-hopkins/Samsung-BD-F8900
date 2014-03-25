/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_bccmd.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_events_handler.h"
#include "csr_bt_addr.h"

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
static void csrBtCmRfcExtractEScoParms(cmInstanceData_t *cmData, CsrBtCmSyncConnectInd *prim)
{
    cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromScoHandle, &(prim->syncHandle));

    if (theElement)
    {
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

        prim->txBdw           = theLogicalLink->eScoParms.txBdw;
        prim->rxBdw           = theLogicalLink->eScoParms.rxBdw;
        prim->maxLatency      = theLogicalLink->eScoParms.maxLatency;
        prim->voiceSettings   = theLogicalLink->eScoParms.voiceSettings;
        prim->reTxEffort      = theLogicalLink->eScoParms.reTxEffort;
        prim->packetType      = theLogicalLink->eScoParms.packetType;
    }
}
#endif

static void csrBtCmSyncConnectIndEventMsgSend(cmInstanceData_t        *cmData,
                                              CsrSchedQid                   appHandle,
                                              CsrBool                  incoming,
                                              hci_connection_handle_t  syncHandle,
                                              CsrBtDeviceAddr          deviceAddr,
                                              CsrUint8                 linkType,
                                              CsrUint8                 txInterval,
                                              CsrUint8                 weSco,
                                              CsrUint16                rxPacketLength,
                                              CsrUint16                txPacketLength,
                                              CsrUint8                 airMode,
                                              CsrBtResultCode          resultCode,
                                              CsrBtSupplier            resultSupplier)
{
    CsrBtCmSyncConnectInd *prim;

    prim                    = (CsrBtCmSyncConnectInd *)CsrPmemZalloc(sizeof(CsrBtCmSyncConnectInd));
    prim->type              = CSR_BT_CM_SYNC_CONNECT_IND;
    prim->incoming          = incoming;
    prim->syncHandle        = syncHandle;
    prim->deviceAddr        = deviceAddr;
    prim->linkType          = linkType;
    prim->txInterval        = txInterval;
    prim->weSco             = weSco;
    prim->rxPacketLength    = rxPacketLength;
    prim->txPacketLength    = txPacketLength;
    prim->airMode           = airMode;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    csrBtCmRfcExtractEScoParms(cmData, prim);
#endif

    CsrBtCmPutMessage(appHandle, prim);
}

static void csrBtCmSyncDisconnectIndEventMsgSend(cmInstanceData_t        *cmData,
                                                 CsrSchedQid                     appHandle,
                                                 CsrBtResultCode         resultCode,
                                                 CsrBtSupplier     resultSupplier,
                                                 hci_connection_handle_t syncHandle,
                                                 CsrBtDeviceAddr         deviceAddr,
                                                 hci_reason_t            reason)
{
    CsrBtCmSyncDisconnectInd *prim;

    prim                 = (CsrBtCmSyncDisconnectInd *)CsrPmemAlloc(sizeof(CsrBtCmSyncDisconnectInd));
    prim->type           = CSR_BT_CM_SYNC_DISCONNECT_IND;
    prim->syncHandle     = syncHandle;
    prim->deviceAddr     = deviceAddr;
    prim->reason         = reason;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(appHandle, prim);
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_BLUECORE_INITIALIZED
static void csrBtCmBlueCoreInitializedEventMsgSend(cmInstanceData_t        *cmData,
                                                   CsrSchedQid               appHandle)
{
    CsrBtCmBluecoreInitializedInd *prim;

    prim               = (CsrBtCmBluecoreInitializedInd *)CsrPmemAlloc(sizeof(CsrBtCmBluecoreInitializedInd));
    prim->type         = CSR_BT_CM_BLUECORE_INITIALIZED_IND;
    CsrBtCmPutMessage(appHandle, prim);
}
#endif

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION
static void csrBtCmExtSyncConnectIndEventMsgSend(cmInstanceData_t    *cmData,
                                            CsrSchedQid                   appHandle,
                                            CsrBool                  incoming,
                                            hci_connection_handle_t  syncHandle,
                                            CsrBtDeviceAddr          deviceAddr,
                                            CsrUint8                 linkType,
                                            CsrUint8                 txInterval,
                                            CsrUint8                 weSco,
                                            CsrUint8                 reservedSlots,
                                            CsrUint16                rxPacketLength,
                                            CsrUint16                txPacketLength,
                                            CsrUint8                 airMode,
                                            CsrBtResultCode          resultCode,
                                            CsrBtSupplier            resultSupplier)
{
    CsrBtCmExtSyncConnectInd *prim;

    prim                    = (CsrBtCmExtSyncConnectInd *)CsrPmemZalloc(sizeof(CsrBtCmExtSyncConnectInd));
    prim->type              = CSR_BT_CM_EXT_SYNC_CONNECT_IND;
    prim->incoming          = incoming;
    prim->syncHandle        = syncHandle;
    prim->deviceAddr        = deviceAddr;
    prim->linkType          = linkType;
    prim->txInterval        = txInterval;
    prim->weSco             = weSco;
    prim->reservedSlots     = reservedSlots;
    prim->rxPacketLength    = rxPacketLength;
    prim->txPacketLength    = txPacketLength;
    prim->airMode           = airMode;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    {
        cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromScoHandle, &(prim->syncHandle));

        if (theElement)
        {
            cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

            prim->txBdw           = theLogicalLink->eScoParms.txBdw;
            prim->rxBdw           = theLogicalLink->eScoParms.rxBdw;
            prim->maxLatency      = theLogicalLink->eScoParms.maxLatency;
            prim->voiceSettings   = theLogicalLink->eScoParms.voiceSettings;
            prim->reTxEffort      = theLogicalLink->eScoParms.reTxEffort;
            prim->packetType      = theLogicalLink->eScoParms.packetType;
        }
    }
#endif

    CsrBtCmPutMessage(appHandle, prim);
}
#endif
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION
static void csrBtCmAclConnectIndEventMsgSend(cmInstanceData_t *cmData,
                                             CsrSchedQid appHandle,
                                             CsrBtResultCode resultCode,
                                             CsrBtSupplier resultSupplier,
                                             CsrBtDeviceAddr deviceAddr,
                                             CsrBool incoming,
                                             CsrUint24 cod,
                                             CsrUint16 aclConnHandle)
{
    CsrBtCmAclConnectInd *prim;

    prim                 = (CsrBtCmAclConnectInd *)CsrPmemAlloc(sizeof(CsrBtCmAclConnectInd));
    prim->type           = CSR_BT_CM_ACL_CONNECT_IND;
    prim->deviceAddr     = deviceAddr;
    prim->incoming       = incoming;
    prim->cod            = cod;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->addressType    = CSR_BT_ADDR_PUBLIC;
    prim->aclConnHandle  = aclConnHandle;
    CsrBtCmPutMessage(appHandle, prim);
}

static void csrBtCmAclDisconnectIndEventMsgSend(cmInstanceData_t *cmData,
                                                CsrSchedQid appHandle,
                                                CsrBtReasonCode reasonCode,
                                                CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmAclDisconnectInd *prim;

    prim                    = (CsrBtCmAclDisconnectInd *)CsrPmemAlloc(sizeof(CsrBtCmAclDisconnectInd));
    prim->type              = CSR_BT_CM_ACL_DISCONNECT_IND;
    prim->deviceAddr        = deviceAddr;
    prim->reasonCode        = reasonCode;
    prim->reasonSupplier    = CSR_BT_SUPPLIER_HCI;
    prim->addressType       = CSR_BT_ADDR_PUBLIC;
    CsrBtCmPutMessage(appHandle, prim);
}
#endif

#if 0
static void csrBtCmSyncRenegotiateIndEventMsgSend(cmInstanceData_t        *cmData,
                                                  CsrSchedQid                   appHandle,
                                                  CsrBool                  incoming,
                                                  hci_connection_handle_t  syncHandle,
                                                  CsrBtDeviceAddr          deviceAddr)
{
    CsrBtCmSyncRenegotiateInd *prim;

    /* B-37386 */

    prim                = (CsrBtCmSyncRenegotiateInd *)CsrPmemAlloc(sizeof(CsrBtCmSyncRenegotiateInd));
    prim->type          = CSR_BT_CM_SYNC_RENEGOTIATE_IND;
    prim->incoming      = incoming;
    prim->syncHandle    = syncHandle;
    prim->deviceAddr    = deviceAddr;
    CsrBtCmPutMessage(appHandle, prim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LSTO_CHANGE
static void csrBtCmLstoChangeIndMsgSend(cmInstanceData_t        *cmData,
                                        CsrSchedQid             appHandle,
                                        CsrBtDeviceAddr          deviceAddr,
                                        CsrUint16              lsto)
{
    CsrBtCmLstoChangeInd    *cmPrim;

    cmPrim = (CsrBtCmLstoChangeInd *)CsrPmemAlloc(sizeof(CsrBtCmLstoChangeInd));

    cmPrim->type       = CSR_BT_CM_LSTO_CHANGE_IND;
    cmPrim->lsto       = lsto;
    cmPrim->deviceAddr = deviceAddr;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
static void csrBtCmModeChangeIndMsgSend(cmInstanceData_t    *cmData,
                                        CsrSchedQid                   appHandle,
                                        CsrBtDeviceAddr          deviceAddr,
                                        CsrUint16              interval,
                                        CsrUint8               mode,
                                        CsrBtResultCode       resultCode,
                                        CsrBtSupplier   resultSupplier)
{
    CsrBtCmModeChangeInd    *cmPrim;

    cmPrim = (CsrBtCmModeChangeInd *)CsrPmemAlloc(sizeof(CsrBtCmModeChangeInd));

    cmPrim->type            = CSR_BT_CM_MODE_CHANGE_IND;
    cmPrim->interval        = interval;
    cmPrim->mode            = mode;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    cmPrim->deviceAddr      = deviceAddr;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE
static void csrBtCmSwitchRoleIndMsgSend(cmInstanceData_t    *cmData,
                                        CsrSchedQid                 appHandle,
                                        CsrBtDeviceAddr     deviceAddr,
                                        CsrUint8             role,
                                        CsrBtResultCode     resultCode,
                                        CsrBtSupplier resultSupplier)
{
    CsrBtCmRoleChangeInd    *cmPrim;

    cmPrim = (CsrBtCmRoleChangeInd *)CsrPmemAlloc(sizeof(CsrBtCmRoleChangeInd));

    cmPrim->type            = CSR_BT_CM_ROLE_CHANGE_IND;
    cmPrim->deviceAddr      = deviceAddr;
    cmPrim->role            = role;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
static void csrBtCmSniffSubRatingIndMsgSend(cmInstanceData_t    *cmData,
                                            CsrSchedQid                   appHandle,
                                            CsrBtDeviceAddr       deviceAddr,
                                            CsrBtResultCode       resultCode,
                                            CsrBtSupplier   resultSupplier,
                                            CsrUint16              maxTxLatency,
                                            CsrUint16              maxRxLatency,
                                            CsrUint16              minRemoteTimeout,
                                            CsrUint16              minLocalTimeout)
{
    CsrBtCmSniffSubRatingInd *cmPrim;

    cmPrim = (CsrBtCmSniffSubRatingInd *)CsrPmemAlloc(sizeof(CsrBtCmSniffSubRatingInd));

    cmPrim->type                = CSR_BT_CM_SNIFF_SUB_RATING_IND;
    cmPrim->deviceAddr          = deviceAddr;
    cmPrim->resultCode          = resultCode;
    cmPrim->resultSupplier      = resultSupplier;
    cmPrim->maxTxLatency        = maxTxLatency;
    cmPrim->maxRxLatency        = maxRxLatency;
    cmPrim->minRemoteTimeout    = minRemoteTimeout;
    cmPrim->minLocalTimeout     = minLocalTimeout;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

static void csrBtCmSetEventMaskCfmMsgSend(CsrSchedQid appHandle,
                                          CsrUint32 eventMask)
{
    CsrBtCmSetEventMaskCfm *cmPrim;

    cmPrim            = (CsrBtCmSetEventMaskCfm *)CsrPmemAlloc(sizeof(CsrBtCmSetEventMaskCfm));
    cmPrim->type      = CSR_BT_CM_SET_EVENT_MASK_CFM;
    cmPrim->eventMask = eventMask;
    CsrBtCmPutMessage(appHandle, cmPrim);
}


#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
static void csrBtCmLogicalChannelTypesIndSend(CsrSchedQid    appHandle,
                                    CsrBtDeviceAddr     deviceAddr,
                                    CsrBtLogicalChannelType logicalChannelTypeMask,
                                    CsrUint8            noOfGuaranteedLogicalChannels)
{
    CsrBtCmLogicalChannelTypesInd *cmPrim = (CsrBtCmLogicalChannelTypesInd *)CsrPmemAlloc(sizeof(CsrBtCmLogicalChannelTypesInd));

    cmPrim->type                                = CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND;
    cmPrim->deviceAddr                          = deviceAddr;
    cmPrim->logicalChannelTypeMask              = logicalChannelTypeMask;
    cmPrim->numberOfGuaranteedLogicalChannels   = noOfGuaranteedLogicalChannels;

    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_FEATURES
static void csrBtCmRemoteFeaturesIndMsgSend(cmInstanceData_t    *cmData,
                                            CsrSchedQid              appHandle,
                                            CsrBtDeviceAddr     deviceAddr,
                                            CsrUint8            lmpFeatures[8],
                                            CsrBtResultCode     resultCode,
                                            CsrBtSupplier       resultSupplier)
{
    CsrBtCmRemoteFeaturesInd *cmPrim = (CsrBtCmRemoteFeaturesInd *)CsrPmemAlloc(sizeof(CsrBtCmRemoteFeaturesInd));

    cmPrim->type                 = CSR_BT_CM_REMOTE_FEATURES_IND;
    cmPrim->deviceAddr           = deviceAddr;
    cmPrim->resultCode           = resultCode;
    cmPrim->resultSupplier       = resultSupplier;
    CsrMemCpy(cmPrim->remoteLmpFeatures, lmpFeatures, sizeof(cmPrim->remoteLmpFeatures));
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_VERSION
static void csrBtCmRemoteVersionIndMsgSend(cmInstanceData_t *cmData,
                                           CsrSchedQid           appHandle,
                                           CsrBtDeviceAddr  deviceAddr,
                                           CsrUint8         lmpVersion,
                                           CsrUint16        manufacturerName,
                                           CsrUint16        lmpSubversion,
                                           CsrBtResultCode  resultCode,
                                           CsrBtSupplier    resultSupplier)
{
    CsrBtCmRemoteVersionInd *cmPrim = (CsrBtCmRemoteVersionInd *)CsrPmemAlloc(sizeof(CsrBtCmRemoteVersionInd));

    cmPrim->type                 = CSR_BT_CM_REMOTE_VERSION_IND;
    cmPrim->deviceAddr           = deviceAddr;
    cmPrim->lmpVersion           = lmpVersion;
    cmPrim->manufacturerName     = manufacturerName;
    cmPrim->lmpSubversion        = lmpSubversion;
    cmPrim->resultCode           = resultCode;
    cmPrim->resultSupplier       = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE
static void csrBtCmA2dpBitRateIndSend(cmInstanceData_t *cmData,
                                      CsrSchedQid            appHandle,
                                      CsrBtDeviceAddr   deviceAddr,
                                      CsrUint8          streamIdx,
                                      CsrUint32         bitRate)
{
    CsrBtCmA2dpBitRateInd *cmPrim = (CsrBtCmA2dpBitRateInd *)CsrPmemAlloc(sizeof(CsrBtCmA2dpBitRateInd));

    cmPrim->type                 = CSR_BT_CM_A2DP_BIT_RATE_IND;
    cmPrim->deviceAddr           = deviceAddr;
    cmPrim->streamIdx            = streamIdx;
    cmPrim->bitRate              = bitRate;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
static void csrBtCmInquiryPageStateIndSend(cmInstanceData_t *cmData,
                                      CsrSchedQid            appHandle,
                                      CsrBtCmInquiryEventType       inquiryState,
                                      CsrBtCmPagingEventType        pageState)
{
    CsrBtCmInquiryPageEventInd *cmPrim = (CsrBtCmInquiryPageEventInd *)CsrPmemAlloc(sizeof(CsrBtCmInquiryPageEventInd));

    cmPrim->type        = CSR_BT_CM_INQUIRY_PAGE_EVENT_IND;
    cmPrim->inquiry     = inquiryState;
    cmPrim->paging      = pageState;

    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE
static void csrBtCmEncryptChangeIndSend(cmInstanceData_t *cmData, 
                                        CsrSchedQid           appHandle,
                                        CsrBtDeviceAddr  deviceAddr,
                                        CsrBool          encrypted,
                                        CsrBtResultCode  resultCode,
                                        CsrBtSupplier    resultSupplier)
{
    CsrBtCmEncryptChangeInd  *cmPrim = (CsrBtCmEncryptChangeInd *)CsrPmemAlloc(sizeof(CsrBtCmEncryptChangeInd));

    cmPrim->type            = CSR_BT_CM_ENCRYPT_CHANGE_IND;
    cmPrim->deviceAddr      = deviceAddr;
    cmPrim->encrypted       = encrypted;
    cmPrim->resultCode      = resultCode;  
    cmPrim->resultSupplier  = resultSupplier;   
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA
static void csrBtCmHighPriorityDataIndSend(cmInstanceData_t *cmData,
                                           CsrSchedQid       appHandle,
                                           CsrBtDeviceAddr  *deviceAddr,
                                           CsrBool           start)
{
    CsrBtCmHighPriorityDataInd *cmPrim = (CsrBtCmHighPriorityDataInd *)CsrPmemAlloc(sizeof(CsrBtCmHighPriorityDataInd));

    cmPrim->type                 = CSR_BT_CM_HIGH_PRIORITY_DATA_IND;
    cmPrim->deviceAddr           = *deviceAddr;
    cmPrim->start                = start;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA */


static CsrBool csrBtCmAppSubscribedToEvent(subscribeParms *pSubscribeParms, CsrUint32 event)
{
    if (pSubscribeParms->appHandle == CSR_SCHED_QID_INVALID)
    {
        return FALSE;
    }
    else
    {
        if ((event & pSubscribeParms->eventMask) == event)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

static CsrBool csrBtCmAppCondSatisfied(subscribeParms *pSubscribeParms, CsrBtCmEventMaskCond condition)
{
    if (pSubscribeParms->appHandle == CSR_SCHED_QID_INVALID)
    {
        return FALSE;
    }
    else
    {
        if ((condition & pSubscribeParms->condition) == condition)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}
#define CM_UINT8_PTR_TO_VAL(a) *((CsrUint8 *) (a))

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
void CsrBtCmPropgateSyncConnectEvents(cmInstanceData_t  *cmData,
                                      CsrSchedQid             appHandle,
                                      void              *pContext1,
                                      void              *pContext2)
{
    DM_SYNC_CONNECT_COMPLETE_IND_T *dmPrim = (DM_SYNC_CONNECT_COMPLETE_IND_T *) pContext1;

    CsrUint8 incoming = CM_UINT8_PTR_TO_VAL(pContext2);

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmSyncConnectIndEventMsgSend(cmData,
                                          appHandle,
                                          incoming,
                                          dmPrim->handle,
                                          dmPrim->bd_addr,
                                          dmPrim->link_type,
                                          dmPrim->tx_interval,
                                          dmPrim->wesco,
                                          dmPrim->rx_packet_length,
                                          dmPrim->tx_packet_length,
                                          dmPrim->air_mode,
                                          CSR_BT_RESULT_CODE_CM_SUCCESS,
                                          CSR_BT_SUPPLIER_CM);
    }
    else
    {
        csrBtCmSyncConnectIndEventMsgSend(cmData,
                                          appHandle,
                                          incoming,
                                          dmPrim->handle,
                                          dmPrim->bd_addr,
                                          dmPrim->link_type,
                                          dmPrim->tx_interval,
                                          dmPrim->wesco,
                                          dmPrim->rx_packet_length,
                                          dmPrim->tx_packet_length,
                                          dmPrim->air_mode,
                                          (CsrBtResultCode) dmPrim->status,
                                          CSR_BT_SUPPLIER_HCI);
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmPropgateRfcExtSyncConnectEvents(cmInstanceData_t *cmData,
                                  CsrSchedQid             appHandle,
                                  void                  *pContext1,
                                  void                  *pContext2)
{
    cmRfcConnElement *currentElem = (cmRfcConnElement *)pContext1;

    csrBtCmExtSyncConnectIndEventMsgSend(cmData,
                                     appHandle,
                                     currentElem->cmRfcConnInst->eScoParms.incoming,
                                     currentElem->cmRfcConnInst->eScoParms.handle,
                                     currentElem->cmRfcConnInst->deviceAddr,
                                     currentElem->cmRfcConnInst->eScoParms.linkType,
                                     currentElem->cmRfcConnInst->eScoParms.txInterval,
                                     currentElem->cmRfcConnInst->eScoParms.weSco,
                                     currentElem->cmRfcConnInst->eScoParms.reservedSlots,
                                     currentElem->cmRfcConnInst->eScoParms.rxPacketLength,
                                     currentElem->cmRfcConnInst->eScoParms.txPacketLength,
                                     currentElem->cmRfcConnInst->eScoParms.airMode,
                                     CSR_BT_RESULT_CODE_CM_SUCCESS,
                                     CSR_BT_SUPPLIER_CM);

}
#endif
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
void CsrBtCmPropgateSyncDiscIndEvents(cmInstanceData_t *cmData,
                                      CsrSchedQid         appHandle,
                                      void              *pContext1,
                                      void              *pContext2)
{
    DM_SYNC_DISCONNECT_IND_T *dmPrim = (DM_SYNC_DISCONNECT_IND_T *) pContext1;

    CSR_UNUSED(pContext2);

    csrBtCmSyncDisconnectIndEventMsgSend(cmData,
                                         appHandle,
                                         CSR_BT_RESULT_CODE_CM_SUCCESS,
                                         CSR_BT_SUPPLIER_CM,
                                         dmPrim->handle,
                                         dmPrim->bd_addr,
                                         dmPrim->reason);
}

void CsrBtCmPropgateSyncDiscCfmEvents(cmInstanceData_t *cmData,
                                      CsrSchedQid         appHandle,
                                      void              *pContext1,
                                      void              *pContext2)
{
    DM_SYNC_DISCONNECT_CFM_T *dmPrim = (DM_SYNC_DISCONNECT_CFM_T *) pContext1;

    CSR_UNUSED(pContext2);

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmSyncDisconnectIndEventMsgSend(cmData,
                                             appHandle,
                                             CSR_BT_RESULT_CODE_CM_SUCCESS,
                                             CSR_BT_SUPPLIER_CM,
                                             dmPrim->handle,
                                             dmPrim->bd_addr,
                                             HCI_ERROR_UNSPECIFIED);
    }
    else
    {
        csrBtCmSyncDisconnectIndEventMsgSend(cmData,
                                             appHandle,
                                             (CsrBtResultCode) dmPrim->status,
                                             CSR_BT_SUPPLIER_HCI,
                                             dmPrim->handle,
                                             dmPrim->bd_addr,
                                             HCI_ERROR_UNSPECIFIED);
    }
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION
void CsrBtCmPropgateAclConnectEvents(cmInstanceData_t *cmData,
                                     CsrSchedQid            appHandle,
                                     void             *pContext1,
                                     void             *pContext2)
{
    DM_ACL_CONN_HANDLE_IND_T *dmPrim = (DM_ACL_CONN_HANDLE_IND_T *) pContext1;

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmAclConnectIndEventMsgSend(cmData,
                                         appHandle,
                                         CSR_BT_RESULT_CODE_CM_SUCCESS,
                                         CSR_BT_SUPPLIER_CM,
                                         dmPrim->addrt.addr,
                                         (CsrBool)(dmPrim->flags & DM_ACL_FLAG_INCOMING ? TRUE : FALSE),
                                         dmPrim->dev_class,
                                         dmPrim->acl_conn_handle);
    }
    else
    {
        csrBtCmAclConnectIndEventMsgSend(cmData,
                                         appHandle,
                                         (CsrBtResultCode) dmPrim->status,
                                         CSR_BT_SUPPLIER_HCI,
                                         dmPrim->addrt.addr,
                                         (CsrBool)(dmPrim->flags & DM_ACL_FLAG_INCOMING ? TRUE : FALSE),
                                         dmPrim->dev_class,
                                         CSR_BT_ACL_HANDLE_INVALID);
    }
}

void CsrBtCmPropgateAclDisconnectEvents(cmInstanceData_t *cmData,
                                        CsrSchedQid              appHandle,
                                        void             *pContext1,
                                        void             *pContext2)
{
    DM_ACL_CLOSED_IND_T *dmPrim = (DM_ACL_CLOSED_IND_T *) pContext1;

    csrBtCmAclDisconnectIndEventMsgSend(cmData,
                                        appHandle,
                                        (CsrBtReasonCode) dmPrim->reason,
                                        dmPrim->addrt.addr);
    CSR_UNUSED(pContext2);
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LSTO_CHANGE
void CsrBtCmPropgateLstoChangeEvents(cmInstanceData_t *cmData,
                                     CsrSchedQid             appHandle,
                                     void                  *pContext1,
                                     void                  *pContext2)
{
    DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *dmPrim = (DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *) pContext1;

    CSR_UNUSED(pContext2);

    csrBtCmLstoChangeIndMsgSend(cmData,
                                appHandle,
                                dmPrim->bd_addr,
                                dmPrim->timeout);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
void CsrBtCmPropgateModeChangeEvents(cmInstanceData_t *cmData,
                                     CsrSchedQid                 appHandle,
                                     void                *pContext1,
                                     void                *pContext2)
{
    DM_HCI_MODE_CHANGE_EVENT_IND_T *dmPrim = (DM_HCI_MODE_CHANGE_EVENT_IND_T *) pContext1;

    CSR_UNUSED(pContext2);

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmModeChangeIndMsgSend(cmData,
                                    appHandle,
                                    dmPrim->bd_addr,
                                    dmPrim->length,
                                    dmPrim->mode,
                                    CSR_BT_RESULT_CODE_CM_SUCCESS,
                                    CSR_BT_SUPPLIER_CM);
    }
    else
    {
        csrBtCmModeChangeIndMsgSend(cmData,
                                    appHandle,
                                    dmPrim->bd_addr,
                                    dmPrim->length,
                                    dmPrim->mode,
                                    (CsrBtResultCode) dmPrim->status,
                                    CSR_BT_SUPPLIER_HCI);
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE
void CsrBtCmPropgateRoleSwitchEvents(cmInstanceData_t *cmData,
                                     CsrSchedQid             appHandle,
                                     void                  *pContext1,
                                     void                  *pContext2)
{
    DM_HCI_SWITCH_ROLE_CFM_T *dmPrim = (DM_HCI_SWITCH_ROLE_CFM_T *) pContext1;

    CSR_UNUSED(pContext2);

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmSwitchRoleIndMsgSend(cmData,
                                    appHandle,
                                    dmPrim->bd_addr,
                                    dmPrim->role,
                                    CSR_BT_RESULT_CODE_CM_SUCCESS,
                                    CSR_BT_SUPPLIER_CM);
    }
    else
    {
        csrBtCmSwitchRoleIndMsgSend(cmData,
                                    appHandle,
                                    dmPrim->bd_addr,
                                    dmPrim->role,
                                    (CsrBtResultCode) dmPrim->status,
                                    CSR_BT_SUPPLIER_HCI);
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
void CsrBtCmPropgateSsrEvents(cmInstanceData_t *cmData,
                              CsrSchedQid             appHandle,
                              void                  *pContext1,
                              void                  *pContext2)
{
    DM_HCI_SNIFF_SUB_RATING_IND_T *dmPrim = (DM_HCI_SNIFF_SUB_RATING_IND_T *) pContext1;

    CSR_UNUSED(pContext2);

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmSniffSubRatingIndMsgSend(cmData,
                                        appHandle,
                                        dmPrim->bd_addr,
                                        CSR_BT_RESULT_CODE_CM_SUCCESS,
                                        CSR_BT_SUPPLIER_CM,
                                        dmPrim->transmit_latency,
                                        dmPrim->receive_latency,
                                        dmPrim->remote_timeout,
                                        dmPrim->local_timeout);
    }
    else
    {
        csrBtCmSniffSubRatingIndMsgSend(cmData,
                                        appHandle,
                                        dmPrim->bd_addr,
                                        (CsrBtResultCode) dmPrim->status,
                                        CSR_BT_SUPPLIER_HCI,
                                        dmPrim->transmit_latency,
                                        dmPrim->receive_latency,
                                        dmPrim->remote_timeout,
                                        dmPrim->local_timeout);
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_BLUECORE_INITIALIZED
void CsrBtCmPropgateBlueCoreInitializedEvents(cmInstanceData_t *cmData,
                                              CsrSchedQid             appHandle,
                                              void                  *pContext1,
                                              void                  *pContext2)
{
    CSR_UNUSED(pContext1);
    CSR_UNUSED(pContext2);

    if (cmData->globalState != notReady_s)
    {
        csrBtCmBlueCoreInitializedEventMsgSend(cmData, appHandle);
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
void CsrBtCmPropgateLogicalChannelTypeEvents(cmInstanceData_t *cmData,
                                  CsrSchedQid             appHandle,
                                  void               *pContext1,
                                  void               *pContext2)
{
    aclTable   *aclConElem = (aclTable *)pContext1;
    CSR_UNUSED(pContext2);

    csrBtCmLogicalChannelTypesIndSend(appHandle,aclConElem->deviceAddr,
                aclConElem->logicalChannelTypeMask,aclConElem->noOfGuaranteedLogicalChannels);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE
void CsrBtCmPropagateLocalNameChangeEvent(cmInstanceData_t *cmInst,
                                          CsrSchedQid appHandle,
                                          void *arg1, /* localName - copied */
                                          void *arg2) /* unused */
{
    CsrBtCmLocalNameChangeInd *ind;
    ind = CsrPmemAlloc(sizeof(CsrBtCmLocalNameChangeInd));
    ind->type = CSR_BT_CM_LOCAL_NAME_CHANGE_IND;
    ind->localName = (CsrUtf8String*)CsrStrDup((CsrCharString*)arg1);
    CsrBtCmPutMessage(appHandle, ind);
}

static void csrBtEventsCmPropagateExistingLocalName(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    if(cmData->dmVar.localName != NULL)
    {
        CsrBtCmPropagateLocalNameChangeEvent(cmData, appHandle,
                                             cmData->dmVar.localName,
                                             NULL);
    }
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE */

#ifdef CSR_BT_LE_ENABLE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
void CsrBtCmPropagateLeAdvertisingEvent(cmInstanceData_t *cmInst,
                                        CsrSchedQid appHandle,
                                        void *arg1, /* ptr to uint8(event) */
                                        void *arg2) /* unused */
{
    CsrBtCmLeEventAdvertisingInd *ind;
    CsrUint8 event;

    event = *(CsrUint8*)arg1;
    CSR_UNUSED(arg2);

    ind = CsrPmemAlloc(sizeof(CsrBtCmLeEventAdvertisingInd));
    ind->type = CSR_BT_CM_LE_EVENT_ADVERTISING_IND;
    ind->event = event;
    ind->advType = cmInst->leVar.advType;
    ind->intervalMin = cmInst->leVar.advIntervalMin;
    ind->intervalMax = cmInst->leVar.advIntervalMax;
    ind->channelMap = cmInst->leVar.advChannelMap;

    CsrBtCmPutMessage(appHandle, ind);
}

void CsrBtCmPropagateLeScanEvent(cmInstanceData_t *cmInst,
                                 CsrSchedQid appHandle,
                                 void *arg1, /* CsrUint8 ptr (event) */
                                 void *arg2) /* unused */
{
    CsrBtCmLeEventScanInd *ind;
    CsrUint8 event;

    event = *(CsrUint8*)arg1;

    ind = CsrPmemAlloc(sizeof(CsrBtCmLeEventScanInd));
    ind->type = CSR_BT_CM_LE_EVENT_SCAN_IND;
    ind->event = event;
    ind->scanType = cmInst->leVar.scanType;
    ind->interval = cmInst->leVar.scanInterval;
    ind->window = cmInst->leVar.scanWindow;

    CsrBtCmPutMessage(appHandle, ind);
}

/* Connection up, so send advertise/scan stop */
void CsrBtCmPropagateLeConnStopScanAdvEvent(cmInstanceData_t *cmInst,
                                            CsrSchedQid appHandle,
                                            void *arg1, /* unused */
                                            void *arg2) /* unused */
{
    CsrUint8 mode;
    mode = CSR_BT_CM_LE_MODE_OFF;

    if ((cmInst->leVar.scanMode == CSR_BT_CM_LE_MODE_ON)
        && (cmInst->leVar.scanType == CSR_BT_CM_LE_SCANTYPE_INITIATING))
    {
        CsrBtCmPropagateLeScanEvent(cmInst, appHandle,
                                    &mode, NULL);
    }
    else if ((cmInst->leVar.advMode == CSR_BT_CM_LE_MODE_ON)
             && ((cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_UNDIRECTED)
                 || (cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_DIRECTED)))
    {
        CsrBtCmPropagateLeAdvertisingEvent(cmInst, appHandle,
                                           &mode, NULL);
    }
}

void CsrBtCmPropagateLeConnectionEvent(cmInstanceData_t *cmInst,
                                       CsrSchedQid appHandle,
                                       void *arg1, /* DM_ACL_OPENED/CLOSED_IND ptr */
                                       void *arg2) /* unused */
{
    CsrUint16 type;
    CsrBtCmLeEventConnectionInd *ind;
    ind = CsrPmemZalloc(sizeof(CsrBtCmLeEventConnectionInd));

    type = *(CsrUint16*)arg1;
    ind->type = CSR_BT_CM_LE_EVENT_CONNECTION_IND;

    if(type == DM_ACL_CONN_HANDLE_IND)
    {
        DM_ACL_CONN_HANDLE_IND_T *dm = (DM_ACL_CONN_HANDLE_IND_T*)arg1;

        ind->event = (CsrUint8)(dm->status == HCI_SUCCESS
                                ? CSR_BT_CM_LE_MODE_ON
                                : CSR_BT_CM_LE_MODE_OFF);
        ind->deviceAddr = dm->addrt;
        ind->role = (CsrUint8)(dm->flags & DM_ACL_FLAG_INCOMING
                               ? HCI_SLAVE
                               : HCI_MASTER);
        ind->interval = dm->ble_conn_params.conn_interval;
        ind->timeout = dm->ble_conn_params.supervision_timeout;
        ind->latency = dm->ble_conn_params.conn_latency;
        ind->accuracy = dm->ble_conn_params.clock_accuracy;
    }
    else
    {
        DM_ACL_CLOSED_IND_T *dm;
        dm = (DM_ACL_CLOSED_IND_T*)arg1;

        ind->event = CSR_BT_CM_LE_MODE_OFF;
        ind->deviceAddr = dm->addrt;
    }

    CsrBtCmPutMessage(appHandle, ind);
}

static void csrBtEventsCmPropagateExistingLowEnergy(cmInstanceData_t *cmData,
                                                    CsrSchedQid appHandle)
{
    leConnVar *conn;

    CsrBtCmPropagateLeScanEvent(cmData, appHandle,
                                &(cmData->leVar.scanMode), NULL);
    CsrBtCmPropagateLeAdvertisingEvent(cmData, appHandle,
                                       &(cmData->leVar.advMode), NULL);

    /* Traverse LE connections */
    for(conn = cmData->leVar.connCache;
        conn != NULL;
        conn = conn->next)
    {
        /* Entire DM_ACL_CONN_HANDLE_IND_T stored in cache */
        CsrBtCmPropagateLeConnectionEvent(cmData,
                                          appHandle,
                                          &(conn->dmPrim),
                                          NULL);
    }
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY */
#endif /* CSR_BT_LE_ENABLE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_FEATURES
void CsrBtCmPropgateReadRemoteFeatureEvents(cmInstanceData_t *cmData,
                                            CsrSchedQid           appHandle,
                                            void             *pContext1,
                                            void             *pContext2)
{
    CsrUint8        lmpFeatures[8];
    CsrBtResultCode resultCode;
    CsrBtSupplier   resultSupplier;
    DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *dmPrim = (DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *) pContext1;

    CSR_UNUSED(pContext2);

    if (dmPrim->status == HCI_SUCCESS)
    {
        resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;
        resultSupplier  = CSR_BT_SUPPLIER_CM;
        CsrMemCpy(lmpFeatures, dmPrim->features, sizeof(lmpFeatures));

    }
    else
    {
        resultCode      = (CsrBtResultCode) dmPrim->status;
        resultSupplier  = CSR_BT_SUPPLIER_HCI;
        CsrMemSet(lmpFeatures, 0, sizeof(lmpFeatures));
    }
    csrBtCmRemoteFeaturesIndMsgSend(cmData,
                                    appHandle,
                                    dmPrim->bd_addr,
                                    lmpFeatures,
                                    resultCode,
                                    resultSupplier);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_VERSION
void CsrBtCmPropgateReadRemoteVersionEvents(cmInstanceData_t *cmData,
                                            CsrSchedQid           appHandle,
                                            void             *pContext1,
                                            void             *pContext2)
{
    CsrBtResultCode                   resultCode;
    CsrBtSupplier                     resultSupplier;
    DM_HCI_READ_REMOTE_VER_INFO_CFM_T *dmPrim = (DM_HCI_READ_REMOTE_VER_INFO_CFM_T *) pContext1;

    CSR_UNUSED(pContext2);

    if (dmPrim->status == HCI_SUCCESS)
    {
        resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;
        resultSupplier  = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        resultCode      = (CsrBtResultCode) dmPrim->status;
        resultSupplier  = CSR_BT_SUPPLIER_HCI;
    }

    csrBtCmRemoteVersionIndMsgSend(cmData,
                                   appHandle,
                                   dmPrim->addrt.addr,
                                   dmPrim->LMP_version,
                                   dmPrim->manufacturer_name,
                                   dmPrim->LMP_subversion,
                                   resultCode,
                                   resultSupplier);   
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
void CsrBtCmPropagateInquiryPageEvents(cmInstanceData_t *cmData,
                                            CsrSchedQid           appHandle,
                                            void             *pContext1,
                                            void             *pContext2)
{
    CsrBtCmInquiryEventType       inquiryState = CSR_BT_CM_INQUIRY_TYPE_STOP;
    CsrBtCmPagingEventType        pageState = CSR_BT_CM_PAGE_TYPE_STOP;
    CsrBool                       rfcRelatedPaging = FALSE;

    CSR_UNUSED(pContext2);

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    if (pContext1 != NULL)
    {
        cmRfcConnInstType * connInst = (cmRfcConnInstType *)pContext1;
        
        if ((connInst->state >= connectInit_s) && (connInst->state <= access_s))
        {
            rfcRelatedPaging = TRUE;
        }
    }
#endif

    if (cmData->dmVar.inquiryAppState == CM_INQUIRY_APP_STATE_INQUIRING)
    {/* Inquiry operation about to start or ongoing. In any case, do not indicate "STOP" */
        inquiryState = CSR_BT_CM_INQUIRY_TYPE_START;
    }

    if ( rfcRelatedPaging 
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
         ||(cmData->l2caVar.connectState != CM_L2CA_IDLE)
#endif
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        || (cmData->bnepVar.connectState != CM_BNEP_IDLE)
#endif
        )

    {/* Connection ongoing, but not completely established yet */
        pageState = CSR_BT_CM_PAGE_TYPE_START;
    }

    csrBtCmInquiryPageStateIndSend(cmData, appHandle, inquiryState, pageState);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE
void CsrBtCmPropagateA2DPBitRateEvents(cmInstanceData_t *cmData,
                                            CsrSchedQid           appHandle,
                                            void             *pContext1,
                                            void             *pContext2)
{
    CsrBtCmA2dpBitRateReq * prim = (CsrBtCmA2dpBitRateReq *)pContext1;

    CSR_UNUSED(pContext2);

    csrBtCmA2dpBitRateIndSend(cmData,appHandle,prim->deviceAddr,prim->streamIdx,prim->bitRate);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE
void CsrBtCmPropagateEncryptIndStatusEvents(cmInstanceData_t *cmData,
                                            CsrSchedQid           appHandle,
                                            void             *pContext1,
                                            void             *pContext2)
{
    DM_SM_ENCRYPTION_CHANGE_IND_T *dmPrim = (DM_SM_ENCRYPTION_CHANGE_IND_T *) pContext1;
    CSR_UNUSED(pContext2);

    csrBtCmEncryptChangeIndSend(cmData, 
                                appHandle,
                                dmPrim->addrt.addr,
                                dmPrim->encrypted,
                                CSR_BT_RESULT_CODE_CM_SUCCESS,
                                CSR_BT_SUPPLIER_CM);
}

void CsrBtCmPropagateEncryptCfmStatusEvents(cmInstanceData_t *cmData,
                                            CsrSchedQid           appHandle,
                                            void             *pContext1,
                                            void             *pContext2)
{
    CsrBtResultCode     resultCode;
    CsrBtSupplier       resultSupplier;
    DM_SM_ENCRYPT_CFM_T *dmPrim = (DM_SM_ENCRYPT_CFM_T *) pContext1;
    CSR_UNUSED(pContext2);

    if (dmPrim->success)
    {
        resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;
        resultSupplier  = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        resultCode      = (CsrBtResultCode) HCI_ERROR_UNSPECIFIED;
        resultSupplier  = CSR_BT_SUPPLIER_HCI;
    }

    csrBtCmEncryptChangeIndSend(cmData, 
                                appHandle,
                                dmPrim->bd_addr,
                                dmPrim->encrypted,
                                resultCode,
                                resultSupplier);
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA
void CsrBtCmPropagateHighPriorityIndStatusEvents(cmInstanceData_t *cmData,
                                                 CsrSchedQid       appHandle,
                                                 void             *pContext1,
                                                 void             *pContext2)
{
    CsrBtDeviceAddr *deviceAddr = ((CsrBtDeviceAddr *) pContext1);
    CsrBool start = *((CsrBool *) pContext2);

    csrBtCmHighPriorityDataIndSend(cmData,
                                   appHandle,
                                   deviceAddr,
                                   start);
}
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA */

static CsrBtCmEventMaskCond csrBtCmMapHciStatusToCondType(CsrUint8 status)
{
    if (status == HCI_SUCCESS)
    {
        return CSR_BT_CM_EVENT_MASK_COND_SUCCESS;
    }
    else
    {
        return CSR_BT_CM_EVENT_MASK_COND_UNKNOWN;
    }
}

void CsrBtCmPropgateEvent(cmInstanceData_t           *cmData,
                          cmEventHandlerFuncType eventHandler,
                          CsrUint32               eventMask,
                          CsrUint8                status,
                          void                   *pContext1,
                          void                   *pContext2)
{
    CsrUintFast16 listSize = CsrBtUtilBllGetNofEntriesEx(cmData->subscriptions);
    CsrUint16 dataSize;
    CsrUint16 idx;

    CsrBtCmEventMaskCond condition;

    subscribeParms *pSubscribeParms;

    condition = csrBtCmMapHciStatusToCondType(status);

    while (listSize > 0)
    {
        idx = listSize - 1;
        pSubscribeParms = (subscribeParms *) CsrBtUtilBllGetDataPointerEx(cmData->subscriptions,
                                                                        idx,
                                                                        &dataSize);

        CsrBtCmAssertRet(pSubscribeParms);

        if (csrBtCmAppSubscribedToEvent(pSubscribeParms, eventMask) &&
            csrBtCmAppCondSatisfied(pSubscribeParms, condition))
        {
            eventHandler(cmData, pSubscribeParms->appHandle, pContext1, pContext2);
        }
        listSize--;
    }
}

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_BLUECORE_INITIALIZED
static void csrBtCmPropagateExistingBlueCoreInit(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrBtCmPropgateBlueCoreInitializedEvents(cmData,
                                             appHandle,
                                             NULL,
                                             NULL);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION
static void csrBtCmPropagateExistingAclConns(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            csrBtCmAclConnectIndEventMsgSend(cmData,
                                             appHandle,
                                             CSR_BT_RESULT_CODE_CM_SUCCESS,
                                             CSR_BT_SUPPLIER_CM,
                                             cmData->roleVar.aclVar[i].deviceAddr,
                                             cmData->roleVar.aclVar[i].incoming,
                                             cmData->roleVar.aclVar[i].cod,
                                             cmData->roleVar.aclVar[i].aclConnHandle);
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
static void csrBtCmPropagateExistingSsr(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr) &&
           cmData->roleVar.aclVar[i].curSsrSettings.valid)
        {
            csrBtCmSniffSubRatingIndMsgSend(cmData,
                                            appHandle,
                                            cmData->roleVar.aclVar[i].deviceAddr,
                                            CSR_BT_RESULT_CODE_CM_SUCCESS,
                                            CSR_BT_SUPPLIER_CM,
                                            cmData->roleVar.aclVar[i].curSsrSettings.maxTxLatency,
                                            cmData->roleVar.aclVar[i].curSsrSettings.maxRxLatency,
                                            cmData->roleVar.aclVar[i].curSsrSettings.minRemoteTimeout,
                                            cmData->roleVar.aclVar[i].curSsrSettings.minLocalTimeout);
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE
static void csrBtCmPropagateExistingRole(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            csrBtCmSwitchRoleIndMsgSend(cmData,
                                        appHandle,
                                        cmData->roleVar.aclVar[i].deviceAddr,
                                        cmData->roleVar.aclVar[i].role,
                                        CSR_BT_RESULT_CODE_CM_SUCCESS,
                                        CSR_BT_SUPPLIER_CM);
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LSTO_CHANGE
static void csrBtCmPropagateExistingLsto(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            csrBtCmLstoChangeIndMsgSend(cmData,
                                        appHandle,
                                        cmData->roleVar.aclVar[i].deviceAddr,
                                        cmData->roleVar.aclVar[i].lsto);
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
static void csrBtCmPropagateExistingMode(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            csrBtCmModeChangeIndMsgSend(cmData,
                                        appHandle,
                                        cmData->roleVar.aclVar[i].deviceAddr,
                                        cmData->roleVar.aclVar[i].interval,
                                        cmData->roleVar.aclVar[i].mode,
                                        CSR_BT_RESULT_CODE_CM_SUCCESS,
                                        CSR_BT_SUPPLIER_CM);
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_FEATURES
static void csrBtCmPropagateExistingRemoteFeatures(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr) &&
           cmData->roleVar.aclVar[i].remoteFeaturesValid)
        {
            csrBtCmRemoteFeaturesIndMsgSend(cmData,
                                            appHandle,
                                            cmData->roleVar.aclVar[i].deviceAddr,
                                            cmData->roleVar.aclVar[i].remoteFeatures,
                                            CSR_BT_RESULT_CODE_CM_SUCCESS,
                                            CSR_BT_SUPPLIER_CM);
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_VERSION
static void csrBtCmPropagateExistingRemoteVersion(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i = 0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr) &&
           cmData->roleVar.aclVar[i].lmpVersion != CSR_BT_CM_INVALID_LMP_VERSION)
        {
            csrBtCmRemoteVersionIndMsgSend(cmData,
                                           appHandle,
                                           cmData->roleVar.aclVar[i].deviceAddr,
                                           cmData->roleVar.aclVar[i].lmpVersion,
                                           cmData->roleVar.aclVar[i].manufacturerName,
                                           cmData->roleVar.aclVar[i].lmpSubversion,
                                           CSR_BT_RESULT_CODE_CM_SUCCESS,
                                           CSR_BT_SUPPLIER_CM);
        }
    }
}
#endif

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE
static void csrBtCmPropagateA2DPBitRate(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;
    
    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            /* Now check all the L2CAP connections to the given bd address */
            cmL2caConnElement *currentElem;
            CsrUint8 strIdx = 0;

            for (currentElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentElem; currentElem = currentElem->next)
            { /* Search through the L2CAP connection list                     */
                if (currentElem->cmL2caConnInst)
                {
                    if ((currentElem->cmL2caConnInst->state == l2capConnected_s) &&
                        (bd_addr_eq(&(cmData->roleVar.aclVar[i].deviceAddr), &(currentElem->cmL2caConnInst->deviceAddr))) )
                    {
                        if (currentElem->cmL2caConnInst->logicalChannelTypeMask == CSR_BT_ACTIVE_STREAM_CHANNEL)
                        {/* A2DP stream channels are only possible in L2CAP connections */
                            csrBtCmA2dpBitRateIndSend(cmData, appHandle,
                                        cmData->roleVar.aclVar[i].deviceAddr,
                                        strIdx,
                                        CSR_BT_A2DP_BIT_RATE_UNKNOWN);
                            strIdx++;
                        }
                    }
                }
            }            
        }
    }
}
#endif
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
static void csrBtCmPropagateInquiryPageState(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    csrBtCmInquiryPageStateIndSend(cmData, appHandle,
                        CSR_BT_CM_INQUIRY_TYPE_STOP,
                        CSR_BT_CM_PAGE_TYPE_STOP);
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE
static void csrBtCmPropagateExistingEncryptStatus(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            csrBtCmEncryptChangeIndSend(cmData, 
                                        appHandle,
                                        cmData->roleVar.aclVar[i].deviceAddr,
                                        cmData->roleVar.aclVar[i].encrypted,
                                        CSR_BT_RESULT_CODE_CM_SUCCESS,
                                        CSR_BT_SUPPLIER_CM);
        }
    }
}
#endif

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
static void csrBtCmRfcPropagateExistingSyncConns(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    cmRfcConnElement *currentElem;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    { /* Search through the RFC connection table                                */
        if (currentElem->cmRfcConnInst)
        {
            if(currentElem->cmRfcConnInst->state == connected_s)
            {
                if(currentElem->cmRfcConnInst->eScoParms.handle != NO_SCO &&
                   currentElem->cmRfcConnInst->eScoParms.handle != SCOBUSY_ACCEPT)
                {
                    csrBtCmSyncConnectIndEventMsgSend(cmData,
                                                      appHandle,
                                                      currentElem->cmRfcConnInst->eScoParms.incoming,
                                                      currentElem->cmRfcConnInst->eScoParms.handle,
                                                      currentElem->cmRfcConnInst->deviceAddr,
                                                      currentElem->cmRfcConnInst->eScoParms.linkType,
                                                      currentElem->cmRfcConnInst->eScoParms.txInterval,
                                                      currentElem->cmRfcConnInst->eScoParms.weSco,
                                                      currentElem->cmRfcConnInst->eScoParms.rxPacketLength,
                                                      currentElem->cmRfcConnInst->eScoParms.txPacketLength,
                                                      currentElem->cmRfcConnInst->eScoParms.airMode,
                                                      CSR_BT_RESULT_CODE_CM_SUCCESS,
                                                      CSR_BT_SUPPLIER_CM);
                }
            }
        }
    }
}
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION
static void csrBtCmRfcPropagateExistingExtSyncConns(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    cmRfcConnElement *currentElem;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    { /* Search through the RFC connection table                                */
        if (currentElem->cmRfcConnInst)
        {
            if(currentElem->cmRfcConnInst->state == connected_s)
            {
                if(currentElem->cmRfcConnInst->eScoParms.handle != NO_SCO &&
                   currentElem->cmRfcConnInst->eScoParms.handle != SCOBUSY_ACCEPT)
                {
                    csrBtCmExtSyncConnectIndEventMsgSend(cmData,
                                             appHandle,
                                             currentElem->cmRfcConnInst->eScoParms.incoming,
                                             currentElem->cmRfcConnInst->eScoParms.handle,
                                             currentElem->cmRfcConnInst->deviceAddr,
                                             currentElem->cmRfcConnInst->eScoParms.linkType,
                                             currentElem->cmRfcConnInst->eScoParms.txInterval,
                                             currentElem->cmRfcConnInst->eScoParms.weSco,
                                             currentElem->cmRfcConnInst->eScoParms.reservedSlots,
                                             currentElem->cmRfcConnInst->eScoParms.rxPacketLength,
                                             currentElem->cmRfcConnInst->eScoParms.txPacketLength,
                                             currentElem->cmRfcConnInst->eScoParms.airMode,
                                             CSR_BT_RESULT_CODE_CM_SUCCESS,
                                             CSR_BT_SUPPLIER_CM);
                }
            }
        }
    }
}
#endif
#endif
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
static void csrBtCmPropagateLogicalChannelType(cmInstanceData_t *cmData, CsrSchedQid appHandle)
{
    CsrUintFast8 i =0;
    aclTable    *aclConnectionElement = NULL;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if(!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr))
        {
            aclConnectionElement = &(cmData->roleVar.aclVar[i]);
            csrBtCmLogicalChannelTypesIndSend(appHandle,
                                    cmData->roleVar.aclVar[i].deviceAddr,
                                    aclConnectionElement->logicalChannelTypeMask,
                                    aclConnectionElement->noOfGuaranteedLogicalChannels);
        }
    }
}
#endif

static void csrBtCmPropogateAllNewEvents(cmInstanceData_t *cmData, CsrSchedQid appHandle, CsrUint32 oldMask, CsrUint32 newMask)
{
    CsrUintFast8 i;
    CsrUint32 diffMask;

    if (oldMask == CSR_BT_CM_EVENT_MASK_SUBSCRIBE_NONE)
    {
        diffMask = newMask;
    }
    else
    {
        diffMask = ~oldMask & newMask;
    }

    for (i = 0; i < CSR_BT_CM_NUM_OF_CM_EVENTS; i++)
    {
        if (((diffMask>>i) & 0x000000001) != 0)
        {
            switch (diffMask & (1<<i))
            {
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION:
                    {

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
                        csrBtCmRfcPropagateExistingSyncConns(cmData, appHandle);
#endif
                        break;
                    }
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION:
                    {
                        csrBtCmPropagateExistingAclConns(cmData, appHandle);
                        break;
                    }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE:
                {
                        csrBtCmPropagateExistingRole(cmData, appHandle);
                        break;
                    }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE:
                    {
                        csrBtCmPropagateExistingMode(cmData, appHandle);
                        csrBtCmPropagateExistingSsr(cmData, appHandle);
                        break;
                    }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LSTO_CHANGE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LSTO_CHANGE:
                    {
                        csrBtCmPropagateExistingLsto(cmData, appHandle);
                        break;
                    }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_BLUECORE_INITIALIZED
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_BLUECORE_INITIALIZED:
                {
                    csrBtCmPropagateExistingBlueCoreInit(cmData, appHandle);
                    break;
                }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE:
                {
                    csrBtCmPropagateLogicalChannelType(cmData, appHandle);
                    break;
                }
#endif

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION:
                {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
                    csrBtCmRfcPropagateExistingExtSyncConns(cmData, appHandle);
#endif
                    break;
                }
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_EXT_SYNC_CONNECTION */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_FEATURES
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_REMOTE_FEATURES:
                {
                    csrBtCmPropagateExistingRemoteFeatures(cmData, appHandle);
                    break;
                }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_VERSION
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_REMOTE_VERSION:
                {
                    csrBtCmPropagateExistingRemoteVersion(cmData, appHandle);
                    break;
                }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE:
                {
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
                    /* A2DP streams are only possible on L2CAP channels. */
                    csrBtCmPropagateA2DPBitRate(cmData, appHandle);
#endif
                    break;
                }
#endif /* CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_A2DP_BIT_RATE */

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE:
                {
                    csrBtCmPropagateInquiryPageState(cmData, appHandle);
                    break;
                }
#endif

#ifdef CSR_BT_LE_ENABLE
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY:
                {
                    csrBtEventsCmPropagateExistingLowEnergy(cmData, appHandle);
                    break;
                }
#endif
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE:
                {
                    csrBtEventsCmPropagateExistingLocalName(cmData, appHandle);
                    break;
                }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE:
                {
                    csrBtCmPropagateExistingEncryptStatus(cmData, appHandle);
                    break;
                }
#endif

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA
                case CSR_BT_CM_EVENT_MASK_SUBSCRIBE_HIGH_PRIORITY_DATA:
                {
                    /* Do nothing */
                    break;
                }
#endif
                default:
                    {
                        break;
                    }
            }
        }
        else
        {
            ;
        }
    }
}

void CsrBtCmStoreDownstreamEScoParms(eScoParmVars *eScoParms,
                                     CsrBool     incoming,
                                     CsrUint8    linkType,
                                     CsrUint8    txInterval,
                                     CsrUint8    weSco,
                                     CsrUint16   rxPacketLength,
                                     CsrUint16   txPacketLength,
                                     CsrUint8    airMode,
                                     CsrUint8    status)
{
    eScoParms->incoming = incoming;

    if (status == HCI_SUCCESS)
    {
        eScoParms->linkType       = linkType;
        eScoParms->txInterval     = txInterval;
        eScoParms->weSco          = weSco;
        eScoParms->rxPacketLength = rxPacketLength;
        eScoParms->txPacketLength = txPacketLength;
        eScoParms->airMode        = airMode;
    }
}

void CsrBtCmSetEventMaskReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSetEventMaskReq *prim = (CsrBtCmSetEventMaskReq *) cmData->recvMsgP;
    subscribeParms *pSubscribeParms;
    CsrUint32 newEventMask;

    CsrUint32 eventMask = prim->eventMask & CSR_BT_CM_EVENT_MASK_RESERVER_VALUES_MASK;

    pSubscribeParms   = CsrBtUtilBllFindDataPointer(cmData->subscriptions,
                                                    &prim->phandle,
                                                    CsrBtUtilBllCompareDataUint16FuncType);

    if (eventMask == CSR_BT_CM_EVENT_MASK_SUBSCRIBE_NONE)
    {
        /* The app has requested to unsubscribe from all current event */
        cmData->subscriptions = CsrBtUtilBllFreeLinkedListEntryByDataPointer(cmData->subscriptions,
                                                                             pSubscribeParms,
                                                                             CsrBtUtilBllPfreeWrapper);
        csrBtCmSetEventMaskCfmMsgSend(prim->phandle, eventMask);
    }
    else
    {
        if (!pSubscribeParms)
        {
            /* Unknown app, so create a new instance to store the subscription */
            pSubscribeParms            = pnew(subscribeParms);
            pSubscribeParms->eventMask = CSR_BT_CM_EVENT_MASK_SUBSCRIBE_NONE;
            cmData->subscriptions      = CsrBtUtilBllCreateNewEntry(cmData->subscriptions,
                                                                    (CsrUint8 *) pSubscribeParms,
                                                                    sizeof(subscribeParms));
        }

        pSubscribeParms->condition = prim->conditionMask;
        pSubscribeParms->appHandle = prim->phandle;

        newEventMask = eventMask | pSubscribeParms->eventMask;

        csrBtCmSetEventMaskCfmMsgSend(prim->phandle, newEventMask);

        csrBtCmPropogateAllNewEvents(cmData, prim->phandle, pSubscribeParms->eventMask, eventMask);

        pSubscribeParms->eventMask = newEventMask;
    }
}
