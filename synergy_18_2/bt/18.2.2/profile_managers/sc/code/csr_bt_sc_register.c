/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_sched.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_oob.h"
#ifdef CSR_BT_LE_ENABLE
#include "csr_bt_sc_le.h"
#endif

void getUuidOfRegisteredChannel(ScInstanceData_t *scData,
                                      CsrUint16 * service)
{
    ScServiceRecordType * currentServiceList;

    if (scData->protocolId == SEC_PROTOCOL_RFCOMM)
    {
        currentServiceList = scData->rfcommServices;
    }
    else
    {
        currentServiceList = scData->l2capServices;
    }

    if (currentServiceList)
    {
        CsrUintFast16            i;
        do
        {
            for ( i = 0; i < SC_SERVICE_POOL_SIZE; i++)
            {
                if (currentServiceList->serviceRegister[i].channel == scData->channel &&
                    currentServiceList->serviceRegister[i].inUse)
                {
                    *service = currentServiceList->serviceRegister[i].serviceUuid;
                    return;
                }
            }
            currentServiceList    = currentServiceList->nextQueuePool;
        } while (currentServiceList);
        *service = 0;
    }
    else
    {
        *service = 0;
    }
}

static void csrBtScInsertElementInServiceRecordList(ScInstanceData_t  *scData,
                                                           dm_protocol_id_t  protocolId,
                                                           scServiceRecord_t **returnElement)
{
    CsrUintFast16            i;
    ScServiceRecordType * theServiceList;

    if (protocolId == SEC_PROTOCOL_RFCOMM)
    {
        theServiceList = scData->rfcommServices;
    }
    else
    {
        theServiceList = scData->l2capServices;
    }

    if (theServiceList)
    {
        ScServiceRecordType * previousElement;
        do
        {
            if (theServiceList->numberOfMessagesInThisPool < SC_SERVICE_POOL_SIZE)
            {
                for (i = 0; i < SC_SERVICE_POOL_SIZE; i++)
                {
                    if(theServiceList->serviceRegister[i].inUse == FALSE)
                    {
                        theServiceList->numberOfMessagesInThisPool++;
                        *returnElement = &(theServiceList->serviceRegister[i]);
                        return;
                    }
                }
            }
            previousElement = theServiceList;
            theServiceList  = theServiceList->nextQueuePool;
        }while (theServiceList);

        theServiceList                  = (ScServiceRecordType *) CsrPmemAlloc(sizeof(ScServiceRecordType));
        previousElement->nextQueuePool  = theServiceList;
    }
    else
    {
        if (protocolId == SEC_PROTOCOL_RFCOMM)
        {
            scData->rfcommServices   = (ScServiceRecordType *) CsrPmemAlloc(sizeof(ScServiceRecordType));
            theServiceList           = scData->rfcommServices;
        }
        else
        {
            scData->l2capServices    = (ScServiceRecordType *) CsrPmemAlloc(sizeof(ScServiceRecordType));
            theServiceList           = scData->l2capServices;
        }
    }
    theServiceList->nextQueuePool               = NULL;
    theServiceList->numberOfMessagesInThisPool  = 1;
    for (i = 0; i < SC_SERVICE_POOL_SIZE; i++)
    {
        theServiceList->serviceRegister[i].inUse        = FALSE;
        theServiceList->serviceRegister[i].serviceUuid  = 0;
        theServiceList->serviceRegister[i].channel      = 0;
    }
    *returnElement = &(theServiceList->serviceRegister[0]);
}

static void csrBtScRemoveElementInServiceRecordList(ScInstanceData_t *scData,
                                                              dm_protocol_id_t  protocolId,
                                                              CsrUint32          channel)
{
    ScServiceRecordType * theServiceList;

    if (protocolId == SEC_PROTOCOL_RFCOMM)
    {
        theServiceList = scData->rfcommServices;
    }
    else
    {
        theServiceList = scData->l2capServices;
    }

    if (theServiceList)
    {
        CsrUintFast16            i;
        ScServiceRecordType * previousServiceList;

        previousServiceList = NULL;
        do
        {
            for (i = 0; i < SC_SERVICE_POOL_SIZE; i++)
            {
                if (theServiceList->serviceRegister[i].channel == channel &&
                    theServiceList->serviceRegister[i].inUse)
                {
                    theServiceList->numberOfMessagesInThisPool--;
                    theServiceList->serviceRegister[i].channel      = 0;
                    theServiceList->serviceRegister[i].inUse        = FALSE;
                    theServiceList->serviceRegister[i].serviceUuid  = 0;

                    if (theServiceList->numberOfMessagesInThisPool == 0)
                    {
                       if (theServiceList == scData->rfcommServices ||
                           theServiceList == scData->l2capServices )
                       { /* The element is place first */
                          if (protocolId == SEC_PROTOCOL_RFCOMM)
                          {
                               scData->rfcommServices = theServiceList->nextQueuePool;
                          }
                          else
                          {
                               scData->l2capServices  = theServiceList->nextQueuePool;
                          }
                       }
                       else if (previousServiceList)
                       {
                            if(theServiceList->nextQueuePool == NULL)
                            {/* This element is placed at the end of the linked list */
                                previousServiceList->nextQueuePool = NULL;
                            }
                            else
                            {/* This element is placed in the middle of the linked list */
                                previousServiceList->nextQueuePool = theServiceList->nextQueuePool;
                            }
                        }
                        CsrPmemFree(theServiceList);
                    }
                    return;
                }
            }
            previousServiceList = theServiceList;
            theServiceList      = theServiceList->nextQueuePool;
        } while (theServiceList);
    }
    else
    {
        ;/* nothing to remove */
    }
}

CsrUint8 CsrBtScGetHighestAuthRequirement(ScInstanceData_t *scData)
{
    const ScServiceRecordType * serviceLists[2];
    CsrUintFast8 i, j;

    serviceLists[0] = scData->rfcommServices;
    serviceLists[1] = scData->l2capServices;

    for (j = 0; j < CSR_ARRAY_SIZE(serviceLists); j++)
    {
        const ScServiceRecordType * theServiceList = serviceLists[j];
        for (i = 0; theServiceList && i < SC_SERVICE_POOL_SIZE; i++)
        {
            if (theServiceList->serviceRegister[i].inUse)
            {
                if (theServiceList->serviceRegister[i].secLevel & SECL4_IN_MITM)
                {
                    return HCI_MITM_REQUIRED_GENERAL_BONDING;
                }
            }
        }
    }

    return HCI_MITM_NOT_REQUIRED_GENERAL_BONDING;
}

CsrUint8 CsrBtScGetCompatibleAuthRequirement(ScInstanceData_t *scData)
{
#ifdef CSR_BT_LE_ENABLE
    if (scData->bondData.lowEnergy)
    {
        CsrUint8 req;

        /* Only least significant 8 bits needed (the most significant
         * are internal bits which aren't used in the IO caps
         * exchange) */
        req = (CsrUint8)(scData->bondData.leLocalAuth & 0xFF);
        
        /* Key distribution toggles bonding-mode */
        if(scData->bondData.keyDistribution != 0)
        {
            req |= DM_SM_SECURITY_BONDING;
        }
        else
        {
            req &= ~DM_SM_SECURITY_BONDING;
        }

        if (((scData->bondData.remoteAuth & DM_SM_SECURITY_MITM_PROTECTION) > 0) && 
            CsrBtScRemoteAuthValid(scData->bondData.remoteAuth))
        {
            CsrUint8 msgTypeByIoCaps;

            msgTypeByIoCaps = CsrBtScGetAssocModel(CsrBtScIoLimitier(scData, scData->bondData.localIoCaps),
                                                   CsrBtScIoLimitier(scData, scData->bondData.remoteIoCaps));
            if (msgTypeByIoCaps != MSG_TYPE_JUSTWORKS)
            {
                req |= DM_SM_SECURITY_MITM_PROTECTION;
            }
        }

        return req;
    }
    else
#endif
    if (scData->state == SC_ST_REMOTE_BONDING)
    {
        if (scData->bondData.remoteAuth > HCI_MITM_PROTECTION_MAX)
        {
            /* Enforcing of outgoing connection. In this case our choice is overruled by the DM */
            return HCI_MITM_NOT_REQUIRED_GENERAL_BONDING;
        }
        else
        {
            CsrUint8 auth = CsrBtScGetHighestAuthRequirement(scData);
            CsrUint8 msgTypeByIoCaps;

            msgTypeByIoCaps = CsrBtScGetAssocModel(CsrBtScIoLimitier(scData, scData->bondData.localIoCaps),
                                                   CsrBtScIoLimitier(scData, scData->bondData.remoteIoCaps));
            
            if ((msgTypeByIoCaps != MSG_TYPE_JUSTWORKS ||
#ifdef CSR_BT_INSTALL_SC_OOB
                 CsrBtScHasOobDataFor(scData, scData->deviceAddr) ||
#endif
                 scData->sspData.remoteOobDataPresent)
                && (auth & 0x1))
            {
                /* We can do MITM and this is required by at least one service */
                return HCI_MITM_REQUIRED_GENERAL_BONDING;
            }
            else
            {
                return HCI_MITM_NOT_REQUIRED_GENERAL_BONDING;
            }
        }
    }
    else if (scData->state == SC_ST_REBOND)
    {
        /* Rebond is always outgoing. In this case our choice is overruled by the DM */
        return HCI_MITM_NOT_REQUIRED_GENERAL_BONDING;
    }
    else
    {
        return scData->bondData.localAuth;
    }
}

/*************************************************************************************
    Register a new service in the SC for incoming service, i.e. where the local
    device is server
 ************************************************************************************/
void CsrBtScRegisterReqAnyS(ScInstanceData_t *scData)
{
    scServiceRecord_t   * serviceElement;
    CsrBtScRegisterReq   * scPrim;

    scPrim = (CsrBtScRegisterReq *) scData->recvMsgP;

    csrBtScInsertElementInServiceRecordList(scData, scPrim->protocolId, &serviceElement);
    serviceElement->channel         = scPrim->channel;
    serviceElement->serviceUuid     = scPrim->profileUuid;
    serviceElement->inUse           = TRUE;
    serviceElement->secLevel        = scPrim->secLevel;

    /*  tell SM that a new service should be registered */
    CsrBtCmScDmRegisterReq(scPrim->protocolId, scPrim->channel, scPrim->outgoingOk, scPrim->secLevel, 0);
}

/*************************************************************************************
    Remove a previous registration from the list of services
 ************************************************************************************/
void CsrBtScDeregisterReqAnyS(ScInstanceData_t *scData)
{
    CsrBtScDeregisterReq     *scPrim;

    scPrim = (CsrBtScDeregisterReq *) scData->recvMsgP;
    /*  remove the registration in SC */
    csrBtScRemoveElementInServiceRecordList(scData, scPrim->protocolId, scPrim->channel);

    /*  tell SM that the service should be deregistered */
    CsrBtCmScDmUnRegisterReq(scPrim->channel, scPrim->protocolId);
}

/*************************************************************************************
  Change the security level
 ************************************************************************************/
#ifdef CSR_BT_INSTALL_SC_SECURITY_MODE
void CsrBtScSetSecurityModeReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScSetSecurityModeReq *scPrim;

    scPrim = (CsrBtScSetSecurityModeReq *) scData->recvMsgP;

    scData->appHandle      = scPrim->phandle;
    SC_CHANGE_STATE(&scData->state, SC_ST_PENDING);
    scData->restoreMsgFlag = FALSE;

    SC_CHANGE_STATE(&scData->dmSmInitState, SM_INIT_ST_MODE);
    CsrBtCmScDmSetSecModeReq(scPrim->securityMode, scData->encryptionMode);
}
#endif
/*************************************************************************************
 * Response from the SM for the init request (security mode setting)
 *
 * This can be used both for updating the config paramters like WAE
 * and other tweaks, setting the mode and toggling debug mode.
 ************************************************************************************/
void CsrBtScDmSmInitCfmPendingS(ScInstanceData_t *scData)
{
#if defined(CSR_BT_LE_ENABLE) || defined(CSR_BT_INSTALL_SC_SECURITY_MODE) || defined(CSR_BT_INSTALL_SC_DEBUG_MODE)
    DM_SM_INIT_CFM_T *dmPrim;
    dmPrim = (DM_SM_INIT_CFM_T *) scData->recvMsgP;
#endif
    CsrBtScStateEventExceptionOn(((DM_SM_INIT_CFM_T *) scData->recvMsgP)->config != scData->sspData.config,
                                 DM_PRIM, scData->sspData.config, scData->state);

    /* Low Energy requires a few persistent local keys. Write these
     * now */
#ifdef CSR_BT_LE_ENABLE
        CsrBtScLeUpdateLocalDbKeys(dmPrim);
#endif

    switch(scData->dmSmInitState)
    {
#ifdef CSR_BT_INSTALL_SC_SECURITY_MODE
        case SM_INIT_ST_MODE:
            {
                CsrBtResultCode result;
                result = (CsrBtResultCode)((dmPrim->status == 0)
                                           ? CSR_BT_RESULT_CODE_SC_SUCCESS
                                           : CSR_BT_RESULT_CODE_SC_SECURITY_MODE_NOT_SET);

                if(result == CSR_BT_RESULT_CODE_SC_SUCCESS)
                {
                    scData->securityMode   = dmPrim->security_mode;
                    scData->encryptionMode = dmPrim->mode3_enc;
                }

                CsrBtScSetSecurityModeCfmSend(scData->appHandle, result, CSR_BT_SUPPLIER_SC);

                /* Mode has changed completely - force reinit */
                CsrBtScResetInstData(scData);
            }
            break;
#endif
#ifdef CSR_BT_INSTALL_SC_DEBUG_MODE
        case SM_INIT_ST_DEBUG:
            {
                scData->sspData.config = dmPrim->config;
                CsrBtScDebugModeCfmSend(scData->appHandle,
                                        (CsrBtResultCode) dmPrim->status,
                                        CSR_BT_SUPPLIER_DM,
                                        (CsrBool) ((dmPrim->config & DM_SM_SEC_MODE_CONFIG_DEBUG_MODE) != 0));

                /* Mode has changed completely - force reinit */
                CsrBtScResetInstData(scData);
            }
            break;
#endif
#ifdef CSR_BT_INSTALL_SC_CONFIG_MASK
        case SM_INIT_ST_CONFIG:
            {
                /* We're now ready again */
                SC_CHANGE_STATE(&scData->state, SC_ST_IDLE);
                CsrBtScGo2Idle(scData);
            }
            break;
#endif
        case SM_INIT_ST_IDLE:
            /* fallthrough */
        default:
            /* Never expect idle or other values */
            CsrBtScStateEventException(CSR_BT_SC_PRIM,
                                       *(CsrUint16*)scData->recvMsgP,
                                       scData->state);
            break;
    }

    /* DM_SM_INIT state is now idle */
    SC_CHANGE_STATE(&scData->dmSmInitState, SM_INIT_ST_IDLE);
}

void CsrBtScReadDeviceRecordReqHandler(ScInstanceData_t *scData)
{
    CsrBtScReadDeviceRecordReq * prim;
    CsrBtScDbDeviceRecord      deviceRecord;
    CsrBtDevicePropertiesType  * deviceProperties;
    CsrUint32                  * serviceChangedList = NULL;     
    CsrBtDeviceAddr addrFF;
    CsrUint32            numOfDevices;
    CsrUint32            t               = 0;
    CsrUint32            totalNumber     = 0;

    deviceProperties    = NULL;
    prim                = (CsrBtScReadDeviceRecordReq *) scData->recvMsgP;

    numOfDevices = (prim->maxNumOfBytes / sizeof(CsrBtDevicePropertiesType));

    if (numOfDevices == 0)
    {
        numOfDevices = 1;
    }
    
    addrFF.nap = 0xFFFF;
    addrFF.uap = 0xFF;
    addrFF.lap = 0xFFFFFF;

    if (CsrBtScDbReadFirst(&deviceRecord))
    {
        do
        {
            /* Skip all-0xFF address, which is used for internal storage */
            if (CSR_BT_BD_ADDR_EQ(deviceRecord.deviceAddr, addrFF))
            {
                continue;
            }

            if (!deviceProperties)
            {
                deviceProperties = (CsrBtDevicePropertiesType *) CsrPmemAlloc(sizeof(CsrBtDevicePropertiesType) * numOfDevices);
#ifdef CSR_BT_LE_ENABLE
                if (prim->flags == CSR_BT_SC_ADD_HDL_PAIRS)
                {
                    serviceChangedList = (CsrUint32 *) CsrPmemAlloc(sizeof(CsrUint32) * numOfDevices);
                }
#endif 
            }

            CsrBtScCopyDeviceProperties(&deviceProperties[t],&deviceRecord);
#ifdef CSR_BT_LE_ENABLE
            if (serviceChangedList)
            {
                serviceChangedList[t] = deviceRecord.serviceChangedHandle;
            }
#endif
            t++;
            totalNumber++;

            if (t >= numOfDevices)
            {
                CsrBtScReadDeviceRecordIndSend(prim->phandle, deviceProperties, t, serviceChangedList);
                serviceChangedList = NULL;
                deviceProperties = NULL;
                t                = 0;
            }
        } while (CsrBtScDbReadNext(&deviceRecord));

    }

    CsrBtScReadDeviceRecordCfmSend(prim->phandle, deviceProperties, t, totalNumber, serviceChangedList);
}

void CsrBtScUpdateDeviceReqHandler(ScInstanceData_t *scData)
{
    CsrBtScUpdateDeviceReq  *prim;
    CsrBtScDbDeviceRecord  deviceRecord;

    prim = (CsrBtScUpdateDeviceReq *) scData->recvMsgP;
    CsrMemSet(&deviceRecord, 0, sizeof(CsrBtScDbDeviceRecord));

    /* This is necessary to preserve the linkkey */
    if (CsrBtScDbRead(&prim->deviceAddr, &deviceRecord))
    {
        CsrUtf8StrNCpy(deviceRecord.remoteName, prim->remoteName, CSR_BT_MAX_FRIENDLY_NAME_LEN + 1);
        CsrUtf8StrTruncate(deviceRecord.remoteName, CSR_BT_MAX_FRIENDLY_NAME_LEN);
        
        if(prim->classOfDevice != 0)
        {
            deviceRecord.classOfDevice      = prim->classOfDevice;
        }
        deviceRecord.deviceAddr             = prim->deviceAddr;
        deviceRecord.knownServices11_00_31  = prim->knownServices11_00_31;
        deviceRecord.knownServices11_32_63  = prim->knownServices11_32_63;
        deviceRecord.knownServices12_00_31  = prim->knownServices12_00_31;
        deviceRecord.knownServices13_00_31  = prim->knownServices13_00_31;
        deviceRecord.authorised             = prim->authorised;

        CsrBtScDbWrite(&deviceRecord.deviceAddr, &deviceRecord);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB,
                              "IGNORE %04X:%02X:%06X",
                              deviceRecord.deviceAddr.nap,
                              deviceRecord.deviceAddr.uap,
                              deviceRecord.deviceAddr.lap));
    }

    CsrBtScFreeDownstreamMessageContents(CSR_BT_SC_PRIM, prim);
}

#ifdef CSR_BT_INSTALL_SC_CONFIG_MASK
void CsrBtScConfigReqHandler(ScInstanceData_t *scData)
{
    CsrBtScConfigReq  *prim = (CsrBtScConfigReq *) scData->recvMsgP;
    CsrBtScConfigMask old;

    old = scData->configMask;
    scData->configMask = prim->configMask;

    /* Some flags update the SSP config towards DM_SM - but only do so
     * if anything really changed. XOR is TRUE on non-equal bits */
    if((old ^ scData->configMask) & CSR_BT_SC_CONFIG_MASK_ALWAYS_TRY_MITM)
    {
        if(scData->configMask & CSR_BT_SC_CONFIG_MASK_ALWAYS_TRY_MITM)
        {
            scData->sspData.config |= DM_SM_SEC_MODE_CONFIG_ALWAYS_TRY_MITM;
        }
        else
        {
            scData->sspData.config -= (CsrUint16)(scData->sspData.config & DM_SM_SEC_MODE_CONFIG_ALWAYS_TRY_MITM);
        }

        SC_CHANGE_STATE(&scData->state, SC_ST_PENDING);
        SC_CHANGE_STATE(&scData->dmSmInitState, SM_INIT_ST_CONFIG);
        CsrBtCmScDmSecModeConfigReq(scData->sspData.writeAuthEnable,
                                    scData->sspData.config);
    }
}
#endif
void CsrBtScEncryptionKeySizeReqHandler(ScInstanceData_t *scData)
{
    CsrBtScEncryptionKeySizeReq *req;
    CsrBtScDbDeviceRecord db;

    req = (CsrBtScEncryptionKeySizeReq*)scData->recvMsgP;
    CsrMemSet(&db, 0, sizeof(CsrBtScDbDeviceRecord));

    /* Encryption key size is only currently supported on LE, however,
     * it may make sense to expand this to BR/EDR some day */
#ifdef CSR_BT_LE_ENABLE
    if(CsrBtAddrEqWithType(&(req->address), scData->addressType, &(scData->deviceAddr)))
    {
        /* Use memory cache */
        CsrBtScEncryptionKeySizeCfmSend(req->appHandle,
                                        req->address,
                                        scData->bondData.leKeySize,
                                        (CsrBtResultCode)(scData->bondData.leKeySize != 0
                                                          ? CSR_BT_RESULT_CODE_SC_SUCCESS
                                                          : CSR_BT_RESULT_CODE_SC_ERROR_UNSPECIFIED),
                                        CSR_BT_SUPPLIER_SC);
        return;
    }
    /* D-24371: read the record from the SC DB
       and if the record is found and the key size is present just return that value */
    else if (CsrBtScDbRead(&(CsrBtAddrAddr(req->address)), &db))
    {
        if (db.leKeySize != 0)
        {/* Read from SC_DB */
            CsrBtScEncryptionKeySizeCfmSend(req->appHandle,
                                            req->address,
                                            db.leKeySize,
                                            CSR_BT_RESULT_CODE_SC_SUCCESS,
                                            CSR_BT_SUPPLIER_SC);
            return;
        }
    }
    
    /* else not needed */
    {
        CsrBtScLeLinks *links;
        for(links = scData->leLinks;
            links != NULL;
            links = links->next)
        {
            if (CsrBtAddrEq(&(links->addr), &(req->address)))
            {
                CsrBtScEncryptionKeySizeCfmSend(req->appHandle,
                                                req->address,
                                                links->encryptionKeySize,
                                                CSR_BT_RESULT_CODE_SC_SUCCESS,
                                                CSR_BT_SUPPLIER_SC);
                return;
            }
        }
    }
#endif
    
    /* Key not known by LE, ask the CM for the BR/EDR key */
    CsrBtCmReadEncryptionKeySizeReqSend(CSR_BT_SC_IFACEQUEUE,
                                        req->address,
                                        req->appHandle); /*context*/
}

/* CM has sent us a key size response. Forward to application */
void CsrBtScCmEncryptionKeySizeCfmHandler(ScInstanceData_t *scData)
{
    CsrBtCmReadEncryptionKeySizeCfm *cfm;
    CsrBtResultCode result;
    CsrBtSupplier supplier;
    cfm = scData->recvMsgP;

    if((cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
       && (cfm->resultSupplier == CSR_BT_SUPPLIER_CM))
    {
        result = CSR_BT_RESULT_CODE_SC_SUCCESS;
        supplier = CSR_BT_SUPPLIER_SC;
    }
    else
    {
        result = cfm->resultCode;
        supplier = cfm->resultSupplier;
    }

    CsrBtScEncryptionKeySizeCfmSend(cfm->context, /* appHandle stored in context */
                                    cfm->address,
                                    cfm->keySize,
                                    result, supplier);
    
}

/* Set the LE encryption key size min/max values */
#ifdef CSR_BT_LE_ENABLE
void CsrBtScSetEncryptionKeySizeReqHandler(ScInstanceData_t *scData)
{
    CsrBtScSetEncryptionKeySizeReq *prim;
    prim = scData->recvMsgP;

    CsrBtCmSmSetEncryptionKeySizeReqSend(prim->minKeySize,
                                         prim->maxKeySize);
}
#endif
