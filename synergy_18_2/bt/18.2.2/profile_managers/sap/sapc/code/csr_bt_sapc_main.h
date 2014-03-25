#ifndef CSR_BT_SAPC_MAIN_H__
#define CSR_BT_SAPC_MAIN_H__

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
#include "csr_bt_sapc_prim.h"
#include "csr_message_queue.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sapc Main states */
typedef CsrUint8                   SapcStates_t;

#define Init_s                    ((SapcStates_t) 0x00)
#define Idle_s                    ((SapcStates_t) 0x01)
#define Search_s                  ((SapcStates_t) 0x02)
#define Connected_s               ((SapcStates_t) 0x03)
#define ConnectedFail_s           ((SapcStates_t) 0x04)
#define SapConnected_s            ((SapcStates_t) 0x05)
#define Disconnecting_s           ((SapcStates_t) 0x06)
#define SapcNumberOfStates        ((SapcStates_t) 0x07)

typedef enum {
    SapIdle_s,
    SapApdu_s,
    SapAtr_s,
    SapOff_s,
    SapOn_s,
    SapReset_s,
    SapCardRead_s,
    SapSetProtocol_s
} SapConnectedStates_t;

typedef struct
{
    /* instance variables */
    CsrSchedQid                    appHandle;                /* protocol handle */
    SapcStates_t                 state;                    /* my state */
    SapConnectedStates_t         SapState;                 /* Sub state in SapConnected_s */
    void                         *recvMsgP;                /* pointer to the received message. NULL if it has been passed on */
    CsrUint8                serverChannel;            /* Local server channel */
    CsrBtUuid32                     serviceHandle;            /* Service handle found during SDC search and saved for connect */
    BD_ADDR_T                    bdAddr;                   /* Address of the device to connect to */
    CsrUint32                     sapcConnId;               /* the connection id for the established connection */
    CsrUint16                     maxMsgSize;               /* CSRMAX SAP message size negotiated with server */
    CsrBool                       serverDisconnect;         /* flag to indicate if the server requested the disconnect */
    CsrBtResultCode              connectCfmResult;         /* result to send in connectCfm */
    dm_security_level_t          secOutgoing;              /* Outgoing security level */

    /* Variables for msg fragmentation */
    CsrUint16                     rfcMaxPacketSize;         /* CSRMAX packet size for Rfcomm connection */
    CsrUint8                      *sendBuffer;              /* buffer used by send fragmentation routine */
    CsrBool                       transmitReady;            /* */
    CsrUint16                     payloadSize;              /* */
    CsrUint16                     payloadIndex;             /* */

    /* Instance data used by defragmentation algorithm */
    CsrUint8                      *recvBuffer;              /* */
    CsrUint16                     recvBufferLength;         /* */
    CsrBool                       restoreSapcFlag;
    CsrBool                       headerComplete;           /* used to indicate if header was fragmented */
    CsrMessageQueueType         *saveQueue;
    /* Common SDP search stuff */
    void                         *sdpSapcSearchConData;    /* Instance data for the common SDP search lib */
    CsrBool                       searchOngoing;

} SapcInstanceData_t;

typedef void (* SapcStateHandlerType)(SapcInstanceData_t * sapcInstanceData);
void CsrBtSapcRfcSdcConResultHandler(void                   *inst,
                              CsrUint8                      localServerCh,
                              CsrUint32                     sapcConnId,
                              CsrBtDeviceAddr               deviceAddr,
                              CsrUint16                     maxFrameSize,
                              CsrBool                       validPortPar,
                              RFC_PORTNEG_VALUES_T          portPar,
                              CsrBtResultCode               resultCode,
                              CsrBtSupplier                 resultSupplier,
                              CmnCsrBtLinkedListStruct      *sdpTagList);
#ifdef __cplusplus
}
#endif

#endif
