/****************************************************************************

  (c) Cambridge Silicon Radio Limited Gatt
  All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
 ****************************************************************************/

#include "csr_synergy.h"

#include <string.h>
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_list.h"
#include "csr_exceptionhandler.h"

#include "csr_app_lib.h"
#include "csr_ui_lower.h"
#include "csr_ui_keycode.h"
#include "csr_bt_platform.h"
#include "csr_bt_gap_app_lib.h"
#include "csr_bt_util.h"

#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"

#include "csr_bt_gatt_lib.h"
#include "csr_bt_gatt_demo_app.h"
#include "csr_bt_gatt_demo_defines.h"
#include "csr_bt_gatt_demo_hid_client_app_private_prim.h"
#define PROFILE_NAME(name) CsrUtf8StrDup((CsrUtf8String *) name)
#include "csr_bt_gatt_demo_hid_client_app_ui_sef.h"
#include "csr_bt_gatt_app_task.h"
#include "csr_bt_gatt_demo_db_utils.h"

/* Forward declarations for handler functions */
static void CsrBtHidcAppHandleHidcPrim(CsrBtHidcAppInstData *hidcData);

CsrBtUuid *CsrBtHidcAppUuid16ToUuid128(CsrUint16 uuid)
{
    CsrBtUuid *realUuid = CsrPmemAlloc(sizeof(CsrBtUuid));
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(uuid, realUuid->uuid);

    realUuid->length = CSR_BT_UUID16_SIZE;

    return realUuid;
} 

/******************************************************************************
CsrBtHidcAppStartRegister:
 *****************************************************************************/
void CsrBtHidcAppStartRegister(CsrBtHidcAppInstData *hidcData)
{
    CsrBtGattRegisterReqSend(hidcData->phandle,
                             1234);
}
/******************************************************************************
CsrBtHidcAppStartUnregister:
 *****************************************************************************/
void CsrBtHidcAppStartUnregister(CsrBtHidcAppInstData *hidcData)
{
    CsrBtGattUnregisterReqSend(hidcData->gattId);
}
/******************************************************************************
CsrBtHidcAppStartConnecting:
 *****************************************************************************/
void CsrBtHidcAppStartConnecting(CsrBtHidcAppInstData *hidcData)
{

    if(hidcData->selectedDeviceAddr.addr.uap != 0)
    {
        hidcData->whiteListConnect = FALSE;
        CsrBtGattCentralReqSend(hidcData->gattId,
                                (hidcData->selectedDeviceAddr),
                                CSR_BT_GATT_FLAGS_NONE,
                                0);
    }
    else
    {
        /* no device addr have been selected in GAP yet */
        CsrBtGattAppHandleHidcCsrUiNoSelectedDevAddr(hidcData);
    }
}
/******************************************************************************
CsrBtHidcAppStartConnectWhitelist:
 *****************************************************************************/
void CsrBtHidcAppStartConnectWhitelist(CsrBtHidcAppInstData *hidcData)
{
    hidcData->whiteListConnect = TRUE;
    CsrBtGattCentralReqSend(hidcData->gattId,
                            (hidcData->selectedDeviceAddr),
                            CSR_BT_GATT_FLAGS_WHITELIST,
                            0);
}
/******************************************************************************
CsrBtHidcAppStartAddWhitelist:
 *****************************************************************************/
void CsrBtHidcAppStartAddWhitelist(CsrBtHidcAppInstData *hidcData, CsrBtTypedAddr addr)
{
    CsrBtTypedAddr *pAddr = CsrPmemAlloc(sizeof(CsrBtTypedAddr));
    CsrMemCpy(pAddr, &addr, sizeof(CsrBtTypedAddr));
    CsrBtGattWhitelistAddReqSend(hidcData->gattId,
                                 1 /*addr count */,
                                 pAddr);
}

/******************************************************************************
CsrBtHidcAppStartReadWhitelist:
 *****************************************************************************/
void CsrBtHidcAppStartReadWhitelist(CsrBtHidcAppInstData *hidcData)
{
    CsrBtGattWhitelistReadReqSend(hidcData->gattId);
}

/******************************************************************************
CsrBtHidcAppStartReadUuid:
 *****************************************************************************/
void CsrBtHidcAppStartReadUuid(CsrBtHidcAppInstData *hidcData, CsrUint16 uuid)
{
    CsrBtGattDiscoverPrimaryServicesBy16BitUuidReqSend(hidcData->gattId, hidcData->btConnId, uuid);

}

/******************************************************************************
CsrBtHidcAppStartReadValue:
 *****************************************************************************/
void CsrBtHidcAppStartReadValue(CsrBtHidcAppInstData *hidcData, CsrUint16 pUuid, CsrUint16 cUuid)
{
    CsrUint16                         valueHandle;
    CsrBtGattAppDbPrimServiceElement  *pElem;
    CsrBtGattAppDbCharacElement       *cElem;

    /*Find Primary Service containing the Charac */
    pElem = CSR_BT_GATT_APP_DB_FIND_PRIM_SERVICE_BY_UUID(hidcData->dbPrimServices,
                                                         CsrBtHidcAppUuid16ToUuid128(pUuid));
    /*Find charac containing the handleValue */
    cElem = CSR_BT_GATT_APP_DB_FIND_CHARAC_BY_UUID(pElem->characList, 
                                                   CsrBtHidcAppUuid16ToUuid128(cUuid));
    valueHandle = cElem->valueHandle;

    CsrBtGattReadReqSend(hidcData->gattId,
                         hidcData->btConnId,
                         valueHandle,
                         0);

}

/******************************************************************************
CsrBtHidcAppStartWriteClientConfig:
 *****************************************************************************/
void CsrBtHidcAppStartWriteClientConfig(CsrBtHidcAppInstData *hidcData, CsrBtUuid *pUuid, CsrBtUuid *cUuid, CsrUint16 value)
{

    CsrBtGattAppDbPrimServiceElement  *pElem;
    CsrBtGattAppDbCharacElement       *cElem;
    CsrBtGattAppDbCharacDescrElement  *dElem;

    /*Find Primary Service containing the Charac */
    pElem = CSR_BT_GATT_APP_DB_FIND_PRIM_SERVICE_BY_UUID(hidcData->dbPrimServices, pUuid);
    if(pElem == NULL)
    {
        return; /* the prim Service was not found */
    }
    /*Find charac containing the handleValue */
    cElem = CSR_BT_GATT_APP_DB_FIND_CHARAC_BY_UUID(pElem->characList, cUuid);
    if(cElem == NULL)
    {
        return; /* the charac was not found */
    }

    dElem = CSR_BT_GATT_APP_DB_FIND_CHARAC_DESCR_BY_UUID(cElem->descrList, 
                                                         CsrBtHidcAppUuid16ToUuid128(CSR_BT_GATT_CLIENT_CHARACTERISTIC_CONFIGURATION));
    if(dElem == NULL)
    {
        return; /* the charac descriptor was not found */
    }
    CsrBtGattWriteClientConfigurationReqSend(hidcData->gattId,
                                             hidcData->btConnId,
                                             dElem->descriptorHandle,
                                             cElem->valueHandle,value);

    /* NOTE!! this is a temporary workaround to make it possible to work with devices without security implemented */
    hidcData->interruptValueHandle = cElem->valueHandle;
}



/******************************************************************************
CsrBtHidcAppStartDisconnect:
 *****************************************************************************/
void CsrBtHidcAppStartDisconnect(CsrBtHidcAppInstData *hidcData)
{
    CsrBtGattDisconnectReqSend(hidcData->gattId, hidcData->btConnId);
    /*reset BT connection ID */
    hidcData->btConnId = 0xff;
}

/******************************************************************************
CsrBtHidcAppInitInstanceData:
 *****************************************************************************/
void CsrBtHidcAppClearHandles(CsrBtHidcAppInstData *hidcData){

    /*init list for local database structure copy */                          
    CsrCmnListInit(&hidcData->dbPrimServices, 0, CsrBtGattAppInitDbPrimList, CsrBtGattAppFreeDbPrimList);
    hidcData->dbPrimServicesCount  =  0;
    hidcData->traversingDb         =  FALSE;
    hidcData->reportNotif          =  FALSE;
    hidcData->interuptNotif        =  FALSE;


}
void CsrBtHidcAppInitInstanceData(CsrBtHidcAppInstData *hidcData)
{
    CsrMemSet(hidcData,0,sizeof(CsrBtHidcAppInstData));

    hidcData->btConnId                =  0xff;
    hidcData->csrUiVar.hMainMenu      =  CSR_UI_DEFAULTACTION;
    hidcData->csrUiVar.hCommonDialog  =  CSR_UI_DEFAULTACTION;
    CsrBtHidcAppClearHandles(hidcData);
}

static void csrBtHidcAppSendHidcAppRegisterReq(CsrSchedQid pHandle)
{
    CsrBtHidcAppRegisterReq *prim;

    prim = (CsrBtHidcAppRegisterReq *) CsrPmemAlloc(sizeof(CsrBtHidcAppRegisterReq));
    prim->type = CSR_BT_HIDC_APP_REGISTER_REQ;
    prim->phandle = pHandle;

    CsrSchedMessagePut(CSR_BT_HIDC_APP_IFACEQUEUE, CSR_BT_HIDC_APP_PRIM,prim);
}

static void csrBtHidcAppSendHidcAppRegisterCfm(CsrSchedQid phandle, CsrSchedQid queueId, CsrUint8 instance)
{
    CsrBtHidcAppRegisterCfm *prim;

    prim = (CsrBtHidcAppRegisterCfm *) CsrPmemAlloc(sizeof(CsrBtHidcAppRegisterCfm));
    prim->type = CSR_BT_HIDC_APP_REGISTER_CFM;
    prim->queueId = queueId;
    prim->instance = instance;

    CsrSchedMessagePut(phandle, CSR_BT_HIDC_APP_PRIM,prim);
}

static char * csrBtHidcAppReturnHidcInstName(CsrBtHidcAppInstData *instance)
{
    return CsrStrDup("LE HID Host");
}

/******************************************************************************
 * Init function called by the scheduler upon initialisation.
 * This function is used to boot the demo application.
 *****************************************************************************/
void CsrBtHidcAppInit(void **gash)
{
    CsrBtHidcAppInstData    *hidcData;

    /* allocate and initialise instance data space */
    *gash     =  (void *) CsrPmemAlloc(sizeof(CsrBtHidcAppInstData));
    hidcData  =  (CsrBtHidcAppInstData *) *gash;
    CsrBtHidcAppInitInstanceData(hidcData);

    hidcData->phandle      =  CsrSchedTaskQueueGet() ;
    hidcData->initialized  =  TRUE;

    /* Start UI creation cycle */
    CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
    csrBtHidcAppSendHidcAppRegisterReq(hidcData->phandle);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtHidcAppDeinit(void **gash)
{
    CsrUint16             msg_type;
    void                  *msg_data;
    CsrBtHidcAppInstData  *hidcData;

    hidcData = (CsrBtHidcAppInstData *) (*gash);

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

    CsrPmemFree(hidcData);
}
#else
void CsrBtHidcAppDeinit(void **gash)
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
void CsrBtHidcAppHandler(void **gash)
{
    CsrBtHidcAppInstData  *hidcData;
    CsrUint16             eventType;

    /* get a message from the demoapplication message queue. The message is returned in prim
       and the event type in eventType */
    hidcData = (CsrBtHidcAppInstData *) (*gash);

    if(!hidcData->restoreFlag)
    { /* No messages on the local save queue                                                */
        CsrSchedMessageGet(&eventType , &hidcData->recvMsgP);
    }
    else
    {
        if(!CsrMessageQueuePop(&hidcData->saveQueue, &eventType , &hidcData->recvMsgP))
        { /* No more messages on the local save queue. Call CsrGetMessage to receive
             a message from the scheduler                                                   */
            hidcData->restoreFlag = FALSE;
            CsrSchedMessageGet(&eventType , &hidcData->recvMsgP);
        }
        else
        { /* A message has been restored from the local queed                               */
            ;
        }
    }

    switch (eventType)
    {
        case CSR_BT_GATT_PRIM:
            {
                CsrBtHidcAppHandleHidcPrim(hidcData);
                break;
            }

        case CSR_BT_HIDC_APP_PRIM:
            {
                CsrPrim *type = (CsrPrim *) hidcData->recvMsgP;

                switch(*type)
                {
                    case CSR_BT_HIDC_APP_REGISTER_REQ:
                        { /* only received by controling hidc demo app instance */
                            if(hidcData->initialized)
                            {
                                CsrBtHidcAppRegisterReq *prim  =  hidcData->recvMsgP;
                                CsrSchedQid queueId            =  CSR_SCHED_QID_INVALID;
                                hidcData->hidcAppHandle        =  prim->phandle;
                                queueId                        =  hidcData->hidcProfileHandle;

                                if(queueId != CSR_SCHED_QID_INVALID)
                                {
                                    csrBtHidcAppSendHidcAppRegisterCfm(prim->phandle, queueId, 1);
                                }
                                else
                                {
                                    CsrGeneralException("CSR_HIDC_DEMO_APP", 
                                                        0, 
                                                        *type,
                                                        "No CsrBtHidc profile Queue Handle Available");
                                }
                            }
                            else
                            {
                                CsrMessageQueuePush(&hidcData->saveQueue, CSR_BT_GATT_APP_PRIM, hidcData->recvMsgP);
                                hidcData->recvMsgP = NULL;
                            }
                            break;
                        }
                    case CSR_BT_HIDC_APP_REGISTER_CFM:
                        { /* Only received by extra hidc demo app instances */

                            hidcData->profileName = csrBtHidcAppReturnHidcInstName(hidcData);
                            hidcData->initialized = TRUE;

                            CsrBtGattAppHandleHidcSetMainMenuHeader(hidcData);
                            CsrAppRegisterReqSend(hidcData->phandle, TECH_BTLE, PROFILE_NAME(hidcData->profileName));
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
                CsrPrim *prim = (CsrPrim *) hidcData->recvMsgP;
                if (*prim == CSR_BT_GAP_APP_GET_SELECTED_DEVICE_CFM)
                {
                    CsrBtGapAppGetSelectedDeviceCfm *prim = hidcData->recvMsgP;
                    hidcData->selectedDeviceAddr.addr.lap = prim->deviceAddr.lap;
                    hidcData->selectedDeviceAddr.addr.nap = prim->deviceAddr.nap;
                    hidcData->selectedDeviceAddr.addr.uap = prim->deviceAddr.uap;
                    CsrAppBacklogReqSend(TECH_BTLE, 
                                         PROFILE_NAME(hidcData->profileName), 
                                         FALSE, 
                                         "Received selected DeviceAddr: %04X:%02X:%06X", 
                                         prim->deviceAddr.nap, 
                                         prim->deviceAddr.uap, 
                                         prim->deviceAddr.lap);
                    /*update UI*/
                    CsrUiUieShowReqSend(hidcData->csrUiVar.hMainMenu,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_BT_HIDC_APP_CSR_UI_PRI);
                }
                else
                {
                    /* unexpected primitive received */
                    CsrGeneralException("CSR_BT_HIDC_APP", 
                                        0, 
                                        *prim, 
                                        "####### Unhandled CSR_BT_GAP_APP_PRIM in CsrBtHidcAppHandler,");
                }
                break;
            }
        case CSR_APP_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) hidcData->recvMsgP;

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
                    CsrGeneralException("CSR_BT_HIDC_APP", 
                                        0, 
                                        *prim, 
                                        "####### Unhandled CSR_APP_PRIM in CsrBtHidcAppHandler,");
                }
                break;
            }
        case CSR_UI_PRIM:
            {
                CsrBtGattAppHandleHidcCsrUiPrim(hidcData);
                break;
            }

        default:
            {
                /* unexpected primitive received */
                CsrGeneralException("CSR_BT_HIDC_APP",
                                    0, 
                                    *((CsrPrim *) hidcData->recvMsgP),
                                    "####### default in CsrBtHidcAppHandler,");
            }
    }
    /* free the received signal again. If the signal for some reason must be stored in the application
     * the pointer (prim) must be set to NULL in order not to free it here
     */
    CsrPmemFree(hidcData->recvMsgP);
}


/******************************************************************************
 * Function to handle all hidc-primitives.
 ******************************************************************************/
static void CsrBtHidcAppHandleHidcPrim(CsrBtHidcAppInstData *hidcData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) hidcData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_GATT_REGISTER_CFM:
            {
                CsrBtGattRegisterCfm *cfm = (CsrBtGattRegisterCfm*)hidcData->recvMsgP;
                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    hidcData->gattId = cfm->gattId;
                    /*update UI*/
                    CsrBtGattAppHandleHidcCsrUiRegisterUnregister(hidcData, TRUE);
                }
                else
                {
                    /*update UI*/
                    CsrBtGattAppHandleHidcCsrUiRegisterUnregister(hidcData, FALSE);
                }

                break;
            }
        case CSR_BT_GATT_UNREGISTER_CFM:
            {
                /*update UI*/
                CsrBtGattAppHandleHidcCsrUiRegisterUnregister(hidcData, FALSE);

                break;
            }
        case CSR_BT_GATT_CONNECT_IND:
            {
                CsrBtGattConnectInd        *ind;

                ind = (CsrBtGattConnectInd *) hidcData->recvMsgP;
                hidcData->btConnId = ind->btConnId;

                if (ind->resultSupplier == CSR_BT_SUPPLIER_GATT &&
                    ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrAppBacklogReqSend(TECH_BTLE, 
                                         PROFILE_NAME(hidcData->profileName), 
                                         TRUE, 
                                         "Successfully Connected with: %04X:%02X:%06X", 
                                         ind->address.addr.nap, 
                                         ind->address.addr.uap, 
                                         ind->address.addr.lap);
                    hidcData->connectedDeviceAddr.addr.nap = ind->address.addr.nap;
                    hidcData->connectedDeviceAddr.addr.lap = ind->address.addr.lap;
                    hidcData->connectedDeviceAddr.addr.uap = ind->address.addr.uap;
                    if(!hidcData->whiteListConnect)
                    {

                        /* Start treversing the DB to get all relevant info */
                        hidcData->traversingDb = TRUE;
                        CsrBtGattDiscoverAllPrimaryServicesReqSend(hidcData->gattId, hidcData->btConnId);
                    } 
                }
                else
                {
                    CsrAppBacklogReqSend(TECH_BTLE, 
                                         PROFILE_NAME(hidcData->profileName), 
                                         TRUE, 
                                         "Failed to connect with: %04X:%02X:%06X, resultCode=%d, resultSupplier=%d", 
                                         ind->address.addr.nap, 
                                         ind->address.addr.uap, 
                                         ind->address.addr.lap, 
                                         ind->resultCode, 
                                         ind->resultSupplier);
                }
                /*update UI*/
                CsrBtGattAppHandleHidcCsrUiConnectDisconnect(hidcData, 
                                                             TRUE,
                                                             (CsrBool)((ind->resultCode == CSR_BT_GATT_RESULT_SUCCESS && 
                                                                        ind->resultSupplier == CSR_BT_SUPPLIER_GATT) ? TRUE : FALSE));
                break;
            }
        case CSR_BT_GATT_DISCONNECT_IND:
            {
                CsrBtGattDisconnectInd *ind = (CsrBtGattDisconnectInd *) hidcData->recvMsgP;

                CsrAppBacklogReqSend(TECH_BTLE, 
                                     PROFILE_NAME(hidcData->profileName), 
                                     TRUE, 
                                     "Disconnected from: %04X:%02X:%06X", 
                                     ind->address.addr.nap, 
                                     ind->address.addr.uap, 
                                     ind->address.addr.lap);

                if(hidcData->whiteListConnect)
                {

                    CsrBtHidcAppStartConnectWhitelist(hidcData);
                }
                else
                {
                    /*reset the btConnId */
                    hidcData->btConnId = 0xff;
                    /*Clear all handles in instance */
                    CsrBtHidcAppClearHandles(hidcData);

                    /*update UI*/
                    CsrBtGattAppHandleHidcCsrUiConnectDisconnect(hidcData, 
                                                                 FALSE,
                                                                 TRUE);

                }
                break;
            }
        case CSR_BT_GATT_CENTRAL_CFM:
            {
                CsrBtGattCentralCfm *cfm = (CsrBtGattCentralCfm*) hidcData->recvMsgP;

                if(cfm->resultCode != CSR_BT_GATT_RESULT_SUCCESS)
                {
                    CsrBtGattAppHandleHidcCsrUiConnectDisconnect(hidcData, 
                                                                 TRUE,
                                                                 (CsrBool)((cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS && 
                                                                            cfm->resultSupplier == CSR_BT_SUPPLIER_GATT) ? TRUE : FALSE));

                }
                else 
                {
                    if(hidcData->whiteListConnect)
                    {
                        /*NOTE!! The follwing is a temporary workaround to be able to work with devices that do not support security */
                        CsrBtGattSubscribeReqSend(hidcData->gattId, hidcData->connectedDeviceAddr, hidcData->interruptValueHandle);
                    }
                }

                break;
            }
        case CSR_BT_GATT_WHITELIST_ADD_CFM:
            {
                CsrBtGattWhitelistAddCfm *cfm = (CsrBtGattWhitelistAddCfm*)hidcData->recvMsgP;
                CsrBtGattAppHandleHidcCsrUiWhitelistAdd(hidcData, 
                                                        (CsrBool)((cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS && 
                                                                   cfm->resultSupplier == CSR_BT_SUPPLIER_GATT) ? TRUE : FALSE));

                break;
            }
        case CSR_BT_GATT_DISCOVER_SERVICES_IND:
            {
                CsrBtGattDiscoverServicesInd *ind = (CsrBtGattDiscoverServicesInd*) hidcData->recvMsgP;
                /*check to see if we already have this prim service*/
                if(!CSR_BT_GATT_APP_DB_FIND_PRIM_SERVICE_BY_UUID(hidcData->dbPrimServices, &ind->uuid))
                {
                    CsrBtGattAppDbPrimServiceElement *elem = CSR_BT_GATT_APP_DB_ADD_PRIM_SERVICE(hidcData->dbPrimServices);
                    elem->uuid        = ind->uuid;
                    elem->startHandle = ind->startHandle;
                    elem->endHandle   = ind->endHandle;
                    elem->id          = hidcData->dbPrimServicesCount++;
                }
                break;
            }
        case CSR_BT_GATT_DISCOVER_SERVICES_CFM:
            {
                CsrBtGattDiscoverServicesCfm *cfm = (CsrBtGattDiscoverServicesCfm*)hidcData->recvMsgP;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /* Start with first primary service element and find all characteristics */
                    hidcData->currentPrimService = CSR_BT_GATT_APP_DB_GET_FIRST_PRIM_SERVICE(hidcData->dbPrimServices);
                    CsrBtGattDiscoverAllCharacOfAServiceReqSend(hidcData->gattId,
                                                                hidcData->btConnId,
                                                                hidcData->currentPrimService->startHandle,
                                                                hidcData->currentPrimService->endHandle);

                }
                else
                {
                    /* Update UI with error */
                    /* Note: here we could show an error in the UI */
                    CsrBtGattAppHandleHidcCsrUiShowError(hidcData, "Discover Services", "Discover services failed");
                }
                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_IND:
            {
                CsrBtGattDiscoverCharacInd  *ind;
                CsrBtGattAppDbCharacElement *cElem = CSR_BT_GATT_APP_DB_ADD_CHARAC(hidcData->currentPrimService->characList);
                ind = (CsrBtGattDiscoverCharacInd*) hidcData->recvMsgP;
                cElem->valueHandle       = ind->valueHandle;
                cElem->declarationHandle = ind->declarationHandle;
                cElem->property          = ind->property;
                cElem->uuid              = ind->uuid;
                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_CFM:
            {
                CsrBtGattDiscoverCharacCfm *cfm = (CsrBtGattDiscoverCharacCfm*) hidcData->recvMsgP;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS || 
                   (cfm->resultCode == ATT_RESULT_ATTR_NOT_FOUND && cfm->resultSupplier == CSR_BT_SUPPLIER_ATT))
                {

                    /*move on to next prim service (if any and find all of its charac*/
                    hidcData->currentPrimService = hidcData->currentPrimService->next;
                    if(hidcData->currentPrimService)
                    {
                        CsrBtGattDiscoverAllCharacOfAServiceReqSend(hidcData->gattId,
                                                                    hidcData->btConnId,
                                                                    hidcData->currentPrimService->startHandle,
                                                                    hidcData->currentPrimService->endHandle);
                    }
                    else
                    {
                        /* we go back to the beginning and start over */
                        hidcData->currentPrimService = CSR_BT_GATT_APP_DB_GET_FIRST_PRIM_SERVICE(hidcData->dbPrimServices);
                        /*start from the first charac (we know that there is at least one) */
                        hidcData->currentCharac      = CSR_BT_GATT_APP_DB_GET_FIRST_CHARAC(hidcData->currentPrimService->characList); 
                        /* read the value - if we are allowed */

                        while((hidcData->currentCharac != NULL) &&
                              ((hidcData->currentCharac->property & CSR_BT_GATT_CHARAC_PROPERTIES_READ) != CSR_BT_GATT_CHARAC_PROPERTIES_READ))
                        {
                            /* we are allowed to read  so let's stop looping*/
                            hidcData->currentCharac = hidcData->currentCharac->next;
                        }
                        /*found one we can read - lets do it*/
                        CsrBtGattReadReqSend(hidcData->gattId, 
                                             hidcData->btConnId, 
                                             hidcData->currentCharac->valueHandle, 0);
                    }
                }
                else 
                {
                    char *error = CsrPmemAlloc(100);
                    sprintf(error, "Discover Charac failed. Error code 0x%04X Supplier: 0x%02X", cfm->resultCode, cfm->resultSupplier);
                    CsrBtGattAppHandleHidcCsrUiShowError(hidcData, "Discover Charac", error);
                }
                break;
            }
        case CSR_BT_GATT_READ_CFM:
            {
                CsrUint16 startHandle = 0, endHandle  = 0;
                CsrBtGattReadCfm *cfm = (CsrBtGattReadCfm*)hidcData->recvMsgP;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /*store the values */
                    hidcData->currentCharac->value       = cfm->value;
                    hidcData->currentCharac->valueLength = cfm->valueLength;
                    /*update menu with new value */
                    CsrBtGattAppHandleHidcCsrUiMenuUpdate(hidcData);
                    if(CsrBtGattAppDbFindNextCharacWithProperty(&hidcData->currentPrimService, 
                                                                &hidcData->currentCharac, 
                                                                CSR_BT_GATT_CHARAC_PROPERTIES_READ))
                    {
                        CsrBtGattReadReqSend(hidcData->gattId, 
                                             hidcData->btConnId,
                                             hidcData->currentCharac->valueHandle,0);
                    }
                    else 
                    {
                        /* Done Reading!
                         * we need to reset the "current" pointers and start working on the descriptors*/
                        hidcData->currentPrimService  = CSR_BT_GATT_APP_DB_GET_FIRST_PRIM_SERVICE(hidcData->dbPrimServices);
                        /*start from the first charac (we know that there is at least one) */
                        hidcData->currentCharac  =  CSR_BT_GATT_APP_DB_GET_FIRST_CHARAC(hidcData->currentPrimService->characList);
                        startHandle              =  hidcData->currentCharac->declarationHandle + 1;
                        if(hidcData->currentCharac->next)
                        {
                            /* there are more characs in this prim service so we use the start of the next one as end handle*/
                            endHandle = ((CsrBtGattAppDbCharacElement*)hidcData->currentCharac->next)->declarationHandle - 1; 
                        }
                        else
                        {
                            /* We are at the last charac in the current prim service so endhandle
                             * is start of next prim service (NOTE: we expect them to be sorted) */
                            endHandle = ((CsrBtGattAppDbPrimServiceElement*)hidcData->currentPrimService->next)->startHandle - 1;
                        }
                        CsrBtGattDiscoverAllCharacDescriptorsReqSend(hidcData->gattId, hidcData->btConnId, startHandle, endHandle);
                    }
                }
                else
                {
                    /*update UI*/
                    CsrBtGattAppHandleHidcCsrUiShowError(hidcData, "Read value", "Read Value failed");
                    /*CsrBtGattAppHandleHidcCsrUiReadValue(hidcData, NULL, FALSE);*/
                }
                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND:
            {
                /* add new element to currentCharac descrList */
                CsrBtGattDiscoverCharacDescriptorsInd *ind;
                CsrBtGattAppDbCharacDescrElement      *dElem = CSR_BT_GATT_APP_DB_ADD_CHARAC_DESCR(hidcData->currentCharac->descrList);
                ind = (CsrBtGattDiscoverCharacDescriptorsInd*) hidcData->recvMsgP;
                dElem->descriptorHandle                      = ind->descriptorHandle;
                dElem->uuid                                  = ind->uuid;
                break;
            }
        case CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM:
            {
                /* Move on to next Charac or if at last one, then move to next Prim Service first charac */
                CsrBtGattDiscoverCharacDescriptorsCfm *cfm  =  (CsrBtGattDiscoverCharacDescriptorsCfm*) hidcData->recvMsgP;
                CsrUint16 startHandle                       =  0;
                CsrUint16 endHandle                         =  0;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS || cfm->resultCode == CSR_BT_GATT_RESULT_INVALID_HANDLE_RANGE
                   || (cfm->resultCode == ATT_RESULT_ATTR_NOT_FOUND && cfm->resultSupplier== CSR_BT_SUPPLIER_ATT))

                {
                    if(CsrBtGattAppDbFindNextCharacWithProperty(&hidcData->currentPrimService, &hidcData->currentCharac, 0))
                    {
                        startHandle  =  hidcData->currentCharac->declarationHandle + 1;
                        if(hidcData->currentCharac->next)
                        {
                            /*still not the last charac in prim service so get endhandle from next charac*/
                            endHandle =  ((CsrBtGattAppDbCharacElement*)hidcData->currentCharac->next)->declarationHandle - 1;
                        }
                        else
                        {              
                            /*get endhandle from prim service */
                            if(hidcData->currentPrimService)
                            {
                                endHandle = hidcData->currentPrimService->endHandle; 
                            }
                            else 
                            {
                                endHandle = 0xFFFF;
                            }
                            /* move to next prim service*/
                        }
                        CsrBtGattDiscoverAllCharacDescriptorsReqSend(hidcData->gattId, 
                                                                     hidcData->btConnId, 
                                                                     startHandle, 
                                                                     endHandle);
                    }
                    else 
                    {
                        /*we are done traversing the DB*/
                        hidcData->traversingDb = FALSE;
                        CsrBtGattAppHandleHidcCsrUiShowError(hidcData, "Discover", "Done traversing the remote database.");
                        CsrBtGattAppHandleHidcCsrUiMenuUpdate(hidcData);
                    }
                }
                else 
                {
                    CsrBtGattAppHandleHidcCsrUiShowError(hidcData, "Discover Descriptors", "Discover Charac descriptors failed.");
                }
                break;
            }
        case CSR_BT_GATT_WRITE_CFM:
            {
                CsrBtGattWriteCfm *cfm = (CsrBtGattWriteCfm*)hidcData->recvMsgP;

                if(cfm->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {
                    /*update UI*/
                    CsrBtGattAppHandleHidcCsrUiWriteValue(hidcData,  TRUE);
                    /*update sub-title in menu*/
                    CsrBtGattAppHandleHidcCsrUiMenuUpdate(hidcData);
                }
                else
                {
                    /*update UI with error message*/
                    CsrBtGattAppHandleHidcCsrUiWriteValue(hidcData, FALSE);
                }
                break;
            }
        case CSR_BT_GATT_NOTIFICATION_IND:
            {
                CsrBtGattNotificationInd *ind = (CsrBtGattNotificationInd*)hidcData->recvMsgP;
                /*the value is sent as NULL on key-release */
                if(&ind->value[2] != NULL)
                {
                    switch(ind->value[2])
                    {
                        case 0x1E: /*1*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('1'));
                                break;
                            }
                        case 0x1F: /*2*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('2'));
                                break;
                            }
                        case 0x20: /*3*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('3'));
                                break;
                            }
                        case 0x21: /*4*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('4'));
                                break;
                            }
                        case 0x22: /*5*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('5'));
                                break;
                            }
                        case 0x23: /*6*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('6'));
                                break;
                            }
                        case 0x24: /*7*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('7'));
                                break;
                            }
                        case 0x25: /*8*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('8'));
                                break;
                            }
                        case 0x26: /*9*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('9'));
                                break;
                            }
                        case 0x27: /*0*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('0'));
                                break;
                            }

                        case 0x4F: /*right*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_RIGHT);
                                break;
                            }
                        case 0x50: /*LEFT*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_LEFT);
                                break;
                            }
                        case 0x51: /*DOWN*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_DOWN);
                                break;
                            }
                        case 0x52: /*UP*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_UP);
                                break;
                            }
                        case 0x28: /* ENTER*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_SK1);
                                break;
                            }
                        case 0x2A: /*BS*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_BACK);
                                break;
                            }
                        case 0x04: /*a*/ 
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('a'));
                                break;
                            }
                        case 0x05: /*b*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('b'));
                                break;
                            }
                        case 0x06: /*c*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('c'));
                                break;
                            }
                        case 0x07: /*d*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('d'));
                                break;
                            }
                        case 0x08: /*e*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('e'));
                                break;
                            }
                        case 0x09: /*f*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('f'));
                                break;
                            }
                        case 0x0a: /*g*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('g'));
                                break;
                            }
                        case 0x0b: /*h*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('h'));
                                break;
                            }
                        case 0x0c: /*i*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('i'));
                                break;
                            }
                        case 0x0d: /*j*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('j'));
                                break;
                            }
                        case 0x0e: /*k*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('k'));
                                break;
                            }
                        case 0x0f: /*l*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('l'));
                                break;
                            }
                        case 0x10: /*m*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('m'));
                                break;
                            }
                        case 0x11: /*n*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('n'));
                                break;
                            }
                        case 0x12: /*o*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('o'));
                                break;
                            }
                        case 0x13: /*p*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('p'));
                                break;
                            }
                        case 0x14: /*q*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('q'));
                                break;
                            }
                        case 0x15: /*r*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('r'));
                                break;
                            }
                        case 0x16: /*s*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('s'));
                                break;
                            }
                        case 0x17: /*t*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('t'));
                                break;
                            }
                        case 0x18: /*u*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('u'));
                                break;
                            }
                        case 0x19: /*v*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('v'));
                                break;
                            }
                        case 0x1a: /*w*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('w'));
                                break;
                            }
                        case 0x1b: /*x*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('x'));
                                break;
                            }
                        case 0x1c: /*y*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('y'));
                                break;
                            }
                        case 0x1d: /*z*/
                            {
                                CsrUiKeyEvent(CSR_UI_KEY_ASCII('z'));
                                break;
                            }



                        case 0x00:
                            {
                                /*do nothing - this is key release */
                                break;
                            }
                        default:
                            {
                                char *text = CsrPmemAlloc(40);
                                sprintf(text, "value:0x%02X (%c)", ind->value[2], ind->value[2]);
                                CsrBtGattAppHandleHidcCsrUiShowError(hidcData, "Input", text);

                            }
                    }
                }
                break;
            }
        default:
            {
                CsrAppBacklogReqSend(TECH_BTLE, 
                                     PROFILE_NAME(hidcData->profileName), 
                                     TRUE, 
                                     "####### default in gatt prim handler 0x%04x,",
                                     *primType);
                CsrBtGattFreeUpstreamMessageContents(CSR_BT_GATT_PRIM, hidcData->recvMsgP);
                /* unexpected primitive received */
            }
    }
}

