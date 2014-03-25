/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_profiles.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_bpps_lib.h"
#include "csr_bt_bpps_prim.h"
#include "csr_bt_obex.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"
#include "csr_unicode.h"
#include "csr_bt_demoapp.h"

#define KEYB_CHECK_TIMEOUT               (100000)

#define MAX_DISCOVERY_RESULTS_IN_LIST    16

#define PRINTER_NAME                    "MyPrinter"
#define PRINTER_LOCATION                "MyLocation"
#define PRINTER_STATE                   "idle"
#define PRINTER_STATE_REASON            "none"
#define PRINTER_OPERATOR                "Fred Bloggs"
#define PRINTER_DOCUMENT_FORMAT_1       "application/vnd.pwg-xhtml-print+xml:0.95"
#define PRINTER_DOCUMENT_FORMAT_2       "application/vnd.hp-PCL:5E"
#define PRINTER_DOCUMENT_FORMAT_3       "text/plain"
#define PRINTER_DOCUMENT_FORMAT_4       "application/PostScript:3"
#define PRINTER_DOCUMENT_FORMAT_5       "application/vnd.pwg-multiplexed"
#define PRINTER_DOCUMENT_FORMAT_6       "text/x-vcard:2.1"
#define PRINTER_DOCUMENT_FORMAT_7       "text/x-vcard:3.0"
#define PRINTER_DOCUMENT_FORMAT_8       "text/x-vcalendar:1.0"
#define PRINTER_DOCUMENT_FORMAT_9       "text/calendar:2.0"
#define PRINTER_DOCUMENT_FORMAT_10      "text/x-vmessage:1.1"
#define PRINTER_IMAGE_FORMAT_1          "image/gif"
#define PRINTER_IMAGE_FORMAT_2          "image/jpeg"
#define PRINTER_COLOR                   "true"
#define PRINTER_MAX_COPIES              "1"
#define PRINTER_SIDES_1                 "one-sided"
#define PRINTER_SIDES_2                 "two-sided-long-edge"
#define PRINTER_SIDES_3                 "two-sided-short-edge"
#define PRINTER_NUMBER_UP               "4"
#define PRINTER_ORIENTATION_1           "portrait"
#define PRINTER_ORIENTATION_2           "landscape"
#define PRINTER_MEDIA_SIZES_1           "iso_a4_210x297mm"
#define PRINTER_MEDIA_SIZES_2           "iso_a3_297x420mm"
#define PRINTER_MEDIA_TYPE_1            "stationary"
#define PRINTER_MEDIA_TYPE_2            "photographic"
#define PRINTER_MEDIA_TYPE_3            "cardstock"
#define PRINTER_QUALITY_1               "draft"
#define PRINTER_QUALITY_2               "normal"
#define PRINTER_QUEUED_JOB_COUNT        "0"
#define PRINTER_BASIC_HEIGHT            "60"
#define PRINTER_BASIC_WIDTH             "125"

#define APP_MIME_TYPE                   "application/vnd.pwg-xhtml-print+xml:1.0,text/plain,text/x-vcard:2.1,text/x-vcard:3.0,text/x-vcalendar:1.0,text/calendar:2.0,text/x-vmessage:1.1,application/vnd.hp-PCL:3C,image/jpeg,application/vnd.pwg-multiplexed"
#define APP_IMAGE_TYPE                  "IMAGE/JPEG"
#define APP_SERVICE_TYPE                 "MFG:CSR;MDL:CSR printer demo application;CMD:MLC,PCL,PML;CLS:PRINTER;DES:CSR printer demo application;"

#define PRINTER_XML_1_STRING            "application/vnd.pwg-xhtml-print+xml:1.0"
#define PRINTER_XML_NO_VER_STRING       "application/vnd.pwg-xhtml-print+xml"

typedef CsrUint8 state;
#define IDLE_STATE (state) 0x00
#define ACTIVATION_PENDING_STATE (state) 0x01
#define ACTIVE_STATE (state) 0x02
#define DEACTIVATION_PENDING_STATE (state) 0x03
#define LAST_STATE (state) 0x04

typedef CsrUint8 keyState;
#define IDLE_KEYBOARD_STATE (keyState) 0x00
#define PASSKEY_KEYBOARD_STATE (keyState) 0x01
#define OBEX_PASSKEY_KEYBOARD_STATE (keyState) 0x02
#define ACTIVATE_OBEX_PASSKEY_KEYBOARD_STATE (keyState) 0x03
#define KEYS_LAST_STATE (keyState) 0x04

typedef CsrUint32 appTarget;
#define APP_NONE_TARGET                  (appTarget)(0)
#define APP_PULL_PB_TARGET               (appTarget)(1)
#define APP_SET_FOLDER_TARGET            (appTarget)(2)
#define APP_PULL_LISTING_TARGET          (appTarget)(3)
#define APP_PULL_ENTRY_TARGET            (appTarget)(4)

typedef struct
{
    CsrSchedQid           CsrSchedQid;
    CsrUint16            event;
    void                *pRecvMsg;
    keyState            keyboardState;
    keyState            prevKeyboardState;
    state               state;
    CsrBool              isAuth;
    CsrUint32            connectionId;
    CsrUint16            obexPeerMaxPacketSize;
    CsrBtDeviceAddr        clientDevAddr;
    appTarget           target;
    char                refObjName[256];
    CsrBool              firstRefPacket;
    CsrUint16            jobId;
    CsrUint32            refObjSize;
    CsrUint32            numOfBytesReceived;
    CsrBool              sessionIsAuth;
    char*               xml1p0;
    char*               xml0p95;
    char*               xmlNoVer;
    char                listOfRefObjects[2000];
    CsrUint32            refObjCount;
    char*               currentObjRetrieval;
    CsrUint32            numOfRetrievedObj;
    char                printJobName[20];
    CsrBool              isXml;
    CsrBool              printCompleted;
    CsrBool              getAllRefObjCompleted;
    CsrUint8             passkeyLength;
    CsrUint8             passkey[CSR_BT_PASSKEY_MAX_LEN];
    ScAppInstData_t     scInstData;
    CsrBool              paperTrayFull;
    CsrBool              autoFinish;
    CsrUint32            delResConnId;
    CsrBool              inPrivateOnlineMode;
    CsrBool             updateEvent;
} appInstanceData;


static void playIntro(appInstanceData* instData);
static void keyMessageService(CsrUint16 m, void* data);
static void handleKeybPrim(appInstanceData* pInst);
static CsrBool idleState(appInstanceData* pInst, char cKey);
static void keyboardObexPasskeyStateHandler(appInstanceData* pInst, char theCharacter);

static void handleBppsPrim(appInstanceData* pInst);
static void handleCsrBtBppsActivateCfm(appInstanceData* pInst);
static void handleCsrBtBppsDeactivateCfm(appInstanceData* pInst);
static void handleCsrBtBppsConnectInd(appInstanceData* pInst);
static void handleBppsAuthCfm(appInstanceData* pInst);
static void handleBppsAuthInd(appInstanceData* pInst);
static void handleCsrBtBppsNextInd(appInstanceData* pInst);
static void handleCsrBtBppsAbortInd(appInstanceData* pInst);
static void handleCsrBtBppsDisconnectInd(appInstanceData* pInst);
static void handleBppsCreateJob(appInstanceData* pInst);
static void handleCsrBtBppsSendDocumentInd(appInstanceData* pInst);
static void handleBppsGetPrinterAttribs(appInstanceData* pInst);
static void handleBppsGetJobAttribs(appInstanceData* pInst);
static void handleBppsCancelJob(appInstanceData* pInst);
static void handleCsrBtBppsGetReferencedObjInd(appInstanceData* pInst);
static void handleCsrBtBppsGetReferencedObjCfm(appInstanceData* pInst);
static void handleCsrBtBppsGetEventInd(appInstanceData* pInst);
static void handleCsrBtBppsJobCompleteCfm(appInstanceData* pInst);

static void handleWriteScanEnableCfm(void *messageData);

/* Forward declarations */
extern int _getch();
extern int _kbhit();

static void bppScAppSetFnFini(void *pFiniData)
{
    appInstanceData *instData = (appInstanceData *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState);
/*
    system(CLS);
*/
    playIntro(pFiniData);
}

static void bppScAppSetFnInit(void *pInitData)
{
    appInstanceData *instData  = (appInstanceData *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState, PASSKEY_KEYBOARD_STATE);
}

static void* bppScAppSetFnGetMessage(void *pRecvMsgPData)
{
    appInstanceData *instData = (appInstanceData *) pRecvMsgPData;
    void *result              = instData->pRecvMsg;
    instData->pRecvMsg        = NULL;
    return result;
}

/**************************************************************************************************
 *
 *    init function called by the scheduler upon initialisation. This function is used to boot
 *    the demo application by sending a request to bond with the headset. Bonding is not mandated
 *    according to the profile but is needed if encryption of the speech is required.
 *
 **************************************************************************************************/
void CsrBtAppInit(void** gash)
{
    appInstanceData* pInst;

    *gash = CsrPmemAlloc(sizeof(appInstanceData));
    pInst = *gash;
    pInst->state = IDLE_STATE;
    pInst->event = 0;
    pInst->pRecvMsg = NULL;
    pInst->isAuth = FALSE;
    pInst->connectionId = 0;
    pInst->obexPeerMaxPacketSize = 0;
    pInst->clientDevAddr.nap = 0;
    pInst->clientDevAddr.uap = 0;
    pInst->clientDevAddr.lap = 0;
    pInst->target = APP_NONE_TARGET;
    pInst->passkey[0] = '\0';
    pInst->passkeyLength = 0;
    pInst->keyboardState = IDLE_KEYBOARD_STATE;
    pInst->firstRefPacket = TRUE;
    pInst->jobId = 0;
    pInst->sessionIsAuth = FALSE;
    pInst->updateEvent   = FALSE;
    pInst->xml1p0 = CsrPmemAlloc(strlen(PRINTER_XML_1_STRING) + 1);
    strcpy(pInst->xml1p0, PRINTER_XML_1_STRING);

    pInst->xml0p95 = CsrPmemAlloc(strlen(PRINTER_DOCUMENT_FORMAT_1) + 1);
    strcpy(pInst->xml0p95, PRINTER_DOCUMENT_FORMAT_1);

    pInst->xmlNoVer = CsrPmemAlloc(strlen(PRINTER_XML_NO_VER_STRING) + 1);
    strcpy(pInst->xmlNoVer, PRINTER_XML_NO_VER_STRING);

    printf("\nBPPC Server Demo:\n\n");
    printf("Hit <ESC> to quit program!\n\n");
    pInst->refObjSize = 0;
    pInst->numOfBytesReceived = 0;
    pInst->refObjCount = 0;
    pInst->currentObjRetrieval = NULL;
    pInst->numOfRetrievedObj = 0;
    sprintf(pInst->printJobName, "printjob");
    pInst->isXml = FALSE;
    pInst->printCompleted = TRUE;
    pInst->getAllRefObjCompleted = TRUE;
    pInst->paperTrayFull = TRUE;
    pInst->autoFinish = TRUE;
    pInst->delResConnId = 0;
    pInst->inPrivateOnlineMode = FALSE;

    /* Application handle */
    pInst->CsrSchedQid = TESTQUEUE;

    /* Security App Configuration */
    CsrBtScActivateReqSend(pInst->CsrSchedQid);
    scAppInitInstanceData(&pInst->scInstData);

    scAppSetFnFini(&pInst->scInstData, bppScAppSetFnFini, pInst);
    scAppSetFnInit(&pInst->scInstData, bppScAppSetFnInit, pInst);
    scAppSetSecLevelSend(&pInst->scInstData, scAppSendProfileSecurityLevelReq, &pInst->scInstData);

    scAppSetFnGetMessage(&pInst->scInstData, bppScAppSetFnGetMessage, pInst);

    scAppSetSecLevelPrim(&pInst->scInstData,
                         0,     /* Index 0. Single profile */
                         "BPPS", CSR_BT_BPPS_PRIM,
                         CSR_BT_BPPS_IFACEQUEUE,
                         CSR_BT_BPPS_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, keyMessageService, 0, NULL);
    playIntro(pInst);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void** gash)
{
    appInstanceData* pInst = *gash;


    scAppDeinitInstanceData(&pInst->scInstData);

    /* get a message from the demoapplication message queue. The
     * message is returned in prim and the event type in eventType
     */
    while ( CsrSchedMessageGet(&pInst->event, &pInst->pRecvMsg) )
    {
        switch (pInst->event)
        {
            case CSR_BT_BPPS_PRIM:
            {
                CsrBtBppsFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                break;
            }
            case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                break;
            }
            default:
            { /* just ignore  */
                break;
            }
        }
        CsrPmemFree(pInst->pRecvMsg);
    }
    CsrPmemFree(pInst->xml1p0);
    CsrPmemFree(pInst->xml0p95);
    CsrPmemFree(pInst->xmlNoVer);

    CsrPmemFree(pInst);
}
#endif

/**************************************************************************************************
 *
 *    this is the demo application handler function. All primitives sent to the demo application will
 *    be received in here. The scheduler ensures that the function is activated when a signal is put
 *    on the demo application signal queue.
 *
 **************************************************************************************************/
void CsrBtAppHandler(void** gash)
{
    appInstanceData* pInst = (appInstanceData*) *gash;

    /* get a message from the demoapplication message queue. The
      * message is returned in prim and the event type in eventType
      */
    if (CsrSchedMessageGet(&pInst->event, &pInst->pRecvMsg))
    {
        /* two event types must be handled. SC for bonding and PAC for
         * the connection and PB related signalling
         */
        switch (pInst->event)
        {
            case CSR_BT_SC_PRIM:
            {
                handleScPrim(&pInst->scInstData);
                break;
            }
            case CSR_BT_BPPS_PRIM:
                {
                    CsrPrim *prim = (CsrPrim *) pInst->pRecvMsg;
                    switch(*prim)
                    {
                        case CSR_BT_BPPS_SECURITY_IN_CFM:
                            handleScSecLevelPrim(&pInst->scInstData);
                            break;

                        default:
                            handleBppsPrim(pInst);
                            break;
                    }
                    break;
                }

            case CSR_BT_CM_PRIM:
                {
                    CsrPrim *prim = (CsrPrim *) pInst->pRecvMsg;
                    switch(*prim)
                    {
                        case CSR_BT_CM_WRITE_SCAN_ENABLE_CFM:
                    {
                            handleWriteScanEnableCfm(pInst->pRecvMsg);
                            break;
                    }
                        default:
                    {
                            printf("Unhandled CM prim 0x%2x received\n", *prim);
                            CsrBtCmFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                    }
                            break;
                    }
                    break;
                }

            case KEY_MESSAGE:
                handleKeybPrim(pInst);
                break;

            default:
                /* unexpected primitive received */
                printf("####### default in testhandler %x,\n", pInst->event);
                break;
        }

        if (NULL != pInst->pRecvMsg)
        {
            /* free the received signal again. If the signal for some
             * reason must be stored in the application the pointer
             * (prim) must be set to NULL in order not to free it
             * here
             */
            CsrPmemFree(pInst->pRecvMsg);
            pInst->pRecvMsg = NULL;
        }
    }
}

/* == LOCAL FUNCTIONS =============================================================================== */
static void playIntro(appInstanceData* instData)
{
    printf("\nOBEX Basic Printer Profile SERVER:\n\n");
    printf("\nProgram options:\n\n");
    printf("    1) Activate Obex BPPC server\n");
    printf("    2) Activate Obex BPPC server with OBEX authentication\n");
    printf("    3) Security Controller Configuration\n");
    printf("    4) Set server into %s Online Mode\n\n", instData->inPrivateOnlineMode ? "Public" : "Private");

    if (!instData->autoFinish &&
        instData->printCompleted == TRUE &&
        instData->getAllRefObjCompleted == TRUE)
    {
            printf("    f) Signal `printing done'\n\n");
    }

    printf("    p) Toggle paper tray Full/Empty. Paper tray is currently %s\n", instData->paperTrayFull ? "full" : "empty");
    printf("    q) Toggle automatic print completion signalling, currently %s\n\n", instData->autoFinish ? "on" : "off");

    printf("    x) Deactivate Obex BPPC\n");
    printf("\nHit <ESC> to quit program!\n\n");
}

/*************************  BPPS PRIMITIVES    ***************************/
static void handleBppsPrim(appInstanceData* pInst)
{
    CsrBtBppsPrim* pMsg = pInst->pRecvMsg;

    switch (*pMsg)
    {
        case CSR_BT_BPPS_ACTIVATE_CFM:
            handleCsrBtBppsActivateCfm(pInst);
            break;

        case CSR_BT_BPPS_DEACTIVATE_CFM:
            handleCsrBtBppsDeactivateCfm(pInst);
            break;

        case CSR_BT_BPPS_CONNECT_IND:
            handleCsrBtBppsConnectInd(pInst);
            break;

        case CSR_BT_BPPS_AUTHENTICATE_CFM:
            handleBppsAuthCfm(pInst);
            break;

        case CSR_BT_BPPS_AUTHENTICATE_IND:
            handleBppsAuthInd(pInst);
            break;

        case CSR_BT_BPPS_NEXT_IND:
            handleCsrBtBppsNextInd(pInst);
            break;

        case CSR_BT_BPPS_ABORT_IND:
            handleCsrBtBppsAbortInd(pInst);
            break;

        case CSR_BT_BPPS_DISCONNECT_IND:
            handleCsrBtBppsDisconnectInd(pInst);
            break;

        case CSR_BT_BPPS_CREATE_JOB_IND:
            handleBppsCreateJob(pInst);
            break;

        case CSR_BT_BPPS_SEND_DOCUMENT_IND:
            handleCsrBtBppsSendDocumentInd(pInst);
            break;

        case CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND:
            handleBppsGetPrinterAttribs(pInst);
            break;

        case CSR_BT_BPPS_GET_JOB_ATTRIBS_IND:
            handleBppsGetJobAttribs(pInst);
            break;

        case CSR_BT_BPPS_CANCEL_JOB_IND:
            handleBppsCancelJob(pInst);
            break;

        case CSR_BT_BPPS_GET_REFERENCED_OBJ_IND:
            handleCsrBtBppsGetReferencedObjInd(pInst);
            break;

        case CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM:
            handleCsrBtBppsGetReferencedObjCfm(pInst);
            break;

        case CSR_BT_BPPS_GET_EVENT_IND:
            handleCsrBtBppsGetEventInd(pInst);
            break;

        case CSR_BT_BPPS_JOB_COMPLETE_CFM:
            handleCsrBtBppsJobCompleteCfm(pInst);
            break;

        default:
            printf("####### Unhandled bppsPrim: %x received\n", *pMsg);
            CsrBtBppsFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
            break;
    }
}
static CsrBool typeIsXml(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,instData->xml1p0) == 0)
    {
        result = TRUE;
    }
    else if (CsrStrCmp(docType,instData->xml0p95) == 0)
    {
        result = TRUE;
    }
    else if (CsrStrCmp(docType,instData->xmlNoVer) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsMultiplexed(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_5) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsJpeg(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_IMAGE_FORMAT_2) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsGif(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_IMAGE_FORMAT_1) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsText(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_3) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsPs(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_4) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsPcl(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_2) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsVcard(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_6) == 0 ||
        CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_7) == 0)
    {
        result = TRUE;
    }
    return result;
}


static CsrBool typeIsVcalendar(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_8) == 0 ||
        CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_9) == 0)
    {
        result = TRUE;
    }
    return result;
}

static CsrBool typeIsVmessage(appInstanceData* instData, char *docType)
{
    CsrBool result = FALSE;
    if (CsrStrCmp(docType,PRINTER_DOCUMENT_FORMAT_10) == 0)
    {
        result = TRUE;
    }
    return result;
}


void handleCsrBtBppsJobCompleteCfm(appInstanceData *pInst)
{
    printf("job completed\n");
}

static void getListOfRefImg(appInstanceData* instData, char* listOfRefObjects, CsrUint32* refObjCount)
{
    CsrFileHandle *fileHandle;
    char * currentListIndex = listOfRefObjects;
    char tempstring[500] = "";
    char *tempStringPtr;

    fileHandle = CsrBtFopen(instData->printJobName, "rb");
    while(CsrBtFgets(tempstring, 500, fileHandle) != NULL)
    {
        tempStringPtr = tempstring;
        while( (tempStringPtr = strstr(tempStringPtr, "<img src=\"")) !=NULL)
        {
            char*    stringStart;
            CsrUint32 count = 0;
            tempStringPtr += strlen("<img src=\"");
            stringStart = tempStringPtr;

            while (*tempStringPtr != '"')
            {
                tempStringPtr += 1;
                count++;
            }
            *tempStringPtr = '\0';
            strncpy(currentListIndex, stringStart, count +1);
            currentListIndex += count + 1;
            *refObjCount += 1;
            tempStringPtr += 1;
        }
    }
    CsrBtFclose(fileHandle);
}


static void handleCsrBtBppsActivateCfm(appInstanceData* pInst)
{
    playIntro(pInst);
    printf("\n\nBpps Server Activated...\n\n");
}

static void handleCsrBtBppsDeactivateCfm(appInstanceData* pInst)
{
    CsrBtBppsDeactivateCfm* pMsg;

    pMsg = (CsrBtBppsDeactivateCfm*)pInst->pRecvMsg;
    (void)pMsg;

    pInst->target = APP_NONE_TARGET;
    pInst->state = IDLE_STATE;
    printf("Bpps Server Deactivated...\n\n");
    playIntro(pInst);
}

static void handleCsrBtBppsConnectInd(appInstanceData* pInst)
{
    CsrBtBppsConnectInd* pMsg = pInst->pRecvMsg;

    pInst->connectionId = pMsg->connectionId;
    pInst->obexPeerMaxPacketSize = pMsg->obexPeerMaxPacketSize;
    pInst->clientDevAddr = pMsg->deviceAddr;
    printf("Client: %04X:%02X:%06X request connection.\n", pInst->clientDevAddr.nap, pInst->clientDevAddr.uap, pInst->clientDevAddr.lap);

    if (pInst->isAuth && !pInst->sessionIsAuth)
    {
        /* we use bluetooth passkey for obex authentication */
        if (pInst->passkeyLength > 0)
        {
            CsrBtBppsAuthenticateReqSend(0, NULL, pInst->passkeyLength,
                                        (CsrUint8 *)CsrStrDup(pInst->passkey),NULL);
        }
        else
        {
            printf("Enter OBEX passkey: ");
            pInst->keyboardState = OBEX_PASSKEY_KEYBOARD_STATE;
        }
    }
    else
    {
        CsrBtBppsConnectResSend(pMsg->connectionId,
                                CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);

        printf("Connection Accepted.\n");
    }
}

static void handleBppsAuthCfm(appInstanceData* pInst)
{
    pInst->sessionIsAuth = TRUE;
    printf("Authenticate confirm received.\n");
}

static void handleBppsAuthInd(appInstanceData* pInst)
{
    CsrBtBppsAuthenticateInd* pMsg = (CsrBtBppsAuthenticateInd *) pInst->pRecvMsg;

    printf("Authenticate indication received - Sending authenticate response.\n");
    CsrBtBppsAuthenticateResSend(pInst->passkeyLength,
                            (CsrUint8*)CsrStrDup( pInst->passkey),
                            CsrStrDup("CSR BPPS demo"));
    CsrPmemFree(pMsg->realm);
}

static void handleCsrBtBppsNextInd(appInstanceData* pInst)
{
    CsrBtBppsNextInd* prim = pInst->pRecvMsg;

    /* printf("next_ind received!\n"); */
    if (prim)
    {
        CsrFileHandle *fileHandle;

        if (prim->bodyLength > 0)
        {
            fileHandle = CsrBtFopen(pInst->printJobName, "ab");

            if (fileHandle != NULL)
            {
                CsrUint32 bytesWritten;

                bytesWritten = CsrBtFwrite(prim->body, 1,
                                         prim->bodyLength,
                                         fileHandle);

                CsrBtFclose(fileHandle);
            }
        }

        if(prim->finalFlag)
        {
            pInst->printCompleted = TRUE;

            if(pInst->isXml)
            {
                getListOfRefImg(pInst, pInst->listOfRefObjects, &pInst->refObjCount);
                if(pInst->refObjCount > 0)
                {
                    char *objName = CsrPmemAlloc(CsrStrLen((char *)pInst->listOfRefObjects) + 1);

                    CsrStrNCpyZero(objName,(char *)pInst->listOfRefObjects,CsrStrLen((char *)pInst->listOfRefObjects) + 1);

                    pInst->getAllRefObjCompleted = FALSE;
                    CsrBtBppsGetReferencedObjReqSend(objName,0,25000,TRUE,FALSE);

                    pInst->currentObjRetrieval = pInst->listOfRefObjects;
                    printf("Count: %d  : Getting: %s \n", pInst->refObjCount, pInst->listOfRefObjects);

                }
                pInst->isXml = FALSE;
            }
            if (pInst->printCompleted == TRUE &&
                pInst->getAllRefObjCompleted == TRUE)
            {
                if (pInst->autoFinish)
                {
                    printf("Automatically signalling print completion.\n");
                    CsrBtBppsNextResSend(prim->connectionId, FALSE);
                    CsrBtBppsJobCompleteReqSend();
                }
                else {
                    printf("Delayed print completion.\n");
                    pInst->delResConnId = prim->connectionId;
                }
            }
            else
            {
                CsrBtBppsNextResSend(prim->connectionId, FALSE);
            }
        }
        else
        {
            CsrBtBppsNextResSend(prim->connectionId, FALSE);
        }

        /* Remember to clean up the data referenced by all the
         * pointers in the message * when the information has been
         * used * Note that the file name could be stored until the
         * print job is completed
         */
        CsrPmemFree(prim->body);
    }

}

static void handleCsrBtBppsAbortInd(appInstanceData* pInst)
{
}

static void handleCsrBtBppsDisconnectInd(appInstanceData* pInst)
{
    pInst->sessionIsAuth = FALSE;
    printf("Disconnection indication received. BPPS ready for new session\n");
}

static void handleBppsCreateJob(appInstanceData* pInst)
{

    CsrBtBppsCreateJobInd* bppsCreateJobInd = pInst->pRecvMsg;

    if (bppsCreateJobInd)
    {
        /* within here, the jobId and operation status need to be
         * evaluated and send back to the BPPC manager library.
         */
        pInst->jobId += 1;
        CsrBtBppsCreateJobResSend(pInst->jobId, CSR_BT_OBEX_OPERATION_STATUS_CODE_SUCCESSFUL_OK_IGNORED_OR_SUBSTITUTED_ATTRIBUTES, FALSE);

        /* Remember to clean up the data referenced by all the
         * pointers in the message when the information has been used
         */
        CsrPmemFree(bppsCreateJobInd->jobName);
        CsrPmemFree(bppsCreateJobInd->jobOriginatingUserName);
        CsrPmemFree(bppsCreateJobInd->documentFormat);
        CsrPmemFree(bppsCreateJobInd->copies);
        CsrPmemFree(bppsCreateJobInd->sides);
        CsrPmemFree(bppsCreateJobInd->numberUp);
        CsrPmemFree(bppsCreateJobInd->orientationRequested);
        CsrPmemFree(bppsCreateJobInd->mediaSize);
        CsrPmemFree(bppsCreateJobInd->mediaType);
        CsrPmemFree(bppsCreateJobInd->printQuality);
        CsrPmemFree(bppsCreateJobInd->cancelOnLostLink);

    }
}

static void handleCsrBtBppsSendDocumentInd(appInstanceData* pInst)
{
    CsrBtBppsSendDocumentInd* bppsSendDocumentInd = pInst->pRecvMsg;

    if (bppsSendDocumentInd)
    {
        char tempDocType[100];
        char *mime;
        CsrBool append_mime;
        char intToStringbuffer[6];
        CsrFileHandle *fileHandle;

        CsrMemSet(tempDocType, 0, sizeof(tempDocType));
        sprintf(pInst->printJobName, "printjob");

        if (bppsSendDocumentInd->jobId == 0)
        {
            printf("%s received (simple push model method)\n",
                   bppsSendDocumentInd->fileName);
        }
        else
        {
            printf("%s received (job-based transfer method)\n",
                   bppsSendDocumentInd->fileName);
        }

        CsrIntToBase10(pInst->jobId, intToStringbuffer);
        CsrStrCat(pInst->printJobName, intToStringbuffer);

        append_mime = TRUE;
        if(bppsSendDocumentInd->documentTypeLength > 0)
        {
            {
                int i;
                printf("\ndocumentType (in hex): ");
                for(i=0; i<bppsSendDocumentInd->documentTypeLength; i++)
                {
                    printf("0x%02x ", bppsSendDocumentInd->documentType[i]);
                }
                printf("\ndocumentType (printed): %s\n\n", bppsSendDocumentInd->documentType);
            }

            if (bppsSendDocumentInd->docTypeDependentInfo)
            {
                printf("Document Type Dependent Information (printed): %s\n\n", bppsSendDocumentInd->docTypeDependentInfo);
                CsrPmemFree(bppsSendDocumentInd->docTypeDependentInfo);
            }

            CsrStrNCpy(tempDocType,
                    (char*)bppsSendDocumentInd->documentType,
                    bppsSendDocumentInd->documentTypeLength);

            if (typeIsXml(pInst, tempDocType))
            {
                mime = "html";
                printf("Print data received:\n%s\n", bppsSendDocumentInd->documentBody);
                printf("NOTE: The data may not appear correct on the screen. If not, please refer to the printJob-file created\n");
            }
            else if(typeIsMultiplexed(pInst, tempDocType))
            {
                mime = "mx";
            }
            else if(typeIsJpeg(pInst, tempDocType))
            {
                mime = "jpg";
            }
            else if(typeIsGif(pInst, tempDocType))
            {
                mime = "gif";
            }
            else if(typeIsText(pInst, tempDocType))
            {
                mime = "txt";
                printf("Print data received:\n%s\n\n", bppsSendDocumentInd->documentBody);
            }
            else if(typeIsPs(pInst, tempDocType))
            {
                mime = "ps";
            }
            else if(typeIsPcl(pInst, tempDocType))
            {
                mime = "pcl";
            }
            else if(typeIsVcard(pInst, tempDocType))
            {
                mime = "vcf";
                printf("Print data received:\n%s\n\n", bppsSendDocumentInd->documentBody);
            }
            else if(typeIsVcalendar(pInst, tempDocType))
            {
                mime = "vcs";
                printf("Print data received:\n%s\n\n", bppsSendDocumentInd->documentBody);
            }
            else if(typeIsVmessage(pInst, tempDocType))
            {
                mime = "vmg";
                printf("Print data received:\n%s\n\n", bppsSendDocumentInd->documentBody);
            }
            else
            {
                mime = "<unknown>";
                append_mime = FALSE;
            }
        }
        else
        {
            mime = "<unspecified>";
            append_mime = FALSE;
        }


        /* If file type was recongnized, append extension */
        if(append_mime)
        {
            CsrStrCat(pInst->printJobName, ".");
            CsrStrCat(pInst->printJobName, mime);
        }

        fileHandle = CsrBtFopen(pInst->printJobName, "wb");

        if (fileHandle != NULL)
        {
            CsrUint32 bytesWritten;

            bytesWritten = CsrBtFwrite(bppsSendDocumentInd->documentBody, 1,
                                     bppsSendDocumentInd->documentBodyLength,
                                     fileHandle);

            CsrBtFclose(fileHandle);
            printf("\nReceived data saved in file: %s\n\n", pInst->printJobName);
        }

        /* Remember to clean up the data referenced by all the
         * pointers in the message when the information has been used
         * Note that the file name could be stored until the print
         * job is completed
         */

        pInst->printCompleted = FALSE;

        if(typeIsXml(pInst, tempDocType))
        {
            pInst->isXml = TRUE;
        }

        CsrPmemFree(bppsSendDocumentInd->fileName);
        CsrPmemFree(bppsSendDocumentInd->documentBody);
        CsrPmemFree(bppsSendDocumentInd->documentType);
    }
}

static void handleBppsGetPrinterAttribs(appInstanceData* pInst)
{
    CsrBtBppsGetPrinterAttribsInd* BppsGetPrinterAttribsInd = pInst->pRecvMsg;

    /* note: only some of the parameters have been filled in - this is
     * just to demonstrate how to * create the printer's attributes in
     * the CsrBtBppsGetPrinterAttribsInd structure before posting it on to
     * the BPPC manager library.
     */
    if (BppsGetPrinterAttribsInd)
    {
        CsrUint16 index;

        CsrCharString *pageHeight = NULL;
        CsrCharString *pageWidth = NULL;
        CsrCharString *color = NULL;
        CsrCharString *maxCopies = NULL;
        CsrCharString *mediaLoaded = NULL;
        CsrCharString *numberUp = NULL;
        CsrCharString *currentOperator = NULL;
        CsrCharString *printerLocation = NULL;
        CsrCharString *printerName = NULL;
        CsrCharString *printerState = NULL;
        CsrCharString *printerReasons = NULL;
        CsrCharString *queuedJobCount = NULL;

        CsrUint8  *documentFormats = NULL;
        CsrUint8 documentFormatsCount = 0;
        CsrUint8  *imageFormats = NULL;
        CsrUint8 imageFormatsCount = 0;
        CsrUint8  *orientations = NULL;
        CsrUint8 orientationsCount = 0;
        CsrUint8  *sides = NULL;
        CsrUint8 sidesCount = 0;
        CsrUint8  *mediaSizes = NULL;
        CsrUint8 mediaSizesCount = 0;
        CsrUint8  *mediaTypes = NULL;
        CsrUint8 mediaTypesCount = 0;
        CsrUint8  *printQuality = NULL;
        CsrUint8 printQualityCount = 0;
        CsrUint8 mediaLoadedCount = 0;

        /* The trivial */
        if (BppsGetPrinterAttribsInd->printerName)
        {
            printerName = CsrPmemAlloc(CsrStrLen(PRINTER_NAME) + 1);
            strcpy((char*)printerName, PRINTER_NAME);
        }
        if (BppsGetPrinterAttribsInd->basicTextPageWidth)
        {
            pageWidth = CsrPmemAlloc(CsrStrLen(PRINTER_BASIC_WIDTH) + 1);
            strcpy((char*)pageWidth, PRINTER_BASIC_WIDTH);
        }

        if (BppsGetPrinterAttribsInd->basicTextPageHeight)
        {
            pageHeight = CsrPmemAlloc(CsrStrLen(PRINTER_BASIC_HEIGHT) + 1);
            strcpy((char*)pageHeight, PRINTER_BASIC_HEIGHT);
        }

        if (BppsGetPrinterAttribsInd->queuedJobCount)
        {
            queuedJobCount = CsrPmemAlloc(CsrStrLen(PRINTER_QUEUED_JOB_COUNT) + 1);
            strcpy((char*)queuedJobCount, PRINTER_QUEUED_JOB_COUNT);
        }

        if (BppsGetPrinterAttribsInd->printerLocation)
        {
            printerLocation = CsrPmemAlloc(CsrStrLen(PRINTER_LOCATION) + 1);
            strcpy((char*)printerLocation, PRINTER_LOCATION);
        }
        if (BppsGetPrinterAttribsInd->printerState)
        {
            printerState = CsrPmemAlloc(CsrStrLen(PRINTER_STATE)  + 1);
            strcpy((char*)printerState, PRINTER_STATE);
        }
        if (BppsGetPrinterAttribsInd->printerStateReasons)
        {
            printerReasons = CsrPmemAlloc(CsrStrLen(PRINTER_STATE_REASON)  + 1);
            strcpy((char*)printerReasons, PRINTER_STATE_REASON);
        }
        if (BppsGetPrinterAttribsInd->colorSupported)
        {
            color = CsrPmemAlloc(CsrStrLen(PRINTER_COLOR) + 1);
            strcpy((char*)color, PRINTER_COLOR);
        }

        if (BppsGetPrinterAttribsInd->maxCopiesSupported)
        {
            maxCopies = CsrPmemAlloc(CsrStrLen(PRINTER_MAX_COPIES) + 1);
            strcpy((char*)maxCopies, PRINTER_MAX_COPIES);
        }
        if (BppsGetPrinterAttribsInd->numberUpSupported)
        {
            numberUp = CsrPmemAlloc(CsrStrLen(PRINTER_NUMBER_UP) + 1);
            strcpy((char*)numberUp, PRINTER_NUMBER_UP);
        }
        if (BppsGetPrinterAttribsInd->printerGeneralCurrentOperator)
        {
            currentOperator = CsrPmemAlloc(CsrStrLen(PRINTER_OPERATOR) + 1);
            strcpy((char*)currentOperator, PRINTER_OPERATOR);
        }

        /* Multistrings */
        if (BppsGetPrinterAttribsInd->documentFormatsSupported)
        {
            documentFormatsCount = 10;
            documentFormats = CsrPmemAlloc(CsrStrLen(PRINTER_DOCUMENT_FORMAT_1) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_2) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_3) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_4) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_5) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_6) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_7) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_8) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_9) + 1 +
                                      CsrStrLen(PRINTER_DOCUMENT_FORMAT_10) + 1);

            strcpy((char*)documentFormats, PRINTER_DOCUMENT_FORMAT_1);
            index = (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_1);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_2);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_2);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_3);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_3);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_4);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_4);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_5);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_5);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_6);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_6);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_7);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_7);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_8);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_8);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_9);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_9);
            documentFormats[index++] = '\0';

            strcpy((char*)documentFormats + index, PRINTER_DOCUMENT_FORMAT_10);
            index += (CsrUint16)CsrStrLen(PRINTER_DOCUMENT_FORMAT_10);
            documentFormats[index++] = '\0';
        }


        if (BppsGetPrinterAttribsInd->sidesSupported)
        {
            sidesCount = 3;
            sides = CsrPmemAlloc(CsrStrLen(PRINTER_SIDES_1) + 1 +
                            CsrStrLen(PRINTER_SIDES_2) + 1 +
                            CsrStrLen(PRINTER_SIDES_3) + 1);

            strcpy((char*)sides, PRINTER_SIDES_1);
            index = (CsrUint16)CsrStrLen(PRINTER_SIDES_1);
            sides[index++] = '\0';

            strcpy((char*)sides + index, PRINTER_SIDES_2);
            index += (CsrUint16)CsrStrLen(PRINTER_SIDES_2);
            sides[index++] = '\0';

            strcpy((char*)sides + index, PRINTER_SIDES_3);
            index += (CsrUint16)CsrStrLen(PRINTER_SIDES_3);
            sides[index++] = '\0';
        }


        if (BppsGetPrinterAttribsInd->orientationsSupported)
        {
            orientationsCount = 2;
            orientations = CsrPmemAlloc(CsrStrLen(PRINTER_ORIENTATION_1) + 1 +
                                   CsrStrLen(PRINTER_ORIENTATION_2) + 1);

            strcpy((char*)orientations, PRINTER_ORIENTATION_1);
            index = (CsrUint16)CsrStrLen(PRINTER_ORIENTATION_1);
            orientations[index++] = '\0';

            strcpy((char*)orientations + index, PRINTER_ORIENTATION_2);
            index += (CsrUint16)CsrStrLen(PRINTER_ORIENTATION_2);
            orientations[index++] = '\0';
        }

        if (BppsGetPrinterAttribsInd->mediaSizesSupported)
        {
            mediaSizesCount = 2,
                mediaSizes = CsrPmemAlloc(CsrStrLen(PRINTER_MEDIA_SIZES_1) + 1 +
                                     CsrStrLen(PRINTER_MEDIA_SIZES_2) + 1);

            strcpy((char*)mediaSizes, PRINTER_MEDIA_SIZES_1);
            index = (CsrUint16)CsrStrLen(PRINTER_MEDIA_SIZES_1);
            mediaSizes[index++] = '\0';

            strcpy((char*)mediaSizes + index, PRINTER_MEDIA_SIZES_2);
            index += (CsrUint16)CsrStrLen(PRINTER_MEDIA_SIZES_2);
            mediaSizes[index++] = '\0';
        }

        if (BppsGetPrinterAttribsInd->mediaTypesSupported)
        {
            mediaTypesCount = 3;
            mediaTypes = CsrPmemAlloc(CsrStrLen(PRINTER_MEDIA_TYPE_1) + 1 +
                                 CsrStrLen(PRINTER_MEDIA_TYPE_2) + 1 +
                                 CsrStrLen(PRINTER_MEDIA_TYPE_3) + 1);

            strcpy((char*)mediaTypes, PRINTER_MEDIA_TYPE_1);
            index = (CsrUint16)CsrStrLen(PRINTER_MEDIA_TYPE_1);
            mediaTypes[index++] = '\0';

            strcpy((char*)mediaTypes + index, PRINTER_MEDIA_TYPE_2);
            index += (CsrUint16)CsrStrLen(PRINTER_MEDIA_TYPE_2);
            mediaTypes[index++] = '\0';

            strcpy((char*)mediaTypes + index, PRINTER_MEDIA_TYPE_3);
            index += (CsrUint16)CsrStrLen(PRINTER_MEDIA_TYPE_3);
            mediaTypes[index++] = '\0';
        }

        if (BppsGetPrinterAttribsInd->imageFormatsSupported)
        {
            imageFormatsCount = 2;
            imageFormats = CsrPmemAlloc(CsrStrLen(PRINTER_IMAGE_FORMAT_1) + 1 +
                                   CsrStrLen(PRINTER_IMAGE_FORMAT_2) + 1);

            strcpy((char*)imageFormats, PRINTER_IMAGE_FORMAT_1);
            index = (CsrUint16)CsrStrLen(PRINTER_IMAGE_FORMAT_1);
            imageFormats[index++] = '\0';

            strcpy((char*)imageFormats + index, PRINTER_IMAGE_FORMAT_2);
            index += (CsrUint16)CsrStrLen(PRINTER_IMAGE_FORMAT_2);
            imageFormats[index++] = '\0';
        }

        if (BppsGetPrinterAttribsInd->printQualitySupported)
        {
            printQualityCount = 2;
            printQuality = CsrPmemAlloc(CsrStrLen(PRINTER_QUALITY_1) + 1 +
                                   CsrStrLen(PRINTER_QUALITY_2) + 1);

            strcpy((char*)printQuality, PRINTER_QUALITY_1);
            index = (CsrUint16)CsrStrLen(PRINTER_QUALITY_1);
            printQuality[index++] = '\0';

            strcpy((char*)printQuality + index, PRINTER_QUALITY_2);
            index += (CsrUint16)CsrStrLen(PRINTER_QUALITY_2);
            printQuality[index++] = '\0';
        }

        if (BppsGetPrinterAttribsInd->mediaLoaded)
        {
            mediaLoadedCount = 2;
            mediaLoaded = CsrPmemAlloc(CsrStrLen(PRINTER_MEDIA_SIZES_1) + 1 +
                                  CsrStrLen(PRINTER_MEDIA_TYPE_1) + 1 +
                                  CsrStrLen(PRINTER_MEDIA_SIZES_2) + 1 +
                                  CsrStrLen(PRINTER_MEDIA_TYPE_2) + 1);

            strcpy((char*)mediaLoaded, PRINTER_MEDIA_SIZES_1);
            index = (CsrUint16)CsrStrLen(PRINTER_MEDIA_SIZES_1);
            mediaLoaded[index++] = '\0';

            strcpy((char*)mediaLoaded + index, PRINTER_MEDIA_TYPE_1);
            index += (CsrUint16)CsrStrLen(PRINTER_MEDIA_TYPE_1);
            mediaLoaded[index++] = '\0';

            strcpy((char*)mediaLoaded + index, PRINTER_MEDIA_SIZES_2);
            index += (CsrUint16)CsrStrLen(PRINTER_MEDIA_SIZES_2);
            mediaLoaded[index++] = '\0';

            strcpy((char*)mediaLoaded + index, PRINTER_MEDIA_TYPE_2);
            index += (CsrUint16)CsrStrLen(PRINTER_MEDIA_TYPE_2);
            mediaLoaded[index++] = '\0';
        }


        CsrBtBppsGetPrinterAttribsResSend(pageHeight,
                                     pageWidth,
                                     color,
                                     documentFormats,
                                     documentFormatsCount,
                                     imageFormats,
                                     imageFormatsCount,
                                     maxCopies,
                                     mediaLoaded,
                                     mediaSizes,
                                     mediaSizesCount,
                                     mediaTypes,
                                     mediaTypesCount,
                                     numberUp,
                                     orientations,
                                     orientationsCount,
                                     currentOperator,
                                     printerLocation,
                                     printerName,
                                     printerState,
                                     printerReasons,
                                     printQuality,
                                     printQualityCount,
                                     queuedJobCount,
                                     sides,
                                     sidesCount,
                                     mediaLoadedCount,
                                     CSR_BT_OBEX_OPERATION_STATUS_CODE_SUCCESSFUL_OK,
                                     FALSE);
    }
}

static void handleBppsGetJobAttribs(appInstanceData* pInst)
{
    CsrBtBppsGetJobAttribsInd* bppsGetJobAttribsInd = pInst->pRecvMsg;

    if (bppsGetJobAttribsInd)
    {
        char  *jobState = CsrPmemAlloc(CsrStrLen((char *)"printing")+1);
        char  *jobName  = CsrPmemAlloc(CsrStrLen((char*)"MyExpenseReport")+1);
        char  *jobOriginatingUserName = CsrPmemAlloc(CsrStrLen((char*)"mailto:MyEmail")+1);

        CsrStrNCpyZero(jobState,(char *)"printing",CsrStrLen((char *)"printing")+1);
        CsrStrNCpyZero(jobName,(char *)"MyExpenseReport",CsrStrLen((char *)"MyExpenseReport")+1);
        CsrStrNCpyZero(jobOriginatingUserName,(char *)"mailto:MyEmail",CsrStrLen((char *)"mailto:MyEmail")+1);

        CsrBtBppsGetJobAttribsResSend(bppsGetJobAttribsInd->jobId,jobState,jobName,jobOriginatingUserName,2,0,
                                    CSR_BT_OBEX_OPERATION_STATUS_CODE_SUCCESSFUL_OK_IGNORED_OR_SUBSTITUTED_ATTRIBUTES,
                                    FALSE);

        /* Remember to clean up the data referenced by all the pointers in
         * the message when the information has been used
         */
        CsrPmemFree(bppsGetJobAttribsInd->jobAttribs);
    }
}

static void handleBppsCancelJob(appInstanceData* pInst)
{
    CsrBtBppsCancelJobInd* bppsCancelJobInd = pInst->pRecvMsg;

    if (bppsCancelJobInd)
    {
        CsrBtBppsCancelJobResSend(bppsCancelJobInd->jobId,
                    CSR_BT_OBEX_OPERATION_STATUS_CODE_SUCCESSFUL_OK_IGNORED_OR_SUBSTITUTED_ATTRIBUTES);
    }
}

static void handleCsrBtBppsGetReferencedObjInd(appInstanceData* pInst)
{
    CsrBtBppsGetReferencedObjInd* prim = pInst->pRecvMsg;

    char fileName[100];
    CsrFileHandle *fileHandle;
    if(prim->bodyLength > 0)
    {

        sprintf(fileName, "%s", (char*)pInst->currentObjRetrieval);
        if(pInst->firstRefPacket)
        {
            fileHandle = CsrBtFopen(fileName, "wb");
            pInst->firstRefPacket = FALSE;
            if(prim->filesize > 0)
            {
                pInst->refObjSize = prim->filesize;
                printf("Referenced object size: %d [0x%X] \n", pInst->refObjSize, pInst->refObjSize);
            }
        }
        else
        {
            fileHandle = CsrBtFopen(fileName, "ab");
        }
        if (fileHandle != NULL)
        {
            CsrUint32 bytesWritten;

            bytesWritten = CsrBtFwrite(prim->payload + prim->bodyOffset,
                                     1,
                                     prim->bodyLength,
                                     fileHandle);

            CsrBtFclose(fileHandle);
            pInst->numOfBytesReceived += bytesWritten;
        }
    }
    else
    {
        if(pInst->firstRefPacket && prim->filesize > 0)
        {
            pInst->refObjSize = prim->filesize;
            printf("Referenced object size: %d [0x%X] \n", pInst->refObjSize, pInst->refObjSize);
        }
    }
    CsrBtBppsGetReferencedObjResSend(FALSE);
    CsrPmemFree(prim->payload);

}

static void handleCsrBtBppsGetReferencedObjCfm(appInstanceData* pInst)
{
    CsrBtBppsGetReferencedObjCfm* prim = pInst->pRecvMsg;

    char fileName[100];
    CsrFileHandle *fileHandle;
    if(prim->resultCode     == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE  &&
       prim->resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    {
        if(prim->bodyLength > 0)
        {

            sprintf(fileName, "%s", (char*)pInst->currentObjRetrieval);
            fileHandle = CsrBtFopen(fileName, "ab");

            if (fileHandle != NULL)
            {
                CsrUint32 bytesWritten;

                bytesWritten = CsrBtFwrite(prim->payload + prim->bodyOffset,
                                         1,
                                         prim->bodyLength,
                                         fileHandle);
                CsrBtFclose(fileHandle);
                pInst->numOfBytesReceived += bytesWritten;
            }
            CsrPmemFree(prim->payload);
        }
    }
    if(pInst->numOfBytesReceived < pInst->refObjSize)
    {
        char *objName = CsrPmemAlloc(CsrStrLen((char *)pInst->currentObjRetrieval) + 1);
        CsrStrNCpyZero(objName,(char *)pInst->currentObjRetrieval,CsrStrLen((char *)pInst->currentObjRetrieval) + 1);

        CsrBtBppsGetReferencedObjReqSend(objName, pInst->numOfBytesReceived, 25000, TRUE, FALSE);
    }
    else
    {
        pInst->numOfRetrievedObj += 1;
        pInst->numOfBytesReceived = 0;
        pInst->firstRefPacket = TRUE;
        if (pInst->numOfRetrievedObj < pInst->refObjCount)
        {
            char *objName;

            pInst->currentObjRetrieval = pInst->currentObjRetrieval + strlen(pInst->currentObjRetrieval) + 1;

            objName = CsrPmemAlloc(CsrStrLen((char *)pInst->currentObjRetrieval) + 1);
            CsrStrNCpyZero(objName,(char *)pInst->currentObjRetrieval,CsrStrLen((char *)pInst->currentObjRetrieval) + 1);

            CsrBtBppsGetReferencedObjReqSend(objName, 0, 25000, TRUE, FALSE);
            printf("Getting: %s\n", pInst->currentObjRetrieval);
        }
        else
        {
            pInst->numOfRetrievedObj = 0;
            pInst->refObjCount = 0;
            pInst->getAllRefObjCompleted = TRUE;

            if (pInst->autoFinish &&
                pInst->printCompleted == TRUE &&
                pInst->getAllRefObjCompleted == TRUE)
            {
                CsrBtBppsJobCompleteReqSend();
            }
        }
    }
}

static void cstBtBppsSendGetEventRes(appInstanceData* pInst)
{
    CsrCharString *jobState;
    CsrCharString *printerState;
    CsrCharString *printerReasons;
    if (pInst->paperTrayFull == TRUE)
    {
        if (pInst->printCompleted)
        {
            jobState = CsrStrDup("completed");
            printerState = CsrStrDup("idle");
            printerReasons = CsrStrDup("none");
        }
        else
        {
            jobState = CsrStrDup("printing");
            printerState = CsrStrDup("processing");
            printerReasons = CsrStrDup("none");
        }
    }
    else
    {
        jobState = CsrStrDup("stopped");
        printerState = CsrStrDup("stopped");
        printerReasons = CsrStrDup("media-empty");
    }

    CsrBtBppsGetEventResSend(pInst->jobId,
                        jobState,
                        printerState,
                        printerReasons,
                        CSR_BT_OBEX_OPERATION_STATUS_CODE_SUCCESSFUL_OK_IGNORED_OR_SUBSTITUTED_ATTRIBUTES,
                        FALSE);
}

static void handleCsrBtBppsGetEventInd(appInstanceData* pInst)
{
    CsrBtBppsGetEventInd* prim = pInst->pRecvMsg;

    if(prim->updateEvents == TRUE)
    {
        printf("Get event update activated.\n");
        pInst->updateEvent = TRUE;
        cstBtBppsSendGetEventRes(pInst);
    }
    else
    {
        pInst->updateEvent = FALSE;
        printf("Get event update de-activated.\n");
    }
}

/**************************************************************************************************
 *
 *    function to handle CM_WRITE_SCAN_ENABME_CFM primitive
 *
 **************************************************************************************************/

static void handleWriteScanEnableCfm(void *messageData)
{

    CsrBtCmWriteScanEnableCfm *ptr;
    ptr = (CsrBtCmWriteScanEnableCfm *) messageData;

    if (ptr->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && ptr->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        printf("Server changed Online Mode successfully\n");
    }
    else
    {
        printf("ERROR: Server could not change Online Mode. resultCode: 0x%02X and resultSupplier: 0x%02X\n", ptr->resultCode, ptr->resultSupplier);
    }
}

 /**************************************************************************************************
 *
 *    function to handle all keyboard presses (sent as a keyboard event signal).
 *
 **************************************************************************************************/

static void keyMessageService(CsrUint16 m, void* data)
{
    if (_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, keyMessageService, m, data);
}

/**************************************************************************************************
 *
 *    a keyboard event is received.
 *    first check for selection of a found device during inquiry if state is inquiry. If not start
 *    proper action as selected.
 *
 **************************************************************************************************/
static void handleKeybPrim(appInstanceData* pInst)
{
    CsrBtKeyPress_t *key;

    key = (CsrBtKeyPress_t*)pInst->pRecvMsg;
    if (ESCAPE_KEY == key->key)
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
        playIntro(pInst);
    }
    else
    {
        switch (pInst->keyboardState)
        {
            case OBEX_PASSKEY_KEYBOARD_STATE:
                keyboardObexPasskeyStateHandler(pInst, key->key);
                break;
            case PASSKEY_KEYBOARD_STATE:
                handleScKeybPrim(&pInst->scInstData);
                break;
            case IDLE_KEYBOARD_STATE:
                idleState(pInst, key->key);
                break;
            default:
                break;
        }
    }
}

static void keyboardObexPasskeyStateHandler(appInstanceData* pInst, char theCharacter)
{
    if (theCharacter == RETURN_KEY)
    {
        if (!pInst->passkeyLength)
        {
            CsrBtBppsAuthenticateReqSend(0, NULL, 0, NULL, NULL);
        }
        else
        {
            CsrBtBppsAuthenticateReqSend(0, NULL, pInst->passkeyLength,
                                        (CsrUint8 *)CsrStrDup(pInst->passkey),NULL);
        }

        pInst->keyboardState = IDLE_KEYBOARD_STATE;
/*
        system(CLS);
*/
        playIntro(pInst);
    }
    else if (theCharacter == BACKSPACE_KEY)
    {
        if (pInst->passkeyLength > 0)
        {
            pInst->passkeyLength--;
            printf("\b \b");
        }
    }
    else if ((theCharacter >= 32) && (theCharacter < 127))
    {
        if (pInst->passkeyLength < 16)
        {
            pInst->passkey[pInst->passkeyLength++] = theCharacter;
            printf("%c", theCharacter);
        }
        else
        {
            printf("The maximum number of characters is reached. Please press BACKSPACE or RETURN\n");
        }
    }
}

static CsrBool idleState(appInstanceData* pInst, char cKey)
{
    CsrBool isProcessed = TRUE;
    /*CsrUint8 characterRepertoiresSupported[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0xf3};*/
    CsrUtf8String * pMimeType;
    CsrUint8      * pCharsSupported;
    CsrUtf8String * pImgType;
    CsrUtf8String * pServiceType;

    switch(cKey)
    {
        case '1':
            {
                if ( IDLE_STATE == pInst->state )
                {
                    pMimeType = CsrPmemAlloc(CsrStrLen(APP_MIME_TYPE)+1);
                    CsrUtf8StrNCpy(pMimeType,APP_MIME_TYPE,CsrStrLen(APP_MIME_TYPE)+1);
                    pCharsSupported = CsrPmemAlloc(16);
                    CsrMemSet(pCharsSupported,0x00,16);
                    pCharsSupported[14] = 0x11;
                    pCharsSupported[15] = 0xF3;
                    pImgType =  CsrPmemAlloc(CsrStrLen(APP_IMAGE_TYPE)+1);
                    CsrUtf8StrNCpy(pImgType,APP_IMAGE_TYPE,CsrStrLen(APP_IMAGE_TYPE)+1);
                    pServiceType =  CsrPmemAlloc(CsrStrLen(APP_SERVICE_TYPE)+1);
                    CsrUtf8StrNCpy(pServiceType,APP_SERVICE_TYPE,CsrStrLen(APP_SERVICE_TYPE)+1);

                    pInst->state = ACTIVE_STATE;
                    pInst->isAuth = FALSE;
                    CsrBtBppsActivateReqSend(pInst->CsrSchedQid,
                                             pMimeType,
                                             pCharsSupported,
                                             16, /* char rep sup length */
                                             pImgType,
                                             pServiceType,
                                             CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                             0, /*windowSize*/
                                             TRUE); /*enableSrm*/

                    printf("\nActivating server...\n");
                }
                break;
            }
        case '2':
            {
                if ( IDLE_STATE == pInst->state )
                {
                    pMimeType = CsrPmemAlloc(CsrStrLen(APP_MIME_TYPE)+1);
                    CsrUtf8StrNCpy(pMimeType,APP_MIME_TYPE,CsrStrLen(APP_MIME_TYPE)+1);
                    pCharsSupported = CsrPmemAlloc(16);
                    CsrMemSet(pCharsSupported,0x00,16);
                    pCharsSupported[14] = 0x11;
                    pCharsSupported[15] = 0xF3;
                    pImgType =  CsrPmemAlloc(CsrStrLen(APP_IMAGE_TYPE)+1);
                    CsrUtf8StrNCpy(pImgType,APP_IMAGE_TYPE,CsrStrLen(APP_IMAGE_TYPE)+1);
                    pServiceType =  CsrPmemAlloc(CsrStrLen(APP_SERVICE_TYPE)+1);
                    CsrUtf8StrNCpy(pServiceType,APP_SERVICE_TYPE,CsrStrLen(APP_SERVICE_TYPE)+1);

                    pInst->state = ACTIVE_STATE;
                    pInst->isAuth = TRUE;
                    CsrBtBppsActivateReqSend(pInst->CsrSchedQid,
                                             pMimeType,
                                             pCharsSupported,
                                             16,
                                             pImgType,
                                             pServiceType,
                                             CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                             0, /*windowSize*/
                                             TRUE); /*enableSrm*/

                    printf("\nServer started ...\n");
                }
                break;
            }
        case '3':
            {
                if ( IDLE_STATE == pInst->state )
                {
                    handleScKeybPrim(&pInst->scInstData);
                }
                break;
            }
        case '4':
            {
                if ( IDLE_STATE == pInst->state )
                {
                    pInst->state = ACTIVE_STATE;
                    pInst->isAuth = FALSE;

                    pMimeType = CsrPmemAlloc(CsrStrLen(APP_MIME_TYPE)+1);
                    CsrUtf8StrNCpy(pMimeType,APP_MIME_TYPE,CsrStrLen(APP_MIME_TYPE)+1);
                    pCharsSupported = CsrPmemAlloc(16);
                    CsrMemSet(pCharsSupported,0x00,16);
                    pCharsSupported[14] = 0x11;
                    pCharsSupported[15] = 0xF3;
                    pImgType =  CsrPmemAlloc(CsrStrLen(APP_IMAGE_TYPE)+1);
                    CsrUtf8StrNCpy(pImgType,APP_IMAGE_TYPE,CsrStrLen(APP_IMAGE_TYPE)+1);
                    pServiceType =  CsrPmemAlloc(CsrStrLen(APP_SERVICE_TYPE)+1);
                    CsrUtf8StrNCpy(pServiceType,APP_SERVICE_TYPE,CsrStrLen(APP_SERVICE_TYPE)+1);

                    CsrBtBppsActivateReqSend(pInst->CsrSchedQid,
                                             pMimeType,
                                             pCharsSupported,
                                             16, pImgType,
                                             pServiceType,
                                             CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                             0, /*windowSize*/
                                             TRUE); /*enableSrm*/

                    printf("\nServer started ...\n");
                }
                /* If we started in IDLE_STATE, we should now be in ACTIVE_STATE and thus */
                /* the if-statement below should be TRUE */
                if ( ACTIVE_STATE == pInst->state)
                {
                    if ( FALSE == pInst->inPrivateOnlineMode )
                    {
                        CsrBtCmWriteScanEnableReqSend(pInst->CsrSchedQid,
                                                 TRUE,
                                                 FALSE);
                        printf("Setting Server in Private Online Mode....\n");
                        pInst->inPrivateOnlineMode = TRUE;
                    }
                    else
                    {
                        CsrBtCmWriteScanEnableReqSend(pInst->CsrSchedQid,
                                                 FALSE,
                                                 FALSE);
                        printf("Setting Server in Public Online Mode....\n");
                        pInst->inPrivateOnlineMode = FALSE;
                    }
                }
                break;
            }

        case 'f':
        case 'F':
            if (!pInst->autoFinish &&
                pInst->printCompleted == TRUE &&
                pInst->getAllRefObjCompleted == TRUE)
            {
                CsrBtBppsNextResSend(pInst->delResConnId, FALSE);
                CsrBtBppsJobCompleteReqSend();
                printf("`Printing done' signalled.\n");
            }
            break;
        case 'p':
        case 'P':
            {
                pInst->paperTrayFull = pInst->paperTrayFull ? FALSE : TRUE;
                if (pInst->updateEvent)
                {
                    cstBtBppsSendGetEventRes(pInst);
                }
                else
                {
                    ;
                }
                playIntro(pInst);
                break;
            }
        case 'q':
        case 'Q':
            pInst->autoFinish = pInst->autoFinish ? FALSE : TRUE;
            playIntro(pInst);
            break;
        case 'x':
        case 'X':
            printf("Deactivate server\n");
            pInst->state = DEACTIVATION_PENDING_STATE;
            CsrBtBppsDeactivateReqSend();
            break;
        default:
            /* unexpected primitive received */
            printf("####### default in keybPrim handler %x,\n", cKey);
            isProcessed = FALSE;
            break;

    }
    return isProcessed;
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
