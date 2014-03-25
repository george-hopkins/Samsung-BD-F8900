#ifndef CSR_BT_HIDH_MAIN_H__
#define CSR_BT_HIDH_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#include "csr_bt_hidh_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"


/* maximum concurrent connected HID devices */
#define MAX_HID_CONNECTIONS                7

#define HID_MAJOR_DEV_CLASS                (0x000500)
#define NOT_FOUND                          0xFF

#ifdef __cplusplus
extern "C" {
#endif

/* HIDH main instance states */
#define IDLE                    (0x00)
#define ACTIVE                  (0x01)

/* states for connected HID device instances */
#define NULL_STATE              (0x10)
#define AWAIT_ESTABLISH         (0x11)
#define SDP_SEARCH              (0x12)
#define ESTABLISH_CTRL          (0x13)
#define ESTABLISH_INTR          (0x14)
#define CONNECTED               (0x15)
#define DISCONNECTING           (0x16)
#define CANCEL_ESTABLISH        (0x17)
#define UNPLUGGING              (0x18)
#define TIMEOUT_ESTABLISH       (0x19)

/* data queue element structure */
typedef struct q_element_tag_t
{
    CsrUint16                    length;
    void                        *data;
    struct q_element_tag_t      *next;
} q_element_t;

/* data structure related to a L2CAP channel (lower interface) */
typedef struct
{
    CsrBtConnId             cid;                               /* BT Global connection id */
    CsrUint16               mtu;                               /* remote maximum transmit unit size */
    CsrUint16               localMtu;                          /* local maximum transmit unit size */
    L2CA_QOS_T                  *qos;                              /* flow QoS */
    CsrBool                 sending;                           /* flag for outstanding data request */
    q_element_t            *qFirst;                           /* pointer to first entry in tx queue */
    q_element_t            *qLast;                            /* pointer to last entry in tx queue */
    CsrUint16               rxMsgCount;                        /* size of (defragmented) rx message */
    CsrUint16               rxMsgAllocCount;                   /* size allocated for defragmentation */
    CsrUint8                *rxMsg;                            /* pointer to defragmented message */
} l2ca_data_ch_t;

/* data structure for connected HID device instances */
typedef struct
{
    CsrUint32               subState;                          /* connect device state */
    CsrSchedQid                    userHandle;                        /* user application handle */
    CsrBool                 doTerminate;                       /* terminate this sub-instance (reset) */
    CsrBtDeviceAddr           bdAddr;                            /* address of connected device */
    CsrUint16               flushTimeout;                      /* flush timeout value (interrupt ch.) */
    l2ca_data_ch_t         ctrl_ch;                           /* L2CAP control channel structure */
    l2ca_data_ch_t         intr_ch;                           /* L2CAP interrupt channel structure */
    CsrBtHidhSdpInfo         sdpInfo;                           /* HID SDP information structure */
    CsrCharString               *serviceName;                      /* SDP service name */
    CsrUint16               descriptorLength;                  /* SDP descriptor length */
    CsrUint8                *descriptor;                       /* SDP raw descriptor */
    CsrBool                 isReconnect;                       /* flag indicating a reconnect */
    CsrBool                 useExtDataPath;                    /* flag if ext. interface should be used*/
    CsrSchedTid                    timerId;                           /* timer id for reconnect timer */
} hidh_subInstData_t;

/* main instance data structure */
typedef struct
{
    CsrUint32               state;                             /* main state of the HID host profile component */
    CsrSchedQid                    ctrlHandle;                        /* control application handle */
    CsrBtUuid32            hidServiceRecordHandle;            /* HID service search record handle */
    CsrBtUuid32            devIdServiceRecordHandle;          /* Device ID service search record handle */
    CsrUint16               sdpAttrib;                         /* SDP attribute being requested during search */
    CsrUint8                connectDeviceId;                   /* the device id of device being connected */
    CsrBool                 doTerminate;                       /* terminate all HID host activity (app. reset) */
    extConnector_t         extConnectorFct;                   /* Function pointer for connect-hook */
    extDisconnector_t      extDisconnectorFct;                /* Function pointer for disconnect-hook */
    extModeChange_t        extModeChangeFct;                  /* Function pointer for mode-change-hook */
    extSendMsg_t           extSendMsgFct;                     /* Function pointer to sending on ext. data path*/
    CsrUint8                *recvMsgP;                         /* received message reference */
    hidh_subInstData_t     subInstData[MAX_HID_CONNECTIONS];  /* a data sub-structure per connected device */
    CsrSchedQid                    slots[MAX_HID_CONNECTIONS];        /* user application slots */
    dm_security_level_t    secIncoming;                       /* incoming security level */
    dm_security_level_t    secOutgoing;                       /* outgoing security level */
} hidh_instData_t;

typedef void (* hidhStateHandler_t)(hidh_instData_t * instData);

#ifdef CSR_LOG_ENABLE
#define CSR_BT_HIDH_SUB_STATE_CHANGE(var, val) CsrBtHidhChangeState((CsrCharString*) __FILE__, __LINE__, (CsrCharString*)  "instData->subInstData[" #var "].subState", &(instData->subInstData[(var)].subState), val, (CsrCharString*) #val)

#define CSR_BT_HIDH_STATE_CHANGE(var, val) CsrBtHidhChangeState((CsrCharString*) __FILE__, __LINE__, (CsrCharString*) #var, &(var), val, (CsrCharString*) #val)

void CsrBtHidhChangeState(const CsrCharString *file, CsrUint32 line,
                              const CsrCharString *fsmName,
                              CsrUint32 *state, CsrUint32 newState, const CsrCharString *newStateName);
#else
#define CSR_BT_HIDH_SUB_STATE_CHANGE(var, val) ((instData->subInstData[(var)].subState) = val)
#define CSR_BT_HIDH_STATE_CHANGE(var, val) (var = val)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _HIDH_MAIN_H */
