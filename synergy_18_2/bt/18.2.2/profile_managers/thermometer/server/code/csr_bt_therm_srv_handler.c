/****************************************************************************

  Copyright Cambridge Silicon Radio Limited and its licensors 2011-2012.

  All rights reserved. 

REVISION:      $Revision: #9 $
 ****************************************************************************/

#include "csr_synergy.h"

#include "csr_list.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"

#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"

#include "csr_bt_gatt_lib.h"
#include "csr_bt_gatt_services_defines.h"
#include "csr_bt_therm_srv.h"
#include "csr_bt_therm_srv_db.h"
#include "csr_bt_therm_srv_lib.h"
#include "csr_bt_therm_srv_prim.h"
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define ADVERTISE_DATA_THERMOMETER_LEN  4

/* Forward declarations for handler functions */
void CsrBtThermSrvHandleThermSrvPrim(CsrBtThermSrvInstData *thermSrvData);
void CsrBtThermSrvHandleGattPrim(CsrBtThermSrvInstData *thermSrvData);
void CsrBtThermSrvHandleSdPrim(CsrBtThermSrvInstData *thermSrvData);

static void csrBtThermSrvConvertMemToClientConfigList(CsrBtThermSrvInstData *thermSrvData, CsrUint8 *memPtr, CsrUint16 size)
{
    CsrUint16 x     = 0;
    CsrUint8 *head  = memPtr;
    CsrBtThermSrvClientConfigElement *active; 
    CsrUint16 count = size/sizeof(CsrBtThermSrvClientConfigElement);

    for(x=0;x<count; x++)
    {
        active                    =  CSR_BT_THERM_SRV_ADD_CLIENT_CONFIG(thermSrvData->clientConfigList);
        active->connectionFailed  =  ((CsrBtThermSrvClientConfigElement*)head)->connectionFailed;
        active->deviceAddr        =  ((CsrBtThermSrvClientConfigElement*)head)->deviceAddr;
        active->paired            =  ((CsrBtThermSrvClientConfigElement*)head)->paired;
        active->value             =  ((CsrBtThermSrvClientConfigElement*)head)->value;

        head += sizeof(CsrBtThermSrvClientConfigElement);
    }
    CsrPmemFree(memPtr);
}

static CsrUint16 csrBtThermSrvConvertClientConfigListToMem(CsrUint8 **memPtr, CsrCmnList_t *clientList)
{
    CsrUint16 size  = 0;
    CsrUint8 *head  = NULL;
    CsrUint16 x     = 0;
    CsrUint16 count = (CsrUint16)CsrCmnListGetCount(clientList);
    CsrBtThermSrvClientConfigElement *active = (CsrBtThermSrvClientConfigElement*)CsrCmnListGetFirst(clientList);

    size     =  count*sizeof(CsrBtThermSrvClientConfigElement);
    *memPtr  =  CsrPmemAlloc(size);
    /* point head at beginning of memPtr */
    head     = *memPtr;
    for(x=0;x<count; x++)
    {
        CsrMemCpy(head, active, sizeof(CsrBtThermSrvClientConfigElement));
        head += sizeof(CsrBtThermSrvClientConfigElement);
        active = active->next;
    }
    return size;
}

void CsrBtThermSrvClientConfigInitList(CsrCmnListElm_t *elem)
{
    /* Initialise a CsrBtThermSrvClientConfigElement. This function is called every
     * time a new entry is made on the queue list */
    CsrBtThermSrvClientConfigElement *cElem  =  (CsrBtThermSrvClientConfigElement *) elem;
    cElem->value                              =  NULL;
    cElem->paired                             =  FALSE;
    cElem->connectionFailed                   =  FALSE;
}

void CsrBtThermSrvFreeClientConfigList(CsrCmnListElm_t *elem)
{
    /* CsrPmemFree local pointers in the CsrBtThermSrvClientConfigElement.  
     * This function is called every time a element is removed from the
     * ClientConfig List list */
    CsrBtThermSrvClientConfigElement *cElem = (CsrBtThermSrvClientConfigElement *) elem;
    if(NULL != cElem->value)
    {
        CsrPmemFree(cElem->value);
    }
}

CsrBool CsrBtThermSrvFindClientConfigByAddr(CsrCmnListElm_t *elem, void *value)
{ /* Return TRUE if uuid matches*/
    CsrBtDeviceAddr *addr             = (CsrBtDeviceAddr *) value;
    CsrBtThermSrvClientConfigElement *element = (CsrBtThermSrvClientConfigElement *)elem;
    if(element->deviceAddr.lap != addr->lap || element->deviceAddr.uap != addr->uap || element->deviceAddr.nap != addr->nap)
    { 
        return FALSE;
    }
    /*the uuid is the same */
    return TRUE;
}

void CsrBtThermSrvSendNewTemperature(CsrBtThermSrvInstData *inst, CsrUint8 *value, CsrUint16 valueSize)
{
    CsrBtThermSrvClientConfigElement *cElem = CSR_BT_THERM_SRV_FIND_CLIENT_CONFIG_BY_ADDR(inst->clientConfigList, (void*)(&inst->connectedDeviceAddr.addr));
    if(cElem)
    {
        if(NULL != cElem->value && ((CsrUint16*)cElem->value)[0] == CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION)
        {
            CsrBtGattIndicationEventReqSend(inst->gattId, inst->btConnId, 
                                            (CsrUint16)(CSR_BT_THERM_SRV_THERMOMETER_HANDLE+inst->dbStartHandle+2),
                                            valueSize,(CsrUint8*)value);
        }
        else
        {
            CsrPmemFree(value);
        }
    }
    else
    {
        CsrPmemFree(value);
    }

}

/******************************************************************************
CsrBtThermSrvStartRegister:
 *****************************************************************************/
void CsrBtThermSrvStartRegister(CsrBtThermSrvInstData *thermSrvData)
{
    CsrBtGattRegisterReqSend(thermSrvData->phandle,
                             1234);
}
/******************************************************************************
CsrBtThermSrvStartUnregister:
 *****************************************************************************/
void CsrBtThermSrvStartUnregister(CsrBtThermSrvInstData *thermSrvData)
{
    CsrBtGattUnregisterReqSend(thermSrvData->gattId);
}
/******************************************************************************
CsrBtThermSrvStartActivate:
 *****************************************************************************/
void CsrBtThermSrvStartActivate(CsrBtThermSrvInstData *thermSrvData)
{
    CsrBtTypedAddr addr;
    
    CsrUint8 *advData = CsrPmemAlloc( (CsrUint8) ADVERTISE_DATA_THERMOMETER_LEN);
    advData[0] = ADVERTISE_DATA_THERMOMETER_LEN - 1; /* length */
    advData[1] = 0x03; /* AD type: Complete list of 16-bit UUIDs available */
    advData[2] = 0x09; /* UUID: Thermometer */
    advData[3] = 0x18;    

    CsrBtAddrZero(&addr);

    CsrBtGattPeripheralReqDataSend(thermSrvData->gattId,
                                   addr,
                                   CSR_BT_GATT_FLAGS_UNDIRECTED|CSR_BT_GATT_FLAGS_APPEND_DATA,
                                   0,
                                   (CsrUint8)ADVERTISE_DATA_THERMOMETER_LEN, 
                                   advData, 
                                   0,
                                   NULL);
}
/******************************************************************************
CsrBtThermSrvStartDeactivating:
 *****************************************************************************/
void CsrBtThermSrvStartDeactivate(CsrBtThermSrvInstData *thermSrvData)
{

}



/******************************************************************************
CsrBtThermSrvStartDisconnect:
 *****************************************************************************/
void CsrBtThermSrvStartDisconnect(CsrBtThermSrvInstData *thermSrvData)
{
    /*stop update timer*/
    CsrSchedTimerCancel(thermSrvData->update_timer,
                        0,
                        NULL);


    CsrBtGattDisconnectReqSend(thermSrvData->gattId, thermSrvData->btConnId);
    /*reset BT connection ID */
    thermSrvData->btConnId = 0xff;
}

/******************************************************************************
CsrBtThermSrvInitInstanceData:
 *****************************************************************************/
void CsrBtThermSrvClearHandles(CsrBtThermSrvInstData *thermSrvData){

}
void CsrBtThermSrvInitInstanceData(CsrBtThermSrvInstData *thermSrvData)
{
    CsrMemSet(thermSrvData,0,sizeof(CsrBtThermSrvInstData));

    thermSrvData->btConnId                          = 0xff;
    thermSrvData->tempData                          = NULL;
    thermSrvData->tempDataSize                      = 0;
    thermSrvData->deactivating                      = FALSE;
    thermSrvData->battMask                          = (CSR_BT_THERM_SRV_BATT_PRESENT_UNKNOWN | CSR_BT_THERM_SRV_BATT_DISCHARGING_UNKNOWN | CSR_BT_THERM_SRV_BATT_CHARGING_UNKNOWN | CSR_BT_THERM_SRV_BATT_CRITICAL_UNKNOWN);
    CsrBtThermSrvClearHandles(thermSrvData);
    CsrCmnListInit(&thermSrvData->clientConfigList, 0, CsrBtThermSrvClientConfigInitList, CsrBtThermSrvFreeClientConfigList);
}

void CsrBtThermSrvDeactivateCfmSend(CsrBtThermSrvInstData *thermSrvData, CsrBtResultCode result, CsrBtSupplier supplier)
{
    CsrBtThermSrvDeactivateCfm *cfm  =  CsrPmemAlloc(sizeof(CsrBtThermSrvDeactivateCfm));
    cfm->type                       =  CSR_BT_THERM_SRV_DEACTIVATE_CFM;
    cfm->clientConfigSize           =  csrBtThermSrvConvertClientConfigListToMem(&cfm->clientConfig, &thermSrvData->clientConfigList);
    cfm->resultCode                 =  result;
    cfm->resultSupplier             =  supplier;
    CsrSchedMessagePut(thermSrvData->appHandle, CSR_BT_THERM_SRV_PRIM, cfm);
}

void CsrBtThermSrvActivateCfmSend(CsrBtThermSrvInstData *thermSrvData, CsrBtResultCode result, CsrBtSupplier supplier)
{
    CsrBtThermSrvActivateCfm *cfm  =  CsrPmemAlloc(sizeof(CsrBtThermSrvActivateCfm));
    cfm->type                       =  CSR_BT_THERM_SRV_ACTIVATE_CFM;
    cfm->resultCode                 =  result;
    cfm->resultSupplier             =  supplier;
    cfm->dbStartHandle              = thermSrvData->dbStartHandle;
    cfm->dbEndHandle                = thermSrvData->dbEndHandle;
    cfm->gattId                     = thermSrvData->gattId;
    CsrSchedMessagePut(thermSrvData->appHandle, CSR_BT_THERM_SRV_PRIM, cfm);
}

void CsrBtThermSrvConnectIndSend(CsrBtThermSrvInstData *thermSrvData, CsrBtResultCode result, CsrBtSupplier supplier)
{
    CsrBtThermSrvConnectInd *ind     = CsrPmemAlloc(sizeof(CsrBtThermSrvConnectInd));
    ind->type                       = CSR_BT_THERM_SRV_CONNECT_IND;
    ind->btConnId                   = thermSrvData->connectedBtConnId;
    ind->deviceAddr                 = thermSrvData->connectedDeviceAddr;
    ind->resultCode                 = result;
    ind->resultSupplier             = supplier;
    CsrSchedMessagePut(thermSrvData->appHandle, CSR_BT_THERM_SRV_PRIM, ind);
}

void CsrBtThermSrvDisconnectIndSend(CsrBtThermSrvInstData *thermSrvData, CsrBtReasonCode reason, CsrBtSupplier supplier)
{
    CsrBtThermSrvDisconnectInd *ind  = CsrPmemAlloc(sizeof(CsrBtThermSrvDisconnectInd));
    ind->type                       = CSR_BT_THERM_SRV_DISCONNECT_IND;
    ind->btConnId                   = thermSrvData->connectedBtConnId;
    ind->deviceAddr                 = thermSrvData->connectedDeviceAddr;
    ind->reasonCode                 = reason;
    ind->reasonSupplier             = supplier;
    CsrSchedMessagePut(thermSrvData->appHandle, CSR_BT_THERM_SRV_PRIM, ind);
}

void CsrBtThermSrvWriteEventIndSend(CsrBtThermSrvInstData *thermSrvData, CsrUint16 valueSize, CsrUint8 *value, CsrUint16 handle)
{
    if((thermSrvData->eventMask & CSR_BT_THERM_SRV_EVENT_WRITE) == CSR_BT_THERM_SRV_EVENT_WRITE)
    {
        CsrBtThermSrvWriteEventInd *ind  = CsrPmemAlloc(sizeof(CsrBtThermSrvWriteEventInd));
        ind->type                       = CSR_BT_THERM_SRV_WRITE_EVENT_IND;

        ind->value                      = value;
        ind->valueHandle                = handle;
        ind->valueSize                  = valueSize;
        CsrSchedMessagePut(thermSrvData->appHandle, CSR_BT_THERM_SRV_PRIM, ind);

    }
    else
    {
        /* we are final destination for pointer - free it! */
        CsrPmemFree(value);
    }
}

void CsrBtThermSrvUpdateBattLevelCfmSend(CsrBtThermSrvInstData *thermSrvData)
{
    CsrBtThermSrvUpdateBattLevelCfm *cfm = CsrPmemAlloc(sizeof(CsrBtThermSrvUpdateBattLevelCfm));
    cfm->type                         = CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM;
    cfm->resultSupplier               = CSR_BT_SUPPLIER_THERM_SRV;
    cfm->resultCode                   = CSR_BT_THERM_SRV_RESULT_SUCCESS;
    CsrSchedMessagePut(thermSrvData->appHandle, CSR_BT_THERM_SRV_PRIM, cfm);
}

void CsrBtThermSrvUpdateHealthInfoCfmSend(CsrBtThermSrvInstData *thermSrvData, CsrBtResultCode result, CsrBtSupplier supplier)

{
    CsrBtThermSrvUpdateBattLevelCfm *cfm = CsrPmemAlloc(sizeof(CsrBtThermSrvUpdateBattLevelCfm));
    cfm->type                         = CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM;
    cfm->resultSupplier               = supplier;
    cfm->resultCode                   = result;
    CsrSchedMessagePut(thermSrvData->appHandle, CSR_BT_THERM_SRV_PRIM, cfm);
}

/******************************************************************************
 * Init function called by the scheduler upon initialisation.
 * This function is used to boot the demo application.
 *****************************************************************************/
void CsrBtThermSrvInit(void **gash)
{
    CsrBtThermSrvInstData    *thermSrvData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(CsrBtThermSrvInstData));
    thermSrvData = (CsrBtThermSrvInstData *) *gash;
    CsrBtThermSrvInitInstanceData(thermSrvData);

    thermSrvData->phandle = CsrSchedTaskQueueGet() ;

    thermSrvData->initialized = TRUE;

}

#ifdef ENABLE_SHUTDOWN
void CsrBtThermSrvDeinit(void **gash)
{
    CsrUint16 msg_type;
    void *msg_data;
    CsrBtThermSrvInstData *thermSrvData;

    thermSrvData = (CsrBtThermSrvInstData *) (*gash);
    /* get a message from the demoapplication message queue. The message is returned in prim
       and the event type in eventType */
    while ( CsrSchedMessageGet(&msg_type, &msg_data) )
    {
        switch (msg_type)
        {
            case CSR_BT_GATT_PRIM:
                CsrBtGattFreeUpstreamMessageContents(msg_type, msg_data);
                break;
        }
        CsrPmemFree(msg_data);
    }

    CsrPmemFree(thermSrvData);
}
#else
void CsrBtThermSrvDeinit(void **gash)
{
    CSR_UNUSED(gash);
}
#endif

/******************************************************************************
 * This is the demo application handler function. All primitives sent
 * to the demo application will be received in here. The scheduler
 * ensures that the function is activated when a signal is put on the
 * demo application signal queue.
 *****************************************************************************/
void CsrBtThermSrvHandler(void **gash)
{
    CsrBtThermSrvInstData *thermSrvData;
    CsrUint16    eventType;

    /* get a message from the demoapplication message queue. The message is returned in prim
       and the event type in eventType */
    thermSrvData = (CsrBtThermSrvInstData *) (*gash);

    if(!thermSrvData->restoreFlag)
    { /* No messages on the local save queue                                                */
        CsrSchedMessageGet(&eventType , &thermSrvData->recvMsgP);
    }
    else
    {
        if(!CsrMessageQueuePop(&thermSrvData->saveQueue, &eventType , &thermSrvData->recvMsgP))
        { /* No more messages on the local save queue. Call CsrGetMessage to receive
             a message from the scheduler                                                   */
            thermSrvData->restoreFlag = FALSE;
            CsrSchedMessageGet(&eventType , &thermSrvData->recvMsgP);
        }
        else
        { /* A message has been restored from the local queed                               */
            ;
        }
    }

    switch (eventType)
    {
        case CSR_BT_SD_PRIM:
            {
                CsrBtThermSrvHandleSdPrim(thermSrvData);
                break;
            }
        case CSR_BT_GATT_PRIM:
            {
                CsrBtThermSrvHandleGattPrim(thermSrvData);
                break;
            }

        case CSR_BT_THERM_SRV_PRIM:
            {
                CsrBtThermSrvHandleThermSrvPrim(thermSrvData);
                break;
            }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                /* unexpected primitive received */
                CsrGeneralException("CSR_BT_THERM_SRV",
                                    0, 
                                    *((CsrPrim *) thermSrvData->recvMsgP),
                                    "####### default in CsrBtThermSrvHandler,");
            }
#endif
    }
    /* free the received signal again. If the signal for some reason must be stored in the application
     * the pointer (prim) must be set to NULL in order not to free it here
     */
    CsrPmemFree(thermSrvData->recvMsgP);
}

/******************************************************************************
 * Function to handle all THERM_SRV-primitives.
 ******************************************************************************/

void CsrBtThermSrvHandleThermSrvPrim(CsrBtThermSrvInstData *thermSrvData)
{
    CsrPrim *type = (CsrPrim *) thermSrvData->recvMsgP;

    switch(*type)
    {
        case CSR_BT_THERM_SRV_ACTIVATE_REQ:
            { 
                CsrBtThermSrvActivateReq *prim  =  thermSrvData->recvMsgP;
                thermSrvData->appHandle         =  prim->appHandle;
                thermSrvData->eventMask         =  prim->eventMask;
                if(prim->clientConfigSize)
                {
                    csrBtThermSrvConvertMemToClientConfigList(thermSrvData, prim->clientConfig, prim->clientConfigSize);
                }
                /* Register with Gatt, then add DB and then start listening for connections (advertise) */
                CsrBtGattRegisterReqSend(CSR_BT_THERM_SRV_IFACEQUEUE, 0);
                break;
            }
        case CSR_BT_THERM_SRV_DEACTIVATE_REQ:
            {
                /*unregister with gatt*/
                CsrBtGattUnregisterReqSend(thermSrvData->gattId);
                thermSrvData->deactivating = TRUE;
                break;
            }
        case CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ:
            {
                CsrBtThermSrvUpdateBattLevelReq *prim = thermSrvData->recvMsgP;
                thermSrvData->battValue               = prim->battLevel;
                thermSrvData->battMask                = prim->battMask;
                thermSrvData->serviceRequired         = prim->serviceRequired;
                CsrBtThermSrvUpdateBattLevelCfmSend(thermSrvData);
                break;
            }
        case CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ:
            {
                CsrBtThermSrvUpdateHealthInfoReq *prim = thermSrvData->recvMsgP;
                switch(prim->infoType)
                    {
                        case CSR_BT_THERM_SRV_HEALTH_INFO_MANUFACT_NAME_STR:
                            {
                                CsrBtGattWriteLocalReqSend(thermSrvData->gattId, 
                                                           CSR_BT_THERM_SRV_HEALTH_INFO_MANUFACT_NAME_HANDLE+thermSrvData->dbStartHandle,
                                                           prim->infoLength+1, 
                                                           prim->info);
                                break;
                            }
                        case CSR_BT_THERM_SRV_HEALTH_INFO_MODEL_NUM_STR:
                            {
                                CsrBtGattWriteLocalReqSend(thermSrvData->gattId, 
                                                           CSR_BT_THERM_SRV_HEALTH_INFO_MODEL_NUM_HANDLE+thermSrvData->dbStartHandle,
                                                           prim->infoLength+1, 
                                                           prim->info);

                                break;
                            }
                        case CSR_BT_THERM_SRV_HEALTH_INFO_SERIAL_NUM_STR:
                            {
                                CsrBtGattWriteLocalReqSend(thermSrvData->gattId, 
                                                           CSR_BT_THERM_SRV_HEALTH_INFO_SERIAL_NUM_HANDLE+thermSrvData->dbStartHandle,
                                                           prim->infoLength+1, 
                                                           prim->info);
                                break;
                            }
                        case CSR_BT_THERM_SRV_HEALTH_INFO_HW_REVISION_STR:
                            {
                                CsrBtGattWriteLocalReqSend(thermSrvData->gattId, 
                                                           CSR_BT_THERM_SRV_HEALTH_INFO_HW_REVISION_HANDLE+thermSrvData->dbStartHandle,
                                                           prim->infoLength+1, 
                                                           prim->info);
                                break;
                            }
                        case CSR_BT_THERM_SRV_HEALTH_INFO_FW_REVISION_STR:
                            {
                                CsrBtGattWriteLocalReqSend(thermSrvData->gattId, 
                                                           CSR_BT_THERM_SRV_HEALTH_INFO_FW_REVISION_HANDLE+thermSrvData->dbStartHandle,
                                                           prim->infoLength+1, 
                                                           prim->info);
                                break;
                            }
                        case CSR_BT_THERM_SRV_HEALTH_INFO_SW_REVISION_STR:
                            {
                                CsrBtGattWriteLocalReqSend(thermSrvData->gattId, 
                                                           CSR_BT_THERM_SRV_HEALTH_INFO_SW_REVISION_HANDLE+thermSrvData->dbStartHandle,
                                                           prim->infoLength+1, 
                                                           prim->info);
                                break;
                            }
                        default:
                            {
                                CsrBtThermSrvUpdateHealthInfoCfmSend(thermSrvData, 
                                                                     CSR_BT_THERM_SRV_RESULT_UNKNOWN_INFO_TYPE, 
                                                                     CSR_BT_SUPPLIER_THERM_SRV);
                            }
                    }
                CsrBtThermSrvUpdateBattLevelCfmSend(thermSrvData);
                break;
            }
        case CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ:
            {
                CsrBtThermSrvUpdateTemperatureReq *prim = thermSrvData->recvMsgP;
                CsrBtThermSrvSendNewTemperature(thermSrvData, prim->tempData, prim->tempDataSize);
                break;
            }
        default:
            { /* Do nothing */
                break;
            }
    }

}

/******************************************************************************
 * Function to handle all SD-primitives.
 ******************************************************************************/
void CsrBtThermSrvHandleSdPrim(CsrBtThermSrvInstData *thermSrvData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) thermSrvData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_SD_READ_DEVICE_INFO_CFM:
            {
                /*lets see if the device is paired or not - add it to client config list if it does not exist */
                CsrBtSdReadDeviceInfoCfm *cfm = (CsrBtSdReadDeviceInfoCfm*)thermSrvData->recvMsgP;
                CsrBtThermSrvClientConfigElement *cElem = CSR_BT_THERM_SRV_FIND_CLIENT_CONFIG_BY_ADDR(thermSrvData->clientConfigList, (void*)(&cfm->deviceAddr));
                if(!cElem)
                {
                    cElem = CSR_BT_THERM_SRV_ADD_CLIENT_CONFIG(thermSrvData->clientConfigList);
                    cElem->deviceAddr.lap = cfm->deviceAddr.lap;
                    cElem->deviceAddr.nap = cfm->deviceAddr.nap;
                    cElem->deviceAddr.uap = cfm->deviceAddr.uap;
                }
                cElem->paired = ((cfm->deviceStatus & CSR_BT_SD_STATUS_PAIRED) == CSR_BT_SD_STATUS_PAIRED)?TRUE:FALSE;
                break;
            }
        default:
            {
                CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, thermSrvData->recvMsgP);
                /* unexpected primitive received */

            }


    }
}


/******************************************************************************
 * Function to handle all therms-primitives.
 ******************************************************************************/
void CsrBtThermSrvHandleGattPrim(CsrBtThermSrvInstData *thermSrvData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) thermSrvData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_GATT_REGISTER_CFM:
            {
                CsrBtGattRegisterCfm *cfm = (CsrBtGattRegisterCfm*)thermSrvData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    thermSrvData->gattId = cfm->gattId;
                    /* subscribe to notifications about new physical link connections/loss - needed for clientConfig */
                    CsrBtGattSetEventMaskReqSend(cfm->gattId,CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_NONE /*PHYSICAL_LINK_STATUS*/);
                }
                else
                {
                    CsrBtThermSrvActivateCfmSend(thermSrvData, cfm->resultCode, cfm->resultSupplier);
                }

                break;
            }
        case CSR_BT_GATT_SET_EVENT_MASK_CFM:
            {
                /*event mask set, now lets move on to DB setup */
                /* Allocate and add DB in Gatt*/
                CsrBtGattDbAllocReqSend(thermSrvData->gattId, CSR_BT_THERM_SRV_DB_HANDLE_COUNT, CSR_BT_THERM_SRV_DB_PREFERRED_HANDLE );
                break;
            }
        case CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND:
            {
                /* a physical link has either been connected or disconnected
                 * we need to get info about the connection in order to add/remove the item from
                 * the client config list 
                 */
                CsrBtGattPhysicalLinkStatusInd *ind = (CsrBtGattPhysicalLinkStatusInd*)thermSrvData->recvMsgP;
                CsrBtSdReadDeviceInfoReqSendEx(thermSrvData->phandle, ind->address.addr, ind->address.type, 0);
                break;
            }
        case CSR_BT_GATT_DB_ADD_CFM:
            {
                CsrBtGattDbAddCfm *cfm = (CsrBtGattDbAddCfm*)thermSrvData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrBtThermSrvStartActivate(thermSrvData);
                }
                else
                {
                    CsrBtThermSrvActivateCfmSend(thermSrvData, cfm->resultCode, cfm->resultSupplier);
                }
                break;
            }
        case CSR_BT_GATT_UNREGISTER_CFM:
            {
                CsrBtThermSrvDeactivateCfmSend(thermSrvData, CSR_BT_THERM_SRV_RESULT_SUCCESS, CSR_BT_SUPPLIER_THERM_SRV);
                /*deactivate over*/
                thermSrvData->deactivating = FALSE;
                break;
            }
        case CSR_BT_GATT_DB_ALLOC_CFM:
            {
                CsrBtGattDbAllocCfm *cfm = (CsrBtGattDbAllocCfm*)thermSrvData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrBtGattDb *db = CsrBtThermSrvDbCreate(cfm->start);
                    CsrBtGattDbAddReqSend(thermSrvData->gattId, db);
                    thermSrvData->dbStartHandle = cfm->start;
                    thermSrvData->dbEndHandle = cfm->end;
                }
                else
                {
                    /*We could show error about not being able to allocate */
                    CsrBtThermSrvActivateCfmSend(thermSrvData, cfm->resultCode, cfm->resultSupplier);
                }
                break;
            }
        case CSR_BT_GATT_CONNECT_IND:
            {
                CsrBtGattConnectInd        *ind;

                ind = (CsrBtGattConnectInd *) thermSrvData->recvMsgP;
                thermSrvData->btConnId = ind->btConnId;

                if (ind->resultSupplier == CSR_BT_SUPPLIER_GATT &&
                    ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    thermSrvData->connectedDeviceAddr.addr.nap = ind->address.addr.nap;
                    thermSrvData->connectedDeviceAddr.addr.lap = ind->address.addr.lap;
                    thermSrvData->connectedDeviceAddr.addr.uap = ind->address.addr.uap;
                    CsrBtThermSrvConnectIndSend(thermSrvData, CSR_BT_THERM_SRV_RESULT_SUCCESS, CSR_BT_SUPPLIER_THERM_SRV);
                }
                else
                {
                    /* error during connect */
                    CsrBtThermSrvConnectIndSend(thermSrvData, ind->resultCode, ind->resultSupplier);
                }
                break;
            }
        case CSR_BT_GATT_WRITE_CFM:
            {
                CsrBtGattWriteCfm *prim = thermSrvData->recvMsgP;
                if(prim->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrBtThermSrvUpdateHealthInfoCfmSend(thermSrvData, CSR_BT_THERM_SRV_RESULT_SUCCESS, CSR_BT_SUPPLIER_THERM_SRV);
                }
                else
                {
                    CsrBtThermSrvUpdateHealthInfoCfmSend(thermSrvData, prim->resultCode, prim->resultSupplier);
                }

                break;
            }
        case CSR_BT_GATT_DISCONNECT_IND:
            {
                CsrBtGattDisconnectInd    *ind;

                ind      = (CsrBtGattDisconnectInd *) thermSrvData->recvMsgP;

                if(ind->reasonCode == CSR_BT_GATT_RESULT_SUCCESS && ind->reasonSupplier == CSR_BT_SUPPLIER_GATT)
                {
                    CsrBtThermSrvDisconnectIndSend(thermSrvData, CSR_BT_THERM_SRV_RESULT_SUCCESS, CSR_BT_SUPPLIER_THERM_SRV);
                }
                else
                {
                    CsrBtThermSrvDisconnectIndSend(thermSrvData, ind->reasonCode, ind->reasonSupplier);
                }
                /*reset instance data */
                /*reset the btConnId */
                thermSrvData->btConnId = 0xff;
                /*Stop the UPDATE TIMER */
                CsrSchedTimerCancel(thermSrvData->update_timer,
                                    0,
                                    NULL);
                /*Clear all handles in instance */
                CsrBtThermSrvClearHandles(thermSrvData);
                /*go back to listening state */
                if(!thermSrvData->deactivating)
                {
                    CsrBtThermSrvStartActivate(thermSrvData);
                }
                break;
            }
        case CSR_BT_GATT_PERIPHERAL_CFM:
            {
                CsrBtGattPeripheralCfm *cfm;
                cfm = (CsrBtGattPeripheralCfm*) thermSrvData->recvMsgP;

                if(cfm->resultCode != CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /*something is wrong */
                    CsrBtThermSrvActivateCfmSend(thermSrvData, cfm->resultCode, cfm->resultSupplier);
                }
                break;
            }
        case CSR_BT_GATT_DB_ACCESS_WRITE_IND:
            {
                CsrBtGattDbAccessWriteInd *ind = (CsrBtGattDbAccessWriteInd*)thermSrvData->recvMsgP;
                CsrUint16 handle =  ind->writeUnit[0].attrHandle - thermSrvData->dbStartHandle; 
                if((ind->writeUnit[0].value[0] == (CsrUint8)(0x00FF & CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION) ||
                    ind->writeUnit[0].value[0] == (CsrUint8)(0x00FF & CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT)) &&
                    ind->writeUnit[0].valueLength == 2)
                {
                    switch(handle)
                    {
                        case CSR_BT_THERM_SRV_TEMP_MEASUREMENT_CC_HANDLE: /*client config change*/
                            {
                                CsrBtThermSrvClientConfigElement *cElem = CSR_BT_THERM_SRV_FIND_CLIENT_CONFIG_BY_ADDR(thermSrvData->clientConfigList, (void*)(&thermSrvData->connectedDeviceAddr.addr));
                                if(!cElem)
                                {
                                    /*We really should never get in-here as the clientConfig should have been added elsewhere */
                                    /* no previous clientConfig record in list
                                     * lets add one */
                                    cElem = CSR_BT_THERM_SRV_ADD_CLIENT_CONFIG(thermSrvData->clientConfigList);
                                    cElem->deviceAddr = thermSrvData->connectedDeviceAddr.addr;
                                }
                                /*update with latest write value - this is stored per device */
                                CsrPmemFree(cElem->value);
                                cElem->value      = ind->writeUnit[0].value;
                                break;
                            }
                    }
                    /*let the app know that they try to write */
                    CsrBtThermSrvWriteEventIndSend(thermSrvData, ind->writeUnit[0].valueLength, ind->writeUnit[0].value, ind->writeUnit[0].attrHandle);

                    /* tell Gatt that write is ok*/
                    CsrBtGattDbWriteAccessResSend(thermSrvData->gattId,
                                                  thermSrvData->btConnId,
                                                  ind->writeUnit[0].attrHandle,
                                                  CSR_BT_GATT_ACCESS_RES_SUCCESS);

                }
                else
                {
                    /*we only allow INDICATIONS or DEFAULT */
                    CsrBtGattDbWriteAccessResSend(thermSrvData->gattId,
                                                  thermSrvData->btConnId,
                                                  ind->writeUnit[0].attrHandle,
                                                  CSR_BT_GATT_ACCESS_RES_WRITE_NOT_PERMITTED);
                    CsrPmemFree(ind->writeUnit[0].value);
                }
                break;

            }
        case CSR_BT_GATT_DB_ACCESS_READ_IND:
            {
                CsrBtGattDbAccessReadInd *ind = (CsrBtGattDbAccessReadInd*)thermSrvData->recvMsgP;
                switch (ind->attrHandle-2/*value offset*/ - thermSrvData->dbStartHandle )
                {
                    case CSR_BT_THERM_SRV_THERMOMETER_HANDLE:
                        {
                            if(thermSrvData->tempDataSize)
                            {
                                CsrUint8 *val = CsrPmemAlloc(thermSrvData->tempDataSize);
                                CsrMemCpy(val, thermSrvData->tempData, thermSrvData->tempDataSize);
                                /* tell Gatt that write is ok*/
                                CsrBtGattDbReadAccessResSend(thermSrvData->gattId, 
                                                             thermSrvData->btConnId, 
                                                             ind->attrHandle, 
                                                             CSR_BT_GATT_ACCESS_RES_SUCCESS,
                                                             1 /*value length*/,
                                                             val);
                            }
                            else
                            {
                                CsrBtGattDbReadAccessResSend(thermSrvData->gattId, 
                                                             thermSrvData->btConnId, 
                                                             ind->attrHandle, 
                                                             CSR_BT_GATT_ACCESS_RES_READ_NOT_PERMITTED,
                                                             0 /*value length*/,
                                                             NULL);

                            }
                            break;
                        }
                    case CSR_BT_THERM_SRV_TEMP_MEASUREMENT_CC_HANDLE-2:
                        {
                            {
                                CsrUint8 *val = CsrPmemAlloc(2);
                                CsrBtThermSrvClientConfigElement *cElem = CSR_BT_THERM_SRV_FIND_CLIENT_CONFIG_BY_ADDR(thermSrvData->clientConfigList, (void*)(&thermSrvData->connectedDeviceAddr.addr));
                                if(!cElem)
                                {
                                    val[0] = 0;
                                    val[1] = 0;
                                }
                                else
                                {
                                    val[0] = cElem->value[0];
                                    val[1] = cElem->value[1];
                                }

                                /* tell Gatt that write is ok*/
                                CsrBtGattDbReadAccessResSend(thermSrvData->gattId, 
                                                             thermSrvData->btConnId, 
                                                             ind->attrHandle, 
                                                             CSR_BT_GATT_ACCESS_RES_SUCCESS,
                                                             2 /*value length*/,
                                                             val);

                            }
                            break;
                        }
                    case CSR_BT_THERM_SRV_BATTERY_HANDLE:
                        {
                            CsrUint8 *val = CsrPmemAlloc(1);
                            val[0] = (CsrInt8)thermSrvData->battMask;
                            CsrBtGattDbReadAccessResSend(thermSrvData->gattId, 
                                                         thermSrvData->btConnId, 
                                                         ind->attrHandle, 
                                                         CSR_BT_GATT_ACCESS_RES_SUCCESS,
                                                         1 /*value length*/,
                                                         val);
                            break;
                        }
                    case (CSR_BT_THERM_SRV_BATTERY_HANDLE+2):
                        {
                            CsrUint8 *val = CsrPmemAlloc(1);
                            val[0] = (CsrInt8)thermSrvData->battValue;
                            CsrBtGattDbReadAccessResSend(thermSrvData->gattId, 
                                                         thermSrvData->btConnId, 
                                                         ind->attrHandle, 
                                                         CSR_BT_GATT_ACCESS_RES_SUCCESS,
                                                         1 /*value length*/,
                                                         val);
                            break;
                        }
                    case (CSR_BT_THERM_SRV_BATTERY_HANDLE+4):
                        {
                            CsrUint8 *val = CsrPmemAlloc(1);
                            val[0] = (CsrInt8)thermSrvData->serviceRequired;
                            CsrBtGattDbReadAccessResSend(thermSrvData->gattId, 
                                                         thermSrvData->btConnId, 
                                                         ind->attrHandle, 
                                                         CSR_BT_GATT_ACCESS_RES_SUCCESS,
                                                         1 /*value length*/,
                                                         val);
                            break;
                        }
                }

                break;
            }
        default:
            {
                CsrBtGattFreeUpstreamMessageContents(CSR_BT_GATT_PRIM, thermSrvData->recvMsgP);
                /* unexpected primitive received */
            }
    }
}

