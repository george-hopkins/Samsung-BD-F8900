/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_gatt_private.h"

void CsrBtGattMsgTransport(void *msg)
{
    CsrMsgTransport(CSR_BT_GATT_IFACEQUEUE, CSR_BT_GATT_PRIM, msg);
}

/* Covers Registration and Un-register an application instance to Gatt */
CsrBtGattRegisterReq *CsrBtGattRegisterReq_struct(CsrSchedQid   qid,
                                                  CsrUint16     context)
{
    CsrBtGattRegisterReq *prim = (CsrBtGattRegisterReq *)
                                  CsrPmemAlloc(sizeof(CsrBtGattRegisterReq));

    prim->type      = CSR_BT_GATT_REGISTER_REQ;
    prim->qid       = qid;
    prim->context   = context;
    return prim;
}

CsrBtGattUnregisterReq *CsrBtGattUnregisterReq_struct(CsrBtGattId gattId)
{
    CsrBtGattUnregisterReq *prim = (CsrBtGattUnregisterReq *)
                                    CsrPmemAlloc(sizeof(CsrBtGattUnregisterReq));

    prim->type      = CSR_BT_GATT_UNREGISTER_REQ;
    prim->gattId    = gattId;
    return prim;
}

/* Covers connection (GAP) features */
CsrBtGattCentralReq *CsrBtGattCentralReq_struct(CsrBtGattId        gattId,
                                                CsrBtTypedAddr   address,
                                                CsrBtGattConnFlags flags,
                                                CsrUint16          preferredMtu)
{
    CsrBtGattCentralReq *prim;
    prim = (CsrBtGattCentralReq*)CsrPmemAlloc(sizeof(CsrBtGattCentralReq));
    prim->type = CSR_BT_GATT_CENTRAL_REQ;
    prim->gattId = gattId;
    prim->address = address;
    prim->flags = flags;
    prim->preferredMtu = preferredMtu;
    return prim;
}

CsrBtGattPeripheralReq *CsrBtGattPeripheralReq_struct(CsrBtGattId        gattId,
                                                      CsrBtTypedAddr   address,
                                                      CsrBtGattConnFlags flags,
                                                      CsrUint16          preferredMtu,
                                                      CsrUint8           advLength,
                                                      CsrUint8           *advData,
                                                      CsrUint8           scanrspLength,
                                                      CsrUint8           *scanrsp)
{
    CsrBtGattPeripheralReq *prim;
    prim = (CsrBtGattPeripheralReq*)CsrPmemAlloc(sizeof(CsrBtGattPeripheralReq));
    prim->type = CSR_BT_GATT_PERIPHERAL_REQ;
    prim->gattId = gattId;
    prim->address = address;
    prim->flags = flags;
    prim->advertisingDataLength = advLength;
    prim->advertisingData = advData;
    prim->scanResponseDataLength = scanrspLength;
    prim->scanResponseData = scanrsp;
    prim->preferredMtu = preferredMtu;
    return prim;
}

CsrBtGattBredrConnectReq *CsrBtGattBredrConnectReq_struct(CsrBtGattId gattId,
                                                          CsrBtTypedAddr address,
                                                          CsrBtGattConnFlags flags)
{
    CsrBtGattBredrConnectReq *prim;
    prim = (CsrBtGattBredrConnectReq*)CsrPmemAlloc(sizeof(CsrBtGattBredrConnectReq));
    prim->type = CSR_BT_GATT_BREDR_CONNECT_REQ;
    prim->gattId = gattId;
    prim->address = address;
    prim->flags = flags;
    return prim;
}

CsrBtGattBredrAcceptReq *CsrBtGattBredrAcceptReq_struct(CsrBtGattId gattId,
                                                        CsrBtGattConnFlags flags)
{
    CsrBtGattBredrAcceptReq *prim;
    prim = (CsrBtGattBredrAcceptReq*)CsrPmemAlloc(sizeof(CsrBtGattBredrAcceptReq));
    prim->type = CSR_BT_GATT_BREDR_ACCEPT_REQ;
    prim->gattId = gattId;
    prim->flags = flags;
    return prim;
}

CsrBtGattScanReq *CsrBtGattScanReq_struct(CsrBtGattId gattId,
                                          CsrBool enable,
                                          CsrBtGattScanFlags scanFlags,
                                          CsrUint8 filterAddrCount,
                                          CsrBtTypedAddr *filterAddr,
                                          CsrUint8 filterDataCount,
                                          CsrBtGattDataFilter *filterData)
{
    CsrBtGattScanReq *prim;
    prim = (CsrBtGattScanReq*)CsrPmemAlloc(sizeof(CsrBtGattScanReq));
    prim->type = CSR_BT_GATT_SCAN_REQ;
    prim->gattId = gattId;
    prim->enable = enable;
    prim->scanFlags = scanFlags;
    prim->filterAddrCount = filterAddrCount;
    prim->filterAddr = filterAddr;
    prim->filterDataCount = filterDataCount;
    prim->filterData = filterData;
    return prim;
}

CsrBtGattAdvertiseReq *CsrBtGattAdvertiseReq_struct(CsrBtGattId gattId,
                                                    CsrBool enable,
                                                    CsrBtGattConnFlags advFlags,
                                                    CsrUint8 advLength,
                                                    CsrUint8 *advData,
                                                    CsrUint8 scanrspLength,
                                                    CsrUint8 *scanrsp)
{
    CsrBtGattAdvertiseReq *prim;
    prim = (CsrBtGattAdvertiseReq*)CsrPmemAlloc(sizeof(CsrBtGattAdvertiseReq));
    prim->type = CSR_BT_GATT_ADVERTISE_REQ;
    prim->gattId = gattId;
    prim->enable = enable;
    prim->advertisingFlags = advFlags;
    prim->advertisingDataLength = advLength;
    prim->advertisingData = advData;
    prim->scanResponseDataLength = scanrspLength;
    prim->scanResponseData = scanrsp;
    return prim;
}

CsrBtGattDisconnectReq *CsrBtGattDisconnectReq_struct(CsrBtGattId gattId,
                                                      CsrBtConnId btConnId)
{
    CsrBtGattDisconnectReq *prim;
    prim = (CsrBtGattDisconnectReq*)CsrPmemAlloc(sizeof(CsrBtGattDisconnectReq));
    prim->type = CSR_BT_GATT_DISCONNECT_REQ;
    prim->gattId = gattId;
    prim->btConnId = btConnId;
    return prim;
}


/* Covers DataBase Configuration */
CsrBtGattDbAllocReq *CsrBtGattDbAllocReq_struct(CsrBtGattId gattId,
                                                CsrUint16   numOfAttrHandles,
                                                CsrUint16   preferredStartHandle)
{
    CsrBtGattDbAllocReq *prim   = (CsrBtGattDbAllocReq *)
                                   CsrPmemAlloc(sizeof(CsrBtGattDbAllocReq));

    prim->type                  = CSR_BT_GATT_DB_ALLOC_REQ;
    prim->gattId                = gattId;
    prim->numOfAttrHandles      = numOfAttrHandles;
    prim->preferredStartHandle  = preferredStartHandle; 
    return prim;
}

CsrBtGattDbDeallocReq *CsrBtGattDbDeallocReq_struct(CsrBtGattId gattId)
{
    CsrBtGattDbDeallocReq *prim = (CsrBtGattDbDeallocReq *)
                                   CsrPmemAlloc(sizeof(CsrBtGattDbDeallocReq));

    prim->type      = CSR_BT_GATT_DB_DEALLOC_REQ;
    prim->gattId    = gattId;
    return prim;
}

CsrBtGattDbAddReq *CsrBtGattDbAddReq_struct(CsrBtGattId gattId,
                                            CsrBtGattDb *db)
{
    CsrBtGattDbAddReq *prim = (CsrBtGattDbAddReq *)
                               CsrPmemAlloc(sizeof(CsrBtGattDbAddReq));

    prim->type      = CSR_BT_GATT_DB_ADD_REQ;
    prim->gattId    = gattId;
    prim->db        = db;
    return prim;
}

CsrBtGattDbRemoveReq *CsrBtGattDbRemoveReq_struct(CsrBtGattId       gattId,
                                                  CsrBtGattHandle   start,
                                                  CsrBtGattHandle   end)
{
    CsrBtGattDbRemoveReq *prim = (CsrBtGattDbRemoveReq *)
                                  CsrPmemAlloc(sizeof(CsrBtGattDbRemoveReq));

    prim->type      = CSR_BT_GATT_DB_REMOVE_REQ;
    prim->gattId    = gattId;
    prim->start     = start;
    prim->end       = end;
    return prim;
}

CsrBtGattDbAccessRes *CsrBtGattDbAccessRes_struct(CsrBtGattId              gattId,
                                                  CsrBtConnId              btConnId,
                                                  CsrBtGattHandle          attrHandle,
                                                  CsrBtGattDbAccessRspCode responseCode,
                                                  CsrUint16                valueLength,
                                                  CsrUint8                 *value)
{
    CsrBtGattDbAccessRes *prim = (CsrBtGattDbAccessRes *)
                                  CsrPmemAlloc(sizeof(CsrBtGattDbAccessRes));

    prim->type          = CSR_BT_GATT_DB_ACCESS_RES;
    prim->gattId        = gattId;
    prim->btConnId      = btConnId;
    prim->attrHandle    = attrHandle;
    prim->responseCode  = responseCode;
    prim->valueLength   = valueLength;
    prim->value         = value;
    return prim;
}

/* Covers Server Initiated Notification and Indication  */
CsrBtGattEventSendReq *CsrBtGattEventSendReq_struct(CsrBtGattId       gattId,
                                                    CsrBtConnId       btConnId,
                                                    CsrBtGattHandle   attrHandle,
                                                    CsrBtGattHandle   endGroupHandle,
                                                    CsrUint16         flags,
                                                    CsrUint16         valueLength,
                                                    CsrUint8          *value)
{
    CsrBtGattEventSendReq *prim = (CsrBtGattEventSendReq *)
                                     CsrPmemAlloc(sizeof(CsrBtGattEventSendReq));

    prim->type           = CSR_BT_GATT_EVENT_SEND_REQ;
    prim->gattId         = gattId;
    prim->btConnId       = btConnId;
    prim->attrHandle     = attrHandle;
    prim->endGroupHandle = endGroupHandle;
    prim->flags          = flags;
    prim->valueLength    = valueLength;
    prim->value          = value;
    return prim;
}

/* Covers item 2, Primary Service Discovery, in the GATT feature table  */
CsrBtGattDiscoverServicesReq *CsrBtGattDiscoverServicesReq_struct(CsrBtGattId gattId,
                                                                  CsrBtConnId btConnId,
                                                                  CsrBtUuid   uuid)
{
    CsrBtGattDiscoverServicesReq *prim = (CsrBtGattDiscoverServicesReq *)
                                          CsrPmemAlloc(sizeof(CsrBtGattDiscoverServicesReq));

    prim->type      = CSR_BT_GATT_DISCOVER_SERVICES_REQ;
    prim->gattId    = gattId;
    prim->btConnId  = btConnId;
    prim->uuid      = uuid;
    return prim;
}

/* Covers item 3, Relationship Discovery, in the GATT feature table */
CsrBtGattFindInclServicesReq *CsrBtGattFindInclServicesReq_struct(CsrBtGattId     gattId,
                                                                  CsrBtConnId     btConnId,
                                                                  CsrBtGattHandle startHandle,
                                                                  CsrBtGattHandle endGroupHandle)
{
    CsrBtGattFindInclServicesReq *prim = (CsrBtGattFindInclServicesReq *)
                                          CsrPmemAlloc(sizeof(CsrBtGattFindInclServicesReq));

    prim->type           = CSR_BT_GATT_FIND_INCL_SERVICES_REQ;
    prim->gattId         = gattId;
    prim->btConnId       = btConnId;
    prim->startHandle    = startHandle;
    prim->endGroupHandle = endGroupHandle;
    return prim;
}

/* Covers item 4, Characteristic Discovery, in the GATT feature table */
CsrBtGattDiscoverCharacReq *CsrBtGattDiscoverCharacReq_struct(CsrBtGattId     gattId,
                                                              CsrBtConnId     btConnId,
                                                              CsrBtUuid       uuid,
                                                              CsrBtGattHandle startHandle,
                                                              CsrBtGattHandle endGroupHandle)
{
    CsrBtGattDiscoverCharacReq *prim = (CsrBtGattDiscoverCharacReq *)
                                        CsrPmemAlloc(sizeof(CsrBtGattDiscoverCharacReq));

    prim->type           = CSR_BT_GATT_DISCOVER_CHARAC_REQ;
    prim->gattId         = gattId;
    prim->btConnId       = btConnId;
    prim->uuid           = uuid;
    prim->startHandle    = startHandle;
    prim->endGroupHandle = endGroupHandle;
    return prim;
}

/* Covers item 5, Characteristic Descriptor Discovery, in the GATT feature table */
CsrBtGattDiscoverCharacDescriptorsReq *CsrBtGattDiscoverCharacDescriptorsReq_struct(CsrBtGattId     gattId,
                                                                                    CsrBtConnId     btConnId,
                                                                                    CsrBtGattHandle startHandle,
                                                                                    CsrBtGattHandle endGroupHandle)
{
    CsrBtGattDiscoverCharacDescriptorsReq *prim = (CsrBtGattDiscoverCharacDescriptorsReq *)
                                                   CsrPmemAlloc(sizeof(CsrBtGattDiscoverCharacDescriptorsReq));

    prim->type           = CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ;
    prim->gattId         = gattId;
    prim->btConnId       = btConnId;
    prim->startHandle    = startHandle;
    prim->endGroupHandle = endGroupHandle;
    return prim;
}

/* Covers item 6, Characteristic Value Read, and item 10 Read Characteristic Descriptor Value 
   in the GATT feature table */
CsrBtGattReadReq *CsrBtGattReadReq_struct(CsrBtGattId     gattId,
                                          CsrBtConnId     btConnId,
                                          CsrBtGattHandle handle,
                                          CsrUint16       offset,
                                          CsrUint16       flags)
{
    CsrBtGattReadReq *prim = (CsrBtGattReadReq *)
                              CsrPmemAlloc(sizeof(CsrBtGattReadReq));

    prim->type      = CSR_BT_GATT_READ_REQ;
    prim->gattId    = gattId;
    prim->btConnId  = btConnId;
    prim->handle    = handle;
    prim->flags     = flags;
    prim->offset    = offset;    
    return prim;
}

CsrBtGattReadByUuidReq *CsrBtGattReadByUuidReq_struct(CsrBtGattId       gattId,
                                                      CsrBtConnId       btConnId,
                                                      CsrBtGattHandle   startHandle,
                                                      CsrBtGattHandle   endGroupHandle,
                                                      CsrBtUuid         uuid)
{
    CsrBtGattReadByUuidReq *prim = (CsrBtGattReadByUuidReq *)
                                    CsrPmemAlloc(sizeof(CsrBtGattReadByUuidReq));

    prim->type           = CSR_BT_GATT_READ_BY_UUID_REQ;
    prim->gattId         = gattId;
    prim->btConnId       = btConnId;
    prim->startHandle    = startHandle;
    prim->endGroupHandle = endGroupHandle;
    prim->uuid           = uuid;
    return prim;
}

CsrBtGattReadMultiReq *CsrBtGattReadMultiReq_struct(CsrBtGattId     gattId,
                                                    CsrBtConnId     btConnId,
                                                    CsrUint16       handlesCount,
                                                    CsrBtGattHandle *handles)
{
    CsrBtGattReadMultiReq *prim = (CsrBtGattReadMultiReq *)
                                   CsrPmemAlloc(sizeof(CsrBtGattReadMultiReq));

    prim->type           = CSR_BT_GATT_READ_MULTI_REQ;
    prim->gattId         = gattId;
    prim->btConnId       = btConnId;
    prim->handlesCount   = handlesCount;
    prim->handles        = handles;
    return prim;
}

/* Covers item 7, Characteristic Value Write, in the GATT feature table */
CsrBtGattWriteReq *CsrBtGattWriteReq_struct(CsrBtGattId             gattId,
                                            CsrBtConnId             btConnId,
                                            CsrUint16               flags,
                                            CsrUint16               attrWritePairsCount,
                                            CsrBtGattAttrWritePairs *attrWritePairs)
{
    CsrBtGattWriteReq *prim = (CsrBtGattWriteReq *)
                               CsrPmemAlloc(sizeof(CsrBtGattWriteReq));

    prim->type                = CSR_BT_GATT_WRITE_REQ;
    prim->gattId              = gattId;
    prim->btConnId            = btConnId;
    prim->flags               = flags;
    prim->attrWritePairsCount = attrWritePairsCount;
    prim->attrWritePairs      = attrWritePairs;
    return prim;
}

/* Covers item 8 and 9, Characteristic Value Indication/Notification, in the GATT feature table */
CsrBtGattWriteClientConfigurationReq *CsrBtGattWriteClientConfigurationReq_struct(CsrBtGattId             gattId,
                                                                                  CsrBtConnId             btConnId,
                                                                                  CsrBtTypedAddr        address,
                                                                                  CsrBtGattHandle         descriptorHandle,
                                                                                  CsrBtGattHandle         valueHandle,
                                                                                  CsrBtGattCliConfigBits  configuration)
{
    CsrBtGattWriteClientConfigurationReq *prim = (CsrBtGattWriteClientConfigurationReq *)
                                                  CsrPmemAlloc(sizeof(CsrBtGattWriteClientConfigurationReq));

    prim->type               = CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ;
    prim->gattId             = gattId;
    prim->btConnId           = btConnId;
    prim->address            = address;
    prim->clientConfigHandle = descriptorHandle;
    prim->valueHandle        = valueHandle;
    prim->configuration      = configuration;
    return prim;
}

/* Construct the CsrBtGattParamScanReq primitive */
CsrBtGattParamScanReq *CsrBtGattParamScanReq_struct(CsrBtGattId gattId,
                                                    CsrUint16 scanInterval,
                                                    CsrUint16 scanWindow)
{
    CsrBtGattParamScanReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtGattParamScanReq));
    prim->type = CSR_BT_GATT_PARAM_SCAN_REQ;
    prim->gattId = gattId;
    prim->scanInterval = scanInterval;
    prim->scanWindow = scanWindow;
    return prim;
}

/* Construct the CsrBtGattParamAdvertiseReq primitive */
CsrBtGattParamAdvertiseReq *CsrBtGattParamAdvertiseReq_struct(CsrBtGattId gattId,
                                                              CsrUint16 advIntervalMin,
                                                              CsrUint16 advIntervalMax,
                                                              CsrBtGattAdvChannel advertisingChannelMap)
{
    CsrBtGattParamAdvertiseReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtGattParamAdvertiseReq));
    prim->type = CSR_BT_GATT_PARAM_ADVERTISE_REQ;
    prim->gattId = gattId;
    prim->advIntervalMin = advIntervalMin;
    prim->advIntervalMax = advIntervalMax;
    prim->advertisingChannelMap = advertisingChannelMap;
    return prim;
}

#ifdef CSR_BT_INSTALL_GATT_PARAM_CONNECTION
/* Construct the CsrBtGattParamConnectionReq primitive */
CsrBtGattParamConnectionReq *CsrBtGattParamConnectionReq_struct(CsrBtGattId gattId,
                                                                CsrUint16 scanInterval,
                                                                CsrUint16 scanWindow,
                                                                CsrUint16 connIntervalMin,
                                                                CsrUint16 connIntervalMax,
                                                                CsrUint16 connLatency,
                                                                CsrUint16 supervisionTimeout,
                                                                CsrUint16 connAttemptTimeout,
                                                                CsrUint16 advIntervalMin,
                                                                CsrUint16 advIntervalMax,
                                                                CsrUint16 connLatencyMax,
                                                                CsrUint16 supervisionTimeoutMin,
                                                                CsrUint16 supervisionTimeoutMax)
{
    CsrBtGattParamConnectionReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtGattParamConnectionReq));
    prim->type = CSR_BT_GATT_PARAM_CONNECTION_REQ;
    prim->gattId = gattId;
    prim->scanInterval = scanInterval;
    prim->scanWindow = scanWindow;
    prim->connIntervalMin = connIntervalMin;
    prim->connIntervalMax = connIntervalMax;
    prim->connLatency = connLatency;
    prim->supervisionTimeout = supervisionTimeout;
    prim->connAttemptTimeout = connAttemptTimeout;
    prim->advIntervalMin = advIntervalMin;
    prim->advIntervalMax = advIntervalMax;
    prim->connLatencyMax = connLatencyMax;
    prim->supervisionTimeoutMin = supervisionTimeoutMin;
    prim->supervisionTimeoutMax = supervisionTimeoutMax;
    return prim;
}
#endif

/* Construct the CsrBtGattParamConnectionReq primitive */
CsrBtGattParamConnUpdateReq *CsrBtGattParamConnUpdateReq_struct(CsrBtGattId gattId,
                                                                CsrBtConnId btConnId,
                                                                CsrUint16 connIntervalMin,
                                                                CsrUint16 connIntervalMax,
                                                                CsrUint16 connLatency,
                                                                CsrUint16 supervisionTimeout,
                                                                CsrUint16 minimumCeLength,
                                                                CsrUint16 maximumCeLength,
                                                                CsrBool accept,
                                                                l2ca_identifier_t l2caSignalId)
{
    CsrBtGattParamConnUpdateReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtGattParamConnUpdateReq));
    prim->type = CSR_BT_GATT_PARAM_CONN_UPDATE_REQ;
    prim->gattId = gattId;
    prim->btConnId = btConnId;
    prim->connIntervalMin = connIntervalMin;
    prim->connIntervalMax = connIntervalMax;
    prim->connLatency = connLatency;
    prim->supervisionTimeout = supervisionTimeout;
    prim->minimumCeLength = minimumCeLength;
    prim->maximumCeLength = maximumCeLength;
    prim->accept = accept;
    prim->l2caSignalId = l2caSignalId;  
    return prim;
}

/* Construct the CsrBtGattWhitelistAddReq primitive */
CsrBtGattWhitelistAddReq *CsrBtGattWhitelistAddReq_struct(CsrBtGattId gattId,
                                                          CsrUint16 addressCount,
                                                          CsrBtTypedAddr *address)
{
    CsrBtGattWhitelistAddReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtGattWhitelistAddReq));
    prim->type = CSR_BT_GATT_WHITELIST_ADD_REQ;
    prim->gattId = gattId;
    prim->addressCount = addressCount;
    prim->address = address;
    return prim;
}

/* Construct the CsrBtGattWhitelistClearReq primitive */
CsrBtGattWhitelistClearReq *CsrBtGattWhitelistClearReq_struct(CsrBtGattId gattId)
{
    CsrBtGattWhitelistClearReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtGattWhitelistClearReq));
    prim->type = CSR_BT_GATT_WHITELIST_CLEAR_REQ;
    prim->gattId = gattId;
    return prim;
}

/* Construct the CsrBtGattWhitelistReadReq primitive */
CsrBtGattWhitelistReadReq *CsrBtGattWhitelistReadReq_struct(CsrBtGattId gattId)
{
    CsrBtGattWhitelistReadReq *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtGattWhitelistReadReq));
    prim->type = CSR_BT_GATT_WHITELIST_READ_REQ;
    prim->gattId = gattId;
    return prim;
}

/* Construct the CsrBtGattCancelReq primitive */
CsrBtGattCancelReq *CsrBtGattCancelReq_struct(CsrBtGattId gattId,
                                              CsrBtConnId btConnId)
{
    CsrBtGattCancelReq *prim = CsrPmemAlloc(sizeof(CsrBtGattCancelReq));
    prim->type     = CSR_BT_GATT_CANCEL_REQ;
    prim->gattId   = gattId;
    prim->btConnId = btConnId;
    return prim;
}

/* Construct the CsrBtGattSubscriptionReq primitive */
CsrBtGattSubscriptionReq *CsrBtGattSubscriptionReq_struct(CsrBtGattId     gattId,
                                                          CsrBtTypedAddr  address,
                                                          CsrBtGattHandle valueHandle,
                                                          CsrBool         subscribe)
{
    CsrBtGattSubscriptionReq *prim = (CsrBtGattSubscriptionReq *)
                                      CsrPmemAlloc(sizeof(CsrBtGattSubscriptionReq));

    prim->type          = CSR_BT_GATT_SUBSCRIPTION_REQ;
    prim->gattId        = gattId;
    prim->address       = address;
    prim->valueHandle   = valueHandle;
    prim->subscribe     = subscribe;
    return prim;
}

/* Construct the CsrBtGattSetEventMaskReq primitive */
CsrBtGattSetEventMaskReq *CsrBtGattSetEventMaskReq_struct(CsrBtGattId        gattId,
                                                          CsrBtGattEventMask eventMask)
{
    CsrBtGattSetEventMaskReq *prim = (CsrBtGattSetEventMaskReq *)
                                      CsrPmemAlloc(sizeof(CsrBtGattSetEventMaskReq));

    prim->type      = CSR_BT_GATT_SET_EVENT_MASK_REQ;
    prim->gattId    = gattId;
    prim->eventMask = eventMask;
    return prim;
}

/* Construct the CsrBtGattParamConnUpdateRes primitive */
CsrBtGattParamConnUpdateRes *CsrBtGattParamConnUpdateRes_struct(CsrBtGattId gattId, 
                                                                CsrBtConnId btConnId,
                                                                CsrUint16   identifier,
                                                                CsrBool     accept)
{
    CsrBtGattParamConnUpdateRes *prim = (CsrBtGattParamConnUpdateRes *)
                                         CsrPmemAlloc(sizeof(CsrBtGattParamConnUpdateRes));

    prim->type       = CSR_BT_GATT_PARAM_CONN_UPDATE_RES;
    prim->gattId     = gattId;
    prim->btConnId   = btConnId;
    prim->identifier = identifier;
    prim->accept     = accept;
    return prim;

}

/* Construct the CsrBtGattSecurityReq primitive */
CsrBtGattSecurityReq *CsrBtGattSecurityReq_struct(CsrBtGattId gattId, 
                                                  CsrBtConnId btConnId,
                                                  CsrBtGattSecurityFlags secRequirements)
{
    CsrBtGattSecurityReq *prim = (CsrBtGattSecurityReq *)
                                  CsrPmemAlloc(sizeof(CsrBtGattSecurityReq));

    prim->type                 = CSR_BT_GATT_SECURITY_REQ;
    prim->gattId               = gattId;
    prim->btConnId             = btConnId;
    prim->securityRequirements = secRequirements;
    return prim;
}



