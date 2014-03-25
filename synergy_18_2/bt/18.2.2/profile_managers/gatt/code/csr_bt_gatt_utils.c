/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2009-2012. All rights reserved.

REVISION:      $Revision: #6 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_private.h"

static CsrBool csrBtGattGetUuidData(CsrUint32 dataLen,
                                    CsrUint8  *data,
                                    CsrUint8  **uuidData,
                                    CsrUint32 *uuidSize)
{
    CsrUint8  type;
    CsrUint32 size, readBytes = 0,  read = 0;
    CsrUint16 index = 0;

    while (read < dataLen)
    {
        if(CsrBtSdcGetTypeDescriptorInsideList((CsrUint16)dataLen, data, index, &type, &size, &readBytes))
        {
            if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
            {
                readBytes += size;
            }
            if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
            {
                if (dataLen >= (size + index + 1))
                {
                    *uuidData = data + index + 1;
                    *uuidSize = size;
                    return TRUE;
                }
            }
            index += (CsrUint16)readBytes;
            read += readBytes;
        }
    }     
    return FALSE;
}
                                

static CsrBool csrBtGattGetServiceClassId(CmnCsrBtLinkedListStruct *sdpTagList,
                                          CsrUint16                i,
                                          CsrBtUuid                *uuid)
{
    CsrUint32 dataLen;
    CsrUint8  *data;
            
    if (CsrBtUtilSdrGetDataElementSequenceDataAttributeFromAttributeUuid(sdpTagList, 
                                                                         i,
                                                                         CSR_BT_SERVICE_CLASS_ID_LIST, 
                                                                         &data, 
                                                                         &dataLen))
    {
        CsrUint8 *uuidData;
        CsrUint32 uuidSize;

        if (csrBtGattGetUuidData(dataLen, data, &uuidData, &uuidSize))
        {
            CsrBtUuid tmp;

            if (uuidSize == CSR_BT_UUID16_SIZE)
            {
                tmp.length  = CSR_BT_UUID16_SIZE;
                CsrMemSet(tmp.uuid, 0, CSR_BT_UUID128_SIZE);
                tmp.uuid[0] = uuidData[1];
                tmp.uuid[1] = uuidData[0];
                *uuid = tmp;
                return TRUE;
            }
            else if (uuidSize == CSR_BT_UUID128_SIZE)
            {
                CsrUint16 i;
                tmp.length = CSR_BT_UUID128_SIZE;
                for (i = 0; i < CSR_BT_UUID128_SIZE; i++)
                {
                    tmp.uuid[i] = uuidData[CSR_BT_UUID128_SIZE - 1 - i];
                }
                *uuid = tmp;
                return TRUE;
            }
            /* Else - Ignore only looking for 16 or 128 bit services */
        }
    }
    return FALSE;
}

static CsrBool csrBtGattGetServiceFoundBySdp(CmnCsrBtLinkedListStruct *sdpTagList,
                                             CsrUint16                i,
                                             CsrBtUuid                *uuid,
                                             CsrUint16                *startHandle,
                                             CsrUint16                *endHandle)
{

*startHandle = CSR_BT_GATT_ATTR_HANDLE_INVALID;
*endHandle  = CSR_BT_GATT_ATTR_HANDLE_INVALID;

    if (csrBtGattGetServiceClassId(sdpTagList, i, uuid))
    {
        CsrUint32 dataLen;
        CsrUint8  *data;

        if (CsrBtUtilSdrGetDataElementSequenceDataAttributeFromAttributeUuid(sdpTagList, 
                                                                             i,
                                                                             CSR_BT_PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER, 
                                                                             &data, 
                                                                             &dataLen))
        {
            CsrUint8  type;
            CsrUint32 size, readBytes = 0, read = 0;
            CsrUint16 index = 0;
            CsrBool   count = 0; 

            while (read < dataLen)
            {
                if(CsrBtSdcGetTypeDescriptorInsideList((CsrUint16)dataLen, data, index, &type, &size, &readBytes))
                {
                    if (type != CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
                    {
                        readBytes += size;
                    }

                    if (type == CSR_BT_DATA_ELEMENT_UUID_TYPE)
                    {
                        CsrUint16 value;
                        CsrUint32 newIndex = index;

                        if (count > 0)
                        {
                            return FALSE;
                        }

                        if (size != 2)
                        {
                            newIndex += 2;
                        }

                        if (CsrBtSdcGetUint16Value((CsrUint16)dataLen, data, newIndex, &value))
                        {
                            if (value == CSR_BT_ATT_PROTOCOL_UUID16_IDENTIFIER && count == 0)
                            {
                                count = 1;
                            }
                        }
                    }

                    if (count > 0 && type == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE && size == 2)
                    {
                        CsrUint16 tmp;
                        
                        if (CsrBtSdcGetUint16Value((CsrUint16)dataLen, data, index, &tmp))
                        {
                            if (count == 1)
                            {
                                *startHandle = tmp;
                                count++;
                            }
                            else
                            {
                                *endHandle = tmp;
                                return TRUE;
                            }
                        }
                    }
                    index += (CsrUint16)readBytes;
                    read += readBytes;
                }
            }
        }
    }
    return FALSE;
}

static void csrBtGattDiscoverServicesBySdpResultHandler(void                     *inst,
                                                        CmnCsrBtLinkedListStruct *sdpTagList,
                                                        CsrBtDeviceAddr          deviceAddr,
                                                        CsrBtResultCode          resultCode,
                                                        CsrBtSupplier            resultSupplier)
{
    GattMainInst *gattInst       = (GattMainInst *)inst;
    CsrBtGattQueueElement *qElem = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(gattInst->queue, &gattInst->sdpBtConnId);

    if (qElem && qElem->gattMsg)
    {
        if (qElem->msgState == CSR_BT_GATT_MSG_QUEUE_CANCELLED)
        { /* The procure is cancelled. E.g. the 
             CSR_BT_GATT_DISCOVER_SERVICES_CFM  message 
             was sent to the application at the time 
             GATT received CSR_BT_GATT_CANCEL_REQ.  */
            ;
        }
        else
        {
            if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                resultSupplier == CSR_BT_SUPPLIER_CM &&
                sdpTagList)
            {
                CsrBtResultCode gattResult           = CSR_BT_GATT_RESULT_BR_EDR_NO_PRIMARY_SERVICES_FOUND;
                CsrBtGattDiscoverServicesReq *reqMsg = (CsrBtGattDiscoverServicesReq *) qElem->gattMsg;
                
                CsrUint16 result, task, taskResult;
                CsrBtUuid32 service32;
                CsrUintFast16   i, numOfSdpRecords = CsrBtUtilBllGetNofEntriesEx(sdpTagList);
                
                
                for (i = 0; i < numOfSdpRecords; i++)
                {
                    if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, i, &service32, 
                                                              &result, &task, &taskResult) && 
                        result == SDR_SDC_SEARCH_SUCCESS)
                    {
                        CsrBtUuid       uuid;
                        CsrBtGattHandle startHandle;
                        CsrBtGattHandle endHandle;

                        if (csrBtGattGetServiceFoundBySdp(sdpTagList, (CsrUint16)i, &uuid, &startHandle, &endHandle))
                        {
                            CsrBool valid = FALSE;
                            
                            if (reqMsg->uuid.length == CSR_BT_UUID16_SIZE && 
                                CSR_BT_UUID_GET_16(reqMsg->uuid) == CSR_BT_GATT_PRIMARY_SERVICE_UUID)
                            {
                                /* Discover all primary services */
                                valid = TRUE;
                            }
                            else if (reqMsg->uuid.length == uuid.length &&
                                     !CsrMemCmp(reqMsg->uuid.uuid, uuid.uuid, uuid.length))
                            {
                                /* Discover primary services by UUID,
                                 * where the UUID do match. Send
                                 * result to the application */
                                valid = TRUE;
                            }

                            if (valid)
                            {
                                CsrBtGattDiscoverServicesInd *msg;
                                msg = (CsrBtGattDiscoverServicesInd*)CsrPmemAlloc(sizeof(CsrBtGattDiscoverServicesInd));
                                msg->type = CSR_BT_GATT_DISCOVER_SERVICES_IND;
                                msg->gattId = qElem->gattId;
                                msg->btConnId = qElem->btConnId;
                                msg->startHandle = (CsrBtGattHandle)startHandle;
                                msg->endHandle = endHandle;
                                msg->uuid = uuid;

                                CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(qElem->gattId),
                                                   msg);
                                gattResult = CSR_BT_GATT_RESULT_SUCCESS;
                            }                                
                        }
                    }
                }
                CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_SERVICES_CFM,
                                            qElem->gattId,
                                            gattResult,
                                            CSR_BT_SUPPLIER_GATT,
                                            qElem->btConnId);               
            }
            else
            { /* The SDP search failed */
                CsrBtGattStdBtConnIdCfmSend(CSR_BT_GATT_DISCOVER_SERVICES_CFM,
                                            qElem->gattId,
                                            resultCode,
                                            resultSupplier,
                                            qElem->btConnId);
            }
        }
        /* This procedure is finish. Start the next if any */
        CsrBtGattQueueRestoreHandler(inst, qElem);
    }
    CsrBtUtilBllFreeLinkedList(&sdpTagList, CsrBtUtilBllPfreeWrapper);
    gattInst->sdpBtConnId = CSR_BT_CONN_ID_INVALID;
}

static CsrBool csrBtGattPrepareIterateDeleteDone(CsrCmnListElm_t *elem, void *data)
{
    CsrBtGattPrepareBuffer *prep;
    CSR_UNUSED(data);
    prep = (CsrBtGattPrepareBuffer*)elem;

    /* Remove 0,0 or DONE elements */
    if ( ((prep->cid == 0) && (prep->handle == 0))
         || (prep->state == CSR_BT_GATT_PREPEXEC_DONE) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void csrBtGattInitPrepare(CsrCmnListElm_t *elem)
{
    CsrBtGattPrepareBuffer *prep;
    prep = (CsrBtGattPrepareBuffer*)elem;
    prep->cid = 0;
    prep->handle = 0;
    prep->state = CSR_BT_GATT_PREPEXEC_NEW;
    prep->offset = 0;
    prep->dataLength = 0;
    prep->data = NULL;
}

static void csrBtGattFreePrepare(CsrCmnListElm_t *elem)
{
    CsrBtGattPrepareBuffer *prep;
    prep = (CsrBtGattPrepareBuffer*)elem;
    CsrPmemFree(prep->data);
}

/* Find prepare write buffer to send to app. Return FALSE if none were
 * sent, otherwise TRUE */
CsrBool CsrBtGattPlayExecuteWrite(GattMainInst *inst, CsrUint16 cid)
{
    CsrBtGattPrepareBuffer *cur;
    CsrBtGattHandle handle;
    CsrBtGattHandle lastHandle;
    CsrUint16 unitCount;
    CsrBtGattAttrWritePairs *unit;
    CsrBtGattAppElement *app;
    CsrBool restart;
    CsrUint8 method;

    /* Figure out method */
    method = CSR_BT_GATT_PREPEXEC_ACCESS;
    for(cur = CSR_BT_GATT_PREPARE_INST_GET_FIRST(inst->prepare),
            handle = 0, lastHandle = 0;
        cur != NULL;
        cur = cur->next)
    {
        if (cur->cid == cid)
        {
            /* Detect if multiple handles are used */
            if ((lastHandle != 0) && cur->handle != lastHandle)
            {
                method = CSR_BT_GATT_PREPEXEC_ACCESS_AND_INFORM;
            }
            lastHandle = cur->handle;

            /* Select unprocessed target to use from here */
            if ((handle == 0)
                && (cur->state == CSR_BT_GATT_PREPEXEC_EXECUTE))
            {
                handle = cur->handle;
            }
        }
    }

    /* Could not find handle to process */
    if(handle == 0)
    {
        return FALSE;
    }

    /* Collect units to as few elements as possible */
    for(cur = CSR_BT_GATT_PREPARE_INST_GET_FIRST(inst->prepare);
        cur != NULL;
        cur = cur->next)
    {
        /* When an element has been found, scan the list again to find
         * subsequent prepared writes (ie. we do not trust the
         * offset/length ordering). Try to combine these with the
         * original element to reduce the number of items in the list
         * that we send upwards. */
        restart = TRUE;
        while (restart
               && (cur->cid == cid)
               && (cur->handle == handle)
               && (cur->state == CSR_BT_GATT_PREPEXEC_EXECUTE))
        {
            CsrBtGattPrepareBuffer *sub;

            /* Assume nothing matches */
            restart = FALSE;

            /* We will collect tailing elements in the current
             * element, so do not look at this element in this
             * collector loop again, i.e. mark as "access" or
             * "access+inform" */
            cur->state = method;

            /* Scan subelements, beginning at cur->next */
            for(sub = cur->next;
                sub != NULL;
                sub = sub->next)
            {
                if((cur->cid == sub->cid)
                   && (cur->handle == sub->handle)
                   && (sub->state == CSR_BT_GATT_PREPEXEC_EXECUTE))
                {
                    if( /* trivial case, offset continues at end of cur */
                        (sub->offset == (cur->offset + cur->dataLength + 1))
                        /* sub overwrites something in the middle of cur */
                        || ((sub->offset >= cur->offset)
                            && (sub->offset <= (cur->offset + cur->dataLength)))
                        /* sub prepends or overwrites start of cur */
                        || ((sub->offset < cur->offset)
                            && ((sub->offset + sub->dataLength + 1) >= cur->offset))
                        )
                    {
                        /* Fold 'sub' in to 'cur' */
                        CsrUint16 newoff;
                        CsrUint16 newend;
                        CsrUint8 *data;
                        
                        newoff = CSRMIN(cur->offset, sub->offset);
                        newend = CSRMAX((cur->offset + cur->dataLength),
                                        (sub->offset + sub->dataLength));
                        data = CsrPmemAlloc(newend - newoff);
                        
                        /* Copy in cur and sub at their respective
                         * relative new offset positions */
                        CsrMemCpy(data + cur->offset - newoff,
                                  cur->data, cur->dataLength);
                        CsrMemCpy(data + sub->offset - newoff,
                                  sub->data, sub->dataLength);
                        
                        /* Update 'cur' contents */
                        CsrPmemFree(cur->data);
                        cur->offset = newoff;
                        cur->data = data;
                        cur->dataLength = newend - newoff;

                        /* Invalidate 'sub' element */
                        CsrPmemFree(sub->data);
                        sub->data = NULL;
                        sub->dataLength = 0;
                        sub->state = CSR_BT_GATT_PREPEXEC_DONE;

                        /* Restart scan when necessary */
                        restart = TRUE;

                    } /* data blob proximity */
                } /* sub element found */
            } /* sub loop */
        } /* restart + cur element found */
    } /* cur loop */

    /* Delete abandoned 0,0/DONE elements */
    CsrCmnListIterateAllowRemove(&inst->prepare,
                                 csrBtGattPrepareIterateDeleteDone,
                                 NULL);
    
    /* Count disparate cid/handle elements that remain */
    for(cur = CSR_BT_GATT_PREPARE_INST_GET_FIRST(inst->prepare),
            unitCount = 0;
        cur != NULL;
        cur = cur->next)
    {
        /* Count combined items */
        if ((cur->cid == cid)
            && (cur->handle == handle))
        {
            unitCount++;
        }
    }

    /* Only send to app if it exists */
    app = CSR_BT_GATT_APP_INST_FIND_ATTR_HANDLE(inst->appInst, &handle);
    if (app)
    {
        CsrBtConnId btConnId;
        CsrBtGattConnElement *conn;
        CsrBtTypedAddr address;
        CsrBtGattConnInfo connInfo;
        CsrBtGattPrepareBuffer *curnext;

        /* Alloc array and fill it in */
        unit = CsrPmemAlloc(sizeof(CsrBtGattAttrWritePairs) * unitCount);
        unitCount = 0;
        for(cur = CSR_BT_GATT_PREPARE_INST_GET_FIRST(inst->prepare);
            cur != NULL;
            cur = curnext)
        {
            curnext = cur->next;

            if ((cur->cid == cid)
                && (cur->handle == handle))
            {
                unit[unitCount].attrHandle = handle;
                unit[unitCount].offset = cur->offset;
                unit[unitCount].valueLength = cur->dataLength;
                unit[unitCount].value = cur->data;
                
                cur->dataLength = 0;
                cur->data = NULL;
                unitCount++;

                /* Simple element. We can remove it immediately as no
                 * final prepare/execute write completion is needed */
                if (cur->state == CSR_BT_GATT_PREPEXEC_ACCESS)
                {                
                    CSR_BT_GATT_PREPARE_INST_REMOVE(inst->prepare, cur);
                    method = CSR_BT_GATT_PREPEXEC_ACCESS;
                }
                else if (cur->state == CSR_BT_GATT_PREPEXEC_ACCESS_AND_INFORM)
                {
                    /* Ready for final 'inform' pass */
                    cur->state = CSR_BT_GATT_PREPEXEC_INFORM_OK;
                    method = CSR_BT_GATT_PREPEXEC_ACCESS_AND_INFORM;
                }
                else
                {
                    CsrStateEventException("CSR_BT_GATT_DB_ACCESS_WRITE_IND element state mismatch",
                                           cid, handle, cur->state);
                }
            }
        }

        /* Try to find maching connection (to obtain address etc) */
        conn = NULL;
        btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(cid);
        CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                   &conn, btConnId,
                                                   app->gattId);
        if(conn == NULL)
        {
            conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                                   &btConnId);
        }
        
        if(conn != NULL)
        {
            address = conn->peerAddr;
            connInfo = (CsrUint32)(L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                                   ? CSR_BT_GATT_CONNINFO_LE
                                   : CSR_BT_GATT_CONNINFO_BREDR);
        }
        else
        {
            CsrBtAddrZero(&address);
            connInfo = CSR_BT_GATT_CONNINFO_LE;
        }

        /* Set 'check' based in the "access" or "access+inform"
         * method */
        CsrBtGattAccessWriteIndSend(CSR_BT_GATT_GET_QID_FROM_GATT_ID(app->gattId),
                                    app->gattId,
                                    btConnId,
                                    (CsrBtGattAccessCheck)(method == CSR_BT_GATT_PREPEXEC_ACCESS_AND_INFORM
                                                           ? CSR_BT_GATT_ACCESS_CHECK_RELIABLE_WRITE
                                                           : 0),
                                    connInfo,
                                    address,
                                    unitCount,
                                    unit,
                                    handle);
        return TRUE;
    }
    else
    {
        /* This should not happen. App has disappeared */
        CsrStateEventException("CSR_BT_GATT_DB_ACCESS_WRITE_IND server missing for cid/handle",
                               cid, handle, 0);
        return FALSE;
    }
}

/* Final processing of prepared write */
void CsrBtGattPlayExecuteFinalise(GattMainInst *inst,
                                  CsrUint16 cid)
{
    CsrBtGattPrepareBuffer *cur;
    CsrBtGattAppElement *app;
    for(cur = CSR_BT_GATT_PREPARE_INST_GET_FIRST(inst->prepare);
        cur != NULL;
        cur = cur->next)
    {
        if (cur->cid == cid)
        {
            if (( (cur->state == CSR_BT_GATT_PREPEXEC_INFORM_OK)
                  || (cur->state == CSR_BT_GATT_PREPEXEC_INFORM_FAIL))
                && ((app = CSR_BT_GATT_APP_INST_FIND_ATTR_HANDLE(inst->appInst, &(cur->handle))) != NULL))
            {
                CsrBtConnId btConnId;
                CsrBtGattConnElement *conn;
                CsrBtTypedAddr address;
                CsrBtGattConnInfo connInfo;

                /* Try to find maching connection (to obtain address etc) */
                conn = NULL;
                btConnId = CSR_BT_GATT_CREATE_LE_CONN_ID(cid);
                CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                           &conn, btConnId,
                                                           app->gattId);
                if(conn == NULL)
                {
                    conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                                           &btConnId);
                }
                
                if(conn != NULL)
                {
                    address = conn->peerAddr;
                    connInfo = (CsrUint32)(L2CA_CONFLAG_IS_LE(conn->l2capFlags)
                                           ? CSR_BT_GATT_CONNINFO_LE
                                           : CSR_BT_GATT_CONNINFO_BREDR);
                }
                else
                {
                    CsrBtAddrZero(&address);
                    connInfo = CSR_BT_GATT_CONNINFO_LE;
                }
                
                CsrBtGattDbAccessCompleteIndSend(CSR_BT_GATT_GET_QID_FROM_GATT_ID(app->gattId),
                                                 app->gattId,
                                                 btConnId,
                                                 connInfo,
                                                 address,
                                                 cur->handle,
                                                 (CsrBool)(cur->state == CSR_BT_GATT_PREPEXEC_INFORM_OK
                                                           ? TRUE
                                                           : FALSE));
            }

            /* Flag for removal */
            cur->state = CSR_BT_GATT_PREPEXEC_DONE;
        }
    }

    /* Purge completed 0,0/DONE elements */
    CsrCmnListIterateAllowRemove(&inst->prepare,
                                 csrBtGattPrepareIterateDeleteDone,
                                 NULL);    
} 

void CsrBtGattPlayExecuteMarkFail(GattMainInst *inst,
                                  CsrUint16 cid)
{
    CsrBtGattPrepareBuffer *cur;
    for(cur = CSR_BT_GATT_PREPARE_INST_GET_FIRST(inst->prepare);
        cur != NULL;
        cur = cur->next)
    {
        if ((cur->cid == cid)
            && (cur->state != CSR_BT_GATT_PREPEXEC_NEW))
        {
            /* If we told app about this, make sure we also tell it
             * that the operation failed */
            if (cur->state == CSR_BT_GATT_PREPEXEC_INFORM_OK)
            {
                cur->state = CSR_BT_GATT_PREPEXEC_INFORM_FAIL;
            }
            else
            {
                /* App never got told about this handle, so don't
                 * bother at all */
                cur->state = CSR_BT_GATT_PREPEXEC_DONE;
            }
        }
    }
}

/* Free scan filters in app instance */
void CsrBtGattFreeAppScanFilters(CsrBtGattAppElement *app)
{
    if(app->scan)
    {
        CsrUint8 i;
        CsrPmemFree(app->scan->filterAddr);
        app->scan->filterAddr = NULL;
        app->scan->filterAddrCount = 0;
        
        for(i=0; i<app->scan->filterDataCount; i++)
        {
            CsrPmemFree(app->scan->filterData[i].data);
        }
        CsrPmemFree(app->scan->filterData);
        app->scan->filterData = NULL;
        app->scan->filterDataCount = 0;
    }
}

/* Store AD/SR data in application */
CsrBool CsrBtGattStoreData(CsrBtGattAppAdvParams *adv,
                           CsrBool append,
                           CsrUint8 *adData,
                           CsrUint8 adLength,
                           CsrUint8 *srData,
                           CsrUint8 srLength)
{
    /* Clear data if not appending */
    if(!append)
    {
        adv->advertisingDataLength = 0;
        adv->scanResponseDataLength = 0;
        CsrMemSet(adv->advertisingData, 0, CSR_BT_CM_LE_MAX_REPORT_LENGTH);
        CsrMemSet(adv->scanResponseData, 0, CSR_BT_CM_LE_MAX_REPORT_LENGTH);
    }

    /* Exceeded max length? */
    if((adLength + adv->advertisingDataLength > CSR_BT_CM_LE_MAX_REPORT_LENGTH)
       || (srLength + adv->scanResponseDataLength > CSR_BT_CM_LE_MAX_REPORT_LENGTH))
    {
        return FALSE;
    }
    
    if (adLength > 0)
    {
        CsrMemCpy(adv->advertisingData + adv->advertisingDataLength,
                  adData,
                  adLength);
        adv->advertisingDataLength += adLength;
    }
    if (srLength > 0)
    {
        CsrMemCpy(adv->scanResponseData + adv->scanResponseDataLength,
                  srData,
                  srLength);
        adv->scanResponseDataLength += srLength;
    }
    return TRUE;
}

/* Return overall optimal scan parameters */
static void csrBtGattParamScan(GattMainInst *inst,
                               CsrUint8 *scanEnable,
                               CsrUint8 *scanType,
                               CsrUint16 *scanInterval,
                               CsrUint16 *scanWindow,
                               CsrUint8 *scanFilter,
                               CsrUint8 *filterDups)
{
    CsrBtGattAppElement *app;
    CsrBool first;

    /* Sane defaults (lowest duty cycle) */
    *scanEnable = CSR_BT_CM_LE_MODE_OFF;
    *scanType = HCI_ULP_ACTIVE_SCANNING;
    *scanInterval = CSR_BT_LE_DEFAULT_SCAN_INTERVAL;
    *scanWindow = CSR_BT_LE_DEFAULT_SCAN_WINDOW;
    *scanFilter = HCI_ULP_SCAN_FP_ALLOW_ALL;
    *filterDups = HCI_ULP_FILTER_DUPLICATES_ENABLED;
    first = TRUE;

    for(app = CSR_BT_GATT_APP_INST_GET_FIRST(inst->appInst);
        app != NULL;
        app = app->next)
    {
        CsrBtGattAppScanParams *aps;
        aps = app->scan;

        /* Only consider apps who want to scan */
        if(aps && (app->mode & CSR_BT_GATT_MODE_SCAN))
        {
            *scanEnable = CSR_BT_CM_LE_MODE_ON;

            /* Only set parameters if valid */
            if(app->mode & CSR_BT_GATT_ADV_CONFIG_SET)
            {
                /* If first active scanning app found, reset interval and
                 * window to lowest possible values */
                if(first)
                {
                    *scanInterval = HCI_ULP_SCAN_INTERVAL_MIN;
                    *scanWindow = HCI_ULP_SCAN_WINDOW_MIN;
                    first = FALSE;
                }

                /* Interval/window - select highest within bounds */
                *scanInterval = CSRMIN(CSRMAX(*scanInterval, aps->scanInterval),
                                       HCI_ULP_SCAN_INTERVAL_MAX);
                *scanWindow = CSRMIN(CSRMAX(*scanWindow, aps->scanWindow),
                                     HCI_ULP_SCAN_WINDOW_MAX);
            }

            /* Passive scan mode takes precedence if found */
            if(aps->scanFlags & CSR_BT_GATT_SCAN_STANDARD)
            {
                *scanType = HCI_ULP_ACTIVE_SCANNING;
            }

            /* Whitelist filter policy takes precedence if found */
            if(aps->scanFlags & CSR_BT_GATT_SCAN_WHITELIST)
            {
                *scanFilter = HCI_ULP_SCAN_FP_ALLOW_WHITELIST;
            }

            /* No-dup-filtering takes precedence if found */
            if(aps->scanFlags & CSR_BT_GATT_SCAN_NONDUP)
            {
                *filterDups = HCI_ULP_FILTER_DUPLICATES_DISABLED;
            }
        }
    }

}

/* Does this app have at least one connection in 'peripheral'
 * state? */
static CsrBool csrBtGattAppHasPeripheral(GattMainInst *inst,
                                         CsrBtGattAppElement *app)
{
    CsrBtGattConnElement *conn;
    for(conn = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
        conn != NULL;
        conn = conn->next)
    {
        if((conn->gattId == app->gattId)
           && ((conn->state == GATT_CONN_ST_PERIPHERAL)
               || (conn->state == GATT_CONN_ST_PENDING)))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/* Return overall optimal advertising paramters (numbers only, not data) */
static void csrBtGattParamAdvertise(GattMainInst *inst,
                                    CsrUint8 *advEnable,
                                    CsrUint16 *advIntervalMin,
                                    CsrUint16 *advIntervalMax,
                                    CsrUint8 *advertisingType,
                                    CsrUint8 *filterPolicy)
{
    CsrBtGattAppElement *app;
    CsrBool first;

    /* Sane defaults (lowest duty cycle) */
    *advEnable = CSR_BT_CM_LE_MODE_IGNORE;
    *advIntervalMin = CSR_BT_LE_DEFAULT_ADV_INTERVAL_MIN;
    *advIntervalMax = CSR_BT_LE_DEFAULT_ADV_INTERVAL_MAX;
    *advertisingType = HCI_ULP_ADVERT_DISCOVERABLE; /* scan response on */
    *filterPolicy = HCI_ULP_ADV_FP_ALLOW_ANY;
    first = TRUE;

    for(app = CSR_BT_GATT_APP_INST_GET_FIRST(inst->appInst);
        app != NULL;
        app = app->next)
    {
        CsrBtGattAppAdvParams *apa;
        CsrBool hasPerip;
        apa = app->adv;
        hasPerip = csrBtGattAppHasPeripheral(inst, app);

        /* Only consider apps who want to advertise. Or are in the
         * peripheral role */
        if(apa &&
           ((app->mode & CSR_BT_GATT_MODE_ADVERTISE)
            || hasPerip))
        {
            /* Allow transitions 'ignore' -> 'on' -> 'off'. The latter
             * happens if any app wants peripheral mode, in which case
             * we must use L2CAPs connection oriented way to turn on
             * advertising */
            if(*advEnable == CSR_BT_CM_LE_MODE_IGNORE)
            {
                *advEnable = CSR_BT_CM_LE_MODE_ON;
            }
            if(hasPerip)
            {
                *advEnable = CSR_BT_CM_LE_MODE_OFF;
            }

            /* Only set parameters if valid */
            if(app->mode & CSR_BT_GATT_ADV_CONFIG_SET)
            {
                /* If first active scanning app found, reset interval and
                 * window to lowest possible values */
                if(first)
                {
                    *advIntervalMin = CSR_BT_CM_HCI_LE_ADV_INTERVAL_MIN;
                    *advIntervalMax = CSR_BT_CM_HCI_LE_ADV_INTERVAL_MAX;
                    first = FALSE;
                }
                /* Select higher minimum and lower maximum within bounds */
                *advIntervalMin = CSRMAX(*advIntervalMin, apa->advIntervalMin);
                *advIntervalMax = CSRMIN(*advIntervalMax, apa->advIntervalMax);
            }

            /* Disable scan response if found */
            if(apa->advFlags & CSR_BT_GATT_FLAGS_DISABLE_SCAN_RESPONSE)
            {
                *advertisingType = HCI_ULP_ADVERT_NON_CONNECTABLE;
            }
            
            /* Whitelist filter policy takes precedence if found */
            if(apa->advFlags & CSR_BT_GATT_FLAGS_WHITELIST)
            {
                *filterPolicy = HCI_ULP_ADV_FP_ALLOW_WHITELIST;
            }
        }
    }

    /* Advertise-mode 'ignore' means 'off' as noone is interested in
     * advertising mode */
    if(*advEnable == CSR_BT_CM_LE_MODE_IGNORE)
    {
        *advEnable = CSR_BT_CM_LE_MODE_OFF;
    }
}

/* Return advertising and scan-response data */
static void csrBtGattDataAdvertise(GattMainInst *inst,
                                   CsrUint8 **advData, CsrUint8 *advLength,
                                   CsrUint8 **scanData, CsrUint8 *scanLength)
{
    CsrBtGattAppElement *app;
    CsrUint8 *adFlags;
    CsrUint8 adv[CSR_BT_CM_LE_MAX_REPORT_LENGTH];
    CsrUint8 rsp[CSR_BT_CM_LE_MAX_REPORT_LENGTH];
    CsrBool useSr;

    useSr = TRUE; /* allow scan response */

    (*advLength) = 0;
    (*scanLength) = 0;
    CsrMemSet(adv, 0, CSR_BT_CM_LE_MAX_REPORT_LENGTH);
    CsrMemSet(rsp, 0, CSR_BT_CM_LE_MAX_REPORT_LENGTH);

    /* Always only put flags in AD */
    adv[(*advLength)++] = 2;
    adv[(*advLength)++] = CSR_BT_EIR_DATA_TYPE_FLAGS;
    adFlags = &adv[(*advLength)++];
    *adFlags = (CSR_BT_EIR_FLAG_SIMUL_BREDR_LE_CONTROLLER
                | CSR_BT_EIR_FLAG_LE_GENERAL_DISCOVERABLE);

    /* Add application specific bits*/
    for(app = CSR_BT_GATT_APP_INST_GET_FIRST(inst->appInst);
        app != NULL;
        app = app->next)
    {
        CsrBtGattAppAdvParams *apa;
        apa = app->adv;

        if(apa &&
           ((app->mode & CSR_BT_GATT_MODE_ADVERTISE)
            || csrBtGattAppHasPeripheral(inst, app)))
        {
            /* Non-discoverable takes precedence */
            if(apa->advFlags & CSR_BT_GATT_FLAGS_NONDISCOVERABLE)
            {
                *adFlags &= ~CSR_BT_EIR_FLAG_LE_GENERAL_DISCOVERABLE;
                *adFlags &= ~CSR_BT_EIR_FLAG_LE_LIMITED_DISCOVERABLE;
            }
            /* And then limited discovery follows */
            if((apa->advFlags & CSR_BT_GATT_FLAGS_LIMITED_DISCOVERABLE)
               && (*adFlags & CSR_BT_EIR_FLAG_LE_GENERAL_DISCOVERABLE))
            {
                *adFlags &= ~CSR_BT_EIR_FLAG_LE_GENERAL_DISCOVERABLE;
                *adFlags |=  CSR_BT_EIR_FLAG_LE_LIMITED_DISCOVERABLE;
            }
            
            /* Is scan response disabled? */
            if(apa->advFlags & CSR_BT_GATT_FLAGS_DISABLE_SCAN_RESPONSE)
            {
                useSr = FALSE;
            }
            
            /* Copy app advertising data */
            if((apa->advertisingDataLength > 0)
               && (*advLength + apa->advertisingDataLength <= CSR_BT_CM_LE_MAX_REPORT_LENGTH))
            {
                CsrMemCpy((CsrUint8*)(adv+*advLength),
                          apa->advertisingData,
                          apa->advertisingDataLength);
                *advLength += apa->advertisingDataLength;
            }
            
            /* Copy app scan-response data */
            if((apa->scanResponseDataLength > 0)
               && (*scanLength + apa->scanResponseDataLength <= CSR_BT_CM_LE_MAX_REPORT_LENGTH))
            {
                CsrMemCpy((CsrUint8*)(rsp+*scanLength),
                          apa->scanResponseData,
                          apa->scanResponseDataLength);
                *scanLength += apa->scanResponseDataLength;
            }
        }
    }

    /* Add device name */
    if (inst->localName != NULL)
    {
        CsrUint8 *tgt;
        CsrUint8 *tgtlen;
        CsrUint8 nameLen;
        CsrBool useAd; /* put name in AD */

        nameLen = (CsrUint8)(CsrStrLen((CsrCharString*)inst->localName) & 0xFF);

        /* Only one name entry is allowed. Choose the data chunck with
         * the most room, but allow use of AD if entire name fits */
        if((*advLength <= *scanLength)
           || ((CSR_BT_CM_LE_MAX_REPORT_LENGTH - 2 - *advLength) >= nameLen))
        {
            useAd = TRUE;
        }
        else
        {
            useAd = FALSE;
        }

        /* If scan response is disabled, force use of AD */
        if(!useSr)
        {
            useAd = TRUE;
        }

        tgtlen = (CsrUint8*)(useAd ? advLength : scanLength);
        tgt = (CsrUint8*)(useAd ? adv : rsp);

        if(CSR_BT_CM_LE_MAX_REPORT_LENGTH - 2 - *tgtlen >= nameLen)
        {
            tgt[(*tgtlen)++] = (CsrUint8)(1 + nameLen);
            tgt[(*tgtlen)++] = CSR_BT_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME;
        }
        else if (CSR_BT_CM_LE_MAX_REPORT_LENGTH - 2 - *tgtlen >= CSR_BT_GATT_ADV_NAMELEN_THRES)
        {
            nameLen = CSRMIN(nameLen,
                             CSR_BT_CM_LE_MAX_REPORT_LENGTH - 2 - *tgtlen);
            tgt[(*tgtlen)++] = (CsrUint8)(1 + nameLen);
            tgt[(*tgtlen)++] = CSR_BT_EIR_DATA_TYPE_SHORT_LOCAL_NAME;
        }
        else
        {
            nameLen = 0;
        }

        if(nameLen > 0)
        {
            CsrMemCpy(tgt+*tgtlen, inst->localName, nameLen);
            CsrUtf8StrTruncate(tgt+*tgtlen, nameLen);
            (*tgtlen) += nameLen;
        }
    }

    *advData = CsrMemDup(adv, *advLength);
    *scanData = CsrMemDup(rsp, *scanLength);
}

/* Generic advertise data/parameter updater */
void CsrBtGattUpdateAdvertise(GattMainInst *inst)
{
    CsrUint8 *adData;
    CsrUint8 *srData;
    CsrUint8 adLen;
    CsrUint8 srLen;
    CsrUint8 advEnable;
    CsrUint16 advIntervalMin;
    CsrUint16 advIntervalMax;
    CsrUint8 advType;
    CsrUint8 filterPolicy;
    CsrBtTypedAddr zeroAddr;

    CsrBtAddrZero(&zeroAddr);

    /* Get parameters and trigger change */
    csrBtGattDataAdvertise(inst,
                           &adData, &adLen,
                           &srData, &srLen);
    csrBtGattParamAdvertise(inst,
                            &advEnable,
                            &advIntervalMin,
                            &advIntervalMax,
                            &advType,
                            &filterPolicy);
    CsrBtCmLeAdvertiseReqSend(CSR_BT_GATT_IFACEQUEUE, advEnable,
                              (CsrUint8)(CSR_BT_CM_LE_PARCHG_PAR |
                                         CSR_BT_CM_LE_PARCHG_DATA_AD |
                                         CSR_BT_CM_LE_PARCHG_DATA_SR),
                              adLen, adData, srLen, srData,
                              advIntervalMin, advIntervalMax, advType,
                              zeroAddr,
                              HCI_ULP_ADVERT_CHANNEL_DEFAULT, filterPolicy);
}

/* Generic scan parameter updater */
void CsrBtGattUpdateScan(GattMainInst *inst)
{
    CsrUint8 scanEnable;
    CsrUint8 scanType;
    CsrUint16 scanInterval;
    CsrUint16 scanWindow;
    CsrUint8 scanFilter;
    CsrUint8 filterDups;

    /* Get parameters and trigger change */
    csrBtGattParamScan(inst,
                       &scanEnable,
                       &scanType,
                       &scanInterval,
                       &scanWindow,
                       &scanFilter,
                       &filterDups);
    CsrBtCmLeScanReqSend(CSR_BT_GATT_IFACEQUEUE,
                         scanEnable,
                         CSR_BT_CM_LE_PARCHG_PAR,
                         scanType,
                         scanInterval,
                         scanWindow,
                         scanFilter,
                         filterDups);
}

/* Get optimal connection parameters for new connections */
static void csrBtGattParamConnection(GattMainInst *inst,
                                     CsrUint16 *scanInterval,
                                     CsrUint16 *scanWindow,
                                     CsrUint16 *connIntervalMin,
                                     CsrUint16 *connIntervalMax,
                                     CsrUint16 *connLatency,
                                     CsrUint16 *supervisionTimeout,
                                     CsrUint16 *connAttemptTimeout,
                                     CsrUint16 *advIntervalMin,
                                     CsrUint16 *advIntervalMax,
                                     CsrUint16 *connLatencyMax,
                                     CsrUint16 *supervisionTimeoutMin,
                                     CsrUint16 *supervisionTimeoutMax)
{
    CsrBtGattAppElement *app;
    CsrBool first;

    *scanInterval = CSR_BT_LE_DEFAULT_SCAN_INTERVAL;
    *scanWindow = CSR_BT_LE_DEFAULT_SCAN_WINDOW;
    *connIntervalMin = CSR_BT_LE_DEFAULT_CONN_INTERVAL_MIN;
    *connIntervalMax = CSR_BT_LE_DEFAULT_CONN_INTERVAL_MAX;
    *connLatency = CSR_BT_LE_DEFAULT_CONN_LATENCY;
    *supervisionTimeout = CSR_BT_LE_DEFAULT_CONN_SUPERVISION_TIMEOUT;
    *connAttemptTimeout = CSR_BT_LE_DEFAULT_CONN_ATTEMPT_TIMEOUT;
    *advIntervalMin = CSR_BT_LE_DEFAULT_ADV_INTERVAL_MIN;
    *advIntervalMax = CSR_BT_LE_DEFAULT_ADV_INTERVAL_MAX;
    *connLatencyMax = CSR_BT_LE_DEFAULT_CONN_LATENCY_MAX;
    *supervisionTimeoutMin = CSR_BT_LE_DEFAULT_CONN_SUPERVISION_TIMEOUT_MIN;
    *supervisionTimeoutMax = CSR_BT_LE_DEFAULT_CONN_SUPERVISION_TIMEOUT_MAX;
    first = TRUE;

    for(app = CSR_BT_GATT_APP_INST_GET_FIRST(inst->appInst);
        app != NULL;
        app = app->next)
    {
        CsrBtGattAppConnParams *apc;
        apc = app->conn;
        
        if(apc)
        {
            /* First first entry found, reset to spec min/max values to
             * allow overrides */
            if(first)
            {
                *scanInterval = HCI_ULP_SCAN_INTERVAL_MIN;
                *scanWindow = HCI_ULP_SCAN_WINDOW_MIN;
                *connIntervalMin = HCI_ULP_CONN_INTERVAL_MIN;
                *connIntervalMax = HCI_ULP_CONN_INTERVAL_MAX;
                *connLatency = HCI_ULP_CONN_LATENCY_MAX;
                *supervisionTimeout = HCI_ULP_SUPERVISION_TIMEOUT_MAX;
                *connAttemptTimeout = 0xFFFF;
                *advIntervalMin = CSR_BT_CM_HCI_LE_ADV_INTERVAL_MIN;
                *advIntervalMax = CSR_BT_CM_HCI_LE_ADV_INTERVAL_MAX;
                *connLatencyMax = HCI_ULP_CONN_LATENCY_MAX;
                *supervisionTimeoutMin = HCI_ULP_SUPERVISION_TIMEOUT_MIN;
                *supervisionTimeoutMax = HCI_ULP_SUPERVISION_TIMEOUT_MAX;
                first = FALSE;
            }

            *scanInterval = CSRMIN(CSRMAX(*scanInterval, apc->scanInterval),
                                   HCI_ULP_SCAN_INTERVAL_MAX);
            *scanWindow = CSRMIN(CSRMAX(*scanWindow, apc->scanWindow),
                                 HCI_ULP_SCAN_WINDOW_MAX);
            *connIntervalMin = CSRMIN(CSRMAX(*connIntervalMin, apc->connIntervalMin),
                                      HCI_ULP_CONN_INTERVAL_MAX);
            *connIntervalMax = CSRMAX(CSRMIN(*connIntervalMax, apc->connIntervalMax),
                                      HCI_ULP_CONN_INTERVAL_MIN);
            *connLatency = CSRMIN(CSRMIN(*connLatency, apc->connLatency),
                                  HCI_ULP_CONN_LATENCY_MAX);
            *supervisionTimeout = CSRMAX(CSRMIN(*supervisionTimeout, apc->supervisionTimeout),
                                         HCI_ULP_SUPERVISION_TIMEOUT_MIN);
            *connAttemptTimeout = CSRMIN(*connAttemptTimeout, apc->connAttemptTimeout);
            *advIntervalMin = CSRMAX(CSRMAX(*advIntervalMin, apc->advIntervalMin),
                                     CSR_BT_CM_HCI_LE_ADV_INTERVAL_MIN);
            *advIntervalMax = CSRMIN(CSRMIN(*advIntervalMax, apc->advIntervalMax),
                                     CSR_BT_CM_HCI_LE_ADV_INTERVAL_MAX);
            *connLatencyMax = CSRMIN(CSRMIN(*connLatencyMax, apc->connLatencyMax),
                                     HCI_ULP_CONN_LATENCY_MAX);
            *supervisionTimeoutMin = CSRMAX(CSRMIN(*supervisionTimeoutMin, apc->supervisionTimeoutMin),
                                            HCI_ULP_SUPERVISION_TIMEOUT_MIN);                                            
            *supervisionTimeoutMax = CSRMIN(CSRMAX(*supervisionTimeoutMax, apc->supervisionTimeoutMax),
                                            HCI_ULP_SUPERVISION_TIMEOUT_MAX);
        }
    }
}

/* Generic connection parameter updater */
void CsrBtGattUpdateConnection(GattMainInst *inst)
{
    CsrUint16 scanInterval;
    CsrUint16 scanWindow;
    CsrUint16 connIntervalMin;
    CsrUint16 connIntervalMax;
    CsrUint16 connLatency;
    CsrUint16 supervisionTimeout;
    CsrUint16 connAttemptTimeout;
    CsrUint16 advIntervalMin;
    CsrUint16 advIntervalMax;
    CsrUint16 connLatencyMax;
    CsrUint16 supervisionTimeoutMin;
    CsrUint16 supervisionTimeoutMax;

    csrBtGattParamConnection(inst,
                             &scanInterval, &scanWindow,
                             &connIntervalMin, &connIntervalMax,
                             &connLatency,
                             &supervisionTimeout, &connAttemptTimeout,
                             &advIntervalMin, &advIntervalMax,
                             &connLatencyMax,
                             &supervisionTimeoutMin, &supervisionTimeoutMax);
    CsrBtCmLeConnparamReqSend(CSR_BT_GATT_IFACEQUEUE,
                              scanInterval, scanWindow,
                              connIntervalMin, connIntervalMax,
                              connLatency,
                              supervisionTimeout, connAttemptTimeout,
                              advIntervalMin, advIntervalMax,
                              connLatencyMax,
                              supervisionTimeoutMin, supervisionTimeoutMax);
}

/* Is report address in filter? */
CsrBool CsrBtGattFilterAddr(CsrBtGattAppElement *app,
                            CsrBtTypedAddr *address)
{
    if((app->scan == NULL)
       || (app->scan->filterAddrCount == 0))
    {
        /* No addresses in filter means no filtering done */
        return TRUE;
    }
    else
    {
        CsrUint8 i;
        for(i=0; i<app->scan->filterAddrCount; i++)
        {
            if(CsrBtAddrEqWithType(&(app->scan->filterAddr[i]), address->type, &address->addr))
            {
                return TRUE;
            }
        }
        return FALSE;
    }
}

CsrBool CsrBtGattFilterData(CsrBtGattAppElement *app,
                            CsrUint8 length,
                            CsrUint8 *data)
{
    CsrUint8 i;

    if((app->scan == NULL) || (app->scan->filterDataCount == 0))
    {
        /* No tags in filter means no filtering done */
        return TRUE;
    }

    /* Loop all data filters for this app */
    for(i=0; i<app->scan->filterDataCount; i++)
    {
        CsrUint8 pos;
        CsrBtGattDataFilter *filter;
        filter = &(app->scan->filterData[i]);

        /* Traverse all [length,tag,data] blocks (see BT4.0 part C
         * section 8) */
        for(pos=0;
            (pos+2)<length;
            pos += data[pos]+1) /* +1 because 'length' itself is not included */
        {
            /* AD type "tag" check */
            if((filter->adType == 0)
               || (data[pos+1] == filter->adType))
            {
                if(filter->dataLength == 0)
                {
                    /* No pattern to match for */
                    return TRUE;
                }
                else
                {
                    /* Data pattern scan within this element */
                    CsrUint16 tlen; /* tag data length */
                    CsrUint8 *tdata; /* tag data */
                    CsrUint16 offset; /* offset into tag data */
                    tlen = data[pos]-1;
                    tdata = &(data[pos+2]);
                    offset = 0;

                    /* Sanity */
                    if(filter->dataLength <= tlen)
                    {
                        /* Match at every interval. If interval is zero,
                         * we only run this once (hence the do-while) */
                        do
                        {
                            if(CsrMemCmp(tdata+offset,
                                         filter->data,
                                         filter->dataLength) == 0)
                            {
                                /* Tag and pattern matches */
                                return TRUE;
                            }
                            offset += filter->interval;
                        }
                        while ((filter->interval != 0)
                               && (offset + filter->dataLength <= tlen));
                    } /* pattern length <= tlen */
                } /* data pattern to match */
            } /* tag type */
        } /* tag iteration */
    } /* filter iteration */

    /* No filters matched */
    return FALSE;
}

CsrBool CsrBtGattConnSetL2capFlags(CsrBtGattConnElement *conn,
                                   CsrBtGattConnFlags flags,
                                   CsrBool master,
                                   CsrBool bredr)
{
    L2CA_CONNECTION_T method;

    /* Internal flags */
    method = L2CA_CONNECTION_LE_MASTER_DIRECTED;
    conn->flags = flags;

    /* Figure out connection method flags */
    if(bredr)
    {
        if((master && !CsrBtBdAddrEqZero(&(conn->peerAddr.addr)))
           || !master)
        {
            method = L2CA_CONNECTION_BR_EDR;
        }
    }
    else if (flags & CSR_BT_GATT_FLAGS_WHITELIST)
    {
        method = (master
                  ? L2CA_CONNECTION_LE_MASTER_WHITELIST
                  : L2CA_CONNECTION_LE_SLAVE_WHITELIST);
    }
    else if ((flags & CSR_BT_GATT_FLAGS_UNDIRECTED) &&
             !master)
    {
        method = L2CA_CONNECTION_LE_SLAVE_UNDIRECTED;
    }
    else if (!CsrBtBdAddrEqZero(&(conn->peerAddr.addr)))
    {
        method = (master
                  ? L2CA_CONNECTION_LE_MASTER_DIRECTED
                  : L2CA_CONNECTION_LE_SLAVE_DIRECTED);
    }
    else
    {
        return FALSE;
    }

    /* Set method */
    conn->l2capFlags = L2CA_CONFLAG_ENUM(method);

    /* Extra flags */
    if((flags & CSR_BT_GATT_FLAGS_ADVERTISE_TIMEOUT)
       && (method != L2CA_CONNECTION_BR_EDR))
    {
        conn->l2capFlags |= L2CA_CONFLAG_PAGE_TIMEOUT;
    }

    return TRUE;
}

void CsrBtGattGetAttUuid(CsrBtUuid       uuid,
                         CsrUint32       *attUuid,
                         att_uuid_type_t *uuidType)
{
    if (uuid.length == CSR_BT_UUID16_SIZE)
    {
        *uuidType    = ATT_UUID16;
        attUuid[0]  = (CsrUint32)((uuid.uuid[1] << 8) | uuid.uuid[0]);
        attUuid[1]  = 0;
        attUuid[2]  = 0;
        attUuid[3]  = 0;
    }
    else
    {
        *uuidType   = ATT_UUID128;
        attUuid[0] = (CsrUint32)((uuid.uuid[15] << 24) | (uuid.uuid[14] << 16) | (uuid.uuid[13] << 8) | uuid.uuid[12]);
        attUuid[1] = (CsrUint32)((uuid.uuid[11] << 24) | (uuid.uuid[10] << 16) | (uuid.uuid[9] << 8) | uuid.uuid[8]);
        attUuid[2] = (CsrUint32)((uuid.uuid[7] << 24) | (uuid.uuid[6] << 16) | (uuid.uuid[5] << 8) | uuid.uuid[4]);
        attUuid[3] = (CsrUint32)((uuid.uuid[3] << 24) | (uuid.uuid[2] << 16) | (uuid.uuid[1] << 8) | uuid.uuid[0]);
    }
}

/* --------------------------------------------------------------------
  Helper function used to control the subscribe List
   -------------------------------------------------------------------- */
void CsrBtGattAddHandleForServiceChanged(GattMainInst        *inst,
                                         CsrBtTypedAddr      address,
                                         CsrBtGattHandle     attrHandle)
{ /* Add the handle of the Service Changed Characteristic Value to 
     GATTs private subscribe list */
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &inst->privateGattId);

    if (appElement)
    {
        CsrBtGattSubscribeElement * subElem = CSR_BT_GATT_SUBSCRIBE_INST_ADD_FIRST(appElement->subscribeInst);
        subElem->address       = address; 
        subElem->attrHandle    = attrHandle;
        subElem->configuration = CSR_BT_GATT_CHARAC_SERVICE_CHANGED;
    }
    else
    {
        CsrStateEventException("CsrBtGattAddHandleForServiceChanged no appElement",
                                CSR_BT_GATT_PRIM, 0, 0);
    }
}

void CsrBtGattRemoveHandleForServiceChanged(GattMainInst   *inst,
                                            CsrBtTypedAddr address,
                                            CsrBool        update)
{ /* Remove the handle of the Service Changed Characteristic Value from 
     GATTs private subscribe list */
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &inst->privateGattId);
   
    if (appElement)
    {
        CsrBtGattSubscribeElement *subElem = CSR_BT_GATT_SUBSCRIBE_INST_FIND_SERVICE_CHANGED(appElement, 
                                                                                             &address);

        if (subElem)
        {
            if (update)
            { /* Let SD save the handle in its trusted device List */
                CsrBtSdUpdateServiceChangedReqSend(address.addr, 
                                                   address.type, 
                                                   CSR_BT_GATT_CREATE_EXT_SERVICE_CHANGED_HANDLE(subElem->attrHandle));
            }
            /* Remove data from GATT */    
            CSR_BT_GATT_SUBSCRIBE_INST_REMOVE(appElement->subscribeInst, subElem);
        }
        /* Else - Ignore, Nothing to remove */
    }
    else
    {
        CsrStateEventException("CsrBtGattRemoveHandleForServiceChanged no appElement",
                                CSR_BT_GATT_PRIM, 0, 0);
    }
}

CsrBool CsrBtGattSubscribeInstFindByAddr(CsrCmnListElm_t *elem, void *value)
{
    /* Return TRUE if subscribe Inst "elem" matches
     * address/addressType */
    CsrBtTypedAddr address             = *(CsrBtTypedAddr *) value;
    CsrBtGattSubscribeElement *element = (CsrBtGattSubscribeElement *)elem;
    return (CsrBtAddrEqWithType(&(element->address), address.type, &(address.addr)) ? TRUE : FALSE);
}

CsrBool CsrBtGattSubscribeInstFindId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattSubscribeFindIds *ids     = (CsrBtGattSubscribeFindIds *)value;
    CsrBtGattSubscribeElement *element = (CsrBtGattSubscribeElement *)elem;

    /* Return TRUE if subscribe Inst "elem" matches combined
     * address/addressType/attrHandle "value" */
    return ((CsrBtAddrEqWithType(&(element->address), ids->address.type, &(ids->address.addr))
             && (element->attrHandle == ids->attrHandle))
            ? TRUE
            : FALSE);
}

CsrBtGattSubscribeElement *CsrBtGattGetSubscribeElement(GattMainInst *inst,
                                                        CsrBtGattAppElement *appElement,
                                                        CsrBtTypedAddr address,
                                                        CsrBtGattHandle valueHandle)
{   
    CsrBtGattSubscribeFindIds ids;
            
    ids.connInfo       = CSR_BT_GATT_CONNINFO_BREDR; /* not used */
    ids.address        = address; 
    ids.attrHandle     = valueHandle;
    ids.valueLength    = 0;
    ids.value          = NULL;
    ids.btConnId       = CSR_BT_GATT_LOCAL_BT_CONN_ID;
    ids.configuration  = CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT; 
    
    return (CSR_BT_GATT_SUBSCRIBE_INST_FIND_SUBSCRIBE_ID(appElement, &ids));
}

void CsrBtGattAddOrRemoveSubscribeElement(GattMainInst                         *inst, 
                                          CsrBtGattWriteClientConfigurationReq *prim)
{
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, &prim->gattId);
    
    if (appElement)
    {
        CsrBtGattSubscribeElement *subElement = CsrBtGattGetSubscribeElement(inst,
                                                                             appElement,
                                                                             prim->address,
                                                                             prim->valueHandle);

        if (subElement)
        { /* The application has allready subscribe for it */
            if (prim->configuration == CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT)
            { /* Remove the handle from the subscribe List */
                CSR_BT_GATT_SUBSCRIBE_INST_REMOVE(appElement->subscribeInst, subElement);
            }
            else
            { /* The application must have change the level 
                 subscribtion level */
                subElement->configuration   = prim->configuration;
            }
        }
        else
        {
            if (prim->configuration != CSR_BT_GATT_CLIENT_CHARAC_CONFIG_DEFAULT)
            {
                subElement = CSR_BT_GATT_SUBSCRIBE_INST_ADD_FIRST(appElement->subscribeInst);
                subElement->address         = prim->address; 
                subElement->attrHandle      = prim->valueHandle;
                subElement->configuration   = prim->configuration;
            }
            /* Else - Ignore Request to remove a handle which does not exist */
        }
    }
    else
    {
        CsrStateEventException("CsrBtGattAddOrRemoveSubscribeElement no appElement",
                                CSR_BT_GATT_PRIM, 0, 0);
    }
}

/* --------------------------------------------------------------------
  Helper function used to control the Sdp List
   -------------------------------------------------------------------- */
static void csrBtGattInitSdpInst(CsrCmnListElm_t *elem)
{
    /* Initialise a CsrBtGattSdpRecordElement. This function is call every
     * time a new entry is made on the queue list */
    CsrBtGattSdpRecordElement *sdpElem = (CsrBtGattSdpRecordElement *) elem;
    sdpElem->serviceRecordHandle = CMN_SDR_INVALID_SERVICE_HANDLE;
    sdpElem->state               = CSR_BT_GATT_SDP_INIT;    
    sdpElem->uuid.length         = 0;
    CsrMemSet(sdpElem->uuid.uuid, 0, CSR_BT_UUID128_SIZE);
}

CsrBool CsrBtGattSdpInstFindState(CsrCmnListElm_t *elem, void *value)
{ /* Returns TRUE if sdpInst "elem" has given state "value" */
    CsrBtGattSdpRecordElement *sdpElem = (CsrBtGattSdpRecordElement *) elem;
    CsrUint8 state = *(CsrUint8 *) value;
    return (sdpElem->state == state ? TRUE : FALSE);
}

CsrBool CsrBtGattSdpInstIterateUnregisterHandler(CsrCmnListElm_t *elem, void *value)
{ /* This function is called when ATT_REMOVE_CFM is received. It goes 
     though the SDP list and set the SDP state correct before 
     CsrBtGattSdsRequestHandler. E.g the procedure that start to Register
     or Unregister SDP records */
    CsrBtGattSdpRecordElement *sdpElem = (CsrBtGattSdpRecordElement *) elem;
    CsrBtGattSdpUnregisterIds *ids = (CsrBtGattSdpUnregisterIds *) value;

    if (sdpElem->startHandle >= ids->start &&
        sdpElem->startHandle <= ids->end)
    { /* Found a SDP record that must be remove. */
        switch (sdpElem->state)
        {
            case CSR_BT_GATT_SDP_PENDING:
            { 
                if (sdpElem->serviceRecordHandle == CMN_SDR_INVALID_SERVICE_HANDLE)
                { /* This SDP record has never been registered. 
                     Return TRUE to remove it from the SDP list */
                    return TRUE;
                }
                /* Else - This SDP record it waiting to be remove */
                break;
            }
            case CSR_BT_GATT_SDP_REGISTERED:
            { /* Set state to CSR_BT_GATT_SDP_PENDING to
                 indicate that the SDP records must be remove */
                sdpElem->state = CSR_BT_GATT_SDP_PENDING;
                break;
            }
            default:
            {
                CsrStateEventException("CsrBtGattSdpInstIterateUnregisterHandler Invalid state", 
                                        CSR_BT_GATT_PRIM, 0, 0);
                break;
            }
        }
    }
    return FALSE;
}

void CsrBtGattSdpInstChangeFromInitToPendingState(CsrCmnListElm_t *elem, void *value)
{ /* Change state from CSR_BT_GATT_SDP_INIT to CSR_BT_GATT_SDP_PENDING */
    CsrBtGattSdpRecordElement *sdpElem = (CsrBtGattSdpRecordElement *) elem;

    if (sdpElem->state == CSR_BT_GATT_SDP_INIT)
    {
        sdpElem->state = CSR_BT_GATT_SDP_PENDING;
    }
    /* Else - Ignore */
    CSR_UNUSED(value);
}

/* --------------------------------------------------------------------
  Helper function used to control the application List
   -------------------------------------------------------------------- */
CsrBool CsrBtGattAppInstFindGattId(CsrCmnListElm_t *elem, void *value)
{
    /* Returns TRUE if appInst "elem" has given gattId "value" */
    CsrBtGattAppElement *element = (CsrBtGattAppElement *)elem;
    CsrBtGattId         gattId   = *(CsrBtGattId *) value;
    return (element->gattId == gattId) ? TRUE : FALSE;
}

CsrBool CsrBtGattAppInstFindAttrHandle(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattAppElement *element = (CsrBtGattAppElement *)elem;
    CsrUint16           handle   = *(CsrUint16 *)value;

    if (handle >= element->start && handle <= element->end)
    {
        return TRUE;
    }
    return FALSE;
}

CsrInt32 CsrBtGattAppInstSortByAttributeValue(CsrCmnListElm_t *elem1, CsrCmnListElm_t *elem2)
{
    /* Sort attr start handles so smallest value is placed first */
    CsrBtGattAppElement *elemA = (CsrBtGattAppElement *)elem1;
    CsrBtGattAppElement *elemB = (CsrBtGattAppElement *)elem2;

    if (elemA->start < elemB->start)
    {
        return -1;
    }
    else if (elemA->start > elemB->start)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void csrBtGattInitAppInst(CsrCmnListElm_t *elem)
{
    /* Initialise a CsrBtGattAppElement. This function is called every
     * time a new entry is added to the app list */
    CsrBtGattAppElement *app = (CsrBtGattAppElement*)elem;

    app->gattId     = CSR_BT_GATT_INVALID_GATT_ID;
    app->qid        = CSR_SCHED_QID_INVALID;
    app->start      = 0;
    app->end        = 0;
    app->mode       = 0;
    app->conn       = NULL;
    app->adv        = NULL;
    app->scan       = NULL;
    app->eventMask  = CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_NONE;
    CsrCmnListInit(&(app->subscribeInst), 0, NULL, NULL);
}

static void csrBtGattFreeAppInst(CsrCmnListElm_t *elem)
{
    /* CsrPmemFree local pointers in the CsrBtGattAppElement. This
     * function is called every time an element is removed from the
     * app list */
    CsrBtGattAppElement *app = (CsrBtGattAppElement*)elem;
    CsrBtGattFreeAppScanFilters(app);
    CsrPmemFree(app->scan);
    CsrPmemFree(app->adv);
    CsrPmemFree(app->conn);
    CsrCmnListDeinit(&(app->subscribeInst));
}

/* --------------------------------------------------------------------
  Helper function used to control the Connection List
   -------------------------------------------------------------------- */
static void csrBtGattInitConnInst(CsrCmnListElm_t *elem)
{
    /* Initialise a CsrBtGattConnElement. This function is called
     * every time a new entry is made on the conn list */
    CsrBtGattConnElement *element = (CsrBtGattConnElement *) elem;

    element->state = GATT_CONN_ST_NULL;
    CsrBtAddrZero(&(element->peerAddr));
    element->l2capFlags = 0;
    element->flags = 0;
    element->btConnId = CSR_BT_CONN_ID_INVALID;
    element->gattId = CSR_BT_GATT_INVALID_GATT_ID;
    element->mtu = 0;
    element->preferredMtu = CSR_BT_ATT_MTU_DEFAULT;
    element->interval = CSR_BT_GATT_INVALID_CONN_INTERVAL;      
    element->timeout = CSR_BT_GATT_INVALID_CONN_SUPERVISION_TIMEOUT;
    element->latency = CSR_BT_GATT_INVALID_CONN_LATENCY;
    element->connIntervalMin = CSR_BT_LE_DEFAULT_CONN_INTERVAL_MIN;
    element->connIntervalMax = CSR_BT_LE_DEFAULT_CONN_INTERVAL_MAX;
    element->connLatencyMax = CSR_BT_LE_DEFAULT_CONN_LATENCY_MAX;
    element->supervisionTimeoutMin = CSR_BT_LE_DEFAULT_CONN_SUPERVISION_TIMEOUT_MIN;
    element->supervisionTimeoutMax = CSR_BT_LE_DEFAULT_CONN_SUPERVISION_TIMEOUT_MAX;
    element->connParamUpdateId = CSR_BT_GATT_CONN_PARAM_UPDATE_IDENTIFIER_INVALID;
    element->remoteName         = NULL;
    element->remoteNameLength   = 0;
}

static void csrBtGattFreeConnInst(CsrCmnListElm_t *elem)
{
    /* CsrPmemFree local pointers in the CsrBtGattConnElement. This
     * function is called every time an element is removed from the
     * conn list */
    CsrBtGattConnElement *element = (CsrBtGattConnElement *) elem;
    CsrPmemFree(element->remoteName);
}

/* Returns TRUE if queue "elem" matches combined btConnId/gattId "value" */
CsrBool CsrBtGattQueueInstFindBtConnGattId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnFindIds *ids     = (CsrBtGattConnFindIds *)value;
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *)elem;

    return ((element->btConnId == ids->btConnId
             && element->gattId == ids->gattId)
            ? TRUE
            : FALSE);
}

/* Returns TRUE if connInst "elem" matches combined btConnId/gattId "value" */
CsrBool CsrBtGattConnInstFindBtConnGattId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnFindIds *ids     = (CsrBtGattConnFindIds *)value;
    CsrBtGattConnElement *element = (CsrBtGattConnElement *)elem;

    return ((element->btConnId == ids->btConnId
             && element->gattId == ids->gattId)
            ? TRUE
            : FALSE);
}

CsrBool CsrBtGattConnInstFindBredrAcceptPending(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnFindIds *ids     = (CsrBtGattConnFindIds *)value;
    CsrBtGattConnElement *element = (CsrBtGattConnElement *)elem;

    return ((element->btConnId == ids->btConnId && 
             element->gattId   == ids->gattId   &&
             element->state    == GATT_CONN_ST_BREDR_ACCEPT)
            ? TRUE
            : FALSE);
}


CsrBool CsrBtGattConnInstFindConnectedBtConnId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement *conn = (CsrBtGattConnElement *)elem;
    CsrBtConnId     btConnId   = *(CsrBtConnId *) value;
    return ((conn->btConnId == btConnId &&
            CSR_BT_GATT_CONN_IS_CONNECTED(conn->state)) ? TRUE : FALSE);
}

CsrBool CsrBtGattConnInstFindBredrAcceptBtConnId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement *conn = (CsrBtGattConnElement *)elem;
    CsrBtConnId     btConnId   = *(CsrBtConnId *) value;
    return ((conn->btConnId == btConnId &&
             conn->state == GATT_CONN_ST_BREDR_ACCEPT) ? TRUE : FALSE);
}

CsrBool CsrBtGattConnInstFindBtConnId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement *conn = (CsrBtGattConnElement *)elem;
    CsrBtConnId     btConnId   = *(CsrBtConnId *) value;
    return ((conn->btConnId == btConnId) ? TRUE : FALSE);
}

CsrBool CsrBtGattConnInstFindBtConnIdFromAddress(CsrCmnListElm_t *elem, void *value)
{
     CsrBtGattConnElement *conn = (CsrBtGattConnElement *)elem;
     CsrBtTypedAddr address     = *(CsrBtTypedAddr *) value;

     return (((CsrBtAddrEqWithType(&(conn->peerAddr), address.type, &(address.addr))) &&
              (conn->state == GATT_CONN_ST_CONNECTED)) ? TRUE : FALSE);
}

void CsrBtGattConnInstUpdateMtu(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement *conn     = (CsrBtGattConnElement *)elem;
    CsrBtGattConnUpdateMtuIds *ids = (CsrBtGattConnUpdateMtuIds *) value;

    if (conn->btConnId == ids->btConnId &&
        CSR_BT_GATT_CONN_IS_CONNECTED(conn->state))
    {
        conn->mtu = ids->mtu;
        CsrBtGattMtuChangedIndSend(conn->gattId, conn->btConnId, conn->mtu);
    }
}

void CsrBtGattConnInstUpdateRemoteName(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement *conn            = (CsrBtGattConnElement *)elem;
    CsrBtGattConnUpdateRemoteNameIds *ids = (CsrBtGattConnUpdateRemoteNameIds *) value;

    if (conn->btConnId == ids->btConnId &&
        CSR_BT_GATT_CONN_IS_CONNECTED(conn->state))
    {
        CsrPmemFree(conn->remoteName);
        conn->remoteName       = (CsrUint8 *) CsrPmemAlloc(ids->length);
        conn->remoteNameLength = ids->length;
        CsrMemCpy(conn->remoteName, ids->name, ids->length);
    }
}

void CsrBtGattConnInstGetNumOfExistingPending(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement  *conn               = (CsrBtGattConnElement *)elem;
    CsrBtGattConnNumOfExistingPendingIds *ids = (CsrBtGattConnNumOfExistingPendingIds *) value;

    if (conn->btConnId == ids->btConnId         && 
        (conn->state ==  GATT_CONN_ST_CONNECTED ||  
         conn->state == GATT_CONN_ST_PENDING))
    {
        ids->numOfRsp++;
        *(CsrBtGattConnNumOfExistingPendingIds*)value = *ids;
    }
}

void CsrBtGattParamConnChangeIndSendHandler(GattMainInst *inst, CsrBtGattConnElement *conn)
{ /* Check if the application has subscribe for 
     CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND events */
    CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                        &conn->gattId);
    if (appElement && 
        CSR_MASK_IS_SET(appElement->eventMask, CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND))
    { /* The application has subscribe for the connection parameter update event */
        CsrBtGattParamConnChangedInd *msg = (CsrBtGattParamConnChangedInd*)
                                             CsrPmemAlloc(sizeof(CsrBtGattParamConnChangedInd));

        msg->type     = CSR_BT_GATT_PARAM_CONN_CHANGED_IND;
        msg->gattId   = conn->gattId;
        msg->btConnId = conn->btConnId;
        msg->interval = conn->interval;
        msg->latency  = conn->latency;
        msg->timeout  = conn->timeout;
        CsrSchedMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(conn->gattId), CSR_BT_GATT_PRIM, msg);
    }
    else
    { /* just ignore */
        ;
    }
}

void CsrBtGattConnInstUpdateParamChanged(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement  *conn       = (CsrBtGattConnElement *)elem;
    CsrBtGattConnParamChangedIds *ids = (CsrBtGattConnParamChangedIds *) value;

    if (CsrBtAddrEqWithType(&conn->peerAddr, ids->deviceAddr.type, &ids->deviceAddr.addr) &&
        (conn->interval != ids->interval ||      
         conn->timeout  != ids->timeout  ||
         conn->latency  != ids->latency))
    { /* At least one of the LE Connection parameters has been updated */
        conn->interval = ids->interval;
        conn->timeout  = ids->timeout;
        conn->latency  = ids->latency;
        
        if (CSR_BT_GATT_CONN_IS_CONNECTED(conn->state))
        { /* Check if the application has subscribe for 
             CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND events */
            CsrBtGattParamConnChangeIndSendHandler(ids->gattInst, conn);
        }
    }
}

void CsrBtGattConnInstUpdateParamReq(CsrCmnListElm_t *elem, void *value)
{ /* This function check if the proposed Connection parameter values shall
     be accepted by the application or not */
    CsrBtGattConnElement  *conn         = (CsrBtGattConnElement *)elem;
    CsrBtGattConnParamUpdateReqIds *ids = (CsrBtGattConnParamUpdateReqIds *) value;

    if(conn->btConnId == ids->btConnId &&
       conn->gattId   != ids->gattId   && 
       CSR_BT_GATT_CONN_IS_CONNECTED(conn->state))
    {
        if (ids->connIntervalMax > conn->connIntervalMax || 
            ids->connIntervalMin < conn->connIntervalMin ||
            ids->latency > conn->connLatencyMax          ||              
            ids->timeout > conn->supervisionTimeoutMax   ||
            ids->timeout < conn->supervisionTimeoutMin)
        { /* The proposed values are out of the once that GATT
             can auto accept */ 
            GattMainInst *inst = ids->gattInst;
            CsrBtGattAppElement *appElement = CSR_BT_GATT_APP_INST_FIND_GATT_ID(inst->appInst, 
                                                                                &conn->gattId);
            
            /* Check if the application has subscribe for 
               CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND events */
            if (appElement && 
                CSR_MASK_IS_SET(appElement->eventMask, CSR_BT_GATT_EVENT_MASK_SUBSCRIBE_PARAM_CONN_UPDATE_IND))
            { /* The application has subscribe for the connection parameter update event.
                 Send a CsrBtGattParamConnUpdateInd to the application */
                CsrBtGattParamConnUpdateInd *msg = (CsrBtGattParamConnUpdateInd*)
                                            CsrPmemAlloc(sizeof(CsrBtGattParamConnUpdateInd));

                msg->type = CSR_BT_GATT_PARAM_CONN_UPDATE_IND;
                msg->gattId = conn->gattId;
                msg->btConnId = conn->btConnId;
                msg->connIntervalMax = ids->connIntervalMax;
                msg->connIntervalMin = ids->connIntervalMin;
                msg->connLatency = ids->latency;
                msg->supervisionTimeout = ids->timeout;
                msg->incoming = ((ids->gattId == inst->privateGattId) ? TRUE : FALSE);
                msg->identifier = ids->connParamUpdateId;
                CsrSchedMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(conn->gattId), CSR_BT_GATT_PRIM, msg);

                conn->connParamUpdateId = ids->connParamUpdateId;
                ids->numOfRsp++;
                *(CsrBtGattConnParamUpdateReqIds*)value = *ids;
            }
            else
            { /* The application has not subscribe for the connection parameter 
                 update event. Check if this request is local or remote 
                 initiated. If local initiated GATT shall always consider this 
                 as an accept and if peer initiated GATT shall consider this as 
                 reject */
                if (ids->gattId == inst->privateGattId)
                {
                    ids->accept = FALSE;
                    *(CsrBtGattConnParamUpdateReqIds*)value = *ids;
                }
            }
        }
        /* Else - The proposed values are within the range of the once 
                  GATT is able to auto accept */
    }
    /* Else - Not connected to the same physical connection */
}

void CsrBtGattConnInstUpdateParamRes(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattConnElement  *conn         = (CsrBtGattConnElement *)elem;
    CsrBtGattConnParamUpdateResIds *ids = (CsrBtGattConnParamUpdateResIds *) value;
    
    if(conn->btConnId == ids->btConnId            &&
       CSR_BT_GATT_CONN_IS_CONNECTED(conn->state) &&
       conn->connParamUpdateId == ids->connParamUpdateId)
    {
        if (ids->cancelled)
        {
            conn->connParamUpdateId = CSR_BT_GATT_CONN_PARAM_UPDATE_IDENTIFIER_INVALID;
        }
        else
        {
            ids->numOfRsp++;
            *(CsrBtGattConnParamUpdateResIds*)value = *ids;
        }
    }
}

/* --------------------------------------------------------------------
  Helper function used to control the Pending Command Queue List
   -------------------------------------------------------------------- */
static void csrBtGattInitQueueInst(CsrCmnListElm_t *elem)
{
    /* Initialise a CsrBtGattQueueElement. This function is call every
     * time a new entry is made on the queue list */
    CsrBtGattQueueElement *qElem = (CsrBtGattQueueElement *) elem;
    qElem->btConnId       = CSR_BT_CONN_ID_INVALID;
    qElem->msgState       = CSR_BT_GATT_MSG_QUEUE_IDLE;
    qElem->dataOffset     = 0;
    qElem->dataElemIndex  = 0;    
    qElem->gattMsg        = NULL;
    qElem->data           = NULL;
    qElem->attrHandle     = CSR_BT_GATT_ATTR_HANDLE_INVALID;
    qElem->restoreFunc    = NULL;
    qElem->cancelFunc     = NULL;
    qElem->securityFunc   = NULL;  
}

static void csrBtGattFreeQueueInst(CsrCmnListElm_t *elem)
{
    /* CsrPmemFree local pointers in the CsrBtGattQueueElement.  This
     * function is called every time a element is removed from the
     * queue list */
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *) elem;

    if (element->data)
    {
        CsrPmemFree(element->data);
        element->data = NULL;
    }

    if (element->gattMsg)
    {
        CsrBtGattFreeDownstreamMessageContents(CSR_BT_GATT_PRIM, element->gattMsg);
        CsrPmemFree(element->gattMsg);
        element->gattMsg = NULL;
    }
}

CsrBool CsrBtGattQueueFindBtConnId(CsrCmnListElm_t *elem, void *value)
{
    /* Goes though the queue list in order to find a matching
     * btConnId. If it find a match the first found
     * CsrBtGattQueueElement is returned */
    CsrBtGattQueueElement *element = (CsrBtGattQueueElement *)elem;
    CsrBtConnId           btConnId = *(CsrBtConnId *)value;
    return ((element->btConnId == btConnId &&
             btConnId != CSR_BT_CONN_ID_INVALID) ? TRUE : FALSE);
}

CsrBool CsrBtGattQueueFindMsgToCancel(CsrCmnListElm_t *elem, void *value)
{
    CsrBtGattQueueElement *element   = (CsrBtGattQueueElement *)elem;
    CsrBtGattFindMsgToCancelIds *ids = (CsrBtGattFindMsgToCancelIds *)value;

    return ((element->gattId   == ids->gattId &&
             element->btConnId == ids->btConnId && 
             element->cancelFunc) 
             ? TRUE : FALSE);
}

CsrBool CsrBtGattQueueFindPrivateReadName(CsrCmnListElm_t *elem, void *value)
{ /* Goes though the queue list in order to find out if the message
     trigget by CsrBtGattReadRemoteLeNameHandler is still on queue */     
     
    CsrBtGattQueueElement *element   = (CsrBtGattQueueElement *)elem;
    CsrBtGattFindMsgToCancelIds *ids = (CsrBtGattFindMsgToCancelIds *)value;

    if (element->btConnId == ids->btConnId &&
        element->gattId   == ids->gattId &&
        element->gattMsg)
    {
        CsrBtGattPrim type = *(CsrBtGattPrim *) element->gattMsg;

        if (type == CSR_BT_GATT_READ_BY_UUID_REQ)
        {
            CsrBtGattReadByUuidReq *reqMsg = (CsrBtGattReadByUuidReq *) element->gattMsg;

            if (reqMsg->uuid.length == CSR_BT_UUID16_SIZE && 
                CSR_GET_UINT16_FROM_LITTLE_ENDIAN(reqMsg->uuid.uuid) == CSR_BT_GATT_DEVICE_NAME_UUID)
            { /* The Remote Name Characteristic Value is still on the Queue */
                return TRUE;
            }
        }
    }
    return FALSE;
}

CsrUint16 CsrBtGattValidateBtConnIdByMtu(GattMainInst *inst,
                                         CsrBtGattId  gattId,
                                         CsrBtConnId  btConnId)
{
    if (btConnId == CSR_BT_GATT_LOCAL_BT_CONN_ID)
    {
        return CSR_BT_GATT_LOCAL_MAX_MTU;
    }
    else
    {
        CsrBtGattConnElement  *conn = NULL;

        if (gattId != inst->privateGattId)
        {
            CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                                       &conn,
                                                       btConnId,
                                                       gattId);
        }

        if (conn == NULL)
        {
            conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst, 
                                                                   &btConnId);
        }

        if (conn &&
            CSR_BT_GATT_CONN_IS_CONNECTED(conn->state))
        {
            return conn->mtu;
        }
    }
    return 0;
}

void CsrBtGattQueueRestoreHandler(GattMainInst *inst, CsrBtGattQueueElement *element)
{
    if (element)
    {
        CsrBtGattQueueElement *tmp;

        CsrBtConnId btConnId = element->btConnId;
        CSR_BT_GATT_QUEUE_REMOVE(inst->queue, element);
        tmp = CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(inst->queue, &btConnId);

        if (tmp && tmp->gattMsg && tmp->restoreFunc)
        {
            tmp->msgState = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS;
            tmp->restoreFunc(inst,
                             tmp,
                             CsrBtGattValidateBtConnIdByMtu(inst,
                                                            tmp->gattId,
                                                            tmp->btConnId));
        }
    }
}

/* Append whitelist. Consumes 'addrs' */
void CsrBtGattWhitelistAdd(GattMainInst *inst,
                           CsrUint16 num,
                           CsrBtTypedAddr *addrs)
{
    if(inst->whitelist == NULL)
    {
        inst->whitelist = addrs;
        inst->whitelistCount = num;
    }
    else
    {
        /* Append to existing list */
        CsrBtTypedAddr *new;
        new = CsrPmemAlloc((inst->whitelistCount + num) * sizeof(CsrBtTypedAddr));
        CsrMemCpy(new, inst->whitelist, inst->whitelistCount*sizeof(CsrBtTypedAddr));
        CsrMemCpy(&(new[inst->whitelistCount]), addrs, num*sizeof(CsrBtTypedAddr));
        CsrPmemFree(inst->whitelist);
        inst->whitelist = new;
        inst->whitelistCount += num;
        CsrPmemFree(addrs);
    }

    (void)CsrBtGattWhitelistUpdate(inst);
}

/* Find btConnId given address */
CsrBtConnId CsrBtGattConnIdByAddr(GattMainInst *inst, CsrBtTypedAddr *addr)
{
    CsrBtGattConnElement *conn;

    for(conn = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
        conn != NULL;
        conn = conn->next)
    {
        if(CsrBtAddrEqWithType(&conn->peerAddr, addr->type, &addr->addr))
        {
            return conn->btConnId;
        }
    }
    return CSR_BT_CONN_ID_INVALID;
}

/* Send next whitelist-add command. Return TRUE if finished */
CsrBool CsrBtGattWhitelistUpdate(GattMainInst *inst)
{
    if(inst->whitelistCount > inst->whitelistIndex)
    {
        /* Send next index. Increment happens in CM completion handler
         * in cm_sef */
        CsrBtCmLeWhitelistSetReqSend(CSR_BT_GATT_IFACEQUEUE,
                                     CSR_BT_CM_LE_WHITELIST_ADD,
                                     inst->whitelist[inst->whitelistIndex]);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/* Setup security level for BR/EDR */
void CsrBtGattInitSecurity(void)
{
    CsrUint16 secOut;
    CsrUint16 secIn;

    secIn = 0;
    secOut = 0;
    CsrBtScMapSecInLevel(CSR_BT_GATT_MANDATORY_SECURITY_INCOMING, &secIn);
    CsrBtScMapSecOutLevel(CSR_BT_GATT_MANDATORY_SECURITY_OUTGOING, &secOut);

    dm_sm_register_req(CSR_BT_GATT_IFACEQUEUE,
                       0, /*context*/
                       SEC_PROTOCOL_L2CAP,
                       CSR_BT_GATT_PSM,
                       TRUE, /*outgoing_ok*/
                       (CsrUint16)(secIn | secOut),
                       0, /*psm for connectionless*/
                       NULL);
}

/* Count number of page-scanning applications */
CsrUint16 CsrBtGattConnPageScanners(GattMainInst *inst)
{
    CsrUint16 i;
    CsrBtGattConnElement *find;

    /* Are more connections page scanning? */
    for(i=0, find = CSR_BT_GATT_CONN_INST_GET_FIRST(inst->connInst);
        find != NULL;
        find = find->next)
    {
        if(find->state == GATT_CONN_ST_BREDR_ACCEPT)
        {
            i++;
        }
    }
    return i;
}

/* Initialisation handler */
void CsrBtGattInitHandler(GattMainInst *inst)
{
    CsrBtGattAppElement *appElement;
    CsrBtGattQueueElement *qElememt;

    /* Create a gattId for gatt itself */
    inst->privateGattId = CSR_BT_GATT_CREATE_GATT_ID(1, CSR_BT_GATT_IFACEQUEUE);

    /* Init data for running SDS search */
    inst->sdpBtConnId   = CSR_BT_CONN_ID_INVALID;
    inst->sdpSearchData = CsrBtUtilSdcInit(csrBtGattDiscoverServicesBySdpResultHandler, 
                                           CSR_BT_GATT_IFACEQUEUE);

    /* Ensure a valid (empty) local name. CM will send us the correct
     * one as soon as it has been read and/or changes */
    inst->localName = (CsrUtf8String *) CsrPmemZalloc(sizeof(CsrUtf8String));

    inst->connParamUpdateIdCounter = CSR_BT_GATT_CONN_PARAM_UPDATE_IDENTIFIER_INVALID;

    /* Init the queue, conn, applist and sdp list */
    CsrCmnListInit(&inst->queue, 0, csrBtGattInitQueueInst, csrBtGattFreeQueueInst);
    CsrCmnListInit(&inst->connInst, 0, csrBtGattInitConnInst, csrBtGattFreeConnInst);
    CsrCmnListInit(&inst->appInst, 0, csrBtGattInitAppInst, csrBtGattFreeAppInst);
    CsrCmnListInit(&inst->prepare, 0, csrBtGattInitPrepare, csrBtGattFreePrepare);
    CsrCmnListInit(&(inst->sdpRecordInst), 0, csrBtGattInitSdpInst, NULL);

    /* Add an dummy element in to the queue in order to make sure that
     * the queue is block while GATT init itself */
    qElememt            = CSR_BT_GATT_QUEUE_ADD_LAST(inst->queue);
    qElememt->gattId    = inst->privateGattId;
    qElememt->btConnId  = CSR_BT_GATT_LOCAL_BT_CONN_ID;

    /* GATT must assing some attribute handles to itself in order to
     * add some mandatory attributes to the database. Note GATT cannot
     * make these entries before it knows the local device name*/
    appElement = CSR_BT_GATT_APP_INST_ADD_FIRST(inst->appInst);

    if (appElement)
    {
        appElement->qid      = CSR_BT_GATT_IFACEQUEUE;
        appElement->start    = CSR_BT_GATT_ATTR_HANDLE_START;
        appElement->end      = CSR_BT_GATT_ATTR_HANDLE_END;
        appElement->gattId   = inst->privateGattId;

        /* Tell the SD that it must look for the
         * CSR_BT_GENERIC_ATTRIBUTE_PROFILE_UUID service when it
         * performs a SD_READ_AVAILABLE_SERVICE_REQ */
        CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_GENERIC_ATTRIBUTE_PROFILE_UUID);

        /* Register GATT to the ATT subsystem */
        attlib_register_req(CSR_BT_GATT_IFACEQUEUE, NULL);

        /* Register GATT with the CM (for report indications) */
        CsrBtCmRegisterHandlerReqSend(CSR_BT_CM_HANDLER_TYPE_LE,
                                      CSR_BT_GATT_IFACEQUEUE,
                                      0); /*flags, not used*/
    }
    else
    {
        CsrStateEventException("GATT cannot add private AppInst", 0, 0, 0);
    }
}


/*******************************************************************************
 * Public Util Helpers
 *******************************************************************************/
CsrBool CsrBtGattUtilGetEirInfo(CsrUint8 *data,
                                CsrUint8 eirDataType,
                                CsrUint8 *returnDataOffset,
                                CsrUint8 *returnDataLength)
{
    CsrUint8 count = 0;
    CsrUint8 currentTypeLength = data[0];

    for (count = 0; count < CSR_BT_CM_LE_MAX_REPORT_LENGTH; )
    {
        if (data[count + 1] == eirDataType)
        {
            *returnDataOffset = count + 2;
            *returnDataLength = currentTypeLength - 1;
            return TRUE;
        }
        else
        {
            count += (currentTypeLength + 1);
            currentTypeLength = data[count];
        }
    }

    /* EIR data type not found */
    *returnDataOffset = 0;
    *returnDataLength = 0;
    return FALSE;
    
}
/* Helpers to Create a Database*/
CsrBtGattDb *CsrBtGattUtilCreateDbEntryFromUuid128(CsrBtGattDb        *head,
                                                   CsrBtGattHandle    *attrHandle,
                                                   CsrBtUuid128       uuid128,
                                                   CsrBtGattPermFlags attrPermission,
                                                   CsrBtGattAttrFlags attrFlags,
                                                   CsrUint16          attrValueLength,
                                                   const CsrUint8     *attrValue,
                                                   CsrBtGattDb        **tail)
{
    CsrBtGattDb *db = CsrPmemAlloc(sizeof(CsrBtGattDb));
    db->handle  = *attrHandle;
    *attrHandle = (CsrBtGattHandle)(db->handle + 1); 
    db->perm    = attrPermission;
    db->flags   = attrFlags;
    db->next    = NULL;
    db->uuid[0] = (CsrUint32)((uuid128[15] << 24) | (uuid128[14] << 16) | (uuid128[13] << 8) | uuid128[12]);
    db->uuid[1] = (CsrUint32)((uuid128[11] << 24) | (uuid128[10] << 16) | (uuid128[9] << 8) | uuid128[8]);
    db->uuid[2] = (CsrUint32)((uuid128[7] << 24) | (uuid128[6] << 16) | (uuid128[5] << 8) | uuid128[4]);
    db->uuid[3] = (CsrUint32)((uuid128[3] << 24) | (uuid128[2] << 16) | (uuid128[1] << 8) | uuid128[0]);

    if (attrValueLength > 0 && attrValue)
    {
        db->size_value   = attrValueLength;
        db->value        = (CsrUint8 *)CsrPmemAlloc(db->size_value);
        CsrMemCpy(db->value, attrValue, db->size_value);
    }
    else
    {
        db->value   = NULL;
        db->size_value = 0;
    }

    if (head == NULL)
    {
        /* This it the first entry in the dataBase */
        head  = db;
        *tail = head;
    }
    else
    { 
        (*tail)->next = db;
        *tail         = db;
    }
    return (head);
}

CsrBtGattDb *CsrBtGattUtilCreateDbEntryFromUuid16(CsrBtGattDb        *head,
                                                  CsrBtGattHandle    *attrHandle,
                                                  CsrBtUuid16        uuid16,
                                                  CsrBtGattPermFlags attrPermission,
                                                  CsrBtGattAttrFlags attrFlags,
                                                  CsrUint16          attrValueLength,
                                                  const CsrUint8     *attrValue,
                                                  CsrBtGattDb        **tail)
{
    CsrBtUuid128 uuid128 = {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
                            0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  
    uuid128[12] = (CsrUint8)(uuid16 & 0x00ff); 
    uuid128[13] = (CsrUint8)((uuid16 & 0xff00) >> 8); 
    return (CsrBtGattUtilCreateDbEntryFromUuid128(head,
                                                  attrHandle,
                                                  uuid128,
                                                  attrPermission,
                                                  attrFlags,
                                                  attrValueLength,
                                                  attrValue,
                                                  tail));
}

CsrBtGattDb *CsrBtGattUtilCreatePrimaryServiceWith16BitUuid(CsrBtGattDb        *head,
                                                            CsrBtGattHandle    *attrHandle,
                                                            CsrBtUuid16        serviceUuid16,
                                                            CsrBool            leOnly,
                                                            CsrBtGattDb        **tail)
{
    CsrUint8 uuid[CSR_BT_UUID16_SIZE];
    CsrBtGattAttrFlags attrFlags;

    /* Note setting the attrFlags to CSR_BT_GATT_ATTR_FLAGS_DISABLE_BREDR
       is in this case only to indicate that GATT shall try to generate 
       a SDP record. Before the create local database it sent to ATT, GATT 
       makes sure that attrFlags always is set to CSR_BT_GATT_ATTR_FLAGS_NONE 
       in the function called csrBtGattBrEdrSupportHandler */
    if (leOnly)
    {
        attrFlags = CSR_BT_GATT_ATTR_FLAGS_DISABLE_BREDR;
    }
    else
    {
        attrFlags = CSR_BT_GATT_ATTR_FLAGS_NONE;
    }
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(serviceUuid16, uuid);
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_PRIMARY_SERVICE_UUID,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 attrFlags,
                                                 CSR_BT_UUID16_SIZE,
                                                 uuid,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreatePrimaryServiceWith128BitUuid(CsrBtGattDb     *head,
                                                             CsrBtGattHandle *attrHandle,
                                                             CsrBtUuid128    uuid128,
                                                             CsrBool         leOnly,
                                                             CsrBtGattDb     **tail)
{
    CsrBtGattAttrFlags attrFlags;

    /* Note setting the attrFlags to CSR_BT_GATT_ATTR_FLAGS_DISABLE_BREDR
       is in this case only to indicate that GATT shall try to generate 
       a SDP record. Before the create local database it sent to ATT, GATT 
       makes sure that attrFlags always is set to CSR_BT_GATT_ATTR_FLAGS_NONE 
       in the function called csrBtGattBrEdrSupportHandler */
    if (leOnly)
    {
        attrFlags = CSR_BT_GATT_ATTR_FLAGS_DISABLE_BREDR;
    }
    else
    {
        attrFlags = CSR_BT_GATT_ATTR_FLAGS_NONE;
    }
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_PRIMARY_SERVICE_UUID,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 attrFlags,
                                                 CSR_BT_UUID128_SIZE,
                                                 (CsrUint8 *)uuid128,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreateSecondaryServiceWith16BitUuid(CsrBtGattDb     *head,
                                                              CsrBtGattHandle *attrHandle,
                                                              CsrBtUuid16     serviceUuid16,
                                                              CsrBtGattDb     **tail)
{
    CsrUint8 uuid[CSR_BT_UUID16_SIZE];
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(serviceUuid16, uuid);
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_SECONDARY_SERVICE_UUID,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                 CSR_BT_UUID16_SIZE,
                                                 uuid,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreateSecondaryServiceWith128BitUuid(CsrBtGattDb     *head,
                                                               CsrBtGattHandle *attrHandle,
                                                               CsrBtUuid128    uuid128,
                                                               CsrBtGattDb     **tail)
{
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_SECONDARY_SERVICE_UUID,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                 CSR_BT_UUID128_SIZE,
                                                 (CsrUint8 *)uuid128,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreateIncludeDefinitionWithUuid(CsrBtGattDb     *head,
                                                          CsrBtGattHandle *attrHandle,
                                                          CsrBtGattHandle inclServiceAttrHandle,
                                                          CsrBtGattHandle endGroupHandle,  
                                                          CsrBtUuid16     serviceUuid,
                                                          CsrBtGattDb     **tail)
{
    CsrUint8 value[CSR_BT_GATT_INCLUDE_WITH_UUID_LENGTH];
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(inclServiceAttrHandle, value);
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(endGroupHandle, &(value[2]));
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(serviceUuid, &(value[4]));
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_INCLUDE_UUID,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                 CSR_BT_GATT_INCLUDE_WITH_UUID_LENGTH,
                                                 value,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreateIncludeDefinitionWithoutUuid(CsrBtGattDb     *head,
                                                             CsrBtGattHandle *attrHandle,
                                                             CsrBtGattHandle inclServiceAttrHandle,
                                                             CsrBtGattHandle endGroupHandle,  
                                                             CsrBtGattDb     **tail)
{
    CsrUint8 value[CSR_BT_GATT_INCLUDE_WITHOUT_UUID_LENGTH];
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(inclServiceAttrHandle, value);
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(endGroupHandle, &(value[2]));
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_INCLUDE_UUID,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                 CSR_BT_GATT_INCLUDE_WITHOUT_UUID_LENGTH,
                                                 value,
                                                 tail));
}

static CsrBtGattPermFlags csrBtGattUtilCharacDefinitionGetPermission(CsrBtGattPropertiesBits properties)
{
    CsrBtGattPermFlags attrValuePermission = CSR_BT_GATT_PERM_FLAGS_NONE;
    
    if (CSR_MASK_IS_SET(properties, CSR_BT_GATT_CHARAC_PROPERTIES_READ))
    {
        attrValuePermission |= CSR_BT_GATT_PERM_FLAGS_READ;
    }

    if (CSR_MASK_IS_SET(properties, CSR_BT_GATT_CHARAC_PROPERTIES_WRITE_WITHOUT_RESPONSE))
    {
        attrValuePermission |= CSR_BT_GATT_PERM_FLAGS_WRITE_CMD;
    }

    if (CSR_MASK_IS_SET(properties, CSR_BT_GATT_CHARAC_PROPERTIES_WRITE))
    {
        attrValuePermission |= CSR_BT_GATT_PERM_FLAGS_WRITE_REQ;
    }

    if (CSR_MASK_IS_SET(properties, CSR_BT_GATT_CHARAC_PROPERTIES_AUTH_SIGNED_WRITES))
    {
        attrValuePermission |= CSR_BT_GATT_PERM_FLAGS_AUTH_SIGNED_WRITES;
    }
    return attrValuePermission;
}

CsrBtGattDb *CsrBtGattUtilCreateCharacDefinitionWith16BitUuid(CsrBtGattDb             *head,
                                                              CsrBtGattHandle         *attrHandle,
                                                              CsrBtGattPropertiesBits properties,
                                                              CsrBtUuid16             uuid16,
                                                              CsrBtGattAttrFlags      attrValueFlags,
                                                              CsrUint16               attrValueLength,
                                                              const CsrUint8          *attrValue,
                                                              CsrBtGattDb             **tail)
{
    CsrUint8  value[CSR_BT_GATT_CHARAC_DECLARATION_MIN_LENGTH];
    CsrBtGattPermFlags attrValuePermission = csrBtGattUtilCharacDefinitionGetPermission(properties);
    value[0] = properties;
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN((*attrHandle + 1), &(value[1]));
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(uuid16, &(value[3]));
    head = CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                attrHandle,
                                                CSR_BT_GATT_CHARACTERISTIC_UUID,
                                                CSR_BT_GATT_PERM_FLAGS_READ,
                                                CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                CSR_BT_GATT_CHARAC_DECLARATION_MIN_LENGTH,
                                                (CsrUint8*)&value,
                                                tail);

    return (CsrBtGattUtilCreateDbEntryFromUuid16(head, 
                                                 attrHandle, 
                                                 uuid16, 
                                                 attrValuePermission, 
                                                 attrValueFlags, 
                                                 attrValueLength, 
                                                 attrValue, 
                                                 tail));
}



CsrBtGattDb *CsrBtGattUtilCreateCharacDefinitionWith128BitUuid(CsrBtGattDb            *head,
                                                               CsrBtGattHandle         *attrHandle,
                                                               CsrBtGattPropertiesBits properties,
                                                               CsrBtUuid128            uuid128,
                                                               CsrBtGattAttrFlags      attrValueFlags,
                                                               CsrUint16               attrValueLength,
                                                               const CsrUint8          *attrValue,
                                                               CsrBtGattDb             **tail)
{
    
    CsrUint8 value[CSR_BT_GATT_CHARAC_DECLARATION_MAX_LENGTH];
    CsrBtGattPermFlags attrValuePermission = csrBtGattUtilCharacDefinitionGetPermission(properties);

    value[0] = properties;
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN((*attrHandle + 1), &(value[1]));
    CsrMemCpy(&(value[3]), uuid128, CSR_BT_UUID128_SIZE); 
    
    head = CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                attrHandle,
                                                CSR_BT_GATT_CHARACTERISTIC_UUID,
                                                CSR_BT_GATT_PERM_FLAGS_READ,
                                                CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                CSR_BT_GATT_CHARAC_DECLARATION_MAX_LENGTH,
                                                value,
                                                tail);

    return (CsrBtGattUtilCreateDbEntryFromUuid128(head, 
                                                  attrHandle, 
                                                  uuid128, 
                                                  attrValuePermission, 
                                                  attrValueFlags, 
                                                  attrValueLength, 
                                                  attrValue, 
                                                  tail));


}

CsrBtGattDb *CsrBtGattUtilCreateCharacExtProperties(CsrBtGattDb                 *head,
                                                    CsrBtGattHandle             *attrHandle,
                                                    CsrBtGattExtPropertiesBits  extProperties,
                                                    CsrBtGattDb                 **tail)
{
    CsrUint8 value[] = {0,0};
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(extProperties, value);
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_CHARACTERISTIC_EXTENDED_PROPERTIES,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                 sizeof(value),
                                                 value,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreateCharacUserDescription(CsrBtGattDb                 *head,
                                                      CsrBtGattHandle             *attrHandle,
                                                      const CsrUtf8String         *userDescription,
                                                      CsrBtGattPermFlags          attrPermission,
                                                      CsrBtGattAttrFlags          attrFlags,
                                                      CsrBtGattDb                 **tail)
{
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_CHARACTERISTIC_USER_DESCRIPTION,
                                                 attrPermission,
                                                 attrFlags,
                                                 (CsrUint16)(CsrUtf8StringLengthInBytes(userDescription) + 1),
                                                 userDescription,
                                                 tail));

}

CsrBtGattDb *CsrBtGattUtilCreateClientCharacConfiguration(CsrBtGattDb        *head,
                                                          CsrBtGattHandle    *attrHandle,
                                                          CsrBtGattAttrFlags attrFlags,
                                                          CsrBtGattDb        **tail)
{
    CsrUint8 value[] = {0,0};
    attrFlags |= (CSR_BT_GATT_ATTR_FLAGS_IRQ_READ | CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE);
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_CLIENT_CHARACTERISTIC_CONFIGURATION,
                                                 (CSR_BT_GATT_PERM_FLAGS_READ | 
                                                  CSR_BT_GATT_PERM_FLAGS_WRITE),
                                                 attrFlags,
                                                 sizeof(value),
                                                 value,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreateServerCharacConfiguration(CsrBtGattDb            *head,
                                                          CsrBtGattHandle        *attrHandle,
                                                          CsrBtGattAttrFlags     attrFlags,
                                                          CsrBtGattSrvConfigBits configurationBits,
                                                          CsrBtGattDb            **tail)
{
    CsrUint8 value[] = {0,0};
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(configurationBits, value);
    attrFlags |= CSR_BT_GATT_ATTR_FLAGS_IRQ_WRITE;
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_SERVER_CHARACTERISTIC_CONFIGURATION,
                                                 (CSR_BT_GATT_PERM_FLAGS_READ | 
                                                  CSR_BT_GATT_PERM_FLAGS_WRITE),
                                                 attrFlags,
                                                 sizeof(value),
                                                 value,
                                                 tail));
}

CsrBtGattDb *CsrBtGattUtilCreateCharacPresentationFormat(CsrBtGattDb      *head,
                                                         CsrBtGattHandle  *attrHandle,
                                                         CsrBtGattFormats format,
                                                         CsrInt8          exponent,
                                                         CsrUint16        unit, 
                                                         CsrUint8         nameSpace,
                                                         CsrUint16        description, 
                                                         CsrBtGattDb      **tail)
{
    CsrUint8 value[CSR_BT_GATT_CHARAC_PRESENTATION_FORMAT_LENGTH];
    value[0] = format;
    value[1] = exponent;
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(unit, &(value[2]));
    value[4] = nameSpace;
    CSR_COPY_UINT16_TO_LITTLE_ENDIAN(description, &(value[5]));
    return (CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                 attrHandle,
                                                 CSR_BT_GATT_CHARACTERISTIC_PRESENTATION_FORMAT,
                                                 CSR_BT_GATT_PERM_FLAGS_READ,
                                                 CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                 CSR_BT_GATT_CHARAC_PRESENTATION_FORMAT_LENGTH,
                                                 value,
                                                 tail));    
}

CsrBtGattDb *CsrBtGattUtilCreateCharacAggregateFormat(CsrBtGattDb            *head,
                                                      CsrBtGattHandle        *attrHandle,
                                                      CsrUint16              handlesCount,
                                                      const CsrBtGattHandle  *handles,
                                                      CsrBtGattDb            **tail)
{
    CsrUint16 i, t = 0;
    CsrUint16 length = (CsrUint16) (handlesCount * sizeof(CsrBtGattHandle));
    CsrUint8  *value = (CsrUint8 *) CsrPmemAlloc(handlesCount * sizeof(CsrBtGattHandle));

    for (i = 0; i < handlesCount; i++)
    {
        CSR_COPY_UINT16_TO_LITTLE_ENDIAN(handles[i], &(value[t]));
        t+=2;
    }

    head = CsrBtGattUtilCreateDbEntryFromUuid16(head,
                                                attrHandle,
                                                CSR_BT_GATT_CHARACTERISTIC_AGGREGATE_FORMAT,
                                                CSR_BT_GATT_PERM_FLAGS_READ,
                                                CSR_BT_GATT_ATTR_FLAGS_NONE,
                                                length,
                                                value,
                                                tail);    
    CsrPmemFree(value);
    return (head);
}

CsrBtGattConnInfo CsrBtGattGetConnInfo(GattMainInst *inst,
                                       CsrBtGattId  gattId,
                                       CsrBtConnId  btConnId)
{
    CsrBtGattConnElement *conn;

    conn = NULL;
    CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(inst->connInst,
                                               &conn,
                                               btConnId, gattId);
    if (conn == NULL)
    {
        conn = CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(inst->connInst,
                                                               &btConnId);
    }
    
    return (CsrBtGattConnInfo)(((conn != NULL)
                                && L2CA_CONFLAG_IS_LE(conn->l2capFlags))
                               ? CSR_BT_GATT_CONNINFO_LE
                               : CSR_BT_GATT_CONNINFO_BREDR);
}
