#ifndef CSR_BT_GATT_UTIL_H__
#define CSR_BT_GATT_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_gatt_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Misc */
void CsrBtGattFreeAppScanFilters(CsrBtGattAppElement *app);
void CsrBtGattWhitelistAdd(GattMainInst *inst,
                           CsrUint16 num,
                           CsrBtTypedAddr *addrs);
CsrBool CsrBtGattWhitelistUpdate(GattMainInst *inst);
CsrBool CsrBtGattFilterAddr(CsrBtGattAppElement *app,
                            CsrBtTypedAddr *address);
CsrBool CsrBtGattFilterData(CsrBtGattAppElement *app,
                            CsrUint8 length,
                            CsrUint8 *data);
void CsrBtGattUpdateAdvertise(GattMainInst *inst);
void CsrBtGattUpdateScan(GattMainInst *inst);
void CsrBtGattUpdateConnection(GattMainInst *inst);
CsrBool CsrBtGattStoreData(CsrBtGattAppAdvParams *adv,
                           CsrBool append,
                           CsrUint8 *adData,
                           CsrUint8 adLength,
                           CsrUint8 *srData,
                           CsrUint8 srLength);
void CsrBtGattInitSecurity(void);
void CsrBtGattInitHandler(GattMainInst *inst);
CsrBool CsrBtGattConnSetL2capFlags(CsrBtGattConnElement *conn,
                                   CsrBtGattConnFlags flags,
                                   CsrBool master,
                                   CsrBool bredr);
void CsrBtGattGetAttUuid(CsrBtUuid uuid,
                         CsrUint32 *attUuid,
                         att_uuid_type_t *uuidType);

CsrUint16 CsrBtGattValidateBtConnIdByMtu(GattMainInst *inst,
                                         CsrBtGattId  gattId,
                                         CsrBtConnId  btConnId);

CsrBtGattConnInfo CsrBtGattGetConnInfo(GattMainInst *inst,
                                       CsrBtGattId  gattId,
                                       CsrBtConnId  btConnId);

CsrUint16 CsrBtGattConnPageScanners(GattMainInst *inst);

/* Send buffered prepare write access indications */
CsrBool CsrBtGattPlayExecuteWrite(GattMainInst *inst, CsrUint16 cid);
void CsrBtGattPlayExecuteFinalise(GattMainInst *inst, CsrUint16 cid);
void CsrBtGattPlayExecuteMarkFail(GattMainInst *inst, CsrUint16 cid);

/* Gatt app inst func */
typedef struct
{
    CsrBtTypedAddr    address;
    CsrBool           status;
    CsrBtGattConnInfo connInfo;
} CsrBtGattphysicalLinkStatusIds;

CsrInt32  CsrBtGattAppInstSortByAttributeValue(CsrCmnListElm_t *elem1, CsrCmnListElm_t *elem2);
CsrBool   CsrBtGattAppInstFindGattId(CsrCmnListElm_t *elem, void *value);
CsrBool   CsrBtGattAppInstFindAttrHandle(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_APP_INST_ADD_FIRST(_appList) \
    (CsrBtGattAppElement *)CsrCmnListElementAddFirst(&(_appList), \
                                                     sizeof(CsrBtGattAppElement))

#define CSR_BT_GATT_APP_INST_FIND_GATT_ID(_appList,_gattId) \
    ((CsrBtGattAppElement *)CsrCmnListSearch(&(_appList), \
                                             CsrBtGattAppInstFindGattId, \
                                             (void *)(_gattId)))

#define CSR_BT_GATT_APP_INST_GET_FIRST(_appList) \
    ((CsrBtGattAppElement *)CsrCmnListGetFirst(&(_appList)))

#define CSR_BT_GATT_APP_INST_FIND_ATTR_HANDLE(_appList,_attrHandle) \
    ((CsrBtGattAppElement *)CsrCmnListSearch(&(_appList), \
                                             CsrBtGattAppInstFindAttrHandle, \
                                             (void *)(_attrHandle)))

#define CSR_BT_GATT_APP_INST_REMOVE(_appList,_appElem) \
                                    (CsrCmnListElementRemove((CsrCmnList_t *)&(_appList), \
                                                             (CsrCmnListElm_t *)(_appElem)))

#define CSR_BT_GATT_APP_INST_ITERATE(_appList,_func,_dataPtr) \
                                    (CsrCmnListIterate(&(_appList), (_func), (void *)(_dataPtr)))

#define CSR_BT_GATT_APP_INST_SORT_BY_ATTR_VALUE(_appList) \
                                               (CsrCmnListSort(&(_appList), \
                                                CsrBtGattAppInstSortByAttributeValue))

/* Prepare write handlers */
typedef struct
{
    CsrUint16 cid;
    CsrBtGattHandle handle;
} CsrBtGattPrepareCidHandle;

#define CSR_BT_GATT_PREPARE_INST_REMOVE(_prepareList,_prepare) \
    (CsrCmnListElementRemove((CsrCmnList_t *)&(_prepareList), \
                             (CsrCmnListElm_t *)(_prepare)))

#define CSR_BT_GATT_PREPARE_INST_GET_FIRST(_prepareList) \
    ((CsrBtGattPrepareBuffer*)CsrCmnListGetFirst(&(_prepareList)))

#define CSR_BT_GATT_PREPARE_INST_ADD_LAST(_prepareList) \
    (CsrBtGattPrepareBuffer*)CsrCmnListElementAddLast(&(_prepareList), \
                                                      sizeof(CsrBtGattPrepareBuffer))


/* The the value for the last extern application. Intern application is 
   GATT, SD, SC */ 
#define CSR_BT_GATT_APP_INST_LAST_EXTERN_APPLICATION 0x00000004

#define CSR_BT_GATT_APP_INST_COUNT(_appList) (CsrCmnListGetCount(&(_appList)))

/* Gatt SDP service record inst func which is included in app inst */ 
#define CSR_BT_GATT_SDP_INST_ADD_LAST(_sdpList) \
    (CsrBtGattSdpRecordElement *)CsrCmnListElementAddLast(&(_sdpList), \
                                                     sizeof(CsrBtGattSdpRecordElement))

CsrBool CsrBtGattSdpInstFindState(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_SDP_INST_FIND_STATE(_sdpList,_sdpElem,_state) \
{ \
    CsrUint8 _ids  = _state; \
    (*_sdpElem) = ((CsrBtGattSdpRecordElement *)CsrCmnListSearch(&(_sdpList), \
                                                                 CsrBtGattSdpInstFindState, \
                                                                 (void *)(&_ids))); \
};    

void CsrBtGattSdpInstChangeFromInitToPendingState(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_SDP_INST_ITERATE_CHANGE_FROM_INIT_TO_PENDING_STATE(_sdpList) \
                                    (CsrCmnListIterate(&(_sdpList), \
                                     CsrBtGattSdpInstChangeFromInitToPendingState, \
                                     NULL))

typedef struct
{
    CsrBtGattHandle     start; /* The first handle that must be removed */
    CsrBtGattHandle     end;   /* The last handle that must be removed */
} CsrBtGattSdpUnregisterIds;

CsrBool CsrBtGattSdpInstIterateUnregisterHandler(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_SDP_INST_ITERATE_UNREGISTER(_sdpList,_start,_end) \
{ \
    CsrBtGattSdpUnregisterIds _ids; \
    _ids.start = (_start); \
    _ids.end   = (_end); \
    (CsrCmnListIterateAllowRemove(&(_sdpList), \
                                  CsrBtGattSdpInstIterateUnregisterHandler, \
                                  (void *)(&_ids))); \
};

#define CSR_BT_GATT_SDP_INST_ITERATE_REMOVE_BY_STATE(_sdpList,_state) \
{ \
    CsrUint8 _ids = _state; \
    (CsrCmnListIterateAllowRemove(&(_sdpList), \
                                  CsrBtGattSdpInstFindState, \
                                  (void *)(&_ids))); \
};

#define CSR_BT_GATT_SDP_INST_REMOVE(_sdpList,_sdpElem) \
                                    (CsrCmnListElementRemove((CsrCmnList_t *)&(_sdpList), \
                                                             (CsrCmnListElm_t *)(_sdpElem)))



/* Gatt subscribe inst func which is included in app inst */ 
#define CSR_BT_GATT_SUBSCRIBE_INST_ADD_FIRST(_subscribeList) \
    (CsrBtGattSubscribeElement *)CsrCmnListElementAddFirst(&(_subscribeList), \
                                                     sizeof(CsrBtGattSubscribeElement))
typedef struct
{
    CsrBtTypedAddr          address;
    CsrBtGattConnInfo       connInfo;
    CsrBtGattHandle         attrHandle;
    CsrUint16               valueLength;
    CsrUint8               *value;
    CsrBtConnId             btConnId;
    CsrBtGattCliConfigBits  configuration;
    CsrBtGattId             privateGattId;
} CsrBtGattSubscribeFindIds;

CsrBool CsrBtGattSubscribeInstFindId(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_SUBSCRIBE_INST_FIND_SUBSCRIBE_ID(_appElement, _ids) \
    ((CsrBtGattSubscribeElement *)CsrCmnListSearch(&(_appElement->subscribeInst), \
                                                   CsrBtGattSubscribeInstFindId, \
                                                   (void *)(_ids)))

void CsrBtGattAddOrRemoveSubscribeElement(GattMainInst                         *inst, 
                                          CsrBtGattWriteClientConfigurationReq *prim);

CsrBtGattSubscribeElement * CsrBtGattGetSubscribeElement(GattMainInst        *inst,
                                                         CsrBtGattAppElement *appElement,
                                                         CsrBtTypedAddr      address,
                                                         CsrBtGattHandle     valueHandle);

CsrBool CsrBtGattSubscribeInstFindByAddr(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_SUBSCRIBE_INST_FIND_SERVICE_CHANGED(_appElement, _address) \
    ((CsrBtGattSubscribeElement *)CsrCmnListSearch(&(_appElement->subscribeInst), \
                                                   CsrBtGattSubscribeInstFindByAddr, \
                                                   (void *)(_address)))

#define CSR_BT_GATT_SUBSCRIBE_INST_ITERATE_REMOVE(_subscribeList,_func,_dataPtr) \
                              (CsrCmnListIterateAllowRemove(&(_subscribeList), (_func), (void *)(_dataPtr)))

#define CSR_BT_GATT_SUBSCRIBE_INST_REMOVE(_subscribeList,_subElem) \
                                          (CsrCmnListElementRemove((CsrCmnList_t *)&(_subscribeList), \
                                                                   (CsrCmnListElm_t *)(_subElem)))

void CsrBtGattAddHandleForServiceChanged(GattMainInst        *inst,
                                         CsrBtTypedAddr      address,
                                         CsrBtGattHandle     attrHandle);

void CsrBtGattRemoveHandleForServiceChanged(GattMainInst   *inst,
                                            CsrBtTypedAddr address,
                                            CsrBool        update);


/* Gatt conn inst func */
typedef struct
{
    CsrBtConnId btConnId;
    CsrBtGattId gattId;
} CsrBtGattConnFindIds;

CsrBool CsrBtGattConnInstFindBtConnGattId(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_FIND_BT_CONN_GATT_ID(_connList,_conn,_btConnId,_gattId) \
{ \
    CsrBtGattConnFindIds _ids; \
    _ids.btConnId = (_btConnId); \
    _ids.gattId = (_gattId); \
    (*_conn) = ((CsrBtGattConnElement *)CsrCmnListSearch(&(_connList), \
                                                         CsrBtGattConnInstFindBtConnGattId, \
                                                         (void *)(&_ids))); \
};

CsrBool CsrBtGattConnInstFindBredrAcceptPending(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_FIND_BREDR_ACCEPT_PENDING(_connList,_conn,_btConnId,_gattId) \
{ \
    CsrBtGattConnFindIds _ids; \
    _ids.btConnId = (_btConnId); \
    _ids.gattId = (_gattId); \
    (*_conn) = ((CsrBtGattConnElement *)CsrCmnListSearch(&(_connList), \
                                                         CsrBtGattConnInstFindBredrAcceptPending, \
                                                         (void *)(&_ids))); \
};

CsrBool CsrBtGattConnInstFindConnectedBtConnId(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_FIND_CONNECTED_BT_CONN_ID(_connList,_btConnId) \
    ((CsrBtGattConnElement *)CsrCmnListSearch(&(_connList), \
                                             CsrBtGattConnInstFindConnectedBtConnId, \
                                             (void *)(_btConnId)))

CsrBool CsrBtGattConnInstFindBredrAcceptBtConnId(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_FIND_BREDR_ACCEPT_BT_CONN_ID(_connList,_btConnId) \
    ((CsrBtGattConnElement *)CsrCmnListSearch(&(_connList), \
                                             CsrBtGattConnInstFindBredrAcceptBtConnId, \
                                             (void *)(_btConnId)))

CsrBool CsrBtGattConnInstFindBtConnId(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_FIND_BT_CONN_ID(_connList,_btConnId) \
    ((CsrBtGattConnElement *)CsrCmnListSearch(&(_connList), \
                                             CsrBtGattConnInstFindBtConnId, \
                                             (void *)(_btConnId)))

#define CSR_BT_GATT_CONN_INST_ADD_LAST(_connList) \
    (CsrBtGattConnElement *)CsrCmnListElementAddLast(&(_connList), \
                                                     sizeof(CsrBtGattConnElement))

#define CSR_BT_GATT_CONN_INST_REMOVE(_connList,_connElement) \
    (CsrCmnListElementRemove(&(_connList), \
                             (CsrCmnListElm_t *)(_connElement)))

#define CSR_BT_GATT_CONN_INST_GET_FIRST(_connList) \
    ((CsrBtGattConnElement *)CsrCmnListGetFirst(&(_connList)))

CsrBool CsrBtGattConnInstFindBtConnIdFromAddress(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_FIND_CONN_ID_FROM_ADDRESS(_connList,_address) \
        ((CsrBtGattConnElement *)CsrCmnListSearch(&(_connList), \
                                              CsrBtGattConnInstFindBtConnIdFromAddress, \
                                              (void *)(_address)))

#define CSR_BT_GATT_NUMBER_OF_PHYSICAL_CONN (6)

typedef struct
{
    CsrUint16       addressCount;
    CsrBtGattId     gattId;
    CsrBtTypedAddr  address[CSR_BT_GATT_NUMBER_OF_PHYSICAL_CONN];
} CsrBtGattFindPhysicalConn;

#define CSR_BT_GATT_CONN_INST_ITERATE(_connList,_func,_dataPtr) \
                                    (CsrCmnListIterate(&(_connList), (_func), (void *)(_dataPtr)))

typedef struct
{
    CsrBtConnId btConnId;
    CsrUint16   mtu;
} CsrBtGattConnUpdateMtuIds;

void CsrBtGattConnInstUpdateMtu(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_UPDATE_MTU(_connList,_btConnId,_mtu) \
{ \
    CsrBtGattConnUpdateMtuIds _ids; \
    _ids.btConnId = (_btConnId); \
    _ids.mtu      = (_mtu); \
    (CsrCmnListIterate(&(_connList), CsrBtGattConnInstUpdateMtu, (void *)(&_ids))); \
};

typedef struct
{
    CsrBtConnId btConnId;
    CsrUint16   length;
    CsrUint8    *name;
} CsrBtGattConnUpdateRemoteNameIds;

void CsrBtGattConnInstUpdateRemoteName(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_UPDATE_REMOTE_NAME(_connList,_btConnId,_length, _name) \
{ \
    CsrBtGattConnUpdateRemoteNameIds _ids; \
    _ids.btConnId = (_btConnId); \
    _ids.length   = (_length); \
    _ids.name     = (_name); \
    (CsrCmnListIterate(&(_connList), CsrBtGattConnInstUpdateRemoteName, (void *)(&_ids))); \
};

typedef struct
{
    GattMainInst    *gattInst;
    CsrBtTypedAddr  deviceAddr; /* Peer device address */
    CsrUint16       interval;   /* Connection interval (in slots, i.e. x * 0.625ms) */
    CsrUint16       timeout;    /* Supervision timeout (in 10ms units) */
    CsrUint16       latency;    /* Connection latency (in slots, i.e. x * 0.625ms) */
} CsrBtGattConnParamChangedIds;

void CsrBtGattParamConnChangeIndSendHandler(GattMainInst *inst, CsrBtGattConnElement *conn);
void CsrBtGattConnInstUpdateParamChanged(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_ITERATE_INST_UPDATE_PARAM(_gattInst,_connList, \
                                                   _deviceAddr,_interval,_timeout,_latency) \
{ \
    CsrBtGattConnParamChangedIds _ids; \
    _ids.deviceAddr = (_deviceAddr); \
    _ids.interval   = (_interval); \
    _ids.timeout    = (_timeout); \
    _ids.latency    = (_latency); \
    _ids.gattInst   = (_gattInst); \
    (CsrCmnListIterate(&(_connList), CsrBtGattConnInstUpdateParamChanged, (void *)(&_ids))); \
};

typedef struct
{
    GattMainInst    *gattInst;
    CsrBtGattId     gattId;           /* application owner for this connection */
    CsrBtConnId     btConnId;         /* connection id and CID */
    CsrUint16       connIntervalMin;  /* New proposed min connection interval */
    CsrUint16       connIntervalMax;  /* New proposed max connection interval */    
    CsrUint16       latency;          /* New proposed latency */       
    CsrUint16       timeout;          /* New proposed Supervision timeout */
    CsrUint16       numOfRsp;         /* Number of applications that must accept before can be changed */ 
    CsrBool         accept;           /* Only accepted if all applications accepts */   
    CsrUint16       connParamUpdateId;/* The identifier of the conn update param procedure */
} CsrBtGattConnParamUpdateReqIds;

void CsrBtGattConnInstUpdateParamReq(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_ITERATE_UPDATE_PARAM_REQ(_gattInst,_connList,_ids,_gattId,_btConnId, \
                                                       _connIntervalMin, _connIntervalMax, \
                                                       _latency,_timeout,_connParamUpdateId) \
{ \
    _ids.gattInst          = (_gattInst); \
    _ids.gattId            = (_gattId); \
    _ids.btConnId          = (_btConnId); \
    _ids.connIntervalMin   = (_connIntervalMin); \
    _ids.connIntervalMax   = (_connIntervalMax); \
    _ids.latency           = (_latency); \
    _ids.timeout           = (_timeout); \
    _ids.connParamUpdateId = (_connParamUpdateId); \
    _ids.numOfRsp          = 0; \
    _ids.accept            = TRUE; \
    (CsrCmnListIterate(&(_connList), CsrBtGattConnInstUpdateParamReq, (void *)(&_ids))); \
};

typedef struct
{
    CsrBtConnId     btConnId;         /* connection id and CID */
    CsrUint16       numOfRsp;         /* Number of connection with btConnId in connected or pending state */ 
} CsrBtGattConnNumOfExistingPendingIds;

void CsrBtGattConnInstGetNumOfExistingPending(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_NUM_OF_EXISTING_PENDING_CONN(_connList,_ids,_btConnId) \
{ \
    _ids.btConnId = (_btConnId); \
    _ids.numOfRsp = 0; \
    (CsrCmnListIterate(&(_connList), CsrBtGattConnInstGetNumOfExistingPending, (void *)(&_ids))); \
};

typedef struct
{
    CsrBtConnId btConnId;           /* connection id and CID */
    CsrUint16   connParamUpdateId;  /* The identifier of the conn update param procedure */
    CsrUint16   numOfRsp;           /* Number of applications that must accept before can be changed */ 
    CsrBool     cancelled;          /* Indicates that the procedure is cancelled*/
} CsrBtGattConnParamUpdateResIds;

void CsrBtGattConnInstUpdateParamRes(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_CONN_INST_ITERATE_UPDATE_PARAM_RES(_connList,_ids,_btConnId,_connParamUpdateId, _cancelled) \
{ \
    _ids.btConnId          = (_btConnId); \
    _ids.connParamUpdateId = (_connParamUpdateId); \
    _ids.cancelled         = (_cancelled); \
    _ids.numOfRsp          = 0; \
    (CsrCmnListIterate(&(_connList), CsrBtGattConnInstUpdateParamRes, (void *)(&_ids))); \
};



/* Gatt queue func */
CsrBool CsrBtGattQueueFindBtConnId(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtGattQueueFindMsgToCancel(CsrCmnListElm_t *elem, void *value);
void CsrBtGattQueueRestoreHandler(GattMainInst *inst, CsrBtGattQueueElement *element);
CsrBtConnId CsrBtGattConnIdByAddr(GattMainInst *inst, CsrBtTypedAddr *addr);
CsrBool CsrBtGattQueueInstFindBtConnGattId(CsrCmnListElm_t *elem, void *value);
CsrBool CsrBtGattQueueFindPrivateReadName(CsrCmnListElm_t *elem, void *value);

#define CSR_BT_GATT_QUEUE_GET_FIRST(_queueList) \
    ((CsrBtGattQueueElement *)CsrCmnListGetFirst(&(_queueList)))

#define CSR_BT_GATT_QUEUE_FIND_BT_CONN_ID(_queueList,_btConnId) \
    ((CsrBtGattQueueElement *)CsrCmnListSearch(&(_queueList), \
                                               CsrBtGattQueueFindBtConnId, \
                                               (void *)(_btConnId)))

#define CSR_BT_GATT_QUEUE_FIND_BT_CONN_GATT_ID(_queueList,_queue,_btConnId,_gattId) \
{ \
    CsrBtGattConnFindIds _ids; \
    _ids.btConnId = (_btConnId); \
    _ids.gattId = (_gattId); \
    (*_queue) = ((CsrBtGattQueueElement *)CsrCmnListSearch(&(_queueList), \
                                                           CsrBtGattQueueInstFindBtConnGattId, \
                                                           (void *)(&_ids))); \
};


typedef CsrBtGattConnFindIds CsrBtGattFindMsgToCancelIds;

#define CSR_BT_GATT_QUEUE_FIND_MSG_TO_CANCEL(_queueList,_qElem,_gattId,_btConnId) \
{ \
    CsrBtGattFindMsgToCancelIds _ids; \
    _ids.btConnId = (_btConnId); \
    _ids.gattId = (_gattId); \
    (*_qElem) = ((CsrBtGattQueueElement *)CsrCmnListSearch(&(_queueList), \
                                                         CsrBtGattQueueFindMsgToCancel, \
                                                         (void *)(&_ids))); \
};

#define CSR_BT_GATT_QUEUE_FIND_PRIVATE_READ_NAME_MSG(_queueList,_qElem,_gattId,_btConnId) \
{ \
    CsrBtGattFindMsgToCancelIds _ids; \
    _ids.btConnId = (_btConnId); \
    _ids.gattId = (_gattId); \
    (*_qElem) = ((CsrBtGattQueueElement *)CsrCmnListSearch(&(_queueList), \
                                                         CsrBtGattQueueFindPrivateReadName, \
                                                         (void *)(&_ids))); \
};

#define CSR_BT_GATT_QUEUE_REMOVE(_queueList,_qElem) \
    (CsrCmnListElementRemove(&(_queueList), \
                             (CsrCmnListElm_t *)(_qElem)))

#define CSR_BT_GATT_QUEUE_ADD_LAST(_queueList) \
    (CsrBtGattQueueElement *)CsrCmnListElementAddLast(&(_queueList), \
                                                      sizeof(CsrBtGattQueueElement))

#define CSR_BT_GATT_QUEUE_ITERATE_REMOVE(_queueList,_func,_dataPtr) \
                              (CsrCmnListIterateAllowRemove(&(_queueList), (_func), (void *)(_dataPtr)))

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_GATT_UTIL_H__ */

