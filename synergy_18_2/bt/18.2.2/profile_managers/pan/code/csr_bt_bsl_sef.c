/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
        
            All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_bsl_main.h"
#include "csr_bt_bsl_prim.h"
#include "csr_bt_bsl_util.h"
#include "csr_bt_bsl_sef.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_tasks.h"
#include "csr_bt_bnep_prim.h"
#include "csr_bt_result.h"

static void csrBtBslSendBslActivateCfm(BslInstanceData_t *instData,
                                CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtBslActivateCfm *prim;

    prim = (CsrBtBslActivateCfm *) CsrPmemAlloc(sizeof(CsrBtBslActivateCfm));
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->type = CSR_BT_BSL_ACTIVATE_CFM;
    CsrBtPanMessagePut(instData->appHandle, prim);
}

/******************************************************************************

    Send responses for security mode changes

******************************************************************************/
void csrBtBslSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtBslSecurityInCfm *prim;
    prim = (CsrBtBslSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtBslSecurityInCfm));
    prim->type = CSR_BT_BSL_SECURITY_IN_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtPanMessagePut(appHandle, prim);
}

void csrBtBslSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtBslSecurityOutCfm *prim;
    prim = (CsrBtBslSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtBslSecurityOutCfm));
    prim->type = CSR_BT_BSL_SECURITY_OUT_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtPanMessagePut(appHandle, prim);
}

/*--------------------    Register state functions    -----------------------*/
void CsrBtBslRegisterStateBslRegisterReqHandler(BslInstanceData_t *instData)
{
    CsrBtBslRegisterReq *prim;

    prim = (CsrBtBslRegisterReq *) instData->recvMsgP;
    instData->appHandle = prim->connPhandle;
    instData->dataHandle = prim->dataPhandle;
    CsrBtCmReadLocalBdAddrReqSend(CSR_BT_BSL_IFACEQUEUE);
    StateChange(instData, RegisterS);
}


/*--------------------    Idle state functions    -----------------------*/
void CsrBtBslIdleStateBslConnectReqHandler(BslInstanceData_t *instData)
{
    CsrBtBslConnectReq        *prim;
    BNEP_CONNECT_REQ_FLAGS    connFlags;
    ETHER_ADDR                ethAddr;
    CsrUint8 connectionIndex;
    BslConnection_t *connection = NULL;

    prim = (CsrBtBslConnectReq *) instData->recvMsgP;
    connectionIndex = CsrBtBslFindBdAddrIndex(instData, &prim->bdAddr);

    if (connectionIndex != BSL_INVALID_CONNECTION)
    {
        connection = &instData->connectionList[connectionIndex];
    }

    if (connection && connection->connectCancelled)
    {
        Go2Idle(instData);
        connection->connectCancelled = FALSE;
        connection->connectState = IdleConnectionS;
        CsrBtBslSendBslConnectInd(instData->appHandle,
                             0,
                             &prim->bdAddr, prim->remoteRole,
                             prim->localRole,
                             CSR_BT_RESULT_CODE_BSL_CONNECT_CANCELLED,
                             CSR_BT_SUPPLIER_BSL);
        CsrBtBslClearConnection(connection);
    }
    else
    {
        if (!connection)
        {
            connectionIndex = CsrBtBslFindEmptyIndex(instData);
            if (connectionIndex == BSL_INVALID_CONNECTION)
            {
                CsrBtBslSendBslConnectInd(instData->appHandle,
                                     0,
                                     &prim->bdAddr, prim->remoteRole,
                                     prim->localRole,
                                     CSR_BT_RESULT_CODE_BSL_CONNECT_LIMIT,
                                     CSR_BT_SUPPLIER_BSL);
                return;
            }
            connection = &instData->connectionList[connectionIndex];
            connection->bdAddr = prim->bdAddr;
        }
        if (instData->activated)
        {
            /* cancel accept before handling the connect req */
            CsrMessageQueuePush( &(instData->saveQueue),CSR_BT_BSL_PRIM, instData->recvMsgP);
            instData->recvMsgP = NULL;
            CsrBtCmBnepCancelConnectAcceptReqSend();
            connection->connectState = ConnectCancelAcceptS;
        }
        else
        {

            /* set the right connect flags for supported roles etc */
            connFlags = CsrBtCmBnepSetConnectReqFlagsFromRoles(prim->localRole, prim->remoteRole, FALSE, FALSE);
            /* store the role requested for the outgoing connection in order to figure out 
               whether to request a role switch when the connection is established */
            connection->outgoingRole   = prim->localRole;
            instData->currentConn2Addr = prim->bdAddr;
            ethAddr = CsrBtBdAddrToEtherAddr(&(instData->currentConn2Addr));

            /* make a copy of the connect signal just in case it is
             * rejected with 'try later' */
            instData->connFlagsCopy = connFlags;
            instData->numberOfRetries = 0;

            CsrBtCmBnepConnectReqSend(connFlags,
                                 ethAddr,
                                 CSR_BT_BSL_PROFILE_DEFAULT_MTU_SIZE,
                                 instData->secOutgoing);
            connection->connectState = ConnectingS;
        }
        StateChange(instData, BusyS);
    }
}

/*--------------------    Busy state functions    -----------------------*/
void CsrBtBslBusyStateCmBnepCancelConnectAcceptCfmHandler(BslInstanceData_t *instData)
{
    /* cancel connect accept is simply a primitive indicating that the
       cancel is completed - no need to check the result here. Restore
       the save queue as a connect may be stored while cancelling */
    instData->activated = FALSE;
    Go2Idle(instData);
}

void CsrBtBslBusyStateCmSdcSearchIndHandler(BslInstanceData_t *instData)
{
    CsrBtCmSdcSearchInd *prim;

    prim = (CsrBtCmSdcSearchInd *) instData->recvMsgP;
    if (prim->serviceHandleListCount)
    {
        /* always select the first one */
        instData->sdcSearchHandle = *prim->serviceHandleList;
        instData->sdcAttrSearchIndex = SEARCH_CHANNEL_OPEN_INDEX;
    }
    else
    {
        CsrBtBslSendBslServiceSearchCfm(instData,
            CSR_BT_RESULT_CODE_BSL_UNDEFINED_SERVICE,
            CSR_BT_SUPPLIER_BSL);
        Go2Idle(instData);
    }
    CsrPmemFree(prim->serviceHandleList);
}

void CsrBtBslBusyStateCmSdcSearchCfmHandler(BslInstanceData_t *instData)
{
    /* uuid16_t not found on peer device, so terminate the search */
    if (instData->sdcAttrSearchIndex == OPENING_SEARCH_CHANNEL_INDEX)
    {
        CsrBtBslSendBslServiceSearchCfm(instData,
            CSR_BT_RESULT_CODE_BSL_UNDEFINED_SERVICE,
            CSR_BT_SUPPLIER_BSL);
        Go2Idle(instData);
    }
    else    /* now start search for attributes */
    {
        instData->sdcSearchRec = CsrPmemZalloc(sizeof(CsrBtBslSearchRecord));
        /* set index to first attribute in the list */
        instData->sdcAttrSearchIndex = PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER_INDEX;
        CsrBtBslSendCmSdcAttributeReq(instData);
    }
}

void CsrBtBslBusyStateCmSdcAttributeCfmHandler(BslInstanceData_t *instData)
{
    CsrBtCmSdcAttributeCfm *prim;

    prim = (CsrBtCmSdcAttributeCfm *) instData->recvMsgP;
    /* handle response    and put in record for later send to app */
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        PutSdsVariables(instData, prim->attributeListSize, prim->attributeList);
    }

    /* if more attributes to search: continue search */
    instData->sdcAttrSearchIndex++;
    if (instData->sdcAttrSearchIndex < LAST_SEARCH_ATTRIBUTE_INDEX)
    {
        CsrBtBslSendCmSdcAttributeReq(instData);
    }
    else    /* if last attribute sent: send response to app */
    {
        CsrBtBslSendBslServiceSearchResultInd(instData);
        /* the collected search results are copied in the send result
         * function, so free it now */
        if (instData->sdcSearchRec != NULL)
        {
            CsrPmemFree(instData->sdcSearchRec->serviceName);
            CsrPmemFree(instData->sdcSearchRec->serviceDescription);
            CsrPmemFree(instData->sdcSearchRec->ipv4Subnet);
            CsrPmemFree(instData->sdcSearchRec->ipv6Subnet);
            CsrPmemFree(instData->sdcSearchRec);
            instData->sdcSearchRec = NULL;
        }
        CsrBtCmSdcCloseReqSend(CSR_BT_BSL_IFACEQUEUE);
    }
    CsrPmemFree(prim->attributeList);
}

void CsrBtBslBusyStateCmSdcCloseIndHandler(BslInstanceData_t *instData)
{
    CsrBtResultCode resultCode;

    /* Search is finished so just close search towards app */
    if (instData->sdcAttrSearchIndex == LAST_SEARCH_ATTRIBUTE_INDEX)
    {
        resultCode = CSR_BT_RESULT_CODE_BSL_SUCCESS;
    }
    /* uuid16_t not found on peer device, so terminate the search */
    else if (instData->sdcAttrSearchIndex == OPENING_SEARCH_CHANNEL_INDEX)
    {
        resultCode = CSR_BT_RESULT_CODE_BSL_UNDEFINED_SERVICE;
    }

    else    /* If not finished it must be abnormal disc */
    {
        if (instData->sdcSearchRec != NULL)
        {
            CsrPmemFree(instData->sdcSearchRec->serviceName);
            CsrPmemFree(instData->sdcSearchRec->serviceDescription);
            CsrPmemFree(instData->sdcSearchRec->ipv4Subnet);
            CsrPmemFree(instData->sdcSearchRec->ipv6Subnet);
            CsrPmemFree(instData->sdcSearchRec);
            instData->sdcSearchRec = NULL;
        }
        resultCode = CSR_BT_RESULT_CODE_BSL_ABNORMAL_LINK_DISCONNECT;
    }
    CsrBtBslSendBslServiceSearchCfm(instData, resultCode,
        CSR_BT_SUPPLIER_BSL);
    Go2Idle(instData);
}


/*--------------------    Register state functions    -----------------------*/
void CsrBtBslRegisterStateCmReadLocalBdAddrCfmHandler(BslInstanceData_t *instData)
{
    CsrBtCmReadLocalBdAddrCfm *prim;

    /* Registration in bnep */
    prim = (CsrBtCmReadLocalBdAddrCfm *) instData->recvMsgP;

    /* Tell the SD that it must look for the CSR_BT_PAN_PANU_PROFILE_UUID,
       CSR_BT_PAN_NAP_PROFILE_UUID and the CSR_BT_PAN_GN_PROFILE_UUID service ,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ              */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_PAN_PANU_PROFILE_UUID);
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_PAN_NAP_PROFILE_UUID);
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_PAN_GN_PROFILE_UUID);

    CsrBtCmBnepRegisterReqSend(FALSE,
                          TRUE,
                          FALSE,
                          CSR_BT_BSL_IFACEQUEUE,
                          prim->deviceAddr);

    CsrBtScRegisterReqSend(0x1115,
                      CSR_BT_PAN_BNEP_PSM,
                      TRUE, /* Also applies for outgoing connections */
                      SEC_PROTOCOL_L2CAP,
                      (dm_security_level_t) (instData->secIncoming|instData->secOutgoing));
    Go2Idle(instData);
}


/*---------------------------------------------------------------------------
  Result of registration in sdp server. If failed then try again.
  ---------------------------------------------------------------------------*/
void CsrBtBslSdsRegisterStateCmSdsRegisterCfmHandler(BslInstanceData_t *instData)
{
    CsrBtCmSdsRegisterCfm *prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* store the returned sds rec handle */
        if (instData->currentRoleRegister == CSR_BT_BSL_NAP_ROLE)
        {
            instData->napSdsRecHandle = prim->serviceRecHandle;
            instData->localRole = instData->localRole | CSR_BT_BSL_NAP_ROLE;
        }
        else if (instData->currentRoleRegister == CSR_BT_BSL_GN_ROLE)
        {
            instData->gnSdsRecHandle = prim->serviceRecHandle;
            instData->localRole = instData->localRole | CSR_BT_BSL_GN_ROLE;
        }
        else /*(instData->currentRoleRegister == CSR_BT_BSL_PANU_ROLE)*/
        {
            instData->panuSdsRecHandle = prim->serviceRecHandle;
            instData->localRole = instData->localRole | CSR_BT_BSL_PANU_ROLE;
        }

        /* Now see if more register signals should be sent or we are done */
        if (SdsRoleRegister(instData) == TRUE)
        {
            /* a register has been sent */
            StateChange(instData, SdsRegisterS);
        }
        else if (SdsRoleDeregister(instData) == TRUE)
        {
            /* a deregister has been sent */
            StateChange(instData, SdsRegisterS);
            instData->numberOfRetries = 0;
        }
        else
        {
            /* nothing sent so we are done */
            /* enable page scan */
            ETHER_ADDR                etherAddr;
            BNEP_CONNECT_REQ_FLAGS    connFlags;

            etherAddr.w[0] = ETHER_UNKNOWN;
            connFlags = CsrBtCmBnepSetConnectReqFlagsFromRoles(instData->localRole,
                                                    instData->remoteRole, TRUE,
                                                    instData->singleUser);

            CsrBtCmBnepConnectAcceptReqSend(connFlags,
                                       etherAddr,
                                       CSR_BT_BSL_PROFILE_DEFAULT_MTU_SIZE,
                                       BSL_CLASS_OF_DEVICE,
                                       instData->secIncoming);
            /* remain in sdsregister state */
        }
    }
    else
    {
        if (instData->numberOfRetries < MAX_NUMBER_OF_SDS_RETRIES)
        {
            SdsRoleRegister(instData);
            instData->numberOfRetries++;
        }
        else
        {
            csrBtBslSendBslActivateCfm(instData, prim->resultCode, prim->resultSupplier);
            Go2Idle(instData);
        }
    }
}

/*---------------------------------------------------------------------------
  Result of accept connect req. Send result to app.
  ---------------------------------------------------------------------------*/
void CsrBtBslSdsRegisterStateCmBnepConnectAcceptCfmHandler(BslInstanceData_t *instData)
{
    CsrBtCmBnepConnectAcceptCfm *prim;

    prim = (CsrBtCmBnepConnectAcceptCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        csrBtBslSendBslActivateCfm(instData, CSR_BT_RESULT_CODE_BSL_SUCCESS, CSR_BT_SUPPLIER_BSL);
    }
    else
    {
        instData->activated= FALSE;
        csrBtBslSendBslActivateCfm(instData, prim->resultCode, prim->resultSupplier);
    }
    Go2Idle(instData);
}

/*---------------------------------------------------------------------------
  Result of deregistration in sdp server. If failed then try again.
  ---------------------------------------------------------------------------*/
void CsrBtBslSdsRegisterStateCmSdsUnregisterCfmHandler(BslInstanceData_t *instData)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm *) instData->recvMsgP;
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* store the returned sds rec handle */
        if (instData->currentRoleRegister == CSR_BT_BSL_NAP_ROLE)
        {
            instData->napSdsRecHandle = SDS_REC_HANDLE_UNUSED;
            instData->localRole = instData->localRole & ~CSR_BT_BSL_NAP_ROLE;
        }
        else if (instData->currentRoleRegister == CSR_BT_BSL_GN_ROLE)
        {
            instData->gnSdsRecHandle = SDS_REC_HANDLE_UNUSED;
            instData->localRole = instData->localRole & ~CSR_BT_BSL_GN_ROLE;
        }
        else /*(instData->currentRoleRegister == CSR_BT_BSL_PANU_ROLE)*/
        {
            instData->panuSdsRecHandle = SDS_REC_HANDLE_UNUSED;
            instData->localRole = instData->localRole & ~CSR_BT_BSL_PANU_ROLE;
        }

        /* no deregister sent so we are through */
        if (SdsRoleDeregister(instData) == FALSE)
        {
            /* disable page scan if no roles are active */
            if (instData->localRole == CSR_BT_BSL_NO_ROLE)
            {
                CsrBtCmBnepCancelConnectAcceptReqSend();
                /* remain in sdsregister state */
            }
            else
            {
                /* enable page scan */
                ETHER_ADDR                etherAddr;
                BNEP_CONNECT_REQ_FLAGS    connFlags;

                etherAddr.w[0] = ETHER_UNKNOWN;
                connFlags = CsrBtCmBnepSetConnectReqFlagsFromRoles(instData->localRole,
                                                        instData->remoteRole,
                                                        TRUE,
                                                        instData->singleUser);
                CsrBtCmBnepConnectAcceptReqSend(connFlags,
                                           etherAddr,
                                           CSR_BT_BSL_PROFILE_DEFAULT_MTU_SIZE,
                                           BSL_CLASS_OF_DEVICE,
                                           instData->secIncoming);
                /* remain in sdsregister state */
            }
        }
    }
    else
    {
        if (instData->numberOfRetries < MAX_NUMBER_OF_SDS_RETRIES)
        {
            (void)SdsRoleDeregister(instData);
            instData->numberOfRetries++;
        }
        else
        {
            csrBtBslSendBslActivateCfm(instData, prim->resultCode, prim->resultSupplier);
            Go2Idle(instData);
        }
    }
}

/*---------------------------------------------------------------------------
  Result of cancel accept connect req. Send result to app.
  ---------------------------------------------------------------------------*/
void CsrBtBslSdsRegisterStateCmBnepCancelConnectAcceptCfmHandler(BslInstanceData_t *instData)
{
    CsrBtCmBnepCancelConnectAcceptCfm *prim;

    prim = (CsrBtCmBnepCancelConnectAcceptCfm *) instData->recvMsgP;
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->activated = FALSE;
        csrBtBslSendBslActivateCfm(instData, CSR_BT_RESULT_CODE_BSL_SUCCESS, CSR_BT_SUPPLIER_BSL);
    }
    else
    {
        csrBtBslSendBslActivateCfm(instData, prim->resultCode, prim->resultSupplier);
    }
    Go2Idle(instData);
}

/*--------------------    Timer handler functions    -----------------------*/
void CsrBtBslXStateCsrBtCmBnepConnectReqSendTimerHandler(CsrUint16 mv, void *mp)
{
    ETHER_ADDR ethAddr;
    BslConnection_t *connection = NULL;
    CsrUint8 connectionIndex;
    BslInstanceData_t *instData = mp;

    connectionIndex = CsrBtBslFindBdAddrIndex(instData, &(instData->currentConn2Addr));
    if (connectionIndex != BSL_INVALID_CONNECTION)
    {
        connection = &instData->connectionList[connectionIndex];
        connection->connectState = ConnectingS;
    }

    instData->bnepConnectReqTimerId = 0;
    ethAddr = CsrBtBdAddrToEtherAddr( &(instData->currentConn2Addr) );
    CsrBtCmBnepConnectReqSend(instData->connFlagsCopy,
                         ethAddr,
                         CSR_BT_BSL_PROFILE_DEFAULT_MTU_SIZE,
                         instData->secOutgoing);
}

void CsrBtBslXStateCsrBtCmBnepRoleSwitchRetryTimerHandler(CsrUint16 id, void *mp)
{
    CSR_UNUSED(mp);
    CsrBtCmBnepSwitchRoleReqSend(id, CSR_BT_MASTER_ROLE);
}


/*--------------------    Multi state functions    -----------------------*/
void CsrBtBslXStateBslActivateReqHandler(BslInstanceData_t *instData)
{
    CsrBtBslActivateReq    *prim;

    prim = (CsrBtBslActivateReq *) instData->recvMsgP;
    instData->wantedRole = prim->localRole;
    instData->singleUser = prim->singleUser;
    instData->remoteRole = prim->remoteRole;
    instData->activated = TRUE;
    instData->numberOfRetries = 0;
    bd_addr_zero( &(instData->currentConn2Addr) );

    /* no register has been sent */
    if (SdsRoleRegister(instData) == TRUE)
    {
        StateChange(instData, SdsRegisterS);
    }
    /* no deregister so we are through */
    else if (SdsRoleDeregister(instData) == TRUE)
    {
        StateChange(instData, SdsRegisterS);
    }
    else
    {
        /* no changes specified so just confirm the request */
        csrBtBslSendBslActivateCfm(instData, CSR_BT_RESULT_CODE_BSL_SUCCESS, CSR_BT_SUPPLIER_BSL);
        StateChange(instData, IdleS);
    }
}

void CsrBtBslXStateBslServiceSearchReqHandler(BslInstanceData_t *instData)
{
    CsrBtBslServiceSearchReq        *prim;
    CsrBtUuid32   *searchUuid = (CsrBtUuid32 *)CsrPmemAlloc(sizeof(CsrBtUuid32));

    prim = (CsrBtBslServiceSearchReq *) instData->recvMsgP;
    instData->currentConn2Addr = prim->bd_addr;
    instData->numberOfRetries = 0;
    instData->sdpHandle = prim->phandle;
    instData->sdcSearchRole = prim->searchRole;

    if (prim->searchRole == CSR_BT_BSL_PANU_ROLE)
    {
        *searchUuid = CSR_BT_PAN_PANU_PROFILE_UUID;
    }
    else if (prim->searchRole == CSR_BT_BSL_NAP_ROLE)
    {
        *searchUuid = CSR_BT_PAN_NAP_PROFILE_UUID;
    }
    else if (prim->searchRole == CSR_BT_BSL_GN_ROLE)
    {
        *searchUuid = CSR_BT_PAN_GN_PROFILE_UUID;
    }
    else
    {
        CsrBtBslSendBslServiceSearchCfm(instData, CSR_BT_RESULT_CODE_BSL_UNEXPECTED_PARAMETER,
            CSR_BT_SUPPLIER_BSL);
        CsrPmemFree(searchUuid);
        return;
    }

    /* set to first attribute to search for */
    instData->sdcAttrSearchIndex = OPENING_SEARCH_CHANNEL_INDEX;
    CsrBtCmSdcSearchReqSend(CSR_BT_BSL_IFACEQUEUE, instData->currentConn2Addr, searchUuid, 1 );
    StateChange(instData, BusyS);
}

void CsrBtBslXStateCmBnepConnectIndHandler(BslInstanceData_t *instData)
{
    CsrBtCmBnepConnectInd    *prim;
    BD_ADDR_T                devAddr;
    CsrUint8 connectionIndex;
    BslConnection_t *connection = NULL;

    prim = (CsrBtCmBnepConnectInd *) instData->recvMsgP;
    devAddr = CsrBtBdAddrFromEtherAddr( &prim->rem_addr );
    connectionIndex = CsrBtBslFindBdAddrIndex(instData, &devAddr);

    if (connectionIndex == BSL_INVALID_CONNECTION)
    {
        connectionIndex = CsrBtBslFindEmptyIndex(instData);
        if (connectionIndex == BSL_INVALID_CONNECTION)
        {
            /* If this happens, we have bigger problems */
            return;
        }
    }
    connection = &instData->connectionList[connectionIndex];
    if (CsrBtBdAddrEqZero(&connection->bdAddr))
    {
        connection->bdAddr = devAddr;
    }

    if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if (connection->connectCancelled)
        {
            CsrBtCmBnepDisconnectReqSend( 0, prim->id);
            connection->connectState = DisconnectingS;
            connection->id = prim->id;
        }
        else
        {
            CsrBool outgoing = FALSE;
            /* inform app layer of success */
            CsrBtBslSendBslConnectInd(instData->appHandle,
                                 prim->id,
                                 &devAddr,
                                 CsrBtBslGetRoleFromUuid(prim->rem_uuid),
                                 CsrBtBslGetRoleFromUuid(prim->loc_uuid),
                                 CSR_BT_RESULT_CODE_BSL_SUCCESS,
                                 CSR_BT_SUPPLIER_BSL);

            if (connection->connectState == ConnectingS)
            {
                outgoing = TRUE;
            }
            connection->connectState = ConnectedS;
            connection->id = prim->id;

            /* initiate role switch if in multiuser mode and local role is gn or nap */
            if ( (!instData->singleUser) &&  /* multiuser and... */
                 ((outgoing && ((connection->outgoingRole & CSR_BT_BSL_PANU_ROLE) == 0)) || /* ...outgoing and either GN or NAP role or...*/
                 ((!outgoing) && (instData->localRole & ~CSR_BT_BSL_PANU_ROLE))) ) /* ..incoming and PANU not supported */
            {
                instData->numberOfRetries = 0;
                CsrBtCmBnepSwitchRoleReqSend(prim->id, CSR_BT_MASTER_ROLE);
            }
            else
            {
                /* enter sniff mode if enable by define */
                if (CSR_BT_BSL_ENABLE_SNIFF)
                {
                    CsrBtCmBnepModeChangeReqSend(prim->id, CSR_BT_SNIFF_MODE);
                }
            }

            /* connect ind may be received for both accept and req. If
             * address stored while initiating a connect is received then
             * handle this as a 'connect cfm' */
            if ( bd_addr_eq( &(instData->currentConn2Addr), &devAddr) )
            {
                Go2Idle(instData);
            }
        }
    }
    else if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
        prim->resultCode == CSR_BT_RESULT_CODE_CM_CANCELLED)
    {
        if (connection->connectCancelled)
        {
            CsrBtBslSendBslConnectInd(instData->appHandle,
                                 prim->id,
                                 &devAddr, CsrBtBslGetRoleFromUuid(prim->rem_uuid),
                                 CsrBtBslGetRoleFromUuid(prim->loc_uuid),
                                 CSR_BT_RESULT_CODE_BSL_CONNECT_CANCELLED,
                                 CSR_BT_SUPPLIER_BSL);
            Go2Idle(instData);
            CsrBtBslClearConnection(connection);
        }
        else
        {
            ; /* Something is seriously hosed. */
        }
    }
    else if (prim->resultCode == BNEP_CONNECT_CFM_STATUS_TRYLATER && prim->resultSupplier == CSR_BT_SUPPLIER_BNEP_CONNECT_CFM)
    {
        if (connection->connectCancelled)
        {
            CsrBtBslSendBslConnectInd(instData->appHandle,
                                 prim->id,
                                 &devAddr, CsrBtBslGetRoleFromUuid(prim->rem_uuid),
                                 CsrBtBslGetRoleFromUuid(prim->loc_uuid),
                                 CSR_BT_RESULT_CODE_BSL_CONNECT_CANCELLED,
                                 CSR_BT_SUPPLIER_BSL);
            Go2Idle(instData);
            CsrBtBslClearConnection(connection);
        }
        else
        {
            /* bnep is busy at the receipt of the connect req so we will
             * give it another try using the copied connect req signal */
            if (instData->numberOfRetries < MAX_NUMBER_OF_CONNECT_RETRIES)
            {
                instData->numberOfRetries++;

                instData->bnepConnectReqTimerId = CsrSchedTimerSet(CONNECT_RETRY_DELAY,
                                                                 CsrBtBslXStateCsrBtCmBnepConnectReqSendTimerHandler,
                                                                 0,
                                                                 (void *)instData);
                connection->connectState = ConnectingRetryS;
            }
            else
            {
                CsrBtBslSendBslConnectInd( instData->appHandle,
                                      prim->id,
                                      &devAddr,
                                      CSR_BT_BSL_NO_ROLE, CSR_BT_BSL_NO_ROLE,
                                      prim->resultCode,
                                      prim->resultSupplier);
                Go2Idle(instData);
                CsrBtBslClearConnection(connection);
            }
        }
    }
    else
    {
        CsrBtBslSendBslConnectInd(instData->appHandle,
                             prim->id,
                             &devAddr, CsrBtBslGetRoleFromUuid(prim->rem_uuid),
                             CsrBtBslGetRoleFromUuid(prim->loc_uuid),
                             prim->resultCode,
                             prim->resultSupplier);
        Go2Idle(instData);
        CsrBtBslClearConnection(connection);
    }
}

void CsrBtBslXStateBslDisconnectReqHandler(BslInstanceData_t *instData)
{
    CsrUint8 connectionIndex;
    BslConnection_t *connection = NULL;
    CsrBtBslDisconnectReq *prim;

    prim = (CsrBtBslDisconnectReq *) instData->recvMsgP;
    connectionIndex = CsrBtBslFindIdIndex(instData, prim->id);

    if (connectionIndex != BSL_INVALID_CONNECTION)
    {
        connection = &instData->connectionList[connectionIndex];
        connection->connectState = DisconnectingS;
    }
    CsrBtCmBnepDisconnectReqSend( 0, prim->id);
}

void CsrBtBslXStateCmBnepDisconnectIndHandler(BslInstanceData_t *instData)
{
    CsrUint8 connectionIndex;
    BslConnection_t *connection = NULL;
    CsrBtCmBnepDisconnectInd *prim;

    prim = (CsrBtCmBnepDisconnectInd *) instData->recvMsgP;
    connectionIndex = CsrBtBslFindIdIndex(instData, prim->id);

    if (connectionIndex != BSL_INVALID_CONNECTION)
    {
        connection = &instData->connectionList[connectionIndex];
        connection->connectState = IdleConnectionS;
    }
    if (connection && connection->connectCancelled)
    {
        CsrBtBslSendBslConnectInd(instData->appHandle,
                             prim->id,
                             &instData->currentConn2Addr, instData->remoteRole,
                             instData->localRole,
                             CSR_BT_RESULT_CODE_BSL_CONNECT_CANCELLED,
                             CSR_BT_SUPPLIER_BSL);
        Go2Idle(instData);
    }
    else
    {
        CsrBtBslSendBslDisconnectInd(instData->appHandle, prim->id,
                                    prim->reasonCode, prim->reasonSupplier);
    }
    if (connection)
    {
        CsrBtBslClearConnection(connection);
    }
}

void CsrBtBslXStateBslDisconnectResHandler(BslInstanceData_t *instData)
{
    CsrBtBslDisconnectRes *prim;

    prim = (CsrBtBslDisconnectRes *) instData->recvMsgP;
    CsrBtCmBnepDisconnectResSend( 0, prim->id);
}

void CsrBtBslXStateSaveMessage(BslInstanceData_t *instData)
{
    CsrMessageQueuePush( &(instData->saveQueue),CSR_BT_BSL_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

void CsrBtBslXStateCmBnepExtendedDataIndHandler(BslInstanceData_t *instData)
{
    CsrBtCmBnepExtendedDataInd    *prim;

    prim = (CsrBtCmBnepExtendedDataInd *) instData->recvMsgP;
    prim->type = CSR_BT_BSL_DATA_IND;
    instData->recvMsgP = NULL;
    prim->dstAddr = CsrBtBslConvNativeEther2EtherBig( &prim->dstAddr);
    prim->srcAddr = CsrBtBslConvNativeEther2EtherBig( &prim->srcAddr);
    CsrBtPanMessagePut(instData->dataHandle, prim);
}

void CsrBtBslXStateBslDataReqHandler(BslInstanceData_t *instData)
{
    CsrBtBslDataReq    *prim;

    prim = (CsrBtBslDataReq *) instData->recvMsgP;
    /*
     * If the lowest (1) bit of the first eight bits
     * of the dst MAC addr is NOT set then the frame
     * is unicast.  Otherwise it is multicast (possibly
     * broadcast which is just a special case of mcast).
     */
    if (!ETHER_IS_MULTICAST(&prim->dstAddr))
    {
        prim->type = CSR_BT_CM_BNEP_EXTENDED_DATA_REQ;
    }
    else
    {
        prim->id = CSR_BT_BSL_ID_MULTI_NO_LOCAL;
        prim->type = CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ;
    }
    /* ensure that the signal is not free'ed on exit of bsl handler function */
    instData->recvMsgP = NULL;
    prim->dstAddr = CsrBtBslConvEtherBig2Native( &prim->dstAddr);
    prim->srcAddr = CsrBtBslConvEtherBig2Native( &prim->srcAddr);
    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, prim);
}

void CsrBtBslXStateBslMulticastDataReqHandler(BslInstanceData_t *instData)
{
    CsrBtBslMulticastDataReq    *prim;

    prim = (CsrBtBslMulticastDataReq *) instData->recvMsgP;
    prim->type = CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ;
    instData->recvMsgP = NULL;
    prim->dstAddr = CsrBtBslConvEtherBig2Native( &prim->dstAddr);
    prim->srcAddr = CsrBtBslConvEtherBig2Native( &prim->srcAddr);
    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, prim);
}

void CsrBtBslXStateCmBnepModeChangeIndHandler(BslInstanceData_t *instData)
{
    CsrBtCmBnepModeChangeInd *prim;

    prim = (CsrBtCmBnepModeChangeInd *) instData->recvMsgP;
    /* only inform app layer if successful */
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtBslSendBslStatusInd(instData, prim->id, CSR_BT_BSL_LINK_STATE_EVENT, prim->mode);
        /* do not want to be in park mode so try sniff instead (if enabled by define) */
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        if (prim->mode == CSR_BT_PARK_MODE)
        {
            if (CSR_BT_BSL_ENABLE_SNIFF)
            {
                CsrBtCmBnepModeChangeReqSend(prim->id, CSR_BT_LINK_STATUS_SNIFF);
            }
            else
            {
                CsrBtCmBnepModeChangeReqSend(prim->id, CSR_BT_LINK_STATUS_CONNECTED);
            }
        }
#endif
    }
}

void CsrBtBslXStateCmSwitchRoleIndHandler(BslInstanceData_t *instData)
{
    CsrBtCmBnepSwitchRoleInd *prim;

    prim = (CsrBtCmBnepSwitchRoleInd *) instData->recvMsgP;
    /* if successful just pass information to app */
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtBslSendBslStatusInd(instData, prim->id, CSR_BT_BSL_SWITCH_ROLE_EVENT, prim->role);
        /* try to enter sniff mode    if enabled */
        if (CSR_BT_BSL_ENABLE_SNIFF)
        {
            CsrBtCmBnepModeChangeReqSend(prim->id, CSR_BT_SNIFF_MODE);
        }
    }
    else
    {
        if (instData->numberOfRetries < MAX_NUMBER_OF_DM_RETRIES)
        {
            /* role change may fail due to collision on air if so we give
               it an additional try */
            instData->numberOfRetries++;
            instData->bnepSwitchRoleReqTimerId = CsrSchedTimerSet(BSL_ROLE_SWITCH_RETRY_DELAY,
                                                             CsrBtBslXStateCsrBtCmBnepRoleSwitchRetryTimerHandler,
                                                             prim->id,
                                                             NULL);
            
        }
        else
        {
            /* role change not completed successfully so we will disconnect. Cm/bnep
               will send a disconnect ind which we will pass on to the app layer. App
               layer will send a disc req which we will pass on to cm and here it is
               ignored due to id unknown */
            CsrBtCmBnepDisconnectReqSend(0, prim->id);
        }
    }
}

void CsrBtBslIgnoreMessage(BslInstanceData_t *instData)
{
    /* Nop */
    CSR_UNUSED(instData);
}

void CsrBtBslSecurityInReqHandler(BslInstanceData_t *instData)
{
    CsrBtResultCode rval;
    CsrBtBslSecurityInReq *prim;
    prim = (CsrBtBslSecurityInReq*)instData->recvMsgP;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_PAN_MANDATORY_SECURITY_INCOMING,
        CSR_BT_PAN_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_BSL_SUCCESS,
        CSR_BT_RESULT_CODE_BSL_UNACCEPTABLE_PARAMETER);

    csrBtBslSecurityInCfmSend(prim->appHandle,
         rval, CSR_BT_SUPPLIER_BSL);
}

void CsrBtBslSecurityOutReqHandler(BslInstanceData_t *instData)
{
    CsrBtResultCode rval;
    CsrBtBslSecurityOutReq *prim;
    prim = (CsrBtBslSecurityOutReq*)instData->recvMsgP;

    rval = CsrBtScSetSecOutLevel(&instData->secOutgoing, prim->secLevel,
        CSR_BT_PAN_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_PAN_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_BSL_SUCCESS,
        CSR_BT_RESULT_CODE_BSL_UNACCEPTABLE_PARAMETER);

    csrBtBslSecurityOutCfmSend(prim->appHandle,
         rval, CSR_BT_SUPPLIER_BSL);

}

static CsrBool cancelBslMsg(BslInstanceData_t *instData, CsrBtBslPrim type, BD_ADDR_T bd_addr)
{
    CsrUint16                eventClass;
    void                    *msg;
    CsrBool                  cancelMsg   = FALSE;
    CsrMessageQueueType    *tempQueue  = NULL;

    while(CsrMessageQueuePop(&instData->saveQueue, &eventClass, &msg))
    {
        if (!cancelMsg && eventClass == CSR_BT_BSL_PRIM && (type == (*((CsrBtBslPrim *) msg))))
        {
            switch (type)
            {
                case CSR_BT_BSL_CONNECT_REQ:
                {
                    CsrBtBslConnectReq * prim = (CsrBtBslConnectReq *) msg;

                    if (bd_addr_eq(&(prim->bdAddr), &(bd_addr)))
                    {
                        cancelMsg = TRUE;
                        CsrPmemFree(msg);
                    }
                    else
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    }
                    break;
                }
                default:
                {
                    CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    break;
                }
            }
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, eventClass, msg);
        }
    }
    instData->saveQueue = tempQueue;
    return (cancelMsg);
}


void CsrBtBslXStateCancelConnectReqHandler(BslInstanceData_t *instData)
{
    CsrBtBslCancelConnectReq *prim;
    CsrUint8 connectionIndex;

    prim = (CsrBtBslCancelConnectReq *)instData->recvMsgP;
    connectionIndex = CsrBtBslFindBdAddrIndex(instData, &prim->bdAddr);
    if (connectionIndex != BSL_INVALID_CONNECTION)
    {
        BslConnection_t *connection;
        connection = &instData->connectionList[connectionIndex];
        switch (connection->connectState)
        {
            case IdleConnectionS:
            {
                /* Connection cannot be cancelled because no connection is underway */
                break;
            }
            case ConnectCancelAcceptS:
            {
                connection->connectCancelled = TRUE;
                break;
            }
            case ConnectingS:
            {
                CsrBtCmCancelBnepConnectReqSend(CsrBtBdAddrToEtherAddr(&(prim->bdAddr)));
                connection->connectCancelled = TRUE;
                break;
            }
            case ConnectingRetryS:
            {
                CsrSchedTimerCancel(instData->bnepConnectReqTimerId, NULL, NULL);
                instData->bnepConnectReqTimerId = 0;
                instData->numberOfRetries = 0;
                connection->connectState = IdleConnectionS;
                CsrBtBslSendBslConnectInd(instData->appHandle,
                                     0,
                                     &prim->bdAddr, instData->remoteRole,
                                     instData->localRole,
                                     CSR_BT_RESULT_CODE_BSL_CONNECT_CANCELLED,
                                     CSR_BT_SUPPLIER_BSL);
                break;
            }
            case ConnectedS:
            {
                CsrBtCmBnepDisconnectReqSend(0, connection->id);
                connection->connectState = DisconnectingS;
                break;
            }
            case DisconnectingS:
            {
                /* Cancel cannot be sent while disconnecting, ignore */
                break;
            }
            default:
            {
                /* This should never happen */
                break;
            }
        }
    }
    else
    {
        /* We do not know anything about this device, check the save queue */
        if (cancelBslMsg(instData, CSR_BT_BSL_CONNECT_REQ, prim->bdAddr))
        {
            CsrBtBslSendBslConnectInd(instData->appHandle,
                                 0,
                                 &prim->bdAddr, 0, 0,
                                 CSR_BT_RESULT_CODE_BSL_CONNECT_CANCELLED,
                                 CSR_BT_SUPPLIER_BSL);
        }
        /* Else the cancel is unknown, so ignore. */
    }
}

