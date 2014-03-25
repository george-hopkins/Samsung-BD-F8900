#ifndef CSR_BT_MAPS_PRIM_H__
#define CSR_BT_MAPS_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_unicode.h"
#include "csr_bt_profiles.h"
#include "csr_bt_obex.h"
#include "csr_bt_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrBtMapsPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

typedef CsrPrim    CsrBtMapsPrim;

/*******************************************************************************
 * Primitive definitions
 *******************************************************************************/
#define CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST                               (0x0000)

#define CSR_BT_MAPS_GET_INSTANCE_IDS_REQ                ((CsrBtMapsPrim) (0x0000 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_SELECT_MAS_INSTANCE_RES             ((CsrBtMapsPrim) (0x0001 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_ACTIVATE_REQ                        ((CsrBtMapsPrim) (0x0002 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_DEACTIVATE_REQ                      ((CsrBtMapsPrim) (0x0003 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_CONNECT_RES                         ((CsrBtMapsPrim) (0x0004 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_BACK_FOLDER_RES                 ((CsrBtMapsPrim) (0x0005 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_ROOT_FOLDER_RES                 ((CsrBtMapsPrim) (0x0006 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_FOLDER_RES                      ((CsrBtMapsPrim) (0x0007 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_RES       ((CsrBtMapsPrim) (0x0008 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_FOLDER_LISTING_RES              ((CsrBtMapsPrim) (0x0009 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES      ((CsrBtMapsPrim) (0x000A + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_LISTING_RES             ((CsrBtMapsPrim) (0x000B + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_HEADER_RES              ((CsrBtMapsPrim) (0x000C + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_RES                     ((CsrBtMapsPrim) (0x000D + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_MESSAGE_STATUS_RES              ((CsrBtMapsPrim) (0x000E + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES             ((CsrBtMapsPrim) (0x000F + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_PUSH_MESSAGE_RES                    ((CsrBtMapsPrim) (0x0010 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_UPDATE_INBOX_RES                    ((CsrBtMapsPrim) (0x0011 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_NOTIFICATION_REGISTRATION_RES       ((CsrBtMapsPrim) (0x0012 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_EVENT_NOTIFICATION_REQ              ((CsrBtMapsPrim) (0x0013 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_EVENT_NOTIFICATION_RES              ((CsrBtMapsPrim) (0x0014 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_SECURITY_LEVEL_REQ                  ((CsrBtMapsPrim) (0x0015 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))
#define CSR_BT_MAPS_REGISTER_QID_REQ                    ((CsrBtMapsPrim) (0x0016 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST))

#define CSR_BT_MAPS_PRIM_DOWNSTREAM_HIGHEST                              (0x0016 + CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST)

/*******************************************************************************/

#define CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST                                 (0x0000 + CSR_PRIM_UPSTREAM)

#define CSR_BT_MAPS_GET_INSTANCE_IDS_CFM                    ((CsrBtMapsPrim) (0x0000 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND                 ((CsrBtMapsPrim) (0x0001 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_ACTIVATE_CFM                            ((CsrBtMapsPrim) (0x0002 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_DEACTIVATE_CFM                          ((CsrBtMapsPrim) (0x0003 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_CONNECT_IND                             ((CsrBtMapsPrim) (0x0004 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_DISCONNECT_IND                          ((CsrBtMapsPrim) (0x0005 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_ABORT_IND                               ((CsrBtMapsPrim) (0x0006 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_BACK_FOLDER_IND                     ((CsrBtMapsPrim) (0x0007 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_ROOT_FOLDER_IND                     ((CsrBtMapsPrim) (0x0008 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_FOLDER_IND                          ((CsrBtMapsPrim) (0x0009 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND           ((CsrBtMapsPrim) (0x000A + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_FOLDER_LISTING_IND                  ((CsrBtMapsPrim) (0x000B + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND          ((CsrBtMapsPrim) (0x000C + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_LISTING_IND                 ((CsrBtMapsPrim) (0x000D + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_HEADER_IND                  ((CsrBtMapsPrim) (0x000E + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_GET_MESSAGE_IND                         ((CsrBtMapsPrim) (0x000F + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_SET_MESSAGE_STATUS_IND                  ((CsrBtMapsPrim) (0x0010 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND                 ((CsrBtMapsPrim) (0x0011 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_PUSH_MESSAGE_IND                        ((CsrBtMapsPrim) (0x0012 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_UPDATE_INBOX_IND                        ((CsrBtMapsPrim) (0x0013 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND           ((CsrBtMapsPrim) (0x0014 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND  ((CsrBtMapsPrim) (0x0015 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_EVENT_NOTIFICATION_IND                  ((CsrBtMapsPrim) (0x0016 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_EVENT_NOTIFICATION_CFM                  ((CsrBtMapsPrim) (0x0017 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))
#define CSR_BT_MAPS_SECURITY_LEVEL_CFM                      ((CsrBtMapsPrim) (0x0018 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST))

#define CSR_BT_MAPS_PRIM_UPSTREAM_HIGHEST                                    (0x0018 + CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST)

#define CSR_BT_MAPS_PRIM_DOWNSTREAM_COUNT               (CSR_BT_MAPS_PRIM_DOWNSTREAM_HIGHEST + 1 - CSR_BT_MAPS_PRIM_DOWNSTREAM_LOWEST)
#define CSR_BT_MAPS_PRIM_UPSTREAM_COUNT                 (CSR_BT_MAPS_PRIM_UPSTREAM_HIGHEST + 1 - CSR_BT_MAPS_PRIM_UPSTREAM_LOWEST)
/*******************************************************************************
 * End primitive definitions
 *******************************************************************************/
/*** MAPS MULTI INSTANCE HANDLING ***/
typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             appHandle;                      /* application handle */
} CsrBtMapsGetInstanceIdsReq;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
     CsrUint8               instanceIdsListSize;            /* number of _items_ in instanceIdsList, _not_ length in bytes */
     CsrSchedQid            *instanceIdsList;               /* list of instance */
} CsrBtMapsGetInstanceIdsCfm;

typedef struct
{
    CsrBtDeviceName         serviceName;                    /* ServiceName from SDP record */
    CsrUint8                masInstanceId;                  /* MASInstanceID from SDP record */
    CsrBtMapMesSupport      supportedMessages;              /* bit pattern of supported message types in this MAS instance */
} CsrBtMapsMasInstance;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBtMapsMasInstance    *masInstanceList;               /* pointer to the list of available MAS instances on the peer */
    CsrUint16               masInstanceListSize;            /* Number of items in masInstanceList */
} CsrBtMapsSelectMasInstanceInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBool                 proceedWithConnection;          /* if TRUE the connection establishment is continued with the given masInstanceId */
    CsrUint8                masInstanceId;                  /* MASInstanceID to connect */
} CsrBtMapsSelectMasInstanceRes;


/*** CONNECTION HANDLING ***/
typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             appHandle;                      /* application handle */
    CsrBtMapMesSupport      supportedMessages;              /* bit pattern of supported message types in the server */
    CsrUint16               obexMaxPacketSize;
    CsrUint16               windowSize;
    CsrBool                 srmEnable;
    CsrUint8                instanceId;                     /* MASInstanceID to register */
    CsrUint16               nameLen;                        /* Length of the name of the MASInstance */
    CsrUint8                *name;                          /* Name of the MASInstance */
} CsrBtMapsActivateReq;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtMapsActivateCfm;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
} CsrBtMapsDeactivateReq;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
} CsrBtMapsDeactivateCfm;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBtDeviceAddr         deviceAddr;
    CsrUint32               length;
    CsrUint32               count;
    CsrBtConnId             btConnId;               /* Global Bluetooth connection ID */
} CsrBtMapsConnectInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBtDeviceAddr         deviceAddr;
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtMapsNotificationRegistrationCompleteInd;

typedef struct
{
    CsrBtMapsPrim           type;
    CsrBtObexResponseCode   responseCode;
} CsrBtMapsConnectRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBtReasonCode         reasonCode;
    CsrBtSupplier           reasonSupplier;
} CsrBtMapsDisconnectInd;

typedef struct
{
    CsrBtMapsPrim           type;
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUint16               descriptionOffset;              /* Payload relative offset */
    CsrUint16               descriptionLength;
    CsrUint8              *payload;
    CsrUint16               payloadLength;
} CsrBtMapsAbortInd;


/*** BROWSING ***/
typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUcs2String           *folderName;                    /* Name header. Equals NULL if not name header is present */
} CsrBtMapsSetBackFolderInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
} CsrBtMapsSetBackFolderRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
} CsrBtMapsSetRootFolderInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
} CsrBtMapsSetRootFolderRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUcs2String           *folderName;                    /* Name header. Equals NULL if not name header is present */
} CsrBtMapsSetFolderInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
} CsrBtMapsSetFolderRes;


/*** GETTING ***/
typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUint16               maxListCount;                   /* maximum number of folders in the listing */
    CsrUint16               listStartOffset;                /* offset of where to start the listing */
} CsrBtMapsGetFolderListingHeaderInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrUint16               fullFolderListingSize;          /* number of bytes in the complete folder listing */
    CsrBool                 srmpOn;
} CsrBtMapsGetFolderListingHeaderRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUint16               obexResponsePacketLength;       /* Maximum allowed body length of the corresponding response */
} CsrBtMapsGetFolderListingInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
    CsrUint16               bodyLength;
    CsrUint8                *body;
    CsrBool                 srmpOn;
} CsrBtMapsGetFolderListingRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUcs2String           *folderName;                    /* null terminated name string of the folder from where the current message listing is to be retreived: NB: if NULL this means current folder */
    CsrUint16               maxListCount;                   /* maximum number of messages in the listing */
    CsrUint16               listStartOffset;                /* offset of where to start the listing */
    CsrUint8                maxSubjectLength;               /* maximum string length allowed of the subject field */
    CsrBtMapMesParms        parameterMask;                  /* bitmask of relevant parameters for the message listing. NB: a bit value of 1 means that the parameter should be present and a value of 0 means it should be filtered out */
    CsrBtMapMesTypes        filterMessageType;              /* bitmask specifying which message types to include/exclude in the listing. NB: a bit value of 1 means that the message type should be filtered and a value of 0 means that it should be present */
    CsrUtf8String          *filterPeriodBegin;             /* null terminated time string */
    CsrUtf8String          *filterPeriodEnd;               /* null terminated time string */
    CsrBtMapReadStatus      filterReadStatus;               /* bitmask specifying if filtering should be done on behalf of the read status */
    CsrUtf8String          *filterRecipient;               /* null terminated recipient string */
    CsrUtf8String          *filterOriginator;              /* null terminated originator string */
    CsrBtMapPriority        filterPriority;                 /* bitmask specifying which priority type to include in the listing */
} CsrBtMapsGetMessageListingHeaderInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtMapNewMessage      newMessages;                    /* specifies if there's unread messages on the MSE or not */
    CsrUtf8String          *mseTime;                       /* current time basis and UTC-offset. Null terminated time string */    
    CsrUint16               fullMessageListingSize;         /* number of bytes in the complete message listing */
    CsrBool                 srmpOn;
} CsrBtMapsGetMessageListingHeaderRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUint16               obexResponsePacketLength;       /* Maximum allowed body length of the corresponding response */
} CsrBtMapsGetMessageListingInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
    CsrUint16               bodyLength;
    CsrUint8                *body;
    CsrBool                 srmpOn;
} CsrBtMapsGetMessageListingRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUcs2String           *messageHandle;                 /* null terminated message handle string */
    CsrBtMapAttachment      attachment;                     /* bitmask specifying whether to include attachment or not */
    CsrBtMapCharset         charset;                        /* bitmask used to specifying trans-coding of the message */
    CsrBtMapFracReq         fractionRequest;                /* bitmask which fragment of the message to get _if any_ */
} CsrBtMapsGetMessageHeaderInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtMapFracDel         fractionDeliver;                /* bitmask specifying the fragment status of the message retrieved if sent by peer */
    CsrBool                 srmpOn;
} CsrBtMapsGetMessageHeaderRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUint16               obexResponsePacketLength;       /* Maximum allowed body length of the corresponding response */
} CsrBtMapsGetMessageInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
    CsrUint16               bodyLength;
    CsrUint8                *body;
    CsrBool                 srmpOn;
} CsrBtMapsGetMessageRes;


/*** PUTTING ***/
typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUcs2String           *messageHandle;                 /* null terminated message handle string */
    CsrBtMapStatusInd       statusIndicator;                /* specifies which status information to modify */
    CsrBtMapStatusVal       statusValue;                    /* specifies the new value of the status indication in question */
} CsrBtMapsSetMessageStatusInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
    CsrBool                 srmpOn;
} CsrBtMapsSetMessageStatusRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUcs2String           *folderName;                    /* null terminated string specifying the name of the folder where the message should be pushed */
    CsrBtMapTrans           transparent;                    /* specifies if the MSE should keep a copy of the message in the sent folder */
    CsrBtMapRetry           retry;                          /* specifies if the MSE should try to resent if first delivery to the network fails */
    CsrBtMapCharset         charset;                        /* used to specify the format of the content delivered */
} CsrBtMapsPushMessageHeaderInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrCharString           *messageHandle;                 /* null terminated string specifying message handle assigned by the MSE */
    CsrBool                 srmpOn;
} CsrBtMapsPushMessageHeaderRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBool                 finalFlag;
    CsrUint16               bodyLength;
    CsrUint8                *body;
} CsrBtMapsPushMessageInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
    CsrBool                 srmpOn;
    CsrCharString           *messageHandle;                 /* null terminated string specifying message handle assigned by the MSE */
} CsrBtMapsPushMessageRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
} CsrBtMapsUpdateInboxInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
    CsrBool                 srmpOn;
} CsrBtMapsUpdateInboxRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBool                 notificationStatus;             /* Boolean specifying if the notification service should be switched on or off */
} CsrBtMapsNotificationRegistrationInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response to the sync command */
    CsrBool                 srmpOn;
} CsrBtMapsNotificationRegistrationRes;


/*** EVENT NOTIFICATION ***/
typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
} CsrBtMapsEventNotificationReq;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrUint16               obexResponsePacketLength;       /* Maximum allowed body length of the corresponding response */
} CsrBtMapsEventNotificationInd;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrBtObexResponseCode   responseCode;                   /* OBEX response */
    CsrUint16               bodyLength;
    CsrUint8                *body;
} CsrBtMapsEventNotificationRes;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtMapsEventNotificationCfm;


/*** SECURITY ***/
typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             appHandle;                      /* application handle */
    CsrUint16               secLevel;                       /* outgoing security level */
} CsrBtMapsSecurityLevelReq;

typedef struct
{
    CsrBtMapsPrim           type;                           /* primitive type */
    CsrSchedQid             instanceId;                     /* id of the MAPS instance that generated this event */
    CsrBtResultCode         resultCode;
    CsrBtSupplier           resultSupplier;
} CsrBtMapsSecurityLevelCfm;


/*** MAPS Internal Qid registration ***/
typedef struct
{
   CsrBtMapsPrim            type;                            /* primitive type */
   CsrSchedQid              mapsInstanceId;                  /* application handle */
} CsrBtMapsRegisterQidReq;

#ifdef __cplusplus
}
#endif

#endif

