/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_private.h"

/* Service UUID constants */
static const CsrUint32 csrBtGattPrimaryService[4]   = {0x00002800,0x00001000,0x80000080,0x5F9B34FB};
static const CsrUint32 csrBtGattSecondaryService[4] = {0x00002801,0x00001000,0x80000080,0x5F9B34FB};

/* Common connection creation failure, used by failing
 * central/peripheral/connect/accept restore handlers */
static void csrBtGattConnRestoreFail(GattMainInst           *inst,
                                     CsrBtGattConnElement   *conn,
                                     CsrBtGattQueueElement  *qelm,
                                     CsrBtGattId            gattId,
                                     CsrBtGattPrim          cfmType,
                                     CsrBtResultCode        result)
{
    CsrBtGattQueueRestoreHandler(inst, qelm);
    CsrBtGattStdBtConnIdCfmSend(cfmType,
                                gattId,
                                result,
                                CSR_BT_SUPPLIER_GATT,
                                CSR_BT_CONN_ID_INVALID);
    if (conn)
    {
        CSR_BT_GATT_CONN_INST_REMOVE(inst->connInst, conn);
    }
}

static CsrBool csrBtGattNewReqHandler(GattMainInst          *inst,
                                      CsrBtConnId           btConnId,
                                      CsrBtGattId           gattId,
                                      CsrBtGattRestoreType  restoreFunc,
                                      CsrBtGattCancelType   cancelFunc,
                                      CsrBtGattSecurityType securityFunc)
{
    
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &gattId);

    if (appElement)
    {
        CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_ADD_LAST(inst->queue);
        CsrBtGattQueueElement *tmp   = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, 
                                                                         &btConnId);

        qElem->gattId       = gattId;
        qElem->btConnId     = btConnId;
        qElem->gattMsg      = inst->msg;
        qElem->restoreFunc  = restoreFunc;
        qElem->cancelFunc   = cancelFunc;
        qElem->securityFunc = securityFunc;
        inst->msg           = NULL;

        if (tmp)
        {
            /* Saved Msg */
            qElem->msgState = CSR_BT_GATT_MSG_QUEUE_QUEUED;
        }
        else
        {
            /* Call callback function direct */
            qElem->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
            qElem->restoreFunc(inst, 
                               qElem, 
                               CsrBtGattValidateBtConnIdByMtu(inst, gattId, btConnId));
        }
        return TRUE;
    }
    return FALSE;
}

static void csrBtGattStdCancelElementHandler(GattMainInst          *inst, 
                                             CsrBtGattQueueElement *element)
{
    if (element->msgState == CSR_BT_GATT_MSG_QUEUE_QUEUED)
    { /* This procedure is cancel before it even has started 
         to interact with ATT. E.g the Request that is being 
         cancel is still placed on the local GATT queue */  
        CSR_BT_GATT_QUEUE_REMOVE(inst->queue, element);
    }
    else
    { /* This procedure is cancel while GATT is interacting
         with ATT or SC. Set cancelFunc to NULL in order to prevent 
         that this procedure is cancel twice */ 
        element->msgState   = CSR_BT_GATT_MSG_QUEUE_CANCELLED;
        element->cancelFunc = NULL;
    }
}

static void csrBtGattStdCancelHandler(void            *gattInst, 
                                      void            *qElem, 
                                      CsrBtGattPrim   confirmType,
                                      CsrBtResultCode result, 
                                      CsrBtSupplier   supplier)
{
    GattMainInst          *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *)qElem;

    if (result == CSR_BT_GATT_RESULT_CANCELLED)
    {
        CsrBtGattStdBtConnIdCfmSend(confirmType,
                                    element->gattId,
                                    result,
                                    supplier,
                                    element->btConnId);
        /* Set element->restoreFunc to NULL in order to make sure that 
           the application do not received the confirm message twice in case 
           of an ATT_DISCONNECT_IND */
        element->restoreFunc = NULL;
    }
    /* Else - GATT has Cancel the Procedure as part of the 
              unregister procedure */

    /* Check what to do with the pending message */
    csrBtGattStdCancelElementHandler(inst, element); 
}

static CsrBool csrBtGattValidateAttrHandles(CsrBtGattDb *db,
                                            CsrBtGattAppElement *element)
{
    CsrBtGattDb *dbTmp;

    for(dbTmp = db; dbTmp != NULL; dbTmp = dbTmp->next)
    {
        /* Ensure handles belong to the application */
        if(!(dbTmp->handle >= element->start &&
             dbTmp->handle <= element->end))
        {
            return FALSE;
        }

        /* Authentication and encryption are mutually exclusive */
        if( ((dbTmp->flags & CSR_BT_GATT_ATTR_FLAGS_WRITE_ENCRYPTION)
             && (dbTmp->flags & CSR_BT_GATT_ATTR_FLAGS_WRITE_AUTHENTICATION)) ||
            ((dbTmp->flags & CSR_BT_GATT_ATTR_FLAGS_READ_ENCRYPTION)
             && (dbTmp->flags & CSR_BT_GATT_ATTR_FLAGS_READ_AUTHENTICATION)))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/* Covers Registration and Un-register an application instance to Gatt */
static void csrBtGattRegisterRestoreHandler(void      *gattInst, 
                                            void      *qElem, 
                                            CsrUint16 mtu)
{
    GattMainInst          *inst    = (GattMainInst *) gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    CsrBtGattRegisterReq  *prim    = (CsrBtGattRegisterReq *) element->gattMsg;
    CsrBtConnId btConnId           = CSR_BT_GATT_LOCAL_BT_CONN_ID;

    CSR_UNUSED(mtu);

    CsrBtGattRegisterCfmSend(prim->qid,
                             element->gattId,
                             CSR_BT_GATT_RESULT_SUCCESS,
                             CSR_BT_SUPPLIER_GATT,
                             prim->context);

    /* This procedure has finished. Start the next if any */
    CsrBtGattQueueRestoreHandler(inst, 
                                 CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId));
}

CsrBool CsrBtGattRegisterReqHandler(GattMainInst *inst)
{
    CsrUint16            count       = 1;
    CsrBtGattId          gattId      = CSR_BT_GATT_INVALID_GATT_ID;
    CsrBtGattRegisterReq *prim       = (CsrBtGattRegisterReq *) inst->msg;
    CsrBtGattAppElement  *appElement = NULL;
    
    do
    {
        gattId = CSR_BT_GATT_CREATE_GATT_ID(count, prim->qid);
        
        if (CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &gattId))
        {
            count++;
        }
        else
        {
            appElement = CSR_BT_GATT_APP_INST_ADD_FIRST(inst->appInst);
            count      = 0xFFFF;
        }
    }
    while(count < 0xFFFF);

    if (appElement)
    { /* Must ensure that a success registration is put on queue, 
         so the register procedure cannot be run while unregister 
         is running */
        appElement->gattId = gattId;
        return (csrBtGattNewReqHandler(inst, 
                                       CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                       gattId, 
                                       csrBtGattRegisterRestoreHandler,
                                       NULL,
                                       NULL));
    }
    else
    { /* Internal Error unable to create a new element to the appInst.
         Reply to app with an error */
        CsrBtGattRegisterCfmSend(prim->qid,
                                 CSR_BT_GATT_INVALID_GATT_ID,
                                 CSR_BT_GATT_RESULT_INTERNAL_ERROR,
                                 CSR_BT_SUPPLIER_GATT,
                                 prim->context);
        return TRUE;
    }
}

static CsrBtConnId csrBtGattIterateReleaseAllConn(GattMainInst *inst,
                                                  CsrBtGattId gattId)
{
    CsrBtGattConnElement *conn;
    CsrBtGattConnElement *next;
    CsrBtConnId lastConnId;

    /* Traverse the list in a deletion-safe way */
    lastConnId = CSR_BT_GATT_LOCAL_BT_CONN_ID;
    conn = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
    while(conn)
    {
        next = conn->next;
        if(conn->gattId == gattId)
        {
            CsrBtGattQueueElement *qElem = NULL;
            
            /* Check if the application is in the middle of a procedure
             * that can be cancelled */
            CSR_BT_GATT_QUEUE_FIND_MSG_TO_CANCEL(inst->queue, &qElem,
                                                 conn->gattId, conn->btConnId);
            
            if (qElem)
            {
                /* Make sure that all running processes are cancelled.
                 * Note the result code shall not be set to
                 * CSR_BT_GATT_RESULT_CANCELLED.  If set to
                 * CSR_BT_GATT_RESULT_CANCELLED the application WILL
                 * receive a confirm message for the running procedure */
                qElem->cancelFunc(inst, qElem, CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID, CSR_BT_SUPPLIER_GATT);
            }
            
            /* Release the connection. Note at this point it is guaranteed
             * that inst->msg is NULL. E.g. csrBtGattNewReqHandler is
             * always called and this function sets inst->msg to NULL */
            lastConnId = conn->btConnId;
            inst->msg = CsrBtGattDisconnectReq_struct(conn->gattId, 
                                                      conn->btConnId);
            CsrBtGattDisconnectReqHandler(inst);
        }
        conn = next;
    }

    return lastConnId;
}

static CsrBool csrBtGattRemoveQueuedMessages(CsrCmnListElm_t *elem, void *value)
{
    /* Remove all queue message on the given  gattId */
    CsrBtGattQueueElement *qElem = (CsrBtGattQueueElement *) elem;
    CsrBtGattId           gattId = *(CsrBtGattId *) value;

    if (qElem->gattId   == gattId &&
        qElem->msgState == CSR_BT_GATT_MSG_QUEUE_QUEUED)
    {
        /* Remove the queued messages */
        return TRUE;
    }
    return FALSE;
}

static void csrBtGattStartLocalUnregisterHandler(GattMainInst        *inst,
                                                 CsrBtGattAppElement *appElement)
{
    if (appElement->start > 0)
    { /* GATT makes sure all handles belonging to the
         application is remove from the data base */
         attlib_remove_req(CSR_BT_GATT_IFACEQUEUE, 
                           appElement->start, 
                           appElement->end, 
                           NULL);
    }
    else
    { /* Ensure that the scan settings are in sync with 
         remaining applications. */
        CsrBtGattUpdateScan(inst);
    }
}

static void csrBtGattPrivateUnregisterRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst            *inst       = (GattMainInst*)gattInst;
    CsrBtGattQueueElement   *element    = (CsrBtGattQueueElement *)qElem;
    CsrBtGattPrivateUnregisterReq *prim = (CsrBtGattPrivateUnregisterReq *)element->gattMsg;
    CsrBtGattAppElement *appElement     = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                            &prim->gattId);

    if (appElement)
    {
        /* Start local unregister procedure */
        csrBtGattStartLocalUnregisterHandler(inst, appElement);
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_PRIVATE_UNREGISTER_REQ no appElement",
                                CSR_BT_GATT_PRIM, prim->type, 0);   
    }
    /* This procedure has finished. Start the next if any */
    CsrBtGattQueueRestoreHandler(inst, element);
}


static void csrBtGattUnregisterRestoreHandler(void      *gattInst, 
                                              void      *qElem, 
                                              CsrUint16 mtu)
{
    
    GattMainInst *inst = (GattMainInst *) gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement*)qElem;
    CsrBtGattUnregisterReq *prim = (CsrBtGattUnregisterReq*)element->gattMsg;
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &prim->gattId);
    CsrBtConnId lastConnId;

    CSR_UNUSED(mtu);
    if (appElement)
    {
        void                                        *oldMsg;
        oldMsg = inst->msg; /* keep a copy of the msg-pointer to avoid a leak. */
        /* Remove all queued messages that belongs to the 
           application being unregistered */
        CSR_BT_GATT_QUEUE_ITERATE_REMOVE(inst->queue, 
                                         csrBtGattRemoveQueuedMessages, 
                                         &prim->gattId);


        /* Release all the connection which belong to the application
           being unregistered */
        lastConnId = csrBtGattIterateReleaseAllConn(inst, prim->gattId);
    
        /* Do not setup new scan/advertise/connection params */
        CsrPmemFree(appElement->scan);
        CsrPmemFree(appElement->adv);
        CsrPmemFree(appElement->conn);
        appElement->scan = NULL;
        appElement->adv  = NULL;
        appElement->conn = NULL;
        
        /* If the last btConnId to be cancelled was 'local' then we
         * are not waiting for e.g. ATT to finish - and we can start
         * the local unregistration immediately */
        if (lastConnId == CSR_BT_GATT_LOCAL_BT_CONN_ID)
        {
            /* Start local unregister procedure */ 
            csrBtGattStartLocalUnregisterHandler(inst, appElement);
        }
        else
        {
            /* Make sure that the last unregister part is not done
             * before the last disconnect request message has been
             * issued. Note at this point it is guaranteed that
             * inst->msg is NULL. E.g. csrBtGattNewReqHandler is
             * always called and this function sets inst->msg to
             * NULL */
            CsrBtGattPrivateUnregisterReq *newMsg = (CsrBtGattPrivateUnregisterReq *)
                                                     CsrPmemAlloc(sizeof(CsrBtGattPrivateUnregisterReq));

            newMsg->type     = CSR_BT_GATT_PRIVATE_UNREGISTER_REQ;
            newMsg->btConnId = lastConnId;
            newMsg->gattId   = prim->gattId;
            inst->msg        = newMsg;
            (void)(csrBtGattNewReqHandler(inst, 
                                          lastConnId, 
                                          prim->gattId, 
                                          csrBtGattPrivateUnregisterRestoreHandler,
                                          NULL,
                                          NULL));
            inst->msg = oldMsg; /* Remove when D-25132 is solved. */
        }
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_UNREGISTER_REQ no appElement",
                                CSR_BT_GATT_PRIM, prim->type, 0);
    }
}

CsrBool CsrBtGattUnregisterReqHandler(GattMainInst *inst)
{
    CsrBtGattUnregisterReq *prim    = (CsrBtGattUnregisterReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId, 
                                   csrBtGattUnregisterRestoreHandler,
                                   NULL,
                                   NULL));
}

/* Covers LE connect procedure */
static void csrBtGattSaveAcceptableConnParams(GattMainInst *inst, CsrBtGattConnElement *conn)
{ /* Copies the application default acceptable values to conn Inst */ 
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &conn->gattId);
    
    if (appElement && appElement->conn)
    {
        CsrBtGattAppConnParams *connParams = appElement->conn;
        conn->connIntervalMin = connParams->connIntervalMin;
        conn->connIntervalMax = connParams->connIntervalMax;
        conn->connLatencyMax = connParams->connLatencyMax;
        conn->supervisionTimeoutMin = connParams->supervisionTimeoutMin;
        conn->supervisionTimeoutMax = connParams->supervisionTimeoutMax;
    }
}

static void csrBtGattCentralRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst           = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *qelm  = (CsrBtGattQueueElement*) qElem;
    CsrBtGattCentralReq   *prim  = (CsrBtGattCentralReq*) qelm->gattMsg;
    CsrBtGattConnElement  *conn  = CSR_BT_GATT_CONN_INST_ADD_LAST(inst->connInst);
    CsrBtResultCode       result = CSR_BT_GATT_RESULT_INTERNAL_ERROR;
    
    CSR_UNUSED(mtu);
    
    if(conn)
    {
        conn->peerAddr      = prim->address;
        conn->gattId        = prim->gattId;
        conn->preferredMtu  = prim->preferredMtu;
        /* Note before csrBtGattSaveAcceptableConnParams is called 
           the conn->gattId shall be set */
        csrBtGattSaveAcceptableConnParams(inst, conn);

        if(CsrBtGattConnSetL2capFlags(conn, prim->flags,
                                      TRUE, /* central */
                                      FALSE)) /* !bredr */
        {
            GattConnArgs connargs;

            /* Connection checks are done in the FSM */
            connargs.result     = CSR_BT_GATT_RESULT_SUCCESS;
            connargs.supplier   = CSR_BT_SUPPLIER_GATT;
            connargs.inst       = inst;
            GATT_FSMRUN(&connargs, GATT_CONN_EV_CENTRAL_REQ, conn);
            return;
        }
        else
        { 
            result = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
        }
    }
    /* Else - Failed to create a conn instance */
    csrBtGattConnRestoreFail(inst, 
                             conn, 
                             qelm,
                             prim->gattId,
                             CSR_BT_GATT_CENTRAL_CFM,
                             result);
}

CsrBool CsrBtGattCentralReqHandler(GattMainInst *inst)
{
    CsrBtGattCentralReq *prim = (CsrBtGattCentralReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId, 
                                   csrBtGattCentralRestoreHandler,
                                   NULL,
                                   NULL));
}

static void csrBtGattPeripheralRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst           *inst  = (GattMainInst *) gattInst;
    CsrBtGattQueueElement  *qelm  = (CsrBtGattQueueElement *) qElem;
    CsrBtGattPeripheralReq *prim  = (CsrBtGattPeripheralReq *) qelm->gattMsg;
    CsrBtGattConnElement   *conn  = CSR_BT_GATT_CONN_INST_ADD_LAST(inst->connInst);
    CsrBtResultCode        result = CSR_BT_GATT_RESULT_INTERNAL_ERROR;
    CsrBtGattAppElement    *app   = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst,
                                                                      &prim->gattId);
    CSR_UNUSED(mtu);

    if(conn && app)
    {
        /* Store AD/SR data */
        if(app->adv == NULL)
        {
            app->adv = CsrPmemZalloc(sizeof(CsrBtGattAppAdvParams));
        }
        app->adv->advFlags = prim->flags;
        
        if(!CsrBtGattStoreData(app->adv,
                               (CsrBool)(prim->flags & CSR_BT_GATT_FLAGS_APPEND_DATA
                                         ? TRUE : FALSE),
                               prim->advertisingData,
                               prim->advertisingDataLength,
                               prim->scanResponseData,
                               prim->scanResponseDataLength))
        {
            result = CSR_BT_GATT_RESULT_INVALID_LENGTH;
        }
        else
        {
            conn->peerAddr      = prim->address;
            conn->gattId        = prim->gattId;
            conn->preferredMtu  = prim->preferredMtu;

            /* Note before csrBtGattSaveAcceptableConnParams
               is called the conn->gattId shall be set */
            csrBtGattSaveAcceptableConnParams(inst, conn);
        
            if(CsrBtGattConnSetL2capFlags(conn, prim->flags,
                                          FALSE, FALSE)) /* !central, !bredr */
            {
                GattConnArgs connargs;

                /* Connection checks are done in the FSM */
                connargs.result = CSR_BT_GATT_RESULT_SUCCESS;
                connargs.supplier = CSR_BT_SUPPLIER_GATT;
                connargs.inst = inst;
                GATT_FSMRUN(&connargs, GATT_CONN_EV_PERIPHERAL_REQ, conn);
                return;
            }
            else
            {
                result = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
            }
        }
    }
    /* Else - Failed to create a conn instance */
    csrBtGattConnRestoreFail(inst, 
                             conn, 
                             qelm,
                             prim->gattId,
                             CSR_BT_GATT_PERIPHERAL_CFM,
                             result);
}

CsrBool CsrBtGattPeripheralReqHandler(GattMainInst *inst)
{
    CsrBtGattPeripheralReq *prim = (CsrBtGattPeripheralReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId,
                                   csrBtGattPeripheralRestoreHandler,
                                   NULL,
                                   NULL));
}

static void csrBtGattBredrConnectRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst             *inst  = (GattMainInst*) gattInst;
    CsrBtGattQueueElement    *qelm  = (CsrBtGattQueueElement*) qElem;
    CsrBtGattBredrConnectReq *prim  = (CsrBtGattBredrConnectReq*) qelm->gattMsg;
    CsrBtGattConnElement     *conn  = CSR_BT_GATT_CONN_INST_ADD_LAST(inst->connInst);
    CsrBtResultCode          result = CSR_BT_GATT_RESULT_INTERNAL_ERROR;

    CSR_UNUSED(mtu);

    if(conn)
    {
        conn->peerAddr     = prim->address;
        conn->gattId       = prim->gattId;
        conn->preferredMtu = CSR_BT_ATT_MTU_DEFAULT;

        if(CsrBtGattConnSetL2capFlags(conn, prim->flags,
                                      TRUE, TRUE)) /* master, bredr */
        {
            GattConnArgs connargs;

            /* Connection checks are done in the FSM */
            connargs.result = CSR_BT_GATT_RESULT_SUCCESS;
            connargs.supplier = CSR_BT_SUPPLIER_GATT;
            connargs.inst = inst;
            GATT_FSMRUN(&connargs, GATT_CONN_EV_BREDR_CONNECT_REQ, conn);
            return;
        }
        else
        {
            result = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
        }
    }
    /* Else - Failed to create a conn instance */
    csrBtGattConnRestoreFail(inst, 
                             conn, 
                             qelm,
                             prim->gattId,
                             CSR_BT_GATT_BREDR_CONNECT_CFM,
                             result);    
}

CsrBool CsrBtGattBredrConnectReqHandler(GattMainInst *inst)
{
    CsrBtGattBredrConnectReq *prim = (CsrBtGattBredrConnectReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId,
                                   csrBtGattBredrConnectRestoreHandler,
                                   NULL,
                                   NULL));
}

static void csrBtGattBredrAcceptRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst              = (GattMainInst*)gattInst;
    CsrBtGattQueueElement     *qelm = (CsrBtGattQueueElement*)qElem;
    CsrBtGattBredrAcceptReq  *prim  = (CsrBtGattBredrAcceptReq*)qelm->gattMsg;
    CsrBtGattConnElement     *conn  = CSR_BT_GATT_CONN_INST_ADD_LAST(inst->connInst);  
    CsrBtResultCode          result = CSR_BT_GATT_RESULT_INTERNAL_ERROR;
    
    CSR_UNUSED(mtu);
    
    if(conn)
    {
        conn->gattId = prim->gattId;
        if(CsrBtGattConnSetL2capFlags(conn, prim->flags,
                                      FALSE, TRUE)) /* slave, bredr */
        {
            GattConnArgs connargs;
            
            /* Connection checks are done in the FSM */
            connargs.result = CSR_BT_GATT_RESULT_SUCCESS;
            connargs.supplier = CSR_BT_SUPPLIER_GATT;
            connargs.inst = inst;
            GATT_FSMRUN(&connargs, GATT_CONN_EV_BREDR_ACCEPT_REQ, conn);
            return;
        }
        else
        {
            result = CSR_BT_GATT_BREDR_ACCEPT_CFM;
        }
    }
    /* Else - Failed to create a conn instance */
    csrBtGattConnRestoreFail(inst, 
                             conn, 
                             qelm,
                             prim->gattId,
                             CSR_BT_GATT_BREDR_ACCEPT_CFM,
                             result);    
    
}

CsrBool CsrBtGattBredrAcceptReqHandler(GattMainInst *inst)
{
    CsrBtGattBredrAcceptReq *prim = (CsrBtGattBredrAcceptReq*)inst->msg;
    return(csrBtGattNewReqHandler(inst, 
                                  CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                  prim->gattId,
                                  csrBtGattBredrAcceptRestoreHandler,
                                  NULL,
                                  NULL));
}

static void csrBtGattDisconnectRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst           *inst       = (GattMainInst*)gattInst;
    CsrBtGattQueueElement  *element    = (CsrBtGattQueueElement*)qElem;
    CsrBtGattDisconnectReq *prim       = (CsrBtGattDisconnectReq*)element->gattMsg;
    CsrBtGattConnElement   *conn       = NULL;
    
    if (prim->btConnId != CSR_BT_CONN_ID_INVALID)
    { /* Releasing connected logical link or cancelling
         a Central, Peripheral, or a BrEdrConnect connection */
        CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                   &conn,
                                                   prim->btConnId,
                                                   prim->gattId);
    }
    else
    { /* Cancelling a BrEdrAccept */
        CSR_BT_GATT_CONN_INST_FIND_BREDR_ACCEPT_PENDING(inst->connInst,
                                                        &conn,
                                                        prim->btConnId,
                                                        prim->gattId);
    }
    CSR_UNUSED(mtu);
 
    if(conn)
    {
        GattConnArgs connargs;
        connargs.result = CSR_BT_GATT_RESULT_SUCCESS;
        connargs.supplier = CSR_BT_SUPPLIER_GATT;
        connargs.inst = inst;
        GATT_FSMRUN(&connargs, GATT_CONN_EV_DISCONNECT_REQ, conn);
    }
    else
    {
        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattDisconnectReqHandler(GattMainInst *inst)
{
    CsrBtGattDisconnectReq *prim = (CsrBtGattDisconnectReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattDisconnectRestoreHandler,
                                   NULL,
                                   NULL));
}

/* Covers LE Scanning  procedures */
static void csrBtGattScanRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst             = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement*)qElem;
    CsrBtGattScanReq *prim         = (CsrBtGattScanReq*)element->gattMsg;
    CsrBtGattAppElement *app       = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                       &prim->gattId);
        
    CSR_UNUSED(mtu);

    if (app)
    {
        /* Always clear existing filters */
        CsrBtGattFreeAppScanFilters(app);

        /* Toggle this application's status */
        if(prim->enable)
        {
            app->mode |= CSR_BT_GATT_MODE_SCAN;

            if(app->scan == NULL)
            {
                app->scan = CsrPmemZalloc(sizeof(CsrBtGattAppScanParams));
            }
            app->scan->scanFlags = prim->scanFlags;

            /* Consume filter contents */
            app->scan->filterAddrCount = prim->filterAddrCount;
            app->scan->filterAddr = prim->filterAddr;
            app->scan->filterDataCount = prim->filterDataCount;
            app->scan->filterData = prim->filterData;
            prim->filterAddr = NULL;
            prim->filterData = NULL;
        }
        else
        {
            app->mode -= (app->mode & CSR_BT_GATT_MODE_SCAN);
        }

        CsrBtGattUpdateScan(inst);
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_SCAN_REQ no appElement",
                                CSR_BT_GATT_PRIM, prim->type, 0);
    }
}

CsrBool CsrBtGattScanReqHandler(GattMainInst *inst)
{
    CsrBtGattScanReq *prim = (CsrBtGattScanReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId,
                                   csrBtGattScanRestoreHandler,
                                   NULL,
                                   NULL));
}

/* Covers LE Advertise  procedures */
static void csrBtGattAdvertiseRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst             = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement*)qElem;
    CsrBtGattAdvertiseReq *prim    = (CsrBtGattAdvertiseReq*) element->gattMsg;
    CsrBtGattAppElement *app       = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                       &prim->gattId);
   
    CSR_UNUSED(mtu);

    if (app)
    {
        if(prim->enable)
        {
            app->mode |= CSR_BT_GATT_MODE_ADVERTISE;

            if(app->adv == NULL)
            {
                app->adv = CsrPmemZalloc(sizeof(CsrBtGattAppAdvParams));
            }
            app->adv->advFlags = prim->advertisingFlags;
            if(!CsrBtGattStoreData(app->adv,
                                   (CsrBool)(prim->advertisingFlags & CSR_BT_GATT_FLAGS_APPEND_DATA
                                             ? TRUE : FALSE),
                                   prim->advertisingData,
                                   prim->advertisingDataLength,
                                   prim->scanResponseData,
                                   prim->scanResponseDataLength))
            {
                CsrBtConnId btConnId = CSR_BT_GATT_LOCAL_BT_CONN_ID;

                CsrBtGattQueueRestoreHandler(inst, 
                                             CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId));
                
                CsrBtGattStdCfmSend(CSR_BT_GATT_ADVERTISE_CFM,
                                    app->gattId,
                                    CSR_BT_GATT_RESULT_INVALID_LENGTH,
                                    CSR_BT_SUPPLIER_GATT);
                return;
            }
        }
        else
        {
            app->mode -= (app->mode & CSR_BT_GATT_MODE_ADVERTISE);
            if(app->adv)
            {
                app->adv->advFlags = 0;
            }
        }

        CsrBtGattUpdateAdvertise(inst);
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_ADVERTISE_REQ no appElement",
                                CSR_BT_GATT_PRIM, prim->type, 0);
    }
}

CsrBool CsrBtGattAdvertiseReqHandler(GattMainInst *inst)
{
    CsrBtGattAdvertiseReq *prim = (CsrBtGattAdvertiseReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId,
                                   csrBtGattAdvertiseRestoreHandler,
                                   NULL,
                                   NULL));
}

static void csrBtGattParamScanRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst             = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement*)qElem;
    CsrBtGattParamScanReq *prim    = (CsrBtGattParamScanReq*) element->gattMsg;
    CsrBtGattAppElement *app       = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                       &prim->gattId);

    CSR_UNUSED(mtu);

    if (app)
    {
        if(app->scan == NULL)
        {
            app->scan = CsrPmemZalloc(sizeof(CsrBtGattAppScanParams));
        }

        app->mode |= CSR_BT_GATT_SCAN_CONFIG_SET;
        app->scan->scanInterval = prim->scanInterval;
        app->scan->scanWindow = prim->scanWindow;
    
        CsrBtGattUpdateScan(inst);
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_PARAM_SCAN_REQ no appElement",
                                CSR_BT_GATT_PRIM, prim->type, 0);
    }
}

CsrBool CsrBtGattParamScanReqHandler(GattMainInst *inst)
{
    CsrBtGattParamScanReq *prim = (CsrBtGattParamScanReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId,
                                   csrBtGattParamScanRestoreHandler,
                                   NULL,
                                   NULL));
}

static void csrBtGattParamAdvertiseRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst               = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element   = (CsrBtGattQueueElement*)qElem;
    CsrBtGattParamAdvertiseReq *prim = (CsrBtGattParamAdvertiseReq*) element->gattMsg;
    CsrBtGattAppElement *app         = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                         &prim->gattId);

    CSR_UNUSED(mtu);

    if (app)
    {
        if(app->adv == NULL)
        {
            app->adv = CsrPmemZalloc(sizeof(CsrBtGattAppAdvParams));
        }

        app->mode |= CSR_BT_GATT_ADV_CONFIG_SET;
        app->adv->advIntervalMin = prim->advIntervalMin;
        app->adv->advIntervalMax = prim->advIntervalMax;
        app->adv->advertisingChannelMap = prim->advertisingChannelMap;

        CsrBtGattUpdateAdvertise(inst);
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_PARAM_ADVERTISE_REQ no appElement",
                                CSR_BT_GATT_PRIM, prim->type, 0);
    }
}

CsrBool CsrBtGattParamAdvertiseReqHandler(GattMainInst *inst)
{
    CsrBtGattParamAdvertiseReq *prim = (CsrBtGattParamAdvertiseReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                   prim->gattId,
                                   csrBtGattParamAdvertiseRestoreHandler,
                                   NULL,
                                   NULL));
}

#ifdef CSR_BT_INSTALL_GATT_PARAM_CONNECTION
static void csrBtGattParamConnectionRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst                = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element    = (CsrBtGattQueueElement*)qElem;
    CsrBtGattParamConnectionReq *prim = (CsrBtGattParamConnectionReq*) element->gattMsg;
    CsrBtGattAppElement *app          = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                          &prim->gattId);

    CSR_UNUSED(mtu);

    if (app)
    {
        if(app->conn == NULL)
        {
            app->conn = CsrPmemAlloc(sizeof(CsrBtGattAppConnParams));
        }

        app->conn->scanInterval = prim->scanInterval;
        app->conn->scanWindow = prim->scanWindow;
        app->conn->connIntervalMin = prim->connIntervalMin;
        app->conn->connIntervalMax = prim->connIntervalMax;
        app->conn->connLatency = prim->connLatency;
        app->conn->supervisionTimeout = prim->supervisionTimeout;
        app->conn->connAttemptTimeout = prim->connAttemptTimeout;
        app->conn->advIntervalMin = prim->advIntervalMin;
        app->conn->advIntervalMax = prim->advIntervalMax;
        app->conn->connLatencyMax = prim->connLatencyMax;
        app->conn->supervisionTimeoutMin = prim->supervisionTimeoutMin;
        app->conn->supervisionTimeoutMax = prim->supervisionTimeoutMax;

        CsrBtGattUpdateConnection(inst);
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_PARAM_CONNECTION_REQ no appElement",
                                    CSR_BT_GATT_PRIM, prim->type, 0);
    }
}
#endif

#ifdef CSR_BT_INSTALL_GATT_PARAM_CONNECTION
CsrBool CsrBtGattParamConnectionReqHandler(GattMainInst *inst)
{
    CsrBtGattParamConnectionReq *prim = (CsrBtGattParamConnectionReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID,
                                   prim->gattId,
                                   csrBtGattParamConnectionRestoreHandler,
                                   NULL,
                                   NULL));
}
#endif

static void csrBtGattParamConnUpdateRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst                  = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element      = (CsrBtGattQueueElement*)qElem;
    CsrBtGattParamConnUpdateReq *prim   = (CsrBtGattParamConnUpdateReq*)element->gattMsg;
    CsrBtGattConnElement *conn          = NULL;

    if (prim->gattId != inst->privateGattId)
    {
        CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                   &conn,
                                                   prim->btConnId,
                                                   prim->gattId);
    }
    else
    {
        conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst, 
                                                               &prim->btConnId);
    }

    CSR_UNUSED(mtu);
    
    if (conn)
    {
        CsrBtGattConnParamUpdateReqIds ids;

        if (prim->connIntervalMin >= L2CA_CON_INTERVAL_MIN   && 
            prim->connIntervalMin <= prim->connIntervalMax   &&
            prim->connIntervalMax <= L2CA_CON_INTERVAL_MAX   &&
            prim->supervisionTimeout >= L2CA_CON_TIMEOUT_MIN &&
            prim->supervisionTimeout <= L2CA_CON_TIMEOUT_MAX &&
            prim->connLatency <= L2CA_CON_SLAVE_LATENCY_MAX  &&
            ((prim->connLatency + 1) <= ((prim->supervisionTimeout * 8)/ prim->connIntervalMax)))
        { /* The connection parameters are valid */ 
            inst->connParamUpdateIdCounter++;

            if (inst->connParamUpdateIdCounter == CSR_BT_GATT_CONN_PARAM_UPDATE_IDENTIFIER_INVALID)
            {
                inst->connParamUpdateIdCounter++;
            }

            /* Check if all application running on the same physical connection
               can accept these requested intervals */
            CSR_BT_GATT_CONN_INST_ITERATE_UPDATE_PARAM_REQ(inst,
                                                           inst->connInst,
                                                           ids,
                                                           prim->gattId,
                                                           prim->btConnId,
                                                           prim->connIntervalMin,
                                                           prim->connIntervalMax, 
                                                           prim->connLatency,
                                                           prim->supervisionTimeout,
                                                           inst->connParamUpdateIdCounter);
        }
        else
        { /* The connection parameters are invalid */
            ids.numOfRsp = 0;
            ids.accept   = FALSE;
        }

        if (ids.numOfRsp == 0)
        { 
            if (prim->gattId != inst->privateGattId)
            { /* Local initiated */
                if (ids.accept)
                { /* All applications has accepted the new connection 
                     intervals and the connection parameter are valid */
                    CsrBtCmLeConnparamUpdateReqSend(CSR_BT_GATT_IFACEQUEUE,
                                                    conn->peerAddr,
                                                    prim->connIntervalMin,
                                                    prim->connIntervalMax,
                                                    prim->connLatency,
                                                    prim->supervisionTimeout,
                                                    prim->minimumCeLength,
                                                    prim->maximumCeLength);
                }
                else
                { /* The connection paramters are invalid. Send confirm to the 
                     application with an error */
                    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_PARAM_CONN_UPDATE_CFM,
                                                prim->gattId,
                                                CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER,
                                                CSR_BT_SUPPLIER_GATT,
                                                prim->btConnId);
                    
                    /* This procedure has finished. Start the next if any */
                    CsrBtGattQueueRestoreHandler(inst, element);
                }
            }
            else
            { /* Requested by remote slave device - Either accepted or rejected
                 Accepted if the interval is within range, rejected if outside 
                 range and no apps has subscribe for this event */
                CsrBtCmLeAcceptConnparamUpdateResSend(conn->peerAddr,
                                                      prim->l2caSignalId,
                                                      prim->connIntervalMin,
                                                      prim->connIntervalMax,
                                                      prim->connLatency,
                                                      prim->supervisionTimeout,
                                                      ids.accept);
                
                /* This procedure has finished. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, element);
            }
        }
        else
        { /* Wait for the ids.numOfRsp CsrBtGattParamConnUpdateRes before
             sending this request to the controller */
            prim->accept = ids.accept;
        }
    }
    else
    {
        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattParamConnUpdateReqHandler(GattMainInst *inst)
{
    CsrBtGattParamConnUpdateReq *prim = (CsrBtGattParamConnUpdateReq*)inst->msg;

    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattParamConnUpdateRestoreHandler,
                                   NULL,
                                   NULL));
}

CsrBool CsrBtGattParamConnUpdateResHandler(GattMainInst *inst)
{
    CsrBtGattConnElement   *conn      = NULL;
    CsrBtGattParamConnUpdateRes *prim = (CsrBtGattParamConnUpdateRes*) inst->msg;
    
    CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                               &conn,
                                               prim->btConnId,
                                               prim->gattId);

    if (conn && 
        CSR_BT_GATT_CONN_IS_CONNECTED(conn->state) &&
        conn->connParamUpdateId == prim->identifier)
    {
        CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &prim->btConnId);

        if (qElem && qElem->gattMsg && (*(CsrBtGattPrim *) qElem->gattMsg) == CSR_BT_GATT_PARAM_CONN_UPDATE_REQ)
        {   
            CsrBtGattConnParamUpdateResIds ids;
            CsrBtGattParamConnUpdateReq *reqMsg = (CsrBtGattParamConnUpdateReq *) qElem->gattMsg;

            conn->connParamUpdateId = CSR_BT_GATT_CONN_PARAM_UPDATE_IDENTIFIER_INVALID;

            if (prim->accept)
            { /* The application has accepted the request to update the 
                 connection parameters. Updated the acceptable connection 
                 parameters */
                conn->connIntervalMin = reqMsg->connIntervalMin;
                conn->connIntervalMax = reqMsg->connIntervalMax;
                conn->connLatencyMax  = reqMsg->connLatency;
                
                if (reqMsg->supervisionTimeout > conn->supervisionTimeoutMax)
                {
                    conn->supervisionTimeoutMax = reqMsg->supervisionTimeout;
                }
                else if (reqMsg->supervisionTimeout < conn->supervisionTimeoutMin)
                {
                    conn->supervisionTimeoutMin = reqMsg->supervisionTimeout;
                }                
            }
            else
            { /* The application has rejected the request to update the 
                 connection parameters */
                 reqMsg->accept = FALSE;
            }

            /* Check if this procedure is done */
            CSR_BT_GATT_CONN_INST_ITERATE_UPDATE_PARAM_RES(inst->connInst, 
                                                           ids,
                                                           prim->btConnId,
                                                           prim->identifier,
                                                           FALSE);

            if (ids.numOfRsp == 0)
            {
                if (reqMsg->gattId == inst->privateGattId)
                {
                    CsrBtCmLeAcceptConnparamUpdateResSend(conn->peerAddr,
                                                          reqMsg->l2caSignalId,
                                                          reqMsg->connIntervalMin,
                                                          reqMsg->connIntervalMax,
                                                          reqMsg->connLatency,
                                                          reqMsg->supervisionTimeout,
                                                          reqMsg->accept);

                    /* This procedure has finished. Start the next if any */
                    CsrBtGattQueueRestoreHandler(inst, qElem);
                }
                else
                {
                    if (reqMsg->accept)
                    {
                        CsrBtCmLeConnparamUpdateReqSend(CSR_BT_GATT_IFACEQUEUE,
                                                        conn->peerAddr,
                                                        reqMsg->connIntervalMin,
                                                        reqMsg->connIntervalMax,
                                                        reqMsg->connLatency,
                                                        reqMsg->supervisionTimeout,
                                                        reqMsg->minimumCeLength,
                                                        reqMsg->maximumCeLength);
                    }
                    else
                    {
                        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_PARAM_CONN_UPDATE_CFM,
                                                    reqMsg->gattId,
                                                    CSR_BT_GATT_RESULT_PARAM_CONN_UPDATE_LOCAL_REJECT, 
                                                    CSR_BT_SUPPLIER_GATT,
                                                    prim->btConnId);

                        /* This procedure has finished. Start the next if any */
                        CsrBtGattQueueRestoreHandler(inst, qElem);
                    }
                }
            }
            /* Else - Still waiting for ids.numOfRsp application to response */
        }
        else
        { /* Return FALSE to indicate an exception */
            return FALSE;
        }
    }
    /* Else - The connection has been release while waiting for this response. */
    return TRUE;
}

static void csrBtGattWhitelistAddRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst             = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *qelm    = (CsrBtGattQueueElement*)qElem;
    CsrBtGattWhitelistAddReq *prim = (CsrBtGattWhitelistAddReq*)qelm->gattMsg;

    CSR_UNUSED(mtu);
    if(prim->addressCount == 0)
    {
        /* Must contain at least one address */
        CsrBtGattStdCfmSend(CSR_BT_GATT_WHITELIST_ADD_CFM,
                            prim->gattId,
                            CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER,
                            CSR_BT_SUPPLIER_GATT);
        CsrBtGattQueueRestoreHandler(inst, qelm);       
    }
    else
    {
        /* Append new addresses. This consumes the primitive data */
        CsrBtGattWhitelistAdd(gattInst,
                              prim->addressCount,
                              prim->address);
        prim->address = NULL;
        prim->addressCount = 0;
    }
}

CsrBool CsrBtGattWhitelistAddReqHandler(GattMainInst *inst)
{
    CsrBtGattWhitelistAddReq *prim = (CsrBtGattWhitelistAddReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID,
                                   prim->gattId,
                                   csrBtGattWhitelistAddRestoreHandler,
                                   NULL,
                                   NULL));
}

static void csrBtGattWhitelistClearRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    CsrBtTypedAddr zeroaddr;
    CSR_UNUSED(mtu);
    CSR_UNUSED(gattInst);
    CSR_UNUSED(qElem);

    /* Send command */
    CsrBtAddrZero(&zeroaddr);
    CsrBtCmLeWhitelistSetReqSend(CSR_BT_GATT_IFACEQUEUE,
                                 CSR_BT_CM_LE_WHITELIST_CLEAR,
                                 zeroaddr);
}

CsrBool CsrBtGattWhitelistClearReqHandler(GattMainInst *inst)
{
    CsrBtGattWhitelistClearReq *prim = (CsrBtGattWhitelistClearReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID,
                                   prim->gattId,
                                   csrBtGattWhitelistClearRestoreHandler,
                                   NULL,
                                   NULL));
}

static void csrBtGattWhitelistReadRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst *inst              = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element  = (CsrBtGattQueueElement*)qElem;
    CsrBtGattWhitelistReadReq *prim = (CsrBtGattWhitelistReadReq*)element->gattMsg;

    CSR_UNUSED(mtu);
    CsrBtGattWhitelistReadCfmSend(prim->gattId,
                                  inst->whitelistCount,
                                  CsrMemDup(inst->whitelist,
                                            inst->whitelistCount*sizeof(CsrBtTypedAddr)));
    CsrBtGattQueueRestoreHandler(inst, element);
}

CsrBool CsrBtGattWhitelistReadReqHandler(GattMainInst *inst)
{
    CsrBtGattWhitelistReadReq *prim = (CsrBtGattWhitelistReadReq *)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   CSR_BT_GATT_LOCAL_BT_CONN_ID,
                                   prim->gattId,
                                   csrBtGattWhitelistReadRestoreHandler,
                                   NULL,
                                   NULL));
}

/* Covers DataBase Configuration */
static CsrUint16 csrBtGattAppInstGetFirstAttrHandle(GattMainInst     *inst,
                                                    CsrUint16        numOfAttrHandles,
                                                    CsrUint16        preferredStartHandle)
{ /* Check if GATT can assign the number of requested attribute
     handles after the worst fit algoritme */
    CsrBtGattAppElement *elem;

    CsrUint16 startHandle  = CSR_BT_GATT_ATTR_HANDLE_INVALID;
    CsrUint16 gap          = 0;
    CsrUint16 maxGap       = 0;
    CsrUint16 nextFree     = 0;   

    /* As GATT has assign a number of handles to itself e.g. there
     * must always be at least one element in the list */
    for (elem = CSR_BT_GATT_APP_INST_GET_FIRST(inst->appInst); elem; elem = elem->next)
    {
        if (elem->start != CSR_BT_GATT_ATTR_HANDLE_INVALID)
        {
            /* GATT always insure that element with the start value
             * CSR_BT_GATT_ATTR_HANDLE_INVALID always is placed first
             * in the list. */
            nextFree = elem->end + 1;

            if (elem->next)
            {
                gap = (elem->next->start - nextFree);
            }
            else if (elem->end == CSR_BT_GATT_ATTR_HANDLE_MAX)
            {
                gap = 0;
            }
            else
            {
                gap = (CSR_BT_GATT_ATTR_HANDLE_MAX - nextFree);
            }
            
            if (preferredStartHandle > 0           && 
                gap >= numOfAttrHandles            &&
                preferredStartHandle >= nextFree   &&
                (preferredStartHandle + numOfAttrHandles) <= (nextFree + gap))
            {
                return preferredStartHandle;
            }
            else if (gap == numOfAttrHandles)
            {
                /* Found a free gap which exact match the number of
                 * requested attributes */
                return nextFree;
            }
            else if (gap > maxGap)
            {
                /* Found the largest gap so far, update maxGap */
                maxGap      = gap;
                startHandle = nextFree;
            }
        }
    }

    if (maxGap > numOfAttrHandles)
    {
        /* GATT can assign the the number of requested attribute
         * handles */
        return startHandle;
    }

    /* Could not assing the requested number of attribute handles */
    return CSR_BT_GATT_ATTR_HANDLE_INVALID;
}


CsrBool CsrBtGattDbAllocReqHandler(GattMainInst *inst)
{
    CsrBtGattDbAllocReq *prim = (CsrBtGattDbAllocReq*)inst->msg;
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &prim->gattId);

    if (appElement)
    {
        CsrBtResultCode resultCode = CSR_BT_GATT_RESULT_SUCCESS;

        if (appElement->start > 0)
        {
            /* The application have already allocated some attribute handles */
            resultCode = CSR_BT_GATT_RESULT_ATTR_HANDLES_ALREADY_ALLOCATED;
        }
        else if (prim->numOfAttrHandles == 0)
        {
            resultCode = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
        }
        else
        {
            CsrUint16 start = csrBtGattAppInstGetFirstAttrHandle(inst, 
                                                                 prim->numOfAttrHandles,
                                                                 prim->preferredStartHandle);

            if (start == CSR_BT_GATT_ATTR_HANDLE_INVALID)
            {
                /* Cannot meet the application request - Return ERROR */
                resultCode = CSR_BT_GATT_RESULT_INSUFFICIENT_NUM_OF_HANDLES;
            }
            else
            {
                /* Is able to meet the application request add it to appInst */
                appElement->start = start;
                appElement->end   = (start + prim->numOfAttrHandles);
                CSR_BT_GATT_APP_INST_SORT_BY_ATTR_VALUE(inst->appInst);
            }
        }
        /* Reply to app */
        CsrBtGattDbAllocCfmSend(prim->gattId,
                                resultCode,
                                CSR_BT_SUPPLIER_GATT,
                                appElement->start,
                                appElement->end,
                                prim->preferredStartHandle);
        return TRUE;
    }
    return FALSE;
}

static void csrBtGattDbDeallocRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst          *inst       = (GattMainInst*) gattInst;
    CsrBtGattQueueElement *element    = (CsrBtGattQueueElement*) qElem;
    CsrBtGattDbDeallocReq *prim       = (CsrBtGattDbDeallocReq*) element->gattMsg;
    CsrBtGattAppElement   *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                          &prim->gattId);

    CSR_UNUSED(mtu);
    
    if (appElement)
    {
        if (appElement->start == 0)
        {
            CsrBtGattDbDeallocCfmSend(prim->gattId,
                                      CSR_BT_GATT_RESULT_SUCCESS,
                                      CSR_BT_SUPPLIER_GATT,
                                      appElement->start,
                                      appElement->end);

            /* This procedure has finished. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, element);
        }
        else
        { /* GATT makes sure the handles that the application want to
             deallocate also is remove from the data base */
            attlib_remove_req(CSR_BT_GATT_IFACEQUEUE, 
                              appElement->start, 
                              appElement->end, 
                              NULL);
        }
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_DB_DEALLOC_REQ no appElement",
                                    CSR_BT_GATT_PRIM, prim->type, 0);
    }
}

CsrBool CsrBtGattDbDeallocReqHandler(GattMainInst *inst)
{
    CsrBtGattDbDeallocReq *prim = (CsrBtGattDbDeallocReq*)inst->msg;
    return(csrBtGattNewReqHandler(inst, 
                                  CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                  prim->gattId,
                                  csrBtGattDbDeallocRestoreHandler,
                                  NULL,
                                  NULL));
}

/* Scan for end-handle for given service */
static CsrBtGattHandle csrBtGattGetServiceEndHandle(CsrBtGattDb *db,
                                                    CsrBtGattHandle start)
{
    CsrBtGattHandle targetEnd;
    CsrBtGattHandle finalEnd;
    CsrBtGattDb *find;
    
    targetEnd = CSR_BT_GATT_ATTR_HANDLE_MAX;
    finalEnd = 0;

    /* Find next primary or secondary service in unordered list. The
     * target end is then next's start handle - 1 */
    for (find = db; find != NULL; find = find->next)
    {
        if ((find->handle != CSR_BT_GATT_ATTR_HANDLE_INVALID)
            && (find->handle > start)
            && (find->handle < targetEnd)
            && ((CsrMemCmp(csrBtGattPrimaryService, find->uuid, CSR_BT_UUID128_SIZE) == 0)
                || (CsrMemCmp(csrBtGattSecondaryService, find->uuid, CSR_BT_UUID16_SIZE) == 0)))
        {
            targetEnd = find->handle - 1;
        }

        /* Track the all-time-highest */
            if ((find->handle != CSR_BT_GATT_ATTR_HANDLE_INVALID)
                && (find->handle > finalEnd))
        {
            finalEnd = find->handle;
        }
    }
    
    /* If no services follows the current one, end-handle is the
     * all-time highest as that will be the end of the last service */
    if (targetEnd == CSR_BT_GATT_ATTR_HANDLE_MAX)
    {
        targetEnd = finalEnd;
    }

    return targetEnd;
}

/* Add SDP records for BR/EDR-capable services */
static void csrBtGattBrEdrSupportHandler(GattMainInst *inst,
                                         CsrBtGattDb  *db)
{
    CsrBtGattDb *find;

    /* Scan database and look for BR/EDR-capable services */
    for (find = db; find != NULL; find = find->next)
    {
        if ((find->handle != CSR_BT_GATT_ATTR_HANDLE_INVALID)
            && (CsrMemCmp(csrBtGattPrimaryService, find->uuid, CSR_BT_UUID128_SIZE) == 0)
            && !(find->flags & CSR_BT_GATT_ATTR_FLAGS_DISABLE_BREDR))
        {
            /* Primary or secondary service supports BR/EDR. Add to SDP list and
             * find the end-handle */
            CsrBtGattSdpRecordElement *sdpElem;
            CsrUint8 i;

            sdpElem = CSR_BT_GATT_SDP_INST_ADD_LAST(inst->sdpRecordInst);
            sdpElem->startHandle = find->handle;
            sdpElem->endHandle = csrBtGattGetServiceEndHandle(db, find->handle);

            /* Save UUID in big indian as SDP using this */            
            sdpElem->uuid.length = (CsrBtUuidSize) find->size_value;
            CsrMemSet(sdpElem->uuid.uuid, 0, CSR_BT_UUID128_SIZE);
            for (i = 0; i < find->size_value; i++)
            {
                sdpElem->uuid.uuid[i] = find->value[(find->size_value - i - 1)];
            }
        }
    }
}

static void csrBtGattDbAddRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst          *inst       = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element    = (CsrBtGattQueueElement*)qElem;
    CsrBtGattDbAddReq     *prim       = (CsrBtGattDbAddReq*)element->gattMsg;
    CsrBtGattAppElement   *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                          &prim->gattId);
    CSR_UNUSED(mtu);
    
    if (prim->db && appElement && csrBtGattValidateAttrHandles(prim->db, appElement))
    {
        csrBtGattBrEdrSupportHandler(inst, prim->db);
        attlib_add_req(CSR_BT_GATT_IFACEQUEUE, prim->db, NULL);
        prim->db = NULL;
    }
    else
    {
        CsrBtGattStdCfmSend(CSR_BT_GATT_DB_ADD_CFM,
                            prim->gattId,
                            CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER,
                            CSR_BT_SUPPLIER_GATT);
        
        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattDbAddReqHandler(GattMainInst *inst)
{
    CsrBtGattDbAddReq *prim      = (CsrBtGattDbAddReq*)inst->msg;
    return(csrBtGattNewReqHandler(inst, 
                                  CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                  prim->gattId,
                                  csrBtGattDbAddRestoreHandler,
                                  NULL,
                                  NULL));
}

static void csrBtGattDbRemoveRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst          *inst       = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element    = (CsrBtGattQueueElement*)qElem;
    CsrBtGattDbRemoveReq  *prim       = (CsrBtGattDbRemoveReq*)element->gattMsg;
    CsrBtGattAppElement   *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &prim->gattId);
        
    CSR_UNUSED(mtu);

    if (appElement)
    {
        if (prim->start >= appElement->start &&
            prim->end <=   appElement->end)
        {
            attlib_remove_req(CSR_BT_GATT_IFACEQUEUE, 
                              prim->start, 
                              prim->end, 
                              NULL);
        }
        else
        {
            CsrBtGattDbRemoveCfmSend(prim->gattId,
                                     CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER,
                                     CSR_BT_SUPPLIER_GATT,
                                     0);

            /* This procedure has finished. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, element);
        }
    }
    else
    {
        CsrStateEventException("CSR_BT_GATT_DB_REMOVE_REQ no appElement",
                                    CSR_BT_GATT_PRIM, prim->type, 0);
    }
}

CsrBool CsrBtGattDbRemoveReqHandler(GattMainInst *inst)
{
    CsrBtGattDbRemoveReq *prim = (CsrBtGattDbRemoveReq*)inst->msg;

    return(csrBtGattNewReqHandler(inst, 
                                  CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                  prim->gattId,
                                  csrBtGattDbRemoveRestoreHandler,
                                  NULL,
                                  NULL));
}

CsrBool CsrBtGattDbAccessResHandler(GattMainInst *inst)
{
    CsrBtGattDbAccessRes *prim = (CsrBtGattDbAccessRes*)inst->msg;
    CsrUint16 cid;
    CsrBool active;

    cid = CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId);

    /* If successful, see if this response triggers another prepared
     * access ind */
    if (prim->responseCode == CSR_BT_GATT_ACCESS_RES_SUCCESS)
    {
        active = CsrBtGattPlayExecuteWrite(inst, cid);
    }
    else
    {
        /* Mark all pending as bad */
        CsrBtGattPlayExecuteMarkFail(inst, cid);
        active = FALSE;
    }

    if(!active)
    {
        /* Finalise whatever process we were running for this CID */
        CsrBtGattPlayExecuteFinalise(inst, cid);
        
        /* ...and tell ATT */
        if (CsrBtGattValidateBtConnIdByMtu(inst, prim->gattId, prim->btConnId) > 0)
        {
            attlib_access_rsp(CSR_BT_GATT_IFACEQUEUE, 
                              CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId), 
                              prim->attrHandle, 
                              (att_result_t) prim->responseCode, 
                              prim->valueLength, 
                              prim->value, 
                              NULL);
            
            /* Local operation - can not fail */
            prim->value = NULL;
        }
    }
    /* Else, we are still active and PlayExecuteWrite has sent the
     * next access request. We should do no futher processing here */


    /* Always processed */
    return TRUE;
}

/* Covers SDS Register/Unregister handling  */
#define CSR_BT_GATT_SERVICE_RECORD_INDEX_UUID       0x06
#define CSR_BT_GATT_SERVICE_RECORD_INDEX_START      0x13
#define CSR_BT_GATT_SERVICE_RECORD_INDEX_END        0x16

static const CsrUint8 gatt16BitServiceClassServiceRecord[] =
{
    /* Service class ID list */
    0x09, 0x00, 0x01,      /* AttrID , ServiceClassIDList */
    0x35, 0x03,            /* 3 bytes in total DataElSeq */
    0x19, 0x00, 0x00,      /* 2 byte UUID, Service class */
};

static const CsrUint8 gatt128BitServiceClassServiceRecord[] =
{
    /* Service class ID list */
    0x09, 0x00, 0x01,             /* AttrID , ServiceClassIDList */
    0x35, 0x11,                   /* 17 bytes in total DataElSeq */
    0x1C, 0x00, 0x00, 0x00, 0x00, /* 16 byte UUID, Service class */
          0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00,
};

static const CsrUint8 gattCommonServiceRecord[] =
{
    /* Protocol descriptor list */
    0x09, 0x00, 0x04,      /* AttrId ProtocolDescriptorList */
    0x35, 0x13,            /* 19 bytes (0x13) in total DataElSeq */

    /* L2CAP */
    0x35, 0x06,            /* Data element seq. 6 bytes */
    0x19, 0x01, 0x00,      /* UUID(2 bytes), Protocol = L2CAP */
    0x09, 0x00, 0x1F,      /* PSM value = ATT = 0x001F */

    /* ATT */
    0x35, 0x09,            /* Data element seq. 6 bytes */
    0x19, 0x00, 0x07,      /* UUID(2 bytes), Protocol = ATT */

    /* The GATT Start Handle, shall be set to the attribute handle 
       of the Generic Attribute Profile service declaration */
    0x09, 0x00,0x00,
    
    /* The GATT End Handle, shall be set to the attribute handle 
       of the last attribute within the Generic Attribute Profile */      
    0x09, 0x00,0x00,

    /* BrowseGroupList */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static void csrBtGattRegisterSdpRecord(CsrBtGattSdpRecordElement *sdpElem)
{ /* This function makes a SDP record and request SDP to register it */
    CsrUint8    *sdpRecord;
    CsrUint16   offset, i, commonLength = sizeof(gattCommonServiceRecord);
    
    if (sdpElem->uuid.length == CSR_BT_UUID16_SIZE)
    {
        offset    = (CsrUint16)(sizeof(gatt16BitServiceClassServiceRecord));
        sdpRecord = (CsrUint8 *) CsrPmemAlloc(offset + commonLength);
        CsrMemCpy(sdpRecord, gatt16BitServiceClassServiceRecord, sizeof(gatt16BitServiceClassServiceRecord));
    }
    else
    {
        offset    = (CsrUint16)(sizeof(gatt128BitServiceClassServiceRecord));
        sdpRecord = (CsrUint8 *) CsrPmemAlloc(offset + commonLength);
        CsrMemCpy(sdpRecord, gatt128BitServiceClassServiceRecord, sizeof(gatt128BitServiceClassServiceRecord));
    }
    for (i = 0; i < sdpElem->uuid.length; i++)
    {
        sdpRecord[CSR_BT_GATT_SERVICE_RECORD_INDEX_UUID + i] = sdpElem->uuid.uuid[i];
    }
    CsrMemCpy(&sdpRecord[offset], gattCommonServiceRecord, commonLength);
    sdpRecord[CSR_BT_GATT_SERVICE_RECORD_INDEX_START + offset]     = (CsrUint8)(sdpElem->startHandle >> 8);
    sdpRecord[CSR_BT_GATT_SERVICE_RECORD_INDEX_START + offset + 1] = (CsrUint8)(sdpElem->startHandle & 0x00FF);
    sdpRecord[CSR_BT_GATT_SERVICE_RECORD_INDEX_END + offset]       = (CsrUint8)(sdpElem->endHandle >> 8);
    sdpRecord[CSR_BT_GATT_SERVICE_RECORD_INDEX_END + offset + 1]   = (CsrUint8)(sdpElem->endHandle & 0x00FF);
    CsrBtCmSdsRegisterReqSend(CSR_BT_GATT_IFACEQUEUE, sdpRecord, (CsrUint16)(commonLength + offset));
}

void CsrBtGattSdsRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{ 
    CsrBtGattSdpRecordElement *sdpElem;
    GattMainInst *inst = (GattMainInst *) gattInst;
    
    CSR_UNUSED(mtu);

    CSR_BT_GATT_SDP_INST_FIND_STATE(inst->sdpRecordInst, 
                                    &sdpElem, 
                                    CSR_BT_GATT_SDP_PENDING);
    
    if (sdpElem)
    { /* Found a SDP Element that either needs to be Register or  
         Unregister */
        if (sdpElem->serviceRecordHandle == CMN_SDR_INVALID_SERVICE_HANDLE)
        { /* This SDP element must be register. Build the SDP record
             and request SDP to register it */
            csrBtGattRegisterSdpRecord(sdpElem);
        }
        else
        { /* Request to remove this record */
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_GATT_IFACEQUEUE, 
                                        sdpElem->serviceRecordHandle);            
        }
        /* Change state to CSR_BT_GATT_SDP_IN_PROGESS so GATT
           recognize this sdp Element when it receives the 
           CSR_BT_CM_SDS_REGISTER_CFM or 
           CSR_BT_CM_SDS_UNREGISTER_CFM messagev */
        sdpElem->state = CSR_BT_GATT_SDP_IN_PROGESS;
    }
    else
    {
        /* All SDP records has been Register/Unregister. Call
         * CsrBtCmSetEventMaskReqSend to prevent that GATT
         * receives CSR_BT_CM_ACL_DISCONNECT_IND messages */
        CsrBtCmSetEventMaskReqSend(CSR_BT_GATT_IFACEQUEUE,
                                   CSR_BT_GATT_DEFAULT_CM_EVENT_MASK,
                                   CSR_BT_CM_EVENT_MASK_COND_SUCCESS);
        
        /* This procedure has finished. Start the next if any */  
        CsrBtGattQueueRestoreHandler(inst, (CsrBtGattQueueElement*) qElem);
    }
}

void CsrBtGattSdsRequestHandler(GattMainInst *inst, CsrUint16 event)
{ 
    CsrBtGattSdsReq *prim = (CsrBtGattSdsReq *) 
                             CsrPmemAlloc(sizeof(CsrBtGattSdsReq));

    prim->type            = CSR_BT_GATT_SDS_REQ;
 
    /* Pfree inst->msg */
    switch (event)
    {
        case CSR_BT_ATT_PRIM:
        { 
            attlib_free((ATT_UPRIM_T *)inst->msg);
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, inst->msg);
            CsrPmemFree(inst->msg);
            break;
        }
        default:
        {
            CsrStateEventException("CsrBtGattSdsRequestHandler - unknown event", event, *(CsrPrim *)inst->msg, 0);
            break;
        }
    }
    /* Add CSR_BT_GATT_SDS_REQ message to GATT message queue */
    inst->msg = prim;
    (void)(csrBtGattNewReqHandler(inst, 
                                  CSR_BT_GATT_LOCAL_BT_CONN_ID, 
                                  inst->privateGattId, 
                                  CsrBtGattSdsRestoreHandler,
                                  NULL,
                                  NULL));    
}

/* Covers Server Initiated Notification and Indication  */
static CsrBool csrBtGattAttHandleValueIndSecurityHandler(void            *gattInst, 
                                                         void            *qElem,
                                                         CsrBtResultCode result, 
                                                         CsrBtSupplier   supplier)
{ /* GATT has sent a ATT_READ_BLOB_REQ after receiving a ATT_HANDLE_VALUE_IND 
     with a long attribute. While reading the long attribute the peer server 
     has requested security */ 
    CSR_UNUSED(gattInst);

    if (result   == CSR_BT_RESULT_CODE_SC_SUCCESS && 
        supplier == CSR_BT_SUPPLIER_SC)
    { /* Security is set up with success. Continue the 
         Read procedure */
        CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;

        attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE, 
                             CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId), 
                             element->attrHandle, 
                             element->dataOffset, 
                             NULL);
        element->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
        return TRUE;
    }
    return FALSE;
}

static void csrBtGattEventSendRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{ /* This message are normally use by a Server application 
     to send a notification or an indication to the Client.
     However this message is also use intern by GATT to trigger 
     a ATT_READ_BLOB_REQ message if a ATT_HANDLE_VALUE_IND
     message is received with a long attribute. If the 
     message is use as a intern GATT message the 
     prim->gattId == inst->privateGattId */
    GattMainInst          *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement*)qElem;
    CsrBtGattEventSendReq *prim    = (CsrBtGattEventSendReq*)element->gattMsg;

    if (prim->gattId == inst->privateGattId)
    { /* This message is sent by GATT itself after
         receiving a ATT_HANDLE_VALUE_IND with a  
         long attribute. Read Blob if the link 
         is still connected. E.g prim->flags == 
         CSR_BT_GATT_LONG_ATTRIBUTE_EVENT */
        CsrBtGattConnElement *conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst, 
                                                                                     &prim->btConnId);

        if(conn)
        { /* Note, in order to be able to handle that the server requires security 
             on this read blob request GATT shall set element->securityFunc */
            element->securityFunc = csrBtGattAttHandleValueIndSecurityHandler;
            element->data         = (CsrUint8*)CsrPmemZalloc(CSR_BT_ATT_MTU_MAX);
            element->dataOffset   = prim->valueLength;
            element->attrHandle   = prim->attrHandle;
            CsrMemCpy(element->data, prim->value, prim->valueLength);
            attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE, 
                                 CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId), 
                                 element->attrHandle, 
                                 element->dataOffset, 
                                 NULL);
        }
        else
        {
            /* This procedure has finished. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, element);
        }
    }
    else
    { /* This message is sent by a Server application in order  
         to send a notification or an indication to the Client. */
        if (mtu > 0)
        {
            if (prim->flags == CSR_BT_GATT_SERVICE_CHANGED_EVENT)
            { /* A Service Changed event shall be sent. Need to
                 build the payload as GATT is the owner of
                 Service Change */
                prim->flags       = CSR_BT_GATT_INDICATION_EVENT;
                prim->valueLength = CSR_BT_GATT_SERVICE_CHANGED_LENGTH;
                prim->value       = (CsrUint8 *)CsrPmemAlloc(prim->valueLength);
                CSR_COPY_UINT16_TO_LITTLE_ENDIAN(prim->attrHandle, prim->value);
                CSR_COPY_UINT16_TO_LITTLE_ENDIAN(prim->endGroupHandle, &(prim->value[2]));
                /* Now set prim->attrHandle to CSR_BT_GATT_ATTR_HANDLE_SERVICE_CHANGED */
                prim->attrHandle  = CSR_BT_GATT_ATTR_HANDLE_SERVICE_CHANGED;
            }

            /* Else - Called CsrBtGattNotificationEventReqSend or 
               CsrBtGattIndicationEventReqSend no need to do
               any manipulation */  
            attlib_handle_value_req(CSR_BT_GATT_IFACEQUEUE, 
                                    CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                    prim->attrHandle,
                                    prim->flags,
                                    prim->valueLength,
                                    prim->value,
                                    NULL);

            /* Can not fail due to security */
            prim->value = NULL;
        }
        else
        { /* No connection, reply to app and restore queue */
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_EVENT_SEND_CFM,
                                        prim->gattId,
                                        CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                        CSR_BT_SUPPLIER_GATT,
                                        prim->btConnId);
            /* This procedure has finished. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, element);
        }
    }
}

CsrBool CsrBtGattEventSendReqHandler(GattMainInst *inst)
{ 
    CsrBtGattEventSendReq *prim = (CsrBtGattEventSendReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattEventSendRestoreHandler,
                                   NULL,
                                   NULL));
}

/* Covers item 1, Server Configuration, in the GATT feature table */
static void csrBtGattExchangeMtuRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst            *inst       = (GattMainInst*)gattInst;
    CsrBtGattQueueElement   *element    = (CsrBtGattQueueElement *)qElem;
    CsrBtGattExchangeMtuReq *prim       = (CsrBtGattExchangeMtuReq*)element->gattMsg;

    if (mtu > 0)
    { 
        attlib_exchange_mtu_req(CSR_BT_GATT_IFACEQUEUE, 
                                CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId), 
                                                                   prim->mtu, 
                                                                   NULL);
    }
    else
    {
        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

void CsrBtGattExchangeMtuHandler(GattMainInst   *inst, 
                                 CsrUint16      mtu, 
                                 CsrUint16      cid, 
                                 CsrBtTypedAddr address)
{
    /* An LE-only connection has just been created, and the Client
     * application has requested a MTU different from
     * CSR_BT_ATT_MTU_DEFAULT. Start the Server Configuration
     * Procedure as defined in BLUETOOTH SPECIFICATION Version 4.0 Vol
     * 3 Part G section 4.3.1 - Exchange MTU */    
    CsrBtGattExchangeMtuReq *prim;
    prim = (CsrBtGattExchangeMtuReq*)CsrPmemAlloc(sizeof(CsrBtGattExchangeMtuReq));
    prim->type     = CSR_BT_GATT_EXCHANGE_MTU_REQ;
    prim->btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(cid);
    prim->mtu      = mtu;
    prim->address  = address;      
    attlib_free((ATT_UPRIM_T *)inst->msg);
    inst->msg      = prim;
    (void)(csrBtGattNewReqHandler(inst, 
                                  prim->btConnId, 
                                  inst->privateGattId, 
                                  csrBtGattExchangeMtuRestoreHandler,
                                  NULL,
                                  NULL));
}

/* Covers item 2, Primary Service Discovery, in the GATT feature table  */
static void csrBtGattDiscoverServicesCancelHandler(void *gattInst, void *qElem,
                                                   CsrBtResultCode result, CsrBtSupplier supplier)
{
    GattMainInst *inst = (GattMainInst*) gattInst;

    if (inst->sdpBtConnId == CSR_BT_CONN_ID_INVALID)
    { /* GATT is using ATT to Discover Primary Services. E.g
         the LE-Radio is being uses */
        csrBtGattStdCancelHandler(gattInst, 
                                  qElem, 
                                  CSR_BT_GATT_DISCOVER_SERVICES_CFM,
                                  result, 
                                  supplier);
    }
    else
    { /* GATT is using SDP to Discover Primary Services. E.g
         the BR/EDR-Radio is being uses */
        CsrBtUtilSdcSearchCancel(inst, inst->sdpSearchData);
    }
}

static void csrBtGattDiscoverServicesRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst                 *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement        *element = (CsrBtGattQueueElement *)qElem;
    CsrBtGattDiscoverServicesReq *prim    = (CsrBtGattDiscoverServicesReq*)element->gattMsg;

    if (mtu > 0)
    {
        CsrBtGattConnInfo connInfo = CSR_BT_GATT_CONNINFO_LE;
        CsrBtGattConnElement *conn = NULL; 
        /* According to The BLUETOOTH SPECIFICATION Version 4.0 
           Vol 3 Part G section 4.4, GATT shall Discover the 
           Services with ATT if it uses a LE Radio and with 
           SDP it uses a BR/EDR radio */
        if (prim->btConnId != CSR_BT_GATT_LOCAL_BT_CONN_ID)
        {
            conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst, 
                                                                   &prim->btConnId);
            if (conn)
            {
                connInfo = CSR_BT_GATT_GET_CONNINFO(conn);
            }
        }
        
        if (connInfo == CSR_BT_GATT_CONNINFO_LE)
        { /* Using the LE Radio, use ATT to Discover Primary services */
            if (prim->uuid.length == CSR_BT_UUID16_SIZE &&
                CSR_BT_UUID_GET_16(prim->uuid) == CSR_BT_GATT_PRIMARY_SERVICE_UUID)
            { /* Discover all primary services */
                CsrUint32 primaryService[4] = {0x00002800, 0, 0, 0};

                attlib_read_by_group_type_req(CSR_BT_GATT_IFACEQUEUE,
                                              CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                              CSR_BT_GATT_ATTR_HANDLE_START,
                                              CSR_BT_GATT_ATTR_HANDLE_MAX,
                                              ATT_UUID16,
                                              primaryService,
                                              NULL);
            }
            else
            { /* Discover primary services by using a service UUID */
                CsrUint8 *value = (CsrUint8*)CsrPmemAlloc(prim->uuid.length);
                CsrMemCpy(value, prim->uuid.uuid, prim->uuid.length);

                attlib_find_by_type_value_req(CSR_BT_GATT_IFACEQUEUE,
                                              CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                              CSR_BT_GATT_ATTR_HANDLE_START,
                                              CSR_BT_GATT_ATTR_HANDLE_MAX,
                                              CSR_BT_GATT_PRIMARY_SERVICE_UUID,
                                              prim->uuid.length,
                                              value,
                                              NULL);
            }
        }
        else
        { /* Using the BR/EDR Radio, use SDP Discover Primary services */
            CsrUint16 index;
            CmnCsrBtLinkedListStruct *sdpTag = NULL;

            /* Full service search for ATT UUID. */
            sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, 
                                                                    CSR_BT_ATT_PROTOCOL_UUID16_IDENTIFIER, 
                                                                    &index);
            
            /* Try to get Service Class ID List Attribute */
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, 
                                                 index, 
                                                 CSR_BT_SERVICE_CLASS_ID_LIST, 
                                                 NULL, 
                                                 0);
            /* Try to get Protocol Descriptor List Attribute */
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, 
                                                 index, 
                                                 CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, 
                                                 NULL, 
                                                 0);
            
            CsrBtUtilSdrInsertPerformExtendedSearch(sdpTag, index);

            /* Start the SDP search procedure */
            inst->sdpBtConnId  = prim->btConnId;     
            CsrBtUtilSdcSearchStart(inst, 
                                    inst->sdpSearchData,
                                    sdpTag, 
                                    conn->peerAddr.addr);    
        }
    }
    else
    {
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_SERVICES_CFM,
                                    prim->gattId,
                                    CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                    CSR_BT_SUPPLIER_GATT,
                                    prim->btConnId);

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattDiscoverServicesReqHandler(GattMainInst *inst)
{
    CsrBtGattDiscoverServicesReq *prim = (CsrBtGattDiscoverServicesReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattDiscoverServicesRestoreHandler,
                                   csrBtGattDiscoverServicesCancelHandler,
                                   NULL));
}

/* Covers item 3, Relationship Discovery, in the GATT feature table */
static void csrBtGattFindInclServicesCancelHandler(void *gattInst, void *qElem,
                                                   CsrBtResultCode result, CsrBtSupplier supplier)
{
    csrBtGattStdCancelHandler(gattInst, qElem, CSR_BT_GATT_FIND_INCL_SERVICES_CFM,
                              result, supplier);
}

static void csrBtGattFindInclServicesRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst                 *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement        *element = (CsrBtGattQueueElement *)qElem;
    CsrBtGattFindInclServicesReq *prim = (CsrBtGattFindInclServicesReq*)element->gattMsg;

    if (mtu > 0)
    {
        CsrUint32 inclUuid[4] = {0x00002802, 0, 0, 0};

        attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                prim->startHandle,
                                prim->endGroupHandle,
                                ATT_UUID16,
                                &inclUuid[0],
                                NULL);
    }
    else
    {
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_FIND_INCL_SERVICES_IND,
                                    prim->gattId,
                                    CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                    CSR_BT_SUPPLIER_GATT,
                                    prim->btConnId);

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattFindInclServicesReqHandler(GattMainInst *inst)
{
    CsrBtGattFindInclServicesReq *prim = (CsrBtGattFindInclServicesReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattFindInclServicesRestoreHandler,
                                   csrBtGattFindInclServicesCancelHandler,
                                   NULL));
}

/* Covers item 4, Characteristic Discovery, in the GATT feature table */
static void csrBtGattDiscoverCharacCancelHandler(void            *gattInst, 
                                                 void            *qElem,
                                                 CsrBtResultCode result, 
                                                 CsrBtSupplier   supplier)
{
    csrBtGattStdCancelHandler(gattInst, 
                              qElem, 
                              CSR_BT_GATT_DISCOVER_CHARAC_CFM,
                              result, 
                              supplier);
}

static void csrBtGattDiscoverCharacRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst               *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement      *element = (CsrBtGattQueueElement *)qElem;
    CsrBtGattDiscoverCharacReq *prim    = (CsrBtGattDiscoverCharacReq*)element->gattMsg;

    if (mtu > 0)
    {
        CsrUint32 characUuid[4] = {0x00002803, 0, 0, 0};
        attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                prim->startHandle,
                                prim->endGroupHandle,
                                ATT_UUID16,
                                characUuid,
                                NULL);
    }
    else
    {
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_CHARAC_CFM,
                                    prim->gattId,
                                    CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                    CSR_BT_SUPPLIER_GATT,
                                    prim->btConnId);

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattDiscoverCharacReqHandler(GattMainInst *inst)
{
    CsrBtGattDiscoverCharacReq *prim = (CsrBtGattDiscoverCharacReq*)inst->msg;

    return(csrBtGattNewReqHandler(inst, 
                                  prim->btConnId, 
                                  prim->gattId,
                                  csrBtGattDiscoverCharacRestoreHandler,
                                  csrBtGattDiscoverCharacCancelHandler,
                                  NULL));
}

/* Covers item 5, Characteristic Descriptor Discovery, in the GATT feature table */
static void csrBtGattDiscoverCharacDescriptorsCancelHandler(void            *gattInst, 
                                                            void            *qElem,
                                                            CsrBtResultCode result, 
                                                            CsrBtSupplier   supplier)
{
    csrBtGattStdCancelHandler(gattInst, 
                              qElem, 
                              CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM,
                              result, 
                              supplier);
}

static void csrBtGattDiscoverCharacDescriptorsRestoreHandler(void      *gattInst, 
                                                             void      *qElem, 
                                                             CsrUint16 mtu)
{
    GattMainInst               *inst            = (GattMainInst*)gattInst;
    CsrBtGattQueueElement      *element         = (CsrBtGattQueueElement *)qElem;
    CsrBtGattDiscoverCharacDescriptorsReq *prim = (CsrBtGattDiscoverCharacDescriptorsReq *)
        element->gattMsg;

    if (mtu > 0)
    {
        attlib_find_info_req(CSR_BT_GATT_IFACEQUEUE,
                             CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                             prim->startHandle,
                             prim->endGroupHandle,
                             NULL);
    }
    else
    {
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM,
                                    prim->gattId,
                                    CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                    CSR_BT_SUPPLIER_GATT,
                                    prim->btConnId);

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattDiscoverCharacDescriptorsReqHandler(GattMainInst *inst)
{
    CsrBtGattDiscoverCharacDescriptorsReq *prim = (CsrBtGattDiscoverCharacDescriptorsReq*)inst->msg;

    return(csrBtGattNewReqHandler(inst, 
                                  prim->btConnId, 
                                  prim->gattId,
                                  csrBtGattDiscoverCharacDescriptorsRestoreHandler,
                                  csrBtGattDiscoverCharacDescriptorsCancelHandler,
                                  NULL));

}

/* Covers item 6, Characteristic Value Read, and item 10, Read Characteristic Descriptor Value, 
   in the GATT feature table*/
static CsrBool csrBtGattReadSecurityHandler(void            *gattInst, 
                                            void            *qElem,
                                            CsrBtResultCode result, 
                                            CsrBtSupplier   supplier)
{
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    CSR_UNUSED(gattInst);

    if (element->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    { /* The CsrBtGattReadReq procedure is still active */
        if (result   == CSR_BT_RESULT_CODE_SC_SUCCESS && 
            supplier == CSR_BT_SUPPLIER_SC)
        { /* Security is set up with success. Continue the 
             Read procedure */
            CsrBtGattReadReq *reqMsg = (CsrBtGattReadReq *) element->gattMsg;
            CsrUint16 offset         = reqMsg->offset;
            
            if ((element->dataOffset + offset) == 0)
            {
                attlib_read_req(CSR_BT_GATT_IFACEQUEUE,
                                CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId),
                                element->attrHandle,
                                NULL);
            }
            else
            {
                attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE,
                                     CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId),
                                     element->attrHandle,
                                     (CsrUint16)(element->dataOffset + offset),
                                     NULL);
            }
            element->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
            return TRUE;
        }
        else
        { /* Failed to setup security */
            CsrUint8 *tmp = NULL;
            CsrBtGattReadCfmHandler((CsrBtGattReadReq*) element->gattMsg,
                                    result,
                                    supplier,
                                    0,
                                    &tmp);
        }
    }
    /* Else - The CsrBtGattReadReq procedure has been cancelled while 
              running the security procedure. Note in this case
              the application has already received the confirm 
              message from the  function csrBtGattReadCancelHandler. */
    return FALSE;
}

static void csrBtGattReadCancelHandler(void *gattInst, void *qElem,
                                       CsrBtResultCode result, CsrBtSupplier supplier)
{
    GattMainInst          *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;

    if (result == CSR_BT_GATT_RESULT_CANCELLED)
    {
        CsrUint8 *tmp = NULL;
        CsrBtGattReadCfmHandler((CsrBtGattReadReq*) element->gattMsg,
                                result,
                                supplier,
                                0,
                                &tmp);
        
        /* Set element->restoreFunc to NULL in order to make sure that 
           the application do not received the confirm message twice in case 
           of an ATT_DISCONNECT_IND */
        element->restoreFunc = NULL;
    }
    /* Else - GATT has Cancel the Procedure as part of the 
              unregister procedure */

    /* Check what to do with the pending message */
    csrBtGattStdCancelElementHandler(inst, element); 
}

static void csrBtGattReadRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst          *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *)qElem;
    CsrBtGattReadReq      *prim    = (CsrBtGattReadReq*) element->gattMsg;

    if (mtu > 0)
    {
        element->attrHandle = prim->handle;

        if (prim->offset == 0)
        {
            attlib_read_req(CSR_BT_GATT_IFACEQUEUE,
                            CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                            prim->handle,
                            NULL);
        }
        else
        {
            element->data       = (CsrUint8 *) CsrPmemZalloc(CSR_BT_ATT_MTU_MAX);
            attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE, 
                                 CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId), 
                                 prim->handle, 
                                 prim->offset, 
                                 NULL);
        }
    }
    else
    {
        CsrUint8 *tmp = NULL;
        CsrBtGattReadCfmHandler(prim,
                                CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                CSR_BT_SUPPLIER_GATT,
                                0,
                                &tmp);

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattReadReqHandler(GattMainInst *inst)
{
    CsrBtGattReadReq *prim = (CsrBtGattReadReq*)inst->msg;
    return(csrBtGattNewReqHandler(inst, 
                                  prim->btConnId, 
                                  prim->gattId,
                                  csrBtGattReadRestoreHandler,
                                  csrBtGattReadCancelHandler,
                                  csrBtGattReadSecurityHandler));
}

CsrBool CsrBtGattReadByUuidSecurityHandler(void            *gattInst,
                                           void            *qElem,
                                           CsrBtResultCode result, 
                                           CsrBtSupplier   supplier)
{
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    CSR_UNUSED(gattInst);

    if (element->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    { /* The CsrBtGattReadByUuidReq procedure is still active */
        if (result   == CSR_BT_RESULT_CODE_SC_SUCCESS && 
            supplier == CSR_BT_SUPPLIER_SC)
        { /* Security is set up with success while reading by type. 
             Continue the Read By Uuid procedure */
            CsrUint32       attUuid[4];
            att_uuid_type_t uuidType;
            CsrBtGattReadByUuidReq *reqMsg = (CsrBtGattReadByUuidReq *) element->gattMsg;
            
            CsrBtGattGetAttUuid(reqMsg->uuid, attUuid, &uuidType);
        
            attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                    CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId),
                                    element->attrHandle,
                                    reqMsg->endGroupHandle,
                                    uuidType,
                                    attUuid,
                                    NULL);
            
            element->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
            return TRUE;
        }
        else
        { /* Failed to setup security */
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_READ_BY_UUID_CFM,
                                        element->gattId,
                                        result,
                                        supplier,
                                        element->btConnId);
        }
    }
    /* Else - The CsrBtGattReadByUuidReq procedure has been cancelled 
              while running the security procedure. Note in this case
              the application has already received the confirm message 
              from the function csrBtGattReadByUuidCancelHandler. */
    return FALSE;
}

static void csrBtGattReadByUuidCancelHandler(void *gattInst, void *qElem,
                                             CsrBtResultCode result, CsrBtSupplier supplier)
{
    csrBtGattStdCancelHandler(gattInst, qElem, CSR_BT_GATT_READ_BY_UUID_CFM,
                              result, supplier);
}

static void csrBtGattReadByUuidRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst           *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement  *element = (CsrBtGattQueueElement *)qElem;
    CsrBtGattReadByUuidReq *prim    = (CsrBtGattReadByUuidReq*)element->gattMsg;

    if (mtu > 0)
    {
        CsrUint32              attUuid[4];
        att_uuid_type_t        uuidType;
        CsrBtGattGetAttUuid(prim->uuid, attUuid, &uuidType);
        /* element->attrHandle is set prim->startHandle so GATT is 
           able to replay this command in case of security failure */
        element->attrHandle = prim->startHandle;
        attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                prim->startHandle,
                                prim->endGroupHandle,
                                uuidType,
                                attUuid,
                                NULL);
    }
    else
    {
        if (prim->gattId != inst->privateGattId)
        {
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_READ_BY_UUID_CFM,
                                        prim->gattId,
                                        CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                        CSR_BT_SUPPLIER_GATT,
                                        prim->btConnId);
        }
        /* Else - GATT started this procedure in order to find 
           the handle of the Service Changed Characteristic Value */

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

void CsrBtGattFindServiceChangedHandler(GattMainInst *inst, CsrUint16 cid, CsrBtTypedAddr address)
{ /* This function check if the handle of the Service Changed Characteristic Value
     is known or not */
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &inst->privateGattId);
    
    if (appElement)
    {
        if (!CSR_BT_GATT_SUBSCRIBE_INST_FIND_SERVICE_CHANGED(appElement, &address))
        { /* GATT needs to find the handle of the Service Changed Characteristic 
             Value */
            CsrBtUuid uuid;
            CsrMemSet(uuid.uuid, 0, CSR_BT_UUID128_SIZE);
            uuid.length = (CsrUint16) (sizeof(CsrBtUuid16));
            CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_GATT_SERVICE_CHANGED_UUID,uuid.uuid);
        
            if (inst->msg)
            {
                attlib_free((ATT_UPRIM_T *)inst->msg);
            }
        
            inst->msg = CsrBtGattReadByUuidReq_struct(inst->privateGattId,
                                                      CSR_BT_GATT_CREATE_LE_CONN_ID(cid),
                                                      CSR_BT_GATT_ATTR_HANDLE_START,
                                                      CSR_BT_GATT_ATTR_HANDLE_MAX,
                                                      uuid);
        
            /* The reason why GATT don't call CsrBtGattReadByUuidReqHandler
               direct is that GATT want to make sure that this procedure cannot 
               be cancel by an application and that the security procedure is 
               not started */
            (void)(csrBtGattNewReqHandler(inst, 
                                          CSR_BT_GATT_CREATE_LE_CONN_ID(cid), 
                                          inst->privateGattId, 
                                          csrBtGattReadByUuidRestoreHandler,
                                          NULL,
                                          NULL));
        }
        /* Else - GATT have already found the handle of the Service Changed 
           Characteristic Value */
    }
    else
    {
        CsrStateEventException("CsrBtGattFindServiceChangedHandler no appElement",
                                    CSR_BT_GATT_PRIM, 0, 0);
    }
}

void CsrBtGattReadRemoteLeNameHandler(GattMainInst *inst, CsrUint16 cid, l2ca_conflags_t flags)
{
    if ((L2CA_CONFLAG_IS_LE(flags)))
    { /* Using the LE radio. Read remote name using ATT and make sure that
         this message is read before SC can starts reading this message */
        CsrBtUuid uuid;
        CsrMemSet(uuid.uuid, 0, CSR_BT_UUID128_SIZE);
        uuid.length = (CsrUint16) (sizeof(CsrBtUuid16));
        CSR_COPY_UINT16_TO_LITTLE_ENDIAN(CSR_BT_GATT_DEVICE_NAME_UUID,uuid.uuid);
        
        if (inst->msg)
        { 
            attlib_free((ATT_UPRIM_T *)inst->msg);
        }

        inst->msg = CsrBtGattReadByUuidReq_struct(inst->privateGattId,
                                                  CSR_BT_GATT_CREATE_LE_CONN_ID(cid),
                                                  CSR_BT_GATT_ATTR_HANDLE_START,
                                                  CSR_BT_GATT_ATTR_HANDLE_MAX,
                                                  uuid);

        /* The reason why GATT don't call CsrBtGattReadByUuidReqHandler
           direct is that GATT want to make sure that this procedure cannot 
           be cancel by an application and that the security procedure is 
           not started */
        (void)(csrBtGattNewReqHandler(inst, 
                                      CSR_BT_GATT_CREATE_LE_CONN_ID(cid), 
                                      inst->privateGattId, 
                                      csrBtGattReadByUuidRestoreHandler,
                                      NULL,
                                      NULL));
    }
}

CsrBool CsrBtGattReadByUuidReqHandler(GattMainInst *inst)
{
    CsrBtGattReadByUuidReq *prim = (CsrBtGattReadByUuidReq*)inst->msg;
    
    if (CSR_BT_GATT_GET_QID_FROM_GATT_ID(prim->gattId) == CSR_BT_SC_IFACEQUEUE &&
        CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &prim->btConnId))
    { /* The SC is trying to read the remote le name while another process is running.
         As the running process may start the Security procedure and this may 
         block the queue because GATT is waiting SC and SC waiting for GATT, GATT
         will return the saved remote name direct to SC */
        CsrBtResultCode resultCode = CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID;
        CsrBtGattConnElement *conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                                                     &prim->btConnId);
        if (conn)
        {
            if (conn->remoteName)
            { /* Gatt has already read the name */
                CsrUint8 *value = (CsrUint8 *) CsrPmemAlloc(conn->remoteNameLength);
                resultCode      = CSR_BT_GATT_RESULT_SUCCESS;
                CsrMemCpy(value, conn->remoteName, conn->remoteNameLength);
                CsrBtGattReadByUuidIndSend(prim->gattId,
                                           prim->btConnId,
                                           0xFFFF,
                                           conn->remoteNameLength,
                                           value);               
            }
            else
            { /* Check if GATT is reading the name or it failed to read it */
                CsrBtGattQueueElement *qElem = NULL;
                CSR_BT_GATT_QUEUE_FIND_PRIVATE_READ_NAME_MSG(inst->queue, 
                                                             &qElem, 
                                                             inst->privateGattId, 
                                                             prim->btConnId);
                if (qElem && qElem->dataElemIndex == 0)
                { /* This message is received before GATT had time to read the name.
                     qElem->dataElemIndex to SC ID to mark that GATT shall sent a 
                     CsrBtGattReadByUuidInd, CsrBtGattReadByUuidCfm message
                     to SC as soon as it has read the name. Note the qElem->dataElemIndex
                     is not used by a normal CsrBtGattReadByUuidReq procedure, so
                     GATT is using this parameter to mark if that this request
                     has been issue by SC. It is mark in a way that GATT easy can 
                     find SC gattId again */ 
                    qElem->dataElemIndex = (CsrUint16) ((prim->gattId & 0xFFFF0000) >> 16);
                    return TRUE;
                }
                else
                { /* GATT either failed to read the name or SC tries to read it twice. */
                    resultCode = CSR_BT_GATT_RESULT_INTERNAL_ERROR;
                }
            }
        }
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_READ_BY_UUID_CFM,
                                    prim->gattId,
                                    resultCode,
                                    CSR_BT_SUPPLIER_GATT,
                                    prim->btConnId);
        return TRUE;
    }
    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattReadByUuidRestoreHandler,
                                   csrBtGattReadByUuidCancelHandler,
                                   CsrBtGattReadByUuidSecurityHandler));
}

static CsrBool csrBtGattReadMultiSecurityHandler(void            *gattInst, 
                                                 void            *qElem,
                                                 CsrBtResultCode result, 
                                                 CsrBtSupplier   supplier)
{
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    CSR_UNUSED(gattInst);

    if (element->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    { /* The CsrBtGattReadMultiReq procedure is still active */
        if (result   == CSR_BT_RESULT_CODE_SC_SUCCESS && 
            supplier == CSR_BT_SUPPLIER_SC)
        { /* Security is set up with success. Continue the 
             Read Multi procedure */
            CsrBtGattReadMultiReq *reqMsg = (CsrBtGattReadMultiReq *) element->gattMsg;
            
            attlib_read_multi_req(CSR_BT_GATT_IFACEQUEUE,
                                  CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(reqMsg->btConnId),
                                  reqMsg->handlesCount,
                                  reqMsg->handles,
                                  NULL);
            /* Note: prim->handles are not copied as in csrBtGattReadMultiRestoreHandler
               because GATT only can replay this command once in case of security failure */
            reqMsg->handles   = NULL;  
            element->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
            return TRUE;
        }
        else
        { /* Failed to setup security */
            CsrBtGattStdReadCfmSend(CSR_BT_GATT_READ_MULTI_CFM,
                                    element->gattId,
                                    result,
                                    supplier,
                                    element->btConnId,
                                    0,
                                    NULL);    
        }
    }
    /* Else - The CsrBtGattReadMultiReq procedure has been cancelled 
              while running the security procedure. Note in this case
              the application has already received the confirm 
              message from the  function csrBtGattReadCancelHandler. */
    return FALSE;
}

static void csrBtGattReadMultiCancelHandler(void            *gattInst, 
                                            void            *qElem,
                                            CsrBtResultCode result, 
                                            CsrBtSupplier   supplier)
{
    GattMainInst          *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    
    if (result == CSR_BT_GATT_RESULT_CANCELLED)
    {
        CsrBtGattStdReadCfmSend(CSR_BT_GATT_READ_MULTI_CFM,
                                element->gattId,
                                result,
                                supplier,
                                element->btConnId,
                                0,
                                NULL);

        /* Set element->restoreFunc to NULL in order to make sure that 
           the application do not received the confirm message twice in case 
           of an ATT_DISCONNECT_IND */
        element->restoreFunc = NULL;
    }
    /* Else - GATT has Cancel the Procedure as part of the 
              unregister procedure */

    /* Check what to do with the pending message */
    csrBtGattStdCancelElementHandler(inst, element); 
}

static void csrBtGattReadMultiRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    GattMainInst          *inst    = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *)qElem;
    CsrBtGattReadMultiReq *prim    = (CsrBtGattReadMultiReq*)element->gattMsg;

    if (mtu > 0)
    {
        attlib_read_multi_req(CSR_BT_GATT_IFACEQUEUE,
                              CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                              prim->handlesCount,
                              CsrMemDup(prim->handles, prim->handlesCount * sizeof(CsrBtGattHandle)),
                              NULL);
        /* Note: prim->handles copied so we can replay this command in
         * case of security failure */
    }
    else
    {
        CsrBtGattStdReadCfmSend(CSR_BT_GATT_READ_MULTI_CFM,
                                prim->gattId,
                                CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID,
                                CSR_BT_SUPPLIER_GATT,
                                prim->btConnId,
                                0,
                                NULL);

        /* This procedure has finished. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, element);
    }
}

CsrBool CsrBtGattReadMultiReqHandler(GattMainInst *inst)
{
    CsrBtGattReadMultiReq *prim = (CsrBtGattReadMultiReq*)inst->msg;
    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattReadMultiRestoreHandler,
                                   csrBtGattReadMultiCancelHandler,
                                   csrBtGattReadMultiSecurityHandler));

}

/* Covers item 7, Characteristic Value Write, in the GATT feature table */
void CsrBtGattGetAttPrepareWriteSend(CsrBtGattQueueElement *qElem,
                                     CsrUint16             prepareWriteOffset,
                                     CsrUint16             mtu,
                                     CsrUint16             valueLength,
                                     CsrUint8              *value)
{
    CsrUint8  *prepareValue;
    CsrUint16 prepareValueLength;

    if ((valueLength - qElem->dataOffset) > (mtu - CSR_BT_GATT_ATT_PREPARE_WRITE_HEADER_LENGTH))
    { /* Need to fragment the attribute value */ 
        prepareValueLength = (mtu - CSR_BT_GATT_ATT_PREPARE_WRITE_HEADER_LENGTH);
    }
    else
    { /* Send the last fragment or the entire attribute value */ 
        prepareValueLength = (valueLength - qElem->dataOffset);
    }

    prepareValue = (CsrUint8*)CsrPmemAlloc(prepareValueLength);
    CsrMemCpy(prepareValue, &value[qElem->dataOffset], prepareValueLength);

    attlib_prepare_write_req(CSR_BT_GATT_IFACEQUEUE,
                             CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(qElem->btConnId),
                             qElem->attrHandle,
                             prepareWriteOffset,
                             prepareValueLength,
                             prepareValue,
                             NULL);    
}

static void csrBtGattWriteRequestCancelHandler(void            *gattInst, 
                                               void            *qElem,
                                               CsrBtResultCode result, 
                                               CsrBtSupplier   supplier)
{
    GattMainInst          *inst    = (GattMainInst*) gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;

    if (result == CSR_BT_GATT_RESULT_CANCELLED && 
        element->msgState == CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_SECURITY)
    { /* The Write request procedure is cancelled while security 
         is pending. */
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                    element->gattId,
                                    result,
                                    supplier,
                                    element->btConnId);

        /* Set element->restoreFunc to NULL in order to make sure that 
           the application do not received the confirm message twice in case 
           of an ATT_DISCONNECT_IND */
        element->restoreFunc = NULL;
    }
    /* Else - GATT has Cancel the Procedure as part of the 
              unregister procedure or while a ATT_WRITE_REQ
              is pending.  */

    /* Check what to do with the pending message */
    csrBtGattStdCancelElementHandler(inst, element); 
}

static void csrBtGattWriteCancelHandler(void            *gattInst, 
                                        void            *qElem,
                                        CsrBtResultCode result, 
                                        CsrBtSupplier   supplier)
{ /* GATT is using this callback function to cancel the
     ATT_PREPARE_WRITE_REQ sub procedure. GATT also uses this
     callback function if the write procedure is cancel before
     it even has started to interact with ATT.
     After receiving a CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ
     or a CSR_BT_GATT_WRITE_REQ message (which do not uses a
     ATT_PREPARE_WRITE_REQ) the cancel callback function is either 
     set to NULL or to csrBtGattWriteRequestCancelHandler */
    GattMainInst          *inst    = (GattMainInst*) gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;

    if (result == CSR_BT_GATT_RESULT_CANCELLED)
    {
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                    element->gattId,
                                    result,
                                    supplier,
                                    element->btConnId);
        /* Set element->restoreFunc to NULL in order to make sure that 
           the application do not received the confirm message twice in case 
           of an ATT_DISCONNECT_IND */
        element->restoreFunc = NULL;
    }
    /* Else - GATT has Cancel the Procedure as part of the 
              unregister procedure */
    csrBtGattStdCancelElementHandler(inst, element);
}

static CsrBool csrBtGattWriteSecurityHandler(CsrBtGattQueueElement *element,
                                             CsrBtResultCode       result, 
                                             CsrBtSupplier         supplier)
{
    if (element->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    { /* The CsrBtGattWriteReq or the 
         CsrBtGattWriteClientConfigurationReq procedure is still active */
        if (result   == CSR_BT_RESULT_CODE_SC_SUCCESS && 
            supplier == CSR_BT_SUPPLIER_SC)
        { /* Security is set up with success. Continue the 
             Write procedure */
            return TRUE;
        }
        else
        { /* Failed to setup security */
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                        element->gattId,
                                        result,
                                        supplier,
                                        element->btConnId);
        }
    }
    /* Else - The CsrBtGattWriteReq or the CsrBtGattWriteClientConfigurationReq 
              procedure has been cancel while running the security procedure. 
              Note in this case the application has already received the confirm 
              message from the  function csrBtGattWriteRequestCancelHandler. */
    return FALSE;
}

static CsrBool csrBtGattWriteRequestSecurityHandler(void            *gattInst, 
                                                    void            *qElem,
                                                    CsrBtResultCode result, 
                                                    CsrBtSupplier   supplier)
{
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    CSR_UNUSED(gattInst);

    if (csrBtGattWriteSecurityHandler(element, result, supplier))
    {
        attlib_write_req(CSR_BT_GATT_IFACEQUEUE,
                         CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId),
                         element->attrHandle,
                         CSR_BT_GATT_WRITE_REQUEST,
                         element->dataOffset,
                         element->data,
                         NULL);
        /* Note, element->data are not copied because the GATT shall not
           replay this command */
        element->data = NULL;
        element->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
        return TRUE;
    }
    return FALSE;
}

CsrBool CsrBtGattPrepareWriteSecurityHandler(void            *gattInst, 
                                             void            *qElem,
                                             CsrBtResultCode result, 
                                             CsrBtSupplier   supplier)
{
    
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;

    if (csrBtGattWriteSecurityHandler(element, result, supplier))
    {
        GattMainInst *inst    = (GattMainInst*) gattInst;
        CsrBtGattWriteReq *reqMsg = (CsrBtGattWriteReq *) element->gattMsg;
        CsrBtGattAttrWritePairs *attrPair = &(reqMsg->attrWritePairs[element->dataElemIndex]);
        CsrBtGattGetAttPrepareWriteSend(element,
                                        (CsrUint16)(attrPair->offset + element->dataOffset),
                                        CsrBtGattValidateBtConnIdByMtu(inst, element->gattId, element->btConnId),
                                        attrPair->valueLength,
                                        attrPair->value);

        element->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
        return TRUE;
    }

    if (element->dataElemIndex > 0 || element->dataOffset > 0)
    { /* At least one ATT_PREPARE_WRITE_CFM with success has been 
         received. Set qElem->msgState to Cancelled in order
         to make sure that CSR_BT_GATT_WRITE_CFM is not
         sent to the application twice and set qElem->cancelFunc 
         to NULL in order to make sure that this procedure cannot 
         be cancel twice */
        element->cancelFunc = NULL;
        element->msgState   = CSR_BT_GATT_MSG_QUEUE_CANCELLED;
        attlib_execute_write_req(CSR_BT_GATT_IFACEQUEUE,
                                 CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId),
                                 ATT_EXECUTE_CANCEL,
                                 NULL);
        return TRUE;
    }
    return FALSE;
}



static void csrBtGattWriteRestoreHandler(void *gattInst, void *qElem, CsrUint16 mtu)
{
    CsrBtResultCode         resultCode = CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID;
    GattMainInst            *inst      = (GattMainInst*)gattInst;
    CsrBtGattQueueElement   *element   = (CsrBtGattQueueElement *)qElem;
    CsrBtGattWriteReq       *prim      = (CsrBtGattWriteReq*)element->gattMsg;
    
    if (prim->btConnId == CSR_BT_GATT_LOCAL_BT_CONN_ID)
    { /* Make sure that the application only can change the value of an 
         attribute that it owns */
        CsrUint32 i;
        CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &prim->gattId);
        
        if (appElement)
        {
            for (i = 0; i < prim->attrWritePairsCount; i++)
            {

                if (prim->attrWritePairs[i].attrHandle < appElement->start ||
                    prim->attrWritePairs[i].attrHandle > appElement->end)
                { /* The application is trying to change the value of an 
                     attribute which it do not own. Return Error */
                    mtu = 0;
                    resultCode = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
                    break;
                }
            }
        }
        else
        {
            CsrStateEventException("CSR_BT_GATT_WRITE_REQ no appElement",
                                    CSR_BT_GATT_PRIM, prim->type, 0);
        }
    }

    if (mtu > 0)
    {
        CsrBtGattAttrWritePairs *attrPair = &(prim->attrWritePairs[0]);
        element->attrHandle = attrPair->attrHandle;

        switch (prim->flags)
        {
            case CSR_BT_GATT_WRITE_REQUEST:
            { 
                /* attrPair->value is copied so GATT can replay this in case of 
                   security failure. In case of a prepare write this is done by 
                   CsrBtGattGetAttPrepareWriteSend. */
                if (attrPair->offset > 0 ||
                    attrPair->valueLength > (mtu - CSR_BT_GATT_ATT_WRITE_HEADER_LENGTH))
                { /* Must use ATT_PREPARE_WRITE_REQ. Note, GATT do not need 
                     to set element->cancelFunc as it is already set to 
                     csrBtGattWriteCancelHandler */
                    element->securityFunc = CsrBtGattPrepareWriteSecurityHandler;
                    CsrBtGattGetAttPrepareWriteSend(element,
                                                    attrPair->offset,
                                                    mtu, 
                                                    attrPair->valueLength, 
                                                    attrPair->value);
                }
                else
                { /* Note element->cancelFunc is set to 
                     csrBtGattWriteRequestCancelHandler 
                     because a Write Request can only be cancel while 
                     setting up security. */
                    element->cancelFunc   = csrBtGattWriteRequestCancelHandler;
                    element->securityFunc = csrBtGattWriteRequestSecurityHandler;
                    element->dataOffset   = attrPair->valueLength;
                    element->data         = attrPair->value;
                    element->attrHandle   = attrPair->attrHandle;
                    attrPair->value       = NULL;
                    attlib_write_req(CSR_BT_GATT_IFACEQUEUE,
                                     CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                     attrPair->attrHandle,
                                     prim->flags,
                                     attrPair->valueLength,
                                     CsrMemDup(element->data, element->dataOffset),
                                     NULL);
                }
                return;
            }
            case CSR_BT_GATT_WRITE_COMMAND:
            { 
                if (attrPair->valueLength > (mtu - CSR_BT_GATT_ATT_WRITE_HEADER_LENGTH))
                {
                    resultCode = CSR_BT_GATT_RESULT_INVALID_LENGTH;
                }
                else
                { /* Note: attrPair->value are not copied because the server shall not 
                     send any response so GATT shall never replay this command. That is
                     also why this command cannot be cancel. To prevent this 
                     element->cancelFunc is set to NULL */
                    attlib_write_req(CSR_BT_GATT_IFACEQUEUE,
                                     CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                     attrPair->attrHandle,
                                     prim->flags,
                                     attrPair->valueLength,
                                     attrPair->value,
                                     NULL);

                    attrPair->value     = NULL;
                    element->cancelFunc = NULL;
                    return;
                }
                break;
            }
            case CSR_BT_GATT_WRITE_SIGNED_COMMAND:
            {
                if (attrPair->valueLength > (mtu - CSR_BT_GATT_ATT_SIGNED_WRITE_HEADER_LENGTH))
                {
                    resultCode = CSR_BT_GATT_RESULT_INVALID_LENGTH;
                }
                else
                { /* Note: attrPair->value are not copied because the server shall not 
                     send any response so GATT shall never replay this command. That is
                     also why this command cannot be cancel. To prevent this 
                     element->cancelFunc is set to NULL */
                    attlib_write_req(CSR_BT_GATT_IFACEQUEUE,
                                     CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                     attrPair->attrHandle,
                                     prim->flags,
                                     attrPair->valueLength,
                                     attrPair->value,
                                     NULL);

                    attrPair->value     = NULL;
                    element->cancelFunc = NULL;
                    return;
                }
                break;
            }
            case CSR_BT_GATT_WRITE_RELIABLE:
            {
                /* attrPair->value is copied so GATT can replay
                   this in case of security failure. This is done by 
                   CsrBtGattGetAttPrepareWriteSend. Note, GATT do not 
                   need to set element->cancelFunc as it is already 
                   set to csrBtGattWriteCancelHandler */
                element->securityFunc = CsrBtGattPrepareWriteSecurityHandler;
                CsrBtGattGetAttPrepareWriteSend(element,
                                                attrPair->offset,
                                                mtu, 
                                                attrPair->valueLength, 
                                                attrPair->value);
                return;
            }
            default:
            {
                resultCode = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
                break;
            }
        }
    }

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                prim->gattId,
                                resultCode,
                                CSR_BT_SUPPLIER_GATT,
                                prim->btConnId);

    /* This procedure has finished. Start the next if any */
    CsrBtGattQueueRestoreHandler(inst, element);
}

CsrBool CsrBtGattWriteReqHandler(GattMainInst *inst)
{
    CsrBtGattWriteReq *prim = (CsrBtGattWriteReq*)inst->msg;
   
    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattWriteRestoreHandler,
                                   csrBtGattWriteCancelHandler,
                                   NULL));
}

/* Covers item 8 and 9, Characteristic Value Indication/Notification, in the GATT feature table */
static void csrBtGattFindClientConfigurationValue(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppElement *appElement       = (CsrBtGattAppElement*)elem;    
    CsrBtGattSubscribeElement *subElement = CSR_BT_GATT_SUBSCRIBE_INST_FIND_SUBSCRIBE_ID(appElement, value);

    if (subElement)
    { 
        CsrBtGattSubscribeFindIds *ids = (CsrBtGattSubscribeFindIds *) value;
        
        if (subElement->configuration <= CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION && 
            ids->configuration < subElement->configuration)
        {
            ids->configuration = subElement->configuration;
            *(CsrBtGattSubscribeFindIds*)value = *ids;
        }
    }
}

static CsrUint8 * csrBtGattReturnClientConfigurationValue(GattMainInst                          *inst,
                                                          CsrBtGattWriteClientConfigurationReq  *prim)
{
    CsrBtGattCliConfigBits config = prim->configuration;
    CsrUint8 *value               = (CsrUint8*)CsrPmemAlloc(sizeof(CsrBtGattCliConfigBits));
    
    if (config != CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION)
    {
        CsrBtGattSubscribeFindIds ids;

        ids.connInfo      = CSR_BT_GATT_CONNINFO_BREDR; /* not used */
        ids.address       = prim->address; 
        ids.attrHandle    = 0;
        ids.valueLength   = 0;
        ids.value         = NULL;
        ids.btConnId      = CSR_BT_GATT_LOCAL_BT_CONN_ID;
        ids.configuration = config; 
        ids.privateGattId = inst->privateGattId;
        CSR_BT_GATT_APP_INST_ITERATE(inst->appInst, csrBtGattFindClientConfigurationValue, &ids);
        config            = ids.configuration;    
    }
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(config, value);
    return (value);
}

static void csrBtGattWriteClientConfigurationRestoreHandler(void       *gattInst, 
                                                            void       *qElem,
                                                            CsrUint16  mtu)
{
    CsrBtResultCode       resultCode           = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
    GattMainInst          *inst                = (GattMainInst*)gattInst;
    CsrBtGattQueueElement *element             = (CsrBtGattQueueElement *)qElem;
    CsrBtGattWriteClientConfigurationReq *prim = (CsrBtGattWriteClientConfigurationReq*)element->gattMsg;
    
    if (prim->configuration <= CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION)
    {
        if (mtu > 0)
        {
            if (prim->btConnId == CSR_BT_GATT_LOCAL_BT_CONN_ID)
            { /* The application has requested off-line registration */
                CsrBtGattAddOrRemoveSubscribeElement(inst, prim);
                resultCode = CSR_BT_GATT_RESULT_SUCCESS;
            }
            else
            {
                CsrBtGattConnElement *conn = NULL;
                
                CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                           &conn,
                                                           prim->btConnId,
                                                           prim->gattId);

                if (conn)
                { /* Note element->cancelFunc is set to csrBtGattWriteRequestCancelHandler 
                     because a Write Request can only be cancel while setting up security. */
                    element->cancelFunc   = csrBtGattWriteRequestCancelHandler;
                    element->securityFunc = csrBtGattWriteRequestSecurityHandler;
                    prim->address         = conn->peerAddr;
                    element->dataOffset   = sizeof(CsrBtGattCliConfigBits);
                    /* The Config Configuration Value is copied so GATT can replay 
                       attlib_write_req in case of security failure. */
                    element->data         = csrBtGattReturnClientConfigurationValue(inst, prim);
                    element->attrHandle   = prim->clientConfigHandle;

                    attlib_write_req(CSR_BT_GATT_IFACEQUEUE,
                                     CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(prim->btConnId),
                                     prim->clientConfigHandle,
                                     CSR_BT_GATT_WRITE_REQUEST,
                                     element->dataOffset,
                                     CsrMemDup(element->data, element->dataOffset),
                                     NULL);
                    return;
                }
                else
                {
                    CsrStateEventException("CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ no connElement",
                                            CSR_BT_GATT_PRIM, prim->type, 0);
                }
            }
        }
        else
        {
            resultCode = CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID;
        }
    }

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                prim->gattId,
                                resultCode,
                                CSR_BT_SUPPLIER_GATT,
                                prim->btConnId);
    
    /* This procedure has finished. Start the next if any */
    CsrBtGattQueueRestoreHandler(inst, element);
}

CsrBool CsrBtGattWriteClientConfigurationReqHandler(GattMainInst *inst)
{
    CsrBtGattWriteClientConfigurationReq *prim = (CsrBtGattWriteClientConfigurationReq*)inst->msg;

    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattWriteClientConfigurationRestoreHandler,
                                   csrBtGattWriteCancelHandler,
                                   csrBtGattWriteRequestSecurityHandler));
}

CsrBool CsrBtGattSubscriptionReqHandler(GattMainInst *inst)
{
    CsrBtGattSubscriptionReq *prim = (CsrBtGattSubscriptionReq *)inst->msg;

    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &prim->gattId);

    if (appElement)
    {
        CsrBtResultCode           resultCode  = CSR_BT_GATT_RESULT_SUCCESS;
        CsrBtGattSubscribeElement *subElement = CsrBtGattGetSubscribeElement(inst,
                                                                             appElement,
                                                                             prim->address,
                                                                             prim->valueHandle);
        if (subElement)
        { /* The application has allready subscribe for it */
            if (subElement->configuration == CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION ||
                subElement->configuration == CSR_BT_GATT_CLIENT_CHARAC_CONFIG_NOTIFICATION)
            { /* The application is not allow to use CsrBtGattSubscriptionReq
                 as Client Configuration has higher priority */
                resultCode = CSR_BT_GATT_RESULT_CLIENT_CONFIGURATION_IN_USED;
            }
            else
            { /* The application is allow to use CsrBtGattSubscriptionReq */
                if (!prim->subscribe)
                { /* Remove the handle from the subscribe List */
                    CSR_BT_GATT_SUBSCRIBE_INST_REMOVE(appElement->subscribeInst, subElement);
                }
                /* Else - Just return CSR_BT_GATT_RESULT_SUCCESS, the 
                   application has already subscribe for the Attribute 
                   Value Handle/Address pair */ 
            }
        }
        else
        { /* The application has not subscribe for it */
            if (prim->subscribe)
            { /* Add new entry to the subscribe List */
                subElement = CSR_BT_GATT_SUBSCRIBE_INST_ADD_FIRST(appElement->subscribeInst);
                subElement->address         = prim->address; 
                subElement->attrHandle      = prim->valueHandle;
                subElement->configuration   = CSR_BT_GATT_CHARAC_SUBSCRIBE;
            }
            /* Else - Just return CSR_BT_GATT_RESULT_SUCCESS, the application 
               is trying to remove a handle which does not exist */
        }
        CsrBtGattStdCfmSend(CSR_BT_GATT_SUBSCRIPTION_CFM,
                            prim->gattId,
                            resultCode,
                            CSR_BT_SUPPLIER_GATT);
        return TRUE;
    }
    return FALSE;
}

/* Covers cancel of a GATT procedure */
CsrBool CsrBtGattCancelReqHandler(GattMainInst *inst)
{
    CsrBtGattCancelReq *prim = (CsrBtGattCancelReq*) inst->msg;

    if (CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &(prim->gattId)))
    {   
        CsrBtGattQueueElement *qElem = NULL;
        CSR_BT_GATT_QUEUE_FIND_MSG_TO_CANCEL(inst->queue,&qElem, prim->gattId, prim->btConnId);
        
        if (qElem)
        {
            qElem->cancelFunc(inst, qElem, CSR_BT_GATT_RESULT_CANCELLED, CSR_BT_SUPPLIER_GATT);
        }
        /* Else Nothing to cancel just ignore */
        return TRUE;
    }
    /* Invalid gattId */
    return FALSE;
}

/* Covers setting the event Event Mask */
static void csrBtGattIteratePhysicalConn(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement *conn     = (CsrBtGattConnElement *) elem;
    CsrBtGattFindPhysicalConn *ids = (CsrBtGattFindPhysicalConn*) value;

    if (CSR_BT_GATT_CONN_IS_CONNECTED(conn->state) &&
        ids->addressCount < CSR_BT_GATT_NUMBER_OF_PHYSICAL_CONN)
    {
        CsrUint16 i;

        for (i = 0; i < ids->addressCount; i++)
        {
            if(CsrBtAddrEqWithType(&(conn->peerAddr), ids->address[i].type, &(ids->address[i].addr)))
            { /* A CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND for this
                 address has already been sent to the application */
                return;
            }
        }
        ids->address[ids->addressCount] = conn->peerAddr;
        ids->addressCount++;    
        /* Send the CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND message
           to the application */
        CsrBtGattPhysicalLinkStatusIndSend(ids->gattId, 
                                           conn->peerAddr, 
                                           CSR_BT_GATT_GET_CONNINFO(conn),
                                           TRUE); /* TRUE = connected */   
    }
    /* Else - Not connected, just ignore */
}

CsrBool CsrBtGattSetEventMaskReqHandler(GattMainInst *inst)
{
    CsrBtGattSetEventMaskReq *prim  = (CsrBtGattSetEventMaskReq *) inst->msg;
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &prim->gattId);
    CsrBtGattEventMask mask;

    if (appElement)
    {
        /* Ensure only valid bits are set in the CFM */
        mask = (prim->eventMask
                & (CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_NONE
                   | CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PHYSICAL_LINK_STATUS
                   | CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_WHITELIST_CHANGE
                   | CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND));

        if ((prim->eventMask - mask) != 0)
        {
            /* Attempt to subscribe to invalid mask */
            CsrBtGattStdCfmSend(CSR_BT_GATT_SET_EVENT_MASK_CFM,
                                prim->gattId,
                                CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER,
                                CSR_BT_SUPPLIER_GATT);
        }
        else
        {
            /* Setup new mask */
            appElement->eventMask = mask;
            CsrBtGattStdCfmSend(CSR_BT_GATT_SET_EVENT_MASK_CFM,
                                prim->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT);

            /* Notify about current physical links */
            if (mask & CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PHYSICAL_LINK_STATUS)
            {
                CsrBtGattFindPhysicalConn ids;
                
                ids.gattId = appElement->gattId;
                ids.addressCount = 0;
                /* Do not initialise addr. The addr is set when
                 * ids.addressCount is incremented. */
                
                /* Find all Physical connection and send a
                 * CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND for each of
                 * them */
                CSR_BT_GATT_CONN_INST_ITERATE(inst->connInst,
                                              csrBtGattIteratePhysicalConn, &ids);
            }

            /* Notify this app about non-empty whitelists */
            if ((mask & CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_WHITELIST_CHANGE)
                && (inst->whitelistCount > 0))
            {
                CsrBtGattWhitelistChangeIndSend(appElement->gattId);
            }
        }
        return TRUE;
    }

    /* Invalid gattId */
    return FALSE;
}

static CsrBool csrBtGattSecurityReqSecurityHandler(void            *gattInst, 
                                                   void            *qElem,
                                                   CsrBtResultCode result, 
                                                   CsrBtSupplier   supplier)
{
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    CSR_UNUSED(gattInst);

    if (result == CSR_BT_RESULT_CODE_SC_SUCCESS && 
        supplier == CSR_BT_SUPPLIER_SC)
    { /* Security is set up with success. Map to the right  */
        result = CSR_BT_GATT_RESULT_SUCCESS;
        supplier = CSR_BT_SUPPLIER_GATT;
    }
    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_SECURITY_CFM,
                                element->gattId,
                                result,
                                supplier,
                                element->btConnId);
    /* Always returned FALSE to indicate that the security is finish.
       E.g. the csrBtGattScLeSecurityCfmHandler will call 
       CsrBtGattQueueRestoreHandler */
    return FALSE;
}

static void csrBtGattSecurityReqRestoreHandler(void       *gattInst, 
                                               void       *qElem,
                                               CsrUint16  mtu)
{
    CsrBtResultCode resultCode     = CSR_BT_GATT_RESULT_UNACCEPTABLE_PARAMETER;
    GattMainInst *inst             = (GattMainInst*) gattInst;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) qElem;
    CsrBtGattSecurityReq *prim     = (CsrBtGattSecurityReq *) element->gattMsg;

    if (prim->btConnId != CSR_BT_GATT_LOCAL_BT_CONN_ID)
    {
        CsrBtGattConnElement *conn = NULL;
        resultCode = CSR_BT_GATT_RESULT_UNKNOWN_CONN_ID;
        
        CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                   &conn,
                                                   prim->btConnId,
                                                   prim->gattId);
        if (conn && CSR_BT_GATT_CONN_IS_CONNECTED(conn->state))
        {
            element->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_SECURITY;
            CsrBtScLeSecurityReqSend(CSR_BT_GATT_IFACEQUEUE,
                                       prim->securityRequirements, 
                                       conn->peerAddr,
                                       conn->l2capFlags); 
            return;
        }
    }
    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_SECURITY_CFM,
                                prim->gattId,
                                resultCode,
                                CSR_BT_SUPPLIER_GATT,
                                prim->btConnId);

    /* This procedure has finished. Start the next if any */
    CsrBtGattQueueRestoreHandler(inst, element);
}


CsrBool CsrBtGattSecurityReqHandler(GattMainInst *inst)
{
    CsrBtGattSecurityReq *prim = (CsrBtGattSecurityReq *) inst->msg;

    return (csrBtGattNewReqHandler(inst, 
                                   prim->btConnId, 
                                   prim->gattId,
                                   csrBtGattSecurityReqRestoreHandler,
                                   NULL,
                                   csrBtGattSecurityReqSecurityHandler));
}



