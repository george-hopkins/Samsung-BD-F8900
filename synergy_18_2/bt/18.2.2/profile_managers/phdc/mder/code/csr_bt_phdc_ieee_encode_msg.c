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
#include "csr_bt_phdc_ieee_utils.h"
#include "csr_bt_phdc_ieee_dev_specific.h"
/* Libraries Headers */
#include "csr_panic.h"
#include "csr_types.h"
#include "csr_pmem.h"

#define IEEE_MAX_TX_APDU_SIZE   (896)

#define NO_OF_BYTES_FOR_LEN 2

/************Global Variable Definitions************/

/************Function Prototypes************/

static CsrUint16 csrBtPhdcMderEncodeInt8(const CsrUint8 val, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeInt16(const CsrUint16 val, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeInt32(const CsrUint32 val, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeOctectString(const CsrUint16 length, const CsrUint8 *str, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeDataProtoList(const DataProtoList *list, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeDataProto(const DataProto *const data, CsrUint8 * msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeDataProtoInfo(const Any *info,CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodePhdAssociationInformation(const PhdAssociationInformation *const phd_asso_info , CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeDataReqModeCapab(const DataReqModeCapab *const capab, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeAttributeList(const AttributeList *const attribute_list,CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeReportChoice(const DATA_apdu *const report_choice, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeEventReportArgumentSimple(const EventReportArgumentSimple *event_report, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeGetArgumentSimple(const GetArgumentSimple *event_report, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeErrorResult(const ErrorResult *const err_result, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeAttributeValue(const Any *const attribute_value, CsrUint8 *msg_ptr, OID_Type attribute_id,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeAbsoluteTime(const AbsoluteTime *const absTime, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeEnumObsVal(const EnumObsValue *const enum_obs_val, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeProductionSpec(const ProductionSpec *const production_spec, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeScanReportInfoFixed(const ScanReportInfoFixed *const scan_report_info_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeScanReportInfoVar(const ScanReportInfoVar *const scan_report_info_var, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeObservationScanFixedList(const ObservationScanFixedList *const observation_scan_fixed_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeObservationScanList(const ObservationScanList *const observation_scan_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeObservationScanFixed(const ObservationScanFixed *const observation_scan_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeObservationScan(const ObservationScan *const observation_scan, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeTYPE(const TYPE *const type, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeMetricStructSmall(const MetricStructureSmall *const metStructSmall, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeEventReportResultSimple(const EventReportResultSimple *const event_report, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeGetResultSimple(const GetResultSimple *const get_result, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeMdcNotiConfigRsp(const ConfigReportRsp *const config_report_rsp, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeMdcNotiConfig(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeMdcNotiScanReportFixed(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeMdcNotiScanReportVar(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeMdcNotiScanReportMPFixed(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeScanReportInfoMPFixed(const ScanReportInfoMPFixed *const scan_report_info_mp_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeScanReportPerFixedList(const ScanReportPerFixedList *const scan_report_per_fixed_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeScanReportPerFixed(const ScanReportPerFixed *const scan_report_per_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeConfigObjectList(const ConfigObjectList *const config_object_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeAttributeIdList (const AttributeIdList *const attribute_id_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeAarqApdu(const AARQ_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeAareApdu(const AARE_apdu *const apdu, CsrUint8 * msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeRlrqApdu(const RLRQ_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeRlreApdu(const RLRE_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodeAbrtApdu(const ABRT_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
static CsrUint16 csrBtPhdcMderEncodePrstApdu(const PRST_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
/* Vendor specific information not supported */
/*
static CsrUint16 csrBtPhdcMderEncodeVendorSpecificData(AttrDataFormat *pAttrDataFrmt, void *pMeasData, CsrUint8 *msg_ptr,CsrBool calEncStrLen);
*/


/************Function Definitions************/

static CsrUint16 csrBtPhdcMderEncodeInt8(const CsrUint8 val, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    /* Write to memory only if the csrBtPhdcMderEncode routine is not called for csrBtPhdcMderEncode 
        string length calculation */
    if(calEncStrLen == FALSE)
    {
        msg_ptr[0] = val;
    }
    return 1;
}

static CsrUint16 csrBtPhdcMderEncodeInt16(const CsrUint16 val, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    /* Write to memory only if the csrBtPhdcMderEncode routine is not called for csrBtPhdcMderEncode 
        string length calculation */
    if(calEncStrLen == FALSE)
    {    
        msg_ptr[0] = (CsrUint8)((val & 0xff00)>>8);
        msg_ptr[1] = (val & 0x00ff);
    }

    return 2;
}

static CsrUint16 csrBtPhdcMderEncodeInt32(const CsrUint32 val, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    /* Write to memory only if the csrBtPhdcMderEncode routine is not called for csrBtPhdcMderEncode 
        string length calculation */
    if(calEncStrLen == FALSE)
    {    
        msg_ptr[0] = (CsrUint8)((val & 0xff000000)>>24);
        msg_ptr[1] = (CsrUint8)((val & 0x00ff0000)>>16);
        msg_ptr[2] = (CsrUint8)((val & 0x0000ff00)>>8);
        msg_ptr[3] = (CsrUint8)(val & 0x000000ff);
    }
    return 4;
}

static CsrUint16 csrBtPhdcMderEncodeOctectString(const CsrUint16 length, const CsrUint8 *str, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;

    /* Write to memory only if the csrBtPhdcMderEncode routine is not called for csrBtPhdcMderEncode 
        string length calculation */
    if(calEncStrLen == FALSE)
    {

        msg_ptr += csrBtPhdcMderEncodeInt16(length, msg_ptr,calEncStrLen);

        for(i = 0; i < length; i++)
        {
            msg_ptr[i] = str[i];      
        }
    }
    return (length + 2);
}

/*
static void csrBtPhdcMderEncodeFloat(unit8 exponent, CsrUint32 mantissa, CsrUint8 * csrBtPhdcMderEncode_arr, CsrUint8 * offset)
{
   CsrUint32 val = ((~mantissa) & 0x00ffffff) |0x01;
   
   csrBtPhdcMderEncode_arr[*offset] = (~exponent)|0x01;
   (*offset)++;
   csrBtPhdcMderEncode_arr[*offset] = (val & 0x00ff0000) >> 16;   
   (*offset)++;
   csrBtPhdcMderEncode_arr[*offset] = (val & 0x0000ff00) >> 8;   
   (*offset)++;
   csrBtPhdcMderEncode_arr[*offset] = (val & 0x000000ff);   
}
static void csrBtPhdcMderEncodeSfloat(unit8 exponent, CsrUint16 mantissa, CsrUint8 * csrBtPhdcMderEncode_arr, CsrUint8 * offset)
{
   (*offset)++;
   csrBtPhdcMderEncode_arr[*offset] = (CsrUint8)((~mantissa)&0x00ff)|0x01;
   if(((~mantissa)&0x00ff) == 0x00ff)
   {
      csrBtPhdcMderEncode_arr[*offset-1] = ((~exponent) |0x01)&0x0f)|(CsrUint8)((((~mantissa)&0x0f00)>>8)|0x01);
   }
   else
   {
      csrBtPhdcMderEncode_arr[*offset-1] = ((~exponent) |0x01)&0x0f)|(CsrUint8)(((~mantissa)&0x0f00)>>8);
   }

}*/

CsrUint16 CsrBtPhdcMderCalculateApduEncodedLength(const APDU *const apduData)
{
    /* Encode and transfer the APDU */
    CsrUint8           *msg_ptr = NULL;
    CsrUint8           *msg_start = NULL;
    CsrUint16          csrBtPhdcMderEncodeLen = 0;

    /* Allocate word size memory to just act as a filler for re-using the csrBtPhdcMderEncode 
        routines. Though, the pointer is getting incremented but the memory will 
        never be written. The csrBtPhdcMderEncode routines will be used just to calculate 
        csrBtPhdcMderEncoded string memory requirements. */
    msg_start = (CsrUint8 *)CsrPmemAlloc(sizeof(CsrUint16));

    msg_ptr = msg_start;

    msg_ptr += csrBtPhdcMderEncodeInt16(apduData->choice, msg_ptr,TRUE);

    msg_ptr += NO_OF_BYTES_FOR_LEN;

    switch(apduData->choice)
    {
        case AARQ_CHOSEN:
            {
                csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeAarqApdu(&apduData->u.aarq, msg_ptr,TRUE);
                break;
            }
        case AARE_CHOSEN:
            {
                csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeAareApdu(&apduData->u.aare, msg_ptr,TRUE);
                break;
            }
        case RLRQ_CHOSEN:
            {
                csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeRlrqApdu(&apduData->u.rlrq, msg_ptr,TRUE);
                break;
            }
        case RLRE_CHOSEN:
            {
                csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeRlreApdu(&apduData->u.rlre, msg_ptr,TRUE);
                break;
            }
        case ABRT_CHOSEN:
            {
                csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeAbrtApdu(&apduData->u.abrt, msg_ptr,TRUE);
                break;
            }
        case PRST_CHOSEN:
            {
                csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodePrstApdu(&apduData->u.prst, msg_ptr,TRUE);
                break;
            }
        default:
            {
                
            }
    }

    /* Reuse 'csrBtPhdcMderEncodeLen' variable for storing final length as well */
    csrBtPhdcMderEncodeLen += (CsrUint16)(msg_ptr - msg_start);

 
    CsrPmemFree(msg_start);

    return csrBtPhdcMderEncodeLen;
}

/* Caller of this function has to allocate memory for *msg_ptr.
   size of mmeory to be allocated is calculated by using function 
   CsrBtPhdcMderCalculateApduEncodedLength
   */
void CsrBtPhdcMderEncodeApdu(const APDU *const apduData, CsrUint8 *msg_ptr)
{
    /* Encode the APDU */    
    CsrUint8           *lenPtr = NULL; 
    CsrUint16          csrBtPhdcMderEncodeLen = 0;
    CsrBool calEncStrLen = FALSE;

    msg_ptr += csrBtPhdcMderEncodeInt16(apduData->choice, msg_ptr,calEncStrLen);

    lenPtr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    switch(apduData->choice)
    {
    case AARQ_CHOSEN:
        {
            csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeAarqApdu(&apduData->u.aarq, msg_ptr,calEncStrLen);
            break;
        }
    case AARE_CHOSEN:
        {
            csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeAareApdu(&apduData->u.aare, msg_ptr,calEncStrLen);
            break;
        }
    case RLRQ_CHOSEN:
        {
            csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeRlrqApdu(&apduData->u.rlrq, msg_ptr,calEncStrLen);
            break;
        }
    case RLRE_CHOSEN:
        {
            csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeRlreApdu(&apduData->u.rlre, msg_ptr,calEncStrLen);
            break;
        }
    case ABRT_CHOSEN:
        {
            csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodeAbrtApdu(&apduData->u.abrt, msg_ptr,calEncStrLen);
            break;
        }
    case PRST_CHOSEN:
        {
            csrBtPhdcMderEncodeLen = csrBtPhdcMderEncodePrstApdu(&apduData->u.prst, msg_ptr,calEncStrLen);
            break;
        }
    default:
        {

        }
    }

    msg_ptr += csrBtPhdcMderEncodeLen;

    /* Store the csrBtPhdcMderEncoded length */
    (void)csrBtPhdcMderEncodeInt16(csrBtPhdcMderEncodeLen, lenPtr,calEncStrLen);
}

CsrUint16 csrBtPhdcMderEncodeAarqApdu(const AARQ_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{   
   CsrUint8 * msg_start = msg_ptr;
   
 
   msg_ptr += csrBtPhdcMderEncodeInt32(apdu->assoc_version, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeDataProtoList(&apdu->data_proto_list, msg_ptr,calEncStrLen);      


   return (CsrUint16)(msg_ptr - msg_start);
}

static CsrUint16 csrBtPhdcMderEncodeDataProtoList(const DataProtoList *list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *data_proto_list_start = msg_ptr;
    CsrUint8 *data_proto_list_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(list->count, msg_ptr,calEncStrLen);
    
    data_proto_list_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    for(i = 0; i < list->count; i++)
    {
        msg_ptr += csrBtPhdcMderEncodeDataProto((list->value + i), msg_ptr,calEncStrLen);
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - data_proto_list_len_ptr -NO_OF_BYTES_FOR_LEN), data_proto_list_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - data_proto_list_start); 
}

static CsrUint16 csrBtPhdcMderEncodeDataProto(const DataProto *const data_proto, CsrUint8 * msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *DataProto_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderEncodeInt16(data_proto->data_proto_id, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeDataProtoInfo(&data_proto->data_proto_info, msg_ptr,calEncStrLen);
   
   return (CsrUint16)(msg_ptr - DataProto_start);
}

static CsrUint16 csrBtPhdcMderEncodeDataProtoInfo(const Any *info,CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *info_start = msg_ptr;
    CsrUint8 *info_start_len_ptr;

    info_start_len_ptr = msg_ptr;
    msg_ptr +=  NO_OF_BYTES_FOR_LEN;

    if(info->value != NULL)
    {
        msg_ptr += csrBtPhdcMderEncodePhdAssociationInformation((PhdAssociationInformation *)info->value, msg_ptr,calEncStrLen);
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - info_start_len_ptr - NO_OF_BYTES_FOR_LEN), info_start_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - info_start);
}

static CsrUint16 csrBtPhdcMderEncodePhdAssociationInformation(const PhdAssociationInformation *const phd_asso_info , CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *phd_asso_info_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderEncodeInt32(phd_asso_info->protocolVersion, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeInt16(phd_asso_info->encodingRules, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeInt32(phd_asso_info->nomenclatureVersion, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeInt32(phd_asso_info->functionalUnits, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeInt32(phd_asso_info->systemType, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeOctectString(phd_asso_info->systemId.length, (CsrUint8 *)phd_asso_info->systemId.value, msg_ptr,calEncStrLen);
   
   msg_ptr += csrBtPhdcMderEncodeInt16(phd_asso_info->dev_config_id, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeDataReqModeCapab(&phd_asso_info->data_req_mode_capab, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeAttributeList(&phd_asso_info->optionList, msg_ptr,calEncStrLen);
   
   return (CsrUint16)(msg_ptr - phd_asso_info_start);
}

static CsrUint16 csrBtPhdcMderEncodeDataReqModeCapab(const DataReqModeCapab *const capab, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *capab_start = msg_ptr;
    
    msg_ptr += csrBtPhdcMderEncodeInt16(capab->data_req_mode_flags, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(capab->data_req_init_agent_count, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(capab->data_req_init_manager_count, msg_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - capab_start);
}

static CsrUint16  csrBtPhdcMderEncodeAttributeList(const AttributeList *const attribute_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *attribute_list_start = msg_ptr;
    CsrUint8 *attribute_list_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(attribute_list->count, msg_ptr,calEncStrLen);

    attribute_list_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    for(i = 0; i < attribute_list->count; i++)
    {
        if((((AVA_Type *)(attribute_list->value + i))->attribute_id != MDC_MDS_OBJ_ATTR_VENDOR_SPECIFIC) &&
           (((AVA_Type *)(attribute_list->value + i))->attribute_id != MDC_OBJ_ATTR_VENDOR_SPECIFIC))
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(((AVA_Type *)(attribute_list->value + i))->attribute_id, msg_ptr,calEncStrLen);
            msg_ptr += csrBtPhdcMderEncodeAttributeValue(&((AVA_Type *)(attribute_list->value + i))->attribute_value, msg_ptr, ((AVA_Type *)(attribute_list->value + i))->attribute_id,calEncStrLen);
        }
        else
        {
            /* Vendor specific information not supported 
            msg_ptr += csrBtPhdcMderEncodeMdsObjAttrVS((AVA_Type *)(attribute_list->value + i), msg_ptr);*/
        }
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - attribute_list_len_ptr - NO_OF_BYTES_FOR_LEN), attribute_list_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - attribute_list_start);
}

/* Vendor specific information not supported */
#if 0
static CsrUint16 csrBtPhdcMderEncodeMdsObjAttrVS(const AVA_Type *const attrValType, CsrUint8 *msg_ptr)
{
    MdsVSAttributeInfo *pMdsVSAttrInfo = NULL;
    CsrUint8 *avaType_value_start = msg_ptr;
    CsrUint8 *avaType_value_len_ptr = NULL;

    pMdsVSAttrInfo = (MdsVSAttributeInfo *)attrValType->attribute_value.value;

    /* Fill Attribute Id */
    msg_ptr += csrBtPhdcMderEncodeInt16(pMdsVSAttrInfo->attrId, msg_ptr);

    /* Reserve space for length */
    avaType_value_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    /* Fill Vendor specific attribute information */
    msg_ptr += csrBtPhdcMderEncodeVendorSpecificData(&pMdsVSAttrInfo->dataFrmt, pMdsVSAttrInfo->data, msg_ptr);

    /* Fill Length */
    csrBtPhdcMderEncodeInt16((msg_ptr - avaType_value_len_ptr - NO_OF_BYTES_FOR_LEN), avaType_value_len_ptr);

    return (CsrUint16)(msg_ptr - avaType_value_start);
}
#endif
/* Vendor specific information not supported */

static CsrUint16 csrBtPhdcMderEncodeAttributeValue(const Any *const attribute_value, CsrUint8 *msg_ptr, OID_Type attribute_id,CsrBool calEncStrLen)
{
    CsrUint8 tempCount;
    CsrUint8 *attribute_value_start = msg_ptr;
    CsrUint8 *attribute_value_len_ptr = msg_ptr;
    

    msg_ptr += NO_OF_BYTES_FOR_LEN;
 
    switch(attribute_id)
    {
        case MDC_ATTR_ID_TYPE:
        {
            msg_ptr += csrBtPhdcMderEncodeTYPE((TYPE *)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_METRIC_SPEC_SMALL:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((MetricSpecSmall *)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_METRIC_STRUCT_SMALL:
        {
            msg_ptr += csrBtPhdcMderEncodeMetricStructSmall((MetricStructureSmall*)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_ID_PHYSIO_LIST:
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        case MDC_ATTR_ID_MODEL:
/*        case MDC_ATTR_ID_LABEL_STRING:
        case MDC_ATTR_UNIT_LABEL_STRING:
        case MDC_ATTR_SYS_ID:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_STR: */
        case MDC_ATTR_SYS_TYPE_SPEC_LIST:
        case MDC_ATTR_SUPPLEMENTAL_TYPES:
        case MDC_ATTR_REG_CERT_DATA_LIST:
        case MDC_ATTR_MDS_TIME_INFO: 
        case MDC_ATTR_TIME_ABS_ADJUST:
        case MDC_ATTR_DEV_CONFIG_ID:
        {
            for(tempCount=0; tempCount < attribute_value->length; tempCount++)
            {
                msg_ptr += csrBtPhdcMderEncodeInt8(attribute_value->value[tempCount],msg_ptr,calEncStrLen);
            }
            /*msg_ptr += csrBtPhdcMderEncodeMetricIdList(attribute_value->value, msg_ptr,calEncStrLen);*/
            break;
        }
        case MDC_ATTR_UNIT_CODE:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((CsrUint16 *)attribute_value->value), msg_ptr,calEncStrLen); 
            break;
        }
        /*
        case MDC_ATTR_ATTRIBUTE_VAL_MAP:
        {
            msg_ptr += csrBtPhdcMderEncodeAttrValMap((AttrValMap *)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }*/
        case MDC_ATTR_TIME_ABS:
        {
            msg_ptr += csrBtPhdcMderEncodeAbsoluteTime((AbsoluteTime *)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }/*
        case MDC_ATTR_ID_MODEL:
        {
            msg_ptr += csrBtPhdcMderEncodeSystemModel((SystemModel *)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_DEV_CONFIG_ID:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((ConfigId *)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }*/
        case MDC_ATTR_MSMT_STAT:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((MeasurementStatus*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_ID_PHYSIO:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((OID_Type*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_METRIC_ID_PART:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((NomPartition*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_ID_LABEL_STRING:
        case MDC_ATTR_UNIT_LABEL_STRING:
        case MDC_ATTR_SYS_ID:
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_STR:
        {
            msg_ptr += csrBtPhdcMderEncodeOctectString(((octet_string*)attribute_value->value)->length,((octet_string*)attribute_value->value)->value, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_SOURCE_HANDLE_REF:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((CsrUint16 *)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_TIME_REL:
        {
            msg_ptr += csrBtPhdcMderEncodeInt32(*((RelativeTime*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }/*
        case MDC_ATTR_SYS_TYPE_SPEC_LIST:
        {
            msg_ptr += csrBtPhdcMderEncodeTypeVerList((TypeVerList *)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }*/
        case MDC_ATTR_ID_PROD_SPECN:
        {
            msg_ptr += csrBtPhdcMderEncodeProductionSpec((ProductionSpec *)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_TIME_REL_HI_RES:
        {
            CsrUint16 count = 0;
            HighResRelativeTime *pHighResRelTime = (HighResRelativeTime*)attribute_value->value;

            for(count = 0; count < 8; ++ count)
            {
                msg_ptr += csrBtPhdcMderEncodeInt8(pHighResRelTime->value[count], msg_ptr,calEncStrLen);
            }

            break;
        }
        case MDC_ATTR_TIME_PD_MSMT_ACTIVE:
        {
            msg_ptr += csrBtPhdcMderEncodeInt32(*((FLOAT_Type*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_NU_ACCUR_MSMT:
        {
            msg_ptr += csrBtPhdcMderEncodeInt32(*((FLOAT_Type*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_POWER_STAT:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((PowerStatus*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_VAL_BATT_CHARGE:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((CsrUint16*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_TIME_BATT_REMAIN:
        {
            BatMeasure *pBatMeas = (BatMeasure *)attribute_value->value;

            msg_ptr += csrBtPhdcMderEncodeInt32(pBatMeas->value, msg_ptr,calEncStrLen);
            msg_ptr += csrBtPhdcMderEncodeInt16(pBatMeas->unit, msg_ptr,calEncStrLen);
            break;
        }/*
        case MDC_ATTR_REG_CERT_DATA_LIST:
        {
            msg_ptr += csrBtPhdcMderEncodeRegCertDataList((RegCertDataList*)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }*/
        case MDC_ATTR_CONFIRM_TIMEOUT:
        {
            msg_ptr += csrBtPhdcMderEncodeInt32(*((RelativeTime*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }/*
        case MDC_ATTR_MDS_TIME_INFO:
        {
            msg_ptr += csrBtPhdcMderEncodeMdsTimeInfo((MdsTimeInfo*)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }*/
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_OID:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((OID_Type*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_ENUM_OBS_VAL_SIMP_BIT_STR:
        {
            msg_ptr += csrBtPhdcMderEncodeInt32(*((CsrUint32*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_ENUM_OBS_VAL_BASIC_BIT_STR:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((CsrUint16 *)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_VAL_ENUM_OBS:
        {
            msg_ptr += csrBtPhdcMderEncodeEnumObsVal((EnumObsValue*)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_ATTR_ENUM_OBS_VAL_PART:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((NomPartition*)attribute_value->value), msg_ptr,calEncStrLen);
            break;
        }/*
        case MDC_ATTR_TIME_ABS_ADJUST:
        {
            msg_ptr += csrBtPhdcMderEncodeAbsoluteTimeAdjust((AbsoluteTimeAdjust*)attribute_value->value, msg_ptr,calEncStrLen);
            break;
        }*/
        default:
        {
            
        }
    }

   csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - attribute_value_len_ptr - NO_OF_BYTES_FOR_LEN), attribute_value_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - attribute_value_start);
}

static CsrUint16 csrBtPhdcMderEncodeScanReportInfoFixed(const ScanReportInfoFixed *const scan_report_info_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *scan_reportInfo_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_info_fixed->data_req_id, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_info_fixed->scan_report_no, msg_ptr,calEncStrLen);

    msg_ptr += csrBtPhdcMderEncodeObservationScanFixedList((ObservationScanFixedList *)&scan_report_info_fixed->obs_scan_fixed, msg_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - scan_reportInfo_start);
}

static CsrUint16 csrBtPhdcMderEncodeScanReportInfoVar(const ScanReportInfoVar *const scan_report_info_var, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *scan_reportInfo_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_info_var->data_req_id, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_info_var->scan_report_no, msg_ptr,calEncStrLen);

    msg_ptr += csrBtPhdcMderEncodeObservationScanList((ObservationScanList *)&scan_report_info_var->obs_scan_var, msg_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - scan_reportInfo_start);
}


static CsrUint16 csrBtPhdcMderEncodeScanReportInfoMPFixed(const ScanReportInfoMPFixed *const scan_report_info_mp_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *scan_reportInfoMP_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_info_mp_fixed->data_req_id, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_info_mp_fixed->scan_report_no, msg_ptr,calEncStrLen);

    msg_ptr += csrBtPhdcMderEncodeScanReportPerFixedList(&scan_report_info_mp_fixed->scan_per_fixed, msg_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - scan_reportInfoMP_start);      
}

static CsrUint16 csrBtPhdcMderEncodeScanReportPerFixedList(const ScanReportPerFixedList *const scan_report_per_fixed_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *scan_report_per_fixed_list_start = msg_ptr;
    CsrUint16  counter;
    CsrUint8 *scan_report_per_fixed_list_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_per_fixed_list->count, msg_ptr,calEncStrLen);

    scan_report_per_fixed_list_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    for(counter = 0; counter < scan_report_per_fixed_list->count; counter ++)
    {
        msg_ptr += csrBtPhdcMderEncodeScanReportPerFixed((scan_report_per_fixed_list->value + counter), msg_ptr,calEncStrLen);
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - scan_report_per_fixed_list_len_ptr - NO_OF_BYTES_FOR_LEN), scan_report_per_fixed_list_len_ptr,calEncStrLen);
    return (CsrUint16)(msg_ptr - scan_report_per_fixed_list_start);      

}

static CsrUint16 csrBtPhdcMderEncodeScanReportPerFixed(const ScanReportPerFixed *const scan_report_per_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *scan_reportPerFixed_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(scan_report_per_fixed->person_id, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeObservationScanFixedList(&scan_report_per_fixed->obs_scan_fix, msg_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - scan_reportPerFixed_start);
}

static CsrUint16 csrBtPhdcMderEncodeObservationScanFixedList(const ObservationScanFixedList *const observation_scan_fixed_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *observation_scan_fixed_list_start = msg_ptr;
    CsrUint8 *observation_scan_fixed_list_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(observation_scan_fixed_list->count, msg_ptr,calEncStrLen);

    observation_scan_fixed_list_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;
    
    for(i = 0;i < observation_scan_fixed_list->count; i++)
    {
        msg_ptr += csrBtPhdcMderEncodeObservationScanFixed((observation_scan_fixed_list->value + i), msg_ptr,calEncStrLen);
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - observation_scan_fixed_list_len_ptr - NO_OF_BYTES_FOR_LEN), observation_scan_fixed_list_len_ptr,calEncStrLen);
    return (CsrUint16)(msg_ptr - observation_scan_fixed_list_start);
}

static CsrUint16 csrBtPhdcMderEncodeObservationScanList(const ObservationScanList *const observation_scan_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *observation_scan_list_start = msg_ptr;
    CsrUint8 *observation_scan_list_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(observation_scan_list->count, msg_ptr,calEncStrLen);

    observation_scan_list_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;
    
    for(i = 0;i < observation_scan_list->count; i++)
    {
        msg_ptr += csrBtPhdcMderEncodeObservationScan((observation_scan_list->value + i), msg_ptr,calEncStrLen);
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - observation_scan_list_len_ptr - NO_OF_BYTES_FOR_LEN), observation_scan_list_len_ptr,calEncStrLen);
    return (CsrUint16)(msg_ptr - observation_scan_list_start);
}


static CsrUint16 csrBtPhdcMderEncodeObservationScanFixed(const ObservationScanFixed *const observation_scan_fixed, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 tempCount;
    CsrUint8                               *observation_scan_fixed_start = msg_ptr;


    msg_ptr += csrBtPhdcMderEncodeInt16(observation_scan_fixed->obj_handle, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt16(observation_scan_fixed->obs_val_data.length, msg_ptr,calEncStrLen);    

    for(tempCount=0;tempCount<observation_scan_fixed->obs_val_data.length;tempCount++)
    {
        msg_ptr += csrBtPhdcMderEncodeInt8(observation_scan_fixed->obs_val_data.value[tempCount],
            msg_ptr,calEncStrLen);            
    }


    return (CsrUint16)(msg_ptr - observation_scan_fixed_start);      
}

static CsrUint16 csrBtPhdcMderEncodeObservationScan(const ObservationScan *const observation_scan, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8                               *observation_scan_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(observation_scan->obj_handle, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeAttributeList(&observation_scan->attributes, msg_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - observation_scan_start);
}

static CsrUint16 csrBtPhdcMderEncodeAbsoluteTime(const AbsoluteTime *const absTime, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *msgStart = msg_ptr;
    
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->century, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->year, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->month, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->day, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->hour, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->minute, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->second, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(absTime->sec_fractions, msg_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - msgStart);
}

static CsrUint16 csrBtPhdcMderEncodeEnumObsVal(const EnumObsValue *const enum_obs_val, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *enum_obs_val_start = msg_ptr;
    CsrUint8 *enum_obs_val_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(enum_obs_val->metric_id, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt16(enum_obs_val->state, msg_ptr,calEncStrLen);

    msg_ptr += csrBtPhdcMderEncodeInt16(enum_obs_val->value.choice, msg_ptr,calEncStrLen);

    enum_obs_val_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    switch(enum_obs_val->value.choice)
    {
        case OBJ_ID_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(enum_obs_val->value.u.enum_obj_id, msg_ptr,calEncStrLen);
            break;
        }
        case TEXT_STRING_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeOctectString(enum_obs_val->value.u.enum_text_string.length, enum_obs_val->value.u.enum_text_string.value, msg_ptr,calEncStrLen);
            break;
        }
        case BIT_STR_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeInt32(enum_obs_val->value.u.enum_bit_str, msg_ptr,calEncStrLen);
            break;
        }
        default:
        {
            
        }
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - enum_obs_val_len_ptr - NO_OF_BYTES_FOR_LEN), enum_obs_val_len_ptr,calEncStrLen);
    return (CsrUint16)(msg_ptr - enum_obs_val_start);

}

static CsrUint16 csrBtPhdcMderEncodeProductionSpec(const ProductionSpec *const production_spec, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *production_spec_start = msg_ptr;
    CsrUint8 *production_spec_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(production_spec->count, msg_ptr,calEncStrLen);

    production_spec_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    for(i = 0; i < production_spec->count; i++)
    {
        msg_ptr += csrBtPhdcMderEncodeInt16(((ProdSpecEntry *)(production_spec->value + i))->spec_type, msg_ptr,calEncStrLen);
        msg_ptr += csrBtPhdcMderEncodeInt16(((ProdSpecEntry *)(production_spec->value + i))->component_id, msg_ptr,calEncStrLen);
        msg_ptr += csrBtPhdcMderEncodeOctectString(((ProdSpecEntry *)(production_spec->value + i))->prod_spec.length, ((ProdSpecEntry *)(production_spec->value + i))->prod_spec.value, msg_ptr,calEncStrLen);
    }   

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - production_spec_len_ptr - NO_OF_BYTES_FOR_LEN), production_spec_len_ptr,calEncStrLen);
    return (CsrUint16)(msg_ptr - production_spec_start);
}

static CsrUint16 csrBtPhdcMderEncodeTYPE(const TYPE *const type, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *type_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(type->partition, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt16(type->code, msg_ptr,calEncStrLen);   
      
    return (CsrUint16)(msg_ptr - type_start);
}

static CsrUint16 csrBtPhdcMderEncodeMetricStructSmall(const MetricStructureSmall *const metStructSmall, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *mss_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt8(metStructSmall->ms_struct, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt8(metStructSmall->ms_comp_no, msg_ptr,calEncStrLen);   
      
    return (CsrUint16)(msg_ptr - mss_start);
}

CsrUint16 csrBtPhdcMderEncodeAareApdu(const AARE_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *apdu_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderEncodeInt16(apdu->result, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeDataProto(&apdu->selected_data_proto, msg_ptr,calEncStrLen);    

   return (CsrUint16)(msg_ptr - apdu_start);
}

CsrUint16 csrBtPhdcMderEncodeRlrqApdu(const RLRQ_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *apdu_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderEncodeInt16(apdu->reason, msg_ptr,calEncStrLen);

   return (CsrUint16)(msg_ptr - apdu_start);
}

CsrUint16 csrBtPhdcMderEncodeRlreApdu(const RLRE_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *apdu_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderEncodeInt16((CsrUint16)apdu->reason, msg_ptr,calEncStrLen);

   return (CsrUint16)(msg_ptr - apdu_start);
}   

CsrUint16 csrBtPhdcMderEncodeAbrtApdu(const ABRT_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *apdu_start = msg_ptr;
   
  
   msg_ptr += csrBtPhdcMderEncodeInt16((CsrUint16)apdu->reason, msg_ptr,calEncStrLen);

   return (CsrUint16)(msg_ptr - apdu_start);
}   

CsrUint16 csrBtPhdcMderEncodePrstApdu(const PRST_apdu *const apdu, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *apdu_start = msg_ptr;
    CsrUint8 *apdu_len_ptr;

    apdu_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    msg_ptr += csrBtPhdcMderEncodeInt16(((DATA_apdu*)apdu->value)->invoke_id, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeReportChoice((DATA_apdu*)apdu->value, msg_ptr,calEncStrLen);

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - apdu_len_ptr - NO_OF_BYTES_FOR_LEN), apdu_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - apdu_start);
}

static CsrUint16 csrBtPhdcMderEncodeReportChoice(const DATA_apdu *const report_choice, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *report_choice_start = msg_ptr;
    CsrUint8 *report_choice_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(report_choice->choice.choice, msg_ptr,calEncStrLen);
    
    report_choice_len_ptr  = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    switch(report_choice->choice.choice)
    {
        case ROIV_CMIP_EVENT_REPORT_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeEventReportArgumentSimple(&report_choice->choice.u.roiv_cmipEventReport, msg_ptr,calEncStrLen);
            break;
        }
        case ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeEventReportArgumentSimple(&report_choice->choice.u.roiv_cmipConfirmedEventReport, msg_ptr,calEncStrLen);
            break;
        }
        case ROIV_CMIP_GET_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeGetArgumentSimple(&report_choice->choice.u.roiv_cmipGet, msg_ptr,calEncStrLen);
            break;
        }
        case ROIV_CMIP_SET_CHOSEN:
        case ROIV_CMIP_CONFIRMED_SET_CHOSEN:
        case ROIV_CMIP_ACTION_CHOSEN:
        case ROIV_CMIP_CONFIRMED_ACTION_CHOSEN:
        {
            
            break;
        }
        case RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeEventReportResultSimple(&report_choice->choice.u.rors_cmipConfirmedEventReport, msg_ptr,calEncStrLen);
            break;
        }
        case RORS_CMIP_GET_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeGetResultSimple(&report_choice->choice.u.rors_cmipGet, msg_ptr,calEncStrLen);
            break;
        }
        case RORS_CMIP_CONFIRMED_SET_CHOSEN:
        case RORS_CMIP_CONFIRMED_ACTION_CHOSEN:
        {
            
            break;
        }
        case ROER_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeErrorResult(&report_choice->choice.u.roer, msg_ptr,calEncStrLen);
            break;
        }
        case RORJ_CHOSEN:
        {
            msg_ptr += csrBtPhdcMderEncodeInt16((CsrUint16)report_choice->choice.u.rorj.problem, msg_ptr,calEncStrLen);
            break;
        }
        default:
        {
            
            break;
        }
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - report_choice_len_ptr - NO_OF_BYTES_FOR_LEN), report_choice_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - report_choice_start);   
}

static CsrUint16 csrBtPhdcMderEncodeGetResultSimple(const GetResultSimple *const get_result, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *get_result_start = msg_ptr;
   

   msg_ptr += csrBtPhdcMderEncodeInt16(get_result->obj_handle, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeAttributeList(&get_result->attribute_list, msg_ptr,calEncStrLen);

   return (CsrUint16)(msg_ptr - get_result_start);
}

static CsrUint16 csrBtPhdcMderEncodeEventReportResultSimple(const EventReportResultSimple *const event_report, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *event_report_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderEncodeInt16(event_report->obj_handle, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeInt32(event_report->currentTime, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeInt16(event_report->event_type, msg_ptr,calEncStrLen);
   
   if(event_report->event_type == MDC_NOTI_CONFIG)
   {
       msg_ptr += csrBtPhdcMderEncodeInt16(event_report->event_reply_info.length, msg_ptr,calEncStrLen);
       msg_ptr += csrBtPhdcMderEncodeMdcNotiConfigRsp((ConfigReportRsp *)event_report->event_reply_info.value, msg_ptr,calEncStrLen);
   }
   else
   {
       msg_ptr += csrBtPhdcMderEncodeInt16(0, msg_ptr,calEncStrLen);
   }
     
   return (CsrUint16)(msg_ptr - event_report_start);   
}

static CsrUint16 csrBtPhdcMderEncodeMdcNotiConfigRsp(const ConfigReportRsp *const config_report_rsp, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *config_report_rsp_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderEncodeInt16(config_report_rsp->config_report_id, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeInt16(config_report_rsp->config_result, msg_ptr,calEncStrLen);
   
   return (CsrUint16)(msg_ptr - config_report_rsp_start);      
}

static CsrUint16 csrBtPhdcMderEncodeEventReportArgumentSimple(const EventReportArgumentSimple *event_report, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *event_report_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(event_report->obj_handle, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt32(event_report->event_time, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeInt16(event_report->event_type, msg_ptr,calEncStrLen);

    switch(event_report->event_type)
    {
        case MDC_NOTI_CONFIG:
        {
            msg_ptr += csrBtPhdcMderEncodeMdcNotiConfig(&event_report->event_info, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_NOTI_SCAN_REPORT_FIXED:
        {
            msg_ptr += csrBtPhdcMderEncodeMdcNotiScanReportFixed(&event_report->event_info, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_NOTI_SCAN_REPORT_MP_FIXED:
        {
            msg_ptr += csrBtPhdcMderEncodeMdcNotiScanReportMPFixed(&event_report->event_info, msg_ptr,calEncStrLen);
            break;
        }
        case MDC_NOTI_SCAN_REPORT_VAR:
        {
            msg_ptr += csrBtPhdcMderEncodeMdcNotiScanReportVar(&event_report->event_info, msg_ptr,calEncStrLen);
            break;
        }
        default:
        {
            
        }
    }

    return (CsrUint16)(msg_ptr - event_report_start);   
}

static CsrUint16 csrBtPhdcMderEncodeMdcNotiConfig(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *mdc_notiConfig_start = msg_ptr;
    CsrUint8 *mdc_notiConfig_len_ptr;

    mdc_notiConfig_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    msg_ptr += csrBtPhdcMderEncodeInt16(((ConfigReport *)event_info->value)->config_report_id, msg_ptr,calEncStrLen);
    msg_ptr += csrBtPhdcMderEncodeConfigObjectList(&((ConfigReport *)event_info->value)->config_obj_list, msg_ptr,calEncStrLen);

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - mdc_notiConfig_len_ptr - NO_OF_BYTES_FOR_LEN), mdc_notiConfig_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - mdc_notiConfig_start);      
}

static CsrUint16 csrBtPhdcMderEncodeMdcNotiScanReportFixed(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *mdc_notiScanReport_start = msg_ptr;
    CsrUint8 *mdc_notiScanReport_len_ptr;

    mdc_notiScanReport_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;
    
    msg_ptr += csrBtPhdcMderEncodeScanReportInfoFixed((ScanReportInfoFixed *)event_info->value, msg_ptr,calEncStrLen);

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - mdc_notiScanReport_len_ptr - NO_OF_BYTES_FOR_LEN), mdc_notiScanReport_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - mdc_notiScanReport_start);
}

static CsrUint16 csrBtPhdcMderEncodeMdcNotiScanReportVar(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *mdc_notiScanReport_start = msg_ptr;
    CsrUint8 *mdc_notiScanReport_len_ptr;

    mdc_notiScanReport_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;
    
    msg_ptr += csrBtPhdcMderEncodeScanReportInfoVar((ScanReportInfoVar*)event_info->value, msg_ptr,calEncStrLen);

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - mdc_notiScanReport_len_ptr - NO_OF_BYTES_FOR_LEN), mdc_notiScanReport_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - mdc_notiScanReport_start);
}


static CsrUint16 csrBtPhdcMderEncodeMdcNotiScanReportMPFixed(const Any *const event_info, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *mdc_notiScanReportMP_start = msg_ptr;
    CsrUint8 *mdc_notiScanReportMP_len_ptr;

    mdc_notiScanReportMP_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;
    
    msg_ptr += csrBtPhdcMderEncodeScanReportInfoMPFixed((ScanReportInfoMPFixed *)event_info->value, msg_ptr,calEncStrLen);

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - mdc_notiScanReportMP_len_ptr - NO_OF_BYTES_FOR_LEN), mdc_notiScanReportMP_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - mdc_notiScanReportMP_start);
}

static CsrUint16 csrBtPhdcMderEncodeConfigObjectList(const ConfigObjectList *const config_object_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *config_object_list_start = msg_ptr;
    CsrUint8 *config_object_list_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(config_object_list->count, msg_ptr,calEncStrLen);

    config_object_list_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    for(i = 0; i < config_object_list->count; i++)
    {
        msg_ptr += csrBtPhdcMderEncodeInt16(((ConfigObject *)(config_object_list->value + i))->obj_class, msg_ptr,calEncStrLen);
        msg_ptr += csrBtPhdcMderEncodeInt16(((ConfigObject *)(config_object_list->value + i))->obj_handle, msg_ptr,calEncStrLen);
        msg_ptr += csrBtPhdcMderEncodeAttributeList(&((ConfigObject *)(config_object_list->value + i))->attributes, msg_ptr,calEncStrLen);
    }

    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - config_object_list_len_ptr - NO_OF_BYTES_FOR_LEN), config_object_list_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - config_object_list_start);
}

static CsrUint16 csrBtPhdcMderEncodeGetArgumentSimple(const GetArgumentSimple *event_report, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
   CsrUint8 *event_report_start = msg_ptr;
   
   msg_ptr += csrBtPhdcMderEncodeInt16(event_report->obj_handle, msg_ptr,calEncStrLen);
   msg_ptr += csrBtPhdcMderEncodeAttributeIdList(&event_report->attribute_id_list, msg_ptr,calEncStrLen);  

   return (CsrUint16)(msg_ptr - event_report_start);
}


static CsrUint16 csrBtPhdcMderEncodeErrorResult(const ErrorResult *const err_result, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint8 *error_result_start = msg_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(err_result->error_value, msg_ptr,calEncStrLen);

    if(err_result->parameter.length > 0)
    {/*
        PANIC_FALSE((err_result->error_value == NOT_ALLOWED_BY_OBJECT) ||
                    (err_result->error_value == ACTION_TIMED_OUT) ||
                    (err_result->error_value == ACTION_ABORTED));

        PANIC_FALSE(err_result->parameter.length == 2);*/

        msg_ptr += csrBtPhdcMderEncodeInt16(err_result->parameter.length, msg_ptr,calEncStrLen); 
        msg_ptr += csrBtPhdcMderEncodeInt16(*((CsrUint16 *)err_result->parameter.value), msg_ptr,calEncStrLen); 
    }
    else
    {
        /* OID_Type not present. Length = 0 */
        msg_ptr += csrBtPhdcMderEncodeInt16(err_result->parameter.length, msg_ptr,calEncStrLen);
    }

    return (CsrUint16)(msg_ptr - error_result_start);
}

static CsrUint16 csrBtPhdcMderEncodeAttributeIdList (const AttributeIdList *const attribute_id_list, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16 i;
    CsrUint8 *attribute_id_list_start = msg_ptr;
    CsrUint8 *attribute_id_list_len_ptr;

    msg_ptr += csrBtPhdcMderEncodeInt16(attribute_id_list->count, msg_ptr,calEncStrLen);

    attribute_id_list_len_ptr = msg_ptr;
    msg_ptr += NO_OF_BYTES_FOR_LEN;

    for(i = 0; i < attribute_id_list->count; i++)
    {
        msg_ptr += csrBtPhdcMderEncodeInt16(*((CsrUint16 *)(attribute_id_list->value + i)), msg_ptr,calEncStrLen);
    }
    
    csrBtPhdcMderEncodeInt16((CsrUint16)(msg_ptr - attribute_id_list_len_ptr - NO_OF_BYTES_FOR_LEN), attribute_id_list_len_ptr,calEncStrLen);

    return (CsrUint16)(msg_ptr - attribute_id_list_start);   
}

/* USER DEFINED ASN ENCODE FUNCTIONS */
/* Vendor specific information not supported */
#if 0
static CsrUint16 csrBtPhdcMderEncodeVendorSpecificData(AttrDataFormat *pAttrDataFrmt, void *pMeasData, CsrUint8 *msg_ptr,CsrBool calEncStrLen)
{
    CsrUint16  count;
    CsrUint8 *vendor_specific_data_start = msg_ptr;


    for(count = 0; count < pAttrDataFrmt->data_count; ++count)
    {
        if(pAttrDataFrmt->data_value[count] & ATTR_DATA_FORMAT_UINT8)
        {
            msg_ptr += csrBtPhdcMderEncodeInt8(*((CsrUint8 *)pMeasData), msg_ptr,calEncStrLen);
            pMeasData = (void *)((CsrUint8 *)pMeasData + sizeof(CsrUint8));
        }
        else if(pAttrDataFrmt->data_value[count] & ATTR_DATA_FORMAT_UINT16)
        {
            msg_ptr += csrBtPhdcMderEncodeInt16(*((CsrUint16 *)pMeasData), msg_ptr,calEncStrLen);
            pMeasData = (void *)((CsrUint8 *)pMeasData + sizeof(CsrUint16));
        }
        else if(pAttrDataFrmt->data_value[count] & ATTR_DATA_FORMAT_UINT32)
        {
            msg_ptr += csrBtPhdcMderEncodeInt32(*((CsrUint32 *)pMeasData), msg_ptr,calEncStrLen);
            pMeasData = (void *)((CsrUint8 *)pMeasData + sizeof(CsrUint32));
        }
        else if(pAttrDataFrmt->data_value[count] & ATTR_DATA_FORMAT_STRING)
        {
            CsrUint16   strLen = *((CsrUint16 *)pMeasData);
            pMeasData = (void *)((CsrUint8 *)pMeasData + sizeof(CsrUint16));
            msg_ptr += csrBtPhdcMderEncodeOctectString(strLen, (CsrUint8 *)pMeasData, msg_ptr,calEncStrLen);
            pMeasData = (void *)((CsrUint8 *)pMeasData + (sizeof(CsrUint8) * strLen));
        }
        else
        {
            
        }
    }

    return (CsrUint16)(msg_ptr - vendor_specific_data_start);
}
#endif
/* Vendor specific information not supported */

#endif

