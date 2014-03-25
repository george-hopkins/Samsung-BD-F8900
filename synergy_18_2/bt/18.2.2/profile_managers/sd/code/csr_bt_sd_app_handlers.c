/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"

#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
void CsrBtSdMemoryConfigReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdMemoryConfigReq *prim = (CsrBtSdMemoryConfigReq *)mainInst->recvMsgP;
    CsrBtResultCode res = CSR_BT_RESULT_CODE_SD_UNACCEPTABLE_PARAMETER;

    if ((prim->deviceListMax >= prim->deviceListInfoMax) || (prim->deviceListMax == CSR_BT_UNLIMITED))
    {
        /* Parameters are valid */
        CsrBtSdUtilDLTrim(mainInst);
        mainInst->memoryConfig  = prim->memoryConfig;
        mainInst->dlMax         = prim->deviceListMax;
        mainInst->dlInfoMax     = prim->deviceListInfoMax;
        res                     = CSR_BT_RESULT_CODE_SD_SUCCESS;
    }
    CsrBtSdSendSdMemoryConfigCfm(mainInst,prim->phandle, res, CSR_BT_SUPPLIER_SD);
    CsrBtSdUtilGo2Idle(mainInst);
}
#endif /* CSR_BT_INSTALL_SD_MEMORY_CONFIG */

#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
void CsrBtSdSearchConfigReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdSearchConfigReq *prim = (CsrBtSdSearchConfigReq *)mainInst->recvMsgP;
    SdSearchInstance_t *searchInst = CsrBtSdUtilSearchInstGetFromHandle(mainInst, prim->phandle);
    CsrBtResultCode res = CSR_BT_RESULT_CODE_SD_UNACCEPTABLE_PARAMETER;
    if (searchInst)
    {
        if (!CSR_MASK_IS_SET(prim->searchConfig, CSR_BT_SD_SEARCH_USE_PRECONFIGURED))
        {
            searchInst->searchConfig        = prim->searchConfig;
        }
        searchInst->readNameTimeout     = prim->readNameTimeout;
        searchInst->searchResultsMax    = prim->maxSearchResults;
        res                             = CSR_BT_RESULT_CODE_SD_SUCCESS;
    }
    else
    {/* No more instances could be added - ignore since no confirmation is available */
    }
    CsrBtSdSendSdSearchConfigCfm(searchInst, prim->phandle, res, CSR_BT_SUPPLIER_SD);
    CsrBtSdUtilGo2Idle(mainInst);
}
#endif /* CSR_BT_INSTALL_SD_SEARCH_CONFIG */

void CsrBtSdReadDeviceInfoReqAllState(SdMainInstance_t *mainInst)
{
    CsrBtSdReadDeviceInfoReq *prim = (CsrBtSdReadDeviceInfoReq *)mainInst->recvMsgP;
    SdDeviceInfo_t *deviceInfo = SD_DL_FIND_FROM_ADDR(mainInst->dl, &prim->deviceAddr);

    if (deviceInfo)
    {
        CsrBtSdSendReadDeviceInfoCfm(prim->phandle, 
                                     prim->deviceAddr, 
                                     deviceInfo,
                                     CSR_BT_RESULT_CODE_SD_SUCCESS, 
                                     CSR_BT_SUPPLIER_SD,
                                     (CsrBtAddressType)((deviceInfo->devFlags & SD_DI_DEV_FLAG_LE_PRIVATE_ADDR)
                                                        ? CSR_BT_ADDR_PRIVATE
                                                        : CSR_BT_ADDR_PUBLIC),
                                     prim->context);
    }
    else
    {
        CsrBtSdSendReadDeviceInfoCfm(prim->phandle, 
                                     prim->deviceAddr, 
                                     deviceInfo,
                                     CSR_BT_RESULT_CODE_SD_DEVICE_NOT_FOUND, 
                                     CSR_BT_SUPPLIER_SD,
                                     prim->addressType, 
                                     prim->context); 
    }
}

void CsrBtSdReadDeviceListReqAllState(SdMainInstance_t *mainInst)
{
    CsrBtSdReadDeviceListReq *prim = (CsrBtSdReadDeviceListReq *)mainInst->recvMsgP;

    CsrBtSdUtilGenerateReadDLInd(mainInst,
                            prim->maxNumOfBytesInEachResult,
                            prim->deviceListConfig,
                            prim->phandle);
}

void CsrBtSdSearchReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdSearchReq *prim;
    SdSearchInstance_t *searchInst;

    prim = (CsrBtSdSearchReq *)mainInst->recvMsgP;
    searchInst = CsrBtSdUtilSearchInstGetFromHandle(mainInst, prim->phandle);
    mainInst->inquiryTxPowerLevel = prim->inquiryTxPowerLevel;

    if (searchInst && CsrBtSdSearchSetupParameters(mainInst, searchInst, prim))
    {
        SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_INQ);

        if (mainInst->processState != SD_STATE_PROC_SET_EVENT_TO_START)
        {
            /* Only start inquiry if the CoD filter is not being updated */
            CsrBtSdSearchInquiryStartFirst(mainInst);
        }

        /* Start other searches pending on the save queue */
        (void)CsrBtSdSearchQueueHandler(mainInst, prim->phandle, FALSE);
    }
    else
    {
        CsrBtSdSendCloseSearchInd(prim->phandle,
                                  CSR_BT_RESULT_CODE_SD_COMMAND_DISALLOWED,
                                  CSR_BT_SUPPLIER_SD);
        CsrBtSdUtilGo2Idle(mainInst); /* In order to restore pending messages */
    }
}

void CsrBtSdSearchReqInqState(SdMainInstance_t *mainInst)
{
    CsrBtSdSearchReq *prim = (CsrBtSdSearchReq *)mainInst->recvMsgP;

    mainInst->inquiryTxPowerLevel = prim->inquiryTxPowerLevel;

    /* Handle the specific search request */
    if ((mainInst->processState != SD_STATE_PROC_CRRN_TO_STOP) &&
        (mainInst->processState != SD_STATE_PROC_SET_EVENT_TO_STOP))
    {
        CsrBtSdSearchReqCommonHandler(mainInst, prim);
    }
    else
    {
        /* Save the message and handle it after name reading or event filter has been fully completed */
        CsrBtSdUtilSaveMessage(mainInst);
    }
}

void CsrBtSdCancelSearchReqAllState(SdMainInstance_t *mainInst)
{
    CsrBtSdCancelSearchReq *prim = (CsrBtSdCancelSearchReq *)mainInst->recvMsgP;

    if (!CsrBtSdSearchQueueHandler(mainInst, prim->phandle, TRUE))
    {
        /* The specific search was not present on the queue */
        SdSearchInstance_t *searchInst = SD_SI_FIND_FROM_PHANDLE(mainInst->searchInst, &prim->phandle);

        if (searchInst && (searchInst->state != SD_STATE_SEARCH_INST_IDLE))
        {
            /* The search is in progress - cancel it */
            CsrBtSdSearchCompleteHandler(searchInst,
                                         CSR_BT_RESULT_CODE_SD_SEARCH_CANCELLED,
                                         CSR_BT_SUPPLIER_SD);
        }
        /* Unknown app - just ignore */
    }
}

void CsrBtSdReadAvailableServicesReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdReadAvailableServicesReq *prim = (CsrBtSdReadAvailableServicesReq *)mainInst->recvMsgP;

    mainInst->phandle       = prim->phandle;
    mainInst->serviceConfig = prim->serviceConfig;
    mainInst->sdpMsgType    = prim->type;
    mainInst->currentDevice = CsrBtSdUtilDLAddFromAddrOnly(mainInst, &prim->deviceAddr);
    
    CsrBtSdRestartServiceSearch(mainInst);
}

void CsrBtSdSaveReadAvailableServicesReqHandler(SdMainInstance_t *mainInst)
{
    CsrBtSdReadAvailableServicesReq *prim = (CsrBtSdReadAvailableServicesReq *) mainInst->recvMsgP;

    if (CsrBtSdSdpQueueHandler(mainInst, prim->type, prim->phandle, TRUE))
    {
        /* Save the message */
        CsrBtSdUtilSaveMessage(mainInst);
    }
    else
    {
        /* The application has already save one request, return error */
        CsrBtSdSendReadAvailableServicesCfmWithError(prim->phandle, prim->deviceAddr);
        CsrPmemFree(prim->filter);
        prim->filter = NULL;
    }
}

void CsrBtSdCancelReadAvailableServicesReqAllState(SdMainInstance_t *mainInst)
{
    CsrBtSdCancelReadAvailableServicesReq *prim = (CsrBtSdCancelReadAvailableServicesReq *) mainInst->recvMsgP;

    /* Check if the CsrBtSdReadAvailableServicesReq is placed on the local queue */
    if (CsrBtSdSdpQueueHandler(mainInst, CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ, prim->phandle, FALSE))
    {/* The CsrBtSdReadAvailableServicesReq msg were found and is removed from the local queue. Inform the application */
        CsrBtSdSendCancelReadAvailableServicesCfm(prim->phandle);
    }
    else if ((mainInst->appState == SD_STATE_APP_OTHER)  &&
             (mainInst->phandle == prim->phandle) &&
             (mainInst->sdpMsgType == CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ))
    {/* The CsrBtSdReadAvailableServicesReq that must be cancelled is running - cancel it */
        CsrBtSdSendCancelReadAvailableServicesCfm(prim->phandle);

        if (mainInst->processState == SD_STATE_PROC_SDC_SEARCH)
        {
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDC_SEARCH_CANCEL);
            CsrBtUtilSdcSearchCancel(mainInst, mainInst->sdpSearchData);
        }
        else
        {/* The eventState must be SD_STATE_PROC_RRN */
            CsrBtSdUtilReadRemoteNameStop(mainInst);
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_CRRN_TO_STOP);
        }
    }
    else
    {/* Just ignore */
    }
}

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
void CsrBtSdReadServiceRecordReqInqSave(SdMainInstance_t *mainInst)
{
    CsrBtSdReadServiceRecordReq *prim = (CsrBtSdReadServiceRecordReq *) mainInst->recvMsgP;

    if (CsrBtSdSdpQueueHandler(mainInst, prim->type, prim->phandle, TRUE))
    {/* Save the message */
        CsrBtSdUtilSaveMessage(mainInst);
    }
    else
    {/* The application has already saved one request, return error */
        CsrBtSdSendReadServiceRecordCfm(prim->phandle, CSR_BT_RESULT_CODE_SD_COMMAND_DISALLOWED,
            CSR_BT_SUPPLIER_SD);
        CsrPmemFree(prim->data);
        prim->data = NULL;
    }
}

void CsrBtSdCancelReadServiceRecordReqAllState(SdMainInstance_t *mainInst)
{
    CsrBtSdCancelReadServiceRecordReq *prim = (CsrBtSdCancelReadServiceRecordReq *) mainInst->recvMsgP;

    if (mainInst->appState == SD_STATE_APP_OTHER  &&
        mainInst->phandle  == prim->phandle &&
        mainInst->sdpMsgType == CSR_BT_SD_READ_SERVICE_RECORD_REQ)
    {/* The CsrBtSdReadServiceRecordReq that must be cancelled, is running, cancel it */
        CsrBtSdSendReadServiceRecordCfm(prim->phandle, CSR_BT_RESULT_CODE_SD_SEARCH_CANCELLED,
            CSR_BT_SUPPLIER_SD);

        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDC_SEARCH_CANCEL);
        CsrBtUtilSdcSearchCancel(mainInst, mainInst->sdpSearchData);
    }
    else
    {/* Check if the CsrBtSdReadServiceRecordReq is placed on the local queue */
        if (CsrBtSdSdpQueueHandler(mainInst, CSR_BT_SD_READ_SERVICE_RECORD_REQ, prim->phandle, FALSE))
        {/* The CsrBtSdReadServiceRecordReq msg were found and is removed from the local queue. Inform the application */
            CsrBtSdSendReadServiceRecordCfm(prim->phandle, CSR_BT_RESULT_CODE_SD_SEARCH_CANCELLED,
                CSR_BT_SUPPLIER_SD);
        }
        /* Nothing to cancel, just ignore */
    }
}

void CsrBtSdRegisterServiceRecordReqSave(SdMainInstance_t *mainInst)
{
    CsrBtSdRegisterServiceRecordReq *prim = (CsrBtSdRegisterServiceRecordReq *) mainInst->recvMsgP;

    if (CsrBtSdSdpQueueHandler(mainInst, prim->type, prim->phandle, TRUE))
    {/* Save the message */
        CsrBtSdUtilSaveMessage(mainInst);
    }
    else
    {/* The application has already save one request, return error */
        CsrBtSdSendRegisterServiceRecordCfm(prim->phandle, CMN_SDR_INVALID_SERVICE_HANDLE,
            CSR_BT_RESULT_CODE_SD_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_SD);
        CsrPmemFree(prim->data);
        prim->data = NULL;
    }
}

void CsrBtSdUnregisterServiceRecordReqSave(SdMainInstance_t *mainInst)
{
    CsrBtSdUnregisterServiceRecordReq*prim = (CsrBtSdUnregisterServiceRecordReq *) mainInst->recvMsgP;

    if (CsrBtSdSdpQueueHandler(mainInst, prim->type, prim->phandle, TRUE))
    {/* Save the message */
        CsrBtSdUtilSaveMessage(mainInst);
    }
    else
    {/* The application has already saved one request, return error */
        CsrBtSdSendUnregisterServiceRecordCfm(prim->phandle, prim->serviceHandle,
            CSR_BT_RESULT_CODE_SD_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_SD);
    }
}

void CsrBtSdReadServiceRecordReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdReadServiceRecordReq *prim = (CsrBtSdReadServiceRecordReq *) mainInst->recvMsgP;

    if (prim->data && prim->dataLen > 0)
    {
        CmnCsrBtLinkedListStruct *sdpTag;

        mainInst->phandle       = prim->phandle;
        mainInst->sdpMsgType    = prim->type;

        mainInst->currentDevice = CsrBtSdUtilDLAddFromAddrOnly(mainInst, &prim->deviceAddr);

        sdpTag = CsrBtUtilBllCreateNewEntry(NULL, prim->data, prim->dataLen);
        CsrBtUtilSdcSearchStart(mainInst, mainInst->sdpSearchData, sdpTag, mainInst->currentDevice->addr);
        prim->data = NULL;

        SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_OTHER);
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDC_SEARCH);
    }
    else
    {
        CsrBtSdSendReadServiceRecordCfm(prim->phandle, CSR_BT_RESULT_CODE_SD_UNACCEPTABLE_PARAMETER,
                CSR_BT_SUPPLIER_SD);
        CsrBtSdUtilGo2Idle(mainInst);
    }
}

void CsrBtSdRegisterServiceRecordReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdRegisterServiceRecordReq *prim = (CsrBtSdRegisterServiceRecordReq *) mainInst->recvMsgP;

    if (prim->data && prim->dataLen > 0)
    {
        mainInst->phandle           = prim->phandle;
        mainInst->sdpMsgType        = prim->type;
        mainInst->sdpRecord         = CsrMemDup(prim->data, prim->dataLen);
        mainInst->sdpRecordLength   = prim->dataLen;
        mainInst->sdpRetryCounter   = 0;
        CsrBtCmSdsRegisterReqSend(CSR_BT_SD_IFACEQUEUE, prim->data, prim->dataLen);
        SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_OTHER);
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDS_REGISTER);
    }
    else
    {
        CsrBtSdSendRegisterServiceRecordCfm(prim->phandle, CMN_SDR_INVALID_SERVICE_HANDLE,
            CSR_BT_RESULT_CODE_SD_UNACCEPTABLE_PARAMETER, CSR_BT_SUPPLIER_SD);
        CsrBtSdUtilGo2Idle(mainInst);
    }
}

void CsrBtSdUnregisterServiceRecordReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdUnregisterServiceRecordReq  *prim = (CsrBtSdUnregisterServiceRecordReq *) mainInst->recvMsgP;
    CsrUint16                      index;

    if (CsrBtSdGetServiceRecordHandleIndex(mainInst, prim->serviceHandle, &index))
    {
        mainInst->phandle           = prim->phandle;
        mainInst->sdpMsgType        = prim->type;
        mainInst->sdpRetryCounter   = 0;
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_SD_IFACEQUEUE, prim->serviceHandle);
        SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_OTHER);
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDS_UNREGISTER);
    }
    else
    {
        CsrBtSdSendUnregisterServiceRecordCfm(prim->phandle, prim->serviceHandle,
            CSR_BT_RESULT_CODE_SD_UNKNOWN_SERVICE_RECORD_HANDLE, CSR_BT_SUPPLIER_SD);
        CsrBtSdUtilGo2Idle(mainInst);
    }
}
#endif /*EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */

void CsrBtSdCleanupReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdCleanupReq *prim = (CsrBtSdCleanupReq *)mainInst->recvMsgP;
    SdSearchInstance_t *searchInst = SD_SI_FIND_FROM_PHANDLE(mainInst->searchInst, &prim->phandle);
    CsrBtResultCode res = CSR_BT_RESULT_CODE_SD_UNACCEPTABLE_PARAMETER;

    switch (prim->cleanupMode)
    {
        case CSR_BT_SD_CLEANUP_EVERYTHING:
        {
            /* Remove all non-paired devices */
            CsrBtSdUtilDLCleanNonPaired(mainInst);

            /* Remove all search instances */
            CsrCmnListDeinit(&mainInst->searchInst);
            res     = CSR_BT_RESULT_CODE_SD_SUCCESS;
            break;
        }

        case CSR_BT_SD_CLEANUP_INSTANCE:
        {
            if (searchInst)
            {
                /* Reset all shown bits for current instance */
                SD_DL_ITERATE(mainInst->dl, CsrBtSdUtilSearchInstUnsetInst, &searchInst->searchInstIdx);

                /* Clean DL */
                CsrBtSdUtilDLCleanAll(mainInst);

                /* Remove search instance */
                SD_SI_REMOVE_INST(mainInst->searchInst, searchInst);
                res     = CSR_BT_RESULT_CODE_SD_SUCCESS;
            }
            break;
        }

        case CSR_BT_SD_CLEANUP_DEVICE_LIST:
        {
            /* Remove all non-paired devices */
            CsrBtSdUtilDLCleanNonPaired(mainInst);
            res     = CSR_BT_RESULT_CODE_SD_SUCCESS;
            break;
        }

        default:
        {/* Just ignore */
            break;
        }
    }

    CsrBtSdUtilGo2Idle(mainInst);
    CsrBtSdSendSdCleanUpCfm(prim->phandle,prim->cleanupMode, res, CSR_BT_SUPPLIER_SD);
}

void CsrBtSdRegisterAvailableServiceReqAllState(SdMainInstance_t *mainInst)
{
    CsrBtSdRegisterAvailableServiceReq *prim = (CsrBtSdRegisterAvailableServiceReq *)mainInst->recvMsgP;

    if (!CsrBtSdUtilUuid32Check(mainInst, prim->service))
    {
        if (!(mainInst->uuid32ListCount < mainInst->uuid32ListSize))
        {/* Not enough room in buffer for UUID32 - reallocate */
            CsrBtUuid32 *tempList;
            CsrUintFast32 i;

            mainInst->uuid32ListSize = mainInst->uuid32ListSize + SD_POOL_SIZE_FOR_UUID32_LIST;
            tempList = CsrPmemAlloc(sizeof(CsrBtUuid32) * mainInst->uuid32ListSize);

            /* Reinsert the old buffer */
            for (i = 0; i < mainInst->uuid32ListCount; i++)
            {
                tempList[i] = mainInst->uuid32List[i];
            }

            CsrPmemFree(mainInst->uuid32List);
            mainInst->uuid32List = tempList;
        }

        mainInst->uuid32List[mainInst->uuid32ListCount] = prim->service;
        mainInst->uuid32ListCount++;
    }
}

void CsrBtSdUpdateDeviceReqAllState(SdMainInstance_t *mainInst)
{
    CsrBtSdUpdateDeviceReq *prim = (CsrBtSdUpdateDeviceReq *)mainInst->recvMsgP;

    if (CSR_MASK_IS_SET(prim->details, SD_UPDATE_DEVICE_BOND))
    {/* Device is bonded */
        SdDeviceInfo_t *deviceInfo = CsrBtSdUtilDLAddFromAddrOnly(mainInst, &prim->addr);

        if (prim->cod != 0)
        {
            deviceInfo->cod = prim->cod;
        }

        if (CSR_MASK_IS_SET(prim->details, SD_UPDATE_DEVICE_TRUSTED))
        {/* When this signal is sent specifically to the SD, 'addedToScDbList'
            actually indicates if the device is authorised (trusted) or not */
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_TRUSTED);
        }

        if (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED))
        {/* Only adjust counters if not previously paired */
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED);
            mainInst->dlPairedCount++;

            if (deviceInfo->infoLen)
            {/* Adjust info counters */
                mainInst->dlInfoCount--;
                mainInst->dlPairedInfoCount++;

                /* Update the SCDB with any info available in the SD */
                CsrBtSdUtilUpdateTrustedDeviceList(deviceInfo);
            }
        }
    }
    else if (CSR_MASK_IS_SET(prim->details, SD_UPDATE_DEVICE_DEBOND))
    {/* Device is debonded */
        if(CsrBtBdAddrEqZero(&prim->addr))
        { /* Device Addr of zero means debond all */
            CsrBtSdUtilDLCleanPairedList(mainInst);
        }
        else
        {
            SdDeviceInfo_t *deviceInfo = SD_DL_FIND_FROM_ADDR(mainInst->dl, &prim->addr);

            if (deviceInfo)
            {
#ifdef CSR_BT_LE_ENABLE
                /* Makes sure that the attribute handle for the Service 
                   Changed Characteristic Value is set invalid */
                deviceInfo->serviceChangedHandle = CSR_BT_GATT_SERVICE_CHANGED_NOT_FOUND;
#endif
                if (CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED))
                {/* Device is already marked as paired - debond it */
                    
                    CSR_MASK_UNSET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED | SD_DI_DEV_FLAG_TRUSTED);
                    
                    mainInst->dlPairedCount--;

                    if (deviceInfo->infoLen)
                    {/* Adjust info counters */
                        mainInst->dlInfoCount++;
                        mainInst->dlPairedInfoCount--;
                    }
                }
            }
        }
    }
#ifdef CSR_BT_LE_ENABLE
    else if (CSR_MASK_IS_SET(prim->details, SD_UPDATE_SERVICE_CHANGED))
    {
        if (prim->serviceChangeHandle != CSR_BT_GATT_SERVICE_CHANGED_NOT_FOUND)
        {
            SdDeviceInfo_t *deviceInfo = SD_DL_FIND_FROM_ADDR(mainInst->dl, &prim->addr);

            if (deviceInfo)
            {
                if (CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED) &&
                    deviceInfo->serviceChangedHandle != prim->serviceChangeHandle)
                { /* Device is marked as paired and serviceChangedHandle is changed.
                     Keep new value and update request SC to store it in sc db */
                    deviceInfo->serviceChangedHandle = prim->serviceChangeHandle;
                    
                    CsrBtScLeAddServiceChangedReqSend(prim->addr,
                                                      prim->addressType,
                                                      prim->serviceChangeHandle);
                }
            }
        }
        return; /* No need to called CsrBtSdUtilDLTrim */
    }
#endif
    CsrBtSdUtilDLTrim(mainInst);
}

void CsrBtSdSynchronizeReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdSynchronizeReq *prim = (CsrBtSdSynchronizeReq *)mainInst->recvMsgP;

    mainInst->phandle = prim->phandle;
    SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_OTHER);
    SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SC_READ_DEVICE_RECORD);
    CsrBtScReadDeviceRecordReqSend(CSR_BT_SD_IFACEQUEUE, SD_SC_READ_DEVICE_RECORD_BYTES_MAX);
}

/*****Upstream *****/
void CsrBtSdSendReadDeviceInfoCfm(CsrSchedQid phandle, CsrBtDeviceAddr deviceAddr,
                                  SdDeviceInfo_t *deviceInfo, CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier, CsrBtAddressType addressType,
                                  CsrUint16 context)
{
    CsrBtSdReadDeviceInfoCfm *prim = pnew(CsrBtSdReadDeviceInfoCfm);

    prim->type                  = CSR_BT_SD_READ_DEVICE_INFO_CFM;
    prim->deviceStatus         = CsrBtSdUtilGetDeviceStatus(deviceInfo);
    prim->deviceAddr           = deviceAddr;
    prim->resultCode           = resultCode;
    prim->resultSupplier       = resultSupplier;
    prim->addressType          = addressType;
    prim->serviceChangedHandle = 0;
    prim->context              = context; 

    if (deviceInfo)
    {
#ifdef CSR_BT_LE_ENABLE
        prim->serviceChangedHandle = deviceInfo->serviceChangedHandle;
#endif
        prim->deviceClass   = deviceInfo->cod;
        prim->infoLen       = deviceInfo->infoLen;
        prim->info          = (CsrUint8*)(deviceInfo->infoLen > 0
                                          ? CsrMemDup(deviceInfo->info, deviceInfo->infoLen)
                                          : NULL);
    }
    else
    {
        prim->deviceClass   = 0;
        prim->infoLen       = 0;
        prim->info          = NULL;
    }

    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdSendSearchResultInd(CsrSchedQid phandle, CsrBtSdPrim type, SdDeviceInfo_t *deviceInfo)
{
    CsrBtSdSearchResultInd *prim = pnew(CsrBtSdSearchResultInd);

    prim->type          = type;
    prim->deviceAddr    = deviceInfo->addr;
    prim->deviceClass   = deviceInfo->cod;
    prim->deviceStatus  = CsrBtSdUtilGetDeviceStatus(deviceInfo);

#ifdef CSR_BT_LE_ENABLE
    prim->rssi = ((deviceInfo->rssi != CSR_BT_RSSI_INVALID)
                  ? deviceInfo->rssi
                  : deviceInfo->leRssi);
    CsrBtSdUtilCombineTags(deviceInfo,
                           &(prim->infoLen),
                           (CsrUint8**)&(prim->info));
#else
    prim->rssi          = deviceInfo->rssi;
    prim->infoLen       = deviceInfo->infoLen;
    prim->info          = (CsrUint8*)(deviceInfo->infoLen > 0
                                      ? CsrMemDup(deviceInfo->info, deviceInfo->infoLen)
                                      : NULL);
#endif

    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdSendCloseSearchInd(CsrSchedQid phandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSdCloseSearchInd *prim = pnew(CsrBtSdCloseSearchInd);

    prim->type   = CSR_BT_SD_CLOSE_SEARCH_IND;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtSdMessagePut(phandle, prim);
}

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
void CsrBtSdSendReadServiceRecordInd(CsrSchedQid phandle, CsrUint16 dataLen, CsrUint8 *data)
{
    CsrBtSdReadServiceRecordInd    * prim;

    prim                = pnew(CsrBtSdReadServiceRecordInd);
    prim->type          = CSR_BT_SD_READ_SERVICE_RECORD_IND;
    prim->data          = data;
    prim->dataLen       = dataLen;
    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdSendReadServiceRecordCfm(CsrSchedQid phandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSdReadServiceRecordCfm    * prim;

    prim                = pnew(CsrBtSdReadServiceRecordCfm);
    prim->type          = CSR_BT_SD_READ_SERVICE_RECORD_CFM;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdSendRegisterServiceRecordCfm(CsrSchedQid phandle, CsrUint32 serviceHandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtSdRegisterServiceRecordCfm * prim;

    prim                    = pnew(CsrBtSdRegisterServiceRecordCfm);
    prim->type              = CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    prim->serviceHandle     = serviceHandle;
    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdSendUnregisterServiceRecordCfm(CsrSchedQid phandle, CsrUint32 serviceHandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtSdUnregisterServiceRecordCfm * prim;

    prim                    = pnew(CsrBtSdUnregisterServiceRecordCfm);
    prim->type              = CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    prim->serviceHandle     = serviceHandle;
    CsrBtSdMessagePut(phandle, prim);
}
#endif


void CsrBtSdSendCancelReadAvailableServicesCfm(CsrSchedQid phandle)
{
    CsrBtSdCancelReadAvailableServicesCfm    * prim;

    prim                = pnew(CsrBtSdCancelReadAvailableServicesCfm);
    prim->type          = CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM;
    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdSendReadAvailableServicesCfmWithError(CsrSchedQid phandle, CsrBtDeviceAddr deviceAddr)
{
    CsrBtSdReadAvailableServicesCfm    * prim;

    prim                = pnew(CsrBtSdReadAvailableServicesCfm);
    prim->type          = CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM;
    prim->resultSupplier = CSR_BT_SUPPLIER_SD;
    prim->resultCode     = CSR_BT_RESULT_CODE_SD_COMMAND_DISALLOWED;
    prim->deviceAddr    = deviceAddr;
    prim->deviceClass   = 0;
    prim->deviceStatus  = 0;
    prim->infoLen       = 0;
    prim->info          = NULL;
    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdSendReadAvailableServicesCfm(SdMainInstance_t * mainInst, SdDeviceInfo_t *deviceInfo,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtSdReadAvailableServicesCfm *prim = zpnew(CsrBtSdReadAvailableServicesCfm);

    prim->type          = CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM;
    prim->resultCode    = resultCode;
    prim->resultSupplier = resultSupplier;
    if(deviceInfo != NULL)
    {
        prim->deviceAddr    = deviceInfo->addr;
        prim->deviceClass   = deviceInfo->cod;
        prim->deviceStatus  = CsrBtSdUtilGetDeviceStatus(deviceInfo);
        prim->info          = (CsrUint8*)(deviceInfo->infoLen > 0
                                          ? CsrMemDup(deviceInfo->info, deviceInfo->infoLen)
                                          : NULL);
        prim->infoLen       = deviceInfo->infoLen;
    }

    CsrBtSdMessagePut(mainInst->phandle, prim);
}

void CsrBtSdSendReadServicesCfm(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo,
                                CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtSdReadServicesCfm *prim = pnew(CsrBtSdReadServicesCfm);

    prim->type          = CSR_BT_SD_READ_SERVICES_CFM;
    prim->resultCode    = resultCode;
    prim->resultSupplier = resultSupplier;
    prim->deviceAddr    = deviceInfo->addr;
    prim->deviceClass   = deviceInfo->cod;
    prim->deviceStatus  = CsrBtSdUtilGetDeviceStatus(deviceInfo);
    prim->listCount     = deviceInfo->listCount;
    prim->list          = deviceInfo->list;

    /* Ownership of list transferred */
    deviceInfo->listCount = 0;
    deviceInfo->list = NULL;

    CsrBtSdMessagePut(mainInst->phandle, prim);
}


CsrBtSdReadDeviceListInd *CsrBtSdBuildReadDeviceListInd(CsrUint32 count)
{
    CsrBtSdReadDeviceListInd *prim = pnew(CsrBtSdReadDeviceListInd);
    prim->type               = CSR_BT_SD_READ_DEVICE_LIST_IND;
    prim->deviceInfoCount    = count;
    prim->deviceInfo         = (count > 0) ? CsrPmemAlloc(count * sizeof(CsrBtSdDeviceInfoType)) : NULL;
    return prim;
}

CsrBtSdReadDeviceListCfm *CsrBtSdBuildReadDeviceListCfm(CsrUint32 count, CsrUint32 total)
{
    CsrBtSdReadDeviceListCfm *prim = pnew(CsrBtSdReadDeviceListCfm);
    prim->type               = CSR_BT_SD_READ_DEVICE_LIST_CFM;
    prim->deviceInfoCount    = count;
    prim->totalNumOfDevices  = total;
    prim->deviceInfo         = (count > 0) ? CsrPmemAlloc(count * sizeof(CsrBtSdDeviceInfoType)) : NULL;
    return prim;
}

void CsrBtSdSendSynchronizeCfm(SdMainInstance_t *mainInst, CsrUint32 totalNumOfDevices)
{
    CsrBtSdSynchronizeCfm *prim = pnew(CsrBtSdSynchronizeCfm);

    prim->type              = CSR_BT_SD_SYNCHRONIZE_CFM;
    prim->totalNumOfDevices = totalNumOfDevices;

    CsrBtSdMessagePut(mainInst->phandle, prim);
}

#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
void CsrBtSdSendSdMemoryConfigCfm(SdMainInstance_t *mainInst, CsrSchedQid phandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSdMemoryConfigCfm *prim = pnew(CsrBtSdMemoryConfigCfm);

    prim->type              = CSR_BT_SD_MEMORY_CONFIG_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    prim->memoryConfig      = mainInst->memoryConfig;
    prim->deviceListMax     = mainInst->dlMax;
    prim->deviceListInfoMax = mainInst->dlInfoMax;

    CsrBtSdMessagePut(phandle, prim);
}
#endif

#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
void CsrBtSdSendSdSearchConfigCfm(SdSearchInstance_t *searchInst, CsrSchedQid phandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSdSearchConfigCfm *prim = pnew(CsrBtSdSearchConfigCfm);

    prim->type              = CSR_BT_SD_SEARCH_CONFIG_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    if (searchInst)
    {
        prim->searchConfig      = searchInst->searchConfig;
        prim->readNameTimeout   = searchInst->readNameTimeout;
        prim->maxSearchResults  = searchInst->searchResultsMax;
    }
    else
    {
        prim->searchConfig      = 0;
        prim->readNameTimeout   = 0;
        prim->maxSearchResults  = 0;
    }
    CsrBtSdMessagePut(phandle, prim);
}
#endif

void CsrBtSdSendSdCleanUpCfm(CsrSchedQid phandle, CsrUint32 mode, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSdCleanupCfm *prim = pnew(CsrBtSdCleanupCfm);

    prim->type              = CSR_BT_SD_CLEANUP_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    prim->cleanupMode       = mode;

    CsrBtSdMessagePut(phandle, prim);
}

void CsrBtSdReadServicesReqIdleState(SdMainInstance_t *mainInst)
{
    CsrBtSdReadServicesReq *prim = (CsrBtSdReadServicesReq *)mainInst->recvMsgP;
    
    mainInst->phandle       = prim->phandle;
    mainInst->serviceConfig = prim->serviceConfig;
    mainInst->sdpMsgType    = prim->type;
    mainInst->currentDevice = CsrBtSdUtilDLAddFromAddrOnly(mainInst, &prim->deviceAddr);

#ifdef CSR_BT_LE_ENABLE
    /* BR/EDR disabled and not LE disabled - skip directly to LE-GATT
     * search */
    if((mainInst->serviceConfig & CSR_BT_SD_SEARCH_DISABLE_BT_CLASSIC)
       && !(mainInst->serviceConfig & CSR_BT_SD_SEARCH_DISABLE_BT_LE))
    {
        CsrBtSdGattServiceSearchStart(mainInst,
                                      mainInst->currentDevice);
    }
    else
#endif
    {
        CsrBtSdRestartServiceSearchFull(mainInst);
    }
}

void CsrBtSdReadServicesReqInqSave(SdMainInstance_t *mainInst)
{
    CsrBtSdReadServicesReq *p = (CsrBtSdReadServicesReq*)mainInst->recvMsgP;

    if (CsrBtSdSdpQueueHandler(mainInst, p->type, p->phandle, TRUE))
    {
        /* Save the message */
        CsrBtSdUtilSaveMessage(mainInst);
    }
    else
    {
        /* The application has already saved one request, return error */
        CsrBtSdSendReadServicesCfm(mainInst, NULL,
                                   CSR_BT_RESULT_CODE_SD_COMMAND_DISALLOWED,
                                   CSR_BT_SUPPLIER_SD);
        CsrPmemFree(p->filter);
        p->filter = NULL;
    }
}

void CsrBtSdReadServicesCancelReqAllState(SdMainInstance_t *mainInst)
{
    /* TODO: implement */
}

