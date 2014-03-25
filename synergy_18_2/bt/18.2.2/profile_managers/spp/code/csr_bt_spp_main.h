#ifndef CSR_BT_SPP_MAIN_H__
#define CSR_BT_SPP_MAIN_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_SPP_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "rfcomm_prim.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_sched.h"
#include "csr_message_queue.h"
#include "csr_bt_spp_prim.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"
#include "csr_am_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPP_CLASS_OF_DEVICE             (0x000000)                        /* The COD for the SPP - definition in BT assigned numbers */
#define ACTIVE_POWER_MODE               (CONNECTION_MODE)                 /* SPP always want to be in active mode once connected */
#define SPP_PAGE_SCAN_INTERVAL          (0x800)                           /* interval 0x12 - 0x1000, default 0x800 */
#define SPP_PAGE_SCAN_WINDOW            (0x100)                           /* window   0x12 - 0x1000, default 0x12 */
#define SPP_PORT_SPEED                  (PORT_SPEED_UNUSED)               /* the port speed used during port neg */
#define MAX_NUMBER_OF_CONNECTIONS       (1)                               /* SPP can only handle one connection at any time (phase I) */
#define SPP_NO_CONNID                   (0xFFFFFFFF)
#define SPP_NUM_OF_RETRIES              (10)                              /* no of retries for sds register and m/s switch */
#define SPP_SDC_NUM_OF_SEARCH_RECS      (5)
#define SPP_SDC_NUM_OF_BYTES_TO_RETURN  (50)                              /* Maximum number of bytes the sdp attribute search will return */

/* Number of SPP instances that is allocated at once. */
#define SPP_INSTANCES_POOL_SIZE         (10)

/********************** Internal defines and types ***************************/
/* Spp states */
#define Init_s                0x00
#define Idle_s                0x01
#define Activated_s           0x02
#define Connected_s           0x03
#define Deactivate_s          0x04
#define SppNumberOfStates     0x05

typedef CsrUint8               SppStates_t;

#define SPP_SUB_IDLE_STATE              (0)
#define SPP_SUB_CROSSING_CONNECT_STATE  (1)
/*#define SPP_SUB_RFC_SEARCH_STATE        (2)*/
/*#define SPP_SUB_UUID_128_SEARCH_STATE   (3)*/
#define SPP_SUB_CONNECTING_STATE        (2) /*(4)*/

/* Spp Audio states */
#define SPP_AUDIO_SUB_STATE_BASE (0x00)
typedef enum SppAudioStates_t
{
    audioOff_s = SPP_AUDIO_SUB_STATE_BASE,
    audioReq_s,
    audioAcceptReq_s,
    audioAcceptInProgress_s,
    audioOn_s,
    audioRelease_s
} SppAudioStates_t;

/***  AM related definitions ***/ 
#define CSR_BT_FIRST_USE_AM_BUILD_ID         0xFFFF /* 6817  */

typedef struct SppInstancesPool_t
{
    CsrUint8                 numberInPool;
    CsrSchedQid               phandles[SPP_INSTANCES_POOL_SIZE];
    struct SppInstancesPool_t *next;
} SppInstancesPool_t;

typedef struct
{
    CsrUint16     selectServiceIdx;
    CsrBtUuid32     serviceHandle;
} sppServiceHandle_t;

/* Settings to determine whether to use the audio manager or not */
typedef enum
{
    UndefinedUseAm  = 0,
    UseAm,
    DoNotUseAm
}SppUseAm_t;


struct SppInstanceData_tTag;
typedef void (SppCallBackFunction)(struct SppInstanceData_tTag* inst);

typedef struct SppInstanceData_tTag
{
    CsrSchedQid              ctrlHandle;              /* control application handle */
    CsrSchedQid              dataHandle;              /* data application handle */
    CsrSchedQid              myAppHandle;             /* own scheduler app handle */
    SppStates_t              state;                   /* my state */
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
    SppAudioStates_t         audioState;
    CsrBool                  pendingSco;
    CsrBool                  pendingScoDisconnect;
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
    CsrUint8                 subState;                /* SPP sub states, used for cancel connect procedure */
    BD_ADDR_T                bdAddr;
    BD_ADDR_T                incomingBdAddr;
    CsrUint8                 role;
    CsrUint16                numberOfConnections;     /* */
    CsrUint32                sdsRecHandle;
    CsrUint32                sppConnId;               /* connectionId */
    CsrUint16                numOfRetries;            /* To register and unregister a SDP record */
    CsrUint8                 serverChannel;           /* Local server channel    for server role */
    CsrBtCplTimer            activateTime;
    CsrCharString            *serviceName;            /* temp storage of service name */
    CsrBool                  sdsUnregInProgress;      /* true when unregistering an sds service (in idle and connected states) */
    CsrBool                  sdsRecordObtain;         /* true if a SDS record is obtained */
    CsrBool                  requestPortPar;
    CsrBool                  validPortPar;
    RFC_PORTNEG_VALUES_T     portPar;                 /* port negotiation parameters */
    CsrBool                  restoreSppFlag;
    CsrMessageQueueType      *saveQueue;
    void                     *recvMsgP;               /* pointer to the received message. NULL if it has been passed on */
    CsrUint16                serviceHandleListSize;
    sppServiceHandle_t       *serviceHandleList;
    CsrBtSppServiceName      *sdpServiceNameList;
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
    CsrBool                  extendedConnect;         /* one of two extended connect used */
    uuid16_t                 extendedProfileUuid;
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
    CsrUint8                 currentLinkMode;
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
    CsrUint8                 *serviceRecord;          /* pointer to the alternative service record */
    CsrUint16                serviceRecordSize;       /* length of the alternative service record */
    CsrUint16                serviceRecordSrvChIndex; /* index to the server channel in alternate service record */
    CsrBool                  serviceRecordExt;        /* true if the alternative service record is used */
    CsrBtUuid128             uuid128Profile;
    CsrUint32                cod;
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
    CsrUint8                 numberOfSppInstances;
    SppInstancesPool_t       *sppInstances;
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
    hci_pkt_type_t           audioQuality;
    CsrUint32                txBandwidth;
    CsrUint32                rxBandwidth;
    CsrUint16                maxLatency;
    CsrUint16                voiceSettings;
    CsrUint8                 reTxEffort;
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
    CsrBool                  connectReqActivated;     /* TRUE of a connect req have been initiated but not confirmed.
                                                      * Used to determine if incoming connections areaccepted or disconnected.*/
    CsrBool                  cancelReceived;
    dm_security_level_t      secIncoming;             /* Incoming security level */
    dm_security_level_t      secOutgoing;             /* Outgoing security level */
        /* Common SDP search stuff */
    void                     *sdpSppSearchConData;    /* Instance data for the common SDP search lib */
    void                     *cmSdcRfcInstData;       /* Value received in "select" callbak function, that shall be used in CSR_BT_SPP_SERVICE_NAME_RES handler */
    CsrBool                  searchOngoing;           /* Search already ongoing or not */
    CsrBool                  sppRegisterDataPathPending;
    CsrBool                  sppPortNegPending;
    CsrUint8                 modemStatus;
    CsrUint8                 breakSignal;
#ifdef CSR_BT_INSTALL_SPP_EXTENDED
    SppUseAm_t               audioUseAm;               /* Audio connections through the Audio manager module? */
    SppCallBackFunction      *amSppCallBack;           /* Call back function for AM messages */
    CsrUint16                amConnId;                 /* Audio manager connection ID */
#endif /* CSR_BT_INSTALL_SPP_EXTENDED */
} SppInstanceData_t;

typedef void (* SppStateHandlerType)(SppInstanceData_t *sppInstanceData);

void CsrBtSppRfcSdcConResultHandler(void *inst,
    CsrUint8               localServerCh,
    CsrUint32                    sppConnId,
    CsrBtDeviceAddr                deviceAddr,
    CsrUint16                    maxFrameSize,
    CsrBool                      validPortPar,
    RFC_PORTNEG_VALUES_T        portPar,
    CsrBtResultCode             resultCode,
    CsrBtSupplier          resultSupplier,
    CmnCsrBtLinkedListStruct     *sdpTagList);

void CsrBtSppSdcSelectServiceHandler(void                    * inst,
                                void                    * cmSdcRfcInstData,
                                CsrBtDeviceAddr            deviceAddr,
                                CsrUint8           serverChannel,
                                CsrUint16                entriesInSdpTaglist,
                                CmnCsrBtLinkedListStruct * sdpTagList);

void CsrSppAmInitCfm(SppInstanceData_t *inst);
void CsrSppAmConnectCfm(SppInstanceData_t *inst);
void CsrSppAmReleaseCfm(SppInstanceData_t *inst);
void CsrSppConnectStream(SppInstanceData_t *inst, CsrAmAudioType audioType, CsrUint8 pcmSlot, CsrUint16 escoHandle);

void CsrBtSppMessagePut(CsrSchedQid phandle, void *msg);

#ifdef __cplusplus
}
#endif

#endif  /*_SPP_HANDLER_H */
#endif  /* EXCLUDE_CSR_BT_SPP_MODULE */
