/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_obex.h"
#include "csr_bt_bppc_prim.h"
#include "csr_bt_bppc_lib.h"
#include "csr_bt_xml_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"

#include "csr_unicode.h"
#include "csr_sched_init.h"

#define XML_INTERPRET                            (1)

#define KEYB_CHECK_TIMEOUT                       (100000)
#define DEMO_NUMBER_OF_STATES                    (1)

#define KEYBOARD_MENU_STATE                      (0)
#define KEYBOARD_SELECT_ADDRESS_STATE            (1)
#define KEYBOARD_BONDING_STATE                   (2)
#define KEYBOARD_AUTHENTICATE_STATE              (3)
#define KEYBOARD_SELECT_MEDIA_SIZE_STATE         (4)
#define KEYBOARD_SELECT_MEDIA_TYPE_STATE         (5)
#define KEYBOARD_SELECT_DOCUMENT_FORMAT_STATE    (6)
#define KEYBOARD_SELECT_ORIENTATION_STATE        (7)
#define KEYBOARD_SELECT_MAX_COPIES_STATE         (8)
#define KEYBOARD_SELECT_NUMBER_UP_STATE          (9)
#define KEYBOARD_SELECT_SIDE_STATE               (10)
#define KEYBOARD_SELECT_PRINT_QUALITY_STATE      (11)
#define KEYBOARD_TYPE_DOCUMENT_FILE_STATE        (12)
#define KEYBOARD_TYPE_MIME_TYPE_MEDIA_STATE      (13)
#define KEYBOARD_SELECT_MIME_TYPE_MEDIA_STATE    (14)
#define KEYBOARD_SELECT_ATTRIBUTE_STATE          (15)
#define KEYBOARD_SELECT_VCARDS_PER_PAGE_STATE    (16)
#define KEYBOARD_SELECT_VCAL_OBJ_PER_PAGE_STATE  (17)

#define MENU_IDLE_STATE                          (0)
#define MENU_SEARCH_STATE                        (1)
#define MENU_CONNECTED_STATE                     (2)
#define MENU_JOB_CREATED_STATE                   (3)
#define MENU_SELECT_TRANSFER_MODEL               (4)
#define MENU_JOB_CREATE_CONFIG_STATE             (5)
#define MENU_SELECT_ATTRIBUTE_STATE              (6)

#define MAX_PACKET_SIZE_TO_RETRIEVE              (8000)

#define XHTML95_MASK                             (1)
#define TEXT_MASK                                (2)
#define VCARD21_MASK                             (4)
#define VCAL_MASK                                (8)
#define ICAL_MASK                                (16)
#define POSTS2_MASK                              (32)
#define POSTS3_MASK                              (64)
#define PCL5E_MASK                               (128)
#define PCL6_MASK                                (256)
#define PDF_MASK                                 (512)
#define JPEG_MASK                                (1024)
#define GIF_MASK                                 (2048)
#define XHTML10_MASK                             (4096)
#define VCARD30_MASK                             (8192)
#define VMESSAGE11_MASK                          (16384)

#define ONE_SIDED_MASK                           (1)
#define TWO_SIDED_LONG_MASK                      (2)
#define TWO_SIDED_SHORT_MASK                     (4)

#define DRAFT_MASK                               (1)
#define NORMAL_MASK                              (2)
#define HIGH_MASK                                (4)

#define PORTRAIT_MASK                            (1)
#define LANDSCAPE_MASK                           (2)
#define REVERSE_LANDSCAPE_MASK                   (4)
#define REVERSE_PORTRAIT_MASK                    (8)
#define MAX_NUM_OF_DOC_FORMATS                   (5)

typedef struct
{
    CsrSchedQid                       CsrSchedQid;
    void                            *recvMsgP;
    CsrUint8                         *recvXmlPtr;
    CsrFileHandle                            *currentFileHandle;
    CsrBtElementList                     *xmlTree;
    CsrUint8                         *xhtmlPrintImageFormats;
    CsrUint8                         *documentFormatsSupported;
    CsrUint8                         *mediaTypesSupported;
    CsrBool                          cancelConnect;
    CsrBool                          transferModel;
    CsrBool                          unsupportedFeature;
    CsrUint8                         keyboardState;
    CsrUint8                         prevKeyboardState;
    CsrUint8                         menuState;
    CsrUint8                         sides;
    CsrUint8                         printfQuality;
    CsrUint8                         orientation;
    CsrUint8                         addressLength;
    CsrUint8                         authenticateLength;
    CsrUint8                         miscLength;
    CsrUint8                         numberUp;
    CsrUint8                         maxCopies;
    CsrUint8                         numOfDiffMediaTypes;
    CsrUint8                         numOfDiffMediaSizes;
    CsrUint16                        numOfDiffDocumentFormats;
    CsrUint16                        documentFormat;
    CsrUint32                        jobId;
    CsrUint32                        recvXmlSize;
    CsrUint32                        currentFilePosition;
    CsrUint32                        currentFileSize;
    char                            mimeMediaType[100];
    char                            documentName[100];
    char                            docTypeDependentInfo[100];
    char                            referenceFileName[100];
    CsrBtDeviceAddr                    deviceAddr;
    CsrBtDeviceAddr                    bondingAddr;
    CsrFileHandle                            *currentRefFileHandle;
    CsrUint8                         *recvStatusXmlPtr;
    CsrUint16                        recvStatusXmlSize;
    CsrBtElementList                     *xmlStatusTree;
    CsrUint32                        currentRefFileSize;
    CsrUint32                        currentRefFilePosition;
    CsrUint32                        refCount;
    CsrUint32                        refOffset;
    CsrBool                          getEventActive;
    CsrSchedTid                             getEventTimeOut;
    CsrBool                          abortDisconnect;
    CsrUint8                         attributeCount;
    CsrBool                          attributesToRetrieve[19];
    CsrBool                          defaultCreateJobParam;
    CsrUint8                         character;
    CsrUint8                         selectedFormat[MAX_NUM_OF_DOC_FORMATS + 1];
    char                            address[12];
    char                            authenticatePassword[20];
    CsrUint8                         passkeyLength;
    CsrUint8                         passkey[CSR_BT_PASSKEY_MAX_LEN];
    ScAppInstData_t                 scInstData;
    CsrBool                         activated;
} BppcDemoInstanceDataType;

typedef void (* BppcDemoStateHandlerType)(BppcDemoInstanceDataType * bppcDemoInstanceData);

#define PRINTER_NAME                  "PrinterName"
#define PRINTER_LOCATION              "PrinterLocation"
#define PRINTER_STATE                 "PrinterState"
#define PRINTER_STATE_REASONS         "PrinterStateReasons"
#define PRINTER_OPERATOR              "PrinterGeneralCurrentOperator"

#define QUEUED_JOB_COUNT              "QueuedJobCount"
#define OPERATION_STATUS              "OperationStatus"
#define TEXT_PAGE_WIDTH               "BasicTextPageWidth"
#define TEXT_PAGE_HEIGHT              "BasicTextPageHeight"

#define COLOR_SUPPORTED               "ColorSupported"
#define MAX_COPIES_SUPPORTED          "MaxCopiesSupported"
#define NUMBER_UP_SUPPORTED           "NumberUpSupported"
#define DOCUMENT_FORMATS_SUPPORTED    "DocumentFormatsSupported"
#define IMAGE_FORMATS_SUPPORTED       "ImageFormatsSupported"
#define SIDES_SUPPORTED               "SidesSupported"
#define ORIENTATIONS_SUPPORTED        "OrientationsSupported"
#define MEDIA_SIZES_SUPPORTED         "MediaSizesSupported"
#define MEDIA_TYPES_SUPPORTED         "MediaTypesSupported"
#define PRINT_QUALITY_SUPPORTED       "PrintQualitySupported"

#define LOADED_MEDIUM                 "MediaLoaded"
#define LOADED_MEDIUM_DETAILS         "LoadedMediumDetails"
#define LOADED_MEDIUM_SIZE            "LoadedMediumSize"
#define LOADED_MEDIUM_TYPE            "LoadedMediumType"

#define ONE_SIDED                     "one-sided"
#define TWO_SIDED_LONG                "two-sided-long-edge"
#define TWO_SIDED_SHORT               "two-sided-short-edge"

#define QUALITY_DRAFT                 "draft"
#define QUALITY_NORMAL                "normal"
#define QUALITY_HIGH                  "high"

#define ORI_PORTRAIT                  "portrait"
#define ORI_LANDSCAPE                 "landscape"
#define ORI_REV_LANDSCAPE             "reverse-landScape"
#define ORI_REV_PORTRAIT              "reverse-portrait"

#define JOB_ID                        "JobId"
#define JOB_STATE                     "JobState"
#define JOB_NAME                      "JobName"
#define JOB_USER_NAME                 "JobOriginatingUserName"
#define JOB_SHEETS_COMPLETED          "JobMediaSheetsCompleted"
#define JOB_NUMBER_OF_INTERVENING     "NumberOfInterveningJobs"

#define ENVELOPE_END                  "</s:Envelope>"
#define MIME_XHTML_0_95               "application/vnd.pwg-xhtml-print+xml:0.95"
#define MIME_XHTML_1_00               "application/vnd.pwg-xhtml-print+xml:1.0"
#define MIME_TEXT                     "text/plain"
#define MIME_VCARD21                  "text/x-vcard:2.1"
#define MIME_VCARD30                  "text/x-vcard:3.0"
#define MIME_VCALENDAR                "text/x-vcalendar:1.0"
#define MIME_ICALENDAR                "text/calendar:2.0"
#define MIME_POST2                    "application/PostScript:2"
#define MIME_POST3                    "application/PostScript:3"
#define MIME_PCL5E                    "application/vpn.hp-PCL:5E"
#define MIME_PCL6                     "application/vpn.hp-PCL:6"
#define MIME_PDF                      "application/PDF"
#define MIME_JPEG                     "image/jpeg"
#define MIME_GIF                      "image/gif:89A"
#define MIME_VMESSAGE11               "text/x-vmessage:1.1"

#define FILE_JPEG                     "jpeg_file.jpg"
#define FILE_VCARD                    "v_card_file_single.vcf"
#define FILE_VCARD_MULTI              "v_card_file_multi.vcf"
#define FILE_VCARD_30                 "v_card_file_30.vcf"
#define FILE_PDF                      "pdf_file.pdf"
#define FILE_TEXT                     "text_file.txt"
#define FILE_TEXT_NON_ISO             "text_file_non_iso.txt"
#define FILE_GIF                      "gif_file.gif"
#define FILE_VCAL                     "v_cal_file_single.vcs"
#define FILE_VCAL_MULTI               "v_cal_file_multi.vcs"
#define FILE_ICAL                     "i_cal_file.vcs"
#define FILE_PCL5E                    "pcl5eFile.pcl"
#define FILE_PCL6                     "pcl6File.pcl"
#define FILE_POST2                    "post_script2file.ps"
#define FILE_POST3                    "post_script3file.ps"
#define FILE_XHTML_0_95               "xhtml_file_ver0_95.htm"
#define FILE_XHTML_1_00               "xhtml_file_ver1_0.htm"
#define FILE_XHTML_0_95_IMG           "xhtml_image_file_ver0_95.htm"
#define FILE_XHTML_1_00_IMG           "xhtml_image_file_ver1_0.htm"
#define FILE_VMESSAGE_11              "v_message_file.vmg"

#define sentFileFolder                "bpp_files/"

static void bppDeactivateCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);;
static void bppConnectCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcGetPrinterAttributesIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcGetPrinterAttributesCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppCreateJobCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcSendDocumentIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcSendDocumentCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcDisconnectIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcAuthenticateIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcGetReferenceObjectIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcSecurityOutCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);

static void bppcGetJobAttributesIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcGetJobAttributesCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppCancelJobCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcGetEventMsgIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppcAbortCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void bppConnectIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void handleSdPrim(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void showMenu(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void keyMessageHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void keyboardMenuStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectAddressStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardAuthenticateStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectMediaSizeStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectMediaTypeStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectDocumentStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectOrientationStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectMaxCopiesStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectNumberUpStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectSideStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectPrintQualityStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void xmlDocumentSelectHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void keyboardTypeDocumentFileStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectMimeTypeMediaStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardTypeMimeTypeMediaStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectAttributeStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectVcardsPerPageStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);
static void keyboardSelectVcalendarObjPerPageStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter);

static void sendingTheSelectedFileRefObj(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint32 count,
                                         CsrUint32 offset, CsrUint32 fileSize, CsrBool lastPacket);
static void jobAttributeResultHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 * object, CsrUint16 length);
static void xmlStatusChannelInterpreterHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void getEventResultHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 * object, CsrUint16 length);

static void KeyMessageService(CsrUint16 m ,void *data);
static void printerAttributeResultHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 * object, CsrUint16 length);
static void xmlMediaSizesHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void xmlMediaTypesHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void xmlInterpreterHandler(BppcDemoInstanceDataType * bppcDemoInstanceData);
static void sendingTheSelectedFile(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint16 maxToSend);

static const BppcDemoStateHandlerType bppcDemoStateHandlers[DEMO_NUMBER_OF_STATES][CSR_BT_BPPC_PRIM_UPSTREAM_COUNT] =
{
    {
        bppDeactivateCfmHandler,                    /* CSR_BT_BPPC_DEACTIVATE_CFM */
        bppConnectIndHandler,                       /* CSR_BT_BPPC_CONNECT_IND */
        bppConnectCfmHandler,                       /* CSR_BT_BPPC_CONNECT_CFM */
        bppcDisconnectIndHandler,                    /* CSR_BT_BPPC_DISCONNECT_IND */
        bppcAuthenticateIndHandler,                  /* CSR_BT_BPPC_AUTHENTICATE_IND */
        bppcGetPrinterAttributesIndHandler,          /* CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */
        bppcGetPrinterAttributesCfmHandler,          /* CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */
        bppCreateJobCfmHandler,                     /* CSR_BT_BPPC_CREATE_JOB_CFM */
        bppcSendDocumentIndHandler,                  /* CSR_BT_BPPC_SEND_DOCUMENT_IND */
        bppcSendDocumentCfmHandler,                  /* CSR_BT_BPPC_SEND_DOCUMENT_CFM */
        bppcGetReferenceObjectIndHandler,            /* BPPC_GET_REF_OBJECT_IND */
        bppcGetJobAttributesIndHandler,              /* CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */
        bppcGetJobAttributesCfmHandler,              /* CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */
        bppCancelJobCfmHandler,                     /* CSR_BT_BPPC_CANCEL_JOB_CFM */
        bppcGetEventMsgIndHandler,                   /* CSR_BT_BPPC_GET_EVENT_IND */
        bppcAbortCfmHandler,                         /* CSR_BT_BPPC_ABORT_CFM */
        bppcSecurityOutCfmHandler                    /* CSR_BT_BPPC_SECURITY_OUT_CFM */
    }
};

static void bppScAppSetFnFini(void *pFiniData)
{
    BppcDemoInstanceDataType *instData = (BppcDemoInstanceDataType *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState);
/*
    system(CLS);
*/
    showMenu(instData);
}

static void bppScAppSetFnInit(void *pInitData)
{
    BppcDemoInstanceDataType *instData  = (BppcDemoInstanceDataType *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState, KEYBOARD_BONDING_STATE);
}

static void* bppScAppSetFnGetMessage(void *pRecvMsgPData)
{
    BppcDemoInstanceDataType *instData = (BppcDemoInstanceDataType *) pRecvMsgPData;
    void *result                      = instData->recvMsgP;
    instData->recvMsgP                = NULL;
    return result;
}

/*******************************************************************************************
  CsrBtAppInit: Allocate and initialise Application instance data space
********************************************************************************************/
void CsrBtAppInit(void ** gash)
{
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;
    BppcDemoInstanceDataType * bppcDemoInstanceData;

    *gash = CsrPmemAlloc(sizeof(BppcDemoInstanceDataType));
    bppcDemoInstanceData = (BppcDemoInstanceDataType *) *gash;
    bppcDemoInstanceData->recvMsgP = NULL;
    bppcDemoInstanceData->currentFileHandle = NULL;
    bppcDemoInstanceData->recvXmlPtr = NULL;
    bppcDemoInstanceData->xmlTree = NULL;
    bppcDemoInstanceData->documentFormatsSupported = NULL;
    bppcDemoInstanceData->mediaTypesSupported = NULL;
    bppcDemoInstanceData->xhtmlPrintImageFormats = NULL;
    bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
    bppcDemoInstanceData->menuState = MENU_IDLE_STATE;
    bppcDemoInstanceData->activated = FALSE;

    CsrMemSet(bppcDemoInstanceData->mimeMediaType,
           0,
           sizeof(bppcDemoInstanceData->mimeMediaType));

    CsrMemSet(bppcDemoInstanceData->documentName,
           0,
           sizeof(bppcDemoInstanceData->documentName));

    CsrMemSet(bppcDemoInstanceData->referenceFileName,
           0,
           sizeof(bppcDemoInstanceData->referenceFileName));

    CsrBtBdAddrZero(&zeroAddr);
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        bppcDemoInstanceData->deviceAddr.lap = defGlobalBdAddr.lap;
        bppcDemoInstanceData->deviceAddr.nap = defGlobalBdAddr.nap;
        bppcDemoInstanceData->deviceAddr.uap = defGlobalBdAddr.uap;
    }
    else
    {
        bppcDemoInstanceData->deviceAddr.lap = 0;
        bppcDemoInstanceData->deviceAddr.nap = 0;
        bppcDemoInstanceData->deviceAddr.uap = 0;
    }
    bppcDemoInstanceData->cancelConnect = FALSE;
    bppcDemoInstanceData->currentFilePosition = 0;
    bppcDemoInstanceData->currentRefFileHandle = NULL;
    bppcDemoInstanceData->currentRefFilePosition = 0;
    bppcDemoInstanceData->recvStatusXmlPtr = NULL;
    bppcDemoInstanceData->xmlStatusTree = NULL;
    bppcDemoInstanceData->getEventActive = FALSE;
    bppcDemoInstanceData->getEventTimeOut = 0;
    bppcDemoInstanceData->abortDisconnect = FALSE;

    /* Application handle */
    bppcDemoInstanceData->CsrSchedQid = TESTQUEUE;

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);

    /* Security App Configuration */
    CsrBtScActivateReqSend(bppcDemoInstanceData->CsrSchedQid);
    scAppInitInstanceData(&bppcDemoInstanceData->scInstData);

    scAppSetFnFini(&bppcDemoInstanceData->scInstData, bppScAppSetFnFini, bppcDemoInstanceData);
    scAppSetFnInit(&bppcDemoInstanceData->scInstData, bppScAppSetFnInit, bppcDemoInstanceData);
    scAppSetSecLevelSend(&bppcDemoInstanceData->scInstData, scAppSendProfileSecurityLevelReq, &bppcDemoInstanceData->scInstData);

    scAppSetFnGetMessage(&bppcDemoInstanceData->scInstData, bppScAppSetFnGetMessage, bppcDemoInstanceData);

    scAppSetSecLevelPrim(&bppcDemoInstanceData->scInstData,
                         0,     /* Index 0. Single profile */
                         "BPPC", CSR_BT_BPPC_PRIM,
                         CSR_BT_BPPC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_BPPC_SECURITY_OUT_REQ);
/*
    system(CLS);
*/
    showMenu(bppcDemoInstanceData);
}

/*******************************************************************************************
  CsrBtAppHandler: Handle incoming event from: the BPPC layer, the SC layer, or the Keyboard
********************************************************************************************/
void CsrBtAppHandler(void ** gash)
{
    CsrUint16    event=0;

    BppcDemoInstanceDataType * bppcDemoInstanceData;

    bppcDemoInstanceData = (BppcDemoInstanceDataType *) *gash;

    CsrSchedMessageGet(&event, &bppcDemoInstanceData->recvMsgP);

    switch (event)
    {
        case CSR_BT_BPPC_PRIM:
            {
                CsrBtBppcPrim    * type;

                type = (CsrBtBppcPrim    *) bppcDemoInstanceData->recvMsgP;

                if (*type == CSR_BT_BPPC_SECURITY_OUT_CFM)
                {

                    handleScSecLevelPrim(&bppcDemoInstanceData->scInstData);
                }
                else if (((*type - CSR_BT_BPPC_PRIM_UPSTREAM_LOWEST ) < CSR_BT_BPPC_PRIM_UPSTREAM_COUNT ) &&
                    bppcDemoStateHandlers[0][(CsrUint16) (*type - CSR_BT_BPPC_PRIM_UPSTREAM_LOWEST )] != NULL)
                {
                    bppcDemoStateHandlers[0][(CsrUint16) (*type - CSR_BT_BPPC_PRIM_UPSTREAM_LOWEST )](bppcDemoInstanceData);
                }
                else
                {
                    printf("A state event error occurred in the BPPC Application layer,\n  it is a CSR_BT_BPPC_PRIM type: %02X\n\n",
                           *type);
                    CsrBtBppcFreeUpstreamMessageContents(event, bppcDemoInstanceData->recvMsgP);
                }
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&bppcDemoInstanceData->scInstData);
                break;
            }
        case CSR_BT_SD_PRIM:
            {
                handleSdPrim(bppcDemoInstanceData);
                break;
            }
        case KEY_MESSAGE:
            {
                keyMessageHandler(bppcDemoInstanceData);
                break;
            }
        default:
            {
                printf("A state event error occurred in the BPPC Application layer,\n  it is a UNKNOWN_PRIM %04X\n",
                       event);
                break;
            }
    }
    CsrPmemFree(bppcDemoInstanceData->recvMsgP);
}


static void bppConnectIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcConnectInd * prim;

    prim = (CsrBtBppcConnectInd *) bppcDemoInstanceData->recvMsgP;

    CsrBtBppcConnectResSend(CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
}

static void bppcGetReferenceObjectIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcGetReferenceObjectInd * prim;

    prim = (CsrBtBppcGetReferenceObjectInd*) bppcDemoInstanceData->recvMsgP;

    if (!bppcDemoInstanceData->currentRefFileHandle)
    {
        char sentFolder[CSR_BT_MAX_PATH_LENGTH];
        int     result;
        CsrBtStat_t statBuffer;
        CsrUint32 length;

        CsrMemSet(sentFolder, 0, CSR_BT_MAX_PATH_LENGTH);
        CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
        statBuffer.st_size = 0;

        bppcDemoInstanceData->refCount = prim->count;
        bppcDemoInstanceData->refOffset = prim->offset;
        bppcDemoInstanceData->currentRefFilePosition = 0;

        length = CsrStrLen((char*)prim->fileName);
        CsrMemCpy(bppcDemoInstanceData->referenceFileName, prim->fileName, length);
        bppcDemoInstanceData->referenceFileName[length] = '\0';

        CsrPmemFree(prim->fileName);

        CsrStrCpy(sentFolder, sentFileFolder);
        CsrStrCat(sentFolder, bppcDemoInstanceData->referenceFileName);

        result = CsrBtStat(sentFolder, &statBuffer);
        if(!result)
        {
            if((bppcDemoInstanceData->currentRefFileHandle = CsrBtFopen(sentFolder, "rb")) == NULL)
            {
                printf("Fail. Demo could not open the file '%s'\n", sentFolder);
            }
            else
            {
                bppcDemoInstanceData->currentRefFileSize = statBuffer.st_size;
                sendingTheSelectedFileRefObj(bppcDemoInstanceData,
                                             prim->count,
                                             prim->offset,
                                             bppcDemoInstanceData->currentRefFileSize,
                                             prim->lastPacket);
            }
        }
        else
        {
            printf("Fail. Demo could not stat the file '%s'\n", sentFolder);
        }
    }
    else
    {
        bppcDemoInstanceData->currentRefFilePosition = prim->offset;
        sendingTheSelectedFileRefObj(bppcDemoInstanceData,
                                     prim->count,
                                     prim->offset,
                                     bppcDemoInstanceData->currentRefFileSize,
                                     prim->lastPacket);
    }

}

static void bppcSecurityOutCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
}


static void simpleModelDocumentFormatInterpreter(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrUint16 i, documentFormatLength, startIndex, length;
    CsrBool     temp = TRUE;
    documentFormatLength = (CsrUint16) CsrStrLen((char*)bppcDemoInstanceData->documentFormatsSupported) + 1;
    startIndex = 0;

    printf("%s", "DocumentFormat\n");

    for (i = 0; i < documentFormatLength; i++)
    {
        if (bppcDemoInstanceData->documentFormatsSupported[i] == ',' ||
            bppcDemoInstanceData->documentFormatsSupported[i] == '\0')
        {
            temp = TRUE;
            printf("\n");
            length = i - startIndex;

            if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                        (CsrUint8 *) MIME_XHTML_0_95,
                        length))
            {
                bppcDemoInstanceData->documentFormat += XHTML95_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_XHTML_1_00,
                             length))
            {
                bppcDemoInstanceData->documentFormat += XHTML10_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_TEXT,
                             length))
            {
                bppcDemoInstanceData->documentFormat += TEXT_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_VCARD21,
                             length))
            {
                bppcDemoInstanceData->documentFormat += VCARD21_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_VCALENDAR,
                             length))
            {
                bppcDemoInstanceData->documentFormat += VCAL_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_ICALENDAR,
                             length))
            {
                bppcDemoInstanceData->documentFormat += ICAL_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_POST2,
                             length))
            {
                bppcDemoInstanceData->documentFormat += POSTS2_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_POST3,
                             length))
            {
                bppcDemoInstanceData->documentFormat += POSTS3_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_PCL5E,
                             length))
            {
                bppcDemoInstanceData->documentFormat += PCL5E_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_PCL6,
                             length))
            {
                bppcDemoInstanceData->documentFormat += PCL6_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_PDF,
                             length))
            {
                bppcDemoInstanceData->documentFormat += PDF_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_JPEG,
                             length))
            {
                bppcDemoInstanceData->documentFormat += JPEG_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_GIF,
                             length))
            {
                bppcDemoInstanceData->documentFormat += GIF_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_VCARD30,
                             length))
            {
                bppcDemoInstanceData->documentFormat += VCARD30_MASK;
            }
            else if (!CsrMemCmp(&bppcDemoInstanceData->documentFormatsSupported[startIndex],
                             (CsrUint8 *) MIME_VMESSAGE11,
                             length))
            {
                bppcDemoInstanceData->documentFormat += VMESSAGE11_MASK;
            }
            startIndex = i + 1;
        }
        else
        {
            if (temp)
            {
                temp = FALSE;
                printf("%-30.30s %c", " ", bppcDemoInstanceData->documentFormatsSupported[i]);
            }
            else
            {
                printf("%c",bppcDemoInstanceData->documentFormatsSupported[i]);
            }
        }
    }
    printf("\n\n");
}

static void simpleModelmediaTypesInterpreter(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    if (bppcDemoInstanceData->mediaTypesSupported)
    {
        CsrUint16 i, mediaTypesLength;
        CsrBool     temp = TRUE;

        if(bppcDemoInstanceData->mediaTypesSupported != NULL)
        {
            mediaTypesLength = (CsrUint16) CsrStrLen((char*)bppcDemoInstanceData->mediaTypesSupported);
            printf("%s", "MediaType\n");

            for (i = 0; i < mediaTypesLength; i++)
            {
                if (bppcDemoInstanceData->mediaTypesSupported[i] != ',')
                {
                    if (temp)
                    {
                        temp = FALSE;
                        printf("%-30.30s %c", " ", bppcDemoInstanceData->mediaTypesSupported[i]);
                    }
                    else
                    {
                        printf("%c",bppcDemoInstanceData->mediaTypesSupported[i]);
                    }
                }
                else
                {
                    temp = TRUE;
                    printf("\n");
                }
            }
            printf("\n\n");
        }
    }
    else
    {
        printf("No 'mediaTypesSupported'\n");
    }
}

static void simpleModelxhtmlPrintImageInterpreter(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    if (bppcDemoInstanceData->xhtmlPrintImageFormats)
    {
        CsrUint16 i, imageFormatsLength;
        CsrBool     temp = TRUE;

        imageFormatsLength = (CsrUint16) CsrStrLen((char*)bppcDemoInstanceData->xhtmlPrintImageFormats);

        printf("%s", "Image Formats\n");

        for (i = 0; i < imageFormatsLength; i++)
        {
            if (bppcDemoInstanceData->xhtmlPrintImageFormats[i] != ',')
            {
                if (temp)
                {
                    temp = FALSE;
                    printf("%-30.30s %c", " ", bppcDemoInstanceData->xhtmlPrintImageFormats[i]);
                }
                else
                {
                    printf("%c",bppcDemoInstanceData->xhtmlPrintImageFormats[i]);
                }
            }
            else
            {
                temp = TRUE;
                printf("\n");
            }
        }
        printf("\n\n");
    }
}

static void bppDeactivateCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcDeactivateCfm * prim;

    prim = (CsrBtBppcDeactivateCfm *) bppcDemoInstanceData->recvMsgP;

    bppcDemoInstanceData->activated = FALSE;

    showMenu(bppcDemoInstanceData);

}

static void bppConnectCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcConnectCfm * prim;

    prim = (CsrBtBppcConnectCfm *) bppcDemoInstanceData->recvMsgP;
    bppcDemoInstanceData->cancelConnect = FALSE;


    if (prim->printerModelId)
    {
        printf("\nThe printer model is: %s\n\n", prim->printerModelId);
        CsrPmemFree(prim->printerModelId);
    }

    if (prim->resultCode      == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
        prim->resultSupplier  == CSR_BT_SUPPLIER_IRDA_OBEX)
    {
        bppcDemoInstanceData->sides = 0;
        bppcDemoInstanceData->printfQuality = 0;
        bppcDemoInstanceData->orientation = 0;
        bppcDemoInstanceData->documentFormat = 0;
        bppcDemoInstanceData->numberUp = 0;
        bppcDemoInstanceData->maxCopies = 0;
        bppcDemoInstanceData->numOfDiffMediaSizes = 0;
        bppcDemoInstanceData->numOfDiffMediaTypes = 0;
        bppcDemoInstanceData->numOfDiffDocumentFormats = 0;


        if (bppcDemoInstanceData->recvXmlPtr)
        {
            CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
            bppcDemoInstanceData->recvXmlPtr = NULL;
        }
        if (bppcDemoInstanceData->xmlTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
            bppcDemoInstanceData->xmlTree = NULL;
        }

        if (prim->documentFormatsSupported)
        {
            bppcDemoInstanceData->documentFormatsSupported = prim->documentFormatsSupported;
            bppcDemoInstanceData->menuState = MENU_SELECT_TRANSFER_MODEL;
            bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;

            /* Delete old */
            if(bppcDemoInstanceData->mediaTypesSupported)
            {
                CsrPmemFree(bppcDemoInstanceData->mediaTypesSupported);
                bppcDemoInstanceData->mediaTypesSupported= NULL;
            }
            if(bppcDemoInstanceData->mediaTypesSupported)
            {
                CsrPmemFree(bppcDemoInstanceData->mediaTypesSupported);
                bppcDemoInstanceData->mediaTypesSupported = NULL;
            }

            /* Set new */
            if(prim->mediaTypesSupported)
            {
                bppcDemoInstanceData->mediaTypesSupported = prim->mediaTypesSupported;
            }
            if(prim->xhtmlPrintImageFormats)
            {
                bppcDemoInstanceData->xhtmlPrintImageFormats = prim->xhtmlPrintImageFormats;
            }

            showMenu(bppcDemoInstanceData);
        }
        else
        {
            CsrPmemFree(prim->xhtmlPrintImageFormats);
            CsrPmemFree(prim->mediaTypesSupported);
            bppcDemoInstanceData->transferModel = CSR_BT_JOB_BASED_TRANSFER_MODEL;
            bppcDemoInstanceData->menuState = MENU_SELECT_ATTRIBUTE_STATE;
/*
            system(CLS);
*/
            showMenu(bppcDemoInstanceData);
        }
    }
    else
    {
        CsrPmemFree(prim->xhtmlPrintImageFormats);
        CsrPmemFree(prim->documentFormatsSupported);
        CsrPmemFree(prim->mediaTypesSupported);
/*
        system(CLS);
*/
        printf("\n\nOBEX connection request against %04X %02X %06X, fail with resultCode 0x%04X, resultSupplier 0x%04X\n\n",
               bppcDemoInstanceData->deviceAddr.nap,
               bppcDemoInstanceData->deviceAddr.uap,
               bppcDemoInstanceData->deviceAddr.lap,
               prim->resultCode,
               prim->resultSupplier);
        showMenu(bppcDemoInstanceData);
    }

}

static void bppcGetPrinterAttributesIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcGetPrinterAttributesInd * prim;

    prim = (CsrBtBppcGetPrinterAttributesInd *) bppcDemoInstanceData->recvMsgP;
    printerAttributeResultHandler(bppcDemoInstanceData,
                                  (prim->payload + prim->printerAttributeObjectOffset),
                                  prim->printerAttributeObjectLength);
    CsrBtBppcGetPrinterAttributesResSend(FALSE);
    CsrPmemFree(prim->payload);
}

static void bppcGetPrinterAttributesCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcGetPrinterAttributesCfm * prim;

    prim = (CsrBtBppcGetPrinterAttributesCfm *) bppcDemoInstanceData->recvMsgP;
/*
    system(CLS);
*/
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        printf("The printers attributes are:\n\n");
        printerAttributeResultHandler(bppcDemoInstanceData,
                                      (prim->payload + prim->printerAttributeObjectOffset),
                                      prim->printerAttributeObjectLength);
        xmlInterpreterHandler(bppcDemoInstanceData);
        showMenu(bppcDemoInstanceData);

    }
    else
    {
        printf("\n\n GetPrinterAttr retrieve with fail code: %04X.... Trying again\n\n", prim->responseCode);
        bppcDemoInstanceData->sides = 0;
        bppcDemoInstanceData->printfQuality = 0;
        bppcDemoInstanceData->orientation = 0;
        bppcDemoInstanceData->documentFormat = 0;
        bppcDemoInstanceData->numberUp = 0;
        bppcDemoInstanceData->maxCopies = 0;
        bppcDemoInstanceData->numOfDiffMediaSizes = 0;
        bppcDemoInstanceData->numOfDiffMediaTypes = 0;
        bppcDemoInstanceData->numOfDiffDocumentFormats = 0;
        if (bppcDemoInstanceData->recvXmlPtr)
        {
            CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
            bppcDemoInstanceData->recvXmlPtr = NULL;
        }
        if (bppcDemoInstanceData->xmlTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
            bppcDemoInstanceData->xmlTree = NULL;
        }

        bppcDemoInstanceData->menuState = MENU_SELECT_ATTRIBUTE_STATE;
        showMenu(bppcDemoInstanceData);
    }
    CsrPmemFree(prim->payload);
}

static void bppcGetJobAttributesIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcGetJobAttributesInd * prim;

    prim = (CsrBtBppcGetJobAttributesInd *) bppcDemoInstanceData->recvMsgP;
    jobAttributeResultHandler(bppcDemoInstanceData,
                              (prim->payload + prim->jobAttributeObjectOffset),
                              prim->jobAttributeObjectLength);
    CsrBtBppcGetJobAttributesResSend(FALSE);
    CsrPmemFree(prim->payload);
}

static void bppcGetJobAttributesCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcGetJobAttributesCfm * prim;

    prim = (CsrBtBppcGetJobAttributesCfm *) bppcDemoInstanceData->recvMsgP;
/*
    system(CLS);
*/
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        printf("The Jobs attributes are:\n\n");
        jobAttributeResultHandler(bppcDemoInstanceData,
                                  (prim->payload + prim->jobAttributeObjectOffset),
                                  prim->jobAttributeObjectLength);
        xmlStatusChannelInterpreterHandler(bppcDemoInstanceData);
        showMenu(bppcDemoInstanceData);

    }
    else
    {
        printf("\n\n GetJobAttr retrieve with fail code: %04X.... \n\n", prim->responseCode);

        bppcDemoInstanceData->menuState = MENU_SELECT_ATTRIBUTE_STATE;
/*
        system(CLS);
*/
        showMenu(bppcDemoInstanceData);
    }
    CsrPmemFree(prim->payload);
}

static void bppcGetEventMsgIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcGetEventInd * prim;

    prim = (CsrBtBppcGetEventInd *) bppcDemoInstanceData->recvMsgP;
    getEventResultHandler(bppcDemoInstanceData,
                          (prim->payload + prim->eventObjectOffset),
                          prim->eventObjectLength);
    CsrBtBppcGetEventResSend(FALSE);
    CsrPmemFree(prim->payload);
}


static void bppcAbortCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcAbortCfm * prim;

    prim = (CsrBtBppcAbortCfm *) bppcDemoInstanceData->recvMsgP;

    CsrSchedTimerCancel(bppcDemoInstanceData->getEventTimeOut, NULL, NULL);
    bppcDemoInstanceData->getEventActive = FALSE;


    if (bppcDemoInstanceData->abortDisconnect)
    {
        CsrBtBppcDisconnectReqSend(TRUE);
        bppcDemoInstanceData->abortDisconnect = FALSE;
    }
}

static void bppCreateJobCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcCreateJobCfm * prim;

    prim = (CsrBtBppcCreateJobCfm *) bppcDemoInstanceData->recvMsgP;

/*
    system(CLS);
*/
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && prim->operationStatus < 0x0003)
    {
        bppcDemoInstanceData->jobId = prim->jobId;
        bppcDemoInstanceData->menuState = MENU_JOB_CREATED_STATE;
        printf("The operationStatus is %02X\n", prim->operationStatus);
        showMenu(bppcDemoInstanceData);
    }
    else
    {
/*
        system(CLS);
*/
        printf("\n\n The request to configure a print job fail with code: %04X and with operationStatus %02X\n\n",
               prim->responseCode,
               prim->operationStatus);
        bppcDemoInstanceData->menuState = MENU_CONNECTED_STATE;
        if (bppcDemoInstanceData->recvXmlPtr)
        {
            CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
            bppcDemoInstanceData->recvXmlPtr = NULL;
        }
        if (bppcDemoInstanceData->xmlTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
            bppcDemoInstanceData->xmlTree = NULL;
        }
        bppcDemoInstanceData->sides = 0;
        bppcDemoInstanceData->printfQuality = 0;
        bppcDemoInstanceData->orientation = 0;
        bppcDemoInstanceData->documentFormat = 0;
        bppcDemoInstanceData->numberUp = 0;
        bppcDemoInstanceData->maxCopies = 0;
        bppcDemoInstanceData->numOfDiffMediaSizes = 0;
        bppcDemoInstanceData->numOfDiffMediaTypes = 0;
        bppcDemoInstanceData->numOfDiffDocumentFormats = 0;

        CsrMemSet(bppcDemoInstanceData->mimeMediaType,
               0,
               sizeof(bppcDemoInstanceData->mimeMediaType));
        CsrMemSet(bppcDemoInstanceData->documentName,
               0,
               sizeof(bppcDemoInstanceData->documentName));

        bppcDemoInstanceData->menuState = MENU_SELECT_ATTRIBUTE_STATE;
        showMenu(bppcDemoInstanceData);
    }
}

static void bppCancelJobCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcCancelJobCfm * prim;

    prim = (CsrBtBppcCancelJobCfm *) bppcDemoInstanceData->recvMsgP;

/*
    system(CLS);
*/
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && prim->operationStatus < 0x0003)
    {
        bppcDemoInstanceData->jobId = 0;
        bppcDemoInstanceData->menuState = MENU_CONNECTED_STATE;
        printf("The this job %d has been cancelled and operationStatus is %02X\n",
               prim->jobId,
               prim->operationStatus);
        showMenu(bppcDemoInstanceData);
    }
    else
    {
/*
        system(CLS);
*/
        printf("\n\n The request to cancel a print job fail with code: %04X and with operationStatus %02X\n\n",
               prim->responseCode,
               prim->operationStatus);

        bppcDemoInstanceData->menuState = MENU_SELECT_ATTRIBUTE_STATE;
        showMenu(bppcDemoInstanceData);
    }
}


static void bppcSendDocumentIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcSendDocumentInd * prim;

    prim = (CsrBtBppcSendDocumentInd *) bppcDemoInstanceData->recvMsgP;

    if (!bppcDemoInstanceData->currentFileHandle)
    {
        char sentFolder[CSR_BT_MAX_PATH_LENGTH];
        int        result;
        CsrBtStat_t statBuffer;

        CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
        statBuffer.st_size = 0;

        CsrStrCpy(sentFolder, sentFileFolder);
        CsrStrCat(sentFolder, bppcDemoInstanceData->documentName);

        result = CsrBtStat(sentFolder, &statBuffer);
        if(!result)
        {
            if((bppcDemoInstanceData->currentFileHandle = CsrBtFopen(sentFolder, "rb")) == NULL)
            {
                printf("Fail. DEMO could not open the file %s\n", sentFolder);
            }
            else
            {
                bppcDemoInstanceData->currentFileSize = statBuffer.st_size;
                sendingTheSelectedFile(bppcDemoInstanceData, prim->printContentLength);
            }
        }
        else
        {
            printf("Fail. DEMO could not open the file %s\n", sentFolder);
        }
    }
    else
    {
        sendingTheSelectedFile(bppcDemoInstanceData, prim->printContentLength);
    }
}

static void bppcSendDocumentCfmHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcSendDocumentCfm * prim;

    prim = (CsrBtBppcSendDocumentCfm *) bppcDemoInstanceData->recvMsgP;

/*
    system(CLS);
*/
    bppcDemoInstanceData->menuState = MENU_CONNECTED_STATE;
    if (prim->responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        printf("\n\n Print content was sent with success\n");
    }
    else
    {
        printf("\n\n Fail to send print content, ResponseCode is %02Xh\n",
               prim->responseCode);
    }

    if (bppcDemoInstanceData->getEventActive) {
        CsrSchedTimerCancel(bppcDemoInstanceData->getEventTimeOut, NULL, NULL);
        bppcDemoInstanceData->getEventActive = FALSE;
    }

    if (bppcDemoInstanceData->currentFileHandle)
    {
        CsrBtFclose(bppcDemoInstanceData->currentFileHandle);
        bppcDemoInstanceData->currentFilePosition = 0;
        bppcDemoInstanceData->currentFileHandle = NULL;
    }
    bppcDemoInstanceData->sides = 0;
    bppcDemoInstanceData->printfQuality = 0;
    bppcDemoInstanceData->orientation = 0;
    bppcDemoInstanceData->documentFormat = 0;
    bppcDemoInstanceData->numberUp = 0;
    bppcDemoInstanceData->maxCopies = 0;
    bppcDemoInstanceData->numOfDiffMediaSizes = 0;
    bppcDemoInstanceData->numOfDiffMediaTypes = 0;
    bppcDemoInstanceData->numOfDiffDocumentFormats = 0;
    if (bppcDemoInstanceData->recvXmlPtr)
    {
        CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
        bppcDemoInstanceData->recvXmlPtr = NULL;
    }
    if (bppcDemoInstanceData->xmlTree)
    {
        CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
        bppcDemoInstanceData->xmlTree = NULL;
    }

    if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
    {
        bppcDemoInstanceData->menuState = MENU_SELECT_ATTRIBUTE_STATE;
/*
        system(CLS);
*/
        showMenu(bppcDemoInstanceData);
    }
    else
    {
        simpleModelDocumentFormatInterpreter(bppcDemoInstanceData);
        simpleModelmediaTypesInterpreter(bppcDemoInstanceData);
        simpleModelxhtmlPrintImageInterpreter(bppcDemoInstanceData);
        showMenu(bppcDemoInstanceData);
    }

    CsrMemSet(bppcDemoInstanceData->mimeMediaType,
           0,
           sizeof(bppcDemoInstanceData->mimeMediaType));
    CsrMemSet(bppcDemoInstanceData->documentName,
           0,
           sizeof(bppcDemoInstanceData->documentName));
}

static void bppcDisconnectIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcDisconnectInd *prim;
    prim = (CsrBtBppcDisconnectInd *) bppcDemoInstanceData->recvMsgP;

    if (prim->channelId == CSR_BT_BPPC_JOB_CHANNEL)
    {
/*
        system(CLS);
*/
        if (bppcDemoInstanceData->currentFileHandle)
        {
            CsrBtFclose(bppcDemoInstanceData->currentFileHandle);
            bppcDemoInstanceData->currentFilePosition = 0;
            bppcDemoInstanceData->currentFileHandle = NULL;
        }
        if (bppcDemoInstanceData->recvXmlPtr)
        {
            CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
            bppcDemoInstanceData->recvXmlPtr = NULL;
        }
        if (bppcDemoInstanceData->xmlTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
            bppcDemoInstanceData->xmlTree = NULL;
        }
        if (bppcDemoInstanceData->recvStatusXmlPtr)
        {
            CsrPmemFree(bppcDemoInstanceData->recvStatusXmlPtr);
            bppcDemoInstanceData->recvStatusXmlPtr = NULL;
        }
        if (bppcDemoInstanceData->xmlStatusTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlStatusTree);
            bppcDemoInstanceData->xmlStatusTree = NULL;
        }
        if (bppcDemoInstanceData->xhtmlPrintImageFormats)
        {
            CsrPmemFree(bppcDemoInstanceData->xhtmlPrintImageFormats);
            bppcDemoInstanceData->xhtmlPrintImageFormats = NULL;
        }
        if (bppcDemoInstanceData->documentFormatsSupported)
        {
            CsrPmemFree(bppcDemoInstanceData->documentFormatsSupported);
            bppcDemoInstanceData->documentFormatsSupported = NULL;
        }
        if (bppcDemoInstanceData->mediaTypesSupported)
        {
            CsrPmemFree(bppcDemoInstanceData->mediaTypesSupported);
            bppcDemoInstanceData->mediaTypesSupported = NULL;
        }
        bppcDemoInstanceData->menuState = MENU_IDLE_STATE;
        bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
    }
    else if (prim->channelId == CSR_BT_BPPC_OBJECT_CHANNEL)
    {
        if (bppcDemoInstanceData->currentRefFileHandle != NULL)
        {
            CsrBtFclose(bppcDemoInstanceData->currentRefFileHandle);
            bppcDemoInstanceData->currentRefFileHandle = NULL;
        }
    }
    showMenu(bppcDemoInstanceData);
}

static void bppcAuthenticateIndHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtBppcAuthenticateInd *prim;
    prim = (CsrBtBppcAuthenticateInd *) bppcDemoInstanceData->recvMsgP;

    printf("\nPlease enter the OBEX response passkey: ");
    bppcDemoInstanceData->authenticateLength = 0;
    bppcDemoInstanceData->keyboardState = KEYBOARD_AUTHENTICATE_STATE;
    CsrPmemFree(prim->realm);
}

static void handleSdPrim(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrPrim    *primType;

    primType = (CsrPrim *) bppcDemoInstanceData->recvMsgP;
    switch (*primType)
    {
        /* the SC must have the passkey to use for the pairing procedure */
        case CSR_BT_SD_SEARCH_RESULT_IND:
            {
                CsrBtSdSearchResultInd * prim;
                CsrUint16 nameLen;
                CsrUint8 *name;

                prim = (CsrBtSdSearchResultInd *) bppcDemoInstanceData->recvMsgP;

                nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
                if (nameLen > 0)
                {
                    printf("   %s\n", name);
                    CsrPmemFree(name);
                }
                printf("   %04X:%02X:%06X\n",
                    prim->deviceAddr.nap,
                    prim->deviceAddr.uap,
                    prim->deviceAddr.lap);
                printf("   %s\n\n",CodDecodeDeviceClass(prim->deviceClass));
                CsrPmemFree(prim->info);
                break;
            }
        case CSR_BT_SD_CLOSE_SEARCH_IND:
            {
                bppcDemoInstanceData->addressLength    = 0;
                bppcDemoInstanceData->keyboardState    = KEYBOARD_SELECT_ADDRESS_STATE;
                printf("\n\nPlease type an address, or press enter to select default: %04X %02X %06X\n",
                       bppcDemoInstanceData->deviceAddr.nap,
                       bppcDemoInstanceData->deviceAddr.uap,
                       bppcDemoInstanceData->deviceAddr.lap);
                break;
            }
        default:
            {    /* unexpected primitive received */
                printf("####### default in sdPrim handler %04Xh,\n",*primType);
                CsrBtSdFreeUpstreamMessageContents(*primType, bppcDemoInstanceData->recvMsgP);
            }
    }
}

static void showMenu(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    printf("\nCSR plc. -- Synergy BT OBEX Basic Printing Profile Client (BPPC) demo\n");
    printf("\nProgram options:\n\n");
    switch (bppcDemoInstanceData->menuState)
    {
        case MENU_IDLE_STATE:
            {
                printf("    1) Search for Obex Printer devices\n");
                printf("    3) Initiate an OBEX connection\n");
                printf("    4) Security Controller Configuration\n");
                printf("\n    t) Toggle support for Reference printing %s\n",
                                bppcDemoInstanceData->activated ? "OFF (Deactivate)" : "ON (activate)");
                break;
            }
        case MENU_SEARCH_STATE:
            {
                printf("    2) Stop searching for Obex Printer devices\n");
                break;
            }
        case MENU_CONNECTED_STATE:
            {
                if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
                {
                    printf("    5) To create job\n");
                }
                else
                {
                    printf("    6) To select and send a document with the simple transfer model\n");
                }
                printf("    8) To release the OBEX connection\n");
                printf("\n    t) Toggle support for Reference printing %s\n",
                                bppcDemoInstanceData->activated ? "OFF (Deactivate)" : "ON (activate)");
                break;
            }
        case MENU_JOB_CREATED_STATE:
            {
                printf("    6) To send a document\n");
                printf("    7) To get job attributes\n");
                printf("    8) To release the OBEX connection\n");
                printf("    9) To cancel job\n");
                printf("    g) To getEvent or cancel getEvent\n");
                break;
            }
        case MENU_SELECT_TRANSFER_MODEL:
            {
                printf("    a) To select the Simple Push Transfer Model\n");
                printf("    b) To select the Job-Based Transfer Model\n");
                break;
            }
        case MENU_JOB_CREATE_CONFIG_STATE:
            {
                printf("    c) To CreateJob with supported Attributes\n");
                printf("    d) To CreateJob with default Attributes\n");
#ifdef XML_INTERPRET
                printf("    e) To CreateJob with unsupported Attributes (Only used for qual test)\n");
#endif
                break;
            }
        case MENU_SELECT_ATTRIBUTE_STATE:
            {
                printf("    0) To retrieve all printer attributes\n");
                printf("    1) To retrieve selected printer attributes\n");
                printf("    8) To release the OBEX connection\n");
                break;
            }
    }
    printf("\nHit <ESC> to quit program!\n\n");
}

static void keyMessageHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t*)bppcDemoInstanceData->recvMsgP;

    if (key->key == ESCAPE_KEY)
    {
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
    }
    else if (key->key == 0x20)
    {
        showMenu(bppcDemoInstanceData);
    }
    else
    {
        switch (bppcDemoInstanceData->keyboardState)
        {
            case KEYBOARD_MENU_STATE :
                {
                    keyboardMenuStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_ADDRESS_STATE:
                {
                    keyboardSelectAddressStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_BONDING_STATE:
                {
                    handleScKeybPrim(&bppcDemoInstanceData->scInstData);
                    break;
                }
            case KEYBOARD_AUTHENTICATE_STATE:
                {
                    keyboardAuthenticateStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_MEDIA_SIZE_STATE:
                {
                    keyboardSelectMediaSizeStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_MEDIA_TYPE_STATE:
                {
                    keyboardSelectMediaTypeStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_DOCUMENT_FORMAT_STATE:
                {
                    keyboardSelectDocumentStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_ORIENTATION_STATE:
                {
                    keyboardSelectOrientationStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_MAX_COPIES_STATE:
                {
                    keyboardSelectMaxCopiesStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_NUMBER_UP_STATE:
                {
                    keyboardSelectNumberUpStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_SIDE_STATE:
                {
                    keyboardSelectSideStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_PRINT_QUALITY_STATE:
                {
                    keyboardSelectPrintQualityStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_TYPE_DOCUMENT_FILE_STATE:
                {
                    keyboardTypeDocumentFileStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_TYPE_MIME_TYPE_MEDIA_STATE:
                {
                    keyboardTypeMimeTypeMediaStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_MIME_TYPE_MEDIA_STATE:
                {
                    keyboardSelectMimeTypeMediaStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_ATTRIBUTE_STATE :
                {
                    keyboardSelectAttributeStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_VCARDS_PER_PAGE_STATE :
                {
                    keyboardSelectVcardsPerPageStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
            case KEYBOARD_SELECT_VCAL_OBJ_PER_PAGE_STATE :
                {
                    keyboardSelectVcalendarObjPerPageStateHandler(bppcDemoInstanceData, key->key);
                    break;
                }
        }
    }
}

static void keyboardMenuStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    switch(bppcDemoInstanceData->menuState)
    {
        case MENU_IDLE_STATE:
            {
                switch (theCharacter)
                {
                    case '1':
                        {
                            CsrBtSdSearchReqSend(bppcDemoInstanceData->CsrSchedQid,
                                          CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                                          6000, /* RSSI buffer timeout */
                                          CSR_BT_INFINITE_TIME, /* Search timeout */
                                          CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                                          CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                                          CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                                          CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                                          0, /* Filter length */
                                          NULL); /* Filter */

                            bppcDemoInstanceData->menuState = MENU_SEARCH_STATE;
/*
                            system(CLS);
*/
                            showMenu(bppcDemoInstanceData);
                            break;
                        }
                    case '3':
                        {
/*
                            system(CLS);
*/
                            bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_ADDRESS_STATE;
                            bppcDemoInstanceData->addressLength = 0;
                            bppcDemoInstanceData->character = theCharacter;
                            printf("\n\nPlease type an address, or press Enter to select default: %04X %02X %06X\n",
                                   bppcDemoInstanceData->deviceAddr.nap,
                                   bppcDemoInstanceData->deviceAddr.uap,
                                   bppcDemoInstanceData->deviceAddr.lap);
                            break;
                        }
                    case '4':
                        {
                            handleScKeybPrim(&bppcDemoInstanceData->scInstData);
                            break;
                        }
                    case '8':
                        {
                            if (bppcDemoInstanceData->cancelConnect)
                            {
/*
                                system(CLS);
*/
                                printf("\n\n Releasing the OBEX connection\n");
                                CsrBtBppcCancelConnectReqSend();
                                bppcDemoInstanceData->cancelConnect = FALSE;
                            }
                            else
                            {
                                printf("####### default in keybPrim handler %x,\n", theCharacter);
                            }
                            break;
                        }
                    case 'T':
                    case 't':
                    {
                        if (bppcDemoInstanceData->activated)
                        {
                            CsrBtBppcDeactivateReqSend();
                            printf("\nDeactivating...");
                        }
                        else
                        {
                            CsrBtBppcActivateReqSend(bppcDemoInstanceData->CsrSchedQid,
                                                     CSR_BT_ANY_TYPE_SUPPORT,
                                                     MAX_PACKET_SIZE_TO_RETRIEVE,
                                                     0, /*windowSize*/
                                                     TRUE); /*enableSrm*/
                            bppcDemoInstanceData->activated = TRUE;
                            showMenu(bppcDemoInstanceData);
                        }
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
                    case '2':
                        {
                            CsrBtSdCancelSearchReqSend(bppcDemoInstanceData->CsrSchedQid);
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
        case MENU_CONNECTED_STATE:
            {
                switch (theCharacter)
                {

                    case '5':
                        {
                            if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
                            {
/*
                                system(CLS);
*/
                                if (bppcDemoInstanceData->getEventActive)
                                {
                                    CsrSchedTimerCancel(bppcDemoInstanceData->getEventTimeOut, NULL, NULL);
                                    CsrBtBppcAbortReqSend(CSR_BT_BPPC_STATUS_CHANNEL);
                                }

                                bppcDemoInstanceData->menuState = MENU_JOB_CREATE_CONFIG_STATE;
                                showMenu(bppcDemoInstanceData);
                            }
                            else
                            {
                                printf("####### default in keybPrim handler %x,\n", theCharacter);
                            }
                            break;
                        }
                    case '6':
                        {
                            if (bppcDemoInstanceData->transferModel == CSR_BT_SIMPLE_PUSH_TRANSFER_MODEL)
                            {
                                keyboardSelectMediaTypeStateHandler(bppcDemoInstanceData, '0');
                            }
                            else
                            {
                                printf("####### default in keybPrim handler %x,\n", theCharacter);
                            }
                            break;
                        }
                    case '8':
                        {
/*
                            system(CLS);
*/
                            printf("\n\n Releasing the OBEX connection\n");
                            if (bppcDemoInstanceData->getEventActive)
                            {
                                CsrSchedTimerCancel(bppcDemoInstanceData->getEventTimeOut, NULL, NULL);
                                CsrBtBppcAbortReqSend(CSR_BT_BPPC_STATUS_CHANNEL);
                                bppcDemoInstanceData->abortDisconnect = TRUE;
                            }
                            else
                            {
                                CsrBtBppcDisconnectReqSend(TRUE);
                            }
                            break;
                        }
                    case 'T':
                    case 't':
                    {
                        if (bppcDemoInstanceData->activated)
                        {
                            CsrBtBppcDeactivateReqSend();
                            printf("\nDeactivating...");
                        }
                        else
                        {
                            CsrBtBppcActivateReqSend(bppcDemoInstanceData->CsrSchedQid,
                                                     CSR_BT_ANY_TYPE_SUPPORT,
                                                     MAX_PACKET_SIZE_TO_RETRIEVE,
                                                     0, /*windowSize*/
                                                     TRUE); /*enableSrm*/
                            bppcDemoInstanceData->activated = TRUE;
                            showMenu(bppcDemoInstanceData);
                        }
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
        case MENU_JOB_CREATED_STATE:
            {
                switch (theCharacter)
                {
                    case '6':
                        {
                            if (bppcDemoInstanceData->mimeMediaType[0] == '\0')
                            {
                                bppcDemoInstanceData->keyboardState = KEYBOARD_TYPE_DOCUMENT_FILE_STATE;
                                bppcDemoInstanceData->miscLength = 0;
                                printf("Please enter the name of the file to be printed\n");
                            }
                            else
                            {
                                CsrUint8 * ucs2DocName;
                                char *pMimeMediaType = NULL;
                                CsrUint16 len = (CsrUint16)CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1;
                                if (len > 1)
                                {
                                    pMimeMediaType = CsrPmemAlloc(len);
                                    CsrStrNCpy(pMimeMediaType,bppcDemoInstanceData->mimeMediaType,CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1);
                                }
                                ucs2DocName = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->documentName);
                                CsrBtBppcSendDocumentReqSend(pMimeMediaType,
                                                       ucs2DocName,
                                                       NULL,
                                                       0,
                                                       bppcDemoInstanceData->jobId,
                                                       CSR_BT_JOB_BASED_TRANSFER_MODEL);
                                printf("\n\n Sending the print content\n");
                                printf("    8) To release the OBEX connection\n");
                                printf("\nHit <ESC> to quit program!\n\n");

                            }
                            break;
                        }

                    case '7':
                        {
                            if (bppcDemoInstanceData->recvStatusXmlPtr)
                            {
                                CsrPmemFree(bppcDemoInstanceData->recvStatusXmlPtr);
                                bppcDemoInstanceData->recvStatusXmlPtr = NULL;
                            }
                            if (bppcDemoInstanceData->xmlStatusTree)
                            {
                                CsrBtFreeXmlTree(bppcDemoInstanceData->xmlStatusTree);
                                bppcDemoInstanceData->xmlStatusTree = NULL;
                            }

                            CsrBtBppcGetJobAttributesReqSend(bppcDemoInstanceData->jobId,
                                                       TRUE,
                                                       TRUE,
                                                       TRUE,
                                                       TRUE,
                                                       TRUE,
                                                       FALSE);
                            break;
                        }

                    case '9':
                        {
                            CsrBtBppcCancelJobReqSend(bppcDemoInstanceData->jobId);
                            printf("Job cancel sent\n");
                            break;
                        }

                    case 'g':
                        {
                            if (bppcDemoInstanceData->getEventActive)
                            {
                                CsrBtBppcAbortReqSend(CSR_BT_BPPC_STATUS_CHANNEL);
                                printf("getEvent deactivated\n");
                            }
                            else
                            {
                                if (bppcDemoInstanceData->recvStatusXmlPtr)
                                {
                                    CsrPmemFree(bppcDemoInstanceData->recvStatusXmlPtr);
                                    bppcDemoInstanceData->recvStatusXmlPtr = NULL;
                                }
                                if (bppcDemoInstanceData->xmlStatusTree)
                                {
                                    CsrBtFreeXmlTree(bppcDemoInstanceData->xmlStatusTree);
                                    bppcDemoInstanceData->xmlStatusTree = NULL;
                                }
                                bppcDemoInstanceData->getEventActive = TRUE;
                                CsrBtBppcGetEventReqSend(bppcDemoInstanceData->jobId, FALSE);
                                printf("getEvent activated\n");
                            }
                            break;
                        }
                    case '8':
                        {
/*
                            system(CLS);
*/
                            printf("\n\n Releasing the OBEX connection --- please wait\n");
                            if (bppcDemoInstanceData->getEventActive)
                            {
                                CsrBtBppcAbortReqSend(CSR_BT_BPPC_STATUS_CHANNEL);
                                bppcDemoInstanceData->abortDisconnect = TRUE;
                            }
                            else
                            {
                                CsrBtBppcDisconnectReqSend(TRUE);
                            }
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
        case MENU_SELECT_TRANSFER_MODEL:
            {
                switch (theCharacter)
                {
                    case 'a':
                    case 'A':
                        {
/*
                            system(CLS);
*/
                            simpleModelDocumentFormatInterpreter(bppcDemoInstanceData);
                            simpleModelmediaTypesInterpreter(bppcDemoInstanceData);
                            simpleModelxhtmlPrintImageInterpreter(bppcDemoInstanceData);
                            bppcDemoInstanceData->transferModel = CSR_BT_SIMPLE_PUSH_TRANSFER_MODEL;
                            bppcDemoInstanceData->menuState = MENU_CONNECTED_STATE;
                            showMenu(bppcDemoInstanceData);
                            break;
                        }

                    case 'b':
                    case 'B':
                        {
                            if (bppcDemoInstanceData->documentFormatsSupported)
                            {
                                CsrPmemFree(bppcDemoInstanceData->documentFormatsSupported);
                                bppcDemoInstanceData->documentFormatsSupported = NULL;
                            }
                            if (bppcDemoInstanceData->mediaTypesSupported)
                            {
                                CsrPmemFree(bppcDemoInstanceData->mediaTypesSupported);
                                bppcDemoInstanceData->mediaTypesSupported = NULL;
                            }
                            if (bppcDemoInstanceData->xhtmlPrintImageFormats)
                            {
                                CsrPmemFree(bppcDemoInstanceData->xhtmlPrintImageFormats);
                                bppcDemoInstanceData->xhtmlPrintImageFormats = NULL;
                            }
                            bppcDemoInstanceData->transferModel = CSR_BT_JOB_BASED_TRANSFER_MODEL;
                            bppcDemoInstanceData->menuState = MENU_SELECT_ATTRIBUTE_STATE;
/*
                            system(CLS);
*/
                            showMenu(bppcDemoInstanceData);
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
        case MENU_JOB_CREATE_CONFIG_STATE :
            {
                bppcDemoInstanceData->unsupportedFeature = FALSE;
                switch (theCharacter)
                {
                    case 'c':
                    case 'C':
                        {
                            bppcDemoInstanceData->defaultCreateJobParam = FALSE;
#ifdef XML_INTERPRET
/*
                            system(CLS);
*/
                            bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_MEDIA_SIZE_STATE;
                            if (bppcDemoInstanceData->xmlTree                &&
                                bppcDemoInstanceData->recvXmlPtr                &&
                                bppcDemoInstanceData->numOfDiffMediaSizes > 0)
                            {
                                xmlMediaSizesHandler(bppcDemoInstanceData);
                            }
                            else
                            {
                                keyboardSelectMediaSizeStateHandler(bppcDemoInstanceData, '0');
                            }
#else
                            bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_DOCUMENT_FORMAT_STATE;
                            keyboardSelectDocumentStateHandler(bppcDemoInstanceData, '0');
#endif
                            break;
                        }
                    case 'd':
                    case 'D':
                        {
#ifdef XML_INTERPRET
/*
                            system(CLS);
*/
                            bppcDemoInstanceData->defaultCreateJobParam = TRUE;;
                            keyboardSelectMediaTypeStateHandler(bppcDemoInstanceData, '0');
#else
                            CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_XHTML_1_00);
                            CsrStrCpy(bppcDemoInstanceData->documentName, FILE_XHTML_1_00);
                            bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                            CsrBtBppcCreateJobReqSend(FALSE,
                                                0,
                                                0,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                FALSE);
                            printf("Creating print job, please wait ...\n");
#endif
                            break;
                        }
#ifdef XML_INTERPRET
                    case 'e':
                    case 'E':
                        {
                            bppcDemoInstanceData->defaultCreateJobParam = FALSE;
                            bppcDemoInstanceData->unsupportedFeature = TRUE;
/*
                            system(CLS);
*/
                            bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_MEDIA_SIZE_STATE;
                            if (bppcDemoInstanceData->xmlTree                &&
                                bppcDemoInstanceData->recvXmlPtr                &&
                                bppcDemoInstanceData->numOfDiffMediaSizes > 0)
                            {
                                xmlMediaSizesHandler(bppcDemoInstanceData);
                            }
                            else
                            {
                                keyboardSelectMediaSizeStateHandler(bppcDemoInstanceData, '0');
                            }
                            break;
                        }
#endif
                    default:
                        {
                            bppcDemoInstanceData->defaultCreateJobParam = FALSE;
                            bppcDemoInstanceData->unsupportedFeature = FALSE;
                            printf("####### default in keybPrim handler %x,\n", theCharacter);
                            break;
                        }
                }
                break;
            }
        case MENU_SELECT_ATTRIBUTE_STATE:
            {
                switch (theCharacter)
                {
                    case '0':
                        {
                            bppcDemoInstanceData->menuState = MENU_CONNECTED_STATE;
                            if (bppcDemoInstanceData->recvXmlPtr)
                            {
                                CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
                                bppcDemoInstanceData->recvXmlPtr = NULL;
                            }
                            if (bppcDemoInstanceData->xmlTree)
                            {
                                CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
                                bppcDemoInstanceData->xmlTree = NULL;
                            }


                            CsrBtBppcGetPrinterAttributesReqSend(TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           TRUE,
                                                           FALSE);
                            break;
                        }
                    case '1':
                        {
                            CsrUint8 i;

                            for (i = 0; i < 19; i++)
                            {
                                bppcDemoInstanceData->attributesToRetrieve[i] = FALSE;
                            }
                            bppcDemoInstanceData->attributeCount = 0;
                            bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_ATTRIBUTE_STATE;
/*
                            system(CLS);
*/
                            printf("    Retrieve the pageHeight Attribute select (y)es or (n)o, \n");
                            break;
                        }
                    case '8':
                        {
/*
                            system(CLS);
*/
                            printf("\n\n Releasing the OBEX connection\n");
                            if (bppcDemoInstanceData->getEventActive)
                            {
                                CsrBtBppcAbortReqSend(CSR_BT_BPPC_STATUS_CHANNEL);
                                bppcDemoInstanceData->abortDisconnect = TRUE;
                            }
                            else
                            {
                                CsrBtBppcDisconnectReqSend(TRUE);
                            }
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
    }
}

static void keyboardSelectAddressStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    switch(theCharacter)
    {
        case RETURN_KEY :
            {
                bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
                if (bppcDemoInstanceData->addressLength == 12)
                {
                    CsrUint16 nap = 0;
                    CsrUint8 uap = 0;

                    bppcDemoInstanceData->deviceAddr.lap = 0;

                    printf("\n1 %c\n", bppcDemoInstanceData->character);
                    sscanf(bppcDemoInstanceData->address, "%04X%02X%06X",
                           &nap,
                           &uap,
                           &bppcDemoInstanceData->deviceAddr.lap);
                    bppcDemoInstanceData->deviceAddr.nap = nap;
                    bppcDemoInstanceData->deviceAddr.uap = (CsrUint8) uap;
                    scAppSetBondingAddr(&bppcDemoInstanceData->scInstData,
                                        bppcDemoInstanceData->deviceAddr);
                }

                if (bppcDemoInstanceData->menuState == MENU_SEARCH_STATE)
                {
                    bppcDemoInstanceData->menuState = MENU_IDLE_STATE;
/*
                    system(CLS);
*/
                    showMenu(bppcDemoInstanceData);
                }
                else
                {
/*
                    system(CLS);
*/
                    if (bppcDemoInstanceData->character == '3')
                    {
                        printf("\n\nInitiates an OBEX connection against %04X %02X %06X\n",
                               bppcDemoInstanceData->deviceAddr.nap,
                               bppcDemoInstanceData->deviceAddr.uap,
                               bppcDemoInstanceData->deviceAddr.lap);

                        CsrBtBppcConnectReqSend(bppcDemoInstanceData->CsrSchedQid,
                                                MAX_PACKET_SIZE_TO_RETRIEVE,
                                                bppcDemoInstanceData->deviceAddr,
                                                0, /*legth*/
                                                0, /*count*/
                                                0, /*windowSize*/
                                                TRUE); /*enableSrm*/

                        bppcDemoInstanceData->cancelConnect = TRUE;
                        printf("\n\n    8) To cancel the OBEX connection request\n");
                        printf("\nHit <ESC> to quit program!\n\n");
                    }
                    else
                    {
                        CsrBtScBondReqSend(bppcDemoInstanceData->CsrSchedQid,
                                      bppcDemoInstanceData->deviceAddr);

                        printf("Initiating bonding with selected device %04X %02X %06X. Please wait .\n",
                               bppcDemoInstanceData->deviceAddr.nap,
                               bppcDemoInstanceData->deviceAddr.uap,
                               bppcDemoInstanceData->deviceAddr.lap);
                        break;
                    }
                }
                break;
            }
        case BACKSPACE_KEY :
            {
                if(bppcDemoInstanceData->addressLength > 0)
                {
                    bppcDemoInstanceData->addressLength--;
                    printf("\b \b");
                }
                break;
            }
        default :
            {
                if(bppcDemoInstanceData->addressLength <= 11)
                {
                    bppcDemoInstanceData->address[bppcDemoInstanceData->addressLength] = theCharacter;
                    bppcDemoInstanceData->addressLength++;
                    printf("%c", theCharacter);
                }
                break;
            }
    }
}

static void keyboardAuthenticateStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {
        if (!bppcDemoInstanceData->authenticateLength)
        {
            CsrBtBppcAuthenticateResSend(0, NULL, CsrStrDup("CSR-BPPC"));
        }
        else
        {
            CsrBtBppcAuthenticateResSend(bppcDemoInstanceData->authenticateLength,
                                    (CsrUint8*)CsrStrDup(bppcDemoInstanceData->authenticatePassword),
                                   CsrStrDup("CSR-BPPC"));
        }

        bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
/*
        system(CLS);
*/
        showMenu(bppcDemoInstanceData);
    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (bppcDemoInstanceData->authenticateLength > 0)
        {
            bppcDemoInstanceData->authenticateLength--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (bppcDemoInstanceData->authenticateLength < 16)
        {
            bppcDemoInstanceData->authenticatePassword[bppcDemoInstanceData->authenticateLength++] = theCharacter;
            printf("%c", theCharacter);
        }
        else
        {
            printf("The maximum number of character is reach. Please press the BACKSPACE_KEY or the RETURN KEY\n");
        }
    }
}

static void keyboardSelectMediaSizeStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    CsrUint8 index=0;

    if (theCharacter >= '0' && theCharacter <='9')
    {
        index = theCharacter - '0';
    }
    else if (theCharacter >= 'a' && theCharacter <='z')
    {
        index = theCharacter - 'a' + 10;
    }
    else if (theCharacter >= 'A' && theCharacter <='Z')
    {
        index = theCharacter - 'A' + 10;
    }

/*
    system(CLS);
*/
    bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_MEDIA_TYPE_STATE;
    bppcDemoInstanceData->numOfDiffMediaSizes = index;

    if (bppcDemoInstanceData->xmlTree        &&
        bppcDemoInstanceData->recvXmlPtr        &&
        bppcDemoInstanceData->numOfDiffMediaTypes > 0)
    {
        xmlMediaTypesHandler(bppcDemoInstanceData);
    }
    else
    {
        keyboardSelectMediaTypeStateHandler(bppcDemoInstanceData, '0');
    }
}

static void keyboardSelectMediaTypeStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    CsrUint8 index=0;

    if (theCharacter >= '0' && theCharacter <='9')
    {
        index = theCharacter - '0';
    }
    else if (theCharacter >= 'a' && theCharacter <='z')
    {
        index = theCharacter - 'a' + 10;
    }
    else if (theCharacter >= 'A' && theCharacter <='Z')
    {
        index = theCharacter - 'A' + 10;
    }

/*
    system(CLS);
*/
    bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_DOCUMENT_FORMAT_STATE;

    if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
    {
        bppcDemoInstanceData->numOfDiffMediaTypes = index;
    }

    if (bppcDemoInstanceData->documentFormat == 0)
    {
        keyboardSelectDocumentStateHandler(bppcDemoInstanceData, '0');
    }
    else
    {
        printf("    0) To send an unknown file from the BppcFiles folder \n");
        if ((bppcDemoInstanceData->documentFormat & XHTML95_MASK) == XHTML95_MASK)
        {
            printf("    1) To send the file in XHTML-Print version 0.95 format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & XHTML10_MASK) == XHTML10_MASK)
        {
            printf("    2) To send the file in XHTML-Print version 1.0 format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & TEXT_MASK) == TEXT_MASK)
        {
            printf("    3) To send the file in basic text format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & VCARD21_MASK) == VCARD21_MASK)
        {
            printf("    4) To send the file as a vCard:2.1\n");
        }
        if ((bppcDemoInstanceData->documentFormat & VCAL_MASK) == VCAL_MASK)
        {
            printf("    5) To send the file as a vCalendar:1.0\n");
        }
        if ((bppcDemoInstanceData->documentFormat & ICAL_MASK) == ICAL_MASK)
        {
            printf("    6) To send the file as a iCalendar:2.0 (alternate calendar layout)\n");
        }
        if ((bppcDemoInstanceData->documentFormat & POSTS2_MASK) == POSTS2_MASK)
        {
            printf("    7) To send the file in PostScript:2 format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & POSTS3_MASK) == POSTS3_MASK)
        {
            printf("    8) To send the file in PostScript:3 format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & PCL5E_MASK) == PCL5E_MASK)
        {
            printf("    9) To send the file in PCL:5E format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & PCL6_MASK) == PCL6_MASK)
        {
            printf("    a) To send the file in PCL:6 format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & PDF_MASK) == PDF_MASK)
        {
            printf("    b) To send the file in PDF format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & JPEG_MASK) == JPEG_MASK)
        {
            printf("    c) To send the file in JPEG format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & GIF_MASK) == GIF_MASK)
        {
            printf("    d) To send the file in GIF format\n");
        }
        if ((bppcDemoInstanceData->documentFormat & XHTML95_MASK) == XHTML95_MASK)
        {
            printf("    e) To send the file in XHTML-Print version 0.95 format including an image\n");
        }
        if ((bppcDemoInstanceData->documentFormat & XHTML10_MASK) == XHTML10_MASK)
        {
            printf("    f) To send the file in XHTML-Print version 1.0 format including an image\n");
        }
        if ((bppcDemoInstanceData->documentFormat & VCARD21_MASK) == VCARD21_MASK)
        {
            printf("    g) To send the file as a vCard:2.1 (specify number of cards per page)\n");
        }
        if ((bppcDemoInstanceData->documentFormat & VCARD30_MASK) == VCARD30_MASK)
        {
            printf("    h) To send the file as a vCard:3.0 (alternate card layout)\n");
        }
        if ((bppcDemoInstanceData->documentFormat & VCAL_MASK) == VCAL_MASK)
        {
            printf("    i) To send the file as a vCalendar:1.0 (specify number of objects per page)\n");
        }
        if ((bppcDemoInstanceData->documentFormat & XHTML10_MASK) == XHTML10_MASK)
        {
            printf("    j) To send the file in basic text format containing non-ISO-8859-1 chars\n");
        }
        if ((bppcDemoInstanceData->documentFormat & VMESSAGE11_MASK) == VMESSAGE11_MASK)
        {
            printf("    m) To send the file as a vMessage:1.1\n");
        }
        printf("\nHit <ESC> to quit program!\n\n");
    }
}

static void keyboardSelectDocumentStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{

    CsrBool multiItemsPerPage = FALSE;
/*
    system(CLS);
*/
    if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
    {
        bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_ORIENTATION_STATE;
    }

    switch(theCharacter)
    {
        case '1':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_XHTML_0_95);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_XHTML_0_95);
                break;
            }
        case '2':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_XHTML_1_00);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_XHTML_1_00);
                break;
            }
        case '3':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_TEXT);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_TEXT);
                break;
            }
        case '4':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_VCARD21);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_VCARD);
                break;
            }
        case '5':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_VCALENDAR);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_VCAL);
                break;
            }
        case '6':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_ICALENDAR);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_ICAL);
                break;
            }

        case '7':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_POST2);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_POST2);
                break;
            }
        case '8':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_POST3);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_POST3);
                break;
            }
        case '9':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_PCL5E);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_PCL5E);
                break;
            }
        case 'A':
        case 'a':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_PCL6);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_PCL6);
                break;
            }
        case 'B':
        case 'b':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_PDF);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_PDF);
                break;
            }
        case 'C':
        case 'c':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_JPEG);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_JPEG);
                break;
            }
        case 'D':
        case 'd':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_GIF);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_GIF);
                break;
            }
        case 'E':
        case 'e':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_XHTML_0_95);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_XHTML_0_95_IMG);
                break;
            }
        case 'F':
        case 'f':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_XHTML_1_00);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_XHTML_1_00_IMG);
                break;
            }
        case 'G':
        case 'g':
            {
                multiItemsPerPage = TRUE;
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_VCARD21);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_VCARD_MULTI);
/*
                system(CLS);
*/
                bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_VCARDS_PER_PAGE_STATE;
                printf("\n In total 12 vCards will be printed. Please select the number of cards to be printed per page:\n\n");
                printf("    1) One card per page\n");
                printf("    2) Two cards per page\n");
                printf("    3) Three cards per page\n");
                printf("    4) Four cards per page\n");
                printf("    8) Eight cards per page\n");
                printf("    c) All 12 cards on one page\n");
                break;
            }
        case 'H':
        case 'h':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_VCARD30);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_VCARD_30);
                break;
            }
        case 'I':
        case 'i':
            {
                multiItemsPerPage = TRUE;
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_VCALENDAR);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_VCAL_MULTI);
/*
                system(CLS);
*/
                bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_VCAL_OBJ_PER_PAGE_STATE;
                printf("\n Please select the format of the calendar objects to be printed:\n\n");
                printf("    1) Objects printed as single objects. One object per page\n");
                printf("    2) Objects printed as single objects. Two objects per page\n");
                printf("    3) Objects printed as single objects. Four objects per page\n");
                printf("    4) Objects printed in day-format. One day per page\n");
                printf("    5) Objects printed in day-format. Two days per page\n");
                printf("    6) Objects printed in day-format. Five days per page\n");
                printf("    7) Objects printed in day-format. Seven days per page\n");
                printf("    8) Objects printed in week-format. One week per page\n");
                printf("    9) Objects printed in week-format. Two weeks per page\n");
                printf("    a) Objects printed in week-format. Four weeks per page\n");
                printf("    b) Objects printed in month-format. One month per page\n");
                printf("    c) Objects printed in month-format. Three months per page\n");
                printf("    d) Objects printed in month-format. Six months per page\n");
                printf("    e) Objects printed in month-format. Twelve months per page\n");
                break;
            }
        case 'J':
        case 'j':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_TEXT);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_TEXT_NON_ISO);
                break;
            }
        case 'M':
        case 'm':
            {
                CsrStrCpy(bppcDemoInstanceData->mimeMediaType, MIME_VMESSAGE11);
                CsrStrCpy(bppcDemoInstanceData->documentName, FILE_VMESSAGE_11);
                break;
            }
        default :
            {
                CsrMemSet(bppcDemoInstanceData->mimeMediaType,
                       0,
                       sizeof(bppcDemoInstanceData->mimeMediaType));
                CsrMemSet(bppcDemoInstanceData->documentName,
                       0,
                       sizeof(bppcDemoInstanceData->documentName));
                break;
            }
    }

    if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
    {
        if (bppcDemoInstanceData->defaultCreateJobParam && multiItemsPerPage == FALSE)
        {
/*
            system(CLS);
*/
            bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
            CsrBtBppcCreateJobReqSend(FALSE,
                                0,
                                0,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                FALSE);
            printf("Creating print job, please wait ...\n");

        }
        else
        {
            if (bppcDemoInstanceData->orientation == 0 || bppcDemoInstanceData->orientation == PORTRAIT_MASK)
            {
                if (bppcDemoInstanceData->unsupportedFeature)
                {
                    printf("    0) To select portrait\n");
                    printf("    1) To select landscape\n");
                    printf("    2) To select reverse-landscape\n");
                    printf("    3) To select reverse-portrait\n");
                }
                else
                {
                    keyboardSelectOrientationStateHandler(bppcDemoInstanceData, '0');
                }
            }
            else
            {
                if (bppcDemoInstanceData->orientation == LANDSCAPE_MASK)
                {
                    if (bppcDemoInstanceData->unsupportedFeature)
                    {
                        printf("    0) To select portrait\n");
                        printf("    1) To select landscape\n");
                        printf("    2) To select reverse-landscape\n");
                        printf("    3) To select reverse-portrait\n");
                    }
                    else
                    {
                        keyboardSelectOrientationStateHandler(bppcDemoInstanceData, '1');
                    }
                }
                else if (bppcDemoInstanceData->orientation == REVERSE_LANDSCAPE_MASK)
                {
                    if (bppcDemoInstanceData->unsupportedFeature)
                    {
                        printf("    0) To select portrait\n");
                        printf("    1) To select landscape\n");
                        printf("    2) To select reverse-landscape\n");
                        printf("    3) To select reverse-portrait\n");                }
                    else
                    {
                        keyboardSelectOrientationStateHandler(bppcDemoInstanceData, '2');
                    }
                }
                else if (bppcDemoInstanceData->orientation == REVERSE_PORTRAIT_MASK)
                {
                    if (bppcDemoInstanceData->unsupportedFeature)
                    {
                        printf("    0) To select portrait\n");
                        printf("    1) To select landscape\n");
                        printf("    2) To select reverse-landscape\n");
                        printf("    3) To select reverse-portrait\n");
                    }
                    else
                    {
                        keyboardSelectOrientationStateHandler(bppcDemoInstanceData, '3');
                    }
                }
                else
                {
                    CsrUint8 count = 0;

                    if ((bppcDemoInstanceData->orientation & PORTRAIT_MASK) == PORTRAIT_MASK)
                    {
                        printf("    0) To select portrait\n");
                        count++;
                    }
                    if ((bppcDemoInstanceData->orientation & LANDSCAPE_MASK) == LANDSCAPE_MASK)
                    {
                        printf("    1) To select landscape\n");
                        count++;
                    }
                    if ((bppcDemoInstanceData->orientation & REVERSE_LANDSCAPE_MASK) == REVERSE_LANDSCAPE_MASK)
                    {
                        printf("    2) To select reverse-landscape\n");
                        count++;
                    }
                    if ((bppcDemoInstanceData->orientation & REVERSE_PORTRAIT_MASK) == REVERSE_PORTRAIT_MASK)
                    {
                        printf("    3) To select reverse-portrait\n");
                        count++;
                    }
                    if (count < 4 && bppcDemoInstanceData->unsupportedFeature)
                    {
/*
                        system(CLS);
*/
                        printf("    0) To select portrait\n");
                        printf("    1) To select landscape\n");
                        printf("    2) To select reverse-landscape\n");
                        printf("    3) To select reverse-portrait\n");
                    }
                }
            }
        }
    }
    else
    {
        if (bppcDemoInstanceData->documentName[0] == '\0')
        {
            bppcDemoInstanceData->keyboardState = KEYBOARD_TYPE_DOCUMENT_FILE_STATE;
            bppcDemoInstanceData->miscLength = 0;
            printf("Please enter the name of the file to be printed\n");
        }
        else if (multiItemsPerPage == FALSE)
        {
            CsrUint8 * ucs2DocName;
            char *pMimeMediaType = NULL;
            CsrUint16 len = (CsrUint16)CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1;
            if (len > 1)
            {
                pMimeMediaType = CsrPmemAlloc(len);
                CsrStrNCpy(pMimeMediaType,bppcDemoInstanceData->mimeMediaType,CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1);
            }
            bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;

            ucs2DocName = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->documentName);
            CsrBtBppcSendDocumentReqSend(pMimeMediaType,
                                   ucs2DocName,
                                   NULL,
                                   0,
                                   0,
                                   CSR_BT_SIMPLE_PUSH_TRANSFER_MODEL);
            printf("\n\n Sending the print content\n");
            printf("    8) To release the OBEX connection\n");
        }
        else
        {
            multiItemsPerPage = FALSE;
        }
    }
    printf("\nHit <ESC> to quit program!\n\n");
}

static void keyboardSelectOrientationStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
/*
    system(CLS);
*/
    bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_MAX_COPIES_STATE;
    bppcDemoInstanceData->orientation = theCharacter - '0';

    if (bppcDemoInstanceData->maxCopies == 0 || bppcDemoInstanceData->maxCopies == 1)
    {
        keyboardSelectMaxCopiesStateHandler(bppcDemoInstanceData, '1');
    }
    else
    {
        printf("Please enter number of copies. Maximum number is %d\n", bppcDemoInstanceData->maxCopies);
    }
}


static void keyboardSelectMaxCopiesStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
/*
    system(CLS);
*/
    bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_NUMBER_UP_STATE;
    bppcDemoInstanceData->maxCopies = theCharacter - '0';

    if (bppcDemoInstanceData->numberUp == 0 || bppcDemoInstanceData->numberUp == 1)
    {
        keyboardSelectNumberUpStateHandler(bppcDemoInstanceData, '1');
    }
    else
    {
        if (bppcDemoInstanceData->numberUp >= 8)
        {
            printf("    1) To select one page per side\n");
            printf("    2) To select two pages per side\n");
            printf("    4) To select four pages per side\n");
            printf("    8) To select eight pages per side\n");
        }
        else if (bppcDemoInstanceData->numberUp >= 4)
        {
            printf("    1) To select one page per side\n");
            printf("    2) To select two pages per side\n");
            printf("    4) To select four pages per side\n");
        }
        else
        {
            printf("    1) To select one page per side\n");
            printf("    2) To select two pages per side\n");
        }
        printf("\nHit <ESC> to quit program!\n\n");
    }
}

static void keyboardSelectNumberUpStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
/*
    system(CLS);
*/
    bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_SIDE_STATE;
    bppcDemoInstanceData->numberUp = theCharacter - '0';

    if (bppcDemoInstanceData->sides == 0 || bppcDemoInstanceData->sides == ONE_SIDED_MASK)
    {
        keyboardSelectSideStateHandler(bppcDemoInstanceData, '0');
    }
    else
    {
        if (bppcDemoInstanceData->sides == TWO_SIDED_LONG_MASK)
        {
            keyboardSelectSideStateHandler(bppcDemoInstanceData, '1');
        }
        else if (bppcDemoInstanceData->sides == TWO_SIDED_SHORT_MASK)
        {
            keyboardSelectSideStateHandler(bppcDemoInstanceData, '2');
        }
        else
        {
            if ((bppcDemoInstanceData->sides & ONE_SIDED_MASK) == ONE_SIDED_MASK)
            {
                printf("    0) To select one sided\n");
            }
            if ((bppcDemoInstanceData->sides & TWO_SIDED_LONG_MASK) == TWO_SIDED_LONG_MASK)
            {
                printf("    1) To select two sided long edge\n");
            }
            if ((bppcDemoInstanceData->sides & TWO_SIDED_SHORT_MASK) == TWO_SIDED_SHORT_MASK)
            {
                printf("    2) To select two sided short edge\n");
            }
            printf("\nHit <ESC> to quit program!\n\n");
        }
    }
}

static void createJobRequestHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrUint8 *mediaSize[100];
    CsrUint8 *mediaType[100];
    CsrUint8 *sides[100];
    CsrUint8 *orientation[100];
    CsrUint8 *printfQuality[100];

    CsrMemSet(mediaSize, 0, sizeof(mediaSize));
    CsrMemSet(mediaType, 0, sizeof(mediaType));
    CsrMemSet(sides, 0, sizeof(sides));
    CsrMemSet(orientation, 0, sizeof(orientation));
    CsrMemSet(printfQuality, 0, sizeof(printfQuality));

    if (bppcDemoInstanceData->xmlTree && bppcDemoInstanceData->recvXmlPtr)
    {
        if (bppcDemoInstanceData->numOfDiffMediaSizes > 0 ||
            bppcDemoInstanceData->numOfDiffMediaTypes > 0)
        {
            CsrBtRootElementType        * rootElement;
            CsrBtChildElementType    * childElement;

            rootElement = &(bppcDemoInstanceData->xmlTree->elementArray[0].rootElement);
            childElement = rootElement->firstChildElement;

            while (childElement)
            {
                if (!CsrBtStrICmp((char *) childElement->name, MEDIA_SIZES_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;
                    CsrUint16 i = 0;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        i++;
                        if (i == bppcDemoInstanceData->numOfDiffMediaSizes)
                        {
                            CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value) + 1;
                            if (length > 1)
                            {
                                CsrMemCpy(mediaSize, subChildElement->value, length);
                            }
                            break;
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, MEDIA_TYPES_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;
                    CsrUint16 i = 0;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        i++;
                        if (i == bppcDemoInstanceData->numOfDiffMediaTypes)
                        {
                            CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value) + 1;
                            if (length > 1)
                            {
                                CsrMemCpy(mediaType, subChildElement->value, length);
                            }
                            break;
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, PRINTER_NAME) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_LOCATION) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE_REASONS) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_OPERATOR) ||
                         !CsrBtStrICmp((char *) childElement->name, COLOR_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_WIDTH) ||
                         !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_HEIGHT) ||
                         !CsrBtStrICmp((char *) childElement->name, OPERATION_STATUS) ||
                         !CsrBtStrICmp((char *) childElement->name, JOB_ID) ||
                         !CsrBtStrICmp((char *) childElement->name, NUMBER_UP_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, MAX_COPIES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, SIDES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, DOCUMENT_FORMATS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, ORIENTATIONS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, IMAGE_FORMATS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINT_QUALITY_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, LOADED_MEDIUM) ||
                         !CsrBtStrICmp((char *) childElement->name, QUEUED_JOB_COUNT))
                {
                    childElement = childElement->nextSiblingElement;
                }
                else
                {
                    childElement = childElement->firstChildElement;
                }
            }
        }
    }

    switch(bppcDemoInstanceData->sides)
    {
        case 0:
            {
                CsrStrCpy((char*)sides, ONE_SIDED);
                break;
            }
        case 1:
            {
                CsrStrCpy((char*)sides, TWO_SIDED_LONG);
                break;
            }
        case 2:
            {
                CsrStrCpy((char*)sides, TWO_SIDED_SHORT);
                break;
            }
    }

    switch(bppcDemoInstanceData->orientation)
    {
        case 0:
            {
                CsrStrCpy((char*)orientation, ORI_PORTRAIT);
                break;
            }
        case 1:
            {
                CsrStrCpy((char*)orientation, ORI_LANDSCAPE);
                break;
            }
        case 2:
            {
                CsrStrCpy((char*)orientation, ORI_REV_LANDSCAPE);
                break;
            }
        case 3:
            {
                CsrStrCpy((char*)orientation, ORI_REV_PORTRAIT);
                break;
            }
    }

    switch(bppcDemoInstanceData->orientation)
    {
        case 0:
            {
                CsrStrCpy((char*)printfQuality, QUALITY_DRAFT);
                break;
            }
        case 1:
            {
                CsrStrCpy((char*)printfQuality, QUALITY_NORMAL);
                break;
            }
        case 2:
            {
                CsrStrCpy((char*)printfQuality, QUALITY_HIGH);
                break;
            }
    }

    bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
    CsrBtBppcCreateJobReqSend(FALSE,
                        bppcDemoInstanceData->maxCopies,
                        bppcDemoInstanceData->numberUp,
                        (CsrUint8*)sides,
                        (CsrUint8*)orientation,
                        (CsrUint8*)printfQuality,
                        (CsrUint8*)"csr",
                        (CsrUint8*)"bppcTest",
                        (CsrUint8*)bppcDemoInstanceData->mimeMediaType,
                        (CsrUint8*)mediaSize,
                        (CsrUint8*)mediaType,
                        FALSE);

    printf("Creating print job, please wait ...\n");
}

static void keyboardSelectSideStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
/*
    system(CLS);
*/
    bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_PRINT_QUALITY_STATE;
    bppcDemoInstanceData->sides = theCharacter - '0';

    if (bppcDemoInstanceData->printfQuality == 0)
    {
        createJobRequestHandler(bppcDemoInstanceData);
    }
    else
    {
        if (bppcDemoInstanceData->printfQuality == DRAFT_MASK)
        {
            bppcDemoInstanceData->printfQuality = DRAFT_MASK;
            createJobRequestHandler(bppcDemoInstanceData);
        }
        else if (bppcDemoInstanceData->printfQuality == NORMAL_MASK)
        {
            bppcDemoInstanceData->printfQuality = NORMAL_MASK;
            createJobRequestHandler(bppcDemoInstanceData);
        }
        else if (bppcDemoInstanceData->printfQuality == HIGH_MASK)
        {
            bppcDemoInstanceData->printfQuality = HIGH_MASK;
            createJobRequestHandler(bppcDemoInstanceData);
        }
        else
        {
            if ((bppcDemoInstanceData->printfQuality & DRAFT_MASK) == DRAFT_MASK)
            {
                printf("    0) To set the printf quality to Draft\n");
            }
            if ((bppcDemoInstanceData->printfQuality & NORMAL_MASK) == NORMAL_MASK)
            {
                printf("    1) To set the printf quality to Normal\n");
            }
            if ((bppcDemoInstanceData->printfQuality & HIGH_MASK) == HIGH_MASK)
            {
                printf("    2) To set the printf quality to High\n");
            }
            printf("\nHit <ESC> to quit program!\n\n");
        }
    }
}

static void keyboardSelectPrintQualityStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
/*
    system(CLS);
*/
    bppcDemoInstanceData->printfQuality = theCharacter - '0';
    createJobRequestHandler(bppcDemoInstanceData);
}

static void xmlDocumentSelectHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtRootElementType        * rootElement;
    CsrBtChildElementType    * childElement;

    rootElement = &(bppcDemoInstanceData->xmlTree->elementArray[0].rootElement);
    childElement = rootElement->firstChildElement;

    while (childElement)
    {
        if (!CsrBtStrICmp((char *) childElement->name, DOCUMENT_FORMATS_SUPPORTED))
        {
            CsrBtChildElementType    * subChildElement;
            CsrUint16 i = 0;
            subChildElement = childElement->firstChildElement;
            while (subChildElement)
            {
                CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value);
                i++;
                if (length > 0)
                {
                    printf(" Type %d and Enter) To set MIME Media Type to %s\n", i, subChildElement->value);
                }
                subChildElement = subChildElement->nextSiblingElement;
            }
            childElement = childElement->nextSiblingElement;
            return;
        }
        else if (!CsrBtStrICmp((char *) childElement->name, PRINTER_NAME) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_LOCATION) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE_REASONS) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_OPERATOR) ||
                 !CsrBtStrICmp((char *) childElement->name, COLOR_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_WIDTH) ||
                 !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_HEIGHT) ||
                 !CsrBtStrICmp((char *) childElement->name, OPERATION_STATUS) ||
                 !CsrBtStrICmp((char *) childElement->name, JOB_ID) ||
                 !CsrBtStrICmp((char *) childElement->name, NUMBER_UP_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, MAX_COPIES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, MEDIA_TYPES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, SIDES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, MEDIA_SIZES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, ORIENTATIONS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, IMAGE_FORMATS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINT_QUALITY_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, LOADED_MEDIUM) ||
                 !CsrBtStrICmp((char *) childElement->name, QUEUED_JOB_COUNT))
        {
            childElement = childElement->nextSiblingElement;
        }
        else
        {
            childElement = childElement->firstChildElement;
        }
    }
}

static void keyboardTypeDocumentFileStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {
/*
        system(CLS);
*/
        bppcDemoInstanceData->documentName[bppcDemoInstanceData->miscLength] = '\0';
        bppcDemoInstanceData->keyboardState = KEYBOARD_TYPE_MIME_TYPE_MEDIA_STATE;
        bppcDemoInstanceData->miscLength = 0;

        if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL    &&
            bppcDemoInstanceData->xmlTree                                    &&
            bppcDemoInstanceData->recvXmlPtr                                    &&
            bppcDemoInstanceData->documentFormat > 0)
        {
            bppcDemoInstanceData->keyboardState = KEYBOARD_SELECT_MIME_TYPE_MEDIA_STATE;
            CsrMemSet(bppcDemoInstanceData->selectedFormat, '\0', MAX_NUM_OF_DOC_FORMATS + 1);
            xmlDocumentSelectHandler(bppcDemoInstanceData);
        }
        else
        {
            bppcDemoInstanceData->keyboardState = KEYBOARD_TYPE_MIME_TYPE_MEDIA_STATE;
            printf("\nPlease enter the MIME Media Type of the file to be printed\n");
        }
    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (bppcDemoInstanceData->miscLength > 0)
        {
            bppcDemoInstanceData->miscLength--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (bppcDemoInstanceData->miscLength < CSR_BT_MAX_FRIENDLY_NAME_LEN)
        {
            bppcDemoInstanceData->documentName[bppcDemoInstanceData->miscLength++] = theCharacter;
            printf("%c", theCharacter);
        }
        else
        {
            printf("The maximum number of character is reach. Please press the BACKSPACE_KEY or the RETURN KEY\n");
        }
    }
}

static void keyboardTypeMimeTypeMediaStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {
        CsrUint8 * ucs2DocName;

        char *pMimeMediaType = NULL;
        CsrUint16 len = (CsrUint16)CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1;
        if (len > 1)
        {
            pMimeMediaType = CsrPmemAlloc(len);
            CsrStrNCpy(pMimeMediaType,bppcDemoInstanceData->mimeMediaType,CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1);
        }
        bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
        bppcDemoInstanceData->mimeMediaType[bppcDemoInstanceData->miscLength] = '\0';
        ucs2DocName = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->documentName);

        if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
        {
            CsrBtBppcSendDocumentReqSend(pMimeMediaType, ucs2DocName, NULL, 0, bppcDemoInstanceData->jobId, CSR_BT_JOB_BASED_TRANSFER_MODEL);
        }
        else
        {
            CsrBtBppcSendDocumentReqSend(pMimeMediaType, ucs2DocName, NULL, 0, 0, CSR_BT_SIMPLE_PUSH_TRANSFER_MODEL);
        }

        printf("\n\n Sending the print content\n");
        printf("    8) To release the OBEX connection\n");
        printf("\nHit <ESC> to quit program!\n\n");
    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (bppcDemoInstanceData->miscLength > 0)
        {
            bppcDemoInstanceData->miscLength--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (bppcDemoInstanceData->miscLength < CSR_BT_MAX_FRIENDLY_NAME_LEN)
        {
            bppcDemoInstanceData->mimeMediaType[bppcDemoInstanceData->miscLength++] = theCharacter;
            printf("%c", theCharacter);
        }
        else
        {
            printf("The maximum number of character is reach. Please press the BACKSPACE_KEY or the RETURN KEY\n");
        }
    }
}


static void printOutAttributeToSelect(CsrUint8 count)
{
/*
    system(CLS);
*/
    switch(count)
    {
        case 0:
            {
                printf("    Retrieve the pageHeight Attribute select (y)es or (n)o, \n");
                break;
            }
        case 1:
            {
                printf("    Retrieve the pageWidth Attribute select (y)es or (n)o, \n");
                break;
            }
        case 2:
            {
                printf("    Retrieve the color Attribute select (y)es or (n)o, \n");
                break;
            }
        case 3:
            {
                printf("    Retrieve the documentFormats Attribute select (y)es or (n)o, \n");
                break;
            }
        case 4:
            {
                printf("    Retrieve the imageFormats Attribute select (y)es or (n)o, \n");
                break;
            }
        case 5:
            {
                printf("    Retrieve the maxCopies Attribute select (y)es or (n)o, \n");
                break;
            }
        case 6:
            {
                printf("    Retrieve the mediaLoaded Attribute select (y)es or (n)o, \n");
                break;
            }
        case 7:
            {
                printf("    Retrieve the mediaSizes Attribute select (y)es or (n)o, \n");
                break;
            }
        case 8:
            {
                printf("    Retrieve the mediaTypes Attribute select (y)es or (n)o, \n");
                break;
            }
        case 9:
            {
                printf("    Retrieve the numberUp Attribute select (y)es or (n)o, \n");
                break;
            }
        case 10:
            {
                printf("    Retrieve the orientations Attribute select (y)es or (n)o, \n");
                break;
            }
        case 11:
            {
                printf("    Retrieve the currentOperator Attribute select (y)es or (n)o, \n");
                break;
            }
        case 12:
            {
                printf("    Retrieve the printerLocation Attribute select (y)es or (n)o, \n");
                break;
            }
        case 13:
            {
                printf("    Retrieve the printerName Attribute select (y)es or (n)o, \n");
                break;
            }
        case 14:
            {
                printf("    Retrieve the printerState Attribute select (y)es or (n)o, \n");
                break;
            }
        case 15:
            {
                printf("    Retrieve the printerReasons Attribute select (y)es or (n)o, \n");
                break;
            }
        case 16:
            {
                printf("    Retrieve the printQuality Attribute select (y)es or (n)o, \n");
                break;
            }
        case 17:
            {
                printf("    Retrieve the queuedJobCount Attribute select (y)es or (n)o, \n");
                break;
            }
        case 18:
            {
                printf("    Retrieve the sides Attribute select (y)es or (n)o, \n");
                break;
            }
    }
}

static void keyboardSelectVcalendarObjPerPageStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{

    CsrUint8 * ucs2DocName;
    CsrUint8 * ucs2DocTypeDependentInfo;
    CsrUint16 docTypeDependentInfoLength;
    char *formatString;
    char *pMimeMediaType = NULL;
    CsrUint16 len;

    switch (theCharacter)
    {
        case '1':
            {
                formatString =
                    "<CalendarFormat>single</CalendarFormat><CalendarFormatsPerPage>1</CalendarFormatsPerPage>";
                break;
            }
        case '2':
            {
                formatString =
                    "<CalendarFormat>single</CalendarFormat><CalendarFormatsPerPage>2</CalendarFormatsPerPage>";
                break;
            }
        case '3':
            {
                formatString =
                    "<CalendarFormat>single</CalendarFormat><CalendarFormatsPerPage>4</CalendarFormatsPerPage>";
                break;
            }
        case '4':
            {
                formatString =
                    "<CalendarFormat>daily</CalendarFormat><CalendarFormatsPerPage>1</CalendarFormatsPerPage>";
                break;
            }
        case '5':
            {
                formatString =
                    "<CalendarFormat>daily</CalendarFormat><CalendarFormatsPerPage>2</CalendarFormatsPerPage>";
                break;
            }
        case '6':
            {
                formatString =
                    "<CalendarFormat>daily</CalendarFormat><CalendarFormatsPerPage>5</CalendarFormatsPerPage>";
                break;
            }
        case '7':
            {
                formatString =
                    "<CalendarFormat>daily</CalendarFormat><CalendarFormatsPerPage>7</CalendarFormatsPerPage>";
                break;
            }
        case '8':
            {
                formatString =
                    "<CalendarFormat>weekly</CalendarFormat><CalendarFormatsPerPage>1</CalendarFormatsPerPage>";
                break;
            }
        case '9':
            {
                formatString =
                    "<CalendarFormat>weekly</CalendarFormat><CalendarFormatsPerPage>2</CalendarFormatsPerPage>";
                break;
            }
        case 'a':
        case 'A':
            {
                formatString =
                    "<CalendarFormat>weekly</CalendarFormat><CalendarFormatsPerPage>4</CalendarFormatsPerPage>";
                break;
            }
        case 'b':
        case 'B':
            {
                formatString =
                    "<CalendarFormat>monthly</CalendarFormat><CalendarFormatsPerPage>1</CalendarFormatsPerPage>";
                break;
            }
        case 'c':
        case 'C':
            {
                formatString =
                    "<CalendarFormat>monthly</CalendarFormat><CalendarFormatsPerPage>3</CalendarFormatsPerPage>";
                break;
            }
        case 'd':
        case 'D':
            {
                formatString =
                    "<CalendarFormat>monthly</CalendarFormat><CalendarFormatsPerPage>6</CalendarFormatsPerPage>";
                break;
            }
        case 'e':
        case 'E':
            {
                formatString =
                    "<CalendarFormat>monthly</CalendarFormat><CalendarFormatsPerPage>12</CalendarFormatsPerPage>";
                break;
            }
        default:
            {
                printf("\n\n The value selected is not valid. Printing all calendar objects (one month per page\n");
                formatString =
                    "<CalendarFormat>monthly</CalendarFormat><CalendarFormatsPerPage>1</CalendarFormatsPerPage>";
                break;
            }
    }

    len = (CsrUint16)CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1;
    if (len > 1)
    {
        pMimeMediaType = CsrPmemAlloc(len);
        CsrStrNCpy(pMimeMediaType,bppcDemoInstanceData->mimeMediaType,CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1);
    }
    CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, formatString);
    docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)formatString);


    bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;

    ucs2DocName = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->documentName);
    ucs2DocTypeDependentInfo = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->docTypeDependentInfo);
    CsrBtBppcSendDocumentReqSend(pMimeMediaType, ucs2DocName, ucs2DocTypeDependentInfo, docTypeDependentInfoLength, 0, CSR_BT_SIMPLE_PUSH_TRANSFER_MODEL);

    printf("\n\n Sending the print content\n");
    printf("    8) To release the OBEX connection\n");
    printf("\nHit <ESC> to quit program!\n\n");
}
static void keyboardSelectVcardsPerPageStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{

    CsrUint8 * ucs2DocName;
    CsrUint8 * ucs2DocTypeDependentInfo;
    CsrUint16 docTypeDependentInfoLength;
    char *pMimeMediaType = NULL;
    CsrUint16 len;

    switch (theCharacter)
    {
        case '1':
            {
                CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, "<CardsPerPage>1</CardsPerPage>");
                docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)"<CardsPerPage>1</CardsPerPage>");
                break;
            }
        case '2':
            {
                CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, "<CardsPerPage>2</CardsPerPage>");
                docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)"<CardsPerPage>2</CardsPerPage>");
                break;
            }
        case '3':
            {
                CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, "<CardsPerPage>3</CardsPerPage>");
                docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)"<CardsPerPage>3</CardsPerPage>");
                break;
            }
        case '4':
            {
                CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, "<CardsPerPage>4</CardsPerPage>");
                docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)"<CardsPerPage>4</CardsPerPage>");
                break;
            }
        case '8':
            {
                CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, "<CardsPerPage>8</CardsPerPage>");
                docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)"<CardsPerPage>8</CardsPerPage>");
                break;
            }
        case 'c':
        case 'C':
            {
                CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, "<CardsPerPage>12</CardsPerPage>");
                docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)"<CardsPerPage>12</CardsPerPage>");
                break;
            }
        default:
            {
                printf("\n\n The value selected is not valid. Printing all 12 cards on one page\n");
                CsrStrCpy(bppcDemoInstanceData->docTypeDependentInfo, "<CardsPerPage>12</CardsPerPage>");
                docTypeDependentInfoLength = (CsrUint16)CsrStrLen((char*)"<CardsPerPage>12</CardsPerPage>");
                break;
            }
    }

    bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;

    ucs2DocName = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->documentName);
    ucs2DocTypeDependentInfo = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->docTypeDependentInfo);

    len = (CsrUint16)CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1;
    if (len > 1)
    {
        pMimeMediaType = CsrPmemAlloc(len);
        CsrStrNCpy(pMimeMediaType,bppcDemoInstanceData->mimeMediaType,CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1);
    }

    if (bppcDemoInstanceData->transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
    {
        CsrBtBppcSendDocumentReqSend(pMimeMediaType, ucs2DocName, ucs2DocTypeDependentInfo, docTypeDependentInfoLength, 0, CSR_BT_JOB_BASED_TRANSFER_MODEL);
    }
    else
    {
        CsrBtBppcSendDocumentReqSend(pMimeMediaType, ucs2DocName, ucs2DocTypeDependentInfo, docTypeDependentInfoLength, 0, CSR_BT_SIMPLE_PUSH_TRANSFER_MODEL);
    }
    printf("\n\n Sending the print content\n");
    printf("    8) To release the OBEX connection\n");
    printf("\nHit <ESC> to quit program!\n\n");
}

static void keyboardSelectAttributeStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{

    switch (theCharacter)
    {
        case 'y':
        case 'Y':
            {
                bppcDemoInstanceData->attributesToRetrieve[bppcDemoInstanceData->attributeCount] = TRUE;
                bppcDemoInstanceData->attributeCount++;
                break;
            }
        case 'n':
        case 'N':
            {
                bppcDemoInstanceData->attributeCount++;
                break;
            }
    }

    if (bppcDemoInstanceData->attributeCount > 18)
    {
        bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;
        if (bppcDemoInstanceData->recvXmlPtr)
        {
            CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
            bppcDemoInstanceData->recvXmlPtr = NULL;
        }
        if (bppcDemoInstanceData->xmlTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
            bppcDemoInstanceData->xmlTree = NULL;
        }
        bppcDemoInstanceData->menuState = MENU_CONNECTED_STATE;
        CsrBtBppcGetPrinterAttributesReqSend(    bppcDemoInstanceData->attributesToRetrieve[0],
                                           bppcDemoInstanceData->attributesToRetrieve[1],
                                           bppcDemoInstanceData->attributesToRetrieve[2],
                                           bppcDemoInstanceData->attributesToRetrieve[3],
                                           bppcDemoInstanceData->attributesToRetrieve[4],
                                           bppcDemoInstanceData->attributesToRetrieve[5],
                                           bppcDemoInstanceData->attributesToRetrieve[6],
                                           bppcDemoInstanceData->attributesToRetrieve[7],
                                           bppcDemoInstanceData->attributesToRetrieve[8],
                                           bppcDemoInstanceData->attributesToRetrieve[9],
                                           bppcDemoInstanceData->attributesToRetrieve[10],
                                           bppcDemoInstanceData->attributesToRetrieve[11],
                                           bppcDemoInstanceData->attributesToRetrieve[12],
                                           bppcDemoInstanceData->attributesToRetrieve[13],
                                           bppcDemoInstanceData->attributesToRetrieve[14],
                                           bppcDemoInstanceData->attributesToRetrieve[15],
                                           bppcDemoInstanceData->attributesToRetrieve[16],
                                           bppcDemoInstanceData->attributesToRetrieve[17],
                                           bppcDemoInstanceData->attributesToRetrieve[18],
                                           FALSE);
    }
    else
    {
        printOutAttributeToSelect(bppcDemoInstanceData->attributeCount);
    }
}

static void keyboardSelectMimeTypeMediaStateHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {

        CsrUint8    * ucs2DocName;
        CsrBool        flag = FALSE;
        char *pMimeMediaType = NULL;
        CsrUint16 len;

        bppcDemoInstanceData->selectedFormat[bppcDemoInstanceData->miscLength] = '\0';
        bppcDemoInstanceData->mimeMediaType[bppcDemoInstanceData->miscLength] = '\0';
        sscanf((char*)bppcDemoInstanceData->selectedFormat,
               "%hu",
               &bppcDemoInstanceData->numOfDiffDocumentFormats);

        printf("selected %d\n", bppcDemoInstanceData->numOfDiffDocumentFormats);
        bppcDemoInstanceData->keyboardState = KEYBOARD_MENU_STATE;


        if (bppcDemoInstanceData->xmlTree && bppcDemoInstanceData->recvXmlPtr && bppcDemoInstanceData->numOfDiffDocumentFormats > 0)
        {
            CsrBtRootElementType        * rootElement;
            CsrBtChildElementType    * childElement;

            rootElement = &(bppcDemoInstanceData->xmlTree->elementArray[0].rootElement);
            childElement = rootElement->firstChildElement;

            while (childElement)
            {
                if (!CsrBtStrICmp((char *) childElement->name, DOCUMENT_FORMATS_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;
                    CsrUint16 i = 0;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        i++;
                        if (i == bppcDemoInstanceData->numOfDiffDocumentFormats)
                        {
                            CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value) + 1;
                            if (length > 1)
                            {
                                flag = TRUE;
                                CsrMemCpy(bppcDemoInstanceData->mimeMediaType, subChildElement->value, length);
                                bppcDemoInstanceData->mimeMediaType[length] = '\0';
                            }
                            break;
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, PRINTER_NAME) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_LOCATION) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE_REASONS) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINTER_OPERATOR) ||
                         !CsrBtStrICmp((char *) childElement->name, COLOR_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_WIDTH) ||
                         !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_HEIGHT) ||
                         !CsrBtStrICmp((char *) childElement->name, OPERATION_STATUS) ||
                         !CsrBtStrICmp((char *) childElement->name, JOB_ID) ||
                         !CsrBtStrICmp((char *) childElement->name, NUMBER_UP_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, MAX_COPIES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, SIDES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, MEDIA_SIZES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, MEDIA_TYPES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, ORIENTATIONS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, IMAGE_FORMATS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINT_QUALITY_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, LOADED_MEDIUM) ||
                         !CsrBtStrICmp((char *) childElement->name, QUEUED_JOB_COUNT))
                {
                    childElement = childElement->nextSiblingElement;
                }
                else
                {
                    childElement = childElement->firstChildElement;
                }
            }
        }
        if (!flag)
        {
            CsrMemSet(bppcDemoInstanceData->mimeMediaType,
                   0,
                   sizeof(bppcDemoInstanceData->mimeMediaType));
        }
        ucs2DocName = CsrUtf82Ucs2ByteString((CsrUint8*)bppcDemoInstanceData->documentName);
        len = (CsrUint16)CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1;
        if (len > 1)
        {
            pMimeMediaType = CsrPmemAlloc(len);
            CsrStrNCpy(pMimeMediaType,bppcDemoInstanceData->mimeMediaType,CsrStrLen(bppcDemoInstanceData->mimeMediaType) + 1);
        }
        CsrBtBppcSendDocumentReqSend(pMimeMediaType, ucs2DocName, NULL, 0, bppcDemoInstanceData->jobId, CSR_BT_JOB_BASED_TRANSFER_MODEL);

        printf("\n\n Sending the print content\n");
        printf("    8) To release the OBEX connection\n");
        printf("\nHit <ESC> to quit program!\n\n");

    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (bppcDemoInstanceData->miscLength > 0)
        {
            bppcDemoInstanceData->miscLength--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (bppcDemoInstanceData->miscLength < MAX_NUM_OF_DOC_FORMATS)
        {
            bppcDemoInstanceData->selectedFormat[bppcDemoInstanceData->miscLength++] = theCharacter;
            printf("%c", theCharacter);
        }
        else
        {
            printf("The maximum number of character is reach. Please press the BACKSPACE_KEY or the RETURN KEY\n");
        }
    }
}

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

static void printerAttributeResultHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 * object, CsrUint16 length)
{
    if (bppcDemoInstanceData->recvXmlPtr == NULL)
    {
        /* no data is currently stored from a previous command */
        bppcDemoInstanceData->recvXmlPtr = (CsrUint8 *) CsrPmemAlloc(length);
        bppcDemoInstanceData->recvXmlSize = length;
        CsrMemCpy(bppcDemoInstanceData->recvXmlPtr, object, length);
    }
    else
    {
        /* data is already received. Allocate space for the new data and append */
        CsrUint32    currentXmlSize;
        char        *p;

        currentXmlSize = bppcDemoInstanceData->recvXmlSize;
        bppcDemoInstanceData->recvXmlSize += length;
        p = (char*) CsrPmemAlloc(bppcDemoInstanceData->recvXmlSize);

        /* copy existing data */
        CsrMemCpy(p, bppcDemoInstanceData->recvXmlPtr, currentXmlSize);

        /* then append new received data */
        CsrMemCpy((p + currentXmlSize), object, length);

        /* data from object copied so free it */
        CsrPmemFree(bppcDemoInstanceData->recvXmlPtr);
        bppcDemoInstanceData->recvXmlPtr = (CsrUint8*)p;
    }
}

static void jobAttributeResultHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 * object, CsrUint16 length)
{
    if (bppcDemoInstanceData->recvStatusXmlPtr == NULL)
    {
        /* no data is currently stored from a previous command */
        bppcDemoInstanceData->recvStatusXmlPtr = (CsrUint8 *) CsrPmemAlloc(length);
        bppcDemoInstanceData->recvStatusXmlSize = length;
        CsrMemCpy(bppcDemoInstanceData->recvStatusXmlPtr, object, length - 1);
    }
    else
    {
        /* data is already received. Allocate space for the new data and append */
        CsrUint32    currentXmlSize;
        char        *p;

        currentXmlSize = bppcDemoInstanceData->recvStatusXmlSize;
        bppcDemoInstanceData->recvStatusXmlSize += length;
        p = (char*)CsrPmemAlloc(bppcDemoInstanceData->recvStatusXmlSize);

        /* copy existing data */
        CsrMemCpy(p, bppcDemoInstanceData->recvStatusXmlPtr, currentXmlSize);

        /* then append new received data */
        CsrMemCpy((p + currentXmlSize), object, length);

        /* data from object copied so free it */
        CsrPmemFree(bppcDemoInstanceData->recvStatusXmlPtr);
        bppcDemoInstanceData->recvStatusXmlPtr = (CsrUint8*)p;
    }
}

static CsrBool findEnvelopeEnd(CsrUint8 *object, CsrUint16 length)
{
    CsrUint16 i;

    for (i = 0 ; i<= length ; i++)
    {
        if ((i + CsrStrLen((char*)ENVELOPE_END)) > length)
        {
            return FALSE;
        }
        if (!CsrMemCmp(&object[i], (CsrUint8 *) ENVELOPE_END, CsrStrLen((char*)ENVELOPE_END)))
        {
            return TRUE;
        }
    }
    return FALSE;
}


static void getEventResultHandler(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint8 * object, CsrUint16 length)
{
    if (bppcDemoInstanceData->recvStatusXmlPtr == NULL)
    {
        /* no data is currently stored from a previous command */
        bppcDemoInstanceData->recvStatusXmlPtr = (CsrUint8 *) CsrPmemAlloc(length);
        bppcDemoInstanceData->recvStatusXmlSize = length;
        CsrMemCpy(bppcDemoInstanceData->recvStatusXmlPtr, object, length);
        if (findEnvelopeEnd(object,  length))
        {
            xmlStatusChannelInterpreterHandler(bppcDemoInstanceData);
            CsrPmemFree(bppcDemoInstanceData->recvStatusXmlPtr);
            bppcDemoInstanceData->recvStatusXmlSize = 0;
            bppcDemoInstanceData->recvStatusXmlPtr = NULL;
        }
    }
    else
    {
        /* data is already received. Allocate space for the new data and append */
        CsrUint32    currentXmlSize;
        char        *p;

        currentXmlSize = bppcDemoInstanceData->recvStatusXmlSize;
        bppcDemoInstanceData->recvStatusXmlSize += length;
        p = (char*)CsrPmemAlloc(bppcDemoInstanceData->recvStatusXmlSize);

        /* copy existing data */
        CsrMemCpy(p, bppcDemoInstanceData->recvStatusXmlPtr, currentXmlSize);

        /* then append new received data */
        CsrMemCpy((p + currentXmlSize), object, length);

        /* data from object copied so free it */
        CsrPmemFree(bppcDemoInstanceData->recvStatusXmlPtr);
        bppcDemoInstanceData->recvStatusXmlPtr = (CsrUint8*)p;
        if (findEnvelopeEnd(bppcDemoInstanceData->recvStatusXmlPtr,  bppcDemoInstanceData->recvStatusXmlSize))
        {
            xmlStatusChannelInterpreterHandler(bppcDemoInstanceData);
            CsrPmemFree(bppcDemoInstanceData->recvStatusXmlPtr);
            bppcDemoInstanceData->recvStatusXmlSize = 0;
            bppcDemoInstanceData->recvStatusXmlPtr = NULL;
        }

    }
}


static void xmlMediaSizesHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    CsrBtRootElementType        * rootElement;
    CsrBtChildElementType    * childElement;

    rootElement = &(bppcDemoInstanceData->xmlTree->elementArray[0].rootElement);
    childElement = rootElement->firstChildElement;

    while (childElement)
    {
        if (!CsrBtStrICmp((char *) childElement->name, MEDIA_SIZES_SUPPORTED))
        {
            CsrBtChildElementType    * subChildElement;
            CsrUint16 i = 0;
            subChildElement = childElement->firstChildElement;
            while (subChildElement)
            {
                CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value);
                i++;
                if (length > 0)
                {
                    if (i <= 9)
                    {
                        printf("    %d) To set the media size to %s\n", i, subChildElement->value);
                    }
                    else
                    {
                        CsrUint8 i2 = 'a' + i - 10;
                        printf("    %c) To set the media size to %s\n", i2, subChildElement->value);
                    }
                }
                subChildElement = subChildElement->nextSiblingElement;
            }
            printf("\nHit <ESC> to quit program!\n\n");
            childElement = childElement->nextSiblingElement;
            return;
        }
        else if (!CsrBtStrICmp((char *) childElement->name, PRINTER_NAME) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_LOCATION) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE_REASONS) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_OPERATOR) ||
                 !CsrBtStrICmp((char *) childElement->name, COLOR_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_WIDTH) ||
                 !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_HEIGHT) ||
                 !CsrBtStrICmp((char *) childElement->name, OPERATION_STATUS) ||
                 !CsrBtStrICmp((char *) childElement->name, JOB_ID) ||
                 !CsrBtStrICmp((char *) childElement->name, NUMBER_UP_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, MAX_COPIES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, MEDIA_TYPES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, SIDES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, DOCUMENT_FORMATS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, ORIENTATIONS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, IMAGE_FORMATS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINT_QUALITY_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, LOADED_MEDIUM) ||
                 !CsrBtStrICmp((char *) childElement->name, QUEUED_JOB_COUNT))
        {
            childElement = childElement->nextSiblingElement;
        }
        else
        {
            childElement = childElement->firstChildElement;
        }
    }
}

static void xmlMediaTypesHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)

{
    CsrBtRootElementType        * rootElement;
    CsrBtChildElementType    * childElement;

    rootElement = &(bppcDemoInstanceData->xmlTree->elementArray[0].rootElement);
    childElement = rootElement->firstChildElement;

    while (childElement)
    {
        if (!CsrBtStrICmp((char *) childElement->name, MEDIA_TYPES_SUPPORTED))
        {
            CsrBtChildElementType    * subChildElement;
            CsrUint16 i = 0;
            subChildElement = childElement->firstChildElement;

            while (subChildElement)
            {
                CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value);
                i++;
                if (length > 0)
                {
                    if (i <= 9)
                    {
                        printf("    %d) To set the media type to %s\n", i, subChildElement->value);
                    }
                    else
                    {
                        CsrUint8 i2 = 'a' + i - 10;
                        printf("    %c) To set the media type to %s\n", i2, subChildElement->value);
                    }
                }
                subChildElement = subChildElement->nextSiblingElement;
            }
            printf("\nHit <ESC> to quit program!\n\n");
            return;
        }
        else if (!CsrBtStrICmp((char *) childElement->name, PRINTER_NAME) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_LOCATION) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE_REASONS) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINTER_OPERATOR) ||
                 !CsrBtStrICmp((char *) childElement->name, COLOR_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_WIDTH) ||
                 !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_HEIGHT) ||
                 !CsrBtStrICmp((char *) childElement->name, OPERATION_STATUS) ||
                 !CsrBtStrICmp((char *) childElement->name, JOB_ID) ||
                 !CsrBtStrICmp((char *) childElement->name, NUMBER_UP_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, MAX_COPIES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, MEDIA_SIZES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, SIDES_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, DOCUMENT_FORMATS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, ORIENTATIONS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, IMAGE_FORMATS_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, PRINT_QUALITY_SUPPORTED) ||
                 !CsrBtStrICmp((char *) childElement->name, LOADED_MEDIUM) ||
                 !CsrBtStrICmp((char *) childElement->name, QUEUED_JOB_COUNT))
        {
            childElement = childElement->nextSiblingElement;
        }
        else
        {
            childElement = childElement->firstChildElement;
        }
    }
}

static void xmlInterpreterHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    if (bppcDemoInstanceData->recvXmlPtr)
    {
        if (bppcDemoInstanceData->xmlTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlTree);
        }
        bppcDemoInstanceData->xmlTree = NULL;
#ifdef XML_INTERPRET
        bppcDemoInstanceData->xmlTree = CsrBtBuildXmlTree(bppcDemoInstanceData->recvXmlPtr, bppcDemoInstanceData->recvXmlSize);
#endif
        if (bppcDemoInstanceData->xmlTree)
        {
            CsrBtRootElementType        * rootElement;
            CsrBtChildElementType    * childElement;

            rootElement = &(bppcDemoInstanceData->xmlTree->elementArray[0].rootElement);
            childElement = rootElement->firstChildElement;

            while (childElement)
            {
                if (!CsrBtStrICmp((char *) childElement->name, PRINTER_NAME) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_LOCATION) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE_REASONS) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_OPERATOR) ||
                    !CsrBtStrICmp((char *) childElement->name, COLOR_SUPPORTED) ||
                    !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_WIDTH) ||
                    !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_HEIGHT) ||
                    !CsrBtStrICmp((char *) childElement->name, OPERATION_STATUS) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_ID) ||
                    !CsrBtStrICmp((char *) childElement->name, QUEUED_JOB_COUNT) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_NAME) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_USER_NAME) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_STATE))
                {
                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, NUMBER_UP_SUPPORTED))
                {
                    CsrUint16 numberUp;

                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    sscanf((char*)childElement->value, "%hu", &numberUp);

                    bppcDemoInstanceData->numberUp = (CsrUint8) numberUp;
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, MAX_COPIES_SUPPORTED))
                {
                    CsrUint16 copies;

                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    sscanf((char*)childElement->value, "%hu", &copies);
                    if (copies > 9)
                    {
                        copies = 9;
                    }
                    bppcDemoInstanceData->maxCopies = (CsrUint8) copies;
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, MEDIA_SIZES_SUPPORTED))
                {

                    CsrBtChildElementType    * subChildElement;
                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value);

                            if (length > 0)
                            {
                                printf("%-30.30s %s\n", " ", subChildElement->value);
                                bppcDemoInstanceData->numOfDiffMediaSizes++;
                            }
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, MEDIA_TYPES_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;
                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value);

                            if (length > 0)
                            {
                                printf("%-30.30s %s\n", " ", subChildElement->value);
                                bppcDemoInstanceData->numOfDiffMediaTypes++;
                            }
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, DOCUMENT_FORMATS_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;

                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            printf("%-30.30s %s\n", " ", subChildElement->value);
                        }
                        if(!CsrBtStrICmp((char *) subChildElement->value, MIME_XHTML_0_95))
                        {
                            bppcDemoInstanceData->documentFormat += XHTML95_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_TEXT))
                        {
                            bppcDemoInstanceData->documentFormat += TEXT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VCARD21))
                        {
                            bppcDemoInstanceData->documentFormat += VCARD21_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VCARD30))
                        {
                            bppcDemoInstanceData->documentFormat += VCARD30_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VCALENDAR))
                        {
                            bppcDemoInstanceData->documentFormat += VCAL_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_ICALENDAR))
                        {
                            bppcDemoInstanceData->documentFormat += ICAL_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_POST2))
                        {
                            bppcDemoInstanceData->documentFormat += POSTS2_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_POST3))
                        {
                            bppcDemoInstanceData->documentFormat += POSTS3_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_PCL5E))
                        {
                            bppcDemoInstanceData->documentFormat += PCL5E_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_PCL6))
                        {
                            bppcDemoInstanceData->documentFormat += PCL6_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_PDF))
                        {
                            bppcDemoInstanceData->documentFormat += PDF_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_JPEG))
                        {
                            bppcDemoInstanceData->documentFormat += JPEG_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_GIF))
                        {
                            bppcDemoInstanceData->documentFormat += GIF_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_XHTML_1_00))
                        {
                            bppcDemoInstanceData->documentFormat += XHTML10_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VMESSAGE11))
                        {
                            bppcDemoInstanceData->documentFormat += VMESSAGE11_MASK;
                        }
                        bppcDemoInstanceData->numOfDiffDocumentFormats++;
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, SIDES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, ORIENTATIONS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, IMAGE_FORMATS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINT_QUALITY_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;

                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            printf("%-30.30s %s\n", " ", subChildElement->value);
                        }

                        if(!CsrBtStrICmp((char *) subChildElement->value, ONE_SIDED))
                        {
                            bppcDemoInstanceData->sides += ONE_SIDED_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, TWO_SIDED_LONG))
                        {
                            bppcDemoInstanceData->sides += TWO_SIDED_LONG_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, TWO_SIDED_SHORT))
                        {
                            bppcDemoInstanceData->sides += TWO_SIDED_SHORT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, QUALITY_DRAFT))
                        {
                            bppcDemoInstanceData->printfQuality += DRAFT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, QUALITY_NORMAL))
                        {
                            bppcDemoInstanceData->printfQuality += NORMAL_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, QUALITY_HIGH))
                        {
                            bppcDemoInstanceData->printfQuality += HIGH_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_PORTRAIT))
                        {
                            bppcDemoInstanceData->orientation += PORTRAIT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_LANDSCAPE))
                        {
                            bppcDemoInstanceData->orientation += LANDSCAPE_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_REV_LANDSCAPE))
                        {
                            bppcDemoInstanceData->orientation += REVERSE_LANDSCAPE_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_REV_PORTRAIT))
                        {
                            bppcDemoInstanceData->orientation += REVERSE_PORTRAIT_MASK;
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, LOADED_MEDIUM))
                {
                    CsrBtChildElementType    * subChildElement;

                    subChildElement = childElement->firstChildElement;
                    while (subChildElement)
                    {
                        if (!CsrBtStrICmp((char *) subChildElement->name, LOADED_MEDIUM_DETAILS))
                        {
                            CsrBtChildElementType    * subsubChildElement;

                            subsubChildElement = subChildElement->firstChildElement;

                            while (subsubChildElement)
                            {
                                if (!CsrBtStrICmp((char *) subsubChildElement->name, LOADED_MEDIUM_SIZE) ||
                                    !CsrBtStrICmp((char *) subsubChildElement->name, LOADED_MEDIUM_TYPE))
                                {
                                    printf("%-30.30s %s\n", subsubChildElement->name, subsubChildElement->value);
                                }
                                subsubChildElement = subsubChildElement->nextSiblingElement;
                            }

                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }

                else if (!CsrBtStrICmp((char *) childElement->name, JOB_SHEETS_COMPLETED))
                {
                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, JOB_NUMBER_OF_INTERVENING))
                {
                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    childElement = childElement->nextSiblingElement;
                }
                else
                {
                    childElement = childElement->firstChildElement;
                }
            }
        }
        else
        {
            CsrUint32 i;

            for (i = 0; i < bppcDemoInstanceData->recvXmlSize; i++)
            {
                printf("%c", bppcDemoInstanceData->recvXmlPtr[i]);
            }
            printf("Fail to interpret therefor just printed result out\n");

        }
    }
    else
    {
        printf("Fail nothing to interpret\n");
    }
}

static void xmlStatusChannelInterpreterHandler(BppcDemoInstanceDataType * bppcDemoInstanceData)
{
    if (bppcDemoInstanceData->recvStatusXmlPtr)
    {
        if (bppcDemoInstanceData->xmlStatusTree)
        {
            CsrBtFreeXmlTree(bppcDemoInstanceData->xmlStatusTree);
        }
        bppcDemoInstanceData->xmlStatusTree = NULL;
#ifdef XML_INTERPRET
        bppcDemoInstanceData->xmlStatusTree = CsrBtBuildXmlTree(bppcDemoInstanceData->recvStatusXmlPtr, bppcDemoInstanceData->recvStatusXmlSize);
#endif
        if (bppcDemoInstanceData->xmlStatusTree)
        {
            CsrBtRootElementType     * rootElement;
            CsrBtChildElementType    * childElement;

            rootElement = &(bppcDemoInstanceData->xmlStatusTree->elementArray[0].rootElement);
            childElement = rootElement->firstChildElement;

            while (childElement)
            {
                if (!CsrBtStrICmp((char *) childElement->name, PRINTER_NAME) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_LOCATION) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_STATE_REASONS) ||
                    !CsrBtStrICmp((char *) childElement->name, PRINTER_OPERATOR) ||
                    !CsrBtStrICmp((char *) childElement->name, COLOR_SUPPORTED) ||
                    !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_WIDTH) ||
                    !CsrBtStrICmp((char *) childElement->name, TEXT_PAGE_HEIGHT) ||
                    !CsrBtStrICmp((char *) childElement->name, OPERATION_STATUS) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_ID) ||
                    !CsrBtStrICmp((char *) childElement->name, QUEUED_JOB_COUNT) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_NAME) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_USER_NAME) ||
                    !CsrBtStrICmp((char *) childElement->name, JOB_STATE))
                {
                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, NUMBER_UP_SUPPORTED))
                {
                    CsrUint16 numberUp;

                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    sscanf((char*)childElement->value, "%hu", &numberUp);

                    bppcDemoInstanceData->numberUp = (CsrUint8) numberUp;
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, MAX_COPIES_SUPPORTED))
                {
                    CsrUint16 copies;

                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    sscanf((char*)childElement->value, "%hu", &copies);
                    if (copies > 9)
                    {
                        copies = 9;
                    }
                    bppcDemoInstanceData->maxCopies = (CsrUint8) copies;
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, MEDIA_SIZES_SUPPORTED))
                {

                    CsrBtChildElementType    * subChildElement;
                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value);

                            if (length > 0)
                            {
                                printf("%-30.30s %s\n", " ", subChildElement->value);
                                bppcDemoInstanceData->numOfDiffMediaSizes++;
                            }
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, MEDIA_TYPES_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;
                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            CsrUint16 length = (CsrUint16) CsrStrLen((char*)subChildElement->value);

                            if (length > 0)
                            {
                                printf("%-30.30s %s\n", " ", subChildElement->value);
                                bppcDemoInstanceData->numOfDiffMediaTypes++;
                            }
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, DOCUMENT_FORMATS_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;

                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            printf("%-30.30s %s\n", " ", subChildElement->value);
                        }
                        if(!CsrBtStrICmp((char *) subChildElement->value, MIME_XHTML_0_95))
                        {
                            bppcDemoInstanceData->documentFormat += XHTML95_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_TEXT))
                        {
                            bppcDemoInstanceData->documentFormat += TEXT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VCARD21))
                        {
                            bppcDemoInstanceData->documentFormat += VCARD21_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VCARD30))
                        {
                            bppcDemoInstanceData->documentFormat += VCARD30_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VCALENDAR))
                        {
                            bppcDemoInstanceData->documentFormat += VCAL_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_ICALENDAR))
                        {
                            bppcDemoInstanceData->documentFormat += ICAL_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_POST2))
                        {
                            bppcDemoInstanceData->documentFormat += POSTS2_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_POST3))
                        {
                            bppcDemoInstanceData->documentFormat += POSTS3_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_PCL5E))
                        {
                            bppcDemoInstanceData->documentFormat += PCL5E_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_PCL6))
                        {
                            bppcDemoInstanceData->documentFormat += PCL6_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_PDF))
                        {
                            bppcDemoInstanceData->documentFormat += PDF_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_JPEG))
                        {
                            bppcDemoInstanceData->documentFormat += JPEG_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_GIF))
                        {
                            bppcDemoInstanceData->documentFormat += GIF_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_XHTML_1_00))
                        {
                            bppcDemoInstanceData->documentFormat += XHTML10_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, MIME_VMESSAGE11))
                        {
                            bppcDemoInstanceData->documentFormat += VMESSAGE11_MASK;
                        }
                        bppcDemoInstanceData->numOfDiffDocumentFormats++;
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, SIDES_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, ORIENTATIONS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, IMAGE_FORMATS_SUPPORTED) ||
                         !CsrBtStrICmp((char *) childElement->name, PRINT_QUALITY_SUPPORTED))
                {
                    CsrBtChildElementType    * subChildElement;

                    CsrBool    nameFlag = FALSE;
                    subChildElement = childElement->firstChildElement;

                    while (subChildElement)
                    {
                        if (!nameFlag)
                        {
                            nameFlag = TRUE;
                            printf("%-30.30s %s\n", subChildElement->name, subChildElement->value);
                        }
                        else
                        {
                            printf("%-30.30s %s\n", " ", subChildElement->value);
                        }

                        if(!CsrBtStrICmp((char *) subChildElement->value, ONE_SIDED))
                        {
                            bppcDemoInstanceData->sides += ONE_SIDED_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, TWO_SIDED_LONG))
                        {
                            bppcDemoInstanceData->sides += TWO_SIDED_LONG_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, TWO_SIDED_SHORT))
                        {
                            bppcDemoInstanceData->sides += TWO_SIDED_SHORT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, QUALITY_DRAFT))
                        {
                            bppcDemoInstanceData->printfQuality += DRAFT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, QUALITY_NORMAL))
                        {
                            bppcDemoInstanceData->printfQuality += NORMAL_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, QUALITY_HIGH))
                        {
                            bppcDemoInstanceData->printfQuality += HIGH_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_PORTRAIT))
                        {
                            bppcDemoInstanceData->orientation += PORTRAIT_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_LANDSCAPE))
                        {
                            bppcDemoInstanceData->orientation += LANDSCAPE_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_REV_LANDSCAPE))
                        {
                            bppcDemoInstanceData->orientation += REVERSE_LANDSCAPE_MASK;
                        }
                        else if(!CsrBtStrICmp((char *) subChildElement->value, ORI_REV_PORTRAIT))
                        {
                            bppcDemoInstanceData->orientation += REVERSE_PORTRAIT_MASK;
                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, LOADED_MEDIUM))
                {
                    CsrBtChildElementType    * subChildElement;

                    subChildElement = childElement->firstChildElement;
                    while (subChildElement)
                    {
                        if (!CsrBtStrICmp((char *) subChildElement->name, LOADED_MEDIUM_DETAILS))
                        {
                            CsrBtChildElementType    * subsubChildElement;

                            subsubChildElement = subChildElement->firstChildElement;

                            while (subsubChildElement)
                            {
                                if (!CsrBtStrICmp((char *) subsubChildElement->name, LOADED_MEDIUM_SIZE) ||
                                    !CsrBtStrICmp((char *) subsubChildElement->name, LOADED_MEDIUM_TYPE))
                                {
                                    printf("%-30.30s %s\n", subsubChildElement->name, subsubChildElement->value);
                                }
                                subsubChildElement = subsubChildElement->nextSiblingElement;
                            }

                        }
                        subChildElement = subChildElement->nextSiblingElement;
                    }
                    childElement = childElement->nextSiblingElement;
                }

                else if (!CsrBtStrICmp((char *) childElement->name, JOB_SHEETS_COMPLETED))
                {
                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    childElement = childElement->nextSiblingElement;
                }
                else if (!CsrBtStrICmp((char *) childElement->name, JOB_NUMBER_OF_INTERVENING))
                {
                    printf("%-30.30s %s\n", childElement->name, childElement->value);
                    childElement = childElement->nextSiblingElement;
                }
                else
                {
                    childElement = childElement->firstChildElement;
                }
            }
        }
        else
        {
            CsrUint32 i;

            for (i = 0; i < bppcDemoInstanceData->recvStatusXmlSize; i++)
            {
                printf("%c", bppcDemoInstanceData->recvStatusXmlPtr[i]);
            }
            printf("Fail to interpret therefor just printed result out\n");

        }
    }
    else
    {
        printf("Fail nothing to interpret\n");
    }
}

static void sendingTheSelectedFile(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint16 maxToSend)
{
    CsrUint32                bytesToRead;
    CsrBool                    finalFlag;

    if( (bppcDemoInstanceData->currentFileSize - bppcDemoInstanceData->currentFilePosition) <= maxToSend)
    {
        bytesToRead = bppcDemoInstanceData->currentFileSize - bppcDemoInstanceData->currentFilePosition;
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
        CsrUint8                * printContent;

        printContent = (CsrUint8 *) CsrPmemAlloc(bytesToRead);
        bytesRead = CsrBtFread(printContent, 1, bytesToRead, bppcDemoInstanceData->currentFileHandle);
        bppcDemoInstanceData->currentFilePosition += bytesRead;
        CsrBtBppcSendDocumentResSend(finalFlag, bytesRead, printContent);
    }
    if(bppcDemoInstanceData->currentFilePosition >= bppcDemoInstanceData->currentFileSize)
    {
        CsrBtFclose(bppcDemoInstanceData->currentFileHandle);
        bppcDemoInstanceData->currentFilePosition = 0;
        bppcDemoInstanceData->currentFileHandle = NULL;
    }
}

static void sendingTheSelectedFileRefObj(BppcDemoInstanceDataType * bppcDemoInstanceData, CsrUint32 count, CsrUint32 offset, CsrUint32 fileSize, CsrBool lastPacket)
{
    CsrUint32                bytesToRead;
    CsrBool                  finalFlag;
    CsrInt32                 result;

    if( (bppcDemoInstanceData->currentRefFileSize - offset) <= count)
    {
        bytesToRead = bppcDemoInstanceData->currentRefFileSize - offset;
        finalFlag = TRUE;
    }
    else
    {
        bytesToRead = count;
        finalFlag = FALSE;
    }

    if (lastPacket)
    {
        finalFlag = TRUE;
    }

    if(bytesToRead > 0)
    {
        CsrUint16                bytesRead;
        CsrUint8              * printContent;

        printContent = (CsrUint8 *) CsrPmemAlloc(bytesToRead);
        result = CsrBtFseek(bppcDemoInstanceData->currentRefFileHandle, offset, SEEK_SET);
        if (result != 0)
        {
            printf("FSEEK ERROR\n");
            CsrBtBppcGetReferenceObjectResSend(CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE,
                                              NULL,
                                              NULL,
                                              0,
                                              offset,
                                              fileSize,
                                              finalFlag,
                                              FALSE);
        }
        else
        {
            char *pFileName = NULL;
            CsrUint16 len = (CsrUint16)CsrStrLen(bppcDemoInstanceData->referenceFileName) + 1;
            if (len > 1)
            {
                pFileName = CsrPmemAlloc(len);
                CsrStrNCpy(pFileName,bppcDemoInstanceData->referenceFileName,len);
            }
            bytesRead = CsrBtFread(printContent, 1, bytesToRead, bppcDemoInstanceData->currentRefFileHandle);
            CsrBtBppcGetReferenceObjectResSend(CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                              printContent,
                                              pFileName,
                                              bytesToRead,
                                              offset,
                                              fileSize,
                                              finalFlag,
                                              FALSE);
        }
    }
    else
    {
        CsrBtBppcGetReferenceObjectResSend(CSR_BT_OBEX_NO_CONTENT_RESPONSE_CODE,
                                          NULL,
                                          NULL,
                                          0,
                                          offset,
                                          fileSize,
                                          finalFlag,
                                          FALSE);
    }
    if (lastPacket)
    {
        CsrBtFclose(bppcDemoInstanceData->currentRefFileHandle);
        bppcDemoInstanceData->currentRefFilePosition = 0;
        bppcDemoInstanceData->currentRefFileHandle = NULL;
    }
}


#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16    msg_type=0;
    void    * msg_data=NULL;
    BppcDemoInstanceDataType * instanceData;

    instanceData = (BppcDemoInstanceDataType *) *gash;

    scAppDeinitInstanceData(&instanceData->scInstData);

    /* get a message from the demoapplication message queue. The
     * message is returned in prim and the event type in eventType
     */
    while ( CsrSchedMessageGet(&msg_type, &msg_data) )
    {
        switch (msg_type)
        {
            case CSR_BT_BPPC_PRIM:
            {
                CsrBtBppcFreeUpstreamMessageContents(msg_type, msg_data);
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
        CsrPmemFree(msg_data);
    }
    if (instanceData->currentFileHandle)
    {
        CsrBtFclose(instanceData->currentFileHandle);
    }
    CsrPmemFree(instanceData->recvXmlPtr);
    if (instanceData->xmlTree)
    {
        CsrBtFreeXmlTree(instanceData->xmlTree);
    }

    if (instanceData->currentRefFileHandle)
    {
        CsrBtFclose(instanceData->currentRefFileHandle);
    }
    CsrPmemFree(instanceData->recvStatusXmlPtr);

    if (instanceData->xmlStatusTree)
    {
        CsrBtFreeXmlTree(instanceData->xmlStatusTree);
    }
    CsrPmemFree(instanceData->xhtmlPrintImageFormats);
    CsrPmemFree(instanceData->documentFormatsSupported);
    CsrPmemFree(instanceData->mediaTypesSupported);
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
