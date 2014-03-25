#ifndef CSR_BT_MAPS_LIB_H__
#define CSR_BT_MAPS_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "csr_bt_maps_prim.h"
#include "csr_pmem.h"
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Common put_message function to reduce code size */
void CsrBtMapsMsgTransport(CsrSchedQid phandle, void *msg);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsGetInstanceIdsReqSend
 *
 *  DESCRIPTION
 *      This signal is used to get the list of registered Maps instances
 *      from the Maps instance that is also running as Maps-manager
 *
 *  PARAMETERS
 *        appHandle:    Application handle, so the Maps-manager
 *                      knows where to return the result to.
 *---------------------------------------------------------------------------*/
#define CsrBtMapsGetInstanceIdsReqSend(_appHandle) {                    \
        CsrBtMapsGetInstanceIdsReq *msg = pnew(CsrBtMapsGetInstanceIdsReq); \
        msg->type                      = CSR_BT_MAPS_GET_INSTANCE_IDS_REQ; \
        msg->appHandle                 = _appHandle;                    \
        CsrBtMapsMsgTransport(CSR_BT_MAPS_IFACEQUEUE,msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsSelectMasInstanceResSend
 *
 *  DESCRIPTION
 *      This signal is used in response to the CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
 *      where the application can specify which of the available masInstanceId's
 *      it wants to connect (if any).
 *
 *  PARAMETERS
 *      proceedWithConnection:    specifies whether to proceed with the connection
 *                                establishment or not.
 *      masInstanceId: the mas instance which the application wishes to connect
 *---------------------------------------------------------------------------*/
#define CsrBtMapsSelectMasInstanceResSend(_mapsInstanceId,_proceedWithConnection,_masInstanceId) { \
        CsrBtMapsSelectMasInstanceRes *msg = pnew(CsrBtMapsSelectMasInstanceRes); \
        msg->type                  = CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES; \
        msg->proceedWithConnection = _proceedWithConnection;            \
        msg->masInstanceId         = _masInstanceId;                    \
        CsrBtMapsMsgTransport(_mapsInstanceId,msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsActivateReqSend
 *
 *  DESCRIPTION
 *      Try to make a connection the a peer device.
 *
 *    PARAMETERS
 *        appHandle:                ...
 *        supportedMessages        ...
 *----------------------------------------------------------------------------*/
#define CsrBtMapsActivateExtReqSend(_mapsInstanceId,_appHandle,_supportedMessages,_obexMaxPacketSize,_windowSize,_srmEnable,_instanceId,_nameLen,_name) { \
        CsrBtMapsActivateReq *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsActivateReq)); \
        msg__->type              = CSR_BT_MAPS_ACTIVATE_REQ;            \
        msg__->appHandle         = _appHandle;                          \
        msg__->supportedMessages = _supportedMessages;                  \
        msg__->obexMaxPacketSize = _obexMaxPacketSize;                  \
        msg__->windowSize        = _windowSize;                         \
        msg__->srmEnable         = _srmEnable;                          \
        msg__->instanceId        = _instanceId;                         \
        msg__->nameLen           = _nameLen;                            \
        msg__->name              = _name;                               \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

#define CsrBtMapsActivateReqSend(_mapsInstanceId,_appHandle,_supportedMessages,_obexMaxPacketSize,_windowSize,_srmEnable) \
    CsrBtMapsActivateExtReqSend(_mapsInstanceId,_appHandle,_supportedMessages,_obexMaxPacketSize,_windowSize,_srmEnable,0,0,NULL)

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsDeactivateReqSend
 *
 *  DESCRIPTION
 *      Try to make a connection the a peer device.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtMapsDeactivateReqSend(_mapsInstanceId) {                   \
        CsrBtMapsDeactivateReq *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsDeactivateReq)); \
        msg__->type = CSR_BT_MAPS_DEACTIVATE_REQ;                       \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsConnectResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        obexMaxPacketSize;      ....
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsConnectResSend(_mapsInstanceId, _responseCode) {       \
        CsrBtMapsConnectRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsConnectRes)); \
        msg__->type = CSR_BT_MAPS_CONNECT_RES;                          \
        msg__->responseCode = _responseCode;                            \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsSetBackFolderResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsSetBackFolderResSend(_mapsInstanceId, _responseCode) { \
        CsrBtMapsSetBackFolderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsSetBackFolderRes)); \
        msg__->type = CSR_BT_MAPS_SET_BACK_FOLDER_RES;                  \
        msg__->responseCode = _responseCode;                            \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsSetRootFolderResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsSetRootFolderResSend(_mapsInstanceId, _responseCode) { \
        CsrBtMapsSetRootFolderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsSetRootFolderRes)); \
        msg__->type = CSR_BT_MAPS_SET_ROOT_FOLDER_RES;                  \
        msg__->responseCode = _responseCode;                            \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsSetFolderResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsSetFolderResSend(_mapsInstanceId, _responseCode) {     \
        CsrBtMapsSetFolderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsSetFolderRes)); \
        msg__->type = CSR_BT_MAPS_SET_FOLDER_RES;                       \
        msg__->responseCode = _responseCode;                            \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsGetFolderListingHeaderResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        fullFolderListingSize       ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsGetFolderListingHeaderResSend(_mapsInstanceId, _fullFolderListingSize, _srmpOn) { \
        CsrBtMapsGetFolderListingHeaderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsGetFolderListingHeaderRes)); \
        msg__->type = CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES;        \
        msg__->fullFolderListingSize = _fullFolderListingSize;          \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsGetFolderListingResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *        bodyLength;           ....
 *        *body;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsGetFolderListingResSend(_mapsInstanceId, _responseCode, _bodyLength, _body, _srmpOn) { \
        CsrBtMapsGetFolderListingRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsGetFolderListingRes)); \
        msg__->type = CSR_BT_MAPS_GET_FOLDER_LISTING_RES;               \
        msg__->responseCode = _responseCode;                            \
        msg__->bodyLength = _bodyLength;                                \
        msg__->body = _body;                                            \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsGetMessageListingHeaderResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        newMessages       ....
 *        *mseTime       ....
 *        fullMessageListingSize       ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsGetMessageListingHeaderResSend(_mapsInstanceId, _newMessages, _mseTime, _fullMessageListingSize, _srmpOn) { \
        CsrBtMapsGetMessageListingHeaderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsGetMessageListingHeaderRes)); \
        msg__->type = CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES;       \
        msg__->newMessages = _newMessages;                              \
        msg__->mseTime = _mseTime;                                      \
        msg__->fullMessageListingSize = _fullMessageListingSize;        \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsGetMessageListingResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *        bodyLength;           ....
 *        *body;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsGetMessageListingResSend(_mapsInstanceId, _responseCode, _bodyLength, _body, _srmpOn) { \
        CsrBtMapsGetMessageListingRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsGetMessageListingRes)); \
        msg__->type = CSR_BT_MAPS_GET_MESSAGE_LISTING_RES;              \
        msg__->responseCode = _responseCode;                            \
        msg__->bodyLength = _bodyLength;                                \
        msg__->body = _body;                                            \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsGetMessageHeaderResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        fractionDeliver      ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsGetMessageHeaderResSend(_mapsInstanceId, _fractionDeliver, _srmpOn) { \
        CsrBtMapsGetMessageHeaderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsGetMessageHeaderRes)); \
        msg__->type = CSR_BT_MAPS_GET_MESSAGE_HEADER_RES;               \
        msg__->fractionDeliver = _fractionDeliver;                      \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsGetMessageResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *        bodyLength;           ....
 *        *body;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsGetMessageResSend(_mapsInstanceId, _responseCode, _bodyLength, _body, _srmpOn) { \
        CsrBtMapsGetMessageRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsGetMessageRes)); \
        msg__->type = CSR_BT_MAPS_GET_MESSAGE_RES;                      \
        msg__->responseCode = _responseCode;                            \
        msg__->bodyLength = _bodyLength;                                \
        msg__->body = _body;                                            \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsSetMessageStatusResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsSetMessageStatusResSend(_mapsInstanceId, _responseCode, _srmpOn) { \
        CsrBtMapsSetMessageStatusRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsSetMessageStatusRes)); \
        msg__->type = CSR_BT_MAPS_SET_MESSAGE_STATUS_RES;               \
        msg__->responseCode = _responseCode;                            \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsPushMessageHeaderResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        messageHandler;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsPushMessageHeaderResSend(_mapsInstanceId, _messageHandle, _srmpOn) { \
        CsrBtMapsPushMessageHeaderRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsPushMessageHeaderRes)); \
        msg__->type          = CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES;     \
        msg__->messageHandle = _messageHandle;                          \
        msg__->srmpOn        = _srmpOn;                                 \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsPushMessageResSend
 *      CsrBtMapsPushMessageFinalResSend
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsPushMessageFinalResSend(_mapsInstanceId, _responseCode, _srmpOn, _messageHandle) { \
        CsrBtMapsPushMessageRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsPushMessageRes)); \
        msg__->type          = CSR_BT_MAPS_PUSH_MESSAGE_RES;            \
        msg__->responseCode  = _responseCode;                           \
        msg__->srmpOn        = _srmpOn;                                 \
        msg__->messageHandle = _messageHandle;                          \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

#define CsrBtMapsPushMessageResSend(_mapsInstanceId, _responseCode, _srmpOn) CsrBtMapsPushMessageFinalResSend(_mapsInstanceId, _responseCode, _srmpOn, NULL)


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsUpdateInboxResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsUpdateInboxResSend(_mapsInstanceId, _responseCode, _srmpOn) { \
        CsrBtMapsUpdateInboxRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsUpdateInboxRes)); \
        msg__->type = CSR_BT_MAPS_UPDATE_INBOX_RES;                     \
        msg__->responseCode = _responseCode;                            \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsNotificationRegistrationResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        responseCode;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsNotificationRegistrationResSend(_mapsInstanceId, _responseCode, _srmpOn) { \
        CsrBtMapsNotificationRegistrationRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsNotificationRegistrationRes)); \
        msg__->type = CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES;        \
        msg__->responseCode = _responseCode;                            \
        msg__->srmpOn = _srmpOn;                                        \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsEventNotificationReqSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *      ...
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsEventNotificationReqSend(_mapsInstanceId) {            \
        CsrBtMapsEventNotificationReq *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsEventNotificationReq)); \
        msg__->type = CSR_BT_MAPS_EVENT_NOTIFICATION_REQ;               \
        msg__->instanceId = _mapsInstanceId;                            \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsEventNotificationResSend
 *
 *  DESCRIPTION
 *      ...
 *
 *    PARAMETERS
 *        finalFlag;           ....
 *        bodyLength;           ....
 *        *body;           ....
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsEventNotificationResSend(_mapsInstanceId, _responseCode, _bodyLength, _body) { \
        CsrBtMapsEventNotificationRes *msg__ = CsrPmemAlloc(sizeof(CsrBtMapsEventNotificationRes)); \
        msg__->type = CSR_BT_MAPS_EVENT_NOTIFICATION_RES;               \
        msg__->responseCode = _responseCode;                            \
        msg__->bodyLength = _bodyLength;                                \
        msg__->body = _body;                                            \
        CsrBtMapsMsgTransport(_mapsInstanceId, msg__);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsSecurityLevelReqSend
 *
 *  DESCRIPTION
 *      Set the default security settings for new incoming/outgoing connections
 *
 *  PARAMETERS
 *       secLevel        The security level to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtMapsSecurityLevelReqSend(_mapsInstanceId,_appHandle, _secLevel) { \
        CsrBtMapsSecurityLevelReq *msg__;                               \
        msg__ = (CsrBtMapsSecurityLevelReq*)CsrPmemAlloc(sizeof(CsrBtMapsSecurityLevelReq)); \
        msg__->type = CSR_BT_MAPS_SECURITY_LEVEL_REQ;                   \
        msg__->appHandle = _appHandle;                                  \
        msg__->secLevel = _secLevel;                                    \
        CsrBtMapsMsgTransport(_mapsInstanceId,msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtMapsFreeUpstreamMessageContents
 *
 *  DESCRIPTION
 *      During Bluetooth shutdown all allocated payload in the Synergy BT Maps
 *      message must be deallocated. This is done by this function
 *
 *
 *    PARAMETERS
 *      eventClass :  Must be CSR_BT_MAPS_PRIM,
 *      msg:          The message received from Synergy BT Maps
 *----------------------------------------------------------------------------*/
void CsrBtMapsFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif

#endif
