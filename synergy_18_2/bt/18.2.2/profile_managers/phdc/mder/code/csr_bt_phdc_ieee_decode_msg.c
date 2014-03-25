/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_MDER_MODULE

/* IEEE Headers */
#include "csr_bt_phdc_ieee_asn.h"
#include "csr_bt_phdc_ieee_endec.h"

/* Libraries Headers */
#include "csr_panic.h"
#include "csr_types.h"
#include "csr_pmem.h"


/************Global Variable Definitions************/

/************Function Prototypes************/

static CsrUint16 csrBtPhdcMderDecodeCsrUint8(const CsrUint8 *msg_ptr, CsrUint8 *val);
static CsrUint16 csrBtPhdcMderDecodeCsrUint16(const CsrUint8 *msg_ptr, CsrUint16 *val);
static CsrUint16 csrBtPhdcMderDecodeCsrUint32(const CsrUint8 *msg_ptr, CsrUint32 *val);
static CsrUint16 csrBtPhdcMderDecodeOctectString(const CsrUint8 *msg_ptr, octet_string * str);
static CsrUint16 csrBtPhdcMderDecodeDataProtoList (const CsrUint8 *msg_ptr, DataProtoList *dataList);
static CsrUint16 csrBtPhdcMderDecodeDataProto(const CsrUint8 *msg_ptr, DataProto *data);
static CsrUint16 csrBtPhdcMderDecodeDataProtoInfo(const CsrUint8 *msg_ptr,DataProtoId data_proto_id, Any *info);
static CsrUint16 csrBtPhdcMderDecodePhdAssociationInformation(const CsrUint8 *msg_ptr, PhdAssociationInformation *value);
static CsrUint16 csrBtPhdcMderDecodeDataReqModeCapab(const CsrUint8 *msg_ptr, DataReqModeCapab *capab);
static CsrUint16 csrBtPhdcMderDecodeAttributeList(const CsrUint8 *msg_ptr, AttributeList *list);
static CsrUint16 csrBtPhdcMderDecodeAVAType(const CsrUint8 *msg_ptr, AVA_Type *value);
static CsrUint16 csrBtPhdcMderDecodeAttributeValue(const CsrUint8 *msg_ptr, Any *attribute_value);
static CsrUint16 csrBtPhdcMderDecodeDataApdu(const CsrUint8 *msg_ptr, DATA_apdu *data_apdu);
static CsrUint16 csrBtPhdcMderDecodeEventReportResultSimple(const CsrUint8 *msg_ptr, EventReportResultSimple *event_report);
static CsrUint16 csrBtPhdcMderDecodeErrorResult(const CsrUint8 *msg_ptr, ErrorResult *pErrorResult);
static CsrUint16 csrBtPhdcMderDecodeRejectResult(const CsrUint8 *msg_ptr, RejectResult *pRejectResult);
static CsrUint16 csrBtPhdcMderDecodeConfigReportRsp(const CsrUint8 *msg_ptr,ConfigReportRsp *config_report_rsp);
static CsrUint16 csrBtPhdcMderDecodeAttributeIdList(const CsrUint8 *msg_ptr, AttributeIdList *attribute_list);
static CsrUint16 csrBtPhdcMderDecodeConfigReport(const CsrUint8 *msg_ptr, ConfigReport *config_report );
static CsrUint16 csrBtPhdcMderDecodeEventReportArgumentSimple(const CsrUint8 *msg_ptr, EventReportArgumentSimple *event_report );
static CsrUint16 csrBtPhdcMderDecodeGetArgumentSimple(const CsrUint8 *msg_ptr, GetArgumentSimple *get_argument);
static CsrUint16 csrBtPhdcMderDecodeGetResultSimple(const CsrUint8 *msg_ptr, GetResultSimple *get_result);

static CsrUint16 csrBtPhdcMderDecodeAarqApdu(const CsrUint8 *msg_ptr, AARQ_apdu *apdu);
static CsrUint16 csrBtPhdcMderDecodeAareApdu(const CsrUint8 *msg_ptr, AARE_apdu *apdu);
static CsrUint16 csrBtPhdcMderDecodeRlrqApdu(const CsrUint8 *msg_ptr, RLRQ_apdu *apdu);
static CsrUint16 csrBtPhdcMderDecodeRlreApdu(const CsrUint8 *msg_ptr, RLRE_apdu *apdu);
static CsrUint16 csrBtPhdcMderDecodeAbrtApdu(const CsrUint8 *msg_ptr, ABRT_apdu *apdu);
static CsrUint16 csrBtPhdcMderDecodePrstApdu(const CsrUint8 *msg_ptr, PRST_apdu *apdu);
static CsrUint16 csrBtPhdcMderDecodeScanReportPerVar(const CsrUint8 *msg_ptr, ScanReportPerVar *scan_report_per_var);
static CsrUint16 csrBtPhdcMderDecodeScanReportPerVarList(const CsrUint8 *msg_ptr, ScanReportPerVarList *scan_report_per_var_list);
static CsrUint16 csrBtPhdcMderDecodeScanReportInfoMPVar(const CsrUint8 *msg_ptr, ScanReportInfoMPVar *scan_report_info_mp_var);
static CsrUint16 csrBtPhdcMderDecodeObservationScan(const CsrUint8 *msg_ptr, ObservationScan *observation_scan);
static CsrUint16 csrBtPhdcMderDecodeObservationScanList(const CsrUint8 *msg_ptr, ObservationScanList *observation_scan_list);
static CsrUint16 csrBtPhdcMderDecodeScanReportInfoVar(const CsrUint8 *msg_ptr, ScanReportInfoVar *scan_report_info_var);
static CsrUint16 csrBtPhdcMderDecodeObservationScanPerFixed(const CsrUint8 *msg_ptr, ScanReportPerFixed *scan_report_per_fixed);
static CsrUint16 csrBtPhdcMderDecodeObservationScanReportPerFixedList(const CsrUint8 *msg_ptr, ScanReportPerFixedList *observation_scan_per_fixed_list);
static CsrUint16 csrBtPhdcMderDecodeScanReportInfoMPFixed(const CsrUint8 *msg_ptr, ScanReportInfoMPFixed *scan_report_info_mp_fixed);
static CsrUint16 csrBtPhdcMderDecodeObservationScanFixed(const CsrUint8 *msg_ptr, ObservationScanFixed *observation_scan_fixed);
static CsrUint16 csrBtPhdcMderDecodeObservationScanFixedList(const CsrUint8 *msg_ptr, ObservationScanFixedList *observation_scan_fixed_list);
static CsrUint16 csrBtPhdcMderDecodeScanReportInfoFixed(const CsrUint8 *msg_ptr, ScanReportInfoFixed *scan_report_info_fixed);

/************Function Definitions************/

static CsrUint16 csrBtPhdcMderDecodeCsrUint8(const CsrUint8 *msg_ptr, CsrUint8 *val)
{
   *val = (msg_ptr[0]);
   
    return 1;
}

static CsrUint16 csrBtPhdcMderDecodeCsrUint16(const CsrUint8 *msg_ptr, CsrUint16 *val)
{
   *val = (msg_ptr[0]  <<  8) | msg_ptr[1] ;
   
    return 2;
}

static CsrUint16 csrBtPhdcMderDecodeCsrUint32(const CsrUint8 *msg_ptr, CsrUint32 *val)
{
   *val = ((CsrUint32)msg_ptr[0] << 24) | ((CsrUint32)msg_ptr[1] << 16) | ((CsrUint32)msg_ptr[2] << 8) | ((CsrUint32)msg_ptr[3]);

   return 4;
}

static CsrUint16 csrBtPhdcMderDecodeOctectString(const CsrUint8 *msg_ptr, octet_string * str)
{
    CsrUint8 i;
    const CsrUint8 *msg_start = msg_ptr; 

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &str->length);

    str->value = (CsrUint8 *)CsrPmemAlloc(str->length);

    for(i = 0; i < str->length; i++)
    {   
        msg_ptr += csrBtPhdcMderDecodeCsrUint8(msg_ptr, &str->value[i]);
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}

CsrUint16 CsrBtPhdcMderDecodeApdu(const CsrUint8 *msg_ptr, APDU **apdu)
{
    const CsrUint8 *msg_start = msg_ptr;
    CsrUint16      csrBtPhdcMderDecodeLen = 0;
    APDU           *apduData = NULL;

    *apdu = (APDU *)CsrPmemZalloc(sizeof(APDU));

    apduData = *apdu;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &apduData->choice);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &apduData->length);


    switch(apduData->choice)
    {
        case AARQ_CHOSEN:
            {
                csrBtPhdcMderDecodeLen = csrBtPhdcMderDecodeAarqApdu(msg_ptr, &apduData->u.aarq);
                break;
            }
        case AARE_CHOSEN:
            {
                csrBtPhdcMderDecodeLen = csrBtPhdcMderDecodeAareApdu(msg_ptr, &apduData->u.aare);
                break;
            }
        case RLRQ_CHOSEN:
            {
                csrBtPhdcMderDecodeLen = csrBtPhdcMderDecodeRlrqApdu(msg_ptr, &apduData->u.rlrq);
                break;
            }
        case RLRE_CHOSEN:
            {
                csrBtPhdcMderDecodeLen = csrBtPhdcMderDecodeRlreApdu(msg_ptr, &apduData->u.rlre);
                break;
            }
        case ABRT_CHOSEN:
            {
                csrBtPhdcMderDecodeLen = csrBtPhdcMderDecodeAbrtApdu(msg_ptr, &apduData->u.abrt);
                break;
            }
        case PRST_CHOSEN:
            {
                csrBtPhdcMderDecodeLen = csrBtPhdcMderDecodePrstApdu(msg_ptr, &apduData->u.prst);
                break;
            }
        default:
            {
                /* csrBtPhdcMderDecodeLen = apduData->length; */
                break;
                
            }
    }

    msg_ptr += csrBtPhdcMderDecodeLen;
    csrBtPhdcMderDecodeLen = (CsrUint16)(msg_ptr - msg_start);

    msg_ptr = msg_start;

    return (csrBtPhdcMderDecodeLen);
}


static CsrUint16 csrBtPhdcMderDecodeDataProtoList (const CsrUint8 *msg_ptr, DataProtoList *dataList)
{
    const CsrUint8 *msg_start = msg_ptr;
    CsrUint16 i; 


    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &dataList->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &dataList->length);

    if(dataList->count > 0)
    {
        dataList->value = (DataProto *)CsrPmemAlloc(sizeof(DataProto) * dataList->count );
        
        for(i = 0; i < dataList->count; i++)
        {
            msg_ptr += csrBtPhdcMderDecodeDataProto(msg_ptr, &dataList->value[i]);
        }
    }
    else
    {
        dataList->value = NULL;
    }

    return ((CsrUint16)(msg_ptr - msg_start));

}


CsrUint16 csrBtPhdcMderDecodeAarqApdu(const CsrUint8 *msg_ptr, AARQ_apdu *apdu)
{
    const CsrUint8 *msg_start = msg_ptr;


    msg_ptr += csrBtPhdcMderDecodeCsrUint32(msg_ptr, &apdu->assoc_version);
    msg_ptr += csrBtPhdcMderDecodeDataProtoList(msg_ptr, &apdu->data_proto_list);

    return ((CsrUint16)(msg_ptr - msg_start));
}


CsrUint16 csrBtPhdcMderDecodeAareApdu(const CsrUint8 *msg_ptr, AARE_apdu *apdu)
{
   const CsrUint8 *msg_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &apdu->result);      
   msg_ptr += csrBtPhdcMderDecodeDataProto(msg_ptr, &apdu->selected_data_proto); 

   return ((CsrUint16)(msg_ptr - msg_start));
  
}

static CsrUint16  csrBtPhdcMderDecodeDataProto(const CsrUint8 *msg_ptr, DataProto *data)
{  
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &data->data_proto_id);       
    msg_ptr += csrBtPhdcMderDecodeDataProtoInfo(msg_ptr,data->data_proto_id, &data->data_proto_info); 

    return ((CsrUint16)(msg_ptr - msg_start));

}


static CsrUint16 csrBtPhdcMderDecodeDataProtoInfo(const CsrUint8 *msg_ptr, DataProtoId data_proto_id, Any *info)
{

    const CsrUint8 *msg_start = msg_ptr;
      
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &info->length);       

    if(info->length > 0 && data_proto_id == DATA_PROTO_ID_20601)
    {
        /* Allocate the required memory: */   
        info->value = (CsrUint8 *)CsrPmemAlloc(sizeof(PhdAssociationInformation)); 

        msg_ptr += csrBtPhdcMderDecodePhdAssociationInformation(msg_ptr, (PhdAssociationInformation *)info->value);
    }
    else
    {
        /* Jump past unsupported protocol */
        msg_ptr += info->length;        
        
        info->value = NULL;
        info->length = 0;
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodePhdAssociationInformation(const CsrUint8 *msg_ptr, PhdAssociationInformation *value)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint32(msg_ptr, (CsrUint32 *)&value->protocolVersion);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr,&value->encodingRules);
    msg_ptr += csrBtPhdcMderDecodeCsrUint32(msg_ptr, (CsrUint32 *)&value->nomenclatureVersion);
    msg_ptr += csrBtPhdcMderDecodeCsrUint32(msg_ptr, (CsrUint32 *)&value->functionalUnits);
    msg_ptr += csrBtPhdcMderDecodeCsrUint32(msg_ptr, (CsrUint32 *)&value->systemType);
    msg_ptr += csrBtPhdcMderDecodeOctectString(msg_ptr, &value->systemId);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &value->dev_config_id);
    msg_ptr += csrBtPhdcMderDecodeDataReqModeCapab(msg_ptr, &value->data_req_mode_capab);
    msg_ptr += csrBtPhdcMderDecodeAttributeList(msg_ptr, &value->optionList);

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeDataReqModeCapab(const CsrUint8 *msg_ptr, DataReqModeCapab *capab)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &capab->data_req_mode_flags);
    msg_ptr += csrBtPhdcMderDecodeCsrUint8(msg_ptr, &capab->data_req_init_agent_count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint8(msg_ptr, &capab->data_req_init_manager_count);

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeAttributeList(const CsrUint8 *msg_ptr, AttributeList *list)
{
    CsrUint16 cnt;
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &list->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &list->length);

    if(list->count > 0)
    {
        /* Allocate the required memory: */
        list->value = (AVA_Type *)CsrPmemAlloc(sizeof(AVA_Type)*list->count);
        
        for(cnt = 0; cnt < list->count; cnt++)
        {
            msg_ptr += csrBtPhdcMderDecodeAVAType(msg_ptr, &list->value[cnt]);
        }
    }
    else
    {
        list->value = NULL;
    }

    return ((CsrUint16)(msg_ptr - msg_start));

}

static CsrUint16 csrBtPhdcMderDecodeAVAType(const CsrUint8 *msg_ptr, AVA_Type *value)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &value->attribute_id);
    msg_ptr += csrBtPhdcMderDecodeAttributeValue(msg_ptr, &value->attribute_value);  

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeAttributeValue(const CsrUint8 *msg_ptr, Any *attribute_value)
{
    CsrUint16 i;
    const CsrUint8 *msg_start = msg_ptr;


    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &attribute_value->length);       

    /* THAG - As per messages we may need to handle this part. 
    Not required for BPM, WS & PDM. Will look at it later for other device specializations */
    attribute_value->value = (CsrUint8 *)CsrPmemAlloc(attribute_value->length);

    for(i = 0; i < attribute_value->length; i++)
    {
        msg_ptr += csrBtPhdcMderDecodeCsrUint8(msg_ptr, (CsrUint8 *)&attribute_value->value[i]);
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}


CsrUint16 csrBtPhdcMderDecodeRlrqApdu(const CsrUint8 *msg_ptr, RLRQ_apdu *apdu)
{
   const CsrUint8 *msg_start = msg_ptr;
   
   
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &apdu->reason);

   return ((CsrUint16)(msg_ptr - msg_start));
}

CsrUint16 csrBtPhdcMderDecodeRlreApdu(const CsrUint8 *msg_ptr, RLRE_apdu *apdu)
{
   const CsrUint8 *msg_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &apdu->reason);

   return ((CsrUint16)(msg_ptr - msg_start));
}

CsrUint16 csrBtPhdcMderDecodeAbrtApdu(const CsrUint8 *msg_ptr, ABRT_apdu *apdu)
{
   const CsrUint8 *msg_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &apdu->reason);

   return ((CsrUint16)(msg_ptr - msg_start));
}

CsrUint16 csrBtPhdcMderDecodePrstApdu(const CsrUint8 *msg_ptr, PRST_apdu *apdu)
{
    const CsrUint8 *msg_start = msg_ptr;
    DATA_apdu *data_apdu;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &apdu->length);

    apdu->value = (CsrUint8 *)CsrPmemAlloc(sizeof(DATA_apdu));
    data_apdu = (DATA_apdu *)apdu->value;

    msg_ptr += csrBtPhdcMderDecodeDataApdu(msg_ptr, data_apdu);

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeDataApdu(const CsrUint8 *msg_ptr, DATA_apdu *data_apdu)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &data_apdu->invoke_id);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &data_apdu->choice.choice);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &data_apdu->choice.length);   

    switch(data_apdu->choice.choice)
    {
        case ROIV_CMIP_GET_CHOSEN:
            msg_ptr += csrBtPhdcMderDecodeGetArgumentSimple(msg_ptr, &data_apdu->choice.u.roiv_cmipGet);
            break;
        case RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
            msg_ptr += csrBtPhdcMderDecodeEventReportResultSimple(msg_ptr, &data_apdu->choice.u.rors_cmipConfirmedEventReport);
            break;
        case ROER_CHOSEN:
            msg_ptr += csrBtPhdcMderDecodeErrorResult(msg_ptr, &data_apdu->choice.u.roer);
            break;
        case RORJ_CHOSEN:
            msg_ptr += csrBtPhdcMderDecodeRejectResult(msg_ptr, &data_apdu->choice.u.rorj);
            break;
        case ROIV_CMIP_EVENT_REPORT_CHOSEN:
        case ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderDecodeEventReportArgumentSimple(msg_ptr, &data_apdu->choice.u.roiv_cmipConfirmedEventReport);
            break;
        }            
        case RORS_CMIP_GET_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderDecodeGetResultSimple(msg_ptr, &data_apdu->choice.u.rors_cmipGet);
            break;
        }

        case ROIV_CMIP_SET_CHOSEN:
        case ROIV_CMIP_CONFIRMED_SET_CHOSEN:
        case ROIV_CMIP_ACTION_CHOSEN:
        case ROIV_CMIP_CONFIRMED_ACTION_CHOSEN:      
        case RORS_CMIP_CONFIRMED_SET_CHOSEN:
        case RORS_CMIP_CONFIRMED_ACTION_CHOSEN:
        {
            break;
        }
        default:
        {
        }
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeConfigObject(const CsrUint8 *msg_ptr, ConfigObject *object )
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &object->obj_class);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &object->obj_handle);
    msg_ptr += csrBtPhdcMderDecodeAttributeList(msg_ptr, &object->attributes);

    return ((CsrUint16)(msg_ptr - msg_start));
    
}



static CsrUint16 csrBtPhdcMderDecodeConfigObjectList(const CsrUint8 *msg_ptr, ConfigObjectList *objectList )
{
    const CsrUint8 *msg_start = msg_ptr;
    CsrUint16 i; 

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &objectList->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &objectList->length);

    if(objectList->count > 0)
    {
        objectList->value = (ConfigObject *)CsrPmemAlloc(sizeof(ConfigObject) * objectList->count );

        for(i = 0; i < objectList->count; i++)
        {
            msg_ptr += csrBtPhdcMderDecodeConfigObject(msg_ptr, &objectList->value[i]);
        }
    }
    else
    {
        objectList->value = NULL;
    }

    return ((CsrUint16)(msg_ptr - msg_start));

}



static CsrUint16 csrBtPhdcMderDecodeConfigReport(const CsrUint8 *msg_ptr, ConfigReport *config_report )
{
   const CsrUint8 *msg_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr,&config_report->config_report_id);
   msg_ptr += csrBtPhdcMderDecodeConfigObjectList(msg_ptr,&config_report->config_obj_list);

   return ((CsrUint16)(msg_ptr - msg_start));

}


static CsrUint16 csrBtPhdcMderDecodeEventReportArgumentSimple(const CsrUint8 *msg_ptr, EventReportArgumentSimple *event_report )
{
   const CsrUint8 *msg_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &event_report->obj_handle);
   msg_ptr += csrBtPhdcMderDecodeCsrUint32(msg_ptr, &event_report->event_time);
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &event_report->event_type);
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &event_report->event_info.length);
   
    switch(event_report->event_type)
    {
        case MDC_NOTI_CONFIG:
        {
            event_report->event_info.value = CsrPmemAlloc(sizeof(ConfigReport));
            msg_ptr += csrBtPhdcMderDecodeConfigReport(msg_ptr, (ConfigReport *)event_report->event_info.value);
            break;
        }
        case MDC_NOTI_SCAN_REPORT_FIXED:
        {
            event_report->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoFixed));
            msg_ptr += csrBtPhdcMderDecodeScanReportInfoFixed(msg_ptr, (ScanReportInfoFixed *) event_report->event_info.value);
            break;
        }
        case MDC_NOTI_SCAN_REPORT_MP_FIXED:
        {
            event_report->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoMPFixed));
            msg_ptr += csrBtPhdcMderDecodeScanReportInfoMPFixed(msg_ptr,(ScanReportInfoMPFixed *)event_report->event_info.value);
            break;
        }
        case MDC_NOTI_SCAN_REPORT_VAR:
        {
            event_report->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoVar));            
            msg_ptr += csrBtPhdcMderDecodeScanReportInfoVar(msg_ptr, (ScanReportInfoVar *)event_report->event_info.value);
            break;
        }

        case MDC_NOTI_SCAN_REPORT_MP_VAR:
        {
            event_report->event_info.value = CsrPmemAlloc(sizeof(ScanReportInfoMPVar));            
            msg_ptr += csrBtPhdcMderDecodeScanReportInfoMPVar(msg_ptr, (ScanReportInfoMPVar *)event_report->event_info.value);
            break;
        }
        default:
        {
            event_report->event_info.value = NULL;
        }
    }

    return ((CsrUint16)(msg_ptr - msg_start));

    
}

static CsrUint16 csrBtPhdcMderDecodeGetArgumentSimple(const CsrUint8 *msg_ptr, GetArgumentSimple *get_argument)
{
   const CsrUint8 *msg_start = msg_ptr;
   

   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &get_argument->obj_handle);
   
   msg_ptr += csrBtPhdcMderDecodeAttributeIdList(msg_ptr, &get_argument->attribute_id_list);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeGetResultSimple(const CsrUint8 *msg_ptr, GetResultSimple *get_result)
{
   const CsrUint8 *msg_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &get_result->obj_handle);
   
   msg_ptr += csrBtPhdcMderDecodeAttributeList(msg_ptr, &get_result->attribute_list);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeAttributeIdList(const CsrUint8 *msg_ptr, AttributeIdList *attribute_list)
{
    CsrUint16      cnt;
    const CsrUint8 *msg_start = msg_ptr;


    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &attribute_list->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &attribute_list->length);

    if(attribute_list->count > 0)
    {
        attribute_list->value = (OID_Type *)CsrPmemAlloc(sizeof(OID_Type) * attribute_list->count);

        for(cnt = 0; cnt < attribute_list->count; cnt++)
        {
            msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, (CsrUint16 *)&attribute_list->value[cnt]);
        }
    }
    else
    {
        attribute_list->value = NULL;
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeEventReportResultSimple(const CsrUint8 *msg_ptr, EventReportResultSimple *event_report)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &event_report->obj_handle);
    msg_ptr += csrBtPhdcMderDecodeCsrUint32(msg_ptr, (CsrUint32 *)&event_report->currentTime);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &event_report->event_type);
    
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &event_report->event_reply_info.length);

    if(event_report->event_reply_info.length > 0)
    {
        switch(event_report->event_type)
        {
            case MDC_NOTI_CONFIG:
            {
                event_report->event_reply_info.value = (CsrUint8 *)CsrPmemAlloc(sizeof(ConfigReportRsp));
                msg_ptr += csrBtPhdcMderDecodeConfigReportRsp(msg_ptr, (ConfigReportRsp *)event_report->event_reply_info.value);
                break;
            }
            case MDC_NOTI_SCAN_REPORT_FIXED:
            {
                 /* Need to check spec for the case when Manager responds with some data for 
                          SCAN_REPORT_FIXED confirmation */
                event_report->event_reply_info.value = NULL;
                break;
            }
            default:
            {
                event_report->event_reply_info.value = NULL;
                break;
            }
        }
    }
    else
    {
        event_report->event_reply_info.value = NULL;
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeErrorResult(const CsrUint8 *msg_ptr, ErrorResult *pErrorResult)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &pErrorResult->error_value);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &pErrorResult->parameter.length);

    if(pErrorResult->parameter.length > 0)
    {
        /* For error value NOT_ALLOWED_BY_OBJECT, ACTION_TIMED_OUT or ACTION_ABORTED */
        /* THAG - Handling this needs to be clarified with IEEE. IEEE_CHECK*/

        if(pErrorResult->parameter.length == 2) /* Length of OID_Type - See RoerErrorValue in 11073-20601 */
        {
            pErrorResult->parameter.value = (CsrUint8 *)CsrPmemAlloc(sizeof(OID_Type));
            msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, (CsrUint16 *)pErrorResult->parameter.value);
        }
        else
        {
            /* Spec doesn't suggest the value to be more than sizeof(OID_Type), so skip the number
                of bytes for not affecting the remaining debugging */
            msg_ptr += pErrorResult->parameter.length;
            pErrorResult->parameter.value = NULL;
        }
    }
    else
    {
        pErrorResult->parameter.value = NULL;
    }


    return ((CsrUint16)(msg_ptr - msg_start));

}

static CsrUint16 csrBtPhdcMderDecodeRejectResult(const CsrUint8 *msg_ptr, RejectResult *pRejectResult)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &pRejectResult->problem);

    return ((CsrUint16)(msg_ptr - msg_start));

}

static CsrUint16 csrBtPhdcMderDecodeConfigReportRsp(const CsrUint8 *msg_ptr,ConfigReportRsp *config_report_rsp)
{
    const CsrUint8 *msg_start = msg_ptr;

    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &config_report_rsp->config_report_id);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &config_report_rsp->config_result);   

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeScanReportPerVar(const CsrUint8 *msg_ptr, ScanReportPerVar *scan_report_per_var)
{
   const CsrUint8 *msg_start = msg_ptr;
   
 
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_per_var->person_id);

   msg_ptr += csrBtPhdcMderDecodeObservationScanList(msg_ptr, &scan_report_per_var->obs_scan_var);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeScanReportPerVarList(const CsrUint8 *msg_ptr, ScanReportPerVarList *scan_report_per_var_list)
{
    CsrUint8 i;
    const CsrUint8 *msg_start = msg_ptr;
    
   
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_per_var_list->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_per_var_list->length);
        
    scan_report_per_var_list->value = 
        (ScanReportPerVar *)CsrPmemAlloc((scan_report_per_var_list->count)*sizeof(ScanReportPerVar));

    for(i=0;i<scan_report_per_var_list->count;i++)
    {
        msg_ptr += csrBtPhdcMderDecodeScanReportPerVar(msg_ptr,&scan_report_per_var_list->value[i]);
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeScanReportInfoMPVar(const CsrUint8 *msg_ptr, ScanReportInfoMPVar *scan_report_info_mp_var)
{
   const CsrUint8 *msg_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_mp_var->data_req_id);
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_mp_var->scan_report_no);     

   msg_ptr += csrBtPhdcMderDecodeScanReportPerVarList(msg_ptr, &scan_report_info_mp_var->scan_per_var);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeObservationScan(const CsrUint8 *msg_ptr, ObservationScan *observation_scan)
{
    const CsrUint8 *msg_start = msg_ptr;
    
   
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan->obj_handle);        
    msg_ptr += csrBtPhdcMderDecodeAttributeList(msg_ptr,&observation_scan->attributes);


    return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeObservationScanList(const CsrUint8 *msg_ptr, ObservationScanList *observation_scan_list)
{
    CsrUint8 i;
    const CsrUint8 *msg_start = msg_ptr;
    
   
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_list->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_list->length);
        
    observation_scan_list->value = 
        (ObservationScan *)CsrPmemAlloc((observation_scan_list->count)*sizeof(ObservationScan));

    for(i=0;i<observation_scan_list->count;i++)
    {
        msg_ptr += csrBtPhdcMderDecodeObservationScan(msg_ptr,&observation_scan_list->value[i]);
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeScanReportInfoVar(const CsrUint8 *msg_ptr, ScanReportInfoVar *scan_report_info_var)
{
   const CsrUint8 *msg_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_var->data_req_id);
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_var->scan_report_no);     

   msg_ptr += csrBtPhdcMderDecodeObservationScanList(msg_ptr, &scan_report_info_var->obs_scan_var);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeObservationScanPerFixed(const CsrUint8 *msg_ptr, ScanReportPerFixed *scan_report_per_fixed)
{
   const CsrUint8 *msg_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_per_fixed->person_id);

   msg_ptr += csrBtPhdcMderDecodeObservationScanFixedList(msg_ptr, &scan_report_per_fixed->obs_scan_fix);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeObservationScanReportPerFixedList(const CsrUint8 *msg_ptr, ScanReportPerFixedList *observation_scan_per_fixed_list)
{
    CsrUint8 i;
    const CsrUint8 *msg_start = msg_ptr;
    
   
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_per_fixed_list->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_per_fixed_list->length);
        
    observation_scan_per_fixed_list->value = 
        (ScanReportPerFixed *)CsrPmemAlloc((observation_scan_per_fixed_list->count)*sizeof(ScanReportPerFixed));

    for(i=0;i<observation_scan_per_fixed_list->count;i++)
    {
        msg_ptr += csrBtPhdcMderDecodeObservationScanPerFixed(msg_ptr,&observation_scan_per_fixed_list->value[i]);
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeScanReportInfoMPFixed(const CsrUint8 *msg_ptr, ScanReportInfoMPFixed *scan_report_info_mp_fixed)
{
   const CsrUint8 *msg_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_mp_fixed->data_req_id);
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_mp_fixed->scan_report_no);     

   msg_ptr += csrBtPhdcMderDecodeObservationScanReportPerFixedList(msg_ptr, &scan_report_info_mp_fixed->scan_per_fixed);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}

static CsrUint16 csrBtPhdcMderDecodeObservationScanFixed(const CsrUint8 *msg_ptr, ObservationScanFixed *observation_scan_fixed)
{
    const CsrUint8 *msg_start = msg_ptr;
    
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_fixed->obj_handle);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_fixed->obs_val_data.length);

    observation_scan_fixed->obs_val_data.value = 
                        (CsrUint8 *)CsrPmemAlloc(observation_scan_fixed->obs_val_data.length);

    CsrMemCpy(observation_scan_fixed->obs_val_data.value,msg_ptr,observation_scan_fixed->obs_val_data.length);
    
    msg_ptr += observation_scan_fixed->obs_val_data.length;
    
    return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeObservationScanFixedList(const CsrUint8 *msg_ptr, ObservationScanFixedList *observation_scan_fixed_list)
{
    CsrUint8 i;
    const CsrUint8 *msg_start = msg_ptr;
    
   
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_fixed_list->count);
    msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &observation_scan_fixed_list->length);
        
    observation_scan_fixed_list->value = 
        (ObservationScanFixed *)CsrPmemAlloc((observation_scan_fixed_list->count)*sizeof(ObservationScanFixed));

    for(i=0;i<observation_scan_fixed_list->count;i++)
    {
        msg_ptr += csrBtPhdcMderDecodeObservationScanFixed(msg_ptr,&observation_scan_fixed_list->value[i]);
    }

    return ((CsrUint16)(msg_ptr - msg_start));
}


static CsrUint16 csrBtPhdcMderDecodeScanReportInfoFixed(const CsrUint8 *msg_ptr, ScanReportInfoFixed *scan_report_info_fixed)
{
   const CsrUint8 *msg_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_fixed->data_req_id);
   msg_ptr += csrBtPhdcMderDecodeCsrUint16(msg_ptr, &scan_report_info_fixed->scan_report_no);     

   msg_ptr += csrBtPhdcMderDecodeObservationScanFixedList(msg_ptr, &scan_report_info_fixed->obs_scan_fixed);
 
   return ((CsrUint16)(msg_ptr - msg_start));
}


#endif
