/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_gatt_private.h"

void CsrBtGattMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_GATT_PRIM, msg);
}

void CsrBtGattStdCfmSend(CsrBtGattPrim   type,
                         CsrBtGattId     gattId,
                         CsrBtResultCode resultCode,
                         CsrBtSupplier   resultSupplier)
{
    CsrBtGattStdCfm *msg = (CsrBtGattStdCfm *) 
                            CsrPmemAlloc(sizeof(CsrBtGattStdCfm));
    
    msg->type           = type;
    msg->resultCode     = resultCode;
    msg->resultSupplier = resultSupplier;
    msg->gattId         = gattId;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattStdBtConnIdCfmSend(CsrBtGattPrim   type,
                                 CsrBtGattId     gattId,
                                 CsrBtResultCode resultCode,
                                 CsrBtSupplier   resultSupplier,
                                 CsrBtConnId     btConnId)
{
    CsrBtGattStdBtConnIdCfm *msg = (CsrBtGattStdBtConnIdCfm *) 
                                    CsrPmemAlloc(sizeof(CsrBtGattStdBtConnIdCfm));
    
    msg->type           = type;
    msg->resultCode     = resultCode;
    msg->resultSupplier = resultSupplier;
    msg->gattId         = gattId;
    msg->btConnId       = btConnId;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattDisconnectIndSend(CsrBtGattId gattId,
                                CsrBtResultCode resultCode,
                                CsrBtSupplier resultSupplier,
                                CsrBtConnId btConnId,
                                CsrBtTypedAddr *address,
                                CsrBtGattConnInfo connInfo)
{
    CsrBtGattDisconnectInd *msg;
    msg = CsrPmemAlloc(sizeof(CsrBtGattDisconnectInd));
    
    msg->type           = CSR_BT_GATT_DISCONNECT_IND;
    msg->reasonCode     = resultCode;
    msg->reasonSupplier = resultSupplier;
    msg->gattId         = gattId;
    msg->btConnId       = btConnId;
    msg->address        = *address;
    msg->connInfo       = connInfo;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattConnectIndSend(CsrBtGattPrim type,
                             CsrBtGattId gattId,
                             CsrBtResultCode resultCode,
                             CsrBtSupplier resultSupplier,
                             CsrBtConnId btConnId,
                             CsrBtGattConnInfo connInfo,
                             CsrBtTypedAddr *address,
                             CsrUint16 mtu)
{
    CsrBtGattConnectInd *msg = (CsrBtGattConnectInd*)CsrPmemAlloc(sizeof(CsrBtGattConnectInd));
    msg->type           = type;
    msg->resultCode     = resultCode;
    msg->resultSupplier = resultSupplier;
    msg->gattId         = gattId;
    msg->btConnId       = btConnId;
    msg->connInfo       = connInfo;
    msg->address        = *address;
    msg->mtu            = mtu;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattRegisterCfmSend(CsrSchedQid       qid,
                              CsrBtGattId       gattId,
                              CsrBtResultCode   resultCode, 
                              CsrBtSupplier     resultSupplier,
                              CsrUint16         context)
{
    CsrBtGattRegisterCfm *msg = (CsrBtGattRegisterCfm *) 
                                 CsrPmemAlloc(sizeof(CsrBtGattRegisterCfm));

    msg->type           = CSR_BT_GATT_REGISTER_CFM;
    msg->gattId         = gattId;
    msg->resultCode     = resultCode;
    msg->resultSupplier = resultSupplier;
    msg->context        = context;
    CsrBtGattMessagePut(qid, msg);
}

void CsrBtGattDbAllocCfmSend(CsrBtGattId       gattId,
                             CsrBtResultCode   resultCode, 
                             CsrBtSupplier     resultSupplier,
                             CsrBtGattHandle   start,
                             CsrBtGattHandle   end,
                             CsrUint16         preferredHandle)
{
    CsrBtGattDbAllocCfm *msg = (CsrBtGattDbAllocCfm *) 
                                CsrPmemAlloc(sizeof(CsrBtGattDbAllocCfm));

    msg->type                 = CSR_BT_GATT_DB_ALLOC_CFM;
    msg->gattId               = gattId;
    msg->resultCode           = resultCode;
    msg->resultSupplier       = resultSupplier;
    msg->start                = start;
    msg->end                  = end;
    msg->preferredStartHandle = preferredHandle;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattDbDeallocCfmSend(CsrBtGattId       gattId,
                               CsrBtResultCode   resultCode, 
                               CsrBtSupplier     resultSupplier,
                               CsrBtGattHandle   start,
                               CsrBtGattHandle   end)
{
    CsrBtGattDbDeallocCfm *msg = (CsrBtGattDbDeallocCfm *) 
                                  CsrPmemAlloc(sizeof(CsrBtGattDbDeallocCfm));

    msg->type           = CSR_BT_GATT_DB_DEALLOC_CFM;
    msg->gattId         = gattId;
    msg->resultCode     = resultCode;
    msg->resultSupplier = resultSupplier;
    msg->start          = start;
    msg->end            = end;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattDbRemoveCfmSend(CsrBtGattId     gattId,
                              CsrBtResultCode resultCode,
                              CsrBtSupplier   resultSupplier,
                              CsrUint16       numOfAttr)
{
    CsrBtGattDbRemoveCfm *msg = (CsrBtGattDbRemoveCfm *) 
                                 CsrPmemAlloc(sizeof(CsrBtGattDbRemoveCfm));
    
    msg->type           = CSR_BT_GATT_DB_REMOVE_CFM;
    msg->resultCode     = resultCode;
    msg->resultSupplier = resultSupplier;
    msg->gattId         = gattId;
    msg->numOfAttr      = numOfAttr;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattMtuChangedIndSend(CsrBtGattId     gattId,
                                CsrBtConnId     btConnId,
                                CsrUint16       mtu)
{
    CsrBtGattMtuChangedInd *msg = (CsrBtGattMtuChangedInd *)
                                    CsrPmemAlloc(sizeof(CsrBtGattMtuChangedInd));

    msg->type           = CSR_BT_GATT_MTU_CHANGED_IND;
    msg->btConnId       = btConnId;
    msg->mtu            = mtu;
    msg->gattId         = gattId;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattReportIndSend(CsrBtGattId gattId,
                            CsrBtGattReportEvent eventType,
                            CsrBtTypedAddr *address,
                            CsrBtTypedAddr *permanentAddress,
                            CsrUint8 lengthData,
                            CsrUint8 *data,
                            CsrInt8 rssi)
{
    CsrBtGattReportInd *msg;
    msg = (CsrBtGattReportInd*)CsrPmemAlloc(sizeof(CsrBtGattReportInd));
    
    msg->type = CSR_BT_GATT_REPORT_IND;
    msg->gattId = gattId;
    msg->eventType = eventType;
    msg->address = *address;
    msg->permanentAddress = *permanentAddress;
    msg->lengthData = lengthData;

    CsrMemCpy(msg->data, data, lengthData);
    CsrMemSet(&msg->data[lengthData], 0, CSR_BT_CM_LE_MAX_REPORT_LENGTH - lengthData);
        
    msg->rssi = rssi;

    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattDiscoverServicesIndSend(CsrBtGattQueueElement *qElem,
                                      CsrBtConnId           btConnId,
                                      CsrBtGattHandle       startHandle,
                                      CsrBtGattHandle       endHandle,
                                      CsrUint16             length,
                                      CsrUint8              *data)
{
    if (length == CSR_BT_UUID128_SIZE ||
        length == CSR_BT_UUID16_SIZE)
    {
        CsrBtGattDiscoverServicesInd *msg = (CsrBtGattDiscoverServicesInd*)
                                             CsrPmemAlloc(sizeof(CsrBtGattDiscoverServicesInd));
        
        qElem->msgState     = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK;
        msg->type           = CSR_BT_GATT_DISCOVER_SERVICES_IND;
        msg->gattId         = qElem->gattId;
        msg->btConnId       = btConnId;
        msg->startHandle    = startHandle;
        msg->endHandle      = endHandle;
        msg->uuid.length    = (CsrUint8) length;
        CsrMemSet(msg->uuid.uuid, 0, CSR_BT_UUID128_SIZE);
        CsrMemCpy(msg->uuid.uuid, data, length);    
        CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(qElem->gattId), msg);
    }
}

void CsrBtGattDiscoverCharacIndSend(CsrBtGattQueueElement   *qElem,
                                    CsrBtConnId             btConnId,
                                    CsrBtGattHandle         declarationHandle,
                                    CsrBtUuid               uuid,
                                    CsrUint8                *data)
{
    CsrBtGattDiscoverCharacInd *msg = (CsrBtGattDiscoverCharacInd*)
                                       CsrPmemAlloc(sizeof(CsrBtGattDiscoverCharacInd));
    
    qElem->msgState         = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK;
    msg->type               = CSR_BT_GATT_DISCOVER_CHARAC_IND;
    msg->gattId             = qElem->gattId;
    msg->btConnId           = btConnId;
    msg->declarationHandle  = declarationHandle;
    msg->uuid               = uuid;
    msg->property           = data[CSR_BT_GATT_CHARAC_PROPERTIES_INDEX];
    msg->valueHandle        = CSR_BT_GATT_GET_HANDLE(data, CSR_BT_GATT_CHARAC_VALUE_HANDLE_FIRST_INDEX);
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(qElem->gattId), msg);
}

void CsrBtGattFindInclServicesIndSend(CsrBtGattQueueElement *qElem,
                                      CsrBtConnId           btConnId,
                                      CsrBtGattHandle       attrHandle,
                                      CsrUint16             length,
                                      CsrUint8              *data)
{
    CsrBtGattFindInclServicesInd *msg = (CsrBtGattFindInclServicesInd*)
                                         CsrPmemAlloc(sizeof(CsrBtGattFindInclServicesInd));
    
    qElem->msgState     = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK;
    msg->type           = CSR_BT_GATT_FIND_INCL_SERVICES_IND;
    msg->gattId         = qElem->gattId;
    msg->btConnId       = btConnId;
    msg->attrHandle     = attrHandle;
    msg->startHandle    = CSR_BT_GATT_GET_HANDLE(data, CSR_BT_GATT_INCLUDE_START_HANDLE_INDEX);
    msg->endGroupHandle = CSR_BT_GATT_GET_HANDLE(data, CSR_BT_GATT_INCLUDE_END_HANDLE_INDEX);
    msg->uuid.length    = (CsrUint8) (length - CSR_BT_GATT_INCLUDE_WITHOUT_UUID_LENGTH);
    CsrMemSet(msg->uuid.uuid, 0, CSR_BT_UUID128_SIZE);
    CsrMemCpy(msg->uuid.uuid, &data[CSR_BT_GATT_INCLUDE_UUID_INDEX], msg->uuid.length);
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(qElem->gattId), msg);
}

void CsrBtGattDiscoverCharacDescriptorsIndSend(CsrBtGattQueueElement *qElem,
                                               CsrBtConnId           btConnId,
                                               CsrBtGattHandle       descriptorHandle,
                                               att_uuid_type_t       uuidType,
                                               CsrUint32             *attUuid)
{
    CsrBtGattDiscoverCharacDescriptorsInd *msg = (CsrBtGattDiscoverCharacDescriptorsInd *)
                                                  CsrPmemAlloc(sizeof(CsrBtGattDiscoverCharacDescriptorsInd));
    
    qElem->msgState         = CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK;

    msg->type               = CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND;
    msg->gattId             = qElem->gattId;
    msg->btConnId           = btConnId;
    msg->descriptorHandle   = descriptorHandle;
    
    if (uuidType == ATT_UUID16)
    {
        msg->uuid.length = CSR_BT_UUID16_SIZE;
        CsrMemSet(msg->uuid.uuid, 0, CSR_BT_UUID128_SIZE);
        msg->uuid.uuid[0] = (CsrUint8) (((attUuid[0]) & 0x000000FF));
        msg->uuid.uuid[1] = (CsrUint8) (((attUuid[0] >> 8) & 0x000000FF));
    }
    else
    {
        msg->uuid.length = CSR_BT_UUID128_SIZE;
        CSR_COPY_UINT32_TO_LITTLE_ENDIAN(attUuid[0], &(msg->uuid.uuid[12]));    
        CSR_COPY_UINT32_TO_LITTLE_ENDIAN(attUuid[1], &(msg->uuid.uuid[8]));   
        CSR_COPY_UINT32_TO_LITTLE_ENDIAN(attUuid[2], &(msg->uuid.uuid[4]));
        CSR_COPY_UINT32_TO_LITTLE_ENDIAN(attUuid[3], (msg->uuid.uuid));
    }       
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(qElem->gattId), msg);
}

void CsrBtGattStdReadCfmSend(CsrBtGattPrim   type,
                             CsrBtGattId     gattId,
                             CsrBtResultCode resultCode,
                             CsrBtSupplier   resultSupplier,
                             CsrBtConnId     btConnId,
                             CsrUint16       valueLength,
                             CsrUint8        *value)
{
    CsrBtGattReadCfm *msg = (CsrBtGattReadCfm*)
                             CsrPmemAlloc(sizeof(CsrBtGattReadCfm));

    msg->type           = type;
    msg->gattId         = gattId;
    msg->resultCode     = resultCode;
    msg->resultSupplier = resultSupplier;
    msg->btConnId       = btConnId;
    msg->valueLength    = valueLength;
    msg->value          = value;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattReadCfmHandler(CsrBtGattReadReq   *prim,
                             CsrBtResultCode    resultCode,
                             CsrBtSupplier      resultSupplier,
                             CsrUint16          valueLength,
                             CsrUint8           **value)
{
    CsrUint8 *tmpValue = *value;

    switch (prim->flags)
    {
        case CSR_BT_GATT_READ_VALUE:
        {
            CsrBtGattStdReadCfmSend(CSR_BT_GATT_READ_CFM,
                                    prim->gattId,
                                    resultCode,
                                    resultSupplier,
                                    prim->btConnId,
                                    valueLength,
                                    tmpValue);
            *value = NULL;    
            break;
        }
        case CSR_BT_GATT_READ_EXT_PROPERTIES:
        {
            CsrBtGattReadExtendedPropertiesCfm *msg = (CsrBtGattReadExtendedPropertiesCfm*)
                                               CsrPmemAlloc(sizeof(CsrBtGattReadExtendedPropertiesCfm));

            msg->type           = CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM;
            msg->gattId         = prim->gattId;
            msg->resultCode     = resultCode;
            msg->resultSupplier = resultSupplier;
            msg->btConnId       = prim->btConnId;
            msg->extProperties  = 0;
            
            if (tmpValue && valueLength >= sizeof(CsrBtGattExtPropertiesBits))
            {
                msg->extProperties  = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(tmpValue);
            }
            CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(prim->gattId), msg);
            break;
        }
        case CSR_BT_GATT_READ_USER_DESCRIPTOR:
        {
            CsrBtGattReadUserDescriptionCfm *msg = (CsrBtGattReadUserDescriptionCfm*)
                                               CsrPmemAlloc(sizeof(CsrBtGattReadUserDescriptionCfm));

            msg->type           = CSR_BT_GATT_READ_USER_DESCRIPTION_CFM;
            msg->gattId         = prim->gattId;
            msg->resultCode     = resultCode;
            msg->resultSupplier = resultSupplier;
            msg->btConnId       = prim->btConnId;
            msg->usrDescription = CsrUtf8StrTruncate(tmpValue, valueLength);
            *value              = NULL;
            CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(prim->gattId), msg);
            break;
        }
        case CSR_BT_GATT_READ_CLIENT_CONFIGURATION:
        {
            CsrBtGattReadClientConfigurationCfm *msg = (CsrBtGattReadClientConfigurationCfm*)
                                               CsrPmemAlloc(sizeof(CsrBtGattReadClientConfigurationCfm));

            msg->type           = CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM;
            msg->gattId         = prim->gattId;
            msg->resultCode     = resultCode;
            msg->resultSupplier = resultSupplier;
            msg->btConnId       = prim->btConnId;
            msg->configuration  = 0;
            
            if (tmpValue && valueLength >= sizeof(CsrBtGattCliConfigBits))
            {
                msg->configuration  = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(tmpValue);
            }
            CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(prim->gattId), msg);
            break;
        }
        case CSR_BT_GATT_READ_SERVER_CONFIGURATION:
        {
            CsrBtGattReadServerConfigurationCfm *msg = (CsrBtGattReadServerConfigurationCfm*)
                                               CsrPmemAlloc(sizeof(CsrBtGattReadServerConfigurationCfm));

            msg->type           = CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM;
            msg->gattId         = prim->gattId;
            msg->resultCode     = resultCode;
            msg->resultSupplier = resultSupplier;
            msg->btConnId       = prim->btConnId;
            msg->configuration  = 0;
            
            if (tmpValue && valueLength >= sizeof(CsrBtGattSrvConfigBits))
            {
                msg->configuration  = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(tmpValue);
            }
            CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(prim->gattId), msg);
            break;
        }
        case CSR_BT_GATT_READ_PRESENTATION_FORMAT:
        {
            CsrBtGattReadPresentationFormatCfm *msg = (CsrBtGattReadPresentationFormatCfm*)
                                               CsrPmemZalloc(sizeof(CsrBtGattReadPresentationFormatCfm));

            msg->type           = CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM;
            msg->gattId         = prim->gattId;
            msg->resultCode     = resultCode;
            msg->resultSupplier = resultSupplier;
            msg->btConnId       = prim->btConnId;
            
            if (tmpValue && valueLength >= CSR_BT_GATT_CHARAC_PRESENTATION_FORMAT_LENGTH)
            {
                msg->format      = tmpValue[0];
                msg->exponent    = tmpValue[1];
                msg->unit        = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&tmpValue[2]);
                msg->nameSpace   = tmpValue[4];
                msg->description = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&tmpValue[5]);
            }
            CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(prim->gattId), msg);
            break;
        }
        case CSR_BT_GATT_READ_AGGREGATE_FORMAT:
        {
            CsrBtGattReadAggregateFormatCfm *msg = (CsrBtGattReadAggregateFormatCfm*)
                                               CsrPmemZalloc(sizeof(CsrBtGattReadAggregateFormatCfm));

            msg->type           = CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM;
            msg->gattId         = prim->gattId;
            msg->resultCode     = resultCode;
            msg->resultSupplier = resultSupplier;
            msg->btConnId       = prim->btConnId;

            if (tmpValue && (valueLength % 2) == 0)
            {
                CsrUint16 i, t = 0;

                msg->handles      = (CsrBtGattHandle *) CsrPmemAlloc(valueLength);
                msg->handlesCount = (CsrUint16)(valueLength / 2);
                
                for (i = 0; i < msg->handlesCount; i++)
                {
                    msg->handles[i] = CSR_GET_UINT16_FROM_LITTLE_ENDIAN(&tmpValue[t]);
                    t+=2;
                }
            }
            else
            { /* Validation fails */
                if (msg->resultCode == CSR_BT_GATT_RESULT_SUCCESS)
                {  
                    msg->resultCode = CSR_BT_GATT_RESULT_INVALID_ATTRIBUTE_VALUE_RECEIVED;
                }
                msg->handlesCount = 0;
                msg->handles      = NULL;  
            }
            CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(prim->gattId), msg);
            break;
        }
        default :
        { /* CSR_BT_GATT_READ_PROFILE_DEFINED */
            CsrBtGattStdReadCfmSend(CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM,
                                    prim->gattId,
                                    resultCode,
                                    resultSupplier,
                                    prim->btConnId,
                                    valueLength,
                                    tmpValue);
            *value = NULL;
            break;
        }
    }
}

void CsrBtGattNotificationIndSend(CsrBtGattId gattId,
                                  CsrBtConnId btConnId,
                                  CsrBtTypedAddr address,
                                  CsrBtGattHandle valueHandle,
                                  CsrUint16 valueLength,
                                  CsrUint8 *value,
                                  CsrBtGattConnInfo connInfo)
{
    CsrBtGattNotificationInd *msg = (CsrBtGattNotificationInd *)
                                     CsrPmemAlloc(sizeof(CsrBtGattNotificationInd));

    msg->type           = CSR_BT_GATT_NOTIFICATION_IND;
    msg->gattId         = gattId;
    msg->btConnId       = btConnId;
    msg->address        = address;
    msg->valueHandle    = valueHandle;
    msg->valueLength    = valueLength;
    msg->value          = value;
    msg->connInfo       = connInfo;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattReadByUuidIndSend(CsrBtGattId     gattId,
                                CsrBtConnId     btConnId,
                                CsrBtGattHandle valueHandle,
                                CsrUint16       valueLength,
                                CsrUint8        *value)
{
    CsrBtGattReadByUuidInd *msg = (CsrBtGattReadByUuidInd *)
                                   CsrPmemAlloc(sizeof(CsrBtGattReadByUuidInd));

    msg->type           = CSR_BT_GATT_READ_BY_UUID_IND;
    msg->gattId         = gattId;
    msg->btConnId       = btConnId;
    msg->valueHandle    = valueHandle;
    msg->valueLength    = valueLength;
    msg->value          = value;
    CsrSchedMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), CSR_BT_GATT_PRIM, msg);
}

void CsrBtGattWhitelistReadCfmSend(CsrBtGattId gattId,
                                   CsrUint16 addressCount,
                                   CsrBtTypedAddr *address)
{
    CsrBtGattWhitelistReadCfm *msg = (CsrBtGattWhitelistReadCfm *) 
                                      CsrPmemZalloc(sizeof(CsrBtGattWhitelistReadCfm));

    msg->type = CSR_BT_GATT_WHITELIST_READ_CFM;
    msg->addressCount = addressCount;
    msg->address = address;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattPhysicalLinkStatusIndSend(CsrBtGattId       gattId, 
                                        CsrBtTypedAddr    address,
                                        CsrBtGattConnInfo connInfo,
                                        CsrBool           status)
{
    CsrBtGattPhysicalLinkStatusInd *msg = (CsrBtGattPhysicalLinkStatusInd *)
                                           CsrPmemAlloc(sizeof(CsrBtGattPhysicalLinkStatusInd));

    msg->type     = CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND;
    msg->gattId   = gattId;
    msg->status   = status;
    msg->address  = address;   
    msg->connInfo = connInfo;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId), msg);
}

void CsrBtGattAccessReadIndSend(CsrSchedQid appHandle,
                                CsrBtGattId gattId,
                                CsrBtConnId btConnId,
                                CsrBtGattHandle handle,
                                CsrUint16 offset,
                                CsrUint16 mtu,
                                CsrBtGattAccessCheck check,
                                CsrBtGattConnInfo connInfo,
                                CsrBtTypedAddr address)
{
    CsrBtGattDbAccessReadInd *msg;
    msg = CsrPmemAlloc(sizeof(CsrBtGattDbAccessReadInd));

    msg->type = CSR_BT_GATT_DB_ACCESS_READ_IND;
    msg->gattId = gattId;
    msg->btConnId = btConnId;
    msg->attrHandle = handle;
    msg->offset = offset;
    msg->maxRspValueLength  = mtu;
    msg->check = check;
    msg->connInfo = connInfo;
    msg->address = address;

    CsrBtGattMessagePut(appHandle,  msg);
}

void CsrBtGattAccessWriteIndSend(CsrSchedQid appHandle,
                                 CsrBtGattId gattId,
                                 CsrBtConnId btConnId,
                                 CsrBtGattAccessCheck check,
                                 CsrBtGattConnInfo connInfo,
                                 CsrBtTypedAddr address,
                                 CsrUint16 writeUnitCount,
                                 CsrBtGattAttrWritePairs *writeUnit,
                                 CsrBtGattHandle handle)
{
    CsrBtGattDbAccessWriteInd *msg;
    msg = CsrPmemAlloc(sizeof(CsrBtGattDbAccessWriteInd));

    msg->type = CSR_BT_GATT_DB_ACCESS_WRITE_IND;
    msg->gattId = gattId;
    msg->btConnId = btConnId;
    msg->check = check;
    msg->connInfo = connInfo;
    msg->address = address;
    msg->writeUnitCount = writeUnitCount;
    msg->writeUnit = writeUnit;
    msg->attrHandle = handle;
            
    CsrBtGattMessagePut(appHandle, msg);
}

void CsrBtGattDbAccessCompleteIndSend(CsrSchedQid appHandle,
                                      CsrBtGattId gattId,
                                      CsrBtConnId btConnId,
                                      CsrBtGattConnInfo connInfo,
                                      CsrBtTypedAddr address,
                                      CsrBtGattHandle attrHandle,
                                      CsrBool commit)
{
    CsrBtGattDbAccessCompleteInd *msg;
    msg = CsrPmemAlloc(sizeof(CsrBtGattDbAccessCompleteInd));

    msg->type = CSR_BT_GATT_DB_ACCESS_COMPLETE_IND;
    msg->gattId = gattId;
    msg->btConnId = btConnId;
    msg->connInfo = connInfo;
    msg->address = address;
    msg->attrHandle = attrHandle;
    msg->commit = commit;

    CsrBtGattMessagePut(appHandle, msg);
}

void CsrBtGattWhitelistChangeIndSend(CsrBtGattId gattId)
{
    CsrBtGattWhitelistChangeInd *msg;

    msg = (CsrBtGattWhitelistChangeInd*)CsrPmemAlloc(sizeof(CsrBtGattWhitelistChangeInd));

    msg->type     = CSR_BT_GATT_WHITELIST_CHANGE_IND;
    msg->gattId   = gattId;
    CsrBtGattMessagePut(CSR_BT_GATT_GET_QID_FROM_GATT_ID(gattId),
                       msg);
}
