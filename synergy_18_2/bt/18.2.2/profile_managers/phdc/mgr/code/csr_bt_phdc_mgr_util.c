/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE

#include "csr_pmem.h"

#include "csr_bt_phdc_mgr_dev_spec.h"
#include "csr_bt_phdc_ieee_asn.h"
#include "csr_bt_phdc_ieee_asn_nom_codes.h"
#include "csr_bt_phdc_mgr_util.h"

/* Link List function for Device List */
void CsrBtPhdcMgrCmnListDeviceAdd(CsrCmnListElm_t *elem)
{                        
    ((PhdcMgrDeviceListEntry *)elem)->pMds   = CsrPmemAlloc(sizeof(PhdcMgrMDSInfo));
    ((PhdcMgrDeviceListEntry *)elem)->pMds->configStatus = ACCEPTED_UNKNOWN_CONFIG;
    ((PhdcMgrDeviceListEntry *)elem)->pMds->systemId = NULL;
    ((PhdcMgrDeviceListEntry *)elem)->pMds->pSpec = NULL;
    ((PhdcMgrDeviceListEntry *)elem)->pMds->pDataProto = NULL;
    ((PhdcMgrDeviceListEntry *)elem)->pMds->pSavedApdu = NULL;
        
    CsrCmnListInit(&(((PhdcMgrDeviceListEntry *)elem)->pMds->configObjList), 
                   0, 
                   CsrBtPhdcMgrCmnListObjectAdd, 
                   CsrBtPhdcMgrCmnListObjectRemove);
    
    ((PhdcMgrDeviceListEntry *)elem)->invokeIdTx  = 0;
    ((PhdcMgrDeviceListEntry *)elem)->invokeIdAck = 0;
    ((PhdcMgrDeviceListEntry *)elem)->invokeIdRcv = 0;
    ((PhdcMgrDeviceListEntry *)elem)->isDisUserIntiated = FALSE;
    ((PhdcMgrDeviceListEntry *)elem)->aarqOngoing = AARQ_ONGOING_FALSE;
    ((PhdcMgrDeviceListEntry *)elem)->timerId = CSR_BT_PHDC_MGR_INVALID_VAL;
    ((PhdcMgrDeviceListEntry *)elem)->state = CSR_BT_PHDC_MGR_DEVICE_ST_DISCONNECTED;
    ((PhdcMgrDeviceListEntry *)elem)->mdepId = 0;

    /* PARAMS previously NOT INITIALISED !!!*/
   
    ((PhdcMgrDeviceListEntry *)elem)->bdAddr.nap = 0;
    ((PhdcMgrDeviceListEntry *)elem)->bdAddr.uap = 0;
    ((PhdcMgrDeviceListEntry *)elem)->bdAddr.lap = 0;
    ((PhdcMgrDeviceListEntry *)elem)->dataChannelId = 0;
    
    ((PhdcMgrDeviceListEntry *)elem)->maxPacketLength = 0;        
    ((PhdcMgrDeviceListEntry *)elem)->psmIdentifier = 0;          
    ((PhdcMgrDeviceListEntry *)elem)->reason = 0;
    ((PhdcMgrDeviceListEntry *)elem)->fsmParam = NULL;
}

void CsrBtPhdcMgrCmnListDeviceRemove(CsrCmnListElm_t *elem)
{
    CsrPmemFree(((PhdcMgrDeviceListEntry *)elem)->pMds);
}

CsrBool CsrBtPhdcMgrCompareRemoteBdAddr(CsrCmnListElm_t *elem, void *data)
{
    return CsrBtBdAddrEq(&((PhdcMgrDeviceListEntry *)elem)->bdAddr, (BD_ADDR_T *)data);
}

CsrBool CsrBtPhdcMgrCompareDataChannelId(CsrCmnListElm_t *elem, void *data)
{
    return ( ( (PhdcMgrDeviceListEntry *)elem)->dataChannelId ==  *((CsrUint32 *)data) ) ;
}

/* Link List function for Object List */
void CsrBtPhdcMgrCmnListObjectAdd(CsrCmnListElm_t *elem)
{
     ((PhdcMgrConfigObjectEntry *)elem)->objHandle  = 0xFF;
     ((PhdcMgrConfigObjectEntry *)elem)->unitCode   = 0xFF;
     ((PhdcMgrConfigObjectEntry *)elem)->useAVM = TRUE;     
     ((PhdcMgrConfigObjectEntry *)elem)->pAttrValMap = NULL;
     ((PhdcMgrConfigObjectEntry *)elem)->pMetricIdList = NULL;
 }

void CsrBtPhdcMgrCmnListObjectRemove(CsrCmnListElm_t *elem)
{
    if (((PhdcMgrConfigObjectEntry *)elem)->pAttrValMap != NULL )
    {
        if ( (((PhdcMgrConfigObjectEntry *)elem)->pAttrValMap)->length > 0 )
        {
            CsrPmemFree((((PhdcMgrConfigObjectEntry *)elem)->pAttrValMap)->value);
        }
        CsrPmemFree(((PhdcMgrConfigObjectEntry *)elem)->pAttrValMap);
    }

    if (((PhdcMgrConfigObjectEntry *)elem)->pMetricIdList != NULL )
    {
        if ( (((PhdcMgrConfigObjectEntry *)elem)->pMetricIdList)->length > 0 )
        {
            CsrPmemFree((((PhdcMgrConfigObjectEntry *)elem)->pMetricIdList)->value);
        }
        CsrPmemFree(((PhdcMgrConfigObjectEntry *)elem)->pMetricIdList);
    }        
}


CsrBool CsrBtPhdcMgrCompareObjectHandle(CsrCmnListElm_t *elem, void *data)
{
    return (( (PhdcMgrConfigObjectEntry *)elem)->objHandle ==  *((HANDLE *)data));
}

CsrBool CsrBtPhdcMgrCompareObjectType(CsrCmnListElm_t *elem, void *data)
{
    return (( (PhdcMgrConfigObjectEntry *)elem)->objType ==  *((CsrUint32 *)data));
}


/* Link List function for Type Spec List */
void CsrBtPhdcMgrCmnListTypeSpecAdd(CsrCmnListElm_t *elem)
{
    ((PhdcMgrDeviceDataTypeEntry *)elem)->dataType     = 0;
    ((PhdcMgrDeviceDataTypeEntry *)elem)->deviceSpec   = 0;
}

void CsrBtPhdcMgrCmnListTypeSpecRemove(CsrCmnListElm_t *elem)
{
    /* No special needs */
    CSR_UNUSED(elem);
}


CsrBool CsrBtPhdcMgrCompareMdepDataType(CsrCmnListElm_t *elem, void *data)
{
    return ( (PhdcMgrDeviceDataTypeEntry *)elem)->dataType ==  *((CsrBtMdepDataType *)data);
}

CsrBool CsrBtPhdcMgrCompareTypeSpec(CsrCmnListElm_t *elem, void *data)
{
    return ( (PhdcMgrDeviceDataTypeEntry *)elem)->deviceSpec ==  *((CsrUint16 *)data);
}


CsrBool CsrBtPhdcMgrCanTrasmit(PhdcMgrDeviceListEntry *pDevice)
{
    return ((pDevice->invokeIdTx - pDevice->invokeIdRcv) == 0 );
}

CsrUint16 CsrBtPhdcMgrGetInvokeIdTx(PhdcMgrDeviceListEntry *pDevice)
{
    return ((pDevice->invokeIdTx - pDevice->invokeIdRcv) == 0  ? ++pDevice->invokeIdTx :
                                                                 pDevice->invokeIdTx);
}

void  CsrBtPhdcMgrUpdateInvokeIdRcv(PhdcMgrDeviceListEntry *pDevice)
{
    pDevice->invokeIdRcv++;

    CSR_BT_PHDC_M_ASSERT((pDevice->invokeIdTx - pDevice->invokeIdRcv) == 0);
}


void  CsrBtPhdcMgrUpdateInvokeIdAck(APDU *pApdu,PhdcMgrDeviceListEntry *pDevice )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;

    pDevice->invokeIdAck = pData->invoke_id;
    
}

CsrUint16 PhdcMgrMapDataTypeToIEEE(CsrBtMdepDataType   datatype)
{
    CsrUint16 deviceSpec;
    switch (datatype)
    {
        case CSR_BT_HDP_DEVICE_TYPE_BP:
        {
            deviceSpec = MDC_DEV_SPEC_PROFILE_BP;
            break;
        }
        case CSR_BT_HDP_DEVICE_TYPE_SCALE:
        {
            deviceSpec = MDC_DEV_SPEC_PROFILE_SCALE;
            break;
        }
        default :
            deviceSpec = CSR_BT_PHDC_MGR_INVALID_VAL;
            
    }    
    return deviceSpec;
}


static CsrUint16 CsrBtPhdcMgrGetUint16(const CsrUint8 *msg_ptr, CsrUint16 *val)
{
   *val = (msg_ptr[0]  <<  8) | msg_ptr[1] ;
   
    return 2;
}

void CsrBtPhdcMgrFreeDataProtoContents(DataProto  *pData)
{
    pData->data_proto_id = DATA_PROTO_ID_EMPTY;
    pData->data_proto_info.length = 0;
    if (pData->data_proto_info.value != NULL)
    {
        CsrUint16 i;
        PhdAssociationInformation *assoc = (PhdAssociationInformation *)pData->data_proto_info.value;
        CsrPmemFree(assoc->systemId.value);
        
        for (i = 0; i < assoc->optionList.count; i++)
        {
            CsrPmemFree(assoc->optionList.value[i].attribute_value.value);
        }
        CsrPmemFree(assoc->optionList.value);
    }
    CsrPmemFree(pData->data_proto_info.value);
}

Associate_result CsrBtPhdcMgrIsAssociationComptabile(AARQ_apdu *pAarq,PhdcMgrMDSInfo *pMds )
{
    DataProto   *pData = NULL;
    CsrInt16    i;
    
    PhdAssociationInformation *pAssocInfo = NULL;

    if ( pAarq->data_proto_list.count == 0 )
    {
        return REJECTED_NO_COMMON_PARAMETER;
    }

    pData = (DataProto   *)pAarq->data_proto_list.value;

    for (i=0 ; i <  pAarq->data_proto_list.count; i++,pData++)
    {
        if (pData->data_proto_id == DATA_PROTO_ID_20601 )
        {
            pMds->pDataProto = pData;
            break;
        }
        else
        {
            CsrBtPhdcMgrFreeDataProtoContents(pData);
        }
    }
     
    if (i == pAarq->data_proto_list.count)
    {/* Make sure that 'pData' points either at the element found (if 'i' is not equal to 'pAarq->data_proto_list.count')
        or that it points at the last valid elemet... */
        pData = (DataProto   *)pAarq->data_proto_list.value;
    }

    if ( pData->data_proto_id != DATA_PROTO_ID_20601)
    {
        /* Fill up pData as it will be used for encoding
           of Aare in the accept and reject cases */
        /* pData->data_proto_id = DATA_PROTO_ID_EMPTY;
           pData->data_proto_info.length = 0;*/
        /* De-allocate the value pointer and all the pointers that may be contained in it */
        
        CsrPmemFree(pData->data_proto_info.value);
        pData->data_proto_info.value = NULL;

        pMds->pDataProto = pData;
        
        return REJECTED_NO_COMMON_PROTOCOL;
    }
    
    pAssocInfo = (PhdAssociationInformation *)(pData->data_proto_info.value);

    if ( pAarq->assoc_version != ASSOC_VERSION1 )
    {
        return REJECTED_UNSUPPORTED_ASSOC_VERSION;
    }

    if ((pAssocInfo->data_req_mode_capab.data_req_mode_flags & DATA_REQ_SUPP_INIT_AGENT) != DATA_REQ_SUPP_INIT_AGENT)
    {
        return REJECTED_NO_COMMON_PARAMETER;
    }

    if (pAssocInfo->data_req_mode_capab.data_req_init_agent_count != 1)
    {
        return REJECTED_NO_COMMON_PARAMETER;
    }

    if ( ( pAssocInfo->encodingRules & MDER ) != MDER)
    {
        return REJECTED_NO_COMMON_PARAMETER;
    }
    
    if (pAssocInfo->nomenclatureVersion != NOM_VERSION1)
    {
        return REJECTED_NO_COMMON_PARAMETER;
    }

    if (pAssocInfo->protocolVersion != PROTOCOL_VERSION1 && pAssocInfo->protocolVersion != PROTOCOL_VERSION1_1)
    {
        return REJECTED_NO_COMMON_PARAMETER;   
    }

    if (pAssocInfo->systemType != SYS_TYPE_AGENT)
    {
       return REJECTED_NO_COMMON_PARAMETER;  
    }

    CsrPmemFree(pMds->systemId);
    pMds->systemId = CsrPmemAlloc(pAssocInfo->systemId.length);
    CsrMemCpy(pMds->systemId, pAssocInfo->systemId.value, pAssocInfo->systemId.length);

    pMds->devConfigId = pAssocInfo->dev_config_id;    

    return pMds->configStatus;
}


CsrBool CsrBtPhdcMgrIsMDC_NOTI_CONFIG(APDU *pApdu )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;

    if ( pApdu->choice != PRST_CHOSEN)
    {
        return FALSE;
    }

    if (pData->choice.choice != ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
    {
        return FALSE;
    }

    if (pRoiv->event_type != MDC_NOTI_CONFIG)
    {
        return FALSE;
    }

    return TRUE;
}


CsrBool CsrBtPhdcMgrUpdateDevConfig(APDU *pApdu, PhdcMgrMDSInfo *pMds )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
    ConfigReport *pConfig = (ConfigReport *)pRoiv->event_info.value;
    CsrBool ret = TRUE;
    
    if (pConfig->config_report_id != pMds->devConfigId)
    {
        ret = FALSE;
    }
    else
    {
        pMds->configStatus = ACCEPTED;
    }

    return ret;
}

static CsrUint16 CsrBtPhdcMgrDecodeAttrValMapEntry(CsrUint8 *msgPtr,AttrValMapEntry *pAttrValMapEntry)
{
    CsrUint16 j = 0;

    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pAttrValMapEntry->attribute_id);
    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pAttrValMapEntry->attribute_len);

    return j;
}


CsrUint16 CsrBtPhdcMgrDecodeAttrValMap(CsrUint8 *msgPtr,AttrValMap **ppAttrValMap)
{
    CsrUint16 i,j = 0;
    AttrValMap *pAttrValMap;
    AttrValMapEntry *pAttrValMapEntry;     

    *ppAttrValMap = CsrPmemAlloc(sizeof(AttrValMap));
    pAttrValMap = *ppAttrValMap;

    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pAttrValMap->count);
    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pAttrValMap->length);

    pAttrValMap->value = CsrPmemAlloc(sizeof(AttrValMapEntry) * pAttrValMap->count);
    pAttrValMapEntry = pAttrValMap->value ;    

    for(i = 0; i < pAttrValMap->count; i++,pAttrValMapEntry++)
    {
        j += CsrBtPhdcMgrDecodeAttrValMapEntry(&msgPtr[j],pAttrValMapEntry);

    }

    return j;
}

CsrUint16 CsrBtPhdcMgrDecodeMetricIdList(CsrUint8 *msgPtr,MetricIdList **ppMetricIdList)
{
    CsrUint16 i,j = 0;
    MetricIdList *pMetricIdList;
    OID_Type *pOID_Type;     

    *ppMetricIdList = CsrPmemAlloc(sizeof(MetricIdList));
    pMetricIdList = *ppMetricIdList;

    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pMetricIdList->count);
    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pMetricIdList->length);

    pMetricIdList->value = CsrPmemAlloc(sizeof(OID_Type) * pMetricIdList->count);
    pOID_Type = pMetricIdList->value ;    

    for(i = 0; i < pMetricIdList->count; i++,pOID_Type++)
    {
        j += CsrBtPhdcMgrGetUint16(&msgPtr[j],pOID_Type);

    }

    return j;
}

CsrUint16 CsrBtPhdcMgrDecodeBasicNuObsValueCmp(CsrUint8 *msgPtr,BasicNuObsValueCmp **ppBasicNuObsValueCmp)
{
    CsrUint16 i,j = 0;
    BasicNuObsValueCmp *pBasicNuObsValueCmp;
    BasicNuObsValue *pBasicNuObsValue;     

    *ppBasicNuObsValueCmp = CsrPmemAlloc(sizeof(AttrValMap));
    pBasicNuObsValueCmp = *ppBasicNuObsValueCmp;

    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pBasicNuObsValueCmp->count);
    j += CsrBtPhdcMgrGetUint16(&msgPtr[j],&pBasicNuObsValueCmp->length);

    pBasicNuObsValueCmp->value = CsrPmemAlloc(sizeof(OID_Type) * pBasicNuObsValueCmp->count);
    pBasicNuObsValue = pBasicNuObsValueCmp->value ;    

    for(i = 0; i < pBasicNuObsValueCmp->count; i++,pBasicNuObsValue++)
    {
        j += CsrBtPhdcMgrGetUint16(&msgPtr[j],pBasicNuObsValue);

    }

    return j;
}




AttrValMap * CsrBtPhdcMgrGetAttrValMap(ConfigObject *pConfigObj)
{
    CsrUint16 c = pConfigObj->attributes.count,i;
    AVA_Type *pAttribute = (AVA_Type*)pConfigObj->attributes.value;

    AttrValMap *pAttrValMap = NULL;

    for (i = 0; i < c ; i++,pAttribute++)
    {
        if (pAttribute->attribute_id == MDC_ATTR_ATTRIBUTE_VAL_MAP)
        {
            (void)CsrBtPhdcMgrDecodeAttrValMap(pAttribute->attribute_value.value, &pAttrValMap);

            break;
        }
    }

    return (pAttrValMap);

}

MetricIdList * CsrBtPhdcMgrGetMetricIdList(ConfigObject *pConfigObj)
{
    CsrUint16 c = pConfigObj->attributes.count,i;
    AVA_Type *pAttribute = (AVA_Type*)pConfigObj->attributes.value;

    MetricIdList *pMetricIdList = NULL;

    for (i = 0; i < c ; i++,pAttribute++)
    {
        if (pAttribute->attribute_id == MDC_ATTR_ID_PHYSIO_LIST)
        {
            (void)CsrBtPhdcMgrDecodeMetricIdList(pAttribute->attribute_value.value, &pMetricIdList);

            break;
        }
    }
    return (pMetricIdList);
}

CsrUint16 CsrBtPhdcMgrGetNoOfConfigObj(APDU *pApdu )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
    ConfigReport *pConfig = (ConfigReport *)pRoiv->event_info.value;

    return pConfig->config_obj_list.count;
}

ConfigObject* CsrBtPhdcMgrGetConfigObject(APDU *pApdu,CsrUint8 idx )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
    ConfigReport *pConfig = (ConfigReport *)pRoiv->event_info.value;
    ConfigObject *pConfigObj = (ConfigObject *)pConfig->config_obj_list.value;

    return (pConfigObj+idx);
}

CsrUint16 CsrBtPhdcMgrGetUnitCode(ConfigObject *pConfigObj)
{
    CsrUint16 c = pConfigObj->attributes.count,i;
    AVA_Type *pAttribute = (AVA_Type*)pConfigObj->attributes.value;
    CsrUint16 unitCode = 0xFF;

    for (i = 0; i < c ; i++,pAttribute++)
    {
        if (pAttribute->attribute_id == MDC_ATTR_UNIT_CODE)
        {
            CsrBtPhdcMgrGetUint16(pAttribute->attribute_value.value,&unitCode);
            break;
        }
    }

    return (unitCode);
}

CsrUint32 CsrBtPhdcMgrGetObjectType(ConfigObject *pConfigObj)
{
    CsrUint16 c = pConfigObj->attributes.count,i;
    AVA_Type *pAttribute = (AVA_Type*)pConfigObj->attributes.value;
    CsrUint32 objectType = 0xFFFFFFFF;

    for (i = 0; i < c ; i++,pAttribute++)
    {
        if (pAttribute->attribute_id == MDC_ATTR_ID_TYPE)
        {
            objectType = ((pAttribute->attribute_value.value[0] << 24) |
                          (pAttribute->attribute_value.value[1] << 16) |
                          (pAttribute->attribute_value.value[2] << 8) |
                          (pAttribute->attribute_value.value[3] ));
            break;
        }
    }

    return (objectType);
}

CsrBool CsrBtPhdcMgrIsGetRsp(APDU *pApdu )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    
    if ( pApdu->choice != PRST_CHOSEN)
    {
        return FALSE;
    }

    if (pData->choice.choice != RORS_CMIP_GET_CHOSEN)
    {
        return FALSE;
    }

    return TRUE;
}

static void CsrBtPhdcMgrDecodeTypeVerList(CsrUint8 *msg_ptr,TypeVerList **ppSpec)
{
    CsrUint16 i,j=0;
    TypeVer *pVer;
    TypeVerList *pSpec;

    *ppSpec = CsrPmemAlloc(sizeof(TypeVerList));
    pSpec = *ppSpec;
    
    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pSpec->count);
    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pSpec->length);

    pSpec->value = CsrPmemAlloc(sizeof(TypeVer) * pSpec->count);
    pVer = pSpec->value ;
    
    for(i = 0; i < pSpec->count; i++,pVer++)
    {
        j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pVer->type);
        j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pVer->version);
    }   
}

static CsrUint16 CsrBtPhdcMgrDecodeAuthBodyAndStrucType(CsrUint8 *msg_ptr, AuthBodyAndStrucType *pAuthBodyAndStrucType)
{
    CsrUint16 j = 0;

    pAuthBodyAndStrucType->auth_body = msg_ptr[j];
    j++;

    pAuthBodyAndStrucType->auth_body_struc_type = msg_ptr[j];
    j++;

    return j;
}

static CsrUint16 CsrBtPhdcMgrDecodeCertifiedDeviceClassList(CsrUint8 *msg_ptr,CertifiedDeviceClassList **ppCertDeviceClassList)
{
    CsrUint16 i,j=0;
    CertifiedDeviceClassEntry *pDeviceClassEntry;
    CertifiedDeviceClassList *pCertDeviceClassList;

    *ppCertDeviceClassList = CsrPmemAlloc(sizeof(CertifiedDeviceClassList));
    pCertDeviceClassList = *ppCertDeviceClassList;
    
    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pCertDeviceClassList->count);
    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pCertDeviceClassList->length);

    pCertDeviceClassList->value = CsrPmemAlloc(sizeof(CertifiedDeviceClassEntry) * pCertDeviceClassList->count);
    pDeviceClassEntry = pCertDeviceClassList->value;
    
    for(i = 0; i < pCertDeviceClassList->count; i++,pDeviceClassEntry++)
    {
        j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],pDeviceClassEntry);

    }   

    return j;
}

static CsrUint16 CsrBtPhdcMgrDecodeContinuaBodyStruct(CsrUint8 *msg_ptr,ContinuaBodyStruct **ppBodyStruct)
{
    CsrUint16  j = 0;
    ContinuaBodyStruct *pBodyStruct;
    CertifiedDeviceClassList *pCertDeviceClassList = NULL;

    *ppBodyStruct = CsrPmemAlloc(sizeof(ContinuaBodyStruct));
    pBodyStruct = *ppBodyStruct;

    pBodyStruct->major_IG_version = msg_ptr[j];
    j++;

    pBodyStruct->minor_IG_version = msg_ptr[j];
    j++;

    j += CsrBtPhdcMgrDecodeCertifiedDeviceClassList(&msg_ptr[j], &pCertDeviceClassList);
    
    pBodyStruct->certified_devices = *pCertDeviceClassList;
    CsrPmemFree(pCertDeviceClassList);

    return j;    
}

static CsrUint16 CsrBtPhdcMgrDecodeContinuaRegStruct(CsrUint8 *msg_ptr,ContinuaRegStruct **ppRegStruct)
{
    CsrUint16  j = 0;

    *ppRegStruct = CsrPmemAlloc(sizeof(ContinuaRegStruct));
    
    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&((*ppRegStruct)->regulation_bit_field));

    return j;    
}


static CsrUint16 CsrBtPhdcMgrDecodeAuthBodyData(CsrUint8 *msg_ptr,AuthBody authBody,AuthBodyStrucType structType,Any *pAuthBodyAndStrucType)
{
    CsrUint16 j = 0;

    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pAuthBodyAndStrucType->length);

    switch (authBody)
    {
        case AUTH_BODY_CONTINUA :
        {
            if ( structType == CONTINUA_REG_STRUCT)
            {
                j += CsrBtPhdcMgrDecodeContinuaRegStruct(&msg_ptr[j], (ContinuaRegStruct ** )&pAuthBodyAndStrucType->value);
            }
            else if (structType == CONTINUA_VERSION_STRUCT)
            {
                j += CsrBtPhdcMgrDecodeContinuaBodyStruct(&msg_ptr[j], (ContinuaBodyStruct ** )&pAuthBodyAndStrucType->value);
            }
            else
            {
                j += pAuthBodyAndStrucType->length;
                pAuthBodyAndStrucType->value = NULL;
            }
            
            break;
        }
        default :
        {
            j += pAuthBodyAndStrucType->length;
            pAuthBodyAndStrucType->value = NULL;
            break;
        }
            
    }

    return j;
}


static CsrUint16 CsrBtPhdcMgrDecodeRegCertData(CsrUint8 *msg_ptr, RegCertData *pRegCertData )
{
    CsrUint16 j = 0;
    
    j += CsrBtPhdcMgrDecodeAuthBodyAndStrucType(&msg_ptr[j],&pRegCertData->auth_body_and_struc_type);
    j += CsrBtPhdcMgrDecodeAuthBodyData(&msg_ptr[j],
                            pRegCertData->auth_body_and_struc_type.auth_body,
                            pRegCertData->auth_body_and_struc_type.auth_body_struc_type,
                            &pRegCertData->auth_body_data);

    return j;
}

static void CsrBtPhdcMgrDecodeRegCertDataList(CsrUint8 *msg_ptr,RegCertDataList **ppRegCertDataList)
{
    CsrUint16 i,j=0;
    RegCertData *pRegCertData;
    RegCertDataList *pRegCertDataList;

    *ppRegCertDataList = CsrPmemAlloc(sizeof(RegCertDataList));
    pRegCertDataList = *ppRegCertDataList;
    
    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pRegCertDataList->count);
    j += CsrBtPhdcMgrGetUint16(&msg_ptr[j],&pRegCertDataList->length);

    pRegCertDataList->value = CsrPmemAlloc(sizeof(RegCertData) * pRegCertDataList->count);
    pRegCertData = pRegCertDataList->value ;
    
    for(i = 0; i < pRegCertDataList->count; i++,pRegCertData++)
    {
        j += CsrBtPhdcMgrDecodeRegCertData(&msg_ptr[j],pRegCertData);
    }   
}


CsrBool CsrBtPhdcMgrGetMDC_ATTR_SYS_TYPE_SPEC_LIST(APDU *pApdu,TypeVerList **ppSpec )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    GetResultSimple *pRorsGet = (GetResultSimple*)&pData->choice.u.rors_cmipGet;
    AVA_Type    *pAttribute;
    CsrUint8 i;
    CsrUint16 noAttrib;
    CsrBool  bFound = FALSE;

    noAttrib = pRorsGet->attribute_list.count;
    pAttribute = &pRorsGet->attribute_list.value[0];
    
    for (i= 0; i < noAttrib; i++, pAttribute++)
    {
        if ( pAttribute->attribute_id == MDC_ATTR_SYS_TYPE_SPEC_LIST  )
        {
            bFound = TRUE;
            CsrBtPhdcMgrDecodeTypeVerList(&pAttribute->attribute_value.value[0],ppSpec);
            break;
        }
    }
    return bFound;
}




CsrBool CsrBtPhdcMgrIsGetRegulatoryParam(APDU *pApdu, RegCertDataList **ppRegCertDataList)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    GetResultSimple *pRorsGet = (GetResultSimple*)&pData->choice.u.rors_cmipGet;
    CsrUint8 i;
    AVA_Type *pAttribute;
    CsrBool  bFound = FALSE;
    CsrUint16 noAttrib;

    noAttrib = pRorsGet->attribute_list.count;
    pAttribute = &pRorsGet->attribute_list.value[0];
    
    for (i= 0; i < noAttrib; i++, pAttribute++)
    {
        if ( pAttribute->attribute_id == MDC_ATTR_REG_CERT_DATA_LIST  )
        {
            bFound = TRUE;
            CsrBtPhdcMgrDecodeRegCertDataList(pAttribute->attribute_value.value, ppRegCertDataList);
            break;
        }
    }
        
    return bFound;
}

CsrBool CsrBtPhdcMgrIsContinua(RegCertDataList *pRegCertDataList)
{
    CsrUint8 i;
    CsrUint16 num = pRegCertDataList->count;
    RegCertData *pRegCertData = (RegCertData *)pRegCertDataList->value;
    CsrUint16  count = 0;

    for (i= 0; i < num; i++, pRegCertData++)
    {
        if ( pRegCertData->auth_body_and_struc_type.auth_body == AUTH_BODY_CONTINUA  )
        {
            count++;
        }
    }
    /* check for 2 auth_body's. 
        for version and regulatory struct */
    return (count == 2 );
}

ContinuaBodyStruct * CsrBtPhdcMgrGetContinuaStruct(RegCertDataList *pRegCertDataList)
{
    CsrUint8 i;
    CsrUint16 num = pRegCertDataList->count;
    ContinuaBodyStruct *pContinuaInfo = NULL;
    RegCertData *pRegCertData = (RegCertData *)pRegCertDataList->value;

    for (i= 0; i < num; i++, pRegCertData++)
    {
        if ( pRegCertData->auth_body_and_struc_type.auth_body_struc_type == CONTINUA_VERSION_STRUCT  )
        {
            pContinuaInfo =  ( ContinuaBodyStruct *) pRegCertData->auth_body_data.value;            
            break;
        }
    }
    
    return pContinuaInfo;
}


CsrUint16 CsrBtPhdcMgrHashHdp2ContinuaMdepDataType(CsrBtMdepDataType  dataType)
{
    CsrUint16 type;
    switch (dataType)
    {
        case CSR_BT_HDP_DEVICE_TYPE_OXIM:
        {
            type = PAN_PULSE_OXIMETER;
            break;
        }
        case CSR_BT_HDP_DEVICE_TYPE_BP: 
        {
            type = PAN_BLOODPRESSURE_MONITOR;
            break;
        }            
        case CSR_BT_HDP_DEVICE_TYPE_TEMP: 
        {
            type = PAN_THERMOMETER;
            break;
        }            
        case CSR_BT_HDP_DEVICE_TYPE_SCALE: 
        {
            type = PAN_WEIGHING_SCALE;
            break;
        }                
            
        case CSR_BT_HDP_DEVICE_TYPE_GLUCOSE:
        {
            type = PAN_GLUCOSE_METER;
            break;
        }              
        case CSR_BT_HDP_DEVICE_TYPE_HF_CARDIO: 
        {
            type = PAN_CARDIOVASCULAR_FITNESS;
            break;
        }            
        case CSR_BT_HDP_DEVICE_TYPE_HF_STRENGTH: 
        {
            type = PAN_STRENGTH_FITNESS;
            break;
        }             
        case CSR_BT_HDP_DEVICE_TYPE_ACTIVITY_HUB:
        {
            type = PAN_ACTIVITY_HUB;
            break;
        }                   
        default:
        {
            type =  CSR_BT_PHDC_MGR_INVALID_VAL;
            break;
        }
    }

    return type;
}

CsrBool CsrBtPhdcMgrIsContinuaCertifiedCompatible(PhdcMgrDeviceDataTypeEntry *pDataTypeEntry,ContinuaBodyStruct *pContinuaInfo)
{
    CsrUint8 i;
    CsrUint16 num = pContinuaInfo->certified_devices.count,panDeviceType;
    CertifiedDeviceClassEntry *pDeviceClassEntry = (CertifiedDeviceClassEntry *)pContinuaInfo->certified_devices.value;
    CsrBool  bFound = FALSE;


    if (pContinuaInfo->major_IG_version != 1)
    {
        return FALSE;
    }
    
    if (pContinuaInfo->minor_IG_version != 0)
    {
        return FALSE;
    }

    panDeviceType = CsrBtPhdcMgrHashHdp2ContinuaMdepDataType(pDataTypeEntry->dataType);
    
    pContinuaInfo = NULL;

    for (i= 0; i < num; i++, pDeviceClassEntry++)
    {
        if ( *pDeviceClassEntry & panDeviceType  )
        {
            bFound = TRUE;
            break;
        }
    }
    
    return bFound;
}

ConfigResult CsrBtPhdcMgrIsSupportedStandardConfig(CsrUint32 devConfigId)
{
    ConfigResult supported = UNSUPPORTED_CONFIG;

    if((devConfigId >= STANDARD_CONFIG_START) &&
        (devConfigId <= STANDARD_CONFIG_END))
    {
        if((devConfigId == CSR_BT_PHDC_MGR_DEVICE_CONFIG_ID_BPM) ||
            (devConfigId == CSR_BT_PHDC_MGR_DEVICE_CONFIG_ID_WS))
        {
            supported = ACCEPTED_CONFIG; /*0x00*/
        }
        else
        {
            supported = STANDARD_CONFIG_UNKNOWN;
        }
    }
    
    return supported;
}

CsrBool CsrBtPhdcMgrIsSupportedDataType(CsrBtMdepDataType dataType, PhdcMgrDeviceListEntry *pDevice )
{
    CsrUint32  objType;
    CsrBool    bSupported = FALSE;

    switch (dataType)
    {
        case CSR_BT_HDP_DEVICE_TYPE_BP :
        {
            objType = PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_PRESS_BLD_NONINV;
            if (PHDC_MGR_OBJECT_LIST_FIND_FROM_OBJ_TYPE(pDevice->pMds->configObjList,&objType) != NULL )
            {
                bSupported = TRUE;
            }
            break;
        }
        case CSR_BT_HDP_DEVICE_TYPE_SCALE:
        {
            objType = PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_MASS_BODY_ACTUAL;
            if (PHDC_MGR_OBJECT_LIST_FIND_FROM_OBJ_TYPE(pDevice->pMds->configObjList,&objType) != NULL )
            {
                bSupported = TRUE;
            }
            break;
        }
        default:
        {
            bSupported = FALSE;
            break;
        }
    }

    return bSupported;
}


OID_Type CsrBtPhdcMgrIsData(APDU *pApdu )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
    OID_Type ret = FALSE;
    
    if ( pApdu->choice != PRST_CHOSEN)
    {
        return FALSE;
    }

    if (pData->choice.choice != ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN && pData->choice.choice != ROIV_CMIP_EVENT_REPORT_CHOSEN)
    {
        return FALSE;
    }

    if ((pRoiv->event_type >= MDC_NOTI_SCAN_REPORT_FIXED )&&
        (pRoiv->event_type <= MDC_NOTI_SCAN_REPORT_MP_VAR) )
    {
        ret = pRoiv->event_type;
    }

    return ret;
}

CsrBool CsrBtPhdcMgrIsConfirmed(APDU *pApdu)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    CsrBool ret = FALSE;

    if (pData->choice.choice == ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
    {
        ret = TRUE;
    }
    return ret;
}

void CsrBtPhdcMgrHandleObsScanFixed(CsrSchedQid qId, PhdcMgrDeviceListEntry *pDevice, APDU *pApdu )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
    CsrUint16 c,i,person;
    ObservationScanFixed *pScan;

    if(pRoiv->event_type  == MDC_NOTI_SCAN_REPORT_FIXED)
    {
        ScanReportInfoFixed     *pReport = (ScanReportInfoFixed *)pRoiv->event_info.value;
        ObservationScanFixedList *pScanFixedList = (ObservationScanFixedList *)&pReport->obs_scan_fixed;            
        pScan   = (ObservationScanFixed *)pScanFixedList->value;
        c       = pScanFixedList->count;
        person  = CSR_BT_PHDC_MGR_PERSON_ID_NA;

        for ( i = 0 ; i < c ; i++, pScan++ )
        {            
            DecodeObsevationScanFixed(qId,
                                      pDevice,
                                      pRoiv->event_type, 
                                      person,
                                      pScan->obj_handle, 
                                      pScan);
        }
    }
    else
    {
        ScanReportInfoMPFixed *pScanFixedMPList = (ScanReportInfoMPFixed *)pRoiv->event_info.value;
        ScanReportPerFixed *pScanPerFixed = pScanFixedMPList->scan_per_fixed.value;
        CsrUint8 j;
        
        for ( j = 0 ; j < pScanFixedMPList->scan_per_fixed.count ; j++, pScanPerFixed++ )
        {
            pScan   = (ObservationScanFixed *)pScanPerFixed->obs_scan_fix.value;
            c       = pScanPerFixed->obs_scan_fix.count;
            person  = pScanPerFixed->person_id;

            for ( i = 0 ; i < c ; i++, pScan++ )
            {            
                DecodeObsevationScanFixed(qId,
                                          pDevice,
                                          pRoiv->event_type, 
                                          person,
                                          pScan->obj_handle, 
                                          pScan);
            }
        }
    }
       
}


CsrBool CsrBtPhdcMgrHandleObsScanVar(CsrSchedQid qId,PhdcMgrDeviceListEntry *pDevice,APDU *pApdu )
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
    CsrBool obsScanSent = FALSE;
    ObservationScan *pScan;
    CsrUint16 c,i,person;

    if (pRoiv->event_type == MDC_NOTI_SCAN_REPORT_VAR)
    {
        ScanReportInfoVar *pScanReportInfoVar = (ScanReportInfoVar *)pRoiv->event_info.value;
        ObservationScanList *pScanVarList = (ObservationScanList *)&pScanReportInfoVar->obs_scan_var;
        pScan = (ObservationScan *)pScanVarList->value;
        c = pScanVarList->count;
        person = CSR_BT_PHDC_MGR_PERSON_ID_NA;
    }
    else 
    {
        ScanReportInfoMPVar *pScanVarMPList = (ScanReportInfoMPVar *)pRoiv->event_info.value;
        pScan = (ObservationScan *)pScanVarMPList->scan_per_var.value->obs_scan_var.value;
        c = pScanVarMPList->scan_per_var.value->obs_scan_var.count;
        person = pScanVarMPList->scan_per_var.value->person_id;
    }

    for ( i = 0 ; i < c ; i++,pScan++ )
    {            
        obsScanSent = DecodeObsevationScanVar(qId,
                                              pDevice,
                                              pRoiv->event_type, 
                                              person,
                                              pScan->obj_handle, 
                                              pScan);
    }

    return obsScanSent;
}

/* Free function's for PDU decode */
void CsrBtPhdcMgrFreeDecodeAarq(APDU *pApdu)
{
    DataProtoList *pDataProtoList  = &pApdu->u.aarq.data_proto_list;

    if ( pDataProtoList->value->data_proto_id == DATA_PROTO_ID_20601)
    {
        DataProto *pData;
        PhdAssociationInformation *pAssocInfo = NULL;
        
        pData = (DataProto   *)pDataProtoList->value;
        pAssocInfo = (PhdAssociationInformation *)(pData->data_proto_info.value);

        if (pAssocInfo->systemId.length > 0 )
        {
            CsrPmemFree(pAssocInfo->systemId.value);
        }
        if (pAssocInfo->optionList.length > 0 )
        {
            CsrPmemFree( pAssocInfo->optionList.value);
        }
        CsrPmemFree(pAssocInfo);
    }   
    CsrPmemFree(pDataProtoList->value);
    CsrPmemFree(pApdu);
}

void CsrBtPhdcMgrFreeDecodePrstNotifyConfig(APDU *pApdu)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
    ConfigReport *pConfig = (ConfigReport *)pRoiv->event_info.value;  
    CsrUint16 objCnt = 0;
    
    for (objCnt = 0; objCnt < pConfig->config_obj_list.count; objCnt++ )
    { 
        ConfigObject *configObj = (ConfigObject *)&(pConfig->config_obj_list.value[objCnt]);
        CsrUint8 attribCnt = 0;
        AttributeList  *attList = (AttributeList *)&(configObj->attributes);
        
        for (attribCnt = 0; attribCnt < attList->count; attribCnt++)
        {
            AVA_Type *attrib_value = (AVA_Type *)&attList->value[attribCnt];
            CsrPmemFree(attrib_value->attribute_value.value);
        }
        CsrPmemFree(attList->value);
        attList->value = NULL;        
    }
    CsrPmemFree(pConfig->config_obj_list.value);
    CsrPmemFree(pConfig);
    CsrPmemFree(pData);
    CsrPmemFree(pApdu);
}


void CsrBtPhdcMgrFreeDecodePrstGetMdsRsp(APDU *pApdu)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    GetResultSimple *pRorsGet = (GetResultSimple*)&pData->choice.u.rors_cmipGet;
    AVA_Type    *pAttribute;
    CsrUint16 i,n;

    n = pRorsGet->attribute_list.count;
    pAttribute = &pRorsGet->attribute_list.value[0];

    for (i= 0; i < n; i++, pAttribute++)
    {
        CsrPmemFree(pAttribute->attribute_value.value);
    }
    if (pRorsGet->attribute_list.length > 0 )
    {
        CsrPmemFree(pRorsGet->attribute_list.value);
    }
    CsrPmemFree(pData);
    CsrPmemFree(pApdu);
}


void CsrBtPhdcMgrFreeRegCertDataList(RegCertDataList  *pRegCertDataList)
{
    RegCertData   *pRegCertData = pRegCertDataList->value;
    CsrUint16 i,n;

    n = pRegCertDataList->count;

    for (i = 0 ; i < n; i++,pRegCertData++)
    {
        if (pRegCertData->auth_body_and_struc_type.auth_body_struc_type == CONTINUA_VERSION_STRUCT)
        {
            ContinuaBodyStruct *pBodyStruct = (ContinuaBodyStruct *) pRegCertData->auth_body_data.value;
            if (pBodyStruct != NULL)
            {
                CsrPmemFree(pBodyStruct->certified_devices.value);
            }
        }        

        CsrPmemFree(pRegCertData->auth_body_data.value);
    }    
    CsrPmemFree(pRegCertDataList->value);
    CsrPmemFree(pRegCertDataList);
}

void CsrBtPhdcMgrFreeDecodePrstObsScanFixed(APDU *pApdu)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport; 
    CsrUint16 i,c;

    if ((pRoiv->event_type == MDC_NOTI_SCAN_REPORT_FIXED ))
    {
        ScanReportInfoFixed     *pReport = (ScanReportInfoFixed *)pRoiv->event_info.value;
        ObservationScanFixedList *pScanFixedList = (ObservationScanFixedList *)&pReport->obs_scan_fixed;
        ObservationScanFixed *pScan = (ObservationScanFixed *)pScanFixedList->value;
        
        c =pScanFixedList->count;

        for ( i = 0 ; i < c ; i++,pScan++ )
        {
            CsrPmemFree(pScan->obs_val_data.value);
        }
        CsrPmemFree(pScanFixedList->value);
        CsrPmemFree(pReport);
    }
    else if ((pRoiv->event_type == MDC_NOTI_SCAN_REPORT_MP_FIXED))
    {
        ScanReportInfoMPFixed *pScanFixedMPList = (ScanReportInfoMPFixed *)pRoiv->event_info.value;
        ScanReportPerFixed *pScanPerFixed = pScanFixedMPList->scan_per_fixed.value;
        ObservationScanFixed *pScanMP; 
        CsrUint8 j;

        for (j=0; j < pScanFixedMPList->scan_per_fixed.count; j++)
        {
            pScanMP = pScanPerFixed->obs_scan_fix.value;
            c       = pScanPerFixed->obs_scan_fix.count;

            for ( i = 0 ; i < c ; i++, pScanMP++)
            {
                CsrPmemFree(pScanMP->obs_val_data.value);
                                
            }
            CsrPmemFree(pScanPerFixed->obs_scan_fix.value);
            pScanPerFixed++;
        }
        CsrPmemFree(pScanFixedMPList->scan_per_fixed.value);
        CsrPmemFree(pScanFixedMPList);
    }
    CsrPmemFree(pData);
    CsrPmemFree(pApdu);
}


void CsrBtPhdcMgrFreeDecodePrstObsScanVar(APDU *pApdu)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport; 
    AVA_Type    *pAttribute;
    CsrUint16 i,c,j,n;

    if ((pRoiv->event_type == MDC_NOTI_SCAN_REPORT_VAR))
    {
        ScanReportInfoVar *pScanReportInfoVar = (ScanReportInfoVar *)pRoiv->event_info.value;
        ObservationScanList *pScanVarList = (ObservationScanList *)&pScanReportInfoVar->obs_scan_var;
        ObservationScan *pScan = (ObservationScan *)pScanVarList->value;

        c = pScanVarList->count;
        for ( i = 0 ; i < c ; i++,pScan++ )
        {   
            n = pScan->attributes.count;
            pAttribute = pScan->attributes.value;
            for (j = 0; j < n; j++, pAttribute++)
            {
                CsrPmemFree(pAttribute->attribute_value.value);
            }
            CsrPmemFree(pScan->attributes.value);
        }
        CsrPmemFree(pScanVarList->value);
        CsrPmemFree(pScanReportInfoVar);
    }
    else if ((pRoiv->event_type == MDC_NOTI_SCAN_REPORT_MP_VAR))
    {
        ScanReportInfoMPVar *pScanVarMPList = (ScanReportInfoMPVar *)pRoiv->event_info.value;
        ObservationScan *pScanMP = (ObservationScan *)pScanVarMPList->scan_per_var.value->obs_scan_var.value;

        c =pScanVarMPList->scan_per_var.value->obs_scan_var.count;

        for ( i = 0 ; i < c ; i++,pScanMP++ )
        {
            n = pScanMP->attributes.count;
            pAttribute = pScanMP->attributes.value;
            for (j = 0; j < n; j++, pAttribute++)
            {
                CsrPmemFree(pAttribute->attribute_value.value);
            }
            CsrPmemFree(pScanMP->attributes.value);      
        }
        CsrPmemFree(pScanVarMPList->scan_per_var.value->obs_scan_var.value);
        CsrPmemFree(pScanVarMPList->scan_per_var.value);
        CsrPmemFree(pScanVarMPList);
    }

    CsrPmemFree(pData);
    CsrPmemFree(pApdu);
}

/*  If the function return's TRUE, all allocated memory in 
    pApdu is released 

    If it return's FALSE, call the above specific free functions 
    depending on choice */
    
CsrBool CsrBtPhdcMgrFreeDecodeAnyApdu(APDU *pApdu,CsrUint16 *pChoice)
{
    CsrBool bMemFree = FALSE;

    *pChoice = pApdu->choice;
    
    switch(pApdu->choice)
    {
        case AARQ_CHOSEN:
        {
            CsrBtPhdcMgrFreeDecodeAarq(pApdu);
            break;
        }
        case PRST_CHOSEN:            
        {
            CsrBtPhdcMderFreeDecodePrst(pApdu);
            break;
        }
        case AARE_CHOSEN:
        {
            DataProto *pData = (DataProto *)&(((AARE_apdu *)(&pApdu->u.aare))->selected_data_proto);
            if (pData->data_proto_info.value != NULL)
            {
                CsrBtPhdcMgrFreeDataProtoContents(pData);
            }
            bMemFree = TRUE;
            CsrPmemFree(pApdu);
            break;
        }
        case RLRQ_CHOSEN:
        case RLRE_CHOSEN:
        case ABRT_CHOSEN:
        {
            bMemFree = TRUE;
            CsrPmemFree(pApdu);
            break;
        }
        default:
        {
            break;   
        }
    }

    return bMemFree;
}

void CsrBtPhdcMderFreeDecodePrst(APDU *pApdu)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;

    switch(pData->choice.choice)
    {
        case ROIV_CMIP_GET_CHOSEN:
        {
            GetArgumentSimple *pGetArgument =  &pData->choice.u.roiv_cmipGet;
            if ( pGetArgument->attribute_id_list.length > 0 )
            {
                CsrPmemFree(pGetArgument->attribute_id_list.value);
            }
            break;
        }
        case ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
        {
            CsrUint16 eventType = pData->choice.choice;
           
            if ( eventType == MDC_NOTI_SCAN_REPORT_FIXED ||
                 eventType == MDC_NOTI_SCAN_REPORT_MP_FIXED )
            {
                CsrBtPhdcMgrFreeDecodePrstObsScanFixed(pApdu);
                pData = NULL;
                pApdu = NULL;
            }
            else if ( eventType == MDC_NOTI_SCAN_REPORT_VAR ||
                   eventType == MDC_NOTI_SCAN_REPORT_MP_VAR )
            {
                CsrBtPhdcMgrFreeDecodePrstObsScanVar(pApdu);
                pData = NULL;
                pApdu = NULL;
            }
            break;
        }            
        case ROER_CHOSEN:
        {
            ErrorResult *pErrorResult = &pData->choice.u.roer;

            if (pErrorResult->parameter.length > 0 )
            {
                CsrPmemFree(pErrorResult->parameter.value);
            }            
            break;
        }
        case  RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
        {
            CsrUint16 eventType = pData->choice.choice;
         
            if (eventType == MDC_NOTI_CONFIG)
            {
                CsrBtPhdcMgrFreeDecodePrstNotifyConfig(pApdu);
                pData = NULL;
                pApdu = NULL;
            }
            break;
        }            
        case RORS_CMIP_GET_CHOSEN:
        {
            CsrBtPhdcMgrFreeDecodePrstGetMdsRsp(pApdu);
            pData = NULL;
            pApdu = NULL;
            break;
        }
        case ROIV_CMIP_EVENT_REPORT_CHOSEN:
        case ROIV_CMIP_SET_CHOSEN:
        case ROIV_CMIP_CONFIRMED_SET_CHOSEN:
        case ROIV_CMIP_ACTION_CHOSEN:
        case ROIV_CMIP_CONFIRMED_ACTION_CHOSEN:      
        case RORS_CMIP_CONFIRMED_SET_CHOSEN:
        case RORS_CMIP_CONFIRMED_ACTION_CHOSEN:
        case RORJ_CHOSEN:
        default:
        {           
            break;
        }            
    }

    CsrPmemFree(pData);
    CsrPmemFree(pApdu);
}

/* */
#endif
