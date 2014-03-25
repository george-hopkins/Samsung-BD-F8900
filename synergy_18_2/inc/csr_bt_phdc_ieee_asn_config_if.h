#ifndef     CSR_BT_PHDC_IEEE_ASN_CONFIG_IF_H__
#define     CSR_BT_PHDC_IEEE_ASN_CONFIG_IF_H__

#include "csr_synergy.h"
/****************************************************************************

                (c) Cambridge Silicon Radio Limited 2009

                All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
/*!
@file   ieee_asn_config_if.h

@brief Header file for accessing ASN.1 types defined in IEEE 11073 specification.

These types are used for defining IEEE standard objects and attributes. Refer to
IEEE 11073-20601 specification document for details.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_bt_phdc_ieee_asn_nom_codes.h"

/************** IEEE ASN Defined Data Types **************/
typedef CsrUint32 FLOAT_Type;
typedef CsrUint16 SFLOAT_Type;
typedef CsrUint32 RelativeTime;
typedef CsrUint32 MdsTimeInfo;
typedef CsrUint32 AbsoluteTimeAdjust;

typedef CsrUint16 OID_Type;
typedef CsrUint16 HANDLE;

typedef struct Any
{
    CsrUint16 length;
    CsrUint8 *value; /* first element of the array */
} Any;

typedef struct octet_string
{
    CsrUint16 length;
    CsrUint8 *value;
} octet_string;

typedef CsrUint16 NomPartition;

#define NOM_PART_UNSPEC         0
#define NOM_PART_OBJ            1
#define NOM_PART_METRIC         2
#define NOM_PART_ALERT          3
#define NOM_PART_DIM            4
#define NOM_PART_VATTR          5
#define NOM_PART_PGRP           6
#define NOM_PART_SITES          7
#define NOM_PART_INFRASTRUCT    8
#define NOM_PART_FEF            9
#define NOM_PART_ECG_EXTN       10
#define NOM_PART_PHD_DM         128
#define NOM_PART_PHD_HF         129
#define NOM_PART_PHD_AI         130
#define NOM_PART_RET_CODE       255
#define NOM_PART_EXT_NOM        256
#define NOM_PART_PRIV 1024

typedef struct TYPE
{
    NomPartition partition;
    OID_Type code;
} TYPE;

typedef struct TypeVer
{
    OID_Type type;
    CsrUint16 version;
} TypeVer;

typedef struct TypeVerList
{
    CsrUint16 count;
    CsrUint16 length;
    TypeVer *value; /* first element of the array */
} TypeVerList;

typedef struct HighResRelativeTime
{
    CsrUint8 value[8];
} HighResRelativeTime;

typedef struct AbsoluteTime
{
    CsrUint8 century;
    CsrUint8 year;
    CsrUint8 month;
    CsrUint8 day;
    CsrUint8 hour;
    CsrUint8 minute;
    CsrUint8 second;
    CsrUint8 sec_fractions;
} AbsoluteTime;

typedef CsrUint16 MetricSpecSmall;

#define MSS_AVAIL_INTERMITTENT      0x8000
#define MSS_AVAIL_STORED_DATA       0x4000
#define MSS_UPD_APERIODIC           0x2000
#define MSS_MSMT_APERIODIC          0x1000
#define MSS_MSMT_PHYS_EV_ID         0x0800
#define MSS_MSMT_BTB_METRIC         0x0400
#define MSS_ACC_MANAGER_INITIATED   0x0080
#define MSS_ACC_AGENT_INITIATED     0x0040
#define MSS_CAT_MANUAL              0x0008
#define MSS_CAT_SETTING             0x0004
#define MSS_CAT_CALCULATION         0x0002

#define MS_STRUCT_SIMPLE        0
#define MS_STRUCT_COMPOUND      1
#define MS_STRUCT_RESERVED      2
#define MS_STRUCT_COMPOUND_FIX  3

typedef struct MetricStructureSmall
{
    CsrUint8 ms_struct;
    CsrUint8 ms_comp_no;
} MetricStructureSmall;

typedef struct AttrValMapEntry
{
    OID_Type attribute_id;
    CsrUint16 attribute_len;
} AttrValMapEntry;

typedef struct AttrValMap
{
    CsrUint16 count;
    CsrUint16 length;
    AttrValMapEntry *value; /* first element of the array */
} AttrValMap;

typedef struct MetricIdList
{
    CsrUint16 count;
    CsrUint16 length;
    OID_Type *value; /* first element of the array */
} MetricIdList;

typedef struct AttributeIdList
{
    CsrUint16 count;
    CsrUint16 length;
    OID_Type *value; /* first element of the array */
} AttributeIdList;

typedef struct SupplementalTypeList
{
    CsrUint16 count;
    CsrUint16 length;
    TYPE *value; /* first element of the array */
} SupplementalTypeList;

typedef CsrUint16 MeasurementStatus;

#define MS_INVALID              0x8000
#define MS_QUESTIONABLE         0x4000
#define MS_NOT_AVAILABLE        0x2000
#define MS_CALIBRATION_ONGOING  0x1000
#define MS_TEST_DATA            0x0800
#define MS_DEMO_DATA            0x0400
#define MS_VALIDATED_DATA       0x0080

#define MS_EARLY_INDICATION     0x0040
#define MS_MSMT_ONGOING         0x0020

typedef FLOAT_Type SimpleNuObsValue;

typedef struct SimpleNuObsValueCmp
{
    CsrUint16 count;
    CsrUint16 length;
    SimpleNuObsValue *value; /* first element of the array */
} SimpleNuObsValueCmp;

typedef SFLOAT_Type BasicNuObsValue;

typedef struct BasicNuObsValueCmp
{
    CsrUint16 count;
    CsrUint16 length;
    BasicNuObsValue *value; /* first element of the array */
} BasicNuObsValueCmp;

typedef struct NuObsValue
{
    OID_Type metric_id;
    MeasurementStatus state;
    OID_Type unit_code;
    FLOAT_Type value;
} NuObsValue;

typedef struct NuObsValueCmp
{
    CsrUint16 count;
    CsrUint16 length;
    NuObsValue *value; /* first element of the array */
} NuObsValueCmp;

typedef octet_string EnumPrintableString;

#define OBJ_ID_CHOSEN       0x0001
#define TEXT_STRING_CHOSEN  0x0002
#define BIT_STR_CHOSEN      0x0010

typedef struct EnumVal
{
    CsrUint16 choice;
    CsrUint16 length;
    union
    {
        OID_Type enum_obj_id;
        octet_string enum_text_string;
        CsrUint32 enum_bit_str; /* BITS-32*/
    } u;
} EnumVal;

typedef struct EnumObsValue
{
    OID_Type metric_id;
    MeasurementStatus state;
    EnumVal value;
} EnumObsValue;

typedef CsrUint8 AuthBody;

#define AUTH_BODY_EMPTY         0
#define AUTH_BODY_IEEE_11073    1
#define AUTH_BODY_CONTINUA      2
#define AUTH_BODY_EXPERIMENTAL  254
#define AUTH_BODY_RESERVED      255

typedef CsrUint8 AuthBodyStrucType;

typedef struct AuthBodyAndStrucType
{
    AuthBody auth_body;
    AuthBodyStrucType auth_body_struc_type;
} AuthBodyAndStrucType;

typedef struct RegCertData
{
    AuthBodyAndStrucType auth_body_and_struc_type;
    Any auth_body_data;
} RegCertData;

typedef struct RegCertDataList
{
    CsrUint16 count;
    CsrUint16 length;
    RegCertData *value; /* first element of the array */
} RegCertDataList;

/* Continua Version 1.0 Guideline data structure for Regulatory and Certification requirements */

typedef CsrUint8 ContinuaStructType;

#define CONTINUA_VERSION_STRUCT     1
#define CONTINUA_REG_STRUCT         2

typedef CsrUint16 CertifiedDeviceClassEntry;

#define PAN_PULSE_OXIMETER          4
#define PAN_BLOODPRESSURE_MONITOR   7
#define PAN_THERMOMETER             8
#define PAN_WEIGHING_SCALE          15
#define PAN_GLUCOSE_METER           17
#define PAN_CARDIOVASCULAR_FITNESS  41
#define PAN_STRENGTH_FITNESS        42
#define PAN_ACTIVITY_HUB            71

typedef struct CertifiedDeviceClassList
{
    CsrUint16 count;
    CsrUint16 length;
    CertifiedDeviceClassEntry *value; /* first element of the array */
} CertifiedDeviceClassList;

typedef struct ContinuaBodyStruct
{
    CsrUint8 major_IG_version;
    CsrUint8 minor_IG_version;
    CertifiedDeviceClassList certified_devices;
} ContinuaBodyStruct;

typedef CsrUint16 RegulationBitFieldType;

#define UNREGULATED_DEVICE 0x8000 /* Unregulated Device (Bit0) - Set to 0: regulated, Set to 1: not regulated */

typedef struct ContinuaRegStruct
{
    RegulationBitFieldType regulation_bit_field;
} ContinuaRegStruct;


#ifdef __cplusplus
extern "C" }
#endif


#endif /*CSR_BT_PHDC_IEEE_ASN_CONFIG_IF_H_*/

