#ifndef CSR_BT_HFG_MAIN_H__
#define CSR_BT_HFG_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_bt_cm_private_lib.h"
#include "csr_message_queue.h"
#include "csr_bt_hfg_prim.h"
#include "csr_bt_hfg_consts.h"

#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_HFG_NUMBER_AT_CMD      6
#define CSR_BT_HFG_NUMBER_SCO_TRIALS  5  /* Number of failed incoming SCO connections before negotiation start against "old" HF */

#ifdef CSR_USE_DSPM
#define HFG_DSPM_DEFAULT_HANDLE 0xFFFF
#endif
    /* Main profile instance state */
typedef enum
{
    MainNull_s = 0,                                                 /* Register server channels */
    MainIdle_s,                                                     /* Not activated */
    MainActive_s,                                                   /* Activated, use 2nd-level jumptable */
    MainDeactivate_s,                                               /* Closing down */
    MainNum_s
} HfgMainState_t;

    /* Connection instance states */
typedef enum
{
    Activate_s = 0,                                                 /* Accepting new connection (record+accept) */
    Connect_s,                                                      /* Outgoing connection setup in progress (record) */
    ServiceSearch_s,                                                /* SDC in progress (record) */
    Connected_s,                                                    /* SLC established (-) */
    ConnectionNum_s,                                                /* Dummy (-) */
    Unused_s                                                        /* Not in use, outside jump-tables (-) */
} HfgConnectionState_t;

/* Substates for AT sequence */
typedef enum
{
    At0Idle_s = 0,                                                  /* AT sequence not yet started */
    At1Brsf_s,                                                      /* AT+BRSF */
    At2CindQuestion_s,                                              /* AT+CIND=? */
    At3CindStatus_s,                                                /* AT+CIND? */
    At4Cmer_s,                                                      /* AT+CMER= */
    At5End_s                                                        /* AT+CHLD or otherwise finished */
} HfgAtState_t;


/* SCO audio settings parameters */
typedef struct
{
    CsrUint32                    txBandwidth;
    CsrUint32                    rxBandwidth;
    CsrUint16                    maxLatency;
    CsrUint16                    voiceSettings;
    hci_pkt_type_t               audioQuality;
    CsrUint8                     reTxEffort;
    CsrUint8                     scoParmType;                        /* Internal "name" for this set of parameters, e.g SCO_DEFAULT_1P1 */
} HfgAudioParams_t;

/* Service record info placeholder */
typedef struct
{
    CsrUint8                     chan;                               /* Server channel */
    CsrBtHfgConnection           type;                               /* If accepting, this is the connection type */
    CsrBool                      registered;                         /* SDS has been registered */
    CsrBool                      accepting;                          /* Accepting incoming connections */
    CsrBtUuid32                  recHandle;                        /* Service record handle */
} HfgService_t;

/* Service search placeholder */
typedef struct
{
    CsrBtUuid32                  recordId;                           /* Service Record Id */
    CsrUint16                    num;                                /* Number of elements */
} HfgSearch_t;

/* Indicator and setting placeholder */
typedef struct
{
    CsrUint8                     ciev[CSR_BT_CIEV_NUMBER_OF_INDICATORS];    /* CIEV/CIND status indicators */
    CsrUint8                     bia[CSR_BT_CIEV_NUMBER_OF_INDICATORS];     /* BIA activation of ciev/cind update sending */
#ifdef CSR_BT_INSTALL_HFG_C2C
    CsrUint8                     c2c[CSR_BT_C2C_NUMBER_OF_INDICATORS];      /* remote peer CSR-2-CSR indicators */
    CsrUint8                     loc2c[CSR_BT_C2C_NUMBER_OF_INDICATORS];    /* local peer CSR-2-CSR indicators */
#endif
    CsrUint8                     other[CSR_BT_HFG_NUM_OF_SETTINGS];         /* special non-indicator persistent settings */
} HfgIndicators_t;


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
    CsrBtHfgStreamConfigParam*   scoSourceConfigParams;             /* parameters to configure audio source with*/
    CsrUint16                    numScoSinkConfigParams;            /* number of parameters to configure audio source with*/
    CsrBtHfgStreamConfigParam*   scoSinkConfigParams;               /* parameters to configure audio source with*/
    CsrUint16                    numSourceWbsConfigParams;             /* number of parameters to configure audio source with*/
    CsrBtHfgStreamConfigParam*   sourceWbsConfigParams;                /* parameters to configure audio source with*/
    CsrUint16                    numSourceNbConfigParams;             /* number of parameters to configure audio source with*/
    CsrBtHfgStreamConfigParam*   sourceNbConfigParams;                /* parameters to configure audio source with*/
    CsrUint16                    numSinkWbsConfigParams;            /* number of parameters to configure audio source with*/
    CsrBtHfgStreamConfigParam*   sinkWbsConfigParams;               /* parameters to configure audio source with*/
    CsrUint16                    numSinkNbConfigParams;             /* number of parameters to configure audio source with*/
    CsrBtHfgStreamConfigParam*   sinkNbConfigParams;                /* parameters to configure audio source with*/
    CsrUint16                    operatorEncoder;                   /* Operator to use as encoder */
    CsrUint16                    operatorDecoder;                   /* Operator to use as decoder */
    CsrUint16                    connIdAudioDsp;                    /* Connection ID for Audio Type source to DSP encoder */
    CsrUint16                    connIdDspSco;                      /* Connection ID for DSP encoder to SCO sink */
    CsrUint16                    connIdScoDsp;                      /* Connection ID for SCO source to DSP decoder */
    CsrUint16                    connIdDspAudio;                    /* Connection ID for DSP decoder to Audio Type sink */
    CsrBool                      dspError;                          /* Bool to track if the DSP has failed to load an operator and should not be used */
} HfgDspm_t;

#endif

/* Connection instance */
typedef struct HfgInstance_tTag
{
    void                         *main;                              /* Main instance data pointer */
    void                         *msg;                               /* Current message */
    CsrUint16                    msgClass;                           /* Current message class */
    CsrUint8                     index;                              /* My local index in main->linkData */
    CsrUint32                    state;                              /* Connection state */
    CsrUint32                    oldState;                           /* Old connection state */
    CsrUint32                    atState;                            /* Sub-state for AT startup sequence (service search) */
    CsrBtHfgConnection           linkType;                           /* Type of peer connection */
    CsrBtHfgConnectionId         hfgConnId;                          /* Connection ID */
    CsrUint8                     serverChannel;                      /* Server channel */
    CsrUint16                    maxFrameSize;                       /* RFCOMM CSRMAX frame size */
    CsrBtDeviceAddr              address;                            /* Peer address */
    CsrUint32                    remSupFeatures;                     /* Remote peer supported features bitmask */
    CsrBtDeviceName              serviceName;                        /* Peer service name (not friendly name) */
    HfgIndicators_t              ind;                                /* Indicators etc. */
    CsrBool                      remoteSampleRateSupport;

    CsrUint16                    scoHandle;                          /* SCO connection handle */
    CsrBool                      scoWantedState;                     /* Target link state (connected/disconnected) */
    HfgAudioParams_t             scoParams;                          /* User-defined (e)SCO parameters */
    CsrUint8                     scoPcmSlot;                         /* PCM slot for outgoing SCO connection */
    CsrBool                      scoPcmRealloc;                      /* PCM slot reallocation for outgoing SCO connection */

    CsrBtHfgAudioCodecType       audioCodecType;                     /* Audio codec type to use 0x01:CVSD, 0x02:2-bit ADPCM, 0x04:4-bit ADPCM */
    CsrBtHfgAudioSampleRateType  sampleRateUsed;                     /* Sample rate used: 0x01: 8 KHz; 0x02: 16 KHz */
    CsrBool                      pendingCodecNegotiation;            /* Has the HFg started an Auristream negotiation? */
    CsrBtResultCode              searchConnectResult;                /* Connect confirm result for SDC resources */
    HfgSearch_t                  searchHfg;                          /* Service search for HFG */
    HfgSearch_t                  searchAg;                           /* Service search for HFG */
    CsrUint16                    remoteVersion;                      /* Only relevant for HFG */
    CsrBool                      restoreFlag;                        /* Save queue holds items */
    CsrMessageQueueType          *saveQueue;                         /* Local save queue */
    CsrBool                      cmTxReady;                          /* CM ready for next AT command */
    CsrMessageQueueType          *cmTxQueue;                         /* Outgoing AT command queue */
    CsrUint8                     *atCmd;                             /* Partial incoming AT command */
    CsrUint16                    atLen;                              /* Length of partial AT */
    CsrSchedTid                  atChldTimer;                        /* Timer for missing CHLD event */

    CsrBool                      pendingSearch;                      /* SDS search in progress */
    CsrBool                      pendingDisconnect;                  /* Disconnect req from app received */
    CsrBool                      pendingPeerDisconnect;              /* Disconnect req from peer received */
    CsrBool                      pendingCancel;                      /* Cancel accept connection received */
    CsrBool                      pendingSco;                         /* SCO connection is being set up */
    CsrBool                      pendingScoConfirm;                  /* SCO connection is being set up because it is requested by the app "pendingSco" is also used in case of C2C so it can not be used to determine if the app should have an indication or a confirm */
    CsrBool                      pendingScoDisconnect;               /* App has request SCO to be disconnected */
    CsrBool                      pendingModeChange;                  /* We have requested a LP mode change */
    CsrBool                      searchAndCon;                       /* SDS search and connection request in progress */

    CsrSchedTid                  ringTimer;                          /* Ring timer */
    CsrBtHfgRingReq              *ringMsg;                           /* Ring message data */

    CsrUint8                     lpMode;                             /* Current LP mode of link */
    CsrUint8                     lpWantedMode;                       /* Desired LP mode of link */
    CsrSchedTid                  lpTimer;                            /* Low-power retry timer */
    CsrUint8                     lpCount;                            /* LP retry counter */
#ifdef CSR_BT_INSTALL_HFG_C2C
    CsrUint16                    bccmdSeqNo;                         /* Sequence counter used for bccmds related to connection (seqno from main+1) */
#endif
    CsrUint8                     incScoCnt;                          /* Incoming SCO counter if Auristream supported */
    CsrSchedTid                  deRegisterTimer;                    /* Service record de-register timer */
    CsrSchedTid                  conGuardTimer;                      /* Conection guard timer */

    CsrUint8                     waitForDataCfm;                     /* counter to keep track of the data messages needed to ensure the data has been sent */
    CsrUint32                    remSupportedCodecs;                 /* Mask of codecs supported by the remote device */
    CsrUint8                     lastCodecUsed;                      /* Id of the last codec used during an audio connection to the remote device */ 
#ifdef CSR_USE_DSPM    
    HfgDspm_t                    dspmInst;                           /* instance of DSPM data for this HFG instance */
#endif
} HfgInstance_t;

typedef void (HfgCallBackFunction)(struct HfgInstance_tTag *inst);

/* Main (common) instance */
typedef struct HfgMainInstance_tTag
{
    HfgInstance_t               linkData[CSR_BT_HFG_NUM_SERVERS];    /* Connection instances */
    CsrSchedQid                 appHandle;                           /* App handle */
    CsrUint32                   state;                               /* Current global state */
    CsrUint32                   oldState;                            /* Old global state */
    void                        *msg;                                /* Current message */
    CsrUint16                   msgClass;                           /* Current message class */
    CsrUint32                   locSupFeatures;                     /* Specification local supported features bitmask */
    CsrUint32                   callConfig;                         /* CHLD/BTRH setup bitmask */
    CsrUint32                   hfgConfig;                          /* Profile setup bitmask */
    CsrBtDeviceName             localName;                          /* Local service name for HFG SDS */
    CsrBtHfgAtParserMode        atParser;                           /* AT parser mode (full, semi, none) */
    CsrUint8                    maxConnections;                     /* Max. number of concurrent connections */

    HfgIndicators_t             ind;                                /* Global default indicators etc. */
    CsrBtHfgPowerMask           lpMask;                             /* Low-power setup flags */

    CsrBool                     restoreFlag;                        /* Pop from global save queue */
    CsrMessageQueueType         *saveQueue;                         /* Global save queue */

    HfgService_t                 service[CSR_BT_HFG_NUM_CHANNELS];          /* Service records and server channels */
    CsrUint8                     activeService;                      /* Service index currently being manipulated */
    CsrUint8                     activeConnection;                   /* Connection currently being manipulated */

    dm_security_level_t          secIncoming;                        /* Incoming security level */
    dm_security_level_t          secOutgoing;                        /* Outgoing security level */
#ifdef CSR_BT_INSTALL_HFG_C2C
    CsrUint16                    bccmdSeqNo;                         /* Sequence counter used for bccmds */
    CsrBool                      adpcmLocalSupported;                /* does the chip support ADPCM/Optistream or not */
#endif
    CsrBtHfgAudioCodecType       preferredCodecType;                 /* Audio codec type to use 0x01:CVSD, 0x02:2-bit ADPCM, 0x04:4-bit ADPCM */
    CsrBtHfgAudioSampleRateType  preferredSampleRate;                /* Sample rate used: 0x01: 8 KHz; 0x02: 16 KHz */

    /* Common SDP search stuff */
    void                        *sdpHfgSearchConData;               /* Instance data for the common SDP search lib */
    void                        *sdpHfgSearchData;                  /* Instance data for the common SDP search lib */

    CsrUint8                     HfgSendAtToApp[CSR_BT_HFG_NUMBER_AT_CMD];  /* Defined to 6 even though 5 bytes are enough so far... */
#ifdef CSR_BT_INSTALL_HFG_C2C
    CsrBool                      pendingAdcpmQuery;                  /* Does the application wait for an ADPCM support indication? */
#endif
    CsrUint8                     deRegisterTime;                     /* Service record de-register time interval */
    CsrBool                      initSequenceDone;
    CsrUint8                     modemStatus;
#ifdef CSR_USE_DSPM
    HfgDspm_t                    dspmInst;                           /* DSPM data for the main HFG instance */
    CsrUint8                     hfgDspmOpPending;                   /* HFG Instance currently using DSP-Manager or Audio Manager */
    HfgCallBackFunction          *dspmHfgCallBack;                   /* Call back function for DSPM messages */
#endif
} HfgMainInstance_t;

/* Prototypes for top, connection and at handlers */
typedef void (*HfgMainHandler_t)(HfgMainInstance_t *inst);
typedef void (*HfgConnHandler_t)(HfgInstance_t *inst);
typedef void (*HfgAtHandler_t)(HfgInstance_t *inst, CsrUint16 *index, CsrBool seq);

void CsrBtHfgSdcResultHandler(void               * inst,
                        CmnCsrBtLinkedListStruct * sdpTagList,
                        CsrBtDeviceAddr          deviceAddr,
                        CsrBtResultCode          resultCode,
                        CsrBtSupplier      resultSupplier);

void CsrBtHfgRfcSdcConResultHandler(void                  *inst,
                              CsrUint8               localServerCh,
                              CsrUint32                    hfgConnId,
                              CsrBtDeviceAddr                deviceAddr,
                              CsrUint16                    maxFrameSize,
                              CsrBool                      validPortPar,
                              RFC_PORTNEG_VALUES_T        portPar,
                              CsrBtResultCode             resultCode,
                              CsrBtSupplier         resultSupplier,
                              CmnCsrBtLinkedListStruct    *sdpTagList);

void CsrBtHfgSdcSelectServiceHandler(void                    * instData,
                               void                    * cmSdcRfcInstData,
                               CsrBtDeviceAddr            deviceAddr,
                               CsrUint8           serverChannel,
                               CsrUint16                entriesInSdpTaglist,
                               CmnCsrBtLinkedListStruct * sdpTagList);


#ifdef CSR_LOG_ENABLE
void HfgChangeState(char *file, CsrUint32 lineno, CsrUint32 *state, char *stateName, CsrUint32 newState, char *newStateName);
#define HFG_CHANGE_STATE(state, newState) HfgChangeState(__FILE__, __LINE__, &(state), #state, (newState), #newState)
#else
#define HFG_CHANGE_STATE(state, newState) (state = newState)
#endif
#ifdef CSR_USE_DSPM
void CsrHfgDspmInit(HfgInstance_t *inst);
void CsrHfgDspmActivateRspHandler(HfgInstance_t *instData);
void CsrHfgDspmEssCfm(HfgInstance_t *inst);
void CsrBtHfgDspmSendCreateOperators(HfgInstance_t *instData);
void CsrBtHfgDspmCreateOperatorsRspHandler(HfgInstance_t *instData);
void CsrHfgDspmAudioConnect(HfgInstance_t *instData);
void CsrBtHfgDspmStreamOpenRspHandle(HfgInstance_t *instData);
void CsrBtHfgDspmConfigStreamRspHandler(HfgInstance_t *instData);
void CsrBtHfgDspmSendSingleConfigStream(HfgInstance_t *instData, CsrBtHfgAudioDspStreamSingleConfig *configData);
void CsrBtHfgDspmSingleConfigStreamRspHandler(HfgInstance_t *instData);
void CsrBtHfgDspmSendConnectStreams(HfgInstance_t *instData);
void CsrBtHfgDspmConnectionCreateRspHandler(HfgInstance_t *instData);
void CsrBtHfgDspmOperatorStartRspHandler(HfgInstance_t *instData);
void CsrBtHfgDspmSendDisconnectStreams(HfgInstance_t *instData);
void CsrBtHfgDspmOperatorStopRspHandler(HfgInstance_t *instData);
void CsrBtHfgDspmConnectionDestroyRspHandler(HfgInstance_t *instData);
void CsrBtHfgDspmOperatorDestroyRspHandler(HfgInstance_t *instData);
void CsrBtHfgDspmStreamCpCloseRspHandler(HfgInstance_t *instData);
#endif

#ifdef __cplusplus
}
#endif

#endif
