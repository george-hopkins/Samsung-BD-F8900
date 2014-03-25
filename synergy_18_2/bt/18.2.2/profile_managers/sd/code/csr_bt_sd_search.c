/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"

void CsrBtSdSearchTimeoutHandler(CsrUint16 dummy, void *ptr)
{
    SdSearchInstance_t *searchInst = (SdSearchInstance_t *)ptr;

    searchInst->searchTid = 0;
    CsrBtSdSearchCompleteHandler(searchInst, CSR_BT_RESULT_CODE_SD_TIMEOUT,
        CSR_BT_SUPPLIER_SD);
}

void CsrBtSdSearchRssiBufferTimeoutHandler(CsrUint16 dummy, void *ptr)
{
    SdMainInstance_t *mainInst = (SdMainInstance_t *)ptr;

    mainInst->searchBufferTid = 0;

    /* Inquiry will be cancelled later if required */

    /* Sort and process buffer */
    SD_CHANGE_STATE(mainInst->searchState, SD_STATE_SEARCH_BUFFER_PROC);
    CsrCmnListSort(&mainInst->dl, CsrBtSdUtilSortRssi);
    CsrBtSdSearchBufferProcess(mainInst);
}

void CsrBtSdSearchSendAllResultsToInst(CsrCmnListElm_t *elem, void *data)
{
    SdMainInstance_t *mainInst = (SdMainInstance_t *)data;

    CsrBtSdSearchValidateImmResult(mainInst, (SdDeviceInfo_t *)elem);
    (void)CsrBtSdSearchValidateNormalResult(mainInst, (SdDeviceInfo_t *)elem);
}

void CsrBtSdSearchStopAllSearches(SdMainInstance_t *mainInst, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    SdSearchInstance_t *searchInst;

    for (searchInst = SD_SI_GET_FIRST(mainInst->searchInst); searchInst; searchInst = searchInst->next)
    {
        if (searchInst->state == SD_STATE_SEARCH_INST_SEARCH)
        {
            CsrBtSdSearchCompleteHandler(searchInst, resultCode, resultSupplier);
        }
    }
}

void CsrBtSdSearchBufferStart(SdMainInstance_t *mainInst, CsrUint32 rssiBufferTime)
{
    SD_CHANGE_STATE(mainInst->searchState, SD_STATE_SEARCH_BUFFERING);
    mainInst->searchBufferTid = CsrSchedTimerSet(rssiBufferTime * 1000,
                                                 CsrBtSdSearchRssiBufferTimeoutHandler,
                                                 0,
                                                 (void *)mainInst);
    CsrBtSdSearchInquiryStart(mainInst);
}

void CsrBtSdSearchBufferProcess(SdMainInstance_t *mainInst)
{
    SdDeviceInfo_t *deviceInfo;

    for (deviceInfo = SD_DL_GET_FIRST(mainInst->dl); deviceInfo; deviceInfo = deviceInfo->next)
    {
        if(!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_DONE) &&
            CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_FOUND) &&
            CsrBtSdSearchValidateNormalResult(mainInst, deviceInfo))
        {
            /* RRN is required */
            CsrBtSdSearchInquiryStop(mainInst); /* Search will only be stopped if it is active */
            if (CsrBtSdUtilReadRemoteNameStart(mainInst, deviceInfo))
            {
                return;
            }
        }
    }

    if (mainInst->appState == SD_STATE_APP_INQ)
    {
        SD_CHANGE_STATE(mainInst->searchState, SD_STATE_SEARCH_NOT_BUFFERING);

        if (CsrBtSdUtilCheckSearchInstConfig(mainInst, CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, TRUE, FALSE))
        {
            /* At least one instance requires search to continue after RSSI sorting */
            if (mainInst->processState != SD_STATE_PROC_INQ)
            {
                CsrBtSdSearchInquiryStart(mainInst);
            }
        }
        else
        {
            /* Search is complete */
            CsrBtSdSearchStopAllSearches(mainInst, CSR_BT_RESULT_CODE_SD_RSSI_BUFFER_EMPTY, CSR_BT_SUPPLIER_SD);
        }
    }
}

static void csrBtSdSearchBufferStop(SdMainInstance_t *mainInst)
{
    SD_CANCEL_TIMER(mainInst->searchBufferTid);
}

static CsrBool csrBtSdSearchIsResultOk(SdSearchInstance_t *searchInst, SdDeviceInfo_t *deviceInfo,
                                       CsrBool nameAvailable, CsrUint8 resType)
{
    if (/* Instance is searching */
        (searchInst->state == SD_STATE_SEARCH_INST_SEARCH)
        /* Result has not been shown */
        && !CSR_BIT_IS_SET(deviceInfo->instFlags[resType], searchInst->searchInstIdx)
        /* CoD matches */
        && (SD_COMPARE_COD(deviceInfo->cod, searchInst->cod, searchInst->codMask))
        /* The available name is acceptable (names not required for immediate results) */
        && (nameAvailable
            || (!nameAvailable
                && CSR_MASK_IS_SET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES)))
        /* Non-paired device or paired results allowed */
        && (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED)
            || !CSR_MASK_IS_SET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_HIDE_PAIRED_DEVICES))

        /* RSSI is within threshold */
        && (deviceInfo->rssi >= searchInst->rssiThreshold)
#ifdef CSR_BT_LE_ENABLE
        /* BR/EDR inquiry disabled */
        && CSR_MASK_IS_UNSET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_DISABLE_BT_CLASSIC)
#endif
        )
    {
        return TRUE;
    }

    return FALSE;
}

#ifdef CSR_BT_LE_ENABLE
static CsrBool csrBtSdSearchIsReportOk(SdSearchInstance_t *searchInst, SdDeviceInfo_t *deviceInfo,
                                       CsrBool nameAvailable, CsrUint8 resType)
{
    if (/* Instance is searching */
        (searchInst->state == SD_STATE_SEARCH_INST_SEARCH)
        /* Result has not been shown */
        && !CSR_BIT_IS_SET(deviceInfo->instFlags[resType], searchInst->searchInstIdx)
        /* The available name is acceptable (names not required for immediate results) */
        && (nameAvailable
         || (!nameAvailable
             && CSR_MASK_IS_SET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES)))
        /* Non-paired device or paired results allowed */
        && (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED)
            || !CSR_MASK_IS_SET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_HIDE_PAIRED_DEVICES))
        /* RSSI is within threshold */
        && (deviceInfo->leRssi >= searchInst->leRssiThreshold)
        /* LE scan disabled */
        && CSR_MASK_IS_UNSET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_DISABLE_BT_LE))
    {
        return TRUE;
    }

    return FALSE;
}
#endif

CsrBool CsrBtSdSearchValidateNormalResult(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo)
{
    /* Will return TRUE if a remote name is required */
    SdSearchInstance_t *searchInst;

    /* At least one search instance needs a normal result */
    if (mainInst->activeInstFlags[SD_DI_INST_IDX_NORMAL] != 0)
    {
        CsrBool nameAvailable;

        if (CsrBtSdUtilIsUpdatedNameRequired(mainInst, deviceInfo, &nameAvailable))
        {
            /* At least one search instance requires an updated name and one is not available */
            return TRUE; /* RRN is required */
        }
        else
        {
            /* Name is available - send results to matching search instances */
            for (searchInst = SD_SI_GET_FIRST(mainInst->searchInst); searchInst; searchInst = searchInst->next)
            {
                CsrBool notify = FALSE;

                if (csrBtSdSearchIsResultOk(searchInst, deviceInfo, nameAvailable, SD_DI_INST_IDX_NORMAL))
                {
                    notify = TRUE;
                    CSR_BIT_SET(deviceInfo->instFlags[SD_DI_INST_IDX_NORMAL], searchInst->searchInstIdx);
                }
#ifdef CSR_BT_LE_ENABLE
                if((deviceInfo->advertLen != 0)
                   && csrBtSdSearchIsReportOk(searchInst, deviceInfo, nameAvailable, SD_DI_INST_IDX_ADVERT))
                {
                    notify = TRUE;
                    CSR_BIT_SET(deviceInfo->instFlags[SD_DI_INST_IDX_ADVERT], searchInst->searchInstIdx);
                }
                if((deviceInfo->scanrspLen != 0)
                   && csrBtSdSearchIsReportOk(searchInst, deviceInfo, nameAvailable, SD_DI_INST_IDX_SCANRSP))
                {
                    notify = TRUE;
                    CSR_BIT_SET(deviceInfo->instFlags[SD_DI_INST_IDX_SCANRSP], searchInst->searchInstIdx);
                }
#endif

                /* Send result to upper layer */
                if(notify)
                {
                    CsrBtSdSendSearchResultInd(searchInst->phandle, CSR_BT_SD_SEARCH_RESULT_IND, deviceInfo);
                    if ((searchInst->searchResultsMax != CSR_BT_UNLIMITED) &&
                        (++searchInst->searchResultsCount == searchInst->searchResultsMax))
                    {
                        /* The maximum number of devices have been found */
                        CsrBtSdSearchCompleteHandler(searchInst,
                                                     CSR_BT_RESULT_CODE_SD_MAX_RESULTS,
                                                     CSR_BT_SUPPLIER_SD);
                    }
                }
            }
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_DONE);
        }
    }
    return FALSE; /* RRN is not required */
}

void CsrBtSdSearchValidateImmResult(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo)
{
    if (mainInst->activeInstFlags[SD_DI_INST_IDX_IMM] != 0)
    {      
        /* One or more search instances requires an immediate result indication */
        SdSearchInstance_t *searchInst;

        for (searchInst = SD_SI_GET_FIRST(mainInst->searchInst); searchInst; searchInst = searchInst->next)
        {
            if (csrBtSdSearchIsResultOk(searchInst, deviceInfo, TRUE, SD_DI_INST_IDX_IMM)
                && !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NO_CLASSIC))
            {
                /* Send immediate result to app and update flag */
                CsrBtSdSendSearchResultInd(searchInst->phandle, CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND, deviceInfo);
                CSR_BIT_SET(deviceInfo->instFlags[SD_DI_INST_IDX_IMM], searchInst->searchInstIdx);
            }

#ifdef CSR_BT_LE_ENABLE
            /* Notify about advertisement report? */
            if ((deviceInfo->advertLen != 0)
                && CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_ADVERT)
                && csrBtSdSearchIsReportOk(searchInst, deviceInfo, TRUE, SD_DI_INST_IDX_ADVERT))
            {
                /* Send immediate result to app and update flag */
                CsrBtSdSendSearchResultInd(searchInst->phandle, CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND, deviceInfo);
                CSR_BIT_SET(deviceInfo->instFlags[SD_DI_INST_IDX_ADVERT], searchInst->searchInstIdx);
            }
            
            /* Notify about scan-responses? */
            if ((deviceInfo->scanrspLen != 0)
                && CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SCANRSP)
                && csrBtSdSearchIsReportOk(searchInst, deviceInfo, TRUE, SD_DI_INST_IDX_SCANRSP))
            {
                /* Send immediate result to app and update flag */
                CsrBtSdSendSearchResultInd(searchInst->phandle, CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND, deviceInfo);
                CSR_BIT_SET(deviceInfo->instFlags[SD_DI_INST_IDX_SCANRSP], searchInst->searchInstIdx);
            }
#endif
        }
    }
}


void CsrBtSdSearchValidateResult(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo)
{
    CsrBtSdSearchValidateImmResult(mainInst, deviceInfo);

    if (mainInst->searchState == SD_STATE_SEARCH_NOT_BUFFERING)
    {
        /* Get name or just send result */
        if (CsrBtSdSearchValidateNormalResult(mainInst, deviceInfo))
        {
            /* Remote name is required */
            CsrBtSdSearchInquiryStop(mainInst);

            if (mainInst->processState == SD_STATE_PROC_IDLE)
            {
                (void)CsrBtSdUtilReadRemoteNameStart(mainInst, deviceInfo);
            }
        }
    }
}

CsrBool CsrBtSdSearchSetupParameters(SdMainInstance_t *mainInst,
                                     SdSearchInstance_t *searchInst,
                                     CsrBtSdSearchReq *prim)
{
    if( ((prim->totalSearchTime == 0) ||
         (prim->rssiBufferTime < prim->totalSearchTime))
        /* must not disable both radios */
        && !((prim->searchConfig & CSR_BT_SD_SEARCH_DISABLE_BT_LE) &&
             (prim->searchConfig & CSR_BT_SD_SEARCH_DISABLE_BT_CLASSIC)) )
    {
        /* Parameters makes sense */
        searchInst->phandle             = prim->phandle;
        searchInst->activeSearchConfig  = (prim->searchConfig == CSR_BT_SD_SEARCH_USE_PRECONFIGURED
                                           ? searchInst->searchConfig
                                           : prim->searchConfig);
        searchInst->cod                 = prim->deviceClass;
        searchInst->codMask             = prim->deviceClassMask;
        searchInst->totalSearchTime     = prim->totalSearchTime;
        searchInst->rssiBufferTime      = prim->rssiBufferTime;
        searchInst->rssiThreshold       = prim->rssiThreshold;
#ifdef CSR_BT_LE_ENABLE
        searchInst->leRssiThreshold       = prim->leRssiThreshold;
#endif
        searchInst->iac                 = prim->inquiryAccessCode;
        searchInst->searchResultsCount  = 0;
        searchInst->searchTid           = (prim->totalSearchTime
                                           ? CsrSchedTimerSet(prim->totalSearchTime * 1000,
                                                              CsrBtSdSearchTimeoutHandler, 0, (void *)searchInst)
                                           : 0);
        
        if (CSR_MASK_IS_SET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_ENABLE_IMM_RESULTS))
        {
            /* Enable immediate search results */
            CSR_BIT_SET(mainInst->activeInstFlags[SD_DI_INST_IDX_IMM], searchInst->searchInstIdx);
        }

        if (!CSR_MASK_IS_SET(searchInst->activeSearchConfig, CSR_BT_SD_SEARCH_DO_NOT_CLEAR_FILTER))
        {
            /* Clean the corresponding "normal" and "imm" flags for all devices */
            SD_DL_ITERATE(mainInst->dl, CsrBtSdUtilSearchInstUnsetInst, &searchInst->searchInstIdx);
        }

        CSR_BIT_SET(mainInst->activeInstFlags[SD_DI_INST_IDX_NORMAL], searchInst->searchInstIdx);
        SD_CHANGE_STATE(searchInst->state, SD_STATE_SEARCH_INST_SEARCH);
        CsrBtSdSearchSetCodFilter(searchInst);
        mainInst->nameUpdateRequired = CsrBtSdUtilCheckSearchInstConfig(mainInst,
                                                                        CSR_BT_SD_SEARCH_FORCE_NAME_UPDATE,
                                                                        TRUE,
                                                                        FALSE); /* At least one instance requires updated names */
        CsrBtSdUtilReadRemoteNameTimeUpdate(mainInst);
        return TRUE;
    }
    else
    {
        /* Remove the search instance again */
        SD_SI_REMOVE_INST(mainInst->searchInst, searchInst);
        return FALSE;
    }
}

void CsrBtSdSearchSetCodFilter(SdSearchInstance_t *searchInst)
{
    if (searchInst->mainInst->processState == SD_STATE_PROC_IDLE)
    {
        if (searchInst->cod != 0 || searchInst->codMask != 0)
        {
            /* Just send the filter - if it fails to set, all filters will be removed */
            CsrBtCmSetEventFilterCodReqSend(CSR_BT_SD_IFACEQUEUE,
                                       TRUE,
                                       0,
                                       searchInst->cod,
                                       searchInst->codMask);

            SD_CHANGE_STATE(searchInst->mainInst->processState, SD_STATE_PROC_SET_EVENT_TO_START);
        }
    }
    else
    {/* Another process is currently running - make sure to remove the filters */
        CsrBtSdSearchClearCodFilter(searchInst->mainInst);
    }
}

void CsrBtSdSearchClearCodFilter(SdMainInstance_t *mainInst)
{
    CsrBtCmClearEventFilterReqSend(CSR_BT_SD_IFACEQUEUE, INQUIRY_RESULT_FILTER);
}

void CsrBtSdSearchInquiryStartFirst(SdMainInstance_t *mainInst)
{
    /* Determine if RSSI sorting is requested */
    SdSearchInstance_t *searchInst;

    for (searchInst = SD_SI_GET_FIRST(mainInst->searchInst); searchInst; searchInst = searchInst->next)
    {
        if ((searchInst->state == SD_STATE_SEARCH_INST_SEARCH) &&
            (searchInst->rssiBufferTime != CSR_BT_SD_SEARCH_DISABLE_BUFFERING))
        {
            /* RSSI search is requested */
            CsrBtSdSearchBufferStart(mainInst, searchInst->rssiBufferTime);
            return;
        }
    }

    /* RSSI search is not requested */
    SD_CHANGE_STATE(mainInst->searchState, SD_STATE_SEARCH_NOT_BUFFERING);
    CsrBtSdSearchInquiryStart(mainInst);
}

static void csrBtSdSetInquiryAccessCode(CsrCmnListElm_t *elem, void *value)
{
    CsrUint24 inquiryAccessCode = *(CsrUint24 *) value;

    if (inquiryAccessCode == CSR_BT_SD_ACCESS_CODE_GIAC)
    {
        ;
    }
    else
    {
        SdSearchInstance_t * element = (SdSearchInstance_t *) elem;

        if (element->iac == CSR_BT_SD_ACCESS_CODE_GIAC)
        {
            *(CsrUint24 *) value = CSR_BT_SD_ACCESS_CODE_GIAC;
        }
        else if (element->iac == CSR_BT_SD_ACCESS_CODE_LIAC)
        {
            *(CsrUint24 *) value = CSR_BT_SD_ACCESS_CODE_LIAC;
        }
        else if (inquiryAccessCode == 0)
        {
            if (element->iac > CSR_BT_SD_ACCESS_CODE_LIAC &&
                element->iac < CSR_BT_SD_ACCESS_CODE_GIAC)
            {
                *(CsrUint24 *) value = element->iac;
            }
            else if (element->iac > CSR_BT_SD_ACCESS_CODE_GIAC &&
                     element->iac <= CSR_BT_CM_ACCESS_CODE_HIGHEST)
            {
                *(CsrUint24 *) value = element->iac;
            }
            else
            {
                ;
            }
        }
        else
        {
            ;
        }
    }
}

void CsrBtSdSearchInquiryStart(SdMainInstance_t *mainInst)
{
    CsrUint24 inquiryAccessCode = 0;
    CsrUint32 configMask = 0;

    /* Check if at least one instance requires lower priority level on inquiry during ACL */
    if(CsrBtSdUtilCheckSearchInstConfig(mainInst, CSR_BT_SD_SEARCH_LOW_INQUIRY_PRIORITY_DURING_ACL, TRUE, FALSE))
    {
        configMask = CSR_BT_SD_SEARCH_LOW_INQUIRY_PRIORITY_DURING_ACL;
    }

    if(CsrBtSdUtilCheckSearchInstConfig(mainInst, CSR_BT_SD_SEARCH_ENABLE_SCAN_DURING_INQUIRY, TRUE, FALSE) && !mainInst->searchBufferTid)
    {
        configMask |= CSR_BT_SD_SEARCH_ENABLE_SCAN_DURING_INQUIRY;
    }

    SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_INQ);

    SD_SI_ITERATE(mainInst->searchInst, csrBtSdSetInquiryAccessCode, &inquiryAccessCode);

    if (inquiryAccessCode == 0)
    {
        inquiryAccessCode = CSR_BT_SD_ACCESS_CODE_GIAC;
    }

#ifdef CSR_BT_LE_ENABLE
    /* If _all_ active instances have DISABLE_BT_CLASSIC _set_, do not use inquiry */
    if(!CsrBtSdUtilCheckSearchInstConfig(mainInst, CSR_BT_SD_SEARCH_DISABLE_BT_CLASSIC, TRUE, TRUE))
#endif
    {
        mainInst->baseband |= SD_BASEBAND_BREDR;
        CsrBtCmInquiryReqSend(CSR_BT_SD_IFACEQUEUE,
                              inquiryAccessCode,
                              mainInst->inquiryTxPowerLevel,
                              configMask); /* Always use LIAC if GIAC is not requested */
    }

#ifdef CSR_BT_LE_ENABLE
    /* If _all_ active instances have DISABLE_BT_LE _set_, do not use scanning */
    if(!CsrBtSdUtilCheckSearchInstConfig(mainInst, CSR_BT_SD_SEARCH_DISABLE_BT_LE, TRUE, TRUE))
    {
        mainInst->baseband |= SD_BASEBAND_LE;

        /* If _all_ active instances have ENABLE_LE_PASSIVE_SCAN _set_, use passive scanning */
        if(!CsrBtSdUtilCheckSearchInstConfig(mainInst, CSR_BT_SD_SEARCH_ENABLE_LE_PASSIVE_SCAN, FALSE, FALSE))
        {
            CsrBtGattScanReqStartSend(mainInst->gattId, CSR_BT_GATT_SCAN_PASSIVE);
        }
        else
        {
            CsrBtGattScanReqStartSend(mainInst->gattId, CSR_BT_GATT_SCAN_STANDARD);
        }
    }
#endif
}

void CsrBtSdSearchInquiryStop(SdMainInstance_t *mainInst)
{
    if (mainInst->processState == SD_STATE_PROC_INQ)
    {
#ifdef CSR_BT_LE_ENABLE
        if((mainInst->baseband & SD_BASEBAND_LE)
           && (mainInst->gattId != CSR_BT_GATT_INVALID_GATT_ID))
        {
            mainInst->baseband -= (mainInst->baseband & SD_BASEBAND_LE);
            CsrBtGattScanReqStopSend(mainInst->gattId);
        }
#endif

        if(mainInst->baseband & SD_BASEBAND_BREDR)
        {
            mainInst->baseband -= (mainInst->baseband & SD_BASEBAND_BREDR);
            CsrBtCmCancelInquiryReqSend(CSR_BT_SD_IFACEQUEUE);
        }

        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
    }
}

void CsrBtSdSearchCompleteHandler(SdSearchInstance_t *searchInst,
                                  CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier)
{
    /* A search instance has completed search */
    SdMainInstance_t *mainInst = searchInst->mainInst;

    /* Handle the specific search instance */
    CSR_BIT_UNSET(mainInst->activeInstFlags[SD_DI_INST_IDX_IMM], searchInst->searchInstIdx);
    CSR_BIT_UNSET(mainInst->activeInstFlags[SD_DI_INST_IDX_NORMAL], searchInst->searchInstIdx);
#ifdef CSR_BT_LE_ENABLE
    CSR_BIT_UNSET(mainInst->activeInstFlags[SD_DI_INST_IDX_ADVERT], searchInst->searchInstIdx);
    CSR_BIT_UNSET(mainInst->activeInstFlags[SD_DI_INST_IDX_SCANRSP], searchInst->searchInstIdx);
#endif

    SD_CHANGE_STATE(searchInst->state, SD_STATE_SEARCH_INST_IDLE);
    SD_CANCEL_TIMER(searchInst->searchTid);
    CsrBtSdSendCloseSearchInd(searchInst->phandle, resultCode, resultSupplier);

    if ((mainInst->activeInstFlags[SD_DI_INST_IDX_IMM] == 0)
        && (mainInst->activeInstFlags[SD_DI_INST_IDX_NORMAL] == 0))
    {
        /* All searches are complete */
        CsrBtSdSearchStopProcess(mainInst);
        csrBtSdSearchBufferStop(mainInst);
        CsrBtSdUtilDLCleanAll(mainInst);
        CsrBtSdSearchClearCodFilter(mainInst);
        SD_CHANGE_STATE(mainInst->searchState, SD_STATE_SEARCH_IDLE);
    }
}

void CsrBtSdSearchStopProcess(SdMainInstance_t *mainInst)
{/* Stop the current process and update state accordingly */
    switch (mainInst->processState)
    {
        case SD_STATE_PROC_INQ:
        {
            /* The inqury procedure is finished */
            CsrBtSdSearchInquiryStop(mainInst);
            CsrBtSdUtilGo2Idle(mainInst);
            break;
        }

        case SD_STATE_PROC_RRN:
        { 
            /* Wait for a CSR_BT_CM_READ_REMOTE_NAME_CFM */
            CsrBtSdUtilReadRemoteNameStop(mainInst);
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_CRRN_TO_STOP);
            break;
        }

        case SD_STATE_PROC_CRRN:
        { 
            /* Wait for a CSR_BT_CM_READ_REMOTE_NAME_CFM */
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_CRRN_TO_STOP);
            break;
        }

        case SD_STATE_PROC_CRRN_TO_STOP:
        {
            /* Wait for a CSR_BT_CM_READ_REMOTE_NAME_CFM */
            break;
        }

        case SD_STATE_PROC_SET_EVENT_TO_START:
        {
            /* The event filter has already been cleared in CsrBtSdSearchCompleteHandler() */
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SET_EVENT_TO_STOP);
            break;
        }

        case SD_STATE_PROC_SET_EVENT_TO_STOP:
        {
            /* Wait for CM_SET_EVENT_FILTER_CFM */
            break;
        }

        default:
        {
            /* Must be SD_STATE_PROC_IDLE. The inquiry procedure is finished */
            CsrBtSdUtilGo2Idle(mainInst);
            break;
        }
    }
}

void CsrBtSdSearchReqCommonHandler(SdMainInstance_t *mainInst, CsrBtSdSearchReq *searchReq)
{
    SdSearchInstance_t *searchInst = CsrBtSdUtilSearchInstGetFromHandle(mainInst, searchReq->phandle);

    if (searchInst && CsrBtSdSearchSetupParameters(mainInst, searchInst, searchReq))
    {
        if ((mainInst->searchState == SD_STATE_SEARCH_NOT_BUFFERING) ||
            (mainInst->searchState == SD_STATE_SEARCH_BUFFER_PROC))
        {
            /* Send all known results */
            SD_DL_ITERATE(mainInst->dl, CsrBtSdSearchSendAllResultsToInst, mainInst);
        }
    }
    else
    {
        CsrBtSdSendCloseSearchInd(searchReq->phandle,
                                  CSR_BT_RESULT_CODE_SD_COMMAND_DISALLOWED,
                                  CSR_BT_SUPPLIER_SD);
    }
}

/* Start all pending searches or cancel matching pending search */
CsrBool CsrBtSdSearchQueueHandler(SdMainInstance_t *mainInst, CsrSchedQid phandle, CsrBool cancel)
{
    void                    *msg;
    CsrUint16                eventClass;
    CsrMessageQueueType    *tempQueue  = NULL;
    CsrBool                  closeSend   = FALSE;
    CsrBool                  cancelled = FALSE;

    while(CsrMessageQueuePop(&mainInst->saveQueue, &eventClass, &msg))
    {
        if (eventClass == CSR_BT_SD_PRIM)
        {
            switch ((*((CsrBtSdPrim *) msg)))
            {
                case CSR_BT_SD_SEARCH_REQ:
                {
                    CsrBtSdSearchReq *prim = (CsrBtSdSearchReq *) msg;

                    if (cancel)
                    {
                        /* Has requested to cancel a search procedure */
                        if (prim->phandle == phandle)
                        {
                            /* This instance has send a Search req,
                             * send a close ind to the application and
                             * remove the message from the queue */
                            if (!closeSend)
                            {
                                /* Just to make sure that only one
                                 * close ind is send to the
                                 * application even if the same app
                                 * has send more than one search */
                                CsrBtSdSendCloseSearchInd(phandle,
                                                          CSR_BT_RESULT_CODE_SD_SEARCH_CANCELLED,
                                                          CSR_BT_SUPPLIER_SD);
                                cancelled = TRUE;
                            }
                            CsrPmemFree(msg);
                        }
                        else
                        {
                            /* Just ignore the phandle do not match,
                             * must be another Search instanc, save
                             * meg again */
                            CsrMessageQueuePush(&tempQueue, eventClass, msg);
                        }
                    }
                    else
                    {
                        /* Has requested to start a search procedure */
                        CsrBtSdSearchReqCommonHandler(mainInst, prim);
                        CsrPmemFree(msg);
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
    mainInst->saveQueue = tempQueue;
    return cancelled;
}

