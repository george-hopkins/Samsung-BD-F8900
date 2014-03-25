/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_HDP_MODULE

#include "csr_bt_hdp_prim_out.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"

static void CsrBtHdpMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_HDP_PRIM, msg);
}

void CsrBtHdpRegisterCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{

    CsrBtHdpRegisterCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtHdpRegisterCfm));

    pMsg->type = CSR_BT_HDP_REGISTER_CFM;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    CsrBtHdpMessagePut(appHandle, pMsg);
}

void CsrBtHdpRegisterIndSend(CsrSchedQid appHandle, CsrBtMdepId mdepId)
{

    CsrBtHdpRegisterInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_REGISTER_IND;
    pMsg->mdepId = mdepId;

    CsrBtHdpMessagePut(appHandle, pMsg);
}

void CsrBtHdpActivateCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{

    CsrBtHdpActivateCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_ACTIVATE_CFM;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    CsrBtHdpMessagePut(appHandle, pMsg);
}

void CsrBtHdpGetCapabCfmSend(CsrSchedQid appHandle,CsrBtDeviceAddr *addr, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
   CsrBtHdpCtrlGetCapabCfm *prim = CsrPmemAlloc(sizeof(CsrBtHdpCtrlGetCapabCfm));
   prim->type          = CSR_BT_HDP_CTRL_GET_CAPAB_CFM;
   CsrBtBdAddrCopy(&prim->deviceAddr, addr);
   prim->resultCode = resultCode;
   prim->resultSupplier = resultSupplier;

   CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpRemoteFeaturesIndSend(CsrSchedQid appHandle, CsrBtDeviceAddr *addr, CsrBtHdpSdpRecord *record,CsrUint32 hdpInstance)
{
    CsrBtHdpCtrlGetCapabInd *prim = CsrPmemAlloc(sizeof(CsrBtHdpCtrlGetCapabInd));

    prim->type          = CSR_BT_HDP_CTRL_GET_CAPAB_IND;
    prim->dataexchangeSpecification = record->dataExchangeSpecification;
    prim->supportedProcedures = record->mcapFeatures;
    prim->hdpInstanceId  = hdpInstance;
    prim->providerName   = record->providerName;
    prim->serviceDescrip = record->serviceDescription;
    prim->serviceName    = record->serviceName;
    CsrBtBdAddrCopy(&prim->deviceAddr, addr);
    prim->supportedFeatureListLen = record->supportedFeatureListLen;
    prim->supportedFeatureList = record->supportedFeatureList;
    
    CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpConnectAssociateChannelCfmSend(CsrSchedQid appHandle,
                                       CsrBtMdepId mdepId,
                                       CsrBtMdepDataType mdepdatatype,
                                       l2ca_mtu_t mtu,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier,
                                       CsrBtDeviceAddr *addr,
                                       CsrUint32 hdpInstanceId,
                                       CsrUint32 assocId)
{
    CsrBtHdpConnectAssociateChannelCfm *prim = CsrPmemAlloc(sizeof(CsrBtHdpConnectAssociateChannelCfm));

    prim->type          = CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM;
    prim->resultCode    = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->mdepId        = mdepId;
    prim->dataType      = mdepdatatype;
    prim->maxPacketLength = mtu;
    CsrBtBdAddrCopy(&prim->deviceAddr, addr);
    prim->hdpInstanceId = hdpInstanceId;
    prim->assocChannelId= assocId;
    prim->btConnId = CSR_BT_CONN_ID_INVALID;

    CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpAcceptConnectAssociateChannelIndSend(CsrSchedQid               appHandle,
                                                  CsrBtMdepId       mdepId,
                                                  CsrUint32          assocId,
                                                  CsrBtDeviceAddr   *bd_addr)
{
   CsrBtHdpAcceptConnectAssociateChannelInd *prim = CsrPmemAlloc(sizeof(CsrBtHdpAcceptConnectAssociateChannelInd));

   prim->type          = CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND;
   /*Based on the mdep id find out the local role and datatype*/
   CsrBtBdAddrCopy(&prim->deviceAddr, bd_addr);
   prim->assocChannelId= assocId;
   prim->mdepId        = mdepId;
   prim->btConnId = CSR_BT_CONN_ID_INVALID;

   CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpConnectAssociateChannelIndSend(CsrSchedQid appHandle,
                                       CsrBtDeviceAddr *bd_addr,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier,
                                       CsrBtMdepId mdepId,
                                       CsrBtMdepDataType dataType,
                                       CsrUint32 assocId,
                                       CsrUint16 mtu,
                                       CsrUint32 hdpInstanceId)
{
    CsrBtHdpConnectAssociateChannelInd *prim = CsrPmemAlloc(sizeof(CsrBtHdpConnectAssociateChannelInd));

    prim->type          = CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND;
    prim->resultCode    = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->mdepId        = mdepId;
    prim->maxPacketLength = mtu;
    prim->dataType      = dataType;
    CsrBtBdAddrCopy(&prim->deviceAddr, bd_addr);
    prim->hdpInstanceId = hdpInstanceId;    /*Check for proper value being sent*/
    prim->assocChannelId= assocId;
    prim->btConnId = CSR_BT_CONN_ID_INVALID;

    CsrBtHdpMessagePut(appHandle, prim);
}


void CsrBtHdpConnectVirtualChCfmSend(CsrSchedQid appHandle,
                                     CsrUint32 assocChId,
                                     CsrUint32 virtualChId,
                                     CsrUint16 maxPacketLength,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier)
{

    CsrBtHdpConnectVirtualChannelCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM;
    pMsg->assocChannelId = assocChId;
    pMsg->virtualChannelId = virtualChId;
    pMsg->maxPacketLength = maxPacketLength;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    pMsg->btConnId = CSR_BT_CONN_ID_INVALID;

    CsrBtHdpMessagePut(appHandle, pMsg);
}


void CsrBtHdpAcceptConnectVirtualChIndSend(CsrSchedQid appHandle,
                                CsrUint32 assocChId,
                                CsrUint32 virtualChId,
                                CsrUint8 virtualChannelConfig)
{

    CsrBtHdpAcceptConnectVirtualChannelInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND;
    pMsg->assocChannelId = assocChId;
    pMsg->virtualChannelId = virtualChId;
    pMsg->virtualChannelConfig = virtualChannelConfig;
    pMsg->btConnId = CSR_BT_CONN_ID_INVALID;

    CsrBtHdpMessagePut(appHandle, pMsg);
}

void CsrBtHdpConnectVirtualChIndSend(CsrSchedQid appHandle,
                                CsrUint32 assocChId,
                                CsrUint32 virtualChId,
                                CsrUint16 maxPacketLength,
                                CsrBtResultCode resultCode,
                                CsrBtSupplier resultSupplier)
{

    CsrBtHdpConnectVirtualChannelInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND;
    pMsg->assocChannelId = assocChId;
    pMsg->virtualChannelId = virtualChId;
    pMsg->maxPacketLength = maxPacketLength;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    pMsg->btConnId = CSR_BT_CONN_ID_INVALID;

    CsrBtHdpMessagePut(appHandle, pMsg);
}

void CsrBtHdpDisconnectAssociateChIndSend(CsrSchedQid appHandle,
                                     CsrUint32 assocChId,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier)
{
   CsrBtHdpDisconnectAssociateChannelInd *prim = CsrPmemAlloc(sizeof(CsrBtHdpDisconnectAssociateChannelInd));

   prim->type = CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND;
   prim->assocChId = assocChId;
   prim->resultCode = resultCode;
   prim->resultSupplier = resultSupplier;

   CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpDisconnectAssociateChCfmSend(CsrSchedQid appHandle,
                                     CsrUint32 assocChId,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier)
{
   CsrBtHdpDisconnectAssociateChannelCfm *prim = CsrPmemAlloc(sizeof(CsrBtHdpDisconnectAssociateChannelCfm));

   prim->type = CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM;
   prim->assocChId = assocChId;
   prim->resultCode = resultCode;
   prim->resultSupplier = resultSupplier;

   CsrBtHdpMessagePut(appHandle, prim);
}


void CsrBtHdpDisconnectVirtualChIndSend(CsrSchedQid appHandle,
                                   CsrUint32 virtualChId,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier)
{
   CsrBtHdpDisconnectVirtualChannelInd *prim = CsrPmemAlloc(sizeof(CsrBtHdpDisconnectVirtualChannelInd));

   prim->type = CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND;
   prim->virtualChId = virtualChId;
   prim->resultCode = resultCode;
   prim->resultSupplier = resultSupplier;

   CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpDisconnectVirtualChCfmSend(CsrSchedQid appHandle,
                                   CsrUint32 virtualChId,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier)
{
   CsrBtHdpDisconnectVirtualChannelCfm *prim = CsrPmemAlloc(sizeof(CsrBtHdpDisconnectVirtualChannelCfm));

   prim->type = CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM;
   prim->virtualChId = virtualChId;
   prim->resultCode = resultCode;
   prim->resultSupplier = resultSupplier;

   CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpDataIndSend(CsrSchedQid    appHandle,
                         CsrUint8       *payload,
                         CsrUint16      length,
                         CsrUint32      assocChId)
{
   CsrBtHdpDataInd *prim = CsrPmemAlloc(sizeof(CsrBtHdpDataInd));

   prim->type = CSR_BT_HDP_DATA_IND;
   prim->length = length;
   prim->data = payload;
   prim->assocChId = assocChId;

   CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpDataCfmSend(CsrSchedQid appHandle,CsrUint32 assocChId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
   CsrBtHdpDataCfm *prim = CsrPmemAlloc(sizeof(CsrBtHdpDataCfm));

   prim->type = CSR_BT_HDP_DATA_CFM;
   prim->resultCode = resultCode;
   prim->resultSupplier = resultSupplier;
   prim->assocChId = assocChId;

   CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpDeactivateCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtHdpDeactivateCfm *prim = CsrPmemAlloc(sizeof(CsrBtHdpDeactivateCfm ));

    prim->type = CSR_BT_HDP_DEACTIVATE_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtHdpMessagePut(appHandle, prim);
}

void CsrBtHdpSuspendCfmSend(CsrSchedQid appHandle,CsrUint32 assocChId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{

    CsrBtHdpSuspendCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_SUSPEND_CFM;
    pMsg->assocChId = assocChId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    CsrBtHdpMessagePut(appHandle, pMsg);
}



void CsrBtHdpResumeCfmSend(CsrSchedQid appHandle,CsrBtDeviceAddr *bd_addr,CsrUint32 assocChId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{

    CsrBtHdpResumeCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_RESUME_CFM;
    if ( bd_addr != NULL)
    {
        pMsg->deviceAddr = *bd_addr;
    }
    else
    {
        CsrBtBdAddrZero(&pMsg->deviceAddr);
    }

    pMsg->assocChId = assocChId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    CsrBtHdpMessagePut(appHandle, pMsg);
}


void CsrBtHdpResumeIndSend(CsrSchedQid appHandle,CsrUint32 chId,CsrBtDeviceAddr    deviceAddr)
{

    CsrBtHdpResumeInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_RESUME_IND;
    pMsg->chId = chId;
    pMsg->deviceAddr = deviceAddr;
    pMsg->resultCode = CSR_BT_RESULT_CODE_HDP_SUCCESS;
    pMsg->resultSupplier = CSR_BT_SUPPLIER_HDP;

    CsrBtHdpMessagePut(appHandle, pMsg);
}


void CsrBtHdpUnregisterCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{

    CsrBtHdpUnregisterCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_HDP_UNREGISTER_CFM;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    CsrBtHdpMessagePut(appHandle, pMsg);
}


#endif
