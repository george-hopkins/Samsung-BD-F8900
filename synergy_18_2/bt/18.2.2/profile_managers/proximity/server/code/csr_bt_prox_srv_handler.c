/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011-2012.
   
   All rights reserved. 

REVISION:      $Revision: #6 $
 ****************************************************************************/

#include "csr_synergy.h"

#include "csr_util.h"
#include "csr_list.h"
#include "csr_pmem.h"

#include "csr_bt_util.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_gatt_lib.h"
#include "csr_bt_gatt_services_defines.h"
#include "csr_bt_prox_srv_prim.h"
#include "csr_bt_prox_srv_lib.h"
#include "csr_bt_prox_srv.h"
#include "csr_bt_prox_srv_db.h"
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define ADVERTISE_DATA_PRIXIMITY_LEN  8

static void csrBtProxSrvConvertMemToClientConfigList(CsrBtProxSrvInstData *proxSrvData, CsrUint8 *memPtr, CsrUint16 size)
{
    CsrUint16 x     = 0;
    CsrUint8 *head  = memPtr;
    CsrBtProxSrvClientConfigElement *active; 
    CsrUint16 count = size/sizeof(CsrBtProxSrvClientConfigElement);
    
    for(x=0;x<count; x++)
    {
        active                    =  CSR_BT_PROX_SRV_ADD_CLIENT_CONFIG(proxSrvData->clientConfigList);
        active->connectionFailed  =  ((CsrBtProxSrvClientConfigElement*)head)->connectionFailed;
        active->deviceAddr        =  ((CsrBtProxSrvClientConfigElement*)head)->deviceAddr;
        active->paired            =  ((CsrBtProxSrvClientConfigElement*)head)->paired;
        active->value             =  ((CsrBtProxSrvClientConfigElement*)head)->value;

        head += sizeof(CsrBtProxSrvClientConfigElement);
    }
    CsrPmemFree(memPtr);
}
 
static CsrUint16 csrBtProxSrvConvertClientConfigListToMem(CsrUint8 **memPtr, CsrCmnList_t *clientList)
{
    CsrUint16 size  = 0;
    CsrUint8 *head  = NULL;
    CsrUint16 x     = 0;
    CsrUint16 count = (CsrUint16)CsrCmnListGetCount(clientList);
    CsrBtProxSrvClientConfigElement *active = (CsrBtProxSrvClientConfigElement*)CsrCmnListGetFirst(clientList);

    size     =  count*sizeof(CsrBtProxSrvClientConfigElement);
    *memPtr  =  CsrPmemAlloc(size);
    /* point head at beginning of memPtr */
    head     = *memPtr;
    for(x=0;x<count; x++)
    {
        CsrMemCpy(head, active, sizeof(CsrBtProxSrvClientConfigElement));
        head += sizeof(CsrBtProxSrvClientConfigElement);
        active = active->next;
    }

    

    return size;
}


CsrBool CsrBtProxSrvFindClientConfigByAddr(CsrCmnListElm_t *elem, void *value)
{ /* Return TRUE if uuid matches*/
    CsrBtDeviceAddr *addr             = (CsrBtDeviceAddr *) value;
    CsrBtProxSrvClientConfigElement *element = (CsrBtProxSrvClientConfigElement *)elem;
    if(element->deviceAddr.lap != addr->lap || element->deviceAddr.uap != addr->uap || element->deviceAddr.nap != addr->nap)
    { 
        return FALSE;
    }
    /*the uuid is the same */
    return TRUE;
}


void CsrBtProxSrvTxPowerTimerInterupt(CsrUint16 mi, void *mv)
{
    CsrBtProxSrvInstData *inst = (CsrBtProxSrvInstData*)mv;
    CsrBtProxSrvClientConfigElement *cElem = CSR_BT_PROX_SRV_FIND_CLIENT_CONFIG_BY_ADDR(inst->clientConfigList, (void*)(&inst->connectedDeviceAddr.addr));
    if(cElem)
    {
        if(NULL != cElem->value && ((CsrUint16*)cElem->value)[0] == CSR_BT_GATT_CLIENT_CHARAC_CONFIG_NOTIFICATION)
        {
            CsrBtCmReadTxPowerLevelReqSend(inst->phandle, inst->connectedDeviceAddr.addr, 0xff);
            /* we only want to restart the time and poll if it is actually relevant */
            inst->txPower_timer = CsrSchedTimerSet(CSR_BT_PROX_SRV_TX_POWER_TIMER_INTERVAL,
                                                   CsrBtProxSrvTxPowerTimerInterupt,
                                                   mi,
                                                   mv);

        }
    }
}

void sendGattPeripheralReq(CsrBtGattId gattId)
{
    CsrBtTypedAddr addr;
    
    CsrUint8 *advData = CsrPmemAlloc( (CsrUint8) ADVERTISE_DATA_PRIXIMITY_LEN);
    advData[0] = ADVERTISE_DATA_PRIXIMITY_LEN -1 ; /* length */
    advData[1] = 0x03; /* AD type: Complete list of 16-bit UUIDs available */
    advData[2] = 0x02; /* UUID: Thermometer */
    advData[3] = 0x18;    
    advData[4] = 0x03; /* UUID: Thermometer */
    advData[5] = 0x18;    
    advData[6] = 0x04; /* UUID: Thermometer */
    advData[7] = 0x18;    

    CsrBtAddrZero(&addr);

    CsrBtGattPeripheralReqDataSend(gattId,
                                   addr,
                                   CSR_BT_GATT_FLAGS_UNDIRECTED,
                                   0,
                                   (CsrUint8)ADVERTISE_DATA_PRIXIMITY_LEN, 
                                   advData, 
                                   0,
                                   NULL);
}

/******************************************************************************
CsrBtProxSrvCrank
 *****************************************************************************/
void CsrBtProxSrvCrank(CsrBtProxSrvInstData *proxSrvData)
{
    /* Crank the state - i.e. listen or disconnect or whatever we need
     * to do */
    switch (proxSrvData->state)
    {
        case CSR_BT_PROX_SRV_ST_LISTEN:
        {
            /* accept connections*/
            if (proxSrvData->leBtConnId == CSR_BT_CONN_ID_INVALID)
            {
                sendGattPeripheralReq(proxSrvData->gattId);
            }

        }
        break;
        case CSR_BT_PROX_SRV_ST_LE:
        {
            /*we should not do anything*/
            break;
        }
        case CSR_BT_PROX_SRV_ST_STOP:
        {
            /* disconnect all */
            if (proxSrvData->leBtConnId != CSR_BT_CONN_ID_INVALID)
            {
                CsrBtGattDisconnectReqSend(proxSrvData->gattId,
                                           proxSrvData->leBtConnId);
            }
        }
        break;
    }
}
void CsrBtProxSrvDeactivateCfmSend(CsrBtProxSrvInstData *proxSrvData, CsrBtResultCode result, CsrBtSupplier supplier)
{
    CsrBtProxSrvDeactivateCfm *cfm  =  CsrPmemAlloc(sizeof(CsrBtProxSrvDeactivateCfm));
    cfm->type                       =  CSR_BT_PROX_SRV_DEACTIVATE_CFM;
    cfm->clientConfigSize           =  csrBtProxSrvConvertClientConfigListToMem(&cfm->clientConfig, &proxSrvData->clientConfigList);
    cfm->resultCode                 =  result;
    cfm->resultSupplier             =  supplier;
    CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, cfm);
}

void CsrBtProxSrvActivateCfmSend(CsrBtProxSrvInstData *proxSrvData, CsrBtResultCode result, CsrBtSupplier supplier)
{
    CsrBtProxSrvActivateCfm *cfm  =  CsrPmemAlloc(sizeof(CsrBtProxSrvActivateCfm));
    cfm->type                       =  CSR_BT_PROX_SRV_ACTIVATE_CFM;
    cfm->resultCode                 =  result;
    cfm->resultSupplier             =  supplier;
    cfm->dbStartHandle              = proxSrvData->dbStartHandle;
    cfm->dbEndHandle                = proxSrvData->dbEndHandle;
    cfm->gattId                     = proxSrvData->gattId;
    CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, cfm);
}

void CsrBtProxSrvConnectIndSend(CsrBtProxSrvInstData *proxSrvData, CsrBtResultCode result, CsrBtSupplier supplier)
{
    CsrBtProxSrvConnectInd *ind     = CsrPmemAlloc(sizeof(CsrBtProxSrvConnectInd));
    ind->type                       = CSR_BT_PROX_SRV_CONNECT_IND;
    ind->btConnId                   = proxSrvData->connectedBtConnId;
    ind->deviceAddr                 = proxSrvData->connectedDeviceAddr;
    ind->resultCode                 = result;
    ind->resultSupplier             = supplier;
    CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, ind);
}

void CsrBtProxSrvDisconnectIndSend(CsrBtProxSrvInstData *proxSrvData, CsrBtReasonCode reason, CsrBtSupplier supplier)
{
    CsrBtProxSrvDisconnectInd *ind  = CsrPmemAlloc(sizeof(CsrBtProxSrvDisconnectInd));
    ind->type                       = CSR_BT_PROX_SRV_DISCONNECT_IND;
    ind->btConnId                   = proxSrvData->connectedBtConnId;
    ind->deviceAddr                 = proxSrvData->connectedDeviceAddr;
    ind->reasonCode                 = reason;
    ind->reasonSupplier             = supplier;
    CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, ind);
}

void CsrBtProxSrvTxPowerChangedEventIndSend(CsrBtProxSrvInstData *proxSrvData)
{
    if((proxSrvData->eventMask & CSR_BT_PROX_SRV_EVENT_TX_POWER_CHANGED) == CSR_BT_PROX_SRV_EVENT_TX_POWER_CHANGED)
    {
        CsrBtProxSrvTxPowerChangedEventInd *ind     = CsrPmemAlloc(sizeof(CsrBtProxSrvTxPowerChangedEventInd));
        ind->type                       = CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND;
        ind->txPower                    = proxSrvData->txValue;
        CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, ind);

    }
}


void CsrBtProxSrvWriteEventIndSend(CsrBtProxSrvInstData *proxSrvData, CsrUint16 valueSize, CsrUint8 *value, CsrUint16 handle)
{
    if((proxSrvData->eventMask & CSR_BT_PROX_SRV_EVENT_WRITE) == CSR_BT_PROX_SRV_EVENT_WRITE)
    {
        CsrBtProxSrvWriteEventInd *ind  = CsrPmemAlloc(sizeof(CsrBtProxSrvWriteEventInd));
        ind->type                       = CSR_BT_PROX_SRV_WRITE_EVENT_IND;

        ind->value                      = value;
        ind->valueHandle                = handle;
        ind->valueSize                  = valueSize;
        CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, ind);

    }
    else
    {
        /* we are final destination for pointer - free it! */
        CsrPmemFree(value);
    }
}
void CsrBtProxSrvAlertEventIndSend(CsrBtProxSrvInstData *proxSrvData, CsrBtProxSrvAlertType type, CsrBtProxSrvAlertLevel level )
{
        CsrBtProxSrvAlertEventInd *ind  = CsrPmemAlloc(sizeof(CsrBtProxSrvAlertEventInd));
        ind->type                       = CSR_BT_PROX_SRV_ALERT_EVENT_IND;
        ind->alertLevel                 = level;
        ind->alertType                  = type;
        CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, ind);

}
void CsrBtProxSrvUpdateTxPowerCfmSend(CsrBtProxSrvInstData *proxSrvData)
{
    CsrBtProxSrvUpdateTxPowerCfm *cfm = CsrPmemAlloc(sizeof(CsrBtProxSrvUpdateTxPowerCfm));
    cfm->type                         = CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM;
    cfm->resultSupplier               = CSR_BT_SUPPLIER_PROX_SRV;
    cfm->resultCode                   = CSR_BT_PROX_SRV_RESULT_SUCCESS;
    CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, cfm);
}
void CsrBtProxSrvUpdateBattLevelCfmSend(CsrBtProxSrvInstData *proxSrvData)
{
    CsrBtProxSrvUpdateBattLevelCfm *cfm = CsrPmemAlloc(sizeof(CsrBtProxSrvUpdateBattLevelCfm));
    cfm->type                         = CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM;
    cfm->resultSupplier               = CSR_BT_SUPPLIER_PROX_SRV;
    cfm->resultCode                   = CSR_BT_PROX_SRV_RESULT_SUCCESS;
    CsrSchedMessagePut(proxSrvData->appHandle, CSR_BT_PROX_SRV_PRIM, cfm);
}


/******************************************************************************
CsrBtProxSrvStartDisconnect:
 *****************************************************************************/
void CsrBtProxSrvStartDisconnect(CsrBtProxSrvInstData *proxSrvData)
{
    /*stop  timer*/
    CsrSchedTimerCancel(proxSrvData->txPower_timer,
                        0,
                        NULL);

    /* Disconnecting. Assume we want to accept new connection */
    proxSrvData->state = CSR_BT_PROX_SRV_ST_LISTEN;
    CsrBtGattDisconnectReqSend(proxSrvData->gattId,
                               proxSrvData->leBtConnId);
}

/******************************************************************************
CsrBtProxSrvInitInstanceData:
 *****************************************************************************/
void CsrBtProxSrvClearHandles(CsrBtProxSrvInstData *proxSrvData)
{
    proxSrvData->llValue                           = 0x0000;
    proxSrvData->iAlertValue                       = 0x0000;
    proxSrvData->txValue                           = 0x00;
    proxSrvData->battValue                         = 0x00;
    proxSrvData->battMask                          = (CSR_BT_PROX_SRV_BATT_PRESENT_UNKNOWN | CSR_BT_PROX_SRV_BATT_DISCHARGING_UNKNOWN | CSR_BT_PROX_SRV_BATT_CHARGING_UNKNOWN | CSR_BT_PROX_SRV_BATT_CRITICAL_UNKNOWN);
    proxSrvData->serviceRequired                   = CSR_BT_PROX_SRV_BATT_SERVICE_REQUIRED_UNKNOWN;
    proxSrvData->eventMask                         = 0;

}
void CsrBtProxSrvInitInstanceData(CsrBtProxSrvInstData *proxSrvData)
{
    CsrMemSet(proxSrvData,0,sizeof(CsrBtProxSrvInstData));
    proxSrvData->leBtConnId                        = CSR_BT_CONN_ID_INVALID;
    proxSrvData->connectedBtConnId                 = CSR_BT_CONN_ID_INVALID;
    CsrBtProxSrvClearHandles(proxSrvData);
}

/******************************************************************************
 * Init function called by the scheduler upon initialisation.
 * This function is used to boot the demo application.
 *****************************************************************************/
void CsrBtProxSrvInit(void **gash)
{
    CsrBtProxSrvInstData    *proxSrvData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(CsrBtProxSrvInstData));
    proxSrvData = (CsrBtProxSrvInstData *) *gash;
    CsrBtProxSrvInitInstanceData(proxSrvData);

    proxSrvData->phandle = CsrSchedTaskQueueGet() ;

    proxSrvData->initialized = TRUE;
    proxSrvData->state = CSR_BT_PROX_SRV_ST_LISTEN;
}

#ifdef ENABLE_SHUTDOWN
void CsrBtProxSrvDeinit(void **gash)
{
    CsrUint16 msg_type;
    void *msg_data;
    CsrBtProxSrvInstData *proxSrvData;

    proxSrvData = (CsrBtProxSrvInstData *) (*gash);
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

    CsrPmemFree(proxSrvData);
}
#else
void CsrBtProxSrvDeinit(void **gash)
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
void CsrBtProxSrvHandler(void **gash)
{
    CsrBtProxSrvInstData *proxSrvData;
    CsrUint16    eventType;

    /* get a message from the demoapplication message queue. The message is returned in prim
       and the event type in eventType */
    proxSrvData = (CsrBtProxSrvInstData *) (*gash);

    if(!proxSrvData->restoreFlag)
    { /* No messages on the local save queue                                                */
        CsrSchedMessageGet(&eventType , &proxSrvData->recvMsgP);
    }
    else
    {
        if(!CsrMessageQueuePop(&proxSrvData->saveQueue, &eventType , &proxSrvData->recvMsgP))
        { /* No more messages on the local save queue. Call CsrGetMessage to receive
             a message from the scheduler                                                   */
            proxSrvData->restoreFlag = FALSE;
            CsrSchedMessageGet(&eventType , &proxSrvData->recvMsgP);
        }
        else
        { /* A message has been restored from the local queed                               */
            ;
        }
    }

    switch (eventType)
    {
        case CSR_BT_SC_PRIM:
            {
                CsrBtProxSrvHandleSdPrim(proxSrvData);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                CsrBtProxSrvHandleCmPrim(proxSrvData);
                break;
            }
        case CSR_BT_GATT_PRIM:
            {
                CsrBtProxSrvHandleGattPrim(proxSrvData);
                break;
            }

        case CSR_BT_PROX_SRV_PRIM:
            {
                CsrBtProxSrvHandleProxSrvPrim(proxSrvData);
                break;
            }
        default:
            {
                /* unexpected primitive received */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("CSR_BT_PROX_SRV",
                                    0, 
                                    *((CsrPrim *) proxSrvData->recvMsgP),
                                    "####### default in CsrBtProxSrvHandler,");
#endif
            }
    }
    /* free the received signal again. If the signal for some reason must be stored in the application
     * the pointer (prim) must be set to NULL in order not to free it here
     */
    CsrPmemFree(proxSrvData->recvMsgP);
}

/******************************************************************************
 * Function to handle all SD-primitives.
 ******************************************************************************/
void CsrBtProxSrvHandleSdPrim(CsrBtProxSrvInstData *proxSrvData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) proxSrvData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_SD_READ_DEVICE_INFO_CFM:
            {
                /*lets see if the device is paired or not - add it to client config list if it does not exist */
                CsrBtSdReadDeviceInfoCfm *cfm = (CsrBtSdReadDeviceInfoCfm*)proxSrvData->recvMsgP;
                CsrBtProxSrvClientConfigElement *cElem = CSR_BT_PROX_SRV_FIND_CLIENT_CONFIG_BY_ADDR(proxSrvData->clientConfigList, (void*)(&cfm->deviceAddr));
                if(!cElem)
                {
                    cElem = CSR_BT_PROX_SRV_ADD_CLIENT_CONFIG(proxSrvData->clientConfigList);
                    cElem->deviceAddr.lap = cfm->deviceAddr.lap;
                    cElem->deviceAddr.nap = cfm->deviceAddr.nap;
                    cElem->deviceAddr.uap = cfm->deviceAddr.uap;
                }
                cElem->paired = ((cfm->deviceStatus & CSR_BT_SD_STATUS_PAIRED) == CSR_BT_SD_STATUS_PAIRED)?TRUE:FALSE;
                break;
            }
        default:
            {
                /* We do not care about anything else from the SD and should not receive any*/
            }


    }
}


/******************************************************************************
 * Function to handle all cm-primitives.
 ******************************************************************************/
void CsrBtProxSrvHandleCmPrim(CsrBtProxSrvInstData *proxSrvData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) proxSrvData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_CM_READ_TX_POWER_LEVEL_CFM:
            {
                CsrBtCmReadTxPowerLevelCfm *cfm = (CsrBtCmReadTxPowerLevelCfm*)proxSrvData->recvMsgP;
                if(proxSrvData->txValue != cfm->powerLevel)
                {
                        if(proxSrvData->leBtConnId != CSR_BT_CONN_ID_INVALID) /*TX POWER never changes on LE */
                        {
                            CsrInt8 *value = CsrPmemAlloc(1);
                            proxSrvData->txValue = cfm->powerLevel;
                            CsrMemCpy(&value[0], &proxSrvData->txValue, 1);
                                CsrBtGattNotificationEventReqSend(proxSrvData->gattId, proxSrvData->leBtConnId,
                                                                  (CsrUint16)(CSR_BT_PROX_SRV_TX_POWER_HANDLE+proxSrvData->dbStartHandle+2),1,(CsrUint8*)value);
                        }
                        CsrBtProxSrvTxPowerChangedEventIndSend(proxSrvData);
                }
                break;
            }

    }
}
/******************************************************************************
 * Function to handle all GATT-primitives.
 ******************************************************************************/
void CsrBtProxSrvHandleProxSrvPrim(CsrBtProxSrvInstData *proxSrvData)
{
    CsrPrim *type = (CsrPrim *) proxSrvData->recvMsgP;

    switch(*type)
    {
        case CSR_BT_PROX_SRV_ACTIVATE_REQ:
            { 
                CsrBtProxSrvActivateReq *prim  =  proxSrvData->recvMsgP;
                proxSrvData->appHandle         =  prim->appHandle;
                proxSrvData->eventMask         =  prim->eventMask;
                if(prim->clientConfigSize)
                {
                     csrBtProxSrvConvertMemToClientConfigList(proxSrvData, prim->clientConfig, prim->clientConfigSize);
                }
                /* Register with Gatt, then add DB and then start listening for connections (advertise) */
                CsrBtGattRegisterReqSend(CSR_BT_PROX_SRV_IFACEQUEUE, 0);
                break;
            }
        case CSR_BT_PROX_SRV_DEACTIVATE_REQ:
            {
                /*unregister with gatt*/
                proxSrvData->state = CSR_BT_PROX_SRV_ST_STOP;
                CsrBtProxSrvCrank(proxSrvData);
                CsrBtGattUnregisterReqSend(proxSrvData->gattId);


                break;
            }
        case CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ:
            {
                CsrBtProxSrvUpdateTxPowerReq *prim = proxSrvData->recvMsgP;
                proxSrvData->txValue               = prim->txPower;
                CsrBtProxSrvUpdateTxPowerCfmSend(proxSrvData);
                break;
            }

        case CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ:
            {
                CsrBtProxSrvUpdateBattLevelReq *prim = proxSrvData->recvMsgP;
                proxSrvData->battValue               = prim->battLevel;
                proxSrvData->battMask                = prim->battMask;
                proxSrvData->serviceRequired         = prim->serviceRequired;
                CsrBtProxSrvUpdateBattLevelCfmSend(proxSrvData);
                break;
            }
        default:
            { /* Do nothing */
                break;
            }
    }

}
/******************************************************************************
 * Function to handle all GATT-primitives.
 ******************************************************************************/
void CsrBtProxSrvHandleGattPrim(CsrBtProxSrvInstData *proxSrvData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) proxSrvData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_GATT_REGISTER_CFM:
            {
                CsrBtGattRegisterCfm *cfm = (CsrBtGattRegisterCfm*)proxSrvData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    proxSrvData->gattId = cfm->gattId;
                    /* Allocate and add DB in Gatt*/
                    CsrBtGattDbAllocReqSend(proxSrvData->gattId, CSR_BT_PROX_SRV_DB_HANDLE_COUNT, CSR_BT_PROX_SRV_DB_PREFERRED_HANDLE );
                }
                else
                {
                    /*if this failed the activation failed */
                    CsrBtProxSrvActivateCfmSend(proxSrvData, cfm->resultCode, cfm->resultSupplier);
                }

                break;
            }
        case CSR_BT_GATT_DB_ADD_CFM:
            {
                /*Get ready */
                CsrBtGattDbAddCfm *cfm = (CsrBtGattDbAddCfm*)proxSrvData->recvMsgP;
                proxSrvData->state = CSR_BT_PROX_SRV_ST_LISTEN;
                CsrBtProxSrvCrank(proxSrvData);
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrBtProxSrvActivateCfmSend(proxSrvData, CSR_BT_PROX_SRV_RESULT_SUCCESS, CSR_BT_SUPPLIER_PROX_SRV);
                }
                else
                {
                    CsrBtProxSrvActivateCfmSend(proxSrvData, cfm->resultCode, cfm->resultSupplier);
                }
                break;
            }
        case CSR_BT_GATT_UNREGISTER_CFM:
            {
                CsrBtGattUnregisterCfm *cfm = (CsrBtGattUnregisterCfm*)proxSrvData->recvMsgP;
                /*We only unregister on deactivate and hence need to send a cfm*/
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrBtProxSrvDeactivateCfmSend(proxSrvData, CSR_BT_GATT_RESULT_SUCCESS, CSR_BT_SUPPLIER_PROX_SRV);
                }
                else
                {
                    CsrBtProxSrvDeactivateCfmSend(proxSrvData, cfm->resultCode, cfm->resultSupplier);
                }
                /*Clear handles */
                CsrBtProxSrvClearHandles(proxSrvData);
                break;
            }
        case CSR_BT_GATT_DB_ALLOC_CFM:
            {
                CsrBtGattDbAllocCfm *cfm = (CsrBtGattDbAllocCfm*)proxSrvData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrBtGattDb *db = CsrBtProxSrvDbCreate(cfm->start);
                    CsrBtGattDbAddReqSend(proxSrvData->gattId, db);
                    proxSrvData->dbStartHandle = cfm->start;
                    proxSrvData->dbEndHandle = cfm->end;
                }
                else
                {
                    /*if this failed the activation failed */
                    CsrBtProxSrvActivateCfmSend(proxSrvData, cfm->resultCode, cfm->resultSupplier);
                }
                break;
            }
        case CSR_BT_GATT_CONNECT_IND:
            {
                CsrBtGattConnectInd        *ind;
                ind = (CsrBtGattConnectInd *) proxSrvData->recvMsgP;

                /* Crank state to disconnect the other */

                if (ind->resultSupplier == CSR_BT_SUPPLIER_GATT &&
                    ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /* send info to app*/
                    proxSrvData->connectedDeviceAddr.addr.nap = ind->address.addr.nap;
                    proxSrvData->connectedDeviceAddr.addr.lap = ind->address.addr.lap;
                    proxSrvData->connectedDeviceAddr.addr.uap = ind->address.addr.uap;
                    CsrBtProxSrvConnectIndSend(proxSrvData, CSR_BT_PROX_SRV_RESULT_SUCCESS, CSR_BT_SUPPLIER_PROX_SRV);

                    proxSrvData->state = CSR_BT_PROX_SRV_ST_LE;
                    proxSrvData->connectedBtConnId = proxSrvData->leBtConnId;


                }
                else
                {
                    CsrBtProxSrvConnectIndSend(proxSrvData, ind->resultCode, ind->resultSupplier);
                    /*  we failed to connect */
                    proxSrvData->leBtConnId = CSR_BT_CONN_ID_INVALID;
                    proxSrvData->state = CSR_BT_PROX_SRV_ST_LISTEN;

                }

                /* Crank state */
                CsrBtProxSrvCrank(proxSrvData);
                break;
            }
        case CSR_BT_GATT_DISCONNECT_IND:
            {
                CsrBtGattDisconnectInd    *ind;

                ind = (CsrBtGattDisconnectInd *) proxSrvData->recvMsgP;

                /* Determine which one got disconnected */

                if (ind->btConnId == proxSrvData->leBtConnId)
                {
                    proxSrvData->leBtConnId = CSR_BT_CONN_ID_INVALID;
                }

                if (ind->btConnId == proxSrvData->connectedBtConnId)
                {
                    proxSrvData->connectedBtConnId = CSR_BT_CONN_ID_INVALID;
                    if(proxSrvData->state != CSR_BT_PROX_SRV_ST_STOP)
                    {
                        /*if we are in STOP state it means that we have been deactivated and hence should not reconnect */
                        proxSrvData->state = CSR_BT_PROX_SRV_ST_LISTEN;
                        CsrBtProxSrvCrank(proxSrvData);
                    }
                    

                    CsrBtProxSrvDisconnectIndSend(proxSrvData, ind->reasonCode, ind->reasonSupplier);
                    /*send link loss alert if value is set */
                    if(proxSrvData->llValue)
                    {
                        CsrBtProxSrvAlertEventIndSend(proxSrvData, CSR_BT_PROX_SRV_ALERT_LINKLOSS, proxSrvData->llValue);
                    }
                    /*Stop the TIMER */
                    CsrSchedTimerCancel(proxSrvData->txPower_timer,
                                        0,
                                        NULL);
                    /*Clear handles */
                    CsrBtProxSrvClearHandles(proxSrvData);
                }

                break;
            }
        case CSR_BT_GATT_PERIPHERAL_CFM:
            {
                CsrBtGattPeripheralCfm *cfm;
                cfm = (CsrBtGattPeripheralCfm*) proxSrvData->recvMsgP;

                if((cfm->resultCode != CSR_BT_GATT_RESULT_SUCCESS)
                   || (cfm->resultSupplier != CSR_BT_SUPPLIER_GATT))
                {
                    /*this is probably a timeout so we can just restart the peripheral process*/
                    proxSrvData->leBtConnId = CSR_BT_CONN_ID_INVALID;
                    sendGattPeripheralReq(proxSrvData->gattId);    
                }
                else if ((cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                         && (cfm->resultSupplier == CSR_BT_SUPPLIER_GATT))
                {
                    /* ConnId assigned, but not yet connected */
                    proxSrvData->leBtConnId = cfm->btConnId;
                }

                break;
            }
        case CSR_BT_GATT_DB_ACCESS_WRITE_IND:
            {
                CsrBtGattDbAccessWriteInd *ind = (CsrBtGattDbAccessWriteInd*)proxSrvData->recvMsgP;
                CsrUint16 handle =  ind->writeUnit[0].attrHandle - proxSrvData->dbStartHandle -2 /*we get value handle not service handle */; 

                switch(handle)
                {
                    case (CSR_BT_PROX_SRV_TX_POWER_CC_HANDLE-2): /*client config change*/
                        {
                            CsrBtProxSrvClientConfigElement *cElem = 
                                CSR_BT_PROX_SRV_FIND_CLIENT_CONFIG_BY_ADDR(proxSrvData->clientConfigList, 
                                                                           (void*)(&proxSrvData->connectedDeviceAddr.addr));
                            if(!cElem)
                            {
                                /*We really should never get in-here as the clientConfig should have been added elsewhere */
                                /* no previous clientConfig record in list
                                 * lets add one */
                                cElem = CSR_BT_PROX_SRV_ADD_CLIENT_CONFIG(proxSrvData->clientConfigList);
                                cElem->deviceAddr = proxSrvData->connectedDeviceAddr.addr;
                            }
                            /*update with latest write value - this is stored per device */
                            CsrPmemFree(cElem->value);
                            cElem->value      = ind->writeUnit[0].value;
                            proxSrvData->txPower_timer = CsrSchedTimerSet(CSR_BT_PROX_SRV_TX_POWER_TIMER_INTERVAL,
                                                                          CsrBtProxSrvTxPowerTimerInterupt,
                                                                          0,
                                                                          (void*)proxSrvData);
                            CsrBtProxSrvTxPowerChangedEventIndSend(proxSrvData);
                            break;
                        }
                    case CSR_BT_PROX_SRV_IALERT_HANDLE:
                        {
                            if((ind->writeUnit[0].value[0] == CSR_BT_GATT_SERVICE_ALERT_LEVEL_OFF ||
                                ind->writeUnit[0].value[0] == CSR_BT_GATT_SERVICE_ALERT_LEVEL_LOW ||
                                ind->writeUnit[0].value[0] == CSR_BT_GATT_SERVICE_ALERT_LEVEL_HIGH) &&
                                ind->writeUnit[0].valueLength == 2)
                            {
                                proxSrvData->iAlertValue = ind->writeUnit[0].value[0];
                                CsrBtProxSrvAlertEventIndSend(proxSrvData, CSR_BT_PROX_SRV_ALERT_IMMEDIATE, proxSrvData->iAlertValue);
                                /*let the app know that they try to write */
                                CsrBtProxSrvWriteEventIndSend(proxSrvData, ind->writeUnit[0].valueLength, ind->writeUnit[0].value, ind->writeUnit[0].attrHandle);
                                /* tell Gatt that write is ok*/
                                CsrBtGattDbWriteAccessResSend(proxSrvData->gattId,
                                                              ind->btConnId,
                                                              ind->writeUnit[0].attrHandle,
                                                              CSR_BT_GATT_ACCESS_RES_SUCCESS);
                            }
                            else
                            {
                                CsrPmemFree(ind->writeUnit[0].value);
                                CsrBtGattDbWriteAccessResSend(proxSrvData->gattId,
                                                              ind->btConnId,
                                                              ind->writeUnit[0].attrHandle,
                                                              CSR_BT_GATT_ACCESS_RES_WRITE_NOT_PERMITTED);
                            }
                            break;
                        }
                    case CSR_BT_PROX_SRV_LINKLOSS_HANDLE:
                        {
                            if((ind->writeUnit[0].value[0] == CSR_BT_GATT_SERVICE_ALERT_LEVEL_OFF ||
                                ind->writeUnit[0].value[0] == CSR_BT_GATT_SERVICE_ALERT_LEVEL_LOW ||
                                ind->writeUnit[0].value[0] == CSR_BT_GATT_SERVICE_ALERT_LEVEL_HIGH) &&
                                ind->writeUnit[0].valueLength == 2)
                            {
                                proxSrvData->llValue = ind->writeUnit[0].value[0];
                                /*let the app know that they try to write */
                                CsrBtProxSrvWriteEventIndSend(proxSrvData, ind->writeUnit[0].valueLength, ind->writeUnit[0].value, ind->writeUnit[0].attrHandle);
                                /* tell Gatt that write is ok*/
                                CsrBtGattDbWriteAccessResSend(proxSrvData->gattId,
                                                              ind->btConnId,
                                                              ind->writeUnit[0].attrHandle,
                                                              CSR_BT_GATT_ACCESS_RES_SUCCESS);
                            }
                            else
                            {
                                CsrPmemFree(ind->writeUnit[0].value);
                                CsrBtGattDbWriteAccessResSend(proxSrvData->gattId,
                                                              ind->btConnId,
                                                              ind->writeUnit[0].attrHandle,
                                                              CSR_BT_GATT_ACCESS_RES_WRITE_NOT_PERMITTED);

                            }
                            break;
                        }
                } 
                CsrPmemFree(ind->writeUnit); /* the value pointers have been passed on, free the member and set reference to NULL
                                                to avoid free of values in use.*/
                ind->writeUnit = NULL;
                break;

            }
        case CSR_BT_GATT_DB_ACCESS_READ_IND:
            {
                CsrBtGattDbAccessReadInd *ind = (CsrBtGattDbAccessReadInd*)proxSrvData->recvMsgP;
                CsrUint8 *val = CsrPmemZalloc(2);
                val[0] = 0;
                switch (ind->attrHandle - 2 /*value offset*/ - proxSrvData->dbStartHandle )
                {
                    case CSR_BT_PROX_SRV_TX_POWER_HANDLE:
                        {
                            val[0] = proxSrvData->txValue;
                            break;
                        }
                    case CSR_BT_PROX_SRV_BATTERY_HANDLE:
                        {
                            val[0] = (CsrUint8)proxSrvData->battMask;
                            break;
                        }
                    case (CSR_BT_PROX_SRV_BATTERY_HANDLE+2):
                        {
                            val[0] = (CsrUint8)proxSrvData->battValue;
                            break;
                        }
                    case (CSR_BT_PROX_SRV_BATTERY_HANDLE+4):
                        {
                            val[0] = (CsrUint8)proxSrvData->serviceRequired;
                            break;
                        }
                    case CSR_BT_PROX_SRV_TX_POWER_CC_HANDLE-2:
                        {
                            CsrBtProxSrvClientConfigElement *cElem = CSR_BT_PROX_SRV_FIND_CLIENT_CONFIG_BY_ADDR(proxSrvData->clientConfigList, (void*)(&proxSrvData->connectedDeviceAddr.addr));
                            if(!cElem)
                            {
                                val[0] = 0;
                            }
                            else
                            {
                                val[0] = cElem->value[0];

                            }
                            break;
                        }
                            
                }
                /* tell Gatt that write is ok*/
                CsrBtGattDbReadAccessResSend(proxSrvData->gattId,
                                             ind->btConnId,
                                             ind->attrHandle, 
                                             CSR_BT_GATT_ACCESS_RES_SUCCESS,
                                             2 /*value length*/,
                                             val);
                break;

            }

        default:
            {
                /*we do not know what to do here...free message and ignore? */
                CsrBtGattFreeUpstreamMessageContents(CSR_BT_GATT_PRIM, proxSrvData->recvMsgP);
                /* unexpected primitive received */
                break;
            }
    }
}
