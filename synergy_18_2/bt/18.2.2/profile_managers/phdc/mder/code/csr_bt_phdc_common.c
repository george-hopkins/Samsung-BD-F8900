/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#if !defined(EXCLUDE_CSR_BT_PHDC_AG_MODULE) || !defined(EXCLUDE_CSR_BT_PHDC_MGR_MODULE)

#include "csr_bt_hdp_lib.h"
#include "csr_bt_phdc_ieee_endec.h"


void CsrBtPhdc20601AbrtEncodeSend(CsrSchedQid phandle, CsrUint32  dataChannelId,Abort_reason reason)
{
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = ABRT_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.abrt.reason = reason;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    pEncodedApdu = CsrPmemAlloc(encodedLen);
    
    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu);
}

void CsrBtPhdc20601RlrqEncodeSend(CsrSchedQid phandle,CsrUint32  dataChannelId,Release_request_reason reason)
{
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = RLRQ_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.rlrq.reason = reason;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    pEncodedApdu = CsrPmemAlloc(encodedLen);
    
    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu);
}


void CsrBtPhdc20601RlreEncodeSend(CsrSchedQid phandle,CsrUint32  dataChannelId,Release_response_reason reason)
{
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = RLRE_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.rlre.reason = reason;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    pEncodedApdu = CsrPmemAlloc(encodedLen);
    
    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu);
}

void CsrBtPhdc20601RorjEncodeSend(CsrSchedQid phandle,CsrUint32  dataChannelId,CsrUint16 invokeId,CsrUint16 rejectResult)
{
    DATA_apdu dataApdu;
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = PRST_CHOSEN;
    pApdu->length = ANY_SIZE;

    dataApdu.invoke_id = invokeId;
    dataApdu.choice.choice = RORJ_CHOSEN;
    dataApdu.choice.length = ANY_SIZE;
    dataApdu.choice.u.rorj.problem = rejectResult;

    pApdu->u.prst.length = ANY_SIZE;

    pApdu->u.prst.value = (CsrUint8 *)&dataApdu;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    pEncodedApdu = CsrPmemAlloc(encodedLen);
    
    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu);
}


void CsrBtPhdc20601RoerEncodeSend(CsrSchedQid phandle,CsrUint32  dataChannelId,CsrUint16 invokeId,CsrUint16 error)
{
    DATA_apdu dataApdu;
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = PRST_CHOSEN;
    pApdu->length = ANY_SIZE;

    dataApdu.invoke_id = invokeId;
    dataApdu.choice.choice = ROER_CHOSEN;
    dataApdu.choice.length = ANY_SIZE;
    dataApdu.choice.u.roer.error_value = error;
    dataApdu.choice.u.roer.parameter.length = 0;

    pApdu->u.prst.length = ANY_SIZE;

    pApdu->u.prst.value = (CsrUint8 *)&dataApdu;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    pEncodedApdu = CsrPmemAlloc(encodedLen);
    
    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu);
}


void CsrBtPhdcMgr20601AbrtEncodeSend(CsrSchedQid phandle,CsrUint32  dataChannelId,Abort_reason reason)
{
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = ABRT_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.abrt.reason = reason;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    pEncodedApdu = CsrPmemAlloc(encodedLen);

    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu);
}


void CsrBtPhdc20601AareEncodeSend(CsrSchedQid phandle,CsrUint32  dataChannelId,DataProto *pDataProto,Associate_result result)
{
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    PhdAssociationInformation *pAssocInfo = NULL;


    pApdu = (APDU *)CsrPmemAlloc(sizeof(CsrUint16)*2 + sizeof(AARE_apdu));

    pApdu->choice = AARE_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.aare.result = result;

    pApdu->u.aare.selected_data_proto = *pDataProto;

    if ( result == ACCEPTED ||
         result == ACCEPTED_UNKNOWN_CONFIG )
    {
        pAssocInfo = (PhdAssociationInformation *)(pDataProto->data_proto_info.value);
        pAssocInfo->systemType = SYS_TYPE_MANAGER;
        pAssocInfo->dev_config_id = 0;
        pAssocInfo->encodingRules =  MDER;
        pAssocInfo->data_req_mode_capab.data_req_mode_flags = 0;
        pAssocInfo->data_req_mode_capab.data_req_init_agent_count = 0;
        pAssocInfo->data_req_mode_capab.data_req_init_manager_count = 0;
    }

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    pEncodedApdu = CsrPmemAlloc(encodedLen);

    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu);
}



void CsrBtPhdc20601GetMdsSend(CsrSchedQid phandle,CsrUint32  dataChannelId,CsrUint16 invokeId)
{
    APDU    *pApdu = NULL;
    CsrUint16 encodedLen ;
    CsrUint8  *pEncodedApdu;
    DATA_apdu  *pDataApdu = NULL;
    GetArgumentSimple *pRoivGet = NULL;

    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = PRST_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.prst.length = ANY_SIZE;
    pApdu->u.prst.value = (CsrUint8 *)CsrPmemAlloc(sizeof(DATA_apdu));

    pDataApdu = (DATA_apdu  *)pApdu->u.prst.value;

    pDataApdu->invoke_id = invokeId; /* given it as 1 */
    pDataApdu->choice.choice = ROIV_CMIP_GET_CHOSEN;
    pDataApdu->choice.length = ANY_SIZE;

    pRoivGet = &pDataApdu->choice.u.roiv_cmipGet;

    pRoivGet->obj_handle = 0;
    pRoivGet->attribute_id_list.count = 0;
    pRoivGet->attribute_id_list.length = 0;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);

    pEncodedApdu = CsrPmemAlloc(encodedLen);

    CsrBtPhdcMderEncodeApdu(pApdu, pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu->u.prst.value);
    CsrPmemFree(pApdu);

}


void CsrBtPhdc20601ConfigRspSend(CsrSchedQid phandle,
                                    CsrUint32  dataChannelId,CsrUint16 invokeId,
                                    CsrUint16 devConfigId,ConfigResult config_result)

{
    APDU    *pApdu = NULL;
    DATA_apdu  *pDataApdu = NULL;
    EventReportResultSimple *pRors=NULL;
    ConfigReportRsp *pConfigRsp=NULL;
    CsrUint8  *pEncodedApdu = NULL;
    CsrUint16 encodedLen ;

    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = PRST_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.prst.length = ANY_SIZE;
    pApdu->u.prst.value = (CsrUint8 *)CsrPmemAlloc(sizeof(DATA_apdu));

    pDataApdu = (DATA_apdu  *)pApdu->u.prst.value;

    pDataApdu->invoke_id = invokeId;
    pDataApdu->choice.choice = RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN;
    pDataApdu->choice.length = ANY_SIZE;

    pRors = &pDataApdu->choice.u.rors_cmipConfirmedEventReport;

    pRors->obj_handle = 0;
    pRors->currentTime = 0x0000;
    pRors->event_type = MDC_NOTI_CONFIG;

    pRors->event_reply_info.length = sizeof(ConfigReportRsp );
    pConfigRsp = (ConfigReportRsp *)CsrPmemAlloc( sizeof(ConfigReportRsp)) ;
    pRors->event_reply_info.value = (CsrUint8 *)pConfigRsp;

    pConfigRsp->config_report_id = devConfigId;
    pConfigRsp->config_result = config_result;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);

    pEncodedApdu = CsrPmemAlloc(encodedLen);

    CsrBtPhdcMderEncodeApdu(pApdu,pEncodedApdu);

    CsrBtHdpDataReqSend(phandle,dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pConfigRsp);
    CsrPmemFree(pApdu->u.prst.value);
    CsrPmemFree(pApdu);

}

void CsrBtPhdc20601DataConfirmedEventSend(CsrSchedQid phandle,
                                             CsrUint32  dataChannelId,CsrUint16 invokeId,
                                             OID_Type eventType)

{
    APDU    *pApdu = NULL;
    DATA_apdu  *pDataApdu = NULL;
    EventReportResultSimple *pRors=NULL;
    CsrUint8  *pEncodedApdu = NULL;
    CsrUint16 encodedLen ;

    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));

    pApdu->choice = PRST_CHOSEN;
    pApdu->length = ANY_SIZE;

    pApdu->u.prst.length = ANY_SIZE;
    pApdu->u.prst.value = (CsrUint8 *)CsrPmemAlloc(sizeof(DATA_apdu));

    pDataApdu = (DATA_apdu  *)pApdu->u.prst.value;

    pDataApdu->invoke_id = invokeId;
    pDataApdu->choice.choice = RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN;
    pDataApdu->choice.length = ANY_SIZE;

    pRors = &pDataApdu->choice.u.rors_cmipConfirmedEventReport;

    pRors->obj_handle = 0;
    pRors->currentTime = 0x0000;
    pRors->event_type = eventType;

    pRors->event_reply_info.length = 0;

    encodedLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);

    pEncodedApdu = CsrPmemAlloc(encodedLen);

    CsrBtPhdcMderEncodeApdu(pApdu,pEncodedApdu);

    CsrBtHdpDataReqSend(phandle, dataChannelId, pEncodedApdu, encodedLen);

    CsrPmemFree(pApdu->u.prst.value);
    CsrPmemFree(pApdu);

}
 /* Free functions */
void CsrBtPhdcFreeDecodePrstObsScanFixed(APDU *pApdu)
{
    DATA_apdu *pData = (DATA_apdu *)pApdu->u.prst.value;
    EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport; 
    CsrUint16 i,c;

    if (pRoiv->event_type == MDC_NOTI_SCAN_REPORT_FIXED )
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
    else if (pRoiv->event_type == MDC_NOTI_SCAN_REPORT_MP_FIXED)
    {
        ScanReportInfoMPFixed *pScanFixedMPList = (ScanReportInfoMPFixed *)pRoiv->event_info.value;
        ObservationScanFixed *pScanMP;
        CsrUint16 j;
        
        for (j=0; j < pScanFixedMPList->scan_per_fixed.count; j++)
        {
            pScanMP = (ObservationScanFixed *)pScanFixedMPList->scan_per_fixed.value[j].obs_scan_fix.value;

            c =pScanFixedMPList->scan_per_fixed.value[j].obs_scan_fix.count;

            for ( i = 0 ; i < c ; i++,pScanMP++ )
            {
                CsrPmemFree(pScanMP->obs_val_data.value);
            }
            CsrPmemFree(pScanFixedMPList->scan_per_fixed.value[j].obs_scan_fix.value);
        }
        CsrPmemFree(pScanFixedMPList->scan_per_fixed.value);
        CsrPmemFree(pScanFixedMPList);
    }
}

void CsrBtPhdcFreeDecodePrstObsScanVar(APDU *pApdu)
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

}

void CsrBtPhdcFreeDecodePrstNotifyConfig(APDU *pApdu)
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
}

void CsrBtPhdcFreeDecodePrstGetMdsRsp(APDU *pApdu)
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
}

void CsrBtPhdcFreeDecodePrst(APDU *pApdu)
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
            EventReportArgumentSimple *pRoiv = &pData->choice.u.roiv_cmipConfirmedEventReport;
            CsrUint16 eventType = pRoiv->event_type;
           
            if ( eventType == MDC_NOTI_SCAN_REPORT_FIXED ||
                 eventType == MDC_NOTI_SCAN_REPORT_MP_FIXED )
            {
                CsrBtPhdcFreeDecodePrstObsScanFixed(pApdu);
            }
            else if ( eventType == MDC_NOTI_SCAN_REPORT_VAR ||
                   eventType == MDC_NOTI_SCAN_REPORT_MP_VAR )
            {
                CsrBtPhdcFreeDecodePrstObsScanVar(pApdu);
            }
            else if (eventType == MDC_NOTI_CONFIG)
            {
                CsrBtPhdcFreeDecodePrstNotifyConfig(pApdu);
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
            EventReportResultSimple *pRors = &pData->choice.u.rors_cmipConfirmedEventReport;
            CsrPmemFree(pRors->event_reply_info.value);
            break;
        }            
        case RORS_CMIP_GET_CHOSEN:
        {
            CsrBtPhdcFreeDecodePrstGetMdsRsp(pApdu);
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

void CsrBtPhdcFreeDataProtoContents(DataProto  *pData)
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

void CsrBtPhdcFreeDecodeAarq(APDU *pApdu)
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
    CsrPmemFree( pDataProtoList->value);
    CsrPmemFree(pApdu);
}

void CsrBtPhdcFreeDecodeAnyApdu(APDU *pApdu)
{   
    switch(pApdu->choice)
    {
        case AARQ_CHOSEN:
        {
            CsrBtPhdcFreeDecodeAarq(pApdu);
            break;
        }
        case PRST_CHOSEN:            
        {
            CsrBtPhdcFreeDecodePrst(pApdu);
            break;
        }
        case AARE_CHOSEN:
        {
            DataProto *pData = (DataProto *)&(((AARE_apdu *)(&pApdu->u.aare))->selected_data_proto);
            if ((pData->data_proto_info.value != NULL) && (pData->data_proto_info.length != 0))
            {
                CsrBtPhdcFreeDataProtoContents(pData);
            }
            CsrPmemFree(pApdu);
            break;
        }
        default:
        {
            CsrPmemFree(pApdu);
            break;   
        }
    }
}

#endif
