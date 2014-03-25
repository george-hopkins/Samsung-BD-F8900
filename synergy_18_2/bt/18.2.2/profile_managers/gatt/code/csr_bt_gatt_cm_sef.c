/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_private.h"

static void csrBtGattIterateWhitelistChange(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppElement *appElement = (CsrBtGattAppElement *) elem;
    CsrBtGattId gattId = *(CsrBtGattId*)value;

    /* Notify all, or avoid named gattId */
    if ((gattId != appElement->gattId)
        && (appElement->eventMask & CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_WHITELIST_CHANGE))
    {
        CsrBtGattWhitelistChangeIndSend(appElement->gattId);
    }
}

static CsrBtGattDb *csrBtGattGetMandatoryDbEntry(CsrUtf8String *localName)
{
    const CsrUint8 peripheralPreferredConnectionValue[] = {0,0,0,0,0,0,0,0};
    const CsrUint8 serviceChangeValue[] = {0,0,0,0};
    const CsrUint8 appearanceValue[] = {0,0};
    CsrBtGattDb *head       = NULL;
    CsrBtGattDb *tail       = NULL;
    CsrUint16   attrHandle  = CSR_BT_GATT_ATTR_HANDLE_START;

    /* Add the Primary Service for GAP */
    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head, 
                                                          &attrHandle, 
                                                          CSR_BT_GAP_UUID, 
                                                          TRUE,
                                                          &tail);
    
    /* Add A Characteristic Definition for Device Name */
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle,
                                                            CSR_BT_GATT_CHARAC_PROPERTIES_READ,
                                                            CSR_BT_GATT_DEVICE_NAME_UUID,
                                                            CSR_BT_GATT_ATTR_FLAGS_DYNLEN,
                                                            (CsrUint16)(CsrStrLen((CsrCharString*)localName) + 1),
                                                            (const CsrUint8 *) localName,
                                                            &tail);

    /* Add A Characteristic Definition for Appearance  */
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_LE_APPEARANCE_VALUE, appearanceValue);
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle,
                                                            CSR_BT_GATT_CHARAC_PROPERTIES_READ,
                                                            CSR_BT_GATT_APPEARANCE_UUID,
                                                            CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                            sizeof(appearanceValue),
                                                            appearanceValue,
                                                            &tail);

    /* Add A Characteristic Definition for the Peripheral Preferred Connection Parameters  characteristic */
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_LE_DEFAULT_CONN_INTERVAL_MIN, peripheralPreferredConnectionValue);
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_LE_DEFAULT_CONN_INTERVAL_MAX, &(peripheralPreferredConnectionValue[2]));
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_LE_DEFAULT_CONN_LATENCY, &(peripheralPreferredConnectionValue[4]));
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_LE_DEFAULT_CONN_SUPERVISION_TIMEOUT, &(peripheralPreferredConnectionValue[6]));

    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle,
                                                            CSR_BT_GATT_CHARAC_PROPERTIES_READ,
                                                            CSR_BT_GATT_PERIPHERAL_CONNECTION_PARAM_UUID,
                                                            CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                            sizeof(peripheralPreferredConnectionValue),
                                                            peripheralPreferredConnectionValue,
                                                            &tail);
    

    /* Add the Primary Service for GATT */
    head = CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(head, 
                                                          &attrHandle, 
                                                          CSR_BT_GATT_UUID, 
                                                          FALSE,
                                                          &tail);


    /* Add A Characteristic Definition for the Service Change characteristic */
    head = CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(head,
                                                            &attrHandle,
                                                            CSR_BT_GATT_CHARAC_PROPERTIES_INDICATE,
                                                            CSR_BT_GATT_SERVICE_CHANGED_UUID,
                                                            CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                            sizeof(serviceChangeValue),
                                                            serviceChangeValue,
                                                            &tail);
    
    return head;
}

static void csrBtGattCmLocalNameChangeIndHandler(GattMainInst *inst)
{
    CsrUint8  *name;
    CsrUint16 length;
    CsrBtCmLocalNameChangeInd *prim = (CsrBtCmLocalNameChangeInd*) inst->msg;

    CsrPmemFree(inst->localName);

    if (prim->localName)
    {
        length = (CsrUint16)(CsrStrLen((CsrCharString*)prim->localName) + 1);
        inst->localName = prim->localName;
        prim->localName = NULL;
    }
    else
    { /* An empty local name must be present at all time */
        length          = (CsrUint16)(sizeof(CsrUtf8String));
        inst->localName = (CsrUtf8String *) CsrPmemZalloc(sizeof(CsrUtf8String));
    }
    
    name = (CsrUint8 *) CsrPmemAlloc(length);
    CsrMemCpy(name, inst->localName, length);
    CsrBtGattWriteLocalReqSend(inst->privateGattId,
                               CSR_BT_GATT_ATTR_HANDLE_DEVICE_NAME,
                               length,                  
                               name);
}

static void csrBtGattCmSdsCfmHandler(GattMainInst    *inst,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier   resultSupplier,
                                     CsrUint32       serviceRecHandle,
                                     CsrBtCmPrim     type)
{
    CsrBtConnId btConnId         = CSR_BT_GATT_LOCAL_BT_CONN_ID;
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem)
    {
        CsrBtGattSdpRecordElement *sdpElem;

        CSR_BT_GATT_SDP_INST_FIND_STATE(inst->sdpRecordInst, 
                                        &sdpElem, 
                                        CSR_BT_GATT_SDP_IN_PROGESS);

        if (sdpElem)
        {
            if(resultSupplier == CSR_BT_SUPPLIER_CM &&
               resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS)
            { 
                if(type == CSR_BT_CM_SDS_REGISTER_CFM)
                { /* An SDP record is registred with success */
                    sdpElem->serviceRecordHandle = serviceRecHandle;
                    sdpElem->state = CSR_BT_GATT_SDP_REGISTERED;
                }
                else
                { /* An SDP record is Unregistred with success.
                     Remove it from the SDP list */
                    CSR_BT_GATT_SDP_INST_REMOVE(inst->sdpRecordInst, sdpElem); 
                }

                /* Check if GATT need to Register/Unregister more
                   SDP records */
                CsrBtGattSdsRestoreHandler(inst, qElem, CSR_BT_GATT_LOCAL_MAX_MTU);
            }
            else
            { /* Fail to Register or Unregister a SDP record - try again 
                 when an ACL connection is closed */
                sdpElem->state = CSR_BT_GATT_SDP_PENDING;

                CsrBtCmSetEventMaskReqSend(CSR_BT_GATT_IFACEQUEUE,
                                           (CSR_BT_GATT_DEFAULT_CM_EVENT_MASK |
                                            CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION),
                                           CSR_BT_CM_EVENT_MASK_COND_SUCCESS);

                /* This procedure is finish. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, qElem);
            }

        }
        else
        {
            CsrStateEventException("csrBtGattCmSdsCfmHandler received with no CSR_BT_GATT_SDP_IN_PROGESS", 
                                    CSR_BT_CM_PRIM, type, 0);
        }
    }
    else
    {
        CsrStateEventException("csrBtGattCmSdsCfmHandler with no qElem", 
                                CSR_BT_CM_PRIM, type, 0);
    }
}


static void csrBtGattCmSdsRegisterCfmHandler(GattMainInst *inst)
{
    CsrBtCmSdsRegisterCfm *prim  = (CsrBtCmSdsRegisterCfm*) inst->msg;

    csrBtGattCmSdsCfmHandler(inst,
                             prim->resultCode,
                             prim->resultSupplier,
                             prim->serviceRecHandle,
                             prim->type);
}

static void csrBtGattCmSdsUnregisterCfmHandler(GattMainInst *inst)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm*) inst->msg;

    csrBtGattCmSdsCfmHandler(inst,
                             prim->resultCode,
                             prim->resultSupplier,
                             prim->serviceRecHandle,
                             prim->type);
    
}

static void csrBtGattConnAdvertiseCfm(GattMainInst *inst,
                                      CsrBtGattPeripheralReq *req,
                                      CsrBtCmLeAdvertiseCfm *cfm)
{
    CsrBtGattConnElement *conn;
    fsm_event_t event;
    CsrBool loop;

    event = ((cfm->resultSupplier == CSR_BT_SUPPLIER_CM) &&
             (cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
             ? GATT_CONN_EV_PARAM_OK
             : GATT_CONN_EV_PARAM_FAIL);        

    do
    {
        loop = FALSE;
        for(conn = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
            conn != NULL;
            conn = conn->next)
        {
            /* Match all connections with our gattId and LE-connection
             * method */
            if((conn->gattId == req->gattId)
               && L2CA_CONFLAG_IS_LE(conn->l2capFlags))
            {
                CsrUint32 count;
                GattConnArgs connargs;
                count = CsrCmnListGetCount(&(inst->connInst));

                connargs.result = cfm->resultCode;
                connargs.supplier = cfm->resultSupplier;
                connargs.inst = inst;
                GATT_FSMRUN(&connargs, event, conn);

                if (count != CsrCmnListGetCount(&(inst->connInst)))
                {
                    loop = TRUE; /* retry scan from beginning of modified
                                  * list */
                }
            }

            /* If list was modified, re-scan from beginning */
            if (loop)
            {
                break;
            }
        }
    }
    while (loop);
}

static void csrBtGattCmL2caConnectAcceptCfmHandler(GattMainInst *inst,
                                                   CsrBtCmL2caConnectAcceptCfm *cfm)
{
    if (cfm->resultCode != CSR_BT_RESULT_CODE_CM_SUCCESS &&
        cfm->resultSupplier !=  CSR_BT_SUPPLIER_CM)
    { /* We need to tell the connection that is in GATT_CONN_ST_BREDR_ACCEPT */
        CsrBtGattConnElement *conn;
        GattConnArgs connargs;
        
        do
        {
            conn = CSR_BT_GATT_CONN_INST_FIND_BREDR_ACCEPT_BT_CONN_ID(inst->connInst,
                                                                      CSR_BT_CONN_ID_INVALID);
            if (conn)
            { /* The connection list was modified (invalidated), 
                 need to start the list scan from the beginning */
                connargs.result   = cfm->resultCode;
                connargs.supplier = cfm->resultSupplier;
                connargs.inst     = inst;
                GATT_FSMRUN(&connargs, GATT_CONN_EV_CONNECT_FAIL, conn);
            }
        }
        while(conn);
    }
}

static void csrBtGattCmLeAdvertiseCfmHandler(GattMainInst *inst,
                                             CsrBtCmLeAdvertiseCfm *cfm)
{
    CsrBtGattQueueElement *qelm;
    CsrBtConnId btConnId = CSR_BT_GATT_LOCAL_BT_CONN_ID;

    qelm = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if(qelm &&
       (CSR_BT_GATT_GET_QID_FROM_GATT_ID(qelm->gattId) == CSR_BT_GATT_IFACEQUEUE))
    {
        if (qelm->gattMsg)
        { /* GATT has Updated the advertise parameters after 
             it has updated the Device Name Characteristic.*/
            
            /* This procedure is finish. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, qelm);
        }
        else
        { 
            /* Setup default connection parameters - this is a part of
             * the GATT init sequence */
            CsrBtGattUpdateConnection(inst);

            /* Also initialise BR/EDR security */
            CsrBtGattInitSecurity();
        }
    }
    else if(qelm && qelm->gattMsg)
    {
        CsrBtGattId gattId;
        CsrPrim type;

        switch(*(CsrPrim*)qelm->gattMsg)
        {
            case CSR_BT_GATT_ADVERTISE_REQ:
            {
                type = CSR_BT_GATT_ADVERTISE_CFM;
                gattId = ((CsrBtGattAdvertiseReq*)qelm->gattMsg)->gattId;
                break;
            }
            case CSR_BT_GATT_PARAM_ADVERTISE_REQ:
            {
                type = CSR_BT_GATT_PARAM_ADVERTISE_CFM;
                gattId = ((CsrBtGattParamAdvertiseReq*)qelm->gattMsg)->gattId;
                break;
            }
            case CSR_BT_GATT_PERIPHERAL_REQ:
            {
                /*  Connection setup - multiplex */
                csrBtGattConnAdvertiseCfm(inst,
                                          (CsrBtGattPeripheralReq*)qelm->gattMsg,
                                          cfm);
                return;
            }
            case CSR_BT_GATT_UNREGISTER_REQ:
            { /* Ensure that the connection settings are in sync with 
                 remaining applications. */
                CsrBtGattUpdateConnection(inst);
                return;
            }
            default:
            {
                CsrStateEventException("Invalid request in advertise handler", CSR_BT_CM_PRIM, cfm->type, 0);
                return;
            }
        }

        if((cfm->resultSupplier == CSR_BT_SUPPLIER_CM) &&
           (cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS))
        {
            CsrBtGattStdCfmSend(type, gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT);
        }
        else
        {
            CsrBtGattStdCfmSend(type, gattId,
                                cfm->resultCode,
                                cfm->resultSupplier);
        }
        CsrBtGattQueueRestoreHandler(inst, qelm);
    }
}

static void csrBtGattConnScanCfm(GattMainInst *inst,
                                 CsrBtGattCentralReq *req,
                                 CsrBtCmLeScanCfm *cfm)
{
    CsrBtGattConnElement *conn;
    fsm_event_t event;
    CsrBool loop;

    event = ((cfm->resultSupplier == CSR_BT_SUPPLIER_CM) &&
             (cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
             ? GATT_CONN_EV_PARAM_OK
             : GATT_CONN_EV_PARAM_FAIL);        

    do
    {
        loop = FALSE;
        for(conn = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
            conn != NULL;
            conn = conn->next)
        {
            /* Match all connections with our gattId and LE-connection
             * method */
            if((conn->gattId == req->gattId)
               && L2CA_CONFLAG_IS_LE(conn->l2capFlags))
            {
                CsrUint32 count;
                GattConnArgs connargs;
                count = CsrCmnListGetCount(&(inst->connInst));

                connargs.result = cfm->resultCode;
                connargs.supplier = cfm->resultSupplier;
                connargs.inst = inst;
                GATT_FSMRUN(&connargs, event, conn);
                
                /* If list was modified, re-scan from beginning */
                if (count != CsrCmnListGetCount(&(inst->connInst)))
                {
                    loop = TRUE; /* retry scan from beginning of modified
                                  * list */
                }
            }
            
            if (loop)
            {
                break;
            }
        }
    }
    while (loop);
}

static void csrBtGattCmLeScanCfmHandler(GattMainInst *inst,
                                        CsrBtCmLeScanCfm *cfm)
{
    CsrBtGattQueueElement *qelm;
    CsrBtConnId btConnId = CSR_BT_GATT_LOCAL_BT_CONN_ID;

    qelm = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if(qelm &&
       (CSR_BT_GATT_GET_QID_FROM_GATT_ID(qelm->gattId) == CSR_BT_GATT_IFACEQUEUE))
    {
        /* Setup default advertising parameters - this is a part of
         * the GATT init sequence */
        CsrBtGattUpdateAdvertise(inst);
    }
    else if(qelm && qelm->gattMsg)
    {
        CsrBtGattId gattId;
        CsrPrim type;

        switch(*(CsrPrim*)qelm->gattMsg)
        {
            case CSR_BT_GATT_SCAN_REQ:
            {
                type = CSR_BT_GATT_SCAN_CFM;
                gattId = ((CsrBtGattScanReq*)qelm->gattMsg)->gattId;
                break;
            }    
            case CSR_BT_GATT_PARAM_SCAN_REQ:
            {
                type = CSR_BT_GATT_PARAM_SCAN_CFM;
                gattId = ((CsrBtGattParamScanReq*)qelm->gattMsg)->gattId;
                break;
            }
            case CSR_BT_GATT_CENTRAL_REQ:
            {    /*  Connection setup - multiplex */
                csrBtGattConnScanCfm(inst,
                                     (CsrBtGattCentralReq*)qelm->gattMsg,
                                     cfm);
                return;
            }
            case CSR_BT_GATT_UNREGISTER_REQ:
            { /* Ensure that the advertise settings are in sync with 
                 remaining applications. */
                CsrBtGattUpdateAdvertise(inst);
                return;
            }
            default:
            {
                CsrStateEventException("Invalid request in scan handler", CSR_BT_CM_PRIM, cfm->type, 0);
                return;
            }
        }

        if((cfm->resultSupplier == CSR_BT_SUPPLIER_CM) &&
           (cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS))
        {
            CsrBtGattStdCfmSend(type, gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT);
        }
        else
        {
            CsrBtGattStdCfmSend(type, gattId,
                                cfm->resultCode,
                                cfm->resultSupplier);
        }
        CsrBtGattQueueRestoreHandler(inst, qelm);
    }
}

static void csrBtGattUnregisterCfmHandler(GattMainInst          *inst,
                                          CsrBtGattQueueElement *qelm)
{
    CsrBtGattId gattId = ((CsrBtGattUnregisterReq *) qelm->gattMsg)->gattId;
                
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &gattId);
    
    CsrBtGattStdCfmSend(CSR_BT_GATT_UNREGISTER_CFM,
                        gattId,
                        CSR_BT_GATT_RESULT_SUCCESS,
                        CSR_BT_SUPPLIER_GATT);
    
    if (appElement)
    { /* Remove gattId from list */     
        CSR_BT_GATT_APP_INST_REMOVE(inst->appInst, appElement);
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_UNREGISTER_REQ no appElement",
                                CSR_BT_GATT_PRIM, 0, 0);
    }
}

static void csrBtGattCmLeWhitelistSetCfmHandler(GattMainInst *inst,
                                                CsrBtCmLeWhitelistSetCfm *cfm)
{
    CsrBtConnId btConnId   = CSR_BT_GATT_LOCAL_BT_CONN_ID;
    CsrBtGattQueueElement *qelm = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if(qelm && qelm->gattMsg)
    {
        /* Assume error */
        CsrBtResultCode result = cfm->resultCode;
        CsrBtSupplier supplier = cfm->resultSupplier;

        switch (*(CsrPrim*)qelm->gattMsg)
        {
            case CSR_BT_GATT_WHITELIST_ADD_REQ:
            {
                CsrBtGattId gattId;
                gattId = ((CsrBtGattWhitelistAddReq*)qelm->gattMsg)->gattId;

                if(cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && 
                   cfm->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    inst->whitelistIndex++; /* Send next if present */
                    if(CsrBtGattWhitelistUpdate(inst))
                    {
                        /* Operation successful */
                        result = CSR_BT_GATT_RESULT_SUCCESS;
                        supplier = CSR_BT_SUPPLIER_GATT;
                    }
                    else
                    {
                        /* Nop. Add command sent. */
                        return;
                    }
                }
                else
                {
                    /* Modification failed. Stop here and crop the
                     * list accordingly */
                    CsrBtTypedAddr *new;

                    inst->whitelistCount = inst->whitelistIndex;
                    new = CsrPmemAlloc(inst->whitelistCount * sizeof(CsrBtTypedAddr));
                    CsrMemCpy(new, inst->whitelist, inst->whitelistCount * sizeof(CsrBtTypedAddr));
                    CsrPmemFree(inst->whitelist);
                    inst->whitelist = new;
                }
                CsrBtGattStdCfmSend(CSR_BT_GATT_WHITELIST_ADD_CFM, 
                                    gattId, 
                                    result, 
                                    supplier);

                /* Notify whitelist change subscribers */
                CSR_BT_GATT_APP_INST_ITERATE(inst->appInst,
                                             csrBtGattIterateWhitelistChange,
                                             &gattId);
                break;
            }
            case CSR_BT_GATT_WHITELIST_CLEAR_REQ:
            {
                CsrBtGattId gattId;
                gattId = ((CsrBtGattWhitelistClearReq*)qelm->gattMsg)->gattId;

                if(cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && 
                   cfm->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    /* Operation successful */
                    result = CSR_BT_GATT_RESULT_SUCCESS;
                    supplier = CSR_BT_SUPPLIER_GATT;
                
                    inst->whitelistCount = 0;
                    inst->whitelistIndex = 0;
                    CsrPmemFree(inst->whitelist);
                    inst->whitelist = NULL;
                }
                else
                {
                    /* Modification failed. Can not do anything about
                     * this. */
                }
                CsrBtGattStdCfmSend(CSR_BT_GATT_WHITELIST_CLEAR_CFM, 
                                    gattId, 
                                    result, 
                                    supplier);

                /* Notify whitelist change subscribers */
                CSR_BT_GATT_APP_INST_ITERATE(inst->appInst,
                                             csrBtGattIterateWhitelistChange,
                                             &gattId);
                break;
            }
            case CSR_BT_GATT_UNREGISTER_REQ:
            {
                /* The Unregister procedure has finished. Send
                 * CSR_BT_GATT_UNREGISTER_CFM to the application */
                csrBtGattUnregisterCfmHandler(inst, qelm);
                break;
            }
            default:
            {
                CsrStateEventException("Invalid request in Whitelist handler",
                                       CSR_BT_CM_PRIM, cfm->type, 0);
                return;
            }
        }        

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, qelm);
    }
}

static void csrBtGattCmLeConnparamCfmHandler(GattMainInst *inst,
                                             CsrBtCmLeConnparamCfm *cfm)
{
    CsrBtGattQueueElement *qelm;
    
    CsrBtConnId btConnId;

    btConnId = CSR_BT_GATT_LOCAL_BT_CONN_ID;
    qelm = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if(qelm &&
       (CSR_BT_GATT_GET_QID_FROM_GATT_ID(qelm->gattId) == CSR_BT_GATT_IFACEQUEUE))
    {
        /* Start-up sequence. Add mandatory database */
        CsrBtGattDb *dbEntry;
        CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &inst->privateGattId);
        dbEntry = csrBtGattGetMandatoryDbEntry(inst->localName);
        if (appElement)
        {
            attlib_add_req(CSR_BT_GATT_IFACEQUEUE, dbEntry, NULL);
        }
        else
        {
            CsrStateEventException("GATT cannot add private AppInst", 0, 0, 0);
        }
    }
    else if (qelm && qelm->gattMsg)
    {
        switch(*(CsrPrim*)qelm->gattMsg)
        {
            case CSR_BT_GATT_PARAM_CONNECTION_REQ:
            {
                CsrBtGattParamConnectionReq *req = (CsrBtGattParamConnectionReq*) qelm->gattMsg;

                if((cfm->resultSupplier == CSR_BT_SUPPLIER_CM) &&
                   (cfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS))
                {
                   CsrBtGattStdCfmSend(CSR_BT_GATT_PARAM_CONNECTION_CFM,
                                       req->gattId,
                                       CSR_BT_GATT_RESULT_SUCCESS,
                                       CSR_BT_SUPPLIER_GATT);     
                }
                else
                {
                    CsrBtGattStdCfmSend(CSR_BT_GATT_PARAM_CONNECTION_CFM,
                                       req->gattId,
                                       cfm->resultCode,
                                       cfm->resultSupplier);
                }
                CsrBtGattQueueRestoreHandler(inst, qelm);
                break;
            }
            case CSR_BT_GATT_UNREGISTER_REQ:
            { /* Ensure that the advertise settings are in sync with 
                 remaining applications. */
                if (CSR_BT_GATT_APP_INST_COUNT(inst->appInst) == CSR_BT_GATT_APP_INST_LAST_EXTERN_APPLICATION)
                { /* Clear white List */
                    CsrBtTypedAddr zeroaddr;
                    CsrBtAddrZero(&zeroaddr);
                    CsrBtCmLeWhitelistSetReqSend(CSR_BT_GATT_IFACEQUEUE,
                                                 CSR_BT_CM_LE_WHITELIST_CLEAR,
                                                 zeroaddr);   /* Clear white List */
                }
                else
                {
                    csrBtGattUnregisterCfmHandler(inst, qelm);
                    CsrBtGattQueueRestoreHandler(inst, qelm);
                }
                break;
            }
            default:
            {
                CsrStateEventException("Invalid request in Conn Param handler", CSR_BT_CM_PRIM, cfm->type, 0);
                break;
            }
        }
    }
}

static void csrBtGattCmLeConnParamUpdateCmpIndHandler(GattMainInst *inst,
                                                      CsrBtCmLeConnparamUpdateCmpInd *ind)
{
    CsrBtGattQueueElement *qelm;
    CsrBtGattParamConnUpdateReq *req;
    CsrBtConnId btConnId;
    CsrBtResultCode result;
    CsrBtSupplier supplier;

    btConnId = CsrBtGattConnIdByAddr(inst, &ind->address);
    qelm = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qelm && qelm->gattMsg)
    {
        req = (CsrBtGattParamConnUpdateReq*)qelm->gattMsg;

        if((ind->resultSupplier == CSR_BT_SUPPLIER_CM) &&
           (ind->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS))
        {
            result = CSR_BT_GATT_RESULT_SUCCESS;
            supplier = CSR_BT_SUPPLIER_GATT;
        }
        else
        {
            result = ind->resultCode;
            supplier = ind->resultSupplier;
        }

        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_PARAM_CONN_UPDATE_CFM,
                                    req->gattId,
                                    result,
                                    supplier,
                                    btConnId);
        CsrBtGattQueueRestoreHandler(inst, qelm);
    }
}

static void csrBtGattCmLeReportIndHandler(GattMainInst *inst,
                                          CsrBtCmLeReportInd *ind)
{
    CsrBtGattAppElement *app;
    CsrUint8 pos;

    /* Cull zero-padded reports */
    for(pos=0;
        (pos+2)<ind->lengthData;
        pos += ind->data[pos]+1) /* +1 because 'length' itself is not included */
    {
        /* Tag length is zero. If rest is all zeroes, pretend that
         * this was the end of the data */
        if(ind->data[pos] == 0)
        {
            CsrUint8 i;
            CsrBool zero = TRUE;
            for(i=pos; i<ind->lengthData; i++)
            {
                if(ind->data[i] != 0)
                {
                    zero = FALSE;
                }
            }
            if(zero)
            {
                ind->lengthData = pos;
                break;
            }
        }
    }

    /* Loop all applications, and send report to all who are scanning */
    for(app = CSR_BT_GATT_APP_INST_GET_FIRST(inst->appInst);
        app != NULL;
        app = app->next)
    {
        /* FIX ME add support for filters*/
        if((app->mode & CSR_BT_GATT_MODE_SCAN)
           && CsrBtGattFilterAddr(app, &(ind->address))
           && CsrBtGattFilterData(app, ind->lengthData, ind->data))
        {
            CsrBtGattReportIndSend(app->gattId,
                                   (CsrBtGattReportEvent)(ind->eventType),
                                   &ind->address,
                                   &ind->permanentAddress,
                                   ind->lengthData,
                                   ind->data,
                                   ind->rssi);
        }
    }
}

static void csrBtGattCmLeEventConnectionIndHandler(GattMainInst *inst,
                                                   CsrBtCmLeEventConnectionInd *ind)
{
    CSR_BT_GATT_CONN_ITERATE_INST_UPDATE_PARAM(inst,
                                               inst->connInst,
                                               ind->deviceAddr,
                                               ind->interval,
                                               ind->timeout,
                                               ind->latency);
}

static void csrBtGattCmLeAcceptConnparamUpdateIndHandler(GattMainInst *inst,
                                                         CsrBtCmLeAcceptConnparamUpdateInd *ind)
{ /* The Slave has requested to Updated the connection parameters */
    CsrBtGattConnElement *conn = CSR_BT_GATT_CONN_INST_FIND_CONN_ID_FROM_ADDRESS(inst->connInst, 
                                                                                 &(ind->address)); 
    if (conn)
    { 
        CsrBtGattParamConnUpdateReq * newMsg = CsrBtGattParamConnUpdateReq_struct(inst->privateGattId,
                                                                                  conn->btConnId,
                                                                                  ind->connIntervalMin,
                                                                                  ind->connIntervalMax,
                                                                                  ind->connLatency,
                                                                                  ind->supervisionTimeout,
                                                                                  0,
                                                                                  0,
                                                                                  TRUE,
                                                                                  ind->l2caSignalId);
        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, inst->msg);
        CsrPmemFree(inst->msg);
        inst->msg = newMsg;
        CsrBtGattParamConnUpdateReqHandler(inst);
    }
    else
    {
        CsrBtCmLeAcceptConnparamUpdateResSend(ind->address,
                                              ind->l2caSignalId,
                                              ind->connIntervalMin,
                                              ind->connIntervalMax,
                                              ind->connLatency,
                                              ind->supervisionTimeout,
                                              FALSE);
    }
}

void CsrBtGattDispatchCm(GattMainInst *inst)
{
    CsrPrim type = *(CsrPrim *)inst->msg;
    switch(type)
    {
        case CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM:
        { /* This message return error if CM cannot enten page scan. 
             Need to handle this */
             csrBtGattCmL2caConnectAcceptCfmHandler(inst, (CsrBtCmL2caConnectAcceptCfm*) inst->msg);   
            break;
        }
        case CSR_BT_CM_SET_EVENT_MASK_CFM:
        {
            /* Just ignore */
            break;
        }
        case CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM:
        {
            /* Just ignore */
            break;
        }
        case CSR_BT_CM_BLUECORE_INITIALIZED_IND:
        {
            /* Setup default scan parameters. This will in turn
             * trigger advertise and connection parameters. Part of
             * the startup sequence */
            CsrBtGattUpdateScan(inst);
            break;
        }
        case CSR_BT_CM_LOCAL_NAME_CHANGE_IND:
        {
            csrBtGattCmLocalNameChangeIndHandler(inst);
            break;
        }    
        case CSR_BT_CM_SDS_REGISTER_CFM:
        {
            csrBtGattCmSdsRegisterCfmHandler(inst);
            break;
        }
        case CSR_BT_CM_SDS_UNREGISTER_CFM:
        {
            csrBtGattCmSdsUnregisterCfmHandler(inst);
            break;
        }
        case CSR_BT_CM_LE_ADVERTISE_CFM:
        {
            csrBtGattCmLeAdvertiseCfmHandler(inst, (CsrBtCmLeAdvertiseCfm*)inst->msg);
            break;
        }
        case CSR_BT_CM_LE_SCAN_CFM:
        {
            csrBtGattCmLeScanCfmHandler(inst, (CsrBtCmLeScanCfm*)inst->msg);
            break;
        }
        case CSR_BT_CM_LE_REPORT_IND:
        {
            csrBtGattCmLeReportIndHandler(inst, (CsrBtCmLeReportInd*)inst->msg);
            break;
        }
        case CSR_BT_CM_LE_WHITELIST_SET_CFM:
        {
            csrBtGattCmLeWhitelistSetCfmHandler(inst, (CsrBtCmLeWhitelistSetCfm*)inst->msg);
            break;
        }
        case CSR_BT_CM_LE_CONNPARAM_CFM:
        {
            csrBtGattCmLeConnparamCfmHandler(inst, (CsrBtCmLeConnparamCfm*)inst->msg);
            break;
        }
        case CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND:
        {
            csrBtGattCmLeConnParamUpdateCmpIndHandler(inst, (CsrBtCmLeConnparamUpdateCmpInd*)inst->msg);
            break;
        }
        case CSR_BT_CM_ACL_CONNECT_IND:
        { /* Ignore */
            break;
        }
        case CSR_BT_CM_LE_EVENT_CONNECTION_IND:
        {
            csrBtGattCmLeEventConnectionIndHandler(inst, (CsrBtCmLeEventConnectionInd*) inst->msg);
            break;
        }
        case CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND:
        {
            csrBtGattCmLeAcceptConnparamUpdateIndHandler(inst, (CsrBtCmLeAcceptConnparamUpdateInd*) inst->msg);
            break;
        }
        case CSR_BT_CM_LE_EVENT_ADVERTISING_IND:
        {
            break;
        }
        case CSR_BT_CM_LE_EVENT_SCAN_IND:
        {
            break;
        }
        case CSR_BT_CM_ACL_DISCONNECT_IND:
        { /* An ACL connection is closed, check if GATT needs
             to Register or Unregister some service records.
             Note GATT only subscribe for the ACL events as
             long at it has some SDP records to Register or 
             Unregister */
            CsrBtGattSdsRequestHandler(inst, CSR_BT_CM_PRIM);
            break;
        }
        default:
        {
            CsrStateEventException("GATT handler - unknown CM primitive", CSR_BT_CM_PRIM, type, 0);
            break;
        }
    }
}
