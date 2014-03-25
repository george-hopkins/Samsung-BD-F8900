/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_gatt_private.h"

/* Restore queue for btConnId */
static void csrBtGattConnRestore(GattMainInst *inst,
                                 CsrBtConnId btConnId,
                                 CsrBtGattId gattId)
{
    CsrBtGattQueueElement *qelem = NULL;

    CSR_BT_GATT_QUEUE_FIND_BT_CONN_GATT_ID(inst->queue,
                                           &qelem,
                                           btConnId, gattId);
    CsrBtGattQueueRestoreHandler(inst, qelem);
}

/* Get existing connections toward address. Sets FSM event to be used
 * with new connections */
static CsrBtGattConnElement *csrBtGattConnGetExistingConn(GattMainInst *inst,
                                                          CsrBtGattConnElement *conn,
                                                          fsm_event_t *event)
{
    CsrBtGattConnElement *find;

    for (find = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
         find != NULL;
         find = find->next)
    {
        if((conn != find)
           && (CsrBtAddrEqWithType(&conn->peerAddr, find->peerAddr.type ,&find->peerAddr.addr)))
        {
            if(event != NULL)
            {
                conn->btConnId  = find->btConnId;
                conn->mtu       = find->mtu;
                conn->interval  = find->interval;      
                conn->timeout   = find->timeout;
                conn->latency   = find->latency;
                
                if (find->remoteName)
                {
                    conn->remoteName = (CsrUint8 *) CsrPmemAlloc(find->remoteNameLength); 
                    CsrMemCpy(conn->remoteName, find->remoteName, find->remoteNameLength);
                }

                switch(find->state)
                {
                    case GATT_CONN_ST_BREDR_CONNECT:
                    case GATT_CONN_ST_BREDR_ACCEPT:
                    case GATT_CONN_ST_PENDING:
                        *event = GATT_CONN_EV_CONNECT_PENDING;
                        break;

                    case GATT_CONN_ST_CONNECTED:
                        *event = GATT_CONN_EV_CONNECT_OK;
                        break;
                }
            }
            return find;
        }
    }
    return NULL;
}

/* Purge connection */
fsm_event_t CsrBtGattConnFsmPurge(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;
    CsrBtGattQueueElement *qElem;
    CsrBtConnId btConnId = conn->btConnId;

    inst = connargs->inst;

    qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);
    CSR_BT_GATT_CONN_INST_REMOVE(inst->connInst, conn);

    if (qElem)
    {
        /* Restore handler will gracefully fail connection oriented
         * commands. The rest must be dealt with before the purge */
        if (qElem->gattMsg && 
            qElem->restoreFunc)
        {
            qElem->restoreFunc(inst, qElem, 0);
        }
        else
        {
            CsrBtGattQueueRestoreHandler(inst, qElem);
        }
    }
    return FSM_EVENT_NULL;
}

/* Start central role. May kill the FSM */
fsm_event_t CsrBtGattConnFsmCentralReq(void *arg1, void *arg2)
{
    CsrBtGattConnElement *existing;
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;
    fsm_event_t event           = FSM_EVENT_NULL;

    /* Scan for existing connection to peer */
    existing = csrBtGattConnGetExistingConn(inst, conn, &event);
    if(existing != NULL)
    {
        /* Intialise queue with assigned btConnId. We are then in a
         * position where we can jump-start the fsm */
        conn->l2capFlags = existing->l2capFlags;
    }
    else
    {
        /* Ensure we do not attempt to create a scatter-net over LE */
        CsrBtGattConnElement *find;
        for (find = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
             find != NULL;
             find = find->next)
        {
            if(conn != find)
            {
                CsrUint16 method;
                method = find->l2capFlags & L2CA_CONFLAG_ENUM_MASK;
                if((method == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_SLAVE_DIRECTED))
                   || (method == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_SLAVE_WHITELIST))
                   || (method == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_SLAVE_UNDIRECTED)))
                {
                    /* Scatter-net detected. Jump start the FSM to return a
                       central cfm with error and restore the queue */
                    connargs->result = CSR_BT_GATT_RESULT_SCATTERNET;
                    return GATT_CONN_EV_CONNECT_FAIL;
                }
            }
        }
    }

    if(event == FSM_EVENT_NULL)
    { /* Normal, new connection setup. Kick scan parameter update */
        CsrBtGattUpdateScan(inst);
    }
    /* Else - A connection is already setup or being setup. 
              jump-start the fsm to send a central Cfm with success
              and restore queue */
    return event;
}

/* Start peripheral role. May kill the FSM */
fsm_event_t CsrBtGattConnFsmPeripheralReq(void *arg1, void *arg2)
{
    CsrBtGattConnElement *existing;
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;
    fsm_event_t event           = FSM_EVENT_NULL;
    
    /* Scan for existing connection to peer */
    existing = csrBtGattConnGetExistingConn(inst, conn, &event);
    if(existing != NULL)
    {
        /* Intialise queue with assigned btConnId. We are then in a
         * position where we can jump-start the fsm */
        conn->l2capFlags = existing->l2capFlags;
    }
    else
    {
        /* Ensure we do not attempt to create a scatter-net over LE */
        CsrBtGattConnElement *find;
        for (find = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
             find != NULL;
             find = find->next)
        {
            if(conn != find)
            {
                CsrUint16 method;
                method = find->l2capFlags & L2CA_CONFLAG_ENUM_MASK;
                if(((method == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_MASTER_DIRECTED))
                    || (method == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_MASTER_WHITELIST)))
                   || !CsrBtAddrEqWithType(&find->peerAddr, conn->peerAddr.type ,&conn->peerAddr.addr))
                {
                    /* Scatter-net detected. Jump start the FSM to return a
                       Peripheral cfm with error and restore the queue */
                    connargs->result = CSR_BT_GATT_RESULT_SCATTERNET;
                    return GATT_CONN_EV_CONNECT_FAIL;
                }
            }
        }
    }

    if(event == FSM_EVENT_NULL)
    {
        /* Normal, new connection setup. Kick advertising parameter
         * update */
        CsrBtGattUpdateAdvertise(inst);
    }
    /* Else - A connection is already setup or being setup. 
              jump-start the fsm to send a Peripheral Cfm with success
              and restore queue */
    return event;
}

/* Central parameters ok. Start ATT connection */
fsm_event_t CsrBtGattConnFsmCentralParamOk(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;
    CsrBtTypedAddr ad;

    inst = connargs->inst;
    CSR_UNUSED(inst);

    /* Poke the CM with a coex notification */
    if(L2CA_CONFLAG_IS_LE(conn->l2capFlags))
    {
        CsrBtCmLeScanReqCoexSend(CSR_BT_CM_IFACEQUEUE,
                                 (CsrUint8)(CSR_BT_CM_LE_MODE_ON
                                            |CSR_BT_CM_LE_MODE_COEX_NOTIFY));
    }

    ad.addr = conn->peerAddr.addr;
    ad.type = conn->peerAddr.type;

    /* Ask SD if it knows the Attribute Handle for Service Changed*/
    CsrBtSdReadDeviceInfoReqSendEx(CSR_BT_GATT_IFACEQUEUE,
                                   conn->peerAddr.addr,
                                   conn->peerAddr.type,
                                   CSR_BT_GATT_SD_READ_DEVICE_INFO_READ);

    attlib_connect_req(CSR_BT_GATT_IFACEQUEUE,
                       &ad,
                       CSR_BT_GATT_GET_L2CA_METHOD(conn->l2capFlags),
                       conn->l2capFlags,
                       NULL);

    return FSM_EVENT_NULL;
}

/* Connection setup or Central parameters bad. Kill the FSM */
fsm_event_t CsrBtGattConnFsmCentralFail(void *arg1, void *arg2)
{
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;
    CsrBtGattId  gattId         = conn->gattId;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_CENTRAL_CFM,
                                conn->gattId,
                                connargs->result,
                                connargs->supplier,
                                CSR_BT_CONN_ID_INVALID);

    CSR_BT_GATT_CONN_INST_REMOVE(inst->connInst, conn);

    csrBtGattConnRestore(inst,
                         CSR_BT_GATT_LOCAL_BT_CONN_ID,
                         gattId);

    return FSM_EVENT_NULL;
}




/* Central connection pending */
fsm_event_t CsrBtGattConnFsmCentralPending(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;

    inst = connargs->inst;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_CENTRAL_CFM,
                                conn->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT,
                                conn->btConnId);
    csrBtGattConnRestore(inst, CSR_BT_GATT_LOCAL_BT_CONN_ID, conn->gattId);

    /* Next expected event is a connect-ind/cfm from ATT */
    return FSM_EVENT_NULL;
}

/* Central connection ok */
fsm_event_t CsrBtGattConnFsmCentralOk(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;

    inst = connargs->inst;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_CENTRAL_CFM,
                                conn->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT,
                                conn->btConnId);
    csrBtGattConnRestore(inst, CSR_BT_GATT_LOCAL_BT_CONN_ID, conn->gattId);

    /* Progress immediately */
    return GATT_CONN_EV_CONNECT_OK;
}

/* Peripheral parameters ok. Start ATT connection */
fsm_event_t CsrBtGattConnFsmPeripheralParamOk(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;
    CsrBtTypedAddr ad;
    CsrUint8 advType;

    inst = connargs->inst;
    CSR_UNUSED(inst);

    /* Poke the CM with a coex notification */
    advType = (CsrUint8)((((conn->l2capFlags & L2CA_CONFLAG_ENUM_MASK) >> L2CA_CONFLAG_ENUM_OFFSET)
                          == L2CA_CONNECTION_LE_SLAVE_DIRECTED)
                         ? HCI_ULP_ADVERT_CONNECTABLE_DIRECTED
                         : HCI_ULP_ADVERT_CONNECTABLE_UNDIRECTED);
    CsrBtCmLeAdvertiseReqCoexSend(CSR_BT_CM_IFACEQUEUE,
                                  (CsrUint8)(CSR_BT_CM_LE_MODE_ON
                                             |CSR_BT_CM_LE_MODE_COEX_NOTIFY),
                                  advType);

    ad.addr = conn->peerAddr.addr;
    ad.type = conn->peerAddr.type;

    /* Ask SD if it knows the Attribute Handle for Service Changed*/
    CsrBtSdReadDeviceInfoReqSendEx(CSR_BT_GATT_IFACEQUEUE,
                                   conn->peerAddr.addr,
                                   conn->peerAddr.type,
                                   CSR_BT_GATT_SD_READ_DEVICE_INFO_READ);

    attlib_connect_req(CSR_BT_GATT_IFACEQUEUE,
                       &ad,
                       CSR_BT_GATT_GET_L2CA_METHOD(conn->l2capFlags),
                       conn->l2capFlags,
                       NULL);

    return FSM_EVENT_NULL;
}

/* Connection setup or Peripheral parameters bad. Kill the FSM */
fsm_event_t CsrBtGattConnFsmPeripheralFail(void *arg1, void *arg2)
{
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;
    CsrBtGattId  gattId         = conn->gattId;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_PERIPHERAL_CFM,
                                conn->gattId,
                                connargs->result,
                                connargs->supplier,
                                CSR_BT_CONN_ID_INVALID);

    CSR_BT_GATT_CONN_INST_REMOVE(inst->connInst, conn);

    csrBtGattConnRestore(inst,
                         CSR_BT_GATT_LOCAL_BT_CONN_ID,
                         gattId);

    return FSM_EVENT_NULL;
}

/* Peripheral connection pending */
fsm_event_t CsrBtGattConnFsmPeripheralPending(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;

    inst = connargs->inst;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_PERIPHERAL_CFM,
                                conn->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT,
                                conn->btConnId);
    csrBtGattConnRestore(inst, CSR_BT_GATT_LOCAL_BT_CONN_ID, conn->gattId);

    /* Next expected event is a connect-ind/cfm from ATT */
    return FSM_EVENT_NULL;
}

/* Peripheral connection ok */
fsm_event_t CsrBtGattConnFsmPeripheralOk(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;

    inst = connargs->inst;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_PERIPHERAL_CFM,
                                conn->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT,
                                conn->btConnId);
    csrBtGattConnRestore(inst, CSR_BT_GATT_LOCAL_BT_CONN_ID, conn->gattId);


    /* Progress immediately */
    return GATT_CONN_EV_CONNECT_OK;
}

/* BR/EDR master connect. May kill the FSM */
fsm_event_t CsrBtGattConnFsmBredrConnectReq(void *arg1, void *arg2)
{
    CsrBtGattConnElement *existing;
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;
    fsm_event_t event           = FSM_EVENT_NULL;

    /* Scan for existing connection to peer */
    existing = csrBtGattConnGetExistingConn(inst, conn, &event);
    if(existing != NULL)
    {
        /* Intialise queue with assigned btConnId. We are then in a
         * position where we can jump-start the fsm */
        conn->l2capFlags = existing->l2capFlags;
    }
    else
    {
        CsrBtTypedAddr ad;
        
        ad.addr = conn->peerAddr.addr;
        ad.type = conn->peerAddr.type;

        /* Ask SD if it knows the Attribute Handle for Service Changed*/
        CsrBtSdReadDeviceInfoReqSendEx(CSR_BT_GATT_IFACEQUEUE,
                                       conn->peerAddr.addr,
                                       conn->peerAddr.type,
                                       CSR_BT_GATT_SD_READ_DEVICE_INFO_READ);

        attlib_connect_req(CSR_BT_GATT_IFACEQUEUE,
                           &ad,
                           CSR_BT_GATT_GET_L2CA_METHOD(conn->l2capFlags),
                           conn->l2capFlags,
                           NULL);
        event = FSM_EVENT_NULL;
    }

    return event;
}

/* BR/EDR master connect failed */
fsm_event_t CsrBtGattConnFsmBredrConnectFail(void *arg1, void *arg2)
{
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;
    CsrBtGattId  gattId         = conn->gattId;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_BREDR_CONNECT_CFM,
                                conn->gattId,
                                connargs->result,
                                connargs->supplier,
                                CSR_BT_CONN_ID_INVALID);

    CSR_BT_GATT_CONN_INST_REMOVE(inst->connInst, conn);

    csrBtGattConnRestore(inst,
                         CSR_BT_GATT_LOCAL_BT_CONN_ID,
                         gattId);

    return FSM_EVENT_NULL;
}

/* BR/EDR master connection setup in progress */
fsm_event_t CsrBtGattConnFsmBredrConnectPending(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;

    inst = connargs->inst;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_BREDR_CONNECT_CFM,
                                conn->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT,
                                conn->btConnId);
    csrBtGattConnRestore(inst, CSR_BT_GATT_LOCAL_BT_CONN_ID, conn->gattId);

    /* Next expected event is a connect-ind/cfm from ATT */
    return FSM_EVENT_NULL;
}

/* BR/EDR master connection done */
fsm_event_t CsrBtGattConnFsmBredrConnectOk(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    GattMainInst *inst;

    inst = connargs->inst;

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_BREDR_CONNECT_CFM,
                                conn->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT,
                                conn->btConnId);
    csrBtGattConnRestore(inst, CSR_BT_GATT_LOCAL_BT_CONN_ID, conn->gattId);

    /* Progress immediately */
    return GATT_CONN_EV_CONNECT_OK;
}

/* BR/EDR slave connect accept start */
fsm_event_t CsrBtGattConnFsmBredrAcceptReq(void *arg1, void *arg2)
{
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;

    /* BR/EDR accept is basically a standard page-scan request, so it
     * doesn't really make sense to copy existing connections. Still,
     * only start page scan if current scan count is 1 (which means
     * that we're the the first) */
    if (CsrBtGattConnPageScanners(inst) == 1)
    {
        CsrBtCml2caConnectAcceptReqSend(CSR_BT_GATT_IFACEQUEUE,
                                        0, /* local psm */
                                        0, /* cod */
                                        SECL4_IN_LEVEL_2, /* security (enc+auth) */
                                        0, /* frame size */
                                        0); /* uuid */
    }

    /* Ask SD if it knows the Attribute Handle for Service Changed*/
    CsrBtSdReadDeviceInfoReqSendEx(CSR_BT_GATT_IFACEQUEUE,
                                   conn->peerAddr.addr,
                                   conn->peerAddr.type,
                                   CSR_BT_GATT_SD_READ_DEVICE_INFO_READ);

    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_BREDR_ACCEPT_CFM,
                                conn->gattId,
                                CSR_BT_GATT_RESULT_SUCCESS,
                                CSR_BT_SUPPLIER_GATT,
                                conn->btConnId); /* still 'invalid' */

    /* The btConnId is still 'unassigned' as the L2CAP connection
     * is still waiting for the peer to connect. So, to actually
     * restore the queue we use the temporary "empty" btConnId */
    csrBtGattConnRestore(inst, CSR_BT_GATT_LOCAL_BT_CONN_ID, conn->gattId);
    return FSM_EVENT_NULL;
}

/* BR/EDR slave connect accept stop */
fsm_event_t CsrBtGattConnFsmBredrAcceptStop(void *arg1, void *arg2)
{
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;

    if(CsrBtGattConnPageScanners(inst) <= 1)
    {
        /* We are the last page scanner. Stop. */
        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_GATT_IFACEQUEUE,
                                              0); /* psm */

        /* Remove the handle of the Service Changed Characteristic
           Value from GATTs private subscribe list.
           Note update set to FALSE as GATT did not have a change
           for reading the Service Changed Characteristic
           Value*/
        CsrBtGattRemoveHandleForServiceChanged(inst,
                                               conn->peerAddr,
                                               FALSE);
    }
    return GATT_CONN_EV_DISCONNECT_IND;
}


/* BR/EDR connect accept success */
fsm_event_t CsrBtGattConnFsmBredrAcceptOk(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    GattMainInst *inst     = connargs->inst;
    CSR_UNUSED(arg2);

    /* FSM state already changed, so see if we can stop page-scan
     * mode */
    if(CsrBtGattConnPageScanners(inst) < 1)
    {
        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_GATT_IFACEQUEUE,
                                              0); /* psm */
    }

    /* Use normal connect-ok procedure, Progress immediately */
    return GATT_CONN_EV_CONNECT_OK;
}

/* Connection failed while pending */
fsm_event_t CsrBtGattConnFsmConnectFail(void *arg1, void *arg2)
{
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;

    CsrBtGattConnectIndSend(CSR_BT_GATT_CONNECT_IND,
                            conn->gattId,
                            connargs->result,
                            connargs->supplier,
                            conn->btConnId,
                            CSR_BT_GATT_GET_CONNINFO(conn),
                            &conn->peerAddr,
                            conn->mtu);
    CSR_BT_GATT_CONN_INST_REMOVE(inst->connInst, conn);
    return FSM_EVENT_NULL;
}

/* Connection success. */
fsm_event_t CsrBtGattConnFsmConnectOk(void *arg1, void *arg2)
{
    GattConnArgs *connargs = arg1;
    CsrBtGattConnElement *conn = arg2;
    
    CsrBtGattConnectIndSend(CSR_BT_GATT_CONNECT_IND,
                            conn->gattId,
                            CSR_BT_GATT_RESULT_SUCCESS,
                            CSR_BT_SUPPLIER_GATT,
                            conn->btConnId,
                            CSR_BT_GATT_GET_CONNINFO(conn),
                            &conn->peerAddr,
                            conn->mtu);
    /* Note this check shall be made after CsrBtGattConnectIndSend
       is called to ensure that the application receives the 
       CSR_BT_GATT_CONNECT_IND first */
    if (conn->latency != CSR_BT_GATT_INVALID_CONN_LATENCY)
    {
        CsrBtGattParamConnChangeIndSendHandler(connargs->inst, conn);
    }
    return FSM_EVENT_NULL;
}

fsm_event_t CsrBtGattConnFsmDisconnectReq(void *arg1, void *arg2)
{
    CsrBtGattConnNumOfExistingPendingIds ids;
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;

    CSR_BT_GATT_CONN_INST_NUM_OF_EXISTING_PENDING_CONN(inst->connInst,
                                                       ids,
                                                       conn->btConnId);
    if (ids.numOfRsp == 0)
    { /* Last man standing. Turn off the lights. */
        attlib_disconnect_req(CSR_BT_GATT_IFACEQUEUE,
                              CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(conn->btConnId),
                              NULL);
        return FSM_EVENT_NULL;
    }
    else
    {
        CsrBtGattDisconnectIndSend(conn->gattId,
                                   CSR_BT_GATT_RESULT_SUCCESS,
                                   CSR_BT_SUPPLIER_GATT,
                                   conn->btConnId,
                                   &conn->peerAddr,
                                   (CsrBtGattConnInfo)(L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                                                       ? CSR_BT_GATT_CONNINFO_LE
                                                       : CSR_BT_GATT_CONNINFO_BREDR));

        return GATT_CONN_EV_PURGED;
    }
}

/* Shared master/central established connection has died. Kill the FSM */
fsm_event_t CsrBtGattConnFsmDisconnectInd(void *arg1, void *arg2)
{
    GattConnArgs *connargs      = arg1;
    CsrBtGattConnElement *conn  = arg2;
    GattMainInst *inst          = connargs->inst;
    CSR_UNUSED(inst);

    if(conn->gattId !=  CSR_BT_GATT_INVALID_GATT_ID)
    {
        CsrBtGattDisconnectIndSend(conn->gattId,
                                   connargs->result,
                                   connargs->supplier,
                                   conn->btConnId,
                                   &conn->peerAddr,
                                   (CsrBtGattConnInfo)(L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                                                       ? CSR_BT_GATT_CONNINFO_LE
                                                       : CSR_BT_GATT_CONNINFO_BREDR));
    }
    return GATT_CONN_EV_PURGED;
}
