#ifndef _CSR_BT_GATT_MAIN_H_
#define _CSR_BT_GATT_MAIN_H_

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_message_queue.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_util.h"
#include "csr_bt_tasks.h"
#include "att_prim.h"
#include "attlib.h"
#include "l2cap_prim.h"
#include "l2caplib.h"
#include "csr_bt_gatt_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_gatt_conn_genfsm.h"
#include "csr_list.h"

#ifdef __cplusplus
extern "C" {
#endif

/* CsrBtSdReadDeviceInfoReqSendEx context defines  */
#define CSR_BT_GATT_SD_READ_DEVICE_INFO_READ         0x0000   
#define CSR_BT_GATT_SD_READ_DEVICE_INFO_DELETE       0x0001

/* Special handle values */
#define CSR_BT_GATT_ATTR_HANDLE_START                ((CsrBtGattHandle) 0x0001)
#define CSR_BT_GATT_ATTR_HANDLE_DEVICE_NAME          ((CsrBtGattHandle)(CSR_BT_GATT_ATTR_HANDLE_START + 2))
#define CSR_BT_GATT_ATTR_HANDLE_GATT_UUID            ((CsrBtGattHandle)(CSR_BT_GATT_ATTR_HANDLE_START + 7))
#define CSR_BT_GATT_ATTR_HANDLE_SERVICE_CHANGED      ((CsrBtGattHandle)(CSR_BT_GATT_ATTR_HANDLE_START + 9))
#define CSR_BT_GATT_ATTR_HANDLE_END                  ((CsrBtGattHandle)(CSR_BT_GATT_ATTR_HANDLE_SERVICE_CHANGED + 1))

/* Different ATT Offset defines */ 
#define CSR_BT_GATT_ATT_READ_HEADER_LENGTH          (1)
#define CSR_BT_GATT_ATT_READ_BLOB_HEADER_LENGTH     (1)
#define CSR_BT_GATT_ATT_READ_BY_TYPE_HEADER_LENGTH  (4)
#define CSR_BT_GATT_ATT_WRITE_HEADER_LENGTH         (3)
#define CSR_BT_GATT_ATT_PREPARE_WRITE_HEADER_LENGTH (5)
#define CSR_BT_GATT_ATT_SIGNED_WRITE_HEADER_LENGTH (15)
#define CSR_BT_GATT_ATT_NOTIFICATION_HEADER_LENGTH  (3)

/* Spec defined MTUs */
#define CSR_BT_ATT_MTU_MAX                       (517)
#define CSR_BT_ATT_MTU_DEFAULT                    (23)

/* Local DB operations MTU */
#define CSR_BT_GATT_LOCAL_MAX_MTU                (0xFFFF)   

/* Define msg queue states */
#define CSR_BT_GATT_MSG_QUEUE_IDLE                          (0x00)
#define CSR_BT_GATT_MSG_QUEUE_QUEUED                        (0x01)
#define CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS                   (0x02)
#define CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_ACK               (0x03)
#define CSR_BT_GATT_MSG_QUEUE_CANCELLED                     (0x04)
#define CSR_BT_GATT_MSG_QUEUE_IGNORE_CHARAC_DESCRIPTOR      (0x05)
#define CSR_BT_GATT_MSG_QUEUE_EXECUTE_WRITE_CANCEL          (0x06)
#define CSR_BT_GATT_MSG_QUEUE_IN_PROGRESS_SECURITY          (0x07)

/* Define invalid conn param update identifier  */
#define CSR_BT_GATT_CONN_PARAM_UPDATE_IDENTIFIER_INVALID  (0x0000)

/* Define Sdp queue states */
#define CSR_BT_GATT_SDP_INIT                                (0x00) /* Have never call SDS Register */
#define CSR_BT_GATT_SDP_PENDING                             (0x01) /* Still needs to Register/Unregister the SDP record */ 
#define CSR_BT_GATT_SDP_REGISTERED                          (0x02) /* The SDP record is Registered */
#define CSR_BT_GATT_SDP_IN_PROGESS                          (0x04) /* The record currently being Process. Always just one */ 


/* Special appInst mode flags */
#define CSR_BT_GATT_MODE_ADVERTISE                (0x00000001) /* app wants to advertise */
#define CSR_BT_GATT_MODE_SCAN                     (0x00000002) /* app wants to scan */
#define CSR_BT_GATT_ADV_CONFIG_SET                (0x00000004) /* advertise paramters valid */
#define CSR_BT_GATT_SCAN_CONFIG_SET               (0x00000004) /* scan parameters valid */
#define CSR_BT_GATT_ADV_NAMELEN_THRES             (0x05) /* how many bytes required to send partial name */

/* Different attribute length and Indexes */ 
#define CSR_BT_GATT_CHARAC_DECLARATION_MIN_LENGTH       (5)
#define CSR_BT_GATT_CHARAC_DECLARATION_MAX_LENGTH      (19)
#define CSR_BT_GATT_CHARAC_PROPERTIES_LENGTH            (1)
#define CSR_BT_GATT_CHARAC_PROPERTIES_INDEX             (0)
#define CSR_BT_GATT_CHARAC_VALUE_HANDLE_LENGTH          (2) 
#define CSR_BT_GATT_CHARAC_VALUE_HANDLE_FIRST_INDEX     (1)
#define CSR_BT_GATT_CHARAC_UUID_FIRST_INDEX             (3)
#define CSR_BT_GATT_INCLUDE_WITH_UUID_LENGTH            (6)
#define CSR_BT_GATT_INCLUDE_WITHOUT_UUID_LENGTH         (4)
#define CSR_BT_GATT_INCLUDE_128_BIT_LENGTH             (20)
#define CSR_BT_GATT_INCLUDE_START_HANDLE_INDEX          (0)
#define CSR_BT_GATT_INCLUDE_END_HANDLE_INDEX            (2)
#define CSR_BT_GATT_INCLUDE_UUID_INDEX                  (4)  
#define CSR_BT_GATT_CHARAC_PRESENTATION_FORMAT_LENGTH   (7)
#define CSR_BT_GATT_SERVICE_CHANGED_LENGTH              (4)

/* Prepare/execute buffer state */
#define CSR_BT_GATT_PREPEXEC_NEW                        (0x00) /* new element */
#define CSR_BT_GATT_PREPEXEC_EXECUTE                    (0x01) /* ready for execute assesment */
#define CSR_BT_GATT_PREPEXEC_ACCESS                     (0x02) /* method: only access needed */
#define CSR_BT_GATT_PREPEXEC_ACCESS_AND_INFORM          (0x03) /* method: do access, then inform */
#define CSR_BT_GATT_PREPEXEC_INFORM_OK                  (0x04) /* method: only information "success" needed */
#define CSR_BT_GATT_PREPEXEC_INFORM_FAIL                (0x05) /* method: only information "failure" needed */
#define CSR_BT_GATT_PREPEXEC_DONE                       (0x06) /* all done, ready to remove element */

/* default CM events to subscribe for */
#define CSR_BT_GATT_DEFAULT_CM_EVENT_MASK ((CsrUint32)(CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY | \
                                                       CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOCAL_NAME_CHANGE | \
                                                       CSR_BT_CM_EVENT_MASK_SUBSCRIBE_BLUECORE_INITIALIZED))

/* Helper macros */
#define CSR_BT_GATT_GET_QID_MASK                    ((CsrBtGattId) 0x0000FFFF)
#define CSR_BT_GATT_CREATE_LE_CONN_ID(c)           ((CsrBtConnId) (CSR_BT_CONN_ID_LE_TECH_MASK | c))
#define CSR_BT_GATT_GET_CID_FROM_BTCONN_ID(c)      ((CsrUint16) (CSR_BT_CONN_ID_GET_MASK & c))
#define CSR_BT_GATT_CREATE_GATT_ID(_id, _queue)     ((CsrBtGattId) ((_id << 16) | _queue))
#define CSR_BT_GATT_GET_QID_FROM_GATT_ID(_gattId)   ((CsrUint16) (CSR_BT_GATT_GET_QID_MASK & _gattId))

#define CSR_BT_GATT_GET_HANDLE(source, index)    ((CsrBtGattHandle)(source[index] | source[index + 1] << 8))
#define CSR_BT_GATT_GET_L2CA_METHOD(f)           ((L2CA_CONNECTION_T)(((f) & L2CA_CONFLAG_ENUM_MASK)>>L2CA_CONFLAG_ENUM_OFFSET))
#define CSR_BT_GATT_GET_CONNINFO(conn)           ((CsrBtGattConnInfo)(L2CA_CONFLAG_IS_LE((conn)->l2capFlags) ? CSR_BT_GATT_CONNINFO_LE : CSR_BT_GATT_CONNINFO_BREDR))
#define CSR_BT_GATT_GET_SUPPLIER(_result)        ((CsrBtSupplier)((_result == ATT_RESULT_SUCCESS) ? CSR_BT_SUPPLIER_GATT : CSR_BT_SUPPLIER_ATT))

#define CSR_BT_GATT_GET_SERVICE_CHANGED_HANDLE_MASK         0x0000FFFF
#define CSR_BT_GATT_GET_SERVICE_CHANGED_FOUND_MASK          0x00010000
#define CSR_BT_GATT_GET_SERVICE_CHANGED_HANDLE(_eH)         ((CsrUint16) (CSR_BT_GATT_GET_SERVICE_CHANGED_HANDLE_MASK & _eH))
#define CSR_BT_GATT_CREATE_EXT_SERVICE_CHANGED_HANDLE(_hdl)    ((CsrUint32) (CSR_BT_GATT_GET_SERVICE_CHANGED_FOUND_MASK | _hdl))
        
/* Is connection in any of the connected states? */
#define CSR_BT_GATT_CONN_IS_CONNECTED(st) \
    (((st) == GATT_CONN_ST_CONNECTED))

#define CSR_BT_GATT_ATT_PRIM_UP_COUNT (ATT_DEBUG_IND - ATT_PRIM_UP)

typedef void (*CsrBtGattRestoreType)(void *gattInst, void *qElem, CsrUint16 mtu);
typedef void (*CsrBtGattCancelType)(void *gattInst, void *qElem, CsrBtResultCode result, CsrBtSupplier supplier);
typedef CsrBool (*CsrBtGattSecurityType)(void *gattInst, void *qElem, CsrBtResultCode result, CsrBtSupplier supplier);

/* The CsrBtGattCliConfigBits parameter in CsrBtGattSubscribeElement has 
   been extended to have two more values than the once the GATT spec defines.
   The reason for this is that GATT itself tries to keep track of the 
   Service Change Characteristic Value and the Client can subscribe
   for any handles */
#define CSR_BT_GATT_CHARAC_SERVICE_CHANGED ((CsrBtGattCliConfigBits) (CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION + 1))
#define CSR_BT_GATT_CHARAC_SUBSCRIBE       ((CsrBtGattCliConfigBits) (CSR_BT_GATT_CHARAC_SERVICE_CHANGED + 1))

/* The following rules apply to the 
 * CsrBtGattCliConfigBits parameter in the 
 * CsrBtGattSubscribeElement List are:
 *
 * 1: If Set to CSR_BT_GATT_CHARAC_SERVICE_CHANGED the 
 *    Element is remove by GATT if ATT_DISCONNECT_IND
 *    is received and the two devices are not bonded
 *
 * 2: If Set to CSR_BT_GATT_CLIENT_CHARAC_CONFIG_NOTIFICATION
 *    or to CSR_BT_GATT_CHARAC_SUBSCRIBE or to 
 *    CSR_BT_GATT_CLIENT_CHARAC_CONFIG_INDICATION the 
 *    Element is remove by GATT if:
 *      - ATT_DISCONNECT_IND is received and the two 
 *        devices are not bonded. 
 *      - A Service Changed Ind is received and the
 *        Affected Attribute Handle Range contains the
 *        Attribute handle of the Client Characteristic 
 *        Configuration declaration. 
 */
typedef struct CsrBtGattSubscribeElementTag
{
    struct CsrBtGattSubscribeElementTag        *next; /* must be 1st */
    struct CsrBtGattSubscribeElementTag        *prev; /* must be 2nd */
    CsrBtTypedAddr                              address; /* peer address */
    CsrBtGattHandle                             attrHandle; /* attribute handle subscribed for */
    CsrBtGattCliConfigBits                      configuration; /* client characteristic configuration bits */
} CsrBtGattSubscribeElement;

typedef struct CsrBtGattQueueElementTag
{
    struct CsrBtGattQueueElementTag            *next; /* must be 1st */
    struct CsrBtGattQueueElementTag            *prev; /* must be 2nd */
    CsrBtConnId                                 btConnId;
    CsrBtGattId                                 gattId;
    void                                       *gattMsg;
    CsrUint8                                    msgState;
    CsrBtGattHandle                             attrHandle;
    CsrUint8                                   *data;
    CsrUint16                                   dataOffset;
    CsrUint16                                   dataElemIndex;
    CsrBtGattRestoreType                        restoreFunc; 
    CsrBtGattCancelType                         cancelFunc;
    CsrBtGattSecurityType                       securityFunc;
} CsrBtGattQueueElement;

/* Optional application specific scan parameters */
typedef struct
{
    CsrBtGattScanFlags                          scanFlags; /* special scan flags */
    CsrUint16                                   scanInterval; /* scanning interval */
    CsrUint16                                   scanWindow; /* scanning window */
    CsrUint8                                    filterAddrCount; /* number of address filters */
    CsrBtTypedAddr                             *filterAddr; /* address filter. Empty means all */
    CsrUint8                                    filterDataCount; /* number of data filters */
    CsrBtGattDataFilter                        *filterData; /* data filters */
} CsrBtGattAppScanParams;

/* Optional application specific advertising parameters */
typedef struct
{
    CsrBtGattConnFlags                          advFlags; /* special advertise flags */
    CsrUint8                                    advertisingDataLength; /* max 31 */
    CsrUint8                                    advertisingData[CSR_BT_CM_LE_MAX_REPORT_LENGTH];
    CsrUint8                                    scanResponseDataLength; /* max 31 */
    CsrUint8                                    scanResponseData[CSR_BT_CM_LE_MAX_REPORT_LENGTH];
    CsrUint16                                   advIntervalMin; /* Advertising interval minimum */
    CsrUint16                                   advIntervalMax; /* Advertising interval maximum */
    CsrBtGattAdvChannel                         advertisingChannelMap; /* Advertising channel map */
} CsrBtGattAppAdvParams;

/* Optional application specific connection parameters */
typedef struct
{
    CsrUint16                                   scanInterval; /* Scan interval for central */
    CsrUint16                                   scanWindow; /* Scan window for central */
    CsrUint16                                   connIntervalMin; /* Connection interval minimum */
    CsrUint16                                   connIntervalMax; /* Connection interval maximum */
    CsrUint16                                   connLatency; /* Default connection latency */
    CsrUint16                                   supervisionTimeout; /* Default connection supervision timeout */
    CsrUint16                                   connAttemptTimeout; /* Connection attempt timeout */
    CsrUint16                                   advIntervalMin; /* Peripheral advertise interval min */
    CsrUint16                                   advIntervalMax; /* Peripheral advertise interval max */
    CsrUint16                                   connLatencyMax; /* Max acceptable connection latency */
    CsrUint16                                   supervisionTimeoutMin; /* Min acceptable supervision timeout */
    CsrUint16                                   supervisionTimeoutMax; /* Max acceptable supervision timeout */
} CsrBtGattAppConnParams;

typedef struct CsrBtGattSdpRecordElementTag
{
    struct CsrBtGattSdpRecordElementTag         *next;        /* must be 1st */
    struct CsrBtGattSdpRecordElementTag         *prev;        /* must be 2nd */
    CsrBtGattHandle                             startHandle;  
    CsrBtGattHandle                             endHandle;    
    CsrUint32                                   serviceRecordHandle;
    CsrUint8                                    state;
    CsrBtUuid                                   uuid;         /* Note the UUID is saved in BIG indian as SDP uses this */   
} CsrBtGattSdpRecordElement;

typedef struct CsrBtGattAppElementTag
{
    struct CsrBtGattAppElementTag              *next; /* must be 1st */
    struct CsrBtGattAppElementTag              *prev; /* must be 2nd */
    CsrBtGattId                                 gattId;
    CsrSchedQid                                 qid;
    CsrUint16                                   start; /* db start handle */
    CsrUint16                                   end; /* db end handle */
    CsrUint32                                   mode; /* special mode flags */
    CsrCmnList_t                                subscribeInst; /* subscribtion list */
    CsrBtGattEventMask                          eventMask; /* Event Mask */    

    /* Advertising/scan/connection parameters */
    CsrBtGattAppScanParams                     *scan; /* scan parameters */
    CsrBtGattAppConnParams                     *conn; /* connection parameters */
    CsrBtGattAppAdvParams                      *adv; /* advertising parameters */
} CsrBtGattAppElement;

/* Connection instance. One per ATT connection */
typedef struct CsrBtGattConnElementTag
{
    struct CsrBtGattConnElementTag             *next; /* must be 1st */
    struct CsrBtGattConnElementTag             *prev; /* must be 2nd */
    fsm16_state_t                               state; /* FSM state */
    CsrBtTypedAddr                              peerAddr; /* Peer address */
    l2ca_conflags_t                             l2capFlags; /* L2CAP connection flags */
    CsrUint32                                   flags; /* all flags */
    CsrBtConnId                                 btConnId; /* connection id and CID */
    CsrBtGattId                                 gattId; /* application owner for this connection */
    CsrUint16                                   mtu; /* local+remote mtu */
    CsrUint16                                   preferredMtu;
    CsrUint16                                   interval; /* Connection interval (in slots, i.e. x * 0.625ms) */
    CsrUint16                                   timeout; /* Supervision timeout (in 10ms units) */
    CsrUint16                                   latency;  /* Connection latency (in slots, i.e. x * 0.625ms) */
    CsrUint16                                   connIntervalMin; /* Min acceptable connection interval */
    CsrUint16                                   connIntervalMax; /* Max acceptable connection interval */    
    CsrUint16                                   connLatencyMax;  /* Max acceptable connection latency */
    CsrUint16                                   supervisionTimeoutMin; /* Min acceptable supervision timeout */
    CsrUint16                                   supervisionTimeoutMax; /* Max acceptable supervision timeout */
    CsrUint16                                   connParamUpdateId; /* The identifier of the conn update param procedure */
    CsrUint16                                   remoteNameLength; /* The length of the remote name */
    CsrUint8                                    *remoteName; /* The remote name */
} CsrBtGattConnElement;

typedef struct CsrBtGattPrepareBufferTag
{
    struct CsrBtGattPrepareBufferTag           *next; /* must be 1st */
    struct CsrBtGattPrepareBufferTag           *prev; /* must be 2nd */
    CsrUint16                                   cid; /* l2cap cid */
    CsrBtGattHandle                             handle;
    CsrUint8                                    state; /* CSR_BT_GATT_PREPEXEC_... */
    CsrUint16                                   offset;
    CsrUint16                                   dataLength;
    CsrUint8                                   *data;
} CsrBtGattPrepareBuffer;

/* Main instance */
typedef struct
{
    CsrBtGattId                                 privateGattId;
    CsrUtf8String                               *localName; /* local device name */
    void                                        *msg; /* current pending message, pointer */
    CsrCmnList_t                                queue; /* pending command queue list */
    CsrCmnList_t                                appInst; /* application list */
    CsrCmnList_t                                connInst; /* connection list */
    CsrCmnList_t                                sdpRecordInst; /* SDP service record list */
    CsrUint16                                   whitelistCount; /* number of addresses in whitelist */
    CsrUint16                                   whitelistIndex; /* current index in list in progress */
    CsrCmnList_t                                prepare; /* prepare write buffering */
    CsrBtTypedAddr                              *whitelist; /* whitelist array */
    void                                        *sdpSearchData;
    CsrBtConnId                                 sdpBtConnId;
    CsrUint16                                   connParamUpdateIdCounter;
} GattMainInst;

/* Connection arguments (result code) abstraction for the FSM */
typedef struct
{
    GattMainInst                               *inst;
    CsrBtResultCode                             result;
    CsrBtSupplier                               supplier;
} GattConnArgs;

/* Rattle the FSM */
#define GATT_FSMRUN(inst, evt, conn) \
    (void)fsm_16bit_run(&(csr_bt_gatt_conn_fsm), \
                        &((conn)->state), \
                        (evt), (inst), (conn))

typedef CsrBool (*CsrBtGattHandlerType)(GattMainInst *inst);
typedef void (*CsrBtGattAttHandlerType)(GattMainInst *inst);

#ifdef __cplusplus
}
#endif

#endif

