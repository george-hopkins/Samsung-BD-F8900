/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
CsrBool CsrBtSdGetServiceRecordHandleIndex(SdMainInstance_t *mainInst, CsrUint32 serviceRecHandle, CsrUint16 *index)
{
    if (mainInst->serviceHandleList)
    {
        CsrUintFast16 i;

        for (i = 0; i < mainInst->serviceHandleListCount; i++)
        {
            if (mainInst->serviceHandleList[i] == serviceRecHandle)
            {
                *index = (CsrUint16) i;
                return TRUE;
            }
        }
    }
    return FALSE;
}

void CsrBtSdInsertServiceRecordHandle(SdMainInstance_t *mainInst, CsrUint32 serviceRecHandle)
{
    CsrUint16 index;

    if (CsrBtSdGetServiceRecordHandleIndex(mainInst, CMN_SDR_INVALID_SERVICE_HANDLE, &index))
    {
        mainInst->serviceHandleList[index] = serviceRecHandle;
    }
    else
    {
        CsrUint32 *tmpList = mainInst->serviceHandleList;
        CsrUint16 tmpSize  = mainInst->serviceHandleListCount;

        mainInst->serviceHandleListCount += SD_POOL_SIZE_FOR_SERVICE_RECORD_HANDLES;
        mainInst->serviceHandleList = CsrPmemZalloc(mainInst->serviceHandleListCount * sizeof(CsrUint32));

        if (tmpList)
        {
            CsrMemCpy(mainInst->serviceHandleList, tmpList, tmpSize * sizeof(CsrUint32));
            CsrPmemFree(tmpList);
        }

        mainInst->serviceHandleList[tmpSize] = serviceRecHandle;
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */

static void csrBtSdSdpRemoveOldServices(SdMainInstance_t *mainInst, SdDeviceInfo_t **deviceInfo)
{
    CsrBtSdUtilInfoRemoveTag(mainInst, deviceInfo, CSR_BT_SD_DEVICE_INFO_TYPE_UUID16_INCOMPLETE);
    CsrBtSdUtilInfoRemoveTag(mainInst, deviceInfo, CSR_BT_SD_DEVICE_INFO_TYPE_UUID16_COMPLETE);
    CsrBtSdUtilInfoRemoveTag(mainInst, deviceInfo, CSR_BT_SD_DEVICE_INFO_TYPE_UUID32_INCOMPLETE);
    CsrBtSdUtilInfoRemoveTag(mainInst, deviceInfo, CSR_BT_SD_DEVICE_INFO_TYPE_UUID32_COMPLETE);
    CsrBtSdUtilInfoRemoveTag(mainInst, deviceInfo, CSR_BT_SD_DEVICE_INFO_TYPE_UUID128_INCOMPLETE);
    CsrBtSdUtilInfoRemoveTag(mainInst, deviceInfo, CSR_BT_SD_DEVICE_INFO_TYPE_UUID128_COMPLETE);
}

static void csrBtSdSdpRemoveDuplicateUuids(CmnCsrBtLinkedListStruct *sdpTagList)
{
    CsrBtUuid32    service, service2;
    CsrIntFast16 i;
    CsrUintFast16 i2;
    CsrUint16     bchsResult, bchsTask, bchsTaskResult;
    CsrUintFast16    entries = CsrBtUtilBllGetNofEntriesEx(sdpTagList);

    /* Determine the number of supported services */
    for (i = 0; (CsrUintFast16)i < entries; i++)
    {
        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, (CsrUintFast16)i, &service,
                            &bchsResult, &bchsTask, &bchsTaskResult))
        {
            for (i2 = i + 1; i2 < entries; i2++)
            {
                if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, i2, &service2,
                            &bchsResult, &bchsTask, &bchsTaskResult))
                {
                    if (service == service2)
                    {
                        CsrBtUtilBllFreeLinkedListEntryEx(sdpTagList, i2, CsrBtUtilBllPfreeWrapper);
                        entries = CsrBtUtilBllGetNofEntriesEx(sdpTagList);

                        /* Restart the search for duplicates */
                        i = -1; /* Will be increased by 1 when restarting the outer loop */
                        break;
                    }
                }
            }
        }
    }

}

static void csrBtSdSdpExtractServiceClasses(SdMainInstance_t *mainInst,
                                            CmnCsrBtLinkedListStruct *sdpTagList)                               
{
    CsrBtUuid32 service;
    CsrUintFast16 entries, index;
    CsrUint16 result, task, taskResult;
    CsrUint16 attrCount;

    /* Clear existing service list */
    mainInst->currentDevice->listCount = 0;
    CsrPmemFree(mainInst->currentDevice->list);
    mainInst->currentDevice->list = NULL;

    attrCount = 0;
    entries = CsrBtUtilBllGetNofEntriesEx(sdpTagList);
            
    /* Count number of attributes in each service */
    for(index=0; index<entries; index++)
    {
        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, index, &service,
                                                  &result, &task, &taskResult)
            && (result == SDR_SDC_SEARCH_SUCCESS))
        {
            CsrUint16 count = 0;
            CsrUint32 *list = NULL;
            if(CsrBtUtilSdrGetServiceClassIdList(sdpTagList, index,
                                                 &count,
                                                 &list))
            {
                attrCount += count;
                CsrPmemFree(list);
            }
        }
    }

    /* Copy all attributes from each service SDR */
    if(attrCount > 0)
    {
        /* Note; attrCount is a 'worst case' number of service class
         * IDs.  the actual number may be smaller as we filter
         * duplicates later on */
        CsrBtSdResizeDiList(mainInst->currentDevice, attrCount);

        for(index=0; index<entries; index++)
        {
            if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, index, &service,
                                                      &result, &task, &taskResult)
                && (result == SDR_SDC_SEARCH_SUCCESS))
            {
                CsrUint16 count = 0;
                CsrUint32 *list = NULL;

                if(CsrBtUtilSdrGetServiceClassIdList(sdpTagList, index,
                                                     &count,
                                                     &list))
                {
                    /* Store service uuid if not already there */
                    for(; count>0; count--)
                    {
                        CsrUint16 j;
                        CsrBool add;
                        CsrBtUuid elm;

                        /* Convert sloppy 32bit UUID in to proper UUID container */
                        elm.length = CSR_BT_UUID32_SIZE;
                        CsrMemSet(elm.uuid, 0, CSR_BT_UUID128_SIZE);
                        elm.uuid[0] = (CsrUint8)(list[count-1] & 0xFF);
                        elm.uuid[1] = (CsrUint8)((list[count-1] >> 8) & 0xFF);
                        elm.uuid[2] = (CsrUint8)((list[count-1] >> 16) & 0xFF);
                        elm.uuid[3] = (CsrUint8)((list[count-1] >> 24) & 0xFF);
                                                
                        for(add = TRUE, j=0;
                            j<mainInst->currentDevice->listCount;
                            j++)
                        {
                            if(CsrBtSdUuidEqual(&elm, &(mainInst->currentDevice->list[j])))
                            {
                                add = FALSE;
                            }
                        }
                        
                        if(add)
                        {
                            CsrMemCpy(&(mainInst->currentDevice->list[mainInst->currentDevice->listCount]),
                                      &elm,
                                      sizeof(CsrBtUuid));
                            mainInst->currentDevice->listCount++;
                        }
                    }
                    CsrPmemFree(list);
                }
            }        
        }
    }
}

static void csrBtSdSdpAddServices(SdMainInstance_t *mainInst, CmnCsrBtLinkedListStruct *sdpTagList)
{
    CsrBtUuid32    service;
    CsrUint16    bchsResult, bchsTask, bchsTaskResult, serviceCount = 0, tmpTagLen;
    CsrUint8     *tmpTagValue;
    CsrUintFast16    i, entries = CsrBtUtilBllGetNofEntriesEx(sdpTagList);

    /* Remove duplicates */
    csrBtSdSdpRemoveDuplicateUuids(sdpTagList);

    /* Determine the number of supported services */
    for (i = 0; i < entries; i++)
    {
        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, i, &service,
                            &bchsResult, &bchsTask, &bchsTaskResult))
        {
            if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
            {
                serviceCount++;
            }
        }
    }

    if (serviceCount)
    {
        /* Insert the supported services */
        tmpTagLen = serviceCount * sizeof(CsrBtUuid32);
        tmpTagValue = CsrPmemAlloc(tmpTagLen);
        i = 0;

        while (i < serviceCount)
        {
            if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, i, &service,
                                &bchsResult, &bchsTask, &bchsTaskResult))
            {
                if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
                {
                    /* Insert services */
                    CSR_COPY_UINT32_TO_LITTLE_ENDIAN(service, &tmpTagValue[i * sizeof(service)]);
                    i++;
                }
            }
        }

        /* Remove old services */
        csrBtSdSdpRemoveOldServices(mainInst, &mainInst->currentDevice);

        /* Add new services */
        CsrBtSdUtilInfoUpdateTag(mainInst, &mainInst->currentDevice, CSR_BT_SD_DEVICE_INFO_TYPE_UUID32_COMPLETE, tmpTagLen, tmpTagValue);
        CsrBtSdUtilUpdateTrustedDeviceList(mainInst->currentDevice);
        CsrPmemFree(tmpTagValue);
    }
}

CsrBool CsrBtSdSdpQueueHandler(SdMainInstance_t * mainInst, CsrBtSdPrim type, CsrSchedQid phandle, CsrBool save)
{
    void                    *msg;
    CsrUint16                eventClass;
    CsrMessageQueueType    *tempQueue  = NULL;
    CsrBool                  accept = save;

    while(CsrMessageQueuePop(&mainInst->saveQueue, &eventClass, &msg))
    {
        if ((eventClass == CSR_BT_SD_PRIM) && (type == (*((CsrBtSdPrim *) msg))))
        {
            switch (type)
            {
                case CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ:
                {
                    CsrBtSdReadAvailableServicesReq * prim;
                    prim = (CsrBtSdReadAvailableServicesReq *) msg;

                    if (prim->phandle == phandle)
                    {
                        /* The application has stored this message */
                        if (save)
                        {
                            /* Has requested to saved the message */
                            accept = FALSE;
                        }
                        else
                        {
                            /* Has requested to remove the message */
                            accept = TRUE;
                            CsrPmemFree(msg);
                        }
                    }
                    else
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    }
                    break;
                }

                case CSR_BT_SD_READ_SERVICES_REQ:
                {
                    CsrBtSdReadServicesReq * prim;
                    prim = (CsrBtSdReadServicesReq *) msg;

                    if (prim->phandle == phandle)
                    {
                        /* The application has stored this message */
                        if (save)
                        {
                            /* Has requested to saved the message */
                            accept = FALSE;
                        }
                        else
                        {
                            /* Has requested to remove the message */
                            accept = TRUE;
                            CsrPmemFree(msg);
                        }
                    }
                    else
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    }
                    break;
                }

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                case CSR_BT_SD_READ_SERVICE_RECORD_REQ:
                {
                    CsrBtSdReadServiceRecordReq * prim;
                    prim = (CsrBtSdReadServiceRecordReq *) msg;

                    if (prim->phandle == phandle)
                    {
                        /* The application has stored this message */
                        if (save)
                        {
                            /* Has requested to saved the message */
                            accept = FALSE;
                        }
                        else
                        {
                            /* Has requested to remove the message */
                            CsrPmemFree(prim->data);
                            accept     = TRUE;
                            CsrPmemFree(msg);
                        }
                    }
                    else
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    }
                    break;
                }
                case CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ:
                {
                    CsrBtSdRegisterServiceRecordReq * prim;

                    prim = (CsrBtSdRegisterServiceRecordReq *) msg;

                    if (prim->phandle == phandle)
                    {
                        /* The application has stored this message.
                         * This message can only be saved */
                        accept = FALSE;
                    }
                    else
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    }
                    break;
                }
                case CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ:
                {
                    CsrBtSdUnregisterServiceRecordReq * prim;

                    prim = (CsrBtSdUnregisterServiceRecordReq *) msg;

                    if (prim->phandle == phandle)
                    {
                        /* The application has stored this message.
                         * This message can only be saved */
                        accept = FALSE;
                    }
                    else
                    {
                        CsrMessageQueuePush(&tempQueue, eventClass, msg);
                    }
                    break;
                }
#endif /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
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
    mainInst->saveQueue = tempQueue;
    return (accept);
}

void CsrBtSdRestartServiceSearch(SdMainInstance_t * mainInst)
{
    CsrUint16 i, newIndex;
    CmnCsrBtLinkedListStruct *sdpTag = NULL;

    SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDC_SEARCH);
    SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_OTHER);

    for (i = 0; i < mainInst->uuid32ListCount; i++)
    {
        sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag,
                                                      mainInst->uuid32List[i],
                                                      &newIndex);
    }

    CsrBtUtilSdcSearchStart(mainInst, mainInst->sdpSearchData, sdpTag, mainInst->currentDevice->addr);
}

void CsrBtSdRestartServiceSearchFull(SdMainInstance_t * mainInst)
{
    CsrUint16 index;
    CmnCsrBtLinkedListStruct *sdpTag = NULL;

    SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDC_SEARCH);
    SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_OTHER);

    /* Full service search. Look for L2CAP protocol and the service
     * class ID list */
    sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_L2CAP_PROTOCOL_UUID16_IDENTIFIER, &index);
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SERVICE_CLASS_ID_LIST, NULL, 0);
    CsrBtUtilSdrInsertPerformExtendedSearch(sdpTag, index);

    CsrBtUtilSdcSearchStart(mainInst, mainInst->sdpSearchData,
                            sdpTag, mainInst->currentDevice->addr);
}

void CsrBtSdReadSdpResultHandler(void *inst,
                                 CmnCsrBtLinkedListStruct *sdpTagList,
                                 CsrBtDeviceAddr deviceAddr,
                                 CsrBtResultCode resultCode,
                                 CsrBtSupplier resultSupplier)
{
    SdMainInstance_t *mainInst = (SdMainInstance_t *)inst;

    switch (mainInst->processState)
    {
        case SD_STATE_PROC_SDC_SEARCH:
        {
            if (resultSupplier == CSR_BT_SUPPLIER_CM &&
                resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && sdpTagList)
            {
                if(mainInst->sdpMsgType == CSR_BT_SD_READ_SERVICES_REQ)
                {
                    csrBtSdSdpExtractServiceClasses(mainInst, sdpTagList);

                    if (CSR_MASK_IS_SET(mainInst->serviceConfig, CSR_BT_SD_SERVICE_NO_NAME_UPDATE))
                    {
                        /* No need to update name, inform the application */
                        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
                        CsrBtSdSendReadServicesCfm(mainInst,
                                                   mainInst->currentDevice,
                                                   CSR_BT_RESULT_CODE_SD_SUCCESS,
                                                   CSR_BT_SUPPLIER_SD);
                        CsrBtSdUtilGo2Idle(mainInst);
                    }
                    else
                    {
                        /* Still need to update the remote name */
                        (void)CsrBtSdUtilReadRemoteNameStart(mainInst, mainInst->currentDevice);
                    }
                }
                else if (mainInst->sdpMsgType == CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ)
                {
                    csrBtSdSdpAddServices(mainInst, sdpTagList);

                    if (mainInst->currentDevice)
                    {
                        if (CSR_MASK_IS_SET(mainInst->serviceConfig, CSR_BT_SD_SERVICE_NO_NAME_UPDATE))
                        {
                            /* No need to update name, inform the application */
                            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
                            CsrBtSdSendReadAvailableServicesCfm(mainInst,
                                                                mainInst->currentDevice,
                                                                CSR_BT_RESULT_CODE_SD_SUCCESS,
                                                                CSR_BT_SUPPLIER_SD);
                            CsrBtSdUtilGo2Idle(mainInst);
                        }
                        else
                        {
                            /* Still need to update the remote name */
                            (void)CsrBtSdUtilReadRemoteNameStart(mainInst, mainInst->currentDevice);
                        }
                    }
                }
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                else
                {
                    /* Must be CSR_BT_SD_READ_SERVICE_RECORD_REQ */
                    CsrUint8 *tmpPtr;
                    CsrUint16 dataLen;
                    CsrUintFast16 i, entries = CsrBtUtilBllGetNofEntriesEx(sdpTagList);
                    SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);

                    for (i = 0; i < entries; i++)
                    {
                        tmpPtr = CsrBtUtilBllGetDataPointerEx(sdpTagList, i, &dataLen);
                        CsrBtSdSendReadServiceRecordInd(mainInst->phandle, dataLen, tmpPtr);
                        CsrBtUtilBllSetDataPointerEx(sdpTagList, i, NULL, 0);
                    }

                    CsrBtSdSendReadServiceRecordCfm(mainInst->phandle,
                                                    CSR_BT_RESULT_CODE_SD_SUCCESS,
                                                    CSR_BT_SUPPLIER_SD);
                    CsrBtSdUtilGo2Idle(mainInst);
                }
#endif /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
            }
            else
            {
                /* The procedure is finished */
                SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);

                if (resultSupplier == CSR_BT_SUPPLIER_CM &&
                    resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                {
                    resultCode = SDC_NO_RESPONSE_DATA;
                    resultSupplier = CSR_BT_SUPPLIER_SDP_SDC;
                }

                if (mainInst->sdpMsgType == CSR_BT_SD_READ_SERVICES_REQ)
                {
                    CsrBtSdSendReadServicesCfm(mainInst,
                                               mainInst->currentDevice,
                                               resultCode, resultSupplier);
                }
                else if (mainInst->sdpMsgType == CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ)
                {
                    /* The CsrBtSdReadAvailableServicesReq failed */
                    CsrBtSdSendReadAvailableServicesCfm(mainInst, mainInst->currentDevice,
                                                        resultCode, resultSupplier);
                }
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                else
                {
                    /* The CsrBtSdReadServiceRecordReq failed */
                    CsrBtSdSendReadServiceRecordCfm(mainInst->phandle, resultCode, resultSupplier);
                }
#endif /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
                CsrBtSdUtilGo2Idle(mainInst);
            }
            break;
        }
        default:
        {
            /* processState must be SD_STATE_PROC_SDC_SEARCH_CANCEL.
             * The CsrBtSdReadAvailableServicesReq or
             * SdReadServiceRecordReq is finish restore the
             * queue. Note that the application already has been
             * informed */
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
            CsrBtSdUtilGo2Idle(mainInst);
            break;
        }
    }
    CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
}


