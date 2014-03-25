#ifndef CSR_BT_AV_MAIN_H__
#define CSR_BT_AV_MAIN_H__

#include "csr_synergy.h"
/****************************************************************************

      Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
      
                All rights reserved.

REVISION:      $Revision: #6 $
****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_message_queue.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_usr_config.h"
#include "csr_bt_av_fragmentation.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AV_QOS_MAX_INDICATION            (10)        /* This will show the status of the buffer 0 - AV_QOS_MAX_INDICATION */
#define AV_SDP_REG_RETRIES               (10)        /* Number of SDP register tries before failure */
#define DEFAULT_AVDTP_VERSION            ((CsrUint16) 0x102)
#define AVDTP_1_3_VERSION                ((CsrUint16) 0x103)

#define CSR_BT_AV_MEDIA_TYPE_INDEX            (2)
#define CSR_BT_AV_CODEC_TYPE_INDEX            (3)
/* SBC */
#define CSR_BT_AV_SFREQ_AND_CMODE_INDEX       (4)
#define CSR_BT_AV_BLOCKS_SUBBAND_METHOD_INDEX (5)
#define CSR_BT_AV_MIN_BITPOOL_INDEX           (6)
#define CSR_BT_AV_MAX_BITPOOL_INDEX           (7)
/* MPEG 2,4 AAC*/
#define CSR_BT_AV_BIT_RATE_HIGH_IDX          (7)
#define CSR_BT_AV_BIT_RATE_MED_IDX           (8)
#define CSR_BT_AV_BIT_RATE_LOW_IDX           (9)

/* Apt-X codec definitions */
#define CSR_BT_AV_APTX_VENDOR_ID1                   (0x0000004F)
#define CSR_BT_AV_APTX_VENDOR_ID2                   (0x0000000A)
#define CSR_BT_AV_APTX_CODEC_ID1                    (0x0001)
#define CSR_BT_AV_APTX_CODEC_ID2                    (0x0002)
#define CSR_BT_AV_APTX_VENDOR_ID_LSB_INDEX          (4)
#define CSR_BT_AV_APTX_CODEC_ID_LSB_INDEX           (8)
#define CSR_BT_AV_APTX_SFREQ_AND_CMODE_INDEX        (0x0A)

#define CSR_BT_AV_HIGH_NIBBLE_MASK                  (0xF0)
#define CSR_BT_AV_LOW_NIBBLE_MASK                   (0x0F)
#define CSR_BT_AV_SUBBANDS_MASK                     (0x0C)
#define CSR_BT_AV_ALLOC_METHOD_MASK                 (0x03)

/* Media type defines - Assigned Numbers */
#define CSR_BT_AV_AUDIO_MEDIA_TYPE       (0x00)
#define CSR_BT_AV_VIDEO_MEDIA_TYPE       (0x01)
#define CSR_BT_AV_MULTI_MEDIA_TYPE       (0x02)

/* Audio codec ID defines - Assigned Numbers */
#define CSR_BT_AV_AUDIO_SBC_CODEC               (0x00)
#define CSR_BT_AV_AUDIO_MPEG_1_2_CODEC          (0x01)
#define CSR_BT_AV_AUDIO_MPEG_2_4_AAC_CODEC      (0x02)
#define CSR_BT_AV_AUDIO_ATRAC_CODEC             (0x04)
#define CSR_BT_AV_AUDIO_NON_A2DP_CODEC          (0x0FF)

/* Video codec ID defines - Assigned Numbers */
#define CSR_BT_AV_VIDEO_H263_BASELINE_CODEC     (0x01)
#define CSR_BT_AV_VIDEO_MPEG4_VSP_CODEC         (0x02)
#define CSR_BT_AV_VIDEO_H263_P3_CODEC           (0x03)
#define CSR_BT_AV_VIDEO_H263_P8_CODEC           (0x04)

/* SBC-related definitions */
/* channel mode */
#define CSR_BT_AV_DUAL_CMODE                    (0x01)
#define CSR_BT_AV_STEREO_CMODE                  (0x02)
#define CSR_BT_AV_JOINT_STEREO_CMODE            (0x04)
#define CSR_BT_AV_MONO_CMODE                    (0x08)

/* SUBBANDS */
#define CSR_BT_AV_4_SUBBANDS                    (0x08)
#define CSR_BT_AV_8_SUBBANDS                    (0x04)

typedef enum
{
    NULL_S,
    READY_S,
    CLEANUP_S
} av_state_t;

typedef enum
{
    DISCONNECTED_S,
    CONNECTING_S,
    CANCEL_CONNECTING_S,
    CONNECTED_S,
    DISCONNECTING_S
} av_con_state_t;

typedef enum
{
    IDLE_S,
    CONFIGURING_S,
    CONFIGURED_S,
    OPENING_S,
    PEER_OPENING_S,
    PEER_OPENING_TO_STREAM_S,
    OPENED_S,
    STARTING_S,
    STREAMING_S,
    CLOSING_S,
    PEER_CLOSING_S,
    ABORTING_S,
    PEER_ABORTING_S,
    TERMINATE_PEER_OPENING_S,
    TERMINATE_OPENING_S
} av_stream_state_t;

/* data queue element structure */
typedef struct q_element_tag_t
{
    CsrUint16                    length;
    void                        *data;
    struct q_element_tag_t      *next;
} q_element_t;

typedef struct
{
    av_stream_state_t        streamState;                /* Stream state */
    CsrUint8                 remoteSeid;                 /* Stream end-point of remote device */
    CsrUint8                 localSeid;                  /* Stream end-point of local device */
    CsrUint8                 conId;                      /* Holds the index to the connection information */
    CsrBtConnId              mediaCid;                   /* L2CAP channel ID for media transport */
    l2ca_mtu_t               mediaMtu;                   /* The MTU size of the media channel */
    CsrUint8                 tLabel;                     /* Holds the transaction label for outstanding signal command */
    CsrUint16                seqNo;                      /* Holds the sequence number for stream data */
#if defined(AV_STREAM_INACTIVITY_LP_ENABLE) && (AV_STREAM_INACTIVITY_LP_ENABLE > 0)
    CsrUint16                seqNoAtLastLpCheck;         /* Sequence number when the lp mode was last checked */
    CsrSchedTid                   lpTimerId;                  /* Low power check timer id */
#endif
    CsrUint8                 lpModeRequested;            /* The requested low power mode for the stream channel */
    CsrUint8                 lpModeCurrent;                 /* The current low power mode for the stream channel */
    CsrBtCmPrim              *fifoQ[CSR_BT_AV_MEDIA_BUFFER_SIZE];/* Media data queue - array of pointers to CSR_BT_CM_L2CA_DATA_REQ's */
    CsrUint8                 fifoQIn;                    /* Holds the index to end of queue */
    CsrUint8                 fifoQOut;                   /* Holds the index to start of queue */
    CsrBool                  sending;                    /* Flag indicating if L2CAP is busy with transmit (outstanding data req) */
    CsrBool                  sentBufFullInd;             /* Flag indicating if an CSR_BT_AV_QOS_IND(full) has been sent to app. */
    CsrUint16                qos_replyCounter;           /* Count when to send QoS info */
    CsrSchedTid                   timerId;                    /* Holds id of running timer (outstanding signalling command)*/
    CsrBool                  modeChangeOverRuleByApp;    /* If TRUE the application controls low power else not      */
    CsrUint32                bitRate;                    /* bit rate of the A2DP stream; 0 if unknown */
    CsrUint16                delayValue;                 /* Used to compensate delay between audio and video if needed */
} av_stream_info_t;


typedef struct
{
    av_con_state_t           conState;                   /* Connection state */
    CsrBtDeviceAddr          remoteDevAddr;              /* Address of remote device */
    CsrBtConnId              signalCid;                  /* Signalling L2CAP channel ID */
    CsrUint16                signalMtu;                  /* Signal channel L2CAP MTU size */
    CsrBool                  incoming;                   /* Flag indicating the origin of the connection */
    CsrBool                  sending;                    /* Flag indicating if L2CAP is busy with transmit (outstanding data req) */
    q_element_t              *qFirst;                    /* Pointer to first entry in tx queue */
    q_element_t              *qLast;                     /* Pointer to last entry in tx queue */
    CsrSchedTid                   timerId;                    /* Holds id of running timer (outstanding signalling command)*/
    CsrMessageQueueType     *pendingSigQueue;           /* Holds the pending signalling messages to be sent when mode change completes */
    CsrSchedTid                   lpSigTimerId;               /* Holds id of running timer (lp signalling channel)*/
    CsrUint8                 lpSigMode;                  /* Current link state on the signalling channel */
    CsrBool                  lpModeChangePending;        /* A mode change request is already pending */
    CsrBool                  modeChangeOverRuleByApp;    /* If TRUE the application controls low power else not      */
    CsrUint8                 pendingSigProc;             /* Holds the pending signalling procedure (discover & get_capa procedures) */
    CsrUint16                remoteAVDTPVersion;         /* Needed to decide whther to issue "getCapabilities" or "getAllCapabilities" */
} av_connection_info_t;

typedef struct
{
    av_state_t              state;                      /* AV main (instance) state */
    CsrUint16               l2capCtx;                   /* L2CAP data context */
    CsrSchedQid                  appHandle;                  /* Control application handle */
    CsrSchedQid                  streamAppHandle;            /* Data application handle */
    av_stream_info_t        stream[CSR_BT_AV_MAX_NUM_STREAMS]; /* Stream information */
    av_connection_info_t    con[CSR_BT_AV_MAX_NUM_CONNECTIONS];/* Connection information */
    void                    *recvMsgP;                  /* Holds the received primitive currently being handled */
    CsrMessageQueueType    *saveQueue;                 /* Save queue, used to postpone primitive handling */
    fragmentHead_t          *fragmentHeadPtr;           /* Queue of received signal message fragments to be defragmented */
    CsrBool                 restoreFlag;                /* Flag indicating if primitives should be pop'ed off the save q */
    CsrBool                 doDeactivate;               /* Flag indicating if deactivation is ongoing */
    CsrBool                 isConnectable;              /* Flag indicating if there is an outstanding connect accept */
    CsrUint8                registerRetryCount;         /* Counts number of SDS request attempts to avoid re-attempting forever */
    CsrUint8                roleRegister[4];            /* Holds the number of activations for each role */
    CsrUint32               serviceRecordHandles[4];    /* Holds the handle to a registered service record for each role */
    CsrBtAvRole             role;                       /* The role currently being registered/searched */

    dm_security_level_t     secIncoming;                /* Incoming security level */
    dm_security_level_t     secOutgoing;                /* Outgoing security level */
    CsrUint16               qosInterval;
    CsrBool                 allowStartLpNeg;            /* Holds information on whether it is allowed to start negotiation to enter LP mode */
    void                    *sdpAvSearchData;           /* Instance data for the common SDP search lib */
    CsrUint16               localVersion;               /* Local AVDTP supported version */
} av_instData_t;


typedef void (* avStateHandlerType)(av_instData_t * instData);

#define AV_NUMBER_OF_STATES                 (CLEANUP_S + 1 - NULL_S)
#define NUMBER_OF_AV_DOWNSTREAM_MESSAGES    (CSR_BT_AV_PRIM_DOWNSTREAM_COUNT)

void CsrBtAvMessagePut(CsrSchedQid phandle, void *msg);
#define PUT_PRIM_TO_APP(prim) \
    CsrBtAvMessagePut((CsrSchedQid)((instData->streamAppHandle != CSR_SCHED_QID_INVALID) ? instData->streamAppHandle : instData->appHandle), prim);

#if defined(AV_STREAM_INACTIVITY_LP_ENABLE) && (AV_STREAM_INACTIVITY_LP_ENABLE > 0)
#define AV_START_LP_TIMER(sid) \
                CsrSchedTimerSet( (1000 * CSR_BT_AV_LP_TIMEOUT), \
                                (void (*) (CsrUint16, void *)) avLowPowerHandler, \
                                (CsrUint16) (sid), \
                                (void *) instData);
#endif

/* prototypes for csr_bt_av_con.c */
void CsrBtAvConnectReqHandler( av_instData_t *instData);
void CsrBtAvCancelConnectReqHandler( av_instData_t *instData);
void CsrBtAvCancelConnectReqNullStateHandler( av_instData_t *instData);
void CsrBtAvDisconnectReqHandler( av_instData_t *instData);
void CsrBtAvCmL2caConnectCfmHandler( av_instData_t *instData);
void CsrBtAvCmL2caConnectAcceptCfmHandler( av_instData_t *instData);
void CsrBtAvCmL2caCancelConnectAcceptCfmHandler( av_instData_t *instData);
void CsrBtAvCmL2caDisconnectIndHandler( av_instData_t *instData);
void CsrBtAvCmL2caConnectCfmHandlerCleanup( av_instData_t *instData);
void CsrBtAvCmL2caConnectAcceptCfmHandlerCleanup( av_instData_t *instData);
void CsrBtAvCmL2caCancelConnectAcceptCfmHandlerCleanup( av_instData_t *instData);
void CsrBtAvCmL2caDisconnectIndHandlerCleanup( av_instData_t *instData);
void CsrBtAvCmL2caModeChangeIndHandler(av_instData_t *instData);
void CsrBtAvSignallingLowPowerHandler(CsrUint16 conId, void *data);
void avLowPowerHandler(CsrUint16 sid, void *data);
void CsrBtAvSecurityInHandler(av_instData_t *instData);
void CsrBtAvSecurityOutHandler(av_instData_t *instData);
void CsrBtAvCmL2caChannelInfoCfmHandler(av_instData_t *instData);

/* prototypes for csr_bt_av_register.c */
void CsrBtAvActivateReqHandler( av_instData_t *instData);
void CsrBtAvDeactivateReqHandler( av_instData_t *instData);
void CsrBtAvCmSdsRegisterCfmHandler( av_instData_t *instData);
void CsrBtAvCmSdsUnregisterCfmHandler( av_instData_t *instData);
void CsrBtAvRegisterStreamHandleReqHandler( av_instData_t *instData);
void CsrBtAvCmL2caRegisterCfmHandler(av_instData_t *instData);
void CsrBtAvCmSdsRegisterCfmHandlerCleanup( av_instData_t *instData);
void CsrBtAvCmSdsUnregisterCfmHandlerCleanup( av_instData_t *instData);

/* prototypes for csr_bt_av_signal.c */
void CsrBtAvDiscoverReqHandler(av_instData_t *instData);
void CsrBtAvGetCapabilitiesReqHandler(av_instData_t *instData);
void CsrBtAvSetConfigurationReqHandler(av_instData_t *instData);
void CsrBtAvGetConfigurationReqHandler(av_instData_t *instData);
void CsrBtAvReconfigureReqHandler(av_instData_t *instData);
void CsrBtAvOpenReqHandler(av_instData_t *instData);
void CsrBtAvStartReqHandler(av_instData_t *instData);
void CsrBtAvCloseReqHandler(av_instData_t *instData);
void CsrBtAvIgnoreReqHandler(av_instData_t *instData);
void CsrBtAvSuspendReqHandler(av_instData_t *instData);
void CsrBtAvAbortReqHandler(av_instData_t *instData);
void CsrBtAvSecurityReqHandler(av_instData_t *instData);
void CsrBtAvSecurityReqFree(av_instData_t *instData);
void CsrBtAvStreamDataReqHandler(av_instData_t *instData);
void CsrBtAvStreamDataReqFree(av_instData_t *instData);
void CsrBtAvDiscoverResHandler(av_instData_t *instData);
void CsrBtAvGetCapabilitiesResHandler(av_instData_t *instData);
void CsrBtAvGetCapabilitiesResFree(av_instData_t *instData);
void CsrBtAvSetConfigurationResHandler(av_instData_t *instData);
void CsrBtAvGetConfigurationResHandler(av_instData_t *instData);
void CsrBtAvGetConfigurationResFree(av_instData_t *instData);
void CsrBtAvReconfigureResHandler(av_instData_t *instData);
void CsrBtAvOpenResHandler(av_instData_t *instData);
void CsrBtAvStartResHandler(av_instData_t *instData);
void CsrBtAvCloseResHandler(av_instData_t *instData);
void CsrBtAvSuspendResHandler(av_instData_t *instData);
void CsrBtAvAbortResHandler(av_instData_t *instData);
void CsrBtAvSecurityResHandler(av_instData_t *instData);
void CsrBtAvSecurityResFree(av_instData_t *instData);
void CsrBtAvCmL2caDataIndHandler(av_instData_t *instData);
void CsrBtAvCmL2caDataIndHandlerCleanup(av_instData_t *instData);
void CsrBtAvCmL2caDataCfmHandler(av_instData_t *instData);
void CsrBtAvGetChannelInfoReqHandler(av_instData_t *instData);
void CsrBtAvGetStreamChannelInfoReqHandler(av_instData_t *instData);

/* prototypes for csr_bt_av_util.c */
void CsrBtAvClearConnection(av_instData_t *instData, CsrUint8 connectionId);
void CsrBtAvClearStreamQ(av_stream_info_t *stream);
void CsrBtAvClearStream(av_instData_t *instData, CsrUint8 s);
void CsrBtAvIsCleanUpFinished(av_instData_t *instData);
void CsrBtAvSendHouseCleaning(av_instData_t *instData);
void CsrBtAvSaveMessage(av_instData_t *instData);
CsrUint8 getNumActivations(CsrUint8 *roleRegister);
CsrUint8 getNumIncomingCon(av_instData_t *instData);
void CsrBtAvMakeConnectable(av_instData_t *instData);
void CsrBtAvRegisterStreamHandleCfmSend(CsrSchedQid handle);

void CsrBtAvActivateCfmSend(CsrSchedQid handle,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);
void CsrBtAvDeactivateCfmSend(CsrSchedQid handle,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvStatusIndSend(av_instData_t *instData, CsrBtAvPrim signalId, CsrBtAvRole role, CsrUint8 conId);

void CsrBtAvConnectCfmSend(CsrSchedQid handle,
                           CsrUint8 connectionId,
                           CsrBtDeviceAddr devAddr,
                           CsrBtResultCode resultCode,
                           CsrBtSupplier resultSupplier,
                           CsrBtConnId btConnId);

void CsrBtAvConnectIndSend(CsrSchedQid handle,
                           CsrUint8 connectionId,
                           CsrBtDeviceAddr devAddr,
                           CsrBtConnId btConnId);

void CsrBtAvDisconnectIndSend(CsrSchedQid handle, CsrUint8 connectionId,
                              CsrBool localTerminated,
                              CsrBtReasonCode reasonCode,
                              CsrBtSupplier reasonSupplier);

void CsrBtAvStreamMtuSizeIndSend(av_instData_t *instData, CsrUint8 shandle, CsrBtConnId btConnId);

void CsrBtAvDiscoverCfmSendReject(av_instData_t *instData, CsrUint8 conId, CsrUint8 label,
    CsrBtResultCode avResultCode,
    CsrBtSupplier avResultSupplier);

void CsrBtAvGetCapabilitiesCfmSendReject(av_instData_t *instData, CsrUint8 conId, CsrUint8 label,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvSetConfigurationCfmSend(av_instData_t *instData, CsrUint8 conId, CsrUint8 label, CsrUint8 shandle, CsrUint8 servCategory,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvGetConfigurationCfmSend(av_instData_t *instData, CsrUint8 label, CsrUint8 shandle, CsrUint16 servCapLen, CsrUint8 *servCapData,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvReconfigureCfmSend(av_instData_t *instData, CsrUint8 label, CsrUint8 shandle, CsrUint8 servCategory,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvOpenCfmSend(av_instData_t *instData, CsrUint8 shandle, CsrUint8 label,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvStartCfmSend(av_instData_t *instData,
    CsrUint8 shandle,
    CsrUint8 label,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvCloseCfmSend(av_instData_t *instData,
    CsrUint8 shandle,
    CsrUint8 label,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvSuspendCfmSend(av_instData_t *instData, CsrUint8 shandle, CsrUint8 label,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

void CsrBtAvAbortCfmSend(av_instData_t *instData, CsrUint8 shandle, CsrUint8 label);

void CsrBtAvSecurityControlCfmSend(av_instData_t *instData, CsrUint8 shandle, CsrUint8 label, CsrUint16 length, CsrUint8 *data,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);


void CsrBtAvQosIndSend(av_instData_t *instData, CsrUint8 shandle, CsrUint16 bufferStatus);

void CsrBtAvSecurityInCfmSend(CsrSchedQid appHandle,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);
void CsrBtAvSecurityOutCfmSend(CsrSchedQid appHandle,
    CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier);

#ifdef CSR_BT_INSTALL_AV_SET_QOS_INTERVAL
void CsrBtAvSetQosIntervalReqHandler(av_instData_t *instData);
#endif
void CsrBtAvLpNegConfigReqHandler(av_instData_t *instData);
void CsrBtAvDelayReportReqHandler(av_instData_t *instData);

void CsrBtAvSdcResultHandler(void                     * inst,
                             CmnCsrBtLinkedListStruct * sdpTagList,
                             CsrBtDeviceAddr          deviceAddr,
                             CsrBtResultCode          resultCode,
                             CsrBtSupplier            resultSupplier);
CsrUint32 csrBtAvCalculateStreamBitRate(av_instData_t *instData, CsrUint8 *servCap, CsrUint16 servCapLen, CsrUint8 strIdx);
/* Prototypes from av_free_down.c */
void CsrBtAvFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

#endif
