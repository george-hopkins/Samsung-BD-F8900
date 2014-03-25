#ifndef CSR_BT_HF_MAIN_H__
#define CSR_BT_HF_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #6 $
****************************************************************************/

#include "csr_message_queue.h"
#include "csr_bt_hf_prim.h"
#include "csr_bt_hf_consts.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HF / HS Main states */
#define Null_s                                  0x00
#define Activated_s                             0x01
#define Deactivate_s                            0x02
#define HfMainNumberOfStates                    0x03
#define HfMainNumberOfUpstreamStates            0x03

/* HF / HS sub states */
#define Activate_s                              0x00
#define Connect_s                               0x01
#define Connected_s                             0x02
#define LpEnabled_s                             0x03
#define ServiceSearch_s                         0x04
#define HfNumberOfStates                        0x05
#define HsNumberOfStates                        (HfNumberOfStates - 1)

typedef struct
{
    CsrBtHfPrim             type;
    CsrBtHfConnectionId     connectionId;
    CsrBtCmeeResultCode     cmeeResultCode;
} HF_GENERAL_CFM_T;


typedef enum
{
    supportFeatures,
    codecSupport,
    cindSupport,
    cindStatus,
    eventReport,
    callHold,
    serviceLevel,
    copsQuery,
    rest
} HfAtStates_t;

typedef enum
{
    sdcState_s,
    btConnect_s,
    serviceConnect_s
} HfServiceState_t;

typedef enum
{
  idleCmd = 0, /* ready to send a new command; no confirm message pending */
  vgm,
  vgs,
  bsrf,
  cindSupportCmd,
  cindStatusCmd,
  cmer,
  chldSupport,
  chld,
  ccwa,
  clip,
  btrh,
  clcc,
  cnum,
  copsQueryCmd,
  copsSet,
  cmee,
  bvra,
  binp,
  answer,
  callEnd,
  ckpd,
  dialNumber,
  dialMem,
  redial,
  nrec,
  vts,
  c2cpwr,
  c2cbatt,
  c2cSf,
  c2cSms,
  c2c,
  c2cFn,
  bia,
  bcs,
  bac,
  bcc,
  other  /* direct AT command from the application (CSR_BT_HF_AT_CMD_REQ); send the answer directly to the app (CSR_BT_HF_AT_CMD_IND) */
} HfLastAtCmdSent_t;

typedef CsrUint8 HfStates_t;

typedef struct
{
    CsrBool                              allowed2SendCmData;      /* send only one packet at a time and wait for cfm */
    CsrBool                              dataReceivedInConnected;
    CsrMessageQueueType                  *cmDataReqQueue;         /* queue for data messages to cm */
    CsrUint16                            recvAtCmdSize;           /* number of char's currently received when */
    CsrUint8                             *recvAtCmd;              /* collecting a complete AT-command line */
    CsrUint16                            maxRfcFrameSize;
    CsrSchedMsgId                        atResponseTimerId;
} HfHsData_t;

typedef struct
{
    CsrUint8                     remC2C[CSR_BT_C2C_NUMBER_OF_INDICATORS];     /* remote peer CSR-2-CSR indicators */
    CsrUint8                     locC2C[CSR_BT_C2C_NUMBER_OF_INDICATORS];     /* local peer CSR-2-CSR indicators */
    CsrBool                      remoteSupport;                        /* if TRUE: remote device supports C2C (even if disabled) */
} HfIndicators_t;

typedef struct
{
    hci_pkt_type_t                       theAudioQuality;
    CsrUint32                            theTxBandwidth;
    CsrUint32                            theRxBandwidth;
    CsrUint16                            theMaxLatency;
    CsrUint16                            theVoiceSettings;
    CsrUint8                             theReTxEffort;
    CsrUint8                             scoParmType;            /* Internal "name" for this set of parameters, e.g SCO_DEFAULT_1P1 */
    CsrUint8                             pcmSlot;
    CsrBool                              pcmReassign;
} audioSetupParams_t;

/* Service record info placeholder */
typedef struct
{
    CsrUint8                     chan;                               /* Server channel */
    CsrBtHfConnectionType        type;                               /* If accepting, this is the connection type */
    CsrBool                      registered;                         /* SDS has been registered */
    CsrBool                      accepting;                          /* Accepting incoming connections */
    CsrBtUuid32                  recHandle;                          /* Service record handle */
} HfService_t;

struct HfMainInstanceData_tTag;
typedef void (HfCallBackFunction)(struct HfMainInstanceData_tTag* inst);

#ifdef CSR_USE_DSPM

typedef struct
{
    CsrUint16                    scoSourceHandle;                   /* SCO source handle returned from DSPM*/
    CsrUint16                    scoSinkHandle;                     /* SCO sink handle returned from DSPM*/
    CsrUint16                    sourceHandle;                      /* audio source handle returned from DSPM*/
    CsrUint16                    sinkHandle;                        /* audio sink handle returned from DSPM*/
    CsrUint16                    sourceType;                        /* source to use for SCO audio - eg PCM, I2S, CODEC, etc*/
    CsrUint16                    sourceInstance;                    /* which instance of the sourceType hardware to use */
    CsrUint16                    sourceChannel;                     /* which channel of the sourceType to use */
    CsrUint16                    sinkType;                          /* sink to use for SCO audio - eg PCM, I2S, CODEC, etc*/
    CsrUint16                    sinkInstance;                      /* which instance of the sinkType hardware to use */
    CsrUint16                    sinkChannel;                       /* which channel of the sinkType to use */
    CsrUint16                    numScoSourceConfigParams;          /* number of parameters to configure audio source with*/
    CsrBtHfStreamConfigParam*    scoSourceConfigParams;             /* parameters to configure audio source with*/
    CsrUint16                    numScoSinkConfigParams;            /* number of parameters to configure audio source with*/
    CsrBtHfStreamConfigParam*    scoSinkConfigParams;               /* parameters to configure audio source with*/
    CsrUint16                    numSourceWbsConfigParams;          /* number of parameters to configure audio source with*/
    CsrBtHfStreamConfigParam*    sourceWbsConfigParams;             /* parameters to configure audio source with if wide-band is used*/
    CsrUint16                    numSourceNbConfigParams;           /* number of parameters to configure audio source with*/
    CsrBtHfStreamConfigParam*    sourceNbConfigParams;              /* parameters to configure audio source with if narrow-band is used*/
    CsrUint16                    numSinkWbsConfigParams;            /* number of parameters to configure audio source with*/
    CsrBtHfStreamConfigParam*    sinkWbsConfigParams;               /* parameters to configure audio sink with if wide-band is used*/
    CsrUint16                    numSinkNbConfigParams;             /* number of parameters to configure audio source with*/
    CsrBtHfStreamConfigParam*    sinkNbConfigParams;                /* parameters to configure audio sink with if narrow-band is used*/
    CsrUint16                    operatorEncoder;                   /* Operator to use as encoder */
    CsrUint16                    operatorDecoder;                   /* Operator to use as decoder */
    CsrUint16                    nextOperatorEncoder;               /* Operator to use as encoder when the next eSCO is established */
    CsrUint16                    nextOperatorDecoder;               /* Operator to use as decoder when the next eSCO is established */
    CsrUint16                    connIdAudioDsp;                    /* Connection ID for Audio Type source to DSP encoder */
    CsrUint16                    connIdDspSco;                      /* Connection ID for DSP encoder to SCO sink */
    CsrUint16                    connIdScoDsp;                      /* Connection ID for SCO source to DSP decoder */
    CsrUint16                    connIdDspAudio;                    /* Connection ID for DSP decoder to Audio Type sink */
    CsrBool                      codecNegOngoing;                   /* Bool to indicate whether the codec negotiation is needed or not */
    CsrBool                      codecAlreadyNegotiated;            /* Bool to indicate whether the codec has been negotiated during the current SLC */
    CsrBool                      cleanUp;                           /* Bool to indicate whether the operators shall be de-allocated */
    CsrBool                      opSwap;                            /* Bool to indicate whether the newly created "next" operators shall replace the old ones */
} HfDspm_t;

#endif

typedef struct
{
    void                                 *mainInstance;           /* pointer to the main instance */
    HfStates_t                           state;                  /* my state */
    CsrUint32                            hfConnId;              
    CsrUint8                             serverChannel;          /* allocated (local) server channel */
    CsrUint8                             linkState;              /* state of the ACL link (connected, parked, sniff, disconnected) */
    CsrBtHfConnectionType                linkType;               /* Type of peer connection */
    CsrUint16                            scoHandle;              /* Sco handle */
    CsrUint32                            serviceRecHandle;       /* used to save the service record */
    CsrBool                              sdsRegistered;          /* flag to indicate if service record has been unregistered */
    CsrBool                              deactivated;            /* flag to indicate if HF has been deactivated (ConnectAccept cancelled */
                                                                /* and service record unregistered */
    CsrUint8                             audioCodecType;
    CsrUint8                             sampleRate;

    CsrBool                              disconnectReqReceived;  /* hf_cancel_req received from the app. */
    CsrBool                              disconnectPeerReceived; /* disconnect received from the peer */
    CsrBtDeviceAddr                      currentDeviceAddress;
    HfAtStates_t                         atSequenceState;
    HfServiceState_t                     serviceState;
    CsrUint8                             *cindSupportString;
    CsrUint8                             *cindStartValueString;
    CsrUint8                             nrOfIndicators;
    CsrUint8                             *chldStringStored;
    CsrBtDeviceName                      serviceName;
    CsrUint32                            supportedFeatures;
    CsrUint8                             network;
    CsrUint16                            currentAttributeSearch;
    CsrUint32                            currentSupportedFeatures;
    CsrUint8                             currentNetwork;
    CsrUint8                             linkRecoveryCounter;
    HfStates_t                           oldState;
    CsrUint32                            localSupportedFeatures;
    CsrUint16                            serviceHandleListCount;
    CsrBtUuid32                          serviceHandle;
    HfHsData_t                           *data;
    HfLastAtCmdSent_t                    lastAtCmdSent;

    /* following flags are used to handle audio requests */
    CsrBool                              audioPending;
    CsrBool                              lastAudioReq;
    hci_pkt_type_t                       audioQuality;           /* Sco packet type HV1 HV2 HV3 */
    CsrBtResultCode                      disconnectResult;
    CsrBtSupplier                        disconnectSupplier;
    CsrBool                              disconnectLocalTerminated;
    CsrUint16                            remoteVersion;
    CsrUint8                             pcmSlot;
    CsrBool                              pcmReassign;
    CsrBool                              pcmMappingReceived;
    CsrBool                              audioAcceptPending;
    /* Common SDP search stuff */
    CsrBool                              searchAndCon;
    CsrBool                              searchOngoing;

    CsrBool                              pendingCancel;
    CsrBool                              scoConnectAcceptPending;
    audioSetupParams_t                   audioSetupParams;
    CsrBool                              updateAudioSettingsFromMain;  /* Audio config received while SCO exists; update after SCO release */
    /* CSR-2-CSR related stuff */
    HfIndicators_t                       linkInd;
    /* Low power */
    CsrUint8                             lpMode;                  /* Current LP mode of link */
    CsrUint8                             lpWantedMode;            /* Desired LP mode of link */
    CsrSchedTid                          lpTimer;                 /* Low-power retry timer */
    CsrUint8                             lpCount;                 /* LP retry counter */
    CsrBool                              pendingModeChange;       /* We have requested a LP mode change */
    /* Connection status*/
    CsrBool                              incomingSlc;             /* Connection started from remote device */
    CsrBool                              outgoingAudio;           /* Audio connection requested from local device */
    CsrBool                              localDisconnect;         /* Disconnection started from local device */
    CsrBool                              pendingSlcXtraCmd;       /* Need to enable CLIP, CCWA or CMEE automatically at SLC establishment? */
    CsrSchedMsgId                        deregisterTimerId;       /* Timer to decide when to unregister the service record */
    CsrBool                              accepting;               /* CSR_BT_CM_CONNECT_ACCEPT_REQ sent; ready to accept connection */
    /*Audio */
    CsrUint8                             codecToUse;              /* Codec requested from the AG, which should be used for the next eSCO connection */
#ifdef CSR_USE_DSPM
    HfDspm_t                             dspmInst;                /* instance of DSPM data for this HF instance */
#endif
} HfInstanceData_t;

typedef struct HfMainInstanceData_tTag
{
    CsrSchedQid                         appHandle;               /* Application handle */
    HfStates_t                          state;                   /* Main state */
    void                                *recvMsgP;               /* Pointer to the received message. NULL if it has been passed on */
    CsrUint16                           eventClass;                             /* */
    CsrBool                             restoreFlag;             /* Common saveQueue */
    CsrMessageQueueType                 *saveQueue;
    CsrUint8                            numberOfUnregister;      /* Used to control the number of retries on CmSdsUnregister */
    HfInstanceData_t                    *linkData;               /* linkData */
    CsrUintFast8                            index;                   /* index to current connection */
    CsrUint8                            searchIdxHf;
    CsrUint8                            searchIdxHs;
    CsrBtDeviceAddr                     currentDeviceAddress;
    CsrBool                             connectSdcActive;        /* Set when hf_main_sef runs SDC search in connect */
    CsrUint8                            linkRecoveryCounter;
    CsrBtHfConnectionType               linkRegistering;         /* Used to indicate which link is in the process of registering a service record */
    CsrBool                             registerPending;         /* used to indicate that a link has tried to register but couldn't */
    CsrBool                             deactivated;             /* Used when profile is deactivated in the ServiceSearch_s state. Then the message */
                                                                 /* is saved and this flag is used to indicate that no messages should be send to app */
    CsrBool                             doingCleanup;
    dm_security_level_t                 secIncoming;             /* Incoming security level */
    dm_security_level_t                 secOutgoing;             /* Outgoing security level */
    CsrUint32                           mainConfig;
    CsrUint8                            allocInactiveHfCons;     /* number of inactive HF service records allocated (registered at CM) */
    CsrUint8                            allocInactiveHsCons;     /* number of inactive HS service records allocated (registered at CM) */
    CsrUint8                            maxHFConnections;        /* maximum number of simultaneous HF connections allowed; if 0, the HFP is disabled! */
    CsrUint8                            maxHSConnections;        /* maximum number of simultaneous HS connections allowed; if 0, the HSP is disabled! */
    CsrUint8                            maxTotalSimultaneousConnections; /* maximum number of simultaneous connections allowed; overrides the two previous if needed */
    CsrUint32                           localSupportedFeatures;
    /* CSR-2-CSR related stuff */
    CsrUint8                            locC2C[CSR_BT_C2C_NUMBER_OF_INDICATORS];     /* local peer CSR-2-CSR indicators */
    audioSetupParams_t                  generalAudioParams;
    /* Low Power (the field mainConfig contains LP info as well) */
    CsrBtHfConfigLowPowerMask           lpMask;                  /* Low power set-up flags */
    void                                *sdpHfSearchConData;     /* Instance data for the common SDP search lib */
    void                                *sdpHfSearchData;        /* Instance data for the common SDP search lib */
    CsrUint8                            atRespWaitTime;          /* time to wait for answer from HFG  after sending an AT cmd */
    CsrUint8                            deregisterTime;          /* time to keep a service record after connection */
    CsrUint8                            reActivating;            /* activation ongoing (even if already activated) */
    CsrUint8                            modemStatus;
#ifdef CSR_USE_DSPM
    CsrUint8                            hfDspmOpPending;         /* Hf Instance ID currently using DSP-Manager or Audio Manager */
    HfCallBackFunction                  *dspmHfCallBack;         /* Call back function for DSPM messages */
    HfDspm_t                            dspmInst;                /* instance of DSPM data for this HF instance */
#endif
    CsrUint32                           supportedCodecsMask;
} HfMainInstanceData_t;

typedef void (*HfStateHandlerType)(HfMainInstanceData_t * hfInstanceData);
void CsrBtHfpHandler(HfMainInstanceData_t * instData);
typedef CsrBool (*HfAtHandler_t)(HfMainInstanceData_t *instData, CsrUint8 *atTextString);

void CsrBtHfSdcResultHandler(void                     * inst,
                             CmnCsrBtLinkedListStruct * sdpTagList,
                             CsrBtDeviceAddr          deviceAddr,
                             CsrBtResultCode          resultCode,
                             CsrBtSupplier      resultSupplier);

void CsrBtHfRfcSdcConResultHandler(void                        *inst,
                                   CsrUint8               localServerCh,
                                   CsrUint32                    hfConnId,
                                   CsrBtDeviceAddr             deviceAddr,
                                   CsrUint16                    maxFrameSize,
                                   CsrBool                      validPortPar,
                                   RFC_PORTNEG_VALUES_T        portPar,
                                   CsrBtResultCode             resultCode,
                                   CsrBtSupplier         resultSupplier,
                                   CmnCsrBtLinkedListStruct    *sdpTagList);

void CsrBtHfSdcSelectServiceHandler(void                    * instData,
                               void                    * cmSdcRfcInstData,
                               CsrBtDeviceAddr            deviceAddr,
                               CsrUint8           serverChannel,
                               CsrUint16                entriesInSdpTaglist,
                               CmnCsrBtLinkedListStruct * sdpTagList);

#ifdef __cplusplus
}
#endif

#endif          /*_HF_MAIN_H*/
