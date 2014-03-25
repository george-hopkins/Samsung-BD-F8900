/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
            All rights reserved.

REVISION:      $Revision: #6 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"

static void csrBtSdGattDisconnect(SdMainInstance_t *mainInst,
                                  SdDeviceInfo_t *deviceInfo)
{
    if(deviceInfo->btConnId != CSR_BT_CONN_ID_INVALID)
    {
        CsrBtGattDisconnectReqSend(mainInst->gattId,
                                   deviceInfo->btConnId);
    }
}

static void csrBtSdGattConnect(SdMainInstance_t *mainInst,
                               SdDeviceInfo_t *deviceInfo,
                               CsrBool exclusive)
{
    /* Bring up connection */
    CsrBtTypedAddr addr;
    CsrUint16 tagLen;
    CsrUint8 *tag = NULL;
    CsrBool bredr;
    
    /* Assume BR/EDR connection, then check if we can use LE */
    bredr = TRUE;
    if(deviceInfo->advertLen > 0)
    {
        tagLen = CsrBtSdInfoGetTag(deviceInfo->advertLen, deviceInfo->advert,
                                   &tag, CSR_BT_EIR_DATA_TYPE_FLAGS);
        if((tagLen == 1)
           && (tag != NULL)
           && (tag[0] & CSR_BT_EIR_FLAG_LE_BREDR_NOT_SUPPORTED))
        {
            /* LE-only. Don't connect over BR/EDR */
            bredr = FALSE;
        }
    }

    /* BR/EDR disabled, force LE */
    if(mainInst->serviceConfig & CSR_BT_SD_SEARCH_DISABLE_BT_CLASSIC)
    {
        bredr = FALSE;
    }

    /* Set address and type */
    addr.addr = deviceInfo->addr;
    addr.type = ((deviceInfo->devFlags & SD_DI_DEV_FLAG_LE_PRIVATE_ADDR)
                 ? CSR_BT_ADDR_PRIVATE
                 : CSR_BT_ADDR_PUBLIC);

    if(bredr)
    {
        CsrBtGattBredrConnectReqSend(mainInst->gattId,
                                     addr,
                                     CSR_BT_GATT_FLAGS_NONE);
    }
    else
    {
        CsrBtGattCentralReqSend(mainInst->gattId,
                                addr,
                                CSR_BT_GATT_FLAGS_NONE,
                                0);
    }
}

static void csrBtSdGattServiceSearchContinue(SdMainInstance_t *mainInst,
                                             CsrBtResultCode result,
                                             CsrBtSupplier supplier)
{
    /* GATT service search finished. Finalise the entire procedure */
    csrBtSdGattDisconnect(mainInst, mainInst->currentDevice);

    if (CSR_MASK_IS_SET(mainInst->serviceConfig, CSR_BT_SD_SERVICE_NO_NAME_UPDATE))
    {
        /* If list isn't empty this procedure hasn't failed even if
         * the LE-only part failed... */
        if(mainInst->currentDevice->listCount > 0)
        {
            result = CSR_BT_RESULT_CODE_SD_SUCCESS;
            supplier = CSR_BT_SUPPLIER_SD;
        }

        /* No need to update name, inform the application */
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
        CsrBtSdSendReadServicesCfm(mainInst,
                                   mainInst->currentDevice,
                                   result,
                                   supplier);
        CsrBtSdUtilGo2Idle(mainInst);
    }
    else
    {
        /* Still need to update the remote name */
        (void)CsrBtSdUtilReadRemoteNameStart(mainInst, mainInst->currentDevice);
    }
}

static void csrBtSdGattDisconnectIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattDisconnectInd *prim;
    prim = (CsrBtGattDisconnectInd*)mainInst->recvMsgP;

    if(mainInst->currentDevice
       && (mainInst->currentDevice->btConnId == prim->btConnId))
    {
        /* Fail operation now that link died */
        if (mainInst->processState == SD_STATE_PROC_RRN)
        {
            CsrBtSdCmReadRemoteNameContinue(mainInst);
        }
        else if (mainInst->processState == SD_STATE_PROC_SDC_SEARCH)
        {
            csrBtSdGattServiceSearchContinue(mainInst,
                                             CSR_BT_SUPPLIER_SD,
                                             CSR_BT_RESULT_CODE_SD_CONNECTION_TERMINATED);
        }
        else if ((mainInst->processState == SD_STATE_PROC_IDLE) || 
                 (mainInst->processState == SD_STATE_PROC_SDC_SEARCH_CANCEL) ||
                 (mainInst->processState == SD_STATE_PROC_SET_EVENT_TO_STOP) ||
                 (mainInst->processState == SD_STATE_PROC_CRRN_TO_STOP))
        {/* This is due to a search that has been cancelled: make sure to unlock the SD queue! */
            SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_IDLE);
            CsrBtSdUtilGo2Idle(mainInst);
        }
        mainInst->currentDevice->btConnId = CSR_BT_CONN_ID_INVALID;
    }
}

static void csrBtSdGattReportIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattReportInd *prim;
    SdDeviceInfo_t *deviceInfo;

    prim = (CsrBtGattReportInd*)mainInst->recvMsgP;
    deviceInfo = CsrBtSdUtilDLAddCmReportRes(mainInst, prim);

    /* Reuse standard report validator */
    CsrBtSdSearchValidateResult(mainInst, deviceInfo);
}

static void csrBtSdGattScanCfmHandler(SdMainInstance_t *mainInst)
{
    /* Ignore: CsrBtGattScanCfm *prim =
     * (CsrBtGattScanCfm*)mainInst->recvMsgP; */
}

static void csrBtSdGattRegisterCfmHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattRegisterCfm *cfm;
    cfm = (CsrBtGattRegisterCfm*)mainInst->recvMsgP;

    if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS &&
       cfm->resultSupplier == CSR_BT_SUPPLIER_GATT)
    {
        mainInst->gattId = cfm->gattId;
    }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
    else
    {
        CsrGeneralException("SD", mainInst->appState,
                            CSR_BT_GATT_REGISTER_CFM,
                            "GATT registration failed");
    }
#endif
}

static void csrBtSdGattCentralCfmHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattStdBtConnIdCfm *prim;
    prim = (CsrBtGattStdBtConnIdCfm*)mainInst->recvMsgP;

    if(prim->resultCode == CSR_BT_GATT_RESULT_SUCCESS
       && prim->resultSupplier == CSR_BT_SUPPLIER_GATT)
    {
        mainInst->currentDevice->btConnId = prim->btConnId;
        /* All processes at this point wait for the connect-ind */
    }
    else
    {
        if (mainInst->processState == SD_STATE_PROC_RRN)
        {
            CsrBtSdCmReadRemoteNameContinue(mainInst);
        }
        else if(mainInst->processState == SD_STATE_PROC_SDC_SEARCH)
        {
            csrBtSdGattServiceSearchContinue(mainInst,
                                             prim->resultCode,
                                             prim->resultSupplier);
        }
    }
}

static void csrBtSdGattConnectIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattConnectInd *prim;
    prim = (CsrBtGattConnectInd*)mainInst->recvMsgP;

    if(prim->resultCode == CSR_BT_GATT_RESULT_SUCCESS
       && prim->resultSupplier == CSR_BT_SUPPLIER_GATT)
    {
        CsrBtGattReadBy16BitUuidReqSend(mainInst->gattId,
                                        mainInst->currentDevice->btConnId,
                                        0x0001, 0xffff, /* start,end */
                                        CSR_BT_GATT_DEVICE_NAME_UUID);
    }
    else
    {
        if (mainInst->processState == SD_STATE_PROC_RRN)
        {
            CsrBtSdCmReadRemoteNameContinue(mainInst);
        }
        else if (mainInst->processState == SD_STATE_PROC_SDC_SEARCH)
        {
            csrBtSdGattServiceSearchContinue(mainInst,
                                             prim->resultCode,
                                             prim->resultSupplier);
        }
        else
        {
            /* Spurious event. Disconnect. */
            csrBtSdGattDisconnect(mainInst, mainInst->currentDevice);
        }
    }
}

/* Service discovery result handler */
void csrBtSdGattDiscoverServicesIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattDiscoverServicesInd *prim;
    prim = (CsrBtGattDiscoverServicesInd*)mainInst->recvMsgP;

    if (mainInst->processState == SD_STATE_PROC_SDC_SEARCH)
    {
        /* Filter away duplicates */
        CsrUintFast16 i;
        CsrBool add;
        CsrBtUuid *list;

        list = mainInst->currentDevice->list;
        add = TRUE;
        for(i=0; i<mainInst->currentDevice->listCount; i++)
        {
            if(CsrBtSdUuidEqual(&(list[i]), &(prim->uuid)))
            {
                add = FALSE;
            }
        }

        /* If not duplicate, do chunky resize of UUID list + store
         * number */
        if(add)
        {
            CsrBtSdResizeDiList(mainInst->currentDevice,
                                (CsrUint16)(mainInst->currentDevice->listCount+1));
            CsrMemCpy(&(mainInst->currentDevice->list[mainInst->currentDevice->listCount]),
                      &prim->uuid,
                      sizeof(CsrBtUuid));
            mainInst->currentDevice->listCount++;
        }
    }
}

/* Service discovery complete handler */
void csrBtSdGattDiscoverServicesCfmHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattDiscoverServicesCfm *prim;
    prim = (CsrBtGattDiscoverServicesCfm*)mainInst->recvMsgP;
    CSR_UNUSED(prim);

    if(mainInst->processState == SD_STATE_PROC_SDC_SEARCH)
    {
        csrBtSdGattServiceSearchContinue(mainInst,
                                         CSR_BT_SUPPLIER_SD,
                                         CSR_BT_RESULT_CODE_SD_SUCCESS);
    }
    else
    {
        /* Spurious event. Disconnect. */
        csrBtSdGattDisconnect(mainInst, mainInst->currentDevice);
    }
}

void csrBtSdGattReadByUuidIndHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattReadByUuidInd *prim;

    prim = (CsrBtGattReadByUuidInd*)mainInst->recvMsgP;

    if(mainInst->processState == SD_STATE_PROC_RRN)
    {
        /* Store new name. Spec guarantees (yeah right!) that only
         * one device name exists, and that this is the full name,
         * so completely replace old/partial name */
        CsrBtSdUtilInfoRemoveTag(mainInst,
                                 &mainInst->currentDevice,
                                 CSR_BT_SD_DEVICE_INFO_TYPE_NAME_SHORT);
        CsrBtSdUtilInfoUpdateTag(mainInst,
                                 &mainInst->currentDevice,
                                 CSR_BT_SD_DEVICE_INFO_TYPE_NAME_FULL,
                                 prim->valueLength,
                                 prim->value);
        CSR_MASK_SET(mainInst->currentDevice->devFlags, SD_DI_DEV_FLAG_NAME_UPDATED);
        CsrBtSdUtilUpdateTrustedDeviceList(mainInst->currentDevice);
    }
}

static void csrBtSdGattReadByUuidCfmHandler(SdMainInstance_t *mainInst)
{
    /* Read by UUID (device name) completed. Close connection and stop
     * RRN */
    csrBtSdGattDisconnect(mainInst, mainInst->currentDevice);
    CsrBtSdCmReadRemoteNameContinue(mainInst);
}

void CsrBtSdGattHandler(SdMainInstance_t *mainInst)
{
    CsrBtGattPrim primType = *(CsrBtGattPrim*)mainInst->recvMsgP;
    switch(primType)
    {
        case CSR_BT_GATT_REGISTER_CFM:
            csrBtSdGattRegisterCfmHandler(mainInst);
            break;

        case CSR_BT_GATT_SCAN_CFM:
            csrBtSdGattScanCfmHandler(mainInst);
            break;

        case CSR_BT_GATT_REPORT_IND:
            csrBtSdGattReportIndHandler(mainInst);
            break;
            
        case CSR_BT_GATT_CENTRAL_CFM:
            /* fallthrough */
        case CSR_BT_GATT_BREDR_CONNECT_CFM:
            csrBtSdGattCentralCfmHandler(mainInst);
            break;

        case CSR_BT_GATT_CONNECT_IND:
            csrBtSdGattConnectIndHandler(mainInst);
            break;

        case CSR_BT_GATT_DISCONNECT_IND:
            csrBtSdGattDisconnectIndHandler(mainInst);
            break;

        case CSR_BT_GATT_DISCOVER_SERVICES_CFM:
            csrBtSdGattDiscoverServicesCfmHandler(mainInst);
            break;

        case CSR_BT_GATT_DISCOVER_SERVICES_IND:
            csrBtSdGattDiscoverServicesIndHandler(mainInst);
            break;

        case CSR_BT_GATT_READ_BY_UUID_IND:
            csrBtSdGattReadByUuidIndHandler(mainInst);
            break;

        case CSR_BT_GATT_READ_BY_UUID_CFM:
            csrBtSdGattReadByUuidCfmHandler(mainInst);
            break;
        case CSR_BT_GATT_SERVICE_CHANGED_IND:
            break;
        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrGeneralException("SD", mainInst->appState, primType, "Unknown GATT prim or undefined state");
#endif
            break;
    }
}

void CsrBtSdGattGetNameStart(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo)
{
    csrBtSdGattConnect(mainInst, deviceInfo, TRUE);
}

void CsrBtSdGattGetNameStop(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo)
{
    /* Disconnect. */
    csrBtSdGattDisconnect(mainInst, deviceInfo);
}

CsrBool CsrBtSdGattServiceSearchStart(SdMainInstance_t *mainInst, SdDeviceInfo_t *deviceInfo)
{
    CsrBool ok = FALSE;

    /* Start GATT search if we've seen AD/SR data, or BR/EDR has been
     * disabled */
    if(CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_ADVERT)
       || CSR_MASK_IS_SET(deviceInfo->devFlags, SD_DI_DEV_FLAG_SCANRSP)
       || (mainInst->serviceConfig & CSR_BT_SD_SEARCH_DISABLE_BT_CLASSIC))
    {
        ok = TRUE;
    }
    else
    {
        CsrUintFast16 i;
        for(i=0; i<deviceInfo->listCount; i++)
        {
            if((deviceInfo->list[i].length == CSR_BT_UUID16_SIZE
                && CSR_BT_UUID_GET_16(deviceInfo->list[i]) == CSR_BT_GENERIC_ATTRIBUTE_PROFILE_UUID)
               || (deviceInfo->list[i].length == CSR_BT_UUID32_SIZE
                   && CSR_BT_UUID_GET_32(deviceInfo->list[i]) == CSR_BT_GENERIC_ATTRIBUTE_PROFILE_UUID))
            {
                ok = TRUE;
                break;
            }
        }
    }
    
    if(ok)
    {
        SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SDC_SEARCH);
        SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_OTHER);
        csrBtSdGattConnect(mainInst, deviceInfo, FALSE);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

