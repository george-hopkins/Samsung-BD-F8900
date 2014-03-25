#ifndef CSR_BT_SAPS_MAIN_H__
#define CSR_BT_SAPS_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "rfcomm_prim.h"
#include "bluetooth.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "hci_prim.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "sds_prim.h"
#include "csr_bt_saps_prim.h"
#include "csr_message_queue.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Saps states */
typedef CsrUint32                SapsStates_t;
#define Init_s                  0x00
#define Idle_s                  0x01
#define Activated_s             0x02
#define Connected_s             0x03
#define SapConnected_s          0x04
#define Disconnecting_s         0x05
#define Deactivate_s            0x06
#define SapsNumberOfStates      0x07

/* Connect states */
typedef CsrUint32                SapsConnectStates_t;
#define BtConnect_s             0x00
#define ConnectIndSend_s        0x01
#define ConnectResSend_s        0x02
#define ConnectDisconnect_s     0x03

#define NAX_NUM_RETRIES         3
#define SNIFF_DELAY             1000000

typedef struct
{
    /* Instance variables */
    CsrSchedQid                         appHandle;                /* protocol handle */
    SapsStates_t                state;                    /* my state */
    SapsConnectStates_t         connectState;             /* flag to indicate if we are SAP connected. Set to TRUE when server has accepted maxMsgSize */
    CsrBtReasonCode             discReason;               /* holds reasonCode from the CM during SAPS_CONNECT_IND og CM_DISCONNECT_IND xover */
    void                        *recvMsgP;                /* pointer to the received message. NULL if it has been passed on */
    CsrUint32                   sapsConnId;
    CsrUint8                    serverChannel;            /* allocated (local) server channel */
    CsrBtDeviceAddr             deviceAddr;
    CsrUint32                   serviceRecHandle;         /* handle to the SAP service record */
    CsrBool                     activateSaps;             /* flag to indicate if Saps has been activated in Init_s */
    CsrBool                     serverDisconnect;         /* flag to indicate if the server has requested IMMEDIATE disconnect and should CSR_BT_CM_DISCONNECT_REQ */
    CsrBool                     serverDeactivated;        /* flag to indicate that the server has been deactivated */
    CsrUint16                   maxMsgSize;               /* CSRMAX message size accepted by the profile. Negotiated between Server/client */
    CsrUint8                    statusToSend;             /* Parameter to send in the status indication */
    CsrBool                     timerPending;             /* used to indicate if the mode change timer event is pending */
    CsrSchedTid                      timerId;                  /* used to save the CsrSchedTid of the timed event */
    CsrBool                     sapRegistered;            /* Used to indicate if the SAP servicerecord is already registered */
    CsrUint8                    numUnregRetries;          /* counter for number of times to retry unregistering service record */
    dm_security_level_t         secIncoming;              /* Incoming security level */

    /* Instance data used by send fragmentation */
    CsrUint16                    rfcMaxPacketSize;         /* CSRMAX packet size for Rfcomm connection */
    CsrUint8                     *sendBuffer;              /* buffer used by send fragmentation routine */
    CsrBool                      transmitReady;            /* flag to indicate if profile can transmit */
    CsrUint16                    payloadSize;              /* */
    CsrUint16                    payloadIndex;             /* */

    /* Instance data used by defragmentation algorithm */
    CsrUint8                     *recvBuffer;              /* */
    CsrUint16                    recvBufferLength;         /* */
    CsrBool                      headerComplete;           /* used to indicate if header was fragmented */

    /* Message queue */
    CsrBool                      restoreSapsFlag;
    CsrMessageQueueType        *saveQueue;
} SapsInstanceData_t;

typedef void (* SapsStateHandlerType)(SapsInstanceData_t * sapsInstanceData);

#ifdef __cplusplus
}
#endif

#endif        /*_SAPS_MAIN_H*/
