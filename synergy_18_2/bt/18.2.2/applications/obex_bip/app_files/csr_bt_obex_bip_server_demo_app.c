/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>

#include "csr_sched.h"
#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_lib.h"
#endif /* CSR_AMP_ENABLE */
#include "csr_bt_util.h"
#include "csr_bt_obex.h"
#include "csr_bt_xml_lib.h"
#include "csr_bt_bips_prim.h"
#include "csr_bt_bips_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"
#include "csr_unicode.h"
#include "csr_bt_demoapp.h"

#define MON_IMG_1 ("bip_files"PATH_SEP"mon_img_1.jpg")
#define MON_IMG_2 ("bip_files"PATH_SEP"mon_img_2.jpg")
#define MON_IMG_3 ("bip_files"PATH_SEP"mon_img_3.jpg")
#define IMG_1 ("bip_files"PATH_SEP"img_1.jpg")
#define IMG_2 ("bip_files"PATH_SEP"img_2.jpg")
#define IMG_3 ("bip_files"PATH_SEP"img_3.jpg")

#define KEYB_CHECK_TIMEOUT                          (100000)
#define DEMO_NUMBER_OF_STATES                       (1)
#define MAX_NUMBER_OF_BIPS_INSTANCES                (2)

#define KEYBOARD_MENU_STATE                         (0)
#define KEYBOARD_BONDING_STATE                      (1)
#define KEYBOARD_AUTHENTICATE_STATE                 (2)
#define KEYBOARD_IMAGE_LIST_NB_RETURNED_HANDLES     (3)
#define KEYBOARD_IMAGE_LIST_START_OFFSET            (4)
#define KEYBOARD_IMAGE_LIST_LATEST_CAPTURED_IMAGES  (5)
#define KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CODING     (6)
#define KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CREATION   (7)
#define KEYBOARD_IMAGE_DESCRIPTOR_FILTER_PIXEL_SIZE (8)
#define KEYBOARD_IMAGE_DESCRIPTOR_FILTER_MODIFIED   (9)
#define KEYBOARD_IMAGE_HANDLE                       (10)
#define KEYBOARD_ATTACHMENT_TYPE                    (11)
#define KEYBOARD_GET_IMAGE_DESCRIPTOR               (12)
#define KEYBOARD_GET_IMAGE_CAPABILITIES             (13)
#define KEYBOARD_CHALLENGE_STATE                    (14)

#define MENU_IDLE_STATE                (0)
#define MENU_AA_STATE                  (1)
#define MENU_AA_BUSY_STATE             (2)


#define MAX_PEER_PACKET_SIZE           (0x4000)
#define MAX_LOCAL_OBEX_SIZE            (0x4000)

/* Define the maximum memory available for image storage.
 * Max value is 0xFFFFFFFFFFFFFFFF */
#define TOTAL_IMAGING_UPPER_DATA_CAPACITY     (0x00000000)
#define TOTAL_IMAGING_LOWER_DATA_CAPACITY     (0x008CA000)

/* Element name used in the attachment descriptor and the
 * image descriptor */
static char ImageDescriptor[]       = "image-descriptor";
static char AttachmentDescriptor[]  = "attachment-descriptor";
static char ImageProperties[]       = "image-properties";
static char Native[]                = "native";
static char Variant[]               = "variant";
static char Image[]                 = "image";
static char Attachment[]            = "attachment";
static char ImageListing[]          = "image-listing";

#define FIRST_INST_ACTIVATED                  (0x01)
#define SECOND_INST_ACTIVATED                 (0x02)

/* Attribute name used in the attachment descriptor and the image descriptor    */
static char Version[]               = "version";
static char Encoding[]              = "encoding";
static char Pixel[]                 = "pixel";
static char Size[]                  = "size";
static char MaxSize[]               = "maxsize";
static char Transformation[]        = "transformation";
static char ContentType[]           = "content-type";
static char Charset[]               = "charset";
static char Name[]                  = "name";
static char Created[]               = "created";
static char Handle[]                = "handle";


/*  Automatic archive */
#define DEFAULT_IMAGE_LIST_FILTER "< image-handles-descriptor version=\"1.0\" > <filtering-parameters encoding=\"JPEG\" /> < /image-handles-descriptor >"



typedef enum
{
    waitingBeforeGettingInstances,
    idle,
    connected,
    pairing,
    disconnecting,
    inquiry,
    selecting,
    numOfClients,
    entryNum,
    selectBipsInstance,
    connecting,
    activated,
    deactivate,
    busy,
    sdcSearch,
} BipsDemoStates_t;

typedef struct
{
    CsrBool     imageListExist;
    CsrUint16   nofReturnedHandles;
    CsrUint8    *usedFilter;
    char        *imageListObject;
    CsrUint16   imageListObjectLength;
    CsrBool     firstPart;
    CsrUint16   imagePropertiesLength;
    CsrUint8    *imageProperties;
    char        *imgDescriptorStr;
    CsrUint16   imgDescriptorStrLength;
    CsrUint16   maxListSize;
    CsrUint16   listOffset;
    CsrBool     lastImage;
    CsrUint16   imageHandleIndex;
    CsrUint16   imageHandleIndexMax;
    char        imageHandles[50][8];
    CsrBool     attachTypeWav;
    CsrUint8    *imageDescriptorCreated;
    CsrUint8    *imageDescriptorModified;
    CsrUint8    *imageDescriptorPixel;
    CsrUint8    *imageDescriptorEncoding;
}AaInstanceDataType;

typedef struct
{
    CsrSchedQid          CsrSchedQid;                    /* Profile queue */
    BipsDemoStates_t     state;                  /* Instance state */
    CsrSchedQid          pHandleInst;            /* BIPS instance CsrSchedQid */
    CsrFileHandle        *writingFileHandle;
    CsrUint32            connectionId;
    CsrUint16            sendObjectLength;
    CsrUint16            sendIndex;
    CsrUint16            maxPeerObexPacketSize;
    CsrUint8             imageHandleCount;
    CsrUint8             authenticateLength;
    CsrUint8             *imageFileName;
    CsrUint8             *thumbnailFileName;
    CsrUint8             *attachmentFileName;
    char                 authenticatePassword[20];
    CsrUint32            currentSize;
    CsrUint8             currentMonImg;
    CsrFileHandle        *currentFileHandle;
    CsrUint32            currentFilePosition;
    CsrUint32            currentFileSize;
    char                 imgName[24];
    CsrUint8             reqHandle;
    CsrUint8             menuState;
    CsrBtDeviceAddr       bondingAddr;
    CsrUint8             inputStr[200];
    CsrUint8             inputStrIndex;
    char                 *capabilitiesString;
    CsrUint16            capabilitiesStringSize;
    AaInstanceDataType   *aa;
} BipsDemoInstanceDataType;

typedef struct
{
    void                        *recvMsgP;
    CsrUint8                     keyboardState;
    CsrUint8                     prevKeyboardState;
    CsrUint32                    upperDataCapacity;
    CsrUint32                    lowerDataCapacity;
    CsrUint8                     passkeyLength;
    CsrSchedQid                   phandle;                                        /* Demo app CsrSchedQid */
    CsrUint8                     numberOfActiveInst;                             /* Current number of the active instance */
    BipsDemoInstanceDataType    *instData;                                      /* Current active Instance */
    BipsDemoInstanceDataType    bipsInstances[MAX_NUMBER_OF_BIPS_INSTANCES];    /* Instance data */
    CsrUint8                     numberOfBipsInstances;                          /* Total number of active instances */
    CsrUint8                     passkey[CSR_BT_PASSKEY_MAX_LEN];
    CsrUint8                     globalState;
    ScAppInstData_t             scInstData;
} BipsGlobalInstanceDataType;

typedef void (* BipsDemoStateHandlerType)(BipsGlobalInstanceDataType * bipsDemoInstanceData);

static char capabilitiesXmlString[] =   "<imaging-capabilities version=\"1.0\"><preferred-format encoding=\"JPEG\" pixel=\"640*480\" /><image-formats encoding=\"JPEG\" pixel=\"0*0-65535*65535\" maxsize=\"2758320\"/><attachment-formats content-type=\"audio/32kadpcm\" /><attachment-formats content-type=\"audio/basic\" /><attachment-formats content-type=\"audio/basic\" /><attachment-formats content-type=\"text/plain\" /></imaging-capabilities>";
static char capabilitiesXmlString1[] =   "<imaging-capabilities version=\"1.0\"><preferred-format encoding=\"JPEG\" pixel=\"160*120\" /><image-formats encoding=\"JPEG\" pixel=\"160*120\"/></imaging-capabilities>";
static char capabilitiesXmlString2[] =   "<imaging-capabilities version=\"1.0\"><preferred-format encoding=\"JPEG\" pixel=\"160*120\" /><image-formats encoding=\"JPEG\" pixel=\"0*0-65535*65535\"/></imaging-capabilities>";
static char capabilitiesXmlString3[] =   "<imaging-capabilities version=\"1.0\"><preferred-format encoding=\"JPEG\" pixel=\"640*480\" /><image-formats encoding=\"JPEG\" pixel=\"640*480\" /></imaging-capabilities>";

static char propertiesXmlString1[]  = "<image-properties version=\"1.0\" handle=\"0000001\" friendly-name=\"img_01.jpg\"><native encoding=\"JPEG\" pixel=\"1136*852\" size=\"78452\"/><variant encoding=\"JPEG\" pixel=\"640*480\"/><attachment content-type=\"audio/32kadpcm\" name=\"attach.wav\" size=\"39700\"/></image-properties>";
static char propertiesXmlString2[]  = "<image-properties version=\"1.0\" handle=\"0000002\" friendly-name=\"img_02.jpg\"><native encoding=\"JPEG\" pixel=\"1136*852\" size=\"74355\"/><variant encoding=\"JPEG\" pixel=\"640*480\"/><attachment content-type=\"audio/32kadpcm\" name=\"attach.wav\" size=\"39700\"/></image-properties>";
static char propertiesXmlString3[]  = "<image-properties version=\"1.0\" handle=\"0000003\" friendly-name=\"img_03.jpg\"><native encoding=\"JPEG\" pixel=\"1136*852\" size=\"76081\"/><variant encoding=\"JPEG\" pixel=\"640*480\"/><attachment content-type=\"audio/32kadpcm\" name=\"attach.wav\" size=\"39700\"/></image-properties>";

static void bipsGetInstQidCfmHandler(BipsGlobalInstanceDataType *instData);
static void bipsDeactivateIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsConnectIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsAuthenticateIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetCapabilitiesHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetCapabilitiesObjectIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsPutImageHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsPutImageFileIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsPutLinkedThumbnailHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsPutLinkedThumbnailFileIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsPutLinkedAttachmentHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsPutLinkedAttachmentFileIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetMonitoringImageHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetMonitoringImageObjectIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImagePropertiesHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImagePropertiesObjectIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImageHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImageObjectIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetLinkedThumbnailHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetLinkedThumbnailObjectIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsAbortIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsDisconnectIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void updateTotalImagingDataCapacity( BipsGlobalInstanceDataType * bipsGlobalInstanceData);

static void  bipsGetImageListIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImageListHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImageListCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetCapabilitiesIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetCapabilitiesCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImagePropertiesIndhandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImagePropertiesCfmhandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImageIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetImageCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetLinkedAttachmentIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetLinkedAttachmentCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetLinkedThumbnailIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsGetLinkedThumbnailCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsDeleteImageCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void bipsAbortCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData);
static void printImageList(AaInstanceDataType *pInst);

static char * interpretImageDescriptor(CsrUint8 *imageDescriptor, CsrUint16 imageDescriptorLength);
static void interpretAttachmentDescriptor( BipsGlobalInstanceDataType * bipsGlobalInstanceData, CsrUint8 *attachmentDescriptor, CsrUint16 attachmentDescriptorLength);
static void interpretImageHandlesDescriptor(BipsGlobalInstanceDataType * bipsGlobalInstanceData,
                                            CsrUint8 *imageHandlesDescriptor,
                                            CsrUint16 imageHandlesDescriptorLength);
static void extractImageProperties(CsrUint8 *imageProperties, CsrUint16 imagePropertiesLength);
static CsrUint8 * extractImageDescriptor(CsrUint8 index, CsrUint8 *imageProperties,
                                        CsrUint16 imagePropertiesLength,
                                        CsrUint16 *imgDescLength);

static char *copyString(char *s);
static void setActiveInstance( BipsGlobalInstanceDataType *instData, CsrSchedQid CsrSchedQid);
static void bipsAaHandleKeyPress(BipsGlobalInstanceDataType *instData, CsrSchedQid CsrSchedQid, char key);
static void bipsAaHandleAbortKeyPress(BipsGlobalInstanceDataType *instData, CsrSchedQid CsrSchedQid, char key);
static void KeyMessageService(CsrUint16 m ,void *data)
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

void showMenu(BipsGlobalInstanceDataType * instData)
{
    CsrUint8 i = 0;
    char c;

    printf("\nOBEX BASIC IMAGE PROFILE SERVER:\n\n");

    printf("\nInstance status:\n");
    for( i=0; i<MAX_NUMBER_OF_BIPS_INSTANCES; i++ )
    {
        c = (instData->numberOfActiveInst == i) ? '*' : ' ';  /* Indicate the active instance in the demo scope */

        if( instData->bipsInstances[i].state == idle )
        {
            printf("    (%c) BIP server instance %i is currently idle\n", c, i+1);
        }
        else if( instData->bipsInstances[i].state == activated )
        {
            printf("    (%c) BIP server instance %i is currently active\n", c, i+1);
        }
        else if( instData->bipsInstances[i].state == deactivate )
        {
            printf("    (%c) BIP server instance %i is currently deactivate\n", c, i+1);
        }
    }

    printf("\nProgram options:\n\n");

    if( (instData->instData->state == idle) | (instData->instData->state == deactivate) )
    {
        printf("    1) Activate current BIP Server instance\n");
        printf("    4) Activate current BIP Server instance with possibility to challenge\n");
    }
    else if( instData->instData->state == activated )
    {
        printf("    1) Deactivate current BIP Server instance\n");
    }
    printf("    2) Change active instance in demo scope\n");
    printf("    3) Security Controller Configuration\n");

    if(instData->instData->menuState == MENU_AA_STATE)
    {
        printf("\n\n    a) Get imaging capabilities\n");
        printf("    b) Get image list\n");
        if((instData->instData->aa->imageListExist)&&
           (instData->instData->aa->imageListObjectLength > 0))
        {
            printf("    c) Get image properties \n");
            printf("    d) Get image \n");
            printf("    e) Get linked thumbnail \n");
            printf("    f) Get linked attachment \n");
            printf("    g) Delete image \n");
            printf("\n");

            printf("    h) Choose a different image handle (currently: %s )\n",
                   instData->instData->aa->imageHandles[instData->instData->aa->imageHandleIndex]);
        }

        printf("\n\n    8) Disconnect Obex Automatic Archive \n");

        printImageList(instData->instData->aa);
    }
    else if (instData->instData->menuState == MENU_AA_BUSY_STATE)
    {
        printf("\n\n    x) Abort current process\n");
    }
    printf("\n\n");
}



/* ---------- BIPS jump tables ---------- */
static const BipsDemoStateHandlerType bipsDemoCommonStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPS_PRIM_UPSTREAM_COUNT] =
{
    {
        bipsDeactivateIndHandler,                   /* CSR_BT_BIPS_DEACTIVATE_IND */
        bipsConnectIndHandler,                      /* CSR_BT_BIPS_CONNECT_IND */
        bipsAuthenticateIndHandler,                 /* CSR_BT_BIPS_AUTHENTICATE_IND */
        bipsAbortIndHandler,                        /* CSR_BT_BIPS_ABORT_IND */
        bipsDisconnectIndHandler,                   /* CSR_BT_BIPS_DISCONNECT_IND */
        bipsGetInstQidCfmHandler,                   /* CSR_BT_BIPS_GET_INSTANCES_QID_CFM */
        NULL                                        /* CSR_BT_BIPS_SECURITY_IN_CFM */
    }
};

static const BipsDemoStateHandlerType bipsDemoPushStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPS_PUSH_PRIM_UPSTREAM_COUNT] =
{
    {
        bipsGetCapabilitiesHeaderIndHandler,        /* CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND */
        bipsGetCapabilitiesObjectIndHandler,        /* CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND */
        bipsPutImageHeaderIndHandler,               /* CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */
        bipsPutImageFileIndHandler,                 /* CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */
        bipsPutLinkedThumbnailHeaderIndHandler,     /* CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */
        bipsPutLinkedThumbnailFileIndHandler,       /* CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */
        bipsPutLinkedAttachmentHeaderIndHandler,    /* CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */
        bipsPutLinkedAttachmentFileIndHandler       /* CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */
    }
};

static const BipsDemoStateHandlerType bipsDemoRcStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPS_RC_PRIM_UPSTREAM_COUNT] =
{
    {
        bipsGetMonitoringImageHeaderIndHandler,     /* BIPS_RC_MONITORING_IMAGE_HEADER_IND */
        bipsGetMonitoringImageObjectIndHandler,     /* BIPS_RC_MONITORING_IMAGE_OBJECT_IND */
        bipsGetImagePropertiesHeaderIndHandler,     /* CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */
        bipsGetImagePropertiesObjectIndHandler,     /* CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND */
        bipsGetImageHeaderIndHandler,               /* CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */
        bipsGetImageObjectIndHandler,               /* CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND */
        bipsGetLinkedThumbnailHeaderIndHandler,     /* CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */
        bipsGetLinkedThumbnailObjectIndHandler      /* CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND */
    }
};

static const BipsDemoStateHandlerType bipsDemoAaStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BIPS_AA_PRIM_UPSTREAM_COUNT] =
{
    {
        bipsGetImageListIndHandler,                 /* CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */
        bipsGetImageListHeaderIndHandler,           /* CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */
        bipsGetImageListCfmHandler,                 /* CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */
        bipsGetCapabilitiesIndHandler,              /* CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */
        bipsGetCapabilitiesCfmHandler,              /* CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */
        bipsGetImagePropertiesIndhandler,           /* CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */
        bipsGetImagePropertiesCfmhandler,           /* CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */
        bipsGetImageIndHandler,                     /* CSR_BT_BIPS_AA_GET_IMAGE_IND */
        bipsGetImageCfmHandler,                     /* CSR_BT_BIPS_AA_GET_IMAGE_CFM */
        bipsGetLinkedAttachmentIndHandler,          /* CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */
        bipsGetLinkedAttachmentCfmHandler,          /* CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */
        bipsGetLinkedThumbnailIndHandler,           /* CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */
        bipsGetLinkedThumbnailCfmHandler,           /* CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */
        bipsDeleteImageCfmHandler,                  /* CSR_BT_BIPS_AA_DELETE_IMAGE_CFM */
        bipsAbortCfmHandler                          /* CSR_BT_BIPS_AA_ABORT_CFM */
    }
};

static void bipScAppSetFnFini(void *pFiniData)
{
    BipsGlobalInstanceDataType *instData = (BipsGlobalInstanceDataType *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState);
    system_cls();
    showMenu(instData);
}

static void bipScAppSetFnInit(void *pInitData)
{
    BipsGlobalInstanceDataType *instData = (BipsGlobalInstanceDataType *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState, KEYBOARD_BONDING_STATE);
}

static void* bipScAppSetFnGetMessage(void *pRecvMsgPData)
{
    BipsGlobalInstanceDataType *instData = (BipsGlobalInstanceDataType *) pRecvMsgPData;
    void *result                         = instData->recvMsgP;
    instData->recvMsgP                   = NULL;
    return result;
}

/*****************************************************************************
  CsrBtAppInit: Allocate and initialise Application instance data space
******************************************************************************/
void initInstanceData(BipsGlobalInstanceDataType *bipsGlobalInstanceData)
{
    CsrUint8 i;

    bipsGlobalInstanceData->recvMsgP                = NULL;
    bipsGlobalInstanceData->keyboardState           = KEYBOARD_MENU_STATE;
    bipsGlobalInstanceData->upperDataCapacity       = TOTAL_IMAGING_UPPER_DATA_CAPACITY;
    bipsGlobalInstanceData->lowerDataCapacity       = TOTAL_IMAGING_LOWER_DATA_CAPACITY;
    bipsGlobalInstanceData->instData                = &bipsGlobalInstanceData->bipsInstances[0];  /* Frist instance */
    bipsGlobalInstanceData->globalState             = 0x00;
    bipsGlobalInstanceData->numberOfBipsInstances   = 0;
    bipsGlobalInstanceData->numberOfActiveInst      = 0;
    bipsGlobalInstanceData->instData->inputStr[0]   = '\0';
    bipsGlobalInstanceData->instData->inputStrIndex = 0;
    for(i=0;i<MAX_NUMBER_OF_BIPS_INSTANCES;i++)
    {
        bipsGlobalInstanceData->bipsInstances[i].maxPeerObexPacketSize = 0;
        bipsGlobalInstanceData->bipsInstances[i].writingFileHandle     = NULL;
        bipsGlobalInstanceData->bipsInstances[i].imageFileName         = NULL;
        bipsGlobalInstanceData->bipsInstances[i].thumbnailFileName     = NULL;
        bipsGlobalInstanceData->bipsInstances[i].attachmentFileName    = NULL;
        bipsGlobalInstanceData->bipsInstances[i].sendIndex             = 0;
        bipsGlobalInstanceData->bipsInstances[i].sendObjectLength      = 0;
        bipsGlobalInstanceData->bipsInstances[i].imageHandleCount      = 0;
        bipsGlobalInstanceData->bipsInstances[i].authenticateLength    = 0;
        bipsGlobalInstanceData->bipsInstances[i].currentMonImg         = 0;
        bipsGlobalInstanceData->bipsInstances[i].currentFileHandle     = NULL;
        bipsGlobalInstanceData->bipsInstances[i].currentFilePosition   = 0;
        bipsGlobalInstanceData->bipsInstances[i].currentFileSize       = 0;
        bipsGlobalInstanceData->bipsInstances[i].reqHandle             = 0;
        bipsGlobalInstanceData->bipsInstances[i].CsrSchedQid                   = 0xFFFF;
        bipsGlobalInstanceData->bipsInstances[i].currentSize           = 0;
        bipsGlobalInstanceData->bipsInstances[i].connectionId          = 0;
        bipsGlobalInstanceData->bipsInstances[i].state                 = idle;
        bipsGlobalInstanceData->bipsInstances[i].menuState             = MENU_IDLE_STATE;

        bipsGlobalInstanceData->bipsInstances[i].aa                    = CsrPmemAlloc(sizeof(AaInstanceDataType));
        bipsGlobalInstanceData->bipsInstances[i].aa->imageListExist    = FALSE;
        bipsGlobalInstanceData->bipsInstances[i].aa->nofReturnedHandles = 0;
        bipsGlobalInstanceData->bipsInstances[i].aa->usedFilter         = NULL;
        bipsGlobalInstanceData->bipsInstances[i].aa->imageListObject    = NULL;
        bipsGlobalInstanceData->bipsInstances[i].aa->imageListObjectLength    = 0;
        bipsGlobalInstanceData->bipsInstances[i].aa->imagePropertiesLength = 0;
        bipsGlobalInstanceData->bipsInstances[i].aa->imageProperties    = NULL;
        bipsGlobalInstanceData->bipsInstances[i].aa->imgDescriptorStrLength = 0;
        bipsGlobalInstanceData->bipsInstances[i].aa->imgDescriptorStr   = NULL;
        bipsGlobalInstanceData->bipsInstances[i].aa->firstPart          = TRUE;
        bipsGlobalInstanceData->bipsInstances[i].aa->attachTypeWav      = TRUE;
        bipsGlobalInstanceData->bipsInstances[i].aa->imageDescriptorCreated        = NULL;
        bipsGlobalInstanceData->bipsInstances[i].aa->imageDescriptorModified       = NULL;
        bipsGlobalInstanceData->bipsInstances[i].aa->imageDescriptorPixel          = NULL;
        bipsGlobalInstanceData->bipsInstances[i].aa->imageDescriptorEncoding       = NULL;
    }

    /* Application handle */
    bipsGlobalInstanceData->phandle = TESTQUEUE;

    /* Security App Configuration */
    CsrBtScActivateReqSend(bipsGlobalInstanceData->phandle);
    scAppInitInstanceData(&bipsGlobalInstanceData->scInstData);

    scAppSetFnFini(&bipsGlobalInstanceData->scInstData, bipScAppSetFnFini, bipsGlobalInstanceData);
    scAppSetFnInit(&bipsGlobalInstanceData->scInstData, bipScAppSetFnInit, bipsGlobalInstanceData);
    scAppSetSecLevelSend(&bipsGlobalInstanceData->scInstData, scAppSendProfileSecurityLevelReq, &bipsGlobalInstanceData->scInstData);

    scAppSetFnGetMessage(&bipsGlobalInstanceData->scInstData, bipScAppSetFnGetMessage, bipsGlobalInstanceData);

    scAppSetSecLevelPrim(&bipsGlobalInstanceData->scInstData,
                         0,     /* Index 0. Single profile */
                         "BIPS", CSR_BT_BIPS_PRIM,
                         CSR_BT_BIPS_IFACEQUEUE,
                         CSR_BT_BIPS_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);
}


/******************************************************************************
 * Init function called by the scheduler upon initialisation.
 * This function is used to boot the demo application.
 *****************************************************************************/
void CsrBtAppInit(void ** gash)
{
    BipsGlobalInstanceDataType * bipsGlobalInstanceData;

    /* allocate and initialise instance data space */
    *gash                  = CsrPmemAlloc(sizeof(BipsGlobalInstanceDataType));
    bipsGlobalInstanceData = (BipsGlobalInstanceDataType *) *gash;
    initInstanceData(bipsGlobalInstanceData);

    printf("Starting up, Please wait....\n");
    CsrBtBipsGetInstancesQidReqSend( bipsGlobalInstanceData->phandle );
}

/******************************************************************************
 * This is the demo application handler function. All primitives sent
 * to the demo application will be received in here. The scheduler
 * ensures that the function is activated when a signal is put on the
 * demo application signal queue.
 * The handler function also handle incoming events form the SC layer and the Keyboard
******************************************************************************/
void CsrBtAppHandler(void ** gash)
{
    CsrUint16    event=0;
    CsrBool stateEventError = TRUE;

    BipsGlobalInstanceDataType * bipsGlobalInstanceData;
    BipsGlobalInstanceDataType * instData;

    bipsGlobalInstanceData = (BipsGlobalInstanceDataType *) *gash;
    instData = bipsGlobalInstanceData;
    CsrSchedMessageGet(&event, &bipsGlobalInstanceData->recvMsgP);


    switch (event)
    {
        case CSR_BT_BIPS_PRIM:
            {
                CsrBtBipsPrim    * type;

                type = (CsrBtBipsPrim *) bipsGlobalInstanceData->recvMsgP;

                if (*type == CSR_BT_BIPS_SECURITY_IN_CFM)
                {
                    handleScSecLevelPrim(&instData->scInstData);
                    stateEventError = FALSE;
                }
                /* Check to see if message is for common jump table */
                else if (((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) >= CSR_BT_BIPS_PRIM_DOWNSTREAM_LOWEST) &&
                    ((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) < (CSR_BT_BIPS_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_PRIM_DOWNSTREAM_COUNT)) &&
                    (bipsDemoCommonStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_PRIM_DOWNSTREAM_LOWEST] != NULL))
                {
                    bipsDemoCommonStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_PRIM_DOWNSTREAM_LOWEST](bipsGlobalInstanceData);
                    stateEventError = FALSE;
                }

                /* Check to see if message is for push jump table */
                else if (((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) >= CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST) &&
                    ((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) < (CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_COUNT)) &&
                    (bipsDemoPushStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST] != NULL))
                {
                    bipsDemoPushStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_PUSH_PRIM_DOWNSTREAM_LOWEST](bipsGlobalInstanceData);
                    stateEventError = FALSE;
                }

                /* Check to see if message is for remote camera jump table */
                else if (((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) >= CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST) &&
                    ((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) < (CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_COUNT)) &&
                    (bipsDemoRcStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST] != NULL))
                {
                    bipsDemoRcStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_RC_PRIM_DOWNSTREAM_LOWEST](bipsGlobalInstanceData);
                    stateEventError = FALSE;
                }

                /* Check to see if message is for auto archive jump table */
                else if (((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) >= CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST) &&
                    ((*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) < (CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST + CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_COUNT)) &&
                    (bipsDemoAaStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST] != NULL))
                {
                    bipsDemoAaStateHandlers[0][(*type - CSR_BT_BIPS_PRIM_UPSTREAM_LOWEST) - CSR_BT_BIPS_AA_PRIM_DOWNSTREAM_LOWEST](bipsGlobalInstanceData);
                    stateEventError = FALSE;
                }
                else if (*type == CSR_BT_BIPS_CHALLENGE_IND)
                {
                    CsrBtBipsChallengeInd * prim = (CsrBtBipsChallengeInd *) bipsGlobalInstanceData->recvMsgP;
                    printf("A peer device (%04X:%02X:%06X) is connecting with%sauthentication challenge\n",
                                                                                    prim->deviceAddr.nap,
                                                                                    prim->deviceAddr.uap,
                                                                                    prim->deviceAddr.lap,
                                                                                    prim->challenged ? " " : "out ");
                    printf("If authentication is wanted, enter the OBEX challenge passkey for ");

                    switch(prim->connectType)
                    {
                        case CSR_BT_BIPS_PUSH_CONNECT:
                        {
                            printf("Image Push ");
                            break;
                        }
                        case CSR_BT_BIPS_RC_CONNECT:
                        {
                            printf("Remote Camera ");
                            break;
                        }
                        case CSR_BT_BIPS_AA_CONNECT:
                        {
                            printf("Automatic Archive ");
                            break;
                        }
                        default:
                        {/* This should not happen */
                            printf("unknown connection ");
                            break;
                        }
                    }

                    printf("If authentication is not wanted, just press ENTER\n");
                    bipsGlobalInstanceData->instData->authenticateLength = 0;
                    bipsGlobalInstanceData->keyboardState                = KEYBOARD_CHALLENGE_STATE;
                    stateEventError = FALSE;
                }

                if (stateEventError == TRUE)
                {
                    printf("A state event error occurred in the BIPS Application layer,\n  it is a CSR_BT_BIPS_PRIM type: %02X\n\n",
                           *type);
                    CsrBtBipsFreeUpstreamMessageContents(event, bipsGlobalInstanceData->recvMsgP);
                }
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&bipsGlobalInstanceData->scInstData);
                break;
            }

        /* Handle AMPM primitives */
#ifdef CSR_AMP_ENABLE
        case CSR_BT_AMPM_PRIM:
            {
                switch(*(CsrBtAmpmPrim*)bipsGlobalInstanceData->recvMsgP)
                {
                    case CSR_BT_AMPM_REGISTER_CFM:
                        {
                            CsrBtAmpmRegisterCfm *prim;
                            prim = (CsrBtAmpmRegisterCfm*)bipsGlobalInstanceData->recvMsgP;
                            printf("AMPM register confirm: btConnId 0x%08x, result 0x%04x, supplier 0x%04x\n",
                                   prim->btConnId, prim->resultCode, prim->resultSupplier);
                        }
                        break;

                    case CSR_BT_AMPM_DEREGISTER_CFM:
                        {
                            CsrBtAmpmDeregisterCfm *prim;
                            prim = (CsrBtAmpmDeregisterCfm*)bipsGlobalInstanceData->recvMsgP;
                            printf("AMPM unregister confirm: btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                                   prim->btConnId, prim->resultSupplier, prim->resultCode);
                        }
                        break;

                    case CSR_BT_AMPM_CONTROLLER_IND:
                    case CSR_BT_AMPM_CONTROLLER_CFM:
                        {
                            CsrBtAmpmControllerInd *prim;
                            CsrUint8 i;

                            prim = (CsrBtAmpmControllerInd*)bipsGlobalInstanceData->recvMsgP;
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
                                CsrBtAmpmMoveReqSend(bipsGlobalInstanceData->instData->connectionId,
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
                            prim = (CsrBtAmpmMoveInd*)bipsGlobalInstanceData->recvMsgP;
                            printf("AMP move channel indication, btConnId 0x%08x, localId 0x%04x - accepting...\n",
                                   prim->btConnId, prim->localId);
                            CsrBtAmpmMoveResSend(prim->btConnId, TRUE);
                        }
                        break;


                    case CSR_BT_AMPM_MOVE_CMP_IND:
                        {
                            CsrBtAmpmMoveCmpInd *prim;
                            prim = (CsrBtAmpmMoveCmpInd*)bipsGlobalInstanceData->recvMsgP;
                            printf("AMP move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                                   prim->btConnId, prim->resultSupplier, prim->resultCode);
                        }
                        break;

                    case CSR_BT_AMPM_MOVE_CFM:
                        {
                            CsrBtAmpmMoveCfm *prim;
                            prim = (CsrBtAmpmMoveCfm*)bipsGlobalInstanceData->recvMsgP;
                            printf("AMP move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                                   prim->btConnId, prim->resultSupplier, prim->resultCode);
                        }
                        break;

                    case CSR_BT_AMPM_AUTO_MOVE_CMP_IND:
                        {
                            CsrBtAmpmAutoMoveCmpInd *prim;
                            prim = (CsrBtAmpmAutoMoveCmpInd*)bipsGlobalInstanceData->recvMsgP;
                            printf("AMP auto move channel complete indication, btConnId 0x%08x, supplier 0x%04x, result 0x%04x\n",
                                   prim->btConnId, prim->resultSupplier, prim->resultCode);
                        }
                        break;

                    default:
                        printf("Unhandled AMPM primitive received: type 0x%04x\n",
                               *(CsrPrim*)bipsGlobalInstanceData->recvMsgP);
                        break;
                } /* End of switch(*(CsrBtAmpmPrim*)inst->recvMsgP) */
            } /* End of case CSR_BT_AMPM_PRIM */
#endif
        case KEY_MESSAGE:
            {
                CsrBtKeyPress_t *key;

                key = (CsrBtKeyPress_t*)bipsGlobalInstanceData->recvMsgP;

                if (key->key == ESCAPE_KEY)
                {
                    CsrInt8 i;
                    for(i=0;i<MAX_NUMBER_OF_BIPS_INSTANCES;i++)
                    {
                        CsrPmemFree(bipsGlobalInstanceData->bipsInstances[i].aa->usedFilter);
                        CsrPmemFree(bipsGlobalInstanceData->bipsInstances[i].aa->imageListObject);
                        CsrPmemFree(bipsGlobalInstanceData->bipsInstances[i].aa->imageProperties);
                        CsrPmemFree(bipsGlobalInstanceData->bipsInstances[i].aa->imgDescriptorStr);
                        CsrPmemFree(bipsGlobalInstanceData->bipsInstances[i].aa);
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
                    switch (bipsGlobalInstanceData->keyboardState)
                    {
                        case KEYBOARD_MENU_STATE :
                            {
                                switch (key->key)
                                {
                                    case '1': /* Activate or deactivate current instance */
                                        {
                                            if( (instData->instData->state == idle) | (instData->instData->state == deactivate) )
                                            {
                                                CsrBtBipsActivateReqSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                         bipsGlobalInstanceData->phandle,
                                                                         bipsGlobalInstanceData->upperDataCapacity,
                                                                         bipsGlobalInstanceData->lowerDataCapacity,
                                                                         (CSR_BT_BIPS_IMAGE_PUSH_FEATURE |
                                                                          CSR_BT_BIPS_AUTO_ARCHIVE_FEATURE |
                                                                          CSR_BT_BIPS_REMOTE_CAMERA_FEATURE),
                                                                         MAX_LOCAL_OBEX_SIZE,
                                                                         FALSE, /*digest*/
                                                                         0, /*windowSize*/
                                                                         TRUE); /*enableSrm*/

                                                bipsGlobalInstanceData->globalState = bipsGlobalInstanceData->globalState | FIRST_INST_ACTIVATED;
                                                bipsGlobalInstanceData->instData->state = activated;


                                            }
                                            else if( instData->instData->state == activated )
                                            {
                                                CsrBtBipsDeactivateReqSend(bipsGlobalInstanceData->instData->CsrSchedQid);
                                                bipsGlobalInstanceData->instData->state = deactivate;
                                            }
                                            /*system(CLS);*/
                                            showMenu(bipsGlobalInstanceData);
                                            break;
                                        }
                                    case '4':
                                    {
                                        if( (instData->instData->state == idle) | (instData->instData->state == deactivate) )
                                        {
                                            CsrBtBipsActivateReqSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                     bipsGlobalInstanceData->phandle,
                                                                     bipsGlobalInstanceData->upperDataCapacity,
                                                                     bipsGlobalInstanceData->lowerDataCapacity,
                                                                     (CSR_BT_BIPS_IMAGE_PUSH_FEATURE |
                                                                      CSR_BT_BIPS_AUTO_ARCHIVE_FEATURE |
                                                                      CSR_BT_BIPS_REMOTE_CAMERA_FEATURE),
                                                                     MAX_LOCAL_OBEX_SIZE,
                                                                     TRUE, /*digest*/
                                                                     0, /*windowSize*/
                                                                     TRUE); /*enableSrm*/

                                            bipsGlobalInstanceData->globalState = bipsGlobalInstanceData->globalState | FIRST_INST_ACTIVATED;
                                            bipsGlobalInstanceData->instData->state = activated;
                                            showMenu(bipsGlobalInstanceData);
                                        }
                                        break;
                                    }
                                    case '2': /* Change instance */
                                        {
                                            bipsGlobalInstanceData->numberOfActiveInst =  !(bipsGlobalInstanceData->numberOfActiveInst);
                                            bipsGlobalInstanceData->instData = &bipsGlobalInstanceData->bipsInstances[bipsGlobalInstanceData->numberOfActiveInst];

                                            if(instData->instData->state == idle)
                                            {
                                                CsrBtBipsGetInstancesQidReqSend( bipsGlobalInstanceData->phandle );
                                            }

                                            /*system(CLS);*/
                                            showMenu(bipsGlobalInstanceData);
                                            break;
                                        }
                                    case '3': /* Security Controller Configuration */
                                        {
                                            handleScKeybPrim(&bipsGlobalInstanceData->scInstData);
                                            break;
                                        }
                                    default:
                                        if(bipsGlobalInstanceData->instData->menuState == MENU_AA_STATE)
                                        {
                                            bipsAaHandleKeyPress(
                                                bipsGlobalInstanceData,
                                                bipsGlobalInstanceData->instData->CsrSchedQid,
                                                key->key);
                                        }
                                        else if(bipsGlobalInstanceData->instData->menuState == MENU_AA_BUSY_STATE)
                                        {
                                            bipsAaHandleAbortKeyPress(
                                                bipsGlobalInstanceData,
                                                bipsGlobalInstanceData->instData->CsrSchedQid,
                                                key->key);
                                        }
                                        break;

                                }
                                break;
                            }
                        case KEYBOARD_BONDING_STATE :
                            {
                                handleScKeybPrim(&bipsGlobalInstanceData->scInstData);
                                break;
                            }
                        case KEYBOARD_AUTHENTICATE_STATE:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {
                                            if (!bipsGlobalInstanceData->instData->authenticateLength)
                                            {
                                                CsrBtBipsAuthenticateResSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                        NULL,
                                                                        0,
                                                                        CsrStrDup((char *)"CSR-BIPS"));
                                            }
                                            else
                                            {
                                                CsrBtBipsAuthenticateResSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                        (CsrUint8 *)CsrStrDup(bipsGlobalInstanceData->instData->authenticatePassword),
                                                                        bipsGlobalInstanceData->instData->authenticateLength,
                                                                        CsrStrDup((char *)"CSR-SYNC"));
                                            }

                                            bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                                            /*system(CLS);*/
                                            showMenu(bipsGlobalInstanceData);
                                            break;
                                        }
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->authenticateLength > 0)
                                            {
                                                bipsGlobalInstanceData->instData->authenticateLength--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    default:
                                        {
                                            if ((key->key >= 32) && (key->key < 127))
                                            {
                                                if (bipsGlobalInstanceData->instData->authenticateLength < 16)
                                                {
                                                    bipsGlobalInstanceData->instData->authenticatePassword[bipsGlobalInstanceData->instData->authenticateLength++] = key->key;
                                                    printf("%c", key->key);
                                                }
                                            }
                                            else
                                            {
                                                printf("\a");
                                            }
                                            break;
                                        }
                                }
                                break;
                            }
                        case KEYBOARD_CHALLENGE_STATE:
                        {
                            switch (key->key)
                            {
                                case RETURN_KEY:
                                {
                                    if (!bipsGlobalInstanceData->instData->authenticateLength)
                                    {
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
                                        CsrBtBipsChallengeExtResSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                     0,
                                                                     NULL,
                                                                     0,
                                                                     NULL,
                                                                     NULL,
                                                                     TRUE);
#else
                                        CsrBtBipsChallengeResSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                  0,
                                                                  NULL,
                                                                  0,
                                                                  NULL,
                                                                  NULL);
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE       */
                                    }
                                    else
                                    {
#ifdef CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
                                        CsrBtBipsChallengeExtResSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                     0,
                                                                     NULL,
                                                                     bipsGlobalInstanceData->instData->authenticateLength,
                                                                     (CsrUint8 *)CsrStrDup(bipsGlobalInstanceData->instData->authenticatePassword),
                                                                     NULL,
                                                                     TRUE);
#else
                                        CsrBtBipsChallengeResSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                  0,
                                                                  NULL,
                                                                  bipsGlobalInstanceData->instData->authenticateLength,
                                                                  (CsrUint8 *)CsrStrDup(bipsGlobalInstanceData->instData->authenticatePassword),
                                                                  NULL);
#endif /* CSR_BT_OBEX_AUTH_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE       */

                                    }

                                    bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                                    /*system(CLS);*/
                                    showMenu(bipsGlobalInstanceData);
                                    break;
                                }
                                case BACKSPACE_KEY:
                                {
                                    if (bipsGlobalInstanceData->instData->authenticateLength > 0)
                                    {
                                        bipsGlobalInstanceData->instData->authenticateLength--;
                                        printf("\b \b");
                                    }
                                    break;
                                }
                                default:
                                {
                                    if ((key->key >= 32) && (key->key < 127))
                                    {
                                        if (bipsGlobalInstanceData->instData->authenticateLength < 16)
                                        {
                                            bipsGlobalInstanceData->instData->authenticatePassword[bipsGlobalInstanceData->instData->authenticateLength++] = key->key;
                                            printf("%c", key->key);
                                        }
                                    }
                                    else
                                    {
                                        printf("\a");
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                        case KEYBOARD_IMAGE_LIST_NB_RETURNED_HANDLES:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {
                                            CsrUint16 maxListSize = 0;

                                            bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = '\0';
                                            sscanf(bipsGlobalInstanceData->instData->inputStr,"%hd",&maxListSize);
                                            bipsGlobalInstanceData->instData->aa->maxListSize = maxListSize;

                                            if(maxListSize == 0)
                                            {
                                                bipsGlobalInstanceData->keyboardState = KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CODING;
                                                printf("\nChoose to filter on JPEG(j), GIF(g), or nothing (return) \n");

                                            }
                                            else
                                            {

                                                bipsGlobalInstanceData->keyboardState = KEYBOARD_IMAGE_LIST_START_OFFSET;
                                                printf("\nStart offset for image list: ");
                                            }

                                            bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            break;
                                        }
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStrIndex--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    case '0': case '1': case '2': case '3': case '4': case '5':
                                    case '6': case '7': case '8': case '9':
                                        if(bipsGlobalInstanceData->instData->inputStrIndex < 7)
                                        {
                                            bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                            bipsGlobalInstanceData->instData->inputStrIndex++;
                                            printf("%c",key->key);
                                        }
                                        break;

                                    default:
                                        {
                                            printf("\a");
                                            break;
                                        }
                                }
                                break;
                            }
                            break;
                        case KEYBOARD_IMAGE_LIST_START_OFFSET:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {
                                            CsrUint16 listOffset = 0;

                                            bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = '\0';
                                            sscanf(bipsGlobalInstanceData->instData->inputStr,"%hd",&listOffset);

                                            bipsGlobalInstanceData->instData->aa->listOffset = listOffset;
                                            bipsGlobalInstanceData->keyboardState = KEYBOARD_IMAGE_LIST_LATEST_CAPTURED_IMAGES;
                                            printf("\nRetrieve just the latest captured images (Y/N): ");


                                            bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            break;
                                        }
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStrIndex--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    case '0': case '1': case '2': case '3': case '4': case '5':
                                    case '6': case '7': case '8': case '9':
                                        if(bipsGlobalInstanceData->instData->inputStrIndex < 7)
                                        {
                                            bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                            bipsGlobalInstanceData->instData->inputStrIndex++;
                                            printf("%c",key->key);
                                        }
                                        break;

                                    default:
                                        {
                                            printf("\a");
                                            break;
                                        }
                                }
                                break;
                            }
                            break;
                        case KEYBOARD_IMAGE_LIST_LATEST_CAPTURED_IMAGES:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {

                                            if( (bipsGlobalInstanceData->instData->inputStr[0] == 'y') ||
                                                (bipsGlobalInstanceData->instData->inputStr[0] == 'Y'))
                                            {
                                                bipsGlobalInstanceData->instData->aa->lastImage = TRUE;
                                            }
                                            else
                                            {
                                                bipsGlobalInstanceData->instData->aa->lastImage = FALSE;
                                            }

                                            bipsGlobalInstanceData->keyboardState = KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CODING;
                                            printf("\nChoose to filter on JPEG(j), GIF(g), or nothing (return) \n");

                                            bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            break;
                                        }
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStrIndex--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    case 'Y': case 'y': case 'N': case 'n':
                                        if(bipsGlobalInstanceData->instData->inputStrIndex < 1)
                                        {
                                            bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                            bipsGlobalInstanceData->instData->inputStrIndex++;
                                            printf("%c",key->key);
                                        }
                                        break;

                                    default:
                                        {
                                            printf("\a");
                                            break;
                                        }
                                }
                                break;
                            }
                            break;
                        case KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CODING:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {
                                            if(bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = '\0';
                                                bipsGlobalInstanceData->instData->aa->imageDescriptorEncoding = CsrPmemAlloc(bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemSet(bipsGlobalInstanceData->instData->aa->imageDescriptorEncoding, 0, bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemCpy(bipsGlobalInstanceData->instData->aa->imageDescriptorEncoding, bipsGlobalInstanceData->instData->inputStr,bipsGlobalInstanceData->instData->inputStrIndex);
                                            }
                                            CsrMemSet(bipsGlobalInstanceData->instData->inputStr, 0, 200);
                                            bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            
                                            /* Now set the KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CREATION filter*/
                                            bipsGlobalInstanceData->keyboardState = KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CREATION;
                                            printf("\nChoose to filter on creation (format YYYMMDDTHHMMSS; or just return for no filtering on created): \n");
                                            break;
                                        }
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStrIndex--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    default:
                                        if ((key->key == 'j') ||(key->key == 'J') ||
                                            (key->key == 'g') ||(key->key == 'G'))
                                        {

                                            if(bipsGlobalInstanceData->instData->inputStrIndex < 1)
                                            {
                                                bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                                bipsGlobalInstanceData->instData->inputStrIndex++;
                                                printf("%c",key->key);
                                            }
                                            else
                                            {
                                                printf("\a");
                                            }
                                        }
                                        else
                                        {
                                            printf("\a");
                                        }
                                        break;
                                }
                                break;
                            }

                        case KEYBOARD_IMAGE_DESCRIPTOR_FILTER_CREATION:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = '\0';
                                                bipsGlobalInstanceData->instData->aa->imageDescriptorCreated = CsrPmemAlloc(bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemSet(bipsGlobalInstanceData->instData->aa->imageDescriptorCreated, 0, bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemCpy(bipsGlobalInstanceData->instData->aa->imageDescriptorCreated, bipsGlobalInstanceData->instData->inputStr,bipsGlobalInstanceData->instData->inputStrIndex);
                                             
                                                CsrMemSet(bipsGlobalInstanceData->instData->inputStr, 0, 200);
                                                bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            }
                                            /* Now set the KEYBOARD_IMAGE_DESCRIPTOR_FILTER_PIXEL_SIZE filter*/
                                            bipsGlobalInstanceData->keyboardState = KEYBOARD_IMAGE_DESCRIPTOR_FILTER_PIXEL_SIZE;
                                            printf("\nChoose to filter on pixel size (format width*height or just RETURN for no filtering on pixel size): \n");
                                            break;
                                        }
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStrIndex--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    default:
                                        bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                        bipsGlobalInstanceData->instData->inputStrIndex++;
                                        printf("%c",key->key);
                                        break;
                                }
                                break;
                            }
                        case KEYBOARD_IMAGE_DESCRIPTOR_FILTER_PIXEL_SIZE:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = '\0';
                                                bipsGlobalInstanceData->instData->aa->imageDescriptorPixel = CsrPmemAlloc(bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemSet(bipsGlobalInstanceData->instData->aa->imageDescriptorPixel, 0, bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemCpy(bipsGlobalInstanceData->instData->aa->imageDescriptorPixel, bipsGlobalInstanceData->instData->inputStr,bipsGlobalInstanceData->instData->inputStrIndex);
                                             
                                                CsrMemSet(bipsGlobalInstanceData->instData->inputStr, 0, 200);
                                                bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            }
                                            /* Now set the KEYBOARD_IMAGE_DESCRIPTOR_FILTER_MODIFIED filter*/
                                            bipsGlobalInstanceData->keyboardState = KEYBOARD_IMAGE_DESCRIPTOR_FILTER_MODIFIED;
                                            printf("\nChoose to filter on modified (format YYYMMDDTHHMMSS; or just return for no filtering on modified): \n");
                                            break;
                                        }
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStrIndex--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    default:
                                        bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                        bipsGlobalInstanceData->instData->inputStrIndex++;
                                        printf("%c",key->key);
                                        break;
                                }
                                break;
                            }
                        case KEYBOARD_IMAGE_DESCRIPTOR_FILTER_MODIFIED:
                            {
                                switch (key->key)
                                {
                                    case RETURN_KEY:
                                        {
                                            CsrUint8    * imageDescriptor = NULL;
                                            CsrUint16    imageDescriptorLength = 0,i = 0;
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = '\0';
                                                bipsGlobalInstanceData->instData->aa->imageDescriptorModified = CsrPmemAlloc(bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemSet(bipsGlobalInstanceData->instData->aa->imageDescriptorModified, 0, bipsGlobalInstanceData->instData->inputStrIndex);
                                                CsrMemCpy(bipsGlobalInstanceData->instData->aa->imageDescriptorModified, bipsGlobalInstanceData->instData->inputStr,bipsGlobalInstanceData->instData->inputStrIndex);
                                             
                                                CsrMemSet(bipsGlobalInstanceData->instData->inputStr, 0, 200);
                                                bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            }
                                            
                                            imageDescriptorLength = CsrBtReturnImgHandlesDescriptionLength(
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorCreated, 
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorModified, 
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorEncoding, 
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorPixel);
                                            
                                            
                                            imageDescriptor       = CsrBtBuildImgHandlesDescriptorHeader(
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorCreated, 
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorModified, 
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorEncoding, 
                                                    bipsGlobalInstanceData->instData->aa->imageDescriptorPixel,
                                                    imageDescriptorLength);
                                        
                                            for(i=0; i<imageDescriptorLength; i++)
                                                printf("%c",imageDescriptor[i]);

                                            if(bipsGlobalInstanceData->instData->aa->maxListSize == 0)
                                            {
                                                bipsGlobalInstanceData->instData->aa->listOffset = 0;
                                                bipsGlobalInstanceData->instData->aa->lastImage = FALSE;
                                            }
                                            CsrBtBipsAaGetImageListReqSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                      bipsGlobalInstanceData->instData->aa->maxListSize,
                                                                      bipsGlobalInstanceData->instData->aa->listOffset,
                                                                      bipsGlobalInstanceData->instData->aa->lastImage,
                                                                      imageDescriptorLength,
                                                                      imageDescriptor,
                                                                      FALSE);
                                            
                                            bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                            bipsGlobalInstanceData->instData->menuState = MENU_AA_BUSY_STATE;
                                            bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                                            CsrPmemFree(bipsGlobalInstanceData->instData->aa->imageDescriptorModified);
                                            bipsGlobalInstanceData->instData->aa->imageDescriptorModified = NULL;
                                            CsrPmemFree(bipsGlobalInstanceData->instData->aa->imageDescriptorEncoding);
                                            bipsGlobalInstanceData->instData->aa->imageDescriptorEncoding = NULL;
                                            CsrPmemFree(bipsGlobalInstanceData->instData->aa->imageDescriptorPixel);
                                            bipsGlobalInstanceData->instData->aa->imageDescriptorPixel = NULL;
                                            CsrPmemFree(bipsGlobalInstanceData->instData->aa->imageDescriptorCreated);
                                            bipsGlobalInstanceData->instData->aa->imageDescriptorCreated = NULL;
                                            showMenu(bipsGlobalInstanceData);
                                        }
                                        break;
                                    case BACKSPACE_KEY:
                                        {
                                            if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                            {
                                                bipsGlobalInstanceData->instData->inputStrIndex--;
                                                printf("\b \b");
                                            }
                                            break;
                                        }
                                    default:
                                        bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                        bipsGlobalInstanceData->instData->inputStrIndex++;
                                        printf("%c",key->key);
                                        break;
                                }
                            }
                            break;

                        case KEYBOARD_IMAGE_HANDLE:
                            switch (key->key)
                            {
                                case RETURN_KEY:
                                    {
                                        if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                        {
                                            CsrUint16 value = 0;
                                            bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = '\0';
                                            sscanf(bipsGlobalInstanceData->instData->inputStr,"%hd",&value);

                                            if (value < bipsGlobalInstanceData->instData->aa->imageHandleIndexMax)
                                            {
                                                bipsGlobalInstanceData->instData->aa->imageHandleIndex = value;
                                            }
                                            else
                                            {
                                                printf(" invalid input %d ignored\n",value);
                                            }

                                        }

                                        bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                        bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                                        printf("\n");
                                        showMenu(bipsGlobalInstanceData);
                                        break;
                                    }
                                case BACKSPACE_KEY:
                                    if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                    {
                                        bipsGlobalInstanceData->instData->inputStrIndex--;
                                        printf("\b \b");
                                    }
                                    break;

                                case '0': case '1': case '2': case '3': case '4': case '5':
                                case '6': case '7': case '8': case '9':
                                    bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                    bipsGlobalInstanceData->instData->inputStrIndex++;
                                    printf("%c",key->key);
                                    break;

                                default:
                                    printf("\a");
                                    break;
                            }
                            break;
                        case KEYBOARD_ATTACHMENT_TYPE:
                            switch (key->key)
                            {
                                case RETURN_KEY:
                                    {
                                        CsrBool allFine = TRUE;
                                        CsrUcs2String *fileName;

                                        bipsGlobalInstanceData->instData->aa->attachTypeWav = TRUE;


                                        if( (bipsGlobalInstanceData->instData->inputStr[0] == 't') ||
                                            (bipsGlobalInstanceData->instData->inputStr[0] == 'T'))
                                        {
                                            bipsGlobalInstanceData->instData->aa->attachTypeWav = FALSE;
                                        }

                                        if(bipsGlobalInstanceData->instData->aa->attachTypeWav)
                                        {
                                            fileName      = CsrUtf82Ucs2ByteString("attach.wav");
                                            if((instData->instData->currentFileHandle = CsrBtFopen("aa_attach.wav", "wb")) == NULL)
                                            {
                                                printf("Fail. BIPS DEMO APP could not open the file for writing\n");
                                                allFine = FALSE;
                                            }
                                        }
                                        else
                                        {
                                            fileName      = CsrUtf82Ucs2ByteString("attach.txt");
                                            if((instData->instData->currentFileHandle = CsrBtFopen("aa_attach.txt", "wb")) == NULL)
                                            {
                                                printf("Fail. BIPS DEMO APP could not open the file for writing\n");
                                                allFine = FALSE;
                                            }
                                        }
                                        if(allFine)
                                        {
                                            instData->instData->aa->firstPart = TRUE;
                                            CsrBtBipsAaGetLinkedAttachmentReqSend(
                                                                bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                bipsGlobalInstanceData->instData->aa->imageHandles[
                                                                bipsGlobalInstanceData->instData->aa->imageHandleIndex],
                                                                (CsrUint16)CsrUcs2ByteStrLen(fileName),
                                                                fileName,
                                                                FALSE);

                                            bipsGlobalInstanceData->instData->menuState = MENU_AA_BUSY_STATE;

                                        }
                                        bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                        bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                                        printf("\n");
                                        showMenu(bipsGlobalInstanceData);

                                    }
                                    break;
                                case BACKSPACE_KEY:
                                    if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                    {
                                        bipsGlobalInstanceData->instData->inputStrIndex--;
                                        printf("\b \b");
                                    }
                                    break;

                                case 'w':case 'W': case 't': case 'T':
                                    bipsGlobalInstanceData->instData->inputStr[bipsGlobalInstanceData->instData->inputStrIndex] = key->key;
                                    bipsGlobalInstanceData->instData->inputStrIndex++;
                                    printf("%c",key->key);
                                    break;

                                default:
                                    printf("\a");
                                    break;
                            }
                            break;
                        case KEYBOARD_GET_IMAGE_DESCRIPTOR:
                            switch (key->key)
                            {
                                case RETURN_KEY:
                                    {
                                        CsrUint16 i;
                                        bipsGlobalInstanceData->instData->inputStrIndex = 0;
                                        bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                                        printf("\n");

                                        for(i = 0; i < bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength; i++)
                                        {
                                            printf("%c",
                                                   bipsGlobalInstanceData->instData->aa->imgDescriptorStr[i]);
                                        }
                                        printf("\n");
                                        bipsGlobalInstanceData->instData->aa->firstPart    = TRUE;

                                        CsrBtBipsAaGetImageReqSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                   bipsGlobalInstanceData->instData->aa->imageHandles[bipsGlobalInstanceData->instData->aa->imageHandleIndex],
                                                                   bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength,
                                                                   bipsGlobalInstanceData->instData->aa->imgDescriptorStr,
                                                                   FALSE);
                                        bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                                        bipsGlobalInstanceData->instData->menuState = MENU_AA_BUSY_STATE;
                                        showMenu(bipsGlobalInstanceData);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength = 0;
                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr = NULL;
                                    }
                                    break;
                                case BACKSPACE_KEY:
                                    if (bipsGlobalInstanceData->instData->inputStrIndex > 0)
                                    {
                                        bipsGlobalInstanceData->instData->inputStrIndex--;
                                        printf("\b \b");
                                    }
                                    break;
                                case 't':
                                    printf("%c",key->key);
                                    bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength =
                                        CsrBtReturnImgDescriptionLength("JPEG", "160*120", "", "", "");
                                    bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                        CsrBtBuildImgDescriptorHeader("JPEG", "160*120", "", "", "",
                                                                 bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                    break;
                                case '0':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);
                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(0, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);

                                    }
                                    break;
                                case '1':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(1, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case '2':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(2, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case '3':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(3, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case '4':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(4, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case '5':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(5, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case '6':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(6, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case '7':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(7, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case '8':
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(8, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                case 9:
                                    {
                                        CsrUint8 *str;
                                        printf("%c\n",key->key);

                                        str = CsrPmemAlloc(bipsGlobalInstanceData->instData->aa->imagePropertiesLength);
                                        CsrMemCpy(str, bipsGlobalInstanceData->instData->aa->imageProperties,
                                               bipsGlobalInstanceData->instData->aa->imagePropertiesLength);

                                        bipsGlobalInstanceData->instData->aa->imgDescriptorStr =
                                            extractImageDescriptor(9, str,
                                                                   bipsGlobalInstanceData->instData->aa->imagePropertiesLength,
                                                                   &bipsGlobalInstanceData->instData->aa->imgDescriptorStrLength);
                                        CsrPmemFree(str);
                                    }
                                    break;
                                default:
                                    printf("\a");
                                    break;
                            }
                            break;

                        case KEYBOARD_GET_IMAGE_CAPABILITIES:
                            switch (key->key)
                            {
                                case 'b':
                                    bipsGlobalInstanceData->instData->capabilitiesString = capabilitiesXmlString1;
                                    bipsGlobalInstanceData->instData->capabilitiesStringSize = sizeof(capabilitiesXmlString1);
                                    break;
                                case 'c':
                                    bipsGlobalInstanceData->instData->capabilitiesString = capabilitiesXmlString2;
                                    bipsGlobalInstanceData->instData->capabilitiesStringSize = sizeof(capabilitiesXmlString2);
                                    break;
                                case 'd':
                                    bipsGlobalInstanceData->instData->capabilitiesString = capabilitiesXmlString3;
                                    bipsGlobalInstanceData->instData->capabilitiesStringSize = sizeof(capabilitiesXmlString3);
                                    break;
                                case 'a': /* a is default so do fall through */
                                default:
                                    bipsGlobalInstanceData->instData->capabilitiesString = capabilitiesXmlString;
                                    bipsGlobalInstanceData->instData->capabilitiesStringSize = sizeof(capabilitiesXmlString);
                                    break;
                            }
                            bipsGlobalInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                            CsrBtBipsPushGetCapabilitiesHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid,
                                                                      CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                                                      FALSE);
                            break;
                    }
                }
                break;
            }

        default:
            {
                printf("A state event error occurred in the BIPS Application layer,\n  it is a UNKNOWN_PRIM %04X\n", event);
                break;
            }
    }
    CsrPmemFree(bipsGlobalInstanceData->recvMsgP);
}



/******************************************************************************
  getBIPSInstFromQid:
*****************************************************************************/
static void setActiveInstance( BipsGlobalInstanceDataType *instData, CsrSchedQid CsrSchedQid)
{
    CsrUint8 i = 0;

    instData->instData = NULL;

    for(i=0; i<instData->numberOfBipsInstances; i++)
    {
        if (CsrSchedQid == instData->bipsInstances[i].CsrSchedQid)
        {
            instData->instData = &instData->bipsInstances[i];
        }
    }

    if( instData->instData == NULL )
    {
        printf("Queue mismatch \n");
    }
}


/******************************************************************************
 * bipsAaHandleKeyPress: The Automatic archive feature has turned the server
 * into a sort of client, and we must be able to read the users keypress
 *****************************************************************************/

static void bipsAaHandleKeyPress(BipsGlobalInstanceDataType *instData,
                                 CsrSchedQid CsrSchedQid, char key)
{
    switch(key)
    {
        case 'a':  /* a)  Get imaging capabilities */
            CsrBtBipsAaGetCapabilitiesReqSend(CsrSchedQid, FALSE);
            instData->instData->menuState = MENU_AA_BUSY_STATE;
            showMenu(instData);
            break;
        case 'b':/*  b) Get image list */
            {
                if(instData->instData->aa->imageListObject)
                {
                    CsrPmemFree(instData->instData->aa->imageListObject);
                    instData->instData->aa->imageListObject = NULL;
                }
                instData->instData->aa->imageListObjectLength    = 0;
                if(instData->instData->aa->usedFilter)
                {
                    CsrPmemFree(instData->instData->aa->usedFilter);
                    instData->instData->aa->usedFilter = NULL;
                }
                instData->instData->aa->firstPart    = TRUE;

                instData->keyboardState = KEYBOARD_IMAGE_LIST_NB_RETURNED_HANDLES;

                printf("Specify CSRMAX size of image list \nSpecial values (0: Request length of list, 65535: not limited)):");
            }
            break;
        case 'c':/*  c) Get image properties */
            if((instData->instData->aa->imageListExist)&&
               (instData->instData->aa->imageListObjectLength > 0))
            {
                instData->instData->aa->firstPart    = TRUE;
                instData->instData->aa->imagePropertiesLength = 0;
                CsrBtBipsAaGetImagePropertiesReqSend(CsrSchedQid,instData->instData->aa->imageHandles[instData->instData->aa->imageHandleIndex], FALSE);
                instData->instData->menuState = MENU_AA_BUSY_STATE;
                showMenu(instData);
            }
            else
            {
                printf("No Image List exist\n");
            }
            break;
        case 'd':/*  d) Get image */
            if((instData->instData->aa->imageListExist)&&
               (instData->instData->aa->imageListObjectLength > 0))
            {
                if((instData->instData->currentFileHandle = CsrBtFopen("aa_image.jpg", "wb")) == NULL)
                {
                    printf("Fail. BIPS DEMO APP could not open the file for writing\n");
                }
                else
                {
                    CsrUint8 *str;
                    str = CsrPmemAlloc(instData->instData->aa->imagePropertiesLength);
                    CsrMemCpy(str, instData->instData->aa->imageProperties,
                           instData->instData->aa->imagePropertiesLength);

                    instData->keyboardState = KEYBOARD_GET_IMAGE_DESCRIPTOR;
                    printf("Choose among the following image formats: \n");
                    extractImageProperties(str,
                                           instData->instData->aa->imagePropertiesLength);
                    CsrPmemFree(str);

                    printf("\nt) thumbnail \nencoding               : JPEG \npixel                  : 160*120");
                    printf("\n\n<enter> for no image descriptor\n");

                    instData->instData->aa->imgDescriptorStrLength = 0;
                    CsrPmemFree(instData->instData->aa->imgDescriptorStr);
                    instData->instData->aa->imgDescriptorStr = NULL;

                }
            }
            else
            {
                printf("No Image List exist\n");
            }
            break;
        case 'e':/*  e) Get linked thumbnail */
            if((instData->instData->aa->imageListExist)&&
               (instData->instData->aa->imageListObjectLength > 0))
            {
                if((instData->instData->currentFileHandle = CsrBtFopen("aa_thumb.jpg", "wb")) == NULL)
                {
                    instData->instData->aa->firstPart    = TRUE;
                    printf("Fail. BIPS DEMO APP could not open the file for writing\n");
                }
                else
                {
                    CsrBtBipsAaGetLinkedThumbnailReqSend(CsrSchedQid,
                                                         instData->instData->aa->imageHandles[instData->instData->aa->imageHandleIndex],
                                                         FALSE);
                    instData->instData->menuState = MENU_AA_BUSY_STATE;
                    showMenu(instData);
                }

            }
            else
            {
                printf("No Image List exist\n");
            }
            break;
        case 'f':/*  f) Get linked attachment */
            if((instData->instData->aa->imageListExist)&&
               (instData->instData->aa->imageListObjectLength > 0))
            {

                instData->keyboardState = KEYBOARD_ATTACHMENT_TYPE;

                printf("Specify attachment type [w]av or [t]xt (default w): ");
            }
            else
            {
                printf("No Image List exist\n");
            }

            break;
        case 'g':/*  g) Delete image */
            if((instData->instData->aa->imageListExist)&&
               (instData->instData->aa->imageListObjectLength > 0))
            {
                printf("Requesting deletion of image\n");
                CsrBtBipsAaDeleteImageReqSend(CsrSchedQid, instData->instData->aa->imageHandles[instData->instData->aa->imageHandleIndex]);
                instData->instData->menuState = MENU_AA_BUSY_STATE;
                showMenu(instData);
            }
            else
            {
                printf("No Image List exist\n");
            }
            break;
        case 'h':/*  h) Choose a different image handle (currently: XXXXXXX) */
            if((instData->instData->aa->imageListExist)&&
               (instData->instData->aa->imageListObjectLength > 0))
            {
                CsrInt16 i;
                for(i=0;i < instData->instData->aa->imageHandleIndexMax; i++)
                {
                    CsrUint16 j;
                    printf("%d to choose handle=\"",i);

                    for(j = 0; j < 7; j++)
                    {
                        printf("%c", instData->instData->aa->imageHandles[i][j]);
                    }
                    printf("\"\n");
                }

                printf("\nChoose from image handles above: [0:%d] \n",
                       instData->instData->aa->imageHandleIndexMax-1 );
                /* Reset image properties and descriptor string */
                instData->instData->aa->imagePropertiesLength = 0;
                CsrPmemFree(instData->instData->aa->imageProperties);
                instData->instData->aa->imageProperties = NULL;


                CsrPmemFree(instData->instData->aa->imgDescriptorStr);
                instData->instData->aa->imgDescriptorStr = NULL;
                instData->instData->aa->imgDescriptorStrLength = 0;


                instData->keyboardState = KEYBOARD_IMAGE_HANDLE;
            }
            else
            {
                printf("No Image List exist\n");
            }
            break;
        case '8':
            instData->instData->aa->imageListExist = FALSE;
            printf("\n\n Releasing the OBEX connection\n");
            CsrBtBipsDisconnectReqSend(CsrSchedQid, TRUE);
            break;
        default:
            break;
    }
}

static void bipsAaHandleAbortKeyPress(BipsGlobalInstanceDataType *instData,
                                      CsrSchedQid CsrSchedQid, char key)
{
    switch(key)
    {
        case 'x':
            printf("\n\n Cancelling the current operation\n");
            CsrBtBipsAaAbortReqSend(CsrSchedQid);
            break;
        default:
            break;
    }
}
/******************************************************************************
 * bipsDeactivateIndHandler: This signal is indicating that the
 * Image Push server has been deactivated
 *****************************************************************************/
static void
bipsDeactivateIndHandler(BipsGlobalInstanceDataType *bipsGlobalInstanceData)
{
    bipsGlobalInstanceData->instData->sendIndex        = 0;
    bipsGlobalInstanceData->instData->sendObjectLength = 0;

    if (bipsGlobalInstanceData->instData->writingFileHandle)
    {
        CsrBtFclose(bipsGlobalInstanceData->instData->writingFileHandle);
        bipsGlobalInstanceData->instData->writingFileHandle = NULL;
    }
    CsrPmemFree(bipsGlobalInstanceData->instData->imageFileName);
    bipsGlobalInstanceData->instData->imageFileName = NULL;
    CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
    bipsGlobalInstanceData->instData->thumbnailFileName = NULL;

    /*system(CLS);*/
    showMenu(bipsGlobalInstanceData);
}

/******************************************************************************

     *****************************************************************************/
static void bipsGetInstQidCfmHandler(BipsGlobalInstanceDataType *instData)
{
    CsrUint8 i;

    CsrBtBipsGetInstancesQidCfm *myPrim = (CsrBtBipsGetInstancesQidCfm *) instData->recvMsgP;

    instData->numberOfBipsInstances = myPrim->phandlesListSize;

    if( myPrim->phandlesListSize > MAX_NUMBER_OF_BIPS_INSTANCES )
    {
        printf("\n Error. Not possible to activate more then %i instances",instData->numberOfBipsInstances );
    }
    for(i=0;i<myPrim->phandlesListSize;i++)
    {
        instData->bipsInstances[i].CsrSchedQid = myPrim->phandlesList[i];
}

    CsrPmemFree(myPrim->phandlesList);
    /*system(CLS);*/
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    showMenu(instData);
}

/******************************************************************************
 *  bipsConnectIndHandler: This signal is indicating that BIP client is
 * starting a image push session. The application can accept or deny
 * the request but has to return the connectionId received in the indication
 *****************************************************************************/
static void bipsConnectIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsConnectInd * prim;

    prim = (CsrBtBipsConnectInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    bipsGlobalInstanceData->instData->maxPeerObexPacketSize = prim->obexPeerMaxPacketSize;
    bipsGlobalInstanceData->instData->connectionId          = prim->connectionId;
    if ( bipsGlobalInstanceData->instData->maxPeerObexPacketSize > MAX_PEER_PACKET_SIZE)
    {
        /* Only to ensure that we don't try to allocate a packet size
         * larger than our system can pmalloc
         */
        bipsGlobalInstanceData->instData->maxPeerObexPacketSize = MAX_PEER_PACKET_SIZE;
    }
    printf("\n\nBluetooth address: %04X %02X %06X is now connected to the server\n",
           prim->deviceAddr.nap,
           prim->deviceAddr.uap,
           prim->deviceAddr.lap);
    switch (prim->connectType)
    {
    case CSR_BT_BIPS_PUSH_CONNECT:
        {
            printf("The server is acting Image Push\n");
            break;
        }
    case CSR_BT_BIPS_RC_CONNECT:
        {
            printf("The server is acting Remote Camera\n");
            break;
        }
    case CSR_BT_BIPS_AA_CONNECT:
        {
            bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
            printf("The server is acting Automatic Archive\n");
            showMenu(bipsGlobalInstanceData);
            break;
        }
    default:
        {
            /* This should not happen */
            printf("The server har received an unknown connection\n");
            break;
        }
    }
    CsrBtBipsConnectResSend(bipsGlobalInstanceData->instData->CsrSchedQid, prim->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
#ifdef CSR_AMP_ENABLE
    printf("Registering with AMP manager...\n");
    CsrBtAmpmRegisterReqSend(TESTQUEUE,
                             0, /* flags */
                             CSR_BT_AMPM_EVENTS_AUTO_MOVE, /* eventMask */
                             prim->btConnId, /* btConnId */
                             &prim->deviceAddr); /* addr */
#endif

}

/*****************************************************************************
 * bipsAuthenticateIndHandler: This Indication signal is used when the BIP
 * client wants to OBEX authenticate the BIP server. The application has to
 * response with a password or pin number in the bipsResponsePassword and
 * a bipsResponseUserId for client to identify the proper password.
 *****************************************************************************/
static void bipsAuthenticateIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAuthenticateInd * prim;

    prim = (CsrBtBipsAuthenticateInd *) bipsGlobalInstanceData->recvMsgP;

    printf("Enter the OBEX response passkey: ");
    bipsGlobalInstanceData->instData->authenticateLength = 0;
    bipsGlobalInstanceData->keyboardState                = KEYBOARD_AUTHENTICATE_STATE;
    CsrPmemFree(prim->realm);
}

/******************************************************************************
 * bipsGetCapabilitiesHeaderIndHandler: This Indication signal is the
 * first part of an operation where the client has requested to retrieve the
 * imaging-capabilities object from the BIP server
 *****************************************************************************/
static void bipsGetCapabilitiesHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsPushGetCapabilitiesHeaderInd * prim;

    prim = (CsrBtBipsPushGetCapabilitiesHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        printf("\n(a) %s\n\n(b) %s\n\n(c) %s\n\n(d) %s \n\nChoose among one of the above capabilities objects (a is default):  \n\n",
               capabilitiesXmlString,
               capabilitiesXmlString1,
               capabilitiesXmlString2,
               capabilitiesXmlString3);

        bipsGlobalInstanceData->keyboardState = KEYBOARD_GET_IMAGE_CAPABILITIES;
    }
    else
    {
        CsrBtBipsPushGetCapabilitiesHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, FALSE);
    }
}

/******************************************************************************
 * bipsGetCapabilitiesObjectIndHandler: This signal is a part of an
 * operation where the client has requested to retrieve the
 * imagingCapabilities object from the BIP server. The imaging-capabilities
 * object is a mandatory object that describes in more detail the various
 * options, formats, and attributes that are supported by the BIP server.
 *****************************************************************************/
static void bipsGetCapabilitiesObjectIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode             responseCode;
    CsrUint8                        *capabilitiesObject;
    CsrUint16                          length;
    CsrBtBipsPushGetCapabilitiesObjectInd  *prim;

    prim = (CsrBtBipsPushGetCapabilitiesObjectInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        CsrUint16                        index;

        index = bipsGlobalInstanceData->instData->sendIndex;

        if (bipsGlobalInstanceData->instData->sendObjectLength == 0)
        {
            /* This is the first BipsPushGetxCapabilitiesObjectInd message */
            bipsGlobalInstanceData->instData->sendObjectLength =
                bipsGlobalInstanceData->instData->capabilitiesStringSize;
        }

        if (bipsGlobalInstanceData->instData->sendObjectLength > bipsGlobalInstanceData->instData->maxPeerObexPacketSize)
        {
            /* The capabilitiesObject can not be sent in a single packet */
            bipsGlobalInstanceData->instData->sendObjectLength -= bipsGlobalInstanceData->instData->maxPeerObexPacketSize;
            length                                              = bipsGlobalInstanceData->instData->maxPeerObexPacketSize;
            responseCode                                   = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            bipsGlobalInstanceData->instData->sendIndex        += length;
        }
        else
        {
            /* The capabilitiesObject or the last part of it is now sent */
            length                                            = bipsGlobalInstanceData->instData->sendObjectLength;
            bipsGlobalInstanceData->instData->sendIndex        = 0;
            bipsGlobalInstanceData->instData->sendObjectLength = 0;
            responseCode                                    = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
        capabilitiesObject    = (CsrUint8 *) CsrPmemAlloc(length);
        CsrMemCpy(capabilitiesObject,
               &bipsGlobalInstanceData->instData->capabilitiesString[index],
               length);

    }
    else
    {
        responseCode                                    = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        capabilitiesObject                                = NULL;
        length                                            = 0;
    }
    CsrBtBipsPushGetCapabilitiesObjectResSend(bipsGlobalInstanceData->instData->CsrSchedQid, length, capabilitiesObject, responseCode, FALSE);
}

/******************************************************************************
 * bipsPutImageHeaderIndHandler: This Indication signal is the first part
 *  of an operation where the client pushed an image to the BIP server.
 *****************************************************************************/
static void bipsPutImageHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode            responseCode;
    CsrBtBipsPushPutImageHeaderInd   *prim;
    char                        imageHandle[CSR_BT_IMG_HANDLE_LENGTH + 1];

    responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;

    printf("Received image header\n");

    CsrPmemFree(bipsGlobalInstanceData->instData->imageFileName);
    bipsGlobalInstanceData->instData->imageFileName = NULL;

    prim = (CsrBtBipsPushPutImageHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if(prim->ucs2nameOffset != 0)
    {
        bipsGlobalInstanceData->instData->imageFileName  = CsrUcs2ByteString2Utf8(prim->payload + prim->ucs2nameOffset);
    }
    else
    {
        bipsGlobalInstanceData->instData->imageFileName = copyString("unknownName.jpg");
    }

    printf("\n\nThe client is now pushing an image called %s.\n", bipsGlobalInstanceData->instData->imageFileName);

    if ((prim->connectionId == bipsGlobalInstanceData->instData->connectionId) &&
        (prim->imageDescriptorOffset != 0))
    {
        if((bipsGlobalInstanceData->instData->writingFileHandle = CsrBtFopen(bipsGlobalInstanceData->instData->imageFileName, "wb")) == NULL)
        {
            responseCode = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
            printf("Fail. BIP DEMO APP could not open the file for writing\n");
            CsrPmemFree(bipsGlobalInstanceData->instData->imageFileName);
            bipsGlobalInstanceData->instData->imageFileName = NULL;
        }
        else
        {
            char *pixelStr = NULL;
            pixelStr = interpretImageDescriptor((prim->payload + prim->imageDescriptorOffset), prim->imageDescriptorLength);
            CsrPmemFree(pixelStr);
            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            bipsGlobalInstanceData->instData->currentSize = 0;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        CsrPmemFree(bipsGlobalInstanceData->instData->imageFileName);
        bipsGlobalInstanceData->instData->imageFileName = NULL;
    }

    bipsGlobalInstanceData->instData->imageHandleCount++;
    if (bipsGlobalInstanceData->instData->imageHandleCount == 0)
    {
        bipsGlobalInstanceData->instData->imageHandleCount++;
    }
    sprintf(imageHandle, "%07d", bipsGlobalInstanceData->instData->imageHandleCount);

    printf("Transferring image with handle '%s'... Please wait...\n",
           imageHandle);

    CsrBtBipsPushPutImageHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, imageHandle, responseCode, FALSE);
    CsrPmemFree(prim->payload);
}

/******************************************************************************
 * bipsPutImageFileIndHandler: This signal is a part of an operation where
 * the client pushed an image to the BIP server. Please notice that
 * the BIP server uses the last response code to request the client to send
 * the thumbnail version of the image it just received.
 *****************************************************************************/
static void bipsPutImageFileIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsPushPutImageFileInd    * prim;

    CsrBtObexResponseCode    responseCode   = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    prim                                    = (CsrBtBipsPushPutImageFileInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        printf(".");
        CsrBtFwrite((prim->payload + prim->imageFileOffset),
                  1,
                  prim->imageFileLength,
                  bipsGlobalInstanceData->instData->writingFileHandle);
        bipsGlobalInstanceData->instData->currentSize += prim->imageFileLength;

        if (prim->finalFlag)
        {
#ifndef __linux__
            char temp[300];
#endif

            responseCode = CSR_BT_OBEX_PARTIAL_CONTENT_RESPONSE_CODE;
            CsrBtFclose(bipsGlobalInstanceData->instData->writingFileHandle);
            bipsGlobalInstanceData->instData->writingFileHandle = NULL;
            updateTotalImagingDataCapacity(bipsGlobalInstanceData);
#ifndef __linux__
            sprintf(temp, "start %s", bipsGlobalInstanceData->instData->imageFileName);
            system(temp);
#endif
            printf("Image transfer complete\n");
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        bipsGlobalInstanceData->instData->currentSize = 0;
    }
    CsrBtBipsPushPutImageFileResSend(bipsGlobalInstanceData->instData->CsrSchedQid, responseCode, FALSE);
    CsrPmemFree(prim->payload);
}

/******************************************************************************
 * bipsPutLinkedThumbnailHeaderIndHandler: This Indication signal is the
 * first part of an operation where the client pushed a thumbnail version of
 * the image just being send to the BIP server.
 *****************************************************************************/
static void bipsPutLinkedThumbnailHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode                       responseCode;
    CsrBtBipsPushPutLinkedThumbnailHeaderInd    * prim;

    prim    = (CsrBtBipsPushPutLinkedThumbnailHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
    bipsGlobalInstanceData->instData->thumbnailFileName = NULL;

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        /* 11 is length of "thumbnail_" incl '\0' */
        CsrUint32 length = CsrStrLen(bipsGlobalInstanceData->instData->imageFileName) + 11;

        bipsGlobalInstanceData->instData->thumbnailFileName = (CsrUint8 *) CsrPmemAlloc(length);
        CsrStrCpy(bipsGlobalInstanceData->instData->thumbnailFileName, "thumbnail_");
        CsrMemCpy(&bipsGlobalInstanceData->instData->thumbnailFileName[10],
               bipsGlobalInstanceData->instData->imageFileName,
               (length - 10));

        if((bipsGlobalInstanceData->instData->writingFileHandle = CsrBtFopen(bipsGlobalInstanceData->instData->thumbnailFileName, "wb")) == NULL)
        {
            printf("Fail. BIP DEMO APP could not open the file for writing\n");
            responseCode = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
            CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
            bipsGlobalInstanceData->instData->thumbnailFileName = NULL;
        }
        else
        {
            bipsGlobalInstanceData->instData->currentSize = 0;
            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
    }

    CsrBtBipsPushPutLinkedThumbnailHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, responseCode, FALSE);
    CsrPmemFree(prim->payload);
}


/*****************************************************************************
 * bipsPutLinkedThumbnailFileIndHandler: This signal is a part of an
 * operation where the client pushed a thumbnail version of an image to
 * the BIP server
 *****************************************************************************/
static void bipsPutLinkedThumbnailFileIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsPushPutLinkedThumbnailFileInd * prim;

    CsrBtObexResponseCode responseCode    = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    prim                 = (CsrBtBipsPushPutLinkedThumbnailFileInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        printf(".");
        CsrBtFwrite((prim->payload + prim->thumbnailFileOffset), 1, prim->thumbnailFileLength, bipsGlobalInstanceData->instData->writingFileHandle);
        bipsGlobalInstanceData->instData->currentSize += prim->thumbnailFileLength;
        if (prim->finalFlag)
        {
#ifndef __linux__
            char temp[300];
#endif
            CsrBtFclose(bipsGlobalInstanceData->instData->writingFileHandle);
            bipsGlobalInstanceData->instData->writingFileHandle = NULL;
            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            updateTotalImagingDataCapacity(bipsGlobalInstanceData);
#ifndef __linux__
            sprintf(temp, "start %s", bipsGlobalInstanceData->instData->thumbnailFileName);
            system(temp);
#endif
            CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
            bipsGlobalInstanceData->instData->thumbnailFileName = NULL;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        bipsGlobalInstanceData->instData->currentSize = 0;
        CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
        bipsGlobalInstanceData->instData->thumbnailFileName = NULL;
    }

    CsrBtBipsPushPutLinkedThumbnailFileResSend(bipsGlobalInstanceData->instData->CsrSchedQid, responseCode, FALSE);
    CsrPmemFree(prim->payload);
}

/******************************************************************************
 * bipsPutLinkedAttachmentHeaderIndHandler: This Indication signal is the
 * first part of an operation where the client pushed attachments associates
 *  with an image to the BIP server
 *****************************************************************************/
static void bipsPutLinkedAttachmentHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode                        responseCode;
    CsrBtBipsPushPutLinkedAttachmentHeaderInd    * prim;

    prim = (CsrBtBipsPushPutLinkedAttachmentHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    CsrPmemFree(bipsGlobalInstanceData->instData->attachmentFileName);
    bipsGlobalInstanceData->instData->attachmentFileName = NULL;
    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        interpretAttachmentDescriptor(bipsGlobalInstanceData,
                                      (prim->payload + prim->attachmentDescriptorOffset),
                                      prim->attachmentDescriptorLength);

        if(bipsGlobalInstanceData->instData->attachmentFileName)
        {

            if((bipsGlobalInstanceData->instData->writingFileHandle = CsrBtFopen(bipsGlobalInstanceData->instData->attachmentFileName, "wb")) == NULL)
            {
                printf("Fail. BIP DEMO APP could not open the file for writing\n");
                responseCode = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
                CsrPmemFree(bipsGlobalInstanceData->instData->attachmentFileName);
                bipsGlobalInstanceData->instData->attachmentFileName = NULL;

            }
            else
            {
                bipsGlobalInstanceData->instData->currentSize = 0;
                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            }
        }
        else
        {
            printf("Fail. No attachment file name found\n");
            responseCode = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        CsrPmemFree(bipsGlobalInstanceData->instData->attachmentFileName);
        bipsGlobalInstanceData->instData->attachmentFileName = NULL;
    }



    printf("Transferring attachment associated to image handle '%s'... Please wait...\n",
          CsrUcs2ByteString2Utf8(prim->payload + prim->imageHandleOffset) );

    CsrBtBipsPushPutLinkedAttachmentHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, responseCode, FALSE);
    CsrPmemFree(prim->payload);
}

/******************************************************************************
 * bipsPutLinkedAttachmentFileIndHandler: This signal is a part of an
 * operation where the client pushed attachments associates with an
 * image to the BIP server.
 *****************************************************************************/
static void bipsPutLinkedAttachmentFileIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsPushPutLinkedAttachmentFileInd * prim;

    CsrBtObexResponseCode responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    prim                 = (CsrBtBipsPushPutLinkedAttachmentFileInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        printf(".");
        CsrBtFwrite((prim->payload + prim->attachmentFileOffset), 1, prim->attachmentFileLength, bipsGlobalInstanceData->instData->writingFileHandle);
        bipsGlobalInstanceData->instData->currentSize += prim->attachmentFileLength;
        if (prim->finalFlag)
        {
#ifndef __linux__
            char temp[300];
#endif
            CsrBtFclose(bipsGlobalInstanceData->instData->writingFileHandle);
            bipsGlobalInstanceData->instData->writingFileHandle = NULL;
            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            updateTotalImagingDataCapacity(bipsGlobalInstanceData);
#ifndef __linux__
            sprintf(temp, "start %s", bipsGlobalInstanceData->instData->attachmentFileName);
            system(temp);
#endif

            printf("Attachment %s has been saved on the disk\n", bipsGlobalInstanceData->instData->attachmentFileName);
            CsrPmemFree(bipsGlobalInstanceData->instData->attachmentFileName);
            bipsGlobalInstanceData->instData->attachmentFileName = NULL;
        }
    }
    else
    {
        bipsGlobalInstanceData->instData->currentSize = 0;
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
    }
    CsrBtBipsPushPutLinkedAttachmentFileResSend(bipsGlobalInstanceData->instData->CsrSchedQid, responseCode, FALSE);
    CsrPmemFree(prim->payload);
}

/******************************************************************************
 * bipsGetMonitoringImageHeaderIndHandler: This Indication signal is the
 * first part of an operation where the client has requested to retrieve the
 * monitoring image object from the BIP server
 *****************************************************************************/
static void bipsGetMonitoringImageHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsRcGetMonitoringImageHeaderInd * prim;
    CsrUint8 imageHandle[7];

    prim = (CsrBtBipsRcGetMonitoringImageHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {

        bipsGlobalInstanceData->instData->currentMonImg++;
        if (bipsGlobalInstanceData->instData->currentMonImg > 3)
        {
            bipsGlobalInstanceData->instData->currentMonImg = 1;
        }

        if (prim->storeFlag == TRUE)
        {
            sprintf(imageHandle, "%07d", bipsGlobalInstanceData->instData->currentMonImg);
        }
        CsrBtBipsRcGetMonitoringImageHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, imageHandle, FALSE);

    }
    else
    {
        CsrBtBipsRcGetMonitoringImageHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, NULL, FALSE);
    }
}

/******************************************************************************
 * bipsGetMonitoringImageObjectIndHandler: This signal is a part of an
 * operation where the client has requested to retrieve the
 * monitoring image object from the BIP server.
 *****************************************************************************/
static void bipsGetMonitoringImageObjectIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode              responseCode;
    CsrUint16                           length;
    CsrUint8                         *monImgObject;
    CsrBtBipsRcGetMonitoringImageObjectInd  *prim;

    prim = (CsrBtBipsRcGetMonitoringImageObjectInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        if (!bipsGlobalInstanceData->instData->currentFileHandle)
        {
            int        result;
            CsrBtStat_t statBuffer;

            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
            statBuffer.st_size = 0;
            switch (bipsGlobalInstanceData->instData->currentMonImg)
            {
            case 1:
                {
                    CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_1, sizeof(MON_IMG_1));
                    break;
                }
            case 2:
                {
                    CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_2, sizeof(MON_IMG_2));
                    break;
                }
            default:
                {
                    CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_3, sizeof(MON_IMG_3));
                    break;
                }
            }

            result = CsrBtStat(bipsGlobalInstanceData->instData->imgName, &statBuffer);
            if(!result)
            {
                if((bipsGlobalInstanceData->instData->currentFileHandle = CsrBtFopen(bipsGlobalInstanceData->instData->imgName, "rb")) == NULL)
                {
                    printf("Fail. DEMO could not open the file %s\n", bipsGlobalInstanceData->instData->imgName);
                    return;
                }
                else
                {
                    bipsGlobalInstanceData->instData->currentFileSize = statBuffer.st_size;
                }
            }
            else
            {
                printf("Fail. DEMO could not open the file %s\n", bipsGlobalInstanceData->instData->imgName);
                return;
            }
        }
        else
        {
            ;
        }

        if (bipsGlobalInstanceData->instData->currentFileSize > prim->allowedImageLength)
        {
            /* The monitoring image object can not be sent in a single packet */
            bipsGlobalInstanceData->instData->currentFileSize -= prim->allowedImageLength;
            length                                     = prim->allowedImageLength;
            responseCode                               = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        else
        {
            /* The monitoring image object or the last part of it is now sent */
            /* The cast from CsrUint32 to CsrUint16 does no discard or corrupt any data
               since to OBEX package cannot be larger than CsrUint16 and this has already
               been checked for. */
            length                                               = (CsrUint16) bipsGlobalInstanceData->instData->currentFileSize;
            bipsGlobalInstanceData->instData->currentFilePosition = 0;
            bipsGlobalInstanceData->instData->currentFileSize     = 0;
            responseCode                              = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            printf("Image transfer complete\n");

        }
        monImgObject = CsrPmemAlloc(length);
        bipsGlobalInstanceData->instData->currentFilePosition += CsrBtFread(monImgObject, 1, length, bipsGlobalInstanceData->instData->currentFileHandle);

        if (bipsGlobalInstanceData->instData->currentFileSize <= 0)
        {
            CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle);
            bipsGlobalInstanceData->instData->currentFileHandle = NULL;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        monImgObject = NULL;
        length       = 0;
    }
    CsrBtBipsRcGetMonitoringImageObjectResSend(bipsGlobalInstanceData->instData->CsrSchedQid, length, monImgObject, responseCode, FALSE);
}

/******************************************************************************
 * bipsGetImagePropertiesHeaderIndHandler: This Indication signal is the
 * first part of an operation where the client has requested to retrieve the
 * image-properties object about an image from the BIP server
 *****************************************************************************/
static void bipsGetImagePropertiesHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsRcGetImagePropertiesHeaderInd * prim;

    prim = (CsrBtBipsRcGetImagePropertiesHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        char  *utfHandle;

        bipsGlobalInstanceData->instData->reqHandle = 0;

        utfHandle = (char*) CsrUcs2ByteString2Utf8(prim->payload + prim->imageHandleOffset);
        if (CsrBtStrICmpLength(utfHandle, "0000001", 7))
        {
            bipsGlobalInstanceData->instData->reqHandle = 1;
        }
        if (CsrBtStrICmpLength(utfHandle, "0000002", 7))
        {
            bipsGlobalInstanceData->instData->reqHandle = 2;
        }
        if (CsrBtStrICmpLength(utfHandle, "0000003", 7))
        {
            bipsGlobalInstanceData->instData->reqHandle = 3;
        }
        CsrBtBipsRcGetImagePropertiesHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
        CsrPmemFree(utfHandle);
    }
    else
    {
        CsrBtBipsRcGetImagePropertiesHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, FALSE);
    }
}

/******************************************************************************
 * bipsGetImagePropertiesObjectIndHandler: This signal is a part of an
 * operation where the client has requested to retrieve the
 * image-properties object from the BIP server. The image-properties
 * object describes in detail the various options, formats, and attributes
 * available for a single image.
 *****************************************************************************/
static void bipsGetImagePropertiesObjectIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode              responseCode;
    CsrUint8                         *propertiesObject;
    CsrUint16                           length;
    CsrBtBipsRcGetImagePropertiesObjectInd  *prim;

    prim = (CsrBtBipsRcGetImagePropertiesObjectInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        CsrUint16 index;

        index = bipsGlobalInstanceData->instData->sendIndex;

        if (bipsGlobalInstanceData->instData->sendObjectLength == 0)
        {
            /* This is the first BipsPushGetxCapabilitiesObjectInd message */
            bipsGlobalInstanceData->instData->sendObjectLength = sizeof(propertiesXmlString1);
        }

        if (bipsGlobalInstanceData->instData->sendObjectLength > prim->allowedImageLength)
        {
            /* The capabilitiesObject can not be sent in a single packet */
            bipsGlobalInstanceData->instData->sendObjectLength  -= prim->allowedImageLength;
            length                                  = prim->allowedImageLength;
            responseCode                            = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            bipsGlobalInstanceData->instData->sendIndex         += length;
        }
        else
        {
            /* The capabilitiesObject or the last part of it is now sent */
            length                                             = bipsGlobalInstanceData->instData->sendObjectLength;
            bipsGlobalInstanceData->instData->sendIndex         = 0;
            bipsGlobalInstanceData->instData->sendObjectLength  = 0;
            responseCode                            = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            printf("Properties transfer complete\n");

        }
        propertiesObject = (CsrUint8 *) CsrPmemAlloc(length);
        switch (bipsGlobalInstanceData->instData->reqHandle)
        {
        case 1:
            {
                CsrMemCpy(propertiesObject, &propertiesXmlString1[index], length);
                break;
            }
        case 2:
            {
                CsrMemCpy(propertiesObject, &propertiesXmlString2[index], length);
                break;
            }
        default:
            {
                CsrMemCpy(propertiesObject, &propertiesXmlString3[index], length);
                break;
            }
        }

    }
    else
    {
        responseCode     = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        propertiesObject = NULL;
        length           = 0;
    }
    CsrBtBipsRcGetImagePropertiesObjectResSend(bipsGlobalInstanceData->instData->CsrSchedQid, length, propertiesObject, responseCode, FALSE);
}


/******************************************************************************
 * bipsGetImageHeaderIndHandler: This Indication signal is the first part
 * of an operation where the client has requested to retrieve an image
 * object from the BIP server
 *****************************************************************************/
static void bipsGetImageHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsRcGetImageHeaderInd * prim;

    prim = (CsrBtBipsRcGetImageHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {


        if (!bipsGlobalInstanceData->instData->currentFileHandle)
        {
            char  *utfHandle;
            int        result;
            CsrBtStat_t statBuffer;
            CsrBool nativeImage = TRUE;

            bipsGlobalInstanceData->instData->reqHandle = 0;

            utfHandle = (char*) CsrUcs2ByteString2Utf8(prim->payload + prim->imageHandleOffset);
            if (CsrBtStrICmpLength(utfHandle, "0000001", 7))
            {
                bipsGlobalInstanceData->instData->reqHandle = 1;
            }
            if (CsrBtStrICmpLength(utfHandle, "0000002", 7))
            {
                bipsGlobalInstanceData->instData->reqHandle = 2;
            }
            if (CsrBtStrICmpLength(utfHandle, "0000003", 7))
            {
                bipsGlobalInstanceData->instData->reqHandle = 3;
            }
            CsrPmemFree(utfHandle);
            utfHandle = NULL;

            if( prim->descriptorLength )
            {
                char *pixelStr = NULL;
                pixelStr = interpretImageDescriptor((prim->payload + prim->descriptorOffset), prim->descriptorLength);

                if(pixelStr)
                {
                     if (!CsrBtStrICmpLength(pixelStr, "640*480", 7)  ||
                         !CsrBtStrICmpLength(pixelStr, "160*120", 7) )
                     {
                         nativeImage = FALSE;
                     }
                     printf("%10s\n",pixelStr);
                     CsrPmemFree(pixelStr);
                }
            }

            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
            statBuffer.st_size = 0;
            if(nativeImage)
            {
                switch (bipsGlobalInstanceData->instData->reqHandle)
                {
                    case 1:
                        {
                            CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, IMG_1, sizeof(IMG_1));
                            break;
                        }
                    case 2:
                        {
                            CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, IMG_2, sizeof(IMG_2));
                            break;
                        }
                    default:
                        {
                            CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, IMG_3, sizeof(IMG_3));
                            break;
                        }
                }

            }
            else
            {
                switch (bipsGlobalInstanceData->instData->reqHandle)
                {
                    case 1:
                        {
                            CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_1, sizeof(MON_IMG_1));
                            break;
                        }
                    case 2:
                        {
                            CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_2, sizeof(MON_IMG_2));
                            break;
                        }
                    default:
                        {
                            CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_3, sizeof(MON_IMG_3));
                            break;
                        }
                }

            }
            result = CsrBtStat(bipsGlobalInstanceData->instData->imgName, &statBuffer);
            if(!result)
            {
                if((bipsGlobalInstanceData->instData->currentFileHandle = CsrBtFopen(bipsGlobalInstanceData->instData->imgName, "rb")) == NULL)
                {
                    printf("Fail. DEMO could not open the file %s\n", bipsGlobalInstanceData->instData->imgName);
                    return;
                }
                else
                {
                    bipsGlobalInstanceData->instData->currentFileSize = statBuffer.st_size;
                }
            }
            else
            {
                printf("Fail. DEMO could not open the file %s\n", bipsGlobalInstanceData->instData->imgName);
                return;
            }
        }
        else
        {
            ;
        }

        CsrBtBipsRcGetImageHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, (CsrUint16) bipsGlobalInstanceData->instData->currentFileSize, FALSE);
    }
    else
    {
        CsrBtBipsRcGetImageHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, 0, FALSE);
    }

    if (prim->payload)
    {
        CsrPmemFree(prim->payload);
        prim->payload = NULL;
    }
}

/******************************************************************************
 * bipsGetImageObjectIndHandler: This signal is a part of an operation where
 * the client has requested to retrieve an image object from the BIP server.
 *****************************************************************************/
static void bipsGetImageObjectIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode   responseCode;
    CsrUint16                length;
    CsrUint8              *imgObject;
    CsrBtBipsRcGetImageObjectInd *prim;

    prim = (CsrBtBipsRcGetImageObjectInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {
        printf(".");
        if (bipsGlobalInstanceData->instData->currentFileSize > prim->allowedObjectLength)
        {
            /* The monitoring image object can not be sent in a single packet */
            bipsGlobalInstanceData->instData->currentFileSize -= prim->allowedObjectLength;
            length                                     = prim->allowedObjectLength;
            responseCode                               = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        else
        {
            /* The monitoring image object or the last part of it is now sent */
            /* The cast from CsrUint32 to CsrUint16 does no discard or corrupt any data
               since to OBEX package cannot be larger than CsrUint16 and this has already
               been checked for. */
            length                                               = (CsrUint16) bipsGlobalInstanceData->instData->currentFileSize;
            bipsGlobalInstanceData->instData->currentFilePosition = 0;
            bipsGlobalInstanceData->instData->currentFileSize     = 0;
            responseCode                              = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            printf("Image transfer complete\n");
        }
        imgObject = CsrPmemAlloc(length);
        bipsGlobalInstanceData->instData->currentFilePosition += CsrBtFread(imgObject, 1, length, bipsGlobalInstanceData->instData->currentFileHandle);

        if (bipsGlobalInstanceData->instData->currentFileSize <= 0)
        {
            CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle);
            bipsGlobalInstanceData->instData->currentFileHandle = NULL;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        imgObject    = NULL;
        length       = 0;
    }
    CsrBtBipsRcGetImageObjectResSend(bipsGlobalInstanceData->instData->CsrSchedQid, length, imgObject, responseCode, FALSE);
}


/******************************************************************************
 * bipsGetLinkedThumbnailHeaderIndHandler: This Indication signal is the first part
 * of an operation where the client has requested to retrieve a thumbnail
 * object from the BIP server
 *****************************************************************************/
static void bipsGetLinkedThumbnailHeaderIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderInd * prim;

    prim = (CsrBtBipsRcGetLinkedThumbnailHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {


        if (!bipsGlobalInstanceData->instData->currentFileHandle)
        {
            char  *utfHandle;
            int        result;
            CsrBtStat_t statBuffer;

            bipsGlobalInstanceData->instData->reqHandle = 0;

            utfHandle = (char*) CsrUcs2ByteString2Utf8(prim->payload + prim->imageHandleOffset);
            if (CsrBtStrICmpLength(utfHandle, "0000001", 7))
            {
                bipsGlobalInstanceData->instData->reqHandle = 1;
            }
            if (CsrBtStrICmpLength(utfHandle, "0000002", 7))
            {
                bipsGlobalInstanceData->instData->reqHandle = 2;
            }
            if (CsrBtStrICmpLength(utfHandle, "0000003", 7))
            {
                bipsGlobalInstanceData->instData->reqHandle = 3;
            }
            CsrPmemFree(utfHandle);
            utfHandle = 0;

            CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
            statBuffer.st_size = 0;
            switch (bipsGlobalInstanceData->instData->reqHandle)
            {
            case 1:
                {
                    CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_1, sizeof(MON_IMG_1));
                    break;
                }
            case 2:
                {
                    CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_2, sizeof(MON_IMG_2));
                    break;
                }
            default:
                {
                    CsrStrNCpy(bipsGlobalInstanceData->instData->imgName, MON_IMG_3, sizeof(MON_IMG_3));
                    break;
                }
            }

            result = CsrBtStat(bipsGlobalInstanceData->instData->imgName, &statBuffer);
            if(!result)
            {
                if((bipsGlobalInstanceData->instData->currentFileHandle = CsrBtFopen(bipsGlobalInstanceData->instData->imgName, "rb")) == NULL)
                {
                    printf("Fail. DEMO could not open the file %s\n", bipsGlobalInstanceData->instData->imgName);
                    return;
                }
                else
                {
                    bipsGlobalInstanceData->instData->currentFileSize = statBuffer.st_size;
                }
            }
            else
            {
                printf("Fail. DEMO could not open the file %s\n", bipsGlobalInstanceData->instData->imgName);
                return;
            }
        }
        else
        {
            ;
        }

        CsrBtBipsRcGetLinkedThumbnailHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
    }
    else
    {
        CsrBtBipsRcGetLinkedThumbnailHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE, FALSE);
    }

    if (prim->payload != NULL)
    {
        CsrPmemFree(prim->payload);
        prim->payload = NULL;
    }
}

/******************************************************************************
 * bipsGetLinkedThumbnailObjectIndHandler: This signal is a part of an operation where
 * the client has requested to retrieve a thumbnail object from the BIP server.
 *****************************************************************************/
static void bipsGetLinkedThumbnailObjectIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtObexResponseCode             responseCode;
    CsrUint16                          length;
    CsrUint8                        *imgObject;
    CsrBtBipsRcGetLinkedThumbnailObjectInd *prim;

    prim = (CsrBtBipsRcGetLinkedThumbnailObjectInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    if (prim->connectionId == bipsGlobalInstanceData->instData->connectionId)
    {

        if (bipsGlobalInstanceData->instData->currentFileSize > prim->allowedObjectLength)
        {
            /* The monitoring image object can not be sent in a single packet */
            bipsGlobalInstanceData->instData->currentFileSize -= prim->allowedObjectLength;
            length                                     = prim->allowedObjectLength;
            responseCode                               = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        else
        {
            /* The monitoring image object or the last part of it is now sent */
            /* The cast from CsrUint32 to CsrUint16 does no discard or corrupt any data
               since to OBEX package cannot be larger than CsrUint16 and this has already
               been checked for. */
            length                                               = (CsrUint16) bipsGlobalInstanceData->instData->currentFileSize;
            bipsGlobalInstanceData->instData->currentFilePosition = 0;
            bipsGlobalInstanceData->instData->currentFileSize     = 0;
            responseCode                              = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            printf("Transfer complete\n");
        }
        imgObject = CsrPmemAlloc(length);
        bipsGlobalInstanceData->instData->currentFilePosition += CsrBtFread(imgObject, 1, length, bipsGlobalInstanceData->instData->currentFileHandle);

        if (bipsGlobalInstanceData->instData->currentFileSize <= 0)
        {
            CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle);
            bipsGlobalInstanceData->instData->currentFileHandle = NULL;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_SERVICE_UNAVAILABLE_RESPONSE_CODE;
        imgObject    = NULL;
        length       = 0;
    }
    CsrBtBipsRcGetLinkedThumbnailObjectResSend(bipsGlobalInstanceData->instData->CsrSchedQid, length, imgObject, responseCode, FALSE);
}

/******************************************************************************
 * bipsGetImageLiseIndHandler: This signal is used when a secondary client
 * is requesting the image list
 *****************************************************************************/
static void  bipsGetImageListIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetImageListInd *prim = (CsrBtBipsAaGetImageListInd *) bipsGlobalInstanceData->recvMsgP;
    AaInstanceDataType *aa = bipsGlobalInstanceData->instData->aa;

    if(aa->firstPart)
    {

        aa->firstPart = FALSE;

        aa->imageListObject = (CsrUint8 *)
            CsrPmemAlloc(prim->imageListingObjectLength);

        aa->imageListObjectLength =
            prim->imageListingObjectLength;

        CsrMemCpy(aa->imageListObject,
               prim->payload + prim->imageListingObjectOffset,
               prim->imageListingObjectLength);
    }
    else
    {
        CsrUint8 *tmp;
        CsrUint16 oldLength;

        tmp = aa->imageListObject;
        oldLength = aa->imageListObjectLength;

        aa->imageListObjectLength +=
            prim->imageListingObjectLength;
        aa->imageListObject =
        CsrPmemAlloc(aa->imageListObjectLength);

        CsrMemCpy(aa->imageListObject,tmp, oldLength);
        CsrMemCpy(aa->imageListObject,
               (prim->payload + prim->imageListingObjectOffset) ,
               prim->imageListingObjectLength);

        CsrPmemFree(tmp);


    }


    CsrPmemFree(prim->payload);
    CsrBtBipsAaGetImageListResSend(bipsGlobalInstanceData->instData->CsrSchedQid, FALSE);

}
/******************************************************************************
 * bipsGetImageListHeaderIndHandler: This signal is used when a secondary client
 * is requesting the image list
 *****************************************************************************/
static void bipsGetImageListHeaderIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetImageListHeaderInd *prim = (CsrBtBipsAaGetImageListHeaderInd *) bipsGlobalInstanceData->recvMsgP;

    bipsGlobalInstanceData->instData->aa->nofReturnedHandles
        = prim->nbReturnedHandles;

    if(prim->imageHandlesDescriptorLength)
    {
        bipsGlobalInstanceData->instData->aa->usedFilter
            = prim->imageHandlesDescriptor;
    }
    else
    {
        bipsGlobalInstanceData->instData->aa->usedFilter = NULL;
    }
    CsrBtBipsAaGetImageListHeaderResSend(bipsGlobalInstanceData->instData->CsrSchedQid, FALSE);

}
/******************************************************************************
 * bipsGetImageListCfmHandler: This signal is used when a secondary client
 * is requesting the image list
 *****************************************************************************/
static void bipsGetImageListCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetImageListCfm *prim = (CsrBtBipsAaGetImageListCfm *) bipsGlobalInstanceData->recvMsgP;
    AaInstanceDataType *aa = bipsGlobalInstanceData->instData->aa;
    CsrUint8 *bufferCopy;
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        if(aa->firstPart)
        {


            aa->imageListObject = (CsrUint8 *)
                CsrPmemAlloc(prim->imageListingObjectLength);

            aa->imageListObjectLength =
                prim->imageListingObjectLength;

            CsrMemCpy(aa->imageListObject,
                   prim->payload + prim->imageListingObjectOffset,
                   prim->imageListingObjectLength);
        }
        else
        {
            CsrUint8 *tmp;
            CsrUint16 oldLength;

            tmp = aa->imageListObject;
            oldLength = aa->imageListObjectLength;

            aa->imageListObjectLength +=
                prim->imageListingObjectLength;
            aa->imageListObject =
                CsrPmemAlloc(aa->imageListObjectLength);

            CsrMemCpy(aa->imageListObject,tmp, oldLength);
            CsrMemCpy(aa->imageListObject,
                   (prim->payload + prim->imageListingObjectOffset) ,
                   prim->imageListingObjectLength);
            CsrPmemFree(tmp);

        }
        aa->firstPart = TRUE;

        aa->imageListExist = TRUE;


        bufferCopy = CsrPmemAlloc(aa->imageListObjectLength);
        CsrMemCpy(bufferCopy, aa->imageListObject, aa->imageListObjectLength);
        interpretImageHandlesDescriptor(bipsGlobalInstanceData,
                                        bufferCopy, aa->imageListObjectLength);

        CsrPmemFree(bufferCopy);

    }
    else
    {
        printf("\nCsrBtBipsAaGetImageListCfm fail with responseCode %04Xh\n",
               prim->responseCode);

    }
    bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
    showMenu(bipsGlobalInstanceData);
    CsrPmemFree(prim->payload);
}

/******************************************************************************
 * bipsGetCapabilitiesIndHandler: This signal is used when a secondary client
 * is requesting the capabilities
 *****************************************************************************/

static void
bipsGetCapabilitiesIndHandler(BipsGlobalInstanceDataType *bipsGlobalInstanceData)
{
    CsrUint32  i;
    CsrBtBipsAaGetCapabilitiesInd  *prim;

    prim = (CsrBtBipsAaGetCapabilitiesInd *) bipsGlobalInstanceData->recvMsgP;
    CsrBtBipsAaGetCapabilitiesResSend(CSR_BT_BIPS_IFACEQUEUE, FALSE);

    for (i = 0; i < prim->capabilitiesObjectLength; i++)
    {
        printf("%c", prim->payload[prim->capabilitiesObjectOffset + i]);
    }
    CsrPmemFree(prim->payload);
}


/******************************************************************************
 * bipsGetCapabilitiesCfmHandler: This signal is used when a secondary client
 * is requesting the capabilities
 *****************************************************************************/
static void
bipsGetCapabilitiesCfmHandler(BipsGlobalInstanceDataType *bipsGlobalInstanceData)
{
    CsrBtBipsAaGetCapabilitiesCfm *prim;

    prim = (CsrBtBipsAaGetCapabilitiesCfm *) bipsGlobalInstanceData->recvMsgP;

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
        printf("\nCsrBtBipsAaGetCapabilitiesCfm fail with responseCode %04Xh\n",
               prim->responseCode);
    }

    bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
    showMenu(bipsGlobalInstanceData);
    CsrPmemFree(prim->payload);

}
/******************************************************************************
 * bipsGetImagePropertiesIndHandler: This signal is used when a secondary client
 * is requesting the image properties
 *****************************************************************************/
static void bipsGetImagePropertiesIndhandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetImagePropertiesInd *prim;
    AaInstanceDataType *aa = bipsGlobalInstanceData->instData->aa;
    prim = (CsrBtBipsAaGetImagePropertiesInd *) bipsGlobalInstanceData->recvMsgP;

    if(aa->firstPart)
    {
        aa->firstPart = FALSE;
        showMenu(bipsGlobalInstanceData);
        printf("\n");
    }


    if(aa->imagePropertiesLength)
    {
        CsrUint8 *tmp;
        CsrUint16 oldLength;

        tmp = aa->imageProperties;
        oldLength = aa->imagePropertiesLength;

        aa->imagePropertiesLength +=
            prim->propertiesObjectLength;
        aa->imageProperties =
        CsrPmemAlloc(aa->imagePropertiesLength);

        CsrMemCpy(aa->imageProperties,tmp, oldLength);
        CsrMemCpy(aa->imageProperties,
               (prim->payload + prim->propertiesObjectOffset) ,
               prim->propertiesObjectLength);

        CsrPmemFree(tmp);
    }
    else
    {
        aa->imagePropertiesLength =
            prim->propertiesObjectLength;
        aa->imageProperties =
            (CsrUint8 *)CsrPmemAlloc(aa->imagePropertiesLength);
        CsrMemCpy(aa->imageProperties,
               prim->payload + prim->propertiesObjectOffset,
               aa->imagePropertiesLength);
    }

    CsrPmemFree(prim->payload);
    CsrBtBipsAaGetImagePropertiesExtResSend(bipsGlobalInstanceData->instData->CsrSchedQid, FALSE);
}
/******************************************************************************
 * bipsGetImagePropertiesCfmHandler: This signal is used when a secondary client
 * is requesting the image properties
 *****************************************************************************/
static void bipsGetImagePropertiesCfmhandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetImagePropertiesCfm *prim;
    AaInstanceDataType *aa = bipsGlobalInstanceData->instData->aa;
    prim = (CsrBtBipsAaGetImagePropertiesCfm *) bipsGlobalInstanceData->recvMsgP;

    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {

        if(aa->firstPart)
        {
            bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
            showMenu(bipsGlobalInstanceData);
            printf("\n");
        }

        if(aa->imagePropertiesLength)
        {
            CsrUint8 *tmp;
            CsrUint16 oldLength;

            tmp = aa->imageProperties;
            oldLength = aa->imagePropertiesLength;

            aa->imagePropertiesLength +=
                prim->propertiesObjectLength;
            aa->imageProperties =
                CsrPmemAlloc(aa->imagePropertiesLength);

            CsrMemCpy(aa->imageProperties,
                   tmp, oldLength);
            CsrMemCpy(aa->imageProperties,
                   (prim->payload + prim->propertiesObjectOffset) ,
                   prim->propertiesObjectLength);

            CsrPmemFree(tmp);
        }
        else
        {
            aa->imagePropertiesLength =
                prim->propertiesObjectLength;
            aa->imageProperties =
                (CsrUint8 *)CsrPmemAlloc(aa->imagePropertiesLength);
            CsrMemCpy(aa->imageProperties,
                   prim->payload + prim->propertiesObjectOffset,
                   aa->imagePropertiesLength);
        }


        aa->firstPart = TRUE;
        printf("\nImageProperties Received\n");
        printf("%*.*s\n", aa->imagePropertiesLength,
               aa->imagePropertiesLength,
               aa->imageProperties);

    }
    else
    {
        printf("\nCsrBtBipsAaGetImagePropertiesCfm fail with responseCode %04Xh\n",
               prim->responseCode);
    }

    CsrPmemFree(prim->payload);
    aa->firstPart = TRUE;
}
/******************************************************************************
 * bipsGetImageIndHandler: This signal is used when a secondary client
 * is requesting an image
 *****************************************************************************/
static void bipsGetImageIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetImageInd *prim;
    prim = (CsrBtBipsAaGetImageInd *) bipsGlobalInstanceData->recvMsgP;
    printf(".");
    /* save image to file system */
    CsrBtFwrite((prim->payload + prim->imageObjectOffset) ,
              1,
              prim->imageObjectLength,
              bipsGlobalInstanceData->instData->currentFileHandle);

    CsrPmemFree(prim->payload);
    CsrBtBipsAaGetImageResSend(bipsGlobalInstanceData->instData->CsrSchedQid, FALSE);

}
/******************************************************************************
 * bipsGetImageCfmHandler: This signal is used when a secondary client
 * is requesting an image
 *****************************************************************************/
static void bipsGetImageCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetImageCfm *prim;
    prim = (CsrBtBipsAaGetImageCfm *) bipsGlobalInstanceData->recvMsgP;

    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        CsrBtFwrite((prim->payload + prim->imageObjectOffset) ,
                  1,
                  prim->imageObjectLength,
                  bipsGlobalInstanceData->instData->currentFileHandle);

        if(CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle) == EOF)
        {
            printf("Image could not be stored on local device \n");
        }
        else
        {
            /* display image */
#ifndef __linux__
            system("start aa_image.jpg");
#endif
        }

        bipsGlobalInstanceData->instData->currentFileHandle = NULL;
    }
    else
    {
        printf("\nCsrBtBipsAaGetImageCfm fail with responseCode %04Xh\n",
               prim->responseCode);
    }
    bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
    showMenu(bipsGlobalInstanceData);
    CsrPmemFree(prim->payload);

}
/******************************************************************************
 * bipsGetLinkedAttachmentIndHandler: This signal is used when a secondary client
 * is requesting a linked attachment
 *****************************************************************************/
static void bipsGetLinkedAttachmentIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetLinkedAttachmentInd *prim;
    prim = (CsrBtBipsAaGetLinkedAttachmentInd *) bipsGlobalInstanceData->recvMsgP;

    printf("*");
    /* save the attachment to file system */
    CsrBtFwrite((prim->payload + prim->attachmentFileOffset) ,
              1,
              prim->attachmentFileLength,
              bipsGlobalInstanceData->instData->currentFileHandle);

    CsrPmemFree(prim->payload);
    CsrBtBipsAaGetLinkedAttachmentResSend(bipsGlobalInstanceData->instData->CsrSchedQid, FALSE);

}
/******************************************************************************
 * bipsGetLinkedAttachmentCfmHandler: This signal is used when a secondary client
 * is requesting a linked attachment
 *****************************************************************************/
static void bipsGetLinkedAttachmentCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetLinkedAttachmentCfm *prim;
    prim = (CsrBtBipsAaGetLinkedAttachmentCfm *) bipsGlobalInstanceData->recvMsgP;

    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        CsrBtFwrite((prim->payload + prim->attachmentFileOffset) ,
                  1,
                  prim->attachmentFileLength,
                  bipsGlobalInstanceData->instData->currentFileHandle);

        if(CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle) == EOF)
        {
            printf("Attachment could not be stored on local device \n");
        }
        else
        {
            /* display image */
#ifndef __linux__
            if(bipsGlobalInstanceData->instData->aa->attachTypeWav)
            {
                system("start aa_attach.wav");
            }
            else
            {
                system("start aa_attach.txt");
            }
            bipsGlobalInstanceData->instData->aa->attachTypeWav = TRUE;
#endif
        }

    }
    else
    {
        printf("\nCsrBtBipsAaGetLinkedAttachmentCfm fail with responseCode %04Xh\n",
               prim->responseCode);
    }
    bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
    showMenu(bipsGlobalInstanceData);
    CsrPmemFree(prim->payload);

}
/******************************************************************************
 * bipsGetLinkedThumbnailIndHandler: This signal is used when a secondary client
 * is requesting  a linked thumbnail
 *****************************************************************************/
static void bipsGetLinkedThumbnailIndHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetLinkedThumbnailInd *prim;
    prim = (CsrBtBipsAaGetLinkedThumbnailInd *) bipsGlobalInstanceData->recvMsgP;

    printf("*");
    /* save image to file system */
    CsrBtFwrite((prim->payload + prim->thumbnailObjectOffset) ,
              1,
              prim->thumbnailObjectLength,
              bipsGlobalInstanceData->instData->currentFileHandle);

    CsrPmemFree(prim->payload);
    CsrBtBipsAaGetLinkedThumbnailResSend(bipsGlobalInstanceData->instData->CsrSchedQid, FALSE);

}
/******************************************************************************
 * bipsLinkedThumbnailCfmHandler: This signal is used when a secondary client
 * is requesting a linked thumbnail
 *****************************************************************************/
static void bipsGetLinkedThumbnailCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaGetLinkedThumbnailCfm *prim;
    prim = (CsrBtBipsAaGetLinkedThumbnailCfm *) bipsGlobalInstanceData->recvMsgP;
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        CsrBtFwrite((prim->payload + prim->thumbnailObjectOffset) ,
                  1,
                  prim->thumbnailObjectLength,
                  bipsGlobalInstanceData->instData->currentFileHandle);

        if(CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle) == EOF)
        {
            printf("Thumbnail could not be stored on local device \n");
        }
        else
        {
            /* display image */
#ifndef __linux__
            system("start aa_thumb.jpg");
#endif
        }

    }
    else
    {
        printf("\nCsrBtBipsAaGetLinkedThumbnailCfm fail with responseCode %04Xh\n",
               prim->responseCode);
    }
    bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
    showMenu(bipsGlobalInstanceData);
    CsrPmemFree(prim->payload);

}
/******************************************************************************
 * bipsDeleteImageCfmHandler: This signal is used when a secondary client
 * is requesting to delete an image
 *****************************************************************************/
static void bipsDeleteImageCfmHandler( BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAaDeleteImageCfm *prim;
    prim = (CsrBtBipsAaDeleteImageCfm *) bipsGlobalInstanceData->recvMsgP;

    bipsGlobalInstanceData->instData->menuState = MENU_AA_STATE;
    showMenu(bipsGlobalInstanceData);

    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        printf("\nDelete image succeeded\n");
    }
    else
    {
        printf("\nDelete image failed with responseCode %04Xh\n",
               prim->responseCode);
    }

}
/******************************************************************************
 * bipsAbortCfmHandler: This signal is used when a secondary client
 * is requesting to abort the current OBEX process
 *****************************************************************************/
static void bipsAbortCfmHandler( BipsGlobalInstanceDataType * pInstData)
{
    if (pInstData->instData->currentFileHandle)
    {
        CsrBtFclose(pInstData->instData->currentFileHandle);
    }
    pInstData->instData->currentFilePosition  = 0;
    pInstData->instData->currentFileHandle    = NULL;
    pInstData->instData->currentFileSize      = 0;

    /*system(CLS);*/
    pInstData->instData->menuState = MENU_AA_STATE;
    showMenu(pInstData);
}
/******************************************************************************
 * bipsAbortIndHandler: This signal is used when a client decides to
 * terminate a multi-packet operation before it normally ends
 *****************************************************************************/
static void bipsAbortIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    CsrBtBipsAbortInd    * prim;

    prim    = (CsrBtBipsAbortInd *) bipsGlobalInstanceData->recvMsgP;

    setActiveInstance( bipsGlobalInstanceData, prim->pHandleInst );

    bipsGlobalInstanceData->instData->sendIndex = 0;
    bipsGlobalInstanceData->instData->sendObjectLength  = 0;

    if (bipsGlobalInstanceData->instData->writingFileHandle)
    {
        CsrBtFclose(bipsGlobalInstanceData->instData->writingFileHandle);
        bipsGlobalInstanceData->instData->writingFileHandle = NULL;
    }

    if(bipsGlobalInstanceData->instData->currentFileHandle)
    {
        CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle);
        bipsGlobalInstanceData->instData->currentFileHandle = NULL;
    }

    bipsGlobalInstanceData->instData->currentFileSize = 0;
    bipsGlobalInstanceData->instData->currentFilePosition = 0;

    CsrPmemFree(bipsGlobalInstanceData->instData->imageFileName);
    bipsGlobalInstanceData->instData->imageFileName = NULL;
    CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
    bipsGlobalInstanceData->instData->thumbnailFileName = NULL;
}



/******************************************************************************
 * bipsDisconnectIndHandler: This signal is indicating that the OBEX image
 * push session is finished, and is ready for a new one.
 *****************************************************************************/
static void bipsDisconnectIndHandler(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    bipsGlobalInstanceData->instData->sendIndex = 0;
    bipsGlobalInstanceData->instData->sendObjectLength  = 0;
    if (bipsGlobalInstanceData->instData->writingFileHandle)
    {
        CsrBtFclose(bipsGlobalInstanceData->instData->writingFileHandle);
        bipsGlobalInstanceData->instData->writingFileHandle = NULL;
    }
    CsrPmemFree(bipsGlobalInstanceData->instData->imageFileName);
    bipsGlobalInstanceData->instData->imageFileName = NULL;
    CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
    bipsGlobalInstanceData->instData->thumbnailFileName = NULL;
    /*system(CLS);*/
    bipsGlobalInstanceData->instData->menuState  = MENU_IDLE_STATE;
    bipsGlobalInstanceData->instData->aa->imageListExist = FALSE;
    showMenu(bipsGlobalInstanceData);
}


static void updateTotalImagingDataCapacity(BipsGlobalInstanceDataType * bipsGlobalInstanceData)
{
    if (bipsGlobalInstanceData->instData->currentSize <= bipsGlobalInstanceData->lowerDataCapacity)
    {
        bipsGlobalInstanceData->lowerDataCapacity -= bipsGlobalInstanceData->instData->currentSize;
    }
    else if (bipsGlobalInstanceData->upperDataCapacity > 0 )
    {
        CsrUint32 temp;

        temp = 0xFFFFFFFF - bipsGlobalInstanceData->instData->currentSize + bipsGlobalInstanceData->lowerDataCapacity + 1;

        bipsGlobalInstanceData->lowerDataCapacity = temp;
        bipsGlobalInstanceData->upperDataCapacity--;
    }
    bipsGlobalInstanceData->instData->currentSize = 0;
}

static char * interpretImageDescriptor(CsrUint8 *imageDescriptor, CsrUint16 imageDescriptorLength)
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
            printf("The image the client wants the server to provide for it,\nis as follows:\n\n");
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

static void interpretAttachmentDescriptor(BipsGlobalInstanceDataType * bipsGlobalInstanceData, CsrUint8 *attachmentDescriptor, CsrUint16 attachmentDescriptorLength)
{
    CsrBtElementList                * xmlTree;

    xmlTree = CsrBtBuildXmlTree(attachmentDescriptor, attachmentDescriptorLength);

    if (xmlTree)
    {
        CsrBtRootElementType        * rootElement;

        rootElement    = &(xmlTree->elementArray[0].rootElement);

        if (!CsrBtStrICmp((char *)rootElement->name, AttachmentDescriptor))
        {
            printf("\n\nThe properties of the attachment is as follows:\n");
            if (!CsrBtStrICmp((char *) rootElement->firstAttribute->name, Version))
            {
                CsrBtChildElementType    * childElement;

                printf("%-25.25s: ",rootElement->firstAttribute->name);
                printf("%s\n",rootElement->firstAttribute->value);

                childElement = rootElement->firstChildElement;

                while (childElement)
                {
                    if (!CsrBtStrICmp((char *) childElement->name, Attachment))
                    {
                        CsrBtElementAttributeType * attribute;

                        attribute = childElement->firstAttribute;

                        while (attribute)
                        {
                            if (!CsrBtStrICmp((char *)attribute->name, ContentType)    ||
                                !CsrBtStrICmp((char *)attribute->name, Charset)        ||
                                !CsrBtStrICmp((char *)attribute->name, Name)            ||
                                !CsrBtStrICmp((char *)attribute->name, Size)            ||
                                !CsrBtStrICmp((char *)attribute->name, Created)
                                )
                            {
                                printf("%-25.25s: ",  attribute->name);
                                printf("%s\n", attribute->value);
                                if (!CsrBtStrICmp((char *)attribute->name, Name))
                                {
                                    bipsGlobalInstanceData->instData->attachmentFileName = (CsrUint8 *) CsrPmemAlloc(CsrStrLen(attribute->value) + 1);
                                    CsrStrCpy(bipsGlobalInstanceData->instData->attachmentFileName, attribute->value);
                                }
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
        printf("The attachment descriptor is not valid\n");
    }
}
static void interpretImageHandlesDescriptor(BipsGlobalInstanceDataType * bipsGlobalInstanceData,
                                            CsrUint8 *imageHandlesDescriptor,
                                            CsrUint16 imageHandlesDescriptorLength)
{
    CsrBtElementList                * xmlTree;
    bipsGlobalInstanceData->instData->aa->imageHandleIndex = 0;
    bipsGlobalInstanceData->instData->aa->imageHandleIndexMax = 0;
    xmlTree = CsrBtBuildXmlTree(imageHandlesDescriptor, imageHandlesDescriptorLength);
    if (xmlTree)
    {
        CsrBtRootElementType        * rootElement;

        rootElement    = &(xmlTree->elementArray[0].rootElement);

        if (!CsrBtStrICmp((char *)rootElement->name, ImageListing))
        {
            if (!CsrBtStrICmp((char *) rootElement->firstAttribute->name, Version))
            {
                CsrBtChildElementType    * childElement;

                childElement = rootElement->firstChildElement;

                while (childElement)
                {
                    if (!CsrBtStrICmp((char *) childElement->name, Image))
                    {
                        CsrBtElementAttributeType * attribute;

                        attribute = childElement->firstAttribute;

                        while (attribute)
                        {
                            if (!CsrBtStrICmp((char *)attribute->name, Handle)    ||
                                !CsrBtStrICmp((char *)attribute->name, Created)
                                )
                            {
                                if (!CsrBtStrICmp((char *)attribute->name, Handle))
                                {
                                    if( bipsGlobalInstanceData->instData->aa->imageHandleIndexMax < 50 )
                                    {
                                        CsrStrCpy(bipsGlobalInstanceData->instData->aa->imageHandles
                                               [bipsGlobalInstanceData->instData->aa->imageHandleIndexMax],
                                               attribute->value);
                                        bipsGlobalInstanceData->instData->aa->imageHandles
                                            [bipsGlobalInstanceData->instData->aa->imageHandleIndexMax++][7] = '\0';
                                    }
                                }
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
        printf("The image list is not valid\n");
    }

}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16  msg_type=0;
    void    *msg_data=NULL;

    BipsGlobalInstanceDataType * bipsGlobalInstanceData;

    bipsGlobalInstanceData = (BipsGlobalInstanceDataType*) (*gash);

    scAppDeinitInstanceData(&bipsGlobalInstanceData->scInstData);

    while ( CsrSchedMessageGet(&msg_type, &msg_data) )
    {
        switch (msg_type)
        {
            case CSR_BT_BIPS_PRIM:
            {
                CsrBtBipsFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            case KEY_MESSAGE:
            {
                break;
            }
        }
        CsrPmemFree(msg_data);
    }

    if (bipsGlobalInstanceData->instData->writingFileHandle)
    {
        CsrBtFclose(bipsGlobalInstanceData->instData->writingFileHandle);
    }
    CsrPmemFree(bipsGlobalInstanceData->instData->imageFileName);
    CsrPmemFree(bipsGlobalInstanceData->instData->thumbnailFileName);
    CsrPmemFree(bipsGlobalInstanceData->recvMsgP);
    CsrPmemFree(bipsGlobalInstanceData->instData->attachmentFileName);
    if (bipsGlobalInstanceData->instData->currentFileHandle != NULL)
    {
        CsrBtFclose(bipsGlobalInstanceData->instData->currentFileHandle);
    }
    CsrPmemFree(bipsGlobalInstanceData);
}

#endif

static char *copyString(char *s)
{
    int i;
    char *r;

    i = CsrStrLen(s) + 1;
    r = CsrPmemAlloc(i);
    CsrStrCpy(r, s);
    r[i-1] = '\0';

    return r;
}

static void printImageList(AaInstanceDataType *pInst)
{
    if(pInst->imageListExist)
    {
        CsrUint16 i;
        printf("\n\nNumber of returned handles: %d\n",
               pInst->nofReturnedHandles);
        if(pInst->usedFilter)
        {
            printf("Used Filter: %s\n",
                   pInst->usedFilter);
        }
        else
        {
            printf("No filter used \n");
        }
        printf("Image List: \n");
        for(i = 0; i < pInst->imageListObjectLength ; i++)
        {
            printf("%c", pInst->imageListObject[i]);
        }
        printf("\n\n");
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
        *imgDescLength    = CsrBtReturnImgDescriptionLength(encoding, pixel, size, "", "");
        imgDesc          = CsrBtBuildImgDescriptorHeader(encoding, pixel, size, "", "", *imgDescLength);
        CsrBtFreeXmlTree(xmlTree);
    }
    else
    {
        printf("The image descriptor is not valid\n");
        imgDesc = NULL;
        *imgDescLength = 0;
    }
    return imgDesc;
}

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
