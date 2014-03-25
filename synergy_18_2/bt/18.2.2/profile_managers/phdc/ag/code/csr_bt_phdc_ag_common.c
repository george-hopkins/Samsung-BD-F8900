/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE

#include "csr_bt_phdc_ag_common.h"
#include "csr_bt_phdc_ag_fsm.h"
#include "csr_bt_phdc_ieee_endec.h"
#include "csr_bt_hdp_lib.h"

#define CSR_PHDC_AG_NUM_MDS_ATTRIBUTES 7
#define CSR_PHDC_AGENT_BPM_REG_CERT_DATA_LEN 0x16

#define CSR_PHDC_AGENT_MAX_DEV_SPECIALIZATION_SUPPORTED 0x02



CsrUint16 CsrBtPhdcAgentGetDeviceSpecialization(CsrUint8 index)
{
    CsrUint16 deviceSpecialization;
    
    switch(index)
    {
        case 0: /* Weighing Scale */
            deviceSpecialization = MDC_DEV_SPEC_PROFILE_SCALE;
        break;

        case 1: /* BPM */
            deviceSpecialization = MDC_DEV_SPEC_PROFILE_BP;
        break;

        default:
            deviceSpecialization = 0;
        break;        
    }

    return deviceSpecialization;
}

void CsrBtPhdcAgentFillTypeVerList(CsrUint8 supDevSpecialization,TypeVerList *pTypeVerList)
{
    CsrUint8 tempCount;
    CsrUint16 deviceSpecialization;

    pTypeVerList->count = 0;

    for(tempCount=0;tempCount<CSR_PHDC_AGENT_MAX_DEV_SPECIALIZATION_SUPPORTED;tempCount++)
    {
        if(((supDevSpecialization >> tempCount) & (0x01)) == (0x01))
        {
            pTypeVerList->count++;
            deviceSpecialization = CsrBtPhdcAgentGetDeviceSpecialization(tempCount);
            pTypeVerList->value = (TypeVer *)CsrPmemAlloc(sizeof(TypeVer));
            pTypeVerList->value->type = deviceSpecialization;
            pTypeVerList->value->version = 0x01;
        }
    }

    pTypeVerList->length = pTypeVerList->count * 4;

}

#define PHDC_AG_NO_OF_BYTES_FOR_LEN 2


static CsrUint16 CsrBtPhdcencodeInt16(const CsrUint16 val, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    /* Write to memory only if the encode routine is not called for encode 
        string length calculation */
    if(calEncStrLen == FALSE)
    {    
        msg_ptr[0] = (CsrUint8)((val & 0xff00)>>8);
        msg_ptr[1] = (val & 0x00ff);
    }

    return 2;
}

static CsrUint16 CsrBtPhdcencodeTypeVer(const TypeVer *const type_ver, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *type_ver_start = msg_ptr;

    

    msg_ptr += CsrBtPhdcencodeInt16(type_ver->type, msg_ptr,calEncStrLen);
    msg_ptr += CsrBtPhdcencodeInt16(type_ver->version, msg_ptr,calEncStrLen);   
      
    return (CsrUint16)(msg_ptr - type_ver_start);
}


static CsrUint16 CsrBtPhdcencodeTypeVerList(const TypeVerList *const type_ver_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *type_ver_list_start = msg_ptr;
    CsrUint8 *type_ver_list_len_ptr;

  

    msg_ptr += CsrBtPhdcencodeInt16(type_ver_list->count, msg_ptr,calEncStrLen);

    type_ver_list_len_ptr = msg_ptr;
    msg_ptr += PHDC_AG_NO_OF_BYTES_FOR_LEN;

    for(i = 0; i < type_ver_list->count; i++)
    {
        msg_ptr += CsrBtPhdcencodeTypeVer((type_ver_list->value + i), msg_ptr,calEncStrLen);
    }   

    CsrBtPhdcencodeInt16((CsrUint16)(msg_ptr - type_ver_list_len_ptr - PHDC_AG_NO_OF_BYTES_FOR_LEN), type_ver_list_len_ptr,calEncStrLen);
    return (CsrUint16)(msg_ptr - type_ver_list_start);
}

static CsrUint16 encodeByteString(const CsrUint16 length, const CsrUint8 *str, CsrUint8 *msg_ptr)
{
    CsrUint16 i;

    for(i = 0; i < length; i++)
    {
        msg_ptr[i] = str[i];      
    }
    return (length);
}



static void CsrBtPhdcencodeSystemModel(const SystemModel *const sysModel, CsrUint8 *msg_ptr)
{  
    msg_ptr += CsrBtPhdcencodeInt16(sysModel->manufacturer.length, msg_ptr,FALSE);
    msg_ptr += encodeByteString(sysModel->manufacturer.length, (CsrUint8 *)sysModel->manufacturer.value, msg_ptr);
    msg_ptr += CsrBtPhdcencodeInt16(sysModel->model_number.length, msg_ptr,FALSE);    
    msg_ptr += encodeByteString(sysModel->model_number.length, (CsrUint8 *)sysModel->model_number.value, msg_ptr);
}


void CsrBtPhdcAgentFreeMdsConfiguration(CsrBtPhdcAgentInstance *pInstance)
{
    if (pInstance->pGetResultSimple != NULL)
    {
        CsrUint8 tempCount=0;
        GetResultSimple *pMdsObjectAttributes;
        pMdsObjectAttributes = &pInstance->pGetResultSimple->choice.u.rors_cmipGet;
        for(tempCount=0;tempCount<pMdsObjectAttributes->attribute_list.count;tempCount++)
        {
            if(tempCount == 2)
            {
                octet_string *pSystemId;
                pSystemId = (octet_string *)pMdsObjectAttributes->attribute_list.value[2].attribute_value.value;
                CsrPmemFree(pSystemId->value);

            }
            if(tempCount == 4)
            {
                ProductionSpec *pProductionSpec;
                pProductionSpec = (ProductionSpec *)pMdsObjectAttributes->attribute_list.value[4].attribute_value.value;

                CsrPmemFree(pProductionSpec->value->prod_spec.value);
                CsrPmemFree(pProductionSpec->value);

            }
            CsrPmemFree(pMdsObjectAttributes->attribute_list.value[tempCount].attribute_value.value);
        }
        CsrPmemFree(pMdsObjectAttributes->attribute_list.value);
        CsrPmemFree(pInstance->pGetResultSimple);
        pInstance->pGetResultSimple = NULL;
    }
    CsrPmemFree(pInstance->systemId.value);
    pInstance->systemId.value = NULL;
}

void CsrBtPhdcAgentStoreMdsConfiguration(CsrBtPhdcAgConfigureMdsReq *pCsrBtPhdcConfigureMdsReq,
                                         CsrBtPhdcAgentInstance *pAgentInstance)
{
    GetResultSimple *pMdsObjectAttributes;
    CsrUint16 encodedMsgLen;
    CsrUint8 *msgPtr = NULL;

    TypeVerList *pTypeVerList;
    SystemModel *pSystemModel;
    octet_string *pSystemId;
    ProductionSpec *pProductionSpec;
    AbsoluteTime *pAbsoluteTime;

    AVA_Type *tmpValue;

    CsrUint8 regCertData[] = {  0x00,0x02,
                                0x00,0x12,
                                0x02,0x02,0x00,0x02,0x00,0x00,
                                0x02,0x01,0x00,0x08,0x01,0x00,
                                0x00,0x01,0x00,0x02,0x00,0x0f  };

    pAgentInstance->devConfigId = pCsrBtPhdcConfigureMdsReq->devConfigId;

    pAgentInstance->systemId.length = sizeof(pCsrBtPhdcConfigureMdsReq->systemId);
    pAgentInstance->systemId.value = CsrPmemZalloc(pAgentInstance->systemId.length);
    CsrMemCpy(pAgentInstance->systemId.value,pCsrBtPhdcConfigureMdsReq->systemId,pAgentInstance->systemId.length);

    pAgentInstance->pGetResultSimple = (DATA_apdu *)CsrPmemZalloc(sizeof(DATA_apdu)); 
    pAgentInstance->pGetResultSimple->choice.choice = RORS_CMIP_GET_CHOSEN;
    pAgentInstance->pGetResultSimple->choice.length = sizeof(GetResultSimple);

    pMdsObjectAttributes = &pAgentInstance->pGetResultSimple->choice.u.rors_cmipGet;
    pMdsObjectAttributes->obj_handle = 0;
    pMdsObjectAttributes->attribute_list.count = CSR_PHDC_AG_NUM_MDS_ATTRIBUTES;    
    pMdsObjectAttributes->attribute_list.length  = sizeof(AVA_Type) * CSR_PHDC_AG_NUM_MDS_ATTRIBUTES;

    tmpValue = (AVA_Type *)CsrPmemZalloc(pMdsObjectAttributes->attribute_list.length);
    pMdsObjectAttributes->attribute_list.value = tmpValue;

    pTypeVerList = (TypeVerList *) CsrPmemZalloc(sizeof(TypeVerList));
    CsrBtPhdcAgentFillTypeVerList(pCsrBtPhdcConfigureMdsReq->supportedDeviceSpecializations,pTypeVerList);
    encodedMsgLen = CsrBtPhdcencodeTypeVerList(pTypeVerList,msgPtr,TRUE);
    
    tmpValue[0].attribute_id = MDC_ATTR_SYS_TYPE_SPEC_LIST;
    tmpValue[0].attribute_value.length = encodedMsgLen;
    tmpValue[0].attribute_value.value = CsrPmemZalloc(encodedMsgLen);
    
    encodedMsgLen = CsrBtPhdcencodeTypeVerList(pTypeVerList,tmpValue[0].attribute_value.value,FALSE);

    CsrPmemFree(pTypeVerList->value);
    CsrPmemFree(pTypeVerList);

    tmpValue[1].attribute_id = MDC_ATTR_ID_MODEL; 
    
    pSystemModel = (SystemModel *) CsrPmemZalloc(sizeof(SystemModel));
    pSystemModel->manufacturer.length = (CsrUint16)CsrStrLen(pCsrBtPhdcConfigureMdsReq->manufacturer);
    pSystemModel->manufacturer.value = (CsrUint8 *)pCsrBtPhdcConfigureMdsReq->manufacturer;
    pSystemModel->model_number.length = (CsrUint16)CsrStrLen(pCsrBtPhdcConfigureMdsReq->modelNumber);
    pSystemModel->model_number.value = (CsrUint8 *)pCsrBtPhdcConfigureMdsReq->modelNumber;
    
    tmpValue[1].attribute_value.length  = pSystemModel->model_number.length + 
                                          pSystemModel->manufacturer.length + 
                                          sizeof(pSystemModel->model_number.length) + 
                                          sizeof(pSystemModel->manufacturer.length);
    tmpValue[1].attribute_value.value = CsrPmemZalloc(tmpValue[1].attribute_value.length); 
    CsrBtPhdcencodeSystemModel(pSystemModel, tmpValue[1].attribute_value.value );

    CsrPmemFree(pSystemModel);

    tmpValue[2].attribute_id = MDC_ATTR_SYS_ID;
    tmpValue[2].attribute_value.length = sizeof(octet_string);
    tmpValue[2].attribute_value.value = CsrPmemZalloc(tmpValue[2].attribute_value.length);

    pSystemId = (octet_string *)tmpValue[2].attribute_value.value;
    pSystemId->length = sizeof(pCsrBtPhdcConfigureMdsReq->systemId); 
    pSystemId->value = CsrPmemZalloc(pSystemId->length);    
    CsrMemCpy(pSystemId->value,pCsrBtPhdcConfigureMdsReq->systemId,pSystemId->length);
    
    tmpValue[3].attribute_id = MDC_ATTR_DEV_CONFIG_ID;
    tmpValue[3].attribute_value.length = sizeof(pCsrBtPhdcConfigureMdsReq->devConfigId);
    tmpValue[3].attribute_value.value = CsrPmemZalloc(tmpValue[3].attribute_value.length );
    tmpValue[3].attribute_value.value[0] = (CsrUint8)((pCsrBtPhdcConfigureMdsReq->devConfigId & 0xFF00)>>0x08);
    tmpValue[3].attribute_value.value[1] = (CsrUint8)(pCsrBtPhdcConfigureMdsReq->devConfigId & 0x00FF);

    tmpValue[4].attribute_id = MDC_ATTR_ID_PROD_SPECN;
    tmpValue[4].attribute_value.length = sizeof(ProductionSpec);
    tmpValue[4].attribute_value.value = CsrPmemZalloc(tmpValue[4].attribute_value.length);
    
    pProductionSpec = (ProductionSpec *)tmpValue[4].attribute_value.value;
    pProductionSpec->count = 1;
    pProductionSpec->length = sizeof(ProdSpecEntry) * pProductionSpec->count;
    pProductionSpec->value = (ProdSpecEntry *)CsrPmemZalloc(pProductionSpec->length);
    pProductionSpec->value->spec_type = 1;
    pProductionSpec->value->component_id = 0;
    pProductionSpec->value->prod_spec.length = (CsrUint16)CsrStrLen(pCsrBtPhdcConfigureMdsReq->serialNumber);
    pProductionSpec->value->prod_spec.value = CsrPmemZalloc(pProductionSpec->value->prod_spec.length);
    CsrMemCpy(pProductionSpec->value->prod_spec.value,pCsrBtPhdcConfigureMdsReq->serialNumber,pProductionSpec->value->prod_spec.length);
    
    tmpValue[5].attribute_id = MDC_ATTR_TIME_ABS;    
    tmpValue[5].attribute_value.length = sizeof(AbsoluteTime);
    tmpValue[5].attribute_value.value = CsrPmemZalloc(tmpValue[5].attribute_value.length);
    pAbsoluteTime = (AbsoluteTime *)tmpValue[5].attribute_value.value;
    CsrMemCpy(pAbsoluteTime,(AbsoluteTime *)(&pCsrBtPhdcConfigureMdsReq->absolutetime),sizeof(AbsoluteTime));
    
    if(pCsrBtPhdcConfigureMdsReq->supportedDeviceSpecializations == 0x01)
    {
        regCertData[21] = 0x0f;
    }
    else if(pCsrBtPhdcConfigureMdsReq->supportedDeviceSpecializations == 0x02)
    {
        regCertData[21] = 0x07;
    }
    tmpValue[6].attribute_id = MDC_ATTR_REG_CERT_DATA_LIST;     
    tmpValue[6].attribute_value.length = sizeof(regCertData);
    tmpValue[6].attribute_value.value = CsrPmemZalloc(tmpValue[6].attribute_value.length); 
    CsrMemCpy(tmpValue[6].attribute_value.value, regCertData,tmpValue[6].attribute_value.length);
}


void CsrBtPhdcAgentFormConfigObject(ConfigObject *pConfigObject,CsrBtPhdcAgentObjectInfo *pObjectList)
{
    CsrUint8 tempAttribCount=0;
    AVA_Type *pAvaType;

    pConfigObject->obj_class = pObjectList->objClass;
    pConfigObject->obj_handle = pObjectList->objHandle;
    pConfigObject->attributes.count = pObjectList->numAttrib;
    pConfigObject->attributes.value = (AVA_Type *)CsrPmemZalloc(sizeof(AVA_Type) * pObjectList->numAttrib);

    for(tempAttribCount=0;tempAttribCount<pObjectList->numAttrib;tempAttribCount++)
    {
        pAvaType = &pConfigObject->attributes.value[tempAttribCount];

        pAvaType->attribute_id = pObjectList->pAttribList[tempAttribCount].attribId;
        pAvaType->attribute_value.length = pObjectList->pAttribList[tempAttribCount].attribInfoLength;
        pAvaType->attribute_value.value = CsrPmemZalloc(pAvaType->attribute_value.length);
        CsrMemCpy(pAvaType->attribute_value.value,
                  pObjectList->pAttribList[tempAttribCount].attribInfo,
                  pAvaType->attribute_value.length);
    }
}

void CsrBtPhdcAgentFormConfigObjectList(CsrBtPhdcAgentInstance *pAgentInstance)
{
    CsrUint8 tempObjCount=0;
    pAgentInstance->configReport.config_obj_list.count = pAgentInstance->objCount;
    pAgentInstance->configReport.config_obj_list.value = (ConfigObject *)CsrPmemAlloc(sizeof(ConfigObject) * pAgentInstance->objCount);
    for(tempObjCount=0; tempObjCount < pAgentInstance->configReport.config_obj_list.count; tempObjCount++)
    {
        CsrBtPhdcAgentFormConfigObject(&pAgentInstance->configReport.config_obj_list.value[tempObjCount],
                &pAgentInstance->pObjectList[tempObjCount]);
    }
}


void CsrBtPhdcFormEventReportArgumentSimpleNotiConfig(CsrBtPhdcAgentInstance *pAgentInstance)
{
    EventReportArgumentSimple *pRoivCmipConfirmedEventReport;

    pAgentInstance->pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));
    pAgentInstance->pApdu->choice = PRST_CHOSEN;

    pAgentInstance->pEventReportNotifyConfig = (DATA_apdu *)CsrPmemAlloc(sizeof(DATA_apdu));

    pAgentInstance->pEventReportNotifyConfig->invoke_id = pAgentInstance->invokeId++;
    pAgentInstance->pEventReportNotifyConfig->choice.choice = ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN;

    pRoivCmipConfirmedEventReport = &pAgentInstance->pEventReportNotifyConfig->choice.u.roiv_cmipConfirmedEventReport;

    pRoivCmipConfirmedEventReport->obj_handle = 0;
    pRoivCmipConfirmedEventReport->event_time = 0xFFFFFFFF;
    pRoivCmipConfirmedEventReport->event_type = MDC_NOTI_CONFIG;

    pAgentInstance->configReport.config_report_id = EXTENDED_CONFIG_START;
    CsrBtPhdcAgentFormConfigObjectList(pAgentInstance);
    pRoivCmipConfirmedEventReport->event_info.value = (CsrUint8 *)&pAgentInstance->configReport;

    pAgentInstance->pApdu->u.prst.value = (CsrUint8 *)pAgentInstance->pEventReportNotifyConfig;

}

void CsrBtPhdcAgFormScanReportPerFixed(ScanReportPerFixed *pScanReportPerFixed,CsrBtPhdcAgDataNode *pData)
{
    pScanReportPerFixed->person_id = pData->personId;
    pScanReportPerFixed->obs_scan_fix.count = 1;
    pScanReportPerFixed->obs_scan_fix.value = (ObservationScanFixed *)CsrPmemAlloc(sizeof(ObservationScanFixed));
    CsrBtPhdcAgFormObservationScanFixed(pScanReportPerFixed->obs_scan_fix.value,pData);
}


void CsrBtPhdcAgFormScanReportPerVar(ScanReportPerVar *pScanReportPerVar,CsrBtPhdcAgDataNode *pData,CsrBtPhdcAgentInstance *pInstance)
{
    pScanReportPerVar->person_id = pData->personId;
    pScanReportPerVar->obs_scan_var.count = 1;
    pScanReportPerVar->obs_scan_var.value = (ObservationScan *)CsrPmemAlloc(sizeof(ObservationScan));
    CsrBtPhdcAgFormObservationScan(pInstance,pData);
}


void CsrBtPhdcAgFormObservationScanFixed(ObservationScanFixed *pObservationScanFixed,CsrBtPhdcAgDataNode *pData)
{
    CsrUint16 dataLen;
    CsrUint8 tempCount=0;
    pObservationScanFixed->obj_handle = pData->objHandle;

    pObservationScanFixed->obs_val_data.length = (pData->dataLength) * 2 + sizeof(pData->time);
    pObservationScanFixed->obs_val_data.value = CsrPmemAlloc(pObservationScanFixed->obs_val_data.length);
    
    for(dataLen=0;dataLen<pData->dataLength;dataLen++)
    {        
        pObservationScanFixed->obs_val_data.value[dataLen+tempCount] = ((pData->data[dataLen] & 0xFF00) >> 8);
        tempCount++;
        pObservationScanFixed->obs_val_data.value[dataLen+tempCount] = (pData->data[dataLen] & 0x00FF) ;        
    }
    CsrMemCpy(&pObservationScanFixed->obs_val_data.value[(pData->dataLength)*2],(CsrUint8 *)&pData->time, sizeof(CsrBtPhdcAgAbsoluteTime));

}


void CsrBtPhdcAgFormRoivCmipConfirmedEventReport(EventReportArgumentSimple *pRoivCmipConfirmedEventReport,
                                    CsrBtPhdcAgentInstance *pInstance,CsrBtPhdcAgDataNode *pData)
{

    pRoivCmipConfirmedEventReport->obj_handle = CSR_BT_PHDC_AG_MDS_OBJECT_HANDLE;
    pRoivCmipConfirmedEventReport->event_time = 0xFFFFFFFF;

    if(pData->personId == CSR_BT_PHDC_AG_PERSON_ID_NA)
    {
        if(pInstance->eventReportingType == CSR_BT_PHDC_EVENT_REPORTING_TYPE_FIXED)
        {
            ScanReportInfoFixed *pScanReportInfoFixed;
            
            pRoivCmipConfirmedEventReport->event_type = MDC_NOTI_SCAN_REPORT_FIXED;
            pRoivCmipConfirmedEventReport->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoFixed));
            pInstance->pScanReportInfoFixed = (ScanReportInfoFixed *)pRoivCmipConfirmedEventReport->event_info.value;
            pScanReportInfoFixed = pInstance->pScanReportInfoFixed;
            pScanReportInfoFixed->data_req_id = DATA_REQ_ID_AGENT_INITIATED;

            pScanReportInfoFixed->obs_scan_fixed.count = 1; /* First measurement */
        }
        else
        {
            ScanReportInfoVar *pScanReportInfoVar;
            
            pRoivCmipConfirmedEventReport->event_type = MDC_NOTI_SCAN_REPORT_VAR;
            pRoivCmipConfirmedEventReport->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoVar));
            pInstance->pScanReportInfoVar = (ScanReportInfoVar *)pRoivCmipConfirmedEventReport->event_info.value;
            pScanReportInfoVar = pInstance->pScanReportInfoVar;
            pScanReportInfoVar->data_req_id = DATA_REQ_ID_AGENT_INITIATED;
            pScanReportInfoVar->obs_scan_var.count = 1; /* First measurement */
        }
    }
    else
    {
        if(pInstance->eventReportingType == CSR_BT_PHDC_EVENT_REPORTING_TYPE_FIXED)
        {
            ScanReportInfoMPFixed *pScanReportInfoMPFixed;
            
            pRoivCmipConfirmedEventReport->event_type = MDC_NOTI_SCAN_REPORT_MP_FIXED;
            pRoivCmipConfirmedEventReport->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoMPFixed));
            pInstance->pScanReportInfoMPFixed = (ScanReportInfoMPFixed *)pRoivCmipConfirmedEventReport->event_info.value;
            pScanReportInfoMPFixed = pInstance->pScanReportInfoMPFixed;
            pScanReportInfoMPFixed->data_req_id = DATA_REQ_ID_AGENT_INITIATED;
            pScanReportInfoMPFixed->scan_per_fixed.count = 1;
        }
        else
        {
            ScanReportInfoMPVar *pScanReportInfoMPVar;
            
            pRoivCmipConfirmedEventReport->event_type = MDC_NOTI_SCAN_REPORT_MP_VAR;
            pRoivCmipConfirmedEventReport->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoMPVar));
            pInstance->pScanReportInfoMPVar = (ScanReportInfoMPVar *)pRoivCmipConfirmedEventReport->event_info.value;
            pScanReportInfoMPVar = pInstance->pScanReportInfoMPVar;
            pScanReportInfoMPVar->data_req_id = DATA_REQ_ID_AGENT_INITIATED;
            pScanReportInfoMPVar->scan_per_var.count = 1;

        }
        
    }

    
}




CsrBool CsrBtPhdcAgentValidateAare(AARE_apdu *pAare)
{
    DataProto *pDataProto;
    CsrBool result=TRUE;
    PhdAssociationInformation *pPhdAssociationInformation;
    pDataProto = &pAare->selected_data_proto;

    if(pDataProto->data_proto_id != DATA_PROTO_ID_20601)
    {
        result = FALSE;
    }
    else
    {            
        pPhdAssociationInformation = (PhdAssociationInformation *)pAare->selected_data_proto.data_proto_info.value;

        if((pPhdAssociationInformation->protocolVersion != PROTOCOL_VERSION1) ||
           (pPhdAssociationInformation->encodingRules != MDER) ||
           (pPhdAssociationInformation->nomenclatureVersion != NOM_VERSION1) ||
           (pPhdAssociationInformation->systemType != SYS_TYPE_MANAGER) ||
           (pPhdAssociationInformation->dev_config_id != 0) ||
           (pPhdAssociationInformation->data_req_mode_capab.data_req_mode_flags != 0) ||
           (pPhdAssociationInformation->data_req_mode_capab.data_req_init_agent_count != 0) ||
           (pPhdAssociationInformation->data_req_mode_capab.data_req_init_manager_count != 0))
        {
            result = FALSE;
        }
    }
    return result;
}

CsrUint8 CsrBtPhdcAgGetObjectIndexdFromObjectHandle(CsrBtPhdcAgentInstance *pInstance,CsrUint16  objHandle)
{
    CsrUint8 objIdx=0;
    for(objIdx=0;objIdx<pInstance->objCount;objIdx++)
    {
        if(pInstance->pObjectList[objIdx].objHandle == objHandle)
        {
            break;
        }
    }

    return objIdx;
}

void CsrBtPhdcAgFormObservationScan(CsrBtPhdcAgentInstance *pInstance,CsrBtPhdcAgDataNode *dataNode)
{
    CsrUint8 objIdx=0,count=0;
    CsrBtPhdcAgentObjectInfo *pAgentObjectInfo;
    ObservationScan *pObservationScan;
    AVA_Type *pAvaType;

    objIdx = CsrBtPhdcAgGetObjectIndexdFromObjectHandle(pInstance,dataNode->objHandle);
    pAgentObjectInfo = &pInstance->pObjectList[objIdx];

    pObservationScan = pInstance->pScanReportInfoVar->obs_scan_var.value;
    pObservationScan->obj_handle = dataNode->objHandle;
    pObservationScan->attributes.count = pAgentObjectInfo->dataValueLength;
    pObservationScan->attributes.value = (AVA_Type *)CsrPmemAlloc(sizeof(AVA_Type) * pAgentObjectInfo->dataValueLength);

    for(count=0;count<pAgentObjectInfo->dataValueLength;count++)
    {
        pAvaType = &pObservationScan->attributes.value[count];
        if(pAgentObjectInfo->dataValue[count] == CSR_BT_PHDC_AG_ATTR_DATA_FORMAT_NU_VAL_OBS_SIMP)
        {
            pAvaType->attribute_id = MDC_ATTR_NU_VAL_OBS_SIMP;
            pAvaType->attribute_value.length = 4;
            pAvaType->attribute_value.value = CsrPmemAlloc(4);
            pAvaType->attribute_value.value[0] = (CsrUint8)((dataNode->data[0]& 0xFF00) >> 8); 
            pAvaType->attribute_value.value[1] = (CsrUint8)(dataNode->data[0]& 0x00FF); 
            pAvaType->attribute_value.value[2] = (CsrUint8)((dataNode->data[1]& 0xFF00) >> 8); 
            pAvaType->attribute_value.value[3] = (CsrUint8)(dataNode->data[1]& 0x00FF); 
            
        }
        if(pAgentObjectInfo->dataValue[count] == CSR_BT_PHDC_AG_ATTR_DATA_FORMAT_ABSOLUTE_TIME)        
        {
            pAvaType->attribute_id = MDC_ATTR_TIME_STAMP_ABS;
            pAvaType->attribute_value.length = 8;
            pAvaType->attribute_value.value = CsrPmemAlloc(sizeof(CsrBtPhdcAgAbsoluteTime));            
            CsrMemCpy(pAvaType->attribute_value.value,&dataNode->time,sizeof(CsrBtPhdcAgAbsoluteTime));
        }
        
        if( (pAgentObjectInfo->dataValue[count] & CSR_BT_PHDC_AG_ATTR_DATA_FORMAT_NU_CMPD_VAL_OBS_BASIC)
            == CSR_BT_PHDC_AG_ATTR_DATA_FORMAT_NU_CMPD_VAL_OBS_BASIC)
        {
            CsrUint16 tempCount=0,idx;
            BasicNuObsValueCmp *pBasicNuObsValueCmp;
            tempCount = pAgentObjectInfo->dataValue[count] & 0x00FF;
            pAvaType->attribute_id = MDC_ATTR_NU_CMPD_VAL_OBS_BASIC;
            pAvaType->attribute_value.length = 4/* 2 bytes for length + 2 bytes for count*/ + tempCount * 2;

            pAvaType->attribute_value.value = CsrPmemAlloc(sizeof(BasicNuObsValueCmp) + sizeof(BasicNuObsValue) * tempCount);

            pBasicNuObsValueCmp = (BasicNuObsValueCmp *)pAvaType->attribute_value.value;
            pBasicNuObsValueCmp->count = tempCount;
            pBasicNuObsValueCmp->length = pBasicNuObsValueCmp->count * sizeof(BasicNuObsValue);
            for(idx=0;idx<tempCount;idx++)
            {
                pBasicNuObsValueCmp->value[idx] = dataNode->data[idx+2];
            }
        }
        
        if(pAgentObjectInfo->dataValue[count] == CSR_BT_PHDC_AG_ATTR_DATA_FORMAT_NU_VAL_OBS_BASIC)        
        {
            pAvaType->attribute_id = MDC_ATTR_NU_VAL_OBS_BASIC;
            pAvaType->attribute_value.length = 2;
            pAvaType->attribute_value.value = CsrPmemAlloc(2);
            pAvaType->attribute_value.value[0] = (CsrUint8)((dataNode->data[0]& 0xFF00) >> 8); 
            pAvaType->attribute_value.value[1] = (CsrUint8)(dataNode->data[0]& 0x00FF); 
            
        }        
    }
}

CsrBool CsrBtPhdcAgIsValidObjectHandle(CsrUint16 objHandle,CsrBtPhdcAgentInstance *pInstance)
{
    CsrUint8 tempCount;
    CsrBool res = FALSE;

    for(tempCount=0;tempCount<pInstance->objCount;tempCount++)
    {
        if(objHandle == pInstance->objHandles[tempCount])
        {
            res = TRUE;
            break;
        }
    }

    return res;
}


void CsrBtPhdcAgFreeObjectInfo(CsrBtPhdcAgentInstance *pInstance)
{
    CsrUint16 objCount,attribCount;

    if((pInstance->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_WS) ||
        (pInstance->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_BP))
        return;

    for(objCount=0;objCount<pInstance->objCount;objCount++)
    {
        CsrBtPhdcAgentObjectInfo *tmpObj = &(pInstance->pObjectList[objCount]);
        CsrPmemFree(tmpObj->dataValue);
        for(attribCount=0; (attribCount < tmpObj->numAttrib); attribCount++)
        {
            CsrBtPhdcAgentAttribInfo *tmp = &(tmpObj->pAttribList[attribCount]);

            CsrPmemFree(tmp->attribInfo);
        }
        CsrPmemFree(tmpObj->pAttribList);
    }
    CsrPmemFree(pInstance->pObjectList);
    pInstance->pObjectList = NULL;
    pInstance->objCount = 0;
    pInstance->objIndex = 0;       
}

void CsrBtPhdcAgFreeMemory(CsrBtPhdcAgentInstance *pInst)
{

    CsrPmemFree(pInst->pApdu);
    CsrPmemFree(pInst->pEventReportNotifyConfig);

    CsrPmemFree(pInst->pGetResultSimple);

    if(pInst->pObjectList != NULL)
    {
        CsrBtPhdcAgFreeObjectInfo(pInst);
    }

    CsrPmemFree(pInst->pRecvMsg);

    CsrPmemFree(pInst->pRoivCmfdEventReport);
}


void CsrBtPhdcFillAarq(AARQ_apdu *aarqApdu,CsrBtPhdcAgentInstance *pInstance)
{
    PhdAssociationInformation *pPhdAssociationInformation;
    aarqApdu->assoc_version = ASSOC_VERSION1;
    aarqApdu->data_proto_list.count = 1;
    aarqApdu->data_proto_list.value = (DataProto *)CsrPmemAlloc(sizeof(DataProto));
    aarqApdu->data_proto_list.value->data_proto_id = DATA_PROTO_ID_20601;
    aarqApdu->data_proto_list.value->data_proto_info.value = 
        CsrPmemAlloc(sizeof(PhdAssociationInformation));
    pPhdAssociationInformation = (PhdAssociationInformation *)aarqApdu->data_proto_list.value->data_proto_info.value;
    pPhdAssociationInformation->protocolVersion = PROTOCOL_VERSION1;
    pPhdAssociationInformation->encodingRules = MDER;
    pPhdAssociationInformation->nomenclatureVersion = NOM_VERSION1;
    pPhdAssociationInformation->functionalUnits = 0x00;
    pPhdAssociationInformation->systemType= SYS_TYPE_AGENT;
    pPhdAssociationInformation->systemId.length = pInstance->systemId.length;
    pPhdAssociationInformation->systemId.value = CsrPmemAlloc(pInstance->systemId.length);
    CsrMemCpy(pPhdAssociationInformation->systemId.value,pInstance->systemId.value,pInstance->systemId.length);
    pPhdAssociationInformation->dev_config_id = pInstance->devConfigId;
    pPhdAssociationInformation->data_req_mode_capab.data_req_mode_flags = DATA_REQ_SUPP_INIT_AGENT; /* supports requesting objects based on object handle */
    pPhdAssociationInformation->data_req_mode_capab.data_req_init_agent_count = 1;
    pPhdAssociationInformation->data_req_mode_capab.data_req_init_manager_count = 0;
    pPhdAssociationInformation->optionList.count = 0;
    pPhdAssociationInformation->optionList.length = 0;
}

void CsrBtPhdcStoreApdu(CsrUint8 *msgPtr,CsrUint16 encodedMsgLen,CsrBtPhdcAgentInstance *pInstance)
{
    pInstance->tempApdu.commandApduLength = encodedMsgLen;
    pInstance->tempApdu.apdu = CsrPmemAlloc(encodedMsgLen);
    CsrMemCpy(pInstance->tempApdu.apdu,msgPtr,encodedMsgLen);     
}


void CsrBtPhdc20601AarqEncodeSend(CsrBtPhdcAgentInstance *pInstance)
{
    APDU apdu;
    CsrUint16 encodedMsgLen;
    CsrUint8 *msgPtr;
    PhdAssociationInformation *pPhdAssociationInformation;
    
    apdu.choice = AARQ_CHOSEN;
    CsrBtPhdcFillAarq(&apdu.u.aarq,pInstance);

    encodedMsgLen = CsrBtPhdcMderCalculateApduEncodedLength(&apdu);
    msgPtr = CsrPmemAlloc(encodedMsgLen*sizeof(CsrUint8));
    
    CsrBtPhdcMderEncodeApdu(&apdu, msgPtr);
    CsrBtPhdcStoreApdu(msgPtr,encodedMsgLen,pInstance);

    pPhdAssociationInformation = (PhdAssociationInformation *)apdu.u.aarq.data_proto_list.value->data_proto_info.value;
    CsrPmemFree(pPhdAssociationInformation->systemId.value);
    CsrPmemFree(apdu.u.aarq.data_proto_list.value->data_proto_info.value);
    CsrPmemFree(apdu.u.aarq.data_proto_list.value);

    CsrBtHdpDataReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,pInstance->assocChannelId,msgPtr,encodedMsgLen);

    pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_TO_ASSOC, 
                                       CsrBtPhdcAgentTimedEvent, 
                                       PHDC_AG_DEVICE_EV_TIMER_TO_ASSOC_EXPIRY, 
                                       (void *) pInstance);
    pInstance->rcAssoc = 0;
    
}

#endif
