/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_maps_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_assert.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_private_lib.h"
#endif /* CSR_AMP_ENABLE */

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

static void csrBtObexMapsServerStart(MapsInstanceData *pInst);
static CsrBool csrBtMapsCheckTargetHeader(CsrUint8   *target, 
                                          CsrUint16  targetHeaderLength);
static CsrUint8 csrBtMapsServerConnectResponseHandler(MapsInstanceData      *pInst,
                                                      CsrBtObexResponseCode responseCode, 
                                                      CsrUcs2String         *description);
static CsrBool csrBtMapsGetNameHeader(MapsInstanceData *pInst, CsrUint8 *obexPacket, CsrUint16  obexPacketLength);
static CsrBool csrBtMapsGetTypeHeader(MapsInstanceData *pInst, CsrUint8 *obexPacket, CsrUint16  obexPacketLength);
static CsrBool csrBtMapsGetAppHeader(MapsInstanceData *pInst, CsrUint8 *obexPacket, CsrUint16  obexPacketLength);
static void csrBtMapsGetHeaderIndSend(MapsInstanceData *pInst);
static void csrBtMapsPutHeaderIndSend(MapsInstanceData *pInst);
static void csrBtMapsGetIndSend(MapsInstanceData *pInst, CsrUint16 obexResponseLength);
static void csrBtMapsResetNameHeader(MapsInstanceData *pInst);


#define TYPE_HEADER_GET_FOLDER_LISTING_TXT          "x-obex/folder-listing"
#define TYPE_HEADER_GET_MESSAGE_LISTING_TXT         "x-bt/MAP-msg-listing"
#define TYPE_HEADER_GET_MESSAGE_TXT                 "x-bt/message"
#define TYPE_HEADER_SET_MESSAGE_STATUS_TXT          "x-bt/messageStatus"
#define TYPE_HEADER_UPDATE_INBOX_TXT                "x-bt/MAP-messageUpdate"
#define TYPE_HEADER_NOTIFICATION_REGISTRATION_TXT   "x-bt/MAP-NotificationRegistration"
#define TYPE_HEADER_EVENT_NOTIFICATION_TXT          "x-bt/MAP-event-report"

#define CSR_BT_MAPS_TAG_HEADER_SIZE                     2

/* OBEX Server part of MAPS */
#define RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX      24
#define MAS_INSTANCE_ID_SERVICE_RECORD_INDEX            55
#define SUPPORTED_MESSAGE_TYPES_SERVICE_RECORD_INDEX    60
#define MAS_INSTANCE_NAME_LENGTH_INDEX                  65

#define MAS_INSTANCE_DEFAULT_NAME_LENGTH                0x0C

static const CsrUint8 csrBtMapsObexDefaultName[] =
{
    'M','A','P',' ','M','A','S','-','n','a','m','e'
};

static const CsrUint8 csrBtMapsObexMapsMasServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x03,             /* 3 bytes in total DataElSeq */
    0x19,0x11,0x32,        /* 2 byte UUID, Service class = Message Access Server */

    /* protocol descriptor list */
    0x09,0x00,0x04,        /* AttrId ProtocolDescriptorList */
    0x35,0x11,             /* 17 bytes in total DataElSeq */
    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,        /* 2 byte UUID, Protocol = L2CAP */

    0x35,0x05,             /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,        /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,             /* 1 byte UINT - server channel template value 0 - to be filled in by app */

    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19, 0x00, 0x08,      /* 2 byte UUID, Protocol = OBEX */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */

    /* profile descriptor list */
    0x09,0x00,0x09,        /* AttrId, ProfileDescriptorList */
    0x35,0x08,
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x34,        /* 2 byte UUID, Service class = Message Access Profile */
    0x09,0x01,0x00,        /* 2 byte uint, version = 100 */

    /* MAS instance ID */
    0x09,0x03,0x15,        /* AttrId - MAS instance ID */
    0x08,0x00,             /* 1 byte UINT - MAS instance ID template value 0 - to be filled in by app */

    /* Supported Message Types */
    0x09,0x03,0x16,        /* AttrId - Supported Message Types */
    0x08,0x00,             /* 1 byte UINT - Supported Message Types template value 0 - to be filled in by app */

    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name */
    0x25,MAS_INSTANCE_DEFAULT_NAME_LENGTH,             /* 12 bytes string */
    'M','A','P',' ','M','A','S','-','n','a','m','e'
};


/* Target header UUID defines */
#define MAPS_MAS_TARGET {0xbb,0x58,0x2b,0x40,0x42,0x0c,0x11,0xdb,0xb0,0xde,0x08,0x00,0x20,0x0c,0x9a,0x66}
#define MAPS_MNS_TARGET {0xbb,0x58,0x2b,0x41,0x42,0x0c,0x11,0xdb,0xb0,0xde,0x08,0x00,0x20,0x0c,0x9a,0x66}
#define MAPS_TARGET_LENGTH 0x10


/*** UPSTREAM PRIMITIVE SEND FUNCTIONS ***/
void CsrBtMapsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_MAPS_PRIM, msg);
}

static void csrBtMapsActivateCfmSend(CsrSchedQid appHandle, 
                                     CsrSchedQid instanceId, 
                                     CsrBtResultCode resultCode, 
                                     CsrBtSupplier resultSupplier)
{
    CsrBtMapsActivateCfm *pMsg = (CsrBtMapsActivateCfm*)CsrPmemAlloc(sizeof(CsrBtMapsActivateCfm));
    pMsg->type                 = CSR_BT_MAPS_ACTIVATE_CFM;
    pMsg->instanceId           = instanceId;
    pMsg->resultCode           = resultCode;
    pMsg->resultSupplier       = resultSupplier;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsDeactivateCfmSend(CsrSchedQid appHandle, 
                                       CsrSchedQid instanceId)
{
    CsrBtMapsDeactivateCfm *pMsg = (CsrBtMapsDeactivateCfm*)CsrPmemAlloc(sizeof(CsrBtMapsDeactivateCfm));
    pMsg->type                   = CSR_BT_MAPS_DEACTIVATE_CFM;
    pMsg->instanceId             = instanceId;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsConnectIndSend(CsrSchedQid          appHandle, 
                                    CsrSchedQid          instanceId,
                                    CsrBtDeviceAddr deviceAddr,
                                    CsrUint32       length,
                                    CsrUint32       count,
                                    CsrBtConnId     btConnId)
{
    CsrBtMapsConnectInd *pMsg = (CsrBtMapsConnectInd*)CsrPmemAlloc(sizeof(CsrBtMapsConnectInd));
    pMsg->type                = CSR_BT_MAPS_CONNECT_IND;
    pMsg->instanceId          = instanceId;
    pMsg->deviceAddr          = deviceAddr;
    pMsg->length              = length;
    pMsg->count               = count;
    pMsg->btConnId            = btConnId;

    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsDisconnectIndSend(CsrSchedQid           appHandle, 
                                       CsrSchedQid           instanceId,
                                       CsrBtReasonCode  reasonCode,
                                       CsrBtSupplier    reasonSupplier)
{
    CsrBtMapsDisconnectInd *pMsg = (CsrBtMapsDisconnectInd*)CsrPmemAlloc(sizeof(CsrBtMapsDisconnectInd));
    pMsg->type                   = CSR_BT_MAPS_DISCONNECT_IND;
    pMsg->instanceId             = instanceId;
    pMsg->reasonCode             = reasonSupplier;   
    pMsg->reasonSupplier         = reasonSupplier;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsSecurityLevelCfmSend(CsrSchedQid appHandle, 
                                          CsrSchedQid instanceId, 
                                          CsrBtResultCode resultCode, 
                                          CsrBtSupplier resultSupplier)
{
    CsrBtMapsSecurityLevelCfm *pMsg = (CsrBtMapsSecurityLevelCfm*)CsrPmemAlloc(sizeof(CsrBtMapsSecurityLevelCfm));
    pMsg->type                      = CSR_BT_MAPS_SECURITY_LEVEL_CFM;
    pMsg->instanceId                = instanceId;
    pMsg->resultCode                = resultCode;
    pMsg->resultSupplier            = resultSupplier;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsSetBackFolderIndSend(CsrSchedQid        appHandle, 
                                          CsrSchedQid        instanceId,
                                          CsrUcs2String *fName)
{
    CsrBtMapsSetBackFolderInd *pMsg = (CsrBtMapsSetBackFolderInd*)CsrPmemAlloc(sizeof(CsrBtMapsSetBackFolderInd));
    pMsg->type                      = CSR_BT_MAPS_SET_BACK_FOLDER_IND;
    pMsg->instanceId                = instanceId;
    pMsg->folderName                = fName;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsSetRootFolderIndSend(CsrSchedQid    appHandle, 
                                          CsrSchedQid    instanceId)
{
    CsrBtMapsSetRootFolderInd *pMsg = (CsrBtMapsSetRootFolderInd*)CsrPmemAlloc(sizeof(CsrBtMapsSetRootFolderInd));
    pMsg->type                      = CSR_BT_MAPS_SET_ROOT_FOLDER_IND;
    pMsg->instanceId                = instanceId;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsSetFolderIndSend(CsrSchedQid        appHandle, 
                                      CsrSchedQid        instanceId,
                                      CsrUcs2String *fName)
{
    CsrBtMapsSetFolderInd *pMsg = (CsrBtMapsSetFolderInd*)CsrPmemAlloc(sizeof(CsrBtMapsSetFolderInd));
    pMsg->type                  = CSR_BT_MAPS_SET_FOLDER_IND;
    pMsg->instanceId            = instanceId;
    pMsg->folderName            = fName;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsGetFolderListingHeaderIndSend(CsrSchedQid       appHandle, 
                                                   CsrSchedQid       instanceId,
                                                   CsrUint16    maxListCount, 
                                                   CsrUint16    listStartOffset)
{
    CsrBtMapsGetFolderListingHeaderInd *pMsg = (CsrBtMapsGetFolderListingHeaderInd*)CsrPmemAlloc(sizeof(CsrBtMapsGetFolderListingHeaderInd));
    pMsg->type                               = CSR_BT_MAPS_GET_FOLDER_LISTING_HEADER_IND;
    pMsg->instanceId                         = instanceId;
    pMsg->maxListCount                       = maxListCount;
    pMsg->listStartOffset                    = listStartOffset;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsGetFolderListingIndSend(CsrSchedQid     appHandle, 
                                             CsrSchedQid     instanceId,
                                             CsrUint16  maxObexResponseLength)
{
    CsrBtMapsGetFolderListingInd *pMsg = (CsrBtMapsGetFolderListingInd*)CsrPmemAlloc(sizeof(CsrBtMapsGetFolderListingInd));
    pMsg->type                         = CSR_BT_MAPS_GET_FOLDER_LISTING_IND;
    pMsg->instanceId                   = instanceId;
    pMsg->obexResponsePacketLength     = maxObexResponseLength;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsGetMessageListingHeaderIndSend(CsrSchedQid              appHandle, 
                                                    CsrSchedQid              instanceId,
                                                    CsrUcs2String       *folderName, 
                                                    CsrUint16           maxListCount, 
                                                    CsrUint16           listStartOffset,
                                                    CsrUint8            subjectLength,
                                                    CsrBtMapMesParms    parameterMask,
                                                    CsrBtMapMesTypes    filterMessageType,
                                                    CsrUtf8String           *filterPeriodBegin,
                                                    CsrUtf8String           *filterPeriodEnd,
                                                    CsrBtMapReadStatus  filterReadStatus,
                                                    CsrUtf8String           *filterRecipient,
                                                    CsrUtf8String           *filterOriginator,
                                                    CsrBtMapPriority    filterPriority)
{
    CsrBtMapsGetMessageListingHeaderInd *pMsg = (CsrBtMapsGetMessageListingHeaderInd*)CsrPmemAlloc(sizeof(CsrBtMapsGetMessageListingHeaderInd));
    pMsg->type                                = CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND;
    pMsg->instanceId                          = instanceId;
    pMsg->folderName                          = folderName;
    pMsg->maxListCount                        = maxListCount;
    pMsg->listStartOffset                     = listStartOffset;
    pMsg->maxSubjectLength                    = subjectLength;
    pMsg->parameterMask                       = parameterMask;
    pMsg->filterMessageType                   = filterMessageType;
    pMsg->filterPeriodBegin                   = filterPeriodBegin;
    pMsg->filterPeriodEnd                     = filterPeriodEnd;
    pMsg->filterReadStatus                    = filterReadStatus;
    pMsg->filterRecipient                     = filterRecipient;
    pMsg->filterOriginator                    = filterOriginator;
    pMsg->filterPriority                      = filterPriority;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsGetMessageListingIndSend(CsrSchedQid    appHandle, 
                                              CsrSchedQid    instanceId,
                                              CsrUint16 maxObexResponseLength)
{
    CsrBtMapsGetMessageListingInd *pMsg = (CsrBtMapsGetMessageListingInd*)CsrPmemAlloc(sizeof(CsrBtMapsGetMessageListingInd));
    pMsg->type                          = CSR_BT_MAPS_GET_MESSAGE_LISTING_IND;
    pMsg->instanceId                    = instanceId;
    pMsg->obexResponsePacketLength      = maxObexResponseLength;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsGetMessageHeaderIndSend(CsrSchedQid             appHandle, 
                                             CsrSchedQid             instanceId,
                                             CsrUcs2String      *messageHandle, 
                                             CsrBtMapAttachment attachment, 
                                             CsrBtMapCharset    charset, 
                                             CsrBtMapFracReq    fractionRequest)
{
    CsrBtMapsGetMessageHeaderInd *pMsg = (CsrBtMapsGetMessageHeaderInd*)CsrPmemAlloc(sizeof(CsrBtMapsGetMessageHeaderInd));
    pMsg->type                         = CSR_BT_MAPS_GET_MESSAGE_HEADER_IND;
    pMsg->instanceId                   = instanceId;
    pMsg->messageHandle                = messageHandle;
    pMsg->attachment                   = attachment;
    pMsg->charset                      = charset;
    pMsg->fractionRequest              = fractionRequest;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsGetMessageIndSend(CsrSchedQid       appHandle, 
                                       CsrSchedQid       instanceId,
                                       CsrUint16    maxObexResponseLength)
{
    CsrBtMapsGetMessageInd *pMsg   = (CsrBtMapsGetMessageInd*)CsrPmemAlloc(sizeof(CsrBtMapsGetMessageInd));
    pMsg->type                     = CSR_BT_MAPS_GET_MESSAGE_IND;
    pMsg->instanceId               = instanceId;
    pMsg->obexResponsePacketLength = maxObexResponseLength;
    CsrBtMapsMessagePut(appHandle, pMsg);
}

static void csrBtMapsSetMessageStatusIndSend(MapsInstanceData *pInst)
{
    CsrBtMapsSetMessageStatusInd *pMsg = (CsrBtMapsSetMessageStatusInd*)CsrPmemAlloc(sizeof(CsrBtMapsSetMessageStatusInd));
    pMsg->type                    = CSR_BT_MAPS_SET_MESSAGE_STATUS_IND;
    pMsg->instanceId              = pInst->mapsInstanceId;
    pMsg->messageHandle           = pInst->pNameHeader; 
    pMsg->statusIndicator         = pInst->statusIndicator;
    pMsg->statusValue             = pInst->statusValue;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
    pInst->pNameHeader = NULL;
    CsrPmemFree(pInst->bodyFromPutInd);
    pInst->bodyFromPutInd = NULL;
    pInst->bodyFromPutIndLength = 0;
}

static void csrBtMapsPushMessageHeaderIndSend(MapsInstanceData *pInst)
{
    CsrBtMapsPushMessageHeaderInd *pMsg = (CsrBtMapsPushMessageHeaderInd*)CsrPmemAlloc(sizeof(CsrBtMapsPushMessageHeaderInd));
    pMsg->type                          = CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND;
    pMsg->instanceId                    = pInst->mapsInstanceId;
    pMsg->folderName                    = pInst->pNameHeader;
    pMsg->transparent                   = pInst->transparent;
    pMsg->retry                         = pInst->retry;
    pMsg->charset                       = pInst->charset;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
    pInst->pNameHeader = NULL;
}

static void csrBtMapsPushMessageIndSend(MapsInstanceData *pInst)
{
    CsrBtMapsPushMessageInd *pMsg = (CsrBtMapsPushMessageInd*)CsrPmemAlloc(sizeof(CsrBtMapsPushMessageInd));
    pMsg->type                    = CSR_BT_MAPS_PUSH_MESSAGE_IND;
    pMsg->instanceId              = pInst->mapsInstanceId;
    pMsg->finalFlag               = pInst->finalBitFromPutInd;
    pMsg->bodyLength              = pInst->bodyFromPutIndLength;
    pMsg->body                    = pInst->bodyFromPutInd;
    pInst->bodyFromPutInd         = NULL;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtMapsUpdateInboxIndSend(MapsInstanceData *pInst)
{
    CsrBtMapsUpdateInboxInd *pMsg = (CsrBtMapsUpdateInboxInd*)CsrPmemAlloc(sizeof(CsrBtMapsUpdateInboxInd));
    pMsg->type                    = CSR_BT_MAPS_UPDATE_INBOX_IND;
    pMsg->instanceId              = pInst->mapsInstanceId;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
    CsrPmemFree(pInst->bodyFromPutInd);
    pInst->bodyFromPutInd = NULL;
}

static void csrBtMapsNotificationRegistrationIndSend(MapsInstanceData *pInst)
{
    CsrBtMapsNotificationRegistrationInd *pMsg = (CsrBtMapsNotificationRegistrationInd*)CsrPmemAlloc(sizeof(CsrBtMapsNotificationRegistrationInd));
    pMsg->type                                 = CSR_BT_MAPS_NOTIFICATION_REGISTRATION_IND;
    pMsg->instanceId                           = pInst->mapsInstanceId;
    pMsg->notificationStatus                   = pInst->notificationStatus;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
    CsrPmemFree(pInst->bodyFromPutInd);
    pInst->bodyFromPutInd = NULL;
}

static void csrBtMapsNotificationRegistrationCompleteIndSend(MapsInstanceData   *pInst, 
                                                             CsrBtDeviceAddr    deviceAddr,
                                                             CsrBtResultCode    resultCode, 
                                                             CsrBtSupplier      resultSupplier)
{
    CsrBtMapsNotificationRegistrationCompleteInd *pMsg = 
        (CsrBtMapsNotificationRegistrationCompleteInd*)CsrPmemAlloc(sizeof(CsrBtMapsNotificationRegistrationCompleteInd));
    pMsg->type                                         = CSR_BT_MAPS_NOTIFICATION_REGISTRATION_COMPLETE_IND;
    pMsg->instanceId                                   = pInst->mapsInstanceId;
    pMsg->deviceAddr                                   = deviceAddr;
    pMsg->resultCode                                   = resultCode;
    pMsg->resultSupplier                               = resultSupplier;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtMapsEventNotificationIndSend(MapsInstanceData *pInst, CsrUint16 packetLength)
{
    CsrBtMapsEventNotificationInd *pMsg = (CsrBtMapsEventNotificationInd*)CsrPmemAlloc(sizeof(CsrBtMapsEventNotificationInd));
    pMsg->type                          = CSR_BT_MAPS_EVENT_NOTIFICATION_IND;
    pMsg->instanceId                    = pInst->mapsInstanceId;
    pMsg->obexResponsePacketLength      = packetLength;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtMapsEventNotificationCfmSend(MapsInstanceData  *pInst, 
                                              CsrBtResultCode   resultCode, 
                                              CsrBtSupplier     resultSupplier)
{
    CsrBtMapsEventNotificationCfm *pMsg = (CsrBtMapsEventNotificationCfm*)CsrPmemAlloc(sizeof(CsrBtMapsEventNotificationCfm));
    pMsg->type                          = CSR_BT_MAPS_EVENT_NOTIFICATION_CFM;
    pMsg->instanceId                    = pInst->mapsInstanceId;
    pMsg->resultCode                    = resultCode;
    pMsg->resultSupplier                = resultSupplier;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
}

/*** UPSTREAM PRIMITIVE RECEIVE FUNCTIONS ***/
static void csrBtMapsServerActivateCfmHandler(void            *instData,
                                              CsrUint8   serverChannel,
                                              psm_t           psm,
                                              CsrBtResultCode resultCode,
                                              CsrBtSupplier   resultSupplier)
{
    MapsInstanceData *pInst = (MapsInstanceData *) instData;
    
    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS && 
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* Server is activated */
        pInst->serverChannel    = serverChannel;
        ObexUtilServConnectableStart(pInst->obexServerInst, 
                                     CSR_BT_MAPS_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->maxObexPacketSize);

        csrBtMapsActivateCfmSend(pInst->appHandle, pInst->mapsInstanceId, CSR_BT_RESULT_CODE_OBEX_SUCCESS, CSR_BT_SUPPLIER_OBEX_PROFILES);
        CsrPmemFree(pInst->masInstName);
        pInst->masInstName = NULL;
    }
    else
    { /* Try to activate again */
        csrBtObexMapsServerStart(pInst);
    }
    CSR_UNUSED(psm);
}

static void CsrBtMapsDeactivateCfmHandler(void          *instData, 
                                          CsrUint8 serverChannel,
                                          psm_t         psm)
{
    MapsInstanceData *pInst = (MapsInstanceData *) instData;

    pInst->serverChannel = serverChannel;
    csrBtMapsDeactivateCfmSend(pInst->appHandle, pInst->mapsInstanceId);
    CSR_UNUSED(psm);
}

static void csrBtMapsClientAbortIndHandler(void                  *instData,
                                           CsrBtObexResponseCode responseCode,
                                           CsrUint8            *obexPacket,
                                           CsrUint16             obexPacketLength)
{
    MapsInstanceData *pInst = (MapsInstanceData *) instData;

    csrBtMapsEventNotificationCfmSend(pInst, 
                                      CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, 
                                      CSR_BT_SUPPLIER_OBEX_PROFILES);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtMapsServerAbortIndHandler(void                    *instData,
                                           CsrUint16               descriptionOffset,
                                           CsrUint16               descriptionLength,
                                           CsrUint16               obexPacketLength,
                                           CsrUint8              *obexPacket)
{
    MapsInstanceData    *pInst = (MapsInstanceData *)instData;
    CsrBtMapsAbortInd   *pMsg;    
    
    if (descriptionOffset == 0)
    { /* An OBEX Descriptor header is not included, CsrPmemFree obexPacket */
        CsrPmemFree(obexPacket);
        obexPacket          = NULL;
        obexPacketLength    = 0;     
    }
    pMsg = pnew(CsrBtMapsAbortInd);
    
    pMsg->type                = CSR_BT_MAPS_ABORT_IND;
    pMsg->instanceId          = pInst->mapsInstanceId;
    pMsg->descriptionOffset   = descriptionOffset;
    pMsg->descriptionLength   = descriptionLength;
    pMsg->payload             = obexPacket;
    pMsg->payloadLength       = obexPacketLength;
    CsrBtMapsMessagePut(pInst->appHandle, pMsg);
    csrBtMapsResetNameHeader(pInst);
}

static void csrBtMapsServerConnectIndHandler(void                    *instData,
                                             CsrBtDeviceAddr         deviceAddr,
                                             CsrBtConnId              cid,
                                             CsrUint16               maxPeerObexPacketLength,
                                             CsrUint16               obexResponsePacketLength,
                                             CsrUint32               length,
                                             CsrUint32               count,
                                             CsrUint16               targetHeaderLength,
                                             CsrUint8                *target,
                                             CsrBool                 authenticated,
                                             CsrBool                 challenged,
                                             CsrUint16               obexPacketLength,
                                             CsrUint8                *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    
    if (csrBtMapsCheckTargetHeader(target, targetHeaderLength))
    {   
        pInst->deviceAddr = deviceAddr;
        pInst->srvConnected = TRUE;
        csrBtMapsConnectIndSend(pInst->appHandle,
                                pInst->mapsInstanceId,
                                deviceAddr,
                                length,
                                count,
                                cid);
#ifdef CSR_AMP_ENABLE
        pInst->masterBtConnId = cid;
#endif /* CSR_AMP_ENABLE */
    }
    else
    {   
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target missing");
        csrBtMapsServerConnectResponseHandler(pInst, CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                              description);
    }
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
    CSR_UNUSED(maxPeerObexPacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
}

static void csrBtMapsServerDisconnectIndHandler(void                *instData,
                                                CsrBtDeviceAddr     deviceAddr,
                                                CsrBtReasonCode     reasonCode,
                                                CsrBtSupplier       reasonSupplier,
                                                CsrUint16           obexPacketLength,
                                                CsrUint8            *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    
    pInst->srvConnected = FALSE;
    
    if (pInst->cliState != CSR_BT_MAPS_CLI_NULL_STATE)
    {
        ObexUtilDisconnectRequest(pInst->obexClientInst, TRUE, NULL);
    }
    csrBtMapsResetNameHeader(pInst);
    
    csrBtMapsDisconnectIndSend(pInst->appHandle, 
                               pInst->mapsInstanceId,
                               reasonCode,
                               reasonSupplier);

#ifdef CSR_AMP_ENABLE
    pInst->masterBtConnId = 0;
#endif /* CSR_AMP_ENABLE */    
    
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsObexGetCollectHandler(void       *instData,
                                           CsrUint16  obexResponsePacketLength,
                                           CsrBool    authenticated,
                                           CsrBool    finalBitSet,
                                           CsrUint16  obexPacketLength,
                                           CsrUint8 *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    CsrBool          valid = TRUE;

    valid &= csrBtMapsGetTypeHeader(pInst, obexPacket, obexPacketLength);
    valid &= csrBtMapsGetAppHeader(pInst, obexPacket, obexPacketLength);
    if (pInst->typeHeader != TYPE_GET_FOLDER_LISTING)
    {
        valid &= csrBtMapsGetNameHeader(pInst, obexPacket, obexPacketLength);
    }

    if (!valid)
    {
        ObexUtilServGetResponse(pInst->obexServerInst, 
                                CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, 
                                NULL, 
                                0,
                                CSR_BT_OBEX_SRMP_INVALID);
        return;
    }
    if (finalBitSet)
    {
        csrBtMapsGetHeaderIndSend(pInst);
    }
    else
    {
        CsrUint8 headerPriorityTable[6];
        CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);

        ObexUtilServGetWithHeaderResponse(pInst->obexServerInst, 
                                          CSR_BT_OBEX_CONTINUE_RESPONSE_CODE,
                                          NULL, 
                                          headerPriorityTable, 
                                          CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX, 
                                          0,
                                          NULL, 
                                          NULL, 
                                          0, 
                                          NULL, 
                                          0, 
                                          NULL, 
                                          NULL, 
                                          0,
                                          CSR_BT_OBEX_SRMP_INVALID); 
    }
    
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

static void CsrBtMapsPushMessageNextIndHandler(void                    *instData,
                                               CsrUint16               obexResponsePacketLength,
                                               CsrBool                 authenticated,
                                               CsrBool                 finalBitSet,
                                               CsrUint16               bodyLength,
                                               CsrUint16               bodyOffset,
                                               CsrUint16               obexPacketLength,
                                               CsrUint8              *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;

    pInst->bodyFromPutIndLength = bodyLength;
    pInst->bodyFromPutInd = CsrPmemAlloc(bodyLength);
    CsrMemCpy(pInst->bodyFromPutInd, obexPacket + bodyOffset, bodyLength);
    pInst->finalBitFromPutInd = finalBitSet;
    csrBtMapsPushMessageIndSend(pInst);
    
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsServerNextPutIndHandler(void       *instData,
                                         CsrUint16  obexResponsePacketLength,
                                         CsrBool    authenticated,
                                         CsrBool    finalBitSet,
                                         CsrUint16  bodyLength,
                                         CsrUint16  bodyOffset,
                                         CsrUint16  obexPacketLength,
                                         CsrUint8 *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;

    if (bodyLength)
    {
        if ((pInst->bodyFromPutIndLength == 0) || (pInst->bodyFromPutInd == NULL))
        {
            CsrPmemFree(pInst->bodyFromPutInd);
            pInst->bodyFromPutIndLength = bodyLength;
            pInst->bodyFromPutInd = CsrPmemAlloc(bodyLength + 1);
            CsrMemCpy(pInst->bodyFromPutInd, obexPacket + bodyOffset, bodyLength); 
            pInst->bodyFromPutInd[bodyLength] = '\0';
        }
        else
        {
            CsrUint8 *tmpPutInd;
            CsrUint16 tmpLength = pInst->bodyFromPutIndLength + bodyLength;
            tmpPutInd = CsrPmemAlloc(tmpLength + 1);
            
            CsrMemCpy(tmpPutInd, pInst->bodyFromPutInd, pInst->bodyFromPutIndLength);
            CsrMemCpy(&tmpPutInd[pInst->bodyFromPutIndLength], obexPacket + bodyOffset, bodyLength);
            pInst->bodyFromPutIndLength = tmpLength;
            CsrPmemFree(pInst->bodyFromPutInd);
            pInst->bodyFromPutInd = tmpPutInd;
            pInst->bodyFromPutInd[pInst->bodyFromPutIndLength] = '\0';
        }
    }

    pInst->finalBitFromPutInd = finalBitSet;

    if (finalBitSet)
    {
        csrBtMapsPutHeaderIndSend(pInst);
    }
    else
    {
        ObexUtilServPutResponse(pInst->obexServerInst, 
                                CSR_BT_OBEX_CONTINUE_RESPONSE_CODE,
                                CSR_BT_OBEX_SRMP_INVALID);
    }
   
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsServerPutIndHandler(void       *instData,
                                         CsrUint16  obexResponsePacketLength,
                                         CsrBool    authenticated,
                                         CsrBool    finalBitSet,
                                         CsrUint16  bodyLength,
                                         CsrUint16  bodyOffset,
                                         CsrUint16  obexPacketLength,
                                         CsrUint8 *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    CsrBool          valid = TRUE;

    pInst->nameHeader = NAME_NONE;
    pInst->typeHeader = TYPE_NONE;
    pInst->appHeader  = APP_NONE;

    valid &= csrBtMapsGetTypeHeader(pInst, obexPacket, obexPacketLength);
    if (pInst->typeHeader != TYPE_UPDATE_INBOX) /* An Update Inbox message does not contain any application parameter header */
    {
        valid &= csrBtMapsGetAppHeader(pInst, obexPacket, obexPacketLength);
        if (pInst->typeHeader != TYPE_NOTIFICATION_REGISTRATION) /* An Set Notification Registration message does not contain any name header */
        {
            valid &= csrBtMapsGetNameHeader(pInst, obexPacket, obexPacketLength);
        }
    }

    if (!valid)
    {
        ObexUtilServPutResponse(pInst->obexServerInst, 
                                CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                CSR_BT_OBEX_SRMP_INVALID);
        return;
    }

    if (bodyLength && finalBitSet)
    {        
        CsrPmemFree(pInst->bodyFromPutInd);
        pInst->bodyFromPutIndLength = bodyLength;
        pInst->bodyFromPutInd = CsrPmemAlloc(bodyLength + 1);
        CsrMemCpy(pInst->bodyFromPutInd, obexPacket + bodyOffset, bodyLength); 
        pInst->bodyFromPutInd[bodyLength] = '\0';
    }

    pInst->finalBitFromPutInd = finalBitSet;
    if (finalBitSet)
    {
        csrBtMapsPutHeaderIndSend(pInst);
    }
    else
    {/* final flag not set: answer with "continue" and wait for the next PUT message */
        ObexUtilServSetPutContinueFunc(pInst->obexServerInst,csrBtMapsServerNextPutIndHandler);
        ObexUtilServPutResponse(pInst->obexServerInst, 
                                CSR_BT_OBEX_CONTINUE_RESPONSE_CODE,
                                CSR_BT_OBEX_SRMP_INVALID);
    }

    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsServerGetIndHandler(void       *instData,
                                         CsrUint16  obexResponsePacketLength,
                                         CsrBool    authenticated,
                                         CsrBool    finalBitSet,
                                         CsrUint16  obexPacketLength,
                                         CsrUint8 *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    CsrBool          valid = TRUE;

    pInst->nameHeader = NAME_NONE;
    pInst->typeHeader = TYPE_NONE;
    pInst->appHeader  = APP_NONE;
    pInst->maxObexResponseLength = obexResponsePacketLength;

    valid &= csrBtMapsGetTypeHeader(pInst, obexPacket, obexPacketLength);
    valid &= csrBtMapsGetAppHeader(pInst, obexPacket, obexPacketLength);
    if (pInst->typeHeader != TYPE_GET_FOLDER_LISTING)
    {
        valid &= csrBtMapsGetNameHeader(pInst, obexPacket, obexPacketLength);
    }

    if (!valid)
    {
        ObexUtilServGetResponse(pInst->obexServerInst, 
                                CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, 
                                NULL, 
                                0,
                                CSR_BT_OBEX_SRMP_INVALID);
        return;
    }

    if (finalBitSet)
    {
        csrBtMapsGetHeaderIndSend(pInst);
    }
    else /* finalBit not set */
    {
        CsrUint8 headerPriorityTable[6];
        CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);

        ObexUtilServSetGetContinueFunc(pInst->obexServerInst,
                                       csrBtMapsObexGetCollectHandler);

        ObexUtilServGetWithHeaderResponse(pInst->obexServerInst, 
                                          CSR_BT_OBEX_CONTINUE_RESPONSE_CODE,
                                          NULL, 
                                          headerPriorityTable, 
                                          CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX, 
                                          0,
                                          NULL, 
                                          NULL, 
                                          0, 
                                          NULL, 
                                          0, 
                                          NULL, 
                                          NULL, 
                                          0,
                                          CSR_BT_OBEX_SRMP_INVALID); 
    }
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsServerGetNextIndHandler(void       *instData,
                                             CsrUint16  obexResponsePacketLength,
                                             CsrBool    authenticated,
                                             CsrBool    finalBitSet,
                                             CsrUint16  obexPacketLength,
                                             CsrUint8 *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;

    pInst->maxObexResponseLength = obexResponsePacketLength;
    pInst->responseAppHeaderLength = 0;
    pInst->responseAppHeader = NULL;

    csrBtMapsGetIndSend(pInst, obexResponsePacketLength);
    
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsServerSetPathIndHandler(void       *instData,
                                             CsrUint16  obexResponsePacketLength,
                                             CsrUint8   flags,
                                             CsrUint8   constants,
                                             CsrBool    authenticated,
                                             CsrUint16  obexPacketLength,
                                             CsrUint8 *obexPacket)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    CsrUint16        nameOffset = 0;
    CsrUint16        nameLength = 0;
    CsrUcs2String    *folderName;

    nameOffset = CsrBtObexHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, CSR_BT_OBEX_NAME_HEADER, &nameLength);
    
    if (nameOffset && (nameLength > CSR_BT_OBEX_HEADER_LENGTH))
    {
        nameLength -= CSR_BT_OBEX_HEADER_LENGTH;
        folderName = CsrPmemAlloc(nameLength);
        CsrMemCpy(folderName, obexPacket + (nameOffset + CSR_BT_OBEX_HEADER_LENGTH), nameLength);
    }
    else
    {
        folderName = NULL;
    }
    if (flags & 0x01) /* if bit0 is set */
    {
        csrBtMapsSetBackFolderIndSend(pInst->appHandle, pInst->mapsInstanceId, folderName);
    }
    else if (flags & 0x02)
    {
        if (folderName != NULL)
        {
            csrBtMapsSetFolderIndSend(pInst->appHandle, pInst->mapsInstanceId, folderName);
        }
        else
        {
            csrBtMapsSetRootFolderIndSend(pInst->appHandle, pInst->mapsInstanceId);
        }
    }
    else
    {
        CsrUint8  headerPriorityTable[3] = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

        ObexUtilServSetpathResponse(pInst->obexServerInst, CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, NULL, headerPriorityTable, NULL, NULL, 0, NULL);
        CsrPmemFree(folderName);
    }
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(constants);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtMapsClientConnectResultHandler(void                      *instData,
                                                CsrBtResultCode           resultCode,
                                                CsrBtSupplier             resultSupplier,
                                                CsrBtDeviceAddr           deviceAddr,
                                                CsrBtConnId               cid,
                                                CsrUint16                 maxPeerObexPacketLength,
                                                CmnCsrBtLinkedListStruct  *sdpTag,
                                                CsrUint16                 obexPacketLength,
                                                CsrUint8                  *obexPacket)
{
    MapsInstanceData *pInst    = (MapsInstanceData *)instData;

    if((resultCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE) && (resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX))
    {
        pInst->cliState = CSR_BT_MAPS_CLI_IDLE_STATE;
#ifdef CSR_AMP_ENABLE
        /* Tell the AMPM that the slave channel is connected */
        pInst->slaveBtConnId = (CsrBtConnId)cid;
        CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 1, &pInst->slaveBtConnId);
#endif /* CSR_AMP_ENABLE */
    }
    else
    {
        pInst->cliState = CSR_BT_MAPS_CLI_NULL_STATE;
    }

    if (pInst->srvConnected)
    {
        csrBtMapsNotificationRegistrationCompleteIndSend(pInst, deviceAddr, resultCode, resultSupplier);
    }
    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);

    CSR_UNUSED(cid);
    CSR_UNUSED(maxPeerObexPacketLength);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsClientDisconnectIndHandler(void            *instData,
                                                CsrBtReasonCode reasonCode,
                                                CsrBtSupplier   reasonSupplier,
                                                CsrUint8        *obexPacket,
                                                CsrUint16       obexPacketLength)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;

    switch (pInst->cliState)
    {
        case CSR_BT_MAPS_CLI_NULL_STATE:
            {
                /* Ignore */
                break;
            }
        case CSR_BT_MAPS_CLI_IDLE_STATE:
            {
                pInst->cliState = CSR_BT_MAPS_CLI_NULL_STATE;
                pInst->notificationStatus = FALSE;
                csrBtMapsNotificationRegistrationIndSend(pInst);
                break;
            }
        case CSR_BT_MAPS_CLI_PENDING_APP_STATE:
            {
                pInst->cliState = CSR_BT_MAPS_CLI_NULL_STATE;
                pInst->notificationStatus = FALSE;
                break;
            }
        case CSR_BT_MAPS_CLI_PENDING_PEER_STATE:
            {
                pInst->cliState = CSR_BT_MAPS_CLI_NULL_STATE;
                csrBtMapsEventNotificationCfmSend(pInst, 
                                                  CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, 
                                                  CSR_BT_SUPPLIER_OBEX_PROFILES);
                pInst->notificationStatus = FALSE;
                csrBtMapsNotificationRegistrationIndSend(pInst);
                break;
            }
        default:
            {
                CSR_BT_GENERAL_EXCEPTION("MAPS - Client disconnect function",0,0,"Unknown Client state");
                break;
            }
    }
#ifdef CSR_AMP_ENABLE
    /* Tell the AMPM that the slave channel is disconnected */
    pInst->slaveBtConnId = 0;
    CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 0, NULL);
#endif /* CSR_AMP_ENABLE */
    CSR_UNUSED(reasonCode);
    CSR_UNUSED(reasonSupplier);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtMapsEventNotificationContinueHandler(void      *instData,
                                                      CsrUint16 obexPacketLength)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    
    csrBtMapsEventNotificationIndSend(pInst, obexPacketLength);
    pInst->cliState = CSR_BT_MAPS_CLI_PENDING_APP_STATE;
}

static void csrBtMapsEventNotificationResultHandler(void                  *instData,
                                                    CsrBtObexResponseCode responseCode,
                                                    CsrUint8            *obexPacket,
                                                    CsrUint16             obexPacketLength)
{
    MapsInstanceData *pInst = (MapsInstanceData *)instData;
    
    csrBtMapsEventNotificationCfmSend(pInst, responseCode, CSR_BT_SUPPLIER_IRDA_OBEX);
    pInst->cliState = CSR_BT_MAPS_CLI_IDLE_STATE;

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

/*** SERVER HELPING FUNCTIONS ***/
static void csrBtMapsGetIndSend(MapsInstanceData *pInst, CsrUint16 obexResponseLength)
{
    if (pInst->typeHeader == TYPE_GET_FOLDER_LISTING)
    {
        csrBtMapsGetFolderListingIndSend(pInst->appHandle, 
                                         pInst->mapsInstanceId, 
                                         obexResponseLength);
    }
    else if (pInst->typeHeader == TYPE_GET_MESSAGE_LISTING)
    {
        csrBtMapsGetMessageListingIndSend(pInst->appHandle, 
                                          pInst->mapsInstanceId, 
                                          obexResponseLength);
    }
    else if (pInst->typeHeader == TYPE_GET_MESSAGE)
    {
        csrBtMapsGetMessageIndSend(pInst->appHandle, 
                                   pInst->mapsInstanceId, 
                                   obexResponseLength);
    }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE                
    else 
    {
        CsrGeneralException("MAPS - csrBtMapsGetIndSend", 0, 0, "Wrong Type header\n");
    }
#endif
 
}

static void csrBtMapsGetHeaderIndSend(MapsInstanceData *pInst)
{
    if (pInst->typeHeader == TYPE_GET_FOLDER_LISTING)
    {
        csrBtMapsGetFolderListingHeaderIndSend(pInst->appHandle, 
                                               pInst->mapsInstanceId, 
                                               pInst->maxListCount, 
                                               pInst->listStartOffset);
    }
    else if (pInst->typeHeader == TYPE_GET_MESSAGE_LISTING &&
             pInst->nameHeader == NAME_FOUND)
    {
        csrBtMapsGetMessageListingHeaderIndSend(pInst->appHandle, 
                                                pInst->mapsInstanceId, 
                                                pInst->pNameHeader, 
                                                pInst->maxListCount, 
                                                pInst->listStartOffset,
                                                pInst->subjectLength,
                                                pInst->parameterMask,
                                                pInst->filterMessageType,
                                                pInst->filterPeriodBegin,
                                                pInst->filterPeriodEnd,
                                                pInst->filterReadStatus,
                                                pInst->filterRecipient,
                                                pInst->filterOriginator,
                                                pInst->filterPriority);
        pInst->pNameHeader = NULL;
    }
    else if (pInst->typeHeader == TYPE_GET_MESSAGE &&
             pInst->nameHeader == NAME_FOUND)
    {
        csrBtMapsGetMessageHeaderIndSend(pInst->appHandle, 
                                         pInst->mapsInstanceId, 
                                         pInst->pNameHeader, 
                                         pInst->attachment, 
                                         pInst->charset, 
                                         pInst->fractionRequest);
        pInst->pNameHeader = NULL;
    }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE                
    else 
    {
        CsrGeneralException("MAPS - csrBtMapsGetHeaderIndSend", 0, 0, "Wrong Type header\n");
    }
#endif
 
}

static void csrBtMapsPutHeaderIndSend(MapsInstanceData *pInst)
{
    if (pInst->typeHeader == TYPE_SET_MESSAGE_STATUS &&
        pInst->nameHeader == NAME_FOUND &&
        pInst->appHeader == APP_FOUND)
    {
        csrBtMapsSetMessageStatusIndSend(pInst);
    }
    else if (pInst->typeHeader == TYPE_GET_MESSAGE &&
             pInst->nameHeader == NAME_FOUND &&
             pInst->appHeader == APP_FOUND)
    {
        csrBtMapsPushMessageHeaderIndSend(pInst);
    }
    else if (pInst->typeHeader == TYPE_UPDATE_INBOX)
    {
        csrBtMapsUpdateInboxIndSend(pInst);
    }
    else if (pInst->typeHeader == TYPE_NOTIFICATION_REGISTRATION)
    {
        csrBtMapsNotificationRegistrationIndSend(pInst);
    }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE                
    else 
    {
        CsrGeneralException("MAPS - csrBtMapsPutHeaderIndSend", 0, 0, "Wrong Type header\n");
    }
#endif
}

void csrBtMapsResetAppHeaderTags(MapsInstanceData *pInst)
{
    pInst->maxListCount = CSR_BT_MAPS_DEFAULT_MAX_LIST_COUNT;
    pInst->listStartOffset = 0;
    pInst->filterMessageType = 0;
    pInst->filterPeriodBegin = NULL;
    pInst->filterPeriodEnd = NULL;
    pInst->filterRecipient = NULL;
    pInst->filterOriginator = NULL;
    pInst->filterPriority = 0;
    pInst->attachment = 0;
    pInst->transparent = 0;
    pInst->retry = 0;
    pInst->newMessage = 0;
    pInst->notificationStatus = FALSE;
    pInst->masInstanceId = 0;
    pInst->parameterMask = 0;
    pInst->folderListingSize = 0;
    pInst->messageListingSize = 0;
    pInst->subjectLength = 0;
    pInst->charset = 0;
    pInst->fractionRequest = 0;
    pInst->fractionDeliver = 0;
    pInst->statusIndicator = 0;
    pInst->statusValue = 0;
    if (pInst->mseTime)
    {
        pInst->mseTime[0] = '\0';
    }
    pInst->mseTime = NULL;    
}

static void csrBtMapsResetNameHeader(MapsInstanceData *pInst)
{
    if (pInst->pNameHeader)
    {
        CsrPmemFree(pInst->pNameHeader);
        pInst->pNameHeader = NULL;
    }
}

static void csrBtMapsSaveFirstReceivedMessageHandle(MapsInstanceData *pInst, CsrCharString *messageHandle)
{
    if (messageHandle)
    {
        if (pInst->pNameHeader == NULL)
        {
            pInst->pNameHeader = CsrUtf82Ucs2ByteString((CsrUint8 *)messageHandle);
        }
        CsrPmemFree(messageHandle);
    } 
}

static CsrBool csrBtMapsGetNameHeader(MapsInstanceData *pInst, CsrUint8 *obexPacket, CsrUint16  obexPacketLength)
{
    CsrUint16   nameOffset = 0;
    CsrUint16   length = 0;
    
    nameOffset = CsrBtObexHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, CSR_BT_OBEX_NAME_HEADER, &length);
    
    if (pInst->nameHeader == NAME_NONE && nameOffset)
    {
        pInst->nameHeader = NAME_FOUND;
        csrBtMapsResetNameHeader(pInst);
        
        if (length > CSR_BT_OBEX_HEADER_LENGTH)
        {
            /* If last character in the folder name is not backslash, we need to insert a backslash */
            if (pInst->typeHeader == TYPE_GET_MESSAGE_LISTING &&
                obexPacket[nameOffset + length - CSR_BT_OBEX_HEADER_LENGTH] != 0x5C) /* 5C is the ASCII value for the backslash character */
            {
                length += 2;
            }
            
            pInst->pNameHeader = (CsrUcs2String *) CsrPmemAlloc(length - CSR_BT_OBEX_HEADER_LENGTH);
            CsrMemCpy(pInst->pNameHeader, obexPacket + nameOffset + CSR_BT_OBEX_HEADER_LENGTH, length - CSR_BT_OBEX_HEADER_LENGTH);
            if (pInst->typeHeader == TYPE_GET_MESSAGE_LISTING &&
                obexPacket[nameOffset + length - CSR_BT_OBEX_HEADER_LENGTH] != 0x5C) /* 5C is the ASCII value for the backslash character */
            {
                pInst->pNameHeader[length - CSR_BT_OBEX_HEADER_LENGTH - 3] = 0x5C; /* 5C is the ASCII value for the backslash character */
                pInst->pNameHeader[length - CSR_BT_OBEX_HEADER_LENGTH - 2] = 0x00; /* Last two bytes of a UCS2 encoded string must be 0x00 */
                pInst->pNameHeader[length - CSR_BT_OBEX_HEADER_LENGTH - 1] = 0x00; /* Last two bytes of a UCS2 encoded string must be 0x00 */
            }
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

static CsrBool csrBtMapsGetTypeHeader(MapsInstanceData *pInst, CsrUint8 *obexPacket, CsrUint16  obexPacketLength)
{
    CsrUint16   typeOffset = 0;
    CsrUint16   typeLength = 0;
    
    if (pInst->typeHeader == TYPE_NONE &&
        CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeOffset, &typeLength))
    { /* A type header is found */
        if  (obexPacketLength >= (typeOffset + sizeof(TYPE_HEADER_GET_FOLDER_LISTING_TXT)) &&
            (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)TYPE_HEADER_GET_FOLDER_LISTING_TXT, (CsrUint8)CsrStrLen((char *)TYPE_HEADER_GET_FOLDER_LISTING_TXT))))
        {
            pInst->typeHeader = TYPE_GET_FOLDER_LISTING;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(TYPE_HEADER_GET_MESSAGE_LISTING_TXT)) &&
                (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)TYPE_HEADER_GET_MESSAGE_LISTING_TXT, (CsrUint8)CsrStrLen((char *)TYPE_HEADER_GET_MESSAGE_LISTING_TXT))))
        {
            pInst->typeHeader = TYPE_GET_MESSAGE_LISTING;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(TYPE_HEADER_SET_MESSAGE_STATUS_TXT)) &&
                (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)TYPE_HEADER_SET_MESSAGE_STATUS_TXT, (CsrUint8)CsrStrLen((char *)TYPE_HEADER_SET_MESSAGE_STATUS_TXT))))
        {
            pInst->typeHeader = TYPE_SET_MESSAGE_STATUS;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(TYPE_HEADER_GET_MESSAGE_TXT)) &&
                (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)TYPE_HEADER_GET_MESSAGE_TXT, (CsrUint8)CsrStrLen((char *)TYPE_HEADER_GET_MESSAGE_TXT))))
        {
            pInst->typeHeader = TYPE_GET_MESSAGE;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(TYPE_HEADER_UPDATE_INBOX_TXT)) &&
                (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)TYPE_HEADER_UPDATE_INBOX_TXT, (CsrUint8)CsrStrLen((char *)TYPE_HEADER_UPDATE_INBOX_TXT))))
        {
            pInst->typeHeader = TYPE_UPDATE_INBOX;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(TYPE_HEADER_NOTIFICATION_REGISTRATION_TXT)) &&
                (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)TYPE_HEADER_NOTIFICATION_REGISTRATION_TXT, (CsrUint8)CsrStrLen((char *)TYPE_HEADER_NOTIFICATION_REGISTRATION_TXT))))
        {
            pInst->typeHeader = TYPE_NOTIFICATION_REGISTRATION;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

static CsrBool csrBtMapsGetAppHeader(MapsInstanceData *pInst, CsrUint8 *obexPacket, CsrUint16  obexPacketLength)
{
    CsrUint16   appParametersOffset = 0; 
    CsrUint16   appParametersLength = 0; 
    CsrUint16   readOffset = 0;

    if (pInst->appHeader == APP_NONE &&
        CsrBtObexGetAppParametersOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, 
                                        obexPacketLength, &appParametersOffset, &appParametersLength))
    { /* An application header is found */
        CsrUint8 tmpNotifStatus = pInst->notificationStatus;
        
        csrBtMapsResetAppHeaderTags(pInst);
        pInst->appHeader = APP_FOUND;
        /* Keep the notification status in case the CSR_BT_MAP_TAG_ID_NOTIFICATION_STATUS is not present */
        pInst->notificationStatus = tmpNotifStatus;
        
        if (appParametersLength)
        {
            readOffset = appParametersOffset;

            while (readOffset < (appParametersOffset + appParametersLength))
            {
                CsrUint8    tag = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                CsrUint16   length = 0;

                readOffset++; /* We just read the tag (one byte) */

                switch (tag)
                {
                    case CSR_BT_MAP_TAG_ID_MAX_LIST_COUNT:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->maxListCount = CSR_BT_OBEX_GET_WORD_AT(obexPacket, readOffset);
                            readOffset += 2;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_START_OFFSET:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->listStartOffset = CSR_BT_OBEX_GET_WORD_AT(obexPacket, readOffset);
                            readOffset += 2;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FILTER_MESSAGE_TYPE:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->filterMessageType = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FILTER_PERIOD_BEGIN:
                        {
                            length = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            
                            if (length)
                            {
                                pInst->filterPeriodBegin = CsrPmemAlloc(length + 1);
                                CsrMemCpy(pInst->filterPeriodBegin, obexPacket + readOffset, length); 
                                pInst->filterPeriodBegin[length] = '\0';
                                readOffset += length;
                            }
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FILTER_PERIOD_END:
                        {
                            length = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            
                            if (length)
                            {
                                pInst->filterPeriodEnd = CsrPmemAlloc(length + 1);
                                CsrMemCpy(pInst->filterPeriodEnd, obexPacket + readOffset, length); 
                                pInst->filterPeriodEnd[length] = '\0';
                                readOffset += length;
                            }
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FILTER_READ_STATUS:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->filterReadStatus = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FILTER_RECIPIENT:
                        {
                            length = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            
                            if (length)
                            {
                                pInst->filterRecipient = CsrPmemAlloc(length + 1);
                                CsrMemCpy(pInst->filterRecipient, obexPacket + readOffset, length); 
                                pInst->filterRecipient[length] = '\0';
                                readOffset += length;
                            }
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FILTER_ORIGINATOR:
                        {
                            length = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            
                            if (length)
                            {
                                pInst->filterOriginator = CsrPmemAlloc(length + 1);
                                CsrMemCpy(pInst->filterOriginator, obexPacket + readOffset, length); 
                                pInst->filterOriginator[length] = '\0';
                                readOffset += length;
                            }
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FILTER_PRIORITY:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->filterPriority = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_ATTACHMENT:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->attachment = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_TRANSPARENT:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->transparent = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_RETRY:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->retry = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_NEW_MESSAGE:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->newMessage = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_NOTIFICATION_STATUS:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->notificationStatus = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_MAS_INSTANCE_ID:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->masInstanceId = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_PARAMETER_MASK:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->parameterMask = CSR_BT_OBEX_GET_D_WORD_AT(obexPacket, readOffset);
                            readOffset += 4;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->folderListingSize = CSR_BT_OBEX_GET_WORD_AT(obexPacket, readOffset);
                            readOffset += 2;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_MESSAGES_LISTING_SIZE:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->messageListingSize = CSR_BT_OBEX_GET_WORD_AT(obexPacket, readOffset);
                            readOffset += 2;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_SUBJECT_LENGTH:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->subjectLength = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_CHARSET:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->charset = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FRACTION_REQUEST:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->fractionRequest = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_FRACTION_DELIVER:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->fractionDeliver = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_STATUS_INDICATOR:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->statusIndicator = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_STATUS_VALUE:
                        {
                            readOffset += 1; /* ignore length as this tag has a known length */
                            pInst->statusValue = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            break;
                        }
                    case CSR_BT_MAP_TAG_ID_MSE_TIME:
                        {
                            length = CSR_BT_OBEX_GET_BYTE_AT(obexPacket, readOffset);
                            readOffset += 1;
                            
                            if (length)
                            {
                                pInst->mseTime = CsrPmemAlloc(length);
                                CsrMemCpy(obexPacket + readOffset, pInst->mseTime, length); 
                                readOffset += length;
                            }
                            break;
                        }
                    default:
                        {
                            return FALSE;
                        }
                } /* End of switch(tag) */
            } /* End of while loop */
        }
    }
    
    return TRUE;
}
static CsrUint8 csrBtMapsServerConnectResponseHandler(MapsInstanceData      *pInst,
                                                      CsrBtObexResponseCode responseCode, 
                                                      CsrUcs2String         *description)
{
    CsrUint8  numOfHeaders                   = 0;
    CsrUint8  headerPriorityTable[1]         = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    CsrUint8  tempTarget[MAPS_TARGET_LENGTH] = MAPS_MAS_TARGET;
    CsrUint16 whoHeaderLength                = MAPS_TARGET_LENGTH;
    CsrUint8  *who                           = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
    responseCode                             = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    
    if (description)
    {
        numOfHeaders = 1;
    }
    CsrMemCpy(who, tempTarget, whoHeaderLength); 
    
    return (ObexUtilServConnectResponse(pInst->obexServerInst, 
                                        responseCode, 
                                        whoHeaderLength, 
                                        &who, 
                                        (CsrUint32)pInst->serverChannel, 
                                        numOfHeaders, 
                                        headerPriorityTable, 
                                        NULL, 
                                        &description, 
                                        0, 
                                        NULL,
                                        FALSE));
}

static CsrUint16 csrBtMapsFindServiceHandleIndexFromMasInstanceId(MapsInstanceData *pInst,
                                                                  CsrUint16 masInstanceId)
{
    CsrUint16 i;
    CsrUint16 retVal = 0xFFFF;
    if(pInst->masInstToServiceMap)
    {
        for(i=0; i < pInst->masInstToServiceMapLength; i++)
        {
            if(pInst->masInstToServiceMap[i].masInstanceId == masInstanceId)
            {
                retVal = pInst->masInstToServiceMap[i].serviceIdx;
                break;
            }
        }

        CsrPmemFree(pInst->masInstToServiceMap);
        pInst->masInstToServiceMap = NULL;
    }

    return retVal; 
}

static CsrBool csrBtMapsCheckTargetHeader(CsrUint8   *target, 
                                          CsrUint16  targetHeaderLength)
{
    if (target && (targetHeaderLength == MAPS_TARGET_LENGTH))
    {
        CsrUint8 tempTarget[MAPS_TARGET_LENGTH] = MAPS_MAS_TARGET;
    
        if (!CsrMemCmp(target, tempTarget, MAPS_TARGET_LENGTH))
        {
            return (TRUE);
        }
    }
    return (FALSE);       
}

static void csrBtObexMapsServerStart(MapsInstanceData *pInst)
{
    CsrUint8 *record, recordLength = (CsrUint8) sizeof(csrBtMapsObexMapsMasServiceRecord);

    recordLength = (CsrUint8)(recordLength + ((CsrUint8)(pInst->masInstNameLen)) - MAS_INSTANCE_DEFAULT_NAME_LENGTH);
    /* Prepare to register Service Record */
    record = CsrPmemAlloc(recordLength);
    CsrMemCpy(record, csrBtMapsObexMapsMasServiceRecord, recordLength);

    record[MAS_INSTANCE_ID_SERVICE_RECORD_INDEX] = pInst->masInstId;
    record[SUPPORTED_MESSAGE_TYPES_SERVICE_RECORD_INDEX] = (CsrUint8)pInst->supportedMessages;
    record[MAS_INSTANCE_NAME_LENGTH_INDEX] = (CsrUint8)pInst->masInstNameLen;
    CsrMemCpy(&record[MAS_INSTANCE_NAME_LENGTH_INDEX+1], pInst->masInstName ,pInst->masInstNameLen);

    ObexUtilServActivateStart(pInst->obexServerInst,
                              pInst->secInLevel,
                              CSR_BT_OBEX_MESSAGE_ACCESS_SERVER_UUID,
                              CSR_BT_MAPS_LP_SUPERVISION_TIMEOUT,
                              pInst->serverChannel,
                              L2CA_PSM_INVALID,
                              recordLength,
                              RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              &record,
                              CSR_BT_OBJECT_TRANSFER_COD,
                              csrBtMapsServerConnectIndHandler,
                              csrBtMapsServerPutIndHandler,
                              csrBtMapsServerGetIndHandler,
                              csrBtMapsServerSetPathIndHandler,
                              csrBtMapsServerAbortIndHandler,
                              csrBtMapsServerDisconnectIndHandler,
                              NULL, /* Authentication is not used for MAP */
                              csrBtMapsServerActivateCfmHandler,
                              NULL,
                              pInst->maxObexPacketSize,
                              pInst->windowSize,
                              pInst->localSrmEnable);
}


/*** DOWNSTREAM PRIMITIVE HANDLERS ***/
CsrUint8 CsrBtMapsGetInstanceIdsReqHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsGetInstanceIdsReq  *prim_req;
    CsrBtMapsGetInstanceIdsCfm  *prim_cfm;
    CsrBtMapsInstancePool       *ptr;
    CsrUint8                    offset;

    if (pInst->mapsInstanceId == CSR_BT_MAPS_IFACEQUEUE)
    {
        prim_req=(CsrBtMapsGetInstanceIdsReq *) msg;

        prim_cfm = CsrPmemAlloc(sizeof(CsrBtMapsGetInstanceIdsCfm));

        prim_cfm->type = CSR_BT_MAPS_GET_INSTANCE_IDS_CFM;
        prim_cfm->instanceIdsListSize = pInst->numberOfMapsInstances;

        if (pInst->numberOfMapsInstances == 0)
        {
            prim_cfm->instanceIdsList = NULL;
        }
        else
        {
            prim_cfm->instanceIdsList = CsrPmemAlloc(sizeof(CsrSchedQid) *
                                              pInst->numberOfMapsInstances);

            ptr = pInst->mapsInstances;
            offset = 0;

            while(ptr)
            {
                CsrMemCpy(&prim_cfm->instanceIdsList[offset],
                       ptr->phandles,
                       sizeof(CsrSchedQid) * ptr->numberInPool);

                offset += ptr->numberInPool;
                ptr = ptr->next;
            }
        }

        CsrBtMapsMessagePut(prim_req->appHandle,prim_cfm);
        return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    }
    else
    {
        return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    }
}

CsrUint8 CsrBtMapsSelectMasInstanceResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsSelectMasInstanceRes *pMsg = (CsrBtMapsSelectMasInstanceRes *) msg;
    CsrUint16 *serviceHandleIndexList = NULL;
    CsrUint16 nofServiceHandleIndicis = 0;

    if(pMsg->proceedWithConnection)
    {
        CsrUint16 tempServiceHandle = csrBtMapsFindServiceHandleIndexFromMasInstanceId(pInst, pMsg->masInstanceId);
        if(tempServiceHandle != 0xFFFF)
        {
            nofServiceHandleIndicis = 1;
            serviceHandleIndexList = CsrPmemAlloc(sizeof(CsrUint16));
            *serviceHandleIndexList = tempServiceHandle;
        }
    }

    return (ObexUtilSetServiceHandleIndexListResponse(pInst->obexServerInst, &serviceHandleIndexList, nofServiceHandleIndicis));
}

CsrUint8 CsrBtMapsActivateReqHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsActivateReq *pMsg = (CsrBtMapsActivateReq *)msg;

    pInst->appHandle = pMsg->appHandle;
    pInst->supportedMessages = pMsg->supportedMessages;
    pInst->maxObexPacketSize = pMsg->obexMaxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;
    pInst->masInstId      = pMsg->instanceId;
    pInst->masInstNameLen = pMsg->nameLen;
    CsrPmemFree(pInst->masInstName);
    pInst->masInstName = NULL;
    
    if (pMsg->nameLen)
    {
        pInst->masInstName   = CsrPmemAlloc(pInst->masInstNameLen);
        CsrMemCpy(pInst->masInstName, pMsg->name, pMsg->nameLen);
    }
    else
    {/* Keep a valid and unique name....*/
        CsrUint8 instIdString[4];
        CsrUint8 pInstId = pInst->masInstId / 10;
        CsrUint8 pRest   = pInst->masInstId % 10;
        
        instIdString[0] = ' ';
        
        instIdString[3] = pRest + '0';
        pRest = pInstId % 10;
        pInstId /= 10;

        instIdString[2] = pRest + '0';
        pRest = pInstId % 10;
        
        instIdString[1] = pRest + '0';

        pInst->masInstNameLen = MAS_INSTANCE_DEFAULT_NAME_LENGTH + 4;
        pInst->masInstName   = CsrPmemAlloc(pInst->masInstNameLen);
        CsrMemCpy(pInst->masInstName, csrBtMapsObexDefaultName, MAS_INSTANCE_DEFAULT_NAME_LENGTH);
        CsrMemCpy(&pInst->masInstName[MAS_INSTANCE_DEFAULT_NAME_LENGTH], instIdString, 4);
    }
    csrBtObexMapsServerStart(pInst);    

    return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
}

CsrUint8 CsrBtMapsDeactivateReqHandler(MapsInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    
    return (ObexUtilServDeactivateStart(pInst->obexServerInst,
                                        pInst->serverChannel,
                                        L2CA_PSM_INVALID,
                                        CsrBtMapsDeactivateCfmHandler));
}

CsrUint8 CsrBtMapsConnectResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsConnectRes *pMsg = (CsrBtMapsConnectRes *) msg;

    if (pMsg->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        pInst->srvConnected = TRUE;
    }
    return (csrBtMapsServerConnectResponseHandler(pInst, pMsg->responseCode, NULL));
}

CsrUint8 CsrBtMapsSetBackFolderResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsSetBackFolderRes *pMsg  = (CsrBtMapsSetBackFolderRes *) msg;
    CsrUint8  headerPriorityTable[3] = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    return (ObexUtilServSetpathResponse(pInst->obexServerInst, 
                                        pMsg->responseCode, 
                                        NULL, 
                                        headerPriorityTable, 
                                        NULL,
                                        NULL,
                                        0,
                                        NULL));
}

CsrUint8 CsrBtMapsSetRootFolderResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsSetRootFolderRes *pMsg  = (CsrBtMapsSetRootFolderRes *) msg;
    CsrUint8  headerPriorityTable[3] = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    return (ObexUtilServSetpathResponse(pInst->obexServerInst, 
                                        pMsg->responseCode, 
                                        NULL, 
                                        headerPriorityTable, 
                                        NULL,
                                        NULL,
                                        0,
                                        NULL));
}

CsrUint8 CsrBtMapsSetFolderResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsSetFolderRes *pMsg      = (CsrBtMapsSetFolderRes *) msg;
    CsrUint8  headerPriorityTable[3] = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    return (ObexUtilServSetpathResponse(pInst->obexServerInst, 
                                        pMsg->responseCode, 
                                        NULL, 
                                        headerPriorityTable, 
                                        NULL,
                                        NULL,
                                        0,
                                        NULL));
}

CsrUint8 CsrBtMapsGetFolderListingHeaderResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsGetFolderListingHeaderRes *pMsg = (CsrBtMapsGetFolderListingHeaderRes *)msg;

    pInst->responseAppHeaderLength = CSR_BT_OBEX_APP_PAR_HEADER_SIZE + 
                                     CSR_BT_MAPS_TAG_HEADER_SIZE + 
                                     CSR_BT_MAP_TAG_ID_LENGTH_FOLDER_LISTING_SIZE;

    pInst->responseAppHeader = CsrPmemAlloc(pInst->responseAppHeaderLength);
    pInst->responseAppHeader[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    pInst->responseAppHeader[1] = (CsrUint8) (pInst->responseAppHeaderLength >> 8);
    pInst->responseAppHeader[2] = (CsrUint8) (pInst->responseAppHeaderLength);
    pInst->responseAppHeader[3] = CSR_BT_MAP_TAG_ID_FOLDER_LISTING_SIZE;
    pInst->responseAppHeader[4] = CSR_BT_MAP_TAG_ID_LENGTH_FOLDER_LISTING_SIZE;
    pInst->responseAppHeader[5] = (CsrUint8) (pMsg->fullFolderListingSize >> 8);
    pInst->responseAppHeader[6] = (CsrUint8) (pMsg->fullFolderListingSize);
    
    pInst->maxObexResponseLength = pInst->maxObexResponseLength - pInst->responseAppHeaderLength;
    csrBtMapsGetFolderListingIndSend(pInst->appHandle,
                                     pInst->mapsInstanceId,
                                     pInst->maxObexResponseLength);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtMapsGetMessageListingHeaderResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsGetMessageListingHeaderRes *pMsg = (CsrBtMapsGetMessageListingHeaderRes *)msg;
    CsrUint16   i = 0, j = 0;

    pInst->responseAppHeaderLength = CSR_BT_OBEX_APP_PAR_HEADER_SIZE + 
                                     CSR_BT_MAPS_TAG_HEADER_SIZE + 
                                     CSR_BT_MAP_TAG_ID_LENGTH_NEW_MESSAGE + 
                                     CSR_BT_MAPS_TAG_HEADER_SIZE + 
                                     CsrStrLen((char *)pMsg->mseTime) + 
                                     CSR_BT_MAPS_TAG_HEADER_SIZE + 
                                     CSR_BT_MAP_TAG_ID_LENGTH_MESSAGES_LISTING_SIZE;

    pInst->responseAppHeader = CsrPmemAlloc(pInst->responseAppHeaderLength);
    pInst->responseAppHeader[i++] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    pInst->responseAppHeader[i++] = (CsrUint8) (pInst->responseAppHeaderLength >> 8);
    pInst->responseAppHeader[i++] = (CsrUint8) (pInst->responseAppHeaderLength);
    pInst->responseAppHeader[i++] = CSR_BT_MAP_TAG_ID_NEW_MESSAGE;
    pInst->responseAppHeader[i++] = CSR_BT_MAP_TAG_ID_LENGTH_NEW_MESSAGE;
    pInst->responseAppHeader[i++] = pMsg->newMessages;
    pInst->responseAppHeader[i++] = CSR_BT_MAP_TAG_ID_MSE_TIME;
    pInst->responseAppHeader[i++] = (CsrUint8) (CsrStrLen((char *)pMsg->mseTime));

    for (j = 0; j < CsrStrLen((char *)pMsg->mseTime); j++)
    {
        pInst->responseAppHeader[i++] = *(pMsg->mseTime + j);
    }
    pInst->responseAppHeader[i++] = CSR_BT_MAP_TAG_ID_MESSAGES_LISTING_SIZE;
    pInst->responseAppHeader[i++] = CSR_BT_MAP_TAG_ID_LENGTH_MESSAGES_LISTING_SIZE;
    pInst->responseAppHeader[i++] = (CsrUint8) (pMsg->fullMessageListingSize >> 8);
    pInst->responseAppHeader[i++] = (CsrUint8) (pMsg->fullMessageListingSize );
    
    
    pInst->maxObexResponseLength = pInst->maxObexResponseLength - pInst->responseAppHeaderLength;
    csrBtMapsGetMessageListingIndSend(pInst->appHandle,
                                      pInst->mapsInstanceId,
                                      pInst->maxObexResponseLength);
    CsrPmemFree(pMsg->mseTime);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtMapsGetMessageHeaderResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsGetMessageHeaderRes *pMsg = (CsrBtMapsGetMessageHeaderRes *)msg;

    pInst->responseAppHeaderLength = CSR_BT_OBEX_APP_PAR_HEADER_SIZE + 
                                     CSR_BT_MAPS_TAG_HEADER_SIZE + 
                                     CSR_BT_MAP_TAG_ID_LENGTH_FRACTION_DELIVER;

    pInst->responseAppHeader = CsrPmemAlloc(pInst->responseAppHeaderLength);
    pInst->responseAppHeader[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    pInst->responseAppHeader[1] = (CsrUint8) (pInst->responseAppHeaderLength >> 8);
    pInst->responseAppHeader[2] = (CsrUint8) (pInst->responseAppHeaderLength);
    pInst->responseAppHeader[3] = CSR_BT_MAP_TAG_ID_FRACTION_DELIVER;
    pInst->responseAppHeader[4] = CSR_BT_MAP_TAG_ID_LENGTH_FRACTION_DELIVER;
    pInst->responseAppHeader[5] = pMsg->fractionDeliver;
    
    pInst->maxObexResponseLength = pInst->maxObexResponseLength - pInst->responseAppHeaderLength;
    csrBtMapsGetMessageIndSend(pInst->appHandle,
                               pInst->mapsInstanceId,
                               pInst->maxObexResponseLength);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 csrBtCommonGetResHandler(MapsInstanceData *pInst,
                                  CsrBtObexResponseCode responseCode, 
                                  CsrUint16 bodyLength, 
                                  CsrUint8 *body)
{   
    CsrUint8 headerPriorityTable[6];

    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
    headerPriorityTable[0] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
    headerPriorityTable[1] = CSR_BT_OBEX_UTIL_BODY_HEADER;
   
    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
       ObexUtilServSetGetContinueFunc(pInst->obexServerInst, csrBtMapsServerGetNextIndHandler);
    }
    return (ObexUtilServGetWithHeaderResponse(pInst->obexServerInst,
                                             responseCode,
                                             NULL,
                                             headerPriorityTable,
                                             1,  
                                             0,
                                             NULL,
                                             NULL,
                                             pInst->responseAppHeaderLength,
                                             &(pInst->responseAppHeader),
                                             0,  
                                             NULL,
                                             &body,
                                             bodyLength,
                                             CSR_BT_OBEX_SRMP_INVALID));
    
   
}

CsrUint8 CsrBtMapsGetFolderListingResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsGetFolderListingRes *pMsg = (CsrBtMapsGetFolderListingRes *)msg;

    return (csrBtCommonGetResHandler(pInst, pMsg->responseCode, pMsg->bodyLength, pMsg->body));
}

CsrUint8 CsrBtMapsGetMessageListingResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsGetMessageListingRes *pMsg = (CsrBtMapsGetMessageListingRes *)msg;

    return (csrBtCommonGetResHandler(pInst, pMsg->responseCode, pMsg->bodyLength, pMsg->body));
}

CsrUint8 CsrBtMapsGetMessageResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsGetMessageRes *pMsg = (CsrBtMapsGetMessageRes *)msg;

    return (csrBtCommonGetResHandler(pInst, pMsg->responseCode, pMsg->bodyLength, pMsg->body));
}

CsrUint8 CsrBtMapsSetMessageStatusResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsSetMessageStatusRes *pMsg = (CsrBtMapsSetMessageStatusRes *)msg;

    return (ObexUtilServPutResponse(pInst->obexServerInst, 
                                    pMsg->responseCode,
                                    CSR_BT_OBEX_SRMP_INVALID));
}


CsrUint8 CsrBtMapsPushMessageHeaderResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsPushMessageHeaderRes *pMsg = (CsrBtMapsPushMessageHeaderRes *)msg;
    csrBtMapsSaveFirstReceivedMessageHandle(pInst, pMsg->messageHandle);
    csrBtMapsPushMessageIndSend(pInst);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtMapsPushMessageResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrUint8 result;    
    CsrBtMapsPushMessageRes *pMsg = (CsrBtMapsPushMessageRes *)msg;
    csrBtMapsSaveFirstReceivedMessageHandle(pInst, pMsg->messageHandle);

    if(pMsg->responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        ObexUtilServSetPutContinueFunc(pInst->obexServerInst, CsrBtMapsPushMessageNextIndHandler);
        result = ObexUtilServPutResponse(pInst->obexServerInst, pMsg->responseCode, CSR_BT_OBEX_SRMP_INVALID);
    }
    else
    {
        CsrUint8    headerPriorityTable[4];

        CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 4);
        headerPriorityTable[0] = CSR_BT_OBEX_UTIL_NAME_HEADER;

        result = ObexUtilServPutWithHeaderResponse(pInst->obexServerInst,
                                                   pMsg->responseCode,
                                                   NULL,
                                                   headerPriorityTable,
                                                   &(pInst->pNameHeader),
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   CSR_BT_OBEX_SRMP_INVALID);
                    
        pInst->pNameHeader = NULL;
    }
    return (result);
}

CsrUint8 CsrBtMapsUpdateInboxResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsUpdateInboxRes *pMsg = (CsrBtMapsUpdateInboxRes *)msg;

    return (ObexUtilServPutResponse(pInst->obexServerInst, 
                                    pMsg->responseCode,
                                    CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtMapsNotificationRegistrationResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsNotificationRegistrationRes *pMsg = (CsrBtMapsNotificationRegistrationRes *)msg;
    CsrUint8                             result;
    result = ObexUtilServPutResponse(pInst->obexServerInst, 
                                     pMsg->responseCode,
                                     CSR_BT_OBEX_SRMP_INVALID);

    if (pMsg->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
        pInst->srvConnected &&
        result == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
    {
        if (pInst->notificationStatus && 
            pInst->cliState == CSR_BT_MAPS_CLI_NULL_STATE)
        {
            CsrUint16                   index;
            CmnCsrBtLinkedListStruct    *sdpTag = NULL;
            CsrUint8                    tempTarget[MAPS_TARGET_LENGTH] = MAPS_MNS_TARGET;
            CsrUint8                    *target = (CsrUint8 *) CsrPmemAlloc(MAPS_TARGET_LENGTH);
            
            CsrMemCpy(target, tempTarget, MAPS_TARGET_LENGTH);
        
            sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_MESSAGE_NOTIFICATION_SERVER_UUID, &index);
            CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

            result = ObexUtilConnectRequest(pInst->obexClientInst,
                                            &sdpTag,
                                            pInst->deviceAddr,
                                            pInst->secOutLevel,
                                            pInst->maxObexPacketSize,
                                            CSR_BT_MAPS_PROFILE_DEFAULT_MTU_SIZE,
                                            &target,
                                            MAPS_TARGET_LENGTH,
                                            0,
                                            NULL,
                                            CSR_BT_MAPS_LP_SUPERVISION_TIMEOUT,
                                            NULL,
                                            0,
                                            0,
                                            NULL,
                                            0,
                                            NULL,
                                            csrBtMapsClientConnectResultHandler,
                                            NULL,
                                            csrBtMapsClientDisconnectIndHandler,
                                            NULL,
                                            pInst->windowSize,
                                            pInst->localSrmEnable);
                                          
            pInst->cliState = CSR_BT_MAPS_CLI_IDLE_STATE;
            pInst->bodyFromPutIndLength = 0;
        }
        else if (pInst->srvConnected)
        {
            pInst->cliState = CSR_BT_MAPS_CLI_NULL_STATE;
            result = ObexUtilDisconnectRequest(pInst->obexClientInst, TRUE, NULL);
        }
        else
        {
            result = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
        }
    }

    return result;
}

CsrUint8 CsrBtMapsEventNotificationReqHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsEventNotificationReq *pMsg = (CsrBtMapsEventNotificationReq*)msg;
    CsrUint8                      *requestAppHeader;
    CsrUint16                     requestAppHeaderLength;
    CsrUint8                      headerPriorityTable[6];

    if (pInst->cliState == CSR_BT_MAPS_CLI_NULL_STATE ||
        pInst->notificationStatus == FALSE)
    {
        csrBtMapsEventNotificationCfmSend(pInst, 
                                          CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE, 
                                          CSR_BT_SUPPLIER_OBEX_PROFILES);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
        headerPriorityTable[0] = CSR_BT_OBEX_UTIL_TYPE_HEADER;
        headerPriorityTable[1] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
    

        requestAppHeaderLength = CSR_BT_OBEX_APP_PAR_HEADER_SIZE + 
                                 CSR_BT_MAPS_TAG_HEADER_SIZE + 
                                 CSR_BT_MAP_TAG_ID_LENGTH_MAS_INSTANCE_ID;

        requestAppHeader = CsrPmemAlloc(requestAppHeaderLength);
        requestAppHeader[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
        requestAppHeader[1] = (CsrUint8) (requestAppHeaderLength >> 8);
        requestAppHeader[2] = (CsrUint8) (requestAppHeaderLength);
        requestAppHeader[3] = CSR_BT_MAP_TAG_ID_MAS_INSTANCE_ID;
        requestAppHeader[4] = CSR_BT_MAP_TAG_ID_LENGTH_MAS_INSTANCE_ID;
        requestAppHeader[5] = (CsrUint8)pMsg->instanceId;
    
        pInst->cliState = CSR_BT_MAPS_CLI_PENDING_PEER_STATE;

        return (ObexUtilPutRequest(pInst->obexClientInst,
                                   headerPriorityTable,
                                   0,         
                                   (CsrUint8 *)TYPE_HEADER_EVENT_NOTIFICATION_TXT,                 
                                   NULL,
                                   NULL,
                                   requestAppHeaderLength,
                                   &requestAppHeader,
                                   0,  
                                   NULL,      
                                   csrBtMapsEventNotificationContinueHandler,
                                   csrBtMapsEventNotificationResultHandler));  
    }
}

CsrUint8 CsrBtMapsEventNotificationResHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsEventNotificationRes *pMsg = (CsrBtMapsEventNotificationRes*)msg;

    if (pInst->cliState == CSR_BT_MAPS_CLI_NULL_STATE)
    {
        csrBtMapsEventNotificationCfmSend(pInst, 
                                          CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE, 
                                          CSR_BT_SUPPLIER_OBEX_PROFILES);
        pInst->notificationStatus = FALSE;
        csrBtMapsNotificationRegistrationIndSend(pInst);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else if (pMsg->responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE ||
             pMsg->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        pInst->cliState = CSR_BT_MAPS_CLI_PENDING_PEER_STATE;

        return(ObexUtilPutContinueRequest(pInst->obexClientInst,
                                          (CsrBool)(pMsg->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE ? TRUE : FALSE),
                                          &pMsg->body,
                                          pMsg->bodyLength));
    }
    else
    {
        pInst->cliState = CSR_BT_MAPS_CLI_IDLE_STATE;
        return (ObexUtilAbortRequest(pInst->obexClientInst,
                                     NULL,
                                     csrBtMapsClientAbortIndHandler));
    }
}

CsrUint8 CsrBtMapsSecurityLevelReqHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsSecurityLevelReq *pMsg     = (CsrBtMapsSecurityLevelReq*)msg;
    CsrBtResultCode           result    = CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER;
    CsrBtResultCode           inResult  = CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER;
    CsrBtResultCode           outResult = CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER;
    
    inResult = CsrBtScSetSecInLevel(&pInst->secInLevel, pMsg->secLevel,
                                    CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_INCOMING,
                                    CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_INCOMING,
                                    CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                    CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    outResult = CsrBtScSetSecOutLevel(&pInst->secOutLevel, pMsg->secLevel,
                                      CSR_BT_OBEX_MESSAGE_ACCESS_MANDATORY_SECURITY_INCOMING,
                                      CSR_BT_OBEX_MESSAGE_ACCESS_DEFAULT_SECURITY_INCOMING,
                                      CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                      CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);
    
    if (inResult  == CSR_BT_RESULT_CODE_OBEX_SUCCESS && 
        outResult == CSR_BT_RESULT_CODE_OBEX_SUCCESS)
    {
        result = CSR_BT_RESULT_CODE_OBEX_SUCCESS;
    }
    ObexUtilServChangeIncomingSecurity(pInst->obexServerInst, pInst->secInLevel);
    csrBtMapsSecurityLevelCfmSend(pInst->appHandle,
                                  pInst->mapsInstanceId,
                                  result,
                                  CSR_BT_SUPPLIER_OBEX_PROFILES);

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtMapsRegisterQIDReqHandler(MapsInstanceData *pInst, void *msg)
{
    CsrBtMapsRegisterQidReq *prim;
    CsrBtMapsInstancePool   *ptr;
    CsrBtMapsInstancePool   *prev;

    prim = (CsrBtMapsRegisterQidReq *) msg;

    if (pInst->mapsInstanceId == CSR_BT_MAPS_IFACEQUEUE)
    {
        ptr = pInst->mapsInstances;
        prev = NULL;

        while((ptr) && (ptr->numberInPool == MAPS_INSTANCES_POOL_SIZE))
        {
            prev = ptr;
            ptr = ptr->next;
        }

        if (ptr)
        {
            /* Do nothing */
        }
        else
        {
            ptr = CsrPmemZalloc(sizeof(CsrBtMapsInstancePool));
            ptr->numberInPool = 0;
            ptr->next=NULL;

            if (prev)
            {
                prev->next = ptr;
            }
            else
            {
                pInst->mapsInstances = ptr;
            }
        }

        ptr->phandles[ptr->numberInPool++] = prim->mapsInstanceId;
        pInst->numberOfMapsInstances++;
    }
    else
    {
        CSR_BT_GENERAL_EXCEPTION("MAPS - Register QID req",0,CSR_BT_MAPS_REGISTER_QID_REQ,"Task not MAPS-manager");
    }

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

