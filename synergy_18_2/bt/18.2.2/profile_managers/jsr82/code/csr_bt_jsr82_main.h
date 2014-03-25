#ifndef CSR_BT_JSR82_MAIN_H__
#define CSR_BT_JSR82_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

/* 7 ACL conn. for HCI build, 3 for RFCOMM */
#ifdef RFC_BUILD
#define MAX_ACL_LINKS       3
#else
#define MAX_ACL_LINKS       7
#endif

#include "csr_types.h"
#include "csr_message_queue.h"
#include "csr_bt_jsr82_prim.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sc_prim.h"
#include "csr_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef CsrUint8 Jsr82State;

/* States */
#define IDLE_S                      ((Jsr82State)(0x00))
#define BUSY_S                      ((Jsr82State)(IDLE_S + 1))
#define DISCOVERY_S                 ((Jsr82State)(BUSY_S + 1))
#define SERV_SEARCH_S               ((Jsr82State)(DISCOVERY_S + 1))
#define UPDATING_RECORD_S           ((Jsr82State)(SERV_SEARCH_S + 1))
#define WRITE_COD_S                 ((Jsr82State)(UPDATING_RECORD_S + 1))
#define SELECT_SERVICE_S            ((Jsr82State)(WRITE_COD_S + 1))
#define RD_NAME_S                   ((Jsr82State)(SELECT_SERVICE_S + 1))
#define CLEANUP_S                   ((Jsr82State)(RD_NAME_S + 1))
#define NUM_OF_JSR82_STATES         ((Jsr82State)(CLEANUP_S + 1))


#define SERVICE_REC_HANDLE_POOL_SIZE    ((CsrUint16)10)
#define PSM_POOL_SIZE                   ((CsrUint16)10)
#define RFC_CONNECTION_POOL_SIZE        ((CsrUint16)10)
#define JSR82_REMOTE_DEVICE_POOL_SIZE   ((CsrUint16)10)
#define JSR82_RFC_DOUBLE_REQUEST_DELAY  (1000)
#define JSR82_L2CA_DOUBLE_REQUEST_DELAY (1000)

#define JSR82_EVENT_MASK_RESERVER_VALUES_MASK 0x00000003


typedef void (* jsr82PsmAllocateCallbackType)(void *inst, psm_t psm, CsrBtJsr82Prim **orgMsg);

typedef void (* jsr82GetServerChannelCallBackType)(void *inst, CsrUint8 localServerChannel);

typedef struct
{
    CsrBtUuid32                        *serviceRecHandles;
    CsrUint16                        serviceHandleListCount;
    CsrUint16                        serviceIndex;
    CsrUint16                        attrSetLength;
    CsrUint16                        *attrSet;
    CsrUint16                        attributesLength;
    CsrUint8                         *attributes;
    CsrBool                          gettingDBState;
    CsrUint32                        servDBstate;
    CsrBool                          servDBvalid;
    CsrUint16                        attrCounter;
    CsrUint8                         attrRetries;
    CsrBtResultCode                  attrResult;
    CsrUint16                        serviceCount;
} getAttrStruct;

typedef struct
{
    CsrUint32                        serviceDataBaseState;
    CsrUint32                        serviceRecHandle;
    CsrUint16                        attrSetLength;
    CsrUint16                        *attrSet;
    CsrBool                          done;
    CsrBtResultCode                  result;
} populateStruct;

typedef struct
{
    CsrUint32    reqID;
    CsrBool      valid;
}validSearchType;

typedef struct tagValidSearchStruct
{
    struct tagValidSearchStruct   *next;
    validSearchType search;
} validSearchStruct;

typedef struct
{
    CsrUint16                        attrSetLength;
    CsrUint16                        *attrSet;
} servSearchStruct;

typedef struct
{
    CsrUint32                        position;
    CsrUint32                        bufferLength;
    CsrUint8                         *buffer;
    CsrSchedQid                       appHandle;
    CsrUint32                        reqID;
    CsrBool                          sendingPacket;
} rfcSendStruct;


typedef struct
{
    CsrUint8                         *receiveBuffer;
    CsrUint32                        readPointer;
    CsrUint32                        writePointer;
    CsrUint32                        validData;
    CsrMessageQueueType            *dataReqQueue;
    CsrBool                          cmHasCredit;
} rfcReceiveStruct;

typedef struct
{
    CsrSchedQid                           appHandle;
    CsrUint32                        reqID;
    rfcSendStruct                    *sendInfo;
    rfcReceiveStruct                 receiveInfo;
    CsrUint16                        maxFrameSize;
    CsrUint32                        jsr82ConnId;
    CsrUint32                        attrs;
    CsrBtDeviceAddr                  deviceAddr;
    CsrUint8                         serverChannel;
    CsrBool                          connected;
    CsrBool                          connecting;
    CsrBool                          connectable;
    CsrBool                          server;
    CsrBool                          destroy;
} rfcConnectionStruct;

typedef struct tagRfcConnectionPoolStruct
{
    struct tagRfcConnectionPoolStruct   *next;
    rfcConnectionStruct                 connectionList[RFC_CONNECTION_POOL_SIZE];
} rfcConnectionPoolStruct;

typedef struct
{
    CsrUint8                       remoteServerChannel;
    CsrBtDeviceAddr                        deviceAddr;
    CsrUint32                            attrs;
} rfcConnectStruct;

typedef struct
{
CsrBtDeviceAddr deviceAddr;
CsrUint16     activeConnections;
} aclConnStruct;

typedef struct tagSelectServiceListStruct
{
    struct tagSelectServiceListStruct   *next;
    CsrBtDeviceAddr                        deviceAddr;
} selectServiceListStruct;

typedef struct
{
    selectServiceListStruct         *list;
    selectServiceListStruct         *listPtr;
    CsrUint16                        uuidLength;
    CsrUint8                         *uuid;
    CsrBtUuid32                        *handleList;
    CsrUint16                        handleListLength;
    CsrUint16                        handleListIndex;
    CsrUint16                        currentAttribute;
    CsrSchedTid                             inquiryTimer;
} selectServiceStruct;

typedef struct tagJsr82LinkedListStruct
{
    struct tagJsr82LinkedListStruct *next;
} jsr82LinkedListStruct;


typedef struct
{
    CsrSchedQid                       appHandle;
    CsrUint32                        reqID;
    CsrBool                          connectable;
    CsrBool                          connected;
    CsrBool                          connecting;
    CsrUint32                        attrs;
    CsrBtConnId                      cid;
    CsrBtDeviceAddr                    deviceAddr;
    l2ca_mtu_t                      transmitMtu;
    l2ca_mtu_t                      receiveMtu;
    CsrUint16                        receivedDataLength;
    CsrUint32                        dataQueueLength;
    CsrMessageQueueType            *dataQueue;
    CsrMessageQueueType            *dataReqQueue;
    CsrBool                          sendingPacket;
} cidStruct;

typedef struct Jsr82CidTag
{
    struct Jsr82CidTag              *next;          /* Do not move - used for common linked list handling */
    struct Jsr82CidTag              *prev;          /* Do not move - used for common linked list handling */
    cidStruct                       *cidInst;
} jsr82CidElement;

typedef struct tagPsmValue
{
    struct tagPsmValue              *next;
    psm_t                           psm;
    CsrBool                          assigned;
    CsrBool                          client;
    CsrBool                          destroy;
    CsrCmnList_t                    cidList;
    jsr82PsmAllocateCallbackType    returnFunction;
}psmValue;

typedef struct
{
    CsrUint32                        upstreamHandle;
    CsrUint32                        downstreamHandle;
    CsrBool                          allocated;
    CsrBool                          assigned;
} serviceHandleMapping;

typedef struct tagServiceHandlePoolStruct
{
    struct tagServiceHandlePoolStruct   *next;
    serviceHandleMapping                mapList[SERVICE_REC_HANDLE_POOL_SIZE];
} serviceHandlePoolStruct;

typedef struct tagRequestedNamesStruct
{
    struct tagRequestedNamesStruct   *next;
    CsrBtDeviceAddr    address;
    CsrUint32        reqID;
    CsrSchedQid       appHandle;
    CsrBool          alwaysAsk;
    CsrBool          cfmSent;
} requestedNamesStruct;

/* Inquiry states */
#define INQUIRY_STATE           0
#define CANCEL_STATE            1
#define NAME_STATE              2

typedef struct
{
    CsrSchedTid                     timer;
    CsrBool                  timedOut;
    CsrBool                  cancelled;
    CsrBool                  indSent;
    CsrBool                  nameReqSent;
    CsrSchedQid               appHandle;
    CsrUint32                reqID;
    CsrUint8                 state;
    CsrUint24                iac;
    CsrUint16                resultCount;
} inquiryStruct;

typedef struct
{
    CsrSchedQid               appHandle;
    CsrBtJsr82EventMask       eventMask;
    CsrBtJsr82EventMaskCond  conditionMask;
} subscribeStruct;

typedef struct
{
    CsrSchedQid                       appHandle;         /* Handle for the upstream application using the JSR82 interface */
    CsrUint32                        reqID;
    Jsr82State                      state;
    CsrBool                          restoreFlag;
    CsrMessageQueueType            *saveQueue;
    CsrMessageQueueType            *pendingQueue;
    void                            *recvMsg;
    CsrUint16                        eventClass;
    void                            *argv;
    getAttrStruct                   *getAttr;
    servSearchStruct                *servSearch;
    validSearchStruct               *validSearches;
    populateStruct                  *populate;
    serviceHandlePoolStruct         *serviceRecHandlePool;
    psmValue                        *psmList;
    CsrUint16                        allocatedPsms;
    jsr82GetServerChannelCallBackType   getServerChannelCallBack;
    CsrBtJsr82SrRegisterRecordReq        *updateMessage;
    rfcConnectionPoolStruct         *rfcConnectionPool;
    CsrUint8                         codFieldsMask;
    device_class_t                  javaDeviceClassBits;
    device_class_t                  newDeviceClass;
    CsrBool                          trustedCheck;
    CsrUint16                        trustedDevicesTempNumber;
    CsrBtDeviceAddr                    *trustedDevicesTempStorage;
    selectServiceStruct             *selectService;
    inquiryStruct                   *inquiry;
    requestedNamesStruct            *requestedNames;
    CsrBool                          gettingName;
    CsrBool                          rdIsTrusted;
    CsrBtDeviceAddr                    rdIsTrustedBdAddr;
    aclConnStruct                   aclConnections[MAX_ACL_LINKS];
    CsrBool                          doingCleanup;
    CsrBtDeviceAddr                    *inqDeviceList;
    CsrUint16                        inqDeviceListCount;
    CsrUint16                        inqDeviceListMax;
    CsrBtDeviceAddr                    serviceSearchDeviceAddr;
    subscribeStruct                 subscribeInfo;
    void                            *cmRfcDynJumpTable[CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT];
    void                            *cmSdcDynJumpTable[CSR_BT_CM_SDC_PRIM_UPSTREAM_COUNT];
    void                            *cmL2caDynJumpTable[CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT];
    void                            *cmBnepDynJumpTable[CSR_BT_CM_BNEP_PRIM_UPSTREAM_COUNT];
    void                            *cmInquiryDynJumpTable[CSR_BT_CM_INQUIRY_PRIM_UPSTREAM_COUNT];
    void                            *cmDmDynJumpTable[CSR_BT_CM_DM_PRIM_UPSTREAM_COUNT];
    void                            *cmDm1P2DynJumpTable[CSR_BT_CM_DM_1P2_PRIM_UPSTREAM_COUNT];
    void                            *cmSendDynJumpTable[CSR_BT_CM_SEND_PRIM_UPSTREAM_COUNT];
    void                            *sdDynJumpTable[CSR_BT_SD_PRIM_UPSTREAM_COUNT];
    void                            *scDynJumpTable[CSR_BT_SC_PRIM_UPSTREAM_COUNT];
    CsrBool                          deinit;
} Jsr82MainInstanceData;

typedef struct
{
    CsrSchedQid               appHandle;
    CsrUint32                reqID;
    Jsr82State              prevState;
    CsrUint8                 *serviceRecord;
    CsrUint16                serviceRecordSize;
    CsrUint32                serviceRecHandle;
    CsrUint16                retries;
    Jsr82MainInstanceData   *instData;
} registerServiceType;

typedef struct
{
    CsrSchedQid               appHandle;
    CsrUint32                reqID;
    Jsr82State              prevState;
    CsrUint32                serviceRecHandle;
    CsrUint16                retries;
    Jsr82MainInstanceData   *instData;
} unregisterServiceType;


typedef CsrBool (* jsr82DynSignalHandler)(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg);
typedef void (* jsr82SendDownMsgType)(void *reqMsg);

typedef struct tagJsr82QElement
{
    struct tagJsr82QElement *next;
    CsrBtJsr82Prim               *orgMsg;
    CsrUint16                event;
    void                    *reqMsg;
    jsr82DynSignalHandler   signalHandler;
    CsrUint16                reqType;
} Jsr82QElement;

typedef struct
{
    jsr82SendDownMsgType    msgSender;
    CsrUint16                *primList;
    CsrUint16                primListLength;
} jsr82PrimMapping;


void CsrBtJsr82Init(void **gash);
void CsrBtJsr82Deinit(void **gash);
void CsrBtJsr82Handler(void **gash);
void CsrBtJsr82CleanupReqHandler(Jsr82MainInstanceData *instData);

typedef void (* jsr82HandlerType)(Jsr82MainInstanceData *instData);


/* Prototypes from jsr82_free_down.c */
void CsrBtJsr82FreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_JSR82_MAIN_H__ */
