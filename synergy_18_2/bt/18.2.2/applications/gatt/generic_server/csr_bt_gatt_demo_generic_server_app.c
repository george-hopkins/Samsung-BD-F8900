/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      $Revision: #1 $
 ****************************************************************************/

#include "csr_synergy.h"

#include <string.h>
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_exceptionhandler.h"
#include "csr_app_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_gap_app_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_gatt_lib.h"
#include "csr_bt_gatt_demo_app.h"
#include "csr_bt_gatt_demo_defines.h"
#include "csr_bt_gatt_demo_generic_server_app_private_prim.h"
#include "csr_bt_gatt_demo_generic_server_app_ui_sef.h"
#include "csr_bt_gatt_demo_generic_srv_lib.h"
#include "csr_bt_gatt_demo_generic_srv_db.h"
#include "csr_bt_gatt_demo_generic_srv_prim.h"
#include "csr_bt_gatt_app_task.h"

/* Forward declarations for handler functions */
void CsrBtGenericSrvAppHandleGenericSrvPrim(CsrBtGenericSrvAppInstData *instData);
void CsrBtGenericSrvAppHandleCmPrim(CsrBtGenericSrvAppInstData *instData);
void CsrBtGenericSrvAppHandleSdPrim(CsrBtGenericSrvAppInstData *instData);



/*used to simulate temperature changes */
CsrUint8 simplePseudoRandom(CsrUint8 lim)
{
    static CsrUint8 a = 1; 
    a = (a * 32719 + 3) % 32749;
    return ((a % lim) + 1);
}

CsrBool CsrBtGenericSrvAppFindClientConfigByAddr(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGenericSrvAppClientConfigElement* ccElem = (CsrBtGenericSrvAppClientConfigElement*)elem;
    CsrBtDeviceAddr* currentAddr = (CsrBtDeviceAddr *)value;
    if(ccElem->deviceAddr.nap == currentAddr->nap)
    {
        if(ccElem->deviceAddr.lap == currentAddr->lap)
        {
            if(ccElem->deviceAddr.uap == currentAddr->uap)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/* Timer deactivated in this demo*/
/*void CsrBtGenericSrvAppUpdateTimerInterupt(CsrUint16 mi, void *mv)
{
    CsrBtGenericSrvAppInstData *inst = (CsrBtGenericSrvAppInstData*)mv;
    CsrUint8 *value = CsrPmemAlloc(5); */ /* 1xflags + 4xdata */ /*
    float   fVal =(float)simplePseudoRand(100);
    value[0] = inst->tempType;
    CsrMemCpy(&value[1], &fVal, 4);

    CsrBtThermSrvUpdateTemperatureReqSend(value,5);
    inst->update_timer = CsrSchedTimerSet(CSR_BT_GENERIC_SRV_APP_UPDATE_TIMER_INTERVAL,
                                          CsrBtGenericSrvAppUpdateTimerInterupt,
                                          mi,
                                          mv);

}*/

/******************************************************************************
CsrBtGenericSrvAppStartActivate:
 *****************************************************************************/
void CsrBtGenericSrvAppStartActivate(CsrBtGenericSrvAppInstData *inst)
{
    CsrBtGattRegisterReqSend(inst->phandle, 1234);
}
/******************************************************************************
CsrBtGenericSrvAppStartDeactivating:
 *****************************************************************************/
void CsrBtGenericSrvAppStartDeactivate(CsrBtGenericSrvAppInstData *inst)
{
    CsrBtGenericSrvDeactivateReqSend();
    CsrBtGattUnregisterReqSend(inst->gattId);
}

/******************************************************************************
CsrBtGenericSrvAppStartDisconnect:
 *****************************************************************************/
void CsrBtGenericSrvAppStartDisconnect(CsrBtGenericSrvAppInstData *inst)
{
    /*stop update timer*/
    /*CsrSchedTimerCancel(inst->update_timer,NULL,NULL);*/

    /*we actually deactivate as the server will keep being reconnectable as long as it is activated */
    CsrBtGenericSrvDeactivateReqSend();
    /*reset BT connection ID */
    inst->btConnId = 0xff;
}

/******************************************************************************
CsrBtThermsAppInitInstanceData:
 *****************************************************************************/
void CsrBtGenericSrvAppClearHandles(CsrBtGenericSrvAppInstData *thermsData){

}
void CsrBtGenericSrvAppInitInstanceData(CsrBtGenericSrvAppInstData *instData)
{
    CsrMemSet(instData,0,sizeof(CsrBtGenericSrvAppInstData));

    instData->btConnId                          = 0xff;
    instData->csrUiVar.hMainMenu                = CSR_UI_DEFAULTACTION;
    instData->csrUiVar.hCommonDialog            = CSR_UI_DEFAULTACTION;
    CsrBtGenericSrvAppClearHandles(instData);
}

static void csrBtGenericSrvAppSendGenericSrvAppRegisterReq(CsrSchedQid pHandle)
{
    CsrBtGenericSrvAppRegisterReq *prim;

    prim = (CsrBtGenericSrvAppRegisterReq *) CsrPmemAlloc(sizeof(CsrBtGenericSrvAppRegisterReq));
    prim->type = CSR_BT_GENERIC_SRV_APP_REGISTER_REQ;
    prim->phandle = pHandle;

    CsrSchedMessagePut(CSR_BT_GENERIC_SERVER_APP_IFACEQUEUE, CSR_BT_GENERIC_SERVER_APP_PRIM,prim);
}

static void csrBtGenericSrvAppSendGenSrvAppRegisterCfm(CsrSchedQid phandle, CsrSchedQid queueId, CsrUint8 instance)
{
    CsrBtGenericSrvAppRegisterCfm *prim;

    prim = (CsrBtGenericSrvAppRegisterCfm *) CsrPmemAlloc(sizeof(CsrBtGenericSrvAppRegisterCfm));
    prim->type = CSR_BT_GENERIC_SRV_APP_REGISTER_CFM; 
    prim->queueId = queueId;
    prim->instance = instance;

    CsrSchedMessagePut(phandle, CSR_BT_GENERIC_SERVER_APP_PRIM,prim);
}

static char * csrBtGenericSrvAppReturnGenSrvInstName(CsrBtGenericSrvAppInstData *instance)
{
    char *buf;
    buf = CsrPmemAlloc(CsrStrLen("Generic Server")+1);

    sprintf(buf, "Generic Server");
    return buf;
}

/******************************************************************************
 * Init function called by the scheduler upon initialisation.
 * This function is used to boot the demo application.
 *****************************************************************************/
void CsrBtGenericServerAppInit(void **gash)
{
    CsrBtGenericSrvAppInstData    *instData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(CsrBtGenericSrvAppInstData));
    instData = (CsrBtGenericSrvAppInstData *) *gash;
    CsrBtGenericSrvAppInitInstanceData(instData);

    instData->phandle = CsrSchedTaskQueueGet() ;

    instData->initialized = TRUE;

    /* Start UI creation cycle */
    CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);

    csrBtGenericSrvAppSendGenericSrvAppRegisterReq(instData->phandle);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtGenericServerAppDeinit(void **gash)
{
    CsrUint16 msg_type;
    void *msg_data;
    CsrBtGenericSrvAppInstData *instData;

    instData = (CsrBtGenericSrvAppInstData *) (*gash);
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

    CsrPmemFree(instData);
}
#else
void CsrBtGenericServerAppDeinit(void **gash)
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
void CsrBtGenericServerAppHandler(void **gash)
{
    CsrBtGenericSrvAppInstData *instData;
    CsrUint16    eventType;

    /* get a message from the demoapplication message queue. The message is returned in prim
       and the event type in eventType */
    instData = (CsrBtGenericSrvAppInstData *) (*gash);

    if(!instData->restoreFlag)
    { /* No messages on the local save queue                                                */
        CsrSchedMessageGet(&eventType , &instData->recvMsgP);
    }
    else
    {
        if(!CsrMessageQueuePop(&instData->saveQueue, &eventType , &instData->recvMsgP))
        { /* No more messages on the local save queue. Call CsrGetMessage to receive
             a message from the scheduler                                                   */
            instData->restoreFlag = FALSE;
            CsrSchedMessageGet(&eventType , &instData->recvMsgP);
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
                CsrBtGenericSrvAppHandleSdPrim(instData);
                break;
            }
        case CSR_BT_GATT_PRIM:
            {
                CsrBtGenericSrvAppHandleGenericSrvPrim(instData);
                break;
            }

        case CSR_BT_GENERIC_SERVER_APP_PRIM:
            {
                CsrPrim *type = (CsrPrim *) instData->recvMsgP;

                switch(*type)
                {
                    case CSR_BT_GENERIC_SRV_APP_REGISTER_REQ:
                        { /* only received by controling Generic Server demo app instance */
                            if(instData->initialized)
                            {
                                CsrSchedQid queueId = CSR_SCHED_QID_INVALID;
                                CsrBtGenericSrvAppRegisterReq *prim = instData->recvMsgP;


                                instData->genSrvAppHandle = prim->phandle;
                                queueId = instData->genSrvProfileHandle;
                                if(queueId != CSR_SCHED_QID_INVALID)
                                {
                                    csrBtGenericSrvAppSendGenSrvAppRegisterCfm(prim->phandle, queueId,1);
                                }
                                else
                                {
                                    CsrGeneralException("CSR_GENERIC_SERVER_DEMO_APP", 
                                                        0, 
                                                        *type,
                                                        "No CsrBtGenericServer profile Queue Handle Available");
                                }
                            }
                            else
                            {
                                CsrMessageQueuePush(&instData->saveQueue, CSR_BT_GENERIC_SERVER_APP_PRIM, instData->recvMsgP);
                                instData->recvMsgP = NULL;
                            }
                            break;
                        }
                    case CSR_BT_GENERIC_SRV_APP_REGISTER_CFM:
                        { /* Only received by extra Generic Server demo app instances */
                            instData->profileName = csrBtGenericSrvAppReturnGenSrvInstName(instData);

                            CsrBtGattAppHandleGenericSrvSetMainMenuHeader(instData);

                            instData->initialized = TRUE;

                            CsrAppRegisterReqSend(instData->phandle, TECH_BTLE, PROFILE_NAME(instData->profileName));
                            break;
                        }
                    default:
                        { /* Do nothing */
                            break;
                        }
                }
                break;
            }
        case CSR_BT_GAP_APP_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) instData->recvMsgP;
                if (*prim == CSR_BT_GAP_APP_GET_SELECTED_DEVICE_CFM)
                {
                    CsrBtGapAppGetSelectedDeviceCfm *prim = instData->recvMsgP;
                    instData->selectedDeviceAddr.addr.lap = prim->deviceAddr.lap;
                    instData->selectedDeviceAddr.addr.nap = prim->deviceAddr.nap;
                    instData->selectedDeviceAddr.addr.uap = prim->deviceAddr.uap;
                    instData->selectedDeviceAddr.type     = (prim->deviceStatus & CSR_BT_SD_STATUS_PRIVATE_ADDR)? TBDADDR_RANDOM:TBDADDR_PUBLIC;
                    CsrAppBacklogReqSend(TECH_BTLE, 
                                         PROFILE_NAME(instData->profileName), 
                                         FALSE, 
                                         "Received selected DeviceAddr: %04X:%02X:%06X (%s)", 
                                         prim->deviceAddr.nap, 
                                         prim->deviceAddr.uap, 
                                         prim->deviceAddr.lap,
                                         (CsrBtAddrIsRandom(instData->selectedDeviceAddr))? "Random":"Public");
                    /*update UI*/
                    CsrUiUieShowReqSend(instData->csrUiVar.hMainMenu,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_BT_GENERIC_SRV_APP_CSR_UI_PRI);
                }
                else
                {
                    /* unexpected primitive received */
                    CsrGeneralException("CSR_BT_GENERIC_SERVER_APP", 
                                        0, 
                                        *prim, 
                                        "####### Unhandled CSR_BT_GAP_APP_PRIM in CsrBtGenericServerAppHandler,");
                }
                break;
            }
        case CSR_APP_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) instData->recvMsgP;

                if (*prim == CSR_APP_TAKE_CONTROL_IND)
                { /* We need to query the GAP task for the currently selected device addr */
                    CsrBtGapAppGetSelectedDeviceReqSend(CsrSchedTaskQueueGet());
                }
                else if (*prim == CSR_APP_REGISTER_CFM)
                { /* Just ignore this primitive since we only register one menu item we don't need to remember the controlId */
                    ;
                }
                else
                {
                    /* unexpected primitive received */
                    CsrGeneralException("CSR_BT_GENERIC_SERVER_APP", 
                                        0, 
                                        *prim, 
                                        "####### Unhandled CSR_APP_PRIM in CsrBtGenericServerAppHandler,");
                }
                break;
            }
        case CSR_UI_PRIM:
            {
                CsrBtGattAppHandleGenericSrvCsrUiPrim(instData);
                break;
            }

        default:
            {
                /* unexpected primitive received */
                CsrGeneralException("CSR_BT_GENERIC_SERVER_APP",
                                    0, 
                                    *((CsrPrim *) instData->recvMsgP),
                                    "####### default in CsrBtGenericServerAppHandler,");
            }
    }
    /* free the received signal again. If the signal for some reason must be stored in the application
     * the pointer (prim) must be set to NULL in order not to free it here
     */
    CsrPmemFree(instData->recvMsgP);
}
/******************************************************************************
 * Function to handle all SD-primitives.
 ******************************************************************************/
void CsrBtGenericSrvAppHandleSdPrim(CsrBtGenericSrvAppInstData *instData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) instData->recvMsgP;
    switch (*primType)
    {
        default:
            {
                CsrAppBacklogReqSend(TECH_BTLE, 
                                     PROFILE_NAME(instData->profileName), 
                                     TRUE, 
                                     "####### default in SD prim handler 0x%04x,",
                                     *primType);
                CsrBtGattFreeUpstreamMessageContents(CSR_BT_SD_PRIM, instData->recvMsgP);
                /* unexpected primitive received */

            }


    }
}


/******************************************************************************
 * Function to handle all genericSrv-primitives.
 ******************************************************************************/
void CsrBtGenericSrvAppHandleGenericSrvPrim(CsrBtGenericSrvAppInstData *instData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) instData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_GATT_REGISTER_CFM:
            {
                CsrBtGattRegisterCfm *cfm = (CsrBtGattRegisterCfm*)instData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    instData->gattId = cfm->gattId;
                    /* subscribe to notifications about new physical link connections/loss - needed for clientConfig */
                    CsrBtGattSetEventMaskReqSend(cfm->gattId,CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_NONE /*PHYSICAL_LINK_STATUS*/);
                }
                else
                {

                }

                break;
            }
        case CSR_BT_GATT_SET_EVENT_MASK_CFM:
            {
                /*event mask set, now lets move on to DB setup */
                /* Allocate and add DB in Gatt*/
                CsrBtGattDbAllocReqSend(instData->gattId, CSR_BT_GENERIC_SRV_DB_HANDLE_COUNT, CSR_BT_GENERIC_SRV_DB_PREFERRED_HANDLE );
                break;
            }
        case CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND:
            {
                /* a physical link has either been connected or disconnected
                 * we need to get info about the connection in order to add/remove the item from
                 * the client config list 
                 */
                CsrBtGattPhysicalLinkStatusInd *ind = (CsrBtGattPhysicalLinkStatusInd*)instData->recvMsgP;
                CsrBtSdReadDeviceInfoReqSendEx(instData->phandle, ind->address.addr, ind->address.type, 0);
                break;
            }
        case CSR_BT_GATT_DB_ADD_CFM:
            {
                CsrBtTypedAddr addr;

                addr.addr.lap = 0;
                addr.addr.nap = 0;
                addr.addr.uap = 0;
                CsrBtGattPeripheralReqSend(instData->gattId,
                                            addr,
                                            CSR_BT_GATT_FLAGS_UNDIRECTED,
                                            0);
                break;
            }
        case CSR_BT_GATT_UNREGISTER_CFM:
            {
                /* Not able to Register/Unregister from UI */

                break;
            }
        case CSR_BT_GATT_DB_ALLOC_CFM:
            {
                CsrBtGattDbAllocCfm *cfm = (CsrBtGattDbAllocCfm*)instData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                        CsrBtGattDb *db = CsrBtGenericSrvDbCreate(cfm->start);
                        CsrBtGattDbAddReqSend(instData->gattId, db);
                        instData->dbStartHandle = cfm->start;
                        instData->dbEndHandle = cfm->end;

                        
                }
                else
                {
                        /*We could show error about not being able to allocate */
                }
                

                break;
            }
        case CSR_BT_GATT_CONNECT_IND:
            {
                CsrBtGattConnectInd        *ind;

                ind = (CsrBtGattConnectInd *) instData->recvMsgP;
                instData->btConnId = ind->btConnId;

                if (ind->resultSupplier == CSR_BT_SUPPLIER_GATT &&
                    ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrAppBacklogReqSend(TECH_BTLE, 
                                         PROFILE_NAME(instData->profileName), 
                                         TRUE, 
                                         "Successfully Connected with: %04X:%02X:%06X", 
                                         ind->address.addr.nap, 
                                         ind->address.addr.uap, 
                                         ind->address.addr.lap);
                    instData->connectedDeviceAddr.addr.nap = ind->address.addr.nap;
                    instData->connectedDeviceAddr.addr.lap = ind->address.addr.lap;
                    instData->connectedDeviceAddr.addr.uap = ind->address.addr.uap;
                    /*instData->update_timer = CsrSchedTimerSet(CSR_BT_THERMS_APP_UPDATE_TIMER_INTERVAL,
                                                             CsrBtThermsAppUpdateTimerInterupt,
                                                             0,
                                                             (void*)thermsData);*/

                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BTLE, 
                                         PROFILE_NAME(instData->profileName), 
                                         TRUE, 
                                         "Failed to connect with: %04X:%02X:%06X, resultCode=%d, resultSupplier=%d", 
                                         ind->address.addr.nap, 
                                         ind->address.addr.uap, 
                                         ind->address.addr.lap, 
                                         ind->resultCode, 
                                         ind->resultSupplier);
                }
                /*update UI*/
                CsrBtGattAppHandleGenericSrvCsrUiActivateDeactivate(instData, 
                                                              TRUE,
                                                              (CsrBool)((ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS && 
                                                                         ind->resultSupplier == CSR_BT_SUPPLIER_GATT) ? TRUE : FALSE));
                break;
            }
        case CSR_BT_GATT_DISCONNECT_IND:
            {
                CsrBtGattDisconnectInd    *ind;

                ind      = (CsrBtGattDisconnectInd *) instData->recvMsgP;

                CsrAppBacklogReqSend(TECH_BTLE, 
                                     PROFILE_NAME(instData->profileName), 
                                     TRUE, 
                                     "Disconnected from: %04X:%02X:%06X", 
                                     ind->address.addr.nap, 
                                     ind->address.addr.uap, 
                                     ind->address.addr.lap);
                /*reset instance data */
                /*reset the btConnId */
                instData->btConnId = 0xff;
                /*Stop the UPDATE TIMER */
                /*CsrSchedTimerCancel(instData->update_timer,
                                    0,
                                    NULL);*/
                /*Clear all handles in instance */
                CsrBtGenericSrvAppClearHandles(instData);

                /*update UI*/
                CsrBtGattAppHandleGenericSrvCsrUiActivateDeactivate(instData, 
                                                              FALSE,
                                                              TRUE);


                break;
            }
        case CSR_BT_GATT_PERIPHERAL_CFM:
            {
                CsrBtGattPeripheralCfm *cfm;
                cfm = (CsrBtGattPeripheralCfm*) instData->recvMsgP;

                if(cfm->resultCode != CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /*We only care about non-successful as success will be handled on CONNECT_IND*/
                    CsrBtGattAppHandleGenericSrvCsrUiActivateDeactivate(instData, 
                                                                  TRUE,
                                                                  (CsrBool)((cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS && 
                                                                             cfm->resultSupplier == CSR_BT_SUPPLIER_GATT) ? TRUE : FALSE));

                }
                break;
            }
        case CSR_BT_GATT_DB_ACCESS_WRITE_IND:
            {
                CsrBtGenericSrvAppClientConfigElement *cElem;
                CsrBtGattDbAccessWriteInd *ind = (CsrBtGattDbAccessWriteInd*)instData->recvMsgP;
                CsrUint16 handle =  ind->writeUnit[0].attrHandle - instData->dbStartHandle; 
                CsrUint8 *clientAddress = CsrPmemZalloc(sizeof(CsrUint8)*60);
                switch(handle)
                {  /* Not the generic way. This switch require knowledge of the database prior to compiling */
                    case CSR_BT_GENERIC_SRV_TEMP_MEASUREMENT_CC_HANDLE: /* This handle has indications allowed */
                    case CSR_BT_GENERIC_SRV_BATTERY_CC_HANDLE: /* This handle has notifications allowed */
                        {
                             cElem = CSR_BT_GENERIC_SRV_APP_FIND_CLIENT_CONFIG_BY_ADDR(instData->clientConfigList, (void*)(&instData->connectedDeviceAddr.addr));
                            if(!cElem)
                            {
                                /*We really should never get in-here as the clientConfig should have been added elsewhere */
                                /* no previous clientConfig record in list
                                    * lets add one */
                                cElem = CSR_BT_GENERIC_SRV_APP_ADD_CLIENT_CONFIG(instData->clientConfigList);
                                cElem->deviceAddr = instData->connectedDeviceAddr.addr;
                            }
                            /*update with latest write value - this is stored per device */
                            cElem->value      = ind->writeUnit[0].value;

                            break;
                        }
                    default:
                        {
                            /* Do different things if the write is not for client configurations */
                            break;
                        }
                }

                
                /* Tells about the writer */
                sprintf((char *)clientAddress, "Write from: %04X:%02X:%06X\nHandle %d\nValue: %X",
                                         ind->address.addr.nap, 
                                         ind->address.addr.uap, 
                                         ind->address.addr.lap, 
                                         handle,
                                         *ind->writeUnit[0].value);

                CsrBtGattAppAcceptPopup(instData, "DB Write", (char *)clientAddress);

                /* tell Gatt that write is ok*/
                CsrBtGattDbWriteAccessResSend(instData->gattId, 
                                              instData->btConnId, 
                                              ind->writeUnit[0].attrHandle, 
                                              CSR_BT_GATT_ACCESS_RES_SUCCESS);

                break;

            }
        case CSR_BT_GATT_DB_ACCESS_READ_IND:
            {
                CsrBtGattDbAccessReadInd *ind = (CsrBtGattDbAccessReadInd*)instData->recvMsgP;
                CsrUint8 *clientAddress = CsrPmemZalloc(sizeof(CsrUint8)*40);
                
                CsrUint8 *val = CsrPmemAlloc(sizeof(CsrUint8));
                
                val[0] = simplePseudoRandom((CsrUint8)ind->address.addr.lap);

                sprintf((char *)clientAddress, "Read from: %04X:%02X:%06X\nHandle: %d",
                                         ind->address.addr.nap, 
                                         ind->address.addr.uap, 
                                         ind->address.addr.lap, ind->attrHandle);


                CsrBtGattAppAcceptPopup(instData, "DB Read", (void*)clientAddress);

                /* tell Gatt that write is ok*/
                CsrBtGattDbReadAccessResSend(instData->gattId, 
                                              instData->btConnId, 
                                              ind->attrHandle, 
                                              CSR_BT_GATT_ACCESS_RES_SUCCESS,
                                              1 /*value length*/,
                                              val);
                break;
            }
        case CSR_BT_GATT_WRITE_CFM:
            {
                break;
            }
        case CSR_BT_GATT_READ_CFM:
            {
                break;
            }
        default:
            {
                CsrAppBacklogReqSend(TECH_BTLE, 
                                     PROFILE_NAME(instData->profileName), 
                                     TRUE, 
                                     "####### default in gatt prim handler 0x%04x,",
                                     *primType);
                CsrBtGattFreeUpstreamMessageContents(CSR_BT_GATT_PRIM, instData->recvMsgP);
                /* unexpected primitive received */
            }
    }
}

