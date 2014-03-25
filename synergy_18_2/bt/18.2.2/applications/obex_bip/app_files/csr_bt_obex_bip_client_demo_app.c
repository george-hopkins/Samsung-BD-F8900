/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "csr_bt_xml_lib.h"
#include <time.h>

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sd_lib.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_lib.h"
#endif /* CSR_AMP_ENABLE */
#include "csr_bt_util.h"
#include "csr_bt_obex.h"
#include "csr_bt_bipc_prim.h"
#include "csr_bt_bipc_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"

#include "csr_unicode.h"

/* Element name used in the image handles descriptor */
static char ImageHandlesDescriptor[]= "image-handles-descriptor";
static char ImageProperties[]       = "image-properties";
static char ImageDescriptor[]       = "image-descriptor";
static char Filter[]                = "filtering-parameters";
static char Image[]                 = "image";
static char Attachment[]            = "attachment";
static char Native[]                = "native";
static char Variant[]               = "variant";

/* Attribute name used in the image handles descriptor */
static char Version[]               = "version";
static char Encoding[]              = "encoding";
static char Pixel[]                 = "pixel";
static char Created[]               = "created";
static char Modified[]              = "modified";
static char Size[]                  = "size";
static char MaxSize[]               = "maxsize";
static char Transformation[]        = "transformation";
static char ContentType[]           = "content-type";
static char Name[]                  = "name";



#define KEYB_CHECK_TIMEOUT                 (100000)
#define DEMO_NUMBER_OF_STATES              (1)

#define KEYBOARD_MENU_STATE                (0)
#define KEYBOARD_SELECT_ADDRESS_STATE      (1)
#define KEYBOARD_SELECT_IMAGE              (2)
#define KEYBOARD_BONDING_STATE             (3)
#define KEYBOARD_AUTHENTICATE_STATE        (4)
#define KEYBOARD_SELECT_ATTACH_FILE        (5)
#define KEYBOARD_SELECT_IMAGE_CAPABILITIES (6)
#define KEYBOARD_SELECT_IMAGE_DESCRIPTOR   (7)

#define MENU_IDLE_STATE                    (0)
#define MENU_SEARCH_STATE                  (1)
#define MENU_PUSH_CONNECTED_STATE          (2)
#define MENU_RC_CONNECTED_STATE            (3)
#define MENU_AA_CONNECTED_STATE            (4)
#define MENU_PUSH_BUSY_STATE               (5)
#define MENU_RC_BUSY_STATE                 (6)

#define OBEX_MAX_PACKET_LENGTH             (30000)/* 255 - 32896 */

#define IMAGE_FILE                         (0)
#define THUMBNAIL                          (1)
#define ATTACHMENT                         (2)

#define DIR_LENGTH                         (10)
#define BIP_FOLDER                         "bip_files"
/* We assume that the following correctly reflect the content
   of the folder 'bip_files' */
#define MON_IMG_1 ("bip_files"PATH_SEP"mon_img_1.jpg")
#define ATT_FILE_1 ("bip_files"PATH_SEP"attach.wav")
#define ATT_FILE_11 ("bip_files"PATH_SEP"attach.txt")
#define IMG_1 ("bip_files"PATH_SEP"img_1.jpg")
#define IMG_2 ("bip_files"PATH_SEP"img_2.jpg")
#define IMG_3 ("bip_files"PATH_SEP"img_3.jpg")
#define IMG_4 ("bip_files"PATH_SEP"mon_img_1.jpg")
#define IMG_5 ("bip_files"PATH_SEP"mon_img_2.jpg")
#define IMG_6 ("bip_files"PATH_SEP"mon_img_3.jpg")
#define IMG_7 ("bip_files"PATH_SEP"test1.jpg")
#define IMG_8 ("bip_files"PATH_SEP"test2.jpg")
#define IMG_9 ("bip_files"PATH_SEP"test3.jpg")
#define IMG_10 ("bip_files"PATH_SEP"test4.jpg")
#define IMG_11 ("bip_files"PATH_SEP"test5.jpg")
#define IMG_12 ("bip_files"PATH_SEP"test6.jpg")
#define IMG_13 ("bip_files"PATH_SEP"test_t.jpg")

#define BIP_DEMO_MAX_INQUIRY_RESULTS    (10)
#define DEFAULT_IMAGE_DESCRIPTOR "< image-descriptor version=\"1.0\" > <image encoding=\"JPEG\" /> < /image-descriptor >"
typedef struct FolderInfoTag
{
    char                  *name;
    CsrTime                  time;
    struct FolderInfoTag  *nextInfoElement;
} FolderInfo;

typedef struct
{
    CsrBool    exist;
    CsrUint8   *fileName;
    CsrUint8   imageHandleString[9];
    CsrUint8  *properties;
    CsrUint8   timeOfCreation[33];
}AutoArchiveImageRecord;

typedef struct
{
    CsrSchedQid              CsrSchedQid;
    void                *recvMsgP;
    CsrUint8            keyboardState;
    CsrUint8            prevKeyboardState;
    CsrBtDeviceAddr     deviceAddr;
    CsrBtDeviceAddr     bondingAddr;
    CsrFileHandle                *currentFileHandle;
    CsrUint32           currentFilePosition;
    CsrUint32           currentFileSize;
    CsrUint8            menuState;
    CsrUint8            addressLength;
    CsrUint8            authenticateLength;
    CsrBool             linkedAttachmentSupported;
    CsrBool             imageSent;
    CsrBool             cancelConnect;
    CsrBool             normalDisconnectFlag;
    CsrUint32           supportedFunctions;
    char                address[12];
    char                imgName[24];
    char                imgThumbName[26];
    char                imgAttachName[22];
    char                authenticatePassword[20];
    CsrUint8            character;
    CsrUint8            passkeyLength;
    CsrUint8            passkey[CSR_BT_PASSKEY_MAX_LEN];
    CsrUint8            imageHandle[CSR_BT_IMG_HANDLE_LENGTH + 1];
    CsrUint8            imageHandleValue;
    CsrUint16           imagePropertiesLength;
    CsrUint8            *imageProperties;
    CsrBool             firstImageProperties;
    CsrBool             indefinite;
    CsrBool             storeImage;
    CsrUint32           connectionId;
    CsrUint16           maxPeerObexPacketSize;
    CsrUint16           sendObjectLength;
    CsrUint16           sendIndex;
    CsrBtDeviceAddr     inquiryResults[BIP_DEMO_MAX_INQUIRY_RESULTS];
    CsrUint8            inquiryResultIndex;
    CsrUint16           requestedNbReturnedHandles;
    CsrUint16           listStartOffset;
    CsrUint16           nbReturnedHandles;
    CsrBool             latestCapturedImages;
    char                imageHandlesStrings[50][9];
    char                createdFileTimesStrings[50][18];
    char                *imgCapabilitiesStr;
    CsrUint16           imgCapabilitiesStrSize;
    char                *imgDescriptorStr;
    CsrUint16           imgDescriptorStrSize;
    char                *propertiesXmlString;

    AutoArchiveImageRecord aaImageTable[13];
    ScAppInstData_t    scInstData;
    CsrBool            chalBack;
} BipcDemoInstanceDataType;

typedef void (* BipcDemoStateHandlerType)(BipcDemoInstanceDataType *bipcDemoInstanceData);

#define XML_HANDLE_FORMAT "\"%07d\""
#define XML_CREATED_FORMAT "\"%04d%02d%02dT%02d%02d%02dZ\""


static char capabilitiesXmlString[] = "<imaging-capabilities version=\"1.0\"><preferred-format encoding=\"JPEG\" pixel=\"640*480\" /><image-formats encoding=\"JPEG\" pixel=\"0*0-65535*65535\" maxsize=\"2758320\"/><attachment-formats content-type=\"audio/32kadpcm\" /><attachment-formats content-type=\"audio/basic\" /><attachment-formats content-type=\"audio/basic\" /><attachment-formats content-type=\"text/plain\" /></imaging-capabilities>";
static char capabilitiesCreationXmlString[] = "<imaging-capabilities version=\"1.0\"> <filtering-parameters creation=\"1\"/></imaging-capabilities>";
static char capabilitiesModificationXmlString[] = "<imaging-capabilities version=\"1.0\"> <filtering-parameters modified=\"1\"/></imaging-capabilities>";
static char capabilitiesPixelXmlString[] = "<imaging-capabilities version=\"1.0\"> <filtering-parameters pixel=\"1\"/></imaging-capabilities>";
static char capabilitiesEncodingXmlString[] = "<imaging-capabilities version=\"1.0\"> <filtering-parameters encoding=\"1\"/></imaging-capabilities>";


static char propertiesXmlString1[]   = "<image-properties version=\"1.0\" handle=\"0000001\" friendly-name=\"img_01.jpg\"><native encoding=\"JPEG\" pixel=\"1136*852\" size=\"78452\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/><attachment content-type=\"audio/32kadpcm\" name=\"attach.wav\" size=\"39700\"/></image-properties>";
static char propertiesXmlString2[]   = "<image-properties version=\"1.0\" handle=\"0000002\" friendly-name=\"img_02.jpg\"><native encoding=\"JPEG\" pixel=\"1136*852\" size=\"74355\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString3[]   = "<image-properties version=\"1.0\" handle=\"0000003\" friendly-name=\"img_03.jpg\"><native encoding=\"JPEG\" pixel=\"1136*852\" size=\"76081\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString4[]   = "<image-properties version=\"1.0\" handle=\"0000004\" friendly-name=\"mon_img_01.jpg\"><native encoding=\"JPEG\" pixel=\"320*240\" size=\"9932\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString5[]   = "<image-properties version=\"1.0\" handle=\"0000005\" friendly-name=\"mon_img_02.jpg\"><native encoding=\"JPEG\" pixel=\"320*240\" size=\"9746\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString6[]   = "<image-properties version=\"1.0\" handle=\"0000006\" friendly-name=\"mon_img_03.jpg\"><native encoding=\"JPEG\" pixel=\"320*240\" size=\"10219\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString7[]   = "<image-properties version=\"1.0\" handle=\"0000007\" friendly-name=\"test1.jpg\"><native encoding=\"JPEG\" pixel=\"160*120\" size=\"21308\"/></image-properties>";
static char propertiesXmlString8[]   = "<image-properties version=\"1.0\" handle=\"0000008\" friendly-name=\"test2.jpg\"><native encoding=\"JPEG\" pixel=\"320*240\" size=\"26864\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString9[]   = "<image-properties version=\"1.0\" handle=\"0000009\" friendly-name=\"test3.jpg\"><native encoding=\"JPEG\" pixel=\"640*480\" size=\"70766\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString10[]   = "<image-properties version=\"1.0\" handle=\"0000010\" friendly-name=\"test4.jpg\"><native encoding=\"JPEG\" pixel=\"800*600\" size=\"122075\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString11[]   = "<image-properties version=\"1.0\" handle=\"0000011\" friendly-name=\"test5.jpg\"><native encoding=\"JPEG\" pixel=\"1024*768\" size=\"197379\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/><attachment content-type=\"text/plain\" name=\"attach.txt\" size=\"11\"/></image-properties>";
static char propertiesXmlString12[]   = "<image-properties version=\"1.0\" handle=\"0000012\" friendly-name=\"test6.jpg\"><native encoding=\"JPEG\" pixel=\"1280*960\" size=\"314390\"/><variant encoding=\"JPEG\" pixel=\"1024*768\" size=\"197379\"/><variant encoding=\"JPEG\" pixel=\"160*120\"/></image-properties>";
static char propertiesXmlString13[]   = "<image-properties version=\"1.0\" handle=\"0000013\" friendly-name=\"test_t.jpg\"><native encoding=\"JPEG\" pixel=\"160*120\" size=\"2608\"/></image-properties>";

static void bipcGetRemoteFeatureCfmHandler(BipcDemoInstanceDataType * bipcDemoInstanceData);
static void bipcConnectCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcDisconnectIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAbortCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcPutImageFileIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcPutThumbnailIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcPutAttachmentFileIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcPutAttachmentCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcPutImageCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAuthenticateIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetCapabilitiesIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetCapabilitiesCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetMonitoringImageCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetMonitoringImageHeaderIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetMonitoringImageFileIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetImagePropertiesIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetImagePropertiesCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetImageCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetImageIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetThumbnailCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcGetThumbnailIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);

static void bipcAaGetImageListHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetImageListObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetCapabilitiesHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetCapabilitiesObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetImagePropertiesHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetImagePropertiesObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetImageHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetImageObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetLinkedThumbnailHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetLinkedThumbnailObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetLinkedAttachmentHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaGetLinkedAttachmentObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void bipcAaDeleteImageIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);

static void bipcAaAbortIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static char * bipcAaInterpretImageDescriptor(CsrUint8 *imageDescriptor, CsrUint16 imageDescriptorLength);
static void keyMessageHandler(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void showMenu(BipcDemoInstanceDataType *bipcDemoInstanceData);
static void sendingTheSelectedFile(BipcDemoInstanceDataType *bipcDemoInstanceData, CsrUint16 maxToSend, CsrUint8 typeToSend);
static void handleSdPrim(BipcDemoInstanceDataType *bipcDemoInstanceData);
#ifdef CSR_AMP_ENABLE
static void handleAmpmPrim(BipcDemoInstanceDataType *bipcDemoInstanceData);
#endif /* CSR_AMP_ENABLE */
static void keyboardMenuStateHandler(BipcDemoInstanceDataType *bipcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectAddressStateHandler(BipcDemoInstanceDataType *bipcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectImageStateHandler(BipcDemoInstanceDataType *bipcDemoInstanceData, CsrUint8 theCharacter);
#if 0
static void keyboardBondingStateHandler(BipcDemoInstanceDataType *bipcDemoInstanceData, CsrUint8 theCharacter);
#endif
static void keyboardAuthenticateStateHandler(BipcDemoInstanceDataType *bipcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectAttachFileStateHandler(BipcDemoInstanceDataType *bipcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectImageCapabilitiesStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectImageDescriptorStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter);
static void KeyMessageService(CsrUint16 m ,void *data);
static void bipcSetImageHandleValue(BipcDemoInstanceDataType * bipcDemoInstanceData, char * payload, CsrUint16 offset);
static CsrUint16 bipcSetPropertiesXmlString(BipcDemoInstanceDataType * bipcDemoInstanceData);
static void bipcSetImageName(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrBool nativeImage, char *pixelStr);
static void interpretImageProperties(CsrUint8 *imageDescriptor, CsrUint16 imageDescriptorLength);

static const BipcDemoStateHandlerType bipcDemoGenStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPC_PRIM_UPSTREAM_COUNT] =
{
    {
        bipcConnectCfmHandler,                      /* CSR_BT_BIPC_CONNECT_CFM */
        bipcAbortCfmHandler,                        /* CSR_BT_BIPC_ABORT_CFM */
        bipcDisconnectIndHandler,                   /* CSR_BT_BIPC_DISCONNECT_IND */
        bipcAuthenticateIndHandler,                 /* CSR_BT_BIPC_AUTHENTICATE_IND */
        NULL,                                       /* CSR_BT_BIPC_SECURITY_OUT_CFM */
        bipcGetRemoteFeatureCfmHandler              /* CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM */
    }
};
static const BipcDemoStateHandlerType bipcDemoPushStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPC_PUSH_PRIM_UPSTREAM_COUNT] =
{

    {
        bipcGetCapabilitiesIndHandler,              /* CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */
        bipcGetCapabilitiesCfmHandler,              /* CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */
        bipcPutImageCfmHandler,                     /* BIPC_PUT_IMAGE_CFM */
        bipcPutImageFileIndHandler,                 /* BIPC_PUT_IMAGE_FILE_IND */
        bipcPutThumbnailIndHandler,                 /* BIPC_PUT_THUMBNAIL_FILE_IND */
        bipcPutAttachmentCfmHandler,                /* BIPC_PUT_ATTACHMENT_CFM */
        bipcPutAttachmentFileIndHandler             /* BIPC_PUT_ATTACHMENT_FILE_IND */
    }
};
static const BipcDemoStateHandlerType bipcDemoRcStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPC_RC_PRIM_UPSTREAM_COUNT] =
{

    {
        bipcGetMonitoringImageCfmHandler,           /* CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM */
        bipcGetMonitoringImageHeaderIndHandler,     /* CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND */
        bipcGetMonitoringImageFileIndHandler,       /* CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */
        bipcGetImagePropertiesIndHandler,           /* CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */
        bipcGetImagePropertiesCfmHandler,           /* CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */
        bipcGetImageCfmHandler,                     /* CSR_BT_BIPC_RC_GET_IMAGE_CFM */
        bipcGetImageIndHandler,                     /* CSR_BT_BIPC_RC_GET_IMAGE_IND */
        bipcGetThumbnailCfmHandler,                 /* CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */
        bipcGetThumbnailIndHandler                  /* CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */

    }
};
static const BipcDemoStateHandlerType bipcDemoAaStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPC_AA_PRIM_UPSTREAM_COUNT] =
{
    {
        bipcAaGetImageListHeaderIndDemoHandler,         /* CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */
        bipcAaGetImageListObjectIndDemoHandler,         /* CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND */
        bipcAaGetCapabilitiesHeaderIndDemoHandler,      /* CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND */
        bipcAaGetCapabilitiesObjectIndDemoHandler,      /* CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND */
        bipcAaGetImagePropertiesHeaderIndDemoHandler,   /* CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */
        bipcAaGetImagePropertiesObjectIndDemoHandler,   /* CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND */
        bipcAaGetImageHeaderIndDemoHandler,             /* CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */
        bipcAaGetImageObjectIndDemoHandler,             /* CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND */
        bipcAaGetLinkedThumbnailHeaderIndDemoHandler,   /* CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */
        bipcAaGetLinkedThumbnailObjectIndDemoHandler,   /* CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND */
        bipcAaGetLinkedAttachmentHeaderIndDemoHandler,  /* CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */
        bipcAaGetLinkedAttachmentObjectIndDemoHandler,  /* CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND */
        bipcAaDeleteImageIndDemoHandler,                 /* CSR_BT_BIPC_AA_DELETE_IMAGE_IND */
        bipcAaAbortIndDemoHandler,                       /* CSR_BT_BIPC_AA_ABORT_IND */
    }
};

static void bipScAppSetFnFini(void *pFiniData)
{
    BipcDemoInstanceDataType *instData = (BipcDemoInstanceDataType *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState);
    system_cls();
    showMenu(instData);
}

static void bipScAppSetFnInit(void *pInitData)
{
    BipcDemoInstanceDataType *instData = (BipcDemoInstanceDataType *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState, KEYBOARD_BONDING_STATE);
}

static void* bipScAppSetFnGetMessage(void *pRecvMsgPData)
{
    BipcDemoInstanceDataType *instData = (BipcDemoInstanceDataType *) pRecvMsgPData;
    void *result                       = instData->recvMsgP;
    instData->recvMsgP                 = NULL;
    return result;
}

/******************************************************************************
  CsrBtAppInit: Allocate and initialise Application instance data space
******************************************************************************/
void CsrBtAppInit(void ** gash)
{
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;
    BipcDemoInstanceDataType * bipcDemoInstanceData;

    *gash                                = CsrPmemAlloc(sizeof(BipcDemoInstanceDataType));
    bipcDemoInstanceData                 = (BipcDemoInstanceDataType *) *gash;
    bipcDemoInstanceData->keyboardState  = KEYBOARD_MENU_STATE;
    bipcDemoInstanceData->menuState      = MENU_IDLE_STATE;

    CsrBtBdAddrZero(&zeroAddr);
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        bipcDemoInstanceData->deviceAddr.lap = defGlobalBdAddr.lap;
        bipcDemoInstanceData->deviceAddr.nap = defGlobalBdAddr.nap;
        bipcDemoInstanceData->deviceAddr.uap = defGlobalBdAddr.uap;
    }
    else
    {
        bipcDemoInstanceData->deviceAddr.lap = 0;
        bipcDemoInstanceData->deviceAddr.nap = 0;
        bipcDemoInstanceData->deviceAddr.uap = 0;
    }
    bipcDemoInstanceData->currentFileHandle         = NULL;
    bipcDemoInstanceData->currentFilePosition       = 0;
    bipcDemoInstanceData->currentFileSize           = 0;
    bipcDemoInstanceData->connectionId              = 0;
    bipcDemoInstanceData->sendObjectLength          = 0;
    bipcDemoInstanceData->sendIndex                 = 0;
    bipcDemoInstanceData->supportedFunctions        = 0;
    bipcDemoInstanceData->linkedAttachmentSupported = FALSE;
    bipcDemoInstanceData->cancelConnect             = FALSE;
    bipcDemoInstanceData->imageSent                 = FALSE;
    bipcDemoInstanceData->normalDisconnectFlag      = TRUE;
    CsrStrCpy(bipcDemoInstanceData->imgName, "bip_files"PATH_SEP);
    CsrStrCpy(bipcDemoInstanceData->imgThumbName, "bip_files"PATH_SEP"test_t.jpg");
    CsrStrCpy(bipcDemoInstanceData->imgAttachName, "bip_files"PATH_SEP);
    bipcDemoInstanceData->imageHandle[0] = '\0';
    bipcDemoInstanceData->imagePropertiesLength = 0;
    bipcDemoInstanceData->imageProperties = NULL;

    /* Set up the image table for autoarchive */
    bipcDemoInstanceData->aaImageTable[0].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[0].fileName = IMG_1;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[0].imageHandleString[0],XML_HANDLE_FORMAT, 1);
    bipcDemoInstanceData->aaImageTable[0].properties = propertiesXmlString1;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[0].timeOfCreation[0],XML_CREATED_FORMAT,
            2007,2,26,9,24,0);

    bipcDemoInstanceData->aaImageTable[1].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[1].fileName = IMG_2;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[1].imageHandleString[0],XML_HANDLE_FORMAT, 2);
    bipcDemoInstanceData->aaImageTable[1].properties = propertiesXmlString2;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[1].timeOfCreation[0],XML_CREATED_FORMAT,
            2007,2,26,9,25,0);

    bipcDemoInstanceData->aaImageTable[2].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[2].fileName = IMG_3;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[2].imageHandleString[0],XML_HANDLE_FORMAT, 3);
    bipcDemoInstanceData->aaImageTable[2].properties = propertiesXmlString3;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[2].timeOfCreation[0],XML_CREATED_FORMAT,
            2007,2,26,9,26,0);

    bipcDemoInstanceData->aaImageTable[3].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[3].fileName = IMG_4;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[3].imageHandleString[0],XML_HANDLE_FORMAT, 4);
    bipcDemoInstanceData->aaImageTable[3].properties = propertiesXmlString4;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[3].timeOfCreation[0],XML_CREATED_FORMAT,
            2007,2,26,9,23,0);

    bipcDemoInstanceData->aaImageTable[4].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[4].fileName = IMG_5;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[4].imageHandleString[0],XML_HANDLE_FORMAT, 5);
    bipcDemoInstanceData->aaImageTable[4].properties = propertiesXmlString5;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[4].timeOfCreation[0],XML_CREATED_FORMAT,
            2007,2,26,9,25,0);

    bipcDemoInstanceData->aaImageTable[5].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[5].fileName = IMG_6;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[5].imageHandleString[0],XML_HANDLE_FORMAT, 6);
    bipcDemoInstanceData->aaImageTable[5].properties = propertiesXmlString6;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[5].timeOfCreation[0],XML_CREATED_FORMAT,
            2007,2,26,9,26,0);

    bipcDemoInstanceData->aaImageTable[6].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[6].fileName = IMG_7;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[6].imageHandleString[0],XML_HANDLE_FORMAT, 7);
    bipcDemoInstanceData->aaImageTable[6].properties = propertiesXmlString7;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[6].timeOfCreation[0],XML_CREATED_FORMAT,
            2004,3,30,9,24,0);

    bipcDemoInstanceData->aaImageTable[7].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[7].fileName = IMG_8;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[7].imageHandleString[0],XML_HANDLE_FORMAT, 8);
    bipcDemoInstanceData->aaImageTable[7].properties = propertiesXmlString8;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[7].timeOfCreation[0],XML_CREATED_FORMAT,
            2004,3,30,9,24,0);

    bipcDemoInstanceData->aaImageTable[8].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[8].fileName = IMG_9;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[8].imageHandleString[0],XML_HANDLE_FORMAT, 9);
    bipcDemoInstanceData->aaImageTable[8].properties = propertiesXmlString9;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[8].timeOfCreation[0],XML_CREATED_FORMAT,
            2004,3,30,9,24,0);

    bipcDemoInstanceData->aaImageTable[9].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[9].fileName = IMG_10;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[9].imageHandleString[0],XML_HANDLE_FORMAT, 10);
    bipcDemoInstanceData->aaImageTable[9].properties = propertiesXmlString10;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[9].timeOfCreation[0],XML_CREATED_FORMAT,
            2004,3,30,9,24,0);

    bipcDemoInstanceData->aaImageTable[10].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[10].fileName = IMG_11;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[10].imageHandleString[0],XML_HANDLE_FORMAT,11);
    bipcDemoInstanceData->aaImageTable[10].properties = propertiesXmlString11;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[10].timeOfCreation[0],XML_CREATED_FORMAT,
            2004,3,30,9,24,0);

    bipcDemoInstanceData->aaImageTable[11].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[11].fileName = IMG_12;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[11].imageHandleString[0],XML_HANDLE_FORMAT, 12);
    bipcDemoInstanceData->aaImageTable[11].properties = propertiesXmlString12;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[11].timeOfCreation[0],XML_CREATED_FORMAT,
            2004,3,30,9,24,0);

    bipcDemoInstanceData->aaImageTable[12].exist = TRUE;
    bipcDemoInstanceData->aaImageTable[12].fileName = IMG_13;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[12].imageHandleString[0],XML_HANDLE_FORMAT, 13);
    bipcDemoInstanceData->aaImageTable[12].properties = propertiesXmlString13;
    sprintf((char *) &bipcDemoInstanceData->aaImageTable[12].timeOfCreation[0],XML_CREATED_FORMAT,
            2004,3,30,9,24,0);


    /** **/
    bipcDemoInstanceData->firstImageProperties = TRUE;

    bipcDemoInstanceData->imgDescriptorStr = NULL;
    bipcDemoInstanceData->imgDescriptorStrSize = 0;

    /* Application handle */
    bipcDemoInstanceData->CsrSchedQid = TESTQUEUE;
    bipcDemoInstanceData->storeImage = FALSE;
    bipcDemoInstanceData->indefinite = FALSE;

    bipcDemoInstanceData->inquiryResultIndex = 0;
    CsrMemSet(&bipcDemoInstanceData->inquiryResults[0], 0, sizeof(bipcDemoInstanceData->inquiryResults));

    bipcDemoInstanceData->chalBack = FALSE;

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    showMenu(bipcDemoInstanceData);

    /* Security App Configuration */
    CsrBtScActivateReqSend(bipcDemoInstanceData->CsrSchedQid);
    scAppInitInstanceData(&bipcDemoInstanceData->scInstData);

    scAppSetFnFini(&bipcDemoInstanceData->scInstData, bipScAppSetFnFini, bipcDemoInstanceData);
    scAppSetFnInit(&bipcDemoInstanceData->scInstData, bipScAppSetFnInit, bipcDemoInstanceData);
    scAppSetSecLevelSend(&bipcDemoInstanceData->scInstData, scAppSendProfileSecurityLevelReq, &bipcDemoInstanceData->scInstData);

    scAppSetFnGetMessage(&bipcDemoInstanceData->scInstData, bipScAppSetFnGetMessage, bipcDemoInstanceData);

    scAppSetSecLevelPrim(&bipcDemoInstanceData->scInstData,
                         0,     /* Index 0. Single profile */
                         "BIPC", CSR_BT_BIPC_PRIM,
                         CSR_BT_BIPC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_BIPC_SECURITY_OUT_REQ);
}

/*******************************************************************************************
  CsrBtAppHandler: Handle incoming event from: the BIPS layer, the SC layer, or the Keyboard
********************************************************************************************/
void CsrBtAppHandler(void ** gash)
{
    CsrUint16    event=0;

    BipcDemoInstanceDataType * bipcDemoInstanceData;

    bipcDemoInstanceData = (BipcDemoInstanceDataType *) *gash;

    CsrSchedMessageGet(&event, &bipcDemoInstanceData->recvMsgP);

    switch (event)
    {
        case CSR_BT_BIPC_PRIM:
            {
                CsrBtBipcPrim    * type;

                type = (CsrBtBipcPrim   *) bipcDemoInstanceData->recvMsgP;

                if (*type == CSR_BT_BIPC_SECURITY_OUT_CFM)
                {
                    handleScSecLevelPrim(&bipcDemoInstanceData->scInstData);
                }
                else if ((*type - CSR_BT_BIPC_PRIM_UPSTREAM_LOWEST) < CSR_BT_BIPC_PRIM_UPSTREAM_COUNT )
                {
                    if( bipcDemoGenStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_PRIM_UPSTREAM_LOWEST)] != NULL)
                    {
                        bipcDemoGenStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_PRIM_UPSTREAM_LOWEST)](bipcDemoInstanceData);
                    }
                }
                else if ((*type - CSR_BT_BIPC_PUSH_PRIM_UPSTREAM_LOWEST) < CSR_BT_BIPC_PUSH_PRIM_UPSTREAM_COUNT )
                {
                    if(bipcDemoPushStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_PUSH_PRIM_UPSTREAM_LOWEST)] != NULL)
                    {
                        bipcDemoPushStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_PUSH_PRIM_UPSTREAM_LOWEST)](bipcDemoInstanceData);
                    }
                }
                else if ((*type - CSR_BT_BIPC_RC_PRIM_UPSTREAM_LOWEST) < CSR_BT_BIPC_RC_PRIM_UPSTREAM_COUNT )
                {
                    if(bipcDemoRcStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_RC_PRIM_UPSTREAM_LOWEST)] != NULL)
                    {
                        bipcDemoRcStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_RC_PRIM_UPSTREAM_LOWEST)](bipcDemoInstanceData);
                    }
                }
                else if ((*type - CSR_BT_BIPC_AA_PRIM_UPSTREAM_LOWEST) < CSR_BT_BIPC_AA_PRIM_UPSTREAM_COUNT )
                {
                    if(bipcDemoAaStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_AA_PRIM_UPSTREAM_LOWEST)] != NULL)
                    {
                        bipcDemoAaStateHandlers[0][(CsrUint16) (*type - CSR_BT_BIPC_AA_PRIM_UPSTREAM_LOWEST)](bipcDemoInstanceData);
                    }
                }
                else
                {
                    printf("A state event error occurred in the BIPC Application layer,\n  it is a CSR_BT_BIPC_PRIM type: %02X\n\n",
                           *type);

                    CsrBtBipcFreeUpstreamMessageContents(event, bipcDemoInstanceData->recvMsgP);
                }
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&bipcDemoInstanceData->scInstData);
                break;
            }
        case CSR_BT_SD_PRIM:
            {
                handleSdPrim(bipcDemoInstanceData);
                break;
            }
#ifdef CSR_AMP_ENABLE
        case CSR_BT_AMPM_PRIM:
            {
                handleAmpmPrim(bipcDemoInstanceData);
                break;
            }
#endif /* CSR_AMP_ENABLE */
        case KEY_MESSAGE:
            {
                keyMessageHandler(bipcDemoInstanceData);
                break;
            }
        default:
            {
                printf("A state event error occurred in the BIPC Application layer,\n  it is a UNKNOWN_PRIM %04X\n",
                       event);
                break;
            }
    }
    CsrPmemFree(bipcDemoInstanceData->recvMsgP);
}


static void bipcSetImageHandleValue(BipcDemoInstanceDataType *instData,  char * payload, CsrUint16 offset)
{
    char number[3];
    if (offset > 0)
    {
        number[0] = payload[offset + 11];
        number[1] = payload[offset + 13];
        number[2] = '\0';
        printf("%c",payload[++offset]);offset++;
        printf("%c",payload[++offset]);offset++;
        printf("%c",payload[++offset]);offset++;
        printf("%c",payload[++offset]);offset++;
        printf("%c",payload[++offset]);offset++;
        printf("%c",payload[++offset]);offset++;
        printf("%c",payload[++offset]);offset++;

        sscanf(number,"%hhu",&instData->imageHandleValue);
    }
    else
    {
        printf("-");
    }
}
static CsrUint16 bipcSetPropertiesXmlString(BipcDemoInstanceDataType * instData)
{
    CsrUint16 strSize = 0;
    switch(instData->imageHandleValue)
    {
        case 1:
            instData->propertiesXmlString = propertiesXmlString1;
            strSize = sizeof(propertiesXmlString1);
            break;
        case 2:
            instData->propertiesXmlString = propertiesXmlString2;
            strSize = sizeof(propertiesXmlString2);
            break;
        case 3:
            instData->propertiesXmlString = propertiesXmlString3;
            strSize = sizeof(propertiesXmlString3);
            break;
        case 4:
            instData->propertiesXmlString = propertiesXmlString4;
            strSize = sizeof(propertiesXmlString4);
            break;
        case 5:
            instData->propertiesXmlString = propertiesXmlString5;
            strSize = sizeof(propertiesXmlString5);
            break;
        case 6:
            instData->propertiesXmlString = propertiesXmlString6;
            strSize = sizeof(propertiesXmlString6);
            break;
        case 7:
            instData->propertiesXmlString = propertiesXmlString7;
            strSize = sizeof(propertiesXmlString7);
            break;
        case 8:
            instData->propertiesXmlString = propertiesXmlString8;
            strSize = sizeof(propertiesXmlString8);
            break;
        case 9:
            instData->propertiesXmlString = propertiesXmlString9;
            strSize = sizeof(propertiesXmlString9);
            break;
        case 10:
            instData->propertiesXmlString = propertiesXmlString10;
            strSize = sizeof(propertiesXmlString10);
            break;
        case 11:
            instData->propertiesXmlString = propertiesXmlString11;
            strSize = sizeof(propertiesXmlString11);
            break;
        case 12:
            instData->propertiesXmlString = propertiesXmlString12;
            strSize = sizeof(propertiesXmlString12);
            break;
        case 13:
            instData->propertiesXmlString = propertiesXmlString13;
            strSize = sizeof(propertiesXmlString13);
            break;
        default:
            instData->propertiesXmlString = propertiesXmlString1;
            strSize = sizeof(propertiesXmlString1);
            break;
    }
    return strSize;
}

void bipcSetImageName(BipcDemoInstanceDataType *instData,  CsrBool nativeImage, char *pixelStr)
{
    if(nativeImage)
    {
        switch(instData->imageHandleValue)
        {
            case 1:
                CsrStrNCpy(instData->imgName, IMG_1, sizeof(IMG_1));
                break;
            case 2:
                CsrStrNCpy(instData->imgName, IMG_2, sizeof(IMG_2));
                break;
            case 3:
                CsrStrNCpy(instData->imgName, IMG_3, sizeof(IMG_3));
                break;
            case 4:
                CsrStrNCpy(instData->imgName, IMG_4, sizeof(IMG_4));
                break;
            case 5:
                CsrStrNCpy(instData->imgName, IMG_5, sizeof(IMG_5));
                break;
            case 6:
                CsrStrNCpy(instData->imgName, IMG_6, sizeof(IMG_6));
                break;
            case 7:
                CsrStrNCpy(instData->imgName, IMG_7, sizeof(IMG_7));
                break;
            case 8:
                CsrStrNCpy(instData->imgName, IMG_8, sizeof(IMG_8));
                break;
            case 9:
                CsrStrNCpy(instData->imgName, IMG_9, sizeof(IMG_9));
                break;
            case 10:
                CsrStrNCpy(instData->imgName, IMG_10, sizeof(IMG_10));
                break;
            case 11:
                CsrStrNCpy(instData->imgName, IMG_11, sizeof(IMG_11));
                break;
            case 12:
                if(!CsrBtStrICmpLength(pixelStr, "1280*960", 7))
                {
                    printf(" 12\n");
                    CsrStrNCpy(instData->imgName, IMG_12, sizeof(IMG_12));
                }
                else if (!CsrBtStrICmpLength(pixelStr, "1024*768", 7))
                {
                    printf(" 11\n");
                    CsrStrNCpy(instData->imgName, IMG_11, sizeof(IMG_11));
                }
                else
                {
                    printf(" default - 12\n");
                   CsrStrNCpy(instData->imgName, IMG_12, sizeof(IMG_12));
                }
                break;
            case 13:
                CsrStrNCpy(instData->imgName, IMG_13, sizeof(IMG_13));
            break;
        default:
            instData->imgName[0] = '\0';
            break;
        }
    }
    else
    {
        switch(instData->imageHandleValue)
        {
            case 1: case 4:
                CsrStrNCpy(instData->imgName, IMG_4, sizeof(IMG_4));
                break;
            case 2: case 5:
                CsrStrNCpy(instData->imgName, IMG_5, sizeof(IMG_5));
                break;
            case 3: case 6:
                CsrStrNCpy(instData->imgName, IMG_6, sizeof(IMG_6));
                break;
            case 7: case 8: case 9: case 10: case 11: case 12: case 13:
                CsrStrNCpy(instData->imgName, IMG_13, sizeof(IMG_13));
                break;
            default:
                CsrStrNCpy(instData->imgName, MON_IMG_1, sizeof(MON_IMG_1));
                break;
        }

    }

}
static void
bipcGetRemoteFeatureCfmHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcGetRemoteFeaturesCfm * prim;

    prim = (CsrBtBipcGetRemoteFeaturesCfm *) bipcDemoInstanceData->recvMsgP;

    /*system(CLS);*/
    showMenu(bipcDemoInstanceData);

    printf("Supported features are: 0x%x\n\n", prim->supportedFeatures);

    if(prim->supportedFeatures & 0x01)
    {
        printf("\tImage Push\n");
    }
    else
    {
        if(prim->supportedFeatures & 0x02)
        {
            printf("\tImage Push - Store\n");
        }
        if(prim->supportedFeatures & 0x04)
        {
            printf("\tImage Push - Print\n");
        }
        if(prim->supportedFeatures & 0x08)
        {
            printf("\tImage Push - Display\n");
        }
    }
    /* 0x10 ImagePull
       0x20 Advanced Image Printing */
    if(prim->supportedFeatures & 0x40)
    {
        printf("\tAuto Archive\n");
    }
    if(prim->supportedFeatures & 0x80)
    {
        printf("\tRemote Camera\n");
    }

}
static void bipcConnectCfmHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcConnectCfm * prim;

    prim = (CsrBtBipcConnectCfm *) bipcDemoInstanceData->recvMsgP;
    bipcDemoInstanceData->cancelConnect        = FALSE;

    if (prim->resultCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
        prim->resultSupplier   == CSR_BT_SUPPLIER_IRDA_OBEX)
    {
        if(bipcDemoInstanceData->character == '3')
        {
            bipcDemoInstanceData->menuState = MENU_PUSH_CONNECTED_STATE;
        }
        else if(bipcDemoInstanceData->character == '4')
        {
            bipcDemoInstanceData->menuState = MENU_RC_CONNECTED_STATE;
        }
        else if(bipcDemoInstanceData->character == '5')
        {
            bipcDemoInstanceData->menuState = MENU_AA_CONNECTED_STATE;
        }
        /* Reset image handle */
        bipcDemoInstanceData->imageHandle[0] = '\0';
        bipcDemoInstanceData->imagePropertiesLength = 0;
        bipcDemoInstanceData->imageProperties = NULL;
        bipcDemoInstanceData->maxPeerObexPacketSize = prim->obexPeerMaxPacketSize;

        /*system(CLS);*/
        bipcDemoInstanceData->supportedFunctions = prim->supportedFunctions;
        showMenu(bipcDemoInstanceData);

        printf("    Maximum memory available for image storage is 0x%02X%02X%02X%02X%02X%02X%02X%02X\n\n",
               prim->totalImagingDataCapacity[0],
               prim->totalImagingDataCapacity[1],
               prim->totalImagingDataCapacity[2],
               prim->totalImagingDataCapacity[3],
               prim->totalImagingDataCapacity[4],
               prim->totalImagingDataCapacity[5],
               prim->totalImagingDataCapacity[6],
               prim->totalImagingDataCapacity[7]);
#ifdef CSR_AMP_ENABLE
                printf("Registering with AMP manager...\n");
                CsrBtAmpmRegisterReqSend(TESTQUEUE,
                                         0, /* flags */
                                         CSR_BT_AMPM_EVENTS_AUTO_MOVE, /* eventMask */
                                         prim->btConnId, /* btConnId */
                                         &bipcDemoInstanceData->deviceAddr); /* addr */
#endif
    }
    else
    {
        /*system(CLS);*/
        printf("\n\nOBEX connection request against %04X %02X %06X, fail with resultCode: 0x%02X, resultSupplier: 0x%02X\n\n",
               bipcDemoInstanceData->deviceAddr.nap,
               bipcDemoInstanceData->deviceAddr.uap,
               bipcDemoInstanceData->deviceAddr.lap,
               prim->resultCode,
               prim->resultSupplier);
        showMenu(bipcDemoInstanceData);
    }
}

static void bipcDisconnectIndHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    if (bipcDemoInstanceData->currentFileHandle)
    {
        CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
        bipcDemoInstanceData->currentFilePosition = 0;
        bipcDemoInstanceData->currentFileHandle = NULL;
    }
        bipcDemoInstanceData->currentFileSize        = 0;


    /*system(CLS);*/
    bipcDemoInstanceData->menuState                  = MENU_IDLE_STATE;
    bipcDemoInstanceData->supportedFunctions         = 0;
    bipcDemoInstanceData->linkedAttachmentSupported  = FALSE;
    bipcDemoInstanceData->imageSent                  = FALSE;
    bipcDemoInstanceData->normalDisconnectFlag       = TRUE;

    showMenu(bipcDemoInstanceData);

}

static void bipcAuthenticateIndHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcAuthenticateInd * prim;

    prim = (CsrBtBipcAuthenticateInd *) bipcDemoInstanceData->recvMsgP;

    printf("\nPlease enter the OBEX response passkey: ");
    bipcDemoInstanceData->authenticateLength = 0;
    bipcDemoInstanceData->keyboardState = KEYBOARD_AUTHENTICATE_STATE;
    CsrPmemFree(prim->realm);
}

static void bipcAbortCfmHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    if (bipcDemoInstanceData->currentFileHandle)
    {
        CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
        bipcDemoInstanceData->currentFilePosition = 0;
        bipcDemoInstanceData->currentFileHandle = NULL;
    }
    bipcDemoInstanceData->currentFileSize      = 0;

    bipcDemoInstanceData->normalDisconnectFlag     = TRUE;
    /*system(CLS);*/
    if( bipcDemoInstanceData->menuState ==  MENU_PUSH_BUSY_STATE)
    {
        bipcDemoInstanceData->menuState =  MENU_PUSH_CONNECTED_STATE;
    }
    else
    {
        bipcDemoInstanceData->menuState =  MENU_RC_CONNECTED_STATE;
    }
    showMenu(bipcDemoInstanceData);
}

static void bipcPutImageFileIndHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcPushPutImageFileInd * prim;

    prim = (CsrBtBipcPushPutImageFileInd *) bipcDemoInstanceData->recvMsgP;

    if (!bipcDemoInstanceData->currentFileHandle)
    {
        int        result;
        CsrBtStat_t statBuffer;

        CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
        statBuffer.st_size    = 0;

        result = CsrBtStat(bipcDemoInstanceData->imgName, &statBuffer);
        if(!result)
        {
            if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen(bipcDemoInstanceData->imgName, "rb")) == NULL)
            {
                printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);
            }
            else
            {
                bipcDemoInstanceData->currentFileSize = statBuffer.st_size;
                sendingTheSelectedFile(bipcDemoInstanceData, prim->imageFileLength, IMAGE_FILE);
            }
        }
        else
        {
            printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);
        }
    }
    else
    {
        sendingTheSelectedFile(bipcDemoInstanceData, prim->imageFileLength, IMAGE_FILE);
    }
}

static void bipcPutThumbnailIndHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcPushPutThumbnailFileInd * prim;

    prim = (CsrBtBipcPushPutThumbnailFileInd *) bipcDemoInstanceData->recvMsgP;

    if (!bipcDemoInstanceData->currentFileHandle)
    {
        int        result;
        CsrBtStat_t statBuffer;

        CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
        statBuffer.st_size    = 0;

        result                = CsrBtStat(bipcDemoInstanceData->imgThumbName, &statBuffer);
        printf("\n\n Sending thumbnail version of image\n");

        if(!result)
        {
            if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen(bipcDemoInstanceData->imgThumbName, "rb")) == NULL)
            {
                printf("Fail. DEMO could not open the file %s\n",
                       bipcDemoInstanceData->imgThumbName);
            }
            else
            {
                bipcDemoInstanceData->currentFileSize = statBuffer.st_size;
                sendingTheSelectedFile(bipcDemoInstanceData,
                                       prim->thumbnailFileLength, THUMBNAIL);
            }
        }
        else
        {
            printf("Fail. DEMO could not open the file %s\n",
                   bipcDemoInstanceData->imgThumbName);
        }
    }
    else
    {
        sendingTheSelectedFile(bipcDemoInstanceData, prim->thumbnailFileLength, THUMBNAIL);
    }
}

static void bipcGetCapabilitiesIndHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrUint32  i;
    CsrBtBipcPushGetCapabilitiesInd    * prim;

    prim = (CsrBtBipcPushGetCapabilitiesInd *) bipcDemoInstanceData->recvMsgP;
    CsrBtBipcPushGetCapabilitiesResSend(FALSE);

    for (i = 0; i < prim->capabilitiesObjectLength; i++)
    {
        printf("%c", prim->payload[prim->capabilitiesObjectOffset + i]);
    }
    CsrPmemFree(prim->payload);
}

static void bipcGetCapabilitiesCfmHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcPushGetCapabilitiesCfm * prim;

    prim = (CsrBtBipcPushGetCapabilitiesCfm *) bipcDemoInstanceData->recvMsgP;

    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        CsrUint32 i;
        printf("Capabilities:\n\n");

        for (i = 0; i < prim->capabilitiesObjectLength; i++)
        {
            printf("%c", prim->payload[prim->capabilitiesObjectOffset + i]);
        }
        printf("\n");
    }
    else
    {
        printf("\nCsrBtBipcPushGetCapabilitiesCfm fail with responseCode %04Xh\n",
               prim->responseCode);
    }
    bipcDemoInstanceData->menuState =  MENU_PUSH_CONNECTED_STATE;
        showMenu(bipcDemoInstanceData);

    bipcDemoInstanceData->normalDisconnectFlag    = TRUE;
    CsrPmemFree(prim->payload);
}

static void bipcPutAttachmentFileIndHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcPushPutAttachmentFileInd * prim;

    prim = (CsrBtBipcPushPutAttachmentFileInd *) bipcDemoInstanceData->recvMsgP;

    if (!bipcDemoInstanceData->currentFileHandle)
    {
        int        result;
        CsrBtStat_t statBuffer;

        CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
        statBuffer.st_size    = 0;

        result                = CsrBtStat(bipcDemoInstanceData->imgAttachName, &statBuffer);
        printf("\n\n Sending attachment\n");

        if(!result)
        {
            if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen(bipcDemoInstanceData->imgAttachName, "rb")) == NULL)
            {
                printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgAttachName);
            }
            else
            {
                bipcDemoInstanceData->currentFileSize = statBuffer.st_size;
                sendingTheSelectedFile(bipcDemoInstanceData, prim->attachmentFileLength, ATTACHMENT);
            }
        }
        else
        {
            printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgAttachName);
        }
    }
    else
    {
        sendingTheSelectedFile(bipcDemoInstanceData, prim->attachmentFileLength, ATTACHMENT);
    }

}

static void bipcPutAttachmentCfmHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcPushPutAttachmentCfm * prim;

    prim = (CsrBtBipcPushPutAttachmentCfm *) bipcDemoInstanceData->recvMsgP;

    /*system(CLS);*/
    bipcDemoInstanceData->menuState =  MENU_PUSH_CONNECTED_STATE;
    showMenu(bipcDemoInstanceData);
    bipcDemoInstanceData->normalDisconnectFlag = TRUE;
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        printf("\n\n Attachment was sent with CSR_BT_OBEX_SUCCESS_RESPONSE_CODE \n");
    }
    else
    {
        if (bipcDemoInstanceData->currentFileHandle)
        {
            CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
            bipcDemoInstanceData->currentFilePosition = 0;
            bipcDemoInstanceData->currentFileHandle = NULL;
        }
        printf("\n\n Fail to send Attachment, ResponseCode is %02Xh\n", prim->responseCode);
    }
}

static void bipcPutImageCfmHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtBipcPushPutImageCfm * prim;

    prim = (CsrBtBipcPushPutImageCfm *) bipcDemoInstanceData->recvMsgP;


    /*system(CLS);*/
    bipcDemoInstanceData->normalDisconnectFlag = TRUE;
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        bipcDemoInstanceData->imageSent = TRUE;
        CsrMemCpy(bipcDemoInstanceData->imageHandle, prim->imageHandle, sizeof(prim->imageHandle));
        printf("\n\n Image was sent with success\n");
    }
    else
    {

        if (bipcDemoInstanceData->currentFileHandle)
        {
            CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
            bipcDemoInstanceData->currentFilePosition    = 0;
            bipcDemoInstanceData->currentFileHandle        = NULL;
        }
        printf("\n\n Fail to send image, ResponseCode is %02Xh\n", prim->responseCode);
    }
    bipcDemoInstanceData->menuState =  MENU_PUSH_CONNECTED_STATE;
    showMenu(bipcDemoInstanceData);
}
static void
bipcGetMonitoringImageCfmHandler(
    BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    if(bipcDemoInstanceData->indefinite)
    {
        bipcDemoInstanceData->menuState =  MENU_RC_BUSY_STATE;
        if(bipcDemoInstanceData->storeImage)
        {
            printf("\n\nRecording ... \n\n");
            bipcDemoInstanceData->indefinite = FALSE;
            showMenu(bipcDemoInstanceData);
        }

        CsrBtBipcRcGetMonitoringImageReqSend(bipcDemoInstanceData->storeImage, FALSE);
    }
    else
    {
        bipcDemoInstanceData->menuState =  MENU_RC_CONNECTED_STATE;
        showMenu(bipcDemoInstanceData);
        printf("\nImage transfer complete\n");
    }


    bipcDemoInstanceData->storeImage = FALSE;
    if(CsrBtFclose(bipcDemoInstanceData->currentFileHandle) == EOF)
    {
        printf("Image could not be stored on local device \n");
    }
    else
    {
#ifdef _WIN32
        /* display image */
        system("start rc_monitor.jpg");
#endif
    }
    bipcDemoInstanceData->currentFileHandle = NULL;
    bipcDemoInstanceData->currentFilePosition = 0;        
}
static void
bipcGetMonitoringImageHeaderIndHandler(
    BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcRcGetMonitoringImageHeaderInd * prim;

    prim = (CsrBtBipcRcGetMonitoringImageHeaderInd *)bipcDemoInstanceData->recvMsgP;

    /* Open file for writing */
    if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen("rc_monitor.jpg", "wb")) == NULL)
    {
        printf("Fail. BIP DEMO APP could not open the file for writing\n");
    }

    /*  save handle in case we want to retrieve it */
    if(prim->imageHandle[0] != '\0')
    {
        CsrMemCpy( bipcDemoInstanceData->imageHandle, prim->imageHandle,
                sizeof(bipcDemoInstanceData->imageHandle));
    }
    CsrBtBipcRcGetMonitoringImageHeaderResSend(FALSE);
}

static void bipcGetMonitoringImageFileIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcRcGetMonitoringImageFileInd * prim;

    prim = (CsrBtBipcRcGetMonitoringImageFileInd *)bipcDemoInstanceData->recvMsgP;
    /* save image to file system */
    CsrBtFwrite((prim->payload + prim->imageFileOffset) ,
              1,
              prim->imageFileLength,
              bipcDemoInstanceData->currentFileHandle);

    CsrPmemFree(prim->payload);
    CsrBtBipcRcGetMonitoringImageFileResSend(FALSE);
}

static void bipcGetImagePropertiesIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcRcGetImagePropertiesInd *prim;

    prim = (CsrBtBipcRcGetImagePropertiesInd *)bipcDemoInstanceData->recvMsgP;

    if(bipcDemoInstanceData->firstImageProperties)
    {
        bipcDemoInstanceData->firstImageProperties = FALSE;
        showMenu(bipcDemoInstanceData);
        printf("\n");
    }


    if(bipcDemoInstanceData->imagePropertiesLength)
    {
        CsrUint8 *tmp;
        CsrUint16 oldLength;

        tmp = bipcDemoInstanceData->imageProperties;
        oldLength = bipcDemoInstanceData->imagePropertiesLength;

        bipcDemoInstanceData->imagePropertiesLength +=
            prim->propertiesObjectLength;
        bipcDemoInstanceData->imageProperties =
        CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);

        CsrMemCpy(bipcDemoInstanceData->imageProperties,
               tmp, oldLength);
        CsrMemCpy(bipcDemoInstanceData->imageProperties,
               (prim->payload + prim->propertiesObjectOffset) ,
               prim->propertiesObjectLength);

        CsrPmemFree(tmp);
    }
    else
    {
        bipcDemoInstanceData->imagePropertiesLength =
            prim->propertiesObjectLength;
        bipcDemoInstanceData->imageProperties =
            (CsrUint8 *)CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
        CsrMemCpy(bipcDemoInstanceData->imageProperties,
               prim->payload + prim->propertiesObjectOffset,
               bipcDemoInstanceData->imagePropertiesLength);
    }

    CsrPmemFree(prim->payload);
    CsrBtBipcRcGetImagePropertiesResSend(FALSE);
}
static void
bipcGetImagePropertiesCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcRcGetImagePropertiesCfm *prim;
    CsrUint8 *str;

    prim = (CsrBtBipcRcGetImagePropertiesCfm *)bipcDemoInstanceData->recvMsgP;

    if(bipcDemoInstanceData->firstImageProperties)
    {
        bipcDemoInstanceData->menuState =  MENU_RC_CONNECTED_STATE;
        showMenu(bipcDemoInstanceData);
        printf("\n");
    }

    if(bipcDemoInstanceData->imagePropertiesLength)
    {
        CsrUint8 *tmp;
        CsrUint16 oldLength;

        tmp = bipcDemoInstanceData->imageProperties;
        oldLength = bipcDemoInstanceData->imagePropertiesLength;

        bipcDemoInstanceData->imagePropertiesLength +=
            prim->propertiesObjectLength;
        bipcDemoInstanceData->imageProperties =
        CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);

        CsrMemCpy(bipcDemoInstanceData->imageProperties,
               tmp, oldLength);
        CsrMemCpy(bipcDemoInstanceData->imageProperties,
               (prim->payload + prim->propertiesObjectOffset) ,
               prim->propertiesObjectLength);

        CsrPmemFree(tmp);
    }
    else
    {
        bipcDemoInstanceData->imagePropertiesLength =
            prim->propertiesObjectLength;
        bipcDemoInstanceData->imageProperties =
            (CsrUint8 *)CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
        CsrMemCpy(bipcDemoInstanceData->imageProperties,
               prim->payload + prim->propertiesObjectOffset,
               bipcDemoInstanceData->imagePropertiesLength);
    }

    CsrPmemFree(prim->payload);

    bipcDemoInstanceData->firstImageProperties = TRUE;
    printf("\nImageProperties Received\n\n");

    str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
    CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
           bipcDemoInstanceData->imagePropertiesLength);
    interpretImageProperties(str,
                             bipcDemoInstanceData->imagePropertiesLength);
    CsrPmemFree(str);

}

static void
bipcGetImageCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcRcGetImageCfm *prim;

    prim = (CsrBtBipcRcGetImageCfm *)bipcDemoInstanceData->recvMsgP;
    /* save image to file system */
    CsrBtFwrite((prim->payload + prim->imageObjectOffset) ,
              1,
              prim->imageObjectLength,
              bipcDemoInstanceData->currentFileHandle);

    CsrPmemFree(prim->payload);

    if(CsrBtFclose(bipcDemoInstanceData->currentFileHandle) == EOF)
    {
        printf("Image could not be stored on local device \n");
    }
    else
    {
        /* display image */
#ifdef _WIN32
        system("start rc_image.jpg");
#endif
    }
    bipcDemoInstanceData->currentFilePosition    = 0;
    bipcDemoInstanceData->currentFileSize        = 0;
    bipcDemoInstanceData->currentFileHandle      = NULL;

    bipcDemoInstanceData->menuState =  MENU_RC_CONNECTED_STATE;
    showMenu(bipcDemoInstanceData);

}
static void
bipcGetImageIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcRcGetImageInd * prim;

    printf(".");
    prim = (CsrBtBipcRcGetImageInd *)bipcDemoInstanceData->recvMsgP;
    /* save image to file system */
    CsrBtFwrite((prim->payload + prim->imageObjectOffset) ,
              1,
              prim->imageObjectLength,
              bipcDemoInstanceData->currentFileHandle);

    CsrPmemFree(prim->payload);
    CsrBtBipcRcGetImageResSend(FALSE);

}
static void
bipcGetThumbnailCfmHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcRcGetLinkedThumbnailCfm *prim;

    prim = (CsrBtBipcRcGetLinkedThumbnailCfm *)bipcDemoInstanceData->recvMsgP;
    /* save image to file system */
    CsrBtFwrite((prim->payload + prim->thumbnailObjectOffset) ,
              1,
              prim->thumbnailObjectLength,
              bipcDemoInstanceData->currentFileHandle);

    CsrPmemFree(prim->payload);

    if(CsrBtFclose(bipcDemoInstanceData->currentFileHandle) == EOF)
    {
        printf("Thumbnail could not be stored on local device \n");
    }
    else
    {
        /* display image */
#ifdef _WIN32
        system("start rc_image_thumb.jpg");
#endif
    }
    bipcDemoInstanceData->currentFileHandle = NULL;
    bipcDemoInstanceData->currentFilePosition = 0;        
    bipcDemoInstanceData->menuState =  MENU_RC_CONNECTED_STATE;
    showMenu(bipcDemoInstanceData);
}
static void bipcGetThumbnailIndHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
  CsrBtBipcRcGetLinkedThumbnailInd * prim;

    printf("*");
    prim = (CsrBtBipcRcGetLinkedThumbnailInd *)bipcDemoInstanceData->recvMsgP;
    /* save image to file system */
    CsrBtFwrite((prim->payload + prim->thumbnailObjectOffset) ,
              1,
              prim->thumbnailObjectLength,
              bipcDemoInstanceData->currentFileHandle);

    CsrPmemFree(prim->payload);
    CsrBtBipcRcGetLinkedThumbnailResSend(FALSE);

}
static CsrBool interpretImageDescriptor(CsrUint8 *imageDescriptor, CsrUint16 imageDescriptorLength, CsrBool *useJpg)
{
    CsrBtElementList * xmlTree;
    CsrBool      returnFlag = TRUE;
    xmlTree = CsrBtBuildXmlTree(imageDescriptor, imageDescriptorLength);

    *useJpg = FALSE;
    if (xmlTree)
    {
        CsrBtRootElementType        * rootElement;

        rootElement    = &(xmlTree->elementArray[0].rootElement);

        if (!CsrBtStrICmp((char *)rootElement->name, ImageHandlesDescriptor))
        {
            if (!CsrBtStrICmp((char *) rootElement->firstAttribute->name, Version))
            {
                CsrBtChildElementType    * childElement;

                if (!CsrMemCmp((const unsigned char *)"1.0", (const unsigned char *)rootElement->firstAttribute->value, 3))
                {
                    ;
                }
                else
                {
                    printf("Version Value parameter is wrong");
                    returnFlag = FALSE;
                }

                childElement = rootElement->firstChildElement;

                while (childElement)
                {
                    if (!CsrBtStrICmp((char *) childElement->name, Filter))
                    {
                        CsrBtElementAttributeType * attribute;

                        attribute = childElement->firstAttribute;

                        while (attribute)
                        {
                            if (!CsrBtStrICmp((char *)attribute->name, Encoding))
                            {
                                if (!CsrMemCmp((const unsigned char *)"JPEG" , (const unsigned char *)attribute->value, 4))
                                {
                                   *useJpg = TRUE;
                                }
                            }
                            else if (!CsrBtStrICmp((char *)attribute->name, Pixel))
                            {
                                ;
                            }
                            else if (!CsrBtStrICmp((char *)attribute->name, Modified))
                            {
                                ;
                            }
                            else if (!CsrBtStrICmp((char *)attribute->name, Created))
                            {
                                ;
                            }
                            else
                            {
                                printf("The following unexpected attribute is found: %s\n", attribute->name);
                                returnFlag = FALSE;
                            }
                            attribute = attribute->nextAttribute;
                        }
                    }
                    else
                    {
                        printf("The following unexpected element is found: %s\n", childElement->name);
                        returnFlag = FALSE;
                    }
                    childElement = childElement->nextSiblingElement;
                }
            }
        }
        else
        {
            printf("The following unexpected element is found: %s", rootElement->name);
            returnFlag = FALSE;
        }
        CsrBtFreeXmlTree(xmlTree);
    }
    else
    {
        printf("The image descriptor is not valid");
        returnFlag = FALSE;
    }
    return (returnFlag);
}
static void interpretImageProperties(CsrUint8 *imageProperties, CsrUint16 imagePropertiesLength)
{
    CsrBtElementList                * xmlTree;

    xmlTree = CsrBtBuildXmlTree(imageProperties, imagePropertiesLength);

    if (xmlTree)
    {
        CsrBtRootElementType        * rootElement;

        rootElement    = &(xmlTree->elementArray[0].rootElement);

        if (!CsrBtStrICmp((char *)rootElement->name, ImageProperties))
        {
            printf("The image-properties is as follows:\n\n");
            if (!CsrBtStrICmp((char *) rootElement->firstAttribute->name, Version))
            {
                CsrBtChildElementType    * childElement;

                printf("%-25.25s: ",rootElement->firstAttribute->name);
                printf("%s\n",rootElement->firstAttribute->value);

                childElement = rootElement->firstChildElement;

                while (childElement)
                {
                    if (!CsrBtStrICmp((char *)childElement->name, Native)              ||
                        !CsrBtStrICmp((char *)childElement->name, Variant)             ||
                        !CsrBtStrICmp((char *)childElement->name, Attachment)
                        )
                    {
                        CsrBtElementAttributeType * attribute;

                        printf("\n%s: \n",  childElement->name);
                        attribute = childElement->firstAttribute;

                        while (attribute)
                        {
                            if (!CsrBtStrICmp((char *)attribute->name, Encoding)            ||
                                !CsrBtStrICmp((char *)attribute->name, Pixel)               ||
                                !CsrBtStrICmp((char *)attribute->name, MaxSize)             ||
                                !CsrBtStrICmp((char *)attribute->name, ContentType)         ||
                                !CsrBtStrICmp((char *)attribute->name, Name)                ||
                                !CsrBtStrICmp((char *)attribute->name, Size)                ||
                                !CsrBtStrICmp((char *)attribute->name, Transformation)
                                )
                            {
                                printf("%-25.25s: ",  attribute->name);
                                printf("%s\n", attribute->value);


                            }
                            else
                            {
                                printf("The following unexpected attribute is found: %s\n", attribute->name);
                            }
                            attribute = attribute->nextAttribute;
                        }
                    }
                    else
                    {
                        printf("The following unexpected element is found: %s\n", childElement->name);
                    }
                    childElement = childElement->nextSiblingElement;
                }
            }
        }
        else
        {
            printf("The following unexpected element is found: %s\n", rootElement->name);
        }
        CsrBtFreeXmlTree(xmlTree);
    }
    else
    {
        printf("The image properties is not valid\n");
    }
}
static void extractImageProperties(CsrUint8 *imageProperties, CsrUint16 imagePropertiesLength)
{
    CsrBtElementList                * xmlTree;

    xmlTree = CsrBtBuildXmlTree(imageProperties, imagePropertiesLength);

    if (xmlTree)
    {
        CsrBtRootElementType        * rootElement;

        rootElement    = &(xmlTree->elementArray[0].rootElement);

        if (!CsrBtStrICmp((char *)rootElement->name, ImageProperties))
        {
            if (!CsrBtStrICmp((char *) rootElement->firstAttribute->name, Version))
            {
                CsrUint8 i = 0;
                CsrBtChildElementType    * childElement;

                childElement = rootElement->firstChildElement;

                while (childElement)
                {
                    if (!CsrBtStrICmp((char *)childElement->name, Native)              ||
                        !CsrBtStrICmp((char *)childElement->name, Variant)
                        )
                    {
                        CsrBtElementAttributeType * attribute;

                        printf("\n%d)  %s: \n", i++, childElement->name);
                        attribute = childElement->firstAttribute;

                        while (attribute)
                        {
                            if (!CsrBtStrICmp((char *)attribute->name, Encoding)            ||
                                !CsrBtStrICmp((char *)attribute->name, Pixel)               ||
                                !CsrBtStrICmp((char *)attribute->name, MaxSize)             ||
                                !CsrBtStrICmp((char *)attribute->name, Size)                ||
                                !CsrBtStrICmp((char *)attribute->name, Transformation)
                                )
                            {
                                printf("%-25.25s: ",  attribute->name);
                                printf("%s\n", attribute->value);


                            }
                            attribute = attribute->nextAttribute;
                        }
                    }
                    childElement = childElement->nextSiblingElement;
                }
            }
        }
        else
        {
            printf("The following unexpected element is found: %s\n", rootElement->name);
        }
        CsrBtFreeXmlTree(xmlTree);
    }
    else
    {
        printf("The image descriptor is not valid or present\n");
    }
}
static CsrUint8 * extractImageDescriptor(CsrUint8 index, CsrUint8 *imageProperties,
                                        CsrUint16 imagePropertiesLength,
                                        CsrUint16 *imgDescLength)
{
    CsrBtElementList  * xmlTree;
    CsrUint8 *imgDesc;

    char *encoding = NULL, *pixel= NULL, *size= NULL;

    *imgDescLength = 0;

    xmlTree = CsrBtBuildXmlTree(imageProperties, imagePropertiesLength);

    if (xmlTree)
        {
        CsrBtRootElementType        * rootElement;

        rootElement    = &(xmlTree->elementArray[0].rootElement);

        if (!CsrBtStrICmp((char *)rootElement->name, ImageProperties))
        {
            if (!CsrBtStrICmp((char *) rootElement->firstAttribute->name, Version))
            {
                CsrUint8 i = 0;
                CsrBtChildElementType    * childElement;

                childElement = rootElement->firstChildElement;

                while (childElement)
                {
                    if (!CsrBtStrICmp((char *)childElement->name, Native)              ||
                        !CsrBtStrICmp((char *)childElement->name, Variant)
                        )
                    {
                        if(index == i)
                        {
                            CsrBtElementAttributeType * attribute;


                            attribute = childElement->firstAttribute;

                            while (attribute)
                            {
                                if(!CsrBtStrICmp((char *)attribute->name, Encoding))
                                {
                                    encoding = attribute->value;
                                }
                                if(!CsrBtStrICmp((char *)attribute->name, Pixel))
                                {
                                    pixel = attribute->value;
                                }
                                if(!CsrBtStrICmp((char *)attribute->name, Size))
                                {
                                    size = attribute->value;
                                }
                                attribute = attribute->nextAttribute;
                            }
                        }
                        i++;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                }
            }
        else
        {
            printf("The following unexpected element is found: %s\n", rootElement->name);
        }
        CsrBtFreeXmlTree(xmlTree);
        *imgDescLength    = CsrBtReturnImgDescriptionLength(encoding, pixel, size, "", "");
        imgDesc          = CsrBtBuildImgDescriptorHeader(encoding, pixel, size, "", "", *imgDescLength);
    }
    else
    {
        printf("The image descriptor is not valid\n");
        imgDesc = NULL;
        *imgDescLength = 0;
    }
    return imgDesc;
}

static CsrUint16 readNofImagesInDir(char *filePattern, BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrUint16 imageCount=0, i;
    if(CsrBtChdir(BIP_FOLDER))
    {
        printf("Unable to locate the directory: %s\n", BIP_FOLDER);
    }

    for(i = 0; i < 13; i++)
    {
        if (bipcDemoInstanceData->aaImageTable[i].exist)
        {
            imageCount++;
        }
    }
    if(CsrBtChdir(".."))
    {
        printf("Unable to locate the directory: %s\n", BIP_FOLDER);
    }

    return imageCount;
}

static void bipcAaGetImageListHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcAaGetImageListHeaderInd * prim;
    CsrBool useJpg = FALSE;
    CsrUint8 *toPrint;
    CsrInt16 nofImages = 0;

    prim = (CsrBtBipcAaGetImageListHeaderInd *)bipcDemoInstanceData->recvMsgP;

    if (bipcDemoInstanceData->connectionId == 0)
    {
        bipcDemoInstanceData->connectionId = prim->connectionId;
    }


    if (prim->imageDescriptorLength > 0)
    {
        CsrUint16 i=0;
        printf("The following filter is to be used for the list of returned images:\n\n");
        toPrint = CsrPmemAlloc(prim->imageDescriptorLength + 1);
        CsrStrNCpy((char *)toPrint, (char *)(prim->payload + prim->imageDescriptorOffset), prim->imageDescriptorLength);

        for(i=0; i< prim->imageDescriptorLength; i++)
            printf("%c",toPrint[i]);
        CsrPmemFree(toPrint);

        interpretImageDescriptor((CsrUint8 *)(prim->payload + prim->imageDescriptorOffset),
                                          (CsrUint16)(prim->imageDescriptorLength - (CsrUint16)7),
                                 &useJpg);
    }
    printf("\n");

    if (useJpg)
    {
        nofImages = readNofImagesInDir("*.jpg",bipcDemoInstanceData);
    }
    else
    {
        nofImages = readNofImagesInDir("*.jpg",bipcDemoInstanceData);
        nofImages = readNofImagesInDir("*.gif",bipcDemoInstanceData);
        nofImages = readNofImagesInDir("*.png",bipcDemoInstanceData);
    }

    if( prim->nbReturnedHandles == 0)
    {
        printf("\nAutoArchive has requested to know the size of the image list (%d)\n", nofImages);
    }
    else
    {
        nofImages = 13;
        if(prim->listStartOffset > 13)
        {
            nofImages = 0;
        }
        else
        {
            nofImages -= prim->listStartOffset;
        }
        if(nofImages > prim->nbReturnedHandles)
        {
            nofImages = prim->nbReturnedHandles;
        }
        printf("\nAutoArchive has requested CSRMAX %i image handles in an image list, and gets %d\n",
               prim->nbReturnedHandles,
               nofImages);

    }

    bipcDemoInstanceData->latestCapturedImages = prim->latestCapturedImages;;
    bipcDemoInstanceData->requestedNbReturnedHandles = prim->nbReturnedHandles;
    bipcDemoInstanceData->listStartOffset = prim->listStartOffset;

    bipcDemoInstanceData->nbReturnedHandles = nofImages;
    CsrPmemFree(prim->payload);
    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        CsrUint8    * imageDescriptor = NULL;
        CsrUint16    imageDescriptorLength = 0;
        if(useJpg)
        {
            imageDescriptorLength = CsrBtReturnImgHandlesDescriptionLength(NULL, NULL, "JPEG", NULL);
            imageDescriptor       = CsrBtBuildImgHandlesDescriptorHeader(NULL, NULL, "JPEG", NULL,imageDescriptorLength);
        }
        CsrBtBipcAaGetImageListHeaderResSend( imageDescriptorLength, imageDescriptor, nofImages, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
    }
    else
    {
        CsrBtBipcAaGetImageListHeaderResSend( 0, NULL, 0, CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, FALSE);
    }
}


static void bipcAaGetImageListObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtObexResponseCode       responseCode;
    CsrUint8                  *listingObject;
    CsrUint16                     length;
    CsrBtBipcAaGetImageListObjectInd *prim;

    prim = (CsrBtBipcAaGetImageListObjectInd *)bipcDemoInstanceData->recvMsgP;

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        CsrUint32 index = bipcDemoInstanceData->currentFilePosition;
        char *buffer;
        CsrUint16 listLength, i, j;
        /* Initialize to silence warnings */
        char *imageListString = NULL;

        if(bipcDemoInstanceData->nbReturnedHandles > 0)
        {
            if( bipcDemoInstanceData->nbReturnedHandles > 13)
            {
                if(bipcDemoInstanceData->listStartOffset < 13)
                {
                   bipcDemoInstanceData->nbReturnedHandles = 13 - bipcDemoInstanceData->listStartOffset;
                }
                else
                {
                    bipcDemoInstanceData->nbReturnedHandles = 0;
                }
            }
            buffer = CsrBtBuildImgListHeader(bipcDemoInstanceData->nbReturnedHandles,
                                        CsrBtReturnImgListLength(bipcDemoInstanceData->nbReturnedHandles),
                                        &listLength);
            imageListString = buffer;

            buffer += listLength;

            if( bipcDemoInstanceData->latestCapturedImages )
            { /* make sure we use cronological order in this case*/
                j = bipcDemoInstanceData->nbReturnedHandles + bipcDemoInstanceData->listStartOffset;
            }

            for(i = bipcDemoInstanceData->listStartOffset;
                i < (bipcDemoInstanceData->nbReturnedHandles + bipcDemoInstanceData->listStartOffset);
                i++)
            {
                if( bipcDemoInstanceData->latestCapturedImages )
                { /* make sure we use cronological order in this case*/
                    j--;
                }
                else
                {
                    j = i;
                }
                if((bipcDemoInstanceData->aaImageTable[j].exist))
                {
                    listLength = 0;
                    buffer =
                        CsrBtBuildImgListObject((CsrUint8 *) buffer,
                                           (char *) &bipcDemoInstanceData->aaImageTable[j].imageHandleString[0],
                                           (char *) &bipcDemoInstanceData->aaImageTable[j].timeOfCreation[0], &listLength);
                    buffer += listLength;
                }
            }
            buffer =
                CsrBtBuildImgListTerminator((CsrUint8 *) buffer);


            for (i=0; i<CsrBtReturnImgListLength(bipcDemoInstanceData->nbReturnedHandles); i++)
            {
                printf("%c", imageListString[i]);
            }
            printf("\n");
        }

        if (bipcDemoInstanceData->currentFileSize == 0)
        {
            /* This is the first BipsAaGetImageListObjectInd message */
            if((bipcDemoInstanceData->requestedNbReturnedHandles > 0) &&
               (bipcDemoInstanceData->nbReturnedHandles > 0))
            {
                bipcDemoInstanceData->currentFileSize =
                    CsrBtReturnImgListLength(bipcDemoInstanceData->nbReturnedHandles);
            }
            else
            {
                bipcDemoInstanceData->currentFileSize = 0;
            }
        }

        if (bipcDemoInstanceData->currentFileSize > prim->allowedObjectLength)
        {
            /* The capabilitiesObject can not be sent in a single packet */
            bipcDemoInstanceData->currentFileSize       -= prim->allowedObjectLength;
            length                                       = prim->allowedObjectLength;
            responseCode                                 = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            bipcDemoInstanceData->currentFilePosition   += length;
        }
        else
        {
            /* The capabilitiesObject or the last part of it is now sent */
            length                                      = (CsrUint16) bipcDemoInstanceData->currentFileSize; /* Since an OBEX package can be no larger than CsrUint16 it is okay to cast here */
            bipcDemoInstanceData->currentFilePosition   = 0;
            bipcDemoInstanceData->currentFileSize       = 0;
            responseCode                                = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            bipcDemoInstanceData->connectionId          = 0;
        }
        if(bipcDemoInstanceData->requestedNbReturnedHandles > 0)
        {
            listingObject = (CsrUint8 *) CsrPmemAlloc(length);
            CsrMemCpy(listingObject, imageListString+index, length);
            CsrPmemFree(imageListString);
        } /* else: send an empty image list object when only the size of the list is requested */
        else
        {
            listingObject   = NULL;
        }
    }
    else
    {
        responseCode    = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        listingObject   = NULL;
        length          = 0;
    }

    CsrBtBipcAaGetImageListObjectResSend( length, listingObject, responseCode, FALSE);

}


/******************************************************************************
 * bipcAaGetCapabilitiesHeaderIndDemoHandler: This Indication signal is the
 * first part of an operation where the client has requested to retrieve the
 * imaging-capabilities object from the BIP server
 *****************************************************************************/
static void bipcAaGetCapabilitiesHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcAaGetCapabilitiesHeaderInd * prim;

    prim = (CsrBtBipcAaGetCapabilitiesHeaderInd *) bipcDemoInstanceData->recvMsgP;

    if (bipcDemoInstanceData->connectionId == 0)
    {
        bipcDemoInstanceData->connectionId = prim->connectionId;
    }
    bipcDemoInstanceData->currentFilePosition = 0;

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {

        printf("\n(a) %s\n\n(b) %s\n\n(c) %s\n\n(d) %s \n\n(e) %s \n\nChoose among one of the above capabilities objects (a is default):  ",
               capabilitiesXmlString,
               capabilitiesCreationXmlString,
               capabilitiesModificationXmlString,
               capabilitiesPixelXmlString,
               capabilitiesEncodingXmlString);

        bipcDemoInstanceData->imgCapabilitiesStr = NULL;
        bipcDemoInstanceData->imgCapabilitiesStrSize = 0;
        bipcDemoInstanceData->keyboardState = KEYBOARD_SELECT_IMAGE_CAPABILITIES;
    }
    else
    {
        CsrBtBipcAaGetCapabilitiesHeaderResSend( CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, FALSE);
    }
}

/******************************************************************************
 * bipcAaGetCapabilitiesObjectIndDemoHandler: This signal is a part of an
 * operation where the client has requested to retrieve the
 * imagingCapabilities object from the BIP server. The imaging-capabilities
 * object is a mandatory object that describes in more detail the various
 * options, formats, and attributes that are supported by the BIP server.
 *****************************************************************************/
static void bipcAaGetCapabilitiesObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtObexResponseCode            responseCode;
    CsrUint8                       *capabilitiesObject;
    CsrUint16                         length;
    CsrBtBipcAaGetCapabilitiesObjectInd   *prim;

    prim = (CsrBtBipcAaGetCapabilitiesObjectInd *) bipcDemoInstanceData->recvMsgP;

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        CsrUint32 index;

        index = bipcDemoInstanceData->currentFilePosition;

        if (bipcDemoInstanceData->currentFileSize == 0)
        {
            /* This is the first BipsPushGetxCapabilitiesObjectInd message */
            bipcDemoInstanceData->currentFileSize = bipcDemoInstanceData->imgCapabilitiesStrSize;
        }

        if (bipcDemoInstanceData->currentFileSize > bipcDemoInstanceData->maxPeerObexPacketSize)
        {
            /* The capabilitiesObject can not be sent in a single packet */
            bipcDemoInstanceData->currentFileSize       -= bipcDemoInstanceData->maxPeerObexPacketSize;
            length                                       = bipcDemoInstanceData->maxPeerObexPacketSize;
            responseCode                                 = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            bipcDemoInstanceData->currentFilePosition   += length;
        }
        else
        {
            /* The capabilitiesObject or the last part of it is now sent */
            length                                      = (CsrUint16) bipcDemoInstanceData->currentFileSize; /* Since an OBEX package can be no larger than CsrUint16 it is okay to cast here */
            bipcDemoInstanceData->currentFilePosition   = 0;
            bipcDemoInstanceData->currentFileSize       = 0;
            responseCode                                = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            bipcDemoInstanceData->connectionId          = 0;
        }
        capabilitiesObject    = (CsrUint8 *) CsrPmemAlloc(length);
        CsrMemCpy(capabilitiesObject, &bipcDemoInstanceData->imgCapabilitiesStr[index], length);
    }
    else
    {
        responseCode       = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        capabilitiesObject = NULL;
        length             = 0;
    }
    CsrBtBipcAaGetCapabilitiesObjectResSend( length, capabilitiesObject, responseCode, FALSE);
}


/******************************************************************************
 * bipcAaGetImagePropertiesHeaderIndDemoHandler: This Indication signal is the
 * first part of an operation where the client has requested to retrieve the
 * image-properties object about an image from the BIP server
 *****************************************************************************/
static void bipcAaGetImagePropertiesHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcAaGetImagePropertiesHeaderInd * prim;

    prim = (CsrBtBipcAaGetImagePropertiesHeaderInd *) bipcDemoInstanceData->recvMsgP;

    if (bipcDemoInstanceData->connectionId == 0)
    {
        bipcDemoInstanceData->connectionId = prim->connectionId;
    }

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {

        printf("Image Properties of imageHandle ");
        bipcSetImageHandleValue(bipcDemoInstanceData, (char *) prim->payload, prim->imageHandleOffset);
        printf(" requested\n");
        CsrBtBipcAaGetImagePropertiesHeaderResSend( CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
    }
    else
    {
        CsrBtBipcAaGetImagePropertiesHeaderResSend( CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, FALSE);
    }
}


/******************************************************************************
 * bipsGetImagePropertiesObjectIndHandler: This signal is a part of an
 * operation where the client has requested to retrieve the
 * image-properties object from the BIP server. The image-properties
 * object describes in detail the various options, formats, and attributes
 * available for a single image.
 *****************************************************************************/
static void bipcAaGetImagePropertiesObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtObexResponseCode              responseCode;
    CsrUint8                         *propertiesObject;
    CsrUint16                           length;
    CsrBtBipcAaGetImagePropertiesObjectInd  *prim;

    prim = (CsrBtBipcAaGetImagePropertiesObjectInd *) bipcDemoInstanceData->recvMsgP;

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        CsrUint16 index;

        index = bipcDemoInstanceData->sendIndex;

        if (bipcDemoInstanceData->sendObjectLength == 0)
        {
            /* This is the first BipsPushGetxCapabilitiesObjectInd message */
            bipcDemoInstanceData->sendObjectLength =
                bipcSetPropertiesXmlString(bipcDemoInstanceData);
        }

        if (bipcDemoInstanceData->sendObjectLength > prim->allowedImageLength)
        {
            /* The capabilitiesObject can not be sent in a single packet */
            bipcDemoInstanceData->sendObjectLength  -= prim->allowedImageLength;
            length                                   = prim->allowedImageLength;
            responseCode                             = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            bipcDemoInstanceData->sendIndex         += length;
        }
        else
        {
            /* The capabilitiesObject or the last part of it is now sent */
            length                                  = bipcDemoInstanceData->sendObjectLength;
            bipcDemoInstanceData->sendIndex         = 0;
            bipcDemoInstanceData->sendObjectLength  = 0;
            responseCode                            = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

        }
        propertiesObject = (CsrUint8 *) CsrPmemAlloc(length);
        CsrMemCpy(propertiesObject, &(bipcDemoInstanceData->propertiesXmlString[index]), length);
    }
    else
    {
        responseCode     = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        propertiesObject = NULL;
        length           = 0;
    }
    CsrBtBipcAaGetImagePropertiesObjectResSend( length, propertiesObject, responseCode, FALSE);
}



/******************************************************************************
 * bipcAaGetImageHeaderIndDemoHandler: This Indication signal is the first part
 * of an operation where the client has requested to retrieve an image
 * object from the BIP server
 *****************************************************************************/
static void bipcAaGetImageHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcAaGetImageHeaderInd * prim;
    char *pixelStr = NULL;
    CsrBtObexResponseCode obexRespCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    prim = (CsrBtBipcAaGetImageHeaderInd *) bipcDemoInstanceData->recvMsgP;

    if (bipcDemoInstanceData->connectionId == 0)
    {
        bipcDemoInstanceData->connectionId = prim->connectionId;
    }

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        printf("Image referenced by imageHandle ");
        bipcSetImageHandleValue(bipcDemoInstanceData, (char *) prim->payload, prim->imageHandleOffset);
        printf(" requested\n");

        if (!bipcDemoInstanceData->currentFileHandle)
        {
            int        result;
            CsrBtStat_t statBuffer;
            CsrBool nativeImage = TRUE;

            if( prim->descriptorLength )
            {
                pixelStr = bipcAaInterpretImageDescriptor((prim->payload + prim->descriptorOffset), prim->descriptorLength);

                if(pixelStr)
                {
                     if (!CsrBtStrICmpLength(pixelStr, "640*480", 7)  ||
                         !CsrBtStrICmpLength(pixelStr, "160*120", 7) )
                     {
                         nativeImage = FALSE;
                     }
                }
            }
            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
            statBuffer.st_size = 0;

            bipcSetImageName(bipcDemoInstanceData, nativeImage, pixelStr);

            result = CsrBtStat(bipcDemoInstanceData->imgName, &statBuffer);
            if(!result)
            {
                if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen(bipcDemoInstanceData->imgName, "rb")) == NULL)
                {
                    printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);

                    obexRespCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                }
                else
                {
                    bipcDemoInstanceData->currentFileSize = statBuffer.st_size;
                }
            }
            else
            {
                obexRespCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);
            }
        }
    }
    else
    {
        obexRespCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
    }

    CsrBtBipcAaGetImageHeaderResSend( obexRespCode, (CsrUint16) bipcDemoInstanceData->currentFileSize, FALSE);

    if (prim->payload != NULL)
    {
        CsrPmemFree(prim->payload);
        prim->payload = NULL;
    }
    CsrPmemFree(pixelStr);
}

/******************************************************************************
 * bipcAaGetImageObjectIndDemoHandler: This signal is a part of an operation where
 * the client has requested to retrieve an image object from the BIP server.
 *****************************************************************************/
static void bipcAaGetImageObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtObexResponseCode   responseCode;
    CsrUint16                length;
    CsrUint8              *imgObject;
    CsrBtBipcAaGetImageObjectInd *prim;

    prim = (CsrBtBipcAaGetImageObjectInd *) bipcDemoInstanceData->recvMsgP;

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        printf("*");
        if (bipcDemoInstanceData->currentFileSize > prim->allowedObjectLength)
        {
            /* The image object can not be sent in a single packet */
            bipcDemoInstanceData->currentFileSize -= prim->allowedObjectLength;
            length                                 = prim->allowedObjectLength;
            responseCode                           = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        else
        {
            printf(" - final part transmitted\n");
            /* The image object or the last part of it is now sent */
            /* The cast from CsrUint32 to CsrUint16 does no discard or corrupt any data
               since to OBEX package cannot be larger than CsrUint16 and this has already
               been checked for. */
            length                                    = (CsrUint16) bipcDemoInstanceData->currentFileSize;
            bipcDemoInstanceData->currentFileSize     = 0;
            responseCode                              = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
        imgObject = CsrPmemAlloc(length);
        bipcDemoInstanceData->currentFilePosition += CsrBtFread(imgObject, 1, length, bipcDemoInstanceData->currentFileHandle);

        if (bipcDemoInstanceData->currentFileSize <= 0)
        {
            CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
            bipcDemoInstanceData->currentFileHandle = NULL;
            bipcDemoInstanceData->currentFilePosition = 0;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        imgObject    = NULL;
        length       = 0;
    }
    CsrBtBipcAaGetImageObjectResSend( length, imgObject, responseCode, FALSE);
}


/******************************************************************************
 * bipcAaGetLinkedThumbnailHeaderIndDemoHandler: This Indication signal is the first part
 * of an operation where the client has requested to retrieve a thumbnail
 * object from the BIP server
 *****************************************************************************/
static void bipcAaGetLinkedThumbnailHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderInd * prim;
    CsrBtObexResponseCode obexRespCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

    prim = (CsrBtBipcAaGetLinkedThumbnailHeaderInd *) bipcDemoInstanceData->recvMsgP;

    if (bipcDemoInstanceData->connectionId == 0)
    {
        bipcDemoInstanceData->connectionId = prim->connectionId;
    }

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        printf("Thumbnail referenced by imageHandle ");
        bipcSetImageHandleValue(bipcDemoInstanceData, (char *) prim->payload, prim->imageHandleOffset);
        printf(" requested\n");

        if (!bipcDemoInstanceData->currentFileHandle)
        {
            int        result;
            CsrBtStat_t statBuffer;

            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
            statBuffer.st_size = 0;
            switch(bipcDemoInstanceData->imageHandleValue)
            {
                case 1: case 4:
                    CsrStrNCpy(bipcDemoInstanceData->imgName, IMG_4, sizeof(IMG_4));
                    break;
                case 2: case 5:
                    CsrStrNCpy(bipcDemoInstanceData->imgName, IMG_5, sizeof(IMG_5));
                    break;
                case 3: case 6:
                    CsrStrNCpy(bipcDemoInstanceData->imgName, IMG_6, sizeof(IMG_6));
                    break;
                case 7: case 8: case 9: case 10: case 11: case 12: case 13:
                    CsrStrNCpy(bipcDemoInstanceData->imgName, IMG_13, sizeof(IMG_13));
                    break;
                default:
                    CsrStrNCpy(bipcDemoInstanceData->imgName, MON_IMG_1, sizeof(MON_IMG_1));
                    break;
            }


            result = CsrBtStat(bipcDemoInstanceData->imgName, &statBuffer);
            if(!result)
            {
                if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen(bipcDemoInstanceData->imgName, "rb")) == NULL)
                {
                    obexRespCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                    printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);
                }
                else
                {
                    bipcDemoInstanceData->currentFileSize = statBuffer.st_size;
                }
            }
            else
            {
                obexRespCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);
            }
        }
    }
    else
    {
        obexRespCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
    }
    CsrBtBipcAaGetLinkedThumbnailHeaderResSend( obexRespCode, FALSE);
    if (prim->payload != NULL)
    {
        CsrPmemFree(prim->payload);
        prim->payload = NULL;
    }
}


/******************************************************************************
 * bipcAaGetLinkedThumbnailObjectIndHandler: This signal is a part of an operation where
 * the client has requested to retrieve a thumbnail object from the BIP server.
 *****************************************************************************/
static void bipcAaGetLinkedThumbnailObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtObexResponseCode             responseCode;
    CsrUint16                          length;
    CsrUint8                        *imgObject;
    CsrBtBipcAaGetLinkedThumbnailObjectInd *prim;

    prim = (CsrBtBipcAaGetLinkedThumbnailObjectInd *) bipcDemoInstanceData->recvMsgP;

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        printf("*");
        if (bipcDemoInstanceData->currentFileSize > prim->allowedObjectLength)
        {
            /* The monitoring image object can not be sent in a single packet */
            bipcDemoInstanceData->currentFileSize -= prim->allowedObjectLength;
            length                                 = prim->allowedObjectLength;
            responseCode                           = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        else
        {
            printf(" - final part transmitted\n");
            /* The monitoring image object or the last part of it is now sent */
            /* The cast from CsrUint32 to CsrUint16 does no discard or corrupt any data
               since to OBEX package cannot be larger than CsrUint16 and this has already
               been checked for. */
            length                                    = (CsrUint16) bipcDemoInstanceData->currentFileSize;
            bipcDemoInstanceData->currentFilePosition = 0;
            bipcDemoInstanceData->currentFileSize     = 0;
            responseCode                              = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

        }
        imgObject = CsrPmemAlloc(length);
        bipcDemoInstanceData->currentFilePosition += CsrBtFread(imgObject, 1, length, bipcDemoInstanceData->currentFileHandle);

        if (bipcDemoInstanceData->currentFileSize <= 0)
        {
            CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
            bipcDemoInstanceData->currentFilePosition = 0;
            bipcDemoInstanceData->currentFileHandle = NULL;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        imgObject    = NULL;
        length       = 0;
    }
    CsrBtBipcAaGetLinkedThumbnailObjectResSend( length, imgObject, responseCode, FALSE);
}


/******************************************************************************
 * bipcAaGetLinkedAttachmentHeaderIndDemoHandler: This Indication signal is the first part
 * of an operation where the client has requested to retrieve an attachment
 * object from the BIP server
 *****************************************************************************/
static void bipcAaGetLinkedAttachmentHeaderIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderInd * prim;
    CsrBtObexResponseCode obexRespCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

    prim = (CsrBtBipcAaGetLinkedAttachmentHeaderInd *) bipcDemoInstanceData->recvMsgP;

    if (bipcDemoInstanceData->connectionId == 0)
    {
        bipcDemoInstanceData->connectionId = prim->connectionId;
    }

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        printf(" Retrieving attachment file referenced by imageHandle ");
        bipcSetImageHandleValue(bipcDemoInstanceData, (char *) prim->payload, prim->imageHandleOffset);
        printf(" \n");


        if (!bipcDemoInstanceData->currentFileHandle)
        {
            int        result;
            CsrBtStat_t statBuffer;
            CsrBool fileExist = TRUE;

            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
            statBuffer.st_size = 0;
            switch(bipcDemoInstanceData->imageHandleValue)
            {
                case 1:
                    CsrStrNCpy(bipcDemoInstanceData->imgName, ATT_FILE_1, sizeof(ATT_FILE_1));
                    break;
                case 11:
                    CsrStrNCpy(bipcDemoInstanceData->imgName, ATT_FILE_11, sizeof(ATT_FILE_11));
                    break;
                default:
                    obexRespCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                    fileExist = FALSE;
                    printf("This image do not have any attachment! \n");
                    break;
            }

            if(fileExist)
            {
                printf("file name is '%s'\n",bipcDemoInstanceData->imgName);
                result = CsrBtStat(bipcDemoInstanceData->imgName, &statBuffer);
                if(!result)
                {
                    if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen(bipcDemoInstanceData->imgName, "rb")) == NULL)
                    {
                        printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);
                        obexRespCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                    }
                    else
                    {
                        bipcDemoInstanceData->currentFileSize = statBuffer.st_size;
                    }
                }
                else
                {
                    obexRespCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                    printf("Fail. DEMO could not open the file %s\n", bipcDemoInstanceData->imgName);
                }
            }
        }
    }
    else
    {
        obexRespCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
    }

    CsrBtBipcAaGetLinkedAttachmentHeaderResSend( obexRespCode, FALSE);

    if (prim->payload != NULL)
    {
        CsrPmemFree(prim->payload);
        prim->payload = NULL;
    }
}


/******************************************************************************
 * bipcAaGetLinkedAttachmentObjectIndDemoHandler: This signal is a part of an operation where
 * the client has requested to retrieve a thumbnail object from the BIP server.
 *****************************************************************************/
static void bipcAaGetLinkedAttachmentObjectIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtObexResponseCode              responseCode;
    CsrUint16                           length;
    CsrUint8                         *imgObject;
    CsrBtBipcAaGetLinkedAttachmentObjectInd *prim;

    prim = (CsrBtBipcAaGetLinkedAttachmentObjectInd *) bipcDemoInstanceData->recvMsgP;

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        printf("*");
        if (bipcDemoInstanceData->currentFileSize > prim->allowedObjectLength)
        {
            /* The object can not be sent in a single packet */

            bipcDemoInstanceData->currentFileSize -= prim->allowedObjectLength;
            length                                 = prim->allowedObjectLength;
            responseCode                           = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        else
        {
            printf(" - final part transmitted\n");
            /* The object or the last part of it is now sent */
            /* The cast from CsrUint32 to CsrUint16 does no discard or corrupt any data
               since to OBEX package cannot be larger than CsrUint16 and this has already
               been checked for. */
            length                                    = (CsrUint16) bipcDemoInstanceData->currentFileSize;
            bipcDemoInstanceData->currentFilePosition = 0;
            bipcDemoInstanceData->currentFileSize     = 0;
            responseCode                              = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
        imgObject = CsrPmemAlloc(length);
        bipcDemoInstanceData->currentFilePosition += CsrBtFread(imgObject, 1, length, bipcDemoInstanceData->currentFileHandle);

        if (bipcDemoInstanceData->currentFileSize <= 0)
        {
            CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
            bipcDemoInstanceData->currentFilePosition = 0;
            bipcDemoInstanceData->currentFileHandle = NULL;
        }
    }
    else
    {
        printf("PrimConnection id %d expected connection id %d\n",
               prim->connectionId, bipcDemoInstanceData->connectionId);
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        imgObject    = NULL;
        length       = 0;
    }
    CsrBtBipcAaGetLinkedAttachmentObjectResSend( length, imgObject, responseCode, FALSE);
}


static void bipcAaDeleteImageIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    CsrBtBipcAaDeleteImageInd * prim;

    prim = (CsrBtBipcAaDeleteImageInd *) bipcDemoInstanceData->recvMsgP;

    if (bipcDemoInstanceData->connectionId == 0)
    {
        bipcDemoInstanceData->connectionId = prim->connectionId;
    }

    if (prim->connectionId == bipcDemoInstanceData->connectionId)
    {
        CsrUint8 index;
        bipcSetImageHandleValue(bipcDemoInstanceData, (char *) prim->payload, prim->imageHandleOffset);
        index = bipcDemoInstanceData->imageHandleValue - 1;

        if(bipcDemoInstanceData->aaImageTable[index].exist)
        {
            bipcDemoInstanceData->aaImageTable[index].exist = FALSE;
            printf(" The BIP demo has deleted image (%s) referenced by image handle \n",bipcDemoInstanceData->aaImageTable[index].fileName);
            CsrBtBipcAaDeleteImageResSend( CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        }
        else
        {
                printf(" Failed to delete file (%s)\n", bipcDemoInstanceData->aaImageTable[index].fileName);
            CsrBtBipcAaDeleteImageResSend( CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE);
        }
    }
    else
    {
        printf("Failed to delete file \n" );
        CsrBtBipcAaDeleteImageResSend( CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE);
    }

    if (prim->payload != NULL)
    {
        CsrPmemFree(prim->payload);
        prim->payload = NULL;
    }
}

void bipcAaAbortIndDemoHandler(BipcDemoInstanceDataType *bipcDemoInstanceData)
{
    printf("\n\n Abort Ind received \n\n");
    if(bipcDemoInstanceData->currentFileHandle)
    {
        CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
        bipcDemoInstanceData->currentFilePosition = 0;
        bipcDemoInstanceData->currentFileHandle = NULL;
    }

    bipcDemoInstanceData->currentFilePosition = 0;
    bipcDemoInstanceData->currentFileSize = 0;

}

static char * bipcAaInterpretImageDescriptor(CsrUint8 *imageDescriptor, CsrUint16 imageDescriptorLength)
{
    CsrBtElementList                * xmlTree;
    char *pixelStr = NULL;
    xmlTree = CsrBtBuildXmlTree(imageDescriptor, imageDescriptorLength);


    if (xmlTree)
    {
        CsrBtRootElementType        * rootElement;

        rootElement    = &(xmlTree->elementArray[0].rootElement);

        if (!CsrBtStrICmp((char *)rootElement->name, ImageDescriptor))
        {
            printf("The image the client wants the server to provide for it,\nhas the  following properties:\n\n");
            if (!CsrBtStrICmp((char *) rootElement->firstAttribute->name, Version))
            {
                CsrBtChildElementType    * childElement;

                printf("%-25.25s: ",rootElement->firstAttribute->name);
                printf("%s\n",rootElement->firstAttribute->value);

                childElement = rootElement->firstChildElement;

                while (childElement)
                {
                    if (!CsrBtStrICmp((char *) childElement->name, Image))
                    {
                        CsrBtElementAttributeType * attribute;

                        attribute = childElement->firstAttribute;

                        while (attribute)
                        {
                            if (!CsrBtStrICmp((char *)attribute->name, Encoding)           ||
                                !CsrBtStrICmp((char *)attribute->name, Pixel)              ||
                                !CsrBtStrICmp((char *)attribute->name, MaxSize)            ||
                                !CsrBtStrICmp((char *)attribute->name, Size)               ||
                                !CsrBtStrICmp((char *)attribute->name, Transformation)
                                )
                            {
                                printf("%-25.25s: ",  attribute->name);
                                printf("%s\n", attribute->value);


                            }
                            else
                            {
                                printf("The following unexpected attribute is found: %s\n", attribute->name);
                            }
                            if (!CsrBtStrICmp((char *)attribute->name, Pixel))
                            {
                                pixelStr = CsrPmemAlloc(21);
                                CsrMemCpy(pixelStr, attribute->value,
                                       20);
                                pixelStr[20] = '\0';
                            }
                            attribute = attribute->nextAttribute;
                        }
                    }
                    else
                    {
                        printf("The following unexpected element is found: %s\n", childElement->name);
                    }
                    childElement = childElement->nextSiblingElement;
                }
            }
        }
        else
        {
            printf("The following unexpected element is found: %s\n", rootElement->name);
        }
        CsrBtFreeXmlTree(xmlTree);
    }
    else
    {
        printf("The image descriptor is not valid\n");
    }
    return pixelStr;
}

static void keyMessageHandler(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t*)bipcDemoInstanceData->recvMsgP;

    if (key->key == ESCAPE_KEY)
    {
        if(bipcDemoInstanceData->imgDescriptorStr)
        {
            CsrPmemFree(bipcDemoInstanceData->imgDescriptorStr);
            bipcDemoInstanceData->imgDescriptorStr = NULL;
        }
        if(bipcDemoInstanceData->imageProperties)
        {
            CsrPmemFree(bipcDemoInstanceData->imageProperties);
            bipcDemoInstanceData->imageProperties = NULL;
        }
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }
    else
    {
        switch (bipcDemoInstanceData->keyboardState)
        {
            case KEYBOARD_MENU_STATE :
                {
                    keyboardMenuStateHandler(bipcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_ADDRESS_STATE:
                {
                    keyboardSelectAddressStateHandler(bipcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_IMAGE :
                {
                    keyboardSelectImageStateHandler(bipcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_BONDING_STATE :
                {
                    handleScKeybPrim(&bipcDemoInstanceData->scInstData);
                    break;
                }
            case KEYBOARD_AUTHENTICATE_STATE :
                {
                    keyboardAuthenticateStateHandler(bipcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_ATTACH_FILE:
                {
                    keyboardSelectAttachFileStateHandler(bipcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_IMAGE_CAPABILITIES:
                {
                    keyboardSelectImageCapabilitiesStateHandler(bipcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_IMAGE_DESCRIPTOR:
                {
                    keyboardSelectImageDescriptorStateHandler(bipcDemoInstanceData, key->key);
                    break;
                }

        }
    }
}

static void handleSdPrim(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    CsrPrim    *primType;

    primType = (CsrPrim *) bipcDemoInstanceData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_SD_SEARCH_RESULT_IND:
            {
                CsrBtSdSearchResultInd * prim;
                CsrUint16 nameLen;
                CsrUint8 *name;

                prim = (CsrBtSdSearchResultInd *) bipcDemoInstanceData->recvMsgP;

                nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);

                printf("   -- %02d --\n", bipcDemoInstanceData->inquiryResultIndex);

                if (nameLen > 0)
                {
                    printf("   %s\n", name);
                    CsrPmemFree(name);
                }
                printf("   %04X:%02X:%06X\n",
                    prim->deviceAddr.nap,
                    prim->deviceAddr.uap,
                    prim->deviceAddr.lap);
                printf("   %s\n\n", CodDecodeDeviceClass(prim->deviceClass));

                CsrBtBdAddrCopy(&bipcDemoInstanceData->inquiryResults[bipcDemoInstanceData->inquiryResultIndex], &prim->deviceAddr);

                bipcDemoInstanceData->inquiryResultIndex == BIP_DEMO_MAX_INQUIRY_RESULTS - 1 ?
                    bipcDemoInstanceData->inquiryResultIndex = 0 : bipcDemoInstanceData->inquiryResultIndex++;

                CsrPmemFree(prim->info);
                break;
            }
        case CSR_BT_SD_CLOSE_SEARCH_IND:
            {
                bipcDemoInstanceData->addressLength    = 0;
                bipcDemoInstanceData->keyboardState    = KEYBOARD_SELECT_ADDRESS_STATE;
                printf("\n\nPlease type an address, or press enter to select default: %04X %02X %06X\n",
                       bipcDemoInstanceData->deviceAddr.nap,
                       bipcDemoInstanceData->deviceAddr.uap,
                       bipcDemoInstanceData->deviceAddr.lap);
                break;
            }
        default:
            {
                /* Unexpected primitive received */
                printf("####### default in sdPrim handler %04Xh,\n",*primType);
                CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, bipcDemoInstanceData->recvMsgP);
            }
    }
}

/* Handle AMPM primitives */
#ifdef CSR_AMP_ENABLE
static void handleAmpmPrim(BipcDemoInstanceDataType *inst)
{
    switch(*(CsrBtAmpmPrim*)inst->recvMsgP)
    {
        case CSR_BT_AMPM_REGISTER_CFM:
            {
                CsrBtAmpmRegisterCfm *prim;
                prim = (CsrBtAmpmRegisterCfm*)inst->recvMsgP;
                printf("AMPM register confirm: btConnId 0x%08x, result 0x%04x, supplier 0x%04x\n",
                       prim->btConnId, prim->resultCode, prim->resultSupplier);
            }
            break;

        case CSR_BT_AMPM_DEREGISTER_CFM:
            {
                CsrBtAmpmDeregisterCfm *prim;
                prim = (CsrBtAmpmDeregisterCfm*)inst->recvMsgP;
                printf("AMPM unregister confirm: btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_CONTROLLER_IND:
        case CSR_BT_AMPM_CONTROLLER_CFM:
            {
                CsrBtAmpmControllerInd *prim;
                CsrUint8 i;

                prim = (CsrBtAmpmControllerInd*)inst->recvMsgP;
                printf("AMP controller listing (supplier 0x%04x error code 0x%04x)\n",
                       prim->resultSupplier, prim->resultCode);

                for(i = 0; i < prim->ampsCount; i++)
                {
                    printf("  ampType:0x%02x status:0x%02x caps:0x%08x localId:0x%04x remoteId:0x%04x\n",
                           prim->amps[i].ampType,
                           prim->amps[i].status,
                           prim->amps[i].caps,
                           prim->amps[i].localId,
                           prim->amps[i].remoteId);
                }
                printf("\n");

                if((prim->resultSupplier == CSR_BT_SUPPLIER_AMPM) &&
                   (prim->resultCode == CSR_BT_AMPM_RESULT_SUCCESS))
                {
                    printf("Initiating move to controller 0x%04x/0x%04x...\n",
                           prim->amps[0].localId, prim->amps[0].remoteId);
                    CsrBtAmpmMoveReqSend(inst->connectionId,
                                         prim->amps[0].localId, prim->amps[0].remoteId);
                }
                else
                {
                    printf("Could not initiate move\n");
                }

                CsrPmemFree(prim->amps);
                prim->amps = NULL;
            }
            break;

        case CSR_BT_AMPM_MOVE_IND:
            {
                CsrBtAmpmMoveInd *prim;
                prim = (CsrBtAmpmMoveInd*)inst->recvMsgP;
                printf("AMP move channel indication, btConnId 0x%08x, localId 0x%04x - accepting...\n",
                       prim->btConnId, prim->localId);
                CsrBtAmpmMoveResSend(prim->btConnId, TRUE);
            }
            break;


        case CSR_BT_AMPM_MOVE_CMP_IND:
            {
                CsrBtAmpmMoveCmpInd *prim;
                prim = (CsrBtAmpmMoveCmpInd*)inst->recvMsgP;
                printf("AMP move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_MOVE_CFM:
            {
                CsrBtAmpmMoveCfm *prim;
                prim = (CsrBtAmpmMoveCfm*)inst->recvMsgP;
                printf("AMP move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        case CSR_BT_AMPM_AUTO_MOVE_CMP_IND:
            {
                CsrBtAmpmAutoMoveCmpInd *prim;
                prim = (CsrBtAmpmAutoMoveCmpInd*)inst->recvMsgP;
                printf("AMP auto move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                       prim->btConnId, prim->resultSupplier, prim->resultCode);
            }
            break;

        default:
            printf("Unhandled AMPM primitive received: type 0x%04x\n",
                   *(CsrPrim*)inst->recvMsgP);
            break;
    }
}
#endif

static void showMenu(BipcDemoInstanceDataType * bipcDemoInstanceData)
{
    /*system(CLS);*/
    printf("\nOBEX BIP Client:\n\n");

    switch (bipcDemoInstanceData->menuState)
    {
        case MENU_IDLE_STATE:
            {
                printf("\nProgram options:\n\n");
                printf("    1) Search for Obex Image Push devices\n");
                printf("    2) Retrieve remote features available\n");
                printf("    3) To initiates an OBEX connection (ImagePush)\n");
                printf("    4) To initiates an OBEX connection (RemoteCamera)\n");
                printf("    5) To initiates an OBEX connection (Automatic Archive)\n");
                printf("    9) Security Controller Configuration\n");
                printf("    a) To initiates an OBEX connection with Auth: CSR\n");
                printf("\n    c) Toggle re-challenge %s\n", bipcDemoInstanceData->chalBack ? "OFF" : "ON");
                break;
            }
        case MENU_SEARCH_STATE:
            {
                printf("\nProgram options:\n\n");
                printf("    c) Stop searching for Obex Image Push devices\n");
                break;
            }
        case MENU_PUSH_CONNECTED_STATE:
            {
                printf("Image Push Connected\n\n");
                printf("\nProgram options:\n\n");
                printf("    4) To retrieve the imaging-capabilities object\n");
                printf("    5) To push an image\n");
                if (bipcDemoInstanceData->linkedAttachmentSupported && bipcDemoInstanceData->imageSent)
                {
                    printf("    6) To send attachments associated with an image\n");
                }
                printf("    8) To release the OBEX connection\n");
                printf("    9) Security Controller Configuration\n");
                break;
            }
        case MENU_RC_CONNECTED_STATE:
            printf("Remote Camera Connected\n\n");
            printf("\nProgram options:\n\n");
            if(bipcDemoInstanceData->indefinite)
            {
                printf("    0) Cancel getting monitoring image \n");
            }
            else
            {
                printf("    0) To get monitoring image - continuesly \n");
            }
            printf("    1) To get monitoring image\n");
            printf("    2) To get and store monitoring image\n");

            if((bipcDemoInstanceData->imageHandle[0] != '\0') &&
               (!bipcDemoInstanceData->indefinite))
            {
                printf("    3) To get image properties '%s'\n",
                       bipcDemoInstanceData->imageHandle);
                printf("    4) To get image thumbnail '%s'\n",
                       bipcDemoInstanceData->imageHandle);
                printf("    5) To get image '%s' using image descriptor\n",
                       bipcDemoInstanceData->imageHandle);
                printf("    6) To get image '%s' with out sending image descriptor\n",
                       bipcDemoInstanceData->imageHandle);
            }
            printf("    8) To release the OBEX connection\n");
            printf("    9) Security Controller Configuration\n");
            break;
        case  MENU_AA_CONNECTED_STATE:
            printf("Automatic Archive Connected\n\n\tActing as primary client & secondary server\n");

            printf(" Waiting on action from peer ...\n\n");
            printf("\nProgram options:\n\n");
            printf("    8) To release the OBEX connection early\n");
            printf("    9) Security Controller Configuration\n");
            break;
        case MENU_RC_BUSY_STATE:
            printf("Remote Camera Connected\n\n");
            printf("\nProgram options:\n\n");
            if(bipcDemoInstanceData->indefinite)
            {
                printf("    0) Cancel getting monitoring image \n");
                printf("    1) To get monitoring image\n");
                printf("    2) To get and store monitoring image\n");
            }
            printf("\n    7) To abort/cancels the current operation\n");
            break;
        case MENU_PUSH_BUSY_STATE:
            printf("Image Push Connected\n\n");
            printf("\nProgram options:\n\n");
            printf("    7) To abort/cancels the current operation\n");
            break;
        default:
            break;

    }

    printf("\nHit <ESC> to quit program!\n\n");

    if (bipcDemoInstanceData->supportedFunctions > 0)
    {

        if(bipcDemoInstanceData->menuState == MENU_PUSH_CONNECTED_STATE)
        {
            printf("\n The following function are supported:\n\n");
        if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_GET_CAPABILITIES_MASK) == CSR_BT_GET_CAPABILITIES_MASK)
        {
            printf("    GetCapabilities\n");
        }
        if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_PUT_IMAGE_MASK) == CSR_BT_PUT_IMAGE_MASK)
        {
            printf("    PutImage\n");
        }
        if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_PUT_LINKED_ATTACHMENT_MASK) == CSR_BT_PUT_LINKED_ATTACHMENT_MASK)
        {
            printf("    PutLinkedAttachment\n");
            bipcDemoInstanceData->linkedAttachmentSupported    = TRUE;
        }
        if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_PUT_LINKED_THUMBNAIL_MASK) == CSR_BT_PUT_LINKED_THUMBNAIL_MASK)
        {
            printf("    PutLinkedThumbnail\n");
        }
    }
        else if (bipcDemoInstanceData->menuState == MENU_RC_CONNECTED_STATE)
        {
            printf("\n The following function are supported:\n\n");
            if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_GET_MONITORING_IMAGE_MASK) == CSR_BT_GET_MONITORING_IMAGE_MASK)
            {
                printf("    GetMonitoringImage\n");
            }
            if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_GET_IMAGE_PROPERTIES_MASK) == CSR_BT_GET_IMAGE_PROPERTIES_MASK)
            {
                printf("    GetImageProperties\n");
            }
            if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_GET_IMAGE_MASK) == CSR_BT_GET_IMAGE_MASK)
            {
                printf("    GetImage\n");
            }
            if ((bipcDemoInstanceData->supportedFunctions & CSR_BT_GET_LINKED_THUMBNAIL_MASK) == CSR_BT_GET_LINKED_THUMBNAIL_MASK)
            {
                printf("    GetLinkedThumbnail\n");
            }
        }
    }
}

static void sendingTheSelectedFile(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint16 maxToSend, CsrUint8 typeToSend)
{
    CsrUint32                bytesToRead;
    CsrBool                    finalFlag;

    if( (bipcDemoInstanceData->currentFileSize - bipcDemoInstanceData->currentFilePosition) <= maxToSend)
    {
        bytesToRead = bipcDemoInstanceData->currentFileSize - bipcDemoInstanceData->currentFilePosition;
        finalFlag = TRUE;
    }
    else
    {
        bytesToRead = maxToSend;
        finalFlag = FALSE;
    }

    if(bytesToRead > 0)
    {
        CsrUint16                bytesRead;
        CsrUint8                * image;

        image = (CsrUint8 *) CsrPmemAlloc(bytesToRead);
        bytesRead = CsrBtFread(image, 1, bytesToRead, bipcDemoInstanceData->currentFileHandle);
        bipcDemoInstanceData->currentFilePosition += bytesRead;
        printf("*");
        switch(typeToSend)
        {
            case IMAGE_FILE:
                {
                    CsrBtBipcPushPutImageFileResSend(finalFlag, bytesRead, image);
                    break;
                }
            case THUMBNAIL:
                {
                    CsrBtBipcPushPutThumbnailFileResSend(finalFlag, bytesRead, image);
                    break;
                }
            case ATTACHMENT:
                {
                    CsrBtBipcPushPutLinkedAttachmentFileResSend(finalFlag, bytesRead, image);
                    break;
                }
        }
    }
    if(bipcDemoInstanceData->currentFilePosition >= bipcDemoInstanceData->currentFileSize)
    {
        CsrBtFclose(bipcDemoInstanceData->currentFileHandle);
        bipcDemoInstanceData->currentFilePosition    = 0;
        bipcDemoInstanceData->currentFileHandle        = NULL;
    }
}

static void keyboardMenuStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    switch(bipcDemoInstanceData->menuState)
    {
        case MENU_IDLE_STATE:
            {
                switch (theCharacter)
                {
                    case '1':
                        {
                            CsrBtSdSearchReqSend(bipcDemoInstanceData->CsrSchedQid,
                                          CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                                          6000, /* RSSI buffer timeout */
                                          CSR_BT_INFINITE_TIME, /* Search timeout */
                                          CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                                          CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                                          CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                                          CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                                          0, /* Filter length */
                                          NULL); /* Filter */

                            bipcDemoInstanceData->menuState = MENU_SEARCH_STATE;
                            /*system(CLS);*/
                            bipcDemoInstanceData->inquiryResultIndex = 0;
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case 'a':
                    case 'A':
                        {
                            /*system(CLS);*/
                            bipcDemoInstanceData->keyboardState = KEYBOARD_SELECT_ADDRESS_STATE;
                            bipcDemoInstanceData->addressLength    = 0;
                            bipcDemoInstanceData->character        = theCharacter;
                            printf("\n\nPlease type an address, or press enter to select default: %04X %02X %06X\n",
                                   bipcDemoInstanceData->deviceAddr.nap,
                                   bipcDemoInstanceData->deviceAddr.uap,
                                   bipcDemoInstanceData->deviceAddr.lap);
                            break;
                        }
                    case '8':
                        {
                            if (bipcDemoInstanceData->cancelConnect)
                            {
                                /*system(CLS);*/
                                printf("\n\n _Releasing the OBEX connection\n");
                                CsrBtBipcDisconnectReqSend(FALSE);
                                bipcDemoInstanceData->cancelConnect = FALSE;
                            }
                            else
                            {
                                printf("####### default in keybPrim handler %x,\n", theCharacter);
                            }
                            break;
                        }
                    case '9':
                        {
                            handleScKeybPrim(&bipcDemoInstanceData->scInstData);
                            break;
                        }
                    case 'C':
                    case 'c': /* Fall-through intended */
                        {
                            /*system(CLS);*/
                            bipcDemoInstanceData->chalBack = !bipcDemoInstanceData->chalBack;
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                    case ' ':
                        {
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                    default:
                        {
                            printf("####### default in keybPrim handler %x,\n", theCharacter);
                            break;
                        }
                }
                break;
            }
        case MENU_SEARCH_STATE:
            {
                switch (theCharacter)
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                        {
                            /* Select device */
                            CsrBtDeviceAddr deviceAddr = bipcDemoInstanceData->inquiryResults[theCharacter - '0'];
                            printf("Selecting device: %04X:%02X:%06X\n", deviceAddr.nap, deviceAddr.uap, deviceAddr.lap);
                            CsrBtBdAddrCopy(&bipcDemoInstanceData->deviceAddr, &deviceAddr);
                        }   /* Intentional fall-through */
                    case 'c':
                    case 'C':
                        {
                            CsrBtSdCancelSearchReqSend(bipcDemoInstanceData->CsrSchedQid);
                            break;
                        }
                    case ' ':
                        {
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                    default:
                        {
                            printf("####### default in keybPrim handler %x,\n", theCharacter);
                            break;
                        }
                }
                break;
            }
        case MENU_PUSH_CONNECTED_STATE:
            {
                switch (theCharacter)
                {
                    case '4':
                        {
                            /*system(CLS);*/
                            CsrBtBipcPushGetCapabilitiesReqSend(FALSE);

                            bipcDemoInstanceData->menuState = MENU_PUSH_BUSY_STATE;
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                    case '5':
                        {
                            /*system(CLS);*/
                            printf("    1)  160*120 jpeg\n");
                            printf("    2)  320*240 jpeg\n");
                            printf("    3)  640*480 jpeg\n");
                            printf("    4)  800*600 jpeg\n");
                            printf("    5) 1024*768 jpeg\n");
                            printf("    6) 1280*960 jpeg\n");
                            bipcDemoInstanceData->keyboardState = KEYBOARD_SELECT_IMAGE;
                            break;
                        }
                    case '6':
                        {
                            if (bipcDemoInstanceData->linkedAttachmentSupported && bipcDemoInstanceData->imageSent)
                            {
                                /*system(CLS);*/
                                printf("    1)  wav file\n");
                                printf("    2)  plain text file\n");
                                printf("\n    a) change image handle from current (%s)\n", bipcDemoInstanceData->imageHandle);
                                bipcDemoInstanceData->keyboardState = KEYBOARD_SELECT_ATTACH_FILE;
                            }
                            else
                            {
                                printf("####### default in keybPrim handler %x,\n", theCharacter);
                            }
                            break;
                        }
                    case '8':
                        {
                            /*system(CLS);*/
                            printf("\n\n Releasing the OBEX connection\n");
                            bipcDemoInstanceData->menuState = MENU_IDLE_STATE;
                            if (bipcDemoInstanceData->normalDisconnectFlag)
                            {
                                CsrBtBipcDisconnectReqSend(TRUE);
                            }
                            else
                            {
                                CsrBtBipcDisconnectReqSend(FALSE);
                            }
                            break;
                        }
                    case '9':
                        {
                            handleScKeybPrim(&bipcDemoInstanceData->scInstData);
                            break;
                        }
                    case ' ':
                        {
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                }
                break;
            }
        case MENU_RC_CONNECTED_STATE:
            {
                switch (theCharacter)
                {
                    case '0':
                        if(bipcDemoInstanceData->indefinite)
                        {
                            bipcDemoInstanceData->indefinite = FALSE;
                        }
                        else
                        {
                            bipcDemoInstanceData->indefinite = TRUE;
                            CsrBtBipcRcGetMonitoringImageReqSend(FALSE, FALSE);
                            bipcDemoInstanceData->menuState = MENU_RC_BUSY_STATE;
                            showMenu(bipcDemoInstanceData);
                        }
                        break;
                    case '1':
                        if(bipcDemoInstanceData->indefinite)
                        {
                            bipcDemoInstanceData->indefinite = FALSE;
                            bipcDemoInstanceData->storeImage = FALSE;
                            printf("    Retrieving monitoring image\n");
                        }
                        else
                        {
                            CsrBtBipcRcGetMonitoringImageReqSend(FALSE, FALSE);
                            bipcDemoInstanceData->menuState = MENU_RC_BUSY_STATE;
                            showMenu(bipcDemoInstanceData);
                        }
                        break;
                    case '2':
                        if(bipcDemoInstanceData->indefinite)
                        {
                            bipcDemoInstanceData->storeImage = TRUE;
                            printf("    Storing monitoring image\n");
                        }
                        else
                        {
                            CsrBtBipcRcGetMonitoringImageReqSend(TRUE, FALSE);
                            bipcDemoInstanceData->menuState = MENU_RC_BUSY_STATE;
                            showMenu(bipcDemoInstanceData);
                        }
                        break;
                    case '3':
                        if(bipcDemoInstanceData->indefinite)
                        {
                            bipcDemoInstanceData->indefinite = FALSE;
                            printf(" Stopping indefinite getting of monitoring\n");
                        }
                        else
                        {
                            if(bipcDemoInstanceData->imageHandle[0] != '\0')
                            {
                                CsrBtBipcRcGetImagePropertiesReqSend(
                                                            bipcDemoInstanceData->imageHandle,
                                                            FALSE);
                                bipcDemoInstanceData->menuState = MENU_PUSH_BUSY_STATE;
                                showMenu(bipcDemoInstanceData);
                                printf("    Retrieving image properties \n");
                            }
                            else
                            {
                                showMenu(bipcDemoInstanceData);
                                printf(" You must first store (2) an image on the server\n");
                            }
                        }
                        break;
                    case '4':
                        if(bipcDemoInstanceData->indefinite)
                        {
                            bipcDemoInstanceData->indefinite = FALSE;
                            printf(" Stopping indefinite getting of monitoring\n");
                        }
                        else
                        {
                            if(bipcDemoInstanceData->imageHandle[0] != '\0')
                            {
                                CsrBtBipcRcGetLinkedThumbnailReqSend(
                                                                     bipcDemoInstanceData->imageHandle,
                                                                     FALSE);
                                bipcDemoInstanceData->menuState = MENU_RC_BUSY_STATE;
                                showMenu(bipcDemoInstanceData);
                                printf("\n\n   Retrieving thumbnail ");
                                    /* Open file for writing */
                                if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen("rc_image_thumb.jpg", "wb")) == NULL)
                                {
                                    printf("Fail. BIP DEMO APP could not open the file for writing\n");
                                }

                            }
                            else
                            {
                                showMenu(bipcDemoInstanceData);
                                printf(" You must first store (2) an image on the server\n");
                            }
                        }
                        break;
                    case '5':
                        if(bipcDemoInstanceData->indefinite)
                        {
                            bipcDemoInstanceData->indefinite = FALSE;
                            printf(" Stopping indefinite getting of monitoring\n");
                        }
                        else
                        {
                            if(bipcDemoInstanceData->imageHandle[0] != '\0')
                            {
                                CsrUint8 *str;
                                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                                       bipcDemoInstanceData->imagePropertiesLength);
                                printf("\n Please choose image descriptor : ");
                                extractImageProperties(str,
                                                       bipcDemoInstanceData->imagePropertiesLength);
                                CsrPmemFree(str);

                                printf("\nt) to choose thumbnail (encoding= JPEG pixel= 160*120)");
                                printf("\n<enter> for no image descriptor\n");


                                bipcDemoInstanceData->keyboardState = KEYBOARD_SELECT_IMAGE_DESCRIPTOR;
                            }
                            else
                            {
                                showMenu(bipcDemoInstanceData);
                                printf(" You must first store (2) an image on the server\n");
                            }
                        }
                        break;
                    case '6':
                        if(bipcDemoInstanceData->indefinite)
                        {
                            bipcDemoInstanceData->indefinite = FALSE;
                            printf(" Stopping indefinite getting of monitoring\n");
                        }
                        else
                        {
                            if(bipcDemoInstanceData->imageHandle[0] != '\0')
                            {
                                CsrBtBipcRcGetImageReqSend(bipcDemoInstanceData->imageHandle,
                                                           0,
                                                           NULL,
                                                           FALSE);

                                    /* Open file for writing */
                                if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen("rc_image.jpg", "wb")) == NULL)
                                {
                                    showMenu(bipcDemoInstanceData);
                                    printf("Fail. BIP DEMO APP could not open the file for writing\n");
                                }
                                else
                                {
                                    bipcDemoInstanceData->menuState = MENU_RC_BUSY_STATE;
                                    printf("\n\n   Retrieving image ");
                                    showMenu(bipcDemoInstanceData);
                                }


                            }
                            else
                            {
                                showMenu(bipcDemoInstanceData);
                                printf(" You must first store (2) an image on the server\n");
                            }
                        }
                        break;
                    case '8':
                        {
                            printf("\n\n Releasing the OBEX connection\n");
                            bipcDemoInstanceData->menuState = MENU_IDLE_STATE;
                            if (bipcDemoInstanceData->normalDisconnectFlag)
                            {
                                CsrBtBipcDisconnectReqSend(TRUE);
                            }
                            else
                            {
                                CsrBtBipcDisconnectReqSend(FALSE);
                            }
                            break;
                        }
                    case '9':
                        {
                            handleScKeybPrim(&bipcDemoInstanceData->scInstData);
                            break;
                        }
                    case ' ':
                        {
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                    default:
                        {
                            printf("####### default in keybPrim handler %x,\n", theCharacter);
                            break;
                        }
                }
                break;
            }
        case MENU_AA_CONNECTED_STATE:
            {
                switch (theCharacter)
                {
                    case '8':
                        {
                            /*system(CLS);*/
                            printf("\n\n Releasing the OBEX connection\n");
                            bipcDemoInstanceData->menuState = MENU_IDLE_STATE;
                            if (bipcDemoInstanceData->normalDisconnectFlag)
                            {
                                CsrBtBipcDisconnectReqSend(TRUE);
                            }
                            else
                            {
                                CsrBtBipcDisconnectReqSend(FALSE);
                            }
                            break;
                        }
                    case ' ':
                        {
                            showMenu(bipcDemoInstanceData);
                            break;
                        }
                    default:
                        {
                            printf("####### default in keybPrim handler %x,\n", theCharacter);
                            break;
                        }

                }
                break;
            }
        case MENU_PUSH_BUSY_STATE:
            switch (theCharacter)
            {
                case '7':
                    /*system(CLS);*/
                    printf("\n\n Cancelling the current operation\n");
                    CsrBtBipcAbortReqSend();
                    break;
                default :

                    printf("####### default in keybPrim handler %x,\n", theCharacter);
                    break;
            }
            break;

        case MENU_RC_BUSY_STATE:
            switch (theCharacter)
            {
                case '0':
                    if(bipcDemoInstanceData->indefinite)
                    {
                        bipcDemoInstanceData->indefinite = FALSE;
                    }
                    break;
                case '1':
                    if(bipcDemoInstanceData->indefinite)
                    {
                        bipcDemoInstanceData->indefinite = FALSE;
                        bipcDemoInstanceData->storeImage = FALSE;
                        printf("    Retrieving monitoring image\n");
                        }
                    break;
                case '2':
                    if(bipcDemoInstanceData->indefinite)
                    {
                        bipcDemoInstanceData->storeImage = TRUE;
                        printf("    Storing monitoring image\n");
                    }
                    break;
                case '7':
                    printf("\n\n Cancelling the current operation\n");
                    CsrBtBipcAbortReqSend();
                    break;
                default :
                    printf("####### default in keybPrim handler %x,\n", theCharacter);
                    break;
            }
            break;
        default :
            {
                printf("####### default in keybPrim handler %x,\n", theCharacter);
                break;
            }

    }
}

static void keyboardSelectAddressStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    switch(theCharacter)
    {
        case RETURN_KEY :
            {
                bipcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                if (bipcDemoInstanceData->addressLength == 12)
                {
                    CsrUint8 uap;
                    CsrUint32 tmplap;
                    sscanf(bipcDemoInstanceData->address, "%04X", &bipcDemoInstanceData->deviceAddr.nap);
                    sscanf(&bipcDemoInstanceData->address[4], "%02X", &uap);
                    sscanf(&bipcDemoInstanceData->address[6], "%06X", &tmplap);
                    bipcDemoInstanceData->deviceAddr.lap = tmplap;
                    bipcDemoInstanceData->deviceAddr.uap = uap;
                }

                if (bipcDemoInstanceData->menuState == MENU_SEARCH_STATE)
                {
                    bipcDemoInstanceData->menuState = MENU_IDLE_STATE;
                    /*system(CLS);*/
                    showMenu(bipcDemoInstanceData);
                }
                else
                {
                    /*system(CLS);*/
                    if (bipcDemoInstanceData->character == '2')
                    {
                        printf("\n\nRetrieving available features of %04X %02X %06X\n",
                               bipcDemoInstanceData->deviceAddr.nap,
                               bipcDemoInstanceData->deviceAddr.uap,
                               bipcDemoInstanceData->deviceAddr.lap);
                        CsrBtBipcGetRemoteFeaturesReqSend(bipcDemoInstanceData->CsrSchedQid,
                                                         bipcDemoInstanceData->deviceAddr,
                                                         FALSE);
                    }
                    else if (bipcDemoInstanceData->character == '3')
                    {
                        printf("\n\nInitiates an OBEX connection (ImagePush) against %04X %02X %06X\n",
                               bipcDemoInstanceData->deviceAddr.nap,
                               bipcDemoInstanceData->deviceAddr.uap,
                               bipcDemoInstanceData->deviceAddr.lap);
                        CsrBtBipcConnectReqSend(bipcDemoInstanceData->CsrSchedQid,
                                                OBEX_MAX_PACKET_LENGTH,
                                                bipcDemoInstanceData->deviceAddr,
                                                CSR_BT_IMAGE_PUSH_FEATURE,
                                                FALSE,
                                                0,
                                                NULL,
                                                0,
                                                NULL,
                                                NULL,
                                                0, /*length*/
                                                0, /*count*/
                                                0, /*windowsize*/
                                                TRUE); /*enableSrm*/
                        bipcDemoInstanceData->cancelConnect = TRUE;
                        printf("\n\n    8) To cancel the OBEX connection request\n");
                    }
                    else if (bipcDemoInstanceData->character == '4')
                    {
                        printf("\n\nInitiates an OBEX connection (RemoteCamera) against %04X %02X %06X\n",
                               bipcDemoInstanceData->deviceAddr.nap,
                               bipcDemoInstanceData->deviceAddr.uap,
                               bipcDemoInstanceData->deviceAddr.lap);
                        CsrBtBipcConnectReqSend(bipcDemoInstanceData->CsrSchedQid,
                                                OBEX_MAX_PACKET_LENGTH,
                                                bipcDemoInstanceData->deviceAddr,
                                                CSR_BT_REMOTE_CAMERA_FEATURE,
                                                FALSE,
                                                0,
                                                NULL,
                                                0,
                                                NULL,
                                                NULL,
                                                0, /*length*/
                                                0, /*count*/
                                                0, /*windowsize*/
                                                TRUE); /*enableSrm*/

                        bipcDemoInstanceData->cancelConnect = TRUE;
                        printf("\n\n    8) To cancel the OBEX connection request\n");
                    }
                    else if (bipcDemoInstanceData->character == '5')
                    {
                        printf("\n\nInitiates an OBEX connection (AutoArchive) against %04X %02X %06X\n",
                               bipcDemoInstanceData->deviceAddr.nap,
                               bipcDemoInstanceData->deviceAddr.uap,
                               bipcDemoInstanceData->deviceAddr.lap);
                        CsrBtBipcConnectReqSend(bipcDemoInstanceData->CsrSchedQid,
                                                OBEX_MAX_PACKET_LENGTH,
                                                bipcDemoInstanceData->deviceAddr,
                                                CSR_BT_AUTO_ARCHIVE_FEATURE,
                                                FALSE,
                                                0,
                                                NULL,
                                                0,
                                                NULL,
                                                NULL,
                                                0, /*length*/
                                                0, /*count*/
                                                0, /*windowsize*/
                                                TRUE); /*enableSrm*/

                        bipcDemoInstanceData->cancelConnect = TRUE;
                        printf("\n\n    8) To cancel the OBEX connection request\n");
                    }
                    else if (bipcDemoInstanceData->character == 'a' || bipcDemoInstanceData->character == 'A')
                    {
                        printf("\n\nInitiates an OBEX connection against %04X %02X %06X with Auth\n",
                               bipcDemoInstanceData->deviceAddr.nap,
                               bipcDemoInstanceData->deviceAddr.uap,
                               bipcDemoInstanceData->deviceAddr.lap);
                        CsrBtBipcConnectReqSend(bipcDemoInstanceData->CsrSchedQid,
                                                OBEX_MAX_PACKET_LENGTH,
                                                bipcDemoInstanceData->deviceAddr,
                                                CSR_BT_IMAGE_PUSH_FEATURE,
                                                TRUE,
                                                0,
                                                NULL,
                                                (CsrUint16)CsrStrLen("CSR"),
                                                (CsrUint8 *)CsrStrDup("CSR"),
                                                NULL,
                                                0, /*length*/
                                                0, /*count*/
                                                0, /*windowsize*/
                                                TRUE); /*enableSrm*/
                        
                        bipcDemoInstanceData->cancelConnect = TRUE;
                        printf("\n\n    8) To cancel the OBEX connection request\n");

                    }
                    else
                    {
                        CsrBtScBondReqSend(bipcDemoInstanceData->CsrSchedQid,
                                      bipcDemoInstanceData->deviceAddr);

                        printf("Initiating bonding with selected device %04X %02X %06X. Please wait .\n",
                               bipcDemoInstanceData->deviceAddr.nap,
                               bipcDemoInstanceData->deviceAddr.uap,
                               bipcDemoInstanceData->deviceAddr.lap);
                        break;
                    }
                }
                break;
            }
        case BACKSPACE_KEY :
            {
                if(bipcDemoInstanceData->addressLength > 0)
                {
                    bipcDemoInstanceData->addressLength--;
                    printf("\b \b");
                }
                break;
            }
        default :
            {
                if(bipcDemoInstanceData->addressLength > 11)
                {

                }
                else
                {
                    bipcDemoInstanceData->address[bipcDemoInstanceData->addressLength] = theCharacter;
                    bipcDemoInstanceData->addressLength++;
                    printf("%c", theCharacter);
                }
                break;
            }
    }
}

static void keyboardSelectImageStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    CsrUcs2String *imageName;
    CsrUint8   *imageDescriptor;
    CsrUint16        imageDescriptorLength;
    CsrBool        ok = TRUE;

    switch(theCharacter)
    {
        case '1':
            {
                /*system(CLS);*/
                printf("\n\n Sending JPEG image, resolution 160*120 and size 21308\n");
                CsrStrCpy(&bipcDemoInstanceData->imgName[DIR_LENGTH], "test1.jpg");
                bipcDemoInstanceData->keyboardState             = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag    = FALSE;
                imageDescriptorLength    = CsrBtReturnImgDescriptionLength("JPEG", "160*120", "21308", "", "");
                imageDescriptor          = CsrBtBuildImgDescriptorHeader("JPEG", "160*120", "21308", "", "", imageDescriptorLength);
                imageName                = CsrUtf82Ucs2ByteString("test1.jpg");
                CsrBtBipcPushPutImageHeaderReqSend(imageName, imageDescriptor, imageDescriptorLength);
                break;
            }
        case '2':
            {
                /*system(CLS);*/
                printf("\n\n Sending JPEG image, resolution 320*240 and size 26864 \n");
                CsrStrCpy(&bipcDemoInstanceData->imgName[DIR_LENGTH], "test2.jpg");
                bipcDemoInstanceData->keyboardState                = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag        = FALSE;
                imageDescriptorLength    = CsrBtReturnImgDescriptionLength("JPEG", "320*240", "26864", "", "");
                imageDescriptor          = CsrBtBuildImgDescriptorHeader("JPEG", "320*240", "26864", "", "", imageDescriptorLength);
                imageName                = CsrUtf82Ucs2ByteString("test2.jpg");
                CsrBtBipcPushPutImageHeaderReqSend(imageName, imageDescriptor, imageDescriptorLength);
                break;
            }
        case '3':
            {
                /*system(CLS);*/
                printf("\n\n Sending JPEG image, resolution 640*480 and size 70766\n");
                CsrStrCpy(&bipcDemoInstanceData->imgName[DIR_LENGTH], "test3.jpg");
                bipcDemoInstanceData->keyboardState                = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag        = FALSE;
                imageDescriptorLength    = CsrBtReturnImgDescriptionLength("JPEG", "640*480", "70766", "", "");
                imageDescriptor          = CsrBtBuildImgDescriptorHeader("JPEG", "640*480", "70766", "", "", imageDescriptorLength);
                imageName                = CsrUtf82Ucs2ByteString("test3.jpg");
                CsrBtBipcPushPutImageHeaderReqSend(imageName, imageDescriptor, imageDescriptorLength);
                break;
            }
        case '4':
            {
                /*system(CLS);*/
                printf("\n\n Sending JPEG image, resolution 800*600 and size 122075\n");
                CsrStrCpy(&bipcDemoInstanceData->imgName[DIR_LENGTH], "test4.jpg");
                bipcDemoInstanceData->keyboardState                = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag        = FALSE;
                imageDescriptorLength    = CsrBtReturnImgDescriptionLength("JPEG", "800*600", "122075", "", "");
                imageDescriptor          = CsrBtBuildImgDescriptorHeader("JPEG", "800*600", "122075", "", "", imageDescriptorLength);
                imageName                = CsrUtf82Ucs2ByteString("test4.jpg");
                CsrBtBipcPushPutImageHeaderReqSend(imageName, imageDescriptor, imageDescriptorLength);
                break;
            }
        case '5':
            {
                /*system(CLS);*/
                printf("\n\n Sending JPEG image, resolution 1024*768 and size 197379\n");
                CsrStrCpy(&bipcDemoInstanceData->imgName[DIR_LENGTH], "test5.jpg");
                bipcDemoInstanceData->keyboardState                = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag        = FALSE;
                imageDescriptorLength    = CsrBtReturnImgDescriptionLength("JPEG", "1024*768", "197379", "", "");
                imageDescriptor          = CsrBtBuildImgDescriptorHeader("JPEG", "1024*768", "197379", "", "", imageDescriptorLength);
                imageName                = CsrUtf82Ucs2ByteString("test5.jpg");
                CsrBtBipcPushPutImageHeaderReqSend(imageName, imageDescriptor, imageDescriptorLength);
                break;
            }
        case '6':
            {
                /*system(CLS);*/
                printf("\n\n Sending JPEG image, resolution 1280*960 and size 314390\n");
                CsrStrCpy(&bipcDemoInstanceData->imgName[DIR_LENGTH], "test6.jpg");
                bipcDemoInstanceData->keyboardState                = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag        = FALSE;
                imageDescriptorLength    = CsrBtReturnImgDescriptionLength("JPEG", "1280*960", "314390", "", "");
                imageDescriptor          = CsrBtBuildImgDescriptorHeader("JPEG", "1280*960", "314390", "", "", imageDescriptorLength);
                imageName                = CsrUtf82Ucs2ByteString("test6.jpg");
                CsrBtBipcPushPutImageHeaderReqSend(imageName, imageDescriptor, imageDescriptorLength);
                break;
            }
        default :
            {
                ok = FALSE;
                printf("####### default in keybPrim handler %x,\n", theCharacter);
                break;
            }
    }
    if(ok)
    {
        bipcDemoInstanceData->menuState =  MENU_PUSH_BUSY_STATE;
        showMenu(bipcDemoInstanceData);
    }
}
#if 0
static void keyboardBondingStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {
        if (bipcDemoInstanceData->passkeyLength == 0)
        {
            CsrBtScPasskeyResSend(FALSE, (bipcDemoInstanceData->bondingAddr),
                             bipcDemoInstanceData->passkeyLength,
                             bipcDemoInstanceData->passkey,
                             TRUE,
                             TRUE);
        }
        else
        {
            CsrBtScPasskeyResSend(TRUE,
                             (bipcDemoInstanceData->bondingAddr),
                             bipcDemoInstanceData->passkeyLength,
                             bipcDemoInstanceData->passkey,
                             TRUE,
                             TRUE);
        }
        bipcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
        printf("\n");
    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (bipcDemoInstanceData->passkeyLength > 0)
        {
            bipcDemoInstanceData->passkeyLength--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (bipcDemoInstanceData->passkeyLength < CSR_BT_PASSKEY_MAX_LEN)
        {
            bipcDemoInstanceData->passkey[bipcDemoInstanceData->passkeyLength++] = theCharacter;
            printf("%c", theCharacter);
        }
    }
}
#endif

static void keyboardAuthenticateStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {
        if (bipcDemoInstanceData->chalBack)
        {
            if (!bipcDemoInstanceData->authenticateLength)
            {
                CsrBtBipcAuthenticateWithChalResSend(NULL, 0, (CsrCharString *) CsrStrDup("CSR-BIPS"), 0, NULL, 0, NULL, (CsrCharString *) CsrStrDup("CSR-BIPC"));
            }
            else
            {
                /* Using the same password for both directions */
                CsrBtBipcAuthenticateWithChalResSend((CsrUint8*) CsrStrDup(bipcDemoInstanceData->authenticatePassword),
                                                     bipcDemoInstanceData->authenticateLength,
                                                     (CsrCharString *) CsrStrDup("CSR-BIPS"),
                                                     0,
                                                     NULL,
                                                     bipcDemoInstanceData->authenticateLength,
                                                     (CsrUint8*) CsrStrDup(bipcDemoInstanceData->authenticatePassword),
                                                     (CsrCharString *) CsrStrDup("CSR-BIPC"));
                /*system(CLS);*/
                showMenu(bipcDemoInstanceData);
                bipcDemoInstanceData->authenticatePassword[bipcDemoInstanceData->authenticateLength] = '\0';
                printf("\n\n--- Authentication --- Password is: %s\n", CsrStrDup(bipcDemoInstanceData->authenticatePassword));
            }
        }
        else
        {
            if (!bipcDemoInstanceData->authenticateLength)
            {
                CsrBtBipcAuthenticateResSend(NULL, 0, (CsrCharString *) CsrStrDup("CSR-BIPS"));
            }
            else
            {
                CsrBtBipcAuthenticateResSend((CsrUint8*) CsrStrDup(bipcDemoInstanceData->authenticatePassword),
                                             bipcDemoInstanceData->authenticateLength,
                                             (CsrCharString *) CsrStrDup("CSR-BIPS"));
            }
            /*system(CLS);*/
            showMenu(bipcDemoInstanceData);
        }

        bipcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (bipcDemoInstanceData->authenticateLength > 0)
        {
            bipcDemoInstanceData->authenticateLength--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (bipcDemoInstanceData->authenticateLength < 16)
        {
            bipcDemoInstanceData->authenticatePassword[bipcDemoInstanceData->authenticateLength++] = theCharacter;
            printf("%c", theCharacter);
        }
        else
        {
            printf("The maximum number of character is reach. Please press the BACKSPACE_KEY or the RETURN KEY\n");
        }
    }
}

static void keyboardSelectAttachFileStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    CsrUint8      *attachmentDescriptor;
    CsrUint16         attachmentDescriptorLength;
    CsrBool           ok = TRUE;
    switch(theCharacter)
    {
        case '1':
            {
                /*system(CLS);*/
                CsrStrCpy(&bipcDemoInstanceData->imgAttachName[DIR_LENGTH], "attach.wav");
                bipcDemoInstanceData->keyboardState        = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag = FALSE;
                attachmentDescriptorLength  = CsrBtReturnImgAttachDescriptionLength("audio",
                                                                               "",
                                                                               "sound.wav",
                                                                               "39700",
                                                                               "");
                attachmentDescriptor = CsrBtBuildImgAttachmentDescriptorHeader("audio",
                                                                          "",
                                                                          "sound.wav",
                                                                          "39700",
                                                                          "",
                                                                          attachmentDescriptorLength);
                CsrBtBipcPushPutLinkedAttachmentHeaderReqSend(
                                                         bipcDemoInstanceData->imageHandle,
                                                         attachmentDescriptor,
                                                         attachmentDescriptorLength);
                break;
            }
        case '2':
            {
                /*system(CLS);*/
                CsrStrCpy(&bipcDemoInstanceData->imgAttachName[DIR_LENGTH], "attach.txt");
                bipcDemoInstanceData->keyboardState                = KEYBOARD_MENU_STATE;
                bipcDemoInstanceData->normalDisconnectFlag        = FALSE;
                attachmentDescriptorLength = CsrBtReturnImgAttachDescriptionLength("text",
                                                                              "",
                                                                              "textFile.txt",
                                                                              "11",
                                                                              "");
                attachmentDescriptor = CsrBtBuildImgAttachmentDescriptorHeader("text",
                                                                          "",
                                                                          "textFile.txt",
                                                                          "11",
                                                                          "",
                                                                          attachmentDescriptorLength);
                CsrBtBipcPushPutLinkedAttachmentHeaderReqSend(
                                                         bipcDemoInstanceData->imageHandle,
                                                         attachmentDescriptor,
                                                         attachmentDescriptorLength);
                break;
            }
        case 'a':
            {
                char number[3];

                number[0] = bipcDemoInstanceData->imageHandle[5];
                number[1] = bipcDemoInstanceData->imageHandle[6];
                number[2] = '\0';


                sscanf(number,"%d",&bipcDemoInstanceData->imageHandleValue);

                if(bipcDemoInstanceData->imageHandleValue > 1)
                {
                    bipcDemoInstanceData->imageHandleValue--;
                    sprintf((char *) &bipcDemoInstanceData->imageHandle[0],"%07d", bipcDemoInstanceData->imageHandleValue);
                }
                printf("\n Image Handle changed to %s\n", bipcDemoInstanceData->imageHandle);
                ok = FALSE;
                break;
            }
        default :
            {
                ok = FALSE;
                printf("####### default in keybPrim handler %x,\n", theCharacter);
                break;
            }
    }
    if (ok)
    {
        bipcDemoInstanceData->menuState =  MENU_PUSH_BUSY_STATE;
        showMenu(bipcDemoInstanceData);
    }
}
static void keyboardSelectImageCapabilitiesStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    printf("%c",theCharacter);
    switch(theCharacter)
    {
        case RETURN_KEY:
            bipcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
            if(bipcDemoInstanceData->imgCapabilitiesStr == NULL)
            {
                bipcDemoInstanceData->imgCapabilitiesStrSize = sizeof(capabilitiesXmlString);
                bipcDemoInstanceData->imgCapabilitiesStr = capabilitiesXmlString;
            }
            CsrBtBipcAaGetCapabilitiesHeaderResSend( CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
            printf("\nSending: \n%s\n",bipcDemoInstanceData->imgCapabilitiesStr);
            printf("\n");
            bipcDemoInstanceData->menuState =  MENU_RC_BUSY_STATE;
            showMenu(bipcDemoInstanceData);
            break;

        case BACKSPACE_KEY:
            printf("\b \b");
            break;

        case 'a':
            bipcDemoInstanceData->imgCapabilitiesStrSize = sizeof(capabilitiesXmlString);
            bipcDemoInstanceData->imgCapabilitiesStr = capabilitiesXmlString;
            break;

        case 'b':
            bipcDemoInstanceData->imgCapabilitiesStrSize = sizeof(capabilitiesCreationXmlString);
            bipcDemoInstanceData->imgCapabilitiesStr = capabilitiesCreationXmlString;
            break;

        case 'c':
            bipcDemoInstanceData->imgCapabilitiesStrSize = sizeof(capabilitiesModificationXmlString);
            bipcDemoInstanceData->imgCapabilitiesStr = capabilitiesModificationXmlString;
            break;

        case 'd':
            bipcDemoInstanceData->imgCapabilitiesStrSize = sizeof(capabilitiesPixelXmlString);
            bipcDemoInstanceData->imgCapabilitiesStr = capabilitiesPixelXmlString;
            break;

        case 'e':
            bipcDemoInstanceData->imgCapabilitiesStrSize = sizeof(capabilitiesEncodingXmlString);
            bipcDemoInstanceData->imgCapabilitiesStr = capabilitiesEncodingXmlString;
            break;

        default:
            break;
    }
}
static void keyboardSelectImageDescriptorStateHandler(BipcDemoInstanceDataType * bipcDemoInstanceData, CsrUint8 theCharacter)
{
    printf("%c",theCharacter);
    switch(theCharacter)
    {
        case RETURN_KEY:
            {
                CsrUint8 *imgDescriptor = NULL;
                bipcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                if(bipcDemoInstanceData->imgDescriptorStr == NULL)
                {
                    bipcDemoInstanceData->imgDescriptorStrSize = 0;
                }
                else
                {
                    imgDescriptor = CsrPmemAlloc(bipcDemoInstanceData->imgDescriptorStrSize);
                    CsrMemCpy(imgDescriptor, bipcDemoInstanceData->imgDescriptorStr,
                           bipcDemoInstanceData->imgDescriptorStrSize);
                }
                CsrBtBipcRcGetImageReqSend(
                                      bipcDemoInstanceData->imageHandle,
                                      bipcDemoInstanceData->imgDescriptorStrSize,
                                      imgDescriptor,
                                      FALSE);


                printf("\n\n   Retrieving image ");
                /* Open file for writing */
                if((bipcDemoInstanceData->currentFileHandle = CsrBtFopen("rc_image.jpg", "wb")) == NULL)
                {
                    printf("Fail. BIP DEMO APP could not open the file for writing\n");
                }
                else
                {
                    CsrUint16 i;
                    printf("\nSending: \n");
                    for( i = 0; i <  bipcDemoInstanceData->imgDescriptorStrSize; i++)
                    {
                        printf("%c",bipcDemoInstanceData->imgDescriptorStr[i] );
                    }
                }
                printf("\n");

                bipcDemoInstanceData->menuState =  MENU_RC_BUSY_STATE;
                showMenu(bipcDemoInstanceData);

                bipcDemoInstanceData->imgDescriptorStrSize = 0;
                CsrPmemFree(bipcDemoInstanceData->imgDescriptorStr);
                bipcDemoInstanceData->imgDescriptorStr = NULL;
            }
            break;
        case BACKSPACE_KEY:
            printf("\b \b");
            break;
        case 't':
            bipcDemoInstanceData->imgDescriptorStrSize =
                CsrBtReturnImgDescriptionLength("JPEG", "160*120", "", "", "");
            bipcDemoInstanceData->imgDescriptorStr =
                CsrBtBuildImgDescriptorHeader("JPEG", "160*120", "", "", "",
                                         bipcDemoInstanceData->imgDescriptorStrSize);
            break;
        case '0':
            {
                CsrUint8 *str;
                printf("\n");
                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(0, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '1':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(1, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '2':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(2, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '3':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(3, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '4':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(4, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '5':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(5, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '6':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(6, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '7':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(7, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case '8':
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(8, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        case 9:
            {
                CsrUint8 *str;
                printf("\n");

                str = CsrPmemAlloc(bipcDemoInstanceData->imagePropertiesLength);
                CsrMemCpy(str, bipcDemoInstanceData->imageProperties,
                       bipcDemoInstanceData->imagePropertiesLength);

                bipcDemoInstanceData->imgDescriptorStr =
                    extractImageDescriptor(9, str,
                                           bipcDemoInstanceData->imagePropertiesLength,
                                           &bipcDemoInstanceData->imgDescriptorStrSize);
                CsrPmemFree(str);
            }
            break;
        default:
            break;
    }

}
static void KeyMessageService(CsrUint16 m, void *data)
{
    if (_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, m, data);
}


#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16    msg_type=0;
    void    * msg_data=NULL;
    BipcDemoInstanceDataType * instanceData;

    instanceData = (BipcDemoInstanceDataType *) *gash;

    scAppDeinitInstanceData(&instanceData->scInstData);

    while (CsrSchedMessageGet(&msg_type, &msg_data))
    {
        switch (msg_type)
        {
            case CSR_BT_BIPC_PRIM:
            {
                CsrBtBipcFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            case CSR_BT_SD_PRIM:
            {
                CsrBtSdFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
        }
    }
    if (instanceData->currentFileHandle)
    {
        CsrBtFclose(instanceData->currentFileHandle);
        instanceData->currentFilePosition = 0;
        instanceData->currentFileHandle = NULL;
    }
    CsrPmemFree(instanceData);
}

#endif

void applicationUsage(char *col, char *des, char *opt)
{
}

char* applicationCmdLineParse(int ch, char *optionArgument)
{
    return NULL;
}

void applicationSetup(void)
{
}

char* applicationExtraOptions(void)
{
    return "";
}
