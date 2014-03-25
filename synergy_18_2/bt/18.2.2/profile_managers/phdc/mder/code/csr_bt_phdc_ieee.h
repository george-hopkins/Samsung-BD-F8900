#ifndef     CSR_BT_PHDC_IEEE_H__
#define     CSR_BT_PHDC_IEEE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
/*!
@file   ieee.h

@brief Header file for accessing IEEE 11073 functionality for Health Care Devices

*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef DO_NOT_DOCUMENT /* hide from doxygen */
#include "bluetooth.h"
/*#include <message.h>  For Task Definition */
/*#include <connection.h>  For sdp_search_status */
#include "csr_types.h"

#include "csr_bt_phdc_ieee_asn_config_if.h"
#endif /*DO_NOT_DOCUMENT */

typedef CsrUint16         tBool;
typedef BD_ADDR_T bdaddr;
typedef CsrUint16 sdp_search_status;
typedef BD_ADDR_T Sink;


#ifndef TRUE
#define TRUE           ((tBool)1)
#endif
#ifndef FALSE
#define FALSE          ((tBool)0)
#endif

/*!
    \brief Maximum string length for Manufacturer Name. As per IEEE 11073-10601
    spec, the string size shall be even. Though, the string length is fixed at
    16, it can be modified if there is a compelling reason.*/
#define MAX_MANUFACTURER_NAME_LEN       16


/*!
    \brief Maximum string length for Serial Number.*/
#define MAX_SERIAL_NUMBER_LENGTH        16

/*!
    \brief Maximum string length for Firmware Version.*/
#define MAX_FW_VERSION_LENGTH       16

/*!
    \brief Maximum string length for Model information. As per IEEE 11073-10601
    spec, the string size shall be even. Though, the string length is fixed at
    16, it can be modified if there is a compelling reason.*/
#define MAX_MODEL_NUMBER_LEN            16

/*! \brief Extended Unique Identifier (IEEE EUI-64) length */
#define IEEE_SYSTEM_ID_SIZE 8

/*! \brief IEEE Library initialisation status */

typedef enum {

/*! \brief Successfully completed IEEE Library initialisation */
    ieee_success = 0x00,

/*! \brief Failed to complete IEEE library initialisation */
    ieee_failure

} ieeeStatus;

/*! \brief IEEE connection procedure status */

typedef enum {

/*! \brief IEEE connection with the Manager device is successfully established */
    ieee_con_success = 0x00,

/*! \brief IEEE connection failed because of failure in establishing l2cap channel
    for control channel*/
    ieee_con_l2cap_channel_failure,

/*! \brief IEEE Connect Req received in an IEEE internal state where it can not
    be handled. */
    ieee_con_invalid_state,

/*! \brief IEEE Connect Req received when a Connect Req from Manager device
    is already being processed. */
    ieee_con_mgr_init_con_ongoing,

/*! \brief IEEE connection failed because of failure to create a MCAP Data Link (MDL)
    with the Manager device. This could happen if the resources are not temporarily
    available with the Manager like MDL or MDEP are busy.*/
    ieee_con_mgr_busy_temp,

/*! \brief IEEE connection failed because of failure to create a MCAP Data Link (MDL)
    with the Manager device due to an un-specified error. */
    ieee_con_mgr_unspecified_err,

/*! \brief IEEE connection failed because of transport getting disconnected. This
    can be due to link-loss or disconnect-by-manager. This will happen only when
    the control channel is already setup */
    ieee_con_mgr_disconnected,

/*! \brief Post successful control channel setup, IEEE connection failed because
    l2cap channel for data link could not be setup */
    ieee_con_data_connect_failed

} ieeeConStatus;

/*! \brief IEEE association release procedure status */

typedef enum {
/*! \brief IEEE association has been successfully released with the Manager
    device. After this procedure both the Agent and Manager devices are '
    in IEEE 'Unassociated' state. */
    ieee_asso_rel_success = 0x00,

/*! \brief IEEE Association Release Req received in an IEEE internal state
    where it can not be handled. It doesn't affect the current state of IEEE library. */
    ieee_asso_rel_invalid_state,

/*! \brief No response received from Manager for TOrelease timer (Time out for
    association release procedure). IEEE Agent aborts the procedure and moves
    to 'Unassociated' state.*/
    ieee_asso_rel_no_response,

/*! \brief Association Release procedure is aborted by the Agent because of
    receiving an un-expected APDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_asso_rel_abrt_unexp_apdu_rcvd,

/*! \brief Association Release procedure is aborted by the Manager because of
    Buffer Overflow at its side. IEEE agent moves to 'Unassociated' state. */
    ieee_asso_rel_abrt_mgr_buf_ovrflow,

/*! \brief Association Release procedure is aborted by the Manager because of
    response timeout. As no timer related to Association Release procedure is
    running on Manager side, this reason doesn't seem to be suitable. Hence,
    passing this error to Application for its action. IEEE agent moves to
    'Unassociated' state.*/
    ieee_asso_rel_abrt_mgr_rsp_timeout,

/*! \brief Association Release procedure is aborted by the Manager because of
    configuration timeout. As configuration procedure is not on-going, this
    reason doesn't seem to be suitable. Hence, passing this error to Application
    for its action. IEEE agent moves to 'Unassociated' state.*/
    ieee_asso_rel_abrt_mgr_cfg_timeout,

/*! \brief Association Release procedure is aborted by the Manager because of
    undefined error at its side. IEEE agent moves to 'Unassociated' state. */
    ieee_asso_rel_abrt_mgr_undefined

} ieeeAssociateRelStatus;


/*! \brief IEEE Abort indication status */

typedef enum {
    /*! \brief In operating state, when data transfer is not on-going, the operation
    has been aborted by the Manager because of Buffer Overflow. IEEE Agent
    moves to 'Unassociated' state. */
    ieee_abrt_mgr_buf_ovrflow = 0x00,

    /*! \brief In operating state, when data transfer is not on-going, the operation
    has been aborted by the Manager because of response time-out for an on-going
    procedure. IEEE Agent moves to 'Unassociated' state. */
    ieee_abrt_mgr_rsp_timeout,

    /*! \brief In operating state, when data transfer is not on-going, the operation
    has been aborted by the Manager because of configuration time-out. As configuration
    procedure is not on-going, this reason doesn't seem to be suitable. Hence,
    passing this error to Application for its action. IEEE Agent moves to
    'Unassociated' state.*/
    ieee_abrt_mgr_cfg_timeout,

    /*! \brief In operating state, when data transfer is not on-going, the operation
    has been aborted by the Manager because of undefined error. IEEE Agent
    moves to 'Unassociated' state. */
    ieee_abrt_mgr_undefined,

    /*! \brief Unexpected APDU received. IEEE Agent
    aborts the on-going procedure and moves to 'Unassociated' state */
    ieee_abrt_unexp_apdu_rcvd,

    /*! \brief An on-going procedure message has been rejected by Manager in IEEE
    'Operating' state. IEEE Agent aborts the on-going procedure and moves to
    'Unassociated' state */
    ieee_abrt_rej_mgr,

    /*! \brief IEEE DataApdu invoked action resulted in an error in IEEE
    'Operating' state. IEEE Agent aborts the on-going procedure and moves
    to 'Unassociated' state */
    ieee_abrt_err_mgr,

    /*! \brief Remote Agent device tried associating with this device. The attempt has been
    rejected. The Agent moves to 'Unassociated' state*/
    ieee_abrt_agent_agent_association


} ieeeAbortStatus;

/*! \brief IEEE association procedure status */

typedef enum {
    /*! \brief IEEE association procedure has been successfully completed with
    the Manager device. IEEE Agent moves to 'Operating' state */
    ieee_asso_accepted = 0x00,

    /*! \brief IEEE association procedure has been rejected by the Manager
    device. IEEE Agent moves back to 'Unassociated' state */
    ieee_asso_rejected,

    /*! \brief IEEE association procedure has timed-out. It either involves
    time-out for receiving response for Association Request ie., expiry
    of TOassoc timer (for association procedure) with the RCassoc retry
    count OR time-out for receiving response for Configuration Request ie.,
    expiry of TOconfig timer. IEEE Agent moves back to IEEE 'unassociated'
    state after aborting the on-going procedure.*/
    ieee_asso_no_response,

    /*! \brief IEEE Association Req message received in an IEEE internal state
    where it can not be handled. This doesn't affect the current state of IEEE library. */
    ieee_asso_invalid_state,

    /*! \brief Association procedure is aborted by the Manager because of
    Buffer Overflow at its side. IEEE agent moves to 'Unassociated' state. */
    ieee_asso_abrt_mgr_buf_overflow,

    /*! \brief Association procedure is aborted by the Manager because of
    Response Timeout at its side. IEEE agent moves to 'Unassociated' state.*/
    ieee_asso_abrt_mgr_rsp_timeout,

    /*! \brief Association procedure is aborted by the Manager because of
    Congiguration Timeout (ie., TOconfig expiry) at its side. This doesn't
    seem to be proper scenario for a Manager device as the Configuration or
    Association procedure is always triggered by IEEE Agent device. Hence,
    passing this error to Application for its action. IEEE Agent moves to
    'Unassociated' state. */
    ieee_asso_abrt_mgr_cfg_timeout,

    /*! \brief Association procedure is aborted by the Manager because of an
    undefined error at its side. IEEE agent moves to 'Unassociated' state. */
    ieee_asso_abrt_mgr_undefined,

    /*! \brief Association procedure is aborted by IEEE Agent because of
    protocol error like invoke-id mis-match, invalid common data protocol
    version etc., in the received response message. IEEE agent moves to
    'Unassociated' state. */
    ieee_asso_abrt_proto_err_mgr,

    /*! \brief Association procedure is aborted by the Agent because of
    receiving an un-expected APDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_asso_abrt_unexp_apdu_rcvd,

    /*! \brief Association procedure is aborted by the Agent because of
    receiving an REJECT DataAPDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_asso_rej_mgr,

    /*! \brief Association procedure is aborted by the Agent because of
    receiving an ERROR DataAPDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_asso_err_mgr,

    /*! \brief Association released by the Agent as there is no common
    configuration supported by Manager device. */
    ieee_asso_rel_unsupported_config

} ieeeAssocStatus;

/*! \brief IEEE data transfer procedure status */

typedef enum {
    /*! \brief IEEE Data transfer procedure has been successfully completed. IEEE Agent
    remains in 'Operating' state. */
    ieee_dt_success = 0x00,

    /*! \brief IEEE Data transfer Req message received in an IEEE internal state
    where it can not be handled. This doesn't affect the current state of IEEE library. */
    ieee_dt_invalid_state,

/*! \brief No response received from Manager for TOcer-mds timer (Time out for
    MDS Confirmed Event Reports). This timer is started for data transmission to
    the Manager device. IEEE Agent aborts the procedure and moves to 'Unassociated'
    state.*/
    ieee_dt_no_response,

/*! \brief Data transfer procedure is aborted by the Agent because of
    receiving an un-expected APDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_dt_abrt_unexp_apdu_rcvd,

/*! \brief Data transfer procedure is aborted by the Manager because of an undefined
    error at its side. IEEE agent moves to 'Unassociated' state. */
    ieee_dt_abrt_mgr_undefined,

/*! \brief Data transfer procedure is aborted by the Manager with an invalid cause
    of Configuration TimeOut. Hence, passing this error to Application for its action.
    IEEE Agent moves to 'Unassociated' state. */
    ieee_dt_abrt_mgr_cfg_timeout,

/*! \brief Data transfer procedure is aborted by the Manager because of
    Response Timeout at its side. IEEE agent moves to 'Unassociated' state.*/
    ieee_dt_abrt_mgr_rsp_timeout,

/*! \brief Data transfer procedure is aborted by the Manager because of
    Buffer Overflow at its side. IEEE agent moves to 'Unassociated' state.*/
    ieee_dt_abrt_mgr_buf_overflow,

/*! \brief Data transfer procedure is aborted by IEEE Agent because of
    protocol error like invoke-id mis-match etc., in the received response message.
    IEEE agent moves to 'Unassociated' state. */
    ieee_dt_abrt_proto_err_mgr,

/*! \brief Data transfer procedure is aborted by the Agent because of
    receiving an REJECT DataAPDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_dt_rej_mgr,

/*! \brief Data transfer procedure is aborted by the Agent because of
    receiving an ERROR DataAPDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_dt_err_mgr

} ieeeDataTrsfStatus;

/*! \brief IEEE disconnect procedure status */

typedef enum {
/*! \brief Disconnect procedure has been successfully completed with the Manager device.
    IEEE Agent moves to 'Disconnected' state. */
    ieee_discon_success = 0x00,

/*! \brief Disconnect because of link-loss with the Manager device. IEEE Agent moves
    to 'Disconnected' state. */
    ieee_discon_link_loss,

/*! \brief Disconnect request received in Invalid state and hence cannot be processed. */
    ieee_discon_invalid_state

} ieeeDisconnectStatus;

/*! \brief IEEE Config Object Update procedure status */

typedef enum {

/*! \brief IEEE Update Config Object Req procedure has been successfully completed.
    IEEE Agent remains in 'Operating' state. */
    ieee_update_confObj_success = 0x00,

/*! \brief IEEE Update Config Object Req message received in an IEEE internal state
    where it can not be handled. This doesn't affect the current state of IEEE library. */
    ieee_update_confObj_invalid_state,

/*! \brief IEEE Update Config Object Req message validation failed. This doesn't affect the
    current state of IEEE library. */
    ieee_update_confObj_validation_failure,

/*! \brief No response received from Manager for TOcer-mds timer (Time out for
    MDS Confirmed Event Reports). This timer is started when variable event report is
    transmitted to the Manager device. IEEE Agent aborts the procedure and moves to
    'Unassociated' state.*/
    ieee_update_confObj_no_response,

/*! \brief Config Object Update procedure is aborted by the Agent because of
    receiving an un-expected APDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_update_confObj_abrt_unexp_apdu_rcvd,

/*! \brief Config Object Update procedure is aborted by the Manager because of an undefined
    error at its side. IEEE agent moves to 'Unassociated' state. */
    ieee_update_confObj_abrt_mgr_undefined,

/*! \brief Config Object Update procedure is aborted by the Manager with an invalid cause
    of Configuration TimeOut. Hence, passing this error to Application for its action.
    IEEE Agent moves to 'Unassociated' state. */
    ieee_update_confObj_abrt_mgr_cfg_timeout,

/*! \brief Config Object Update procedure is aborted by the Manager because of
    Response Timeout at its side. IEEE agent moves to 'Unassociated' state.*/
    ieee_update_confObj_abrt_mgr_rsp_timeout,

/*! \brief Config Object Update procedure is aborted by the Manager because of
    Buffer Overflow at its side. IEEE agent moves to 'Unassociated' state.*/
    ieee_update_confObj_abrt_mgr_buf_overflow,

/*! \brief Config Object Update procedure is aborted by the Agent because of
    receiving REJECT DataAPDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_update_confObj_rej_mgr,

/*! \brief Config Object Update procedure is aborted by the Agent because of
    receiving an ERROR DataAPDU from Manager. IEEE agent moves to
    'Unassociated' state. */
    ieee_update_confObj_err_mgr

} ieeeConfObjUpdateStatus;


/*! \brief IEEE Library logical entities corresponding to device data specializations */

typedef enum {

/*! \brief Blood Pressure Monitor - IEEE 11073-10407 - SUPPORTED */
    ieee_le_bpm = 0x00,

/*! \brief Weighing Scale - IEEE 11073-10415 - SUPPORTED */
    ieee_le_weighing_scale,

/*! \brief Cardiovascular Fitness Monitor - IEEE 11073-10441 - SUPPORTED */
    ieee_le_cardio,

/*! \brief Thermometer - IEEE 11073-10408 - NOT SUPPORTED */
    ieee_le_thermometer,

/*! \brief Glucose Meter - IEEE 11073-10417 - NOT SUPPORTED */
    ieee_le_glucose_meter,

/*! \brief Pulse Oximeter - IEEE 11073-10404 - NOT SUPPORTED */
    ieee_le_pulse_oximeter,

/*! \brief Usage of ieee_le_all enum value should be avoided by the application till
    multi-functional support is made available from IEEE library */
    ieee_le_all,

/*! \brief None - This option should not be used by application while triggering
    any IEEE library procedure. It is reserved for internal use by IEEE
    library and may also be used in reporting Errors when
    none of the device data specialization is active. As an example, when connection
    attempt triggered by the manager device fails to establish control channel,
    there is no information of Device data specialization available with the
    IEEE library. In such case, ieee_le_none option could be used.*/
    ieee_le_none
} ieeeLogEntityType;

/*! \brief IEEE Library procedures */

typedef enum {
/*! \brief No ongoing procedure */
    ieee_none = 0x00,

/*! \brief IEEE library initialisation procedure. */
    ieee_init,

/*! \brief IEEE Connect procedure (Agent initiated or Manager initiated). */
    ieee_connect,

/*! \brief IEEE Association procedure. */
    ieee_associate,

/*! \brief IEEE Configuration procedure - This is a sub-procedure of IEEE Association
    procedure which is initiated if the Manager device is not aware of the configuration
    supported by Agent device. */
    ieee_configure,

/*! \brief IEEE Data Transfer procedure. */
    ieee_data_transfer,

/*! \brief IEEE Configuration Object Update procedure. */
    ieee_conf_obj_update,

/*! \brief IEEE Association Release procedure. */
    ieee_disassociate,

/*! \brief IEEE Disconnect procedure (Agent initiated or Manager initiated or link-loss). */
    ieee_disconnect

} ieeeProcedureType;


/*! \brief State maintained by IEEE library per Logical Entity (ie., device data specialization) */

typedef enum {
/*! \brief Invalid State - Post initialisation none of the supported device data specialization
    logical entities should be in this state.*/
    invalid_st  = 0x00,

/*! \brief Logical Entity is not connected to any Manager device */
    disconnected,

/*! \brief Logical Entity is getting connected to Manager device */
    connecting,

/*! \brief Logical Entity is connected to Manager device and in IEEE 'unassociated' state */
    unassociated,

/*! \brief Logical Entity is associating to Manager device */
    associating,

/*! \brief Logical Entity is exchanging configuration information with Manager device */
    configuring,

/*! \brief Logical Entity is ready for data transfer in 'Operating' state */
    operating,

/*! \brief Logical Entity is getting disassociated to the Manager device */
    disassociating,

/*! \brief Logical Entity is getting disconnected to Manager device */
    disconnecting

} logEntityStateType;


/*! \brief The User defined PSKEYS used by IEEE Library. */
enum
{

    /* PSKEY 24 is not used as faced issues while using it.
        Seems it is being used by some other library */

    /*! \brief User defined PS KEY (30) for setting Production Specification
    information in the format given in structure tIeeeProductionSpec. Two field i.e.,
    Serial Number and Firmware Number needs to be filled. It is mandatory (as
    per Continua V1.0 guidelines) for application to set this PS KEY. */
    PSKEY_PRODUCTION_SPEC               = 30,

    /*! \brief User defined PS KEY (31) for setting Manufacturer's information
    in the format given in structure tIeeeManufactureInfo. It is mandatory (as
    per Continua V1.0 guidelines) for application to set this PS KEY. */
    PSKEY_MANUFACTURER_INFO             = 31

  /* USER defined PS KEYs 40 to 49 are used as given below
    PSKEY_STACK_DEVICE_TABLE            = 40 - 49*/
};

/* START - Vendor Specific Attributes */
/*! \brief From nomenclature perspective, every partition has a reservation area that is set aside
for vendor codes (0xF000 - 0xFFFF).  A chunk of this space was reserved by Continua
(0xFC00 - 0xFFFF) leaving 0xF000 to 0xFBFF for OEMs.*/

/*! \brief Vendor / OEM specific attributes range (0xF000 to 0xFBFF) */

/*! \brief Attribute Identity 0xF000 has been reserved by IEEE library to identify
Vendor specific MDS Object Attribute */
#define MDC_MDS_OBJ_ATTR_VENDOR_SPECIFIC 0xF000

/*! \brief From nomenclature perspective, every partition has a reservation area that is set aside
for vendor codes (0xF000 - 0xFFFF).  A chunk of this space was reserved by Continua
(0xFC00 - 0xFFFF) leaving 0xF000 to 0xFBFF for OEMs.*/

/*! \brief Vendor / OEM specific attributes range (0xF000 to 0xFBFF) */

/*! \brief Attribute Identity 0xF001 has been reserved by IEEE library to identify
Vendor specific Non-MDS Object Attribute */
#define MDC_OBJ_ATTR_VENDOR_SPECIFIC 0xF001

/* END */

#ifndef DO_NOT_DOCUMENT /* Hide from Doxygen */
#define ATTR_DATA_FORMAT_UINT8 0x0001
#define ATTR_DATA_FORMAT_UINT16 0x0002
#define ATTR_DATA_FORMAT_UINT32 0x0004
#define ATTR_DATA_FORMAT_STRING 0x0008
#endif /* DO_NOT_DOCUMENT */

/*! \brief Describes data format for vendor specific attribute or object structures */

typedef struct AttrDataFormat
{
    /*! \brief The number of elements in data_value array given below. Basically, this number is
    same as the number of elements in the structure which the data format is representing.*/
    CsrUint16 data_count;

    /*! \brief 'data_value' array takes basic data format values given below for each
    element in the structure that it represents. */
    /*! \brief The value for supported basic data types are */
    /*! \brief #define ATTR_DATA_FORMAT_UINT8 0x0001 (for CsrUint8) */
    /*! \brief #define ATTR_DATA_FORMAT_UINT16 0x0002 (for CsrUint16) */
    /*! \brief #define ATTR_DATA_FORMAT_UINT32 0x0004 (for CsrUint32) */
    /*! \brief #define ATTR_DATA_FORMAT_STRING 0x0008 (for string) */
    /*! \note Float_Type and SFloat_Type are represented by CsrUint32 and CsrUint16 basic
    data types respectively. */
    CsrUint16 *data_value;

} AttrDataFormat;

/*! \brief Describe vendor specific attribute information for MDS object */

typedef struct MdsVSAttributeInfo
{
    /*! \brief Attribute identity - range 0xF002 to 0xFBFF */
    CsrUint16          attrId;

    /*! \brief Attribute structure data format */
    AttrDataFormat  dataFrmt;

    /*! \brief Attribute value structure typecasted to void pointer. The string type in
    attribute value structure should be preceded by CsrUint16 'length' field. */
    void*           data;

} MdsVSAttributeInfo;

/*! \brief Describe vendor specific attribute information for non-MDS object */

typedef MdsVSAttributeInfo VSAttributeInfo;

/*! \brief Attribute information to be used while specifying extended configuration. */

typedef struct AttributeInfo
{
    /*! \brief Attribute identity. For standard attributes refer to IEEE 11073-20601 specification.
    Use attribute identity in the range 0xF040 to 0xF060 for vendor specific attributes. */
    CsrUint16      attrId;

    /*! \brief Attribute Information as given for standard attributes defined in IEEE 11073-20601
    specification  'attrInfo' is of type suitable for attribute identity. As an example, for attribute id
    'MDC_ATTR_TIME_ABS', type used should be 'AbsoluteTime'. Please refer to Table 2 (MDS attributes)
    and Table 5 (Metric attributes) of IEEE 11073-20601 for details on standard MDS attributes and Metric
    attributes respectively. */

    /*! \note Please note that following MDS attributes are not supported.
    MDC_ATTR_MDS_TIME_INFO - Synchronization and settable time is not supported.
    MDC_ATTR_TIME_ABS_ADJUST - Not required for Get MDS command.
    MDC_ATTR_SYS_TYPE - Either this or MDC_ATTR_SYS_TYPE_SPEC_LIST should be supported and IEEE library
                        supports MDC_ATTR_SYS_TYPE_SPEC_LIST
    MDC_ATTR_REG_CERT_DATA_LIST - Will be supported in the later release.
    MDC_ATTR_ATTRIBUTE_VAL_MAP - Fixed data transmission not supported for MDS object. */

    /*! \note Application can pass vendor specific attributes for objects. It should use
    'MDC_MDS_OBJ_ATTR_VENDOR_SPECIFIC' as the attribute id for MDS Object Attributes and
    'MDC_OBJ_ATTR_VENDOR_SPECIFIC' for non-MDS Object Attru=ibutes. The 'attrInfo' should
    use structure 'MdsVSAttributeInfo' and 'VSAttributeInfo' for vendor specific MDS
    attributes and vendor specific Non-MDS attributes respectively.*/
    void*       attrInfo;
} AttributeInfo;

/*! \brief Attribute Infomation List */
typedef struct AttributeInfoList
{
    /*! \brief Number of attributes */
    CsrUint16          count;

    /*! \brief Attribute information */
    AttributeInfo   *attributeInfo;
} AttributeInfoList;

/*! \brief Object information as passed by Application to IEEE library */
typedef struct ObjectInfo
{
    /*! \brief Object class - refer to IEEE 11073-20601 for details of object class */
    OID_Type        obj_class;

    /*! \brief Object Handle as given by the application to uniquely identify the objects.
    Handle value '0' is reserved for MDS object. Refer to IEEE 11073-20601 for details */
    HANDLE          obj_handle;

    /*! \brief Data format in which the object data will be transmitted by application to IEEE
    library */
    AttrDataFormat  dataFormat;

    /*! \brief Number of attributes of the object */
    CsrUint16          attr_count;

    /*! \brief Attribute information list */
    AttributeInfo   *attrInfo;

} ObjectInfo;

/*! \brief Configuration table containing information of the objects and attributes. */

typedef struct ConfigTable
{
    /*! \brief Config Id can be either standard or extended configuration identity. */
    CsrUint16          configId;

    /*! \brief Number of object in the configuration. For standard configuration ,
    'objCount' should be set to Zero. */
    CsrUint16          objCount;

    /*! \brief Object information list. For standard configuration, 'pObjData' should be
    set to NULL.*/
    ObjectInfo      *pObjData;

} ConfigTable;

/*! \brief Extended Configuration Information - List of configurations in the order of
preferrence, with the most preferred being the first one in the list */

typedef struct tIeeeExtConfInfo
{
    /*! \brief Number of configurations */
    CsrUint16          size_ConfTable;

    /*! \brief Configuration information list. */

    /*! \note While using Extended Configuration application needs to specify whether
    standard configuration is being used and as well specify the order in which
    it needs to be agreed with the Manager */
    ConfigTable     *pConfTable;

} tIeeeExtConfInfo;

/*! \brief Measurement data information */

typedef struct MeasData
{
    /*! \brief Object handle to which measurement data belongs */
    HANDLE      objHandle;

    /*! \brief For fixed format measurements (the only supported format at the moment),
    Object measurement data structure should be of the same type as 'dataFormat' given in
    'ObjectInfo'.*/
    void*       data;

} MeasData;

/*! \brief Structure used for passing measurement data for extended configuration objects. */

typedef struct{

    /*! \brief Person Identity - This parameter is used when readings are taken
    in multiple person configuration mode of the device. This parameter is set to
    0xFFFF for un-known person. In single person configuration this parameter
    should be ignored.*/
    CsrUint16          personId;

    /*! \brief Number of object for which measurement data is being passed. */
    CsrUint16          objCount;

    /*! \brief Data list for measurement objects */
    MeasData        *pMeasData;

} tIeeeExtConfMeasData;

/*! \brief Manager information received in Association Response message */

typedef struct {
    /*! \brief Manager System Id:
        EUI-64 - Extended Unique Identifier consists of 24-bit unique OUI (Organizationally
        Unique Identifier) followed by a 40-bit manufacturer defined identifier. */
    CsrUint8    systemId[IEEE_SYSTEM_ID_SIZE];

    /*! \brief Associated Config Identity
        Configuration identity agreed between Agent and Manager device */
    CsrUint16      assoConfigId;

} tIeeeAssoRspData;

/*! \brief Time-stamp at which the measurement is taken.
*/

typedef struct {
/* \brief Century */
    CsrUint8 century;

/* \brief Year */
    CsrUint8 year;

/* \brief Month */
    CsrUint8 month;

/* \brief Date */
    CsrUint8 date;

/* \brief Hour */
    CsrUint8 hour;

/* \brief Minutes */
    CsrUint8 minutes;

/* \brief Seconds */
    CsrUint8 seconds;

/* \brief Seconds Fraction */
    CsrUint8 secondsFraction;
} tAbsTimeData;

/*! \brief
    Unit in which blood pressure measurement is taken.
*/
typedef enum {
/*! \brief
    Millimeters of Mercury */
    mmhg,

/*! \brief
    Kilopascals */
    kpa
} tIeeeBPMUnit;

/*! \brief
    Unit in which weight measurement is taken.
*/
typedef enum {
/*! \brief
    Kilogram */
    kg,

/*! \brief
    Pounds */
    lb
} tIeeeWeightUnit;


/*! \brief Represents predicted amount of operational time left on the
    batteries. It is measured in Days / Hours / Minutes.
*/
typedef struct {
    /*! \brief
        Numeric value (Float) of the battery remaining.
    */
    CsrUint32                  value;

    /*! \brief
        Unit in Days / Hours / Minutes
        The values to be used are */
        /*! \brief \#define IEEE_MDC_DIM_MIN 2208 (for minintes) */
        /*! \brief \#define IEEE_MDC_DIM_HR 2240 (for hours) */
        /*! \brief \#define IEEE_MDC_DIM_DAY 2272 (for days) */
    /*! \brief These values are defined in IEEE Nomenclature coding.
    */
    CsrUint16                  unit; /* Days / Hour / Minutes */

    #ifndef DO_NOT_DOCUMENT /* Hide from Doxygen */
    #define IEEE_MDC_DIM_MIN 2208 /* (for minintes) */
    #define IEEE_MDC_DIM_HR 2240 /* (for hours) */
    #define IEEE_MDC_DIM_DAY 2272 /* (for days) */
    #endif

}   ieeeBatteryMeasure;

/*! \brief
    Contains measurement data for Blood Pressure Monitor (IEEE 11073-10407) device data
    specialization.
*/
typedef struct {

/*! \brief
    Pulse Rate (SFLOAT_Type - refer IEEE 11073-20601 Annex F.7) reading as taken by the Agent device.
    Pulse Rate being an optional element, if not present, the value should be set to
    0xFFFF.
*/
    CsrUint16 pulse;

/*! \brief
    Diastolic Pressure (SFLOAT_Type refer IEEE 11073-20601 Annex F.7) reading as taken by the Agent
    device.
*/
    CsrUint16 diastolic;

/*! \brief
    Systolic Pressure (SFLOAT_Type refer IEEE 11073-20601 Annex F.7) reading as taken by the Agent
    device.
*/
    CsrUint16 systolic;

/*! \brief
    Mean Arterial Pressure (SFLOAT_Type refer IEEE 11073-20601 Annex F.7) reading as taken by the Agent
    device.
*/
    CsrUint16 map;

/*! \brief
    Blood pressure unit (mmhg or kpa) in which the readings are taken.
*/
    tIeeeBPMUnit unit;

/*! \brief
    Time stamp at which the readings are taken.
*/
    tAbsTimeData timeStamp;

/*! \brief Person Identity - This parameter is used when readings are taken
    in multiple person configuration mode of the device. This parameter is set to
    0xFFFF for un-known person. In single person configuration this parameter
    should be ignored.
*/
    CsrUint16 personId;

} tIeeeBPMMeasData;

/*! \brief
    Contains measurement data for Weighing Scale (IEEE 11073-10415) device data
    specialization.
*/
typedef struct {

/*! \brief
    Weight (FLOAT_Type - Refer IEEE 11073-20601 Annex F.6) reading as taken by the Agent device.
*/
    CsrUint32 weight;

/*! \brief
    Weight unit (kg or lb) in which the readings are taken.
*/
    tIeeeWeightUnit unit;

/*! \brief
    Time stamp at which the readings are taken.
*/
    tAbsTimeData timeStamp;

/*! \brief Person Identity - This parameter is used when readings are taken
    in multiple person configuration mode of the device. This parameter is set to
    0xFFFF for un-known person. In single person configuration this parameter
    should be ignored.
*/
    CsrUint16 personId;

} tIeeeWSMeasData;

/*! \brief Contains manufacturer specific information which is not expected to change at
    run-time.
*/

typedef struct {

    /* MDS Attributes */
    /*! \brief Length of the Manufacturer Name string */
    CsrUint16                  manuNameLen;

    /*! \brief Manufacuturer Name forms a part of SystemModel attribute
        of IEEE MDS object. */
    CsrUint8                   manName[MAX_MANUFACTURER_NAME_LEN];

    /*! \brief Length of the Model Number string */
    CsrUint16                  modelNumLen;

    /*! \brief Model Number forms a part of SystemModel attribute
        of IEEE MDS object. */
    CsrUint8                   modelNum[MAX_MODEL_NUMBER_LEN];

    /*! \brief Agent device System Id:
        EUI-64 - Extended Unique Identifier consists of 24-bit unique OUI (Organizationally
        Unique Identifier) followed by a 40-bit manufacturer defined identifier. */
    CsrUint8                   systemId[IEEE_SYSTEM_ID_SIZE];

}   tIeeeManufactureInfo;

/*! \brief Production specification details */

typedef struct {

    /* MDS Attributes */
    /*! \brief Length of the Serial Number string */
    CsrUint16                  serialNumLength;

    /*! \brief Serial Number forms a part of Production Spec attribute of
        IEEE MDS object */
    CsrUint8                   serialNumber[MAX_SERIAL_NUMBER_LENGTH];

    /*! \brief Length of the Firmware Version string */
    CsrUint16                  fwVersionLength;

    /*! \brief FW Version forms a part of Production Spec attribute of
        IEEE MDS object */
    CsrUint8                   fwVersion[MAX_FW_VERSION_LENGTH];

}tIeeeProductionSpec;

/*! \brief Contains information about the Agent device. If supported, this information
        should be periodically updated by the Agent device application to the IEEE
        library.
    */

typedef struct {

    /*! \brief Power Status, Battery Level and Remaining Battery Time are optional
        elements. Application on the Agent device may wish to include all,
        none or any of them.

        Set Choice using following bit-positions. */

        /*! \brief \#define IEEE_POWER_STATUS_CHOSEN                    0x0001 (Bit 0) */
        /*! \brief \#define IEEE_BATTERY_LEVEL_CHOSEN                   0x0002 (Bit 1) */
        /*! \brief \#define IEEE_REMAINING_BAT_TIME_CHOSEN              0x0004 (Bit 2) */
        /*! \brief \#define IEEE_REG_CERT_INFO_CHOSEN                   0x0008 (Bit 3) */

    CsrUint16              choice;

    #ifndef DO_NOT_DOCUMENT /* Hide from Doxygen */
    #define IEEE_POWER_STATUS_CHOSEN                0x0001
    #define IEEE_BATTERY_LEVEL_CHOSEN               0x0002
    #define IEEE_REMAINING_BAT_TIME_CHOSEN          0x0004
    #define IEEE_REG_CERT_INFO_CHOSEN               0x0008
    #endif
    /*! \brief Indicates whether the Agent device is running on Mains or Batteries.
        Upper bits define the charge state (FULL / TRICKLING / OFF).

        Set Power Staus using following bit-positions.*/
        /*! \brief \#define POWER_STATUS_ON_MAINS_CHOSEN            0x8000 (Bit 15)*/
        /*! \brief \#define POWER_STATUS_ON_BATTERY_CHOSEN          0x4000 (Bit 14)*/
        /*! \brief \#define POWER_STATUS_CHARGING_FULL_CHOSEN       0x0080 (Bit 7)*/
        /*! \brief \#define POWER_STATUS_CHARGING_TRICKLE_CHOSEN        0x0040 (Bit 6)*/
        /*! \brief \#define POWER_STATUS_CHARGING_OFF_CHOSEN            0x0020 (Bit 5)*/

    CsrUint16              powerStatus;

    #ifndef DO_NOT_DOCUMENT /* Hide from Doxygen */
    #define POWER_STATUS_ON_MAINS_CHOSEN            0x8000 /* Bit 15 */
    #define POWER_STATUS_ON_BATTERY_CHOSEN          0x4000 /* Bit 14 */
    #define POWER_STATUS_CHARGING_FULL_CHOSEN       0x0080 /* Bit 7 */
    #define POWER_STATUS_CHARGING_TRICKLE_CHOSEN    0x0040 /* Bit 6 */
    #define POWER_STATUS_CHARGING_OFF_CHOSEN        0x0020 /* Bit 5 */
    #endif

    /*! \brief Battery Level indicates percentage of battery capacity remaining.
        Undefined - if value > 100
    */
    CsrUint16              batteryLevel;

    /*! \brief Remaining Battery Time represents the predicted amount of operational time
        left on the batteries.
    */
    ieeeBatteryMeasure  remainingBatTime;

    /*! \brief Regulatory and Certification information of IEEE Agent device as per Continua
    Version 1.0 guidelines.
    */
    const RegCertDataList *pRegCertDataList;

} tIeeeDeviceInfo;

#ifndef DO_NOT_DOCUMENT /* hide from doxygen */

#define IEEE_MSG_BASE (0x5800) /*22528 */

typedef enum
{
    IEEE_INIT_CFM = IEEE_MSG_BASE,     /* 22528 */
    IEEE_SEARCH_FEATURES_IND,          /* 22529 */
    IEEE_SEARCH_FEATURES_COMPLETE_IND, /* 22530 */
    IEEE_CONNECT_CFM,                  /* 22531 */
    IEEE_CTRL_CONNECT_IND,             /* 22532 */
    IEEE_DATA_CONNECT_IND,             /* 22533 */
    IEEE_ASSOCIATE_CFM,                /* 22534 */
    IEEE_DATA_TRANSFER_CFM,            /* 22535 */
    IEEE_ASSOCIATION_RELEASE_CFM,      /* 22536 */
    IEEE_ASSOCIATION_RELEASE_IND,      /* 22537 */
    IEEE_DISCONNECT_CFM,               /* 22538 */
    IEEE_DISCONNECT_IND,               /* 22539 */
    IEEE_ABORT_IND,                    /* 22540 */
    IEEE_GET_STATUS_CFM,               /* 22541 */
    IEEE_EXT_CONFIG_SUPPORT_CFM,       /* 22542 */
    IEEE_UPDATE_CONFIG_OBJECT_CFM,     /* 22543 */
    IEEE_GET_MDS_ATTRIBUTES_IND,       /* 22544 */

    /* Below given messages are for internal use by IEEE library. */

    /* Internal Messages Start */
    IEEE_ASSOCIATE_TIMER_IND,           /* 22545 */
    IEEE_CONFIGURATION_TIMER_IND,       /* 22546 */
    IEEE_ASSOCIATE_RELEASE_TIMER_IND,   /* 22547 */
    IEEE_MDS_CONF_EVENT_REPORT_TIMER_IND,/* 22548 */

    /* Internal Messages End */

    IEEE_MESSAGE_LAST
} IeeeMessageId;

#define IEEE_MSG_END IEEE_MESSAGE_LAST
#endif /* DO_NOT_DOCUMENT */


/*!
    \brief Confirmation response to IeeeInit().

    IEEE library instance has completed its initialisation.
*/
typedef struct
{
    /*! \brief IEEE Library initialisation status. */
    ieeeStatus status;
} IEEE_INIT_CFM_T;

/*!
    \brief Information about remote device supported features.

    This message is a response to IeeeSearchFeatures(). After all reported
    MDEPs \link IEEE_SEARCH_FEATURES_COMPLETE_T IEEE_SEARCH_FEATURES_COMPLETE
    \endlink is sent to the application.
*/
typedef struct
{

    /*! \brief Address of the remote device */
    bdaddr addr;

    /*! \brief MCAP Data End Point Identifier */
    CsrUint8 mdep;

    /*! \brief MDEP Data Type (Device Data Specialization code) */
    CsrUint16 type;

    /*! \brief MDEP role of the data endpoint (0 for Source and 1 for Sink)*/
    CsrUint16 role;
} IEEE_SEARCH_FEATURES_IND_T;

/*!
    \brief Confirmation response to IeeeSearchFeatures().
*/
typedef struct
{

    /*! \brief Status of the search request */
    sdp_search_status     status;

    /*! \brief Address of the remote device */
    bdaddr addr;

} IEEE_SEARCH_FEATURES_COMPLETE_IND_T;


/*!
    \brief Confirmation response to IeeeConnectReq(). This message is also sent as a
    final confirmation for the incoming connect request \link IEEE_CTRL_CONNECT_IND_T
    IEEE_CTRL_CONNECT_IND\endlink and \link IEEE_DATA_CONNECT_IND_T IEEE_DATA_CONNECT_IND\endlink.

    On success, the logical entity moves to 'Connected' state and a data link would
    have been created to the Manager device for specified logical entity.
*/
typedef struct
{
    /*! \brief Status of Connection setup */
    ieeeConStatus status;

    /*! \brief Device data specialization (Logical Entity) for which connection is being setup */
    ieeeLogEntityType logEntity;

    /*! \brief Boolean set to TRUE if this primitive is used as a final confirmation for an
    incoming connect request from remote device */
    tBool           managerInitiated;

    /*! \brief Sink associated with the control channel of the IEEE connection. It will be
    only valid if the connection status is 'ieee_con_success' and should be ignored otherwise.
    This sink identifies the remote Manager device with which the Agent device is connected */
    Sink            sink;

} IEEE_CONNECT_CFM_T;

/*! \brief Incoming Control Channel indication

    Remote HDP device has initiated connection attempt.

    Application should ensure that the paired device for which IEEE_CTRL_CONNECT_IND
    is received is comaptible with the remote manager device before accepting or rejecting
    the Control Channel the connection attempt. Compatible means share common supported
    device data specializations.

    Application shall use IeeeCtrlConnectResponse() to accept or reject the
    connection attempt.
*/
typedef struct
{
    /*! \brief Bluetooth address of the remote device */
    bdaddr          bd_addr;
    /*!
      \brief L2CAP Channel Identifier

      This is unique identifier for the connection attempt and should be
      copied directly into the response function.

      \note Application does not need to save this value as it is used only
      once in call to IeeeCtrlConnectResponse().
    */
    CsrUint16          cid;
} IEEE_CTRL_CONNECT_IND_T;

/*! \brief Received a connection indication from Manager device.
    The indication is transmitted to application for getting authorisation to accept
    or reject the connection.

    \note The indication is transmitted to application when data channel for supported
    device data specialization is being setup.

    \note The sequence is IEEE_DATA_CONNECT_IND is sent to application for getting
    authorisation to accept or reject the incoming data connection. The application
    responds by calling IeeeDataConnectRsp(), which will be followed by IEEE_CONNECT_CFM
    marking the completion of IEEE Connect procedure triggered by the Manager.
*/

typedef struct
{
    /*! \brief Sink associated with the control channel of the incoming IEEE connection.
    Application need not remember 'sink' and just need to pass it to IEEE library in
    IeeeDataConnectRsp().*/
    Sink                sink;

    /*! \brief Device data specialization (Logical Entity) for which connection setup
    is requested */
    ieeeLogEntityType logEntity;

    /*! \brief Reconnect flag is used internally by IEEE library and is of no significance
    to application. Application need not store this flag. This flag is passed back to
    IEEE library in IeeeDataConnectRsp() function. */
    tBool               reconnect;

} IEEE_DATA_CONNECT_IND_T;

/*! \brief Confirmation for IeeeExtConfigSupportReq() primitive.

    This confirmation indicates that IEEE library is updated with the table of extended
    configurations as given in IeeeExtConfigSupportReq() primitive. */

typedef struct
{
    /*! \brief Device data specialization (Logical Entity) for which status is reported. */
    ieeeLogEntityType       logEntity;

    /*! \brief IEEE Library confirmation status. */
    ieeeStatus              status;

} IEEE_EXT_CONFIG_SUPPORT_CFM_T;

/*! \brief Confirmation for IeeeUpdateConfigObjectReq() primitive.

    This confirmation indicates the status of Configuration Object information
    update to Manager device.*/
typedef struct
{

    /*! \brief Status of Config Object Update Request. */
    ieeeConfObjUpdateStatus status;

    /*! \brief Device data specialization (Logical Entity) for which status is reported. */
    ieeeLogEntityType       logEntity;

} IEEE_UPDATE_CONFIG_OBJECT_CFM_T;

/*! \brief Incoming GET MDS Command received from Manager device. This indication is received only
    once the agent device is associated to the manager device.

    Application should respond to this indication using IeeeGetMdsAttributesRsp() primitive.
    In response to this indication, application may pass the list of vendor specific and
    other standard attributes if requested by the Manager device. Refer to section 8.9.3.2
    of IEEE 11073-20601 specification for details.*/

/*! \note Handling of this primitive is mandatory for the application.*/

typedef struct
{
    /*! \brief Device data specialization (Logical Entity) for which GET MDS Commmand
    is received. */
    ieeeLogEntityType       logEntity;

    /*! \brief 'pAttrIdList' contains Attribute Id List. If set to NULL, all
    attributes (including vendor specific attributes) implemented by Agent
    need to be returned. While considering implemented attributes, ignore
    the below given attributes as IEEE library is aware of the latest value
    of these attributes through other primitives.*/

    /*! \brief MDC_ATTR_ID_HANDLE, MDC_ATTR_SYS_TYPE_SPEC_LIST,
    MDC_ATTR_ID_MODEL, MDC_ATTR_SYS_ID, MDC_ATTR_ID_PROD_SPECN,
    MDC_ATTR_DEV_CONFIG_ID, MDC_ATTR_POWER_STAT, MDC_ATTR_VAL_BATT_CHARGE,
    MDC_ATTR_TIME_BATT_REMAIN and MDC_ATTR_CONFIRM_TIMEOUT*/

    /*! \note The pointer value in 'pAttriIdList' needs to be returned to
    IEEE library in IeeeGetMdsAttributesRsp() primitive. Application
    need NOT free the memory allocated to this pointer.*/
    AttributeIdList         *pAttrIdList;

    /*! \brief Get MDS Command Invoke Id. This needs to be passed back to IEEE library
    in IeeeGetMdsAttributesRsp() primitive */
    CsrUint16                  mdsInvId;

} IEEE_GET_MDS_ATTRIBUTES_IND_T;

/*!
    \brief Confirmation response to IeeeAssociateReq().

    On success, the supported device data specialization moves to 'Operating' state.
    Data transmission can now be initiated to the Manager device.

    On failure, the supported device data specialization moves back to 'Unassociated'
    state.
*/

typedef struct
{
    /*! \brief Status of Association request. */
    ieeeAssocStatus status;

    /*! \brief Association Response data from the Manager like Manager System Id.
        Needs to be filled only if the status is Accepted */
    tIeeeAssoRspData rspData;

} IEEE_ASSOCIATE_CFM_T;

/*!
    \brief Confirmation response to IeeeDataTransferReq().

    On success, the supported device data specialization remains in 'Operating' state.

    On failure, the supported device data specialization moves to 'Unassociated' state.
*/

typedef struct
{
    /*! \brief Status of Data Transfer request. */
    ieeeDataTrsfStatus status;

    /*! \brief Device data specialization for which measurement data is transmitted */
    ieeeLogEntityType logEntity;

} IEEE_DATA_TRANSFER_CFM_T;

/*!
    \brief Confirmation response to IeeeAssociationReleaseReq().

    On success, the supported device data specialization moves to 'Unassociated' state.

    On failure, the supported device data specialization moves back to last device state.
*/

typedef struct
{
    /*! \brief Status of Association Release request. */
    ieeeAssociateRelStatus status;

} IEEE_ASSOCIATION_RELEASE_CFM_T;

/*!
    \brief Indication that Association is released by Manager device. The Agent responds to
    Association Release request by sending Association Release Response and moves to
    'Unassociated' state.
*/

typedef struct
{
    /*! \brief On-going IEEE procedure when Association Release indication is received.
    This could be of use to the application. Association can be released by the Manager
    when the Agent is in Associated state ie., Agent can be in Configuring or Operating
    sub-sates as per IEEE specification. */
    ieeeProcedureType       ongoingProc;

} IEEE_ASSOCIATION_RELEASE_IND_T;


/*!
    \brief Confirmation response to IeeeDisconnectReq().

    The specified device data specialization moves to 'Disconnected' state.
*/

typedef struct
{
    /*! \brief Status of Disconnect request. */
    ieeeDisconnectStatus status;

    /*! \brief Device data specialization (Logical Entity) that has been disconnected. */
    ieeeLogEntityType logEntity;

} IEEE_DISCONNECT_CFM_T;

/*!
    \brief Indication for the disconnect procedure triggered by Manager device or
    due to link-loss.

    The specified device data specialization moves to 'Disconnected' state.
*/

typedef struct
{
    /*! \brief Status of Disconnect procedure. */
    ieeeDisconnectStatus    status;

    /*! \brief Device data specialization (Logical Entity) that has been disconnected. */
    ieeeLogEntityType logEntity;

    /*! \brief On-going IEEE procedure when disconnect indication is received. This
    could be of use to the application. */
    ieeeProcedureType       ongoingProc;

} IEEE_DISCONNECT_IND_T;

/*!
    \brief Abort Indication is send to the application when an ABORT Indication is triggered
    by the Manager device or when the Agent device encounters an un-recoverable error
    and it needs to abort the IEEE association with Manager device.

    This indication is only sent when there is no on-going IEEE procedure, otherwise
    ABORT indication will get clubbed with the response of the on-going IEEE procedure.

    The IEEE Agent device for the specified device data specialization moves to 'Unassociated'
    state.
*/

typedef struct
{
    /*! \brief Status for Abort indication. */
    ieeeAbortStatus         status;

    /*! \brief Device data specialization (Logical Entity) that has been aborted. */
    ieeeLogEntityType       logEntity;

    /*! \brief On-going IEEE procedure when abort indication is received. This
    could be of use to the application. */
    ieeeProcedureType       ongoingProc;

} IEEE_ABORT_IND_T;

/*!
    \brief Confirmation response to IeeeGetStatusReq().
*/

typedef struct
{
    /*! \brief Device data specialization (Logical Entity) for which status is reported. */
    ieeeLogEntityType       logEntity;

    /*! \brief Current state of device data specialization (Logical Entity). */
    logEntityStateType      currentState;

    /*! \brief On-going IEEE procedure when getStatus request is received. */
    ieeeProcedureType       ongoingProc;

} IEEE_GET_STATUS_CFM_T;

/* Function Protypes */
/*!
    \brief Initialise IEEE library instance.
    \param app              The application task. This task will receive messages
                            from the IEEE library.
    \param logEntity        List of device data specializations \em ieeeLogEntityType supported by application.
    \param size_logEntity   Number of device data specializations supported by application.
    \param name             ServiceName to be registered in the SDP record.
    \param size_name        Length of the \em name parameter.

    \return Message \link IEEE_INIT_CFM_T IEEE_INIT_CFM\endlink is sent
    to the application task when operation has finished.

    \note Information on supported logical entities \em logEntList by
    application is static in nature and is not expected to change at run-time.

*/
/*void IeeeInit(Task app, ieeeLogEntityType *logEntity, CsrUint16 size_logEntity, CsrUint8 *name, CsrUint16 size_name);*/

/*!
    \brief Search Supported Features of the remote Manager device.
    \param deviceAddr       Bluetooth address of the remote Manager device.
    \param logEntity        Device data specialization <em>ieeeLogEntityType </em>
    \param role             MDEP role of the remote device
    <em>role</em> is a bitmap to filter results, where bit0 is source and bit1
    is sink.

    \return Message \link IEEE_SEARCH_FEATURES_IND_T IEEE_SEARCH_FEATURES_IND
    \endlink is sent to the application task for every found item in the
    remote MDEP list, and \link IEEE_SEARCH_FEATURES_COMPLETE_IND_T
    IEEE_SEARCH_FEATURES_COMPLETE_IND\endlink when the operation has finished.

    Search Supported features (MDEP list) from the remote device.
*/
void IeeeSearchFeatures(bdaddr *deviceAddr, ieeeLogEntityType logEntity, CsrUint16 role);

/*!
    \brief Initiate connection to the remote Manager device.
    \param deviceAddr       Bluetooth address of the remote Manager device.
    \param logEntity        Device data specialization <em>ieeeLogEntityType </em>
                            configured for IEEE communication.
    \param mdepIdentity     MCAP data end point identifier on the remote device to
                            which Agent device wants to set up logical IEEE data
                            link. MDEP ID corresponds to a device data specialization
                            and is published in the SDP record along with the supported
                            device data specialization.

    \return Message \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink is sent to
    the application task when operation has finished.

    \note IeeeConnectReq() leads to the creation of HDP Control and Data channels. HDP
    control channel facilitates the creation of Data channels and are not used for
    data communication for different device data specializations. For IEEE connection,
    successful creation of data channel for device data specialization is mandatory.

*/
void IeeeConnectReq(bdaddr *deviceAddr, ieeeLogEntityType logEntity, CsrUint8 mdepIdentity);

/*!
    \brief Update IEEE library with the device information like Power Status
    (Mains/Battery), Battery Level (%age) and Remaining Battery time
    \param deviceInfo       Pointer to the Agent device information structure

    \return No message is returned as this is just an Agent device information
    update request.

    \note IeeeUpdateDeviceInfoReq() is an optional request for Agent devices which
    want to share Power Status or Battery Information with the Manager devices.
*/

void IeeeUpdateDeviceInfoReq(tIeeeDeviceInfo *deviceInfo);

/*!
    \brief Update IEEE library with the extended configuration information.
    \param sink             Sink identifying the remote manager device as returned in
                            \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink
    \param logEntity        Device data specialization <em>ieeeLogEntityType </em>
                            configured for IEEE communication.
    \param pExtConfInfo     Pointer to the extended configuration information.

    \return Message \link IEEE_EXT_CONFIG_SUPPORT_CFM_T IEEE_EXT_CONFIG_SUPPORT_CFM \endlink is sent to the
    application task once the update is finished.

    \note This primitive is optional and should only be used if the extended
    configuration is supported by the device.

    \note This primitive is used prior to triggering Association procedure from the application.

    \note Application shall not free the pointer to extended configuration information
    till the Agent is associated.

*/
void IeeeExtConfigSupportReq(Sink sink, ieeeLogEntityType logEntity, const tIeeeExtConfInfo *pExtConfInfo);

/*!
    \brief Initiate IEEE Association procedure with the remote device.
    \param sink             Sink identifying the remote manager device as returned in
                            \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink
    \param tocermds         Timer value used to cover Data Transmission using confirmed event
                            reports. This timer value is in milliseconds. The timer will
                            not be started if set to Zero.

    \return Message \link IEEE_ASSOCIATE_CFM_T IEEE_ASSOCIATE_CFM\endlink is sent
    to the application task when operation has finished.

    \note For the time being only standard configuration is supported for
    Blood Pressure Monitor ie., IEEE 11073-10407 device data specialization.

    \note If required, this procedure will take care of sharing the configuration
    information of supported device data specialization with the Manager device.
*/
void IeeeAssociateReq(Sink sink, CsrUint16 tocermds);

/*!
    \brief Update the value of semi-static or dynamic attributes of configured objects.
    Like modifying attribute value for attribute id 'MDC_ATTR_UNIT_CODE' of 'Weight' object.
    from 'MDC_DIM_KILO_G' (Kg) to 'MDC_DIM_LB' (Lb)

    \param logEntity        Device data specialization <em>ieeeLogEntityType </em>
                            configured for IEEE communication.
    \param pConfigObj       Pointer to object information list
    \param size_pConfigObj  Size of the object list being modified

    \return Message \link IEEE_UPDATE_CONFIG_OBJECT_CFM_T IEEE_UPDATE_CONFIG_OBJECT_CFM \endlink is sent to the
    application task once the update is finished.

    \note This primitive is optional and should only be used if the configured object
    information needs to be modified.

    \note This primitive is used once the configuration is agreed between Agent and Manager
    device ie., when the Agent is in IEEE 'operating' state.

*/
void IeeeUpdateConfigObjectReq(ieeeLogEntityType logEntity, const ObjectInfo *pConfigObj, CsrUint16 size_pConfigObj);

/*!
    \brief Initiate IEEE Association Release procedure with the remote device.
    \param Sink             Sink identifying the remote manager device as returned in
                            \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink

    \return Message \link IEEE_ASSOCIATION_RELEASE_CFM_T IEEE_ASSOCIATION_RELEASE_CFM\endlink is sent
    to the application task when operation has finished.
*/
void IeeeAssociationReleaseReq(Sink sink);

/*!
    \brief Initiate connection release with the remote Manager device.
    \param Sink             Sink identifying the remote manager device as returned in
                            \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink
    \param logEntity        Device data specialization \em ieeeLogEntityType
                            marked for connection release.

    \return Message \link IEEE_DISCONNECT_CFM_T IEEE_DISCONNECT_CFM\endlink is sent to
    the application task when operation has finished.

    \note IeeeDisconnectReq() leads to the disconnect of HDP Data channel for the
    specified device data specialization. If this is the only configured device
    data specialization then HDP control channel will also be disconnected. Also,
    Disconnect Request will never fail.

*/

void IeeeDisconnectReq(Sink sink, ieeeLogEntityType logEntity);

/*!
    \brief Application response for the incoming control connection request
    \link IEEE_CTRL_CONNECT_IND_T IEEE_CTRL_CONNECT_IND\endlink from a remote device.

    \param deviceAddr           Bluetooth address of the remote Manager device as received in
                                \link IEEE_CTRL_CONNECT_IND_T IEEE_CTRL_CONNECT_IND\endlink message.
    \param cid                  Connection ID from \link IEEE_CTRL_CONNECT_IND_T IEEE_CTRL_CONNECT_IND
                                \endlink message.
    \param accept               Flag indicating whether incoming Control Connection should
                                be accepted (#TRUE) or rejected (#FALSE).

    \return If the application has accepted the incoming control connection,
    message it will be followed by Data Connection procedure from the remote
    manager device.

    When application gets message \link IEEE_CTRL_CONNECT_IND_T IEEE_CTRL_CONNECT_IND
    \endlink indicating incoming Control Channel it shall either accept or
    reject the connection using IeeeCtrlConnectRsp().
*/

void IeeeCtrlConnectRsp(bdaddr *deviceAddr, CsrUint16 cid, tBool accept);

/*!
    \brief Application response for the incoming data connection request
    \link IEEE_DATA_CONNECT_IND_T IEEE_DATA_CONNECT_IND\endlink from a remote device
    for a device data specialization.

    \param Sink                 Sink identifying the remote manager device as returned in
                                \link IEEE_DATA_CONNECT_IND_T IEEE_DATA_CONNECT_IND\endlink
    \param logEntity            Device data specialization \em ieeeLogEntityType
                                for which connection is requested.
    \param accepted             Boolean flag indicating whether the connection is accepted
                                or Rejected
    \param reconnect            Boolean flag indicating whether the remote device has
                                triggered reconnection to an existing MCAP data link.
                                This parameter is of no significance to the application
                                and should be passed on as it is from \link IEEE_DATA_CONNECT_IND_T
                                IEEE_DATA_CONNECT_IND\endlink message.

    \return If the application has accepted the incoming data connection,
    message \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink is sent to the
    application when the operation has finished.
*/
void IeeeDataConnectRsp(Sink sink, ieeeLogEntityType logEntity, tBool accepted, tBool reconnect);

/*!
    \brief Application response to incoming Get MDS Command message request \link
    IEEE_GET_MDS_ATTRIBUTES_IND_T IEEE_GET_MDS_ATTRIBUTES_IND \endlink from a remote
    manager device.

    \param pAttrIdList      Pointer to Attribute Id list as received in \link
                            IEEE_GET_MDS_ATTRIBUTES_IND_T IEEE_GET_MDS_ATTRIBUTES_IND \endlink
                            configured for IEEE communication.
    \param pAttrList        Pointer to Attribute information if there is any that needs to be
                            updated by application. If no attribute information needs to be passed,
                            this pointer should be set to NULL.
    \param mdsInvokeId      Invoke Id as received in \link IEEE_GET_MDS_ATTRIBUTES_IND_T
                            IEEE_GET_MDS_ATTRIBUTES_IND \endlink

    \return No message is returned as this message is sent in response to \link
    IEEE_GET_MDS_ATTRIBUTES_IND_T IEEE_GET_MDS_ATTRIBUTES_IND \endlink

    \note Handling of this primitive is mandatory for the application.

*/

void IeeeGetMdsAttributesRsp(const AttributeIdList *pAttrIdList, AttributeInfoList *pAttrList, CsrUint16  mdsInvokeId);

/*!
    \brief Initiate IEEE Data Transfer procedure with the remote device for the specified
    device data specialization.
    \param Sink                 Sink identifying the remote manager device as returned in
                                \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink
    \param logEntity            Device data specialization \em ieeeLogEntityType
                                for which measurement data needs to be transmitted.
    \param multiPersonSupport   Boolean - Whether measurements were taken in multiple-person
                                configuration of the device.
    \param measData             Measurement data in \em tIeeeBPMMeasData structure format for BPM

    \return Message \link IEEE_DATA_TRANSFER_CFM_T IEEE_DATA_TRANSFER_CFM\endlink is sent
    to the application task when operation has finished.
*/
void IeeeDataTransferReq(Sink sink, ieeeLogEntityType logEntity, tBool multiPersonSupport, void *measData);

/*!
    \brief Once connected, get the current status of the IEEE library for given device
    data specialization.
    \param Sink                 Sink identifying the remote manager device as returned in
                                \link IEEE_CONNECT_CFM_T IEEE_CONNECT_CFM\endlink
    \param logEntity            Device data specialization \em ieeeLogEntityType
                                for which Status needs to be sent back to application.

    \return Message \link IEEE_GET_STATUS_CFM_T IEEE_GET_STATUS_CFM\endlink is sent
    to the application task when operation has finished.

    \note This procedure is used mainly for trouble shooting.
*/
void IeeeGetStatusReq(Sink sink, ieeeLogEntityType logEntity);

#ifdef __cplusplus
extern "C" }
#endif

#endif

