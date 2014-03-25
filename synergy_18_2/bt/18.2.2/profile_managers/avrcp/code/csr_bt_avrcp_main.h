#ifndef CSR_BT_AVRCP_MAIN_H__
#define CSR_BT_AVRCP_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"
#include "csr_sched.h"
#include "csr_bt_usr_config.h"
#include "csr_message_queue.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_list.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "csr_bt_avrcp_cmn_data.h"
#include "csr_bt_avrcp_prim.h"


#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Application states */
#define AVRCP_STATE_APP_IDLE            (0)
#define AVRCP_STATE_APP_BUSY            (1)
#define AVRCP_STATE_APP_NUM             (2)

/* Post-connection states */
#define AVRCP_STATE_PC_VALID            (0)
#define AVRCP_STATE_PC_INVALID          (1)
#define AVRCP_STATE_PC_NUM              (2)

/***** Connection specific *****/
/* Connection states */
#define AVRCP_STATE_CONN_DISCONNECTED   (0)
#define AVRCP_STATE_CONN_PENDING        (1)
#define AVRCP_STATE_CONN_CONNECTING     (2)
#define AVRCP_STATE_CONN_CONNECTED      (3)
#define AVRCP_STATE_CONN_DISCONNECTING  (4)
#define AVRCP_STATE_CONN_CANCELLING     (5)
#define AVRCP_STATE_CONN_DISC2RECONNECT (6)

/* Connection directions */
#define AVRCP_CONN_DIR_INVALID          (0)
#define AVRCP_CONN_DIR_INCOMING         (1)
#define AVRCP_CONN_DIR_OUTGOING         (2)

/* Misc connection */
#define AVRCP_MTU_INVALID               (0)
#define AVRCP_CID_INVALID               (0)
#define AVRCP_PSM_INVALID               (0xFFFF)

/* Activate states */
#define AVRCP_STATE_ACT_ACTIVATED       (0x00)
#define AVRCP_STATE_ACT_DEACTIVATED     (0x01)
#define AVRCP_STATE_ACT_DEACTIVATING    (0x02)

/***** SDP specific *****/
/* SDP states */
#define AVRCP_STATE_SDP_IDLE            (0)
#define AVRCP_STATE_SDP_ACTIVE          (1)
#define AVRCP_STATE_SDP_DONE            (2)
#define AVRCP_STATE_SDP_PENDING         (3)

/* Service record parameters */
#define AVRCP_VERSION_AVCTP_10          (0x0100)
#define AVRCP_VERSION_AVCTP_12          (0x0102)
#define AVRCP_VERSION_AVCTP_13          (0x0103)

/***** Pending message states *****/
#define AVRCP_STATE_PM_IDLE             (0x00)
/* Pass-through states */
#define AVRCP_STATE_PM_PT_RELEASE_PENDING   (0x01)
#define AVRCP_STATE_PM_PT_PRESS_PENDING     (0x02)

/* Misc SDP */
#define AVRCP_SDP_INVALID_SR_HANDLE     (0)
#define AVRCP_CT_SDP_FEATURES_MASK (0x004F) /* Mask RFA bits */
#define AVRCP_TG_SDP_FEATURES_MASK (0x00FF) /* Mask RFA bits */

/* AVRCP roles */
#define AVRCP_ROLE_INVALID              (0x00)
#define AVRCP_ROLE_TARGET               (0x01)
#define AVRCP_ROLE_CONTROLLER           (0x02)
#define AVRCP_ROLE_BOTH                 (0x03)

#define AVRCP_CHANGE_STATE_INDEX(var, state, index)         (var = state)
#define AVRCP_CHANGE_STATE(var, state)                      (var = state)

#define AVRCP_MSG_ID_INVALID                                (0x00000000)
#define AVRCP_MSG_ID_ASSIGN(msgId)                          (++(msgId))

/***** Macros for handling lists *****/
/* Connection list (connList) */
#define AVRCP_LIST_CONN_ADD_FIRST(listPtr)          ((AvrcpConnInstance_t *)CsrCmnListElementAddFirst((listPtr), sizeof(AvrcpConnInstance_t)))
#define AVRCP_LIST_CONN_REMOVE(listPtr, elemPtr)    (CsrCmnListElementRemove((listPtr), (CsrCmnListElm_t *)(elemPtr)))
#define AVRCP_LIST_CONN_GET_FIRST(listPtr)          ((AvrcpConnInstance_t *)CsrCmnListGetFirst((listPtr)))
#define AVRCP_LIST_CONN_GET_ADDR(listPtr, addPtr)   ((AvrcpConnInstance_t *)CsrCmnListSearchOffsetAddr((listPtr), offsetof(AvrcpConnInstance_t, address), addPtr))
#define AVRCP_LIST_CONN_GET_AID(listPtr, id)        ((AvrcpConnInstance_t *)CsrCmnListSearchOffsetUint8((listPtr), offsetof(AvrcpConnInstance_t, appConnId), id))
#define AVRCP_LIST_CONN_GET_C_ST(listPtr, cstate)   ((AvrcpConnInstance_t *)CsrCmnListSearchOffsetUint8((listPtr), offsetof(AvrcpConnInstance_t, control.state), cstate))
#define AVRCP_LIST_CONN_GET_SDP_ST(listPtr, sstate) ((AvrcpConnInstance_t *)CsrCmnListSearchOffsetUint8((listPtr), offsetof(AvrcpConnInstance_t, sdpState), sstate))
#define AVRCP_LIST_CONN_GET_C_CID(listPtr, ccid)    ((AvrcpConnInstance_t *)CsrCmnListSearchOffsetUint32((listPtr), offsetof(AvrcpConnInstance_t, control.btConnId), ccid))
#define AVRCP_LIST_CONN_GET_B_CID(listPtr, bcid)    ((AvrcpConnInstance_t *)CsrCmnListSearchOffsetUint32((listPtr), offsetof(AvrcpConnInstance_t, browsing.btConnId), bcid))

/* Pending TX data list (pendingTxDataList) */
#define AVRCP_LIST_TXDATA_ADD_FIRST(listPtr)        ((AvrcpPendingData_t *)CsrCmnListElementAddFirst((listPtr), sizeof(AvrcpPendingData_t)))
#define AVRCP_LIST_TXDATA_ADD_LAST(listPtr)         ((AvrcpPendingData_t *)CsrCmnListElementAddLast((listPtr), sizeof(AvrcpPendingData_t)))
#define AVRCP_LIST_TXDATA_REMOVE(listPtr, elemPtr)  (CsrCmnListElementRemove((listPtr), (CsrCmnListElm_t *)(elemPtr)))
#define AVRCP_LIST_TXDATA_GET_FIRST(listPtr)        ((AvrcpPendingData_t *)CsrCmnListGetFirst((listPtr)))

/* TG specific Media Player list */
#define AVRCP_LIST_TG_MP_ADD_FIRST(listPtr)         ((CsrBtAvrcpTgMp *)CsrCmnListElementAddFirst((listPtr), sizeof(CsrBtAvrcpTgMp)))
#define AVRCP_LIST_TG_MP_ADD_LAST(listPtr)          ((CsrBtAvrcpTgMp *)CsrCmnListElementAddLast((listPtr), sizeof(CsrBtAvrcpTgMp)))
#define AVRCP_LIST_TG_MP_REMOVE(listPtr, elemPtr)   (CsrCmnListElementRemove((listPtr), (CsrCmnListElm_t *)(elemPtr)))
#define AVRCP_LIST_TG_MP_GET_FIRST(listPtr)         ((CsrBtAvrcpTgMp *)CsrCmnListGetFirst((listPtr)))
#define AVRCP_LIST_TG_MP_GET_COUNT(listPtr)         (CsrCmnListGetCount((listPtr)))
#define AVRCP_LIST_TG_MP_GET_ID(listPtr, id)        ((CsrBtAvrcpTgMp *)CsrCmnListSearchOffsetUint32((listPtr), offsetof(CsrBtAvrcpTgMp, mpId), id))

/* Target specific pending message list */
#define AVRCP_LIST_TG_PMSG_ADD_FIRST(listPtr)       ((AvrcpTgPendingMsgInfo_t *)CsrCmnListElementAddFirst((listPtr), sizeof(AvrcpTgPendingMsgInfo_t)))
#define AVRCP_LIST_TG_PMSG_REMOVE(listPtr, elemPtr) (CsrCmnListElementRemove((listPtr), (CsrCmnListElm_t *)(elemPtr)))
#define AVRCP_LIST_TG_PMSG_GET_FIRST(listPtr)       ((AvrcpTgPendingMsgInfo_t *)CsrCmnListGetFirst((listPtr)))
#define AVRCP_LIST_TG_PMSG_GET_MSGID(listPtr, msgIdVal) ((AvrcpTgPendingMsgInfo_t *)CsrCmnListSearchOffsetUint32((listPtr), offsetof(AvrcpTgPendingMsgInfo_t, msgId), msgIdVal))

/* Controller specific pending message list */
#define AVRCP_LIST_CT_PMSG_ADD_FIRST(listPtr)       ((AvrcpCtPendingMsgInfo_t *)CsrCmnListElementAddFirst((listPtr), sizeof(AvrcpCtPendingMsgInfo_t)))
#define AVRCP_LIST_CT_PMSG_REMOVE(listPtr, elemPtr) (CsrCmnListElementRemove((listPtr), (CsrCmnListElm_t *)(elemPtr)))
#define AVRCP_LIST_CT_PMSG_GET_FIRST(listPtr)       ((AvrcpCtPendingMsgInfo_t *)CsrCmnListGetFirst((listPtr)))
#define AVRCP_LIST_CT_PMSG_GET_TLABEL(listPtr, tl)  ((AvrcpCtPendingMsgInfo_t *)CsrCmnListSearchOffsetUint8((listPtr), offsetof(AvrcpCtPendingMsgInfo_t, tLabel), tl))

/* Low Power mode control */
#define CSR_BT_AVRCP_LP_TIME                        (3*CSR_SCHED_SECOND)          /* Retry LP mode request after this delay */
/***** Type definitions *****/
/* Forward declarations */
struct AvrcpInstanceDataTag;
struct AvrcpConnInstanceTag;

/* Media player instances */
typedef struct AvrcpTgMpInstTag
{
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    struct AvrcpTgMpInstTag     *next;              /* Do not move - required by linked-list library */
    struct AvrcpTgMpInstTag     *prev;              /* Do not move - required by linked-list library */
#endif
    CsrSchedQid                   mpHandle;
    CsrUint32                    mpId;
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    CsrUint32                    notificationMask;   /* Notifications supported by the player */
    CsrUint32                    configMask;
    CsrUint8                     playStatus;         /**< Holds the play status of the MP when a control gets the MP list - temporary */

    /* PAS specific */
    CsrUint16                    pasLen;
    CsrUint8                     *pas;
#endif
    /* Player details */
    CsrUint8                     majorType;          /**< The major type of the player (\ref avrcp_tg_mp_major_types) */
    CsrUint32                    subType;            /**< The sub type of the player (\ref avrcp_tg_mp_major_types) */
    CsrBtAvrcpMpFeatureMask        featureMask;        /**< The features supported ny the media player (\ref avrcp_tg_mp_features) */
    CsrUtf8String                    *playerName;        /**< Pointer to the name of the media player (NUL-terminated)  */
} CsrBtAvrcpTgMp;

typedef struct AvrcpPendingDataTag
{
    struct AvrcpPendingDataTag  *next;              /* Do not move - required by linked-list library */
    struct AvrcpPendingDataTag  *prev;              /* Do not move - required by linked-list library */
    CsrUint16                    dataLen;
    CsrUint8                     *data;
    /* FIXME: Link to the corresponding pending msg */
} AvrcpPendingData_t; /* Used for pending outgoing and ingoing data */

typedef struct
{
    CsrBtConnId                 btConnId;                /* L2CAP connection ID of the specific connection */
    l2ca_mtu_t                  mtu;                /* Max frame size reported by the remote device */
    CsrUint8                     state;
    CsrUint8                     ctTLabel;

    /* Data specific */
    CsrBool                      dataSendAllowed;
    CsrCmnList_t                pendingTxDataList;  /* [AvrcpPendingData_t]  */
} CsrBtAvrcpConnDetails; /* Information about a single L2CAP connection */


typedef struct AvrcpCtPendingMsgInfoTag
{
    struct AvrcpCtPendingMsgInfoTag *next;              /* Do not move - required by linked-list library */
    struct AvrcpCtPendingMsgInfoTag *prev;              /* Do not move - required by linked-list library */
    CsrUint8                     tLabel;                 /* The TLabel of the latest transmitted command */
    CsrSchedTid                         timerId;
    void                        *cfmMsg;
    CsrUint8                     tmpState;                  /* Used when the CT is expecting multiple responses before sending a CFM to app */
    CsrUint16                    psm;                   /* Control or browsing channel */
    CsrSchedQid                   phandle;                /**< Where to send the confirmation/indication */
    struct AvrcpConnInstanceTag *connInst;              /* Link to associated connection instance */
} AvrcpCtPendingMsgInfo_t; /* List of pending controller specific messages */

typedef struct AvrcpTgPendingMsgInfoTag
{
    struct AvrcpTgPendingMsgInfoTag *next;              /* Do not move - required by linked-list library */
    struct AvrcpTgPendingMsgInfoTag *prev;              /* Do not move - required by linked-list library */
    CsrBtAvrcpPrim                 msgType;
    CsrSchedTid                         timerId;
    CsrUint32                    msgId;
    CsrUint8                     *rxData;
    CsrUint16                    psm;                   /* Control or browsing channel */
    struct AvrcpConnInstanceTag *connInst;              /* Link to associated connection instance */
} AvrcpTgPendingMsgInfo_t; /* List of pending target specific messages */


typedef struct
{
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    /* MP specific */
    CsrBtAvrcpTgMp                 *mpBrowsed;             /* Pointer to the currently browsed MP */
#endif
    CsrBtAvrcpTgMp                 *mpAddressed;           /* Pointer to the currently addressed MP */

    /* SDP specific */
    CsrUint16                    ctSdpAvrcpVersion;
    CsrUint16                    ctSdpSupportedFeatures; /* Direct copy of remote SDP record */

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    /* Notification specific */
    CsrUint32                    notificationsActive;
    CsrUint32                    notificationsPBInterval;
    CsrUint8                     notiList[CSR_BT_AVRCP_NOTI_ID_COUNT];     /* List of TLabels for registered notifications */
    /* FIXME: Add timer for playback position changed */

    /* For use with getting PlayStatus for media players */
    CsrBtAvrcpTgMp              *currentPlayer;
    CsrUint32                   itemCnt;
#endif
    /* More stuff */
    CsrCmnList_t                pendingMsgList;      /* [AvrcpTgPendingMsgInfo_t] */

    /* Continuing response */
    CsrUint8                     pduId;              /**< AVRCP_DATA_PDU_ID_GET_PAS_ATTRIBUTE_TEXT / AVRCP_DATA_PDU_ID_GET_PAS_VALUE_TEXT / AVRCP_DATA_PDU_ID_GET_ELEMENT_ATTRIBUTES*/
    CsrUint8                     packetType;         /**< AVRCP_DATA_MD_PACKET_TYPE_START|CONTINUE|END */

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
    /* PAS - handled entirely internally */
    CsrUint32                    attMask;
    CsrUint8                     attIdCount;         /* GetAttributeText */
    CsrBtAvrcpPasAttId             *attId;
    CsrUint8                     valIdFromAttId;     /* GetValueText */
    CsrUint8                     valIdCount;
    CsrBtAvrcpPasValId             *valId;

    /* GetElementAttribute - information from application */
    CsrBtAvrcpTgGetAttributesRes *getAttribResPrim;
    CsrUint16                    getAttribIndex;
#endif
} CsrBtAvrcpTgConnInfo; /* Information specific to a single connection and the local target */

typedef struct
{
    /* SDP info */
    CsrUint16                    tgSdpAvrcpVersion;
    CsrUint16                    tgSdpSupportedFeatures; /* Direct copy of remote SDP record */

    /* Notification specific */
    CsrUint32                    activeNotifications;
    CsrUint32                    tgSupportedNotifications;
    CsrUint32                    ctRequestedNotifications;
    CsrUint32                    playbackInterval;
    CsrUint32                    notiConfig;
    CsrUint8                     notiList[CSR_BT_AVRCP_NOTI_ID_COUNT];/* List of TLabels for registered notifications */

    /* Misc */
    AvrcpCtPendingMsgInfo_t     *pendingMsgInfo;        /* For use when the CT app should reply to an indication */
    CsrUint16                    mpAddressedId;
    CsrUint16                    mpBrowsedId;
    CsrCmnList_t                pendingMsgList;         /* [AvrcpCtPendingMsgInfo_t] */
} CsrBtAvrcpCtConnInfo; /* Information specific to a single connection and the local controller */

typedef struct AvrcpConnInstanceTag
{
    struct AvrcpConnInstanceTag *next;              /* Do not move - required by linked-list library */
    struct AvrcpConnInstanceTag *prev;              /* Do not move - required by linked-list library */

    /* Connection details */
    CsrBtDeviceAddr                address;            /* Address of the remote device */
    CsrUint8                     appConnId;          /* Connection ID to send to app - will start from 0 */
    CsrSchedTid                         reconnectTid;

    CsrBtAvrcpConnDetails          control;
    CsrBtAvrcpConnDetails          browsing;

    /* State details */
    CsrUint8                     sdpState;           /* Specifies whether SDP search is done, active or pending */
    CsrUint8                     connDirection;      /* Specifies the direction of the connection */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
    CsrBtAvrcpTgConnInfo           tgLocal;            /* Info about local target/remote controller */
#endif

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
    CsrBtAvrcpCtConnInfo           ctLocal;            /* Info about local controller/remote controller */
#endif

    /* AVCTP fragmentation - only relevant for control channel */
    CsrUint16                    pendingRxDataBufferLen; /* Must not be larger than 512+AVCTP header (3) == 515 */
    CsrUint8                     *pendingRxDataBuffer;

    /* Misc */
    CsrUint32                    remoteFeatures;
    struct AvrcpInstanceDataTag *instData;          /* Link to main instance data to avoid passing this between all functions */
} AvrcpConnInstance_t; /* Information related to a single remote device */

typedef struct
{
    /* Role configuration */
    CsrUint32                      configuration;

    /* SDP */
    CsrBtAvrcpConfigSrVersion      srAvrcpVersion;
    CsrBtAvrcpConfigSrFeatureMask  srFeatures;
    CsrUint32                      srHandle;

    CsrUint32                      tgMsgId;


    /* MP specific */
    CsrCmnList_t                   mpList;                     /* [CsrBtAvrcpTgMp] */
    CsrBtAvrcpTgMp                 *mpDefault;
        
    /* UID counter */
    CsrUint16                      uidCount;
} AvrcpTgMainInfo_t; /* Global (non connection specific) settings for the local target */

typedef struct
{
    /* Role configuration */
    CsrUint32                    configuration;

    /* SDP */
    CsrBtAvrcpConfigSrVersion      srAvrcpVersion;
    CsrBtAvrcpConfigSrFeatureMask  srFeatures;
    CsrUint32                    srHandle;
} AvrcpCtMainInfo_t; /* Global (non connection specific) settings for the local controller */

typedef struct AvrcpInstanceDataTag
{
    /* Control/state specific */
    CsrSchedQid                   ctrlHandle;                 /* Control application handle */
    CsrUint8                     appState;                   /* State of the application - normally IDLE/BUSY */
    CsrUint8                     activateStateCont;          /* Specifies whether incoming control connections are being accepted */
    CsrUint8                     activateStateBrow;          /* Specifies whether incoming browsing connections are being accepted */

    CsrMessageQueueType        *saveQueue;                 /* Stores pending messages */
    void                        *recvMsgP;                  /* Pointer to the received message. NULL if it has been passed on */
    CsrBool                      restoreFlag;                /* Specifies whether stored messages should be restored */

    /* Connection specific */
    CsrUint8                     incomingMaximum;
    CsrUint8                     incomingCurrent;
    CsrBool                      sniffReqResendAllowed;      /* Only retry sniff request one time */
    CsrUint8                     lpMode;                     /* Current LP mode of link */
    CsrUint8                     lpWantedMode;               /* Desired LP mode of link */
    CsrSchedTid                       lpTimer;                    /* Low-power retry timer */
    CsrUint16                    mtu;
    CsrBtAvrcpPrim               pendingCtrlPrim;            /* Holds the type of a single pending control prim (connect cfm, activate cfm etc...) */

    /* SDP search specific */
    CsrUint8                     sdpState;                   /* Specifies whether a SDP search is in progress or not */
    void                        *sdpSearchData;             /* Instance data for the common SDP search lib */

    /* SR (un)registration specific */
    CsrBtAvrcpConfigReq          *srPending;
    CsrUint8                     srActiveRole;
    CsrBtAvrcpRoleDetails          *tgDetails;
    CsrBtAvrcpRoleDetails          *ctDetails;

    /* Role specific settings */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
    AvrcpTgMainInfo_t           tgLocal;
#endif

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
    AvrcpCtMainInfo_t           ctLocal;
#endif

    /* Misc */
    CsrUint32                    globalConfig;
    CsrUint16                    srAvrcpVersionHighest;      /* Highest of the local TG/CT AVRCP versions */
    CsrCmnList_t                connList;                   /* [AvrcpConnInstance_t] List of active connection instances */
    dm_security_level_t         secIncomingCont;            /* Incoming security level - control channel */
    dm_security_level_t         secOutgoingCont;            /* Outgoing security level - control channel */
    dm_security_level_t         secIncomingBrow;            /* Incoming security level - browsing channel */
    dm_security_level_t         secOutgoingBrow;            /* Outgoing security level - browsing channel */
} AvrcpInstanceData_t;

/***** Prototypes *****/
/* csr_bt_avrcp_cmn_cm_handlers.c */
void CsrBtAvrcpCmL2caRegisterCfmHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmSdsRegisterCfmHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmSdsUnregisterCfmHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmL2caConnectAcceptCfmHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmL2caCancelConnectAcceptCfmHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmL2caConnectCfmHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmL2caDisconnectIndHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmModeChangeIndHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmL2caDataIndHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCmL2caDataCfmHandler(AvrcpInstanceData_t *instData);

/* csr_bt_avrcp_cmn_app_handlers.c */
void CsrBtAvrcpConfiqReqIdleState(AvrcpInstanceData_t *instData);
void CsrBtAvrcpActivateReqIdleState(AvrcpInstanceData_t *instData);
void CsrBtAvrcpDeactivateReqIdleState(AvrcpInstanceData_t *instData);
void CsrBtAvrcpConnectReqIdleState(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCancelConnectReqIdleBusyState(AvrcpInstanceData_t *instData);
void CsrBtAvrcpDisconnectReqIdleBusyState(AvrcpInstanceData_t *instData);
void CsrBtAvrcpSecurityInReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpSecurityOutReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpRemoteFeaturesIndSend(CsrSchedQid phandle, CsrUint8 connId, CsrBtDeviceAddr addr, CsrBtAvrcpRoleDetails *tgFeatures, CsrBtAvrcpRoleDetails *ctFeatures);

void CsrBtAvrcpConfigCfmSend(CsrSchedQid ctrlHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvrcpActivateCfmSend(CsrSchedQid ctrlHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvrcpDeactivateCfmSend(CsrSchedQid ctrlHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvrcpConnectIndSend(CsrSchedQid ctrlHandle,
                              CsrBtDeviceAddr *addr,
                              CsrUint16 frameSize,
                              CsrUint8 connId,
                              CsrBtConnId btConnId);

void CsrBtAvrcpConnectCfmSend(CsrSchedQid ctrlHandle,
                              CsrBtDeviceAddr *addr,
                              CsrUint16 frameSize,
                              CsrUint8 connId,
                              CsrBtAvrcpRoleDetails *tgDetails,
                              CsrBtAvrcpRoleDetails *ctDetails,
                              CsrBtResultCode resultCode,
                              CsrBtSupplier resultSupplier,
                              CsrBtConnId btConnId);

void CsrBtAvrcpDisconnectIndSend(CsrSchedQid ctrlHandle,
                                 CsrUint8 connId,
                                 CsrBtReasonCode reasonCode,
                                 CsrBtSupplier reasonSupplier,
                                 CsrBool localTerminated);

void CsrBtAvrcpSecurityInCfmSend(CsrSchedQid ctrlHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtAvrcpSecurityOutCfmSend(CsrSchedQid ctrlHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

/* csr_bt_avrcp_cmn_utils.c */
void CsrBtAvrcpMessagePut(CsrSchedQid phandle, void *msg);
void CsrBtAvrcpLpTimeoutHandler(AvrcpInstanceData_t *inst, AvrcpConnInstance_t *connInst);
void CsrBtAvrcpLpTimeout(CsrUint16 mi, void *mv);
void CsrBtAvrcpUtilNewConnEstablished(AvrcpInstanceData_t *instData, AvrcpConnInstance_t *connInst, CsrBool incoming);
void CsrBtAvrcpUtilConnectAccept(AvrcpInstanceData_t *instData);
CsrBool CsrBtAvrcpUtilConnectAcceptCancel(AvrcpInstanceData_t *instData);
void CsrBtAvrcpUtilSaveMessage(AvrcpInstanceData_t *instData);
void CsrBtAvrcpUtilGo2Busy(AvrcpInstanceData_t *instData, CsrBtAvrcpPrim primType);
void CsrBtAvrcpUtilGo2Idle(AvrcpInstanceData_t *instData);
CsrBool CsrBtAvrcpUtilCancelSavedMessage(AvrcpInstanceData_t *instData, CsrBtAvrcpPrim primType, CsrBtDeviceAddr *addr);
AvrcpConnInstance_t *CsrBtAvrcpUtilGetConnFromL2caCid(AvrcpInstanceData_t *instData, CsrBtConnId btConnId, CsrBtAvrcpConnDetails **connDetails);
AvrcpConnInstance_t *CsrBtAvrcpUtilConnAdd(AvrcpInstanceData_t *instData, CsrBtDeviceAddr *addr);
void CsrBtAvrcpUtilConnRemoveCB(CsrCmnListElm_t *elem);
void CsrBtAvrcpUtilFreeConfigReq(CsrBtAvrcpConfigReq **prim);
void CsrBtAvrcpUtilConnect(AvrcpConnInstance_t *connInst);
void CsrBtAvrcpUtilDisconnect(AvrcpConnInstance_t *connInst);
L2CA_FLOW_T *CsrBtAvrcpUtilGetFlow(l2ca_mtu_t mtu);

void CsrBtAvrcpUtilPendingDataRemoveCB(CsrCmnListElm_t *elem);
void CsrBtAvrcpUtilPendingDataAddLast(CsrBtAvrcpConnDetails *conn, CsrUint16 dataLen, CsrUint8 *data);
void CsrBtAvrcpUtilPendingDataSend(AvrcpConnInstance_t *connInst, CsrBtConnId connId);

/* csr_bt_avrcp_cmn_sdp.c */
CsrBool CsrBtAvrcpSdpRegisterSR(AvrcpInstanceData_t *instData);
CsrUint16 CsrBtAvrcpSdpGenerateServiceRecord(CsrBtAvrcpRoleDetails *ptr, CsrUint8 **sr, CsrUint8 role);
void CsrBtAvrcpSdpSearchStart(AvrcpInstanceData_t *instData, AvrcpConnInstance_t *connInst);
void CsrBtAvrcpSdpSearchCancel(AvrcpConnInstance_t *connInst);
void CsrBtAvrcpSdpResultHandler(void                     *inst,
                        CmnCsrBtLinkedListStruct  *sdpTagList,
                        CsrBtDeviceAddr         deviceAddr,
                        CsrBtResultCode          resultCode,
                        CsrBtSupplier      resultSupplier);
void CsrBtAvrcpSdpRestartSearch(AvrcpInstanceData_t *instData);

/* csr_bt_avrcp_cmn_data.c */
void AvrcpUtilMetaDataFrag(AvrcpInstanceData_t *instData, AvrcpConnInstance_t *connInst, CsrUint16 signalLen, CsrUint8 *signalData);
void AvrcpInfoSend(AvrcpInstanceData_t *instData, AvrcpConnInstance_t *connInst, CsrUint16 payloadLen, CsrUint8 *payload);
void CsrBtAvrcpDataInsertAvctpHeader(CsrUint8 *txData, CsrUint8 tLabel, CsrUint8 cr);
void CsrBtAvrcpDataInsertAvcCommonHeader(CsrUint8 *data, CsrUint8 crType);
void CsrBtAvrcpDataInsertAvcVendorHeader(CsrUint8 *data, CsrUint8 pduId, CsrUint8 packetType, CsrUint16 paramLen);
void CsrBtAvrcpDataInsertAvcPassThroughHeader(CsrUint8 *data, CsrUint8 state, CsrUint8 opId);
void CsrBtAvrcpDataInsertAvcGroupNavigationHeader(CsrUint8 *txData, CsrUint8 state, CsrUint16 operation);
void CsrBtAvrcpControlDataSend(AvrcpConnInstance_t *connInst, CsrUint16 dataLen, CsrUint8 *data);
void CsrBtAvrcpBrowsingDataSend(AvrcpConnInstance_t *connInst, CsrUint16 dataLen, CsrUint8 *data);
void CsrBtAvrcpDataVendorDataInsert(CsrUint8 *txData, CsrUint8 tLabel, CsrUint8 cr, CsrUint8 crType, CsrUint8 pduId, CsrUint16 paramLen);
void CsrBtAvrcpDataInsertBrowsingHeader(CsrUint8 *txData, CsrUint8 tLabel, CsrUint8 cr, CsrUint8 pduId, CsrUint16 paramLen);
void CsrBtAvrcpDataInsertControlHeader(CsrUint8 *txData,
                                       CsrUint8 tLabel,
                                       CsrUint8 cr,
                                       CsrUint8 cType,
                                       CsrUint8 subunitType,
                                       CsrUint8 subunitId,
                                       CsrUint8 opcode,
                                       CsrUint8 pduId,
                                       CsrUint16 paramLen);
void CsrBtAvrcpDataSimpleVendorFrameSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 cr, CsrUint8 crType, CsrUint8 pduId);
CsrBool CsrBtAvrcpUtilDataCheckAvctp(CsrUint16 rxDataLen, CsrUint8 *rxData);
CsrBool CsrBtAvrcpUtilDataCheckAVC(CsrUint16 rxDataLen, CsrUint8 *rxData);
CsrBool CsrBtAvrcpUtilDataCheckVendor(CsrUint16 rxDataLen, CsrUint8 *rxData);
CsrBool CsrBtAvrcpUtilDataCheckPT(CsrUint16 rxDataLen, CsrUint8 *rxData);
CsrBool CsrBtAvrcpUtilDataCheckBrowsing(CsrUint16 rxDataLen, CsrUint8 *rxData);
CsrBool CsrBtAvrcpUtilDataFragRxHandle(AvrcpConnInstance_t *connInst, CsrUint16 *rxDataLen, CsrUint8 **rxData);
void CsrBtAvrcpUtilDataFragRxFree(AvrcpConnInstance_t *connInst);

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
/* csr_bt_avrcp_tg_app_handlers.c */
void CsrBtAvrcpTgMpRegisterReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgMpUnregisterReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgNotiReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgPasSetReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgSetAddressedPlayerReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgNotiResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgPlayResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgSearchResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgChangePathResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgPassThroughResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgPasSetResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgPasCurrentResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgSetVolumeResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgGetFolderItemsResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgGetAttributesResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgGetPlayStatusResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgSetAddressedPlayerResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgSetBrowsedPlayerResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgAddToNowPlayingResHandler(AvrcpInstanceData_t *instData);

void CsrBtAvrcpTgMpRegisterCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtAvrcpTgMpUnregisterCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtAvrcpTgSetAddressedPlayerCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtAvrcpTgNotiIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 notiType, CsrUint32 playbackInterval);
void CsrBtAvrcpTgPlayIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCounter, CsrUint8 *uid);
void CsrBtAvrcpTgSearchIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrCharString *text);
void CsrBtAvrcpTgPasCurrentIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attIdCount, CsrBtAvrcpPasAttId *attId);
void CsrBtAvrcpTgChangePathIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpFolderDirection folderDir, CsrUint8 *folderUid);
void CsrBtAvrcpTgPassThroughIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 opId, CsrUint8 state);
void CsrBtAvrcpTgPasSetIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attValPairCount, CsrBtAvrcpPasAttValPair* changedPas);
void CsrBtAvrcpTgSetVolumeIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 volume);
void CsrBtAvrcpTgGetFolderItemsIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 scope, CsrUint32 startItem, CsrUint32 endItem, CsrUint32 attribMask, CsrUint32 maxData);
void CsrBtAvrcpTgGetPlayStatusIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp);
void CsrBtAvrcpTgSetAddressedPlayerIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp);
void CsrBtAvrcpTgSetBrowsedPlayerIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp);
void CsrBtAvrcpTgGetAttributesIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpTgMp *mp, CsrBtAvrcpItemAttMask attribMask, CsrUint32 maxData, CsrBtAvrcpScope scope, CsrBtAvrcpUid uid, CsrUint16 uidCounter);
void CsrBtAvrcpTgAddToNPLIndSend(AvrcpTgPendingMsgInfo_t *pendingMsgInfo, CsrUint8 scope, CsrUint16 uidCounter, CsrUint8 *uid);
void CsrBtAvrcpTgInformDispCharsetIndSend(CsrUint8 conId, CsrUint32 playerId, CsrSchedQid mpHandle, CsrUint8 charsetCount, CsrBtAvrcpCharSet *charset);
void CsrBtAvrcpTgBatteryStatusOfCtIndSend(CsrUint8 conId, CsrSchedQid mpHandle, CsrUint32 playerId, CsrUint8 battLevel);

void CsrBtAvrcpTgNotiCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtAvrcpNotiId notiType, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvrcpTgPasSetCfmSend(CsrUint32 playerId, CsrSchedQid phandle,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

/* csr_bt_avrcp_tg_msg_handlers.c */
void CsrBtAvrcpTgRejectAvcSend(AvrcpConnInstance_t *connInst, CsrUint8 *rxData, CsrUint8 rType);
void CsrBtAvrcpTgRejectAvcVendorSend(AvrcpConnInstance_t *connInst, CsrUint8 *rxData, CsrUint8 rType, CsrUint8 status);
void CsrBtAvrcpTgGeneralRejectBrowsingSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 status);
void CsrBtAvrcpTgNormalRejectBrowsingSend(AvrcpConnInstance_t *connInst, CsrUint8 *rxData, CsrUint8 status);
void CsrBtAvrcpTgRegisterNotificationPasRspSend(AvrcpConnInstance_t *connInst, CsrUint8 rType, CsrUint8 pairsCount, CsrBtAvrcpPasAttValPair *pairs);
void CsrBtAvrcpTgGetElementAttributesRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel);
void CsrBtAvrcpTgGetItemAttributesRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint16 txDataLen, CsrUint8 *txData, CsrUint8 attribCount, CsrUint8 status);

void CsrBtAvrcpTgRegisterNotificationRspSend(AvrcpConnInstance_t *connInst, CsrUint8 notiId, CsrUint8 *notiData, CsrUint8 rspType, CsrBool ApChanged);
void CsrBtAvrcpTgGetPlayStatusRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint32 songLength, CsrBtAvrcpPlaybackPos songPosition, CsrBtAvrcpPlaybackStatus playStatus);

void CsrBtAvrcpTgSetAddressedPlayerRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint8 cType);
void CsrBtAvrcpTgSetBrowsedPlayerRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint16 uidCounter, CsrUint32 itemsCount, CsrUint8 folderDepth, CsrUint16 foldersNameLen, CsrUint8 *folderNames);

void CsrBtAvrcpTgPlayRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status);
void CsrBtAvrcpTgSearchRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint16 uidCount, CsrUint32 itemCount);
void CsrBtAvrcpTgChangePathRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status, CsrUint32 itemCount);
void CsrBtAvrcpTgSetVolumeRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint8 volume);
void CsrBtAvrcpTgGetFolderItemsRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrUint16 txDataLen, CsrUint8 *txData, CsrUint16 itemCount, CsrUint16 uidCounter);
void CsrBtAvrcpTgAddToNPLRspSend(AvrcpConnInstance_t *connInst, CsrUint8 tLabel, CsrBtAvrcpStatus status);

void CsrBtAvrcpTgRxControlHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData);
void CsrBtAvrcpTgRxBrowsingHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData);

/* csr_bt_avrcp_tg_util.c */
void CsrBtAvrcpTgPendingMsgTimeout(CsrUint16 dummy, void *pendingMsg);
AvrcpTgPendingMsgInfo_t *CsrBtAvrcpTgUtilMsgQueueAdd(AvrcpConnInstance_t *connInst, psm_t psm, CsrBtAvrcpPrim msgType, CsrTime delay, CsrUint16 rxDataLen, CsrUint8 **rxData);
void CsrBtAvrcpTgUtilPendingMsgUpdate(AvrcpTgPendingMsgInfo_t *pendingMsgInfo);
CsrBtAvrcpTgMp *CsrBtAvrcpTgUtilMpListAdd(AvrcpInstanceData_t *instData, CsrBtAvrcpTgMpRegisterReq *req);
void CsrBtAvrcpTgUtilAvailableMPChanged(AvrcpInstanceData_t *instData);
void CsrBtAvrcpTgUtilMpListUpdateAddressed(AvrcpInstanceData_t *instData, CsrBtAvrcpTgMp *mp, CsrUint16 uidCounter);
void CsrBtAvrcpTgUtilMpListUpdateBrowsed(AvrcpConnInstance_t *connInst, CsrBtAvrcpTgMp *mp);
void CsrBtAvrcpTgUtilContinuingRspReset(AvrcpConnInstance_t *connInst, CsrBool completeReset);
void CsrBtAvrcpTgUtilMsgQueueRemoveCB(CsrCmnListElm_t *elem);
void CsrBtAvrcpTgUtilMpListAddCB(CsrCmnListElm_t *elem);
void CsrBtAvrcpTgUtilMpListRemoveCB(CsrCmnListElm_t *elem);
void CsrBtAvrcpTgUtilInitConnLocal(AvrcpInstanceData_t *instData, CsrBtAvrcpTgConnInfo *connInfo);
void CsrBtAvrcpTgUtilGetFolderItemsMPListBuild(AvrcpInstanceData_t *instData, CsrUint16 *txDataLen, CsrUint8 **txData, CsrUint32 startIdx, CsrUint32 endIdx);
CsrUint8 CsrBtAvrcpTgUtilSBPFolderNamesAdd(CsrUint8 **data, CsrUint16 *dataLen, CsrCharString *folderNames);
#endif


#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
/* csr_bt_avrcp_ct_app_handlers.c */
void CsrBtAvrcpCtInformDispCharsetReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtNotiRegisterReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPassThroughReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasAttIdReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasValIdReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasAttTxtReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasAttTxtResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasValTxtReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasValTxtResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasCurrentReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPasSetReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtGetPlayStatusReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtGetAttributesReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtGetAttributesResHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtGetFolderItemsReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtPlayReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtSearchReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtChangePathReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtSetVolumeReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtSetSSVolumeReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtSetAddressedPlayerReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtSetBrowsedPlayerReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtAddToNowPlayingReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtInformBatteryStatusReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtUnitInfoCmdReqHandler(AvrcpInstanceData_t *instData);
void CsrBtAvrcpCtSubUnitInfoCmdReqHandler(AvrcpInstanceData_t *instData);

CsrBtAvrcpCtInformDispCharsetCfm *CsrBtAvrcpCtInformDispCharsetCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtNotiRegisterCfm *CsrBtAvrcpCtNotiRegisterCfmBuild(CsrUint8 connId, CsrUint32 playbackInterval,CsrBtAvrcpNotiMask notiMask);
CsrBtAvrcpCtPasAttIdCfm *CsrBtAvrcpCtPasAttIdCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtPasValIdCfm *CsrBtAvrcpCtPasValIdCfmBuild(CsrUint8 connId, CsrBtAvrcpPasAttId attId);
CsrBtAvrcpCtPasAttTxtCfm *CsrBtAvrcpCtPasAttTxtCfmBuild(CsrUint8 connId);
void CsrBtAvrcpCtPasAttTxtIndSend(CsrUint8 connId, CsrUint16 pasDataLen, CsrUint8 *pasData, CsrSchedQid phandle);
CsrBtAvrcpCtPasValTxtCfm *CsrBtAvrcpCtPasValTxtCfmBuild(CsrUint8 connId, CsrBtAvrcpPasAttId attId);
void CsrBtAvrcpCtPasValTxtIndSend(CsrUint8 connId, CsrBtAvrcpPasAttId attId, CsrUint16 pasDataLen, CsrUint8 *pasData, CsrSchedQid phandle);
CsrBtAvrcpCtPasCurrentCfm *CsrBtAvrcpCtPasCurrentCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtPasSetCfm *CsrBtAvrcpCtPasSetCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtPassThroughCfm *CsrBtAvrcpCtPassThroughCfmBuild(CsrUint8 appConnId, CsrUint8 opId, CsrUint8 state);
void CsrBtAvrcpCtNotiIndSend(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 *rxData);
CsrBtAvrcpCtGetPlayStatusCfm *CsrBtAvrcpCtGetPlayStatusCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtGetAttributesCfm *CsrBtAvrcpCtGetAttributesCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid uid);
void CsrBtAvrcpCtGetAttributesIndSend(CsrSchedQid phandle, CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid *uid, CsrUint8 attribCount, CsrUint16 attribDataLen, CsrUint8 *attribData, CsrUint16 offset);
CsrBtAvrcpCtGetFolderItemsCfm *CsrBtAvrcpCtGetFolderItemsCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrUint32 startItem, CsrUint32 endItem);
CsrBtAvrcpCtPlayCfm *CsrBtAvrcpCtPlayCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid *uid);
CsrBtAvrcpCtSearchCfm *CsrBtAvrcpCtSearchCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtChangePathCfm *CsrBtAvrcpCtChangePathCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtSetVolumeCfm *CsrBtAvrcpCtSetVolumeCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtSetAddressedPlayerCfm *CsrBtAvrcpCtSetAddressedPlayerCfmBuild(CsrUint8 connId, CsrUint32 playerId);
CsrBtAvrcpCtSetBrowsedPlayerCfm *CsrBtAvrcpCtSetBrowsedPlayerCfmBuild(CsrUint8 connId, CsrUint32 playerId);
CsrBtAvrcpCtAddToNowPlayingCfm *CsrBtAvrcpCtAddToNowPlayingCfmBuild(CsrUint8 connId, CsrBtAvrcpScope scope, CsrBtAvrcpUid *uid);
CsrBtAvrcpCtInformBatteryStatusCfm *CsrBtAvrcpCtInformBatteryStatusCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtUnitInfoCmdCfm *CsrBtAvrcpCtUnitInfoCmdCfmBuild(CsrUint8 connId);
CsrBtAvrcpCtSubUnitInfoCmdCfm *CsrBtAvrcpCtSubUnitInfoCmdCfmBuild(CsrUint8 connId);

/* csr_bt_avrcp_ct_msg_handlers.c */
void CsrBtAvrcpCtInformDispCharsetCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 charsetCount, CsrBtAvrcpCharSet *charset);
void CsrBtAvrcpCtPassThroughCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 opId, CsrUint8 state);
void CsrBtAvrcpCtGroupNavigationCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 operation, CsrUint8 state);
void CsrBtAvrcpCtGetCapabilitiesCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 capaType);
void CsrBtAvrcpCtRegisterNotificationCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 notificationId, CsrUint32 interval);
void CsrBtAvrcpCtPasListAttCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo);
void CsrBtAvrcpCtPasListValCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpPasAttId attId);
void CsrBtAvrcpCtPasGetAttTextCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attIdCount, CsrBtAvrcpPasAttId *attId);
void CsrBtAvrcpCtPasGetValTextCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpPasAttId attId, CsrUint8 valIdCount, CsrBtAvrcpPasValId *valId);
void CsrBtAvrcpCtPasGetCurrentValCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 attIdCount, CsrBtAvrcpPasAttId *attId);
void CsrBtAvrcpCtPasSetValCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 changedPasCount, CsrBtAvrcpPasAttValPair *changedPas);
void CsrBtAvrcpCtGetElementAttributesCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint32 attributeMask);

void CsrBtAvrcpCtGetPlayStatusCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo);
void CsrBtAvrcpCtRequestAbortContinuationCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBool proceed, CsrUint8 pduId);
void CsrBtAvrcpCtSetAbsoluteVolumeCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint8 volume);
void CsrBtAvrcpCtSetAddressedPlayerCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 playerId);
void CsrBtAvrcpCtSetBrowsedPlayerCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 playerId);
void CsrBtAvrcpCtGetFolderItemsCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint32 startItem, CsrUint32 endItem, CsrBtAvrcpItemAttMask attributeMask);
void CsrBtAvrcpCtChangePathCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 uidCount, CsrBtAvrcpFolderDirection direction, CsrBtAvrcpUid *folderUid);
void CsrBtAvrcpCtGetItemAttributesCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCount, CsrBtAvrcpUid *itemUid, CsrBtAvrcpItemAttMask attributeMask);
void CsrBtAvrcpCtPlayItemCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCount, CsrBtAvrcpUid *itemUid);
void CsrBtAvrcpCtSearchCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpCharSet charset, CsrUtf8String *text);
void CsrBtAvrcpCtAddToNPLCmdSend(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrBtAvrcpScope scope, CsrUint16 uidCount, CsrBtAvrcpUid *itemUid);
void CsrBtAvrcpCtInformBatterySend(AvrcpConnInstance_t *connInst,AvrcpCtPendingMsgInfo_t *pendingMsgInfo,CsrBtAvrcpBatteryStatus batStatus);
void CsrBtAvrcpCtUnitInfoCmdSend(AvrcpConnInstance_t *connInst,AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 pDataLen, CsrUint8 *pData);
void CsrBtAvrcpCtSubUnitInfoCmdSend(AvrcpConnInstance_t *connInst,AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrUint16 pDataLen, CsrUint8 *pData);

void CsrBtAvrcpCtRxBrowsingHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData);
void CsrBtAvrcpCtRxControlHandler(AvrcpConnInstance_t *connInst, CsrUint16 rxDataLen, CsrUint8 **rxData);

/* csr_bt_avrcp_ct_util.c */
void CsrBtAvrcpCtCompletePendingMsg(AvrcpCtPendingMsgInfo_t *pendingMsgInfo);
void CsrBtAvrcpCtPendingMsgTimeout(CsrUint16 dummy, void *pendingMsg);

void CsrBtAvrcpCtPendingMsgUpdateResult(AvrcpCtPendingMsgInfo_t *pendingMsg,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);

void CsrBtAvrcpCtUtilPendingMsgCompleteFromPsm(AvrcpConnInstance_t *connInst,
    CsrUint16 psm, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvrcpCtUtilGetAvailableTLabel(AvrcpConnInstance_t *connInst, CsrBtAvrcpConnDetails *conn);
AvrcpCtPendingMsgInfo_t *CsrBtAvrcpCtUtilMsgQueueAdd(AvrcpConnInstance_t *connInst, void *cfmMsg, CsrSchedQid phandle, CsrUint16 psm);
void CsrBtAvrcpCtUtilMsgQueueRemoveCB(CsrCmnListElm_t *elem);
void CsrBtAvrcpCtResetPendingMsg(CsrBtAvrcpConnDetails *conn, AvrcpCtPendingMsgInfo_t *pendingMsgInfo, CsrTime time);

CsrBtResultCode CsrBtAvrcpCtUtilCheckRemoteRdy(AvrcpConnInstance_t *connInst, CsrUint16 avrcpVersion, CsrBool control);

void CsrBtAvrcpCtUtilConvertAVCRspType(CsrUint8 rspType,
    CsrBtResultCode *resultCode, CsrBtSupplier *resultSupplier);

void CsrBtAvrcpCtUtilConvertBrowsingRspType(CsrUint8 rspType,
    CsrBtResultCode *resultCode, CsrBtSupplier *resultSupplier);

CsrBool CsrBtAvrcpCtRegisterNextNotification(AvrcpConnInstance_t *connInst, AvrcpCtPendingMsgInfo_t *pendingMsgInfo);
void CsrBtAvrcpCtUtilInitConnLocal(CsrBtAvrcpCtConnInfo *connInfo);
CsrUint16 CsrBtAvrcpCtSBPFolderNamesGet(CsrUint8 **folderPathName, CsrUint8 *inputData, CsrUint8 depth);

#endif

/* Generic downsteam free function */
void CsrBtAvrcpFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif
