#ifndef CSR_BT_OBEX_PRIVATE_UTIL_H__
#define CSR_BT_OBEX_PRIVATE_UTIL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_OBEX_UTIL_CONNECT_REQUEST_WITHDRAW           (CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_OBEX_UTIL_SETPATH_REQUEST_LENGTH + CSR_BT_OBEX_UTIL_MAX_DIGEST_RESPONSE_LENGTH + CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH)
#define CSR_BT_OBEX_UTIL_CONNECT_REQUEST_NO_AUTH_WITHDRAW   (CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_OBEX_UTIL_SETPATH_REQUEST_LENGTH + CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH)
#define CSR_BT_OBEX_UTIL_PUT_REQUEST_WITHDRAW               (CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH + CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_LENGTH)
#define CSR_BT_OBEX_UTIL_THRESHOLD                          (CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_OBEX_UTIL_MAX_DIGEST_RESPONSE_LENGTH + CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH + 0x20)
#define CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_WITHDRAW          0x2B /* Std Connect Rsp = 7, who = 3,  connId = 5, description = 3, challenge = 25 (realm Length comes from app) */
#define CSR_BT_OBEX_UTIL_CONNECT_RESPONSE_CLI_AUTH_WITHDRAW 0x36 /* Std Connect Rsp = 7, who = 3,  connId = 5, description = 3, digest = (61 - challenge) = 36. */
#define CSR_BT_OBEX_UTIL_SETPATH_RESPONSE_WITHDRAW          0x22 /* Std Setpath Rsp = 3, name = 3, description = 3, challenge = 25 (realm Length comes from app) */
#define CSR_BT_OBEX_UTIL_PUT_RESPONSE_WITHDRAW              0x22 /* Std Put     Rsp = 3, name = 3, description = 3, challenge = 25 (realm Length comes from app) */
#define CSR_BT_OBEX_UTIL_PUT_RESPONSE_NO_AUTH_WITHDRAW      0x09 /* Std Put     Rsp = 3, name = 3, description = 3, */
#define CSR_BT_OBEX_UTIL_GET_RESPONSE_WITHDRAW              0x2A /* Std Get     Rsp = 3, name = 3, description = 3, length = 5, body = 3, challenge = 25 (realm Length comes from app) */
#define CSR_BT_OBEX_UTIL_GET_RESPONSE_NO_AUTH_WITHDRAW      0x11 /* Std Get     Rsp = 3, name = 3, description = 3, length = 5, body = 3 */
#define CSR_BT_OBEX_UTIL_ACTION_RESPONSE_WITHDRAW           0x1F /* Std Action  Rsp = 3, description = 3, challenge = 25 (realm Length comes from app) */

#define CSR_BT_OBEX_UTIL_IDLE_RESULT_HANDLER                0x00
#define CSR_BT_OBEX_UTIL_CONNECT_REQUEST_RESULT_HANDLER     0x01
#define CSR_BT_OBEX_UTIL_DISCONNECT_REQUEST_RESULT_HANDLER  0x02
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
#define CSR_BT_OBEX_UTIL_SETPATH_REQUEST_RESULT_HANDLER     0x03
#endif
#define CSR_BT_OBEX_UTIL_PUT_REQUEST_RESULT_HANDLER         0x04
#define CSR_BT_OBEX_UTIL_GET_REQUEST_RESULT_HANDLER         0x05
#define CSR_BT_OBEX_UTIL_ABORT_REQUEST_RESULT_HANDLER       0x06
#define CSR_BT_OBEX_UTIL_ACTION_REQUEST_RESULT_HANDLER      0x07
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#define CSR_BT_OBEX_UTIL_NUM_OF_REQUEST_RESULT_HANDLER      0x08
#else
#define CSR_BT_OBEX_UTIL_NUM_OF_REQUEST_RESULT_HANDLER      0x07
#endif

/* Defines process states */
#define CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_P_S              0x00
#define CSR_BT_OBEX_UTIL_BT_L2CA_REGISTER_PSM_P_S           0x01
#define CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_P_S             0x02
#define CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECT_REQUEST_P_S      0x03
#define CSR_BT_OBEX_UTIL_BT_DISCONNECT_ABNORMAL_REQUEST_P_S 0x04
#define CSR_BT_OBEX_UTIL_BT_DISCONNECT_NORMAL_REQUEST_P_S   0x05
#define CSR_BT_OBEX_UTIL_BT_REGISTER_SERVERCH_REQUEST_P_S   0x06
#define CSR_BT_OBEX_UTIL_BT_REGISTER_SDP_RECORD_REQUEST_P_S 0x07
#define CSR_BT_OBEX_UTIL_BT_ACTIVATED_P_S                   0x08
#define CSR_BT_OBEX_UTIL_BT_CONNECTABLE_P_S                 0x09
#define CSR_BT_OBEX_UTIL_BT_NON_CONNECTABLE_P_S             0x0A
#define CSR_BT_OBEX_UTIL_BT_DEACTIVATING_P_S                0x0B
#define CSR_BT_OBEX_UTIL_BT_LIMITED_DEACTIVATING_P_S        0x0C
#define CSR_BT_OBEX_UTIL_BT_CONNECTED_P_S                   0x0D
#define CSR_BT_OBEX_UTIL_BT_PENDING_CONNECTABLE_P_S         0x0E
#define CSR_BT_OBEX_UTIL_OBEX_CONNECT_REQUEST_P_S           0x0F
#define CSR_BT_OBEX_UTIL_OBEX_CONNECTED_P_S                 0x10
#define CSR_BT_OBEX_UTIL_OBEX_SETPATH_REQUEST_P_S           0x11
#define CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_P_S               0x12
#define CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_IDLE_PENDING_P_S  0x13
#define CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_PENDING_P_S       0x14
#define CSR_BT_OBEX_UTIL_OBEX_PUT_REQUEST_SPLIT_P_S         0x15
#define CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_P_S               0x16
#define CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_SPLIT_P_S         0x17
#define CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_PENDING_P_S       0x18
#define CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_P_S             0x19
#define CSR_BT_OBEX_UTIL_OBEX_ABORT_REQUEST_PENDING_P_S     0x1A
#define CSR_BT_OBEX_UTIL_OBEX_AUTH_PENDING_P_S              0x1B
#define CSR_BT_OBEX_UTIL_OBEX_DISCONNECT_REQUEST_P_S        0x1C
#define CSR_BT_OBEX_UTIL_OBEX_CONNECT_RESPONSE_PENDING_P_S  0x1D
#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
#define CSR_BT_OBEX_UTIL_OBEX_SETPATH_RESPONSE_PENDING_P_S  0x1E
#endif
#define CSR_BT_OBEX_UTIL_OBEX_PUT_RESPONSE_PENDING_P_S      0x1F
#define CSR_BT_OBEX_UTIL_OBEX_ABORT_RESPONSE_PENDING_P_S    0x20
#define CSR_BT_OBEX_UTIL_OBEX_GET_RESPONSE_PENDING_P_S      0x21
#define CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_IDLE_PENDING_P_S  0x22
#define CSR_BT_OBEX_UTIL_OBEX_ACTION_REQUEST_P_S            0x23
#define CSR_BT_OBEX_UTIL_OBEX_ACTION_RESPONSE_PENDING_P_S   0x24
#define CSR_BT_OBEX_UTIL_OBEX_GET_REQUEST_SRM_REJECTED_P_S  0x25
#define CSR_BT_OBEX_UTIL_OBEX_FLUSH_PUT_REQUEST_PENDING_P_S 0x26

/* Defines secondary server process state */
#define CSR_BT_OBEX_UTIL_BT_IDLE_SERVER_P_S                 0x00
#define CSR_BT_OBEX_UTIL_BT_CONNECTABLE_SERVER_P_S          0x01
#define CSR_BT_OBEX_UTIL_BT_CANCEL_CONNECTABLE_SERVER_P_S   0x02
#define CSR_BT_OBEX_UTIL_BT_CONNECTED_SERVER_P_S            0x03
#define CSR_BT_OBEX_UTIL_BT_DISCONNECTING_SERVER_P_S        0x04

/* Defines CM states, e.g. used when receiveing an incoming CM message */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#define CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S                 0x00
#define CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTED_S                0x01
#define CSR_BT_OBEX_UTIL_BT_DISCONNECTING_S                 0x02
#define CSR_BT_OBEX_UTIL_BT_ACTIVATED_S                     0x03
#define CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S                  0x04
#define CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_S                0x05
#define CSR_BT_OBEX_UTIL_BT_L2CA_CONNECTING_S               0x06
#else
#define CSR_BT_OBEX_UTIL_BT_RFC_CONNECTED_S                 0x00
#define CSR_BT_OBEX_UTIL_BT_DISCONNECTING_S                 0x01
#define CSR_BT_OBEX_UTIL_BT_ACTIVATED_S                     0x02
#define CSR_BT_OBEX_UTIL_BT_DEACTIVATING_S                  0x03
#define CSR_BT_OBEX_UTIL_BT_RFC_CONNECTING_S                0x04
#endif

#define CSR_BT_OBEX_UTIL_CONNECT_LENGTH                     0x07
#define CSR_BT_OBEX_UTIL_SETPATH_REQUEST_LENGTH             0x05
#define CSR_BT_OBEX_UTIL_TARGET_WHO_LENGTH                  0x13
#define CSR_BT_OBEX_UTIL_LENGTH_OF_OBJECT_LENGTH            0x05 /*(one byte opcode + 4 byte payload)*/
#define CSR_BT_OBEX_UTIL_CONNECTION_ID_LENGTH               0x05 /*(one byte opcode + 4 byte payload)*/
#define CSR_BT_OBEX_UTIL_SINGLE_RESPONSE_MODE_LENGTH        0x02
#define CSR_BT_OBEX_UTIL_ACTION_ID_LENGTH                   0x02
#define CSR_BT_OBEX_UTIL_SRMP_LENGTH                        0x02
#define CSR_BT_OBEX_UTIL_COUNT_LENGTH                       0x05
#define CSR_BT_OBEX_UTIL_PERMISSIONS_LENGTH                 0x05
#define CSR_BT_OBEX_UTIL_SETPATH_FLAGS_INDEX                0x03
#define CSR_BT_OBEX_UTIL_SETPATH_CONSTANTS_INDEX            0x04


/* Note if Nonce (tag 0x02) shall be sent in the Digest Response this value
   must be change to 0x3D                                                   */
#define CSR_BT_OBEX_UTIL_MAX_DIGEST_RESPONSE_LENGTH         0x2B

#define CSR_BT_OBEX_UTIL_FIRST_ABORT_DELAY            (5000000)
#define CSR_BT_OBEX_UTIL_SECOND_ABORT_DELAY           (1000000)
#define CSR_BT_OBEX_UTIL_MAX_NUM_OF_HEADERS                 0x0B
#define CSR_BT_OBEX_UTIL_MAX_NUM_OF_HEADERS_IN_GET_RESPONSE 0x06

#define CSR_BT_OBEX_UTIL_CLIENT_ROLE                       (TRUE)
#define CSR_BT_OBEX_UTIL_SERVER_ROLE                       (FALSE)

#define CSR_BT_OBEX_UTIL_RFC_TRANSPORT                      0x00
#define CSR_BT_OBEX_UTIL_L2CAP_TRANSPORT                    0x01

#define CSR_BT_OBEX_UTIL_TX_WINDOW                         (0x20)   /* Note: The maximum value for this define is 0x3F */
#define CSR_BT_OBEX_UTIL_MAX_RETRANS                       (0xFF)
#define CSR_BT_OBEX_CONFTAB_WORK_SIZE                      (30)

/* Defines Single Response Mode (SRM) states */
#define CSR_BT_OBEX_UTIL_SRM_IDLE                           0x00
#define CSR_BT_OBEX_UTIL_SRM_REJECTED                       0x01
#define CSR_BT_OBEX_UTIL_SRM_ACTIVE                         0x02
#define CSR_BT_OBEX_UTIL_SRM_ACTIVE_PENDING                 0x03
#define CSR_BT_OBEX_UTIL_SRM_DISABLED                       0x04
#define CSR_BT_OBEX_UTIL_SRM_NOT_SUPPORTED                  0xFF

/* Defines SRM values */
#define CSR_BT_OBEX_UTIL_SRM_ENABLED                        0x01
#define CSR_BT_OBEX_UTIL_SRM_SUPPORTED                      0x02

/* Defines Single Response Mode Parameters (SRMP) states */
#define CSR_BT_OBEX_UTIL_SRMP_IDLE                          0x00
#define CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_REQUEST           0x01
#define CSR_BT_OBEX_UTIL_SRMP_WAIT_SERVER_REQUEST           0x02
#define CSR_BT_OBEX_UTIL_SRMP_WAIT_CLIENT_SERVER_REQUEST    0x03
#define CSR_BT_OBEX_UTIL_SRMP_DISABLED                      0x04


#define CSR_BT_OBEX_UTIL_TOTAL_NUM_OF_ACTION_CMD_HEADERS    0x05

typedef struct
{
    CsrBool                             obtainedServer;
    dm_security_level_t                 outgoingSecurityLevel;
#ifdef CSR_BT_INSTALL_OBEX_CLI_SET_PATH
    CsrUint8                            setpathFlags;
#endif
    CsrUint8                            abortNumOfRsp;
    CsrSchedTid                         abortSuperVisionTimerId;
    ObexUtilDisconnectIndFuncType       disconnectResultFunc;
    ObexUtilCommonRequestFuncType       commonResultFunc;
    ObexUtilPutContinueRequestFuncType  putContinueFunc;
#ifdef CSR_BT_INSTALL_OBEX_CLI_GET
    ObexUtilGetRequestFuncType          getResultFunc;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE
    ObexUtilAuthenticateIndFuncType     authResultFunc;
#endif
    CsrUint16                           abortIncomingObexPacketLength;
    CsrUint8                           *abortIncomingObexPacket;
    CsrUcs2String                      *abortDescription;
    CmnCsrBtLinkedListStruct           *sdpTag;
    ObexUtilConnectCfmFuncType          connectResultHandler;
    ObexUtilSelectSdpRecordFuncType     selectSdpRecordFunc;
    void                                *rfcConnectInst;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    psm_t                               remotePsm;
#endif
} ObexUtilCliInstType;

typedef struct
{
#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
    CsrBool                             deleteDigestChallenge;
    CsrBool                             authenticated;
#endif
    CsrUint8                            rfcSubProcessState;
    CsrUint16                           profileUuid;
    CsrUint24                           classOfDevice;
    dm_security_level_t                 secIncoming;
    CsrUint32                           serviceRecordHandle;
    CsrUint16                           rfcommMtu;
    CsrUint16                           serverChServiceRecordIndex;
    CsrUint16                           servChServiceRecordAltIndex;
    CsrUint16                           serviceRecordLength;
    CsrUint8                            *serviceRecord;
    CsrUint8                            opCode;
    CsrBool                             obexConnected;    
    ObexUtilServConnectFuncType         connectIndHandler;
    ObexUtilServPutFuncType             putIndHandler;
    ObexUtilServPutFuncType             putContinueIndHandler;
#ifdef CSR_BT_INSTALL_OBEX_SRV_GET
    ObexUtilServGetFuncType             getIndHandler;
    ObexUtilServGetFuncType             getContinueIndHandler;
#endif    
#ifdef CSR_BT_INSTALL_OBEX_SRV_SET_PATH
    ObexUtilServSetpathFuncType         setpathIndHandler;
#endif
    ObexUtilServAbortFuncType           abortIndHandler;
    ObexUtilServDisconnectFuncType      disconnectIndHandler;
    ObexUtilAuthenticateIndFuncType     authenticateIndHandler;
    ObexUtilServActivateFuncType        activateCfmHandler;
    ObexUtilServDeactivateFuncType      nonConnectableCfmHandler;
    ObexUtilServDeactivateFuncType      deactivateCfmHandler;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    CsrUint16                           psmServiceRecordIndex;
    ObexUtilServActionFuncType          actionIndHandler;
    CsrUint16                           l2capMtu;
    CsrUint8                            l2caSubProcessState;
#endif
} ObexUtilServInstType;


/* Instance data used intern by this library */
typedef struct
{
    CsrBool                             transmittingData;
    CsrBool                             headerSplit;
    CsrUint8                            processState;
    CsrUint8                            preProcessState;
    CsrUint8                            cmState;
    CsrBtDeviceAddr                     deviceAddr;
    CsrBtConnId                         btConnId;
    CsrUint8                            localServerCh;
    CsrTime                             lpSuperVisionTimeout;
    CsrSchedTid                         lpSuperVisionTimerId;
    CsrSchedTid                         obexCmdSuperVisionTimerId;
    CsrUint8                            actualLinkMode;
    CsrUint8                            requestedLinkMode;
    CsrUint16                           headerPayloadIndex;
    CsrUint16                           maxPeerObexPacketLength;
    CsrUint16                           outgoingObexPacketLength;
    CsrUint16                           maxTransportPacketLength;
    CsrUint16                           transportPacketIndex;
    CsrUint16                           maxLocalObexPacketLength; /* also L2CAP MTU */
    CsrUint16                           incomingObexPacketLength;
    CsrUint16                           incomingObexPacketIndex;
    CsrUint8                           *incomingObexPacket;
    CsrMblk                            *outgoingMblkPkt;
    CsrUint32                           lengthOfObject;
    CsrUint32                           count;   
    CsrBtObexResponseCode               responseCode;
    CsrBtResultCode                     resultCode;
    CsrBtSupplier                       resultSupplier;
    CsrUint16                           bodyLength;
    CsrUint16                           typeLength;
#if defined(CSR_BT_INSTALL_OBEX_CLI_HEADER_TARGET_WHO_CID) || defined(CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID)
    CsrUint32                           connectionId;
    CsrUint16                           targetWhoHeaderLength;
    CsrUint8                            *targetWho;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    CsrUint16                           digestChallengeLength;
    CsrUint8                            *digestChallengeHeader;
#endif
#if defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE) || defined(CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE)
    ObexUtilDigestChallengeType         *digestChallenge;
    CsrUint8                            peerChallengeNonce[CSR_BT_NONCE_LENGTH];
#endif    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_RESPONSE    
    CsrUint16                           digestResponseLength;
    CsrUint8                            *digestResponseHeader;
#endif    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_APP_PARAMS
    CsrUint8                            *appParametersHeader;
    CsrUint16                           appParametersHeaderLength;
#endif
#ifdef CSR_BT_INSTALL_OBEX_HEADER_USER_DEFINE
    CsrUint16                           userDefinedHeaderLength;
    CsrUint8                            *userDefinedHeader;
#endif
    CsrUcs2String                       *description;
    CsrUcs2String                       *name;
    CsrUint8                            *type;
    CsrUint8                            *body;
    CsrUint8                            numOfheaderTypes;
    CsrUint8                            headerTypeTable[CSR_BT_OBEX_UTIL_MAX_NUM_OF_HEADERS];
    CsrUint8                            firstHeaderTypeIndex;
    CsrUint8                            currentHeaderTypeIndex;
    CsrUint8                            lastHeaderTypeIndex;
    CsrUint8                            obexOperationIndex;
    ObexUtilCliInstType                *cliInst;
    ObexUtilServInstType               *servInst;
    void                               *obexUtilOwnInstData;
    void                               *profilesOwnInstData;  
    psm_t                               localPsm;
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    CsrUint8                            actionId;
    CsrUint8                            srmState;
    CsrUint8                            srmpState;
    CsrUint8                           *singleResponseModeHeader;
    CsrUint8                           *srmpHeader;
    CsrUint32                           permission;
    CsrUcs2String                      *destName;
    CsrUint16                           windowSize;
    CsrBool                             rxDataPending;
    CsrMessageQueueType                 *rxQueue;
    CsrUint16                           rxQueueCount;
#endif
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    ObexUtilDigestChallengeType         *clientDigestChallenge;
#endif 
} ObexUtilPrivateInstType;

typedef struct
{
    CsrSchedQid                         phandle;
    CsrUint8                            instId;
    void                               *profilesOwnInstData;
    ObexUtilPrivateInstType            *privateInst;
} ObexUtilInstType;

typedef CsrUint8 * (* ObexUtilBuildPacketHandlerType)(ObexUtilPrivateInstType *priInst,
                                                        CsrUint8 *transportBuffer,
                                                        CsrBool *next);

typedef CsrUint16 (* ObexUtilHeaderLengthHandlerType)(ObexUtilPrivateInstType *priInst);

typedef void (* ObexUtilCallBackHandleType)(ObexUtilPrivateInstType *priInst);

typedef void (* ObexUtilCmRfcUpstreamMsgHandleType)(ObexUtilPrivateInstType *priInst, 
                                                    void                    *msg);

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
typedef void (* ObexUtilCmL2caUpstreamMsgHandleType)(ObexUtilPrivateInstType *priInst, 
                                                     void                    **msg);
#endif

typedef CsrUint8 (* ObexUtilCmUpstreamMsgContextHandleType)(void *msg);


#ifdef __cplusplus
}
#endif

#endif /*CSR_BT_OBEX_PRIVATE_UTIL_H__*/

