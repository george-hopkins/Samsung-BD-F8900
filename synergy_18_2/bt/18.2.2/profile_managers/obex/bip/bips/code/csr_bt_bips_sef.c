/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bips_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_bips_sef.h"
#include "csr_bt_bips_prim.h"
#include "csr_bt_sdc_support.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_private_lib.h"
#endif /* CSR_AMP_ENABLE */

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define BIPS_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_LENGTH        0x0E
#define BIPS_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG           0x01
#define BIPS_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG_LENGTH    0x02
#define BIPS_APPLICATION_PARAMETERS_LISTSTARTOFFSET_TAG             0x02
#define BIPS_APPLICATION_PARAMETERS_LISTSTARTOFFSET_TAG_LENGTH      0x02
#define BIPS_APPLICATION_PARAMETERS_CAPTURED_IMAGES_TAG             0x03
#define BIPS_APPLICATION_PARAMETERS_CAPTURED_IMAGES_TAG_LENGTH      0x01

static const char CSR_BT_BIPS_TEXTTYPE_IMG_IMAGE[]         = {"x-bt/img-img"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_THUMB[]         = {"x-bt/img-thm"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_ATTACHMENT[]    = {"x-bt/img-attachment"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_ARCHIVE[]       = {"x-bt/img-archive"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_CAPABILITIES[]  = {"x-bt/img-capabilities"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_MONITORING[]    = {"x-bt/img-monitoring"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_PROPERTIES[]    = {"x-bt/img-properties"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_STATUS[]        = {"x-bt/img-status"};
static const char CSR_BT_BIPS_TEXTTYPE_IMG_LISTING[]       = {"x-bt/img-listing"};

static CsrUint8 csrBtBipsActivateStartHandler(BipsInstanceData *pInst);
static CsrBool  csrBtBipsGetOperationFromType(BipsInstanceData *pInst, CsrUint8 obexOperation);

static const CsrUint8 BasicImagingSessions[CSR_BT_NUMBER_OF_BASIC_IMAGING_SESSION][CSR_BT_BIPS_TARGET_LENGTH] =
{
    CSR_BT_BASIC_IMAGING_IMAGE_PUSH,
    CSR_BT_BASIC_IMAGING_REMOTE_CAMERA,
    CSR_BT_BASIC_IMAGING_AUTOMATIC_ARCHIVE
};

static const CsrUint8 serviceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,                     /* AttrID , ServiceClassIDList */
    0x35,0x03,                          /* 3 bytes in total DataElSeq */
    0x19,0x11,0x1B,                     /* 2 byte UUID, Service class = ImagingResponde */

    /* Protocol descriptor list */
    0x09,0x00,0x04,                     /* AttrId ProtocolDescriptorList */
    0x35,0x11,                          /* 17 bytes in total DataElSeq */
    0x35,0x03,                          /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,                     /* 2 byte UUID, Protocol = L2CAP */
    0x35,0x05,                          /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,                     /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,                          /* 1 byte UINT - server channel */
    0x35,0x03,                          /* 3 bytes in DataElSeq */
    0x19, 0x00, 0x08,                   /* 2 byte UUID, Protocol = OBEX */

    /* Bluetooth Profile Descriptor List */
    0x09,0x00,0x09,                    /* AttrId, BluetoothProfileDescriptorList */
    0x35,0x08,                         /* 8 bytes in total DataElSeq */
    0x35,0x06,                         /* 6 bytes in total DataElSeq */
    0x19,0x11,0x1A,                    /* 2 byte UUID, Service class = Imaging */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    0x09,0x01,0x01,                    /* 2 byte UINT, Profile Version = 0x0101 */
#else
    0x09,0x01,0x00,                    /* 2 byte UINT, Profile Version = 0x0100 */
#endif

    /* Service name */
    0x09,0x01,0x00,                     /* AttrId - Service Name */
    0x25,0x07,                          /* 7 byte string */
    'I', 'M', 'A', 'G', 'I', 'N', 'G',

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    /* OBEX-over-L2CAP */
    0x09, 0x02, 0x00,      /* 16 bit attribute */
    0x09, 0x00, 0x00,      /* 16 bit L2CAP PSM - filled in by app (index:34=0x22) */
#endif

    /* Supported Capabilities */
    0x09, 0x03, 0x10,                  /* AttrId, SupportedCapabalities */
    0x08, 0x01,                        /* 1 byte UINT - Set to Generic imaging */

    /* Supported Features */
    0x09, 0x03, 0x11,                  /* AttrId, SupportedFeatures */
    0x09, 0x00, 0x00,                  /* 2 byte UINT. Set to ImagePush, Remote Camera and Auto Archive */

    /* Supported Functions */
    0x09, 0x03, 0x12,                  /* AttrId, SupportedFunctions */
    0x0A, 0x00, 0x00, 0x00, 0x00,      /* 4 byte UINT */

    /* Total imaging data capacity */
    0x09, 0x03, 0x13,                  /* AttrId, TotalImagingDataCapacity */
    0x0B, 0x00, 0x00, 0x00, 0x00,      /* 8 byte UINT */
    0x00, 0x00, 0x00, 0x00,

    /* BrowseGroupList */
    0x09, 0x00, 0x05,                  /* AttrId = BrowseGroupList */
    0x35, 0x03,                        /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,                  /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

/* Common upStream message handlers */
void CsrBtBipsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_BIPS_PRIM, msg);
}

static void csrBtBipsConnectIndSend(BipsInstanceData *pInst,
                                    CsrUint16        maxPeerObexPacketLength,
                                    CsrUint32        length,
                                    CsrUint32        count,
                                    CsrUint32        supportedFunctions,
                                    CsrBtConnId      btConnId)
{
    CsrBtBipsConnectInd *pMsg   = (CsrBtBipsConnectInd *) CsrPmemAlloc(sizeof(CsrBtBipsConnectInd));
    pMsg->type                  = CSR_BT_BIPS_CONNECT_IND;
    pMsg->connectionId          = (CsrUint32) pInst->localServerCh;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->obexPeerMaxPacketSize = (CsrUint16)(maxPeerObexPacketLength - CSR_BT_BIPS_HEADER_SIZE);
    pMsg->deviceAddr            = pInst->deviceAddr;
    pMsg->connectType           = pInst->connectType;
    pMsg->length                = length;
    pMsg->count                 = count;
    pMsg->supportedFunctions    = supportedFunctions;
    pMsg->btConnId              = btConnId;

    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsAuthenticateIndSend(BipsInstanceData *pInst,
                                         CsrBtDeviceAddr  deviceAddr,
                                         CsrUint8         options,
                                         CsrUint16        realmLength,
                                         CsrUint8       *realm)
{
    CsrBtBipsAuthenticateInd *pMsg  = (CsrBtBipsAuthenticateInd *) CsrPmemAlloc(sizeof(CsrBtBipsAuthenticateInd));
    pMsg->type                      = CSR_BT_BIPS_AUTHENTICATE_IND;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->realmLength               = realmLength;
    pMsg->realm                     = realm;
    pMsg->deviceAddr                = deviceAddr;
    pMsg->options                   = options;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsDisconnectIndSend(BipsInstanceData *pInst,
                                       CsrBtReasonCode  reasonCode,
                                       CsrBtSupplier    reasonSupplier)
{
    CsrBtBipsDisconnectInd * pMsg = (CsrBtBipsDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtBipsDisconnectInd));
    pMsg->type                    = CSR_BT_BIPS_DISCONNECT_IND;
    pMsg->pHandleInst             = pInst->myAppHandle;
    pMsg->connectionId            = (CsrUint32) pInst->localServerCh;
    pMsg->reasonCode              = reasonCode;
    pMsg->reasonSupplier          = reasonSupplier;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsConIdIndSend(BipsInstanceData *pInst,
                                  CsrBtBipsPrim    type)
{
    CsrBtBipsConIdInd * pMsg = (CsrBtBipsConIdInd *) CsrPmemAlloc(sizeof(CsrBtBipsConIdInd));
    pMsg->type          = type;
    pMsg->pHandleInst   = pInst->myAppHandle;
    pMsg->connectionId  = (CsrUint32) pInst->localServerCh;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsChallengeIndSend(BipsInstanceData *pInst, CsrBool challengeFlag)
{
    CsrBtBipsChallengeInd * pMsg = (CsrBtBipsChallengeInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsChallengeInd));

    pMsg->type          = CSR_BT_BIPS_CHALLENGE_IND;
    pMsg->pHandleInst   = pInst->myAppHandle;
    pMsg->deviceAddr    = pInst->deviceAddr;
    pMsg->connectType   = pInst->connectType;
    pMsg->challenged    = challengeFlag;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}


/* Image Push upStream message handlers */
static void csrBtBipsPushPutImageHeaderIndSend(BipsInstanceData *pInst)
{
    CsrBtBipsPushPutImageHeaderInd *pMsg = (CsrBtBipsPushPutImageHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsPushPutImageHeaderInd));

    pMsg->type                          = CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND;
    pMsg->pHandleInst                   = pInst->myAppHandle;
    pMsg->connectionId                  = (CsrUint32) pInst->localServerCh;
    pMsg->imageDescriptorOffset         = pInst->collectedInfo->imgDescriptorOffset;
    pMsg->imageDescriptorLength         = pInst->collectedInfo->imgDescriptorLength;
    pMsg->ucs2nameOffset                = pInst->collectedInfo->nameOffset;
    pMsg->payloadLength                 = pInst->collectedInfo->obexPacketLength;
    pMsg->payload                       = pInst->collectedInfo->obexPacket;
    pInst->collectedInfo->obexPacket    = NULL;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsPushPutLinkedThumbnailHeaderIndSend(BipsInstanceData *pInst)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderInd *pMsg = (CsrBtBipsPushPutLinkedThumbnailHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsPushPutLinkedThumbnailHeaderInd));

    pMsg->type                          = CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND;
    pMsg->pHandleInst                   = pInst->myAppHandle;
    pMsg->connectionId                  = (CsrUint32) pInst->localServerCh;
    pMsg->imageHandleOffset             = pInst->collectedInfo->imgHandleOffset;
    pMsg->payloadLength                 = pInst->collectedInfo->obexPacketLength;
    pMsg->payload                       = pInst->collectedInfo->obexPacket;
    pInst->collectedInfo->obexPacket    = NULL;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsPushPutLinkedAttachmentHeaderIndSend(BipsInstanceData *pInst)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderInd *pMsg = (CsrBtBipsPushPutLinkedAttachmentHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsPushPutLinkedAttachmentHeaderInd));

    pMsg->type                          = CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND;
    pMsg->pHandleInst                   = pInst->myAppHandle;
    pMsg->connectionId                  = (CsrUint32) pInst->localServerCh;
    pMsg->imageHandleOffset             = pInst->collectedInfo->imgHandleOffset;
    pMsg->attachmentDescriptorOffset    = pInst->collectedInfo->imgDescriptorOffset;
    pMsg->attachmentDescriptorLength    = pInst->collectedInfo->imgDescriptorLength;
    pMsg->payloadLength                 = pInst->collectedInfo->obexPacketLength;
    pMsg->payload                       = pInst->collectedInfo->obexPacket;
    pInst->collectedInfo->obexPacket    = NULL;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

/* Remote Camera (RC) upStream message handlers */
static void csrBtBipsRcGetMonitoringImageHeaderIndSend(BipsInstanceData *pInst, CsrBool storeFlag)
{
    CsrBtBipsRcGetMonitoringImageHeaderInd *pMsg = (CsrBtBipsRcGetMonitoringImageHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetMonitoringImageHeaderInd));

    pMsg->type          = CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND;
    pMsg->pHandleInst   = pInst->myAppHandle;
    pMsg->connectionId  = (CsrUint32) pInst->localServerCh;
    pMsg->storeFlag     = storeFlag;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsRcGetImagePropertiesHeaderIndSend(BipsInstanceData *pInst)
{
    CsrBtBipsRcGetImagePropertiesHeaderInd * pMsg = (CsrBtBipsRcGetImagePropertiesHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetImagePropertiesHeaderInd));

    pMsg->type              = CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND;
    pMsg->pHandleInst       = pInst->myAppHandle;
    pMsg->connectionId      = (CsrUint32) pInst->localServerCh;
    pMsg->imageHandleOffset = pInst->collectedInfo->imgHandleOffset;
    pMsg->payloadLength     = pInst->collectedInfo->obexPacketLength;
    pMsg->payload           = pInst->collectedInfo->obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsRcGetImageHeaderIndSend(BipsInstanceData *pInst)
{
    CsrBtBipsRcGetImageHeaderInd * pMsg = (CsrBtBipsRcGetImageHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetImageHeaderInd));

    pMsg->type              = CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND;
    pMsg->pHandleInst       = pInst->myAppHandle;
    pMsg->connectionId      = (CsrUint32) pInst->localServerCh;
    pMsg->imageHandleOffset = pInst->collectedInfo->imgHandleOffset;
    pMsg->descriptorOffset  = pInst->collectedInfo->imgDescriptorOffset;
    pMsg->descriptorLength  = pInst->collectedInfo->imgDescriptorLength;
    pMsg->payloadLength     = pInst->collectedInfo->obexPacketLength;
    pMsg->payload           = pInst->collectedInfo->obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsRcGetLinkedThumbnailHeaderIndSend(BipsInstanceData *pInst)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderInd * pMsg = (CsrBtBipsRcGetLinkedThumbnailHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetLinkedThumbnailHeaderInd));

    pMsg->type              = CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND;
    pMsg->pHandleInst       = pInst->myAppHandle;
    pMsg->connectionId      = (CsrUint32) pInst->localServerCh;
    pMsg->imageHandleOffset = pInst->collectedInfo->imgHandleOffset;
    pMsg->payloadLength     = pInst->collectedInfo->obexPacketLength;
    pMsg->payload           = pInst->collectedInfo->obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

/* Auto Archive (AA) upStream message handlers */
static void csrBtBipsAaGetCapabilitiesIndSend(BipsInstanceData *pInst,
                                              CsrUint32        capabilitiesObjectOffset,
                                              CsrUint32        capabilitiesObjectLength,
                                              CsrUint16        obexPacketLength,
                                              CsrUint8       *obexPacket)
{
    CsrBtBipsAaGetCapabilitiesInd * pMsg = (CsrBtBipsAaGetCapabilitiesInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsAaGetCapabilitiesInd));

    pMsg->type                      = CSR_BT_BIPS_AA_GET_CAPABILITIES_IND;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->capabilitiesObjectOffset  = capabilitiesObjectOffset;
    pMsg->capabilitiesObjectLength  = capabilitiesObjectLength;
    pMsg->payloadLength             = obexPacketLength;
    pMsg->payload                   = obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsAaGetCapabilitiesCfmSend(BipsInstanceData      *pInst,
                                              CsrBtObexResponseCode responseCode,
                                              CsrUint32             capabilitiesObjectOffset,
                                              CsrUint32             capabilitiesObjectLength,
                                              CsrUint16             obexPacketLength,
                                              CsrUint8            *obexPacket)
{
    CsrBtBipsAaGetCapabilitiesCfm * pMsg = (CsrBtBipsAaGetCapabilitiesCfm *)
        CsrPmemAlloc(sizeof(CsrBtBipsAaGetCapabilitiesCfm));

    pMsg->type                      = CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->responseCode              = responseCode;
    pMsg->capabilitiesObjectOffset  = capabilitiesObjectOffset;
    pMsg->capabilitiesObjectLength  = capabilitiesObjectLength;
    pMsg->payloadLength             = obexPacketLength;
    pMsg->payload                   = obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsAaGetImageListHeaderIndSend(BipsInstanceData *pInst,
                                                 CsrUint16        nbReturnedHandles,
                                                 CsrUint16        imageHandlesDescriptorLength,
                                                 CsrUint8         *imageHandlesDescriptor)
{
    CsrBtBipsAaGetImageListHeaderInd * pMsg = (CsrBtBipsAaGetImageListHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsAaGetImageListHeaderInd));

    pMsg->type                          = CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND;
    pMsg->pHandleInst                   = pInst->myAppHandle;
    pMsg->nbReturnedHandles             = nbReturnedHandles;
    pMsg->imageHandlesDescriptorLength  = imageHandlesDescriptorLength;
    pMsg->imageHandlesDescriptor        = imageHandlesDescriptor;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsAaDeleteImageCfmSend(BipsInstanceData      *pInst,
                                          CsrBtObexResponseCode responseCode)
{
    CsrBtBipsAaDeleteImageCfm * pMsg = (CsrBtBipsAaDeleteImageCfm *)
        CsrPmemAlloc(sizeof(CsrBtBipsAaDeleteImageCfm));

    pMsg->type                  = CSR_BT_BIPS_AA_DELETE_IMAGE_CFM;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->responseCode          = responseCode;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipsAaAbortCfmSend(BipsInstanceData *pInst)
{
    CsrBtBipsAaAbortCfm * pMsg  = (CsrBtBipsAaAbortCfm *) CsrPmemAlloc(sizeof(CsrBtBipsAaAbortCfm));
    pMsg->type                  = CSR_BT_BIPS_AA_ABORT_CFM;
    pMsg->pHandleInst           = pInst->myAppHandle;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
}

static CsrBool csrBtBipsAaExtractSdpInfo(BipsInstanceData           *pInst,
                                         CmnCsrBtLinkedListStruct   *sdpTag,
                                         CsrUint32                  *supportedFunctions)
{
    *supportedFunctions = 0;

    if (sdpTag)
    {
        CsrUint32 dataLen;
        CsrUint8  *data;
        CsrUintFast16 i;
        CsrUintFast16 numOfSdpRecords = CsrBtUtilBllGetNofEntriesEx(sdpTag);

        for (i = 0; i < numOfSdpRecords; i++)
        { /* Must find a record where the ServiceId match pInst->serviceId */
            if (CsrBtUtilSdrGetUuidDataAttributeFromAttributeUuid(sdpTag, i,
                                                                  CSR_BT_SERVICE_ID_ATTRIBUTE_IDENTIFIER, &data, &dataLen) &&
                dataLen == CSR_BT_BIPS_APPLICATION_PARAMETER_SERVICEID_LENGTH)
            {
                if (!CsrMemCmp(pInst->serviceId, data, CSR_BT_BIPS_APPLICATION_PARAMETER_SERVICEID_LENGTH))
                {
                    return(CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTag, i, CSR_BT_SUPPORTED_FUNCTIONS_ATTRIBUTE_IDENTIFIER, supportedFunctions));
                }
            }
        }
    }
    return (FALSE);
}

static CsrBool csrBtBipsMapResponseCode(CsrBool               finalFlag,
                                        CsrBtObexResponseCode *responseCode)
{
    CsrBtObexResponseCode rspCode = (*responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK);
    *responseCode                 = rspCode;

    if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE ||
        rspCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && !finalFlag)
        {
            *responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        return (TRUE);
    }
    return (FALSE);
}

static CsrUint8 *csrBtBipsBuildServiceRecordHandler(BipsInstanceData *pInst, CsrUint16 *recordLength)
{
    CsrUint16 length       = sizeof(serviceRecord);
    CsrUint8 *record       = CsrPmemAlloc(length);
    CsrMemCpy(record, serviceRecord, length);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX]       = (CsrUint8)((pInst->upperDataCapacity  >> 24) & 0xff);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX + 1]   = (CsrUint8)((pInst->upperDataCapacity  >> 16) & 0xff);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX + 2]   = (CsrUint8)((pInst->upperDataCapacity  >> 8)  & 0xff);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX + 3]   = (CsrUint8) (pInst->upperDataCapacity         & 0xff);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX + 4]   = (CsrUint8)((pInst->lowerDataCapacity  >> 24) & 0xff);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX + 5]   = (CsrUint8)((pInst->lowerDataCapacity  >> 16) & 0xff);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX + 6]   = (CsrUint8)((pInst->lowerDataCapacity  >> 8)  & 0xff);
    record[CSR_BT_BIPS_TOTAL_IMG_DATA_CAPACITY_INDEX + 7]   = (CsrUint8) (pInst->lowerDataCapacity         & 0xff);
    
    /* If no features are selected in the activation, which is an error
       this statement makes sure that the mandatory feature Image Push
       is enabled. */
    if (pInst->featureSelection == 0)
    {
        pInst->featureSelection = CSR_BT_BIPS_IMAGE_PUSH_FEATURE;
    }

    /* The service record is here altered to enable the selected features */
    if (pInst->featureSelection & CSR_BT_BIPS_IMAGE_PUSH_FEATURE)
    {
        record[CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX + 1] = record[CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX + 1]  | 0x01;
        record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 3]     = record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 3] | 0x0F;
    }

    if (pInst->featureSelection & CSR_BT_BIPS_REMOTE_CAMERA_FEATURE)
    {
        record[CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX + 1] = record[CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX + 1]  | 0x80;
        record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 2]     = record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 2] | 0x41;
        record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 3]     = record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 3] | 0xC0;
    }

    if (pInst->featureSelection & CSR_BT_BIPS_AUTO_ARCHIVE_FEATURE)
    {
        record[CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX + 1] = record[CSR_BT_BIPS_SUPPORTED_FEATURES_SDP_RECORD_INDEX + 1]  | 0x40;
        record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 1]     = record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 1] | 0x01;
        record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 2]     = record[CSR_BT_BIPS_SUPPORTED_FUNC_SDP_RECORD_INDEX + 2] | 0x20;
    }
    *recordLength = length;
    return (record);
}

static CsrUint8 * csrBtBipsBuildImageHandleHeader(CsrUint8  *imageHandle,
                                                  CsrUint16 *userDefinedHeaderLength)
{
    CsrUint8  *userDefinedHeader;
    CsrUint8  i, t = 0;
    CsrUint16 length, tagLength = 0;

    if (imageHandle)
    {
        tagLength  = (CsrUint16) ((CSR_BT_IMG_HANDLE_LENGTH + 1) * 2);
    }
    length               = (CsrUint16) (tagLength + CSR_BT_OBEX_HEADER_LENGTH);
    userDefinedHeader    = (CsrUint8 *) CsrPmemAlloc(length);
    userDefinedHeader[0] = CSR_BT_OBEX_IMG_HANDLE_HEADER;
    userDefinedHeader[1] = (CsrUint8) ((length) >> 8);
    userDefinedHeader[2] = (CsrUint8) (length);

    for (i = 0; i < tagLength; i+=2)
    {
        userDefinedHeader[i + CSR_BT_OBEX_HEADER_LENGTH]     = '\0';
        userDefinedHeader[i + CSR_BT_OBEX_HEADER_LENGTH + 1] = imageHandle[t];
        t++;
    }
    *userDefinedHeaderLength = length;
    return (userDefinedHeader);
}

static CsrUint8 * csrBtBipsBuildImageHandleImageDescriptorHeader(CsrUint8   *imageHandle,
                                                                 CsrUint16  imageDescriptorLength,
                                                                 CsrUint8   *imageDescriptor,
                                                                 CsrUint16  *userDefinedHeaderLength)
{
    CsrUint16 i, t = 0;
    CsrUint16 imageHandleTagLength       = (CsrUint16) ((CSR_BT_IMG_HANDLE_LENGTH + 1) * 2);
    CsrUint16 imageHandleLength          = (CsrUint16) (imageHandleTagLength + CSR_BT_OBEX_HEADER_LENGTH);
    CsrUint16 totalImageDescriptorLength = (CsrUint16) (imageDescriptorLength + CSR_BT_OBEX_HEADER_LENGTH);
    CsrUint16 length                     = (CsrUint16) (imageHandleLength + totalImageDescriptorLength);
    CsrUint8 * userDefinedHeader         = (CsrUint8 *) CsrPmemAlloc(length);

    userDefinedHeader[0] = CSR_BT_OBEX_IMG_HANDLE_HEADER;
    userDefinedHeader[1] = (CsrUint8) ((imageHandleLength) >> 8);
    userDefinedHeader[2] = (CsrUint8) (imageHandleLength);

    for (i = 0; i < imageHandleTagLength; i+=2)
    {
        userDefinedHeader[i + CSR_BT_OBEX_HEADER_LENGTH]     = '\0';
        userDefinedHeader[i + CSR_BT_OBEX_HEADER_LENGTH + 1] = imageHandle[t];
        t++;
    }
    userDefinedHeader[imageHandleLength]         = CSR_BT_OBEX_IMG_DESCRIPTOR_HEADER;
    userDefinedHeader[imageHandleLength + 1]     = (CsrUint8) ((totalImageDescriptorLength) >> 8);
    userDefinedHeader[imageHandleLength + 2]     = (CsrUint8) (totalImageDescriptorLength);
    CsrMemCpy(&userDefinedHeader[imageHandleLength + 3], imageDescriptor, imageDescriptorLength);
    CsrPmemFree(imageDescriptor);
    *userDefinedHeaderLength = (CsrUint16) (length);
    return (userDefinedHeader);
}

static CsrUint8 * csrBtBipsBuildGetImageListAppParams(CsrUint16  nbReturnedHandles,
                                                      CsrUint16  listStartOffset,
                                                      CsrUint8   latestCapturedImages,
                                                      CsrUint16  imageDescriptorLength,
                                                      CsrUint8   *imageDescriptor,
                                                      CsrUint16  *userDefinedHeaderLength)
{
    CsrUint16 totalImageDescriptorLength = (CsrUint16) (imageDescriptorLength + CSR_BT_OBEX_HEADER_LENGTH);
    CsrUint16 length                     = (CsrUint16) (BIPS_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_LENGTH + totalImageDescriptorLength);
    CsrUint8 * userDefinedHeader         = (CsrUint8 *) CsrPmemAlloc(length);

    userDefinedHeader[0]                 = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    userDefinedHeader[1]                 = 0x00;
    userDefinedHeader[2]                 = BIPS_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_LENGTH;
    userDefinedHeader[3]                 = BIPS_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG;
    userDefinedHeader[4]                 = BIPS_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG_LENGTH;
    userDefinedHeader[5]                 = (CsrUint8) ((nbReturnedHandles) >> 8);
    userDefinedHeader[6]                 = (CsrUint8) (nbReturnedHandles);
    userDefinedHeader[7]                 = BIPS_APPLICATION_PARAMETERS_LISTSTARTOFFSET_TAG;
    userDefinedHeader[8]                 = BIPS_APPLICATION_PARAMETERS_LISTSTARTOFFSET_TAG_LENGTH;
    userDefinedHeader[9]                 = (CsrUint8) ((listStartOffset) >> 8);
    userDefinedHeader[10]                = (CsrUint8) (listStartOffset);
    userDefinedHeader[11]                = BIPS_APPLICATION_PARAMETERS_CAPTURED_IMAGES_TAG;
    userDefinedHeader[12]                = BIPS_APPLICATION_PARAMETERS_CAPTURED_IMAGES_TAG_LENGTH;
    userDefinedHeader[13]                = latestCapturedImages;
    userDefinedHeader[14]                = CSR_BT_OBEX_IMG_DESCRIPTOR_HEADER;
    userDefinedHeader[15]                = (CsrUint8) ((totalImageDescriptorLength) >> 8);
    userDefinedHeader[16]                = (CsrUint8) (totalImageDescriptorLength);
    CsrMemCpy(&userDefinedHeader[17], imageDescriptor, imageDescriptorLength);
    CsrPmemFree(imageDescriptor);
    *userDefinedHeaderLength = (CsrUint16) (length);
    return (userDefinedHeader);
}

static CsrBtObexResponseCode csrBtBipsPutHeaderResHandler(BipsInstanceData        *pInst,
                                                          CsrBtObexResponseCode   responseCode)
{
    if (csrBtBipsMapResponseCode(pInst->finalFlag, &responseCode))
    { /* The application has accepted the PUT procedure */
        if (pInst->collectedInfo)
        {
            if (pInst->collectedInfo->cloneObexPacket)
            { /* A body header were included in the first OBEX Put Request
                 packet. */
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            else
            { /* A body header were not included in the first OBEX Put Request
                 packet. Send a response to the OBEX client */
                return (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE);
            }
        }
        else
        { /* A internal error has occured. Send an abort ind message to the
             application in order to tell it that the operation is finish */
            csrBtBipsConIdIndSend(pInst, CSR_BT_BIPS_ABORT_IND);
            return (CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE);
        }
    }
    else
    { /* The application has rejected the PUT Image procedure */
        CsrBtBipsPfreeCollectedInfo(pInst);
        return (responseCode);
    }
}

static CsrBtObexResponseCode csrBtBipsGetHeaderResHandler(BipsInstanceData        *pInst,
                                                          CsrBtObexResponseCode   responseCode)
{
    if (csrBtBipsMapResponseCode(TRUE, &responseCode))
    { /* The application has accepted the GET procedure */
        if (pInst->collectedInfo)
        {
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
        else
        { /* A internal error has occured. Send an abort ind message to the
             application in order to tell it that the operation is finish */
            csrBtBipsConIdIndSend(pInst, CSR_BT_BIPS_ABORT_IND);
            return (CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE);
        }
    }
    else
    { /* The application has rejected the GET Image procedure */
        CsrBtBipsPfreeCollectedInfo(pInst);
        return (responseCode);
    }
}

static CsrBtObexResponseCode csrBtBipsCheckTargetHeader(BipsInstanceData *pInst, CsrUint8 *target, CsrUint16 targetHeaderLength)
{
    pInst->connectType = CSR_BT_BIPS_INVALID_CONNECT_TYPE;
    if (target && targetHeaderLength == CSR_BT_BIPS_TARGET_LENGTH)
    {
        if (!CsrMemCmp(target, (CsrUint8 *) BasicImagingSessions[CSR_BT_BIPS_PUSH_CONNECT], CSR_BT_BIPS_TARGET_LENGTH))
        {
            if (pInst->featureSelection & CSR_BT_BIPS_IMAGE_PUSH_FEATURE)
            {
                pInst->connectType = CSR_BT_BIPS_PUSH_CONNECT;
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            return (CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE);
        }
        else if (!CsrMemCmp(target, (CsrUint8 *) BasicImagingSessions[CSR_BT_BIPS_RC_CONNECT], CSR_BT_BIPS_TARGET_LENGTH))
        {
            if (pInst->featureSelection & CSR_BT_BIPS_REMOTE_CAMERA_FEATURE)
            {
                pInst->connectType = CSR_BT_BIPS_RC_CONNECT;
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            return (CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE);
        }
        else if (!CsrMemCmp(target, (CsrUint8 *) BasicImagingSessions[CSR_BT_BIPS_AA_CONNECT], CSR_BT_BIPS_TARGET_LENGTH))
        {
            if (pInst->featureSelection & CSR_BT_BIPS_AUTO_ARCHIVE_FEATURE)
            {
                pInst->connectType = CSR_BT_BIPS_AA_CONNECT;
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            return (CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE);
        }
    }
    return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
}

static CsrBool csrBtBipsGetApplicationTag(CsrUint8 *obexData, CsrUint8 tag, CsrUint16 *offset)
{
    CsrUint16 headerIndex;

    *offset     = 0;
    headerIndex = CsrBtObexAppParamIndex(obexData);

    if (headerIndex > 0)
    {
        headerIndex = headerIndex + CSR_BT_OBEX_HEADER_LENGTH;

        if (*(obexData + headerIndex) == tag)
        {
            *offset = (CsrUint16)(headerIndex + 2);
            return (TRUE);
        }
    }
    return(FALSE);
}


static CsrBool csrBtBipsDetermineAaServiceId(CsrUint8 *obexData, CsrUint8 *serviceId)
{
    CsrUint16 offset;
    if (csrBtBipsGetApplicationTag(obexData, CSR_BT_BIPS_APPLICATION_PARAMETER_SERVICEID, &offset))
    {
        CsrUint8 i;
        for (i = 0; i < CSR_BT_BIPS_APPLICATION_PARAMETER_SERVICEID_LENGTH; i++)
        {
            serviceId[i]  = *(obexData + offset + i);
        }
        return (TRUE);
    }
    return (FALSE);
}

static CsrBool csrBtBipsDetermineRcStoreFlag(CsrUint8 *obexData)
{
    CsrUint16 offset;
    if (csrBtBipsGetApplicationTag(obexData, CSR_BT_BIPS_APPLICATION_PARAMETER_STOREFLAG, &offset))
    {
        if (*(obexData + offset) == 0x01)
        { /* The value is either 0x00 (false) or 0x01 (true) */
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrBool csrBtBipsDetermineAaNbReturnedHandles(CsrUint8 *obexData, CsrUint16 *nbHandles)
{
    CsrUint16 offset;
    if (csrBtBipsGetApplicationTag(obexData, CSR_BT_BIPS_APPLICATION_PARAMETER_NBRETURNEDHANDLES, &offset))
    {
        *nbHandles = ((*(obexData + offset)) << 8) + *(obexData + offset + 1);
        return (TRUE);
    }
    return (FALSE);
}

static CsrUint8 csrBtBipsConnectResponseHandler(BipsInstanceData             *pInst,
                                                CsrBtObexResponseCode        responseCode,
                                                ObexUtilDigestChallengeType  *digestChallenge,
                                                CsrBool                      authenticateResponse)
{
    CsrUint16 whoHeaderLength = 0;
    CsrUint8  *who            = NULL;
    responseCode              = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

    if (pInst->connectType != CSR_BT_BIPS_INVALID_CONNECT_TYPE)
    {
        whoHeaderLength = CSR_BT_BIPS_TARGET_LENGTH;
        who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
        CsrMemCpy(who, BasicImagingSessions[pInst->connectType], whoHeaderLength);
    }
    return (ObexUtilServConnectResponse(pInst->obexServInst,
                                        responseCode,
                                        whoHeaderLength,
                                        &who,
                                        (CsrUint32) pInst->localServerCh,
                                        0,
                                        NULL,
                                        &digestChallenge,
                                        NULL,
                                        0,
                                        NULL,
                                        authenticateResponse));
}

static CsrUint8 csrBtBipsAaGetReqHandler(BipsInstanceData           *pInst,
                                         CsrUint8                   *imageHandle,
                                         CsrUint16                  imageDescriptorLength,
                                         CsrUint8                   *imageDescriptor,
                                         const CsrUint8             *typeHeader,
                                         ObexUtilGetRequestFuncType getResultHandler,
                                         CsrBool                    srmpOn)
{
    CsrUint16 userDefinedHeaderLength;
    CsrUint8  *userDefinedHeader;
    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                        CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    if (!imageDescriptor)
    {
        userDefinedHeader = csrBtBipsBuildImageHandleHeader(imageHandle,
                                                            &userDefinedHeaderLength);
    }
    else
    {
        userDefinedHeader = csrBtBipsBuildImageHandleImageDescriptorHeader(imageHandle,
                                                                           imageDescriptorLength,
                                                                           imageDescriptor,
                                                                           &userDefinedHeaderLength);
    }
    return (ObexUtilGetRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               typeHeader,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               userDefinedHeaderLength,
                               &userDefinedHeader,
                               ObexUtilReturnSrmpValue(srmpOn),
                               getResultHandler,
                               NULL));
}

static CsrBtObexResponseCode csrBtBipsMissingHeaderHandler(BipsInstanceData *pInst)
{
    if (pInst->finalFlag || pInst->collectedInfo->bodyOffset > 0)
    { /* Not all the headers were found */
        return (CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE);
    }
    else
    { /* The headers can come in the next OBEX packet */
        if (pInst->maxPeerObexPacketSize > pInst->collectedInfo->obexPacketLength + CSR_BT_OBEX_HEADER_LENGTH)
        {
            return (CSR_BT_OBEX_CONTINUE_RESPONSE_CODE);
        }
        else
        { /* The profile API don't allow us to to collect more OBEX packet */
            return (CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE);
        }
    }
}

static CsrBool csrBtBipsCollectedInfoHandler(BipsInstanceData *pInst,
                                             CsrUint16        bodyOffset,
                                             CsrUint16        bodyLength,
                                             CsrUint16        obexResponsePacketLength,
                                             CsrUint16        obexPacketLength,
                                             CsrUint8       *obexPacket)
{
    if (!pInst->collectedInfo)
    { /* This is the first OBEX Put or Get Request packet */
        pInst->collectedInfo                            = (BipsCollectedInfo *) CsrPmemZalloc(sizeof(BipsCollectedInfo));
        pInst->collectedInfo->bodyOffset                = bodyOffset;
        pInst->collectedInfo->bodyLength                = bodyLength;
        pInst->collectedInfo->obexPacket                = obexPacket;
        pInst->collectedInfo->cloneObexPacket           = NULL;
        pInst->collectedInfo->userDefinedHeader         = NULL;
        pInst->collectedInfo->obexPacketLength          = obexPacketLength;
    }
    else
    { /* The all OBEX headers were not send in the first OBEX Request packet */
        CsrUint16  length = (CsrUint16) (pInst->collectedInfo->obexPacketLength + obexPacketLength - CSR_BT_OBEX_HEADER_LENGTH);

        if (length > pInst->maxPeerObexPacketSize)
        { /* All the headers cannot fit into a single OBEX packet */
            CsrPmemFree(obexPacket);
            return (FALSE);
        }
        else
        { /* All the headers can be copied into one OBEX packet */
            CsrUint8 *tmpObexPacket = (CsrUint8 *) CsrPmemAlloc(length);
            CsrMemCpy(tmpObexPacket, pInst->collectedInfo->obexPacket, pInst->collectedInfo->obexPacketLength);
            CsrMemCpy(&tmpObexPacket[pInst->collectedInfo->obexPacketLength], &obexPacket[CSR_BT_OBEX_HEADER_LENGTH], (obexPacketLength - CSR_BT_OBEX_HEADER_LENGTH));
            tmpObexPacket[1] = (CsrUint8) (length >> 8);
            tmpObexPacket[2] = (CsrUint8) (length);
            CsrPmemFree(pInst->collectedInfo->obexPacket);
            CsrPmemFree(obexPacket);
            pInst->collectedInfo->bodyOffset        = (CsrUint16)(bodyOffset + pInst->collectedInfo->obexPacketLength - CSR_BT_OBEX_HEADER_LENGTH);
            pInst->collectedInfo->bodyLength        = bodyLength;
            pInst->collectedInfo->obexPacketLength  = length;
            pInst->collectedInfo->obexPacket        = tmpObexPacket;
        }
    }
    if (bodyOffset > 0)
    { /* Clone the OBEX packet */
        pInst->collectedInfo->cloneObexPacket = (CsrUint8  *)(CsrPmemAlloc(pInst->collectedInfo->obexPacketLength));
        CsrMemCpy(pInst->collectedInfo->cloneObexPacket, pInst->collectedInfo->obexPacket, pInst->collectedInfo->obexPacketLength);
    }
    pInst->collectedInfo->obexResponsePacketLength = obexResponsePacketLength;
    return (TRUE);
}

static CsrUint8 csrBtBipsConnectIndSuccessHandler(BipsInstanceData *pInst)
{
    if(pInst->connectType == CSR_BT_BIPS_AA_CONNECT)
    { /* Accept it straigth from the profile */
        pInst->priServState = CSR_BT_BIPS_PRISERV_OBEX_CONNECTED_STATE;
        return (csrBtBipsConnectResponseHandler(pInst,
                                                CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                                NULL,
                                                FALSE));
    }
    else
    { /* Send an indication to the application */
        pInst->primaryPending = TRUE;
        csrBtBipsConnectIndSend(pInst,
                                pInst->maxPeerObexPacketSize,
                                pInst->length,
                                pInst->count,
                                0, /* supported functions */
                                pInst->btConnId);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}


/* The activate callback function */
static void csrBtBipsActivateCfmHandler(void            *instData,
                                        CsrUint8   serverChannel,
                                        psm_t           psm,
                                        CsrBtResultCode resultCode,
                                        CsrBtSupplier   resultSupplier)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;
    pInst->localServerCh     = serverChannel;
    pInst->localPsm          = psm;

    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The primary BIP server is activate. Note the profile is allow to
         called this function from the callback function */
        ObexUtilServConnectableStart(pInst->obexServInst,
                                     CSR_BT_BIPS_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->maxLocalObexPacketSize);
    }
    else
    { /* The primary BIP server cannot send this information to the application
         as it has no API for it. Only thing to do is to activate it again */
        csrBtBipsActivateStartHandler(pInst);
    }
    CSR_UNUSED(psm);
}

static void csrBtBipsDeactivateCfmHandler(void          *instData,
                                          CsrUint8 serverChannel,
                                          psm_t         psm)
{
    BipsInstanceData       *pInst   = (BipsInstanceData *) instData;
    CsrBtBipsDeactivateInd *pMsg    = (CsrBtBipsDeactivateInd *) CsrPmemAlloc(sizeof(CsrBtBipsDeactivateInd));
    pInst->localServerCh            = serverChannel;
    pInst->localPsm                 = psm;
    pInst->priServState             = CSR_BT_BIPS_PRISERV_IDLE_STATE;
    pInst->connectType              = CSR_BT_BIPS_INVALID_CONNECT_TYPE;
    pMsg->type                      = CSR_BT_BIPS_DEACTIVATE_IND;
    pMsg->pHandleInst               = pInst->myAppHandle;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CsrBtBipsPfreeCollectedInfo(pInst);
    CsrBtBipsPfreeAaObexPacket(pInst);
    CSR_UNUSED(psm);
}

/* The non connectable callback function */
static void csrBtBipsNonConnectableCfmHandler(void          *instData,
                                              CsrUint8 serverChannel,
                                              psm_t         psm)
{ /* The primary server has been set non-connectable in order to release the
     transport connection. Activate it again */
    BipsInstanceData      *pInst = (BipsInstanceData *) instData;

    if (pInst->priServState == CSR_BT_BIPS_PRISERV_OBEX_APP_CONNECTED_STATE)
    { /* The application have accepted the OBEX connection */
        csrBtBipsDisconnectIndSend(pInst,
                                   CSR_BT_RESULT_CODE_OBEX_TRANSPORT_CONNECTION_CLOSED_DIRECT,
                                   CSR_BT_SUPPLIER_OBEX_PROFILES);
    }
    ObexUtilServConnectableStart(pInst->obexServInst,
                                 CSR_BT_BIPS_PROFILE_DEFAULT_MTU_SIZE,
                                 pInst->maxLocalObexPacketSize);
    CSR_UNUSED(serverChannel);
    CSR_UNUSED(psm);
}

/* The connect callback functions */
static void csrBtBipsConnectIndHandler(void            *instData,
                                       CsrBtDeviceAddr deviceAddr,
                                       CsrBtConnId     btConnId,
                                       CsrUint16       maxPeerObexPacketLength,
                                       CsrUint16       obexResponsePacketLength,
                                       CsrUint32       length,
                                       CsrUint32       count,
                                       CsrUint16       targetHeaderLength,
                                       CsrUint8        *target,
                                       CsrBool         authenticated,
                                       CsrBool         challenged,
                                       CsrUint16       obexPacketLength,
                                       CsrUint8      *obexPacket)
{ /* An OBEX Connect Request packet is received */
    BipsInstanceData      *pInst       = (BipsInstanceData *) instData;
    CsrBtObexResponseCode responseCode = csrBtBipsCheckTargetHeader(pInst, target, targetHeaderLength);

    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        pInst->deviceAddr               = deviceAddr;
        pInst->maxPeerObexPacketSize    = maxPeerObexPacketLength;
        pInst->length                   = length;
        pInst->count                    = count;
        pInst->btConnId                 = btConnId;
#ifdef CSR_AMP_ENABLE
        pInst->masterBtConnId = btConnId;
#endif /* CSR_AMP_ENABLE */

        if (pInst->digestChallenge && !authenticated)
        { /* The BIPS server must have the posibility to OBEX Challenge the client */
            csrBtBipsChallengeIndSend(pInst, challenged);
        }
        else
        {
            csrBtBipsConnectIndSuccessHandler(pInst);
        }
    }
    else
    {
        csrBtBipsConnectResponseHandler(pInst,
                                        responseCode,
                                        NULL,
                                        FALSE);
    }
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtBipsAaConnectCfmHandler(void                     *instData,
                                         CsrBtResultCode          resultCode,
                                         CsrBtSupplier            resultSupplier,
                                         CsrBtDeviceAddr          deviceAddr,
                                         CsrBtConnId              btConnId,
                                         CsrUint16                maxPeerObexPacketLength,
                                         CmnCsrBtLinkedListStruct *sdpTag,
                                         CsrUint16                obexPacketLength,
                                         CsrUint8                 *obexPacket)
{
    BipsInstanceData *pInst = (BipsInstanceData *) instData;

    if(resultCode     == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
       resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    {
        CsrUint32 supportedFunctions;

        pInst->btConnId = btConnId;
#ifdef CSR_AMP_ENABLE
        /* Tell the AMPM that the slave channel is connected */
        pInst->slaveBtConnId = btConnId;
        CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 1, &pInst->slaveBtConnId);
#endif /* CSR_AMP_ENABLE */

        if (csrBtBipsAaExtractSdpInfo(pInst, sdpTag, &supportedFunctions))
        {
            pInst->primaryPending = FALSE;
            csrBtBipsConnectIndSend(pInst, maxPeerObexPacketLength,
                                    0, 0,
                                    supportedFunctions,
                                    pInst->btConnId);
        }
        else
        { /* Release the secondary OBEX connection. When the function
             csrBtBipsAaDisconnectIndHandler is called the primary
             connection will be released if it is still connected */
            pInst->clientConnected          = FALSE;
            ObexUtilDisconnectRequest(pInst->obexCliInst, TRUE, NULL);
        }
    }
    else
    { /* priServState cannot be CSR_BT_BIPS_PRISERV_OBEX_APP_CONNECTED_STATE
         because the application has not received a CSR_BT_BIPS_CONNECT_IND
         message */
        pInst->clientConnected          = FALSE;
        pInst->getStatusResponseCode    = CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE;
        if (pInst->priServState == CSR_BT_BIPS_PRISERV_OBEX_CONNECTED_STATE)
        { /* Release the primary server connection */
            pInst->priServState = CSR_BT_BIPS_PRISERV_ACTIVATED_STATE;
            ObexUtilServNonConnectableStart(pInst->obexServInst, csrBtBipsNonConnectableCfmHandler);
        }
    }
    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

/* The PUT callback functions */
static void csrBtBipsPutImgImageIndHandler(void          *instData,
                                           CsrUint16     obexResponsePacketLength,
                                           CsrBool       authenticated,
                                           CsrBool       finalBitSet,
                                           CsrUint16     bodyLength,
                                           CsrUint16     bodyOffset,
                                           CsrUint16     obexPacketLength,
                                           CsrUint8    *obexPacket)
{
    BipsInstanceData *pInst = (BipsInstanceData *) instData;

    CsrBtBipsPushPutImageFileInd *pMsg = (CsrBtBipsPushPutImageFileInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsPushPutImageFileInd));
    pInst->finalFlag      = finalBitSet;
    pMsg->type            = CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND;
    pMsg->pHandleInst     = pInst->myAppHandle;
    pMsg->connectionId    = (CsrUint32) pInst->localServerCh;
    pMsg->finalFlag       = pInst->finalFlag;
    pMsg->imageFileOffset = bodyOffset;
    pMsg->imageFileLength = bodyLength;
    pMsg->payloadLength   = obexPacketLength;
    pMsg->payload         = obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
}

static void csrBtBipsPutImgLinkedThumbnailIndHandler(void          *instData,
                                                     CsrUint16     obexResponsePacketLength,
                                                     CsrBool       authenticated,
                                                     CsrBool       finalBitSet,
                                                     CsrUint16     bodyLength,
                                                     CsrUint16     bodyOffset,
                                                     CsrUint16     obexPacketLength,
                                                     CsrUint8    *obexPacket)
{
    BipsInstanceData *pInst = (BipsInstanceData *) instData;
    CsrBtBipsPushPutLinkedThumbnailFileInd *pMsg = (CsrBtBipsPushPutLinkedThumbnailFileInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsPushPutLinkedThumbnailFileInd));
    pInst->finalFlag          = finalBitSet;
    pMsg->type                = CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND;
    pMsg->pHandleInst         = pInst->myAppHandle;
    pMsg->connectionId        = (CsrUint32) pInst->localServerCh;
    pMsg->finalFlag           = pInst->finalFlag;
    pMsg->thumbnailFileOffset = bodyOffset;
    pMsg->thumbnailFileLength = bodyLength;
    pMsg->payloadLength       = obexPacketLength;
    pMsg->payload             = obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);

}

static void csrBtBipsPutImgLinkedAttachmentIndHandler(void          *instData,
                                                      CsrUint16     obexResponsePacketLength,
                                                      CsrBool       authenticated,
                                                      CsrBool       finalBitSet,
                                                      CsrUint16     bodyLength,
                                                      CsrUint16     bodyOffset,
                                                      CsrUint16     obexPacketLength,
                                                      CsrUint8    *obexPacket)
{
    BipsInstanceData *pInst = (BipsInstanceData *) instData;

    CsrBtBipsPushPutLinkedAttachmentFileInd *pMsg = (CsrBtBipsPushPutLinkedAttachmentFileInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsPushPutLinkedAttachmentFileInd));
    pInst->finalFlag           = finalBitSet;
    pMsg->type                 = CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND;
    pMsg->pHandleInst          = pInst->myAppHandle;
    pMsg->connectionId         = (CsrUint32) pInst->localServerCh;
    pMsg->finalFlag            = pInst->finalFlag;
    pMsg->attachmentFileOffset = bodyOffset;
    pMsg->attachmentFileLength = bodyLength;
    pMsg->payloadLength        = obexPacketLength;
    pMsg->payload              = obexPacket;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
}

static void csrBtBipsPutCollectHeaderIndHandler(void          *instData,
                                                CsrUint16     obexResponsePacketLength,
                                                CsrBool       authenticated,
                                                CsrBool       finalBitSet,
                                                CsrUint16     bodyLength,
                                                CsrUint16     bodyOffset,
                                                CsrUint16     obexPacketLength,
                                                CsrUint8     *obexPacket)
{ /* This function collect all the headers and find out which PUT procedure
     is requested */

    BipsInstanceData *pInst = (BipsInstanceData *) instData;
    pInst->finalFlag        = finalBitSet;

    if (csrBtBipsCollectedInfoHandler(pInst, bodyOffset, bodyLength, obexResponsePacketLength,
                                      obexPacketLength, obexPacket))
    { /* All the headers can fit into a single reassemble OBEX packet */
        BipsCollectedInfo *info = pInst->collectedInfo;

        if (info->nameOffset == 0)
        { /* Look for the name header (0x01) */
            CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, info->obexPacket, &(info->nameOffset));
        }

        if (info->imgDescriptorOffset == 0)
        { /* Look for the Image description header (0x71) */
            CsrBtObexGetImgDescriptorOffset(CSR_BT_OBEX_RESPONSE_NORMAL, info->obexPacket,
                                            &(info->imgDescriptorOffset), &(info->imgDescriptorLength));
        }

        if (info->imgHandleOffset == 0)
        { /* Look for the Image description header (0x30) */
            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_RESPONSE_NORMAL, info->obexPacket, &(info->imgHandleOffset));
        }

        if(pInst->connectType == CSR_BT_BIPS_AA_CONNECT && !info->serviceIdFound)
        {
            info->serviceIdFound = csrBtBipsDetermineAaServiceId(info->obexPacket, pInst->serviceId);
        }

        if (info->typeOffset == 0)
        { /* Look for the type header (0x42) */
            CsrUint16 typeLength;
            CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, info->obexPacket, &info->typeOffset, &typeLength);
        }

        if (info->typeOffset > 0)
        { /* A type header is found */
            CsrBtObexResponseCode responseCode = csrBtBipsGetOperationFromType(pInst, CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION);

            switch (responseCode)
            {
                case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
                    { /* All the headers is retrieve and the procedure is ongoing */
                        break;
                    }
                case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
                    { /* More headers to come. In order to make sure that 
                         the application has the possibility to
                         use SRMP later set SRMP to CSR_BT_OBEX_SRMP_WAIT   */
                        ObexUtilServPutResponse(pInst->obexServInst,
                                                responseCode,
                                                CSR_BT_OBEX_SRMP_WAIT);
                        break;
                    }
                default:
                    { /* Something went wrong inform the client. No need to 
                         check the SRMP value because the PUT operation is now 
                         done                                                    */   
                        CsrBtBipsPfreeCollectedInfo(pInst);
                        ObexUtilServPutResponse(pInst->obexServInst,
                                                responseCode,
                                                CSR_BT_OBEX_SRMP_INVALID);
                        break;
                    }
            }
        }
        else
        { /* The Type header is still not found, check if it can come in the
             next OBEX packet. In order to make sure that the application has 
             the possibility to use SRMP later set SRMP to CSR_BT_OBEX_SRMP_WAIT */
            CsrBtObexResponseCode responseCode = csrBtBipsMissingHeaderHandler(pInst);
            ObexUtilServPutResponse(pInst->obexServInst,
                                    responseCode,
                                    CSR_BT_OBEX_SRMP_WAIT);

            if (responseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
            {
                CsrBtBipsPfreeCollectedInfo(pInst);
            }
        }
    }
    else
    { /* All the headers cannot fit into a single reassemble OBEX packet 
         No need to check the SRMP value because the PUT operation is now done */
        CsrBtBipsPfreeCollectedInfo(pInst);
        ObexUtilServPutResponse(pInst->obexServInst,
                                CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE,
                                CSR_BT_OBEX_SRMP_INVALID);
    }
    CSR_UNUSED(authenticated);
}

static void csrBtBipsPutIndHandler(void          *instData,
                                   CsrUint16     obexResponsePacketLength,
                                   CsrBool       authenticated,
                                   CsrBool       finalBitSet,
                                   CsrUint16     bodyLength,
                                   CsrUint16     bodyOffset,
                                   CsrUint16     obexPacketLength,
                                   CsrUint8    *obexPacket)
{
    /* The first Put Request packet is received. Collect all headers */
    BipsInstanceData * pInst = (BipsInstanceData *) instData;

    CsrBtBipsPfreeCollectedInfo(pInst);
    ObexUtilServSetPutContinueFunc(pInst->obexServInst,
                                   csrBtBipsPutCollectHeaderIndHandler);

    csrBtBipsPutCollectHeaderIndHandler(instData,
                                        obexResponsePacketLength,
                                        authenticated,
                                        finalBitSet,
                                        bodyLength,
                                        bodyOffset,
                                        obexPacketLength,
                                        obexPacket);
}

/* The GET callback functions */
static void csrBtBipsPushGetCapabilitiesObjectIndHandler(void          *instData,
                                                         CsrUint16     obexResponsePacketLength,
                                                         CsrBool       authenticated,
                                                         CsrBool       finalBitSet,
                                                         CsrUint16     obexPacketLength,
                                                         CsrUint8    *obexPacket)
{
    csrBtBipsConIdIndSend((BipsInstanceData *) instData, CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipsRcGetMonitoringImageObjectIndHandler(void          *instData,
                                                          CsrUint16     obexResponsePacketLength,
                                                          CsrBool       authenticated,
                                                          CsrBool       finalBitSet,
                                                          CsrUint16     obexPacketLength,
                                                          CsrUint8    *obexPacket)
{
    CsrBtBipsRcGetMonitoringImageObjectInd * pMsg = (CsrBtBipsRcGetMonitoringImageObjectInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetMonitoringImageObjectInd));

    BipsInstanceData *pInst     = (BipsInstanceData *) instData;
    pMsg->type                  = CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->connectionId          = (CsrUint32) pInst->localServerCh;
    pMsg->allowedImageLength    = obexResponsePacketLength;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipsRcGetImagePropertiesObjectIndHandler(void          *instData,
                                                          CsrUint16     obexResponsePacketLength,
                                                          CsrBool       authenticated,
                                                          CsrBool       finalBitSet,
                                                          CsrUint16     obexPacketLength,
                                                          CsrUint8    *obexPacket)
{
    CsrBtBipsRcGetImagePropertiesObjectInd * pMsg = (CsrBtBipsRcGetImagePropertiesObjectInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetImagePropertiesObjectInd));

    BipsInstanceData *pInst     = (BipsInstanceData *) instData;
    pMsg->type                  = CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->connectionId          = (CsrUint32) pInst->localServerCh;
    pMsg->allowedImageLength    = obexResponsePacketLength;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipsRcGetImageObjectIndHandler(void          *instData,
                                                CsrUint16     obexResponsePacketLength,
                                                CsrBool       authenticated,
                                                CsrBool       finalBitSet,
                                                CsrUint16     obexPacketLength,
                                                CsrUint8    *obexPacket)
{
    CsrBtBipsRcGetImageObjectInd * pMsg = (CsrBtBipsRcGetImageObjectInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetImageObjectInd));

    BipsInstanceData *pInst     = (BipsInstanceData *) instData;
    pMsg->type                  = CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->connectionId          = (CsrUint32) pInst->localServerCh;
    pMsg->allowedObjectLength   = obexResponsePacketLength;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipsRcGetLinkedThumbnailObjectIndHandler(void          *instData,
                                                          CsrUint16     obexResponsePacketLength,
                                                          CsrBool       authenticated,
                                                          CsrBool       finalBitSet,
                                                          CsrUint16     obexPacketLength,
                                                          CsrUint8    *obexPacket)
{
    CsrBtBipsRcGetLinkedThumbnailObjectInd * pMsg = (CsrBtBipsRcGetLinkedThumbnailObjectInd *)
        CsrPmemAlloc(sizeof(CsrBtBipsRcGetLinkedThumbnailObjectInd));

    BipsInstanceData *pInst     = (BipsInstanceData *) instData;
    pMsg->type                  = CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->connectionId          = (CsrUint32) pInst->localServerCh;
    pMsg->allowedObjectLength   = obexResponsePacketLength;
    CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipsGetCollectHeaderHandler(void          *instData,
                                             CsrUint16     obexResponsePacketLength,
                                             CsrBool       authenticated,
                                             CsrBool       finalBitSet,
                                             CsrUint16     obexPacketLength,
                                             CsrUint8    *obexPacket)
{ /* This function collect all the headers and find out which GET procedure
     is requested */
    BipsInstanceData        *pInst = (BipsInstanceData *) instData;

    if (csrBtBipsCollectedInfoHandler(pInst, 0, 0, obexResponsePacketLength,
                                      obexPacketLength,
                                      obexPacket))
    { /* All the headers can fit into a single reassemble OBEX packet */
        if (finalBitSet)
        { /* The final bit is set, e.g. all the headers are retrieved. Find out
             which GET procedure is started */
            CsrUint16         typeLength;
            BipsCollectedInfo *info = pInst->collectedInfo;

            /* Look for the type header (0x42) */
            if(CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, info->obexPacket, &info->typeOffset, &typeLength))
            {
                CsrBtObexResponseCode responseCode = csrBtBipsGetOperationFromType(pInst, CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION);

                if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                { /* The GET operation is running */
                    ;
                }
                else
                { /* Invalid type header received. No need to check the 
                     SRMP value because the GET operation is now done   */
                    CsrBtBipsPfreeCollectedInfo(pInst);
                    ObexUtilServGetResponse(pInst->obexServInst,
                                            responseCode,
                                            NULL,
                                            0,
                                            CSR_BT_OBEX_SRMP_INVALID);
                }
            }
            else
            { /* Cannot find the type header. No need to check the 
                 SRMP value because the GET operation is now done   */
                CsrBtBipsPfreeCollectedInfo(pInst);
                ObexUtilServGetResponse(pInst->obexServInst,
                                        CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                        NULL,
                                        0,
                                        CSR_BT_OBEX_SRMP_INVALID);
            }
        }
        else
        { /* The final bit is not set, e.g. more headers will come.
             Set SRMP value to CSR_BT_OBEX_SRMP_WAIT in order to make 
             sure the app can SRMP later  */
            CsrUint8 headerPriorityTable[6];
            CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipsGetCollectHeaderHandler);
            ObexUtilServGetWithHeaderResponse(pInst->obexServInst,
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
                                              CSR_BT_OBEX_SRMP_WAIT);
        }
    }
    else
    { /* All the headers cannot fit into a single reassemble OBEX packet
         No need to check the SRMP value because the GET operation is now done */
        CsrBtBipsPfreeCollectedInfo(pInst);
        ObexUtilServGetResponse(pInst->obexServInst,
                                CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE,
                                NULL,
                                0,
                                CSR_BT_OBEX_SRMP_INVALID);
    }
    CSR_UNUSED(authenticated);
}

static void csrBtBipsGetIndHandler(void          *instData,
                                   CsrUint16     obexResponsePacketLength,
                                   CsrBool       authenticated,
                                   CsrBool       finalBitSet,
                                   CsrUint16     obexPacketLength,
                                   CsrUint8    *obexPacket)
{ /* The first Get Request packet is received */
    BipsInstanceData * pInst = (BipsInstanceData *) instData;

    CsrBtBipsPfreeCollectedInfo(pInst);

    csrBtBipsGetCollectHeaderHandler(instData,
                                     obexResponsePacketLength,
                                     authenticated,
                                     finalBitSet,
                                     obexPacketLength,
                                     obexPacket);
}

/* Auto Archive (AA) callback functions */
static void csrBtBipsAaGetImageListHandler(BipsInstanceData      *pInst,
                                           CsrBtObexResponseCode responseCode,
                                           CsrBool               bodyHeader,
                                           CsrUint16             bodyLength,
                                           CsrUint16             bodyOffset,
                                           CsrUint8            *obexPacket,
                                           CsrUint16             obexPacketLength)
{
    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtBipsAaGetImageListInd * pMsg = (CsrBtBipsAaGetImageListInd *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetImageListInd));

        pMsg->type                      = CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND;
        pMsg->pHandleInst               = pInst->myAppHandle;
        pMsg->imageListingObjectOffset  = bodyOffset;
        pMsg->imageListingObjectLength  = bodyLength;
        pMsg->payloadLength             = obexPacketLength;
        pMsg->payload                   = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    else
    {
        CsrBtBipsAaGetImageListCfm * pMsg = (CsrBtBipsAaGetImageListCfm *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetImageListCfm));

        pMsg->type                      = CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM;
        pMsg->pHandleInst               = pInst->myAppHandle;
        pMsg->responseCode              = responseCode;
        pMsg->imageListingObjectOffset  = bodyOffset;
        pMsg->imageListingObjectLength  = bodyLength;
        pMsg->payloadLength             = obexPacketLength;
        pMsg->payload                   = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    CSR_UNUSED(bodyHeader);
}

static void csrBtBipsAaGetImageListIndHandler(void                  *instData,
                                              CsrBtObexResponseCode responseCode,
                                              CsrBool               bodyHeader,
                                              CsrUint16             bodyLength,
                                              CsrUint16             bodyOffset,
                                              CsrUint8            *obexPacket,
                                              CsrUint16             obexPacketLength)
{
    BipsInstanceData *pInst = (BipsInstanceData *) instData;
    if (pInst->aAFoundAllHeaders)
    { /* All headers is allready found */
        csrBtBipsAaGetImageListHandler(pInst,
                                       responseCode,
                                       bodyHeader,
                                       bodyLength,
                                       bodyOffset,
                                       obexPacket,
                                       obexPacketLength);
    }
    else
    { /* The img-description header has not been found */
        if (csrBtBipsMapResponseCode(TRUE, &responseCode))
        { /* The secodary server has accepted this request. Look for missing headers*/
            CsrUint16 imgDescriptorOffset;
            CsrUint16 imgDescriptorLength;

            if (pInst->nbReturnedHandles == 0)
            { /* The nbReturnedHandles has not been found, Look for it */
                csrBtBipsDetermineAaNbReturnedHandles(obexPacket, &pInst->nbReturnedHandles);
            }

            if (CsrBtObexGetImgDescriptorOffset(CSR_BT_OBEX_REQUEST,
                                                obexPacket,
                                                &imgDescriptorOffset,
                                                &imgDescriptorLength))
            { /* Found the img-description header */
                CsrUint8 *imageHandlesDescriptor = (CsrUint8 *) CsrPmemAlloc(imgDescriptorLength);
                pInst->aAFoundAllHeaders         = TRUE;
                CsrMemCpy(imageHandlesDescriptor, &obexPacket[imgDescriptorOffset], imgDescriptorLength);

                if (bodyHeader)
                { /* A body header is also included save the obex packet */
                    pInst->aACollectedInfo = (BipsAaCollectedInfo *) CsrPmemAlloc(sizeof(BipsAaCollectedInfo));
                    pInst->aACollectedInfo->aaObexPacket        = obexPacket;
                    pInst->aACollectedInfo->aaObexPacketLength  = obexPacketLength;
                    pInst->aACollectedInfo->responseCode        = responseCode;
                    pInst->aACollectedInfo->bodyLength          = bodyLength;
                    pInst->aACollectedInfo->bodyOffset          = bodyOffset;
                }
                else
                { /* No body header is included */
                    CsrPmemFree(obexPacket);
                }
                csrBtBipsAaGetImageListHeaderIndSend(pInst,
                                                     pInst->nbReturnedHandles,
                                                     imgDescriptorLength,
                                                     imageHandlesDescriptor);
            }
            else
            { /* Did not find the img-description header */
                if (bodyHeader)
                { /* The peer device do not follow the spec as all headers must
                     be send before the body header. Release the connection */
                    pInst->clientConnected          = FALSE;
                    ObexUtilDisconnectRequest(pInst->obexCliInst, FALSE, NULL);
                }
                else
                { /* Request another OBEX packet. In order to make sure that 
                     the application has the possibility to use SRMP later 
                     set SRMP to CSR_BT_OBEX_SRMP_WAIT                      */ 
                    ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                                      CSR_BT_OBEX_SRMP_WAIT);
                }
                CsrPmemFree(obexPacket);
            }
        }
        else
        { /* The secodary server has rejected this request */
            csrBtBipsAaGetImageListHandler(pInst,
                                           responseCode,
                                           bodyHeader,
                                           bodyLength,
                                           bodyOffset,
                                           obexPacket,
                                           obexPacketLength);
        }
    }
}

static void csrBtBipsAaGetCapabilitiesIndHandler(void                  *instData,
                                                 CsrBtObexResponseCode responseCode,
                                                 CsrBool               bodyHeader,
                                                 CsrUint16             bodyLength,
                                                 CsrUint16             bodyOffset,
                                                 CsrUint8            *obexPacket,
                                                 CsrUint16             obexPacketLength)
{
    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        csrBtBipsAaGetCapabilitiesIndSend((BipsInstanceData *) instData,
                                          bodyOffset,
                                          bodyLength,
                                          obexPacketLength,
                                          obexPacket);
    }
    else
    {
        csrBtBipsAaGetCapabilitiesCfmSend((BipsInstanceData *) instData,
                                          responseCode,
                                          bodyOffset,
                                          bodyLength,
                                          obexPacketLength,
                                          obexPacket);
    }
    CSR_UNUSED(bodyHeader);
}

static void csrBtBipsAaGetLinkedAttachmentIndHandler(void                  *instData,
                                                     CsrBtObexResponseCode responseCode,
                                                     CsrBool               bodyHeader,
                                                     CsrUint16             bodyLength,
                                                     CsrUint16             bodyOffset,
                                                     CsrUint8            *obexPacket,
                                                     CsrUint16             obexPacketLength)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtBipsAaGetLinkedAttachmentInd * pMsg = (CsrBtBipsAaGetLinkedAttachmentInd *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetLinkedAttachmentInd));

        pMsg->type                  = CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND;
        pMsg->pHandleInst           = pInst->myAppHandle;
        pMsg->attachmentFileOffset  = bodyOffset;
        pMsg->attachmentFileLength  = bodyLength;
        pMsg->payloadLength         = obexPacketLength;
        pMsg->payload               = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    else
    {
        CsrBtBipsAaGetLinkedAttachmentCfm * pMsg = (CsrBtBipsAaGetLinkedAttachmentCfm *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetLinkedAttachmentCfm));

        pMsg->type                  = CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM;
        pMsg->pHandleInst           = pInst->myAppHandle;
        pMsg->responseCode          = responseCode;
        pMsg->attachmentFileOffset  = bodyOffset;
        pMsg->attachmentFileLength  = bodyLength;
        pMsg->payloadLength         = obexPacketLength;
        pMsg->payload               = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    CSR_UNUSED(bodyHeader);
}

static void csrBtBipsAaGetLinkedThumbnailIndHandler(void                  *instData,
                                                    CsrBtObexResponseCode responseCode,
                                                    CsrBool               bodyHeader,
                                                    CsrUint16             bodyLength,
                                                    CsrUint16             bodyOffset,
                                                    CsrUint8            *obexPacket,
                                                    CsrUint16             obexPacketLength)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtBipsAaGetLinkedThumbnailInd * pMsg = (CsrBtBipsAaGetLinkedThumbnailInd *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetLinkedThumbnailInd));

        pMsg->type                  = CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND;
        pMsg->pHandleInst           = pInst->myAppHandle;
        pMsg->thumbnailObjectOffset = bodyOffset;
        pMsg->thumbnailObjectLength = bodyLength;
        pMsg->payloadLength         = obexPacketLength;
        pMsg->payload               = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    else
    {
        CsrBtBipsAaGetLinkedThumbnailCfm * pMsg = (CsrBtBipsAaGetLinkedThumbnailCfm *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetLinkedThumbnailCfm));

        pMsg->type                  = CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM;
        pMsg->pHandleInst           = pInst->myAppHandle;
        pMsg->responseCode          = responseCode;
        pMsg->thumbnailObjectOffset = bodyOffset;
        pMsg->thumbnailObjectLength = bodyLength;
        pMsg->payloadLength         = obexPacketLength;
        pMsg->payload               = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    CSR_UNUSED(bodyHeader);
}

static void csrBtBipsAaGetLinkedPropertiesIndHandler(void                 *instData,
                                                     CsrBtObexResponseCode responseCode,
                                                     CsrBool               bodyHeader,
                                                     CsrUint16             bodyLength,
                                                     CsrUint16             bodyOffset,
                                                     CsrUint8            *obexPacket,
                                                     CsrUint16             obexPacketLength)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtBipsAaGetImagePropertiesInd * pMsg = (CsrBtBipsAaGetImagePropertiesInd *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetImagePropertiesInd));

        pMsg->type                      = CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND;
        pMsg->pHandleInst               = pInst->myAppHandle;
        pMsg->propertiesObjectOffset    = bodyOffset;
        pMsg->propertiesObjectLength    = bodyLength;
        pMsg->payloadLength             = obexPacketLength;
        pMsg->payload                   = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    else
    {
        CsrBtBipsAaGetImagePropertiesCfm * pMsg = (CsrBtBipsAaGetImagePropertiesCfm *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetImagePropertiesCfm));

        pMsg->type                      = CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM;
        pMsg->pHandleInst               = pInst->myAppHandle;
        pMsg->responseCode              = responseCode;
        pMsg->propertiesObjectOffset    = bodyOffset;
        pMsg->propertiesObjectLength    = bodyLength;
        pMsg->payloadLength             = obexPacketLength;
        pMsg->payload                   = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    CSR_UNUSED(bodyHeader);
}

static void csrBtBipsAaGetImageIndHandler(void                  *instData,
                                          CsrBtObexResponseCode responseCode,
                                          CsrBool               bodyHeader,
                                          CsrUint16             bodyLength,
                                          CsrUint16             bodyOffset,
                                          CsrUint8            *obexPacket,
                                          CsrUint16             obexPacketLength)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtBipsAaGetImageInd * pMsg = (CsrBtBipsAaGetImageInd *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetImageInd));

        pMsg->type              = CSR_BT_BIPS_AA_GET_IMAGE_IND;
        pMsg->pHandleInst       = pInst->myAppHandle;
        pMsg->imageObjectOffset = bodyOffset;
        pMsg->imageObjectLength = bodyLength;
        pMsg->payloadLength     = obexPacketLength;
        pMsg->payload           = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    else
    {
        CsrBtBipsAaGetImageCfm * pMsg = (CsrBtBipsAaGetImageCfm *)
            CsrPmemAlloc(sizeof(CsrBtBipsAaGetImageCfm));

        pMsg->type              = CSR_BT_BIPS_AA_GET_IMAGE_CFM;
        pMsg->pHandleInst       = pInst->myAppHandle;
        pMsg->responseCode      = responseCode;
        pMsg->imageObjectOffset = bodyOffset;
        pMsg->imageObjectLength = bodyLength;
        pMsg->payloadLength     = obexPacketLength;
        pMsg->payload           = obexPacket;
        CsrBtBipsMessagePut(pInst->appHandle, pMsg);
    }
    CSR_UNUSED(bodyHeader);
}

static void csrBtBipsAaDeleteImageCfmHandler(void                  *instData,
                                             CsrBtObexResponseCode responseCode,
                                             CsrUint8            *obexPacket,
                                             CsrUint16             obexPacketLength)
{
    csrBtBipsAaDeleteImageCfmSend((BipsInstanceData *) instData, responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}


/* The ABORT callback function */
static void csrBtBipsAbortIndHandler(void              *instData,
                                     CsrUint16         descriptionOffset,
                                     CsrUint16         descriptionLength,
                                     CsrUint16         obexPacketLength,
                                     CsrUint8        *obexPacket)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;
    csrBtBipsConIdIndSend(pInst, CSR_BT_BIPS_ABORT_IND);
    CsrBtBipsPfreeCollectedInfo(pInst);
    CSR_UNUSED(descriptionOffset);
    CSR_UNUSED(descriptionLength);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipsAaAbortCfmHandler(void                  *instData,
                                       CsrBtObexResponseCode responseCode,
                                       CsrUint8            *obexPacket,
                                       CsrUint16              obexPacketLength)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;
    CsrBtBipsPfreeAaObexPacket(pInst);
    csrBtBipsAaAbortCfmSend(pInst);
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

/* The DISCONNECT callback functions */
static void csrBtBipsDisconnectIndHandler(void            *instData,
                                          CsrBtDeviceAddr deviceAddr,
                                          CsrBtReasonCode reasonCode,
                                          CsrBtSupplier   reasonSupplier,
                                          CsrUint16       obexPacketLength,
                                          CsrUint8        *obexPacket)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;

    if (pInst->clientConnected)
    { /* The secondary is still connected */
        pInst->clientConnected = FALSE;
        ObexUtilDisconnectRequest(pInst->obexCliInst, TRUE, NULL);
    }

    if (pInst->priServState == CSR_BT_BIPS_PRISERV_OBEX_APP_CONNECTED_STATE)
    { /* The application have accepted the OBEX connection */
        csrBtBipsDisconnectIndSend(pInst, reasonCode, reasonSupplier);
    }
#ifdef CSR_AMP_ENABLE
    pInst->masterBtConnId = 0;
#endif /* CSR_AMP_ENABLE */

    pInst->priServState = CSR_BT_BIPS_PRISERV_ACTIVATED_STATE;
    CsrBtBipsPfreeCollectedInfo(pInst);
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipsAaDisconnectIndHandler(void                *instData,
                                            CsrBtReasonCode     reasonCode,
                                            CsrBtSupplier       reasonSupplier,
                                            CsrUint8          *obexPacket,
                                            CsrUint16           obexPacketLength)
{
    BipsInstanceData * pInst = (BipsInstanceData *) instData;
    pInst->clientConnected   = FALSE;
#ifdef CSR_AMP_ENABLE
    /* Tell the AMPM that the slave channel is disconnected */
    pInst->slaveBtConnId = 0;
    CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 0, NULL);
#endif /* CSR_AMP_ENABLE */
    CsrBtBipsPfreeAaObexPacket(pInst);
    if (pInst->getStatusResponseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        pInst->getStatusResponseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }

    if (pInst->priServState == CSR_BT_BIPS_PRISERV_OBEX_APP_CONNECTED_STATE ||
        pInst->priServState == CSR_BT_BIPS_PRISERV_OBEX_CONNECTED_STATE)
    { /* Release the primary server connection */
        ObexUtilServNonConnectableStart(pInst->obexServInst, csrBtBipsNonConnectableCfmHandler);
    }
    CSR_UNUSED(reasonCode);
    CSR_UNUSED(reasonSupplier);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

/* The Authenticate callback functions */
static void csrBtBipsAuthenticateIndHandler(void            *instData,
                                            CsrBtDeviceAddr deviceAddr,
                                            CsrUint8        options,
                                            CsrUint16       realmLength,
                                            CsrUint8      *realm)
{
    BipsInstanceData * pInst        = (BipsInstanceData *) instData;
    pInst->primaryPending           = TRUE;
    csrBtBipsAuthenticateIndSend(pInst, deviceAddr, options, realmLength, realm);
}

static void csrBtBipsAaAuthenticateIndHandler(void            *instData,
                                              CsrBtDeviceAddr deviceAddr,
                                              CsrUint8        options,
                                              CsrUint16       realmLength,
                                              CsrUint8      *realm)
{
    BipsInstanceData * pInst        = (BipsInstanceData *) instData;
    pInst->primaryPending           = FALSE;
    csrBtBipsAuthenticateIndSend(pInst, deviceAddr, options, realmLength, realm);
}

static CsrUint8 csrBtBipsActivateStartHandler(BipsInstanceData *pInst)
{
    CsrUint16 recordLength;
    CsrUint8 *record        = csrBtBipsBuildServiceRecordHandler(pInst, &recordLength);
    CsrUint8 returnValue    = ObexUtilServActivateStart(pInst->obexServInst,
                                                        pInst->secIncoming,
                                                        (uuid16_t)CSR_BT_OBEX_IMAGING_RESPONDER_UUID,
                                                        CSR_BT_BIPS_LP_SUPERVISION_TIMEOUT,
                                                        pInst->localServerCh,
                                                        pInst->localPsm,
                                                        recordLength,
                                                        CSR_BT_BIPS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                                                        CSR_BT_BIPS_L2CAP_PSM_SDP_RECORD_INDEX,
                                                        CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                                        &record,
                                                        CSR_BT_OBJECT_TRANSFER_COD,
                                                        csrBtBipsConnectIndHandler,
                                                        csrBtBipsPutIndHandler,
                                                        csrBtBipsGetIndHandler,
                                                        NULL,
                                                        csrBtBipsAbortIndHandler,
                                                        csrBtBipsDisconnectIndHandler,
                                                        csrBtBipsAuthenticateIndHandler,
                                                        csrBtBipsActivateCfmHandler,
                                                        NULL,
                                                        pInst->maxLocalObexPacketSize,
                                                        pInst->windowSize,
                                                        pInst->localSrmEnable);


    if (returnValue == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
    {
        pInst->priServState = CSR_BT_BIPS_PRISERV_ACTIVATED_STATE;
    }
    return (returnValue);
}

static CsrBtObexResponseCode csrBtBipsAaConnectRequestHandler(BipsInstanceData *pInst)
{
    if (!pInst->clientConnected)
    { /* All mandatory headers are received */
        CsrUint8                 status;
        CsrUint16                index;
        dm_security_level_t      secOutgoing;

        CsrUint8 tempTarget[CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH]  = CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS;
        CmnCsrBtLinkedListStruct *sdpTag = NULL;
        CsrUint8                 *target = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH);

        sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_IMAGING_AUTOMATIC_ARCHIVE_UUID, &index);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SERVICE_ID_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SUPPORTED_FUNCTIONS_ATTRIBUTE_IDENTIFIER, NULL, 0);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_OBEX_L2CAP_PSM_ATTRIBUTE, NULL, 0);
#endif 
        CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);
        CsrMemCpy(target, tempTarget, CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH);
        CsrBtScSetSecOutLevel(&secOutgoing,
                              CSR_BT_SEC_DEFAULT,
                              CSR_BT_OBEX_IMAGING_INITIATOR_MANDATORY_SECURITY_OUTGOING,
                              CSR_BT_OBEX_IMAGING_INITIATOR_DEFAULT_SECURITY_OUTGOING,
                              CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                              CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

        status = ObexUtilConnectRequest(pInst->obexCliInst,
                                        &sdpTag,
                                        pInst->deviceAddr,
                                        secOutgoing,
                                        pInst->maxLocalObexPacketSize,
                                        CSR_BT_BIPS_PROFILE_DEFAULT_MTU_SIZE,
                                        &target,
                                        CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH,
                                        0,
                                        NULL,
                                        CSR_BT_BIPS_LP_SUPERVISION_TIMEOUT,
                                        NULL,
                                        0,
                                        0,
                                        NULL,
                                        0,
                                        NULL,
                                        csrBtBipsAaConnectCfmHandler,
                                        csrBtBipsAaAuthenticateIndHandler,
                                        csrBtBipsAaDisconnectIndHandler,
                                        NULL,
                                        pInst->windowSize,
                                        pInst->localSrmEnable);

        if (status == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
        { /* No need to check the SRMP value because the PUT operation is now done */
            CsrBtBipsPfreeCollectedInfo(pInst);
            ObexUtilServPutResponse(pInst->obexServInst,
                                    CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                    CSR_BT_OBEX_SRMP_INVALID);

            pInst->clientConnected          = TRUE;
            pInst->getStatusResponseCode    = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }

    }
    pInst->getStatusResponseCode = CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE;
    return (CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE);
}

static CsrBtObexResponseCode csrBtBipsGetOperationFromType(BipsInstanceData *pInst, CsrUint8 obexOperation)
{
    CsrUint8 *obexPacket     = pInst->collectedInfo->obexPacket;
    CsrUint16 obexPacketLength = pInst->collectedInfo->obexPacketLength;
    CsrUint16 typeOffset       = pInst->collectedInfo->typeOffset;

    if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_IMAGE)) &&
        (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_IMAGE, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_IMAGE))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION)
        {
            if (pInst->collectedInfo->nameOffset > 0 &&
                pInst->collectedInfo->imgDescriptorOffset > 0)
            { /* All mandatory headers are received */
                ObexUtilServSetPutContinueFunc(pInst->obexServInst, csrBtBipsPutImgImageIndHandler);
                csrBtBipsPushPutImageHeaderIndSend(pInst);
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            else
            { /* Not all mandatory headers are received */
                return (csrBtBipsMissingHeaderHandler(pInst));
            }
        }
        else
        { /* It must be a GET operation */
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipsRcGetImageObjectIndHandler);

            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST,
                                        pInst->collectedInfo->obexPacket,
                                        &(pInst->collectedInfo->imgHandleOffset));

            CsrBtObexGetImgDescriptorOffset(CSR_BT_OBEX_REQUEST,
                                            pInst->collectedInfo->obexPacket,
                                            &(pInst->collectedInfo->imgDescriptorOffset),
                                            &(pInst->collectedInfo->imgDescriptorLength));

            csrBtBipsRcGetImageHeaderIndSend(pInst);
            pInst->collectedInfo->obexPacket = NULL;
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
    }
    else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_THUMB)) &&
             (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_THUMB, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_THUMB))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION)
        {
            if (pInst->collectedInfo->imgHandleOffset > 0)
            { /* All mandatory headers are received */
                ObexUtilServSetPutContinueFunc(pInst->obexServInst, csrBtBipsPutImgLinkedThumbnailIndHandler);
                csrBtBipsPushPutLinkedThumbnailHeaderIndSend(pInst);
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            else
            { /* Not all mandatory headers are received */
                return (csrBtBipsMissingHeaderHandler(pInst));
            }
        }
        else
        { /* It must be a GET operation */
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipsRcGetLinkedThumbnailObjectIndHandler);

            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST,
                                        pInst->collectedInfo->obexPacket,
                                        &(pInst->collectedInfo->imgHandleOffset));
            csrBtBipsRcGetLinkedThumbnailHeaderIndSend(pInst);
            pInst->collectedInfo->obexPacket = NULL;
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
    }
    else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_ATTACHMENT)) &&
             (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_ATTACHMENT, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_ATTACHMENT))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION)
        {
            if (pInst->collectedInfo->imgHandleOffset > 0 &&
                pInst->collectedInfo->imgDescriptorOffset > 0)
            { /* All mandatory headers are received */
                ObexUtilServSetPutContinueFunc(pInst->obexServInst, csrBtBipsPutImgLinkedAttachmentIndHandler);
                csrBtBipsPushPutLinkedAttachmentHeaderIndSend(pInst);
                return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
            }
            else
            { /* Not all mandatory headers are received */
                return (csrBtBipsMissingHeaderHandler(pInst));
            }
        }
        else
        { /* Not supported by the GET operation */
            return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
        }
    }
    else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_ARCHIVE)) &&
             (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_ARCHIVE, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_ARCHIVE))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_PUT_REQUEST_OPERATION)
        {
            if (pInst->collectedInfo->serviceIdFound)
            { /* All mandatory headers are received */
                return (csrBtBipsAaConnectRequestHandler(pInst));
            }
            else
            { /* Not all mandatory headers are received */
                return (CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);
            }
        }
        else
        { /* Not supported by the GET operation */
            return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
        }
    }
    else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_CAPABILITIES)) &&
             (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_CAPABILITIES, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_CAPABILITIES))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION)
        {
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipsPushGetCapabilitiesObjectIndHandler);
            csrBtBipsConIdIndSend(pInst, CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND);
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
        else
        { /* Not supported by the PUT operation */
            return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
        }
    }
    else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_PROPERTIES)) &&
             (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_PROPERTIES, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_PROPERTIES))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION)
        {
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipsRcGetImagePropertiesObjectIndHandler);
            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST, pInst->collectedInfo->obexPacket, &(pInst->collectedInfo->imgHandleOffset));
            csrBtBipsRcGetImagePropertiesHeaderIndSend(pInst);
            pInst->collectedInfo->obexPacket = NULL;
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
        else
        { /* Not supported by the PUT operation */
            return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
        }
    }
    else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_MONITORING)) &&
             (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_MONITORING, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_MONITORING))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION)
        {
            CsrBool storeFlag = csrBtBipsDetermineRcStoreFlag(pInst->collectedInfo->obexPacket);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipsRcGetMonitoringImageObjectIndHandler);
            csrBtBipsRcGetMonitoringImageHeaderIndSend(pInst, storeFlag);
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
        else
        { /* Not supported by the PUT operation */
            return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
        }
    }
    else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_STATUS)) &&
             (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPS_TEXTTYPE_IMG_STATUS, sizeof(CSR_BT_BIPS_TEXTTYPE_IMG_STATUS))))
    {
        if (obexOperation == CSR_BT_OBEX_UTIL_GET_REQUEST_OPERATION)
        { /* This one is always a one shot operation but the response codes are
             uses different from other. Must therefore set
             ObexUtilServSetPutContinueFunc to csrBtBipsPutIndHandler.
             Set SRMP value to CSR_BT_OBEX_SRMP_WAIT in order to make 
             sure the app can SRMP later                                    */
            ObexUtilServSetPutContinueFunc(pInst->obexServInst, csrBtBipsPutIndHandler);
            ObexUtilServGetResponse(pInst->obexServInst,
                                    pInst->getStatusResponseCode,
                                    NULL,
                                    0,
                                    CSR_BT_OBEX_SRMP_WAIT);

            CsrBtBipsPfreeCollectedInfo(pInst);
            return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
        else
        { /* Not supported by the PUT operation */
            return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
        }
    }
    else
    {
        return (CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE);
    }
}

void CsrBtBipsPfreeCollectedInfo(BipsInstanceData *pInst)
{
    if (pInst->collectedInfo)
    {
        CsrPmemFree(pInst->collectedInfo->cloneObexPacket);
        CsrPmemFree(pInst->collectedInfo->obexPacket);
        CsrPmemFree(pInst->collectedInfo->userDefinedHeader);
        CsrPmemFree(pInst->collectedInfo);
        pInst->collectedInfo = NULL;
    }
}

void CsrBtBipsPfreeAaObexPacket(BipsInstanceData *pInst)
{
    if (pInst->aACollectedInfo)
    {
        CsrPmemFree(pInst->aACollectedInfo->aaObexPacket);
        CsrPmemFree(pInst->aACollectedInfo);
        pInst->aACollectedInfo = NULL;
    }
}

/* Common downStream message handlers */
CsrUint8 CsrBtBipsActivateReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsActivateReq *pMsg      = (CsrBtBipsActivateReq *) msg;
    pInst->appHandle                = pMsg->qId;
    pInst->upperDataCapacity        = pMsg->upperDataCapacity;
    pInst->lowerDataCapacity        = pMsg->lowerDataCapacity;
    pInst->featureSelection         = pMsg->featureSelection;
    pInst->maxLocalObexPacketSize   = pMsg->obexMaxPacketSize;
    pInst->digestChallenge          = pMsg->digestChallenge;
    pInst->localSrmEnable           = pMsg->srmEnable;
    pInst->windowSize               = pMsg->windowSize;
    return (csrBtBipsActivateStartHandler(pInst));
}

CsrUint8 CsrBtBipsDeactivateReqHandler(BipsInstanceData *pInst, void *msg)
{
    pInst->priServState          = CSR_BT_BIPS_PRISERV_IDLE_STATE;
    pInst->getStatusResponseCode = CSR_BT_OBEX_NOT_ACCEPTABLE_RESPONSE_CODE;
    if (pInst->clientConnected)
    {
        CsrBtBipsDisconnectReq *pMsg = (CsrBtBipsDisconnectReq*) msg;
        pInst->clientConnected       = FALSE;
        ObexUtilDisconnectRequest(pInst->obexCliInst, pMsg->normalDisconnect, NULL);
    }
    return (ObexUtilServDeactivateStart(pInst->obexServInst,
                                        pInst->localServerCh,
                                        pInst->localPsm,
                                        csrBtBipsDeactivateCfmHandler));
}

CsrUint8 CsrBtBipsConnectResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsConnectRes *pMsg = (CsrBtBipsConnectRes *) msg;
    if (pInst->primaryPending)
    { /* The primary OBEX connection is being establish */
        CsrUint8 returnValue = csrBtBipsConnectResponseHandler(pInst,
                                                               pMsg->responseCode,
                                                               NULL,
                                                               FALSE);

        if (returnValue == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED &&
            (pMsg->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        {
            pInst->priServState = CSR_BT_BIPS_PRISERV_OBEX_APP_CONNECTED_STATE;
        }
        return (returnValue);
    }
    else
    { /* The primary OBEX connection is allready established, and the
         secondary connection has just been setup */
        if (pInst->clientConnected)
        { /* The primary and secondary is connected */
            if (csrBtBipsMapResponseCode(TRUE, &pMsg->responseCode))
            { /* The application has accepted the connection */
                pInst->priServState = CSR_BT_BIPS_PRISERV_OBEX_APP_CONNECTED_STATE;
            }
            else
            { /* The application has rejected the connection. Release the
                 secondary OBEX connection. When the function
                 csrBtBipsAaDisconnectIndHandler is called the primary
                 connection will be released if it is still connected */
                pInst->clientConnected       = FALSE;
                return (ObexUtilDisconnectRequest(pInst->obexCliInst, TRUE, NULL));
            }
        }
        else
        { /* The secondary connnection is either being or is released */
            ;
        }
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}

CsrUint8 CsrBtBipsAuthenticateResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAuthenticateRes *pMsg = (CsrBtBipsAuthenticateRes *) msg;

    if (pInst->primaryPending)
    { /* The primary OBEX connection has been authenticated */
        return (ObexUtilServAuthenticateResponse(pInst->obexServInst,
                                                 pMsg->passwordLength,
                                                 &pMsg->password,
                                                 &pMsg->userId));
    }
    else
    { /* The secondary OBEX connection has been authenticated */
        if (pInst->clientConnected)
        { /* The primary and secondary is connected */
            return (ObexUtilCliAuthenticateResponse(pInst->obexCliInst,
                                                    pMsg->passwordLength,
                                                    &pMsg->password,
                                                    &pMsg->userId,
                                                    NULL));
        }
    }

    /* This signal is crossing with a disconnect, ignore it */
    CsrBtBipsFreeDownstreamMessageContents(CSR_BT_BIPS_PRIM, msg);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBipsDisconnectReqHandler(BipsInstanceData *pInst, void *msg)
{
    if (pInst->clientConnected)
    {
        CsrBtBipsDisconnectReq *pMsg = (CsrBtBipsDisconnectReq*) msg;
        pInst->clientConnected       = FALSE;
        return (ObexUtilDisconnectRequest(pInst->obexCliInst, pMsg->normalDisconnect, NULL));
    }
    else
    { /* Can cross, ignore it */
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}

CsrUint8 CsrBtBipsGetInstanceQidReqHandler(BipsInstanceData *pInst, void *msg)
{
    if (pInst->myAppHandle == CSR_BT_BIPS_IFACEQUEUE)
    {
        CsrBtBipsGetInstancesQidReq *pMsg = (CsrBtBipsGetInstancesQidReq*) msg;
        CsrBtBipsGetInstancesQidCfm *prim = (CsrBtBipsGetInstancesQidCfm *)
            CsrPmemAlloc(sizeof(CsrBtBipsGetInstancesQidCfm));

        prim->type                        = CSR_BT_BIPS_GET_INSTANCES_QID_CFM;
        prim->phandlesListSize            = pInst->numberOfBipsInstances;

        if (pInst->numberOfBipsInstances == 0)
        {
            prim->phandlesList = NULL;
        }
        else
        {
            BipsInstancesPool *ptr   = pInst->bipsInstances;
            CsrUint8          offset = 0;
            prim->phandlesList       = (CsrSchedQid *) CsrPmemAlloc(sizeof(CsrSchedQid) * pInst->numberOfBipsInstances);

            while(ptr)
            {
                CsrMemCpy(&(prim->phandlesList[offset]), ptr->phandles, (sizeof(CsrSchedQid) * ptr->numberInPool));
                offset += ptr->numberInPool;
                ptr = ptr->next;
            }
        }
        CsrBtBipsMessagePut(pMsg->qId, prim);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
}

CsrUint8 CsrBtBipsRegisterQidReqHandler(BipsInstanceData *pInst, void *msg)
{
    if (pInst->myAppHandle == CSR_BT_BIPS_IFACEQUEUE)
    {
        CsrBtBipsRegisterQidReq *pMsg = (CsrBtBipsRegisterQidReq*) msg;
        BipsInstancesPool       *ptr  = pInst->bipsInstances;
        BipsInstancesPool       *prev = NULL;

        while((ptr) && (ptr->numberInPool == CSR_BT_BIPS_INSTANCES_POOL_SIZE))
        {
            prev = ptr;
            ptr  = ptr->next;
        }

        if (ptr)
        {
            /* Do nothing */
        }
        else
        {
            ptr                 = (BipsInstancesPool *) CsrPmemZalloc(sizeof(BipsInstancesPool));
            ptr->numberInPool   = 0;
            ptr->next           = NULL;

            if (prev)
            {
                prev->next = ptr;
            }
            else
            {
                pInst->bipsInstances = ptr;
            }
        }
        ptr->phandles[ptr->numberInPool] = pMsg->qId;
        ptr->numberInPool                = ptr->numberInPool + 1;
        pInst->numberOfBipsInstances     = (CsrUint8) (pInst->numberOfBipsInstances + 1);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
}

CsrUint8 CsrBtBipsSecurityInReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsSecurityInReq *pMsg = (CsrBtBipsSecurityInReq*) msg;
    CsrBtBipsSecurityInCfm *prim = (CsrBtBipsSecurityInCfm*) CsrPmemAlloc(sizeof(CsrBtBipsSecurityInCfm));

    prim->type                  = CSR_BT_BIPS_SECURITY_IN_CFM;
    prim->pHandleInst           = pInst->myAppHandle;
    prim->resultSupplier        = CSR_BT_SUPPLIER_OBEX_PROFILES;
    prim->resultCode            = CsrBtScSetSecInLevel(&pInst->secIncoming, pMsg->secLevel,
                                                       CSR_BT_OBEX_IMAGING_RESPONDER_MANDATORY_SECURITY_INCOMING,
                                                       CSR_BT_OBEX_IMAGING_RESPONDER_DEFAULT_SECURITY_INCOMING,
                                                       CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                       CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);
    CsrBtBipsMessagePut(pMsg->appHandle, prim);
    ObexUtilServChangeIncomingSecurity(pInst->obexServInst, pInst->secIncoming);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBipsChallengeResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsChallengeRes *pMsg = (CsrBtBipsChallengeRes*) msg;

    if (pMsg->password)
    { /* The Server has challenge the peer client */
        ObexUtilDigestChallengeType *chal = ObexUtilReturnDigestChallengePointer(TRUE,
                                                                                 &pMsg->password,
                                                                                 pMsg->passwordLength,
                                                                                 &pMsg->userId,
                                                                                 &pMsg->realm,
                                                                                 pMsg->realmLength);

        return (csrBtBipsConnectResponseHandler(pInst,
                                                CSR_BT_OBEX_UNAUTHORIZED_CODE,
                                                chal,
                                                pMsg->authenticateResponse));
    }
    else
    { /* The Server don't want to challenge the client */
        CsrPmemFree(pMsg->userId);
        CsrPmemFree(pMsg->realm);
        return(csrBtBipsConnectIndSuccessHandler(pInst));
    }
}


/* Image Push downStream message handlers */
CsrUint8 CsrBtBipsPushGetCapabilitiesHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushGetCapabilitiesHeaderRes *pMsg = (CsrBtBipsPushGetCapabilitiesHeaderRes *) msg;
    CsrBtBipsPfreeCollectedInfo(pInst);
    if (csrBtBipsMapResponseCode(TRUE, &pMsg->responseCode))
    { /* The application has accepted this GET procedure */
        csrBtBipsConIdIndSend(pInst, CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected this GET procedure */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsPushGetCapabilitiesObjectResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushGetCapabilitiesObjectRes *pMsg = (CsrBtBipsPushGetCapabilitiesObjectRes *) msg;
    return (ObexUtilServGetResponse(pInst->obexServInst,
                                    pMsg->responseCode,
                                    &pMsg->capabilitiesObject,
                                    pMsg->capabilitiesObjectLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsPushPutImageHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushPutImageHeaderRes *pMsg = (CsrBtBipsPushPutImageHeaderRes *) msg;
    CsrBtObexResponseCode responseCode   = csrBtBipsPutHeaderResHandler(pInst, pMsg->responseCode);
    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* Send a CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND message to the application */
        csrBtBipsPutImgImageIndHandler(pInst,
                                       0,
                                       FALSE,
                                       pInst->finalFlag,
                                       pInst->collectedInfo->bodyLength,
                                       pInst->collectedInfo->bodyOffset,
                                       pInst->collectedInfo->obexPacketLength,
                                       pInst->collectedInfo->cloneObexPacket);

        pInst->collectedInfo->cloneObexPacket   = NULL;
        pInst->collectedInfo->userDefinedHeader = csrBtBipsBuildImageHandleHeader(pMsg->imageHandle,
                                                                                  &pInst->collectedInfo->userDefinedHeaderLength);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        pInst->collectedInfo->userDefinedHeader = csrBtBipsBuildImageHandleHeader(pMsg->imageHandle,
                                                                                  &pInst->collectedInfo->userDefinedHeaderLength);
        return (ObexUtilServPutResponse(pInst->obexServInst, 
                                        responseCode, 
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
    else
    { /* The application has rejected this PUT procedure */
        CsrBtBipsPfreeCollectedInfo(pInst);
        return (ObexUtilServPutResponse(pInst->obexServInst, 
                                        responseCode, 
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsPushPutImageFileResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushPutImageFileRes *pMsg = (CsrBtBipsPushPutImageFileRes *) msg;
    csrBtBipsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);

    switch (pMsg->responseCode)
    {
        case CSR_BT_OBEX_CONTINUE_RESPONSE_CODE:
        { /* Send an OBEX response packet to the Client */
            return (ObexUtilServPutResponse(pInst->obexServInst, 
                                            pMsg->responseCode, 
                                            ObexUtilReturnSrmpValue(pMsg->srmpOn)));
        }
        case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
        case CSR_BT_OBEX_PARTIAL_CONTENT_RESPONSE_CODE:
        { /* Send an OBEX response packet with the image handle header (0x30) */

            CsrUint8 headerPriorityTable[4] = {CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

            CsrUint8 returnValue = ObexUtilServPutWithHeaderResponse(pInst->obexServInst,
                                                                     pMsg->responseCode,
                                                                     NULL,
                                                                     headerPriorityTable,
                                                                     NULL,
                                                                     NULL,
                                                                     0,
                                                                     NULL,
                                                                     pInst->collectedInfo->userDefinedHeaderLength,
                                                                     &(pInst->collectedInfo->userDefinedHeader),
                                                                     ObexUtilReturnSrmpValue(pMsg->srmpOn));
            pInst->collectedInfo->userDefinedHeader = NULL;
            CsrBtBipsPfreeCollectedInfo(pInst);
            return (returnValue);
        }
        default:
        { /* The application has rejected the PUT image procedure */
            CsrBtBipsPfreeCollectedInfo(pInst);
            return (ObexUtilServPutResponse(pInst->obexServInst, 
                                            pMsg->responseCode, 
                                            ObexUtilReturnSrmpValue(pMsg->srmpOn)));
        }
    }
}

CsrUint8 CsrBtBipsPushPutLinkedThumbnailHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderRes *pMsg = (CsrBtBipsPushPutLinkedThumbnailHeaderRes *) msg;
    CsrBtObexResponseCode responseCode = csrBtBipsPutHeaderResHandler(pInst, pMsg->responseCode);

    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        csrBtBipsPutImgLinkedThumbnailIndHandler(pInst,
                                                 0,
                                                 FALSE,
                                                 pInst->finalFlag,
                                                 pInst->collectedInfo->bodyLength,
                                                 pInst->collectedInfo->bodyOffset,
                                                 pInst->collectedInfo->obexPacketLength,
                                                 pInst->collectedInfo->cloneObexPacket);
        pInst->collectedInfo->cloneObexPacket   = NULL;
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return (ObexUtilServPutResponse(pInst->obexServInst, 
                                        responseCode, 
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsPushPutLinkedThumbnailFileResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailFileRes *pMsg = (CsrBtBipsPushPutLinkedThumbnailFileRes *) msg;
    csrBtBipsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);
    CsrBtBipsPfreeCollectedInfo(pInst);
    return (ObexUtilServPutResponse(pInst->obexServInst, 
                                    pMsg->responseCode, 
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsPushPutLinkedAttachmentHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderRes *pMsg = (CsrBtBipsPushPutLinkedAttachmentHeaderRes *) msg;
    CsrBtObexResponseCode responseCode = csrBtBipsPutHeaderResHandler(pInst, pMsg->responseCode);
    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        csrBtBipsPutImgLinkedAttachmentIndHandler(pInst,
                                                  0,
                                                  FALSE,
                                                  pInst->finalFlag,
                                                  pInst->collectedInfo->bodyLength,
                                                  pInst->collectedInfo->bodyOffset,
                                                  pInst->collectedInfo->obexPacketLength,
                                                  pInst->collectedInfo->cloneObexPacket);
        pInst->collectedInfo->cloneObexPacket   = NULL;
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return (ObexUtilServPutResponse(pInst->obexServInst, 
                                        responseCode, 
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsPushPutLinkedAttachmentFileResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentFileRes *pMsg = (CsrBtBipsPushPutLinkedAttachmentFileRes *) msg;
    csrBtBipsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);
    CsrBtBipsPfreeCollectedInfo(pInst);
    return (ObexUtilServPutResponse(pInst->obexServInst, 
                                    pMsg->responseCode, 
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

/* Remote Camera (RC) downStream message handlers */
CsrUint8 CsrBtBipsRcGetMonitoringHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetMonitoringImageHeaderRes *pMsg = (CsrBtBipsRcGetMonitoringImageHeaderRes *) msg;
    CsrBtObexResponseCode responseCode           = csrBtBipsGetHeaderResHandler(pInst, pMsg->responseCode);

    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The application has accepted this GET procedure */
        CsrUint16 responsePacketLength = (CsrUint16)(pInst->collectedInfo->obexResponsePacketLength -
                                                     pInst->collectedInfo->userDefinedHeaderLength);

        pInst->collectedInfo->userDefinedHeader = csrBtBipsBuildImageHandleHeader(pMsg->imageHandle,
                                                                                  &pInst->collectedInfo->userDefinedHeaderLength);

        csrBtBipsRcGetMonitoringImageObjectIndHandler(pInst,
                                                      responsePacketLength,
                                                      FALSE,
                                                      TRUE,
                                                      0,
                                                      NULL);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected this GET procedure */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        responseCode,
                                        NULL,
                                        0,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsRcGetMonitoringObjectResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetMonitoringImageObjectRes *pMsg = (CsrBtBipsRcGetMonitoringImageObjectRes *) msg;

    if (csrBtBipsMapResponseCode(TRUE, &pMsg->responseCode))
    { /* The application has accepted this GET procedure */
        if (pInst->collectedInfo)
        { /* This is the first time the application called this function */
            CsrUint8 headerPriorityTable[6];
            CsrUint8 returnValue;
            CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
            headerPriorityTable[0] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER;

            returnValue = ObexUtilServGetWithHeaderResponse(pInst->obexServInst,
                                                            pMsg->responseCode,
                                                            NULL,
                                                            headerPriorityTable,
                                                            CSR_BT_BIPS_RC_GET_MONITORING_BODY_HEADER_INDEX,
                                                            0,
                                                            NULL,
                                                            NULL,
                                                            0,
                                                            NULL,
                                                            pInst->collectedInfo->userDefinedHeaderLength,
                                                            &(pInst->collectedInfo->userDefinedHeader),
                                                            &pMsg->monitoringObject,
                                                            pMsg->monitoringObjectLength,
                                                            ObexUtilReturnSrmpValue(pMsg->srmpOn));
            pInst->collectedInfo->userDefinedHeader = NULL;
            CsrBtBipsPfreeCollectedInfo(pInst);
            return (returnValue);
        }
        else
        { /* This is part of an multi GET operation */
            return (ObexUtilServGetResponse(pInst->obexServInst,
                                            pMsg->responseCode,
                                            &pMsg->monitoringObject,
                                            pMsg->monitoringObjectLength,
                                            ObexUtilReturnSrmpValue(pMsg->srmpOn)));
        }
    }
    else
    { /* The application has rejected this GET procedure */
        CsrBtBipsPfreeCollectedInfo(pInst);
        CsrPmemFree(pMsg->monitoringObject);
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsRcGetImagePropertiesHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetImagePropertiesHeaderRes *pMsg = (CsrBtBipsRcGetImagePropertiesHeaderRes *) msg;
    CsrBtObexResponseCode responseCode           = csrBtBipsGetHeaderResHandler(pInst, pMsg->responseCode);

    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The application has accepted this GET procedure */
        csrBtBipsRcGetImagePropertiesObjectIndHandler(pInst,
                                                      pInst->collectedInfo->obexResponsePacketLength,
                                                      FALSE,
                                                      TRUE,
                                                      0,
                                                      NULL);
        CsrBtBipsPfreeCollectedInfo(pInst);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected this GET procedure */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        responseCode,
                                        NULL,
                                        0,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsRcGetImagePropertiesObjectResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetImagePropertiesObjectRes *pMsg = (CsrBtBipsRcGetImagePropertiesObjectRes *) msg;
    return (ObexUtilServGetResponse(pInst->obexServInst,
                                    pMsg->responseCode,
                                    &pMsg->propertiesObject,
                                    pMsg->propertiesObjectLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsRcGetImageHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetImageHeaderRes *pMsg = (CsrBtBipsRcGetImageHeaderRes *) msg;
    CsrBtObexResponseCode responseCode = csrBtBipsGetHeaderResHandler(pInst, pMsg->responseCode);

    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The application has accepted this GET procedure */
        pInst->collectedInfo->lengthOfObject = pMsg->imageTotalLength;

        csrBtBipsRcGetImageObjectIndHandler(pInst,
                                            pInst->collectedInfo->obexResponsePacketLength,
                                            FALSE,
                                            TRUE,
                                            0,
                                            NULL);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected this GET procedure */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        responseCode,
                                        NULL,
                                        0,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsRcGetImageObjectResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetImageObjectRes *pMsg = (CsrBtBipsRcGetImageObjectRes *) msg;

    if (csrBtBipsMapResponseCode(TRUE, &pMsg->responseCode))
    { /* The application has accepted this GET procedure */
        if (pInst->collectedInfo)
        { /* This is the first time the application called this function */
            CsrUint8    headerPriorityTable[6];
            CsrUint32   lengthOfObject = pInst->collectedInfo->lengthOfObject;

            CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
            headerPriorityTable[0] = CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER;
            CsrBtBipsPfreeCollectedInfo(pInst);

            return(ObexUtilServGetWithHeaderResponse(pInst->obexServInst,
                                                     pMsg->responseCode,
                                                     NULL,
                                                     headerPriorityTable,
                                                     CSR_BT_BIPS_RC_GET_IMAGE_BODY_HEADER_INDEX,
                                                     lengthOfObject,
                                                     NULL,
                                                     NULL,
                                                     0,
                                                     NULL,
                                                     0,
                                                     NULL,
                                                     &pMsg->imageObject,
                                                     pMsg->imageObjectLength,
                                                     ObexUtilReturnSrmpValue(pMsg->srmpOn)));
        }
        else
        { /* This is part of an multi GET operation */
            return (ObexUtilServGetResponse(pInst->obexServInst,
                                            pMsg->responseCode,
                                            &pMsg->imageObject,
                                            pMsg->imageObjectLength,
                                            ObexUtilReturnSrmpValue(pMsg->srmpOn)));
        }
    }
    else
    { /* The application has rejected this GET procedure */
        CsrBtBipsPfreeCollectedInfo(pInst);
        CsrPmemFree(pMsg->imageObject);
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsRcGetLinkedThumbnailHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderRes *pMsg = (CsrBtBipsRcGetLinkedThumbnailHeaderRes *) msg;
    CsrBtObexResponseCode responseCode           = csrBtBipsGetHeaderResHandler(pInst, pMsg->responseCode);

    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The application has accepted this GET procedure */
        csrBtBipsRcGetLinkedThumbnailObjectIndHandler(pInst,
                                                      pInst->collectedInfo->obexResponsePacketLength,
                                                      FALSE,
                                                      TRUE,
                                                      0,
                                                      NULL);
        CsrBtBipsPfreeCollectedInfo(pInst);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected this GET procedure */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        responseCode,
                                        NULL,
                                        0,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsRcGetLinkedThumbnailObjectResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailObjectRes *pMsg = (CsrBtBipsRcGetLinkedThumbnailObjectRes *) msg;
    return (ObexUtilServGetResponse(pInst->obexServInst,
                                    pMsg->responseCode,
                                    &pMsg->thumbnailObject,
                                    pMsg->thumbnailObjectLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

/* Auto Archive (AA) downStream message handlers */
CsrUint8 CsrBtBipsAaGetImageListResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetImageListRes *pMsg = (CsrBtBipsAaGetImageListRes *) msg;
    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsAaGetImageListReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrUint16                  userDefinedHeaderLength;
    CsrBtBipsAaGetImageListReq *pMsg = (CsrBtBipsAaGetImageListReq *) msg;

    CsrUint8 * userDefinedHeader     = csrBtBipsBuildGetImageListAppParams(pMsg->nbReturnedHandles,
                                                                           pMsg->listStartOffset,
                                                                           (CsrUint8) pMsg->latestCapturedImages,
                                                                           pMsg->imageHandlesDescriptorLength,
                                                                           pMsg->imageHandlesDescriptor,
                                                                           &userDefinedHeaderLength);

    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                        CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    pInst->aAFoundAllHeaders         = FALSE;
    pInst->nbReturnedHandles         = 0;

    return (ObexUtilGetRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               (const CsrUint8*) CSR_BT_BIPS_TEXTTYPE_IMG_LISTING,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               userDefinedHeaderLength,
                               &userDefinedHeader,
                               ObexUtilReturnSrmpValue(pMsg->srmpOn),
                               csrBtBipsAaGetImageListIndHandler,
                               NULL));
}

CsrUint8 CsrBtBipsAaGetImageListHeaderResHandler(BipsInstanceData *pInst, void *msg)
{
    if (pInst->aACollectedInfo)
    { /* The obex packet were saved because it contained a body header */
        csrBtBipsAaGetImageListHandler(pInst,
                                       pInst->aACollectedInfo->responseCode,
                                       TRUE,
                                       pInst->aACollectedInfo->bodyLength,
                                       pInst->aACollectedInfo->bodyOffset,
                                       pInst->aACollectedInfo->aaObexPacket,
                                       pInst->aACollectedInfo->aaObexPacketLength);

        pInst->aACollectedInfo->aaObexPacket = NULL;
        CsrBtBipsPfreeAaObexPacket(pInst);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        CsrBtBipsAaGetImageListHeaderRes *pMsg = (CsrBtBipsAaGetImageListHeaderRes *) msg;

        return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                           ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtBipsAaGetCapabilitiesReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetCapabilitiesReq *pMsg = (CsrBtBipsAaGetCapabilitiesReq *) msg;

    CsrUint8 headerPriorityTable[5]     = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    return (ObexUtilGetRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               (const CsrUint8*) CSR_BT_BIPS_TEXTTYPE_IMG_CAPABILITIES,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               0,
                               NULL,
                               ObexUtilReturnSrmpValue(pMsg->srmpOn),
                               csrBtBipsAaGetCapabilitiesIndHandler,
                               NULL));
}

CsrUint8 CsrBtBipsAaGetCapabilitiesResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetCapabilitiesRes *pMsg = (CsrBtBipsAaGetCapabilitiesRes *) msg;
    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsAaGetImagePropertiesReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetImagePropertiesReq *pMsg = (CsrBtBipsAaGetImagePropertiesReq *) msg;
    return (csrBtBipsAaGetReqHandler(pInst,
                                     pMsg->imageHandle,
                                     0,
                                     NULL,
                                     (const CsrUint8*) CSR_BT_BIPS_TEXTTYPE_IMG_PROPERTIES,
                                     csrBtBipsAaGetLinkedPropertiesIndHandler,
                                     pMsg->srmpOn));
}

CsrUint8 CsrBtBipsAaGetImagePropertiesResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetImagePropertiesRes *pMsg = (CsrBtBipsAaGetImagePropertiesRes *) msg;
    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsAaGetImageReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetImageReq *pMsg = (CsrBtBipsAaGetImageReq *) msg;
    return (csrBtBipsAaGetReqHandler(pInst,
                                     pMsg->imageHandle,
                                     pMsg->imageDescriptorLength,
                                     pMsg->imageDescriptor,
                                     (const CsrUint8*) CSR_BT_BIPS_TEXTTYPE_IMG_IMAGE,
                                     csrBtBipsAaGetImageIndHandler,
                                     pMsg->srmpOn));
}

CsrUint8 CsrBtBipsAaGetImageResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetImageRes *pMsg = (CsrBtBipsAaGetImageRes *) msg;
    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsAaGetLinkedAttachmentReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrUint16 userDefinedHeaderLength;
    CsrBtBipsAaGetLinkedAttachmentReq *pMsg = (CsrBtBipsAaGetLinkedAttachmentReq *) msg;
    CsrUint8 * userDefinedHeader = csrBtBipsBuildImageHandleHeader(pMsg->imageHandle,
                                                                   &userDefinedHeaderLength);

    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                        CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                        CSR_BT_OBEX_UTIL_NAME_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    return (ObexUtilGetRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               (const CsrUint8*) CSR_BT_BIPS_TEXTTYPE_IMG_ATTACHMENT,
                               &pMsg->fileName,
                               NULL,
                               0,
                               NULL,
                               userDefinedHeaderLength,
                               &userDefinedHeader,
                               ObexUtilReturnSrmpValue(pMsg->srmpOn),
                               csrBtBipsAaGetLinkedAttachmentIndHandler,
                               NULL));
}

CsrUint8 CsrBtBipsAaGetLinkedAttachmentResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentRes *pMsg = (CsrBtBipsAaGetLinkedAttachmentRes *) msg;
    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsAaGetLinkedThumbnailReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailReq *pMsg = (CsrBtBipsAaGetLinkedThumbnailReq *) msg;
    return (csrBtBipsAaGetReqHandler(pInst,
                                     pMsg->imageHandle,
                                     0,
                                     NULL,
                                     (const CsrUint8*) CSR_BT_BIPS_TEXTTYPE_IMG_THUMB,
                                     csrBtBipsAaGetLinkedThumbnailIndHandler,
                                     pMsg->srmpOn));
}

CsrUint8 CsrBtBipsAaGetLinkedThumbnailResHandler(BipsInstanceData *pInst, void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailRes *pMsg = (CsrBtBipsAaGetLinkedThumbnailRes *) msg;
    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipsAaDeleteImageReqHandler(BipsInstanceData *pInst, void *msg)
{
    CsrUint16                 userDefinedHeaderLength;
    CsrBtBipsAaDeleteImageReq *pMsg  = (CsrBtBipsAaDeleteImageReq *) msg;

    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                        CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrUint8  *userDefinedHeader     =  csrBtBipsBuildImageHandleHeader(pMsg->imageHandle,
                                                                        &userDefinedHeaderLength);
    return (ObexUtilPutRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               0,
                               (const CsrUint8*) CSR_BT_BIPS_TEXTTYPE_IMG_IMAGE,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               userDefinedHeaderLength,
                               &userDefinedHeader,
                               NULL,
                               csrBtBipsAaDeleteImageCfmHandler));
}

CsrUint8 CsrBtBipsAaAbortReqHandler(BipsInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilAbortRequest(pInst->obexCliInst, NULL, csrBtBipsAaAbortCfmHandler));
}

