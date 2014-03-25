#ifndef CSR_BT_BSL_MAIN_H__
#define CSR_BT_BSL_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
            
              All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "rfcomm_prim.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_sched.h"
#include "csr_message_queue.h"
#include "csr_bt_bsl_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BSL_CLASS_OF_DEVICE                     (CSR_BT_NETWORKING_MAJOR_SERVICE_MASK) /* The COD bits to be set for the profile. */
#define BSL_TIMER_UNUSED                        (0xFFFF)
#define MAX_NUMBER_OF_SDC_RETRIES               10
#define MAX_NUMBER_OF_SDS_RETRIES               10
#define MAX_NUMBER_OF_DM_RETRIES                3
#define MAX_NUMBER_OF_CONNECT_RETRIES           5
#define CONNECT_RETRY_DELAY                     ((CsrTime)(500000))  /*(uSec) */
#define BSL_ROLE_SWITCH_RETRY_DELAY             ((CsrTime)(250000))  /*(uSec) */
#define SDS_REC_HANDLE_UNUSED                   0xFFFFFFFF
#define BSL_UUID_LSB_PANU                       0x15
#define BSL_UUID_LSB_NAP                        0x16
#define BSL_UUID_LSB_GN                         0x17
#define SDC_MAX_RETURN_BYTES                    255
#define BSL_INVALID_CONNECTION                  0xFF

/* BSL states */
typedef CsrUint8                                 BslState_t;
#define IdleS                                   ((BslState_t) (0x00) )
#define RegisterS                               ((BslState_t) (0x01) )
#define SdsRegisterS                            ((BslState_t) (0x02) )
#define BusyS                                   ((BslState_t) (0x03) )
#define BslNumberOfStates                       ((BslState_t) (0x04) )

typedef CsrUint8                                 BslConnectState_t;
#define IdleConnectionS                         ((BslConnectState_t) 0x00)
#define ConnectCancelAcceptS                    ((BslConnectState_t) 0x01)
#define ConnectingS                             ((BslConnectState_t) 0x02)
#define ConnectingRetryS                        ((BslConnectState_t) 0x03)
#define ConnectedS                              ((BslConnectState_t) 0x04)
#define DisconnectingS                          ((BslConnectState_t) 0x05)

/* BSL sdc search uuid */
typedef CsrUint8                                                      BslSdcAttrSearchIndex_t;
#define PROTOCOL_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER_INDEX          ((BslSdcAttrSearchIndex_t) 0x00)
#define LANGUAGE_BASE_ATTRIBUTE_ID_LIST_ATTRIBUTE_IDENTIFIER_INDEX   ((BslSdcAttrSearchIndex_t) 0x01)
#define SERVICE_AVAILABILITY_ATTRIBUTE_IDENTIFIER_INDEX              ((BslSdcAttrSearchIndex_t) 0x02)
#define BLUETOOTH_PROFILE_DESCRIPTOR_LIST_ATTRIBUTE_IDENTIFIER_INDEX ((BslSdcAttrSearchIndex_t) 0x03)
#define SERVICE_NAME_ATTRIBUTE_IDENTIFIER_INDEX                      ((BslSdcAttrSearchIndex_t) 0x04)
#define SERVICE_DESCRIPTION_ATTRIBUTE_IDENTIFIER_INDEX               ((BslSdcAttrSearchIndex_t) 0x05)
#define SECURITY_DESCRIPTION_ATTRIBUTE_IDENTIFIER_INDEX              ((BslSdcAttrSearchIndex_t) 0x06)
#define NET_ACCESS_TYPE_ATTRIBUTE_IDENTIFIER_INDEX                   ((BslSdcAttrSearchIndex_t) 0x07)
#define MAX_NET_ACCESS_RATE_ATTRIBUTE_IDENTIFIER_INDEX               ((BslSdcAttrSearchIndex_t) 0x08)
#define IPV4_SUBNET_ATTRIBUTE_IDENTIFIER_INDEX                       ((BslSdcAttrSearchIndex_t) 0x09)
#define IPV6_SUBNET_ATTRIBUTE_IDENTIFIER_INDEX                       ((BslSdcAttrSearchIndex_t) 0x0A)
#define LAST_SEARCH_ATTRIBUTE_INDEX                                  ((BslSdcAttrSearchIndex_t) 0x0B)
#define OPENING_SEARCH_CHANNEL_INDEX                                 ((BslSdcAttrSearchIndex_t) 0x0C)
#define SEARCH_CHANNEL_OPEN_INDEX                                    ((BslSdcAttrSearchIndex_t) 0x0D)

typedef struct
{
    BD_ADDR_T           bdAddr;
    CsrUint16           id;
    CsrBool             connectCancelled;
    BslConnectState_t   connectState;
    CsrBtBslPanRole     outgoingRole;
} BslConnection_t;

typedef struct
{
    /* instance variables */
    CsrSchedQid                    appHandle;              /* protocol handle for all control signalling */
    CsrSchedQid                    dataHandle;             /* protocol handle for all control signalling */
    CsrSchedQid                    sdpHandle;              /* protocol handle used for sdc search */
    BslState_t                   state;                  /* my state */
    CsrBool                       restoreFlag;
    CsrMessageQueueType         *saveQueue;
    void                         *recvMsgP;              /* pointer to the received message. NULL if it has been passed on */
    CsrUint16                     numberOfRetries;        /* retry counter for sdp sessions */
    CsrBool                       activated;              /* an activate req has been received */
    CsrBtBslPanRole                 localRole;
    CsrBtBslPanRole                 remoteRole;
    CsrBtBslPanRole                 sdcSearchRole;
    CsrUint16                     currentRoleRegister;
    CsrUint16                     wantedRole;
    CsrUint32                     panuSdsRecHandle;
    CsrUint32                     napSdsRecHandle;
    CsrUint32                     gnSdsRecHandle;
    CsrBool                       singleUser;
    BD_ADDR_T                    currentConn2Addr;
    BNEP_CONNECT_REQ_FLAGS       connFlagsCopy;
    BslSdcAttrSearchIndex_t      sdcAttrSearchIndex;
    CsrUint32                     sdcSearchHandle;        /* handle to sdc channel in cm for search on peer */
    CsrBtBslSearchRecord            *sdcSearchRec;
    dm_security_level_t          secIncoming;            /* Incoming security level */
    dm_security_level_t          secOutgoing;            /* Outgoing security level */
    CsrSchedTid                  bnepConnectReqTimerId;
    CsrSchedTid                  bnepSwitchRoleReqTimerId; /* Used to delay role switch attempts. */
    BslConnection_t              connectionList[CSR_BT_BSL_MAX_NUM_OF_CONNECTIONS];
} BslInstanceData_t;

typedef void (* BslStateHandlerType)(BslInstanceData_t * bslInstanceData);

/* Prototypes from bsl_free_down.c */
void CsrBtBslFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif
