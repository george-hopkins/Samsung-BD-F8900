#ifndef CSR_BT_PHDC_AG_MAIN_H
#define CSR_BT_PHDC_AG_MAIN_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_pmem.h"
#include "bluetooth.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_phdc_ieee_asn.h"
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif
#include "csr_bt_phdc_ag_genfsm.h"
#include "csr_bt_phdc_ag_device_genfsm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Assertion helpers */
#if !defined(__KERNEL__) && defined(_DEBUG)
#include <assert.h>
#define CSR_BT_PHDC_AGENT_ASSERT(x) assert(x)
#endif
#ifndef CSR_BT_PHDC_AGENT_ASSERT
#define CSR_BT_PHDC_AGENT_ASSERT(x)
#endif

#define CSR_BT_PHDC_AG_MDS_OBJECT_HANDLE (0)
#define CSR_BT_PHDC_AG_MAX_MEAS_REPORTS   (25)
#define CSR_BT_PHDC_AG_STDCONFIG_PRSTAPDU_WS_LEN  (72)
#define CSR_BT_PHDC_AG_STDCONFIG_PRSTAPDU_BP_LEN  (136)
#define CSR_BT_PHDC_AG_MAX_OBJECT_HANDLES   (10)

typedef CsrUint16 CsrBtPhdcEventReportingType;
#define CSR_BT_PHDC_EVENT_REPORTING_TYPE_INVALID (0)
#define CSR_BT_PHDC_EVENT_REPORTING_TYPE_FIXED   (1)
#define CSR_BT_PHDC_EVENT_REPORTING_TYPE_VAR     (2)

/* Association attempts according to IEEE */
#define CSR_BT_PHDC_RC_ASSOC        3
/* Timeout values according to IEEE */
#define CSR_BT_PHDC_TO_ASSOC        (10 * CSR_SCHED_SECOND)
#define CSR_BT_PHDC_TO_CONFIG       (10 * CSR_SCHED_SECOND)
#define CSR_BT_PHDC_TO_CER_MDS      (3 * CSR_SCHED_SECOND)
#define CSR_BT_PHDC_TO_RELEASE      (3 * CSR_SCHED_SECOND)
#define CSR_BT_PHDC_BT_LINK_RELEASE (20 * CSR_SCHED_SECOND)

#define ANY_SIZE 3

typedef struct
{
    CsrUint16       attribId;            /*  Attribute identity. For standard attributes refer to IEEE 11073-20601 specification.
                                             Use attribute identity in the range 0xF040 to 0xF060 for vendor specific attributes. */
    CsrUint16       attribInfoLength;    /* Attribute Information as given for standard attributes defined in IEEE 11073-20601
                                            specification  'attrInfo' is of type suitable for attribute identity. As an example, for attribute id
                                            'MDC_ATTR_TIME_ABS', type used should be 'AbsoluteTime'. Please refer to Table 2 (MDS attributes)
                                            and Table 5 (Metric attributes) of IEEE 11073-20601 for details on standard MDS attributes and Metric
                                            attributes respectively. */
    CsrUint8        *attribInfo;

}CsrBtPhdcAgentAttribInfo;

typedef struct
{
    CsrUint16                   objClass;       /* Object class - refer to IEEE 11073-20601 for details of object class */
    CsrUint16                   objHandle;      /* Object Handle as given by the application to uniquely identify the objects.
                                                 * Handle value '0' is reserved for MDS object. Refer to IEEE 11073-20601 for details */
    CsrUint16                   dataValueLength;/* The number of elements in dataValue array given below. Basically, this number is
                                                 * same as the number of elements in the structure which the data format is representing.*/
    CsrUint16                   *dataValue;     /* 'data_value' array takes basic data format values given below for each
                                                 * element in the structure that it represents. */
                                                /* The value for supported basic data types are */
                                                /* #define ATTR_DATA_FORMAT_NU_VAL_OBS_BASIC 0x0100 (for uint16) */
                                                /* #define ATTR_DATA_FORMAT_NU_CMPD_VAL_OBS_BASIC 0x0200 (for array of uint16),Lower byte
                                                 * indicates number of array elements.
                                                 * ex : 0x0202, indicates array of two elements of size uint16*/
                                                /* #define ATTR_DATA_FORMAT_NU_VAL_OBS_SIMP 0x0400 (for uint32) */
                                                /* #define ATTR_DATA_FORMAT_NU_CMPD_VAL_OBS_SIMP 0x0800 (for array of uint32),Lower byte
                                                 * indicates number of array elements.
                                                 * ex : 0x0802, indicates array of two elements of size uint32*/
                                                /* #define ATTR_DATA_FORMAT_NU_VAL_OBS 0x1000 (for NuObsValue) */
                                                /* #define ATTR_DATA_FORMAT_NU_CMPD_VAL_OBS 0x2000 (for array of NuObsValue),Lower byte
                                                 * indicates number of array elements.
                                                 * ex : 0x2002, indicates array of two elements of size NuObsValue*/
                                                /* #define ATTR_DATA_FORMAT_ABSOLUTE_TIME 0x0001 (for Absolute Time) */
                                                /* note Float_Type and SFloat_Type are represented by uint32 and uint16 basic
                                                 * data types respectively. */
    CsrUint16                   numAttrib;  /* Number of attributes of the object */
    CsrUint16                   attribIndex;
    CsrBtPhdcAgentAttribInfo    *pAttribList;
}CsrBtPhdcAgentObjectInfo;


typedef struct
{
    CsrUint16 commandApduLength;
    CsrUint8 *apdu;
}CsrBtPhdcTempApdu;

typedef struct
{
    /* Main */
    void                        *pRecvMsg;       /* Receive message */
    CsrSchedQid                 appHandle;
    BD_ADDR_T                   local_bd_addr;
    fsm_state_t                 state;
    fsm_state_t                 deviceState;
    CsrSchedTid                 timerId;         /* Timer used by 20601 */
    
    DATA_apdu                   *pGetResultSimple;
    DATA_apdu                   *pRoivCmfdEventReport;
    ScanReportInfoFixed         *pScanReportInfoFixed;
    ScanReportInfoVar           *pScanReportInfoVar;
    ScanReportInfoMPFixed       *pScanReportInfoMPFixed;
    ScanReportInfoMPVar         *pScanReportInfoMPVar;
    CsrBtPhdcEventReportingType eventReportingType;
    CsrUint16                   personId;
    CsrBool                     waitingForDataCfm;
    
    CsrBtPhdcTempApdu           tempApdu;  /* Used to store APDU for retransmissions */
    DATA_apdu                   *pEventReportNotifyConfig;
    APDU                        *pApdu;
    
    octet_string                systemId; /* Received in CsrBtPhdcAgConfigureMdsReq */

    CsrUint16                   devConfigId;
    CsrUint16                   objCount;
    CsrUint16                   objIndex;
    CsrUint16                   objHandles[CSR_BT_PHDC_AG_MAX_OBJECT_HANDLES];
    CsrBtPhdcAgentObjectInfo    *pObjectList;
    CsrBool                     exhaustedAllConfig; /* release param received in CsrBtPhdcAgAssociateReq */
    
    CsrUint32                   assocChannelId;
    CsrUint32                   psmIdentifier;
    CsrBtMdepId                 mdepId;
    CsrUint16                   mdepDataType;
    CsrUint16                   maxPacketLength;
    CsrUint32                   dataChannelId;
    
    CsrUint8                    rcAssoc;
    CsrUint16                   associationResult; /* Received in AARE  + other errors due to timer expiry etc*/
    CsrBool                     associateReq;
    CsrBool                     localDisassociateReq;
   
    CsrBool                     sendConfigAgain;
    ConfigReport                configReport;
    CsrUint16                   invokeId;
    CsrUint8                    transmitFlag; /* Received in CsrBtPhdcAgDataReq */
    CsrUint16                   scanReportNo;

    CsrUint16                   localUnitCodeReqObjHandle;
    CsrBool                     localUnitCodeReq;

    CsrUint8                    stdConfigPrstApduWs[CSR_BT_PHDC_AG_STDCONFIG_PRSTAPDU_WS_LEN]; /* Contains the PRST APDU for standard config - weighing scale */
    CsrUint8                    stdConfigPrstApduBp[CSR_BT_PHDC_AG_STDCONFIG_PRSTAPDU_BP_LEN]; /* Contains the PRST APDU for standard config - Blood Pressure */
    
    
} CsrBtPhdcAgentInstance;


/* Prototypes */
extern void CsrBtPhdcAgentHandler(void **gash);

/* Prototype for autogenerated helper function */
extern void CsrBtPhdcAgFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_PHDC_AG_MAIN_H */
