
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_private.h"

static void csrBtGattPhysicalLinkStatusIndSendHandler(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppElement *appElement = (CsrBtGattAppElement *) elem;

    if (CSR_MASK_IS_SET(appElement->eventMask, CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PHYSICAL_LINK_STATUS))
    { /* The application has subscribe for physical link status event */
        CsrBtGattphysicalLinkStatusIds *ids  = (CsrBtGattphysicalLinkStatusIds *) value;
        CsrBtGattPhysicalLinkStatusIndSend(appElement->gattId,
                                           ids->address,
                                           ids->connInfo,
                                           ids->status);
    }
    /* Else - The application has not subscribe for physical
       link status event, just ignore */
}

/* Generic ATT_CONNECT handler */
static CsrUint16 csrBtGattConnectLeHandler(GattMainInst    *inst,
                                           CsrBtTypedAddr  *addr,
                                           CsrUint16       cid,
                                           l2ca_conflags_t flags,
                                           CsrUint16       result,
                                           CsrUint16       mtu)
{
    CsrBtGattConnElement *conn;
    CsrBtConnId btConnId;
    CsrBool loop;
    CsrBool wanted;
    CsrBool exclusive;
    CsrUint16 preferedMtu = CSR_BT_ATT_MTU_DEFAULT;
    GattConnArgs connargs;

    /* Initial scan of existing connections */
    exclusive = FALSE;
    if (result == L2CA_CONNECT_SUCCESS)
    {
        /* It is more appropriate to send the
         * CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND message before all the
         * CSR_BT_GATT_CONNECT_IND messages is send */
        CsrBool radioType;
        
        CsrBtGattphysicalLinkStatusIds ids;
        ids.address   = *addr;
        ids.status    = TRUE; /* TRUE == Connected */
        ids.connInfo  = ((L2CA_CONFLAG_IS_LE(flags)) ? CSR_BT_GATT_CONNINFO_LE : CSR_BT_GATT_CONNINFO_BREDR);
        radioType     = (CsrBool)((CSR_MASK_IS_SET(ids.connInfo, CSR_BT_GATT_CONNINFO_BREDR)) ? FALSE : TRUE);
        
        /* Tell CM that an physical link has been established */
        CsrBtCmLePhysicalLinkStatusReqSend(ids.address, radioType, TRUE);

        CSR_BT_GATT_APP_INST_ITERATE(inst->appInst,
                                     csrBtGattPhysicalLinkStatusIndSendHandler,
                                     &ids);
    }

    for(conn = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
        conn != NULL;
        conn = conn->next)
    {
        /* Connection collision, i.e. addr matches but CID does not */
        if((conn->btConnId != CSR_BT_CONN_ID_INVALID)
           && (CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(conn->btConnId) != cid)
           && CsrBtAddrEqWithType(addr, conn->peerAddr.type, &conn->peerAddr.addr))
        {
            /* Disconnect and completely bail out */
            attlib_disconnect_req(CSR_BT_GATT_IFACEQUEUE,
                                  cid,
                                  NULL);
            return 0;
        }

        /* Exclusive access detected */
        if((conn->btConnId == CSR_BT_CONN_ID_INVALID)
           && (conn->flags & CSR_BT_GATT_FLAGS_EXCLUSIVE)
           && CsrBtAddrEqWithType(addr, conn->peerAddr.type, &conn->peerAddr.addr))
        {
            exclusive = TRUE;
        }
    }

    /* Find connections tied in with this CID */
    wanted = FALSE;
    do
    {
        loop = FALSE;

        for(conn = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
            conn != NULL;
            conn = conn->next)
        {
            btConnId = conn->btConnId;
            connargs.result = result;
            connargs.supplier = CSR_BT_SUPPLIER_ATT;
            connargs.inst = inst;

            /* Exclusive access needed */
            if(exclusive
                && (btConnId == CSR_BT_CONN_ID_INVALID)
                && ((conn->flags & CSR_BT_GATT_FLAGS_EXCLUSIVE) != CSR_BT_GATT_FLAGS_EXCLUSIVE))
            {
                /* This connection wasn't the exclusive one */
                continue;
            }

            /* Does this connection really match what we are expecting? */
            else if(L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                    && !(conn->flags & CSR_BT_GATT_FLAGS_WHITELIST)
                    && !(conn->flags & CSR_BT_GATT_FLAGS_UNDIRECTED)
                    && !CsrBtAddrEqWithType(addr, conn->peerAddr.type, &conn->peerAddr.addr))
            {
                /* Directed connection, but address didn't match */
                continue;
            }

            /* Are a BredrAccept pending while LE also uses a device address of 0?*/
            else if (btConnId == CSR_BT_CONN_ID_INVALID      &&
                     result == L2CA_CONNECT_INITIATING       &&
                     CsrBtBdAddrEqZero(&conn->peerAddr.addr) &&
                     !L2CA_CONFLAG_IS_LE(conn->l2capFlags))
            {
                /* Is BredrAccept pending, just ignore this as it belongs
                   to the LE Central or Peripheral procedure */
                continue;
            }

            /* Direct match on CID */
            else if(((result == L2CA_CONNECT_SUCCESS)
                     || (result == L2CA_CONNECT_INITIATING))
                    && (cid == CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(btConnId)))
            {
                /* Inject event */
                wanted = TRUE;
                conn->mtu = mtu;
                if (conn->preferredMtu > preferedMtu)
                {
                    preferedMtu = conn->preferredMtu;
                }
                CsrBtAddrCopy(&conn->peerAddr, addr);

                /* Connargs set when conn is found */
                GATT_FSMRUN(&connargs,
                            (fsm_event_t)(result == L2CA_CONNECT_SUCCESS
                                          ? GATT_CONN_EV_CONNECT_OK
                                          : GATT_CONN_EV_CONNECT_PENDING),
                            conn);
            }
            /* Connect success on BR/EDR - only happens for BredrAccept */
            else if ((btConnId == CSR_BT_CONN_ID_INVALID)
                     && (result == L2CA_CONNECT_SUCCESS)
                     && !L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                     && !L2CA_CONFLAG_IS_LE(flags))
            {
                /* Set connection btConnId */
                conn->btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(cid);
                conn->l2capFlags = flags;

                /* Inject event */
                wanted = TRUE;
                conn->mtu = mtu;
                if (conn->preferredMtu > preferedMtu)
                {
                    preferedMtu = conn->preferredMtu;
                }
                CsrBtAddrCopy(&conn->peerAddr, addr);

                /* Connargs set when conn is found */
                GATT_FSMRUN(&connargs,
                            GATT_CONN_EV_CONNECT_OK,
                            conn);
            }
            else if((btConnId == CSR_BT_CONN_ID_INVALID)
                    && (result == L2CA_CONNECT_INITIATING)
                    && CsrBtAddrEqWithType(addr, conn->peerAddr.type, &conn->peerAddr.addr))
            {
                /* Set connection btConnId */
                conn->btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(cid);
                conn->l2capFlags = flags;

                /* Inject event */
                wanted = TRUE;
                conn->mtu = mtu;
                if (conn->preferredMtu > preferedMtu)
                {
                    preferedMtu = conn->preferredMtu;
                }
                CsrBtAddrCopy(&conn->peerAddr, addr);

                /* Connargs set when conn is found */
                GATT_FSMRUN(&connargs,
                            GATT_CONN_EV_CONNECT_PENDING,
                            conn);
            }
            else if((cid ==  CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(btConnId))
                    || CsrBtAddrEqWithType(addr, conn->peerAddr.type, &conn->peerAddr.addr))
            {
                /* Error. This will delete the connection instance. */
                wanted = TRUE;
                CsrBtAddrCopy(&conn->peerAddr, addr);

                /* Connargs set when conn is found */
                GATT_FSMRUN(&connargs, GATT_CONN_EV_CONNECT_FAIL, conn);
                loop = TRUE; /* retry scan from beginning of modified
                              * list */
            }

            /* If connection list was modified (invalidated) we need
             * to restart the scan from the beginning */
            if(loop)
            {
                break;
            }
        }
    }
    while(loop);

    if(!wanted && result == L2CA_CONNECT_SUCCESS)
    { /* Unwanted connection Disconnect it */
        attlib_disconnect_req(CSR_BT_GATT_IFACEQUEUE,
                              cid,
                              NULL);
        return 0;
    }

    return preferedMtu;
}


static CsrBool csrBtGattValidateQueueMsgState(att_result_t result, CsrUint8 queueMsgState)
{
    if (queueMsgState == CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    { /* The procure is cancelled */
        if (result == ATT_RESULT_SUCCESS_MORE ||
            result == ATT_RESULT_SUCCESS_SENT)
        { /* ATT need to send more messages to GATT before
             this procedure can be stop */
            return FALSE;
        }
    }
    return TRUE;
}

static CsrBool csrBtGattGetResultCode(CsrUint8              queueMsgState,
                                      att_result_t          result,
                                      CsrBtResultCode       *resultCode,
                                      CsrBtSupplier         *resultSupplier)
{
    *resultCode     = CSR_BT_GATT_RESULT_SUCCESS;
    *resultSupplier = CSR_BT_SUPPLIER_GATT;

    switch (result)
    {
        case ATT_RESULT_SUCCESS:
            {
                if (queueMsgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
                {
                    return TRUE;
                }
                break;
            }
        case ATT_RESULT_SUCCESS_MORE:
            /* fallthrough */
        case ATT_RESULT_SUCCESS_SENT:
            {
                return TRUE;
            }
        case ATT_RESULT_ATTR_NOT_FOUND:
            {
                if (queueMsgState != CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK)
                {
                    *resultCode      = (CsrBtResultCode) result;
                    *resultSupplier  = CSR_BT_SUPPLIER_ATT;
                }
                break;
            }
        default:
            {
                *resultCode      = (CsrBtResultCode) result;
                *resultSupplier  = CSR_BT_SUPPLIER_ATT;
                break;
            }
    }
    return FALSE;
}

static CsrBool csrBtGattAttDiscoverCharacHandler(GattMainInst               *inst,
                                                 CsrBtGattQueueElement      *qElem,
                                                 CsrBtGattDiscoverCharacReq *reqMsg,
                                                 ATT_READ_BY_TYPE_CFM_T     *prim)
{
    CsrBtResultCode resultCode;
    CsrBtSupplier   resultSupplier;

    if (csrBtGattGetResultCode(qElem->msgState,
                               prim->result,
                               &resultCode,
                               &resultSupplier))
    {
        if (prim->size_value == CSR_BT_GATT_CHARAC_DECLARATION_MIN_LENGTH ||
            prim->size_value == CSR_BT_GATT_CHARAC_DECLARATION_MAX_LENGTH)
        {
            CsrBtUuid uuid;
            uuid.length = (CsrUint8) (prim->size_value - CSR_BT_GATT_CHARAC_PROPERTIES_LENGTH - CSR_BT_GATT_CHARAC_VALUE_HANDLE_LENGTH);
            CsrMemSet(uuid.uuid, 0, CSR_BT_UUID128_SIZE);
            CsrMemCpy(uuid.uuid, &prim->value[CSR_BT_GATT_CHARAC_UUID_FIRST_INDEX], uuid.length);

            if (reqMsg->uuid.length == CSR_BT_UUID16_SIZE &&
                CSR_BT_UUID_GET_16(reqMsg->uuid) == CSR_BT_GATT_CHARACTERISTIC_UUID)
            { /* Discovers all Characteristics of a service,
                 Send result to the application */
                CsrBtGattDiscoverCharacIndSend(qElem,
                                               reqMsg->btConnId,
                                               prim->handle,
                                               uuid,
                                               prim->value);
            }
            else if (reqMsg->uuid.length == uuid.length &&
                     !CsrMemCmp(reqMsg->uuid.uuid, uuid.uuid, uuid.length))
            { /* Discover Characteristics by UUID, where the UUID
                 do match. Send result to the application */
                CsrBtGattDiscoverCharacIndSend(qElem,
                                               reqMsg->btConnId,
                                               prim->handle,
                                               uuid,
                                               prim->value);

            }
            /* Else - Discover Characteristics by UUID where the UUID
               don't match, just ignore */
        }
        /* Else - Invalid length, just ignore */

        if (prim->handle + 1 <= reqMsg->endGroupHandle)
        {
            return TRUE;
        }
    }

    if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    {
        if (prim->result == ATT_RESULT_SUCCESS &&
            qElem->msgState != CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK)
        {
            resultCode     = (CsrBtResultCode) ATT_RESULT_ATTR_NOT_FOUND;
            resultSupplier = CSR_BT_SUPPLIER_ATT;
        }
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_CHARAC_CFM,
                                    qElem->gattId,
                                    resultCode,
                                    resultSupplier,
                                    reqMsg->btConnId);
    }
    /* Else - This message has sent to the application when GATT
       received CSR_BT_GATT_CANCEL_REQ */
    return FALSE;
}

static CsrBool csrBtGattAttFindInclServiceHandler(CsrBtGattQueueElement  *qElem,
                                                  CsrBtConnId            btConnId,
                                                  CsrBtGattHandle        endGroupHandle,
                                                  ATT_READ_BY_TYPE_CFM_T *prim,
                                                  CsrBtGattHandle        *inclHandle)
{
    CsrBtResultCode resultCode;
    CsrBtSupplier   resultSupplier;

    *inclHandle = CSR_BT_GATT_ATTR_HANDLE_INVALID;

    if (csrBtGattGetResultCode(qElem->msgState,
                               prim->result,
                               &resultCode,
                               &resultSupplier))
    {
        if (prim->size_value == CSR_BT_GATT_INCLUDE_WITH_UUID_LENGTH)
        { /* The service UUID must be a 16-bit Bluetooth UUID */
            CsrBtGattFindInclServicesIndSend(qElem,
                                             btConnId,
                                             prim->handle,
                                             prim->size_value,
                                             prim->value);
            if (prim->handle + 1 <= endGroupHandle)
            { /* Read next */
                return TRUE;
            }
        }
        else if (prim->size_value >= CSR_BT_GATT_INCLUDE_WITHOUT_UUID_LENGTH &&
                 prim->result == ATT_RESULT_SUCCESS)
        { /* The service UUID must be a 128-bit Bluetooth UUID */
            qElem->attrHandle   = prim->handle;
            qElem->data         = (CsrUint8 *) CsrPmemZalloc(CSR_BT_GATT_INCLUDE_128_BIT_LENGTH);
            qElem->dataOffset   = CSR_BT_GATT_INCLUDE_WITHOUT_UUID_LENGTH;
            CsrMemCpy(qElem->data, prim->value, CSR_BT_GATT_INCLUDE_WITHOUT_UUID_LENGTH);
            *inclHandle = CSR_BT_GATT_GET_HANDLE(prim->value, CSR_BT_GATT_INCLUDE_START_HANDLE_INDEX);
            return FALSE;
        }
        else
        { /* The peer device include definition declaration
             don't follow the GATT specification. */
            if (prim->handle + 1 <= endGroupHandle)
            { /* Read next, this result is ignore */
                return TRUE;
            }
            else if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK)
            { /* The peer device has only one include definition declaration
                 which don't follow the spec return Error */
                resultCode      = (CsrBtResultCode)ATT_RESULT_INVALID_PDU;
                resultSupplier  = CSR_BT_SUPPLIER_ATT;
            }
        }
    }

    if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    {
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_FIND_INCL_SERVICES_CFM,
                                    qElem->gattId,
                                    resultCode,
                                    resultSupplier,
                                    btConnId);
    }
    /* Else - This message has sent to the application when GATT
       received CSR_BT_GATT_CANCEL_REQ */
    return FALSE;
}

static CsrBool csrBtGattAttReadByTypeFindInclServiceHandler(CsrBtGattQueueElement  *qElem,
                                                            ATT_READ_BY_TYPE_CFM_T *prim,
                                                            CsrBtConnId            btConnId)
{
    CsrBtGattHandle inclHandle;
    CsrBtGattFindInclServicesReq *reqMsg = (CsrBtGattFindInclServicesReq *) qElem->gattMsg;

    if (csrBtGattAttFindInclServiceHandler(qElem,
                                           btConnId,
                                           reqMsg->endGroupHandle,
                                           prim,
                                           &inclHandle))
    {
        if (prim->result == ATT_RESULT_SUCCESS)
        {
            CsrUint32 inclUuid[4] = {0x00002802, 0, 0, 0};

            attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                    prim->cid,
                                    (CsrUint16)(prim->handle + 1),
                                    reqMsg->endGroupHandle,
                                    ATT_UUID16,
                                    &inclUuid[0],
                                    NULL);
        }
        /* Else - Wait for another ATT_READ_BY_TYPE_CFM message */
    }
    else
    {
        if (inclHandle == CSR_BT_GATT_ATTR_HANDLE_INVALID)
        { /* Return TRUE to indicate that this procedure is finish */
            return TRUE;
        }
        else
        { /* The service UUID must be a 128-bit Bluetooth UUID,
             used the Read Request to get it                */
            attlib_read_req(CSR_BT_GATT_IFACEQUEUE,
                            prim->cid,
                            inclHandle,
                            NULL);
        }
    }
    return FALSE;
}

static CsrBool csrBtGattAttReadCfmFindInclServiceHandler(CsrBtGattQueueElement *qElem,
                                                         ATT_READ_CFM_T        *prim,
                                                         CsrBtConnId           btConnId)
{
    CsrBtGattFindInclServicesReq *reqMsg = (CsrBtGattFindInclServicesReq *) qElem->gattMsg;

    if (prim->result == ATT_RESULT_SUCCESS &&
        qElem->dataOffset + prim->size_value == CSR_BT_GATT_INCLUDE_128_BIT_LENGTH)
    {
        CsrMemCpy(&(qElem->data[qElem->dataOffset]), prim->value, prim->size_value);
        CsrBtGattFindInclServicesIndSend(qElem,
                                         btConnId,
                                         qElem->attrHandle,
                                         CSR_BT_GATT_INCLUDE_128_BIT_LENGTH,
                                         qElem->data);
    }


    if (qElem->attrHandle + 1 <= reqMsg->endGroupHandle)
    {
        CsrUint32 inclUuid[4] = {0x00002802, 0, 0, 0};


        attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                prim->cid,
                                (CsrUint16)(qElem->attrHandle + 1),
                                reqMsg->endGroupHandle,
                                ATT_UUID16,
                                &inclUuid[0],
                                NULL);
        /* Must free qElem->data and set it to NULL as it may be
           re-used later */
        CsrPmemFree(qElem->data);
        qElem->data = NULL;
        qElem->attrHandle = CSR_BT_GATT_ATTR_HANDLE_INVALID;
    }
    else
    { /* This procedure is finish. Start the next if any and
         send a confirm message to the application */
        if (qElem->msgState == CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK)
        {
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_FIND_INCL_SERVICES_CFM,
                                        qElem->gattId,
                                        CSR_BT_GATT_RESULT_SUCCESS,
                                        CSR_BT_SUPPLIER_GATT,
                                        btConnId);
        }
        else
        {
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_FIND_INCL_SERVICES_CFM,
                                        qElem->gattId,
                                        (CsrBtResultCode) ATT_RESULT_ATTR_NOT_FOUND,
                                        CSR_BT_SUPPLIER_ATT,
                                        btConnId);
        }
        /* Return TRUE to indicate that this procedure is finish */
        return TRUE;
    }
    return FALSE;
}

static CsrBool csrBtGattAttPrimaryServiceHandler(CsrBtGattQueueElement *qElem,
                                                 CsrBtConnId           btConnId,
                                                 att_result_t          result,
                                                 CsrUint16             handle,
                                                 CsrUint16             end,
                                                 CsrUint16             length,
                                                 CsrUint8              *value)
{
    CsrBtResultCode resultCode;
    CsrBtSupplier   resultSupplier;

    if (csrBtGattGetResultCode(qElem->msgState,
                               result,
                               &resultCode,
                               &resultSupplier))
    {
        CsrBtGattDiscoverServicesIndSend(qElem,
                                         btConnId,
                                         handle,
                                         end,
                                         length,
                                         value);
        if (end < CSR_BT_GATT_ATTR_HANDLE_MAX)
        {
            return TRUE;
        }
    }

    if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    {
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_SERVICES_CFM,
                                    qElem->gattId,
                                    resultCode,
                                    resultSupplier,
                                    btConnId);
    }
    /* Else - This message has sent to the application when GATT
       received CSR_BT_GATT_CANCEL_REQ */
    return FALSE;
}

static CsrBool csrBtGattAttFindInfoUuidCheck(CsrBtGattQueueElement *qElem,
                                             att_uuid_type_t       uuidType,
                                             CsrUint32             attUuid)
{
    if (qElem->msgState == CSR_BT_GATT_MSG_QUEUE_IGNORE_CHARAC_DESCRIPTOR)
    {
        return FALSE;
    }
    else
    {
        if (uuidType == ATT_UUID16)
        {
            if (attUuid == CSR_BT_GATT_PRIMARY_SERVICE_UUID   ||
                attUuid == CSR_BT_GATT_SECONDARY_SERVICE_UUID ||
                attUuid == CSR_BT_GATT_CHARACTERISTIC_UUID)
            {
                qElem->msgState = CSR_BT_GATT_MSG_QUEUE_IGNORE_CHARAC_DESCRIPTOR;
                return FALSE;
            }
        }
    }
    return TRUE;
}

static CsrBool csrBtGattAttDiscoverCharacDescriptorsHandler(CsrBtGattQueueElement *qElem,
                                                            CsrBtConnId           btConnId,
                                                            CsrBtGattHandle       endGroupHandle,
                                                            ATT_FIND_INFO_CFM_T   *prim)
{
    CsrBtResultCode resultCode;
    CsrBtSupplier   resultSupplier;

    if (csrBtGattGetResultCode(qElem->msgState,
                               prim->result,
                               &resultCode,
                               &resultSupplier))
    {
        if (csrBtGattAttFindInfoUuidCheck(qElem, prim->uuid_type, prim->uuid[0]))
        {
            CsrBtGattDiscoverCharacDescriptorsIndSend(qElem,
                                                      btConnId,
                                                      prim->handle,
                                                      prim->uuid_type,
                                                      prim->uuid);

            if (prim->handle + 1 <= endGroupHandle)
            { /* Read next */
                return TRUE;
            }
        }
        else
        {
            if (prim->result == ATT_RESULT_SUCCESS_MORE)
            {
                return TRUE;
            }
            /* Else - prim->result must be ATT_RESULT_SUCCESS */
        }
    }

    if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    {
        if (qElem->msgState == CSR_BT_GATT_MSG_QUEUE_IGNORE_CHARAC_DESCRIPTOR)
        {
            resultCode     = CSR_BT_GATT_RESULT_INVALID_HANDLE_RANGE;
            resultSupplier = CSR_BT_SUPPLIER_GATT;
        }
        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM,
                                    qElem->gattId,
                                    resultCode,
                                    resultSupplier,
                                    btConnId);
    }
    /* Else - This message has sent to the application when GATT
       received CSR_BT_GATT_CANCEL_REQ */
    return FALSE;
}

static void csrBtGattAttValueIndSendHandler(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppElement *appElement = (CsrBtGattAppElement *) elem;
    CsrBtGattSubscribeFindIds *ids  = (CsrBtGattSubscribeFindIds*) value;

    if (appElement->gattId != ids->privateGattId)
    {
        if (CSR_BT_GATT_SUBSCRIBE_INST_FIND_SUBSCRIBE_ID(appElement, value))
        {
            /* Found Match */
            CsrUint8 *data = NULL;

            if (ids->value && ids->valueLength > 0)
            {
                data = (CsrUint8 *) CsrPmemAlloc(ids->valueLength);
                CsrMemCpy(data, ids->value, ids->valueLength);
            }
            else
            {
                ids->valueLength = 0;
            }
            CsrBtGattNotificationIndSend(appElement->gattId,
                                         ids->btConnId,
                                         ids->address,
                                         ids->attrHandle,
                                         ids->valueLength,
                                         data,
                                         ids->connInfo);
        }
        /* Else - Did not find a match, just ignore */
    }
    /* Else - Ignore, hit GATT own subscribe List.
       This list is only used to handle
       Service Changed */
}

static CsrBool csrBtGattRemoveAffectedAttrHandles(CsrCmnListElm_t *elem, void *value)
{
    /* Remove the attribute handles which are invalid after a Service
     * Changed is received */
    CsrBtGattSubscribeElement  *subElem = (CsrBtGattSubscribeElement *) elem;
    CsrBtGattServiceChangedInd *msg     = (CsrBtGattServiceChangedInd*) value;

    if (CsrBtAddrEqWithType(&(subElem->address), msg->address.type, &(msg->address.addr)) &&
        subElem->attrHandle >= msg->startHandle           &&
        subElem->attrHandle <= msg->endHandle)
    {
        return TRUE;
    }
    return FALSE;
}

static void csrBtGattServiceChangedIndSendHandler(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppElement *appElement = (CsrBtGattAppElement *) elem;
    CsrBtGattSubscribeFindIds *ids  = (CsrBtGattSubscribeFindIds *) value;

    if (appElement->gattId != ids->privateGattId)
    {
        CsrBtGattServiceChangedInd *msg = (CsrBtGattServiceChangedInd *)
            CsrPmemAlloc(sizeof(CsrBtGattServiceChangedInd));

        msg->type           = CSR_BT_GATT_SERVICE_CHANGED_IND;
        msg->gattId         = appElement->gattId;
        msg->address        = ids->address;
        msg->startHandle    = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(ids->value);
        msg->endHandle      = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&ids->value[2]);
        msg->connInfo       = ids->connInfo;

        /* Before CsrBtGattServiceChangedInd it sent to the application,
           GATT must insure that attribute handles which has become invalid
           is remove from the subscribe List*/
        CSR_BT_GATT_SUBSCRIBE_INST_ITERATE_REMOVE(appElement->subscribeInst,
                                                  csrBtGattRemoveAffectedAttrHandles,
                                                  msg);

        CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(appElement->gattId),
                           msg);
    }
    /* Else - Ignore, hit GATT own subscribe List.
       This list is only used to handle
       Service Changed */
}

static CsrBool csrBtGattAttReadCfmReadByUuidHandler(GattMainInst           *inst,
                                                    CsrBtGattQueueElement  *qElem,
                                                    CsrBtConnId            btConnId,
                                                    CsrUint16              mtu,
                                                    CsrBtGattHandle        endGroupHandle,
                                                    ATT_READ_BY_TYPE_CFM_T *prim,
                                                    CsrBool                *readBlob)
{
    CsrBtResultCode resultCode;
    CsrBtSupplier   resultSupplier;

    *readBlob = FALSE;

    if (csrBtGattGetResultCode(qElem->msgState,
                               prim->result,
                               &resultCode,
                               &resultSupplier))
    {
        if (prim->result == ATT_RESULT_SUCCESS &&
            prim->size_value >= (mtu - CSR_BT_GATT_ATT_READ_BY_TYPE_HEADER_LENGTH))
        {
            *readBlob = TRUE;
            return TRUE;
        }
        else
        {
            CsrBool remoteNameRead = FALSE;
            CsrBtGattReadByUuidReq *reqMsg = (CsrBtGattReadByUuidReq *) qElem->gattMsg;

            if (reqMsg->uuid.length == CSR_BT_UUID16_SIZE && 
                CSR_GET_UINT16_FROM_LITTLE_ENDIAN(reqMsg->uuid.uuid) == CSR_BT_GATT_DEVICE_NAME_UUID)
            { /* The Remote Name Characteristic Value has been read, Update saved named */
                remoteNameRead = TRUE;
                CSR_BT_GATT_CONN_INST_UPDATE_REMOTE_NAME(inst->connInst,
                                                         btConnId,
                                                         prim->size_value,
                                                         prim->value);  
            }

            if (qElem->gattId != inst->privateGattId)
            {
                qElem->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK;
                CsrBtGattReadByUuidIndSend(qElem->gattId,
                                           btConnId,
                                           prim->handle,
                                           prim->size_value,
                                           prim->value);
                prim->value = NULL;
                
                if (prim->handle + 1 <= endGroupHandle)
                {
                    return TRUE;
                }
            }
            else
            { /* GATT have called ATT_READ_BY_TYPE_REQ */
                if (remoteNameRead)
                { /* In order to read the read the remote name */
                     if (qElem->dataElemIndex > 0)
                     { /* SC has requested to read the name, 
                          before before GATT had time to read it. 
                          Send Result to SC */
                        CsrBtGattId gattId = CSR_BT_GATT_CREATE_GATT_ID(qElem->dataElemIndex, 
                                                                        CSR_BT_SC_IFACEQUEUE);
                        CsrBtGattReadByUuidIndSend(gattId,
                                                   btConnId,
                                                   prim->handle,
                                                   prim->size_value,
                                                   prim->value);
                        prim->value = NULL;
                     }
                }
                /* Else -  In order to find the attribute Handle of the 
                           Service Changed Characteristic Value. Note, 
                           if GATT is reading the Service Changed Characteristic
                           the peer server do not follow the spec as the Service 
                           Changed Characteristic Value shall be set to Not Readable */
            }
        }
    }

    if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
    {
        if (qElem->gattId != inst->privateGattId)
        {
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_READ_BY_UUID_CFM,
                                        qElem->gattId,
                                        resultCode,
                                        resultSupplier,
                                        btConnId);
        }
        else
        { /* GATT have called ATT_READ_BY_TYPE_REQ in order
             to either find the attribute Handle of the Service Changed
             Characteristic Value or to read the remote name */
            CsrBtGattConnElement *conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                                                         &btConnId);

            if (conn)
            {
                CsrBtGattReadByUuidReq *reqMsg = (CsrBtGattReadByUuidReq *) qElem->gattMsg;

                if (CSR_GET_UINT16_FROM_LITTLE_ENDIAN(reqMsg->uuid.uuid) == CSR_BT_GATT_SERVICE_CHANGED_UUID)
                { /* GATT were looking the Service Changed Characteristic Value */
                    if (prim->result == ATT_RESULT_READ_NOT_PERMITTED &&
                        prim->handle != CSR_BT_GATT_ATTR_HANDLE_INVALID)
                    { /* GATT were able to find the handle of the Service
                         Changed Characteristic Value */
                        CsrBtGattAddHandleForServiceChanged(inst,
                                                            conn->peerAddr,
                                                            prim->handle);
                    }
                    else
                    { /* GATT were not able to find the handle. E.g.
                         the peer server do not support this feature.
                        Set the handle to CSR_BT_GATT_ATTR_HANDLE_INVALID
                        to indicate that GATT had search for it */
                        CsrBtGattAddHandleForServiceChanged(inst,
                                                            conn->peerAddr,
                                                            CSR_BT_GATT_ATTR_HANDLE_INVALID);
                    }
                }
                else
                { /* GATT failed to Read the Remote Name Characteristic Value  */
                    if (qElem->dataElemIndex > 0)
                    { /* SC has requested to read the name, 
                         before before GATT had time to read it. 
                         Send Result to SC */
                        CsrBtGattId gattId = CSR_BT_GATT_CREATE_GATT_ID(qElem->dataElemIndex, 
                                                                        CSR_BT_SC_IFACEQUEUE);

                        CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_READ_BY_UUID_CFM,
                                                    gattId,
                                                    resultCode,
                                                    resultSupplier,
                                                    btConnId);
                    }
                }
            }
        }
    }
    /* Else - This message has sent to the application when GATT
              received CSR_BT_GATT_CANCEL_REQ */
    return FALSE;
}

static CsrBool csrBtGattReadByUuidReadBlobSecurityHandler(void            *gattInst,
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
        { /* Security is set up with success while reading blob. 
             Continue the Read Blob procedure */
            attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE, 
                                 CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(element->btConnId), 
                                 element->attrHandle, 
                                 element->dataOffset, 
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

static CsrBool csrBtGattAttReadByTypeReadByUuidHandler(GattMainInst           *inst,
                                                       CsrBtGattQueueElement  *qElem,
                                                       ATT_READ_BY_TYPE_CFM_T *prim,
                                                       CsrBtConnId            btConnId,
                                                       CsrUint16              mtu)
{
    CsrBool readBlob;
    CsrBtGattReadByUuidReq *reqMsg = (CsrBtGattReadByUuidReq *) qElem->gattMsg;

    if (csrBtGattAttReadCfmReadByUuidHandler(inst,
                                             qElem,
                                             btConnId,
                                             mtu,
                                             reqMsg->endGroupHandle,
                                             prim,
                                             &readBlob))
    {
        if (prim->result == ATT_RESULT_SUCCESS)
        {
            if (readBlob)
            {
                qElem->data = (CsrUint8 *) CsrPmemZalloc(CSR_BT_ATT_MTU_MAX);
                qElem->dataOffset = prim->size_value;
                qElem->attrHandle = prim->handle;
                CsrMemCpy(qElem->data, prim->value, prim->size_value);
                if (qElem->securityFunc)
                { /* The ATT_READ_BY_TYPE_CFM did not required security. 
                     Change the security qElem->securityFunc in order to able 
                     to handle security on a ATT_READ_BLOB_CFM message. E.g
                     GATT will only try to setup security automatic once per 
                     operation */
                    qElem->securityFunc = csrBtGattReadByUuidReadBlobSecurityHandler;
                }
                /* Else - Security have passed on the given handle, do not set 
                   qElem->securityFunc avoid retry deadlock */
                attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE, prim->cid, 
                                     qElem->attrHandle, qElem->dataOffset, NULL);
            }
            else
            {
                CsrUint32       attUuid[4];
                att_uuid_type_t uuidType;
                CsrBtGattGetAttUuid(reqMsg->uuid, attUuid, &uuidType);
                
                /* Note, in order to be able to handle that the server requires 
                   security on the next handle being read, GATT shall set the 
                   security callback function again, because the function 
                   csrBtGattScLeSecurityCfmHandler set it to NULL and GATT 
                   shall allow one security procedure per handle being read.
                   The qElem->attrHandle is set to
                   prim->handle + 1 so GATT is able replay this command in 
                   case of security failure */
                qElem->attrHandle   = (CsrUint16)(prim->handle + 1);
                qElem->securityFunc = CsrBtGattReadByUuidSecurityHandler;
                attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                        prim->cid,
                                        qElem->attrHandle,
                                        reqMsg->endGroupHandle,
                                        uuidType,
                                        attUuid,
                                        NULL);
            }
        }
        /* Else - Wait for another ATT_READ_BY_TYPE_CFM message */
    }
    else
    { /* Return TRUE to indicate that this procedure is finish */
        return TRUE;
    }
    return FALSE;
}

static CsrBool csrBtGattValidatePrepareWriteData(CsrUint16               writeFlags,
                                                 ATT_PREPARE_WRITE_CFM_T *prim,
                                                 CsrUint8                *srcValue)
{
    if (writeFlags == CSR_BT_GATT_WRITE_RELIABLE)
    {
        if(CsrMemCmp(srcValue, prim->value, prim->size_value))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/* Public ATT upstream handlers */
void CsrBtGattAttRegisterCfmHandler(GattMainInst *inst)
{ /* Received an ATT subsystem register confirmation */
    ATT_REGISTER_CFM_T *prim = (ATT_REGISTER_CFM_T *) inst->msg;

    if(prim->result == ATT_RESULT_SUCCESS)
    {
        /* Initialisation of scan/advertise/connection parameters is
         * now triggered by the BLUECORE_INITIALISED event from the
         * CM */
        CsrBtCmSetEventMaskReqSend(CSR_BT_GATT_IFACEQUEUE,
                                   (CSR_BT_GATT_DEFAULT_CM_EVENT_MASK |
                                    CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION),
                                   CSR_BT_CM_EVENT_MASK_COND_ALL);
    }
    else
    {
        CsrStateEventException("ATT_REGISTER_CFM Failed", CSR_BT_ATT_PRIM, prim->type, 0);
    }
}

void CsrBtGattAttAddCfmHandler(GattMainInst *inst)
{
    ATT_ADD_CFM_T *prim          = (ATT_ADD_CFM_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_LOCAL_BT_CONN_ID;
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem)
    {
        if (CSR_BT_GATT_GET_QID_FROM_GATT_ID(qElem->gattId) == CSR_BT_GATT_IFACEQUEUE)
        {
            if (prim->result == ATT_RESULT_SUCCESS)
            {   /* GATT has added mandatory attributes to the database.
                 * I.e. the GATT init procedure is done. Subscribe to name
                 * changes and send scan/advertise/connection settings,
                 * and register Mandatory SDP record. Note this
                 * procedure is responsible of subscribe for the
                 * CSR_BT_CM_LOCAL_NAME_CHANGE_IND messages */
                CsrBtGattSdpRecordElement *sdpElem = CSR_BT_GATT_SDP_INST_ADD_LAST(inst->sdpRecordInst);
                sdpElem->startHandle = CSR_BT_GATT_ATTR_HANDLE_GATT_UUID;
                sdpElem->endHandle   = CSR_BT_GATT_ATTR_HANDLE_SERVICE_CHANGED;
                sdpElem->uuid.length = CSR_BT_UUID16_SIZE;
                CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_GATT_UUID, sdpElem->uuid.uuid);
                sdpElem->state       = CSR_BT_GATT_SDP_PENDING;

                /* Start the SDP procedure to register an SDP record */
                CsrBtGattSdsRequestHandler(inst, CSR_BT_ATT_PRIM);
            }
            else
            { /* Unable to Register Mandatory GATT db doing the Init procedure */
                CsrStateEventException("ATT_ADD_CFM with Error", CSR_BT_ATT_PRIM, prim->type, 0);
            }
        }
        else
        {
            /* Need to store prim->result in local variable as the
               function CsrBtGattSdsRequestHandler is freeing
               inst->msg */
            att_result_t result = prim->result;

            if (result == ATT_RESULT_SUCCESS)
            { /* Need to register at least one SDP record. E.g. a Primary service
                 support BR/EDR. Change sdpState from CSR_BT_GATT_SDP_INIT
                 to CSR_BT_GATT_SDP_PENDING to indicate that all record
                 needs to be register */
                CsrBtGattSdpRecordElement *sdpElem;
                CSR_BT_GATT_SDP_INST_FIND_STATE(inst->sdpRecordInst,
                                                &sdpElem,
                                                CSR_BT_GATT_SDP_INIT);

                if (sdpElem)
                {
                    /* Start the SDP procedure to register an SDP record */
                    CSR_BT_GATT_SDP_INST_ITERATE_CHANGE_FROM_INIT_TO_PENDING_STATE(inst->sdpRecordInst);
                    CsrBtGattSdsRequestHandler(inst, CSR_BT_ATT_PRIM);
                }
            }
            else
            { /* Remove all element with sdpState == CSR_BT_GATT_SDP_INIT */
                CSR_BT_GATT_SDP_INST_ITERATE_REMOVE_BY_STATE(inst->sdpRecordInst, CSR_BT_GATT_SDP_INIT);
            }

            /* Send result to the application */
            CsrBtGattStdCfmSend(CSR_BT_GATT_DB_ADD_CFM,
                                qElem->gattId,
                                result,
                                CSR_BT_GATT_GET_SUPPLIER(result));
        }

        /* This procedure is finish. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, qElem);
    }
    else
    {
        CsrStateEventException("ATT_ADD_CFM received with no qElem", CSR_BT_ATT_PRIM, prim->type, 0);
    }
}

void CsrBtGattAttRemoveCfmHandler(GattMainInst *inst)
{ /* Confirmation to attribute removal from the data base.
     This function return FALSE if an exception has occurred */
    ATT_REMOVE_CFM_T      *prim  = (ATT_REMOVE_CFM_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_LOCAL_BT_CONN_ID;
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    { /* Sent the result to the application */
        CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &qElem->gattId);

        if (appElement)
        {
            CsrPrim type          = *(CsrPrim *)qElem->gattMsg;
            CsrBtGattHandle start = CSR_BT_GATT_ATTR_HANDLE_INVALID;
            CsrBtGattHandle end   = CSR_BT_GATT_ATTR_HANDLE_INVALID;

            switch (type)
            {
                case CSR_BT_GATT_DB_REMOVE_REQ:
                    { /* The application has requested GATT to remove
                         some handles from the data base */
                        if (prim->result == ATT_RESULT_SUCCESS)
                        {
                            CsrBtGattDbRemoveReq *reqMsg = (CsrBtGattDbRemoveReq *) qElem->gattMsg;
                            start = reqMsg->start;
                            end   = reqMsg->end;
                        }
                        CsrBtGattDbRemoveCfmSend(qElem->gattId,
                                                 prim->result,
                                                 CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                                 prim->num_attr);

                        /* This procedure is finish. Start the next if any */
                        CsrBtGattQueueRestoreHandler(inst, qElem);
                        break;
                    }
                case CSR_BT_GATT_DB_DEALLOC_REQ:
                    { /* GATT have made sure that the handles that the application
                         want to deallocate also is remove from the data base */
                        CsrBtGattDbDeallocCfmSend(qElem->gattId,
                                                  CSR_BT_GATT_RESULT_SUCCESS,
                                                  CSR_BT_SUPPLIER_GATT,
                                                  appElement->start,
                                                  appElement->end);
                        start = appElement->start;
                        end   = appElement->end;

                        appElement->start = CSR_BT_GATT_ATTR_HANDLE_INVALID;
                        appElement->end   = CSR_BT_GATT_ATTR_HANDLE_INVALID;

                        CSR_BT_GATT_APP_INST_SORT_BY_ATTR_VALUE(inst->appInst);

                        /* This procedure is finish. Start the next if any */
                        CsrBtGattQueueRestoreHandler(inst, qElem);
                        break;
                    }
                case CSR_BT_GATT_UNREGISTER_REQ:
                    { /* The application is being unregister from GATT
                         Ensure that the scan settings are in sync with
                         remaining applications. */
                        start = appElement->start;
                        end   = appElement->end;
                        CsrBtGattUpdateScan(inst);
                        break;
                    }
                default:
                    {
                        CsrStateEventException("ATT_REMOVE_CFM received with invalid request type",
                                               CSR_BT_ATT_PRIM, prim->type, 0);
                        break;
                    }
            }

            if (start != CSR_BT_GATT_ATTR_HANDLE_INVALID)
            {
                /* This function goes though the SDP list and set
                   the SDP state correct before CsrBtGattSdsRequestHandler
                   is called */
                CSR_BT_GATT_SDP_INST_ITERATE_UNREGISTER(inst->sdpRecordInst, start, end);

                /* Start the SDP procedure to Unregister an SDP record */
                CsrBtGattSdsRequestHandler(inst, CSR_BT_ATT_PRIM);
            }
        }
        else
        {
            CsrStateEventException("ATT_REMOVE_CFM with no appElement",
                                   CSR_BT_ATT_PRIM, prim->type, 0);
        }
    }
    else
    {
        CsrStateEventException("ATT_REMOVE_CFM received with no qElem",
                               CSR_BT_ATT_PRIM, prim->type, 0);
    }
}

void CsrBtGattAttConnectCfmHandler(GattMainInst *inst)
{
    ATT_CONNECT_CFM_T *cfm = (ATT_CONNECT_CFM_T*)inst->msg;

    if(CSR_MASK_IS_SET(cfm->flags, L2CA_CONFLAG_INCOMING) &&
       cfm->result != L2CA_CONNECT_SUCCESS)
    { /* GATT has rejected an incoming BR/EDR connection.
         Just ignore/ break out */
        return; 
    }

    /* COEX: Connection failed. The HCI event will be handled by the
     * DM/L2CAP, so the CM will never know about this. So, we tell it
     * now so it in turn can notify the COEX-listeners */
    if((cfm->result != L2CA_CONNECT_INITIATING)
       && (cfm->result != L2CA_CONNECT_PENDING)
       && (cfm->result != L2CA_CONNECT_SUCCESS)
       && L2CA_CONFLAG_IS_LE(cfm->flags))
    {
        CsrUint16 method;
        method = cfm->flags & L2CA_CONFLAG_ENUM_MASK;

        if((method == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_MASTER_DIRECTED))
           || (method == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_LE_MASTER_WHITELIST)))
        {
            CsrBtCmLeScanReqCoexSend(CSR_BT_CM_IFACEQUEUE,
                                     (CsrUint8)(CSR_BT_CM_LE_MODE_OFF
                                                |CSR_BT_CM_LE_MODE_COEX_NOTIFY));
        }
        else
        {
            CsrBtCmLeAdvertiseReqCoexSend(CSR_BT_CM_IFACEQUEUE,
                                          (CsrUint8)(CSR_BT_CM_LE_MODE_OFF
                                                     |CSR_BT_CM_LE_MODE_COEX_NOTIFY),
                                          (CsrUint8)((((cfm->flags & L2CA_CONFLAG_ENUM_MASK)
                                                       >> L2CA_CONFLAG_ENUM_OFFSET)
                                                      == L2CA_CONNECTION_LE_SLAVE_DIRECTED)
                                                     ? HCI_ULP_ADVERT_CONNECTABLE_DIRECTED
                                                     : HCI_ULP_ADVERT_CONNECTABLE_UNDIRECTED));
        }
    }

    if(cfm->result == L2CA_CONNECT_PENDING)
    {
        /* Ignore the pending result code entirely */
        ;
    }
    else
    {
        CsrBtTypedAddr addr;
        CsrUint16 mtu;
        CsrUint16 cid         = cfm->cid;
        l2ca_conflags_t flags = cfm->flags;

        addr.addr = cfm->addrt.addr;
        addr.type = cfm->addrt.type;

        mtu = csrBtGattConnectLeHandler(inst,
                                        &addr,
                                        cfm->cid,
                                        cfm->flags,
                                        (CsrUint16)cfm->result,
                                        cfm->mtu);

        if (mtu > 0)
        {
            if (cfm->result == L2CA_CONNECT_SUCCESS)
            {
                if (mtu > CSR_BT_ATT_MTU_DEFAULT)
                { /* GATT shall Exchange MTU */
                    CsrBtGattExchangeMtuHandler(inst, mtu, cid, addr);
                }

                /* Check if the handle of the Service Changed Characteristic
                    Value is known or not. If not GATT shall find it */
                CsrBtGattFindServiceChangedHandler(inst, cid, addr);

                /* If using the LE radio, make sure that the Le name is read */ 
                CsrBtGattReadRemoteLeNameHandler(inst, cid, flags);
            }
            else if (cfm->result != L2CA_CONNECT_RETRYING &&
                     cfm->result != L2CA_CONNECT_INITIATING)
            {
                /* Fail to setup the connection. Remove the handle of the
                 * Service Changed Characteristic Value from GATTs private
                 * subscribe list. Note update set to FALSE as GATT did
                 * not have a change for reading the Service Changed
                 * Characteristic Value */
                CsrBtGattRemoveHandleForServiceChanged(inst, addr, FALSE);
            }
            /* Else - The connection is being setup, just ignore */
        }
        /* Else - An unwanted connection is being disconnected, just ignore */
    }
}

void CsrBtGattAttConnectIndHandler(GattMainInst *inst)
{
    ATT_CONNECT_IND_T *ind = (ATT_CONNECT_IND_T*)inst->msg;

    if(CSR_MASK_IS_SET(ind->flags, L2CA_CONFLAG_INCOMING))
    { /* A peer device request to setup an BR/EDR connection */
        if (CsrBtGattConnPageScanners(inst) == 0)
        { /* GATT is not ready to accept incoming BR/EDR connections.
             Reject it */
            attlib_connect_rsp(CSR_BT_GATT_IFACEQUEUE, ind->cid, L2CA_CONNECT_REJ_PSM, NULL);
        }
        else
        { /* GATT is ready to accept incoming BR/EDR connections.
             Accept it */
            attlib_connect_rsp(CSR_BT_GATT_IFACEQUEUE, ind->cid, L2CA_CONNECT_SUCCESS, NULL);
        }
    }
    else
    {
        CsrBtTypedAddr addr;
        CsrUint16 mtu;
        CsrUint16 cid         = ind->cid;
        l2ca_conflags_t flags = ind->flags;

        addr.addr = ind->addrt.addr;
        addr.type = ind->addrt.type;

        mtu = csrBtGattConnectLeHandler(inst,
                                        &addr,
                                        ind->cid,
                                        ind->flags,
                                        L2CA_RESULT_SUCCESS,
                                        ind->mtu);

        if (mtu > 0)
        {
            if (mtu > CSR_BT_ATT_MTU_DEFAULT)
            { /* GATT shall Exchange MTU */
                CsrBtGattExchangeMtuHandler(inst, mtu, cid, addr);
            }

            /* Check if the handle of the Service Changed Characteristic
               Value is known or not. If not GATT shall find it */
            CsrBtGattFindServiceChangedHandler(inst, cid, addr);

            /* If using the LE radio, make sure that the Le name is read */ 
            CsrBtGattReadRemoteLeNameHandler(inst, cid, flags);
        }
        /* Else - An unwanted connection is being disconnected, just ignore */
    }
}

void CsrBtGattAttDisconnectIndHandler(GattMainInst *inst)
{
    CsrBtGattphysicalLinkStatusIds ids;
    CsrBtGattConnElement *conn = NULL;
    ATT_DISCONNECT_IND_T *ind = (ATT_DISCONNECT_IND_T *) inst->msg;
    CsrBtConnId btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(ind->cid);
    GattConnArgs connargs;

    ids.status = TRUE;

    /* Mark prepared writes as bad and fail */
    CsrBtGattPlayExecuteMarkFail(inst, ind->cid);
    CsrBtGattPlayExecuteFinalise(inst, ind->cid);

    /* We need to tell all connections attached to this CID */
    do
    {
        conn = CSR_BT_GATT_CONN_INST_FIND_BT_CONN_ID(inst->connInst,
                                                     &btConnId);

        if (conn)
        {
            /* Found a valid device address */
            ids.address  = conn->peerAddr;
            ids.status   = FALSE; /* FALSE == released */
            ids.connInfo = CSR_BT_GATT_GET_CONNINFO(conn);

            /* The connection list was modified (invalidated), we need
             * to start the list scan from the beginning */
            if((ind->reason == L2CA_DISCONNECT_NORMAL)
                || (conn->state == GATT_CONN_ST_DISCONNECTING)
                || ((ind->reason == ATT_RESULT_INVALID_CID)))
            {
                connargs.result     = CSR_BT_GATT_RESULT_SUCCESS;
                connargs.supplier   = CSR_BT_SUPPLIER_GATT;
            }
            else
            {
                connargs.result     = ind->reason;
                connargs.supplier   = CSR_BT_SUPPLIER_ATT;
            }
            connargs.inst = inst;
            GATT_FSMRUN(&connargs, GATT_CONN_EV_DISCONNECT_IND, conn);
        }
    }
    while(conn);

    if (!ids.status)
    { /* Found a valid device address */

        CsrBool radioType = (CsrBool)((CSR_MASK_IS_SET(ids.connInfo, CSR_BT_GATT_CONNINFO_BREDR)) ? FALSE : TRUE);

        /* Remove the handle of the Service Changed
           Characteristic Value from GATTs private
           subscribe list. Note update set to TRUE as
           GATT did have a change for reading the
           Service Changed Characteristic Value */
        CsrBtGattRemoveHandleForServiceChanged(inst,
                                               ids.address,
                                               TRUE);

        /* Check if the peer device is paired or not.
           If not GATT must remove all handle that the
           application has subscribe for */
        CsrBtSdReadDeviceInfoReqSendEx(CSR_BT_GATT_IFACEQUEUE,
                                       ids.address.addr,
                                       ids.address.type,
                                       CSR_BT_GATT_SD_READ_DEVICE_INFO_DELETE);
        
        /* Tell CM that an physical link has been released */
        CsrBtCmLePhysicalLinkStatusReqSend(ids.address, radioType, FALSE);

        /* It is more appropriate to send all the CSR_BT_GATT_DISCONNECT_IND
           messages before the CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND is send */
        CSR_BT_GATT_APP_INST_ITERATE(inst->appInst,
                                     csrBtGattPhysicalLinkStatusIndSendHandler,
                                     &ids);
    }
}

void CsrBtGattAttExchangeMtuCfmHandler(GattMainInst *inst)
{
    ATT_EXCHANGE_MTU_CFM_T *prim    = (ATT_EXCHANGE_MTU_CFM_T *) inst->msg;
    CsrBtConnId            btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement  *qElem   = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem)
    {
        if (prim->result == ATT_RESULT_SUCCESS && prim->mtu > CSR_BT_ATT_MTU_DEFAULT)
        {
            CSR_BT_GATT_CONN_INST_UPDATE_MTU(inst->connInst, qElem->btConnId, prim->mtu);
        }
        /* Else - No need to update MTU */

        /* This procedure is finish. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, qElem);
    }
    /* Else - ATT_EXCHANGE_MTU_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttExchangeMtuIndHandler(GattMainInst *inst)
{ /* The Client has send a Exchange MTU request. The GATT
     server will only accept the Client proposal if the server
     is exchanging the MTU as well. This ensures that the Server
     only allows that MTU is exchange right after the ATT
     connection is setup */
    CsrUint16              mtu   = CSR_BT_ATT_MTU_DEFAULT;
    ATT_EXCHANGE_MTU_IND_T *prim = (ATT_EXCHANGE_MTU_IND_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattConnElement   *conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                                                   &btConnId);

    if (conn)
    {
        CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);
        mtu = conn->mtu;

        if (qElem && qElem->gattMsg)
        {
            CsrBtGattPrim type = *(CsrBtGattPrim *) qElem->gattMsg;

            if (type == CSR_BT_GATT_EXCHANGE_MTU_REQ)
            {
                CsrBtGattExchangeMtuReq *reqMsg = (CsrBtGattExchangeMtuReq *) qElem->gattMsg;

                if (reqMsg->btConnId == btConnId)
                { /* When the MTU is exchanged in both directions,
                     the MTUs shall be the same in each direction */
                    mtu = reqMsg->mtu;
                }
            }
        }
    }
    attlib_exchange_mtu_rsp(CSR_BT_GATT_IFACEQUEUE, prim->cid, mtu, NULL);
}

void CsrBtGattAttFindInfoCfmHandler(GattMainInst *inst)
{
    ATT_FIND_INFO_CFM_T *prim    = (ATT_FIND_INFO_CFM_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        if(CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId) > 0 &&
           csrBtGattValidateQueueMsgState(prim->result, qElem->msgState))
        {
            CsrBtGattDiscoverCharacDescriptorsReq *reqMsg =
                (CsrBtGattDiscoverCharacDescriptorsReq *) qElem->gattMsg;

            if (csrBtGattAttDiscoverCharacDescriptorsHandler(qElem,
                                                             btConnId,
                                                             reqMsg->endGroupHandle,
                                                             prim))
            {
                if (prim->result == ATT_RESULT_SUCCESS)
                {
                    attlib_find_info_req(CSR_BT_GATT_IFACEQUEUE,
                                         prim->cid,
                                         (CsrUint16)(prim->handle + 1),
                                         reqMsg->endGroupHandle,
                                         NULL);
                }
                /* Else - Wait for another ATT_FIND_INFO_CFM message */
            }
            else
            { /* This procedure is finish. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, qElem);
            }
        }
        /* Else - The connection is invalid or this procedure has
           been cancelled. If cancelled, GATT shall wait for
           another ATT_FIND_INFO_CFM message */
    }
    /* Else - ATT_FIND_INFO_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttFindByTypeValueCfmHandler(GattMainInst *inst)
{
    ATT_FIND_BY_TYPE_VALUE_CFM_T *prim = (ATT_FIND_BY_TYPE_VALUE_CFM_T *) inst->msg;
    CsrBtConnId btConnId               = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem       = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        if(CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId) > 0 &&
           csrBtGattValidateQueueMsgState(prim->result, qElem->msgState))
        {
            CsrBtGattDiscoverServicesReq *reqMsg = (CsrBtGattDiscoverServicesReq *) qElem->gattMsg;

            if (csrBtGattAttPrimaryServiceHandler(qElem,
                                                  btConnId,
                                                  prim->result,
                                                  prim->handle,
                                                  prim->end,
                                                  reqMsg->uuid.length,
                                                  reqMsg->uuid.uuid))
            {


                if (prim->result == ATT_RESULT_SUCCESS)
                {

                    CsrUint8 *value = (CsrUint8 *) CsrPmemAlloc(reqMsg->uuid.length);
                    CsrMemCpy(value, reqMsg->uuid.uuid, reqMsg->uuid.length);
                    attlib_find_by_type_value_req(CSR_BT_GATT_IFACEQUEUE,
                                                  prim->cid,
                                                  (CsrUint16)(prim->end + 1),
                                                  CSR_BT_GATT_ATTR_HANDLE_MAX,
                                                  CSR_BT_GATT_PRIMARY_SERVICE_UUID,
                                                  reqMsg->uuid.length,
                                                  value,
                                                  NULL);
                }
                /* Else - Wait for another ATT_FIND_BY_TYPE_VALUE_CFM message */
            }
            else
            { /* This procedure is finish. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, qElem);
            }
        }
        /* Else - The connection is invalid or this procedure has
           been cancelled. If cancelled, GATT shall wait for
           another ATT_FIND_BY_TYPE_VALUE_CFM message */
    }
    /* Else - ATT_FIND_BY_TYPE_VALUE_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttReadByTypeCfmHandler(GattMainInst *inst)
{
    /* Note according to ATT spec the Read Blob Request would be used to
       read the remaining octets of a long attribute value. GATT only
       check if Read blob must be use when the CSR_BT_GATT_READ_BY_UUID_REQ
       procedure is running. The reason for this is that the other two
       procedures CSR_BT_GATT_FIND_INCL_SERVICES_REQ and
       CSR_BT_GATT_DISCOVER_CHARAC_REQ never will exceed ATT default mtu (23)
       as CSR_BT_GATT_FIND_INCL_SERVICES_REQ return a 128 bit uuid and
       CSR_BT_GATT_DISCOVER_CHARAC_REQ reads the Characteristic Declaration
       which max length is CSR_BT_GATT_CHARAC_DECLARATION_MAX_LENGTH (19 octets) */
    ATT_READ_BY_TYPE_CFM_T *prim = (ATT_READ_BY_TYPE_CFM_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        CsrUint16 mtu = CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId);

        /* If we failed due to security, see if we can do anything
         * about this.  Func return TRUE if security was started and
         * no further handling should be performed at this time */
        if(CsrBtGattCheckSecurity(inst, prim->result, qElem))
        {
            return;
        }

        if(mtu > 0 && csrBtGattValidateQueueMsgState(prim->result, qElem->msgState))
        {
            CsrBtGattPrim type = *(CsrBtGattPrim *) qElem->gattMsg;

            switch (type)
            {
                case CSR_BT_GATT_FIND_INCL_SERVICES_REQ:
                {
                    if (csrBtGattAttReadByTypeFindInclServiceHandler(qElem, prim, btConnId))
                    { /* This procedure is finish. Start the next if any */
                        CsrBtGattQueueRestoreHandler(inst, qElem);
                    }
                    /* Else - Wait for another ATT_READ_BY_TYPE_CFM message
                       or is reading a 128-bit Bluetooth UUID */
                    break;
                }
                case CSR_BT_GATT_DISCOVER_CHARAC_REQ:
                {
                    CsrBtGattDiscoverCharacReq *reqMsg = (CsrBtGattDiscoverCharacReq *) qElem->gattMsg;

                    if (csrBtGattAttDiscoverCharacHandler(inst,
                                                          qElem,
                                                          reqMsg,
                                                          prim))
                    {
                        if (prim->result == ATT_RESULT_SUCCESS)
                        {

                            CsrUint32 characUuid[4] = {0x00002803, 0, 0, 0};

                            attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                                    prim->cid,
                                                    (CsrUint16)(prim->handle + 1),
                                                    reqMsg->endGroupHandle,
                                                    ATT_UUID16,
                                                    characUuid,
                                                    NULL);
                        }
                        /* Else - Wait for another ATT_READ_BY_TYPE_CFM message */
                    }
                    else
                    { /* This procedure is finish. Start the next if any */
                        CsrBtGattQueueRestoreHandler(inst, qElem);
                    }
                    break;
                }
                case CSR_BT_GATT_READ_BY_UUID_REQ:
                {
                    if (csrBtGattAttReadByTypeReadByUuidHandler(inst, qElem, prim, btConnId, mtu))
                    { /* This procedure is finish. Start the next if any */
                        CsrBtGattQueueRestoreHandler(inst, qElem);
                    }
                    /* Else - Wait for another ATT_READ_BY_TYPE_CFM message */
                    break;
                }
                default :
                {
                    CsrStateEventException("ATT_READ_BY_TYPE_CFM received with invalid request type ",
                                           CSR_BT_ATT_PRIM, prim->type, 0);
                }
            }
        }
        /* Else - The connection is invalid or this procedure has
           been cancelled. If cancelled, GATT shall wait for
           another ATT_READ_BY_TYPE_CFM message */
    }
    /* Else - ATT_READ_BY_TYPE_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttReadCfmHandler(GattMainInst *inst)
{
    ATT_READ_CFM_T *prim         = (ATT_READ_CFM_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        CsrUint16 mtu = CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId);

        /* If we failed due to security, see if we can do anything
         * about this.  Func return TRUE if security was started and
         * no further handling should be performed at this time */
        if(CsrBtGattCheckSecurity(inst, prim->result, qElem))
        {
            return;
        }

        if(mtu > 0)
        {
            if (qElem->msgState == CSR_BT_GATT_MSG_QUEUE_CANCELLED)
            { /* This running procedure has been cancelled. E.g.
                 either a CSR_BT_GATT_FIND_INCL_SERVICES_CFM or
                 a proper Read Cfm message were sent to the
                 application at the time GATT received
                 CSR_BT_GATT_CANCEL_REQ.

                 This procedure is finish. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, qElem);
            }
            else
            {
                CsrBtGattPrim type = *(CsrBtGattPrim *) qElem->gattMsg;

                switch (type)
                {
                    case CSR_BT_GATT_FIND_INCL_SERVICES_REQ:
                        {
                            if (csrBtGattAttReadCfmFindInclServiceHandler(qElem, prim, btConnId))
                            { /* This procedure is finish. Start the next if any */
                                CsrBtGattQueueRestoreHandler(inst, qElem);
                            }
                            /* Else - Looking for more included services */
                            break;
                        }
                    case CSR_BT_GATT_READ_REQ:
                        {
                            CsrBtGattReadReq *reqMsg = (CsrBtGattReadReq *) qElem->gattMsg;

                            if (prim->result == ATT_RESULT_SUCCESS &&
                                prim->size_value >= (mtu - CSR_BT_GATT_ATT_READ_HEADER_LENGTH))
                            {
                                qElem->data = (CsrUint8 *) CsrPmemZalloc(CSR_BT_ATT_MTU_MAX);
                                qElem->dataOffset = prim->size_value;
                                CsrMemCpy(qElem->data, prim->value, prim->size_value);
                                attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE,
                                                     prim->cid,
                                                     qElem->attrHandle,
                                                     qElem->dataOffset,
                                                     NULL);
                            }
                            else
                            {
                                CsrBtGattReadCfmHandler(reqMsg,
                                                        (CsrBtResultCode) prim->result,
                                                        CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                                        prim->size_value,
                                                        &prim->value);

                                /* This procedure is finish. Start the next if any */
                                CsrBtGattQueueRestoreHandler(inst, qElem);
                            }
                            break;
                        }
                    default :
                        {
                            CsrStateEventException("ATT_READ_CFM received with invalid request type ",
                                                   CSR_BT_ATT_PRIM, prim->type, 0);
                        }
                }
            }
        }
    }
    /* Else - ATT_READ_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttReadBlobCfmHandler(GattMainInst *inst)
{
    ATT_READ_BLOB_CFM_T *prim    = (ATT_READ_BLOB_CFM_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        CsrBtGattConnElement *conn = NULL;
        CsrUint16            mtu   = 0;

        /* If we failed due to security, see if we can do anything
         * about this.  Func return TRUE if security was started and
         * no further handling should be performed at this time */
        if(CsrBtGattCheckSecurity(inst, prim->result, qElem))
        {
            return;
        }

        if (qElem->gattId != inst->privateGattId)
        {
            mtu = CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId);
        }
        else
        {
            conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                                   &btConnId);

            if (conn)
            {
                mtu = conn->mtu;
            }
        }

        if(mtu > 0)
        {
            CsrBtGattPrim type = *(CsrBtGattPrim *) qElem->gattMsg;

            if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
            {
                if (prim->result == ATT_RESULT_SUCCESS)
                {
                    if (prim->value && qElem->dataOffset + prim->size_value < CSR_BT_ATT_MTU_MAX)
                    { /* size_value should never be larger then mtu - 1 Vol 3 part F sec 3.4.4.4 */
                        CsrMemCpy(&(qElem->data[qElem->dataOffset]), prim->value, prim->size_value);
                        qElem->dataOffset = (CsrUint16)(qElem->dataOffset + prim->size_value);
                    }


                    if (prim->result == ATT_RESULT_SUCCESS &&
                        prim->size_value >= (mtu - CSR_BT_GATT_ATT_READ_BLOB_HEADER_LENGTH))
                    {
                        CsrUint16 offset = 0;

                        if (type == CSR_BT_GATT_READ_REQ)
                        {
                            CsrBtGattReadReq *reqMsg = (CsrBtGattReadReq *) qElem->gattMsg;
                            offset = reqMsg->offset;
                        }
                        attlib_read_blob_req(CSR_BT_GATT_IFACEQUEUE,
                                             prim->cid,
                                             qElem->attrHandle,
                                             (CsrUint16)(qElem->dataOffset + offset),
                                             NULL);
                        return;
                    }
                }
                else if (prim->result == ATT_RESULT_INVALID_OFFSET &&
                         qElem->dataOffset > 0)
                { /* Offset specified was past the end of the long attribute */
                    prim->result = ATT_RESULT_SUCCESS;
                }
            }
            /* Else - The ongoing procedure has been cancelled */

            switch (type)
            {
                case CSR_BT_GATT_READ_REQ:
                {
                    if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
                    {
                        CsrBtGattReadCfmHandler((CsrBtGattReadReq *) qElem->gattMsg,
                                                (CsrBtResultCode) prim->result,
                                                CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                                (qElem->dataOffset),
                                                &qElem->data);
                    }
                    /* Else - This procedure has been cancelled. E.g. the
                       proper Read Cfm message was sent to the
                       application at the time GATT received
                       CSR_BT_GATT_CANCEL_REQ.  */

                    /* This procedure is finish. Start the next if any */
                    CsrBtGattQueueRestoreHandler(inst, qElem);
                    break;
                }
                case CSR_BT_GATT_READ_BY_UUID_REQ:
                {
                    if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
                    {
                        CsrBtGattReadByUuidReq *reqMsg = (CsrBtGattReadByUuidReq *) qElem->gattMsg;

                        if (prim->result == ATT_RESULT_SUCCESS)
                        {
                            if (reqMsg->uuid.length == CSR_BT_UUID16_SIZE &&
                                CSR_GET_UINT16_FROM_LITTLE_ENDIAN(reqMsg->uuid.uuid) == CSR_BT_GATT_DEVICE_NAME_UUID)
                            { /* The Remote Name Characteristic Value has been read, Update saved named */
                                CSR_BT_GATT_CONN_INST_UPDATE_REMOTE_NAME(inst->connInst,
                                                                        btConnId,
                                                                        qElem->dataOffset,
                                                                        qElem->data);  
                            }
                    
                            if (qElem->gattId != inst->privateGattId)
                            {
                                qElem->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK;
                                CsrBtGattReadByUuidIndSend(qElem->gattId,
                                                           btConnId,
                                                           qElem->attrHandle,
                                                           qElem->dataOffset,
                                                           qElem->data);
                            }
                            else
                            { /* GATT have called ATT_READ_BY_TYPE_REQ 
                                 in order to to Read the remote name 
                                 Characteristic Value. Note in this process 
                                 ATT_READ_BLOB_REQ were called in order 
                                 to received a complete long name */
                                if (qElem->dataElemIndex > 0)
                                { /* SC has requested to read the name, 
                                     before before GATT had time to read it. 
                                     Send Result to SC */
                                    CsrBtGattId gattId = CSR_BT_GATT_CREATE_GATT_ID(qElem->dataElemIndex, 
                                                                                    CSR_BT_SC_IFACEQUEUE);
                                    CsrBtGattReadByUuidIndSend(gattId,
                                                               btConnId,
                                                               qElem->attrHandle,
                                                               qElem->dataOffset,
                                                               qElem->data);
                                }
                            }
                        }
                        else
                        {
                            CsrPmemFree(qElem->data);
                        }
                        qElem->data = NULL;

                        if (qElem->gattId != inst->privateGattId                   && 
                            qElem->attrHandle + 1 <= reqMsg->endGroupHandle        &&
                            prim->result != ATT_RESULT_INSUFFICIENT_AUTHENTICATION &&
                            prim->result != ATT_RESULT_INSUFFICIENT_ENCRYPTION     &&
                            prim->result != ATT_RESULT_SIGN_FAILED)
                        { /* GATT shall only continue if read blob do not failed 
                             for security reasons or if GATT itself have tried to
                             read the remote name Characteristic Value. 
                             Note, GATT will try to run the security one time per handle. 
                             This is handle by the function CsrBtGattCheckSecurity. Also
                             note that if GATT itself is trying to read the Read Name
                             it do not need to continue as a peer device only shall 
                             have one of these Characteristic */
                            CsrUint32       attUuid[4];
                            att_uuid_type_t uuidType;
                            CsrBtGattGetAttUuid(reqMsg->uuid, attUuid, &uuidType);
                            /* Note, in order to be able to handle that the server requires 
                               security on the next handle being read, GATT shall set the 
                               security callback function again because the function
                               csrBtGattScLeSecurityCfmHandler set it to NULL and GATT 
                               shall allow one security procedure per handle being read. 
                               The qElem->attrHandle is set to
                               qElem->attrHandle + 1 so GATT is able replay this command in 
                               case of security failure */
                            qElem->attrHandle++;
                            qElem->securityFunc = CsrBtGattReadByUuidSecurityHandler;
                            attlib_read_by_type_req(CSR_BT_GATT_IFACEQUEUE,
                                                    prim->cid,
                                                    qElem->attrHandle,
                                                    reqMsg->endGroupHandle,
                                                    uuidType,
                                                    attUuid,
                                                    NULL);
                        }
                        else
                        {
                            if (qElem->gattId != inst->privateGattId)
                            {
                                CsrBtResultCode resultCode;
                                CsrBtSupplier   resultSupplier;

                                /* No need to check the return value of
                                   csrBtGattGetResultCode as it is sent
                                   direct to the application */
                                (void)(csrBtGattGetResultCode(qElem->msgState,
                                                              prim->result,
                                                              &resultCode,
                                                              &resultSupplier));

                                CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_READ_BY_UUID_CFM,
                                                            qElem->gattId,
                                                            resultCode,
                                                            resultSupplier,
                                                            btConnId);
                            }
                            else
                            { /* GATT have called ATT_READ_BY_TYPE_REQ 
                                 in order to to Read the remote name 
                                 Characteristic Value. Note in this process 
                                 ATT_READ_BLOB_REQ were called in order 
                                 to received a complete name */
                                if (qElem->dataElemIndex > 0)
                                { /* SC has requested to read the name, 
                                     before before GATT had time to read it. 
                                     Send Result to SC */
                                    CsrBtGattId gattId = CSR_BT_GATT_CREATE_GATT_ID(qElem->dataElemIndex, 
                                                                                    CSR_BT_SC_IFACEQUEUE);

                                    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_READ_BY_UUID_CFM,
                                                                gattId,
                                                                (CsrBtResultCode) prim->result,
                                                                CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                                                btConnId);
                                }
                            }

                            /* This procedure is finish. Start the next if any */
                            CsrBtGattQueueRestoreHandler(inst, qElem);
                        }
                    }
                    else
                    { /* This procedure has been cancelled. E.g.
                         CSR_BT_GATT_READ_BY_UUID_CFM was sent
                         to the application at the time GATT received
                         CSR_BT_GATT_CANCEL_REQ.
                         This procedure is finish. Start the next if any */
                        CsrBtGattQueueRestoreHandler(inst, qElem);
                    }
                    break;
                }
                case CSR_BT_GATT_EVENT_SEND_REQ:
                {
                    /* Read Blob was called by GATT itself after it
                     * received a ATT_HANDLE_VALUE_IND message with a
                     * long attribute. Note this procedure cannot be
                     * cancelled by the application */
                    if (conn && prim->result == ATT_RESULT_SUCCESS)
                    {
                        /* Send result to the application */
                        CsrBtGattSubscribeFindIds ids;

                        ids.address       = conn->peerAddr;
                        ids.connInfo      = (CsrBtGattConnInfo)(L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                                                                ? CSR_BT_GATT_CONNINFO_LE
                                                                : CSR_BT_GATT_CONNINFO_BREDR);
                        ids.attrHandle    = qElem->attrHandle;
                        ids.valueLength   = qElem->dataOffset;
                        ids.value         = qElem->data;
                        ids.btConnId      = btConnId;
                        ids.configuration = CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT;
                        ids.privateGattId = inst->privateGattId;
                        CSR_BT_GATT_APP_INST_ITERATE(inst->appInst,
                                                     csrBtGattAttValueIndSendHandler,
                                                     &ids);
                    }
                    /* This procedure is finished. Start the next if any */
                    CsrBtGattQueueRestoreHandler(inst, qElem);
                    break;
                }
                default:
                {
                    CsrStateEventException("ATT_READ_BLOB_CFM received with invalid request type ",
                                           CSR_BT_ATT_PRIM, prim->type, 0);
                }
            }
        }
    }
    /* Else - ATT_READ_BLOB_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttReadMultiCfmHandler(GattMainInst *inst)
{
    ATT_READ_MULTI_CFM_T *prim   = (ATT_READ_MULTI_CFM_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        /* If we failed due to security, see if we can do anything
         * about this.  Func return TRUE if security was started and
         * no further handling should be performed at this time */
        if(CsrBtGattCheckSecurity(inst, prim->result, qElem))
        {
            return;
        }

        if(CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId) > 0)
        {
            if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
            {
                CsrBtGattStdReadCfmSend(CSR_BT_GATT_READ_MULTI_CFM,
                                        qElem->gattId,
                                        prim->result,
                                        CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                        btConnId,
                                        prim->size_value,
                                        prim->value);

                prim->value = NULL;
            }
            /* Else -This procedure has been cancelled. E.g.
               CSR_BT_GATT_READ_MULTI_CFM was sent
               to the application at the time GATT received
               CSR_BT_GATT_CANCEL_REQ. */

            /* This procedure is finish. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, qElem);
        }
    }
    /* Else - ATT_READ_MULTI_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttReadByGroupTypeCfmHandler(GattMainInst *inst)
{
    /* Note according to ATT spec the Read Blob Request would be used to
       read the remaining octets of a long attribute value. GATT do not
       use Read blob because ATT_READ_BY_GROUP_TYPE_REQ only are used
       to Discover/Read Primary Services (Service Declarations) which
       means that the value never will exceed ATT default MTU (23).
       Also note that GATT do not handle security because the server 
       shall not require authentication or authorisation when the GATT
       client reads Primary Services */
    ATT_READ_BY_GROUP_TYPE_CFM_T *prim  = (ATT_READ_BY_GROUP_TYPE_CFM_T *) inst->msg;
    CsrBtConnId btConnId                = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem        = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        if(CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId) > 0 &&
           csrBtGattValidateQueueMsgState(prim->result, qElem->msgState))
        {
            if (csrBtGattAttPrimaryServiceHandler(qElem,
                                                  btConnId,
                                                  prim->result,
                                                  prim->handle,
                                                  prim->end,
                                                  prim->size_value,
                                                  prim->value))
            {
                if (prim->result == ATT_RESULT_SUCCESS)
                {
                    CsrUint32 primaryService[4] = {0x0002800, 0, 0, 0};

                    attlib_read_by_group_type_req(CSR_BT_GATT_IFACEQUEUE,
                                                  prim->cid,
                                                  (CsrUint16)(prim->end + 1),
                                                  CSR_BT_GATT_ATTR_HANDLE_MAX,
                                                  ATT_UUID16,
                                                  primaryService,
                                                  NULL);
                }
                /* Else - Wait for another ATT_READ_BY_GROUP_TYPE_CFM message */
            }
            else
            { /* This procedure is finish. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, qElem);
            }
        }
        /* Else - The connection is invalid or this procedure has
           been cancelled. If cancelled, GATT shall wait for
           another ATT_READ_BY_GROUP_TYPE_CFM message */
    }
    /* Else - ATT_READ_BY_GROUP_TYPE_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttWriteCfmHandler(GattMainInst *inst)
{
    ATT_WRITE_CFM_T *prim           = (ATT_WRITE_CFM_T *) inst->msg;
    CsrBtConnId btConnId            = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem    = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        /* If we failed due to security, see if we can do anything
         * about this.  Func return TRUE if security was started and
         * no further handling should be performed at this time */
        if(CsrBtGattCheckSecurity(inst, prim->result, qElem))
        {
            return;
        }

        if(CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId) > 0)
        {
            if (CSR_BT_GATT_GET_QID_FROM_GATT_ID(qElem->gattId) == CSR_BT_GATT_IFACEQUEUE)
            { /* Gatt has updated the Device Name Characteristic entry, because the
                 local name has change. Update the advertise parameters  */
                CsrBtGattUpdateAdvertise(inst);
            }
            else
            {
                CsrBtGattPrim type = *(CsrBtGattPrim *) qElem->gattMsg;

                if (prim->result == ATT_RESULT_SUCCESS &&
                    type == CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ)
                {
                    CsrBtGattAddOrRemoveSubscribeElement(inst,
                                                         (CsrBtGattWriteClientConfigurationReq*)qElem->gattMsg);
                }

                CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                            qElem->gattId,
                                            (CsrBtResultCode) prim->result,
                                            CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                            btConnId);

                /* This procedure is finish. Start the next if any */
                CsrBtGattQueueRestoreHandler(inst, qElem);
            }
        }
    }
    /* Else - ATT_WRITE_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttPrepareWriteCfmHandler(GattMainInst *inst)
{
    ATT_PREPARE_WRITE_CFM_T *prim   = (ATT_PREPARE_WRITE_CFM_T *) inst->msg;
    CsrBtConnId btConnId            = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem    = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        CsrUint16 mtu = CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId);

        /* If we failed due to security, see if we can do anything
         * about this.  Func return TRUE if security was started and
         * no further handling should be performed at this time */
        if(CsrBtGattCheckSecurity(inst, prim->result, qElem))
        {
            return;
        }

        if (mtu > 0)
        {
            if (prim->result == ATT_RESULT_SUCCESS)
            {
                if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
                {
                    CsrBtGattWriteReq *reqMsg = (CsrBtGattWriteReq *) qElem->gattMsg;
                    CsrBtGattAttrWritePairs *attrPair = &(reqMsg->attrWritePairs[qElem->dataElemIndex]);

                    if (qElem->dataOffset + prim->size_value <= attrPair->valueLength &&
                        csrBtGattValidatePrepareWriteData(reqMsg->flags,
                                                          prim,
                                                          &(attrPair->value[qElem->dataOffset])))
                    {
                        qElem->dataOffset = (CsrUint16)(qElem->dataOffset + prim->size_value);

                        if (qElem->dataOffset < attrPair->valueLength)
                        { /* Send the next piece of the attribute value */
                            CsrBtGattGetAttPrepareWriteSend(qElem,
                                                            (CsrUint16)(attrPair->offset + qElem->dataOffset),
                                                            mtu,
                                                            attrPair->valueLength,
                                                            attrPair->value);
                        }
                        else
                        { /* One entire attribute value is sent */
                            qElem->dataElemIndex++;


                            if (qElem->dataElemIndex < reqMsg->attrWritePairsCount)
                            { /* Start sending the next attribute value. 
                                 Note, in order to be able to handle that the server requires 
                                 security on the next handle being written, GATT shall set the 
                                 security callback function again, because the function 
                                 csrBtGattScLeSecurityCfmHandler set it to NULL and GATT 
                                 shall allow one security procedure per handle being written. */
                                attrPair              = &(reqMsg->attrWritePairs[qElem->dataElemIndex]);
                                qElem->attrHandle     = attrPair->attrHandle;
                                qElem->dataOffset     = 0;
                                qElem->securityFunc   = CsrBtGattPrepareWriteSecurityHandler;
                                CsrBtGattGetAttPrepareWriteSend(qElem,
                                                                attrPair->offset,
                                                                mtu,
                                                                attrPair->valueLength,
                                                                attrPair->value);
                            }
                            else
                            { /* All attribute values are sent - Execute.
                                 Note set cancelFunc to NULL because when
                                 attlib_execute_write_req is called a write
                                 procedure cannot be cancel */
                                qElem->cancelFunc = NULL;
                                attlib_execute_write_req(CSR_BT_GATT_IFACEQUEUE,
                                                         prim->cid,
                                                         ATT_EXECUTE_WRITE,
                                                         NULL);
                            }
                        }
                    }
                    else
                    { /* The data received is not the same data as sent. Cancel
                         the attlib_prepare_write_req procedure and set cancelFunc
                         to NULL because when attlib_execute_write_req is called
                         a write procedure cannot be cancel */
                        qElem->msgState   = CSR_BT_GATT_MSG_QUEUE_EXECUTE_WRITE_CANCEL;
                        qElem->cancelFunc = NULL;
                        attlib_execute_write_req(CSR_BT_GATT_IFACEQUEUE,
                                                 prim->cid,
                                                 ATT_EXECUTE_CANCEL,
                                                 NULL);
                    }
                }
                else
                { /* This procedure has been cancel by the application */
                    attlib_execute_write_req(CSR_BT_GATT_IFACEQUEUE,
                                             prim->cid,
                                             ATT_EXECUTE_CANCEL,
                                             NULL);
                }
            }
            else
            {
                if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
                {
                    CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                                qElem->gattId,
                                                (CsrBtResultCode) prim->result,
                                                CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                                btConnId);
                }
                /* Else -This procedure has been cancelled. E.g.
                   CSR_BT_GATT_WRITE_CFM was sent
                   to the application at the time GATT received
                   CSR_BT_GATT_CANCEL_REQ. */

                if (qElem->dataElemIndex > 0 || qElem->dataOffset > 0)
                { /* At least one ATT_PREPARE_WRITE_CFM with success has been 
                     received. Set qElem->msgState to Cancelled in order
                     to make sure that CSR_BT_GATT_WRITE_CFM is not
                     sent to the application twice and set qElem->cancelFunc 
                     to NULL in order to make sure that this procedure cannot 
                     be cancel twice */
                    qElem->cancelFunc = NULL;
                    qElem->msgState   = CSR_BT_GATT_MSG_QUEUE_CANCELLED;
                    attlib_execute_write_req(CSR_BT_GATT_IFACEQUEUE,
                                             prim->cid,
                                             ATT_EXECUTE_CANCEL,
                                             NULL);
                }
                else
                { /* GATT shall not issue a ATT_EXECUTE_WRITE_REQ because
                     it have not received any ATT_PREPARE_WRITE_CFM messages
                     with success. This procedure is finish. Start the next if any */
                    CsrBtGattQueueRestoreHandler(inst, qElem);
                }
            }
        }
    }
    /* Else - ATT_PREPARE_WRITE_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttExecuteWriteCfmHandler(GattMainInst *inst)
{
    ATT_EXECUTE_WRITE_CFM_T *prim  = (ATT_EXECUTE_WRITE_CFM_T *) inst->msg;
    CsrBtConnId btConnId           = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattQueueElement *qElem   = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

    if (qElem && qElem->gattMsg)
    {
        if(CsrBtGattValidateBtConnIdByMtu(inst, qElem->gattId, btConnId) > 0)
        {
            if (qElem->msgState != CSR_BT_GATT_MSG_QUEUE_CANCELLED)
            {
                CsrBtResultCode resultCode;
                CsrBtSupplier   resultSupplier = CSR_BT_SUPPLIER_GATT;

                if (qElem->msgState == CSR_BT_GATT_MSG_QUEUE_EXECUTE_WRITE_CANCEL)
                {
                    resultCode     = CSR_BT_GATT_RESULT_RELIABLE_WRITE_VALIDATION_ERROR;
                    resultSupplier = CSR_BT_SUPPLIER_GATT;
                }
                else
                {
                    resultCode     = (CsrBtResultCode) prim->result;
                    resultSupplier = CSR_BT_GATT_GET_SUPPLIER(prim->result);
                }

                CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_WRITE_CFM,
                                            qElem->gattId,
                                            resultCode,
                                            resultSupplier,
                                            btConnId);
            }
            /* Else - This procedure has been cancelled or GATT has 
                      received a ATT_PREPARE_WRITE_CFM with error after
                      it has received at least one successful ATT_PREPARE_WRITE_CFM.
                      E.g. CSR_BT_GATT_WRITE_CFM was sent to the application at the 
                      time GATT received CSR_BT_GATT_CANCEL_REQ or at the time it 
                      received an ATT_PREPARE_WRITE_CFM message with error */

            /* This procedure is finish. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, qElem);
        }
    }
    /* Else - ATT_EXECUTE_WRITE_REQ and ATT_DISCONNECT_IND has cross */
}

void CsrBtGattAttHandleValueCfmHandler(GattMainInst *inst)
{
    ATT_HANDLE_VALUE_CFM_T *prim  = (ATT_HANDLE_VALUE_CFM_T *) inst->msg;

    if (prim->result != ATT_RESULT_SUCCESS_SENT)
    {
        CsrBtConnId btConnId          = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
        CsrBtGattQueueElement *qElem  = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

        if (qElem && qElem->gattMsg)
        {
            CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_EVENT_SEND_CFM,
                                        qElem->gattId,
                                        prim->result,
                                        CSR_BT_GATT_GET_SUPPLIER(prim->result),
                                        btConnId);

            /* This procedure is finish. Start the next if any */
            CsrBtGattQueueRestoreHandler(inst, qElem);
        }
        /* Else - ATT_HANDLE_VALUE_REQ and ATT_DISCONNECT_IND has cross */
    }
    /* Else - Indication sent, awaiting confirmation from the client */
}

void CsrBtGattAttHandleValueIndHandler(GattMainInst *inst)
{
    ATT_HANDLE_VALUE_IND_T *prim = (ATT_HANDLE_VALUE_IND_T *) inst->msg;
    CsrBtConnId btConnId         = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    CsrBtGattConnElement   *conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                                                   &btConnId);

    if ((prim->flags & ATT_HANDLE_VALUE_INDICATION) == ATT_HANDLE_VALUE_INDICATION)
    {
        /* The peer server has sent an indication to the local
         * client.  E.g. the Client shall response this message */
        attlib_handle_value_rsp(CSR_BT_GATT_IFACEQUEUE, prim->cid, NULL);
    }
    /* Else - The peer server has sent a notification to the local
       client.  E.g. the Client shall not response this
       message */

    if(conn)
    {
        if (prim->size_value >= (conn->mtu - CSR_BT_GATT_ATT_NOTIFICATION_HEADER_LENGTH))
        { /* Note: for a client to get a long attribute, it must
             use ATT_READ_BLOB_REQ. GATT are using
             CsrBtGattEventSendReq message to trigger the
             ATT_READ_BLOB_REQ message as it must be sure
             that it is placed on the GATT queue system in
             order to prevent that two Request are sent to
             the server at the same time */
            CsrBtGattEventSendReq * newMsg = CsrBtGattEventSendReq_struct(inst->privateGattId,
                                                                          btConnId,
                                                                          prim->handle,
                                                                          CSR_BT_GATT_ATTR_HANDLE_INVALID,
                                                                          CSR_BT_GATT_LONG_ATTRIBUTE_EVENT,
                                                                          prim->size_value,
                                                                          prim->value);
            prim->value = NULL;
            attlib_free((ATT_UPRIM_T *) inst->msg);
            inst->msg   = newMsg;
            CsrBtGattEventSendReqHandler(inst);
        }
        else
        {
            /* Send result to the application */
            CsrBtGattSubscribeFindIds ids;

            ids.address       = conn->peerAddr;
            ids.connInfo      = CsrBtGattGetConnInfo(inst, inst->privateGattId,
                                                     btConnId);
            ids.attrHandle    = prim->handle;
            ids.valueLength   = prim->size_value;
            ids.value         = prim->value;
            ids.btConnId      = btConnId;
            ids.configuration = CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT;
            ids.privateGattId = inst->privateGattId;

            CSR_BT_GATT_APP_INST_ITERATE(inst->appInst,
                                         csrBtGattAttValueIndSendHandler,
                                         &ids);

            if (prim->size_value == CSR_BT_GATT_SERVICE_CHANGED_LENGTH)
            {
                /* This may be a Service Changed */
                CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst,
                                                                                    &inst->privateGattId);

                if (appElement)
                {
                    CsrBtGattSubscribeElement *subElem = CSR_BT_GATT_SUBSCRIBE_INST_FIND_SERVICE_CHANGED(appElement,
                                                                                                         &conn->peerAddr);
                    if (subElem && subElem->attrHandle == prim->handle)
                    {
                        /* A Service Change it received. Inform all
                         * applications.  Note, if the affected
                         * attribute handle range includes some of the
                         * handles the application has subscribe for
                         * these shall be remove as there are not
                         * valid anymore */
                        CSR_BT_GATT_APP_INST_ITERATE(inst->appInst,
                                                     csrBtGattServiceChangedIndSendHandler,
                                                     &ids);
                    }
                    /* Else - GATT do know this handle, e.g it is not a Service Change */
                }
                else
                {
                    CsrStateEventException("ATT_HANDLE_VALUE_IND no appElement",
                                           CSR_BT_ATT_PRIM, prim->type, 0);
                }

            }
            /* Else - Cannot be the handle of a Service Changed it is always
               CSR_BT_GATT_SERVICE_CHANGED_LENGTH (0x04) byte long */
        }
    }
}

static CsrBtGattAccessCheck csrBtGattGetAccessCheckValue(CsrUint16 flags)
{
    CsrBtGattAccessCheck check = 0;

    check |= (CsrBtGattAccessCheck)(flags & ATT_ACCESS_PERMISSION
                                    ? CSR_BT_GATT_ACCESS_CHECK_AUTHORISATION
                                    : 0);
    check |= (CsrBtGattAccessCheck)(flags & ATT_ACCESS_ENCRYPTION_KEY_LEN
                                    ? CSR_BT_GATT_ACCESS_CHECK_ENCR_KEY_SIZE
                                    : 0);
    return check;
}

void CsrBtGattAttAccessIndHandler(GattMainInst *inst)
{
    ATT_ACCESS_IND_T *prim;
    CsrBtGattAppElement *appElement;
    CsrBtConnId btConnId;
    CsrBtGattConnElement *conn;
    CsrUint16 mtu;
    CsrBtTypedAddr address;
    CsrBtGattConnInfo connInfo;

    prim = (ATT_ACCESS_IND_T *) inst->msg;
    appElement = CSR_BT_GATT_APP_INST_FIND_ATTR_HANDLE(inst->appInst,
                                                       &(prim->handle));
    btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid);
    conn = NULL;

    /* Use application's own connection if possible. If not, pick
     * an open connection */
    if(appElement)
    {
        CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                   &conn, btConnId,
                                                   appElement->gattId);
    }
    if (conn == NULL)
    {
        conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                               &btConnId);
    }

    if(conn != NULL)
    {
        mtu = conn->mtu;
        address = conn->peerAddr;
        connInfo = (CsrBtGattConnInfo)(L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                                       ? CSR_BT_GATT_CONNINFO_LE
                                       : CSR_BT_GATT_CONNINFO_BREDR);
    }
    else
    {
        mtu = CSR_BT_ATT_MTU_DEFAULT;
        CsrBtAddrZero(&address);
        connInfo = CSR_BT_GATT_CONNINFO_LE;
    }

    /* Note for write access indications: Interleaving of
     * prepare/execute and other commands is not possible as ATT will
     * buffer up everything until the execute arrives (except for
     * security, which is always served immediately) -- and nothing
     * gets sent until we have responded to the execute */

    /* Send stuff to application for which this handle belongs */
    if (appElement
        && (prim->flags & ATT_ACCESS_READ))
    {
        CsrBtGattAccessReadIndSend(CSR_BT_GATT_GET_QID_FROM_GATT_ID(appElement->gattId),
                                   appElement->gattId,
                                   btConnId,
                                   prim->handle,
                                   prim->offset,
                                   (CsrUint16)(mtu - CSR_BT_GATT_ATT_READ_HEADER_LENGTH),
                                   csrBtGattGetAccessCheckValue(prim->flags),
                                   connInfo,
                                   address);

        /* All good, bail out */
        return;
    }
    /* If any of the security-ish flags are set, we need to run the
     * access ind/rsp immediately */
    else if (appElement
             && ((prim->flags & ATT_ACCESS_PERMISSION)
                 || (prim->flags & ATT_ACCESS_ENCRYPTION_KEY_LEN)
                 || (prim->size_value == 0 && prim->value == NULL && ((prim->flags & ATT_ACCESS_WRITE_COMPLETE) == 0))))
    {
        CsrBtGattAttrWritePairs *unit;

        if (prim->size_value > 0)
        {
            unit = CsrPmemAlloc(sizeof(CsrBtGattAttrWritePairs));
            unit->attrHandle = prim->handle;
            unit->offset = prim->offset;
            unit->valueLength = prim->size_value;
            unit->value = prim->value;
            /* Ownership of data transferrred to 'unit' */
            prim->value = NULL;
            prim->size_value = 0;
        }
        else
        {
            unit = NULL;
        }

        CsrBtGattAccessWriteIndSend(CSR_BT_GATT_GET_QID_FROM_GATT_ID(appElement->gattId),
                                    appElement->gattId,
                                    CSR_BT_GATT_CREATE_LE_CONN_ID(prim->cid),
                                    csrBtGattGetAccessCheckValue(prim->flags),
                                    connInfo,
                                    address,
                                    (CsrUint16)(unit != NULL ? 1 : 0), /* 1 or 0 units */
                                    unit,
                                    prim->handle);

    }
    /* Prepare write. Buffer it up */
    else if (appElement
             && (prim->flags & ATT_ACCESS_WRITE))
    {
        /* Add to prepare buffer */
        CsrBtGattPrepareBuffer *buf;
        buf = CSR_BT_GATT_PREPARE_INST_ADD_LAST(inst->prepare);
        buf->cid = prim->cid;
        buf->handle = prim->handle;
        buf->state = CSR_BT_GATT_PREPEXEC_NEW;
        buf->offset = prim->offset;
        buf->dataLength = prim->size_value;
        buf->data = prim->value;

        /* Data consumed */
        prim->value = NULL;
        prim->size_value = 0;
    }
    /* Handle not owned by any application we know of */
    else if ((prim->handle != CSR_BT_GATT_ATTR_HANDLE_INVALID) && ((prim->flags & ATT_ACCESS_WRITE_COMPLETE) == 0))
    {
        attlib_access_rsp(CSR_BT_GATT_IFACEQUEUE,
                          prim->cid,
                          prim->handle,
                          ATT_RESULT_ATTR_NOT_FOUND,
                          0,
                          NULL,
                          NULL);
    }

    /* Empty prepared write buffer */
    if (prim->flags & ATT_ACCESS_WRITE_COMPLETE)
    {
        /* Mark all prepared writes ready for execute accessment. This
         * is application-ignorant, and the 'handle' may even be set
         * to zero */
        CsrBtGattPrepareBuffer *buf;
        for(buf = CSR_BT_GATT_PREPARE_INST_GET_FIRST(inst->prepare);
            buf != NULL;
            buf = buf->next)
        {
            if(buf->cid == prim->cid)
            {
                buf->state = CSR_BT_GATT_PREPEXEC_EXECUTE;
            }
        }

        /* Find and collect handles */
        if (CsrBtGattPlayExecuteWrite(inst, prim->cid) == FALSE)
        {
            /* Nothing to send. Finalise */
            CsrBtGattPlayExecuteFinalise(inst, prim->cid);
        }
    }
}

