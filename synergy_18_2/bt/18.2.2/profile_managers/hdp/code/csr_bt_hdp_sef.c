/****************************************************************************

(c) Cambridge Silicon Radio Limited 2009

All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_HDP_MODULE

#include "csr_bt_hdp_sef.h"
#include "csr_bt_hdp_prim.h"
#include "csr_bt_hdp_util.h"
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_hdp_prim_out.h"
#include "csr_bt_sdc_support.h"
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

void CsrBtHdpSdcSearchCancel(HdpInstanceData *pInstance)
{
    /*Has to be used when you think a serach is active and you need to shut down*/
    pInstance->sdpState = HDP_SDP_IDLE_STATE;
    CsrBtUtilSdcSearchCancel((void *)pInstance, pInstance->sdpSearchData);
}

void CsrBtHdpSdpRestartSearch(HdpInstanceData *pInstance)
{
    if(pInstance->sdpState == HDP_SDP_PENDING_STATE)
    {
        CsrBtHdpGetCapabReqHandler(pInstance);
    }

}


void CsrBtHdpSdcResultHandler(void                      *inst,
                              CmnCsrBtLinkedListStruct  *sdpTagList,
                              CsrBtDeviceAddr           deviceAddr,
                              CsrBtResultCode           resultCode,
                              CsrBtSupplier             resultSupplier)
{
    HdpInstanceData *instData = (HdpInstanceData *) inst;
    CsrUint32    handle = 0;

    if (HDP_SDP_ACTIVE_STATE == instData->sdpState)
    {
        if (CSR_BT_RESULT_CODE_HDP_SUCCESS == resultCode &&
            CSR_BT_SUPPLIER_CM == resultSupplier &&
            (sdpTagList !=NULL))
        {
            CsrUint16   tmpResult,dummy1, dummy2;
            CsrBtUuid32 tmpUuid;
            CsrUintFast16   sdpRecordIndex, numOfSdpRecords = CsrBtUtilBllGetNofEntriesEx(sdpTagList);

            for (sdpRecordIndex = 0; sdpRecordIndex < numOfSdpRecords; sdpRecordIndex++)
            {/* Handle each service record */
                CsrBtHdpSdpRecord record;
                CsrBtHdpSdpInitServiceRecordStruct(&record);
                if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList,
                                                          sdpRecordIndex,
                                                          &tmpUuid,
                                                          &tmpResult,
                                                          &dummy1,
                                                          &dummy2))
                {
                    if (SDR_SDC_SEARCH_SUCCESS == tmpResult)
                    {
                        if(CsrBtHdpSdpExtractServiceRecordData(sdpTagList,(CsrUint16)sdpRecordIndex,&record,&handle,instData->mdepDataTypeMask,instData->roleMask))
                        {
                            /* Send indication to app */
                            if(instData->state == HDP_SUSPEND_STATE)
                            {
                                instData->hdpInstanceId = handle;
                                CsrBtHdpUtilUpdateConnectionListForReconnect(instData,&record,&handle,&deviceAddr);
                                instData->sdpState = HDP_SDP_IDLE_STATE;
                                CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
                                CsrPmemFree(record.serviceDescription);
                                CsrPmemFree(record.providerName);
                                CsrPmemFree(record.serviceName);
                                CsrPmemFree(record.supportedFeatureList);
                                return;
                            }
                            CsrBtHdpRemoteFeaturesIndSend(instData->appHandle,
                                                          &deviceAddr,
                                                          &record,
                                                          handle);
                        }
                    }
                }
            }
        }
    }
    else
    {
        /* Device is unknown or in incorrect state - just ignore */
    }
    if(instData->getCapab == HDP_SDP_USER_INITIATED)
    {
        CsrBtHdpGetCapabCfmSend(instData->appHandle,
                                &deviceAddr,
                                resultCode,
                                resultSupplier);
    }
    else
    {
        instData->hdpInstanceId = handle;
    }
    instData->sdpState = HDP_SDP_IDLE_STATE;
    CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
}

/********************************************************/
/*                                                      */
/*    HDP primitive handlers                           */
/*                                                      */
/********************************************************/

void CsrBtHdpGetCapabReqHandler(HdpInstanceData * pInstance)
{

    CsrBtHdpCtrlGetCapabReq *msg = (CsrBtHdpCtrlGetCapabReq *)pInstance->recvMsgP;

    pInstance->mdepDataTypeMask = msg->mdepDataTypeMask;
    pInstance->roleMask = msg->mdepRoleMask;
    pInstance->appHandle = msg->appHandle;
    pInstance->getCapab = HDP_SDP_USER_INITIATED;
    CsrBtHdpSdcStartSearch(pInstance,msg->deviceAddr);
}


void CsrBtHdpIdleRegisterReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpRegisterReq    *pMsg;

    pMsg = (CsrBtHdpRegisterReq *)pInstance->recvMsgP;

    if ( pInstance->psm.control == HDP_PSM_INVALID &&
        pInstance->psm.data == HDP_PSM_INVALID)
    {
        CsrBtHdpCopyRegisterReqAttributes(pInstance, pMsg);
    }

    if ( pInstance->hServiceRecord == HDP_SDP_INVALID_SR_HANDLE )
    {
        pInstance->aux_state = HDP_IDLE_W4_PSM;

        CsrBtCml2caRegisterReqExtSend(CSR_BT_HDP_IFACEQUEUE,
                                        CSR_BT_ASSIGN_DYNAMIC_PSM,
                                        (L2CA_MODE_MASK_ENHANCED_RETRANS | L2CA_MODE_MASK_STREAMING),
                                        0); /*flags*/
    }
    else
    {
        CsrBtHdpRegisterCfmSend(pMsg->appHandle,
            CSR_BT_RESULT_CODE_HDP_ALREADY_REGISTERED,
            CSR_BT_SUPPLIER_HDP);
    }
}

void CsrBtHdpInvalidRegisterReqEvHandler(HdpInstanceData *pInstance)

{
    CsrBtHdpRegisterReq    *pMsg;

    pMsg = (CsrBtHdpRegisterReq *)pInstance->recvMsgP;

    CsrBtHdpRegisterCfmSend(pMsg->appHandle, CSR_BT_RESULT_CODE_HDP_INVALID_OP, CSR_BT_SUPPLIER_HDP);

}

void CsrBtHdpIdleRegisterRspEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpRegisterRsp    *pMsg;

    HdpMdepListEntry  *pMdepNode;
    CsrUint8 *csrBtHdpServiceRecord;
    CsrUint16 csrBtHdpServiceRecordLen;
    CsrUint32 mdepDescriptionLen;

    pMsg = (CsrBtHdpRegisterRsp *)pInstance->recvMsgP;



    if (pInstance->state == HDP_IDLE_STATE )
    {
        pInstance->countNumOfMdep++;

        pMdepNode = HDP_MDEP_ADD_ENTRY(pInstance->mdepList);


        pMdepNode->mdep.id = ( pMsg->reuseMdepId ) ?  pInstance->currentMdepId :
        ++pInstance->currentMdepId;

        pMdepNode->mdep.dataType = pMsg->datatype;
        pMdepNode->mdep.role     = pMsg->role;
        if(pMsg->description != NULL )
        {
            mdepDescriptionLen =  HdpStrLen((char *)(pMsg->description));
            mdepDescriptionLen = (mdepDescriptionLen > CSR_BT_MAX_FRIENDLY_NAME_LEN ) ? CSR_BT_MAX_FRIENDLY_NAME_LEN : mdepDescriptionLen;

            CsrMemCpy(&(pMdepNode->mdep.description[0]),pMsg->description,mdepDescriptionLen);
            CsrUtf8StrTruncate(pMdepNode->mdep.description, mdepDescriptionLen);
            CsrPmemFree(pMsg->description);
        }
        else
        {
            pMdepNode->mdep.description[0] = '\0';
        }
        pMdepNode->busy = FALSE;

        if (pInstance->countNumOfMdep < pInstance->numOfMdep )
        {
            CsrBtHdpRegisterIndSend(pInstance->appHandle, pInstance->currentMdepId);
        }
        else
        {
            csrBtHdpServiceRecordLen = buildHdpServiceRecord(pInstance,&csrBtHdpServiceRecord);
            pInstance->aux_state = HDP_IDLE_W4_SERVICE_HANDLE;
            CsrBtCmSdsRegisterReqSend(CSR_BT_HDP_IFACEQUEUE, csrBtHdpServiceRecord, csrBtHdpServiceRecordLen);
        }
    }
    else
    {
        CsrBtHdpRegisterCfmSend(pInstance->appHandle, CSR_BT_RESULT_CODE_HDP_INVALID_OP, CSR_BT_SUPPLIER_HDP);
    }

}


void CsrBtHdpInvalidRegisterRspEvHandler(HdpInstanceData *pInstance)

{
    CsrBtHdpRegisterCfmSend(pInstance->appHandle, CSR_BT_RESULT_CODE_HDP_INVALID_OP, CSR_BT_SUPPLIER_HDP);
}


void CsrBtHdpIdleActivateReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpActivateReq    *pMsg = (CsrBtHdpActivateReq *)pInstance->recvMsgP;

    /* check if registered and not activated */
    if ( ( pInstance->hServiceRecord != HDP_SDP_INVALID_SR_HANDLE) &&
        ( pInstance->aux_state != HDP_IDLE_CONNECTABLE))
    {

        CsrBtMcapActivateReqSend(pInstance->psm.control,
                                CSR_BT_HDP_IFACEQUEUE,
                                0,
                                (SECL4_IN_SSP | SECL_IN_AUTHENTICATION | SECL_IN_ENCRYPTION) ,
                                48,
                                0,
                                CSR_BT_HDP_PROFILE_UUID,
                                pMsg->noOfConnections);
        pInstance->aux_state = HDP_IDLE_CONNECTABLE;

    }
    else
    {
        pMsg = (CsrBtHdpActivateReq *)pInstance->recvMsgP;
        CsrBtHdpActivateCfmSend(pMsg->appHandle, CSR_BT_RESULT_CODE_HDP_ALREADY_ACTIVATED, CSR_BT_SUPPLIER_HDP);

    }
}

void CsrBtHdpInvalidActivateReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpActivateReq    *pMsg;

    pMsg = (CsrBtHdpActivateReq *)pInstance->recvMsgP;

    CsrBtHdpActivateCfmSend(pMsg->appHandle, CSR_BT_RESULT_CODE_HDP_INVALID_OP, CSR_BT_SUPPLIER_HDP);
}

void CsrBtHdpDataConnectVirtualChannelReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpConnectVirtualChannelReq *pMsg;
    HdpConnectionListEntry *pList,*pNewEntry;
    CsrUint16 mclId;
    CsrUint16 mdlId;
    psm_t remoteDataPsm;
    L2CA_FLOW_T *flow;

    pMsg = (CsrBtHdpConnectVirtualChannelReq *)pInstance->recvMsgP;

    mclId = (CsrUint16)(pMsg->assocChannelId >> 16);
    mdlId = CsrBtHdpUtilGetNextFreeMdl(&(pInstance->mdlIdList));
    pList = CsrBtHdpUtilConnectionEntryMatchingAssocChId(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), pMsg->assocChannelId);

    if (pList != NULL && mdlId != HDP_INVALID_MDLID && pInstance->connectAssociate == FALSE)
    {
        pNewEntry = HDP_CONNECTION_ADD_ENTRY(pInstance->connectionList);
        CsrMemSet(&pNewEntry->mclId,0, (sizeof(HdpConnectionListEntry) - (2 * sizeof(HdpConnectionListEntry *))));
        pNewEntry->active = TRUE;
        pNewEntry->mclId = mclId;
        pNewEntry->mdlId = mdlId;
        pNewEntry->mdepId = pList->mdepId;
        pNewEntry->mdepRole = pList->mdepRole;
        pNewEntry->mdepDataType = pList->mdepDataType;
        pNewEntry->assocChId = pMsg->assocChannelId;
        CsrBtBdAddrCopy(&pNewEntry->bd_addr, &pList->bd_addr);
        pNewEntry->localMtu = pMsg->maxPacketLength;

        remoteDataPsm = (CsrUint16)(pInstance->hdpInstanceId & CSR_BT_HDP_INSTANCE_MDL_MASK);
        pNewEntry->remotedataPsm = remoteDataPsm;

        flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_STREAMING, pMsg->maxPacketLength);
        pInstance->connectAssociate = TRUE;
        if(pList->mdepRole == CSR_BT_HDP_MDEP_SOURCE)
        {
            CsrBtMcapConnectMdlReqSend(mclId,mdlId,pList->mdepId,CSR_BT_HDP_CONFIG_STREAMING_CHANNEL, pInstance->psm.data, remoteDataPsm, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , pMsg->maxPacketLength, flow);
        }
        else
        { 
            CsrBtMcapConnectMdlReqSend(mclId,mdlId,pList->mdepId,CSR_BT_HDP_CONFIG_UNKNOWN, pInstance->psm.data, remoteDataPsm, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , pMsg->maxPacketLength, flow);
        }
        pInstance->prev_state = pInstance->state;
        pInstance->state = HDP_CONNECTED_STATE;
    }
    else
    {
        CsrBtHdpConnectVirtualChCfmSend(pInstance->appHandle,
                                        0,
                                        0,
                                        pMsg->maxPacketLength,
                                        CSR_BT_RESULT_CODE_HDP_CONNECT_VIRTUAL_CHANNEL_FAIL,
                                        CSR_BT_SUPPLIER_HDP);
    }

}


void CsrBtHdpConnectedAcceptConnectVirtualChannelResEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpAcceptConnectVirtualChannelRes *pMsg;
    HdpConnectionListEntry *pList;
    CsrUint16 mclId;
    CsrUint16 mdlId;
    L2CA_FLOW_T *flow;

    pMsg = (CsrBtHdpAcceptConnectVirtualChannelRes *)pInstance->recvMsgP;

    mclId = (CsrUint16)(pMsg->virtualChannelId >> 16);
    mdlId = (CsrUint16)(pMsg->virtualChannelId & 0xFF);

    pList = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), mclId, mdlId);
    if(pList != NULL)
    {
        psm_t localDataPsm = pInstance->psm.data;
        
        if ( pMsg->responseCode != CSR_BT_RESULT_CODE_HDP_SUCCESS)
        {
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, pList);
        }
        else
        {
            pList->localMtu = pMsg->maxPacketLength;
        }

        flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_STREAMING, pMsg->maxPacketLength);
        if(pMsg->virtualChannelConfig != CSR_BT_HDP_CONFIG_UNKNOWN)
        {
            CsrBtMcapCreateMdlResSend(mclId,
                                      mdlId,
                                      pMsg->responseCode,
                                      pMsg->virtualChannelConfig,
                                      localDataPsm,
                                      (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION),
                                      pMsg->maxPacketLength,
                                      flow);
        }
        else
        {
            CsrBtMcapCreateMdlResSend(mclId,
                                      mdlId,
                                      CSR_BT_RESULT_CODE_MCAP_PROTOCOL_CONFIGURATION_REJECTED,
                                      pMsg->virtualChannelConfig,
                                      localDataPsm,
                                      (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,
                                      pMsg->maxPacketLength,
                                      flow);
        }
    }
    else
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("HDP Virtual Accept Connect Res Handler: Invalid MCL MDL ID %d",
                               pInstance->eventClass,
                               pMsg->type,
                               pInstance->state);
#endif
    }
}


void CsrBtHdpConnectAssociateChannelReqHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpConnectAssociateChannelReq *msg = (CsrBtHdpConnectAssociateChannelReq *)pInstance->recvMsgP;
    pInstance->hdpInstanceId = msg->hdpInstanceId;

    if(pInstance->connectAssociate == TRUE)
    {
        CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle, msg->mdepId,
                                                msg->mdepDataType,
                                                msg->maxPacketLength, CSR_BT_RESULT_CODE_HDP_CONNECT_ASSOCIATE_CHANNEL_FAIL, CSR_BT_SUPPLIER_HDP,
                                                &(msg->deviceAddr), HDP_INVALID_INSTANCE_ID, HDP_INVALID_ASSOC_CH_ID);
    }

    if((pInstance->psm.control != HDP_PSM_INVALID || pInstance->psm.data != HDP_PSM_INVALID))
    {
        psm_t psm = HDP_GET_CTRL_PSM(msg->hdpInstanceId);
        pInstance->savedMsg = (CsrBtHdpConnectAssociateChannelReq *)CsrPmemAlloc(sizeof(CsrBtHdpConnectAssociateChannelReq));
        CsrBtMcapConnectMclReqSend(msg->deviceAddr, pInstance->psm.control, psm, CSR_BT_HDP_IFACEQUEUE, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , CSR_BT_SNIFF_TIMEOUT_MCL);
        pInstance->connectAssociate = TRUE;
        CsrMemCpy(pInstance->savedMsg,pInstance->recvMsgP,sizeof(CsrBtHdpConnectAssociateChannelReq));
        pInstance->recvMsgP = NULL;
        pInstance->state = HDP_CONNECTING_STATE;
    }
    else
    {
        CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle, msg->mdepId,
                                                msg->mdepDataType,
                                                msg->maxPacketLength, CSR_BT_RESULT_CODE_HDP_NOT_REGISTERED, CSR_BT_SUPPLIER_HDP,
                                                &(msg->deviceAddr), HDP_INVALID_INSTANCE_ID, HDP_INVALID_ASSOC_CH_ID);
    }
}

void CsrBtHdpXXXConnectAssociateChannelReqHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpConnectAssociateChannelReq *msg = (CsrBtHdpConnectAssociateChannelReq *)pInstance->recvMsgP;
    HdpConnectionListEntry *node,*pNewEntry;
    CsrUint16 remotePsm;
    HdpMdepListEntry *mdepList;
    node = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), &msg->deviceAddr);

    pInstance->hdpInstanceId = msg->hdpInstanceId;

    if(node != NULL && pInstance->connectAssociate == FALSE)
    {
        L2CA_FLOW_T *flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, msg->maxPacketLength);
        remotePsm = (CsrUint16) msg->hdpInstanceId & HDP_DATA_PSM_MASK;

        pNewEntry = HDP_CONNECTION_ADD_ENTRY(pInstance->connectionList);
        CsrMemSet(&pNewEntry->mclId,0, (sizeof(HdpConnectionListEntry) - (2 * sizeof(HdpConnectionListEntry *))));
        pNewEntry->active = TRUE;
        pNewEntry->assocChId = 0;
        CsrBtBdAddrCopy(&pNewEntry->bd_addr, &msg->deviceAddr);
        pNewEntry->mclId = node->mclId;
        pNewEntry->mdepId = msg->mdepId;
        pNewEntry->mdepDataType = msg->mdepDataType;
        pNewEntry->mdepRole = msg->mdepRole;
        pNewEntry->localMtu = msg->maxPacketLength;
        pNewEntry->mdlId = CsrBtHdpUtilGetNextFreeMdl(&(pInstance->mdlIdList));
        pNewEntry->remotedataPsm = remotePsm;
        pNewEntry->remotecontrolPsm = HDP_GET_CTRL_PSM(msg->hdpInstanceId);
        mdepList = CsrBtHdpUtilCmnListMdepMatchingMdep(pInstance->mdepList, pNewEntry->mdepId);
        if(mdepList != NULL)
        {
            pInstance->connectAssociate = TRUE;
            if(mdepList->mdep.role == CSR_BT_HDP_MDEP_SOURCE)
            {
                CsrBtMcapConnectMdlReqSend(pNewEntry->mclId, pNewEntry->mdlId, pNewEntry->mdepId,
                                            CSR_BT_HDP_CONFIG_RELIABLE_CHANNEL, pInstance->psm.data, remotePsm,
                                            (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , msg->maxPacketLength, flow);
            }
            else
            {
                CsrBtMcapConnectMdlReqSend(pNewEntry->mclId, pNewEntry->mdlId, pNewEntry->mdepId,
                                            CSR_BT_HDP_CONFIG_UNKNOWN, pInstance->psm.data, remotePsm,
                                            (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , msg->maxPacketLength, flow);
            }
        }
        else if(msg->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP)
        {
            pInstance->connectAssociate = TRUE;
            CsrBtMcapConnectMdlReqSend(pNewEntry->mclId, pNewEntry->mdlId, pNewEntry->mdepId,
                                        CSR_BT_HDP_CONFIG_RELIABLE_CHANNEL, pInstance->psm.data,remotePsm,
                                        (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , msg->maxPacketLength, flow);
        }
        else
        {
            CsrBtMcapDisconnectMclReqSend(node->bd_addr, node->remotecontrolPsm, CSR_BT_HDP_IFACEQUEUE);
            CsrPmemFree(flow);
        }
    }
    else if(pInstance->connectAssociate == TRUE)
    {
        CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle, 
                                               msg->mdepId,
                                               msg->mdepDataType,
                                               msg->maxPacketLength, 
                                               CSR_BT_RESULT_CODE_HDP_CONNECT_ASSOCIATE_CHANNEL_FAIL, 
                                               CSR_BT_SUPPLIER_HDP,
                                               &(msg->deviceAddr), 
                                               HDP_INVALID_INSTANCE_ID, 
                                               HDP_INVALID_ASSOC_CH_ID);
    }
    else
    {
       psm_t psm = HDP_GET_CTRL_PSM(msg->hdpInstanceId);
       pInstance->savedMsg = (CsrBtHdpConnectAssociateChannelReq *)CsrPmemAlloc(sizeof(CsrBtHdpConnectAssociateChannelReq));
       CsrBtMcapConnectMclReqSend(msg->deviceAddr, pInstance->psm.control, psm, CSR_BT_HDP_IFACEQUEUE, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , CSR_BT_SNIFF_TIMEOUT_MCL);
       pInstance->connectAssociate = TRUE;
       CsrMemCpy(pInstance->savedMsg,pInstance->recvMsgP,sizeof(CsrBtHdpConnectAssociateChannelReq));
       pInstance->recvMsgP = NULL;
       pInstance->state = HDP_CONNECTING_STATE;
    }
}

void CsrBtHdpAcceptConnectAssociateChannelResHandler(HdpInstanceData *pInstance)
{
    HdpConnectionListEntry *node;
    CsrBtHdpAcceptConnectAssociateChannelRes *msg = (CsrBtHdpAcceptConnectAssociateChannelRes*) pInstance->recvMsgP;

    node = CsrBtHdpUtilConnectionEntryMatchingAssocChId(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->assocChannelId);
    
    if(msg->responseCode == CSR_BT_RESULT_CODE_HDP_SUCCESS && node!= NULL)
    {
        L2CA_FLOW_T *flow;
        psm_t localPsm = pInstance->psm.data;
        
        node->localMtu = msg->mtu;
        node->localDataPsm = localPsm;

        flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, msg->mtu);
        CsrBtMcapCreateMdlResSend(node->mclId, node->mdlId, CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS, CSR_BT_HDP_CONFIG_RELIABLE_CHANNEL, 
                                  localPsm, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION), msg->mtu, flow);
    }
    else
    {
        if(node !=NULL )
        {
            CsrBtHdpConnectAssociateChannelIndSend(pInstance->appHandle,&(node->bd_addr),
                                                    CSR_BT_RESULT_CODE_HDP_CONNECT_ASSOCIATE_CHANNEL_FAIL,CSR_BT_SUPPLIER_HDP,node->mdepId,
                                                    node->mdepDataType,
                                                    node->assocChId,msg->mtu,
                                                    pInstance->hdpInstanceId);
            node = CsrBtHdpUtilConnectionEntryMatchingAssocChId(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->assocChannelId);
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, node);
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("HDP Accept Connect Associate Res Handler: Invalid Association Id %d",
                                    pInstance->eventClass,
                                    msg->type,
                                    pInstance->state);
#endif
        }
    }
}

void CsrBtHdpAssociateChDisconnectReqHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpDisconnectAssociateChannelReq *msg = (CsrBtHdpDisconnectAssociateChannelReq *)pInstance->recvMsgP;
    CsrUint32 assocChId = msg->assocChId;

    HdpConnectionListEntry *node;
    psm_t remotePsm = HDP_GET_CTRL_PSM(pInstance->hdpInstanceId);
    
    node = CsrBtHdpUtilConnectionEntryMatchingAssocChId(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),assocChId);

    if(node != NULL)
    {
        pInstance->disconnectAssociate = TRUE;
        if(!CsrBtHdpUtilConnectionEntryDeleteMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),node->assocChId))
        {
            CsrBtMcapDisconnectMclReqSend(node->bd_addr, remotePsm, CSR_BT_HDP_IFACEQUEUE);
        }
    }
    else
    {
        CsrBtHdpDisconnectAssociateChCfmSend(pInstance->appHandle, assocChId, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
    }
}

void CsrBtHdpVirtualChDisconnectReqHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpDisconnectVirtualChannelReq *msg = (CsrBtHdpDisconnectVirtualChannelReq *)pInstance->recvMsgP;
    HdpConnectionListEntry *node;
    CsrUint16 mdlId = (CsrUint16)(msg->virtualChId & CSR_BT_HDP_INSTANCE_MDL_MASK);
    CsrUint16 mclId = (CsrUint16)(msg->virtualChId >> 16);
    node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), mclId, mdlId);
    if(node != NULL)
    {
        CsrBtMcapDisconnectMdlReqSend(mclId, mdlId);
        pInstance->prev_state = pInstance->state;
        pInstance->state = HDP_CONNECTED_STATE;
    }
    else
    {
        CsrBtHdpDisconnectVirtualChCfmSend(pInstance->appHandle, msg->virtualChId, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
    }

}


void CsrBtHdpDataReqHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpDataReq *msg = (CsrBtHdpDataReq *)pInstance->recvMsgP;
    CsrUint16 mclId;
    CsrUint16 mdlId;
    HdpConnectionListEntry *node;

    if ( pInstance->state != HDP_SUSPEND_STATE )
    {
        mdlId = (CsrUint16)(msg->chId & CSR_BT_HDP_INSTANCE_MDL_MASK);
        mclId = (CsrUint16)(msg->chId >> 16);

        node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), mclId, mdlId);
        if(( node != NULL ) && ( node->active == TRUE ))
        {
            if(node->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP)
            {
                pInstance->echoTestInit = TRUE;
            }
            CsrBtMcapDataSendReqSend(mclId, mdlId, msg->dataLen, msg->data);
            pInstance->state = HDP_DATA_STATE;
        }
        else
        {
            CsrBtHdpDataCfmSend(pInstance->appHandle,
                                msg->chId,
                                CSR_BT_RESULT_CODE_HDP_INVALID_MDL,
                                CSR_BT_SUPPLIER_HDP);
            CsrPmemFree(msg->data);
        }
    }
    else
    {
        CsrBtHdpDataCfmSend(pInstance->appHandle,
                            msg->chId,
                            CSR_BT_RESULT_CODE_HDP_INVALID_OP,
                            CSR_BT_SUPPLIER_HDP);
        CsrPmemFree(msg->data);
    }
}

void CsrBtHdpXXXSuspendReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpSuspendReq    *pMsg;
    HdpConnectionListEntry *pNode;
    CsrUint16 mclId,mdlId;


    pMsg = (CsrBtHdpSuspendReq *)pInstance->recvMsgP;
    mclId = HDP_GET_MCLID(pMsg->assocChId);
    mdlId = HDP_GET_MDLID(pMsg->assocChId);

    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), mclId, mdlId);

    if (IS_VALID_PTR(pNode) && ( pNode->active == TRUE ) )
    {
        /* mark the MDL as inactive since we do not want any data requests
        on this channel */

        pNode->active = FALSE;

        CsrBtMcapDisconnectMdlReqSend(mclId,mdlId);

        pInstance->prev_state = pInstance->state;
        pInstance->state = HDP_SUSPEND_STATE;

    }
    else
    {
        CsrBtHdpSuspendCfmSend(pInstance->appHandle, pMsg->assocChId, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);
    }

}

void CsrBtHdpInvalidSuspendReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpSuspendReq    *pMsg;

    pMsg = (CsrBtHdpSuspendReq *)pInstance->recvMsgP;

    CsrBtHdpSuspendCfmSend(pMsg->appHandle, pMsg->assocChId, CSR_BT_RESULT_CODE_HDP_INVALID_OP, CSR_BT_SUPPLIER_HDP);

}
void CsrBtHdpXXXResumeReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpResumeReq    *pMsg;
    HdpConnectionListEntry *pNode;
    CsrUint16 mclId,mdlId;

    pMsg = (CsrBtHdpResumeReq *)pInstance->recvMsgP;
    /* check if there is an mcl active on the bd address on which the assoc Channel is active */
    /*If a NULL value is returned there can be two cases: Invalid AssocID or MCL does not exist
    In case one a negative cfm is returned else SDP search is initiated*/
    pNode = CsrBtHdpUtilConnectionEntryMclActiveOnAssocChId(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),pMsg->assocChId);

    if ( IS_VALID_PTR(pNode))
    {
        /* reconnect the primary mdl */
        CsrBtMcapReconnectMdlReqSend(pNode->mclId,pNode->mdlId, pInstance->psm.data,pNode->remotedataPsm);
        pInstance->state = HDP_SUSPEND_STATE;
    }
    else
    {
        mclId = HDP_GET_MCLID(pMsg->assocChId);
        mdlId = HDP_GET_MDLID(pMsg->assocChId);

        pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), mclId, mdlId);

        if (IS_VALID_PTR(pNode))
        {
            /* Perform a SDP search again*/
            pInstance->mdepDataTypeMask = 0xFF;
            pInstance->roleMask = CSR_BT_HDP_ROLE_ANY;
            pInstance->getCapab = HDP_SDP_CONNECT_INITIATED;
            CsrBtHdpSdcStartSearch(pInstance,pNode->bd_addr);
            pInstance->prev_state = pInstance->state;
            pInstance->state = HDP_SUSPEND_STATE;
        }
        else
        {
            CsrBtHdpResumeCfmSend(pMsg->appHandle, NULL,pMsg->assocChId, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);
        }
    }
}

void CsrBtHdpInvalidResumeReqEvHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpResumeReq    *pMsg;

    pMsg = (CsrBtHdpResumeReq *)pInstance->recvMsgP;

    CsrBtHdpResumeCfmSend(pMsg->appHandle,NULL, pMsg->assocChId, CSR_BT_RESULT_CODE_HDP_INVALID_OP, CSR_BT_SUPPLIER_HDP);


}

void CsrBtHdpSuspendHdpResumeRspHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpResumeRsp *pMsg = (CsrBtHdpResumeRsp *)pInstance->recvMsgP;
    CsrUint16 mclId,mdlId,psm;
    L2CA_FLOW_T *flow;
    McapResponseType mcapResponse;
    HdpConnectionListEntry *pNode;

    mclId = HDP_GET_MCLID(pMsg->chId);
    mdlId = HDP_GET_MDLID(pMsg->chId);

    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), mclId, mdlId);


    if ( IS_VALID_PTR(pNode) )
    {
        psm  = pNode->localDataPsm;

        if (pMsg->resume == TRUE )
        {
            mcapResponse = (McapResponseType)CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS;
        }
        else
        {
            mcapResponse = (McapResponseType)CSR_BT_RESULT_CODE_MCAP_PROTOCOL_RESOURCE_UNAVAILABLE;
        }

        flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS,pNode->localMtu);

        CsrBtMcapReconnectMdlResSend(mclId,mdlId, mcapResponse, psm, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , pNode->localMtu,flow);

        pInstance->state = pInstance->prev_state;
    }
    else
    {
        CsrBtMcapReconnectMdlResSend(mclId,mdlId, CSR_BT_RESULT_CODE_MCAP_PROTOCOL_RESOURCE_UNAVAILABLE, HDP_PSM_INVALID, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , 0,NULL);
    }
}

void CsrBtHdpXXXHdpUnregisterReqHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpUnregisterReq *pMsg = (CsrBtHdpUnregisterReq *)pInstance->recvMsgP;


    if ( pInstance->hServiceRecord != HDP_SDP_INVALID_SR_HANDLE )
    {
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_HDP_IFACEQUEUE,pInstance->hServiceRecord);
    }
    else
    {
        CsrBtHdpUnregisterCfmSend(pMsg->appHandle,CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
    }

    pInstance->state = HDP_IDLE_STATE;
}

void CsrBtHdpDeactivateReqHandler(HdpInstanceData *pInstance)
{
    CsrBtHdpDeactivateReq *msg = (CsrBtHdpDeactivateReq *)pInstance->recvMsgP;

    if(pInstance->sdpState == HDP_SDP_ACTIVE_STATE)
    {
        CsrBtHdpSdcSearchCancel(pInstance);
    }
    if( ( pInstance->psm.control != HDP_PSM_INVALID ) &&
        ( pInstance->isActivated == TRUE))
    {
        psm_t ctrlPsm =   pInstance->psm.control;
        CsrBtMcapDeactivateReqSend(ctrlPsm, CSR_BT_HDP_IFACEQUEUE);
    }
    else
    {
        CsrBtHdpDeactivateCfmSend(msg->appHandle,CSR_BT_RESULT_CODE_HDP_NOT_REGISTERED, CSR_BT_SUPPLIER_HDP);
    }
}
/********************************************************/
/*                                                      */
/*    MCAP Event handlers                               */
/*                                                      */
/********************************************************/

void CsrBtHdpMcapDeactivateCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDeactivateCfm *msg = (CsrBtMcapDeactivateCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *connectionNode,*nextconnectionNode;
    psm_t ctrlPsm =   pInstance->psm.control;

    if(ctrlPsm == msg->psm)
    {
        for (connectionNode = HDP_CONNECTION_GET_FIRST(pInstance->connectionList); connectionNode; connectionNode = nextconnectionNode)
        {
            nextconnectionNode = (HdpConnectionListEntry *)connectionNode->next;
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, connectionNode);
        }
        if ( msg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS )
        {
            msg->resultCode = CSR_BT_RESULT_CODE_HDP_SUCCESS;
            msg->resultSupplier = CSR_BT_SUPPLIER_HDP;
        }
        CsrBtHdpDeactivateCfmSend(pInstance->appHandle,msg->resultCode,msg->resultSupplier);
        pInstance->state = HDP_IDLE_STATE;
    }

}


void CsrBtHdpIdleMcapActivateCfmEvHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapActivateCfm *pMsg;


    pMsg = (CsrBtMcapActivateCfm *)pInstance->recvMsgP;

    if (( pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS) &&
        ( pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP) &&
        ( pMsg->psm == pInstance->psm.control) &&
        ( pInstance->aux_state == HDP_IDLE_CONNECTABLE ) )
    {
        CsrBtHdpActivateCfmSend(pInstance->appHandle, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
        pInstance->isActivated = TRUE;
    }
    else
    {
        CsrBtHdpActivateCfmSend(pInstance->appHandle, pMsg->resultCode, pMsg->resultSupplier);
    }
}

void CsrBtHdpMcapConnectMclIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapConnectMclInd *msg = (CsrBtMcapConnectMclInd*)pInstance->recvMsgP;
    HdpConnectionListEntry *node;
    CsrUint16 mclId;

    /* search if the bd-addr exists then this is a case of reconnect */
    node = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                        &msg->bd_addr);
   if (node == NULL)
   {
       node = HDP_CONNECTION_ADD_ENTRY(pInstance->connectionList);
       CsrMemSet(&node->mclId,0, (sizeof(HdpConnectionListEntry) - (2 * sizeof(HdpConnectionListEntry *))));
       node->active = TRUE;
       node->mclId = msg->mclId;
       CsrBtBdAddrCopy(&node->bd_addr, &msg->bd_addr);
       node->mdlId = HDP_INVALID_MDLID;  /* Filling a dummy value. Correct value will be filled when we receive first mdl on this mcl */
       node->assocChId = HDP_INVALID_ASSOC_CH_ID;
       node->remotecontrolPsm = msg->remotePsm;

        if(pInstance->state == HDP_IDLE_STATE)
        {
            pInstance->state = HDP_CONNECTING_STATE;
        }
    }
    else
    {
        mclId = node->mclId;
        /* run through all node matching bd and mcl-id and change to new mclId
        though the mclId can change, mdlId does not change */
        do
        {
            node->mclId = msg->mclId;
            node->assocChId = HDP_FORM_ASSOC_CH_ID(msg->mclId , HDP_GET_MDLID(node->assocChId));
            node = CsrBtHdpUtilConnectionEntryGetMatchingBdMcl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                                &msg->bd_addr,
                                                                mclId);
        }while (node != NULL) ;

        pInstance->prev_state = pInstance->state;
    }

    /* Need to perform the SDP search here to get the data ie the PSM values */
    pInstance->mdepDataTypeMask = 0xFF;
    pInstance->roleMask = CSR_BT_HDP_ROLE_ANY;
    pInstance->getCapab = HDP_SDP_CONNECT_INITIATED;
    CsrBtHdpSdcStartSearch(pInstance,msg->bd_addr); 
}


void CsrBtHdpMcapConnectMclCfmHandler(HdpInstanceData *pInstance)
{
    HdpConnectionListEntry *node;
    CsrUint16 mdlId = 1;
    
    CsrBtMcapConnectMclCfm *msg = (CsrBtMcapConnectMclCfm *)pInstance->recvMsgP;

    if(pInstance->connectAssociate == TRUE)
    {
        L2CA_FLOW_T *flow;

        CsrBtHdpConnectAssociateChannelReq *savedPrim = (CsrBtHdpConnectAssociateChannelReq *)pInstance->savedMsg;
        
        if (msg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS &&
            msg->resultSupplier == CSR_BT_SUPPLIER_MCAP)
        {
            psm_t remotePsm = (CsrUint16)(pInstance->hdpInstanceId & CSR_BT_HDP_INSTANCE_MDL_MASK);

            flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, savedPrim->maxPacketLength);

            mdlId = CsrBtHdpUtilGetNextFreeMdl(&(pInstance->mdlIdList));

            node = HDP_CONNECTION_ADD_ENTRY(pInstance->connectionList);
            CsrMemSet(&node->mclId,0, (sizeof(HdpConnectionListEntry) - (2 * sizeof(HdpConnectionListEntry *))));
            node->active = TRUE;
            node->mclId = msg->mclId;
            node->mdepId = savedPrim->mdepId;
            node->mdepDataType = savedPrim->mdepDataType;
            node->mdepRole = savedPrim->mdepRole;
            node->mdlId = mdlId;
            node->assocChId = HDP_INVALID_ASSOC_CH_ID;
            node->remotecontrolPsm = (CsrUint16)(pInstance->hdpInstanceId >> 16);
            node->remotedataPsm  = remotePsm;
            node->localMtu = savedPrim->maxPacketLength;

            CsrBtBdAddrCopy(&node->bd_addr, &msg->bd_addr);

            if(node->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP)
            {
                CsrBtMcapConnectMdlReqSend(msg->mclId, mdlId, savedPrim->mdepId, CSR_BT_HDP_CONFIG_RELIABLE_CHANNEL,pInstance->psm.data, remotePsm, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,savedPrim->maxPacketLength,flow);
            }
            else
            {
                if(savedPrim->mdepRole == CSR_BT_HDP_MDEP_SOURCE)
                {
                    CsrBtMcapConnectMdlReqSend(msg->mclId, mdlId, savedPrim->mdepId, CSR_BT_HDP_CONFIG_RELIABLE_CHANNEL,pInstance->psm.data, remotePsm, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,savedPrim->maxPacketLength,flow);
                }
                else
                {
                    CsrBtMcapConnectMdlReqSend(msg->mclId, mdlId, savedPrim->mdepId, CSR_BT_HDP_CONFIG_UNKNOWN,pInstance->psm.data, remotePsm, (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,savedPrim->maxPacketLength,flow);
                }
            }

        }
        else
        {
            CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle,savedPrim->mdepId ,
                                                    savedPrim->mdepDataType,0,
                                                    msg->resultCode, msg->resultSupplier,&(savedPrim->deviceAddr),
                                                    pInstance->hdpInstanceId,0);
        }
        CsrPmemFree(pInstance->savedMsg);
    }
    else
    {
        CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle,0 ,0, 0,CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR,CSR_BT_SUPPLIER_HDP,NULL,pInstance->hdpInstanceId,0);
    }
}

void CsrBtHdpMcapCreateMdlIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapCreateMdlInd *msg = (CsrBtMcapCreateMdlInd *)pInstance->recvMsgP;
    HdpConnectionListEntry *node,*mdlNode,*pNewEntry;
    CsrUint16 remoteDataPsm;
    CsrUint16 localDataPsm;
    HdpMdepListEntry  *pMdepNode;
    CsrUint32 assocId;
    
    assocId = msg->mclId;
    assocId = (assocId << 16 | msg->mdlId);
    remoteDataPsm = (CsrUint16)(pInstance->hdpInstanceId & CSR_BT_HDP_INSTANCE_MDL_MASK);
    localDataPsm = pInstance->psm.data;

    node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->mclId,HDP_INVALID_MDLID); /* This has to be checked for multiple MDL */
    mdlNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->mclId,msg->mdlId);
    if(node != NULL)
    {
        node->mdepId = msg->mdepId;
        node->mdlId  = msg->mdlId;
        node->assocChId = assocId;
        node->active = TRUE;
        node->remotedataPsm = remoteDataPsm;
        node->localDataPsm = localDataPsm;

        pMdepNode = CsrBtHdpUtilCmnListMdepMatchingMdepFree(pInstance->mdepList,msg->mdepId);

        if ( pMdepNode != NULL )
        {
            pMdepNode->busy = TRUE;
            node->mdepDataType = pMdepNode->mdep.dataType;


            if( (pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SOURCE && msg->configuration != CSR_BT_HDP_CONFIG_UNKNOWN) ||
                (pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SINK && msg->configuration == CSR_BT_HDP_CONFIG_UNKNOWN) ||
                msg->configuration > CSR_BT_HDP_CONFIG_STREAMING_CHANNEL)
            {
                L2CA_FLOW_T *flow;
                flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, CSR_BT_HDP_MAX_PACKET_LEN);
                CsrBtMcapCreateMdlResSend(msg->mclId, 
                                          msg->mdlId,
                                          CSR_BT_RESULT_CODE_HDP_INVALID_CONFIG, 
                                          CSR_BT_HDP_CONFIG_UNKNOWN,
                                          localDataPsm,
                                          (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,
                                          0,
                                          flow);

                if(msg->configuration > CSR_BT_HDP_CONFIG_STREAMING_CHANNEL)
                {
                    CsrBtMcapDisconnectMclReqSend(node->bd_addr, node->remotecontrolPsm,CSR_BT_HDP_IFACEQUEUE);
                }
            }
            else if(mdlNode!= NULL)
            {
                L2CA_FLOW_T *flow;
                mdlNode->mdlId = msg->mdlId;
                mdlNode->mdepId = msg->mdepId;
                flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, mdlNode->localMtu);
                CsrBtMcapCreateMdlResSend(msg->mclId, 
                                          msg->mdlId,
                                          CSR_BT_RESULT_CODE_HDP_SUCCESS, 
                                          CSR_BT_HDP_CONFIG_RELIABLE_CHANNEL,
                                          localDataPsm,
                                          (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,
                                          mdlNode->localMtu,
                                          flow);
            }
            else
            {
                CsrBtHdpAcceptConnectAssociateChannelIndSend(pInstance->appHandle,
                                                             node->mdepId,
                                                             node->assocChId,
                                                             &node->bd_addr);
            }

        }
        else if(msg->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP)
        {
            node->mdepDataType = CSR_BT_HDP_DEVICE_TYPE_ECHO_TEST;
            CsrBtHdpAcceptConnectAssociateChannelIndSend(pInstance->appHandle,
                                                         node->mdepId,
                                                         node->assocChId,
                                                         &node->bd_addr);
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("HDP Create MDL Ind: MDEP Busy",
                                    pInstance->eventClass,
                                    msg->type,
                                    pInstance->state);
#endif
            /*@note busy mdep should not occur for this state handler */
        }
    }
    else if(pInstance->connectAssociate == TRUE)
    {
        L2CA_FLOW_T *flow;
        flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, CSR_BT_HDP_MAX_PACKET_LEN);
        CsrBtMcapCreateMdlResSend(msg->mclId, 
                                  msg->mdlId,
                                  CSR_BT_RESULT_CODE_HDP_INVALID_OP, 
                                  CSR_BT_HDP_CONFIG_UNKNOWN, 
                                  localDataPsm,
                                  (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,
                                  0,
                                  flow);
    }
    else if (!CsrBtHdpUtilCmnListIsMdepBusy(msg->mdepId,pInstance->mdepList))
    {
        node = CsrBtHdpUtilConnectionEntryMatchingMcl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), msg->mclId);
        if(node != NULL)
        {
            pNewEntry = HDP_CONNECTION_ADD_ENTRY(pInstance->connectionList);
            CsrMemSet(&pNewEntry->mclId,0, (sizeof(HdpConnectionListEntry) - (2 * sizeof(HdpConnectionListEntry *))));
            pNewEntry->active = TRUE;
            pNewEntry->mclId = msg->mclId;
            CsrBtBdAddrCopy(&pNewEntry->bd_addr, &node->bd_addr);
            pNewEntry->mdlId = msg->mdlId;
            pNewEntry->assocChId = assocId;
            pNewEntry->remotecontrolPsm = node->remotecontrolPsm;
            pNewEntry->remotedataPsm = node->remotedataPsm;
            pNewEntry->localDataPsm = node->localDataPsm;
            pNewEntry->mdepId = msg->mdepId;
            CsrBtHdpAcceptConnectAssociateChannelIndSend(pInstance->appHandle,
                                                        pNewEntry->mdepId,
                                                        pNewEntry->assocChId,
                                                        &pNewEntry->bd_addr);
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("HDP Create MDL Ind: Invalid MCL",
                                    pInstance->eventClass,
                                    msg->type,
                                    pInstance->state);
#endif
        }
    }
    else if ( msg->configuration >= CSR_BT_HDP_CONFIG_STREAMING_CHANNEL)
    {
        /* it is not right to handle streaming through this event handler
        invalid state */
        L2CA_FLOW_T *flow;
        flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, CSR_BT_HDP_MAX_PACKET_LEN);
        CsrBtMcapCreateMdlResSend(msg->mclId, 
                                  msg->mdlId,
                                  CSR_BT_RESULT_CODE_HDP_INVALID_CONFIG,
                                  CSR_BT_HDP_CONFIG_UNKNOWN,
                                  localDataPsm,
                                  (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION),
                                  0,
                                  flow);

        if(msg->configuration > CSR_BT_HDP_CONFIG_STREAMING_CHANNEL)
        {/* At this point the variable 'node' is NULL */
            if (mdlNode != NULL)
            {
                CsrBtMcapDisconnectMclReqSend(mdlNode->bd_addr, mdlNode->remotecontrolPsm, CSR_BT_HDP_IFACEQUEUE);
            }
        }

    }
    else
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("HDP Create MDL Ind: MDEP Busy",
                               pInstance->eventClass,
                               msg->type,
                               pInstance->state);
#endif
    }
}

void CsrBtHdpMcapConnectMdlIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapConnectMdlInd *msg = (CsrBtMcapConnectMdlInd*)pInstance->recvMsgP;
    HdpConnectionListEntry *node;
    CsrUint32 assocId = msg->mclId;
    assocId = assocId << 16 | msg->mdlId;
    
    pInstance->connectAssociate = FALSE;

    node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->mclId,msg->mdlId);
    if(node != NULL)
    {
        node->localMtu = msg->localMtu;
        pInstance->state = HDP_CONNECTED_STATE;
        pInstance->hdpInstanceId |= msg->remotePsm;

        node->remotecontrolPsm = (CsrUint16)(pInstance->hdpInstanceId >> 16);
        node->remotedataPsm = msg->remotePsm;
        node->remoteMtu = msg->remoteMtu;
        node->localDataPsm = msg->localPsm;

        if (msg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS ||
            msg->resultSupplier ==  CSR_BT_SUPPLIER_MCAP )
        {
            msg->resultCode = CSR_BT_RESULT_CODE_HDP_SUCCESS;
            msg->resultSupplier = CSR_BT_SUPPLIER_HDP;
        }

        CsrBtHdpConnectAssociateChannelIndSend(pInstance->appHandle,
                                               &(node->bd_addr),
                                               msg->resultCode,
                                               msg->resultSupplier,
                                               node->mdepId,
                                               node->mdepDataType,
                                               node->assocChId,
                                               msg->remoteMtu,
                                               pInstance->hdpInstanceId);

    }
    else
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("HDP Connect Mdl Indication: Invalid MCL Id",
                                pInstance->eventClass,
                                msg->type,
                                pInstance->state);
#endif
    }
}



void CsrBtHdpMcapConnectMdlCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapConnectMdlCfm *pMsg = (CsrBtMcapConnectMdlCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode;
    CsrUint32 assocId = 0;
    pInstance->connectAssociate = FALSE;

    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),pMsg->mclId,pMsg->mdlId);

    if( pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS &&
        pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP)
    {
        assocId = pMsg->mclId;
        assocId = assocId << 16 | pMsg->mdlId;
        pNode->assocChId = assocId;
        pNode->active = TRUE;
        pNode->localDataPsm = pMsg->localPsm;
        pNode->localMtu = pMsg->localMtu;
        pNode->remoteMtu = pMsg->remoteMtu;
        pNode->remotedataPsm = pMsg->remotePsm;
        pInstance->state = HDP_CONNECTED_STATE;

        CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle,
                                               pNode->mdepId,
                                               pNode->mdepDataType,
                                               pMsg->remoteMtu,
                                               CSR_BT_RESULT_CODE_HDP_SUCCESS,
                                               CSR_BT_SUPPLIER_HDP,
                                               &(pNode->bd_addr),
                                               pInstance->hdpInstanceId,
                                               pNode->assocChId);
    }
    else
    {
        CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle,
                                               pNode->mdepId,
                                               pNode->mdepDataType,
                                               pMsg->remoteMtu,
                                               pMsg->resultCode,
                                               pMsg->resultSupplier,
                                               &(pNode->bd_addr),
                                               pInstance->hdpInstanceId,
                                               pNode->assocChId);

        CsrBtMcapDisconnectMclReqSend(pNode->bd_addr, pMsg->remotePsm, CSR_BT_HDP_IFACEQUEUE);


    }
}





void CsrBtHdpConnectedMcapConnectMdlCfmEvHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapConnectMdlCfm *pMsg;
    HdpConnectionListEntry *pList;
    CsrUint32 virtualChId;
    CsrBtResultCode   resultCode;
    CsrBtSupplier     resultSupplier;

    pMsg = (CsrBtMcapConnectMdlCfm *)pInstance->recvMsgP;

    pList = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), pMsg->mclId, pMsg->mdlId);
    pList->active = TRUE;
    pInstance->connectAssociate = FALSE;
    virtualChId = ( pMsg->mclId << 16 ) | pMsg->mdlId;
    pList->localDataPsm = pMsg->localPsm;
    pList->localMtu = pMsg->localMtu;
    pList->remoteMtu = pMsg->remoteMtu;
    pList->remotedataPsm = pMsg->remotePsm;

    if( pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS ||
        pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP )
    {
        resultCode = CSR_BT_RESULT_CODE_HDP_SUCCESS;
        resultSupplier = CSR_BT_SUPPLIER_HDP;
    }
    else
    {
        resultCode = pMsg->resultCode;
        resultSupplier = pMsg->resultSupplier;
    }

    if (pList->assocChId == 0 )
    {
        pList->assocChId = virtualChId;
        if (pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS &&
            pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP)
        {
            CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle,
                                                   pList->mdepId ,
                                                   pList->mdepDataType,
                                                   pMsg->remoteMtu,
                                                   CSR_BT_RESULT_CODE_HDP_SUCCESS,
                                                   CSR_BT_SUPPLIER_HDP,
                                                   &(pList->bd_addr),
                                                   pInstance->hdpInstanceId,
                                                   pList->assocChId);
        }
        else
        {
            CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle,
                                                   pList->mdepId ,
                                                   pList->mdepDataType,
                                                   pMsg->remoteMtu,
                                                   resultCode,
                                                   resultSupplier,
                                                   &(pList->bd_addr),
                                                   pInstance->hdpInstanceId,
                                                   pList->assocChId);
        }
    }
    else
    {
        CsrBtHdpConnectVirtualChCfmSend(pInstance->appHandle,
                                        pList->assocChId,
                                        virtualChId,
                                        pMsg->localMtu,
                                        resultCode,
                                        resultSupplier);
    }

    if(pMsg->resultCode != CSR_BT_RESULT_CODE_MCAP_SUCCESS ||
        pMsg->resultSupplier != CSR_BT_SUPPLIER_MCAP)
    {
        HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList,pList);
    }
}

void CsrBtHdpXXXMcapCreateMdlIndEvHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapCreateMdlInd *pMsg;
    HdpConnectionListEntry *pList,*pNewEntry,*pNode,*mdlNode;
    HdpMdepListEntry *pMdepNode;
    CsrUint32 virtualChId;
    CsrUint16 localDataPsm;

    pMsg = (CsrBtMcapCreateMdlInd *)pInstance->recvMsgP;
    pNode = CsrBtHdpUtilConnectionEntryMatchingMcl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                    pMsg->mclId);
    localDataPsm = pInstance->psm.data;

    pMdepNode = CsrBtHdpUtilCmnListMdepMatchingMdep(pInstance->mdepList, pMsg->mdepId);
    if(pMdepNode != NULL)
    {
        if((pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SOURCE && pMsg->configuration != CSR_BT_HDP_CONFIG_UNKNOWN) || 
            (pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SINK && pMsg->configuration == CSR_BT_HDP_CONFIG_UNKNOWN) || 
            pMsg->configuration > CSR_BT_HDP_CONFIG_STREAMING_CHANNEL)
        {
            L2CA_FLOW_T *flow;
            flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, CSR_BT_HDP_MAX_PACKET_LEN);
            CsrBtMcapCreateMdlResSend(pMsg->mclId, 
                                      pMsg->mdlId,
                                      CSR_BT_RESULT_CODE_HDP_INVALID_CONFIG,
                                      CSR_BT_HDP_CONFIG_UNKNOWN,
                                      localDataPsm,
                                      (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) ,
                                      0,
                                      flow);
            if(pMsg->configuration > CSR_BT_HDP_CONFIG_STREAMING_CHANNEL)
            {
                CsrBtMcapDisconnectMclReqSend(pNode->bd_addr, pNode->remotecontrolPsm,CSR_BT_HDP_IFACEQUEUE);
            }
        }

        mdlNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), pMsg->mclId, pMsg->mdlId);

        if ( (pMsg->configuration == CSR_BT_HDP_CONFIG_STREAMING_CHANNEL && pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SINK) || 
            (pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SOURCE && pMsg->configuration == CSR_BT_HDP_CONFIG_UNKNOWN && (mdlNode!= NULL || CsrBtHdpUtilCmnListIsMdepBusy(pMsg->mdepId,pInstance->mdepList))))
        {
            if(mdlNode != NULL)
            {
                L2CA_FLOW_T *flow;
                mdlNode->mdlId = pMsg->mdlId;
                mdlNode->mdepId = pMsg->mdepId;
                flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, mdlNode->localMtu);
                CsrBtMcapCreateMdlResSend(pMsg->mclId, 
                                          pMsg->mdlId,
                                          CSR_BT_RESULT_CODE_HDP_SUCCESS, 
                                          CSR_BT_HDP_CONFIG_STREAMING_CHANNEL,
                                          localDataPsm,
                                          (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION),
                                          mdlNode->localMtu,
                                          flow);

                pInstance->state = HDP_CONNECTED_STATE;
                return;
            }
            pList = CsrBtHdpUtilConnectionEntryMatchingMclMdep(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), pMsg->mclId, pMsg->mdepId);
            virtualChId = pMsg->mclId;
            virtualChId = virtualChId << 16 | pMsg->mdlId;

           pNewEntry = HDP_CONNECTION_ADD_ENTRY(pInstance->connectionList);
           
           CsrMemSet(&pNewEntry->mclId,0, (sizeof(HdpConnectionListEntry) - (2 * sizeof(HdpConnectionListEntry *))));
           pNewEntry->active = TRUE;
           pNewEntry->mclId = pMsg->mclId;
           pNewEntry->mdlId = pMsg->mdlId;
           pNewEntry->assocChId = pList->assocChId;
           pNewEntry->mdepId = pMsg->mdepId;
           CsrBtBdAddrCopy(&pNewEntry->bd_addr, &(pList->bd_addr));

           CsrBtHdpAcceptConnectVirtualChIndSend(pInstance->appHandle,
                                                pList->assocChId,
                                                virtualChId,
                                                pMsg->configuration);


            pInstance->prev_state = pInstance->state;
            pInstance->state = HDP_CONNECTED_STATE;
        }
        else if ( (pMsg->configuration == CSR_BT_HDP_CONFIG_RELIABLE_CHANNEL && ( pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SINK)) || 
            (pMdepNode->mdep.role == CSR_BT_HDP_MDEP_SOURCE && pMsg->configuration == CSR_BT_HDP_CONFIG_UNKNOWN) )
        {
            if (!CsrBtHdpUtilCmnListIsMdepBusy(pMsg->mdepId,pInstance->mdepList))
            {
                CsrBtHdpMcapCreateMdlIndHandler(pInstance);
            }
            else if(mdlNode != NULL)
            {
                /* Since the Mdep is busy check if the ind has come for a mdl
                for an exisiting MDL in an MCL or for a MDL on a different MCL
                in the former case, send SUCCESS in the later send MDEP_BUSY */
                L2CA_FLOW_T *flow;
                mdlNode->mdlId = pMsg->mdlId;
                mdlNode->mdepId = pMsg->mdepId;
                flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, mdlNode->localMtu);
                CsrBtMcapCreateMdlResSend(pMsg->mclId, 
                                          pMsg->mdlId,
                                          CSR_BT_RESULT_CODE_HDP_SUCCESS, 
                                          CSR_BT_HDP_CONFIG_STREAMING_CHANNEL,
                                          localDataPsm,
                                          (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION),
                                          mdlNode->localMtu,
                                          flow);
                pInstance->state = HDP_CONNECTED_STATE;
                return;
            }
            else
            {
                L2CA_FLOW_T *flow;
                HdpConnectionListEntry *delNode;

                delNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), pMsg->mclId, HDP_INVALID_MDLID);
                if(delNode != NULL)
                {
                    HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, delNode);
                }
                flow = CsrBtHdpFillFlowParameters(L2CA_FLOW_MODE_ENHANCED_RETRANS, 0);
                CsrBtMcapCreateMdlResSend(pMsg->mclId, 
                                          pMsg->mdlId, 
                                          CSR_BT_RESULT_CODE_HDP_MDL_BUSY, 
                                          pMsg->configuration, 
                                          localDataPsm,
                                          (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION), 
                                          0, 
                                          flow);
            }
        }

    }
    else if(pMsg->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP)
    {
        pNewEntry = HDP_CONNECTION_ADD_ENTRY(pInstance->connectionList);
        CsrMemSet(&pNewEntry->mclId,0, (sizeof(HdpConnectionListEntry) - (2 * sizeof(HdpConnectionListEntry *))));
        pNewEntry->active = TRUE;
        pNewEntry->mclId = pMsg->mclId;
        CsrBtBdAddrCopy(&pNewEntry->bd_addr, &pNode->bd_addr);
        pNewEntry->mdlId = HDP_INVALID_MDLID;
        pNewEntry->assocChId = HDP_INVALID_ASSOC_CH_ID;
        pNewEntry->remotecontrolPsm = pNode->remotecontrolPsm;
        CsrBtHdpAcceptConnectAssociateChannelIndSend(pInstance->appHandle,
                                                     pMsg->mdepId,
                                                     pNode->assocChId,
                                                     &pNode->bd_addr);
        pInstance->prev_state = pInstance->state;
        pInstance->state = HDP_CONNECTED_STATE;
    }
}


void CsrBtHdpXXXMcapConnectMdlIndEvHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapConnectMdlInd *pMsg = (CsrBtMcapConnectMdlInd *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode;
    CsrUint16 mdlId;
    CsrUint32 chId;
    CsrBtResultCode   resultCode;
    CsrBtSupplier     resultSupplier;

    pInstance->connectAssociate = FALSE;

    if( pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS ||
        pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP )
    {
        resultCode = CSR_BT_RESULT_CODE_HDP_SUCCESS;
        resultSupplier = CSR_BT_SUPPLIER_HDP;
    }
    else
    {
        resultCode = pMsg->resultCode;
        resultSupplier = pMsg->resultSupplier;
    }

    pInstance->mdlDeleted = FALSE;
    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                        pMsg->mclId,
                                                        pMsg->mdlId);
    /* D-9075 Added flow parameters here*/

    if ( IS_VALID_PTR(pNode))
    {
        if(pNode->active == FALSE && !(pNode->remoteMtu == pMsg->remoteMtu && pNode->localDataPsm == pMsg->localPsm))
        {
            CsrBtMcapDisconnectMdlReqSend(pMsg->mclId, pMsg->mdlId);
            return;
        }

        pNode->active = TRUE;
        pNode->localMtu = pMsg->localMtu;
        pNode->remotedataPsm = pMsg->remotePsm;
        pNode->localDataPsm = pMsg->localPsm;
        pNode->remoteMtu = pMsg->remoteMtu;

        mdlId = HDP_GET_MDLID(pNode->assocChId);
        if ( mdlId == pMsg->mdlId )
        {
            CsrBtHdpConnectAssociateChannelIndSend(pInstance->appHandle,
                                                    &pNode->bd_addr,
                                                    resultCode,
                                                    resultSupplier,
                                                    pNode->mdepId,
                                                    pNode->mdepDataType,
                                                    pNode->assocChId,
                                                    pMsg->remoteMtu,
                                                    pInstance->hdpInstanceId);
        }
        else
        {
            /* send virtual channel request */
            chId = HDP_FORM_ASSOC_CH_ID(pMsg->mclId,pMsg->mdlId);

            CsrBtHdpConnectVirtualChIndSend(pInstance->appHandle,
                                            pNode->assocChId,
                                            chId,
                                            pMsg->localMtu,
                                            resultCode,
                                            resultSupplier);
        }
    }
    else
    {
        /* throw exception */
    }

    if ( pMsg->resultCode != CSR_BT_RESULT_CODE_MCAP_SUCCESS  ||
        pMsg->resultSupplier != CSR_BT_SUPPLIER_MCAP)
    {
        HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, pNode);
    }

}


void CsrBtHdpConnectingMcapMclDiscIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDisconnectMclInd *msg = (CsrBtMcapDisconnectMclInd *)pInstance->recvMsgP;
    HdpConnectionListEntry *node;
   
    if(pInstance->sdpState == HDP_SDP_ACTIVE_STATE)
    {
        CsrBtHdpSdcSearchCancel(pInstance);
    }
   
    node = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),&msg->bd_addr);
   
    if(node != NULL)
    {
        /* Disconnect on MCL while waiting for connect of MDL - therefore failed */
        if(pInstance->connectAssociate)
        {
            CsrBtHdpConnectAssociateChannelCfmSend(pInstance->appHandle,
                                                   node->mdepId,
                                                   node->mdepDataType,
                                                   node->remoteMtu,
                                                   CSR_BT_RESULT_CODE_HDP_CONNECT_ASSOCIATE_CHANNEL_FAIL,
                                                   CSR_BT_SUPPLIER_HDP,
                                                   &(node->bd_addr),
                                                   pInstance->hdpInstanceId,
                                                   node->assocChId);
            pInstance->connectAssociate = FALSE;
        }
        CsrBtHdpUtilMdepIdUnCheckBusy(node->mdepId, pInstance->mdepList);
        HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, node);
    }
  
    if(HDP_CONNECTION_GET_FIRST(pInstance->connectionList) == NULL)
    {
        pInstance->state = HDP_IDLE_STATE;
    }
}


void CsrBtHdpXXXMcapMclDisconnectIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDisconnectMclInd *msg = (CsrBtMcapDisconnectMclInd *)pInstance->recvMsgP;
    HdpConnectionListEntry *node;
   
    if(pInstance->sdpState == HDP_SDP_ACTIVE_STATE)
    {
        CsrBtHdpSdcSearchCancel(pInstance);
    }
   
    node = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),&msg->bd_addr);
   
    if(node != NULL)
    {
        if(pInstance->mdlDeleted && node->mdepId != CSR_BT_HDP_ECHO_TEST_MDEP && pInstance->disconnectAssociate)
        {
            CsrBtHdpDisconnectAssociateChCfmSend(pInstance->appHandle, node->assocChId, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
            CsrBtHdpUtilMdepIdUnCheckBusy(node->mdepId, pInstance->mdepList);
            pInstance->disconnectAssociate = FALSE;
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, node);
        }
    }
  
    if(HDP_CONNECTION_GET_FIRST(pInstance->connectionList) == NULL)
    {
        pInstance->state = HDP_IDLE_STATE;
    }
}

void CsrBtHdpXXXMcapMclDisconnectCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDisconnectMclCfm *msg = (CsrBtMcapDisconnectMclCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *node;

    if(pInstance->sdpState == HDP_SDP_ACTIVE_STATE)
    {
        CsrBtHdpSdcSearchCancel(pInstance);
    }
    node = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),&msg->bd_addr);
    if(node != NULL)
    {
        if(pInstance->mdlDeleted == TRUE && node->mdepId != CSR_BT_HDP_ECHO_TEST_MDEP)
        {
            if(msg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS &&
                msg->resultSupplier == CSR_BT_SUPPLIER_MCAP)
            {
                CsrBtHdpDisconnectAssociateChCfmSend(pInstance->appHandle, node->assocChId, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
                CsrBtHdpUtilMdepIdUnCheckBusy(node->mdepId, pInstance->mdepList);
            }
            else
            {
                if(pInstance->disconnectAssociate == TRUE)
                {
                    CsrBtHdpDisconnectAssociateChCfmSend(pInstance->appHandle, node->assocChId, msg->resultCode, msg->resultSupplier);
                }
            }
            pInstance->disconnectAssociate = FALSE;
        }
        HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, node);

        if((node = HDP_CONNECTION_GET_FIRST(pInstance->connectionList)) == NULL)
        {
            pInstance->state = HDP_IDLE_STATE;
        }
    }
    else
    {
        /* Already deleted */
    }
}

void CsrBtHdpXXXMcapMdlDisconnectIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDisconnectMdlInd *msg = (CsrBtMcapDisconnectMdlInd *)pInstance->recvMsgP;
    HdpConnectionListEntry *node;
    CsrUint16 mdlId;

    node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->mclId, msg->mdlId);
    if( node != NULL)
    {
        mdlId = (CsrUint16)(node->assocChId & CSR_BT_HDP_INSTANCE_MDL_MASK);

        if(msg->mdlId == mdlId)
        {
            CsrBtHdpUtilMdepIdUnCheckBusy(node->mdepId, pInstance->mdepList);
            if(pInstance->disconnectAssociate == FALSE || node->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP)
            {
                if (msg->resultSupplier == CSR_BT_SUPPLIER_L2CAP_DISCONNECT &&
                    msg->resultCode== L2CA_DISCONNECT_NORMAL)
                {
                    CsrBtHdpDisconnectAssociateChIndSend(pInstance->appHandle,
                                                        node->assocChId,
                                                        CSR_BT_RESULT_CODE_HDP_SUCCESS,
                                                        CSR_BT_SUPPLIER_HDP);
                }
                else
                {
                    CsrBtHdpDisconnectAssociateChIndSend(pInstance->appHandle,
                                                        node->assocChId,
                                                        msg->resultCode,
                                                        msg->resultSupplier);
                }
            }
            else
            {
                if(!CsrBtHdpUtilConnectionEntryFindOtherConnection(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), node->assocChId))
                {
                    CsrBtHdpDisconnectAssociateChCfmSend(pInstance->appHandle,
                                                        node->assocChId,
                                                        msg->resultCode,
                                                        msg->resultSupplier);
                    pInstance->disconnectAssociate = FALSE;
                }
            }

        }
        else
        {
            CsrUint32 virtualChId = msg->mclId;
            virtualChId = ((virtualChId << 16) | msg->mdlId);
            if (msg->resultSupplier == CSR_BT_SUPPLIER_L2CAP_DISCONNECT &&
                msg->resultCode== L2CA_DISCONNECT_NORMAL)
            {
                CsrBtHdpDisconnectVirtualChIndSend(pInstance->appHandle,
                                                    virtualChId,
                                                    CSR_BT_RESULT_CODE_HDP_SUCCESS,
                                                    CSR_BT_SUPPLIER_HDP);
            }
            else
            {
                CsrBtHdpDisconnectVirtualChIndSend(pInstance->appHandle,
                                                    virtualChId,
                                                    msg->resultCode,
                                                    msg->resultSupplier);
            }
        }
        /* if the L2CA data channel is not disconnected before the the MDL
        we will respond in positive to MDL_DEL_IND and have to handle
        MDL-DISCONNECT-IND as MCAP will than delete the MDL */
        if (node->active == FALSE )
        {
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, node);
        }
        else
        {
            node->active = FALSE;
        }
    }
    else
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("HDP Disconnect MDL Ind: Invalid MDL Id",
                                pInstance->eventClass,
                                msg->type,
                                pInstance->state);
#endif
    }

}

void CsrBtHdpXXXMcapMdlDisconnectCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDisconnectMdlCfm *msg = (CsrBtMcapDisconnectMdlCfm *)pInstance->recvMsgP;
    CsrUint32 virtualChId;
    HdpConnectionListEntry *node;
    virtualChId = msg->mclId;
    virtualChId = virtualChId << 16 | msg->mdlId;
    node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->mclId, msg->mdlId);
    node->active = FALSE;

    if(pInstance->disconnectAssociate == FALSE )
    {
        if(node->assocChId == virtualChId)
        {
            CsrBtHdpUtilMdepIdUnCheckBusy(node->mdepId, pInstance->mdepList);
        }
        if (msg->resultSupplier == CSR_BT_SUPPLIER_L2CAP_DISCONNECT &&
            msg->resultCode == L2CA_DISCONNECT_NORMAL)
        {
            CsrBtHdpDisconnectAssociateChIndSend(pInstance->appHandle,
                                                node->assocChId,
                                                CSR_BT_RESULT_CODE_HDP_SUCCESS,
                                                CSR_BT_SUPPLIER_HDP);
        }
        else
        {
            CsrBtHdpDisconnectAssociateChIndSend(pInstance->appHandle,
                                                node->assocChId,
                                                msg->resultCode,
                                                msg->resultSupplier);
        }
    }
    else
    {
        if(node->assocChId == virtualChId)
        {
            CsrBtHdpUtilMdepIdUnCheckBusy(node->mdepId, pInstance->mdepList);
            CsrBtHdpDisconnectAssociateChCfmSend(pInstance->appHandle,node->assocChId, msg->resultCode, msg->resultSupplier);
            pInstance->disconnectAssociate = FALSE;
        }
        else
        {
            CsrBtHdpDisconnectVirtualChCfmSend(pInstance->appHandle,virtualChId, msg->resultCode, msg->resultSupplier);
        }
    }
}


void CsrBtHdpXXXMcapMdlDeleteCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDeleteMdlCfm *msg = (CsrBtMcapDeleteMdlCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *node;
    CsrUint16 mdlCount;
    CsrUint32 assocChId;
    psm_t remotePsm = HDP_GET_CTRL_PSM(pInstance->hdpInstanceId);

    node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->mclId,msg->mdlId);
    if(msg->mcapResponse == CSR_BT_RESULT_CODE_MCAP_SUCCESS)
    {
        if(node != NULL)
        {
            assocChId = node->assocChId;
            mdlCount = CsrBtHdpUtilFindNumberofEntries(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),msg->mclId);
            if(mdlCount == 1 && pInstance->mdlDeleted == TRUE)
            {
                CsrBtMcapDisconnectMclReqSend(node->bd_addr, remotePsm, CSR_BT_HDP_IFACEQUEUE);
                return;
            }
            else if(mdlCount == 1 && pInstance->mdlDeleted == FALSE)
            {
                (void)CsrBtHdpUtilConnectionEntryDeleteMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),assocChId);
                pInstance->mdlDeleted = TRUE;
                return;
            }
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList,node);
            (void)CsrBtHdpUtilConnectionEntryDeleteMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),assocChId);
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("HDP Delete Mdl Cfm: Invalid MDL Id",
                                    pInstance->eventClass,
                                    msg->type,
                                    pInstance->state);
#endif
        }
    }
    else
    {
        CsrBtMcapDisconnectMclReqSend(node->bd_addr, remotePsm, CSR_BT_HDP_IFACEQUEUE);
    }
}

void CsrBtHdpXXXMcapMdlDeleteIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDeleteMdlInd *pMsg = (CsrBtMcapDeleteMdlInd *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode;


    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), pMsg->mclId, pMsg->mdlId);
    if(pNode!= NULL)
    {
        CsrBtMcapDeleteMdlResSend(pMsg->mclId, pMsg->mdlId, CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS);
        /* if the MCAP-DELETE-MDL-IND is received before the
        the L2CAP data channel is disconnected than do not delete the
        it from the database. Delete it on receiving the MCAP-DISCONNECT_IND */
        if ( pNode->active == FALSE)
        {
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, pNode);
        }
        else
        {
            pNode->active = FALSE;
        }
    }
    else if ( pMsg->mdlId == HDP_WILDCARD_ID ) /* wild card deletion of MDL */
    {
        CsrBtMcapDeleteMdlResSend(pMsg->mclId, pMsg->mdlId, CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS);

        pNode = CsrBtHdpUtilConnectionEntryMatchingMcl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                        pMsg->mclId);
        do
        {
            if ( pNode->active == FALSE)
            {
                HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, pNode);
            }

            pNode = CsrBtHdpUtilConnectionEntryMatchingMcl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                pMsg->mclId);
        }while ( IS_VALID_PTR(pNode));
    }
    else /* the mdl id does not match anything from db => invalid MDL */
    {
        CsrBtMcapDeleteMdlResSend(pMsg->mclId, pMsg->mdlId, CSR_BT_RESULT_CODE_MCAP_PROTOCOL_INVALID_MDL);
    }
}

void CsrBtHdpSuspendMcapMdlDisconnectCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDisconnectMdlCfm *pMsg = (CsrBtMcapDisconnectMdlCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode;
    CsrUint16 remotePsm;
    CsrUint32  chId;


    if ((pMsg->resultCode == L2CA_RESULT_SUCCESS &&
        pMsg->resultSupplier == CSR_BT_SUPPLIER_L2CAP_DISCONNECT) ||
        (pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_PROTOCOL_SUCCESS &&
        pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP_PROTOCOL))
    {
        /* from ch Id get assocChId */
        pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),pMsg->mclId,pMsg->mdlId);

        if ( pNode != NULL )
        {
            /* find if there are any other MDLs which are active with
            the same AssocChID. */
            chId = pNode->assocChId;
            pNode = CsrBtHdpUtilConnectionEntryGetActiveMdlInAssocChId(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),chId);

            if (pNode == NULL )
            {
                /* since there are no active MDLs, search if there are MDLs
                active on other MDEPs */
                pNode = CsrBtHdpUtilConnectionEntryGetActiveMdepInMcl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList), pMsg->mclId);

                if ( pNode != NULL )
                {
                    /* active MDLs so do not diconnect the MCL. */
                    pInstance->state = pInstance->prev_state;
                    CsrBtHdpSuspendCfmSend(pInstance->appHandle, chId, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
                }
                else
                {
                    /*since there are no active MDLs on other MDEPs, disconnect the
                    MCL */
                    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),pMsg->mclId,pMsg->mdlId);
                    remotePsm = HDP_GET_CTRL_PSM(pInstance->hdpInstanceId);
                    CsrBtMcapDisconnectMclReqSend(pNode->bd_addr, remotePsm, CSR_BT_HDP_IFACEQUEUE);
                }
            }
            else
            {
                pNode->active = FALSE;

                CsrBtMcapDisconnectMdlReqSend(pNode->mclId,pNode->mdlId);
            }
        }
    }
    else
    {
        pInstance->state = pInstance->prev_state;
        CsrBtHdpSuspendCfmSend(pInstance->appHandle, HDP_WILDCARD_ID, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);
    }

}
void CsrBtHdpSuspendMcapMclDisconnectCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapDisconnectMclCfm *pMsg = (CsrBtMcapDisconnectMclCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *node;

    node = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),&pMsg->bd_addr);

    if (IS_VALID_PTR(node))
    {
        CsrBtHdpSuspendCfmSend(pInstance->appHandle, node->assocChId, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
    }
    else
    {
        CsrBtHdpSuspendCfmSend(pInstance->appHandle, HDP_INVALID_ASSOC_CH_ID, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);
    }
    pInstance->state = pInstance->prev_state;
}

void CsrBtHdpSuspendMcapMclConnectCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapConnectMclCfm *pMsg = (CsrBtMcapConnectMclCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode;
    CsrUint16 mclId;

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS &&
        pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP)
    {
        /* find the node which matches the bd-addr and remotePsm */
        pNode = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                            &pMsg->bd_addr);
        mclId = pNode->mclId;

        /* run through all node matching bd and mcl-id and change to new mclId
        though the mclId can change,mdlId does not change */
        do
        {
            pNode->mclId = pMsg->mclId;
            pNode->assocChId = HDP_FORM_ASSOC_CH_ID(pMsg->mclId , HDP_GET_MDLID(pNode->assocChId));
            pNode = CsrBtHdpUtilConnectionEntryGetMatchingBdMcl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                                &pMsg->bd_addr,
                                                                mclId);
        }while (pNode != NULL);

        pNode = CsrBtHdpUtilConnectionEntryMatchingBdAddr(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                            &pMsg->bd_addr);
        /* reconnect the primary mdl */
        CsrBtMcapReconnectMdlReqSend(pNode->mclId,pNode->mdlId, pInstance->psm.data,pNode->remotedataPsm);
    }
    else
    {
        pInstance->state = pInstance->prev_state;

        CsrBtHdpResumeCfmSend(pInstance->appHandle,NULL,HDP_WILDCARD_ID, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);
    }
}

void CsrBtHdpSuspendMcapReconnectMdlCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapReconnectMdlCfm *pMsg = (CsrBtMcapReconnectMdlCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode, *pMoreMdl;
    CsrUint16 mclId;
    CsrUint16 mdlId;
    CsrUint16 remotePsm;
    CsrUint32 virtualChId,chId;

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS &&
        pMsg->resultSupplier == CSR_BT_SUPPLIER_MCAP)
    {
        pNode =  CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                            pMsg->mclId,
                                                            pMsg->mdlId);
        if ( IS_VALID_PTR(pNode))
        {
            chId  = pNode->assocChId;
            pNode->active = TRUE;

            /* reconnect the other mdl's in the related to the AssocChId*/
            pMoreMdl =  CsrBtHdpUtilConnectionEntryGetInActiveMdlInAssocChId(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                                           pNode->assocChId);
            if ( IS_VALID_PTR(pMoreMdl))
            {
                mclId = pMoreMdl->mclId;
                mdlId = pMoreMdl->mdlId;
                remotePsm = pMoreMdl->remotedataPsm;

                CsrBtMcapReconnectMdlReqSend(mclId, mdlId, pInstance->psm.data, remotePsm);
            }
            else
            {
                pInstance->state = pInstance->prev_state;
                CsrBtHdpResumeCfmSend(pInstance->appHandle, &pNode->bd_addr, chId, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
            }
        }
        else
        {
            CsrBtDeviceAddr     zeroAddr;
            CsrMemSet(&zeroAddr, 0, sizeof(CsrBtDeviceAddr));

            pInstance->state = pInstance->prev_state;
            CsrBtHdpResumeCfmSend(pInstance->appHandle, 
                                &zeroAddr, 
                                0, /* invalid channel ID */
                                CSR_BT_RESULT_CODE_HDP_INVALID_MDL, 
                                CSR_BT_SUPPLIER_HDP);
        }
    }
    else
    {
        chId = HDP_FORM_ASSOC_CH_ID(pMsg->mclId , pMsg->mdlId);

        pInstance->state = pInstance->prev_state;
        CsrBtHdpResumeCfmSend(pInstance->appHandle,NULL, chId, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);

        pNode =  CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
            pMsg->mclId,
            pMsg->mdlId);
        if ( pNode != NULL )
        {
            mdlId = HDP_GET_MDLID(pNode->assocChId);

            if ( mdlId == pMsg->mdlId )
            {
                CsrBtHdpConnectAssociateChannelIndSend(pInstance->appHandle,
                                                        &pNode->bd_addr,
                                                        CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR,
                                                        CSR_BT_SUPPLIER_HDP,
                                                        pNode->mdepId,
                                                        pNode->mdepDataType,
                                                        pNode->assocChId,
                                                        0,
                                                        pInstance->hdpInstanceId);
            }
            else
            {

                virtualChId = HDP_FORM_ASSOC_CH_ID(pMsg->mclId,pMsg->mdlId);

                CsrBtHdpConnectVirtualChIndSend(pInstance->appHandle,
                                                pNode->assocChId,
                                                virtualChId,
                                                0,
                                                CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR,
                                                CSR_BT_SUPPLIER_HDP);
            }
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, pNode);
        }
    }
}

void CsrBtHdpXXXMcapReconnectMdlIndHandler(HdpInstanceData *pInstance)
{
    CsrBtMcapReconnectMdlInd *pMsg = (CsrBtMcapReconnectMdlInd *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode;

    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdlBdPsm(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                            pMsg->mclId,
                                                            pMsg->mdlId,
                                                            &pMsg->bd_addr,
                                                            pMsg->remotePsm);

    /* D-9075 Check for the reconnect parameters*/

    if( ( pNode != NULL ) &&
        ( !CsrBtHdpUtilCmnListIsMdepBusy(pNode->mdepId,pInstance->mdepList) || ( pNode->active == FALSE) ) )
    {
        pNode->assocChId = HDP_FORM_ASSOC_CH_ID(pNode->mclId, pMsg->mdlId);

        CsrBtHdpResumeIndSend(pInstance->appHandle, pNode->assocChId,pNode->bd_addr);
        if ( pInstance->state != HDP_SUSPEND_STATE )
        {
            pInstance->prev_state = pInstance->state;
            pInstance->state = HDP_SUSPEND_STATE;
        }
    }
    else
    {
        CsrBtMcapReconnectMdlResSend(pMsg->mclId,
                                    pMsg->mdlId,
                                    CSR_BT_RESULT_CODE_MCAP_PROTOCOL_UNSPECIFIED_ERROR,
                                    pMsg->localPsm,
                                    (SECL4_OUT_SSP  | SECL_OUT_AUTHENTICATION | SECL_OUT_ENCRYPTION) , 0,NULL);
    }

}

void CsrBtHdpMcapDataIndHandler(HdpInstanceData * pInstance)
{
    CsrBtMcapDataSendInd *msg = (CsrBtMcapDataSendInd*)pInstance->recvMsgP;
    CsrUint8 *dataPtr1;
    HdpConnectionListEntry *node;

    dataPtr1 = (CsrUint8 *)CsrPmemAlloc(msg->length);
    CsrMemCpy(dataPtr1, msg->payload, msg->length);

    node = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                    msg->mclId,
                                                    msg->mdlId);
    if(node != NULL)
    {
        if(node->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP && pInstance->echoTestInit == FALSE)
        {
            CsrUint8 *dataPtr2;
            dataPtr2 = (CsrUint8 *)CsrPmemAlloc(msg->length);
            CsrMemCpy(dataPtr2, msg->payload, msg->length);
            CsrBtMcapDataSendReqSend(msg->mclId, msg->mdlId, msg->length, dataPtr2);
        }
        CsrBtHdpDataIndSend(pInstance->appHandle,dataPtr1,msg->length,node->assocChId);

       if(node->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP && pInstance->echoTestInit == TRUE)
       {
          pInstance->echoTestInit = FALSE;
          CsrBtMcapDeleteMdlReqSend(msg->mclId, msg->mdlId);
       }
   }
   else
   {
       CsrPmemFree(dataPtr1);
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("Data Ind Received on invalid MDL",
                                pInstance->eventClass,
                                msg->type,
                                pInstance->state);
#endif
    }
    pInstance->state = HDP_DATA_STATE;
    CsrPmemFree(msg->payload);
}

void CsrBtHdpMcapDataCfmHandler(HdpInstanceData * pInstance)
{
    CsrBtMcapDataSendCfm *msg = (CsrBtMcapDataSendCfm *)pInstance->recvMsgP;
    HdpConnectionListEntry *pNode;


    pNode = CsrBtHdpUtilConnectionEntryMatchingMclMdl(HDP_CONNECTION_GET_FIRST(pInstance->connectionList),
                                                        msg->mclId,
                                                        msg->mdlId);

    if (msg->resultCode == CSR_BT_RESULT_CODE_MCAP_SUCCESS &&
        msg->resultSupplier == CSR_BT_SUPPLIER_MCAP &&
        pNode != NULL)
    {
        CsrBtHdpDataCfmSend(pInstance->appHandle,
                            pNode->assocChId,
                            CSR_BT_RESULT_CODE_HDP_SUCCESS,
                            CSR_BT_SUPPLIER_HDP);
    }
    else if (pNode != NULL)
    {
        CsrBtHdpDataCfmSend(pInstance->appHandle,pNode->assocChId,
                            msg->resultCode,
                            msg->resultSupplier);
    }
    else
    {
        CsrBtHdpDataCfmSend(pInstance->appHandle,
                            HDP_INVALID_ASSOC_CH_ID,
                            msg->resultCode,
                            msg->resultSupplier);
    }
}

/********************************************************/
/*                                                      */
/*    CM Event handlers                               */
/*                                                      */
/********************************************************/


void CsrBtHdpIdleCmSdsRegisterCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtCmSdsRegisterCfm *pMsg;


    pMsg = (CsrBtCmSdsRegisterCfm *)pInstance->recvMsgP;

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && pMsg->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        pInstance->hServiceRecord = pMsg->serviceRecHandle;

        CsrBtHdpRegisterCfmSend(pInstance->appHandle, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);
    }
    else
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("CmSdsRegisterCfm:Check service record",
                                pInstance->eventClass,
                                pMsg->type,
                                pInstance->state);
#endif

    }
}

void CsrBtHdpIdleCmSdsUnregisterCfmHandler(HdpInstanceData *pInstance)
{
    CsrBtCmSdsUnregisterCfm *pMsg = (CsrBtCmSdsUnregisterCfm *)pInstance->recvMsgP;
    HdpMdepListEntry *mdepNode,*nextMdepNode;
    HdpConnectionListEntry *connectionNode,*nextconnectionNode;

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && pMsg->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        pInstance->hServiceRecord = HDP_SDP_INVALID_SR_HANDLE;

        CsrBtCml2caUnRegisterReqSend(CSR_BT_HDP_IFACEQUEUE, pInstance->psm.control);
        CsrBtCml2caUnRegisterReqSend(CSR_BT_HDP_IFACEQUEUE, pInstance->psm.data);

        CsrBtHdpUnregisterCfmSend(pInstance->appHandle, CSR_BT_RESULT_CODE_HDP_SUCCESS, CSR_BT_SUPPLIER_HDP);

        CsrPmemFree(pInstance->serviceName);
        CsrPmemFree(pInstance->serviceDescription);
        CsrPmemFree(pInstance->providerName);
        CsrPmemFree(pInstance->sdpSearchData);

        for (mdepNode =HDP_MDEP_GET_FIRST(pInstance->mdepList); mdepNode; mdepNode = nextMdepNode)
        {
            nextMdepNode = (HdpMdepListEntry *)mdepNode->next;
            HDP_MDEP_DEL_ENTRY(pInstance->mdepList, mdepNode);
        }

        /*@note - check use cases under which this has to be freed under unregister */

        for (connectionNode = HDP_CONNECTION_GET_FIRST(pInstance->connectionList); connectionNode; connectionNode = nextconnectionNode)
        {
            nextconnectionNode = (HdpConnectionListEntry *)connectionNode->next;
            HDP_CONNECTION_DEL_ENTRY(pInstance->connectionList, connectionNode);
        }

        CsrBtHdpAssignDefaultValues(pInstance);

    }
    else
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("CmSdsRegisterCfm:Check service record",
                                pInstance->eventClass,
                                pMsg->type,
                                pInstance->state);
#endif

    }
}




void CsrBtHdpIdleL2CaRegisterCfmEvHandler(HdpInstanceData *pInstance)
{
    CsrBtCmL2caRegisterCfm  *pMsg = (CsrBtCmL2caRegisterCfm *)pInstance->recvMsgP;

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && pMsg->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if ( pInstance->psm.control == HDP_PSM_INVALID )
        {
            pInstance->psm.control = pMsg->localPsm;

            CsrBtCml2caRegisterReqExtSend(CSR_BT_HDP_IFACEQUEUE,
                                        CSR_BT_ASSIGN_DYNAMIC_PSM,
                                        (L2CA_MODE_MASK_ENHANCED_RETRANS | L2CA_MODE_MASK_STREAMING),
                                        0); /*flags*/
        }
        else if (pInstance->psm.data == HDP_PSM_INVALID )
        {
            pInstance->psm.data = pMsg->localPsm;

            pInstance->aux_state = HDP_IDLE_W4_MDEP;

            CsrBtHdpRegisterIndSend(pInstance->appHandle, pInstance->currentMdepId);
        }
        else
        {
            /*  this is spurious cfm event. */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("HDP-CsrBtCmL2caRegisterCfm:spurious signal",
                                    pInstance->eventClass,
                                    pMsg->type,
                                    pInstance->state);
#endif
        }
    }
    else
    {
        /* if the creation of the second PSM fails, unregister that PSM */
        if (pInstance->psm.control != HDP_PSM_INVALID )
        {
            CsrBtCml2caUnRegisterReqSend(CSR_BT_HDP_IFACEQUEUE, pInstance->psm.control);
        }
        CsrBtHdpRegisterCfmSend(pInstance->appHandle, CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_HDP);
    }

}


#endif

