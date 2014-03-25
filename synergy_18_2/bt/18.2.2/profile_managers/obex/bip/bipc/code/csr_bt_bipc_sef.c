/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bipc_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_bipc_sef.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_private_lib.h"
#endif /* CSR_AMP_ENABLE */

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_BIPC_AUTO_ARCHIVE_SERVICE_ID                             0x01
#define CSR_BT_BIPC_NUMBER_OF_BASIC_IMAGING_CLIENT_SESSIONS             0x06
#define CSR_BT_BIPC_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX          0x2C
#define CSR_BT_BIPC_SERVICE_ID_SERVICE_RECORD_INDEX                     0x0C

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#define CSR_BT_BIPC_L2CAP_PSM_SDP_RECORD_INDEX                          0x61
#else
#define CSR_BT_BIPC_L2CAP_PSM_SDP_RECORD_INDEX                  CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
#endif

#define CSR_BT_BIPC_TARGET_LENGTH                                       0x10
#define CSR_BT_BIPC_IMAGE_PUSH_SUPPORTED_FUNCTION_MASK                0x000F
#define CSR_BT_BIPC_REMOTE_CAMERA_SUPPORTED_FUNCTION_MASK             0x41C0
#define CSR_BT_BIPC_AUTO_ARCHIVE_SUPPORTED_FUNCTION_MASK              0x07E1
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_TAG               0x0A
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_TAG_LENGTH        0x01
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_LENGTH            0x06
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_TAG                0x09
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_TAG_LENGTH         0x10
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_LENGTH             0x15
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG        0x01
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG_LENGTH 0x02
#define CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_LENGTH     0x07
#define CSR_BT_BIPC_APPLICATION_PARAMETER_LISTSTARTOFFSET               0x02
#define CSR_BT_BIPC_APPLICATION_PARAMETER_LATESTCAPTURESIMAGES          0x03
#define CSR_BT_BIPC_LOCAL_SUPPORTED_FEATURES                            0xCF /* All ImagePush (0xF), AutoArchive (0x40) and RemoteCamera (0x80) */
#define CSR_BT_BIPC_NO_SUPPORTED_FEATURES                             0x0000
#define CSR_BT_BIPC_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH               0x10
#define CSR_BT_BIPC_BODY_HEADER_INDEX                                   0x01

static const char CSR_BT_BIPC_TEXTTYPE_IMG_ATTACHMENT[]    = {"x-bt/img-attachment"};
static const char CSR_BT_BIPC_TEXTTYPE_IMG_THUMB[]         = {"x-bt/img-thm"};
static const char CSR_BT_BIPC_TEXTTYPE_IMG_PROPERTIES[]    = {"x-bt/img-properties"};
static const char CSR_BT_BIPC_TEXTTYPE_IMG_CAPABILITIES[]  = {"x-bt/img-capabilities"};
static const char CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE[]         = {"x-bt/img-img"};
static const char CSR_BT_BIPC_TEXTTYPE_IMG_LIST[]          = {"x-bt/img-listing"};
static const char CSR_BT_BIPC_TEXTTYPE_IMG_ARCHIVE[]       = {"x-bt/img-archive"};
static const char CSR_BT_BIPC_TEXTTYPE_IMG_MONITORING[]    = "x-bt/img-monitoring";

static const CsrUint8 CSR_BT_BIPC_BASIC_IMAGING_ARCHIVED_OBJECTS[CSR_BT_BIPC_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH] =
{0x8E, 0x61, 0xF9, 0x5E, 0x1A, 0x79, 0x11, 0xD4, 0x8E, 0xA4, 0x00, 0x80, 0x5F, 0x9B, 0x98, 0x34};

static const CsrUint8 BasicImagingClientSessions[CSR_BT_BIPC_NUMBER_OF_BASIC_IMAGING_CLIENT_SESSIONS][CSR_BT_BIPC_TARGET_LENGTH] =
{
    CSR_BT_BASIC_IMAGING_IMAGE_PUSH,
    CSR_BT_BASIC_IMAGING_IMAGE_PULL,
    CSR_BT_BASIC_IMAGING_AUTOMATIC_ARCHIVE,
    CSR_BT_BASIC_IMAGING_ADVANCED_IMAGE_PRINTING,
    CSR_BT_BASIC_IMAGING_REMOTE_CAMERA,
    CSR_BT_BASIC_IMAGING_REMOTE_DISPLAY
};

static const CsrUint8 serviceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,                     /* AttrID , ServiceClassIDList */
    0x35,0x03,                          /* 3 bytes in total DataElSeq */
    0x19,0x11,0x1C,                     /* 2 byte UUID, Service class = Imaging Automatic Archive */

    /* Service id */
    0x09, 0x00,0x03,                     /* AttrId - Service Id */
    0x1C,                                /* data descriptor 16 Byte UUID*/
    0x00, 0x00,0x00, 0x00,               /* service id value */
    0x00, 0x00,0x00, 0x00,
    0x00, 0x00,0x00, 0x00,
    0x00, 0x00,0x00, 0x00,

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
    0x25,0x19,                          /* 25 byte string */
    'I','m','a','g','i','n','g',' ','A','u','t','o','m','a','t','i','c',' ',
    'A','r','c','h','i','v','e',

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    /* OBEX-over-L2CAP */
    0x09, 0x02, 0x00,      /* 16 bit attribute */
    0x09, 0x00, 0x00,      /* 16 bit L2CAP PSM - filled in by app (index:34=0x22) */
#endif

    /* Supported Functions */
    0x09, 0x03, 0x12,                  /* AttrId, SupportedFunctions */
    0x0A, 0x00, 0x00, 0x07, 0xE1,      /* 4 byte UINT */

    /* BrowseGroupList */
    0x09, 0x00, 0x05,                  /* AttrId = BrowseGroupList */
    0x35, 0x03,                        /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,                  /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static void csrBtBipcGetNameOffset(CsrUint8 *obexPacket, CsrUint16 *offset, CsrUint16 *length)
{
    CsrUint16    index;
    CsrUint16    len;

    *offset     = 0;
    *length     = 0;
    index       = CsrBtObexNameHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, &len);

    if (index && len > CSR_BT_OBEX_HEADER_LENGTH)
    {
        *offset = (CsrUint16)(index + CSR_BT_OBEX_HEADER_LENGTH);
        *length = (CsrUint16)(len - CSR_BT_OBEX_HEADER_LENGTH);
    }
}

static CsrBool csrBtBipcResponseCodeAccepted(CsrBtObexResponseCode responseCode)
{
    CsrBtObexResponseCode rspCode = (responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK);

    if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE || rspCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        return (TRUE);
    }
    return (FALSE);
}

static CsrBool csrBtBipcCheckTargetHeader(CsrUint8        *target,
                                          CsrUint16       targetHeaderLength)
{
    if (target && targetHeaderLength == CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH)
    {
        if (!CsrMemCmp(target, CSR_BT_BIPC_BASIC_IMAGING_ARCHIVED_OBJECTS, CSR_BT_BIPC_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH))
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrUint8 * csrBtBipcBuildImageDescriptorHeader(CsrUint16 imageDescriptorLength,
                                                      CsrUint8  *imageDescriptor,
                                                      CsrUint16 *userDefinedHeaderLength)
{
    CsrUint16 length             = (CsrUint16) (imageDescriptorLength + CSR_BT_OBEX_HEADER_LENGTH);
    CsrUint8 * userDefinedHeader = (CsrUint8 *) CsrPmemAlloc(length);
    userDefinedHeader[0]         = CSR_BT_OBEX_IMG_DESCRIPTOR_HEADER;
    userDefinedHeader[1]         = (CsrUint8) ((length) >> 8);
    userDefinedHeader[2]         = (CsrUint8) (length);
    CsrMemCpy(&userDefinedHeader[3], imageDescriptor, imageDescriptorLength);
    CsrPmemFree(imageDescriptor);
    *userDefinedHeaderLength = length;
    return (userDefinedHeader);
}

static CsrUint8 * csrBtBipcBuildImageHandleHeader(CsrUint8  *imageHandle,
                                                  CsrUint16 *userDefinedHeaderLength)
{
    CsrUint8  i, t = 0;
    CsrUint16 tagLength  = (CsrUint16) ((CSR_BT_IMG_HANDLE_LENGTH + 1) * 2);
    CsrUint16 length     = (CsrUint16) (tagLength + CSR_BT_OBEX_HEADER_LENGTH);

    CsrUint8 * userDefinedHeader = (CsrUint8 *) CsrPmemAlloc(length);
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

static CsrUint8 * csrBtBipcBuildImageHandleImageDescriptorHeader(CsrUint8   *imageHandle,
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

static CsrUint8 * csrBtBipcBuildApplicationParametersStoreFlagHeader(CsrBool          storeFlag,
                                                                     CsrUint16        *applicationParametersHeaderLength)
{
    CsrUint8 * applicationHeader        = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_LENGTH);
    applicationHeader[0]                = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    applicationHeader[1]                = 0x00;
    applicationHeader[2]                = CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_LENGTH;
    applicationHeader[3]                = CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_TAG;
    applicationHeader[4]                = CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_TAG_LENGTH;
    applicationHeader[5]                = (CsrUint8) storeFlag;
    *applicationParametersHeaderLength  = CSR_BT_BIPC_APPLICATION_PARAMETERS_STORE_FLAG_LENGTH;
    return (applicationHeader);
}

static CsrUint8 * csrBtBipcBuildApplicationParametersServiceIdHeader(CsrUint16 *applicationParametersHeaderLength)
{
    CsrUint8 * applicationHeader = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_LENGTH);
    CsrMemSet(applicationHeader, 0x00, CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_LENGTH);
    applicationHeader[0]                = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    applicationHeader[1]                = 0x00;
    applicationHeader[2]                = CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_LENGTH;
    applicationHeader[3]                = CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_TAG;
    applicationHeader[4]                = CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_TAG_LENGTH;
    applicationHeader[5]                = (CsrUint8) CSR_BT_BIPC_AUTO_ARCHIVE_SERVICE_ID;
    *applicationParametersHeaderLength  = CSR_BT_BIPC_APPLICATION_PARAMETERS_SERVICEID_LENGTH;
    return (applicationHeader);
}

static CsrUint8 * csrBtBipcBuildAppParamsNbReturnedHandlersImgDescripHeader(CsrUint16  nbReturnedHandles,
                                                                            CsrUint16  imageDescriptorLength,
                                                                            CsrUint8   *imageDescriptor,
                                                                            CsrUint16  *userDefinedHeaderLength)
{
    CsrUint16 totalImageDescriptorLength = (CsrUint16) (imageDescriptorLength + CSR_BT_OBEX_HEADER_LENGTH);
    CsrUint16 length                     = (CsrUint16) (CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_LENGTH + totalImageDescriptorLength);
    CsrUint8 * userDefinedHeader         = (CsrUint8 *) CsrPmemAlloc(length);

    userDefinedHeader[0]                 = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    userDefinedHeader[1]                 = 0x00;
    userDefinedHeader[2]                 = CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_LENGTH;
    userDefinedHeader[3]                 = CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG;
    userDefinedHeader[4]                 = CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG_LENGTH;
    userDefinedHeader[5]                 = (CsrUint8) ((nbReturnedHandles) >> 8);
    userDefinedHeader[6]                 = (CsrUint8) (nbReturnedHandles);
    userDefinedHeader[7]                 = CSR_BT_OBEX_IMG_DESCRIPTOR_HEADER;
    userDefinedHeader[8]                 = (CsrUint8) ((totalImageDescriptorLength) >> 8);
    userDefinedHeader[9]                 = (CsrUint8) (totalImageDescriptorLength);
    CsrMemCpy(&userDefinedHeader[10], imageDescriptor, imageDescriptorLength);
    CsrPmemFree(imageDescriptor);
    *userDefinedHeaderLength = (CsrUint16) (length);
    return (userDefinedHeader);
}

static void csrBtBipcDetermineImgListParameters(CsrUint8 *obexData,
                                                CsrUint16  *nbReturnedHandles,
                                                CsrUint16  *listStartOffset,
                                                CsrBool    *latestCapturedImages)
{
    CsrUint16 headerIndex;
    CsrUint8 counter = 0;

    /* Set default values */
    *nbReturnedHandles      = 0;
    *listStartOffset        = 0;
    *latestCapturedImages   = FALSE;

    headerIndex = CsrBtObexAppParamIndex(obexData);

    /* If 0 is returned something went wrong */
    if (headerIndex == 0)
    {
        return;
    }

    /* Increment pointer by 3 to get past 1 byte OBEX Header Identifier and 2 bytes length field */
    headerIndex = headerIndex + 3;

    while (counter < 3)
    {
        switch (*(obexData + headerIndex))
        {
            case CSR_BT_BIPC_APPLICATION_PARAMETERS_NBRETURNEDHANDLES_TAG:
                {
                    headerIndex += 2; /* Move to the value */
                    *nbReturnedHandles = (((CsrUint16)((CsrUint8)(*(obexData + headerIndex)))) << 8) | (CsrUint16)((CsrUint8)(*(obexData + headerIndex + 1)));
                    break;
                }
            case CSR_BT_BIPC_APPLICATION_PARAMETER_LISTSTARTOFFSET:
                {
                    headerIndex += 2; /* Move to the value */
                    *listStartOffset = (((CsrUint16)((CsrUint8)(*(obexData + headerIndex)))) << 8) | (CsrUint16)((CsrUint8)(*(obexData + headerIndex + 1)));
                    break;
                }
            case CSR_BT_BIPC_APPLICATION_PARAMETER_LATESTCAPTURESIMAGES:
                {
                    headerIndex += 2; /* Move to the value */

                    /* The value is either 0x00 (false) or 0x01 (true) */
                    if (*(obexData + headerIndex) == 0x01)
                    {
                        *latestCapturedImages = TRUE;
                    }
                    else
                    {
                        *latestCapturedImages = FALSE;
                    }
                    break;
                }
            default:
                {
                    headerIndex += 2; /* Move to the value */

                    /* Apparently some value is missing. There is nothing to do about is.
                       Rely on default values. */
                    break;
                }
        }
        headerIndex += obexData[headerIndex - 1]; /* Move to new parameter by adding length field */
        counter++;
    }
    return;
}

static void csrBtBipcMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_BIPC_PRIM, msg);
}

static void csrBtBipcDisconnectIndSend(BipcInstanceData *pInst, CsrBtReasonCode reasonCode, CsrBtSupplier reasonSupplier)
{
    CsrBtBipcDisconnectInd *pMsg = CsrPmemAlloc(sizeof(CsrBtBipcDisconnectInd));

    pMsg->type              = CSR_BT_BIPC_DISCONNECT_IND;
    pMsg->pHandleInst       = pInst->myAppHandle;
    pMsg->reasonCode        = reasonCode;
    pMsg->reasonSupplier    = reasonSupplier;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcConnectCfmSend(BipcInstanceData *pInst,
                                    CsrBtResultCode  resultCode,
                                    CsrBtSupplier    resultSupplier,
                                    CsrBtConnId      btConnId)
{
    CsrBtBipcConnectCfm *pMsg   = (CsrBtBipcConnectCfm *) CsrPmemAlloc(sizeof(CsrBtBipcConnectCfm));
    pInst->connectCfmSent       = TRUE;
    pMsg->type                  = CSR_BT_BIPC_CONNECT_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    pMsg->supportedFunctions    = pInst->supportedFunctions;
    pMsg->obexPeerMaxPacketSize = pInst->maxPeerObexPacketLength;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->btConnId              = btConnId;

    CsrMemCpy(pMsg->totalImagingDataCapacity, pInst->dataCapacity, CSR_BT_DATA_CAPACITY_LEN);

    switch(pInst->feature)
    {
        case CSR_BT_IMAGE_PUSH_FEATURE:
            {
                pMsg->supportedFunctions &= CSR_BT_BIPC_IMAGE_PUSH_SUPPORTED_FUNCTION_MASK;
                break;
            }
        case CSR_BT_REMOTE_CAMERA_FEATURE:
            {
                pMsg->supportedFunctions &= CSR_BT_BIPC_REMOTE_CAMERA_SUPPORTED_FUNCTION_MASK;
                break;
            }
        case CSR_BT_AUTO_ARCHIVE_FEATURE:
            {
                pInst->supportedFunctions &= CSR_BT_BIPC_AUTO_ARCHIVE_SUPPORTED_FUNCTION_MASK;
                break;
            }
        default:
            {
                pMsg->supportedFunctions = 0x00;
                break;
            }
    }
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void CsrBtBipcAaDeactivateCfmHandler(void          *instData,
                                            CsrUint8 serverChannel,
                                            psm_t         psm)
{
    BipcInstanceData * pInst    = (BipcInstanceData *) instData;
    pInst->servState            = BIPC_SERVER_IDLE_STATE;
    CsrPmemFree(pInst->servMiscInfo);
    pInst->servMiscInfo         = NULL;

    if (pInst->clientState == BIPC_CLIENT_IDLE_STATE)
    {
        if (pInst->connectCfmSent)
        { /* The application has received a successful
             CSR_BT_BIPC_CONNECT_CFM message, send a
             CSR_BT_BIPC_DISCONNECT_IND message */
            csrBtBipcDisconnectIndSend(pInst, L2CA_DISCONNECT_NORMAL, CSR_BT_SUPPLIER_L2CAP_DISCONNECT);
        }
        else
        { /* The application has not received a
             CSR_BT_BIPC_CONNECT_CFM message yet */
            csrBtBipcConnectCfmSend(pInst,
                                    CSR_BT_RESULT_CODE_OBEX_SECONDARY_CHANNEL_ERROR,
                                    CSR_BT_SUPPLIER_OBEX_PROFILES,
                                    CSR_BT_CONN_ID_INVALID);
        }
    }
    else
    { /* Wait for the primary OBEX client connection is relased */
        ;
    }
    CSR_UNUSED(psm);
}

static void csrBtBipcReleaseChannelsHandler(BipcInstanceData *pInst)
{
    CsrBool normalDisconnect = TRUE;

    if (pInst->servState == BIPC_SERVER_ACTIVATED_STATE ||
        pInst->servState == BIPC_SERVER_ACTIVATING_STATE)
    {
        pInst->servState = BIPC_SERVER_DEACTIVATING_STATE;
        normalDisconnect = FALSE;
        ObexUtilServDeactivateStart(pInst->obexServInst,
                                    CSR_BT_NO_SERVER,
                                    L2CA_PSM_INVALID,
                                    CsrBtBipcAaDeactivateCfmHandler);
    }

    if (pInst->clientState > BIPC_CLIENT_SDP_SEARCH_STATE &&
        pInst->clientState != BIPC_CLIENT_OBEX_RELEASE_STATE)
    {
        pInst->clientState = BIPC_CLIENT_OBEX_RELEASE_STATE;
        ObexUtilDisconnectRequest(pInst->obexCliInst, normalDisconnect, NULL);
    }
}

static void csrBtBipcSecurityOutCfmSend(CsrSchedQid          appHandle,
                                        CsrSchedQid          pHandleInst,
                                        CsrBtResultCode resultCode,
                                        CsrBtSupplier   resultSupplier)
{
    CsrBtBipcSecurityOutCfm *prim = (CsrBtBipcSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtBipcSecurityOutCfm));
    prim->type                    = CSR_BT_BIPC_SECURITY_OUT_CFM;
    prim->resultCode              = resultCode;
    prim->resultSupplier          = resultSupplier;
    prim->pHandleInst             = pHandleInst;
    csrBtBipcMessagePut(appHandle, prim);
}

static void csrBtBipcAaAbortIndSend(BipcInstanceData *pInst)
{
    CsrBtBipcAaAbortInd *pMsg = (CsrBtBipcAaAbortInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaAbortInd));
    pMsg->type                = CSR_BT_BIPC_AA_ABORT_IND;
    pMsg->pHandleInst         = pInst->myAppHandle;
    pMsg->connectionId        = (CsrUint32)pInst->servServerChannel;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcAaDeleteImageHeaderIndSend(CsrSchedQid myAppHandle,
                                                CsrSchedQid appHandle,
                                                CsrUint32   connectionId,
                                                CsrUint16   imgHandleOffset,
                                                CsrUint16   obexPacketLength,
                                                CsrUint8  *obexPacket)
{
    CsrBtBipcAaDeleteImageInd *pMsg = (CsrBtBipcAaDeleteImageInd *)
        CsrPmemAlloc(sizeof(CsrBtBipcAaDeleteImageInd));
    pMsg->type                      = CSR_BT_BIPC_AA_DELETE_IMAGE_IND;
    pMsg->pHandleInst               = myAppHandle;
    pMsg->connectionId              = connectionId;
    pMsg->imageHandleOffset         = imgHandleOffset;
    pMsg->payloadLength             = obexPacketLength;
    pMsg->payload                   = obexPacket;
    csrBtBipcMessagePut(appHandle, pMsg);
}

static void csrBtBipcAaGetImageListObjectIndSend(BipcInstanceData *pInst, CsrUint16 obexResponsePacketLength)
{
    CsrBtBipcAaGetImageListObjectInd * prim = (CsrBtBipcAaGetImageListObjectInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetImageListObjectInd));
    prim->type                              = CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND;
    prim->connectionId                      = (CsrUint32) pInst->servServerChannel;
    prim->allowedObjectLength               = obexResponsePacketLength;
    prim->pHandleInst                       = pInst->myAppHandle;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetCapabilitiesObjectIndSend(BipcInstanceData *pInst)
{
    CsrBtBipcAaGetCapabilitiesObjectInd * prim = (CsrBtBipcAaGetCapabilitiesObjectInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetCapabilitiesObjectInd));
    prim->type                                 = CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND;
    prim->pHandleInst                          = pInst->myAppHandle;
    prim->connectionId                         = (CsrUint32) pInst->servServerChannel;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetImagePropertiesObjectIndSend(BipcInstanceData *pInst, CsrUint16 obexResponsePacketLength)
{
    CsrBtBipcAaGetImagePropertiesObjectInd * prim = (CsrBtBipcAaGetImagePropertiesObjectInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetImagePropertiesObjectInd));
    prim->type                                    = CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND;
    prim->pHandleInst                             = pInst->myAppHandle;
    prim->connectionId                            = (CsrUint32) pInst->servServerChannel;
    prim->allowedImageLength                      = obexResponsePacketLength;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetImageObjectIndSend(BipcInstanceData *pInst, CsrUint16 obexResponsePacketLength)
{
    CsrBtBipcAaGetImageObjectInd * prim = (CsrBtBipcAaGetImageObjectInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetImageObjectInd));
    prim->type                          = CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND;
    prim->pHandleInst                   = pInst->myAppHandle;
    prim->connectionId                  = (CsrUint32) pInst->servServerChannel;
    prim->allowedObjectLength           = obexResponsePacketLength;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetLinkedThumbnailObjectIndSend(BipcInstanceData *pInst, CsrUint16 obexResponsePacketLength)
{
    CsrBtBipcAaGetLinkedThumbnailObjectInd * prim = (CsrBtBipcAaGetLinkedThumbnailObjectInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetLinkedThumbnailObjectInd));
    prim->type                                    = CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND;
    prim->connectionId                            = (CsrUint32) pInst->servServerChannel;
    prim->pHandleInst                             = pInst->myAppHandle;
    prim->allowedObjectLength                     = obexResponsePacketLength;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetLinkedAttachmentObjectIndSend(BipcInstanceData *pInst, CsrUint16 obexResponsePacketLength)
{
    CsrBtBipcAaGetLinkedAttachmentObjectInd * prim = (CsrBtBipcAaGetLinkedAttachmentObjectInd *)
        CsrPmemAlloc(sizeof(CsrBtBipcAaGetLinkedAttachmentObjectInd));
    prim->type                                     = CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND;
    prim->pHandleInst                              = pInst->myAppHandle;
    prim->connectionId                             = (CsrUint32) pInst->servServerChannel;
    prim->allowedObjectLength                      = obexResponsePacketLength;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetImageListHeaderIndSend(BipcInstanceData *pInst,
                                                 CsrUint16        imgDescriptorOffset,
                                                 CsrUint16        imgDescriptorLength,
                                                 CsrUint16        nbReturnedHandles,
                                                 CsrUint16        listStartOffset,
                                                 CsrBool          latestCapturedImages,
                                                 CsrUint16        obexResponsePacketLength,
                                                 CsrUint16        obexPacketLength,
                                                 CsrUint8       *obexPacket)
{
    CsrBtBipcAaGetImageListHeaderInd * prim = (CsrBtBipcAaGetImageListHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipcAaGetImageListHeaderInd));
    prim->type                              = CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND;
    prim->pHandleInst                       = pInst->myAppHandle;
    prim->connectionId                      = (CsrUint32) pInst->servServerChannel;
    prim->nbReturnedHandles                 = nbReturnedHandles;
    prim->listStartOffset                   = listStartOffset;
    prim->latestCapturedImages              = latestCapturedImages;
    prim->allowedDescriptorLength           = obexResponsePacketLength;
    prim->imageDescriptorOffset             = imgDescriptorOffset;
    prim->imageDescriptorLength             = imgDescriptorLength;
    prim->payloadLength                     = obexPacketLength;
    prim->payload                           = obexPacket;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetCapabilitiesHeaderIndSend(BipcInstanceData *pInst)
{
    CsrBtBipcAaGetCapabilitiesHeaderInd * prim = (CsrBtBipcAaGetCapabilitiesHeaderInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetCapabilitiesHeaderInd));
    prim->type                                 = CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND;
    prim->pHandleInst                          = pInst->myAppHandle;
    prim->connectionId                         = (CsrUint32) pInst->servServerChannel;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetImagePropertiesHeaderIndSend(BipcInstanceData *pInst,
                                                       CsrUint16        imgHandleOffset,
                                                       CsrUint16        obexPacketLength,
                                                       CsrUint8       *obexPacket)
{
    CsrBtBipcAaGetImagePropertiesHeaderInd * prim  = (CsrBtBipcAaGetImagePropertiesHeaderInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetImagePropertiesHeaderInd));
    prim->type                                     = CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND;
    prim->pHandleInst                              = pInst->myAppHandle;
    prim->connectionId                             = (CsrUint32) pInst->servServerChannel;
    prim->imageHandleOffset                        = imgHandleOffset;
    prim->payloadLength                            = obexPacketLength;
    prim->payload                                  = obexPacket;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetImageHeaderIndSend(BipcInstanceData *pInst,
                                             CsrUint16        imgHandleOffset,
                                             CsrUint16        imgDescriptorOffset,
                                             CsrUint16        imgDescriptorLength,
                                             CsrUint16        obexPacketLength,
                                             CsrUint8       *obexPacket)
{
    CsrBtBipcAaGetImageHeaderInd * prim = (CsrBtBipcAaGetImageHeaderInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetImageHeaderInd));
    prim->type                          = CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND;
    prim->pHandleInst                   = pInst->myAppHandle;
    prim->connectionId                  = (CsrUint32) pInst->servServerChannel;
    prim->imageHandleOffset             = imgHandleOffset;
    prim->descriptorOffset              = imgDescriptorOffset;
    prim->descriptorLength              = imgDescriptorLength;
    prim->payloadLength                 = obexPacketLength;
    prim->payload                       = obexPacket;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetLinkedThumbnailHeaderIndSend(BipcInstanceData *pInst,
                                                       CsrUint16        imgHandleOffset,
                                                       CsrUint16        obexPacketLength,
                                                       CsrUint8       *obexPacket)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderInd * prim = (CsrBtBipcAaGetLinkedThumbnailHeaderInd *) CsrPmemAlloc(sizeof(CsrBtBipcAaGetLinkedThumbnailHeaderInd));
    prim->type                                    = CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND;
    prim->pHandleInst                             = pInst->myAppHandle;
    prim->connectionId                            = (CsrUint32) pInst->servServerChannel;
    prim->imageHandleOffset                       = imgHandleOffset;
    prim->payloadLength                           = obexPacketLength;
    prim->payload                                 = obexPacket;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAaGetLinkedAttachmentHeaderIndSend(BipcInstanceData *pInst,
                                                        CsrUint16        imgHandleOffset,
                                                        CsrUint16        nameOffset,
                                                        CsrUint16        nameLength,
                                                        CsrUint16        obexPacketLength,
                                                        CsrUint8       *obexPacket)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderInd * prim = (CsrBtBipcAaGetLinkedAttachmentHeaderInd *)
        CsrPmemAlloc(sizeof(CsrBtBipcAaGetLinkedAttachmentHeaderInd));

    prim->type                                     = CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND;
    prim->pHandleInst                       = pInst->myAppHandle;
    prim->connectionId                             = (CsrUint32) pInst->servServerChannel;
    prim->imageHandleOffset                        = imgHandleOffset;
    prim->attachmentNameOffset                     = nameOffset;
    prim->attachmentNameLength                     = nameLength;
    prim->payloadLength                            = obexPacketLength;
    prim->payload                                  = obexPacket;
    csrBtBipcMessagePut(pInst->appHandle, prim);
}

static void csrBtBipcAuthenticateIndSend(CsrSchedQid hApp, CsrSchedQid pHandleInst, CsrBtDeviceAddr deviceAddr, CsrUint8 options)
{
    CsrBtBipcAuthenticateInd *pMsg = (CsrBtBipcAuthenticateInd *) CsrPmemAlloc(sizeof(CsrBtBipcAuthenticateInd));

    pMsg->type          = CSR_BT_BIPC_AUTHENTICATE_IND;
    pMsg->pHandleInst   = pHandleInst;
    pMsg->deviceAddr    = deviceAddr;
    pMsg->options       = options;
    pMsg->realmLength   = 0;
    pMsg->realm         = NULL;
    csrBtBipcMessagePut(hApp, pMsg);
}

static void csrBtBipcGetRemoteFeaturesCfmSend(BipcInstanceData *pInst,
                                              CsrUint16        supportedFeatures)
{
    CsrBtBipcGetRemoteFeaturesCfm *pMsg = (CsrBtBipcGetRemoteFeaturesCfm *) CsrPmemAlloc(sizeof(CsrBtBipcGetRemoteFeaturesCfm));
    pMsg->type                          = CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM;
    pMsg->pHandleInst                   = pInst->myAppHandle;
    pMsg->supportedFeatures             = (CsrUint16)(supportedFeatures & CSR_BT_BIPC_LOCAL_SUPPORTED_FEATURES);
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcAbortCfmSend(BipcInstanceData *pInst)
{
    CsrBtBipcAbortCfm *pMsg  = (CsrBtBipcAbortCfm *) CsrPmemAlloc(sizeof(CsrBtBipcAbortCfm));
    pMsg->type               = CSR_BT_BIPC_ABORT_CFM;
    pMsg->pHandleInst        = pInst->myAppHandle;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcImagePushGetCapabilitiesIndSend(BipcInstanceData *pInst,
                                                     CsrUint16        bodyLength,
                                                     CsrUint16        bodyOffset,
                                                     CsrUint8       *obexPacket,
                                                     CsrUint16        obexPacketLength)
{
    CsrBtBipcPushGetCapabilitiesInd *pMsg = (CsrBtBipcPushGetCapabilitiesInd *) CsrPmemAlloc(sizeof(CsrBtBipcPushGetCapabilitiesInd));
    pMsg->type                            = CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND;
    pMsg->pHandleInst                     = pInst->myAppHandle;
    pMsg->capabilitiesObjectOffset        = bodyOffset;
    pMsg->capabilitiesObjectLength        = bodyLength;
    pMsg->payload                         = obexPacket;
    pMsg->payloadLength                   = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcImagePushPutImageFileIndSend(BipcInstanceData   *pInst,
                                                  CsrUint16          fileLength)
{
    CsrBtBipcPushPutImageFileInd *pMsg = (CsrBtBipcPushPutImageFileInd *) CsrPmemAlloc(sizeof(CsrBtBipcPushPutImageFileInd));

    pMsg->type              = CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND;
    pMsg->pHandleInst       = pInst->myAppHandle;
    pMsg->imageFileLength   = fileLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcImagePushPutThumbnailIndSend(BipcInstanceData   *pInst,
                                                  CsrUint16          thumbnailLength)
{
    CsrBtBipcPushPutThumbnailFileInd *pMsg = (CsrBtBipcPushPutThumbnailFileInd *) CsrPmemAlloc(sizeof(CsrBtBipcPushPutThumbnailFileInd));
    pMsg->type                  = CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->thumbnailFileLength   = thumbnailLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcImagePushPutImageCfmSend(BipcInstanceData      *pInst,
                                              CsrBtObexResponseCode responseCode,
                                              CsrUint8              *imgHandle)
{
    CsrBtBipcPushPutImageCfm *pMsg = (CsrBtBipcPushPutImageCfm *) CsrPmemAlloc(sizeof(CsrBtBipcPushPutImageCfm));

    pMsg->type                     = CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM;
    pMsg->responseCode             = responseCode;
    pMsg->pHandleInst              = pInst->myAppHandle;
    CsrStrNCpy((char*)pMsg->imageHandle, (char*)imgHandle, CSR_BT_IMG_HANDLE_LENGTH);
    pMsg->imageHandle[CSR_BT_IMG_HANDLE_LENGTH] = '\0';
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcImagePushPutAttachmentIndSend(BipcInstanceData * pInst,
                                                   CsrUint16 attachFileLength)
{
    CsrBtBipcPushPutAttachmentFileInd *pMsg = (CsrBtBipcPushPutAttachmentFileInd *) CsrPmemAlloc(sizeof(CsrBtBipcPushPutAttachmentFileInd));

    pMsg->type                  = CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND;
    pMsg->pHandleInst           = pInst->myAppHandle;
    pMsg->attachmentFileLength  = attachFileLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcImagePushPutAttachmentCfmSend(BipcInstanceData      * pInst,
                                                   CsrBtObexResponseCode responseCode)
{
    CsrBtBipcPushPutAttachmentCfm *pMsg = (CsrBtBipcPushPutAttachmentCfm *) CsrPmemAlloc(sizeof(CsrBtBipcPushPutAttachmentCfm));

    pMsg->type          = CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM;
    pMsg->pHandleInst   = pInst->myAppHandle;
    pMsg->responseCode  = responseCode;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetMonitoringImageHeaderIndSend(BipcInstanceData      *pInst,
                                                       CsrUint8              *imgHandle)
{
    CsrBtBipcRcGetMonitoringImageHeaderInd *pMsg = (CsrBtBipcRcGetMonitoringImageHeaderInd *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetMonitoringImageHeaderInd));

    pMsg->type             = CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND;
    pMsg->pHandleInst      = pInst->myAppHandle;
    CsrStrNCpy((char*)pMsg->imageHandle, (char*)imgHandle, CSR_BT_IMG_HANDLE_LENGTH);
    pMsg->imageHandle[CSR_BT_IMG_HANDLE_LENGTH] = '\0';
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetMonitoringImageFileIndSend(BipcInstanceData      *pInst,
                                                     CsrUint16             bodyLength,
                                                     CsrUint16             bodyOffset,
                                                     CsrUint8            *obexPacket,
                                                     CsrUint16             obexPacketLength)
{
    CsrBtBipcRcGetMonitoringImageFileInd *pMsg = (CsrBtBipcRcGetMonitoringImageFileInd *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetMonitoringImageFileInd));

    pMsg->type             = CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND;
    pMsg->pHandleInst      = pInst->myAppHandle;
    pMsg->imageFileOffset  = bodyOffset;
    pMsg->imageFileLength  = bodyLength;
    pMsg->payload          = obexPacket;
    pMsg->payloadLength    = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetMonitoringImageCfmSend(BipcInstanceData      *pInst,
                                                 CsrBtObexResponseCode responseCode)
{
    CsrBtBipcRcGetMonitoringImageCfm *pMsg = (CsrBtBipcRcGetMonitoringImageCfm *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetMonitoringImageCfm));

    pMsg->type             = CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM;
    pMsg->pHandleInst      = pInst->myAppHandle;
    pMsg->responseCode     = responseCode;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetImagePropertiesIndSend(BipcInstanceData      *pInst,
                                                 CsrUint16             bodyLength,
                                                 CsrUint16             bodyOffset,
                                                 CsrUint8            *obexPacket,
                                                 CsrUint16             obexPacketLength)
{
    CsrBtBipcRcGetImagePropertiesInd *pMsg = (CsrBtBipcRcGetImagePropertiesInd *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetImagePropertiesInd));

    pMsg->type                      = CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->propertiesObjectOffset    = bodyOffset;
    pMsg->propertiesObjectLength    = bodyLength;
    pMsg->payload                   = obexPacket;
    pMsg->payloadLength             = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetImagePropertiesCfmSend(BipcInstanceData      *pInst,
                                                 CsrBtObexResponseCode responseCode,
                                                 CsrUint16             bodyLength,
                                                 CsrUint16             bodyOffset,
                                                 CsrUint8            *obexPacket,
                                                 CsrUint16             obexPacketLength)
{
    CsrBtBipcRcGetImagePropertiesCfm *pMsg = (CsrBtBipcRcGetImagePropertiesCfm *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetImagePropertiesCfm));

    pMsg->type                      = CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->responseCode              = responseCode;
    pMsg->propertiesObjectOffset    = bodyOffset;
    pMsg->propertiesObjectLength    = bodyLength;
    pMsg->payload                   = obexPacket;
    pMsg->payloadLength             = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetImageIndSend(BipcInstanceData      *pInst,
                                       CsrUint16             bodyLength,
                                       CsrUint16             bodyOffset,
                                       CsrUint8            *obexPacket,
                                       CsrUint16             obexPacketLength)
{
    CsrBtBipcRcGetImageInd *pMsg = (CsrBtBipcRcGetImageInd *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetImageInd));

    pMsg->type                      = CSR_BT_BIPC_RC_GET_IMAGE_IND;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->imageObjectOffset         = bodyOffset;
    pMsg->imageObjectLength         = bodyLength;
    pMsg->payload                   = obexPacket;
    pMsg->payloadLength             = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetImageCfmSend(BipcInstanceData      *pInst,
                                       CsrBtObexResponseCode responseCode,
                                       CsrUint16             bodyLength,
                                       CsrUint16             bodyOffset,
                                       CsrUint8            *obexPacket,
                                       CsrUint16             obexPacketLength)
{
    CsrBtBipcRcGetImageCfm *pMsg = (CsrBtBipcRcGetImageCfm *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetImageCfm));

    pMsg->type                      = CSR_BT_BIPC_RC_GET_IMAGE_CFM;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->responseCode              = responseCode;
    pMsg->imageObjectOffset         = bodyOffset;
    pMsg->imageObjectLength         = bodyLength;
    pMsg->payload                   = obexPacket;
    pMsg->payloadLength             = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetLinkedThumbnailIndSend(BipcInstanceData      *pInst,
                                                 CsrUint16             bodyLength,
                                                 CsrUint16             bodyOffset,
                                                 CsrUint8            *obexPacket,
                                                 CsrUint16             obexPacketLength)
{
    CsrBtBipcRcGetLinkedThumbnailInd *pMsg = (CsrBtBipcRcGetLinkedThumbnailInd *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetLinkedThumbnailInd));

    pMsg->type                      = CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->thumbnailObjectOffset     = bodyOffset;
    pMsg->thumbnailObjectLength     = bodyLength;
    pMsg->payload                   = obexPacket;
    pMsg->payloadLength             = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcRcGetLinkedThumbnailCfmSend(BipcInstanceData      *pInst,
                                                 CsrBtObexResponseCode responseCode,
                                                 CsrUint16             bodyLength,
                                                 CsrUint16             bodyOffset,
                                                 CsrUint8            *obexPacket,
                                                 CsrUint16             obexPacketLength)
{
    CsrBtBipcRcGetLinkedThumbnailCfm *pMsg = (CsrBtBipcRcGetLinkedThumbnailCfm *) CsrPmemAlloc(sizeof(CsrBtBipcRcGetLinkedThumbnailCfm));

    pMsg->type                      = CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM;
    pMsg->pHandleInst               = pInst->myAppHandle;
    pMsg->responseCode              = responseCode;
    pMsg->thumbnailObjectOffset     = bodyOffset;
    pMsg->thumbnailObjectLength     = bodyLength;
    pMsg->payload                   = obexPacket;
    pMsg->payloadLength             = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcStartArchiveResultHandler(void                  *instData,
                                               CsrBtObexResponseCode responseCode,
                                               CsrUint8            *obexPacket,
                                               CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The remote OBEX server accepted to start draining this client
         image files. Just wait for the secondary server channel to be
         connected */
        ;
    }
    else
    { /* The remote OBEX server rejected to start draining this client
         image files */
        csrBtBipcReleaseChannelsHandler(pInst);
    }
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

CsrUint8 csrBtBipcStartArchiveFunctionSend(BipcInstanceData * pInst)
{
    CsrUint16 appParametersHeaderLength;

    CsrUint8 headerPriorityTable[6]   = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                         CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER,
                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrUint8 * appParametersHeader    = csrBtBipcBuildApplicationParametersServiceIdHeader(&appParametersHeaderLength);

    return (ObexUtilPutRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               0,
                               (const CsrUint8*)(CSR_BT_BIPC_TEXTTYPE_IMG_ARCHIVE),
                               NULL,
                               NULL,
                               appParametersHeaderLength,
                               &appParametersHeader,
                               0,
                               NULL,
                               NULL,
                               csrBtBipcStartArchiveResultHandler));
}

static void csrBtBipcActivateAaCfmHandler(void            *instData,
                                          CsrUint8   serverChannel,
                                          psm_t           psm,
                                          CsrBtResultCode resultCode,
                                          CsrBtSupplier   resultSupplier)
{ /* The Primary channel must be connected, if not the deactivation
     procedure is allready called, which means that this cannot
     occurred */
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The BIPC Auto Archive server is activate and the primary
         channel must be connected. Set the server connectable and
         send the StartArchive command */
        pInst->servServerChannel    = serverChannel;
        pInst->servState            = BIPC_SERVER_ACTIVATED_STATE;
        ObexUtilServConnectableStart(pInst->obexServInst,
                                     CSR_BT_BIPC_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->originalMaxPacketSize);

        if (csrBtBipcStartArchiveFunctionSend(pInst) == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
        {
            ;
        }
        else
        { /* The OBEX PUT command failed */
            csrBtBipcReleaseChannelsHandler(pInst);
        }
    }
    else
    { /* The BIPC Auto Archive server could not be activated */
        pInst->servState = BIPC_SERVER_IDLE_STATE;
        csrBtBipcReleaseChannelsHandler(pInst);
    }
    CSR_UNUSED(psm);
}

static CsrUint8 csrBtBipcAaGetWithHeadersResponseHandler(BipcInstanceData            * pInst,
                                                         CsrUint8                    firstHeader,
                                                         CsrUint8                    secondHeader,
                                                         CsrUint8                    bodyHeaderPriorityIndex,
                                                         ObexUtilDigestChallengeType *digestChallenge,
                                                         CsrBtObexResponseCode       responseCode,
                                                         CsrUint32                   lengthOfObject,
                                                         CsrUint16                   appParametersHeaderLength,
                                                         CsrUint8                    *appParametersHeader,
                                                         CsrUint16                   userDefinedHeaderLength,
                                                         CsrUint8                    *userDefinedHeader,
                                                         CsrUint16                   bodyLength,
                                                         CsrUint8                    *body,
                                                         CsrBool                     srmpOn)
{
    CsrUint8 headerPriorityTable[6];

    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
    headerPriorityTable[0] = firstHeader;
    headerPriorityTable[1] = secondHeader;

    return (ObexUtilServGetWithHeaderResponse(pInst->obexServInst,
                                              responseCode,
                                              &digestChallenge,
                                              headerPriorityTable,
                                              bodyHeaderPriorityIndex,
                                              lengthOfObject,
                                              NULL,
                                              NULL,
                                              appParametersHeaderLength,
                                              &appParametersHeader,
                                              userDefinedHeaderLength,
                                              &userDefinedHeader,
                                              &body,
                                              bodyLength,
                                              ObexUtilReturnSrmpValue(srmpOn)));
}

static void csrBtBipcAaDisconnectIndHandler(void            *instData,
                                            CsrBtDeviceAddr deviceAddr,
                                            CsrBtReasonCode reasonCode,
                                            CsrBtSupplier   reasonSupplier,
                                            CsrUint16       obexPacketLength,
                                            CsrUint8        *obexPacket)
{ /* The Primary channel must be connected, if not the deactivation
     procedure is allready called, which means that this cannot
     occurred */
    BipcInstanceData * pInst     = (BipcInstanceData *) instData;
#ifdef CSR_AMP_ENABLE
    /* Tell the AMPM that the slave channel is disconnected */
    pInst->slaveBtConnId = 0;
    CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 0, NULL);
#endif /* CSR_AMP_ENABLE */

    csrBtBipcReleaseChannelsHandler(pInst);
    CsrPmemFree(pInst->servMiscInfo);
    pInst->servMiscInfo = NULL;
    CSR_UNUSED(reasonCode);
    CSR_UNUSED(reasonSupplier);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaConnectIndHandler(void            *instData,
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
                                         CsrUint8        *obexPacket)
{
    CsrUint8         returnValue;
    BipcInstanceData *pInst = (BipcInstanceData *) instData;

    if (CsrBtBdAddrEq(&deviceAddr, &pInst->deviceAddr))
    {
        CsrBtObexResponseCode responseCode           = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
        CsrUint8              *who                   = NULL;
        CsrUint16             whoHeaderLength        = 0;

        pInst->btConnId = btConnId;

        if (csrBtBipcCheckTargetHeader(target, targetHeaderLength))
        {
            whoHeaderLength = CSR_BT_BIPC_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH;
            who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
            responseCode    = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            CsrMemCpy(who, CSR_BT_BIPC_BASIC_IMAGING_ARCHIVED_OBJECTS, whoHeaderLength);
            csrBtBipcConnectCfmSend(pInst,
                                    CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                    CSR_BT_SUPPLIER_IRDA_OBEX,
                                    pInst->btConnId);
        }
#ifdef CSR_AMP_ENABLE
        if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        {
            /* Tell the AMPM that this is a slave channel belonging to the AA-master channel */
            pInst->slaveBtConnId = btConnId;
            CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 1, &pInst->slaveBtConnId);
        }
#endif /* CSR_AMP_ENABLE */
        responseCode = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

        returnValue  = ObexUtilServConnectResponse(pInst->obexServInst,
                                                   responseCode,
                                                   whoHeaderLength,
                                                   &who,
                                                   (CsrUint32) pInst->servServerChannel,
                                                   0,
                                                   NULL,
                                                   NULL,
                                                   NULL,
                                                   0,
                                                   NULL,
                                                   FALSE);
    }
    else
    {
        returnValue = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    }

    if (returnValue == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        csrBtBipcReleaseChannelsHandler(pInst);
    }
    CSR_UNUSED(length);
    CSR_UNUSED(count);
    CSR_UNUSED(maxPeerObexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtBipcAaAbortIndHandler(void              *instData,
                                       CsrUint16         descriptionOffset,
                                       CsrUint16         descriptionLength,
                                       CsrUint16         obexPacketLength,
                                       CsrUint8        *obexPacket)
{
    BipcInstanceData * pInst  = (BipcInstanceData *) instData;
    csrBtBipcAaAbortIndSend(pInst);
    CsrPmemFree(pInst->servMiscInfo);
    pInst->servMiscInfo = NULL;
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(descriptionOffset);
    CSR_UNUSED(descriptionLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaAuthenticateIndHandler(void            *instData,
                                              CsrBtDeviceAddr deviceAddr,
                                              CsrUint8        options,
                                              CsrUint16       realmLength,
                                              CsrUint8      *realm)
{
    BipcInstanceData * pInst    = (BipcInstanceData *) instData;
    pInst->clientAuthRspPending = FALSE;
    csrBtBipcAuthenticateIndSend(pInst->appHandle, pInst->myAppHandle, deviceAddr, options);
    CsrPmemFree(realm);
    CSR_UNUSED(realmLength);
}

static void csrBtBipcAaPutIndHandler(void          *instData,
                                     CsrUint16     obexResponsePacketLength,
                                     CsrBool       authenticated,
                                     CsrBool       finalBitSet,
                                     CsrUint16     bodyLength,
                                     CsrUint16     bodyOffset,
                                     CsrUint16     obexPacketLength,
                                     CsrUint8    *obexPacket)
{
    /* The first Put Request packet is received */
    CsrUint16        typeOffset;
    CsrUint16        typeLength;
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if (CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeOffset, &typeLength))
    { /* A Type header is found */
        if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE)) &&
            (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]),
                                 (char*)CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE, sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE))))
        {
            CsrUint16 imgHandleOffset;

            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST, obexPacket, &imgHandleOffset);
            csrBtBipcAaDeleteImageHeaderIndSend(pInst->myAppHandle,
                                                pInst->appHandle,
                                                (CsrUint32) pInst->servServerChannel,
                                                imgHandleOffset,
                                                obexPacketLength,
                                                obexPacket);
            obexPacket = NULL;
        }
    }


    if (obexPacket)
    {
        ObexUtilServPutResponse(pInst->obexServInst,
                                CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE,
                                CSR_BT_OBEX_SRMP_INVALID);
        CsrPmemFree(obexPacket);
    }
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(bodyLength);
    CSR_UNUSED(bodyOffset);
}

static void csrBtBipcAaGetImageListIndHandler(void          *instData,
                                              CsrUint16     obexResponsePacketLength,
                                              CsrBool       authenticated,
                                              CsrBool       finalBitSet,
                                              CsrUint16     obexPacketLength,
                                              CsrUint8    *obexPacket)
{
    BipcInstanceData * pInst                = (BipcInstanceData *) instData;
    csrBtBipcAaGetImageListObjectIndSend(pInst, obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaGetCapabilitiesIndHandler(void          *instData,
                                                 CsrUint16     obexResponsePacketLength,
                                                 CsrBool       authenticated,
                                                 CsrBool       finalBitSet,
                                                 CsrUint16     obexPacketLength,
                                                 CsrUint8    *obexPacket)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcAaGetCapabilitiesObjectIndSend(pInst);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaGetImagePropertiesIndHandler(void          *instData,
                                                    CsrUint16     obexResponsePacketLength,
                                                    CsrBool       authenticated,
                                                    CsrBool       finalBitSet,
                                                    CsrUint16     obexPacketLength,
                                                    CsrUint8    *obexPacket)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcAaGetImagePropertiesObjectIndSend(pInst, obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaGetImageIndHandler(void          *instData,
                                          CsrUint16     obexResponsePacketLength,
                                          CsrBool       authenticated,
                                          CsrBool       finalBitSet,
                                          CsrUint16     obexPacketLength,
                                          CsrUint8    *obexPacket)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcAaGetImageObjectIndSend(pInst, obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaGetLinkedThumbnailIndHandler(void          *instData,
                                                    CsrUint16     obexResponsePacketLength,
                                                    CsrBool       authenticated,
                                                    CsrBool       finalBitSet,
                                                    CsrUint16     obexPacketLength,
                                                    CsrUint8    *obexPacket)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    csrBtBipcAaGetLinkedThumbnailObjectIndSend(pInst, obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaGetLinkedAttachmentIndHandler(void          *instData,
                                                     CsrUint16     obexResponsePacketLength,
                                                     CsrBool       authenticated,
                                                     CsrBool       finalBitSet,
                                                     CsrUint16     obexPacketLength,
                                                     CsrUint8    *obexPacket)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcAaGetLinkedAttachmentObjectIndSend(pInst, obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAaGetIndHandler(void          *instData,
                                     CsrUint16     obexResponsePacketLength,
                                     CsrBool       authenticated,
                                     CsrBool       finalBitSet,
                                     CsrUint16     obexPacketLength,
                                     CsrUint8    *obexPacket)
{ /* The first Get Request packet is received */
    CsrUint16        typeOffset;
    CsrUint16        typeLength;
    BipcInstanceData * pInst        = (BipcInstanceData *) instData;
    pInst->servObexPacketRspLength  = obexResponsePacketLength;
    pInst->servLengthOfObject       = 0;

    if (CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeOffset, &typeLength))
    { /* A type header is found */
        if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_LIST)) &&
            (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPC_TEXTTYPE_IMG_LIST, sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_LIST))))
        {
            CsrUint16 imgDescriptorOffset;
            CsrUint16 imgDescriptorLength;
            CsrUint16 nbReturnedHandles;
            CsrUint16 listStartOffset;
            CsrBool   latestCapturedImages;

            csrBtBipcDetermineImgListParameters(obexPacket, &nbReturnedHandles, &listStartOffset, &latestCapturedImages);
            CsrBtObexGetImgDescriptorOffset(CSR_BT_OBEX_REQUEST, obexPacket, &imgDescriptorOffset, &imgDescriptorLength);


            csrBtBipcAaGetImageListHeaderIndSend(pInst,
                                                 imgDescriptorOffset,
                                                 imgDescriptorLength,
                                                 nbReturnedHandles,
                                                 listStartOffset,
                                                 latestCapturedImages,
                                                 obexResponsePacketLength,
                                                 obexPacketLength,
                                                 obexPacket);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipcAaGetImageListIndHandler);
            obexPacket = NULL;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_CAPABILITIES)) &&
                 (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPC_TEXTTYPE_IMG_CAPABILITIES, sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_CAPABILITIES))))
        {
            csrBtBipcAaGetCapabilitiesHeaderIndSend(pInst);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipcAaGetCapabilitiesIndHandler);
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_PROPERTIES)) &&
                 (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPC_TEXTTYPE_IMG_PROPERTIES, sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_PROPERTIES))))
        {
            CsrUint16 imgHandleOffset;
            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST, obexPacket, &imgHandleOffset);
            csrBtBipcAaGetImagePropertiesHeaderIndSend(pInst, imgHandleOffset, obexPacketLength, obexPacket);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipcAaGetImagePropertiesIndHandler);
            obexPacket = NULL;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE)) &&
                 (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE, sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE))))
        {
            CsrUint16 imgHandleOffset;
            CsrUint16 imgDescriptorOffset;
            CsrUint16 imgDescriptorLength;
            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST, obexPacket, &imgHandleOffset);
            CsrBtObexGetImgDescriptorOffset(CSR_BT_OBEX_REQUEST, obexPacket, &imgDescriptorOffset, &imgDescriptorLength);
            csrBtBipcAaGetImageHeaderIndSend(pInst, imgHandleOffset, imgDescriptorOffset, imgDescriptorLength, obexPacketLength, obexPacket);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipcAaGetImageIndHandler);
            obexPacket = NULL;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_THUMB)) &&
                 (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPC_TEXTTYPE_IMG_THUMB, sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_THUMB))))
        {
            CsrUint16 imgHandleOffset;
            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST, obexPacket, &imgHandleOffset);
            csrBtBipcAaGetLinkedThumbnailHeaderIndSend(pInst, imgHandleOffset, obexPacketLength, obexPacket);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipcAaGetLinkedThumbnailIndHandler);
            obexPacket = NULL;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_ATTACHMENT)) &&
                 (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]), (char*)CSR_BT_BIPC_TEXTTYPE_IMG_ATTACHMENT, sizeof(CSR_BT_BIPC_TEXTTYPE_IMG_ATTACHMENT))))
        {
            CsrUint16 imgHandleOffset;
            CsrUint16 nameOffset;
            CsrUint16 nameLength;
            CsrBtObexGetImgHandleOffset(CSR_BT_OBEX_REQUEST, obexPacket, &imgHandleOffset);
            csrBtBipcGetNameOffset(obexPacket, &nameOffset, &nameLength);
            csrBtBipcAaGetLinkedAttachmentHeaderIndSend(pInst, imgHandleOffset, nameOffset, nameLength, obexPacketLength, obexPacket);
            ObexUtilServSetGetContinueFunc(pInst->obexServInst, csrBtBipcAaGetLinkedAttachmentIndHandler);
            obexPacket = NULL;
        }
        else
        {
            ObexUtilServGetResponse(pInst->obexServInst,
                                    CSR_BT_OBEX_NOT_IMPLEMENTED_RESPONSE_CODE,
                                    NULL,
                                    0,
                                    CSR_BT_OBEX_SRMP_INVALID);
        }
    }
    else
    {
        ObexUtilServGetResponse(pInst->obexServInst,
                                CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE,
                                NULL,
                                0,
                                CSR_BT_OBEX_SRMP_INVALID);
    }
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

static CsrUint8 csrBtBipcActivateAaStartHandler(BipcInstanceData * pInst)
{
    dm_security_level_t secIncoming;
    CsrUint16 recordLength       = (CsrUint16) (sizeof(serviceRecord));
    CsrUint8  *record            = (CsrUint8 *)CsrPmemAlloc(recordLength);
    CsrMemCpy(record, serviceRecord, recordLength);
    record[CSR_BT_BIPC_SERVICE_ID_SERVICE_RECORD_INDEX] = CSR_BT_BIPC_AUTO_ARCHIVE_SERVICE_ID;

    CsrBtScSetSecInLevel(&secIncoming,
                         CSR_BT_SEC_DEFAULT,
                         CSR_BT_SEC_NONE,
                         CSR_BT_OBEX_IMAGING_RESPONDER_DEFAULT_SECURITY_INCOMING,
                         CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                         CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    return (ObexUtilServActivateStart(pInst->obexServInst,
                                      secIncoming,
                                      CSR_BT_OBEX_IMAGING_AUTOMATIC_ARCHIVE_UUID,
                                      CSR_BT_BIPC_LP_SUPERVISION_TIMEOUT,
                                      CSR_BT_NO_SERVER,
                                      L2CA_PSM_INVALID,
                                      recordLength,
                                      CSR_BT_BIPC_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                                      CSR_BT_BIPC_L2CAP_PSM_SDP_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      &record,
                                      CSR_BT_OBJECT_TRANSFER_COD,
                                      csrBtBipcAaConnectIndHandler,
                                      csrBtBipcAaPutIndHandler,
                                      csrBtBipcAaGetIndHandler,
                                      NULL,
                                      csrBtBipcAaAbortIndHandler,
                                      csrBtBipcAaDisconnectIndHandler,
                                      csrBtBipcAaAuthenticateIndHandler,
                                      csrBtBipcActivateAaCfmHandler,
                                      NULL,
                                      pInst->originalMaxPacketSize,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));
}

static void csrBtBipcConnectCfmHandler(BipcInstanceData *pInst, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    if (resultCode     == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE  &&
        resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    {
        pInst->clientState = BIPC_CLIENT_OBEX_CONNECTED_STATE;
#ifdef CSR_AMP_ENABLE
        pInst->masterBtConnId = pInst->btConnId;
#endif /* CSR_AMP_ENABLE */
    }
    else
    {
        pInst->clientState = BIPC_CLIENT_IDLE_STATE;
    }

    if(pInst->feature == CSR_BT_AUTO_ARCHIVE_FEATURE        &&
       pInst->clientState == BIPC_CLIENT_OBEX_CONNECTED_STATE)
    { /* An primary client auto archive channel is established.
         Activate the secondary server */
        if (csrBtBipcActivateAaStartHandler(pInst) == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
        { /* The server has started it activation procedure */
            pInst->servState = BIPC_SERVER_ACTIVATING_STATE;
        }
        else
        { /* The server could not start it activation procedure */
            csrBtBipcReleaseChannelsHandler(pInst);
        }
    }
    else
    {
        pInst->servState = BIPC_SERVER_IDLE_STATE;
        csrBtBipcConnectCfmSend(pInst,
                                resultCode,
                                resultSupplier,
                                pInst->btConnId);
    }
}

static CsrUint8 * csrBtBipcIsFeatureSupported(CsrUint8 feature)
{
    switch (feature)
    {
        case CSR_BT_IMAGE_PUSH_FEATURE:
        case CSR_BT_REMOTE_CAMERA_FEATURE:
        case CSR_BT_AUTO_ARCHIVE_FEATURE:
            {
                CsrUint8 *target  = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BIPC_TARGET_LENGTH);
                CsrMemCpy(target, BasicImagingClientSessions[feature], CSR_BT_BIPC_TARGET_LENGTH);
                return (target);
            }
        default:
            {
                return (NULL);
            }
    }
}

static void csrBtBipcExtractConnectSdpAttributes(BipcInstanceData * pInst, CmnCsrBtLinkedListStruct * sdpTag)
{
    if (sdpTag)
    {
        CsrBtUuid32  service;
        CsrUint16    bchsResult, bchsTask, bchsTaskResult;

        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTag, 0, &service, &bchsResult, &bchsTask, &bchsTaskResult))
        {
            if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
            {
                CsrUint32 returnValue;
                CsrUint8  *data;

                if (CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTag, 0,
                                                                           CSR_BT_SUPPORTED_FUNCTIONS_ATTRIBUTE_IDENTIFIER, &returnValue))
                {
                    pInst->supportedFunctions = returnValue;
                }
                if (CsrBtUtilSdrGetUintDataAttributeFromAttributeUuid(sdpTag, 0,
                                                                      CSR_BT_TOTAL_IMAGING_DATA_CAPACITY_ATTRIBUTE_IDENTIFIER, &data, &returnValue) &&
                    returnValue == CSR_BT_DATA_CAPACITY_LEN)
                {
                    CsrMemCpy(pInst->dataCapacity, data, CSR_BT_DATA_CAPACITY_LEN);
                }
            }
        }
    }
}

static void csrBtBipcConnectResultHandler(void                      *instData,
                                          CsrBtResultCode           resultCode,
                                          CsrBtSupplier             resultSupplier,
                                          CsrBtDeviceAddr           deviceAddr,
                                          CsrBtConnId               btConnId,
                                          CsrUint16                 maxPeerObexPacketLength,
                                          CmnCsrBtLinkedListStruct  *sdpTag,
                                          CsrUint16                 obexPacketLength,
                                          CsrUint8                  *obexPacket)
{
    BipcInstanceData * pInst        = (BipcInstanceData *) instData;
    pInst->maxPeerObexPacketLength  = maxPeerObexPacketLength;
    pInst->deviceAddr               = deviceAddr;
    pInst->btConnId                 = btConnId;

    csrBtBipcExtractConnectSdpAttributes(pInst, sdpTag);
    csrBtBipcConnectCfmHandler(pInst, resultCode, resultSupplier);
    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

static void csrBtBipcAuthenticateIndHandler(void            *instData,
                                            CsrBtDeviceAddr deviceAddr,
                                            CsrUint8        options,
                                            CsrUint16       realmLength,
                                            CsrUint8      *realm)
{
    BipcInstanceData * pInst    = (BipcInstanceData *) instData;
    pInst->clientAuthRspPending = TRUE;
    csrBtBipcAuthenticateIndSend(pInst->appHandle, pInst->myAppHandle, deviceAddr, options);
    CsrPmemFree(realm);
    CSR_UNUSED(realmLength);
}

static void csrBtBipcDisconnectResultHandler(void               *instData,
                                             CsrBtReasonCode     reasonCode,
                                             CsrBtSupplier       reasonSupplier,
                                             CsrUint8            *obexPacket,
                                             CsrUint16           obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;


    pInst->clientState = BIPC_CLIENT_IDLE_STATE;

    switch (pInst->servState)
    {
        case BIPC_SERVER_ACTIVATING_STATE:
        case BIPC_SERVER_ACTIVATED_STATE:
            { /* The server is either being avtivated or it is
                 already activated. Deactivated it */
                csrBtBipcReleaseChannelsHandler(pInst);
                break;
            }
        case BIPC_SERVER_DEACTIVATING_STATE:
            { /* The server is being deactivated, wait for the
                 CsrBtBipcAaDeactivateCfmHandler function to be
                 called */
                break;
            }
        default:
            { /* Both the server and the client is in idle state */
                if (pInst->connectCfmSent)
                { /* The application has received a successful
                     CSR_BT_BIPC_CONNECT_CFM message, send a
                     CSR_BT_BIPC_DISCONNECT_IND message */
                    csrBtBipcDisconnectIndSend(pInst, reasonCode, reasonSupplier);
                }
                else
                { /* The application has not received a
                     CSR_BT_BIPC_CONNECT_CFM message yet */
                    csrBtBipcConnectCfmSend(pInst,
                                            CSR_BT_RESULT_CODE_OBEX_SECONDARY_CHANNEL_ERROR,
                                            CSR_BT_SUPPLIER_OBEX_PROFILES,
                                            pInst->btConnId);
                }
                break;
            }
    }
#ifdef CSR_AMP_ENABLE
    pInst->masterBtConnId = 0;
#endif /* CSR_AMP_ENABLE */
    CsrPmemFree(obexPacket);
    CsrPmemFree(pInst->clientMiscInfo);
    CsrPmemFree(pInst->servMiscInfo);
    CSR_UNUSED(obexPacketLength);
    pInst->servMiscInfo     = NULL;
    pInst->clientMiscInfo   = NULL;
}

CsrUint8 CsrBtBipcConnectReqHandler(BipcInstanceData *pInst, void *msg)
{
    if (pInst->clientState  == BIPC_CLIENT_IDLE_STATE &&
        pInst->servState    == BIPC_SERVER_IDLE_STATE)
    {
        CsrBtBipcConnectReq *pMsg       = (CsrBtBipcConnectReq *) msg;
        CsrUint8 *target                = csrBtBipcIsFeatureSupported(pMsg->feature);
        pInst->appHandle                = pMsg->appHandle;
        pInst->feature                  = pMsg->feature;
        pInst->maxPeerObexPacketLength  = 0;
        pInst->supportedFunctions       = 0;
        pInst->connectCfmSent           = FALSE;
        pInst->originalMaxPacketSize    = pMsg->maxPacketSize;
        pInst->windowSize               = pMsg->windowSize;
        pInst->localSrmEnable           = pMsg->srmEnable;
        CsrMemSet(pInst->dataCapacity, 0, CSR_BT_DATA_CAPACITY_LEN);

        if(target)
        {
            CsrUint8  returnValue;
            CsrUint16 index;
            CsrUint8   numOfHeaders             = 0;
            CmnCsrBtLinkedListStruct * sdpTag   = NULL;
            CsrUint8   headerPriorityTable[2]   = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
            ObexUtilDigestChallengeType * chal  = ObexUtilReturnDigestChallengePointer(pMsg->authorize,
                                                                                       &pMsg->password,
                                                                                       pMsg->passwordLength,
                                                                                       &pMsg->userId,
                                                                                       &pMsg->realm,
                                                                                       pMsg->realmLength);

            sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_IMAGING_RESPONDER_UUID, &index);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SUPPORTED_FUNCTIONS_ATTRIBUTE_IDENTIFIER, NULL, 0);
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_TOTAL_IMAGING_DATA_CAPACITY_ATTRIBUTE_IDENTIFIER, NULL, 0);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
            CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_OBEX_L2CAP_PSM_ATTRIBUTE, NULL, 0);
#endif 
            CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

            if (pMsg->length > 0)
            {
                numOfHeaders = (CsrUint8)(numOfHeaders + 1);
            }

            if (pMsg->count > 0)
            {
                numOfHeaders = (CsrUint8)(numOfHeaders + 1);
            }

            returnValue = ObexUtilConnectRequest(pInst->obexCliInst,
                                                 &sdpTag,
                                                 pMsg->deviceAddr,
                                                 pInst->secOutgoing,
                                                 pMsg->maxPacketSize,
                                                 CSR_BT_BIPC_PROFILE_DEFAULT_MTU_SIZE,
                                                 &target,
                                                 CSR_BT_BIPC_TARGET_LENGTH,
                                                 numOfHeaders,
                                                 headerPriorityTable,
                                                 CSR_BT_BIPC_LP_SUPERVISION_TIMEOUT,
                                                 &chal,
                                                 pMsg->length,
                                                 pMsg->count,
                                                 NULL,
                                                 0,
                                                 NULL,
                                                 csrBtBipcConnectResultHandler,
                                                 csrBtBipcAuthenticateIndHandler,
                                                 csrBtBipcDisconnectResultHandler,
                                                 NULL,
                                                 pInst->windowSize,
                                                 pInst->localSrmEnable);

            if (returnValue == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
            {
                pInst->clientState = BIPC_CLIENT_OBEX_CONNECTING_STATE;
            }
            return (returnValue);
        }
        else
        {
            csrBtBipcConnectCfmHandler(pInst, CSR_BT_RESULT_CODE_OBEX_LOCAL_FEATURE_NOT_SUPPORTED, CSR_BT_SUPPLIER_OBEX_PROFILES);
            CsrBtBipcFreeDownstreamMessageContents(CSR_BT_BIPC_PRIM, msg);
            return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
        }
    }
    else
    {
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}

void CsrBtBipcExtractRemoteFeatureSdpAttributes(void                     *instData,
                                                CmnCsrBtLinkedListStruct *sdpTagList,
                                                CsrBtDeviceAddr          deviceAddr,
                                                CsrBtResultCode          resultCode,
                                                CsrBtSupplier            resultSupplier)
{
    BipcInstanceData * pInst            = (BipcInstanceData *) instData;
    CsrUint16        supportedFeatures  = 0;

    if (sdpTagList)
    {
        CsrBtUuid32  service;
        CsrUint16    bchsResult, bchsTask, bchsTaskResult;

        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTagList, 0, &service, &bchsResult, &bchsTask, &bchsTaskResult))
        {
            if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
            {
                CsrUint32 returnValue;

                if (CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTagList, 0,
                                                                           CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, &returnValue))
                {
                    supportedFeatures = (CsrUint16) returnValue;
                }
            }
        }
    }
    csrBtBipcGetRemoteFeaturesCfmSend(pInst, supportedFeatures);
    CsrBtUtilBllFreeLinkedList(&(sdpTagList), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(resultCode);
    CSR_UNUSED(resultSupplier);
}

CsrUint8 CsrBtBipcRemoteFeaturesReqHandler(BipcInstanceData *pInst, void *msg)
{ /* The application wants to know the available features */
    if (pInst->clientState  == BIPC_CLIENT_IDLE_STATE &&
        pInst->servState    == BIPC_SERVER_IDLE_STATE)
    {
        CsrUint16                     index;
        CmnCsrBtLinkedListStruct      *sdpTag   = NULL;
        CsrBtBipcGetRemoteFeaturesReq *pMsg     = (CsrBtBipcGetRemoteFeaturesReq *) msg;
        pInst->clientState                      = BIPC_CLIENT_SDP_SEARCH_STATE;
        pInst->appHandle                        = pMsg->appHandle;
        sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_IMAGING_RESPONDER_UUID, &index);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SUPPORTED_FEATURES_ATTRIBUTE_IDENTIFIER, NULL, 0);
        /* Start the SDP search procedure */          
        CsrBtUtilSdcSearchStart((void *)pInst, pInst->sdpSearchData, sdpTag, pMsg->deviceAddr);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}

static void csrBtBipcAbortResultHandler(void                  *instData,
                                        CsrBtObexResponseCode  responseCode,
                                        CsrUint8             *obexPacket,
                                        CsrUint16              obexPacketLength)
{
    BipcInstanceData  *pInst = (BipcInstanceData *) instData;

    csrBtBipcAbortCfmSend(pInst);
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(pInst->clientMiscInfo);
    pInst->clientMiscInfo = NULL;
    CsrPmemFree(obexPacket);
}

CsrUint8 CsrBtBipcAbortReqHandler(BipcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    if (pInst->clientState == BIPC_CLIENT_OBEX_CONNECTED_STATE)
    {
        return(ObexUtilAbortRequest(pInst->obexCliInst, NULL, csrBtBipcAbortResultHandler));
    }
    else
    { /* This signal is crossing with a disconnect, ignore it */
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}

CsrUint8 CsrBtBipcDisconnectReqHandler(BipcInstanceData *pInst, void *msg)
{/* The application has requested that the connection is disconnected */
    CSR_UNUSED(msg);

    switch (pInst->clientState)
    {
        case BIPC_CLIENT_OBEX_CONNECTING_STATE:
        case BIPC_CLIENT_OBEX_CONNECTED_STATE:
            { /* Release the primary connection and deactivate the
                 server */
                csrBtBipcReleaseChannelsHandler(pInst);
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
        case BIPC_CLIENT_IDLE_STATE:
        case BIPC_CLIENT_OBEX_RELEASE_STATE:
            { /* Just ignore either it has been or being release */
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
        default:
            { /* BIPC_CLIENT_SDP_SEARCH_STATE */
                return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
            }
    }
}

CsrUint8 CsrBtBipcAuthenticateResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAuthenticateRes    *pMsg = (CsrBtBipcAuthenticateRes *) msg;
    

    if (pInst->clientAuthRspPending)
    { /* The client part has been authenticated */

        if (pInst->clientState == BIPC_CLIENT_OBEX_CONNECTING_STATE ||
            pInst->clientState == BIPC_CLIENT_OBEX_CONNECTED_STATE)
        {
            ObexUtilDigestChallengeType *chal = NULL;

            if (pMsg->chalPassword)
            { /* The application want to has challenge the peer server */
                chal = ObexUtilReturnDigestChallengePointer(TRUE,
                                                            &pMsg->chalPassword,
                                                            pMsg->chalPasswordLength,
                                                            &pMsg->chalUserId,
                                                            &pMsg->chalRealm,
                                                            pMsg->chalRealmLength);
            }
            else
            { /* Pfree pointers to prevent a memory leak */
                CsrPmemFree(pMsg->chalUserId);
                CsrPmemFree(pMsg->chalRealm);
            }
            return (ObexUtilCliAuthenticateResponse(pInst->obexCliInst, pMsg->authPasswordLength, &pMsg->authPassword, &pMsg->authUserId, &chal));
        }
    }
    else
    { /* The server part has been authenticated */
        if (pInst->servState == BIPC_SERVER_ACTIVATED_STATE)
        {
            return (ObexUtilServAuthenticateResponse(pInst->obexServInst, pMsg->authPasswordLength, &pMsg->authPassword, &pMsg->authUserId));
        }
    }

    /* This signal is crossing with a disconnect, ignore it */
    CsrBtBipcFreeDownstreamMessageContents(CSR_BT_BIPC_PRIM, msg);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBipcSecurityOutReqHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtResultCode rval;
    CsrBtBipcSecurityOutReq *prim = (CsrBtBipcSecurityOutReq*) msg;

    rval = CsrBtScSetSecOutLevel(&pInst->secOutgoing, prim->secLevel,
                                 CSR_BT_OBEX_IMAGING_INITIATOR_MANDATORY_SECURITY_OUTGOING,
                                 CSR_BT_OBEX_IMAGING_INITIATOR_DEFAULT_SECURITY_OUTGOING,
                                 CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                 CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtBipcSecurityOutCfmSend(prim->appHandle, pInst->myAppHandle, rval, CSR_BT_SUPPLIER_OBEX_PROFILES);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

/* Image Push handlers */
static void csrBtBipcImagePushGetCapabilitiesCfmSend(BipcInstanceData      *pInst,
                                                     CsrUint16             bodyLength,
                                                     CsrUint16             bodyOffset,
                                                     CsrUint8            *obexPacket,
                                                     CsrUint16             obexPacketLength,
                                                     CsrBtObexResponseCode responseCode)
{
    CsrBtBipcPushGetCapabilitiesCfm *pMsg = (CsrBtBipcPushGetCapabilitiesCfm *) CsrPmemAlloc(sizeof(CsrBtBipcPushGetCapabilitiesCfm));
    pMsg->type                            = CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM;
    pMsg->pHandleInst                     = pInst->myAppHandle;
    pMsg->responseCode                    = responseCode;
    pMsg->capabilitiesObjectOffset        = bodyOffset;
    pMsg->capabilitiesObjectLength        = bodyLength;
    pMsg->payload                         = obexPacket;
    pMsg->payloadLength                   = obexPacketLength;
    csrBtBipcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtBipcImagePushGetCapabilitiesResultHandler(void                   *instData,
                                                           CsrBtObexResponseCode  responseCode,
                                                           CsrBool                bodyHeader,
                                                           CsrUint16              bodyLength,
                                                           CsrUint16              bodyOffset,
                                                           CsrUint8             *obexPacket,
                                                           CsrUint16              obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        csrBtBipcImagePushGetCapabilitiesIndSend(pInst, bodyLength, bodyOffset, obexPacket, obexPacketLength);
    }
    else
    {
        csrBtBipcImagePushGetCapabilitiesCfmSend(pInst, bodyLength, bodyOffset, obexPacket, obexPacketLength, responseCode);
    }
    CSR_UNUSED(bodyHeader);
}

CsrUint8 CsrBtBipcImagePushGetCapabilitiesReqHandler(BipcInstanceData * pInst, void *msg)
{
    CsrBtBipcPushGetCapabilitiesReq *pMsg =  (CsrBtBipcPushGetCapabilitiesReq *) msg;
    CsrUint8 headerPriorityTable[5]       = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                             CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                             CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                             CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                             CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    return(ObexUtilGetRequest(pInst->obexCliInst,
                              headerPriorityTable,
                              (const CsrUint8*)(CSR_BT_BIPC_TEXTTYPE_IMG_CAPABILITIES),
                              NULL,
                              NULL,
                              0,
                              NULL,
                              0,
                              NULL,
                              ObexUtilReturnSrmpValue(pMsg->srmpOn),
                              csrBtBipcImagePushGetCapabilitiesResultHandler,
                              NULL));

}

CsrUint8 CsrBtBipcImagePushGetCapabilitiesResHandler(BipcInstanceData * pInst, void *msg)
{
    CsrBtBipcPushGetCapabilitiesRes *pMsg =  (CsrBtBipcPushGetCapabilitiesRes *) msg;

    return(ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                      ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

static void csrBtBipcImagePushThumbnailContinueHandler(void        *instData,
                                                       CsrUint16   obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcImagePushPutThumbnailIndSend(pInst, obexPacketLength);
}

static void csrBtBipcImagePushThumbnailResultHandler(void                  *instData,
                                                     CsrBtObexResponseCode responseCode,
                                                     CsrUint8            *obexPacket,
                                                     CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcImagePushPutImageCfmSend(pInst, responseCode, pInst->imgHandle);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtBipcImagePushImageContinueHandler(void        *instData,
                                                   CsrUint16   obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcImagePushPutImageFileIndSend(pInst, obexPacketLength);
}

static void csrBtBipcImagePushImageResultHandler(void                  *instData,
                                                 CsrBtObexResponseCode responseCode,
                                                 CsrUint8            *obexPacket,
                                                 CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    switch (responseCode)
    {
        case CSR_BT_OBEX_PARTIAL_CONTENT_RESPONSE_CODE:
        case CSR_BT_OBEX_SUCCESS_RESPONSE_CODE:
            {
                if (CsrBtObexCopyDecodedImgHandle(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, pInst->imgHandle))
                {
                    if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
                    {
                        csrBtBipcImagePushPutImageCfmSend(pInst, responseCode, pInst->imgHandle);
                    }
                    else
                    {
                        CsrUint16 userDefinedHeaderLength;
                        CsrUint8  headerPriorityTable[6]   = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                                              CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

                        CsrUint8 * userDefinedHeader       = csrBtBipcBuildImageHandleHeader(pInst->imgHandle, &userDefinedHeaderLength);

                        ObexUtilPutRequest(pInst->obexCliInst,
                                           headerPriorityTable,
                                           0,
                                           (const CsrUint8*)(CSR_BT_BIPC_TEXTTYPE_IMG_THUMB),
                                           NULL,
                                           NULL,
                                           0,
                                           NULL,
                                           userDefinedHeaderLength,
                                           &userDefinedHeader,
                                           csrBtBipcImagePushThumbnailContinueHandler,
                                           csrBtBipcImagePushThumbnailResultHandler);
                    }
                }
                else
                {
                    csrBtBipcReleaseChannelsHandler(pInst);
                }
                break;
            }
        default:
            {
                csrBtBipcImagePushPutImageCfmSend(pInst, responseCode, (CsrUint8 *) "");
                break;
            }
    }
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}


CsrUint8 CsrBtBipcImagePushPutImageReqHandler(BipcInstanceData * pInst, void *msg)
{
    CsrUint16 userDefinedHeaderLength;

    CsrUint8 headerPriorityTable[6]   = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                         CSR_BT_OBEX_UTIL_NAME_HEADER,
                                         CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtBipcPushPutImageReq *pMsg    = (CsrBtBipcPushPutImageReq *) msg;
    CsrUint8 * userDefinedHeader      = csrBtBipcBuildImageDescriptorHeader(pMsg->imageDescriptorLength,
                                                                            pMsg->imageDescriptor,
                                                                            &userDefinedHeaderLength);

    return (ObexUtilPutRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               0,
                               (const CsrUint8*)(CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE),
                               &pMsg->ucs2imageName,
                               NULL,
                               0,
                               NULL,
                               userDefinedHeaderLength,
                               &userDefinedHeader,
                               csrBtBipcImagePushImageContinueHandler,
                               csrBtBipcImagePushImageResultHandler));
}

CsrUint8 CsrBtBipcImagePushPutImageFileResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcPushPutImageFileRes *pMsg =  (CsrBtBipcPushPutImageFileRes *) msg;

    return(ObexUtilPutContinueRequest(pInst->obexCliInst,
                                      pMsg->finalFlag,
                                      &pMsg->imageFile,
                                      pMsg->imageFileLength));
}

CsrUint8 CsrBtBipcImagePushPutThumbnailFileResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcPushPutThumbnailFileRes *pMsg =  (CsrBtBipcPushPutThumbnailFileRes *) msg;

    return(ObexUtilPutContinueRequest(pInst->obexCliInst,
                                      pMsg->finalFlag,
                                      &pMsg->thumbnailFile,
                                      pMsg->thumbnailFileLength));
}

static void csrBtBipcImageAttachmentContinueHandler(void        *instData,
                                                    CsrUint16   obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcImagePushPutAttachmentIndSend(pInst, obexPacketLength);
}

static void csrBtBipcImageAttachmentResultHandler(void                  *instData,
                                                  CsrBtObexResponseCode responseCode,
                                                  CsrUint8            *obexPacket,
                                                  CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;
    csrBtBipcImagePushPutAttachmentCfmSend(pInst, responseCode);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(obexPacketLength);
}

CsrUint8 CsrBtBipcImagePushPutLinkedAttachmentReqHandler(BipcInstanceData * pInst, void *msg)
{
    CsrUint16 userDefinedHeaderLength;

    CsrUint8 headerPriorityTable[6]    = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                          CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtBipcPushPutAttachmentReq *pMsg = (CsrBtBipcPushPutAttachmentReq *) msg;
    CsrUint8 * userDefinedHeader        = csrBtBipcBuildImageHandleImageDescriptorHeader(pInst->imgHandle,
                                                                                         pMsg->attachmentDescriptorLength,
                                                                                         pMsg->attachmentDescriptor,
                                                                                         &userDefinedHeaderLength);
    return (ObexUtilPutRequest(pInst->obexCliInst,
                               headerPriorityTable,
                               0,
                               (const CsrUint8*)(CSR_BT_BIPC_TEXTTYPE_IMG_ATTACHMENT),
                               NULL,
                               NULL,
                               0,
                               NULL,
                               userDefinedHeaderLength,
                               &userDefinedHeader,
                               csrBtBipcImageAttachmentContinueHandler,
                               csrBtBipcImageAttachmentResultHandler));
}

CsrUint8 CsrBtBipcImagePushPutLinkedAttachmentResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcPushPutAttachmentFileRes *pMsg =  (CsrBtBipcPushPutAttachmentFileRes *) msg;
    return(ObexUtilPutContinueRequest(pInst->obexCliInst, pMsg->finalFlag, &pMsg->attachmentFile, pMsg->attachmentFileLength));
}

/* Image Remote Camera handlers */
static CsrBool bipcRcGetMonitoringImageHeaderHandler(CsrBool    storeFlag,
                                                     CsrUint8 *obexPacket,
                                                     CsrUint8   *imageHandle)
{
    if (storeFlag)
    {
        if (CsrBtObexCopyDecodedImgHandle(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, imageHandle))
        { /* The Img-Handle header is found */
            return (TRUE);
        }
        else
        { /* The Img-Handle header is not found */
            return (FALSE);
        }
    }
    else
    {
        CsrMemSet(imageHandle, '\0', (CSR_BT_IMG_HANDLE_LENGTH + 1));
    }
    return (TRUE);
}

static void BipcRcGetMonitoringImageResultHandler(void                  *instData,
                                                  CsrBtObexResponseCode responseCode,
                                                  CsrBool               bodyHeader,
                                                  CsrUint16             bodyLength,
                                                  CsrUint16             bodyOffset,
                                                  CsrUint8            *obexPacket,
                                                  CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if ((responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE) ||
        (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE))
    {
        pInst->clientResponseCode = responseCode;

        if (pInst->headerCfm)
        { /* The Img-Handle header is not found */
            CsrUint8 imageHandle[CSR_BT_IMG_HANDLE_LENGTH + 1];

            if (bipcRcGetMonitoringImageHeaderHandler(pInst->storeFlag, obexPacket, imageHandle))
            { /* The Img-Handle header is found */
                pInst->headerCfm = FALSE;
                csrBtBipcRcGetMonitoringImageHeaderIndSend(pInst, imageHandle);

                if (bodyHeader)
                { /* A body header is present */
                    pInst->clientMiscInfo       = obexPacket;
                    pInst->clientMiscInfoLength = obexPacketLength;
                    obexPacket                  = NULL;
                }
                else
                { /* Only the Img-Handle header is present */
                    ;
                }
            }
            else
            { /* The Img-Handle header is not found */
                csrBtBipcReleaseChannelsHandler(pInst);
            }
        }
        else
        { /* The Img-Handle header is already found and sent to app */
            csrBtBipcRcGetMonitoringImageFileIndSend(pInst, bodyLength, bodyOffset, obexPacket, obexPacketLength);
            obexPacket = NULL;
        }
    }
    else
    {
        csrBtBipcRcGetMonitoringImageCfmSend(pInst, responseCode);
    }
    CsrPmemFree(obexPacket);
}

CsrUint8 CsrBtBipcRcGetMonitoringImageReqHandler(BipcInstanceData *pInst, void *msg)
{
    CsrUint16 appParametersHeaderLength;
    CsrUint8  headerPriorityTable[5]        = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                               CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtBipcRcGetMonitoringImageReq *pMsg  = (CsrBtBipcRcGetMonitoringImageReq *) msg;
    CsrUint8 * appParametersHeader          = csrBtBipcBuildApplicationParametersStoreFlagHeader(pMsg->storeFlag,
                                                                                                 &appParametersHeaderLength);
    pInst->headerCfm                        = TRUE;
    pInst->storeFlag                        = pMsg->storeFlag;
    return(ObexUtilGetRequest(pInst->obexCliInst,
                              headerPriorityTable,
                              (const CsrUint8*)CSR_BT_BIPC_TEXTTYPE_IMG_MONITORING,
                              NULL,
                              NULL,
                              appParametersHeaderLength,
                              &appParametersHeader,
                              0,
                              NULL,
                              ObexUtilReturnSrmpValue(pMsg->srmpOn),
                              BipcRcGetMonitoringImageResultHandler,
                              NULL));
}

CsrUint8 CsrBtBipcRcGetMonitoringImageFileResHandler(BipcInstanceData *pInst, void *msg)
{
    if (pInst->clientResponseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    { /* Send the next GET command */
        if (msg)
        {
            CsrBtBipcRcGetMonitoringImageFileRes *pMsg =  (CsrBtBipcRcGetMonitoringImageFileRes *) msg;
            return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                               ObexUtilReturnSrmpValue(pMsg->srmpOn)));
        }
        return (ObexUtilGetContinueRequest(pInst->obexCliInst, CSR_BT_OBEX_SRMP_WAIT));
    }
    else
    { /* The Get procedure is done */
        csrBtBipcRcGetMonitoringImageCfmSend(pInst, pInst->clientResponseCode);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}

CsrUint8 CsrBtBipcRcGetRcMonitoringImageHeaderResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrUint8     bodyType;
    CsrUint16    bodyOffset  = 0;
    CsrUint16    bodyLength  = 0;
    CsrUint8     returnValue = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
    if (pInst->clientMiscInfo &&
        CsrBtObexGetBodyOffset(CSR_BT_OBEX_RESPONSE_NORMAL, pInst->clientMiscInfo, &bodyType, &bodyOffset, &bodyLength))
    { /* The first received OBEX GET Response packet contain a Body header */
        csrBtBipcRcGetMonitoringImageFileIndSend(pInst, bodyLength, bodyOffset,
                                                 pInst->clientMiscInfo, pInst->clientMiscInfoLength);
    }
    else
    { /* The first received OBEX GET Response packet did not contain a Body header */
        returnValue = CsrBtBipcRcGetMonitoringImageFileResHandler(pInst, NULL);
        CsrPmemFree(pInst->clientMiscInfo);
    }
    pInst->clientMiscInfo       = NULL;
    pInst->clientMiscInfoLength = 0;
    CSR_UNUSED(msg);
    return (returnValue);
}

static void csrBipcRcGetImagePropertiesResultHandler(void                  *instData,
                                                     CsrBtObexResponseCode responseCode,
                                                     CsrBool               bodyHeader,
                                                     CsrUint16             bodyLength,
                                                     CsrUint16             bodyOffset,
                                                     CsrUint8            *obexPacket,
                                                     CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        csrBtBipcRcGetImagePropertiesIndSend(pInst, bodyLength, bodyOffset, obexPacket, obexPacketLength);
    }
    else
    {
        csrBtBipcRcGetImagePropertiesCfmSend(pInst, responseCode, bodyLength, bodyOffset, obexPacket, obexPacketLength);
    }
}

CsrUint8 CsrBtBipcRcGetImagePropertiesReqHandler(BipcInstanceData *pInst, void *msg)
{
    CsrUint16 userDefinedHeaderLength;

    CsrUint8  headerPriorityTable[5]        = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                               CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtBipcRcGetImagePropertiesReq *pMsg  = (CsrBtBipcRcGetImagePropertiesReq *) msg;
    CsrUint8 * userDefinedHeader            = csrBtBipcBuildImageHandleHeader(pMsg->imageHandle, &userDefinedHeaderLength);

    return(ObexUtilGetRequest(pInst->obexCliInst,
                              headerPriorityTable,
                              (const CsrUint8*)CSR_BT_BIPC_TEXTTYPE_IMG_PROPERTIES,
                              NULL,
                              NULL,
                              0,
                              NULL,
                              userDefinedHeaderLength,
                              &userDefinedHeader,
                              ObexUtilReturnSrmpValue(pMsg->srmpOn),
                              csrBipcRcGetImagePropertiesResultHandler,
                              NULL));
}

CsrUint8 CsrBtBipcRcGetImagePropertiesResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcRcGetImagePropertiesRes *pMsg  = (CsrBtBipcRcGetImagePropertiesRes *) msg;

    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

static void csrBipcRcGetImageResultHandler(void                  *instData,
                                           CsrBtObexResponseCode responseCode,
                                           CsrBool               bodyHeader,
                                           CsrUint16             bodyLength,
                                           CsrUint16             bodyOffset,
                                           CsrUint8            *obexPacket,
                                           CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        csrBtBipcRcGetImageIndSend(pInst, bodyLength, bodyOffset, obexPacket, obexPacketLength);
    }
    else
    {
        csrBtBipcRcGetImageCfmSend(pInst, responseCode, bodyLength, bodyOffset, obexPacket, obexPacketLength);
    }
}

CsrUint8 CsrBtBipcRcGetImageReqHandler(BipcInstanceData *pInst, void *msg)
{
    CsrUint16 userDefinedHeaderLength;

    CsrUint8  headerPriorityTable[5]        = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                               CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtBipcRcGetImageReq *pMsg = (CsrBtBipcRcGetImageReq *) msg;
    CsrUint8 * userDefinedHeader = csrBtBipcBuildImageHandleImageDescriptorHeader(pMsg->imageHandle,
                                                                                  pMsg->imageDescriptorLength,
                                                                                  pMsg->imageDescriptor,
                                                                                  &userDefinedHeaderLength);
    return(ObexUtilGetRequest(pInst->obexCliInst,
                              headerPriorityTable,
                              (const CsrUint8*) CSR_BT_BIPC_TEXTTYPE_IMG_IMAGE,
                              NULL,
                              NULL,
                              0,
                              NULL,
                              userDefinedHeaderLength,
                              &userDefinedHeader,
                              ObexUtilReturnSrmpValue(pMsg->srmpOn),
                              csrBipcRcGetImageResultHandler,
                              NULL));
}

CsrUint8 CsrBtBipcRcGetImageResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcRcGetImageRes *pMsg = (CsrBtBipcRcGetImageRes *) msg;

    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

static void csrBipcRcGetLinkedThumbnailResultHandler(void                  *instData,
                                                     CsrBtObexResponseCode responseCode,
                                                     CsrBool               bodyHeader,
                                                     CsrUint16             bodyLength,
                                                     CsrUint16             bodyOffset,
                                                     CsrUint8            *obexPacket,
                                                     CsrUint16             obexPacketLength)
{
    BipcInstanceData * pInst = (BipcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        csrBtBipcRcGetLinkedThumbnailIndSend(pInst, bodyLength, bodyOffset, obexPacket, obexPacketLength);
    }
    else
    {
        csrBtBipcRcGetLinkedThumbnailCfmSend(pInst, responseCode, bodyLength, bodyOffset, obexPacket, obexPacketLength);
    }
}

CsrUint8 CsrBtBipcRcGetLinkedThumbnailReqHandler(BipcInstanceData *pInst, void *msg)
{
    CsrUint16 userDefinedHeaderLength;

    CsrUint8  headerPriorityTable[5]        = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                               CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtBipcRcGetLinkedThumbnailReq *pMsg  = (CsrBtBipcRcGetLinkedThumbnailReq *) msg;
    CsrUint8 * userDefinedHeader            = csrBtBipcBuildImageHandleHeader(pMsg->imageHandle, &userDefinedHeaderLength);

    return(ObexUtilGetRequest(pInst->obexCliInst,
                              headerPriorityTable,
                              (const CsrUint8*)CSR_BT_BIPC_TEXTTYPE_IMG_THUMB,
                              NULL,
                              NULL,
                              0,
                              NULL,
                              userDefinedHeaderLength,
                              &userDefinedHeader,
                              ObexUtilReturnSrmpValue(pMsg->srmpOn),
                              csrBipcRcGetLinkedThumbnailResultHandler,
                              NULL));
}

CsrUint8 CsrBtBipcRcGetLinkedThumbnailResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailRes *pMsg  = (CsrBtBipcRcGetLinkedThumbnailRes *) msg;

    return (ObexUtilGetContinueRequest(pInst->obexCliInst, 
                                       ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

/* Automatic Archive Downstream handlers */
CsrUint8 CsrBtBipcAaGetImageListHeaderResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetImageListHeaderRes *pMsg = (CsrBtBipcAaGetImageListHeaderRes *) msg;
    if (csrBtBipcResponseCodeAccepted(pMsg->responseCode))
    {
        CsrUint8 returnValue;
        pInst->servMiscInfo = csrBtBipcBuildAppParamsNbReturnedHandlersImgDescripHeader(pMsg->nbReturnedHandles,
                                                                                        pMsg->imageDescriptorLength,
                                                                                        pMsg->imageDescriptor,
                                                                                        &pInst->servMiscInfoLength);

        if (pInst->servObexPacketRspLength > pInst->servMiscInfoLength)
        {
            csrBtBipcAaGetImageListObjectIndSend(pInst, (CsrUint16)(pInst->servObexPacketRspLength - pInst->servMiscInfoLength));
            returnValue = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        }
        else
        {
            returnValue = csrBtBipcAaGetWithHeadersResponseHandler(pInst,
                                                                   CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                                                   CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                                   CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                                   NULL,
                                                                   pMsg->responseCode,
                                                                   0,
                                                                   0,
                                                                   NULL,
                                                                   pInst->servMiscInfoLength,
                                                                   pInst->servMiscInfo,
                                                                   0,
                                                                   NULL,
                                                                   pMsg->srmpOn);
            pInst->servMiscInfo = NULL;
        }
        return (returnValue);
    }
    else
    { /* The application has rejected the OBEX command */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtBipcAaGetImageListObjectResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrUint8 returnValue;
    CsrBtBipcAaGetImageListObjectRes *pMsg = (CsrBtBipcAaGetImageListObjectRes *) msg;

    if (pInst->servMiscInfo)
    {
        returnValue = csrBtBipcAaGetWithHeadersResponseHandler(pInst,
                                                               CSR_BT_OBEX_UTIL_USER_DEFINED_HEADER,
                                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                               CSR_BT_BIPC_BODY_HEADER_INDEX,
                                                               NULL,
                                                               pMsg->responseCode,
                                                               0,
                                                               0,
                                                               NULL,
                                                               pInst->servMiscInfoLength,
                                                               pInst->servMiscInfo,
                                                               pMsg->imagesListObjectLength,
                                                               pMsg->imagesListObject,
                                                               pMsg->srmpOn);
        pInst->servMiscInfo = NULL;
    }
    else
    {
        returnValue = ObexUtilServGetResponse(pInst->obexServInst,
                                              pMsg->responseCode,
                                              &pMsg->imagesListObject,
                                              pMsg->imagesListObjectLength,
                                              ObexUtilReturnSrmpValue(pMsg->srmpOn));
    }
    return returnValue;
}

CsrUint8 CsrBtBipcAaGetCapabilitiesHeaderResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetCapabilitiesHeaderRes *pMsg = (CsrBtBipcAaGetCapabilitiesHeaderRes *) msg;

    if (csrBtBipcResponseCodeAccepted(pMsg->responseCode))
    { /* The application has accepted the OBEX command */
        csrBtBipcAaGetCapabilitiesObjectIndSend(pInst);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected the OBEX command */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtBipcAaGetCapabilitiesObjectResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetCapabilitiesObjectRes *pMsg = (CsrBtBipcAaGetCapabilitiesObjectRes *) msg;
    return (ObexUtilServGetResponse(pInst->obexServInst,
                                    pMsg->responseCode,
                                    &pMsg->capabilitiesObject,
                                    pMsg->capabilitiesObjectLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipcAaGetImagePropertiesHeaderResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetImagePropertiesHeaderRes *pMsg = (CsrBtBipcAaGetImagePropertiesHeaderRes *) msg;

    if (csrBtBipcResponseCodeAccepted(pMsg->responseCode))
    { /* The application has accepted the OBEX command */
        csrBtBipcAaGetImagePropertiesObjectIndSend(pInst, pInst->servObexPacketRspLength);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected the OBEX command */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtBipcAaGetImagePropertiesObjectResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetImagePropertiesObjectRes *pMsg = (CsrBtBipcAaGetImagePropertiesObjectRes *) msg;
    return (ObexUtilServGetResponse(pInst->obexServInst,
                                    pMsg->responseCode,
                                    &pMsg->propertiesObject,
                                    pMsg->propertiesObjectLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipcAaGetImageHeaderResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetImageHeaderRes *pMsg = (CsrBtBipcAaGetImageHeaderRes *) msg;

    if (csrBtBipcResponseCodeAccepted(pMsg->responseCode))
    { /* The application has accepted the OBEX command */
        pInst->servLengthOfObject = pMsg->imageTotalLength;
        csrBtBipcAaGetImageObjectIndSend(pInst, pInst->servObexPacketRspLength);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected the OBEX command */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtBipcAaGetImageObjectResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetImageObjectRes *pMsg = (CsrBtBipcAaGetImageObjectRes *) msg;

    if (pInst->servLengthOfObject == 0)
    {
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        &pMsg->imageObject,
                                        pMsg->imageObjectLength,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
    else
    {
        CsrUint32 lengthOfObject  = pInst->servLengthOfObject;
        pInst->servLengthOfObject = 0;
        return (csrBtBipcAaGetWithHeadersResponseHandler(pInst,
                                                         CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                         CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                         1,
                                                         NULL,
                                                         pMsg->responseCode,
                                                         lengthOfObject,
                                                         0,
                                                         NULL,
                                                         0,
                                                         NULL,
                                                         pMsg->imageObjectLength,
                                                         pMsg->imageObject,
                                                         pMsg->srmpOn));
    }
}

CsrUint8 CsrBtBipcAaGetLinkedThumbnailHeaderResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderRes *pMsg = (CsrBtBipcAaGetLinkedThumbnailHeaderRes *) msg;

    if (csrBtBipcResponseCodeAccepted(pMsg->responseCode))
    { /* The application has accepted the OBEX command */
        csrBtBipcAaGetLinkedThumbnailObjectIndSend(pInst, pInst->servObexPacketRspLength);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected the OBEX command */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtBipcAaGetLinkedThumbnailObjectResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailObjectRes *pMsg = (CsrBtBipcAaGetLinkedThumbnailObjectRes *) msg;
    return (ObexUtilServGetResponse(pInst->obexServInst,
                                    pMsg->responseCode,
                                    &pMsg->thumbnailObject,
                                    pMsg->thumbnailObjectLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipcAaGetLinkedAttachmentHeaderResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderRes *pMsg = (CsrBtBipcAaGetLinkedAttachmentHeaderRes *) msg;
    if (csrBtBipcResponseCodeAccepted(pMsg->responseCode))
    { /* The application has accepted the OBEX command */
        csrBtBipcAaGetLinkedAttachmentObjectIndSend(pInst, pInst->servObexPacketRspLength);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    { /* The application has rejected the OBEX command */
        return (ObexUtilServGetResponse(pInst->obexServInst,
                                        pMsg->responseCode,
                                        NULL,
                                        0,
                                        CSR_BT_OBEX_SRMP_INVALID));
    }
}

CsrUint8 CsrBtBipcAaGetLinkedAttachmentObjectResHandler(BipcInstanceData *pInst, void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentObjectRes *pMsg = (CsrBtBipcAaGetLinkedAttachmentObjectRes *) msg;
    return (ObexUtilServGetResponse(pInst->obexServInst,
                                    pMsg->responseCode,
                                    &pMsg->attachmentObject,
                                    pMsg->attachmentObjectLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtBipcAaDeleteImageResHandler(BipcInstanceData *pInst, void *msg)
{ 
    CsrBtBipcAaDeleteImageRes *pMsg = (CsrBtBipcAaDeleteImageRes *) msg;
    return(ObexUtilServPutResponse(pInst->obexServInst,
                                   pMsg->responseCode,
                                   CSR_BT_OBEX_SRMP_INVALID));
}




