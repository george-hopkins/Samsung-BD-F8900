#ifndef CSR_BT_SC_MAIN_H__
#define CSR_BT_SC_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "dm_prim.h"
#include "csr_bt_usr_config.h"
#include "csr_message_queue.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_private_prim.h"
#ifdef CSR_BT_LE_ENABLE
#include "csr_bt_gatt_prim.h"
#include "csr_bt_gatt_lib.h"
#endif
#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#include "csr_log_text.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CSR_LOG_ENABLE
#define LOG_TEXT_TASK_ID    ((CsrLogTextTaskId) CSR_BT_SC_IFACEQUEUE)
#define LOG_TEXT_SO_SCDB    0
#define LOG_TEXT_SO_STATE   1
#endif /* CSR_LOG_ENABLE */

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#define CsrBtScStateEventException(theEventClass,theType,theState) CsrStateEventException("SC", (theEventClass), (theType), (theState))
#define CsrBtScStateEventExceptionOn(cond, theEventClass,theType,theState)  \
do {                                                                       \
    if (cond)                                                              \
    {                                                                      \
        CsrBtScStateEventException((theEventClass), (theType), (theState)); \
    }                                                                      \
} while(0)
#else
#define CsrBtScStateEventException(theEventClass,theType,theState) 
#define CsrBtScStateEventExceptionOn(cond, theEventClass,theType,theState) 
#endif


#define DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND (DM_SM_ACCESS_IND+1)

#define SC_INTERNAL_SUPERVISION_TIMER    ((CsrTime)     30000000)
#define SC_SERVICE_POOL_SIZE             ((CsrUintFast8)  20)
#define MAX_NO_OF_ACL                    ((CsrUintFast8)  10)

#define SC_AUTH_REQ_NONE                 ((CsrUint8) 0x00)
#define SC_AUTH_REQ_MITM                 ((CsrUint8) 0x01)
#define SC_AUTH_REQ_DEDICATED_BONDING    ((CsrUint8) 0x02)
#define SC_AUTH_REQ_GENERAL_BONDING      ((CsrUint8) 0x04)

typedef struct
{
    CsrBool                  inUse;
    uuid16_t                serviceUuid;
    CsrUint16                channel;
    uuid16_t                secLevel;
} scServiceRecord_t;

typedef struct ScServiceRecordStructTag
{
    CsrUint16                        numberOfMessagesInThisPool;
    struct ScServiceRecordStructTag *nextQueuePool;
    scServiceRecord_t               serviceRegister[SC_SERVICE_POOL_SIZE];
} ScServiceRecordType;

/* Main states used in the SC jump table */
typedef enum 
{
SC_ST_NULL = 0x00,
SC_ST_IDLE,
SC_ST_LOCAL_POST_LISBON_BONDING,
SC_ST_REMOTE_BONDING,
SC_ST_AUTHORISATION,
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
SC_ST_AUTHENTICATE,
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
SC_ST_ENCRYPT,
#endif
SC_ST_PENDING,
SC_ST_REBOND
} csr_bt_sc_state;

#define SC_MAX_NR_OF_STATES             (SC_ST_REBOND+1)

#define SC_NUM_DM_UPSTREAM_PRIMS        ((CsrUint16)(ENUM_DM_SM_EN_REMOVE_DEVICE_CFM-ENUM_DM_SM_INIT_CFM+1))

/* Denotes which DM primitives we're currently processing */
#define MSG_TYPE_NONE                   ((CsrUint8) 0x00)
#define MSG_TYPE_AUTHORISE              ((CsrUint8) 0x01)
#define MSG_TYPE_JUSTWORKS              ((CsrUint8) 0x02)
#define MSG_TYPE_COMPARE                ((CsrUint8) 0x03)
#define MSG_TYPE_PASSKEY                ((CsrUint8) 0x04)
#define MSG_TYPE_NOTIFICATION           ((CsrUint8) 0x05)
#define MSG_TYPE_LEGACY                 ((CsrUint8) 0x06)
#define MSG_TYPE_REBOND                 ((CsrUint8) 0x07)
#define MSG_TYPE_IO_CAPS                ((CsrUint8) 0x08)
#define MSG_TYPE_OOB                    ((CsrUint8) 0x09)
#define MSG_TYPE_LE_ENCRYPTION_CHANGED  ((CsrUint8) 0x0a)
#define MSG_TYPE_LE_WAIT_ENCR_ENABLE    ((CsrUint8) 0x0b) /* LE diversifier matched, wait for encryption change */

/* Who initiated the the pairing procedure */
#define INITIATOR_NONE                  ((CsrUint8) 0x00)
#define INITIATOR_BOND                  ((CsrUint8) 0x01)
#define INITIATOR_REMOTE                ((CsrUint8) 0x02)
#define INITIATOR_AUTHENTICATE          ((CsrUint8) 0x03)
#define INITIATOR_ENCRYPT               ((CsrUint8) 0x04)
#define INITIATOR_REBOND                ((CsrUint8) 0x05)

/* Application state */
#define APP_IDLE                        ((CsrUint8) 0x00)
#define APP_RESPOND                     ((CsrUint8) 0x01)
#define APP_RESPONDED                   ((CsrUint8) 0x02)

/* UI state */
#define UI_IDLE                         ((CsrUint8) 0x00)
#define UI_READ_DEV                     ((CsrUint8) 0x01)
#define UI_RNR                          ((CsrUint8) 0x02)
#define UI_COMPLETE                     ((CsrUint8) 0x03)
#define UI_CANCELLED                    ((CsrUint8) 0x04)

/* SC state */
#define SC_IDLE                         ((CsrUint8) 0x00)
#define SC_ACTIVE                       ((CsrUint8) 0x01)
#define SC_CANCELLED                    ((CsrUint8) 0x02)

/* DM state */
#define DM_IDLE                         ((CsrUint8) 0x00)
#define DM_PENDING                      ((CsrUint8) 0x01)
#define DM_CONNECTING                   ((CsrUint8) 0x02)
#define DM_IO_CAPS_RECV                 ((CsrUint8) 0x03)
#define DM_AUTHEN_RECV                  ((CsrUint8) 0x04)
#define DM_AUTHEN_RESP                  ((CsrUint8) 0x05)
#define DM_BOND_COMPLETE                ((CsrUint8) 0x06)

/* DM_SM_INIT state */
#define SM_INIT_ST_IDLE                 ((CsrUint8) 0x00) /* sleeping */
#define SM_INIT_ST_MODE                 ((CsrUint8) 0x01) /* set security mode */
#define SM_INIT_ST_DEBUG                ((CsrUint8) 0x02) /* toggle debug mode */
#define SM_INIT_ST_CONFIG               ((CsrUint8) 0x03) /* change config parameters */

typedef struct
{
    CsrUint8     localIoCaps;
    CsrUint8     remoteIoCaps;
    CsrUint8     localAuth;
    CsrUint32    remoteAuth;
    CsrUint32    numValue;
    CsrBool      localInitiator;
    CsrBool      bondAckSent;
    CsrUint8     pairingType;
    CsrUint8     initiator;

    CsrUint8     linkKey[SIZE_LINK_KEY];
    CsrUint8     linkKeyType;

    CsrUint8     passKeyLength;
    CsrUint8     passKey[CSR_BT_PASSKEY_MAX_LEN];

    CsrUint8     defaultBondable;
    CsrSchedTid  bondableTid;

    CsrSchedTid  wdTimer;

    /* States */
    CsrUint8     bondable;
    CsrUint8     scState;
    CsrUint8     uiState;
    CsrUint8     dmState;
    CsrUint8     appState;

#ifdef CSR_BT_LE_ENABLE
    CsrUint16                keyDistribution; /* LE key distribution */
    CsrUint16                leLocalAuth; /* LE authentication requirements */
    CsrUint16                leDefaultLocalAuth; /* LE authentication requirements */
    CsrBool                  lowEnergy; /* currently talking to LE device */
    l2ca_conflags_t          l2capFlags; /* L2CAP connection flags */
    CsrBtConnId              btConnId; /* GATT connection */

    /* Current LE keys */
    CsrUint16                leKeyValid;
    DM_SM_KEY_ENC_CENTRAL_T *leKeyEncCentral;
    DM_SM_KEY_ID_T          *leKeyId;
    DM_SM_KEY_SIGN_T        *leKeySign;
    CsrUint16                leKeyDiv;
    CsrUint32                leKeySignCounter;
    CsrUint16                leKeySize;
    CsrUint16                leSecurityRequirements;
    CsrSchedQid              leAppHandle;
    CsrUint8                 leRebondCnt;
#endif /* CSR_BT_LE_ENABLE */

} ScBondInstanceData;

typedef struct
{
    CsrUint16        writeAuthEnable;
    CsrUint16        config;
    CsrBtDeviceAddr  deviceAddr;
    CsrUint8         oobHashC[SIZE_OOB_DATA];
    CsrUint8         oobRandR[SIZE_OOB_DATA];
    CsrBool          remoteOobDataPresent;
    CsrBool          initRreq; /* what sort of init req is pending */
} ScSspInstanceData;

typedef struct
{
    CsrUint16    repairId;
} ScRepairInstanceData;

typedef struct
{
    CsrBtClassOfDevice         cod;
    CsrBtDeviceAddr            deviceAddr;
} ScAclListType;

#ifdef CSR_BT_LE_ENABLE
typedef struct CsrBtScLeLinksTag
{
    struct CsrBtScLeLinksTag *next;
    CsrBtTypedAddr addr;
    CsrUint16 encryptionKeySize;
} CsrBtScLeLinks;
#endif

typedef struct
{
    CsrSchedQid              appHandle;
    CsrSchedQid              mainAppHandle;
    CsrUint8                 state;
    CsrUint32                eventMask;
    CsrBtDeviceAddr          localAddr;
    CsrUint8                 dmSmInitState;

    TIME                     appTimeout;

    void                    *recvMsgP;
    CsrMessageQueueType     *saveQueue;
    CsrMessageQueueType     *ignoreQueue;
    CsrBool                  restoreMsgFlag;
    CsrUint8                 pendingMsgType;

    CsrUint8                 encryptionMode;
    CsrUint8                 securityMode;
    CsrBool                  encEnable;

    ScServiceRecordType     *rfcommServices;
    ScServiceRecordType     *l2capServices;
    CsrUint16                channel;
    dm_protocol_id_t         protocolId;
    CsrBool                  incoming;

    CsrBtClassOfDevice       cod;
    CsrBtDeviceAddr          deviceAddr;
    CsrBtAddressType         addressType;
    
    CsrBtDeviceName          friendlyName;
    CsrBool                  addDevice;
    CsrBool                  authorised;
    CsrBool                  accept;
    CsrUint8                 hciStatus;
    CsrUint8                 paired;

    ScAclListType            aclList[MAX_NO_OF_ACL];

    ScBondInstanceData       bondData;
    ScSspInstanceData        sspData;
    ScRepairInstanceData     repairData;
#ifdef CSR_BT_INSTALL_SC_CONFIG_MASK
    CsrBtScConfigMask        configMask;
#endif
    CsrSchedTid              bondTime;
    CsrBool                  acceptingBond;
    psm_t                    localPsm;
    CsrSchedQid              bondPhandle;
    CsrBtResultCode          cancelBondResult;

#ifdef CSR_BT_LE_ENABLE
    CsrBtGattId              gattId; /* GATT registration ID */
    CsrBtScLeLinks          *leLinks; /* db for storing encr key sizes */
#endif
} ScInstanceData_t;

typedef void (* StateHandlerType)(ScInstanceData_t * taskData);

/* Prototypes from sc_free_down.c */
void CsrBtScFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

/* Is current bond instance using low energy? */
#ifdef CSR_BT_LE_ENABLE
#define CSR_BT_SC_IS_LOW_ENERGY(inst) ((inst)->bondData.lowEnergy)
#define CSR_BT_SC_ADDR_TYPE(inst) ((CsrUint8)( ((inst)->bondData.lowEnergy) \
                                               ? (inst)->addressType \
                                               : CSR_BT_ADDR_PUBLIC) )
#define CSR_BT_SC_KEY_DIST(inst) ((inst)->bondData.keyDistribution)
#else
#define CSR_BT_SC_IS_LOW_ENERGY(inst) (FALSE)
#define CSR_BT_SC_ADDR_TYPE(inst) ((CsrUint8)CSR_BT_ADDR_PUBLIC)
#define CSR_BT_SC_KEY_DIST(inst) (0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* ndef _SC_MAIN_H */
