/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
    All rights reserved. 

    REVISION:   $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_sd_main.h"

void CsrBtSdMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SD_PRIM, msg);
}

void CsrBtSdUtilReadRemoteNameTimeout(CsrUint16 dummy, void *ptr)
{
    SdMainInstance_t *mainInst = (SdMainInstance_t *)ptr;

    mainInst->rrnTid = 0;
    CsrBtSdUtilReadRemoteNameStop(mainInst);
}

CsrBool CsrBtSdUtilReadRemoteNameStart(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo)
{
    CsrBool run = TRUE;

#ifdef CSR_BT_LE_ENABLE
    run = FALSE;

    if ((CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_ADVERT) ||
         CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SCANRSP))
        && CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NO_CLASSIC))
    {
        CsrUint16 flagLen;
        CsrUint8 *flagTag = NULL;

        flagLen = CsrBtSdInfoGetTag(deviceInfo->advertLen, deviceInfo->advert,
                                    &flagTag, CSR_BT_EIR_DATA_TYPE_FLAGS);
        if (flagLen == 0)
        {
            flagLen = CsrBtSdInfoGetTag(deviceInfo->scanrspLen, deviceInfo->scanrsp,
                                    &flagTag, CSR_BT_EIR_DATA_TYPE_FLAGS);
        }
        if ((flagLen == 1)
            && (flagTag != NULL)
            && ((flagTag[0] & CSR_BT_EIR_FLAG_LE_LIMITED_DISCOVERABLE)
                || (flagTag[0] & CSR_BT_EIR_FLAG_LE_GENERAL_DISCOVERABLE))
            && !CsrBtSdInfoCheckTag(deviceInfo->advertLen, deviceInfo->advert,
                                    CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME)
            && !CsrBtSdInfoCheckTag(deviceInfo->scanrspLen, deviceInfo->scanrsp,
                                    CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME))
        {
            /* Start GATT name retrieval */
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_RRN);
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_ATTEMPT);
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_GATT_NAME_ATTEMPT);
            mainInst->rrnTid =  CsrSchedTimerSet(mainInst->rrnActiveTime * 1000,
                                                 CsrBtSdUtilReadRemoteNameTimeout,
                                                 0,
                                                 (void *)mainInst);
            mainInst->currentDevice = deviceInfo;
            CsrBtSdGattGetNameStart(mainInst, mainInst->currentDevice);
            run = TRUE;
        }
    }
    
    /* Do not attempt RRN if peer does not have a BR/EDR radio... */
    if (!run
        && !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NO_CLASSIC))
    {
#endif
        /* Unless LE-only, always fall thorugh to BR/EDR RRN */
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_RRN);
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_ATTEMPT);
        mainInst->rrnTid =  CsrSchedTimerSet(mainInst->rrnActiveTime * 1000,
                                             CsrBtSdUtilReadRemoteNameTimeout,
                                             0,
                                             (void *)mainInst);
        mainInst->currentDevice = deviceInfo;
        CsrBtCmReadRemoteNameReqSend(CSR_BT_SD_IFACEQUEUE, deviceInfo->addr);    
#ifdef CSR_BT_LE_ENABLE
        run = TRUE;
    }

    /* Unlikely failure handling. Avoids waiting for nothing and
     * invoke the timeout handler immediately */
    if (!run)
    {
        CsrBtSdUtilReadRemoteNameStop(mainInst);
    }
#endif
    return run;
}

void CsrBtSdUtilReadRemoteNameStop(SdMainInstance_t *mainInst)
{
    SD_CANCEL_TIMER(mainInst->rrnTid);

    if (mainInst->processState == SD_STATE_PROC_RRN)
    {
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_CRRN);
#ifdef CSR_BT_LE_ENABLE
        if(CSR_MASK_IS_SET(mainInst->currentDevice->devFlags, SD_DI_DEV_FLAG_GATT_NAME_ATTEMPT))
        {
            CsrBtSdGattGetNameStop(mainInst, mainInst->currentDevice);
        }
        else
#endif
        {
            CsrBtCmCancelReadRemoteNameReqSend(CSR_BT_SD_IFACEQUEUE, mainInst->currentDevice->addr);
        }
    }
}

void CsrBtSdUtilReadRemoteNameTimeUpdate(SdMainInstance_t *mainInst)
{
    SdSearchInstance_t *searchInst;
    mainInst->rrnActiveTime = 0;

    for (searchInst = SD_SI_GET_FIRST(mainInst->searchInst); searchInst; searchInst = searchInst->next)
    {
        if (searchInst->state == SD_STATE_SEARCH_INST_SEARCH)
        {
            /* Search is active for the instance */
            if (searchInst->readNameTimeout > mainInst->rrnActiveTime)
            {
                mainInst->rrnActiveTime = searchInst->readNameTimeout;
            }
        }
    }
}

void CsrBtSdUtilSaveMessage(SdMainInstance_t *mainInst)
{
    CsrMessageQueuePush( &mainInst->saveQueue, CSR_BT_SD_PRIM, mainInst->recvMsgP);
    mainInst->recvMsgP = NULL;
}

void CsrBtSdUtilGo2Idle(SdMainInstance_t *mainInst)
{
    CsrBtSdHouseCleaning *sdPrim = pnew(CsrBtSdHouseCleaning);

    SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_IDLE);

    /* restore save queue (if any) */
    sdPrim->type  = CSR_BT_SD_HOUSE_CLEANING;
    CsrBtSdMessagePut(CSR_BT_SD_IFACEQUEUE, sdPrim);
    mainInst->restoreMsgFlag = TRUE;
}

void CsrBtSdUtilUpdateTrustedDeviceList(SdDeviceInfo_t *deviceInfo)
{
#ifdef SD_DONT_UPDATE_SC_TRUSTED_DEVICE_LIST
    CSR_UNUSED(deviceInfo);
#else
    if (CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED))
    {
        /* Only handle already paired devices */
        CsrBtDevicePropertiesType *deviceProperties;

        deviceProperties = CsrPmemAlloc(sizeof(*deviceProperties));

        CsrBtSdUtilInfoConvertToDeviceProperties(deviceInfo, deviceProperties);
        CsrBtScUpdateDeviceReqSend(CSR_BT_SD_IFACEQUEUE, deviceProperties);
    }
#endif
}

static CsrUint8 csrBtSdUtilGetNewLowestSearchIdx(SdMainInstance_t *mainInst)
{
    SdSearchInstance_t *searchInst = SD_SI_GET_FIRST(mainInst->searchInst);
    CsrUint8 currentIdx = 0;

    while (searchInst)
    {
        if (searchInst->searchInstIdx == currentIdx)
        {
            /* Index already in use - skip to next and restart */
            currentIdx++;
            searchInst = SD_SI_GET_FIRST(mainInst->searchInst);
        }
        else
        {
            searchInst = searchInst->next;
        }
    }

    return currentIdx;
}

CsrBool CsrBtSdUtilCheckSearchInstConfig(SdMainInstance_t *mainInst, CsrUint32 mask, CsrBool set, CsrBool all)
{
    /* set | all
        0  |  0    : Returns TRUE if a single instance has NOT set the mask
        1  |  0    : Returns TRUE if a single instance has set the mask
        0  |  1    : Returns TRUE if all instances have NOT set the mask
        1  |  1    : Returns TRUE if all instances have set the mask */
    SdSearchInstance_t *searchInst;

    for (searchInst = SD_SI_GET_FIRST(mainInst->searchInst); searchInst; searchInst = searchInst->next)
    {
        if (searchInst->state == SD_STATE_SEARCH_INST_SEARCH)
        {
            /* Only consider active instances */
            if ((CSR_MASK_IS_SET(searchInst->activeSearchConfig, mask) && set) || /* Mask is set */
                (CSR_MASK_IS_UNSET(searchInst->activeSearchConfig, mask) &&  !set)) /* Mask is not set */
            {
                if (!all)
                {
                    /* A single instance fulfilled requirements */
                    return TRUE;
                }
            }
            else if (all)
            {
                /* An instance did not fulfill requirements */
                return FALSE;
            }
        }
    }

    return all; /* All or none fulfilled the requirements */
}

CsrUint32 CsrBtSdUtilGetDeviceStatus(SdDeviceInfo_t *deviceInfo)
{
    CsrUint32 status = 0;

    if (deviceInfo)
    {
        status |= CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED) ? CSR_BT_SD_STATUS_PAIRED : 0;
        status |= CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_TRUSTED) ? CSR_BT_SD_STATUS_TRUSTED : 0;
        status |= CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED) ? CSR_BT_SD_STATUS_REMOTE_NAME_UPDATED : 0;

        status |= CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NO_CLASSIC) ? 0 : CSR_BT_SD_STATUS_RADIO_BREDR;
#ifdef CSR_BT_LE_ENABLE
        status |= CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_LE_PRIVATE_ADDR) ? CSR_BT_SD_STATUS_PRIVATE_ADDR : 0;
        status |= CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_ADVERT) ? CSR_BT_SD_STATUS_RADIO_LE : 0;
        status |= CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SCANRSP) ? CSR_BT_SD_STATUS_RADIO_LE : 0;

        /* If we only received EIR results from a device, it may still
         * support LE. Look at the data to see if this is the case */
        if(deviceInfo->infoLen > 0)
        {
            CsrUint16 flagLen;
            CsrUint8 *flagTag = NULL;
            flagLen = CsrBtSdInfoGetTag(deviceInfo->infoLen,
                                        deviceInfo->info,
                                        &flagTag,
                                        CSR_BT_EIR_DATA_TYPE_FLAGS);
            if((flagLen == 1) && (flagTag != NULL)
                && ( (flagTag[0] & CSR_BT_EIR_FLAG_SIMUL_BREDR_LE_CONTROLLER)
                     || (flagTag[0] & CSR_BT_EIR_FLAG_SIMUL_BREDR_LE_HOST)) )
            {
                status |= CSR_BT_SD_STATUS_RADIO_LE;
            }
        }
#endif
    }

    return status;
}

CsrBool CsrBtSdUtilIsUpdatedNameRequired(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo, CsrBool *nameAvailable)
{
    /* Check if name must be updated:
     * - An incomplete name or none at all is available
     * - Name has not been updated this search and updated names are requested
     * - At least one app has not disabled name reading */

    *nameAvailable = CsrBtSdInfoCheckTag(deviceInfo->infoLen, deviceInfo->info,
                                         CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME);

#ifdef CSR_BT_LE_ENABLE
    

    if( (*nameAvailable == FALSE) && (deviceInfo->scanrspLen > 0))
    {
        *nameAvailable = CsrBtSdInfoCheckTag(deviceInfo->scanrspLen, deviceInfo->scanrsp,
                                             CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME);
    }
    if((*nameAvailable == FALSE) && (deviceInfo->advertLen > 0))
    {
        *nameAvailable = CsrBtSdInfoCheckTag(deviceInfo->advertLen, deviceInfo->advert,
                                             CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME);
    }
    if (*nameAvailable)
    {
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED);
    }
#endif

    if (mainInst->nameUpdateRequired
        && !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED)
        && !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_ATTEMPT))
    {
        /* At least one instance requires an updated name (and an attempt has not already been made) */
        return TRUE;
    }
    else if (CsrBtSdUtilCheckSearchInstConfig(mainInst, CSR_BT_SD_SEARCH_DISABLE_NAME_READING, TRUE, TRUE))
    {
        /* All search instances have disabled name reading */
        return FALSE;
    }
    else if (!*nameAvailable && !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_ATTEMPT))
    {
        /* A complete name is not available and an attempt has not
         * been made at retrieving one */
        return TRUE;
    }

    return FALSE;
}

void CsrBtSdUtilDeviceInfoDeinit(CsrCmnListElm_t *elem)
{
    SdDeviceInfo_t *di;
    di = (SdDeviceInfo_t*)elem;
    CsrPmemFree(di->list);
}

void CsrBtSdUtilSearchInstDeinit(CsrCmnListElm_t *elem)
{
    CSR_UNUSED(elem);
}

SdSearchInstance_t *CsrBtSdUtilSearchInstAdd(SdMainInstance_t *mainInst, CsrSchedQid phandle)
{
    SdSearchInstance_t *searchInst = NULL;

    if (SD_SI_COUNT(mainInst->searchInst) < SD_SEARCH_INSTANCES_MAX)
    {
        searchInst = SD_SI_ADD_INST(mainInst->searchInst);

        /* Initialize all parameters */
        searchInst->phandle             = phandle;
        searchInst->readNameTimeout     = CSR_BT_SD_READ_NAME_TIMEOUT_DEFAULT;
        searchInst->activeSearchConfig  = CSR_BT_SD_SEARCH_CONFIG_DEFAULT;
        searchInst->cod                 = 0;
        searchInst->codMask             = 0;
        searchInst->iac                 = CSR_BT_SD_ACCESS_CODE_GIAC;
        searchInst->searchResultsMax    = SD_SEARCH_MAX_RESULTS_DEFAULT;
        searchInst->searchResultsCount  = 0;
        searchInst->rssiBufferTime      = 0; /* Will always be updated by a CsrBtSdSearchReq before a search is started */
        searchInst->rssiThreshold       = CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE;
        searchInst->searchConfig        = CSR_BT_SD_SEARCH_CONFIG_DEFAULT;
        searchInst->searchTid           = 0;
        searchInst->totalSearchTime     = 0; /* Will always be updated by a CsrBtSdSearchReq before a search is started */
        searchInst->searchInstIdx       = 0xFF; /* Initialize to CSRMAX number since csrBtSdUtilGetNewLowestSearchIdx will access the parameter */
        searchInst->searchInstIdx       = csrBtSdUtilGetNewLowestSearchIdx(mainInst);
        searchInst->mainInst            = mainInst;
#ifdef CSR_BT_LE_ENABLE
        searchInst->leRssiThreshold     = CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE;
#endif
        SD_CHANGE_STATE(searchInst->state, SD_STATE_SEARCH_INST_IDLE);
    }

    return searchInst;
}

CsrBool CsrBtSdUtilSearchInstComparePhandle(CsrCmnListElm_t *elem, void *data)
{
    return (((SdSearchInstance_t *)elem)->phandle == *((CsrSchedQid *)data));
}

void CsrBtSdUtilSearchInstUnsetInst(CsrCmnListElm_t *elem, void *data)
{
    /* data: searchInst->searchInstIdx */
    CsrUintFast8 i;
    for (i = 0; i < SD_DI_INST_IDX_COUNT; i++)
    {
        CSR_BIT_UNSET(((SdDeviceInfo_t *)elem)->instFlags[i], *(CsrUint8 *)data);
    }
}

SdSearchInstance_t *CsrBtSdUtilSearchInstGetFromHandle(SdMainInstance_t *mainInst, CsrSchedQid phandle)
{
    SdSearchInstance_t *searchInst = SD_SI_FIND_FROM_PHANDLE(mainInst->searchInst, &phandle);

    return searchInst ? searchInst : CsrBtSdUtilSearchInstAdd(mainInst, phandle);
}

static void csrBtSdUtilDLRemove(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo, CsrBool paired)
{
    if (deviceInfo)
    {
        if (deviceInfo->infoLen > 0)
        {
            paired ? mainInst->dlPairedInfoCount-- : mainInst->dlInfoCount--;
        }
        if (paired)
        {
            mainInst->dlPairedCount--;
        }

        SD_DL_REMOVE_DEVICE(mainInst->dl, deviceInfo);
    }
}

static SdDeviceInfo_t *csrBtSdUtilDLInsert(SdMainInstance_t *mainInst, CsrUint16 infoLen, CsrBool paired)
{
    SdDeviceInfo_t *deviceInfo = SD_DL_ADD_DEVICE(mainInst->dl, infoLen);
    
    deviceInfo->info  = (CsrUint8 *)(deviceInfo);
    deviceInfo->info += sizeof(SdDeviceInfo_t);

    if (infoLen > 0)
    {
        paired ? mainInst->dlPairedInfoCount++ : mainInst->dlInfoCount++;
    }

    if (paired)
    {
        mainInst->dlPairedCount++;
    }

    return deviceInfo;
}

SdDeviceInfo_t *CsrBtSdUtilDLFindOldest(SdMainInstance_t *mainInst, CsrBool withInfo)
{
    /* Find the oldest device, e.g. the device with the lowest value of 'age' */
    SdDeviceInfo_t *deviceInfo;
    SdDeviceInfo_t *currDeviceInfo = SD_DL_GET_FIRST(mainInst->dl);
    CsrUint32 currentAge = 0xFFFFFFFF;

    for (deviceInfo = SD_DL_GET_FIRST(mainInst->dl); deviceInfo; deviceInfo = deviceInfo->next)
    {
        if (deviceInfo->age < currentAge)
        {
            if (/* Include only devices with info if requested */
                ((!withInfo && (deviceInfo->infoLen == 0)) ||
                 (withInfo && (deviceInfo->infoLen > 0))) &&
                /* Exclude paired devices */
                (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED)))
            {
                currentAge = deviceInfo->age;
                currDeviceInfo = deviceInfo;
            }
        }
    }

    return currDeviceInfo;
}

static void csrBtSdUtilDIReallocate(SdMainInstance_t *mainInst, SdDeviceInfo_t **deviceInfo,
                                    CsrUint16 newInfoLen, CsrBool maintainOld)
{
    /* Allocates a new element since the size is different */
    CsrBool paired = CSR_MASK_IS_SET((*deviceInfo)->devFlags, SD_DI_DEV_FLAG_PAIRED);
    SdDeviceInfo_t *newDeviceInfo = csrBtSdUtilDLInsert(mainInst, newInfoLen, paired);
    SdDeviceInfo_t *next = newDeviceInfo->next;
    SdDeviceInfo_t *prev = newDeviceInfo->prev;

    /* Copy everything from the old device to the new device */
    *newDeviceInfo = **deviceInfo;

    /* Links should be maintained for the new device */
    newDeviceInfo->next = next;
    newDeviceInfo->prev = prev;

    /* infoLen should be updated to new requested length */
    newDeviceInfo->infoLen = newInfoLen;
    newDeviceInfo->info    = (CsrUint8 *)(newDeviceInfo);
    newDeviceInfo->info   += sizeof(SdDeviceInfo_t);

    /* Copy the info from the old device to the new if requested */
    if (maintainOld && (newDeviceInfo->infoLen >= (*deviceInfo)->infoLen))
    {
        CsrMemCpy(newDeviceInfo->info, (*deviceInfo)->info, (*deviceInfo)->infoLen);
    }

    /* Remove the old device from the DL and update the parent pointer (can be mainInst->currentDevice) */
    (*deviceInfo)->listCount = 0;
    (*deviceInfo)->list = NULL;
  
    if (mainInst->currentDevice == *deviceInfo)
    {
        /* The currently active device can be updated through a crossing incoming name update */
        csrBtSdUtilDLRemove(mainInst, mainInst->currentDevice, paired);
        mainInst->currentDevice = newDeviceInfo;
    }
    else
    {
        csrBtSdUtilDLRemove(mainInst, *deviceInfo, paired);
    }

    *deviceInfo = newDeviceInfo;
}

static void csrBtSdUtilDIInit(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo, CsrBtDeviceAddr *addr, CsrUint16 infoLen)
{
    CsrUintFast8 i;

    deviceInfo->addr        = *addr;
    deviceInfo->age         = mainInst->dlCurrentAge++;
    deviceInfo->cod         = 0;
    deviceInfo->devFlags    = 0;
    deviceInfo->infoLen     = infoLen;
    deviceInfo->listCount   = 0;
    deviceInfo->list        = NULL;
    deviceInfo->rssi        = CSR_BT_RSSI_INVALID;
    deviceInfo->info        = (CsrUint8 *)(deviceInfo);
    deviceInfo->info       += sizeof(SdDeviceInfo_t);

#ifdef CSR_BT_LE_ENABLE
    CsrMemSet(deviceInfo->advert, 0, SD_LE_REPORT_LENGTH);
    CsrMemSet(deviceInfo->scanrsp, 0, SD_LE_REPORT_LENGTH);
    deviceInfo->advertLen = 0;
    deviceInfo->scanrspLen = 0;
    deviceInfo->leRssi = CSR_BT_RSSI_INVALID;
    deviceInfo->serviceChangedHandle = CSR_BT_GATT_SERVICE_CHANGED_NOT_FOUND;
#endif

    for (i = 0; i < SD_DI_INST_IDX_COUNT; i++)
    {
        deviceInfo->instFlags[i] = 0;
    }
}

static void csrBtSdUtilWriteUint16Field(CsrUint16 data, CsrUint8 *info, CsrUint16 *writePtr)
{
    info[(*writePtr)++] = (CsrUint8)(data & 0xFF);
    info[(*writePtr)++] = (CsrUint8)((data >> 8) & 0xFF);
}

static CsrUint16 csrBtSdUtilService16Len(CsrBtDevicePropertiesType *deviceProperties)
{
    CsrUint16 service16Count;

    service16Count = CsrBitCountSparse(deviceProperties->knownServices11_00_31) +
        CsrBitCountSparse(deviceProperties->knownServices11_32_63) +
        CsrBitCountSparse(deviceProperties->knownServices12_00_31) +
        CsrBitCountSparse(deviceProperties->knownServices13_00_31);


    return ((service16Count > 0) ? ((CsrUint16)(service16Count * sizeof(uuid16_t))) : 0);
}

static void csrBtSdUtilWriteService16Field(CsrBtDevicePropertiesType *deviceProperties, CsrUint8 *info)
{
    CsrUint32 i;
    CsrUint16 service16Length;
    CsrUint16 writePtr = 0;

    service16Length = csrBtSdUtilService16Len(deviceProperties);
    if (service16Length)
    {
        csrBtSdUtilWriteUint16Field((CsrUint16)(service16Length + CSR_BT_SD_DEVICE_INFO_FORMAT_TYPE_SIZE), info, &writePtr);
        csrBtSdUtilWriteUint16Field(CSR_BT_EIR_DATA_TYPE_COMPLETE_16_BIT_UUID, info, &writePtr);

        for (i = 0; i < 32; i++)
        {
            if (CSR_BIT_IS_SET(deviceProperties->knownServices11_00_31, i))
            {
                /* UUID: 0x1100 -> 0x111F */
                csrBtSdUtilWriteUint16Field((CsrUint16)(0x1100 + i), info, &writePtr);
            }
            if (CSR_BIT_IS_SET(deviceProperties->knownServices11_32_63, i))
            {
                /* UUID: 0x1120 -> 0x113F */
                csrBtSdUtilWriteUint16Field((CsrUint16)(0x1120 + i), info, &writePtr);
            }
            if (CSR_BIT_IS_SET(deviceProperties->knownServices12_00_31, i))
            {
                /* UUID: 0x1200 -> 0x121F */
                csrBtSdUtilWriteUint16Field((CsrUint16)(0x1200 + i), info, &writePtr);
            }
            if (CSR_BIT_IS_SET(deviceProperties->knownServices13_00_31, i))
            {
                /* UUID: 0x1300 -> 0x131F */
                csrBtSdUtilWriteUint16Field((CsrUint16)(0x1300 + i), info, &writePtr);
            }
        }
    }
}

void CsrBtSdUtilDLTrim(SdMainInstance_t *mainInst)
{
    /* Check if the size limits of the Device List have been reached */
    CsrBool restart = FALSE;

    do
    {
        if ((mainInst->dlInfoMax != CSR_BT_UNLIMITED) &&
            (mainInst->dlInfoCount > mainInst->dlInfoMax))
        {
            /* Too many devices with info in list */
            SdDeviceInfo_t *oldestDevice = CsrBtSdUtilDLFindOldest(mainInst, TRUE);
            csrBtSdUtilDIReallocate(mainInst, &oldestDevice, 0, FALSE); /* Reallocate in order to remove info */
            restart = TRUE;
        }
        else
        {
            restart = FALSE;
        }

        if ((mainInst->dlMax != CSR_BT_UNLIMITED) &&
            ((SD_DL_COUNT(mainInst->dl) - mainInst->dlPairedCount) > mainInst->dlMax))
        {
            /* Too many devices in list */
            csrBtSdUtilDLRemove(mainInst, CsrBtSdUtilDLFindOldest(mainInst, FALSE), FALSE);
            restart = TRUE;
        }
    } while (restart);
}

SdDeviceInfo_t *CsrBtSdUtilDLAdd(SdMainInstance_t *mainInst, CsrBtDeviceAddr *addr,
                                 CsrUint16 infoLen, CsrBool preventRealloc,
                                 CsrBool *infoUpdated, CsrBool paired)
{
    /* Adding a device can be triggered by the following events:
     * - Inquiry result
     * - Only realloc if not found before
     * - Read remote name ind/cfm
     * - Do not realloc - update tag after addition
     * - SDP search initiated (SD_READ_AVAI../SD_READ_SERVI...)
     * - Do not realloc - update tag after SDP search complete
     * - Device bonded (CSR_BT_SC_BOND_IND)
     * - Do not realloc
     * - SC_DB list being read (CSR_BT_SC_READ_DEVICE_RECORD_IND|CFM)
     * - Always realloc  */
    SdDeviceInfo_t *deviceInfo = SD_DL_FIND_FROM_ADDR(mainInst->dl, addr);

    if (deviceInfo)
    {
        deviceInfo->age = mainInst->dlCurrentAge++;

        /* Reallocate if the available info has changed since an earlier search or is from SC DB */
        if ((CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED) ||
             !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_FOUND)) &&
            ((deviceInfo->infoLen != infoLen) && (!preventRealloc)))
        {
            csrBtSdUtilDIReallocate(mainInst,
                                    &deviceInfo,
                                    infoLen,
                                    FALSE);

            *infoUpdated = TRUE;
        }
        else
        {
            *infoUpdated = FALSE;
        }
    }
    else
    {
        deviceInfo = csrBtSdUtilDLInsert(mainInst, infoLen, paired);

        /* Make sure there is not overflow in the Device List */
        csrBtSdUtilDIInit(mainInst, deviceInfo, addr, infoLen);

        if (paired)
        {
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED); 
        }

        *infoUpdated = TRUE;
    }

    CsrBtSdUtilDLTrim(mainInst);
    return deviceInfo;
}

#ifdef CSR_BT_LE_ENABLE
SdDeviceInfo_t *CsrBtSdUtilDLAddCmReportRes(SdMainInstance_t *mainInst, CsrBtGattReportInd *prim)
{
    SdDeviceInfo_t *deviceInfo;
    CsrBool infoUpdated;
    CsrUint16 flagLen;
    CsrUint8 *flagTag = NULL;
    /*CsrUint8 *tmpInfo = NULL;
    CsrBool combineTags = FALSE;*/

    deviceInfo = CsrBtSdUtilDLAdd(mainInst,
                                  &(prim->address.addr),
                                  0, /* infoLen */
                                  TRUE, /* prevent realloc */
                                  &infoUpdated,
                                  FALSE);
    
    /*if (deviceInfo->infoLen > 0)
    {
        tmpInfo = deviceInfo->info;
    }*/
    
    /* Always override existing data */
    if(prim->eventType == HCI_ULP_EV_ADVERT_SCAN_RESPONSE)
    {
        CsrMemSet(deviceInfo->scanrsp, 0, SD_LE_REPORT_LENGTH);
        deviceInfo->scanrspLen = 0;
        CsrBtSdUtilConvertEir2Info(prim->lengthData, prim->data,
                                   &deviceInfo->scanrspLen, deviceInfo->scanrsp);
        if (deviceInfo->scanrspLen)
        {
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SCANRSP);
            /*combineTags = TRUE;*/
        }
    }
    /* Directed connects does not have any AD data */
    else if(prim->eventType != HCI_ULP_EV_ADVERT_CONNECTABLE_DIRECTED)
    {
        CsrMemSet(deviceInfo->advert, 0, SD_LE_REPORT_LENGTH);
        deviceInfo->advertLen = 0;
        CsrBtSdUtilConvertEir2Info(prim->lengthData, prim->data,
                                   &deviceInfo->advertLen, deviceInfo->advert);
        if (deviceInfo->advertLen)
        {
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_ADVERT);
            /*combineTags = TRUE;*/
        }
    }

   /* if (combineTags)
    {
        CsrBtSdUtilCombineTags(deviceInfo, (CsrUint32 *)&deviceInfo->infoLen ,(CsrUint8**)&(deviceInfo->info));
        CsrPmemFree(tmpInfo);
    }*/

    /* Do we have the entire name? */
    if (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED) &&
        (CsrBtSdInfoCheckTag(deviceInfo->infoLen, deviceInfo->info,
                             CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME)))
    {
        /* Name is updated */
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED);
    }

    /* Is address random/private */
    if(prim->address.type == CSR_BT_ADDR_PRIVATE)
    {
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_LE_PRIVATE_ADDR);
    }

    /* Does peer disable BR/EDR support? */
    flagLen = CsrBtSdInfoGetTag(deviceInfo->advertLen, deviceInfo->advert,
                                &flagTag, CSR_BT_EIR_DATA_TYPE_FLAGS);
    if ((((flagLen == 1)
          && (flagTag != NULL)
          && (flagTag[0] & CSR_BT_EIR_FLAG_LE_BREDR_NOT_SUPPORTED))
         || (flagLen == 0)
         || (flagTag == NULL))
        && !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SEEN_BREDR))
    {
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NO_CLASSIC);
    }
    else
    {
        flagLen = CsrBtSdInfoGetTag(deviceInfo->scanrspLen, deviceInfo->scanrsp,
                                &flagTag, CSR_BT_EIR_DATA_TYPE_FLAGS);
        if ((((flagLen == 1)
              && (flagTag != NULL)
              && (flagTag[0] & CSR_BT_EIR_FLAG_LE_BREDR_NOT_SUPPORTED))
             || (flagLen == 0)
             || (flagTag == NULL))
            && !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SEEN_BREDR))
        {
            CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NO_CLASSIC);
        }
    }

    /* Common handling */    
    deviceInfo->leRssi = prim->rssi;
    CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_FOUND);

    return deviceInfo;
}
#endif

SdDeviceInfo_t *CsrBtSdUtilDLAddCmInqRes(SdMainInstance_t *mainInst, CsrBtCmInquiryResultInd *prim)
{
    CsrBool infoUpdated;
    CsrBool trustedUpdated;
    CsrUint16 infoLen;
    SdDeviceInfo_t *deviceInfo;

    trustedUpdated = FALSE;

    /* Validate EIR. Ignore data if not valid. */
    if(!CsrBtSdUtilEirValidate(prim->eirData, prim->eirDataLength))
    {
        CsrPmemFree(prim->eirData);
        prim->eirData = NULL;
        prim->eirDataLength = 0;
    }

    /* Length and type field are one byte longer each */
    infoLen = prim->eirDataLength + CsrBtSdUtilEirCountTags(prim->eirDataLength, prim->eirData) * 2;

    /* Prevent realloc if non-EIR device */
    deviceInfo = CsrBtSdUtilDLAdd(mainInst,
                                  &prim->deviceAddr,
                                  infoLen,
                                  (CsrBool)!CSR_MASK_IS_SET(prim->status, CSR_BT_CM_INQUIRY_STATUS_EIR), 
                                  &infoUpdated,
                                  FALSE);

    /* Specific EIR handling */
    if (CSR_MASK_IS_SET(prim->status, CSR_BT_CM_INQUIRY_STATUS_EIR))
    {
        /* This is an EIR result */
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_EIR);

        if (infoUpdated && (infoLen > 0))
        {
            /* EIR contains infoUpdated information */
            CsrBtSdUtilConvertEir2Info(prim->eirDataLength, prim->eirData,
                                       &deviceInfo->infoLen, deviceInfo->info);

            if (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED) &&
                (CsrBtSdInfoCheckTag(deviceInfo->infoLen, deviceInfo->info,
                                     CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME)))
            {
                /* Name is updated */
                CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED);
            }

            CsrBtSdUtilUpdateTrustedDeviceList(deviceInfo);
            trustedUpdated = TRUE;
        }
    }

    if (!trustedUpdated && (deviceInfo->cod != prim->classOfDevice))
    {
        /* CoD has changed */
        CsrBtSdUtilUpdateTrustedDeviceList(deviceInfo);
    }

    /* Common handling */
    deviceInfo->cod  = prim->classOfDevice;
    deviceInfo->rssi = prim->rssi;
    CSR_MASK_UNSET(deviceInfo->devFlags, SD_DI_DEV_FLAG_NO_CLASSIC);
    CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_FOUND);
    CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SEEN_BREDR);

    return deviceInfo;
}

void CsrBtSdUtilDLAddDevicePropertiesType(SdMainInstance_t          *mainInst, 
                                          CsrBtDevicePropertiesType *deviceProperties, 
                                          CsrUint32                 serviceChangedHandle)
{
    CsrBool infoUpdated;
    SdDeviceInfo_t *deviceInfo;
    CsrUint16 serviceLen = csrBtSdUtilService16Len(deviceProperties);
    CsrUint16 nameLen = (CsrUint16)CsrUtf8StringLengthInBytes(deviceProperties->friendlyName);
    CsrUint16 infoLen = ((serviceLen > 0) ? (serviceLen + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE) : 0 ) +
        ((nameLen > 0) ? (nameLen + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE) : 0);

    /* Add/update element */
    deviceInfo = CsrBtSdUtilDLAdd(mainInst,
                                  &deviceProperties->deviceAddr,
                                  infoLen,
                                  FALSE,
                                  &infoUpdated,
                                  TRUE);

    /* Insert all services */
    csrBtSdUtilWriteService16Field(deviceProperties, deviceInfo->info);

    if (nameLen > 0)
    {
        /* Insert the name */
        CsrUint16 serviceOffset = (serviceLen > 0 ? serviceLen + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE: 0);
        CsrUintFast8 i;

        CSR_COPY_UINT16_TO_LITTLE_ENDIAN(nameLen + CSR_BT_SD_DEVICE_INFO_FORMAT_TYPE_SIZE, deviceInfo->info + serviceOffset);
        CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL, deviceInfo->info + serviceOffset + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE);
        for (i=0;i<nameLen;i++)
        {
            *(deviceInfo->info + serviceOffset + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE + i) = deviceProperties->friendlyName[i];
        }
    }

    /* Update remaining parameters */
    deviceInfo->cod = deviceProperties->classOfDevice;
#ifdef CSR_BT_LE_ENABLE
    deviceInfo->serviceChangedHandle = serviceChangedHandle;
#endif 

    if (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED))
    {
        /* Device was already on DL but not previously paired, update counters */
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED);

        mainInst->dlPairedCount++;

        if (deviceInfo->infoLen)
        {
            /* Adjust info counters */
            mainInst->dlInfoCount--;
            mainInst->dlPairedInfoCount++;
        }
    }
    if(deviceProperties->authorised )
    {
        CSR_MASK_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_TRUSTED); 
    }
}

SdDeviceInfo_t *CsrBtSdUtilDLAddFromAddrOnly(SdMainInstance_t *mainInst, CsrBtDeviceAddr *addr)
{
    CsrBool dummy;
    return CsrBtSdUtilDLAdd(mainInst,
                            addr,
                            0,
                            TRUE,
                            &dummy,
                            FALSE);
}

CsrBool CsrBtSdUtilDLGetCountFromSize(SdMainInstance_t *mainInst, SdDeviceInfo_t *currentDevice,
                                      CsrUint32 size, CsrUint32 config, CsrUint32 *count)
{
    *count = 0;

    while (currentDevice)
    {
        if (!(CSR_MASK_IS_SET(config, CSR_BT_SD_DEVICE_EXCLUDE_PAIRED_DEVICES)
              && CSR_MASK_IS_SET(currentDevice->devFlags, SD_DI_DEV_FLAG_PAIRED)) &&
            !(CSR_MASK_IS_SET(config, CSR_BT_SD_DEVICE_EXCLUDE_NON_PAIRED_DEVICES)
              && !CSR_MASK_IS_SET(currentDevice->devFlags, SD_DI_DEV_FLAG_PAIRED)))
        {
            /* Device should be included */
            if (size >= (sizeof(CsrBtSdDeviceInfoType) + currentDevice->infoLen))
            {
                /* There is room for the device */
                size -= (sizeof(CsrBtSdDeviceInfoType) + currentDevice->infoLen);
                (*count)++;
            }
            else if ((*count) == 0)
            {
                /* Always include at least one device in the indication */
                (*count)++;
                size = 0;
            }
            else
            {
                return FALSE; /* Not room for all devices */
            }
        }

        currentDevice = currentDevice->next;
    }

    return TRUE; /* Room for all devices */
}

CsrBool CsrBtSdUtilCompareBdaddr(CsrCmnListElm_t *elem, void *value)
{
    return CSR_BT_BD_ADDR_EQ(((SdDeviceInfo_t *)elem)->addr, *(CsrBtDeviceAddr *)value);
}

CsrInt32 CsrBtSdUtilSortRssi(CsrCmnListElm_t *elem1, CsrCmnListElm_t *elem2)
{
    /* Sort ascending */
    SdDeviceInfo_t *dev1 = (SdDeviceInfo_t *)elem1;
    SdDeviceInfo_t *dev2 = (SdDeviceInfo_t *)elem2;
    CsrInt8 rssi1;
    CsrInt8 rssi2;

#ifdef CSR_BT_LE_ENABLE
    rssi1 = ((dev1->rssi != CSR_BT_RSSI_INVALID)
             ? dev1->rssi
             : dev1->leRssi);
    rssi2 = ((dev2->rssi != CSR_BT_RSSI_INVALID)
             ? dev2->rssi
             : dev2->leRssi);
#else
    rssi1 = dev1->rssi;
    rssi2 = dev2->rssi;
#endif

    if (rssi1 < rssi2)
    {
        return 1;
    }
    else if (rssi1 > rssi2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void CsrBtSdUtilDLCleanAll(SdMainInstance_t *mainInst)
{
    /* - Clean all devices that have not been shown (normal results)
       - Unless if they are non-EIR devices with name or services available
       - Remove all found/updated/... name flag */

    SdDeviceInfo_t *deviceInfo = SD_DL_GET_FIRST(mainInst->dl), *nextDeviceInfo;

    while (deviceInfo)
    {
        /* Store link to next since deviceInfo can be removed later */
        nextDeviceInfo = deviceInfo->next;

        if (!deviceInfo->instFlags[SD_DI_INST_IDX_NORMAL] && /* Normal results have not been sent to any instances */
            (deviceInfo->infoLen == 0) && /* No extended info is available */
            !CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED) &&
            (deviceInfo != mainInst->currentDevice)) /* This is not the device being currently handled (name can be pending) */
        {
            /* Device can be removed from list */
            csrBtSdUtilDLRemove(mainInst, deviceInfo, FALSE);
        }
        else
        {
            /* Device should be kept - reset flags */
            CSR_MASK_UNSET(deviceInfo->devFlags,
                           SD_DI_DEV_FLAG_FOUND |
                           SD_DI_DEV_FLAG_NAME_UPDATED |
                           SD_DI_DEV_FLAG_NAME_ATTEMPT |
                           SD_DI_DEV_FLAG_GATT_NAME_ATTEMPT |
                           SD_DI_DEV_FLAG_DONE);
        }

        deviceInfo = nextDeviceInfo;
    }
}

void CsrBtSdUtilDLCleanNonPaired(SdMainInstance_t *mainInst)
{
    SdDeviceInfo_t *deviceInfo = SD_DL_GET_FIRST(mainInst->dl), *nextDeviceInfo;

    while (deviceInfo)
    {
        nextDeviceInfo = deviceInfo->next;

        if (!CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED))
        {
            csrBtSdUtilDLRemove(mainInst, deviceInfo, FALSE);
        }

        deviceInfo = nextDeviceInfo;
    }

    /* The DL has been reset - also reset the current age - just
     * ignore the age for the currently paired devices */
    mainInst->dlCurrentAge = 0;
}

void CsrBtSdUtilDLCleanPairedList(SdMainInstance_t *mainInst)
{
    SdDeviceInfo_t *deviceInfo = SD_DL_GET_FIRST(mainInst->dl), *nextDeviceInfo;

    while (deviceInfo)
    {
        nextDeviceInfo = deviceInfo->next;
#ifdef CSR_BT_LE_ENABLE
        /* Makes sure that the attribute handle for the Service Changed
           Characteristic Value is set invalid */
           deviceInfo->serviceChangedHandle = CSR_BT_GATT_SERVICE_CHANGED_NOT_FOUND;
#endif
        if (CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED))
        {
            /* Device is marked as paired - debond it */
            CSR_MASK_UNSET(deviceInfo->devFlags, SD_DI_DEV_FLAG_PAIRED | SD_DI_DEV_FLAG_TRUSTED);
            mainInst->dlPairedCount--;

            if (deviceInfo->infoLen)
            {
                /* Adjust info counters */
                mainInst->dlInfoCount++;
                mainInst->dlPairedInfoCount--;
            }
        }

        deviceInfo = nextDeviceInfo;
    }
}

CsrUint8 CsrBtSdUtilEirCountTags(CsrUint8 eirDataLen, CsrUint8 *eirData)
{
    CsrUintFast16 eirIndex;
    CsrUint8 tagCount = 0;

    /* Validate EIR. Stop processing if invalid */
    if(!CsrBtSdUtilEirValidate(eirData, eirDataLen))
    {
        return 0;
    }

    /* Determine number of tags in order to determine the size of info */
    for (eirIndex = 0; eirIndex < eirDataLen; eirIndex += eirData[eirIndex] + 1)
    {
        tagCount++;
    }

    return tagCount;
}

CsrBool CsrBtSdUtilEirValidate(CsrUint8 *data, CsrUint16 length)
{
    CsrUintFast16 i;

    /* EIR must contain at least one entry */
    if(length < CSR_BT_EIR_TAG_HEADER_SIZE)
    {
        return FALSE;
    }

    /* Determine number of tags in order to determine the size of info */
    for (i = 0;
         i + CSR_BT_EIR_TAG_HEADER_SIZE < length;
         i += data[i] + 1)
    {
        ; /* do nothing */
    }

    return (CsrBool)(i == length
                     ? TRUE
                     : FALSE);
}

#ifndef SD_DONT_UPDATE_SC_TRUSTED_DEVICE_LIST
void CsrBtSdUtilInfoConvertToDeviceProperties(SdDeviceInfo_t *deviceInfo, CsrBtDevicePropertiesType *deviceProperties)
{
    CsrUint16 nameLen;
    CsrUint8 *name;
    CsrUintFast16 serviceCount, i;
    CsrBtUuid32 *serviceList;

    /* Find name */
    nameLen = CsrBtSdInfoGetTag(deviceInfo->infoLen,
                                deviceInfo->info,
                                &name,
                                CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL);

    /* Insert name */
    if( nameLen > CSR_BT_MAX_FRIENDLY_NAME_LEN )
    {
        nameLen = CSR_BT_MAX_FRIENDLY_NAME_LEN;
    }
    CsrUtf8StrNCpy(deviceProperties->friendlyName, (CsrUtf8String*)name, nameLen);
    CsrUtf8StrTruncate(deviceProperties->friendlyName, nameLen);
    
    /* Initialize rest of parameters */
    deviceProperties->deviceAddr            = deviceInfo->addr;
    deviceProperties->classOfDevice         = deviceInfo->cod;
    deviceProperties->authorised            = CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_TRUSTED);
    deviceProperties->knownServices11_00_31 = 0;
    deviceProperties->knownServices11_32_63 = 0;
    deviceProperties->knownServices12_00_31 = 0;
    deviceProperties->knownServices13_00_31 = 0;

    /* Convert services */
    serviceCount = CsrBtSdReadServiceList(deviceInfo->info, deviceInfo->infoLen, &serviceList);

    for (i = 0; i < serviceCount; i++)
    {
        if (serviceList[i] <= 0x111F)
        {
            /* UUID: 0x1100 -> 0x111F */
            CSR_BIT_SET(deviceProperties->knownServices11_00_31, serviceList[i] - 0x1100);
        }
        else if (serviceList[i] <= 0x113F)
        {
            /* UUID: 0x1120 -> 0x113F */
            CSR_BIT_SET(deviceProperties->knownServices11_32_63, serviceList[i] - 0x1120);
        }
        else if (serviceList[i] <= 0x121F)
        {
            /* UUID: 0x1200 -> 0x121F */
            CSR_BIT_SET(deviceProperties->knownServices12_00_31, serviceList[i] - 0x1200);
        }
        else if (serviceList[i] <= 0x131F)
        {
            /* UUID: 0x1300 -> 0x131F */
            CSR_BIT_SET(deviceProperties->knownServices13_00_31, serviceList[i] - 0x1300);
        }
    }

    CsrPmemFree(serviceList);
}
#endif

CsrUint16 CsrBtSdUtilInfoRemoveTag(SdMainInstance_t *mainInst, SdDeviceInfo_t **deviceInfo, CsrUint16 tag)
{
    /*
       a     b    c         a: removeTag - orgInfo
     +---+------+---+       b: removeTagLen
     |old|remove|old|       c: orgInfoLen - a - b
     +---+------+---+       1: orgInfo
     1   2      3   4       2: removeTag
     3: 2 + b  */
    CsrUint8 *removeTag;
    CsrUint16 removeTagLen = CsrBtSdInfoGetTag((*deviceInfo)->infoLen, (*deviceInfo)->info, &removeTag, tag);

    if (removeTagLen && removeTag)
    {
        /* The tag was found - remove it */
        CsrUint16 newInfoLen = (*deviceInfo)->infoLen - (removeTagLen + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE);
        CsrUint16 orgInfoLen = (*deviceInfo)->infoLen;
        CsrUint8 *orgInfo = (*deviceInfo)->info;
        CsrUint8 *newInfo = CsrPmemAlloc(newInfoLen);

        /* Adjust removeTag(Len) to include the header */
        removeTag -= CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE;
        removeTagLen += CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE;

        /* Copy the info before the old tag */
        CsrMemCpy(newInfo, orgInfo, (CsrUint32)(removeTag - orgInfo));

        /* Copy the info after the old tag */
        CsrMemCpy(newInfo + (removeTag - orgInfo), removeTag + removeTagLen, (CsrUint32)(orgInfoLen - (removeTag - orgInfo) - removeTagLen));

        /* Reallocate and insert the old info */
        csrBtSdUtilDIReallocate(mainInst, deviceInfo, newInfoLen, FALSE);
        CsrMemCpy((*deviceInfo)->info, newInfo, newInfoLen);
        CsrPmemFree(newInfo);
    }

    return removeTagLen;
}

void CsrBtSdUtilInfoUpdateTag(SdMainInstance_t *mainInst, SdDeviceInfo_t **deviceInfo, CsrUint16 tag, CsrUint16 valueLen, CsrUint8 *value)
{
    /* Generate new/updated info and insert in an reallocated element */
    CsrUint16 orgInfoLen = (*deviceInfo)->infoLen;
    CsrUint16 newInfoLen = valueLen + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE + orgInfoLen;
    CsrUint16 removeTagFullLen = CsrBtSdUtilInfoRemoveTag(mainInst, deviceInfo, tag);

    /* Remove the tag if it already exists and adjust the length of the new info */
    newInfoLen -= removeTagFullLen;

    /* Reallocate with room for new tag */
    csrBtSdUtilDIReallocate(mainInst, deviceInfo, newInfoLen, TRUE);

    /* Insert the new tag at the end */
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(valueLen + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE, (*deviceInfo)->info + orgInfoLen - removeTagFullLen);
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(tag, (*deviceInfo)->info + orgInfoLen - removeTagFullLen + CSR_BT_SD_DEVICE_INFO_FORMAT_TYPE_SIZE);
    CsrMemCpy((*deviceInfo)->info - removeTagFullLen + orgInfoLen + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE, value, valueLen);
}

void CsrBtSdUtilConvertEir2Info(CsrUint8 eirDataLen, CsrUint8 *eirData, CsrUint16 *infoLen, CsrUint8 *info)
{
    CsrUint16 eirIndex, infoIndex, tagLen;

    /* Validate EIR. Stop processing if invalid */
    if(!CsrBtSdUtilEirValidate(eirData, eirDataLen))
    {
        return;
    }

    /* Add two additional bytes per tag (since both length and type is
     * one byte longer) */
    *infoLen = eirDataLen + CsrBtSdUtilEirCountTags(eirDataLen, eirData) * 2;

    for (eirIndex = 0, infoIndex = 0;
         eirIndex < eirDataLen;
         infoIndex += eirData[eirIndex] + 3, eirIndex += eirData[eirIndex] + 1)
    {
        CsrUint16 data;

        tagLen = eirData[eirIndex] - 1;

        if (((eirData[eirIndex + 1] == CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME) ||
             (eirData[eirIndex + 1] == CSR_BT_EIR_DATA_TYPE_SHORT_LOCAL_NAME)) &&
            (tagLen > CSR_BT_MAX_FRIENDLY_NAME_LEN))
        {
            /* The name is too long. Calculate a infoLen again and set type to CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME */
            *infoLen                = *infoLen - (tagLen - CSR_BT_MAX_FRIENDLY_NAME_LEN);
            tagLen                  = CSR_BT_MAX_FRIENDLY_NAME_LEN;
            eirData[eirIndex + 1]   = CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME;
        }

        data = eirData[eirIndex + 1];

        /* Copy length, type, data */
        CSR_COPY_UINT16_TO_LITTLE_ENDIAN(tagLen + CSR_BT_SD_DEVICE_INFO_FORMAT_TYPE_SIZE, &(info[infoIndex]));
        CSR_COPY_UINT16_TO_LITTLE_ENDIAN(data, &(info[infoIndex + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE]));
        CsrMemCpy(&(info[infoIndex + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE]), &eirData[eirIndex + CSR_BT_EIR_TAG_HEADER_SIZE], tagLen);

        /* Device name is only UTF8 data we can possibly know
         * about. Do proper termination of string */
        if ((infoIndex + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE + tagLen) < *infoLen)
        {
            if ((eirData[eirIndex + 1] == CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME) ||
                (eirData[eirIndex + 1] == CSR_BT_EIR_DATA_TYPE_SHORT_LOCAL_NAME))
            {
                CsrUtf8StrTruncate((CsrUtf8String *)(&(info[infoIndex + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE])), tagLen);
            }
         }
    }
}

#ifdef CSR_BT_LE_ENABLE
void CsrBtSdUtilCombineTags(SdDeviceInfo_t *devInfo, CsrUint32 *length, CsrUint8 **data)
{
    CsrUint8 *tmp;
    CsrUint16 tmpLen;
    CsrUint16 srcIdx;
    CsrUint16 thisLen;

    *length = 0;
    *data = NULL;

    if(devInfo->infoLen + devInfo->advertLen + devInfo->scanrspLen == 0)
    {
        return;
    }
    tmp = CsrPmemZalloc(devInfo->infoLen + devInfo->advertLen + devInfo->scanrspLen);

    /* Always append classic EIR */
    tmpLen = devInfo->infoLen;
    CsrMemCpy(tmp, devInfo->info, tmpLen);
    
    /* Iterate advertise and copy non-existing bits */
    for(srcIdx=0;
        (srcIdx + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE) < devInfo->advertLen;
        srcIdx += (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(devInfo->advert + srcIdx)
                   + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE))
    {
        if(!CsrBtSdInfoCheckTag(tmpLen, tmp,
                                (CsrUint16)CSR_GET_UINT16_FROM_LITTLE_ENDIAN(
                                    devInfo->advert + srcIdx
                                    + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE)))
        {
            /* Tag isn't already in buffer so transfer it */
            thisLen = (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(devInfo->advert + srcIdx)
                       + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE);
            CsrMemCpy(tmp+tmpLen,
                      (devInfo->advert + srcIdx), 
                      thisLen);
            tmpLen += thisLen;
        }
    }

    /* Iterate scanrsp and copy non-existing bits */
    for(srcIdx=0;
        (srcIdx + CSR_BT_SD_DEVICE_INFO_FORMAT_HEADER_SIZE) < devInfo->scanrspLen;
        srcIdx += (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(devInfo->scanrsp + srcIdx)
                   + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE))
    {
        if(!CsrBtSdInfoCheckTag(tmpLen, tmp,
                                (CsrUint16)CSR_GET_UINT16_FROM_LITTLE_ENDIAN(
                                    devInfo->scanrsp + srcIdx
                                    + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE)))
        {
            /* Tag isn't already in buffer so transfer it */
            thisLen = (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(devInfo->scanrsp + srcIdx)
                       + CSR_BT_SD_DEVICE_INFO_FORMAT_LEN_SIZE);
            CsrMemCpy(tmp+tmpLen,
                      (devInfo->scanrsp + srcIdx),
                      thisLen);
            tmpLen += thisLen;
        }
    }

    /* Resize buffer if necessary */
    if(tmpLen != (devInfo->infoLen + devInfo->advertLen + devInfo->scanrspLen))
    {
        CsrUint8 *n;
        n = CsrMemDup(tmp, tmpLen);
        CsrPmemFree(tmp);
        tmp = n;
    }

    *data = tmp;
    *length = tmpLen;
}
#endif

CsrBool CsrBtSdUtilUuid32Check(SdMainInstance_t * mainInst, CsrBtUuid32 uuid32)
{
    CsrUint16 i;

    for (i = 0; i < mainInst->uuid32ListCount; i++)
    {
        if (uuid32 == mainInst->uuid32List[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}

void CsrBtSdUtilDICopy2DIType(SdDeviceInfo_t *deviceInfo, CsrBtSdDeviceInfoType *infoType)
{
    infoType->deviceAddr    = deviceInfo->addr;
    infoType->deviceClass   = deviceInfo->cod;
    infoType->deviceStatus  = CsrBtSdUtilGetDeviceStatus(deviceInfo);
    infoType->infoLen       = deviceInfo->infoLen;
    infoType->info          = (deviceInfo->infoLen > 0) ? CsrMemDup(deviceInfo->info, deviceInfo->infoLen) : NULL;
}

void CsrBtSdUtilGenerateReadDLInd(SdMainInstance_t *mainInst, CsrUint32 size, CsrUint32 config, CsrSchedQid phandle)
{
    SdDeviceInfo_t *currentDevice = SD_DL_GET_FIRST(mainInst->dl);
    CsrUint32 i, count, totalCount = 0;
    CsrBtSdDeviceInfoType *infoPtr;
    void *prim;
    CsrBool confSend = FALSE;

    do /* Always send at least a confirmation */
    {
        /* Determine which type of pritive to use */
        if (CsrBtSdUtilDLGetCountFromSize(mainInst, currentDevice, size, config, &count))
        {
            /* Room for all devices */
            CsrBtSdReadDeviceListCfm *cfm = CsrBtSdBuildReadDeviceListCfm(count, count + totalCount);
            infoPtr = cfm->deviceInfo;
            prim    = cfm;
            confSend = TRUE;
        }
        else
        {
            /* Not room for all devices */
            CsrBtSdReadDeviceListInd *ind = CsrBtSdBuildReadDeviceListInd(count);
            infoPtr     = ind->deviceInfo;
            prim        = ind;
            totalCount  += count;
        }

        /* Insert devices in primitive */
        i = 0;
        while ((i < count) && currentDevice)
        {
            if (!(CSR_MASK_IS_SET(config, CSR_BT_SD_DEVICE_EXCLUDE_PAIRED_DEVICES)
                  && CSR_MASK_IS_SET(currentDevice->devFlags, SD_DI_DEV_FLAG_PAIRED))
                && !(CSR_MASK_IS_SET(config, CSR_BT_SD_DEVICE_EXCLUDE_NON_PAIRED_DEVICES)
                     && !CSR_MASK_IS_SET(currentDevice->devFlags, SD_DI_DEV_FLAG_PAIRED)))
            {
                /* Device should be included - insert info */
                CsrBtSdUtilDICopy2DIType(currentDevice, &infoPtr[i++]);
            }
            currentDevice = currentDevice->next;
        }

        CsrBtSdMessagePut(phandle, prim);
    }
    while (currentDevice && !confSend && (count > 0));
}

/* To avoid reallocations for every new (GATT) service, allocate the
 * UUID list in chunks of SD_SS_LIST_INCREMENT */
void CsrBtSdResizeDiList(SdDeviceInfo_t *devInfo, CsrUint16 newCount)
{
    CsrUint16 new;
    CsrUint16 old;

    /* Calculate block sizes */
    old = (devInfo->listCount / SD_SS_LIST_INCREMENT) + 1;
    new = (newCount / SD_SS_LIST_INCREMENT) + 1;

    /* Resize needed? */
    if((new > old)
       || (devInfo->list == NULL))
    {
        CsrBtUuid *list;

        list = CsrPmemZalloc(new*SD_SS_LIST_INCREMENT*sizeof(CsrBtUuid));
        if((devInfo->listCount > 0) && (devInfo->list != NULL))
        {
            CsrMemCpy(list, devInfo->list, devInfo->listCount*sizeof(CsrBtUuid));
        }
        CsrPmemFree(devInfo->list);
        devInfo->list = list;
    }
}

void CsrBtSdUuidExpand128(CsrBtUuid *in, CsrBtUuid *out)
{
    CsrUintFast8 i;
    static const CsrUint8 base[] =
        { 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x10, 0x00,
          0x80, 0x00, 0x00, 0x80,
          0x5f, 0x9b, 0x34, 0xfb };

    out->length = CSR_BT_UUID128_SIZE;
    for(i=0; i<in->length; i++)
    {
        out->uuid[i] = in->uuid[i];
    }
    for(i=in->length; i<CSR_BT_UUID128_SIZE; i++)
    {
        out->uuid[i] = base[i];
    }
}

CsrBool CsrBtSdUuidEqual(CsrBtUuid *a, CsrBtUuid *b)
{
    CsrBtUuid ea, eb;

    /* Convert to full 128 bit uuid */
    CsrBtSdUuidExpand128(a, &ea);
    CsrBtSdUuidExpand128(b, &eb);

    return (CsrBool)(CsrMemCmp(ea.uuid, eb.uuid, sizeof(CsrBtUuid128)) == 0
                     ? TRUE
                     : FALSE);
}
