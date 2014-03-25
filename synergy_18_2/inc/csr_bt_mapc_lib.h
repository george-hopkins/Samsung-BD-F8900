#ifndef CSR_BT_MAPC_LIB_H__
#define CSR_BT_MAPC_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_mapc_prim.h"
#include "csr_pmem.h"
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Common put_message function to reduce code size */
void CsrBtMapcMsgTransport(CsrSchedQid phandle, void *msg);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcGetInstanceIdsReqSend
 *
 *  DESCRIPTION
 *      This signal is used to get the list of registered MAPC instances
 *      from the MAPC instance that is also running as MAPC-manager
 *
 *  PARAMETERS
 *        appHandle:    Application handle, so the MAPC-manager
 *                      knows where to return the result to.
 *---------------------------------------------------------------------------*/
#define CsrBtMapcGetInstanceIdsReqSend(_appHandle) {                    \
        CsrBtMapcGetInstanceIdsReq *msg = pnew(CsrBtMapcGetInstanceIdsReq); \
        msg->type                      = CSR_BT_MAPC_GET_INSTANCE_IDS_REQ; \
        msg->appHandle                 = _appHandle;                    \
        CsrBtMapcMsgTransport(CSR_BT_MAPC_IFACEQUEUE,msg);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcConnectReqSend
 *
 *  DESCRIPTION
 *      Try to make a connection the a peer device.
 *
 *    PARAMETERS
 *        appHandle:                ...
 *        maxPacketSize:            ...
 *        deviceAddr:              ...
 *        length:               Length is used to express the approximate total
 *                              length of the bodies of all the objects in the
 *                              transaction
 *        count:                Count is used to indicate the number of objects
 *                              that will be sent during this connection
 *----------------------------------------------------------------------------*/
#define CsrBtMapcConnectReqSend(_mapcInstanceId,_appHandle,_maxPacketSize,_deviceAddr,_length,_count,_windowSize,_srmEnable) { \
        CsrBtMapcConnectReq *msg__ = pnew(CsrBtMapcConnectReq);         \
        msg__->type           = CSR_BT_MAPC_CONNECT_REQ;                \
        msg__->appHandle      = _appHandle;                             \
        msg__->maxPacketSize  = _maxPacketSize;                         \
        msg__->deviceAddr     = _deviceAddr;                            \
        msg__->length         = _length;                                \
        msg__->count          = _count;                                 \
        msg__->windowSize     = _windowSize;                            \
        msg__->srmEnable      = _srmEnable;                             \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcCancelConnectReqSend
 *
 *  DESCRIPTION
 *      Try to cancel the connection which is being establish to a peer device.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtMapcCancelConnectReqSend(_mapcInstanceId) {                \
        CsrBtMapcCancelConnectReq *msg__ = pnew(CsrBtMapcCancelConnectReq); \
        msg__->type    = CSR_BT_MAPC_CANCEL_CONNECT_REQ;                \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcDisconnectReqSend
 *
 *  DESCRIPTION
 *      The OBEX - and the Bluetooth connection is release
 *
 *    PARAMETERS
 *        normalObexDisconnect : FALSE defines an Abnormal disconnect sequence where
 *                               the Bluetooth connection is release direct. TRUE
 *                               defines a normal disconnect sequence where the OBEX
 *                               connection is release before the Bluetooth connection
 *----------------------------------------------------------------------------*/
#define CsrBtMapcDisconnectReqSend(_mapcInstanceId,_normalObexDisconnect) { \
        CsrBtMapcDisconnectReq *msg__ = pnew(CsrBtMapcDisconnectReq);   \
        msg__->type                  = CSR_BT_MAPC_DISCONNECT_REQ;      \
        msg__->normalObexDisconnect  = _normalObexDisconnect;           \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcSelectMasInstanceResSend
 *
 *  DESCRIPTION
 *      This signal is used in response to the CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
 *      where the application can specify which of the available masInstanceId's
 *      it wants to connect (if any).
 *
 *  PARAMETERS
 *      proceedWithConnection:    specifies whether to proceed with the connection
 *                                establishment or not.
 *      masInstanceId: the mas instance which the application wishes to connect
 *---------------------------------------------------------------------------*/
#define CsrBtMapcSelectMasInstanceResSend(_mapcInstanceId,_proceedWithConnection,_masInstanceId) { \
        CsrBtMapcSelectMasInstanceRes *msg = pnew(CsrBtMapcSelectMasInstanceRes); \
        msg->type                  = CSR_BT_MAPC_SELECT_MAS_INSTANCE_RES; \
        msg->proceedWithConnection = _proceedWithConnection;            \
        msg->masInstanceId         = _masInstanceId;                    \
        CsrBtMapcMsgTransport(_mapcInstanceId,msg);}



/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcSetFolderReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *        folderName:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcSetFolderReqSend(_mapcInstanceId,_folderName) {        \
        CsrBtMapcSetFolderReq *msg__ = pnew(CsrBtMapcSetFolderReq);     \
        msg__->type              = CSR_BT_MAPC_SET_FOLDER_REQ;          \
        msg__->folderName              = _folderName;                   \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcSetBackFolderReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtMapcSetBackFolderReqSend(_mapcInstanceId) {                \
        CsrBtMapcSetBackFolderReq *msg__ = pnew(CsrBtMapcSetBackFolderReq); \
        msg__->type              = CSR_BT_MAPC_SET_BACK_FOLDER_REQ;     \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcSetBackFolderReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtMapcSetRootFolderReqSend(_mapcInstanceId) {                \
        CsrBtMapcSetRootFolderReq *msg__ = pnew(CsrBtMapcSetRootFolderReq); \
        msg__->type              = CSR_BT_MAPC_SET_ROOT_FOLDER_REQ;     \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcGetFolderListingReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *        maxListCount:          ...
 *        listStartOffset:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcGetFolderListingReqSend(_mapcInstanceId,_maxListCount, _listStartOffset, _srmpOn) { \
        CsrBtMapcGetFolderListingReq *msg__ = pnew(CsrBtMapcGetFolderListingReq); \
        msg__->type              = CSR_BT_MAPC_GET_FOLDER_LISTING_REQ;  \
        msg__->maxListCount      = _maxListCount;                       \
        msg__->listStartOffset   = _listStartOffset;                    \
        msg__->srmpOn            = _srmpOn;                             \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcGetFolderListingResSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtMapcGetFolderListingResSend(_mapcInstanceId, _srmpOn) {    \
        CsrBtMapcGetFolderListingRes *msg__ = pnew(CsrBtMapcGetFolderListingRes); \
        msg__->type              = CSR_BT_MAPC_GET_FOLDER_LISTING_RES;  \
        msg__->srmpOn            = _srmpOn;                             \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcGetMessageListingReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *       folderName:            ...
 *       maxListCount:          ...
 *       listStartOffset:       ...
 *       maxSubjectLength:      ...
 *       parameterMask:         ...
 *       filterMessageType:     ...
 *       filterPeriodBegin:     ...
 *       filterPeriodEnd:       ...
 *       filterReadStatus:      ...
 *       filterRecipient:       ...
 *       filterOriginator:      ...
 *       filterPriority:        ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcGetMessageListingReqSend(_mapcInstanceId, _folderName, _maxListCount, _listStartOffset, _maxSubjectLength, _parameterMask, _filterMessageType, _filterPeriodBegin, \
                                          _filterPeriodEnd, _filterReadStatus, _filterRecipient, _filterOriginator, _filterPriority, _srmpOn) { \
        CsrBtMapcGetMessageListingReq *msg__ = pnew(CsrBtMapcGetMessageListingReq); \
        msg__->type                   = CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ; \
        msg__->folderName             = _folderName;                    \
        msg__->maxListCount           = _maxListCount;                  \
        msg__->listStartOffset        = _listStartOffset;               \
        msg__->maxSubjectLength       = _maxSubjectLength;              \
        msg__->parameterMask          = _parameterMask;                 \
        msg__->filterMessageType      = _filterMessageType;             \
        msg__->filterPeriodBegin      = _filterPeriodBegin;             \
        msg__->filterPeriodEnd        = _filterPeriodEnd;               \
        msg__->filterReadStatus       = _filterReadStatus;              \
        msg__->filterRecipient        = _filterRecipient;               \
        msg__->filterOriginator       = _filterOriginator;              \
        msg__->filterPriority         = _filterPriority;                \
        msg__->srmpOn                 = _srmpOn;                        \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcGetMessageListingResSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtMapcGetMessageListingResSend(_mapcInstanceId, _srmpOn) {   \
        CsrBtMapcGetMessageListingRes *msg__ = pnew(CsrBtMapcGetMessageListingRes); \
        msg__->type              = CSR_BT_MAPC_GET_MESSAGE_LISTING_RES; \
        msg__->srmpOn            = _srmpOn;                             \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      MapcGetMessageReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *       messageHandle:         ...
 *       attachment:            ...
 *       charset:               ...
 *       fractionRequest:       ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcGetMessageReqSend(_mapcInstanceId,_messageHandle, _attachment, _charset, _fractionRequest, _srmpOn) { \
        CsrBtMapcGetMessageReq *msg__ = pnew(CsrBtMapcGetMessageReq);   \
        msg__->type                      = CSR_BT_MAPC_GET_MESSAGE_REQ; \
        msg__->messageHandle             = _messageHandle;              \
        msg__->attachment                = _attachment;                 \
        msg__->charset                   = _charset;                    \
        msg__->fractionRequest           = _fractionRequest;            \
        msg__->srmpOn                    = _srmpOn;                     \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcGetMessageResSend
 *
 *  DESCRIPTION
 *      .....
 *
 *    PARAMETERS
 *                    None
 *----------------------------------------------------------------------------*/
#define CsrBtMapcGetMessageResSend(_mapcInstanceId, _srmpOn) {          \
        CsrBtMapcGetMessageRes *msg__ = pnew(CsrBtMapcGetMessageRes);   \
        msg__->type              = CSR_BT_MAPC_GET_MESSAGE_RES;         \
        msg__->srmpOn            = _srmpOn;                             \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcSetMessageStatusReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *        messageHandle:            ...
 *        statusIndicator:          ...
 *        statusValue:              ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcSetMessageStatusReqSend(_mapcInstanceId,_messageHandle, _statusIndicator, _statusValue) { \
        CsrBtMapcSetMessageStatusReq *msg__ = pnew(CsrBtMapcSetMessageStatusReq); \
        msg__->type                      = CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ; \
        msg__->messageHandle             = _messageHandle;              \
        msg__->statusIndicator           = _statusIndicator;            \
        msg__->statusValue               = _statusValue;                \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcPushMessageReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *        folderName:               ...
 *        lengthOfObject:               ...
 *        transparent:               ...
 *        retry:               ...
 *        charset:               ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcPushMessageReqSend(_mapcInstanceId,_folderName,_lengthOfObject, _transparent, _retry, _charset) { \
        CsrBtMapcPushMessageReq *msg__ = pnew(CsrBtMapcPushMessageReq); \
        msg__->type                      = CSR_BT_MAPC_PUSH_MESSAGE_REQ; \
        msg__->folderName                = _folderName;                 \
        msg__->lengthOfObject            = _lengthOfObject;             \
        msg__->transparent               = _transparent;                \
        msg__->retry                     = _retry;                      \
        msg__->charset                   = _charset;                    \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcPushMessageResSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *        finalFlag:        ...
 *        payloadLength:        ...
 *        *payload:            ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcPushMessageResSend(_mapcInstanceId,_finalFlag,_payloadLength,_payload) { \
        CsrBtMapcPushMessageRes *msg__ = pnew(CsrBtMapcPushMessageRes); \
        msg__->type                     = CSR_BT_MAPC_PUSH_MESSAGE_RES; \
        msg__->finalFlag                = _finalFlag;                   \
        msg__->payloadLength            = _payloadLength;               \
        msg__->payload                  = _payload;                     \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcUpdateInboxReqSend
 *
 *  DESCRIPTION
 *      ......
 *
 *    PARAMETERS
 *        None
 *----------------------------------------------------------------------------*/
#define CsrBtMapcUpdateInboxReqSend(_mapcInstanceId) {                  \
    CsrBtMapcUpdateInboxReq *msg__ = pnew(CsrBtMapcUpdateInboxReq);     \
    msg__->type                  = CSR_BT_MAPC_UPDATE_INBOX_REQ;        \
    CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcAbortReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *    PARAMETERS
 *        None
 *----------------------------------------------------------------------------*/
#define CsrBtMapcAbortReqSend(_mapcInstanceId) {                        \
        CsrBtMapcAbortReq *msg__ = pnew(CsrBtMapcAbortReq);             \
        msg__->type                  = CSR_BT_MAPC_ABORT_REQ;           \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcNotificationRegistrationReqSend
 *
 *  DESCRIPTION
 *      .....
 *
 *    PARAMETERS
 *        enableNotifications:        ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcNotificationRegistrationReqSend(_mapcInstanceId, _enableNotifications, _mnsController) { \
        CsrBtMapcNotificationRegistrationReq *msg__ = pnew(CsrBtMapcNotificationRegistrationReq); \
        msg__->type                  = CSR_BT_MAPC_NOTIFICATION_REGISTRATION_REQ; \
        msg__->enableNotifications   = _enableNotifications;            \
        msg__->mnsController         = _mnsController;                  \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcEventNotificationResSend
 *
 *  DESCRIPTION
 *      .....
 *
 *    PARAMETERS
 *        response:          ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapcEventNotificationResSend(_mapcInstanceId,_response,_srmpOn) { \
        CsrBtMapcEventNotificationRes *msg__ = pnew(CsrBtMapcEventNotificationRes); \
        msg__->type                  = CSR_BT_MAPC_EVENT_NOTIFICATION_RES; \
        msg__->response              = _response;                       \
        msg__->srmpOn                = _srmpOn;                         \
        CsrBtMapcMsgTransport(_mapcInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcSecurityInReqSend
 *
 *  DESCRIPTION
 *      Set the default security settings for new incoming/outgoing connections
 *
 *  PARAMETERS
 *       secLevel        The security level to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapcSecurityInReqSend(_mapcInstanceId,_appHandle, _secLevel) { \
        CsrBtMapcSecurityInReq  *msg = pnew(CsrBtMapcSecurityInReq);    \
        msg->type          = CSR_BT_MAPC_SECURITY_IN_REQ;               \
        msg->appHandle     = _appHandle;                                \
        msg->secLevel      = _secLevel;                                 \
        CsrBtMapcMsgTransport(_mapcInstanceId,msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcSecurityOutReqSend
 *
 *  DESCRIPTION
 *      Set the default security settings for new incoming/outgoing connections
 *
 *  PARAMETERS
 *       secLevel        The security level to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapcSecurityOutReqSend(_mapcInstanceId,_appHandle, _secLevel) { \
        CsrBtMapcSecurityOutReq *msg = pnew(CsrBtMapcSecurityOutReq);   \
        msg->type          = CSR_BT_MAPC_SECURITY_OUT_REQ;              \
        msg->appHandle     = _appHandle;                                \
        msg->secLevel      = _secLevel;                                 \
        CsrBtMapcMsgTransport(_mapcInstanceId,msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapcFreeUpstreamMessageContents
 *
 *  DESCRIPTION
 *      During Bluetooth shutdown all allocated payload in the Synergy BT MAPC
 *      message must be deallocated. This is done by this function
 *
 *
 *    PARAMETERS
 *      eventClass :  Must be CSR_BT_MAPC_PRIM,
 *      msg:          The message received from Synergy BT MAPC
 *----------------------------------------------------------------------------*/
void CsrBtMapcFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif
