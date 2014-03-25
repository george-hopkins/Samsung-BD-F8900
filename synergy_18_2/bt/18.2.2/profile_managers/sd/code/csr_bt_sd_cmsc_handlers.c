/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #2 $
***************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"

/***** Environment handlers *****/
void CsrBtSdEnvCleanupHandler(SdMainInstance_t *mainInst)
{
    CsrCleanupInd *prim = (CsrCleanupInd *)mainInst->recvMsgP;
    SdSearchInstance_t *searchInst = SD_SI_FIND_FROM_PHANDLE(mainInst->searchInst, &prim->phandle);

    if (searchInst)
    {/* Handle is known */
        /* Reset all shown bits for current instance */
        SD_DL_ITERATE(mainInst->dl, CsrBtSdUtilSearchInstUnsetInst, &searchInst->searchInstIdx);

        /* Clean DL */
        CsrBtSdUtilDLCleanAll(mainInst);

        /* Remove search instance */
        SD_SI_REMOVE_INST(mainInst->searchInst, searchInst);
    }
}

/***** CM handlers *****/
void CsrBtSdCmInquiryResultIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtCmInquiryResultInd *prim = (CsrBtCmInquiryResultInd *)mainInst->recvMsgP;
    SdDeviceInfo_t *deviceInfo = CsrBtSdUtilDLAddCmInqRes(mainInst, prim);

    CsrBtSdSearchValidateResult(mainInst, deviceInfo);
    CsrPmemFree(prim->eirData); /* EIR data should always be freed */
}

void CsrBtSdCmInquiryCfmHandler(SdMainInstance_t *mainInst)
{
    CsrBtCmInquiryCfm *prim;

    prim = (CsrBtCmInquiryCfm *) mainInst->recvMsgP;

    /* Something low-level went wrong - send SD_CLOSE_IND to relevant instances with error */
    CsrBtSdSearchStopAllSearches(mainInst, prim->resultCode, prim->resultSupplier);
}

/* After remote name has been read (both successfully or not), figure
 * out what the next step is. Note that this function will also get
 * called after name has been retrieved over LE */
void CsrBtSdCmReadRemoteNameContinue(SdMainInstance_t *mainInst)
{
    SD_CANCEL_TIMER(mainInst->rrnTid);

    if (mainInst->appState == SD_STATE_APP_INQ)
    {
        if (mainInst->searchState == SD_STATE_SEARCH_BUFFER_PROC)
        {
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
            CsrBtSdSearchBufferProcess(mainInst);
        }
        else if (mainInst->searchState == SD_STATE_SEARCH_NOT_BUFFERING)
        {
            /* Send result and restart inq */
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
            (void)CsrBtSdSearchValidateNormalResult(mainInst, mainInst->currentDevice);

            if (mainInst->searchState != SD_STATE_SEARCH_INST_IDLE)
            {
                /* A search is still in progress */
                CsrBtSdSearchInquiryStart(mainInst);
            }
        }
        else if (mainInst->processState == SD_STATE_PROC_CRRN_TO_STOP)
        {
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
            CsrBtSdUtilGo2Idle(mainInst);
        }
    }
    else if (mainInst->appState == SD_STATE_APP_OTHER)
    {
        if (mainInst->processState != SD_STATE_PROC_CRRN_TO_STOP)
        {
            /* ReadAvailableServices/ReadServices was not cancelled
             * while reading name so send a confirmation */
            if (mainInst->sdpMsgType == CSR_BT_SD_READ_SERVICES_REQ)
            {
                CsrBtSdSendReadServicesCfm(mainInst,
                                           mainInst->currentDevice,
                                           CSR_BT_RESULT_CODE_SD_SUCCESS,
                                           CSR_BT_SUPPLIER_SD);
            }
            else
            {
                CsrBtSdSendReadAvailableServicesCfm(mainInst,
                                                    mainInst->currentDevice,
                                                    CSR_BT_RESULT_CODE_SD_SUCCESS,
                                                    CSR_BT_SUPPLIER_SD);
            }
        } /* else: A CancelReadAvailableServicesCfm has already been sent */

        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
        CsrBtSdUtilGo2Idle(mainInst);
    }
}

void CsrBtSdCmReadRemoteNameCfmHandler(SdMainInstance_t *mainInst)
{
    CsrBtCmReadRemoteNameCfm *prim = (CsrBtCmReadRemoteNameCfm *)mainInst->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS
        && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* Check and insert name */
        CsrUint8 *name;
        CsrUint16 nameLen = CsrBtSdInfoGetTag(mainInst->currentDevice->infoLen,
                                              mainInst->currentDevice->info,
                                              &name,
                                              CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL);
        CsrUint16 primNameLen = (CsrUint16)CsrUtf8StringLengthInBytes(prim->friendlyName);

        if ((primNameLen != nameLen) ||
            (CsrMemCmp(name, (CsrUint8 *)prim->friendlyName, nameLen)))
        {
            /* Name has been changed - update info and SCDB. Make sure
             * that the CSR_BT_SD_DEVICE_INFO_TYPE_NAME_SHORT is
             * removed */
            CsrBtSdUtilInfoRemoveTag(mainInst,
                                     &mainInst->currentDevice,
                                     CSR_BT_SD_DEVICE_INFO_TYPE_NAME_SHORT);
            
            /* This function calls CsrBtSdUtilInfoRemoveTag again,
             * this makes sure that the
             * CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL is removed before
             * the new name is saved  */               
            CsrBtSdUtilInfoUpdateTag(mainInst,
                             &mainInst->currentDevice,
                             CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL,
                             primNameLen,
                             (CsrUint8 *)prim->friendlyName);

            CSR_MASK_SET(mainInst->currentDevice->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED);
            CsrBtSdUtilUpdateTrustedDeviceList(mainInst->currentDevice);
        }
    }

    CsrBtSdCmReadRemoteNameContinue(mainInst);
    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, prim);
}

void CsrBtSdCmReadRemoteNameIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtCmReadRemoteNameInd *prim = (CsrBtCmReadRemoteNameInd *) mainInst->recvMsgP;
    SdDeviceInfo_t *deviceInfo = CsrBtSdUtilDLAddFromAddrOnly(mainInst, &prim->deviceAddr);
    
    /* Make sure that the CSR_BT_SD_DEVICE_INFO_TYPE_NAME_SHORT is remove */
    CsrBtSdUtilInfoRemoveTag(mainInst, &deviceInfo, CSR_BT_SD_DEVICE_INFO_TYPE_NAME_SHORT);

    /* This function calls CsrBtSdUtilInfoRemoveTag again, this
       makes sure that the CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL is remove
       before the new name is saved                                       */  
    CsrBtSdUtilInfoUpdateTag(mainInst,
                             &deviceInfo,
                             CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL,
                             (CsrUint16)CsrUtf8StringLengthInBytes(prim->friendlyName),
                             (CsrUint8 *)prim->friendlyName);
    CsrBtSdUtilUpdateTrustedDeviceList(deviceInfo);
    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, prim);
}

void CsrBtSdCmSetEventFilterCodCfm(SdMainInstance_t *mainInst)
{
    CsrBtCmSetEventFilterCodCfm *prim = (CsrBtCmSetEventFilterCodCfm *)mainInst->recvMsgP;

    if (prim->resultCode != CSR_BT_RESULT_CODE_CM_SUCCESS || prim->resultSupplier != CSR_BT_SUPPLIER_CM)
    {/* Something went wrong - clear all inquiry filters */
        CsrBtSdSearchClearCodFilter(mainInst);
    }

    if (mainInst->processState == SD_STATE_PROC_SET_EVENT_TO_START)
    {
        CsrBtSdSearchInquiryStartFirst(mainInst);
    }
    else if (mainInst->processState == SD_STATE_PROC_SET_EVENT_TO_STOP)
    {
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
        CsrBtSdUtilGo2Idle(mainInst);
    }
}

void CsrBtSdCmClearEventFilterCfm(SdMainInstance_t *mainInst)
{/* Always ignore */
}

/****** SC handlers *****/
void CsrBtSdScReadDeviceRecordIndHandler(SdMainInstance_t *mainInst)
{
    CsrUintFast32 i;
    CsrUint32 serviceChangedHdl      = 0;
    CsrBtScReadDeviceRecordInd *prim = (CsrBtScReadDeviceRecordInd *)mainInst->recvMsgP;

    for (i = 0; i < prim->devicePropertiesLength; i++)
    {
#ifdef CSR_BT_LE_ENABLE
        if (prim->serviceChangedList)
        {
            serviceChangedHdl = prim->serviceChangedList[i];
        }
#endif 
        CsrBtSdUtilDLAddDevicePropertiesType(mainInst, 
                                             &prim->deviceProperties[i],
                                             serviceChangedHdl);
    }
    CsrPmemFree(prim->serviceChangedList);
    CsrPmemFree(prim->deviceProperties);
}

void CsrBtSdScReadDeviceRecordCfmHandler(SdMainInstance_t *mainInst)
{
    CsrUintFast32 i;
    CsrUint32 serviceChangedHdl      = 0;
    CsrBtScReadDeviceRecordCfm *prim = (CsrBtScReadDeviceRecordCfm *) mainInst->recvMsgP;

    for (i = 0; i < prim->devicePropertiesLength; i++)
    {
#ifdef CSR_BT_LE_ENABLE
        if (prim->serviceChangedList)
        {
            serviceChangedHdl = prim->serviceChangedList[i];
        }
#endif 

        CsrBtSdUtilDLAddDevicePropertiesType(mainInst, 
                                             &prim->deviceProperties[i], 
                                             serviceChangedHdl);
    }

    if (mainInst->processState == SD_STATE_PROC_SC_READ_DEVICE_RECORD)
    {
        if (mainInst->appState == SD_STATE_APP_OTHER)
        {
            /* The Device List and SCDB has been requested to be synchronized from the application */
            CsrBtSdSendSynchronizeCfm(mainInst, prim->totalNumOfDevices);
        }
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
    }

    CsrBtSdUtilGo2Idle(mainInst);
    CsrPmemFree(prim->deviceProperties);
    CsrPmemFree(prim->serviceChangedList);
}

void CsrBtSdScUpdateTrustLevelIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtScUpdateTrustLevelInd *prim = (CsrBtScUpdateTrustLevelInd *)mainInst->recvMsgP;
    SdDeviceInfo_t *deviceInfo = SD_DL_FIND_FROM_ADDR(mainInst->dl, &prim->deviceAddr);

    if (deviceInfo && CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED))
    {
        if (prim->authorised)
        {
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_TRUSTED);
        }
        else
        {
            CSR_MASK_UNSET(deviceInfo->devFlags, SD_DI_DEV_FLAG_TRUSTED);
        }
    }
}

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
void CsrBtSdCmSdsRegisterCfmHandler(SdMainInstance_t *mainInst)
{
    CsrBtCmSdsRegisterCfm *prim = (CsrBtCmSdsRegisterCfm *) mainInst->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The service record is register with success save the service
         record handle restore the local queue and goto idle state */
        CsrBtSdInsertServiceRecordHandle(mainInst, prim->serviceRecHandle);
        CsrBtSdSendRegisterServiceRecordCfm(mainInst->phandle, prim->serviceRecHandle,
            CSR_BT_RESULT_CODE_SD_SUCCESS, CSR_BT_SUPPLIER_SD);
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
        CsrPmemFree(mainInst->sdpRecord);
        mainInst->sdpRecord  = NULL;
        CsrBtSdUtilGo2Idle(mainInst);
    }
    else
    { /* Failed to register the service record */
        mainInst->sdpRetryCounter++;

        if (mainInst->sdpRetryCounter < SD_SDP_ATTEMPT)
        { /* Try again. The reason for trying x number of times is that
             Synergy BT cannot register a new service rocord is a SDP channel is open */
            CsrUint8 *record = CsrMemDup(mainInst->sdpRecord, mainInst->sdpRecordLength);
            CsrBtCmSdsRegisterReqSend(CSR_BT_SD_IFACEQUEUE, record, mainInst->sdpRecordLength);
        }
        else
        { /* Fail to register the service record. Restore the local
             queue and goto idle state */
            CsrBtSdSendRegisterServiceRecordCfm(mainInst->phandle, 0,
                prim->resultCode, prim->resultSupplier);
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
            CsrPmemFree(mainInst->sdpRecord);
            mainInst->sdpRecord = NULL;
            CsrBtSdUtilGo2Idle(mainInst);
        }
    }
}

void CsrBtSdCmSdsUnregisterCfmHandler(SdMainInstance_t *mainInst)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm *) mainInst->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The service record is removed from the SDP layer. Remove the
         service record handle from the local list and restore the local
         queue and goto idle state */
        CsrUint16 index;

        if (CsrBtSdGetServiceRecordHandleIndex(mainInst, prim->serviceRecHandle, &index))
        {
            mainInst->serviceHandleList[index] = 0;
        }

        CsrBtSdSendUnregisterServiceRecordCfm(mainInst->phandle, prim->serviceRecHandle,
            CSR_BT_RESULT_CODE_SD_SUCCESS, CSR_BT_SUPPLIER_SD);
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
        CsrBtSdUtilGo2Idle(mainInst);
    }
    else
    {
        mainInst->sdpRetryCounter++;

        if (mainInst->sdpRetryCounter < SD_SDP_ATTEMPT)
        {
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_SD_IFACEQUEUE, prim->serviceRecHandle);
        }
        else
        {
            CsrBtSdSendUnregisterServiceRecordCfm(mainInst->phandle, prim->serviceRecHandle,
                prim->resultCode, prim->resultSupplier);
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
            CsrBtSdUtilGo2Idle(mainInst);
        }
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */


