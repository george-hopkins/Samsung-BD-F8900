#ifndef CSR_BT_CM_MAIN_H__
#define CSR_BT_CM_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "csr_message_queue.h"
#include "l2cap_prim.h"
#include "bluestacklib.h"
#include "bkeyval.h"

#include "csr_bt_sc_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_profiles.h"
#include "csr_bt_util.h"
#include "csr_bt_cmn_linked_list.h"
#include "csr_list.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* L2CAP's default is 3 queues, which leaves the lowest priority at 2,
 * so cap at this value */
#define CSR_BT_CM_PRIORITY_LOW (2)

#ifdef CSR_LOG_ENABLE
void CsrBtCmChangeState(const CsrCharString *file, CsrUint32 line,
                              const CsrCharString *fsmName,
                              CsrUint32 *state, CsrUint32 newState, const CsrCharString *newStateName);

#define CSR_BT_CM_STATE_CHANGE(state, newState) CsrBtCmChangeState((CsrCharString*) __FILE__, __LINE__, (CsrCharString*) #state, &(state), newState, (CsrCharString*) #newState)
#else
#define CSR_BT_CM_STATE_CHANGE(state, newState) (state = newState)

#endif


#ifdef CSR_LOG_ENABLE
void CsrBtCmChangePlayer(char *file, CsrUint32 lineno, CsrUint8 *player, char *playerName, CsrUint8 newPlayer, char *newPlayerName);
#define CM_CHANGE_PLAYER(player, newPlayer) CsrBtCmChangePlayer(__FILE__, __LINE__, (player), #player, (newPlayer), #newPlayer)
#else
#define CM_CHANGE_PLAYER(player, newPlayer) (*(player) = newPlayer)
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#define CsrBtCmStateEventException(prim, type, state) \
    CsrStateEventException("CM", (prim), (CsrUint16) (type), (CsrUint16) (state))

#define CsrBtCmGeneralException(prim, type, str) \
    CsrGeneralException("CM", (prim), (CsrUint16) (type), (str))

#define CsrBtCmGeneralWarning(prim, type, str) \
    CsrGeneralWarning("CM", (prim), (CsrUint16) (type), (str))

#define CsrBtCmStateEventExceptionOn(cond, prim, type, state)        \
    do {                                                            \
        if ((cond))                                                 \
            CsrStateEventException("CM", (prim), (type), (state)); \
    } while(0)

#else
#define CsrBtCmStateEventException(a, b, c)
#define CsrBtCmGeneralWarning(a, b, c)
#define CsrBtCmStateEventExceptionOn(a, b, c, d)
#define CsrBtCmGeneralException(a, b, c)
#endif

#define CsrBtCmAssertRetVal(cond, retval)                            \
    do {                                                            \
        if (!(cond))                                                \
        {                                                           \
            CsrBtCmGeneralException(0, 0, #cond);                    \
            return (retval);                                        \
        }                                                           \
    } while(0)

#define CsrBtCmAssertRet(cond)                                       \
    do {                                                            \
        if (!(cond))                                                \
        {                                                           \
            CsrBtCmGeneralException(0, 0, #cond);                    \
        }                                                           \
    } while(0)


#define MILLI_TO_BB_SLOTS(a) ((CsrUint16) (((a) * CSR_SCHED_MILLISECOND) / 625))

#define CM_ERROR                                      (255)
#define RFC_PLAYER                                    (0x00)
#define L2CAP_PLAYER                                  (0x01)
#define BNEP_PLAYER                                   (0x02)
#define SDC_PLAYER                                    (0x03)
#define SC_PLAYER                                     (0x04)
#define CM_PLAYER                                     (0x05)
#define CM_LINK_POLICY_PLAYER                         (0x06)
#define CM_ROLE_SWITCH_PLAYER                         (0x07)
#define APP_PLAYER                                    (0x08)
#define RNR_PLAYER                                    (0x09)
#define AMP_PLAYER                                    (0x0A)
#define LE_PLAYER                                     (0x0B)
#define UNDEFINED_PLAYER                              (0x0C)
#define KEEP_CURRENT_PLAYER                           (0xFF)

#define MAX_PCM_SLOTS                                 (4)

/* Defines Sniff values         */
#define CM_HCI_MAX_SNIFF_INTERVAL                    (0xFFFE)
#define CM_HCI_MIN_SNIFF_INTERVAL                    (0x0002)
#define CM_HCI_MAX_SNIFF_ATTEMPT                     (0x7FFF)
#define CM_HCI_MIN_SNIFF_ATTEMPT                     (0x0001)
#define CM_HCI_MAX_SNIFF_TIMEOUT                     (0x7FFF)
#define CM_HCI_SNIFF_DRAWBACK                        (0x0010)

/* Defines for DM cache handling */
#define CM_DM_CACHE_FLUSH_TIMEOUT                     (25000000)
#define CM_DM_CACHE_PARAM_POOL_SIZE                   (10)
#define CM_DM_MAX_NUM_OF_CACHE_POOLS                  (10)
#define CM_DM_MAX_SEQ_NUMBER                          (65535)

#define NUM_OF_ACL_CONNECTION                         (8)

/* Defines for link policy state */
#define LINK_POLICY_IDLE_STATE                        (0)
#define LINK_POLICY_CONNECT_OR_DONE_STATE             (1)
#define LINK_POLICY_WRITE_LINK_POLICY_STATE           (2)
#define LINK_POLICY_ROLE_SWITCH_STATE                 (3)
#define LINK_POLICY_INIT_RFC_SCO_STATE                (4)
#define LINK_POLICY_APP_CONTROL_STATE                 (5)
#define LINK_POLICY_AMP_CONTROL_STATE                 (6)

/* Defines for RFC CONNECT state */
#define CM_RFC_IDLE                                   (0)
#define CM_RFC_CONNECTED                              (1)
#define CM_RFC_CANCELING                              (2)
#define CM_RFC_SSP_REPAIR                             (3)

/* Defines for L2CA connect State */
#define CM_L2CA_IDLE                                  (0)
#define CM_L2CA_CONNECT                               (1)
#define CM_L2CA_CONNECT_PENDING                       (2)
#define CM_L2CA_SSP_REPAIR                            (3)
#define CM_L2CA_CANCELING                             (4)

/* Defines for L2CA connection context */
#define CM_L2CA_INCOMING_CONNECT_REJECTED_CTX         (0)
#define CM_L2CA_CONNECT_INPROGRESS_CTX                (1)
#define CM_L2CA_RECONFIG_INPROGRESS_CTX               (2)

/* Defines for BNEP connect State */
#define CM_BNEP_IDLE                                  (0)
#define CM_BNEP_CONNECT                               (1)
#define CM_BNEP_SSP_REPAIR                            (2)

/* Defines for SSR */
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
#define CM_SSR_DISABLED                               (0x0)
#define CM_SSR_MAX_LATENCY_VALUE                      (0xFFFE)
#define SSR_MAX_LATENCY_DONT_CARE                     (0xFFFF)
#define SSR_MIN_REMOTE_TIMEOUT_DONT_CARE              (0xFFFF)
#define SSR_MIN_LOCAL_TIMEOUT_DONT_CARE               (0xFFFF)
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */

#define SSP_REPAIR_DELAY                              (300000)

/* Defines for low power settings */
#define DEFAULT_PRIORITY                              (255)
#define SNIFF_MAX_INTERVAL                            (0xFFFF)
#define SNIFF_MIN_INTERVAL                            (0xFFFF)

/* SCO related defines*/
#define SCO_PACKET_RESERVED_BITS                      (0xFC00)
#define HCI_ESCO_PKT_COUNT                            (0x0A)

/* CM inquiry states */
#define CM_INQUIRY_APP_STATE_IDLE                     (0)
#define CM_INQUIRY_APP_STATE_INQUIRING                (1)       /* An inquiry is in progress */
#define CM_INQUIRY_APP_STATE_RESTARTING               (2)       /* An inquiry is pending */

#define CM_INQUIRY_DM_STATE_IDLE                      (0)
#define CM_INQUIRY_DM_STATE_INQUIRING                 (1)       /* Performing inquiry */
#define CM_INQUIRY_DM_STATE_CANCELLING                (2)       /* Cancelling inquiry */
#define CM_INQUIRY_DM_STATE_SETTING_INQUIRY_POWER_LEVEL (3)     /* Setting Inquiry TX power level */
#define CM_INQUIRY_DM_STATE_SET_LOW_PRIORITY          (4) 
#define CM_INQUIRY_DM_STATE_SCAN_ALLOWED              (5) 

/* Defines for CM switch role state */
#define CM_SWITCH_ROLE_IDLE                           (0x00)
#define CM_SWITCH_ROLE_ALLOW_MSS                      (0x01)
#define CM_SWITCH_ROLE_DO                             (0x02)
#define CM_SWITCH_ROLE_RESTORE_MSS                    (0x03)

/* CM modem status states */
#define CSR_BT_CM_MODEM_STATUS_IDLE                   (0x00)
#define CSR_BT_CM_MODEM_STATUS_PENDING                (0x01)
#define CSR_BT_CM_MODEM_STATUS_QUEUED                 (0x02)


/* Write CoD */
#define CM_COD_NONE                                   (0)
#define CM_COD_IN_PROGRESS                            (1)

/* Invalid L2CAP extended features cache */
#define CM_INVALID_L2CAP_EXT_FEAT                     ((CsrUint32)0xFFFFFFFF)

/* Define for InquiryMask                               */
#define CSR_BT_CM_INQUIRY_MASK_SET_NONE             0x0000
#define CSR_BT_CM_INQUIRY_MASK_SET_TRANSMIT_POWER   0x0001
#define CSR_BT_CM_INQUIRY_MASK_SET_PRIORITY_LEVEL   0x0002

/* Define for Inquiry Priority levels                   */
#define CSR_BT_CM_DEFAULT_INQUIRY_LEVEL               0x00
#define CSR_BT_CM_LOW_PRIORITY_INQUIRY_LEVEL          0x01
#define CSR_BT_CM_UNDEFINED_PRIORITY_INQUIRY_LEVEL    0xFF

/* AMP/BR-EDR controller activity */
#define CTRL_ACTIVE_AMP_ONLY                          0
#define CTRL_ACTIVE_BREDR_ACTIVE                      1
#define CTRL_ACTIVE_INACTIVE                          2

/* AMP/BR-EDR controller processState */
#define CSR_BT_CM_AMP_PROCESS_IDLE                    0
#define CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING   1
#define CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING   2

/* Default HCI Quality of Service parameters                    */
#define CSR_BT_CM_DEFAULT_QOS_SERVICE_TYPE        (HCI_QOS_BEST_EFFORT) /* Valid values are HCI_QOS_BEST_EFFORT, HCI_QOS_NO_TRAFFIC, HCI_QOS_GUARANTEED */
#define CSR_BT_CM_DEFAULT_QOS_TOKEN_RATE          (0x00000000)          /* Token Rate in bytes per second       */
#define CSR_BT_CM_DEFAULT_QOS_PEAK_BANDWIDTH      (0x00000000)          /* Peak Bandwidth in bytes per second   */
#define CSR_BT_CM_DEFAULT_QOS_LATENCY             (0x000061A8)          /* Latency in micro seconds             */
#define CSR_BT_CM_DEFAULT_QOS_DELAY_VARIATION     (0x000061A8)          /* Delay Variation in micro seconds     */

#define CSR_BT_CM_INVALID_LMP_VERSION               0xFF

#define CM_CREATE_RFC_CONN_ID(c)                    ((CsrBtConnId) (CSR_BT_CONN_ID_RFCOMM_TECH_MASK | c))
#define CM_CREATE_L2CA_CONN_ID(c)                   ((CsrBtConnId) (CSR_BT_CONN_ID_L2CAP_TECH_MASK | c))
#define CM_GET_UINT16ID_FROM_BTCONN_ID(c)           ((CsrUint16) (CSR_BT_CONN_ID_GET_MASK & c))

/* States */
#define idle_s                                        0x00
#define connectInit_s                                 0x01
#define connect_s                                     0x02
#define access_s                                      0x03
#define connectAble_s                                 0x04
#define connectAccept_s                               0x05
#define connectAcceptFinal_s                          0x06
#define cancelConnectAble_s                           0x07
#define releaseInit_s                                 0x08
#define release_s                                     0x09
#define releaseFinal_s                                0x0A
#define timeOut                                       0x0B
#define connected_s                                   0x0C
#define cancelTimer_s                                 0x0D
#define notReady_s                                    0x0E
#define l2capConnect_s                                0x0F
#define l2capConnectInit_s                            0x10
#define l2capConnectAccept_s                          0x11
#define l2capConnectAcceptFinal_s                     0x12
#define l2capSaveDisconnect_s                         0x13
#define l2capReleaseInit_s                            0x14
#define l2capReleaseInitXover_s                       0x15
#define l2capRelease_s                                0x16
#define l2capReleaseFinal_s                           0x17
#define l2capConnectAble_s                            0x18
#define l2capConnected_s                              0x19
#define l2capCancelConnectAble_s                      0x1A
#define l2capLegacyDetach_s                           0x1B
#define bnepConnect_s                                 0x1C
#define bnepConnectInit_s                             0x1D
#define bnepConnectCfm_s                              0x1E
#define bnepConnectAccept_s                           0x1F
#define bnepConnectAcceptFinal_s                      0x20
#define bnepCancelConnectAccept_s                     0x21
#define bnepDisconnectInit_s                          0x22
#define bnepDisconnectReq_s                           0x23
#define bnepDisconnectInd_s                           0x24
#define bnepConnected_s                               0x25
#define bnepSearchInit_s                              0x26
#define bnepSearch_s                                  0x27
#define sdcSearchInit_s                               0x28
#define sdcSearch_s                                   0x29
#define sdcAttr_s                                     0x2A
#define sdcClose_s                                    0x2B
#define scNull_s                                      0x2C
#define scConnect_s                                   0x2D
#define scConnectInit_s                               0x2E

typedef struct
{
    CsrCmnListDataElm_t          *next; /* Do not move - used for common linked list handling */
    CsrCmnListDataElm_t          *prev; /* Do not move - used for common linked list handling */
    CsrSchedQid                   appHandle;
    CsrBtDeviceAddr               addr;
} remoteVersionReq;

typedef struct {
    CsrUtf8String                *currentName;               /* Current local name */
    CsrUint8                     *services;                  /* Locally registered service records with service UUIDs */
    CsrUint16                     servicesOctetsUsed;
    CsrUint16                     manufacturerDataSettings;
    CsrUint8                      manufacturerDataLength;
    CsrUint8                     *manufacturerData;          /* Requested manufacturer data */
    CsrUint8                      flags;                     /* EIR flags */

    /* Temporary storage of parameters between request and a successful confirm */
    CsrUtf8String                *requestedName;             /* NULL-terminated */
    CsrUint16                     requestedServicesLength;
    CsrUint8                     *requestedServices;
    CsrBool                       requestedServicesNew;
} localEirData_t;

typedef struct
{
    CsrBool                       dataResReceived;
    CsrUint16                     restoreCount;
    CsrUint16                     saveCount;
    CsrUint8                      txCount;
    CsrBool                       dataCfmPending;
    CsrBool                       pendingBufferStatus;
    void                          *receivedBuffer[CSR_BT_CM_INIT_CREDIT];
} dataParameters;

typedef CsrUint32 cm_event_mask_t;

typedef struct
{
    CsrSchedQid                   appHandle;
    CsrBtCmEventMask              eventMask;
    CsrBtCmEventMaskCond          condition;
} subscribeParms;

typedef struct
{
    CsrSchedQid                   appHandle;
    CsrUint8                      map[10];
} afhMapParms_t;

typedef struct
{
    CsrSchedQid                   appHandle;
    CsrUint16                     timeout;
    CsrBtDeviceAddr               deviceAddr;
} cmLstoParms_t;

typedef struct
{
    hci_pkt_type_t                packetTypes[HCI_ESCO_PKT_COUNT];
    CsrBtCmScoCommonParms         parms;
    CsrUint16                     count;
    CsrUint16                     index;
} cmSyncNegotiationType_t;

typedef struct
{
    cmSyncNegotiationType_t      *negotiateTypes;
    CsrUint16                     count;
    CsrUint16                     index;
} cmSyncNegotiationCntType_t;

typedef struct
{
    CsrBool                       incoming;
    hci_connection_handle_t       handle;
    CsrUint32                     txBdw;
    CsrUint32                     rxBdw;
    CsrUint16                     maxLatency;
    CsrUint16                     voiceSettings;
    CsrUint8                      reTxEffort;
    hci_pkt_type_t                packetType;
    CsrUint8                      pcmSlot;
    cmSyncNegotiationCntType_t   *negotiateCnt;
    CsrUint8                      linkType;
    CsrUint8                      txInterval;
    CsrUint8                      weSco;
    CsrUint8                      reservedSlots;
    CsrUint16                     rxPacketLength;
    CsrUint16                     txPacketLength;
    CsrUint8                      airMode;
} eScoParmVars;

typedef struct
{
    CsrSchedQid                   appHandle;
    CsrUint24                     classOfDevice;
    CsrBtConnId                   btConnId;
    CsrUint8                      serverChannel;
    CsrUint8                      remoteServerChan;
    CsrBtDeviceAddr               deviceAddr;
    CsrUint16                     profileMaxFrameSize;
    CsrUint16                     maxLinkFrameSize;
    CsrBool                       timerFlag;
    CsrSchedTid                   timerId;
    CsrUint32                     state;
    CsrUint8                      actualMode;
    CsrUint8                      requestedMode;
    CsrBool                       requestPortPar;
    CsrBool                       validPortPar;
    RFC_PORTNEG_VALUES_T          portPar;
    dataParameters                dataControl;
    void                         *controlSignalQueue;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    eScoParmVars                  eScoParms;
#endif
    CsrBtSniffSettings            sniffSettings;
    CsrBtParkSettings             parkSettings;
    CsrUint8                      lowPowerPriority;
    CsrUint16                     context;
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    CsrBtSsrSettingsDownstream    ssrSettings;
    CsrBool                       ssrAccepted;
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
    CsrBtLogicalChannelType       logicalChannelTypeMask;
#ifdef CSR_AMP_ENABLE
    l2ca_identifier_t             moveIdentifier;
    CsrBtAmpController            controller;
    CsrUint8                      ampProcessState;
#endif
    CsrUint8                      modemStatus;
    CsrUint8                      modemStatusState;
    CsrUint8                      signalBreak;
    CsrUint8                      mscTimeout;
    CsrBtConnId                   pendingBtConnId;
} cmRfcConnInstType;

typedef struct CmRfcConnTag
{
    struct CmRfcConnTag          *next;          /* Do not move - used for common linked list handling */
    struct CmRfcConnTag          *prev;          /* Do not move - used for common linked list handling */
    CsrUint8                     elementId;
    cmRfcConnInstType            *cmRfcConnInst;
} cmRfcConnElement;

typedef struct
{
    CsrUint8                 serverChannel;
    CsrBool                       inUse;
} serverChanTable;

typedef struct
{
    CsrBool                       rfcStartRestoreFromQueue;
    CsrUint8                      rfcTimeOutIgnoreCounter;
    CsrBool                       unhandleTimeOut;
    CsrUint16                     operatingFrameSize;
    CsrUint16                     connectAcceptTimeOut;
    CsrUint8                      localServerChListSize;
    CsrBool                       cancelConnect;
    CsrUint32                     connectState;
    cmRfcConnElement              *connElement;
    serverChanTable               *localServerChList;
    CsrCmnList_t                  connList;
    CsrUint8                      elementCounter;
    CsrBtConnId                   btConnId;
} rfcVariables;

typedef struct
{
    CsrSchedQid                   appHandle;
    CsrUint32                     state;
    CsrUint8                      extFeatMask;
    CsrBool                       authorised;
    psm_t                         psm;
    psm_t                         remotePsm;
    CsrBtConnId                   btConnId;
    CsrMblk                      *combine;          /* Partial L2CAP data read resegment buffer */

    /* Tx and Rx queues */
    CsrMessageQueueType          *rxQueue;          /* Receive buffer queue */
    CsrUint16                     rxQueueCount;     /* Total number of waiting/unacked messages */
    CsrUint16                     rxQueueMax;       /* Max number of elements we allow */
    CsrUint16                     rxQueueOverflow;  /* Number of overflowed indications */
    CsrUint16                     rxQueueOverpack;  /* Cummulative overflowed 'packets' counter */
    CsrUint16                     rxAppCredits;     /* Number of acks application has given us */
    CsrBool                       rxBusy;           /* Has RnR been sent? (only for ERTM) */

    CsrUint16                     txCount;          /* Current number of elements pending in L2CAP */
    CsrUint16                     txMaxCount;       /* Max number of pending elements */
    CsrUint16                     txPendingContext; /* Context for blocked element */

    /* L2CAP configuration */
    CsrBool                       ertm;
    BKV_ITERATOR_T                conftabIter;
    CsrBtDeviceAddr               deviceAddr;
    l2ca_mtu_t                    outgoingMtu;
    l2ca_mtu_t                    incomingMtu;
    l2ca_mtu_t                    incomingMps;
    CsrUint16                     outgoingFlush;

    CsrUint24                     classOfDevice;
    CsrUint8                      actualMode; /* low power */
    CsrUint8                      requestedMode; /* low power */
    CsrBtSniffSettings            sniffSettings;
    CsrBtParkSettings             parkSettings;
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    CsrBtSsrSettingsDownstream    ssrSettings;
    CsrBool                       ssrAccepted;
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
#ifdef CSR_AMP_ENABLE
    l2ca_identifier_t             moveIdentifier;
    CsrBtAmpController            controller;
    CsrUint8                      ampProcessState;  
#endif
    CsrUint8                      lowPowerPriority;
    CsrBtLogicalChannelType       logicalChannelTypeMask;
    CsrUint16                     dataPriority;
    dm_security_level_t           secLevel;
    CsrUint16                     context;
    CsrBool                       pendingBufferStatus;
} cmL2caConnInstType;

typedef struct CmL2caConnTag
{
    struct CmL2caConnTag         *next;          /* Do not move - used for common linked list handling */
    struct CmL2caConnTag         *prev;          /* Do not move - used for common linked list handling */
    CsrUint8                     elementId;
    cmL2caConnInstType           *cmL2caConnInst;
} cmL2caConnElement;

#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
typedef struct cmL2caConnlessTag
{
    CsrCmnListDataElm_t          *next;          /* Do not move - used for common linked list handling */
    CsrCmnListDataElm_t          *prev;          /* Do not move - used for common linked list handling */
    CsrBtDeviceAddr               deviceAddr;    /* Peer address */
    psm_t                         psm;           /* Connectionless PSM */
    CsrMessageQueueType          *queue;         /* Tx queue while waiting for ACL */
    CsrBtConnId                  btConnId;       /* L2CA_CID_INVALID for channels being mapped */
    CsrSchedTid                  unmapTimer;     /* Unmap delay timer */
} cmL2caConnlessElement;
#endif

typedef struct
{
    CsrCmnList_t                  connList;
    cmL2caConnElement            *connElement;
    CsrBool                       cancelConnect;
    CsrUint32                     connectState;
    CsrUint8                      elementCounter;
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
    CsrCmnList_t                  connlessList;
#endif
} l2CaVariables;

typedef struct
{
    CsrUint32                     psm;
    CsrBtDeviceAddr               deviceAddr;
}scVariables;

typedef struct
{
    CsrUint32                      state;
    CsrBtDeviceAddr                deviceAddr;
    CsrUint16                      id;
    CsrUint8                       actualMode;
    CsrUint8                       requestedMode;
    CsrBool                        initConnect;
    CsrBool                        connectionSucceeded;
    CsrBtSniffSettings             sniffSettings;
    CsrBtParkSettings              parkSettings;
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    CsrBtSsrSettingsDownstream     ssrSettings;
    CsrBool                        ssrAccepted;
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
    CsrUint8                       lowPowerPriority;
    CsrBtLogicalChannelType        logicalChannelTypeMask;
} bnepTable;

typedef struct
{
    CsrUint32                     state;
    CsrSchedQid                          appHandle;
    CsrUint8                      role;
    CsrBtCmRoleType               roleType;
}  aclRoleVars_t;

typedef struct
{
    CsrBool                       roleSwitchNeeded;
}  rnrWorkaround_t;

typedef struct
{
    CsrBtDeviceAddr               deviceAddr;
    rnrWorkaround_t               rnrWorkaround;
    aclRoleVars_t                 roleVars;
    CsrUint8                      role;
    CsrBool                       roleChecked;
    link_policy_settings_t        actualLinkPolicySettings;
    link_policy_settings_t        linkPolicySettings;
    CsrBtSniffSettings            sniffSettings;
    CsrBtParkSettings             parkSettings;
    CsrBtSsrSettingsUpstream      curSsrSettings;
    CsrBool                       encrypted;
    CsrUint16                     flushTo;
    CsrSchedTid                   rfcCloseTimerId;
    CsrUint16                     maxRfcLinkFrameSize;
    CsrBool                       detachRequested;
    CsrUint8                      remoteFeatures[8];
    CsrBool                       remoteFeaturesValid;
    CsrUint8                      lmpVersion;
    CsrUint16                     manufacturerName;
    CsrUint16                     lmpSubversion;
    CsrUint16                     lsto;
    CsrUint16                     interval;
    CsrUint8                      mode;
    CsrBtClassOfDevice            cod;
    CsrBool                       incoming;
    CsrBool                       appControlsLowPower;
    CsrBtLogicalChannelType       logicalChannelTypeMask;
    CsrUint8                      noOfGuaranteedLogicalChannels;
    CsrUint32                     l2capExtendedFeatures;
    CsrBool                       sniffForceByAmp;
    CsrBool                       unsolicitedQosSetup;
    CsrUint16                     aclConnHandle;
#ifdef CSR_BT_LE_ENABLE
    CsrBool                       gattConnectionActive;
#endif
}  aclTable;

typedef struct
{
    aclTable                     aclVar[NUM_OF_ACL_CONNECTION];
    CsrUint8                      index;
    CsrUint8                      requestedRole;
    CsrBool                       roleSwitchFlag;
    CsrBool                       roleSwitchPerformed;
    CsrBool                       alwaysSupportMasterRole;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    CsrBool                       doMssBeforeScoSetup;
#endif
    CsrBool                       doMssBeforeRnr;
} roleVariables;

typedef struct
{
    CsrUint16                     seqNumber;
    CsrBtDeviceAddr               deviceAddr;
    CsrUint16                     clockOffset;
    page_scan_mode_t              pageScanMode;
    page_scan_rep_mode_t          pageScanRepMode;
} dmCacheParamEntry;

typedef struct dmCacheParamPool
{
    CsrUint16                     numOfEntriesInPool;
    CsrBtCmPrim                   requestType;
    dmCacheParamEntry             cacheTable[CM_DM_CACHE_PARAM_POOL_SIZE];
    struct dmCacheParamPool      *nextCachePool;
} dmCacheParamPool;


typedef struct
{
    bnepTable                     connectVar[CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS];
    bnepTable                    *indexPtr;
    CsrBool                       moreLpSettingsInProgress;
    CsrUint24                     classOfDevice;
    CsrUint8                      connectTypeInProgress;
    CsrSchedQid                   appHandle;
    BNEP_CONNECT_REQ_FLAGS        connectReqFlags;
    CsrBool                       disconnectInd;
    CsrUint8                      requestedRole;
    CsrBool                       roleSwitchReqIndex;
    CsrBool                       cancelConnect;
    CsrUint32                     connectState;
} bnepVariables;

typedef struct
{
    CsrBtUuid32                  *serviceHandle;
    CsrUint8                     *serverChannelList;
    CsrBool                      *validResult;
    CsrUint16                     resultListSize;
} sdcList_t;

typedef struct SdcSearchStructTag
{
    CsrSchedQid                   appHandle;
    CsrBtDeviceAddr               deviceAddr;
    CsrBtUuid32                  *serviceList;
    CsrBtUuid128                 *uuid128ServiceList;
    CsrUint8                     *uuidSet;
    CsrUint16                     uuidSetLength;
    sdcList_t                   **searchPtrArray;
    CsrUint16                     numOfSearchPtr;
    CsrBool                       obtainServerChannels;
    CsrBool                       dmOpenResult;
    CsrBool                       readAttrDirect;
    CsrBool                       extendedProtocolDescriptorList;
    CsrBool                       extendedUuidSearch;
    struct SdcSearchStructTag    *nextSdcElement;
} sdcSearchElement;

typedef struct
{
    filter_type_t                type;
    filter_condition_type_t      conditionType;
    CONDITION_T                  condition;
} eventFilterStruct;

typedef struct
{
    CsrUint8                      linkKeyType;
    CsrBtDeviceAddr               keyMissingDeviceAddr;
    CsrSchedTid                   keyMissingTimerId;
    CsrUint16                     keyMissingId;
    hci_return_t                  dmSmAccessIndStatus;
} rebond_t;

typedef struct
{
    CsrUint32                       state;
    CsrSchedQid                   appHandle;
    CsrBool                         cancelConnect;
    CsrBool                         dmInProgress;
    CsrBtCmPrim                     dmMsgTypeInProgress;
    CsrBtCmPrim                     dmMsgInProgress;
    CsrBool                         remoteNamePending;
    CsrUint24                       inquiryAccessCode;
    CsrSchedQid                   inquiryAppHandle;
    CsrUint32                       inquiryAppState;
    CsrUint32                       inquiryDmState;
    CsrUint8                        contableIndex;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    hci_connection_handle_t         closeScoHandle;
#endif
    CsrUint8                        requestedLinkMode;
    CsrUint8                        roleSwitchLinkMode;
    CsrBool                         roleSwitchInProgress;
    CsrMessageQueueType          *SaveQueue;
#ifdef CSR_BT_INSTALL_CM_DUT_MODE
    CsrBool                         deviceUnderTest;
#endif
    CsrBool                         disableInquiryScan;
    CsrBool                         disablePageScan;
    CsrSchedQid                   connectAblePhandle;
    CsrBtClassOfDevice            serviceCod;
    CsrBtClassOfDevice            majorCod;
    CsrBtClassOfDevice            minorCod;
    CsrBtClassOfDevice            pendingCod;
    CsrUint32                       writingCod;
    CsrBtDeviceAddr               switchRoleDeviceAddr;
    CsrUint8                        lmpVersion;
    CsrBool                         fallbackPerformed;
    link_policy_settings_t          defaultLinkPolicySettings;
    CsrBtSniffSettings              defaultSniffSettings;
    CsrBtParkSettings               defaultParkSettings;
    CsrUint32                       linkPolicyState;
    CsrBtCmPrim                     pendingFilterRequest;
    CsrUint8                        maxEventFilters;
    eventFilterStruct               filterInProgress;
    eventFilterStruct               *eventFilters;
    CsrUint8                        numberOfSupportedIac;
    CsrUint24                       pendingIac;
    CsrSchedTid                   liacRestoreTimer;
    CsrSchedQid                   liacRestoreAppHandle;
    CsrUint8                      liacRetryCounter;
    CsrUint24                     liacCod;
    dmCacheParamPool             *dmCacheParamTable;
    page_scan_mode_t             pageScanMode;
    page_scan_rep_mode_t         pageScanRepMode;
    CsrBtDeviceAddr                 cacheTargetDev;
    CsrSchedTid                   cacheFlushTimerId;
    CsrUint8                        numberOfCachePools;
    CsrUint16                       seqNumber;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
    CsrBtDeviceAddr               scoConnectAddr;
#endif
    hci_connection_handle_t       pcmAllocationTable[MAX_PCM_SLOTS];
    CsrUint24                     codWrittenToChip;
    CsrUint8                      currentChipScanMode;
    CsrUint8                      pendingChipScanMode;
    CsrUint16                     pagescanInterval;
    CsrUint16                     pagescanWindow;
    CsrUint8                      pagescanType;
    CsrUint16                     inquiryscanInterval;
    CsrUint16                     inquiryscanWindow;
    CsrUint8                      inquiryscanType;
    CsrBool                       readingName;
    CsrBool                       cancelRemoteName;
    CsrBtDeviceAddr               readNameDeviceAddr;
    CsrUint8                      retryCounter;
    rebond_t                     rebond;
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    localEirData_t                  *localEirData;
#endif
    CsrUint8                        lmpSuppFeatures[8];
    CsrBtSniffSettings              requestedSniffSetting;
    CsrBool                         appControlsAllLowPower;
    CsrBool                         forceSniffSettings;
    CsrInt8                         inquiryTxPowerLevel;
    CsrUint8                        lowInquiryPriorityLevel;
    CsrUint16                       inquiryMask;
    CsrBool                         scanEnabled;
    CsrUint32                       scanTime;
    CsrSchedTid                     scanTimerId;
    CsrBool                         scanIntervalChanged;
    CsrUint16                       origInqScanInterval;
    CsrUint16                       origInqScanWindow;
    CsrUint16                       origPageScanInterval;
    CsrUint16                       origPageScanWindow;
    CsrBtDeviceAddr                 detachAddr;
    CsrBtDeviceAddr                 operatingBdAddr;
    CsrUtf8String                  *localName;
} dmVariables;

/* Low energy cache */
#ifdef CSR_BT_LE_ENABLE
typedef struct leConnVarTag
{
    struct leConnVarTag          *next;
    DM_ACL_CONN_HANDLE_IND_T     dmPrim;
} leConnVar;
typedef struct
{
    CsrUint8                      scanMode; /* on/off */
    CsrUint8                      scanType;
    CsrUint16                     scanInterval;
    CsrUint16                     scanWindow;
    CsrUint8                      advMode; /* on/off */
    CsrUint8                      advType;
    CsrUint16                     advIntervalMin;
    CsrUint16                     advIntervalMax;
    CsrUint8                      advChannelMap;
    leConnVar                    *connCache;
} leVariables;
#endif

typedef struct
{
    CsrSchedQid                   appHandle;
    CsrBool                       smInProgress;
    CsrUint8                      activePlayer;
    CsrBtDeviceAddr               operatingBdAddr;
    CsrMessageQueueType          *SaveQueue;
    CsrBtCmPrim                   smMsgTypeInProgress;
    CsrBool                       popFromSaveQueue;
    CsrBtResultCode               resultCode;
    CsrBtSupplier                 resultSupplier;
    CsrUint16                     context;
    CsrUint8                      registeringSrvChannel;
} smVariables;

typedef struct
{
    sdcSearchElement             *sdcSearchList;
    sdcSearchElement             *currentElement;
    CsrUint16                     currentIndex;
    CsrUint16                     currentServiceIndex;
    CsrUint16                     attrId;
    CsrBool                       result;
    CsrBool                       validSearchResult;
    CsrUint8                      requestedServiceListSize;
    CsrBool                       aclOpenRequested;
    CsrUint32                     state;
    CsrUint8                      localServer;
    CsrBool                       sdcMsgPending;
    CsrBool                       cancelPending;
} sdcVariables;

typedef struct
{
    void                        *recvMsgP;
    CsrBool                      rfcBuild;
    CsrUint32                    globalState;
    dmVariables                  dmVar;
    sdcVariables                 sdcVar;
    smVariables                  smVar;
    roleVariables                roleVar;
    CmnCsrBtLinkedListStruct    *subscriptions;
    CmnCsrBtLinkedListStruct    *afhMaps;
    CmnCsrBtLinkedListStruct    *lstoList;
    CsrCmnList_t                 remoteVersion;
    CsrSchedQid                  sdHandle;
    CsrSchedQid                  scHandle;
    void                        *callback;

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    rfcVariables                 rfcVar;
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    l2CaVariables                l2caVar;
    scVariables                  scVar;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    bnepVariables                bnepVar;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifdef CSR_AMP_ENABLE
    CsrSchedQid                  ampmHandle;
#endif /* CSR_AMP_ENABLE */

#ifdef CSR_BT_LE_ENABLE
    CsrSchedQid                  gattHandle;
    leVariables                  leVar;
#endif /* CSR_BT_LE_ENABLE */

} cmInstanceData_t;

typedef void (* SignalHandlerType)(cmInstanceData_t * taskData);

/* Prototypes from csr_bt_cm_free_down.c */
void CsrBtCmFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);
void CsrBtCmFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* ndef _CM_MAIN_H */
