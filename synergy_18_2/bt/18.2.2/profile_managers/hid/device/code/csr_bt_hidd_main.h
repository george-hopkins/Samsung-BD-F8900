#ifndef CSR_BT_HIDD_MAIN_H__
#define CSR_BT_HIDD_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_cm_prim.h"
#include "csr_message_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------- Internal defines and types --------- */
/* hidd states */
typedef CsrUint32 HiddStatesType;
#define HIDD_INIT_STATE                     (HiddStatesType)0x00
#define HIDD_IDLE_STATE                     (HiddStatesType)0x01
#define HIDD_REGISTERING_SDP                (HiddStatesType)0x02
#define HIDD_ACCEPT_CONNECTING_STATE        (HiddStatesType)0x03
#define HIDD_CONNECTING_STATE               (HiddStatesType)0x04
#define HIDD_UNREGISTERING_SDP              (HiddStatesType)0x05
#define HIDD_CONNECTED_STATE                (HiddStatesType)0x06
#define HIDD_NOT_CONNECTED_STATE            (HiddStatesType)0x07
#define HIDD_NUMBER_OF_STATES               (HiddStatesType)0x08

typedef struct
{
    CsrUint32        recordHandle;
    CsrUint16        recordLen;
    CsrUint8         *record;
}HiddSdpInfo;

typedef struct
{
    L2CA_QOS_T       *qos;
    CsrUint8*        recvMsg;
    CsrUint8*        sendMsg;
    CsrUint16        recvMsgLength;
    CsrUint16        sendMsgLength;
    CsrUint16        sendMsgOffset;
    CsrBool          modeChangeSuccess;
    CsrUint8         reportType;
    l2ca_mtu_t       hostMtu;
    l2ca_mtu_t       localMtu;
    CsrBtConnId      cid;
}HiddChInfo;

/* hidd instance data */
typedef struct
{
    void*                   pRecvMsg;           /* Pointer to the received message. NULL if it has been passed on */
    CsrMessageQueueType*   saveQueue;          /* Saved message */
    CsrMessageQueueType*   saveCmQueue;        /* Saved CM message */
    CsrSchedQid               appHandle;          /* application handle */
    CsrSchedQid               myAppHandle;        /* appHandle of my task running in the scheduler - for multiple instances */
    BD_ADDR_T               bdAddr;             /* the bluetooth address of the device to connect to */
    HiddSdpInfo             deviceIdSdp;        /* info of device id service record */
    HiddSdpInfo             hidSdp;             /* info of hid service record */
    HiddChInfo              ctrlCh;             /* control channel info */
    HiddChInfo              intrCh;             /* interrupt channel info */
    CsrSchedTid                     timerId;            /* timer identifier */
    CsrUint16                flushTimeout;       /* flush timeout for l2cap channels */
    CsrUint16                currentMode;        /* the current low power mode */
    CsrUint16                nextMode;           /* the requested low power mode when previously disconnected */
    CsrBtResultCode         prevError;          /* request failed */
    CsrBtResultCode         prevErrorSupplier;  /* request failed */
    CsrUint8                 hidFlags;           /* flags specifying how hid profile behaves */
    CsrUint8                 numOfRetries;       /* number of attempts to un-/register service record */
    CsrUint8                 disconnectReason;   /* the reason for disconnecting when connected */
    CsrUint8                 cancelChCount;      /* number of cancelled channels confirmed */
    CsrBool                  reconnect;          /* connection reestablishing */
    CsrBool                  deactivating;       /* deactivate requested */
    CsrBool                  cleanUp;            /* clean up after application is gone */
    CsrBool                  restoreHiddFlag;    /* indicate whether to restore mgs queue */
    CsrBool                  restoreCmFlag;      /* indicate whether to restore mgs queue */
    CsrBool                  active;             /* indicate whether service is active or not */
    CsrBool                  registeringNewRec;  /* indicate wether a new service record is being registered */
    CsrBool                  newRecUnregCfmRec;  /* indicate wether the first CSR_BT_CM_SDS_UNREGISTER_CFM (of the two expected) is received while registering a new service record */
    CsrBool                  deactUnregCfmRec;   /* indicate wether the first CSR_BT_CM_SDS_UNREGISTER_CFM (of the two expected) is received while deactivating */
    HiddStatesType          state;              /* the state of the profile */
    dm_security_level_t     secIncoming;        /* incoming security level */
    dm_security_level_t     secOutgoing;        /* outgoing security level */
} HiddInstanceDataType;

typedef void (* HiddStateHandlerType)(HiddInstanceDataType * hiddInstanceData);

extern void CsrBtHiddCleanUpToIdle(HiddInstanceDataType *instData);

#define CSR_BT_HIDD_SUB_STATE_CHANGE(y,x) CSR_BT_HIDD_STATE_CHANGE(instData->subInstData[(y)].subState, (x))

#ifdef CSR_LOG_ENABLE
#define CSR_BT_HIDD_STATE_CHANGE(var, val) CsrBtHiddChangeState((CsrCharString*) __FILE__, __LINE__, (CsrCharString*) #var, &(var), val, (CsrCharString*) #val)

void CsrBtHiddChangeState(const CsrCharString *file, CsrUint32 line,
                              const CsrCharString *fsmName,
                              CsrUint32 *state, CsrUint32 newState, const CsrCharString *newStateName);
#else
#define CSR_BT_HIDD_STATE_CHANGE(var, val) (var = val)
#endif

#ifdef __cplusplus
}
#endif
#endif /* CSR_BT_HIDD_MAIN_H__ */

