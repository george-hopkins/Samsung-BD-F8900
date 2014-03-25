#ifndef CSR_BT_PHDC_IEEE_ASN_H__
#define CSR_BT_PHDC_IEEE_ASN_H__

#include "csr_synergy.h"
/****************************************************************************

(c) Cambridge Silicon Radio Limited 2009

All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
/* IEEE Headers */
#include "csr_bt_phdc_ieee_asn_config_if.h"

#define AARQ_CHOSEN 0xE200
#define AARE_CHOSEN 0xE300
#define RLRQ_CHOSEN 0xE400
#define RLRE_CHOSEN 0xE500
#define ABRT_CHOSEN 0xE600
#define PRST_CHOSEN 0xE700

#define ANY_SIZE 3

/*
The following typedefs may need to be changed depending on the
Compiler and machine architecture.
*/

typedef CsrUint16 PrivateOid;
typedef CsrUint16 InstNumber;

typedef struct AVA_Type
{
    OID_Type attribute_id;
    Any attribute_value;
} AVA_Type;

typedef struct AttributeList
{
    CsrUint16 count;
    CsrUint16 length;
    AVA_Type *value;
} AttributeList;

typedef CsrUint16 OperationalState;

#define OS_DISABLED 0
#define OS_ENABLED 1
#define OS_NOT_AVAILABLE 2

typedef struct SystemModel
{
    octet_string manufacturer;
    octet_string model_number;
} SystemModel;

#define UNSPECIFIED 0
#define SERIAL_NUMBER 1
#define PART_NUMBER 2
#define HW_REVISION 3
#define SW_REVISION 4
#define FW_REVISION 5
#define PROTOCOL_REVISION 6
#define PROD_SPEC_GMDN 7

typedef struct ProdSpecEntry
{
    CsrUint16 spec_type;
    PrivateOid component_id;
    octet_string prod_spec;
} ProdSpecEntry;

typedef struct ProductionSpec
{
    CsrUint16 count;
    CsrUint16 length;
    ProdSpecEntry *value; /* first element of the array */
} ProductionSpec;

typedef CsrUint16 PowerStatus;

#define ON_MAINS 0x8000
#define ON_BATTERY 0x4000
#define CHARGING_FULL 0x0080
#define CHARGING_TRICKLE 0x0040
#define CHARGING_OFF 0x0020

typedef struct BatMeasure
{
    FLOAT_Type value;
    OID_Type unit;
} BatMeasure;

typedef struct SampleType
{
    CsrUint8 sample_size;
    CsrUint8 significant_bits;
} SampleType;

#define SAMPLE_TYPE_SIGNIFICANT_BITS_SIGNED_SAMPLES 255

typedef CsrUint16 SaFlags;

#define SMOOTH_CURVE 0x8000
#define DELAYED_CURVE 0x4000
#define STATIC_SCALE 0x2000
#define SA_EXT_VAL_RANGE 0x1000

typedef struct SaSpec
{
    CsrUint16 array_size;
    SampleType sample_type;
    SaFlags flags;
} SaSpec;

typedef struct ScaleRangeSpec8
{
    FLOAT_Type lower_absolute_value;
    FLOAT_Type upper_absolute_value;
    CsrUint8 lower_scaled_value;
    CsrUint8 upper_scaled_value;
} ScaleRangeSpec8;

typedef struct ScaleRangeSpec16
{
    FLOAT_Type lower_absolute_value;
    FLOAT_Type upper_absolute_value;
    CsrUint16 lower_scaled_value;
    CsrUint16 upper_scaled_value;
} ScaleRangeSpec16;

typedef struct ScaleRangeSpec32
{
    FLOAT_Type lower_absolute_value;
    FLOAT_Type upper_absolute_value;
    CsrUint32 lower_scaled_value;
    CsrUint32 upper_scaled_value;
} ScaleRangeSpec32;

typedef struct HandleAttrValMapEntry
{
    HANDLE obj_handle;
    AttrValMap attr_val_map;
} HandleAttrValMapEntry;

typedef CsrUint16 ConfirmMode;

#define UNCONFIRMED 0x0000
#define CONFIRMED 0x0001

typedef struct HandleAttrValMap
{
    CsrUint16 count;
    CsrUint16 length;
    HandleAttrValMapEntry *value; /* first element of the array */
} HandleAttrValMap;

typedef CsrUint16 StoSampleAlg;

#define ST_ALG_NOS 0x0000
#define ST_ALG_MOVING_AVERAGE 0x0001
#define ST_ALG_RECURSIVE_ 0x0002
#define ST_ALG_MIN_PICK 0x0003
#define ST_ALG_MAX_PICK 0x0004
#define ST_ALG_MEDIAN 0x0005
#define ST_ALG_TRENDED 0x0200
#define ST_ALG_NO_DOWNSAMPLING 0x0400

typedef struct SetTimeInvoke
{
    AbsoluteTime date_time;
    FLOAT_Type accuracy;
} SetTimeInvoke;

typedef struct SegmIdList
{
    CsrUint16 count;
    CsrUint16 length;
    InstNumber *value; /* first element of the array */
} SegmIdList;

typedef struct AbsTimeRange
{
    AbsoluteTime from_time;
    AbsoluteTime to_time;
} AbsTimeRange;

typedef struct SegmentInfo
{
    InstNumber seg_inst_no;
    AttributeList seg_info;
} SegmentInfo;

typedef struct SegmentInfoList
{
    CsrUint16 count;
    CsrUint16 length;
    SegmentInfo *value; /* first element of the array */
} SegmentInfoList;


#define ALL_SEGMENTS_CHOSEN 0x0001
#define SEGM_ID_LIST_CHOSEN 0x0002
#define ABS_TIME_RANGE_CHOSEN 0x0003

typedef struct SegmSelection
{
    CsrUint16 choice;
    CsrUint16 length;
    union
    {
        CsrUint16 all_segments;
        SegmIdList segm_id_list;
        AbsTimeRange abs_time_range;
    } u;
} SegmSelection;

typedef CsrUint16 PMStoreCapab;

#define PMSC_VAR_NO_OF_SEGM 0x8000
#define PMSC_EPI_SEG_ENTRIES 0x0800
#define PMSC_PERI_SEG_ENTRIES 0x0400
#define PMSC_ABS_TIME_SELECT 0x0200
#define PMSC_CLEAR_SEGM_BY_LIST_SUP 0x0100
#define PMSC_CLEAR_SEGM_BY_TIME_SUP 0x0080
#define PMSC_CLEAR_SEGM_REMOVE 0x0040
#define PMSC_MULTI_PERSON 0x0008

typedef CsrUint16 SegmEntryHeader;

#define SEG_ELEM_HDR_ABSOLUTE_TIME 0x8000
#define SEG_ELEM_HDR_RELATIVE_TIME 0x4000
#define SEG_ELEM_HDR_HIRES_RELATIVE_TIME 0x2000

typedef struct SegmEntryElem
{
    OID_Type class_id;
    TYPE metric_type;
    HANDLE handle;
    AttrValMap attr_val_map;
} SegmEntryElem;

typedef struct SegmEntryElemList
{
    CsrUint16 count;
    CsrUint16 length;
    SegmEntryElem *value; /* first element of the array */
} SegmEntryElemList;

typedef struct PmSegmentEntryMap
{
    SegmEntryHeader segm_entry_header;
    SegmEntryElemList segm_entry_elem_list;
} PmSegmentEntryMap;

typedef struct SegmElemStaticAttrEntry
{
    OID_Type class_id;
    TYPE metric_type;
    AttributeList attribute_list;
} SegmElemStaticAttrEntry;

typedef struct PmSegmElemStaticAttrList
{
    CsrUint16 count;
    CsrUint16 length;
    SegmElemStaticAttrEntry *value; /* first element of the array */
} PmSegmElemStaticAttrList;

typedef struct TrigSegmDataXferReq
{
    InstNumber seg_inst_no;
} TrigSegmDataXferReq;

typedef CsrUint16 TrigSegmXferRsp;

#define TSXR_SUCCESSFUL 0
#define TSXR_FAIL_NO_SUCH_SEGMENT 1
#define TSXR_FAIL_SEGM_TRY_LATER 2
#define TSXR_FAIL_SEGM_EMPTY 3
#define TSXR_FAIL_OTHER 512

typedef struct TrigSegmDataXferRsp
{
    InstNumber seg_inst_no;
    TrigSegmXferRsp trig_segm_xfer_rsp;
} TrigSegmDataXferRsp;

typedef CsrUint16 SegmEvtStatus;

#define SEVTSTA_FIRST_ENTRY 0x8000
#define SEVTSTA_LAST_ENTRY 0x4000
#define SEVTSTA_AGENT_ABORT 0x0800
#define SEVTSTA_MANAGER_CONFIRM 0x0080
#define SEVTSTA_MANAGER_ABORT 0x0008

typedef struct SegmDataEventDescr
{
    InstNumber segm_instance;
    CsrUint32 segm_evt_entry_index;
    CsrUint32 segm_evt_entry_count;
    SegmEvtStatus segm_evt_status;
} SegmDataEventDescr;

typedef struct SegmentDataEvent
{
    SegmDataEventDescr segm_data_event_descr;
    octet_string segm_data_event_entries;
} SegmentDataEvent;

typedef struct SegmentDataResult
{
    SegmDataEventDescr segm_data_event_descr;
} SegmentDataResult;

typedef CsrUint16 SegmStatType;

#define SEGM_STAT_TYPE_MINIMUM 1
#define SEGM_STAT_TYPE_MAXIMUM 2
#define SEGM_STAT_TYPE_AVERAGE 3

typedef struct SegmentStatisticEntry
{
    SegmStatType segm_stat_type;
    octet_string segm_stat_entry;
} SegmentStatisticEntry;

typedef struct SegmentStatistics
{
    CsrUint16 count;
    CsrUint16 length;
    SegmentStatisticEntry *value; /* first element of the array */
} SegmentStatistics;


typedef CsrUint32 AssociationVersion;

#define ASSOC_VERSION1 0x80000000

typedef CsrUint32 ProtocolVersion;

#define PROTOCOL_VERSION1   0x80000000
#define PROTOCOL_VERSION1_1 0xC0000000

typedef CsrUint16 EncodingRules;

#define MDER 0x8000
#define XER 0x4000
#define PER 0x2000

typedef struct UUID_Ident
{
    CsrUint8 value[16];
} UUID_Ident;

typedef CsrUint16 DataProtoId;

#define DATA_PROTO_ID_EMPTY     0
#define DATA_PROTO_ID_20601     20601
#define DATA_PROTO_ID_EXTERNAL  65535

typedef struct DataProto
{
    DataProtoId data_proto_id;
    Any data_proto_info;
} DataProto;

typedef struct DataProtoList
{
    CsrUint16 count;
    CsrUint16 length;
    DataProto *value;
} DataProtoList;

typedef struct AARQ_apdu
{
    AssociationVersion assoc_version;
    DataProtoList data_proto_list;
} AARQ_apdu;

typedef CsrUint16 Associate_result;

#define ACCEPTED                            0
#define REJECTED_PERMANENT                  1
#define REJECTED_TRANSIENT                  2
#define ACCEPTED_UNKNOWN_CONFIG             3
#define REJECTED_NO_COMMON_PROTOCOL         4
#define REJECTED_NO_COMMON_PARAMETER        5
#define REJECTED_UNKNOWN                    6
#define REJECTED_UNAUTHORIZED               7
#define REJECTED_UNSUPPORTED_ASSOC_VERSION  8

typedef struct AARE_apdu
{
    Associate_result result;
    DataProto selected_data_proto;
} AARE_apdu;

typedef CsrUint16 Release_request_reason;

#define RELEASE_REQUEST_REASON_NORMAL           0
#define RELEASE_REQUEST_REASON_NO_MORE_CONFIG   1
#define RELEASE_REQUEST_REASON_CONFIG_CHANGED   2

typedef struct RLRQ_apdu
{
    Release_request_reason reason;
} RLRQ_apdu;

typedef CsrUint16 Release_response_reason;

#define RELEASE_RESPONSE_REASON_NORMAL 0

typedef struct RLRE_apdu
{
    Release_response_reason reason;
} RLRE_apdu;

typedef CsrUint16 Abort_reason;

#define ABORT_REASON_UNDEFINED 0
#define ABORT_REASON_BUFFER_OVERFLOW 1
#define ABORT_REASON_RESPONSE_TIMEOUT 2
#define ABORT_REASON_CONFIGURATION_TIMEOUT 3

typedef struct ABRT_apdu
{
    Abort_reason reason;
} ABRT_apdu;

typedef octet_string PRST_apdu;

typedef CsrUint16 InvokeIDType;

typedef struct EventReportArgumentSimple
{
    HANDLE obj_handle;
    RelativeTime event_time;
    OID_Type event_type;
    Any event_info;
} EventReportArgumentSimple;

typedef struct GetArgumentSimple
{
    HANDLE obj_handle;
    AttributeIdList attribute_id_list;
} GetArgumentSimple;

typedef CsrUint16 ModifyOperator;

#define REPLACE 0
#define ADD_VALUES 1
#define REMOVE_VALUES 2
#define SET_TO_DEFAULT 3

typedef struct AttributeModEntry
{
    ModifyOperator modify_operator;
    AVA_Type attribute;
} AttributeModEntry;

typedef struct ModificationList
{
    CsrUint16 count;
    CsrUint16 length;
    AttributeModEntry *value; /* first element of the array */
} ModificationList;

typedef struct SetArgumentSimple
{
    HANDLE obj_handle;
    ModificationList modification_list;
} SetArgumentSimple;

typedef struct ActionArgumentSimple
{
    HANDLE obj_handle;
    OID_Type action_type;
    Any action_info_args;
} ActionArgumentSimple;

typedef struct EventReportResultSimple
{
    HANDLE obj_handle;
    RelativeTime currentTime;
    OID_Type event_type;
    Any event_reply_info;
} EventReportResultSimple;

typedef struct GetResultSimple
{
    HANDLE obj_handle;
    AttributeList attribute_list;
} GetResultSimple;

typedef struct SetResultSimple
{
    HANDLE obj_handle;
    AttributeList attribute_list;
} SetResultSimple;

typedef struct ActionResultSimple
{
    HANDLE obj_handle;
    OID_Type action_type;
    Any action_info_args;
} ActionResultSimple;

typedef CsrUint16 ERROR;

#define NO_SUCH_OBJECT_INSTANCE 1
#define ACCESS_DENIED 2
#define NO_SUCH_ACTION 9
#define INVALID_OBJECT_INSTANCE 17
#define PROTOCOL_VIOLATION 23
#define NOT_ALLOWED_BY_OBJECT 24
#define ACTION_TIMED_OUT 25
#define ACTION_ABORTED 26

typedef struct ErrorResult
{
    ERROR error_value;
    Any parameter;
} ErrorResult;

typedef CsrUint16 RorjProblem;

#define UNRECOGNIZED_APDU       0
#define BADLY_STRUCTURED_APDU   2
#define UNRECOGNIZED_OPERATION  101
#define RESOURCE_LIMITATION     103
#define UNEXPECTED_ERROR        303

typedef struct RejectResult
{
    RorjProblem problem;
} RejectResult;

#define ROIV_CMIP_EVENT_REPORT_CHOSEN           0x0100
#define ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN 0x0101
#define ROIV_CMIP_GET_CHOSEN                    0x0103
#define ROIV_CMIP_SET_CHOSEN                    0x0104
#define ROIV_CMIP_CONFIRMED_SET_CHOSEN          0x0105
#define ROIV_CMIP_ACTION_CHOSEN                 0x0106
#define ROIV_CMIP_CONFIRMED_ACTION_CHOSEN       0x0107
#define RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN 0x0201
#define RORS_CMIP_GET_CHOSEN                    0x0203
#define RORS_CMIP_CONFIRMED_SET_CHOSEN          0x0205
#define RORS_CMIP_CONFIRMED_ACTION_CHOSEN       0x0207
#define ROER_CHOSEN                             0x0300
#define RORJ_CHOSEN                             0x0400

typedef struct DATA_apdu
{
    InvokeIDType invoke_id;
    struct
    {
        CsrUint16 choice;
        CsrUint16 length;
        union
        {
            EventReportArgumentSimple roiv_cmipEventReport;
            EventReportArgumentSimple roiv_cmipConfirmedEventReport;
            GetArgumentSimple roiv_cmipGet;
            SetArgumentSimple roiv_cmipSet;
            SetArgumentSimple roiv_cmipConfirmedSet;
            ActionArgumentSimple roiv_cmipAction;
            ActionArgumentSimple roiv_cmipConfirmedAction;
            EventReportResultSimple rors_cmipConfirmedEventReport;
            GetResultSimple rors_cmipGet;
            SetResultSimple rors_cmipConfirmedSet;
            ActionResultSimple rors_cmipConfirmedAction;
            ErrorResult roer;
            RejectResult rorj;
        } u;
    } choice;
} DATA_apdu;

typedef struct APDU
{
    CsrUint16 choice;
    CsrUint16 length;

    union
    {
        AARQ_apdu aarq;
        AARE_apdu aare;
        RLRQ_apdu rlrq;
        RLRE_apdu rlre;
        ABRT_apdu abrt;
        PRST_apdu prst;
    } u;
} APDU;

typedef CsrUint32 NomenclatureVersion;
#define NOM_VERSION1                0x80000000
typedef CsrUint32 FunctionalUnits;
#define FUN_UNITS_UNIDIRECTIONAL    0x80000000
#define FUN_UNITS_HAVETESTCAP       0x40000000
#define FUN_UNITS_CREATETESTASSOC   0x20000000
typedef CsrUint32 SystemType;
#define SYS_TYPE_MANAGER            0x80000000
#define SYS_TYPE_AGENT              0x00800000
typedef CsrUint16 ConfigId;
#define MANAGER_CONFIG_RESPONSE     0x0000
#define STANDARD_CONFIG_START       0x0001
#define STANDARD_CONFIG_END         0x3FFF
#define EXTENDED_CONFIG_START       0x4000
#define EXTENDED_CONFIG_END         0x7FFF
#define RESERVED_START              0x8000
#define RESERVED_END                0xFFFF

typedef CsrUint16 DataReqModeFlags;

typedef struct DataReqModeCapab
{
    DataReqModeFlags    data_req_mode_flags;
    CsrUint8            data_req_init_agent_count;
    CsrUint8            data_req_init_manager_count;
} DataReqModeCapab;

#define DATA_REQ_SUPP_STOP 0x8000
#define DATA_REQ_SUPP_SCOPE_ALL 0x0800
#define DATA_REQ_SUPP_SCOPE_CLASS 0x0400
#define DATA_REQ_SUPP_SCOPE_HANDLE 0x0200
#define DATA_REQ_SUPP_MODE_SINGLE_RSP 0x0080
#define DATA_REQ_SUPP_MODE_TIME_PERIOD 0x0040
#define DATA_REQ_SUPP_MODE_TIME_NO_LIMIT 0x0020
#define DATA_REQ_SUPP_PERSON_ID 0x0010
#define DATA_REQ_SUPP_INIT_AGENT 0x0001

typedef struct PhdAssociationInformation
{
    ProtocolVersion     protocolVersion;
    EncodingRules       encodingRules;
    NomenclatureVersion nomenclatureVersion;
    FunctionalUnits     functionalUnits;
    SystemType          systemType;
    octet_string        systemId;
    CsrUint16           dev_config_id;
    DataReqModeCapab    data_req_mode_capab;
    AttributeList       optionList;
} PhdAssociationInformation;

typedef struct ManufSpecAssociationInformation
{
    UUID_Ident data_proto_id_ext;
    Any data_proto_info_ext;
} ManufSpecAssociationInformation;

typedef CsrUint16 PersonId;

#define UNKNOWN_PERSON_ID 0xFFFF

typedef CsrUint16 DataReqId;

#define DATA_REQ_ID_MANAGER_INITIATED_MIN 0x0000
#define DATA_REQ_ID_MANAGER_INITIATED_MAX 0xEFFF
#define DATA_REQ_ID_AGENT_INITIATED 0xF000

/* var */
typedef struct ObservationScan
{
    HANDLE obj_handle;
    AttributeList attributes;
} ObservationScan;


typedef struct ObservationScanList
{
    CsrUint16 count;
    CsrUint16 length;
    ObservationScan *value; /* first element of the array */
} ObservationScanList;

/* fixed */
typedef struct ObservationScanFixed
{
    HANDLE obj_handle;
    octet_string obs_val_data;
} ObservationScanFixed;

typedef struct ObservationScanFixedList
{
    CsrUint16 count;
    CsrUint16 length;
    ObservationScanFixed *value; /* first element of the array */
} ObservationScanFixedList;

/* single var */
typedef struct ScanReportInfoVar
{
    DataReqId data_req_id;
    CsrUint16 scan_report_no;
    ObservationScanList obs_scan_var;
} ScanReportInfoVar;

/* single fixed */
typedef struct ScanReportInfoFixed
{
    DataReqId data_req_id;
    CsrUint16 scan_report_no;
    ObservationScanFixedList obs_scan_fixed;
} ScanReportInfoFixed;

/* multiple var */
typedef struct ScanReportPerVar
{
    CsrUint16 person_id;
    ObservationScanList obs_scan_var;
} ScanReportPerVar;

typedef struct ScanReportPerVarList
{
    CsrUint16 count;
    CsrUint16 length;
    ScanReportPerVar *value; /* first element of the array */
} ScanReportPerVarList;

typedef struct ScanReportInfoMPVar
{
    DataReqId data_req_id;
    CsrUint16 scan_report_no;
    ScanReportPerVarList scan_per_var;
} ScanReportInfoMPVar;

/* multiple fixed */
typedef struct ScanReportPerFixed
{
    CsrUint16 person_id;
    ObservationScanFixedList obs_scan_fix;
} ScanReportPerFixed;

typedef struct ScanReportPerFixedList
{
    CsrUint16 count;
    CsrUint16 length;
    ScanReportPerFixed *value; /* first element of the array */
} ScanReportPerFixedList;

typedef struct ScanReportInfoMPFixed
{
    DataReqId data_req_id;
    CsrUint16 scan_report_no;
    ScanReportPerFixedList scan_per_fixed;
} ScanReportInfoMPFixed;



/**/

typedef octet_string ObservationScanGrouped;

typedef struct ScanReportInfoGroupedList
{
    CsrUint16 count;
    CsrUint16 length;
    ObservationScanGrouped *value; /* first element of the array */
} ScanReportInfoGroupedList;

typedef struct ScanReportInfoGrouped
{
    CsrUint16 data_req_id;
    CsrUint16 scan_report_no;
    ScanReportInfoGroupedList obs_scan_grouped;
} ScanReportInfoGrouped;

typedef struct ScanReportPerGrouped
{
    PersonId person_id;
    ObservationScanGrouped obs_scan_grouped;
} ScanReportPerGrouped;

typedef struct ScanReportPerGroupedList
{
    CsrUint16 count;
    CsrUint16 length;
    ScanReportPerGrouped *value; /* first element of the array */
} ScanReportPerGroupedList;

typedef struct ScanReportInfoMPGrouped
{
    CsrUint16 data_req_id;
    CsrUint16 scan_report_no;
    ScanReportPerGroupedList scan_per_grouped;
} ScanReportInfoMPgrouped;

typedef struct ConfigObject
{
    OID_Type obj_class;
    HANDLE obj_handle;
    AttributeList attributes;
} ConfigObject;

typedef struct ConfigObjectList
{
    CsrUint16 count;
    CsrUint16 length;
    ConfigObject *value; /* first element of the array */
} ConfigObjectList;

typedef struct ConfigReport
{
    ConfigId config_report_id;
    ConfigObjectList config_obj_list;
} ConfigReport;

typedef CsrUint16 ConfigResult;

#define ACCEPTED_CONFIG         0x0000
#define UNSUPPORTED_CONFIG      0x0001
#define STANDARD_CONFIG_UNKNOWN 0x0002

typedef struct ConfigReportRsp
{
    ConfigId config_report_id;
    ConfigResult config_result;
} ConfigReportRsp;

typedef CsrUint16 DataReqMode;

#define DATA_REQ_START_STOP 0x8000
#define DATA_REQ_CONTINUATION 0x4000
#define DATA_REQ_SCOPE_ALL 0x0800
#define DATA_REQ_SCOPE_TYPE 0x0400
#define DATA_REQ_SCOPE_HANDLE 0x0200
#define DATA_REQ_MODE_SINGLE_RSP 0x0080
#define DATA_REQ_MODE_TIME_PERIOD 0x0040
#define DATA_REQ_MODE_TIME_NO_LIMIT 0x0020
#define DATA_REQ_MODE_DATA_REQ_PERSON_ID 0x0008

typedef struct HANDLEList
{
    CsrUint16 count;
    CsrUint16 length;
    HANDLE *value; /* first element of the array */
} HANDLEList;

typedef struct DataRequest
{
    DataReqId data_req_id;
    DataReqMode data_req_mode;
    RelativeTime data_req_time;
    CsrUint16 DataRequest_data_req_person_id;
    OID_Type data_req_class;
    HANDLEList data_req_obj_handle_list;
} DataRequest;

typedef CsrUint16 DataReqResult;

#define DATA_REQ_RESULT_NO_ERROR 0
#define DATA_REQ_RESULT_UNSPECIFIC_ERROR 1
#define DATA_REQ_RESULT_NO_STOP_SUPPORT 2
#define DATA_REQ_RESULT_NO_SCOPE_ALL_SUPPORT 3
#define DATA_REQ_RESULT_NO_SCOPE_CLASS_SUPPORT 4
#define DATA_REQ_RESULT_NO_SCOPE_HANDLE_SUPPORT 5
#define DATA_REQ_RESULT_NO_MODE_SINGLE_RSP_SUPPORT 6
#define DATA_REQ_RESULT_NO_MODE_TIME_PERIOD_SUPPORT 7
#define DATA_REQ_RESULT_NO_MODE_TIME_NO_LIMIT_SUPPORT 8
#define DATA_REQ_RESULT_NO_PERSON_ID_SUPPORT 9
#define DATA_REQ_RESULT_UNKNOWN_PERSON_ID 11
#define DATA_REQ_RESULT_UNKNOWN_CLASS 12
#define DATA_REQ_RESULT_UNKNOWN_HANDLE 13
#define DATA_REQ_RESULT_UNSUPP_SCOPE 14
#define DATA_REQ_RESULT_UNSUPP_MODE 15
#define DATA_REQ_RESULT_INIT_MANAGER_OVERFLOW 16
#define DATA_REQ_RESULT_CONTINUATION_NOT_SUPPORTED 17
#define DATA_REQ_RESULT_INVALID_REQ_ID 18

typedef struct DataResponse
{
    RelativeTime rel_time_stamp;
    DataReqResult data_req_result;
    OID_Type event_type;
    Any event_info;
} DataResponse;

#ifdef __cplusplus
extern "C" }
#endif

#endif /* IEEE_ASN */

